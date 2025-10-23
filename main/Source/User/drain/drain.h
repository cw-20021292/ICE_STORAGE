/**
 * File : drain.h
 * 
 * Drain Tank Level Control Module
*/
#ifndef __DRAIN_H__
#define __DRAIN_H__

#include "prj_type.h"

#include "level.h"

void  InitDrain(void);

void  SetDrainLevel(U8 val);
U8    GetDrainLevel(void);

void  SetInitDrain( U8 val);
U8    GetInitDrain(void);

void  SetInitDrainReady(U8 val);
U8    GetInitDrainReady(void);

void StartDrainPumpOnLevel(void);

void ControlDrainLevel(void);

#endif /* __DRAIN_H__ */
