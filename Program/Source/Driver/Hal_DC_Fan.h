/// @file     Hal_CoolingFan.h
/// @date     2025/02/21
/// @author   Jaejin Ham
/// @brief    Cooling Fan Control file


#ifndef _HAL_COOLING_FAN_H_
#define _HAL_COOLING_FAN_H_


/*  DC Fan ���� Function ����
    1. ���� ���� ���� �ϴ� ���
      => Port�� ���� �����ϴ� ����� ��� �Ѵ�

      ��� ��)
      TURN_ON_DC_FAN_21V();


    2. ���� ���� ���� ���
      => Set_DC_Fan_Control(DC Fan ID, On/off Data);

      ��� ��)
      Set_DC_Fan_Control(DC_FanID_1, 1);      // DC Fan 1 On
      Set_DC_Fan_Control(DC_FanID_1, ON);     // DC Fan 1 On

      Set_DC_Fan_Control(DC_FanID_1, 0);      // DC Fan 1 Off
      Set_DC_Fan_Control(DC_FanID_1, OFF);    // DC Fan 1 Off

      Hal_All_DC_Fan_Off();                   // ��� DC Fan Off

      u8Return = Get_CoolingFan_Status(DC_FanID_1);  // DC Fan 1 ���� ���� Ȯ�� - 1(ON), 0(OFF)
*/

#define VOLT_CONTROL_DC_FAN_USE         NO_USE

#if (VOLT_CONTROL_DC_FAN_USE == USE)
// ������ �����Ͽ� �����ϴ� DC FAN�� ��쿡�� Port�� �����Ͽ� �ڵ忡�� ���� ���
#define P_DC_FAN_21V                    P6_bit.no7
#define TURN_ON_DC_FAN_21V()            { P_DC_FAN_21V = 1; }
#define TURN_OFF_DC_FAN_21V()           { P_DC_FAN_21V = 0; }
#define GET_STATUS_DC_FAN_21V()         ( P_DC_FAN_21V )

#define P_DC_FAN_18V                     P6_bit.no6
#define TURN_ON_DC_FAN_18V()             { P_DC_FAN_18V = 1; }
#define TURN_OFF_DC_FAN_18V()            { P_DC_FAN_18V = 0; }
#define GET_STATUS_DC_FAN_18V()          ( P_DC_FAN_18V )
#endif


// DC FAN �ִ� 5�� ��� ����(���а��� ���� Fan ������� ���)
typedef enum
{
    DC_FAN_ID_NONE = 0,                 // 0 :
    DC_FAN_ID_COOLING,                  // 1 :
    DC_FAN_ID_MAX                       // 6 Max, 6�� ������ �ȵ�
} EDC_FanID_T;

#define DC_FAN_COUNT                    0       // ���� ������ ���� DC Fan�� ���� - MAX 5

#if (DC_FAN_COUNT > 0)

#define DC_FAN_ID_MAX_COUNT             DC_FAN_ID_MAX

// DC Fan Port �Ҵ�
// ������ skip���� ��ġ�Ͽ� ����ϸ�, �̻�� port�� 0���� ó��
// ������ ID�� �����ϰ� ��ġ
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

