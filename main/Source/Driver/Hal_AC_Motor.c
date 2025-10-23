/// @file     Hal_AC_Motor.c
/// @date     2025/03/20
/// @author   Jaejin Ham
/// @brief    AC Motor Control file


#include "Global_Header.h"


#if (AC_MOTOR_COUNT > 0)

/// @brief  AC Motor ���� ���� �ڷ���
typedef struct {
    U8  au8Status[AC_MOTOR_ID_MAX];        // AC Motor Status - 0(OFF), 1(CW), 2(CCW)
}   SAC_MotorData_T;

SAC_MotorData_T      SAC_MotorControlData;      // AC Motor Control Data


/// @brief      AC Motor Initialize
/// @details    AC ���� ����� ���õ� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_Initialize(void)
{
    MEMSET( (void __FAR *)&SAC_MotorControlData, 0x00, sizeof(SAC_MotorData_T) );
}


/// @brief      AC Motor ���� ���� �Լ�
/// @details    AC ���͸� ������ �������� ���� ��Ų��
/// @param      mu8MotorID : ���� ��Ű���� ���� ID
///             mu8Direction : ���� ��Ű���� ���� - 0(OFF), 1(CW), 2(CCW)
/// @return     void
void Set_AC_MotorControl(U8 mu8MotorID, U8 mu8Direction)
{
    SAC_MotorControlData.au8Status[mu8MotorID] = mu8Direction;
}


/// @brief      AC Motor control Function(@While)
/// @details    AC ���͸� While �� �ȿ��� �����Ѵ�
/// @param      void
/// @return     void
void AC_MotorControl(void)
{
    U8 mu8MotorBuf = 0;

    for (mu8MotorBuf = 1 ; mu8MotorBuf < AC_MOTOR_ID_MAX_COUNT ; mu8MotorBuf++)
    {
        Hal_AC_MotorControl(mu8MotorBuf);
    }
}


/// @brief      AC Motor HW Control
/// @details    AC ���͸� ������ �������� HW �����Ͽ� ���� ��Ų��
/// @param      mu8MotorID : ���۽�Ű���� ���� ID
/// @return     void
void Hal_AC_MotorControl(U8 mu8MotorID)
{
    switch (mu8MotorID)
    {
        case 1:
#if (AC_MOTOR_COUNT >= 1)
            if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CW)
            {
                Hal_AC_Motor_1_CW();
            }
            else if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CCW)
            {
                Hal_AC_Motor_1_CCW();
            }
            else
            {
                Hal_AC_Motor_1_OFF();
            }
#endif
            break;

        case 2:
#if (AC_MOTOR_COUNT >= 2)
            if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CW)
            {
                Hal_AC_Motor_2_CW();
            }
            else if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CCW)
            {
                Hal_AC_Motor_2_CCW();
            }
            else
            {
                Hal_AC_Motor_2_OFF();
            }
#endif
            break;

        case 3:
#if (AC_MOTOR_COUNT >= 3)
            if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CW)
            {
                Hal_AC_Motor_3_CW();
            }
            else if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CCW)
            {
                Hal_AC_Motor_3_CCW();
            }
            else
            {
                Hal_AC_Motor_3_OFF();
            }
#endif
            break;

        case 4:
#if (AC_MOTOR_COUNT >= 4)
            if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CW)
            {
                Hal_AC_Motor_4_CW();
            }
            else if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CCW)
            {
                Hal_AC_Motor_4_CCW();
            }
            else
            {
                Hal_AC_Motor_4_OFF();
            }
#endif
            break;

        case 5:
#if (AC_MOTOR_COUNT >= 5)
            if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CW)
            {
                Hal_AC_Motor_5_CW();
            }
            else if (SAC_MotorControlData.au8Status[mu8MotorID] == AC_MOTOR_CCW)
            {
                Hal_AC_Motor_5_CCW();
            }
            else
            {
                Hal_AC_Motor_5_OFF();
            }
#endif
            break;

        default:
#if (AC_MOTOR_COUNT >= 1)
            Hal_AC_Motor_1_OFF();
#endif

#if (AC_MOTOR_COUNT >= 2)
            Hal_AC_Motor_2_OFF();
#endif

#if (AC_MOTOR_COUNT >= 3)
            Hal_AC_Motor_3_OFF();
#endif

#if (AC_MOTOR_COUNT >= 4)
            Hal_AC_Motor_4_OFF();
#endif

#if (AC_MOTOR_COUNT >= 5)
            Hal_AC_Motor_5_OFF();
#endif
            break;

    }
}



#if (AC_MOTOR_COUNT >= 1)
/// @brief      AC Motor 1 CW ON
/// @details    AC ���� 1�� CW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_1_CW(void)
{
    TURN_ON_AC_MOTOR_1_CW();
    TURN_OFF_AC_MOTOR_1_CCW();
}


/// @brief      AC Motor 1 CCW ON
/// @details    AC ���� 1�� CCW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_1_CCW(void)
{
    TURN_OFF_AC_MOTOR_1_CW();
    TURN_ON_AC_MOTOR_1_CCW();
}


/// @brief      AC Motor 1 OFF
/// @details    AC ���� 1�� OFF ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_1_OFF(void)
{
    TURN_OFF_AC_MOTOR_1_CW();
    TURN_OFF_AC_MOTOR_1_CCW();
}
#endif


#if (AC_MOTOR_COUNT >= 2)
/// @brief      AC Motor 2 CW ON
/// @details    AC ���� 2�� CW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_2_CW(void)
{
    TURN_ON_AC_MOTOR_2_CW();
    TURN_OFF_AC_MOTOR_2_CCW();
}


/// @brief      AC Motor 2 CCW ON
/// @details    AC ���� 2�� CCW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_2_CCW(void)
{
    TURN_OFF_AC_MOTOR_2_CW();
    TURN_ON_AC_MOTOR_2_CCW();
}


/// @brief      AC Motor 2 OFF
/// @details    AC ���� 2�� OFF ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_2_OFF(void)
{
    TURN_OFF_AC_MOTOR_2_CW();
    TURN_OFF_AC_MOTOR_2_CCW();
}
#endif


#if (AC_MOTOR_COUNT >= 3)
/// @brief      AC Motor 3 CW ON
/// @details    AC ���� 3�� CW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_3_CW(void)
{
    TURN_ON_AC_MOTOR_3_CW();
    TURN_OFF_AC_MOTOR_3_CCW();
}


/// @brief      AC Motor 3 CCW ON
/// @details    AC ���� 3�� CCW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_3_CCW(void)
{
    TURN_OFF_AC_MOTOR_3_CW();
    TURN_ON_AC_MOTOR_3_CCW();
}


/// @brief      AC Motor 3 OFF
/// @details    AC ���� 1�� OFF ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_3_OFF(void)
{
    TURN_OFF_AC_MOTOR_3_CW();
    TURN_OFF_AC_MOTOR_3_CCW();
}
#endif


#if (AC_MOTOR_COUNT >= 4)
/// @brief      AC Motor 4 CW ON
/// @details    AC ���� 4�� CW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_4_CW(void)
{
    TURN_ON_AC_MOTOR_4_CW();
    TURN_OFF_AC_MOTOR_4_CCW();
}


/// @brief      AC Motor 4 CCW ON
/// @details    AC ���� 4�� CCW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_4_CCW(void)
{
    TURN_OFF_AC_MOTOR_4_CW();
    TURN_ON_AC_MOTOR_4_CCW();
}


/// @brief      AC Motor 4 OFF
/// @details    AC ���� 4�� OFF ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_4_OFF(void)
{
    TURN_OFF_AC_MOTOR_4_CW();
    TURN_OFF_AC_MOTOR_4_CCW();
}
#endif


#if (AC_MOTOR_COUNT >= 5)
/// @brief      AC Motor 5 CW ON
/// @details    AC ���� 5�� CW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_5_CW(void)
{
    TURN_ON_AC_MOTOR_5_CW();
    TURN_OFF_AC_MOTOR_5_CCW();
}


/// @brief      AC Motor 5 CCW ON
/// @details    AC ���� 5�� CCW �������� ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_5_CCW(void)
{
    TURN_OFF_AC_MOTOR_5_CW();
    TURN_ON_AC_MOTOR_5_CCW();
}


/// @brief      AC Motor 5 OFF
/// @details    AC ���� 4�� OFF ���� ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_5_OFF(void)
{
    TURN_OFF_AC_MOTOR_5_CW();
    TURN_OFF_AC_MOTOR_5_CCW();
}
#endif


#endif


// Hal AC Motor Module ***************************************************************************************

/// @brief      Hal AC Motor Initilaize Module
/// @details    AC ���� ����� ���õ� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_Module_Initialize(void)
{
#if (AC_MOTOR_COUNT > 0)
    Hal_AC_Motor_Initialize();              // AC ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal AC Motor Module in While Loop
/// @details    Basic Loop �� While�� �ȿ��� AC ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_AC_Motor_Module_Control(void)
{
#if (AC_MOTOR_COUNT > 0)
    AC_MotorControl();                      // AC ���� ���� ����
#endif
}







