/// @file     Hal_Heater.h
/// @date     2025/02/17
/// @author   Jaejin Ham
/// @brief    Heater Control file


#ifndef _HAL_HEATER_H_
#define _HAL_HEATER_H_


/*  �Ϲ� Heater ���� Function ����
    => Set_Heater_Control(Heater ID, On/Off Data, Delay Time);      // Heater ID�� Heater�� delay �ð�(100ms ����) �Ŀ� On/Off ����
    => Set_HeaterPort(Heater ID, On/Off data);                      // Heater ID�� Heater�� ���� Port�� ���� ����, ������ ��� ���� ������ �ʿ��� ��� ���
    => Get_Heater_Status(Heater ID);                                // Heater ID�� ���� ���¸� Ȯ��

    ��� ��)
    Set_Heater_Control(HeaterID_1, 1, 0);               // HeaterID_1 �̸��� ���͸� 0�� ������ �� On
    Set_Heater_Control(HeaterID_1, ON, 0);              // HeaterID_1 �̸��� ���͸� 0�� ������ �� On
    Set_Heater_Control(HeaterID_1, 0, 0);               // HeaterID_1 �̸��� ���͸� 0�� ������ �� Off
    Set_Heater_Control(HeaterID_1, OFF, 0);             // HeaterID_1 �̸��� ���͸� 0�� ������ �� Off
    Set_Heater_Control(HeaterID_1, 1, 5);               // HeaterID_1 �̸��� ���͸� 500ms ������ �� On
    Set_Heater_Control(HeaterID_1, ON, 5);              // HeaterID_1 �̸��� ���͸� 500ms ������ �� On
    Set_Heater_Control(HeaterID_1, 0, 5);               // HeaterID_1 �̸��� ���͸� 500ms ������ �� Off
    Set_Heater_Control(HeaterID_1, OFF, 5);             // HeaterID_1 �̸��� ���͸� 500ms ������ �� Off
    Hal_All_Heater_Off();                               // ��� ���� OFF
    u8Return = Get_Heater_Status(HeaterID_1);          // Heater 1 ���ۻ��� ��û, mu8Return : 1(ON), 0(OFF)
*/


#define NORMAL_HEATER_USE                               USE     // �Ϲ� ���� ��� ���� - 0(No Use), 1(Use)

#if (NORMAL_HEATER_USE == USE)

// �Ϲ� Heater ���� �ִ� 10�� ��� ����
typedef enum
{
    HEATER_ID_NONE = 0,                                 // 0 :
    HEATER_ID_HOT_TANK,                                 // 1 :
    HEATER_ID_MAX                                       // 11 Max, 11�� ������ �ȵ�
} EHeaterID_T;

#define HEATER_COUNT                                    1       // Heater�� ���� - MAX 10

// ������ skip���� ��ġ�Ͽ� ����ϸ�, �̻�� port�� 0���� ó��
// ������ ID�� �����ϰ� ��ġ
#define PORT_DEFINE_HEATER_1                            P7_bit.no0      // �¼� ��ũ ����
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

