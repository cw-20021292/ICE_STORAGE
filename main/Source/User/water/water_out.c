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

#define  CONFIG_STOP_OUT_COLD_LOW_LEVEL   (1)     /// �ü� ������ ���� �̰���, ���� ����

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
    U8 Continue;               // ���� ���� (TRUE or FALSE)
    U16 ContinueReturnTime;     // ���� ���� ���� �ð� 
    U8 InfinityAmout;          // ���� ���� ����/����
    U8 FixedAmoutType;         //  FIXED_AMOUNT_1L, FIXED_AMOUNT_1D5L
    U32 FixedAmoutTargetTime;   // @10msec 
    U32 FixedAmoutCurrentTime;  // @10msec

    /* Water Out Time */
    U32 LastedTime[ MAX_WATER_ID ];  // ������ ���� �ð� @10msec
    U32 DailyTime[ MAX_WATER_ID ];   // ���� ���� �ð� @10msec
    U32 TotalTime[ MAX_WATER_ID ];   // ��ü ���� �ð� @10msec

    U32 AccumulateTime[ MAX_WATER_ID ]; // ���� �� ���� �ð�
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
//  TANK CLEAN - 15min ( ���� ����ð� 5�� 48�� )
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

/* ���� ���� */
void  StartWaterOut(void)
{
    if( Out.WaterOut == FALSE )
    {
        Out.LastedTime[ Out.Select ] = 0;
    }
    Out.WaterOut = TRUE;
}

/* ���� ���� */
void  StopWaterOut(void)
{
    Out.WaterOut = FALSE;
    SetTankClean( FALSE );

#if 0

  // IGROO

    SetIoCareMode( IOC_ID_USE_WATER_DATA );
#endif
}

/* ���� ���� ��ȯ
 * Return
 *  -TRUE : ���� ��
 *  -FALSE : ���� ���� ����
 * */
U8 GetWaterOut(void)
{
    return Out.WaterOut;
}


/* ���� ���� ����/���� */
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

// �ü� ������ ���� / �ü� ���� ������� 2L �̻��� ���� �ü� ������ �ǵ��� �ؾ� ��
// �ü� �������� �̰����Ǹ� �ü� ���� ����� Clear
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
    ControlWaterOutSelect();      // ��/��/�� ���� 
    ControlWaterOutContinue();    // ���� ���� ��� 
    CheckWaterOutStopCondition(); // ���� ���� ���� �˻�
    ControlTankClean();
    ControlWaterOutValve();       // ��/��/�� ���� ��� ����
}

void ControlWaterOutEol(void)
{
    ControlWaterOutSelect();         // ��/��/�� ���� 
    ControlWaterOutContinue();       // ���� ���� ��� 
    CheckWaterOutStopConditionEol(); // ���� ���� ���� �˻�
    ControlTankClean();
    ControlWaterOutValve();          // ��/��/�� ���� ��� ����
}

static void  ControlWaterOutSelect(void)
{

    /* 1. �¼� ���� ��  7sec �Ŀ� �ٽ� �ü� �Ǵ� ������ �� ������ ���� ��Ų��.
     * 2. ��, �ü� ����� �����Ǿ� �ִ� ��쿡�� ������ �����Ѵ�.
     * 3. �ü� ���� �� �ü� ����� �����Ǿ� �ִ� ��쿡�� ������ �� ������ ���� ��Ų��.
     *
     * ���� ���� ���� ��쿡��, �� ���� ���Ŀ� 5�� Ÿ�̸Ӱ� ����� �� �ֵ���
     * Ÿ�̸Ӹ� reset �Ѵ�.
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

    // ���� �߿��� �ð� reload
    if( Out.WaterOut == TRUE && Out.Continue == TRUE )
    {
        Out.ContinueReturnTime = DEFAULT_RETURN_TIME;
    }

    // ���� ���� ��� ���� 
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

    // ���� ���� ��ǥ �ð� �� ���� �ð� ���...
    amoutType = GetWaterOutAmoutType();
    if( Out.WaterOut == FALSE 
            || Out.Continue == FALSE )
    {
        Out.FixedAmoutTargetTime = gu32FixedAmoutTimeTable[ Out.Select ][amoutType];

        // �߰� �Լ� ( ������ ���� )
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

    // ��ũ ���� ���, ���� ��ǥ �ð� ���� �缳��
    if( GetTankClean() == TRUE )
    {
        SetColdWaterFeedTime(0);      // �ü� ������ �ð� �ʱ�ȭ
        SetRoomWaterInitFull(TRUE);   // �ü� �Լ��� ���ؼ� ���� ���� ������ ���� ����
        Out.FixedAmoutTargetTime = gu32FixedAmoutTimeTable[ Out.Select ][FIXED_AMOUNT_TANK_CLEAN];
    }

    if( Out.FixedAmoutCurrentTime < Out.FixedAmoutTargetTime )
    {
        Out.FixedAmoutCurrentTime++;
    }
    else
    {
        // ���� ���� �ð� ����Ǹ� ���� ����...
        StopWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        Sound( SOUND_EFFLUENT_END );
    }
}

// ���� ���� ���� �˻�
// Precondition 
//  -. ��ũ ��� ����� �ƴ� ��,
//  -. �� ���� ���� ��,
static void CheckWaterOutStopCondition(void)
{
    // �� ���� ���� ��,
    if( Out.WaterOut == FALSE )
    {
        return ;
    }

    // ��ũ ��� ����� �ƴ� ��,
    if( GetTankClean() == TRUE )
    {
        return ;
    }

#if( CONFIG_STOP_OUT_COLD_LOW_LEVEL == 1 )
    // �ü� ���� ��, �ü� ��ũ �������̸� ������ ������Ų��.
    // �ü� ���� ���� ������ �����ϱ� ���� �����̴�.
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

// ���� ���� ���� �˻�
// Precondition 
//  -. ��ũ ��� ����� �ƴ� ��,
//  -. �� ���� ���� ��,
static void CheckWaterOutStopConditionEol(void)
{
    // �� ���� ���� ��,
    if( Out.WaterOut == FALSE )
    {
        return ;
    }

    // ��ũ ��� ����� �ƴ� ��,
    if( GetTankClean() == TRUE )
    {
        return ;
    }

#if( CONFIG_STOP_OUT_COLD_LOW_LEVEL == 1 )
    // �ü� ���� ��, �ü� ��ũ �������̸� ������ ������Ų��.
    // �ü� ���� ���� ������ �����ϱ� ���� �����̴�.
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
