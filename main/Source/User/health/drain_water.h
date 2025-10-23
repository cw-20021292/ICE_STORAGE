#ifndef __DRAIN_WATER_H__
#define __DRAIN_WATER_H__

#include "prj_type.h"

typedef struct _drain_water_
{
    U8 Start;     // ��� ���� ( 0: ����, 1: ���� )
    U8 Mode;      // Ÿ�� ( 0: HALF DRAIN, 1: FULL DRAIN )

    U8 SkipResetDrain;  // ����Ű�� ���� ��� ���� �� Ű�� POP �� �� Ű�� �������� �ν��Ͽ� ��� ��ҵǴ� ���� �ذ��� ���� FLAG

    U32 DrainTime;    // ��� ��ǥ �ð� @100ms

    U32 ConsumRoomWater;      // ���� �ּ� �� ��뷮(�ð�@100ms)
    U32 ConsumColdWater;      // �ü� �ּ� �� ��뷮(�ð�@100ms)

    U8 CheckRoomLevel;        // ���� ������ �˻� ����( 0: �̰˻�, 1: �˻� �Ϸ� )
    U16 CheckRoomTime;        // ���� ������ ���� �˻� �ð�(@100ms)
} DrainWater_T;


void  InitDrainWater(void);
void  GetDrainWaterData(DrainWater_T *pData);

// ��� ����/����
#define  HALF_DRAIN  0     // �ݸ� ����
#define  FULL_DRAIN  1     // ���� ����
U8 StartDrainWater(U8 mu8Mode);
U8 IsStartDrainWater(void);

// ��� Ÿ��( HALF or FULL )
// RETURN :
// TRUE - DRAIN_HALF Ÿ��
// FALSE - DRAIN_FULL Ÿ��
U8 IsDrainWaterHalf(void);

void SetSkipResetDrain(U8 Skip);
U8 IsSkipResetDrain(void);

// ��ȯ ���� ��� ���̸�, TRUE
// �׷��� ������ FALSE
U8 IsGoingOnDrainWaterFull(void);

U8 ResetDrainWater(void);

void ControlDrainWater(void);

#endif /* __DRAIN_WATER_H__ */
