/**
 * File : display_lib.h
 * 
 * Led Display For Blink
*/
#include "display_lib.h"
#include "display.h"
#include "segment.h"

typedef struct _disp_timer_
{
    U16 TimerBlink[DISP_TIMER_ID_BLINK_MAX];
    U16 TimerCount[DISP_TIMER_ID_BLINK_MAX];

    U16 Timer[DISP_TIMER_ID_MAX];
} SDispTimer_T;

SDispTimer_T DispTimer;

typedef struct _seg_disp_timer_
{
    U16 TimerBlink[DISP_TIMER_ID_BLINK_MAX];
    U16 TimerCount[DISP_TIMER_ID_BLINK_MAX];

    U16 Timer[DISP_TIMER_ID_MAX];
} SSegDispTimer_T;

SSegDispTimer_T SegDispTimer;

void InitDispTimer(void)
{
    DispTimer.TimerCount[DISP_TIMER_ID_500MS] = TIMER_500MS;
    DispTimer.TimerCount[DISP_TIMER_ID_1SEC] = TIMER_1SEC;
    DispTimer.TimerCount[DISP_TIMER_ID_3SEC] = TIMER_3SEC;
    DispTimer.TimerCount[DISP_TIMER_ID_4SEC] = TIMER_4SEC;
    DispTimer.TimerCount[DISP_TIMER_ID_100MS] = TIMER_100MS;   
}

U8 IsExpiredDispTimer(U8 id)
{
    if( DispTimer.Timer[id] == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

void SetDispTimer(U8 id, U16 time)
{
    DispTimer.Timer[id] = time;
}


U8 IsExpiredDispBlinkTimer(U8 id)
{
    if( DispTimer.TimerBlink[id] == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

static void ResetDisplayBlinkTimer(U8 id)
{
    DispTimer.TimerBlink[id] = DispTimer.TimerCount[id];
}

/* LED 점멸 제어 함수 
 * Param
 *  - mu8OnOff : LED on/off 명령
 *  - pFun : LED 제어 함수 포인터 주소
 *  - mu16Timer : 타이머 변수 
 */
U8 BlinkLED(U8 onOff, void (*pFun)(U8 onOff), U8 id)
{
    if( pFun == NULL )
    {
        return;  // 에러 처리..
    }

    if( IsExpiredDispBlinkTimer(id) == TRUE )
    {
        if( onOff == OFF )
        {
            onOff = ON;
        }
        else
        {
            onOff = OFF;
        }
    }

    if( pFun != NULL )
    {
        pFun( onOff );  
    }

    return onOff;
}

void InitSegDispTimer(void)
{
    SegDispTimer.TimerCount[DISP_TIMER_ID_500MS] = TIMER_500MS;
    SegDispTimer.TimerCount[DISP_TIMER_ID_1SEC] = TIMER_1SEC;
    SegDispTimer.TimerCount[DISP_TIMER_ID_3SEC] = TIMER_3SEC;
    SegDispTimer.TimerCount[DISP_TIMER_ID_4SEC] = TIMER_4SEC;
    SegDispTimer.TimerCount[DISP_TIMER_ID_100MS] = TIMER_100MS;   
}

U8 IsExpiredSegDispTimer(U8 id)
{
    if( SegDispTimer.Timer[id] == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

void SetSegDispTimer(U8 id, U16 time)
{
    SegDispTimer.Timer[id] = time;
}


U8 IsExpiredSegDispBlinkTimer(U8 id)
{
    if( SegDispTimer.TimerBlink[id] == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

static void ResetSegDisplayBlinkTimer(U8 id)
{
    SegDispTimer.TimerBlink[id] = SegDispTimer.TimerCount[id];
}

U8 BlinkSEG(U8 onOff, const char *str, U8 id)
{
    if( IsExpiredSegDispBlinkTimer(id) == TRUE )
    {
        if( onOff == OFF )
        {
            onOff = ON;
            DispGaSegmentStr(str);
        }
        else
        {
            onOff = OFF; 
            AllOffGroupASeg();
        }
    }

    return onOff;
}


// @100ms
/*
void BlinkSegOff(void)
{
    static U8 mu8Counter = 10;

    if( mu8Counter <= 5 )
    {
        DispSegStr("OFF");
    }
    else
    {
        AllOffGroupASeg();
    }  
    
    mu8Counter--;
    if( mu8Counter == 0 )
    {
        mu8Counter = 10;
    }
}
*/


void UpdateDisplayTimer(void)
{
    U8 i = 0U;

    /* Update Blink Timer */
    for( i=0; i<DISP_TIMER_ID_BLINK_MAX; i++ )
    {
        // Toggle Blink LED 
        if( DispTimer.TimerBlink[i] == 0 )
        {
            ResetDisplayBlinkTimer(i);
        }
        else
        {
            DispTimer.TimerBlink[i]--;
        }
    }

    /* Update Key Timer */
    for( i=0; i<DISP_TIMER_ID_MAX; i++ )
    {
        if( DispTimer.Timer[ i ] != 0 )
        {
            DispTimer.Timer[ i ]--;
        }
    }
}

void UpdateSegDisplayTimer(void)
{
    U8 i = 0U;

    /* Update Blink Timer */
    for( i=0; i<DISP_TIMER_ID_BLINK_MAX; i++ )
    {
        // Toggle Blink LED 
        if( SegDispTimer.TimerBlink[i] == 0 )
        {
            ResetSegDisplayBlinkTimer(i);
        }
        else
        {
            SegDispTimer.TimerBlink[i]--;
        }
    }

    /* Update Key Timer */
    for( i=0; i<DISP_TIMER_ID_MAX; i++ )
    {
        if( SegDispTimer.Timer[ i ] != 0 )
        {
            SegDispTimer.Timer[ i ]--;
        }
    }
}
