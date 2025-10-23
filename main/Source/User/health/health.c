/**
 * File : health.c
 * 
 * Health
*/
#include "health.h"
#include "eeprom.h"

#include "rtc.h"
#include "error.h"
#include "ster.h"
#include "drain_water.h"
#include "water_out.h"
#include "ice_water_out.h"
#include "ice_door.h"
#include "room_water.h"

Health_T Health;

void InitHealth(void)
{
    Health.Mode = HEALTH_MODE_DRAIN;

    Health.Hour = 1;
    Health.Min = 0;
    Health.AdjustMode = 0;
    Health.SetHourConfig = FALSE;
    Health.SetHourMode = HOUR_MODE_AFTER_14H;    

    // 일반 모델..
    Health.ConfTargetTime = NONE_RTC_TARGET_TIME;
    Health.Conf1Hour = NONE_RTC_1HOUR;
    Health.Conf2Hour = NONE_RTC_2HOUR;
    Health.ConfMaxTargetTime = MAX_NONE_RTC_TIME_OUT;
    Health.ConfWeekDays = DEFAULT_UNUSED_WEEK_DAYS;

    Health.UnusedWeekDays = DEFAULT_UNUSED_WEEK_DAYS;
    Health.TargetTimeOut = NONE_RTC_TARGET_TIME; // target 24hour timeout..@1sec
    Health.CurrentTimeOut = 0; // current 24hour timeout..@1sec
}

void GetHealthData(Health_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Health, sizeof( Health_T ));
}

// 헬스 기능 사용 가능 여부 검사
//  -. RTC 오류
//  -. 시간 설정 상태 여부
//  -. 설정 정보 여부
U8 IsConfHealth(void)
{
#if( CONFIG_STER == 1 )
    // RTC 상태 
    if( IsRTC_Error() == TRUE )
    {
        return FALSE;
    }

    // 살균 시간 설정 상태 ( WIFI 또는  제품에서의 설정 )
    if( GetRtcConfigInit() == FALSE )
    {
        return FALSE;
    }

    // 살균 설정 정보 유무
    if( GetSterPeriodMode() == 0xFF 
            || GetHealthHour() == 0xFF
            || GetHealthMin() == 0xFF 
            || GetSterPeriodMode() == 0xFF )
    {
        return FALSE;
    }
#endif

    // 살균 동작 가능 유무
    if( IsValidHealth() == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

// 헬스 기능 동작 가능 여부 검사
U8  IsStartHealth(void)
{
    // 배수 동작은 FULL 타입일 때에만 한다...
    if( IsGoingOnDrainWaterFull() == TRUE )
    {
        return TRUE;
    }
#if 0
    if( IsStartSter() == TRUE )
    {
        return TRUE;
    }
#endif
    return FALSE;
}

/* 위생 모드 */
void  SetHealthMode(U8 mode)
{
#if( CONFIG_STER == 0 )
    mode &= ~HEALTH_MODE_STER;
#endif

    Health.Mode |= mode;
    SaveEepromId( EEP_ID_CONF_STER );
}

void  ClearHealthMode(U8 mode)
{
    Health.Mode &= ~mode;
    SaveEepromId( EEP_ID_CONF_STER );
}

U8 IsSetHealthMode(U8 mode)
{
    if( (Health.Mode & mode) == mode )
    {
        return TRUE;
    }

    return FALSE;
}

U8 GetHealthMode(void)
{
    return Health.Mode;
}

// 예약 시간 (시)
void SetHealthHour(U8 val)
{
    if( val < 24U )
    {
        if( Health.Hour != val )
        {
            SaveEepromId(EEP_ID_STER_RESERVATION_HOUR);
        }
        Health.Hour = val;
    }
}

U8 GetHealthHour(void)
{
    return Health.Hour;
}

// 예약 시간 (분)
void SetHealthMin(U8 val)
{
    if( val == 30U || val == 0U )
    {
        if( Health.Min != val )
        {
            SaveEepromId(EEP_ID_STER_RESERVATION_MIN);
        }
        Health.Min = val;
    }
}

U8 GetHealthMin(void)
{
    return Health.Min;
}

// 예약 시간 보정
void SetHealthAdjustMode(U8 mode)
{
    if( mode > ADJUST_MODE_AFTER )
    {
        return;
    }

    Health.AdjustMode = mode;
    SaveEepromId(EEP_ID_STER_ADJUST_MODE);
}

U8 GetHealthAdjustMode(void)
{
    return Health.AdjustMode;
}

// 예약 시간 보정이 된 시간 반환
U8 GetHealthHourAdjust(void)
{
    U8 hour;
    U8 mode;

    hour = GetHealthHour();
    mode = GetHealthAdjustMode();
    if( mode == ADJUST_MODE_BEFORE )    // -2시간 
    {
        if( hour >= 2 )  
        {
            hour = hour - 2;
        }
        else if( hour == 1 ) 
        {
            hour = 23;
        }
        else if( hour == 0 ) 
        {
            hour = 22;
        }
    }
    else if( mode == ADJUST_MODE_AFTER )   // +2시간 
    {
        /* 살균 시간을 2시간 앞으로 이동 */
        if( hour <= 21 )  
        {
            hour = hour + 2;
        }
        else if( hour == 22 ) 
        {
            hour = 0;
        }
        else if( hour == 23 ) 
        {
            hour = 1;
        }
    }

    return hour;
}


void SetHealthHourConfig(U8 config)
{
    if( config != TRUE && config != FALSE )
    {
        return;
    }

    if( Health.SetHourConfig != config )
    {
        SaveEepromId(EEP_ID_STER_SET_HOUT_CONFIG);
    }
    Health.SetHourConfig = config;
}

U8 GetHealthHourConfig(void)
{
    return Health.SetHourConfig;
}

U8 GetHealthHourMode(void)
{
    return Health.SetHourMode;
}

void SetHealthHourMode(void)
{
    static U8 HourMode = HOUR_MODE_AFTER_NONE;

    HourMode = Health.SetHourMode;
    
    HourMode++;

    if( HourMode >= HOUR_MODE_AFTER_MAX )
    {
        HourMode = HOUR_MODE_AFTER_8H;
    }

    Health.SetHourMode = HourMode;
}

void SetHealthManualHourMode(void)
{
    TimeData_T rtcTime;
    U8 hour = 0;
    U8 min = 0;

    rtcTime.Y2K   = 1;
    rtcTime.Year  = 0;
    rtcTime.Month = 0;
    rtcTime.Date  = 0;
    rtcTime.Hour  = 0;
    rtcTime.Min   = 0;
    rtcTime.Sec   = 0;

    switch(Health.SetHourMode)
    {
        case HOUR_MODE_AFTER_8H :
        	hour = 8;
        	break;

        case HOUR_MODE_AFTER_10H :
        	hour = 10;
        	break;

        case HOUR_MODE_AFTER_12H :
        	hour = 12;
        	break;

        case HOUR_MODE_AFTER_14H :
        	hour = 14;
        	break;

        case HOUR_MODE_AFTER_16H :
        	hour = 16;
        	break;

        case HOUR_MODE_AFTER_18H :
        	hour = 18;
        	break;

        default :
          break;
    }

    //SetSterPassDay( 1 );

    SetRtcTime(&rtcTime);
    SetHealthHour(hour);
    SetHealthMin(min);

    SetRtcConfigInit(TRUE);
}

void ClearHealthManualHourMode(void)
{
/*
    E_WIFI_DISPLAY_T mWifiStatus = 0;
    static E_WIFI_DISPLAY_T premWifiStatus = 0;

    U8 Hour = 0;
    U8 Min = 0;

    mWifiStatus = GetWifiStatusValue(WIFI_STATUS_DISP);

    if( premWifiStatus != mWifiStatus )
    {
        premWifiStatus = mWifiStatus;
    
        if( mWifiStatus == WIFI_DISP_CONNECT )
        {
            if( Health.SetHourConfig == FALSE )
            {
                return;
            }

            SetHealthHour( Hour );
            SetHealthMin( Min );
            Health.SetHourConfig = FALSE;
        }
    }
*/
}

// 사용자 제품 사용중?
// RETURN
//  TRUE -사용 중
//  FALSE - 미사용 중
static U8 IsUserUsing(void)
{
    if( GetWaterOut() == TRUE )
    {
        return TRUE;
    }

    if( IsStartIceDoorWater() == TRUE )
    {
        return TRUE;
    }

    if( IsOpenIceDoor() == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

static void ResetTime(void)
{
    Health.TargetTimeOut  = Health.ConfTargetTime;
    Health.CurrentTimeOut = 0;
}

void SetHealthConfDrainWater(U32 target, U32 hour_1, U32 hour_2, U8 days )
{
    Health.ConfTargetTime = target;
    Health.Conf1Hour = hour_1;
    Health.Conf2Hour = hour_2;
    Health.ConfWeekDays = days;
    Health.ConfMaxTargetTime = (target + hour_1);

    ResetTime();
}

// 위생 동작 조건 검사 
// RETURN
//  - FALSE ; 중지 조건
//  - TRUE :  동작 가능함
U8 IsValidHealth(void)
{
    // 정수 수위가 물넘침이 아닌 경우, 
    if( GetRoomWaterLevel() != LEVEL_LOW 
            && GetRoomWaterLevel() != LEVEL_MID 
            && GetRoomWaterLevel() != LEVEL_HIGH )
    {
        return FALSE;
    }

    // 부하 에러
    if( IsErrorType(ERROR_TYPE_DRAIN) == TRUE )
    {
        return FALSE;
    }

    // 사용 중
    if( IsUserUsing() == TRUE )
    {
        return FALSE;
    }
#if 0
    if( IsStartSter() == TRUE ) 
    {
        return FALSE ;
    }
#endif
    if( IsStartDrainWater() == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 GetAlaramByRTC(U8 action)
{
    static U8 alaram = FALSE; // 알람 시간에 한 번만 동작되도록...
    U8 hour = 0U;
    U8 min = 0U;

    hour = GetHealthHourAdjust();
    min  = GetHealthMin();
    if ( IsAlaramTime(hour, min, 0) == STATUS_ALARAM
              && IsAlaramTime(hour, min + 10, 0) == STATUS_NOT_YET )

    {
        if( alaram == FALSE )
        {
            alaram = TRUE;
            action  = TRUE;
        }
    }
    else
    {
        alaram = FALSE;
        action  = FALSE;
    }

    return action;
}

static void MakeOneHourEarly(void)
{
    if( Health.UnusedWeekDays != 0 )
    {
        Health.UnusedWeekDays--;
    }
    else
    {
        Health.UnusedWeekDays = Health.ConfWeekDays;
        Health.CurrentTimeOut = Health.Conf1Hour;
    }
}

U8 GetAlaramByNoneRTC(U8 action)
{
    U32 remainTime = 0UL;

    remainTime = Health.TargetTimeOut - Health.CurrentTimeOut;

    // 22~24시간 사이에 물 사용량 있으면 7일 카운터를 초기화한다.
    // 초기화 하고, 즉시 한번 감소 되기 때문에 카운터는 8로 설정한다.
    if( remainTime <= Health.Conf2Hour )
    {
        if( IsUserUsing() == TRUE )
        {
            Health.UnusedWeekDays = Health.ConfWeekDays;
        }
    }

    // 23~24시간 사이에 추출 동작이 있으면, +1Hour 추가한다.
    // 단, 1회만 +1 Hour를 더할 수 있다.
    // 잔여 시간 계산.. 1Hour?
    if( remainTime <= Health.Conf1Hour )
    {
        if( IsUserUsing() == TRUE )
        {
            if( Health.TargetTimeOut < Health.ConfMaxTargetTime )
            {
                Health.TargetTimeOut += Health.Conf1Hour;
            }
        }
    }

    // 24시간 계산..
    if( Health.TargetTimeOut > Health.CurrentTimeOut )
    {
        action = FALSE;

        Health.CurrentTimeOut++;
    }
    else
    {
        action = TRUE;

        // 시간 재설정
        ResetTime();

        // 7일 동안 물 사용량이 없으면 다음 배수 시간을 +1시간 당김.
        MakeOneHourEarly();
    }

    return action;
}

// 살균 동작 시간 직전으로 시간 재설정
void UpdateHealthOnTime(void)
{
    TimeData_T time;
    U8 hour;
    U8 min;

    hour = GetHealthHourAdjust();
    min  = GetHealthMin();
    GetRtcTime(&time);

    if( min == 0 )
    {
        if( hour != 0 )
        {
            time.Hour = hour - 1;
        }
        else
        {
            time.Hour = 23;
        }

        time.Min = 59;
    }
    else
    {
        time.Hour = hour;
        time.Min = 29;
    }

    time.Sec = 55;

    SetRtcTime(&time);
}

void UpdateHealthOnTime_2(void)
{
    Health.CurrentTimeOut = Health.TargetTimeOut - 5UL;
}

void UpdateHealthOnTimeOneHour(void)
{
    Health.CurrentTimeOut = Health.TargetTimeOut - (NONE_RTC_1HOUR);
}

void UpdateHealthOnTimeTwoHour(void)
{
    Health.CurrentTimeOut = Health.TargetTimeOut - (NONE_RTC_2HOUR);
}

void UpdateHealthOnTimeWeek(void)
{
    Health.UnusedWeekDays = 0;
}

// 제어 
void ControlHealth(void)
{
    static U8 checkAction = FALSE; 

#if( CONFIG_STER == 1 )
    ClearHealthManualHourMode();

    // RTC 에러 
    if( IsRTC_Error() == TRUE 
            && GetRtcConfigInit() == FALSE
            && GetHealthMode() != HEALTH_MODE_NONE )
    {
        ClearHealthMode(HEALTH_MODE_ALL);
        return;
    }
#endif
    checkAction = GetAlaramByNoneRTC(checkAction);

    // On/Off
    if( Health.Mode == HEALTH_MODE_NONE )
    {
        checkAction = FALSE;
        return;
    }

    // Check conditions
    if( IsValidHealth() == FALSE )
    {
        return;
    }

    // Check reservation time
#if( CONFIG_STER == 1 )
    checkAction = GetAlaramByRTC(checkAction);
#endif

    // Health Mode 제어
    if( checkAction == TRUE )
    {
        if( IsSetHealthMode(HEALTH_MODE_STER) == TRUE )
        {
            // 살균 동작!!!
            //StartSter();
        }

        // 살균 동작이 없으면 배수 동작!!!
        // 순환 배수 사용 설정이 되어 있는 경우, 배수 시작
        if( //IsStartSter() == FALSE 
                IsSetHealthMode(HEALTH_MODE_DRAIN) == TRUE )
        {
            StartDrainWater(HALF_DRAIN);
        }

        if( IsStartDrainWater() == TRUE )
        {
            checkAction = FALSE;
        }
    }
}
