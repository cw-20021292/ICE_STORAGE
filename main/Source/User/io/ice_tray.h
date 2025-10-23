#ifndef __ICE_TRAY_H__
#define __ICE_TRAY_H__

#include "prj_type.h"

typedef struct _ice_tray_
{
    U8 Status;    // MOVING / ICING / DEICING
    U8 Mode;      // INIT(0x01) / DEICING(0x02) / ICING(0x04)
    U8 ErrorMoving;        // FALSE(Normal) or TRUE(Error)
    U8 ErrorMovingDeicing; // FALSE(Normal) or TRUE(Error)
    U8 ErrorMicroSwitch;   // FALSE(Normal) or TRUE(Error)
    U8 ErrorIcingCount;    // Error Repeat Count
    U8 ErrorDeIcingCount;  // Error Repeat Count
    U8 ErrorDeIcingAccumCount;  // Error Repeat Accumulative Count

    /* Step */
    U8 IcingStep;
    U8 DeIcingStep;

    /* TIME */
    U32 StartTime;      
    U32 MovingTime;    

    /* POSITION */
    U8 Posi_Icing;
    U8 Posi_DeIcing;
    U16 DeIcingDetectWaitTime;
    U16 IcingDetectWaitTime;
    U16 CheckTimer;     // ���� ��ġ�� ���� ���̵� �ϱ� ���� Ÿ�̸�

    /* Recovery Reversing */
    U8 RR_Count;
    U32 RR_WaitTime;

    /* Recovery Coil Freezing */
    U8 RCF_Cmd;
    U32 RCF_Time;
    U8 RCF_CmdCount;

    /* Recovery Faild Deicing */
    U32 RFD_WaitTime;

} IceTray_T;

void InitIceTray(void);
void GetIceTrayData( IceTray_T *pData );

// ���̽� Ʈ���� ���� ���� ���� ��?
// 0-����, 1-���� ��ġ ����, 2-�p�� ��ġ �̵� ����
U8 IsRetryIceTray(void);


// ���̽� Ʈ���� ���� ���� ����.
// Ż�� ��ġ �̵� �� ���� �߻� ������ ��� TRUE ��ȯ
U8 IsErrorIceTrayCoilFrezzing(void);

// Ʈ���� �̵� ���� ���� ī��Ʈ �ʱ�ȭ
void ResetErrorIceTrayAccumCount(void );

#define  ICE_TRAY_MOVING    0
#define  ICE_TRAY_ICING     1
#define  ICE_TRAY_DEICING   2
U8 GetIceTrayStatus(void);

U8 GetIceTrayErrorMoving(void);
U8 GetIceTrayErrorMovingDeicing(void);
U8 GetIceTrayErrorMicroSwitch(void);

void MoveIceTrayIcingEol(void);
void MoveIceTrayIcing(void);
void MoveIceTrayDeIcing(void);

void ControlIceTray(void);

#endif /* __ICE_TRAY_H__ */
