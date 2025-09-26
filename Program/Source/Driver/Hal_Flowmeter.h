/// @file     Hal_Flowmeter.h
/// @date     2025/03/20
/// @author   Jaejin Ham
/// @brief    Flowmeter Control file


#ifndef _HAL_FLOW_METER_H_
#define _HAL_FLOW_METER_H_


/*  Flowmeter ���� Function ����
    => Get_Flowmeter_Hz(Flowmeter ID);                     // ID�� �ش��ϴ� Flowmeter�� Hz Ȯ��
    => Set_FlowmeterCountdownPulse(mu8FlowmeterID, Pulse); // ID�� �ش��ϴ� Flowmeter�� Count Down �Ϸ��� Pulse ���� �Ҵ�
    => Get_FlowmeterCountdownEnd(mu8FlowmeterID);          // ID�� �ش��ϴ� Flowmeter�� �Ҵ��ߴ� Pulse ������ Count Down �Ϸ� üũ - 0(�̿Ϸ�), 1(�Ϸ�)

    ��� ��)
    u8Return = Get_Flowmeter_Hz(FLOWMETER_1);               // FLOWMETER_1 ID�� �������� Hz�� Ȯ��
    Set_FlowmeterCountdownPulse(FLOWMETER_1, 1000);         // FLOWMETER_1 ID�� ���������� Count Down�� 1000 Pulse �Ҵ�
    u8Return = Get_FlowmeterCountdownEnd(FLOWMETER_1);      // FLOWMETER_1 ID�� pulse Count Down �Ϸ� ���� Ȯ��
*/


// Flowmeter �ִ� 5�� ��� ����
typedef enum
{
    FLOWMETER_ID_NONE = 0,                              // 0 :
    FLOWMETER_ID_FLOW_1,                                // 1 : �Լ� ���� ����
    FLOWMETER_ID_FLOW_2,                                // 2 :
    FLOWMETER_ID_FLOW_3,                                // 3 :
    FLOWMETER_ID_MAX                                    // 6 Max, 6�� ������ �ȵ�
} EFlowmeterID_T;

#define FLOWMETER_COUNT                                 1           // Flowmeter ���� - MAX 5

//#define EW_MODULE_PWM_START()                         exStart();  // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Start�Լ� ��ġ
//#define EW_MODULE_PWM_STOP()                          exStop();   // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Stop�Լ� ��ġ
#if (FLOWMETER_COUNT >= 1)
#define FLOWMETER_1_ID                                  FLOWMETER_ID_FLOW_1   // Flowmeter 1�� ������ ID
#define FLOWMETER_1_INTERRUPT_START()                   R_Config_INTC_INTP2_Start();    // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Start�Լ� ��ġ
#define FLOWMETER_1_INTERRUPT_STOP()                    R_Config_INTC_INTP2_Stop();     // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Stop�Լ� ��ġ
#endif

#if (FLOWMETER_COUNT >= 2)
#define FLOWMETER_2_ID                                  FLOWMETER_ID_FLOW_2     // Flowmeter 2�� ������ ID
#define FLOWMETER_2_INTERRUPT_START()                   0                       // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Start�Լ� ��ġ
#define FLOWMETER_2_INTERRUPT_STOP()                    0                       // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Stop�Լ� ��ġ
#endif

#if (FLOWMETER_COUNT >= 3)
#define FLOWMETER_3_ID                                  FLOWMETER_ID_FLOW_3     // Flowmeter 3�� ������ ID
#define FLOWMETER_3_INTERRUPT_START()                   0                       // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Start�Լ� ��ġ
#define FLOWMETER_3_INTERRUPT_STOP()                    0                       // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Stop�Լ� ��ġ
#endif

#if (FLOWMETER_COUNT >= 4)
#define FLOWMETER_4_ID                                  0                       // Flowmeter 4�� ������ ID
#define FLOWMETER_4_INTERRUPT_START()                   0                       // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Start�Լ� ��ġ
#define FLOWMETER_4_INTERRUPT_STOP()                    0                       // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Stop�Լ� ��ġ
#endif

#if (FLOWMETER_COUNT >= 5)
#define FLOWMETER_5_ID                                  0                       // Flowmeter 5�� ������ ID
#define FLOWMETER_5_INTERRUPT_START()                   0                       // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Start�Լ� ��ġ
#define FLOWMETER_5_INTERRUPT_STOP()                    0                       // Code Generate�� Flowmeter �ܺ� ���ͷ�Ʈ Stop�Լ� ��ġ
#endif


#if (FLOWMETER_COUNT > 0)

#define FLOWMETER_ID_MAX_COUNT                  FLOWMETER_ID_MAX

#define FLOWMETER_CHECK_CONTROL_TIME_PERIOD     500         // 500ms@1ms

void Flowmeter_Initialize(void);

#if (FLOWMETER_COUNT >= 1)
void Flowmeter_1_Interrupt(void);
#endif

#if (FLOWMETER_COUNT >= 2)
void Flowmeter_2_Interrupt(void);
#endif

#if (FLOWMETER_COUNT >= 3)
void Flowmeter_3_Interrupt(void);
#endif

#if (FLOWMETER_COUNT >= 4)
void Flowmeter_4_Interrupt(void);
#endif

#if (FLOWMETER_COUNT >= 5)
void Flowmeter_5_Interrupt(void);
#endif

void Flowmeter_CheckControl(void);
void Flowmeter_Hz_Calculation(void);
U16 Get_Flowmeter_Hz(U8 mu8FlowmeterID);
void Set_FlowmeterCountdownPulse(U8 mu8FlowmeterID, U16 mu16Pulse);
U16 Get_FlowmeterCountdownPulse(U8 mu8FlowmeterID);
void Flowmeter_Pulse_CountDownHz(U8 mu8FlowmeterID);
U8 Get_FlowmeterCountdownEnd(U8 mu8FlowmeterID);
void Set_FlowmeterTestHz(U8 mu8FlowmeterID, U16 mu16Hz);
U16 Get_FlowmeterTestHz(U8 mu8FlowmeterID);


#endif


void Hal_Flowmeter_Module_Initialize(void);
void Hal_Flowmeter_Module_1ms_Control(void);

#endif

