/// @file     Hal_AC_Motor_IceMaking.c
/// @date     2025/07/30
/// @author   Jaejin Ham
/// @brief    ���� ��� ���� AC Motor Control file


#include "Global_Header.h"

#if (AC_MOTOR_ICE_MAKING_USE == USE)

U8 gu8AC_Motor_IceMakingControlTimer = 0;           /// @brief  ���� ���� AC ���� ���� �ֱ� Ÿ�̸�

U8 gu8Tray_CW_Interval_Timer = 0;                   /// @brief  Tray CW ȸ�� ���� �ð�
U8 F_Tray_CW_delay_finish = 0;                      /// @brief  Tray CW ȸ�� ���� ���� �Ϸ� Flag
U8 gu8Tray_CCW_Interval_Timer = 0;                  /// @brief  Tray CCW ȸ�� ���� �ð�
U8 F_Tray_CCW_delay_finish = 0;                     /// @brief  Tray CCW ȸ�� ���� ���� �Ϸ� Flag

U16 gu16Ice_Tray_Up_Reset_Delay_Tmr = 0;            /// @brief  Tray Up ���� �ʱ�ȭ ���� ���� �ð�
U16 gu16Ice_Tray_Down_Reset_Delay_Tmr = 0;          /// @brief  Tray Down ���� �ʱ�ȭ ���� ���� �ð�
U8 F_Ice_Tray_Up_Move_Reset = 0;                    /// @brief  Ʈ���� Up ���� �ʱ�ȭ ���� - 0(�̵���), 1(����)
U8 F_Ice_Tray_Down_Move_Reset = 0;                  /// @brief  Ʈ���� Down ���� �ʱ�ȭ ���� - 0(�̵���), 1(����)

U8 F_TrayMotorUP = 0;                               /// @brief  Ʈ���� ���� Up �̵� ���� Flag - 0(���̵�), 1(�̵�)
U8 F_TrayMotorPreUP = 0;                            /// @brief  Ʈ���� ���� ���� Up �̵� ���� Flag - 0(���̵�), 1(�̵�)
U8 F_TrayMotorDOWN = 0;                             /// @brief  Ʈ���� ���� Down �̵� ���� Flag - 0(���̵�), 1(�̵�)
U8 F_TrayMotorPreDOWN = 0;                          /// @brief  Ʈ���� ���� ���� Down �̵� ���� Flag - 0(���̵�), 1(�̵�)

U8 F_Over_ice_check_enable = 0;                     /// @brief  ������ üũ ���� Flag - 0(üũ����), 1(üũ����)
U16 gu16Over_ice_melt_timer = 0;                    /// @brief  ������ ���̴� ���� �ð�
U8 gu8Over_ice_melt_proc = 0;                       /// @brief  ������ ���̴� ���� ���� �ܰ�
U8 gu8Over_ice_melt_operation_count = 0;            /// @brief  ������ ���̴� ���� ���� �ݺ� Ƚ��

U8 F_Over_Ice_Heater_State = 0;                     /// @brief  Ż������ ������ ���� Flag - <�̻��>

U8 F_ice_out_back_state = 0;                        /// @brief  ���� ���� �� �Ǵ� ��ũ�� ������ ���� Flag - 0(�̵���), 1(����)
U8 gu8ice_out_reverse_timer = 0;                    /// @brief  ���� ���� �� �Ǵ� ��ũ�� ������ ���� �ð�

U8 gu8Ice_out_operation_timer = 0;                  /// @brief  ���� ���� ���� ���� �ð�

U8 F_ice_stuck_back_state = 0;                      /// @brief  ���� �ɸ� ���� ���� ���� ���� - 0(�̵���), 1(����)
U8 gu8Ice_stuck_reverse_timer = 0;                  /// @brief  ���� �ɸ� ���� ��ȸ�� ���� �ð�

U8 gu8Feeder_ice_release_timer = 0;                 /// @brief  ���� feeder ���� ���� �ð�

U8 gu8IceOutCCWInterval = 0;                        /// @breif  ���� ���� ��� �ð�

U16 gu16ErrIceOut = 0;                              /// @brief  ���� ���� ���� üũ �ð�


/// @brief      ���� ���� AC ���͵��� ���� �ʱ�ȭ �Լ�
/// @details    ���� ���� AC ���͵��� ����� ���õ� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_AC_Motor_IceMaking_Initialize(void)
{
    gu8AC_Motor_IceMakingControlTimer = 0;

    gu8Tray_CW_Interval_Timer = 0;
    F_Tray_CW_delay_finish = 0;
    gu8Tray_CCW_Interval_Timer = 0;
    F_Tray_CCW_delay_finish = 0;

    gu16Ice_Tray_Up_Reset_Delay_Tmr = 0;
    gu16Ice_Tray_Down_Reset_Delay_Tmr = 0;
    F_Ice_Tray_Up_Move_Reset = 0;
    F_Ice_Tray_Down_Move_Reset = 0;

    F_TrayMotorUP = 0;
    F_TrayMotorPreUP = 0;
    F_TrayMotorDOWN = 0;
    F_TrayMotorPreDOWN = 0;

    F_Over_ice_check_enable = 0;
    gu16Over_ice_melt_timer = 0;
    gu8Over_ice_melt_proc = 0;
    gu8Over_ice_melt_operation_count = 0;

    F_Over_Ice_Heater_State = 0;

    F_ice_out_back_state = 0;
    gu8ice_out_reverse_timer = 0;

    gu8Ice_out_operation_timer = 0;

    F_ice_stuck_back_state = 0;
    gu8Ice_stuck_reverse_timer = 0;

    gu8Feeder_ice_release_timer = 0;

    gu8IceOutCCWInterval = 0;
}


/// @brief      ���� ���� AC ���͵��� Control Timer(@1ms interrupt)
/// @details    ���� ���� AC ���͵��� ��� ���� Ÿ�̹��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void AC_Motor_IceMaking_ControlTimer(void)
{
    if (gu8AC_Motor_IceMakingControlTimer < AC_MOTOR_ICE_MAKING_CONTROL_TIME_PERIOD)
    {
        gu8AC_Motor_IceMakingControlTimer++;
    }
}


/// @brief      ���� ���� AC Motor ���� �Լ�(@100ms)
/// @details    ���� ���� AC ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Output_AC_Motor_IceMaking(void)
{
    if (gu8AC_Motor_IceMakingControlTimer >= AC_MOTOR_ICE_MAKING_CONTROL_TIME_PERIOD)
    {   // 100ms���� ����
        gu8AC_Motor_IceMakingControlTimer = 0;

        Output_Ice_Feeder_Motor();      // Feeder ���� ����
        Output_Ice_Tray_Motor();        // Tray ���� ����
    }
}


/// @brief      ���� ���� ���� ��� �ð� ���� �Լ�(���� ����Ű �ԷµǾ� ���� ���� ���۽� �� ����)
/// @details    ���� ���� ���� ��� �ð��� ���� ���� ��ư �ԷµǾ� ���� ���� ���۽� ���� �����Ѵ�
/// @param      mu8Data : ���� ���� ���۽� ���� ���� ���� ��� �ð� ������(100ms����)
/// @return     void
void Set_IceOutIntervalTime(U8 mu8Data)
{
    gu8IceOutCCWInterval = mu8Data;
}


/// @brief      ���� ���� ���� ��� �ð� Ȯ�� �Լ�
/// @details    ���� ���� ��ư �ԷµǾ� ���� ���� ���۽� ���� ������ ���� ���� ���� ��� �ð��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���۽� ���� ���� ���� ��� �ð� ������(100ms����)
U8 Get_IceOutIntervalTime(void)
{
    return  gu8IceOutCCWInterval;
}


/// @brief      ���� ���� �� �Ǵ� ��ũ�� ������ ���� ���� �Լ�
/// @details    ���� ���� �� �Ǵ� ��ũ�� �������� ������ ������ �����Ѵ�
/// @param      mu8Status : ���� ���� �� �Ǵ� ��ũ�� ������ ���� Flag - 0(�̵���), 1(����)
/// @return     void
void Set_IceOutBackStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_ice_out_back_state = SET;
    }
    else
    {
        F_ice_out_back_state = CLEAR;
    }
}


/// @brief      ���� ���� �� �Ǵ� ��ũ�� ������ ���� Ȯ�� �Լ�
/// @details    ���� ���� �� �Ǵ� ��ũ�� �������� �����ϴ��� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� �� �Ǵ� ��ũ�� ������ ���� Flag - 0(�̵���), 1(����)
U8 Get_IceOutBackStatus(void)
{
    if (F_ice_out_back_state)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� �ɸ� ���� ���� ���� ���� ���� �Լ�
/// @details    ���� �ɸ� ���� ���� ���� ���θ� �����Ѵ�
/// @param      mu8Status : ���� �ɸ� ���� ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_IceStuckBackStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_ice_stuck_back_state = SET;
    }
    else
    {
        F_ice_stuck_back_state = CLEAR;
    }
}


/// @brief      ���� �ɸ� ���� ���� ���� ���� Ȯ�� �Լ�
/// @details    ���� �ɸ� ���� ���� ���� ���θ� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �ɸ� ���� ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_IceStuckBackStatus(void)
{
    if (F_ice_stuck_back_state)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ���� Feeder Motor ���� �Լ�
/// @details    ���� ���� Feeder ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Output_Ice_Feeder_Motor(void)
{
    U8 mu8IceOutCCWStatusBuf = 0;
    U8 mu8IceOutContinueBuf = 0;
    U8 mu8IceOutCCW_PortStatus = 0;
    U8 mu8IceOutBackStatusBuf = 0;
    U8 mu8IceStuckBackStatusBuf = 0;

    if (gu8IceOutCCWInterval > 0)
    {
        gu8IceOutCCWInterval--;    // ������� ���3->2��
    }

    if (gu16ErrIceOut > 0)
    {
        gu16ErrIceOut--;                // �������� ���Ϳ��� 120��
    }

/*..hui [20-4-17���� 3:04:49] �Ǵ� ��ȸ�� ������ ice_extraction_finish�� F_IR ���������..��~~~..*/
/*..hui [20-4-17���� 3:04:59] ���� ������ 2�� ��ȸ�� �Ϸ� �� �ϵ��� �ϱ� ����..*/
    //===========================================// �������� ���� ����
    mu8IceOutCCWStatusBuf = Get_IceOutCCWStatus();
    mu8IceOutContinueBuf = Get_IceOutContinueStatus();

    if (mu8IceOutCCWStatusBuf == TRUE)                      // ����ȸ��(��ȸ��)
    {
        /*..hui [25-3-6���� 5:35:50] �̵�..*/
        if (gu8IceOutCCWInterval > 0)
        {
            /*..hui [25-3-6���� 5:38:06] �̰͵� �̵�..*/
            gu16ErrIceOut = 1200;              //������� 120��
            gu8Feeder_ice_release_timer = 0;
        }
        else{}

        /*..hui [25-3-6���� 5:36:54] ������ �ƴҶ��� ���� ����..*/
        /*..hui [25-3-9���� 7:18:57] ������ �ƴҶ��� 3����Ŭ ������ ���ӵ���..*/
        if (mu8IceOutContinueBuf == FALSE)
        {
            if (gu8IceOutCCWInterval == 0)
            {
                gu8Feeder_ice_release_timer++;

                if (gu8Feeder_ice_release_timer <= 20)
                {
                    TURN_ON_MOTOR_ICE_OUT_CCW();
                }
                else if (gu8Feeder_ice_release_timer <= 25)
                {
                    /*P_MOTOR_ICE_OUT_CCW = CLEAR;*/
                    TURN_ON_MOTOR_ICE_OUT_CCW();
                }
                else if (gu8Feeder_ice_release_timer <= 45)
                {
                    TURN_ON_MOTOR_ICE_OUT_CCW();
                }
                else if (gu8Feeder_ice_release_timer <= 50)
                {
                    /*P_MOTOR_ICE_OUT_CCW = CLEAR;*/
                    TURN_ON_MOTOR_ICE_OUT_CCW();
                }
                else if (gu8Feeder_ice_release_timer <= 70)
                {
                    TURN_ON_MOTOR_ICE_OUT_CCW();
                }
                else if (gu8Feeder_ice_release_timer <= 75)
                {
                    /*P_MOTOR_ICE_OUT_CCW = CLEAR;*/
                    TURN_ON_MOTOR_ICE_OUT_CCW();
                }
                else
                {
                    gu8Feeder_ice_release_timer = 76;
                    TURN_ON_MOTOR_ICE_OUT_CCW();
                }
            }
            else
            {

            }
        }
        else
        {
            if (gu8IceOutCCWInterval == 0)
            {
                TURN_ON_MOTOR_ICE_OUT_CCW();
            }
            else
            {

            }

        }

        /*..hui [20-4-14���� 12:45:16] ���� 2�� �̻� ���ӽ� ���� ����..*/
        mu8IceOutCCW_PortStatus = GET_STATUS_MOTOR_ICE_OUT_CCW();

        if (mu8IceOutCCW_PortStatus == SET)
        {
            if (gu16ErrIceOut == 0)
            {
                TURN_OFF_MOTOR_ICE_OUT_CCW();
                Set_IceOutCCWStatus(CLEAR);
			    /*..sean [25-06-30] �ƿ� ������ �����Ű���� ����..*/
                Set_IceOutStatus(CLEAR);

			    // ������ ��� �߰� �ʿ�
            }
        }
        else
        {

        }

        if (mu8IceOutCCW_PortStatus == SET)
        {
            gu8Ice_out_operation_timer++;

            //if( gu8Ice_out_operation_timer >= 60 )
            /*..sean [25-07-07] 3�� �����, 3�� ���� �� ��ȸ������ ����..*/
            if (gu8Ice_out_operation_timer >= 30)
            {
                //gu8Ice_out_operation_timer = 60;
                gu8Ice_out_operation_timer = 30;
                F_ice_out_back_state = SET;
            }
            else{}
        }
        else
        {
        }

        /*..hui [20-4-14���� 1:09:53] ����� ��ȸ�� ����..*/
        TURN_OFF_MOTOR_ICE_OUT_CW();
        gu8ice_out_reverse_timer = 0;
        gu8Ice_stuck_reverse_timer = 0;
    }
    else
    {
        /*P_MOTOR_ICE_OUT_CCW = CLEAR;*/
        gu8Ice_out_operation_timer = 0;
        gu8Feeder_ice_release_timer = 0;

        mu8IceOutBackStatusBuf = Get_IceOutBackStatus();
        mu8IceStuckBackStatusBuf = Get_IceStuckBackStatus();

        /*..hui [20-4-14���� 12:38:42] �Ǵ���ũ���� ���� �������� ������ ������..*/
        if (mu8IceOutBackStatusBuf == TRUE)
        {
            TURN_OFF_MOTOR_ICE_OUT_CCW();

            gu8ice_out_reverse_timer++;

            /*..hui [20-4-14���� 12:38:59] 3�ʵ��� ��/�� �Ѵ� OFF �� ���..*/
            /*..hui [20-4-14���� 12:39:18] 3�� �� ��ȸ�� ���� ���� ����..*/
            /*..hui [20-4-22���� 10:29:26] �������¿��� 6���̻� ���������� 6���� ..*/
            /*..sean [25-07-07] �������¿��� 3���̻� ���������� 3���� ..*/
            if (gu8ice_out_reverse_timer >= 40)
            {
                TURN_ON_MOTOR_ICE_OUT_CW();
            }
            else
            {

            }

            /*..hui [20-4-14���� 12:40:01] 2�� ���� ���� �� ��ȸ�� ����..*/
            if (gu8ice_out_reverse_timer >= 55)
            {
                TURN_OFF_MOTOR_ICE_OUT_CW();
                Set_IceOutBackStatus(CLEAR);
                gu8ice_out_reverse_timer = 0;
                /*..hui [20-4-22���� 10:35:40] ��ȸ�� �Ϸ� �� ��������..*/
                Set_IceFullCheck(SET);
            }
            else
            {

            }

            gu8Ice_stuck_reverse_timer = 0;
        }
        else if (mu8IceStuckBackStatusBuf == TRUE)
        {
            TURN_OFF_MOTOR_ICE_OUT_CCW();

            gu8Ice_stuck_reverse_timer++;

            /*..hui [25-3-17���� 4:16:14] ���� �ɸ� ���� ���� 2��..*/
            if (gu8Ice_stuck_reverse_timer >= 10)
            {
                gu8Ice_stuck_reverse_timer = 0;
                Set_IceStuckBackStatus(CLEAR);
            }
            else
            {
                TURN_ON_MOTOR_ICE_OUT_CW();
            }
        }
        else
        {
            TURN_OFF_MOTOR_ICE_OUT_CCW();
            TURN_OFF_MOTOR_ICE_OUT_CW();
            gu8ice_out_reverse_timer = 0;
            gu8Ice_stuck_reverse_timer = 0;
        }
    }
}


/// @brief      ���� ���� Tray Motor ���� �Լ�
/// @details    ���� ���� Tray ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Output_Ice_Tray_Motor(void)
{
    U8 mu8E61_Buf = 0;
    U8 mu8E61_DetectBuf = 0;
    U8 mu8E62_Buf = 0;
    U8 mu8E63_Buf = 0;

    mu8E61_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ALL_DETECT_E61);
    mu8E61_DetectBuf = Get_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61);
    mu8E62_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62);
    mu8E63_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_DE_ICE_E63);

    /*..hui [18-2-9���� 1:09:49] CW CCW ���� ���� �� 5�� ��� �� ��ȯ..*/
    inverse_direction_time_check();

    if ( (mu8E61_Buf == TRUE) ||
         (mu8E62_Buf == TRUE) ||
         (mu8E63_Buf == TRUE) )
    {
        /*pMOTOR_ICE_TRAY_CW = CLEAR;*/
        /*pMOTOR_ICE_TRAY_CCW = CLEAR;*/
        run_stop_ice_tray();
    }
    else if (mu8E61_DetectBuf == TRUE)
    {   // E61 ���� ���� ���¸� Ȯ�� �˻�� ���� üũ Lib���� ��ü ����
        /*..hui [18-2-13���� 3:16:13] move ���� Ȯ�� �� ���ð��� ���� üũ�� ��ȯ�� ��� Ŭ����..*/
        /*..hui [18-2-13���� 3:16:29] �ٽ� move�����̵Ǹ� ó������ �����ϵ���....*/
        reset_micro_sw_move_err_check_state();
    }
    else
    {   // E62, E63 ���� üũ�� ���� üũ Lib���� ��ü ����
        Set_MicroSW_AllDetectErrorCheckStep(0);
    }

    check_ice_system_ok();

    if ( (mu8E61_Buf == FALSE) &&
         (mu8E62_Buf == FALSE) &&
         (mu8E63_Buf == FALSE) )
    {
        /*..hui [25-3-17���� 3:16:46] ���̽�Ʈ���� ���� ���..*/
        /*..hui [25-3-17���� 3:17:56] ���� �ƴҶ��� ��µǵ���..*/
        control_ice_tray();
    }
    else
    {

    }
}


/// @brief      ����ũ�� ����ġ �̵� ���� üũ ���� �ʱ�ȭ �Լ�
/// @details    ����ũ�� ����ġ �̵� ���� üũ ���� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void reset_micro_sw_move_err_check_state(void)
{
    F_Ice_Tray_Up_Move_Reset = 0;
    F_Ice_Tray_Down_Move_Reset = 0;
    Set_IceSafetyRoutine(CLEAR);
    Clear_E62_E63_CheckTimer();
}


/// @brief      ����ũ�� ����ġ �̵� ���� üũ ���� �ʱ�ȭ �Լ�
/// @details    ����ũ�� ����ġ �̵� ���� üũ ���� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void inverse_direction_time_check(void)
{
    U8 mu8IceTrayModeBuf = 0;

    mu8IceTrayModeBuf = Get_IceTrayMode();

    /*if( pMOTOR_ICE_TRAY_CW == SET )*/
    if (mu8IceTrayModeBuf == ICE_TRAY_MODE_UP)
    {
        gu8Tray_CW_Interval_Timer = 0;
        F_Tray_CW_delay_finish = CLEAR;
    }
    else
    {
        gu8Tray_CW_Interval_Timer++;

        if (gu8Tray_CW_Interval_Timer >= 50)
        {
            gu8Tray_CW_Interval_Timer = 50;
            F_Tray_CW_delay_finish = SET;
        }
        else
        {

        }
    }

    /*if(pMOTOR_ICE_TRAY_CCW == SET)*/
    if (mu8IceTrayModeBuf == ICE_TRAY_MODE_DOWN)
    {
        gu8Tray_CCW_Interval_Timer = 0;
        F_Tray_CCW_delay_finish = CLEAR;
    }
    else
    {
        gu8Tray_CCW_Interval_Timer++;

        if (gu8Tray_CCW_Interval_Timer >= 50)
        {
            gu8Tray_CCW_Interval_Timer = 50;
            F_Tray_CCW_delay_finish = SET;
        }
        else
        {

        }
    }
}


/// @brief      ���� Ʈ���� Up ���� �ʱ�ȭ �Լ�
/// @details    ���� Ʈ���� Up ���� �ʱ�ȭ ������ �Ѵ�
/// @param      void
/// @return     void
void ice_make_system_up_move_reset(void)
{
    U16 mu16_delay_time = 0;
    U8 mu8TrayUp_ErrorCountBuf = 0;
    U8 mu8IceTrayPositionBuf = 0;

    mu8TrayUp_ErrorCountBuf = Get_TrayUp_ErrorCount();
    mu8IceTrayPositionBuf = Get_IceTrayPosition();

    if (mu8TrayUp_ErrorCountBuf <= TRAY_UP_ICE_STUCK_COUNT)
    {
        /*..hui [25-3-17���� 3:59:26] ù 5ȸ�� 1������..*/
        mu16_delay_time = TRAY_UP_ICE_STUCK_DELAY_TIME;
    }
    else
    {
        mu16_delay_time = TRAY_UP_RESET_DELAY_TIME;
    }

    if (F_Ice_Tray_Up_Move_Reset == SET)
    {
        gu16Ice_Tray_Up_Reset_Delay_Tmr++;

        /*if(gu16Ice_Tray_Up_Reset_Delay_Tmr >= TRAY_UP_RESET_DELAY_TIME)*/
        if (gu16Ice_Tray_Up_Reset_Delay_Tmr >= mu16_delay_time)
        {
            if (mu8TrayUp_ErrorCountBuf <= 2)
            {
                ice_stuck_reset();
                F_Ice_Tray_Up_Move_Reset = CLEAR;
                /*..hui [25-3-17���� 4:20:05] Ʈ���� ������ �ٽ� ����..*/
                /*run_down_ice_tray();*/
                down_tray_motor();
            }
            else
            {
                ice_system_reset();
                F_Ice_Tray_Up_Move_Reset = CLEAR;
            }
        }
        else
        {
            /*..hui [18-2-13���� 3:24:13] 10�� ����߿� ���� ���ͽ� �ϴ��� ����....*/
            if (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_ICE_MAKING)
            {
                ice_system_reset();
                F_Ice_Tray_Up_Move_Reset = CLEAR;
            }
            else
            {

            }
        }
    }
    else
    {
        gu16Ice_Tray_Up_Reset_Delay_Tmr = 0;
    }
}


/// @brief      ���� Ʈ���� Down ���� �ʱ�ȭ �Լ�
/// @details    ���� Ʈ���� Down ���� �ʱ�ȭ ������ �Ѵ�
/// @param      void
/// @return     void
void ice_make_system_down_move_reset(void)
{
    over_ice_making_check();
}


/// @brief      ������ ���� üũ �Լ�
/// @details    ������ ���� üũ ������ �Ѵ�
/// @param      void
/// @return     void
void over_ice_making_check(void)
{
    U8 mu8E62_Buf = 0;
    U8 mu8E63_Buf = 0;

    mu8E62_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62);
    mu8E63_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_DE_ICE_E63);

    F_Over_ice_check_enable = F_Ice_Tray_Down_Move_Reset;

    if ( (F_Over_ice_check_enable != SET) ||
         (mu8E62_Buf == TRUE) ||
         (mu8E63_Buf == TRUE) )
    {
        gu16Ice_Tray_Down_Reset_Delay_Tmr = 0;
        gu16Over_ice_melt_timer = 0;
        gu8Over_ice_melt_proc = 0;
        gu8Over_ice_melt_operation_count = 0;
        //F_over_ice_melt_mode_cold_water_valve_out = CLEAR;
        F_Over_Ice_Heater_State = CLEAR;
    }
    else
    {
        gu16Ice_Tray_Down_Reset_Delay_Tmr++;

        if (gu16Ice_Tray_Down_Reset_Delay_Tmr >= TRAY_ERROR_STANDBY_TIME)   //10min
        {
            gu16Ice_Tray_Down_Reset_Delay_Tmr = TRAY_ERROR_STANDBY_TIME;

            over_ice_making_melt_proc();
        }
        else{}
    }
}


/// @brief      ������ ���� ���̴� ���� �Լ�
/// @details    ������ ���� ���̴� ������ �Ѵ�
/// @param      void
/// @return     void
void over_ice_making_melt_proc(void)
{
    U8 mu8IceTrayPositionBuf = 0;

    mu8IceTrayPositionBuf = Get_IceTrayPosition();

    /*..hui [18-2-6���� 9:42:36] �������Ȱ� ���̴��߿� ������ Ʈ���� ���� ���͵Ǹ� �ϴ��� ����..*/
    if (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_ICE_THROW)
    {
        ice_system_reset();
        F_Ice_Tray_Down_Move_Reset = CLEAR;
    }
    else
    {

    }

    switch (gu8Over_ice_melt_proc)
    {
        case 0:
            gu16Over_ice_melt_timer = 0;
            gu8Over_ice_melt_proc++;
            break;

        case 1:
            /*..hui [18-2-6���� 9:16:17] ���� 10�ʵ��� �ü� �޼���� ON�Ѵ�..*/
            gu16Over_ice_melt_timer++;

            /*if(gu16Over_ice_melt_timer >= 100)*/
            /*..hui [23-9-22���� 11:26:29] 15�ʷ� �ø�.. �����ܾ��̽� ���..*/
            if (gu16Over_ice_melt_timer >= 150)
            {
                Set_IceValve(CLEAR);
                gu16Over_ice_melt_timer = 0;
                gu8Over_ice_melt_proc++;
            }
            else
            {
                Set_IceValve(SET);
            }

            F_Over_Ice_Heater_State = CLEAR;
            break;

        case 2:
            /*..hui [18-2-6���� 9:21:25] �ü� �Լ��ϰ� 20�ʵ��� Ż�����͸� �����Ѵ�..*/
            gu16Over_ice_melt_timer++;

            /*..hui [20-4-21���� 7:15:57] ������ ���� 10�ʷ� ����..*/
            /*..hui [20-4-21���� 7:22:24] AIS30�� Ż���� �ʱ� OFF ���� �����ϹǷ� �ٽ� 15�ʷ�....*/
            /*if(gu16Over_ice_melt_timer >= 150)*/
            /*if(gu16Over_ice_melt_timer >= 6000)*/
            /*..hui [23-9-22���� 11:26:49] Ż������ ���� ����.. 20�е��� ���..*/
            if (gu16Over_ice_melt_timer >= 12000)
            {
                F_Over_Ice_Heater_State = CLEAR;
                gu16Over_ice_melt_timer = 0;
                gu8Over_ice_melt_proc++;
            }
            else
            {
                F_Over_Ice_Heater_State = SET;
            }
            break;


        case 3:
            gu16Over_ice_melt_timer = 0;
            gu8Over_ice_melt_proc++;

            F_Over_Ice_Heater_State = CLEAR;
            break;

        case 4:
            /*..hui [18-2-6���� 9:23:11] 5�ʵ��� �ü� �޼���� ON..*/
            gu16Over_ice_melt_timer++;

            if (gu16Over_ice_melt_timer >= 50)
            {
                Set_IceValve(CLEAR);
                gu16Over_ice_melt_timer = 0;
                gu8Over_ice_melt_proc++;
            }
            else
            {
                Set_IceValve(SET);
            }

            F_Over_Ice_Heater_State = CLEAR;
            break;

        case 5:
            /*..hui [18-2-6���� 9:23:58] 10�е��� ���..*/
            gu16Over_ice_melt_timer++;

            if (gu16Over_ice_melt_timer >= 6000)
            {
                /*..hui [18-2-6���� 9:25:47] 10�е��� ����Ŀ� �ٽ� �޼���� 5�� ON�Ѵ�..*/
                gu16Over_ice_melt_timer = 0;
                gu8Over_ice_melt_proc = 3;

                /*..hui [18-2-6���� 9:29:04] �� 9�� �뷫 90�е��� ����..*/
                gu8Over_ice_melt_operation_count++;

                /*..hui [18-2-6���� 9:30:30] 90�� ���� ���� �� CCW ���� �ѹ� �� ����..*/
                if (gu8Over_ice_melt_operation_count >= 10)
                {
                    ice_system_reset();
                    F_Ice_Tray_Down_Move_Reset = CLEAR;

                    if (mu8IceTrayPositionBuf != ICE_TRAY_POSITION_ICE_THROW)
                    {
                        F_TrayMotorUP = CLEAR;
                        F_TrayMotorDOWN = SET;
                    }
                    else
                    {

                    }
                }
                else
                {

                }
            }
            else
            {

            }

            F_Over_Ice_Heater_State = CLEAR;

            break;

        default:
            gu8Over_ice_melt_proc = 0;
            gu8Over_ice_melt_operation_count = 0;
            gu16Over_ice_melt_timer = 0;
            break;

    }
}


/// @brief      Tray Down ���� ���� �Լ�
/// @details    Tray Down ���� ������ �Ѵ�
/// @param      void
/// @return     void
void down_tray_motor(void)
{
    F_TrayMotorUP = CLEAR;
    F_TrayMotorPreUP = CLEAR;
    F_TrayMotorDOWN = SET;
    F_TrayMotorPreDOWN = SET;
}


/// @brief      Tray Up ���� ���� �Լ�
/// @details    Tray Up ���� ������ �Ѵ�
/// @param      void
/// @return     void
void up_tray_motor(void)
{
    F_TrayMotorUP = SET;
    F_TrayMotorPreUP = SET;
    F_TrayMotorDOWN = CLEAR;
    F_TrayMotorPreDOWN = CLEAR;
}


/// @brief      Tray CCW ���� ���� �Ϸ� ���� �Լ�
/// @details    Tray CCW ���� ���� �Ϸ� ���θ� �����Ѵ�
/// @param      mu8Data : Tray CCW ���� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
/// @return     void
void Set_TrayCCW_DelayEnd(U8 mu8Data)
{
    if (mu8Data)
    {
        F_Tray_CCW_delay_finish = SET;
    }
    else
    {
        F_Tray_CCW_delay_finish = CLEAR;
    }
}


/// @brief      Tray CCW ���� ���� �Ϸ� Ȯ�� �Լ�
/// @details    Tray CCW ���� ���� �Ϸ� ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Tray CCW ���� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_TrayCCW_DelayEnd(void)
{
    if (F_Tray_CCW_delay_finish)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Tray CW ���� ���� �Ϸ� ���� �Լ�
/// @details    Tray CW ���� ���� �Ϸ� ���θ� �����Ѵ�
/// @param      mu8Data : Tray CW ���� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
/// @return     void
void Set_TrayCW_DelayEnd(U8 mu8Data)
{
    if (mu8Data)
    {
        F_Tray_CW_delay_finish = SET;
    }
    else
    {
        F_Tray_CW_delay_finish = CLEAR;
    }
}


/// @brief      Tray CW ���� ���� �Ϸ� Ȯ�� �Լ�
/// @details    Tray CW ���� ���� �Ϸ� ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Tray CW ���� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_TrayCW_DelayEnd(void)
{
    if (F_Tray_CW_delay_finish)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Tray Up �ʱ�ȭ ���� ���� ���� �Լ�
/// @details    Tray Up �ʱ�ȭ ���� ���¸� �����Ѵ�
/// @param      mu8Data : Tray Up �ʱ�ȭ ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_TrayUpMoveReset(U8 mu8Data)
{
    if (mu8Data)
    {
        F_Ice_Tray_Up_Move_Reset = SET;
    }
    else
    {
        F_Ice_Tray_Up_Move_Reset = CLEAR;
    }
}


/// @brief      Tray Up �ʱ�ȭ ���� ���� Ȯ�� �Լ�
/// @details    Tray Up �ʱ�ȭ ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Tray Up �ʱ�ȭ ���� ���� - 0(�̵���), 1(����)
U8 Get_TrayUpMoveReset(void)
{
    if (F_Ice_Tray_Up_Move_Reset)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Tray Down �ʱ�ȭ ���� ���� ���� �Լ�
/// @details    Tray Down �ʱ�ȭ ���� ���¸� �����Ѵ�
/// @param      mu8Data : Tray Down �ʱ�ȭ ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_TrayDownMoveReset(U8 mu8Data)
{
    if (mu8Data)
    {
        F_Ice_Tray_Down_Move_Reset = SET;
    }
    else
    {
        F_Ice_Tray_Down_Move_Reset = CLEAR;
    }
}


/// @brief      Tray Down �ʱ�ȭ ���� ���� Ȯ�� �Լ�
/// @details    Tray Down �ʱ�ȭ ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Tray Down �ʱ�ȭ ���� ���� - 0(�̵���), 1(����)
U8 Get_TrayDownMoveReset(void)
{
    if (F_Ice_Tray_Down_Move_Reset)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Tray Up ���� ���� ���� �Լ�
/// @details    Tray Up ���� ���¸� �����Ѵ�
/// @param      mu8Data : Tray Up ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_TrayMotorUpStatus(U8 mu8Data)
{
    if (mu8Data)
    {
        F_TrayMotorUP = SET;
    }
    else
    {
        F_TrayMotorUP = CLEAR;
    }
}


/// @brief      Tray Up ���� ���� Ȯ�� �Լ�
/// @details    Tray Up ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Tray Up ���� ���� - 0(�̵���), 1(����)
U8 Get_TrayMotorUpStatus(void)
{
    if (F_TrayMotorUP)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Tray Down ���� ���� ���� �Լ�
/// @details    Tray Down ���� ���¸� �����Ѵ�
/// @param      mu8Data : Tray Down ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_TrayMotorDownStatus(U8 mu8Data)
{
    if (mu8Data)
    {
        F_TrayMotorDOWN = SET;
    }
    else
    {
        F_TrayMotorDOWN = CLEAR;
    }
}


/// @brief      Tray Down ���� ���� Ȯ�� �Լ�
/// @details    Tray Down ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Tray Down ���� ���� - 0(�̵���), 1(����)
U8 Get_TrayMotorDownStatus(void)
{
    if (F_TrayMotorDOWN)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Tray Up ���� ���� ���� ���� �Լ�
/// @details    Tray Up ���� ���� ���¸� �����Ѵ�
/// @param      mu8Data : Tray Up ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_TrayMotorPreUpStatus(U8 mu8Data)
{
    if (mu8Data)
    {
        F_TrayMotorPreUP = SET;
    }
    else
    {
        F_TrayMotorPreUP = CLEAR;
    }
}


/// @brief      Tray Up ���� ���� ���� Ȯ�� �Լ�
/// @details    Tray Up ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Tray Up ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_TrayMotorPreUpStatus(void)
{
    if (F_TrayMotorPreUP)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Tray Down ���� ���� ���� ���� �Լ�
/// @details    Tray Down ���� ���� ���¸� �����Ѵ�
/// @param      mu8Data : Tray Down ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_TrayMotorPreDownStatus(U8 mu8Data)
{
    if (mu8Data)
    {
        F_TrayMotorPreDOWN = SET;
    }
    else
    {
        F_TrayMotorPreDOWN = CLEAR;
    }
}


/// @brief      Tray Down ���� ���� ���� Ȯ�� �Լ�
/// @details    Tray Down ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Tray Down ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_TrayMotorPreDownStatus(void)
{
    if (F_TrayMotorPreDOWN)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}

#endif

// Hal AC Motor Ice Making Module **************************************************************************************

/// @brief      Hal AC Motor Ice Making Initilaize Module
/// @details    ���� ���� AC Motor ���� ���� ���� ��� �ʱ�ȭ
/// @param      void
/// @return     void
void Hal_AC_Motor_IceMaking_Module_Initialize(void)
{
#if (AC_MOTOR_ICE_MAKING_USE == USE)
    Hal_AC_Motor_IceMaking_Initialize();
#endif
}


/// @brief      Hal AC Motor Ice Making Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ���� ���� AC Motor ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_AC_Motor_IceMaking_Module_1ms_Control(void)
{
#if (AC_MOTOR_ICE_MAKING_USE == USE)
    AC_Motor_IceMaking_ControlTimer();                 // Swing Bar  ���� �ð� Counter
#endif
}


/// @brief      Hal AC Motor Ice Making in While Loop
/// @details    Basic Loop �� While �� �ȿ��� ���� ���� AC Motor ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_AC_Motor_IceMaking_Module_Control(void)
{
#if (AC_MOTOR_ICE_MAKING_USE == USE)
    Output_AC_Motor_IceMaking();
#endif
}
