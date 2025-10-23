/**
 * File : fct.c
 * 
 * Fct
*/
#include "fct.h"

#define DEFAULT_LIMIT_TIME_VALUE      (30U)     /* 30sec, @1sec */
#define DEFAULT_TEST_TIME_VALUE       (1800U)   /* 5min, @1sec */

typedef struct _fct_
{
    U8 Status;          /* START or STOP */
    U16 LimitTimer;     /* Enter FCT Mode Timeout */
    U16 TestTimer;      /* Interval Full Test Timer */

    /* CHECK INPUT */
    U32 InputTest;      /* Test Value */
} SFct_T;

SFct_T fct;

void InitFct(void)
{
    fct.Status = FALSE; 
    fct.LimitTimer = DEFAULT_LIMIT_TIME_VALUE;
    fct.TestTimer = DEFAULT_TEST_TIME_VALUE;
    fct.InputTest = 0UL;
}

void StartFct(void)
{
    if( fct.LimitTimer == 0 )
    {
        return ;
    }

    fct.Status = TRUE;
}

void StopFct(void)
{
    fct.Status = FALSE;
}

U8 IsExpiredFctLimitTimer(void)
{
    if( fct.LimitTimer != 0 )
    {
        return FALSE;   // no expired
    }
    return TRUE;  // expired!!
}

U8 GetFctStatus(void)
{
    return fct.Status;
}

void SetFctTestTimer(U16 time)
{
    fct.TestTimer = time;
}

U16 GetFctTestTimer(void)
{
    return fct.TestTimer;
}

// Test Input
void SetFctTestInputBit(U32 maskBit)
{
    fct.InputTest |= maskBit;
}

void ClearFctTestInputBit(U32 maskBit)
{
    fct.InputTest &= ~maskBit;
}

void SetFctTestInputVal(U32 maskBit)
{
    fct.InputTest = maskBit;
}

U8 IsSetFctTestInputVal(U32 maskBit)
{
    if( (fct.InputTest & maskBit) != 0 )
    {
        return TRUE;
    }

    return FALSE;
}

U32 GetFctTestInputVal(void)
{
    return fct.InputTest;
}

void UpdateFctTimer(void)
{
    if( fct.LimitTimer != 0 )
    {
        fct.LimitTimer--;
    }
    if( fct.TestTimer != 0 )
    {
        fct.TestTimer--;
    }
}
