/**
 * File : hotgas_switch_valve.c
 * 
 * HotGas Switch Valve
*/
#include "api_step_motor.h"

#include "hotgas_switch_valve.h"
#include "comp.h"

enum
{
    INIT_POWER_ON = 0,
    INIT_WAIT_POWER_ON,

    INIT_MOVE_ZERO,
    INIT_WAIT_ZERO,

    INIT_MOVE_NORMAL,
    INIT_WAIT_NORMAL,

    INIT_DONE
};

enum
{
    NORMAL_POWER_ON = 0,
    NORMAL_WAIT_POWER_ON,

    NORMAL_CHECK_POSITION,

    NORMAL_MOVE,
    NORMAL_WAIT,

    NORMAL_DONE
};

enum
{
    HOT_POWER_ON = 0,
    HOT_WAIT_POWER_ON,

    HOT_CHECK_POSITION,

    HOT_MOVE,
    HOT_WAIT,

    HOT_DONE
};

#define HOTGAS_SWITCH_ID       SM_ID_2
#define HOTGAS_SWITCH_SPEED    SM_SPEED_1

#define HOTGAS_SWITCH_NORMAL_STEP_VAL  (34)
#define HOTGAS_SWITCH_HOT_STEP_VAL     (154)
#define HOTGAS_SWITCH_END_STEP_VAL     (200)
#define HOTGAS_SWITCH_INIT_STEP_VAL    (-30)

#define POWER_ON_TIME                  (10U)
#define COMP_OFF_INIT_TIME             (60U)

SHotGasSwitchValve_T  HotGasSwitch;

void InitHotGasSwitch(void)
{
    HotGasSwitch.Status = HOTGAS_SWITCH_HOT;

#if 0
    HotGasSwitch.Mode = HOTGAS_SWITCH_MODE_POWER_INIT;
#else
    HotGasSwitch.Mode = 0;//HOTGAS_SWITCH_MODE_POWER_INIT;
#endif
    HotGasSwitch.InitStep = 0;
    HotGasSwitch.NormalStep = 0;
    HotGasSwitch.HotStep = 0;
    HotGasSwitch.PowerOnTime = POWER_ON_TIME; // @100ms, 1sec

    InitStepVal(HOTGAS_SWITCH_ID);
    SetCurrentStep(HOTGAS_SWITCH_ID, HOTGAS_SWITCH_END_STEP_VAL);
    SetTargetStep(HOTGAS_SWITCH_ID, HOTGAS_SWITCH_END_STEP_VAL);
    SetStepSpeed(HOTGAS_SWITCH_ID, HOTGAS_SWITCH_SPEED);
}

void GetHotGasSwitchData(SHotGasSwitchValve_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&HotGasSwitch, sizeof(HotGasSwitch) );
}

static void SetMode(U8 mode);
static void ClearMode(U8 mode);
U8 GetHotGasSwitchStatus(void)
{
    return HotGasSwitch.Status;
}

static void SetHotGasSwitchStatus(U8 status)
{
    HotGasSwitch.Status = status;
}

U8 GetHotGasSwitchMode(void)
{
    return HotGasSwitch.Mode;
}

U8 IsHotGasSwitchMoving(void)
{
    if( HotGasSwitch.Mode != 0U )
    {
        return TRUE;
    }

    return FALSE;
}

void HotGasSwitchInit(void)
{
    SetMode(HOTGAS_SWITCH_MODE_INIT);
}
void HotGasSwitchNormal(void)
{
    SetMode(HOTGAS_SWITCH_MODE_NORMAL);
}

void HotGasSwitchHot(void)
{
    SetMode(HOTGAS_SWITCH_MODE_HOT);
}

static void SetMode(U8 mode)
{
    // Set Mode
    HotGasSwitch.Mode |= mode;

    if( mode == HOTGAS_SWITCH_MODE_NORMAL )
    {
        ClearMode(HOTGAS_SWITCH_MODE_HOT);
    }
    else if( mode == HOTGAS_SWITCH_MODE_HOT )
    {
        ClearMode(HOTGAS_SWITCH_MODE_NORMAL);
    }

    // Set Init Mode by case
}

static void ClearMode(U8 mode)
{
    HotGasSwitch.Mode &= ~mode;

    if( mode == HOTGAS_SWITCH_MODE_POWER_INIT )
    {
        HotGasSwitch.InitStep = 0;
    }
    else if( mode == HOTGAS_SWITCH_MODE_INIT )
    {
        HotGasSwitch.InitStep = 0;
    }
    else if( mode == HOTGAS_SWITCH_MODE_NORMAL )
    {
        HotGasSwitch.NormalStep = 0;
    }
    else if( mode == HOTGAS_SWITCH_MODE_HOT )
    {
        HotGasSwitch.HotStep = 0;
    }
}

static void ClearMode(U8 mode);
static U8 InitSwitch(void);
static U8 NormalSwitch(void);
static U8 HotSwitch(void);
static U8 NormalSwitchWithInit(void);
void ControlHotGasSwitch(void)
{
    U8 ret = FALSE;

    /* CONTROL SMPS 12V */

    /* Power Init */
    // 원점을 잡기 위해 초기화 한 후, 초기 위치 설정을 냉수측으로 정의한다.
    if( (HotGasSwitch.Mode & HOTGAS_SWITCH_MODE_POWER_INIT ) == HOTGAS_SWITCH_MODE_POWER_INIT )
    {
        ret = InitSwitch();
        if( ret == TRUE )
        {
            ClearMode(HOTGAS_SWITCH_MODE_POWER_INIT);
            SetHotGasSwitchStatus(HOTGAS_SWITCH_NORMAL);
        }
        else
        {
            SetHotGasSwitchStatus(HOTGAS_SWITCH_BUSY);
        }
        return;
    }

    /* Init */
    // 초기화를 하고 실제 스텝 위치는 방열판측이지만,
    // 위치 설정은 하지 않는다.
    if( (HotGasSwitch.Mode & HOTGAS_SWITCH_MODE_INIT ) == HOTGAS_SWITCH_MODE_INIT )
    {
        ret = InitSwitch();
        if( ret == TRUE )
        {
            ClearMode(HOTGAS_SWITCH_MODE_INIT);
            SetHotGasSwitchStatus(HOTGAS_SWITCH_NORMAL);
        }
        else
        {
            SetHotGasSwitchStatus(HOTGAS_SWITCH_BUSY);
        }
        return;
    }

    /* Normal */
    if( (HotGasSwitch.Mode & HOTGAS_SWITCH_MODE_NORMAL ) == HOTGAS_SWITCH_MODE_NORMAL )
    {
        ret = NormalSwitch();
        if( ret == TRUE )
        {
            ClearMode(HOTGAS_SWITCH_MODE_NORMAL);
            SetHotGasSwitchStatus(HOTGAS_SWITCH_NORMAL);
        }
        else
        {
            SetHotGasSwitchStatus(HOTGAS_SWITCH_BUSY);
        }
        return;
    }

    /* Hot */
    if( (HotGasSwitch.Mode & HOTGAS_SWITCH_MODE_HOT ) == HOTGAS_SWITCH_MODE_HOT )
    {
        ret = HotSwitch();
        if( ret == TRUE )
        {
            ClearMode(HOTGAS_SWITCH_MODE_HOT);
            SetHotGasSwitchStatus(HOTGAS_SWITCH_HOT);
        }
        else
        {
            SetHotGasSwitchStatus(HOTGAS_SWITCH_BUSY);
        }
        return;
    }
}

static U8 InitSwitch(void)
{
    switch( HotGasSwitch.InitStep )
    {
        case INIT_POWER_ON:
            HotGasSwitch.PowerOnTime = POWER_ON_TIME;
            HotGasSwitch.InitStep++;
            break;

        case INIT_WAIT_POWER_ON:
            if( HotGasSwitch.PowerOnTime != 0 )
            {
                HotGasSwitch.PowerOnTime--;
            }
            else
            {
                HotGasSwitch.InitStep++;
            }
            break;

        case INIT_MOVE_ZERO:
            SetHoldStep(HOTGAS_SWITCH_ID, 0);
            SetTargetStep(HOTGAS_SWITCH_ID, HOTGAS_SWITCH_INIT_STEP_VAL);
            HotGasSwitch.InitStep++;
            break;

        case INIT_WAIT_ZERO:
            if( IsDoneMoveStep(HOTGAS_SWITCH_ID) == TRUE )
            {
                HotGasSwitch.InitStep++;

                InitStepVal(HOTGAS_SWITCH_ID);
            }
            break;

        case INIT_MOVE_NORMAL:
            SetHoldStep(HOTGAS_SWITCH_ID, 0);
            SetTargetStep(HOTGAS_SWITCH_ID, HOTGAS_SWITCH_NORMAL_STEP_VAL);
            HotGasSwitch.InitStep++;
            break;

        case INIT_WAIT_NORMAL:
            if( IsDoneMoveStep(HOTGAS_SWITCH_ID) == TRUE )
            {
                HotGasSwitch.InitStep++;
            }
            break;

        case INIT_DONE :
            return TRUE;

        default :
            HotGasSwitch.InitStep = INIT_POWER_ON;
            break;
    }

    return FALSE;
}

static U8 NormalSwitch(void)
{
    switch( HotGasSwitch.NormalStep )
    {
        // POWER ON 
        case NORMAL_POWER_ON:
            HotGasSwitch.PowerOnTime = POWER_ON_TIME;
            HotGasSwitch.NormalStep++;
            break;

        case NORMAL_WAIT_POWER_ON:
            if( HotGasSwitch.PowerOnTime != 0 )
            {
                HotGasSwitch.PowerOnTime--;
            }
            else
            {
                HotGasSwitch.NormalStep++;
            }
            break;


            // MOVE
        case NORMAL_CHECK_POSITION:
            if( GetTargetStep(HOTGAS_SWITCH_ID) == HOTGAS_SWITCH_NORMAL_STEP_VAL )
            {
                HotGasSwitch.NormalStep = NORMAL_WAIT;
            }
            else
            {
                HotGasSwitch.NormalStep++;
            }
            break;

        case NORMAL_MOVE:
            SetHoldStep(HOTGAS_SWITCH_ID, 0);
            SetTargetStep(HOTGAS_SWITCH_ID, HOTGAS_SWITCH_NORMAL_STEP_VAL);
            HotGasSwitch.NormalStep++;
            break;

        case NORMAL_WAIT:
            if( IsDoneMoveStep(HOTGAS_SWITCH_ID) == TRUE )
            {
                HotGasSwitch.NormalStep++;
            }
            break;

        case NORMAL_DONE:
            return TRUE;

        default :
            HotGasSwitch.NormalStep = NORMAL_POWER_ON;
            break;
    }
    return FALSE;
}


static U8 HotSwitch(void)
{
    switch( HotGasSwitch.HotStep )
    {
        case HOT_POWER_ON:
            HotGasSwitch.PowerOnTime = POWER_ON_TIME;
            HotGasSwitch.HotStep++;
            break;

        case HOT_WAIT_POWER_ON:
            if( HotGasSwitch.PowerOnTime != 0 )
            {
                HotGasSwitch.PowerOnTime--;
            }
            else
            {
                HotGasSwitch.HotStep++;
            }
            break;

        // 현재 위치이면 이동 없이 정상 종료
        case HOT_CHECK_POSITION:
            if( GetTargetStep(HOTGAS_SWITCH_ID) == HOTGAS_SWITCH_HOT_STEP_VAL )
            {
                HotGasSwitch.HotStep = HOT_WAIT;
            }
            else
            {
                HotGasSwitch.HotStep++;
            }
            break;

        case HOT_MOVE:
            SetHoldStep(HOTGAS_SWITCH_ID, 0);
            SetTargetStep(HOTGAS_SWITCH_ID, HOTGAS_SWITCH_HOT_STEP_VAL);
            HotGasSwitch.HotStep++;
            break;

        case HOT_WAIT:
            if( IsDoneMoveStep(HOTGAS_SWITCH_ID) == TRUE )
            {
                HotGasSwitch.HotStep++;
            }
            break;

        case HOT_DONE:
            return TRUE;

        default :
            HotGasSwitch.HotStep = HOT_POWER_ON;
            break;

    }

    return FALSE;
}

static U8 NormalSwitchWithInit(void)
{
    switch( HotGasSwitch.NormalStep )
    {
        // 현재 위치이면 이동 없이 정상 종료
        case 0:
            if( GetTargetStep(HOTGAS_SWITCH_ID) == HOTGAS_SWITCH_NORMAL_STEP_VAL )
            {
                HotGasSwitch.NormalStep = 4;
            }
            else
            {
                HotGasSwitch.NormalStep++;
            }
            break;
        // 원점으로 이동..
        case 1:
            SetHoldStep(HOTGAS_SWITCH_ID, 0);
            SetTargetStep(HOTGAS_SWITCH_ID, -10);
            HotGasSwitch.NormalStep++;
            break;

            // 원점 이동하고 나서는
            // 변수도 원점 기준으로 초기화 한다.
        case 2:
            if( IsDoneMoveStep(HOTGAS_SWITCH_ID) == TRUE )
            {
                InitStepVal(HOTGAS_SWITCH_ID);
                HotGasSwitch.NormalStep++;
            }
            break;

            // 이제 냉수 위치로 이동...
        case 3:
            SetHoldStep(HOTGAS_SWITCH_ID, 0);
            SetTargetStep(HOTGAS_SWITCH_ID, HOTGAS_SWITCH_NORMAL_STEP_VAL);
            HotGasSwitch.NormalStep++;
            break;

        case 4:
            if( IsDoneMoveStep(HOTGAS_SWITCH_ID) == TRUE )
            {
                HotGasSwitch.NormalStep++;
            }
            break;

        case 5:
            return TRUE;

    }

    return FALSE;
}

void ProcessHotGasSwitch(void)
{
    static U8 prevCmd = OFF;
    static U8 runInit = TRUE;
    U8 curCmd = OFF;

    curCmd = GetCompOnOff();
    if( prevCmd != curCmd )
    {
        prevCmd = curCmd;
        if( curCmd == OFF )
        {
            runInit = TRUE;  // 원점 초기화 설정 
        }
    }

    // 압축기 OFF 후, 3분 후에 원점 초기화 동작
    if( runInit == TRUE )
    {
        if( IsExpiredCompProtectTime() == FALSE )
        {
            // 3분 후..
            if( GetCompProtectOffTime() < COMP_OFF_INIT_TIME )
            {
                runInit = FALSE;
                HotGasSwitchInit();
            }
        }
    }

#if( CONFIG_TEST_HOTGAS_SWITCH == 1 )
    TEST_RepeatHotGasValve();
#endif
}


/*************************************************************************************************************************
 * TEST
 * 핫가스 냉매 전환 밸브 위치 반복 테스트 프로그램
 */
#define TEST_DELAY  (0)
SHotGasSwitchValveTest_T  HotGasTest;

void TEST_GetHotGasValveData(SHotGasSwitchValveTest_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&HotGasTest, sizeof(HotGasTest) );
}

void TEST_StartHotGasValve(void)
{
    HotGasTest.Run = TRUE;
    HotGasTest.Delay = 0;
    HotGasTest.RepeatCount = 0;
}

void TEST_StopHotGasValve(void)
{
    HotGasTest.Run = FALSE;
}

void TEST_RepeatHotGasValve(void)
{
    static U8 mu8Toggle = 0;

    if( HotGasTest.Run == FALSE )
    {
        return ;
    }

    if( mu8Toggle != 0 )
    {
        if( HotGasSwitch.Mode == 0 )
        {
            mu8Toggle = 0;

            HotGasTest.RepeatCount++;
            
            if( HotGasTest.Delay != 0 )
            {
                HotGasTest.Delay--;
                return;
            }

            HotGasSwitchNormal();
        }
        else
        {
            HotGasTest.Delay = TEST_DELAY;
        }
    }
    else
    {
        if( HotGasSwitch.Mode == 0 )
        {
            mu8Toggle = 1;

            if( HotGasTest.Delay != 0 )
            {
                HotGasTest.Delay--;
                return;
            }
            HotGasSwitchHot();
        }
        else
        {
            HotGasTest.Delay = TEST_DELAY;
        }
    }
}
