/// @file     Lib_Water_IceMaking.c
/// @date     2025/08/04
/// @author   Jaejin Ham
/// @brief    ���� ���� ���� file


#include "Global_Header.h"
#include "Lib_Water_IceMakeTimeTable.h"


#if (LIB_ICE_MAKING_USE == USE)

U8 gu8IceMakingControlTimer = 0;                    /// @brief  ���� ���� �ֱ� Ÿ�̸�

U8 F_IceOn = 0;                                     /// @brief  ���� ��� ON/OFF ���� ���� - 0(OFF), 1(ON)

U8 gu8InitStep = 0;                                 /// @brief  ���� ��� �ʱ�ȭ ���� ���� Step

U8 gu8IceMakeDisableStatus = 0;                     /// @brief  ���� ���� �Ұ� ���� ���� - 0(���� ����), 1(���� �Ұ���)

U8 F_IceInit = 0;                                   /// @brief  ���� ���� �ʱ�ȭ ���� Flag - 0(�̵���), 1(����)

U8 F_re_ice_init_error = 0;                         /// @brief  �����߿� ���� �ɷ������� ���������� ����Ż�� ���� Flag - 0(�̵���), 1(����)

U8 F_IceFull = 0;                                   /// @brief  ���� ���� Flag - 0(���� �ƴ�), 1(����)
U8 F_IR = 0;                                        /// @brief  ���� �˻� ���� Flag - 0(�̰˻�), 1(�˻�)
U8 gu8IRTime = 0;                                   /// @brief  ���� �˻� �ð�(100ms ����)
U8 gu8IRCount = 0;                                  /// @brief  ���� üũ Ƚ��
U16 gu16IRInterval = 0;                             /// @brief  ���� �ν� �ֱ�(100ms ����)
U16 gu16ADIceFull = 0;                              /// @brief  ���� ���� ���� AD��

U8 gu8Ice_out_continue = 0;                         /// @brief  ���� ���� ���� ���� - 0(�̵���), 1(����)

U8 F_IceOut = 0;                                    /// @brief  ���� ���� ���� Flag - 0(������), 1(����)

U8 F_Safety_Routine = 0;                            /// @brief  ���� �ý��� ���� ���� ���� ���� Flag - 0(�̵���), 1(����)

U8 gu8IceStep = 0;                                /// @brief  ���� ���� ���� Step
//ICE_STEP gu8IceStep;

U8 F_IceVV = 0;                                     /// @brief  �ü� �޼� ��� ���� Flag - 0(�̵���), 1(����)

U16 gu16IceHeaterTime = 0;                          /// @brief  Ż�� �ְ���(Heater) ���� �ð�
U8 F_WaterInit = 0;                                 /// @brief  ���� ���� �Ϸ� Flag - 0(�̿Ϸ�), 1(�Ϸ�)

U8 F_IceOutCCW = 0;                                 /// @brief  ���� ���� ȸ�� ���� Flag - 0(�̵���), 1(����)

U16 gu16IceMakeTime = 0;                            /// @brief  ���� �ð�

U8 gu8_ice_tray_reovery_time = 0;                   /// @brief  Ice Tray ���� ���� �ð�

U16 gu16_Ice_Tray_Fill_Hz = 0;                      /// @brief  Ice Tray ��ä�� ���� ���� Hz(����)

U16 gu16_cody_ice_make_time = 0;                    /// @brief  �ڵ� ��� ���� ���� �ð�

U16 gu16_preheat_time = 0;                          /// @brief  �ְ��� ��� �̵� �ð�

U32 gu32_ice_init_cancel_timer = 0;                 /// @brief  ���� �ʱ�ȭ ���� ��� �ð�(100ms ����)

U8 bit_start_preheat = 0;                           /// @brief  ���� ���� ���� - 0(�̵���), 1(����)

U8 gu8Ice_system_ok = 0;                            /// @brief  ���� �ý��� �̻� ���� - 0(�̻�), 1(����)


/// @brief      ���� ���� ���� Flag & ���� �ʱ�ȭ
/// @details    ���� ��� ���Ǵ� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_IceMaking_Initialize(void)
{
    gu8IceMakingControlTimer = 0;

    F_IceOn = 0;

    gu8InitStep = 0;

    gu8IceMakeDisableStatus = 0;

    F_IceInit = 0;

    F_re_ice_init_error = 0;

    F_IceFull = 0;
    F_IR = 0;
    gu8IRTime = 0;
    gu8IRCount = 0;
    gu16IRInterval = 0;
    gu16ADIceFull = 0;

    gu8Ice_out_continue = 0;

    F_IceOut = 0;

    F_Safety_Routine = 0;

    gu8IceStep = STATE_0_STANDBY;

    F_IceVV = 0;

    gu16IceHeaterTime = 0;
    F_WaterInit = 0;

    F_IceOutCCW = 0;

    gu16IceMakeTime = 0;

    gu8_ice_tray_reovery_time = 0;

    gu16_Ice_Tray_Fill_Hz = 0;

    gu16_cody_ice_make_time = 0;

    gu16_preheat_time = 0;

    gu32_ice_init_cancel_timer = 0;

    bit_start_preheat = 0;

    gu8Ice_system_ok = 0;
}


/// @brief      ���� ���� Control Timer(@1ms)
/// @details    ���� ���� ��� ���� ���� �ֱ� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void IceMakingControlTimer(void)
{
    if (gu8IceMakingControlTimer < HEAT_TANK_CONTROL_TIME_PERIOD)
    {
        gu8IceMakingControlTimer++;
    }
}


/// @brief      ���� ��� ���� ���� ���� �Լ�
/// @details    ���� ��� ���� ���¸� �����Ѵ�
/// @param      mu8Status : ���� ��� ON/OFF ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_IceOnOff(U16 mu8Status)
{
    if (mu8Status)
    {
        F_IceOn = SET;
    }
    else
    {
        F_IceOn = CLEAR;
    }
}


/// @brief      ���� ��� ���� ���� Ȯ�� �Լ�
/// @details    ���� ��� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ��� ON/OFF ���� ���� - 0(OFF), 1(ON)
U8 Get_IceOnOff(void)
{
    if (F_IceOn)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}

/// @brief      ���� ���� ���� ���� �Լ�
/// @details    ���� ���� ���� ���� ���¸� �����Ѵ�
/// @param      mu8Status : ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_IcePreheat(U16 mu8Status)
{
    if (mu8Status)
    {
        bit_start_preheat = SET;
    }
    else
    {
        bit_start_preheat = CLEAR;
    }
}


/// @brief      ���� ���� ���� ���� Ȯ�� �Լ�
/// @details    ���� ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_IcePreheat(void)
{
    if (bit_start_preheat)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ������ ������ AD�� ���� �Լ�
/// @details    ���� ������ ������ AD���� �����Ѵ�
/// @param      mu16AD_Data : ���� ���� ������ AD��
/// @return     void
void Set_AD_IceFull(U16 mu16AD_Data)
{
    gu16ADIceFull = mu16AD_Data;
}


/// @brief      ���� ������ ������ AD�� Ȯ�� �Լ�
/// @details    ���� ������ ������ AD���� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ������ AD��
U16 Get_AD_IceFull(void)
{
    return  gu16ADIceFull;
}


/// @brief      ���� üũ �Լ�
/// @details    ���� üũ ������ �Ѵ�
/// @param      void
/// @return     void
void check_ice_full(void)
{
    U8 mu8IR_PowerBuf = 0;

    if (gu8IRTime > 0)
    {
        gu8IRTime--;                             // �����˻� 10��
    }
    else
    {

    }

    //==================================================================
    if (F_IR == SET)
    {
        mu8IR_PowerBuf = GET_STATUS_IR_POWER();

        if (mu8IR_PowerBuf == OFF)
        {   // IR ��� �������
            TURN_ON_IR_POWER();

            gu8IRTime = 100;
        }
        else
        {

        }

        if (gu16ADIceFull >= ICEFULL_LEV)
        {
            gu8IRCount++;
        }
        else
        {

        }

        if (gu8IRTime == 0)
        {
            if(gu8IRCount >= 50)
            {
                F_IceFull = CLEAR;
            }
            else
            {
                F_IceFull = SET;
            }

            gu8IRCount = 0;
            TURN_OFF_IR_POWER();
            F_IR = CLEAR;

            gu16IRInterval = IR_INTERVAL;
        }
    }
}


/// @brief      ���� �ʱ�ȭ ���� �Լ�
/// @details    ���� �ʱ�ȭ ������ �Ѵ�
/// @param      void
/// @return     void
void ice_init_operation(void)
{
    U8 mu8_return = 0;

    U8 mu8IceTrayPositionBuf = 0;
    U8 mu8TrayMotorUpBuf = 0;
    U8 mu8TrayMotorDownBuf = 0;
    U16 mu16CompOnDelayBuf = 0;
    U8 mu8GasSwitchStatusBuf = 0;

    mu8IceTrayPositionBuf = Get_IceTrayPosition();
    mu8TrayMotorUpBuf = Get_TrayMotorUpStatus();
    mu8TrayMotorDownBuf = Get_TrayMotorDownStatus();
    mu16CompOnDelayBuf = Get_CompOnDelayTime();
    mu8GasSwitchStatusBuf = Get_GasSwitchStatus();

    switch (gu8InitStep)
    {
        case 0:
            if (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_ICE_THROW)
            {   // Ż����ġ �̵�
                gu8InitStep = 1;
            }
            else
            {

            }

            if ( (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_ICE_MAKING) ||
                 (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_MOVING) )
            {   // �̵� ��/���� ��ġ -> Ż����ġ
                down_tray_motor();
            }
            else
            {   // ���� -> 3�� ���� ����

            }
            break;

        case 1:
            if ( (mu8TrayMotorUpBuf == CLEAR) &&
                 (mu8TrayMotorDownBuf == CLEAR) &&
                 (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_ICE_THROW) )
            {
                gu8InitStep = 2;
            }
            else
            {

            }
            break;

        case 2:
            gu8InitStep = 3;

            break;

        case 3:
            if (mu16CompOnDelayBuf == 0)
            {   // Comp �⵿ �� OFF Delay �ð��� ��� ����� ���
                /*..hui [19-7-25���� 1:36:36] �ø� ��ȯ��� ���� ���� �̵�..*/
                GasSwitchIce();
                gu8InitStep = 4;
            }
            else
            {

            }
            break;

        case 4:
            if (mu8GasSwitchStatusBuf == GAS_SWITCH_ICE)
            {
                gu16IceMakeTime = DUMMY_ICE_MELT_TIME;
                gu8InitStep = 5;

                Set_BLDC_Comp_Hz(BLDC_65Hz);       // ** ��ǰ�� ������ Comp ������ �ڵ��� ���� �� **
            }
            else
            {

            }
            break;

        case 5:
            if (gu16IceMakeTime > 0)
            {
                gu16IceMakeTime--;
            }
            else
            {

            }

            if (gu16IceMakeTime == 0)
            {
                gu8InitStep = 6;
            }
            else
            {

            }
            break;

        case 6:
            Set_TrayMotorDownStatus(SET);
            Set_BLDC_Comp_Hz(BLDC_65Hz);        // ** ��ǰ�� ������ Comp ������ �ڵ��� ���� �� **

            gu16IceHeaterTime = get_hotgas_time();

            GasSwitchHotGas();

            gu8InitStep = 7;
            break;

        case 7:
            mu8_return = hot_gas_operation();

            if (mu8_return == SET)
            {
                gu8InitStep = 8;
            }
            else
            {

            }
            break;

        case 8:
            gu8InitStep = 9;
            break;

        case 9:
            F_IceInit = 0;
            gu8InitStep = 0;
            gu8IceStep = STATE_0_STANDBY;
            break;

        default:
            gu8InitStep = 0;
            break;
    }
}


/// @brief      ���� ���� ���μ��� �Լ�
/// @details    ���� ���� ���μ������ �����Ѵ�
/// @param      void
/// @return     void
void Ice_Make_Process(void)
{
    U8 mu8ErrorAboutIceMaking = 0;
    U8 mu8E61_DetectBuf = 0;

    mu8ErrorAboutIceMaking = Get_ErrorAboutIceMaking();
    mu8E61_DetectBuf = Get_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61);

    if (gu8IceMakingControlTimer >= HEAT_TANK_CONTROL_TIME_PERIOD)
    {   // 100ms ����
        gu8IceMakingControlTimer = 0;

        check_ice_full();       // ���� ���� üũ

        /*..hui [17-12-19���� 7:07:33] �����϶� ���� �� ���� ���� �߰�....*/
        /*..hui [17-12-19���� 7:07:54] ���߿� �ü� �Լ���� OFF��Ű�°͵� �߰��ؾ���....*/
	    /*..sean [25-06-16]..cody ��� ���Խ� �ʱ�ȭ�ϵ��� ���ܻ��� �߰�*/
        if ( (mu8ErrorAboutIceMaking == TRUE) ||
             (F_Safety_Routine == SET) ||
             (mu8E61_DetectBuf == TRUE) )
        {
            gu8InitStep = 0;
            gu8IceStep = STATE_0_STANDBY;
        }
        else
        {
            //=======================================================//�������� �ʱ�ȭ
            if (F_IceInit == SET)
            {
                ice_init_operation();
            }
            else
            {
                gu8InitStep = 0;

                //======================================================// ���� �ʱ�, �ü�, ���� �����ƴϸ� return

                //======================================================// ���������
                if(Bit1_Ice_Make_Go != SET)
                {
                    gu8IceStep = STATE_0_STANDBY;
                }
                else
                {
                    //=======================================================// ���� ������
                    ice_make_operation();
                }
            }
        }
    }
}


/// @brief      ���� ���� ���� �Լ�
/// @details    ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void ice_make_operation(void)
{
    U8 mu8_return_value = 0;
    U8 mu8_comp_rps = 0;

    U16 mu16CompOnDelayBuf = 0;
    U8 mu8CompActBuf = 0;
    U8 mu8IceTrayInBlocked_Buf = 0;

    F32 mf32_AirTemp = 0;
    F32 mf32_RoomTemp = 0;

    U8 mu8GasSwitch_StatusBuf = 0;
    U8 mu8CompPowerStatus_Buf = 0;

    U8 mu8TrayMotorUp_Buf = 0;
    U8 mu8TrayMotorDown_Buf = 0;

    U8 mu8IceTrayPosition_Buf = 0;

    mu16CompOnDelayBuf = Get_CompOnDelayTime();
    mu8CompActBuf = Get_CompActStatus();
    mu8IceTrayInBlocked_Buf = Get_IceTrayInBlocked();

    mf32_AirTemp = Get_Temp(ADC_ID_TH_AIR);         // �ܱ� �µ�
    mf32_RoomTemp = Get_Temp(ADC_ID_TH_ROOM);       // ���� �µ�


    mu8GasSwitch_StatusBuf = Get_GasSwitchStatus();
    mu8CompPowerStatus_Buf = GET_STATUS_BLDC_COMP();

    mu8TrayMotorUp_Buf = Get_TrayMotorUpStatus();
    mu8TrayMotorDown_Buf = Get_TrayMotorDownStatus();

    mu8IceTrayPosition_Buf = Get_IceTrayPosition();

    //=======================================================// ���� ������
    switch(gu8IceStep)
    {
        //----------------------------------------------// �������� ����ȸ��
        case STATE_0_STANDBY :
            if ( (mu16CompOnDelayBuf == 0) ||
                 (mu8CompActBuf == TRUE) )
            {
                /*..hui [19-10-23���� 12:47:20] ���� �����߿��� Ʈ���� �ø��� �ʵ���..*/
                /*..hui [19-10-23���� 12:47:34] �����߿� ������ �з��� Ʈ���̰� �ɸ���..*/
                /*..hui [19-10-23���� 12:47:46] �ⱸ������ ������ �ʿ���..*/
                if ( (F_IceOut == CLEAR) &&
                     (mu8IceTrayInBlocked_Buf == FALSE) )
                {
                    /*if( bit_start_preheat == SET )*/
                    if ( (bit_start_preheat == SET) &&
                         (mf32_AirTemp < PREHEAT_AMB_TEMP) )
                    {
                        gu8IceStep = STATE_5_PREHEAT_HOTGAS_MOVE;
                    }
                    else
                    {
                        gu8IceStep = STATE_10_ICE_TRAY_MOVE_UP;
                    }
                }
                else
                {

                }
            }
            else
            {

            }
            break;

        case STATE_5_PREHEAT_HOTGAS_MOVE :
            /*..hui [23-4-7���� 5:10:11] �ø���ȯ��� �ְ��� �̵�..*/
            GasSwitchHotGas();
            gu8IceStep = STATE_6_CALC_PREHEAT_TIME;
            break;

        case STATE_6_CALC_PREHEAT_TIME :
            if (mu8GasSwitch_StatusBuf == GAS_SWITCH_HOTGAS)
            {
                /*..hui [23-4-7���� 5:10:03] �ø���ȯ��� �ְ��� �̵� �Ϸ� ��, COMP ����, 60HZ..*/
                mu8_comp_rps = get_preheat_mode_comp_rps();
                Set_BLDC_Comp_Hz(mu8_comp_rps);

                gu16_preheat_time = get_preheat_time();

                gu8IceStep = STATE_7_PREHEAT_OPERATION;
            }
            else
            {
                GasSwitchHotGas();
            }
            break;

        case STATE_7_PREHEAT_OPERATION :
            if ( (gu16_preheat_time > 0) &&
                 (mu8CompPowerStatus_Buf == ON) )
            {
                gu16_preheat_time--;
            }
            else
            {

            }

            if (gu16_preheat_time == 0)
            {
                GasSwitchIce();
                gu8IceStep = STATE_10_ICE_TRAY_MOVE_UP;
            }
            else
            {

            }
            break;

        //----------------------------------------------// �������� ����ȸ��
        case STATE_10_ICE_TRAY_MOVE_UP :
            //if(F_TrayMotorDOWN != SET)
            //if(gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_THROW)
            /*if(SIceTrayStepMotor.state == STEP_STATE_DEACTIVE)*/
            if (mu8TrayMotorDown_Buf == FALSE)
            {
                up_tray_motor();
                //Set_Step_Direction(1, 1400);
                //Set_Step_Direction(1, 1600);
                gu8IceStep = STATE_11_WAIT_ROOM_WATER_FULL;

                Set_SwingBar_Operation(SET);
            }
            else
            {

            }
            break;

        //----------------------------------------------// ����ȸ���� ������VV ON
        case STATE_11_WAIT_ROOM_WATER_FULL :
            //if( /*F_TrayMotorUP != SET && */gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING )
            //if( (SIceTrayStepMotor.state == STEP_STATE_DEACTIVE) && ( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING ) )
            /*if ( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING )*/
            if ( (mu8TrayMotorUp_Buf == FALSE) &&
                 (mu8IceTrayPosition_Buf == ICE_TRAY_POSITION_ICE_MAKING) )
            {
                Set_TrayMotorUpStatus(SET);

                /*..hui [23-4-7���� 5:52:57] ���� ���� �����ϰ��..*/
                /*..hui [23-8-14���� 1:56:47] ���� Ȯ���� Ʈ���� �Լ��ʿ���.. ..*/
                /*..hui [23-8-14���� 1:57:10] ������� ���� ���̿� �� �� �������� COMP �������·� ���ʿ��� ����ϰԵ�..*/
                if (mu8CompActBuf == TRUE)
                {
                    gu8IceStep = STATE_12_CONT_ICE_SWITCH_MOVE;
                }
                else
                {
                    gu8IceStep = STATE_14_CHECK_ICE_TRAY_HZ;
                }
            }
            else
            {
                /*..hui [18-3-20���� 7:21:54] ������ũ ������ ä����߿� Ʈ���� ������ ���� ������..*/
                if (mu8TrayMotorUp_Buf == FALSE)
                {
                    gu8IceStep = STATE_10_ICE_TRAY_MOVE_UP;
                }
                else
                {

                }
            }
            break;

        case STATE_12_CONT_ICE_SWITCH_MOVE :
            GasSwitchIce();
            gu8IceStep = STATE_13_CONT_RPS_SETTING;
            break;

        case STATE_13_CONT_RPS_SETTING :
            /*..hui [19-7-24���� 4:35:37] �ø���ȯ��� �̵� �Ϸ� ��..*/
            if (mu8GasSwitch_StatusBuf == GAS_SWITCH_ICE)
            {
                 mu8_comp_rps = get_ice_mode_comp_rps();
                 Set_BLDC_Comp_Hz(mu8_comp_rps);

                 gu8IceStep = STATE_14_CHECK_ICE_TRAY_HZ;
            }
            else
            {
                GasSwitchIce();
            }
            break;

        //----------------------------------------------// ����ȸ���� ������VV ON
        case STATE_14_CHECK_ICE_TRAY_HZ :
            /*..hui [19-7-23���� 2:08:21] Ʈ�����Լ� ����� ���濡 ���� ��� ����(�������� �Ĵ�)..*/
            /*..hui [19-7-23���� 2:08:26] �¼� ����߿��� Ʈ���� �Լ� ����..*/
            /*if(u8WaterOutState == HOT_WATER_SELECT && F_WaterOut == SET)*/
            /*..hui [19-8-28���� 9:54:48] �¼� �Ӹ��ƴ϶� �� �����߿��� Ʈ���� �Լ� ���..*/

            /*..hui [23-8-14���� 11:05:50] �׳� �Ѿ.. �� ������ ���� �ܰ迡�� ó��..*/
            gu16_Ice_Tray_Fill_Hz = C_ICE_TRAY_FILL_200CC;
            gu8IceStep = STATE_20_WATER_IN_ICE_TRAY;
            break;

        //-----------------------------------------------// ������VV Off
        case STATE_20_WATER_IN_ICE_TRAY :
            if (gu16_Ice_Tray_Fill_Hz <= 0)
            {
                gu8IceStep = STATE_21_ICE_SWITCH_MOVE;

//                gu16_wifi_tray_in_flow = 260;
            }
            else
            {
                /*..hui [23-7-21���� 4:53:40] Ʈ���� �� �ִ��߿� ������ ���Ϸ� �������� ���� ���..*/
                if (mu8IceTrayInBlocked_Buf == TRUE)
                {
                    down_tray_motor();

                    if (mu8CompActBuf == TRUE)
                    {
                        gu8IceStep = STATE_40_ICE_TRAY_MOVE_DOWN;
                    }
                    else
                    {
                        gu8IceStep = STATE_51_FINISH_ICE_MAKE;
                    }
                }
                else{}

//                gu16_wifi_tray_in_time++;
            }

            break;

        //-----------------------------------------------// ������VV Off
        case STATE_21_ICE_SWITCH_MOVE :
            GasSwitchIce();
            gu8IceStep = STATE_30_CALC_ICE_MAKING_TIME;
            break;

        //-----------------------------------------------// ���� ����
        case STATE_30_CALC_ICE_MAKING_TIME :

            /*..hui [19-7-24���� 4:35:37] �ø���ȯ��� �̵� �Ϸ� ��..*/
            if (mu8GasSwitch_StatusBuf == GAS_SWITCH_ICE)
            {
                if ( (mu16CompOnDelayBuf == 0) ||
                     (mu8CompActBuf == TRUE) )
                {
                    /*gu16IceMakeTime
                        = (U16)calc_ice_make_time( gu8_Amb_Front_Temperature_One_Degree, gu8_Room_Temperature_One_Degree);*/
                    gu16IceMakeTime = (U16)calc_ice_make_time(mf32_AirTemp, mf32_RoomTemp);

                    /*..hui [19-7-5���� 2:08:13] 100ms ī��Ʈ ���� ����..*/
                    gu16IceMakeTime = (U16)(gu16IceMakeTime  * 10);

                    /*..hui [25-3-27���� 1:34:22] ���� ��� ��� �߰�..*/
//                    if (bit_ice_size == ICE_SIZE_SMALL)
//                    {
//                        gu16IceMakeTime = (U16)((F32)gu16IceMakeTime * 0.8f);
//                    }
//                    else{}

                    /*..hui [20-2-19���� 3:32:11] �ڵ� ��� ���� Ż���� 3�� ���Ŀ��� �����ϱ����� �߰�..*/
//                    gu16_cody_ice_make_time  = gu16IceMakeTime;
//
//                    /*..hui [24-2-14���� 4:28:53] UV �����ð� ����..*/
//                    gu16_uv_ice_make_time = gu16IceMakeTime;
//
                    mu8_comp_rps = get_ice_mode_comp_rps();
                    Set_BLDC_Comp_Hz(mu8_comp_rps);

                    gu8IceStep = STATE_31_MAIN_ICE_MAKING;
                    gu8_ice_tray_reovery_time = 0;
                }
                else
                {

                }
            }
            else
            {
                GasSwitchIce();
            }
            break;

        //-----------------------------------------------// �����Ϸ��� ����ȸ��
        case STATE_31_MAIN_ICE_MAKING :
            if ((gu16IceMakeTime > 0) &&
                (mu8CompPowerStatus_Buf == ON) )
            {
                gu16IceMakeTime--;

//                gu32_wifi_ice_make_time++;
            }
            else
            {

            }

            if(gu16IceMakeTime == 0)
            {
                /*gu16IceHeaterTime = calc_ice_heater_time();*/
                down_tray_motor();

                gu8IceStep = STATE_40_ICE_TRAY_MOVE_DOWN;
                Set_SwingBar_Operation(CLEAR);
            }
            else
            {
                recovery_ice_tray();

                if (gu16IceMakeTime <= HOT_GAS_NOISE_REDUCE_TIME)
                {
                    reduce_hot_gas_noise();
                }
                else
                {

                }
            }
            break;

        //-----------------------------------------------// ����ȸ���Ϸ�
        case STATE_40_ICE_TRAY_MOVE_DOWN :
            //if(F_TrayMotorDOWN != SET && gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_THROW)
            //if( ( SIceTrayStepMotor.state == STEP_STATE_DEACTIVE ) &&  ( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_THROW ) )
            /*if( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_THROW )*/
            if ( (mu8TrayMotorDown_Buf == FALSE) &&
                 (mu8IceTrayPosition_Buf == ICE_TRAY_POSITION_ICE_THROW) )
            {
                Set_TrayMotorDownStatus(CLEAR);
                gu8IceStep = STATE_41_GAS_SWITCH_HOT_GAS;
            }
            else
            {

            }
            break;

        case STATE_41_GAS_SWITCH_HOT_GAS :
            mu8_comp_rps = get_hotgas_mode_comp_rps();
            Set_BLDC_Comp_Hz(mu8_comp_rps);

            GasSwitchHotGas();

            gu8IceStep = STATE_42_CALC_HOT_GAS_TIME;
            break;

        case STATE_42_CALC_HOT_GAS_TIME :
            if( mu8GasSwitch_StatusBuf == GAS_SWITCH_HOTGAS)
            {
                gu16IceHeaterTime = get_hotgas_time();
                gu8IceStep = STATE_43_ICE_TAKE_OFF;
            }
            else
            {
                GasSwitchHotGas();
            }
            break;

        case STATE_43_ICE_TAKE_OFF :
//            gu32_wifi_ice_heater_timer++;

            mu8_return_value = hot_gas_operation();

            if(mu8_return_value == SET)
            {
                F_IR = SET;              // �������� ����
                gu8IceStep = STATE_50_ICE_FULL_IR_CHECK;
            }
            else
            {

            }
            break;

        //-------------------------------------------------// SB �ü� ȸ���Ϸ�
        case STATE_50_ICE_FULL_IR_CHECK :
            if(F_IR != SET)
            {
                gu8IceStep = STATE_51_FINISH_ICE_MAKE;
            }
            else
            {

            }
            break;

        case STATE_51_FINISH_ICE_MAKE :
            gu8IceStep = STATE_0_STANDBY;
            /*..hui [23-7-21���� 4:52:04] �߰�..*/
            Set_SwingBar_Operation(CLEAR);
            break;

        default :
            gu8IceStep = STATE_0_STANDBY;
            gu8InitStep = 0;
            F_IceInit = SET;
            break;
      }

}


/// @brief      ���� �ð� ���� �Լ�
/// @details    ���� �ð��� �ܱ� �µ��� ���� �µ��� �������� �����Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �ð�
U16 calc_ice_make_time(F32 mf32AirTemp, F32 mf32RoomTemp)
{
    F32 mf32_amb_temp = 0;
    F32 mf32_room_temp = 0;

    U8 mu8_amb_temp = 0;
    U8 mu8_room_temp = 0;

    if (mf32AirTemp >= 45.0f)
    {
        mf32_amb_temp = 45.0f;
    }
    else
    {
        mf32_amb_temp = mf32AirTemp;
    }

    if (mf32RoomTemp >= 45.0f)
    {
        mf32_room_temp = 45.0f;
    }
    else
    {
        mf32_room_temp = mf32RoomTemp;
    }

    mu8_amb_temp = ConvTemp2Char(mf32_amb_temp);
    mu8_room_temp = ConvTemp2Char(mf32_room_temp);

    return  Temp_MakeTime[mu8_room_temp][mu8_amb_temp];
}


/// @brief      �ְ��� ���� �Լ�
/// @details    �ְ��� ���� ��� �Ѵ�
/// @param      void
/// @return     return : �ְ��� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 hot_gas_operation(void)
{
    if (gu16IceHeaterTime > 0)
    {
        gu16IceHeaterTime--;

//        gu32_wifi_hot_gas_time++;
    }
    else
    {

    }

    if (gu16IceHeaterTime == 0)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Ice Tray ���� ���� ����
/// @details    Ice Tray ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void recovery_ice_tray(void)
{
    U8 mu8IceTrayPosition_Buf = 0;

    mu8IceTrayPosition_Buf = Get_IceTrayPosition();

    /*..hui [18-3-22���� 9:39:58] �ⱸ���� ������ ������ �ƿ���Ʈ���̰� ������ ���� �߻��Ҽ�����..*/
    /*..hui [18-3-22���� 9:40:16] ���̽�Ʈ���̰� ������ 10�ʿ� �ѹ��� �ٽ� �÷��ش�..*/
    if (mu8IceTrayPosition_Buf != ICE_TRAY_POSITION_ICE_MAKING)
    {
        /*..hui [18-3-22���� 10:00:09] Ʈ���� ��õ���� �߿��� ���۾���..*/
        if(F_Safety_Routine != SET)
        {
            gu8_ice_tray_reovery_time++;
        }
        else
        {
            gu8_ice_tray_reovery_time = 0;
        }

        if(gu8_ice_tray_reovery_time >= 200)
        {
            gu8_ice_tray_reovery_time = 0;
            up_tray_motor();
        }
        else
        {

        }
    }
    else
    {
        gu8_ice_tray_reovery_time = 0;
    }
}


/// @brief      �ְ��� Noise ���� ����
/// @details    �ְ��� Noise ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void reduce_hot_gas_noise(void)
{
    U8 mu8_comp_rps = 0;

    mu8_comp_rps = get_hotgas_mode_comp_rps();
    Set_BLDC_Comp_Hz(mu8_comp_rps);
}


/// @brief      ���� ���� Comp Hz �Ǵ� �Լ�
/// @details    ���� ���۽� �����ϴ� Comp�� Hz�� �Ǵ��Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Comp ���� Hz
U8 get_ice_mode_comp_rps(void)
{
    U8 mu8_return = 0;
    F32 mf32AirTemp = 0;

    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

	/*.. sean [25-02-04] gu8_Amb_Temperature_One_Degree���� ó���Ǿ� front�� ����..*/
    if (mf32AirTemp <= 10.0f)
    {
        /*..hui [23-4-7���� 11:15:58] 10�� ����..*/
        mu8_return = BLDC_65Hz;
    }
    else if (mf32AirTemp <= 20.f)
    {
        /*..hui [23-4-7���� 11:16:02] 20�� ����..*/
        mu8_return = BLDC_65Hz;
    }
    else if (mf32AirTemp <= 25.0f)
    {
        /*..hui [23-4-7���� 11:16:06] 25�� ����..*/
        mu8_return = BLDC_65Hz;
    }
    else if (mf32AirTemp <= 30.0f)
    {
        /*..hui [23-4-7���� 11:16:10] 30�� ����..*/
        mu8_return = BLDC_65Hz;
    }
    else
    {
        /*..hui [23-4-7���� 11:16:14] 30�� �ʰ�..*/
        mu8_return = BLDC_60Hz;
    }

    return mu8_return;
}


/// @brief      �ְ��� ���� Comp Hz �Ǵ� �Լ�
/// @details    �ְ��� ���۽� �����ϴ� Comp�� Hz�� �Ǵ��Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Comp ���� Hz
U8 get_hotgas_mode_comp_rps(void)
{
    U8 mu8_return = 0;
    F32 mf32AirTemp = 0;

    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

    if (mf32AirTemp < 9.0f)
    {
        /*..hui [23-4-7���� 1:17:04] 9�� �̸�..*/
        mu8_return = BLDC_50Hz;
    }
    else if (mf32AirTemp <= 13.0f)
    {
        /*..hui [23-4-7���� 1:23:42] 9�� ~ 13��..*/
        mu8_return = BLDC_50Hz;
    }
    else if (mf32AirTemp <= 19.0f)
    {
        /*..hui [23-4-7���� 1:23:55] 14�� ~ 19��..*/
        mu8_return = BLDC_50Hz;
    }
    else if (mf32AirTemp <= 24.0f)
    {
        /*..hui [23-4-7���� 1:24:08] 20�� ~ 24��..*/
        mu8_return = BLDC_47Hz;
    }
    else if (mf32AirTemp <= 29.0f)
    {
        /*..hui [23-4-7���� 1:24:18] 25�� ~ 29��..*/
        mu8_return = BLDC_47Hz;
    }
    else
    {
        /*..hui [23-4-7���� 1:24:23] 30�� �̻�..*/
        mu8_return = BLDC_43Hz;
    }

    return mu8_return;
}


/// @brief      ���� ���� Comp Hz �Ǵ� �Լ�
/// @details    ���� ���۽� �����ϴ� Comp�� Hz�� �Ǵ��Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Comp ���� Hz
U8 get_preheat_mode_comp_rps(void)
{
    /*..hui [23-4-7���� 5:00:06] ���� ��� RPS 60HZ..*/
    return BLDC_60Hz;
}


/// @brief      �ְ��� ���� ���� �ð� �Ǵ� �Լ�
/// @details    �ְ��� ���۽� �����ϴ� �ð��� �Ǵ��Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ְ��� ���� ���� �ð�
U16 get_hotgas_time(void)
{
    U16 mu16_return = 0;
    F32 mf32AirTemp = 0;

    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

    if (mf32AirTemp < 9.0f)
    {
        /*..hui [23-4-7���� 1:17:04] 9�� �̸�..*/
        mu16_return = HOT_GAS_TIME_9_UNDER_765S;
    }
    else if (mf32AirTemp <= 13.0f)
    {
        /*..hui [23-4-7���� 1:23:42] 9�� ~ 13��..*/
        mu16_return = HOT_GAS_TIME_13_UNDER_600S;
    }
    else if (mf32AirTemp <= 19.0f)
    {
        /*..hui [23-4-7���� 1:23:55] 14�� ~ 19��..*/
        mu16_return = HOT_GAS_TIME_19_UNDER_180S;
    }
    else if (mf32AirTemp <= 24.0f)
    {
        /*..hui [23-4-7���� 1:24:08] 20�� ~ 24��..*/
        mu16_return = HOT_GAS_TIME_24_UNDER_30S;
    }
    else if (mf32AirTemp <= 29.0f)
    {
        /*..hui [23-4-7���� 1:24:18] 25�� ~ 29��..*/
        mu16_return = HOT_GAS_TIME_29_UNDER_20S;
    }
    else
    {
        /*..hui [23-4-7���� 1:24:23] 30�� �̻�..*/
        mu16_return = HOT_GAS_TIME_30_OVER_15S;
    }

    return mu16_return;
}


/// @brief      ���� ���� ���� �ð� �Ǵ� �Լ�
/// @details    ���� ���۽� �����ϴ� �ð��� �Ǵ��Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� �ð�
U16 get_preheat_time(void)
{
    U16 mu16_return = 0;
    F32 mf32AirTemp = 0;

    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

    if (mf32AirTemp < 14.0f)
    {
        /*..hui [23-4-7���� 5:05:22] 14�� �̸�..*/
        mu16_return = PREHEAT_TIME_14_UNDER_600S;
    }
    else if (mf32AirTemp < 20.0f)
    {
        /*..hui [23-4-7���� 5:05:35] 14�� ~ 19��..*/
        mu16_return = PREHEAT_TIME_20_UNDER_360S;
    }
    else
    {
        /*..hui [23-4-7���� 5:05:45] 20�� ~ 24��..*/
        mu16_return = PREHEAT_TIME_25_UNDER_360S;
    }

    return mu16_return;
}


/// @brief      ���� �ý��� �ʱ�ȭ �Լ�
/// @details    ���� �ý����� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void ice_system_reset(void)
{
    Set_IceSafetyRoutine(CLEAR);
    gu8IceStep = STATE_0_STANDBY;
    gu8InitStep = 0;

    F_IceVV = CLEAR;

    /*..hui [18-3-22���� 10:04:37] CCW ���� �̵��߿��� ������ ����������.. ����Ż�� ����..������ ª�Ҵ�....*/
    F_IceInit = SET;
}


/// @brief      ���� �ɸ� ���� ���� �ʱ�ȭ �Լ�
/// @details    ���� �ɸ� ���� ������ �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void ice_stuck_reset(void)
{
    Set_IceSafetyRoutine(CLEAR);
    gu8IceStep = STATE_0_STANDBY;
    gu8InitStep = 0;

    F_IceVV = CLEAR;
}


/// @brief      ���� �ý��� ���� �Լ�
/// @details    ���� �ý����� ���� ��Ų��
/// @param      void
/// @return     void
void ice_system_stop(void)
{
    Set_IceSafetyRoutine(SET);
    //F_IceMakeGo = CLEAR;
    //F_IceInit = CLEAR;
}


/// @brief      �ü� �޼� ��� ���� ���� ���� �Լ�
/// @details    �ü� �޼� ��� ���� ���¸� �����Ѵ�
/// @param      mu8Status : �ü� �޼� ��� ���� Flag - 0(�̵���), 1(����)
/// @return     void
void Set_IceValve(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IceVV = SET;
    }
    else
    {
        F_IceVV = CLEAR;
    }
}


/// @brief      �ü� �޼� ��� ���� ���� Ȯ�� �Լ�
/// @details    �ü� �޼� ��� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ü� �޼� ��� ���� Flag - 0(�̵���), 1(����)
U8 Get_IceValve(void)
{
    if (F_IceVV)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� �ý��� ���� ���� ���� ���� ���� �Լ�
/// @details    ���� �ý��� ���� ���� ���� ���¸� �����Ѵ�
/// @param      mu8Status : ���� �ý��� ���� ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_IceSafetyRoutine(U8 mu8Status)
{
    if (mu8Status)
    {
        F_Safety_Routine = SET;
    }
    else
    {
        F_Safety_Routine = CLEAR;
    }
}


/// @brief      ���� �ý��� ���� ���� ���� ���� Ȯ�� �Լ�
/// @details    ���� �ý��� ���� ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �ý��� ���� ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_IceSafetyRoutine(void)
{
    if (F_Safety_Routine)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� �˻� ���� ���� �Լ�
/// @details    ���� �˻� ������ ������ �������� ���� ���¸� �����Ѵ�
/// @param      mu8Status : ���� �˻� ���� ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_IceFullCheck(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IR = SET;
    }
    else
    {
        F_IR = CLEAR;
    }
}


/// @brief      ���� �˻� ���� ���� Ȯ�� �Լ�
/// @details    ���� �˻� ������ �����ϴ��� ���θ� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �˻� ���� ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_IceFullCheck(void)
{
    if (F_IR)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ���� ���� �Լ�
/// @details    ���� ���� �������� �����Ѵ�
/// @param      mu8Status : ���� ���� ���� - 0(���� �ƴ�), 1(����)
/// @return     void
void Set_IceFullStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IceFull = SET;
    }
    else
    {
        F_IceFull = CLEAR;
    }
}


/// @brief      ���� ���� Ȯ�� �Լ�
/// @details    ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� - 0(���� �ƴ�), 1(����)
U8 Get_IceFullStatus(void)
{
    if (F_IceFull)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� �ʱ�ȭ ���� ���� �Լ�
/// @details    ���� �ʱ�ȭ ������ ������ �������� ���� ���¸� �����Ѵ�
/// @param      mu8Status : ���� �ʱ�ȭ ���� ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_InitIceStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IceInit = SET;
    }
    else
    {
        F_IceInit = CLEAR;
    }
}


/// @brief      ���� �ʱ�ȭ ���� ���� Ȯ�� �Լ�
/// @details    ���� �ʱ�ȭ ������ �����ϴ����� ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �ʱ�ȭ ���� ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_InitIceStatus(void)
{
    if (F_IceInit)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� �ʱ�ȭ ���� �ܰ� ���� �Լ�
/// @details    ���� �ʱ�ȭ ���� �ܰ踦 �����Ѵ�
/// @param      mu8Step : ���� �ʱ�ȭ ���� �ܰ�
/// @return     void
void Set_InitIceStep(U8 mu8Step)
{
    gu8InitStep = mu8Step;
}


/// @brief      ���� ���� ���� �ʱ�ȭ ���� �ܰ� Ȯ�� �Լ�
/// @details    ���� ���� ���� �ʱ�ȭ ���� �ܰ踦 Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �ʱ�ȭ ���� �ܰ�
U8 Get_InitIceStep(void)
{
    return  gu8InitStep;
}


/// @brief      ���� ���� �ܰ� ���� �Լ�
/// @details    ���� ���� �ܰ踦 �����Ѵ�
/// @param      mu8Step : ���� ���� �ܰ�
/// @return     void
void Set_IceStep(U8 mu8Step)
{
    gu8IceStep = mu8Step;
}


/// @brief      ���� ���� ���� ���� �ܰ� Ȯ�� �Լ�
/// @details    ���� ���� ���� ���� �ܰ踦 Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� �ܰ�
U8 Get_IceStep(void)
{
    return  gu8IceStep;
}


/// @brief      ���� �Ұ� ���� ���� �Լ�
/// @details    ���� �Ұ� ���¸� �����Ѵ�
/// @param      mu8Status : ���� �Ұ� ���� - 0(�̹߻�), 1(�߻�)
/// @return     void
void Set_IceMakingDisable(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8IceMakeDisableStatus = SET;
    }
    else
    {
        gu8IceMakeDisableStatus = CLEAR;
    }
}


/// @brief      ���� �Ұ� ���� Ȯ�� �Լ�
/// @details    ���� �Ұ� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �Ұ� ���� - 0(�̹߻�), 1(�߻�)
U8 Get_IceMakingDisable(void)
{
    if (gu8IceMakeDisableStatus)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ���� ȸ�� ���°� ���� �Լ�
/// @details    ���� ���� ȸ�� ���� Flag�� �����Ѵ�
/// @param      mu8Status : ���� ȸ�� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_IceOutCCWStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IceOutCCW = SET;
    }
    else
    {
        F_IceOutCCW = CLEAR;
    }
}


/// @brief      ���� ���� ȸ�� ���°� Ȯ�� �Լ�
/// @details    ���� ���� ȸ�� ���� Flag�� ���� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ȸ�� ���� - 0(�̵���), 1(����)
U8 Get_IceOutCCWStatus(void)
{
    if (F_IceOutCCW)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ���� ���� ���� �Լ�
/// @details    ���� ���� ���°��� �����Ѵ�
/// @param      mu8Status : ���� ���� ���� - 0(������), 1(����)
/// @return     void
void Set_IceOutStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_IceOut = SET;
    }
    else
    {
        F_IceOut = CLEAR;
    }
}


/// @brief      ���� ���� ���� Ȯ�� �Լ�
/// @details    ���� ���� ���°��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� - 0(������), 1(����)
U8 Get_IceOutStatus(void)
{
    if (F_IceOut)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ���� ���� ���� ���� �Լ�
/// @details    ���� ���� ���� ���°��� �����Ѵ�
/// @param      mu8Status : ���� ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_IceOutContinueStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8Ice_out_continue = SET;
    }
    else
    {
        gu8Ice_out_continue = CLEAR;
    }
}


/// @brief      ���� ���� ���� ���� Ȯ�� �Լ�
/// @details    ���� ���� ���� ���°��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_IceOutContinueStatus(void)
{
    if (gu8Ice_out_continue)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� �Ұ� ��Ȳ Ȯ�� �Լ�
/// @details    ���� �Ұ� ��Ȳ�� �߻��ߴ��� üũ�Ѵ�
/// @param      void
/// @return     void
void Check_AbleIceMaking(void)
{
    U8 mu8ErrorAboutIceMaking = 0;

    U8 mu8E43_Buf = 0;
    U8 mu8E53_Buf = 0;
    U8 mu8E01_Buf = 0;
    U8 mu8E61_Buf = 0;
    U8 mu8E62_Buf = 0;
    U8 mu8E63_Buf = 0;

    U8 mu8IceTrayPosition_Buf = 0;

    mu8ErrorAboutIceMaking = Get_ErrorAboutIceMaking();

    if (mu8ErrorAboutIceMaking == TRUE)
    {
        Set_IceMakingDisable(SET);

        if (F_IceInit == SET)
        {
            gu32_ice_init_cancel_timer++;

            /*..hui [24-2-6���� 9:43:07] ����Ż�� ���� ���� ���� ���� �߻��ϰ� 2�ð� ������ ����Ż�� ���..*/
            /*..hui [24-2-6���� 9:43:13] �ð� ��ȯ�ؼ� �ð��ϱ� ����..*/
            if (gu32_ice_init_cancel_timer >= ICE_INIT_ERROR_CANCEL_TIME)
            {
                gu32_ice_init_cancel_timer = 0;
                F_IceInit = CLEAR;
                gu8InitStep = 0;
            }
            else
            {

            }
        }
        else
        {
            gu32_ice_init_cancel_timer = 0;
        }

        /*..hui [19-12-18���� 7:50:28] ���� �����ǰ� ����Ż�� �����ϴ� ������..*/
        /*..hui [19-12-18���� 10:41:07] �ǵ���/���� ��߶��� ���� �߰�..*/
        /*..hui [19-12-18���� 10:44:09] �ǵ���/���� ��߶����� �ڵ������� �ƴϱ⶧���� �ǹ̾���..*/
        /*..hui [19-12-18���� 10:44:30] ���������� ������ ��������..*/
        /*..hui [19-12-19���� 1:00:22] �����µ������� ���� ���� ���������ϱ� ������ ����..*/
           /*..hui [23-9-20���� 9:24:23] �� �� ����..*/
        mu8E43_Buf = Get_ErrorStatus(ERROR_ID_AIR_TEMP_1_E43);
        mu8E53_Buf = Get_ErrorStatus(ERROR_ID_AIR_TEMP_2_E53);
        mu8E01_Buf = Get_ErrorStatus(ERROR_ID_LEAKAGE_E01);

        if ( ( (mu8E43_Buf == TRUE) && (mu8E53_Buf == TRUE) ) ||
             (mu8E01_Buf == TRUE) )
        {
            if ( (gu8IceStep >= STATE_31_MAIN_ICE_MAKING) &&
                 (gu8IceStep <= STATE_43_ICE_TAKE_OFF) )
            {
                /*..hui [19-12-18���� 10:43:42] �����߿� ���� �ɷ������� ���������� ����Ż�� ����..*/
                F_re_ice_init_error = SET;
            }
            else
            {

            }
        }
        else
        {

        }

        /*..hui [20-2-13���� 5:42:54] ���� �߻� �� Ʈ���� ����.. Ʈ���̿� �� ���ִ� ��Ȳ �߻��� �� ����..*/
        /*..hui [20-2-14���� 11:20:25] ��, Ʈ���� ���� ������ �׳� ����ġ��� ���ܵ�.. ���� �����ϵ���..*/
        mu8E61_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ALL_DETECT_E61);
        mu8E62_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62);
        mu8E63_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_DE_ICE_E63);

        mu8IceTrayPosition_Buf = Get_IceTrayPosition();

        if ( (mu8E61_Buf == FALSE) &&
             (mu8E62_Buf == FALSE) &&
             (mu8E63_Buf == FALSE) )
        {
            if( gu8IceStep >= STATE_10_ICE_TRAY_MOVE_UP )
            {
                if (mu8IceTrayPosition_Buf != ICE_TRAY_POSITION_ICE_THROW)
                {
                    //down_tray_motor();
                    /*.. sean [25-02-05] Ʈ���� �̰��� �̽��� ���ܸ��� pulse �߰�..*/
                	//Set_Step_Direction(0, 1400);
                	/*Set_Step_Direction(0, TRAY_MOVE_MAX);*/
                    down_tray_motor();
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
    }
    else
    {
        Set_IceMakingDisable(CLEAR);

        if (F_re_ice_init_error == SET)
        {
            F_re_ice_init_error = CLEAR;
            F_IceInit = SET;
        }
        else
        {

        }
    }
}


/// @brief      ���� �ܿ� �ð� Ȯ�� �Լ�
/// @details    ���� �ܿ� �ð��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �ܿ� �ð�
U16 Get_IceMakeTime(void)
{
    return  gu16IceMakeTime;
}


/// @brief      ���� �ý��� �̻� ���� üũ �Լ�
/// @details    ���� �ý��� �̻� ���θ� üũ�Ѵ�
/// @param      void
/// @return     void
void check_ice_system_ok(void)
{
    U8 mu8E61_Buf = 0;
    U8 mu8E62_Buf = 0;
    U8 mu8E63_Buf = 0;

    U8 mu8TrayUpMoveReset_Buf = 0;
    U8 mu8TrayDownMoveReset_Buf = 0;

    U8 mu8Err_TrayDownCount_Buf = 0;
    U8 mu8Err_TrayUpCount_Buf = 0;

    mu8E61_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ALL_DETECT_E61);
    mu8E62_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62);
    mu8E63_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_DE_ICE_E63);

    mu8TrayUpMoveReset_Buf = Get_TrayUpMoveReset();
    mu8TrayDownMoveReset_Buf = Get_TrayDownMoveReset();
    mu8Err_TrayDownCount_Buf = Get_TrayDown_ErrorCount();
    mu8Err_TrayUpCount_Buf = Get_TrayUp_ErrorCount();

    if ( (mu8E61_Buf == FALSE) &&
         (mu8E62_Buf == FALSE) &&
         (mu8E63_Buf == FALSE) &&
         (mu8TrayUpMoveReset_Buf == FALSE) &&
         (mu8TrayDownMoveReset_Buf == FALSE) &&
         (F_Safety_Routine == CLEAR) &&
         (mu8Err_TrayDownCount_Buf == 0) &&
         (mu8Err_TrayUpCount_Buf == 0 ) )
    {
        gu8Ice_system_ok = SET;
    }
    else
    {
        gu8Ice_system_ok = CLEAR;
    }
}


/// @brief      ���� �ý��� �̻� ���� ���� ���� �Լ�
/// @details    ���� �ý��� �̻� ���� ���¸� �����Ѵ�
/// @param      mu8Status : ���� �ý��� �̻� ���� - 0(�̻�), 1(����)
/// @return     void
void Set_IceSystemStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8Ice_system_ok = SET;
    }
    else
    {
        gu8Ice_system_ok = CLEAR;
    }
}


/// @brief      ���� �ý��� �̻� ���� ���� Ȯ�� �Լ�
/// @details    ���� �ý��� �̻� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �ý��� �̻� ���� - 0(�̻�), 1(����)
U8 Get_IceSystemStatus(void)
{
    if (gu8Ice_system_ok)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


#endif


// Lib Ice Making Module *********************************************************************************

/// @brief      Lib Ice Making Module Initilaize
/// @details    Ice Making ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_IceMaking_Module_Initialize(void)
{
#if (LIB_ICE_MAKING_USE == USE)
    Lib_IceMaking_Initialize();
#endif
}


/// @brief      Lib Ice Making Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� Ice Making ���� ��� Library
/// @param      void
/// @return     void
void Lib_IceMaking_Module_1ms_Control(void)
{
#if (LIB_ICE_MAKING_USE == USE)
    IceMakingControlTimer();
#endif
}


/// @brief      Lib Ice Making Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Ice Making ���� ��� Library
/// @param      void
/// @return     void
void Lib_IceMaking_Module_Control(void)
{
#if (LIB_ICE_MAKING_USE == USE)
    Check_AbleIceMaking();
    Ice_Make_Process();
#endif
}
