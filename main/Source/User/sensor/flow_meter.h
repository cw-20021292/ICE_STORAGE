/**
 * File : flow_meter.h
 *
 * Flow Meter
*/
#ifndef __FLOW_METER_H__
#define __FLOW_METER_H__

#include "prj_type.h"

/* MIN OUTAGE VALUE */
// 39hz = 0.252 LPM
// 40hz = 0.258 LPM
// 48hz = 0.310 LPM
#define OUTAGE_WATER_HZ    (40)

/**
 * 유량 센서 사양
 * 정격 : 3.8VDC ~ 24VDC, 20mA이하
 * 동작 유량 범위 : 0.3~1.2 [Liter / Min]
 * 유량 ([Liter / Min])에 따른 주파수 특성 : 
  - 0.3[Liter / Min] = 48.9hz(+-15%)
  - 0.5[Liter / Min] = 79.8hz(+-10%)
  - 1.0[Liter / Min] = 154.9hz(+-10%)   => 1/60 [Liter/Sec] =154.9[Rvolutions/Sec]
 * 1.0 Liter Rvolutions(회전수, Cycle)  : 1/60 Liter = 154.9 Rvolutions : 1Liter = 60*154.9(9294) Rvolutions
 * 1000ml = 9294 Rvolutions : 1ml = 9.294 Rvolutions
 * 사용 주위 온도 : -10도 ~ 60도 ( 단, 액체가 얼지 않을 것 )
 * 사용 유체 온도 : 0 ~ 60도 ( 단, 액체가 얼지 않을 것 )
 * 설치 방향 : 수평
 * 최대 내수압 : 17.5kgf/cm^2 ( 15분간 파손 및 누수 없을 것 )
 * DUTY CYCLE : 30~70%
**/

// Default Flow sensor Amount @ COUNTER TICK
#define AMOUNT_HZ_1ML          (9.294f)

#define AMOUNT_HZ_5ML          (U32)(AMOUNT_HZ_1ML * 5.0f)
#define AMOUNT_HZ_10ML         (U32)(AMOUNT_HZ_1ML * 10.0f)
#define AMOUNT_HZ_60ML         (U32)(AMOUNT_HZ_1ML * 60.0f)
#define AMOUNT_HZ_200ML        (U32)(AMOUNT_HZ_1ML * 200.0f)
#define AMOUNT_HZ_250ML        (U32)(AMOUNT_HZ_1ML * 250.0f)
#define AMOUNT_HZ_350ML        (U32)(AMOUNT_HZ_1ML * 350.0f)
#define AMOUNT_HZ_500ML        (U32)(AMOUNT_HZ_1ML * 500.0f)
#define AMOUNT_HZ_600ML        (U32)(AMOUNT_HZ_1ML * 600.0f)
#define AMOUNT_HZ_650ML        (U32)(AMOUNT_HZ_1ML * 650.0f)
#define AMOUNT_HZ_700ML        (U32)(AMOUNT_HZ_1ML * 700.0f)
#define AMOUNT_HZ_750ML        (U32)(AMOUNT_HZ_1ML * 750.0f)
#define AMOUNT_HZ_1000ML       (U32)(AMOUNT_HZ_1ML * 1000.0f)
#define AMOUNT_HZ_1500ML       (U32)(AMOUNT_HZ_1ML * 1500.0f)

// Outage
// Outage 검사 시간 설정 @ 100ms...
#define OUTAGE_CHECK_TIME_1SEC      (10UL)    /// 1sec
#define OUTAGE_CHECK_TIME_2SEC      (20UL)    /// 2sec
#define OUTAGE_CHECK_TIME_3SEC      (30UL)    /// 3sec
#define OUTAGE_CHECK_TIME_10SEC     (100UL)   /// 10sec
#define OUTAGE_CHECK_TIME_30SEC     (300UL)   /// 30sec
#define OUTAGE_CHECK_TIME_60SEC     (600UL)   /// 60sec
#define OUTAGE_CHECK_TIME_FLUSHING  (600UL)   /// 60sec

// Outage Status
typedef enum _outage_status_
{
    FLOW_SUPPLY,
    FLOW_OUTAGE
} EOutageStatus_T;
#define OUTAGE_HOT_OUT_DELAY_TIME     (1U)     /// 100msec @100ms
#define OUTAGE_WATER_OUT_DELAY_TIME   (50U)    /// 5sec @100ms
#define OUTAGE_DELAY_TIME             (300U)   /// 30sec @100ms..
#define OUTAGE_FLUSHING_DELAY_TIME    (3000U)  /// 300sec @100ms..

// Outage Type
typedef enum _outage_type_
{
    OUTAGE_TYPE_WATER = 0,  /// 0 :
    OUTAGE_TYPE_ICE_TRAY,   /// 0 :
    OUTAGE_TYPE_ICE_WATER,  /// 0 :
    OUTAGE_TYPE_MAX,        /// 0 :
} EOutageType_T;

// Flow meter id
typedef enum _flow_id_
{
    FLOW_ID_WATER = 0,    /// 0 :
    FLOW_ID_ICE_TRAY,     /// 0 :
    FLOW_ID_ICE_WATER,    /// 0 :    
    FLOW_ID_MAX,          /// 0 :
} EFlowId_T;

#define TICK_HZ_NUM        (10)
typedef struct _flow_meter_
{
   // Hz
    U8 sync;
    U8 indexList;
    U32 tickHzList[TICK_HZ_NUM];
    U32 tickHz;  // 100ms...

    U32 hz;
    F32 lpm;
    F32 lastedLpm;

    // Outage
    U8 checkOutage;                             /// TRUE or FALSE ( START or STOP )
    EOutageStatus_T outage[OUTAGE_TYPE_MAX];    /// SUPPLY or OUTAGE
    EOutageType_T   type;
    U16             waitTime;                   /// Stable Timer..

    U16 confOutageTime;           /// 단수 검사 설정 시간 ( 물 종류에 따라 다름 )
    U16 checkOutageTime;          /// Check Timer..

    // ISR - FLOW METER PIN INTERRUPT
    void (*callBack[FLOW_ID_MAX])(void);         // WATER OUT
} SFlowMeter_T;

void InitFlowMeter(void);

void GetFlowMeterData(SFlowMeter_T *data);

void StartCheckOutage(U16 waitTime, U16 checkTime, EOutageType_T type);
void StopCheckOutage(void);
U8 GetCheckOutage(void);
EOutageStatus_T GetOutageStatus(EOutageType_T type);
void SetOutageStatus(EOutageType_T type, EOutageStatus_T status);
U8 IsOutage(void);
EOutageType_T GetOutageType(void);
void SetOutageType(EOutageType_T type);

U32 GetFlowMeterHz(void);
F32 GetFlowMeterLPM(void);
U16 GetFlowMeterCc(void);
F32 GetFlowMeterLastedLPM(void);

void RegisterFlowMeterId(EFlowId_T id,  void (*pCallBack)(void) );
F32 ConvHz2Lpm(U32 hz);
U16 ConvLpm2Hz(F32 lpm);

void FilterFlowMeterNoise(void);
void CheckFlowMeterIsr(void);

void ProcessFlowMeter(void);

void TestAutoCountingFlowMeter(void);

#endif  /* __FLOW_METER_H__ */
