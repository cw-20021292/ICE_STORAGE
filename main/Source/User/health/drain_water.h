#ifndef __DRAIN_WATER_H__
#define __DRAIN_WATER_H__

#include "prj_type.h"

typedef struct _drain_water_
{
    U8 Start;     // 배수 동작 ( 0: 중지, 1: 시작 )
    U8 Mode;      // 타입 ( 0: HALF DRAIN, 1: FULL DRAIN )

    U8 SkipResetDrain;  // 조합키로 강제 배수 실행 후 키를 POP 할 때 키가 눌림으로 인식하여 즉시 취소되는 문제 해결을 위한 FLAG

    U32 DrainTime;    // 배수 목표 시간 @100ms

    U32 ConsumRoomWater;      // 정수 최소 물 사용량(시간@100ms)
    U32 ConsumColdWater;      // 냉수 최소 물 사용량(시간@100ms)

    U8 CheckRoomLevel;        // 정수 만수위 검사 여부( 0: 미검사, 1: 검사 완료 )
    U16 CheckRoomTime;        // 정수 만수위 조건 검사 시간(@100ms)
} DrainWater_T;


void  InitDrainWater(void);
void  GetDrainWaterData(DrainWater_T *pData);

// 배수 시작/중지
#define  HALF_DRAIN  0     // 반만 배출
#define  FULL_DRAIN  1     // 전부 배출
U8 StartDrainWater(U8 mu8Mode);
U8 IsStartDrainWater(void);

// 배수 타입( HALF or FULL )
// RETURN :
// TRUE - DRAIN_HALF 타입
// FALSE - DRAIN_FULL 타입
U8 IsDrainWaterHalf(void);

void SetSkipResetDrain(U8 Skip);
U8 IsSkipResetDrain(void);

// 순환 강제 배수 중이면, TRUE
// 그렇지 않으면 FALSE
U8 IsGoingOnDrainWaterFull(void);

U8 ResetDrainWater(void);

void ControlDrainWater(void);

#endif /* __DRAIN_WATER_H__ */
