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

/*Drain - 배수 */
typedef enum
{
    STEP_DRAIN_READY,      // 배수 준비
    STEP_DRAIN,            // 배수
    STEP_DRAIN_RESIDUAL,
    STEP_DRAIN_DONE,
} DrainStep_T;

typedef struct _do_drain_
{
    DrainStep_T Step;
    U16 WaitTime;         // 냉수 입수 밸브 지연 시간
    U16 ResidualTime;     // 정수 탱크 잔수 제거 타이머
    U16 ExitTime;         // 전체 배수 동작 리미트 타이머
    U16 StayTime;         // 냉수 저수위 센서 감지 확정 시간
} DoDrain_T;

// 배수
void InitHealthDrain(void);
DrainStep_T GetHealthDrain(void);
void GetHealthDrainData(DoDrain_T *pDrain);
void UpdateHealthDrainTime(void);


typedef struct _plumber_
{
    U8 Run;
    U32 WaitTime;          // 펌프 동작 대기 시간 
    U32 MaxTime;           // 펌프 최대 동작 시간
} Plumber_T;

void GetPlumberData( Plumber_T *pPlumber );
void UpdatePlumberWaitTime(void);
void UpdatePlumberMaxTime(void);

HMStatus_T DoHealthDrain(void);

#endif /* __HEALTH_MAKER_H__ */
