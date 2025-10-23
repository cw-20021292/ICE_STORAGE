/**
 * File : api_relay.h
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#ifndef __API_RELAY_H__
#define __API_RELAY_H__

typedef enum _realy_id_
{
    RELAY_ID_COMP = 0,       // 0 : 
    RELAY_ID_HEATER,         // 1 :
    RELAY_ID_FEEDER_CW,      // 2 :
    RELAY_ID_FEEDER_CCW,     // 3 :
    RELAY_ID_ICE_TRAY_CW,    // 4 :
    RELAY_ID_ICE_TRAY_CCW,   // 5 :
    RELAY_ID_MAX,            // 6 :
} ERelayId_T;

void OnRelay(ERelayId_T	relay);
void OffRelay(ERelayId_T	relay);

#endif  /* __API_RELAY_H__ */
