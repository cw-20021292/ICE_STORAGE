/// @file     Hal_Stepmotor.h
/// @date     2025/03/12
/// @author   Jaejin Ham
/// @brief    Step Motor ���� ���� ��� file


#ifndef _HAL_STEPMOTOR_H_
#define _HAL_STEPMOTOR_H_


// ******************************************************************
//                          Stepmotor ����
// ******************************************************************
/*  Stepmotor ���� Function ����
    => Set_Stepmotor_Move(mu8MotorID, mu8Direction, mu8Pulse);         // ID�� �ش��ϴ� ���͸� Direction �������� Pulse��ŭ ��ũ �̵�
    => Get_Stepmotor_Move_Status(mu8MotorID);                          // ID�� �ش��ϴ� ���͸� �̵� ������ ������ �������� Ȯ���ϴ� �Լ� - 0(Stop), 1(Moving)

    ��� ��)
    Set_Stepmotor_Move(MOTOR_1, STEPMOTOR_CW, 100);                     // MOTOR_1 �̸��� ID�� Stepmotor�� CW �������� 100pulse �̵�
    Set_Stepmotor_Move(MOTOR_1, STEPMOTOR_CCW, 100);                    // MOTOR_1 �̸��� ID�� Stepmotor�� CCW �������� 100pulse �̵�

    Set_Stepmotor_Move(MOTOR_1, STEPMOTOR_CW, STEPMOTOR_1_ZERO_BACK);   // MOTOR_1 �̸��� ID�� Stepmotor�� CW �������� Zero Back ��ġ�� �̵�
    Set_Stepmotor_Move(MOTOR_1, STEPMOTOR_CCW, STEPMOTOR_1_ZERO_BACK);  // MOTOR_1 �̸��� ID�� Stepmotor�� CCW �������� Zero Back ��ġ�� �̵�

    Hal_Stepmotor_Stop(MOTOR_1);                                        // MOTOR_1 �̸��� ID�� Stepmotor�� Port ���¸� ��� 0���� �ʱ�ȭ
    mu8Return = Get_Stepmotor_Move_Status(MOTOR_1);                     // MOTOR_1 �̸��� ID�� Stepmotor�� ���� ���� Ȯ��
*/


// Stepmotor �ִ� 5�� ��� ����
typedef enum
{
    STEPMOTOR_ID_NONE = 0,                              // 0 :
    STEPMOTOR_ID_1,                                     // 1 :
    STEPMOTOR_ID_MAX                                    // 6 Max, 6�� ������ �ȵ�
} EStepmotorID_T;

#define STEPMOTOR_COUNT                                 1       // Stepmotor ���� - MAX 5

// ���� ���
// - 1���� : 1 -> 2 -> 3 -> 4 -> 1 -> ....
// - 1-2���� : 1 -> 1,2 -> 2 -> 2,3 -> 3 -> ....
// - 2���� : 1,2 -> 2,3 -> 3,4 -> 4,1 -> ....
#define STEPMOTOR_1_PHASE_EXCITATION                    0       // 1����
#define STEPMOTOR_2_PHASE_EXCITATION                    1       // 2����
#define STEPMOTOR_1_2_PHASE_EXCITATION                  2       // 1-2����
#define STEPMOTOR_CONTROL_TYPE                          STEPMOTOR_2_PHASE_EXCITATION    // Stepmotor ���� ��� - 0(1����), 1(2����), 2(1-2����)

#if (STEPMOTOR_COUNT >= 1)
#define DEFINE_STEPMOTOR_1_ZERO_BACK                    3000            // Stepmotor 1 Zero back Pulse
#define DEFINE_STEPMOTOR_1_CONTROL_PPS                  333             // Stepmotor 1 ���� PPS
#define PORT_DEFINE_STEPMOTOR_1_1                       P1_bit.no0      // Stepmotor 1 A+
#define PORT_DEFINE_STEPMOTOR_1_2                       P9_bit.no0      // Stepmotor 1 A-
#define PORT_DEFINE_STEPMOTOR_1_3                       P9_bit.no1      // Stepmotor 1 B+
#define PORT_DEFINE_STEPMOTOR_1_4                       P9_bit.no2      // Stepmotor 1 B-
#endif
#if (STEPMOTOR_COUNT >= 2)
#define DEFINE_STEPMOTOR_2_ZERO_BACK                    0               // Stepmotor 2 Zero back Pulse
#define DEFINE_STEPMOTOR_2_CONTROL_PPS                  0               // Stepmotor 2 ���� PPS
#define PORT_DEFINE_STEPMOTOR_2_1                       0               // Stepmotor 2 A+
#define PORT_DEFINE_STEPMOTOR_2_2                       0               // Stepmotor 2 A-
#define PORT_DEFINE_STEPMOTOR_2_3                       0               // Stepmotor 2 B
#define PORT_DEFINE_STEPMOTOR_2_4                       0               // Stepmotor 2 B-
#endif
#if (STEPMOTOR_COUNT >= 3)
#define DEFINE_STEPMOTOR_3_ZERO_BACK                    0               // Stepmotor 3 Zero back Pulse
#define DEFINE_STEPMOTOR_3_CONTROL_PPS                  0               // Stepmotor 3 ���� PPS
#define PORT_DEFINE_STEPMOTOR_3_1                       0               // Stepmotor 3 A+
#define PORT_DEFINE_STEPMOTOR_3_2                       0               // Stepmotor 3 A-
#define PORT_DEFINE_STEPMOTOR_3_3                       0               // Stepmotor 3 B
#define PORT_DEFINE_STEPMOTOR_3_4                       0               // Stepmotor 3 B-
#endif
#if (STEPMOTOR_COUNT >= 4)
#define DEFINE_STEPMOTOR_4_ZERO_BACK                    0               // Stepmotor 4 Zero back Pulse
#define DEFINE_STEPMOTOR_4_CONTROL_PPS                  0               // Stepmotor 4 ���� PPS
#define PORT_DEFINE_STEPMOTOR_4_1                       0               // Stepmotor 4 A+
#define PORT_DEFINE_STEPMOTOR_4_2                       0               // Stepmotor 4 A-
#define PORT_DEFINE_STEPMOTOR_4_3                       0               // Stepmotor 4 B
#define PORT_DEFINE_STEPMOTOR_4_4                       0               // Stepmotor 4 B-
#endif
#if (STEPMOTOR_COUNT >= 5)
#define DEFINE_STEPMOTOR_5_ZERO_BACK                    0               // Stepmotor 5 Zero back Pulse
#define DEFINE_STEPMOTOR_5_CONTROL_PPS                  0               // Stepmotor 5 ���� PPS
#define PORT_DEFINE_STEPMOTOR_5_1                       0               // Stepmotor 5 A+
#define PORT_DEFINE_STEPMOTOR_5_2                       0               // Stepmotor 5 A-
#define PORT_DEFINE_STEPMOTOR_5_3                       0               // Stepmotor 5 B
#define PORT_DEFINE_STEPMOTOR_5_4                       0               // Stepmotor 5 B-
#endif


#if (STEPMOTOR_COUNT > 0)

#define STEPMOTOR_ID_MAX_COUNT                  STEPMOTOR_ID_MAX

#if (STEPMOTOR_COUNT >= 1)
#define STEPMOTOR_1_ZERO_BACK                   DEFINE_STEPMOTOR_1_ZERO_BACK
#define STEPMOTOR_1_CONTROL_PPS_MS              (1000 / DEFINE_STEPMOTOR_1_CONTROL_PPS)
#define P_STEPMOTOR_1_1                         PORT_DEFINE_STEPMOTOR_1_1
#define TURN_ON_STEPMOTOR_1_1()                 { P_STEPMOTOR_1_1 = 1; }
#define TURN_OFF_STEPMOTOR_1_1()                { P_STEPMOTOR_1_1 = 0; }
#define GET_STATUS_STEPMOTOR_1_1()              ( P_STEPMOTOR_1_1 )
#define P_STEPMOTOR_1_2                         PORT_DEFINE_STEPMOTOR_1_2
#define TURN_ON_STEPMOTOR_1_2()                 { P_STEPMOTOR_1_2 = 1; }
#define TURN_OFF_STEPMOTOR_1_2()                { P_STEPMOTOR_1_2 = 0; }
#define GET_STATUS_STEPMOTOR_1_2()              ( P_STEPMOTOR_1_2 )
#define P_STEPMOTOR_1_3                         PORT_DEFINE_STEPMOTOR_1_3
#define TURN_ON_STEPMOTOR_1_3()                 { P_STEPMOTOR_1_3 = 1; }
#define TURN_OFF_STEPMOTOR_1_3()                { P_STEPMOTOR_1_3 = 0; }
#define GET_STATUS_STEPMOTOR_1_3()              ( P_STEPMOTOR_1_3 )
#define P_STEPMOTOR_1_4                         PORT_DEFINE_STEPMOTOR_1_4
#define TURN_ON_STEPMOTOR_1_4()                 { P_STEPMOTOR_1_4 = 1; }
#define TURN_OFF_STEPMOTOR_1_4()                { P_STEPMOTOR_1_4 = 0; }
#define GET_STATUS_STEPMOTOR_1_4()              ( P_STEPMOTOR_1_4 )
#endif
#if (STEPMOTOR_COUNT >= 2)
#define STEPMOTOR_2_ZERO_BACK                   DEFINE_STEPMOTOR_2_ZERO_BACK
#define STEPMOTOR_2_CONTROL_PPS_MS              (1000 / DEFINE_STEPMOTOR_2_CONTROL_PPS)
#define P_STEPMOTOR_2_1                         PORT_DEFINE_STEPMOTOR_2_1
#define TURN_ON_STEPMOTOR_2_1()                 { P_STEPMOTOR_2_1 = 1; }
#define TURN_OFF_STEPMOTOR_2_1()                { P_STEPMOTOR_2_1 = 0; }
#define GET_STATUS_STEPMOTOR_2_1()              ( P_STEPMOTOR_2_1 )
#define P_STEPMOTOR_2_2                         PORT_DEFINE_STEPMOTOR_2_2
#define TURN_ON_STEPMOTOR_2_2()                 { P_STEPMOTOR_2_2 = 1; }
#define TURN_OFF_STEPMOTOR_2_2()                { P_STEPMOTOR_2_2 = 0; }
#define GET_STATUS_STEPMOTOR_2_2()              ( P_STEPMOTOR_2_2 )
#define P_STEPMOTOR_2_3                         PORT_DEFINE_STEPMOTOR_2_3
#define TURN_ON_STEPMOTOR_2_3()                 { P_STEPMOTOR_2_3 = 1; }
#define TURN_OFF_STEPMOTOR_2_3()                { P_STEPMOTOR_2_3 = 0; }
#define GET_STATUS_STEPMOTOR_2_3()              ( P_STEPMOTOR_2_3 )
#define P_STEPMOTOR_2_4                         PORT_DEFINE_STEPMOTOR_2_4
#define TURN_ON_STEPMOTOR_2_4()                 { P_STEPMOTOR_2_4 = 1; }
#define TURN_OFF_STEPMOTOR_2_4()                { P_STEPMOTOR_2_4 = 0; }
#define GET_STATUS_STEPMOTOR_2_4()              ( P_STEPMOTOR_2_4 )
#endif
#if (STEPMOTOR_COUNT >= 3)
#define STEPMOTOR_3_ZERO_BACK                   DEFINE_STEPMOTOR_3_ZERO_BACK
#define STEPMOTOR_3_CONTROL_PPS_MS              (1000 / DEFINE_STEPMOTOR_3_CONTROL_PPS)
#define P_STEPMOTOR_3_1                         PORT_DEFINE_STEPMOTOR_3_1
#define TURN_ON_STEPMOTOR_3_1()                 { P_STEPMOTOR_3_1 = 1; }
#define TURN_OFF_STEPMOTOR_3_1()                { P_STEPMOTOR_3_1 = 0; }
#define GET_STATUS_STEPMOTOR_3_1()              ( P_STEPMOTOR_3_1 )
#define P_STEPMOTOR_3_2                         PORT_DEFINE_STEPMOTOR_3_2
#define TURN_ON_STEPMOTOR_3_2()                 { P_STEPMOTOR_3_2 = 1; }
#define TURN_OFF_STEPMOTOR_3_2()                { P_STEPMOTOR_3_2 = 0; }
#define GET_STATUS_STEPMOTOR_3_2()              ( P_STEPMOTOR_3_2 )
#define P_STEPMOTOR_3_3                         PORT_DEFINE_STEPMOTOR_3_3
#define TURN_ON_STEPMOTOR_3_3()                 { P_STEPMOTOR_3_3 = 1; }
#define TURN_OFF_STEPMOTOR_3_3()                { P_STEPMOTOR_3_3 = 0; }
#define GET_STATUS_STEPMOTOR_3_3()              ( P_STEPMOTOR_3_3 )
#define P_STEPMOTOR_3_4                         PORT_DEFINE_STEPMOTOR_3_4
#define TURN_ON_STEPMOTOR_3_4()                 { P_STEPMOTOR_3_4 = 1; }
#define TURN_OFF_STEPMOTOR_3_4()                { P_STEPMOTOR_3_4 = 0; }
#define GET_STATUS_STEPMOTOR_3_4()              ( P_STEPMOTOR_3_4 )
#endif
#if (STEPMOTOR_COUNT >= 4)
#define STEPMOTOR_4_ZERO_BACK                   DEFINE_STEPMOTOR_4_ZERO_BACK
#define STEPMOTOR_4_CONTROL_PPS_MS              (1000 / DEFINE_STEPMOTOR_4_CONTROL_PPS)
#define P_STEPMOTOR_4_1                         PORT_DEFINE_STEPMOTOR_4_1
#define TURN_ON_STEPMOTOR_4_1()                 { P_STEPMOTOR_4_1 = 1; }
#define TURN_OFF_STEPMOTOR_4_1()                { P_STEPMOTOR_4_1 = 0; }
#define GET_STATUS_STEPMOTOR_4_1()              ( P_STEPMOTOR_4_1 )
#define P_STEPMOTOR_4_2                         PORT_DEFINE_STEPMOTOR_4_2
#define TURN_ON_STEPMOTOR_4_2()                 { P_STEPMOTOR_4_2 = 1; }
#define TURN_OFF_STEPMOTOR_4_2()                { P_STEPMOTOR_4_2 = 0; }
#define GET_STATUS_STEPMOTOR_4_2()              ( P_STEPMOTOR_4_2 )
#define P_STEPMOTOR_4_3                         PORT_DEFINE_STEPMOTOR_4_3
#define TURN_ON_STEPMOTOR_4_3()                 { P_STEPMOTOR_4_3 = 1; }
#define TURN_OFF_STEPMOTOR_4_3()                { P_STEPMOTOR_4_3 = 0; }
#define GET_STATUS_STEPMOTOR_4_3()              ( P_STEPMOTOR_4_3 )
#define P_STEPMOTOR_4_4                         PORT_DEFINE_STEPMOTOR_4_4
#define TURN_ON_STEPMOTOR_4_4()                 { P_STEPMOTOR_4_4 = 1; }
#define TURN_OFF_STEPMOTOR_4_4()                { P_STEPMOTOR_4_4 = 0; }
#define GET_STATUS_STEPMOTOR_4_4()              ( P_STEPMOTOR_4_4 )
#endif
#if (STEPMOTOR_COUNT >= 5)
#define STEPMOTOR_5_ZERO_BACK                   DEFINE_STEPMOTOR_5_ZERO_BACK
#define STEPMOTOR_5_CONTROL_PPS_MS              (1000 / DEFINE_STEPMOTOR_5_CONTROL_PPS)
#define P_STEPMOTOR_5_1                         PORT_DEFINE_STEPMOTOR_5_1
#define TURN_ON_STEPMOTOR_5_1()                 { P_STEPMOTOR_5_1 = 1; }
#define TURN_OFF_STEPMOTOR_5_1()                { P_STEPMOTOR_5_1 = 0; }
#define GET_STATUS_STEPMOTOR_5_1()              ( P_STEPMOTOR_5_1 )
#define P_STEPMOTOR_5_2                         PORT_DEFINE_STEPMOTOR_5_2
#define TURN_ON_STEPMOTOR_5_2()                 { P_STEPMOTOR_5_2 = 1; }
#define TURN_OFF_STEPMOTOR_5_2()                { P_STEPMOTOR_5_2 = 0; }
#define GET_STATUS_STEPMOTOR_5_2()              ( P_STEPMOTOR_5_2 )
#define P_STEPMOTOR_5_3                         PORT_DEFINE_STEPMOTOR_5_3
#define TURN_ON_STEPMOTOR_5_3()                 { P_STEPMOTOR_5_3 = 1; }
#define TURN_OFF_STEPMOTOR_5_3()                { P_STEPMOTOR_5_3 = 0; }
#define GET_STATUS_STEPMOTOR_5_3()              ( P_STEPMOTOR_5_3 )
#define P_STEPMOTOR_5_4                         PORT_DEFINE_STEPMOTOR_5_4
#define TURN_ON_STEPMOTOR_5_4()                 { P_STEPMOTOR_5_4 = 1; }
#define TURN_OFF_STEPMOTOR_5_4()                { P_STEPMOTOR_5_4 = 0; }
#define GET_STATUS_STEPMOTOR_5_4()              ( P_STEPMOTOR_5_4 )
#endif

#define STEPMOTOR_CW                            0
#define STEPMOTOR_CCW                           1

void Hal_Stepmotor_Initialize(void);
void Set_Stepmotor_Move(U8 mu8MotorID, U8 mu8Direction, U16 mu8Pulse);
U8 Get_Stepmotor_Move_Status(U8 mu8MotorID);
void Hal_StepmotorControl(void);
void Stepmotor_Control(U8 mu8MotorID);
void Stepmotor_1_Control(U8 mu8Step);
void Stepmotor_2_Control(U8 mu8Step);
void Stepmotor_3_Control(U8 mu8Step);
void Stepmotor_4_Control(U8 mu8Step);
void Stepmotor_5_Control(U8 mu8Step);
void Hal_Stepmotor_Stop(U8 mu8MotorID);

#endif


void Hal_Stepmotor_Module_Initialize(void);
void Hal_Stepmotor_Module_1ms_Control(void);

#endif

