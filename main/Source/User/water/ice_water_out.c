/**
 * File : ice_water_out.c
 * 
 * Ice Water Out
*/
#include "ice_water_out.h"
#include "valve.h"
#include "sound.h"
#include "eeprom.h"
#include "cold_water.h"
#include "circulate_pump.h"
#include "tank_clean.h"
#include "amount.h"
#include "ice_door.h"
#include "process_display.h"
#include "display_lib.h"
#include "ice_door.h"
#include "flow_meter.h"
#include "error.h"

#define  CONFIG_STOP_OUT_COLD_LOW_LEVEL   (1)     /// 냉수 저수위 센서 미감지, 추출 중지

#define  DEFAULT_RETURN_TIME              (700)   /// 7sec @10ms

#define  LIMIT_TIME  (U32)-1

typedef struct _ice_water_out_
{
    /* Select */
    U8   Select;
    U8   LastedSelect;
    U8   PrevSelect;
    U16  ReturnTime;

    /* Water Out Mode */
    U8   IceOut;                 // TRUE or FALSE
    U8   WaterOut;               // TRUE or FALSE

    /* Fixed Amouted */
    U8   Continue;               // 연속 추출 (TRUE or FALSE)
    U16  ContinueReturnTime;     // 연속 선택 해제 시간 
    U8   InfinityAmout;          // 연속 추출 설정/해제
    U8   FixedAmoutType;         // FIXED_AMOUNT_500mL, FIXED_AMOUNT_1L, FIXED_AMOUNT_1D5L
    U32  FixedAmoutTargetTime;   // @10msec 
    U32  FixedAmoutCurrentTime;  // @10msec
    
    U32  FixedIceTargetTime;   // @10msec 
    U32  FixedIceCurrentTime;  // @10msec
    U32  FixedAmountTarget;      // 유량센서 타켓 용량
    U32  FixedAmountCurrent;     // 유량센서 현재 용량

    /* Water Out Time */
    U32  LastedTime;  // 마지막 추출 시간 @10msec
    U32  DailyTime;   // 일일 추출 시간 @10msec
    U32  TotalTime;   // 전체 추출 시간 @10msec

    U32 AccumulateTime; // 누적 얼음물 추출 시간
} SIceWaterOut_T;

SIceWaterOut_T IceWaterOut;

// @100ms
// 1Cup - 120ml
const static U32 gu32TimePerCup = 250UL;

const static U32 gu32FixedIceTimeTable[MAX_FIXED_ICE_WATER_AMOUNT_NUM] = 
{
    /* 얼음1개 9g기준 */
    /* 500ml(135g)   700mL(180g)   1.0L(270g)  INFINITY  */
      //740UL,         1057UL,       1480UL,      9000UL         /// ICE
      1820UL,         2442UL,       3000UL,      9000UL         /// ICE
};

//  HIGH or MID @10ms
//  INFINITY - 3min
//  TANK CLEAN - 15min ( 실제 추출시간 5분 48초 )
const static U32 gu32FixedAmoutTimeTable[MAX_FIXED_ICE_WATER_AMOUNT_NUM] = 
{
    /* 500ml(350ml)  700mL(500ml)  1.0L(700ml)  INFINITY  */
      1480UL,        2114UL,       2960UL,      18000UL         /// ICE_WATER (COLD)
};

// Lower Level Add Time
const static U32 gu32LowLevelAddTimeTable = 0UL; /// ICE_WATER(COLD)

void  InitIceWaterOut(void)
{
    IceWaterOut.Select = SEL_ICE;
    IceWaterOut.LastedSelect = SEL_ICE;
    IceWaterOut.PrevSelect = SEL_ICE;
    IceWaterOut.ReturnTime = 0;

    IceWaterOut.IceOut = FALSE;
    IceWaterOut.WaterOut = FALSE;

    IceWaterOut.Continue = FALSE;
    IceWaterOut.ContinueReturnTime = 0;
    IceWaterOut.InfinityAmout = FALSE;
    IceWaterOut.FixedAmoutType = FIXED_ICE_WATER_AMOUNT_500ML;
    IceWaterOut.FixedAmoutTargetTime = 0UL;
    IceWaterOut.FixedAmoutCurrentTime = 0UL;

    IceWaterOut.FixedIceTargetTime = 0UL;
    IceWaterOut.FixedIceCurrentTime = 0UL;
    IceWaterOut.FixedAmountTarget = 0UL;
    IceWaterOut.FixedAmountCurrent = 0UL;

    IceWaterOut.LastedTime = 0UL;

    IceWaterOut.DailyTime = 0UL;
    
    IceWaterOut.TotalTime = 0UL;

    IceWaterOut.AccumulateTime = 0UL;
}

void SetIceWaterSelect(U8 id)
{
    if( id >= MAX_ICE_WATER_ID )
    {
        return;
    }

    if( IceWaterOut.Select != id )
    {
        IceWaterOut.PrevSelect = IceWaterOut.Select;
    }
    IceWaterOut.ReturnTime = DEFAULT_RETURN_TIME; /// 7sec@100ms

    IceWaterOut.Select = id;

    SaveEepromId(EEP_ID_SEL_ICE_WATER);
}

U8 GetIceWaterSelect(void)
{
    return IceWaterOut.Select;
}

U8 GetIceWaterLastedSelect(void)
{
    return IceWaterOut.LastedSelect;
}

void UpdateIceWaterOutReturnTime(void)
{
    IceWaterOut.ReturnTime = DEFAULT_RETURN_TIME;
}

void ClearIceWaterOutReturnTime(void)
{
    IceWaterOut.ReturnTime = 0U;
}

U8 IsExpiredIceWaterOutReturnTime(void)
{
    if( IceWaterOut.ReturnTime != 0 )
    {
        return FALSE;
    }

    return TRUE;
}

/* 얼음 추출 시작 */
void StartIceOut(void)
{
    IceWaterOut.IceOut = TRUE;
}

void StopIceOut(void)
{
    IceWaterOut.IceOut = FALSE;
}

U8 GetIceOut(void)
{
    return IceWaterOut.IceOut;
}

/* 추출 시작 */
void  StartIceWaterOut(void)
{
    if( IceWaterOut.WaterOut == FALSE )
    {
        IceWaterOut.LastedTime = 0;
    }
    IceWaterOut.WaterOut = TRUE;
}

/* 추출 중지 */
void  StopIceWaterOut(void)
{
    IceWaterOut.WaterOut = FALSE;
}

/* 추출 상태 반환
 * Return
 *  -TRUE : 추출 중
 *  -FALSE : 추출 중지 상태
 * */
U8 GetIceWaterOut(void)
{
    return IceWaterOut.WaterOut;
}

/* 연속 추출 설정/해제 */
void SetIceWaterOutContinue(U8 mode)
{
    if( mode != TRUE && mode != FALSE )
    {
        return ;
    }

    IceWaterOut.Continue = mode;
    if( mode == TRUE )
    {
        IceWaterOut.ContinueReturnTime = DEFAULT_RETURN_TIME;
    }
    SaveEepromId(EEP_ID_CONF_ICE_WATER_CONTY);
}

U8 GetIceWaterOutContinue(void)
{
    return IceWaterOut.Continue;  // TRUE : 얼음물 정량추출
}

void SetIceWaterOutFixedAmountType(U8 type)
{
    if( type >= MAX_FIXED_ICE_WATER_AMOUNT_NUM )
    {
        return ;
    }

    if( type == FIXED_ICE_WATER_AMOUNT_INFINITY )
    {
        IceWaterOut.InfinityAmout = TRUE;
    }
    else
    {
        IceWaterOut.InfinityAmout  = FALSE;
        IceWaterOut.FixedAmoutType  = type;
    }

    IceWaterOut.FixedAmoutTargetTime = gu32FixedAmoutTimeTable[type];

    SaveEepromId( EEP_ID_CONF_ICE_WATER_FIXED_AMOUNT );
}

U8 GetIceWaterOutFixedAmountType(void)
{
    return IceWaterOut.FixedAmoutType;
}

U8 GetIceWaterOutAmoutType(void)
{
    if( IceWaterOut.InfinityAmout == TRUE )
    {
        return FIXED_ICE_WATER_AMOUNT_INFINITY;
    }

    return GetIceWaterOutFixedAmountType();
}

void ClearIceWaterOutInfinityAmout(void)
{
    IceWaterOut.InfinityAmout = FALSE;
}


void SetIceWaterOutFixedAmoutTargetTime(U32 time)
{
    IceWaterOut.FixedAmoutTargetTime = time;
}

U32 GetIceWaterOutFixedAmoutTargetTime(void)
{
    return IceWaterOut.FixedAmoutTargetTime;
}

void SetIceWaterOutFixedAmoutCurrentTime(U32 time)
{
    IceWaterOut.FixedAmoutCurrentTime = time;
}

U32 GetIceWaterOutFixedAmoutCurrentTime(void)
{
    return IceWaterOut.FixedAmoutCurrentTime;
}

void SetIceWaterOutLastedTime(U32 time)
{
    IceWaterOut.LastedTime = time;;
}

U32 GetIceWaterOutLastedTime(void)
{
    return IceWaterOut.LastedTime;
}

void SetIceWaterOutDailyTime(U32 time)
{
    IceWaterOut.DailyTime = time;
}

U32 GetIceWaterOutDailyTime(void)
{
    return IceWaterOut.DailyTime;
}

void SetIceWaterOutTotalTime(U32 time)
{
    IceWaterOut.TotalTime = time;
}

U32 GetIceWaterOutTotalTime(void)
{
    return IceWaterOut.TotalTime;
}

void SetIceWaterOutTime(U8 type, U32 time)
{
    if( type == TYPE_ICE_WATER_LASTED )
    {
        IceWaterOut.LastedTime = time;
    }
    else if( type == TYPE_ICE_WATER_DAILY )
    {
        IceWaterOut.DailyTime = time;
    }
    else
    {
        IceWaterOut.TotalTime = time;
    }
}

U32 GetIceWaterOutTime(U8 type)
{
    U32 mu32Time = 0;

    if( (type & TYPE_ICE_WATER_LASTED) != 0 )
    {
        mu32Time += IceWaterOut.LastedTime;
    }

    if( (type & TYPE_ICE_WATER_DAILY) != 0 )
    {
        mu32Time += IceWaterOut.DailyTime;
    }

    if( (type & TYPE_ICE_WATER_TOTAL) != 0 )
    {
        mu32Time += IceWaterOut.TotalTime;
    }

    return mu32Time;
}

void ResetIceWaterOutTime(U8 type)
{
    if( (type & TYPE_ICE_WATER_LASTED) != 0 )
    {
        IceWaterOut.LastedTime = 0;
    }

    if( (type & TYPE_ICE_WATER_DAILY) != 0 )
    {
        IceWaterOut.DailyTime = 0;
    }

    if( (type & TYPE_ICE_WATER_TOTAL) != 0 )
    {
        IceWaterOut.TotalTime = 0;
    }
}

static U8 ConvIceWaterOut_Time2Cup(U32 time, U32 cupTime)
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

U8 GetIceWaterOutLastedCupNum(void)
{
    U8 num = 0U;

    num = ConvIceWaterOut_Time2Cup( 
            IceWaterOut.LastedTime,
            gu32TimePerCup
            );

    return num;
}

U8 GetIceWaterOutDailyCupNum(void)
{
    U8 num = 0U;

    num = ConvIceWaterOut_Time2Cup( 
            IceWaterOut.DailyTime,
            gu32TimePerCup
            );

    return num;
}

U8 GetIceWaterOutTotalCupNum(void)
{
    U8 num = 0U;

    num = ConvIceWaterOut_Time2Cup( 
            IceWaterOut.TotalTime,
            gu32TimePerCup
            );

    return num;   
}

// 얼음물 만수위 기준 / 얼음물 누적 추출양이 2L 이상일 때만 냉수 재정수 되도록 해야 함
// 얼음물 만수위가 미감지되면 냉수 누적 추출양 Clear
#define MAX_WATER_OUT_COUNT       (60000UL)
#define MAX_WATER_OUT_TIME        (5920UL)      // 59.20sec @ 10ms
void SetIceWaterOutAccumulateTime(U32 time)
{
    if( IceWaterOut.AccumulateTime < MAX_WATER_OUT_COUNT )
    {
        IceWaterOut.AccumulateTime += (time);
    }
}

void ClearIceWaterOutAccumulateTime(void)
{
    IceWaterOut.AccumulateTime = 0U;
}

U32 GetIceWaterOutAccumulateTime(void)
{
    return IceWaterOut.AccumulateTime;
}

U8 IsIceWaterOutMaxTime(void)
{
    if( IceWaterOut.AccumulateTime >= MAX_WATER_OUT_TIME )
    {
        return TRUE;
    }

    return FALSE;
}

U8 GetIceWaterOutPercent(void)
{
    return CalcPercent( IceWaterOut.FixedAmoutTargetTime, IceWaterOut.FixedAmoutCurrentTime );
}

U8 GetIceWaterOutAmountPercent(void)
{
    return CalcPercent( IceWaterOut.FixedAmountTarget, IceWaterOut.FixedAmountCurrent );
}

static void UpdateIceWaterOutCurrentAmount(void)
{
    IceWaterOut.FixedAmountCurrent++;
}

void RegisterOutageIceWaterOut(void)
{
    IceWaterOut.FixedAmountCurrent = 0UL;
    
    /// Register Function Call - 추추량 증가 함수
    RegisterFlowMeterId(FLOW_ID_ICE_WATER, UpdateIceWaterOutCurrentAmount);

    /// Start Check Outage...
    StartCheckOutage(OUTAGE_WATER_OUT_DELAY_TIME, OUTAGE_CHECK_TIME_30SEC, OUTAGE_TYPE_ICE_WATER);   
}

void UnRegisterOutageIceWaterOut(void)
{
    /// UnRegister User Usage
    IceWaterOut.FixedAmountCurrent = 0UL;

    /// UnRegister...
    RegisterFlowMeterId(FLOW_ID_ICE_WATER, NULL);

    /// Stop Check Outage
    StopCheckOutage();
}

//U32 GetFixedAmoutTimeTable(U8 mu8select, U8 mu8AmountType)
//{
//    U32 mu32FixedAmountTimeTable;
//
//    mu32FixedAmountTimeTable = gu32FixedAmoutTimeTable[ mu8select ][ mu8AmountType ];
//    
//    return mu32FixedAmountTimeTable;
//}

static void ControlIceWaterOutSelect(void);
static void ControlIceOutContinue(void);
static void ControlIceWaterOutContinue(void);

static void CheckIceWaterOutStopCondition(void);
static void CheckIceWaterOutStopConditionEol(void);
static void ControlIceWaterOutValve(void);
void ControlIceWaterOut(void)
{  
    ControlIceWaterOutSelect();      // 얼음/얼음물 선택 
    ControlIceOutContinue();         // 얼음 정량 추출 기능 
    ControlIceWaterOutContinue();    // 정량 추출 기능 
    CheckIceWaterOutStopCondition(); // 추출 정지 조건 검사
    ControlIceWaterOutValve();       // 얼음물 추출 밸브 제어
}

void ControlIceWaterOutEol(void)
{  
    ControlIceWaterOutSelect();         // 얼음/얼음물 선택 
    ControlIceOutContinue();            // 얼음 정량 추출 기능 
    ControlIceWaterOutContinue();       // 정량 추출 기능 
    CheckIceWaterOutStopConditionEol(); // 추출 정지 조건 검사
    ControlIceWaterOutValve();          // 얼음물 추출 밸브 제어
}

static void  ControlIceWaterOutSelect(void)
{
    if( IceWaterOut.Select == SEL_ICE_WATER )
    {
        if( IsStartIceDoorWater() == TRUE )
        {
            IceWaterOut.ReturnTime  = DEFAULT_RETURN_TIME;
        }

        if( IceWaterOut.ReturnTime == 0 )
        {
            // Ice Water -> Ice
            if( IceWaterOut.Select == SEL_ICE_WATER )
            {
                IceWaterOut.Select = SEL_ICE;
            }
        }
    }
    else if( IceWaterOut.Select == SEL_ICE )
    {
        if( IsOpenIceDoor() == TRUE )
        {
            IceWaterOut.ReturnTime  = DEFAULT_RETURN_TIME;
        }
    }

    if( IceWaterOut.ReturnTime != 0 )
    {
        IceWaterOut.ReturnTime--;
    }
}

#if( CONFIG_USE_FLOW_METER_AT_ICE_WATER == 1 )
static EAmountId_T GetIceWaterOutAmountId(void)
{
    U8 amountType = 0U;

    amountType = GetIceWaterOutAmoutType();

    if( amountType == FIXED_ICE_WATER_AMOUNT_1L  )
    {
        return AMOUNT_ID_ICE_WATER_1000ML;             
    }
    else if( amountType == FIXED_ICE_WATER_AMOUNT_700ML )
    {
        return AMOUNT_ID_ICE_WATER_700ML;
    }

    return AMOUNT_ID_ICE_WATER_500ML;
}
#endif

static void ControlIceOutContinue(void)
{
    U8 amountType = 0U;
    
    amountType = GetIceWaterOutAmoutType();

    if( GetIceOut() == FALSE 
            || IceWaterOut.Continue == FALSE )
    {
        IceWaterOut.FixedIceTargetTime = gu32FixedIceTimeTable[amountType];
        IceWaterOut.FixedIceCurrentTime = 0;
        return;
    }

    if( IceWaterOut.FixedIceCurrentTime < IceWaterOut.FixedIceTargetTime )
    {
        IceWaterOut.FixedIceCurrentTime++;
    }
    else
    {
        // 연속 추출 시간 만료되면 추출 중지...
        CloseIceDoor();
        StopIceOut();
    }
}

static void ControlIceWaterOutContinue(void)
{
#if( CONFIG_USE_FLOW_METER_AT_ICE_WATER == 1 )
    EAmountId_T amountId = 0U;
#else
    U8 amountType = 0U;
#endif

    // 추출 중에는 시간 reload
    if( IsStartIceDoorWater() == TRUE && IceWaterOut.Continue == TRUE )
    {
        IceWaterOut.ContinueReturnTime = DEFAULT_RETURN_TIME;
    }

    // 연속 추출 기능 해제 
    if( IceWaterOut.Continue == TRUE && IceWaterOut.ContinueReturnTime == 0 )
    {
        SetIceWaterOutContinue(FALSE);
        ClearIceWaterOutInfinityAmout();
    }
    else
    {
        if( IceWaterOut.ContinueReturnTime != 0 )
        {
            IceWaterOut.ContinueReturnTime--;
        }
    }

    // 연속 추출 목표 시간 및 현재 시간 계산...
#if( CONFIG_USE_FLOW_METER_AT_ICE_WATER == 1 )

    amountId = GetIceWaterOutAmountId();

    if( GetIceWaterOut() == FALSE 
            || IceWaterOut.Continue == FALSE )
    {
        IceWaterOut.FixedAmountTarget = GetAmountHz(amountId);
        IceWaterOut.FixedAmountCurrent = 0;
        return;
    }

    if( (IsError(ERR_ID_OUTAGE_WATER) == TRUE)
        || (IceWaterOut.FixedAmountCurrent >= IceWaterOut.FixedAmountTarget) )
    {
        // 연속 추출 시간 만료되면 추출 중지...
        CloseIceDoorWaterOut();
        StopIceWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        Sound(SOUND_EFFLUENT_END);
    }


#else
    
    amountType = GetIceWaterOutAmoutType();

    if( GetIceWaterOut() == FALSE 
            || IceWaterOut.Continue == FALSE )
    {
        IceWaterOut.FixedAmoutTargetTime = gu32FixedAmoutTimeTable[amountType];

        // 추가 입수 ( 저수위 조건 )
        if( amountType == FIXED_ICE_WATER_AMOUNT_500ML || amountType == FIXED_ICE_WATER_AMOUNT_1L )
        {
            if( IceWaterOut.Select == SEL_ICE_WATER && GetColdWaterLevel() == LEVEL_LOW )
            {
                IceWaterOut.FixedAmoutTargetTime += gu32LowLevelAddTimeTable;
            }
        }

        IceWaterOut.FixedAmoutCurrentTime= 0;
        return;
    }

    if( IceWaterOut.FixedAmoutCurrentTime < IceWaterOut.FixedAmoutTargetTime )
    {
        IceWaterOut.FixedAmoutCurrentTime++;
    }
    else
    {
        // 연속 추출 시간 만료되면 추출 중지...
        CloseIceDoorWaterOut();
        StopIceWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        Sound(SOUND_EFFLUENT_END);
    }

#endif
}

// 추출 중지 조건 검사
// Precondition 
//  -. 탱크 비움 기능이 아닐 때,
//  -. 물 추출 중일 떄,
static void CheckIceWaterOutStopCondition(void)
{
    // 얼음물 추출 중일 때,
    if( IsStartIceDoorWater() == FALSE )
    {
        return;
    }

    if( GetIceWaterOut() == FALSE )
    {
        return;
    }

    // 탱크 비움 기능이 아닐 때,
    if( GetTankClean() == TRUE )
    {
        return;
    }

#if( CONFIG_STOP_OUT_COLD_LOW_LEVEL == 1 )
    /// 얼음물  추출 중, 냉수 탱크 저수위이면 추출을 중지시킨다.
    /// 얼음물 추출 펌프 소음을 방지하기 위한 조건이다.
    if( IceWaterOut.Select == SEL_ICE_WATER )
    {
        if( GetColdWaterLevel() == LEVEL_LOW )
        {
            CloseIceDoorWaterOut();
            StopIceWaterOut();
            StartDispTimerId(DISP_TIMER_ID_ERROR);
            Sound(SOUND_EFFLUENT_END);
        }
    }
#endif
}

static void CheckIceWaterOutStopConditionEol(void)
{
    // 얼음물 추출 중일 때,
    if( IsStartIceDoorWater() == FALSE )
    {
        return;
    }

    if( GetIceWaterOut() == FALSE )
    {
        return;
    }

    // 탱크 비움 기능이 아닐 때,
    if( GetTankClean() == TRUE )
    {
        return;
    }

#if( CONFIG_STOP_OUT_COLD_LOW_LEVEL == 1 )
    /// 얼음물  추출 중, 냉수 탱크 저수위이면 추출을 중지시킨다.
    /// 얼음물 추출 펌프 소음을 방지하기 위한 조건이다.
    if( IceWaterOut.Select == SEL_ICE_WATER )
    {
        if( GetTankLevel( LEVEL_ID_COLD ) == LEVEL_LOW )
        {
            CloseIceDoorWaterOut();
            StopIceWaterOut();
            StartDispTimerId(DISP_TIMER_ID_ERROR);
            Sound(SOUND_EFFLUENT_END);
        }
    }
#endif
}


/* 얼음물 추출 */
static void ControlIceWaterOutValve(void)
{
    U8 select;

    /* TURN OFF WATER OUT */
    if( GetIceWaterOut() == FALSE )
    {    
        /* ICE WATER */
        TurnOffCirculatePumpIceWaterOut();
        return;
    }

    select = IceWaterOut.Select;

    /* Update Time information */
    if( IceWaterOut.LastedTime < LIMIT_TIME )
    {
        IceWaterOut.LastedTime++;
    }
    if( IceWaterOut.DailyTime < LIMIT_TIME )
    {
        IceWaterOut.DailyTime++;
    }
    if( IceWaterOut.TotalTime < LIMIT_TIME )
    {
        IceWaterOut.TotalTime++;
    }

    /* TURN ON WATER OUT - ICE_WATER */
    if( select == SEL_ICE_WATER )
    {
        TurnOnCirculatePumpIceWaterOut();
    }
    
    /* Updated Lasted Selected Out */
    IceWaterOut.LastedSelect = select;
}
