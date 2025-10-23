#ifndef __STER_H__
#define __STER_H__

#include "prj_type.h"

typedef enum
{
    STER_GOING_OFF,
    STER_GOING_ON_LV_1, // 살균 수 입수 중 -> 살균 수 배수로 이동
    STER_GOING_ON_LV_2, // 살균 수 배수 완료 -> 헹굼수 입수 단계
    STER_GOING_ON_LV_3  // 헹굼 수 입수 완료 -> 헹굼 수 배수 단계 이동
} SterGoingStatus_T;

typedef struct _sterilize_
{
    U8 Start;        // 살균 동작 ( 0: 중지, 1:시작 )
    U8 Done;         // 살균 동작 완료 (0:NONE, 1:완료)
    U8 ErrStopOut;   // 살균 중 에러 발생 (0: 정상, 1: 추출 중지 )
    SterGoingStatus_T Going;        // 살균 투입 ( 0: 살균 수 없음 , 1: 살균수 투입 )
    U16 Mode;        // 살균 제어 상세 모드

    U8 PeriodMode;   // 주기 변경 모드 ( AUTO, FIXED )
    U8 PeriodDay;    // 주기 ( 3일 또는 5일 )
    U8 PassedDay;    // 마지막 살균일로 부터 경과일

    U32 ConsumWater; //자동 주기 변경 기준 물 사용량(시간@100ms)
} Ster_T;

void InitSter(void);
void GetSterData(Ster_T *pData);

// 살균  시작/중지
void StartSter(void);
void StartSterMode(U16 mu16Mode);
U8 StopSoftSter(void);
U8 StopSterError(void);
U8 IsStartSter(void);

// 상세 제어 모드
#define STER_MODE_NONE         0x0000     
#define STER_MODE_INIT         0x0001     
#define STER_MODE_DRAIN_1ST    0x0002    
#define STER_MODE_START        0x0004
#define STER_MODE_DRAIN_2ND    0x0010
#define STER_MODE_RINSE        0x0020
#define STER_MODE_DRAIN_3TH    0x0040
#define STER_MODE_FEED         0x0080

// 단수 검사, 초기 배수, 
#define STER_MODE_PRE   \
    (STER_MODE_INIT|STER_MODE_DRAIN_1ST|STER_MODE_START)

// 살균수 배수, 헹굼수 입수, 헹굼수 배수, 재정수
#define STER_MODE_AFTER \
    (STER_MODE_DRAIN_2ND|STER_MODE_RINSE|STER_MODE_DRAIN_3TH|STER_MODE_FEED)

// 살균수 배수, 헹굼수 입수, 헹굼수 배수, 재정수
#define STER_MODE_LV_1 \
    (STER_MODE_DRAIN_2ND|STER_MODE_RINSE|STER_MODE_DRAIN_3TH|STER_MODE_FEED)

// 헹굼수 배수, 재정수
#define STER_MODE_LV_2 \
    (STER_MODE_RINSE|STER_MODE_DRAIN_3TH|STER_MODE_FEED)

// 헹굼수 배수, 재정수
#define STER_MODE_LV_3 \
    (STER_MODE_DRAIN_3TH|STER_MODE_FEED)

#define STER_MODE_ALL   \
    (STER_MODE_PRE|STER_MODE_AFTER)
void SetSterMode(U16 mu16Mode);
U16 GetSterMode(void);
void ClearSterMode(U16 mu16Mode);
U16 IsSetSterMode(U16 mu16Mode);

// 살균 완료 상태 확인
U8 SetSterDone(U8 mu8Done);
U8 GetSterDone(void);

// 살균 중 에러 발생
void SetSterErrWaterOut(U8 mu8Done);
U8 GetSterErrWaterOut(void);

// 살균수 입수 
void SetSterGoing(SterGoingStatus_T mGoing);
SterGoingStatus_T GetSterGoing(void);


U8 ResetSter(void);

// 살균 주기 모드 
#define  STER_PERIOD_MODE_AUTO   0     // 자동
#define  STER_PERIOD_MODE_FIXED  1     // 고정(수동)
void SetSterPeriodMode(U8 mu8Mode );
U8 GetSterPeriodMode(void);

// 살균 주기 - 설정일
void SetSterPeriodDay(U8 mu8Val);
U8 GetSterPeriodDay(void);

// 살균 주기 - 살균 경과일
void SetSterPassDay(U8 mu8Val);
U8 GetSterPassDay(void);


//// 살균일 계산
void ProcessPeriodDay(void);

// 살균 동작 제어
void ControlSter(void);

#endif   /* __STER_H__ */
