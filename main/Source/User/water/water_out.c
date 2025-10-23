/**
 * File : water_out.c
 *
 * Water Out
*/
#include "water_out.h"
#include "valve.h"
#include "circulate_pump.h"
#include "room_water.h"
#include "cold_water.h"
#include "hot_water.h"
#include "sound.h"
#include "eeprom.h"
#include "tank_clean.h"
#include "amount.h"
#include "process_display.h"
#include "display_lib.h"

#define  CONFIG_STOP_OUT_COLD_LOW_LEVEL   (1)     /// 냉수 저수위 센서 미감지, 추출 중지

#define  RETURN_TIME_AFTER_WATER_OUT      (500)   /// 5sec @10ms
#define  DEFAULT_RETURN_TIME              (700)   /// 7sec @10ms

#define  LIMIT_TIME  (U32)-1

typedef struct _water_out_
{
    /* Select */
    U8 Select;
    U8 LastedSelect;
    U8 PrevSelect;
    U16 ReturnTime;

    /* Water Out Mode */
    U8 WaterOut;         // TRUE or FALSE

    /* Fixed Amouted */
    U8 Continue;               // 연속 추출 (TRUE or FALSE)
    U16 ContinueReturnTime;     // 연속 선택 해제 시간 
    U8 InfinityAmout;          // 연속 추출 설정/해제
    U8 FixedAmoutType;         //  FIXED_AMOUNT_1L, FIXED_AMOUNT_1D5L
    U32 FixedAmoutTargetTime;   // @10msec 
    U32 FixedAmoutCurrentTime;  // @10msec

    /* Water Out Time */
    U32 LastedTime[ MAX_WATER_ID ];  // 마지막 추출 시간 @10msec
    U32 DailyTime[ MAX_WATER_ID ];   // 일일 추출 시간 @10msec
    U32 TotalTime[ MAX_WATER_ID ];   // 전체 추출 시간 @10msec

    U32 AccumulateTime[ MAX_WATER_ID ]; // 누적 물 추출 시간
} SWaterOut_T;

SWaterOut_T Out;

// @100ms
// 1Cup - 120ml
const static U32 gu32TimePerCup[ MAX_WATER_ID ] =
{
    500UL,      // ROOM
    500UL,      // COLD
    500UL       // HOT
};

//  HIGH or MID @10ms
//  INFINITY - 3min
//  TANK CLEAN - 15min ( 실제 추출시간 5분 48초 )
const static U32 gu32FixedAmoutTimeTable[MAX_WATER_ID][MAX_FIXED_AMOUNT_NUM] = 
{
    /* 500ml    1L        1.5L   INFINITY  TANK CLEAN */
    { 1517UL,   3019UL,   4700UL,  18000UL,  90000UL },   /// ROOM
    { 1195UL,   2408UL,   3623UL,  18000UL,  90000UL },   /// COLD
#if( CONFIG_TEST_8585_HOT_REPEAT == 1 )
    //250ml
    { 1053UL,   5960UL,   8440UL,  18000UL,  90000UL }    /// HOT
#else
    { 2107UL,   4035UL,   6446UL,  18000UL,  90000UL }    /// HOT
#endif
};

// Lower Level Add Time
const static U32 gu32LowLevelAddTimeTable[ MAX_WATER_ID ] =
{
    170UL,      // ROOM
    0UL,      // COLD
    291UL       // HOT
};


void  InitWaterOut(void)
{
    Out.Select          = SEL_WATER_ROOM;
    Out.LastedSelect    = SEL_WATER_ROOM;
    Out.PrevSelect      = SEL_WATER_ROOM;
    Out.ReturnTime      = 0;

    Out.WaterOut        = FALSE;

    Out.Continue        = FALSE;
    Out.ContinueReturnTime  = 0;
    Out.InfinityAmout   = FALSE;
    Out.FixedAmoutType  = FIXED_AMOUNT_500ML;
    Out.FixedAmoutTargetTime  = 0UL;
    Out.FixedAmoutCurrentTime = 0UL;

    Out.LastedTime[ SEL_WATER_ROOM ] = 0UL;
    Out.LastedTime[ SEL_WATER_COLD ] = 0UL;
    Out.LastedTime[ SEL_WATER_HOT ] = 0UL;

    Out.DailyTime[ SEL_WATER_ROOM ] = 0UL;
    Out.DailyTime[ SEL_WATER_COLD ] = 0UL;
    Out.DailyTime[ SEL_WATER_HOT ] = 0UL;
    
    Out.TotalTime[ SEL_WATER_ROOM ] = 0UL;
    Out.TotalTime[ SEL_WATER_COLD ] = 0UL;
    Out.TotalTime[ SEL_WATER_HOT ] = 0UL;

    Out.AccumulateTime[ SEL_WATER_ROOM ] = 0UL;
    Out.AccumulateTime[ SEL_WATER_COLD ] = 0UL;
    Out.AccumulateTime[ SEL_WATER_HOT ] = 0UL;
}

void  SetWaterOutSelect(U8 id)
{
    if( id >= MAX_WATER_ID )
    {
        return ;
    }

    if( Out.Select != id )
    {
        Out.PrevSelect = Out.Select;
    }
    Out.ReturnTime = DEFAULT_RETURN_TIME; // 7sec@100ms

    Out.Select     = id;

    SaveEepromId( EEP_ID_SEL_WATER );
}

U8 GetWaterOutSelect(void)
{
    return Out.Select;
}

U8 GetWaterOutLastedSelect(void)
{
    return Out.LastedSelect;
}

void UpdateWaterOutReturnTime(void)
{
    Out.ReturnTime = DEFAULT_RETURN_TIME;
}

void ClearWaterOutReturnTime(void)
{
    Out.ReturnTime = 0U;
}

U8 IsExpiredWaterOutReturnTime(void)
{
    if( Out.ReturnTime != 0 )
    {
        return FALSE;
    }

    return TRUE;
}

/* 추출 시작 */
void  StartWaterOut(void)
{
    if( Out.WaterOut == FALSE )
    {
        Out.LastedTime[ Out.Select ] = 0;
    }
    Out.WaterOut = TRUE;
}

/* 추출 중지 */
void  StopWaterOut(void)
{
    Out.WaterOut = FALSE;
    SetTankClean( FALSE );

#if 0

  // IGROO

    SetIoCareMode( IOC_ID_USE_WATER_DATA );
#endif
}

/* 추출 상태 반환
 * Return
 *  -TRUE : 추출 중
 *  -FALSE : 추출 중지 상태
 * */
U8 GetWaterOut(void)
{
    return Out.WaterOut;
}


/* 연속 추출 설정/해제 */
void  SetWaterOutContinue(U8 mode)
{
    if( mode != TRUE && mode != FALSE )
    {
        return ;
    }

    Out.Continue = mode;
    if( mode == TRUE )
    {
        Out.ContinueReturnTime = DEFAULT_RETURN_TIME;
    }
    SaveEepromId( EEP_ID_CONF_CONTY );
}

U8 GetWaterOutContinue(void)
{
    return Out.Continue;
}


void  SetWaterOutFixedAmountType(U8 type)
{
    if( type >= MAX_FIXED_AMOUNT_NUM )
    {
        return ;
    }

    if( type == FIXED_AMOUNT_INFINITY )
    {
        Out.InfinityAmout = TRUE;
    }
    else
    {
        Out.InfinityAmout  = FALSE;
        Out.FixedAmoutType  = type;
    }

    Out.FixedAmoutTargetTime = gu32FixedAmoutTimeTable[Out.Select][type];


    SaveEepromId( EEP_ID_CONF_FIXED_AMOUNT );
}

U8    GetWaterOutFixedAmountType(void)
{ 
    return Out.FixedAmoutType;
}

U8 GetWaterOutAmoutType(void)
{
    if( Out.InfinityAmout == TRUE )
    {
        return FIXED_AMOUNT_INFINITY;
    }

    return GetWaterOutFixedAmountType();
}

void ClearWaterOutInfinityAmout(void)
{
    Out.InfinityAmout = FALSE;
}


void  SetWaterOutFixedAmoutTargetTime(U32 time)
{
    Out.FixedAmoutTargetTime = time;
}

U32   GetWaterOutFixedAmoutTargetTime(void)
{
    return Out.FixedAmoutTargetTime;
}

void  SetWaterOutFixedAmoutCurrentTime(U32 time)
{
    Out.FixedAmoutCurrentTime = time;
}

U32   GetWaterOutFixedAmoutCurrentTime(void)
{
    return Out.FixedAmoutCurrentTime;
}


void  SetWaterOutLastedTime(U8 id, U32 time)
{
    Out.LastedTime[id] = time;;
}

U32  GetWaterOutLastedTime(U8 id )
{
    return Out.LastedTime[id];
}


void  SetWaterOutDailyTime(U8 id, U32 time )
{
    Out.DailyTime[id] = time;
}

U32  GetWaterOutDailyTime(U8 id )
{
    return Out.DailyTime[id];
}


void  SetWaterOutTotalTime(U8 id, U32 time)
{
    Out.TotalTime[id] = time;
}

U32  GetWaterOutTotalTime(U8 id )
{
    return Out.TotalTime[id];
}


void SetWaterOutTime(U8 type, U8 id, U32 time)
{
    if( type == TYPE_LASTED )
    {
        Out.LastedTime[id] = time;
    }
    else if( type == TYPE_DAILY )
    {
        Out.DailyTime[id] = time;
    }
    else
    {
        Out.TotalTime[id] = time;
    }
}

U32  GetWaterOutTime(U8 type, U8 id)
{
    U32 mu32Time = 0;

    if( (type & TYPE_LASTED) != 0 )
    {
        mu32Time += Out.LastedTime[id];
    }

    if( (type & TYPE_DAILY) != 0 )
    {
        mu32Time += Out.DailyTime[id];
    }

    if( (type & TYPE_TOTAL) != 0 )
    {
        mu32Time += Out.TotalTime[id];
    }

    return mu32Time;
}

void ResetWaterOutTime(U8 type, U8 id )
{
    if( (type & TYPE_LASTED) != 0 )
    {
        Out.LastedTime[id] = 0;
    }

    if( (type & TYPE_DAILY) != 0 )
    {
        Out.DailyTime[id] = 0;
    }

    if( (type & TYPE_TOTAL) != 0 )
    {
        Out.TotalTime[id] = 0;
    }
}

static U8 ConvWaterOut_Time2Cup(U32 time, U32 cupTime)
{
    U32 mu32CupSize;


    if( time < (cupTime/3) )
    {
        mu32CupSize = 0;    // 0-cup under 1/3
    }
    else if( time <= cupTime )
    {
        mu32CupSize = 1;
        return 1;    // 1-cup over 1/3
    }
    else
    {
        // Round Up
        time += (cupTime / 2);    
        mu32CupSize = time / cupTime;
    }

    return (U8)mu32CupSize;
}

U8 GetWaterOutLastedCupNum(U8 id)
{
    U8 num = 0U;

    num = ConvWaterOut_Time2Cup( 
            Out.LastedTime[id],
            gu32TimePerCup[id]
            );

    return num;
}

U8 GetWaterOutDailyCupNum(U8 id)
{
    U8 num = 0U;

    num = ConvWaterOut_Time2Cup( 
            Out.DailyTime[id],
            gu32TimePerCup[id]
            );

    return num;
}

U8 GetWaterOutTotalCupNum(U8 id)
{
    U8 num = 0U;

    num = ConvWaterOut_Time2Cup( 
            Out.TotalTime[id],
            gu32TimePerCup[id]
            );

    return num;   
}

// 냉수 만수위 기준 / 냉수 누적 추출양이 2L 이상일 때만 냉수 재정수 되도록 해야 함
// 냉수 만수위가 미감지되면 냉수 누적 추출양 Clear
#define MAX_WATER_OUT_COUNT       (60000UL)
#define MAX_WATER_OUT_TIME        (4816UL)      // 48.16sec @ 10ms
void SetWaterOutAccumulateTime(U8 id, U32 time)
{
    if( Out.AccumulateTime[id] < MAX_WATER_OUT_COUNT )
    {
        Out.AccumulateTime[id] += (time);
    }
}

void ClearWaterOutAccumulateTime(U8 id)
{
    Out.AccumulateTime[id] = 0U;
}

U32 GetWaterOutAccumulateTime(U8 id)
{
    return Out.AccumulateTime[id];
}

U8 IsWaterOutMaxTime(U8 id)
{
    if( Out.AccumulateTime[id] >= MAX_WATER_OUT_TIME )
    {
        return TRUE;
    }

    return FALSE;
}

U8 GetWaterOutPercent(void)
{
    return CalcPercent( Out.FixedAmoutTargetTime, Out.FixedAmoutCurrentTime );
}

//U32 GetFixedAmoutTimeTable(U8 mu8select, U8 mu8AmountType)
//{
//    U32 mu32FixedAmountTimeTable;
//
//    mu32FixedAmountTimeTable = gu32FixedAmoutTimeTable[ mu8select ][ mu8AmountType ];
//    
//    return mu32FixedAmountTimeTable;
//}

static void ControlWaterOutSelect(void);
static void ControlWaterOutContinue(void);

static void CheckWaterOutStopCondition(void);
static void CheckWaterOutStopConditionEol(void);
static void ControlTankClean(void);
static void ControlWaterOutValve(void);
void  ControlWaterOut(void)
{  
    ControlWaterOutSelect();      // 냉/온/정 선택 
    ControlWaterOutContinue();    // 정량 추출 기능 
    CheckWaterOutStopCondition(); // 추출 정지 조건 검사
    ControlTankClean();
    ControlWaterOutValve();       // 냉/온/정 추출 밸브 제어
}

void ControlWaterOutEol(void)
{
    ControlWaterOutSelect();         // 냉/온/정 선택 
    ControlWaterOutContinue();       // 정량 추출 기능 
    CheckWaterOutStopConditionEol(); // 추출 정지 조건 검사
    ControlTankClean();
    ControlWaterOutValve();          // 냉/온/정 추출 밸브 제어
}

static void  ControlWaterOutSelect(void)
{

    /* 1. 온수 선택 후  7sec 후에 다시 냉수 또는 정수로 물 선택을 복귀 시킨다.
     * 2. 단, 냉수 사용이 해제되어 있는 경우에는 정수를 선택한다.
     * 3. 냉수 선택 시 냉수 사용이 해제되어 있는 경우에는 정수로 물 선택을 복귀 시킨다.
     *
     * 물이 추출 중인 경우에는, 물 추출 이후에 5초 타이머가 적용될 수 있도록
     * 타이머를 reset 한다.
     */
#if(  CONFIG_TEST_HOT_WATER == 1 )
    if( Out.Select == SEL_WATER_COLD )
#else
    if( Out.Select == SEL_WATER_HOT 
            || Out.Select == SEL_WATER_COLD )
#endif
    {
        if( Out.WaterOut == TRUE )
        {
            Out.ReturnTime  = DEFAULT_RETURN_TIME;    

            /* Make Cold Off, Select Cold */
            if( Out.Select == SEL_WATER_COLD 
                    && GetColdWaterConfigMake() == FALSE )
            {
                Out.ReturnTime = RETURN_TIME_AFTER_WATER_OUT;
            }
            /* Make Hot Off, Select Hot */
            else if( Out.Select == SEL_WATER_HOT 
                    && GetHotWaterConfigMake() == FALSE )
            {
                Out.ReturnTime = RETURN_TIME_AFTER_WATER_OUT;
            }
        }

        if( Out.ReturnTime == 0 )
        {
            // Hot Water -> Previous 
            if( Out.Select == SEL_WATER_HOT )
            {
                Out.Select = Out.PrevSelect;
            }

            // If Cold & Make Off -> Room 
            if( Out.Select == SEL_WATER_COLD 
                    && GetColdWaterConfigMake() == FALSE 
                    )
            {
                Out.Select = SEL_WATER_ROOM;
            }

        }
    }
    else if( Out.Select == SEL_WATER_ROOM )
    {
        if( Out.WaterOut == TRUE )
        {
            Out.ReturnTime  = DEFAULT_RETURN_TIME;
        }
    }


    if( Out.ReturnTime != 0 )
    {
        Out.ReturnTime--;
    }
}

static void ControlWaterOutContinue(void)
{
    U8 amoutType = 0U;

    // 추출 중에는 시간 reload
    if( Out.WaterOut == TRUE && Out.Continue == TRUE )
    {
        Out.ContinueReturnTime = DEFAULT_RETURN_TIME;
    }

    // 연속 추출 기능 해제 
    if( Out.Continue == TRUE && Out.ContinueReturnTime == 0 )
    {
        SetWaterOutContinue(FALSE);
        ClearWaterOutInfinityAmout();
    }
    else
    {
        if( Out.ContinueReturnTime != 0 )
        {
            Out.ContinueReturnTime--;
        }
    }

    // 연속 추출 목표 시간 및 현재 시간 계산...
    amoutType = GetWaterOutAmoutType();
    if( Out.WaterOut == FALSE 
            || Out.Continue == FALSE )
    {
        Out.FixedAmoutTargetTime = gu32FixedAmoutTimeTable[ Out.Select ][amoutType];

        // 추가 입수 ( 저수위 조건 )
        if( amoutType == FIXED_AMOUNT_500ML || amoutType == FIXED_AMOUNT_1L || amoutType == FIXED_AMOUNT_1D5L )
        {
            if( Out.Select == SEL_WATER_ROOM && GetRoomWaterLevel() == LEVEL_LOW )
            {
                Out.FixedAmoutTargetTime += gu32LowLevelAddTimeTable[ Out.Select ];
            }
            else if( Out.Select == SEL_WATER_HOT && GetHotWaterLevel() == LEVEL_LOW )
            {
                Out.FixedAmoutTargetTime += gu32LowLevelAddTimeTable[ Out.Select ];
            }
        }

        Out.FixedAmoutCurrentTime   = 0;
        return ;
    }

    // 탱크 비우기 모드, 추출 목표 시간 값을 재설정
    if( GetTankClean() == TRUE )
    {
        SetColdWaterFeedTime(0);      // 냉수 재정수 시간 초기화
        SetRoomWaterInitFull(TRUE);   // 냉수 입수를 위해서 정수 최초 만수위 강제 설정
        Out.FixedAmoutTargetTime = gu32FixedAmoutTimeTable[ Out.Select ][FIXED_AMOUNT_TANK_CLEAN];
    }

    if( Out.FixedAmoutCurrentTime < Out.FixedAmoutTargetTime )
    {
        Out.FixedAmoutCurrentTime++;
    }
    else
    {
        // 연속 추출 시간 만료되면 추출 중지...
        StopWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        Sound( SOUND_EFFLUENT_END );
    }
}

// 추출 중지 조건 검사
// Precondition 
//  -. 탱크 비움 기능이 아닐 때,
//  -. 물 추출 중일 떄,
static void CheckWaterOutStopCondition(void)
{
    // 물 추출 중일 때,
    if( Out.WaterOut == FALSE )
    {
        return ;
    }

    // 탱크 비움 기능이 아닐 때,
    if( GetTankClean() == TRUE )
    {
        return ;
    }

#if( CONFIG_STOP_OUT_COLD_LOW_LEVEL == 1 )
    // 냉수 추출 중, 냉수 탱크 저수위이면 추출을 중지시킨다.
    // 냉수 추출 펌프 소음을 방지하기 위한 조건이다.
    if( Out.Select == SEL_WATER_COLD )
    {
        if( GetColdWaterLevel() == LEVEL_LOW )
        {
            StopWaterOut();
            StartDispTimerId(DISP_TIMER_ID_ERROR);
            Sound( SOUND_EFFLUENT_END );
        }
    }
#endif
}

// 추출 중지 조건 검사
// Precondition 
//  -. 탱크 비움 기능이 아닐 때,
//  -. 물 추출 중일 떄,
static void CheckWaterOutStopConditionEol(void)
{
    // 물 추출 중일 때,
    if( Out.WaterOut == FALSE )
    {
        return ;
    }

    // 탱크 비움 기능이 아닐 때,
    if( GetTankClean() == TRUE )
    {
        return ;
    }

#if( CONFIG_STOP_OUT_COLD_LOW_LEVEL == 1 )
    // 냉수 추출 중, 냉수 탱크 저수위이면 추출을 중지시킨다.
    // 냉수 추출 펌프 소음을 방지하기 위한 조건이다.
    if( Out.Select == SEL_WATER_COLD )
    {
        if( GetTankLevel( LEVEL_ID_COLD ) == LEVEL_LOW )
        {
            StopWaterOut();
            StartDispTimerId(DISP_TIMER_ID_ERROR);
            Sound( SOUND_EFFLUENT_END );
        }
    }
#endif
}


#define RESIDUAL_TIME     1300UL // 13sec @10ms
static void ControlTankClean(void)
{
    static U32 residualTime;

    if( GetTankClean() != TRUE )
    {
        residualTime = RESIDUAL_TIME;
        return ;
    }

    if( GetRoomWaterLevel() == LEVEL_LOW 
            && GetColdWaterLevel() == LEVEL_LOW )
    {
        if( residualTime != 0 )
        {
            residualTime--;
        }
        else
        {
            StopWaterOut();
            StartDispTimerId(DISP_TIMER_ID_ERROR);
            Sound( SOUND_EFFLUENT_END );
        }
    }
}

static void  ControlWaterOutValve(void)
{
    U8 select;

    /* TURN OFF WATER OUT */
    if( Out.WaterOut == FALSE )
    {    
        /* ROOM */
        CloseValve( VALVE_ROOM_OUT );

        /* HOT */
        CloseValve( VALVE_HOT_OUT );

        /* COLD */
        TurnOffCirculatePumpWaterOut();
        return ;
    }

    select = Out.Select;

    /* Update Time information */
    if( Out.LastedTime[select] < LIMIT_TIME )
    {
        Out.LastedTime[select]++;
    }
    if( Out.DailyTime[select] < LIMIT_TIME )
    {
        Out.DailyTime[select]++;
    }
    if( Out.TotalTime[select] < LIMIT_TIME )
    {
        Out.TotalTime[select]++;
    }

    /* TURN ON WATER OUT - COLD */
    if( select == SEL_WATER_COLD )
    {
        CloseValve( VALVE_HOT_OUT );
        CloseValve( VALVE_ROOM_OUT );
        TurnOnCirculatePumpWaterOut();
    }
    /* TURN ON WATER OUT - HOT */
    else if( select == SEL_WATER_HOT )
    {
        OpenValve( VALVE_HOT_OUT );
        CloseValve( VALVE_ROOM_OUT );
        TurnOffCirculatePumpWaterOut();
    }
    /* TURN ON WATER OUT - ROOM */
    else
    {
        CloseValve( VALVE_HOT_OUT );
        OpenValve( VALVE_ROOM_OUT );
        TurnOffCirculatePumpWaterOut();
    }
    
    /* Updated Lasted Selected Out */
    Out.LastedSelect = select;
}
