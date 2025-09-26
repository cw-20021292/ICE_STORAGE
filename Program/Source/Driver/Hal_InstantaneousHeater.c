/// @file     Hal_Instantaneous_Heater.c
/// @date     2025/02/18
/// @author   Jaejin Ham
/// @brief    Instantaneous Heater Control file


#include "Global_Header.h"


#if (INSTANT_HEATER_USE == USE)

U8 gu8InstHeaterControlTimer = 0;                   /// @brief  순간온수 히터 제어 타이머

U8 gu8InstHeaterTriacControlCycle = 0;              /// @brief  Triac 제어 시간 사이클
U8 gu8InstHeaterTriacControlDuty = 0;               /// @brief  Triac Duty 제어 시간

// 순간 온수 히터 제어 관리 자료형
typedef struct {
    U8  gu8RelayOn;                                 /// @brief  순간 온수 Heater Relay Status(1:ON, 0:OFF)
    U8  gu8TriacDuty;                               /// @brief  순간 온수 Heater Triac Control Duty(0~100%)
    U8  gu8Delay;                                   /// @brief  순간 온수 Heater 제어 전 Delay 시간
}   SInstHeaterData_T;

SInstHeaterData_T   SInstHeaterControlData;


/// @brief      순간온수 히터 제어 관련 데이터 초기화
/// @details    순간온수 히터 제어 관련 데이터를 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Initialize(void)
{
    MEMSET( (void __FAR *)&SInstHeaterControlData, 0x00, sizeof(SInstHeaterData_T) );

    gu8InstHeaterControlTimer = 0;

    gu8InstHeaterTriacControlCycle = 0;
    gu8InstHeaterTriacControlDuty = 0;

    Hal_Inst_Heater_Relay_Off();
    Hal_Inst_Heater_Triac_Off();
}


/// @brief      순간 온수 히터 제어 타이머(@1ms interrupt)
/// @details    순간 온수 히터 제어 시간을 1ms 인터럽트 안에서 카운트한다
/// @param      void
/// @return     void
void InstHeaterControlTimer(void)
{
    if (gu8InstHeaterControlTimer < INST_HEATER_CONTROL_TIME_PERIOD)
    {
        gu8InstHeaterControlTimer++;
    }
}


/// @brief      순간 온수 히터 제어 함수(@While문)
/// @details    순간 온수 히터 제어를 Whlie문 안에서 처리한다
/// @param      void
/// @return     void
void InstHeaterControl(void)
{
    if (gu8InstHeaterControlTimer >= INST_HEATER_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8InstHeaterControlTimer = 0;

        if (SInstHeaterControlData.gu8Delay)
        {
            SInstHeaterControlData.gu8Delay--;
        }

        Hal_Inst_Heater_Control();
    }

}


/// @brief      순간온수 히터 제어값들을 설정한다
/// @details    순간온수 히터 제어를 하는 제어값들을 설정한다
/// @param      mu8Relay : Relay 제어 상태 - 1(On), 0(Off)
///             mu8duty : Triac Duty - 0 ~ 100 (%)
///             mu8DelayTime : 히터 제어 전 Delay Time
/// @return     void
void Set_Inst_HeaterControl(U8 mu8Relay, U8 mu8Duty, U8 mu8DelayTime)
{
    SInstHeaterControlData.gu8RelayOn = mu8Relay;
    SInstHeaterControlData.gu8TriacDuty = mu8Duty;
    SInstHeaterControlData.gu8Delay = mu8DelayTime;
}


/// @brief      순간온수 히터 Triac 제어 Duty 확인 함수
/// @details    순간온수 히터 Triac 제어를 하는 Duty제어값을 확인하여 반환한다
/// @param      void
/// @return     return : Triac Duty - 0 ~ 100 (%)
U8 Get_Inst_HeaterTriacDuty(void)
{
    return  SInstHeaterControlData.gu8TriacDuty;
}


/// @brief      순간온수 히터 제어 함수
/// @details    순간온수 히터를 제어한다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Control(void)
{
    if (SInstHeaterControlData.gu8Delay == 0)
    {
        Set_Inst_Heater_Relay(SInstHeaterControlData.gu8RelayOn);
        Set_Inst_Heater_Triac_Duty(SInstHeaterControlData.gu8TriacDuty);
    }
}


/// @brief      순간온수 히터 Relay 제어
/// @brief      순간온수 히터 Relay를 제어한다
/// @param      mu8OnOff : 순간온수 Relay 제어 상태 - 0(Off), 1(On)
/// @return     void
void Set_Inst_Heater_Relay(U8 mu8OnOff)
{
    if (mu8OnOff)
    {
        Hal_Inst_Heater_Relay_On();
    }
    else
    {
        Hal_Inst_Heater_Relay_Off();
    }
}


/// @brief      순간온수 히터 Triac 제어 함수
/// @details    순간온수 히터 Triac을 제어한다
/// @param      mu8duty : Triac Control Duty - 0~100
/// @return     void
void Set_Inst_Heater_Triac_Duty(U8 mu8Duty)
{
    if (mu8Duty >= 100)
    {
        gu8InstHeaterTriacControlDuty = INST_HEATER_TRIAC_CONTROL_CYCLE_TIME;
    }
    else
    {
        gu8InstHeaterTriacControlDuty = (U8)(mu8Duty * ((F32)(INST_HEATER_TRIAC_CONTROL_CYCLE_TIME) / 100.0f));
    }
}


/// @brief      순간온수 히터 제어 상태 확인 함수
/// @details    순간온수 히터 제어 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 순간온수 히터 제어 상태 - 3(Heater All On), 2(Heater Relay On), 1(Heater Triac On), 0(All Off)
U8 Get_Inst_Heater_Status(void)
{
    U8 mu8RelayStatus = 0;
    U8 mu8TriacStatus = 0;

    mu8RelayStatus = GET_STATUS_INST_HEATER_RELAY();
    mu8TriacStatus = GET_STATUS_INST_HEATER_TRIAC();

    if ( (mu8RelayStatus != 0) && (mu8TriacStatus != 0) )
    {
        return INST_HEATER_ALL_ON;
    }
    else if (mu8TriacStatus != 0)
    {
        return INST_HEATER_TRIAC_ON;
    }
    else if (mu8RelayStatus != 0)
    {
        return INST_HEATER_RELAY_ON;
    }
    else
    {
        return INST_HEATER_ALL_OFF;
    }
}


/// @brief      순간온수 히터 Relay ON 함수
/// @details    순간온수 히터 릴레이를 ON 시킨다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Relay_On(void)
{
    TURN_ON_INST_HEATER_RELAY();
}


/// @brief      순간온수 히터 Relay OFF 함수
/// @details    순간온수 히터 릴레이를 OFF 시킨다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Relay_Off(void)
{
    TURN_OFF_INST_HEATER_RELAY();
}


/// @brief      순간온수 히터 Triac ON 함수
/// @details    순간온수 히터 Triac을 ON 시킨다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Triac_On(void)
{
    TURN_ON_INST_HEATER_TRIAC();
}


/// @brief      순간온수 히터 Triac OFF 함수
/// @details    순간온수 히터 Triac을 OFF 시킨다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Triac_Off(void)
{
    TURN_OFF_INST_HEATER_TRIAC();
}


/// @brief      순간온수 Triac을 Duty 제어 함수(반드시 1ms Timer Interrupt에서 처리)
/// @details    순간온수 Triac을 Duty 제어에 맞게 전원 ON 시킨다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Triac_Duty_Control(void)
{
    if (gu8InstHeaterTriacControlDuty == 0)
    {
        Hal_Inst_Heater_Triac_Off();
    }
    else
    {
        if (gu8InstHeaterTriacControlCycle <= gu8InstHeaterTriacControlDuty)
        {
            Hal_Inst_Heater_Triac_On();
        }
        else
        {
            Hal_Inst_Heater_Triac_Off();
        }
    }

    if (++gu8InstHeaterTriacControlCycle > INST_HEATER_TRIAC_CONTROL_CYCLE_TIME)
    {
        gu8InstHeaterTriacControlCycle = 0;
    }
}


#endif


// Hal Heater Module *****************************************************************************************

/// @brief      Hal Inst Heater Initilaize Module
/// @details    순간온수 히터 제어 관련 변수를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Module_Initialize(void)
{
#if (INSTANT_HEATER_USE == USE)
    Hal_Inst_Heater_Initialize();           // 순간 온수 히터 관련 초기화
#endif
}


/// @brief      Hal Inst Heater Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 순간 온수 히터 제어와 관련된 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Module_1ms_Control(void)
{
#if (INSTANT_HEATER_USE == USE)
    InstHeaterControlTimer();               // 순간 온수 히터 제어 시간 Counter
    Hal_Inst_Heater_Triac_Duty_Control();   // 순간 온수 Triac Port 제어 함수
#endif
}


/// @brief      Hal Inst Heater Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 순간 온수 히터 제어를 한다
/// @param      void
/// @return     void
void Hal_Inst_Heater_Module_Control(void)
{
#if (INSTANT_HEATER_USE == USE)
    InstHeaterControl();                    // 순간 온수 히터 Control 함수
#endif
}

