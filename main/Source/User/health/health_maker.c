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


/* �ü�, ���� �ܼ� ���� �ð� */
//#define RESIDUAL_TIME           1800U   /*100msec 10 * 60 * 3min = 1800sec */
//#define RESIDUAL_TIME           3000U   /*100msec 10 * 60 * 5min = 3000sec */
#define RESIDUAL_TIME           5400U   /*100msec 10 * 60 * 9min = 5400sec */
#define DEFAULT_RINSE_TIME       300U    /*100msec 10 * 30 = 30sec */


/*����ü �����*/
#define DRAIN_MAX_TIMEOUT       54000U  /*100msec 90min x 60sec x 10 = 90min, �ִ� ��� ���� �ð� */
#define COLD_LOW_STAY_TIME      100U    /*100msec 10 * 10 = 10sec */
DoDrain_T DrainStep;

// ��ȯ ��� ��, �ü� ��ũ���� ����� ���� �ʴ� ���,
// ������ �����ϱ� ���� ��ȯ ������ ���� ��Ų��.
// ��� 50��, 10�� ����, 5�� ���
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

// ��� �ִ� ���� �ð� 
// ��� �ִ� �ð��� �ʰ��ϸ� ��� ���� ���
// Ÿ�̸� �ʰ��̸� TRUE ��ȯ
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
 * �ü� ��ũ ��� ����
 *  - �巹�� ��ũ�� ���ؼ� �ü� ��ũ�� ������ ���°� �ɶ����� ����Ѵ�.
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
        // �巹�� �߼����̰� ���� �������� �� ���, ��� ��긦 ���
        // ��� ��ũ�� ���� ä���.
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

    // �ü��� �������̸� ����..
    if( GetTankLevel(LEVEL_ID_COLD) == LEVEL_LOW ) 
    {
        ResetPlumber();
        return ;
    }

    // �⵿���� ��� �ð�
    // ��� ������ ���� ���̰ų� 
    // ��� ��갡 ��� ���� ������ 
    // Ÿ�̸� �ʱ�ȭ
    if( GetDrainPumpStatus() == ON 
        || (IsOpenValve( VALVE_DRAIN ) == FALSE) )
    {
        ResetPlumber();
        return ;
    }

    // ��� ��갡 OPEN �����ε� ��� ��ũ ���� ������ �̰����̸�
    // ������ ������ ������ �Ǵ���.
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
    /*��� ������� Step*/
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
            // ������ �ü��� ��� ������ �����̸� ���� ���� �̵�
            if(  mu8RoomLevel == LEVEL_LOW && mu8ColdLevel == LEVEL_LOW )
            {
                DrainStep.ResidualTime = RESIDUAL_TIME;
                DrainStep.Step++;
            }
            break;

        case STEP_DRAIN_RESIDUAL:
            // ���� �� �ü� ��ũ �ܼ� ����
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

    Sub_DrainColdTank(mu8RoomLevel);   // ��� ����
    ControlPlumber();

#if( CONFIG_STER_TIME_COUNT == 1 )
    SterSterFeedtDrainTime++;
#endif

    return mStatus;
}
