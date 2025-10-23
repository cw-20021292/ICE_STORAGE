#ifndef __ICING_TABLE_H__
#define __ICING_TABLE_H__

#include "prj_type.h"


// ���� ���� �µ� ��ȿ ���� Ȯ��
U16 IsValidRangeTemp(TEMP_T tRoom, TEMP_T tAmbi );

// ���� �ð� ��ȯ ( ���� : �� )
U16 GetIcingTime(TEMP_T tRoom, TEMP_T tAmbi );

// ���� Comp Rps ��ȯ
U8 GetIcingCompRps( TEMP_T tAmbi );

// ���� Fan on/off ��ȯ
U8 GetIcingFanOnOff(TEMP_T tAmbi);

#endif /* __ICING_TABLE_H__ */
