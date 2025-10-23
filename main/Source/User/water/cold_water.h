#ifndef __COLD_WATER_H__
#define __COLD_WATER_H__

#include "prj_type.h"
#include "level.h"
#include "comp_bldc.h"

/* COLD IN EXTRA FEED TIME WHEN HIGH LEVEL */
#define EXTRA_FEED_TIME    10U /* 1sec @100ms */
//#define EXTRA_FEED_TIME    180U /* 18sec @100ms */

typedef struct _cold_water_
{
    U8 InitFull;
    U8 Level;
    
    /* MAKE */
    U8 ConfigMake;
    U8 Make;

    /* MODE */
    U8 Mode;

    /* TEMP */
    U8      StartingMode;
    U16     StartingModeTime;

    U8      RegionAmbi;

    TEMP_T  TempTargetOn;
    TEMP_T  TempTargetOff;
    TEMP_T  TempCurrent;

    U8      RegionCold;
    RPS_T   TargetRps;

    U8 TempLevel;       // 냉수 온도 표시 kdh

    U16 FeedTime;   
    U16 ExtraFeedTime;  // 만수위 감지 후, 추가 입수 시간

    U8 ExtraMake;       // 외기 온도 30'C 이상 조건에서 추가 제빙 여부
    U16 ExtraMakeTime;  // 외기 온도 30'C 이상 조건에서 추가 제빙 시간

    U8 FanOnOff;
} ColdWater_T;

// 초기화
void  InitColdWater(void);
void  GetColdWaterData(ColdWater_T *pData);

// 냉수 최초 만수위
void  SetColdWaterInitFull(U8 mu8Val );
U8    GetColdWaterInitFull(void);

// 냉수 수위 
void  SetColdWaterLevel(U8 mu8Val );
U8    GetColdWaterLevel(void);

// 냉수 생성 설정
void  SetColdWaterConfigMake(U8 mu8Val );
U8    GetColdWaterConfigMake(void);

// 냉수 생성 명령
void  SetColdWaterMake(U8 mu8Val );
U8    GetColdWaterMake(void);

// 냉수 입수 지연 시간
void  SetColdWaterFeedTime(U16 mu16Time );
U16   GetColdWaterFeedTime(void);

// 냉수 제어 상세 모드
#define  COLD_MODE_NONE          0x00
#define  COLD_MODE_INIT          0x01
#define  COLD_MODE_COOLING       0x02
#define  COLD_MODE_DONE          0x08
#define  COLD_MODE_ALL           0xFF
void SetColdWaterMakeMode(U8 mu8Mode );
void ClearColdWaterMakeMode(U8 mu8Mode );
U8 IsSetColdWaterMakeMode(U8 mu8Mode );
U8 GetColdWaterMakeMode(void );

U8 GetColdWaterMakeFan(void);

RPS_T GetColdWaterMakeCompRps(void);

// 냉수 입수 지연 시간
void  SetColdWaterExtraMakeTime(U16 mu16Time );
U16   GetColdWaterExtraMakeTime(void);

// 냉수 탱크 수위 제어
void  ControlColdWaterLevel(void);

// 냉수 생성 조건 제어
void  MakeColdWater(void);

#endif /* __COLD_WATER_H__ */
