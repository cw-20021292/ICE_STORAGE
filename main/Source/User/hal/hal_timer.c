/**
 * File : hal_timer.c
 * 
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
*/
#pragma interrupt INTTM00 BaseTimer_Interrupt
//#pragma interrupt INTTM01 LedDimmingTimer_Interrupt
#pragma interrupt INTTM03 StepMotorTimerInterrupt

#include "hw.h"
#include "hal_timer.h"

#include "timer.h"
#include "led.h"

#include "api_step_motor.h"

#if ( CONFIG_TIMER00 == 1 )

/**
 * @brief     Base Interrupt service routin
 * @detail    TAU0 / Channel0 / Interval Timer / 1ms
 * @param     none
 * @return    none
**/
__interrupt static void BaseTimer_Interrupt(void)
{
    volatile FPUserISR userISR = GetTimerISR();
    volatile FPUserISR userISRFlow = GetTimerISRFlow();

    UpdateTimer();

    if( userISR != NULL )
    {
        userISR();
    }

    if( userISRFlow != NULL )
    {
        userISRFlow();
    }
}

#endif

#if ( CONFIG_TIMER01 == 1 )

/**
 * @brief     Led Dimming Interrupt service routin
 * @detail    TAU0 / Channel1 / Interval Timer / 1ms
 * @param     none
 * @return    none
**/
__interrupt static void LedDimmingTimer_Interrupt(void)
{
    EI();
    ControlLed();
}

#endif

#if ( CONFIG_TIMER03 == 1 )

/**
 * @brief     Base Interrupt service routin
 * @detail    TAU0 / Channel0 / Interval Timer / 1ms
 * @param     none
 * @return    none
**/
__interrupt static void StepMotorTimerInterrupt(void)
{
    if( GetInActive(SM_ID_0) == 0 )
    {
        ControlStepMotor( SM_ID_0 );   /// Gas Switch 
    }

    if( GetInActive(SM_ID_1) == 0 )
    {
        ControlStepMotor( SM_ID_1 );   /// Ice Door 
    }

    if( GetInActive(SM_ID_2) == 0 )
    {
        ControlStepMotor( SM_ID_2 );   /// Hot Gas 
    }
}

#endif


#if ( CONFIG_TIMER04_05 == 1 )

/* Not Used Interrupt */

#endif


#if ( CONFIG_TIMER11 == 1 )

/* Not Used Interrupt */

#endif

#if ( CONFIG_TIMER12_13 == 1 )

/* Not Used Interrupt */

#endif
