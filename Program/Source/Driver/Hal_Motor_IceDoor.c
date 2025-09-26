/// @file     Hal_Motor_IceDoor.h
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Ice Door 모터 제어


#include "Global_Header.h"


#if (MOTOR_ICE_DOOR_USE == USE)

U8 gu8IceDoorControlTimer = 0;                  /// @brief  Ice Door 제어 시간 카운터
U8 gu8IceDoorMotorControl_3ms = 0;              /// @breif  Ice Door 스탭모터 전원 제어 주기 3ms 타이머

U8 F_IceDoorCW = 0;                             /// @brief  Ice Door CW 회전 Flag(살균 후 ICE Door Reset) - 0(미동작), 1(동작)

U16 gu16IceDoorStepMotorPulse = 0;              /// @brief  Ice Door 동작 Pulse

U8 gu8IceCloseTime = 0;                         /// @brief  얼음도어 닫힘 지연 시간(100ms 단위)

U16 gu16IceDoorCloseTimer_Min = 0;              /// @brief  Ice Door 주기적으로 닫는 동작을 위한 24시간 카운터 Min
U16 gu16IceDoorCloseTimer_Hour = 0;             /// @brief  Ice Door 주기적으로 닫는 동작을 위한 24시간 카운터 Hout
U16 gu16IceDoorCloseResetTimer = 0;             /// @brief  Ice Door 강제 닫힘 동작 시간

U8 F_IceDoorClose = 0;                          /// @brief  아이스 도어 닫힘 동작 Flag - 0(미동작), 1(동작)

U8 F_IceOpen = 0;                               /// @brief  Ice Door 열림 상태 Flag - 0(닫힘), 1(열림)


/// @brief      Ice Door 스탭모터 제어 관련 변수 초기화
/// @details    Ice Door 스탭모터 제어 관련 변수들을 모두 초기화 한다
/// @param      void
/// @return     void
void Hal_IceDoor_Initialize(void)
{
    gu8IceDoorControlTimer = 0;
    gu8IceDoorMotorControl_3ms = 0;

    F_IceDoorCW = 0;

    gu16IceDoorStepMotorPulse = 0;

    gu8IceCloseTime = 0;

    gu16IceDoorCloseTimer_Min = 0;
    gu16IceDoorCloseTimer_Hour = 0;
    gu16IceDoorCloseResetTimer = 0;

    F_IceDoorClose = 0;

    F_IceOpen = 0;
}


/// @brief      Ice Door CW 회전 상태 설정 함수
/// @details    Ice Door CW 회전 상태를 설정한다
/// @param      mu8Status : Ice Door CW 회전 상태 - 0(미동작), 1(동작)
/// @return     void
void Set_IceDoorCW_Status(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IceDoorCW = SET;
    }
    else
    {
        F_IceDoorCW = CLEAR;
    }
}


/// @brief      Ice Door CW 회전 상태 확인 함수
/// @details    Ice Door CW 회전 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Ice Door CW 회전 상태 - 0(미동작), 1(동작)
U8 Get_IceDoorCW_Status(void)
{
    if (F_IceDoorCW)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Ice Door control Timer(@1ms interrupt)
/// @details    Ice Door 제어 시간 주기를 카운트한다
/// @param      void
/// @return     void
void IceDoorControlTimer(void)
{
    if (gu8IceDoorControlTimer < ICE_DOOR_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8IceDoorControlTimer++;
    }
}


/// @brief      Ice Door 스탭모터 제어 진행 함수(@1ms interrupt)
/// @details    1ms Interrput 안에서 Ice Door 스탭모터 제어 진행 동작을 한다
/// @param      void
/// @return     void
void IceDoor_StepMotorInterrupt(void)
{
    if (++gu8IceDoorMotorControl_3ms >= 3)
    {
        gu8IceDoorMotorControl_3ms = 0;

        motor_ice_door_output();
    }
}


/// @brief      Ice Door 동작 스탭모터 제어 함수(@3ms)
/// @details    Ice Door 동작 스탭모터 제어를 한다
/// @param      void
/// @return     void
void motor_ice_door_output(void)
{
    U8 mu8IceDoorOpenBuf = 0;
    U8 mu8IceDoorCW_Buf = 0;
    U8 mu8IceOutBuf = 0;

    //======================================================// Ice Door 전환
    mu8IceDoorOpenBuf = Get_IceDoorStatus();
    mu8IceDoorCW_Buf = Get_IceDoorCW_Status();
    mu8IceOutBuf = Get_IceOutStatus();

    if (mu8IceDoorOpenBuf == TRUE)
    {   // 열림
        if (mu8IceDoorCW_Buf == FALSE)
        {
            Set_IceDoorCW_Status(SET);
        }
        else
        {

        }

        if (gu16IceDoorStepMotorPulse < ICE_DOOR_STEPMOTOR_ANGLE_PULSE)
        {
            gu16IceDoorStepMotorPulse++;
        }
        else
        {
            gu16IceDoorStepMotorPulse = ICE_DOOR_STEPMOTOR_ANGLE_PULSE;
            TURN_OFF_MOTOR_ICE_DOOR_1();
            TURN_OFF_MOTOR_ICE_DOOR_2();
            TURN_OFF_MOTOR_ICE_DOOR_3();
            TURN_OFF_MOTOR_ICE_DOOR_4();

            if (mu8IceOutBuf == FALSE)
            {   // 얼음 추출 미동작 상태인 경우
                Set_IceDoorStatus(CLEAR);       // Door 열림 완료 후 Off
            }
            else
            {

            }
        }
    }
    else                                                  // 닫힘
    {
        if(mu8IceDoorCW_Buf == SET)
        {
            gu16IceDoorStepMotorPulse = ICE_DOOR_STEPMOTOR_ANGLE_PULSE;
            Set_IceDoorCW_Status(CLEAR);
        }
        else
        {

        }

        if (gu16IceDoorStepMotorPulse > 0)
        {
            if (gu8IceCloseTime == 0)
            {
                gu16IceDoorStepMotorPulse--;
            }
            else
            {

            }
        }
        else
        {
            gu16IceDoorStepMotorPulse = 0;
            TURN_OFF_MOTOR_ICE_DOOR_1();
            TURN_OFF_MOTOR_ICE_DOOR_2();
            TURN_OFF_MOTOR_ICE_DOOR_3();
            TURN_OFF_MOTOR_ICE_DOOR_4();
        }
    }

    if ( (gu16IceDoorStepMotorPulse == 0) ||
         (gu16IceDoorStepMotorPulse == ICE_DOOR_STEPMOTOR_ANGLE_PULSE) )
    {
        TURN_OFF_MOTOR_ICE_DOOR_1();
        TURN_OFF_MOTOR_ICE_DOOR_2();
        TURN_OFF_MOTOR_ICE_DOOR_3();
        TURN_OFF_MOTOR_ICE_DOOR_4();
    }
    else
    {
        switch (gu16IceDoorStepMotorPulse % 8)
        {
            case 0:
                TURN_ON_MOTOR_ICE_DOOR_1();
                TURN_OFF_MOTOR_ICE_DOOR_2();
                TURN_OFF_MOTOR_ICE_DOOR_3();
                TURN_OFF_MOTOR_ICE_DOOR_4();
                break;

            case 1:
                TURN_ON_MOTOR_ICE_DOOR_1();
                TURN_OFF_MOTOR_ICE_DOOR_2();
                TURN_OFF_MOTOR_ICE_DOOR_3();
                TURN_ON_MOTOR_ICE_DOOR_4();
                break;

            case 2:
                TURN_OFF_MOTOR_ICE_DOOR_1();
                TURN_OFF_MOTOR_ICE_DOOR_2();
                TURN_OFF_MOTOR_ICE_DOOR_3();
                TURN_ON_MOTOR_ICE_DOOR_4();
                break;

            case 3:
                TURN_OFF_MOTOR_ICE_DOOR_1();
                TURN_OFF_MOTOR_ICE_DOOR_2();
                TURN_ON_MOTOR_ICE_DOOR_3();
                TURN_ON_MOTOR_ICE_DOOR_4();
                break;

            case 4:
                TURN_OFF_MOTOR_ICE_DOOR_1();
                TURN_OFF_MOTOR_ICE_DOOR_2();
                TURN_ON_MOTOR_ICE_DOOR_3();
                TURN_OFF_MOTOR_ICE_DOOR_4();
                break;

            case 5:
                TURN_OFF_MOTOR_ICE_DOOR_1();
                TURN_ON_MOTOR_ICE_DOOR_2();
                TURN_ON_MOTOR_ICE_DOOR_3();
                TURN_OFF_MOTOR_ICE_DOOR_4();
                break;

            case 6:
                TURN_OFF_MOTOR_ICE_DOOR_1();
                TURN_ON_MOTOR_ICE_DOOR_2();
                TURN_OFF_MOTOR_ICE_DOOR_3();
                TURN_OFF_MOTOR_ICE_DOOR_4();
                break;

            case 7:
                TURN_ON_MOTOR_ICE_DOOR_1();
                TURN_ON_MOTOR_ICE_DOOR_2();
                TURN_OFF_MOTOR_ICE_DOOR_3();
                TURN_OFF_MOTOR_ICE_DOOR_4();
                break;

            default:
                break;
        }
    }
}


/// @brief      Ice Door 닫힘 동작 상태 설정 함수
/// @details    Ice Door 닫힘 동작 상태를 설정한다
/// @param      mu8Status : Ice Door 닫힘 동작 상태 - 0(미동작), 1(동작)
/// @return     void
void Set_IceDoorCloseStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IceDoorClose = SET;
    }
    else
    {
        F_IceDoorClose = CLEAR;
    }
}


/// @brief      Ice Door 닫힘 동작 상태 확인 함수
/// @details    Ice Door 닫힘 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Ice Door 닫힘 동작 상태 - 0(미동작), 1(동작)
U8 Get_IceDoorCloseStatus(void)
{
    if (F_IceDoorClose)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      24시간마다 Ice Door를 주기적으로 닫는 동작을 하는 함수(@100ms)
/// @details    24시간마다 Ice Door를 주기적으로 닫는 동작을 수행한다
/// @param      void
/// @return     void
void ice_door_close_24_hour(void)
{
    U16 mu16_forced_close_time_min = 0;
    U16 mu16_forced_close_time_hour = 0;

    U8 mu8IceOutBuf = 0;
    U8 mu8IceDoorCloseBuf = 0;

    //====================================================
    /* 아이스 도어 주기적으로 닫는 로직( 24시간 기준으로 반복 )
     * 아이스 도어가 강제로 열린 경우를 가정해서 24시간 기준으로 닫아 준다.*/

    /*..hui [18-1-23오후 2:29:03] 일반 모드시 60분..*/
    mu16_forced_close_time_min = 36000;     // 60min = 3600sec@100ms
    /*..hui [18-1-23오후 2:29:09] 일반 모드시 24시간..*/
    mu16_forced_close_time_hour = 24;

    mu8IceOutBuf = Get_IceOutStatus();
    mu8IceDoorCloseBuf = Get_IceDoorCloseStatus();

    if (mu8IceOutBuf == TRUE)
    {
        gu16IceDoorCloseTimer_Min = 0;
        gu16IceDoorCloseTimer_Hour = 0;
    }
    else
    {

    }

    /*..hui [18-1-23오후 2:12:10] 60분 타이머..*/
    gu16IceDoorCloseTimer_Min++;

    if (gu16IceDoorCloseTimer_Min >= mu16_forced_close_time_min)
    {
        gu16IceDoorCloseTimer_Min = 0;
        gu16IceDoorCloseTimer_Hour++;
    }
    else
    {

    }

    /*..hui [18-1-23오후 2:12:15] 24시간 타이머..*/
    if (gu16IceDoorCloseTimer_Hour >= mu16_forced_close_time_hour)
    {
        Set_IceDoorCloseStatus(SET);
        gu16IceDoorCloseTimer_Hour = 0;
    }
    else
    {

    }

    if (mu8IceDoorCloseBuf == TRUE)
    {
        Set_IceDoorCloseStatus(CLEAR);

        gu16IceDoorCloseTimer_Min = 0;
        gu16IceDoorCloseTimer_Hour = 0;

        gu16IceDoorCloseResetTimer = 70;
        gu16IceDoorStepMotorPulse = ICE_DOOR_STEPMOTOR_ANGLE_PULSE;
    }
    else
    {

    }

    /*..hui [18-1-23오후 2:44:04] 아이스도어 강제 CLOSE중에 얼음 추출할경우 FULL OPEN..*/
    if(gu16IceDoorCloseResetTimer > 0)
    {
        gu16IceDoorCloseResetTimer--;
    }
    else
    {

    }
}



/// @brief      Ice Door 일반 동작 함수
/// @details    Ice Door 일반 동작을 수행한다
/// @param      void
/// @return     void
void Hal_IceDoorControl(void)
{
    if (gu8IceDoorControlTimer >= ICE_DOOR_CONTROL_TIME_PERIOD)
    {   // 100ms 마다
        gu8IceDoorControlTimer = 0;

        if (gu8IceCloseTime)
        {
            gu8IceCloseTime--;
        }

        ice_door_close_24_hour();
    }
}


/// @brief      Ice Door 열림 상태 설정 함수
/// @details    Ice Door 열림 상태 여부를 설정한다
/// @param      mu8Status : Ice Door 열림 상태 - 0(닫힘), 1(열림)
/// @return     void
void Set_IceDoorStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IceOpen = SET;
    }
    else
    {
        F_IceOpen = CLEAR;
    }
}


/// @brief      Ice Door 열림 상태 확인 함수
/// @details    Ice Door 열림 상태 여부를 확인하여 반환한다
/// @param      void
/// @return     return : Ice Door 열림 상태 - 0(닫힘), 1(열림)
U8 Get_IceDoorStatus(void)
{
    if (F_IceOpen)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}

#endif


// Hal Ice Door Module **************************************************************************************

/// @brief      Hal Ice Door Initilaize Module
/// @details    Ice Door 제어 관련 변수 모두 초기화
/// @param      void
/// @return     void
void Hal_IceDoor_Module_Initialize(void)
{
#if (MOTOR_ICE_DOOR_USE == USE)
    Hal_IceDoor_Initialize();
#endif
}


/// @brief      Hal Ice Door Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 Ice Door 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_IceDoor_Module_1ms_Control(void)
{
#if (MOTOR_ICE_DOOR_USE == USE)
    IceDoorControlTimer();
    IceDoor_StepMotorInterrupt();
#endif
}


/// @brief      Hal Ice Door Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 Ice Door 제어를 한다
/// @param      void
/// @return     void
void Hal_IceDoor_Module_Control(void)
{
#if (MOTOR_ICE_DOOR_USE == USE)
    Hal_IceDoorControl();
#endif
}

