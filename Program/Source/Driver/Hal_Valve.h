/// @file     Hal_Valve.h
/// @date     2025/02/21
/// @author   Jaejin Ham
/// @brief    Valve Control file


#ifndef _HAL_VALVE_H_
#define _HAL_VALVE_H_


/*  Valve 제어 Function 사용법
    => Set_ValveControl(Valve ID, Valve Type(FEED/NOS), Open/Close data, Delay Time);

    => Set_ValvePort(Valve ID, ON/OFf data);                // Valve ID의 Port를 직접 제어할 때 사용하는 함수
    => Set_NosValvePort(NosValve ID, ON/OFf data);          // NosValve ID의 Port를 직접 제어할 때 사용하는 함수, NOS는 제어 신호가 반대라는 것 주의(예 : OPEN시 OFF으로 세팅)

    => Hal_AllFeedValve_Close();                            // 모든 Feed Valve Close
    => Hal_AllNosValve_Open();                              // 모든 NOS Valve Open

    => Get_ValveStatus(mu8ValveID);                         // ID에 해당하는 Feed 밸브 동작 상태 확인 - 1(On), 0(Off)
    => Get_NosValveStatus(mu8ValveID);                      // ID에 해당하는 NOS 밸브 동작 상태 확인 - 1(On), 0(Off)

    사용 예)
    Set_ValveControl(ValveID, FEED, OPEN, 0);               // ValveID 이름의 피드밸브를 0초 딜레이 후 OPEN
    Set_ValveControl(ValveID, FEED, CLOSE,0);               // ValveID 이름의 피드밸브를 0초 딜레이 후 CLOSE
    Set_ValveControl(ValveID, FEED, OPEN, 5);               // ValveID 이름의 피드밸브를 500ms 딜레이 후 OPEN
    Set_ValveControl(ValveID, FEED, CLOSE,5);               // ValveID 이름의 피드밸브를 500ms 딜레이 후 CLOSE

    Set_ValveControl(ValveID, NOS, OPEN, 0);                // ValveID 이름의 NOS밸브를 0초 딜레이 후 OPEN
    Set_ValveControl(ValveID, NOS, CLOSE,0);                // ValveID 이름의 NOS밸브를 0초 딜레이 후 CLOSE
    Set_ValveControl(ValveID, NOS, OPEN, 5);                // ValveID 이름의 NOS밸브를 500ms 딜레이 후 OPEN
    Set_ValveControl(ValveID, NOS, CLOSE,5);                // ValveID 이름의 NOS밸브를 500ms 딜레이 후 CLOSE

    Hal_AllFeedValve_Close();                               // 모든 Feed Valve Close
    Hal_AllNosValve_Open();                                 // 모든 NOS Valve Open

    u8Return = Get_ValveStatus(mu8ValveID);                 // ID에 해당하는 Feed 밸브 동작 상태 확인 - 1(On), 0(Off)
    u8Return = Get_NosValveStatus(mu8ValveID);              // ID에 해당하는 NOS 밸브 동작 상태 확인 - 1(On), 0(Off)
*/


// Feed Valve 최대 20개 사용 가능
typedef enum
{
    VALVE_ID_NONE = 0,                                  // 0 :
    VALVE_ID_WATER_IN,                                  // 1 :
    VALVE_ID_MAX                                        // 21 Max, 21이 넘으면 안됨
} EValveID_T;

// Nos Valve 최대 10개 사용 가능
typedef enum
{
    VALVE_NOS_ID_NONE = 0,                              // 0 :
    VALVE_NOS_ID_WATER_IN,                              // 1 :
    VALVE_NOS_ID_MAX                                    // 11 Max, 11이 넘으면 안됨
} ENosValveID_T;

#define VALVE_COUNT                                     1       // Feed 밸브 개수 - MAX 20
#define VALVE_NOS_COUNT                                 1       // Nos 밸브 개수 - MAX 10

#if (VALVE_COUNT > 0)
// Valve port 설정
// 순서상 skip없이 배치하여 사용하며, 미사용 port는 0으로 처리
// 순서는 ID와 동일하게 배치
// Feed Valve
#define PORT_DEFINE_VALVE_1                             P7_bit.no6
#define PORT_DEFINE_VALVE_2                             0
#define PORT_DEFINE_VALVE_3                             0
#define PORT_DEFINE_VALVE_4                             0
#define PORT_DEFINE_VALVE_5                             0
#define PORT_DEFINE_VALVE_6                             0
#define PORT_DEFINE_VALVE_7                             0
#define PORT_DEFINE_VALVE_8                             0
#define PORT_DEFINE_VALVE_9                             0
#define PORT_DEFINE_VALVE_10                            0
#define PORT_DEFINE_VALVE_11                            0
#define PORT_DEFINE_VALVE_12                            0
#define PORT_DEFINE_VALVE_13                            0
#define PORT_DEFINE_VALVE_14                            0
#define PORT_DEFINE_VALVE_15                            0
#define PORT_DEFINE_VALVE_16                            0
#define PORT_DEFINE_VALVE_17                            0
#define PORT_DEFINE_VALVE_18                            0
#define PORT_DEFINE_VALVE_19                            0
#define PORT_DEFINE_VALVE_20                            0
#endif

// Nos Valve
#if (VALVE_NOS_COUNT > 0)
#define PORT_DEFINE_NOS_VALVE_1                         P6_bit.no5
#define PORT_DEFINE_NOS_VALVE_2                         0
#define PORT_DEFINE_NOS_VALVE_3                         0
#define PORT_DEFINE_NOS_VALVE_4                         0
#define PORT_DEFINE_NOS_VALVE_5                         0
#define PORT_DEFINE_NOS_VALVE_6                         0
#define PORT_DEFINE_NOS_VALVE_7                         0
#define PORT_DEFINE_NOS_VALVE_8                         0
#define PORT_DEFINE_NOS_VALVE_9                         0
#define PORT_DEFINE_NOS_VALVE_10                        0
#endif


#if ( (VALVE_COUNT > 0) || (VALVE_NOS_COUNT > 0) )

#define VALVE_ID_MAX_COUNT              VALVE_ID_MAX
#define VALVE_NOS_ID_MAX_COUNT          VALVE_NOS_ID_MAX


#if (VALVE_COUNT > 0)
// Valve Port Define
#define P_VALVE_1                       PORT_DEFINE_VALVE_1
#define TURN_ON_VALVE_1()               { P_VALVE_1 = 1; }
#define TURN_OFF_VALVE_1()              { P_VALVE_1 = 0; }
#define GET_STATUS_VALVE_1()            ( P_VALVE_1 )

#define P_VALVE_2                       PORT_DEFINE_VALVE_2
#define TURN_ON_VALVE_2()               { P_VALVE_2 = 1; }
#define TURN_OFF_VALVE_2()              { P_VALVE_2 = 0; }
#define GET_STATUS_VALVE_2()            ( P_VALVE_2 )

#define P_VALVE_3                       PORT_DEFINE_VALVE_3
#define TURN_ON_VALVE_3()               { P_VALVE_3 = 1; }
#define TURN_OFF_VALVE_3()              { P_VALVE_3 = 0; }
#define GET_STATUS_VALVE_3()            ( P_VALVE_3 )

#define P_VALVE_4                       PORT_DEFINE_VALVE_4
#define TURN_ON_VALVE_4()               { P_VALVE_4 = 1; }
#define TURN_OFF_VALVE_4()              { P_VALVE_4 = 0; }
#define GET_STATUS_VALVE_4()            ( P_VALVE_4 )

#define P_VALVE_5                       PORT_DEFINE_VALVE_5
#define TURN_ON_VALVE_5()               { P_VALVE_5 = 1; }
#define TURN_OFF_VALVE_5()              { P_VALVE_5 = 0; }
#define GET_STATUS_VALVE_5()            ( P_VALVE_5 )

#define P_VALVE_6                       PORT_DEFINE_VALVE_6
#define TURN_ON_VALVE_6()               { P_VALVE_6 = 1; }
#define TURN_OFF_VALVE_6()              { P_VALVE_6 = 0; }
#define GET_STATUS_VALVE_6()            ( P_VALVE_6 )

#define P_VALVE_7                       PORT_DEFINE_VALVE_7
#define TURN_ON_VALVE_7()               { P_VALVE_7 = 1; }
#define TURN_OFF_VALVE_7()              { P_VALVE_7 = 0; }
#define GET_STATUS_VALVE_7()            ( P_VALVE_7 )

#define P_VALVE_8                       PORT_DEFINE_VALVE_8
#define TURN_ON_VALVE_8()               { P_VALVE_8 = 1; }
#define TURN_OFF_VALVE_8()              { P_VALVE_8 = 0; }
#define GET_STATUS_VALVE_8()            ( P_VALVE_8 )

#define P_VALVE_9                       PORT_DEFINE_VALVE_9
#define TURN_ON_VALVE_9()               { P_VALVE_9 = 1; }
#define TURN_OFF_VALVE_9()              { P_VALVE_9 = 0; }
#define GET_STATUS_VALVE_9()            ( P_VALVE_9 )

#define P_VALVE_10                      PORT_DEFINE_VALVE_10
#define TURN_ON_VALVE_10()              { P_VALVE_10 = 1; }
#define TURN_OFF_VALVE_10()             { P_VALVE_10 = 0; }
#define GET_STATUS_VALVE_10()           ( P_VALVE_10 )

#define P_VALVE_11                      PORT_DEFINE_VALVE_11
#define TURN_ON_VALVE_11()              { P_VALVE_11 = 1; }
#define TURN_OFF_VALVE_11()             { P_VALVE_11 = 0; }
#define GET_STATUS_VALVE_11()           ( P_VALVE_11 )

#define P_VALVE_12                      PORT_DEFINE_VALVE_12
#define TURN_ON_VALVE_12()              { P_VALVE_12 = 1; }
#define TURN_OFF_VALVE_12()             { P_VALVE_12 = 0; }
#define GET_STATUS_VALVE_12()           ( P_VALVE_12 )

#define P_VALVE_13                      PORT_DEFINE_VALVE_13
#define TURN_ON_VALVE_13()              { P_VALVE_13 = 1; }
#define TURN_OFF_VALVE_13()             { P_VALVE_13 = 0; }
#define GET_STATUS_VALVE_13()           ( P_VALVE_13 )

#define P_VALVE_14                      PORT_DEFINE_VALVE_14
#define TURN_ON_VALVE_14()               { P_VALVE_14 = 1; }
#define TURN_OFF_VALVE_14()              { P_VALVE_14 = 0; }
#define GET_STATUS_VALVE_14()            ( P_VALVE_14 )

#define P_VALVE_15                      PORT_DEFINE_VALVE_15
#define TURN_ON_VALVE_15()              { P_VALVE_15 = 1; }
#define TURN_OFF_VALVE_15()             { P_VALVE_15 = 0; }
#define GET_STATUS_VALVE_15()           ( P_VALVE_15 )

#define P_VALVE_16                      PORT_DEFINE_VALVE_16
#define TURN_ON_VALVE_16()              { P_VALVE_16 = 1; }
#define TURN_OFF_VALVE_16()             { P_VALVE_16 = 0; }
#define GET_STATUS_VALVE_16()           ( P_VALVE_16 )

#define P_VALVE_17                      PORT_DEFINE_VALVE_17
#define TURN_ON_VALVE_17()              { P_VALVE_17 = 1; }
#define TURN_OFF_VALVE_17()             { P_VALVE_17 = 0; }
#define GET_STATUS_VALVE_17()           ( P_VALVE_17 )

#define P_VALVE_18                      PORT_DEFINE_VALVE_18
#define TURN_ON_VALVE_18()              { P_VALVE_18 = 1; }
#define TURN_OFF_VALVE_18()             { P_VALVE_18 = 0; }
#define GET_STATUS_VALVE_18()           ( P_VALVE_18 )

#define P_VALVE_19                      PORT_DEFINE_VALVE_19
#define TURN_ON_VALVE_19()              { P_VALVE_19 = 1; }
#define TURN_OFF_VALVE_19()             { P_VALVE_19 = 0; }
#define GET_STATUS_VALVE_19()           ( P_VALVE_19 )

#define P_VALVE_20                      PORT_DEFINE_VALVE_20
#define TURN_ON_VALVE_20()               { P_VALVE_20 = 1; }
#define TURN_OFF_VALVE_20()              { P_VALVE_20 = 0; }
#define GET_STATUS_VALVE_20()            ( P_VALVE_20 )

#endif

#if (VALVE_NOS_COUNT > 0)
#define P_NOS_VALVE_1                   PORT_DEFINE_NOS_VALVE_1
#define TURN_ON_NOS_VALVE_1()           { P_NOS_VALVE_1 = 1; }
#define TURN_OFF_NOS_VALVE_1()          { P_NOS_VALVE_1 = 0; }
#define GET_STATUS_NOS_VALVE_1()        ( P_NOS_VALVE_1 )

#define P_NOS_VALVE_2                   PORT_DEFINE_NOS_VALVE_2
#define TURN_ON_NOS_VALVE_2()           { P_NOS_VALVE_2 = 1; }
#define TURN_OFF_NOS_VALVE_2()          { P_NOS_VALVE_2 = 0; }
#define GET_STATUS_NOS_VALVE_2()        ( P_NOS_VALVE_2 )

#define P_NOS_VALVE_3                   PORT_DEFINE_NOS_VALVE_3
#define TURN_ON_NOS_VALVE_3()           { P_NOS_VALVE_3 = 1; }
#define TURN_OFF_NOS_VALVE_3()          { P_NOS_VALVE_3 = 0; }
#define GET_STATUS_NOS_VALVE_3()        ( P_NOS_VALVE_3 )

#define P_NOS_VALVE_4                   PORT_DEFINE_NOS_VALVE_4
#define TURN_ON_NOS_VALVE_4()           { P_NOS_VALVE_4 = 1; }
#define TURN_OFF_NOS_VALVE_4()          { P_NOS_VALVE_4 = 0; }
#define GET_STATUS_NOS_VALVE_4()        ( P_NOS_VALVE_4 )

#define P_NOS_VALVE_5                   PORT_DEFINE_NOS_VALVE_5
#define TURN_ON_NOS_VALVE_5()           { P_NOS_VALVE_5 = 1; }
#define TURN_OFF_NOS_VALVE_5()          { P_NOS_VALVE_5 = 0; }
#define GET_STATUS_NOS_VALVE_5()        ( P_NOS_VALVE_5 )

#define P_NOS_VALVE_6                   PORT_DEFINE_NOS_VALVE_6
#define TURN_ON_NOS_VALVE_6()           { P_NOS_VALVE_6 = 1; }
#define TURN_OFF_NOS_VALVE_6()          { P_NOS_VALVE_6 = 0; }
#define GET_STATUS_NOS_VALVE_6()        ( P_NOS_VALVE_6 )

#define P_NOS_VALVE_7                   PORT_DEFINE_NOS_VALVE_7
#define TURN_ON_NOS_VALVE_7()           { P_NOS_VALVE_7 = 1; }
#define TURN_OFF_NOS_VALVE_7()          { P_NOS_VALVE_7 = 0; }
#define GET_STATUS_NOS_VALVE_7()        ( P_NOS_VALVE_7 )

#define P_NOS_VALVE_8                   PORT_DEFINE_NOS_VALVE_8
#define TURN_ON_NOS_VALVE_8()           { P_NOS_VALVE_8 = 1; }
#define TURN_OFF_NOS_VALVE_8()          { P_NOS_VALVE_8 = 0; }
#define GET_STATUS_NOS_VALVE_8()        ( P_NOS_VALVE_8 )

#define P_NOS_VALVE_9                   PORT_DEFINE_NOS_VALVE_9
#define TURN_ON_NOS_VALVE_9()           { P_NOS_VALVE_9 = 1; }
#define TURN_OFF_NOS_VALVE_9()          { P_NOS_VALVE_9 = 0; }
#define GET_STATUS_NOS_VALVE_9()        ( P_NOS_VALVE_9 )

#define P_NOS_VALVE_10                  PORT_DEFINE_NOS_VALVE_10
#define TURN_ON_NOS_VALVE_10()          { P_NOS_VALVE_10 = 1; }
#define TURN_OFF_NOS_VALVE_10()         { P_NOS_VALVE_10 = 0; }
#define GET_STATUS_NOS_VALVE_10()       ( P_NOS_VALVE_10 )

#endif

// Valve Type define
#define FEED                            1
#define NOS                             2

#define VALVE_CONTROL_TIME_PERIOD      100         // 100ms@1ms

void Hal_Valve_Initialize(void);
void ValveControlTimer(void);
void ValveControl(void);
void Set_ValveControl(U8 mu8ValveID, U8 mu8ValveType, U8 mu8Status, U8 mu8Delay);

#if (VALVE_COUNT > 0)
void Hal_AllFeedValve_Close(void);
void Hal_ValveControl(U8 mu8ValveID);
void Set_ValvePort(U8 mu8ValveID, U8 mu8OnOff);
U8 Get_ValveStatus(U8 mu8ValveID);
#endif

#if (VALVE_NOS_COUNT > 0)
void Hal_AllNosValve_Open(void);
void Hal_NosValveControl(U8 mu8NosValveID);
void Set_NosValvePort(U8 mu8NosValveID, U8 mu8OnOff);
U8 Get_NosValveStatus(U8 mu8ValveID);
#endif

#endif


void Hal_Valve_Module_Initialize(void);
void Hal_Valve_Module_1ms_Control(void);
void Hal_Valve_Module_Control(void);

#endif

