/// @file     Hal_BoostPump.h
/// @date     2025/03/10
/// @author   Jaejin Ham
/// @brief    Boost Pump Control file


#ifndef _HAL_BOOST_PUMP_H_
#define _HAL_BOOST_PUMP_H_


/*  부스트 펌프 제어 Function 사용법
    => u8Return = Get_BoostInStatus();              // 부스트 펌프 연결 상태 확인 - 0(미연결), 1(연결)
    => Set_BoostPump(On/Off Data, Delay Time);      // 부스트 펌프 동작 상태 설정
    => u8Return = Get_BoostPump_Status();           // 부스트 펌프 동작 상태 확인 - 1(On), 0(Off)

    사용 예)
    Set_BoostPump(1, 0);        // 부스트 펌프 0초 딜레이 후 On
    Set_BoostPump(ON, 0);       // 부스트 펌프 0초 딜레이 후 On

    Set_BoostPump(0, 0);        // 부스트 펌프 0초 딜레이 후 Off
    Set_BoostPump(OFF, 0);      // 부스트 펌프 0초 딜레이 후 Off

    Set_BoostPump(1, 5);        // 부스트 펌프 500ms 딜레이 후 On
    Set_BoostPump(ON, 5);       // 부스트 펌프 500ms 딜레이 후 On

    Set_BoostPump(0, 5);        // 부스트 펌프 500ms 딜레이 후 Off
    Set_BoostPump(OFF, 5);      // 부스트 펌프 500ms 딜레이 후 Off

    mu8Return = Get_BoostPump_Status();     // mu8Return : 1(ON), 0(OFF)
*/

#define BOOST_PUMP_USE                      USE             // 부스트 펌프 사용 여부 - 0(No USE), 1(USE)

#if (BOOST_PUMP_USE == USE)
// Boost Pump Port 할당
// 미사용 port는 0으로 처리
#define PORT_DEFINE_BOOST_PUMP_OUT          P7_bit.no5      // Boost Pump OUT
#define PORT_DEFINE_BOOST_PUMP_IN           0               // Boost Pump IN
#endif


#if (BOOST_PUMP_USE == USE)

// Boost Pump Port Define
#define P_BOOST_OUT                         PORT_DEFINE_BOOST_PUMP_OUT
#define TURN_ON_BOOST_OUT()                 { P_BOOST_OUT = 1; }
#define TURN_OFF_BOOST_OUT()                { P_BOOST_OUT = 0; }
#define GET_STATUS_BOOST_OUT()              ( P_BOOST_OUT )

#define P_BOOST_IN                          PORT_DEFINE_BOOST_PUMP_IN
#define GET_STATUS_BOOST_IN()               ( P_BOOST_IN )


#define BOOST_PUMP_CONTROL_TIME_PERIOD      100         // 100ms@1ms
#define BOOST_PUMP_IN_CHECK_TIME            5           // 100ms@1ms

void Hal_BoostPump_Initialize(void);
void BoostPumpControlTimer(void);
void BoostPumpControl(void);
void BoostInCheck(void);
U8 Get_BoostInStatus(void);
void Set_BoostPump(U8 mu8OnOff, U8 mu8Delay);
U8 Get_BoostPump_Status(void);
void Hal_BoostPump_Control(void);

#endif


void Hal_BoostPump_Module_Initialize(void);
void Hal_BoostPump_Module_1ms_Control(void);
void Hal_BoostPump_Module_Control(void);

#endif

