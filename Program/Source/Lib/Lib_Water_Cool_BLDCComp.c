/// @file     Lib_Water_Cool_BLDCComp.c
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    냉각 동작 제어 file

#include "Global_Header.h"


#if (BLDC_COMP_USE == USE)              // BLDC Comp 사용시


#if (BLDC_COMP_PWM_CONTROL_USE == USE)      // PWM 제어 방식 Comp 제어

/// @brief      히팅 제어 온도 기준 데이터 구성
/// @details    히팅 제어를 하기 위한 기준 온도 데이터들을 구조체로 선언한다
typedef struct
{
    F32 gf32TempFirstCoolOff;               /// @brief  첫 기동시 냉각기 OFF 온도(섭씨값, 소수점)
    F32 gf32TempFirstCoolOn;                /// @brief  첫 기동시 냉각기 ON 온도(섭씨값, 소수점)
    U16 gu16FirstControlRPM;                /// @brief  첫 기동시 냉각기 제어 RPM
    U16 gu16FirstCoolOffDelay;              /// @brief  냉각기 OFF전 Delay(추가 기동) 시간(@100ms)
    F32 gf32TempCoolOff;                    /// @brief  재기동시 냉각기 OFF 온도(섭씨값, 소수점)
    F32 gf32TempCoolOn;                     /// @brief  재기동시 냉각기 ON 온도(섭씨값, 소수점)
    U16 gu16ControlRPM;                     /// @brief  재기동시 냉각기 제어 RPM
    U16 gu16CoolOffDelay;                   /// @brief  냉각기 OFF전 Delay(추가 기동) 시간(@100ms)
}   SCoolTable_T;


/// @brief      냉각기 제어 온도 기준 테이블
/// @details    외기온도 및 절전 모드 상태에 따른 단계별 냉각 On, Off 기준 온도값 및 냉각 Off전 지연시간, 구동 Hz를 세팅한다
__far const SCoolTable_T  SCoolData[POWERSAVE_ID_MAX][COOL_STEP_ID_MAX] = {
// 첫기동 냉각 OFF 온도, 첫기동 냉각 ON 온도, 첫기동 제어 RPM, 첫기동 냉각 OFF Delay 시간, 재기동 냉각 OFF 온도, 재기동 냉각 ON 온도, 재기동 제어 RPM, 재기동 냉각 OFF Delay 시간
    // 일반 동작(절전 동작X)
    {
        {8.0f,      6.0f,   2580,       0,          8.0f,       6.0f,   2580,   0    },     // 외기 온도 에러시
        {8.0f,      6.0f,   2580,       9000,       8.0f,       6.0f,   2580,   9000 },     // 외기 온도에 따른 제어 1단계
        {8.0f,      6.0f,   2580,       9000,       8.0f,       6.0f,   2580,   9000 },     // 외기 온도에 따른 제어 2단계
        {8.0f,      6.0f,   2580,       12000,      8.0f,       6.0f,   2580,   12000},     // 외기 온도에 따른 제어 3단계
        {8.0f,      6.0f,   2580,       12000,      8.0f,       6.0f,   2580,   12000}      // 외기 온도에 따른 제어 4단계
    },

    // 일반 절전
    {
        {10.0f,     6.0f,   2580,       0,          10.0f,      6.0f,   2580,   0    },     // 외기 온도 에러시
        {10.0f,     6.0f,   2580,       9000,       10.0f,      6.0f,   2580,   9000 },     // 외기 온도에 따른 제어 1단계
        {10.0f,     6.0f,   2580,       9000,       10.0f,      6.0f,   2580,   9000 },     // 외기 온도에 따른 제어 2단계
        {10.0f,     6.0f,   2580,       12000,      10.0f,      6.0f,   2580,   12000},     // 외기 온도에 따른 제어 3단계
        {10.0f,     6.0f,   2580,       12000,      10.0f,      6.0f,   2580,   12000}      // 외기 온도에 따른 제어 4단계
    },

    // 미사용 절전
    {
        {10.0f,     6.0f,   2580,       0,          10.0f,      6.0f,   2580,   0    },     // 외기 온도 에러시
        {10.0f,     6.0f,   2580,       9000,       10.0f,      6.0f,   2580,   9000 },     // 외기 온도에 따른 제어 1단계
        {10.0f,     6.0f,   2580,       9000,       10.0f,      6.0f,   2580,   9000 },     // 외기 온도에 따른 제어 2단계
        {10.0f,     6.0f,   2580,       12000,      10.0f,      6.0f,   2580,   12000},     // 외기 온도에 따른 제어 3단계
        {10.0f,     6.0f,   2580,       12000,      10.0f,      6.0f,   2580,   12000}      // 외기 온도에 따른 제어 4단계
    }
};

#else     // 통신형 BLDC Comp 사용

/// @brief      냉각기 제어 온도 기준 데이터 구성
/// @details    외기온도 및 절전 모드 상태에 따른 단계별 냉각 On, Off 기준 온도값 및 냉각 Off전 지연시간, 구동 Hz를 세팅한다
typedef struct
{
    F32 gf32TempFirstCoolOff;               /// @brief  첫 기동시 냉각기 OFF 온도(섭씨값, 소수점)
    F32 gf32TempFirstCoolOn;                /// @brief  첫 기동시 냉각기 ON 온도(섭씨값, 소수점)
    U8 gu8FirstTargetHz;                    /// @brief  첫 기동시 냉각기 제어 Hz
    U16 gu16FirstCoolOffDelay;              /// @brief  냉각기 OFF전 Delay(추가 기동) 시간(@100ms)
    F32 gf32TempCoolOff;                    /// @brief  재기동시 냉각기 OFF 온도(섭씨값, 소수점)
    F32 gf32TempCoolOn;                     /// @brief  재기동시 냉각기 ON 온도(섭씨값, 소수점)
    U8 gu8TargetHz;                         /// @brief  재기동시 냉각기 제어 Hz
    U16 gu16CoolOffDelay;                   /// @brief  냉각기 OFF전 Delay(추가 기동) 시간(@100ms)
}   SCoolTable_T;


/// @brief      냉각기 제어 온도 기준 테이블
/// @details    외기온도 및 절전 모드 상태에 따른 단계별 냉각 On, Off 기준 온도값 및 냉각 Off전 지연시간을 세팅한다
__far const SCoolTable_T  SCoolData[POWERSAVE_ID_MAX][COOL_STEP_ID_MAX] = {
// 첫기동 냉각 OFF 온도, 첫기동 냉각 ON 온도, 첫기동 제어 Hz, 첫기동 냉각 OFF Delay 시간, 재기동 냉각 OFF 온도, 재기동 냉각 ON 온도, 재기동 제어 Hz, 재기동 냉각 OFF Delay 시간
    // 일반 동작(절전 동작X)
    {
        {8.0f,      6.0f,   40,     0,          8.0f,       6.0f,   40,     0    },     // 외기 온도 에러시
        {8.0f,      6.0f,   40,     9000,       8.0f,       6.0f,   40,     9000 },     // 외기 온도에 따른 제어 1단계
        {8.0f,      6.0f,   40,     9000,       8.0f,       6.0f,   40,     9000 },     // 외기 온도에 따른 제어 2단계
        {8.0f,      6.0f,   40,     12000,      8.0f,       6.0f,   40,     12000},     // 외기 온도에 따른 제어 3단계
        {8.0f,      6.0f,   40,     12000,      8.0f,       6.0f,   40,     12000}      // 외기 온도에 따른 제어 4단계
    },

    // 일반 절전
    {
        {10.0f,     6.0f,   40,     0,          10.0f,      6.0f,   40,     0    },     // 외기 온도 에러시
        {10.0f,     6.0f,   40,     9000,       10.0f,      6.0f,   40,     9000 },     // 외기 온도에 따른 제어 1단계
        {10.0f,     6.0f,   40,     9000,       10.0f,      6.0f,   40,     9000 },     // 외기 온도에 따른 제어 2단계
        {10.0f,     6.0f,   40,     12000,      10.0f,      6.0f,   40,     12000},     // 외기 온도에 따른 제어 3단계
        {10.0f,     6.0f,   40,     12000,      10.0f,      6.0f,   40,     12000}      // 외기 온도에 따른 제어 4단계
    },

    // 미사용 절전
    {
        {10.0f,     6.0f,   40,     0,          10.0f,      6.0f,   40,     0    },     // 외기 온도 에러시
        {10.0f,     6.0f,   40,     9000,       10.0f,      6.0f,   40,     9000 },     // 외기 온도에 따른 제어 1단계
        {10.0f,     6.0f,   40,     9000,       10.0f,      6.0f,   40,     9000 },     // 외기 온도에 따른 제어 2단계
        {10.0f,     6.0f,   40,     12000,      10.0f,      6.0f,   40,     12000},     // 외기 온도에 따른 제어 3단계
        {10.0f,     6.0f,   40,     12000,      10.0f,      6.0f,   40,     12000}      // 외기 온도에 따른 제어 4단계
    }
};

#endif

U8 gu8ColdOnOff = 0;                        /// @brief  냉수 기능 On/Off 상태
U8 gu8ColdStrong = 0;                       /// @brief  냉수 기능 강약 상태

U8 gu8CoolControlPeriodTimer = 0;           /// @brief  냉각 제어 시간 주기 타이머

U8 gu8FirstCompAct = 0;                     /// @brief  냉각 첫 기동 여부 - 0(첫 기동 아님), 1(첫 기동임)

U8 gu8ColdActStep = 0;                      /// @brief  외기 온도 조건에 따른 냉각 제어 Step - 0~4단계
F32 gf32ColdTargetHighTemp = 0;             /// @brief  냉각 제어 OFF 기준 온도
F32 gf32ColdTargetLowTemp = 0;              /// @brief  냉각 제어 ON 기준 온도

#if (BLDC_COMP_PWM_CONTROL_USE == USE)      // PWM 제어 방식 Comp 제어
U16 gu16ColdControlRPM = 0;                 /// @brief  PWM 냉각 제어 방식 제어 RPM
#else
U8 gu8ColdTargetHz = 0;                     /// @brief  통신 제어 방식 냉각 제어 Hz
#endif

U16 gu16CoolDelayTimer = 0;                 /// @brief  냉각 동작 전 지연 시간 타이머
U16 gu16CoolerActTimer = 0;                 /// @brief  냉각 연속 동작 시간 타이머
U16 gu16CoolMoreActTimer = 0;               /// @brief  냉각 목표 온도 도달 후 추가 동작 시간(냉각 OFF전 지연 시간) 타이머

U8 gu8ManualCoolTestModeTest = 0;           /// @brief  수동 냉각 테스트 동작 상태 - 0(미동작), 1(동작)
U8 gu8ManualCoolTestModeAct = 0;            /// @brief  수동 냉각 테스트 중 Comp 동작 상태 - 0(OFF), 1(ON)

U16 gu16ColdOffTime = 0;                    /// @brief  냉수 기능 OFF 후 경과시간 타이머



/// @brief      BLDC Comp 냉각 동작 관련 Flag & 변수 초기화
/// @details    BLDC Comp 냉각 동작과 관련된 변수들을 초기화 한다
/// @param      void
/// @return     void
void Lib_BLDC_Comp_Initialize(void)
{
    gu8ColdOnOff = 0;
    gu8ColdStrong = 0;

    gu8CoolControlPeriodTimer = 0;

    gu8FirstCompAct = 0;

    gu8ColdActStep = 0;
    gf32ColdTargetHighTemp = 0;
    gf32ColdTargetLowTemp = 0;

#if (BLDC_COMP_PWM_CONTROL_USE == USE)      // PWM 제어 방식 Comp 제어
    gu16ColdControlRPM = 0;
#else
    gu8ColdTargetHz = 0;
#endif

    gu16CoolDelayTimer = 0;
    gu16CoolerActTimer = 0;
    gu16CoolMoreActTimer = 0;

    gu8ManualCoolTestModeTest = 0;
    gu8ManualCoolTestModeAct = 0;

    gu16ColdOffTime = 0;
}



/// @brief      BLDC Comp 제어 시간 타이머(@1ms)
/// @details    1ms 인터럽트 안에서 BLDC Comp 제어 시간 타이머를 카운트 한다
/// @param      void
/// @return     void
void Comp_ControlTimer(void)
{
    if (gu8CoolControlPeriodTimer < BLDC_COMP_CONTROL_TIME_PERIOD)
    {
        gu8CoolControlPeriodTimer++;
    }
}


/// @brief      냉수 기능 ON/OFF 상태 설정 함수
/// @details    냉수 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8Status : 냉수 기능 설정 상태 : 0(OFF), 1(ON)
/// @return     void
void Set_ColdFuncStatus(U8 mu8Status)
{
    gu8ColdOnOff = mu8Status;
}


/// @brief      냉수 기능 ON/OFF 상태 확인 함수
/// @details    냉수 기능 상태가 ON인지 OFF 인지 확인하여 그 값을 반환한다
/// @param      void
/// @return     return : 냉수 기능 설정 상태 : 0(OFF), 1(ON)
U8 Get_ColdFuncStatus(void)
{
    return  gu8ColdOnOff;
}


/// @brief      냉수 기능 강약 상태 설정 함수
/// @details    냉수 기능 상태가 강인지 약인지 설정한다
/// @param      mu8Status : 냉수 기능 강약 설정 상태 : 0(약), 1(강)
/// @return     void
void Set_ColdStorng(U8 mu8Status)
{
    gu8ColdStrong = mu8Status;
}


/// @brief      냉수 기능 강약 상태 확인 함수
/// @details    냉수 기능 상태가 강인지 약 인지 확인하여 그 값을 반환한다
/// @param      void
/// @return     return : 냉수 기능 강약 설정 상태 : 0(약), 1(강)
U8 Get_ColdStrong(void)
{
    return  gu8ColdStrong;
}


/// @brief      BLDC Comp 수동 테스트 제어 함수(@100ms)
/// @details    BLDC Comp 수동 테스트 동작을 제어한다
/// @param      void
/// @return     void
void Manual_BLDC_CompTest_Control(void)
{
    U8 mu8CompStatus = 0;

    if (gu8ManualCoolTestModeTest == SET)
    {       // 냉각 테스트 진행 중
        if (gu8ManualCoolTestModeAct == SET)
        {   // 냉각 테스트 동작 중
            if (gu16CoolDelayTimer >= DELAY_TIME_BEFORE_COMP_ON)
            {
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                Set_BLDC_Comp_RPM(2580);
#else
                Set_BLDC_Comp_Hz(60);
#endif
            }
        }
        else
        {   // 냉각 테스트 동작 정지
            mu8CompStatus = Get_BLDC_Comp_Status();
            if (mu8CompStatus == TRUE)
            {
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                Set_BLDC_Comp_RPM(0);
#else
                Set_BLDC_Comp_Hz(0);
#endif
                gu16CoolDelayTimer = 0;
            }
        }
    }
}


/// @brief      BLDC Comp 수동 테스트 진행 여부 설정
/// @details    BLDC Comp 수동 테스트 진행 여부를 설정한다
/// @param      mu8Status : BLDC Comp 수동 테스트 여부 - 0(미동작), 1(동작)
/// @return     void
void Set_Manual_BLDC_CompTest(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8ManualCoolTestModeTest = SET;
    }
    else
    {
        gu8ManualCoolTestModeTest = CLEAR;
    }
}


/// @brief      BLDC Comp 수동 테스트 진행 여부 확인
/// @details    BLDC Comp 수동 테스트 진행 여부를 확인하여 반환한다
/// @param      void
/// @return     return : BLDC Comp 수동 테스트 여부 - 0(미동작), 1(동작)
U8 Get_Manual_BLDC_CompTest(void)
{
    if (gu8ManualCoolTestModeTest)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      BLDC Comp 수동 테스트 중 Comp On/Off 설정
/// @details    BLDC Comp 수동 테스트 중 Comp On/Off 상태를 설정한다
/// @param      mu8Status : BLDC Comp 동작 - 0(OFF), 1(ON)
/// @return     void
void Set_Manual_BLDC_CompTestAct(U8 mu8OnOff)
{
    if (mu8OnOff)
    {
        gu8ManualCoolTestModeAct = SET;
        gu16CoolDelayTimer = DELAY_TIME_BEFORE_COMP_ON;         // 바로 테스트 동작할 수 있도록 Comp 동작 전 Delay를 강제로 최대로 세팅
    }
    else
    {
        gu8ManualCoolTestModeAct = CLEAR;
    }
}


/// @brief      BLDC Comp 수동 테스트 중 Comp On/Off 설정 상태 확인
/// @details    BLDC Comp 수동 테스트 중 Comp On/Off 설정 상태를 확인하여 반환한다
/// @param      void
/// @return     return : BLDC Comp 동작 상태 - 0(OFF), 1(ON)
U8 Get_Manual_BLDC_CompTestAct(void)
{
    if (gu8ManualCoolTestModeAct)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      냉각 동작 관련 시간 Counter 함수(@100ms)
/// @details    냉각 제어 동작 관련 시간 카운트 동작을 한다
/// @param      void
/// @return     void
void BLDC_Comp_ActTimeCounter(void)
{
    U8 mu8CompOn = 0;
    U8 mu8ColdOn = 0;
    F32 mf32ColdTemp = 0;

    static U8 mu8CompActTimerSec = 0;
    static U16 mu16CompActTimerMin = 0;

    if (++mu8CompActTimerSec >= 10)
    {   // 1초 마다
        mu8CompActTimerSec = 0;

        mu8CompOn = Get_BLDC_Comp_Status();      // Comp On/Off 상태 확인
        mf32ColdTemp = Get_Temp(COOL_THERMISTOR_1_ID);      // 냉수 온도 센서 상태 확인
        mu8ColdOn = Get_ColdOnOff();        // 냉각 기능 ON/OFF 상태 확인

        // 냉각 동작 전 지연 시간 카운터 ***********************************************************
        if (mu8CompOn == FALSE)
        {   // Comp Off 상태면
            if (gu16CoolDelayTimer < DELAY_TIME_BEFORE_COMP_ON)
            {
                gu16CoolDelayTimer++;
            }
        }

        // Comp 동작 최대 시간 경과 카운터 ********************************************************
        if (mu8CompOn == TRUE)
        {   // Comp On 상태면
            if (gu16CoolerActTimer < COMP_ON_MAX_TIME)
            {
                gu16CoolerActTimer++;

                if (mf32ColdTemp < 1.0f)
                {   // 냉수 온도 센서가 1도 미만이 되는 경우
                    gu16CoolerActTimer = COMP_ON_MAX_TIME;   // 최대 시간 강제 세팅
                }
            }
        }
        else
        {   // Comp Off 상태면
            gu16CoolerActTimer = 0;
        }

        // 냉각 기능 OFF 경과 시간 카운터 **********************************************************
        if (++mu16CompActTimerMin >= 60)
        {   // 1분 마다
            if (mu8ColdOn == FALSE)
            {   // 냉각 기능이 꺼진 경우
                if (gu16ColdOffTime < COMP_OFF_CHECK_MAX_TIME)
                {
                    gu16ColdOffTime++;
                }
            }
            else
            {   // 냉각 기능이 켜진 경우
                gu16ColdOffTime = 0;
            }
        }
    }
}


/// @brief      냉수 기능 OFF 상태 경과 시간 최대 경과 완료 확인 함수
/// @details    냉수 기능이 OFF된 상태로 정해진 최대 경과시간이 경과하였는지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 냉수 기능 OFF 상태로 기준 최대 시간 경과 완료 여부 - 0(미완료), 1(완료)
U8 Get_ColdOffMaxTimeEnd(void)
{
    if (gu16ColdOffTime >= COMP_OFF_CHECK_MAX_TIME)
    {   // 최대 경과 시간 경과 완료
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Comp 동작 최대 제한 시간 경과 완료 확인 함수
/// @details    Comp 동작 최대 제한 시간이 경과하였는지 여부를 체크하여 그 상태를 반환한다
/// @param      void
/// @return     return : Comp 동작 최대 시간 경과 여부 - 0(미완료), 1(완료)
U8 Get_CompActMaxTimeEnd(void)
{
    if (gu16CoolerActTimer >= COMP_ON_MAX_TIME)
    {   // 최대 경과 시간 경과 완료
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Comp 동작 전 지연 시간 경과 완료 확인 함수
/// @details    Comp 동작 전 지연 시간 경과가 완료하였는지 여부를 체크하여 그 상태를 반환한다
/// @param      void
/// @return     return : Comp 동작 전 지연 시간 경과 여부 - 0(미완료), 1(완료)
U8 Get_CompOnDelayTimeEnd(void)
{
    if (gu16CoolDelayTimer >= DELAY_TIME_BEFORE_COMP_ON)
    {   // 경과 시간 경과 완료
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Comp 동작 전 지연 경과 시간 확인 함수
/// @details    Comp 동작 전 지연 경과 시간을 확인하여 그 값을 반환한다
/// @param      void
/// @return     return : Comp 동작 전 지연 경과 시간(1Sec 단위)
U16 Get_CompOnDelayTime(void)
{
    return  gu16CoolDelayTimer;
}


/// @brief      외기 온도에 따라 냉각 제어 Step 결정 함수
/// @details    외기 온도 상태에 따라 냉각 제어 Step을 결정한다
/// @param      void
/// @return     void
void CoolStepCheck_AirTemp(void)
{
    U8 mu8AirError = 0;
    F32 mf32AirTemp = 0;

    mu8AirError = Get_ErrorStatus(ERROR_ID_AIR_TEMP_1_E43);     // 외기 온도 센서 에러 발생 여부 확인

    if (mu8AirError == TRUE)
    {   // 외기 온도 에러 발생시
        gu8ColdActStep = COOL_STEP_ID_AIR_TEMP_ERROR;
    }
    else
    {
        mf32AirTemp = Get_Temp(AIR_THERMISTOR_ID);          // 외기 온도 확인

        if (mf32AirTemp < BLDC_COMP_CONTROL_STEP1_ENV_TEMP)
        {   // 외기 10도 미만
            gu8ColdActStep = COOL_STEP_ID_STEP_1;
        }
        else if (mf32AirTemp < BLDC_COMP_CONTROL_STEP2_ENV_TEMP)
        {   // 외기 20도 미만
            gu8ColdActStep = COOL_STEP_ID_STEP_2;
        }
        else if (mf32AirTemp < BLDC_COMP_CONTROL_STEP3_ENV_TEMP)
        {   // 외기 30도 미만
            gu8ColdActStep = COOL_STEP_ID_STEP_3;
        }
        else
        {   // 외기 30도 이상
            gu8ColdActStep = COOL_STEP_ID_STEP_4;
        }
    }
}


/// @brief      외기 온도에 따라 냉각 제어 Step 확인 함수
/// @details    외기 온도 상태에 따른 냉각 제어 Step을 확인하여 반환한다
/// @param      void
/// @return     return : 외기 온도 상타에 따른 냉각 제어 Step - 0~4단계
U8 Get_CoolStep(void)
{
    return  gu8ColdActStep;
}


/// @brief      BLDC 냉각 콤프레셔 제어 함수(@100ms)
/// @details    BLDC Comp 제어 동작을 한다
/// @param      void
/// @return     void
void Lib_BLDC_Comp_Control(void)
{
    U8 mu8ManualTestStatus = 0;
    U8 mu8ColdOn = 0;
    U8 mu8CompOnDelayEnd = 0;
    U8 mu8ErrorAboutCold = 0;
    U8 mu8FirstLowLevel = 0;
    U8 mu8PowerSaveStatus = 0;
    F32 mf32ColdTemp = 0;
    U8 mu8CoolActMaxEnd = 0;
    U8 mu8CompOn = 0;

#if (NORMAL_HEATER_USE == USE)
    U8 mu8HeaterOn = 0;
#endif

    if (gu8CoolControlPeriodTimer > BLDC_COMP_CONTROL_TIME_PERIOD)
    {
        gu8CoolControlPeriodTimer = 0;

        BLDC_Comp_ActTimeCounter();                         // 냉각 제어 관련 시간 타이머 카운터 함수

        mu8ManualTestStatus = Get_Manual_BLDC_CompTest();   // 수동 테스트 동작 여부 확인
        mu8ColdOn = Get_ColdOnOff();                        // 냉수 기능 ON/OFF 여부 확인
        mu8CompOnDelayEnd = Get_CompOnDelayTimeEnd();       // 냉각 전 지연시간 완료 확인

        mu8ErrorAboutCold = Get_ColdError();                // 냉각 관련 에러 발생 여부 확인
        mf32ColdTemp = Get_Temp(COOL_THERMISTOR_1_ID);      // 냉수 온도 확인
        mu8FirstLowLevel = Get_FirstLowLevelCheck();        // 부팅 후 최초 저수위 감지 여부 확인

        mu8PowerSaveStatus = Get_PowerSaveMode();           // 동작중인 절전 모드 상태 확인
        mu8CoolActMaxEnd = Get_CompActMaxTimeEnd();         // 냉각 최대 동작 시간 경과 완료 확인
        mu8CompOn = Get_BLDC_Comp_Status();                 // Comp On/Off 상태 확인

#if (NORMAL_HEATER_USE == USE)
        mu8HeaterOn = Get_Heater_Status(HEATER_ID_HOT_TANK);    // 히터 On/Off 상태 확인
#endif

        if (mu8ManualTestStatus == FALSE)
        {   // 냉각 수동 테스트 진행 중이 아닌 경우
            if (mu8ColdOn == TRUE)
            {   // 냉각 기능 사용시
                if (mu8CompOnDelayEnd == TRUE)
                {   // Delay 완료 후
                    if (mu8CoolActMaxEnd == FALSE)
                    {   // 연속 기동 최대 시간 미경과시
                        if ( (mu8ColdOn == TRUE) && (mu8FirstLowLevel == TRUE) )
                        {   // 냉수 기능 ON, 최초 저수위 센서 감지 완료인 경우
                            CoolStepCheck_AirTemp();        // 외기 온도별 냉각 제어 단계 판정

                            if (mu8CompOn == FALSE)
                            {   // Comp 미동작 상태인 경우
                                if (mf32ColdTemp >= BLDC_COMP_FIRST_ACT_CHECK_COLD_TEMP)
                                {   // 초기 기동 제어인 경우
                                    gu8FirstCompAct = SET;
                                }
                                else
                                {
                                    gu8FirstCompAct = CLEAR;
                                }
                            }

                            if (mu8ErrorAboutCold == FALSE)
                            {   // 냉수 관련 에러 미 발생시
                                if (gu8FirstCompAct == SET)
                                {
                                    gf32ColdTargetLowTemp   = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gf32TempFirstCoolOff;
                                    gf32ColdTargetHighTemp  = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gf32TempFirstCoolOn;
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                                    gu16ColdControlRPM      = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gu16FirstControlRPM;
#else
                                    gu8ColdTargetHz         = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gu8FirstTargetHz;
#endif
                                    gu16CoolMoreActTimer    = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gu16FirstCoolOffDelay;
                                }
                                else
                                {   // 재기동인 경우
                                    gu8FirstCompAct = CLEAR;

                                    gf32ColdTargetLowTemp   = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gf32TempCoolOff;
                                    gf32ColdTargetHighTemp  = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gf32TempCoolOn;
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                                    gu16ColdControlRPM      = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gu16ControlRPM;
#else
                                    gu8ColdTargetHz         = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gu8TargetHz;
#endif
                                    gu16CoolMoreActTimer    = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gu16CoolOffDelay;
                                }

                                if (mf32ColdTemp > gf32ColdTargetHighTemp)
                                {   // 냉수 온도가 기준보다 높은 경우
                                    if (mu8CompOn == FALSE)
                                    {   // Comp 미동작 상태인 경우
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                                        Set_BLDC_Comp_RPM(gu16ColdControlRPM);
#else
                                        Set_BLDC_Comp_Hz(gu8ColdTargetHz);
#endif

#if (NORMAL_HEATER_USE == USE)
                                        if (mu8HeaterOn == FALSE)
                                        {   // 히터가 꺼져 있는 경우
                                            Set_HotOnDelay(10);         // 히터 ON delay 1초
                                        }
#endif
                                    }
                                }
                                else if (mf32ColdTemp < gf32ColdTargetLowTemp)
                                {   // 냉수 온도가 기준보다 낮은 경우
                                    if (mu8CompOn == TRUE)
                                    {   // Comp 동작 중이면
                                        if (!gu16CoolMoreActTimer)
                                        {
                                            gu16CoolDelayTimer = 0;
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                                            Set_BLDC_Comp_RPM(0);
#else
                                            Set_BLDC_Comp_Hz(0);
#endif
                                        }
                                        else
                                        {
                                            gu16CoolMoreActTimer--;
                                        }
                                    }
                                }
                                else
                                {

                                }
                            }
                            else
                            {   // 냉수 관련 에러 발생시
                                if (mu8CompOn == TRUE)
                                {   // Comp 동작 중이면
                                    gu16CoolDelayTimer = 0;
                                    gu16CoolMoreActTimer = 0;           // 냉각 추가 기동 시간 0
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                                    Set_BLDC_Comp_RPM(0);
#else
                                    Set_BLDC_Comp_Hz(0);
#endif
                                }
                            }
                        }
                        else
                        {   // 냉수 기능 OFF 또는 최초 저수위 센서 감지 미완료인 경우
                            if (mu8CompOn == TRUE)
                            {   // Comp 동작 중이면
                                gu16CoolDelayTimer = 0;
                                gu16CoolMoreActTimer = 0;           // 냉각 추가 기동 시간 0
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                                Set_BLDC_Comp_RPM(0);
#else
                                Set_BLDC_Comp_Hz(0);
#endif
                            }
                        }
                    }
                    else
                    {   // 연속 기동 최대 시간 완료시
                        if (mu8CompOn == TRUE)
                        {   // Comp 동작 중이면
                            gu16CoolDelayTimer = 0;
                            gu16CoolMoreActTimer = 0;           // 냉각 추가 기동 시간 0
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                            Set_BLDC_Comp_RPM(0);
#else
                            Set_BLDC_Comp_Hz(0);
#endif
                        }
                    }
                }
                else
                {   // 재냉각 Delay 중
                    gu16CoolMoreActTimer = 0;           // 냉각 추가 기동 시간 0
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                    Set_BLDC_Comp_RPM(0);
#else
                    Set_BLDC_Comp_Hz(0);
#endif
                }
            }
            else
            {   // 냉각 기능 미사용시
                if (mu8CompOn == TRUE)
                {   // Comp 동작 중이면
                    gu16CoolDelayTimer = 0;
                    gu16CoolMoreActTimer = 0;           // 냉각 추가 기동 시간 0
#if (BLDC_COMP_PWM_CONTROL_USE == USE)
                    Set_BLDC_Comp_RPM(0);
#else
                    Set_BLDC_Comp_Hz(0);
#endif
                }
            }

        }
    }
}

#endif


// Lib BLDC Comp Module ********************************************************************************************

/// @brief      Lib BLDC Comp Module Initilaize
/// @details    BLDC Comp 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_BLDC_Comp_Module_Initialize(void)
{
#if (BLDC_COMP_USE == USE)
    Lib_BLDC_Comp_Initialize();
#endif
}


/// @brief      Lib BLDC Comp Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 BLDC Comp 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_BLDC_Comp_Module_1ms_Control(void)
{
#if (BLDC_COMP_USE == USE)
    Comp_ControlTimer();
#endif
}


/// @brief      Lib BLDC Comp Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 BLDC Comp 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_BLDC_Comp_Module_Control(void)
{
#if (BLDC_COMP_USE == USE)
    Lib_BLDC_Comp_Control();
#endif
}

