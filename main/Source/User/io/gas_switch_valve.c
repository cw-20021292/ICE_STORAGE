/**
 * File : gas_switch_valve.c
 * 
 * Gas Switch Valve
*/
#include "api_step_motor.h"

#include "gas_switch_valve.h"
#include "comp.h"

enum
{
    INIT_POWER_ON = 0,
    INIT_WAIT_POWER_ON,

    INIT_MOVE_ZERO,
    INIT_WAIT_ZERO,

    INIT_MOVE_COLD,
    INIT_WAIT_COLD,

    INIT_DONE
};

enum
{
    COLD_POWER_ON = 0,
    COLD_WAIT_POWER_ON,

    COLD_CHECK_POSITION,

    COLD_MOVE,
    COLD_WAIT,

    COLD_DONE
};

enum
{
    ICE_POWER_ON = 0,
    ICE_WAIT_POWER_ON,

    ICE_CHECK_POSITION,

    ICE_MOVE,
    ICE_WAIT,

    ICE_DONE
};

enum
{
    CLOSE_POWER_ON = 0,
    CLOSE_WAIT_POWER_ON,

    CLOSE_CHECK_POSITION,

    CLOSE_MOVE,
    CLOSE_WAIT,

    CLOSE_DONE
};

#define GAS_SWITCH_ID       SM_ID_0
#define GAS_SWITCH_SPEED    SM_SPEED_1

#define GAS_SWITCH_CLOSE_STEP_VAL  (34)
#define GAS_SWITCH_COLD_STEP_VAL   (100)
#define GAS_SWITCH_ICE_STEP_VAL    (195)
#define GAS_SWITCH_END_STEP_VAL    (200)
#define GAS_SWITCH_INIT_STEP_VAL   (-30)

#define POWER_ON_TIME              (10U)
#define COMP_OFF_INIT_TIME         (60U)

SGasSwitchValve_T GasSwitch;

void InitGasSwitch(void)
{
    GasSwitch.Status = GAS_SWITCH_ICE;

    GasSwitch.Mode = 0;
    GasSwitch.InitStep = 0;
    GasSwitch.ColdStep = 0;
    GasSwitch.IceStep = 0;
    GasSwitch.CloseStep = 0;
    GasSwitch.PowerOnTime = POWER_ON_TIME; // @100ms, 1sec

    InitStepVal(GAS_SWITCH_ID);
    SetCurrentStep(GAS_SWITCH_ID, GAS_SWITCH_END_STEP_VAL);
    SetTargetStep(GAS_SWITCH_ID, GAS_SWITCH_END_STEP_VAL);
    SetStepSpeed(GAS_SWITCH_ID, GAS_SWITCH_SPEED);
}

void GetGasSwitchData(SGasSwitchValve_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&GasSwitch, sizeof(GasSwitch) );
}

static void SetMode(U8 mode);
static void ClearMode(U8 mode);
U8 GetGasSwitchStatus(void)
{
    return GasSwitch.Status;
}

static void SetGasSwitchStatus(U8 status)
{
    GasSwitch.Status = status;
}

U8 GetGasSwitchMode(void)
{
    return GasSwitch.Mode;
}

U8 IsGasSwitchMoving(void)
{
    if( GasSwitch.Mode != 0U )
    {
        return TRUE;
    }

    return FALSE;
}

void GasSwitchInit(void)
{
    SetMode(GAS_SWITCH_MODE_INIT);
}

void GasSwitchCold(void)
{
    SetMode(GAS_SWITCH_MODE_COLD);
}

void GasSwitchIce(void)
{
    SetMode(GAS_SWITCH_MODE_ICE);
}

void GasSwitchClose(void)
{
    SetMode(GAS_SWITCH_MODE_CLOSE);
}

static void SetMode(U8 mode)
{
    // Set Mode
    GasSwitch.Mode |= mode;

    if( mode == GAS_SWITCH_MODE_COLD )
    {
        ClearMode(GAS_SWITCH_MODE_ICE);
        ClearMode(GAS_SWITCH_MODE_CLOSE);
    }
    else if( mode == GAS_SWITCH_MODE_ICE )
    {
        ClearMode(GAS_SWITCH_MODE_COLD);
        ClearMode(GAS_SWITCH_MODE_CLOSE);
    }
    else if( mode == GAS_SWITCH_MODE_CLOSE )
    {
        ClearMode(GAS_SWITCH_MODE_ICE);
        ClearMode(GAS_SWITCH_MODE_COLD);
    }

    // Set Init Mode by case
}

static void ClearMode(U8 mode)
{
    GasSwitch.Mode &= ~mode;

    //if( mu8Mode == GAS_SWITCH_MODE_POWER_INIT )
    //{
    //    GasSwitch.InitStep = 0;
    //}
    //else
    if( mode == GAS_SWITCH_MODE_INIT )
    {
        GasSwitch.InitStep = 0;
    }
    else if( mode == GAS_SWITCH_MODE_COLD )
    {
        GasSwitch.ColdStep = 0;
    }
    else if( mode == GAS_SWITCH_MODE_ICE )
    {
        GasSwitch.IceStep = 0;
    }
    else if( mode == GAS_SWITCH_MODE_CLOSE )
    {
        GasSwitch.CloseStep = 0;
    }
}

static void ClearMode(U8 mode);
static U8 InitSwitch(void);
static U8 ColdSwitch(void);
static U8 IceSwitch(void);
static U8 CloseSwitch(void);
static U8 ColdSwitchWithInit(void);
void ControlGasSwitch(void)
{
    U8 ret = FALSE;

    /* CONTROL SMPS 12V */

#if 0
    /* Power Init */
    // 원점을 잡기 위해 초기화 한 후, 초기 위치 설정을 냉수측으로 정의한다.
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_POWER_INIT ) == GAS_SWITCH_MODE_POWER_INIT )
    {
        mu8Ret = InitSwitch();
        if( mu8Ret == TRUE )
        {
            ClearMode( GAS_SWITCH_MODE_POWER_INIT );
            SetGasSwitchStatus( GAS_SWITCH_COLD );
        }
        else
        {
            SetGasSwitchStatus(GAS_SWITCH_BUSY);
        }
        return;
    }
#endif

    /* Init */
    // 초기화를 하고 실제 스텝 위치는 냉수측이지만,
    // 위치 설정은 하지 않는다.
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_INIT ) == GAS_SWITCH_MODE_INIT )
    {
        ret = InitSwitch();
        if( ret == TRUE )
        {
            ClearMode(GAS_SWITCH_MODE_INIT);
            SetGasSwitchStatus(GAS_SWITCH_COLD);
        }
        else
        {
            SetGasSwitchStatus(GAS_SWITCH_BUSY);
        }
        return;
    }


    /* Cold */
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_COLD ) == GAS_SWITCH_MODE_COLD )
    {
        ret = ColdSwitch();
        if( ret == TRUE )
        {
            ClearMode(GAS_SWITCH_MODE_COLD);
            SetGasSwitchStatus(GAS_SWITCH_COLD);
        }
        else
        {
            SetGasSwitchStatus(GAS_SWITCH_BUSY);
        }
        return;
    }

    /* Ice */
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_ICE ) == GAS_SWITCH_MODE_ICE )
    {
        ret = IceSwitch();
        if( ret == TRUE )
        {
            ClearMode(GAS_SWITCH_MODE_ICE);
            SetGasSwitchStatus(GAS_SWITCH_ICE);
        }
        else
        {
            SetGasSwitchStatus(GAS_SWITCH_BUSY);
        }
        return ;
    }

    /* close */
    if( (GasSwitch.Mode & GAS_SWITCH_MODE_CLOSE ) == GAS_SWITCH_MODE_CLOSE )
    {
        ret = CloseSwitch();
        if( ret == TRUE )
        {
            ClearMode(GAS_SWITCH_MODE_CLOSE);
            SetGasSwitchStatus(GAS_SWITCH_CLOSE);
        }
        else
        {
            SetGasSwitchStatus(GAS_SWITCH_BUSY);
        }
        return ;
    }
}

static U8 InitSwitch(void)
{
    switch( GasSwitch.InitStep )
    {
        case INIT_POWER_ON:
            GasSwitch.PowerOnTime = POWER_ON_TIME;
            GasSwitch.InitStep++;
            break;

        case INIT_WAIT_POWER_ON:
            if( GasSwitch.PowerOnTime != 0 )
            {
                GasSwitch.PowerOnTime--;
            }
            else
            {
                GasSwitch.InitStep++;
            }
            break;

        case INIT_MOVE_ZERO:
            SetHoldStep(GAS_SWITCH_ID, 0);
            SetTargetStep(GAS_SWITCH_ID, GAS_SWITCH_INIT_STEP_VAL);
            GasSwitch.InitStep++;
            break;

        case INIT_WAIT_ZERO:
            if( IsDoneMoveStep(GAS_SWITCH_ID) == TRUE )
            {
                GasSwitch.InitStep++;

                InitStepVal(GAS_SWITCH_ID);
            }
            break;

        case INIT_MOVE_COLD:
            SetHoldStep(GAS_SWITCH_ID, 0);
            SetTargetStep(GAS_SWITCH_ID, GAS_SWITCH_COLD_STEP_VAL);
            GasSwitch.InitStep++;
            break;

        case INIT_WAIT_COLD:
            if( IsDoneMoveStep(GAS_SWITCH_ID) == TRUE )
            {
                GasSwitch.InitStep++;
            }
            break;

        case INIT_DONE :
            return TRUE;

        default :
            GasSwitch.InitStep = INIT_POWER_ON;
            break;
    }

    return FALSE;
}

static U8 ColdSwitch(void)
{
    switch( GasSwitch.ColdStep )
    {
        // POWER ON 
        case COLD_POWER_ON:
            GasSwitch.PowerOnTime = POWER_ON_TIME;
            GasSwitch.ColdStep++;
            break;

        case COLD_WAIT_POWER_ON:
            if( GasSwitch.PowerOnTime != 0 )
            {
                GasSwitch.PowerOnTime--;
            }
            else
            {
                GasSwitch.ColdStep++;
            }
            break;


            // MOVE
        case COLD_CHECK_POSITION:
            if( GetTargetStep(GAS_SWITCH_ID) == GAS_SWITCH_COLD_STEP_VAL )
            {
                GasSwitch.ColdStep = COLD_WAIT;
            }
            else
            {
                GasSwitch.ColdStep++;
            }
            break;

        case COLD_MOVE:
            SetHoldStep(GAS_SWITCH_ID, 0);
            SetTargetStep(GAS_SWITCH_ID, GAS_SWITCH_COLD_STEP_VAL);
            GasSwitch.ColdStep++;
            break;

        case COLD_WAIT:
            if( IsDoneMoveStep(GAS_SWITCH_ID) == TRUE )
            {
                GasSwitch.ColdStep++;
            }
            break;

        case COLD_DONE:
            return TRUE;

        default :
            GasSwitch.ColdStep = COLD_POWER_ON;
            break;
    }
    return FALSE;
}


static U8 IceSwitch(void)
{
    switch( GasSwitch.IceStep )
    {
        case ICE_POWER_ON:
            GasSwitch.PowerOnTime = POWER_ON_TIME;
            GasSwitch.IceStep++;
            break;

        case ICE_WAIT_POWER_ON:
            if( GasSwitch.PowerOnTime != 0 )
            {
                GasSwitch.PowerOnTime--;
            }
            else
            {
                GasSwitch.IceStep++;
            }
            break;

        // 현재 위치이면 이동 없이 정상 종료
        case ICE_CHECK_POSITION:
            if( GetTargetStep(GAS_SWITCH_ID) == GAS_SWITCH_ICE_STEP_VAL)
            {
                GasSwitch.IceStep = ICE_WAIT;
            }
            else
            {
                GasSwitch.IceStep++;
            }
            break;

        case ICE_MOVE:
            SetHoldStep(GAS_SWITCH_ID, 0);
            SetTargetStep(GAS_SWITCH_ID, GAS_SWITCH_ICE_STEP_VAL);
            GasSwitch.IceStep++;
            break;

        case ICE_WAIT:
            if( IsDoneMoveStep(GAS_SWITCH_ID) == TRUE )
            {
                GasSwitch.IceStep++;
            }
            break;

        case ICE_DONE:
            return TRUE;

        default :
            GasSwitch.IceStep = ICE_POWER_ON;
            break;

    }

    return FALSE;
}

static U8 CloseSwitch(void)
{
    switch( GasSwitch.CloseStep )
    {
        case CLOSE_POWER_ON:
            GasSwitch.PowerOnTime = POWER_ON_TIME;
            GasSwitch.CloseStep++;
            break;

        case CLOSE_WAIT_POWER_ON:
            if( GasSwitch.PowerOnTime != 0 )
            {
                GasSwitch.PowerOnTime--;
            }
            else
            {
                GasSwitch.CloseStep++;
            }
            break;

        // 현재 위치이면 이동 없이 정상 종료
        case CLOSE_CHECK_POSITION:
            if( GetTargetStep(GAS_SWITCH_ID) == GAS_SWITCH_CLOSE_STEP_VAL )
            {
                GasSwitch.CloseStep = CLOSE_WAIT;
            }
            else
            {
                GasSwitch.CloseStep++;
            }
            break;

        case CLOSE_MOVE:
            SetHoldStep(GAS_SWITCH_ID, 0);
            SetTargetStep(GAS_SWITCH_ID, GAS_SWITCH_CLOSE_STEP_VAL);
            GasSwitch.CloseStep++;
            break;

        case CLOSE_WAIT:
            if( IsDoneMoveStep(GAS_SWITCH_ID) == TRUE )
            {
                GasSwitch.CloseStep++;
            }
            break;

        case CLOSE_DONE:
            return TRUE;

        default :
            GasSwitch.CloseStep = CLOSE_POWER_ON;
            break;

    }

    return FALSE;
}

static U8 ColdSwitchWithInit(void)
{
    switch( GasSwitch.ColdStep )
    {
        // 현재 위치이면 이동 없이 정상 종료
        case 0:
            if( GetTargetStep(GAS_SWITCH_ID) == GAS_SWITCH_COLD_STEP_VAL )
            {
                GasSwitch.ColdStep = 4;
            }
            else
            {
                GasSwitch.ColdStep++;
            }
            break;
        // 원점으로 이동..
        case 1:
            SetHoldStep(GAS_SWITCH_ID, 0);
            SetTargetStep(GAS_SWITCH_ID, -10);
            GasSwitch.ColdStep++;
            break;

            // 원점 이동하고 나서는
            // 변수도 원점 기준으로 초기화 한다.
        case 2:
            if( IsDoneMoveStep(GAS_SWITCH_ID) == TRUE )
            {
                InitStepVal(GAS_SWITCH_ID);
                GasSwitch.ColdStep++;
            }
            break;

            // 이제 냉수 위치로 이동...
        case 3:
            SetHoldStep(GAS_SWITCH_ID, 0);
            SetTargetStep(GAS_SWITCH_ID, GAS_SWITCH_COLD_STEP_VAL);
            GasSwitch.ColdStep++;
            break;

        case 4:
            if( IsDoneMoveStep(GAS_SWITCH_ID) == TRUE )
            {
                GasSwitch.ColdStep++;
            }
            break;

        case 5:
            return TRUE;

    }

    return FALSE;
}

void ProcessGasSwitch(void)
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
                GasSwitchInit();
            }
        }
    }

#if CONFIG_TEST_GAS_SWITCH
    TEST_RepeatGasValve();
#endif
}

/*************************************************************************************************************************
 * TEST
 * 냉매 전환 밸브 위치 반복 테스트 프로그램
 */
#define TEST_DELAY  (0)

SGasSwitchValveTest_T  GasTest;

void TEST_GetGasValveData(SGasSwitchValveTest_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&GasTest, sizeof(GasTest) );
}

void TEST_StartGasValve(void)
{
    GasTest.Run = TRUE;
    GasTest.Delay = 0;
    GasTest.RepeatCount = 0;
}

void TEST_StopGasValve(void)
{
    GasTest.Run = FALSE;
}

void TEST_RepeatGasValve(void)
{
    static U8 toggle = 0;

    if( GasTest.Run == FALSE )
    {
        return ;
    }

    if( toggle != 0 )
    {
        if( GasSwitch.Mode == 0 )
        {
            toggle = 0;

            GasTest.RepeatCount++;
            
            if( GasTest.Delay != 0 )
            {
                GasTest.Delay--;
                return;
            }

            GasSwitchCold();
        }
        else
        {
            GasTest.Delay = TEST_DELAY;
        }
    }
    else
    {
        if( GasSwitch.Mode == 0 )
        {
            toggle = 1;

            if( GasTest.Delay != 0 )
            {
                GasTest.Delay--;
                return;
            }
            GasSwitchIce();
        }
        else
        {
            GasTest.Delay = TEST_DELAY;
        }
    }
}
