/**
 * File : main.c
 * 
 * Purifier Main Loop
**/
#include "mcu.h"

#include "hw.h"
#include "timer.h"
#include "util.h"
#include "eeprom.h"
#include "rtc.h"
#include "front.h"
#include "sound.h"
#include "parser.h"
#include "eol.h"

#include "process_display.h"
#include "process_sys_event.h"

/**
 * @ brief    main loop
 * @ detail   none  
 * @ param  none
 * @ return   none
**/
void main(void)
{
    // Initialize
    Delay_MS(1400U);    // actually 500ms...

    RESET_WDT();

    InitSystem();
    InitStartTimer();
    EI();

    /* RTC & EEPROM */
    //InitRtc();
#if( (CONFIG_JIG_RBK_FRONT == 0) && (CONFIG_JIG_WOORIM_FRONT == 0) )
    InitEeprom();
#endif
    CheckFactroySetup();

    Sound(SOUND_POWER_ON);

#if( CONFIG_JIG_RBK_FRONT ==  1)
    StartEol(EOL_TYPE_FRONT);
    SetVersionDisp(0);
#endif

#if( CONFIG_JIG_WOORIM_FRONT == 1)
    StartEol(EOL_TYPE_SUB_FRONT);
    SetVersionDisp(0);
#endif

    while(1)
    {
        RESET_WDT();

        // System Scheduler
        ProcessEventHandler();

        // Communication ( Uart )
        RecvPacketHandler();
        SendPacketHandler();
    }
}
