/**
 * File : ice_water_out.h
 * 
 * Ice Water Out
*/
#ifndef __ICE_WATER_OUT_H__
#define __ICE_WATER_OUT_H__

#include "prj_type.h"

void  InitIceWaterOut(void);

/* ���� */
#define SEL_ICE            (0)
#define SEL_ICE_WATER      (1)
#define MAX_ICE_WATER_ID   (2)
void SetIceWaterSelect(U8 id);
U8 GetIceWaterSelect(void);
U8 GetIceWaterLastedSelect(void);

void UpdateIceWaterOutReturnTime(void);
void ClearIceWaterOutReturnTime(void);
U8 IsExpiredIceWaterOutReturnTime(void);

/* ���� ����/���� */
void StartIceOut(void);
void StopIceOut(void);
U8 GetIceOut(void);

/* ����/���� */
void StartIceWaterOut(void);
void StopIceWaterOut(void);
U8 GetIceWaterOut(void);

/* ���� ���� */
void SetIceWaterOutContinue(U8 mode);
U8 GetIceWaterOutContinue(void);

#define FIXED_ICE_WATER_AMOUNT_500ML       (0)  /// 500mL ( ���� : 135g + �� : 350mL )
#define FIXED_ICE_WATER_AMOUNT_700ML       (1)  /// 700mL ( ���� : 180g + �� : 500mL )
#define FIXED_ICE_WATER_AMOUNT_1L          (2)  /// 1L ( ���� : 270g + �� : 700mL )
#define FIXED_ICE_WATER_AMOUNT_INFINITY    (3)
#define MAX_FIXED_ICE_WATER_AMOUNT_NUM     (4)  
void SetIceWaterOutFixedAmountType(U8 type);
U8 GetIceWaterOutFixedAmountType(void);
U8 GetIceWaterOutAmoutType(void);
void ClearIceWaterOutInfinityAmout(void);

void SetIceWaterOutFixedAmoutTargetTime(U32 time);
U32 GetIceWaterOutFixedAmoutTargetTime(void);

void SetIceWaterOutFixedAmoutCurrentTime(U32 time);
U32 GetIceWaterOutFixedAmoutCurrentTime(void);

/* ���� �ð� (������, ����, ����) */
void SetIceWaterOutLastedTime(U32 time);
U32 GetIceWaterOutLastedTime(void);

void SetIceWaterOutDailyTime(U32 time);
U32 GetIceWaterOutDailyTime(void);

void SetIceWaterOutTotalTime(U32 time);
U32 GetIceWaterOutTotalTime(void);

#define  TYPE_ICE_WATER_LASTED  (0x01)
#define  TYPE_ICE_WATER_DAILY   (0x02)
#define  TYPE_ICE_WATER_TOTAL   (0x04)
#define  TYPE_ICE_WATER_ALL     (0x07)
void SetIceWaterOutTime(U8 type, U32 time);
U32  GetIceWaterOutTime(U8 type);
void ResetIceWaterOutTime(U8 type);

U32  GetIceWaterOutTotalTimeAll(void);

/// ���� �ð��� �� ������� ��ȯ
U8 GetIceWaterOutLastedCupNum(void);    
U8 GetIceWaterOutDailyCupNum(void);    
U8 GetIceWaterOutTotalCupNum(void);    

void SetIceWaterOutAccumulateTime(U32 time);
void ClearIceWaterOutAccumulateTime(void);
U32 GetIceWaterOutAccumulateTime(void);
U8 IsIceWaterOutMaxTime(void);

U8 GetIceWaterOutPercent(void);
U8 GetIceWaterOutAmountPercent(void);

void RegisterOutageIceWaterOut(void);
void UnRegisterOutageIceWaterOut(void);

//U32 GetFixedAmoutTimeTable(U8 mu8select, U8 mu8AmountType);

void  ControlIceWaterOut(void);
void ControlIceWaterOutEol(void);

#endif  /* __ICE_WATER_OUT_H__ */