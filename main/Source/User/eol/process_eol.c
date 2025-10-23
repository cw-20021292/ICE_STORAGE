#include "hw.h"
#include "process_eol.h"
#include "eol.h"
#include "parser_eol.h"

#include "key.h"
#include "eeprom.h"
#include "rtc.h"
#include "util.h"
#include "comm.h"
#include "timer.h"

#include "valve.h"
#include "relay.h"
#include "service.h"
#include "ice_door.h"
#include "gas_switch_valve.h"
#include "hotgas_switch_valve.h"
#include "temp.h"
#include "api_adc.h"
#include "api_pump.h"
#include "api_uv.h"
#include "ice_full.h"
#include "ice_low.h"
#include "ice_tray.h"
//#include "health_maker.h"
#include "level.h"
#include "circulate_pump.h"
#include "drain_pump.h"
#include "err_temp_eva_1.h"
#include "err_temp_eva_2.h"
#include "cold_water.h"
#include "input.h"
#include "flow_meter.h"
#include "ice_water_out.h"

// 냉매 유로 시스템 검사
enum
{
    EOL_ICE_INIT_ICE,
    EOL_ICE_CHECK_ICE,
    EOL_ICE_SWITCH,
    EOL_ICE_WAIT_SWITCH,
    EOL_ICE_INIT_COLD,
    EOL_ICE_CHECK_COLD,
    EOL_ICE_DONE
};

static void CheckSensorFlowMeter(void);

static void CheckEeprom(void)
{
    static U8 mu8Init = FALSE;
    static U8 mu8RetryCount = 5;
    U8 mu8Ret = FALSE;

    if( mu8Init == TRUE )
    {
        return ;
    }

    // Check eeprom
    mu8Ret = TestEeprom();
    if( mu8Ret == TRUE )
    {
        mu8Init = TRUE;

        SetEolCheckStatus(EOL_CHECK_ID_EEPROM, 0xA5);  // OK
    }
    else
    {
        SetEolCheckStatus(EOL_CHECK_ID_EEPROM, 0x01);  // ERROR
    }

    if( --mu8RetryCount == 0 )
    {
        mu8Init = TRUE;
    }
}

static U8 MovingIceTray(U8 mu8Step )
{
    switch( mu8Step )
    {
        case 0:
            MoveIceTrayDeIcing();
            mu8Step++;
            break;

        case 1:
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                mu8Step++;
            }
            break;

        case 2:
            MoveIceTrayIcing();
            mu8Step++;
            break;

        case 3:
            if( GetIceTrayStatus() == ICE_TRAY_ICING )
            {
                mu8Step++;
            }
            break;

        case 4:
            mu8Step = 0;
            break;

        default :
            mu8Step = 0;
            break;
    }

    return mu8Step;
}

static void TestIceTray(void)
{
    static U8 mu8Step = 0;

    mu8Step = MovingIceTray( mu8Step );
    if( mu8Step == 2 )
    {
        SetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_DEICING, 1);
    }
    else if( mu8Step == 4 )
    {
        SetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_ICING, 1);
    }
}

static void TestDoneIceTray(void)
{
    static U8 mu8Step = 0;
    static U8 init = FALSE;

    if( init == TRUE )
    {
        return;
    }

    mu8Step = MovingIceTray( mu8Step );
    if( mu8Step == 4 )
    {
        mu8Step = 0;
        init = TRUE;
    }
}

static void CheckSensorFan(void)
{
    U16 tmp = 0U;
    U16 adc = 0U;
    static U16 time = 50;  // 5sec

    if( time != 0 )
    {
        time--;
        TURN_OFF_MOTOR_FAN();
    }
    else
    {        
        TURN_ON_MOTOR_FAN();
    }

    if( GET_STATUS_MOTOR_FAN() == HIGH )
    {
        tmp = (U16)GetEolCheckStatus(EOL_CHECK_ID_FAN);
        adc = GetAdcData(ADC_ID_FAN_IAD);

        if( tmp < adc )
        {
            SetEolCheckStatus( EOL_CHECK_ID_FAN, (U32)adc );
        }
    }
    else
    {
        adc = GetAdcData(ADC_ID_FAN_IAD);
        SetEolCheckStatus(EOL_CHECK_ID_FAN_OFF, (U32)adc);
    }
}

static void CheckSensorDrainPump(void)
{
    U16 tmp;
    U16 adc;
    static U16 time = 50;  // 5sec

    if( time != 0 )
    {
        time--;
        TurnOnPump(PUMP_ID_DRAIN, 100);
    }
    else
    {
        TurnOffPump(PUMP_ID_DRAIN);
    }

    if( IsTurnOnDrainPump() == TRUE )
    {
        tmp = (U16)GetEolCheckStatus(EOL_CHECK_ID_DRAIN_PUMP);
        adc = GetAdcData(ADC_ID_PUMP_DRAIN_IAD);
        if( tmp < adc )
        {
            SetEolCheckStatus(EOL_CHECK_ID_DRAIN_PUMP, (U32)adc);
        }
    }
    else
    {        
        adc = GetAdcData(ADC_ID_PUMP_DRAIN_IAD);
        SetEolCheckStatus(EOL_CHECK_ID_DRAIN_PUMP_OFF, (U32)adc);
    }
}

static void CheckSensorDrainIcePump(void)
{
    U16 tmp;
    U16 adc;
    static U16 time = 50;  // 5sec

    if( time != 0 )
    {
        time--;
        TurnOnPump(PUMP_ID_ICE_DRAIN, 100);
    }
    else
    {
        TurnOffPump(PUMP_ID_ICE_DRAIN);
    }

    if( IsTurnOnIceDrainPump() == TRUE )
    {
        tmp = (U16)GetEolCheckStatus(EOL_CHECK_ID_ICE_DRAIN_PUMP);
        adc = GetAdcData(ADC_ID_PUMP_ICE_DRAIN_IAD);
        if( tmp < adc )
        {
            SetEolCheckStatus(EOL_CHECK_ID_ICE_DRAIN_PUMP, (U32)adc);
        }
    }
    else
    {
        adc = GetAdcData(ADC_ID_PUMP_ICE_DRAIN_IAD);
        SetEolCheckStatus(EOL_CHECK_ID_ICE_DRAIN_PUMP_OFF, (U32)adc);
    }
}

static void CheckSensorCircualtePump(void)
{
    U16 tmp;
    U16 adc;

    //TurnOnCirculatePumpMakeColdForced();

    if( GET_STATUS_PUMP_CIRCULATE() == HIGH )
    {
        tmp = (U16)GetEolCheckStatus(EOL_CHECK_ID_CIRCULATE_PUMP);
        adc = GetAdcData(ADC_ID_PUMP_COLD_IAD);
        if( tmp < adc )
        {
            SetEolCheckStatus(EOL_CHECK_ID_CIRCULATE_PUMP, adc);
        }
    }
    else
    {
        adc = GetAdcData(ADC_ID_PUMP_COLD_IAD);
        SetEolCheckStatus(EOL_CHECK_ID_CIRCULATE_PUMP_OFF, adc);
    }
}

static void CheckSensorIceFull(void)
{
    U16 tmp;
    U16 adc;
    static U16 time = 50;  // 5sec

    if( time != 0 )
    {
        time--;
        TURN_OFF_ICE_INFRARED_HIGH();
    }
    else
    {
        TURN_ON_ICE_INFRARED_HIGH();
    }

    if( GET_STATUS_ICE_INFRARED_HIGH() == HIGH )
    {
        tmp = (U16)GetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_HIGH);
        adc = GetAdcData(ADC_ID_SENSOR_INFRARED_HIGH);
        if( tmp < adc )
        {
            SetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_HIGH, (U32)adc);
        }
    }
    else
    {
        adc = GetAdcData(ADC_ID_SENSOR_INFRARED_HIGH);
        SetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_HIGH_OFF, (U32)adc);
    }
}

static void CheckSensorIceLow(void)
{
    U16 tmp;
    U16 adc;
    static U16 time = 50;  // 5sec

    if( time != 0 )
    {
        time--;
        TURN_OFF_ICE_INFRARED_LOW();
    }
    else
    {        
        TURN_ON_ICE_INFRARED_LOW();
    }

    if( GET_STATUS_ICE_INFRARED_LOW() == HIGH )
    {
        tmp = (U16)GetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_LOW);
        adc = GetAdcData(ADC_ID_SENSOR_INFRARED_LOW);
        if( tmp < adc )
        {
            SetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_LOW, (U32)adc);
        }
    }
    else
    {
        adc = GetAdcData(ADC_ID_SENSOR_INFRARED_LOW);
        SetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_LOW_OFF, (U32)adc);
    }
}

static void CheckSensorBodyUvLed(void)
{
    U16 mu16Tmp;
    U16 mu16Adc;
    static U16 mu16Time = 30;  // 3sec

    // time limit, protect eyes of workers...
    if( mu16Time != 0 )
    {
        mu16Time--;

        API_TurnOnUv(UV_ID_ICE_DOOR);
        API_TurnOnUv(UV_ID_WATER_OUT);
        API_TurnOnUv(UV_ID_ICE_TANK);
        API_TurnOnUv(UV_ID_ICE_TRAY);
        API_TurnOnUv(UV_ID_DRAIN_TANK);
    }
    else
    {
        API_TurnOffUv(UV_ID_ICE_DOOR);
        API_TurnOffUv(UV_ID_WATER_OUT);
        API_TurnOffUv(UV_ID_ICE_TANK);
        API_TurnOffUv(UV_ID_ICE_TRAY);
        API_TurnOffUv(UV_ID_DRAIN_TANK);
    }

    // ICE DOOR
    if( GET_STATUS_UV_ICE_DOOR() == HIGH )
    {
        mu16Tmp = (U16)GetEolCheckStatus(EOL_CHECK_ID_UV_ICE_DOOR);
        mu16Adc = GetAdcData(ADC_ID_UV_ICE_DOOR_IAD);
        if( mu16Tmp < mu16Adc )
        {
            SetEolCheckStatus( EOL_CHECK_ID_UV_ICE_DOOR, mu16Adc );
        }
    }
    else
    {
        mu16Adc = GetAdcData(ADC_ID_UV_ICE_DOOR_IAD);
        SetEolCheckStatus( EOL_CHECK_ID_UV_ICE_DOOR_OFF, mu16Adc );
    }
    
    // WATER OUT
    if( GET_STATUS_UV_WATER_OUT() == HIGH )
    {
        mu16Tmp = (U16)GetEolCheckStatus( EOL_CHECK_ID_UV_WATER_OUT);
        mu16Adc = GetAdcData(ADC_ID_UV_WATER_OUT_IAD);
        if( mu16Tmp < mu16Adc )
        {
            SetEolCheckStatus( EOL_CHECK_ID_UV_WATER_OUT, mu16Adc );
        }
    }
    else
    {
        mu16Adc = GetAdcData(ADC_ID_UV_WATER_OUT_IAD);
        SetEolCheckStatus( EOL_CHECK_ID_UV_WATER_OUT_OFF, mu16Adc );
    }

    // ICE TANK
    if( GET_STATUS_UV_ICE_TANK() == HIGH )
    {
        mu16Tmp = (U16)GetEolCheckStatus( EOL_CHECK_ID_UV_ICE_TANK );
        mu16Adc = GetAdcData(ADC_ID_UV_ICE_TANK_IAD);
        if( mu16Tmp < mu16Adc )
        {
            SetEolCheckStatus( EOL_CHECK_ID_UV_ICE_TANK, mu16Adc );
        }
    }
    else
    {
        mu16Adc = GetAdcData(ADC_ID_UV_ICE_TANK_IAD);
        SetEolCheckStatus( EOL_CHECK_ID_UV_ICE_TANK_OFF, mu16Adc );
    }
    
    // ICE TRAY
    if( GET_STATUS_UV_ICE_TRAY() == HIGH )
    {
        mu16Tmp = (U16)GetEolCheckStatus( EOL_CHECK_ID_UV_ICE_TRAY );
        mu16Adc = GetAdcData(ADC_ID_UV_ICE_TRAY_IAD);
        if( mu16Tmp < mu16Adc )
        {
            SetEolCheckStatus( EOL_CHECK_ID_UV_ICE_TRAY, mu16Adc );
        }
    }
    else
    {
        mu16Adc = GetAdcData(ADC_ID_UV_ICE_TRAY_IAD);
        SetEolCheckStatus( EOL_CHECK_ID_UV_ICE_TRAY_OFF, mu16Adc );
    }

    // DRAIN TANK
    if( GET_STATUS_UV_DRAIN_TANK() == HIGH )
    {
        mu16Tmp = (U16)GetEolCheckStatus( EOL_CHECK_ID_UV_DRAIN_TANK );
        mu16Adc = GetAdcData(ADC_ID_UV_DRAIN_TANK_IAD);
        if( mu16Tmp < mu16Adc )
        {
            SetEolCheckStatus( EOL_CHECK_ID_UV_DRAIN_TANK, mu16Adc );
        }
    }
    else
    {
        mu16Adc = GetAdcData(ADC_ID_UV_DRAIN_TANK_IAD);
        SetEolCheckStatus( EOL_CHECK_ID_UV_DRAIN_TANK_OFF, mu16Adc );
    }
}

static I8 CheckSenserTankOpen(void)
{    
    static I8 mu8result = -1;
    static U8 mu8TankOpen = TRUE;       /// 미감지
    static U8 mu8PreTankOpen = TRUE;    /// 미감지

    // Check Tank Cover Switch
    mu8TankOpen = GET_STATUS_REED_TANK_OPEN();

    if( mu8PreTankOpen != mu8TankOpen )
    {
        if( mu8PreTankOpen == FALSE )   // 이전에 감지
        {
            if( mu8TankOpen == TRUE )   // 현재 미감지 
            {
                mu8result = TRUE;           // 미감지 -> 감지 -> 미감지
            }
        }
        else
        {
            mu8result = FALSE;
        }

        mu8PreTankOpen = mu8TankOpen;
    }

    return mu8result;
}

static I8 CheckSenserTrayBracketOpen(void)
{    
    static I8 mu8result = -1;
    static U8 mu8TrayBracketOpen = TRUE;

    // Check Tray Bracket Switch
    mu8TrayBracketOpen = GetCurrentInputVal(INPUT_ID_TRAY_BRACKET);

    /// OPEN : HIGH, CLOSE : LOW
    if( mu8TrayBracketOpen == FALSE )
    {
        mu8result = TRUE;            /// 감지
    }
    else
    {
        mu8result = FALSE;           /// 미감지
    }

    return mu8result;
}

static I8 CheckSenserWaterLever(void)
{    
    static I8 mu8result = -1;
    static U8 mu8WaterLever = FALSE;       /// 미감지
    static U8 mu8PreWaterLever = FALSE;    /// 미감지

    // Check Tank Cover Switch
    mu8WaterLever = GetLeverWater();

    if( mu8PreWaterLever != mu8WaterLever )
    {
        if( mu8PreWaterLever == TRUE )    /// 이전에 감지
        {
            if( mu8WaterLever == FALSE )  /// 현재 미감지 
            {
                mu8result = TRUE;       /// 미감지 -> 감지 -> 미감지
            }
        }
        else
        {
            mu8result = FALSE;
        }

        mu8PreWaterLever = mu8WaterLever;
    }

    return mu8result;
}

static I8 CheckSenserIceLever(void)
{    
    static I8 mu8result = -1;
    static U8 mu8IceLever = FALSE;       /// 미감지
    static U8 mu8PreIceLever = FALSE;    /// 미감지

    // Check Tank Cover Switch
    mu8IceLever = GetLeverIce();

    if( mu8PreIceLever != mu8IceLever )
    {
        if( mu8PreIceLever == TRUE )    /// 이전에 감지
        {
            if( mu8IceLever == FALSE )  /// 현재 미감지 
            {
                mu8result = TRUE;       /// 미감지 -> 감지 -> 미감지
            }
        }
        else
        {
            mu8result = FALSE;
        }

        mu8PreIceLever = mu8IceLever;
    }

    return mu8result;
}

static I8 CheckSenserSubIceLever(void)
{    
    static I8 mu8result = -1;
    static U8 mu8IceLever = FALSE;       /// 미감지
    static U8 mu8PreIceLever = FALSE;    /// 미감지

    // Check Tank Cover Switch
    mu8IceLever = GetLeverIce();

    if( mu8PreIceLever != mu8IceLever )
    {
        if( mu8PreIceLever == TRUE )    /// 이전에 감지
        {
            if( mu8IceLever == FALSE )  /// 현재 미감지 
            {
                mu8result = TRUE;       /// 미감지 -> 감지 -> 미감지
            }
        }
        else
        {
            mu8result = FALSE;
        }

        mu8PreIceLever = mu8IceLever;
    }

    return mu8result;
}

static void CheckSensor(void)
{
    // Check eol input
    // Tray Bracket Switch
    if( CheckSenserTrayBracketOpen() == TRUE )
    {
        SetEolTestInputBit(MK_EOL_TANK_BRACKET);
    }
    else
    {
        ClearEolTestInputBit(MK_EOL_TANK_BRACKET);
    }

    // Tank Cover Switch
    if( CheckSenserTankOpen() == TRUE )
    {
        SetEolTestInputBit(MK_EOL_TANK_COVER);
    }
    else
    {
        ClearEolTestInputBit(MK_EOL_TANK_COVER);
    }
    
    CheckEeprom();

    // ICE TRAY
    TestIceTray();

    CheckSensorDrainPump();
    CheckSensorDrainIcePump();
    CheckSensorCircualtePump();
    CheckSensorIceFull();
    CheckSensorIceLow();
    CheckSensorBodyUvLed();
    CheckSensorFan();
    CheckSensorFlowMeter();
}


// 얼음 피더 스크류 동작.
// 아이스도 도어 OPEN/CLOSE 반복 동작.
static void CheckVisualTest(void)
{
    U8 mu8Status;

    // Ice Door
    mu8Status = GetIceDoorStatus();
    if( mu8Status == ICE_DOOR_OPEN )
    {
        CloseIceDoor();
    }
    else if( mu8Status  == ICE_DOOR_CLOSE )
    {
        OpenIceDoor();
    }

    // Feeder Screw 
    TurnOnRelay( RELAY_FEEDER_CW );
    TurnOffRelay( RELAY_FEEDER_CCW );

}

static void TestDoneIceDoor(void)
{
    static U8 mu8Step = 0;
    static U8 init = FALSE;

    if( init == TRUE )
    {
        return;
    }

    switch( mu8Step )
    {
        case 0:
            OpenIceDoor();
            mu8Step++;
            break;

        case 1:
            if( GetIceDoorStatus() == ICE_DOOR_OPEN )
            {
                mu8Step++;
            }
            break;

        case 2:
            CloseIceDoor();
            mu8Step++;
            break;

        case 3:
            if( GetIceDoorStatus() == ICE_DOOR_CLOSE )
            {
                mu8Step++;
            }
            break;

        case 4:
            init = TRUE;
            mu8Step = 0;
            break;

        default:
            mu8Step = 0;
            break;
    }
}

static void OnOffDrain(void)
{
#if 1
    // 배수 동작
    if( GetEolDrainWater() == TRUE )
    {
        TurnOnPump(PUMP_ID_DRAIN, 100);
        OpenValve( VALVE_DRAIN );
    }
    else
    {
        TurnOffPump(PUMP_ID_DRAIN);
        CloseValve( VALVE_DRAIN );
    }
#else
    // 배수 동작
    if( GetEolDrainWater() == TRUE )
    {
        OpenValve( VALVE_DRAIN );
        HAL_TurnOnPump( HAL_PUMP_DRAIN, 24.0f );
    }
    else
    {
        CloseValve( VALVE_DRAIN );
        HAL_TurnOffPump( HAL_PUMP_DRAIN );
    }
#endif
}


#if 1
#define EOL_DRAIN_TIME   2400U // 4min = 4x60x10 = 2400 @100ms
static void DrainWater(void)
{
#if 1

    OnOffDrain();

#else
    static U16 mu16DrainTimer = EOL_DRAIN_TIME; 

    OnOffDrain();

    // 냉수 저수위 센서 미감지 후, 추가 동작 후 종료
    if( GetEolDrainWater() == TRUE )
    {
        if( IsDetectTankLevel( LEVEL_ID_COLD, LEVEL_DETECT_LOW ) == TRUE )
        {
            mu16DrainTimer = EOL_DRAIN_TIME; 
        }

        if( mu16DrainTimer == 0 )
        {
            Reset();
        }
        else
        {
            mu16DrainTimer--;
        }
    }
#endif
}
#endif

static void InnerProcessFront(void)
{
    // EEPROM
    CheckEeprom();

    // Visual Test ( ICE DOOR, FEEDER SCREW )
#if 0
    CheckVisualTest();
#endif

#if( CONFIG_JIG_RBK_FRONT == 1 )
    // DONE - OUTPUT PORT 
    if( ((GetEolTestInputVal() & MK_EOL_RBK_FRONT_LAST) == MK_EOL_RBK_FRONT_LAST)
         && (GetEolTestInputCnt(EOL_ID_KEY_ICE) == 5)
         && (GetEolTestInputCnt(EOL_ID_KEY_AMOUNT) == 5)
      )
    {
        // TURN ON OUTPUT +24V
        //CloseValve( VALVE_NOS );
        OpenValve( VALVE_FEED );
    }
    else
    {
        // TURN OFF OUPTUT +24V
        //OpenValve( VALVE_NOS );
        CloseValve( VALVE_FEED );
    }
#endif

    return ;
}

static void InnerProcessMainFront(void)
{
    // EEPROM
    CheckEeprom();

    // Visual Test ( ICE DOOR, FEEDER SCREW )
#if 0
    CheckVisualTest();
#endif

    /// Water Lever
    if( CheckSenserWaterLever() == TRUE )
    {
        SetEolTestInputBit(MK_EOL_LEVER_WATER);
    }
    else
    {
        ClearEolTestInputBit(MK_EOL_LEVER_WATER);
    }

    /// Ice Lever
    if( CheckSenserSubIceLever() == TRUE )
    {
        SetEolTestInputBit(MK_EOL_LEVER_ICE);
    }
    else
    {
        ClearEolTestInputBit(MK_EOL_LEVER_ICE);
    }
}

static void InnerProcessSubFront(void)
{
    // EEPROM
    CheckEeprom();

    // Visual Test ( ICE DOOR, FEEDER SCREW )
#if 0
    CheckVisualTest();
#endif
    
    /// Water Lever
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP1) == MK_EOL_SUB_FRONT_STEP1 )
    {
        if( CheckSenserWaterLever() == TRUE )
        {
            SetEolTestInputBit(MK_EOL_LEVER_WATER);
        }
        else
        {
            ClearEolTestInputBit(MK_EOL_LEVER_WATER);
        }
    }

    /// Ice Lever
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP12) == MK_EOL_SUB_FRONT_STEP12 )
    {
        if( CheckSenserSubIceLever() == TRUE )
        {
            SetEolTestInputBit(MK_EOL_LEVER_ICE);
        }
        else
        {
            ClearEolTestInputBit(MK_EOL_LEVER_ICE);
        }
    }

#if( CONFIG_JIG_WOORIM_FRONT == 1 )
    // DONE - OUTPUT PORT 
    if( (GetEolTestInputVal() & MK_EOL_RBK_FRONT_LAST) == MK_EOL_RBK_FRONT_LAST )
    {
        // TURN ON OUTPUT +24V
        //CloseValve( VALVE_NOS );
        OpenValve( VALVE_FEED );
    }
    else
    {
        // TURN OFF OUPTUT +24V
        //OpenValve( VALVE_NOS );
        CloseValve( VALVE_FEED );
    }
#endif
}

static void ModeReady(void)
{
    // Visual Test
    CheckVisualTest();

    // Check EOL Input 
    CheckSensor();

    OnOffDrain();

    // STOP ON/OFF HEATER TIMER ISR
    //R_TAU0_Channel2_Stop(); 

    // Filter cover 상태에 무관하게 Open
    OpenValve( VALVE_NOS );
    //CloseValve( VALVE_ICE_TRAY_IN );

    StopCheckIceFull();
    StopCheckIceLow();
}

static void ModeInit(void)
{
    /// Logic Off
    TurnOffCirculatePumpTrayIn(); 
    StopIceWaterOut();

    /// RELAY OFF
    TurnOffRelay(RELAY_COMP);
    TurnOffRelay(RELAY_FEEDER_CW);
    TurnOffRelay(RELAY_FEEDER_CCW);

    // 배수 펌프 OFF
    TurnOffPump(PUMP_ID_DRAIN);
    // 제빙수 냉수 배수 펌프 OFF
    TurnOffPump(PUMP_ID_ICE_DRAIN);   
    // 순환 펌프 OFF
    TurnOffPump(PUMP_ID_CIRCULATE);

    /// FAN OFF
    TURN_OFF_MOTOR_FAN();

    CloseValve( VALVE_ALL );
    OpenValve( NORMAL_OPEN_VALVES );
}

// AC HEATER
#define EOL_HEATER_DELAY_TIME   70      // 7sec @100ms
U8 RunCheckHeater   = FALSE;            // 순간 온수 히터 검사
// 순간 온수 히터 검사
// return TRUE : 검사 완료, FALSE : 검사 진행 중
static void CheckHeaterFeedback(void)
{
    static U8 mu8Step = 0;
    static U16 mu16Delay = EOL_HEATER_DELAY_TIME;
    //U16 mu16Val = 0;
    //U16 mu16NewVal = 0;

    if( RunCheckHeater == FALSE )
    {
        TURN_OFF_HEATER();
        mu8Step = 0;
        mu16Delay = EOL_HEATER_DELAY_TIME;

        return;
    }

    switch( mu8Step )
    {
        case 0:
            // CHECK HEATER
            TURN_ON_HEATER();

            if( mu16Delay != 0 )
            {
                mu16Delay--;
            }
            else
            {
                mu16Delay = EOL_HEATER_DELAY_TIME;
                mu8Step++;
            }

            break;

        case 1:
            TURN_OFF_HEATER();

            RunCheckHeater = FALSE;
            mu8Step = 0;
            mu16Delay = EOL_HEATER_DELAY_TIME;
            break;

        default:
            break;
    }
}

/// FlowMeter
static void CheckSensorFlowMeter(void)
{
    U32 uVal = 0U;

    // Flow Meter
    uVal = GetFlowMeterHz();
    if( GetEolCheckStatus( EOL_CHECK_ID_FLOW_METER ) < uVal )
    {
        SetEolCheckStatus( EOL_CHECK_ID_FLOW_METER, uVal );
    }

    SetEolCheckStatus( EOL_CHECK_ID_FLOW_METER_CUR, uVal );
}

// ICE TRAY MOTOR
U8 RunCheckIceTray  = FALSE;        // 아이스 트레이 검사
static void CheckLoadIceTray(void)
{
    static U8 mu8Go = 0;
    U8 mu8Icing;
    U8 mu8DeIcing;

    mu8Icing = GET_STATUS_MICRO_ICE();
    mu8DeIcing = GET_STATUS_MICRO_DEICE();

    if( RunCheckIceTray == FALSE )
    {
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
        TurnOffRelay( RELAY_ICE_TRAY_CW );
        return ;
    }

    if( mu8Icing == LOW && mu8DeIcing == LOW )
    {
        // 모두 감지, 정지
        mu8Go = 0;
    }
    else if( mu8Icing == HIGH && mu8DeIcing == LOW && mu8Go != 1  ) 
    {
        // 탈빙 감지 상태
        // 제빙 미감지, 제빙 이동
        mu8Go = 1;

        SetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_DEICING, 1);
    }
    else if( mu8Icing == LOW && mu8DeIcing == HIGH && mu8Go != 2)
    {
        // 제빙 감지 상태
        // 탈빙 미감지, 탈빙 이동
        mu8Go = 2;

        SetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_ICING, 1);
    }
    else if( mu8Icing == HIGH && mu8DeIcing == HIGH )
    {
        // 모두 미감지,  탈빙 위치 이동
        if( mu8Go == 0 )
        {
            mu8Go = 2;  
        }
    }

    if( mu8Go == 0 )
    {
        // 정지 
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
        TurnOffRelay( RELAY_ICE_TRAY_CW );
    }
    else if( mu8Go == 1)
    {
        // 제빙 이동
        TurnOffRelay( RELAY_ICE_TRAY_CW );
        TurnOnRelay( RELAY_ICE_TRAY_CCW );
    }
    else
    {
        // 탈빙 이동
        TurnOnRelay( RELAY_ICE_TRAY_CW );
        TurnOffRelay( RELAY_ICE_TRAY_CCW );
    }
}

#define EOL_COMP_RPS                (65U)
#define EOL_COMP_OFF_DELAY_TIME     (3000U)   // 5min @100ms
U8 EOL_PrevComp = OFF;
U8 EOL_Comp = OFF;
U8 EOL_GasSwitch = GAS_SWITCH_COLD;
U8 EOL_HotGasSwitch = HOTGAS_SWITCH_NORMAL;
U16 EOL_CompOffDelay = 0U;
U8 EOL_Switch = FALSE;
static void TEST_Sealed(void)
{
    /// 밸브 제어
    OpenValve( VALVE_NOS );         
    OpenValve( VALVE_FEED );    

    CloseValve( VALVE_ICE_TRAY_IN );      
    CloseValve( VALVE_HOT_OUT );     
    CloseValve( VALVE_ROOM_OUT );  

    /// COMP
    if( EOL_Comp == ON )
    {
        if( EOL_CompOffDelay == 0 )
        {
            SetCompBldcTargetRps(EOL_COMP_RPS);
        }
    }
    else
    {
        SetCompBldcTargetRps(0U);
    }

    if( EOL_PrevComp != EOL_Comp )
    {
        EOL_PrevComp = EOL_Comp;
        if( EOL_Comp )
        {
            EOL_CompOffDelay = EOL_COMP_OFF_DELAY_TIME;
        }
    }

    if( EOL_Switch == TRUE )
    {
        EOL_Switch = FALSE;
          
        /// GAS SWITCH
        if( EOL_GasSwitch == GAS_SWITCH_COLD )
        {
            GasSwitchCold();    
        }
        else if( EOL_GasSwitch == GAS_SWITCH_ICE )
        {
            GasSwitchIce();
        }

        /// HOTGAS SWITCH
        if( EOL_HotGasSwitch == HOTGAS_SWITCH_NORMAL )
        {
            HotGasSwitchNormal();    
        }
        else if( EOL_HotGasSwitch == HOTGAS_SWITCH_HOT )
        {
            HotGasSwitchHot();
        }
    }

    /// DC FAN ON
    TURN_ON_MOTOR_FAN();
    
    /// TEST 시간 무한...
    SetEolTestTimer( DEFAULT_TEST_TIME_VALUE );
}

static void InnerProcessLoad(void)
{
    U8 mu8Mode;

    mu8Mode = GetEolMode();
    switch( mu8Mode )
    {
        ///////////////// TEST SEALED ///////////////////////
        case EOL_MODE_SEALED_INIT:
            StopEolDrainWater();
            ModeInit();

            GasSwitchInit();
            HotGasSwitchInit();
            SetEolMode( EOL_MODE_SEALED );
            break;

        case EOL_MODE_SEALED:
            TEST_Sealed();
            DrainWater();

            /// 아이스 트레이 
            TestDoneIceTray();
            /// 아이스 도어
            TestDoneIceDoor();
            break;
        ///////////////// TEST SEALED ///////////////////////
    
        case EOL_MODE_READY:
            ModeReady();

            // 전체 KEY가 동작 및 
            // EW 검사선 감지 및 
            // 탱크리드 스위치 감지 및 
            // 조도센서 감지되면, 2모드(자동화 검사)로 변경
            if( (GetEolTestInputVal() & MK_EOL_LOAD_FRONT_LAST) == MK_EOL_LOAD_FRONT_LAST
                 && ((GetEolTestInputVal() & MK_EOL_KEY_ICE_TRAY_FLOW ) != 0UL )
                 && ((GetEolTestInputVal() & MK_EOL_KEY_ICE_WATER_FLOW ) != 0UL )
                 )
            {
                SetEolTestMode(EOL_TEST_MODE_2);
                SetEolMode(EOL_MODE_INIT);
            }
            break;

        case EOL_MODE_INIT:
            // 배수 기능이 켜져 있다면 강제로 OFF
            StopEolDrainWater();

            // 통신 제어 대기 
            ModeInit();

            TestIceTray();
            CheckSensorIceFull();
            CheckSensorIceLow();
            CheckSensorBodyUvLed();
            CheckHeaterFeedback();
            CheckSensorFlowMeter();
            CheckSensorFan();
            break;

        case EOL_MODE_CHECK_SENSOR:
            TestIceTray();
            CheckSensorIceFull();
            CheckSensorIceLow();
            CheckSensorBodyUvLed();
            CheckHeaterFeedback();
            CheckSensorFlowMeter();
            CheckSensorFan();
            break;

        case EOL_MODE_CHECK_LOAD:
            CheckLoadIceTray();
            TEST_RepeatGasValve();
            TEST_RepeatHotGasValve();
            TEST_RepeatIceDoor();
            CheckHeaterFeedback();
            CheckSensorFlowMeter();
            CheckSensorFan();
            break;

        case EOL_MODE_DONE:
            TURN_ON_MOTOR_FAN();
            DrainWater();
            /// 아이스 트레이 
            TestDoneIceTray();
            /// 아이스 도어
            TestDoneIceDoor();
            CheckSensorFlowMeter();
            break;

        default:
            break;
    }
}

static void UpdateSealdTimer(void)
{
    if( EOL_CompOffDelay != 0 )
    {
        EOL_CompOffDelay--;
    }
}

void ProcessEol(void)
{
    // check expired test time out
    if( GetEolTestTimer() == 0 )
    {
        Reset();
        return ;
    }

    if( GetEolType() == EOL_TYPE_FRONT )
    {
        InnerProcessFront();
    }
    else if( GetEolType() == EOL_TYPE_MAIN_FRONT )
    {
        InnerProcessMainFront();
    }
    else if( GetEolType() == EOL_TYPE_SUB_FRONT )
    {
        InnerProcessSubFront();
    }
    else
    {
        UpdateSealdTimer();
        InnerProcessLoad();
    }
}
