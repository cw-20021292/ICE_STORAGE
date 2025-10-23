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
    U8 Cmd;           // ���� ���� ���� ��� ( 1: ON, 0: OFF )
    U8 Cmd_MakeCold;  // �ð� ���۽� ���� ��� ( 1: ON, 0: OFF )
    U8 Cmd_MakeColdForced;  // �ð� ���۽� ���� ��� ( 1: ON, 0: OFF )
    U8 Cmd_WaterOut;  // �ü� ����� ���� ��� ( 1: ON, 0: OFF )
    U8 Cmd_TrayIn;    // ���� �� Ʈ���� �Լ� �� ���� ��� ( 1: ON, 0: OFF )
    U8 Cmd_IceWaterOut;   // ���� �� ���� �� ���� ��� ( 1: ON, 0: OFF )

    /* �ݺ� ���� ��� */
    U8 RetryCmd;
    U8 RetryCount;
    U16 OnOffTime;     // 1sec

    U8 Error;         // ���� ���� ���� ( 0: ����, 1: ���� )
    U8 ErrorCount;    // ���� ī��Ʈ 
    U16 CheckDelayTime;  // ���� ���� �˻� ������ ���� �ð� @10ms
    U16 CheckOffTime;     // ���� ���� �߻��� command off time
    U16 CheckNormalTime;  // ���� ���� �˻� Ÿ�̸�
    U16 CheckErrorTime;   // ���� ���� �˻� Ÿ�̸�

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
