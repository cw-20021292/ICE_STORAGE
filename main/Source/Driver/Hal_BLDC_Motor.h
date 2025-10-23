/// @file     Hal_BLDC_Motor.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    BLDC Motor Hardware Control Header

#ifndef _HAL_BLDC_MOTOR_H_
#define _HAL_BLDC_MOTOR_H_

#include "Global_Header.h"

/*  BLDC Motor ���� Function ����
    => Hal_BLDC_Motor_Initialize();                           // BLDC ���� �ʱ�ȭ
    => Set_BLDCMotor_Power(ON/OFF);                          // BLDC ���� ���� ���� - 1(ON), 0(OFF)
    => Set_BLDCMotor_PWM(PWM Duty��);                        // BLDC ���� PWM Duty ���� (0~40000)
    => Hal_BLDCMotor_IncrementRPMSignal();                   // Hall Sensor ��ȣ ī��Ʈ ���� (Interrupt���� ȣ��)
    => GetHallSensorSignalValue();                           // Hall Sensor ��ȣ�� �б�

    ��� ��)
    Hal_BLDC_Motor_Initialize();                             // �ý��� �ʱ�ȭ �� ȣ��
    Set_BLDCMotor_Power(1);                                  // BLDC ���� ���� ON
    Set_BLDCMotor_PWM(20000);                               // BLDC ���� PWM Duty 50% ����
    U16 rpm_signal = GetHallSensorSignalValue();            // ���� RPM ��ȣ�� �б�
    Set_BLDCMotor_Power(0);                                  // BLDC ���� ���� OFF

    ���ǻ���)
    - �ʱ�ȭ �Լ��� �ý��� ���� �� �ѹ��� ȣ���� ��
    - PWM ���� ���� �ݵ�� ������ ON ���·� ������ ��
    - Hall Sensor ��ȣ ������ External Interrupt������ ȣ���� ��
*/

// ******************************************************************
//                       BLDC Motor Hardware ����
// ******************************************************************
// �ϵ���� �������� �� ��Ʈ ����
#define REGISTER_DEFINE_BLDC_PWM          TDR11        // BLDC Motor PWM ���� ��������
#define PORT_DEFINE_BLDC_POWER            P6_bit.no6   // BLDC Motor ���� ���� Port
#define PORT_DEFINE_BLDC_HALL_SENSOR_IN   P3_bit.no1   // BLDC Motor Hall Sensor Port

// PWM �� �ϵ���� ���� ���
#define PWM_DUTY_REGISTER                 REGISTER_DEFINE_BLDC_PWM
#define P_MOTOR_POWER                     PORT_DEFINE_BLDC_POWER

// PWM ���� ���� ���
#define VALUE_PWM_DUTY_START              11000   // BLDC Motor �⵿ ����
#define VALUE_PWM_DUTY_STOP              0
#define VALUE_PWM_DUTY_MIN               0
#define VALUE_PWM_DUTY_MAX               40000

// �ϵ���� ���� �Լ�
void Hal_BLDC_Motor_Initialize(void);
void Hal_BLDC_Motor_Module_1ms_Control(void);

// PWM �� ���� ���� �Լ�
void SetPwmOutLow(void);
void Set_BLDCMotor_PWM(U16 mu16Duty);
void Set_BLDCMotor_Power(U8 mu8Act);

// Ȧ���� ���� �Լ�
void Hal_BLDCMotor_IncrementRPMSignal(void);
U16 GetHallSensorSignalValue(void);

#endif // _HAL_BLDC_MOTOR_H_

