#ifndef __DUMMY_DEICING_TABLE_H__
#define __DUMMY_DEICING_TABLE_H__

#include "prj_type.h"

/* DE-ICING REGION */
#define  DUMMY_DEICING_REGION_1   0       // ~14'c
#define  DUMMY_DEICING_REGION_2   1       // ~22'c
#define  DUMMY_DEICING_REGION_3   2       // ~30'c
#define  DUMMY_DEICING_REGION_4   3       // 30'c~
U8 GetDummyDeIcingRegion( TEMP_T _tAmbi );

U8 GetDummyDeIcingRepeatNum(U8 region );

U8 GetDummyDeIcingCmd(U8 index);

U16 GetDummyDeIcingTime(U8 region, U8 index);

/* ���� Ż�� ����� ���� �ð�
 *
 * @param : mu8Region - �ܱ� �µ� ����
 * @return : ����� �����ð� ( sec )
 */
U16 GetDummyDeIcingCompOnTime(U8 region, U8 index);

#endif /* __DUMMY_DEICING_TABLE_H__ */
