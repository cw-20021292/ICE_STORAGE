/**
 * File : flow_meter.c
 * 
 * Flow Meter
*/
#include "hw.h"
#include "flow_meter.h"

#include "hal_timer.h"
#include "hal_gpio.h"

// Dictionary
// FLOW : 유량
// TICK : Hz의 작은 단위 ( 100ms )
// HZ 측정 타운터
SFlowMeter_T  FlowMeter;

static void StartFlowMeter(void)
{
    HAL_INTP4_START();
}

static void StopFlowMeter(void)
{
    HAL_INTP4_STOP();
}

void InitFlowMeter(void)
{
    U8 i = 0U;

    MEMSET( (void __FAR *)&FlowMeter, 0, sizeof(SFlowMeter_T) );

    for( i=0; i<FLOW_ID_MAX; i++ )
    {
        FlowMeter.callBack[i] = NULL;
    }

    FlowMeter.checkOutage = FALSE;
    FlowMeter.outage[OUTAGE_TYPE_WATER] = FLOW_SUPPLY;
    FlowMeter.outage[OUTAGE_TYPE_ICE_TRAY] = FLOW_SUPPLY;
    FlowMeter.confOutageTime = OUTAGE_CHECK_TIME_30SEC;
    FlowMeter.checkOutageTime = OUTAGE_CHECK_TIME_30SEC;

    StartFlowMeter();
}

void GetFlowMeterData(SFlowMeter_T *data)
{
    MEMCPY( (void __FAR *)data, (const void __FAR *)&FlowMeter, sizeof(SFlowMeter_T) );
}

void StartCheckOutage(U16 waitTime, U16 checkTime, EOutageType_T type)
{
    FlowMeter.checkOutage = TRUE;            // start check!!

    FlowMeter.waitTime = waitTime;           // Stable wait time..
    FlowMeter.confOutageTime = checkTime;    // check time
    FlowMeter.type = type;

    FlowMeter.checkOutageTime = checkTime;
}

void StopCheckOutage(void)
{
    FlowMeter.checkOutage = FALSE;
}

U8 GetCheckOutage(void)
{
    return FlowMeter.checkOutage;
}

// Outage
EOutageStatus_T GetOutageStatus(EOutageType_T type)
{
    return FlowMeter.outage[type];
}

void SetOutageStatus(EOutageType_T type, EOutageStatus_T status)
{
    FlowMeter.outage[type] = status;
}

U8 IsOutage(void)
{
    if( FlowMeter.outage[OUTAGE_TYPE_WATER] == TRUE )
    {
        return TRUE;
    }

    return FALSE;
}

// Type
EOutageType_T GetOutageType(void)
{
    return FlowMeter.type;
}

void SetOutageType(EOutageType_T type)
{
    FlowMeter.type = type;
}

// Return Hz
U32 GetFlowMeterHz(void)
{
    return FlowMeter.hz;
}

// Return LPM
F32 GetFlowMeterLPM(void)
{
    return FlowMeter.lpm;
}

// Return CC
U16 GetFlowMeterCc(void)
{
    U16 cc = 0U;

    cc  = (U16)(FlowMeter.lpm  * 1000.0f);
    return cc;
}

F32 GetFlowMeterLastedLPM(void)
{
    return FlowMeter.lastedLpm;
}

void RegisterFlowMeterId(EFlowId_T id,  void (*pCallBack)(void) )
{
    INTS_BYTE_BASE_TIMER();
    INTS_BYTE_FLOW_METER();

    ENTER_CRITICAL_SECTION_TIMER();
    {
        ENTER_CRITICAL_SECTION_FLOW_METER();
        FlowMeter.callBack[id] = pCallBack;
        EXIT_CRITICAL_SECTION_FLOW_METER();
    }
    EXIT_CRITICAL_SECTION_TIMER();
}

// Hz to LPM
F32 ConvHz2Lpm(U32 hz)
{
    F32 lpm = 0.0f;

    /* LPM = (Hz * 60sec ) / (HZ_AMOUNT_10ML * 100 ) */
    lpm = ((U32)hz * 60.0f) / (AMOUNT_HZ_10ML * 100.0f);

    return lpm;
}

// LPM to Hz
U16 ConvLpm2Hz(F32 lpm)
{
    U16 hz;

    /* ((LPM * 100ml) / 60sec ) * AMOUNT_10 */
    hz = (U16)(((lpm * 100.0f) / 60.0f) * (F32)AMOUNT_HZ_10ML);

    return hz;
}

// Tick : 100ms..
// 유량 계산
#if( CONFIG_TEST_FLOW_METER == 1 )
U16 dbg_flow_hz = 0;
#endif
static void CalcFlowMeterHz(void)
{
    U8 i = 0U;
    U32 sum = 0U;
    U32 tickHz = 0U;

    INTS_BYTE_FLOW_METER();
    INTS_BYTE_BASE_TIMER();

    ENTER_CRITICAL_SECTION_TIMER();
    {
        ENTER_CRITICAL_SECTION_FLOW_METER();
        tickHz = FlowMeter.tickHz;
        FlowMeter.tickHz = 0;
        EXIT_CRITICAL_SECTION_FLOW_METER();
    }
    EXIT_CRITICAL_SECTION_TIMER();

    FlowMeter.tickHzList[FlowMeter.indexList] = tickHz;
    FlowMeter.indexList++;
    if( FlowMeter.indexList >= TICK_HZ_NUM )
    {
        FlowMeter.indexList = 0;
    }

    sum = 0; 
    for( i=0; i<TICK_HZ_NUM; i++ )
    {
        sum += FlowMeter.tickHzList[i];
    }

    // Update Hz, Flow Rate(LPM)
    FlowMeter.hz = sum;
#if( CONFIG_TEST_FLOW_METER == 1 )
    FlowMeter.hz += dbg_flow_hz;
#endif
    FlowMeter.lpm = ConvHz2Lpm(FlowMeter.hz);

    if( FlowMeter.lpm != 0 )
    {
        FlowMeter.lastedLpm = FlowMeter.lpm;
    }
}

// Prevent interference noise...
// Call by 1ms routin.....
void FilterFlowMeterNoise(void)
{
    if( FlowMeter.sync == TRUE )
    {
        FlowMeter.sync = FALSE;

        StartFlowMeter();
    }
}

// PIN INTERRUPT ISR
// DETECTING FLOW SENSOR INPUT...
void CheckFlowMeterIsr(void)
{
    U8 i = 0U;

    FlowMeter.sync = TRUE;

    FlowMeter.tickHz++;
    
    for( i=0; i<FLOW_ID_MAX; i++ )
    {
        if( FlowMeter.callBack[i] != NULL )
        {
            FlowMeter.callBack[i]();
        }
    }

    StopFlowMeter();
}

// 단수 검사 로직
static void CheckOutageStatus(void)
{
    if( FlowMeter.checkOutage == FALSE )
    {
        // Reset check timer..
        FlowMeter.checkOutageTime = FlowMeter.confOutageTime;
        return ;
    }

    // Wait stable time..
    if( FlowMeter.waitTime != 0 )
    {
        FlowMeter.waitTime--;

        return ;
    }

    // Check..
    if( FlowMeter.hz <= OUTAGE_WATER_HZ )
    {
        if( FlowMeter.checkOutageTime != 0 )
        {
            FlowMeter.checkOutageTime--;
        }
        else
        {
            // timeout outage!!!
            StopCheckOutage();

            FlowMeter.outage[FlowMeter.type] = FLOW_OUTAGE;
        }
    }
    else
    {
        // Reset check timer
        FlowMeter.checkOutageTime = FlowMeter.confOutageTime;
    }
}

// @100ms..
void ProcessFlowMeter(void)
{
    CalcFlowMeterHz();

    CheckOutageStatus();
}


#if( CONFIG_TEST_FLOW_METER == 1 )
#include "water_out.h"
#include "ice_door.h"
#include "valve.h"
#include "drain_water.h"
U8 dbg_the_flow_test = 0;
#endif
void TestAutoCountingFlowMeter(void)
{
#if( CONFIG_TEST_FLOW_METER == 1 )
    if( dbg_the_flow_test != 0 )
    {
        return ;
    }

    if( ((GetWaterOut() == TRUE || IsStartIceDoorWater() == TRUE ) &&
            (
             IsOpenValve( VALVE_FEED ) == TRUE 
             || IsOpenValve( VALVE_ROOM_OUT ) == TRUE 
             || IsOpenValve( VALVE_COLD_IN ) == TRUE 
             || IsOpenValve( VALVE_HOT_OUT ) == TRUE
             || IsOpenValve( VALVE_COLD_CIRCULATE ) == TRUE
             || IsOpenValve( VALVE_ICE_WATER_OUT ) == TRUE
            )
            ||
            (
            IsOpenValve( VALVE_ICE_TRAY_IN ) == TRUE 
            || IsStartDrainWater() == TRUE
            )
        )
      )
    {
        CheckFlowMeterIsr();
        return ;
    }

#endif
}
