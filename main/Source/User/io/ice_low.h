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
    U8 Low;          /// ���� ���� 
    
    /* CHECK */
    U8 Start;        /// ���� �˻� ���� ���� FLAG
    U8 CheckCount;   /// �˻� �ð� ���� ����ġ �ʰ� Ƚ��
    U16 CheckTime;   /// �˻� �ð�

    U8 PowerOnCheck;  /// ���� RESET ��, ���� �˻� ���� ����
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
