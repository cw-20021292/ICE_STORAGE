/**
 * File : aging.h
 *
 * Aging ( Quility Aging Test )
**/
#ifndef __AGING_H__
#define __AGING_H__

#include "prj_type.h"

void InitAging(void);

void StartAging(void);
void StopAging(void);
U8 GetAging(void);

U8 IsExpiredAgingLimitTimer(void);

U32 GetCycleCount(void);

void ProcessAging(void);

#endif  /* __AGING_H__ */
