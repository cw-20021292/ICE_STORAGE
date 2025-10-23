/**
 * File : api_valve.c
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#include "hw.h"

#include "api_valve.h"

/**
 * @brief               Function For On Power Of Water Out Valve
 * @detail              none
 * @param               none
 * @return              none
**/
static void Power(void)
{
    if( (GET_STATUS_VALVE_ROOM_OUT() != 0)
    || (GET_STATUS_VALVE_COLD_CIRCULATE() != 0)
    || (GET_STATUS_VALVE_HOT_OUT() != 0)
    || (GET_STATUS_VALVE_ICE_WATER_OUT() != 0)   
      )
    {
        OPEN_VALVE_POWER_OUT();
    }
    else
    {
        CLOSE_VALVE_POWER_OUT();
    }
}

/**
 * @brief             Function For Open Valve
 * @detail            none
 * @param       id    valve Id
 * @return            none
**/
static void Open(EValveId_T id)
{
    if( VALVE_ID_NOS == id )
    {
        OPEN_VALVE_NOS();
    }
    else if( VALVE_ID_FEED == id )
    {
        OPEN_VALVE_FEED();
    }
    else if( VALVE_ID_COLD_IN == id )
    {
        OPEN_VALVE_COLD_IN();
    }
    else if( VALVE_ID_ROOM_OUT == id )
    {
        OPEN_VALVE_ROOM_OUT();
    }
    else if( VALVE_ID_HOT_OUT == id )
    {
        OPEN_VALVE_HOT_OUT();
    }
    else if( VALVE_ID_COLD_CIRCULATE == id )
    {
        OPEN_VALVE_COLD_CIRCULATE();
    }
    else if( VALVE_ID_ICE_WATER_OUT == id )
    {
        OPEN_VALVE_ICE_WATER_OUT();
    }
    else if( VALVE_ID_ICE_TRAY_IN == id )
    {
        OPEN_VALVE_ICE_TRAY_IN();
    }
    else if( VALVE_ID_DRAIN == id )
    {
        OPEN_VALVE_DRAIN();
    }
    else if( VAVLE_ID_NOS_FLOW == id )
    {
        OPEN_VALVE_NOS_FLOW();
    }
    else if( VALVE_ID_EXTRA == id )
    {
        OPEN_VALVE_EXTRA();
    }
}

/**
 * @brief               Function For Close Valve
 * @detail              none
 * @param       xEid    valve Id
 * @return              none
**/
static void Close(EValveId_T id)
{
    if( VALVE_ID_NOS == id )
    {
        CLOSE_VALVE_NOS();
    }
    else if( VALVE_ID_FEED == id )
    {
        CLOSE_VALVE_FEED();
    }
    else if( VALVE_ID_COLD_IN == id )
    {
        CLOSE_VALVE_COLD_IN();
    }
    else if( VALVE_ID_ROOM_OUT == id )
    {
        CLOSE_VALVE_ROOM_OUT();
    }
    else if( VALVE_ID_HOT_OUT == id )
    {
        CLOSE_VALVE_HOT_OUT();
    }
    else if( VALVE_ID_COLD_CIRCULATE == id )
    {
        CLOSE_VALVE_COLD_CIRCULATE();
    }
    else if( VALVE_ID_ICE_WATER_OUT == id )
    {
        CLOSE_VALVE_ICE_WATER_OUT();
    }
    else if( VALVE_ID_ICE_TRAY_IN == id )
    {
        CLOSE_VALVE_ICE_TRAY_IN();
    }
    else if( VALVE_ID_DRAIN == id )
    {
        CLOSE_VALVE_DRAIN();
    }
    else if( VAVLE_ID_NOS_FLOW == id )
    {
        CLOSE_VALVE_NOS_FLOW();
    }
    else if( VALVE_ID_EXTRA == id )
    {
        CLOSE_VALVE_EXTRA();
    }
}


void API_OpenValve(EValveId_T id)
{
    Open(id);
    Power();
}

void API_CloseValve(EValveId_T id)
{
    Close(id);
    Power();
}
