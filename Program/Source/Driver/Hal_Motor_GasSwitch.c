/// @file     Hal_Motor_GasSwitch.c
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Gas Switch ���Ǹ��� ����

#include "Global_Header.h"


#if (MOTOR_GAS_SWITCH_USE == USE)

U8 gu8GasSwitchControlTimer = 0;                /// @brief  Gas Switch ���� �ð� ī����

U8 F_Comp_Output_Old = 0;                       /// @brief  Comp ���� ���� ���� - 0(�̵���), 1(����)
U8 F_Comp_Output = 0;                           /// @brief  Comp ���� ���� ���� - 0(�̵���), 1(����)
U8 F_Comp_Restart_Delay_Status = 0;             /// @brief  Comp �⵿ �� ���� �ð� ���� ���� - 0(�ð��� ������), 1(���� �� ���� ������)

U8 gu8_GasSwitch_Mode = 0;                      /// @brief  Gas Switch ���� ���� ���
U8 gu8_GasSwitch_Status = 0;                    /// @brief  Gas Switch�� ���� ���� ���� - 0(�ü�), 1(����), 2(�ְ���)

U8 gu8_GasSwitch_InitStep = 0;                  /// @brief  Gas Switch �ʱ�ȭ ���� ���� Step
U8 gu8_GasSwitch_ColdStep = 0;                  /// @brief  Gas Switch �ü� ���� ���� Step
U8 gu8_GasSwitch_IceStep = 0;                   /// @brief  Gas Switch ���� ���� ���� Step
U8 gu8_GasSwitch_HotGasStep = 0;                /// @brief  Gas Switch �ְ��� ���� ���� Step

U8 gu8_GasSwitch_PowerOnTime = 0;               /// @brief  Gas Switch ���� ON �ð�

S16 gs16_sm_info_target = 0;                    /// @brief  Gas Switch ���Ǹ��� ���� ��ǥ ��ġ ����
S16 gs16_sm_info_current = 0;                   /// @brief  Gas Switch ���Ǹ��� ���� ���� ��ġ ����
S16 gs16_sm_info_move_step = 0;                 /// @brief  Gas Switch ���Ǹ��� ���� �̵��� ����
S16 gs16_sm_info_hold_start_step = 0;           /// @brief  Gas Switch ���Ǹ��� ���� Hold �ð�
S16 gs16_sm_info_hold_stop_step = 0;            /// @brief  Gas Switch ���Ǹ��� ���� Hold �ð�
S8 gs8_sm_info_current_mode = 0;                /// @brief  Gas Switch ���Ǹ��� ���� ���� ���� ��ġ ����
S8 gs8_sm_info_prev_mode = 0;                   /// @brief  Gas Switch ���Ǹ��� ���� ���� ���� ��ġ ����

const U8 gu8PhaseTable[PHASE_STEP_NUM] = { 8, 7, 6, 5, 4, 3, 2, 1 };

U8 F_GasSwitch_Initial = 0;                     /// @brief  Gas Switch ���Ǹ��� ���� �ʱ�ȭ ���� - 0(�̵���), 1(����)

U8 gu8MotorControl_30ms = 0;                    /// @breif  Gas Switch ���Ǹ��� ���� ���� �ֱ� 30ms Ÿ�̸�


/// @brief      �ð��� ���� ���� ���� �Լ�
/// @details    �ð��Ⱑ ���� ������ ���¸� �����Ѵ�
/// @param      mu8Status : �ð��� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_CompActStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_Comp_Output = SET;
    }
    else
    {
        F_Comp_Output = CLEAR;
    }
}


/// @brief      �ð��� ���� ���� Ȯ�� �Լ�
/// @details    �ð��Ⱑ ���� ������ ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ð��� ���� ���� - 0(�̵���), 1(����)
U8 Get_CompActStatus(void)
{
    if (F_Comp_Output)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      �ð��� ���� �� ���� ���� ���� ���� �Լ�
/// @details    �ð��� ���� �� ���� ������ ���¸� �����Ѵ�
/// @param      mu8Status : �ð��� ���� �� ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_CompReactDelayStatus(U8 mu8Status)
{
    if (mu8Status)
    {
        F_Comp_Restart_Delay_Status = SET;
    }
    else
    {
        F_Comp_Restart_Delay_Status = CLEAR;
    }
}


/// @brief      �ð��� ���� �� ���� ���� ���� Ȯ�� �Լ�
/// @details    �ð��� ���� �� ���� ���� ���¸� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ð��� ���� �� ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_CompReactDelayStatus(void)
{
    if (F_Comp_Restart_Delay_Status)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Gas Switch ���� ���� ���� Ȯ�� �Լ�
/// @details    Gas Switch ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Gas Switch�� ���� ���� ���� - 0(�ü�), 1(����), 2(�ְ���)
U8 Get_GasSwitchStatus(void)
{
    return  gu8_GasSwitch_Status;
}


/// @brief      Gas Switch ���� ���� ���� �ʱ�ȭ
/// @details    Gas Switch ����� ���õ� �������� �ʱ�ȭ��Ų��
/// @param      void
/// @return     void
void Hal_GasSwitch_Initialize(void)
{
    gu8GasSwitchControlTimer = 0;

    F_Comp_Output_Old = 0;
    F_Comp_Output = 0;
    F_Comp_Restart_Delay_Status = 0;

    gu8_GasSwitch_Mode = 0;
    gu8_GasSwitch_Status = GAS_SWITCH_ICE;

    gu8_GasSwitch_InitStep = 0;
    gu8_GasSwitch_ColdStep = 0;
    gu8_GasSwitch_IceStep = 0;
    gu8_GasSwitch_HotGasStep = 0;

    gu8_GasSwitch_PowerOnTime = POWER_ON_TIME;

    gs16_sm_info_target = GAS_SWITCH_END_STEP_VAL;
    gs16_sm_info_current = GAS_SWITCH_END_STEP_VAL;
    gs16_sm_info_move_step = 0;
    gs16_sm_info_hold_start_step = 0;
    gs16_sm_info_hold_stop_step = 0;
    gs8_sm_info_current_mode = 0;
    gs8_sm_info_prev_mode = 0;

    F_GasSwitch_Initial = SET;

    gu8MotorControl_30ms = 0;
}


/// @brief      Gas Switch control Timer(@1ms interrupt)
/// @details    Gas Switch ���� �ð� �ֱ⸦ ī��Ʈ�Ѵ�
/// @param      void
/// @return     void
void GasSwitchControlTimer(void)
{
    if (gu8GasSwitchControlTimer < GAS_SWITCH_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8GasSwitchControlTimer++;
    }
}


/// @brief      Gas Switch ���� �Լ�
/// @details    Gas Switch ���� ������ �Ѵ�
/// @param      void
/// @return     void
void Hal_GasSwitchControl(void)
{
    U8 mu8Ret = FALSE;

    if (gu8GasSwitchControlTimer >= GAS_SWITCH_CONTROL_TIME_PERIOD)
    {   // 100ms ���� ó��
        gu8GasSwitchControlTimer = 0;

        ProcessGasSwitch();

        /* Init */
        // �ʱ�ȭ�� �ϰ� ���� ���� ��ġ�� �ü���������,
        // ��ġ ������ ���� �ʴ´�.
        if ( (gu8_GasSwitch_Mode & GAS_SWITCH_MODE_INIT) == GAS_SWITCH_MODE_INIT )
        {
            mu8Ret = InitSwitch();

            if (mu8Ret == TRUE)
            {
                gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_INIT;
                gu8_GasSwitch_InitStep = 0;
                gu8_GasSwitch_Status = GAS_SWITCH_COLD;
            }
            else
            {

            }
            return;
        }
        else
        {

        }

        /* Cold */
        if( (gu8_GasSwitch_Mode & GAS_SWITCH_MODE_COLD ) == GAS_SWITCH_MODE_COLD )
        {
            mu8Ret = ColdSwitch();

            if (mu8Ret == TRUE)
            {
                gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_COLD;
                gu8_GasSwitch_ColdStep = 0;
                gu8_GasSwitch_Status = GAS_SWITCH_COLD;
            }
            else
            {

            }
            return;
        }
        else
        {

        }

        /* Ice */
        if ( (gu8_GasSwitch_Mode & GAS_SWITCH_MODE_ICE) == GAS_SWITCH_MODE_ICE )
        {
            mu8Ret = IceSwitch();

            if (mu8Ret == TRUE)
            {
                gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_ICE;
                gu8_GasSwitch_IceStep = 0;
                gu8_GasSwitch_Status = GAS_SWITCH_ICE;
            }
            else
            {

            }
            return;
        }
        else
        {

        }

        /* HOT-GAS */
        if ( (gu8_GasSwitch_Mode & GAS_SWITCH_MODE_HOTGAS ) == GAS_SWITCH_MODE_HOTGAS )
        {
            mu8Ret = HotGasSwitch();

            if (mu8Ret == TRUE)
            {
                gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_HOTGAS;
                gu8_GasSwitch_HotGasStep = 0;
                gu8_GasSwitch_Status = GAS_SWITCH_HOTGAS;
            }
            return;
        }
        else
        {

        }
    }
}


/// @brief      Gas Switch �ʱ�ȭ ���� �Լ�
/// @details    Gas Switch �ʱ�ȭ ���� ������ �Ѵ�
/// @param      void
/// @return     return : �ʱ�� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 InitSwitch(void)
{
    U8 mu8Return = 0;

    mu8Return = FALSE;

    switch (gu8_GasSwitch_InitStep)
    {
        case INIT_POWER_ON:
            gu8_GasSwitch_PowerOnTime = POWER_ON_TIME;
            gu8_GasSwitch_InitStep++;
            break;

        case INIT_WAIT_POWER_ON:
            if (gu8_GasSwitch_PowerOnTime != 0)
            {
                gu8_GasSwitch_PowerOnTime--;
            }
            else
            {
                gu8_GasSwitch_InitStep++;
            }
            break;

        case INIT_MOVE_ZERO:
            gs16_sm_info_hold_start_step = DEFAULT_HOLD_STEP;  /* about 0.5sec */
            gs16_sm_info_hold_stop_step = DEFAULT_HOLD_STEP;  /* aobut 0.5esc */
            gs16_sm_info_target = GAS_SWITCH_INIT_STEP_VAL;

            gu8_GasSwitch_InitStep++;
            break;

        case INIT_WAIT_ZERO:
            if (gs16_sm_info_target == gs16_sm_info_current)
            {
                gu8_GasSwitch_InitStep++;

                gs16_sm_info_target = 0;
                gs16_sm_info_current = 0;
            }
            break;

        case INIT_MOVE_COLD:
            gs16_sm_info_hold_start_step = DEFAULT_HOLD_STEP;  /* about 0.5sec */
            gs16_sm_info_hold_stop_step = DEFAULT_HOLD_STEP;  /* aobut 0.5esc */
            gs16_sm_info_target = GAS_SWITCH_COLD_STEP_VAL;

            gu8_GasSwitch_InitStep++;
            break;

        case INIT_WAIT_COLD:
            if (gs16_sm_info_target == gs16_sm_info_current)
            {
                gu8_GasSwitch_InitStep++;
            }
            else
            {

            }
            break;

        case INIT_DONE :
            mu8Return = TRUE;
            break;

        default :
            gu8_GasSwitch_InitStep = INIT_POWER_ON;
            break;
    }

    return  mu8Return;
}


/// @brief      Gas Switch �ü� ���� �Լ�
/// @details    Gas Switch �ü� ���� ������ �Ѵ�
/// @param      void
/// @return     return : ���� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 ColdSwitch(void)
{
    U8 mu8Return = 0;

    mu8Return = FALSE;

    switch (gu8_GasSwitch_ColdStep)
    {
        // POWER ON
        case COLD_POWER_ON:
            gu8_GasSwitch_PowerOnTime = POWER_ON_TIME;
            gu8_GasSwitch_ColdStep++;
            break;

        case COLD_WAIT_POWER_ON:
            if (gu8_GasSwitch_PowerOnTime != 0)
            {
                gu8_GasSwitch_PowerOnTime--;
            }
            else
            {
                gu8_GasSwitch_ColdStep++;
            }
            break;

            // MOVE
        case COLD_CHECK_POSITION:
            if (gs16_sm_info_target == GAS_SWITCH_COLD_STEP_VAL)
            {
                gu8_GasSwitch_ColdStep = COLD_WAIT;
            }
            else
            {
                gu8_GasSwitch_ColdStep++;
            }
            break;

        case COLD_MOVE:
            gs16_sm_info_hold_start_step = DEFAULT_HOLD_STEP;  /* about 0.5sec */
            gs16_sm_info_hold_stop_step = DEFAULT_HOLD_STEP;  /* aobut 0.5esc */
            gs16_sm_info_target = GAS_SWITCH_COLD_STEP_VAL;

            gu8_GasSwitch_ColdStep++;
            break;

        case COLD_WAIT:
            if (gs16_sm_info_target == gs16_sm_info_current)
            {
                gu8_GasSwitch_ColdStep++;
            }
            else
            {

            }
            break;

        case COLD_DONE:
            mu8Return = TRUE;
            break;

        default :
            gu8_GasSwitch_ColdStep = COLD_POWER_ON;
            break;
    }

    return  mu8Return;
}


/// @brief      Gas Switch ���� ���� �Լ�
/// @details    Gas Switch ���� ���� ������ �Ѵ�
/// @param      void
/// @return     return : ���� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 IceSwitch(void)
{
    U8 mu8Return = 0;

    mu8Return = FALSE;

    switch (gu8_GasSwitch_IceStep)
    {
        case ICE_POWER_ON:
            gu8_GasSwitch_PowerOnTime = POWER_ON_TIME;
            gu8_GasSwitch_IceStep++;
            break;

        case ICE_WAIT_POWER_ON:
            if (gu8_GasSwitch_PowerOnTime != 0)
            {
                gu8_GasSwitch_PowerOnTime--;
            }
            else
            {
                gu8_GasSwitch_IceStep++;
            }
            break;

        // ���� ��ġ�̸� �̵� ���� ���� ����
        case ICE_CHECK_POSITION:
            if (gs16_sm_info_target == GAS_SWITCH_ICE_STEP_VAL)
            {
                gu8_GasSwitch_IceStep = ICE_WAIT;
            }
            else
            {
                gu8_GasSwitch_IceStep++;
            }
            break;

        case ICE_MOVE:
            gs16_sm_info_hold_start_step = DEFAULT_HOLD_STEP;  /* about 0.5sec */
            gs16_sm_info_hold_stop_step = DEFAULT_HOLD_STEP;  /* aobut 0.5esc */
            gs16_sm_info_target = GAS_SWITCH_ICE_STEP_VAL;

            gu8_GasSwitch_IceStep++;
            break;

        case ICE_WAIT:
            if (gs16_sm_info_target == gs16_sm_info_current)
            {
                gu8_GasSwitch_IceStep++;
            }
            else
            {

            }
            break;

        case ICE_DONE:
            mu8Return = TRUE;
            break;

        default :
            gu8_GasSwitch_IceStep = ICE_POWER_ON;
            break;
    }

    return  mu8Return;
}


/// @brief      Gas Switch �ְ��� ���� �Լ�
/// @details    Gas Switch �ְ��� ���� ������ �Ѵ�
/// @param      void
/// @return     return : ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 HotGasSwitch(void)
{
    U8 mu8Return = 0;

    mu8Return = FALSE;

    switch (gu8_GasSwitch_HotGasStep)
    {
        case HOTGAS_POWER_ON:
            gu8_GasSwitch_PowerOnTime = POWER_ON_TIME;
            gu8_GasSwitch_HotGasStep++;
            break;

        case HOTGAS_WAIT_POWER_ON:
            if (gu8_GasSwitch_PowerOnTime != 0)
            {
                gu8_GasSwitch_PowerOnTime--;
            }
            else
            {
                gu8_GasSwitch_HotGasStep++;
            }
            break;

        // ���� ��ġ�̸� �̵� ���� ���� ����
        case HOTGAS_CHECK_POSITION:
            if (gs16_sm_info_target == GAS_SWITCH_HOTGAS_STEP_VAL)
            {
                gu8_GasSwitch_HotGasStep = ICE_WAIT;
            }
            else
            {
                gu8_GasSwitch_HotGasStep++;
            }
            break;

        case HOTGAS_MOVE:
            gs16_sm_info_hold_start_step = DEFAULT_HOLD_STEP;  /* about 0.5sec */
            gs16_sm_info_hold_stop_step = DEFAULT_HOLD_STEP;  /* aobut 0.5esc */
            gs16_sm_info_target = GAS_SWITCH_HOTGAS_STEP_VAL;

            gu8_GasSwitch_HotGasStep++;
            break;

        case HOTGAS_WAIT:
            if (gs16_sm_info_target == gs16_sm_info_current)
            {
                gu8_GasSwitch_HotGasStep++;
            }
            break;

        case HOTGAS_DONE:
            mu8Return = TRUE;
            break;

        default :
            gu8_GasSwitch_HotGasStep = HOTGAS_POWER_ON;
            break;

    }

    return  mu8Return;
}


/// @brief      Gas Switch ���� ���� �Լ�
/// @details    Gas Switch ���� ���� ������ �Ѵ�
/// @param      void
/// @return     void
void ProcessGasSwitch(void)
{
    U8 mu8CompReactDelayStatusBuf = 0;
    U8 mu16CompOffDelayBuf = 0;

    if (F_Comp_Output_Old != F_Comp_Output)
    {
        F_Comp_Output_Old = F_Comp_Output;

        if (F_Comp_Output == OFF)
        {
            F_GasSwitch_Initial = SET;  // ���� �ʱ�ȭ ����
        }
        else
        {

        }
    }
    else
    {

    }

    // ����� OFF ��, 4�� �Ŀ� ���� �ʱ�ȭ ����
    mu8CompReactDelayStatusBuf = Get_CompReactDelayStatus();
    mu16CompOffDelayBuf = Get_CompOnDelayTime();

    if (F_GasSwitch_Initial == SET)
    {
        if (mu8CompReactDelayStatusBuf == TRUE)
        {   // �ð� ��⵿ ���� ���� ���� ���
            if (mu16CompOffDelayBuf >= (DELAY_TIME_BEFORE_COMP_ON - 30) )
            {   // Comp ���� �ð��� 30�� ������ ��
                F_GasSwitch_Initial = CLEAR;
                GasSwitchInit();
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


/// @brief      Gas Switch ���Ǹ��� ���� ���� �Լ�(@1ms interrupt)
/// @details    1ms Interrput �ȿ��� Gas Switch ���Ǹ��� ���� ���� ������ �Ѵ�
/// @param      void
/// @return     void
void GasSwitch_StepMotorTimerInterrupt(void)
{
    if (++gu8MotorControl_30ms >= 30)
    {   // 30ms ����
        gu8MotorControl_30ms = 0;

        // �ø���ȯ���
        HAL_ControlStepMotor();   // Gas Switch
    }
}


/// @brief      Gas Switch ���Ǹ��� ���� �Լ�(@30ms)
/// @details    100us Interrput �ȿ��� 30ms ���� Gas Switch ���Ǹ��� ���� ������ �Ѵ�
/// @param      void
/// @return     void
void HAL_ControlStepMotor(void)
{
    S16 ms16val = 0;
    S16 ms16target = 0;
    S16 ms16current = 0;

    ms16val = gs16_sm_info_move_step;
    ms16target = gs16_sm_info_target;
    ms16current = gs16_sm_info_current;

    /* CALC STEP, TARGET - CURRENT */
    if (ms16val == 0)
    {
        if (ms16target != ms16current)
        {
            ms16val = ms16target - ms16current;

            gs16_sm_info_move_step = ms16val;
            gs16_sm_info_hold_start_step = DEFAULT_HOLD_STEP;  /* about 0.5sec */
            gs16_sm_info_hold_stop_step = DEFAULT_HOLD_STEP;  /* aobut 0.5esc */
        }
        else
        {

        }
    }
    else
    {

    }

    HAL_MoveStep();
}


/// @brief      Gas Switch ���Ǹ��� �ܰ躰 ���� �Լ�
/// @details    Gas Switch ���Ǹ��� ��� �ܰ������� ������ �Ѵ�
/// @param      void
/// @return     void
void HAL_MoveStep(void)
{
    U8 mu8Val = 0;

    if (gs16_sm_info_hold_start_step > 0)
    {   // START HOLD
        gs16_sm_info_hold_start_step--;
        mu8Val = gu8PhaseTable[gs8_sm_info_prev_mode];
    }
    else if (gs16_sm_info_move_step == 0)
    {
        /* STOP HOLD */
        if (gs16_sm_info_hold_stop_step > 0)
        {
            gs16_sm_info_hold_stop_step--;
            mu8Val = gu8PhaseTable[gs8_sm_info_current_mode];
        }
        else
        {
            /* STOP */
            mu8Val = 0x00;
        }
    }

    else if (gs16_sm_info_move_step > 0)
    {   // MOVE FORWARD
        /* OPEN */
        gs16_sm_info_move_step--;
        gs16_sm_info_current++;

        gs8_sm_info_prev_mode = gs8_sm_info_current_mode;
        gs8_sm_info_current_mode++;
        if (gs8_sm_info_current_mode >= PHASE_STEP_NUM)
        {
            gs8_sm_info_current_mode = 0;
        }
        else
        {

        }

        mu8Val = gu8PhaseTable[gs8_sm_info_current_mode];
    }
    else if (gs16_sm_info_move_step < 0)
    {   // MOVE BACKWARD
        /* CLOSE */
        gs16_sm_info_move_step++;
        gs16_sm_info_current--;

        gs8_sm_info_prev_mode = gs8_sm_info_current_mode;
        gs8_sm_info_current_mode--;
        if (gs8_sm_info_current_mode < 0)
        {
            gs8_sm_info_current_mode = PHASE_STEP_NUM - 1;
        }
        else
        {

        }

        mu8Val = gu8PhaseTable[gs8_sm_info_current_mode];
    }
    else
    {

    }

    HAL_ControlGasValveMotor(mu8Val);
}


/// @brief      Gas Switch �ø� ���� ��ȯ ���Ǹ��� �ܰ躰 ���� ���� �Լ�
/// @details    Gas Switch �ø� ���� ��ȯ ���Ǹ��� ��� �ܰ������� ���� ���� ������ �Ѵ�
/// @param      mu8Mode : ���Ǹ��� ���� ���� ����
/// @return     void
void HAL_ControlGasValveMotor(U8 mu8Mode)
{
    switch (mu8Mode)
    {
        case 0:
            TURN_OFF_MOTOR_GAS_SW_1();
            TURN_OFF_MOTOR_GAS_SW_2();
            TURN_OFF_MOTOR_GAS_SW_3();
            TURN_OFF_MOTOR_GAS_SW_4();
            break;

        case 1:
            TURN_ON_MOTOR_GAS_SW_1();
            TURN_OFF_MOTOR_GAS_SW_2();
            TURN_OFF_MOTOR_GAS_SW_3();
            TURN_ON_MOTOR_GAS_SW_4();
            break;

        case 2:
            TURN_OFF_MOTOR_GAS_SW_1();
            TURN_OFF_MOTOR_GAS_SW_2();
            TURN_OFF_MOTOR_GAS_SW_3();
            TURN_ON_MOTOR_GAS_SW_4();
            break;

        case 3:
            TURN_OFF_MOTOR_GAS_SW_1();
            TURN_OFF_MOTOR_GAS_SW_2();
            TURN_ON_MOTOR_GAS_SW_3();
            TURN_ON_MOTOR_GAS_SW_4();
            break;

        case 4:
            TURN_OFF_MOTOR_GAS_SW_1();
            TURN_OFF_MOTOR_GAS_SW_2();
            TURN_ON_MOTOR_GAS_SW_3();
            TURN_OFF_MOTOR_GAS_SW_4();
            break;

        case 5:
            TURN_OFF_MOTOR_GAS_SW_1();
            TURN_ON_MOTOR_GAS_SW_2();
            TURN_ON_MOTOR_GAS_SW_3();
            TURN_OFF_MOTOR_GAS_SW_4();
            break;

        case 6:
            TURN_OFF_MOTOR_GAS_SW_1();
            TURN_ON_MOTOR_GAS_SW_2();
            TURN_OFF_MOTOR_GAS_SW_3();
            TURN_OFF_MOTOR_GAS_SW_4();
            break;

        case 7:
            TURN_ON_MOTOR_GAS_SW_1();
            TURN_ON_MOTOR_GAS_SW_2();
            TURN_OFF_MOTOR_GAS_SW_3();
            TURN_OFF_MOTOR_GAS_SW_4();
            break;

        case 8:
            TURN_ON_MOTOR_GAS_SW_1();
            TURN_OFF_MOTOR_GAS_SW_2();
            TURN_OFF_MOTOR_GAS_SW_3();
            TURN_OFF_MOTOR_GAS_SW_4();
            break;

        default:
            TURN_OFF_MOTOR_GAS_SW_1();
            TURN_OFF_MOTOR_GAS_SW_2();
            TURN_ON_MOTOR_GAS_SW_3();
            TURN_ON_MOTOR_GAS_SW_4();
            break;
    }
}


/// @brief      Gas Switch �ʱ�ȭ ���� ���� ��� ���� �Լ�
/// @details    Gas Switch �ʱ�ȭ ���� ���� ���� �����Ѵ�
/// @param      void
/// @return     void
void GasSwitchInit(void)
{
    // Set Mode
    gu8_GasSwitch_Mode |= GAS_SWITCH_MODE_INIT;
}


/// @brief      Gas Switch �ü� ���� ���� ��� ���� �Լ�
/// @details    Gas Switch �ü� ���� ���� ���� �����Ѵ�
/// @param      void
/// @return     void
void GasSwitchCold(void)
{
    // Set Mode
    gu8_GasSwitch_Mode |= GAS_SWITCH_MODE_COLD;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_ICE;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_HOTGAS;
}


/// @brief      Gas Switch ���� ���� ���� ��� ���� �Լ�
/// @details    Gas Switch ���� ���� ���� ���� �����Ѵ�
/// @param      void
/// @return     void
void GasSwitchIce(void)
{
    // Set Mode
    gu8_GasSwitch_Mode |= GAS_SWITCH_MODE_ICE;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_COLD;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_HOTGAS;
}


/// @brief      Gas Switch �ְ��� ���� ���� ��� ���� �Լ�
/// @details    Gas Switch �ְ��� ���� ���� ���� �����Ѵ�
/// @param      void
/// @return     void
void GasSwitchHotGas(void)
{
    // Set Mode
    gu8_GasSwitch_Mode |= GAS_SWITCH_MODE_HOTGAS;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_ICE;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_COLD;
}


#endif


// Hal Gas Switch Module **************************************************************************************

/// @brief      Hal Gas Switch Initilaize Module
/// @details    Gas Switch ���� ���� ���� ��� �ʱ�ȭ
/// @param      void
/// @return     void
void Hal_GasSwitch_Module_Initialize(void)
{
#if (MOTOR_GAS_SWITCH_USE == USE)
    Hal_GasSwitch_Initialize();
#endif
}


/// @brief      Hal Gas Switch Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� Gas Switch ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_GasSwitch_Module_1ms_Control(void)
{
#if (MOTOR_GAS_SWITCH_USE == USE)
    GasSwitchControlTimer();
    GasSwitch_StepMotorTimerInterrupt();
#endif
}


/// @brief      Hal Gas Switch Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� Gas Switch ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_GasSwitch_Module_Control(void)
{
#if (MOTOR_GAS_SWITCH_USE == USE)
    Hal_GasSwitchControl();
#endif
}
