/**
 * File : smps.h
 * 
 * Smps Control
*/
#include "hw.h"
#include "smps.h"

#include "api_uv.h"
#include "api_pump.h"
#include "gas_switch_valve.h"
#include "hotgas_switch_valve.h"

// ICE DOOR, ICE WATER on Front board...
static U8 IsTurnOnUvOnBoard(void)
{  
    if( API_GetUvOnOffStatus(UV_ID_ICE_DOOR) != 0 )
    {
        return TRUE;    // ON
    }

    if( API_GetUvOnOffStatus(UV_ID_WATER_OUT) != 0 )
    {
        return TRUE;    // ON
    }
    
    if( API_GetUvOnOffStatus(UV_ID_ICE_TANK) != 0 )
    {
        return TRUE;    // ON
    }

    if( API_GetUvOnOffStatus(UV_ID_ICE_TRAY) != 0 )
    {
        return TRUE;    // ON
    }

    return FALSE;   // OFF
}

void ControlSmps(void)
{
    U8 onOff = OFF;

    // GAS VALVE
    if( GetGasSwitchMode() != GAS_SWITCH_MODE_NONE )
    {
        onOff = ON;
    }

    // HOTGAS VALVE
    if( GetHotGasSwitchMode() != HOTGAS_SWITCH_MODE_NONE )
    {
        onOff = ON;
    }

    // UV CHECK
    if( IsTurnOnUvOnBoard() == ON )
    {
        onOff = ON;
    }

    /// Drain Pump
    if( IsTurnOnDrainPump() == TRUE )
    {
        onOff = ON;
    }

    /// Ice Drain Pump
    if( IsTurnOnIceDrainPump() == TRUE )
    {
        onOff = ON;
    }

    // CONTROL SMPS 12V
    if( onOff == ON )
    {
        TURN_ON_SMPS_12V();
    }
    else
    {
        TURN_OFF_SMPS_12V();
    }
}
