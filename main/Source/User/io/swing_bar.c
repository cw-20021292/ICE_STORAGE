/**
 * File : swing_bar.c
 * 
 * Swing Bar ( Static electricity ? )
*/
#include "hw.h"

#include "swing_bar.h"

#define TEST_SWING_BAR    (0)

#if( TEST_SWING_BAR == 1)
//// ¹ÚÁ¤Ã¶ ºÎÀå´Ô »ç¾ç 2021.08.23
//U8 the_swing_off = 180;
//U8 the_swing_on  = 20;  

// 2021.01.19
U8 the_swing_off = 180;
U8 the_swing_on  = 20;
#define  DEFAULT_SWING_BAR_TIME_OFF   the_swing_off
#define  DEFAULT_SWING_BAR_TIME_ON    the_swing_on
#else
//#define  DEFAULT_SWING_BAR_TIME_OFF   180   /* 1800ms @10ms */
//#define  DEFAULT_SWING_BAR_TIME_ON    20    /* 200ms @10ms */

#define  DEFAULT_SWING_BAR_TIME_OFF   (70)    /* 700ms @10ms */
#define  DEFAULT_SWING_BAR_TIME_ON    (30)    /* 300ms @10ms */
#endif

SSwingBar_T Swing;

void InitSwingBar(void)
{
    Swing.OnOff = OFF;
    Swing.OnTime = 0;
    Swing.OffTime = 0; 
}

void GetSwingBarData(SSwingBar_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Swing, sizeof(SSwingBar_T) );
}

void TurnOnSwingBar(void)
{
    // Set new time value when turn on swing bar...
    if( Swing.OnOff == OFF )
    {
        Swing.OnTime = DEFAULT_SWING_BAR_TIME_ON;
        Swing.OffTime = DEFAULT_SWING_BAR_TIME_OFF;
    }

    Swing.OnOff = ON;
}

void TurnOffSwingBar(void)
{
    Swing.OnOff = OFF;
}

void ControlSwingBar(void)
{
    // TURN OFF SWING BAR
    if( Swing.OnOff == OFF )
    { 
        TURN_OFF_MOTOR_SWING_BAR();

        return;
    }

    // TURN ON SWING BAR
    if( Swing.OnTime != 0 )
    {
        Swing.OnTime--;

        TURN_ON_MOTOR_SWING_BAR();
    }
    else if( Swing.OffTime != 0 )
    {
        Swing.OffTime--;
        
        TURN_OFF_MOTOR_SWING_BAR();
    }
    else
    {
       Swing.OnTime = DEFAULT_SWING_BAR_TIME_ON;
       Swing.OffTime = DEFAULT_SWING_BAR_TIME_OFF;
    }
}
