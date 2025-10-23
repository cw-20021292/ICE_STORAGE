/**
 * File : leak.c
 * 
 * Leak Sensor
**/
#include "hw.h"
#include "leak.h"

#include "api_adc.h"

#define CHECK_NORMAL_TIME           (600) /* 60sec, @100ms */
#define CHECK_ERROR_TIME            (30)  /* 3sec, @100ms */

SLeakage_T  gStLeak;

void InitLeak(void)
{
    gStLeak.mUstatus = FALSE;
    
    gStLeak.mUcheckNormalTime = CHECK_NORMAL_TIME;
    gStLeak.mUcheckErrorTime = CHECK_ERROR_TIME;  

    //TURN_ON_LEAK_SENSOR_POWER_ON_OFF();
}

void GetLeakData( SLeakage_T *xStptrData )
{
    MEMCPY( (void __FAR *)xStptrData, (const void __FAR *)&gStLeak, sizeof( SLeakage_T ));
}

U8 GetLeakStatus(void)
{
    return gStLeak.mUstatus;
}

U8 IsLeakSemi(void)
{
    U16 uAdc = 0;

    //TURN_ON_LEAK_SENSOR_POWER_ON_OFF();

    InitAdc();
    //uAdc = GetAdcData( ADC_ID_LEAK_FB );
    if( uAdc > 50 )
    {
        return TRUE;
    }

    return FALSE;
}

U16 dbgLeak = 0;
static void ControlLeak(void)
{
    U16 uAdc = 0U;

    //uAdc = GetAdcData( ADC_ID_LEAK_FB );

    if( dbgLeak == 1)
    {
        uAdc = 700;
    }
    
    if( uAdc >= LEAK_ADC_LEVEL )
    {
        // ���� ����
        gStLeak.mUcheckNormalTime = CHECK_NORMAL_TIME;
        if( gStLeak.mUcheckErrorTime != 0 )
        {
            gStLeak.mUcheckErrorTime--;
        }    
    }
    else
    {
        // ���� ����
        if( gStLeak.mUcheckNormalTime != 0 )
        {
            gStLeak.mUcheckNormalTime--;
        }
        gStLeak.mUcheckErrorTime = CHECK_ERROR_TIME;    
    }

    if( gStLeak.mUcheckNormalTime == 0 )
    {
        gStLeak.mUstatus = FALSE;         // ���� ���� ����
    }
    else if( gStLeak.mUcheckErrorTime == 0 )
    {
        gStLeak.mUstatus = TRUE;          // ���� ����
    }
}

void ProcessLeak(void)
{
    ControlLeak();
}
