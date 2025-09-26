/// @file     Lib_Water_IceMaking_ModeDecision.c
/// @date     2025/08/05
/// @author   Jaejin Ham
/// @brief    ���� ���� ��� ���� ����


#include "Global_Header.h"

#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)

U8 gu8IceModeDecisionControlTimer = 0;                  /// @brief  ���� ���� ��� ���� �ֱ� Ÿ�̸�

/*..hui [20-1-29���� 1:42:16] �ü� ���� ON/OFF �߰��� ���� Ÿ�̸� �߰�.. Ȥ�ø���..*/
U8 gu8_cold_on_timer = 0;                               /// @brief  �ü� ���� ON Ÿ�̸�
U16 gu16_cold_off_timer = 0;                            /// @brief  �ü� ���� OFF Ÿ�̸�
U8 gu8_cold_protect_timer = 0;                          /// @brief  �ð� ��ȣ ���� Ÿ�̸�

U8 bit_cold_first_op = 0;                               /// @brief  �ð� ù�⵿ ���� - 0(ù�⵿ �ƴ�), 1(ù�⵿)

U8 bit_drain_full_ice_stop = 0;                         /// @brief  �巹�ν� ���� ���� ���� - 0(���� ����), 1(���� ����)
U16 gu16_drain_full_timer = 0;                          /// @brief  �巹�� ��ũ ���� ���� ���� �ð�
U8 gu8DrainWaterLevel = 0;                              /// @brief  �巹�� ��ũ ���� ���� - 0(��ũ ��), 1(������), 2(������), 3(��������)

U8 bit_fast_ice_make = 0;                               /// @brief  ���� �켱 ��� ���� - 0(�̼���), 1(����)
U8 gu8_fast_ice_make_count = 0;                         /// @brief  ���� �켱 ���� ���� Ƚ��
U8 bit_count_ice_make = 0;                              /// @brief  ���� Ƚ�� üũ ���� - 0(��üũ), 1(üũ)
U16 gu16_fast_ice_cold_max_timer = 0;                   /// @brief  ���� �켱 ��� ���� �ִ� �ð� Ÿ�̸�

U8 bit_set_temp_change = 0;                             /// @brief  ���� �µ� ���� ���� - 0(�̺���), 1(����)
U8 gu8_recover_org_fast_ice = 0;                        /// @brief  ���� �켱 ��� ���� ���� ���� ����

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


/// @brief      ���� ���� ��� ���� ���� Flag & ���� �ʱ�ȭ
/// @details    ���� ��� ��� ���Ǵ� �������� �ʱ�ȭ ��Ų��
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


/// @brief      ���� ��� ���� ���� Control Timer(@1ms)
/// @details    ���� ��� ��� ���� ���� �ֱ� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void IceMakingModeDecisionControlTimer(void)
{
    if (gu8IceModeDecisionControlTimer < MODE_DECISION_CONTROL_TIME_PERIOD)
    {
        gu8IceModeDecisionControlTimer++;
    }
}


/// @brief      �ð� ���� ���� �Ǵ� ���� �Լ�
/// @details    �ð� ���� ���� �Ǵ� ������ �Ѵ�
/// @param      void
/// @return     void
void Make_Mode_Decision(void)
{
    if (gu8IceModeDecisionControlTimer >= MODE_DECISION_CONTROL_TIME_PERIOD)
    {   // 100ms ����
        gu8IceModeDecisionControlTimer = 0;

        // ó�� ���� ON�� �����˻� �Ϸ� �� �����ϵ��� ������ ���� �ʿ�

        cold_mode_decision();
        ice_mode_decision();

        final_mode_decision();
    }
}


/// @brief      �ü� ���� ���� �Ǵ� �Լ�
/// @details    �ü� ���� ���� �Ǵ��� �Ѵ�
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

    /*..hui [23-9-4���� 4:17:26] Ʈ���� ���� �ɷ����� �ð��� ���� �����ϱ�����..*/
    /*..hui [23-9-4���� 4:17:42] Ʈ���� ���� �ɷ����� F_IceInit�� SET�� ���·� ����������..*/
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

    /*..hui [19-8-2���� 7:28:42] �ü� ���� ���� �߻� �� �ü��� �ð� ����..*/
    /*..hui [19-8-2���� 7:28:56] �ü� ���� �߻��ص� �������� ���� �����ؾ��ϱ� ������ �������� �̵�..*/
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


/// @brief      �巹�� ��ũ ���� ���� ���� �Լ�
/// @details    �巹�� ��ũ ���� ���¸� �����Ѵ�
/// @param      mu8Status : �巹�� ��ũ ���� ���� - 0(��ũ ��), 1(������), 2(������), 3(��������)
/// @return     void
void Set_IceDrainTankLevel(U16 mu8Status)
{
    gu8DrainWaterLevel = mu8Status;
}


/// @brief      �巹�� ��ũ ���� ���� Ȯ�� �Լ�
/// @details    �巹�� ��ũ ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �巹�� ��ũ ���� ���� - 0(��ũ ��), 1(������), 2(������), 3(��������)
U8 Get_IceDrainTankLevel(void)
{
    return  gu8DrainWaterLevel;
}



/// @brief      ���� �켱 ���� �Լ�
/// @details    ���� �켱 ���� ������ �Ѵ�
/// @param      mu8Status : ���� �켱 ��� ���� - 0(�̼���), 1(����)
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


/// @brief      ���� �켱 ���� Ȯ�� �Լ�
/// @details    ���� �켱 ���� ���� Ȯ���� �Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �켱 ��� ���� - 0(�̼���), 1(����)
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


/// @brief      ���� ���� ���� �Ǵ� �Լ�
/// @details    ���� ���� ���� �Ǵ��� �Ѵ�
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
    /*..hui [23-6-29���� 5:20:21] �̻���������� �����ϰ�� �������..*/
    /*..hui [23-6-29���� 5:20:47] ��ħ���� �������� ä�����ʰ� ���� ��� �ϴ��ű����� �ϰ� ���� ����.. ���ν�..*/
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

    /*..hui [19-12-18���� 1:49:10] Ʈ���� �̵����� ��õ� �����߿��� ���� ���� �� �ð� ��ȯ..*/
    // Bit5_Ice_Stop_Safty_Routine = ~F_Safety_Routine;
    if (mu8IceSafeRoutine_Buf == TRUE)
    {
        Bit5_Ice_Stop_Safty_Routine = CLEAR;
    }
    else
    {
        Bit5_Ice_Stop_Safty_Routine = SET;
    }

    /*..hui [19-12-18���� 8:03:47] ���� �Ұ� ����..*/
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
    /*..hui [24-4-2���� 7:42:15] �巹�� ��ũ ������ ���°� 5�� �̻� ���ӽ� ���� ���� ����..*/
    /*..hui [24-4-2���� 7:42:31] ��� ������ ���� ��ũ �� ��ĥ������..*/
    if (mu8DrainWaterLevel_Buf == DRAIN_LEVEL_HIGH)
    {
        gu16_drain_full_timer++;

        /*..hui [24-4-5���� 4:09:04] 5�� -> 10������ ����..*/
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

    /*..hui [20-1-20���� 10:26:55] �ڵ� ���� ��� ���� �� ���� ����..*/
    /*..hui [23-12-20���� 5:14:23] �����������϶��� ����..*/
    /*.. sean [25-01-20] ���� �����߿��� ����..*/
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
    /*..hui [24-4-11���� 7:08:35] Ȥ�ø���.. ���� ��� ���� Ư����� �߰�..*/
    if (gu8_recover_org_fast_ice == NEW_FAST_ICE)
    {
        /*..hui [24-4-11���� 3:53:13] �����켱�϶�.. ���� Ƚ�� üũ..*/
        count_ice_make();

        if (bit_fast_ice_make == ICE_FIRST_ICE_MAKE)
        {
            if (mu8IceOn_Buf == FALSE)
            {
                /*..hui [24-4-11���� 3:57:33] ���� OFF -> ON �� Ƚ�� �ʱ�ȭ..*/
                gu8_fast_ice_make_count = 0;
            }
            else
            {

            }

//            if( bit_sleep_mode_start == SET )
//            {
//                /*..hui [24-4-12���� 12:14:40] ��ħ��� ���Խ� Ƚ�� �ʱ�ȭ..*/
//                gu8_fast_ice_make_count = 0;
//            }
//            else{}

            /*..hui [24-4-11���� 3:11:27] �����켱 / ���� 4ȸ �ϰ� ����..*/
            if (gu8_fast_ice_make_count >= FAST_ICE_MAKE_COUNT)
            {
                /*..hui [24-4-11���� 3:11:34] �ð� ���� �����̸� �ð����� �̵�..*/
                if (u16ColdOperation == COLD_MAKE_GOGOGO)
                {
                    Bit10_Fast_Ice_Make_Limit = CLEAR;

                    /*..hui [24-4-11���� 4:04:28] �ð� ���� �������� ���ǿ��� Ȯ��..*/
//                    if( Bit0_Cold_Mode_On_State == SET && F_Comp_Output == SET )
                    mu8CompActBuf = Get_CompActStatus();

                    if (mu8CompActBuf == TRUE)
                    {
                        gu16_fast_ice_cold_max_timer++;

                        /*..hui [24-4-11���� 4:02:20] �ð� �����ð� �ִ� 1�ð� ����� �ٽ� �����ؾ���..*/
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
                    /*..hui [24-4-11���� 3:15:38] �ð� �̰��� �����ϰ�� ī��Ʈ �ʱ�ȭ�ϰ� �ٽ� 4ȸ ����..*/
                    /*..hui [24-4-11���� 7:39:40] ���⼭�� �ʱ�ȭ�ϸ�ȵ�..*/
                    /*..hui [24-4-11���� 7:39:53] 4������ �ѹ� �����ϰ� ��� �ð����� Ȯ���ؾ��ϱ� ����..*/
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
            /*..hui [24-4-11���� 3:15:38] �ð� �̰��� �����ϰ�� ī��Ʈ �ʱ�ȭ�ϰ� �ٽ� 4ȸ ����..*/
            gu8_fast_ice_make_count = 0;
        }
    }
    else
    {
        bit_count_ice_make = CLEAR;

        Bit10_Fast_Ice_Make_Limit = SET;
        gu16_fast_ice_cold_max_timer = 0;
        /*..hui [24-4-11���� 3:15:38] �ð� �̰��� �����ϰ�� ī��Ʈ �ʱ�ȭ�ϰ� �ٽ� 4ȸ ����..*/
        gu8_fast_ice_make_count = 0;
    }
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

}


/// @brief      ���� Ƚ�� ī��Ʈ �Լ�
/// @details    ���� Ƚ�� ī��Ʈ ������ �Ѵ�
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


/// @brief      �ü� �ð� ���� �Լ�
/// @details    �ü� �ð� ���� ��� �Ѵ�
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
        /*..hui [24-4-11���� 2:37:22] ���� �켱 ������������.. �ð� �µ� 15��, 7.5��..*/
        mf32_cold_on_temp = 15.0f;
        mf32_cold_off_temp = 7.5f;
        mu16_cold_delay_time = 20;

        /*..hui [24-4-11���� 5:08:34] �����켱 ���� ���� �ƴ� ��� -> ���� �������� ����ɶ�..*/
        /*..hui [24-4-11���� 5:08:39] �ð� ���� ���� �ٽ� Ȯ���ϱ� ����..*/
        /*..hui [24-4-11���� 5:09:15] ����Ż�� -> �ð� �������� 8�� -> �ð� �������� set -> ����Ż�� ����..*/
        /*..hui [24-4-11���� 5:09:39] ���� -> 4ȸ ���� -> �������� set���ֱ� ������ �ð� �������� �Ѿ..*/
        /*..hui [24-4-11���� 5:10:00] ���� ���� ���ǵǸ� �ð� ������ �ٽ� 15���� Ȯ���ϵ���..*/
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
        {   // �ð� �� ������ ���
            if (mf32_AirTemp <= 10.0f)
            {   // �ܱ� 10�� ����
//                if (bit_sleep_mode_start  == SET)
//                {
//                    /*..hui [23-4-7���� 11:15:58] 10�� ����..*/
//                    mf32_cold_on_temp = 10.0f;
//                    mf32_cold_off_temp = 6.5f;
//                    mu16_cold_delay_time = 20;
//                }
//                else
//                {
                    /*..hui [23-4-7���� 11:15:58] 10�� ����..*/
                    /*..hui [25-6-2���� 9:33:33] 250528 �ð� �������(�̰�δ�)..*/
                    mf32_cold_on_temp = 8.5f;
                    mf32_cold_off_temp = 6.5f;
                    mu16_cold_delay_time = 20;
//                }
            }
            else if (mf32_AirTemp <= 20.0f)
            {   // �ܱ� 20�� ����
//                if (bit_sleep_mode_start  == SET)
//                {
//                    /*..hui [23-4-7���� 11:16:02] 20�� ����..*/
//                    mf32_cold_on_temp = 10.0f;
//                    mf32_cold_off_temp = 5.8f;
//                    mu16_cold_delay_time = 20;
//                }
//                else
//                {
                    /*..hui [23-4-7���� 11:16:02] 20�� ����..*/
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
            {   // �ܱ� 30�� ����
//                if (bit_sleep_mode_start  == SET)
//                {
//                    /*..hui [23-4-7���� 11:16:10] 30�� ����..*/
//                    mf32_cold_on_temp = 10.0f;
//                    mf32_cold_off_temp = 5.8f;
//                    mu16_cold_delay_time = 20;
//                }
//                else
//                {
                    /*..hui [23-4-7���� 11:16:10] 30�� ����..*/
                    mf32_cold_on_temp = 7.5f;
                    mf32_cold_off_temp = 5.8f;
                    mu16_cold_delay_time = 20;
//                }
            }
            else
            {
//                if (bit_sleep_mode_start  == SET)
//                {
//                    /*..hui [23-4-7���� 11:16:14] 30�� �ʰ�..*/
//                    mf32_cold_on_temp = 10.0f;
//                    mf32_cold_off_temp = 5.8f;
//                    mu16_cold_delay_time = 3000;
//                }
//                else
//                {
                    /*..hui [23-4-7���� 11:16:14] 30�� �ʰ�..*/
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
            /*..hui [25-3-19���� 2:22:52] �켱 �ü� �༳���� 14�� 10����..*/
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

                /*..hui [25-3-19���� 4:48:38] �⵿ ���� �� �ü��µ� 20�� �̻�, �ʱ� �⵿..*/
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

                    /*..hui [24-4-12���� 11:43:45] �ð� �Ϸ�� ���� Ƚ�� �ʱ�ȭ ���ʿ���..*/
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

    /*..hui [19-7-25���� 5:40:39] 2�� ���� ������Ʈ OFF ��ġ�̵�..*/
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


/// @brief      ���� �ð� ���� �Ǵ� �Լ�
/// @details    ���� �ð� ���� �Ǵ� ��� �Ѵ�
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


/// @brief      �ü� �ð� �켱 ���� �Լ�
/// @details    �ü� �ð� �켱 ���� ��� �Ѵ�
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
            /*..hui [19-7-25���� 4:21:53] �ð� �� �ð� ���� ���� �� ����/�ð� ���� �� Ȯ��..*/
            if(u16ColdOperation != COLD_MAKE_GOGOGO)
            {
                gu8_Make_Mode = MAKE_STOP;
            }
            else
            {

            }
            break;

        case MAKE_ICE:
            /*..hui [19-7-25���� 4:22:10] ���� �� ���� �Ϸ� �� ����/�ð� ���� �� Ȯ��..*/
            /*if(gu8IceStep == STATE_51_FINISH_ICE_MAKE)*/
            if (mu8IceStep_Buf == STATE_0_STANDBY)
            {
                /*..hui [20-1-21���� 11:15:35] ���� ��� �� ��� ���¿��� �ð� ���� �߻� ��..*/
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


/// @brief      ���� �ð� �켱 ���� �Լ�
/// @details    ���� �ð� �켱 ���� ��� �Ѵ�
/// @param      void
/// @return     void
void ice_priority_decision(void)
{
    U8 mu8IceStep_Buf = 0;

    mu8IceStep_Buf = Get_IceStep();

    switch (gu8_Make_Mode)
    {
        case MAKE_STOP:
            /*..hui [19-7-26���� 8:09:21] ���� �켱�̹Ƿ� �����ɶ����� ���� ����..*/
            if (u16IceOperation == ICE_MAKE_GOGOGO)
            {
                gu8_Make_Mode = MAKE_ICE;
            }
            else
            {
                /*..hui [19-7-26���� 8:09:34] ���� ���� �ƴҶ��� �ð� ����..*/
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
                /*..hui [19-11-6���� 1:57:36] �ð� �� ���� ���� �߻� ��..*/
                gu8_Make_Mode = MAKE_STOP;
            }
            else if (u16ColdOperation != COLD_MAKE_GOGOGO)
            {
                /*..hui [19-11-6���� 1:57:57] �ð� �� �ð� ���� ���� ��..*/
                gu8_Make_Mode = MAKE_STOP;
            }
            else
            {

            }
            break;

        case MAKE_ICE:
            /*..hui [19-11-6���� 1:59:36] ���� �� ���� �Ϸ� �� ����/�ð� ���� �� Ȯ��..*/
            /*if(gu8IceStep == STATE_51_FINISH_ICE_MAKE)*/
            if (mu8IceStep_Buf == STATE_0_STANDBY)
            {
                /*..hui [19-12-18���� 4:06:39] ���� ����� ��� ���¿��� ���� ��� �������� �߻� ��..*/
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
/// @details    Ice Making Mode Decision ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_IceMaking_ModeDecision_Module_Initialize(void)
{
#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)
    Lib_ModeDecision_Initialize();
#endif
}


/// @brief      Lib Ice Making Mode Decision Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� Ice Making Mode Decision ���� ��� Library
/// @param      void
/// @return     void
void Lib_IceMaking_ModeDecision_Module_1ms_Control(void)
{
#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)
    IceMakingModeDecisionControlTimer();
#endif
}


/// @brief      Lib Ice Making Mode Decision Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Ice Making Mode Decision ���� ��� Library
/// @param      void
/// @return     void
void Lib_IceMaking_ModeDecision_Module_Control(void)
{
#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)
    Make_Mode_Decision();
#endif
}

