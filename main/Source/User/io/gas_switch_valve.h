/**
 * File : gas_switch_valve.h
 * 
 * Gas Switch Valve
*/
#ifndef __GAS_SWITCH_VALVE_H__
#define __GAS_SWITCH_VALVE_H__

#include "prj_type.h"

typedef struct _gas_switch_valve_
{
    /* STATUS */
    U8 Status;       // Open or Close
    U8 OnInit;      // 밸브 초기화 동작을 해야함.
    
    /* MODE */
    U8 Mode;         // Init / Close / Open 
    U8 InitStep;
    U8 ColdStep;
    U8 IceStep;
    U8 CloseStep;

    U8 PowerOnTime; // SMPS ON WAIT TIMEER @100ms
} SGasSwitchValve_T;

void InitGasSwitch(void);

void GetGasSwitchData(SGasSwitchValve_T *pData);

#define GAS_SWITCH_BUSY    (0)
#define GAS_SWITCH_COLD    (1)
#define GAS_SWITCH_ICE     (2)
#define GAS_SWITCH_CLOSE   (3)
U8 GetGasSwitchStatus(void);

/* MODE */
#define GAS_SWITCH_MODE_NONE           (0x00)
#define GAS_SWITCH_MODE_INIT           (0x01)
#define GAS_SWITCH_MODE_COLD           (0x02)
#define GAS_SWITCH_MODE_ICE            (0x04)
#define GAS_SWITCH_MODE_CLOSE          (0x08)
//#define GAS_SWITCH_MODE_POWER_INIT     (0x10)
U8 GetGasSwitchMode(void);
U8 IsGasSwitchMoving(void);

void GasSwitchInit(void);
void GasSwitchCold(void);
void GasSwitchIce(void);
void GasSwitchClose(void);

void ControlGasSwitch(void);
void ProcessGasSwitch(void);

// 냉매 전환 밸브 테스트 코드
typedef struct _gas_switch_repeat_test_
{
    U8 Run;
    U8 Delay;
    U32 RepeatCount;
} SGasSwitchValveTest_T;

void TEST_GetGasValveData(SGasSwitchValveTest_T *pData);
void TEST_StartGasValve(void);
void TEST_StopGasValve(void);
void TEST_RepeatGasValve(void);

#endif /* __GAS_SWITCH_VALVE_H__ */
