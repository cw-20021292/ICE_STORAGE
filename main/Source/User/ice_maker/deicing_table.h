/**
 * File : deicing_table.h
 * 
 * Deicing Table
*/
#ifndef __DEICING_TABLE_H__
#define __DEICING_TABLE_H__

#include "prj_type.h"

/* DE-ICING REGION */
#define  DEICING_REGION_1   (0)       // ~13'c
#define  DEICING_REGION_2   (1)       // ~22'c
#define  DEICING_REGION_3   (2)       // ~30'c
#define  DEICING_REGION_4   (3)       // 30'c~
U8 GetDeIcingRegion(TEMP_T _tAmbi);

U8 GetDeIcingRepeatNum(U8 region);

U8 GetDeIcingCmd(U8 index);

U16 GetDeIcingTime(U8 region, U8 index);

U8 GetDeIcingCompRps(U8 region);

U8 GetDeIcingFanOnOff(U8 region);

#endif /* __DEICING_TABLE_H__ */
