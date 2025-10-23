/**
 * File : hw.h
 *
 * Hardware Pin Define
**/
#ifndef __HW_H__
#define __HW_H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_adc.h"
#include "r_cg_serial.h"
#include "r_cg_intc.h"
#include "r_cg_macrodriver.h"

/**
 * Load : Watchdog Timer Reset
**/
#define	RESET_WDT()						R_WDT_Restart()

/**
 * Load : Model
**/
/* MODEL TYPE - 0 : HP, 1 : CHP */
#define P_MODEL_TYPE                            P12.0
#define GET_STATUS_MODEL_TYPE()                 ( P_MODEL_TYPE )

/* FAUCET TYPE - 0 : FND, 1 : LCD */


/* EOL COMM LINE */
#define PM_EOL_RX                               PM14.3
#define P_EOL_RX                                P14.3

#define PM_EOL_TX                               PM14.4
#define P_EOL_TX                                P14.4

/**
 * Load : Lcd
**/


/**
 * Load : Comp
*/
/* Relay */
#define P_COMP                                 P4.7
#define TURN_ON_COMP()                         do{ P_COMP = 1; }while(0)
#define TURN_OFF_COMP()                        do{ P_COMP = 0; }while(0)
#define GET_STATUS_COMP()                      ( P_COMP )

/**
 * Load : Heater
*/
/* Relay */
#define P_HEATER                                P6.4
#define TURN_ON_HEATER()                        do{ P_HEATER = 1; }while(0)
#define TURN_OFF_HEATER()                       do{ P_HEATER = 0; }while(0)
#define GET_STATUS_HEATER()                     ( P_HEATER )

/**
 * Load : Pump
*/
/// OPTION PUMP
#define P_PUMP_OPTION                           P0.1
#define TURN_ON_PUMP_OPTION()                   do{ P_PUMP_OPTION = 1; }while(0)
#define TURN_OFF_PUMP_OPTION()                  do{ P_PUMP_OPTION = 0; }while(0)
#define GET_STATUS_PUMP_OPTION()                ( P_PUMP_OPTION )

#define P_PUMP_OPTION_IN                        P12.1
#define GET_STATUS_PUMP_OPTION_IN()             ( P_PUMP_OPTION_IN )


/// CIRCULATE PUMP
/// 기본 : 14.6V, TRIG ON
/// 가변1 : 20.6V, TRIG OFF
/// 가변2 : 23.8V, DI3 SHORT ( HW )
#define P_PUMP_CIRCULATE_TRIG                   P0.4
#define TURN_ON_PUMP_CIRCULATE_TRIG()           do{ P_PUMP_CIRCULATE_TRIG = 1; }while(0)
#define TURN_OFF_PUMP_CIRCULATE_TRIG()          do{ P_PUMP_CIRCULATE_TRIG = 0; }while(0)
#define GET_STATUS_PUMP_CIRCULATE_TRIG()        ( P_PUMP_CIRCULATE_TRIG )

#define P_PUMP_CIRCULATE                        P1.7
#define TURN_ON_PUMP_CIRCULATE()                do{ P_PUMP_CIRCULATE = 1; }while(0)
#define TURN_OFF_PUMP_CIRCULATE()               do{ P_PUMP_CIRCULATE = 0; }while(0)
#define GET_STATUS_PUMP_CIRCULATE()             ( P_PUMP_CIRCULATE )

/**
 * Load : Motor
*/
/// Swing Bar
#define P_MOTOR_SWING_BAR                        P5.6
#define TURN_ON_MOTOR_SWING_BAR()                do{ P_MOTOR_SWING_BAR = 1; }while(0)
#define TURN_OFF_MOTOR_SWING_BAR()               do{ P_MOTOR_SWING_BAR = 0; }while(0)
#define GET_STATUS_MOTOR_SWING_BAR()             ( P_MOTOR_SWING_BAR )

/// Fan
#define P_MOTOR_FAN                              P5.5
#define TURN_ON_MOTOR_FAN()                      do{ P_MOTOR_FAN = 1; }while(0)
#define TURN_OFF_MOTOR_FAN()                     do{ P_MOTOR_FAN = 0; }while(0)
#define GET_STATUS_MOTOR_FAN()                   ( P_MOTOR_FAN )

/// Ice Fan
#define P_MOTOR_FAN                              P5.5
#define TURN_ON_MOTOR_ICE_FAN()                  do{ P_MOTOR_FAN = 1; }while(0)
#define TURN_OFF_MOTOR_ICE_FAN()                 do{ P_MOTOR_FAN = 0; }while(0)
#define GET_STATUS_MOTOR_ICE_FAN()               ( P_MOTOR_FAN )

/// Ice Feeder
#define P_MOTOR_FEEDER_CCW                       P4.1
#define TURN_ON_MOTOR_FEEDER_CCW()               do{ P_MOTOR_FEEDER_CCW = 1; }while(0)
#define TURN_OFF_MOTOR_FEEDER_CCW()              do{ P_MOTOR_FEEDER_CCW = 0; }while(0)
#define GET_STATUS_MOTOR_FEEDER_CCW()            ( P_MOTOR_FEEDER_CCW )

#define P_MOTOR_FEEDER_CW                        P4.2
#define TURN_ON_MOTOR_FEEDER_CW()                do{ P_MOTOR_FEEDER_CW = 1; }while(0)
#define TURN_OFF_MOTOR_FEEDER_CW()               do{ P_MOTOR_FEEDER_CW = 0; }while(0)
#define GET_STATUS_MOTOR_FEEDER_CW()             ( P_MOTOR_FEEDER_CW )

/// Ice Tray
#define P_MOTOR_ICE_TRAY_CCW                     P4.3
#define TURN_ON_MOTOR_ICE_TRAY_CCW()             do{ P_MOTOR_ICE_TRAY_CCW = 1; }while(0)
#define TURN_OFF_MOTOR_ICE_TRAY_CCW()            do{ P_MOTOR_ICE_TRAY_CCW = 0; }while(0)
#define GET_STATUS_MOTOR_ICE_TRAY_CCW()          ( P_MOTOR_ICE_TRAY_CCW )

#define P_MOTOR_ICE_TRAY_CW                      P4.4
#define TURN_ON_MOTOR_ICE_TRAY_CW()              do{ P_MOTOR_ICE_TRAY_CW = 1; }while(0)
#define TURN_OFF_MOTOR_ICE_TRAY_CW()             do{ P_MOTOR_ICE_TRAY_CW = 0; }while(0)
#define GET_STATUS_MOTOR_ICE_TRAY_CW()           ( P_MOTOR_ICE_TRAY_CW )

/**
 * Load : Step Motor
*/
/// Refrigerant Path Conversion
#define P_STEP_MOTOR_GAS_1                       P8.0
#define TURN_ON_STEP_MOTOR_GAS_1()               do{ P_STEP_MOTOR_GAS_1 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_GAS_1()              do{ P_STEP_MOTOR_GAS_1 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_GAS_1()            ( P_STEP_MOTOR_GAS_1 )

#define P_STEP_MOTOR_GAS_2                       P8.1
#define TURN_ON_STEP_MOTOR_GAS_2()               do{ P_STEP_MOTOR_GAS_2 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_GAS_2()              do{ P_STEP_MOTOR_GAS_2 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_GAS_2()            ( P_STEP_MOTOR_GAS_2 )

#define P_STEP_MOTOR_GAS_3                       P0.6
#define TURN_ON_STEP_MOTOR_GAS_3()               do{ P_STEP_MOTOR_GAS_3 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_GAS_3()              do{ P_STEP_MOTOR_GAS_3 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_GAS_3()            ( P_STEP_MOTOR_GAS_3 )

#define P_STEP_MOTOR_GAS_4                       P0.5
#define TURN_ON_STEP_MOTOR_GAS_4()               do{ P_STEP_MOTOR_GAS_4 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_GAS_4()              do{ P_STEP_MOTOR_GAS_4 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_GAS_4()            ( P_STEP_MOTOR_GAS_4 )

#define P_STEP_MOTOR_HOT_1                       P7.4
#define TURN_ON_STEP_MOTOR_HOT_1()               do{ P_STEP_MOTOR_HOT_1 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_HOT_1()              do{ P_STEP_MOTOR_HOT_1 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_HOT_1()            ( P_STEP_MOTOR_HOT_1 )

#define P_STEP_MOTOR_HOT_2                       P7.5
#define TURN_ON_STEP_MOTOR_HOT_2()               do{ P_STEP_MOTOR_HOT_2 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_HOT_2()              do{ P_STEP_MOTOR_HOT_2 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_HOT_2()            ( P_STEP_MOTOR_HOT_2 )

#define P_STEP_MOTOR_HOT_3                       P7.6
#define TURN_ON_STEP_MOTOR_HOT_3()               do{ P_STEP_MOTOR_HOT_3 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_HOT_3()              do{ P_STEP_MOTOR_HOT_3 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_HOT_3()            ( P_STEP_MOTOR_HOT_3 )

#define P_STEP_MOTOR_HOT_4                       P7.7
#define TURN_ON_STEP_MOTOR_HOT_4()               do{ P_STEP_MOTOR_HOT_4 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_HOT_4()              do{ P_STEP_MOTOR_HOT_4 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_HOT_4()            ( P_STEP_MOTOR_HOT_4 )

/// ICE DOOR 
#define P_STEP_MOTOR_ICE_DOOR_1                  P7.3
#define TURN_ON_STEP_MOTOR_ICE_DOOR_1()          do{ P_STEP_MOTOR_ICE_DOOR_1 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_ICE_DOOR_1()         do{ P_STEP_MOTOR_ICE_DOOR_1 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_ICE_DOOR_1()       ( P_STEP_MOTOR_ICE_DOOR_1 )

#define P_STEP_MOTOR_ICE_DOOR_2                  P7.2
#define TURN_ON_STEP_MOTOR_ICE_DOOR_2()          do{ P_STEP_MOTOR_ICE_DOOR_2 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_ICE_DOOR_2()         do{ P_STEP_MOTOR_ICE_DOOR_2 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_ICE_DOOR_2()       ( P_STEP_MOTOR_ICE_DOOR_2 )

#define P_STEP_MOTOR_ICE_DOOR_3                  P7.1
#define TURN_ON_STEP_MOTOR_ICE_DOOR_3()          do{ P_STEP_MOTOR_ICE_DOOR_3 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_ICE_DOOR_3()         do{ P_STEP_MOTOR_ICE_DOOR_3 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_ICE_DOOR_3()       ( P_STEP_MOTOR_ICE_DOOR_3 )

#define P_STEP_MOTOR_ICE_DOOR_4                  P7.0
#define TURN_ON_STEP_MOTOR_ICE_DOOR_4()          do{ P_STEP_MOTOR_ICE_DOOR_4 = 1; }while(0)
#define TURN_OFF_STEP_MOTOR_ICE_DOOR_4()         do{ P_STEP_MOTOR_ICE_DOOR_4 = 0; }while(0)
#define GET_STATUS_STEP_MOTOR_ICE_DOOR_4()       ( P_STEP_MOTOR_ICE_DOOR_4 )

/**
 * Load : Smps
*/
#define P_SMPS_12V                               P4.5
#define TURN_ON_SMPS_12V()                       do{ P_SMPS_12V = 0; }while(0)
#define TURN_OFF_SMPS_12V()                      do{ P_SMPS_12V = 1; }while(0)
#define GET_STATUS_SMPS_12V()                    ( P_SMPS_12V )

/**
 * Load : Level
*/
/// ROOM TANK
#define P_LEVEL_ROOM_TANK_OVERFLOW               P14.6
#define GET_STATUS_LEVEL_ROOM_TANK_OVERFLOW()    ( P_LEVEL_ROOM_TANK_OVERFLOW )

#define P_LEVEL_ROOM_TANK_LOW                    P11.1
#define GET_STATUS_LEVEL_ROOM_TANK_LOW()         ( P_LEVEL_ROOM_TANK_LOW )

#define P_LEVEL_ROOM_TANK_HIGH                   P11.0
#define GET_STATUS_LEVEL_ROOM_TANK_HIGH()        ( P_LEVEL_ROOM_TANK_HIGH )

/// COLD TANK
#define P_LEVEL_COLD_TANK_HIGH                   P12.2
#define GET_STATUS_LEVEL_COLD_TANK_HIGH()        ( P_LEVEL_COLD_TANK_HIGH )

#define P_LEVEL_COLD_TANK_LOW                    P13.7
#define GET_STATUS_LEVEL_COLD_TANK_LOW()         ( P_LEVEL_COLD_TANK_LOW )

/// DRAIN TANK
#define P_LEVEL_DRAIN_TANK_LOW                   P1.5
#define GET_STATUS_LEVEL_DRAIN_TANK_LOW()        ( P_LEVEL_DRAIN_TANK_LOW )

#define P_LEVEL_DRAIN_TANK_HIGH                  P1.6
#define GET_STATUS_LEVEL_DRAIN_TANK_HIGH()       ( P_LEVEL_DRAIN_TANK_HIGH )

/**
 * Load : Valve
*/
/// OUT VALVE POWER
#define P_VALVE_POWER_OUT                         P5.7
#define OPEN_VALVE_POWER_OUT()                    do{ P_VALVE_POWER_OUT = 1; }while(0)
#define CLOSE_VALVE_POWER_OUT()                   do{ P_VALVE_POWER_OUT = 0; }while(0)
#define GET_STATUS_VALVE_POWER_OUT()              ( P_VALVE_POWER_OUT )

/// MAIN VALVE
#define P_VALVE_ICE_TRAY_IN                       P5.4
#define OPEN_VALVE_ICE_TRAY_IN()                  do{ P_VALVE_ICE_TRAY_IN = 1; }while(0)
#define CLOSE_VALVE_ICE_TRAY_IN()                 do{ P_VALVE_ICE_TRAY_IN = 0; }while(0)
#define GET_STATUS_VALVE_ICE_TRAY_IN()            ( P_VALVE_ICE_TRAY_IN )

#define P_VALVE_COLD_IN                           P5.3
#define OPEN_VALVE_COLD_IN()                      do{ P_VALVE_COLD_IN = 1; }while(0)
#define CLOSE_VALVE_COLD_IN()                     do{ P_VALVE_COLD_IN = 0; }while(0)
#define GET_STATUS_VALVE_COLD_IN()                ( P_VALVE_COLD_IN )

#define P_VALVE_ICE_WATER_OUT                     P5.1
#define OPEN_VALVE_ICE_WATER_OUT()                do{ P_VALVE_ICE_WATER_OUT = 1; }while(0)
#define CLOSE_VALVE_ICE_WATER_OUT()               do{ P_VALVE_ICE_WATER_OUT = 0; }while(0)
#define GET_STATUS_VALVE_ICE_WATER_OUT()          ( P_VALVE_ICE_WATER_OUT )

#define P_VALVE_COLD_CIRCULATE                    P5.0
#define OPEN_VALVE_COLD_CIRCULATE()               do{ P_VALVE_COLD_CIRCULATE = 1; }while(0)
#define CLOSE_VALVE_COLD_CIRCULATE()              do{ P_VALVE_COLD_CIRCULATE = 0; }while(0)
#define GET_STATUS_VALVE_COLD_CIRCULATE()         ( P_VALVE_COLD_CIRCULATE )

#define P_VALVE_HOT_OUT                           P3.0
#define OPEN_VALVE_HOT_OUT()                      do{ P_VALVE_HOT_OUT = 1; }while(0)
#define CLOSE_VALVE_HOT_OUT()                     do{ P_VALVE_HOT_OUT = 0; }while(0)
#define GET_STATUS_VALVE_HOT_OUT()                ( P_VALVE_HOT_OUT )

#define P_VALVE_ROOM_OUT                          P8.7
#define OPEN_VALVE_ROOM_OUT()                     do{ P_VALVE_ROOM_OUT = 1; }while(0)
#define CLOSE_VALVE_ROOM_OUT()                    do{ P_VALVE_ROOM_OUT = 0; }while(0)
#define GET_STATUS_VALVE_ROOM_OUT()               ( P_VALVE_ROOM_OUT )

#define P_VALVE_EXTRA                             P8.4
#define OPEN_VALVE_EXTRA()                        do{ P_VALVE_EXTRA = 1; }while(0)
#define CLOSE_VALVE_EXTRA()                       do{ P_VALVE_EXTRA = 0; }while(0)
#define GET_STATUS_VALVE_EXTRA()                  ( P_VALVE_EXTRA )

#define P_VALVE_FEED                              P8.3
#define OPEN_VALVE_FEED()                         do{ P_VALVE_FEED = 1; }while(0)
#define CLOSE_VALVE_FEED()                        do{ P_VALVE_FEED = 0; }while(0)
#define GET_STATUS_VALVE_FEED()                   ( P_VALVE_FEED )

#define P_VALVE_DRAIN                             P8.2
#define OPEN_VALVE_DRAIN()                        do{ P_VALVE_DRAIN = 1; }while(0)
#define CLOSE_VALVE_DRAIN()                       do{ P_VALVE_DRAIN = 0; }while(0)
#define GET_STATUS_VALVE_DRAIN()                  ( P_VALVE_DRAIN )

#define P_VALVE_NOS_FLOW                          P5.2
#define OPEN_VALVE_NOS_FLOW()                     do{ P_VALVE_NOS_FLOW = 0; }while(0)
#define CLOSE_VALVE_NOS_FLOW()                    do{ P_VALVE_NOS_FLOW = 1; }while(0)
#define GET_STATUS_VALVE_NOS_FLOW()               ( P_VALVE_NOS_FLOW )

#define P_VALVE_NOS                               P8.5
#define OPEN_VALVE_NOS()                          do{ P_VALVE_NOS = 0; }while(0)
#define CLOSE_VALVE_NOS()                         do{ P_VALVE_NOS = 1; }while(0)
#define GET_STATUS_VALVE_NOS()                    ( P_VALVE_NOS )

/**
 * Load : Buzzer
*/
#define P_BUZZER                                   P6.6
#define TURN_ON_BUZZER()                           do{ P_BUZZER = 1; }while(0)
#define TURN_OFF_BUZZER()                          do{ P_BUZZER = 0; }while(0)
#define GET_STATUS_BUZZER()                        ( P_BUZZER )

/**
 * Load : Voice
*/

/**
 * Load : Sensor
*/
/// TDS


/// FLOW METER
#define P_FLOW_METER_TRAY                       P3.1
#define GET_STATUS_FLOW_METER_TRAY()            ( P_FLOW_METER_TRAY )

/// REED SWITCH
#define P_REED_TANK_OPEN                        P10.1
#define GET_STATUS_REED_TANK_OPEN()             ( P_REED_TANK_OPEN )

#define P_REED_TRAY_BRACKET                     P1.0
#define GET_STATUS_REED_TRAY_BRACKET()          ( P_REED_TRAY_BRACKET )

// MICRO SWITCH
#define P_MICRO_ICE                             P12.4
#define GET_STATUS_MICRO_ICE()                  ( P_MICRO_ICE )

#define P_MICRO_DEICE                           P12.3
#define GET_STATUS_MICRO_DEICE()                ( P_MICRO_DEICE )

/// TEMP SENSOR
#define P_TEMP_COLD                             P6.1     // OPEN DRAIN PIN
#define TURN_ON_TEMP_COLD()                     do{ P_TEMP_COLD = 1; }while(0)
#define TURN_OFF_TEMP_COLD()                    do{ P_TEMP_COLD = 0; }while(0)
#define GET_STATUS_TEMP_COLD()                  ( P_TEMP_COLD )

#define P_TEMP_ROOM                             P6.0     // OPEN DRAIN PIN
#define TURN_ON_TEMP_ROOM()                     do{ P_TEMP_ROOM = 1; }while(0)
#define TURN_OFF_TEMP_ROOM()                    do{ P_TEMP_ROOM = 0; }while(0)
#define GET_STATUS_TEMP_ROOM()                  ( P_TEMP_ROOM )

/// INFRARED TX
#define P_ICE_INFRARED_LOW                      P10.2
#define TURN_ON_ICE_INFRARED_LOW()              do{ P_ICE_INFRARED_LOW = 1; }while(0)
#define TURN_OFF_ICE_INFRARED_LOW()             do{ P_ICE_INFRARED_LOW = 0; }while(0)
#define GET_STATUS_ICE_INFRARED_LOW()           ( P_ICE_INFRARED_LOW )

#define P_ICE_INFRARED_HIGH                     P13.0
#define TURN_ON_ICE_INFRARED_HIGH()             do{ P_ICE_INFRARED_HIGH = 1; }while(0)
#define TURN_OFF_ICE_INFRARED_HIGH()            do{ P_ICE_INFRARED_HIGH = 0; }while(0)
#define GET_STATUS_ICE_INFRARED_HIGH()          ( P_ICE_INFRARED_HIGH )

/**
 * Load : Ster (Uv, Ew)
*/
/// UV
#define P_UV_ICE_TANK                           P14.0
#define TURN_ON_UV_ICE_TANK()                   do{ P_UV_ICE_TANK = 1; }while(0)
#define TURN_OFF_UV_ICE_TANK()                  do{ P_UV_ICE_TANK = 0; }while(0)
#define GET_STATUS_UV_ICE_TANK()                ( P_UV_ICE_TANK )

#define P_UV_ICE_TRAY                           P14.1
#define TURN_ON_UV_ICE_TRAY()                   do{ P_UV_ICE_TRAY = 1; }while(0)
#define TURN_OFF_UV_ICE_TRAY()                  do{ P_UV_ICE_TRAY = 0; }while(0)
#define GET_STATUS_UV_ICE_TRAY()                ( P_UV_ICE_TRAY )

#define P_UV_WATER_OUT                          P14.2
#define TURN_ON_UV_WATER_OUT()                  do{ P_UV_WATER_OUT = 1; }while(0)
#define TURN_OFF_UV_WATER_OUT()                 do{ P_UV_WATER_OUT = 0; }while(0)
#define GET_STATUS_UV_WATER_OUT()               ( P_UV_WATER_OUT )

#define P_UV_ICE_DOOR                           P14.5
#define TURN_ON_UV_ICE_DOOR()                   do{ P_UV_ICE_DOOR = 1; }while(0)
#define TURN_OFF_UV_ICE_DOOR()                  do{ P_UV_ICE_DOOR = 0; }while(0)
#define GET_STATUS_UV_ICE_DOOR()                ( P_UV_ICE_DOOR )

#define P_UV_DRAIN_TANK                         P0.0
#define TURN_ON_UV_DRAIN_TANK()                 do{ P_UV_DRAIN_TANK = 1; }while(0)
#define TURN_OFF_UV_DRAIN_TANK()                do{ P_UV_DRAIN_TANK = 0; }while(0)
#define GET_STATUS_UV_DRAIN_TANK()              ( P_UV_DRAIN_TANK )

/// EW


/**
 * Load : Key
*/


/**
 * Load : Led
*/

/**
 * Load : Leak
*/

/**
 * Load : WiFi
**/

/**
 * Load : Unused Pin
**/
#define P_UNUSED_PIN_1                           P8.6

#endif    /* __HW_H__ */
