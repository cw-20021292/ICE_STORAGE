/**
* File : api_uv.c
* 
* Application Programming Interface
* Depend on HAL
*/
#include "hw.h"

#include "api_uv.h"
  
#define UV_OFF      (0)
#define UV_ON       (1)
 
 void API_TurnOnUv(SUvId_T id)
 {
     if( UV_ID_ICE_DOOR  == id )
     {
         TURN_ON_UV_ICE_DOOR();
     }
     else if( UV_ID_WATER_OUT == id )
     {
         TURN_ON_UV_WATER_OUT();
     }
     else if( UV_ID_ICE_TANK == id )
     {
         TURN_ON_UV_ICE_TANK();
     }
     else if( UV_ID_ICE_TRAY == id )
     {
         TURN_ON_UV_ICE_TRAY();
     }
     else if( UV_ID_DRAIN_TANK == id )
     {
         TURN_ON_UV_DRAIN_TANK();
     }
 }
 
 void API_TurnOffUv(SUvId_T id)
 {
     if( UV_ID_ICE_DOOR  == id )
     {
         TURN_OFF_UV_ICE_DOOR();
     }
     else if( UV_ID_WATER_OUT    == id )
     {
         TURN_OFF_UV_WATER_OUT();
     }
     else if( UV_ID_ICE_TANK  == id )
     {
         TURN_OFF_UV_ICE_TANK();
     }
     else if( UV_ID_ICE_TRAY  == id )
     {
         TURN_OFF_UV_ICE_TRAY();
     }
     else if( UV_ID_DRAIN_TANK  == id )
     {
         TURN_OFF_UV_DRAIN_TANK();
     }
 }
 
 
 // Return UV On/Off Status..
 // 0: OFF
 // 1: ON
 U8 API_GetUvOnOffStatus(SUvId_T id)
 {
     U8 mu8OnOff = OFF;
 
     if( UV_ID_ICE_DOOR  == id )
     {
         mu8OnOff = GET_STATUS_UV_ICE_DOOR();
     }
     else if( UV_ID_WATER_OUT == id )
     {
         mu8OnOff = GET_STATUS_UV_WATER_OUT();
     }
     else if( UV_ID_ICE_TANK == id )
     {
         mu8OnOff = GET_STATUS_UV_ICE_TANK();
     }
     else if( UV_ID_ICE_TRAY == id )
     {
         mu8OnOff = GET_STATUS_UV_ICE_TRAY();
     }
     else if( UV_ID_DRAIN_TANK == id )
     {
         mu8OnOff = GET_STATUS_UV_DRAIN_TANK();
     }
 
     return mu8OnOff;
 } 
