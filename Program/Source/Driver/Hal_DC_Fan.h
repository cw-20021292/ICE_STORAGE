/// @file     Hal_CoolingFan.h
/// @date     2025/02/21
/// @author   Jaejin Ham
/// @brief    Cooling Fan Control file


#ifndef _HAL_COOLING_FAN_H_
#define _HAL_COOLING_FAN_H_


/*  DC Fan 제어 Function 사용법
    1. 전압 가변 제어 하는 경우
      => Port를 직접 제어하는 방식을 사용 한다

      사용 예)
      TURN_ON_DC_FAN_21V();


    2. 전압 가변 없는 경우
      => Set_DC_Fan_Control(DC Fan ID, On/off Data);

      사용 예)
      Set_DC_Fan_Control(DC_FanID_1, 1);      // DC Fan 1 On
      Set_DC_Fan_Control(DC_FanID_1, ON);     // DC Fan 1 On

      Set_DC_Fan_Control(DC_FanID_1, 0);      // DC Fan 1 Off
      Set_DC_Fan_Control(DC_FanID_1, OFF);    // DC Fan 1 Off

      Hal_All_DC_Fan_Off();                   // 모든 DC Fan Off

      u8Return = Get_CoolingFan_Status(DC_FanID_1);  // DC Fan 1 동작 상태 확인 - 1(ON), 0(OFF)
*/

#define VOLT_CONTROL_DC_FAN_USE         NO_USE

#if (VOLT_CONTROL_DC_FAN_USE == USE)
// 전압을 가변하여 제어하는 DC FAN의 경우에는 Port만 선언하여 코드에서 직접 사용
#define P_DC_FAN_21V                    P6_bit.no7
#define TURN_ON_DC_FAN_21V()            { P_DC_FAN_21V = 1; }
#define TURN_OFF_DC_FAN_21V()           { P_DC_FAN_21V = 0; }
#define GET_STATUS_DC_FAN_21V()         ( P_DC_FAN_21V )

#define P_DC_FAN_18V                     P6_bit.no6
#define TURN_ON_DC_FAN_18V()             { P_DC_FAN_18V = 1; }
#define TURN_OFF_DC_FAN_18V()            { P_DC_FAN_18V = 0; }
#define GET_STATUS_DC_FAN_18V()          ( P_DC_FAN_18V )
#endif


// DC FAN 최대 5개 사용 가능(전압가변 없는 Fan 제어에서만 사용)
typedef enum
{
    DC_FAN_ID_NONE = 0,                 // 0 :
    DC_FAN_ID_COOLING,                  // 1 :
    DC_FAN_ID_MAX                       // 6 Max, 6이 넘으면 안됨
} EDC_FanID_T;

#define DC_FAN_COUNT                    0       // 전압 가변이 없는 DC Fan의 개수 - MAX 5

#if (DC_FAN_COUNT > 0)

#define DC_FAN_ID_MAX_COUNT             DC_FAN_ID_MAX

// DC Fan Port 할당
// 순서상 skip없이 배치하여 사용하며, 미사용 port는 0으로 처리
// 순서는 ID와 동일하게 배치
#define P_DC_FAN_1                      P6_bit.no7
#define TURN_ON_DC_FAN_1()              { P_DC_FAN_1 = 1; }
#define TURN_OFF_DC_FAN_1()             { P_DC_FAN_1 = 0; }
#define GET_STATUS_DC_FAN_1()           ( P_DC_FAN_1 )

#define P_DC_FAN_2                      0
#define TURN_ON_DC_FAN_2()              { P_DC_FAN_2 = 1; }
#define TURN_OFF_DC_FAN_2()             { P_DC_FAN_2 = 0; }
#define GET_STATUS_DC_FAN_2()           ( P_DC_FAN_2 )

#define P_DC_FAN_3                      0
#define TURN_ON_DC_FAN_3()              { P_DC_FAN_3 = 1; }
#define TURN_OFF_DC_FAN_3()             { P_DC_FAN_3 = 0; }
#define GET_STATUS_DC_FAN_3()           ( P_DC_FAN_3 )

#define P_DC_FAN_4                      0
#define TURN_ON_DC_FAN_4()              { P_DC_FAN_4 = 1; }
#define TURN_OFF_DC_FAN_4()             { P_DC_FAN_4 = 0; }
#define GET_STATUS_DC_FAN_4()           ( P_DC_FAN_4 )

#define P_DC_FAN_5                      0
#define TURN_ON_DC_FAN_5()              { P_DC_FAN_5 = 1; }
#define TURN_OFF_DC_FAN_5()             { P_DC_FAN_5 = 0; }
#define GET_STATUS_DC_FAN_5()           ( P_DC_FAN_5 )

#define DC_FAN_CONTROL_TIME_PERIOD      100         // 100ms@1ms


void Hal_DC_Fan_Initialize(void);
void Hal_All_DC_Fan_Off(void);
void DC_FanControlTimer(void);
void DC_FanControl(void);
void Set_DC_Fan_Control(U8 mu8DCFanID, U8 mu8OnOff);
U8 Get_DC_Fan_Status(U8 mu8DCFanID);
void Hal_DC_Fan_Control(U8 mu8DCFanID);

#endif


void Hal_DC_Fan_Module_Initialize(void);
void Hal_DC_Fan_Module_1ms_Control(void);
void Hal_DC_Fan_Module_Control(void);

#endif

