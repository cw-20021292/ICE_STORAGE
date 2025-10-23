#include "health_maker.h"

#include "api_adc.h"
#include "level.h"
#include "valve.h"
#include "drain.h"
#include "drain_pump.h"
#include "ster.h"
#include "room_water.h"
#include "cold_water.h"
#include "ice.h"
#include "circulate_pump.h"
#include "tank_clean.h"


/* 냉수, 정수 잔수 제거 시간 */
//#define RESIDUAL_TIME           1800U   /*100msec 10 * 60 * 3min = 1800sec */
//#define RESIDUAL_TIME           3000U   /*100msec 10 * 60 * 5min = 3000sec */
#define RESIDUAL_TIME           5400U   /*100msec 10 * 60 * 9min = 5400sec */
#define DEFAULT_RINSE_TIME       300U    /*100msec 10 * 30 = 30sec */


/*구조체 선언부*/
#define DRAIN_MAX_TIMEOUT       54000U  /*100msec 90min x 60sec x 10 = 90min, 최대 배수 동작 시간 */
#define COLD_LOW_STAY_TIME      100U    /*100msec 10 * 10 = 10sec */
DoDrain_T DrainStep;

// 순환 살균 중, 냉수 탱크에서 배수가 되지 않는 경우,
// 에러를 제거하기 위해 순환 펌프를 동작 시킨다.
// 대기 50분, 10초 동작, 5분 대기
#define PLUMBER_TIME                6000UL   // @100ms, 10min 
#define PLUMBER_MAX_TIME            6000UL   // @100ms, 10min 
Plumber_T Plumber;

static void StartPlumber(void);
static void StopPlumber(void);
static void ControlPlumber(void);

void InitHealthMaker(void)
{
    InitHealthDrain();
}

void InitHealthDrain(void)
{
    DrainStep.Step = 0;
#if( CONFIG_USE_DRAIN_TIMEOUT == 1 )
    DrainStep.ExitTime = DRAIN_MAX_TIMEOUT;
#endif
    DrainStep.StayTime = COLD_LOW_STAY_TIME; 

    CloseValve( VALVE_DRAIN );
    StopPlumber();
}

DrainStep_T GetHealthDrain(void)
{
    return DrainStep.Step;
}

void GetHealthDrainData(DoDrain_T *pDrain)
{
    MEMCPY( (void __FAR *)pDrain, (const void __FAR *)&DrainStep, sizeof( DoDrain_T ));
}

void UpdateHealthDrainTime(void)
{
    DrainStep.ExitTime = 0U;
}

// 배수 최대 동작 시간 
// 배수 최대 시간을 초과하면 살균 동작 취소
// 타이머 초과이면 TRUE 반환
static U8 CheckDrainTimeOut( void )
{
#if( CONFIG_USE_DRAIN_TIMEOUT == 1 )
    if( DrainStep.ExitTime != 0 )
    {
        DrainStep.ExitTime--;

        return FALSE;
    }

    return TRUE;
#else
    return FALSE;
#endif
}

#if( CONFIG_STER_TIME_COUNT == 1 )
//@ 100ms
U32 SterSterFeedtDrainTime = 0;
#endif

/*
 * 냉수 탱크 배수 제어
 *  - 드레인 탱크를 통해서 냉수 탱크가 저수위 상태가 될때까지 배수한다.
 */
#define MELTWATER_LEVEL_TIME              800     /*100msec *  80 = 800 80sec*/
static void Sub_DrainColdTank(U8 mu8RoomLevel)
{
    static U16 mu16WaitTime = 0U;


    if ( IsDetectTankLevel( LEVEL_ID_DRAIN, LEVEL_DETECT_LOW ) == FALSE 
            && IsDetectTankLevel( LEVEL_ID_DRAIN, LEVEL_DETECT_HIGH ) == FALSE )
    {
        OpenValve(VALVE_DRAIN);
        TurnOffDrainPump();
        mu16WaitTime = MELTWATER_LEVEL_TIME;
    }
    else if( IsDetectTankLevel( LEVEL_ID_DRAIN, LEVEL_DETECT_HIGH ) == TRUE )
    {
        CloseValve(VALVE_DRAIN);
        TurnOnDrainPump();
        mu16WaitTime = MELTWATER_LEVEL_TIME;
    }
    else
    {
        // 드레인 중수위이고 정수 저수위일 때 잠깐, 잠깐 밸브를 열어서
        // 배수 탱크의 물을 채운다.
        if( mu16WaitTime != 0 )
        {
            if( mu8RoomLevel == LEVEL_LOW )
            {
                OpenValve(VALVE_DRAIN);
            }
            mu16WaitTime--;           
        }
        else
        {
            TurnOnDrainPump();
        }
        
    }
}


void GetPlumberData( Plumber_T *pPlumber )
{
    MEMCPY( (void __FAR *)pPlumber, (const void __FAR *)&Plumber, sizeof( Plumber_T ));
}

void UpdatePlumberWaitTime(void)
{
    Plumber.WaitTime = 0;
}

void UpdatePlumberMaxTime(void)
{
    Plumber.MaxTime = 0;
}

static void StartPlumber(void)
{
#if( CONFIG_USE_CIRCULATE_PUMP == 1 )
    Plumber.Run      = TRUE;
    Plumber.WaitTime = PLUMBER_TIME;
    Plumber.MaxTime  = PLUMBER_MAX_TIME;
#endif

}

static void StopPlumber(void)
{
#if( CONFIG_USE_CIRCULATE_PUMP == 1 )
    Plumber.Run = FALSE;
    TurnOffCirculatePumpMakeColdForced();
#endif
}

static void ResetPlumber(void)
{
    Plumber.WaitTime = PLUMBER_TIME;
    Plumber.MaxTime = PLUMBER_MAX_TIME;

    TurnOffCirculatePumpMakeColdForced();
}

static void ControlPlumber(void)
{

#if( CONFIG_USE_CIRCULATE_PUMP == 1 )
    if( Plumber.Run == FALSE )
    {
        return ;
    }

    // 냉수가 저수위이면 종료..
    if( GetTankLevel(LEVEL_ID_COLD) == LEVEL_LOW ) 
    {
        ResetPlumber();
        return ;
    }

    // 출동까지 대기 시간
    // 배수 펌프가 동작 중이거나 
    // 배수 밸브가 모두 닫혀 있으면 
    // 타이머 초기화
    if( GetDrainPumpStatus() == ON 
        || (IsOpenValve( VALVE_DRAIN ) == FALSE) )
    {
        ResetPlumber();
        return ;
    }

    // 배수 밸브가 OPEN 상태인데 배수 탱크 수위 센서가 미감지이면
    // 수막이 형성된 것으로 판단함.
    if( IsOpenValve( VALVE_DRAIN ) == TRUE )
    {
        if( IsDetectTankLevel( LEVEL_ID_DRAIN, LEVEL_DETECT_HIGH ) == FALSE
            &&  IsDetectTankLevel( LEVEL_ID_DRAIN, LEVEL_DETECT_LOW ) == FALSE )
        {
            if( Plumber.WaitTime != 0 )
            {
                Plumber.WaitTime--;
            }
            else
            {
                if( Plumber.MaxTime != 0 )
                {
                    Plumber.MaxTime--;
                    TurnOnCirculatePumpMakeColdForced();
                }
                else
                {
                    TurnOffCirculatePumpMakeColdForced();
                }
            }
        }
        else
        {
            ResetPlumber();
        }
    }
#endif
}

HMStatus_T DoHealthDrain(void)
{
    /*살균 배수동작 Step*/
    HMStatus_T mStatus = STATUS_GOING;
    U8 mu8RoomLevel;
    U8 mu8ColdLevel;

    mu8RoomLevel = GetTankLevel( LEVEL_ID_ROOM );
    mu8ColdLevel = GetTankLevel( LEVEL_ID_COLD );

    switch(DrainStep.Step)
    {
        case STEP_DRAIN_READY:
            DrainStep.ResidualTime = RESIDUAL_TIME; 
            DrainStep.StayTime = COLD_LOW_STAY_TIME;
            StartPlumber();
            DrainStep.Step++;
            break;

        case STEP_DRAIN:
            // 정수와 냉수가 모두 저수위 조건이면 다음 스텝 이동
            if(  mu8RoomLevel == LEVEL_LOW && mu8ColdLevel == LEVEL_LOW )
            {
                DrainStep.ResidualTime = RESIDUAL_TIME;
                DrainStep.Step++;
            }
            break;

        case STEP_DRAIN_RESIDUAL:
            // 정수 및 냉수 탱크 잔수 제거
            if( DrainStep.ResidualTime != 0 )
            {
                DrainStep.ResidualTime--;
            }
            else
            {
                DrainStep.Step++;
            }
            break;

        case STEP_DRAIN_DONE:
            StopPlumber();
            mStatus = STATUS_DONE;
            break;

        default:
            DrainStep.Step = 0;
            break;
    }

    Sub_DrainColdTank(mu8RoomLevel);   // 배수 동작
    ControlPlumber();

#if( CONFIG_STER_TIME_COUNT == 1 )
    SterSterFeedtDrainTime++;
#endif

    return mStatus;
}
