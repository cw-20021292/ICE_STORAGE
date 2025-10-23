/**
 * File : smart_saving.c
 * 
 * Smart Saving, Unused Saving
*/
#include "smart_saving.h"

#include "temp.h"
#include "error.h"

/// Smart Saving
#define SMART_CHECK_BASE_TIME     (3600U)    /// 1hour * 60min * 60sec @1sec
#define	SMART_CHECK_TIME_7DAYS    (168UL)    /// 7days * 24hour @1hour 
#define SMART_CHECK_TIME_16HOUR   (16UL)     /// 16hour @1hour
#define STANDARD_INTENSIVE_WEEK   (2U)       /// Week Unused Num For Intensive
#define STANDARD_GREEN_WEEK       (4U)       /// Week Unused Num For Green

/// Unused Saving
#define UNUSED_TIME_18HOUR        (64800UL)  /// 18hour * 60min * 60sec @1sec
#define UNUSED_TIME_12HOUR        (43200UL)  /// 12hour * 60min * 60sec @1sec
#define UNUSED_TIME_10HOUR        (36000UL)  /// 10hour * 60min * 60sec @1sec
#define UNUSED_TIME_9HOUR         (32400UL)  /// 9hour * 60min * 60sec @1sec
#define UNUSED_TIME_6HOUR         (21600UL)  /// 6hour * 60min * 60sec @1sec
#define UNUSED_TIME               (UNUSED_TIME_9HOUR)

#define AMBI_DETECT_TIME          (5U)       /// 5sec @1sec
#define AMBI_TEMP_NOT_SATISFIED   (0U)
#define AMBI_TEMP_SATISFIED       (1U)

#define AMBI_UNDER_LIMIT_TEMP     (20.0f)
#define AMBI_OVER_LIMIT_TEMP      (33.0f)

SSmartSaving_T  SmartSaving;
SUnusedSaving_T UnusedSaving;

static void InitUnusedSaving(void)
{
    UnusedSaving.Config = TRUE;
    UnusedSaving.Run = FALSE;

    UnusedSaving.OnTime = UNUSED_TIME;
    UnusedSaving.RunTime = 0U;

    UnusedSaving.TempStatus = AMBI_TEMP_SATISFIED;
    UnusedSaving.TempSatisfiedTime = AMBI_DETECT_TIME;
    UnusedSaving.TempNotSatisfiedTime = AMBI_DETECT_TIME;

#if( (CONFIG_TEST_8585 == 1) || (CONFIG_TEST_8585_ICE_WATER == 1) || (CONFIG_TEST_8585_FEEDER == 1) || (CONFIG_TEST_8585_BACK_FEEDER == 1) || (CONFIG_TEST_8585_HOT_REPEAT == 1) )
     UnusedSaving.Config = FALSE;
#endif
}

void InitSmartSaving(void)
{
    SmartSaving.Config = TRUE;
    
    SmartSaving.UnusedForWeekNumConfig = FALSE;
    SmartSaving.UnusedForWeekNum = 0U;
    
    SmartSaving.PrevWeekStatus = STATUS_GREEN_WEEK;
    SmartSaving.WeekStatus = STATUS_GREEN_WEEK;

    SmartSaving.BaseTimer = SMART_CHECK_BASE_TIME;
    SmartSaving.WeekTimer = SMART_CHECK_TIME_7DAYS;
    SmartSaving.UnusedForWeekTimer = SMART_CHECK_TIME_16HOUR;

    InitUnusedSaving();
}

void SetUnusedSavingConfig(U8 config)
{
    UnusedSaving.Config = config;
}

U8 GetUnusedSavingConfig(void)
{
    return UnusedSaving.Config;
}

U8 GetUnusedSavingStatus(void)
{ 
    return UnusedSaving.Run;
}

void SetSmartSavingConfig(U8 config)
{ 
    SmartSaving.Config = config;
}

U8 GetSmartSavingConfig(void)
{ 
    return SmartSaving.Config;
}

ESmartSavingStatus_T GetSmartWeekSatus(void)
{
    return SmartSaving.WeekStatus;
}

static void ResetUnusedSavingTime(void)
{
    UnusedSaving.Run = FALSE; 
   
    UnusedSaving.OnTime = UNUSED_TIME;
    UnusedSaving.RunTime = 0U;
}

static void ResetSmartSavingTime(void)
{
    SmartSaving.UnusedForWeekNumConfig = FALSE;
    SmartSaving.UnusedForWeekNum = 0U;
    
    SmartSaving.PrevWeekStatus = STATUS_GREEN_WEEK;
    SmartSaving.WeekStatus = STATUS_GREEN_WEEK;

    SmartSaving.BaseTimer = SMART_CHECK_BASE_TIME;
    SmartSaving.WeekTimer = SMART_CHECK_TIME_7DAYS;
    SmartSaving.UnusedForWeekTimer = SMART_CHECK_TIME_16HOUR;
}

void ResetSmartUnusedSavingTime(void)
{   
    /// KEY PRESS RESET
    ResetUnusedSavingTime();

    SmartSaving.UnusedForWeekNumConfig = TRUE;
    SmartSaving.UnusedForWeekTimer = SMART_CHECK_TIME_16HOUR;
}

static void ResetSmartUnusedSavingAll(void)
{   
    SmartSaving.WeekTimer = SMART_CHECK_TIME_7DAYS;
    SmartSaving.UnusedForWeekNum = 0;

    ResetSmartUnusedSavingTime();
}

static void UpdateSmartCheckTimer(void)
{   
    if( SmartSaving.BaseTimer != 0 )
    {
        SmartSaving.BaseTimer--;
        return;
    }

    SmartSaving.BaseTimer = SMART_CHECK_BASE_TIME;

    if( SmartSaving.WeekTimer != 0 )
    {
        SmartSaving.WeekTimer--;
    }

    if( SmartSaving.UnusedForWeekTimer != 0 )
    {
        SmartSaving.UnusedForWeekTimer--;
    }
}

/* 미사용 절전 설정 on/off 모드 */
U8 GetSmartSaveConfigMode(void)
{
    return UnusedSaving.ConfigOnOffMode;
}

void SetSmartSaveConfigMode(U8 mu8Conf)
{
    UnusedSaving.ConfigOnOffMode = mu8Conf;
    if ( mu8Conf == TRUE )
    {
        UnusedSaving.ConfigOnOffModeTimer = 100;  // 10sec..
    }
}

U16 GetSmartSaveConfigModeTimer(void)
{
    return UnusedSaving.ConfigOnOffModeTimer;
}

void ResetSmartSaveConfigModeTimer(void)
{
    UnusedSaving.ConfigOnOffModeTimer = 100;	// 10sec..
}

void UpdateSmartSaveConfigModeTimer(void)
{
    UnusedSaving.ConfigOnOffModeTimer--;
}

static void CheckAmbiTempRange(void)
{
    TEMP_T tempAmbi = 0.0f;

    tempAmbi = GetTemp(TEMP_ID_AMBIENT_2);

    /// 외기 온도 변화 시간 지연 감지
    if( (tempAmbi >= AMBI_UNDER_LIMIT_TEMP)
         && (tempAmbi <= AMBI_OVER_LIMIT_TEMP) )
    {
        if( UnusedSaving.TempSatisfiedTime != 0U )
        {
            UnusedSaving.TempSatisfiedTime--;
        }
        UnusedSaving.TempNotSatisfiedTime = AMBI_DETECT_TIME;
    }
    else
    {
        if( UnusedSaving.TempNotSatisfiedTime != 0U )
        {
            UnusedSaving.TempNotSatisfiedTime--;
        }
        UnusedSaving.TempSatisfiedTime = AMBI_DETECT_TIME;
    }

    
    /// 외기 온도 기준 결정
    if( UnusedSaving.TempSatisfiedTime == 0U )
    {
        UnusedSaving.TempStatus = AMBI_TEMP_SATISFIED;
    }
    else if( UnusedSaving.TempNotSatisfiedTime == 0U )
    {
        UnusedSaving.TempStatus = AMBI_TEMP_NOT_SATISFIED;
    }
}

static U8 IsValidUnusedSaving(void)
{
    if( UnusedSaving.Config == FALSE )
    {
        return FALSE;
    }

    if( SmartSaving.WeekStatus == STATUS_INTENSIVE_WEEK )
    {
        return FALSE;
    }

    if( IsError(ERR_ID_TEMP_AMBIENT_2) == TRUE )
    {
        return FALSE;
    }

    return TRUE;    /// Yes, valid
}

static void ControlSmartSaving(void)
{
    UpdateSmartCheckTimer();

    if( SmartSaving.Config == FALSE )
    {
        ResetSmartSavingTime();
        return;
    }

    /* Update Week Status */
    if( SmartSaving.WeekTimer == 0 )
    {
        SmartSaving.PrevWeekStatus = SmartSaving.WeekStatus;
		
        if( SmartSaving.UnusedForWeekNum <= STANDARD_INTENSIVE_WEEK )
        {
            SmartSaving.WeekStatus = STATUS_INTENSIVE_WEEK;
        }

        if( SmartSaving.UnusedForWeekNum >= STANDARD_GREEN_WEEK )
        {
            SmartSaving.WeekStatus = STATUS_GREEN_WEEK;
        }

        ResetSmartUnusedSavingAll();
    }

    /* Update Number of UnusedSaving for Week */
    if(SmartSaving.UnusedForWeekNumConfig == TRUE)
    {
        if( SmartSaving.UnusedForWeekTimer == 0 )
        {
            SmartSaving.UnusedForWeekNum++;
            SmartSaving.UnusedForWeekNumConfig = FALSE;
        }
    }
}

static void ControlUnusedSaving(void)
{
    CheckAmbiTempRange();

    if( IsValidUnusedSaving() == FALSE )
    {
        ResetUnusedSavingTime();
        return;
    }

    if( UnusedSaving.Run == TRUE )
    {
        if( UnusedSaving.RunTime < 6000U )
        {
            UnusedSaving.RunTime++;
        }
    
        if( UnusedSaving.TempStatus == AMBI_TEMP_NOT_SATISFIED )
        {
            ResetUnusedSavingTime();
            return;
        }
    }

    if( UnusedSaving.OnTime != 0U )
    {
        UnusedSaving.OnTime--;
        return;
    }        

    if( UnusedSaving.TempStatus == AMBI_TEMP_SATISFIED )
    {
        UnusedSaving.Run = TRUE;
    }
}

void ProcessSmartSaving(void)
{
    ControlSmartSaving();
    ControlUnusedSaving();
}
