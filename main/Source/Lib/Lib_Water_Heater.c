/// @file     Lib_Water_Heater.c
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    Heat Tank 동작 제어 file

#include "Global_Header.h"


#if (NORMAL_HEATER_USE == USE)                     // 히터를 사용하는 경우


/// @brief      히팅 제어 온도 기준 데이터 구성
/// @details    히팅 제어를 하기 위한 기준 온도 데이터들을 구조체로 선언한다
typedef struct
{
    F32 gf32TempHeatOff;                    /// @brief  히터 OFF 온도(섭씨값, 소수점)
    F32 gf32TempHeatOn;                     /// @brief  히터 ON 온도(섭씨값, 소수점)
}   SHeatTemp_T;


/// @brief      스마트 미사용 절전 동작시 히팅 제어 온도 기준 테이블
/// @details    스마트 미사용 절전 동작시 고도 모드별 히팅 On, Off 기준 온도값을 세팅한다
__far const SHeatTemp_T  SSmartPowerSaveHeatTempData[HEIGHT_MODE_COUNT] = {
//  히터 OFF,  히터 ON
    {87.0f,     60.0f},     // 고도모드 1
    {82.0f,     60.0f},     // 고도모드 2
    {79.0f,     60.0f},     // 고도모드 3
    {76.0f,     60.0f},     // 고도모드 4
};


/// @brief      절전 동작시 히팅 제어 온도 기준 테이블
/// @details    절전 동작시 고도 모드별 히팅 On, Off 기준 온도값을 세팅한다
__far const SHeatTemp_T  SPowerSaveHeatTempData[HEIGHT_MODE_COUNT] = {
//  히터 OFF,  히터 ON
    {93.0f,     70.0f},     // 고도모드 1
    {88.0f,     70.0f},     // 고도모드 2
    {85.0f,     70.0f},     // 고도모드 3
    {82.0f,     70.0f},     // 고도모드 4
};


/// @brief      히팅 제어 온도 기준 테이블
/// @details    고도 모드별 히팅 On, Off 기준 온도값을 세팅한다
__far const SHeatTemp_T  SHeatTempData[HEIGHT_MODE_COUNT] = {
//  히터 OFF,   히터 ON
    {93.0f,     83.0f},     // 고도모드 1
    {88.0f,     80.0f},     // 고도모드 2
    {85.0f,     77.0f},     // 고도모드 3
    {82.0f,     74.0f},     // 고도모드 4
};


/// @brief      히팅 제어 데이터 구성
/// @details    히팅 제어를 하기 위한 관련 데이터들을 구조체로 선언한다
typedef struct
{
    U8 gu8HeightMode;                       /// @brief  고도 모드 단계(0~3)
    U8 gu8FuncStatus;                       /// @brief  온수 기능 설정 상태
    F32 gf32TargetHighTemp;                 /// @brief  히팅 목표 상한 온도(섭씨값, 소수점)
    F32 gf32TargetLowTemp;                  /// @brief  히팅 목표 하한 온도(섭씨값, 소수점)
    U8 gu8LowLevelDelayTime;                /// @brief  저수위시 히터 가동 지연 시간
    U8 gu8OnDelayTime;                      /// @brief  히터 On 전 지연 타이머
    U8 gu8OffDelayTime;                     /// @brief  히터 Off 전 지연 타이머
}   SHotData_T;

SHotData_T  SHotData;

U8 gu8HeatTankControlTimer = 0;             /// @brief  온수 탱크 제어 주기 타이머


/// @brief      Heat Tank 동작 관련 Flag & 변수 초기화
/// @details    Heat Tank 제어에 사용되는 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_HeatTank_Initialize(void)
{
    MEMSET( (void __FAR *)&SHotData, 0x00, sizeof(SHotData_T) );

    gu8HeatTankControlTimer = 0;                    // 온수 탱크 제어 주기 타이머
    SHotData.gu8HeightMode = HEIGHT_MODE_1;         // 고도 모드 1 default
    SHotData.gu8FuncStatus = HOT_ON;                // 온수 기능 설정 상태
    SHotData.gf32TargetHighTemp = SHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOff;    // 히팅 목표 상한 온도
    SHotData.gf32TargetLowTemp = SHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOn;      // 히팅 목표 하한 온도
    SHotData.gu8LowLevelDelayTime = 0;              // 저수위시 히터 가동 지연 시간
    SHotData.gu8OnDelayTime = 0;                    // 히터 On 전 지연 타이머
    SHotData.gu8OffDelayTime = 0;                   // 히터 Off 전 지연 타이머
}


/// @brief      Heat Tank Control Timer(@1ms)
/// @details    온수 탱크 히팅 제어를 위한 제어 주기 타이머 카운트를 한다
/// @param      void
/// @return     void
void HeatTankControlTimer(void)
{
    if (gu8HeatTankControlTimer < HEAT_TANK_CONTROL_TIME_PERIOD)
    {
        gu8HeatTankControlTimer++;
    }
}


/// @brief      온수 기능 ON/OFF 상태 설정 함수
/// @details    온수 기능 상태가 ON인지 OFF인지 설정한다
/// @param      mu8Status : 온수 기능 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_HotFuncStatus(U8 mu8Status)
{
    SHotData.gu8FuncStatus = mu8Status;
}


/// @brief      온수 기능 ON/OFF 상태 확인 함수
/// @details    온수 기능 상태가 ON인지 OFF 인지 확인하여 그 값을 반환한다
/// @param      void
/// @return     return : 온수 기능 설정 상태 - 0(OFF), 1(ON)
U8 Get_HotFuncStatus(void)
{
    U8 mu8Return = 0;

    mu8Return = SHotData.gu8FuncStatus;

    return  mu8Return;
}


/// @brief      온수 히팅 제어 시작전 Delay 시간 설정 함수
/// @details    온수 히팅 제어 시작 전에 일정시간 Delay가 있는 경우 그 시간을 설정한다
/// @param      mu8Time : 온수 히팅 제어 시작 전 Delay 시간(@100ms)
/// @return     void
void Set_HotOnDelay(U8 mu8Time)
{
    SHotData.gu8OnDelayTime = mu8Time;
}


/// @brief      저수위 상태 이후 온수 히팅 제어 시작전 Delay 시간 설정 함수
/// @details    저수위 상태 이후 다시 저수위 감지시 온수 히팅 제어 시작 전에 일정시간 Delay가 있는 경우 그 시간을 설정한다
/// @param      mu8Time : 온수 히팅 제어 시작 전 Delay 시간(@100ms)
/// @return     void
void Set_LowWaterHeatDelay(U8 mu8Time)
{
    SHotData.gu8LowLevelDelayTime = mu8Time;
}


/// @brief      고도 모드 설정 함수
/// @details    고도 모드를 설정한다
/// @param      mu8Step : 고도 모드 단계 - 0(1단계), 1(2단계), 2(3단계), 3(4단계)
/// @return     void
void Set_HeightMode(U8 mu8Step)
{
    SHotData.gu8HeightMode = mu8Step;
}


/// @brief      고도 모드 설정 상태 확인 함수
/// @details    고도 모드 설정 상태를 확인하여 반환한다.
/// @param      void
/// @return     return : 고도 모드 단계 - 0(1단계), 1(2단계), 2(3단계), 3(4단계)
U8 Get_HeightMode(void)
{
    U8 mu8Return = 0;

    mu8Return = SHotData.gu8HeightMode;

    return  mu8Return;
}


/// @brief      히터 상세 제어 함수(@While)
/// @details    히터 제어를 수행한다
/// @param      void
/// @return     void
void Lib_HeatTank_Control(void)
{
    U8 mu8AllKeyLock = 0;           // 전체 버튼 잠금 상태
    U8 mu8HotWaterLevel = 0;        // 온수 탱크 수위 상태
    U8 mu8HotError = 0;             // 온수와 관련된 에러 발생 상태
    F32 mf32HotTemp = 0;            // 온수 온도 센서 온도
    U8 mu8PowerSaveStatus = 0;      // 절전 동작 상태 : 0(미동작), 1(일반 절전), 2(미사용 절전)
    U8 mu8FirstHighLevel = 0;       // 부팅 후 최초 만수위 감지 여부

    if (gu8HeatTankControlTimer >= HEAT_TANK_CONTROL_TIME_PERIOD)
    {
        gu8HeatTankControlTimer = 0;

        mu8AllKeyLock = Get_AllKeyLockStatus();     // 전체 버튼 잠금 상태
        mu8HotWaterLevel = Get_HotWaterLevel();     // 온수 탱크의 수위 확인
        mu8HotError = Get_HotError();               // 온수와 관련된 에러 발생 상태 확인
//        mf32HotTemp = Get_Temp(ADC_ID_TH_HOT);      // 온수 온도 확인
        mu8PowerSaveStatus = Get_PowerSaveMode();   // 동작 중인 절전 모드 상태 확인
        mu8FirstHighLevel = Get_FirstHighLevelCheck();  // 부팅 후 최초 만수위 감지 여부 확인

        if (SHotData.gu8OnDelayTime)
        {   // 히터와 냉각이 동시에 켜지는 일이 없도록 동시 ON 상황에 히터 제어 동작 Delay
            SHotData.gu8OnDelayTime--;
            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);     // 제어 Delay 없이 탱크 히터 OFF
            return;
        }

        if (mu8AllKeyLock == ON)
        {   // 전체 잠금 상태
            SHotData.gu8LowLevelDelayTime = 0;      // 저수위시 히터 가동 지연 시간
            SHotData.gu8OnDelayTime = 0;            // 히터 On 전 지연 타이머
            SHotData.gu8OffDelayTime = 0;           // 히터 Off 전 지연 타이머
            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);     // 제어 Delay 없이 탱크 히터 OFF
            return;
        }

        if (SHotData.gu8FuncStatus == HOT_OFF)
        {   // 온수 기능이 꺼진 상태
            SHotData.gu8LowLevelDelayTime = 0;      // 저수위시 히터 가동 지연 시간
            SHotData.gu8OnDelayTime = 0;            // 히터 On 전 지연 타이머
            SHotData.gu8OffDelayTime = 0;           // 히터 Off 전 지연 타이머
            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);     // 제어 Delay 없이 탱크 히터 OFF
            return;
        }


        if (mu8HotWaterLevel == HOT_WATER_LEVEL_LOW)
        {   // 온수 탱크가 저수위인 경우
            Set_LowWaterHeatDelay(HEAT_ON_DELAY_TIME_LOW_LEVEL);        // 히팅 지연 10초(100 = 10 X 10(100ms))
            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);             // 제어 Delay 없이 탱크 히터 OFF
        }
        else
        {   // 온수 탱크가 저수위가 아닌 경우
            if (SHotData.gu8LowLevelDelayTime)
            {   // 저수위 상태 발생이 있어서 히터 가동 딜레이가 있는 경우
                SHotData.gu8LowLevelDelayTime--;
                Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);             // 제어 Delay 없이 탱크 히터 OFF
            }
            else
            {
                if (mu8FirstHighLevel == SET)
                {   // 최초 만수위 감지 이후 제어
                    if (mu8HotError == FALSE)
                    {   // 히팅 동작 불가 조건 에러 미발생시
                        if (mu8PowerSaveStatus == POWERSAVE_ID_SMART)
                        {
                            SHotData.gf32TargetHighTemp = SSmartPowerSaveHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOff;
                            SHotData.gf32TargetLowTemp = SSmartPowerSaveHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOn;
                        }
                        else if (mu8PowerSaveStatus == POWERSAVE_ID_NORMAL)
                        {   // 절전 모드 상태인 경우
                            SHotData.gf32TargetHighTemp = SPowerSaveHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOff;
                            SHotData.gf32TargetLowTemp = SPowerSaveHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOn;
                        }
                        else
                        {   // 일반 동작 모드
                            SHotData.gf32TargetHighTemp = SHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOff;
                            SHotData.gf32TargetLowTemp = SHeatTempData[SHotData.gu8HeightMode].gf32TempHeatOn;
                        }
                        // 히터 제어
                        if (mf32HotTemp > SHotData.gf32TargetHighTemp)
                        {   // 목표 온도보다 높은 경우 Heater OFF
                            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);         // 제어 Delay 없이 탱크 히터 OFF
                        }
                        else if (mf32HotTemp <= SHotData.gf32TargetLowTemp)
                        {   // 목표 온도보다 낮은 경우 Heater ON
                            Set_Heater_Control(HEATER_ID_HOT_TANK, ON, 0);          // 제어 Delay 없이 탱크 히터 ON
                        }
                        else
                        {   // 히터 OFF 온도와 히터 ON 온도 사이인 경우에는 이전 상태 유지
                        }
                        // 히팅 안전 제어
                        if (mf32HotTemp > 100.0f)
                        {   // 온수 온도가 100도가 넘으면
                            Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);         // 제어 Delay 없이 탱크 히터 OFF
                        }
                    }
                    else
                    {  // 히팅 동작 불가 조건 에러 발생시
                        Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);             // 제어 Delay 없이 탱크 히터 OFF
                    }
                }
                else
                {   // 최초 만수위 감지가 되지 않은 경우
                    Set_Heater_Control(HEATER_ID_HOT_TANK, OFF, 0);                 // 제어 Delay 없이 탱크 히터 OFF
                }
            }
        }
    }
}


#endif      // 일반형 히터 사용 여부 조건 END



// Lib HeatTank Module ********************************************************************************************

/// @brief      Lib Heater Module Initilaize
/// @details    일반 히터 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_Heater_Module_Initialize(void)
{
#if (NORMAL_HEATER_USE == USE)
    Lib_HeatTank_Initialize();
#endif
}


/// @brief      Lib Heater Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 Heater 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_Heater_Module_1ms_Control(void)
{
#if (NORMAL_HEATER_USE == USE)
    HeaterControlTimer();
#endif
}


/// @brief      Lib Heater Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Heater 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_Heater_Module_Control(void)
{
#if (NORMAL_HEATER_USE == USE)
    Lib_HeatTank_Control();
#endif
}

