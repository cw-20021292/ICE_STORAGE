#include "time_short.h"

#include "comp.h"
#include "level.h"
#include "power_saving.h"
#include "hot_water.h"
#include "ice_door.h"
#include "service.h"
#include "water_out.h"
#include "room_water.h"
#include "cold_water.h"
#include "ice_maker.h"
#include "health.h"
#include "err_room_level.h"
#include "timer.h"
#include "process_display.h"


/* 1. 압축기 대기 시간 0sec
 * 2. 수위 센서 감지 시간 1sec
 * 3. 조도 감지 지연 시간 1sec
 * 4. 키 버튼 설정 시간 1sec
 * 5. 연속 추출 시간 10min
 * 6. 미사용 절전 시간 60sec
 * 7. 아이스 도어 강제 close 주기 60sec
 * 8. 서비스 점검 시간 60sec
 */

#define  DEFAULT_LIMIT_TIME_VALUE      30U    /* 30sec, @1sec */
#define  DEFAULT_TEST_TIME_VALUE       7200U   /* 2Hour, 60sec x 60min = 3600, @1sec */


typedef struct _time_short_
{
    U8 Status;          /* START or STOP */
    U16 LimitTimer;     /* Enter FCT Mode Timeout */
    U16 TestTimer;      /* Interval Full Test Timer */
} TimeShort_T;

TimeShort_T ts;


void InitTimeShort(void)
{
    ts.Status       = FALSE; 
    ts.LimitTimer   = DEFAULT_LIMIT_TIME_VALUE;
    ts.TestTimer    = DEFAULT_TEST_TIME_VALUE;
}

void StartTimeShort(void)
{
    ts.Status = TRUE;
}

void StopTimeShort(void)
{
    ts.Status = FALSE;
}

U8 IsExpiredTimeShortLimitTimer(void)
{
    if( ts.LimitTimer != 0 )
    {
        return FALSE;   // no expired
    }
    return TRUE;  // expired!!
}

U8 GetTimeShortStatus(void)
{
    return ts.Status;
}

void SetTimeShortTestTimer(U16 mu16Time)
{
    ts.TestTimer = mu16Time;
}

U16 GetTimeShortTestTimer(void)
{
    return ts.TestTimer;
}


void UpdateTimeShortTimer(void)
{
    if( ts.LimitTimer != 0 )
    {
        ts.LimitTimer--;
    }
    if( ts.TestTimer != 0 )
    {
        ts.TestTimer--;
    }
}

static void Start(void)
{
    //1. 수위 센서 감지 시간 1sec
    SetTankConfDetectTime( LEVEL_ID_ROOM ,    10 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_COLD,     10 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_HOT,      10 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_DRAIN,    10 );   // @100ms
    SetTankConfDetectTime( LEVEL_ID_ICE_TRAY, 10 );   // @100ms

    //2. 정수 수위 센서 에러 카운트 최대값 설정
    SetRoomErrCountLimit( 1 );

    //3. 더미 탈빙 Skip
    StopDummyDeIcing();

    //4. 압축기 대기 시간 5sec
    SetCompProtectOffTime( 5 );  

#if !CONFIG_STER
    // 10min, 10min x 60sec = 600
    // 1min, 1min x 60sec = 60
    SetHealthConfDrainWater( 600UL, 60UL, 120UL, 3 );
#endif
}

static void Running(void)
{
    // 조도 감지 센서 시간 설정 @ 1sec
    if( GetSavingConfTimerSleep() > 1 )
    {
        SetSavingConfTimerSleep( 1 );
    }

    if( GetSavingConfTimerWakeUp() > 1 )
    {
        SetSavingConfTimerWakeUp( 1 );
    }

    // 연속 추출
    if( GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
    {
        SetWaterOutFixedAmoutTargetTime( 60000UL );   // @10ms, 10min 
    }

    // 아이스 도어
    if( GetIceDoorConfClosedTime() > 60 )
    {
        SetIceDoorConfClosedTime( 60 );
    }

    // 서비스 점검
    if( GetServiceTime() > 1 )
    {
        SetServiceTime( 1 );
        StartTimer(TIMER_PROCESS, TIMER_ID_1MIN, SEC(60));
    }

    // 냉수 수위 지연 시간
    if( GetColdWaterFeedTime() > 1 )
    {
        SetColdWaterFeedTime( 1 );
    }

    // 정수 수위 지연 시간
    if( GetRoomWaterFeedTime() > 1 )
    {
        SetRoomWaterFeedTime( 1 );
    }

}

static void Stop(void)
{
    // 메멘토 기능 해제 
    SetMementoDisp( FALSE );

    // 수위 센서 감지 시간
    InitTankConfDetectTime();

    // 조도 타이머 
    SetSavingConfTimerSleep( DEFAULT_SLEEP_TIMER );
    SetSavingConfTimerWakeUp( DEFAULT_WAKE_UP_TIMER );

    // 아이스 도어 닫힘
    SetIceDoorConfClosedTime( ICE_CLOSE_TIME );

    // 서비스 타임 
    SetServiceTime( SERVICE_TIME_MIN );

    // 정수 수위 센서 에러 카운트 최대값 설정
    SetRoomErrCountLimit( DEFAULT_ROOM_ERR_COUNT );


#if !CONFIG_STER
    SetHealthConfDrainWater( NONE_RTC_TARGET_TIME, NONE_RTC_1HOUR, NONE_RTC_2HOUR, DEFAULT_UNUSED_WEEK_DAYS );
#endif
}

void ProcessTimeShort(void)
{
    static U8 mu8InitStart = FALSE;
    static U8 mu8InitStop = TRUE;


    // Start, 1번만 실행
    if( mu8InitStart == FALSE && ts.Status == TRUE )
    {
        mu8InitStart = TRUE;
        mu8InitStop = FALSE;
        Start();
        SetVersionDisp( 0 );
    }

    // Stop, 1번만 실행
    if( mu8InitStop == FALSE && ts.Status == FALSE )
    {
        mu8InitStop = TRUE;
        Stop();
    }

    // 테스트 모드가 종료
    if( ts.Status == FALSE )
    {
        return ;
    }

    // 전체 검사 시간 초과, 테스트 모드 해제
    if( ts.TestTimer == 0 )
    {
        ts.Status = FALSE;
    }

    Running();
}
