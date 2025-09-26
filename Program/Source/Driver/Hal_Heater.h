/// @file     Hal_Heater.h
/// @date     2025/02/17
/// @author   Jaejin Ham
/// @brief    Heater Control file


#ifndef _HAL_HEATER_H_
#define _HAL_HEATER_H_


/*  일반 Heater 제어 Function 사용법
    => Set_Heater_Control(Heater ID, On/Off Data, Delay Time);      // Heater ID의 Heater를 delay 시간(100ms 단위) 후에 On/Off 제어
    => Set_HeaterPort(Heater ID, On/Off data);                      // Heater ID의 Heater의 제어 Port를 직접 제어, 로직상 즉시 제어 동작이 필요한 경우 사용
    => Get_Heater_Status(Heater ID);                                // Heater ID의 동작 상태를 확인

    사용 예)
    Set_Heater_Control(HeaterID_1, 1, 0);               // HeaterID_1 이름의 히터를 0초 딜레이 후 On
    Set_Heater_Control(HeaterID_1, ON, 0);              // HeaterID_1 이름의 히터를 0초 딜레이 후 On
    Set_Heater_Control(HeaterID_1, 0, 0);               // HeaterID_1 이름의 히터를 0초 딜레이 후 Off
    Set_Heater_Control(HeaterID_1, OFF, 0);             // HeaterID_1 이름의 히터를 0초 딜레이 후 Off
    Set_Heater_Control(HeaterID_1, 1, 5);               // HeaterID_1 이름의 히터를 500ms 딜레이 후 On
    Set_Heater_Control(HeaterID_1, ON, 5);              // HeaterID_1 이름의 히터를 500ms 딜레이 후 On
    Set_Heater_Control(HeaterID_1, 0, 5);               // HeaterID_1 이름의 히터를 500ms 딜레이 후 Off
    Set_Heater_Control(HeaterID_1, OFF, 5);             // HeaterID_1 이름의 히터를 500ms 딜레이 후 Off
    Hal_All_Heater_Off();                               // 모든 히터 OFF
    u8Return = Get_Heater_Status(HeaterID_1);          // Heater 1 동작상태 요청, mu8Return : 1(ON), 0(OFF)
*/


#define NORMAL_HEATER_USE                               USE     // 일반 히터 사용 여부 - 0(No Use), 1(Use)

#if (NORMAL_HEATER_USE == USE)

// 일반 Heater 제어 최대 10개 사용 가능
typedef enum
{
    HEATER_ID_NONE = 0,                                 // 0 :
    HEATER_ID_HOT_TANK,                                 // 1 :
    HEATER_ID_MAX                                       // 11 Max, 11이 넘으면 안됨
} EHeaterID_T;

#define HEATER_COUNT                                    1       // Heater의 개수 - MAX 10

// 순서상 skip없이 배치하여 사용하며, 미사용 port는 0으로 처리
// 순서는 ID와 동일하게 배치
#define PORT_DEFINE_HEATER_1                            P7_bit.no0      // 온수 탱크 히터
#define PORT_DEFINE_HEATER_2                            0
#define PORT_DEFINE_HEATER_3                            0
#define PORT_DEFINE_HEATER_4                            0
#define PORT_DEFINE_HEATER_5                            0
#define PORT_DEFINE_HEATER_6                            0
#define PORT_DEFINE_HEATER_7                            0
#define PORT_DEFINE_HEATER_8                            0
#define PORT_DEFINE_HEATER_9                            0
#define PORT_DEFINE_HEATER_10                           0

#if (HEATER_COUNT > 0)

#define HEATER_ID_MAX_COUNT             HEATER_ID_MAX

// Heater Port Define
#define P_HEATER_1                      PORT_DEFINE_HEATER_1
#define TURN_ON_HEATER_1()              { P_HEATER_1 = 1; }
#define TURN_OFF_HEATER_1()             { P_HEATER_1 = 0; }
#define GET_STATUS_HEATER_1()           ( P_HEATER_1 )

#define P_HEATER_2                      PORT_DEFINE_HEATER_2
#define TURN_ON_HEATER_2()              { P_HEATER_2 = 1; }
#define TURN_OFF_HEATER_2()             { P_HEATER_2 = 0; }
#define GET_STATUS_HEATER_2()           ( P_HEATER_2 )

#define P_HEATER_3                      PORT_DEFINE_HEATER_3
#define TURN_ON_HEATER_3()              { P_HEATER_3 = 1; }
#define TURN_OFF_HEATER_3()             { P_HEATER_3 = 0; }
#define GET_STATUS_HEATER_3()           ( P_HEATER_3 )

#define P_HEATER_4                      PORT_DEFINE_HEATER_4
#define TURN_ON_HEATER_4()              { P_HEATER_4 = 1; }
#define TURN_OFF_HEATER_4()             { P_HEATER_4 = 0; }
#define GET_STATUS_HEATER_4()           ( P_HEATER_4 )

#define P_HEATER_5                      PORT_DEFINE_HEATER_5
#define TURN_ON_HEATER_5()              { P_HEATER_5 = 1; }
#define TURN_OFF_HEATER_5()             { P_HEATER_5 = 0; }
#define GET_STATUS_HEATER_5()           ( P_HEATER_5 )

#define P_HEATER_6                      PORT_DEFINE_HEATER_6
#define TURN_ON_HEATER_6()              { P_HEATER_6 = 1; }
#define TURN_OFF_HEATER_6()             { P_HEATER_6 = 0; }
#define GET_STATUS_HEATER_6()           ( P_HEATER_6 )

#define P_HEATER_7                      PORT_DEFINE_HEATER_7
#define TURN_ON_HEATER_7()              { P_HEATER_7 = 1; }
#define TURN_OFF_HEATER_7()             { P_HEATER_7 = 0; }
#define GET_STATUS_HEATER_7()           ( P_HEATER_7 )

#define P_HEATER_8                      PORT_DEFINE_HEATER_8
#define TURN_ON_HEATER_8()              { P_HEATER_8 = 1; }
#define TURN_OFF_HEATER_8()             { P_HEATER_8 = 0; }
#define GET_STATUS_HEATER_8()           ( P_HEATER_8 )

#define P_HEATER_9                      PORT_DEFINE_HEATER_9
#define TURN_ON_HEATER_9()              { P_HEATER_9 = 1; }
#define TURN_OFF_HEATER_9()             { P_HEATER_9 = 0; }
#define GET_STATUS_HEATER_9()           ( P_HEATER_9 )

#define P_HEATER_10                     PORT_DEFINE_HEATER_10
#define TURN_ON_HEATER_10()             { P_HEATER_10 = 1; }
#define TURN_OFF_HEATER_10()            { P_HEATER_10 = 0; }
#define GET_STATUS_HEATER_10()          ( P_HEATER_10 )


#define HEATER_CONTROL_TIME_PERIOD      100         // 100ms@1ms

void Hal_Heater_Initialize(void);
void Hal_All_Heater_Off(void);
void HeaterControlTimer(void);
void HeaterControl(void);
void Set_Heater_Control(U8 mu8HeaterID, U8 mu8OnOff, U8 mu8Delay);
U8 Get_Heater_Status(U8 mu8HeaterID);
void Hal_Heater_Control(U8 mu8HeaterID);
void Set_HeaterPort(U8 mu8HeaterID, U8 mu8OnOff);

#endif

#endif


void Hal_Heater_Module_Initialize(void);
void Hal_Heater_Module_1ms_Control(void);
void Hal_Heater_Module_Control(void);

#endif

