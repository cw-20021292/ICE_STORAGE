/// @file     Lib_Water_ErrorCheck.c
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    정수기 Error 관련 제어 file

#include "Global_Header.h"

U8 gu8ErrorCheckTimer = 0;                      /// @brief  에러 체크를 하는 주기 타이머

#if ( (FLOW_SENSOR_ERROR_CHECK_USE == USE) || (WATER_CUT_OFF_ERROR_CHECK_USE == USE) )
U8 gu8FlowMeterErrorCheckStart = 0;             /// @brief  유량센서 관련 에러 체크 시작 상태 - 0(체크 미시작), 1(체크 시작)
U16 gu16FlowMeterErrorChedkDelayTime = 0;       /// @brief  유량센서 관련 에러 체크 전 지연 시간@100ms
#endif

#if (EFFLUENT_CLOGGING_ERROR_CHECK_USE == USE)
U8 gu8EffluentCloggingErrorCheckStart = 0;      /// @brief  추출 막힘 에러 체크 시작 상태 - 0(체크 미시작), 1(체크 시작)
#endif

#if (HOT_CUT_OFF_ERROR_CHECK_USE == USE)
U8 gu8FHotCutOffCheckStart = 0;                 /// @brief  온수 단수 및 배수 막힘 에러 체크 시작 상태 - 0(체크 미시작), 1(체크 시작)
#endif

#if (BLDC_COMP_COMMUNICATION_ERROR_CHECK_USE == USE)
U8 gu8BLDC_CompCommunicationErrorDetect = 0;    /// @brief  BLDC Comp와 통신시 통신 이상 감지 여부 - 0(미감지), 1(감지)
U8 gu8BLDC_CompErrorCodeRecieve = 0;            /// @brief  BLDC Comp와 통신시 동작 에러 코드 수신 여부 - 0(미수신), 1(수신)
U8 gu8BLDC_ComoErrorCode = 0;                   /// @brief  BLDC Comp와 통신시 수신한 에러 코드
#endif

#if (MICRO_SW_ALL_DETECT_ERROR_CHECK_USE == USE)
U16 gu16DualCheckDelay = 0;                     /// @brief  마이크로 SW 동시 감지 에러 확정 제어 로직에서 동작간 Delay시간
U8 gu8DualTrayStep = 0;                         /// @brief  마이크로 SW 동시 감지 에러 확정 판단 제어 로직 단계
U8 gu8DualTrayCheckCount = 0;                   /// @brief  마이크로 SW 동시 감지 에러 확정 판단을 위한 감지 횟수 카운트

U8 gu8Err_tray_down_acc_count = 0;              /// @brief  트레이 아래쪽 이동 누적 에러 카운트
U8 gu8Tray_up_moving_retry_timer = 0;           /// @brief  트레이 위쪽 이동 재시도 시간
U8 gu8Err_Tray_Up_Count = 0;                    /// @brief  트레이 제빙 방향 이동 재시도 횟수
U8 gu8Err_Tray_Down_Count = 0;                  /// @brief  트레이 탈빙 방향 이동 재시도 횟수
U16 gu16Tray_down_moving_retry_timer = 0;       /// @brief  트레이 아래쪽 이동 재시도 시간
U16 gu16Err_Tray_Motor_Down_Total_Tmr = 0;
U16 gu16Err_Tray_Motor_Up_Total_Tmr = 0;
U8 gu8Tray_Motor_Direction = 0;                 /// @brief  트레이 모터 이동 방향 - 0(Stop), 1(CW), 2(CCW)
U8 gu8Tray_up_contiunue_timer = 0;
#endif

/// @brief      에러 감지 관련 데이터
/// @details    에러 감지와 관련된 제어를 하기 위한 관련 데이터들을 구조체로 선언한다
typedef struct
{
    U8 gu8CheckCount[ERROR_ID_MAX];             /// @brief  에러 감지 횟수
    U8 gu8Detect[ERROR_ID_MAX];                 /// @brief  에러 감지 상태 - 0(미감지), 1(감지)
    U8 gu8Status[ERROR_ID_MAX];                 /// @brief  에러 발생 확정 상태 - 0(미발생), 1(발생)
}   SErrorData_T;

SErrorData_T    SError;


/// @brief      정수기 에러 발생 상태 데이터 초기화
/// @details    정수기 에러 발생 상태 데이터들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_WaterErrorCheck_Initialize(void)
{
    gu8ErrorCheckTimer = 0;

#if ( (FLOW_SENSOR_ERROR_CHECK_USE == USE) || (WATER_CUT_OFF_ERROR_CHECK_USE == USE) )
    gu8FlowMeterErrorCheckStart = 0;
    gu16FlowMeterErrorChedkDelayTime = 0;        /// @brief  유량센서 관련 에러 체크 전 지연 시간@100ms
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


/// @brief      정수기 에러 체크 동작 타이머(@1ms)
/// @details    정수기 에러 체크 동작을 할 주기를 타이머로 카운트 한다
/// @param      void
/// @return     void
void WaterErrorCheckTimer(void)
{
    if (gu8ErrorCheckTimer < ERROR_CHECK_PERIOD_TIME)
    {
        gu8ErrorCheckTimer++;
    }
}


/// @brief      정수기 에러 체크 동작 함수(@while)
/// @details    정수기 에러 체크 동작을 주기적으로 진행한다
/// @param      void
/// @return     void
void Lib_WaterErrorCheck(void)
{
    U8 mu8BootComplete = 0;

    if (gu8ErrorCheckTimer >= ERROR_CHECK_PERIOD_TIME)
    {   // 100ms 마다 확인
        gu8ErrorCheckTimer = 0;

        mu8BootComplete = Get_BootComplete();   // 부팅 동작이 완료되었는지 확인

        if (mu8BootComplete == SET)
        {   // 부팅 동작이 완료되었으면
            LeakageErrorCheck();                    // 누수 에러 체크
            FlowMeterErrorCheck_BuiltInUse();       // 유량센서1, 2 에러 체크
            WaterCutOffErrorCheck();                // 단수 에러 체크
            RoomOverHeatErrorCheck();               // 정수 과열 에러 체크
            FilterCloggingErrorCheck();             // 필터 막힘 에러 체크
            EffluentCloggingErrorCheck();           // 추출 막힘(유량센서3) 에러 체크
            HotCutOffErrorCheck();                  // 온수 단수 및 배수 막힘(순간온수) 에러 체크
            BoostPumpErrorCheck();                  // 부스트 펌프 동작 에러 체크
            PressureSensorErrorCheck();             // 압력 센서 에러 체크
            TrayMotorErrorCheck();                  // 추출 트레이 모터 에러 체크
            MicroSW_AllDetectErrorCheck();          // 마이크로 스위치 동시 감지 에러 체크
            MicroSW_IceMakingErrorCheck();          // 마이크로 스위치 이동 에러 체크
            DrainPumpErrorCheck();                  // 드레인 펌프 에러 체크
            SodaPumpErrorCheck();                   // 탄산 펌프 에러 체크
            AbnormalEffluentErrorCheck();           // 추출 동작 이상 에러 체크
            RoomLevelErrorCheck();                  // 정수 탱크 수위 센서들의 에러 체크
            HotHighLevelErrorCheck();               // 온수 만수위 센서 에러 체크
            IceColdWaterTankLevelErrorCheck();      // 빙축탱크 수위 센서 에러 체크
            ColdLevelErrorCheck();                  // 냉수 탱크 수위 센서들의 에러 체크
            RoomTempErrorCheck();                   // 정수 온도 센서 에러 체크
            HotTempErrorCheck();                    // 온수 온도 센서 에러 체크
            ColdTempErrorCheck();                   // 냉수 온도 센서 에러 체크
            IceTrayInputTempErrorCheck();           // 아이스 트레이 입수 온도 센서 에러 체크
            AirTempErrorCheck();                    // 외기 온도 센서 에러 체크
            HeatsinkTempErrorCheck();               // TEM 방열판 온도 센서 에러 체크
            HotOverheatErrorCheck();                // 온수 과열 에러 체크
            HotPumpErrorCheck();                    // 온수 펌프 에러 체크
            BLDC_CompErrorCheck();                  // BLDC Comp 에러 체크
            CompActErrorCheck();                    // 일반 Comp 동작 에러 체크
            CirculatePumpErrorCheck();              // 순환 펌프 에러 체크
            StirringMoterErrorCheck();              // 교반 모터 에러 체크
            EW_ModuleErrorCheck();                  // EW 살균 모듈 에러 체크
            EW_DrainPumpErrorCheck();               // EW 살균 드레인 에러 체크
            UVErrorCheck();                         // UV 에러 체크
            TDS_In_TempErrorCheck();                // TDS IN 온도 센서 에러 체크
            TDS_Out_TempErrorCheck();               // TDS OUT 온도 센서 에러 체크
            TDS_InErrorCheck();                     // TDS IN 센서 에러 체크
            TDS_OutErrorCheck();                    // TDS OUT 센서 에러 체크
            InputContaminationErrorCheck();         // 원수 오염 에러 체크
            DecontaminationErrorCheck();            // 제거율 불량 에러 체크
        }
    }
}


/// @brief      누수 에러 체크
/// @details    누수 에러 체크 동작을 진행한다
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
        {   // 누수 에러가 발생하지 않은 상태이면
            if (mu16LeakageErrorDetectTimer >= LEAKAGE_ERROR_DETECT_TIME)
            {   // 누수 감지가 3초 이상 감지되면
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
        {   // 누수 에러가 발생한 상태이면
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
/// @brief      유량 센서 관련 에러 체크 시작 상태 설정 함수
/// @details    유량 센서 관련 에러 체크를 시작할 것인지에 대한 상태값을 설정한다
/// @param      mu8Status : 에러 체크 시작 상태값 - 0(미시작), 1(시작)
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


/// @brief      유량 센서 관련 에러 체크 시작 상태 확인 함수
/// @details    유량 센서 관련 에러 체크를 시작할 것인지에 대한 상태값을 확인하여 반환한다
/// @param      void
/// @return     return : 에러 체크 시작 상태값 - 0(미시작), 1(시작)
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


/// @brief      유량 센서 관련 에러 체크 시작 전 지연 시간 설정 함수
/// @details    유량 센서 관련 에러 체크 시작 전 지연 동작을 할 시간을 설정한다
/// @param      mu16Time : 지연 시간 값(100ms단위)
/// @return     void
void Set_FlowMeterErrorCheckDelayTime(U16 mu16Time)
{
    gu16FlowMeterErrorChedkDelayTime = mu16Time;
}


/// @brief      유량 센서 관련 에러 체크 시작 전 지연 시간 확인 함수
/// @details    유량 센서 관련 에러 체크 시작 전 지연 동작을 할 시간을 확인하여 반환한다
/// @param      void
/// @return     return : 지연 시간 값(100ms단위)
U16 Get_FlowMeterErrorCheckDelayTime(void)
{
    return  gu16FlowMeterErrorChedkDelayTime;
}

#endif


/// @brief      유량센서1, 2 에러 체크
/// @details    빌트인 모델에서 유량센서를 2개 사용하는 경우 유량센서1, 2의 에러 체크 동작을 진행한다
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

    mu8ErrorCheckStart = Get_FlowMeterErrorCheckStart();        // 유량센서 에러 체크 시작 상태 확인
    mu8Flow1_Hz = Get_Flowmeter_Hz(FLOWMETER_1_ID);
//    mu8Flow2_Hz = Get_Flowmeter_Hz(FLOWMETER_2_ID);

    if (mu8ErrorCheckStart == TRUE)
    {   // 에러 체크 동작 중이면
        if (mu16ErrorCheckDelay >= FLOW_SENSOR_ERROR_CHECK_DELAY_TIME)
        {
            if (SError.gu8Status[ERROR_ID_FLOW_SENSOR_1_E05] == CLEAR)
            {   // 유량센서1 에러 미발생 상태
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
            {   // 유량센서1 에러가 발생한 상태
                mu16ErrorDetectTime1 = 0;
            }

            if (SError.gu8Status[ERROR_ID_FLOW_SENSOR_2_E06] == CLEAR)
            {   // 유량센서2 에러 미발생 상태
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
            {   // 유량센서2 에러가 발생한 상태
                mu16ErrorDetectTime2 = 0;
            }
        }
        else
        {
            mu16ErrorCheckDelay++;
        }
    }
    else
    {   // 에러 체크 동작 중이 아니면
        mu16ErrorCheckDelay = 0;
        mu16ErrorDetectTime1 = 0;
        mu16ErrorDetectTime2 = 0;
    }
#endif
}


/// @brief      단수 에러 체크
/// @details    단수 에러 체크 동작을 진행한다
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

    mu8ErrorCheckStart = Get_FlowMeterErrorCheckStart();            // 유량센서 에러 체크 시작 상태 확인
    mu8Flow_Hz = Get_Flowmeter_Hz(FLOWMETER_1_ID);
    mu16ErrorCheckDelayTime = Get_FlowMeterErrorCheckDelayTime();   // 에러 체크 시작 전 지연 시간 확인

    if (mu8ErrorCheckStart == TRUE)
    {   // 에러 체크 동작 중이면
        if (mu16ErrorCheckDelay >= mu16ErrorCheckDelayTime)
        {   // 에러 체크 전 지연 시간 이후
            if (SError.gu8Status[ERROR_ID_WATER_CUT_OFF_E09] == CLEAR)
            {   // 단수 에러 미발생 상태
                mu16ErrorAutoClearTime = 0;     // 자동 해제 타이머 CLEAR

                if (mu8Flow_Hz < WATER_CUT_OFF_HZ)
                {   // 기준 유속 미만이면
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
                {   // 기준 유속 이상이면
                    mu16ErrorDetectTime = 0;
                }
            }
            else
            {   // 단수 에러 발생 상태
                mu16ErrorDetectTime = 0;
            }
        }
        else
        {   // 에러 체크 전 지연 시간 이전
            mu16ErrorCheckDelay++;
        }
    }
    else
    {   // 에러 체크 동작 중이 아니면
        mu16ErrorCheckDelay = 0;
        mu16ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_WATER_CUT_OFF_E09] == SET)
        {   // 단수 에러가 발생된 상태
            if (mu16ErrorAutoClearTime >= WTAER_CUT_OFF_AUTO_CLEAR_TIME)
            {   // 일정 시간 후 자동 해제
                mu16ErrorAutoClearTime = 0;

                Set_ErrorStatus(ERROR_ID_WATER_CUT_OFF_E09, CLEAR);
            }
            else
            {
                mu16ErrorAutoClearTime++;
            }
        }
        else
        {   // 단수 에러가 발생되지 않은 상태
            mu16ErrorAutoClearTime = 0;
        }
    }
#endif
}


/// @brief      정수 과열 에러 체크
/// @details    정수 과열 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void RoomOverHeatErrorCheck(void)
{
#if (ROOM_WATER_OVERHEAT_ERROR_CHECK_USE == USE)
    F32 mf32RoomTemp = 0;
    static U16 mu16RoomOverheatDetectTime = 0;

    mf32RoomTemp = Get_Temp(ROOM_THERMISTOR_ID);

    if (mf32RoomTemp >= 50.0f)
    {   // 정수 온도가 50도 이상이면
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
    {   // 정수 온도가 50도 미만이면
        Set_ErrorStatus(ERROR_ID_ROOM_OVERHEAT_E49, CLEAR);
        mu16RoomOverheatDetectTime = 0;
    }
#endif
}


/// @brief      필터 막힘 에러 체크(에러 검출 방안 표준 동작 제정된 내용 없음 - 미사용)
/// @details    필터 막힘 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void FilterCloggingErrorCheck(void)
{
#if (FILTER_CLOGGING_ERROR_CHECK_USE == USE)

#endif
}


#if (EFFLUENT_CLOGGING_ERROR_CHECK_USE == USE)

/// @brief      추출 막힘 에러 체크 시작 상태 설정 함수
/// @details    추출 막힘 에러 체크를 시작할 것인지에 대한 상태값을 설정한다
/// @param      mu8Status : 에러 체크 시작 상태값 - 0(미시작), 1(시작)
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


/// @brief      추출 막힘 에러 체크 시작 상태 확인 함수
/// @details    추출 막힘 에러 체크를 시작할 것인지에 대한 상태값을 확인하여 반환한다
/// @param      void
/// @return     return : 에러 체크 시작 상태값 - 0(미시작), 1(시작)
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


/// @brief      추출 막힘(유량센서3 포함) 에러 체크
/// @details    추출 막힘(유량센서3 포함) 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void EffluentCloggingErrorCheck(void)
{
#if (EFFLUENT_CLOGGING_ERROR_CHECK_USE == USE)

#if (EFFLUENT_CLOGGING_ERROR_FLOWMETER_USE == USE)

    // 유량센서를 사용하는 경우
    static U16 mu16ErrorCheckDelay = 0;
    static U16 mu16ErrorDetectTime = 0;
    U8 mu8ErrorCheckStart = 0;
    U8 mu8Flow_Hz = 0;

    mu8ErrorCheckStart = Get_EffluentCloggingErrorCheckStart();     // 추출 막힘(유량센서3) 에러 체크 시작 상태 확인
    mu8Flow_Hz = Get_Flowmeter_Hz(FLOWMETER_3_ID);

    if (mu8ErrorCheckStart == TRUE)
    {   // 에러 체크 동작 중이면
        if (mu16ErrorCheckDelay >= EFFLUENT_CLOGGING_CHECK_DELAY_TIME)
        {   // 에러 체크 시작 전 지연 이후
            if (SError.gu8Status[ERROR_ID_EFFLUENT_CLOGGING_E04] == CLEAR)
            {   // 에러 미발생 상태
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
        {   // 에러 체크 시작 전 지연 이전
            mu16ErrorCheckDelay++;
        }
    }
    else
    {   // 에러 체크 동작 중이 아니면
        mu16ErrorCheckDelay = 0;
        mu16ErrorDetectTime = 0;
    }

#else

    // 유량센서를 사용하지 않는 경우 - 온수와 냉수를 혼합하여 정수를 추출하는 구조의 제품에서 에러 체크 사용
    static U16 mu16ErrorDetectTime = 0;                 // 에러 감지 체크 시간
    static U16 mu16ErrorAutoClearTime = 0;              // 에러 자동 해제 체크 시간
    U8 mu8ErrorCheckStart = 0;
    static F32 mf32EffluentStartTemp = 0;               // 추출 시작시 추출부 온도
    static F32 mf32EffluentStartColdTemp = 0;           // 추출 시작시 냉수 온도
    F32 mf32Temp = 0;

    mu8ErrorCheckStart = Get_EffluentCloggingErrorCheckStart();     // 추출 막힘 에러 체크 시작 상태 확인
    mf32Temp = Get_Temp(FAUCET_OUT_THERMISTOR_ID);                  // 현재 추출부 온도

    if (mu8ErrorCheckStart == TRUE)
    {   // 에러 체크 동작 중이면
        if (mf32EffluentStartTemp == 0)
        {   // 추출 시작시 추출부 온도가 0이면((= 체크하기 전이면)
            mf32EffluentStartTemp = Get_Temp(FAUCET_OUT_THERMISTOR_ID);
        }

        if (mf32EffluentStartColdTemp == 0)
        {   // 추출 시작시 체크한 냉수 온도가 0이면(= 체크하기 전이면)
            mf32EffluentStartColdTemp = Get_Temp(COOL_THERMISTOR_1_ID);
        }

        if (SError.gu8Status[ERROR_ID_EFFLUENT_CLOGGING_E04] == CLEAR)
        {   // 에러 미발생 상태
            mu16ErrorAutoClearTime = 0;     // 자동 해제 타이머 CLEAR

            if (mf32Temp > 55.0f)
            {
                if (mf32EffluentStartTemp >= EFFLEUNT_CLOGGING_START_STANDARD_TEMP)
                {   // 추출 시작시 온도가 70도 이상인 경우
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
                {   // 추출 시작시 온도가 70도 미만인 경우
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
        {   // 에러 발생 상태
            if ( (mf32EffluentStartColdTemp <= EFFLUENT_CLOGGING_AUTO_CLEAR_COLD_TEMP) &&
                 (mf32EffluentStartTemp >= EFFLUENT_CLOGGING_AUTO_CLEAR_TEMP) )
            {   // 추출 시작시 냉수 온도가 10도 이하, 추출부 온도가 15도 이상인 상태
                if ( mf32Temp < (mf32EffluentStartTemp - 5.0f) )
                {   // 추출 시작시 추출부 온도보다 5도 drop된 온도로 나오는 경우
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
    {   // 에러 체크 동작 중이 아니면
        mu16ErrorDetectTime = 0;
        mu16ErrorAutoClearTime = 0;
        mf32EffluentStartTemp = 0;               // 추출 시작시 추출부 온도
        mf32EffluentStartColdTemp = 0;           // 추출 시작시 냉수 온도
        mf32Temp = 0;
    }
#endif

#endif
}


#if (HOT_CUT_OFF_ERROR_CHECK_USE == USE)

/// @brief      온수 단수 및 배수 막힘 에러 체크 시작 상태 설정 함수
/// @details    온수 단수 및 배수 막힘 에러 체크를 시작할 것인지에 대한 상태값을 설정한다
/// @param      mu8Status : 에러 체크 시작 상태값 - 0(미시작), 1(시작)
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


/// @brief      온수 단수 및 배수 막힘 에러 체크 시작 상태 확인 함수
/// @details    온수 단수 및 배수 막힘 에러 체크를 시작할 것인지에 대한 상태값을 확인하여 반환한다
/// @param      void
/// @return     return : 에러 체크 시작 상태값 - 0(미시작), 1(시작)
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


/// @brief      온수 단수 및 배수 막힘(순간온수) 에러 체크
/// @details    온수 단수 및 배수 막힘(순간온수) 에러 체크 동작을 진행한다
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

    mu8ErrorCheckStart = Get_HotCutOffCheckStart();        // 유량센서 에러 체크 시작 상태 확인
    mu8Flow_Hz = Get_Flowmeter_Hz(FLOWMETER_1_ID);

    if (mu8ErrorCheckStart == TRUE)
    {   // 에러 체크 동작 중이면
        if (mu16ErrorCheckDelay >= HOT_CUT_OFF_ERROR_CHECK_DELAY_TIME)
        {   // 에러 체크 동작 전 지연 시간 이후
            if (SError.gu8Status[ERROR_ID_HOT_CUT_OFF_E08] == CLEAR)
            {   // 에러 미발생 상태
                mu16ErrorAutoClearTime = 0;     // 자동 해제 타이머 CLEAR

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
            {   // 에러 발생 상태
                mu16ErrorDetectTime = 0;
            }
        }
        else
        {   // 에러 체크 동작 전 지연 시간 이전
            mu16ErrorCheckDelay++;
        }
    }
    else
    {   // 에러 체크 동작 중이 아니면
        mu16ErrorCheckDelay = 0;
        mu16ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_HOT_CUT_OFF_E08] == SET)
        {   // 에러가 발생된 상태
            if (mu16ErrorAutoClearTime >= HOT_CUT_OFF_AUTO_CLEAR_TIME)
            {   // 일정 시간 후 자동 해제
                mu16ErrorAutoClearTime = 0;

                Set_ErrorStatus(ERROR_ID_HOT_CUT_OFF_E08, CLEAR);
            }
            else
            {
                mu16ErrorAutoClearTime++;
            }
        }
        else
        {   // 단수 에러가 발생되지 않은 상태
            mu16ErrorAutoClearTime = 0;
        }
    }
#endif
}


/// @brief      부스트 펌프 동작(수배관 막힘) 에러 체크
/// @details    부스트 펌프 동작(수배관 막힘) 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void BoostPumpErrorCheck(void)
{
#if (BOOST_PUMP_ERROR_CHECK_USE == USE)
    static U8 mu8ErrorDetectTime = 0;
    U8 mu8BoostPumpStatus = 0;
    U16 mu16BoostPumpFeedbackAD = 0;

    mu8BoostPumpStatus = Get_BoostPump_Status();                    // 부스트 펌프의 실제 동작 상태 확인
//    mu16BoostPumpFeedbackAD = Get_ADC_Data(ADC_ID_BOOST_PUMP);      // 부스트 펌프의 피드백 AD값 확인

    if (mu8BoostPumpStatus == TRUE)
    {   // 부스트 펌프가 동작 중인 경우
        if ( (mu16BoostPumpFeedbackAD < BOOST_PUMP_ERROR_UNDER_STANDARD_AD) ||
             (mu16BoostPumpFeedbackAD > BOOST_PUMP_ERROR_OVER_STANDARD_AD) )
        {   // 기준값 미만 또는 초과인 경우
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
    {   // 부스트 펌프가 동작 중이 아닌 경우
        mu8ErrorDetectTime = 0;
    }
#endif
}


/// @brief      압력 센서 에러 체크
/// @details    압력 센서 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void PressureSensorErrorCheck(void)
{
#if (PRESSURE_SENSOR_ERROR_CHECK_USE == USE)

#endif
}


/// @brief      추출 트레이 모터 에러 체크(에러 검출 방안 표준 동작 제정된 내용 없음 - 미사용)
/// @details    추출 트레이 모터 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void TrayMotorErrorCheck(void)
{
#if (TRAY_MOTOR_ERROR_CHECK_USE == USE)

#endif
}


/// @brief      마이크로 스위치 동시 감지 에러 체크
/// @details    마이크로 스위치 동시 감지 에러 체크 동작을 진행한다
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
            Set_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61, SET);     // 동시 감지면 Tray M/S Error 상황 발생 감지 설정
        }
    }

    if (SError.gu8Detect[ERROR_ID_MICRO_SW_ALL_DETECT_E61] == SET)
    {   // 에러 상태가 감지된 경우
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
        {   // 에러 확정 동작 단계별 제어
            case 0:
                run_up_ice_tray();

                gu16DualCheckDelay = 100;       // 제빙 10초 이동
                gu8DualTrayStep++;              // 1.CW
                break;

            case 1:
                if (gu16DualCheckDelay == 0)
                {
                    run_stop_ice_tray();
                    gu16DualCheckDelay = 20;    // 2초 지연
                    gu8DualTrayStep++;          // 2.제빙위치에서 정지
                }
                else if ( (gu16DualCheckDelay <= 90) && (gu16DualCheckDelay >= 70) )
                {
                    if (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_MOVING)
                    {   // 이동중 Dual Open 해제
                        gu8DualTrayStep = 0;
                        gu16DualCheckDelay = 0;
                        gu8DualTrayCheckCount = 0;
                        Set_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61, CLEAR);   // Tray 인식 이동중이면 Tray M/S Error 초기발생 해제
                        Set_IceStep(STATE_0_STANDBY);       // 제빙 동작 첫 단계
                        Set_InitIceStep(0);                 // 제빙 초기화 동작 첫 단계
                        Set_InitIceStatus(SET);             // 제빙 초기화 동작 시작
                    }
                    else
                    {

                    }
                }
                break;

            case 2:
                if (gu16DualCheckDelay == 0)
                {
                    /*..hui [18-2-1오전 11:19:15] 트레이 탈빙 방향 이동..*/
                    run_down_ice_tray();
                    gu16DualCheckDelay = 250;   // 탈빙 25초 이동
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
                    gu16DualCheckDelay = 3000;      // 5분 지연
                    gu8DualTrayStep++;              // 4.탈빙위치에서 정지
                }
                else if ( (gu16DualCheckDelay <= 220) && (gu16DualCheckDelay >= 80) )
                {
                    if (mu8IceTrayPositionBuf == ICE_TRAY_POSITION_MOVING)
                    {   // 이동중 Dual Open 해제
                        gu8DualTrayStep = 0;
                        gu16DualCheckDelay = 0;
                        gu8DualTrayCheckCount = 0;
                        Set_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61, CLEAR);   // Tray 인식 이동중이면 Tray M/S Error 초기발생 해제
                        Set_IceStep(STATE_0_STANDBY);       // 제빙 동작 첫 단계
                        Set_InitIceStep(0);                 // 제빙 초기화 동작 첫 단계
                        Set_InitIceStatus(SET);             // 제빙 초기화 동작 시작
                    }
                }
                else
                {

                }
                break;

            case 4:
                if (gu16DualCheckDelay == 0)
                {
                    gu8DualTrayStep = 0;        // 에러 확정 동작 0단계부터 다시 동작

                    if (++gu8DualTrayCheckCount >= 3)
                    {
                        gu8DualTrayCheckCount = 0;
                        Set_ErrorStatus(ERROR_ID_MICRO_SW_ALL_DETECT_E61, SET);     // 3회 시도후 Error
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
/// @brief      마이크로 SW 동시 감지 에러 확정 판단 제어 로직 단계 설정 함수
/// @details    마이크로 SW 동시 감지 에러 확정 판단 제어 로직 동작 단계를 설정한다
/// @param      mu8Step : 마이크로 SW 동시 감지 에러 확정 판단 제어 로직 단계
/// @return     void
void Set_MicroSW_AllDetectErrorCheckStep(U8 mu8Step)
{
    gu8DualTrayStep = mu8Step;
}


/// @brief      마이크로 SW 동시 감지 에러 확정 판단 제어 로직 단계 확인 함수
/// @details    마이크로 SW 동시 감지 에러 확정 판단 제어 로직 동작 단계를 확인하여 반환한다
/// @param      void
/// @return     return : 마이크로 SW 동시 감지 에러 확정 판단 제어 로직 단계
U8 Get_MicroSW_AllDetectErrorCheckStep(void)
{
    return  gu8DualTrayStep;
}
#endif



/// @brief      마이크로 스위치 이동 에러 체크
/// @details    마이크로 스위치 이동 에러 체크 동작을 진행한다
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

    //------------------------------------------------- 얼음받이 회전
    /*..hui [18-1-26오후 8:22:52] F_TrayMotorUP 가 제빙 F_TrayMotorCCW가 탈빙위치.. ..*/
    /*..hui [18-1-26오후 8:23:08] F_TrayMotorUP 가 실제로는 CCW로 돈다..*/
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
            /*..hui [18-2-6오후 1:32:40] CCW로 돌고 5초가 아직 안지난 상태면 대기..*/
            mu8TrayCCW_DelayEnd = Get_TrayCCW_DelayEnd();

            if (mu8TrayCCW_DelayEnd == TRUE)
            {
                /*..hui [18-2-2오후 3:11:59] 제빙방향 이동중 에러 감지..*/
                /*..hui [18-2-6오후 7:02:16] CW가 SET되는 시점은 gu8IceStep이 1일때밖에 없음, 제빙 시작할때..*/
                check_error_tray_motor_up_operation();
            }
            else
            {
                run_stop_ice_tray();
            }

            break;

        case TRAY_MOTOR_CCW_DIRECTION:
            /*..hui [18-2-6오후 1:32:40] CCW로 돌고 5초가 아직 안지난 상태면 대기..*/
            /*..hui [18-2-6오후 8:18:30] CCW가 SET되는 시점은 더미탈빙/제빙탈빙시....*/
            mu8TrayCW_DelayEnd = Get_TrayCW_DelayEnd();       // F_Tray_CW_delay_finish

            if (mu8TrayCW_DelayEnd == TRUE)
            {
                /*..hui [18-2-2오후 3:12:05] 탈빙 방향 이동중 에러 감지..*/
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

    /*..hui [18-2-9오후 1:55:07] SW적으로 동시에 SET될 경우 대비용..*/
    if ( (mu8TrayMotorUpBuf == SET) && (mu8TrayMotorDownBuf == SET) )
    {
        Set_ErrorStatus(ERROR_ID_MICRO_SW_ICE_MAKING_E62, SET);
    }
    else
    {

    }

    /*..hui [19-12-13오후 7:25:08] 트레이 내린 후 정상적으로 탈빙 모드까지 진입 시 누적 카운트 초기화..*/
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
/// @brief      트레이 모터 UP 동작 에러 체크
/// @details    트레이 모터 UP 동작 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void check_error_tray_motor_up_operation(void)
{
    U8 mu8TrayPosition = 0;
    U8 mu8TrayModeBuf = 0;

    mu8TrayPosition = Get_IceTrayPosition();
    mu8TrayModeBuf = Get_IceTrayMode();

    /*..hui [18-2-6오후 5:25:47] 탈빙에서 제빙 방향으로 이동 및 에러감지..*/
    switch (mu8TrayPosition)
    {
        case ICE_TRAY_POSITION_ICE_MAKING:
            /*..hui [18-2-6오후 2:14:43] 제빙 방향으로 감지시 정지..*/
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
                        /*..hui [25-3-17오후 4:18:38] 첫 2회만 얼음걸림 역회전 제어 추가..*/
                        Set_IceStuckBackStatus(SET);
                    }
                    else
                    {

                    }
                }
            }

            break;

        case ICE_TRAY_POSITION_ERROR:
            /*..hui [18-2-6오후 4:49:15] 트레이 동시 감지시 즉시 정지..*/
            /*..hui [18-2-6오후 4:51:07] 이쪽으로 들어올일은 없음 윗부분 동시감지 에러 부분에서 처리..*/
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
                    /*..hui [25-3-17오후 4:18:38] 첫 2회만 얼음걸림 역회전 제어 추가..*/
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


/// @brief      트레이 제빙 방향 이동 재시도 횟수 설정 함수
/// @details    트레이 제빙 방향 이동 재시도 횟수를 설정한다
/// @param      mu8Data : 트레이 제빙 방향 이동 재시도 횟수
/// @return     void
void Set_TrayUp_ErrorCount(U8 mu8Data)
{
    gu8Err_Tray_Up_Count = mu8Data;
}


/// @brief      트레이 제빙 방향 이동 재시도 횟수 확인 함수
/// @details    트레이 제빙 방향 이동 재시도 횟수를 확인하여 반환한다
/// @param      void
/// @return     return : 트레이 제빙 방향 이동 재시도 횟수
U8 Get_TrayUp_ErrorCount(void)
{
    return  gu8Err_Tray_Up_Count;
}


/// @brief      트레이 모터 Down 동작 에러 체크
/// @details    트레이 모터 Down 동작 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void check_error_tray_motor_down_retry_operation(void)
{
    U8 mu8TrayPosition = 0;
    U8 mu8TrayModeBuf = 0;

    mu8TrayPosition = Get_IceTrayPosition();
    mu8TrayModeBuf = Get_IceTrayMode();

    /*..hui [18-2-6오후 8:26:55] 제빙에서 탈빙으로 이동 및 에러감지....*/
    switch (mu8TrayPosition)
    {
        case ICE_TRAY_POSITION_ICE_MAKING:
            /*..hui [18-2-6오후 8:27:13] 5초 이상 제빙 유지시 재 시도 기능 수행..*/
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
            /*..hui [18-2-6오후 2:14:43] 제빙 방향으로 감지시 정지..*/
            /*pMOTOR_ICE_TRAY_CCW = CLEAR;*/
            run_stop_ice_tray();
            Set_TrayMotorDownStatus(CLEAR);
            gu16Err_Tray_Motor_Down_Total_Tmr = 0;
            gu8Err_Tray_Down_Count = 0;
            gu16Tray_down_moving_retry_timer = 0;
            break;

        case ICE_TRAY_POSITION_ERROR:
            /*..hui [18-2-6오후 4:49:15] 트레이 동시 감지시 즉시 정지..*/
            /*..hui [18-2-6오후 4:51:07] 이쪽으로 들어올일은 없음 윗부분 동시감지 에러 부분에서 처리..*/
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


/// @brief      트레이 탈빙 방향 이동 재시도 횟수 설정 함수
/// @details    트레이 탈빙 방향 이동 재시도 횟수를 설정한다
/// @param      mu8Data : 트레이 탈빙 방향 이동 재시도 횟수
/// @return     void
void Set_TrayDown_ErrorCount(U8 mu8Data)
{
    gu8Err_Tray_Down_Count = mu8Data;
}


/// @brief      트레이 탈빙 방향 이동 재시도 횟수 확인 함수
/// @details    트레이 탈빙 방향 이동 재시도 횟수를 확인하여 반환한다
/// @param      void
/// @return     return : 트레이 탈빙 방향 이동 재시도 횟수
U8 Get_TrayDown_ErrorCount(void)
{
    return  gu8Err_Tray_Down_Count;
}


/// @brief      E62, E63 체크 관련 변수 초기화 함수
/// @details    E62와 E63 체크시 사용하는 변수들을 초기화 시킨다
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


/// @brief      드레인 펌프 에러 체크
/// @details    드레인 펌프 에러 체크 동작을 진행한다
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

    mu8DrainPumpStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_DRAIN);    // 드레인 펌프 동작 상태 확인
    mu8TankLevel = Get_WaterLevel();                                // 기준이 되는 탱크의 수위 상태 확인

    if (mu8DrainPumpStatus == TRUE)
    {   // 드레인 펌프가 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_DRAIN_PUMP_E60] == CLEAR)
        {   // 에러 발생이 안된 상태
            if (SError.gu8Detect[ERROR_ID_DRAIN_PUMP_E60] == CLEAR)
            {   // 에러 감지가 안된 상태 또는 감지 재동작 상태
                if (mu8TankLevel > WATER_LEVEL_LOW)
                {   // 저수위 센서 이상 수위가 감지되고 있는 경우
                    if (mu16PumpActTime >= DRAIN_PUMP_ERROR_DETECT_TIME)
                    {   // 기준 시간 이상 수위가 감지된 경우
                        mu16PumpActTime = 0;

                        Set_ErrorDetect(ERROR_ID_DRAIN_PUMP_E60, SET);      // 에러 감지 상태 설정

                        if (mu8ErrorDetectCount >= DRAIN_PUMP_ERROR_CONFIRM_COUNT)
                        {   // 기준 횟수 이상 연속 재감지 된 경우
                            Set_ErrorStatus(ERROR_ID_DRAIN_PUMP_E60, SET);  // 에러 확정
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
                {   // 저수위 센서 미만으로 수위가 내려온 경우
                    mu16PumpActTime = 0;
                    mu8ErrorDetectCount = 0;
                }
            }
            else
            {   // 에러 감지가 된 상태인 경우
                mu16PumpActTime = 0;
            }
        }
        else
        {   // 에러가 발생된 상태
            mu16PumpActTime = 0;
            mu8ErrorDetectCount = 0;
        }
    }
    else
    {   // 드레인 펌프가 동작 중이 아닌 상태
        if (SError.gu8Status[ERROR_ID_DRAIN_PUMP_E60] == CLEAR)
        {   // 에러가 발생되지 않은 상태
            if (SError.gu8Detect[ERROR_ID_DRAIN_PUMP_E60] == SET)
            {
                if (mu8ErrorCheckDelay >= DRAIN_PUMP_ERROR_CHECK_DELAY)
                {   // 일정 시간 지연 후
                    Set_ErrorDetect(ERROR_ID_DRAIN_PUMP_E60, CLEAR);      // 에러 감지를 재검사 할 수 있도록 감지 상태를 초기화
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


/// @brief      탄산 펌프 에러 체크
/// @details    탄산 펌프 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void SodaPumpErrorCheck(void)
{
#if (SODA_PUMP_ERROR_CHECK_USE == USE)
    static mu8ErrorDetectTime = 0;
    U8 mu8SodaPumpStatus = 0;
    U16 mu16SodaPumpFeedbackAD = 0;

    mu8SodaPumpStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_SODA);      // 탄산 펌프 동작 상태 확인
    mu16SodaPumpFeedbackAD = Get_ADC_Data(ADC_ID_SODA_PUMP);        // 탄산 펌프 피드백 AD값 확인

    if (mu8SodaPumpStatus == TRUE)
    {   // 펌프가 동작 중인 경우
        if ( (mu16SodaPumpFeedbackAD < SODA_PUMP_ERROR_UNDER_STANDARD_AD) ||
             (mu16SodaPumpFeedbackAD > SODA_PUMP_ERROR_OVER_STANDARD_AD) )
        {   // 기준값 미만 또는 초과인 경우
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
    {   // 펌프가 동작 중이 아닌 경우
        mu8ErrorDetectTime = 0;
    }
#endif
}


/// @brief      추출 이상 에러 체크
/// @details    추출 이상 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void AbnormalEffluentErrorCheck(void)
{
#if (ABNORMAL_EFFLUENT_ERROR_CHECK_USE == USE)
    static U8 mu8ErrorDetectTime = 0;
    U8 mu8Status = 0;

    mu8Status = Get_OperatingStatus();              // 현재 제품의 동작 상태 확인

    if (mu8Status == OPERATION_ID_EFFLUENT)
    {   // 추출 동작 상태인 경우
        if (SError.gu8Status[ERROR_ID_ABNORMAL_EFFLUENT_E03] == CLEAR)
        {   // 추출 이상 에러 상태가 아닌 경우
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


/// @brief      정수 탱크 수위 센서 이상 에러 체크
/// @details    정수 탱크 수위 센서 이상 에러 체크 동작을 진행한다
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

    mu8Status = Get_OperatingStatus();              // 현재 제품의 동작 상태 확인

#if (MIDDLE_SENSOR_USE == USE)      // 중수위 센서가 있는 경우
    // 에러 검출 *********************************************************************************************
    if (!mu8LowSensor && mu8MiddleSensor)
    {   // 저수위 미감지되는데 중수위 센서 감지되는 경우
        if (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == CLEAR)
        {   // 저수위 에러가 발생하지 않은 경우
            Set_ErrorStatus(ERROR_ID_LOW_SENSOR_E11, SET);              // 저수위 센서 에러는 1회에서 바로 발생
        }
    }
    else if (mu8LowSensor && !mu8MiddleSensor && mu8HighSensor)
    {   // 저수위, 만수위는 감지되는데 중수위가 미감지되는 경우
        if (SError.gu8Status[ERROR_ID_MIDDLE_SENSOR_E12] == CLEAR)
        {   // 중수위 에러가 발생하지 않은 경우
            Set_ErrorStatus(ERROR_ID_MIDDLE_SENSOR_E12, SET);           // 중수위 센서 에러는 1회에서 바로 발생
        }
    }
    else if (!mu8LowSensor && !mu8MiddleSensor && mu8HighSensor)
    {   // 만수위는 감지되는데 저수위, 중수위가 미감지되는 경우
        if (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == CLEAR)
        {   // 저, 중수위 에러가 발생하지 않은 경우
            Set_ErrorStatus(ERROR_ID_LOW_MIDDLE_SENSOR_E15, SET);       // 저, 중수위 센서 복합 에러는 1회에서 바로 발생
        }
    }
    else if (mu8LowSensor && !mu8MiddleSensor && !mu8HighSensor && mu8OverFlowSensor)
    {   // 저수위, 오버플로우는 감지되는데 중수위, 만수위가 미감지되는 경우
        if (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == CLEAR)
        {   // 중, 만수위 에러가 발생하지 않은 경우
            Set_ErrorStatus(ERROR_ID_MIDDLE_HIGH_SENSOR_E16, SET);      // 중, 만수위 센서 복합 에러는 1회에서 바로 발생
        }
    }
    else if (mu8LowSensor && mu8MiddleSensor && !mu8HighSensor && mu8OverFlowSensor)
    {   // 저수위, 중수위, 오버플로우는 감지되는데 만수위가 미감지되는 경우
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == CLEAR)
        {   // 만수위 에러 감지 상태가 미감지이면
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, SET);         // 만수위 에러 상태 감지 상태로 설정함

            if (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == CLEAR)
            {   // 만수위 에러가 발생되지 않은 상태이면
                if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] >= 3)    // 3번 누적으로 에러 발생
                {
                    SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;
                    Set_ErrorStatus(ERROR_ID_HIGH_SENSOR_E13, SET);     // 만수위 에러 확정
                }
                else
                {
                    SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13]++;
                }
            }
        }
    }
    else if (!mu8LowSensor && !mu8MiddleSensor && !mu8HighSensor && mu8OverFlowSensor)
    {   // 오버플로우는 감지되는데 저수위, 중수위, 만수위가 미감지되는 경우
        if (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == CLEAR)
        {   // 오버 플로우 에러가 발생되지 않은 상태이면
            Set_ErrorStatus(ERROR_ID_OVER_FLOW_E17, SET);   // 오버 플로우 에러는 1회 감지시 바로 발생
        }
    }
    else if (mu8LowSensor && mu8MiddleSensor && mu8HighSensor && mu8OverFlowSensor)
    {   // 저수위, 중수위, 만수위, 오버플로우가 모두 감지되는 경우
        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == CLEAR)
        {   // Feed Valve 에러 감지 상태가 미감지이면
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, SET);      // Feed Valve 에러 감지 상태로 설정

            if (SError.gu8Status[ERROR_ID_FEED_VALVE_E10] == CLEAR)
            {   // Feed Valve 에러 상태가 아닌 경우
                if (mu8Status == OPERATION_ID_EW_STERILIZE)
                {   // EW 살균 동작 중이면
                    SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
                    Set_ErrorStatus(ERROR_ID_FEED_VALVE_E10, SET);          // EW 살균 중에는 Feed Valve 에러 1회에 확정
                }
                else
                {
                    if (SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] >= 3)        // 3번 누적으로 에러 발생
                    {
                        SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
                        Set_ErrorStatus(ERROR_ID_FEED_VALVE_E10, SET);          // Feed Valve 에러 확정
                    }
                    else
                    {
                        SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10]++;
                    }
                }
            }
        }
    }

    // 에러 해제 *********************************************************************************************
    if (mu8LowSensor)
    {   // 저수위가 감지되는 경우
        if (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == SET)
        {   // 저수위 센서 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_LOW_SENSOR_E11, CLEAR);            // 저수위 센서 에러 해제
        }

        if (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == SET)
        {   // 저, 중수위 센서 복합 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_LOW_MIDDLE_SENSOR_E15, CLEAR);     // 저, 중수위 센서 복합 에러 해제
        }

        if (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET)
        {   // OverFlow 에러 상태인 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_OVER_FLOW_E17, CLEAR);             // OverFlow 에러 해제
        }
    }
    else
    {   // 저수위가 미감지되는 경우
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // 만수위 에러가 감지되었던 경우 Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] > 0)
        {   // 만수위 센서 에러 카운터 초기화
            SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;      // Feed Valve 에러 카운터 초기화
        }

        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve 에러가 감지되었던 경우 Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] > 0)
        {   // Feed Valve 에러 카운터 초기화
            SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
        }
    }

    if (mu8MiddleSensor)
    {   // 중수위가 감지되는 경우
        if (SError.gu8Status[ERROR_ID_MIDDLE_SENSOR_E12] == SET)
        {   // 중수위 에러가 발생한 경우
            Set_ErrorStatus(ERROR_ID_MIDDLE_SENSOR_E12, CLEAR);         // 중수위 센서 에러 발생됐던 경우 자동 해제
        }

        if (SError.gu8Status[ERROR_ID_LOW_MIDDLE_SENSOR_E15] == SET)
        {   // 저, 중수위 센서 복합 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_LOW_MIDDLE_SENSOR_E15, CLEAR);     // 저, 중수위 센서 복합 에러 해제
        }

        if (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == SET)
        {   // 중, 만수위 센서 복합 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_MIDDLE_HIGH_SENSOR_E16, CLEAR);    // 중, 만수위 센서 복합 에러 발생됐던 경우 자동 해제
        }

        if (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET)
        {   // OverFlow 에러 상태인 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_OVER_FLOW_E17, CLEAR);             // OverFlow 에러 해제
        }
    }
    else
    {   // 중수위가 미감지 되는 경우
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // 만수위 에러가 감지되었던 경우 Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve 에러가 감지되었던 경우 Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }
    }

    if (mu8HighSensor)
    {   // 만수위가 감지 되는 경우
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // 만수위 에러가 감지되었던 경우 Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] > 0)
        {   // 만수위 센서 에러 카운터 초기화
            SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;      // Feed Valve 에러 카운터 초기화
        }

        if (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // 만수위 에러 발생했었던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_HIGH_SENSOR_E13, CLEAR);            // 만수위 센서 에러 해제
        }

        if (SError.gu8Status[ERROR_ID_MIDDLE_HIGH_SENSOR_E16] == SET)
        {   // 중, 만수위 센서 복합 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_MIDDLE_HIGH_SENSOR_E16, CLEAR);    // 중, 만수위 센서 복합 에러 발생됐던 경우 자동 해제
        }

        if (SError.gu8Status[ERROR_ID_OVER_FLOW_E17] == SET)
        {   // OverFlow 에러 상태인 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_OVER_FLOW_E17, CLEAR);             // OverFlow 에러 해제
        }
    }
    else
    {   // 만수위가 미감지 되는 경우
        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve 에러가 감지되었던 경우 Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }
    }

#else           // 중수위 센서가 없는 경우

    // 에러 검출 *********************************************************************************************
    if (!mu8LowSensor && mu8HighSensor)
    {   // 저수위 미감지되는데 만수위 센서 감지되는 경우
        if (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == CLEAR)
        {   // 저수위 에러가 발생하지 않은 경우
            Set_ErrorStatus(ERROR_ID_LOW_SENSOR_E11, SET);              // 저수위 센서 에러는 1회에서 바로 발생
        }
    }
    else if (mu8OverFlowSensor)
    {   // 물넘침 센서 감지시
        if (!mu8LowSensor && !mu8HighSensor)
        {   // 저수위, 고수위 센서 모두 미감지(복합 센서 에러)
            if (SError.gu8Detect[ERROR_ID_COMPLEX_SENSOR_E14] == CLEAR)
            {   // 복합 수위 센서 에러 감지 상태가 아닌 경우
                Set_ErrorDetect(ERROR_ID_COMPLEX_SENSOR_E14, SET);      // 복합 수위 센서 에러 감지 상태 설정

                if (SError.gu8Status[ERROR_ID_COMPLEX_SENSOR_E14] == CLEAR)
                {   // 복합 수위 센서 에러가 발생하지 않은 경우
                    if (SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] >= 3)
                    {   // 3번 누적으로 에러 발생
                        SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] = 0;
                        Set_ErrorStatus(ERROR_ID_COMPLEX_SENSOR_E14, SET);      // 에러 확정
                    }
                    else
                    {
                        SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14]++;
                    }
                }
            }
        }
        else if (mu8LowSensor && !mu8HighSensor)
        {   // 저수위만 감지(만수위 센서 에러)
            if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == CLEAR)
            {   // 만수위 센서 에러 감지 상태가 아닌 경우
                Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, SET);      // 만수위 센서 에러 감지 상태 설정

                if (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == CLEAR)
                {   // 만수위 센서 에러가 발생하지 않은 경우
                    if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] >= 3)
                    {   // 3번 누적으로 에러 발생
                        SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;
                        Set_ErrorStatus(ERROR_ID_HIGH_SENSOR_E13, SET);      // 에러 확정
                    }
                    else
                    {
                        SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13]++;
                    }
                }
            }
        }
        else if (mu8LowSensor && mu8HighSensor)
        {  // 수위 센서가 전부 다 감지되는 경우(Feed Valve Error)
            if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == CLEAR)
            {   // 피드 밸브 에러 감지 상태가 아닌 경우
                Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, SET);      // 피드 밸브 에러 감지 상태 설정

                if (SError.gu8Status[ERROR_ID_FEED_VALVE_E10] == CLEAR)
                {   // 피드 밸브 에러가 발생하지 않은 경우
                    if (mu8Status == OPERATION_ID_EW_STERILIZE)
                    {   // EW 살균 동작 중이면
                        SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
                        Set_ErrorStatus(ERROR_ID_FEED_VALVE_E10, SET);          // EW 살균 중에는 Feed Valve 에러 1회에 확정
                    }
                    else
                    {
                        if (SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] >= 3)        // 3번 누적으로 에러 발생
                        {
                            SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
                            Set_ErrorStatus(ERROR_ID_FEED_VALVE_E10, SET);          // Feed Valve 에러 확정
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

    // 에러 해제 *********************************************************************************************
    if (mu8LowSensor)
    {   // 저수위 센서 감지
        if (SError.gu8Status[ERROR_ID_LOW_SENSOR_E11] == SET)
        {   // 저수위 센서 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_LOW_SENSOR_E11, CLEAR);            // 에러 자동 해제
        }

        if (SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] > 0)
        {   // 기존에 복합 센서 에러 카운팅 중이었던 경우
            SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] = 0;
        }
    }
    else
    {   // 저수위 센서 미감지
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // 만수위 에러가 감지되었던 경우 Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] > 0)
        {   // 만수위 에러 카운트 초기화
            SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;
        }

        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve 에러가 감지되었던 경우 Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }

        if (SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] > 0)
        {   // Feed Valve 에러 카운터 초기화
            SError.gu8CheckCount[ERROR_ID_FEED_VALVE_E10] = 0;
        }
    }

    if (mu8HighSensor)
    {   // 만수위 센서 감지
        if (SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] > 0)
        {   // 기존에 복합 센서 에러 카운팅 중이었던 경우
            SError.gu8CheckCount[ERROR_ID_COMPLEX_SENSOR_E14] = 0;
        }

        if (SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] > 0)
        {   // 만수위 에러 카운트 초기화
            SError.gu8CheckCount[ERROR_ID_HIGH_SENSOR_E13] = 0;
        }

        if (SError.gu8Status[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // 만수위 에러 발생했었던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }
    }
    else
    {   // 만수위 센서 미감지
        if (SError.gu8Detect[ERROR_ID_FEED_VALVE_E10] == SET)
        {   // Feed Valve 에러가 감지되었던 경우 Flag Clear
            Set_ErrorDetect(ERROR_ID_FEED_VALVE_E10, CLEAR);
        }
    }

    if (mu8OverFlowSensor)
    {   // overflow 센서 감지

    }
    else
    {   // overflow 센서 미감지
        if (SError.gu8Detect[ERROR_ID_HIGH_SENSOR_E13] == SET)
        {   // OverFlow 센서 미감지시 만수위 에러 Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_HIGH_SENSOR_E13, CLEAR);
        }

        if (SError.gu8Detect[ERROR_ID_COMPLEX_SENSOR_E14] == SET)
        {   // 수위 복합센서 에러 체크를 위해 물넘침 센서 미감지시 Detect Flag Clear
            Set_ErrorDetect(ERROR_ID_COMPLEX_SENSOR_E14, CLEAR);
        }
    }

#endif

#endif
}


/// @brief      온수 탱크 만수위 센서 이상 에러 체크
/// @details    온수 탱크 만수위 센서 이상 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void HotHighLevelErrorCheck(void)
{
#if (HOT_HIGH_SENSOR_ERROR_CHECK_USE == USE)

    U8 mu8HighSensor = 0;
    U8 mu8OverFlowSensor = 0;

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)     // 전극봉 타입 온수 탱크 수위 센서인 경우

#if (HOT_HIGH_SENSOR_OVERFLOW_SENSOR_USE == NO_USE)    // 온수 탱크 오버 플러우가 없는 경우
    U8 mu8WaterInputStatus = 0;
    static U16 mu16ErrorDetectTime = 0;
#endif

    mu8HighSensor = Get_ElecLevel_Status();
#else
    mu8HighSensor = Get_WaterLevel_Status(LEVEL_ID_HOT_HIGH);
#endif

#if (HOT_HIGH_SENSOR_OVERFLOW_SENSOR_USE == USE)    // 온수 탱크 오버 플러우가 있는 경우
    mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_OVERFLOW);

    if (!mu8HighSensor && mu8OverFlowSensor)
    {   // 온수 만수위가 감지되지 않은 상태에서 온수 오버플로우가 감지되는 경우
        if (SError.gu8Status[ERROR_ID_HOT_HIGH_SENSOR_E33] == CLEAR)
        {   // 온수 만수위 에러가 발생하지 않은 상태
            Set_ErrorStatus(ERROR_ID_HOT_HIGH_SENSOR_E33, SET);
        }
    }

#else       // 온수 탱크 오버 플로우가 없는 경우
    U8 mu8HotWaterLevel = 0;
    U8 mu8Status = 0;

    mu8Status = Get_OperatingStatus();              // 현재 제품의 동작 상태 확인
    mu8HotWaterLevel = Get_HotWaterLevel();

    mu8WaterInputStatus = Get_ValveStatus(VALVE_ID_WATER_IN);              // 입수 동작 상태 확인

    if (mu8Status == OPERATION_ID_STAND_BY)
    {   // 제품이 대기 상태
        if (mu8HotWaterLevel < HOT_WATER_LEVEL_HIGH)
        {   // 온수 만수위가 아닌 경우
            if (mu8WaterInputStatus == HIGH)
            {   // 입수 동작 중이면
                if (SError.gu8Status[ERROR_ID_HOT_HIGH_SENSOR_E33] == CLEAR)
                {   // 온수 만수위 에러가 발생하지 않은 상태
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
    {   // 제품이 물을 사용하고 동작을 하고 있는 상태
        mu16ErrorDetectTime = 0;
    }
#endif

#endif
}


/// @brief      빙축탱크 수위 센서 이상 에러 체크
/// @details    빙축탱크 수위 센서 이상 에러 체크 동작을 진행한다
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
    {   // 냉수 기능이 켜진 상태
        if (mu8IcdColdWaterHighSensor == FALSE)
        {   // 빙축 탱크 수위 센서가 미감지 상태
            if (SError.gu8Status[ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20] == CLEAR)
            {   // 빙축 탱크 수위 센서 에러가 발생하지 않은 상태
                Set_ErrorStatus(ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20, SET);
            }
        }
        else
        {   // 빙축 탱크 수위 센서가 감지 상태
            if (SError.gu8Status[ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20] == SET)
            {   // 빙축 탱크 수위 센서 에러가 발생한 상태
                Set_ErrorStatus(ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20, CLEAR);
            }
        }
    }
#endif
}


/// @brief      냉수 탱크 수위 센서 이상 에러 체크
/// @details    냉수 탱크 수위 센서 이상 에러 체크 동작을 진행한다
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
    mu8LowSensor = Get_WaterLevel_Status(LEVEL_ID_COLD_LOW);        // 냉수 저수위 센서
#endif

#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == USE)
    mu8HighSensor = Get_WaterLevel_Status(LEVEL_ID_COLD_HIGH);      // 냉수 만수위 센서
#endif

    mu8OverFlowSensor = Get_WaterLevel_Status(LEVEL_ID_LOW);        // 정수 저수위 센서

#if (COLD_LEVEL_ERROR_LOW_SENSOR_USE == USE)        // 냉수 저수위가 있는 경우

    // 냉수 저수위 센서 에러
#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == USE)       // 냉수 만수위가 있는 경우

    // 에러 검출 *********************************************************************************************
    if (!mu8LowSensor && mu8HighSensor)
    {   // 냉수 저수위 미감지되는데 냉수 만수위 센서 감지되는 경우
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == CLEAR)
        {   // 냉수 저수위 에러가 발생하지 않은 경우
            Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, SET);              // 냉수 저수위 센서 에러는 1회에서 바로 발생
        }
    }

    // 에러 해제 *********************************************************************************************
    if (mu8LowSensor)
    {   // 냉수 저수위 센서 감지
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET)
        {   // 냉수 저수위 센서 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, CLEAR);            // 에러 자동 해제
        }
    }
    else if (!mu8LowSensor && !mu8HighSensor)
    {   // 냉수 저수위, 냉수 만수위 모두 미감지
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET)
        {   // 냉수 저수위 센서 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, CLEAR);            // 에러 자동 해제
        }
    }
    else
    {

    }

#else       // 냉수 만수위가 없는 경우

    // 에러 검출 *********************************************************************************************
    if (!mu8LowSensor && mu8OverFlowSensor)
    {   // 냉수 저수위 미감지되는데 정수 저수위 센서 감지되는 경우
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == CLEAR)
        {   // 냉수 저수위 에러가 발생하지 않은 경우
            if (mu16LowErrorDetectTime >= COLD_LOW_SENSOR_ERROR_DETECT_TIME)
            {
                Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, SET);              // 냉수 저수위 센서 에러 발생
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

    // 에러 해제 *********************************************************************************************
    if (mu8LowSensor)
    {   // 냉수 저수위 센서 감지
        if (SError.gu8Status[ERROR_ID_COLD_LOW_SENSOR_E21] == SET)
        {   // 냉수 저수위 센서 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_COLD_LOW_SENSOR_E21, CLEAR);            // 에러 자동 해제
        }
    }
    else
    {

    }

#endif

#endif

#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == USE)       // 냉수 만수위가 있는 경우

    // 냉수 만수위 센서 에러
#if (COLD_LEVEL_ERROR_LOW_SENSOR_USE == USE)        // 냉수 저수위가 있는 경우

    // 에러 검출 *********************************************************************************************
    if (mu8LowSensor && !mu8HighSensor && mu8OverFlowSensor)
    {   // 냉수 저수위 감지, 냉수 만수위 미감지, 정수 저수위 감지시
        if (SError.gu8Detect[ERROR_ID_COLD_HIGH_SENSOR_E23] == CLEAR)
        {   // 냉수 만수위 에러 감지가 안된 경우
            Set_ErrorDetect(ERROR_ID_COLD_HIGH_SENSOR_E23, SET);                // 냉수 만수위 에러 감지 상태 설정

            if (SError.gu8Status[ERROR_ID_COLD_HIGH_SENSOR_E23] == CLEAR)
            {   // 냉수 만수위 에러가 발생하지 않은 경우
                if (SError.gu8CheckCount[ERROR_ID_COLD_HIGH_SENSOR_E23] >= 3)
                {
                    Set_ErrorStatus(ERROR_ID_COLD_HIGH_SENSOR_E23, SET);        // 냉수 만수위 센서 에러 발생
                }
                else
                {
                    SError.gu8CheckCount[ERROR_ID_COLD_HIGH_SENSOR_E23]++;
                }
            }
            else
            {   // 냉수 만수위 에러가 발생한 경우
                SError.gu8CheckCount[ERROR_ID_COLD_HIGH_SENSOR_E23] = 0;
            }
        }
    }
    else
    {
        if (SError.gu8Detect[ERROR_ID_COLD_HIGH_SENSOR_E23] == SET)
        {   // 냉수 만수위 에러 감지가 된 경우
            Set_ErrorDetect(ERROR_ID_COLD_HIGH_SENSOR_E23, CLEAR);            // 냉수 만수위 에러 감지 해제 상태 설정
        }
    }

    // 에러 해제 *********************************************************************************************
    if (mu8HighSensor)
    {   // 냉수 만수위 센서 감지
        if (SError.gu8Status[ERROR_ID_COLD_HIGH_SENSOR_E23] == SET)
        {   // 냉수 만수위 센서 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_COLD_HIGH_SENSOR_E23, CLEAR);            // 에러 자동 해제
        }
    }
    else
    {

    }

#else       // 냉수 저수위가 없는 경우

    // 에러 검출 *********************************************************************************************
    if (!mu8HighSensor && mu8OverFlowSensor)
    {   // 냉수 만수위 미감지, 정수 저수위 감지 상태
        if (SError.gu8Status[ERROR_ID_COLD_HIGH_SENSOR_E23] == CLEAR)
        {   // 냉수 만수위 에러가 발생하지 않은 경우
            mu16HighErrorDetectTimeSec++;

            if (mu16HighErrorDetectTimeSec >= 600)
            {   // 60초가 경과한 경우
                mu16HighErrorDetectTimeSec = 0;

                mu8HighErrorDetectTimeMin++;

                if (mu8HighErrorDetectTimeMin >= 120)
                {   // 120min 경과 후
                    mu8HighErrorDetectTimeMin = 0;

                    Set_ErrorStatus(ERROR_ID_COLD_HIGH_SENSOR_E23, SET);
                }
            }
        }
        else
        {   // 냉수 만수위 에러가 발생한 경우
            mu16HighErrorDetectTimeSec = 0;
            mu8HighErrorDetectTimeMin = 0;
        }
    }
    else
    {
        mu16HighErrorDetectTimeSec = 0;
        mu8HighErrorDetectTimeMin = 0;
    }

    // 에러 해제 *********************************************************************************************
    if (mu8HighSensor)
    {   // 냉수 만수위 센서 감지
        if (SError.gu8Status[ERROR_ID_COLD_HIGH_SENSOR_E23] == SET)
        {   // 냉수 만수위 센서 에러 발생됐던 경우 자동 해제
            Set_ErrorStatus(ERROR_ID_COLD_HIGH_SENSOR_E23, CLEAR);            // 에러 자동 해제
        }
    }
    else
    {

    }

#endif

#endif

#endif
}


/// @brief      정수 온도 센서 이상 에러 체크
/// @details    정수 온도 센서 이상 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void RoomTempErrorCheck(void)
{
#if (ROOM_TEMP_1_ERROR_CHECK_USE == USE)    // 정수 온도 센서1 에러 사용시

    U16 mu16RoomTemp1_AD = 0;
    static U8 mu8RoomTemp1ErrorDetectTime = 0;

//    mu16RoomTemp1_AD = Get_ADC_Data(ADC_ID_TH_ROOM_1);   // 정수 온도 센서1 온도 AD값 확인

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

#if (ROOM_TEMP_2_ERROR_CHECK_USE == USE)    // 정수 온도 센서2 에러 사용시

    U16 mu16RoomTemp2_AD = 0;
    static U8 mu8RoomTemp2ErrorDetectTime = 0;

    mu16RoomTemp2_AD = Get_ADC_Data(ADC_ID_TH_ROOM_2);   // 정수 온도 센서2 온도 AD값 확인

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


/// @brief      온수 온도 센서 이상 에러 체크
/// @details    온수 온도 센서 이상 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void HotTempErrorCheck(void)
{
#if (TANK_HOT_TEMP_ERROR_CHECK_USE == USE)    // 온수 탱크 온도 센서 에러 사용시

    U16 mu16HotTankTemp_AD = 0;
    U8 mu8HotOnOff = 0;

    static U8 mu8HotTankTempErrorDetectTime = 0;

//    mu16HotTankTemp_AD = Get_ADC_Data(ADC_ID_TH_HOT);   // 온수 탱크 온도 센서 온도 AD값 확인
    mu8HotOnOff = Get_HotOnOff();

    if (mu8HotOnOff == TRUE)
    {   // 온수 기능이 켜진 경우
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

#if (HOT_IN_TEMP_ERROR_CHECK_USE == USE)    // 순간 온수 입수 온도 센서 에러 사용시

    U16 mu16HotInTemp_AD = 0;
    static U8 mu8HotInTempErrorDetectTime = 0;

    mu16HotInTemp_AD = Get_ADC_Data(ADC_ID_TH_HOT_IN);   // 순간 온수 입수 온도 센서 온도 AD값 확인

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

#if (HOT_OUT_TEMP_ERROR_CHECK_USE == USE)   // 순간 온수 출수 온도 센서 에러 사용시

    U16 mu16HotOutTemp_AD = 0;
    static U8 mu8HotOutTempErrorDetectTime = 0;

    mu16HotOutTemp_AD = Get_ADC_Data(ADC_ID_TH_HOT_OUT);   // 순간 온수 출수 온도 센서 온도 AD값 확인

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

#if (HOT_BODY_TEMP_ERROR_CHECK_USE == USE)  // 순간 온수 히터 내부 온도 센서 에러 사용시

    U16 mu16HotBodyTemp_AD = 0;
    static U8 mu8HotBodyTempErrorDetectTime = 0;

    mu16HotBodyTemp_AD = Get_ADC_Data(ADC_ID_TH_HOT_BODY);   // 순간 온수 히터 내부 온도 센서 온도 AD값 확인

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


/// @brief      냉수 온도 센서 이상 에러 체크
/// @details    냉수 온도 센서 이상 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void ColdTempErrorCheck(void)
{
#if (COLD_TEMP_1_ERROR_CHECK_USE == USE)    // 냉수 온도 센서1 에러 사용시

    U16 mu16ColdTemp1_AD = 0;
    U8 mu8ColdOnOff1 = 0;

    static U8 mu8ColdTemp1ErrorDetectTime = 0;

    mu16ColdTemp1_AD = Get_ADC_Data(ADC_ID_TH_COOL);   // 냉수 온도 센서1 온도 AD값 확인
    mu8ColdOnOff1 = Get_ColdOnOff();

    if (mu8ColdOnOff1 == TRUE)
    {   // 냉수 기능이 켜진 경우
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

#if (COLD_TEMP_2_ERROR_CHECK_USE == USE)    // 냉수 온도 센서2 에러 사용시

    U16 mu16ColdTemp2_AD = 0;
    U8 mu8ColdOnOff2 = 0;

    static U8 mu8ColdTemp2ErrorDetectTime = 0;

    mu16ColdTemp2_AD = Get_ADC_Data(ADC_ID_TH_COOL_2);   // 냉수 온도 센서2 온도 AD값 확인
    mu8ColdOnOff2 = Get_ColdOnOff();

    if (mu8ColdOnOff2 == TRUE)
    {   // 냉수 기능이 켜진 경우
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


/// @brief      아이스 트레이 입수 온도 센서 이상 에러 체크
/// @details    아이스 트레이 입수 온도 센서 이상 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void IceTrayInputTempErrorCheck(void)
{
#if (ICE_TRAY_INPUT_TEMP_ERROR_CHECK_USE == USE)   // 아이스 트레이 입수 온도 에러 센서 사용시

    U16 mu16IceTrayInputTemp_AD = 0;
    static U8 mu8IceTrayInputTempErrorDetectTime = 0;

    mu16IceTrayInputTemp_AD = Get_ADC_Data(ADC_ID_TH_ICE_TRAY_INPUT);   // 아이스 트레이 입수 온도 센서 온도 AD값 확인

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


/// @brief      외기 온도 센서 이상 에러 체크
/// @details    외기 온도 센서 이상 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void AirTempErrorCheck(void)
{
#if (AIR_TEMP_1_ERROR_CHECK_USE == USE)     // 외기 온도 센서1 에러 사용시

    U16 mu16AirTemp1_AD = 0;

    static U8 mu8AirTemp1ErrorDetectTime = 0;

    mu16AirTemp1_AD = Get_ADC_Data(ADC_ID_TH_AIR);   // 외기 온도 센서1 온도 AD값 확인

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

#if (AIR_TEMP_2_ERROR_CHECK_USE == USE)     // 외기 온도 센서2 에러 사용시

    U16 mu16AirTemp2_AD = 0;

    static U8 mu8AirTemp2ErrorDetectTime = 0;

    mu16AirTemp2_AD = Get_ADC_Data(ADC_ID_TH_AIR_2);   // 외기 온도 센서2 온도 AD값 확인

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


/// @brief      TEM 방열판 온도 센서 이상 에러 체크
/// @details    TEM 방열판 온도 센서 이상 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void HeatsinkTempErrorCheck(void)
{
#if (TEM_HEATSINK_TEMP_ERROR_CHECK_USE == USE)

    F32 mf32HeatsinkTemp = 0;

    static U8 mu8HeatsinkTempErrorDetectTime = 0;

    mf32HeatsinkTemp = Get_Temp(ADC_ID_TH_HEATSINK);   // TEM 방열판 온도 센서 온도값 확인

    if (mf32HeatsinkTemp >= TEM_HEATSINK_TEMP_ERROR_OVER_STANDARD_TEMP)
    {   // 기준 온도 이상인 경우
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


/// @brief      온수 과열 에러 체크
/// @details    온수 과열 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void HotOverheatErrorCheck(void)
{
#if (HOT_OVERHEAT_ERROR_CHECK_USE == USE)

#if (HOT_OVERHEAT_ERROR_TANK_USE == USE)    // 탱크형 제품인 경우

    F32 mf32HotTemp = 0;
    U8 mu8HotOnOff = 0;

    static U16 mu16HotOverheatErrorDetectTime = 0;

    mf32HotTemp = Get_Temp(HOT_THERMISTOR_ID);   // 온수 온도 센서 온도값 확인
    mu8HotOnOff = Get_HotOnOff();

    if (mu8HotOnOff == TRUE)
    {   // 온수 기능이 켜진 경우
        if (mf32HotTemp >= HOT_OVERHEAT_ERROR_OVER_STANDARD_TEMP)
        {   // 기준 온도 이상인 경우
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
    {   // 온수 기능이 꺼진 경우
        mu16HotOverheatErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_HOT_OVERHEAT_E40] == SET)
        {
            Set_ErrorStatus(ERROR_ID_HOT_OVERHEAT_E40, CLEAR);
        }
    }

#else   // 직수형 제품인 경우

    F32 mf32HotTemp = 0;

    static U8 mu8HotOverheatErrorDetectTime = 0;

    mf32HotTemp = Get_Temp(ADC_ID_TH_HOT_OUT);   // 온수 출수 온도 센서 온도값 확인

    if (mf32HotTemp >= HOT_OVERHEAT_ERROR_OVER_STANDARD_TEMP)
    {   // 기준 온도 이상인 경우
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


/// @brief      온수 펌프 에러 체크
/// @details    온수 펌프 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void HotPumpErrorCheck(void)
{
#if (HOT_PUMP_ERROR_CHECK_USE == USE)
    static U8 mu8ErrorDetectTime = 0;
    U8 mu8HotPumpStatus = 0;
    U16 mu16HotPumpFeedbackAD = 0;

    mu8HotPumpStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_HOT);    // 온수 펌프의 실제 동작 상태 확인
    mu16HotPumpFeedbackAD = Get_ADC_Data(ADC_ID_HOT_PUMP);       // 부스트 펌프의 피드백 AD값 확인

    if (mu8HotPumpStatus == TRUE)
    {   // 펌프가 동작 중인 경우
        if ( (mu16HotPumpFeedbackAD < HOT_PUMP_ERROR_UNDER_STANDARD_AD) ||
             (mu16HotPumpFeedbackAD > HOT_PUMP_ERROR_OVER_STANDARD_AD) )
        {   // 기준값 미만 또는 초과인 경우
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
    {   // 펌프가 동작 중이 아닌 경우
        mu8ErrorDetectTime = 0;
    }
#endif
}


/// @brief      BLDC Comp 에러 체크
/// @details    BLDC Comp  에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void BLDC_CompErrorCheck(void)
{
#if (BLDC_COMP_ERROR_CHECK_USE == USE)
    U8 mu8ErrorCode = 0;

    mu8ErrorCode = Get_BLDC_Comp_Error();   // BLDC Comp 에러 확인

    switch (mu8ErrorCode)
    {
        case BLDC_COMP_ERROR_81 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_1_E81, SET);     // 에러 확정
            break;

        case BLDC_COMP_ERROR_82 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_2_E82, SET);     // 에러 확정
            break;

        case BLDC_COMP_ERROR_83 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_3_E83, SET);     // 에러 확정
            break;

        case BLDC_COMP_ERROR_84 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_4_E84, SET);     // 에러 확정
            break;

        case BLDC_COMP_ERROR_85 :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_ACT_5_E85, SET);     // 에러 확정
            break;

        case BLDC_COMP_ERROR_COMM :
            Set_ErrorStatus(ERROR_ID_BLDC_COMP_COMMUNICATION_E27, SET);     // 에러 확정
            break;

        default:
            break;
    }
#endif
}


/// @brief      일반 Comp 동작 불량 에러 체크(에러 검출 방안 표준 동작 제정된 내용 없음 - 미사용)
/// @details    일반 Comp 동작 불량 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void CompActErrorCheck(void)
{
#if (COMP_ACT_ERROR_CHECK_USE == USE)

#endif
}


/// @brief      순환 펌프 에러 체크
/// @details    순환 펌프 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void CirculatePumpErrorCheck(void)
{
#if (CIRCULATE_PUMP_ERROR_CHECK_USE == USE)
    U8 mu8PumpStatus = 0;
    U16 mu8PumpFeedbackAD = 0;

    static U8 mu8ErrorDetectTime = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8PumpStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_CIRCULATE);         // 순환 펌프 동작 상태 확인
    mu8PumpFeedbackAD = Get_ADC_Data(ADC_ID_CIRCULATION_PUMP_I_FB);     // 순환 펌프 전류 피드백 상태 확인

    if (mu8PumpStatus == TRUE)
    {   // 펌프가 동작 중인 경우
        if ( (mu8PumpFeedbackAD < CIRCULATE_PUMP_ERROR_UNDER_STANDARD_AD) ||
             (mu8PumpFeedbackAD > CIRCULATE_PUMP_ERROR_OVER_STANDARD_AD) )
        {   // 기준값 미만 또는 초과인 경우
            if (SError.gu8Detect[ERROR_ID_CIRCULATE_PUMP_E26] == CLEAR)
            {   // 에러 미감지 상태인 경우
                if (mu8ErrorDetectTime >= CIRCULATE_PUMP_ERROR_DETECT_TIME)
                {   // 에러 감지 기준 시간 경과시
                    mu8ErrorDetectTime = 0;

                    Set_ErrorDetect(ERROR_ID_CIRCULATE_PUMP_E26, SET);      // 에러 감지 상태로 설정

                    if (mu8ErrorDetectCount >= CIRCULATE_PUMP_ERROR_CONFIRM_COUNT)
                    {   // 에러 감지 상태가 기준 횟수 만큼 반복 발생한 경우
                        Set_ErrorStatus(ERROR_ID_CIRCULATE_PUMP_E26, SET);  // 에러 확정
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
            {   // 에러 감지 상태인 경우
                mu8ErrorDetectTime = 0;
            }
        }
        else
        {   // 정상 범위인 경우
            mu8ErrorDetectTime = 0;
            mu8ErrorDetectCount = 0;

            if (SError.gu8Status[ERROR_ID_CIRCULATE_PUMP_E26] == CLEAR)
            {   // 에러 미발생 상태인 경우
                if (SError.gu8Detect[ERROR_ID_CIRCULATE_PUMP_E26] == SET)
                {   // 에러 감지가 되었던 경우
                    Set_ErrorDetect(ERROR_ID_CIRCULATE_PUMP_E26, CLEAR);    // 에러 재검사를 위해 감지 상태 CLEAR
                }
            }
        }
    }
    else
    {   // 펌프가 동작 중이 아닌 경우
        mu8ErrorDetectTime = 0;

        if (SError.gu8Status[ERROR_ID_CIRCULATE_PUMP_E26] == CLEAR)
        {   // 에러 미발생 상태인 경우
            if (SError.gu8Detect[ERROR_ID_CIRCULATE_PUMP_E26] == SET)
            {   // 에러 감지가 되었던 경우
                Set_ErrorDetect(ERROR_ID_CIRCULATE_PUMP_E26, CLEAR);    // 에러 재검사를 위해 감지 상태 CLEAR
            }
        }
    }
#endif
}


/// @brief      교반 모터 에러 체크
/// @details    교반 모터 에러 체크 동작을 진행한다
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

    mu8MotorStatus = Get_StirringMotorStatus_HW(STIRRING_MOTOR_ID_1);   // 교반 모터 동작 상태 확인
    mu8MotorFeedbackAD = Get_ADC_Data(ADC_ID_STIRRING_MOTOR);            // 교반 모터 피드백 상태 확인

    if (mu8ErrorDetectBefore == SET)
    {   // 이전 동작에서 에러 감지된 이력이 있는 경우
        if (SError.gu8Status[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
        {   // 에러 확정이 안된 경우
            if (++mu8ErrorConfirmTimeSec >= 10)
            {   // 1초 카운트
                if (++mu8ErrorConfirmTimeMin >= 60)
                {   // 1분 카운트
                    mu8ErrorConfirmTimeMin = 0;

                    if (++mu16ErrorConfirmTimeHour >= STIRRING_MOTOR_ERROR_CONFIRM_TIME_HOUR)
                    {   // 48시간 경과시
                        mu16ErrorConfirmTimeHour = 0;
                        Set_ErrorStatus(ERROR_ID_STIRRING_MOTOR_E28, SET);  // 에러 확정
                    }
                }
            }
        }
        else
        {   // 에러 확정된 경우
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
    {   // 에러 확정이 안된 경우
        if (mu8MotorStatus == TRUE)
        {   // 모터가 동작 중인 경우
            if (mu8ErrorCheckDelay >= STIRRING_MOTOR_ERROR_CHECK_DELAY)
            {   // 지연 시간 이후
                if ( (mu8MotorFeedbackAD < STIRRING_MOTOR_ERROR_UNDER_STANDARD_AD) ||
                     (mu8MotorFeedbackAD > STIRRING_MOTOR_ERROR_OVER_STANDARD_AD) )
                {   // 기준값 미만 또는 초과인 경우
                    if (SError.gu8Detect[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
                    {   // 에러 미감지 상태인 경우
                        if (mu8ErrorDetectTime >= STIRRING_MOTOR_ERROR_DETECT_TIME)
                        {   // 에러 감지 기준 시간 경과시
                            mu8ErrorDetectTime = 0;

                            Set_ErrorDetect(ERROR_ID_STIRRING_MOTOR_E28, SET);      // 에러 감지 상태로 설정
                            mu8ErrorDetectBefore = SET;                         // 이전 상태가 에러 감지 상태로 설정
                        }
                        else
                        {
                            mu8ErrorDetectTime++;
                        }
                    }
                    else
                    {   // 에러 감지 상태인 경우
                        mu8ErrorDetectTime = 0;
                    }
                }
                else
                {   // 정상 범위인 경우
                    mu8ErrorDetectTime = 0;
                    mu8ErrorDetectBefore = CLEAR;
                    mu16ErrorRecheckDelay = 0;

                    if (SError.gu8Status[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
                    {   // 에러 미발생 상태인 경우
                        if (SError.gu8Detect[ERROR_ID_STIRRING_MOTOR_E28] == SET)
                        {   // 에러 감지가 되었던 경우
                            Set_ErrorDetect(ERROR_ID_STIRRING_MOTOR_E28, CLEAR);    // 에러 재검사를 위해 감지 상태 CLEAR
                        }
                    }
                }
            }
            else
            {   // 지연 시간 이전
                mu8ErrorCheckDelay++;
            }
        }
        else
        {   // 모터가 동작 중이 아닌 경우
            mu8ErrorCheckDelay = 0;
            mu8ErrorDetectTime = 0;

            if (SError.gu8Status[ERROR_ID_STIRRING_MOTOR_E28] == CLEAR)
            {   // 에러 미발생 상태인 경우
                if (SError.gu8Detect[ERROR_ID_STIRRING_MOTOR_E28] == SET)
                {   // 에러 감지가 되었던 경우
                    if (mu16ErrorRecheckDelay >= STIRRING_MOTOR_ERROR_RE_CHECK_DELAY)
                    {   // 재동작 전 지연 시간 이후 재검사를 위해 감지 상태를 초기화
                        mu16ErrorRecheckDelay = 0;

                        Set_ErrorDetect(ERROR_ID_STIRRING_MOTOR_E28, CLEAR);    // 에러 재검사를 위해 감지 상태 CLEAR
                    }
                    else
                    {
                        mu16ErrorRecheckDelay++;
                    }
                }
                else
                {   // 에러 감지가 된 적이 없으면
                    mu16ErrorRecheckDelay = 0;
                }
            }
        }
    }
    else
    {   // 에러가 확정된 경우
        mu8ErrorCheckDelay = 0;
        mu8ErrorDetectTime = 0;
        mu16ErrorRecheckDelay = 0;
    }
#endif
}


/// @brief      EW 살균 모듈 에러 체크
/// @details    EW 살균 모듈 에러 체크 동작을 진행한다
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
    {   // EW가 동작 중이면
        mu16EW_AD = Get_ADC_Data(ADC_ID_EW_I_FB);       // EW 모듈 전류 피드백 확인

        if ( (mu16EW_AD < EW_MODULE_ERROR_UNDER_STANDARD_AD) ||
             (mu16EW_AD > EW_MODULE_ERROR_OVER_STANDARD_AD) )
        {   // 기준값 미만 또는 초과인 경우
            if (SError.gu8Status[ERROR_ID_EW_MODULE_E71] == CLEAR)
            {   // 에러 미발생 상태인 경우
                if (mu8ErrorDetectTime >= EW_MODULE_ERROR_DETECT_TIME)
                {   // 에러 감지 기준 시간 경과시
                    mu8ErrorDetectTime = 0;

                    if (mu8ErrorDetectCount >= EW_MODULE_ERROR_DETECT_COUNT)
                    {   // 반복 감지 기준 횟수 이상인 경우
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
            {   // 에러 발생 상태인 경우
                mu8ErrorDetectTime = 0;
                mu8ErrorDetectCount = 0;
            }
        }
        else
        {   // 정상인 경우
            mu8ErrorDetectTime = 0;
            mu8ErrorDetectCount = 0;

            if (SError.gu8Status[ERROR_ID_EW_MODULE_E71] == SET)
            {   // 에러 발생 상태인 경우
                Set_ErrorStatus(ERROR_ID_EW_MODULE_E71, CLEAR);
            }
        }
    }
    else
    {   // EW가 동작 중이 아닌 경우
        mu8ErrorDetectTime = 0;
        mu8ErrorDetectCount = 0;

        if (mu8Status != OPERATION_ID_EW_STERILIZE)
        {   // 살균 동작 중이 아닌 경우
            if (SError.gu8Status[ERROR_ID_EW_MODULE_E71] == SET)
            {   // 에러 발생 상태인 경우
                Set_ErrorStatus(ERROR_ID_EW_MODULE_E71, CLEAR);
            }
        }
    }
#endif
}


/// @brief      살균 드레인 펌프 동작 에러 체크
/// @details    살균 드레인 펌프 동작 에러 체크 동작을 진행한다
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

    mu8Status = Get_OperatingStatus();                          // 제품 동작 상태 확인
    mu8WaterLevel = Get_WaterLevel();                           // 정수 탱크 수위 확인
    mu8DrainStatus = Get_DC_PumpStatus_HW(DC_PUMP_ID_DRAIN);    // 드레인 펌프 동작 상태 확인
    mu16DrainPumpAD = Get_ADC_Data(ADC_ID_DRAIN_PUMP);          // 드레인 펌프 피드백 AD값 확인

    if (mu8Status == OPERATION_ID_EW_STERILIZE)
    {   // 살균 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_EW_DRAIN_PUMP_E72] == CLEAR)
        {   // 드레인 펌프 에러가 발생하지 않은 경우
            if (mu8DrainStatus == TRUE)
            {   // 드레인 동작 중인 경우
                if (mu8WaterLevel > WATER_LEVEL_LOW)
                {   // 수위가 저수위 넘게 감되고 있는 경우
                    if ( (mu16DrainPumpAD < EW_DRAIN_PUMP_ERROR_UNDER_STANDARD_AD) ||
                         (mu16DrainPumpAD > EW_DRAIN_PUMP_ERROR_OVER_STANDARD_AD) )
                    {   // 기준값 미만 또는 초과인 경우
                        Set_ErrorStatus(ERROR_ID_EW_DRAIN_PUMP_E72, SET);
                    }
                    else
                    {   // 정상 상태인 경우
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
            {   // 드레인 동작 중이 아닌 경우
                mu16ActTime = 0;
            }
        }
        else
        {   // 에러가 발생한 경우
            mu16ActTime = 0;
        }
    }
    else
    {   // 살균 동작 중이 아닌 경우
        mu16ActTime = 0;
    }
#endif
}


/// @brief      UV 동작 에러 체크
/// @details    UV 동작 에러 체크 동작을 진행한다
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

    // UV1 에러 체크
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_WATER_TANK);   // 살균 동작 확인
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // 살균 UV 피드백 AD값 확인

    if (mu8UVStelizeStatus == TRUE)
    {   // UV 살균 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_UV_1_E73] == CLEAR)
        {   // UV1 에러 미발생시
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // 기준값 미만 또는 초과인 경우
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
    {   // UV 살균 동작 중이 아닌 경우
        mu8ErrorDetectTime_UV1 = 0;

        if (SError.gu8Status[ERROR_ID_UV_1_E73] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_1_E73, CLEAR);
        }
    }

    // UV2 에러 체크
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_COLD_TANK_BACK);   // 살균 동작 확인
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // 살균 UV 피드백 AD값 확인

    if (mu8UVStelizeStatus == TRUE)
    {   // UV 살균 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_UV_2_E74] == CLEAR)
        {   // UV1 에러 미발생시
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // 기준값 미만 또는 초과인 경우
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
    {   // UV 살균 동작 중이 아닌 경우
        mu8ErrorDetectTime_UV2 = 0;

        if (SError.gu8Status[ERROR_ID_UV_2_E74] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_2_E74, CLEAR);
        }
    }

    // UV3 에러 체크
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_ICE_TANK);   // 살균 동작 확인
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // 살균 UV 피드백 AD값 확인

    if (mu8UVStelizeStatus == TRUE)
    {   // UV 살균 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_UV_3_E75] == CLEAR)
        {   // UV1 에러 미발생시
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // 기준값 미만 또는 초과인 경우
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
    {   // UV 살균 동작 중이 아닌 경우
        mu8ErrorDetectTime_UV3 = 0;

        if (SError.gu8Status[ERROR_ID_UV_3_E75] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_3_E75, CLEAR);
        }
    }

    // UV4 에러 체크
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_ICE_TRAY);   // 살균 동작 확인
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // 살균 UV 피드백 AD값 확인

    if (mu8UVStelizeStatus == TRUE)
    {   // UV 살균 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_UV_4_E76] == CLEAR)
        {   // UV1 에러 미발생시
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // 기준값 미만 또는 초과인 경우
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
    {   // UV 살균 동작 중이 아닌 경우
        mu8ErrorDetectTime_UV4 = 0;

        if (SError.gu8Status[ERROR_ID_UV_4_E76] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_4_E76, CLEAR);
        }
    }

    // UV5 에러 체크
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_WATER_FAUCET);   // 살균 동작 확인
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // 살균 UV 피드백 AD값 확인

    if (mu8UVStelizeStatus == TRUE)
    {   // UV 살균 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_UV_5_E77] == CLEAR)
        {   // UV1 에러 미발생시
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // 기준값 미만 또는 초과인 경우
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
    {   // UV 살균 동작 중이 아닌 경우
        mu8ErrorDetectTime_UV5 = 0;

        if (SError.gu8Status[ERROR_ID_UV_5_E77] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_5_E77, CLEAR);
        }
    }

    // UV6 에러 체크
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_ICE_FAUCET);   // 살균 동작 확인
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // 살균 UV 피드백 AD값 확인

    if (mu8UVStelizeStatus == TRUE)
    {   // UV 살균 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_UV_6_E78] == CLEAR)
        {   // UV1 에러 미발생시
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // 기준값 미만 또는 초과인 경우
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
    {   // UV 살균 동작 중이 아닌 경우
        mu8ErrorDetectTime_UV6 = 0;

        if (SError.gu8Status[ERROR_ID_UV_6_E78] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_6_E78, CLEAR);
        }
    }

    // UV7 에러 체크
    mu8UVStelizeStatus = Get_UV_SterilizeStatus(STERILIZE_OPERATION_ID_UV_COLD_TANK_FRONT);   // 살균 동작 확인
    mu16UVFeedbackAD = Get_ADC_Data(ADC_ID_UV_FB);      // 살균 UV 피드백 AD값 확인

    if (mu8UVStelizeStatus == TRUE)
    {   // UV 살균 동작 중인 경우
        if (SError.gu8Status[ERROR_ID_UV_7_E79] == CLEAR)
        {   // UV1 에러 미발생시
            if ( (mu16UVFeedbackAD < UV_ERROR_UNDER_STANDARD_AD) ||
                 (mu16UVFeedbackAD > UV_ERROR_OVER_STANDARD_AD) )
            {   // 기준값 미만 또는 초과인 경우
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
    {   // UV 살균 동작 중이 아닌 경우
        mu8ErrorDetectTime_UV7 = 0;

        if (SError.gu8Status[ERROR_ID_UV_7_E79] == SET)
        {
            Set_ErrorStatus(ERROR_ID_UV_7_E79, CLEAR);
        }
    }

#endif
}


/// @brief      TDS 입수 온도 센서 에러 체크
/// @details    TDS 입수 온도 센서 에러 체크 동작을 진행한다
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
        {   // 기준값 미만 또는 초과인 경우
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


/// @brief      TDS 출수 온도 센서 에러 체크
/// @details    TDS 출수 온도 센서 에러 체크 동작을 진행한다
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
        {   // 기준값 미만 또는 초과인 경우
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


/// @brief      TDS 입수 온도 센서 에러 체크
/// @details    TDS 입수 온도 센서 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void TDS_InErrorCheck(void)
{
#if (TDS_IN_ERROR_CHECK_USE == USE)
    U8 mu8TDSInCheckStatus = 0;
    U16 mu16TDSInValue = 0;

    static U8 mu8ErrorDetectTime = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8TDSInCheckStatus = Get_TDS_CheckStatus();        // TDS 센싱 상태인지 확인
    mu16TDSInValue = Get_TDS_In_Value();                // TDS In 센싱값 확인

    if (SError.gu8Status[ERROR_ID_TDS_IN_E92] == CLEAR)
    {   // 에러 발생이 없는 경우
        if (mu8TDSInCheckStatus == TRUE)
        {   // TDS 센싱 중인 경우
            if (mu16TDSInValue == 0)
            {   // TDS 측정값이 0인 경우
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
            {   // 0이 아닌 경우
                mu8ErrorDetectTime = 0;
                mu8ErrorDetectCount = 0;

                if (SError.gu8Status[ERROR_ID_TDS_IN_E92] == SET)
                {
                    Set_EEPROM_Data(ERROR_ID_TDS_IN_E92, CLEAR);
                }
            }
        }
        else
        {   // TDS 센싱 중이 아닌 경우
            mu8ErrorDetectTime = 0;
            mu8ErrorDetectCount = 0;
        }
    }
    else
    {   // 에러 발생인 경우
        mu8ErrorDetectTime = 0;
        mu8ErrorDetectCount = 0;
    }
#endif
}


/// @brief      TDS 출수 온도 센서 에러 체크
/// @details    TDS 출수 온도 센서 에러 체크 동작을 진행한다
/// @param      void
/// @return     void
void TDS_OutErrorCheck(void)
{
#if (TDS_OUT_ERROR_CHECK_USE == USE)
    U8 mu8TDSOutCheckStatus = 0;
    U16 mu16TDSOutValue = 0;

    static U8 mu8ErrorDetectTime = 0;
    static U8 mu8ErrorDetectCount = 0;

    mu8TDSOutCheckStatus = Get_TDS_CheckStatus();        // TDS 센싱 상태인지 확인
    mu16TDSOutValue = Get_TDS_Out_Value();               // TDS Out 센싱값 확인

    if (SError.gu8Status[ERROR_ID_TDS_OUT_E93] == CLEAR)
    {   // 에러 발생이 없는 경우
        if (mu8TDSOutCheckStatus == TRUE)
        {   // TDS 센싱 중인 경우
            if (mu16TDSOutValue == 0)
            {   // TDS 측정값이 0인 경우
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
            {   // 0이 아닌 경우
                mu8ErrorDetectTime = 0;
                mu8ErrorDetectCount = 0;

                if (SError.gu8Status[ERROR_ID_TDS_OUT_E93] == SET)
                {
                    Set_EEPROM_Data(ERROR_ID_TDS_OUT_E93, CLEAR);
                }
            }
        }
        else
        {   // TDS 센싱 중이 아닌 경우
            mu8ErrorDetectTime = 0;
            mu8ErrorDetectCount = 0;
        }
    }
    else
    {   // 에러 발생인 경우
        mu8ErrorDetectTime = 0;
        mu8ErrorDetectCount = 0;
    }
#endif
}


/// @brief      입수 오염 에러 체크
/// @details    입수 오염 에러 체크 동작을 진행한다
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

    mu8TDSInCheckStatus = Get_TDS_CheckStatus();         // TDS 센싱 상태인지 확인
    mu16TDSInValue = Get_TDS_In_Value();                // TDS Out 센싱값 확인

    if (SError.gu8Status[ERROR_ID_INPUT_CONTAMINATION_E98] == CLEAR)
    {   // 에러 발생이 없는 경우
        if (mu8TDSInCheckStatus == TRUE)
        {   // TDS 센싱 중인 경우
            if (mu8TDSInCheckEnd == CLEAR)
            {   // 한번 체크가 완료되지 않은 경우
                if (mu8ErrorCheckDelay >= INPUT_CONTAMINATION_ERROR_DETECT_DELAY_TIME)
                {   // 체크 전 지연시간 경과 후
                    mu8ErrorCheckDelay = 0;

                    mu8TDSInCheckEnd = SET;

                    if (mu16TDSInValue >= 500)
                    {   // TDS 측정값이 500ppm 이상인 경우
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
                    {   // TDS 측정값이 500ppm 미만인 경우
                        mu8ErrorDetectCount = 0;
                    }
                }
                else
                {
                    mu8ErrorCheckDelay++;
                }
            }
            else
            {   // 한번 체크가 완료된 경우
                mu8ErrorCheckDelay = 0;
            }
        }
        else
        {   // TDS 센싱 중이 아닌 경우
            mu8TDSInCheckEnd = CLEAR;
            mu8ErrorCheckDelay = 0;
        }
    }
    else
    {   // 에러 발생인 경우
        mu8TDSInCheckEnd = CLEAR;
        mu8ErrorCheckDelay = 0;
        mu8ErrorDetectCount = 0;
    }
#endif
}


/// @brief      제거율 불량 에러 체크
/// @details    제거율 불량 에러 체크 동작을 진행한다
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

    mu8TDSCheckStatus = Get_TDS_CheckStatus();          // TDS 센싱 상태인지 확인
    mu16TDSInValue = Get_TDS_In_Value();                // TDS Out 센싱값 확인
    mu16TDSOutValue = Get_TDS_Out_Value();              // TDS Out 센싱값 확인

    if (SError.gu8Status[ERROR_ID_DECONTAMINATION_E99] == CLEAR)
    {   // 에러 발생이 없는 경우
        if (mu8TDSCheckStatus == TRUE)
        {   // TDS 센싱 중인 경우
            if (mu8TDSCheckEnd == CLEAR)
            {   // 한번 체크가 완료되지 않은 경우
                if (mu8ErrorCheckDelay >= DECONTAMINATION_ERROR_DETECT_DELAY_TIME)
                {   // 체크 전 지연시간 경과 후
                    mu8ErrorCheckDelay = 0;

                    mu8TDSCheckEnd = SET;

                    // 제거율 70%이상  =>  (TDS IN * 30% >= TDS OUT)  =>  ex) TDS IN이 100이면 OUT이 30이하여야 OK
                    mu16TDSInValueBuf = (mu16TDSInValue / 10) * 3;   // TDS IN의 30%값 계산

                    if (mu16TDSOutValue > DECONTAMINATION_ERROR_CHECK_STANDARD_OUT_PPM)
                    {   // TDS OUT이 15ppm 초과인 경우 에러 체크함
                        if (mu16TDSInValueBuf >= mu16TDSOutValue)
                        {   // 제거율이 30%이하가 될 때(정상)
                            mu8ErrorCheckDelay = 0;
                            mu8ErrorDetectCount = 0;
                        }
                        else
                        {   //  제거율이 30%초과가 될 때(이상)
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
                    {   // TDS OUT이 15ppm 이하인 경우 에러 체크 안함
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
        {   // TDS 센싱 중이 아닌 경우
            mu8TDSCheckEnd = 0;
            mu8ErrorCheckDelay = 0;
        }
    }
    else
    {   // 에러 발생인 경우
        mu8TDSCheckEnd = CLEAR;
        mu8ErrorCheckDelay = 0;
        mu8ErrorDetectCount = 0;
    }
#endif
}


/// @brief      디스플레이 되어야 하는 에러 코드 확인 함수
/// @details    우선 순위에 따라 현재 표시되어야 하는 에러에 해당하는 디스플레이 표시 Code를 반환한다
/// @param      void
/// @return     return : 에러 표시 코드 - 코드 리스트 내용 참조
U8 Get_ErrorDisplayCode(void)
{
    U8 mu8i = 0;
    U8 mu8ReturnData = 0;

    for (mu8i = 0 ; mu8i < ERROR_ID_MAX ; mu8i++)
    {
        if (SError.gu8Status[mu8i] == SET)
        {   // 우선순위에 따라 표시할 에러가 있는 경우
            mu8ReturnData  = gu8ErrorDisplayCode[mu8i];
            break;  // 확인 정지
        }
    }

    return  mu8ReturnData;
}


/// @brief      에러가 하나라도 발생된 것이 있는지 체크하는 함수
/// @details    체크하는 에러들 중 어떤 에러라도 발생한 것이 있는지 여부를 확인하여 반환한다
/// @param      void
/// @return     return : 에러 발생 여부 - 0(미발생), 1(발생)
U8 Get_ErrorOccuredCheck(void)
{
    U8 mu8i = 0;
    U8 mu8ReturnData = 0;

    for (mu8i = 0 ; mu8i < ERROR_ID_MAX ; mu8i++)
    {
        if (SError.gu8Status[mu8i] == SET)
        {   // 발생한 에러가 있는 경우
            mu8ReturnData  = TRUE;
            break;  // 확인 정지
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



/// @brief      에러 발생 상태 설정 함수
/// @details    ID에 해당하는 에러를 설정한다
/// @param      mu8ID : 에러 ID
///             mu8Status : 에러 발생 여부 - 0(CLEAR, 미발생), 1(SET, 발생)
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


/// @brief      에러 발생 상태 확인 함수
/// @details    ID에 해당하는 에러가 발생하였는지 확인하여 그 상태를 반환한다
/// @param      mu8ID : 에러 ID
/// @return     return : 에러 발생 여부 - 0(FALSE, 미발생), 1(TRUE, 발생)
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


/// @brief      에러 감지 상태 설정 함수
/// @details    ID에 해당하는 에러 감지를 설정한다
/// @param      mu8ID : 에러 ID
///             mu8Status : 에러 감지 여부 - 0(CLEAR, 미발생), 1(SET, 발생)
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


/// @brief      에러 상태 감지 확인 함수
/// @details    ID에 해당하는 에러가 감지(반드시 에러 발생은 아님)되었는지 확인하여 그 상태를 반환한다
/// @param      mu8ID : 에러 ID
/// @return     return : 에러 감지 여부 - 0(FALSE, 미감지), 1(TRUE, 감지)
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


/// @brief      입수 동작 관련 에러 발생 상태 확인 함수
/// @details    입수 동작과 관련된 에러가 발생한 것이 있는지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 에러 발생 여부 - 0(FALSE, 미발생), 1(TRUE, 발생)
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


/// @brief      외기 온도 센서 에러 발생 상태 확인 함수
/// @details    외기 온도 센서 에러가 발생한 것이 있는지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 에러 발생 여부 - 0(FALSE, 미발생), 1(TRUE, 발생)
U8 Get_AirTempError(void)
{
    if ( (SError.gu8Status[ERROR_ID_AIR_TEMP_1_E43] == SET) ||
         (SError.gu8Status[ERROR_ID_AIR_TEMP_2_E53] == SET) )
    {   // 외기1 또는 외기2 온도 센서 에러가 발생한 경우
        return  TRUE;
    }

    return  FALSE;
}


/// @brief      온수 관련 에러 발생 상태 확인 함수
/// @details    온수 관련 에러가 발생한 것이 있는지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 에러 발생 여부 - 0(FALSE, 미발생), 1(TRUE, 발생)
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


/// @brief      냉수 관련 에러 발생 상태 확인 함수
/// @details    냉수 관련 에러가 발생한 것이 있는지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 에러 발생 여부 - 0(FALSE, 미발생), 1(TRUE, 발생)
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


/// @brief      제빙 관련 에러 발생 상태 확인 함수
/// @details    제빙 동작 관련 에러가 발생한 것이 있는지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 에러 발생 여부 - 0(FALSE, 미발생), 1(TRUE, 발생)
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
/// @details    정수기 에러 체크 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_WaterError_Module_Initialize(void)
{
    Lib_WaterErrorCheck_Initialize();
}


/// @brief      Lib Water Error Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 정수기 에러 체크 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_WaterError_Module_1ms_Control(void)
{
    WaterErrorCheckTimer();
}


/// @brief      Lib Water Error Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 정수기 에러 체크 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_WaterError_Module_Control(void)
{
    Lib_WaterErrorCheck();
}


