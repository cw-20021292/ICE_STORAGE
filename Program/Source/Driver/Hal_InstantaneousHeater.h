/// @file     Hal_Instantaneous_Heater.h
/// @date     2025/02/18
/// @author   Jaejin Ham
/// @brief    Instantaneous Heater Control file


#ifndef _HAL_INSTANTANEOUS_HEATER_H_
#define _HAL_INSTANTANEOUS_HEATER_H_

// ******************************************************************
//                         순간 온수 히터 설정
// ******************************************************************
/*  Instantaneous(순간) Heater 제어 Function 사용법
    => Set_Inst_HeaterControl(Relay On/Off, Triac Duty(0~100), Deley Time);
    => Set_Inst_Heater_Relay(Relay On/Off);
    => Set_Inst_Heater_Triac_Duty(Triac Duty(0~100));
    => Get_Inst_Heater_Status();

    사용 예)
    Set_Inst_HeaterControl(1, 100, 0);      // 순간 온수 Heater를 0초 딜레이 후 Relay On, Triac 100% duty
    Set_Inst_HeaterControl(ON, 100, 0);     // 순간 온수 Heater를 0초 딜레이 후 Relay On, Triac 100% duty

    Set_Inst_HeaterControl(1, 50, 5);       // 순간 온수 Heater를 500ms 딜레이 후 Relay On, Triac 50% duty
    Set_Inst_HeaterControl(ON, 50, 5);      // 순간 온수 Heater를 500ms 딜레이 후 Relay On, Triac 50% duty

    Set_Inst_Heater_Relay(1);      // Heater Relay On
    Set_Inst_Heater_Relay(ON);     // Heater Relay On

    Set_Inst_Heater_Relay(0);      // Heater Relay Off
    Set_Inst_Heater_Relay(OFF);    // Heater Relay Off

    Set_Inst_Heater_Triac_Duty(100);    // Heater Triac 100% Duty On

    u8Return = Get_Inst_Heater_Status();    // mu8Return : 3(Heater All On), 2(Heater Relay On), 1(Heater Triac On), 0(All Off)
*/

#define INSTANT_HEATER_USE                              NO_USE     // 순간온수 히터 사용 여부 - 0(No Use), 1(Use)

#if (INSTANT_HEATER_USE == USE)

// Instantaneous Heater port 할당
// Heater Port Define
#define P_INST_HEATER_RELAY_ON                          P7_bit.no1          // 순간 온수 히터 Relay
#define TURN_ON_INST_HEATER_RELAY()                     { P_INST_HEATER_RELAY_ON = 1; }
#define TURN_OFF_INST_HEATER_RELAY()                    { P_INST_HEATER_RELAY_ON = 0; }
#define GET_STATUS_INST_HEATER_RELAY()                  ( P_INST_HEATER_RELAY_ON )

#define P_INST_HEATER_TRIAC_ON                          P7_bit.no2          // 순간 온수 히터 Triac
#define TURN_ON_INST_HEATER_TRIAC()                     { P_INST_HEATER_TRIAC_ON = 1; }
#define TURN_OFF_INST_HEATER_TRIAC()                    { P_INST_HEATER_TRIAC_ON = 0; }
#define GET_STATUS_INST_HEATER_TRIAC()                  ( P_INST_HEATER_TRIAC_ON )

#define INST_HEATER_ALL_OFF                             0
#define INST_HEATER_TRIAC_ON                            1
#define INST_HEATER_RELAY_ON                            2
#define INST_HEATER_ALL_ON                              3

#define INST_HEATER_CONTROL_TIME_PERIOD                 100         // 100ms@1ms

#define INST_HEATER_TRIAC_CONTROL_CYCLE_TIME            200         // 200ms@1ms


void Hal_Inst_Heater_Initialize(void);
void InstHeaterControlTimer(void);
void InstHeaterControl(void);
void Set_Inst_HeaterControl(U8 mu8Relay, U8 mu8duty, U8 mu8DelayTime);
U8 Get_Inst_HeaterTriacDuty(void);
void Hal_Inst_Heater_Control(void);
void Set_Inst_Heater_Relay(U8 mu8OnOff);
void Set_Inst_Heater_Triac_Duty(U8 mu8duty);
U8 Get_Inst_Heater_Status(void);

void Hal_Inst_Heater_Relay_On(void);
void Hal_Inst_Heater_Relay_Off(void);
void Hal_Inst_Heater_Triac_On(void);
void Hal_Inst_Heater_Triac_Off(void);
void Hal_Inst_Heater_Triac_Duty_Control(void);

#endif


void Hal_Inst_Heater_Module_Initialize(void);
void Hal_Inst_Heater_Module_1ms_Control(void);
void Hal_Inst_Heater_Module_Control(void);

#endif

