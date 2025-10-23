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
    U8 Full;           /// ���� ���� 
    U8 WeakFull;      /// Weak ���� ����
    
    U8 FullCheckCount; /// ���� üũ ī��Ʈ ( ICE Make ������ ���� Ƚ�� üũ �ƴϸ� ��� �Ǵ� )
    
    /* CHECK */
    U8 Start;        /// ���� �˻� ���� ���� FLAG
    U8 CheckCount;   /// �˻� �ð� ���� ����ġ �ʰ� Ƚ��
    U16 CheckTime;   /// �˻� �ð�

    U8 PowerOnCheck;  /// ���� RESET ��, ���� �˻� ���� ����
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
