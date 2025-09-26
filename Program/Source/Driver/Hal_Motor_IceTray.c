/// @file     Hal_Motor_IceTray.c
/// @date     2025/08/01
/// @author   Jaejin Ham
/// @brief    Ice Tray 모터 제어


#include "Global_Header.h"


#if (MOTOR_ICE_TRAY_USE == USE)

U8 gu8IceTrayControlTimer = 0;                          /// @brief  Ice Tray 제어 시간 카운터
U8 gu8IceTrayMotorControl_5ms = 0;                      /// @breif  Ice Tray 스탭모터 전원 제어 주기 5ms 타이머

U16 gu16_ice_tray_target = 0;                           /// @brief  Ice Tray 스탭모터 제어 목표 위치 정보
U16 gu16_ice_tray_current = 0;                          /// @brief  Ice Tray 스탭모터 제어 현재 위치 정보

U8 gu8_ice_tray_hold_start_step = 0;                    /// @brief  Ice Tray 스탭모터 시작 Hold 시간
U8 gu8_ice_tray_hold_stop_step = 0;                     /// @brief  Ice Tray 스탭모터 정지 Hold 시간
U8 gu8_ice_tray_current_step = 0;                       /// @brief  Ice Tray 스탭모터 제어 현재 위상 위치 정보
U8 gu8_ice_tray_prev_step = 0;                          /// @brief  Ice Tray 스탭모터 제어 이전 위상 위치 정보

const U8 gu8IceTrayPhaseTable[ICE_TRAY_STEP_NUM] = { 0x01, 0x02, 0x03, 0x04 };

U8 gu8Ice_tray_mode = 0;                                /// @brief  Ice Tray 제어 동작 모드

U8 gu8_ice_tray_init_step = 0;                          /// @brief  Ice Tray 초기화 동작 제어 Step
U8 gu8_ice_tray_up_step = 0;                            /// @brief  Ice Tray Up 동작 제어 Step
U8 gu8_ice_tray_down_step = 0;                          /// @brief  Ice Tray Down 동작 제어 Step

U8 F_IceTrayCW = 0;                                     /// @brief  Ice Tray CW 회전 Flag - 0(미동작), 1(동작)

U16 gu16IceTrayStepMotorPulse = 0;                      /// @brief  Ice Tray 동작 Pulse

U8 gu8_ice_tray_down_timer = 0;                         /// @brief  Ice Tray Down 동작 시간

SIceTrayStepmotor_T SIceTrayStepMotor;                  /// @brief  Ice Tray 제어 데이터 구조체

U8 bit_tray_valve_output = 0;                           /// @brief  Ice Tray 입수 밸브 동작 상태 - 0(미동작), 1(동작)
U8 bit_tray_in_error_temporary = 0;                     /// @brief  Ice Tray 입수 단수 에러 상태 - 0(에러 미발생), 1(에러 발생)
U16 gu16_tray_error_clear_timer = 0;                    /// @brief  Ice Tray 입수 단수 에러 해제 시간


/// @brief      Ice Tray 스탭모터 제어 관련 변수 초기화
/// @details    Ice Tray 스탭모터 제어 관련 변수들을 모두 초기화 한다
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
/// @details    Ice Tray 제어 시간 주기를 카운트한다
/// @param      void
/// @return     void
void IceTrayControlTimer(void)
{
    if (gu8IceTrayControlTimer < ICE_TRAY_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8IceTrayControlTimer++;
    }
}


/// @brief      Ice Tray 스탭모터 제어 진행 함수(@1ms interrupt)
/// @details    1ms Interrput 안에서 Ice Tray 스탭모터 제어 진행 동작을 한다
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


/// @brief      Ice Tray Control 함수
/// @details    Ice Tray 제어 동작을 한다
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


/// @brief      Ice Tray 초기화 동작 제어 함수
/// @details    Ice Tray 초기화 동작 제어를 한다
/// @param      void
/// @return     return : 초기화 동작 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      Ice Tray Up 동작 제어 함수
/// @details    Ice Tray Up 동작 제어를 한다
/// @param      void
/// @return     return : 동작 완료 여부 - 0(미완료), 1(완료)
U8 up_ice_tray(void)
{
    U8 mu8Return = 0;

    mu8Return = FALSE;

    switch (gu8_ice_tray_up_step)
    {
        case 0:
            gu8_ice_tray_hold_start_step = ICE_TRAY_HOLD_STEP;
            gu8_ice_tray_hold_stop_step = ICE_TRAY_HOLD_STEP;

            /*..hui [25-3-17오후 2:24:09] 최대 펄스만큼 위로 올림..*/
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


/// @brief      Ice Tray Down 동작 제어 함수
/// @details    Ice Tray Down 동작 제어를 한다
/// @param      void
/// @return     return : 동작 완료 여부 - 0(미완료), 1(완료)
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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
            /*..hui [25-3-17오후 2:24:27] 내릴때도 어디있든 최대 ㅂ펄스만큼 내린다..*/
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


/// @brief      Ice Tray 동작 스탭모터 제어 함수(@5ms)
/// @details    Ice Tray 동작 스탭모터 제어를 한다
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
        /*..hui [21-6-24오후 2:24:48] 이쪽이 유량 줄이기로..*/
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


/// @brief      Ice Tray 동작 스탭모터 HW 제어 함수
/// @details    Ice Tray 동작 스탭모터 HW Port 제어를 한다
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


/// @brief      Ice Tray 초기화 동작 설정 함수
/// @details    Ice Tray 초기화 동작을 위한 설정을 한다
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


/// @brief      Ice Tray Down 동작 설정 함수
/// @details    Ice Tray Down 동작을 위한 설정을 한다
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


/// @brief      Ice Tray Up 동작 설정 함수
/// @details    Ice Tray Up 동작을 위한 설정을 한다
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


/// @brief      Ice Tray 정지 동작 설정 함수
/// @details    Ice Tray 정지 동작을 위한 설정을 한다
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


/// @brief      Ice Tray 이동 모드 설정 함수
/// @details    Ice Tray 이동 모드를 설정한다
/// @param      mu8Mode : Ice Tray 이동 모드 - 0(Stop), 1(Init), 2(Up), 3(Down)
/// @return     void
void Set_IceTrayMode(U8 mu8Mode)
{
    gu8Ice_tray_mode = mu8Mode;
}


/// @brief      Ice Tray 이동 모드 확인 함수
/// @details    Ice Tray 이동 모드 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Ice Tray 이동 모드 - 0(Stop), 1(Init), 2(Up), 3(Down)
U8 Get_IceTrayMode(void)
{
    return  gu8Ice_tray_mode;
}


/// @brief      Ice Tray 밸드 입수 동작 상태 설정 함수
/// @details    Ice Tray 밸브 입수 동작 상태를 설정한다
/// @param      mu8Status : Ice Tray 입수 밸브 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      Ice Tray 밸드 입수 동작 상태 확인 함수
/// @details    Ice Tray 밸브 입수 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Ice Tray 입수 밸브 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      Ice Tray 일반 동작 함수
/// @details    Ice Tray 일반 동작을 수행한다
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


/// @brief      Ice Tray 입수 막힘 에러 체크 동작 함수
/// @details    Ice Tray 입수 막힘 에러 체크 동작을 한다
/// @param      void
/// @return     void
void check_error_blocked_trayin(void)
{
    U8 mu8IceStepBuf = 0;

    mu8IceStepBuf = Get_IceStep();

    if (mu8IceStepBuf == STATE_20_WATER_IN_ICE_TRAY)
    {
        if (bit_tray_valve_output == SET)
        {   // 트레이 입수 밸브 동작 중일 때
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
//                /*..hui [20-6-22오전 10:21:02] 추출할때 카운트 초기화하도록 수정.. 반복적인 추출일때 해제동작이 제대로 안이루어질수있음..*/
//                /*..hui [20-6-22오전 10:21:06] 이승헌과장..*/
//                u16_tray_flow_check_timer = 0;
//            }
//            else{}
        }
    }
    else
    {   // 트레이 입수 밸브 동작 중이 아닐 때
//        u16_tray_flow_check_timer = 0;
    }


    if (bit_tray_in_error_temporary == SET)
    {   // 트레이 입수 단수 발생시
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


/// @brief      Ice Tray 입수 단수 여부 확인 함수
/// @details    Ice Tray 입수 단수 여부를 확인하여 반환한다
/// @param      void
/// @return     return : Ice Tray 입수 단수 여부 - 0(단수 미발생), 1(단수 발생)
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
/// @details    Ice Tray 제어 관련 변수 모두 초기화
/// @param      void
/// @return     void
void Hal_IceTray_Module_Initialize(void)
{
#if (MOTOR_ICE_TRAY_USE == USE)
    Hal_IceTray_Initialize();
#endif
}


/// @brief      Hal Ice Tray Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 Ice Tray 제어 타이머를 카운트 한다
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
/// @details    Basic Loop 안 While 문 안에서 Ice Tray 제어를 한다
/// @param      void
/// @return     void
void Hal_IceTray_Module_Control(void)
{
#if (MOTOR_ICE_TRAY_USE == USE)
    Hal_IceTrayControl();
#endif
}






