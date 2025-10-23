/**
 * File : hotgas_switch_valve.h
 * 
 * HotGas Switch Valve
*/
#ifndef __HOTGAS_SWITCH_VALVE_H__
#define __HOTGAS_SWITCH_VALVE_H__

#include "prj_type.h"

typedef struct _hotgas_switch_valve_
{
    /* STATUS */
    U8 Status;       // Open or Close
    U8 OnInit;      // 밸브 초기화 동작을 해야함.
    
    /* MODE */
    U8 Mode;         // Init / Close / Open 
    U8 InitStep;
    U8 NormalStep;
    U8 HotStep;

    U8 PowerOnTime; // SMPS ON WAIT TIMEER @100ms
} SHotGasSwitchValve_T;

void InitHotGasSwitch(void);

void GetHotGasSwitchData(SHotGasSwitchValve_T *pData);

#define HOTGAS_SWITCH_BUSY    (0)
#define HOTGAS_SWITCH_NORMAL  (1)
#define HOTGAS_SWITCH_HOT     (2)
U8 GetHotGasSwitchStatus(void);

/* MODE */
#define HOTGAS_SWITCH_MODE_NONE           (0x00)
#define HOTGAS_SWITCH_MODE_INIT           (0x01)
#define HOTGAS_SWITCH_MODE_NORMAL         (0x02)
#define HOTGAS_SWITCH_MODE_HOT            (0x04)
#define HOTGAS_SWITCH_MODE_POWER_INIT     (0x08)    
U8 GetHotGasSwitchMode(void);
U8 IsHotGasSwitchMoving(void);

void HotGasSwitchInit(void);
void HotGasSwitchNormal(void);
void HotGasSwitchHot(void);

void ControlHotGasSwitch(void);
void ProcessHotGasSwitch(void);

// 핫가스 냉매 전환 밸브 테스트 코드
typedef struct _hotgas_switch_repeat_test_
{
    U8 Run;
    U8 Delay;
    U32 RepeatCount;
} SHotGasSwitchValveTest_T;

void TEST_GetHotGasValveData(SHotGasSwitchValveTest_T *pData);
void TEST_StartHotGasValve(void);
void TEST_StopHotGasValve(void);
void TEST_RepeatHotGasValve(void);

#endif /* __HOTGAS_SWITCH_VALVE_H__ */
