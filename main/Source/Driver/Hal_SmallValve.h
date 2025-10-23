/// @file     Hal_SmallValve.h
/// @date     2025/03/28
/// @author   Jaejin Ham
/// @brief    소형 밸브 제어 file


#ifndef _HAL_SMALL_VALVE_H_
#define _HAL_SMALL_VALVE_H_


/*  소형 밸브(전압 가변 제어 밸브) 제어 Function 사용법
    => Set_SmallValveControl(Valve ID, Open/Close data, Delay Time, Change Voltage Delay Time);

    사용 예)
    Set_SmallValveControl(ValveID, OPEN, 0, 0);     // ValveID 이름의 소형밸브를 0초 딜레이 후 OPEN시키고 즉시 전압 가변 ON
    Set_SmallValveControl(ValveID, OPEN, 5, 0);     // ValveID 이름의 소형밸브를 500ms 딜레이 후 OPEN시키고 즉시 전압 가변 ON
    Set_SmallValveControl(ValveID, OPEN, 0, 30);    // ValveID 이름의 소형밸브를 0초 딜레이 후 OPEN시키고 3초 후 전압 가변 ON
    Set_SmallValveControl(ValveID, OPEN, 5, 30);    // ValveID 이름의 소형밸브를 500ms 딜레이 후 OPEN시키고 3초 후 전압 가변 ON

    Set_SmallValveControl(ValveID, CLOSE, 0, 0);    // ValveID 이름의 소형밸브를 0초 딜레이 후 CLOSE시키고 즉시 전압 가변 OFF
    Set_SmallValveControl(ValveID, CLOSE, 5, 0);    // ValveID 이름의 소형밸브를 500ms 딜레이 후 CLOSE시키고 즉시 전압 가변OFF

    Hal_AllSmallValve_Close();                      // 모든 소형 Valve 즉시 Close
*/


// 소형 밸브(전압 가변 제어) 최대 2개 사용 가능
typedef enum
{
    SMALL_VALVE_ID_NONE = 0,                            // 0 :
    SMALL_VALVE_ID_FAUCET_OUT,                          // 1 :
    SMALL_VALVE_ID_MAX                                  // 3 Max, 3이 넘으면 안됨
} ESmallValveID_T;

#define SMALL_VALVE_COUNT                               0       // 소형 밸브 개수 - MAX 2

// 소형 밸브 port 설정
// 순서상 skip없이 배치하여 사용하며, 미사용 port는 0으로 처리
// 순서는 ID와 동일하게 배치
// 소형 밸브
#if (SMALL_VALVE_COUNT >= 1)
#define PORT_DEFINE_SMALL_VALVE_1                       0
#define PORT_DEFINE_SMALL_VALVE_1_TRIG                  0
#endif

#if (SMALL_VALVE_COUNT >= 2)
#define PORT_DEFINE_SMALL_VALVE_2                       0
#define PORT_DEFINE_SMALL_VALVE_2_TRIG                  0
#endif


#if (SMALL_VALVE_COUNT > 0)

#define SMALL_VALVE_ID_MAX_COUNT            SMALL_VALVE_ID_MAX

// Valve Port Define
#define P_SMALL_VALVE_1                     PORT_DEFINE_SMALL_VALVE_1
#define TURN_ON_SMALL_VALVE_1()             { P_SMALL_VALVE_1 = 1; }
#define TURN_OFF_SMALL_VALVE_1()            { P_SMALL_VALVE_1 = 0; }
#define GET_STATUS_SMALL_VALVE_1()          ( P_SMALL_VALVE_1 )

#define P_SMALL_VALVE_1_TRIG                PORT_DEFINE_SMALL_VALVE_1_TRIG
#define TURN_ON_SMALL_VALVE_1_TRIG()        { P_SMALL_VALVE_1_TRIG = 1; }
#define TURN_OFF_SMALL_VALVE_1_TRIG()       { P_SMALL_VALVE_1_TRIG = 0; }
#define GET_STATUS_SMALL_VALVE_1_TRIG()     ( P_SMALL_VALVE_1_TRIG )

#define P_SMALL_VALVE_2                     PORT_DEFINE_SMALL_VALVE_2
#define TURN_ON_SMALL_VALVE_2()             { P_SMALL_VALVE_2 = 1; }
#define TURN_OFF_SMALL_VALVE_2()            { P_SMALL_VALVE_2 = 0; }
#define GET_STATUS_SMALL_VALVE_2()          ( P_SMALL_VALVE_2 )

#define P_SMALL_VALVE_2_TRIG                PORT_DEFINE_SMALL_VALVE_2_TRIG
#define TURN_ON_SMALL_VALVE_2_TRIG()        { P_SMALL_VALVE_2_TRIG = 1; }
#define TURN_OFF_SMALL_VALVE_2_TRIG()       { P_SMALL_VALVE_2_TRIG = 0; }
#define GET_STATUS_SMALL_VALVE_2_TRIG()     ( P_SMALL_VALVE_2_TRIG )

#define SMALL_VALVE_CONTROL_TIME_PERIOD     100         // 100ms@1ms

void Hal_SmallValve_Initialize(void);
void SmallValveControlTimer(void);
void SmallValveControl(void);
void Set_SmallValveControl(U8 mu8ValveID, U8 mu8Status, U8 mu8Delay1, U8 mu8Delay2);
void Hal_AllSmallValve_Close(void);
void Hal_SmallValveControl(U8 mu8ValveID);

#endif

void Hal_SmallValve_Module_Initialize(void);
void Hal_SmallValve_Module_1ms_Control(void);
void Hal_SmallValve_Module_Control(void);

#endif

