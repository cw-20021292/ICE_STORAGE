/**
 * File : api_uv.h
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#ifndef __HAL_UV_H__
#define __HAL_UV_H__

#include "prj_type.h"

typedef enum _uv_id_
{
    UV_ID_ICE_DOOR,
    UV_ID_WATER_OUT,
    UV_ID_ICE_TANK,
    UV_ID_ICE_TRAY,
    UV_ID_DRAIN_TANK,
    UV_ID_MAX
} SUvId_T;

void API_TurnOnUv(SUvId_T id);

void API_TurnOffUv(SUvId_T id);


// Return UV On/Off Status..
// 0: OFF
// 1: ON
U8 API_GetUvOnOffStatus(SUvId_T id);

#endif /* __HAL_UV_H__ */
