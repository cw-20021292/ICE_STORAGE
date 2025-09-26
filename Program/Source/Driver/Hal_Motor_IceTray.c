/// @file     Hal_Motor_IceTray.c
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Ice Tray ���� ����


#include "Global_Header.h"


#if (MOTOR_ICE_TRAY_USE == USE)

U8 gu8IceTrayControlTimer = 0;                          /// @brief  Ice Tray ���� �ð� ī����
U8 gu8IceTrayMotorControl_5ms = 0;                      /// @breif  Ice Tray ���Ǹ��� ���� ���� �ֱ� 5ms Ÿ�̸�

U16 gu16_ice_tray_target = 0;                           /// @brief  Ice Tray ���Ǹ��� ���� ��ǥ ��ġ ����
U16 gu16_ice_tray_current = 0;                          /// @brief  Ice Tray ���Ǹ��� ���� ���� ��ġ ����

U8 gu8_ice_tray_hold_start_step = 0;                    /// @brief  Ice Tray ���Ǹ��� ���� Hold �ð�
U8 gu8_ice_tray_hold_stop_step = 0;                     /// @brief  Ice Tray ���Ǹ��� ���� Hold �ð�
U8 gu8_ice_tray_current_step = 0;                       /// @brief  Ice Tray ���Ǹ��� ���� ���� ���� ��ġ ����
U8 gu8_ice_tray_prev_step = 0;                          /// @brief  Ice Tray ���Ǹ��� ���� ���� ���� ��ġ ����

const U8 gu8IceTrayPhaseTable[ICE_TRAY_STEP_NUM] = { 0x01, 0x02, 0x03, 0x04 };

U8 gu8Ice_tray_mode = 0;                                /// @brief  Ice Tray ���� ���� ���

U8 gu8_ice_tray_init_step = 0;                          /// @brief  Ice Tray �ʱ�ȭ ���� ���� Step
U8 gu8_ice_tray_up_step = 0;                            /// @brief  Ice Tray Up ���� ���� Step
U8 gu8_ice_tray_down_step = 0;                          /// @brief  Ice Tray Down ���� ���� Step

U8 F_IceTrayCW = 0;                                     /// @brief  Ice Tray CW ȸ�� Flag - 0(�̵���), 1(����)

U16 gu16IceTrayStepMotorPulse = 0;                      /// @brief  Ice Tray ���� Pulse

U8 gu8_ice_tray_down_timer = 0;                         /// @brief  Ice Tray Down ���� �ð�

SIceTrayStepmotor_T SIceTrayStepMotor;                  /// @brief  Ice Tray ���� ������ ����ü

U8 bit_tray_valve_output = 0;                           /// @brief  Ice Tray �Լ� ��� ���� ���� - 0(�̵���), 1(����)
U8 bit_tray_in_error_temporary = 0;                     /// @brief  Ice Tray �Լ� �ܼ� ���� ���� - 0(���� �̹߻�), 1(���� �߻�)
U16 gu16_tray_error_clear_timer = 0;                    /// @brief  Ice Tray �Լ� �ܼ� ���� ���� �ð�


/// @brief      Ice Tray ���Ǹ��� ���� ���� ���� �ʱ�ȭ
/// @details    Ice Tray ���Ǹ��� ���� ���� �������� ��� �ʱ�ȭ �Ѵ�
/// @param      void
/// @return     void
void Hal_IceTray_Initialize(void)
{
    gu8IceTrayControlTimer = 0;
    gu8IceTrayMotorControl_5ms = 0;

    gu16_ice_tray_target = 0;
    gu16_ice_tray_current = 0;

    gu8_ice_tray_hold_start_step = 0;
    gu8_ice_tray_hold_stop_step = 0;
    gu8_ice_tray_current_step = 0;
    gu8_ice_tray_prev_step = 0;

    gu8Ice_tray_mode = 0;

    gu8_ice_tray_init_step = 0;
    gu8_ice_tray_up_step = 0;
    gu8_ice_tray_down_step = 0;

    F_IceTrayCW = 0;

    gu16IceTrayStepMotorPulse = 0;
    gu8_ice_tray_down_timer = 0;

    MEMSET(&SIceTrayStepMotor, 0, sizeof(SIceTrayStepMotor));

    bit_tray_valve_output = 0;
    bit_tray_in_error_temporary = 0;
    gu16_tray_error_clear_timer = 0;
}


/// @brief      Ice Tray control Timer(@1ms interrupt)
/// @details    Ice Tray ���� �ð� �ֱ⸦ ī��Ʈ�Ѵ�
/// @param      void
/// @return     void
void IceTrayControlTimer(void)
{
    if (gu8IceTrayControlTimer < ICE_TRAY_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8IceTrayControlTimer++;
    }
}


/// @brief      Ice Tray ���Ǹ��� ���� ���� �Լ�(@1ms interrupt)
/// @details    1ms Interrput �ȿ��� Ice Tray ���Ǹ��� ���� ���� ������ �Ѵ�
/// @param      void
/// @return     void
void IceTray_StepMotorInterrupt(void)
{
    if (++gu8IceTrayMotorControl_5ms >= 5)
    {
        gu8IceTrayMotorControl_5ms = 0;

        control_ice_tray_motor();
    }
}


/// @brief      Ice Tray Control �Լ�
/// @details    Ice Tray ���� ������ �Ѵ�
/// @param      void
/// @return     void
void control_ice_tray(void)
{
    U8 mu8Ret = FALSE;


     /* Init */
    if (gu8Ice_tray_mode == ICE_TRAY_MODE_INIT)
    {
        mu8Ret = init_ice_tray();

        if (mu8Ret == TRUE)
        {
            gu8Ice_tray_mode = ICE_TRAY_MODE_NONE;
            gu8_ice_tray_init_step = 0;
        }
        else
        {

        }
        return;
    }

    /* DOWN */
    if (gu8Ice_tray_mode == ICE_TRAY_MODE_DOWN)
    {
        mu8Ret = down_ice_tray();
        if (mu8Ret == TRUE)
        {
            gu8Ice_tray_mode = ICE_TRAY_MODE_NONE;
            gu8_ice_tray_down_step = 0;
        }
        else
        {

        }
        return;
    }

    /* UP */
    if (gu8Ice_tray_mode == ICE_TRAY_MODE_UP)
    {
        mu8Ret = up_ice_tray();

        if( mu8Ret == TRUE )
        {
            gu8Ice_tray_mode = ICE_TRAY_MODE_NONE;
            gu8_ice_tray_up_step = 0;
        }
        else
        {

        }
        return;
    }
}


/// @brief      Ice Tray �ʱ�ȭ ���� ���� �Լ�
/// @details    Ice Tray �ʱ�ȭ ���� ��� �Ѵ�
/// @param      void
/// @return     return : �ʱ�ȭ ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 init_ice_tray(void)
{
    U8 mu8Return = 0;

    mu8Return = FALSE;

    switch (gu8_ice_tray_init_step)
    {
        case 0:
            ////gu16_ice_tray_target = 0;
            gu16_ice_tray_current = ICE_TRAY_INIT_PULSE;
            gu16_ice_tray_target = 0;

            gu8_ice_tray_init_step++;
            break;

        case 1:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu16_ice_tray_target = 0;
                gu16_ice_tray_current = 0;
                gu8_ice_tray_init_step++;
            }
            break;

        case 2:
            mu8Return = TRUE;
            break;

    }

    return  mu8Return;
}


/// @brief      Ice Tray Up ���� ���� �Լ�
/// @details    Ice Tray Up ���� ��� �Ѵ�
/// @param      void
/// @return     return : ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 up_ice_tray(void)
{
    U8 mu8Return = 0;

    mu8Return = FALSE;

    switch (gu8_ice_tray_up_step)
    {
        case 0:
            gu8_ice_tray_hold_start_step = ICE_TRAY_HOLD_STEP;
            gu8_ice_tray_hold_stop_step = ICE_TRAY_HOLD_STEP;

            /*..hui [25-3-17���� 2:24:09] �ִ� �޽���ŭ ���� �ø�..*/
            gu16_ice_tray_target = ICE_TRAY_UP_PULSE;
            gu16_ice_tray_current = 0;

            gu8_ice_tray_up_step++;
            break;

        case 1:
            if( gu16_ice_tray_target == gu16_ice_tray_current )
            {
                gu8_ice_tray_up_step++;
            }
            else
            {

            }
            break;

        case 2:
            mu8Return = TRUE;
            break;

    }

    return  mu8Return;

}


/// @brief      Ice Tray Down ���� ���� �Լ�
/// @details    Ice Tray Down ���� ��� �Ѵ�
/// @param      void
/// @return     return : ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 down_ice_tray(void)
{
    U8 mu8_delay_time = 0;
    U8 mu8Return = 0;

    mu8_delay_time = ICE_TRAY_DOWN_DELAY_TIME;

    mu8Return = FALSE;

    switch (gu8_ice_tray_down_step)
    {
        case 0:
            gu8_ice_tray_hold_start_step = ICE_TRAY_HOLD_STEP;
            gu8_ice_tray_hold_stop_step = ICE_TRAY_HOLD_STEP;

            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_1;
            gu8_ice_tray_down_timer = 0;

            gu8_ice_tray_down_step++;
            break;

        case 1:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_timer++;

                /*if( gu8_ice_tray_down_timer >= ICE_TRAY_DOWN_DELAY_TIME )*/
                if (gu8_ice_tray_down_timer >= mu8_delay_time)
                {
                    gu8_ice_tray_down_step++;
                    gu8_ice_tray_down_timer = 0;
                }
                else
                {

                }
            }
            break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
        case 2:
            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_2;
            gu8_ice_tray_down_timer = 0;

            gu8_ice_tray_down_step++;
            break;

        case 3:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_timer++;

                /*if( gu8_ice_tray_down_timer >= ICE_TRAY_DOWN_DELAY_TIME )*/
                if (gu8_ice_tray_down_timer >= mu8_delay_time)
                {
                    gu8_ice_tray_down_step++;
                    gu8_ice_tray_down_timer = 0;
                }
                else
                {

                }
            }
            break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
        case 4:
            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_3;
            gu8_ice_tray_down_timer = 0;

            gu8_ice_tray_down_step++;
            break;

        case 5:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_timer++;

                /*if( gu8_ice_tray_down_timer >= ICE_TRAY_DOWN_DELAY_TIME )*/
                if (gu8_ice_tray_down_timer >= mu8_delay_time)
                {
                    gu8_ice_tray_down_step++;
                    gu8_ice_tray_down_timer = 0;
                }
                else
                {

                }
            }
            break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
        case 6:
            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_4;
            gu8_ice_tray_down_timer = 0;

            gu8_ice_tray_down_step++;
            break;

        case 7:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_timer++;

                /*if( gu8_ice_tray_down_timer >= ICE_TRAY_DOWN_DELAY_TIME )*/
                if (gu8_ice_tray_down_timer >= mu8_delay_time)
                {
                    gu8_ice_tray_down_step++;
                    gu8_ice_tray_down_timer = 0;
                }
                else
                {

                }
            }
            break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
        case 8:
            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_5;
            gu8_ice_tray_down_timer = 0;

            gu8_ice_tray_down_step++;
            break;

        case 9:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_timer++;

                /*if( gu8_ice_tray_down_timer >= ICE_TRAY_DOWN_DELAY_TIME )*/
                if (gu8_ice_tray_down_timer >= mu8_delay_time)
                {
                    gu8_ice_tray_down_step++;
                    gu8_ice_tray_down_timer = 0;
                }
                else
                {

                }
            }
            break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
        case 10:
            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_6;
            gu8_ice_tray_down_timer = 0;

            gu8_ice_tray_down_step++;
            break;

        case 11:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_timer++;

                /*if( gu8_ice_tray_down_timer >= ICE_TRAY_DOWN_DELAY_TIME )*/
                if (gu8_ice_tray_down_timer >= mu8_delay_time)
                {
                    gu8_ice_tray_down_step++;
                    gu8_ice_tray_down_timer = 0;
                }
                else
                {

                }
            }
            break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
        case 12:
            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_7;
            gu8_ice_tray_down_timer = 0;

            gu8_ice_tray_down_step++;
            break;

        case 13:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_timer++;

                /*if( gu8_ice_tray_down_timer >= ICE_TRAY_DOWN_DELAY_TIME )*/
                if (gu8_ice_tray_down_timer >= mu8_delay_time)
                {
                    gu8_ice_tray_down_step++;
                    gu8_ice_tray_down_timer = 0;
                }
                else
                {

                }
            }
            break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
        case 14:
            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_8;
            gu8_ice_tray_down_timer = 0;

            gu8_ice_tray_down_step++;
            break;

        case 15:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_timer++;

                /*if( gu8_ice_tray_down_timer >= ICE_TRAY_DOWN_DELAY_TIME )*/
                if (gu8_ice_tray_down_timer >= mu8_delay_time)
                {
                    gu8_ice_tray_down_step++;
                    gu8_ice_tray_down_timer = 0;
                }
                else
                {

                }
            }
            break;

//////////////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////////////
        case 16:
            gu16_ice_tray_target = 0;
            /*..hui [25-3-17���� 2:24:27] �������� ����ֵ� �ִ� ���޽���ŭ ������..*/
            gu16_ice_tray_current = ICE_TRAY_DOWN_PULSE_STEP_9;

            gu8_ice_tray_down_step++;
            break;

        case 17:
            if (gu16_ice_tray_target == gu16_ice_tray_current)
            {
                gu8_ice_tray_down_step++;
            }
            break;

        case 18:
            mu8Return = TRUE;
            break;

    }

    return  mu8Return;
}


/// @brief      Ice Tray ���� ���Ǹ��� ���� �Լ�(@5ms)
/// @details    Ice Tray ���� ���Ǹ��� ��� �Ѵ�
/// @param      void
/// @return     void
void control_ice_tray_motor(void)
{
    U8 mu8Val = 0;

    /* START HOLD */
    if (gu8_ice_tray_hold_start_step > 0)
    {
        gu8_ice_tray_hold_start_step--;
        mu8Val = gu8IceTrayPhaseTable[gu8_ice_tray_prev_step];
    }
    else if (gu16_ice_tray_current == gu16_ice_tray_target)
    {
        /* STOP HOLD */
        if (gu8_ice_tray_hold_stop_step > 0)
        {
            gu8_ice_tray_hold_stop_step--;
            mu8Val = gu8IceTrayPhaseTable[gu8_ice_tray_current_step];
        }
        else
        {
            /* STOP */
            mu8Val = 0x00;
        }
    }
    /* MOVE FORWARD */
    else if (gu16_ice_tray_current < gu16_ice_tray_target)
    {
        /* OPEN */
        /*..hui [21-6-24���� 2:24:48] ������ ���� ���̱��..*/
        gu16_ice_tray_current++;

        gu8_ice_tray_current_step++;

        if (gu8_ice_tray_current_step >= ICE_TRAY_STEP_NUM)
        {
            gu8_ice_tray_current_step = 0;
        }
        else
        {

        }

        gu8_ice_tray_prev_step = gu8_ice_tray_current_step;

        mu8Val = gu8IceTrayPhaseTable[gu8_ice_tray_current_step];
    }
    /* MOVE BACKWARD */
    else if (gu16_ice_tray_current > gu16_ice_tray_target)
    {
        /* CLOSE */
        gu16_ice_tray_current--;

        if (gu8_ice_tray_current_step == 0)
        {
            gu8_ice_tray_current_step = ICE_TRAY_STEP_NUM - 1;
        }
        else
        {
            gu8_ice_tray_current_step--;
        }

        gu8_ice_tray_prev_step = gu8_ice_tray_current_step;

        mu8Val = gu8IceTrayPhaseTable[gu8_ice_tray_current_step];
    }
    else
    {

    }

    ice_tray_motor_operation(mu8Val);
}


/// @brief      Ice Tray ���� ���Ǹ��� HW ���� �Լ�
/// @details    Ice Tray ���� ���Ǹ��� HW Port ��� �Ѵ�
/// @param      void
/// @return     void
void ice_tray_motor_operation(U8 mu8Mode)
{
    switch (mu8Mode)
    {
        case 0:
            TURN_OFF_MOTOR_ICE_TRAY_1();
            TURN_OFF_MOTOR_ICE_TRAY_2();
            TURN_OFF_MOTOR_ICE_TRAY_3();
            TURN_OFF_MOTOR_ICE_TRAY_4();
            break;

        case 1:
            TURN_ON_MOTOR_ICE_TRAY_1();
            TURN_OFF_MOTOR_ICE_TRAY_2();
            TURN_OFF_MOTOR_ICE_TRAY_3();
            TURN_ON_MOTOR_ICE_TRAY_4();
            break;

        case 2:
            TURN_ON_MOTOR_ICE_TRAY_1();
            TURN_ON_MOTOR_ICE_TRAY_2();
            TURN_OFF_MOTOR_ICE_TRAY_3();
            TURN_OFF_MOTOR_ICE_TRAY_4();
            break;

        case 3:
            TURN_OFF_MOTOR_ICE_TRAY_1();
            TURN_ON_MOTOR_ICE_TRAY_2();
            TURN_ON_MOTOR_ICE_TRAY_3();
            TURN_OFF_MOTOR_ICE_TRAY_4();
            break;

        case 4:
            TURN_OFF_MOTOR_ICE_TRAY_1();
            TURN_OFF_MOTOR_ICE_TRAY_2();
            TURN_ON_MOTOR_ICE_TRAY_3();
            TURN_ON_MOTOR_ICE_TRAY_4();
            break;

        default:
            TURN_OFF_MOTOR_ICE_TRAY_1();
            TURN_OFF_MOTOR_ICE_TRAY_2();
            TURN_OFF_MOTOR_ICE_TRAY_3();
            TURN_OFF_MOTOR_ICE_TRAY_4();
            break;
    }
}


/// @brief      Ice Tray �ʱ�ȭ ���� ���� �Լ�
/// @details    Ice Tray �ʱ�ȭ ������ ���� ������ �Ѵ�
/// @param      void
/// @return     void
void run_init_ice_tray(void)
{
    gu8Ice_tray_mode = ICE_TRAY_MODE_INIT;
    gu8_ice_tray_init_step = 0;
    gu8_ice_tray_up_step = 0;
    gu8_ice_tray_down_step = 0;

    gu8_ice_tray_hold_start_step = ICE_TRAY_HOLD_STEP;
    gu8_ice_tray_hold_stop_step = ICE_TRAY_HOLD_STEP;
}


/// @brief      Ice Tray Down ���� ���� �Լ�
/// @details    Ice Tray Down ������ ���� ������ �Ѵ�
/// @param      void
/// @return     void
void run_down_ice_tray(void)
{
    if (gu8Ice_tray_mode != ICE_TRAY_MODE_DOWN)
    {
        gu8Ice_tray_mode = ICE_TRAY_MODE_DOWN;
        gu8_ice_tray_down_step = 0;
    }
    else
    {

    }
}


/// @brief      Ice Tray Up ���� ���� �Լ�
/// @details    Ice Tray Up ������ ���� ������ �Ѵ�
/// @param      void
/// @return     void
void run_up_ice_tray(void)
{
    if (gu8Ice_tray_mode != ICE_TRAY_MODE_UP)
    {
        gu8Ice_tray_mode = ICE_TRAY_MODE_UP;
        gu8_ice_tray_up_step = 0;
    }
    else
    {

    }
}


/// @brief      Ice Tray ���� ���� ���� �Լ�
/// @details    Ice Tray ���� ������ ���� ������ �Ѵ�
/// @param      void
/// @return     void
void run_stop_ice_tray(void)
{
    gu8Ice_tray_mode = ICE_TRAY_MODE_NONE;
    gu8_ice_tray_up_step = 0;
    gu8_ice_tray_down_step = 0;
    gu16_ice_tray_target = 0;
    gu16_ice_tray_current = 0;
}


/// @brief      Ice Tray �̵� ��� ���� �Լ�
/// @details    Ice Tray �̵� ��带 �����Ѵ�
/// @param      mu8Mode : Ice Tray �̵� ��� - 0(Stop), 1(Init), 2(Up), 3(Down)
/// @return     void
void Set_IceTrayMode(U8 mu8Mode)
{
    gu8Ice_tray_mode = mu8Mode;
}


/// @brief      Ice Tray �̵� ��� Ȯ�� �Լ�
/// @details    Ice Tray �̵� ��� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ice Tray �̵� ��� - 0(Stop), 1(Init), 2(Up), 3(Down)
U8 Get_IceTrayMode(void)
{
    return  gu8Ice_tray_mode;
}


/// @brief      Ice Tray ��� �Լ� ���� ���� ���� �Լ�
/// @details    Ice Tray ��� �Լ� ���� ���¸� �����Ѵ�
/// @param      mu8Status : Ice Tray �Լ� ��� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_IceTrayValveOut(U8 mu8Status)
{
    if (mu8Status)
    {
        bit_tray_valve_output = SET;
    }
    else
    {
        bit_tray_valve_output = CLEAR;
    }
}


/// @brief      Ice Tray ��� �Լ� ���� ���� Ȯ�� �Լ�
/// @details    Ice Tray ��� �Լ� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ice Tray �Լ� ��� ���� ���� - 0(�̵���), 1(����)
U8 Get_IceTrayValveOut(void)
{
    if (bit_tray_valve_output)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Ice Tray �Ϲ� ���� �Լ�
/// @details    Ice Tray �Ϲ� ������ �����Ѵ�
/// @param      void
/// @return     void
void Hal_IceTrayControl(void)
{
    if (gu8IceTrayControlTimer >= ICE_TRAY_CONTROL_TIME_PERIOD)
    {
        gu8IceTrayControlTimer = 0;

        check_error_blocked_trayin();
    }
}


/// @brief      Ice Tray �Լ� ���� ���� üũ ���� �Լ�
/// @details    Ice Tray �Լ� ���� ���� üũ ������ �Ѵ�
/// @param      void
/// @return     void
void check_error_blocked_trayin(void)
{
    U8 mu8IceStepBuf = 0;

    mu8IceStepBuf = Get_IceStep();

    if (mu8IceStepBuf == STATE_20_WATER_IN_ICE_TRAY)
    {
        if (bit_tray_valve_output == SET)
        {   // Ʈ���� �Լ� ��� ���� ���� ��
//            if( gu8_Flow_1sec <= HOT_BLOCKED_FLOW_HZ )
//            {
//                u16_tray_flow_check_timer++;
//
//                if(u16_tray_flow_check_timer >= BLOCKED_TRAY_IN_CHECK_TIME)
//                {
//                    u16_tray_flow_check_timer = 0;
//                    bit_tray_in_error_temporary = SET;
//                }
//                else{}
//            }
//            else
//            {
//                u16_tray_flow_check_timer = 0;
//            }
//        }
//        else
//        {
//            if( F_WaterOut == SET )
//            {
//                /*..hui [20-6-22���� 10:21:02] �����Ҷ� ī��Ʈ �ʱ�ȭ�ϵ��� ����.. �ݺ����� �����϶� ���������� ����� ���̷����������..*/
//                /*..hui [20-6-22���� 10:21:06] �̽������..*/
//                u16_tray_flow_check_timer = 0;
//            }
//            else{}
        }
    }
    else
    {   // Ʈ���� �Լ� ��� ���� ���� �ƴ� ��
//        u16_tray_flow_check_timer = 0;
    }


    if (bit_tray_in_error_temporary == SET)
    {   // Ʈ���� �Լ� �ܼ� �߻���
        if (mu8IceStepBuf == STATE_0_STANDBY)
        {
            gu16_tray_error_clear_timer++;

            if (gu16_tray_error_clear_timer >= BLOCKED_TRAY_CLEAR_TIME)
            {
                gu16_tray_error_clear_timer = 0;
                bit_tray_in_error_temporary = CLEAR;
            }
            else
            {

            }
        }
        else
        {
            gu16_tray_error_clear_timer = 0;
        }
    }
    else
    {
        gu16_tray_error_clear_timer = 0;
    }
}


/// @brief      Ice Tray �Լ� �ܼ� ���� Ȯ�� �Լ�
/// @details    Ice Tray �Լ� �ܼ� ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ice Tray �Լ� �ܼ� ���� - 0(�ܼ� �̹߻�), 1(�ܼ� �߻�)
U8 Get_IceTrayInBlocked(void)
{
    if (bit_tray_in_error_temporary)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


#endif


// Hal Ice Tray Module **************************************************************************************

/// @brief      Hal Ice Tray Initilaize Module
/// @details    Ice Tray ���� ���� ���� ��� �ʱ�ȭ
/// @param      void
/// @return     void
void Hal_IceTray_Module_Initialize(void)
{
#if (MOTOR_ICE_TRAY_USE == USE)
    Hal_IceTray_Initialize();
#endif
}


/// @brief      Hal Ice Tray Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� Ice Tray ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_IceTray_Module_1ms_Control(void)
{
#if (MOTOR_ICE_TRAY_USE == USE)
    IceTrayControlTimer();
    IceTray_StepMotorInterrupt();
#endif
}


/// @brief      Hal Ice Tray Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� Ice Tray ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_IceTray_Module_Control(void)
{
#if (MOTOR_ICE_TRAY_USE == USE)
    Hal_IceTrayControl();
#endif
}






