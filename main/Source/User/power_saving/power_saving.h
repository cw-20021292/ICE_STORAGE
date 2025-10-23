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
    U8 Config;          /* 절전 기능 사용 여부 : TRUE or FALSE */

    U8 PrevStatus;     
    U8 Status;          /* WAKE_UP or SLEEP */

    U8 Dark2Light;      /* 어두워 졌다가 밝아지는 경우, TRUE로 SET된다. */
    U8 Light2Dark;      /* 밝아 졌다가 어두워지는 경우, TRUE로 SET된다. */
    
    U8 IsDark;             /* 현재 빛 상태 */
    U16 AdcLimitSleep;     /* 절전 진입 ADC 기준치 */
    U16 AdcLimitWakeUp;    /* 절전 해제 ADC 기준치 */
    U16 TimerSleep;        /* 절전 진입 타이머 @1sec */
    U16 TimerWakeUp;       /* 절전 해제 타이머 @1sec */
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
