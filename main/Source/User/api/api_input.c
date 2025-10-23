/**
 * File : api_input.c
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#include "hw.h"

#include "api_input.h"

/// Model Type ( PULL-UP : 5KG, PULL-DOWN : 3KG )
U8 GetPinModelType(void)
{
    return (U8)GET_STATUS_MODEL_TYPE();
}

/// Tank Cover
U8 GetPinTankCover(void)
{
    return (U8)GET_STATUS_REED_TANK_OPEN();
}

/// Ice Tank Bracket
U8 GetPinIceTankBracket(void)
{
    return (U8)GET_STATUS_REED_TRAY_BRACKET();
}

/// Level ( Room Tank )
U8 GetPinLevelRoomTankOverflow(void)
{
    return (U8)GET_STATUS_LEVEL_ROOM_TANK_OVERFLOW();
}

U8 GetPinLevelRoomTankLow(void)
{
    return (U8)GET_STATUS_LEVEL_ROOM_TANK_LOW();
}

U8 GetPinLevelRoomTankHigh(void)
{
    return (U8)GET_STATUS_LEVEL_ROOM_TANK_HIGH();
}

/// Level ( Cold Tank )
U8 GetPinLevelColdTankLow(void)
{
    return (U8)GET_STATUS_LEVEL_COLD_TANK_LOW();
}

U8 GetPinLevelColdTankHigh(void)
{
    return (U8)GET_STATUS_LEVEL_COLD_TANK_HIGH();
}

/// Level ( Drain Tank )
U8 GetPinLevelDrainTankLow(void)
{
    return (U8)GET_STATUS_LEVEL_DRAIN_TANK_LOW();
}

U8 GetPinLevelDrainTankHigh(void)
{
    return (U8)GET_STATUS_LEVEL_DRAIN_TANK_HIGH();
}
