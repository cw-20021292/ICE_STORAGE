/// @file     Hal_BLDC_Motor.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    BLDC Motor Hardware Control Header

#ifndef _HAL_BLDC_MOTOR_H_
#define _HAL_BLDC_MOTOR_H_

#include "Global_Header.h"

/*  BLDC Motor 제어 Function 사용법
    => Hal_BLDC_Motor_Initialize();                           // BLDC 모터 초기화
    => Set_BLDCMotor_Power(ON/OFF);                          // BLDC 모터 전원 제어 - 1(ON), 0(OFF)
    => Set_BLDCMotor_PWM(PWM Duty값);                        // BLDC 모터 PWM Duty 제어 (0~40000)
    => Hal_BLDCMotor_IncrementRPMSignal();                   // Hall Sensor 신호 카운트 증가 (Interrupt에서 호출)
    => GetHallSensorSignalValue();                           // Hall Sensor 신호값 읽기

    사용 예)
    Hal_BLDC_Motor_Initialize();                             // 시스템 초기화 시 호출
    Set_BLDCMotor_Power(1);                                  // BLDC 모터 전원 ON
    Set_BLDCMotor_PWM(20000);                               // BLDC 모터 PWM Duty 50% 설정
    U16 rpm_signal = GetHallSensorSignalValue();            // 현재 RPM 신호값 읽기
    Set_BLDCMotor_Power(0);                                  // BLDC 모터 전원 OFF

    주의사항)
    - 초기화 함수는 시스템 시작 시 한번만 호출할 것
    - PWM 설정 전에 반드시 전원을 ON 상태로 설정할 것
    - Hall Sensor 신호 증가는 External Interrupt에서만 호출할 것
*/

// ******************************************************************
//                       BLDC Motor Hardware 설정
// ******************************************************************
// 하드웨어 레지스터 및 포트 정의
#define REGISTER_DEFINE_BLDC_PWM          TDR11        // BLDC Motor PWM 제어 레지스터
#define PORT_DEFINE_BLDC_POWER            P6_bit.no6   // BLDC Motor 전원 제어 Port
#define PORT_DEFINE_BLDC_HALL_SENSOR_IN   P3_bit.no1   // BLDC Motor Hall Sensor Port

// PWM 및 하드웨어 제어 상수
#define PWM_DUTY_REGISTER                 REGISTER_DEFINE_BLDC_PWM
#define P_MOTOR_POWER                     PORT_DEFINE_BLDC_POWER

// PWM 제어 관련 상수
#define VALUE_PWM_DUTY_START              11000   // BLDC Motor 기동 전압
#define VALUE_PWM_DUTY_STOP              0
#define VALUE_PWM_DUTY_MIN               0
#define VALUE_PWM_DUTY_MAX               40000

// 하드웨어 제어 함수
void Hal_BLDC_Motor_Initialize(void);
void Hal_BLDC_Motor_Module_1ms_Control(void);

// PWM 및 전원 제어 함수
void SetPwmOutLow(void);
void Set_BLDCMotor_PWM(U16 mu16Duty);
void Set_BLDCMotor_Power(U8 mu8Act);

// 홀센서 관련 함수
void Hal_BLDCMotor_IncrementRPMSignal(void);
U16 GetHallSensorSignalValue(void);

#endif // _HAL_BLDC_MOTOR_H_

