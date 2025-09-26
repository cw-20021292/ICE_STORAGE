/// @file     Lib_Water_Fan.c
/// @date     2025/07/09
/// @author   Jaejin Ham
/// @brief    Fan 동작 제어 file

#include "Global_Header.h"


U8 gu8CoolFanControlTimer = 0;                      /// @brief  냉각 Fan 제어를 위한 제어 주기 타이머

#if (DC_FAN_COUNT > 0)
U8 gu8CoolFanOnOff = 0;                             /// @brief  냉각 Fan 동작 상태 - 0(OFF), 1(ON)
#endif

#if (VOLT_CONTROL_DC_FAN_USE == USE)
U8 gu8CoolFanVolt = 0;                              /// @brief  냉각 Fan 동작 전압 - 0 ~ 24V
U8 gu8TimerFanDelay = 0;                            /// @brief  냉각 Fan On/Off 전환 Delay
U8 gu8FanConMode = 0;                               /// @brief  냉각 Fan 제어 모드
#endif


/// @brief      냉각 Fan 제어 Initialize
/// @details    냉각 Fan 제어 관련 변수를 모두 초기화한다
/// @param      void
/// @return     void
void Lib_CoolFan_Initialize(void)
{
#if (DC_FAN_COUNT > 0)
    gu8CoolFanOnOff = 0;
#endif

#if (VOLT_CONTROL_DC_FAN_USE == USE)
    gu8CoolFanVolt = 0;
    gu8TimerFanDelay = 0;
    gu8FanConMode = 0;
#endif
}


/// @brief      Cool Fan Control Timer(@1ms)
/// @details    Cool Fan 제어를 위한 제어 주기 타이머 카운트를 한다
/// @param      void
/// @return     void
void CoolFanControlTimer(void)
{
    if (gu8CoolFanControlTimer < COOL_FAN_CONTROL_TIME_PERIOD)
    {
        gu8CoolFanControlTimer++;
    }
}


#if (DC_FAN_COUNT > 0)
/// @brief      냉각 Fan 동작 설정 함수
/// @details    냉각 Fan 동작을 설정한다
/// @param      mu8OnOff : Fan 동작 설정 상태 - 0(OFF), 1(ON)
/// @return     void
void Set_CoolFanOnOff(U8 mu8OnOff)
{
    if (mu8OnOff)
    {
        gu8CoolFanOnOff = ON;
    }
    else
    {
        gu8CoolFanOnOff = OFF;
    }
}


/// @brief      냉각 Fan 동작 설정 확인 함수
/// @details    냉각 Fan 동작 설정 상태를 확인하여 반환한다
/// @param      void
/// @return     return  : Fan 동작 설정 상태 - 0(OFF), 1(ON)
U8 Get_CoolFanOnOff(void)
{
    if (gu8CoolFanOnOff)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}
#endif


#if (VOLT_CONTROL_DC_FAN_USE == USE)
/// @brief      냉각 Fan 동작 전압 설정 함수
/// @details    냉각 Fan 동작 전압을 설정한다
/// @param      mu8Volt : Fan 동작 전압
/// @return     void
void Set_CoolFanVolt(U8 mu8Volt)
{
    gu8CoolFanVolt = mu8Volt;
}


/// @brief      냉각 Fan 동작 전압 설정 확인 함수
/// @details    냉각 Fan 동작 전압 설정 상태를 확인하여 반환한다
/// @param      void
/// @return     return  : Fan 동작 전압
U8 Get_CoolFanVolt(void)
{
    return  gu8CoolFanVolt;
}


/// @brief      냉각 Fan 동작 모드 설정 함수
/// @details    냉각 Fan 동작 모드를 설정한다
/// @param      mu8Data : Fan 동작 모드
/// @return     void
void Set_CoolFanActMode(U8 mu8Data)
{
    gu8FanConMode = mu8Data;
}


/// @brief      냉각 Fan 동작 모드 확인 함수
/// @details    냉각 Fan 동작 모드 상태를 확인하여 반환한다
/// @param      void
/// @return     return  : Fan 동작 모드
U8 Get_CoolFanActMode(void)
{
    return  gu8FanConMode;
}
#endif


/// @brief      냉각 Fan 제어 함수(@While)
/// @details    냉각 Fan을 제어한다
/// @param      void
/// @return     void
void Lib_CoolFan_Control(void)
{
#if (DC_FAN_COUNT > 0)          // 냉각 DC FAN 사용시(전압 가변 없음)
    U8 mu8CoolFucStatus = 0;
#endif

#if (VOLT_CONTROL_DC_FAN_USE == USE)        // 전압 가변 냉각 FAN
    F32 mf32HeatsinkTemp = 0;
    F32 mf32AirTemp = 0;
    U8 mu8PL_Prevent_FanOn = 0;
    U8 mu8TEM_ActMode = 0;
#endif

    if (gu8CoolFanControlTimer >= COOL_FAN_CONTROL_TIME_PERIOD)
    {
        gu8CoolFanControlTimer = 0;

#if (DC_FAN_COUNT > 0)          // 냉각 DC FAN 사용시(전압 가변 없음)
#if (NORMAL_COMP_USE == USE)
        mu8CoolFucStatus = Get_Comp_Status();
#endif

#if (BLDC_COMP_USE == USE)
        mu8CoolFucStatus = Get_BLDC_Comp_Status();
#endif

        if (mu8CoolFucStatus == TRUE)
        {
            Set_DC_Fan_Control(DC_FAN_ID_COOLING, ON);
        }
        else
        {
            Set_DC_Fan_Control(DC_FAN_ID_COOLING, OFF);
        }
#endif

#if (VOLT_CONTROL_DC_FAN_USE == USE)        // 전압 가변 냉각 FAN
        if (gu8TimerFanDelay)
        {
            gu8TimerFanDelay--;
        }

        mf32HeatsinkTemp = Get_Temp(ADC_ID_TH_HEATSINK);

        switch (gu8FanConMode)
        {
            case FAN_ID_OFF:
                if (mf32HeatsinkTemp > 80.0f)
                {
                    mu8PL_Prevent_FanOn = SET;
                }
                else if (mf32HeatsinkTemp <= 60.0f)
                {
                    mu8PL_Prevent_FanOn = CLEAR;
                }
                else
                {
                }

                if (mu8PL_Prevent_FanOn == SET)
                {
                    if (gu8TimerFanDelay == 0)
                    {
                        TURN_ON_DC_FAN_18V();
                        TURN_OFF_DC_FAN_21V();
                    }
                    else
                    {
                        TURN_OFF_DC_FAN_18V();
                        TURN_OFF_DC_FAN_21V();

                    }
                }
                else
                {
                    TURN_OFF_DC_FAN_18V();
                    TURN_OFF_DC_FAN_21V();
                    gu8TimerFanDelay = FAN_DELAY_TIME;
                }
                break;

            case FAN_ID_18V_ON:
                if (gu8TimerFanDelay == 0)
                {
                    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);
                    mu8TEM_ActMode = Get_TEM_ActMode();

                    if(mu8TEM_ActMode == TEM_MODE_ID_RETAIN)
                    {   // 유지 모드 동작 중
                        if(mf32AirTemp >= 20.0f)
                        {   // 외기 온도 20도 이상
                            TURN_ON_DC_FAN_18V();
                            TURN_ON_DC_FAN_21V();
                        }
                        else
                        {
                            TURN_OFF_DC_FAN_18V();
                            TURN_OFF_DC_FAN_21V();
                        }
                    }
                    else
                    {
                        TURN_ON_DC_FAN_18V();
                        TURN_ON_DC_FAN_21V();
                    }
                }
                break;

            case FAN_ID_21V_ON:
                if (gu8TimerFanDelay == 0)
                {
                    TURN_ON_DC_FAN_18V();
                    TURN_OFF_DC_FAN_21V();
                }
                break;

            default:
                if (mf32HeatsinkTemp > 80.0f)
                {
                    mu8PL_Prevent_FanOn = SET;
                }
                else if (mf32HeatsinkTemp <= 60.0f)
                {
                    mu8PL_Prevent_FanOn = CLEAR;
                }
                else
                {
                }

                if (mu8PL_Prevent_FanOn == SET)
                {
                    if (gu8TimerFanDelay == 0)
                    {
                        TURN_ON_DC_FAN_18V();
                        TURN_OFF_DC_FAN_21V();
                    }
                    else
                    {
                        TURN_OFF_DC_FAN_18V();
                        TURN_OFF_DC_FAN_21V();

                    }
                }
                else
                {
                    TURN_OFF_DC_FAN_18V();
                    TURN_OFF_DC_FAN_21V();
                    gu8TimerFanDelay = FAN_DELAY_TIME;
                }
                break;
        }
#endif
    }
}


// Lib Fan Module ********************************************************************************************

/// @brief      Lib Cool Fan Module Initilaize
/// @details    Cool Fan 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_CoolFan_Module_Initialize(void)
{
#if ( (NORMAL_COMP_USE == USE) || (BLDC_COMP_USE == USE) || (TEM_USE == USE) )
    Lib_CoolFan_Initialize();
#endif
}


/// @brief      Lib CoolFan Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 CoolFan 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_CoolFan_Module_1ms_Control(void)
{
#if ( (NORMAL_COMP_USE == USE) || (BLDC_COMP_USE == USE) || (TEM_USE == USE) )
    CoolFanControlTimer();
#endif
}


/// @brief      Lib Cool Fan Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Cool Fan 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_CoolFan_Module_Control(void)
{
#if ( (NORMAL_COMP_USE == USE) || (BLDC_COMP_USE == USE) || (TEM_USE == USE) )
    Lib_CoolFan_Control();
#endif
}
