/**
 * File : api_relay.c
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#include "hw.h"

#include "api_relay.h"

void OnRelay(ERelayId_T	relay)
{
    if( RELAY_ID_COMP == relay )
    {
        TURN_ON_COMP();
    }
    else if( RELAY_ID_HEATER == relay )
    {
        TURN_ON_HEATER();
    }
    else if( RELAY_ID_FEEDER_CW == relay )
    {
        TURN_ON_MOTOR_FEEDER_CW();
    }
    else if( RELAY_ID_FEEDER_CCW == relay )
    {
        TURN_ON_MOTOR_FEEDER_CCW();
    }
    else if( RELAY_ID_ICE_TRAY_CW == relay )
    {
        TURN_ON_MOTOR_ICE_TRAY_CW();
    }
    else if( RELAY_ID_ICE_TRAY_CCW == relay )
    {
        TURN_ON_MOTOR_ICE_TRAY_CCW();
    }
}

void OffRelay(ERelayId_T	relay)
{
    if( RELAY_ID_COMP == relay )
    {
        TURN_OFF_COMP();
    }
    else if( RELAY_ID_HEATER == relay )
    {
        TURN_OFF_HEATER();
    }
    else if( RELAY_ID_FEEDER_CW == relay )
    {
        TURN_OFF_MOTOR_FEEDER_CW();
    }
    else if( RELAY_ID_FEEDER_CCW == relay )
    {
        TURN_OFF_MOTOR_FEEDER_CCW();
    }
    else if( RELAY_ID_ICE_TRAY_CW == relay )
    {
        TURN_OFF_MOTOR_ICE_TRAY_CW();
    }
    else if( RELAY_ID_ICE_TRAY_CCW == relay )
    {
        TURN_OFF_MOTOR_ICE_TRAY_CCW();
    }
}
