/**
 * File : drain_pump.h
 * 
 * Drain Pump For Empty Drain Tank
*/
#ifndef __DRAIN_PUMP_H__
#define __DRAIN_PUMP_H__

#include "prj_type.h"

typedef struct _drain_pump_
{
    U8   Cmd;        // On/Off command ( 1: ON, 0: OFF )

    U8   RetryCmd;
    U8   RetryCount;
    U16  OnOffTime;     // @1sec
    U8   Error;

    U16 LoadTime;
    U8  Load;

    U16 DeadTime;
    U8  Dead;  
} SDrainPump_T;

void InitDrainPump(void);

U8 GetDrainPumpStatus(void);

void ClearDrainPumpRetryCount(void);

U8 GetDrainPumpError(void);

void TurnOnDrainPump(void);

void TurnOffDrainPump(void);

U8 GetDrainPumpLoadStatus(void);

void ControlDrainPump(void);

void GetDrainData(SDrainPump_T *pData);

#endif /* __DRAIN_PUMP_H__ */
