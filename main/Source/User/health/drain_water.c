/**
 * ���( �ڵ� / ���� ) ��ɿ� ���� ���
 *
 */
#include "drain_water.h"
#include "health_maker.h"
#include "level.h"
#include "water_out.h"
#include "room_water.h"
#include "cold_water.h"
#include "error.h"


// ���ⷮ
#define MIN_CONSUMPTION_ROOM_WATER     302UL // 100ms sec 
#define MIN_CONSUMPTION_COLD_WATER     241UL // 100ms sec

// ���� ���� ������ �˻� �ð�
//#define DEFAULT_CHECK_ROOM_LEVEL_TIME  1800U // @100ms, 3min x 60sec
#define DEFAULT_CHECK_ROOM_LEVEL_TIME  3000U // @100ms, 5min x 60sec


DrainWater_T     DrainWater;

// ��� ��ǥ �ð� @100ms
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

// �ڵ� ��� ����/����
U8 StartDrainWater(U8 mu8Mode)
{
    // ���� ������ �´� ��쿡��, �����Ѵ�.
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

// ��ȯ ���� ��� ���̸�, TRUE
// �׷��� ������ FALSE
U8 IsGoingOnDrainWaterFull(void)
{
    if( IsStartDrainWater() == TRUE
            && IsDrainWaterHalf() == FALSE )
    {
        return TRUE;   
    }

    return FALSE;
}

// ��� ��� ���
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


/* ��� ���� ���� Ȯ��
 * Return
 *  - TRUE : ����
 *  - FALSE : �Ұ���
 */
static U8 IsValidStartDrain(void)
{
    if( IsErrorType(ERROR_TYPE_DRAIN) == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

/* �� ��뷮 ���
 *  - ����� �� ��ŭ �� ��뷮�� ������ Ȯ���Ѵ�.
 *
 * Return
 *  - TRUE : ��� ���� ����
 *  - FALSE : ��� ���� �Ҹ���
 */
static U8 CheckWaterConsumption(void)
{
    if( DrainWater.ConsumRoomWater != 0 && DrainWater.ConsumColdWater != 0 )
    {
        // ����� ���� ������� �ʾ�����, ��� �����̴�.
        return TRUE;
    }

    return FALSE;
}

// �� ��뷮 ���
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

// �ڵ� ��� ������, ��ũ ���� �˻�
// RETURN :
// TRUE - �ڵ� ��� ���� �� �� �ִ� ���� ����
// FALSE - ���� �� �� ����.
static U8 CheckRoomWaterLevel(void)
{
    if( DrainWater.CheckRoomLevel == TRUE )
    {
        return TRUE;
    }

    // ���� ���� �˻�
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
    

    // �� ��뷮 ���
    CountingConsumptionWater();

    if( IsStartDrainWater() == FALSE )
    {
        return ;
    }


    // �� ��뷮 ���� �˻�
    mu8Ret = CheckWaterConsumption();
    if( mu8Ret == FALSE )
    {
        StopDrainWater();
        return ;
    }

    // ���� ��ũ ������ ���� �˻�( �ڵ� ����� ��쿡�� )
    if( IsDrainWaterHalf() == TRUE )
    {
        mu8Ret = CheckRoomWaterLevel();
        if( mu8Ret == FALSE )
        {
            // ���� ��ũ �������� ��� �ð� �ʰ��ϸ�... ��� ����...
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

    // ��� Ÿ�� �ƿ� ����
    // ���� �߻��� ����
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

    // ���...
    if( DoHealthDrain() == STATUS_DONE )
    {
        StopDrainWater();
    }
}
