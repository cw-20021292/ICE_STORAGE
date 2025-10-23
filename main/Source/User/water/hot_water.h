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
    U8 InitWaitTime;    // ���� ��� �ð�
    U8 InitFull;        // ��ũ ���� ������ ( TRUE )
    U8 Level;           // ���� ����
    
    U8 ConfigMake;      // ��� ����
    U8 Make;            // �¼� ���� ����
    U16 MakeDelay;      // �¼� ���� ���� �ð�
    U16 MakeDelayTankOpen; // ��ũ OPEN �� �¼� ���� ���� �ð�

    U8 TempLevel;       // �¼� �µ� ǥ�� kdh

    /* TEMP */
    TEMP_T  TempTargetOn;     // ���� ON �µ� ����ġ
    TEMP_T  TempTargetOff;    // ���� OFF �µ� ����ġ  
    TEMP_T  TempCurrent;      // ���� ���� �µ�

    /* Target Temperetrue as mode */
    U8 Altitude;              // ���� �� ���� 

    /* ���� ���� */
    U8 AirFlowStatus;       // ���� ���� ���� 
    U8 IsAirFlow;           // ���� �Լ��� ���� ���� ���� 
    U8 CheckAir;            
    U16 AirFlowTime;        // ���� ���� ���� �ð�
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
