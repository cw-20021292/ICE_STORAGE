/**
 * File : heater.c
 * 
 * Heater
*/
#include "heater.h"

#include "relay.h"

#define  MAX_TIME  (60000UL)  /// sec

typedef struct _heater_water_
{
    /* COMMAND */
    U8 Mode;         // Command mode
    U8 ForcedOnOff;  // Forced mode - onoff command

    U32 OnTime;      // @1sec
    U32 OffTime;     // @1sec
} Heater_T;

Heater_T Heater;

void InitHeater(void)
{
    Heater.Mode = HEATER_MODE_NONE;
    
    Heater.ForcedOnOff = OFF;

    Heater.OnTime = 0;
    Heater.OffTime = 0;
}

void SetHeaterMode(U8 mode)
{
    Heater.Mode = mode;
}

U8 GetHeaterMode(U8 mode)
{
    return Heater.Mode;
}

void SetHeaterForcedOnOff(U8 onOff)
{
    Heater.ForcedOnOff = onOff;
}

U8 GetHeaterOnOff(void)
{
    if( IsTurnOnRelay(RELAY_HEATER) == TRUE )
    {
        return ON;
    }

    return FALSE;
}


U32 GetHeaterOnTime(void)
{
    return Heater.OnTime;
}

U32 GetHeaterOffTime(void)
{
    return Heater.OffTime;
}

void ControlHeater(U8 onOff)
{
    if( Heater.Mode == HEATER_MODE_FORCED )
    {
        onOff = Heater.ForcedOnOff;
    }

    if( onOff == TRUE )
    {
        TurnOnRelay(RELAY_HEATER);

        if( Heater.OnTime < MAX_TIME )
        {
            Heater.OnTime++;
        }
        Heater.OffTime = 0;
    }
    else 
    {
        TurnOffRelay(RELAY_HEATER);

        Heater.OnTime = 0;
        if( Heater.OffTime < MAX_TIME )
        {
            Heater.OffTime++;
        }
    }
}

