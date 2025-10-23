/**
 * File : circulate_pump.h
 * 
 * Circulate Pump
*/
#ifndef __CIRCULATE_PUMP_H__
#define __CIRCULATE_PUMP_H__

#include "prj_type.h"

typedef struct _circulate_pump_
{
    U8 Cmd;           // 실제 펌프 제어 명령 ( 1: ON, 0: OFF )
    U8 Cmd_MakeCold;  // 냉각 동작시 제어 명령 ( 1: ON, 0: OFF )
    U8 Cmd_MakeColdForced;  // 냉각 동작시 제어 명령 ( 1: ON, 0: OFF )
    U8 Cmd_WaterOut;  // 냉수 추출시 제어 명령 ( 1: ON, 0: OFF )
    U8 Cmd_TrayIn;    // 제빙 시 트레이 입수 시 제어 명령 ( 1: ON, 0: OFF )
    U8 Cmd_IceWaterOut;   // 얼음 물 추출 시 제어 명령 ( 1: ON, 0: OFF )

    /* 반복 제어 모드 */
    U8 RetryCmd;
    U8 RetryCount;
    U16 OnOffTime;     // 1sec

    U8 Error;         // 펌프 에러 상태 ( 0: 정상, 1: 에러 )
    U8 ErrorCount;    // 에러 카운트 
    U16 CheckDelayTime;  // 펌프 에러 검사 시작전 지연 시간 @10ms
    U16 CheckOffTime;     // 펌프 에러 발생시 command off time
    U16 CheckNormalTime;  // 정상 조건 검사 타이머
    U16 CheckErrorTime;   // 에러 조건 검사 타이머

    U8 Purge;             /// Purge Command
    U8 Cycle;             /// Purge Cycle
    U8 PurgeOnTime;       /// Purge On Time
    U8 PurgeOffTime;      /// Purge Off Time
} SCirculatePump_T;

void InitCirculatePump(void);
void GetCirculatePumpData(SCirculatePump_T *pData);

U8 GetCirculatePumpStatus(void);
U8 GetCirculatePumpError(void);

void TurnOnCirculatePumpMakeCold(void);
void TurnOffCirculatePumpMakeCold(void);

void TurnOnCirculatePumpMakeColdForced(void);
void TurnOffCirculatePumpMakeColdForced(void);

void TurnOnCirculatePumpWaterOut(void);
void TurnOffCirculatePumpWaterOut(void);

void TurnOnCirculatePumpTrayIn(void);
void TurnOffCirculatePumpTrayIn(void);

void TurnOnCirculatePumpIceWaterOut(void);
void TurnOffCirculatePumpIceWaterOut(void);

void TurnOnCirculatePump(void);
void TurnOffCirculatePump(void);

void ControlCirculatePump(void);
void ControlCirculatePumpEol(void);

#endif /* __CIRCULATE_PUMP_H__ */
