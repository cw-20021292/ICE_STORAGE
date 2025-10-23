/**
 * File : health.h
 * 
 * Health
*/
#ifndef __HEALTH_H__
#define __HEALTH_H__

#include "prj_type.h"

typedef struct _health_
{
    U8 Mode;        // ��� ( ���, ��� )

    U8 Hour;        // ���� �ð�(��, 0 ~ 23 )
    U8 Min;         // ���� �ð�(��, 0, 30 )
    U8 AdjustMode;  // ���� ���� ��� ( ADJUST_MODE_NONE, BEFORE, AFTER )
    U8 SetHourConfig; // ���� �ð� ���� ��� Config ( WIFI�� ����Ǿ��� �� ���� �ð��� 0�÷� �ʱ�ȭ �ϴ� FLAG )
    U8 SetHourMode;   // ���� �ð� ���� ��� ( HOUR_MODE_AFTER_NONE,  8H, 10H, 12H, 14H, 16H, 18H )

    U32 ConfTargetTime;
    U32 Conf1Hour;
    U32 Conf2Hour;
    U32 ConfMaxTargetTime;
    U8  ConfWeekDays;

    U8  UnusedWeekDays;  // �Ϲ� ��, 7�� ī����
    U32 TargetTimeOut;   // �Ϲݸ� ��� ��ǥ �ð�  (24hour)
    U32 CurrentTimeOut;  // �Ϲݸ� ��� ���� �ð�
} Health_T;

void InitHealth(void);
void GetHealthData(Health_T *pData);

// �ｺ ��� ���� ���� ����
// Return : 
// TRUE - ���� ����, FALSE - ���� �Ұ�
U8 IsConfHealth(void);

// �ｺ ��� ���� ������ �˻� 
// Return :
// TRUE - ���� ��, FALSE - ���� ��
U8 IsStartHealth(void);

#define HEALTH_MODE_NONE     (0x00)  // ����
#define HEALTH_MODE_STER     (0x01)  // ���
#define HEALTH_MODE_DRAIN    (0x02)  // ��� 
#define HEALTH_MODE_ALL      (HEALTH_MODE_STER | HEALTH_MODE_DRAIN)
void SetHealthMode(U8 mode);
void ClearHealthMode(U8 mode);
U8 IsSetHealthMode(U8 mode);
U8 GetHealthMode(void);

// ���� �ð� (��)
void SetHealthHour(U8 val);
U8 GetHealthHour(void);

// ���� �ð� (��)
void SetHealthMin(U8 val);
U8 GetHealthMin(void);

// ���� �ð� ����
#define ADJUST_MODE_NONE     (0)   
#define ADJUST_MODE_BEFORE   (1)   
#define ADJUST_MODE_AFTER    (2)
U8 GetHealthAdjustMode(void);
void SetHealthAdjustMode(U8 mode);
U8 GetHealthHourAdjust(void);

void SetHealthHourConfig(U8 config);
U8 GetHealthHourConfig(void);

typedef enum _hour_mode_
{
    HOUR_MODE_AFTER_NONE = 0,
    HOUR_MODE_AFTER_8H,
    HOUR_MODE_AFTER_10H,
    HOUR_MODE_AFTER_12H,
    HOUR_MODE_AFTER_14H,
    HOUR_MODE_AFTER_16H,
    HOUR_MODE_AFTER_18H,
    HOUR_MODE_AFTER_MAX
} SHourMode_T;
U8 GetHealthHourMode(void);
void SetHealthHourMode(void);
void SetHealthManualHourMode(void);

// 24hour = 24h x 60min x 60sec = 86400sec 
#define NONE_RTC_TARGET_TIME      (86400UL)
// 1hour = 1h x 60min x 60sec = 
#define NONE_RTC_1HOUR            (3600UL)
#define NONE_RTC_2HOUR            (7200UL)
#define MAX_NONE_RTC_TIME_OUT     (NONE_RTC_TARGET_TIME + NONE_RTC_1HOUR)
#define DEFAULT_UNUSED_WEEK_DAYS  (7U)
void SetHealthConfDrainWater(U32 target, U32 hour_1, U32 hour_2, U8 days);

U8 IsValidHealth(void);

U8 GetAlaramByNoneRTC(U8 action);

// Test
void UpdateHealthOnTime(void);
void UpdateHealthOnTime_2(void);
void UpdateHealthOnTimeOneHour(void);
void UpdateHealthOnTimeTwoHour(void);
void UpdateHealthOnTimeWeek(void);

// ����
void ControlHealth(void);

#endif /* __HEALTH_H__ */
