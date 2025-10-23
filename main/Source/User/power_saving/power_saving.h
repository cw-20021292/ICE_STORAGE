#ifndef __POWER_SAVING_H__
#define __POWER_SAVING_H__

#include "prj_type.h"

/* Status */
#define  STATUS_WAKE_UP    0
#define  STATUS_SLEEP      1

/* TIMER */
#define  DEFAULT_SLEEP_TIMER        600U  /* 10min */
#define  DEFAULT_WAKE_UP_TIMER      600U  /* 10min */

typedef struct _power_saving_
{
    U8 Config;          /* ���� ��� ��� ���� : TRUE or FALSE */

    U8 PrevStatus;     
    U8 Status;          /* WAKE_UP or SLEEP */

    U8 Dark2Light;      /* ��ο� ���ٰ� ������� ���, TRUE�� SET�ȴ�. */
    U8 Light2Dark;      /* ��� ���ٰ� ��ο����� ���, TRUE�� SET�ȴ�. */
    
    U8 IsDark;             /* ���� �� ���� */
    U16 AdcLimitSleep;     /* ���� ���� ADC ����ġ */
    U16 AdcLimitWakeUp;    /* ���� ���� ADC ����ġ */
    U16 TimerSleep;        /* ���� ���� Ÿ�̸� @1sec */
    U16 TimerWakeUp;       /* ���� ���� Ÿ�̸� @1sec */
} PowerSaving_T;

void InitSaving(void);
void GetSavingData(PowerSaving_T *pData );

void SetSavingConfig(U8 mu8Val );
U8 GetSavingConfig(void);

U8 GetSavingStatus(void);

// Conf Timer Sleep
U16 GetSavingConfTimerSleep(void);
void SetSavingConfTimerSleep(U16 mu16Timer);

// Conf Timer WakeUp
U16 GetSavingConfTimerWakeUp(void);
void SetSavingConfTimerWakeUp(U16 mu16Timer);

void WakeUpSavingMode(void);

U8 GetSavingStatusDark2Light(void);
U8 GetSavingStatusLight2Dark(void);

void ProcessSaving(void);

#endif /* __POWER_SAVING_H__ */
