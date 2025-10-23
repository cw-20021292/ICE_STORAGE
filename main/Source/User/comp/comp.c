/**
 * File : comp.c
 * 
 * Comp Module
*/
#include "comp.h"

#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_RELAY)
#include "relay.h"
#else
#include "comp_bldc.h"
#endif

//#include <string.h>

/// 압축기 연속 운전 시간 제한 
#define CONFIG_PROTECT_ON_TIME     (1U)       /// 1: 연속 운전 시간 제한 사용, 0: 미사용

/// PROTECT TIME
#define  PROTECT_OFF_TIME          (300UL)    /// 5min( 5min x 60sec  = 300sec )
#define  PROTECT_ON_TIME           (9000UL)   /// 150min ( 150min x 60sec =  9000sec )
#define  PROTECT_SECOND_ON_TIME    (11400UL)  /// 190min ( 190min x 60sec = 114000sec )

/// ON/OFF TIME LIMITED
#define  MAX_TIME                  (60000UL)  /// sec

SCompressor_T Comp;

void InitComp(void)
{
    Comp.mode = COMP_MODE_NONE;
    Comp.coldOnOff = OFF;
    Comp.iceOnOff = OFF;
    Comp.forcedOnOff = OFF;

    Comp.protectOffTime = PROTECT_OFF_TIME;

    Comp.onTime = 0;
    Comp.offTime = 0;

    InitCompBldc();
}

void GetCompData( SCompressor_T *pData )
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Comp, sizeof(SCompressor_T) );
}

void SetCompMode(U8 mode)
{
    Comp.mode = mode;
}

U8 GetCompMode(U8 mode)
{
    return Comp.mode;
}

void SetCompProtectOffTime(U16 val)
{
    Comp.protectOffTime = val;
}

U16 GetCompProtectOffTime(void)
{
    return Comp.protectOffTime;
}

U8 IsOverCompProtectOnTime(void)
{
#if( CONFIG_PROTECT_ON_TIME == 1 )
    if( Comp.onTime > PROTECT_ON_TIME )
    {
        return TRUE; // 보호 시간 초과
    }
#endif

    return FALSE; // 보호 시간 이내
}

U8 IsOverCompProtectSecondOnTime(void)
{
#if( CONFIG_PROTECT_ON_TIME == 1 )
    if( Comp.onTime > PROTECT_SECOND_ON_TIME )
    {
        return TRUE; // 보호 시간 초과
    }
#endif

    return FALSE; // 보호 시간 이내
}

// Turn On, with RPS
void TurnOnCompRps(U8 type, RPS_T rps)
{
    TurnOnComp(type);
#if( CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC )
    SetCompBldcTargetRps(rps);
#endif
}

void TurnOnComp(U8 type)
{
    if( type == COMP_TYPE_COLD )
    {
        Comp.coldOnOff = ON;
    }
    else
    {
        Comp.iceOnOff = ON;
    }
}

void TurnOffComp(U8 type)
{
    if( type == COMP_TYPE_COLD )
    {
        Comp.coldOnOff = OFF;
    }
    else
    {
        Comp.iceOnOff = OFF;
    }
}

U8 GetCompOnOffType(U8 type)
{
    if( type == COMP_TYPE_COLD )
    {
        return Comp.coldOnOff;
    }

    return Comp.iceOnOff;
}

void TurnOnForcedComp(void)
{
    Comp.forcedOnOff = ON;
}

void TurnOffForcedComp(void)
{
    Comp.forcedOnOff = OFF;
}

U32 GetCompOnTime(void)
{
    return Comp.onTime;
}

U32 GetCompOffTime(void)
{
    return Comp.offTime;
}

static U8 IsTurnOnComp(void)
{
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    if( IsTurnOnCompBldc() == TRUE )
    {
        return TRUE;
    }
#else
    if( IsTurnOnRelay(RELAY_COMP) == TRUE )
    {
        return TRUE;
    }
#endif

    return FALSE;
}

static void TurnOnCompType(void)
{
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    if( GetCompBldcTargetRps() < MIN_RPS )
    {
        SetCompBldcTargetRps(MIN_RPS);
    }
#else
    TurnOnRelay(RELAY_COMP);
#endif
}

static void TurnOffCompType(void)
{
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    SetCompBldcTargetRps( 0 );
#else
    TurnOffRelay(RELAY_COMP);
#endif
}

U8 GetCompOnOff(void)
{
    if( IsTurnOnComp() == TRUE )
    {
        return ON;
    }

    return OFF;
}


U8 IsExpiredCompProtectTime(void)
{
    if( IsTurnOnComp() == FALSE 
            && Comp.protectOffTime != 0 )
    {
        return FALSE;   // COMP OFF 상태
    }

    return TRUE;    // COMP ON 상태
}

static void UpdateCompTimer(void)
{
    // 압축기가 OFF이면, Update off timer value
    // 그렇지 않으면 초기화
    if( IsTurnOnComp() == FALSE )
    {
        if( Comp.protectOffTime != 0 )
        {
            Comp.protectOffTime--;
        }
    }
    else
    {
        Comp.protectOffTime = PROTECT_OFF_TIME;
    }
}

static void ControlComp(void)
{
    U8 onOff = OFF;

    /// 0. Get OnOff Command
    if( Comp.coldOnOff == ON )
    {
        onOff = ON;
    }

    if( Comp.iceOnOff == ON )
    {
        onOff = ON;
    }

    /// 1. Protect Comp Off Time, Priority-2
    if( IsExpiredCompProtectTime() == FALSE )
    {
        onOff = OFF;
    }

    /// 2. Forced OnOff, Prioirty-1
    if( Comp.mode == COMP_MODE_FORCED )
    {
        onOff = Comp.forcedOnOff;
    }
    
    /// 3. Control Compressor 
    if( onOff == ON )
    {
        /// Turn on Comp
        TurnOnCompType();

        /// Count On Timer
        Comp.offTime = 0;
        if( Comp.onTime < MAX_TIME )
        {
            Comp.onTime++;
        }

        /// Reload Protect Comp Off Time
        Comp.protectOffTime = PROTECT_OFF_TIME;

    }
    else 
    {
        /// Reload Protect Comp Off Time 
        if( IsTurnOnComp() == TRUE )
        {
            Comp.protectOffTime = PROTECT_OFF_TIME;
        }
        
        /// Turn Off Comp
        TurnOffCompType();

        /// Count Off timer
        Comp.onTime = 0;
        if( Comp.offTime < MAX_TIME )
        {
            Comp.offTime++;
        }
    }
}

void ProcessComp(void)
{
    ControlComp();
#if (CONFIG_BLDC_COMP_TYPE == COMP_TYPE_BLDC)
    ControlCompBldc();
#endif
    UpdateCompTimer();
}
