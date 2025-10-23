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

    U8 TempLevel;       // �ü� �µ� ǥ�� kdh

    U16 FeedTime;   
    U16 ExtraFeedTime;  // ������ ���� ��, �߰� �Լ� �ð�

    U8 ExtraMake;       // �ܱ� �µ� 30'C �̻� ���ǿ��� �߰� ���� ����
    U16 ExtraMakeTime;  // �ܱ� �µ� 30'C �̻� ���ǿ��� �߰� ���� �ð�

    U8 FanOnOff;
} ColdWater_T;

// �ʱ�ȭ
void  InitColdWater(void);
void  GetColdWaterData(ColdWater_T *pData);

// �ü� ���� ������
void  SetColdWaterInitFull(U8 mu8Val );
U8    GetColdWaterInitFull(void);

// �ü� ���� 
void  SetColdWaterLevel(U8 mu8Val );
U8    GetColdWaterLevel(void);

// �ü� ���� ����
void  SetColdWaterConfigMake(U8 mu8Val );
U8    GetColdWaterConfigMake(void);

// �ü� ���� ���
void  SetColdWaterMake(U8 mu8Val );
U8    GetColdWaterMake(void);

// �ü� �Լ� ���� �ð�
void  SetColdWaterFeedTime(U16 mu16Time );
U16   GetColdWaterFeedTime(void);

// �ü� ���� �� ���
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

// �ü� �Լ� ���� �ð�
void  SetColdWaterExtraMakeTime(U16 mu16Time );
U16   GetColdWaterExtraMakeTime(void);

// �ü� ��ũ ���� ����
void  ControlColdWaterLevel(void);

// �ü� ���� ���� ����
void  MakeColdWater(void);

#endif /* __COLD_WATER_H__ */
