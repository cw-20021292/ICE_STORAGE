#include "process_make.h"
#include "heater.h"
#include "comp.h"
#include "fan_motor.h"
#include "api_fan_motor.h"
#include "circulate_pump.h"

#include "hot_water.h"
#include "cold_water.h"
#include "ice.h"
#include "ice_maker.h"
#include "gas_switch_valve.h"
#include "temp.h"


void InitProcessMake(void)
{
    InitHeater();
    InitComp();
    InitFanMotor();
}


/* �¼� ���� ���� */
static void ProcessMakeHotWater(void)
{
    U8 mu8OnOff = OFF;

    /* MAKE ON/OFF */
    if( GetHotWaterMake() == TRUE )
    {
        mu8OnOff = ON;
    }
    ControlHeater( mu8OnOff );
}

/* �ð� ��ȯ ���� ���� */
#define PUMP_ON_CYCLE     (120)    // 2min ( 2* 60 ) @1sec
#define PUMP_ON_TIME      (20)     // 20s  @1sec

#if 0
U16 dbgPumpOnCmd = 0;
U16 dbgPumpOnCycle = 0;
U16 dbgPumpOnTime = 0;
#endif
static void ControlMakeColdCirculatePump(void)
{
    static U16 mu16PumpOnCycle = PUMP_ON_CYCLE;
    static U16 mu16PumpOnTime = PUMP_ON_TIME;

    static U8 mu8PumpOn = FALSE;
    static U8 mu8PrevCmd = OFF;
    U8 mu8Cmd = OFF;

    // �ü� �µ� 10'C �̻��̸� ���� ����, �׷��� ������ ����
    if( GetTemp( TEMP_ID_COLD_WATER ) >= 10.0f )
    {
        mu8Cmd = OFF;

        mu16PumpOnCycle = PUMP_ON_CYCLE;
        //mu16PumpOnTime = PUMP_ON_TIME;
    }
    else
    {
        mu8Cmd = ON;

        if( mu16PumpOnCycle != 0 )
        {
            mu16PumpOnCycle--;
        }
        else
        {
            mu16PumpOnCycle = PUMP_ON_CYCLE;
            mu16PumpOnTime = PUMP_ON_TIME;
        }
    }

#if 0
    // �ܱ� �µ� 30'C �̻� �̸� ���� ����
    if( GetTemp( TEMP_ID_AMBIENT ) >= 30.0f )
    {
        mu8Cmd = OFF;
    }
#endif

    if( mu8PrevCmd != mu8Cmd )
    {
        if( mu8Cmd == ON && mu8PumpOn == FALSE )
        {
            mu16PumpOnTime = PUMP_ON_TIME;
        }
        mu8PrevCmd = mu8Cmd;
    }

    if( mu16PumpOnTime != 0 )
    {
        mu16PumpOnTime--;
    }

    if( mu16PumpOnTime != 0 || mu8Cmd == ON )
    {
        mu8PumpOn = TRUE;
        TurnOnCirculatePumpMakeCold();

        if( mu16PumpOnTime == 0 )
        {
            TurnOffCirculatePumpMakeCold();
        }
    }
    else
    {
        mu8PumpOn = FALSE;
        TurnOffCirculatePumpMakeCold();
    }

#if 0
    dbgPumpOnCmd  = mu8Cmd;
    dbgPumpOnCycle = mu16PumpOnCycle;
    dbgPumpOnTime = mu16PumpOnTime;
#endif
}

/* �ð� ��ȯ ���� ���� ( Comp Off ) */
#define PUMP_ON_CYCLE_COMP_OFF     (1200)   // 20min ( 20* 60 ) @1sec
#define PUMP_ON_TIME_COMP_OFF      (15)     // 15s  @1sec
U16 mu16PumpCycleCompOff = 0; // @1sec
U16 mu16PumpOnTimeCompOff = PUMP_ON_TIME_COMP_OFF; // @1sec
static void ControlMakeColdCirculatePumpCompOff(void)
{
    if( mu16PumpCycleCompOff != 0)
    {
        mu16PumpCycleCompOff--;
    }
    else
    {
        mu16PumpCycleCompOff = PUMP_ON_CYCLE_COMP_OFF;
        mu16PumpOnTimeCompOff = PUMP_ON_TIME_COMP_OFF;
    }

    if( GetColdWaterConfigMake() != TRUE )
    {
        mu16PumpOnTimeCompOff = 0;
    }

    if( GetColdWaterLevel() != LEVEL_HIGH )
    {
        mu16PumpOnTimeCompOff = 0;
    }

    if( mu16PumpOnTimeCompOff != 0 )
    {
        mu16PumpOnTimeCompOff--;
        TurnOnCirculatePumpMakeCold();
    }
    else
    {
        TurnOffCirculatePumpMakeCold();
    }
}


// �ð� ���� ( �ʱ�ȭ, �ð�, �Ϸ� )
static void ProcessMakeColdWater(void)
{
#if 1
    /* �ü� �ʱ�ȭ 
     *  -. �ü� ��ȯ ���� ����
     *  -. �ø� ��� ��ȯ...
     *
     */
    if( IsSetColdWaterMakeMode( COLD_MODE_INIT ) == TRUE )
    {
        // �ð� �ʱ�ȭ
        // ��ȯ ���� ����
        // �ø� ��ȯ ��� �ð����� ��ġ �̵�
        TurnOffCirculatePumpMakeCold();

        /// ���� ī��Ʈ �ʱ�ȭ ( ���� 10ȸ ī��Ʈ�� ��, �ü��Լ��� ��Ű���� �ð��ô� ��� ������ ���� �Լ� ���ѵ� �� )
        ClearIcingCount();

        if( IsExpiredCompProtectTime() == TRUE )
        {
            GasSwitchCold();
            ClearColdWaterMakeMode( COLD_MODE_INIT );
        }

        mu16PumpCycleCompOff = PUMP_ON_CYCLE_COMP_OFF;
        mu16PumpOnTimeCompOff = 0;        
    }
    else if( IsSetColdWaterMakeMode( COLD_MODE_DONE ) == TRUE )
    {
        // �ð� �Ϸ�
        // ����� OFF
        // ��ȯ ���� ����
        TurnOffCirculatePumpMakeCold();
        TurnOffComp(COMP_TYPE_COLD);
        TurnOffFan(FAN_TYPE_COLD);

        ClearColdWaterMakeMode( COLD_MODE_COOLING );
        ClearColdWaterMakeMode( COLD_MODE_DONE );

        mu16PumpCycleCompOff = PUMP_ON_CYCLE_COMP_OFF;
        mu16PumpOnTimeCompOff = 0;
    }
    else if( IsSetColdWaterMakeMode( COLD_MODE_COOLING ) == TRUE )
    {
        RPS_T mRps;
        U8 fanOnOff;

        mRps = GetColdWaterMakeCompRps();
        fanOnOff = GetColdWaterMakeFan();
    
        // �ð� ��
        // ����� ON
        // ��ȯ ���� ON
        //TurnOnCirculatePumpMakeCold();
        ControlMakeColdCirculatePump();
        //TurnOnComp(COMP_TYPE_COLD);
        TurnOnCompRps(COMP_TYPE_COLD, mRps );
        TurnOnOffFan(FAN_TYPE_COLD, fanOnOff);

        mu16PumpCycleCompOff = PUMP_ON_CYCLE_COMP_OFF;
        mu16PumpOnTimeCompOff = 0;
    }
    else 
    {
        // COLD_MODE_NONE
        ControlMakeColdCirculatePumpCompOff();
        TurnOffComp(COMP_TYPE_COLD);
        TurnOffFan(FAN_TYPE_COLD);
    }

#else

    /* �ü� �ʱ�ȭ 
     *  -. �ü� ��ȯ ���� ����
     *  -. �ø� ��� ��ȯ...
     *
     */
    if( IsSetColdWaterMakeMode( COLD_MODE_INIT ) == TRUE )
    {
        // �ð� �ʱ�ȭ
        // ��ȯ ���� ����
        // �ø� ��ȯ ��� �ð����� ��ġ �̵�
        TurnOffCirculatePumpMakeCold();

        if( IsExpiredCompProtectTime() == TRUE )
        {
            GasSwitchCold();
            ClearColdWaterMakeMode( COLD_MODE_INIT );
        }
    }
    else if( IsSetColdWaterMakeMode( COLD_MODE_DONE ) == TRUE )
    {
        // �ð� �Ϸ�
        // ����� OFF
        // ��ȯ ���� ����
        TurnOffCirculatePumpMakeCold();
        TurnOffComp(COMP_TYPE_COLD);
        TurnOffFan();

        ClearColdWaterMakeMode( COLD_MODE_COOLING );
        ClearColdWaterMakeMode( COLD_MODE_DONE );
    }
    else if( IsSetColdWaterMakeMode( COLD_MODE_COOLING ) == TRUE )
    {
        RPS_T mRps;

        mRps = GetColdWaterMakeCompRps();
    
        // �ð� ��
        // ����� ON
        // ��ȯ ���� ON
        //TurnOnCirculatePumpMakeCold();
        ControlMakeColdCirculatePump();
        //TurnOnComp(COMP_TYPE_COLD);
        TurnOnCompRps(COMP_TYPE_COLD, mRps );
        TurnOnFan();
    }
    else 
    {
        // COLD_MODE_NONE
        TurnOffCirculatePumpMakeCold();
        TurnOffComp(COMP_TYPE_COLD);
        TurnOffFan();
    }

#endif
}

static void ProcessMakeIce(void)
{
    /* Process Ice Maker */
    ProcessIceMaker();
}


/* �ð� ���� �� ��� ���� */
static void MakeColdWaterMode(void)
{
    static U8 mu8PrevMake = FALSE;
    U8 mu8Make;
    U8 mu8Mode;


    mu8Make = GetColdWaterMake();         // �ð� ���� ����
    mu8Mode = GetColdWaterMakeMode();     // �ð� ���� �� ���

    // �ð� ���� ���°� ���� ���� �������� �������� ���� �Ѵ�.
    // ����, ���� �����ε� ���� ��尡 ������ ���� ���� ��Ų�� 
    if( mu8PrevMake != mu8Make )
    {
        // ���� -> ���� 
        if( mu8Make == TRUE )
        {
            if( mu8Mode == COLD_MODE_NONE )
            {
                SetColdWaterMakeMode( COLD_MODE_INIT | COLD_MODE_COOLING );
            }
        }
        // ���� -> ����
        else 
        {
            if( mu8Mode != COLD_MODE_NONE )
            {
                SetColdWaterMakeMode( COLD_MODE_DONE );
            }
        }

        mu8PrevMake = mu8Make;
    }
    else if( mu8Make == TRUE )
    {
        // �ð� ��忡�� �ð����� �ƴ϶��, �ð��� �����Ѵ�.
        if( mu8Mode == COLD_MODE_NONE )
        {
            SetColdWaterMakeMode( COLD_MODE_INIT | COLD_MODE_COOLING );
        }
    }
}

#define PREHEAT_DETECT_TIME   (1800UL)    // 30min ( 30min * 60sec = 1800sec )
#define PREHEAT_RELEASE_TIME  (180UL)    // 3min ( 3min * 60sec = 180sec )

U8 DebugPreheat = OFF;

static U8 GetPreheatStatus(void)
{
    static U8 mu8Preheat = OFF;

    if( GetCompOnOff() == TRUE )
    {
        if( GetCompOnTime() >= PREHEAT_RELEASE_TIME )
        {
            mu8Preheat = OFF;
        }
    }
    else
    {
        if( GetCompOffTime() >= PREHEAT_DETECT_TIME )
        {
            mu8Preheat = ON;
        }
    }

    DebugPreheat = mu8Preheat;

    return mu8Preheat;
}



/* ���� �� ���� ��� ���� */
static void MakeIceMakeMode(void)
{
    static U8 mu8PrevMake = FALSE;
    U8 mu8Make;
    U16 mu16Mode;
    U8 mu8Preheat;


    // ���� ����
    mu8Make = GetIceMake();               // ���� ON or OFF ?
    mu16Mode = GetIceMakeMode();          // ���� �� ���
    mu8Preheat = GetPreheatStatus();      // Comp Preheat ON or OFF?
    if( mu8PrevMake != mu8Make )
    {
        // ���� -> ���� 
        if(  mu8Make == TRUE )
        {
            if( mu16Mode == ICE_MODE_NONE )
            {
                if( mu8Preheat == ON )
                {
                   SetIceMakeMode( ICE_MODE_PREHEAT );
                }

                SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);
                ClearIceMakeMode( ICE_MODE_DEICING_OFF );
            }
        }
        else
        {
            // ���� -> ����
            // ������ �������� ���� ���, �⺻ ��ġ�� �̵���Ų��.
            if( IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE
                  || IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE 
                  || IsSetIceMakeMode( ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE 
                  || IsSetIceMakeMode( ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
            {
                ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
                SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_OFF_LOAD );
            }

            // ���� ������ ���� ��Ű�� ���ؼ� ����� OFF
            SetIceMakeMode( ICE_MODE_DEICING_OFF );
        }

        mu8PrevMake = mu8Make;
    }
    else if( mu8Make == TRUE )
    {
        /* ���� �� */
        if( mu16Mode == ICE_MODE_NONE )
        {
            if( mu8Preheat == ON )
            {
                SetIceMakeMode( ICE_MODE_PREHEAT );
            }
        
            SetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);
            ClearIceMakeMode( ICE_MODE_DEICING_OFF );
        }
    }

    // ���� ���� ���¿��� ���� Ż�� ����̸� ���� ����...
    if( mu8Make == FALSE )
    {
        if( mu16Mode == ICE_MODE_DUMMY_DEICING )
        {
            SetIceMakeMode( ICE_MODE_DEICING_OFF );
        }
    }
}

// �ܱ� �µ� 20'C ���� ���ǿ��� Ż�� �� �ð��� 
// �����ϱ� ���� ���� �˻� �Լ�
// Return 
// TRUE : 20'C �̸�  ����
// FALSE : 20'C �̻� ����
#define  DEFAULT_AMBI_TIMER   10  // @1sec
static U8 IsUnderTempAmbi(TEMP_T tAmbi )
{
    static U16 mu16AmbiTimer = DEFAULT_AMBI_TIMER;


    if( (GetTemp(TEMP_ID_AMBIENT_2) < tAmbi ) )
    {
        if( mu16AmbiTimer != 0 )
        {
            mu16AmbiTimer--;
        }
        else
        {
            return TRUE;
        }
    }
    else
    {
        mu16AmbiTimer = DEFAULT_AMBI_TIMER; // 5sec..
    }

    return FALSE;
}
   

/*
 *  �켱 ��忡 ����,
 *  ����/�ð� ���� ����.
 */
#if CONFIG_ENABLE_DEICING_COOLING
static void ManagerIceColdMode(void)
{
    U8 mu8ColdMode;
    U16 mu16IceMode;


    mu8ColdMode = GetColdWaterMakeMode();
    mu16IceMode  = GetIceMakeMode();


    /* ���� Ż���� �ֿ켱���� ó���Ѵ�. */
    if( IsSetIceMakeMode( ICE_MODE_DUMMY_DEICING ) == TRUE )
    {
        ClearColdWaterMakeMode( COLD_MODE_ALL );
        return ;
    }

    /* �ð��� ������ ���� ���� ������ ��쿡 ���� �˻�
     *
     * ���� �켱 - �ð� ��带 �����Ѵ�.
     *  ��, Ż�� �߿��� �ð����� ��ȯ�ȴ�.
     *
     * �ð� �켱 - ���� ��带 �����Ѵ�.
     *  ��, ���� �߿� �ð� ��尡 ������ ��쿡�� ���� ���� ������ �Ϸ��ϰ� �ð����� ��ȯ�Ѵ�.
     *
     */
    if( mu8ColdMode != COLD_MODE_NONE 
            && mu16IceMode != ICE_MODE_NONE )
    {
        if( GetIceTurbo() == TRUE )
        {
            /* ���� �켱, ���� ���� ��츦 �����ϰ�� �ð� ���ǿ����� �ð� ������ ��Ų��. */
            if( mu16IceMode != ICE_MODE_DEICING 
                    && mu16IceMode != ( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) 
                    && mu16IceMode != ( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) 
                    && mu16IceMode != ( ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) 
                    && mu16IceMode != ( ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) )
            {
                ClearColdWaterMakeMode( COLD_MODE_ALL );
            }

            // �ܱ� �µ��� 20'C �̻� ���ǿ�����
            // ���� Ż�� �� �ð� ������ �� �� �ִ�.
            // 20'C ���� ������ 10�� �̻� ���� �Ǿ�� �����Ѵ�.
            if( IsUnderTempAmbi( 20.0f ) == TRUE )
            {
                ClearColdWaterMakeMode( COLD_MODE_ALL );
            }
        }
        else
        {
            if( IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE
                  || IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
            {
                /* �ð� �켱, ���� ������ ����!!! */ 
                ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
            }
            else if( IsSetIceMakeMode( ICE_MODE_PREHEAT ) == TRUE
                    || IsSetIceMakeMode( ICE_MODE_READY ) == TRUE 
                    || IsSetIceMakeMode( ICE_MODE_ICING ) == TRUE )
            {
                /* ��, �ϴ� ������ �������ϰ�~! */
                ClearColdWaterMakeMode( COLD_MODE_ALL );
            }
            else if( IsSetIceMakeMode( ICE_MODE_DEICING ) == TRUE )
            {
                /* Ż�� �ܰ迡���� �ܱ� �µ� 20'c �̻� ������ Ż�� �� �ð� ��� */
                if( IsUnderTempAmbi( 20.0f ) == TRUE )
                {
                    ClearColdWaterMakeMode( COLD_MODE_ALL );
                }
            }
        }
    }
}
#else
static void ManagerIceColdMode(void)
{
    U8 mu8ColdMode;
    U16 mu16IceMode;


    mu8ColdMode = GetColdWaterMakeMode();
    mu16IceMode  = GetIceMakeMode();


    /* ���� Ż���� �ֿ켱���� ó���Ѵ�. */
    if( IsSetIceMakeMode( ICE_MODE_DUMMY_DEICING ) == TRUE )
    {
        ClearColdWaterMakeMode( COLD_MODE_ALL );
        return ;
    }

    /* �ð��� ������ ���� ���� ������ ��쿡 ���� �˻�
     *
     * ���� �켱 - �ð� ��带 �����Ѵ�.
     *  ��, Ż�� �߿��� �ð����� ��ȯ�ȴ�.
     *
     * �ð� �켱 - ���� ��带 �����Ѵ�.
     *  ��, ���� �߿� �ð� ��尡 ������ ��쿡�� ���� ���� ������ �Ϸ��ϰ� �ð����� ��ȯ�Ѵ�.
     *
     */
    if( mu8ColdMode != COLD_MODE_NONE 
            && mu16IceMode != ICE_MODE_NONE )
    {
        if( GetIceTurbo() == TRUE )
        {
            ClearColdWaterMakeMode( COLD_MODE_ALL );
        }
        else
        {
            if( IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE
                || IsSetIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING ) == TRUE )
            {
                /* �ð� �켱, ���� ������ ����!!! */ 
                ClearIceMakeMode( ICE_MODE_INIT | ICE_MODE_PREHEAT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING );
#if CONFIG_ICE_COLD_COMP
                SetIceMakeMode( ICE_MODE_DEICING_OFF );
#endif
            }
            else if( IsSetIceMakeMode( ICE_MODE_PREHEAT ) == TRUE 
                    || IsSetIceMakeMode( ICE_MODE_READY ) == TRUE 
                    || IsSetIceMakeMode( ICE_MODE_ICING ) == TRUE )
            {
                /* ��, �ϴ� ������ �������ϰ�~! */
                ClearColdWaterMakeMode( COLD_MODE_ALL );
            }
            // ICE 4.0
            //else if( IsSetIceMakeMode( ICE_MODE_DEICING ) == TRUE 
            //        || IsSetIceMakeMode( ICE_MODE_DEICING_OFF ) == TRUE )
            else if( IsSetIceMakeMode( ICE_MODE_DEICING ) == TRUE 
                    || IsSetIceMakeMode( ICE_MODE_DEICING_OFF ) == TRUE
                    || IsSetIceMakeMode( ICE_MODE_ICING_MOVE_DEICING ) == TRUE )
            {
                /* ��, �ϴ� Ż���� �������ϰ�~! */
                ClearColdWaterMakeMode( COLD_MODE_ALL );
            }

        }
    }
}
#endif // if CONFIG_ENABLE_DEICING_COOLING


void ProcessMake(void)
{
    /* Make Mode COLD & ICE */
    MakeIceMakeMode();        // ���� Make Mode ����
    MakeColdWaterMode();      // �ð� Make Mode ����
    ManagerIceColdMode();     // �ð�, ���� �켱 ���� ����

    /* Process Cold Water */
    ProcessMakeColdWater();   // �ð� MakeMode Handler

    /* Process Make Ice */
    ProcessMakeIce();         // ���� MakeMode Handler

    /* Process Compressor */
    ProcessComp();            // ����� ����
    ProcessFanMotor();        // DC FAN MOTOR

    /* Process Make Hot Water */
    ProcessMakeHotWater();    // �¼� ���� ����
}
