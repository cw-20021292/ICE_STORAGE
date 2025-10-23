/**
 * File : drain.c
 * 
 * Drain Tank Level Control Module
*/
#include "drain.h"
#include "drain_pump.h"
#include "level.h"
#include "eeprom.h"
#include "ice_full.h"

typedef struct _drain_
{
    U8 InitDrain;         /// 드레인 탱크 초기화 FLAG
    U8 InitDrainReady;    /// 드레인 탱크 초기화 READY
    U16 InitTime;         /// 드레인 탱크 초기화나 낮12시 동작시 최대 동작시간 2분
    U8 Level;             /// 드레인 탱크 수위
    
    U32 RottenTime;       /// 고인물 방지. ( 드레인 저수위 센서 감지 시간 )
    U16 MinOnTimeErrLow;  /// 저수위 센서 고장시 최소 펌프 동작 시간
    U16 MinOnTime;        /// 펌프 피드백 전류 감지 off 전 최소 동작 시간
} SDrain_T;

SDrain_T Drain;

#define INITIAL_DRAIN_MAX  (200)                       /// /20sec, @100msec
#define ROTTEN_TIME        ( 3UL * 60UL * 60UL * 10UL ) /// 3hour, @100ms
#define MIN_ON_TIME        ( 40UL * 10UL )             /// 40sec, @100ms
#define MIN_HALF_ON_TIME   ( 20UL * 10UL )             /// 20sec, @100ms */

/*Init*/
void  InitDrain(void)
{
    Drain.InitDrain = FALSE;
    Drain.InitDrainReady = FALSE;
    Drain.InitTime = 0;
    Drain.Level = LEVEL_LOW;

    Drain.RottenTime = ROTTEN_TIME;
    Drain.MinOnTimeErrLow = 0;
    Drain.MinOnTime = 0;
}

/* Drain Level */
void SetDrainLevel(U8 val)
{
    Drain.Level = val;
}

U8 GetDrainLevel(void)
{
    return Drain.Level;
}

/*InitDrain*/
void SetInitDrain(U8 val)
{
    Drain.InitDrain = val;
}

U8 GetInitDrain(void)
{
    return Drain.InitDrain;
}

void SetInitDrainReady(U8 val)
{
    Drain.InitDrainReady = val;
}

U8 GetInitDrainReady(void)
{
    return Drain.InitDrainReady;
}

void StartDrainPumpOnLevel(void)
{
    if( Drain.Level != LEVEL_LOW )
    {
        TurnOnDrainPump();
    }
}

/// 만빙이 되면 배수 동작을 한다.
static U8 IsSetIceFull(U8 level)
{
    static U8 prev = FALSE;
    U8 cur;

    cur = GetIceFullStatus();
    if( prev != cur )
    {
        prev = cur;
        if( cur == TRUE && level != LEVEL_LOW )
        {
            return TRUE;   // turn on drian pump
        }
    }

    return FALSE; // do not turn on drain pump
}

/* 드레인 탱크 제어 */
void ControlDrainLevel(void)
{
    static U8 initTime = 30; /// 3sec @100ms

    /* Wait Delay Time for Detect Level sensor */
    if( initTime != 0 )
    {
        initTime--;
        return ;
    }

    /* Update Watere Level */
    Drain.Level = GetTankLevel(LEVEL_ID_DRAIN);

    /* Init Drain */
    if( Drain.InitDrain == FALSE )
    {
        /// 초기 배수 시간 제한
        //// 펌프에 부하가 없으면 종료...
        if( GetDrainPumpStatus() == ON )
        {
            Drain.InitTime++;

            if( GetDrainPumpLoadStatus() == FALSE 
                    || Drain.InitTime > INITIAL_DRAIN_MAX )
            {
                TurnOffDrainPump();
                Drain.InitTime = 0;
                Drain.InitDrain = TRUE;
            }
        }

        /// 저수위 아래 잔수 제거를 위해 20초 동안 강제 배수
#if (CONFIG_TANK_TYPE == TANK_RO)
        if( Drain.Level == LEVEL_LOW 
                && GetTankLevel( LEVEL_ID_COLD ) == LEVEL_LOW 
                   && GetTankLevel( LEVEL_ID_ROOM ) == LEVEL_LOW )
#else
        if( Drain.Level == LEVEL_LOW )
#endif
        {
            TurnOnDrainPump();
        }
        else if( Drain.Level != LEVEL_LOW )
        {
            TurnOnDrainPump();
        }
        else 
        {
            Drain.InitDrain = TRUE;
        }

        return;
    }


    if( GetDrainPumpStatus() == OFF )
    {
        Drain.MinOnTime = MIN_HALF_ON_TIME;
        
        // 만수위 센서만 감지 되면 동작함
        if( Drain.Level == LEVEL_HIGH )
        {
            TurnOnDrainPump();
        }
        // 수위 센서 센서 에러 상태에서 동작시에는 최소 운전 시간 설정
        else if( Drain.Level == LEVEL_ERR_LOW )
        {
            TurnOnDrainPump();
            Drain.MinOnTimeErrLow = MIN_ON_TIME;
        }

        // 만빙이 되는 시점에 1회 배수 동작
        if( IsSetIceFull(Drain.Level) == TRUE )
        {
            TurnOnDrainPump();
        }

        // 저수위 센서 감지 상태로 장시간 대기하면,
        // 물이 썩는걸 방지하기 위해 배수 동작
        if( Drain.Level != LEVEL_MID )
        {
            Drain.RottenTime = ROTTEN_TIME;
        }
        else
        {
            if( Drain.RottenTime != 0 )
            {
                Drain.RottenTime--;
            }
            else
            {
                TurnOnDrainPump();
            }
        }
    }
    else 
    {
        Drain.RottenTime = ROTTEN_TIME;

        // 만수위 감지, 저수위 미감지 조건에서 펌프 동작 시간.
        // 펌프가 동작되면 만수위 미감지 상태로 변경.
        // 이때 펌프 최소 동작 시간을 보장
        if( Drain.Level == LEVEL_LOW 
                && Drain.MinOnTimeErrLow == 0 )
        {
            TurnOffDrainPump();
        }
        else if( Drain.Level == LEVEL_MID 
                || Drain.Level == LEVEL_LOW )
        {
            // 저수위 센서가 계속 감지 상태인 경우,
            // 펌프 피드백이 공회전 상태이면 펌프 off
            if( GetDrainPumpLoadStatus() == FALSE 
                    && Drain.MinOnTime == 0 )
            {
                TurnOffDrainPump();
            }
        }

        // 최소 동작 시간
        if( Drain.MinOnTimeErrLow != 0 )
        {
            Drain.MinOnTimeErrLow--;
        }
        
        if( Drain.MinOnTime != 0 )
        {
            Drain.MinOnTime--;
        }
    }
}
