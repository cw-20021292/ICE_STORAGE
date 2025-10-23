/// @file     Hal_TEM.h
/// @date     2025/04/10
/// @author   Jaejin Ham
/// @brief    TEM 제어 file

#ifndef _HAL_TEM_H_
#define _HAL_TEM_H_

// ******************************************************************
//                          TEM 설정
// ******************************************************************
/*  TEM 제어 Function 사용법
    => Set_TEM_Voltage(mu8Volt);           // mu8Volt의 전압으로 TEM 제어(제어 가능 전압 0V, 24V, 5V~12V)
    ※ TEM 제어는 SMPS의 HW 구성 사양에 따라 제어 코드를 수정해야 할 수 있음(구조는 그대로 사용)

    Set_TEM_Voltage(TEM_12V);               // TEM 12V 제어
    Set_TEM_Voltage(12);                    // TEM 12V 제어

    Set_TEM_Voltage(TEM_0V);                // TEM 0V(OFF) 제어
    Set_TEM_Voltage(0);                     // TEM 0V(OFF) 제어
*/


#define TEM_USE                                     NO_USE      // TEM 사용 여부 - 0(No Use), 1(Use)

#if (TEM_USE == USE)
// Port 할당, 미사용시 0으로 세팅
#define TEM_PWM_REGISTER                            TDR12       // PWM Out Register
#define TEM_PWM_TAU_MAX_VALUE                       0x7CFF

#define TEM_PWM_FULL_TDR                            ( (TEM_PWM_TAU_MAX_VALUE+1) - (TEM_PWM_TAU_MAX_VALUE+1) ) //  반전 출력
#define TEM_PWM_OFF_TDR                             ( (TEM_PWM_TAU_MAX_VALUE+1) )     //  반전 출력

#define P_TEM_24V_ONOFF		                        P11_bit.no5
#define TURN_ON_TEM_24V()                           { P_TEM_24V_ONOFF = 1; }
#define TURN_OFF_TEM_24V()                          { P_TEM_24V_ONOFF = 0; }
#define GET_STATUS_TEM_24V()                        ( P_TEM_24V_ONOFF )

#define P_TEM_12V_ONOFF	            	            P11_bit.no6
#define TURN_ON_TEM_12V()                           { P_TEM_12V_ONOFF = 1; }
#define TURN_OFF_TEM_12V()                          { P_TEM_12V_ONOFF = 0; }
#define GET_STATUS_TEM_12V()                        ( P_TEM_12V_ONOFF )

#define TEM_CONTROL_TIME_PERIOD                     100         // 100ms@1ms
#define TEM_VOLTAGE_CHANGE_DELAY_TIME               600         // TEM 전압 변경시 최소 유지시간 1min = (60sec X 10)@100ms
#define TEM_12V_24V_CHANGE_DELAY_TIME               10          // TEM 전압 5~12V 제어에서 24V 제어로 변경시 지연 시간 1sec = 10@100ms
#define TEM_24V_12V_CHANGE_DELAY_TIME               10          // TEM 전압 24V 제어에서 5~12V 제어로 변경시 지연 시간 1sec = 10@100ms

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

