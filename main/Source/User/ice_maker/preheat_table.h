#ifndef __PREHEAT_TABLE_H__
#define __PREHEAT_TABLE_H__

#include "prj_type.h"

/* PREHEAT REGION */
#define  PREHEAT_REGION_1   0
#define  PREHEAT_REGION_2   1
U8 GetPreheatRegion( TEMP_T _tAmbi );

U8 GetPreheatRepeatNum(U8 mu8Region );

U8 GetPreheatCmd(U8 mu8Index);

U16 GetPreheatTime(U8 mu8Region, U8 mu8Index);

U8 GetPreheatCompRps(U8 mu8Region);

#endif  /* __PREHEAT_TABLE_H__ */

