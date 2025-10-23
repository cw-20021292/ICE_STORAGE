/**
 * File : ice.c
 * 
 * Ice Control
*/
#ifndef __ICE_H__
#define __ICE_H__

#include "prj_type.h"

typedef struct _ice_
{
    U8 Turbo;     // ��� ���� ���� ( 0: ����, 1: ���� )

    /* MAKE */
    U8 ConfigMake;   // ���� ���� ���� ( 0: ����, 1:���� )
    U8 Make;         // ���� ���� ( 0: ����, 1: ���� )

    /* MODE */
    U16 Mode;         // ���� �� ���� ���

    /* ICE FULL */
    U16 FullCheckTimer; // ���� �˻� �ֱ� Ÿ�̸� @1sec
    U16 StopMakeTimer;  // ���� ���� Ÿ�̸� @1sec

    U16 OutageTimer;    // �ܼ� Ÿ�̸�@1sec

    U8 DummyDeIcing;    // ���� Ż�� ���� ����( 0: ����, 1:���� )

    U16 DeIcingCompTime;    // ���� Ż�� ���� �� �ּ� ����� ���� �ð�

    U8 IcingCount;          // ����Ƚ�� ī��Ʈ
} SIce_T;

void InitIce(void);

void GetIceData(SIce_T *pIce);

void SetIceTurbo(U8 val);
U8 GetIceTurbo(void);

void SetIceConfigMake(U8 val);
U8 GetIceConfigMake(void);

void SetIceMake(U8 val);
U8 GetIceMake(void);

// ���� ���� �µ� �˻� 
U8 IsValidDoIcingAmbiTempRange(void);
U8 IsValidDoIcingColdTempRange(void);

/* MODE */
#define  ICE_MODE_NONE                (0x0000)
#define  ICE_MODE_INIT                (0x0001)
#define  ICE_MODE_PREHEAT             (0x0002)
#define  ICE_MODE_READY               (0x0004)
#define  ICE_MODE_ICING               (0x0008)
#define  ICE_MODE_DEICING             (0x0010)
#define  ICE_MODE_DUMMY_DEICING       (0x0020)
#define  ICE_MODE_OFF                 (0x0040)
#define  ICE_MODE_OFF_LOAD            (0x0080)
#define  ICE_MODE_DEICING_OFF         (0x0100)
#define  ICE_MODE_ICING_MOVE_DEICING  (0x0200)
#define  ICE_MODE_STORAGE             (0x0400)
#define  ICE_MODE_DEL_HUMIDITY        (0x0800)
#define  ICE_MODE_DEFROST             (0x1000)

#define  ICE_MODE_ALL   \
    ( ICE_MODE_INIT | \
      ICE_MODE_PREHEAT | \
      ICE_MODE_READY | \
      ICE_MODE_ICING | \
      ICE_MODE_DEICING | \
      ICE_MODE_ICING_MOVE_DEICING)
void SetIceMakeMode(U16 mode);
void ClearIceMakeMode(U16 mode);
U16 IsSetIceMakeMode(U16 mode);
U16 GetIceMakeMode(void );

// ���� �˻� �ֱ� �ð� ����/������Ʈ
void SetIceFullCheckTimer(U16 val);

// ���� Ż�� ���� ���� ����/��ȯ
U8 GetIceDummyDeIcing( void );
void SetIceDummyDeIcing(U8 val);

// ���� Ż���� ���� �ּ� ����� ���� �ð�
U16 GetDeIcingCompOnTime(void);
void ResetDeIcingCompOnTime(void);
void UpdateDeIcingCompOnTime(void);

void UpdateIcingCount(void);
void ClearIcingCount(void);
U8 IsMaxIcingCount(void);

void StartIceOutageTimer(void);


void SetIceTrayMovingRetry(void);
void StopIceMakeLoad(void);
void ClearIcingMoveDeicing(void);
// ���� Ż�� 
void SetDummyDeicing(void);

// ���� ����
U8 StopIceMake(void);

void  MakeIceFull(void);
void  MakeIce(void);

#endif /* __ICE_H__ */
