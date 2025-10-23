/// @file     Hal_Motor_IceDoor.h
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Ice Door ���� ����


#include "Global_Header.h"


#if (MOTOR_ICE_DOOR_USE == USE)

U8 gu8IceDoorControlTimer = 0;                  /// @brief  Ice Door ���� �ð� ī����
U8 gu8IceDoorMotorControl_3ms = 0;              /// @breif  Ice Door ���Ǹ��� ���� ���� �ֱ� 3ms Ÿ�̸�

U8 F_IceDoorCW = 0;                             /// @brief  Ice Door CW ȸ�� Flag(��� �� ICE Door Reset) - 0(�̵���), 1(����)

U16 gu16IceDoorStepMotorPulse = 0;              /// @brief  Ice Door ���� Pulse

U8 gu8IceCloseTime = 0;                         /// @brief  �������� ���� ���� �ð�(100ms ����)

U16 gu16IceDoorCloseTimer_Min = 0;              /// @brief  Ice Door �ֱ������� �ݴ� ������ ���� 24�ð� ī���� Min
U16 gu16IceDoorCloseTimer_Hour = 0;             /// @brief  Ice Door �ֱ������� �ݴ� ������ ���� 24�ð� ī���� Hout
U16 gu16IceDoorCloseResetTimer = 0;             /// @brief  Ice Door ���� ���� ���� �ð�

U8 F_IceDoorClose = 0;                          /// @brief  ���̽� ���� ���� ���� Flag - 0(�̵���), 1(����)

U8 F_IceOpen = 0;                               /// @brief  Ice Door ���� ���� Flag - 0(����), 1(����)


/// @brief      Ice Door ���Ǹ��� ���� ���� ���� �ʱ�ȭ
/// @details    Ice Door ���Ǹ��� ���� ���� �������� ��� �ʱ�ȭ �Ѵ�
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


/// @brief      Ice Door CW ȸ�� ���� ���� �Լ�
/// @details    Ice Door CW ȸ�� ���¸� �����Ѵ�
/// @param      mu8Status : Ice Door CW ȸ�� ���� - 0(�̵���), 1(����)
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


/// @brief      Ice Door CW ȸ�� ���� Ȯ�� �Լ�
/// @details    Ice Door CW ȸ�� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ice Door CW ȸ�� ���� - 0(�̵���), 1(����)
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
/// @details    Ice Door ���� �ð� �ֱ⸦ ī��Ʈ�Ѵ�
/// @param      void
/// @return     void
void IceDoorControlTimer(void)
{
    if (gu8IceDoorControlTimer < ICE_DOOR_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8IceDoorControlTimer++;
    }
}


/// @brief      Ice Door ���Ǹ��� ���� ���� �Լ�(@1ms interrupt)
/// @details    1ms Interrput �ȿ��� Ice Door ���Ǹ��� ���� ���� ������ �Ѵ�
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


/// @brief      Ice Door ���� ���Ǹ��� ���� �Լ�(@3ms)
/// @details    Ice Door ���� ���Ǹ��� ��� �Ѵ�
/// @param      void
/// @return     void
void motor_ice_door_output(void)
{
    U8 mu8IceDoorOpenBuf = 0;
    U8 mu8IceDoorCW_Buf = 0;
    U8 mu8IceOutBuf = 0;

    //======================================================// Ice Door ��ȯ
    mu8IceDoorOpenBuf = Get_IceDoorStatus();
    mu8IceDoorCW_Buf = Get_IceDoorCW_Status();
    mu8IceOutBuf = Get_IceOutStatus();

    if (mu8IceDoorOpenBuf == TRUE)
    {   // ����
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
            {   // ���� ���� �̵��� ������ ���
                Set_IceDoorStatus(CLEAR);       // Door ���� �Ϸ� �� Off
            }
            else
            {

            }
        }
    }
    else                                                  // ����
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


/// @brief      Ice Door ���� ���� ���� ���� �Լ�
/// @details    Ice Door ���� ���� ���¸� �����Ѵ�
/// @param      mu8Status : Ice Door ���� ���� ���� - 0(�̵���), 1(����)
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


/// @brief      Ice Door ���� ���� ���� Ȯ�� �Լ�
/// @details    Ice Door ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ice Door ���� ���� ���� - 0(�̵���), 1(����)
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


/// @brief      24�ð����� Ice Door�� �ֱ������� �ݴ� ������ �ϴ� �Լ�(@100ms)
/// @details    24�ð����� Ice Door�� �ֱ������� �ݴ� ������ �����Ѵ�
/// @param      void
/// @return     void
void ice_door_close_24_hour(void)
{
    U16 mu16_forced_close_time_min = 0;
    U16 mu16_forced_close_time_hour = 0;

    U8 mu8IceOutBuf = 0;
    U8 mu8IceDoorCloseBuf = 0;

    //====================================================
    /* ���̽� ���� �ֱ������� �ݴ� ����( 24�ð� �������� �ݺ� )
     * ���̽� ��� ������ ���� ��츦 �����ؼ� 24�ð� �������� �ݾ� �ش�.*/

    /*..hui [18-1-23���� 2:29:03] �Ϲ� ���� 60��..*/
    mu16_forced_close_time_min = 36000;     // 60min = 3600sec@100ms
    /*..hui [18-1-23���� 2:29:09] �Ϲ� ���� 24�ð�..*/
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

    /*..hui [18-1-23���� 2:12:10] 60�� Ÿ�̸�..*/
    gu16IceDoorCloseTimer_Min++;

    if (gu16IceDoorCloseTimer_Min >= mu16_forced_close_time_min)
    {
        gu16IceDoorCloseTimer_Min = 0;
        gu16IceDoorCloseTimer_Hour++;
    }
    else
    {

    }

    /*..hui [18-1-23���� 2:12:15] 24�ð� Ÿ�̸�..*/
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

    /*..hui [18-1-23���� 2:44:04] ���̽����� ���� CLOSE�߿� ���� �����Ұ�� FULL OPEN..*/
    if(gu16IceDoorCloseResetTimer > 0)
    {
        gu16IceDoorCloseResetTimer--;
    }
    else
    {

    }
}



/// @brief      Ice Door �Ϲ� ���� �Լ�
/// @details    Ice Door �Ϲ� ������ �����Ѵ�
/// @param      void
/// @return     void
void Hal_IceDoorControl(void)
{
    if (gu8IceDoorControlTimer >= ICE_DOOR_CONTROL_TIME_PERIOD)
    {   // 100ms ����
        gu8IceDoorControlTimer = 0;

        if (gu8IceCloseTime)
        {
            gu8IceCloseTime--;
        }

        ice_door_close_24_hour();
    }
}


/// @brief      Ice Door ���� ���� ���� �Լ�
/// @details    Ice Door ���� ���� ���θ� �����Ѵ�
/// @param      mu8Status : Ice Door ���� ���� - 0(����), 1(����)
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


/// @brief      Ice Door ���� ���� Ȯ�� �Լ�
/// @details    Ice Door ���� ���� ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ice Door ���� ���� - 0(����), 1(����)
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
/// @details    Ice Door ���� ���� ���� ��� �ʱ�ȭ
/// @param      void
/// @return     void
void Hal_IceDoor_Module_Initialize(void)
{
#if (MOTOR_ICE_DOOR_USE == USE)
    Hal_IceDoor_Initialize();
#endif
}


/// @brief      Hal Ice Door Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� Ice Door ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
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
/// @details    Basic Loop �� While �� �ȿ��� Ice Door ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_IceDoor_Module_Control(void)
{
#if (MOTOR_ICE_DOOR_USE == USE)
    Hal_IceDoorControl();
#endif
}

