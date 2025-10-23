/**
 * File : process_sys_event.c
 *
 * Process Time Scheduler
*/

#include "process_sys_event.h"

#include "timer.h"
#include "water_out.h"
#include "ice_water_out.h"
#include "process_display.h"
#include "relay.h"
#include "valve.h"
#include "ice_tray.h"
#include "ice_tray_test.h"
#include "ice_full.h"
#include "ice_low.h"
#include "buzzer.h"
#include "key.h"
#include "gas_switch_valve.h"
#include "hotgas_switch_valve.h"
#include "process_key.h"

#include "lever.h"
#include "process_lever.h"

#include "api_step_motor.h"
#include "hal_serial.h"
#include "api_adc.h"
#include "led.h"
#include "input.h"
#include "api_key.h"

#include "circulate_pump.h"
#include "drain_pump.h"
#include "process_make.h"

#include "front.h"
#include "room_water.h"
#include "cold_water.h"
#include "hot_water.h"
#include "ice.h"
#include "ice_door.h"
#include "ice_maker.h"
#include "drain.h"
#include "swing_bar.h"
#include "eeprom.h"
#include "power_saving.h"
#include "leak.h"
#include "error.h"
#include "process_error.h"
#include "service.h"
#include "tank_clean.h"

#include "process_uv.h"
#include "health.h"
#include "ster.h"
#include "drain_water.h"
#include "rtc.h"

#include "eol.h"
#include "process_eol.h"
#include "fct.h"
#include "process_fct.h"
#include "time_short.h"

#include "parser.h"
#include "parser_front.h"

#include "uv.h"

#include "temp.h"
#include "sound.h"
#include "smps.h"

#include "comp.h"

#include "ice_care.h"
#include "process_ice_care.h"
#include "flow_meter.h"
#include "input.h"
#include "parser_comp.h"
#include "segment.h"
#include "purifier.h"
#include "smart_saving.h"
#include "aging.h"
#include "ice_feeder.h"

/**
 * @brief     Function For Initialize System
 * @detail    none
 * @param     none
 * @return    none
**/
void InitSystem(void)
{
    /* VESRION Export */
    ExportVersion(PROGRAM_VERSION);

    /* IO */
    InitRelay();
    InitValve();
    InitIceTray();
    InitIceTrayTest();
    InitDrainPump();
    InitCirculatePump();
    InitializeBuzzer();
    InitKey();
    InitComm();
    InitAdc();
    //InitLed();
    InitUv();

    /* IO - ICE */
    InitStepMotor();
    InitGasSwitch();
    InitHotGasSwitch();
    InitIceDoor();
    InitIceFeeder();
    InitSwingBar();

    InitServiceCheck();
    InitDisplay();

    /* WATER */
    InitRoomWater();
    InitColdWater();
    InitHotWater();
    InitWaterOut();
    InitIceWaterOut();
    InitFlowMeter();

    /* ICE */
    InitFront();
    InitIce();
    InitIceFull();
    InitIceLow();
    InitIceCare();

    /* ERROR */
    InitErrorList();

    /* SEGMENT */
    InitSegment();
    
    InitProcessMake();

    InitSaving();
    InitSmartSaving();
    InitEol();
    InitFct();
    InitTimeShort();
    InitAging();

    /* Ster */
#if( CONFIG_STER == 1 )
    InitSter();
    InitCSI();
#endif
    InitHealth();
    InitDrainWater();

    /* 10분동안 최초 냉수 센서 초기 읽기 */
    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    RegisterTimerISR(BuzzerControl);
    RegisterTimerISRFlow(FilterFlowMeterNoise);
}

/**
 * @brief     Function For Initialize Process Timer
 * @detail    none
 * @param     none
 * @return    none
**/
void InitStartTimer(void)
{
    InitTimer();

    StartTimer(TIMER_PROCESS, TIMER_ID_1MS,   1);
    StartTimer(TIMER_PROCESS, TIMER_ID_10MS,  10);
    StartTimer(TIMER_PROCESS, TIMER_ID_100MS, 100);
    StartTimer(TIMER_PROCESS, TIMER_ID_1SEC,  SEC(1));
    StartTimer(TIMER_PROCESS, TIMER_ID_10SEC, SEC(1));

    // TIMER_ID_40SEC는 온도 센서 ON/OFF 전용 타이머이다.
    // 전원 Reset 후 10분이 지난 후 40초 간격으로 온도 센서를 제어한다.
    StartTimer(TIMER_PROCESS, TIMER_ID_40SEC, SEC(600));

    StartTimer(TIMER_PROCESS, TIMER_ID_1MIN,  SEC(1));
#if( CONFIG_JIG_ECOLED_FRONT == 0 )
    StartTimer(TIMER_PROCESS, TIMER_ID_DEBUG, 100);
#endif
    StartTimer(TIMER_PROCESS, TIMER_ID_FRONT, 20);

#if( CONFIG_TEST_8585 == 1 )
    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(20));
#endif
#if( CONFIG_TEST_8585_2 == 1 )
    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(20));
#endif
#if( CONFIG_TEST_8585_ICE == 1 )
    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE, SEC(30));
#endif
#if( CONFIG_TEST_8585_ICE_WATER == 1 )
    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE_WATER, SEC(30));
#endif
#if( CONFIG_TEST_8585_HOT_REPEAT == 1 )
    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_HOT_REPEAT, SEC(20));
#endif

    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_100MS, 100);
    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_1SEC, SEC(5));
}

/**
 * Nomal Function
*/
/**
 * @brief     Function For Handing per 1ms
 * @detail    none
 * @param     none
 * @return    none
*/
static void Evt_1ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1MS, 1);

    ProcessAdc();
#if( CONFIG_STER == 1 )
    ControlCSI();
#endif
}

/**
 * @brief     Function For Handing per 10ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_10ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_10MS, 10);

    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();

    /* LEVER */
    ProcessScanLever();
    ProcessLeverEventHandler();

    /* WATER OUT */
    ControlWaterOut();
    ControlIceWaterOut();
    ControlValve();
    ControlCirculatePump();

    ControlIceTray();
    ProcessRelayConcurrent();
    
    ControlSwingBar();

    ProcessInput();

    TestAutoCountingFlowMeter();
}

/**
 * @brief     Function For Handing per 100ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_100ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_100MS, 100);

    ProcessTemp();
    
    ProcessCheckTankOpen();   /* Service Check - Tank Open */
    ProcessTankLevel();       /* DETECT TANK WATER LEVEL */
#if( CONFIG_LEAK == 1 )
    ProcessLeak();            /* Detect water leakage */
#endif

    ControlRoomWaterLevel();  /* Control Room Tank Water Level */
    ControlColdWaterLevel();  /* Control Cold Tank Water Level */
    ControlHotWaterLevel();   /* Control Hot Tank Water Level */
    ControlDrainLevel();      /* Control Drain Tank Water Level */

    if( GetHotGasSwitchStatus() != HOTGAS_SWITCH_BUSY )
    {
        ControlGasSwitch();
    }

    if( GetGasSwitchStatus() != GAS_SWITCH_BUSY )
    {
        ControlHotGasSwitch();
    }
    
    ControlSmps();
    ControlIceDoor();
    ProcessIceFeeder();

    ProcessError();
    ProcessDisplay();

    ProcessIceTrayTest();

    /* Flow Meter */
    ProcessFlowMeter();

    /* ICE */
    CheckIceFull(); 
    CheckIceLow(); 
    UpdateIceMakerTimer();

    UpdateEolTimer();

    ProcessAging();

#if( CONFIG_STER == 1 )
    /* HEALTH - STER/DRAIN WATER */
    ControlSter();
#endif

#if( CONFIG_TEST_8585 == 0 )
    ProcessUv();
#endif
    ControlDrainWater();
}

/**
 * @brief     Function For Handing per 1sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_1sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1SEC, SEC(1));

    /* Make Cold Water */
    MakeColdWater();

    /* Make ICE */
    MakeIce();
    MakeIceFull();

    /* Make Hot Water */
    MakeHotWater();

    //ProcessUnusedSaving();
    ProcessSmartSaving();

    /* Process making hot water & cold water & ice */
    ProcessMake();

    /* 얼음 장기 방치 시, 붙은 얼음을 해제 시키는 제어 및 만빙 시 얼음을 추출부 가까이 위치 시키는 제어 */
    ProcessIceCare();

    ProcessRelayExclusive();
    ControlDrainPump();

#if 0
    /* Power Saving */
    ProcessSaving();
#endif

    /* EEPROM Writting */
    ProcessEeprom();

    UpdateFctTimer();

    // Time Short 
    UpdateTimeShortTimer();
    ProcessTimeShort();
    
    /* ICE DOOR FORCED CLOSED */
    ControlForcedCloseIceDoor();
    CountingIceDoorOpeningTime();

#if( CONFIG_STER == 1 )

#if( CONFIG_RTC_TEST == 1 )
    if( GetWaterOut() == FALSE )
    {
        ProcessRtc();
    }
#endif
    ProcessPeriodDay();
    Error_Check_CSI();
#endif

    /* HEALTH */
    ControlHealth();

    ProcessTankClean();
    ProcessGasSwitch(); 
    ProcessHotGasSwitch();
}

/**
 * @brief     Function For Handing per 10sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_10sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_10SEC, SEC(10));

#if( CONFIG_STER == 1 )
    /* RTC READ & WRITE */
#if( CONFIG_RTC_TEST == 0 )
    if( GetWaterOut() == FALSE )
    {
        ProcessRtc();
    }
#endif
#endif
}

/**
 * @brief     Function For Handing per 40sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_40sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_40SEC, SEC(40));

    /// 10초 주기로 1초 동안만 냉수 센서 읽기
    if( (GetColdWaterConfigMake() == TRUE) 
         || (GetTempMonitoring() == TRUE) )
    {
        TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_TEMP_READ_TIME );
    }

    TurnOnTempSensor( TEMP_ID_ROOM_WATER, DEFAULT_TEMP_READ_TIME );
}

/**
 * @brief     Function For Handing per 1min
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_1min_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1MIN, SEC(60));

    /* SERVICE CHECK TIMER */
    ProcessServiceCheck();

    /* UNSUED TIME ( SILVER CARE ) */
    CountingUnusedTime();
}

static void Evt_Debug_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_COMM_DEBUG_TX, 0 );
    StartTimer(TIMER_PROCESS, TIMER_ID_DEBUG, 150 );
}

// 50ms 주기로 FRONT COMMUNICATION
#define FRONT_REFRESH_TIME     50 // @50ms...
static void Evt_Front_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_FRONT, FRONT_REFRESH_TIME);

    SetCommHeader(COMM_ID_FRONT, PKT_REQ_FRONT);
    StartTimer(TIMER_USER, TIMER_USER_ID_COMM_FRONT_TX, 0);
}

static void Evt_DrainPump_Handler(void)
{
    StartDrainPumpOnLevel();
    //TurnOnDrainPump();
}

// FRONT RX 통신 에러 헨들러
static void Evt_FrontRxErr_Handler(void)
{
    // 강제로 아이스 도어 닫음
    if( IsOpenIceDoor() == TRUE )
    {
        CloseIceDoor();
        Sound(SOUND_EFFLUENT_END);
    }
}

/**
 * FCT Function
*/
/**
 * @brief     Function For Handing per 1ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Fct_1ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1MS, 1);

    // 테스트 모드에선 상시 ON
    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    ProcessAdc();
}

/**
 * @brief     Function For Handing per 10ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Fct_10ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_10MS, 10);

    /* KEY BUTTON */
    ProcessScanKey();
    ProcessKeyEventHandler();

    /* LEVER */
    ProcessScanLever();
    ProcessLeverEventHandler();

    ProcessInput();
    ProcessFctLoadTest();

    CheckIceFull(); 
    CheckIceLow(); 
    ControlValve();
}

/**
 * @brief     Function For Handing per 100ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Fct_100ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_100MS, 100);

}

/**
 * @brief     Function For Handing per 1sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Fct_1sec_Handler(void)
{
    StartTimer( TIMER_PROCESS, TIMER_ID_1SEC, SEC(1) );

    /* Fct Timer */
    UpdateFctTimer();

    /* Temp Sensor */
    ProcessTemp();

    /* Relay */
    ProcessRelayExclusive();
    ProcessRelayConcurrent();

    /* Fct */
    ProcessFct();
}

/**
 * @brief     Function For Handing per 10sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Fct_10sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_10SEC, SEC(10));
}

/**
 * @brief     Function For Handing per 40sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Fct_40sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_40SEC, SEC(40));
}

/**
 * @brief     Function For Handing per 1min
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Fct_1min_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1MIN, SEC(60));
}


/**
 * Eol Function
**/
/**
 * @brief     Function For Handing per 1ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Eol_1ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1MS, 1);

    TurnOnTempSensor( TEMP_ID_COLD_WATER, DEFAULT_INIT_TEMP_READ_TIME );
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, DEFAULT_INIT_TEMP_READ_TIME );

    ProcessAdc();
}

/**
 * @brief     Function For Handing per 10ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Eol_10ms_Handler(void)
{
    U8 eolMode = 0U;

    StartTimer(TIMER_PROCESS, TIMER_ID_10MS, 10);

    /* Gpio Input */
    ProcessInput();
    
    /* KEY */
    ProcessScanKey();
    ProcessKeyEventHandler();

    /* LEVER KEY */
    ProcessScanLever();
    ProcessLeverEventHandler();

    /* Relay */
    ProcessRelayConcurrent();

    /* WATER OUT */
    eolMode = GetEolMode();
    if( eolMode == EOL_MODE_READY 
            || eolMode == EOL_MODE_DONE )
    {
        ControlWaterOutEol();
        ControlIceWaterOutEol();
        ControlCirculatePump();
    }

    if( eolMode == EOL_MODE_READY 
            || eolMode == EOL_MODE_INIT
            || eolMode == EOL_MODE_CHECK_SENSOR
            || eolMode == EOL_MODE_DONE
            || eolMode == EOL_MODE_SEALED_INIT 
            || eolMode == EOL_MODE_SEALED )
    {
        ControlIceTray();
    }

    if( eolMode == EOL_MODE_INIT
            || eolMode == EOL_MODE_CHECK_SENSOR
            || eolMode == EOL_MODE_CHECK_LOAD )
    {
        ControlCirculatePumpEol();
    }

    /* Valve */
    ControlValve();

    /* IceFull */
    //CheckIceFull(); 
    //CheckIceLow(); 
}

/**
 * @brief     Function For Handing per 100ms
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Eol_100ms_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_100MS, 100);

    ProcessCheckTankOpen();   /* Service Check - Tank Open */
    ProcessTankLevel();       /* DETECT TANK WATER LEVEL */

    /* Temp Sensor */
    ProcessTemp();

    /* Flow Meter */
    ProcessFlowMeter();

    /* Led Display */
    ControlIceDoor();
    ProcessIceFeeder();
    ProcessDisplay();

    /* Eol */
    UpdateEolTimer();
    ProcessEol();

    if( GetHotGasSwitchStatus() != HOTGAS_SWITCH_BUSY )
    {
        ControlGasSwitch();
    }

    if( GetGasSwitchStatus() != GAS_SWITCH_BUSY )
    {
        ControlHotGasSwitch();
    }
    
    ControlSmps();    
}

/**
 * @brief     Function For Handing per 1sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Eol_1sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1SEC, SEC(1));

    /* Comp */
    ControlCompBldc();

    /* Relay */
    ProcessRelayExclusive();

    /* Drain */
    ControlDrainWater();
}

/**
 * @brief     Function For Handing per 10sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Eol_10sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_10SEC, SEC(10));
}

/**
 * @brief     Function For Handing per 40sec
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Eol_40sec_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_40SEC, SEC(40));
    
}

/**
 * @brief     Function For Handing per 1min
 * @detail    none
 * @param     none
 * @return    none
**/
static void Evt_Eol_1min_Handler(void)
{
    StartTimer(TIMER_PROCESS, TIMER_ID_1MIN, SEC(60));

}

static void Evt_Comp_Handler( void )
{
    StartTimer(TIMER_PROCESS, TIMER_ID_COMM_COMP_REQUEST, SEC(2));

    SetCommHeader(COMM_ID_COMP, CMD_COMP_REQ);
    StartTimer(TIMER_USER, TIMER_USER_ID_COMM_COMP_TX, 0);
}

// COMP BLDC COMM ERROR
static void Evt_Comp_Rx_Err_Handler( void )
{
    SetCompBldcErrorComm( TRUE );
}

static void Evt_Test_1sec_Handler( void )
{
    // sound test..
    //TestSound();

    //TEST_ControlRelay();
    //TEST_ControlValve();

    //StartTimer( TIMER_ID_TEST_1SEC, SEC(2) );
}

static void Evt_Test_Ster_Handler( void )
{
    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_STER_CONTY, SEC(60) );

#if 0
    if( IsStartSter() == FALSE )
    {
        if( GetRoomWaterLevel() == LEVEL_HIGH 
                && GetColdWaterLevel() == LEVEL_HIGH )
        {
            StartSterMode( STER_MODE_ALL );
        }
    }
#endif
}

#if( CONFIG_TEST_8585 == 1 )
// 신뢰성 테스트 프로그램
// 85,85 테스트
// 1시간(60분, 3600초) 마다 냉수 5초(200mL)추출
#define  TEST_COLDWATER_OUT_TIME  (5U)     // 5초
#define  TEST_COLDWATER_OUT_WAIT  (3600U)  // 3600초 ( 60분 * 60초 = 1시간)
U8 the_8585_step = 0;
static void Evt_Test_8585_Handler( void )
{
    if( GetRoomWaterInitFull() != TRUE 
            || GetColdWaterInitFull() != TRUE )
    {
        the_8585_step = 0;
        StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC( 1U ));
        return ;
    }

    switch( the_8585_step )
    {
        // COLD
        case 0:
            if( GetWaterOut() == TRUE )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC( 10U ));
                break;
            }
            SetWaterOutSelect( SEL_WATER_COLD );
            StartWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC( TEST_COLDWATER_OUT_TIME ));
            the_8585_step++;
            break;

        case 1:
            StopWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC( TEST_COLDWATER_OUT_WAIT ));
            the_8585_step++;
            break;

        case 2:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC( 1U ));
            the_8585_step = 0;
            break;

        default:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, 0U);
            the_8585_step = 0;
            break;
    }
}

#endif

#if 0
// 신뢰성 테스트 프로그램
// 85,85 테스트
// 5분 마다 정수/냉수/온수 추출, 얼음 추출 10초
#define  TEST_WATER_OUT_TIME  5
static void Evt_Test_8585_Handler( void )
{
    static U8 uStep = 0;

    // 냉수, 정수 최초 만수위 조건 검사
    if( GetRoomWaterInitFull() != TRUE 
            || GetColdWaterInitFull() != TRUE )
    {
        uStep = 0;
        StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(1));
        return ;
    }

    switch( uStep )
    {
        // 온수 추출
        case 0:
            SetWaterOutSelect( SEL_WATER_HOT );
            StartWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(TEST_WATER_OUT_TIME));

            uStep++;
            break;

        case 1:
            StopWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(1));
            uStep++;
            break;

            // 냉수 추출
        case 2:
            SetWaterOutSelect( SEL_WATER_COLD );
            StartWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(TEST_WATER_OUT_TIME));
            uStep++;
            break;

        case 3:
            StopWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(1));
            uStep++;
            break;

            // 정수 추출 
        case 4:
            SetWaterOutSelect( SEL_WATER_ROOM );
            StartWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(TEST_WATER_OUT_TIME));
            uStep++;
            break;

        case 5:
            StopWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(1));
            uStep++;
            break;

            // 얼음 추출 시작
        case 6:
            OpenIceDoor();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(20));
            uStep++;
            break;

            //얼음 추출 정지
        case 7:
            CloseIceDoor();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(1));
            uStep++;
            break;

            // 5분 대기
        default:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585, SEC(300));
            uStep = 0;
            break;
    }
}
#endif

#if( CONFIG_TEST_8585_2 == 1 )
U8 the_8585_step_2 = 0;
#define  TEST2_ROOMWATER_OUT_TIME  (5U)     // 5초
#define  TEST2_ROOMWATER_OUT_WAIT  (5U)     // 5초
#define  TEST2_COLDWATER_OUT_TIME  (5U)     // 5초
#define  TEST2_COLDWATER_OUT_WAIT  (5U)     // 5초
#define  TEST2_HOTWATER_OUT_TIME   (5U)     // 5초
#define  TEST2_HOTWATER_OUT_WAIT   (300U)   // 300초 ( 5분 * 60초 = 5분 )
static void Evt_Test_8585_2_Handler( void )
{
    if( GetRoomWaterInitFull() != TRUE 
            || GetColdWaterInitFull() != TRUE )
    {
        the_8585_step_2 = 0;
        StartTimer( TIMER_ID_TEST_8585_2, SEC( 1U ) );
        return ;
    }

    switch( the_8585_step_2 )
    {
        // ROOM
        case 0:
            if( GetWaterOut() == TRUE )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(10U));
                break;
            }
            SetWaterOutSelect(TIMER_PROCESS, SEL_WATER_ROOM);
            StartWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(TEST2_ROOMWATER_OUT_TIME));
            the_8585_step_2++;
            break;

        case 1:
            StopWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(TEST2_ROOMWATER_OUT_WAIT));
            the_8585_step_2++;
            break;

        // COLD
        case 2:
            if( GetWaterOut() == TRUE )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(10U));
                break;
            }
            SetWaterOutSelect( SEL_WATER_COLD );
            StartWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(TEST2_COLDWATER_OUT_TIME));
            the_8585_step_2++;
            break;

        case 3:
            StopWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(TEST2_COLDWATER_OUT_WAIT));
            the_8585_step_2++;
            break;

        // HOT
        case 4:
            if( GetWaterOut() == TRUE )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(10U));
                break;
            }
            SetWaterOutSelect( SEL_WATER_HOT );
            StartWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(TEST2_HOTWATER_OUT_TIME));
            the_8585_step_2++;
            break;

        case 5:
            StopWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(TEST2_HOTWATER_OUT_WAIT));
            the_8585_step_2++;
            break;       

        case 6:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, SEC(1U));
            the_8585_step_2 = 0;
            break;

        default:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_2, 0U);
            the_8585_step_2 = 0;
            break;
    }
}
#endif

#if( CONFIG_TEST_8585_ICE == 1 )
// 신뢰성 테스트 프로그램 ( 얼음 추출 )
// 85,85 테스트
// 85,85,2 테스트 시, 얼음 만빙 시 얼음 5min -> 2min 추출로 변경해야 함
// 얼음 만빙 시 얼음 5min 추출
#define  TEST_ICE_OUT_TIME_2MIN  (120U)     // 2분
#define  TEST_ICE_OUT_TIME_5MIN  (300U)     // 5분
U8 the_8585_step_ice = 0;
static void Evt_Test_8585_Ice_Handler( void )
{
    switch( the_8585_step_ice )
    {
        // ICE OUT
        case 0:
            if( GetIceFullStatus() == TRUE )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE, SEC(20U));
                the_8585_step_ice++;
            }
            else
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE, SEC(1U));
            }
            break;

        case 1:
            OpenIceDoor();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE, SEC(TEST_ICE_OUT_TIME_5MIN));
            the_8585_step_ice++;
            break;

        case 2:
            CloseIceDoor();
            StartCheckIceFull();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE, SEC(60U));
            the_8585_step_ice = 0;
            break;

        default:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE, SEC(0U));
            the_8585_step_ice = 0;
            break;
    }
}
#endif

#if( CONFIG_TEST_8585_ICE_WATER == 1 )
// 신뢰성 테스트 프로그램 ( 얼음물 추출 )
// 85,85 테스트
// 1시간 8분(68분, 4080초) 마다 얼음물 500mL 정량추출
#define  TEST_ICEWATER_OUT_TIME  (5U)     // 5초
#define  TEST_ICEWATER_OUT_WAIT  (4080U)  // 4080초 ( 68분 * 60초 = 1시간 8분)
U8 the_8585_step_ice_water = 0;
static void Evt_Test_8585_Ice_Water_Handler( void )
{
    if( GetRoomWaterInitFull() != TRUE 
            || GetColdWaterInitFull() != TRUE )
    {
        the_8585_step_ice_water = 0;
        StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE_WATER, SEC( 1U ));
        return ;
    }

    switch( the_8585_step_ice_water )
    {
        // ICE WATER
        case 0:
            if(  IsStartIceDoorWater() == TRUE )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE_WATER, SEC( 10U ));
                break;
            }
            SetIceWaterSelect( SEL_ICE_WATER );
            SetIceWaterOutContinue(TRUE);
            SetIceWaterOutFixedAmountType(FIXED_ICE_WATER_AMOUNT_500ML);    /// 용량 선택
            OpenIceDoorWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE_WATER, SEC( 1U ));
            the_8585_step_ice_water++;
            break;

        case 1:
            if( IsStartIceDoorWater() == TRUE )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE_WATER, SEC( 10U ));
                break;
            }

            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE_WATER, SEC( TEST_ICEWATER_OUT_WAIT ));
            the_8585_step_ice_water++;
            break;

        case 2:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE_WATER, SEC( 1U ));
            the_8585_step_ice_water = 0;
            break;

        default:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_ICE_WATER, 0U);
            the_8585_step_ice_water = 0;
            break;
    }
}

#endif

#if( CONFIG_TEST_8585_FEEDER == 1 )
#define TEST_FEEDER_CW_TIME     (3U)   // 3sec
#define TEST_FEEDER_STOP_TIME   (5U)   // 5sec
U8 the_8585_feeder = FALSE;
U8 the_8585_step_feeder = 0;
static void Evt_Test_8585_Feeder_Handler(void)
{
    switch( the_8585_step_feeder )
    {
        // ICE OUT
        case 0:
            TurnOnRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_FEEDER, SEC(TEST_FEEDER_CW_TIME));
            the_8585_step_feeder++;
            break;

        case 1:
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_FEEDER, SEC(TEST_FEEDER_STOP_TIME));
            the_8585_step_feeder++;
            break;

        case 2:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_FEEDER, 100);
            the_8585_step_feeder = 0;
            break;

        default:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_FEEDER, SEC(0U));
            the_8585_step_feeder = 0;
            break;
    }
}
#endif

#if( CONFIG_TEST_8585_BACK_FEEDER == 1 )
#define TEST_FEEDER_CCW_TIME         (3U)   // 3sec
#define TEST_BACK_FEEDER_STOP_TIME   (5U)   // 5sec
U8 the_8585_back_feeder = FALSE;
U8 the_8585_step_back_feeder = 0;
static void Evt_Test_8585_Back_Feeder_Handler(void)
{
    switch( the_8585_step_back_feeder )
    {
        // ICE OUT
        case 0:
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOnRelay( RELAY_FEEDER_CCW );
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_BACK_FEEDER, SEC(TEST_FEEDER_CCW_TIME));
            the_8585_step_back_feeder++;
            break;

        case 1:
            TurnOffRelay( RELAY_FEEDER_CW );
            TurnOffRelay( RELAY_FEEDER_CCW );
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_BACK_FEEDER, SEC(TEST_BACK_FEEDER_STOP_TIME));
            the_8585_step_back_feeder++;
            break;

        case 2:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_BACK_FEEDER, 100);
            the_8585_step_back_feeder = 0;
            break;

        default:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_BACK_FEEDER, SEC(0U));
            the_8585_step_back_feeder = 0;
            break;
    }
}
#endif

#if( CONFIG_TEST_8585_HOT_REPEAT == 1 )
// 신뢰성 테스트 프로그램 ( 온수 반복 추출 )
// 85,85 테스트
// 온수 250ml 추출 -> 휴지 5초 5회 후 30분 휴지
#define  TEST_HOTWATER_REPEAT_COUNT  (5U)     // 5회
#define  TEST_HOTWATER_OUT_WAIT      (5U)     // 5초
#define  TEST_HOTWATER_OUT_WAIT_LONG (1800U)  // 1800초 ( 30분 * 60초 = 30분 )

U8 the_8585_step_hot_water = 0;
U8 the_8585_step_hot_water_count = 0;
static void Evt_Test_8585_Hot_Water_Handler( void )
{
    if( GetRoomWaterInitFull() != TRUE 
            || GetColdWaterInitFull() != TRUE )
    {
        the_8585_step_hot_water = 0;
        StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_HOT_REPEAT, SEC( 1U ));
        return ;
    }

    switch( the_8585_step_hot_water )
    {
        // HOT WATER 250ml 추출
        case 0:
            if( (GetWaterOut() == TRUE) )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_HOT_REPEAT, SEC( 10U ));
                break;
            }
            SetWaterOutSelect(SEL_WATER_HOT);
            SetWaterOutContinue(TRUE);
            SetWaterOutFixedAmountType(FIXED_AMOUNT_500ML);    /// 용량 선택
            StartWaterOut();
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_HOT_REPEAT, SEC( 1U ));
            the_8585_step_hot_water++;
            break;

        case 1:
            if( GetWaterOut() == TRUE )
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_HOT_REPEAT, SEC( 10U ));
                break;
            }

            the_8585_step_hot_water_count++;
            if( the_8585_step_hot_water_count > TEST_HOTWATER_REPEAT_COUNT )
            {
                the_8585_step_hot_water_count = 0U;
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_HOT_REPEAT, SEC( TEST_HOTWATER_OUT_WAIT_LONG ));
            }
            else
            {
                StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_HOT_REPEAT, SEC( 5U ));
            }
            the_8585_step_hot_water = 0;
            break;

        default:
            StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_HOT_REPEAT, 0U);
            the_8585_step_hot_water = 0;
            break;
    }
}

#endif

static void Evt_Test_100ms_Handler( void )
{
    // key test...

    StartTimer(TIMER_PROCESS, TIMER_ID_TEST_100MS, 100);
}

/**
 * Process
**/
typedef void (*FPAction_T)(void);
typedef struct _sys_event_
{
    U8          timerId;
    FPAction_T  func;
    FPAction_T  funcDiag;
    FPAction_T  funcFct;
    FPAction_T  funcEol;
} SSysEvent_T;

const static SSysEvent_T SysEventList[] =
{
    /* Process Scheduler Timer */
    /* TIMER ID,                        NORMAL,                       DIAGNOSIS,      FCT,                        EOL */
    { TIMER_ID_1MS,                     Evt_1ms_Handler,              NULL,           Evt_Fct_1ms_Handler,        Evt_Eol_1ms_Handler           },
    { TIMER_ID_10MS,                    Evt_10ms_Handler,             NULL,           Evt_Fct_10ms_Handler,       Evt_Eol_10ms_Handler          },
    { TIMER_ID_100MS,                   Evt_100ms_Handler,            NULL,           Evt_Fct_100ms_Handler,      Evt_Eol_100ms_Handler         },
    { TIMER_ID_1SEC,                    Evt_1sec_Handler,             NULL,           Evt_Fct_1sec_Handler,       Evt_Eol_1sec_Handler          },
    { TIMER_ID_10SEC,                   Evt_10sec_Handler,            NULL,           Evt_Fct_10sec_Handler,      NULL                          },
    { TIMER_ID_40SEC,                   Evt_40sec_Handler,            NULL,           Evt_Fct_40sec_Handler,      NULL                          },
    { TIMER_ID_1MIN,                    Evt_1min_Handler,             NULL,           Evt_Fct_1min_Handler,       Evt_Eol_1min_Handler          },

    { TIMER_ID_FRONT,                   Evt_Front_Handler,            NULL,           Evt_Front_Handler,          Evt_Front_Handler             },

#if( (CONFIG_JIG_RBK_FRONT == 0) && (CONFIG_JIG_WOORIM_FRONT == 0) )
    { TIMER_ID_DEBUG,                   Evt_Debug_Handler,            NULL,           Evt_Debug_Handler,          Evt_Debug_Handler             },
    { TIMER_ID_DRAIN_PUMP_24H,          Evt_DrainPump_Handler,        NULL,           NULL,                       NULL                          },
#endif

    { TIMER_ID_COMM_FRONT_RX_ERR,       Evt_FrontRxErr_Handler,       NULL,           NULL,                       NULL                          },

    { TIMER_ID_COMM_COMP_REQUEST,       Evt_Comp_Handler,             NULL,           Evt_Comp_Handler,           Evt_Comp_Handler              },
    { TIMER_ID_COMM_COMP_RX_ERR,        Evt_Comp_Rx_Err_Handler,      NULL,           NULL,                       Evt_Comp_Rx_Err_Handler       },

    /* TEST */
    { TIMER_ID_TEST_STER_CONTY,         Evt_Test_Ster_Handler,        NULL,           NULL,                       NULL                          },

#if( CONFIG_TEST_8585 == 1 )
    { TIMER_ID_TEST_8585,               Evt_Test_8585_Handler,        NULL,           NULL,                       NULL                          },
#endif
#if( CONFIG_TEST_8585_2 == 1 )
    { TIMER_ID_TEST_8585_2,             Evt_Test_8585_2_Handler,      NULL,           NULL,                       NULL                          },
#endif
#if( CONFIG_TEST_8585_ICE == 1 )
    { TIMER_ID_TEST_8585_ICE,           Evt_Test_8585_Ice_Handler,    NULL,           NULL,                       NULL                          },
#endif
#if( CONFIG_TEST_8585_ICE_WATER == 1 )
    { TIMER_ID_TEST_8585_ICE_WATER,     Evt_Test_8585_Ice_Water_Handler,    NULL,           NULL,                       NULL                          },
#endif
#if( CONFIG_TEST_8585_FEEDER == 1 )
    { TIMER_ID_TEST_8585_FEEDER,        Evt_Test_8585_Feeder_Handler,      NULL,           NULL,                       NULL                          },
#endif
#if( CONFIG_TEST_8585_BACK_FEEDER == 1 )
    { TIMER_ID_TEST_8585_BACK_FEEDER,   Evt_Test_8585_Back_Feeder_Handler, NULL,           NULL,                       NULL                          },
#endif
#if( CONFIG_TEST_8585_HOT_REPEAT == 1 )
    { TIMER_ID_TEST_8585_HOT_REPEAT,    Evt_Test_8585_Hot_Water_Handler,   NULL,           NULL,                       NULL                          },
#endif

    { TIMER_ID_TEST_1SEC,               Evt_Test_1sec_Handler,        NULL,           NULL,                       NULL                          }
};
#define	SZ_SYS_EVENT_LIST     ( sizeof(SysEventList)/sizeof(SSysEvent_T))

/**
 * @brief     Function For Scheduler Of Process
 * @detail    Unit : 1ms
 * @param     none
 * @return    none
**/
void ProcessEventHandler( void )
{
    U8          i = 0U;
    FPAction_T  func = NULL;
    SSysEvent_T *pSysEventList = NULL;

    for( i=0; i<SZ_SYS_EVENT_LIST; i++ )
    {
        pSysEventList = &SysEventList[i];

        if( IsExpiredTimer(TIMER_PROCESS, pSysEventList->timerId) == TIMER_EXPIRE )
        {
            // Disable Expired Timer
            DisableTimer(TIMER_PROCESS, pSysEventList->timerId);

            // NORMAL or EOL
            if( GetEolStatus() == TRUE )
            {
                // Get Eol Test Mode Event Handler
                func = pSysEventList->funcEol;
            }
            else if( GetFctStatus() == TRUE )
            {
                // Get Eol Test Mode Event Handler
                func = pSysEventList->funcFct;
            }
            else
            {
                // Get Normal Mode Event Handler
                func = pSysEventList->func;
            }

            // Call a event handl
            if( func != NULL )
            {
                func();
            }
        }
    }
}
