/**
 * 배수( 자동 / 수동 ) 기능에 대한 모듈
 *
 */
#include "drain_water.h"
#include "health_maker.h"
#include "level.h"
#include "water_out.h"
#include "room_water.h"
#include "cold_water.h"
#include "error.h"


// 추출량
#define MIN_CONSUMPTION_ROOM_WATER     302UL // 100ms sec 
#define MIN_CONSUMPTION_COLD_WATER     241UL // 100ms sec

// 정수 수위 만수위 검사 시간
//#define DEFAULT_CHECK_ROOM_LEVEL_TIME  1800U // @100ms, 3min x 60sec
#define DEFAULT_CHECK_ROOM_LEVEL_TIME  3000U // @100ms, 5min x 60sec


DrainWater_T     DrainWater;

// 배수 목표 시간 @100ms
static U32 gu32DrainTimeTable[ 2 ] = 
{
     1200UL,    // HALF DRAIN TIME, 2min.. ( 2m x 60s = 120s )
    60000UL,    // FULL DRAIN TIME, 60min..( 100min x 60s = 6s )
};

static U8 IsValidStartDrain(void);

void  InitDrainWater(void)
{
    DrainWater.Start  = FALSE;
    DrainWater.Mode   = HALF_DRAIN;

    DrainWater.SkipResetDrain = FALSE;

    DrainWater.DrainTime   = 0UL;
    DrainWater.ConsumRoomWater = MIN_CONSUMPTION_ROOM_WATER;
    DrainWater.ConsumColdWater = MIN_CONSUMPTION_COLD_WATER;

    DrainWater.CheckRoomLevel = FALSE;
    DrainWater.CheckRoomTime  = DEFAULT_CHECK_ROOM_LEVEL_TIME;
}

void  GetDrainWaterData(DrainWater_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&DrainWater, sizeof( DrainWater_T ));
}

// 자동 배수 시작/중지
U8 StartDrainWater(U8 mu8Mode)
{
    // 시작 조건이 맞는 경우에만, 시작한다.
    if( IsValidStartDrain() == TRUE )
    {
        DrainWater.Start = TRUE;

        DrainWater.Mode = mu8Mode;
        DrainWater.DrainTime  = gu32DrainTimeTable[ DrainWater.Mode ];
        InitHealthDrain();

        return TRUE;
    }

    return FALSE;
}

static void StopDrainWater(void)
{
    DrainWater.Start = FALSE;

    DrainWater.ConsumRoomWater = MIN_CONSUMPTION_ROOM_WATER;
    DrainWater.ConsumColdWater = MIN_CONSUMPTION_COLD_WATER;

    DrainWater.CheckRoomLevel = FALSE;
    DrainWater.CheckRoomTime  = DEFAULT_CHECK_ROOM_LEVEL_TIME;

    InitHealthDrain();
}

U8 IsStartDrainWater(void)
{
    return DrainWater.Start;
}

U8 IsDrainWaterHalf(void)
{
    if( DrainWater.Mode == HALF_DRAIN )
    {
        return TRUE;
    }

    return FALSE;
}

void SetSkipResetDrain(U8 Skip)
{
    DrainWater.SkipResetDrain = Skip;
}

U8 IsSkipResetDrain(void)
{
    return DrainWater.SkipResetDrain;
}

// 순환 강제 배수 중이면, TRUE
// 그렇지 않으면 FALSE
U8 IsGoingOnDrainWaterFull(void)
{
    if( IsStartDrainWater() == TRUE
            && IsDrainWaterHalf() == FALSE )
    {
        return TRUE;   
    }

    return FALSE;
}

// 배수 기능 취소
U8 ResetDrainWater(void)
{
#if 0
    if( IsSkipResetDrain() == TRUE )
    {
        SetSkipResetDrain(FALSE);
        return FALSE;
    }
#endif

    if( IsStartDrainWater() == TRUE )
    {
        StopDrainWater();
        return TRUE;
    }

    return FALSE;
}


/* 배수 동작 가능 확인
 * Return
 *  - TRUE : 가능
 *  - FALSE : 불가능
 */
static U8 IsValidStartDrain(void)
{
    if( IsErrorType(ERROR_TYPE_DRAIN) == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

/* 물 사용량 계산
 *  - 배수를 할 만큼 물 사용량이 많은지 확인한다.
 *
 * Return
 *  - TRUE : 배수 조건 만족
 *  - FALSE : 배수 조건 불만족
 */
static U8 CheckWaterConsumption(void)
{
    if( DrainWater.ConsumRoomWater != 0 && DrainWater.ConsumColdWater != 0 )
    {
        // 충분히 물을 사용하지 않았으면, 배수 조건이다.
        return TRUE;
    }

    return FALSE;
}

// 물 사용량 계산
static void CountingConsumptionWater(void)
{
    U8 mu8Sel;

    if( GetWaterOut() == TRUE )
    {
        mu8Sel = GetWaterOutSelect();
        if( mu8Sel == SEL_WATER_ROOM || mu8Sel == SEL_WATER_HOT )
        {
            if( DrainWater.ConsumRoomWater != 0 )
            {
                DrainWater.ConsumRoomWater--;
            }
        }
        else
        {
            if( DrainWater.ConsumColdWater != 0 )
            {
                DrainWater.ConsumColdWater--;
            }
        }
    }
}

// 자동 배수 시작전, 탱크 수위 검사
// RETURN :
// TRUE - 자동 배수 시작 할 수 있는 물의 수위
// FALSE - 시작 할 수 없음.
static U8 CheckRoomWaterLevel(void)
{
    if( DrainWater.CheckRoomLevel == TRUE )
    {
        return TRUE;
    }

    // 정수 수위 검사
    if( GetRoomWaterLevel() == LEVEL_HIGH )
    {
        DrainWater.CheckRoomLevel = TRUE;
        return FALSE;
    }

    return FALSE;
}


void ControlDrainWater(void)
{
    U8 mu8RoomLevel;
    U8 mu8ColdLevel;
    U8 mu8Ret = FALSE;
    

    // 물 사용량 계산
    CountingConsumptionWater();

    if( IsStartDrainWater() == FALSE )
    {
        return ;
    }


    // 물 사용량 조건 검사
    mu8Ret = CheckWaterConsumption();
    if( mu8Ret == FALSE )
    {
        StopDrainWater();
        return ;
    }

    // 정수 탱크 만수위 조건 검사( 자동 배수인 경우에만 )
    if( IsDrainWaterHalf() == TRUE )
    {
        mu8Ret = CheckRoomWaterLevel();
        if( mu8Ret == FALSE )
        {
            // 정수 탱크 만수위가 대기 시간 초과하면... 배수 중지...
            if( DrainWater.CheckRoomTime != 0 )
            {
                DrainWater.CheckRoomTime--;
            }
            else
            {
                StopDrainWater();
            }
            return ;
        }
    }

    // 배수 타임 아웃 종료
    // 에러 발생시 종료
    mu8RoomLevel = GetRoomWaterLevel();
    mu8ColdLevel = GetColdWaterLevel();

    if( DrainWater.DrainTime == 0
        ||  IsErrorTankLevel( mu8RoomLevel ) == TRUE
        ||  IsErrorTankLevel( mu8ColdLevel ) == TRUE
        ||  IsErrorType(ERROR_TYPE_DRAIN) == TRUE )
    {
        StopDrainWater();
        return ;
    }

    if( DrainWater.DrainTime != 0 )
    {
        DrainWater.DrainTime--;
    }

    // 배수...
    if( DoHealthDrain() == STATUS_DONE )
    {
        StopDrainWater();
    }
}
