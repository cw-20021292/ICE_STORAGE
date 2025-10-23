/**
 * File : hot_water.h
 * 
 * Hot Water
*/
#ifndef __HOT_WATER_H__
#define __HOT_WATER_H__

#include "prj_type.h"

#include "level.h"

/* ALTITUDE */
#define  ALTITUDE_LEVEL_0     (0)
#define  ALTITUDE_LEVEL_1     (1)
#define  ALTITUDE_LEVEL_2     (2)
#define  ALTITUDE_LEVEL_NUM   (3)

typedef struct _hot_water_
{
    U8 InitWaitTime;    // 최초 대기 시간
    U8 InitFull;        // 탱크 최초 만수위 ( TRUE )
    U8 Level;           // 수위 레벨
    
    U8 ConfigMake;      // 사용 설정
    U8 Make;            // 온수 생성 여부
    U16 MakeDelay;      // 온수 생성 지연 시간
    U16 MakeDelayTankOpen; // 탱크 OPEN 시 온수 생성 지연 시간

    U8 TempLevel;       // 온수 온도 표시 kdh

    /* TEMP */
    TEMP_T  TempTargetOn;     // 히터 ON 온도 기준치
    TEMP_T  TempTargetOff;    // 히터 OFF 온도 기준치  
    TEMP_T  TempCurrent;      // 히터 현재 온도

    /* Target Temperetrue as mode */
    U8 Altitude;              // 히터 고도 레벨 

    /* 에어 빼기 */
    U8 AirFlowStatus;       // 동작 가능 여부 
    U8 IsAirFlow;           // 최초 입수시 에어 빼기 동작 
    U8 CheckAir;            
    U16 AirFlowTime;        // 에어 빼기 동작 시간
} HotWater_T;

void  InitHotWater(void);
void  GetHotWaterData(HotWater_T *pData);

void  SetHotWaterInitFull(U8 val);
U8    GetHotWaterInitFull(void);

void  SetHotWaterLevel(U8 val);
U8    GetHotWaterLevel(void);

void  SetHotWaterConfigMake(U8 val);
U8    GetHotWaterConfigMake(void);

void  SetHotWaterMake(U8 val);
U8    GetHotWaterMake(void);

void  SetHotWaterAltidue(U8 val);
U8    GetHotWaterAltidue(void);

void  UpdateHotMakeDelay(void);
void  ClearHotMakeDelayTankOpen(void);
void  UpdateHotAirFlowTime(void);

void  ControlHotWaterLevel(void);
TEMP_T GetHotWaterTargetOffTemp(void);
void  MakeHotWater(void);

/* AIR FLOW */
void StartHotWaterAirFlow(void);

#endif /* __HOT_WATER_H__ */
