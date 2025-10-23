/**
 * File : ice_full.h
 * 
 * Ice Full
*/
#ifndef __ICE_FULL_H__
#define __ICE_FULL_H__

#include "prj_type.h"

typedef struct _ice_full_
{
    /* Status */
    U8 Full;           /// 만빙 상태 
    U8 WeakFull;      /// Weak 만빙 상태
    
    U8 FullCheckCount; /// 만빙 체크 카운트 ( ICE Make 조건일 때만 횟수 체크 아니면 즉시 판단 )
    
    /* CHECK */
    U8 Start;        /// 만빙 검사 시작 상태 FLAG
    U8 CheckCount;   /// 검사 시간 내에 기준치 초과 횟수
    U16 CheckTime;   /// 검사 시간

    U8 PowerOnCheck;  /// 전원 RESET 후, 최초 검사 실행 여부
} SIceFull_T;

void InitIceFull(void);

void GetIceFullData(SIceFull_T *pIceFull);

U8 GetIceFullStatus(void);

void SetIceFullCheckCount(void);
void UpdateIceFullCheckCount(void);

U8 GetIceFullPowerOnCheck(void);

void StartCheckIceFull(void);
void StopCheckIceFull(void);
U8 IsDoneCheckIceFull(void);

void CheckIceFull(void);

#endif /* __ICE_FULL_H__ */
