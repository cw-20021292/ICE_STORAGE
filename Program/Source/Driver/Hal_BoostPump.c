/// @file     Hal_BoostPump.c
/// @date     2025/03/10
/// @author   Jaejin Ham
/// @brief    Boost Pump Control file


#include "Global_Header.h"


#if (BOOST_PUMP_USE == USE)

U8 gu8BoostPumpControlTimer = 0;        /// @brief  부스트 펌프 제어 시간 타이머
U8 gu8BoostPumpStatus = 0;              /// @brief  부스트 펌프 동작 상태
U8 gu8BoostPumpDelayTime = 0;           /// @brief  부스트 펌프 동작 제어 전 Delay 시간
U8 gu8BoostInDetectTimer = 0;           /// @brief  부스트 펌프 연결 체크 타이머
U8 gu8BoostInUndetectTimer = 0;         /// @brief  부스트 펌프 미연결 체크 타이머
U8 gu8BoostInStatus = 0;                /// @brief  부스트 펌프 연결 상태


/// @brief      Boost Pump Port Initialize
/// @details    부스트 펌프 제어 관련 변수를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_BoostPump_Initialize(void)
{
    gu8BoostPumpControlTimer = 0;
    gu8BoostPumpStatus = 0;
    gu8BoostPumpDelayTime = 0;
    gu8BoostInDetectTimer = 0;
    gu8BoostInUndetectTimer = 0;
    gu8BoostInStatus = 0;

    Set_BoostPump(OFF, 0);
}


/// @brief      Boost Pump control Timer(@1ms interrupt)
/// @details    부스트 펌프 제어 관련 주기 타이머를 카운트 시킨다
/// @param      void
/// @return     void
void BoostPumpControlTimer(void)
{
    if (gu8BoostPumpControlTimer < BOOST_PUMP_CONTROL_TIME_PERIOD)
    {
        gu8BoostPumpControlTimer++;
    }
}


/// @brief      Boost Pump control Function(@While)
/// @details    부스트 펌프 동작을 제어한다
/// @param      void
/// @return     void
void BoostPumpControl(void)
{
    U8 mu8BoostIn = 0;

    if (gu8BoostPumpControlTimer >= BOOST_PUMP_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8BoostPumpControlTimer = 0;

        BoostInCheck();

        mu8BoostIn = Get_BoostInStatus();

        if (mu8BoostIn == TRUE)
        {       // 부스트 펌프가 연결이 감지 확정된 경우
            if (gu8BoostPumpDelayTime)
            {
                gu8BoostPumpDelayTime--;
            }
            else
            {
                gu8BoostPumpDelayTime = 0;
            }
        }
        else
        {
            Set_BoostPump(OFF, 0);
        }

        Hal_BoostPump_Control();
    }
}


/// @brief      Boost Pump In Check Function(100ms@1ms)
/// @details    부스트 펌프 연결 상태를 체크한다
/// @param      void
/// @return     void
void BoostInCheck(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_BOOST_IN();

    if (mu8Status)
    {
        gu8BoostInUndetectTimer = 0;

        if (gu8BoostInDetectTimer < BOOST_PUMP_IN_CHECK_TIME)
        {
            gu8BoostInDetectTimer++;
        }
        else
        {
            gu8BoostInStatus = SET;
        }
    }
    else
    {
        gu8BoostInDetectTimer = 0;

        if (gu8BoostInUndetectTimer < BOOST_PUMP_IN_CHECK_TIME)
        {
            gu8BoostInUndetectTimer++;
        }
        else
        {
            gu8BoostInStatus = CLEAR;

        }
    }
}


/// @brief      Boost Pump 연결 상태 확인 함수
/// @details    Boost Pump 연결 상태를 반환한다
/// @param      void
/// @return     return : 부스트 펌프 연결 상태 - 0(미연결), 1(연결)
U8 Get_BoostInStatus(void)
{
    if (gu8BoostInStatus)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Boost Pump control function
/// @details    부스트 펌프 동작 제어를 설정한다
/// @param      mu8OnOff : 부스트 펌프 동작 상태 - 1(On), 0(Off)
///             mu8Delay : 부스트 펌프 동작 제어 전 Delay Time
/// @return     void
void Set_BoostPump(U8 mu8OnOff, U8 mu8Delay)
{
    if (mu8OnOff == ON)
    {
        gu8BoostPumpStatus = ON;
        gu8BoostPumpDelayTime = mu8Delay;
    }
    else
    {
        gu8BoostPumpStatus = OFF;
        gu8BoostPumpDelayTime = mu8Delay;
    }
}


/// @brief      Check Boost Pump Status
/// @details    부스트 펌프 동작 상태를 반환한다
/// @param      void
/// @return     return : 부스트 펌프 동작 상태 - 1(On), 0(Off)
U8 Get_BoostPump_Status(void)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_BOOST_OUT();

    if (mu8Status)
    {
        mu8Return = TRUE;
    }
    else
    {
        mu8Return = FALSE;
    }

    return  mu8Return;
}


/// @brief      Boost Pump on function
/// @details    부스트 펌프 동작을 제어한다
/// @param      void
/// @return     void
void Hal_BoostPump_Control(void)
{
    if (gu8BoostPumpDelayTime == 0)
    {
        if (gu8BoostPumpStatus)
        {
            TURN_ON_BOOST_OUT();
        }
        else
        {
            TURN_OFF_BOOST_OUT();
        }
    }
}


#endif


// Hal Boost Pump Module *************************************************************************************

/// @brief      Hal Boost Pump Initilaize Module
/// @details    부스트 펌프 제어 관련 변수들을 초기화 시킨다.
/// @param      void
/// @return     void
void Hal_BoostPump_Module_Initialize(void)
{
#if (BOOST_PUMP_USE == USE)
    Hal_BoostPump_Initialize();             // 부스트 펌프 관련 초기화
#endif
}


/// @brief      Hal Boost Pump Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 부스트 펌프 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_BoostPump_Module_1ms_Control(void)
{
#if (BOOST_PUMP_USE == USE)
    BoostPumpControlTimer();                // Boost Pump 제어 시간 Counter
#endif
}


/// @brief      Hal Boost Pump Module in While Loop
/// @details    Basic Loop 안 While문 안에서 부스트 펌프를 제어한다
/// @param      void
/// @return     void
void Hal_BoostPump_Module_Control(void)
{
#if (BOOST_PUMP_USE == USE)
    BoostPumpControl();                     // 부스트 펌프 제어 함수
#endif
}
