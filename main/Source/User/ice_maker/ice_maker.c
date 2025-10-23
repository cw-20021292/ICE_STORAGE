#include "ice_maker.h"

#include "ice.h"
#include "ice_full.h"
#include "ice_low.h"
#include "ice_tray.h"
#include "icing_table.h"
#include "deicing_table.h"
#include "dummy_deicing_table.h"
#include "preheat_table.h"

#include "drain.h"
#include "room_water.h"
#include "error.h"

#include "temp.h"
#include "relay.h"
#include "valve.h"
#include "comp.h"
#include "drain_pump.h"
#include "gas_switch_valve.h"
#include "hotgas_switch_valve.h"
#include "swing_bar.h"
#include "uv.h"
#include "cold_water.h"
#include "circulate_pump.h"

#include "ice_door.h"

#include "amount.h"
#include "water_out.h"
#include "ice_water_out.h"
#include "drain_water.h"
#include "flow_meter.h"

#include "api_fan_motor.h"

#define ICE_TRAY_FEED_TIME          (60U)      /* �ִ� ���̽� Ʈ���� �Լ� �ð� :  @1sec */
//#define  ICE_TRAY_FEED_TIME         (50U)    /* �ִ� ���̽� Ʈ���� �Լ� �ð� :  @1sec */

#define ROOM_OUT_TIME_OUT_TIME      (240)      /* ���� ��� Ÿ�� �ƿ� : 4min  @1sec */
#define GAS_SWITCH_TIME_OUT         (300U)     /* �ø� ��ȯ ��� ���  �ð� : @1sec */

#define GAS_CLOSE_SWITCH_WAIT_TIME  (10U)      /* �ø� ��ȯ ��� ��ȯ ��� �ð� : @1sec */

/* COMP RPS */
#define COMP_ICING_RPS               (55U)

/* REDUCE COMP. DE-ICING NOISE */
#define TIME_REDUCE_DEICING_NOISE    (600U)    // 60sec @100ms

DoInit_T Init;
SDoPreheat_T Preheat;
DoReady_T Ready;
DoIcing_T Icing;
SDoDeIcing_T DeIcing;
SDoDummyDeIcing_T DummyDeIcing;
DoIcingMoveDeIcing_T IcingMoveDeicing;

IcingTemp_T IcingTemp;

void GetIceMakerInitData( DoInit_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Init, sizeof( DoInit_T ));
}

void GetIceMakerPreheatData(SDoPreheat_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Preheat, sizeof(SDoPreheat_T) );
}

void GetIceMakerReadyData( DoReady_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Ready, sizeof( DoReady_T ));
}

void GetIceMakerIcingData( DoIcing_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Icing, sizeof( DoIcing_T ));
}

void GetIceMakerDeIcingData(SDoDeIcing_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&DeIcing, sizeof(SDoDeIcing_T) );
}

void GetIceMakerDummyDeIcingData(SDoDummyDeIcing_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&DummyDeIcing, sizeof( SDoDummyDeIcing_T ) );
}

void GetIceMakerIcingMoveDeIcingData( DoIcingMoveDeIcing_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&IcingMoveDeicing, sizeof( DoIcingMoveDeIcing_T ));
}

void SetIcingTime(U16 mu16Val )
{
    Icing.IcingTime = mu16Val;
}

void SetDeIcingTime(U16 mu16Val )
{
    DeIcing.OnOffTime = mu16Val;
}

// ���� Ż�� ����
void StopDummyDeIcing(void)
{
    // ���� Ż�� ���̸�, ������ �������� �̵��Ѵ�.
    if( IsSetIceMakeMode( ICE_MODE_DUMMY_DEICING ) == TRUE )
    {
        DummyDeIcing.Step = STEP_DUMMY_DEICING_DONE_CONTROL;
    }
}

static void InitDoInit(void)
{
    Init.Step = 0; 
}

static void InitDoPreheat(void)
{
    Preheat.Step = 0; 
}

static void InitDoReady(void)
{
    Ready.Step = 0;
}

static void InitDoIcing(void)
{
    Icing.Step = 0;
}

static void InitDoDeIcing(void)
{
    DeIcing.Step = 0;
}

static void InitDoDummyDeIcing(void)
{
    DummyDeIcing.Step = 0;
}

static void InitDoIcingMoveDeicing(void)
{
    IcingMoveDeicing.Step = 0;
}

static U8 DoInit(void)
{
    U8 mu8Done = FALSE;


    switch( Init.Step )
    {
        /* 1. ���̽� Ʈ���� �̵�
         * 2. �巹�� ��ũ ��� ���� 
         */
        case STEP_INIT_TRAY_MOVE:
            MoveIceTrayDeIcing();

            Init.Step++;
            break;

            /* ���̽� Ʈ���� �̵� �Ϸ� */
        case STEP_INIT_TRAY_DEICING:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                Init.Step++;
            }
            break;

        case STEP_INIT_TRAY_DONE:
            mu8Done = TRUE;
            break;

        default:
            Init.Step = 0;
            break;
    }

    return mu8Done;
}

/* PREHEAT TIME / COMP RPS / BASE TEMP */
//#define PREHEAT_TIME        1800U    // 3min ( 3min x 60sec x 10 ) = 1800sec @100msec )
//#define PREHEAT_COMP_RPS    60U     // 60RPS
#define PREHEAT_BASE_TEMP   20.0f   // ������ �ʿ��� ������ ���� �µ� ( 20�� �̸� )

static void StartPreheat( U8 mu8Region )
{
    Preheat.Region = mu8Region;

    Preheat.Count      = 0;
    Preheat.RepeatNum  = GetPreheatRepeatNum( mu8Region );
}

static U8 MakePreheatOnOff(void)
{
    if( Preheat.Count >= Preheat.RepeatNum )
    {
        Preheat.Count = 0;
        return FALSE;
    }

    Preheat.OnOffCmd  = GetPreheatCmd( Preheat.Count );
    Preheat.PreHeatTime = GetPreheatTime( Preheat.Region, Preheat.Count );
    Preheat.CompRps   = GetPreheatCompRps( Preheat.Region );

    Preheat.Count++;

    return TRUE;
}

static U8 DoPreheat(void)
{
    U8 done = FALSE;

    switch( Preheat.Step )
    {
        case STEP_PREHEAT_CHECK:
            // �ܱ�µ� ���� �� 20�� �̻� �� Preheat ���� ����
            if( IsValidDoIcingAmbiTempRange() == FALSE 
                || IsError(ERR_ID_TEMP_AMBIENT_2 ) == TRUE  )
            {
                ClearIceMakeMode( ICE_MODE_ALL );
                SetIceMakeMode( ICE_MODE_INIT );

                TurnOffComp(COMP_TYPE_ICE);

                Preheat.Step = STEP_PREHEAT_DONE;
            }
            else
            {
              {
                  TEMP_T  tAmbi;
                  U8      mu8Region;

                  tAmbi = GetTemp( TEMP_ID_AMBIENT_2 );

                  if( tAmbi < PREHEAT_BASE_TEMP )
                  {
                      mu8Region = GetPreheatRegion( tAmbi );
                      StartPreheat( mu8Region );
                      Preheat.Step++;
                  }
                  else
                  {
                      Preheat.Step = STEP_PREHEAT_DONE;
                  }
              }
            }
            break;
    
        case STEP_PREHEAT_CHECK_COMP:
            // Comp ��ȣ�ð� üũ
            if( IsExpiredCompProtectTime() == TRUE )
            {
                Preheat.Step++;
            }
            break;

        case STEP_PREHEAT_INIT:
            // PREHEAT RPS SET
            // �ְ��� �ø� ��ȯ FULL CLOSE
            //Preheat.CompRps = PREHEAT_COMP_RPS;

            HotGasSwitchHot();

            Preheat.Step++;
            break;

        case STEP_PREHEAT_HOTGAS_SWITCH_CHECK:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_HOT )
            {
                Preheat.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                Preheat.Step++;
            }
            break;


        case STEP_PREHEAT_GAS_CLOSE_SWITCH_WAIT:
            if( Preheat.GasSwitchWaitTime != 0)
            {
                Preheat.GasSwitchWaitTime--;
            }

            if( Preheat.GasSwitchWaitTime == 0)
            {
                GasSwitchClose();
                Preheat.Step++;
            }
            break;          

        case STEP_PREHEAT_START:
            // COMP ON / PREHEAT TIME SET
            if( GetGasSwitchStatus() == GAS_SWITCH_CLOSE )
            {
                //TurnOnCompRps( COMP_TYPE_ICE, Preheat.CompRps );

                //Preheat.PreHeatTime = PREHEAT_TIME;
                //Preheat.Step++;

                if( MakePreheatOnOff() == TRUE )
                {
                    Preheat.Step++;
                }
            }
            break;

        case STEP_PREHEAT_WAIT:
            // PREHEAT TIME WAIT
            if( Preheat.PreHeatTime == 0)
            {
                //Preheat.Step++;

                if( MakePreheatOnOff() == FALSE )
                {
                    GasSwitchIce();
                    Preheat.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                    Preheat.Step++;
                    break;
                }
            }

            if( Preheat.PreHeatTime != 0)
            {
                if( Preheat.OnOffCmd == ON )
                {
                    TurnOnCompRps(COMP_TYPE_ICE, Preheat.CompRps);
                    TurnOffFan(FAN_TYPE_ICE);
                    //TurnOffFan();
                }
                else
                {
                    // Do Nothing...
                }
            }
            break;

        case STEP_PREHEAT_GAS_ICING_SWITCH_CHECK:
            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                Preheat.Step++;
            }
            break;


        case STEP_PREHEAT_GAS_ICING_SWITCH_WAIT:
            if( Preheat.GasSwitchWaitTime != 0 )
            {
                Preheat.GasSwitchWaitTime--;
            }

            if( Preheat.GasSwitchWaitTime == 0 )
            {
                HotGasSwitchNormal();
                Preheat.Step++;
            }
            break;

        case STEP_PREHEAT_DONE_CONTROL:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL )
            {
                Preheat.Step++;
            }
            break;

        case STEP_PREHEAT_DONE:
            done = TRUE;
            break;

        default:
            Preheat.Step = 0;
            break;
    }

    return done;
}

/* ���� ���� ī���� */
static void UpdateIceTrayCurrentAmount(void)
{
    if( IsOpenValve(VALVE_ICE_TRAY_IN) == TRUE 
            && Ready.TrayFeedAmount != 0UL )
    {
        Ready.TrayFeedAmount--;
    }
    else if( Ready.TrayFeedAmount == 0UL )
    {
        OpenValve(VALVE_NOS_FLOW);
        CloseValve(VALVE_ICE_TRAY_IN);
        TurnOffCirculatePumpTrayIn();
    }
}

static IsUsingWater(void)
{
    if( GetWaterOut() == TRUE )
    {
        return TRUE;
    }

    if( IsStartIceDoorWater() == TRUE )
    {
        return TRUE;
    }

    if( IsStartDrainWater() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

static void StartFeedIceTray(void)
{
    CloseValve(VALVE_NOS_FLOW);
    OpenValve(VALVE_ICE_TRAY_IN);
    TurnOnCirculatePumpTrayIn();

    RegisterFlowMeterId(FLOW_ID_ICE_TRAY, UpdateIceTrayCurrentAmount);     // Register...
//    StartCheckOutage( OUTAGE_DELAY_TIME, OUTAGE_CHECK_TIME_10SEC, OUTAGE_TYPE_ICE_TRAY );
}

static void StopFeedIceTray(void)
{
    OpenValve(VALVE_NOS_FLOW);
    CloseValve(VALVE_ICE_TRAY_IN);
    TurnOffCirculatePumpTrayIn();

    RegisterFlowMeterId(FLOW_ID_ICE_TRAY, NULL);     // Unregister...
//    StopCheckOutage();
}

/* ���� �غ� */
static U8 DoReady(void)
{
    U8 mu8Done = FALSE;
    TEMP_T  tAmbi;
    TEMP_T  tRoom;

    switch( Ready.Step )
    {
        case STEP_READY_CHECK_CONDITION:
            // ���� �ּ� ���� �ð� �ʱ�ȭ
            ResetDeIcingCompOnTime();

            if( GetRoomWaterLevel() >= LEVEL_MID 
                    && IsExpiredCompProtectTime() == TRUE 
              )
            {
                Ready.Step++;
            }
            break;

            /* ���̽� Ʈ���� ���� ��ġ�� �̵� ���� */
        case STEP_READY_TRAY_MOVE:
            MoveIceTrayIcing();
            Ready.Step++;
            break;

            /* ���̽� Ʈ���� ���� ��ġ�� �̵� �Ϸ� */
        case STEP_READY_TRAY_DONE:
            if( GetIceTrayStatus() == ICE_TRAY_ICING )
            {
                // ����� ���� ���̸�, ���� ���� ��������...
                // �׷��� ������ �巹�� ��ũ ������ üũ�ϰ� Ʈ���� �Լ� �����Ϸ� ��� �̵�...
                if( IsTurnOnCompBldc() == TRUE )
                {
                    // GO TO ----  STEP_READY_TRAY_FEED_START
                    Ready.Step++; 
                }
                else
                {
                    // GO TO ----- SKIP STEP_READY_CHECK_CONTY_ICING
                    Ready.Step = STEP_READY_EMPTY_DRAIN_2;
                }
            }
            break;

        case STEP_READY_CHECK_CONTY_ICING:
            // �ø� ��ȯ ��� �ִ� ���� 
            GasSwitchIce();
            Ready.TimeOut = GAS_SWITCH_TIME_OUT;
            Ready.Step++;
            break;

        case STEP_READY_CHECK_CONTY_ICING_DONE:
            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                TurnOnCompRps(COMP_TYPE_ICE, COMP_ICING_RPS);
                TurnOnFan(FAN_TYPE_ICE);
                //TurnOnFan();

                Ready.Step++;
            }
            else if( Ready.TimeOut == 0 )
            {
                // �ø� ��ȯ ��� ��� �ִ� ���� �ð� �ʰ�...
                Ready.Step++;
            }
            break;

            /* �� ���� ���� �ٽ� �ѹ� ��� ��ũ ���� �˻� */
        case STEP_READY_EMPTY_DRAIN_2:
            if( GetDrainLevel() == LEVEL_LOW 
                    && GetDrainPumpStatus() == OFF )
            {
                Ready.Step++;
            }
            else
            {
                StartDrainPumpOnLevel();
            }
            break;

            /* Ʈ���� �� ���� ���� */
        case STEP_READY_TRAY_FEED_START:          
            if( IsUsingWater() == TRUE )
            {
                break;
            }

            Ready.TrayFeedAmount = GetAmountHz(AMOUNT_ID_TRAY);
            //TurnOnTempSensor( TEMP_ID_ROOM_WATER, ROOM_OUT_TIME_OUT_TIME );
            Ready.RoomOutTimeOut = ROOM_OUT_TIME_OUT_TIME;

            StartFeedIceTray();

            Ready.Step++;

#if 0
            CloseValve(VALVE_NOS_FLOW);
            OpenValve(VALVE_ICE_TRAY_IN);
            TurnOnCirculatePumpTrayIn();
            Ready.TrayFeedTime = ICE_TRAY_FEED_TIME;
            TurnOnTempSensor( TEMP_ID_COLD_WATER, ICE_TRAY_FEED_TIME );

            Ready.Step++;
#endif
            break;

            /* Ʈ���� �� ���� ���� ���� �˻� */
        case STEP_READY_TRAY_FEED_DONE :
            if( Ready.RoomOutTimeOut != 0 )
            {
                Ready.RoomOutTimeOut--;
            }
            else
            {
                if( GetFlowMeterLPM() < 0.1f )
                {
                    StartIceOutageTimer();
                }

                // ���� ���...
                StopIceMake();
                StopFeedIceTray();
                break;
            }

            // ����, �ü� �Լ�, �¼� �Լ�(������ ����) ���̸� ���...
            // ��� �ð� �ʰ��� ����
            if( IsUsingWater() == TRUE 
                    || IsOpenValve( VALVE_DRAIN ) == TRUE )
            {
                OpenValve(VALVE_NOS_FLOW);
                CloseValve(VALVE_ICE_TRAY_IN);
                break;
            }

            // �Լ� �Ϸ� 
            CloseValve(VALVE_NOS_FLOW);
            OpenValve(VALVE_ICE_TRAY_IN);
            TurnOnCirculatePumpTrayIn();
            if( Ready.TrayFeedAmount == 0UL )
            {
                StopFeedIceTray();
                Ready.Step++;
            }


#if 0        
            if( Ready.TrayFeedTime != 0 )
            {
                Ready.TrayFeedTime--;
            }

            if( GetDrainLevel() >= LEVEL_MID 
                    || Ready.TrayFeedTime == 0 )
            {
                OpenValve(VALVE_NOS_FLOW);
                CloseValve(VALVE_ICE_TRAY_IN);
                TurnOffCirculatePumpTrayIn();
                Ready.Step++;
            }
#endif
            break;

        case STEP_READY_GAS_SWITCH:
            GasSwitchIce();
            Ready.TimeOut = GAS_SWITCH_TIME_OUT;

            Ready.Step++;
            break;

        case STEP_READY_GAS_SWITCH_DONE:
            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                Ready.Step++;
            }
            else if( Ready.TimeOut == 0 )
            {
                Ready.Step++;
            }
            break;

        case STEP_READY_HOT_GAS_SWITCH:
            HotGasSwitchNormal();
            Ready.TimeOut = GAS_SWITCH_TIME_OUT;

            Ready.Step++;
            break;

        case STEP_READY_COMP_ON:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL )
            {
                TurnOnCompRps(COMP_TYPE_ICE, COMP_ICING_RPS);
                TurnOnFan(FAN_TYPE_ICE);
                //TurnOnFan();

                Ready.Step++;
            }
            else if( Ready.TimeOut == 0 )
            {
                Ready.Step++;
            }
            break;

        case STEP_READY_DONE:
            mu8Done = TRUE;
            break;

        default:
            Ready.Step = 0;
            break;
    }

    return mu8Done;
}


static U8 CheckIcingTemp(void)
{
    if( IsError(ERR_ID_TEMP_AMBIENT_2 ) == TRUE 
            || IsError(ERR_ID_TEMP_COLD_WATER) == TRUE )
    {
        return FALSE;
    }

    if( IsValidDoIcingColdTempRange() == FALSE )
    {
        return FALSE;
    }

    if( IsValidDoIcingAmbiTempRange() == FALSE )
    {
        return FALSE;
    }

     return TRUE;
}

/* ���� */
static U8 DoIcing(void)
{
    U8 done = FALSE;
    U8 region;

    switch( Icing.Step )
    {
        case STEP_ICING_CHECK_COMP:
            if( IsExpiredCompProtectTime() == TRUE )
            {
                Icing.Step++;
            }
            break;

        case STEP_ICING_FIGUARE_OUT_TIME:
            if( IsValidDoIcingColdTempRange() == FALSE 
                    || IsValidDoIcingAmbiTempRange() == FALSE 
                    || IsError(ERR_ID_TEMP_AMBIENT_2) == TRUE 
                    || IsError(ERR_ID_TEMP_COLD_WATER) == TRUE )

            {
                ClearIceMakeMode(ICE_MODE_ALL);
                SetIceMakeMode(ICE_MODE_INIT);

                TurnOffComp(COMP_TYPE_ICE);
                TurnOffFan(FAN_TYPE_ICE);
                //TurnOffFan();
                TurnOffSwingBar();

                Icing.Step = STEP_ICING_DONE;
            }
            else
            {
                TEMP_T  tAmbi;
                TEMP_T  tCold;

                tAmbi = GetTemp(TEMP_ID_AMBIENT_2);
                tCold = GetTemp(TEMP_ID_COLD_WATER);
                Icing.IcingTime = GetIcingTime(tCold, tAmbi);
                Icing.CompRps = GetIcingCompRps(tAmbi);
                Icing.FanOnOff = GetIcingFanOnOff(tAmbi);

                // ���� �ð� �Ϸ� 1�� �� Deicing Rps �� ���ߴ� ���� ������ �߰�
                region = GetDeIcingRegion(tAmbi);
                Icing.DeicingCompRps = GetDeIcingCompRps(region);

#if 0
                Icing.IcingTime = (700 * 10);      // @100ms Test 700s
#endif

                IcingTemp.tCold = tCold;
                IcingTemp.tAmbi = tAmbi;

                GasSwitchIce();
                Icing.Step++;
            }
            break;

        case STEP_ICING:

            if( Icing.IcingTime != 0 )
            {
                RPS_T   mRps = Icing.CompRps;

                // Ż�� ���� COMP RPS�� Ż�� RPS�� ��ȯ ��Ų��.
                // Reduce compressor de-icing noise
                if( Icing.IcingTime <= TIME_REDUCE_DEICING_NOISE )
                {
                    mRps = Icing.DeicingCompRps;
                }
                else
                {
                    mRps = Icing.CompRps;
                }
            
                //TurnOnComp( COMP_TYPE_ICE );
                TurnOnCompRps(COMP_TYPE_ICE, mRps);
                TurnOnOffFan(FAN_TYPE_ICE, Icing.FanOnOff);
                //TurnOnFan();
                TurnOnSwingBar();

                // ���� Ż�� ��� ó���� ���� �ּ� ���� �ð� ���
                UpdateDeIcingCompOnTime();
            }
            else
            {
                Icing.Step++;
                TurnOffSwingBar();
            }
            break;

        case STEP_ICING_DONE:
            done = TRUE;
            break;

        default:
            Icing.Step = 0;
            break;
    }

    return done;
}

static void StartDeIcing(U8 region)
{
    DeIcing.Region = region;

    DeIcing.Count = 0;
    DeIcing.RepeatNum = GetDeIcingRepeatNum(region);
    DeIcing.CompRps = GetDeIcingCompRps(region);
    DeIcing.FanOnOff = GetDeIcingFanOnOff(region);
}

static U8 MakeDeIcingOnOff(void)
{
    if( DeIcing.Count >= DeIcing.RepeatNum )
    {
        return FALSE;
    }

    DeIcing.OnOffCmd  = GetDeIcingCmd(DeIcing.Count);
    DeIcing.OnOffTime = GetDeIcingTime(DeIcing.Region, DeIcing.Count);
    DeIcing.CompRps   = GetDeIcingCompRps(DeIcing.Region);
    DeIcing.FanOnOff = GetDeIcingFanOnOff(DeIcing.Region);

    DeIcing.Count++;

    return TRUE;
}

/* Ż�� */
static U8 DoDeIcing(void)
{
    static U8 mu8CheckTrayError = FALSE;
    U8 region;
    U8 done = FALSE;
    TEMP_T tAmbi;

    switch( DeIcing.Step )
    {
        case STEP_DEICING_TRAY_MOVE:
            mu8CheckTrayError = FALSE;

            /* Ʈ���� Ż�� ��ġ �̵� */
            TurnOffSwingBar();
            MoveIceTrayDeIcing();
            DeIcing.Step++;
            break;

        case STEP_DEICING_TRAY_DONE:
            /* Ʈ���� �̵����� �˻� */
            if( IsErrorIceTrayCoilFrezzing() == TRUE )
            {
                // Ʈ���� �̵� ���� �߻��Ǹ�, �켱 Ż���� ������.
                
                mu8CheckTrayError = TRUE;
                DeIcing.Step++;
                break;
            }

            /* Ʈ���� Ż�� ��ġ �̵� �Ϸ� ��� */
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                // ���� ���� Ʈ���� �̵� ������, ���� ���� ī��Ʈ �ʱ�ȭ
                DeIcing.Step++;
            }
            break;

        case STEP_DEICING_FIGUARE_OUT_REGION:
            /* Ż�� �ð� ��� */
            if( IsError(ERR_ID_TEMP_AMBIENT_2) == TRUE 
                    || IsValidDoIcingAmbiTempRange() == FALSE )
            {
                tAmbi = IcingTemp.tAmbi;
            }
            else
            {
                tAmbi = GetTemp(TEMP_ID_AMBIENT_2);
            }
            region = GetDeIcingRegion(tAmbi);
            StartDeIcing( region );
            HotGasSwitchHot();

            TurnOnOffFan(FAN_TYPE_ICE, DeIcing.FanOnOff);
            TurnOnCompRps(COMP_TYPE_ICE, DeIcing.CompRps);
            //TurnOffFan();

            // UV ON
            {
                U16 mu16OnTime;

                mu16OnTime = GetDeIcingTime(region, 0);
                //TurnOnUvId( UV_ICE_TRAY_SHOT, 0, mu16OnTime );
            }

            if( tAmbi < 20.0f )
            {
                DeIcing.Step = STEP_DEICING_HOTGAS_SWITCH_CHECK;
            }
            else
            {
                DeIcing.Step++;
            }
            break;

/********************************************************************************************************************/        
// 20�� �̻�

        case STEP_DEICING_COMMAND:
            /* Ż�� �ð� �� Ż�� ���� ���� ��� ��� */
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_HOT )
            {
                if( MakeDeIcingOnOff() == TRUE )
                {

                    DeIcing.Step++;
                }
                else
                {
                    /* Ż�� ON/OFF ������ �Ϸ�Ǹ�, ���� �˻� �ܰ�� �̵� */
                    DeIcing.Step = STEP_DEICING_DONE_CONTROL;
                }
            }
            break;

            
        case STEP_DEICING_CONTROL:
            if( DeIcing.OnOffTime == 0 )
            {
                /* Ż�� �ð� �� Ż�� ���� ���� ��� ��� */
                if( MakeDeIcingOnOff() == FALSE )
                {
                    /* Ż�� ON/OFF ������ �Ϸ�Ǹ�, ���� �˻� �ܰ�� �̵� */
                    DeIcing.Step = STEP_DEICING_DONE_CONTROL;
                    break;
                }
            }

            if( DeIcing.OnOffTime != 0 )
            {
                if( DeIcing.OnOffCmd == ON )
                {
                    TurnOnCompRps(COMP_TYPE_ICE, DeIcing.CompRps);
                    TurnOnOffFan(FAN_TYPE_ICE, DeIcing.FanOnOff);
                    //TurnOffFan();
                }
                else
                {
                    // Do Nothing...
                }
            }
            break;

/********************************************************************************************************************/


/********************************************************************************************************************/
// 20�� �̸�
        case STEP_DEICING_HOTGAS_SWITCH_CHECK:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_HOT )
            {
                DeIcing.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                DeIcing.Step++;
            }
            break;

        case STEP_DEICING_GAS_CLOSE_SWITCH_WAIT:
            if( DeIcing.GasSwitchWaitTime != 0 )
            {
                DeIcing.GasSwitchWaitTime--;
            }

            if( DeIcing.GasSwitchWaitTime == 0 )
            {
                GasSwitchClose();
                DeIcing.Step++;
            }
            break;

        case STEP_DEICING_20DEGREE_UNDER_COMMAND:
            if( GetGasSwitchStatus() == GAS_SWITCH_CLOSE )
            {
                if( MakeDeIcingOnOff() == TRUE )
                {

                    DeIcing.Step++;
                }
                else
                {
                    /* Ż�� ON/OFF ������ �Ϸ�Ǹ�, ���� �˻� �ܰ�� �̵� */
                    DeIcing.Step = STEP_DEICING_DONE_CONTROL;
                }
            }
            break;

        case STEP_DEICING_20DEGREE_UNDER_CONTROL :
            if( DeIcing.OnOffTime == 0 )
            {
                /* Ż�� �ð� �� Ż�� ���� ���� ��� ��� */
                if( MakeDeIcingOnOff() == FALSE )
                {
                    /* Ż�� ON/OFF ������ �Ϸ�Ǹ�, ���� �˻� �ܰ�� �̵� */
                    GasSwitchIce();
                    DeIcing.Step++;
                    break;
                }
            }

            if( DeIcing.OnOffTime != 0 )
            {
                if( DeIcing.OnOffCmd == ON )
                {
                    TurnOnCompRps(COMP_TYPE_ICE, DeIcing.CompRps);
                    TurnOnOffFan(FAN_TYPE_ICE, DeIcing.FanOnOff);
                    //TurnOffFan();
                }
                else
                {
                    // Do Nothing...
                }
            }
            break;


        case STEP_DEICING_GAS_ICING_SWITCH_CHECK:
            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                DeIcing.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                DeIcing.Step++;
            }
            break;


        case STEP_DEICING_GAS_ICING_SWITCH_WAIT:
            if( DeIcing.GasSwitchWaitTime != 0 )
            {
                DeIcing.GasSwitchWaitTime--;
            }

            if( DeIcing.GasSwitchWaitTime == 0 )
            {
                DeIcing.Step = STEP_DEICING_DONE_CONTROL;
            }
            break;

/*******************************************************************************************************************/

        case STEP_DEICING_DONE_CONTROL:
            // �濭�� ��ġ�� �̵�
            HotGasSwitchNormal();

            // ICE TRAY �̵� ���� �߻�.
            // ���̽� Ʈ���� �̵��� �����ϸ� ī��Ʈ �ʱ�ȭ
            if( mu8CheckTrayError == TRUE )
            {
                if( GetIceTrayStatus() == ICE_TRAY_DEICING )
                {
                    ResetErrorIceTrayAccumCount();
                    DeIcing.Step++;
                }
                TurnOffComp(COMP_TYPE_ICE);
                TurnOffFan(FAN_TYPE_ICE);
                //TurnOffFan();
            }
            else
            {
                DeIcing.Step++;
            }
            break;

#if CONFIG_BACK_FEEDER

        case STEP_DEICING_BACK_FEEDER_START:
            // ��ȸ�� �Ǵ� 
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL )
            {
                if( IsOpenIceDoor() != TRUE )
                {
                    TurnOffRelay(RELAY_FEEDER_CW);
                    TurnOnRelay(RELAY_FEEDER_CCW);
                    DeIcing.BackFeederTime = 5;     // 0.5s @100ms
                    DeIcing.Step++;
                }
                else
                {
                    DeIcing.BackFeederTime = 0;
                    DeIcing.Step = STEP_DEICING_CHECK_ICE_FULL;
                }
            }
            break;

        case STEP_DEICING_BACK_FEEDER_DONE:
            if( DeIcing.BackFeederTime != 0 )
            {
                // Wait
            }
            else
            {
                TurnOffRelay( RELAY_FEEDER_CW );
                TurnOffRelay( RELAY_FEEDER_CCW );
                DeIcing.Step++;
            }
            break;
#endif

        case STEP_DEICING_CHECK_ICE_FULL:

            // UV OFF
            //TurnOffUvId( UV_ICE_TRAY_SHOT );


#if !CONFIG_BACK_FEEDER
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL )
#endif
            {
                /* ���� �˻� */
                StartCheckIceFull();
                StartCheckIceLow();

                DeIcing.Step++;
            }
            break;

        case STEP_DEICING_WAIT_CHECK_ICE_FULL:
            if( IsDoneCheckIceFull() == TRUE )
            {
                UpdateIceFullCheckCount();
                UpdateIcingCount();
                DeIcing.Step++;
            }
            break;

        case STEP_DEICING_DONE:
           done = TRUE; 
           break;

        default:
           DeIcing.Step = 0;
            break;
    }

    return done;
}

/* ���� Ż�� */
static void StartDummyDeIcing(U8 region)
{
    DummyDeIcing.Region = region;
    DummyDeIcing.Count = 0;
    DummyDeIcing.RepeatNum = GetDeIcingRepeatNum(region);
    DummyDeIcing.FanOnOff = GetDeIcingFanOnOff(region);
}

static U8 MakeDummyDeIcingCompOnOff(void)
{
    if( DummyDeIcing.Count >= DummyDeIcing.RepeatNum )
    {
        DummyDeIcing.Count = 0;
        return FALSE;
    }

    DummyDeIcing.CompOnCmd  = GetDummyDeIcingCmd(DummyDeIcing.Count);
    DummyDeIcing.CompOnTime = GetDummyDeIcingCompOnTime(DummyDeIcing.Region, DummyDeIcing.Count);
    DummyDeIcing.FanOnOff = GetDeIcingFanOnOff(DummyDeIcing.Region);

    DummyDeIcing.Count++;

    return TRUE;
}

static U8 MakeDummyDeIcingOnOff(void)
{
    if( DummyDeIcing.Count >= DummyDeIcing.RepeatNum )
    {
        DummyDeIcing.Count = 0;
        return FALSE;
    }

    DummyDeIcing.OnOffCmd  = GetDummyDeIcingCmd(DummyDeIcing.Count);
    DummyDeIcing.OnOffTime = GetDummyDeIcingTime(DummyDeIcing.Region, DummyDeIcing.Count);
    DummyDeIcing.OnOffRps  = GetDeIcingCompRps(DummyDeIcing.Region);
    DummyDeIcing.FanOnOff = GetDeIcingFanOnOff(DummyDeIcing.Region);

    DummyDeIcing.Count++;

    return TRUE;
}

static U8 DoOffLoad(void)
{
    // ���� OFF

    TurnOffSwingBar();
    TurnOffComp(COMP_TYPE_ICE);
    TurnOffFan(FAN_TYPE_ICE);
    //TurnOffFan();
    OpenValve(VALVE_NOS_FLOW);
    CloseValve(VALVE_ICE_TRAY_IN);
    TurnOffCirculatePumpTrayIn();

    return TRUE;
}

/* ���� ���� ��� OFF */
static U8 DoOff(void)
{
    // ���� Ż�� ����
    // ���� ����
    DoOffLoad();

    ClearIceMakeMode( ICE_MODE_DUMMY_DEICING );
    ClearIceMakeMode( ICE_MODE_ALL );

    return TRUE;
}

/* ���� Ż�� */
static U8 DoDummyDeIcing(void)
{
    static U8 mu8CheckTrayError = FALSE;
    U8 region = 0;
    U8 done = FALSE;
    TEMP_T tAmbi;

    switch( DummyDeIcing.Step )
    {
        case STEP_DUMMY_DEICING_GAS_SWITCH:
            /* ���� ��ġ�� �ø� ��ȯ ��� �̵� */
         //   GasSwitchIce();

            mu8CheckTrayError = FALSE;

            /* ���� ���� ���� OFF */
            TurnOffSwingBar();
            TurnOffComp(COMP_TYPE_ICE);
            TurnOffFan(FAN_TYPE_ICE);
            //TurnOffFan();
            OpenValve(VALVE_NOS_FLOW);
            CloseValve(VALVE_ICE_TRAY_IN);
            TurnOffCirculatePumpTrayIn();

            DummyDeIcing.Step++;
            break;

        case STEP_DUMMY_DEICING_TRAY_MOVE: 
            /* Ʈ���� Ż�� ��ġ�� �̵� */
            MoveIceTrayDeIcing();
            DummyDeIcing.Step++;
            break;

        case STEP_DUMMY_DEICING_TRAY_DONE:
            /* Ʈ���� �̵����� �˻� */
            if( IsErrorIceTrayCoilFrezzing() == TRUE )
            {
                mu8CheckTrayError = TRUE;
            }

            /* Ʈ���� �̵� �Ϸ� ��� */
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                // ���� ���� Ʈ���� �̵� ������, ���� ���� ī��Ʈ �ʱ�ȭ
                if( mu8CheckTrayError == FALSE )
                {
                    ResetErrorIceTrayAccumCount();
                }

                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_COMP_PROTECT_TIME:
            /* ����� ���� �ð� ��� */
            if( IsExpiredCompProtectTime() == TRUE )
            {
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_FIGUARE_OUT_REGION:
            if( IsError(ERR_ID_TEMP_AMBIENT_2) == TRUE )
            {
                /* ����� ���� �ð� ��� */
                DummyDeIcing.Step = STEP_DUMMY_DEICING_DONE_CONTROL;
            }
            else
            {
                /* ����� ���� �ð� ��� */
                tAmbi = GetTemp(TEMP_ID_AMBIENT_2);
                region = GetDummyDeIcingRegion(tAmbi);
                StartDummyDeIcing(region);

                DummyDeIcing.Ambient = tAmbi;
                DummyDeIcing.CompOnRps = GetIcingCompRps( tAmbi );

                if( MakeDummyDeIcingCompOnOff() == TRUE )
                {
                    HotGasSwitchNormal();
                    TurnOnCompRps(COMP_TYPE_ICE, DummyDeIcing.CompOnRps);
                    TurnOnOffFan(FAN_TYPE_ICE, DeIcing.FanOnOff);
                    DummyDeIcing.Step++;
                }
                else
                {
                    /* Ż�� ON/OFF ������ �Ϸ�Ǹ�, ���� �˻� �ܰ�� �̵� */
                    DummyDeIcing.Step = STEP_DUMMY_DEICING_DONE_CONTROL;
                }
            }
            break;

        case STEP_DUMMY_DEICING_GAS_SWITCH_VALVE_CHECK:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL )
            {
                GasSwitchIce();
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_COMP_ON:
            /* ����� ���� */
            if( DummyDeIcing.CompOnTime == 0 )
            {
                if( MakeDummyDeIcingCompOnOff() == FALSE )
                {
                    DummyDeIcing.Step = STEP_DUMMY_DEICING_COMMAND;
                    break;
                }
            }

            if( DummyDeIcing.CompOnTime != 0 )
            {
                if( DummyDeIcing.CompOnCmd == ON )
                {
                    TurnOnCompRps(COMP_TYPE_ICE, DummyDeIcing.CompOnRps);
                    TurnOnOffFan(FAN_TYPE_ICE, DummyDeIcing.FanOnOff);
                    //TurnOffFan();
                }
                else
                {
                    // Do Nothing
                }
            }
            break;

        case STEP_DUMMY_DEICING_COMMAND:
            /* Ż�� �ð� �� Ż�� ���� ���� ��� ��� */
            if( MakeDummyDeIcingOnOff() == TRUE )
            {
                HotGasSwitchHot();
                TurnOnCompRps(COMP_TYPE_ICE, DummyDeIcing.OnOffRps);
                TurnOnOffFan(FAN_TYPE_ICE, DummyDeIcing.FanOnOff);
                //TurnOffFan();

                if( DummyDeIcing.Ambient < 20.0f )
                {
                    DummyDeIcing.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                    DummyDeIcing.Step = STEP_DUMMY_DEICING_20DEGREE_UNDER_HOT_GAS_SWITCH_CHECK;
                }
                else
                {
                    DummyDeIcing.Step++;
                }
            }
            else
            {
                /* Ż�� ON/OFF ������ �Ϸ�Ǹ�, ���� �˻� �ܰ�� �̵� */
                DummyDeIcing.Step = STEP_DUMMY_DEICING_DONE_CONTROL;
            }
            break;

/********************************************************************************************************************/        
// 20�� �̻�
        case STEP_DUMMY_DEICING_HOT_GAS_SWITCH_CHECK:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_HOT )
            {
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_CONTROL:
            /* Ż�� ���� ���� */
            if( DummyDeIcing.OnOffTime == 0 )
            {
                if( MakeDummyDeIcingOnOff() == FALSE )
                {
                    HotGasSwitchNormal();
                    DummyDeIcing.Step = STEP_DUMMY_DEICING_CONTROL_CHECK;
                    break;
                }
            }

            if( DummyDeIcing.OnOffTime != 0 )
            {
                if( DummyDeIcing.OnOffCmd == ON )
                {
                    TurnOnCompRps(COMP_TYPE_ICE, DummyDeIcing.OnOffRps);
                    TurnOnOffFan(FAN_TYPE_ICE, DummyDeIcing.FanOnOff);
                    //TurnOffFan();
                }
                else
                {
                    // Do Nothing
                }
            }
            break;


/********************************************************************************************************************/        
// 20�� �̸�
        case STEP_DUMMY_DEICING_20DEGREE_UNDER_HOT_GAS_SWITCH_CHECK:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_HOT )
            {
                DummyDeIcing.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_GAS_CLOSE_SWITCH_WAIT:
            if( DummyDeIcing.GasSwitchWaitTime != 0 )
            {
                DummyDeIcing.GasSwitchWaitTime--;
            }              

            if( DummyDeIcing.GasSwitchWaitTime == 0 )
            {
                GasSwitchClose();
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_GAS_CLOSE_SWITCH:
            if( GetGasSwitchStatus() == GAS_SWITCH_CLOSE )
            {
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_20DEGREE_UNDER_CONTROL:
            /* Ż�� ���� */
            if( DummyDeIcing.OnOffTime == 0 )
            {
                if( MakeDummyDeIcingOnOff() == FALSE )
                {
                    GasSwitchIce();
                    DummyDeIcing.Step++;
                    break;
                }
            }

            if( DummyDeIcing.OnOffTime != 0 )
            {
                if( DummyDeIcing.OnOffCmd == ON )
                {
                    TurnOnCompRps(COMP_TYPE_ICE, DummyDeIcing.OnOffRps);
                    TurnOnOffFan(FAN_TYPE_ICE, DummyDeIcing.FanOnOff);
                    //TurnOffFan();
                }
                else
                {
                    // Do Nothing
                }
            }
            break;

        case STEP_DUMMY_DEICING_GAS_ICING_SWITCH_CHECK:
            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                DummyDeIcing.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_GAS_ICING_SWITCH_WAIT:
            if( DummyDeIcing.GasSwitchWaitTime != 0 )
            {
                DummyDeIcing.GasSwitchWaitTime--;
            }

            if( DummyDeIcing.GasSwitchWaitTime == 0 )
            {
                HotGasSwitchNormal();
                DummyDeIcing.Step = STEP_DUMMY_DEICING_CONTROL_CHECK;
            }
            break;

/********************************************************************************************************************/

        case STEP_DUMMY_DEICING_CONTROL_CHECK:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL )
            {
                DummyDeIcing.Step++;
            }
            break;

        case STEP_DUMMY_DEICING_DONE_CONTROL:
            /* �ٽ� �� �� ���� OFF */            
            if( GetIceMake() == TRUE  )
            {
                ClearIceMakeMode( ICE_MODE_DEICING_OFF );
            }

            DummyDeIcing.Step++;
            break;

        case STEP_DUMMY_DEICING_DONE:
            done = TRUE;
            break;

        default:
            DummyDeIcing.Step = 0;
            break;
    }

    return done;
}

#define ICING_MOVE_DEICING_RPS            (70U)       // 70rps
#define ICING_MOVE_DEICING_TIME           (600U)      // @1sec 10min
#define ICING_MOVE_DEICING_COMPOFF_TIME   (3000U)     // @1sec 50min
static U8 DoIcingMoveDeicing(void)
{
    static U8 mu8CheckTrayError = FALSE;
    U8 mu8Done = FALSE;

    switch(IcingMoveDeicing.Step)
    {
        case STEP_ICING_MOVE_DEICING_INIT:
            mu8CheckTrayError = FALSE;         

            /* ���� ���� ���� OFF */
            DoOffLoad();
            IcingMoveDeicing.Step++;
            break;

        case STEP_ICING_MOVE_DEICING_COMP_PROTECT_TIME:
            /* ����� ���� �ð� ��� */
            if( IsExpiredCompProtectTime() == TRUE )
            {
                ClearIceMakeMode( ICE_MODE_DEICING_OFF );
                IcingMoveDeicing.Step++;
            }
            break;

        case STEP_ICING_MOVE_DEICING_HOTGAS_SWITCH:
            HotGasSwitchHot();
            IcingMoveDeicing.Step++;
            break;

        case STEP_ICING_MOVE_DEICING_HOTGAS_SWITCH_CHECK:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_HOT )
            {
                IcingMoveDeicing.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                IcingMoveDeicing.Step++;
            }
            break;

        case STEP_ICING_MOVE_DEICING_GAS_CLOSE_SWITCH_WAIT:
            if( IcingMoveDeicing.GasSwitchWaitTime != 0 )
            {
                IcingMoveDeicing.GasSwitchWaitTime--;
            }              

            if( IcingMoveDeicing.GasSwitchWaitTime == 0 )
            {
                GasSwitchClose();
                IcingMoveDeicing.Step++;
            }
            break;

        case STEP_ICING_MOVE_DEICING_GAS_CLOSE_SWITCH_CHECK:
            if( GetGasSwitchStatus() == GAS_SWITCH_CLOSE )
            {
                IcingMoveDeicing.Step++;
            }
            break;

        case STEP_ICING_MOVE_DEICING_COMMAND:
            IcingMoveDeicing.TargetRps = ICING_MOVE_DEICING_RPS;      // 70rps
            IcingMoveDeicing.TargetTime = ICING_MOVE_DEICING_TIME;    // Ż�� �ð�

            TurnOnCompRps(COMP_TYPE_ICE, IcingMoveDeicing.TargetRps);
            TurnOnOffFan(FAN_TYPE_ICE, DummyDeIcing.FanOnOff);
            //TurnOffFan();

            IcingMoveDeicing.Step++;
            break;

        case STEP_ICING_MOVE_DEICING_CONTROL:
            if( IcingMoveDeicing.TargetTime != 0 )
            {     
                IcingMoveDeicing.TargetTime--;
            }

            if( IcingMoveDeicing.TargetTime == 0 )
            {
                GasSwitchIce();
                IcingMoveDeicing.Step++;
            }
            break;

        case STEP_ICING_MOVE_DEICING_GAS_ICING_SWITCH_CHECK:
            if( GetGasSwitchStatus() == GAS_SWITCH_ICE )
            {
                IcingMoveDeicing.GasSwitchWaitTime = GAS_CLOSE_SWITCH_WAIT_TIME;
                IcingMoveDeicing.Step++;
            }
            break;

        case STEP_ICING_MOVE_DEICING_DONE_CONTROL:
            if( IcingMoveDeicing.GasSwitchWaitTime != 0 )
            {
                IcingMoveDeicing.GasSwitchWaitTime--;
            }

            if( IcingMoveDeicing.GasSwitchWaitTime == 0 )
            {
                HotGasSwitchNormal();
                IcingMoveDeicing.Step++;
            }
            break;

        case STEP_ICING_MOVE_DEICING_DONE_CONTROL_CHECK:
            if( GetHotGasSwitchStatus() == HOTGAS_SWITCH_NORMAL )
            {
                DoOffLoad();
                IcingMoveDeicing.CompOffWaitTime = ICING_MOVE_DEICING_COMPOFF_TIME;
                IcingMoveDeicing.Step++;
            }
            break;

        case STEP_ICING_MOVE_DEICING_COMP_OFF_WAIT:
            if( IcingMoveDeicing.CompOffWaitTime != 0 )
            {
                IcingMoveDeicing.CompOffWaitTime--;
            }

            if( IcingMoveDeicing.CompOffWaitTime == 0 )
            {
                IcingMoveDeicing.Step = STEP_ICING_MOVE_DEICING_DONE;
            }
            break;

        case STEP_ICING_MOVE_DEICING_DONE:
             mu8Done = TRUE;
             break;

        default:
            IcingMoveDeicing.Step = 0;
            break;
    }

    return mu8Done;
}

typedef void(*FPInitAction_T)(void);
typedef U8(*FPDoAction_T)(void);
typedef struct _ice_maker_event_
{
    U16 Mode;
    
    FPInitAction_T DoInit;
    FPDoAction_T DoAction;
} SIceMakerEvent_T;

static SIceMakerEvent_T  IceMakerEventList[] = 
{
    { ICE_MODE_OFF_LOAD,            NULL,                    DoOffLoad },           // ���� OFF
    { ICE_MODE_OFF,                 NULL,                    DoOff },               // ���� ����.
    { ICE_MODE_DUMMY_DEICING,       InitDoDummyDeIcing,      DoDummyDeIcing },      // ���� Ż��

    { ICE_MODE_INIT,                InitDoInit,              DoInit },              // Ʈ���� ��ġ �ʱ�ȭ 
    { ICE_MODE_PREHEAT,             InitDoPreheat,           DoPreheat },           // Ż�� ���� ����� ���� COMP ����
    { ICE_MODE_READY,               InitDoReady,             DoReady },             // ���� ��ġ �̵�, Ʈ���� �Լ�
    { ICE_MODE_ICING,               InitDoIcing,             DoIcing },             // ���� 
    { ICE_MODE_DEICING,             InitDoDeIcing,           DoDeIcing },           // Ż��
    { ICE_MODE_ICING_MOVE_DEICING,  InitDoIcingMoveDeicing,  DoIcingMoveDeicing },  // ���̽� Ʈ���� ���� ��Ʈ���� Ż��
    { ICE_MODE_DEICING_OFF,         NULL,                    DoOffLoad },           // ���� ����    

    { ICE_MODE_STORAGE,             NULL,                    NULL },                // (�õ�����) ����
    { ICE_MODE_DEL_HUMIDITY,        NULL,                    NULL },                // (�õ�����) ����
    { ICE_MODE_DEFROST,             NULL,                    NULL },                // (�õ�����) ����
};
#define SZ_ICE_MAKER_EVENT_LIST  (sizeof(IceMakerEventList)/sizeof(SIceMakerEvent_T) )

void ProcessIceMaker(void)
{
    U8 i;
    U8 mode;
    U8 ret;
    SIceMakerEvent_T *pList = NULL;
    U8 init = FALSE;

    /* ��忡 ���� DoAction�� ���� �� ��常 �����ϰ�, 
     * �׿ܿ� ���鿡 ���ؼ��� Init �Լ��� ȣ���ؼ� �� ���鿡 ����
     * switch ���� step�� 0���� �ʱ�ȭ ��Ų��
     *
     * �̷��� �ϴ� ������ �� ��带 ��� �Կ� �־� step�� 0���� �ʱ�ȭ ���� �ʴ� ���
     * �������� �����ϱ� ���ؼ��̴�.
     * ���� Ż��, �Ǵ� ice tray recovery ���� �������� ClearIceMakeMode() �Լ��� ����
     * ��尡 �������� �������� ������ �ʰ� clear�Ǵ� ��� ������ �߻��� �� �ֱ� �����̴�.
     */
    for( i=0; i<SZ_ICE_MAKER_EVENT_LIST; i++ )
    {
        pList = &IceMakerEventList[i];

        if( init == TRUE  
        || IsSetIceMakeMode( pList->Mode ) == FALSE )
        {
            if( pList->DoInit != NULL )
            {
                pList->DoInit();
            }
        }
        else
        {
            init = TRUE;
            ret = FALSE;
            if( pList->DoAction != NULL )
            {
                ret = pList->DoAction();
            }

            if( ret == TRUE )
            {
                ClearIceMakeMode( pList->Mode );
                if( pList->DoInit != NULL )
                {
                    pList->DoInit();
                }
            }
        }
    }

    // Protect Ice Maker
    if( IsOverCompProtectSecondOnTime() == TRUE )
    {
        SetIceMakeMode( ICE_MODE_OFF );
    }
}

void UpdateIceMakerTimer(void)
{
    // ���� Ÿ�̸�
    if( Preheat.PreHeatTime != 0 )
    {
        Preheat.PreHeatTime--;
    }

    // Ready Step Timer
    if( Ready.TimeOut != 0 )
    {
        Ready.TimeOut--;
    }

    // ���� Ÿ�̸�
    if( Icing.IcingTime != 0 )
    {
        Icing.IcingTime--;
    }

    // Ż�� Ÿ�̸�
    if( DeIcing.OnOffTime != 0 )
    {
        DeIcing.OnOffTime--;
    }

#if( CONFIG_BACK_FEEDER == 1 )
    if( DeIcing.BackFeederTime != 0 )
    {
        DeIcing.BackFeederTime--;
    }
#endif

    // ���� Ż�� Ÿ�̸�
    if( DummyDeIcing.OnOffTime != 0 )
    {
        DummyDeIcing.OnOffTime--;
    }

    if( DummyDeIcing.CompOnTime != 0 )
    {
        DummyDeIcing.CompOnTime--;
    }
}
