/// @file     Hal_AC_Motor.h
/// @date     2025/03/20
/// @author   Jaejin Ham
/// @brief    AC Motor Control file


#ifndef _HAL_AC_MOTOR_H_
#define _HAL_AC_MOTOR_H_


/*  AC Motor ���� Function ����
    => Set_AC_MotorControl(Motor ID, Direction);                // ID�� �ش��ϴ� ���͸� Direction �������� ȸ�� : Direction - 0(OFF), 1(CW), 2(CCW)

    ��� ��)
    Set_AC_MotorControl(AC_MOTOR_ID_ICE_TRAY, AC_MOTOR_CW);     // AC_MOTOR_ID_ICE_TRAY ID�� ���͸� CW �������� ����
    Set_AC_MotorControl(AC_MOTOR_ID_ICE_TRAY, AC_MOTOR_CCW);    // AC_MOTOR_ID_ICE_TRAY ID�� ���͸� CCW �������� ����
    Set_AC_MotorControl(AC_MOTOR_ID_ICE_TRAY, AC_MOTOR_OFF);    // AC_MOTOR_ID_ICE_TRAY ID�� ���͸� ���� ����
*/

// AC Motor �ִ� 5�� ��� ����
typedef enum
{
    AC_MOTOR_ID_NONE = 0,                               // 0 :
    AC_MOTOR_ID_ICE_OUT,                                // 1 :
    AC_MOTOR_ID_ICE_TRAY,                               // 2 :
    AC_MOTOR_ID_MAX                                     // 6 Max, 6�� ������ �ȵ�
} EAC_MotorID_T;

#define AC_MOTOR_COUNT                   0              // AC Motor ���� - MAX 5

// AC Motor port ����
// ������ skip���� ��ġ�Ͽ� ����ϸ�, �̻�� port�� 0���� ó��
// ������ ID�� �����ϰ� ��ġ
#if (AC_MOTOR_COUNT >= 1)
#define PORT_DEFINE_AC_MOTOR_1_CW        0
#define PORT_DEFINE_AC_MOTOR_1_CCW       0
#endif

#if (AC_MOTOR_COUNT >= 2)
#define PORT_DEFINE_AC_MOTOR_2_CW        0
#define PORT_DEFINE_AC_MOTOR_2_CCW       0
#endif

#if (AC_MOTOR_COUNT >= 3)
#define PORT_DEFINE_AC_MOTOR_3_CW        0
#define PORT_DEFINE_AC_MOTOR_3_CCW       0
#endif

#if (AC_MOTOR_COUNT >= 4)
#define PORT_DEFINE_AC_MOTOR_4_CW        0
#define PORT_DEFINE_AC_MOTOR_4_CCW       0
#endif

#if (AC_MOTOR_COUNT >= 5)
#define PORT_DEFINE_AC_MOTOR_5_CW        0
#define PORT_DEFINE_AC_MOTOR_5_CCW       0
#endif


#if (AC_MOTOR_COUNT > 0)

#define AC_MOTOR_ID_MAX_COUNT            AC_MOTOR_ID_MAX

// AC Motor�� ȸ�� ���� ������
#define AC_MOTOR_OFF                    0           // Motor OFF
#define AC_MOTOR_CW                     1           // Motor CW ���� ����
#define AC_MOTOR_CCW                    2           // Motor CCW ���� ����

#if (AC_MOTOR_COUNT >= 1)
#define P_AC_MOTOR_1_CW                 PORT_DEFINE_AC_MOTOR_1_CW
#define TURN_ON_AC_MOTOR_1_CW()         { P_AC_MOTOR_1_CW = 1; }
#define TURN_OFF_AC_MOTOR_1_CW()        { P_AC_MOTOR_1_CW = 0; }
#define GET_STATUS_AC_MOTOR_1_CW()      ( P_AC_MOTOR_1_CW )

#define P_AC_MOTOR_1_CCW                PORT_DEFINE_AC_MOTOR_1_CCW
#define TURN_ON_AC_MOTOR_1_CCW()        { P_AC_MOTOR_1_CCW = 1; }
#define TURN_OFF_AC_MOTOR_1_CCW()       { P_AC_MOTOR_1_CCW = 0; }
#define GET_STATUS_AC_MOTOR_1_CCW()     ( P_AC_MOTOR_1_CCW )
#endif

#if (AC_MOTOR_COUNT >= 2)
#define P_AC_MOTOR_2_CW                 PORT_DEFINE_AC_MOTOR_2_CW
#define TURN_ON_AC_MOTOR_2_CW()         { P_AC_MOTOR_2_CW = 1; }
#define TURN_OFF_AC_MOTOR_2_CW()        { P_AC_MOTOR_2_CW = 0; }
#define GET_STATUS_AC_MOTOR_2_CW()      ( P_AC_MOTOR_2_CW )

#define P_AC_MOTOR_2_CCW                PORT_DEFINE_AC_MOTOR_2_CCW
#define TURN_ON_AC_MOTOR_2_CCW()        { P_AC_MOTOR_2_CCW = 1; }
#define TURN_OFF_AC_MOTOR_2_CCW()       { P_AC_MOTOR_2_CCW = 0; }
#define GET_STATUS_AC_MOTOR_2_CCW()     ( P_AC_MOTOR_2_CCW )
#endif

#if (AC_MOTOR_COUNT >= 3)
#define P_AC_MOTOR_3_CW                 PORT_DEFINE_AC_MOTOR_3_CW
#define TURN_ON_AC_MOTOR_3_CW()         { P_AC_MOTOR_3_CW = 1; }
#define TURN_OFF_AC_MOTOR_3_CW()        { P_AC_MOTOR_3_CW = 0; }
#define GET_STATUS_AC_MOTOR_3_CW()      ( P_AC_MOTOR_3_CW )

#define P_AC_MOTOR_3_CCW                PORT_DEFINE_AC_MOTOR_3_CCW
#define TURN_ON_AC_MOTOR_3_CCW()        { P_AC_MOTOR_3_CCW = 1; }
#define TURN_OFF_AC_MOTOR_3_CCW()       { P_AC_MOTOR_3_CCW = 0; }
#define GET_STATUS_AC_MOTOR_3_CCW()     ( P_AC_MOTOR_3_CCW )
#endif

#if (AC_MOTOR_COUNT >= 4)
#define P_AC_MOTOR_4_CW                 PORT_DEFINE_AC_MOTOR_4_CW
#define TURN_ON_AC_MOTOR_4_CW()         { P_AC_MOTOR_4_CW = 1; }
#define TURN_OFF_AC_MOTOR_4_CW()        { P_AC_MOTOR_4_CW = 0; }
#define GET_STATUS_AC_MOTOR_4_CW()      ( P_AC_MOTOR_4_CW )

#define P_AC_MOTOR_4_CCW                PORT_DEFINE_AC_MOTOR_4_CCW
#define TURN_ON_AC_MOTOR_4_CCW()        { P_AC_MOTOR_4_CCW = 1; }
#define TURN_OFF_AC_MOTOR_4_CCW()       { P_AC_MOTOR_4_CCW = 0; }
#define GET_STATUS_AC_MOTOR_4_CCW()     ( P_AC_MOTOR_4_CCW )
#endif

#if (AC_MOTOR_COUNT >= 5)
#define P_AC_MOTOR_5_CW                 PORT_DEFINE_AC_MOTOR_5_CW
#define TURN_ON_AC_MOTOR_5_CW()         { P_AC_MOTOR_5_CW = 1; }
#define TURN_OFF_AC_MOTOR_5_CW()        { P_AC_MOTOR_5_CW = 0; }
#define GET_STATUS_AC_MOTOR_5_CW()      ( P_AC_MOTOR_5_CW )

#define P_AC_MOTOR_5_CCW                PORT_DEFINE_AC_MOTOR_5_CCW
#define TURN_ON_AC_MOTOR_5_CCW()        { P_AC_MOTOR_5_CCW = 1; }
#define TURN_OFF_AC_MOTOR_5_CCW()       { P_AC_MOTOR_5_CCW = 0; }
#define GET_STATUS_AC_MOTOR_5_CCW()     ( P_AC_MOTOR_5_CCW )
#endif

void Hal_AC_Motor_Initialize(void);
void Set_AC_MotorControl(U8 mu8MotorID, U8 mu8Direction);
void AC_MotorControl(void);
void Hal_AC_MotorControl(U8 mu8MotorID);

#if (AC_MOTOR_COUNT >= 1)
void Hal_AC_Motor_1_CW(void);
void Hal_AC_Motor_1_CCW(void);
void Hal_AC_Motor_1_OFF(void);
#endif

#if (AC_MOTOR_COUNT >= 2)
void Hal_AC_Motor_2_CW(void);
void Hal_AC_Motor_2_CCW(void);
void Hal_AC_Motor_2_OFF(void);
#endif

#if (AC_MOTOR_COUNT >= 3)
void Hal_AC_Motor_3_CW(void);
void Hal_AC_Motor_3_CCW(void);
void Hal_AC_Motor_3_OFF(void);
#endif

#if (AC_MOTOR_COUNT >= 4)
void Hal_AC_Motor_4_CW(void);
void Hal_AC_Motor_4_CCW(void);
void Hal_AC_Motor_4_OFF(void);
#endif

#if (AC_MOTOR_COUNT >= 5)
void Hal_AC_Motor_5_CW(void);
void Hal_AC_Motor_5_CCW(void);
void Hal_AC_Motor_5_OFF(void);
#endif


#endif


void Hal_AC_Motor_Module_Initialize(void);
void Hal_AC_Motor_Module_Control(void);

#endif

