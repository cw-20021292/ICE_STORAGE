/// @file     Lib_Water_PowerSave.c
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    절전 동작 제어 file

#include "Global_Header.h"


#if ( (SMART_NO_USE_POWER_SAVE_USE == USE) || (POWER_SAVE_USE == USE) )


#if (PHOTO_SENSOR_USE == USE)
/// @brief      조도 센서 감지 관련 데이터
/// @details    조도 센서 감지와 관련된 제어를 하기 위한 관련 데이터들을 구조체로 선언한다
typedef struct
{
    U8 gu8BeforeStatus;                     /// @brief  이전에 조도 감지된 상태 : 0(밝음), 1(어두움)
    U8 gu8Status;                           /// @brief  현재의 조도 감지된 상태 : 0(밝음), 1(어두움)
    U16 gu16DarkTimer;                      /// @brief  어두움 감지 시간 타이머(@1Sec)
    U16 gu16BrightTimer;                    /// @brief  밝음 감지 시간 타이머(@1Sec)
}   SBrightDetectData_T;
#endif

/// @brief      절전 관련 데이터
/// @details    절전 제어를 하기 위한 관련 데이터들을 구조체로 선언한다
typedef struct
{
    U8 gu8NoUseFuncStatus;                  /// @brief  미사용 절전 기능 설정 상태 : 0(OFF), 1(ON)
    U8 gu8FuncStatus;                       /// @brief  절전 기능 설정 상태 : 0(OFF), 1(ON)
    U8 gu8Mode;                             /// @brief  동작 중인 모드 : 0(절전 미동작), 1(일반 절전), 2(미사용 절전)
    U8 gu8NoUseTimerSecCount;               /// @brief  제품 미사용 시간(1초 단위)
    U8 gu8NoUseTimerMinCount;               /// @brief  제품 미사용 시간(1분 단위)
    U8 gu8NoUseTimerHourCount;              /// @brief  제품 미사용 시간(1시간 단위)
    U16 gu16SmartNoUseCheck1HrTimer;        /// @brief  스마트 미사용 절전 Type 판단 로직 제어 1시간 단위 체크 Timer
    U8 gu8ButtonInputCheck;                 /// @brief  스마트 미사용 절전 체크 시간 동안 버튼 입력 여부 Flag
    U8 gu8SmartPowerSaveCheckHour;          /// @brief  스마트 미사용 절전 미사용 체크 시간 Timer(16시간)
    U8 gu8SmartPowerSaveLongCheckHour;      /// @brief  스마트 미사용 절전 장기 미사용 체크 시간 Timer(84시간)
    U8 gu8SmartPowerSaveControlType;        /// @brief  스마트 미사용절전 현재 동작 타입(1 - GreenWeek, 2 - IntensiveWeek)
    U8 gu8SmartPowerSavePeriodHour;         /// @brief  스마트 미사용절전 제어 변경 주기(7일 = 168시간)
    U8 gu8SmartPowerSaveCheckCount;         /// @brief  스마트 미사용절전 미사용 체크 카운터
}   SPowerSaveData_T;

#if (PHOTO_SENSOR_USE == USE)
SBrightDetectData_T     SBrightData;
#endif

SPowerSaveData_T    SPowerSaveData;

U16 gu16PowerSaveControlTimer = 0;            /// @brief  절전 제어 주기 타이머


/// @brief      절전 동작 관련 Flag & 변수 초기화
/// @details    절전 동작과 관련된 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_PowerSave_Initialize(void)
{
    gu16PowerSaveControlTimer = 0;

#if (PHOTO_SENSOR_USE == USE)
    SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_BRIGHT;     // 이전 감지 상태 밝음
    SBrightData.gu8Status = BRIGHTNESS_DETECT_BRIGHT;           // 현재 감지 상태 밝음
    SBrightData.gu16DarkTimer = 0;                  // 어두움 감지 시간 타이머
    SBrightData.gu16BrightTimer = 0;                // 밝음 감지 시간 타이머
#endif

    SPowerSaveData.gu8FuncStatus = ON;              // 절전 모드 설정 상태 ON
    SPowerSaveData.gu8NoUseFuncStatus = ON;         // 미사용 절전 설정 상태 ON
    SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // 절전 모드 미동작 상태
    SPowerSaveData.gu8NoUseTimerSecCount = 0;       // 미사용 시간 카운트 Sec
    SPowerSaveData.gu8NoUseTimerMinCount = 0;       // 미사용 시간 카운트 Min
    SPowerSaveData.gu8NoUseTimerHourCount = 0;      // 미사용 시간 카운트 Hour
    SPowerSaveData.gu8ButtonInputCheck = CLEAR;     // 스마트 미사용 절전 체크 기간 동안 버튼 입력 여부 Flag Clear
    SPowerSaveData.gu16SmartNoUseCheck1HrTimer = 0;  // 스마트 미사용 절전 1시간 체크 Timer
    SPowerSaveData.gu8SmartPowerSaveCheckHour = SMART_POWER_SAVE_TIME_NO_USE;           // 스마트 미사용절전 미사용 체크 시간 Timer(@hour)
    SPowerSaveData.gu8SmartPowerSaveLongCheckHour = SMART_POWER_SAVE_TIME_NO_USE_LONG;  // 스마트 미사용절전 장기 미사용 체크 시간 Timer(@hour)
    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;     // 스마트 미사용절전 현재 동작 타입
    SPowerSaveData.gu8SmartPowerSavePeriodHour = SMART_POWER_SAVE_TIME_PERIOD;          // 스마트 미사용절전 제어 변경 주기
    SPowerSaveData.gu8SmartPowerSaveCheckCount = 0;                                     // 스마트 미사용절전 미사용 체크 카운터
}


/// @brief      절전 동작 제어 Control Timer(@1ms)
/// @details    절전 동작 제어를 위한 제어 주기 타이머 카운트를 한다
/// @param      void
/// @return     void
void PowerSaveControlTimer(void)
{
    if (gu16PowerSaveControlTimer < POWER_SAVE_CONTROL_TIME_PERIOD)
    {   // 1초 간격
        gu16PowerSaveControlTimer++;
    }
}


/// @brief      미사용 절전 기능의 사용 여부 설정 함수
/// @details    미사용 절전 기능을 사용할 것인지 사용하지 않을 것인지 설정한다
/// @param      mu8OnOff : 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_NoUsePowerSaveFuncSet(U8 mu8OnOff)
{
    SPowerSaveData.gu8NoUseFuncStatus = mu8OnOff;
}


/// @brief      미사용 절전 기능의 사용 여부 설정 상태 확인 함수
/// @details    미사용 절전 기능을 사용할 것인지 사용하지 않을 것인지 설정되어 있는 것을 반환한다
/// @param      void
/// @return     return : 기능 설정 상태 - 0(OFF), 1(ON)
U8 Get_NoUsePowerSaveFuncSet(void)
{
    U8 mu8Return = 0;

    mu8Return = SPowerSaveData.gu8NoUseFuncStatus;

    return  mu8Return;
}


/// @brief      절전 기능의 사용 여부 설정 함수
/// @details    절전 기능을 사용할 것인지 사용하지 않을 것인지 설정한다
/// @param      mu8OnOff : 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_PowerSaveFuncSet(U8 mu8OnOff)
{
    SPowerSaveData.gu8FuncStatus = mu8OnOff;
}


/// @brief      절전 기능의 사용 여부 설정 상태 확인 함수
/// @details    절전 기능을 사용할 것인지 사용하지 않을 것인지 설정되어 있는 것을 반환한다
/// @param      void
/// @return     return : 기능 설정 상태 - 0(OFF), 1(ON)
U8 Get_PowerSaveFuncSet(void)
{
    U8 mu8Return = 0;

    mu8Return = SPowerSaveData.gu8FuncStatus;

    return  mu8Return;
}


/// @brief      현재 동작 중인 절전 모드 설정 함수
/// @details    현재 동작이 진행중인 절전 모드가 어떤 모드인지 설정한다
/// @param      mu8Mode : 현재 동작 중인 절전 모드 상태 - 0(절전 미동작), 1(일반 절전), 2(미사용 절전)
/// @return     void
void Set_PowerSaveMode(U8 mu8Mode)
{
    SPowerSaveData.gu8Mode = mu8Mode;
}


/// @brief      현재 동작 중인 절전 모드 확인 함수
/// @details    현재 동작이 진행중인 절전 모드가 어떤 모드인지 상태를 반환한다
/// @param      void
/// @return     return : 현재 동작 중인 절전 모드 상태 - 0(절전 미동작), 1(일반 절전), 2(미사용 절전)
U8 Get_PowerSaveMode(void)
{
    U8 mu8Return = 0;

    mu8Return = SPowerSaveData.gu8Mode;

    return  mu8Return;
}


#if (PHOTO_SENSOR_USE == USE)
/// @brief      현재 동작 중인 절전 모드 설정 함수
/// @details    현재 동작이 진행중인 절전 모드가 어떤 모드인지 설정한다
/// @param      mu8Data : 현재 밝기 상태 - 0(밝음), 1(어두움)
/// @return     void
void Set_BrightnessData(U8 mu8Data)
{
    SBrightData.gu8Status = mu8Data;
}


/// @brief      현재 감지된 조도 상태 확인 함수
/// @details    현재 조도 센서를 통해 감지 확정된 밝기 상태를 반환한다
/// @param      void
/// @return     return : 현재 밝기 상태 - 0(밝음), 1(어두움)
U8 Get_BrightnessData(void)
{
    U8 mu8Return = 0;

    mu8Return = SBrightData.gu8Status;

    return  mu8Return;
}
#endif


/// @brief      절전 모드 동작 함수(@1Sec)
/// @details    절전 모드 동작을 실제 제어하는 총괄 동작 처리 함수이다
/// @param      void
/// @return     void
void Lib_PowerSaveControl(void)
{
    if (gu16PowerSaveControlTimer >= POWER_SAVE_CONTROL_TIME_PERIOD)
    {   // 1초 단위
        gu16PowerSaveControlTimer = 0;

#if (PHOTO_SENSOR_USE == USE)
        BrightnessCheckControl();       // 조도 밝기 감지 제어
#endif
        PowerSaveControl();             // 절전 동작 진행 판단 제어
        SmartNoUsePowerSaveControl();   // 미사용 절전 동작 진행 판단 제어
    }
}


#if (PHOTO_SENSOR_USE == USE)
/// @brief      조도 감지 체크 제어 함수(1Sec)
/// @details    조도 센서를 이용하여 현재 밝음 또는 어두움을 감지하는 동작을 한다
/// @param      void
/// @return     void
void BrightnessCheckControl(void)
{
    U8 mu8AD_Brightness = 0;

    mu8AD_Brightness = Get_ADC_Data(ADC_ID_PHOTO_SENSOR);   // 조도 센서 AD 확인

    // 조도 감지
    if (mu8AD_Brightness < SLEEP_IN_LUX_DETECT_AD)
    {   // 어두워진 경우
        SBrightData.gu16BrightTimer = 0;        // 밝음 감지 타이머는 초기화

        if (SBrightData.gu8Status == BRIGHTNESS_DETECT_BRIGHT)
        {   // 밝음 감지 상태였던 경우
            if (SBrightData.gu16DarkTimer >= BRIGHT_DETECT_TIME)
            {   // 어두움 감지가 10분 유지시
                SBrightData.gu16DarkTimer = 0;
                SBrightData.gu8Status = BRIGHTNESS_DETECT_DARK;
            }
            else
            {
                SBrightData.gu16DarkTimer++;
            }
        }
    }
    else if (mu8AD_Brightness > SLEEP_OUT_LUX_DETECT_AD)
    {   // 밝아진 경우
        SBrightData.gu16DarkTimer = 0;          // 어두움 감지 타이머는 초기화

        if ((SBrightData.gu8Status == BRIGHTNESS_DETECT_DARK))
        {   // 어두움 감지 상태였던 경우
            if (SBrightData.gu16BrightTimer >= BRIGHT_DETECT_TIME)
            {   // 밝음 감지가 10분 유지시
                SBrightData.gu16BrightTimer = 0;
                SBrightData.gu8Status = BRIGHTNESS_DETECT_BRIGHT;
            }
            else
            {
                SBrightData.gu16BrightTimer++;
            }
        }
    }
    else
    {   // 어두움과 밝음 감지 기준 그 사이인 경우 감지 상태값은 이전 감지된 상태 유지
        SBrightData.gu16DarkTimer = 0;
        SBrightData.gu16BrightTimer = 0;
    }
}
#endif


/// @brief      일반 절전 제어 함수(1Sec)
/// @details    일반 절전 동작과 관련하여 절전 동작 진행 여부를 결정한다
/// @param      void
/// @return     void
void PowerSaveControl(void)
{
#if (PHOTO_SENSOR_USE == USE)       // 조도 센서 사용 절전 모드
    if (SPowerSaveData.gu8FuncStatus == ON)
    {   // 절전 기능을 사용하는 경우
        if (SPowerSaveData.gu8Mode == POWERSAVE_ID_NONE)
        {   // 절전 모드가 동작 중이지 않은 경우
            if (SBrightData.gu8Status == BRIGHTNESS_DETECT_DARK)
            {   // 어두움이 감지된 상태인 경우
                SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_DARK;   // 이전 감지값을 어두움으로 설정
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NORMAL;
            }
        }
        else
        {   // 미사용 절전이나 절전 모드가 동작 중인 경우
            if (SBrightData.gu8Status == BRIGHTNESS_DETECT_BRIGHT)
            {   // 밝음이 감지된 상태인 경우
                SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_BRIGHT; // 이전 감지값을 밝음으로 설정
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // 미사용 절전, 절전 동작 해제
                NoUseTimeClear();       // 제품 미사용 시간 초기화
            }
        }
    }
    else
    {   // 절전 기능을 사용하지 않는 경우
        if (SPowerSaveData.gu8Mode == POWERSAVE_ID_NORMAL)
        {   // 절전 동작 중이었으면
            SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // 절전 동작 해제
        }

        if (SBrightData.gu8Status == BRIGHTNESS_DETECT_DARK)
        {   // 어두움이 감지된 상태인 경우
            SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_DARK;   // 이전 감지값을 어두움으로 설정
        }
        else if (SBrightData.gu8Status == BRIGHTNESS_DETECT_BRIGHT)
        {   // 밝음이 감지된 상태인 경우
            if (SBrightData.gu8BeforeStatus == BRIGHTNESS_DETECT_DARK)
            {   // 이전에 어두움이 감지되었던 경우
                SBrightData.gu8Status = BRIGHTNESS_DETECT_BRIGHT;
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // 미사용 절전, 절전 동작 해제
                NoUseTimeClear();       // 제품 미사용 시간 초기화
            }
        }
        else
        {
        }
    }
#else   // 조도 센서 미사용 절전 모드

#endif
}


/// @brief      미사용 절전 모드 동작 함수(1Sec)
/// @details    미사용 절전 동작과 관련하여 절전 동작 진행 여부를 결정한다
/// @param      void
/// @return     void
void SmartNoUsePowerSaveControl(void)
{
    U8 mu8AirTempError = 0;

    NoUseTimeCounter();                     // 제품 미사용 시간 체크
    DecisionSmartPowerSaveType();           // 미사용 절전 동작 Type 판단

    mu8AirTempError = Get_AirTempError();   // 외기 온도 센서 에러 발생 확인

    if (mu8AirTempError == TRUE)
    {   // 외기 온도 센서 에러가 발생한 경우
        if (SPowerSaveData.gu8Mode == POWERSAVE_ID_SMART)
        {   // 스마트 미사용 절전 동작 중인 경우
            SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // 일반 절전 모드 시작 여부 다시 체크할 수 있도록 절전 미동작 상태로 전환
        }

        NoUseTimeClear();
    }
    else
    {   // 외기 온도 센서 에러가 발생하지 않은 경우
        if (SPowerSaveData.gu8NoUseFuncStatus == ON)
        {   // 미사용 절전 기능 ON시
            if (SPowerSaveData.gu8Mode != POWERSAVE_ID_SMART)
            {   // 미사용 절전 모드 동작 중이 아닌 경우
                if (SPowerSaveData.gu8NoUseTimerHourCount >= NO_USE_TIME_OUT)
                {   // 미사용 9시간 경과 후
                    SPowerSaveData.gu8Mode = POWERSAVE_ID_SMART;
                    NoUseTimeClear();
                }
            }
            else
            {   // 미사용 절전 모드 동작 중인 경우
                NoUseTimeClear();
            }
        }
        else
        {   // 미사용 절전 기능 OFF시
            if (SPowerSaveData.gu8Mode == POWERSAVE_ID_SMART)
            {   // 스마트 미사용 절전 동작 중인 경우
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // 일반 절전 모드 시작 여부 다시 체크할 수 있도록 절전 미동작 상태로 전환
            }

            NoUseTimeClear();
            SmartNoUseCheckClear();
        }
    }
}


/// @brief      미사용 시간 카운터 초기화 함수
/// @details    제품 미사용 시간을 초기화 시킨다
/// @param      void
/// @return     void
void NoUseTimeClear(void)
{
    SPowerSaveData.gu8NoUseTimerSecCount = 0;
    SPowerSaveData.gu8NoUseTimerMinCount = 0;
    SPowerSaveData.gu8NoUseTimerHourCount = 0;
}


/// @brief      미사용 시간 카운터 함수(1Sec)
/// @details    제품 미사용 시간을 카운트 한다
/// @param      void
/// @return     void
void NoUseTimeCounter(void)
{
    if (SPowerSaveData.gu8SmartPowerSaveControlType == SMART_POWER_SAVE_TYPE_INTENSIVE_WEEK)
    {   // 스마트 미사용  집중 주간에는 미사용 절전 모드 진입 안함
        if (SPowerSaveData.gu8Mode == POWERSAVE_ID_SMART)
        {   // 스마트 미사용 절전 동작 중인 경우
            SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // 일반 절전 모드 시작 여부 다시 체크할 수 있도록 절전 미동작 상태로 전환
        }

        NoUseTimeClear();
    }
    else
    {   // 스마트 미사용 절전 주간에는 미사용 절전 모드 진입 여부 판단하도록 미사용 시간 체크
        if (SPowerSaveData.gu8NoUseTimerHourCount < NO_USE_TIME_OUT)
        {   // 9시간 미만이면
            if (++SPowerSaveData.gu8NoUseTimerSecCount >= 60)
            {   // 1분 도달시 마다
                SPowerSaveData.gu8NoUseTimerSecCount = 0;       // 1분 카운트 했으니 초 카운트 Clear

                if (++SPowerSaveData.gu8NoUseTimerMinCount >= 60)
                {   // 1시간 도달시 마다
                    SPowerSaveData.gu8NoUseTimerMinCount = 0;   // 1시간 카운트 했으니 분 카운트 Clear
                    SPowerSaveData.gu8NoUseTimerHourCount++;
                }
            }
        }
        else
        {   // 9시간 이상이면
            SPowerSaveData.gu8NoUseTimerSecCount = 0;   // 초 카운트 Clear
            SPowerSaveData.gu8NoUseTimerMinCount = 0;   // 분 카운트 Clear
        }
    }
}


/// @brief      미사용 절전 동작과 관련하여 미사용 체크 중 버튼 입력 여부 설정 함수
/// @details    미사용 절전 Type 결정을 위한 로직 동작 중 버튼의 입력 여부 이력을 설정해 놓는다
/// @param      mu8Data : 버튼 입력 여부 이력 - 0(미입력), 1(입력)
/// @return     void
void Set_SmartNoUsePowerSaveButtonInput(U8 mu8Data)
{
    if (mu8Data)
    {
        SPowerSaveData.gu8ButtonInputCheck = SET;
    }
    else
    {
        SPowerSaveData.gu8ButtonInputCheck = CLEAR;
    }
}


/// @brief      스마트 미사용 절전 동작 Type 판단 함수
/// @details    스마트 미사용 제어 관련 매주 동작 Type을 판단하기 위한 체크를 위한 동작을 수행한다
/// @param      void
/// @return     void
void DecisionSmartPowerSaveType(void)
{
    if (++SPowerSaveData.gu16SmartNoUseCheck1HrTimer >= 3600)
    {   // 1시간(3600초@1sec) 경과시 마다 제어
        SPowerSaveData.gu16SmartNoUseCheck1HrTimer = 0;     // 스마트 미사용 절전 1시간 체크 Timer

        SmartNoUseTimeCounter();

        if (SPowerSaveData.gu8SmartPowerSavePeriodHour == 0)
        {   // 스마트 미사용 절전 제어 체크 주기(일주일)가 경과된 후
            if (SPowerSaveData.gu8SmartPowerSaveLongCheckHour == 0)
            {   // 장기 미사용 시간이 경과된 상태로 유지된 경우
                SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;     // Green Week로 동작
            }
            else
            {
                if(SPowerSaveData.gu8SmartPowerSaveCheckCount <= 2)
                {   // 주기 내 미사용 카운트가 2회 이하
                    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_INTENSIVE_WEEK;     // Intensive Week로 동작
                }
                else if(SPowerSaveData.gu8SmartPowerSaveCheckCount >= 4)
                {   // 주기 내 미사용 카운트가 4회 이상
                    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;         // Green Week로 동작
                }
                else
                {   // 주기 내 미사용 카운트가 3회인 경우
                    //  이전 제어 type 유지
                }
            }

            if (SPowerSaveData.gu8Mode == POWERSAVE_ID_SMART)
            {   // 스마트 미사용 절전 동작 중인 경우
                SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;     // 일주일 경과시 미사용 절전은 일반 절전 모드 시작 여부 다시 체크할 수 있도록 절전 미동작 상태로 전환
            }

            NoUseTimeClear();               // 미사용 절전 진입 체크 시간 초기화

            SPowerSaveData.gu8ButtonInputCheck = CLEAR;         // 스마트 미사용 절전 체크 기간 동안 버튼 입력 여부 Flag
            SPowerSaveData.gu8SmartPowerSaveCheckHour = SMART_POWER_SAVE_TIME_NO_USE;                   // 16시간 초기화
            SPowerSaveData.gu8SmartPowerSaveLongCheckHour = SMART_POWER_SAVE_TIME_NO_USE_LONG;          // 84시간 초기화
            SPowerSaveData.gu8SmartPowerSavePeriodHour = SMART_POWER_SAVE_TIME_PERIOD;                  // 168시간 초기화
            SPowerSaveData.gu8SmartPowerSaveCheckCount = 0;
        }
        else
        {   // 스마트 미사용 절전 제어 체크 주기 168시간(7일) 이내 동작 중
            if (SPowerSaveData.gu8ButtonInputCheck == SET)
            {   // 버튼 입력이 발생한 경우
                if (SPowerSaveData.gu8SmartPowerSaveCheckHour == 0)
                {   // 버튼 입력이 있었을 때 스마트 미사용 체크 시간(16시간)이 이미 경과된 경우
                    if (SPowerSaveData.gu8SmartPowerSaveCheckCount < 0xFF)
                    {
                        SPowerSaveData.gu8SmartPowerSaveCheckCount++;     // 미사용 카운트 증가
                    }
                }

                SPowerSaveData.gu8SmartPowerSaveCheckHour = SMART_POWER_SAVE_TIME_NO_USE;               // 16시간 초기화
                SPowerSaveData.gu8SmartPowerSaveLongCheckHour = SMART_POWER_SAVE_TIME_NO_USE_LONG;      // 84시간 초기화
                SPowerSaveData.gu8ButtonInputCheck = CLEAR;
            }
            else
            {   // 버튼 입력이 미발생한 경우
                if (SPowerSaveData.gu8SmartPowerSaveLongCheckHour == 0)
                {   // 스마트 미사용 절전 장기 미사용 체크 시간(84시간)이 경과된 경우
                    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;     // 즉시 Green Week로 전환
                }
            }
        }
    }
}


/// @brief      스마트 미사용 절전 동작 Type 판단을 위한 체크 시간 카운트 함수
/// @details    스마트 미사용 절전 동작 Type를 판단하는 일주일 시간 및 16시간 카운트를 카운트다운 한다
/// @param      void
/// @return     void
void SmartNoUseTimeCounter(void)
{
    if (SPowerSaveData.gu8SmartPowerSavePeriodHour)
    {   // 스마트 미사용 절전 제어 체크 주기(168Hr) 카운트 다운
        SPowerSaveData.gu8SmartPowerSavePeriodHour--;
    }

    if (SPowerSaveData.gu8SmartPowerSaveCheckHour)
    {   // 스마트 미사용 절전 미사용 체크 시간(16Hr) 카운트 다운
        SPowerSaveData.gu8SmartPowerSaveCheckHour--;
    }

    if (SPowerSaveData.gu8SmartPowerSaveLongCheckHour)
    {   // 스마트 미사용 절전 장기 미사용 체크 시간(84Hr) 카운트 다운
        SPowerSaveData.gu8SmartPowerSaveLongCheckHour--;
    }
}


/// @brief      스마트 미사용 절전 동작 체크 관련 변수들 모두 초기화
/// @details    스마트 미사용 절전 동작과 관련된 체크 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void SmartNoUseCheckClear(void)
{
    SPowerSaveData.gu8ButtonInputCheck = CLEAR;     // 스마트 미사용 절전 체크 기간 동안 버튼 입력 여부 Flag Clear
    SPowerSaveData.gu16SmartNoUseCheck1HrTimer = 0;  // 스마트 미사용 절전 1시간 체크 Timer
    SPowerSaveData.gu8SmartPowerSaveCheckHour = SMART_POWER_SAVE_TIME_NO_USE;               // 16시간 초기화
    SPowerSaveData.gu8SmartPowerSaveLongCheckHour = SMART_POWER_SAVE_TIME_NO_USE_LONG;      // 84시간 초기화
    SPowerSaveData.gu8SmartPowerSaveControlType = SMART_POWER_SAVE_TYPE_GREEN_WEEK;         // 스마트 미사용절전 현재 동작 타입 - Green Week
    SPowerSaveData.gu8SmartPowerSavePeriodHour = SMART_POWER_SAVE_TIME_PERIOD;              // 168시간 초기화
    SPowerSaveData.gu8SmartPowerSaveCheckCount = 0;                                 // 스마트 미사용절전 미사용 체크 카운터
}


/// @brief      절전, 미사용 절전 동작 해제 함수
/// @details    버튼 입력 등으로 절전 동작이나 미사용 절전 동작을 중단할 때 사용한다
/// @param      void
/// @return     void
void PowerSaveActEnd(void)
{
    if (SPowerSaveData.gu8Mode != POWERSAVE_ID_NONE)
    {   // 절전 또는 미사용 절전 동작중인 경우
        SPowerSaveData.gu8Mode = POWERSAVE_ID_NONE;

#if (PHOTO_SENSOR_USE == USE)
        // 절전 동작 여부는 다시 체크 시작할 수 있도록 조도 밝음 상태로 초기화 및 조도의 밝기 체크 관련 변수들 모두 초기화
        SBrightData.gu8BeforeStatus = BRIGHTNESS_DETECT_BRIGHT;
        SBrightData.gu8Status = BRIGHTNESS_DETECT_BRIGHT;
        SBrightData.gu16BrightTimer = 0;
        SBrightData.gu16DarkTimer = 0;
#endif
    }
}



#endif      // 스마트 미사용 절전 사용 또는 절전 사용 END

// Lib Power Save Module *********************************************************************************

/// @brief      Lib Power Save Module Initilaize
/// @details    미사용 절전, 절전 판단 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_PowerSave_Module_Initialize(void)
{
    Lib_PowerSave_Initialize();
}


/// @brief      Lib Power Save Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 Power Save 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_PowerSave_Module_1ms_Control(void)
{
    PowerSaveControlTimer();
}


/// @brief      Lib Power Save Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Power Save 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_PowerSave_Module_Control(void)
{
    Lib_PowerSaveControl();
}

