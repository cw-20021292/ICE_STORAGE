/// @file     Hal_TEM.h
/// @date     2025/04/10
/// @author   Jaejin Ham
/// @brief    TEM ���� file

#ifndef _HAL_TEM_H_
#define _HAL_TEM_H_

// ******************************************************************
//                          TEM ����
// ******************************************************************
/*  TEM ���� Function ����
    => Set_TEM_Voltage(mu8Volt);           // mu8Volt�� �������� TEM ����(���� ���� ���� 0V, 24V, 5V~12V)
    �� TEM ����� SMPS�� HW ���� ��翡 ���� ���� �ڵ带 �����ؾ� �� �� ����(������ �״�� ���)

    Set_TEM_Voltage(TEM_12V);               // TEM 12V ����
    Set_TEM_Voltage(12);                    // TEM 12V ����

    Set_TEM_Voltage(TEM_0V);                // TEM 0V(OFF) ����
    Set_TEM_Voltage(0);                     // TEM 0V(OFF) ����
*/


#define TEM_USE                                     NO_USE      // TEM ��� ���� - 0(No Use), 1(Use)

#if (TEM_USE == USE)
// Port �Ҵ�, �̻��� 0���� ����
#define TEM_PWM_REGISTER                            TDR12       // PWM Out Register
#define TEM_PWM_TAU_MAX_VALUE                       0x7CFF

#define TEM_PWM_FULL_TDR                            ( (TEM_PWM_TAU_MAX_VALUE+1) - (TEM_PWM_TAU_MAX_VALUE+1) ) //  ���� ���
#define TEM_PWM_OFF_TDR                             ( (TEM_PWM_TAU_MAX_VALUE+1) )     //  ���� ���

#define P_TEM_24V_ONOFF		                        P11_bit.no5
#define TURN_ON_TEM_24V()                           { P_TEM_24V_ONOFF = 1; }
#define TURN_OFF_TEM_24V()                          { P_TEM_24V_ONOFF = 0; }
#define GET_STATUS_TEM_24V()                        ( P_TEM_24V_ONOFF )

#define P_TEM_12V_ONOFF	            	            P11_bit.no6
#define TURN_ON_TEM_12V()                           { P_TEM_12V_ONOFF = 1; }
#define TURN_OFF_TEM_12V()                          { P_TEM_12V_ONOFF = 0; }
#define GET_STATUS_TEM_12V()                        ( P_TEM_12V_ONOFF )

#define TEM_CONTROL_TIME_PERIOD                     100         // 100ms@1ms
#define TEM_VOLTAGE_CHANGE_DELAY_TIME               600         // TEM ���� ����� �ּ� �����ð� 1min = (60sec X 10)@100ms
#define TEM_12V_24V_CHANGE_DELAY_TIME               10          // TEM ���� 5~12V ����� 24V ����� ����� ���� �ð� 1sec = 10@100ms
#define TEM_24V_12V_CHANGE_DELAY_TIME               10          // TEM ���� 24V ����� 5~12V ����� ����� ���� �ð� 1sec = 10@100ms

void Hal_TEM_Initialize(void);
void TEM_ControlTimer(void);
void Set_TEM_Voltage(U8 mu8Volt);
U8 Get_TEM_Voltage(void);
void Set_TEM_TDR(U16 mu16Value);
U16 Get_TEM_TDR(void);
void Hal_TEM_Control(void);


#endif

void Hal_TEM_Module_Initialize(void);
void Hal_TEM_Module_1ms_Control(void);
void Hal_TEM_Module_Control(void);

#endif

