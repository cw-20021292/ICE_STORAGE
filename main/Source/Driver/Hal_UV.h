/// @file     Hal_UV.h
/// @date     2025/02/27
/// @author   Jaejin Ham
/// @brief    UV Control file


#ifndef _HAL_UV_H_
#define _HAL_UV_H_


// ******************************************************************
//                             UV 설정
// ******************************************************************
/*  UV 제어 Function 사용법
    => Set_UV_Control(UV ID, On/off Data);
    => Get_UV_Status(mu8UV_ID);

    사용 예)
    Set_UV_Control(DC_FanID_1, 1);          // UV 1 On
    Set_UV_Control(DC_FanID_1, ON);         // UV 1 On

    Set_UV_Control(DC_FanID_1, 0);          // UV 1 Off
    Set_UV_Control(DC_FanID_1, OFF);        // UV 1 Off

    Hal_All_UV_Off();                       // 모든 UV Off

    u8Return = Get_UV_Status(UV_ID_1);      // UV 1 동작상태 요청, mu8Return : 1(ON), 0(OFF)
*/


// UV 최대 10개 사용 가능
typedef enum
{
    UV_ID_NONE = 0,                                     // 0 :
    UV_ID_WATER_TANK,                                   // 1 :
    UV_ID_MAX                                           // 11 Max, 11이 넘으면 안됨
} EUV_ID_T;

#define UV_COUNT                                        1       // UV의 개수 - MAX 10

#if (UV_COUNT > 0)
// UV Port 할당
// 순서상 skip없이 배치하여 사용하며, 미사용 port는 0으로 처리
// 순서는 ID와 동일하게 배치
#define PORT_DEFINE_UV_1                                P1_bit.no1      // UV 1
#define PORT_DEFINE_UV_2                                0
#define PORT_DEFINE_UV_3                                0
#define PORT_DEFINE_UV_4                                0
#define PORT_DEFINE_UV_5                                0
#define PORT_DEFINE_UV_6                                0
#define PORT_DEFINE_UV_7                                0
#define PORT_DEFINE_UV_8                                0
#define PORT_DEFINE_UV_9                                0
#define PORT_DEFINE_UV_10                               0

#define UV_ID_MAX_COUNT                 UV_ID_MAX

// DC Fan Port Define
#define P_UV_1                          PORT_DEFINE_UV_1
#define TURN_ON_UV_1()                  { P_UV_1 = 1; }
#define TURN_OFF_UV_1()                 { P_UV_1 = 0; }
#define GET_STATUS_UV_1()               ( P_UV_1 )

#define P_UV_2                          PORT_DEFINE_UV_2
#define TURN_ON_UV_2()                  { P_UV_2 = 1; }
#define TURN_OFF_UV_2()                 { P_UV_2 = 0; }
#define GET_STATUS_UV_2()               ( P_UV_2 )

#define P_UV_3                          PORT_DEFINE_UV_3
#define TURN_ON_UV_3()                  { P_UV_3 = 1; }
#define TURN_OFF_UV_3()                 { P_UV_3 = 0; }
#define GET_STATUS_UV_3()               ( P_UV_3 )

#define P_UV_4                          PORT_DEFINE_UV_4
#define TURN_ON_UV_4()                  { P_UV_4 = 1; }
#define TURN_OFF_UV_4()                 { P_UV_4 = 0; }
#define GET_STATUS_UV_4()               ( P_UV_4 )

#define P_UV_5                          PORT_DEFINE_UV_5
#define TURN_ON_UV_5()                  { P_UV_5 = 1; }
#define TURN_OFF_UV_5()                 { P_UV_5 = 0; }
#define GET_STATUS_UV_5()               ( P_UV_5 )

#define P_UV_6                          PORT_DEFINE_UV_6
#define TURN_ON_UV_6()                  { P_UV_6 = 1; }
#define TURN_OFF_UV_6()                 { P_UV_6 = 0; }
#define GET_STATUS_UV_6()               ( P_UV_6 )

#define P_UV_7                          PORT_DEFINE_UV_7
#define TURN_ON_UV_7()                  { P_UV_7 = 1; }
#define TURN_OFF_UV_7()                 { P_UV_7 = 0; }
#define GET_STATUS_UV_7()               ( P_UV_7 )

#define P_UV_8                          PORT_DEFINE_UV_8
#define TURN_ON_UV_8()                  { P_UV_8 = 1; }
#define TURN_OFF_UV_8()                 { P_UV_8 = 0; }
#define GET_STATUS_UV_8()               ( P_UV_8 )

#define P_UV_9                          PORT_DEFINE_UV_9
#define TURN_ON_UV_9()                  { P_UV_9 = 1; }
#define TURN_OFF_UV_9()                 { P_UV_9 = 0; }
#define GET_STATUS_UV_9()               ( P_UV_9 )

#define P_UV_10                         PORT_DEFINE_UV_10
#define TURN_ON_UV_10()                 { P_UV_10 = 1; }
#define TURN_OFF_UV_10()                { P_UV_10 = 0; }
#define GET_STATUS_UV_10()              ( P_UV_10 )

#define UV_CONTROL_TIME_PERIOD          100         // 100ms@1ms

void Hal_UV_Initialize(void);
void Hal_All_UV_Off(void);
void UV_ControlTimer(void);
void UV_Control(void);
void Set_UV_Control(U8 mu8UV_ID, U8 mu8OnOff);
U8 Get_UV_Status(U8 mu8UV_ID);
void Hal_UV_Control(U8 mu8UV_ID);

#endif


void Hal_UV_Module_Initialize(void);
void Hal_UV_Module_1ms_Control(void);
void Hal_UV_Module_Control(void);

#endif

