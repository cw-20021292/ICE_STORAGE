#include "hw.h"
#include "process_fct.h"
#include "fct.h"

#include "api_adc.h"
#include "api_step_motor.h"
#include "api_pump.h"
#include "led.h"

#include "key.h"
#include "eeprom.h"
#include "rtc.h"
#include "util.h"
#include "comm.h"
#include "timer.h"
#include "display.h"
#include "temp.h"

#include "valve.h"
#include "relay.h"
#include "service.h"
#include "ice_full.h"
#include "ice_low.h"

#include "comp_bldc.h"


static U8 IsExpiredTestTime(void)
{
    if( GetFctTestTimer() != 0 )
    {
        return FALSE;   // going on
    }

    return TRUE;  // timeout..
}


static void InitLoad(void)
{
    U8 mu8Ret;
    
    /// Off UART
    //R_UART0_Stop();   // FRONT
    //R_UART2_Stop();   // COMP
    R_UART3_Stop();   // EOL

    /// Init Step Motor
    InitStepVal(SM_ID_0);
    InitStepVal(SM_ID_1);
    InitStepVal(SM_ID_2);

    /// Turn off Valves
    CloseValve(VALVE_ALL);
    OpenValve(NORMAL_OPEN_VALVES);

    /// Turn off Relay
    TurnOffRelayAll();

    /// Turn off UV LED
#if 0
    TURN_OFF_UV_1();
#endif

    /// Trun on SMPS 12V
    TURN_ON_SMPS_12V();
    OPEN_VALVE_POWER_OUT();

    /// 스텝 모터 제어 비활성화
    SetInActive(SM_ID_0, 1);    /// GAS SWITCH
    SetInActive(SM_ID_1, 1);    /// ICE DOOR
    SetInActive(SM_ID_2, 1);    /// HOTGAS SWITCH

    /// TEST, EEPROM
    mu8Ret = TestEeprom();
    if( mu8Ret == TRUE )
    {
        SetFctTestInputBit( MK_FCT_EEPROM );
    }
    else
    {
        ClearFctTestInputBit( MK_FCT_EEPROM );
    }
}


static void ControlStopGasValve(void)
{
    P_STEP_MOTOR_GAS_1 = 0;
    P_STEP_MOTOR_GAS_2 = 0;
    P_STEP_MOTOR_GAS_3 = 0;
    P_STEP_MOTOR_GAS_4 = 0;
}

static void ControlMoveGasValve(void)
{
    static U8 mu8Count = 0;
    static U8 mu8Step = 0;

    if( mu8Count != 0 )
    {
        mu8Count--;
        return ;
    }

    mu8Count = 100;
    switch( mu8Step++ )
    {
        case 0:
            P_STEP_MOTOR_GAS_1  = 1;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 0;
            break;
        case 1:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 1;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 0;
            break;
        case 2:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 1;
            P_STEP_MOTOR_GAS_4  = 0;
            break;
        case 3:
            P_STEP_MOTOR_GAS_1  = 0;
            P_STEP_MOTOR_GAS_2  = 0;
            P_STEP_MOTOR_GAS_3  = 0;
            P_STEP_MOTOR_GAS_4  = 1;
            mu8Step = 0;
            break;
        default:
            mu8Step = 0;
            break;
    }
}

static void ControlStopHotGasValve(void)
{
    P_STEP_MOTOR_HOT_1  = 0;
    P_STEP_MOTOR_HOT_2  = 0;
    P_STEP_MOTOR_HOT_3  = 0;
    P_STEP_MOTOR_HOT_4  = 0;
}

static void ControlMoveHotGasValve(void)
{
    static U8 mu8Count = 0;
    static U8 mu8Step = 0;

    if( mu8Count != 0 )
    {
        mu8Count--;
        return ;
    }

    mu8Count = 100;
    switch( mu8Step++ )
    {
        case 0:
            P_STEP_MOTOR_HOT_1  = 1;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        case 1:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 1;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        case 2:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 1;
            P_STEP_MOTOR_HOT_4  = 0;
            break;
        case 3:
            P_STEP_MOTOR_HOT_1  = 0;
            P_STEP_MOTOR_HOT_2  = 0;
            P_STEP_MOTOR_HOT_3  = 0;
            P_STEP_MOTOR_HOT_4  = 1;
            mu8Step = 0;
            break;
        default:
            mu8Step = 0;
            break;
    }
}

/// 온도 범위 유효성 확인
static U8 IsValidRangeTemp(U8 id, TEMP_T tMin, TEMP_T tMax)
{
    if( GetTemp(id) >= tMin
            && GetTemp(id) <= tMax )
    {
        return TRUE;
    }

    return FALSE;
}

static void CheckCommComp(void)
{
    if( GetCompBldcCommTest() == FALSE )
    {
        ClearFctTestInputBit(MK_FCT_COMM_COMP);
    }
    else
    {
        SetFctTestInputBit(MK_FCT_COMM_COMP);
    }
}

static void CheckCommEol(void)
{
    static U8 mu8Step = 0;

    switch( mu8Step )
    {
        case 0:
            PM_EOL_RX = 1;
            PM_EOL_TX = 0;

            ClearFctTestInputBit(MK_FCT_COMM_EOL);
            mu8Step++;

            break;

        case 1:
            P_EOL_TX = 1;
            if( P_EOL_RX == 1 )
            {
                mu8Step++;
            }
            break;

        case 2:
            P_EOL_TX = 0;
            if( P_EOL_RX == 0 )
            {
                mu8Step++;
            }
            break;

        case 3:
            P_EOL_TX = 1;
            if( P_EOL_RX == 1 )
            {
                mu8Step++;
            }
            break;
    
        case 4:
            SetFctTestInputBit(MK_FCT_COMM_EOL);
            break;

        default:
            mu8Step = 0;
            break;
    }
}

void ProcessFctLoadTest(void)
{
    static U8 uvIceDoorFbCheck = FALSE;
    static U8 uvWaterOutFbCheck = FALSE;
    static U8 uvIceTankFbCheck = FALSE;
    static U8 uvIceTrayFbCheck = FALSE;
    static U8 uvDrainTankFbCheck = FALSE;
    
    /// TURN OFF ALL LEDS
    TurnOffAllLED();

    /// DISPLAY VERSION ON LCD 
    if( DispVersionOnTime() == FALSE )
    {
        return;
    }

    if( DispFrontVersionOnTime() == FALSE )
    {
        return;
    }

    /// 스텝 모터 제어 활성화
    SetInActive(SM_ID_0, 1);      /// GAS SWITCH
    SetInActive(SM_ID_1, 0);      /// ICE DOOR
    SetInActive(SM_ID_2, 1);      /// HOT GAS SWITCH

    // WIFI COMM 

    // EEPROM 
    if( IsSetFctTestInputVal(MK_FCT_EEPROM) == TRUE )
    {
        /// 1.5L ON

    }
    else
    {
        /// 1.5L OFF

    }

    /// BLDC COMM.
    CheckCommComp();
    if( IsSetFctTestInputVal(MK_FCT_COMM_COMP) == TRUE )
    {

    }
    else
    {

    }

    /// EOL COMM.
    CheckCommEol();
    if( IsSetFctTestInputVal(MK_FCT_COMM_EOL) == TRUE )
    {
        /// 1.0L ON

    }
    else
    {
        /// 1.0L OFF

    }

    // ICE FULL
    if( GetIceFullStatus() == TRUE )
    {

    }
    else
    {
    }

    // ICE LOW
    if( GetIceLowStatus() == TRUE )
    {
    }
    else
    {
    }
    
    /*********************************************************************************
     * KEY 
     */

    // KEY 
    // UV ICE TANK
    if( IsSetFctTestInputVal(MK_FCT_KEY_ICE) == TRUE )
    {
        OpenValve(VALVE_DRAIN);
    }
    else
    {
        CloseValve(VALVE_DRAIN);
    }

    if( IsSetFctTestInputVal(MK_FCT_KEY_HOT) == TRUE )
    {
        OpenValve(VALVE_HOT_OUT);
    }
    else
    {
        CloseValve(VALVE_HOT_OUT);
    }
    
    if( IsSetFctTestInputVal(MK_FCT_KEY_ROOM) == TRUE )
    {
        OpenValve(VALVE_ROOM_OUT);
    }
    else
    {
        CloseValve(VALVE_ROOM_OUT);
        OnOffLed(LED_ID_ROOM, OFF);
    }    

    if( IsSetFctTestInputVal( MK_FCT_KEY_COLD ) == TRUE )
    {
        OpenValve(VALVE_COLD_CIRCULATE);
        OnOffLed(LED_ID_COLD, ON);
    }
    else
    {
        CloseValve(VALVE_COLD_CIRCULATE);
        OnOffLed(LED_ID_COLD, OFF);
    }

    if( IsSetFctTestInputVal(MK_FCT_KEY_AMOUNT) == TRUE )
    {
        OpenValve(VALVE_FEED);
        OnOffLed(LED_ID_AMOUNT, ON);
    }
    else
    {
        CloseValve(VALVE_FEED);
    }

    if( IsSetFctTestInputVal(MK_FCT_KEY_ICE_WATER) == TRUE )
    {
        OpenValve(VALVE_ICE_WATER_OUT);
        
    }
    else
    {
        CloseValve(VALVE_ICE_WATER_OUT);
    }

    if( IsSetFctTestInputVal(MK_FCT_KEY_ICE_WATER_AMOUNT) == TRUE )
    {
        CloseValve(VALVE_NOS_FLOW);
    }
    else
    {
        OpenValve(VALVE_NOS_FLOW);
    }

    if( IsSetFctTestInputVal(MK_FCT_KEY_ICE_LOCK) == TRUE )
    {
        TURN_ON_UV_ICE_DOOR();
        uvIceDoorFbCheck = TRUE;
    }
    else
    {
        TURN_OFF_UV_ICE_DOOR();
        uvIceDoorFbCheck = FALSE;
    }

    if( IsSetFctTestInputVal(MK_FCT_KEY_ICE_WATER_LOCK) == TRUE )
    {
        TURN_ON_UV_WATER_OUT();
        uvWaterOutFbCheck = TRUE;
    }
    else
    {
        TURN_OFF_UV_WATER_OUT();
        uvWaterOutFbCheck = FALSE;
    }

    if( IsSetFctTestInputVal(MK_FCT_KEY_HOT_LOCK) == TRUE )
    {
        TURN_ON_UV_ICE_TANK();
        uvIceTankFbCheck = TRUE;
    }
    else
    {
        TURN_OFF_UV_ICE_TANK();
        uvIceTankFbCheck = FALSE;
    }

    // LEVER - WATER
    if( IsSetFctTestInputVal(MK_FCT_LEVER_WATER) == TRUE )
    {
        OpenValve(VALVE_COLD_IN); 
        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, ON);
    }
    else
    {
        CloseValve(VALVE_COLD_IN);
        OnOffLed(LED_ID_WELCOME_RIGHT_WHITE, OFF);
    }

    // LEVER - ICE
    if( IsSetFctTestInputVal(MK_FCT_LEVER_ICE) == TRUE )
    {
        OpenValve(VALVE_ICE_TRAY_IN);
        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, ON);
    }
    else
    {
        CloseValve(VALVE_ICE_TRAY_IN);
        OnOffLed(LED_ID_WELCOME_LEFT_WHITE, OFF);
    }    

    /*********************************************************
     * INPUT
     */
    // TANK REED(SERVICE) SW
    if( GET_STATUS_REED_TANK_OPEN() == HIGH )
    {
        TURN_ON_UV_ICE_TRAY();
        uvIceTrayFbCheck = TRUE;
    }
    else
    {
        TURN_OFF_UV_ICE_TRAY();
        uvIceTrayFbCheck = FALSE;
    }

    // TANK REED(SERVICE) SW
    if( GET_STATUS_REED_TRAY_BRACKET() == HIGH )
    {
        TURN_ON_UV_DRAIN_TANK();
        uvDrainTankFbCheck = TRUE;
    }
    else
    {
        TURN_OFF_UV_DRAIN_TANK();
        uvDrainTankFbCheck = FALSE;
    }

    // FLOW METER TRAY
    if( GET_STATUS_FLOW_METER_TRAY() == HIGH )
    {
        CloseValve(VALVE_NOS);
        OnOffLed(LED_ID_STATUS_UNIT_ML, ON);
    }
    else
    {
        OpenValve(VALVE_NOS);
        OnOffLed(LED_ID_STATUS_UNIT_ML, OFF);
    }

    {
      static U8 mu8OnOff = TRUE;

      // ICE TRAY MS-1  ( ICE DOOR )
      if( GET_STATUS_MICRO_ICE() == 0 )
      {
          mu8OnOff = TRUE;

          if( GetCurrentStep(SM_ID_1) == 1000 )
          {
              SetTargetStep(SM_ID_1, 0);
          }
          else if( GetCurrentStep(SM_ID_1) == 0 )
          {
              SetTargetStep(SM_ID_1, 1000);
          }
      }
      else
      {
          if( mu8OnOff == TRUE )
          {
              mu8OnOff = FALSE;
              StopMove(SM_ID_1);
              InitStepVal(SM_ID_1);
          }
      }
    }

    // ICE TRAY MS-2  ( GAS SWITCH )
    if( GET_STATUS_MICRO_DEICE() != 0 )
    {
        ControlMoveGasValve();
    }
    else
    {
        ControlStopGasValve();
    }

    // DRAIN HIGI LEVEL ( HOT GAS SWITCH  )
    if( GET_STATUS_LEVEL_DRAIN_TANK_HIGH() == HIGH )
    {
        ControlMoveHotGasValve();
    }
    else
    {
        ControlStopHotGasValve();
    }

    // DRAIN LOW LEVEL ( COMP SUPPLY )
    {
        static U8 oneShot = FALSE;
    
        if( GET_STATUS_LEVEL_DRAIN_TANK_LOW() == HIGH )
        {
            if( oneShot == FALSE )
            {
                oneShot = TRUE;
                StartTimer(TIMER_PROCESS, TIMER_ID_COMM_COMP_REQUEST, 0);
            }
            TurnOnRelay(RELAY_COMP);
        }
        else
        {
            oneShot = FALSE;

            StopTimer(TIMER_PROCESS, TIMER_ID_COMM_COMP_REQUEST);
            TurnOffRelay(RELAY_COMP);
        }
    }

    // ROOM HIGH, LOW 
    if( GET_STATUS_LEVEL_ROOM_TANK_HIGH() == HIGH )
    {
        TurnOnRelay(RELAY_ICE_TRAY_CW);
        TurnOffRelay(RELAY_ICE_TRAY_CCW);
    }
    else if( GET_STATUS_LEVEL_ROOM_TANK_LOW() == HIGH )
    {
        TurnOffRelay(RELAY_ICE_TRAY_CW);
        TurnOnRelay(RELAY_ICE_TRAY_CCW);
    }
    else
    {
        TurnOffRelay(RELAY_ICE_TRAY_CW);
        TurnOffRelay(RELAY_ICE_TRAY_CCW);
    }

    // ROOM OVF, COLD HIGH
    if( GET_STATUS_LEVEL_ROOM_TANK_OVERFLOW() == HIGH )
    {
        TurnOnRelay( RELAY_FEEDER_CW );
        TurnOffRelay( RELAY_FEEDER_CCW );
    }
    else if( GET_STATUS_LEVEL_COLD_TANK_HIGH() == HIGH )
    {
        TurnOffRelay( RELAY_FEEDER_CW );
        TurnOnRelay( RELAY_FEEDER_CCW );
    }
    else
    {
        TurnOffRelay( RELAY_FEEDER_CW );
        TurnOffRelay( RELAY_FEEDER_CCW );
    }

    // COLD TANK LOW ( HEATER )
    {
        // 히터 보호 동작 ( 최대 3초만 ON )
        static U16 mu16OnTime = 300U; // @10ms = 3sec, 
    
        if( GET_STATUS_LEVEL_COLD_TANK_LOW() == HIGH  )
        {
            if( mu16OnTime != 0 )
            {
                mu16OnTime--;
                TURN_ON_HEATER();
                OnOffLed(LED_ID_STATUS_HEATING, ON);
            }
            else
            {
                TURN_OFF_HEATER();
                OnOffLed(LED_ID_STATUS_HEATING, OFF);
            }
        
            
        }
        else
        {
            mu16OnTime = 300U;

            TURN_OFF_HEATER();
            OnOffLed(LED_ID_STATUS_HEATING, OFF);
        }
    }

    // COLD TEMPERTURE ( 150Kohm, 3도 )
    if( IsValidRangeTemp(TEMP_ID_COLD_WATER, 0.0f, 6.0f) == TRUE )
    {
        TURN_ON_MOTOR_FAN();
    }
    else
    {
        TURN_OFF_MOTOR_FAN();
    }

    // HOT TEMPERTURE( 17Kohm, 93도 )
    if( IsValidRangeTemp(TEMP_ID_HOT_WATER, 90.0f, 96.0f) == TRUE )
    {
        OpenValve(VALVE_EXTRA);
        OnOffLed(LED_ID_STATUS_MAX, ON);
    }
    else
    {
        CloseValve(VALVE_EXTRA);
        OnOffLed(LED_ID_STATUS_MAX, OFF);
    }

    // TEMP AMBIENT
    if( IsValidRangeTemp(TEMP_ID_AMBIENT, 22.0f, 28.0f) == TRUE )
    {
        TurnOnPump(PUMP_ID_DRAIN, 100);
    }
    else
    {
        TurnOffPump(PUMP_ID_DRAIN);
    }

    if( IsValidRangeTemp(TEMP_ID_AMBIENT_2, 22.0f, 28.0f) == TRUE )
    {
        TurnOnPump(PUMP_ID_ICE_DRAIN, 100);
    }
    else
    {
        TurnOffPump(PUMP_ID_ICE_DRAIN);
    }

    // TEMP ROOM 
    if( GetTemp(TEMP_ID_ROOM_WATER) >= 22.0f 
            && GetTemp(TEMP_ID_ROOM_WATER) <= 28.0f )
    {
#if( CONFIG_STER == 1 )
        HAL_TurnOnCSI();
        SetCSI_TDR(1600);
#else
        OnOffLed(LED_ID_STATUS_BAR_LOW, ON);
#endif
    }
    else
    {
#if( CONFIG_STER == 1 )
        HAL_TurnOffCSI();
        SetCSI_TDR(0);
#else
        OnOffLed(LED_ID_STATUS_BAR_LOW, OFF);
#endif
    }

    // TEMP EVA-1
    if( IsValidRangeTemp(TEMP_ID_EVA_1, 7.0f, 11.0f) == TRUE )
    {
        TurnOnPump(PUMP_ID_CIRCULATE, 100);
    }
    else
    {
        TurnOffPump(PUMP_ID_CIRCULATE);
    }

    // TEMP EVA-2 
    if( IsValidRangeTemp(TEMP_ID_EVA_2, 7.0f, 11.0f) == TRUE )
    {
        TURN_ON_MOTOR_SWING_BAR();
    }
    else
    {
        TURN_OFF_MOTOR_SWING_BAR();
    }

#if( CONFIG_STER == 1 )
    // CSI Module Feedback current  ( 1.6v ~ 2.44v )
    if( GetAdcData(ANI_CSI_CURRENT) >= 328 
            && GetAdcData(ANI_CSI_CURRENT) <= 500 )
    {
        HAL_TurnOnOffLED( TRIPLE,             ON );
        HAL_TurnOnOffLED( SAFE_CARE,          ON );
        HAL_TurnOnOffLED( HORIZONTAL_BAR1,    ON );
        HAL_TurnOnOffLED( HORIZONTAL_BAR2,    ON );
    }
    else
    {
        HAL_TurnOnOffLED( TRIPLE,             OFF );
        HAL_TurnOnOffLED( SAFE_CARE,          OFF );
        HAL_TurnOnOffLED( HORIZONTAL_BAR1,    OFF );
        HAL_TurnOnOffLED( HORIZONTAL_BAR2,    OFF );
    }
#endif

    // Feedback - Circulate Pump Feedback current ( 1.2v ~ 1.8v )
    if( GetAdcData(ADC_ID_PUMP_COLD_IAD) >= 250 
            && GetAdcData(ADC_ID_PUMP_COLD_IAD) <= 370 )
    {
        OnOffLed(LED_ID_STATUS_WATER_PREPARING, ON);
    }
    else
    {
        OnOffLed(LED_ID_STATUS_WATER_PREPARING, OFF);
    }

    // Feedback - DrainPump ( 1v, 0.5v ~ 1.5v )
    if( GetAdcData(ADC_ID_PUMP_DRAIN_IAD) >= 100 
            && GetAdcData(ADC_ID_PUMP_DRAIN_IAD) <= 300 )
    {
        OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, ON);
    }
    else
    {
        OnOffLed(LED_ID_STATUS_UNIT_CELSIUS, OFF);
    }

    // Feedback - IceDrainPump ( 1v, 0.5v ~ 1.5v )
    if( GetAdcData(ADC_ID_PUMP_ICE_DRAIN_IAD) >= 100 
            && GetAdcData(ADC_ID_PUMP_ICE_DRAIN_IAD) <= 300 )
    {
        OnOffLed(LED_ID_STATUS_BAR_INFINITE, ON);
    }
    else
    {
        OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
    }

    if( GetAdcData(ADC_ID_FAN_IAD) >= 50 )
    {
        OnOffLed(LED_ID_STATUS_COOLING, ON);
    }
    else
    {
        OnOffLed(LED_ID_STATUS_COOLING, OFF);
    }
    
    // Ice Out Feedback Current ( 0.048v ~ 0.29v  )
    if( uvIceDoorFbCheck == TRUE )
    {
        if( GetAdcData(ADC_ID_UV_ICE_DOOR_IAD) >= 10 
                && GetAdcData(ADC_ID_UV_ICE_DOOR_IAD) <= 60 )
        {
            OnOffLed(LED_ID_UV_ICE_FAUCET, ON);
        }
        else
        {
            OnOffLed(LED_ID_UV_ICE_FAUCET, OFF);
        }
    }
    
    // Water Out Feedback Current ( 0.048v ~ 0.29v )
    if( uvWaterOutFbCheck == TRUE )
    {
        if( GetAdcData(ADC_ID_UV_WATER_OUT_IAD) >= 10 
                && GetAdcData(ADC_ID_UV_WATER_OUT_IAD) <= 60 )
        {
            OnOffLed(LED_ID_UV_WATER_FAUCET, ON);
        }
        else
        {
            OnOffLed(LED_ID_UV_WATER_FAUCET, OFF);
        }
    }

    // Ice Tank Feedback Current ( 0.048v ~ 0.29v )
    if( uvIceTankFbCheck == TRUE )
    {
        if( GetAdcData(ADC_ID_UV_ICE_TANK_IAD) >= 10 
                && GetAdcData(ADC_ID_UV_ICE_TANK_IAD) <= 60 )
        {
            OnOffLed(LED_ID_UV_ICE_TANK, ON);
        }
        else
        {
            OnOffLed(LED_ID_UV_ICE_TANK, OFF);
        }
    }

    // Ice Tray Feedback Current ( 0.048v ~ 0.29v )
    if( uvIceTrayFbCheck == TRUE )
    {
        if( GetAdcData(ADC_ID_UV_ICE_TRAY_IAD) >= 10 
                && GetAdcData(ADC_ID_UV_ICE_TRAY_IAD) <= 60 )
        {
            OnOffLed(LED_ID_UV_ICE_TRAY, ON);
        }
        else
        {
            OnOffLed(LED_ID_UV_ICE_TRAY, OFF);
        }
    }

    // Drain Tank Feedback Current ( 0.048v ~ 0.29v )
    if( uvDrainTankFbCheck == TRUE )
    {
        if( GetAdcData(ADC_ID_UV_DRAIN_TANK_IAD) >= 10 
                && GetAdcData(ADC_ID_UV_DRAIN_TANK_IAD) <= 60 )
        {
            OnOffLed(LED_ID_UV_CARE_SYSTEM, ON);
        }
        else
        {
            OnOffLed(LED_ID_UV_CARE_SYSTEM, OFF);
        }
    }
}

void ProcessFct(void)
{
    static U8 mu8Init = 0;

    // check expired test time out
    if( IsExpiredTestTime() == TRUE )
    {
        Reset();
        return ;
    }

    if( mu8Init == 0 )
    {
        mu8Init = 1;
        InitLoad();
    }

    // Check FCT Input 
    StartCheckIceFull();
    StartCheckIceLow();
}

