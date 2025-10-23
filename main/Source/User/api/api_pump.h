/**
 * File : api_pump.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_PUMP_H__
#define __API_PUMP_H__

#include "prj_type.h"

typedef enum _pump_id_
{
    PUMP_ID_NONE = 0,     // 0 : None
    PUMP_ID_DRAIN,        // 1 : Drain Pump
    PUMP_ID_CIRCULATE,    // 2 : Circulate Pump
    PUMP_ID_ICE_DRAIN,    // 3 : Ice Water Drain
    PUMP_ID_OPTION,       // 4 : Option 
    PUMP_ID_MAX,          // 5 : Max
} SPumpId_T;

U8 IsTurnOnDrainPump(void);
U8 IsTurnOnIceDrainPump(void);

void TurnOnPump(SPumpId_T pumpId, U8 duty);
void TurnOffPump(SPumpId_T pumpId);

#endif
