/// @file     Hal_AC_Motor_IceMaking.c
/// @date     2025/07/30
/// @author   Jaejin Ham
/// @brief    얼음 기능 관련 AC Motor Control file


#include "Global_Header.h"

#if (AC_MOTOR_ICE_MAKING_USE == USE)

U8 gu8AC_Motor_IceMakingControlTimer = 0;           /// @brief  제빙 관련 AC 모터 제어 주기 타이머

U8 gu8Tray_CW_Interval_Timer = 0;                   /// @brief  Tray CW 회전 동작 시간
U8 F_Tray_CW_delay_finish = 0;                      /// @brief  Tray CW 회전 동작 지연 완료 Flag
U8 gu8Tray_CCW_Interval_Timer = 0;                  /// @brief  Tray CCW 회전 동작 시간
U8 F_Tray_CCW_delay_finish = 0;                     /// @brief  Tray CCW 회전 동작 지연 완료 Flag

U16 gu16Ice_Tray_Up_Reset_Delay_Tmr = 0;            /// @brief  Tray Up 방향 초기화 동작 지연 시간
U16 gu16Ice_Tray_Down_Reset_Delay_Tmr = 0;          /// @brief  Tray Down 방향 초기화 동작 지연 시간
U8 F_Ice_Tray_Up_Move_Reset = 0;                    /// @brief  트레이 Up 동작 초기화 상태 - 0(미동작), 1(동작)
U8 F_Ice_Tray_Down_Move_Reset = 0;                  /// @brief  트레이 Down 동작 초기화 상태 - 0(미동작), 1(동작)

U8 F_TrayMotorUP = 0;                               /// @brief  트레이 모터 Up 이동 상태 Flag - 0(미이동), 1(이동)
U8 F_TrayMotorPreUP = 0;                            /// @brief  트레이 모터 이전 Up 이동 상태 Flag - 0(미이동), 1(이동)
U8 F_TrayMotorDOWN = 0;                             /// @brief  트레이 모터 Down 이동 상태 Flag - 0(미이동), 1(이동)
U8 F_TrayMotorPreDOWN = 0;                          /// @brief  트레이 모터 이전 Down 이동 상태 Flag - 0(미이동), 1(이동)

U8 F_Over_ice_check_enable = 0;                     /// @brief  과제빙 체크 가능 Flag - 0(체크안함), 1(체크진행)
U16 gu16Over_ice_melt_timer = 0;                    /// @brief  과제빙 녹이는 동작 시간
U8 gu8Over_ice_melt_proc = 0;                       /// @brief  과제빙 녹이는 동작 제어 단계
U8 gu8Over_ice_melt_operation_count = 0;            /// @brief  과제빙 녹이는 동작 제어 반복 횟수

U8 F_Over_Ice_Heater_State = 0;                     /// @brief  탈빙히터 과동작 상태 Flag - <미사용>

U8 F_ice_out_back_state = 0;                        /// @brief  얼음 추출 후 피더 스크류 역동작 상태 Flag - 0(미동작), 1(동작)
U8 gu8ice_out_reverse_timer = 0;                    /// @brief  얼음 추출 후 피더 스크류 역동작 제어 시간

U8 gu8Ice_out_operation_timer = 0;                  /// @brief  얼음 추출 동작 제어 시간

U8 F_ice_stuck_back_state = 0;                      /// @brief  얼음 걸림 해제 동작 상태 여부 - 0(미동작), 1(동작)
U8 gu8Ice_stuck_reverse_timer = 0;                  /// @brief  얼음 걸림 해제 역회전 동작 시간

U8 gu8Feeder_ice_release_timer = 0;                 /// @brief  얼음 feeder 동작 제어 시간

U8 gu8IceOutCCWInterval = 0;                        /// @breif  토출 모터 대기 시간

U16 gu16ErrIceOut = 0;                              /// @brief  얼음 토출 에러 체크 시간


/// @brief      제빙 관련 AC 모터들의 변수 초기화 함수
/// @details    제빙 관련 AC 모터들의 제어와 관련된 변수들을 초기화 시킨다
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


/// @brief      제빙 관련 AC 모터들의 Control Timer(@1ms interrupt)
/// @details    제빙 관련 AC 모터들의 제어를 위한 타이밍을 카운트 한다
/// @param      void
/// @return     void
void AC_Motor_IceMaking_ControlTimer(void)
{
    if (gu8AC_Motor_IceMakingControlTimer < AC_MOTOR_ICE_MAKING_CONTROL_TIME_PERIOD)
    {
        gu8AC_Motor_IceMakingControlTimer++;
    }
}


/// @brief      제빙 관련 AC Motor 제어 함수(@100ms)
/// @details    제빙 관련 AC 모터 제어를 한다
/// @param      void
/// @return     void
void Output_AC_Motor_IceMaking(void)
{
    if (gu8AC_Motor_IceMakingControlTimer >= AC_MOTOR_ICE_MAKING_CONTROL_TIME_PERIOD)
    {   // 100ms마다 수행
        gu8AC_Motor_IceMakingControlTimer = 0;

        Output_Ice_Feeder_Motor();      // Feeder 모터 제어
        Output_Ice_Tray_Motor();        // Tray 모터 제어
    }
}


/// @brief      얼음 토출 모터 대기 시간 설정 함수(얼음 추출키 입력되어 얼음 추출 시작시 값 세팅)
/// @details    얼음 토출 모터 대기 시간을 얼음 추출 버튼 입력되어 얼음 추출 시작시 값을 설정한다
/// @param      mu8Data : 얼음 추출 시작시 얼음 토출 모터 대기 시간 설정값(100ms단위)
/// @return     void
void Set_IceOutIntervalTime(U8 mu8Data)
{
    gu8IceOutCCWInterval = mu8Data;
}


/// @brief      얼음 토출 모터 대기 시간 확인 함수
/// @details    얼음 추출 버튼 입력되어 얼음 추출 시작시 값이 설정된 얼음 토출 모터 대기 시간을 확인하여 반환한다
/// @param      void
/// @return     return : 얼음 추출 시작시 얼음 토출 모터 대기 시간 설정값(100ms단위)
U8 Get_IceOutIntervalTime(void)
{
    return  gu8IceOutCCWInterval;
}


/// @brief      얼음 추출 후 피더 스크류 역동작 상태 설정 함수
/// @details    얼음 추출 후 피더 스크류 역동작을 진행할 것인지 설정한다
/// @param      mu8Status : 얼음 추출 후 피더 스크류 역동작 상태 Flag - 0(미동작), 1(동작)
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


/// @brief      얼음 추출 후 피더 스크류 역동작 상태 확인 함수
/// @details    얼음 추출 후 피더 스크류 역동작을 진행하는지 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 얼음 추출 후 피더 스크류 역동작 상태 Flag - 0(미동작), 1(동작)
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


/// @brief      얼음 걸림 해제 동작 진행 여부 설정 함수
/// @details    얼음 걸림 해제 동작 진행 여부를 설정한다
/// @param      mu8Status : 얼음 걸림 해제 동작 상태 여부 - 0(미동작), 1(동작)
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


/// @brief      얼음 걸림 해제 동작 진행 여부 확인 함수
/// @details    얼음 걸림 해제 동작 진행 여부를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 얼음 걸림 해제 동작 상태 여부 - 0(미동작), 1(동작)
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


/// @brief      얼음 관련 Feeder Motor 제어 함수
/// @details    얼음 관련 Feeder 모터 제어를 한다
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
        gu8IceOutCCWInterval--;    // 토출모터 대기3->2초
    }

    if (gu16ErrIceOut > 0)
    {
        gu16ErrIceOut--;                // 얼음토출 모터에러 120초
    }

/*..hui [20-4-17오후 3:04:49] 피더 역회전 넣을때 ice_extraction_finish에 F_IR 지워줘야함..꼭~~~..*/
/*..hui [20-4-17오후 3:04:59] 만빙 감지를 2초 역회전 완료 후 하도록 하기 위해..*/
    //===========================================// 얼음토출 모터 동작
    mu8IceOutCCWStatusBuf = Get_IceOutCCWStatus();
    mu8IceOutContinueBuf = Get_IceOutContinueStatus();

    if (mu8IceOutCCWStatusBuf == TRUE)                      // 토출회전(역회전)
    {
        /*..hui [25-3-6오후 5:35:50] 이동..*/
        if (gu8IceOutCCWInterval > 0)
        {
            /*..hui [25-3-6오후 5:38:06] 이것도 이동..*/
            gu16ErrIceOut = 1200;              //에러대기 120초
            gu8Feeder_ice_release_timer = 0;
        }
        else{}

        /*..hui [25-3-6오후 5:36:54] 만빙이 아닐때는 연속 동작..*/
        /*..hui [25-3-9오후 7:18:57] 만빙이 아닐때는 3사이클 제어후 연속동작..*/
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

        /*..hui [20-4-14오후 12:45:16] 추출 2분 이상 지속시 강제 정지..*/
        mu8IceOutCCW_PortStatus = GET_STATUS_MOTOR_ICE_OUT_CCW();

        if (mu8IceOutCCW_PortStatus == SET)
        {
            if (gu16ErrIceOut == 0)
            {
                TURN_OFF_MOTOR_ICE_OUT_CCW();
                Set_IceOutCCWStatus(CLEAR);
			    /*..sean [25-06-30] 아예 추출을 종료시키도록 변경..*/
                Set_IceOutStatus(CLEAR);

			    // 에러음 출력 추가 필요
            }
        }
        else
        {

        }

        if (mu8IceOutCCW_PortStatus == SET)
        {
            gu8Ice_out_operation_timer++;

            //if( gu8Ice_out_operation_timer >= 60 )
            /*..sean [25-07-07] 3초 추출시, 3초 휴지 후 역회전으로 변경..*/
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

        /*..hui [20-4-14오후 1:09:53] 추출시 역회전 정지..*/
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

        /*..hui [20-4-14오후 12:38:42] 피더스크류가 추출 방향으로 동작을 했으면..*/
        if (mu8IceOutBackStatusBuf == TRUE)
        {
            TURN_OFF_MOTOR_ICE_OUT_CCW();

            gu8ice_out_reverse_timer++;

            /*..hui [20-4-14오후 12:38:59] 3초동안 정/역 둘다 OFF 후 대기..*/
            /*..hui [20-4-14오후 12:39:18] 3초 후 역회전 모터 가동 시작..*/
            /*..hui [20-4-22오전 10:29:26] 만빙상태에서 6초이상 추출했으면 6초후 ..*/
            /*..sean [25-07-07] 만빙상태에서 3초이상 추출했으면 3초후 ..*/
            if (gu8ice_out_reverse_timer >= 40)
            {
                TURN_ON_MOTOR_ICE_OUT_CW();
            }
            else
            {

            }

            /*..hui [20-4-14오후 12:40:01] 2초 동안 가동 후 역회전 정지..*/
            if (gu8ice_out_reverse_timer >= 55)
            {
                TURN_OFF_MOTOR_ICE_OUT_CW();
                Set_IceOutBackStatus(CLEAR);
                gu8ice_out_reverse_timer = 0;
                /*..hui [20-4-22오전 10:35:40] 역회전 완료 후 만빙감지..*/
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

            /*..hui [25-3-17오후 4:16:14] 얼음 걸림 해제 동작 2초..*/
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


/// @brief      얼음 관련 Tray Motor 제어 함수
/// @details    얼음 관련 Tray 모터 제어를 한다
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

    /*..hui [18-2-9오후 1:09:49] CW CCW 방향 변경 시 5초 대기 후 전환..*/
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
    {   // E61 에러 감지 상태면 확정 검사는 에러 체크 Lib에서 자체 동작
        /*..hui [18-2-13오후 3:16:13] move 에러 확인 중 동시감지 에러 체크로 전환시 모두 클리어..*/
        /*..hui [18-2-13오후 3:16:29] 다시 move조건이되면 처음부터 시작하도록....*/
        reset_micro_sw_move_err_check_state();
    }
    else
    {   // E62, E63 에러 체크는 에러 체크 Lib에서 자체 동작
        Set_MicroSW_AllDetectErrorCheckStep(0);
    }

    check_ice_system_ok();

    if ( (mu8E61_Buf == FALSE) &&
         (mu8E62_Buf == FALSE) &&
         (mu8E63_Buf == FALSE) )
    {
        /*..hui [25-3-17오후 3:16:46] 아이스트레이 모터 출력..*/
        /*..hui [25-3-17오후 3:17:56] 에러 아닐때만 출력되도록..*/
        control_ice_tray();
    }
    else
    {

    }
}


/// @brief      마이크로 스위치 이동 에러 체크 관련 초기화 함수
/// @details    마이크로 스위치 이동 에러 체크 관련 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void reset_micro_sw_move_err_check_state(void)
{
    F_Ice_Tray_Up_Move_Reset = 0;
    F_Ice_Tray_Down_Move_Reset = 0;
    Set_IceSafetyRoutine(CLEAR);
    Clear_E62_E63_CheckTimer();
}


/// @brief      마이크로 스위치 이동 에러 체크 관련 초기화 함수
/// @details    마이크로 스위치 이동 에러 체크 관련 변수들을 초기화 시킨다
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


/// @brief      제빙 트레이 Up 동작 초기화 함수
/// @details    제빙 트레이 Up 동작 초기화 동작을 한다
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
        /*..hui [25-3-17오후 3:59:26] 첫 5회는 1분으로..*/
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
                /*..hui [25-3-17오후 4:20:05] 트레이 내리고 다시 시작..*/
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
            /*..hui [18-2-13오후 3:24:13] 10분 대기중에 정상 복귀시 하던거 중지....*/
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


/// @brief      제빙 트레이 Down 동작 초기화 함수
/// @details    제빙 트레이 Down 동작 초기화 동작을 한다
/// @param      void
/// @return     void
void ice_make_system_down_move_reset(void)
{
    over_ice_making_check();
}


/// @brief      과제빙 에러 체크 함수
/// @details    과제빙 에러 체크 동작을 한다
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


/// @brief      과제빙 얼음 녹이는 동작 함수
/// @details    과제빙 얼음 녹이는 동작을 한다
/// @param      void
/// @return     void
void over_ice_making_melt_proc(void)
{
    U8 mu8IceTrayPositionBuf = 0;

    mu8IceTrayPositionBuf = Get_IceTrayPosition();

    /*..hui [18-2-6오후 9:42:36] 과제빙된거 녹이는중에 언제든 트레이 정상 복귀되면 하던거 중지..*/
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
            /*..hui [18-2-6오후 9:16:17] 먼저 10초동안 냉수 급수밸브 ON한다..*/
            gu16Over_ice_melt_timer++;

            /*if(gu16Over_ice_melt_timer >= 100)*/
            /*..hui [23-9-22오전 11:26:29] 15초로 늘림.. 아이콘아이스 사양..*/
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
            /*..hui [18-2-6오후 9:21:25] 냉수 입수하고 20초동안 탈빙히터를 가동한당..*/
            gu16Over_ice_melt_timer++;

            /*..hui [20-4-21오후 7:15:57] 안전을 위해 10초로 변경..*/
            /*..hui [20-4-21오후 7:22:24] AIS30은 탈빙시 초기 OFF 부터 시작하므로 다시 15초로....*/
            /*if(gu16Over_ice_melt_timer >= 150)*/
            /*if(gu16Over_ice_melt_timer >= 6000)*/
            /*..hui [23-9-22오전 11:26:49] 탈빙히터 동작 삭제.. 20분동안 대기..*/
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
            /*..hui [18-2-6오후 9:23:11] 5초동안 냉수 급수밸브 ON..*/
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
            /*..hui [18-2-6오후 9:23:58] 10분동안 대기..*/
            gu16Over_ice_melt_timer++;

            if (gu16Over_ice_melt_timer >= 6000)
            {
                /*..hui [18-2-6오후 9:25:47] 10분동안 대기후에 다시 급수밸브 5초 ON한다..*/
                gu16Over_ice_melt_timer = 0;
                gu8Over_ice_melt_proc = 3;

                /*..hui [18-2-6오후 9:29:04] 총 9번 대략 90분동안 수행..*/
                gu8Over_ice_melt_operation_count++;

                /*..hui [18-2-6오후 9:30:30] 90분 동안 수행 후 CCW 동작 한번 더 수행..*/
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


/// @brief      Tray Down 동작 설정 함수
/// @details    Tray Down 동작 설정을 한다
/// @param      void
/// @return     void
void down_tray_motor(void)
{
    F_TrayMotorUP = CLEAR;
    F_TrayMotorPreUP = CLEAR;
    F_TrayMotorDOWN = SET;
    F_TrayMotorPreDOWN = SET;
}


/// @brief      Tray Up 동작 설정 함수
/// @details    Tray Up 동작 설정을 한다
/// @param      void
/// @return     void
void up_tray_motor(void)
{
    F_TrayMotorUP = SET;
    F_TrayMotorPreUP = SET;
    F_TrayMotorDOWN = CLEAR;
    F_TrayMotorPreDOWN = CLEAR;
}


/// @brief      Tray CCW 동작 지연 완료 설정 함수
/// @details    Tray CCW 동작 지연 완료 여부를 설정한다
/// @param      mu8Data : Tray CCW 동작 지연 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      Tray CCW 동작 지연 완료 확인 함수
/// @details    Tray CCW 동작 지연 완료 여부를 확인하여 반환한다
/// @param      void
/// @return     return : Tray CCW 동작 지연 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      Tray CW 동작 지연 완료 설정 함수
/// @details    Tray CW 동작 지연 완료 여부를 설정한다
/// @param      mu8Data : Tray CW 동작 지연 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      Tray CW 동작 지연 완료 확인 함수
/// @details    Tray CW 동작 지연 완료 여부를 확인하여 반환한다
/// @param      void
/// @return     return : Tray CW 동작 지연 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      Tray Up 초기화 동작 상태 설정 함수
/// @details    Tray Up 초기화 동작 상태를 설정한다
/// @param      mu8Data : Tray Up 초기화 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Up 초기화 동작 상태 확인 함수
/// @details    Tray Up 초기화 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Tray Up 초기화 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Down 초기화 동작 상태 설정 함수
/// @details    Tray Down 초기화 동작 상태를 설정한다
/// @param      mu8Data : Tray Down 초기화 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Down 초기화 동작 상태 확인 함수
/// @details    Tray Down 초기화 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Tray Down 초기화 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Up 동작 상태 설정 함수
/// @details    Tray Up 동작 상태를 설정한다
/// @param      mu8Data : Tray Up 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Up 동작 상태 확인 함수
/// @details    Tray Up 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Tray Up 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Down 동작 상태 설정 함수
/// @details    Tray Down 동작 상태를 설정한다
/// @param      mu8Data : Tray Down 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Down 동작 상태 확인 함수
/// @details    Tray Down 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Tray Down 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Up 이전 동작 상태 설정 함수
/// @details    Tray Up 이전 동작 상태를 설정한다
/// @param      mu8Data : Tray Up 이전 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Up 이전 동작 상태 확인 함수
/// @details    Tray Up 이전 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Tray Up 이전 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Down 이전 동작 상태 설정 함수
/// @details    Tray Down 이전 동작 상태를 설정한다
/// @param      mu8Data : Tray Down 이전 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      Tray Down 이전 동작 상태 확인 함수
/// @details    Tray Down 이전 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : Tray Down 이전 동작 여부 - 0(미동작), 1(동작)
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
/// @details    제빙 관련 AC Motor 제어 관련 변수 모두 초기화
/// @param      void
/// @return     void
void Hal_AC_Motor_IceMaking_Module_Initialize(void)
{
#if (AC_MOTOR_ICE_MAKING_USE == USE)
    Hal_AC_Motor_IceMaking_Initialize();
#endif
}


/// @brief      Hal AC Motor Ice Making Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 제빙 관련 AC Motor 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_AC_Motor_IceMaking_Module_1ms_Control(void)
{
#if (AC_MOTOR_ICE_MAKING_USE == USE)
    AC_Motor_IceMaking_ControlTimer();                 // Swing Bar  제어 시간 Counter
#endif
}


/// @brief      Hal AC Motor Ice Making in While Loop
/// @details    Basic Loop 안 While 문 안에서 제빙 관련 AC Motor 제어를 한다
/// @param      void
/// @return     void
void Hal_AC_Motor_IceMaking_Module_Control(void)
{
#if (AC_MOTOR_ICE_MAKING_USE == USE)
    Output_AC_Motor_IceMaking();
#endif
}
