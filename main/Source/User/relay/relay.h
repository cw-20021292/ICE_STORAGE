/**
 * File : relay.h
 * 
 * Relay
*/
#ifndef __RELAY_H__
#define __RELAY_H__

#include "prj_type.h"

#include "api_relay.h"

/* api_relay.h �� Relay Id ������ ���� �°� ������ �־�� �� */
#define RELAY_COMP            (0x0001)
#define RELAY_HEATER          (0x0002)
#define RELAY_FEEDER_CW       (0x0004)
#define RELAY_FEEDER_CCW      (0x0008)
#define RELAY_ICE_TRAY_CW     (0x0010)
#define RELAY_ICE_TRAY_CCW    (0x0020)
#define MAX_RELAY_NUM         (RELAY_ID_MAX)

void InitRelay(void);

void TurnOnRelay(U16 relay);
void TurnOffRelay(U16 relay);
void TurnOffRelayAll(void);

U8 IsTurnOnRelay(U16 relay);

U16 GetRelayStatus(void);

void ProcessRelayExclusive(void);
void ProcessRelayConcurrent(void);

#if 0 // unused
void TEST_ControlRelay(void);
#endif

#endif /* __RELAY_H__ */
