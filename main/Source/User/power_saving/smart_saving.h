/**
 * File : smart_saving.h
 * 
 * Smart Saving, Unused Saving
*/
#ifndef __SMART_SAVING_H__
#define __SMART_SAVING_H__

#include "prj_type.h"

typedef enum _smart_saving_status_
{
    STATUS_GREEN_WEEK = 0,      /// 0 : GREEN WEEK
    STATUS_INTENSIVE_WEEK,      /// 1 : INTENSIVE WEEK
} ESmartSavingStatus_T;
    
typedef struct _smart_saving_
{
    U8 Config;                            /// SmartSaving function use? : TRUE or FALSE
	
    U8 UnusedForWeekNumConfig;            /// Number of unused for 16hour Config
    U8 UnusedForWeekNum;                  /// Number of unused for 16hour
    ESmartSavingStatus_T PrevWeekStatus;  /// Green Week or Intensive Week
    ESmartSavingStatus_T WeekStatus;      /// Green Week or Intensive Week

    /* Timer */
    U16 BaseTimer;                        /// 1hour count
    U32 WeekTimer;                        /// 7days count
    U32 UnusedForWeekTimer;               /// Check 16hour unused time for check the number of unused status -> If the number of unused times is more than 4? or Less than 2 times? YES -> Then week change  
} SSmartSaving_T;

typedef struct _unused_saving_
{
    U8 Config;                            /// UnusedSaving function use? : TRUE or FALSE
    U8 Run;                               /// Current Oparation Status

    U16 OnTime;                           /// Check 9hour unused time for UnusedSave Status
    U16 RunTime;                          /// Oparation Time

    U16 TempStatus;                       /// Ambient Temp Satus ( NOT_SATISFIED, SATISFIED )
    U16 TempSatisfiedTime;                /// SATISFIED DETECT TIME
    U16 TempNotSatisfiedTime;             /// NOT SATISIFED DETECT TIME

    U8  ConfigOnOffMode;                  /// 스마트미사용절전 기능 ON/OFF 할 수 있는 모드
    U16 ConfigOnOffModeTimer;             /// 스마트미사용절전 기능 ON/OFF모드 리미트 타이머
} SUnusedSaving_T;

void InitSmartSaving(void);

void SetUnusedSavingConfig(U8 config);
U8 GetUnusedSavingConfig(void);

U8 GetUnusedSavingStatus(void);

void SetSmartSavingConfig(U8 config);
U8 GetSmartSavingConfig(void);
ESmartSavingStatus_T GetSmartWeekSatus(void);

void ResetSmartUnusedSavingTime(void);

void ProcessSmartSaving(void);

/* 미사용 절전 설정 on/off 모드 */
U8 GetSmartSaveConfigMode(void);
void SetSmartSaveConfigMode(U8 mu8Conf);
U16 GetSmartSaveConfigModeTimer(void);
void ResetSmartSaveConfigModeTimer(void);
void UpdateSmartSaveConfigModeTimer(void);

#endif /* __SMART_SAVING_H__ */