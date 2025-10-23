/// @file   Lib_Water_Fan.h
/// @date   2025/07/09
/// @author Jaejin Ham
/// @brief  Fan ���� ���� ���� ��� file

#ifndef _LIB_WATER_FAN_H_
#define _LIB_WATER_FAN_H_


/*  Lib Function ����
    => Set_CoolFanOnOff(mu8OnOff);                  // �ð� Fan ���� ���� ���� - 0(OFF), 1(ON)
    => Get_CoolFanOnOff();                          // �ð� Fan ���� ���� Ȯ�� - 0(OFF), 1(ON)

    => Set_CoolFanVolt(mu8Volt);                    // �ð� Fan ���� ���� Volt ����
    => Get_CoolFanVolt();                           // �ð� Fan ���� ���� Volt Ȯ��
    => Set_CoolFanActMode(mu8Data);                 // �ð� Fan ���� ���� Mode ����
    => Get_CoolFanActMode();                        // �ð� Fan ���� ���� Mode Ȯ��
*/


#define COOL_FAN_CONTROL_TIME_PERIOD            100     // �ð��� ��� ���� ���� �ְ� Ÿ�̸� 100ms@1ms

#define FAN_DELAY_TIME                          10      // �ð��� ���� ��ȯ�� Delay �ð� 1sec@100ms

void Lib_CoolFan_Initialize(void);
void CoolFanControlTimer(void);

#if (DC_FAN_COUNT > 0)
void Set_CoolFanOnOff(U8 mu8OnOff);
U8 Get_CoolFanOnOff(void);
#endif

#if (VOLT_CONTROL_DC_FAN_USE == USE)
/// @brief      �ð� FAN ���� ��� ����Ʈ
/// @details    �ð� FAN ���� ��带 ���� ����Ʈ�� �����Ѵ�
typedef enum
{
    FAN_ID_OFF = 0,        // 0 :
    FAN_ID_18V_ON,         // 1 :
    FAN_ID_21V_ON,         // 2 :
    FAN_ID_MAX             // Max
} EFanID_T;

void Set_CoolFanVolt(U8 mu8Volt);
U8 Get_CoolFanVolt(void);
void Set_CoolFanActMode(U8 mu8Data);
U8 Get_CoolFanActMode(void);
#endif

void Lib_CoolFan_Control(void);

void Lib_CoolFan_Module_Initialize(void);
void Lib_CoolFan_Module_1ms_Control(void);
void Lib_CoolFan_Module_Control(void);

#endif

