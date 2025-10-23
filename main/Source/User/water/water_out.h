/**
 * File : water_out.h
 * 
 * Water Out
*/
#ifndef __WATER_OUT_H__
#define __WATER_OUT_H__

#include "prj_type.h"

void  InitWaterOut(void);

/* 선택 */
#define SEL_WATER_ROOM      (0)
#define SEL_WATER_COLD      (1)
#define SEL_WATER_HOT       (2)
#define MAX_WATER_ID        (3)
void SetWaterOutSelect(U8 id);
U8 GetWaterOutSelect(void);
U8 GetWaterOutLastedSelect(void);

void UpdateWaterOutReturnTime(void);
void ClearWaterOutReturnTime(void);
U8 IsExpiredWaterOutReturnTime(void);

/* 추출/중지 */
void  StartWaterOut(void);
void  StopWaterOut(void);
U8 GetWaterOut(void);

/* 연속 추출 */
void  SetWaterOutContinue(U8 mode);
U8    GetWaterOutContinue(void);

#define FIXED_AMOUNT_500ML       (0)  /// 500mL
#define FIXED_AMOUNT_1L          (1)  /// 1L
#define FIXED_AMOUNT_1D5L        (2)  /// 1.5L
#define FIXED_AMOUNT_INFINITY    (3)  /// INFINITY
#define FIXED_AMOUNT_TANK_CLEAN  (4)  /// TANK CLEAN MODE
#define MAX_FIXED_AMOUNT_NUM     (5)  
void  SetWaterOutFixedAmountType(U8 type);
U8    GetWaterOutFixedAmountType(void);
U8    GetWaterOutAmoutType(void);
void  ClearWaterOutInfinityAmout(void);

void  SetWaterOutFixedAmoutTargetTime(U32 time);
U32   GetWaterOutFixedAmoutTargetTime(void);

void  SetWaterOutFixedAmoutCurrentTime(U32 time);
U32   GetWaterOutFixedAmoutCurrentTime(void);

/* 추출 시간 (마지막, 일일, 누적) */
void  SetWaterOutLastedTime(U8 id, U32 time);
U32  GetWaterOutLastedTime(U8 id );

void  SetWaterOutDailyTime(U8 id, U32 time);
U32  GetWaterOutDailyTime(U8 id );

void  SetWaterOutTotalTime(U8 id, U32 time);
U32  GetWaterOutTotalTime(U8 id );

#define  TYPE_LASTED  (0x01)
#define  TYPE_DAILY   (0x02)
#define  TYPE_TOTAL   (0x04)
#define  TYPE_ALL     (0x07)
void SetWaterOutTime(U8 type, U8 id, U32 time);
U32  GetWaterOutTime(U8 type, U8 id);
void ResetWaterOutTime(U8 type, U8 id);

U32  GetWaterOutTotalTimeAll(void);

/// 추출 시간을 컵 사이즈로 반환
U8 GetWaterOutLastedCupNum(U8 id);    
U8 GetWaterOutDailyCupNum(U8 id);    
U8 GetWaterOutTotalCupNum(U8 id);    

void SetWaterOutAccumulateTime(U8 id, U32 time);
void ClearWaterOutAccumulateTime(U8 id);
U32 GetWaterOutAccumulateTime(U8 id);
U8 IsWaterOutMaxTime(U8 id);

U8 GetWaterOutPercent(void);

//U32 GetFixedAmoutTimeTable(U8 mu8select, U8 mu8AmountType);

void  ControlWaterOut(void);
void ControlWaterOutEol(void);

#endif /* __WATER_OUT_H__ */
