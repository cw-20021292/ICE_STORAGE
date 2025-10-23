#ifndef __HEALTH_MAKER_H__
#define __HEALTH_MAKER_H__

#include "prj_type.h"

void InitHealthMaker(void);

/* Health Maker Status */
typedef enum
{
    STATUS_GOING,
    STATUS_DONE,
    STATUS_ERR,
} HMStatus_T;  

/*Drain - ��� */
typedef enum
{
    STEP_DRAIN_READY,      // ��� �غ�
    STEP_DRAIN,            // ���
    STEP_DRAIN_RESIDUAL,
    STEP_DRAIN_DONE,
} DrainStep_T;

typedef struct _do_drain_
{
    DrainStep_T Step;
    U16 WaitTime;         // �ü� �Լ� ��� ���� �ð�
    U16 ResidualTime;     // ���� ��ũ �ܼ� ���� Ÿ�̸�
    U16 ExitTime;         // ��ü ��� ���� ����Ʈ Ÿ�̸�
    U16 StayTime;         // �ü� ������ ���� ���� Ȯ�� �ð�
} DoDrain_T;

// ���
void InitHealthDrain(void);
DrainStep_T GetHealthDrain(void);
void GetHealthDrainData(DoDrain_T *pDrain);
void UpdateHealthDrainTime(void);


typedef struct _plumber_
{
    U8 Run;
    U32 WaitTime;          // ���� ���� ��� �ð� 
    U32 MaxTime;           // ���� �ִ� ���� �ð�
} Plumber_T;

void GetPlumberData( Plumber_T *pPlumber );
void UpdatePlumberWaitTime(void);
void UpdatePlumberMaxTime(void);

HMStatus_T DoHealthDrain(void);

#endif /* __HEALTH_MAKER_H__ */
