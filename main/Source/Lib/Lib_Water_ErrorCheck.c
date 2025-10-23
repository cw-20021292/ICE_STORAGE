/// @file     Lib_Water_ErrorCheck.c
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    ������ Error ���� ���� file

#include "Global_Header.h"

U8 gu8ErrorCheckTimer = 0;                      /// @brief  ���� üũ�� �ϴ� �ֱ� Ÿ�̸�

#if ( (FLOW_SENSOR_ERROR_CHECK_USE == USE) || (WATER_CUT_OFF_ERROR_CHECK_USE == USE) )
U8 gu8FlowMeterErrorCheckStart = 0;             /// @brief  �������� ���� ���� üũ ���� ���� - 0(üũ �̽���), 1(üũ ����)
U16 gu16FlowMeterErrorChedkDelayTime = 0;       /// @brief  �������� ���� ���� üũ �� ���� �ð�@100ms
#endif

#if (EFFLUENT_CLOGGING_ERROR_CHECK_USE == USE)
U8 gu8EffluentCloggingErrorCheckStart = 0;      /// @brief  ���� ���� ���� üũ ���� ���� - 0(üũ �̽���), 1(üũ ����)
#endif

#if (HOT_CUT_OFF_ERROR_CHECK_USE == USE)
U8 gu8FHotCutOffCheckStart = 0;                 /// @brief  �¼� �ܼ� �� ��� ���� ���� üũ ���� ���� - 0(üũ �̽���), 1(üũ ����)
#endif

#if (BLDC_COMP_COMMUNICATION_ERROR_CHECK_USE == USE)
U8 gu8BLDC_CompCommunicationErrorDetect = 0;    /// @brief  BLDC Comp�� ��Ž� ��� �̻� ���� ���� - 0(�̰���), 1(����)
U8 gu8BLDC_CompErrorCodeRecieve = 0;            /// @brief  BLDC Comp�� ��Ž� ���� ���� �ڵ� ���� ���� - 0(�̼���), 1(����)
U8 gu8BLDC_ComoErrorCode = 0;                   /// @brief  BLDC Comp�� ��Ž� ������ ���� �ڵ�
#endif

#if (MICRO_SW_ALL_DETECT_ERROR_CHECK_USE == USE)
U16 gu16DualCheckDelay = 0;                     /// @brief  ����ũ�� SW ���� ���� ���� Ȯ�� ���� �������� ���۰� Delay�ð�
U8 gu8DualTrayStep = 0;                         /// @brief  ����ũ�� SW ���� ���� ���� Ȯ�� �Ǵ� ���� ���� �ܰ�
U8 gu8DualTrayCheckCount = 0;                   /// @brief  ����ũ�� SW ���� ���� ���� Ȯ�� �Ǵ��� ���� ���� Ƚ�� ī��Ʈ

U8 gu8Err_tray_down_acc_count = 0;              /// @brief  Ʈ���� �Ʒ��� �̵� ���� ���� ī��Ʈ
U8 gu8Tray_up_moving_retry_timer = 0;           /// @brief  Ʈ���� ���� �̵� ��õ� �ð�
U8 gu8Err_Tray_Up_Count = 0;                    /// @brief  Ʈ���� ���� ���� �̵� ��õ� Ƚ��
U8 gu8Err_Tray_Down_Count = 0;                  /// @brief  Ʈ���� Ż�� ���� �̵� ��õ� Ƚ��
U16 gu16Tray_down_moving_retry_timer = 0;       /// @brief  Ʈ���� �Ʒ��� �̵� ��õ� �ð�
U16 gu16Err_Tray_Motor_Down_Total_Tmr = 0;
U16 gu16Err_Tray_Motor_Up_Total_Tmr = 0;
U8 gu8Tray_Motor_Direction = 0;                 /// @brief  Ʈ���� ���� �̵� ���� - 0(Stop), 1(CW), 2(CCW)
U8 gu8Tray_up_contiunue_timer = 0;
#endif

/// @brief      ���� ���� ���� ������
/// @details    ���� ������ ���õ� ��� �ϱ� ���� ���� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    U8 gu8CheckCount[ERROR_ID_MAX];             /// @brief  ���� ���� Ƚ��
    U8 gu8Detect[ERROR_ID_MAX];                 /// @brief  ���� ���� ���� - 0(�̰���), 1(����)
    U8 gu8Status[ERROR_ID_MAX];                 /// @brief  ���� �߻� Ȯ�� ���� - 0(�̹߻�), 1(�߻�)
}   SErrorData_T;

SErrorData_T    SError;


/// @brief      ������ ���� �߻� ���� ������ �ʱ�ȭ
/// @details    ������ ���� �߻� ���� �����͵��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_WaterErrorCheck_Initialize(void)
{
    gu8ErrorCheckTimer = 0;

#if ( (FLOW_SENSOR_ERROR_CHECK_USE == USE) || (WATER_CUT_OFF_ERROR_CHECK_USE == USE) )
    gu8FlowMeterErrorCheckStart = 0;
    gu16FlowMeterErrorChedkDelayTime = 0;        /// @brief  �������� ���� ���� üũ �� ���� �ð�@100ms
#endif

#if (EFFLUENT_CLOGGING_ERROR_CHECK_USE == USE)
    gu8EffluentCloggingErrorCheckStart = 0;
#endif

#if (HOT_CUT_OFF_ERROR_CHECK_USE == USE)
    gu8FHotCutOffCheckStart = 0;
#endif

#if (BLDC_COMP_COMMUNICATION_ERROR_CHECK_USE == USE)
    gu8BLDC_CompCommunicationErrorDetect = 0;
    gu8BLDC_CompErrorCodeRecieve = 0;
    gu8BLDC_ComoErrorCode = 0;
#endif

#if (MICRO_SW_ALL_DETECT_ERROR_CHECK_USE == USE)
    gu16DualCheckDelay = 0;
    gu8DualTrayStep = 0;
    gu8DualTrayCheckCount = 0;

    gu8Err_tray_down_acc_count = 0;
    gu8Tray_up_moving_retry_timer = 0;
    gu8Err_Tray_Up_Count = 0;
    gu8Err_Tray_Down_Count = 0;
    gu16Tray_down_moving_retry_timer = 0;
    gu16Err_Tray_Motor_Down_Total_Tmr = 0;
    gu16Err_Tray_Motor_Up_Total_Tmr = 0;
    gu8Tray_Motor_Direction = 0;
    gu8Tray_up_contiunue_timer = 0;
#endif

    MEMSET( (void __FAR *)&SError, 0x00, sizeof(SErrorData_T) );
}


/// @brief      ������ ���� üũ ���� Ÿ�̸�(@1ms)
/// @details    ������ ���� üũ ������ �� �ֱ⸦ Ÿ�̸ӷ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void WaterErrorCheckTimer(void)
{
    if (gu8ErrorCheckTimer < ERROR_CHECK_PERIOD_TIME)
    {
        gu8ErrorCheckTimer++;
    }
}


/// @brief      ������ ���� üũ ���� �Լ�(@while)
/// @details    ������ ���� üũ ������ �ֱ������� �����Ѵ�
/// @param      void
/// @return     void
void Lib_WaterErrorCheck(void)
{
    U8 mu8BootComplete = 0;

    if (gu8ErrorCheckTimer >= ERROR_CHECK_PERIOD_TIME)
    {   // 100ms ���� Ȯ��
        gu8ErrorCheckTimer = 0;

        mu8BootComplete = Get_BootComplete();   // ���� ������ �Ϸ�Ǿ����� Ȯ��

        if (mu8BootComplete == SET)
        {   // ���� ������ �Ϸ�Ǿ�����
            LeakageErrorCheck();                    // ���� ���� üũ
            FlowMeterErrorCheck_BuiltInUse();       // ��������1, 2 ���� üũ
            WaterCutOffErrorCheck();                // �ܼ� ���� üũ
            RoomOverHeatErrorCheck();               // ���� ���� ���� üũ
            FilterCloggingErrorCheck();             // ���� ���� ���� üũ
            EffluentCloggingErrorCheck();           // ���� ����(��������3) ���� üũ
            HotCutOffErrorCheck();                  // �¼� �ܼ� �� ��� ����(�����¼�) ���� üũ
            BoostPumpErrorCheck();                  // �ν�Ʈ ���� ���� ���� üũ
            PressureSensorErrorCheck();             // �з� ���� ���� üũ
            TrayMotorErrorCheck();                  // ���� Ʈ���� ���� ���� üũ
            MicroSW_AllDetectErrorCheck();          // ����ũ�� ����ġ ���� ���� ���� üũ
            MicroSW_IceMakingErrorCheck();          // ����ũ�� ����ġ �̵� ���� üũ
            DrainPumpErrorCheck();                  // �巹�� ���� ���� üũ
            SodaPumpErrorCheck();                   // ź�� ���� ���� üũ
            AbnormalEffluentErrorCheck();           // ���� ���� �̻� ���� üũ
            RoomLevelErrorCheck();                  // ���� ��ũ ���� �������� ���� üũ
            HotHighLevelErrorCheck();               // �¼� ������ ���� ���� üũ
            IceColdWaterTankLevelErrorCheck();      // ������ũ ���� ���� ���� üũ
            ColdLevelErrorCheck();                  // �ü� ��ũ ���� �������� ���� üũ
            RoomTempErrorCheck();                   // ���� �µ� ���� ���� üũ
            HotTempErrorCheck();                    // �¼� �µ� ���� ���� üũ
            ColdTempErrorCheck();                   // �ü� �µ� ���� ���� üũ
            IceTrayInputTempErrorCheck();           // ���̽� Ʈ���� �Լ� �µ� ���� ���� üũ
            AirTempErrorCheck();                    // �ܱ� �µ� ���� ���� üũ
            HeatsinkTempErrorCheck();               // TEM �濭�� �µ� ���� ���� üũ
            HotOverheatErrorCheck();                // �¼� ���� ���� üũ
            HotPumpErrorCheck();                    // �¼� ���� ���� üũ
            BLDC_CompErrorCheck();                  // BLDC Comp ���� üũ
            CompActErrorCheck();                    // �Ϲ� Comp ���� ���� üũ
            CirculatePumpErrorCheck();              // ��ȯ ���� ���� üũ
            StirringMoterErrorCheck();              // ���� ���� ���� üũ
            EW_ModuleErrorCheck();                  // EW ��� ��� ���� üũ
            EW_DrainPumpErrorCheck();               // EW ��� �巹�� ���� üũ
            UVErrorCheck();                         // UV ���� üũ
            TDS_In_TempErrorCheck();                // TDS IN �µ� ���� ���� üũ
            TDS_Out_TempErrorCheck();               // TDS OUT �µ� ���� ���� üũ
            TDS_InErrorCheck();                     // TDS IN ���� ���� üũ
            TDS_OutErrorCheck();                    // TDS OUT ���� ���� üũ
            InputContaminationErrorCheck();         // ���� ���� ���� üũ
            DecontaminationErrorCheck();            // ������ �ҷ� ���� üũ
        }
    }
}


/// @brief      ���� ���� üũ
/// @details    ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void LeakageErrorCheck(void)
{
#if (LEAKAGE_ERROR_CHECK_USE == USE)
    static U16 mu16LeakageErrorDetectTimer = 0;
    static U16 mu16LeakageErrorUndetectTimer = 0;
    U16 mu16AD_Data = 0;

    mu16AD_Data = Get_ADC_Data(ADC_ID_LEAK_SENSOR);

    if (mu16AD_Data > 615)
    {
        mu16LeakageErrorUndetectTimer = 0;

        if (SError.gu8Status[ERROR_ID_LEAKAGE_E01] == CLEAR)
        {   // ���� ������ �߻����� ���� �����̸�
            if (mu16LeakageErrorDetectTimer >= LEAKAGE_ERROR_DETECT_TIME)
            {   // ���� ������ 3�� �̻� �����Ǹ�
                mu16LeakageErrorDetectTimer = 0;

                Set_ErrorStatus(ERROR_ID_LEAKAGE_E01, SET);
            }
            else
            {
                mu16LeakageErrorDetectTimer++;
            }
        }
        else
        {
            mu16LeakageErrorDetectTimer = 0;
        }
    }
    else
    {
        mu16LeakageErrorDetectTimer = 0;

        if (SError.gu8Status[ERROR_ID_LEAKAGE_E01] == SET)
        {   // ���� ������ �߻��� �����̸�
            if (mu16LeakageErrorUndetectTimer >= LEAKAGE_ERROR_UNDETECT_TIME)
            {
                mu16LeakageErrorUndetectTimer = 0;

                Set_ErrorStatus(ERROR_ID_LEAKAGE_E01, CLEAR);
            }
            else
            {
                mu16LeakageErrorUndetectTimer++;
            }
        }
        else
        {
            mu16LeakageErrorUndetectTimer = 0;
        }
    }
#endif
}


#if ( (FLOW_SENSOR_ERROR_CHECK_USE == USE) || (WATER_CUT_OFF_ERROR_CHECK_USE == USE) )
/// @brief      ���� ���� ���� ���� üũ ���� ���� ���� �Լ�
/// @details    ���� ���� ���� ���� üũ�� ������ �������� ���� ���°��� �����Ѵ�
/// @param      mu8Status : ���� üũ ���� ���°� - 0(�̽���), 1(����)
/// @return     void
void Set_FlowMeterErrorCheckStart(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8FlowMeterErrorCheckStart = SET;
    }
    else
    {
        gu8FlowMeterErrorCheckStart = CLEAR;
    }
}


/// @brief      ���� ���� ���� ���� üũ ���� ���� Ȯ�� �Լ�
/// @details    ���� ���� ���� ���� üũ�� ������ �������� ���� ���°��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� üũ ���� ���°� - 0(�̽���), 1(����)
U8 Get_FlowMeterErrorCheckStart(void)
{
    if (gu8FlowMeterErrorCheckStart == SET)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ���� ���� ���� üũ ���� �� ���� �ð� ���� �Լ�
/// @details    ���� ���� ���� ���� üũ ���� �� ���� ������ �� �ð��� �����Ѵ�
/// @param      mu16Time : ���� �ð� ��(100ms����)
/// @return     void
void Set_FlowMeterErrorCheckDelayTime(U16 mu16Time)
{
    gu16FlowMeterErrorChedkDelayTime = mu16Time;
}


/// @brief      ���� ���� ���� ���� üũ ���� �� ���� �ð� Ȯ�� �Լ�
/// @details    ���� ���� ���� ���� üũ ���� �� ���� ������ �� �ð��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �ð� ��(100ms����)
U16 Get_FlowMeterErrorCheckDelayTime(void)
{
    return  gu16FlowMeterErrorChedkDelayTime;
}

#endif


/// @brief      ��������1, 2 ���� üũ
/// @details    ��Ʈ�� �𵨿��� ���������� 2�� ����ϴ� ��� ��������1, 2�� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void FlowMeterErrorCheck_BuiltInUse(void)
{
#if (FLOW_SENSOR_ERROR_CHECK_USE == USE)
    static U16 mu16ErrorCheckDelay = 0;
    static U16 mu16ErrorDetectTime1 = 0;
    static U16 mu16ErrorDetectTime2 = 0;
    U8 mu8ErrorCheckStart = 0;
    U8 mu8Flow1_Hz = 0;
    U8 mu8Flow2_Hz = 0;

    mu8ErrorCheckStart = Get_FlowMeterErrorCheckStart();        // �������� ���� üũ ���� ���� Ȯ��
    mu8Flow1_Hz = Get_Flowmeter_Hz(FLOWMETER_1_ID);
//    mu8Flow2_Hz = Get_Flowmeter_Hz(FLOWMETER_2_ID);

    if (mu8ErrorCheckStart == TRUE)
    {   // ���� üũ ���� ���̸�
        if (mu16ErrorCheckDelay >= FLOW_SENSOR_ERROR_CHECK_DELAY_TIME)
        {
            if (SError.gu8Status[ERROR_ID_FLOW_SENSOR_1_E05] == CLEAR)
            {   // ��������1 ���� �̹߻� ����
                if ( (mu8Flow1_Hz < FLOW_SENSOR_ERROR_HZ) &&
                     (mu8Flow2_Hz > FLOW_SENSOR_ERROR_HZ) )
                {
                    if (mu16ErrorDetectTime1 >= FLOW_SENSOR_ERROR_DETECT_TIME)
                    {
                        mu16ErrorDetectTime1 = 0;

                        Set_ErrorStatus(ERROR_ID_FLOW_SENSOR_1_E05, SET);
                    }
                    else
                    {
                        mu16ErrorDetectTime1++;
                    }
                }
                else
                {
                    mu16ErrorDetectTime1 = 0;
                }
            }
            else
            {   // ��������1 ������ �߻��� ����
                mu16ErrorDetectTime1 = 0;
            }

            if (SError.gu8Status[ERROR_ID_FLOW_SENSOR_2_E06] == CLEAR)
            {   // ��������2 ���� �̹߻� ����
                if ( (mu8Flow2_Hz < FLOW_SENSOR_ERROR_HZ) &&
                     (mu8Flow1_Hz > FLOW_SENSOR_ERROR_HZ) )
                {
                    if (mu16ErrorDetectTime2 >= FLOW_SENSOR_ERROR_DETECT_TIME)
                    {
                        mu16ErrorDetectTime2 = 0;

                        Set_ErrorStatus(ERROR_ID_FLOW_SENSOR_2_E06, SET);
                    }
                    else
                    {
                        mu16ErrorDetectTime2++;
                    }
                }
                else
                {
                    mu16ErrorDetectTime2 = 0;
                }
            }
            else
            {   // ��������2 ������ �߻��� ����
                mu16ErrorDetectTime2 = 0;
            }
        }
        else
        {
            mu16ErrorCheckDelay++;
        }
    }
    else
    {   // ���� üũ ���� ���� �ƴϸ�
        mu16ErrorCheckDelay = 0;
        mu16ErrorDetectTime1 = 0;
        mu16ErrorDetectTime2 = 0;
    }
#endif
}


/// @brief      �ܼ� ���� üũ
/// @details    �ܼ� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void WaterCutOffErrorCheck(void)
{
#if (WATER_CUT_OFF_ERROR_CHECK_USE == USE)
    static U16 mu16ErrorCheckDelay = 0;
    static U16 mu16ErrorDetectTime = 0;
    static U16 mu16ErrorAutoClearTime = 0;
    U16 mu16ErrorCheckDelayTime = 0;
    U8 mu8ErrorCheckStart = 0;
    U8 mu8Flow_Hz = 0;

    mu8ErrorCheckStart = Get_FlowMeterErrorCheckStart();            // �������� ���� üũ ���� ���� Ȯ��
    mu8Flow_Hz = Get_Flowmeter_Hz(FLOWMETER_1_ID);
    mu16ErrorCheckDelayTime = Get_FlowMeterErrorCheckDelayTime();   // ���� üũ ���� �� ���� �ð� Ȯ��

    if (mu8ErrorCheckStart == TRUE)
    {   // ���� üũ ���� ���̸�
        if (mu16ErrorCheckDelay >= mu16ErrorCheckDelayTime)
        {   // ���� üũ �� ���� �ð� ����
            if (SError.gu8Status[ERROR_ID_WATER_CUT_OFF_E09] == CLEAR)
            {   // �ܼ� ���� �̹߻� ����
                mu16ErrorAutoClearTime = 0;     // �ڵ� ���� Ÿ�̸� CLEAR

                if (mu8Flow_Hz < WATER_CUT_OFF_HZ)
                {   // ���� ���� �̸��̸�
                    if (mu16ErrorDetectTime >= WATER_CUT_OFF_DETECT_TIME)
                    {
                        mu16ErrorDetectTime = 0;

                        Set_ErrorStatus(ERROR_ID_WATER_CUT_OFF_E09, SET);
                    }
                    else
                    {
                        mu16ErrorDetectTime++;
                    }
                }
                else
                {   // ���� ���� �̻��̸�
                    mu16ErrorDetectTime = 0;
                }
            }
            else
            {   // �ܼ� ���� �߻� ����
                mu16ErrorDetectTime = 0;
            }
        }
        else
        {   // ���� üũ �� ���� �ð� ����
            mu16ErrorCheckDelay++;
        }
    }
    else
    {   // ���� üũ ���� ���� �ƴϸ�
        mu16ErrorCheckDelay = 0;
        mu16ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_WATER_CUT_OFF_E09] == SET)
        {   // �ܼ� ������ �߻��� ����
            if (mu16ErrorAutoClearTime >= WTAER_CUT_OFF_AUTO_CLEAR_TIME)
            {   // ���� �ð� �� �ڵ� ����
                mu16ErrorAutoClearTime = 0;

                Set_ErrorStatus(ERROR_ID_WATER_CUT_OFF_E09, CLEAR);
            }
            else
            {
                mu16ErrorAutoClearTime++;
            }
        }
        else
        {   // �ܼ� ������ �߻����� ���� ����
            mu16ErrorAutoClearTime = 0;
        }
    }
#endif
}


/// @brief      ���� ���� ���� üũ
/// @details    ���� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void RoomOverHeatErrorCheck(void)
{
#if (ROOM_WATER_OVERHEAT_ERROR_CHECK_USE == USE)
    F32 mf32RoomTemp = 0;
    static U16 mu16RoomOverheatDetectTime = 0;

    mf32RoomTemp = Get_Temp(ROOM_THERMISTOR_ID);

    if (mf32RoomTemp >= 50.0f)
    {   // ���� �µ��� 50�� �̻��̸�
        if (mu16RoomOverheatDetectTime >= ROOM_WATER_OVERHEAT_DETECT_TIME)
        {
            Set_ErrorStatus(ERROR_ID_ROOM_OVERHEAT_E49, SET);
        }
        else
        {
            mu16RoomOverheatDetectTime++;
        }
    }
    else
    {   // ���� �µ��� 50�� �̸��̸�
        Set_ErrorStatus(ERROR_ID_ROOM_OVERHEAT_E49, CLEAR);
        mu16RoomOverheatDetectTime = 0;
    }
#endif
}


/// @brief      ���� ���� ���� üũ(���� ���� ��� ǥ�� ���� ������ ���� ���� - �̻��)
/// @details    ���� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void FilterCloggingErrorCheck(void)
{
#if (FILTER_CLOGGING_ERROR_CHECK_USE == USE)

#endif
}


#if (EFFLUENT_CLOGGING_ERROR_CHECK_USE == USE)

/// @brief      ���� ���� ���� üũ ���� ���� ���� �Լ�
/// @details    ���� ���� ���� üũ�� ������ �������� ���� ���°��� �����Ѵ�
/// @param      mu8Status : ���� üũ ���� ���°� - 0(�̽���), 1(����)
/// @return     void
void Set_EffluentCloggingErrorCheckStart(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8EffluentCloggingErrorCheckStart = SET;
    }
    else
    {
        gu8EffluentCloggingErrorCheckStart = CLEAR;
    }
}


/// @brief      ���� ���� ���� üũ ���� ���� Ȯ�� �Լ�
/// @details    ���� ���� ���� üũ�� ������ �������� ���� ���°��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� üũ ���� ���°� - 0(�̽���), 1(����)
U8 Get_EffluentCloggingErrorCheckStart(void)
{
    if (gu8EffluentCloggingErrorCheckStart == SET)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}

#endif


/// @brief      ���� ����(��������3 ����) ���� üũ
/// @details    ���� ����(��������3 ����) ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void EffluentCloggingErrorCheck(void)
{
#if (EFFLUENT_CLOGGING_ERROR_CHECK_USE == USE)

#if (EFFLUENT_CLOGGING_ERROR_FLOWMETER_USE == USE)

    // ���������� ����ϴ� ���
    static U16 mu16ErrorCheckDelay = 0;
    static U16 mu16ErrorDetectTime = 0;
    U8 mu8ErrorCheckStart = 0;
    U8 mu8Flow_Hz = 0;

    mu8ErrorCheckStart = Get_EffluentCloggingErrorCheckStart();     // ���� ����(��������3) ���� üũ ���� ���� Ȯ��
    mu8Flow_Hz = Get_Flowmeter_Hz(FLOWMETER_3_ID);

    if (mu8ErrorCheckStart == TRUE)
    {   // ���� üũ ���� ���̸�
        if (mu16ErrorCheckDelay >= EFFLUENT_CLOGGING_CHECK_DELAY_TIME)
        {   // ���� üũ ���� �� ���� ����
            if (SError.gu8Status[ERROR_ID_EFFLUENT_CLOGGING_E04] == CLEAR)
            {   // ���� �̹߻� ����
                if (mu8Flow_Hz < EFFLUENT_CLOGGING_HZ)
                {
                    if (mu16ErrorDetectTime >= EFFLUENT_CLOGGING_DETECT_TIME)
                    {
                        mu16ErrorDetectTime = 0;

                        Set_ErrorStatus(ERROR_ID_EFFLUENT_CLOGGING_E04, SET);
                    }
                    else
                    {
                        mu16ErrorDetectTime++;
                    }
                }
                else
                {
                    mu16ErrorDetectTime = 0;
                }
            }
        }
        else
        {   // ���� üũ ���� �� ���� ����
            mu16ErrorCheckDelay++;
        }
    }
    else
    {   // ���� üũ ���� ���� �ƴϸ�
        mu16ErrorCheckDelay = 0;
        mu16ErrorDetectTime = 0;
    }

#else

    // ���������� ������� �ʴ� ��� - �¼��� �ü��� ȥ���Ͽ� ������ �����ϴ� ������ ��ǰ���� ���� üũ ���
    static U16 mu16ErrorDetectTime = 0;                 // ���� ���� üũ �ð�
    static U16 mu16ErrorAutoClearTime = 0;              // ���� �ڵ� ���� üũ �ð�
    U8 mu8ErrorCheckStart = 0;
    static F32 mf32EffluentStartTemp = 0;               // ���� ���۽� ����� �µ�
    static F32 mf32EffluentStartColdTemp = 0;           // ���� ���۽� �ü� �µ�
    F32 mf32Temp = 0;

    mu8ErrorCheckStart = Get_EffluentCloggingErrorCheckStart();     // ���� ���� ���� üũ ���� ���� Ȯ��
    mf32Temp = Get_Temp(FAUCET_OUT_THERMISTOR_ID);                  // ���� ����� �µ�

    if (mu8ErrorCheckStart == TRUE)
    {   // ���� üũ ���� ���̸�
        if (mf32EffluentStartTemp == 0)
        {   // ���� ���۽� ����� �µ��� 0�̸�((= üũ�ϱ� ���̸�)
            mf32EffluentStartTemp = Get_Temp(FAUCET_OUT_THERMISTOR_ID);
        }

        if (mf32EffluentStartColdTemp == 0)
        {   // ���� ���۽� üũ�� �ü� �µ��� 0�̸�(= üũ�ϱ� ���̸�)
            mf32EffluentStartColdTemp = Get_Temp(COOL_THERMISTOR_1_ID);
        }

        if (SError.gu8Status[ERROR_ID_EFFLUENT_CLOGGING_E04] == CLEAR)
        {   // ���� �̹߻� ����
            mu16ErrorAutoClearTime = 0;     // �ڵ� ���� Ÿ�̸� CLEAR

            if (mf32Temp > 55.0f)
            {
                if (mf32EffluentStartTemp >= EFFLEUNT_CLOGGING_START_STANDARD_TEMP)
                {   // ���� ���۽� �µ��� 70�� �̻��� ���
                    if (mu16ErrorDetectTime >= EFFLUENT_CLOGGING_DETECT_TIME_OVER_STANDARD)
                    {
                        mu16ErrorDetectTime = 0;

                        Set_ErrorStatus(ERROR_ID_EFFLUENT_CLOGGING_E04, SET);
                    }
                    else
                    {
                        mu16ErrorDetectTime++;
                    }
                }
                else
                {   // ���� ���۽� �µ��� 70�� �̸��� ���
                    if (mu16ErrorDetectTime >= EFFLUENT_CLOGGING_DETECT_TIME_UNDER_STANDARD)
                    {
                        mu16ErrorDetectTime = 0;

                        Set_ErrorStatus(ERROR_ID_EFFLUENT_CLOGGING_E04, SET);
                    }
                    else
                    {
                        mu16ErrorDetectTime++;
                    }
                }
            }
            else
            {
                mu16ErrorDetectTime = 0;
            }
        }
        else
        {   // ���� �߻� ����
            if ( (mf32EffluentStartColdTemp <= EFFLUENT_CLOGGING_AUTO_CLEAR_COLD_TEMP) &&
                 (mf32EffluentStartTemp >= EFFLUENT_CLOGGING_AUTO_CLEAR_TEMP) )
            {   // ���� ���۽� �ü� �µ��� 10�� ����, ����� �µ��� 15�� �̻��� ����
                if ( mf32Temp < (mf32EffluentStartTemp - 5.0f) )
                {   // ���� ���۽� ����� �µ����� 5�� drop�� �µ��� ������ ���
                    if (mu16ErrorAutoClearTime >= EFFLUENT_CLOGGING_AUTO_CLEAR_TIME)
                    {
                        mu16ErrorAutoClearTime = 0;

                        Set_ErrorStatus(ERROR_ID_EFFLUENT_CLOGGING_E04, CLEAR);
                    }
                    else
                    {
                        mu16ErrorAutoClearTime++;
                    }
                }
            }
            else
            {
                mu16ErrorAutoClearTime = 0;
            }
        }
    }
    else
    {   // ���� üũ ���� ���� �ƴϸ�
        mu16ErrorDetectTime = 0;
        mu16ErrorAutoClearTime = 0;
        mf32EffluentStartTemp = 0;               // ���� ���۽� ����� �µ�
        mf32EffluentStartColdTemp = 0;           // ���� ���۽� �ü� �µ�
        mf32Temp = 0;
    }
#endif

#endif
}


#if (HOT_CUT_OFF_ERROR_CHECK_USE == USE)

/// @brief      �¼� �ܼ� �� ��� ���� ���� üũ ���� ���� ���� �Լ�
/// @details    �¼� �ܼ� �� ��� ���� ���� üũ�� ������ �������� ���� ���°��� �����Ѵ�
/// @param      mu8Status : ���� üũ ���� ���°� - 0(�̽���), 1(����)
/// @return     void
void Set_HotCutOffCheckStart(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8FHotCutOffCheckStart = SET;
    }
    else
    {
        gu8FHotCutOffCheckStart = CLEAR;
    }
}


/// @brief      �¼� �ܼ� �� ��� ���� ���� üũ ���� ���� Ȯ�� �Լ�
/// @details    �¼� �ܼ� �� ��� ���� ���� üũ�� ������ �������� ���� ���°��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� üũ ���� ���°� - 0(�̽���), 1(����)
U8 Get_HotCutOffCheckStart(void)
{
    if (gu8FHotCutOffCheckStart == SET)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}

#endif


/// @brief      �¼� �ܼ� �� ��� ����(�����¼�) ���� üũ
/// @details    �¼� �ܼ� �� ��� ����(�����¼�) ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void HotCutOffErrorCheck(void)
{
#if (HOT_CUT_OFF_ERROR_CHECK_USE == USE)
    static U16 mu16ErrorCheckDelay = 0;
    static U16 mu16ErrorDetectTime = 0;
    static U16 mu16ErrorAutoClearTime = 0;
    U8 mu8ErrorCheckStart = 0;
    U8 mu8Flow_Hz = 0;

    mu8ErrorCheckStart = Get_HotCutOffCheckStart();        // �������� ���� üũ ���� ���� Ȯ��
    mu8Flow_Hz = Get_Flowmeter_Hz(FLOWMETER_1_ID);

    if (mu8ErrorCheckStart == TRUE)
    {   // ���� üũ ���� ���̸�
        if (mu16ErrorCheckDelay >= HOT_CUT_OFF_ERROR_CHECK_DELAY_TIME)
        {   // ���� üũ ���� �� ���� �ð� ����
            if (SError.gu8Status[ERROR_ID_HOT_CUT_OFF_E08] == CLEAR)
            {   // ���� �̹߻� ����
                mu16ErrorAutoClearTime = 0;     // �ڵ� ���� Ÿ�̸� CLEAR

                if (mu8Flow_Hz < HOT_CUT_OFF_HZ)
                {
                    if (mu16ErrorDetectTime >= HOT_CUT_OFF_DETECT_TIME)
                    {
                        mu16ErrorDetectTime = 0;

                        Set_ErrorStatus(ERROR_ID_HOT_CUT_OFF_E08, SET);
                    }
                    else
                    {
                        mu16ErrorDetectTime++;
                    }
                }
                else
                {
                    mu16ErrorDetectTime = 0;
                }
            }
            else
            {   // ���� �߻� ����
                mu16ErrorDetectTime = 0;
            }
        }
        else
        {   // ���� üũ ���� �� ���� �ð� ����
            mu16ErrorCheckDelay++;
        }
    }
    else
    {   // ���� üũ ���� ���� �ƴϸ�
        mu16ErrorCheckDelay = 0;
        mu16ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_HOT_CUT_OFF_E08] == SET)
        {   // ������ �߻��� ����
            if (mu16ErrorAutoClearTime >= HOT_CUT_OFF_AUTO_CLEAR_TIME)
            {   // ���� �ð� �� �ڵ� ����
                mu16ErrorAutoClearTime = 0;

                Set_ErrorStatus(ERROR_ID_HOT_CUT_OFF_E08, CLEAR);
            }
            else
            {
                mu16ErrorAutoClearTime++;
            }
        }
        else
        {   // �ܼ� ������ �߻����� ���� ����
            mu16ErrorAutoClearTime = 0;
        }
    }
#endif
}


/// @brief      �ν�Ʈ ���� ����(����� ����) ���� üũ
/// @details    �ν�Ʈ ���� ����(����� ����) ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void BoostPumpErrorCheck(void)
{
#if (BOOST_PUMP_ERROR_CHECK_USE == USE)
    static U8 mu8ErrorDetectTime = 0;
    U8 mu8BoostPumpStatus = 0;
    U16 mu16BoostPumpFeedbackAD = 0;

    mu8BoostPumpStatus = Get_BoostPump_Status();                    // �ν�Ʈ ������ ���� ���� ���� Ȯ��
//    mu16BoostPumpFeedbackAD = Get_ADC_Data(ADC_ID_BOOST_PUMP);      // �ν�Ʈ ������ �ǵ�� AD�� Ȯ��

    if (mu8BoostPumpStatus == TRUE)
    {   // �ν�Ʈ ������ ���� ���� ���
        if ( (mu16BoostPumpFeedbackAD < BOOST_PUMP_ERROR_UNDER_STANDARD_AD) ||
             (mu16BoostPumpFeedbackAD > BOOST_PUMP_ERROR_OVER_STANDARD_AD) )
        {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
            if (mu8ErrorDetectTime >= BOOST_PUMP_ERROR_DETECT_TIME)
            {
                Set_ErrorStatus(ERROR_ID_BOOST_PUMP_E95, SET);
            }
            else
            {
                mu8ErrorDetectTime++;
            }
        }
        else
        {
            mu8ErrorDetectTime = 0;
        }
    }
    else
    {   // �ν�Ʈ ������ ���� ���� �ƴ� ���
        mu8ErrorDetectTime = 0;
    }
#endif
}


/// @brief      �з� ���� ���� üũ
/// @details    �з� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void PressureSensorErrorCheck(void)
{
#if (PRESSURE_SENSOR_ERROR_CHECK_USE == USE)

#endif
}


/// @brief      ���� Ʈ���� ���� ���� üũ(���� ���� ��� ǥ�� ���� ������ ���� ���� - �̻��)
/// @details    ���� Ʈ���� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void TrayMotorErrorCheck(void)
{
#if (TRAY_MOTOR_ERROR_CHECK_USE == USE)

#endif
}


/// @brief      ����ũ�� ����ġ ���� ���� ���� üũ
/// @details    ����ũ�� ����ġ ���� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void MicroSW_AllDetectErrorCheck(void)
{
#if (MICRO_SW_ALL_DETECT_ERROR_CHECK_USE == USE)
    U8 mu8IceTrayPositionBuf = 0;
    U8 mu8TrayMotorPreUpBuf = 0;

    mu8IceTrayPositionBuf = Get_IceTrayPosition();
    mu8TrayMotorPreUpBuf = Get_TrayMotorPreUpStatus();

    if (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_ERROR)
    {
        if (SError.gu8Detect[ERROR_ID_MICRO_SW_ALL_DETECT_E61] == CLEAR)
        {
            Set_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61, SET);     // ���� ������ Tray M/S Error ��Ȳ �߻� ���� ����
        }
    }

    if (SError.gu8Detect[ERROR_ID_MICRO_SW_ALL_DETECT_E61] == SET)
    {   // ���� ���°� ������ ���
        if (gu16DualCheckDelay > 0)
        {
            gu16DualCheckDelay--;
        }

        if (mu8TrayMotorPreUpBuf == TRUE)
        {
            gu8DualTrayStep = 1;
            Set_TrayMotorPreUpStatus(CLEAR);
        }

        switch (gu8DualTrayStep)
        {   // ���� Ȯ�� ���� �ܰ躰 ����
            case 0:
                run_up_ice_tray();

                gu16DualCheckDelay = 100;       // ���� 10�� �̵�
                gu8DualTrayStep++;              // 1.CW
                break;

            case 1:
                if (gu16DualCheckDelay == 0)
                {
                    run_stop_ice_tray();
                    gu16DualCheckDelay = 20;    // 2�� ����
                    gu8DualTrayStep++;          // 2.������ġ���� ����
                }
                else if ( (gu16DualCheckDelay <= 90) && (gu16DualCheckDelay >= 70) )
                {
                    if (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_MOVING)
                    {   // �̵��� Dual Open ����
                        gu8DualTrayStep = 0;
                        gu16DualCheckDelay = 0;
                        gu8DualTrayCheckCount = 0;
                        Set_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61, CLEAR);   // Tray �ν� �̵����̸� Tray M/S Error �ʱ�߻� ����
                        Set_IceStep(STATE_0_STANDBY);       // ���� ���� ù �ܰ�
                        Set_InitIceStep(0);                 // ���� �ʱ�ȭ ���� ù �ܰ�
                        Set_InitIceStatus(SET);             // ���� �ʱ�ȭ ���� ����
                    }
                    else
                    {

                    }
                }
                break;

            case 2:
                if (gu16DualCheckDelay == 0)
                {
                    /*..hui [18-2-1���� 11:19:15] Ʈ���� Ż�� ���� �̵�..*/
                    run_down_ice_tray();
                    gu16DualCheckDelay = 250;   // Ż�� 25�� �̵�
                    gu8DualTrayStep++;          // 3.CCW
                }
                else
                {

                }
                break;

            case 3:
                if (gu16DualCheckDelay == 0)
                {
                    run_stop_ice_tray();
                    gu16DualCheckDelay = 3000;      // 5�� ����
                    gu8DualTrayStep++;              // 4.Ż����ġ���� ����
                }
                else if ( (gu16DualCheckDelay <= 220) && (gu16DualCheckDelay >= 80) )
                {
                    if (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_MOVING)
                    {   // �̵��� Dual Open ����
                        gu8DualTrayStep = 0;
                        gu16DualCheckDelay = 0;
                        gu8DualTrayCheckCount = 0;
                        Set_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61, CLEAR);   // Tray �ν� �̵����̸� Tray M/S Error �ʱ�߻� ����
                        Set_IceStep(STATE_0_STANDBY);       // ���� ���� ù �ܰ�
                        Set_InitIceStep(0);                 // ���� �ʱ�ȭ ���� ù �ܰ�
                        Set_InitIceStatus(SET);             // ���� �ʱ�ȭ ���� ����
                    }
                }
                else
                {

                }
                break;

            case 4:
                if (gu16DualCheckDelay == 0)
                {
                    gu8DualTrayStep = 0;        // ���� Ȯ�� ���� 0�ܰ���� �ٽ� ����

                    if (++gu8DualTrayCheckCount >= 3)
                    {
                        gu8DualTrayCheckCount = 0;
                        Set_ErrorStatus(ERROR_ID_MICRO_SW_ALL_DETECT_E61, SET);     // 3ȸ �õ��� Error
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
                run_stop_ice_tray();
                break;
        }
    }
#endif
}

#if (MICRO_SW_ALL_DETECT_ERROR_CHECK_USE == USE)
/// @brief      ����ũ�� SW ���� ���� ���� Ȯ�� �Ǵ� ���� ���� �ܰ� ���� �Լ�
/// @details    ����ũ�� SW ���� ���� ���� Ȯ�� �Ǵ� ���� ���� ���� �ܰ踦 �����Ѵ�
/// @param      mu8Step : ����ũ�� SW ���� ���� ���� Ȯ�� �Ǵ� ���� ���� �ܰ�
/// @return     void
void Set_MicroSW_AllDetectErrorCheckStep(U8 mu8Step)
{
    gu8DualTrayStep = mu8Step;
}


/// @brief      ����ũ�� SW ���� ���� ���� Ȯ�� �Ǵ� ���� ���� �ܰ� Ȯ�� �Լ�
/// @details    ����ũ�� SW ���� ���� ���� Ȯ�� �Ǵ� ���� ���� ���� �ܰ踦 Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ����ũ�� SW ���� ���� ���� Ȯ�� �Ǵ� ���� ���� �ܰ�
U8 Get_MicroSW_AllDetectErrorCheckStep(void)
{
    return  gu8DualTrayStep;
}
#endif



/// @brief      ����ũ�� ����ġ �̵� ���� üũ
/// @details    ����ũ�� ����ġ �̵� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void MicroSW_IceMakingErrorCheck(void)
{
#if (MICRO_SW_ICE_MAKING_ERROR_CHECK_USE == USE)
    U8 mu8TrayMotorDownBuf = 0;
    U8 mu8TrayMotorUpBuf = 0;
    U8 mu8TrayCCW_DelayEnd = 0;
    U8 mu8TrayCW_DelayEnd = 0;
    U8 mu8IceStepBuf = 0;

    mu8TrayMotorDownBuf = Get_TrayMotorDownStatus();
    mu8TrayMotorUpBuf = Get_TrayMotorUpStatus();

    //------------------------------------------------- �������� ȸ��
    /*..hui [18-1-26���� 8:22:52] F_TrayMotorUP �� ���� F_TrayMotorCCW�� Ż����ġ.. ..*/
    /*..hui [18-1-26���� 8:23:08] F_TrayMotorUP �� �����δ� CCW�� ����..*/
    gu8Tray_Motor_Direction = ( ((mu8TrayMotorDownBuf << 1) & 0x02) | (mu8TrayMotorUpBuf) );

    switch (gu8Tray_Motor_Direction)
    {
        case TRAY_MOTOR_STOP:
            run_stop_ice_tray();

            gu16Err_Tray_Motor_Up_Total_Tmr = 0;
            gu16Err_Tray_Motor_Down_Total_Tmr = 0;
            gu8Tray_up_moving_retry_timer = 0;
            gu16Tray_down_moving_retry_timer = 0;
            break;

        case TRAY_MOTOR_CW_DIRECTION:
            /*..hui [18-2-6���� 1:32:40] CCW�� ���� 5�ʰ� ���� ������ ���¸� ���..*/
            mu8TrayCCW_DelayEnd = Get_TrayCCW_DelayEnd();

            if (mu8TrayCCW_DelayEnd == TRUE)
            {
                /*..hui [18-2-2���� 3:11:59] �������� �̵��� ���� ����..*/
                /*..hui [18-2-6���� 7:02:16] CW�� SET�Ǵ� ������ gu8IceStep�� 1�϶��ۿ� ����, ���� �����Ҷ�..*/
                check_error_tray_motor_up_operation();
            }
            else
            {
                run_stop_ice_tray();
            }

            break;

        case TRAY_MOTOR_CCW_DIRECTION:
            /*..hui [18-2-6���� 1:32:40] CCW�� ���� 5�ʰ� ���� ������ ���¸� ���..*/
            /*..hui [18-2-6���� 8:18:30] CCW�� SET�Ǵ� ������ ����Ż��/����Ż����....*/
            mu8TrayCW_DelayEnd = Get_TrayCW_DelayEnd();       // F_Tray_CW_delay_finish

            if (mu8TrayCW_DelayEnd == TRUE)
            {
                /*..hui [18-2-2���� 3:12:05] Ż�� ���� �̵��� ���� ����..*/
                check_error_tray_motor_down_retry_operation();
            }
            else
            {
                run_stop_ice_tray();
            }
            break;


        default:
            run_stop_ice_tray();
            break;
    }

    ice_make_system_up_move_reset();
    ice_make_system_down_move_reset();

    /*..hui [18-2-9���� 1:55:07] SW������ ���ÿ� SET�� ��� ����..*/
    if ( (mu8TrayMotorUpBuf == SET) && (mu8TrayMotorDownBuf == SET) )
    {
        Set_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62, SET);
    }
    else
    {

    }

    /*..hui [19-12-13���� 7:25:08] Ʈ���� ���� �� ���������� Ż�� ������ ���� �� ���� ī��Ʈ �ʱ�ȭ..*/
    mu8IceStepBuf = Get_IceStep();

    if (mu8IceStepBuf >= STATE_43_ICE_TAKE_OFF)
    {
        gu8Err_tray_down_acc_count = 0;
    }
    else
    {

    }
#endif
}


#if (MICRO_SW_ICE_MAKING_ERROR_CHECK_USE == USE)
/// @brief      Ʈ���� ���� UP ���� ���� üũ
/// @details    Ʈ���� ���� UP ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void check_error_tray_motor_up_operation(void)
{
    U8 mu8TrayPosition = 0;
    U8 mu8TrayModeBuf = 0;

    mu8TrayPosition = Get_IceTrayPosition();
    mu8TrayModeBuf = Get_IceTrayMode();

    /*..hui [18-2-6���� 5:25:47] Ż������ ���� �������� �̵� �� ��������..*/
    switch (mu8TrayPosition)
    {
        case ICE_TRAY_POSITION_ICE_MAKING:
            /*..hui [18-2-6���� 2:14:43] ���� �������� ������ ����..*/
            run_stop_ice_tray();

            Set_TrayMotorUpStatus(CLEAR);
            gu16Err_Tray_Motor_Up_Total_Tmr = 0;
            gu8Err_Tray_Up_Count = 0;
            gu8Tray_up_moving_retry_timer = 0;
            gu8Tray_up_contiunue_timer = 0;

            break;

        case ICE_TRAY_POSITION_MOVING:
            gu8Tray_up_moving_retry_timer = 0;

            if (mu8TrayModeBuf == ICE_TRAY_MODE_NONE)
            {
                gu8Tray_up_contiunue_timer++;

                if (gu8Tray_up_contiunue_timer >= 10)
                {
                    gu8Tray_up_contiunue_timer = 0;
                    run_up_ice_tray();
                }
                else
                {

                }
            }
            else
            {
                gu8Tray_up_contiunue_timer = 0;
            }

            break;

        case ICE_TRAY_POSITION_ICE_THROW:
            gu8Tray_up_moving_retry_timer++;

            if (gu8Tray_up_moving_retry_timer <= 50)
            {
                run_up_ice_tray();
            }
            else if ( (gu8Tray_up_moving_retry_timer > 50) && (gu8Tray_up_moving_retry_timer <= 100) )
            {
                /*pMOTOR_ICE_TRAY_CW = CLEAR;*/
                run_stop_ice_tray();
            }
            else if ( (gu8Tray_up_moving_retry_timer > 100) && (gu8Tray_up_moving_retry_timer <= 150) )
            {
                /*pMOTOR_ICE_TRAY_CW = SET;*/
                run_up_ice_tray();
            }
            else
            {
                /*pMOTOR_ICE_TRAY_CW = CLEAR;*/
                run_stop_ice_tray();

                Set_TrayMotorUpStatus(CLEAR);
                gu8Tray_up_moving_retry_timer = 0;
                gu8Tray_up_contiunue_timer = 0;

                gu8Err_Tray_Up_Count++;

                if (gu8Err_Tray_Up_Count >= TRAY_UP_RETRY_COUNT)
                {
                    Set_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62, SET);
                }
                else
                {
                    /*gu8Err_Tray_Up_Count++;*/
                    Set_TrayUpMoveReset(SET);
                    ice_system_stop();

                    if (gu8Err_Tray_Up_Count <= TRAY_UP_ICE_STUCK_COUNT)
                    {
                        /*..hui [25-3-17���� 4:18:38] ù 2ȸ�� �����ɸ� ��ȸ�� ���� �߰�..*/
                        Set_IceStuckBackStatus(SET);
                    }
                    else
                    {

                    }
                }
            }

            break;

        case ICE_TRAY_POSITION_ERROR:
            /*..hui [18-2-6���� 4:49:15] Ʈ���� ���� ������ ��� ����..*/
            /*..hui [18-2-6���� 4:51:07] �������� �������� ���� ���κ� ���ð��� ���� �κп��� ó��..*/
            /*pMOTOR_ICE_TRAY_CW = CLEAR;*/
            run_stop_ice_tray();
            Set_TrayMotorUpStatus(CLEAR);
            gu8Tray_up_moving_retry_timer = 0;
            gu8Tray_up_contiunue_timer = 0;
            break;

        default:
            /*pMOTOR_ICE_TRAY_CW = CLEAR;*/
            run_stop_ice_tray();
            Set_TrayMotorUpStatus(CLEAR);
            gu8Tray_up_moving_retry_timer = 0;
            gu8Tray_up_contiunue_timer = 0;

            break;
    }

    /*if( pMOTOR_ICE_TRAY_CW == SET )*/
    if (mu8TrayModeBuf == ICE_TRAY_MODE_UP)
    {
        gu16Err_Tray_Motor_Up_Total_Tmr++;

        if (gu16Err_Tray_Motor_Up_Total_Tmr >= ICETRAY_CW_TIME)
        {
            /*pMOTOR_ICE_TRAY_CW = CLEAR;*/
            run_stop_ice_tray();
            Set_TrayMotorUpStatus(CLEAR);
            gu16Err_Tray_Motor_Up_Total_Tmr = 0;

            gu8Err_Tray_Up_Count++;

            if (gu8Err_Tray_Up_Count >= TRAY_UP_RETRY_COUNT)
            {
                Set_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62, SET);
            }
            else
            {
                Set_TrayUpMoveReset(SET);
                ice_system_stop();

                if (gu8Err_Tray_Up_Count <= TRAY_UP_ICE_STUCK_COUNT)
                {
                    /*..hui [25-3-17���� 4:18:38] ù 2ȸ�� �����ɸ� ��ȸ�� ���� �߰�..*/
                    Set_IceStuckBackStatus(SET);
                }
                else
                {

                }
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


/// @brief      Ʈ���� ���� ���� �̵� ��õ� Ƚ�� ���� �Լ�
/// @details    Ʈ���� ���� ���� �̵� ��õ� Ƚ���� �����Ѵ�
/// @param      mu8Data : Ʈ���� ���� ���� �̵� ��õ� Ƚ��
/// @return     void
void Set_TrayUp_ErrorCount(U8 mu8Data)
{
    gu8Err_Tray_Up_Count = mu8Data;
}


/// @brief      Ʈ���� ���� ���� �̵� ��õ� Ƚ�� Ȯ�� �Լ�
/// @details    Ʈ���� ���� ���� �̵� ��õ� Ƚ���� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ʈ���� ���� ���� �̵� ��õ� Ƚ��
U8 Get_TrayUp_ErrorCount(void)
{
    return  gu8Err_Tray_Up_Count;
}


/// @brief      Ʈ���� ���� Down ���� ���� üũ
/// @details    Ʈ���� ���� Down ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void check_error_tray_motor_down_retry_operation(void)
{
    U8 mu8TrayPosition = 0;
    U8 mu8TrayModeBuf = 0;

    mu8TrayPosition = Get_IceTrayPosition();
    mu8TrayModeBuf = Get_IceTrayMode();

    /*..hui [18-2-6���� 8:26:55] �������� Ż������ �̵� �� ��������....*/
    switch (mu8TrayPosition)
    {
        case ICE_TRAY_POSITION_ICE_MAKING:
            /*..hui [18-2-6���� 8:27:13] 5�� �̻� ���� ������ �� �õ� ��� ����..*/
            gu16Tray_down_moving_retry_timer++;

            if (gu16Tray_down_moving_retry_timer <= 250)
            {
                run_down_ice_tray();
            }
            else if ( (gu16Tray_down_moving_retry_timer > 250) && (gu16Tray_down_moving_retry_timer <= 300) )
            {
                /*pMOTOR_ICE_TRAY_CCW = CLEAR;*/
                run_stop_ice_tray();
            }
            else if ( (gu16Tray_down_moving_retry_timer > 300) && (gu16Tray_down_moving_retry_timer <= 550) )
            {
                /*pMOTOR_ICE_TRAY_CCW = SET;*/
                run_down_ice_tray();
            }
            else
            {
                /*pMOTOR_ICE_TRAY_CCW = CLEAR;*/
                run_stop_ice_tray();
                Set_TrayMotorDownStatus(CLEAR);
                gu16Tray_down_moving_retry_timer = 0;

                gu8Err_Tray_Down_Count++;
                gu8Err_tray_down_acc_count++;

                if ( (gu8Err_Tray_Down_Count >= 3) || (gu8Err_tray_down_acc_count >= 10) )
                {
                    Set_ErrorStatus(ERROR_ID_MICRO_SW_DE_ICE_E63, SET);
                }
                else
                {
                    Set_TrayDownMoveReset(SET);
                    ice_system_stop();
                }
            }

            break;

        case ICE_TRAY_POSITION_MOVING:
            gu16Tray_down_moving_retry_timer = 0;
            /*pMOTOR_ICE_TRAY_CCW = SET;*/
            run_down_ice_tray();
            break;

        case ICE_TRAY_POSITION_ICE_THROW:
            /*..hui [18-2-6���� 2:14:43] ���� �������� ������ ����..*/
            /*pMOTOR_ICE_TRAY_CCW = CLEAR;*/
            run_stop_ice_tray();
            Set_TrayMotorDownStatus(CLEAR);
            gu16Err_Tray_Motor_Down_Total_Tmr = 0;
            gu8Err_Tray_Down_Count = 0;
            gu16Tray_down_moving_retry_timer = 0;
            break;

        case ICE_TRAY_POSITION_ERROR:
            /*..hui [18-2-6���� 4:49:15] Ʈ���� ���� ������ ��� ����..*/
            /*..hui [18-2-6���� 4:51:07] �������� �������� ���� ���κ� ���ð��� ���� �κп��� ó��..*/
            /*pMOTOR_ICE_TRAY_CCW = CLEAR;*/
            run_stop_ice_tray();
            Set_TrayMotorDownStatus(CLEAR);
            gu16Tray_down_moving_retry_timer = 0;
            break;

        default:
            /*pMOTOR_ICE_TRAY_CCW = CLEAR;*/
            run_stop_ice_tray();
            Set_TrayMotorDownStatus(CLEAR);
            gu16Tray_down_moving_retry_timer = 0;
            break;
    }

    /*if(pMOTOR_ICE_TRAY_CCW == SET)*/
    if (mu8TrayModeBuf == ICE_TRAY_MODE_DOWN)
    {
        gu16Err_Tray_Motor_Down_Total_Tmr++;

        if (gu16Err_Tray_Motor_Down_Total_Tmr >= ICETRAY_CCW_TIME)
        {
            /*pMOTOR_ICE_TRAY_CCW = CLEAR;*/
            run_stop_ice_tray();
            Set_TrayMotorDownStatus(CLEAR);
            gu16Err_Tray_Motor_Down_Total_Tmr = 0;

            gu8Err_Tray_Down_Count++;
            gu8Err_tray_down_acc_count++;

            if ( (gu8Err_Tray_Down_Count >= 3) || (gu8Err_tray_down_acc_count >= 10) )
            {
                Set_ErrorStatus(ERROR_ID_MICRO_SW_DE_ICE_E63, SET);
            }
            else
            {
                /*gu8_Err_Tray_Down_Count++;*/
                Set_TrayDownMoveReset(SET);
                ice_system_stop();
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


/// @brief      Ʈ���� Ż�� ���� �̵� ��õ� Ƚ�� ���� �Լ�
/// @details    Ʈ���� Ż�� ���� �̵� ��õ� Ƚ���� �����Ѵ�
/// @param      mu8Data : Ʈ���� Ż�� ���� �̵� ��õ� Ƚ��
/// @return     void
void Set_TrayDown_ErrorCount(U8 mu8Data)
{
    gu8Err_Tray_Down_Count = mu8Data;
}


/// @brief      Ʈ���� Ż�� ���� �̵� ��õ� Ƚ�� Ȯ�� �Լ�
/// @details    Ʈ���� Ż�� ���� �̵� ��õ� Ƚ���� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Ʈ���� Ż�� ���� �̵� ��õ� Ƚ��
U8 Get_TrayDown_ErrorCount(void)
{
    return  gu8Err_Tray_Down_Count;
}


/// @brief      E62, E63 üũ ���� ���� �ʱ�ȭ �Լ�
/// @details    E62�� E63 üũ�� ����ϴ� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Clear_E62_E63_CheckTimer(void)
{
    gu16Err_Tray_Motor_Up_Total_Tmr = 0;
    gu16Err_Tray_Motor_Down_Total_Tmr = 0;
    gu8Tray_up_moving_retry_timer = 0;
    gu16Tray_down_moving_retry_timer = 0;
}

#endif


/// @brief      �巹�� ���� ���� üũ
/// @details    �巹�� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void DrainPumpErrorCheck(void)
{
#if (DRAIN_PUMP_ERROR_CHECK_USE == USE)
    static U16 mu16PumpActTime = 0;
    static U8 mu8ErrorDetectCount = 0;
    static U8 mu8ErrorCheckDelay = 0;
    U8 mu8DrainPumpStatus = 0;
    U8 mu8TankLevel = 0;

    mu8DrainPumpStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_DRAIN);    // �巹�� ���� ���� ���� Ȯ��
    mu8TankLevel = Get_WaterLevel();                                // ������ �Ǵ� ��ũ�� ���� ���� Ȯ��

    if (mu8DrainPumpStatus == TRUE)
    {   // �巹�� ������ ���� ���� ���
        if (SError.gu8Status[ERROR_ID_DRAIN_PUMP_E60] == CLEAR)
        {   // ���� �߻��� �ȵ� ����
            if (SError.gu8Detect[ERROR_ID_DRAIN_PUMP_E60] == CLEAR)
            {   // ���� ������ �ȵ� ���� �Ǵ� ���� �絿�� ����
                if (mu8TankLevel > WATER_LEVEL_LOW)
                {   // ������ ���� �̻� ������ �����ǰ� �ִ� ���
                    if (mu16PumpActTime >= DRAIN_PUMP_ERROR_DETECT_TIME)
                    {   // ���� �ð� �̻� ������ ������ ���
                        mu16PumpActTime = 0;

                        Set_ErrorDetect(ERROR_ID_DRAIN_PUMP_E60, SET);      // ���� ���� ���� ����

                        if (mu8ErrorDetectCount >= DRAIN_PUMP_ERROR_CONFIRM_COUNT)
                        {   // ���� Ƚ�� �̻� ���� �簨�� �� ���
                            Set_ErrorStatus(ERROR_ID_DRAIN_PUMP_E60, SET);  // ���� Ȯ��
                        }
                        else
                        {
                            mu8ErrorDetectCount++;
                        }
                    }
                    else
                    {
                        mu16PumpActTime++;
                    }
                }
                else
                {   // ������ ���� �̸����� ������ ������ ���
                    mu16PumpActTime = 0;
                    mu8ErrorDetectCount = 0;
                }
            }
            else
            {   // ���� ������ �� ������ ���
                mu16PumpActTime = 0;
            }
        }
        else
        {   // ������ �߻��� ����
            mu16PumpActTime = 0;
            mu8ErrorDetectCount = 0;
        }
    }
    else
    {   // �巹�� ������ ���� ���� �ƴ� ����
        if (SError.gu8Status[ERROR_ID_DRAIN_PUMP_E60] == CLEAR)
        {   // ������ �߻����� ���� ����
            if (SError.gu8Detect[ERROR_ID_DRAIN_PUMP_E60] == SET)
            {
                if (mu8ErrorCheckDelay >= DRAIN_PUMP_ERROR_CHECK_DELAY)
                {   // ���� �ð� ���� ��
                    Set_ErrorDetect(ERROR_ID_DRAIN_PUMP_E60, CLEAR);      // ���� ������ ��˻� �� �� �ֵ��� ���� ���¸� �ʱ�ȭ
                }
                else
                {
                    mu8ErrorCheckDelay++;
                }
            }
        }

        mu16PumpActTime = 0;
    }
#endif
}


/// @brief      ź�� ���� ���� üũ
/// @details    ź�� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void SodaPumpErrorCheck(void)
{
#if (SODA_PUMP_ERROR_CHECK_USE == USE)
    static mu8ErrorDetectTime = 0;
    U8 mu8SodaPumpStatus = 0;
    U16 mu16SodaPumpFeedbackAD = 0;

    mu8SodaPumpStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_SODA);      // ź�� ���� ���� ���� Ȯ��
    mu16SodaPumpFeedbackAD = Get_ADC_Data(ADC_ID_SODA_PUMP);        // ź�� ���� �ǵ�� AD�� Ȯ��

    if (mu8SodaPumpStatus == TRUE)
    {   // ������ ���� ���� ���
        if ( (mu16SodaPumpFeedbackAD < SODA_PUMP_ERROR_UNDER_STANDARD_AD) ||
             (mu16SodaPumpFeedbackAD > SODA_PUMP_ERROR_OVER_STANDARD_AD) )
        {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
            if (mu8ErrorDetectTime >= SODA_PUMP_ERROR_DETECT_TIME)
            {
                Set_ErrorStatus(ERROR_ID_SODA_PUMP_E94, SET);
            }
            else
            {
                mu8ErrorDetectTime++;
            }
        }
        else
        {
            Set_ErrorStatus(ERROR_ID_SODA_PUMP_E94, CLEAR);
            mu8ErrorDetectTime = 0;
        }
    }
    else
    {   // ������ ���� ���� �ƴ� ���
        mu8ErrorDetectTime = 0;
    }
#endif
}


/// @brief      ���� �̻� ���� üũ
/// @details    ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void AbnormalEffluentErrorCheck(void)
{
#if (ABNORMAL_EFFLUENT_ERROR_CHECK_USE == USE)
    static U8 mu8ErrorDetectTime = 0;
    U8 mu8Status = 0;

    mu8Status = Get_OperatingStatus();              // ���� ��ǰ�� ���� ���� Ȯ��

    if (mu8Status == OPERATION_ID_EFFLUENT)
    {   // ���� ���� ������ ���
        if (SError.gu8Status[ERROR_ID_ABNORMAL_EFFLUENT_E03] == CLEAR)
        {   // ���� �̻� ���� ���°� �ƴ� ���
            if (mu8ErrorDetectTime >= ABNORMAL_EFFLUENT_DETECT_TIME)
            {
                mu8ErrorDetectTime = 0;

                Set_ErrorStatus(ERROR_ID_ABNORMAL_EFFLUENT_E03, SET);
            }
            else
            {
                mu8ErrorDetectTime++;
            }
        }
        else
        {
            mu8ErrorDetectTime = 0;
        }
    }
    else
    {
        mu8ErrorDetectTime = 0;
    }
#endif
}


/// @brief      ���� ��ũ ���� ���� �̻� ���� üũ
/// @details    ���� ��ũ ���� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void RoomLevelErrorCheck(void)
{
#if (LEVEL_ERROR_CHECK_USE == USE)

    U8 mu8LowSensor = 0;
#if (MIDDLE_SENSOR_USE == USE)
    U8 mu8MiddleSensor = 0;
#endif
    U8 mu8HighSensor = 0;
    U8 mu8OverFlowSensor = 0;

    U8 mu8Status = 0;

    mu8LowSensor = Get_WaterLevel_Status(LEVEL_ID_LOW);
#if (MIDDLE_SENSOR_USE == USE)
    mu8MiddleSensor = Get_WaterLevel_Status(LEVEL_ID_MODDLE);
#endif
    mu8HighSensor = Get_WaterLevel_Status(LEVEL_ID_HIGH);
    mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_OVERFLOW);

    mu8Status = Get_OperatingStatus();              // ���� ��ǰ�� ���� ���� Ȯ��

#if (MIDDLE_SENSOR_USE == USE)      // �߼��� ������ �ִ� ���
    // ���� ���� *********************************************************************************************
    if (!mu8LowSensor && mu8MiddleSensor)
    {   // ������ �̰����Ǵµ� �߼��� ���� �����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == CLEAR)
        {   // ������ ������ �߻����� ���� ���
            Set_ErrorStatus(ERROR_ID_LOW_SENSOR_E11, SET);              // ������ ���� ������ 1ȸ���� �ٷ� �߻�
        }
    }
    else if (mu8LowSensor && !mu8MiddleSensor && mu8HighSensor)
    {   // ������, �������� �����Ǵµ� �߼����� �̰����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_MIDDLE_SENSOR_E12] == CLEAR)
        {   // �߼��� ������ �߻����� ���� ���
            Set_ErrorStatus(ERROR_ID_MIDDLE_SENSOR_E12, SET);           // �߼��� ���� ������ 1ȸ���� �ٷ� �߻�
        }
    }
    else if (!mu8LowSensor && !mu8MiddleSensor && mu8HighSensor)
    {   // �������� �����Ǵµ� ������, �߼����� �̰����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == CLEAR)
        {   // ��, �߼��� ������ �߻����� ���� ���
            Set_ErrorStatus(ERROR_ID_LOW_MIDDLE_SENSOR_E15, SET);       // ��, �߼��� ���� ���� ������ 1ȸ���� �ٷ� �߻�
        }
    }
    else if (mu8LowSensor && !mu8MiddleSensor && !mu8HighSensor && mu8OverFlowSensor)
    {   // ������, �����÷ο�� �����Ǵµ� �߼���, �������� �̰����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == CLEAR)
        {   // ��, ������ ������ �߻����� ���� ���
            Set_ErrorStatus(ERROR_ID_MIDDLE_HIGH_SENSOR_E16, SET);      // ��, ������ ���� ���� ������ 1ȸ���� �ٷ� �߻�
        }
    }
    else if (mu8LowSensor && mu8MiddleSensor && !mu8HighSensor && mu8OverFlowSensor)
    {   // ������, �߼���, �����÷ο�� �����Ǵµ� �������� �̰����Ǵ� ���
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == CLEAR)
        {   // ������ ���� ���� ���°� �̰����̸�
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, SET);         // ������ ���� ���� ���� ���·� ������

            if (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == CLEAR)
            {   // ������ ������ �߻����� ���� �����̸�
                if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] >= 3)    // 3�� �������� ���� �߻�
                {
                    SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;
                    Set_ErrorStatus(ERROR_ID_HIGH_SENSOR_E13, SET);     // ������ ���� Ȯ��
                }
                else
                {
                    SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13]++;
                }
            }
        }
    }
    else if (!mu8LowSensor && !mu8MiddleSensor && !mu8HighSensor && mu8OverFlowSensor)
    {   // �����÷ο�� �����Ǵµ� ������, �߼���, �������� �̰����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == CLEAR)
        {   // ���� �÷ο� ������ �߻����� ���� �����̸�
            Set_ErrorStatus(ERROR_ID_OVER_FLOW_E17, SET);   // ���� �÷ο� ������ 1ȸ ������ �ٷ� �߻�
        }
    }
    else if (mu8LowSensor && mu8MiddleSensor && mu8HighSensor && mu8OverFlowSensor)
    {   // ������, �߼���, ������, �����÷ο찡 ��� �����Ǵ� ���
        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == CLEAR)
        {   // Feed Valve ���� ���� ���°� �̰����̸�
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, SET);      // Feed Valve ���� ���� ���·� ����

            if (SError.gu8Status[ERROR_ID_FEED_VALVE_E10] == CLEAR)
            {   // Feed Valve ���� ���°� �ƴ� ���
                if (mu8Status == OPERATION_ID_EW_STERILIZE)
                {   // EW ��� ���� ���̸�
                    SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
                    Set_ErrorStatus(ERROR_ID_FEED_VALVE_E10, SET);          // EW ��� �߿��� Feed Valve ���� 1ȸ�� Ȯ��
                }
                else
                {
                    if (SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] >= 3)        // 3�� �������� ���� �߻�
                    {
                        SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
                        Set_ErrorStatus(ERROR_ID_FEED_VALVE_E10, SET);          // Feed Valve ���� Ȯ��
                    }
                    else
                    {
                        SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10]++;
                    }
                }
            }
        }
    }

    // ���� ���� *********************************************************************************************
    if (mu8LowSensor)
    {   // �������� �����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == SET)
        {   // ������ ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_LOW_SENSOR_E11, CLEAR);            // ������ ���� ���� ����
        }

        if (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == SET)
        {   // ��, �߼��� ���� ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_LOW_MIDDLE_SENSOR_E15, CLEAR);     // ��, �߼��� ���� ���� ���� ����
        }

        if (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET)
        {   // OverFlow ���� ������ ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_OVER_FLOW_E17, CLEAR);             // OverFlow ���� ����
        }
    }
    else
    {   // �������� �̰����Ǵ� ���
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // ������ ������ �����Ǿ��� ��� Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] > 0)
        {   // ������ ���� ���� ī���� �ʱ�ȭ
            SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;      // Feed Valve ���� ī���� �ʱ�ȭ
        }

        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve ������ �����Ǿ��� ��� Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] > 0)
        {   // Feed Valve ���� ī���� �ʱ�ȭ
            SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
        }
    }

    if (mu8MiddleSensor)
    {   // �߼����� �����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_MIDDLE_SENSOR_E12] == SET)
        {   // �߼��� ������ �߻��� ���
            Set_ErrorStatus(ERROR_ID_MIDDLE_SENSOR_E12, CLEAR);         // �߼��� ���� ���� �߻��ƴ� ��� �ڵ� ����
        }

        if (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == SET)
        {   // ��, �߼��� ���� ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_LOW_MIDDLE_SENSOR_E15, CLEAR);     // ��, �߼��� ���� ���� ���� ����
        }

        if (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == SET)
        {   // ��, ������ ���� ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_MIDDLE_HIGH_SENSOR_E16, CLEAR);    // ��, ������ ���� ���� ���� �߻��ƴ� ��� �ڵ� ����
        }

        if (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET)
        {   // OverFlow ���� ������ ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_OVER_FLOW_E17, CLEAR);             // OverFlow ���� ����
        }
    }
    else
    {   // �߼����� �̰��� �Ǵ� ���
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // ������ ������ �����Ǿ��� ��� Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve ������ �����Ǿ��� ��� Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }
    }

    if (mu8HighSensor)
    {   // �������� ���� �Ǵ� ���
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // ������ ������ �����Ǿ��� ��� Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] > 0)
        {   // ������ ���� ���� ī���� �ʱ�ȭ
            SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;      // Feed Valve ���� ī���� �ʱ�ȭ
        }

        if (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // ������ ���� �߻��߾��� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_HIGH_SENSOR_E13, CLEAR);            // ������ ���� ���� ����
        }

        if (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == SET)
        {   // ��, ������ ���� ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_MIDDLE_HIGH_SENSOR_E16, CLEAR);    // ��, ������ ���� ���� ���� �߻��ƴ� ��� �ڵ� ����
        }

        if (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET)
        {   // OverFlow ���� ������ ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_OVER_FLOW_E17, CLEAR);             // OverFlow ���� ����
        }
    }
    else
    {   // �������� �̰��� �Ǵ� ���
        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve ������ �����Ǿ��� ��� Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }
    }

#else           // �߼��� ������ ���� ���

    // ���� ���� *********************************************************************************************
    if (!mu8LowSensor && mu8HighSensor)
    {   // ������ �̰����Ǵµ� ������ ���� �����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == CLEAR)
        {   // ������ ������ �߻����� ���� ���
            Set_ErrorStatus(ERROR_ID_LOW_SENSOR_E11, SET);              // ������ ���� ������ 1ȸ���� �ٷ� �߻�
        }
    }
    else if (mu8OverFlowSensor)
    {   // ����ħ ���� ������
        if (!mu8LowSensor && !mu8HighSensor)
        {   // ������, ����� ���� ��� �̰���(���� ���� ����)
            if (SError.gu8Detect[ERROR_ID_COMPLEX_SENSOR_E14] == CLEAR)
            {   // ���� ���� ���� ���� ���� ���°� �ƴ� ���
                Set_ErrorDetect(ERROR_ID_COMPLEX_SENSOR_E14, SET);      // ���� ���� ���� ���� ���� ���� ����

                if (SError.gu8Status[ERROR_ID_COMPLEX_SENSOR_E14] == CLEAR)
                {   // ���� ���� ���� ������ �߻����� ���� ���
                    if (SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] >= 3)
                    {   // 3�� �������� ���� �߻�
                        SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] = 0;
                        Set_ErrorStatus(ERROR_ID_COMPLEX_SENSOR_E14, SET);      // ���� Ȯ��
                    }
                    else
                    {
                        SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14]++;
                    }
                }
            }
        }
        else if (mu8LowSensor && !mu8HighSensor)
        {   // �������� ����(������ ���� ����)
            if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == CLEAR)
            {   // ������ ���� ���� ���� ���°� �ƴ� ���
                Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, SET);      // ������ ���� ���� ���� ���� ����

                if (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == CLEAR)
                {   // ������ ���� ������ �߻����� ���� ���
                    if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] >= 3)
                    {   // 3�� �������� ���� �߻�
                        SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;
                        Set_ErrorStatus(ERROR_ID_HIGH_SENSOR_E13, SET);      // ���� Ȯ��
                    }
                    else
                    {
                        SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13]++;
                    }
                }
            }
        }
        else if (mu8LowSensor && mu8HighSensor)
        {  // ���� ������ ���� �� �����Ǵ� ���(Feed Valve Error)
            if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == CLEAR)
            {   // �ǵ� ��� ���� ���� ���°� �ƴ� ���
                Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, SET);      // �ǵ� ��� ���� ���� ���� ����

                if (SError.gu8Status[ERROR_ID_FEED_VALVE_E10] == CLEAR)
                {   // �ǵ� ��� ������ �߻����� ���� ���
                    if (mu8Status == OPERATION_ID_EW_STERILIZE)
                    {   // EW ��� ���� ���̸�
                        SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
                        Set_ErrorStatus(ERROR_ID_FEED_VALVE_E10, SET);          // EW ��� �߿��� Feed Valve ���� 1ȸ�� Ȯ��
                    }
                    else
                    {
                        if (SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] >= 3)        // 3�� �������� ���� �߻�
                        {
                            SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
                            Set_ErrorStatus(ERROR_ID_FEED_VALVE_E10, SET);          // Feed Valve ���� Ȯ��
                        }
                        else
                        {
                            SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10]++;
                        }
                    }
                }
            }
        }
    }

    // ���� ���� *********************************************************************************************
    if (mu8LowSensor)
    {   // ������ ���� ����
        if (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == SET)
        {   // ������ ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_LOW_SENSOR_E11, CLEAR);            // ���� �ڵ� ����
        }

        if (SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] > 0)
        {   // ������ ���� ���� ���� ī���� ���̾��� ���
            SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] = 0;
        }
    }
    else
    {   // ������ ���� �̰���
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // ������ ������ �����Ǿ��� ��� Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] > 0)
        {   // ������ ���� ī��Ʈ �ʱ�ȭ
            SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;
        }

        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve ������ �����Ǿ��� ��� Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] > 0)
        {   // Feed Valve ���� ī���� �ʱ�ȭ
            SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
        }
    }

    if (mu8HighSensor)
    {   // ������ ���� ����
        if (SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] > 0)
        {   // ������ ���� ���� ���� ī���� ���̾��� ���
            SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] = 0;
        }

        if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] > 0)
        {   // ������ ���� ī��Ʈ �ʱ�ȭ
            SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;
        }

        if (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // ������ ���� �߻��߾��� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }
    }
    else
    {   // ������ ���� �̰���
        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve ������ �����Ǿ��� ��� Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }
    }

    if (mu8OverFlowSensor)
    {   // overflow ���� ����

    }
    else
    {   // overflow ���� �̰���
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // OverFlow ���� �̰����� ������ ���� Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8Detect[ERROR_ID_COMPLEX_SENSOR_E14] == SET)
        {   // ���� ���ռ��� ���� üũ�� ���� ����ħ ���� �̰����� Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_COMPLEX_SENSOR_E14, CLEAR);
        }
    }

#endif

#endif
}


/// @brief      �¼� ��ũ ������ ���� �̻� ���� üũ
/// @details    �¼� ��ũ ������ ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void HotHighLevelErrorCheck(void)
{
#if (HOT_HIGH_SENSOR_ERROR_CHECK_USE == USE)

    U8 mu8HighSensor = 0;
    U8 mu8OverFlowSensor = 0;

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)     // ���غ� Ÿ�� �¼� ��ũ ���� ������ ���

#if (HOT_HIGH_SENSOR_OVERFLOW_SENSOR_USE == NO_USE)    // �¼� ��ũ ���� �÷��찡 ���� ���
    U8 mu8WaterInputStatus = 0;
    static U16 mu16ErrorDetectTime = 0;
#endif

    mu8HighSensor = Get_ElecLevel_Status();
#else
    mu8HighSensor = Get_WaterLevel_Status(LEVEL_ID_HOT_HIGH);
#endif

#if (HOT_HIGH_SENSOR_OVERFLOW_SENSOR_USE == USE)    // �¼� ��ũ ���� �÷��찡 �ִ� ���
    mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_OVERFLOW);

    if (!mu8HighSensor && mu8OverFlowSensor)
    {   // �¼� �������� �������� ���� ���¿��� �¼� �����÷ο찡 �����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_HOT_HIGH_SENSOR_E33] == CLEAR)
        {   // �¼� ������ ������ �߻����� ���� ����
            Set_ErrorStatus(ERROR_ID_HOT_HIGH_SENSOR_E33, SET);
        }
    }

#else       // �¼� ��ũ ���� �÷ο찡 ���� ���
    U8 mu8HotWaterLevel = 0;
    U8 mu8Status = 0;

    mu8Status = Get_OperatingStatus();              // ���� ��ǰ�� ���� ���� Ȯ��
    mu8HotWaterLevel = Get_HotWaterLevel();

    mu8WaterInputStatus = Get_ValveStatus(VALVE_ID_WATER_IN);              // �Լ� ���� ���� Ȯ��

    if (mu8Status == OPERATION_ID_STAND_BY)
    {   // ��ǰ�� ��� ����
        if (mu8HotWaterLevel < HOT_WATER_LEVEL_HIGH)
        {   // �¼� �������� �ƴ� ���
            if (mu8WaterInputStatus == HIGH)
            {   // �Լ� ���� ���̸�
                if (SError.gu8Status[ERROR_ID_HOT_HIGH_SENSOR_E33] == CLEAR)
                {   // �¼� ������ ������ �߻����� ���� ����
                    if (mu16ErrorDetectTime >= HOT_HIGH_SENSOR_WATER_INPUT_DETECT_TIME)
                    {
                        mu16ErrorDetectTime = 0;
                        Set_ErrorStatus(ERROR_ID_HOT_HIGH_SENSOR_E33, SET);
                    }
                    else
                    {
                        mu16ErrorDetectTime++;
                    }
                }
                else
                {
                    mu16ErrorDetectTime = 0;
                }
            }
            else
            {
                mu16ErrorDetectTime = 0;
            }
        }
        else
        {
            mu16ErrorDetectTime = 0;
        }
    }
    else
    {   // ��ǰ�� ���� ����ϰ� ������ �ϰ� �ִ� ����
        mu16ErrorDetectTime = 0;
    }
#endif

#endif
}


/// @brief      ������ũ ���� ���� �̻� ���� üũ
/// @details    ������ũ ���� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void IceColdWaterTankLevelErrorCheck(void)
{
#if (ICE_COLD_WATER_TANK_SENSOR_ERROR_CHECK_USE == USE)
    U8 mu8IcdColdWaterHighSensor = 0;
    U8 mu8ColdOnOff = 0;

    mu8IcdColdWaterHighSensor = Get_WaterLevel_Status(LEVEL_ID_ICD_COLD_WATER);
    mu8ColdOnOff = Get_ColdOnOff();

    if (mu8ColdOnOff == TRUE)
    {   // �ü� ����� ���� ����
        if (mu8IcdColdWaterHighSensor == FALSE)
        {   // ���� ��ũ ���� ������ �̰��� ����
            if (SError.gu8Status[ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20] == CLEAR)
            {   // ���� ��ũ ���� ���� ������ �߻����� ���� ����
                Set_ErrorStatus(ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20, SET);
            }
        }
        else
        {   // ���� ��ũ ���� ������ ���� ����
            if (SError.gu8Status[ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20] == SET)
            {   // ���� ��ũ ���� ���� ������ �߻��� ����
                Set_ErrorStatus(ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20, CLEAR);
            }
        }
    }
#endif
}


/// @brief      �ü� ��ũ ���� ���� �̻� ���� üũ
/// @details    �ü� ��ũ ���� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void ColdLevelErrorCheck(void)
{
#if (COLD_LEVEL_ERROR_CHECK_USE == USE)

#if (COLD_LEVEL_ERROR_LOW_SENSOR_USE == USE)
    U8 mu8LowSensor = 0;
    static U16 mu16LowErrorDetectTime = 0;
#endif

#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == USE)
    U8 mu8HighSensor = 0;

    static U16 mu16HighErrorDetectTimeSec = 0;
    static U8 mu8HighErrorDetectTimeMin = 0;
#endif

    U8 mu8OverFlowSensor = 0;

#if (COLD_LEVEL_ERROR_LOW_SENSOR_USE == USE)
    mu8LowSensor = Get_WaterLevel_Status(LEVEL_ID_COLD_LOW);        // �ü� ������ ����
#endif

#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == USE)
    mu8HighSensor = Get_WaterLevel_Status(LEVEL_ID_COLD_HIGH);      // �ü� ������ ����
#endif

    mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_LOW);        // ���� ������ ����

#if (COLD_LEVEL_ERROR_LOW_SENSOR_USE == USE)        // �ü� �������� �ִ� ���

    // �ü� ������ ���� ����
#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == USE)       // �ü� �������� �ִ� ���

    // ���� ���� *********************************************************************************************
    if (!mu8LowSensor && mu8HighSensor)
    {   // �ü� ������ �̰����Ǵµ� �ü� ������ ���� �����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == CLEAR)
        {   // �ü� ������ ������ �߻����� ���� ���
            Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, SET);              // �ü� ������ ���� ������ 1ȸ���� �ٷ� �߻�
        }
    }

    // ���� ���� *********************************************************************************************
    if (mu8LowSensor)
    {   // �ü� ������ ���� ����
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET)
        {   // �ü� ������ ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, CLEAR);            // ���� �ڵ� ����
        }
    }
    else if (!mu8LowSensor && !mu8HighSensor)
    {   // �ü� ������, �ü� ������ ��� �̰���
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET)
        {   // �ü� ������ ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, CLEAR);            // ���� �ڵ� ����
        }
    }
    else
    {

    }

#else       // �ü� �������� ���� ���

    // ���� ���� *********************************************************************************************
    if (!mu8LowSensor && mu8OverFlowSensor)
    {   // �ü� ������ �̰����Ǵµ� ���� ������ ���� �����Ǵ� ���
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == CLEAR)
        {   // �ü� ������ ������ �߻����� ���� ���
            if (mu16LowErrorDetectTime >= COLD_LOW_SENSOR_ERROR_DETECT_TIME)
            {
                Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, SET);              // �ü� ������ ���� ���� �߻�
            }
            else
            {
                mu16LowErrorDetectTime++;
            }
        }
    }
    else
    {
        mu16LowErrorDetectTime = 0;
    }

    // ���� ���� *********************************************************************************************
    if (mu8LowSensor)
    {   // �ü� ������ ���� ����
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET)
        {   // �ü� ������ ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, CLEAR);            // ���� �ڵ� ����
        }
    }
    else
    {

    }

#endif

#endif

#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == USE)       // �ü� �������� �ִ� ���

    // �ü� ������ ���� ����
#if (COLD_LEVEL_ERROR_LOW_SENSOR_USE == USE)        // �ü� �������� �ִ� ���

    // ���� ���� *********************************************************************************************
    if (mu8LowSensor && !mu8HighSensor && mu8OverFlowSensor)
    {   // �ü� ������ ����, �ü� ������ �̰���, ���� ������ ������
        if (SError.gu8Detect[ERROR_ID_COLD_HIGH_SENSOR_E23] == CLEAR)
        {   // �ü� ������ ���� ������ �ȵ� ���
            Set_ErrorDetect(ERROR_ID_COLD_HIGH_SENSOR_E23, SET);                // �ü� ������ ���� ���� ���� ����

            if (SError.gu8Status[ERROR_ID_COLD_HIGH_SENSOR_E23] == CLEAR)
            {   // �ü� ������ ������ �߻����� ���� ���
                if (SError.gu8CheckCount[ERROR_ID_COLD_HIGH_SENSOR_E23] >= 3)
                {
                    Set_ErrorStatus(ERROR_ID_COLD_HIGH_SENSOR_E23, SET);        // �ü� ������ ���� ���� �߻�
                }
                else
                {
                    SError.gu8CheckCount[ERROR_ID_COLD_HIGH_SENSOR_E23]++;
                }
            }
            else
            {   // �ü� ������ ������ �߻��� ���
                SError.gu8CheckCount[ERROR_ID_COLD_HIGH_SENSOR_E23] = 0;
            }
        }
    }
    else
    {
        if (SError.gu8Detect[ERROR_ID_COLD_HIGH_SENSOR_E23] == SET)
        {   // �ü� ������ ���� ������ �� ���
            Set_ErrorDetect(ERROR_ID_COLD_HIGH_SENSOR_E23, CLEAR);            // �ü� ������ ���� ���� ���� ���� ����
        }
    }

    // ���� ���� *********************************************************************************************
    if (mu8HighSensor)
    {   // �ü� ������ ���� ����
        if (SError.gu8Status[ERROR_ID_COLD_HIGH_SENSOR_E23] == SET)
        {   // �ü� ������ ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_COLD_HIGH_SENSOR_E23, CLEAR);            // ���� �ڵ� ����
        }
    }
    else
    {

    }

#else       // �ü� �������� ���� ���

    // ���� ���� *********************************************************************************************
    if (!mu8HighSensor && mu8OverFlowSensor)
    {   // �ü� ������ �̰���, ���� ������ ���� ����
        if (SError.gu8Status[ERROR_ID_COLD_HIGH_SENSOR_E23] == CLEAR)
        {   // �ü� ������ ������ �߻����� ���� ���
            mu16HighErrorDetectTimeSec++;

            if (mu16HighErrorDetectTimeSec >= 600)
            {   // 60�ʰ� ����� ���
                mu16HighErrorDetectTimeSec = 0;

                mu8HighErrorDetectTimeMin++;

                if (mu8HighErrorDetectTimeMin >= 120)
                {   // 120min ��� ��
                    mu8HighErrorDetectTimeMin = 0;

                    Set_ErrorStatus(ERROR_ID_COLD_HIGH_SENSOR_E23, SET);
                }
            }
        }
        else
        {   // �ü� ������ ������ �߻��� ���
            mu16HighErrorDetectTimeSec = 0;
            mu8HighErrorDetectTimeMin = 0;
        }
    }
    else
    {
        mu16HighErrorDetectTimeSec = 0;
        mu8HighErrorDetectTimeMin = 0;
    }

    // ���� ���� *********************************************************************************************
    if (mu8HighSensor)
    {   // �ü� ������ ���� ����
        if (SError.gu8Status[ERROR_ID_COLD_HIGH_SENSOR_E23] == SET)
        {   // �ü� ������ ���� ���� �߻��ƴ� ��� �ڵ� ����
            Set_ErrorStatus(ERROR_ID_COLD_HIGH_SENSOR_E23, CLEAR);            // ���� �ڵ� ����
        }
    }
    else
    {

    }

#endif

#endif

#endif
}


/// @brief      ���� �µ� ���� �̻� ���� üũ
/// @details    ���� �µ� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void RoomTempErrorCheck(void)
{
#if (ROOM_TEMP_1_ERROR_CHECK_USE == USE)    // ���� �µ� ����1 ���� ����

    U16 mu16RoomTemp1_AD = 0;
    static U8 mu8RoomTemp1ErrorDetectTime = 0;

//    mu16RoomTemp1_AD = Get_ADC_Data(ADC_ID_TH_ROOM_1);   // ���� �µ� ����1 �µ� AD�� Ȯ��

    if ( (mu16RoomTemp1_AD > ROOM_TEMP_1_ERROR_OVER_STANDARD_AD) ||
         (mu16RoomTemp1_AD < ROOM_TEMP_1_ERROR_UNDER_STANDARD_AD) )
    {
        if (SError.gu8Status[ERROR_ID_ROOM_TEMP_1_E42] == CLEAR)
        {
            if (mu8RoomTemp1ErrorDetectTime >= ROOM_TEMP_1_ERROR_DETECT_TIME)
            {
                mu8RoomTemp1ErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_ROOM_TEMP_1_E42, SET);
            }
            else
            {
                mu8RoomTemp1ErrorDetectTime++;
            }
        }
        else
        {
            mu8RoomTemp1ErrorDetectTime = 0;
        }
    }
    else
    {
        mu8RoomTemp1ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_ROOM_TEMP_1_E42] == SET)
        {
            Set_ErrorStatus(ERROR_ID_ROOM_TEMP_1_E42, CLEAR);
        }
    }

#endif

#if (ROOM_TEMP_2_ERROR_CHECK_USE == USE)    // ���� �µ� ����2 ���� ����

    U16 mu16RoomTemp2_AD = 0;
    static U8 mu8RoomTemp2ErrorDetectTime = 0;

    mu16RoomTemp2_AD = Get_ADC_Data(ADC_ID_TH_ROOM_2);   // ���� �µ� ����2 �µ� AD�� Ȯ��

    if ( (mu16RoomTemp2_AD > ROOM_TEMP_2_ERROR_OVER_STANDARD_AD) ||
         (mu16RoomTemp2_AD < ROOM_TEMP_2_ERROR_UNDER_STANDARD_AD) )
    {
        if (SError.gu8Status[ERROR_ID_ROOM_TEMP_2_E52] == CLEAR)
        {
            if (mu8RoomTemp2ErrorDetectTime >= ROOM_TEMP_2_ERROR_DETECT_TIME)
            {
                mu8RoomTemp2ErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_ROOM_TEMP_2_E52, SET);
            }
            else
            {
                mu8RoomTemp2ErrorDetectTime++;
            }
        }
        else
        {
            mu8RoomTemp2ErrorDetectTime = 0;
        }
    }
    else
    {
        mu8RoomTemp2ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_ROOM_TEMP_2_E52] == SET)
        {
            Set_ErrorStatus(ERROR_ID_ROOM_TEMP_2_E52, CLEAR);
        }
    }

#endif
}


/// @brief      �¼� �µ� ���� �̻� ���� üũ
/// @details    �¼� �µ� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void HotTempErrorCheck(void)
{
#if (TANK_HOT_TEMP_ERROR_CHECK_USE == USE)    // �¼� ��ũ �µ� ���� ���� ����

    U16 mu16HotTankTemp_AD = 0;
    U8 mu8HotOnOff = 0;

    static U8 mu8HotTankTempErrorDetectTime = 0;

//    mu16HotTankTemp_AD = Get_ADC_Data(ADC_ID_TH_HOT);   // �¼� ��ũ �µ� ���� �µ� AD�� Ȯ��
    mu8HotOnOff = Get_HotOnOff();

    if (mu8HotOnOff == TRUE)
    {   // �¼� ����� ���� ���
        if ( (mu16HotTankTemp_AD > TANK_HOT_TEMP_ERROR_OVER_STANDARD_AD) ||
             (mu16HotTankTemp_AD < TANK_HOT_TEMP_ERROR_UNDER_STANDARD_AD) )
        {
            if (SError.gu8Status[ERROR_ID_TANK_HOT_TEMP_E45] == CLEAR)
            {
                if (mu8HotTankTempErrorDetectTime >= TANK_HOT_TEMP_ERROR_DETECT_TIME)
                {
                    mu8HotTankTempErrorDetectTime = 0;
                    Set_ErrorStatus(ERROR_ID_TANK_HOT_TEMP_E45, SET);
                }
                else
                {
                    mu8HotTankTempErrorDetectTime++;
                }
            }
            else
            {
                mu8HotTankTempErrorDetectTime = 0;
            }
        }
        else
        {
            mu8HotTankTempErrorDetectTime = 0;

            if (SError.gu8Status[ERROR_ID_TANK_HOT_TEMP_E45] == SET)
            {
                Set_ErrorStatus(ERROR_ID_TANK_HOT_TEMP_E45, CLEAR);
            }
        }
    }
    else
    {
        mu8HotTankTempErrorDetectTime = 0;
    }

#endif

#if (HOT_IN_TEMP_ERROR_CHECK_USE == USE)    // ���� �¼� �Լ� �µ� ���� ���� ����

    U16 mu16HotInTemp_AD = 0;
    static U8 mu8HotInTempErrorDetectTime = 0;

    mu16HotInTemp_AD = Get_ADC_Data(ADC_ID_TH_HOT_IN);   // ���� �¼� �Լ� �µ� ���� �µ� AD�� Ȯ��

    if ( (mu16HotInTemp_AD > HOT_IN_TEMP_ERROR_OVER_STANDARD_AD) ||
         (mu16HotInTemp_AD < HOT_IN_TEMP_ERROR_UNDER_STANDARD_AD) )
    {
        if (SError.gu8Status[ERROR_ID_HOT_IN_TEMP_E46] == CLEAR)
        {
            if (mu8HotInTempErrorDetectTime >= HOT_IN_TEMP_ERROR_DETECT_TIME)
            {
                mu8HotInTempErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_HOT_IN_TEMP_E46, SET);
            }
            else
            {
                mu8HotInTempErrorDetectTime++;
            }
        }
        else
        {
            mu8HotInTempErrorDetectTime = 0;
        }
    }
    else
    {
        mu8HotInTempErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_HOT_IN_TEMP_E46] == SET)
        {
            Set_ErrorStatus(ERROR_ID_HOT_IN_TEMP_E46, CLEAR);
        }
    }

#endif

#if (HOT_OUT_TEMP_ERROR_CHECK_USE == USE)   // ���� �¼� ��� �µ� ���� ���� ����

    U16 mu16HotOutTemp_AD = 0;
    static U8 mu8HotOutTempErrorDetectTime = 0;

    mu16HotOutTemp_AD = Get_ADC_Data(ADC_ID_TH_HOT_OUT);   // ���� �¼� ��� �µ� ���� �µ� AD�� Ȯ��

    if ( (mu16HotOutTemp_AD > HOT_OUT_TEMP_ERROR_OVER_STANDARD_AD) ||
         (mu16HotOutTemp_AD < HOT_OUT_TEMP_ERROR_UNDER_STANDARD_AD) )
    {
        if (SError.gu8Status[ERROR_ID_HOT_OUT_TEMP_E47] == CLEAR)
        {
            if (mu8HotOutTempErrorDetectTime >= HOT_OUT_TEMP_ERROR_DETECT_TIME)
            {
                mu8HotOutTempErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_HOT_OUT_TEMP_E47, SET);
            }
            else
            {
                mu8HotOutTempErrorDetectTime++;
            }
        }
        else
        {
            mu8HotOutTempErrorDetectTime = 0;
        }
    }
    else
    {
        mu8HotOutTempErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_HOT_OUT_TEMP_E47] == SET)
        {
            Set_ErrorStatus(ERROR_ID_HOT_OUT_TEMP_E47, CLEAR);
        }
    }

#endif

#if (HOT_BODY_TEMP_ERROR_CHECK_USE == USE)  // ���� �¼� ���� ���� �µ� ���� ���� ����

    U16 mu16HotBodyTemp_AD = 0;
    static U8 mu8HotBodyTempErrorDetectTime = 0;

    mu16HotBodyTemp_AD = Get_ADC_Data(ADC_ID_TH_HOT_BODY);   // ���� �¼� ���� ���� �µ� ���� �µ� AD�� Ȯ��

    if ( (mu16HotBodyTemp_AD > HOT_BODY_TEMP_ERROR_OVER_STANDARD_AD) ||
         (mu16HotBodyTemp_AD < HOT_BODY_TEMP_ERROR_UNDER_STANDARD_AD) )
    {
        if (SError.gu8Status[ERROR_ID_HOT_BODY_TEMP_E48] == CLEAR)
        {
            if (mu8HotBodyTempErrorDetectTime >= HOT_OUT_TEMP_ERROR_DETECT_TIME)
            {
                mu8HotBodyTempErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_HOT_BODY_TEMP_E48, SET);
            }
            else
            {
                mu8HotBodyTempErrorDetectTime++;
            }
        }
        else
        {
            mu8HotBodyTempErrorDetectTime = 0;
        }
    }
    else
    {
        mu8HotBodyTempErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_HOT_BODY_TEMP_E48] == SET)
        {
            Set_ErrorStatus(ERROR_ID_HOT_BODY_TEMP_E48, CLEAR);
        }
    }

#endif

}


/// @brief      �ü� �µ� ���� �̻� ���� üũ
/// @details    �ü� �µ� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void ColdTempErrorCheck(void)
{
#if (COLD_TEMP_1_ERROR_CHECK_USE == USE)    // �ü� �µ� ����1 ���� ����

    U16 mu16ColdTemp1_AD = 0;
    U8 mu8ColdOnOff1 = 0;

    static U8 mu8ColdTemp1ErrorDetectTime = 0;

    mu16ColdTemp1_AD = Get_ADC_Data(ADC_ID_TH_COOL);   // �ü� �µ� ����1 �µ� AD�� Ȯ��
    mu8ColdOnOff1 = Get_ColdOnOff();

    if (mu8ColdOnOff1 == TRUE)
    {   // �ü� ����� ���� ���
        if ( (mu16ColdTemp1_AD > COLD_TEMP_1_ERROR_OVER_STANDARD_AD) ||
             (mu16ColdTemp1_AD < COLD_TEMP_1_ERROR_UNDER_STANDARD_AD) )
        {
            if (SError.gu8Status[ERROR_ID_COLD_TEMP_1_E44] == CLEAR)
            {
                if (mu8ColdTemp1ErrorDetectTime >= COLD_TEMP_1_ERROR_DETECT_TIME)
                {
                    mu8ColdTemp1ErrorDetectTime = 0;
                    Set_ErrorStatus(ERROR_ID_COLD_TEMP_1_E44, SET);
                }
                else
                {
                    mu8ColdTemp1ErrorDetectTime++;
                }
            }
            else
            {
                mu8ColdTemp1ErrorDetectTime = 0;
            }
        }
        else
        {
            mu8ColdTemp1ErrorDetectTime = 0;

            if (SError.gu8Status[ERROR_ID_COLD_TEMP_1_E44] == SET)
            {
                Set_ErrorStatus(ERROR_ID_COLD_TEMP_1_E44, CLEAR);
            }
        }
    }
    else
    {
        mu8ColdTemp1ErrorDetectTime = 0;
    }

#endif

#if (COLD_TEMP_2_ERROR_CHECK_USE == USE)    // �ü� �µ� ����2 ���� ����

    U16 mu16ColdTemp2_AD = 0;
    U8 mu8ColdOnOff2 = 0;

    static U8 mu8ColdTemp2ErrorDetectTime = 0;

    mu16ColdTemp2_AD = Get_ADC_Data(ADC_ID_TH_COOL_2);   // �ü� �µ� ����2 �µ� AD�� Ȯ��
    mu8ColdOnOff2 = Get_ColdOnOff();

    if (mu8ColdOnOff2 == TRUE)
    {   // �ü� ����� ���� ���
        if ( (mu16ColdTemp2_AD > COLD_TEMP_2_ERROR_OVER_STANDARD_AD) ||
             (mu16ColdTemp2_AD < COLD_TEMP_2_ERROR_UNDER_STANDARD_AD) )
        {
            if (SError.gu8Status[ERROR_ID_COLD_TEMP_2_E54] == CLEAR)
            {
                if (mu8ColdTemp2ErrorDetectTime >= COLD_TEMP_2_ERROR_DETECT_TIME)
                {
                    mu8ColdTemp2ErrorDetectTime = 0;
                    Set_ErrorStatus(ERROR_ID_COLD_TEMP_2_E54, SET);
                }
                else
                {
                    mu8ColdTemp2ErrorDetectTime++;
                }
            }
            else
            {
                mu8ColdTemp2ErrorDetectTime = 0;
            }
        }
        else
        {
            mu8ColdTemp2ErrorDetectTime = 0;

            if (SError.gu8Status[ERROR_ID_COLD_TEMP_2_E54] == SET)
            {
                Set_ErrorStatus(ERROR_ID_COLD_TEMP_2_E54, CLEAR);
            }
        }
    }
    else
    {
        mu8ColdTemp2ErrorDetectTime = 0;
    }

#endif
}


/// @brief      ���̽� Ʈ���� �Լ� �µ� ���� �̻� ���� üũ
/// @details    ���̽� Ʈ���� �Լ� �µ� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void IceTrayInputTempErrorCheck(void)
{
#if (ICE_TRAY_INPUT_TEMP_ERROR_CHECK_USE == USE)   // ���̽� Ʈ���� �Լ� �µ� ���� ���� ����

    U16 mu16IceTrayInputTemp_AD = 0;
    static U8 mu8IceTrayInputTempErrorDetectTime = 0;

    mu16IceTrayInputTemp_AD = Get_ADC_Data(ADC_ID_TH_ICE_TRAY_INPUT);   // ���̽� Ʈ���� �Լ� �µ� ���� �µ� AD�� Ȯ��

    if ( (mu16IceTrayInputTemp_AD > ICE_TRAY_INPUT_TEMP_ERROR_OVER_STANDARD_AD) ||
         (mu16IceTrayInputTemp_AD < ICE_TRAY_INPUT_TEMP_ERROR_UNDER_STANDARD_AD) )
    {
        if (SError.gu8Status[ERROR_ID_ICE_TRAY_INPUT_TEMP_E56] == CLEAR)
        {
            if (mu8IceTrayInputTempErrorDetectTime >= ICE_TRAY_INPUT_TEMP_ERROR_DETECT_TIME)
            {
                mu8IceTrayInputTempErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_ICE_TRAY_INPUT_TEMP_E56, SET);
            }
            else
            {
                mu8IceTrayInputTempErrorDetectTime++;
            }
        }
        else
        {
            mu8IceTrayInputTempErrorDetectTime = 0;
        }
    }
    else
    {
        mu8IceTrayInputTempErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_ICE_TRAY_INPUT_TEMP_E56] == SET)
        {
            Set_ErrorStatus(ERROR_ID_ICE_TRAY_INPUT_TEMP_E56, CLEAR);
        }
    }

#endif
}


/// @brief      �ܱ� �µ� ���� �̻� ���� üũ
/// @details    �ܱ� �µ� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void AirTempErrorCheck(void)
{
#if (AIR_TEMP_1_ERROR_CHECK_USE == USE)     // �ܱ� �µ� ����1 ���� ����

    U16 mu16AirTemp1_AD = 0;

    static U8 mu8AirTemp1ErrorDetectTime = 0;

    mu16AirTemp1_AD = Get_ADC_Data(ADC_ID_TH_AIR);   // �ܱ� �µ� ����1 �µ� AD�� Ȯ��

    if ( (mu16AirTemp1_AD > AIR_TEMP_1_ERROR_OVER_STANDARD_AD) ||
         (mu16AirTemp1_AD < AIR_TEMP_1_ERROR_UNDER_STANDARD_AD) )
    {
        if (SError.gu8Status[ERROR_ID_AIR_TEMP_1_E43] == CLEAR)
        {
            if (mu8AirTemp1ErrorDetectTime >= AIR_TEMP_1_ERROR_DETECT_TIME)
            {
                mu8AirTemp1ErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_AIR_TEMP_1_E43, SET);
            }
            else
            {
                mu8AirTemp1ErrorDetectTime++;
            }
        }
        else
        {
            mu8AirTemp1ErrorDetectTime = 0;
        }
    }
    else
    {
        mu8AirTemp1ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_AIR_TEMP_1_E43] == SET)
        {
            Set_ErrorStatus(ERROR_ID_AIR_TEMP_1_E43, CLEAR);
        }
    }

#endif

#if (AIR_TEMP_2_ERROR_CHECK_USE == USE)     // �ܱ� �µ� ����2 ���� ����

    U16 mu16AirTemp2_AD = 0;

    static U8 mu8AirTemp2ErrorDetectTime = 0;

    mu16AirTemp2_AD = Get_ADC_Data(ADC_ID_TH_AIR_2);   // �ܱ� �µ� ����2 �µ� AD�� Ȯ��

    if ( (mu16AirTemp2_AD > AIR_TEMP_2_ERROR_OVER_STANDARD_AD) ||
         (mu16AirTemp2_AD < AIR_TEMP_2_ERROR_UNDER_STANDARD_AD) )
    {
        if (SError.gu8Status[ERROR_ID_AIR_TEMP_2_E53] == CLEAR)
        {
            if (mu8AirTemp2ErrorDetectTime >= AIR_TEMP_2_ERROR_DETECT_TIME)
            {
                mu8AirTemp2ErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_AIR_TEMP_2_E53, SET);
            }
            else
            {
                mu8AirTemp2ErrorDetectTime++;
            }
        }
        else
        {
            mu8AirTemp2ErrorDetectTime = 0;
        }
    }
    else
    {
        mu8AirTemp2ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_AIR_TEMP_2_E53] == SET)
        {
            Set_ErrorStatus(ERROR_ID_AIR_TEMP_2_E53, CLEAR);
        }
    }

#endif
}


/// @brief      TEM �濭�� �µ� ���� �̻� ���� üũ
/// @details    TEM �濭�� �µ� ���� �̻� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void HeatsinkTempErrorCheck(void)
{
#if (TEM_HEATSINK_TEMP_ERROR_CHECK_USE == USE)

    F32 mf32HeatsinkTemp = 0;

    static U8 mu8HeatsinkTempErrorDetectTime = 0;

    mf32HeatsinkTemp = Get_Temp(ADC_ID_TH_HEATSINK);   // TEM �濭�� �µ� ���� �µ��� Ȯ��

    if (mf32HeatsinkTemp >= TEM_HEATSINK_TEMP_ERROR_OVER_STANDARD_TEMP)
    {   // ���� �µ� �̻��� ���
        if (SError.gu8Status[ERROR_ID_TEM_HEATSINK_TEMP_E25] == CLEAR)
        {
            if (mu8HeatsinkTempErrorDetectTime >= TEM_HEATSINK_TEMP_ERROR_DETECT_TIME)
            {
                mu8HeatsinkTempErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_TEM_HEATSINK_TEMP_E25, SET);
            }
            else
            {
                mu8HeatsinkTempErrorDetectTime++;
            }
        }
        else
        {
            mu8HeatsinkTempErrorDetectTime = 0;
        }
    }
    else
    {
        mu8HeatsinkTempErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_TEM_HEATSINK_TEMP_E25] == SET)
        {
            Set_ErrorStatus(ERROR_ID_TEM_HEATSINK_TEMP_E25, CLEAR);
        }
    }

#endif
}


/// @brief      �¼� ���� ���� üũ
/// @details    �¼� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void HotOverheatErrorCheck(void)
{
#if (HOT_OVERHEAT_ERROR_CHECK_USE == USE)

#if (HOT_OVERHEAT_ERROR_TANK_USE == USE)    // ��ũ�� ��ǰ�� ���

    F32 mf32HotTemp = 0;
    U8 mu8HotOnOff = 0;

    static U16 mu16HotOverheatErrorDetectTime = 0;

    mf32HotTemp = Get_Temp(HOT_THERMISTOR_ID);   // �¼� �µ� ���� �µ��� Ȯ��
    mu8HotOnOff = Get_HotOnOff();

    if (mu8HotOnOff == TRUE)
    {   // �¼� ����� ���� ���
        if (mf32HotTemp >= HOT_OVERHEAT_ERROR_OVER_STANDARD_TEMP)
        {   // ���� �µ� �̻��� ���
            if (SError.gu8Status[ERROR_ID_HOT_OVERHEAT_E40] == CLEAR)
            {
                if (mu16HotOverheatErrorDetectTime >= HOT_OVERHEAT_ERROR_DETECT_TIME)
                {
                    mu16HotOverheatErrorDetectTime = 0;
                    Set_ErrorStatus(ERROR_ID_HOT_OVERHEAT_E40, SET);
                }
                else
                {
                    mu16HotOverheatErrorDetectTime++;
                }
            }
            else
            {
                mu16HotOverheatErrorDetectTime = 0;
            }
        }
        else
        {
            mu16HotOverheatErrorDetectTime = 0;

            if (SError.gu8Status[ERROR_ID_HOT_OVERHEAT_E40] == SET)
            {
                Set_ErrorStatus(ERROR_ID_HOT_OVERHEAT_E40, CLEAR);
            }
        }
    }
    else
    {   // �¼� ����� ���� ���
        mu16HotOverheatErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_HOT_OVERHEAT_E40] == SET)
        {
            Set_ErrorStatus(ERROR_ID_HOT_OVERHEAT_E40, CLEAR);
        }
    }

#else   // ������ ��ǰ�� ���

    F32 mf32HotTemp = 0;

    static U8 mu8HotOverheatErrorDetectTime = 0;

    mf32HotTemp = Get_Temp(ADC_ID_TH_HOT_OUT);   // �¼� ��� �µ� ���� �µ��� Ȯ��

    if (mf32HotTemp >= HOT_OVERHEAT_ERROR_OVER_STANDARD_TEMP)
    {   // ���� �µ� �̻��� ���
        if (SError.gu8Status[ERROR_ID_HOT_OVERHEAT_E40] == CLEAR)
        {
            if (mu8HotOverheatErrorDetectTime >= HOT_OVERHEAT_ERROR_DETECT_TIME)
            {
                mu8HotOverheatErrorDetectTime = 0;
                Set_ErrorStatus(ERROR_ID_HOT_OVERHEAT_E40, SET);
            }
            else
            {
                mu8HotOverheatErrorDetectTime++;
            }
        }
        else
        {
            mu8HotOverheatErrorDetectTime  = 0;
        }
    }
    else
    {
        mu8HotOverheatErrorDetectTime = 0;

        if (mf32HotTemp < HOT_OVERHEAT_ERROR_AUTO_CLEAR_TEMP)
        {
            if (SError.gu8Status[ERROR_ID_HOT_OVERHEAT_E40] == SET)
            {
                Set_ErrorStatus(ERROR_ID_HOT_OVERHEAT_E40, CLEAR);
            }
        }
    }

#endif

#endif
}


/// @brief      �¼� ���� ���� üũ
/// @details    �¼� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void HotPumpErrorCheck(void)
{
#if (HOT_PUMP_ERROR_CHECK_USE == USE)
    static U8 mu8ErrorDetectTime = 0;
    U8 mu8HotPumpStatus = 0;
    U16 mu16HotPumpFeedbackAD = 0;

    mu8HotPumpStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_HOT);    // �¼� ������ ���� ���� ���� Ȯ��
    mu16HotPumpFeedbackAD = Get_ADC_Data(ADC_ID_HOT_PUMP);       // �ν�Ʈ ������ �ǵ�� AD�� Ȯ��

    if (mu8HotPumpStatus == TRUE)
    {   // ������ ���� ���� ���
        if ( (mu16HotPumpFeedbackAD < HOT_PUMP_ERROR_UNDER_STANDARD_AD) ||
             (mu16HotPumpFeedbackAD > HOT_PUMP_ERROR_OVER_STANDARD_AD) )
        {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
            if (mu8ErrorDetectTime >= HOT_PUMP_ERROR_DETECT_TIME)
            {
                Set_ErrorStatus(ERROR_ID_HOT_PUMP_E30, SET);
            }
            else
            {
                mu8ErrorDetectTime++;
            }
        }
        else
        {
            mu8ErrorDetectTime = 0;
        }
    }
    else
    {   // ������ ���� ���� �ƴ� ���
        mu8ErrorDetectTime = 0;
    }
#endif
}


/// @brief      BLDC Comp ���� üũ
/// @details    BLDC Comp  ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void BLDC_CompErrorCheck(void)
{
#if (BLDC_COMP_ERROR_CHECK_USE == USE)
    U8 mu8ErrorCode = 0;

    mu8ErrorCode = Get_BLDC_Comp_Error();   // BLDC Comp ���� Ȯ��

    switch (mu8ErrorCode)
    {
        case BLDC_COMP_ERROR_81 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_1_E81, SET);     // ���� Ȯ��
            break;

        case BLDC_COMP_ERROR_82 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_2_E82, SET);     // ���� Ȯ��
            break;

        case BLDC_COMP_ERROR_83 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_3_E83, SET);     // ���� Ȯ��
            break;

        case BLDC_COMP_ERROR_84 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_4_E84, SET);     // ���� Ȯ��
            break;

        case BLDC_COMP_ERROR_85 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_5_E85, SET);     // ���� Ȯ��
            break;

        case BLDC_COMP_ERROR_COMM :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_COMMUNICATION_E27, SET);     // ���� Ȯ��
            break;

        default:
            break;
    }
#endif
}


/// @brief      �Ϲ� Comp ���� �ҷ� ���� üũ(���� ���� ��� ǥ�� ���� ������ ���� ���� - �̻��)
/// @details    �Ϲ� Comp ���� �ҷ� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void CompActErrorCheck(void)
{
#if (COMP_ACT_ERROR_CHECK_USE == USE)

#endif
}


/// @brief      ��ȯ ���� ���� üũ
/// @details    ��ȯ ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void CirculatePumpErrorCheck(void)
{
#if (CIRCULATE_PUMP_ERROR_CHECK_USE == USE)
    U8 mu8PumpStatus = 0;
    U16 mu8PumpFeedbackAD = 0;

    static U8 mu8ErrorDetectTime = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8PumpStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_CIRCULATE);         // ��ȯ ���� ���� ���� Ȯ��
    mu8PumpFeedbackAD = Get_ADC_Data(ADC_ID_CIRCULATION_PUMP_I_FB);     // ��ȯ ���� ���� �ǵ�� ���� Ȯ��

    if (mu8PumpStatus == TRUE)
    {   // ������ ���� ���� ���
        if ( (mu8PumpFeedbackAD < CIRCULATE_PUMP_ERROR_UNDER_STANDARD_AD) ||
             (mu8PumpFeedbackAD > CIRCULATE_PUMP_ERROR_OVER_STANDARD_AD) )
        {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
            if (SError.gu8Detect[ERROR_ID_CIRCULATE_PUMP_E26] == CLEAR)
            {   // ���� �̰��� ������ ���
                if (mu8ErrorDetectTime >= CIRCULATE_PUMP_ERROR_DETECT_TIME)
                {   // ���� ���� ���� �ð� �����
                    mu8ErrorDetectTime = 0;

                    Set_ErrorDetect(ERROR_ID_CIRCULATE_PUMP_E26, SET);      // ���� ���� ���·� ����

                    if (mu8ErrorDetectCount >= CIRCULATE_PUMP_ERROR_CONFIRM_COUNT)
                    {   // ���� ���� ���°� ���� Ƚ�� ��ŭ �ݺ� �߻��� ���
                        Set_ErrorStatus(ERROR_ID_CIRCULATE_PUMP_E26, SET);  // ���� Ȯ��
                    }
                    else
                    {
                        mu8ErrorDetectCount++;
                    }
                }
                else
                {
                    mu8ErrorDetectTime++;
                }
            }
            else
            {   // ���� ���� ������ ���
                mu8ErrorDetectTime = 0;
            }
        }
        else
        {   // ���� ������ ���
            mu8ErrorDetectTime = 0;
            mu8ErrorDetectCount = 0;

            if (SError.gu8Status[ERROR_ID_CIRCULATE_PUMP_E26] == CLEAR)
            {   // ���� �̹߻� ������ ���
                if (SError.gu8Detect[ERROR_ID_CIRCULATE_PUMP_E26] == SET)
                {   // ���� ������ �Ǿ��� ���
                    Set_ErrorDetect(ERROR_ID_CIRCULATE_PUMP_E26, CLEAR);    // ���� ��˻縦 ���� ���� ���� CLEAR
                }
            }
        }
    }
    else
    {   // ������ ���� ���� �ƴ� ���
        mu8ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_CIRCULATE_PUMP_E26] == CLEAR)
        {   // ���� �̹߻� ������ ���
            if (SError.gu8Detect[ERROR_ID_CIRCULATE_PUMP_E26] == SET)
            {   // ���� ������ �Ǿ��� ���
                Set_ErrorDetect(ERROR_ID_CIRCULATE_PUMP_E26, CLEAR);    // ���� ��˻縦 ���� ���� ���� CLEAR
            }
        }
    }
#endif
}


/// @brief      ���� ���� ���� üũ
/// @details    ���� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void StirringMoterErrorCheck(void)
{
#if (STIRRING_MOTOR_ERROR_CHECK_USE == USE)
    U8 mu8MotorStatus = 0;
    U16 mu8MotorFeedbackAD = 0;

    static U8 mu8ErrorCheckDelay = 0;
    static U8 mu8ErrorDetectTime = 0;
    static U8 mu8ErrorDetectBefore = 0;
    static U16 mu16ErrorRecheckDelay = 0;

    static U8 mu8ErrorConfirmTimeSec = 0;
    static U8 mu8ErrorConfirmTimeMin = 0;
    static U16 mu16ErrorConfirmTimeHour = 0;

    mu8MotorStatus = Get_StirringMotorStatus_HW(STIRRING_MOTOR_ID_1);   // ���� ���� ���� ���� Ȯ��
    mu8MotorFeedbackAD = Get_ADC_Data(ADC_ID_STIRRING_MOTOR);            // ���� ���� �ǵ�� ���� Ȯ��

    if (mu8ErrorDetectBefore == SET)
    {   // ���� ���ۿ��� ���� ������ �̷��� �ִ� ���
        if (SError.gu8Status[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
        {   // ���� Ȯ���� �ȵ� ���
            if (++mu8ErrorConfirmTimeSec >= 10)
            {   // 1�� ī��Ʈ
                if (++mu8ErrorConfirmTimeMin >= 60)
                {   // 1�� ī��Ʈ
                    mu8ErrorConfirmTimeMin = 0;

                    if (++mu16ErrorConfirmTimeHour >= STIRRING_MOTOR_ERROR_CONFIRM_TIME_HOUR)
                    {   // 48�ð� �����
                        mu16ErrorConfirmTimeHour = 0;
                        Set_ErrorStatus(ERROR_ID_STIRRING_MOTOR_E28, SET);  // ���� Ȯ��
                    }
                }
            }
        }
        else
        {   // ���� Ȯ���� ���
            mu8ErrorDetectBefore = CLEAR;
            mu8ErrorConfirmTimeSec = 0;
            mu8ErrorConfirmTimeMin = 0;
            mu16ErrorConfirmTimeHour = 0;
        }
    }
    else
    {
        mu8ErrorDetectBefore = CLEAR;
        mu8ErrorConfirmTimeSec = 0;
        mu8ErrorConfirmTimeMin = 0;
        mu16ErrorConfirmTimeHour = 0;
    }

    if (SError.gu8Status[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
    {   // ���� Ȯ���� �ȵ� ���
        if (mu8MotorStatus == TRUE)
        {   // ���Ͱ� ���� ���� ���
            if (mu8ErrorCheckDelay >= STIRRING_MOTOR_ERROR_CHECK_DELAY)
            {   // ���� �ð� ����
                if ( (mu8MotorFeedbackAD < STIRRING_MOTOR_ERROR_UNDER_STANDARD_AD) ||
                     (mu8MotorFeedbackAD > STIRRING_MOTOR_ERROR_OVER_STANDARD_AD) )
                {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                    if (SError.gu8Detect[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
                    {   // ���� �̰��� ������ ���
                        if (mu8ErrorDetectTime >= STIRRING_MOTOR_ERROR_DETECT_TIME)
                        {   // ���� ���� ���� �ð� �����
                            mu8ErrorDetectTime = 0;

                            Set_ErrorDetect(ERROR_ID_STIRRING_MOTOR_E28, SET);      // ���� ���� ���·� ����
                            mu8ErrorDetectBefore = SET;                         // ���� ���°� ���� ���� ���·� ����
                        }
                        else
                        {
                            mu8ErrorDetectTime++;
                        }
                    }
                    else
                    {   // ���� ���� ������ ���
                        mu8ErrorDetectTime = 0;
                    }
                }
                else
                {   // ���� ������ ���
                    mu8ErrorDetectTime = 0;
                    mu8ErrorDetectBefore = CLEAR;
                    mu16ErrorRecheckDelay = 0;

                    if (SError.gu8Status[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
                    {   // ���� �̹߻� ������ ���
                        if (SError.gu8Detect[ERROR_ID_STIRRING_MOTOR_E28] == SET)
                        {   // ���� ������ �Ǿ��� ���
                            Set_ErrorDetect(ERROR_ID_STIRRING_MOTOR_E28, CLEAR);    // ���� ��˻縦 ���� ���� ���� CLEAR
                        }
                    }
                }
            }
            else
            {   // ���� �ð� ����
                mu8ErrorCheckDelay++;
            }
        }
        else
        {   // ���Ͱ� ���� ���� �ƴ� ���
            mu8ErrorCheckDelay = 0;
            mu8ErrorDetectTime = 0;

            if (SError.gu8Status[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
            {   // ���� �̹߻� ������ ���
                if (SError.gu8Detect[ERROR_ID_STIRRING_MOTOR_E28] == SET)
                {   // ���� ������ �Ǿ��� ���
                    if (mu16ErrorRecheckDelay >= STIRRING_MOTOR_ERROR_RE_CHECK_DELAY)
                    {   // �絿�� �� ���� �ð� ���� ��˻縦 ���� ���� ���¸� �ʱ�ȭ
                        mu16ErrorRecheckDelay = 0;

                        Set_ErrorDetect(ERROR_ID_STIRRING_MOTOR_E28, CLEAR);    // ���� ��˻縦 ���� ���� ���� CLEAR
                    }
                    else
                    {
                        mu16ErrorRecheckDelay++;
                    }
                }
                else
                {   // ���� ������ �� ���� ������
                    mu16ErrorRecheckDelay = 0;
                }
            }
        }
    }
    else
    {   // ������ Ȯ���� ���
        mu8ErrorCheckDelay = 0;
        mu8ErrorDetectTime = 0;
        mu16ErrorRecheckDelay = 0;
    }
#endif
}


/// @brief      EW ��� ��� ���� üũ
/// @details    EW ��� ��� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void EW_ModuleErrorCheck(void)
{
#if (EW_MODULE_ERROR_CHECK_USE == USE)
    U8 mu8EWStatus = 0;
    U16 mu16EW_AD = 0;
    U8 mu8Status = 0;

    static U8 mu8ErrorDetectTime = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8EWStatus = GET_STATUS_EW();
    mu8Status = Get_OperatingStatus();

    if (mu8EWStatus == ON)
    {   // EW�� ���� ���̸�
        mu16EW_AD = Get_ADC_Data(ADC_ID_EW_I_FB);       // EW ��� ���� �ǵ�� Ȯ��

        if ( (mu16EW_AD < EW_MODULE_ERROR_UNDER_STANDARD_AD) ||
             (mu16EW_AD > EW_MODULE_ERROR_OVER_STANDARD_AD) )
        {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
            if (SError.gu8Status[ERROR_ID_EW_MODULE_E71] == CLEAR)
            {   // ���� �̹߻� ������ ���
                if (mu8ErrorDetectTime >= EW_MODULE_ERROR_DETECT_TIME)
                {   // ���� ���� ���� �ð� �����
                    mu8ErrorDetectTime = 0;

                    if (mu8ErrorDetectCount >= EW_MODULE_ERROR_DETECT_COUNT)
                    {   // �ݺ� ���� ���� Ƚ�� �̻��� ���
                        mu8ErrorDetectCount = 0;

                        Set_ErrorStatus(ERROR_ID_EW_MODULE_E71, SET);
                    }
                    else
                    {
                        mu8ErrorDetectCount++;
                    }
                }
                else
                {
                    mu8ErrorDetectTime++;
                }
            }
            else
            {   // ���� �߻� ������ ���
                mu8ErrorDetectTime = 0;
                mu8ErrorDetectCount = 0;
            }
        }
        else
        {   // ������ ���
            mu8ErrorDetectTime = 0;
            mu8ErrorDetectCount = 0;

            if (SError.gu8Status[ERROR_ID_EW_MODULE_E71] == SET)
            {   // ���� �߻� ������ ���
                Set_ErrorStatus(ERROR_ID_EW_MODULE_E71, CLEAR);
            }
        }
    }
    else
    {   // EW�� ���� ���� �ƴ� ���
        mu8ErrorDetectTime = 0;
        mu8ErrorDetectCount = 0;

        if (mu8Status != OPERATION_ID_EW_STERILIZE)
        {   // ��� ���� ���� �ƴ� ���
            if (SError.gu8Status[ERROR_ID_EW_MODULE_E71] == SET)
            {   // ���� �߻� ������ ���
                Set_ErrorStatus(ERROR_ID_EW_MODULE_E71, CLEAR);
            }
        }
    }
#endif
}


/// @brief      ��� �巹�� ���� ���� ���� üũ
/// @details    ��� �巹�� ���� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void EW_DrainPumpErrorCheck(void)
{
#if (EW_DRAIN_PUMP_ERROR_CHECK_USE == USE)
    U8 mu8Status = 0;
    U8 mu8WaterLevel = 0;
    U8 mu8DrainStatus = 0;
    U16 mu16DrainPumpAD = 0;

    static U16 mu16ActTime = 0;

    mu8Status = Get_OperatingStatus();                          // ��ǰ ���� ���� Ȯ��
    mu8WaterLevel = Get_WaterLevel();                           // ���� ��ũ ���� Ȯ��
    mu8DrainStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_DRAIN);    // �巹�� ���� ���� ���� Ȯ��
    mu16DrainPumpAD = Get_ADC_Data(ADC_ID_DRAIN_PUMP);          // �巹�� ���� �ǵ�� AD�� Ȯ��

    if (mu8Status == OPERATION_ID_EW_STERILIZE)
    {   // ��� ���� ���� ���
        if (SError.gu8Status[ERROR_ID_EW_DRAIN_PUMP_E72] == CLEAR)
        {   // �巹�� ���� ������ �߻����� ���� ���
            if (mu8DrainStatus == TRUE)
            {   // �巹�� ���� ���� ���
                if (mu8WaterLevel > WATER_LEVEL_LOW)
                {   // ������ ������ �Ѱ� ���ǰ� �ִ� ���
                    if ( (mu16DrainPumpAD < EW_DRAIN_PUMP_ERROR_UNDER_STANDARD_AD) ||
                         (mu16DrainPumpAD > EW_DRAIN_PUMP_ERROR_OVER_STANDARD_AD) )
                    {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                        Set_ErrorStatus(ERROR_ID_EW_DRAIN_PUMP_E72, SET);
                    }
                    else
                    {   // ���� ������ ���
                        if (mu16ActTime >= EW_DRAIN_PUMP_ERROR_DETECT_TIME)
                        {
                            mu16ActTime = 0;

                            Set_ErrorStatus(ERROR_ID_EW_DRAIN_PUMP_E72, SET);
                        }
                        else
                        {
                            mu16ActTime++;
                        }
                    }
                }
            }
            else
            {   // �巹�� ���� ���� �ƴ� ���
                mu16ActTime = 0;
            }
        }
        else
        {   // ������ �߻��� ���
            mu16ActTime = 0;
        }
    }
    else
    {   // ��� ���� ���� �ƴ� ���
        mu16ActTime = 0;
    }
#endif
}


/// @brief      UV ���� ���� üũ
/// @details    UV ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void UVErrorCheck(void)
{
#if (UV_ERROR_CHECK_USE == USE)
    U8 mu8UVStelizeStatus = 0;
    U16 mu16UVFeedbackAD = 0;

    static U8 mu8ErrorDetectTime_UV1 = 0;
    static U8 mu8ErrorDetectTime_UV2 = 0;
    static U8 mu8ErrorDetectTime_UV3 = 0;
    static U8 mu8ErrorDetectTime_UV4 = 0;
    static U8 mu8ErrorDetectTime_UV5 = 0;
    static U8 mu8ErrorDetectTime_UV6 = 0;
    static U8 mu8ErrorDetectTime_UV7 = 0;

    // UV1 ���� üũ
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_WATER_TANK);   // ��� ���� Ȯ��
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // ��� UV �ǵ�� AD�� Ȯ��

    if (mu8UVStelizeStatus == TRUE)
    {   // UV ��� ���� ���� ���
        if (SError.gu8Status[ERROR_ID_UV_1_E73] == CLEAR)
        {   // UV1 ���� �̹߻���
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                if (mu8ErrorDetectTime_UV1 >= UV_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime_UV1 = 0;

                    Set_ErrorStatus(ERROR_ID_UV_1_E73, SET);
                }
                else
                {
                    mu8ErrorDetectTime_UV1++;
                }
            }
        }
    }
    else
    {   // UV ��� ���� ���� �ƴ� ���
        mu8ErrorDetectTime_UV1 = 0;

        if (SError.gu8Status[ERROR_ID_UV_1_E73] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_1_E73, CLEAR);
        }
    }

    // UV2 ���� üũ
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_COLD_TANK_BACK);   // ��� ���� Ȯ��
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // ��� UV �ǵ�� AD�� Ȯ��

    if (mu8UVStelizeStatus == TRUE)
    {   // UV ��� ���� ���� ���
        if (SError.gu8Status[ERROR_ID_UV_2_E74] == CLEAR)
        {   // UV1 ���� �̹߻���
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                if (mu8ErrorDetectTime_UV2 >= UV_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime_UV2 = 0;

                    Set_ErrorStatus(ERROR_ID_UV_2_E74, SET);
                }
                else
                {
                    mu8ErrorDetectTime_UV2++;
                }
            }
        }
    }
    else
    {   // UV ��� ���� ���� �ƴ� ���
        mu8ErrorDetectTime_UV2 = 0;

        if (SError.gu8Status[ERROR_ID_UV_2_E74] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_2_E74, CLEAR);
        }
    }

    // UV3 ���� üũ
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_ICE_TANK);   // ��� ���� Ȯ��
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // ��� UV �ǵ�� AD�� Ȯ��

    if (mu8UVStelizeStatus == TRUE)
    {   // UV ��� ���� ���� ���
        if (SError.gu8Status[ERROR_ID_UV_3_E75] == CLEAR)
        {   // UV1 ���� �̹߻���
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                if (mu8ErrorDetectTime_UV3 >= UV_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime_UV3 = 0;

                    Set_ErrorStatus(ERROR_ID_UV_3_E75, SET);
                }
                else
                {
                    mu8ErrorDetectTime_UV3++;
                }
            }
        }
    }
    else
    {   // UV ��� ���� ���� �ƴ� ���
        mu8ErrorDetectTime_UV3 = 0;

        if (SError.gu8Status[ERROR_ID_UV_3_E75] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_3_E75, CLEAR);
        }
    }

    // UV4 ���� üũ
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_ICE_TRAY);   // ��� ���� Ȯ��
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // ��� UV �ǵ�� AD�� Ȯ��

    if (mu8UVStelizeStatus == TRUE)
    {   // UV ��� ���� ���� ���
        if (SError.gu8Status[ERROR_ID_UV_4_E76] == CLEAR)
        {   // UV1 ���� �̹߻���
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                if (mu8ErrorDetectTime_UV4 >= UV_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime_UV4 = 0;

                    Set_ErrorStatus(ERROR_ID_UV_4_E76, SET);
                }
                else
                {
                    mu8ErrorDetectTime_UV4++;
                }
            }
        }
    }
    else
    {   // UV ��� ���� ���� �ƴ� ���
        mu8ErrorDetectTime_UV4 = 0;

        if (SError.gu8Status[ERROR_ID_UV_4_E76] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_4_E76, CLEAR);
        }
    }

    // UV5 ���� üũ
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_WATER_FAUCET);   // ��� ���� Ȯ��
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // ��� UV �ǵ�� AD�� Ȯ��

    if (mu8UVStelizeStatus == TRUE)
    {   // UV ��� ���� ���� ���
        if (SError.gu8Status[ERROR_ID_UV_5_E77] == CLEAR)
        {   // UV1 ���� �̹߻���
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                if (mu8ErrorDetectTime_UV5 >= UV_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime_UV5 = 0;

                    Set_ErrorStatus(ERROR_ID_UV_5_E77, SET);
                }
                else
                {
                    mu8ErrorDetectTime_UV5++;
                }
            }
        }
    }
    else
    {   // UV ��� ���� ���� �ƴ� ���
        mu8ErrorDetectTime_UV5 = 0;

        if (SError.gu8Status[ERROR_ID_UV_5_E77] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_5_E77, CLEAR);
        }
    }

    // UV6 ���� üũ
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_ICE_FAUCET);   // ��� ���� Ȯ��
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // ��� UV �ǵ�� AD�� Ȯ��

    if (mu8UVStelizeStatus == TRUE)
    {   // UV ��� ���� ���� ���
        if (SError.gu8Status[ERROR_ID_UV_6_E78] == CLEAR)
        {   // UV1 ���� �̹߻���
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                if (mu8ErrorDetectTime_UV6 >= UV_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime_UV6 = 0;

                    Set_ErrorStatus(ERROR_ID_UV_6_E78, SET);
                }
                else
                {
                    mu8ErrorDetectTime_UV6++;
                }
            }
        }
    }
    else
    {   // UV ��� ���� ���� �ƴ� ���
        mu8ErrorDetectTime_UV6 = 0;

        if (SError.gu8Status[ERROR_ID_UV_6_E78] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_6_E78, CLEAR);
        }
    }

    // UV7 ���� üũ
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_COLD_TANK_FRONT);   // ��� ���� Ȯ��
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // ��� UV �ǵ�� AD�� Ȯ��

    if (mu8UVStelizeStatus == TRUE)
    {   // UV ��� ���� ���� ���
        if (SError.gu8Status[ERROR_ID_UV_7_E79] == CLEAR)
        {   // UV1 ���� �̹߻���
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
                if (mu8ErrorDetectTime_UV7 >= UV_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime_UV7 = 0;

                    Set_ErrorStatus(ERROR_ID_UV_7_E79, SET);
                }
                else
                {
                    mu8ErrorDetectTime_UV7++;
                }
            }
        }
    }
    else
    {   // UV ��� ���� ���� �ƴ� ���
        mu8ErrorDetectTime_UV7 = 0;

        if (SError.gu8Status[ERROR_ID_UV_7_E79] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_7_E79, CLEAR);
        }
    }

#endif
}


/// @brief      TDS �Լ� �µ� ���� ���� üũ
/// @details    TDS �Լ� �µ� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void TDS_In_TempErrorCheck(void)
{
#if (TDS_IN_TEMP_ERROR_CHECK_USE == USE)
    U16 mu16TDSInTempAD = 0;

    static U8 mu8ErrorDetecTime = 0;

    mu16TDSInTempAD = Get_ADC_Data(ADC_ID_TH_TDS_IN);

    if (SError.gu8Status[ERROR_ID_TDS_IN_TEMP_E90] == CLEAR)
    {
        if ( (mu16TDSInTempAD < TDS_IN_TEMP_ERROR_UNDER_STANDARD_AD) ||
             (mu16TDSInTempAD > TDS_IN_TEMP_ERROR_OVER_STANDARD_AD) )
        {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
            if (mu8ErrorDetecTime >= TDS_IN_TEMP_ERROR_DETECT_TIME)
            {
                mu8ErrorDetecTime = 0;

                Set_ErrorStatus(ERROR_ID_TDS_IN_TEMP_E90, SET);
            }
            else
            {
                mu8ErrorDetecTime++;
            }
        }
        else
        {
            mu8ErrorDetecTime = 0;
        }
    }
    else
    {
        mu8ErrorDetecTime = 0;

        if (SError.gu8Status[ERROR_ID_TDS_IN_TEMP_E90] == SET)
        {
            Set_ErrorStatus(ERROR_ID_TDS_IN_TEMP_E90, CLEAR);
        }
    }
#endif
}


/// @brief      TDS ��� �µ� ���� ���� üũ
/// @details    TDS ��� �µ� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void TDS_Out_TempErrorCheck(void)
{
#if (TDS_OUT_TEMP_ERROR_CHECK_USE == USE)
    U16 mu16TDSOutTempAD = 0;

    static U8 mu8ErrorDetecTime = 0;

    mu16TDSOutTempAD = Get_ADC_Data(ADC_ID_TH_TDS_OUT);

    if (SError.gu8Status[ERROR_ID_TDS_OUT_TEMP_E91] == CLEAR)
    {
        if ( (mu16TDSOutTempAD < TDS_OUT_TEMP_ERROR_UNDER_STANDARD_AD) ||
             (mu16TDSOutTempAD > TDS_OUT_TEMP_ERROR_OVER_STANDARD_AD) )
        {   // ���ذ� �̸� �Ǵ� �ʰ��� ���
            if (mu8ErrorDetecTime >= TDS_OUT_TEMP_ERROR_DETECT_TIME)
            {
                mu8ErrorDetecTime = 0;

                Set_ErrorStatus(ERROR_ID_TDS_OUT_TEMP_E91, SET);
            }
            else
            {
                mu8ErrorDetecTime++;
            }
        }
        else
        {
            mu8ErrorDetecTime = 0;
        }
    }
    else
    {
        mu8ErrorDetecTime = 0;

        if (SError.gu8Status[ERROR_ID_TDS_OUT_TEMP_E91] == SET)
        {
            Set_ErrorStatus(ERROR_ID_TDS_OUT_TEMP_E91, CLEAR);
        }
    }

#endif
}


/// @brief      TDS �Լ� �µ� ���� ���� üũ
/// @details    TDS �Լ� �µ� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void TDS_InErrorCheck(void)
{
#if (TDS_IN_ERROR_CHECK_USE == USE)
    U8 mu8TDSInCheckStatus = 0;
    U16 mu16TDSInValue = 0;

    static U8 mu8ErrorDetectTime = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8TDSInCheckStatus = Get_TDS_CheckStatus();        // TDS ���� �������� Ȯ��
    mu16TDSInValue = Get_TDS_In_Value();                // TDS In ���̰� Ȯ��

    if (SError.gu8Status[ERROR_ID_TDS_IN_E92] == CLEAR)
    {   // ���� �߻��� ���� ���
        if (mu8TDSInCheckStatus == TRUE)
        {   // TDS ���� ���� ���
            if (mu16TDSInValue == 0)
            {   // TDS �������� 0�� ���
                if (mu8ErrorDetectTime >= TDS_IN_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime = 0;

                    if (mu8ErrorDetectCount >= TDS_IN_ERROR_DETECT_COUNT)
                    {
                        mu8ErrorDetectCount = 0;

                        Set_EEPROM_Data(ERROR_ID_TDS_IN_E92, SET);
                    }
                    else
                    {
                        mu8ErrorDetectCount++;
                    }
                }
                else
                {
                    mu8ErrorDetectTime++;
                }
            }
            else
            {   // 0�� �ƴ� ���
                mu8ErrorDetectTime = 0;
                mu8ErrorDetectCount = 0;

                if (SError.gu8Status[ERROR_ID_TDS_IN_E92] == SET)
                {
                    Set_EEPROM_Data(ERROR_ID_TDS_IN_E92, CLEAR);
                }
            }
        }
        else
        {   // TDS ���� ���� �ƴ� ���
            mu8ErrorDetectTime = 0;
            mu8ErrorDetectCount = 0;
        }
    }
    else
    {   // ���� �߻��� ���
        mu8ErrorDetectTime = 0;
        mu8ErrorDetectCount = 0;
    }
#endif
}


/// @brief      TDS ��� �µ� ���� ���� üũ
/// @details    TDS ��� �µ� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void TDS_OutErrorCheck(void)
{
#if (TDS_OUT_ERROR_CHECK_USE == USE)
    U8 mu8TDSOutCheckStatus = 0;
    U16 mu16TDSOutValue = 0;

    static U8 mu8ErrorDetectTime = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8TDSOutCheckStatus = Get_TDS_CheckStatus();        // TDS ���� �������� Ȯ��
    mu16TDSOutValue = Get_TDS_Out_Value();               // TDS Out ���̰� Ȯ��

    if (SError.gu8Status[ERROR_ID_TDS_OUT_E93] == CLEAR)
    {   // ���� �߻��� ���� ���
        if (mu8TDSOutCheckStatus == TRUE)
        {   // TDS ���� ���� ���
            if (mu16TDSOutValue == 0)
            {   // TDS �������� 0�� ���
                if (mu8ErrorDetectTime >= TDS_OUT_ERROR_DETECT_TIME)
                {
                    mu8ErrorDetectTime = 0;

                    if (mu8ErrorDetectCount >= TDS_OUT_ERROR_DETECT_COUNT)
                    {
                        mu8ErrorDetectCount = 0;

                        Set_EEPROM_Data(ERROR_ID_TDS_OUT_E93, SET);
                    }
                    else
                    {
                        mu8ErrorDetectCount++;
                    }
                }
                else
                {
                    mu8ErrorDetectTime++;
                }
            }
            else
            {   // 0�� �ƴ� ���
                mu8ErrorDetectTime = 0;
                mu8ErrorDetectCount = 0;

                if (SError.gu8Status[ERROR_ID_TDS_OUT_E93] == SET)
                {
                    Set_EEPROM_Data(ERROR_ID_TDS_OUT_E93, CLEAR);
                }
            }
        }
        else
        {   // TDS ���� ���� �ƴ� ���
            mu8ErrorDetectTime = 0;
            mu8ErrorDetectCount = 0;
        }
    }
    else
    {   // ���� �߻��� ���
        mu8ErrorDetectTime = 0;
        mu8ErrorDetectCount = 0;
    }
#endif
}


/// @brief      �Լ� ���� ���� üũ
/// @details    �Լ� ���� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void InputContaminationErrorCheck(void)
{
#if (INPUT_CONTAMINATION_ERROR_CHECK_USE == USE)
    U8 mu8TDSInCheckStatus = 0;
    U16 mu16TDSInValue = 0;

    static U8 mu8TDSInCheckEnd = 0;
    static U8 mu8ErrorCheckDelay = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8TDSInCheckStatus = Get_TDS_CheckStatus();         // TDS ���� �������� Ȯ��
    mu16TDSInValue = Get_TDS_In_Value();                // TDS Out ���̰� Ȯ��

    if (SError.gu8Status[ERROR_ID_INPUT_CONTAMINATION_E98] == CLEAR)
    {   // ���� �߻��� ���� ���
        if (mu8TDSInCheckStatus == TRUE)
        {   // TDS ���� ���� ���
            if (mu8TDSInCheckEnd == CLEAR)
            {   // �ѹ� üũ�� �Ϸ���� ���� ���
                if (mu8ErrorCheckDelay >= INPUT_CONTAMINATION_ERROR_DETECT_DELAY_TIME)
                {   // üũ �� �����ð� ��� ��
                    mu8ErrorCheckDelay = 0;

                    mu8TDSInCheckEnd = SET;

                    if (mu16TDSInValue >= 500)
                    {   // TDS �������� 500ppm �̻��� ���
                        if (mu8ErrorDetectCount >= INPUT_CONTAMINATION_ERROR_DETECT_COUNT)
                        {
                            mu8ErrorDetectCount = 0;

                            Set_EEPROM_Data(ERROR_ID_INPUT_CONTAMINATION_E98, SET);
                        }
                        else
                        {
                            mu8ErrorDetectCount++;
                        }
                    }
                    else
                    {   // TDS �������� 500ppm �̸��� ���
                        mu8ErrorDetectCount = 0;
                    }
                }
                else
                {
                    mu8ErrorCheckDelay++;
                }
            }
            else
            {   // �ѹ� üũ�� �Ϸ�� ���
                mu8ErrorCheckDelay = 0;
            }
        }
        else
        {   // TDS ���� ���� �ƴ� ���
            mu8TDSInCheckEnd = CLEAR;
            mu8ErrorCheckDelay = 0;
        }
    }
    else
    {   // ���� �߻��� ���
        mu8TDSInCheckEnd = CLEAR;
        mu8ErrorCheckDelay = 0;
        mu8ErrorDetectCount = 0;
    }
#endif
}


/// @brief      ������ �ҷ� ���� üũ
/// @details    ������ �ҷ� ���� üũ ������ �����Ѵ�
/// @param      void
/// @return     void
void DecontaminationErrorCheck(void)
{
#if (DECONTAMINATION_ERROR_CHECK_USE == USE)
    U8 mu8TDSCheckStatus = 0;
    U16 mu16TDSInValue = 0;
    U16 mu16TDSInValueBuf = 0;
    U16 mu16TDSOutValue = 0;

    static U8 mu8TDSCheckEnd = 0;
    static U8 mu8ErrorCheckDelay = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8TDSCheckStatus = Get_TDS_CheckStatus();          // TDS ���� �������� Ȯ��
    mu16TDSInValue = Get_TDS_In_Value();                // TDS Out ���̰� Ȯ��
    mu16TDSOutValue = Get_TDS_Out_Value();              // TDS Out ���̰� Ȯ��

    if (SError.gu8Status[ERROR_ID_DECONTAMINATION_E99] == CLEAR)
    {   // ���� �߻��� ���� ���
        if (mu8TDSCheckStatus == TRUE)
        {   // TDS ���� ���� ���
            if (mu8TDSCheckEnd == CLEAR)
            {   // �ѹ� üũ�� �Ϸ���� ���� ���
                if (mu8ErrorCheckDelay >= DECONTAMINATION_ERROR_DETECT_DELAY_TIME)
                {   // üũ �� �����ð� ��� ��
                    mu8ErrorCheckDelay = 0;

                    mu8TDSCheckEnd = SET;

                    // ������ 70%�̻�  =>  (TDS IN * 30% >= TDS OUT)  =>  ex) TDS IN�� 100�̸� OUT�� 30���Ͽ��� OK
                    mu16TDSInValueBuf = (mu16TDSInValue / 10) * 3;   // TDS IN�� 30%�� ���

                    if (mu16TDSOutValue > DECONTAMINATION_ERROR_CHECK_STANDARD_OUT_PPM)
                    {   // TDS OUT�� 15ppm �ʰ��� ��� ���� üũ��
                        if (mu16TDSInValueBuf >= mu16TDSOutValue)
                        {   // �������� 30%���ϰ� �� ��(����)
                            mu8ErrorCheckDelay = 0;
                            mu8ErrorDetectCount = 0;
                        }
                        else
                        {   //  �������� 30%�ʰ��� �� ��(�̻�)
                            if (mu8ErrorDetectCount >= DECONTAMINATION_ERROR_DETECT_COUNT)
                            {
                                mu8ErrorDetectCount = 0;

                                Set_ErrorStatus(ERROR_ID_DECONTAMINATION_E99, SET);
                            }
                            else
                            {
                                mu8ErrorDetectCount++;
                            }
                        }
                    }
                    else
                    {   // TDS OUT�� 15ppm ������ ��� ���� üũ ����
                        mu8ErrorCheckDelay = 0;
                        mu8ErrorDetectCount = 0;
                    }
                }
                else
                {
                    mu8ErrorCheckDelay++;
                }
            }
        }
        else
        {   // TDS ���� ���� �ƴ� ���
            mu8TDSCheckEnd = 0;
            mu8ErrorCheckDelay = 0;
        }
    }
    else
    {   // ���� �߻��� ���
        mu8TDSCheckEnd = CLEAR;
        mu8ErrorCheckDelay = 0;
        mu8ErrorDetectCount = 0;
    }
#endif
}


/// @brief      ���÷��� �Ǿ�� �ϴ� ���� �ڵ� Ȯ�� �Լ�
/// @details    �켱 ������ ���� ���� ǥ�õǾ�� �ϴ� ������ �ش��ϴ� ���÷��� ǥ�� Code�� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ǥ�� �ڵ� - �ڵ� ����Ʈ ���� ����
U8 Get_ErrorDisplayCode(void)
{
    U8 mu8i = 0;
    U8 mu8ReturnData = 0;

    for (mu8i = 0 ; mu8i < ERROR_ID_MAX ; mu8i++)
    {
        if (SError.gu8Status[mu8i] == SET)
        {   // �켱������ ���� ǥ���� ������ �ִ� ���
            mu8ReturnData  = gu8ErrorDisplayCode[mu8i];
            break;  // Ȯ�� ����
        }
    }

    return  mu8ReturnData;
}


/// @brief      ������ �ϳ��� �߻��� ���� �ִ��� üũ�ϴ� �Լ�
/// @details    üũ�ϴ� ������ �� � ������ �߻��� ���� �ִ��� ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �߻� ���� - 0(�̹߻�), 1(�߻�)
U8 Get_ErrorOccuredCheck(void)
{
    U8 mu8i = 0;
    U8 mu8ReturnData = 0;

    for (mu8i = 0 ; mu8i < ERROR_ID_MAX ; mu8i++)
    {
        if (SError.gu8Status[mu8i] == SET)
        {   // �߻��� ������ �ִ� ���
            mu8ReturnData  = TRUE;
            break;  // Ȯ�� ����
        }
    }

    if (mu8ReturnData)
    {
        return  TRUE;
    }
    else
    {
        return FALSE;
    }
}



/// @brief      ���� �߻� ���� ���� �Լ�
/// @details    ID�� �ش��ϴ� ������ �����Ѵ�
/// @param      mu8ID : ���� ID
///             mu8Status : ���� �߻� ���� - 0(CLEAR, �̹߻�), 1(SET, �߻�)
/// @return     void
void Set_ErrorStatus(U8 mu8ID, U8 mu8Status)
{
    if (mu8Status)
    {
        SError.gu8Detect[mu8ID] = SET;
        SError.gu8Status[mu8ID] = SET;
    }
    else
    {
        SError.gu8Detect[mu8ID] = CLEAR;
        SError.gu8Status[mu8ID] = CLEAR;
    }
}


/// @brief      ���� �߻� ���� Ȯ�� �Լ�
/// @details    ID�� �ش��ϴ� ������ �߻��Ͽ����� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      mu8ID : ���� ID
/// @return     return : ���� �߻� ���� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
U8 Get_ErrorStatus(U8 mu8ID)
{
    if (SError.gu8Status[mu8ID] == SET)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ���� ���� ���� ���� �Լ�
/// @details    ID�� �ش��ϴ� ���� ������ �����Ѵ�
/// @param      mu8ID : ���� ID
///             mu8Status : ���� ���� ���� - 0(CLEAR, �̹߻�), 1(SET, �߻�)
/// @return     void
void Set_ErrorDetect(U8 mu8ID, U8 mu8Status)
{
    if (mu8Status)
    {
        SError.gu8Detect[mu8ID] = SET;
    }
    else
    {
        SError.gu8Detect[mu8ID] = CLEAR;
    }
}


/// @brief      ���� ���� ���� Ȯ�� �Լ�
/// @details    ID�� �ش��ϴ� ������ ����(�ݵ�� ���� �߻��� �ƴ�)�Ǿ����� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      mu8ID : ���� ID
/// @return     return : ���� ���� ���� - 0(FALSE, �̰���), 1(TRUE, ����)
U8 Get_ErrorDetect(U8 mu8ID)
{
    if (SError.gu8Detect[mu8ID] == SET)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      �Լ� ���� ���� ���� �߻� ���� Ȯ�� �Լ�
/// @details    �Լ� ���۰� ���õ� ������ �߻��� ���� �ִ��� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �߻� ���� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
U8 Get_ErrorAboutWaterInput(void)
{
    if ( (SError.gu8Status[ERROR_ID_LEAKAGE_E01] == SET) ||
         (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET) ||
         (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == SET) ||
         (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == SET) ||
         (SError.gu8Status[ERROR_ID_MIDDLE_SENSOR_E12] == SET) ||
         (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == SET) ||
         (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET) ||
         (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == SET) ||
         (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET) ||
         (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET) ||
         (SError.gu8Status[ERROR_ID_FEED_VALVE_E10] == SET) )
    {
        return  TRUE;
    }

    return  FALSE;
}


/// @brief      �ܱ� �µ� ���� ���� �߻� ���� Ȯ�� �Լ�
/// @details    �ܱ� �µ� ���� ������ �߻��� ���� �ִ��� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �߻� ���� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
U8 Get_AirTempError(void)
{
    if ( (SError.gu8Status[ERROR_ID_AIR_TEMP_1_E43] == SET) ||
         (SError.gu8Status[ERROR_ID_AIR_TEMP_2_E53] == SET) )
    {   // �ܱ�1 �Ǵ� �ܱ�2 �µ� ���� ������ �߻��� ���
        return  TRUE;
    }

    return  FALSE;
}


/// @brief      �¼� ���� ���� �߻� ���� Ȯ�� �Լ�
/// @details    �¼� ���� ������ �߻��� ���� �ִ��� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �߻� ���� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
U8 Get_HotError(void)
{
    if ( (SError.gu8Status[ERROR_ID_LEAKAGE_E01] == SET) ||
         (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == SET) ||
         (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == SET) ||
         (SError.gu8Status[ERROR_ID_MIDDLE_SENSOR_E12] == SET) ||
         (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == SET) ||
         (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == SET) ||
         (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET) ||
         (SError.gu8Status[ERROR_ID_FEED_VALVE_E10] == SET) ||
         (SError.gu8Status[ERROR_ID_TANK_HOT_TEMP_E45] == SET) ||
         (SError.gu8Status[ERROR_ID_HOT_OVERHEAT_E40] == SET) )
    {
        return  TRUE;
    }

    return  FALSE;
}


/// @brief      �ü� ���� ���� �߻� ���� Ȯ�� �Լ�
/// @details    �ü� ���� ������ �߻��� ���� �ִ��� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �߻� ���� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
U8 Get_ColdError(void)
{
    if ( (SError.gu8Status[ERROR_ID_LEAKAGE_E01] == SET) ||
         (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET) ||
         (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == SET) ||
         (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == SET) ||
         (SError.gu8Status[ERROR_ID_MIDDLE_SENSOR_E12] == SET) ||
         (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == SET) ||
         (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == SET) ||
         (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET) ||
         (SError.gu8Status[ERROR_ID_FEED_VALVE_E10] == SET) ||
         (SError.gu8Status[ERROR_ID_COLD_TEMP_1_E44] == SET) ||
         (SError.gu8Status[ERROR_ID_COLD_TEMP_2_E54] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_COMMUNICATION_E27] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_1_E81] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_2_E82] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_3_E83] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_4_E84] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_5_E85] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_6_E86] == SET) )
    {
        return  TRUE;
    }

    return  FALSE;
}


/// @brief      ���� ���� ���� �߻� ���� Ȯ�� �Լ�
/// @details    ���� ���� ���� ������ �߻��� ���� �ִ��� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� �߻� ���� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
U8 Get_ErrorAboutIceMaking(void)
{
#if (BLDC_COMP_USE == USE)
    U8 mu8BLDC_Comp_CommErrorDetect = 0;
    U8 mu8BLDC_Comp_ErrorCodeDetect = 0;

    mu8BLDC_Comp_CommErrorDetect = Get_BLDC_Comp_Comm_ErrorDetect();
    mu8BLDC_Comp_ErrorCodeDetect = Get_BLDC_Comp_ErrorDetect();
#endif

    if ( (SError.gu8Status[ERROR_ID_ROOM_TEMP_1_E42] == SET) ||
         (SError.gu8Status[ERROR_ID_MICRO_SW_ALL_DETECT_E61] == SET) ||
         (SError.gu8Status[ERROR_ID_MICRO_SW_ICE_MAKING_E62] == SET) ||
         (SError.gu8Status[ERROR_ID_MICRO_SW_DE_ICE_E63] == SET) ||
         (SError.gu8Status[ERROR_ID_DRAIN_PUMP_E60] == SET) ||
         (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET) ||
         ((SError.gu8Status[ERROR_ID_AIR_TEMP_1_E43] == SET) && (SError.gu8Status[ERROR_ID_AIR_TEMP_2_E53] == SET)) ||
         (SError.gu8Status[ERROR_ID_AIR_TEMP_2_E53] == SET) ||
         (SError.gu8Status[ERROR_ID_WATER_CUT_OFF_E09] == SET) ||

         (SError.gu8Status[ERROR_ID_BLDC_COMP_COMMUNICATION_E27] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_1_E81] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_2_E82] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_3_E83] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_4_E84] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_5_E85] == SET) ||
         (SError.gu8Status[ERROR_ID_BLDC_COMP_ACT_6_E86] == SET) ||
#if (BLDC_COMP_USE == USE)
         (mu8BLDC_Comp_CommErrorDetect == TRUE) ||
         (mu8BLDC_Comp_ErrorCodeDetect == TRUE) ||
#endif
         (SError.gu8Status[ERROR_ID_LEAKAGE_E01] == SET) )
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


// Lib Water Error Module *********************************************************************************

/// @brief      Lib Water Error Module Initilaize
/// @details    ������ ���� üũ ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_WaterError_Module_Initialize(void)
{
    Lib_WaterErrorCheck_Initialize();
}


/// @brief      Lib Water Error Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� ������ ���� üũ ���� ��� Library
/// @param      void
/// @return     void
void Lib_WaterError_Module_1ms_Control(void)
{
    WaterErrorCheckTimer();
}


/// @brief      Lib Water Error Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� ������ ���� üũ ���� ��� Library
/// @param      void
/// @return     void
void Lib_WaterError_Module_Control(void)
{
    Lib_WaterErrorCheck();
}


