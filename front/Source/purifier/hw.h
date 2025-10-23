/**
 * File : hw.h
 * 
 * Hardware Pin Define
*/
#ifndef __HW_H__
#define __HW_H__

#include "mcu.h"
#include "r_cg_cgc.h"
#include "r_cg_port.h"
#include "r_cg_wdt.h"
#include "r_cg_timer.h"
#include "r_cg_serial.h"
#include "r_cg_macrodriver.h"

/**
 * Load : Watchdog Timer Reset
*/
#define	RESET_WDT()						                  R_WDT_Restart()

/**
 * Load : Model
*/

/**
 * Load : Lcd
*/

/**
 * Load : Comp
*/

/**
 * Load : Heater
*/

/**
 * Load : Ac Motor
*/

/**
 * Load : Step Motor
*/

/**
 * Load : Smps
*/

/**
 * Load : Level
*/

/**
 * Load : Valve
*/

/**
 * Load : Buzzer
*/

/**
 * Load : Sensor
*/

/**
 * Load : Ster (Uv, Ew)
*/


/**
 * Load : Key
*/
/* Complete */
#define P_KEY_ALL_LCOK                            P15.5
#define GET_STATUS_KEY_ALL_LOCK()                 (P_KEY_ALL_LCOK)
/* Complete */
#define P_KEY_COLD_OFF                            P15.3
#define GET_STATUS_KEY_COLD_OFF()                 (P_KEY_COLD_OFF)
/* Complete */
#define P_KEY_HOT_LOCK                            P2.4
#define GET_STATUS_KEY_HOT_LOCK()                 (P_KEY_HOT_LOCK)

#define P_KEY_ICE_WATER_LOCK                      P2.3
#define GET_STATUS_KEY_ICE_WATER_LOCK()           (P_KEY_ICE_WATER_LOCK)
/* Complete */
#define P_KEY_ICE_LOCK                            P4.1
#define GET_STATUS_KEY_ICE_LOCK()                 (P_KEY_ICE_LOCK)

#define P_KEY_HOT_OFF                             P2.5
#define GET_STATUS_KEY_HOT_OFF()                  (P_KEY_HOT_OFF)
/* Complete */
#define P_KEY_ICE_OFF                             P12.3
#define GET_STATUS_KEY_ICE_OFF()                  (P_KEY_ICE_OFF)

#define P_KEY_ICE_WATER_AMOUNT                    P2.7
#define GET_STATUS_KEY_ICE_WATER_AMOUNT()         (P_KEY_ICE_WATER_AMOUNT)
/* Complete */
#define P_KEY_HOT                                 P2.3
#define GET_STATUS_KEY_HOT()                      (P_KEY_HOT)
/* Complete */
#define P_KEY_ROOM                                P2.5
#define GET_STATUS_KEY_ROOM()                     (P_KEY_ROOM)
/* Complete */
#define P_KEY_COLD                                P2.7
#define GET_STATUS_KEY_COLD()                     (P_KEY_COLD)
/* Complete */
#define P_KEY_AMOUNT                              P15.1
#define GET_STATUS_KEY_AMOUNT()                   (P_KEY_AMOUNT)
/* Complete */
#define P_KEY_ICE                                 P12.2
#define GET_STATUS_KEY_ICE()                      (P_KEY_ICE)
/* Complete */
#define P_KEY_ICE_WATER                           P12.4
#define GET_STATUS_KEY_ICE_WATER()                (P_KEY_ICE_WATER)

/**
 * Load : Lever
*/
/* Complete */
#define P_LEVER_ICE                               P13.7
#define GET_STATUS_LEVER_ICE()                    (P_LEVER_ICE)
/* Complete */
#define P_LEVER_WATER                             P15.6
#define GET_STATUS_LEVER_WATER()                  (P_LEVER_WATER)

/**
 * Load : Led
*/

/* Complete */
#define P_LED_UV_CARE_SYSTEM                       P1.5
#define TURN_ON_LED_UV_CARE_SYSTEM()               do{ P_LED_UV_CARE_SYSTEM = 1; }while(0)
#define TURN_OFF_LED_UV_CARE_SYSTEM()              do{ P_LED_UV_CARE_SYSTEM = 0; }while(0)
#define GET_STATUS_LED_UV_CARE_SYSTEM()            (P_LED_UV_CARE_SYSTEM)

/* Complete */
#define P_LED_UV_ICE_TANK                          P8.5
#define TURN_ON_LED_UV_ICE_TANK()                  do{ P_LED_UV_ICE_TANK = 1; }while(0)
#define TURN_OFF_LED_UV_ICE_TANK()                 do{ P_LED_UV_ICE_TANK = 0; }while(0)
#define GET_STATUS_LED_UV_ICE_TANK()               (P_LED_UV_ICE_TANK)

/* Complete */
#define P_LED_UV_ICE_TRAY                          P8.6
#define TURN_ON_LED_UV_ICE_TRAY()                  do{ P_LED_UV_ICE_TRAY = 1; }while(0)
#define TURN_OFF_LED_UV_ICE_TRAY()                 do{ P_LED_UV_ICE_TRAY = 0; }while(0)
#define GET_STATUS_LED_UV_ICE_TRAY()               (P_LED_UV_ICE_TRAY)

/* Complete */
#define P_LED_UV_ICE_FAUCET                        P5.3
#define TURN_ON_LED_UV_ICE_FAUCET()                do{ P_LED_UV_ICE_FAUCET = 1; }while(0)
#define TURN_OFF_LED_UV_ICE_FAUCET()               do{ P_LED_UV_ICE_FAUCET = 0; }while(0)
#define GET_STATUS_LED_UV_ICE_FAUCET()             (P_LED_UV_ICE_FAUCET)

/* Complete */
#define P_LED_UV_WATER_FAUCET                      P5.4
#define TURN_ON_LED_UV_WATER_FAUCET()              do{ P_LED_UV_WATER_FAUCET = 1; }while(0)
#define TURN_OFF_LED_UV_WATER_FAUCET()             do{ P_LED_UV_WATER_FAUCET = 0; }while(0)
#define GET_STATUS_LED_UV_WATER_FAUCET()           (P_LED_UV_WATER_FAUCET)

#define P_LED_STATUS_ICE_WATER                     P11.5
#define TURN_ON_LED_STATUS_ICE_WATER()             do{ P_LED_STATUS_ICE_WATER = 1; }while(0)
#define TURN_OFF_LED_STATUS_ICE_WATER()            do{ P_LED_STATUS_ICE_WATER = 0; }while(0)
#define GET_STATUS_LED_STATUS_ICE_WATER()          (P_LED_STATUS_ICE_WATER)

#define P_LED_STATUS_MAX                           P11.4
#define TURN_ON_LED_STATUS_MAX()                   do{ P_LED_STATUS_MAX = 1; }while(0)
#define TURN_OFF_LED_STATUS_MAX()                  do{ P_LED_STATUS_MAX = 0; }while(0)
#define GET_STATUS_LED_STATUS_MAX()                (P_LED_STATUS_MAX)

#define P_LED_STATUS_WATER                         P11.3
#define TURN_ON_LED_STATUS_WATER()                 do{ P_LED_STATUS_WATER = 1; }while(0)
#define TURN_OFF_LED_STATUS_WATER()                do{ P_LED_STATUS_WATER = 0; }while(0)
#define GET_STATUS_LED_STATUS_WATER()              (P_LED_STATUS_WATER)

#define P_LED_SEG_1A                               P11.2
#define TURN_ON_LED_SEG_1A()                       do{ P_LED_SEG_1A = 1; }while(0)
#define TURN_OFF_LED_SEG_1A()                      do{ P_LED_SEG_1A = 0; }while(0)
#define GET_STATUS_LED_SEG_1A()                    (P_LED_SEG_1A)

#define P_LED_SEG_1B                               P9.7
#define TURN_ON_LED_SEG_1B()                       do{ P_LED_SEG_1B = 1; }while(0)
#define TURN_OFF_LED_SEG_1B()                      do{ P_LED_SEG_1B = 0; }while(0)
#define GET_STATUS_LED_SEG_1B()                    (P_LED_SEG_1B)

#define P_LED_STATUS_ICE_PREPARING                 P9.6
#define TURN_ON_LED_STATUS_ICE_PREPARING()         do{ P_LED_STATUS_ICE_PREPARING = 1; }while(0)
#define TURN_OFF_LED_STATUS_ICE_PREPARING()        do{ P_LED_STATUS_ICE_PREPARING = 0; }while(0)
#define GET_STATUS_LED_STATUS_ICE_PREPARING()      (P_LED_STATUS_ICE_PREPARING)

/* Complete */
#define P_LED_STATUS_ICE_FULL                      P1.6
#define TURN_ON_LED_STATUS_ICE_FULL()              do{ P_LED_STATUS_ICE_FULL = 1; }while(0)
#define TURN_OFF_LED_STATUS_ICE_FULL()             do{ P_LED_STATUS_ICE_FULL = 0; }while(0)
#define GET_STATUS_LED_STATUS_ICE_FULL()           (P_LED_STATUS_ICE_FULL)

#define P_LED_STATUS_WATER_PREPARING               P9.4
#define TURN_ON_LED_STATUS_WATER_PREPARING()       do{ P_LED_STATUS_WATER_PREPARING = 1; }while(0)
#define TURN_OFF_LED_STATUS_WATER_PREPARING()      do{ P_LED_STATUS_WATER_PREPARING = 0; }while(0)
#define GET_STATUS_LED_STATUS_WATER_PREPARING()    (P_LED_STATUS_WATER_PREPARING)

#define P_LED_STATUS_UNIT_CELSIUS                  P9.3
#define TURN_ON_LED_STATUS_UNIT_CELSIUS()          do{ P_LED_STATUS_UNIT_CELSIUS = 1; }while(0)
#define TURN_OFF_LED_STATUS_UNIT_CELSIUS()         do{ P_LED_STATUS_UNIT_CELSIUS = 0; }while(0)
#define GET_STATUS_LED_STATUS_UNIT_CELSIUS()       (P_LED_STATUS_UNIT_CELSIUS)

#define P_LED_SEG_2A                               P9.2
#define TURN_ON_LED_SEG_2A()                       do{ P_LED_SEG_2A = 1; }while(0)
#define TURN_OFF_LED_SEG_2A()                      do{ P_LED_SEG_2A = 0; }while(0)
#define GET_STATUS_LED_SEG_2A()                    (P_LED_SEG_2A)

#define P_LED_SEG_2B                               P9.1
#define TURN_ON_LED_SEG_2B()                       do{ P_LED_SEG_2B = 1; }while(0)
#define TURN_OFF_LED_SEG_2B()                      do{ P_LED_SEG_2B = 0; }while(0)
#define GET_STATUS_LED_SEG_2B()                    (P_LED_SEG_2B)

#define P_LED_SEG_2C                               P9.0
#define TURN_ON_LED_SEG_2C()                       do{ P_LED_SEG_2C = 1; }while(0)
#define TURN_OFF_LED_SEG_2C()                      do{ P_LED_SEG_2C = 0; }while(0)
#define GET_STATUS_LED_SEG_2C()                    (P_LED_SEG_2C)

#define P_LED_SEG_2D                               P1.0
#define TURN_ON_LED_SEG_2D()                       do{ P_LED_SEG_2D = 1; }while(0)
#define TURN_OFF_LED_SEG_2D()                      do{ P_LED_SEG_2D = 0; }while(0)
#define GET_STATUS_LED_SEG_2D()                    (P_LED_SEG_2D)

#define P_LED_SEG_2E                               P1.1
#define TURN_ON_LED_SEG_2E()                       do{ P_LED_SEG_2E = 1; }while(0)
#define TURN_OFF_LED_SEG_2E()                      do{ P_LED_SEG_2E = 0; }while(0)
#define GET_STATUS_LED_SEG_2E()                    (P_LED_SEG_2E)

#define P_LED_SEG_2F                               P1.2
#define TURN_ON_LED_SEG_2F()                       do{ P_LED_SEG_2F = 1; }while(0)
#define TURN_OFF_LED_SEG_2F()                      do{ P_LED_SEG_2F = 0; }while(0)
#define GET_STATUS_LED_SEG_2F()                    (P_LED_SEG_2F)

#define P_LED_SEG_2G                               P1.3
#define TURN_ON_LED_SEG_2G()                       do{ P_LED_SEG_2G = 1; }while(0)
#define TURN_OFF_LED_SEG_2G()                      do{ P_LED_SEG_2G = 0; }while(0)
#define GET_STATUS_LED_SEG_2G()                    (P_LED_SEG_2G)

#define P_LED_SEG_3A                               P1.4
#define TURN_ON_LED_SEG_3A()                       do{ P_LED_SEG_3A = 1; }while(0)
#define TURN_OFF_LED_SEG_3A()                      do{ P_LED_SEG_3A = 0; }while(0)
#define GET_STATUS_LED_SEG_3A()                    (P_LED_SEG_3A)

#define P_LED_SEG_3B                               P1.5
#define TURN_ON_LED_SEG_3B()                       do{ P_LED_SEG_3B = 1; }while(0)
#define TURN_OFF_LED_SEG_3B()                      do{ P_LED_SEG_3B = 0; }while(0)
#define GET_STATUS_LED_SEG_3B()                    (P_LED_SEG_3B)

#define P_LED_SEG_3C                               P1.6
#define TURN_ON_LED_SEG_3C()                       do{ P_LED_SEG_3C = 1; }while(0)
#define TURN_OFF_LED_SEG_3C()                      do{ P_LED_SEG_3C = 0; }while(0)
#define GET_STATUS_LED_SEG_3C()                    (P_LED_SEG_3C)

#define P_LED_SEG_3D                               P1.7
#define TURN_ON_LED_SEG_3D()                       do{ P_LED_SEG_3D = 1; }while(0)
#define TURN_OFF_LED_SEG_3D()                      do{ P_LED_SEG_3D = 0; }while(0)
#define GET_STATUS_LED_SEG_3D()                    (P_LED_SEG_3D)

#define P_LED_SEG_3E                               P5.7
#define TURN_ON_LED_SEG_3E()                       do{ P_LED_SEG_3E = 1; }while(0)
#define TURN_OFF_LED_SEG_3E()                      do{ P_LED_SEG_3E = 0; }while(0)
#define GET_STATUS_LED_SEG_3E()                    (P_LED_SEG_3E)

#define P_LED_SEG_3F                               P5.6
#define TURN_ON_LED_SEG_3F()                       do{ P_LED_SEG_3F = 1; }while(0)
#define TURN_OFF_LED_SEG_3F()                      do{ P_LED_SEG_3F = 0; }while(0)
#define GET_STATUS_LED_SEG_3F()                    (P_LED_SEG_3F)

#define P_LED_SEG_3G                               P5.5
#define TURN_ON_LED_SEG_3G()                       do{ P_LED_SEG_3G = 1; }while(0)
#define TURN_OFF_LED_SEG_3G()                      do{ P_LED_SEG_3G = 0; }while(0)
#define GET_STATUS_LED_SEG_3G()                    (P_LED_SEG_3G)

#define P_LED_SEG_4A                               P5.4
#define TURN_ON_LED_SEG_4A()                       do{ P_LED_SEG_4A = 1; }while(0)
#define TURN_OFF_LED_SEG_4A()                      do{ P_LED_SEG_4A = 0; }while(0)
#define GET_STATUS_LED_SEG_4A()                    (P_LED_SEG_4A)

#define P_LED_SEG_4B                               P5.3
#define TURN_ON_LED_SEG_4B()                       do{ P_LED_SEG_4B = 1; }while(0)
#define TURN_OFF_LED_SEG_4B()                      do{ P_LED_SEG_4B = 0; }while(0)
#define GET_STATUS_LED_SEG_4B()                    (P_LED_SEG_4B)

#define P_LED_SEG_4C                               P5.2
#define TURN_ON_LED_SEG_4C()                       do{ P_LED_SEG_4C = 1; }while(0)
#define TURN_OFF_LED_SEG_4C()                      do{ P_LED_SEG_4C = 0; }while(0)
#define GET_STATUS_LED_SEG_4C()                    (P_LED_SEG_4C)

#define P_LED_SEG_4D                               P5.1
#define TURN_ON_LED_SEG_4D()                       do{ P_LED_SEG_4D = 1; }while(0)
#define TURN_OFF_LED_SEG_4D()                      do{ P_LED_SEG_4D = 0; }while(0)
#define GET_STATUS_LED_SEG_4D()                    (P_LED_SEG_4D)

#define P_LED_SEG_4E                               P5.0
#define TURN_ON_LED_SEG_4E()                       do{ P_LED_SEG_4E = 1; }while(0)
#define TURN_OFF_LED_SEG_4E()                      do{ P_LED_SEG_4E = 0; }while(0)
#define GET_STATUS_LED_SEG_4E()                    (P_LED_SEG_4E)

#define P_LED_SEG_4F                               P3.0
#define TURN_ON_LED_SEG_4F()                       do{ P_LED_SEG_4F = 1; }while(0)
#define TURN_OFF_LED_SEG_4F()                      do{ P_LED_SEG_4F = 0; }while(0)
#define GET_STATUS_LED_SEG_4F()                    (P_LED_SEG_4F)

#define P_LED_SEG_4G                               P8.7
#define TURN_ON_LED_SEG_4G()                       do{ P_LED_SEG_4G = 1; }while(0)
#define TURN_OFF_LED_SEG_4G()                      do{ P_LED_SEG_4G = 0; }while(0)
#define GET_STATUS_LED_SEG_4G()                    (P_LED_SEG_4G)

#define P_LED_STATUS_BAR_LOW                       P8.6
#define TURN_ON_LED_STATUS_BAR_LOW()               do{ P_LED_STATUS_BAR_LOW = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_LOW()              do{ P_LED_STATUS_BAR_LOW = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_LOW()            (P_LED_STATUS_BAR_LOW)

#define P_LED_STATUS_BAR_MIDDLE                    P8.5
#define TURN_ON_LED_STATUS_BAR_MIDDLE()            do{ P_LED_STATUS_BAR_MIDDLE = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_MIDDLE()           do{ P_LED_STATUS_BAR_MIDDLE = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_MIDDLE()         (P_LED_STATUS_BAR_MIDDLE)

#define P_LED_STATUS_BAR_HIGH                      P8.4
#define TURN_ON_LED_STATUS_BAR_HIGH()              do{ P_LED_STATUS_BAR_HIGH = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_HIGH()             do{ P_LED_STATUS_BAR_HIGH = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_HIGH()           (P_LED_STATUS_BAR_HIGH)

#define P_LED_STATUS_BAR_INFINITE                  P8.3
#define TURN_ON_LED_STATUS_BAR_INFINITE()          do{ P_LED_STATUS_BAR_INFINITE = 1; }while(0)
#define TURN_OFF_LED_STATUS_BAR_INFINITE()         do{ P_LED_STATUS_BAR_INFINITE = 0; }while(0)
#define GET_STATUS_LED_STATUS_BAR_INFINITE()       (P_LED_STATUS_BAR_INFINITE)

#define P_LED_STATUS_UNIT_ML                       P8.2
#define TURN_ON_LED_STATUS_UNIT_ML()               do{ P_LED_STATUS_UNIT_ML = 1; }while(0)
#define TURN_OFF_LED_STATUS_UNIT_ML()              do{ P_LED_STATUS_UNIT_ML = 0; }while(0)
#define GET_STATUS_LED_STATUS_UNIT_ML()            (P_LED_STATUS_UNIT_ML)

/* Right Water Out Faucet (Complete) */
#define P_LED_WELCOME_RIGHT_BLUE                   P12.7
#define TURN_ON_LED_WELCOME_RIGHT_BLUE()           do{ P_LED_WELCOME_RIGHT_BLUE = 1; }while(0)
#define TURN_OFF_LED_WELCOME_RIGHT_BLUE()          do{ P_LED_WELCOME_RIGHT_BLUE = 0; }while(0)
#define GET_STATUS_LED_WELCOME_RIGHT_BLUE()        (P_LED_WELCOME_RIGHT_BLUE)

#define P_LED_WELCOME_RIGHT_GREEN                  P12.6
#define TURN_ON_LED_WELCOME_RIGHT_GREEN()          do{ P_LED_WELCOME_RIGHT_GREEN = 1; }while(0)
#define TURN_OFF_LED_WELCOME_RIGHT_GREEN()         do{ P_LED_WELCOME_RIGHT_GREEN = 0; }while(0)
#define GET_STATUS_LED_WELCOME_RIGHT_GREEN()       (P_LED_WELCOME_RIGHT_GREEN)

#define P_LED_WELCOME_RIGHT_RED                    P12.5
#define TURN_ON_LED_WELCOME_RIGHT_RED()            do{ P_LED_WELCOME_RIGHT_RED = 1; }while(0)
#define TURN_OFF_LED_WELCOME_RIGHT_RED()           do{ P_LED_WELCOME_RIGHT_RED = 0; }while(0)
#define GET_STATUS_LED_WELCOME_RIGHT_RED()         (P_LED_WELCOME_RIGHT_RED)

#define P_LED_WELCOME_RIGHT_WHITE                  P14.2
#define TURN_ON_LED_WELCOME_RIGHT_WHITE()          do{ P_LED_WELCOME_RIGHT_WHITE = 1; }while(0)
#define TURN_OFF_LED_WELCOME_RIGHT_WHITE()         do{ P_LED_WELCOME_RIGHT_WHITE = 0; }while(0)
#define GET_STATUS_LED_WELCOME_RIGHT_WHITE()       (P_LED_WELCOME_RIGHT_WHITE)

/* Left Ice Out Faucet (Outer) (Complete) */
#define P_LED_WELCOME_LEFT_BLUE                   P4.4
#define TURN_ON_LED_WELCOME_LEFT_BLUE()           do{ P_LED_WELCOME_LEFT_BLUE = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT_BLUE()          do{ P_LED_WELCOME_LEFT_BLUE = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT_BLUE()        (P_LED_WELCOME_LEFT_BLUE)

#define P_LED_WELCOME_LEFT_GREEN                  P4.3
#define TURN_ON_LED_WELCOME_LEFT_GREEN()          do{ P_LED_WELCOME_LEFT_GREEN = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT_GREEN()         do{ P_LED_WELCOME_LEFT_GREEN = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT_GREEN()       (P_LED_WELCOME_LEFT_GREEN)

#define P_LED_WELCOME_LEFT_RED                    P4.2
#define TURN_ON_LED_WELCOME_LEFT_RED()            do{ P_LED_WELCOME_LEFT_RED = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT_RED()           do{ P_LED_WELCOME_LEFT_RED = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT_RED()         (P_LED_WELCOME_LEFT_RED)

#define P_LED_WELCOME_LEFT_WHITE                  P7.1
#define TURN_ON_LED_WELCOME_LEFT_WHITE()          do{ P_LED_WELCOME_LEFT_WHITE = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT_WHITE()         do{ P_LED_WELCOME_LEFT_WHITE = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT_WHITE()       (P_LED_WELCOME_LEFT_WHITE)

/* Left Ice Out Faucet (Inner) (Complete) */
#define P_LED_WELCOME_LEFT_BLUE_I                   P4.5
#define TURN_ON_LED_WELCOME_LEFT_BLUE_I()           do{ P_LED_WELCOME_LEFT_BLUE_I = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT_BLUE_I()          do{ P_LED_WELCOME_LEFT_BLUE_I = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT_BLUE_I()        (P_LED_WELCOME_LEFT_BLUE_I)

#define P_LED_WELCOME_LEFT_GREEN_I                  P4.6
#define TURN_ON_LED_WELCOME_LEFT_GREEN_I()          do{ P_LED_WELCOME_LEFT_GREEN_I = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT_GREEN_I()         do{ P_LED_WELCOME_LEFT_GREEN_I = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT_GREEN_I()       (P_LED_WELCOME_LEFT_GREEN_I)

#define P_LED_WELCOME_LEFT_RED_I                    P4.7
#define TURN_ON_LED_WELCOME_LEFT_RED_I()            do{ P_LED_WELCOME_LEFT_RED_I = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT_RED_I()           do{ P_LED_WELCOME_LEFT_RED_I = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT_RED_I()         (P_LED_WELCOME_LEFT_RED_I)

#define P_LED_WELCOME_LEFT_WHITE_I                  P6.7
#define TURN_ON_LED_WELCOME_LEFT_WHITE_I()          do{ P_LED_WELCOME_LEFT_WHITE_I = 1; }while(0)
#define TURN_OFF_LED_WELCOME_LEFT_WHITE_I()         do{ P_LED_WELCOME_LEFT_WHITE_I = 0; }while(0)
#define GET_STATUS_LED_WELCOME_LEFT_WHITE_I()       (P_LED_WELCOME_LEFT_WHITE_I)

/* Complete */
#define P_LED_AMOUNT                              P3.6
#define TURN_ON_LED_AMOUNT()                      do{ P_LED_AMOUNT = 1; }while(0)
#define TURN_OFF_LED_AMOUNT()                     do{ P_LED_AMOUNT = 0; }while(0)
#define GET_STATUS_LED_AMOUNT()                   (P_LED_AMOUNT)

/* Complete */
#define P_LED_COLD                                P3.3
#define TURN_ON_LED_COLD()                        do{ P_LED_COLD = 1; }while(0)
#define TURN_OFF_LED_COLD()                       do{ P_LED_COLD = 0; }while(0)
#define GET_STATUS_LED_COLD()                     (P_LED_COLD)

/* Complete */
#define P_LED_ROOM                                P3.2
#define TURN_ON_LED_ROOM()                        do{ P_LED_ROOM = 1; }while(0)
#define TURN_OFF_LED_ROOM()                       do{ P_LED_ROOM = 0; }while(0)
#define GET_STATUS_LED_ROOM()                     (P_LED_ROOM)

/* Complete */
#define P_LED_HOT                                 P2.3
#define TURN_ON_LED_HOT()                         do{ P_LED_HOT = 1; }while(0)
#define TURN_OFF_LED_HOT()                        do{ P_LED_HOT = 0; }while(0)
#define GET_STATUS_LED_HOT()                      (P_LED_HOT)

#define P_LED_ICE_WATER_AMOUNT                    P4.5
#define TURN_ON_LED_ICE_WATER_AMOUNT()            do{ P_LED_ICE_WATER_AMOUNT = 1; }while(0)
#define TURN_OFF_LED_ICE_WATER_AMOUNT()           do{ P_LED_ICE_WATER_AMOUNT = 0; }while(0)
#define GET_STATUS_LED_ICE_WATER_AMOUNT()         (P_LED_ICE_WATER_AMOUNT)

/* Complete */
#define P_LED_ICE_WATER                           P7.4
#define TURN_ON_LED_ICE_WATER()                   do{ P_LED_ICE_WATER = 1; }while(0)
#define TURN_OFF_LED_ICE_WATER()                  do{ P_LED_ICE_WATER = 0; }while(0)
#define GET_STATUS_LED_ICE_WATER()                (P_LED_ICE_WATER)

/* Complete */
#define P_LED_ICE                                 P7.3
#define TURN_ON_LED_ICE()                         do{ P_LED_ICE = 1; }while(0)
#define TURN_OFF_LED_ICE()                        do{ P_LED_ICE = 0; }while(0)
#define GET_STATUS_LED_ICE()                      (P_LED_ICE)

#define P_LED_STATUS_COOLING                      P10.3
#define TURN_ON_LED_STATUS_COOLING()              do{ P_LED_STATUS_COOLING = 1; }while(0)
#define TURN_OFF_LED_STATUS_COOLING()             do{ P_LED_STATUS_COOLING = 0; }while(0)
#define GET_STATUS_LED_STATUS_COOLING()           (P_LED_STATUS_COOLING)

#define P_LED_STATUS_LOW_WATER                    P10.4
#define TURN_ON_LED_STATUS_LOW_WATER()            do{ P_LED_STATUS_LOW_WATER = 1; }while(0)
#define TURN_OFF_LED_STATUS_LOW_WATER()           do{ P_LED_STATUS_LOW_WATER = 0; }while(0)
#define GET_STATUS_LED_STATUS_LOW_WATER()         (P_LED_STATUS_LOW_WATER)

#define P_LED_STATUS_HEATING                      P10.5
#define TURN_ON_LED_STATUS_HEATING()              do{ P_LED_STATUS_HEATING = 1; }while(0)
#define TURN_OFF_LED_STATUS_HEATING()             do{ P_LED_STATUS_HEATING = 0; }while(0)
#define GET_STATUS_LED_STATUS_HEATING()           (P_LED_STATUS_HEATING)

#define P_LED_STATUS_HOT_LOCK                     P10.6
#define TURN_ON_LED_STATUS_HOT_LOCK()             do{ P_LED_STATUS_HOT_LOCK = 1; }while(0)
#define TURN_OFF_LED_STATUS_HOT_LOCK()            do{ P_LED_STATUS_HOT_LOCK = 0; }while(0)
#define GET_STATUS_LED_STATUS_HOT_LOCK()          (P_LED_STATUS_HOT_LOCK)

#define P_LED_STATUS_ICE_WATER_AMOUNT_LOCK               P3.2
#define TURN_ON_LED_STATUS_ICE_WATER_AMOUNT_LOCK()       do{ P_LED_STATUS_ICE_WATER_AMOUNT_LOCK = 1; }while(0)
#define TURN_OFF_LED_STATUS_ICE_WATER_AMOUNT_LOCK()      do{ P_LED_STATUS_ICE_WATER_AMOUNT_LOCK = 0; }while(0)
#define GET_STATUS_LED_STATUS_ICE_WATER_AMOUNT_LOCK()    (P_LED_STATUS_ICE_WATER_AMOUNT_LOCK)

#define P_LED_STATUS_ICE_WATER_LOCK               P3.3
#define TURN_ON_LED_STATUS_ICE_WATER_LOCK()       do{ P_LED_STATUS_ICE_WATER_LOCK = 1; }while(0)
#define TURN_OFF_LED_STATUS_ICE_WATER_LOCK()      do{ P_LED_STATUS_ICE_WATER_LOCK = 0; }while(0)
#define GET_STATUS_LED_STATUS_ICE_WATER_LOCK()    (P_LED_STATUS_ICE_WATER_LOCK)

#define P_LED_STATUS_ICE_LOCK                     P3.4
#define TURN_ON_LED_STATUS_ICE_LOCK()             do{ P_LED_STATUS_ICE_LOCK = 1; }while(0)
#define TURN_OFF_LED_STATUS_ICE_LOCK()            do{ P_LED_STATUS_ICE_LOCK = 0; }while(0)
#define GET_STATUS_LED_STATUS_ICE_LOCK()          (P_LED_STATUS_ICE_LOCK)

/* Complete */
#define P_LED_ALL_LOCK                            P14.1
#define TURN_ON_LED_ALL_LOCK()                    do{ P_LED_ALL_LOCK = 1; }while(0)
#define TURN_OFF_LED_ALL_LOCK()                   do{ P_LED_ALL_LOCK = 0; }while(0)
#define GET_STATUS_LED_ALL_LOCK()                 (P_LED_ALL_LOCK)

/* Complete */
#define P_LED_COLD_OFF                            P12.0
#define TURN_ON_LED_COLD_OFF()                    do{ P_LED_COLD_OFF = 1; }while(0)
#define TURN_OFF_LED_COLD_OFF()                   do{ P_LED_COLD_OFF = 0; }while(0)
#define GET_STATUS_LED_COLD_OFF()                 (P_LED_COLD_OFF)

#define P_LED_HOT_LOCK                            P3.7
#define TURN_ON_LED_HOT_LOCK()                    do{ P_LED_HOT_LOCK = 1; }while(0)
#define TURN_OFF_LED_HOT_LOCK()                   do{ P_LED_HOT_LOCK = 0; }while(0)
#define GET_STATUS_LED_HOT_LOCK()                 (P_LED_HOT_LOCK)

#define P_LED_ICE_WATER_LOCK                      P12.0
#define TURN_ON_LED_ICE_WATER_LOCK()              do{ P_LED_ICE_WATER_LOCK = 1; }while(0)
#define TURN_OFF_LED_ICE_WATER_LOCK()             do{ P_LED_ICE_WATER_LOCK = 0; }while(0)
#define GET_STATUS_LED_ICE_WATER_LOCK()           (P_LED_ICE_WATER_LOCK)

#define P_LED_ICE_LOCK                            P14.0
#define TURN_ON_LED_ICE_LOCK()                    do{ P_LED_ICE_LOCK = 1; }while(0)
#define TURN_OFF_LED_ICE_LOCK()                   do{ P_LED_ICE_LOCK = 0; }while(0)
#define GET_STATUS_LED_ICE_LOCK()                 (P_LED_ICE_LOCK)

#define P_LED_HOT_OFF                             P14.1
#define TURN_ON_LED_HOT_OFF()                     do{ P_LED_HOT_OFF = 1; }while(0)
#define TURN_OFF_LED_HOT_OFF()                    do{ P_LED_HOT_OFF = 0; }while(0)
#define GET_STATUS_LED_HOT_OFF()                  (P_LED_HOT_OFF)

/* Complete */
#define P_LED_ICE_OFF                             P12.3
#define TURN_ON_LED_ICE_OFF()                     do{ P_LED_ICE_OFF = 1; }while(0)
#define TURN_OFF_LED_ICE_OFF()                    do{ P_LED_ICE_OFF = 0; }while(0)
#define GET_STATUS_LED_ICE_OFF()                  (P_LED_ICE_OFF)

/**
 * Load : Uv
*/

/**
 * Load : Fan
*/

/**
 * Load : Leak
*/

/**
 * Load : WiFi
*/


/**
 * Load : Unused Pin
*/
#define P_UNUSED_OUT_1                       P14.3
#define P_UNUSED_OUT_2                       P14.4
#define P_UNUSED_OUT_3                       P14.5
#define P_UNUSED_OUT_4                       P0.0
#define P_UNUSED_OUT_5                       P0.1
#define P_UNUSED_OUT_6                       P0.4
#define P_UNUSED_OUT_7                       P0.7
#define P_UNUSED_OUT_8                       P10.2
#define P_UNUSED_OUT_9                       P13.0
#define P_UNUSED_OUT_10                      P15.6
#define P_UNUSED_OUT_11                      P10.0
#define P_UNUSED_OUT_12                      P14.7
#define P_UNUSED_OUT_13                      P14.6
#define P_UNUSED_OUT_14                      P8.1
#define P_UNUSED_OUT_15                      P8.0
#define P_UNUSED_OUT_16                      P7.6
#define P_UNUSED_OUT_17                      P7.7
#define P_UNUSED_OUT_18                      P6.7
#define P_UNUSED_OUT_19                      P6.6
#define P_UNUSED_OUT_20                      P6.5
#define P_UNUSED_OUT_21                      P6.4
#define P_UNUSED_OUT_22                      P3.1
#define P_UNUSED_OUT_23                      P6.3
#define P_UNUSED_OUT_24                      P6.2
#define P_UNUSED_OUT_25                      P12.1
#define P_UNUSED_OUT_26                      P12.2
#define P_UNUSED_OUT_27                      P13.7
#define P_UNUSED_OUT_28                      P12.3
#define P_UNUSED_OUT_29                      P12.4
#define P_UNUSED_OUT_30                      P12.5

#endif    /* __HW_H__ */
