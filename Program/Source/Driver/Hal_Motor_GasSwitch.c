/// @file     Hal_Motor_GasSwitch.c
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Gas Switch 스탭모터 제어

#include "Global_Header.h"


#if (MOTOR_GAS_SWITCH_USE == USE)

U8 gu8GasSwitchControlTimer = 0;                /// @brief  Gas Switch 제어 시간 카운터

U8 F_Comp_Output_Old = 0;                       /// @brief  Comp 이전 동작 상태 - 0(미동작), 1(동작)
U8 F_Comp_Output = 0;                           /// @brief  Comp 현재 동작 상태 - 0(미동작), 1(동작)
U8 F_Comp_Restart_Delay_Status = 0;             /// @brief  Comp 기동 전 지연 시간 상태 여부 - 0(냉각기 가동중), 1(가동 전 지연 동작중)

U8 gu8_GasSwitch_Mode = 0;                      /// @brief  Gas Switch 제어 동작 모드
U8 gu8_GasSwitch_Status = 0;                    /// @brief  Gas Switch의 현제 제어 상태 - 0(냉수), 1(얼음), 2(핫가스)

U8 gu8_GasSwitch_InitStep = 0;                  /// @brief  Gas Switch 초기화 동작 제어 Step
U8 gu8_GasSwitch_ColdStep = 0;                  /// @brief  Gas Switch 냉수 동작 제어 Step
U8 gu8_GasSwitch_IceStep = 0;                   /// @brief  Gas Switch 얼음 동작 제어 Step
U8 gu8_GasSwitch_HotGasStep = 0;                /// @brief  Gas Switch 핫가스 동작 제어 Step

U8 gu8_GasSwitch_PowerOnTime = 0;               /// @brief  Gas Switch 전원 ON 시간

S16 gs16_sm_info_target = 0;                    /// @brief  Gas Switch 스탭모터 제어 목표 위치 정보
S16 gs16_sm_info_current = 0;                   /// @brief  Gas Switch 스탭모터 제어 현재 위치 정보
S16 gs16_sm_info_move_step = 0;                 /// @brief  Gas Switch 스탭모터 제어 이동값 정보
S16 gs16_sm_info_hold_start_step = 0;           /// @brief  Gas Switch 스탭모터 시작 Hold 시간
S16 gs16_sm_info_hold_stop_step = 0;            /// @brief  Gas Switch 스탭모터 정지 Hold 시간
S8 gs8_sm_info_current_mode = 0;                /// @brief  Gas Switch 스탭모터 제어 현재 위상 위치 정보
S8 gs8_sm_info_prev_mode = 0;                   /// @brief  Gas Switch 스탭모터 제어 이전 위상 위치 정보

const U8 gu8PhaseTable[PHASE_STEP_NUM] = { 8, 7, 6, 5, 4, 3, 2, 1 };

U8 F_GasSwitch_Initial = 0;                     /// @brief  Gas Switch 스탭모터 제어 초기화 상태 - 0(미동작), 1(동작)

U8 gu8MotorControl_30ms = 0;                    /// @breif  Gas Switch 스탭모터 전원 제어 주기 30ms 타이머


/// @brief      냉각기 동작 상태 설정 함수
/// @details    냉각기가 동작 중인지 상태를 설정한다
/// @param      mu8Status : 냉각기 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      냉각기 동작 상태 확인 함수
/// @details    냉각기가 동작 중인지 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 냉각기 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      냉각기 가동 전 지연 동작 상태 설정 함수
/// @details    냉각기 가동 전 동작 중인지 상태를 설정한다
/// @param      mu8Status : 냉각기 가동 전 지연 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      냉각기 가동 전 지연 동작 상태 확인 함수
/// @details    냉각기 가동 전 지연 동작 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 냉각기 가동 전 지연 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      Gas Switch 현재 제어 상태 확인 함수
/// @details    Gas Switch 현재 제어 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Gas Switch의 현재 제어 상태 - 0(냉수), 1(얼음), 2(핫가스)
U8 Get_GasSwitchStatus(void)
{
    return  gu8_GasSwitch_Status;
}


/// @brief      Gas Switch 제어 관련 변수 초기화
/// @details    Gas Switch 제어와 관련된 변수들을 초기화시킨다
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
/// @details    Gas Switch 제어 시간 주기를 카운트한다
/// @param      void
/// @return     void
void GasSwitchControlTimer(void)
{
    if (gu8GasSwitchControlTimer < GAS_SWITCH_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8GasSwitchControlTimer++;
    }
}


/// @brief      Gas Switch 제어 함수
/// @details    Gas Switch 제어 동작을 한다
/// @param      void
/// @return     void
void Hal_GasSwitchControl(void)
{
    U8 mu8Ret = FALSE;

    if (gu8GasSwitchControlTimer >= GAS_SWITCH_CONTROL_TIME_PERIOD)
    {   // 100ms 마다 처리
        gu8GasSwitchControlTimer = 0;

        ProcessGasSwitch();

        /* Init */
        // 초기화를 하고 실제 스텝 위치는 냉수측이지만,
        // 위치 설정은 하지 않는다.
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


/// @brief      Gas Switch 초기화 제어 함수
/// @details    Gas Switch 초기화 제어 동작을 한다
/// @param      void
/// @return     return : 초기와 동작 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      Gas Switch 냉수 제어 함수
/// @details    Gas Switch 냉수 제어 동작을 한다
/// @param      void
/// @return     return : 동작 제어 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      Gas Switch 얼음 제어 함수
/// @details    Gas Switch 얼음 제어 동작을 한다
/// @param      void
/// @return     return : 제어 동작 완료 여부 - 0(미완료), 1(완료)
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

        // 현재 위치이면 이동 없이 정상 종료
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


/// @brief      Gas Switch 핫가스 제어 함수
/// @details    Gas Switch 핫가스 제어 동작을 한다
/// @param      void
/// @return     return : 동작 완료 여부 - 0(미완료), 1(완료)
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

        // 현재 위치이면 이동 없이 정상 종료
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


/// @brief      Gas Switch 제어 진행 함수
/// @details    Gas Switch 제어 진행 동작을 한다
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
            F_GasSwitch_Initial = SET;  // 원점 초기화 설정
        }
        else
        {

        }
    }
    else
    {

    }

    // 압축기 OFF 후, 4분 후에 원점 초기화 동작
    mu8CompReactDelayStatusBuf = Get_CompReactDelayStatus();
    mu16CompOffDelayBuf = Get_CompOnDelayTime();

    if (F_GasSwitch_Initial == SET)
    {
        if (mu8CompReactDelayStatusBuf == TRUE)
        {   // 냉각 재기동 지연 동작 중인 경우
            if (mu16CompOffDelayBuf >= (DELAY_TIME_BEFORE_COMP_ON - 30) )
            {   // Comp 지연 시간이 30초 남았을 때
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


/// @brief      Gas Switch 스탭모터 제어 진행 함수(@1ms interrupt)
/// @details    1ms Interrput 안에서 Gas Switch 스탭모터 제어 진행 동작을 한다
/// @param      void
/// @return     void
void GasSwitch_StepMotorTimerInterrupt(void)
{
    if (++gu8MotorControl_30ms >= 30)
    {   // 30ms 마다
        gu8MotorControl_30ms = 0;

        // 냉매전환밸브
        HAL_ControlStepMotor();   // Gas Switch
    }
}


/// @brief      Gas Switch 스탭모터 제어 함수(@30ms)
/// @details    100us Interrput 안에서 30ms 마다 Gas Switch 스탭모터 제어 동작을 한다
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


/// @brief      Gas Switch 스탭모터 단계별 제어 함수
/// @details    Gas Switch 스탭모터 제어를 단계적으로 동작을 한다
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


/// @brief      Gas Switch 냉매 유로 전환 스탭모터 단계별 전원 제어 함수
/// @details    Gas Switch 냉매 유로 전환 스탭모터 제어를 단계적으로 전원 제어 동작을 한다
/// @param      mu8Mode : 스탭모터 위상 전원 제어
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


/// @brief      Gas Switch 초기화 동작 상태 모드 설정 함수
/// @details    Gas Switch 초기화 동작 상태 모드로 설정한다
/// @param      void
/// @return     void
void GasSwitchInit(void)
{
    // Set Mode
    gu8_GasSwitch_Mode |= GAS_SWITCH_MODE_INIT;
}


/// @brief      Gas Switch 냉수 동작 상태 모드 설정 함수
/// @details    Gas Switch 냉수 동작 상태 모드로 설정한다
/// @param      void
/// @return     void
void GasSwitchCold(void)
{
    // Set Mode
    gu8_GasSwitch_Mode |= GAS_SWITCH_MODE_COLD;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_ICE;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_HOTGAS;
}


/// @brief      Gas Switch 얼음 동작 상태 모드 설정 함수
/// @details    Gas Switch 얼음 동작 상태 모드로 설정한다
/// @param      void
/// @return     void
void GasSwitchIce(void)
{
    // Set Mode
    gu8_GasSwitch_Mode |= GAS_SWITCH_MODE_ICE;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_COLD;
    gu8_GasSwitch_Mode &= ~GAS_SWITCH_MODE_HOTGAS;
}


/// @brief      Gas Switch 핫가스 동작 상태 모드 설정 함수
/// @details    Gas Switch 핫가스 동작 상태 모드로 설정한다
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
/// @details    Gas Switch 제어 관련 변수 모두 초기화
/// @param      void
/// @return     void
void Hal_GasSwitch_Module_Initialize(void)
{
#if (MOTOR_GAS_SWITCH_USE == USE)
    Hal_GasSwitch_Initialize();
#endif
}


/// @brief      Hal Gas Switch Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 Gas Switch 제어 타이머를 카운트 한다
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
/// @details    Basic Loop 안 While 문 안에서 Gas Switch 제어를 한다
/// @param      void
/// @return     void
void Hal_GasSwitch_Module_Control(void)
{
#if (MOTOR_GAS_SWITCH_USE == USE)
    Hal_GasSwitchControl();
#endif
}
