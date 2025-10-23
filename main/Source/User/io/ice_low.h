/**
 * File : ice_low.h
 * 
 * Ice Low
*/
#ifndef __ICE_LOW_H__
#define __ICE_LOW_H__

#include "prj_type.h"

typedef struct _ice_low_
{
    /* Status */
    U8 Low;          /// 저빙 상태 
    
    /* CHECK */
    U8 Start;        /// 만빙 검사 시작 상태 FLAG
    U8 CheckCount;   /// 검사 시간 내에 기준치 초과 횟수
    U16 CheckTime;   /// 검사 시간

    U8 PowerOnCheck;  /// 전원 RESET 후, 최초 검사 실행 여부
} SIceLow_T;

void InitIceLow(void);

void GetIceLowData(SIceLow_T *pIceLow);

U8 GetIceLowStatus(void);
U8 GetIceLowPowerOnCheck(void);

void StartCheckIceLow(void);
void StopCheckIceLow(void);
U8 IsDoneCheckIceLow(void);

void CheckIceLow(void);

#endif /* __ICE_FULL_H__ */
