/// @file     Lib_Water_Cool_TEM.c
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    열전모듈 냉각 동작 제어 file

#include "Global_Header.h"


#if (TEM_USE == USE)                // 열전모듈 냉각 사용시


/// @brief  TEM PWM 제어 TDR 레지스터 설정값 테이블
__far const U16 gau16TEM_TDR_DataTable[TEM_V_ID_MAX] =
{
    32000,  32000,  32000,  28500,  27500,  26500,  25600,  24800,  24000,  23200,      // 0V ~ 9V
    22000,  21500,  20600,  19800,  18800,  17800,  16700,  15500,  14200,  12600,      // 10V ~ 19V
    10800,  8800,   6200,   3200,   0                                                   // 20V ~ 24V
};

U8 gu8TemControlPeriodTimer = 0;            /// @brief  TEM 제어를 위한 주기 타이머 카운터

U8 gu8ColdOnOff = 0;                        /// @brief  냉수 기능 On/Off 상태
U8 gu8ColdStrong = 0;                       /// @brief  냉수 기능 강약 상태

U8 gu8TEMControlMode = 0;                   /// @brief  TEM 제어 모드

U16 gu16TEM_RapidModeTime = 0;              /// @brief  TEM 급속 모드 가동 시간
U8 gu8TEM_RapidModeConfirmTimer = 0;        /// @brief  TEM 급속 모드 전환 확정 타이머
U8 gu8TEM_RetainModeConfirmTimer = 0;       /// @brief  TEM 유지 모드 전환 확정 타이머
U8 gu8TEM_OFFModeConfirmTimer = 0;          /// @brief  TEM 동작 종료 전환 확정 타이머
U8 gu8TEM_ECOModeConfirmTimer = 0;          /// @brief  TEM ECO 모드 전환 확정 타이머

U8 gu8TEM_TimeCoolingRapidCheck = 0;        /// @brief  TEM 급속 모드에서 추가 가동 시간 체크 시작 상태 - 0(미체크중), 1(체크중)
U16 gu16TEM_TimerCoolingRapid = 0;          /// @brief  TEM 급속 모드에서 추가 가동 시간

U16 gu16TEM_OnDelayRapidOff = 0;            /// @brief  TEM 급속 모드 6시간 기동으로 TEM OFF된 경우 TEM 재기동 전 대기 시간

U8 gu8TEM_HeatsinkTempCheckTime = 0;        /// @brief  TEM Heatsink의 온도가 80도를 넘는 시간 체크 타이머

U16 gu16TimerRapidInCon12V = 0;             /// @brief  냉각 급속 모드로 진입시 초기 전압 12V 인가 시간



/// @brief      TEM 냉각 제어 초기화
/// @details    TEM 냉각 제어 변수들을 모두 초기화 한다
/// @param      void
/// @return     void
void Lib_TEM_Initialize(void)
{
    gu8TemControlPeriodTimer = 0;

    gu8ColdOnOff = ON;                      // 냉수 기능 ON
    gu8ColdStrong = SET;                    // 강모드

    gu8TEMControlMode = TEM_MODE_ID_OFF;    // TEM 제어 모드 OFF

    gu16TEM_RapidModeTime = 0;
    gu8TEM_RapidModeConfirmTimer = 0;
    gu8TEM_RetainModeConfirmTimer = 0;
    gu8TEM_OFFModeConfirmTimer = 0;
    gu8TEM_ECOModeConfirmTimer = 0;

    gu8TEM_TimeCoolingRapidCheck = 0;
    gu16TEM_TimerCoolingRapid = 0;

    gu16TEM_OnDelayRapidOff = 0;

    gu8TEM_HeatsinkTempCheckTime = 0;

    gu16TimerRapidInCon12V = 0;
}


/// @brief      TEM 제어 시간 타이머(@1ms)
/// @details    1ms 인터럽트 안에서 TEM 제어 시간 타이머를 카운트 한다
/// @param      void
/// @return     void
void Lib_TEM_ControlTimer(void)
{
    if (gu8TemControlPeriodTimer < LIB_TEM_CONTROL_TIME_PERIOD)
    {
        gu8TemControlPeriodTimer++;
    }
}


/// @brief      냉수 기능 ON/OFF 상태 설정 함수
/// @details    냉수 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8Status : 냉수 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_ColdFuncStatus(U8 mu8Status)
{
    gu8ColdOnOff = mu8Status;
}


/// @brief      냉수 기능 ON/OFF 상태 확인 함수
/// @details    냉수 기능 상태가 ON인지 OFF 인지 확인하여 그 값을 반환한다
/// @param      void
/// @return     return : 냉수 기능 설정 상태 - 0(OFF), 1(ON)
U8 Get_ColdFuncStatus(void)
{
    return  gu8ColdOnOff;
}


/// @brief      냉수 기능 강약 상태 설정 함수
/// @details    냉수 기능 상태가 강인지 약인지 설정한다
/// @param      mu8Status : 냉수 기능 강약 설정 상태 - 0(약), 1(강)
/// @return     void
void Set_ColdStorng(U8 mu8Status)
{
    gu8ColdStrong = mu8Status;
}


/// @brief      냉수 기능 강약 상태 확인 함수
/// @details    냉수 기능 상태가 강인지 약 인지 확인하여 그 값을 반환한다
/// @param      void
/// @return     return : 냉수 기능 설정 상태 - 0(약), 1(강)
U8 Get_ColdStrong(void)
{
    return  gu8ColdStrong;
}


/// @brief      냉수 기능 TEM 제어모드 상태 확인 함수
/// @details    냉수 기능 TEM 제어모드 상태를 확인하여 그 값을 반환한다
/// @param      void
/// @return     return : TEM 제어모드 상태 - 제어모드는 enum으로 선언된 모드 리스트 참조
U8 Get_TEM_ActMode(void)
{
    return  gu8TEMControlMode;
}


/// @brief      TEM 제어
/// @details    TEM 제어 동작을 한다
/// @param      void
/// @return     void
void Lib_TEM_Control(void)
{
    static U8 mu8TemConTimerSec = 0;

    U8 mu8ColdOnOff = 0;
    U8 mu8ColdStrong = 0;
    F32 mf32AirTemp = 0;
    U8 mu8AirError = 0;
    F32 mf32ColdTemp = 0;
    U8 mu8ColdError = 0;
    U8 mu8TEM_VoltageBuf = 0;
    U16 mu16TEM_TDR_DataBuff = 0;
    U8 mu8LeakError = 0;
    F32 mf32HeatsinkTemp = 0;

    mu8TEM_VoltageBuf = Get_TEM_Voltage();

    if (gu8TemControlPeriodTimer >= LIB_TEM_CONTROL_TIME_PERIOD)
    {   // 100ms 마다
        gu8TemControlPeriodTimer = 0;

        mf32HeatsinkTemp = Get_Temp(ADC_ID_TH_HEATSINK);   // TEM 방열판 온도 센서 온도값 확인
        if (mf32HeatsinkTemp > 80.0f)
        {   // TEM Heatsink가 80도가 넘으면
            if (gu8TEM_HeatsinkTempCheckTime)
            {
                gu8TEM_HeatsinkTempCheckTime--;
            }
        }
        else
        {   // 80이하면
            gu8TEM_HeatsinkTempCheckTime = 10;      // 80도 초과 체크 1sec
        }

        if (gu8TEMControlMode != TEM_MODE_ID_RAPID)
        {
            gu16TEM_RapidModeTime = TEM_RAPID_MODE_MAX_TIME;
        }

        if (++mu8TemConTimerSec >= 10)
        {   // 1초 마다 계산해야 할 변수들 처리
            mu8TemConTimerSec = 0;

            if (gu16TEM_OnDelayRapidOff)
            {   // 급속모드에서 종료되어 재기동 전 지연시간이 있는 경우
                gu16TEM_OnDelayRapidOff--;
            }

            if (gu8TEMControlMode == TEM_MODE_ID_RAPID)
            {
                if (gu16TEM_RapidModeTime)
                {   // 급속모드 동작 시간 체크
                    gu16TEM_RapidModeTime--;
                }
            }

            if (gu8TEM_TimeCoolingRapidCheck)
            {   // 급속모드 추가 기동 시간 체크 중
                if (gu16TEM_TimerCoolingRapid)
                {
                    gu16TEM_TimerCoolingRapid--;
                }
            }
        }

        mu8ColdOnOff = Get_ColdOnOff();                             // 냉수 기능 ON/OFF 상태 확인
        mu8ColdStrong = Get_ColdStrong();                           // 냉수 기능 강약 상태 확인
        mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);                  // 외기 온도 확인
        mu8AirError = Get_ErrorStatus(ERROR_ID_AIR_TEMP_1_E43);         // 외기 온도 센서 에러 상태 확인
        mf32ColdTemp = Get_Temp(ADC_ID_TH_COOL);              // 냉수 온도 확인
        mu8ColdError = Get_ErrorStatus(ADC_ID_TH_COOL);       // 냉수 온도 센서 에러 상태 확인
        mu8LeakError = Get_ErrorStatus(ERROR_ID_LEAKAGE_E01);           // 누수 에러 상태 확인

        if ( (mu8ColdOnOff == TRUE) && (mu8ColdError == FALSE) )
        {   // 냉수 기능 ON시
            // TEM 동작 모드 판단 ===========================================================================================================
            /***************냉수 강모드*******************/
            if (mu8ColdStrong == TRUE)
            {   // 강모드 동작인 경우
                switch (gu8TEMControlMode)
                {
                    case TEM_MODE_ID_OFF:       // TEM OFF 모드
                        gu8TEM_OFFModeConfirmTimer = 0;

                        if (gu16TEM_OnDelayRapidOff == 0)
                        {   // 급속모드 최대 동작시간으로 OFF시 발생하는 재동작 전 지연시간이 없는 경우 시작 판단
                            // 급속 모드 시작 판단
                            if (mf32ColdTemp > RAPID_OPERATE_DEGREE)
                            {   // 급속 모드 전환 판단 온도 기준을 만족하면
                                if (gu8TEM_RapidModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                                {   // 확정 시간만큼 경과 후 모드 전환
                                    gu8TEM_OFFModeConfirmTimer = 0;
                                    gu8TEM_RapidModeConfirmTimer = 0;           // 급속 모드 전환시 급속 모드 전환 체크 시간 초기화
                                    gu8TEM_RetainModeConfirmTimer = 0;          // 급속 모드 전환시 유지 모드 전환 체크 시간 초기화
                                    gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                    gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                    gu8TEMControlMode = TEM_MODE_ID_RAPID;      // 급속 모드 동작
                                }
                                else
                                {
                                    gu8TEM_RapidModeConfirmTimer++;
                                }
                            }
                            else
                            {   // 급속 모드 전환 판단 온도 기준을 만족하지 않으면
                                gu8TEM_RapidModeConfirmTimer = 0;

                                // 유지 모드 시작 판단
                                if (mf32ColdTemp >= COLD_REOPERATE_DEGREE)
                                {   // 유지 모드 재시작 온도 기준을 만족하면
                                    if (gu8TEM_RetainModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                                    {   // 확정 시간만큼 경과 후 모드 전환
                                        gu8TEM_OFFModeConfirmTimer = 0;
                                        gu8TEM_RapidModeConfirmTimer = 0;
                                        gu8TEM_RetainModeConfirmTimer = 0;
                                        gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                        gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                        gu8TEMControlMode = TEM_MODE_ID_RETAIN;      // 유지 모드 동작
                                    }
                                    else
                                    {
                                        gu8TEM_RetainModeConfirmTimer++;
                                    }
                                }
                                else
                                {
                                    gu8TEM_RetainModeConfirmTimer = 0;
                                }
                            }
                        }
                        break;

                    case TEM_MODE_ID_RAPID:     // 급속 모드
                        gu8TEM_RapidModeConfirmTimer = 0;

                        // 급속 모드 종료 온도 조건으로 유지 모드 변경 판단
                        if (mf32ColdTemp < RAPID_COMPLETE_DEGREE)
                        {   // 급속 모드 종료 온도 조건 및 추가 동작 시간으로 유지 모드 전환 판단
                            if (gu8TEM_TimeCoolingRapidCheck == CLEAR)
                            {   // 급속 모드 추가 동작 시간이 없었던 경우 시간 세팅
                                gu8TEM_TimeCoolingRapidCheck = SET;

                                if (mu8AirError == FALSE)
                                {   // 외기 온도 센서 에러 발생시
                                    gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_3_TIME;
                                }
                                else
                                {   // 외기 온도 센서 에러 미발생시
                                    if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_1_TEMP)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_1_TIME;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_2_TEMP)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_2_TIME;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_3_TEMP)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_3_TIME;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_4_TEMP)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_4_TIME;
                                    }
                                    else
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_5_TIME;
                                    }
                                }
                            }
                            else
                            {   // 급속 모드 추가 동작을 시작한 이후
                                if (gu16TEM_TimerCoolingRapid == 0)
                                {   // 추가 냉각 시간 완료시
                                    gu8TEM_OFFModeConfirmTimer = 0;
                                    gu8TEM_RapidModeConfirmTimer = 0;
                                    gu8TEM_RetainModeConfirmTimer = 0;
                                    gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                    gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                    gu8TEMControlMode = TEM_MODE_ID_RETAIN;     // 유지 모드 동작
                                }
                            }
                        }

                        // 유지 모드 전환 냉수 온도 조건으로 유지 모드로 전환 판단
                        if (mf32ColdTemp < RETAIN_JOIN_DEGREE)
                        {   // 유지 모드 전환 판단 기준을 만족하면
                            if (gu8TEM_RetainModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // 확정 시간만큼 경과 후
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                gu8TEMControlMode = TEM_MODE_ID_RETAIN;     // 유지 모드 동작
                            }
                            else
                            {
                                gu8TEM_RetainModeConfirmTimer++;
                            }
                        }
                        else
                        {   // 유지 모드 전환 판단 기준을 만족하지 않으면
                            gu8TEM_RetainModeConfirmTimer = 0;
                        }

                        // 급속 모드에서 냉각 종료 판단
                        if ( (mf32ColdTemp <= COLD_COMPLETE_DEGREE) ||
                             (mu8ColdError == TRUE) ||
                             ( (mf32ColdTemp < 6.0f) && (mf32AirTemp < 6.0f) ) ||
                             (mu8LeakError == TRUE) ||
                             (gu8TEM_HeatsinkTempCheckTime == 0) ||
                             (gu16TEM_RapidModeTime == 0) )
                        {   // 냉각 종료 조건인 경우
                            if (gu8TEM_OFFModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // 확정 시간만큼 경과 후
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                gu8TEMControlMode = TEM_MODE_ID_OFF;

                                if (gu16TEM_RapidModeTime == 0)
                                {   // 급속모드 최대 동작시간으로 TEM OFF되는 경우
                                    gu16TEM_OnDelayRapidOff = TEM_RAPID_OFF_DELAY_TIME;     // 급속모드에서 종료시 재가동 전 지연 시간 설정
                                }
                            }
                            else
                            {
                                gu8TEM_OFFModeConfirmTimer++;
                            }
                        }
                        else
                        {
                            gu8TEM_OFFModeConfirmTimer = 0;
                        }
                        break;

                    case TEM_MODE_ID_RETAIN:        // 유지 모드
                        gu8TEM_RetainModeConfirmTimer = 0;

                        // 급속 모드 시작 판단
                        if (mf32ColdTemp > RAPID_OPERATE_DEGREE)
                        {   // 급속 모드 전환 판단 온도 기준을 만족하면
                            if (gu8TEM_RapidModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // 확정 시간만큼 경과 후 모드 전환
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;           // 급속 모드 전환시 급속 모드 전환 체크 시간 초기화
                                gu8TEM_RetainModeConfirmTimer = 0;          // 급속 모드 전환시 유지 모드 전환 체크 시간 초기화
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                gu8TEMControlMode = TEM_MODE_ID_RAPID;      // 급속 모드 동작
                            }
                            else
                            {
                                gu8TEM_RapidModeConfirmTimer++;
                            }
                        }
                        else
                        {   // 급속 모드 전환 판단 온도 기준을 만족하지 않으면
                            gu8TEM_RapidModeConfirmTimer = 0;
                        }

                        // 유지 모드에서 냉각 종료 판단
                        if ( (mf32ColdTemp <= COLD_COMPLETE_DEGREE) ||
                             (mu8ColdError == TRUE) ||
                             ( (mf32ColdTemp < 6.0f) && (mf32AirTemp < 6.0f) ) ||
                             (mu8LeakError == TRUE) ||
                             (gu8TEM_HeatsinkTempCheckTime == 0) )
                        {   // 냉각 종료 조건인 경우
                            if (gu8TEM_OFFModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // 확정 시간만큼 경과 후
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                gu8TEMControlMode = TEM_MODE_ID_OFF;
                            }
                            else
                            {
                                gu8TEM_OFFModeConfirmTimer++;
                            }
                        }
                        else
                        {
                            gu8TEM_OFFModeConfirmTimer = 0;
                        }
                        break;

                    case TEM_MODE_ID_MIN:           // 최소 모드
                        break;

                    case TEM_MODE_ID_ECO:           // ECO 모드
                        break;

                    case TEM_MODE_ID_ECO_9V:        // ECO 9V 제어 모드
                        break;

                    default:
                        gu8TEM_OFFModeConfirmTimer = 0;
                        gu8TEM_RapidModeConfirmTimer = 0;
                        gu8TEM_RetainModeConfirmTimer = 0;
                        gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                        gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                        gu8TEMControlMode = TEM_MODE_ID_OFF;
                        break;
                }
            }
            else    /***************냉수 약모드*******************/
            {   // 약모드 동작인 경우
                switch (gu8TEMControlMode)
                {
                    case TEM_MODE_ID_OFF:       // TEM OFF 모드
                        gu8TEM_OFFModeConfirmTimer = 0;

                        if (gu16TEM_OnDelayRapidOff == 0)
                        {   // 급속모드 최대 동작시간으로 OFF시 발생하는 재동작 전 지연시간이 없는 경우 시작 판단
                            // 급속 모드 시작 판단
                            if (mf32ColdTemp > RAPID_OPERATE_DEGREE_LOW_MOE)
                            {   // 급속 모드 전환 판단 온도 기준을 만족하면
                                if (gu8TEM_RapidModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                                {   // 확정 시간만큼 경과 후 모드 전환
                                    gu8TEM_OFFModeConfirmTimer = 0;
                                    gu8TEM_RapidModeConfirmTimer = 0;           // 급속 모드 전환시 급속 모드 전환 체크 시간 초기화
                                    gu8TEM_RetainModeConfirmTimer = 0;          // 급속 모드 전환시 유지 모드 전환 체크 시간 초기화
                                    gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                    gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                    gu8TEMControlMode = TEM_MODE_ID_RAPID;      // 급속 모드 동작
                                }
                                else
                                {
                                    gu8TEM_RapidModeConfirmTimer++;
                                }
                            }
                            else
                            {   // 급속 모드 전환 판단 온도 기준을 만족하지 않으면
                                gu8TEM_RapidModeConfirmTimer = 0;

                                // 유지 모드 시작 판단
                                if (mf32ColdTemp >= COLD_REOPERATE_DEGREE_LOW_MODE)
                                {   // 유지 모드 재시작 온도 기준을 만족하면
                                    if (gu8TEM_RetainModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                                    {   // 확정 시간만큼 경과 후 모드 전환
                                        gu8TEM_OFFModeConfirmTimer = 0;
                                        gu8TEM_RapidModeConfirmTimer = 0;
                                        gu8TEM_RetainModeConfirmTimer = 0;
                                        gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                        gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                        gu8TEMControlMode = TEM_MODE_ID_RETAIN;      // 유지 모드 동작
                                    }
                                    else
                                    {
                                        gu8TEM_RetainModeConfirmTimer++;
                                    }
                                }
                                else
                                {
                                    gu8TEM_RetainModeConfirmTimer = 0;
                                }
                            }
                        }
                        break;

                    case TEM_MODE_ID_RAPID:     // 급속 모드
                        gu8TEM_RapidModeConfirmTimer = 0;

                        // 급속 모드 종료 온도 조건으로 유지 모드 변경 판단
                        if (mf32ColdTemp < RAPID_COMPLETE_DEGREE_LOW_MODE)
                        {   // 급속 모드 종료 온도 조건 및 추가 동작 시간으로 유지 모드 전환 판단
                            if (gu8TEM_TimeCoolingRapidCheck == CLEAR)
                            {   // 급속 모드 추가 동작 시간이 없었던 경우 시간 세팅
                                gu8TEM_TimeCoolingRapidCheck = SET;

                                if (mu8AirError == FALSE)
                                {   // 외기 온도 센서 에러 발생시
                                    gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_3_TIME_LOW_MODE;
                                }
                                else
                                {   // 외기 온도 센서 에러 미발생시
                                    if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_1_TEMP_LOW_MODE)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_1_TIME_LOW_MODE;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_2_TEMP_LOW_MODE)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_2_TIME_LOW_MODE;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_3_TEMP_LOW_MODE)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_3_TIME_LOW_MODE;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_4_TEMP_LOW_MODE)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_4_TIME_LOW_MODE;
                                    }
                                    else
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_5_TIME_LOW_MODE;
                                    }
                                }
                            }
                            else
                            {   // 급속 모드 추가 동작을 시작한 이후
                                if (gu16TEM_TimerCoolingRapid == 0)
                                {   // 추가 냉각 시간 완료시
                                    gu8TEM_OFFModeConfirmTimer = 0;
                                    gu8TEM_RapidModeConfirmTimer = 0;
                                    gu8TEM_RetainModeConfirmTimer = 0;
                                    gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                    gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                    gu8TEMControlMode = TEM_MODE_ID_RETAIN;     // 유지 모드 동작
                                }
                            }
                        }

                        // 유지 모드 전환 냉수 온도 조건으로 유지 모드로 전환 판단
                        if (mf32ColdTemp < RETAIN_JOIN_DEGREE_LOW_MODE)
                        {   // 유지 모드 전환 판단 기준을 만족하면
                            if (gu8TEM_RetainModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // 확정 시간만큼 경과 후
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                gu8TEMControlMode = TEM_MODE_ID_RETAIN;     // 유지 모드 동작
                            }
                            else
                            {
                                gu8TEM_RetainModeConfirmTimer++;
                            }
                        }
                        else
                        {   // 유지 모드 전환 판단 기준을 만족하지 않으면
                            gu8TEM_RetainModeConfirmTimer = 0;
                        }

                        // 급속 모드에서 냉각 종료 판단
                        if ( (mf32ColdTemp <= COLD_COMPLETE_DEGREE_LOW_MODE) ||
                             (mu8ColdError == TRUE) ||
                             ( (mf32ColdTemp < 7.0f) && (mf32AirTemp < 6.0f) ) ||
                             (mu8LeakError == TRUE) ||
                             (gu8TEM_HeatsinkTempCheckTime == 0) ||
                             (gu16TEM_RapidModeTime == 0) )
                        {   // 냉각 종료 조건인 경우
                            if (gu8TEM_OFFModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // 확정 시간만큼 경과 후
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                gu8TEMControlMode = TEM_MODE_ID_OFF;

                                if (gu16TEM_RapidModeTime == 0)
                                {   // 급속모드 최대 동작시간으로 TEM OFF되는 경우
                                    gu16TEM_OnDelayRapidOff = TEM_RAPID_OFF_DELAY_TIME;     // 급속모드에서 종료시 재가동 전 지연 시간 설정
                                }
                            }
                            else
                            {
                                gu8TEM_OFFModeConfirmTimer++;
                            }
                        }
                        else
                        {
                            gu8TEM_OFFModeConfirmTimer = 0;
                        }
                        break;

                    case TEM_MODE_ID_RETAIN:        // 유지 모드
                        gu8TEM_RetainModeConfirmTimer = 0;

                        // 급속 모드 시작 판단
                        if (mf32ColdTemp > RAPID_OPERATE_DEGREE_LOW_MOE)
                        {   // 급속 모드 전환 판단 온도 기준을 만족하면
                            if (gu8TEM_RapidModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // 확정 시간만큼 경과 후 모드 전환
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;           // 급속 모드 전환시 급속 모드 전환 체크 시간 초기화
                                gu8TEM_RetainModeConfirmTimer = 0;          // 급속 모드 전환시 유지 모드 전환 체크 시간 초기화
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                gu8TEMControlMode = TEM_MODE_ID_RAPID;      // 급속 모드 동작
                            }
                            else
                            {
                                gu8TEM_RapidModeConfirmTimer++;
                            }
                        }
                        else
                        {   // 급속 모드 전환 판단 온도 기준을 만족하지 않으면
                            gu8TEM_RapidModeConfirmTimer = 0;
                        }

                        // 유지 모드에서 냉각 종료 판단
                        if ( (mf32ColdTemp <= COLD_COMPLETE_DEGREE_LOW_MODE) ||
                             (mu8ColdError == TRUE) ||
                             ( (mf32ColdTemp < 7.0f) && (mf32AirTemp < 6.0f) ) ||
                             (mu8LeakError == TRUE) ||
                             (gu8TEM_HeatsinkTempCheckTime == 0) )
                        {   // 냉각 종료 조건인 경우
                            if (gu8TEM_OFFModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // 확정 시간만큼 경과 후
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
                                gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
                                gu8TEMControlMode = TEM_MODE_ID_OFF;
                            }
                            else
                            {
                                gu8TEM_OFFModeConfirmTimer++;
                            }
                        }
                        else
                        {
                            gu8TEM_OFFModeConfirmTimer = 0;
                        }
                        break;

                    case TEM_MODE_ID_MIN:           // 최소 모드
                        break;

                    case TEM_MODE_ID_ECO:           // ECO 모드
                        break;

                    case TEM_MODE_ID_ECO_9V:        // ECO 9V 제어 모드
                        break;

                    default:
                        break;
                }
            }   // End 냉수강약 모드에서 TEM 동작 모드 판단
        }
        else
        {   // 냉수 기능 OFF 시
            gu8TEM_OFFModeConfirmTimer = 0;
            gu8TEM_RapidModeConfirmTimer = 0;
            gu8TEM_RetainModeConfirmTimer = 0;
            gu8TEM_TimeCoolingRapidCheck = CLEAR;       // 급속 모드 동작 중 완료 전 추가 기동 시간 체크 여부 초기화
            gu16TEM_TimerCoolingRapid = 0;              // 급속 모드 동작 중 완료 전 추가 기동 시간 초기화
            gu8TEMControlMode = TEM_MODE_ID_OFF;
        }

        // TEM 동작 모드별 TEM 전압 제어 ===================================================================================================
        switch (gu8TEMControlMode)
        {
            case TEM_MODE_ID_OFF:
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // 급속모드 진입 초기 12V 전압 유지 시간 세팅

                mu8TEM_VoltageBuf = TEM_V_ID_0V;
                mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];
                Set_CoolFanActMode(FAN_ID_OFF);
                break;

            case TEM_MODE_ID_RAPID:
                /*  12℃이상 : 24v 출력
                 *  6℃ 이상 ~ 12℃ 미만 : 12V출력
                 *  외기온도센서 에러 : 12℃ 이상 ~ 30℃미만 조건으로 출력
                 */

                //  야간모드인경우는 약모드 들어가기 위해 FLAG 봐야함
                if (mu8ColdStrong == TRUE)
                {   // 강 냉각
                    if (mu8AirError == FALSE)
                    {   // 외기 온도 센서 에러 아닌 경우
                        if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_1_TEMP)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_1_V;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_2_TEMP)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_2_V;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_3_TEMP)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_3_V;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_4_TEMP)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_4_V;
                        }
                        else
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_5_V;
                        }
                    }
                    else
                    {   // 외기 온도 센서 에러인 경우
                        mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_3_V;
                    }

                    Set_CoolFanActMode(FAN_ID_21V_ON);
                }
                else        /***************냉수 약모드*******************/
                {
                    //  외기온도센서 아닌경우,
                    if (mu8AirError == FALSE)
                    {   // 외기 온도 센서 에러 아닌 경우
                        if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_1_TEMP_LOW_MODE)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_1_V_LOW_MODE;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_2_TEMP_LOW_MODE)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_2_V_LOW_MODE;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_3_TEMP_LOW_MODE)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_3_V_LOW_MODE;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_4_TEMP_LOW_MODE)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_4_V_LOW_MODE;
                        }
                        else
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_5_V_LOW_MODE;
                        }
                    }
                    else
                    {   // 외기 온도 센서 에러인 경우
                        mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_3_V_LOW_MODE;
                    }

                    Set_CoolFanActMode(FAN_ID_21V_ON);
                }

                //  급속모드 진입 후, 초기 5분간은 12v 출력
                if (gu16TimerRapidInCon12V)
                {
                    gu16TimerRapidInCon12V--;
                    mu8TEM_VoltageBuf = TEM_V_ID_12V;
                    mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];
                }
                else
                {
                    mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];
                }
                break;

            case TEM_MODE_ID_RETAIN:                //  유지모드
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // 급속모드 진입 초기 12V 전압 유지 시간 세팅

                if (mu8ColdStrong == TRUE)
                {   // 강 냉각
                    if (mf32ColdTemp >= 5.0f)
                    {
                        mu8TEM_VoltageBuf = TEM_V_ID_12V;
                    }
                    else
                    {
                        mu8TEM_VoltageBuf = TEM_V_ID_5V;
                    }
                }
                else
                {   // 약 냉각
                    if (mf32ColdTemp >= 12.0f)
                    {
                        mu8TEM_VoltageBuf = TEM_V_ID_12V;
                    }
                    else
                    {
                        mu8TEM_VoltageBuf = TEM_V_ID_5V;
                    }
                }

                mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];

                Set_CoolFanActMode(FAN_ID_18V_ON);
                break;

            case TEM_MODE_ID_ECO:
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // 급속모드 진입 초기 12V 전압 유지 시간 세팅
                break;

            case TEM_MODE_ID_ECO_9V:
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // 급속모드 진입 초기 12V 전압 유지 시간 세팅
                break;

            case TEM_MODE_ID_MIN:
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // 급속모드 진입 초기 12V 전압 유지 시간 세팅
                break;

            default:    // TEM OFF와 동일
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // 급속모드 진입 초기 12V 전압 유지 시간 세팅

                mu8TEM_VoltageBuf = TEM_V_ID_0V;
                mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];
                Set_CoolFanActMode(FAN_ID_OFF);
                break;
        }

        Set_TEM_Voltage(mu8TEM_VoltageBuf);
        Set_TEM_TDR(mu16TEM_TDR_DataBuff);
    }
}

#endif

// Lib TEM Module ********************************************************************************************

/// @brief      Lib TEM Module Initilaize
/// @details    TEM 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_TEM_Module_Initialize(void)
{
#if (TEM_USE == USE)
    Lib_TEM_Initialize();
#endif
}


/// @brief      Lib TEM Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 TEM 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_TEM_Module_1ms_Control(void)
{
#if (TEM_USE == USE)
    Lib_TEM_ControlTimer();
#endif
}


/// @brief      Lib TEM Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 TEM 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_TEM_Module_Control(void)
{
#if (TEM_USE == USE)
    Lib_TEM_Control();
#endif
}
