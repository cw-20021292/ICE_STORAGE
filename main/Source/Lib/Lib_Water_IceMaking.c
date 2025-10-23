/// @file     Lib_Water_IceMaking.c
/// @date     2025/08/04
/// @author   Jaejin Ham
/// @brief    제빙 동작 제어 file


#include "Global_Header.h"
#include "Lib_Water_IceMakeTimeTable.h"


#if (LIB_ICE_MAKING_USE == USE)

U8 gu8IceMakingControlTimer = 0;                    /// @brief  제빙 제어 주기 타이머

U8 F_IceOn = 0;                                     /// @brief  제빙 기능 ON/OFF 설정 상태 - 0(OFF), 1(ON)

U8 gu8InitStep = 0;                                 /// @brief  제빙 기능 초기화 동작 제어 Step

U8 gu8IceMakeDisableStatus = 0;                     /// @brief  제빙 동작 불가 상태 여부 - 0(제빙 가능), 1(제빙 불가능)

U8 F_IceInit = 0;                                   /// @brief  제빙 동작 초기화 동작 Flag - 0(미동작), 1(동작)

U8 F_re_ice_init_error = 0;                         /// @brief  제빙중에 에러 걸렸을때만 해제됐을때 더미탈빙 진행 Flag - 0(미동작), 1(동작)

U8 F_IceFull = 0;                                   /// @brief  만빙 상태 Flag - 0(만빙 아님), 1(만빙)
U8 F_IR = 0;                                        /// @brief  만빙 검사 상태 Flag - 0(미검사), 1(검사)
U8 gu8IRTime = 0;                                   /// @brief  만빙 검사 시간(100ms 단위)
U8 gu8IRCount = 0;                                  /// @brief  만빙 체크 횟수
U16 gu16IRInterval = 0;                             /// @brief  만빙 인식 주기(100ms 단위)
U16 gu16ADIceFull = 0;                              /// @brief  만빙 센서 감지 AD값

U8 gu8Ice_out_continue = 0;                         /// @brief  얼음 연속 추출 상태 - 0(미동작), 1(동작)

U8 F_IceOut = 0;                                    /// @brief  얼음 추출 상태 Flag - 0(미추출), 1(추출)

U8 F_Safety_Routine = 0;                            /// @brief  제빙 시스템 안전 제어 동작 상태 Flag - 0(미동작), 1(동작)

U8 gu8IceStep = 0;                                /// @brief  제빙 동작 제어 Step
//ICE_STEP gu8IceStep;

U8 F_IceVV = 0;                                     /// @brief  냉수 급수 밸브 동작 Flag - 0(미동작), 1(동작)

U16 gu16IceHeaterTime = 0;                          /// @brief  탈빙 핫가스(Heater) 동작 시간
U8 F_WaterInit = 0;                                 /// @brief  최초 정수 완료 Flag - 0(미완료), 1(완료)

U8 F_IceOutCCW = 0;                                 /// @brief  얼음 토출 회전 상태 Flag - 0(미동작), 1(동작)

U16 gu16IceMakeTime = 0;                            /// @brief  제빙 시간

U8 gu8_ice_tray_reovery_time = 0;                   /// @brief  Ice Tray 보완 동작 시간

U16 gu16_Ice_Tray_Fill_Hz = 0;                      /// @brief  Ice Tray 물채움 동작 제어 Hz(직수)

U16 gu16_cody_ice_make_time = 0;                    /// @brief  코디 모드 제빙 동작 시간

U16 gu16_preheat_time = 0;                          /// @brief  핫가스 밸브 이동 시간

U32 gu32_ice_init_cancel_timer = 0;                 /// @brief  제빙 초기화 동작 취소 시간(100ms 단위)

U8 bit_start_preheat = 0;                           /// @brief  예열 동작 상태 - 0(미동작), 1(동작)

U8 gu8Ice_system_ok = 0;                            /// @brief  제빙 시스템 이상 여부 - 0(이상), 1(정상)


/// @brief      제빙 동작 관련 Flag & 변수 초기화
/// @details    제빙 제어에 사용되는 변수들을 초기화 시킨다
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


/// @brief      제빙 동작 Control Timer(@1ms)
/// @details    제빙 동작 제어를 위한 제어 주기 타이머를 카운트 한다
/// @param      void
/// @return     void
void IceMakingControlTimer(void)
{
    if (gu8IceMakingControlTimer < HEAT_TANK_CONTROL_TIME_PERIOD)
    {
        gu8IceMakingControlTimer++;
    }
}


/// @brief      제빙 기능 설정 상태 설정 함수
/// @details    제빙 기능 설정 상태를 설정한다
/// @param      mu8Status : 제빙 기능 ON/OFF 설정 상태 - 0(OFF), 1(ON)
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


/// @brief      제빙 기능 설정 상태 확인 함수
/// @details    제빙 기능 설정 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 제빙 기능 ON/OFF 설정 상태 - 0(OFF), 1(ON)
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

/// @brief      예열 동작 상태 설정 함수
/// @details    예열 동작 상태 설정 상태를 설정한다
/// @param      mu8Status : 예열 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      예열 동작 설정 상태 확인 함수
/// @details    예열 동작 설정 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 예열 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      만빙 센서에 측정된 AD값 설정 함수
/// @details    만빙 센서에 측정된 AD값을 설정한다
/// @param      mu16AD_Data : 만빙 센서 측정된 AD값
/// @return     void
void Set_AD_IceFull(U16 mu16AD_Data)
{
    gu16ADIceFull = mu16AD_Data;
}


/// @brief      만빙 센서에 측정된 AD값 확인 함수
/// @details    만빙 센서에 측정된 AD값을 확인하여 반환한다
/// @param      void
/// @return     return : 만빙 센서 측정된 AD값
U16 Get_AD_IceFull(void)
{
    return  gu16ADIceFull;
}


/// @brief      만빙 체크 함수
/// @details    만빙 체크 동작을 한다
/// @param      void
/// @return     void
void check_ice_full(void)
{
    U8 mu8IR_PowerBuf = 0;

    if (gu8IRTime > 0)
    {
        gu8IRTime--;                             // 만빙검사 10초
    }
    else
    {

    }

    //==================================================================
    if (F_IR == SET)
    {
        mu8IR_PowerBuf = GET_STATUS_IR_POWER();

        if (mu8IR_PowerBuf == OFF)
        {   // IR 출력 없을경우
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


/// @brief      제빙 초기화 동작 함수
/// @details    제빙 초기화 동작을 한다
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
            {   // 탈빙위치 이동
                gu8InitStep = 1;
            }
            else
            {

            }

            if ( (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_ICE_MAKING) ||
                 (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_MOVING) )
            {   // 이동 중/제빙 위치 -> 탈빙위치
                down_tray_motor();
            }
            else
            {   // 제빙 -> 3분 더미 제빙

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
            {   // Comp 기동 전 OFF Delay 시간이 모두 경과한 경우
                /*..hui [19-7-25오후 1:36:36] 냉매 전환밸브 얼음 방향 이동..*/
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

                Set_BLDC_Comp_Hz(BLDC_65Hz);       // ** 제품에 장착된 Comp 동작을 코딩해 넣을 것 **
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
            Set_BLDC_Comp_Hz(BLDC_65Hz);        // ** 제품에 장착된 Comp 동작을 코딩해 넣을 것 **

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


/// @brief      제빙 동작 프로세스 함수
/// @details    제빙 동작 프로세스대로 제어한다
/// @param      void
/// @return     void
void Ice_Make_Process(void)
{
    U8 mu8ErrorAboutIceMaking = 0;
    U8 mu8E61_DetectBuf = 0;

    mu8ErrorAboutIceMaking = Get_ErrorAboutIceMaking();
    mu8E61_DetectBuf = Get_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61);

    if (gu8IceMakingControlTimer >= HEAT_TANK_CONTROL_TIME_PERIOD)
    {   // 100ms 마다
        gu8IceMakingControlTimer = 0;

        check_ice_full();       // 만빙 상태 체크

        /*..hui [17-12-19오후 7:07:33] 누수일때 제빙 및 컴프 정지 추가....*/
        /*..hui [17-12-19오후 7:07:54] 나중에 냉수 입수밸브 OFF시키는것도 추가해야함....*/
	    /*..sean [25-06-16]..cody 모드 진입시 초기화하도록 예외사항 추가*/
        if ( (mu8ErrorAboutIceMaking == TRUE) ||
             (F_Safety_Routine == SET) ||
             (mu8E61_DetectBuf == TRUE) )
        {
            gu8InitStep = 0;
            gu8IceStep = STATE_0_STANDBY;
        }
        else
        {
            //=======================================================//제빙유닛 초기화
            if (F_IceInit == SET)
            {
                ice_init_operation();
            }
            else
            {
                gu8InitStep = 0;

                //======================================================// 제빙 초기, 냉수, 정수 만수아니면 return

                //======================================================// 제빙종료시
                if(Bit1_Ice_Make_Go != SET)
                {
                    gu8IceStep = STATE_0_STANDBY;
                }
                else
                {
                    //=======================================================// 제빙 시퀸스
                    ice_make_operation();
                }
            }
        }
    }
}


/// @brief      제빙 동작 제어 함수
/// @details    제빙 동작을 제어한다
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

    mf32_AirTemp = Get_Temp(ADC_ID_TH_AIR);         // 외기 온도
    mf32_RoomTemp = Get_Temp(ADC_ID_TH_ROOM);       // 정수 온도


    mu8GasSwitch_StatusBuf = Get_GasSwitchStatus();
    mu8CompPowerStatus_Buf = GET_STATUS_BLDC_COMP();

    mu8TrayMotorUp_Buf = Get_TrayMotorUpStatus();
    mu8TrayMotorDown_Buf = Get_TrayMotorDownStatus();

    mu8IceTrayPosition_Buf = Get_IceTrayPosition();

    //=======================================================// 제빙 시퀸스
    switch(gu8IceStep)
    {
        //----------------------------------------------// 얼음받이 제빙회전
        case STATE_0_STANDBY :
            if ( (mu16CompOnDelayBuf == 0) ||
                 (mu8CompActBuf == TRUE) )
            {
                /*..hui [19-10-23오후 12:47:20] 얼음 추출중에는 트레이 올리지 않도록..*/
                /*..hui [19-10-23오후 12:47:34] 추출중에 얼음이 밀려서 트레이가 걸린다..*/
                /*..hui [19-10-23오후 12:47:46] 기구적으로 수정이 필요함..*/
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
            /*..hui [23-4-7오후 5:10:11] 냉매전환밸브 핫가스 이동..*/
            GasSwitchHotGas();
            gu8IceStep = STATE_6_CALC_PREHEAT_TIME;
            break;

        case STATE_6_CALC_PREHEAT_TIME :
            if (mu8GasSwitch_StatusBuf == GAS_SWITCH_HOTGAS)
            {
                /*..hui [23-4-7오후 5:10:03] 냉매전환밸브 핫가스 이동 완료 후, COMP 가동, 60HZ..*/
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

        //----------------------------------------------// 얼음받이 제빙회전
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

        //----------------------------------------------// 제빙회전후 물받이VV ON
        case STATE_11_WAIT_ROOM_WATER_FULL :
            //if( /*F_TrayMotorUP != SET && */gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING )
            //if( (SIceTrayStepMotor.state == STEP_STATE_DEACTIVE) && ( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING ) )
            /*if ( gu8IceTrayLEV == ICE_TRAY_POSITION_ICE_MAKING )*/
            if ( (mu8TrayMotorUp_Buf == FALSE) &&
                 (mu8IceTrayPosition_Buf == ICE_TRAY_POSITION_ICE_MAKING) )
            {
                Set_TrayMotorUpStatus(SET);

                /*..hui [23-4-7오후 5:52:57] 연속 제빙 조건일경우..*/
                /*..hui [23-8-14오후 1:56:47] 수위 확인은 트레이 입수쪽에서.. ..*/
                /*..hui [23-8-14오후 1:57:10] 여기까지 오는 사이에 물 다 빼버리면 COMP 켜진상태로 이쪽에서 대기하게됨..*/
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
                /*..hui [18-3-20오후 7:21:54] 정수탱크 만수위 채우는중에 트레이 쳐지면 제빙 정지됨..*/
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
            /*..hui [19-7-24오후 4:35:37] 냉매전환밸브 이동 완료 후..*/
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

        //----------------------------------------------// 제빙회전후 물받이VV ON
        case STATE_14_CHECK_ICE_TRAY_HZ :
            /*..hui [19-7-23오후 2:08:21] 트레이입수 수배관 변경에 따른 사양 변경(유량센서 후단)..*/
            /*..hui [19-7-23오후 2:08:26] 온수 사용중에는 트레이 입수 금지..*/
            /*if(u8WaterOutState == HOT_WATER_SELECT && F_WaterOut == SET)*/
            /*..hui [19-8-28오전 9:54:48] 온수 뿐만아니라 물 추출중에도 트레이 입수 대기..*/

            /*..hui [23-8-14오전 11:05:50] 그냥 넘어감.. 물 추출은 다음 단계에서 처리..*/
            gu16_Ice_Tray_Fill_Hz = C_ICE_TRAY_FILL_200CC;
            gu8IceStep = STATE_20_WATER_IN_ICE_TRAY;
            break;

        //-----------------------------------------------// 물받이VV Off
        case STATE_20_WATER_IN_ICE_TRAY :
            if (gu16_Ice_Tray_Fill_Hz <= 0)
            {
                gu8IceStep = STATE_21_ICE_SWITCH_MOVE;

//                gu16_wifi_tray_in_flow = 260;
            }
            else
            {
                /*..hui [23-7-21오후 4:53:40] 트레이 물 넣는중에 저수위 이하로 내려가면 제빙 취소..*/
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

        //-----------------------------------------------// 물받이VV Off
        case STATE_21_ICE_SWITCH_MOVE :
            GasSwitchIce();
            gu8IceStep = STATE_30_CALC_ICE_MAKING_TIME;
            break;

        //-----------------------------------------------// 제빙 시작
        case STATE_30_CALC_ICE_MAKING_TIME :

            /*..hui [19-7-24오후 4:35:37] 냉매전환밸브 이동 완료 후..*/
            if (mu8GasSwitch_StatusBuf == GAS_SWITCH_ICE)
            {
                if ( (mu16CompOnDelayBuf == 0) ||
                     (mu8CompActBuf == TRUE) )
                {
                    /*gu16IceMakeTime
                        = (U16)calc_ice_make_time( gu8_Amb_Front_Temperature_One_Degree, gu8_Room_Temperature_One_Degree);*/
                    gu16IceMakeTime = (U16)calc_ice_make_time(mf32_AirTemp, mf32_RoomTemp);

                    /*..hui [19-7-5오후 2:08:13] 100ms 카운트 조건 변경..*/
                    gu16IceMakeTime = (U16)(gu16IceMakeTime  * 10);

                    /*..hui [25-3-27오후 1:34:22] 얼음 대소 기능 추가..*/
//                    if (bit_ice_size == ICE_SIZE_SMALL)
//                    {
//                        gu16IceMakeTime = (U16)((F32)gu16IceMakeTime * 0.8f);
//                    }
//                    else{}

                    /*..hui [20-2-19오후 3:32:11] 코디 모드 강제 탈빙시 3분 이후에만 실행하기위해 추가..*/
//                    gu16_cody_ice_make_time  = gu16IceMakeTime;
//
//                    /*..hui [24-2-14오후 4:28:53] UV 남은시간 계산용..*/
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

        //-----------------------------------------------// 제빙완료후 모터회전
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

        //-----------------------------------------------// 모터회전완료
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
                F_IR = SET;              // 만빙센서 동작
                gu8IceStep = STATE_50_ICE_FULL_IR_CHECK;
            }
            else
            {

            }
            break;

        //-------------------------------------------------// SB 냉수 회전완료
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
            /*..hui [23-7-21오후 4:52:04] 추가..*/
            Set_SwingBar_Operation(CLEAR);
            break;

        default :
            gu8IceStep = STATE_0_STANDBY;
            gu8InitStep = 0;
            F_IceInit = SET;
            break;
      }

}


/// @brief      제빙 시간 판정 함수
/// @details    제빙 시간을 외기 온도와 정수 온도를 기준으로 판정하여 반환한다
/// @param      void
/// @return     return : 제빙 시간
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


/// @brief      핫가스 동작 함수
/// @details    핫가스 동작 제어를 한다
/// @param      void
/// @return     return : 핫가스 동작 완료 상태 - 0(미완료), 1(완료)
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


/// @brief      Ice Tray 복구 동작 제어
/// @details    Ice Tray 복구 동작을 제어한다
/// @param      void
/// @return     void
void recovery_ice_tray(void)
{
    U8 mu8IceTrayPosition_Buf = 0;

    mu8IceTrayPosition_Buf = Get_IceTrayPosition();

    /*..hui [18-3-22오전 9:39:58] 기구적인 문제로 제빙중 아에스트레이가 쳐지는 현상 발생할수있음..*/
    /*..hui [18-3-22오전 9:40:16] 아이스트레이가 쳐지면 10초에 한번씩 다시 올려준다..*/
    if (mu8IceTrayPosition_Buf != ICE_TRAY_POSITION_ICE_MAKING)
    {
        /*..hui [18-3-22오전 10:00:09] 트레이 재시도기능 중에는 동작안함..*/
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


/// @brief      핫가스 Noise 제거 동작
/// @details    핫가스 Noise 제거 동작 제어를 한다
/// @param      void
/// @return     void
void reduce_hot_gas_noise(void)
{
    U8 mu8_comp_rps = 0;

    mu8_comp_rps = get_hotgas_mode_comp_rps();
    Set_BLDC_Comp_Hz(mu8_comp_rps);
}


/// @brief      제빙 동작 Comp Hz 판단 함수
/// @details    제빙 동작시 제어하는 Comp의 Hz를 판단하여 반환한다
/// @param      void
/// @return     return : Comp 제어 Hz
U8 get_ice_mode_comp_rps(void)
{
    U8 mu8_return = 0;
    F32 mf32AirTemp = 0;

    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

	/*.. sean [25-02-04] gu8_Amb_Temperature_One_Degree으로 처리되어 front로 변경..*/
    if (mf32AirTemp <= 10.0f)
    {
        /*..hui [23-4-7오전 11:15:58] 10도 이하..*/
        mu8_return = BLDC_65Hz;
    }
    else if (mf32AirTemp <= 20.f)
    {
        /*..hui [23-4-7오전 11:16:02] 20도 이하..*/
        mu8_return = BLDC_65Hz;
    }
    else if (mf32AirTemp <= 25.0f)
    {
        /*..hui [23-4-7오전 11:16:06] 25도 이하..*/
        mu8_return = BLDC_65Hz;
    }
    else if (mf32AirTemp <= 30.0f)
    {
        /*..hui [23-4-7오전 11:16:10] 30도 이하..*/
        mu8_return = BLDC_65Hz;
    }
    else
    {
        /*..hui [23-4-7오전 11:16:14] 30도 초과..*/
        mu8_return = BLDC_60Hz;
    }

    return mu8_return;
}


/// @brief      핫가스 동작 Comp Hz 판단 함수
/// @details    핫가스 동작시 제어하는 Comp의 Hz를 판단하여 반환한다
/// @param      void
/// @return     return : Comp 제어 Hz
U8 get_hotgas_mode_comp_rps(void)
{
    U8 mu8_return = 0;
    F32 mf32AirTemp = 0;

    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

    if (mf32AirTemp < 9.0f)
    {
        /*..hui [23-4-7오후 1:17:04] 9도 미만..*/
        mu8_return = BLDC_50Hz;
    }
    else if (mf32AirTemp <= 13.0f)
    {
        /*..hui [23-4-7오후 1:23:42] 9도 ~ 13도..*/
        mu8_return = BLDC_50Hz;
    }
    else if (mf32AirTemp <= 19.0f)
    {
        /*..hui [23-4-7오후 1:23:55] 14도 ~ 19도..*/
        mu8_return = BLDC_50Hz;
    }
    else if (mf32AirTemp <= 24.0f)
    {
        /*..hui [23-4-7오후 1:24:08] 20도 ~ 24도..*/
        mu8_return = BLDC_47Hz;
    }
    else if (mf32AirTemp <= 29.0f)
    {
        /*..hui [23-4-7오후 1:24:18] 25도 ~ 29도..*/
        mu8_return = BLDC_47Hz;
    }
    else
    {
        /*..hui [23-4-7오후 1:24:23] 30도 이상..*/
        mu8_return = BLDC_43Hz;
    }

    return mu8_return;
}


/// @brief      예열 동작 Comp Hz 판단 함수
/// @details    예열 동작시 제어하는 Comp의 Hz를 판단하여 반환한다
/// @param      void
/// @return     return : Comp 제어 Hz
U8 get_preheat_mode_comp_rps(void)
{
    /*..hui [23-4-7오후 5:00:06] 예열 모드 RPS 60HZ..*/
    return BLDC_60Hz;
}


/// @brief      핫가스 동작 제어 시간 판단 함수
/// @details    핫가스 동작시 제어하는 시간을 판단하여 반환한다
/// @param      void
/// @return     return : 핫가스 동작 제어 시간
U16 get_hotgas_time(void)
{
    U16 mu16_return = 0;
    F32 mf32AirTemp = 0;

    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

    if (mf32AirTemp < 9.0f)
    {
        /*..hui [23-4-7오후 1:17:04] 9도 미만..*/
        mu16_return = HOT_GAS_TIME_9_UNDER_765S;
    }
    else if (mf32AirTemp <= 13.0f)
    {
        /*..hui [23-4-7오후 1:23:42] 9도 ~ 13도..*/
        mu16_return = HOT_GAS_TIME_13_UNDER_600S;
    }
    else if (mf32AirTemp <= 19.0f)
    {
        /*..hui [23-4-7오후 1:23:55] 14도 ~ 19도..*/
        mu16_return = HOT_GAS_TIME_19_UNDER_180S;
    }
    else if (mf32AirTemp <= 24.0f)
    {
        /*..hui [23-4-7오후 1:24:08] 20도 ~ 24도..*/
        mu16_return = HOT_GAS_TIME_24_UNDER_30S;
    }
    else if (mf32AirTemp <= 29.0f)
    {
        /*..hui [23-4-7오후 1:24:18] 25도 ~ 29도..*/
        mu16_return = HOT_GAS_TIME_29_UNDER_20S;
    }
    else
    {
        /*..hui [23-4-7오후 1:24:23] 30도 이상..*/
        mu16_return = HOT_GAS_TIME_30_OVER_15S;
    }

    return mu16_return;
}


/// @brief      예열 동작 제어 시간 판단 함수
/// @details    예열 동작시 제어하는 시간을 판단하여 반환한다
/// @param      void
/// @return     return : 예열 동작 제어 시간
U16 get_preheat_time(void)
{
    U16 mu16_return = 0;
    F32 mf32AirTemp = 0;

    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

    if (mf32AirTemp < 14.0f)
    {
        /*..hui [23-4-7오후 5:05:22] 14도 미만..*/
        mu16_return = PREHEAT_TIME_14_UNDER_600S;
    }
    else if (mf32AirTemp < 20.0f)
    {
        /*..hui [23-4-7오후 5:05:35] 14도 ~ 19도..*/
        mu16_return = PREHEAT_TIME_20_UNDER_360S;
    }
    else
    {
        /*..hui [23-4-7오후 5:05:45] 20도 ~ 24도..*/
        mu16_return = PREHEAT_TIME_25_UNDER_360S;
    }

    return mu16_return;
}


/// @brief      제빙 시스템 초기화 함수
/// @details    제빙 시스템을 초기화 시킨다
/// @param      void
/// @return     void
void ice_system_reset(void)
{
    Set_IceSafetyRoutine(CLEAR);
    gu8IceStep = STATE_0_STANDBY;
    gu8InitStep = 0;

    F_IceVV = CLEAR;

    /*..hui [18-3-22오전 10:04:37] CCW 방향 이동중에도 얼음이 있을수있음.. 더미탈빙 진행..생각이 짧았다....*/
    F_IceInit = SET;
}


/// @brief      얼음 걸림 해제 동작 초기화 함수
/// @details    얼음 걸림 해제 동작을 초기화 시킨다
/// @param      void
/// @return     void
void ice_stuck_reset(void)
{
    Set_IceSafetyRoutine(CLEAR);
    gu8IceStep = STATE_0_STANDBY;
    gu8InitStep = 0;

    F_IceVV = CLEAR;
}


/// @brief      제빙 시스템 정지 함수
/// @details    제빙 시스템을 정지 시킨다
/// @param      void
/// @return     void
void ice_system_stop(void)
{
    Set_IceSafetyRoutine(SET);
    //F_IceMakeGo = CLEAR;
    //F_IceInit = CLEAR;
}


/// @brief      냉수 급수 밸브 동작 상태 설정 함수
/// @details    냉수 급수 밸브 동작 상태를 설정한다
/// @param      mu8Status : 냉수 급수 밸브 동작 Flag - 0(미동작), 1(동작)
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


/// @brief      냉수 급수 밸브 동작 상태 확인 함수
/// @details    냉수 급수 밸브 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 냉수 급수 밸브 동작 Flag - 0(미동작), 1(동작)
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


/// @brief      제빙 시스템 안전 제어 동작 상태 설정 함수
/// @details    제빙 시스템 안전 제어 동작 상태를 설정한다
/// @param      mu8Status : 제빙 시스템 안전 제어 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      제빙 시스템 안전 제어 동작 상태 확인 함수
/// @details    제빙 시스템 안전 제어 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 제빙 시스템 안전 제어 동작 상태 - 0(미동작), 1(동작)
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


/// @brief      만빙 검사 시작 설정 함수
/// @details    만빙 검사 동작을 진행할 것인지에 대한 상태를 설정한다
/// @param      mu8Status : 만빙 검사 제어 시작 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      만빙 검사 시작 설정 확인 함수
/// @details    만빙 검사 동작을 진행하는지 여부를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 만빙 검사 제어 시작 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      만빙 상태 설정 함수
/// @details    만빙 상태 설정값을 설정한다
/// @param      mu8Status : 만빙 상태 여부 - 0(만빙 아님), 1(만빙)
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


/// @brief      만빙 상태 확인 함수
/// @details    만빙 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 만빙 상태 여부 - 0(만빙 아님), 1(만빙)
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


/// @brief      제빙 초기화 동작 설정 함수
/// @details    제빙 초기화 동작을 진행할 것인지에 대한 상태를 설정한다
/// @param      mu8Status : 제빙 초기화 제어 시작 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      제빙 초기화 동작 여부 확인 함수
/// @details    제빙 초기화 동작을 진행하는지에 대한 설정 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 제빙 초기화 제어 시작 동작 여부 - 0(미동작), 1(동작)
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


/// @brief      제빙 초기화 제어 단계 설정 함수
/// @details    제빙 초기화 제어 단계를 설정한다
/// @param      mu8Step : 제빙 초기화 제어 단계
/// @return     void
void Set_InitIceStep(U8 mu8Step)
{
    gu8InitStep = mu8Step;
}


/// @brief      동작 중인 제빙 초기화 제어 단계 확인 함수
/// @details    동작 중인 제빙 초기화 제어 단계를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 제빙 초기화 제어 단계
U8 Get_InitIceStep(void)
{
    return  gu8InitStep;
}


/// @brief      제빙 제어 단계 설정 함수
/// @details    제빙 제어 단계를 설정한다
/// @param      mu8Step : 제빙 제어 단계
/// @return     void
void Set_IceStep(U8 mu8Step)
{
    gu8IceStep = mu8Step;
}


/// @brief      동작 중인 제빙 제어 단계 확인 함수
/// @details    동작 중인 제빙 제어 단계를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 제빙 제어 단계
U8 Get_IceStep(void)
{
    return  gu8IceStep;
}


/// @brief      제빙 불가 상태 설정 함수
/// @details    제빙 불가 상태를 설정한다
/// @param      mu8Status : 제빙 불가 상태 - 0(미발생), 1(발생)
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


/// @brief      제빙 불가 상태 확인 함수
/// @details    제빙 불가 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 제빙 불가 상태 - 0(미발생), 1(발생)
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


/// @brief      얼음 토출 회전 상태값 설정 함수
/// @details    얼음 토출 회전 상태 Flag를 설정한다
/// @param      mu8Status : 토출 회전 상태 - 0(미동작), 1(동작)
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


/// @brief      얼음 토출 회전 상태값 확인 함수
/// @details    얼음 토출 회전 상태 Flag의 값을 확인하여 반환한다
/// @param      void
/// @return     return : 토출 회전 상태 - 0(미동작), 1(동작)
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


/// @brief      얼음 추출 상태 설정 함수
/// @details    얼음 추출 상태값을 설정한다
/// @param      mu8Status : 얼음 추출 상태 - 0(미추출), 1(추출)
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


/// @brief      얼음 추출 상태 확인 함수
/// @details    얼음 추출 상태값을 확인하여 반환한다
/// @param      void
/// @return     return : 얼음 추출 상태 - 0(미추출), 1(추출)
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


/// @brief      얼음 연속 추출 상태 설정 함수
/// @details    얼음 연속 추출 상태값을 설정한다
/// @param      mu8Status : 얼음 연속 추출 상태 - 0(미동작), 1(동작)
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


/// @brief      얼음 연속 추출 상태 확인 함수
/// @details    얼음 연속 추출 상태값을 확인하여 반환한다
/// @param      void
/// @return     return : 얼음 연속 추출 상태 - 0(미동작), 1(동작)
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


/// @brief      제빙 불가 상황 확인 함수
/// @details    제빙 불가 상황이 발생했는지 체크한다
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

            /*..hui [24-2-6오전 9:43:07] 더미탈빙 전에 제빙 정지 에러 발생하고 2시간 지나면 더미탈빙 취소..*/
            /*..hui [24-2-6오전 9:43:13] 냉각 전환해서 냉각하기 위해..*/
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

        /*..hui [19-12-18오후 7:50:28] 에러 해제되고 더미탈빙 진행하는 에러들..*/
        /*..hui [19-12-18오후 10:41:07] 피드밸브/수위 언발란스 에러 추가..*/
        /*..hui [19-12-18오후 10:44:09] 피드밸브/수위 언발란스는 자동해제가 아니기때문에 의미없음..*/
        /*..hui [19-12-18오후 10:44:30] 만수위센서 에러도 마찬가지..*/
        /*..hui [19-12-19오후 1:00:22] 정수온도센서는 제빙 전에 에러감지하기 때문에 삭제..*/
           /*..hui [23-9-20오전 9:24:23] 둘 다 고장..*/
        mu8E43_Buf = Get_ErrorStatus(ERROR_ID_AIR_TEMP_1_E43);
        mu8E53_Buf = Get_ErrorStatus(ERROR_ID_AIR_TEMP_2_E53);
        mu8E01_Buf = Get_ErrorStatus(ERROR_ID_LEAKAGE_E01);

        if ( ( (mu8E43_Buf == TRUE) && (mu8E53_Buf == TRUE) ) ||
             (mu8E01_Buf == TRUE) )
        {
            if ( (gu8IceStep >= STATE_31_MAIN_ICE_MAKING) &&
                 (gu8IceStep <= STATE_43_ICE_TAKE_OFF) )
            {
                /*..hui [19-12-18오후 10:43:42] 제빙중에 에러 걸렸을때만 해제됐을때 더미탈빙 진행..*/
                F_re_ice_init_error = SET;
            }
            else
            {

            }
        }
        else
        {

        }

        /*..hui [20-2-13오후 5:42:54] 에러 발생 시 트레이 내림.. 트레이에 물 고여있는 상황 발생할 수 있음..*/
        /*..hui [20-2-14오전 11:20:25] 단, 트레이 관련 에러는 그냥 현위치대로 남겨둠.. 서비스 용이하도록..*/
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
                    /*.. sean [25-02-05] 트레이 미감지 이슈로 스텝모터 pulse 추가..*/
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


/// @brief      제빙 잔여 시간 확인 함수
/// @details    제빙 잔여 시간을 확인하여 반환한다
/// @param      void
/// @return     return : 제빙 잔여 시간
U16 Get_IceMakeTime(void)
{
    return  gu16IceMakeTime;
}


/// @brief      제빙 시스템 이상 여부 체크 함수
/// @details    제빙 시스템 이상 여부를 체크한다
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


/// @brief      제빙 시스템 이상 여부 상태 설정 함수
/// @details    제빙 시스템 이상 여부 상태를 설정한다
/// @param      mu8Status : 제빙 시스템 이상 여부 - 0(이상), 1(정상)
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


/// @brief      제빙 시스템 이상 여부 상태 확인 함수
/// @details    제빙 시스템 이상 여부 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 제빙 시스템 이상 여부 - 0(이상), 1(정상)
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
/// @details    Ice Making 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_IceMaking_Module_Initialize(void)
{
#if (LIB_ICE_MAKING_USE == USE)
    Lib_IceMaking_Initialize();
#endif
}


/// @brief      Lib Ice Making Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 Ice Making 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_IceMaking_Module_1ms_Control(void)
{
#if (LIB_ICE_MAKING_USE == USE)
    IceMakingControlTimer();
#endif
}


/// @brief      Lib Ice Making Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Ice Making 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_IceMaking_Module_Control(void)
{
#if (LIB_ICE_MAKING_USE == USE)
    Check_AbleIceMaking();
    Ice_Make_Process();
#endif
}
