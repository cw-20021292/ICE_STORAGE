/// @file     Hal_AC_Motor.c
/// @date     2025/03/20
/// @author   Jaejin Ham
/// @brief    AC Motor Control file


#include "Global_Header.h"


#if (AC_MOTOR_COUNT > 0)

/// @brief  AC Motor 제어 관리 자료형
typedef struct {
    U8  au8Status[AC_MOTOR_ID_MAX];        // AC Motor Status - 0(OFF), 1(CW), 2(CCW)
}   SAC_MotorData_T;

SAC_MotorData_T      SAC_MotorControlData;      // AC Motor Control Data


/// @brief      AC Motor Initialize
/// @details    AC 모터 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_Initialize(void)
{
    MEMSET( (void __FAR *)&SAC_MotorControlData, 0x00, sizeof(SAC_MotorData_T) );
}


/// @brief      AC Motor 동작 제어 함수
/// @details    AC 모터를 설정한 방향으로 동작 시킨다
/// @param      mu8MotorID : 동작 시키려는 모터 ID
///             mu8Direction : 동작 시키려는 방향 - 0(OFF), 1(CW), 2(CCW)
/// @return     void
void Set_AC_MotorControl(U8 mu8MotorID, U8 mu8Direction)
{
    SAC_MotorControlData.au8Status[mu8MotorID] = mu8Direction;
}


/// @brief      AC Motor control Function(@While)
/// @details    AC 모터를 While 문 안에서 제어한다
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
/// @details    AC 모터를 설정된 방향으로 HW 제어하여 동작 시킨다
/// @param      mu8MotorID : 동작시키려는 모터 ID
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
/// @details    AC 모터 1을 CW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_1_CW(void)
{
    TURN_ON_AC_MOTOR_1_CW();
    TURN_OFF_AC_MOTOR_1_CCW();
}


/// @brief      AC Motor 1 CCW ON
/// @details    AC 모터 1을 CCW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_1_CCW(void)
{
    TURN_OFF_AC_MOTOR_1_CW();
    TURN_ON_AC_MOTOR_1_CCW();
}


/// @brief      AC Motor 1 OFF
/// @details    AC 모터 1을 OFF 동작 시킨다
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
/// @details    AC 모터 2를 CW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_2_CW(void)
{
    TURN_ON_AC_MOTOR_2_CW();
    TURN_OFF_AC_MOTOR_2_CCW();
}


/// @brief      AC Motor 2 CCW ON
/// @details    AC 모터 2를 CCW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_2_CCW(void)
{
    TURN_OFF_AC_MOTOR_2_CW();
    TURN_ON_AC_MOTOR_2_CCW();
}


/// @brief      AC Motor 2 OFF
/// @details    AC 모터 2를 OFF 동작 시킨다
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
/// @details    AC 모터 3을 CW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_3_CW(void)
{
    TURN_ON_AC_MOTOR_3_CW();
    TURN_OFF_AC_MOTOR_3_CCW();
}


/// @brief      AC Motor 3 CCW ON
/// @details    AC 모터 3을 CCW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_3_CCW(void)
{
    TURN_OFF_AC_MOTOR_3_CW();
    TURN_ON_AC_MOTOR_3_CCW();
}


/// @brief      AC Motor 3 OFF
/// @details    AC 모터 1을 OFF 동작 시킨다
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
/// @details    AC 모터 4를 CW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_4_CW(void)
{
    TURN_ON_AC_MOTOR_4_CW();
    TURN_OFF_AC_MOTOR_4_CCW();
}


/// @brief      AC Motor 4 CCW ON
/// @details    AC 모터 4를 CCW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_4_CCW(void)
{
    TURN_OFF_AC_MOTOR_4_CW();
    TURN_ON_AC_MOTOR_4_CCW();
}


/// @brief      AC Motor 4 OFF
/// @details    AC 모터 4를 OFF 동작 시킨다
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
/// @details    AC 모터 5를 CW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_5_CW(void)
{
    TURN_ON_AC_MOTOR_5_CW();
    TURN_OFF_AC_MOTOR_5_CCW();
}


/// @brief      AC Motor 5 CCW ON
/// @details    AC 모터 5를 CCW 방향으로 동작 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_5_CCW(void)
{
    TURN_OFF_AC_MOTOR_5_CW();
    TURN_ON_AC_MOTOR_5_CCW();
}


/// @brief      AC Motor 5 OFF
/// @details    AC 모터 4를 OFF 동작 시킨다
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
/// @details    AC 모터 제어와 관련된 변수를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_AC_Motor_Module_Initialize(void)
{
#if (AC_MOTOR_COUNT > 0)
    Hal_AC_Motor_Initialize();              // AC 모터 관련 초기화
#endif
}


/// @brief      Hal AC Motor Module in While Loop
/// @details    Basic Loop 안 While문 안에서 AC 모터 제어를 한다
/// @param      void
/// @return     void
void Hal_AC_Motor_Module_Control(void)
{
#if (AC_MOTOR_COUNT > 0)
    AC_MotorControl();                      // AC 모터 전원 제어
#endif
}







