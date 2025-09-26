/// @file     Lib_Water_IceMaking_ModeDecision.c
/// @date     2025/08/05
/// @author   Jaejin Ham
/// @brief    제빙 관련 모드 제어 관련


#include "Global_Header.h"

#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)

U8 gu8IceModeDecisionControlTimer = 0;                  /// @brief  제빙 관련 모드 제어 주기 타이머

/*..hui [20-1-29오후 1:42:16] 냉수 센서 ON/OFF 추가로 감지 타이머 추가.. 혹시몰라..*/
U8 gu8_cold_on_timer = 0;                               /// @brief  냉수 센서 ON 타이머
U16 gu16_cold_off_timer = 0;                            /// @brief  냉수 센서 OFF 타이머
U8 gu8_cold_protect_timer = 0;                          /// @brief  냉각 보호 동작 타이머

U8 bit_cold_first_op = 0;                               /// @brief  냉각 첫기동 상태 - 0(첫기동 아님), 1(첫기동)

U8 bit_drain_full_ice_stop = 0;                         /// @brief  드레인시 제빙 정지 여부 - 0(제빙 동작), 1(제빙 정지)
U16 gu16_drain_full_timer = 0;                          /// @brief  드레인 탱크 만수 상태 감시 시간
U8 gu8DrainWaterLevel = 0;                              /// @brief  드레인 탱크 수위 상태 - 0(탱크 빔), 1(저수위), 2(만수위), 3(에러감지)

U8 bit_fast_ice_make = 0;                               /// @brief  얼음 우선 기능 여부 - 0(미설정), 1(설정)
U8 gu8_fast_ice_make_count = 0;                         /// @brief  얼음 우선 제빙 동작 횟수
U8 bit_count_ice_make = 0;                              /// @brief  제빙 횟수 체크 여부 - 0(미체크), 1(체크)
U16 gu16_fast_ice_cold_max_timer = 0;                   /// @brief  얼음 우선 기능 동작 최대 시간 타이머

U8 bit_set_temp_change = 0;                             /// @brief  제어 온도 변경 여부 - 0(미변경), 1(변경)
U8 gu8_recover_org_fast_ice = 0;                        /// @brief  얼음 우선 기능 이전 상태 보완 상태

UTYPE_WORD  U16ColdOperationW;
#define u16ColdOperation                                    U16ColdOperationW.word
#define u8ColdOperation_L                                   U16ColdOperationW.byte[0]
#define u8ColdOperation_H                                   U16ColdOperationW.byte[1]
#define Bit0_Cold_Setting                                   U16ColdOperationW.Bit.b0
#define Bit1_Cold_Temp_Protect                              U16ColdOperationW.Bit.b1
#define Bit2_Cold_Temp_Control                              U16ColdOperationW.Bit.b2
#define Bit3_Ice_Init_Cold_Stop                             U16ColdOperationW.Bit.b3
#define Bit4_Cold_Error                                     U16ColdOperationW.Bit.b4
#define Bit5_Cold_Flushing_Stop                             U16ColdOperationW.Bit.b5
#define Bit6_Cold_Ice_Ster_Stop                             U16ColdOperationW.Bit.b6
#define Bit7_Cody_Service_Cold_Off                          U16ColdOperationW.Bit.b7

UTYPE_WORD  U16IceOperationW;
#define u16IceOperation                                     U16IceOperationW.word
#define u8IceOperation_L                                    U16IceOperationW.byte[0]
#define u8IceOperation_H                                    U16IceOperationW.byte[1]
#define Bit0_Ice_Setting                                    U16IceOperationW.Bit.b0
#define Bit1_Ice_Temp_Protect                               U16IceOperationW.Bit.b1
#define Bit2_Ice_Full                                       U16IceOperationW.Bit.b2
#define Bit3_Ice_Stop_Six_Hour                              U16IceOperationW.Bit.b3
#define Bit4_Ice_Init_Ice_Stop                              U16IceOperationW.Bit.b4
#define Bit5_Ice_Stop_Safty_Routine                         U16IceOperationW.Bit.b5
#define Bit6_Ice_Error                                      U16IceOperationW.Bit.b6
#define Bit7_Cody_Service_Ice_Off                           U16IceOperationW.Bit.b7
#define Bit8_Ice_Stop_Flushing                              U16IceOperationW.Bit.b8
#define Bit9_Ice_Stop_Ice_Ster                              U16IceOperationW.Bit.b9
#define Bit10_Fast_Ice_Make_Limit                           U16IceOperationW.Bit.b10

UTYPE_BYTE  U8ColdTempControlB;
#define gu8_Cold_Temp_Control                               U8ColdTempControlB.byte
#define Bit0_Temp_Control                                   U8ColdTempControlB.Bit.b0
#define Bit1_Temp_Add_Control                               U8ColdTempControlB.Bit.b1

UTYPE_BYTE  U8MakeModeB;
#define gu8_Make_Mode                                       U8MakeModeB.byte
#define Bit0_Cold_Make_Go                                   U8MakeModeB.Bit.b0
#define Bit1_Ice_Make_Go                                    U8MakeModeB.Bit.b1


/// @brief      제빙 동작 모드 제어 관련 Flag & 변수 초기화
/// @details    제빙 모드 제어에 사용되는 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_ModeDecision_Initialize(void)
{
    gu8IceModeDecisionControlTimer = 0;

    gu8_cold_on_timer = 0;
    gu16_cold_off_timer = 0;
    gu8_cold_protect_timer = 0;

    bit_cold_first_op = 0;

    bit_drain_full_ice_stop = 0;
    gu16_drain_full_timer = 0;
    gu8DrainWaterLevel = 0;

    bit_fast_ice_make = 0;
    gu8_fast_ice_make_count = 0;
    bit_count_ice_make = 0;
    gu16_fast_ice_cold_max_timer = 0;

    bit_set_temp_change = 0;
    gu8_recover_org_fast_ice = 0;
}


/// @brief      제빙 모드 제어 동작 Control Timer(@1ms)
/// @details    제빙 모드 제어를 위한 제어 주기 타이머를 카운트 한다
/// @param      void
/// @return     void
void IceMakingModeDecisionControlTimer(void)
{
    if (gu8IceModeDecisionControlTimer < MODE_DECISION_CONTROL_TIME_PERIOD)
    {
        gu8IceModeDecisionControlTimer++;
    }
}


/// @brief      냉각 관련 제어 판단 동작 함수
/// @details    냉각 관련 제어 판단 동작을 한다
/// @param      void
/// @return     void
void Make_Mode_Decision(void)
{
    if (gu8IceModeDecisionControlTimer >= MODE_DECISION_CONTROL_TIME_PERIOD)
    {   // 100ms 마다
        gu8IceModeDecisionControlTimer = 0;

        // 처음 전원 ON시 만빙검사 완료 후 진입하도록 딜레이 삽입 필요

        cold_mode_decision();
        ice_mode_decision();

        final_mode_decision();
    }
}


/// @brief      냉수 동작 제어 판단 함수
/// @details    냉수 동작 제어 판단을 한다
/// @param      void
/// @return     void
void cold_mode_decision(void)
{
    U8 mu8E61_Buf = 0;
    U8 mu8E62_Buf = 0;
    U8 mu8E63_Buf = 0;
    U8 mu8ErrorAboutCold_Buf = 0;
    U8 mu8IceInit_Buf = 0;

    Bit0_Cold_Setting = Get_ColdOnOff();
    mu8E61_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ALL_DETECT_E61);
    mu8E62_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62);
    mu8E63_Buf = Get_ErrorStatus(ERROR_ID_MICRO_SW_DE_ICE_E63);
    mu8ErrorAboutCold_Buf = Get_ColdError();
    mu8IceInit_Buf = Get_InitIceStatus();

    Bit2_Cold_Temp_Control = cold_comp_test();

    /*..hui [23-9-4오후 4:17:26] 트레이 에러 걸렸을때 냉각은 정상 동작하기위해..*/
    /*..hui [23-9-4오후 4:17:42] 트레이 에러 걸렸을때 F_IceInit이 SET된 상태로 끝날수잇음..*/
    if ( (mu8E61_Buf == SET) ||
         (mu8E62_Buf == SET) ||
         (mu8E63_Buf == SET) )
    {
        Bit3_Ice_Init_Cold_Stop = SET;
    }
    else
    {
        // Bit3_Ice_Init_Cold_Stop = ~F_IceInit;
        if (mu8IceInit_Buf == TRUE)
        {
            Bit3_Ice_Init_Cold_Stop = CLEAR;
        }
        else
        {
            Bit3_Ice_Init_Cold_Stop = SET;
        }
    }

    /*..hui [19-8-2오후 7:28:42] 냉수 관련 에러 발생 시 냉수측 냉각 정지..*/
    /*..hui [19-8-2오후 7:28:56] 냉수 에러 발생해도 제빙측은 정상 동작해야하기 때문에 이쪽으로 이동..*/
    // Bit4_Cold_Error = ~Bit0_Cold_Operation_Disable_State;
    if (mu8ErrorAboutCold_Buf == TRUE)
    {
        Bit4_Cold_Error = CLEAR;
    }
    else
    {
        Bit4_Cold_Error = SET;
    }

/*
    if( bit_install_flushing_state == CLEAR && gu8_flushing_mode == FLUSHING_NONE_STATE )
    {
        Bit5_Cold_Flushing_Stop = SET;
    }
    else
    {
        Bit5_Cold_Flushing_Stop = CLEAR;
    }

    if( bit_ice_tank_ster_start == CLEAR )
    {
        Bit6_Cold_Ice_Ster_Stop = SET;
    }
    else
    {
        Bit6_Cold_Ice_Ster_Stop = CLEAR;
    }


    if( bit_self_test_start == CLEAR )
    {
        Bit7_Cody_Service_Cold_Off = SET;
    }
    else
    {
        Bit7_Cody_Service_Cold_Off = CLEAR;
    }
*/
}


/// @brief      드레인 탱크 수위 상태 설정 함수
/// @details    드레인 탱크 수위 상태를 설정한다
/// @param      mu8Status : 드레인 탱크 수위 상태 - 0(탱크 빔), 1(저수위), 2(만수위), 3(에러감지)
/// @return     void
void Set_IceDrainTankLevel(U16 mu8Status)
{
    gu8DrainWaterLevel = mu8Status;
}


/// @brief      드레인 탱크 수위 상태 확인 함수
/// @details    드레인 탱크 수위 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 드레인 탱크 수위 상태 - 0(탱크 빔), 1(저수위), 2(만수위), 3(에러감지)
U8 Get_IceDrainTankLevel(void)
{
    return  gu8DrainWaterLevel;
}



/// @brief      얼음 우선 설정 함수
/// @details    얼음 우선 설정 동작을 한다
/// @param      mu8Status : 얼음 우선 기능 여부 - 0(미설정), 1(설정)
/// @return     void
void Set_FastIceMakeStatus(U16 mu8Status)
{
    if (mu8Status)
    {
        bit_fast_ice_make = SET;
    }
    else
    {
        bit_fast_ice_make = CLEAR;
    }
}


/// @brief      얼음 우선 설정 확인 함수
/// @details    얼음 우선 설정 상태 확인을 하여 반환한다
/// @param      void
/// @return     return : 얼음 우선 기능 여부 - 0(미설정), 1(설정)
U8 Get_FastIceMakeStatus(void)
{
    if (bit_fast_ice_make)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      제빙 동작 제어 판단 함수
/// @details    제빙 동작 제어 판단을 한다
/// @param      void
/// @return     void
void ice_mode_decision(void)
{
    U8 mu8IceOn_Buf = 0;
    U8 mu8TrayInBlocked_Buf = 0;
    U8 mu8IceFull_Buf = 0;
    U8 mu8IceInit_Buf = 0;
    U8 mu8IceSafeRoutine_Buf = 0;
    U8 mu8IceDisable_Buf = 0;
    U8 mu8DrainWaterLevel_Buf = 0;
    U8 mu8CompActBuf = 0;

    mu8IceOn_Buf = Get_IceOnOff();
    mu8TrayInBlocked_Buf = Get_IceTrayInBlocked();
    mu8IceFull_Buf = Get_IceFullStatus();
    mu8IceInit_Buf = Get_InitIceStatus();
    mu8IceSafeRoutine_Buf = Get_IceSafetyRoutine();
    mu8IceDisable_Buf = Get_IceMakingDisable();
    mu8DrainWaterLevel_Buf = Get_IceDrainTankLevel();

    if (mu8IceOn_Buf)
    {
        Bit0_Ice_Setting = SET;
    }
    else
    {
        Bit0_Ice_Setting = CLEAR;
    }

    if (mu8TrayInBlocked_Buf == FALSE)
    {
        Bit1_Ice_Temp_Protect = SET;
    }
    else
    {
        Bit1_Ice_Temp_Protect = CLEAR;
    }

    /*Bit3_Ice_Full = ~F_IceFull;*/
    if (mu8IceFull_Buf == TRUE)
    {
        Bit2_Ice_Full = CLEAR;
    }
    else
    {
        Bit2_Ice_Full = SET;
    }

    /*Bit4_Ice_Stop_Six_Hour = ~F_IceStop;*/
    /*..hui [23-6-29오후 5:20:21] 미사용절전모드는 제빙하고는 상관없음..*/
    /*..hui [23-6-29오후 5:20:47] 취침모드는 만빙까지 채우지않고 진입 즉시 하던거까지만 하고 제빙 멈춤.. 최인식..*/
//    Bit3_Ice_Stop_Six_Hour = ~bit_sleep_mode_start;
//    if (bit_sleep_mode_start)
//    {
//        Bit3_Ice_Stop_Six_Hour = CLEAR;
//    }
//    else
//    {
//        Bit3_Ice_Stop_Six_Hour = SET;
//    }

    // Bit4_Ice_Init_Ice_Stop = ~F_IceInit;
    if (mu8IceInit_Buf == TRUE)
    {
        Bit4_Ice_Init_Ice_Stop = CLEAR;
    }
    else
    {
        Bit4_Ice_Init_Ice_Stop = SET;
    }

    /*..hui [19-12-18오후 1:49:10] 트레이 이동에러 재시도 진입중에는 제빙 정지 후 냉각 전환..*/
    // Bit5_Ice_Stop_Safty_Routine = ~F_Safety_Routine;
    if (mu8IceSafeRoutine_Buf == TRUE)
    {
        Bit5_Ice_Stop_Safty_Routine = CLEAR;
    }
    else
    {
        Bit5_Ice_Stop_Safty_Routine = SET;
    }

    /*..hui [19-12-18오후 8:03:47] 제빙 불가 에러..*/
    // Bit6_Ice_Error = ~Bit2_Ice_Operation_Disable_State;
    if (mu8IceDisable_Buf == TRUE)
    {
        Bit6_Ice_Error = CLEAR;
    }
    else
    {
        Bit6_Ice_Error = SET;
    }

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-4-2오후 7:42:15] 드레인 탱크 만수위 상태가 5분 이상 지속시 제빙 진입 금지..*/
    /*..hui [24-4-2오후 7:42:31] 배수 막혀서 얼음 탱크 물 넘칠수있음..*/
    if (mu8DrainWaterLevel_Buf == DRAIN_LEVEL_HIGH)
    {
        gu16_drain_full_timer++;

        /*..hui [24-4-5오후 4:09:04] 5분 -> 10분으로 변경..*/
        if (gu16_drain_full_timer >= DRAIN_FULL_ICE_STOP_TIME)
        {
            gu16_drain_full_timer = DRAIN_FULL_ICE_STOP_TIME;
            bit_drain_full_ice_stop = SET;
        }
        else
        {
            bit_drain_full_ice_stop = CLEAR;
        }
    }
    else
    {
        gu16_drain_full_timer = 0;
        bit_drain_full_ice_stop = CLEAR;
    }

    /*..hui [20-1-20오후 10:26:55] 코디 서비스 모드 진행 중 제빙 정지..*/
    /*..hui [23-12-20오후 5:14:23] 고장진단중일때는 정지..*/
    /*.. sean [25-01-20] 고장 진단중에는 정지..*/
    //if( u8CodyInspectOperation == 0 && bit_drain_full_ice_stop == CLEAR )
//    if (u8CodyInspectOperation == 0 && bit_drain_full_ice_stop == CLEAR && bit_self_test_start == CLEAR )
//    {
//        Bit7_Cody_Service_Ice_Off = SET;
//    }
//    else
//    {
//        Bit7_Cody_Service_Ice_Off = CLEAR;
//    }
//
//    if( bit_install_flushing_state == CLEAR && gu8_flushing_mode == FLUSHING_NONE_STATE )
//    {
//        Bit8_Ice_Stop_Flushing = SET;
//    }
//    else
//    {
//        Bit8_Ice_Stop_Flushing = CLEAR;
//    }
//
//    if( bit_ice_tank_ster_start == CLEAR )
//    {
//        Bit9_Ice_Stop_Ice_Ster = SET;
//    }
//    else
//    {
//        Bit9_Ice_Stop_Ice_Ster = CLEAR;
//    }


////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
    /*..hui [24-4-11오후 7:08:35] 혹시몰라.. 이전 모드 변경 특수모드 추가..*/
    if (gu8_recover_org_fast_ice == NEW_FAST_ICE)
    {
        /*..hui [24-4-11오후 3:53:13] 얼음우선일때.. 제빙 횟수 체크..*/
        count_ice_make();

        if (bit_fast_ice_make == ICE_FIRST_ICE_MAKE)
        {
            if (mu8IceOn_Buf == FALSE)
            {
                /*..hui [24-4-11오후 3:57:33] 얼음 OFF -> ON 시 횟수 초기화..*/
                gu8_fast_ice_make_count = 0;
            }
            else
            {

            }

//            if( bit_sleep_mode_start == SET )
//            {
//                /*..hui [24-4-12오후 12:14:40] 취침모드 진입시 횟수 초기화..*/
//                gu8_fast_ice_make_count = 0;
//            }
//            else{}

            /*..hui [24-4-11오후 3:11:27] 얼음우선 / 제빙 4회 하고 나서..*/
            if (gu8_fast_ice_make_count >= FAST_ICE_MAKE_COUNT)
            {
                /*..hui [24-4-11오후 3:11:34] 냉각 가동 조건이면 냉각으로 이동..*/
                if (u16ColdOperation == COLD_MAKE_GOGOGO)
                {
                    Bit10_Fast_Ice_Make_Limit = CLEAR;

                    /*..hui [24-4-11오후 4:04:28] 냉각 모드로 가동중인 조건에서 확인..*/
//                    if( Bit0_Cold_Mode_On_State == SET && F_Comp_Output == SET )
                    mu8CompActBuf = Get_CompActStatus();

                    if (mu8CompActBuf == TRUE)
                    {
                        gu16_fast_ice_cold_max_timer++;

                        /*..hui [24-4-11오후 4:02:20] 냉각 가동시간 최대 1시간 경과시 다시 제빙해야함..*/
                        if (gu16_fast_ice_cold_max_timer >= FAST_ICE_COLD_MAX_OP_TIME)
                        {
                            gu16_fast_ice_cold_max_timer = 0;
                            gu8_fast_ice_make_count = 0;
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
                    Bit10_Fast_Ice_Make_Limit = SET;
                    gu16_fast_ice_cold_max_timer = 0;
                    /*..hui [24-4-11오후 3:15:38] 냉각 미가동 조건일경우 카운트 초기화하고 다시 4회 제빙..*/
                    /*..hui [24-4-11오후 7:39:40] 여기서는 초기화하면안됨..*/
                    /*..hui [24-4-11오후 7:39:53] 4번이후 한번 제빙하고 계속 냉각조건 확인해야하기 때문..*/
                    /*gu8_fast_ice_make_count = 0;*/
                }
            }
            else
            {
                Bit10_Fast_Ice_Make_Limit = SET;
                gu16_fast_ice_cold_max_timer = 0;
            }
        }
        else
        {
            Bit10_Fast_Ice_Make_Limit = SET;
            gu16_fast_ice_cold_max_timer = 0;
            /*..hui [24-4-11오후 3:15:38] 냉각 미가동 조건일경우 카운트 초기화하고 다시 4회 제빙..*/
            gu8_fast_ice_make_count = 0;
        }
    }
    else
    {
        bit_count_ice_make = CLEAR;

        Bit10_Fast_Ice_Make_Limit = SET;
        gu16_fast_ice_cold_max_timer = 0;
        /*..hui [24-4-11오후 3:15:38] 냉각 미가동 조건일경우 카운트 초기화하고 다시 4회 제빙..*/
        gu8_fast_ice_make_count = 0;
    }
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

}


/// @brief      제빙 횟수 카운트 함수
/// @details    제빙 횟수 카운트 동작을 한다
/// @param      void
/// @return     void
void count_ice_make(void)
{
    U8 mu8IceStep_Buf = 0;

    mu8IceStep_Buf = Get_IceStep();

    if (bit_fast_ice_make == ICE_FIRST_ICE_MAKE)
    {
        if (mu8IceStep_Buf == STATE_43_ICE_TAKE_OFF)
        {
            if (bit_count_ice_make == CLEAR)
            {
                bit_count_ice_make = SET;

                gu8_fast_ice_make_count++;

                if (gu8_fast_ice_make_count >= FAST_ICE_MAKE_COUNT)
                {
                    gu8_fast_ice_make_count = FAST_ICE_MAKE_COUNT;
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
            bit_count_ice_make = CLEAR;
        }

    }
    else
    {
        bit_count_ice_make = CLEAR;
    }
}


/// @brief      냉수 냉각 동작 함수
/// @details    냉수 냉각 동작 제어를 한다
/// @param      void
/// @return     void
U8 cold_comp_test(void)
{
    F32 mf32_cold_on_temp = 0;
    F32 mf32_cold_off_temp = 0;
    U16 mu16_cold_delay_time = 0;

    F32 mf32_ColdTemp = 0;
    F32 mf32_AirTemp = 0;

    U8 mu8ColdStrong_Buf = 0;

    mf32_ColdTemp = Get_Temp(ADC_ID_TH_COOL);
    mf32_AirTemp = Get_Temp(ADC_ID_TH_AIR);

    if ( (gu8_recover_org_fast_ice == NEW_FAST_ICE) &&
         (bit_fast_ice_make == ICE_FIRST_ICE_MAKE) &&
         ( (u16IceOperation == FAST_ICE_MAKE_ENABLE ) ||
           (u16IceOperation == ICE_MAKE_GOGOGO) ) )
    {
        /*..hui [24-4-11오후 2:37:22] 얼음 우선 설정돼있을때.. 냉각 온도 15도, 7.5도..*/
        mf32_cold_on_temp = 15.0f;
        mf32_cold_off_temp = 7.5f;
        mu16_cold_delay_time = 20;

        /*..hui [24-4-11오후 5:08:34] 얼음우선 제빙 조건 아닌 경우 -> 제빙 조건으로 변경될때..*/
        /*..hui [24-4-11오후 5:08:39] 냉각 가동 조건 다시 확인하기 위해..*/
        /*..hui [24-4-11오후 5:09:15] 더미탈빙 -> 냉각 가동조건 8도 -> 냉각 가동조건 set -> 더미탈빙 종료..*/
        /*..hui [24-4-11오후 5:09:39] 제빙 -> 4회 진행 -> 가동조건 set돼있기 때문에 냉각 가동으로 넘어감..*/
        /*..hui [24-4-11오후 5:10:00] 제빙 가동 조건되면 냉각 조건은 다시 15도로 확인하도록..*/
        if (bit_set_temp_change == CLEAR)
        {
            bit_set_temp_change = SET;
            Bit0_Temp_Control = CLEAR;
        }
        else
        {

        }
    }
    else
    {
        bit_set_temp_change = CLEAR;

        mu8ColdStrong_Buf = Get_ColdStrong();

        if (mu8ColdStrong_Buf == TRUE)
        {   // 냉각 강 설정인 경우
            if (mf32_AirTemp <= 10.0f)
            {   // 외기 10도 이하
//                if (bit_sleep_mode_start  == SET)
//                {
//                    /*..hui [23-4-7오전 11:15:58] 10도 이하..*/
//                    mf32_cold_on_temp = 10.0f;
//                    mf32_cold_off_temp = 6.5f;
//                    mu16_cold_delay_time = 20;
//                }
//                else
//                {
                    /*..hui [23-4-7오전 11:15:58] 10도 이하..*/
                    /*..hui [25-6-2오전 9:33:33] 250528 냉각 최종사양(이경민님)..*/
                    mf32_cold_on_temp = 8.5f;
                    mf32_cold_off_temp = 6.5f;
                    mu16_cold_delay_time = 20;
//                }
            }
            else if (mf32_AirTemp <= 20.0f)
            {   // 외기 20도 이하
//                if (bit_sleep_mode_start  == SET)
//                {
//                    /*..hui [23-4-7오전 11:16:02] 20도 이하..*/
//                    mf32_cold_on_temp = 10.0f;
//                    mf32_cold_off_temp = 5.8f;
//                    mu16_cold_delay_time = 20;
//                }
//                else
//                {
                    /*..hui [23-4-7오전 11:16:02] 20도 이하..*/
                    if (bit_cold_first_op == SET)
                    {
                        mf32_cold_on_temp = 7.5f;
                        mf32_cold_off_temp = 5.8f;
                        mu16_cold_delay_time = 20;
                    }
                    else
                    {
                        mf32_cold_on_temp = 8.0f;
                        mf32_cold_off_temp = 6.0f;
                        mu16_cold_delay_time = 20;
                    }
//                }
            }
            else if (mf32_AirTemp <= 30.0f)
            {   // 외기 30도 이하
//                if (bit_sleep_mode_start  == SET)
//                {
//                    /*..hui [23-4-7오전 11:16:10] 30도 이하..*/
//                    mf32_cold_on_temp = 10.0f;
//                    mf32_cold_off_temp = 5.8f;
//                    mu16_cold_delay_time = 20;
//                }
//                else
//                {
                    /*..hui [23-4-7오전 11:16:10] 30도 이하..*/
                    mf32_cold_on_temp = 7.5f;
                    mf32_cold_off_temp = 5.8f;
                    mu16_cold_delay_time = 20;
//                }
            }
            else
            {
//                if (bit_sleep_mode_start  == SET)
//                {
//                    /*..hui [23-4-7오전 11:16:14] 30도 초과..*/
//                    mf32_cold_on_temp = 10.0f;
//                    mf32_cold_off_temp = 5.8f;
//                    mu16_cold_delay_time = 3000;
//                }
//                else
//                {
                    /*..hui [23-4-7오전 11:16:14] 30도 초과..*/
                    if( bit_cold_first_op == SET )
                    {
                        mf32_cold_on_temp = 7.5f;
                        mf32_cold_off_temp = 5.8f;
                        mu16_cold_delay_time = 6000;
                    }
                    else
                    {
                        mf32_cold_on_temp = 6.5f;
                        mf32_cold_off_temp = 5.8f;
                        mu16_cold_delay_time = 3000;
                    }
//                }
            }
        }
        else /*if( gu8_cold_setting_level == COLD_TEMP_LOW )*/
        {
            /*..hui [25-3-19오후 2:22:52] 우선 냉수 약설정시 14도 10도로..*/
            mf32_cold_on_temp = 14.0f;
            mf32_cold_off_temp = 10.0f;
            mu16_cold_delay_time = 20;
        }
    }

    if (mf32_ColdTemp >= mf32_cold_on_temp)
    {
        gu8_cold_on_timer++;

        if (gu8_cold_on_timer >= 20)
        {
            gu8_cold_on_timer = 20;
            /*Bit0_Temp_Control = SET;*/

            if (Bit0_Temp_Control == CLEAR)
            {
                Bit0_Temp_Control = SET;

                /*..hui [25-3-19오후 4:48:38] 기동 시작 시 냉수온도 20도 이상, 초기 기동..*/
                if (mf32_ColdTemp >= COLD_FIRST_OPERATION_20_DEGREE)
                {
                    bit_cold_first_op = SET;
                }
                else
                {
                    bit_cold_first_op = CLEAR;
                }
            }
            else
            {

            }
        }
        else
        {

        }

        gu16_cold_off_timer = 0;
    }
    else
    {
        gu8_cold_on_timer = 0;

        if (Bit0_Temp_Control == SET)
        {
            if (mf32_ColdTemp <= mf32_cold_off_temp)
            {
                gu16_cold_off_timer++;

                if (gu16_cold_off_timer >= mu16_cold_delay_time)
                {
                    gu16_cold_off_timer = 0;
                    Bit0_Temp_Control = CLEAR;

                    /*..hui [24-4-12오전 11:43:45] 냉각 완료시 제빙 횟수 초기화 이쪽에서..*/
                    gu8_fast_ice_make_count = 0;
                }
                else
                {

                }
            }
            else
            {
                gu16_cold_off_timer = 0;
            }
        }
        else
        {
            gu16_cold_off_timer = 0;
        }
    }

    /*..hui [19-7-25오후 5:40:39] 2도 이하 프로텍트 OFF 위치이동..*/
    if (mf32_ColdTemp <= PROTECT_COLD_TEMP)
    {
        gu8_cold_protect_timer++;

        if (gu8_cold_protect_timer >= 20)
        {
            gu8_cold_protect_timer = 20;
            gu8_Cold_Temp_Control = 0;
        }
        else
        {

        }
    }
    else
    {
        gu8_cold_protect_timer = 0;
    }

    if (gu8_Cold_Temp_Control > 0)
    {
        return SET;
    }
    else
    {
        return CLEAR;
    }
}


/// @brief      최종 냉각 동작 판단 함수
/// @details    최종 냉각 동작 판단 제어를 한다
/// @param      void
/// @return     void
void final_mode_decision(void)
{
    /*if( gu8_ice_setting == ICE_SETTING_COLD_FIRST )*/
    if(bit_fast_ice_make == COLD_FIRST_ICE_MAKE)
    {
        cold_priority_decision();
    }
    else
    {
        ice_priority_decision();
    }
}


/// @brief      냉수 냉각 우선 동작 함수
/// @details    냉수 냉각 우선 동작 제어를 한다
/// @param      void
/// @return     void
void cold_priority_decision(void)
{
    U8 mu8IceStep_Buf = 0;

    mu8IceStep_Buf = Get_IceStep();

    switch (gu8_Make_Mode)
    {
        case MAKE_STOP:
            if(u16ColdOperation == COLD_MAKE_GOGOGO)
            {
                gu8_Make_Mode = MAKE_COLD;
            }
            else
            {
                if(u16IceOperation == ICE_MAKE_GOGOGO)
                {
                    gu8_Make_Mode = MAKE_ICE;
                }
                else
                {
                    gu8_Make_Mode = MAKE_STOP;
                }
            }
            break;

        case MAKE_COLD:
            /*..hui [19-7-25오후 4:21:53] 냉각 중 냉각 조건 종료 시 제빙/냉각 조건 재 확인..*/
            if(u16ColdOperation != COLD_MAKE_GOGOGO)
            {
                gu8_Make_Mode = MAKE_STOP;
            }
            else
            {

            }
            break;

        case MAKE_ICE:
            /*..hui [19-7-25오후 4:22:10] 제빙 중 제빙 완료 시 제빙/냉각 조건 재 확인..*/
            /*if(gu8IceStep == STATE_51_FINISH_ICE_MAKE)*/
            if (mu8IceStep_Buf == STATE_0_STANDBY)
            {
                /*..hui [20-1-21오전 11:15:35] 제빙 모드 중 대기 상태에서 냉각 조건 발생 시..*/
                if (u16ColdOperation == COLD_MAKE_GOGOGO)
                {
                    gu8_Make_Mode = MAKE_STOP;
                }
                else if (u16IceOperation != ICE_MAKE_GOGOGO)
                {
                    gu8_Make_Mode = MAKE_STOP;
                }
                else
                {

                }
            }
            else
            {

            }
            break;

        default:
            gu8_Make_Mode = MAKE_STOP;
            break;
    }
}


/// @brief      제빙 냉각 우선 동작 함수
/// @details    제빙 냉각 우선 동작 제어를 한다
/// @param      void
/// @return     void
void ice_priority_decision(void)
{
    U8 mu8IceStep_Buf = 0;

    mu8IceStep_Buf = Get_IceStep();

    switch (gu8_Make_Mode)
    {
        case MAKE_STOP:
            /*..hui [19-7-26오후 8:09:21] 제빙 우선이므로 만빙될때까지 제빙 먼저..*/
            if (u16IceOperation == ICE_MAKE_GOGOGO)
            {
                gu8_Make_Mode = MAKE_ICE;
            }
            else
            {
                /*..hui [19-7-26오후 8:09:34] 제빙 조건 아닐때만 냉각 동작..*/
                if (u16ColdOperation == COLD_MAKE_GOGOGO)
                {
                    gu8_Make_Mode = MAKE_COLD;
                }
                else
                {
                    gu8_Make_Mode = MAKE_STOP;
                }
            }
            break;

        case MAKE_COLD:
            if (u16IceOperation == ICE_MAKE_GOGOGO)
            {
                /*..hui [19-11-6오후 1:57:36] 냉각 중 제빙 조건 발생 시..*/
                gu8_Make_Mode = MAKE_STOP;
            }
            else if (u16ColdOperation != COLD_MAKE_GOGOGO)
            {
                /*..hui [19-11-6오후 1:57:57] 냉각 중 냉각 조건 해제 시..*/
                gu8_Make_Mode = MAKE_STOP;
            }
            else
            {

            }
            break;

        case MAKE_ICE:
            /*..hui [19-11-6오후 1:59:36] 제빙 중 제빙 완료 시 제빙/냉각 조건 재 확인..*/
            /*if(gu8IceStep == STATE_51_FINISH_ICE_MAKE)*/
            if (mu8IceStep_Buf == STATE_0_STANDBY)
            {
                /*..hui [19-12-18오후 4:06:39] 얼음 모드중 대기 상태에서 얼음 모드 해지조건 발생 시..*/
                if (u16IceOperation != ICE_MAKE_GOGOGO)
                {
                    gu8_Make_Mode = MAKE_STOP;
                }
                else
                {

                }
            }
            else
            {

            }
            break;

        default:
            gu8_Make_Mode = MAKE_STOP;
            break;
    }
}

#endif


// Lib Ice Making Mode Decision Module *********************************************************************************

/// @brief      Lib Ice Making Mode Decision Module Initilaize
/// @details    Ice Making Mode Decision 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_IceMaking_ModeDecision_Module_Initialize(void)
{
#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)
    Lib_ModeDecision_Initialize();
#endif
}


/// @brief      Lib Ice Making Mode Decision Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 Ice Making Mode Decision 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_IceMaking_ModeDecision_Module_1ms_Control(void)
{
#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)
    IceMakingModeDecisionControlTimer();
#endif
}


/// @brief      Lib Ice Making Mode Decision Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Ice Making Mode Decision 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_IceMaking_ModeDecision_Module_Control(void)
{
#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)
    Make_Mode_Decision();
#endif
}

