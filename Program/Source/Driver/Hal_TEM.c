/// @file     Hal_TEM.c
/// @date     2025/04/10
/// @author   Jaejin Ham
/// @brief    TEM 제어 file


#include "Global_Header.h"


#if (TEM_USE == USE)

U16 gu16TEM_ControlTimer = 0;           /// @brief  TEM 제어 시간 타이머

U8 gu8TEM_Voltage = 0;                  /// @brief  현재 동작하는 전압값
U8 gu8TEM_Voltage_Backup = 0;           /// @brief  이전 동작 전압값
U16 gu16TEM_TDR_Data = 0;               /// @brief  현재 동작 TDR 레지스터값
U16 gu16TEM_TDR_Data_Backup = 0;        /// @brief  이전 동작 TDR 레지스터값
U16 gu16TEM_VAR_Delay_Timer = 0;        /// @brief  전압 변동전 최소 유지 시간 Timer

U8 gu8Timer_TEM_24V_OffDelay = 0;       /// @brief  TEM 24V 전원 OFF Delay 시간
U8 gu8Timer_TEM_12V_OffDelay = 0;       /// @brief  TEM 12V 전원 OFF Delay 시간


/// @brief      TEM 관련 초기화
/// @details    TEM 제어 관련 변수들을 모두 초기화 한다
/// @param      void
/// @return     void
void Hal_TEM_Initialize(void)
{
    gu16TEM_ControlTimer = 0;

    gu8TEM_Voltage = 0;
    gu8TEM_Voltage_Backup = 0;
    gu16TEM_TDR_Data = 0;
    gu16TEM_TDR_Data_Backup = 0;
    gu16TEM_VAR_Delay_Timer = 0;

    gu8Timer_TEM_24V_OffDelay = 0;
    gu8Timer_TEM_12V_OffDelay = 0;
}


/// @brief      TEM Control Timer(@1ms)
/// @details    TEM 제어 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void TEM_ControlTimer(void)
{
    if (gu16TEM_ControlTimer < TEM_CONTROL_TIME_PERIOD)
    {
        gu16TEM_ControlTimer++;
    }
}


/// @brief      TEM 동작 전압 설정
/// @details    TEM 제어를 진행할 전압값을 설정한다
/// @param      mu8Volt : TEM의 동작 설정 전압값 - 0 ~ 24V
/// @return     void
void Set_TEM_Voltage(U8 mu8Volt)
{
    gu8TEM_Voltage = mu8Volt;
}


/// @brief      TEM 동작 전압 확인
/// @details    TEM 제어 중인 전압값을 확인하여 반환한다
/// @param      void
/// @return     return : TEM의 동작 설정 전압값 - 0 ~ 24V
U8 Get_TEM_Voltage(void)
{
    return  gu8TEM_Voltage;
}


/// @brief      TEM 동작 PWM 제어 레지스터값 설정
/// @details    TEM 동작 제어를 진행할 PWM 제어 레지스터값을 설정한다
/// @param      mu16Value : TEM의 동작 제어 레시스터값
/// @return     void
void Set_TEM_TDR(U16 mu16Value)
{
    gu16TEM_TDR_Data = mu16Value;
}


/// @brief      TEM 동작 PWM 제어 레지스터값 확인
/// @details    TEM 동작 제어를 진행할 PWM 제어 레지스터값을 확인하여 반환한다
/// @param      void
/// @return     return : TEM의 동작 제어 레시스터값
U16 Get_TEM_TDR(void)
{
    return  gu16TEM_TDR_Data;
}


/// @brief      TEM 동작 제어 함수(@While Loop) - SMPS에 따라 코드를 수정해야 될 수 있음
/// @details    TEM 동작을 While문 안에서 제어한다
/// @param      mu8Volt : TEM의 동작 설정 전압값
/// @return     void
void Hal_TEM_Control(void)
{
    U16 mu16TEM_TDR_DataBuf = 0;

    if (gu16TEM_ControlTimer >= TEM_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 수행
        gu16TEM_ControlTimer = 0;

        if (gu8Timer_TEM_12V_OffDelay)
        {
            gu8Timer_TEM_12V_OffDelay--;
        }

        if (gu8Timer_TEM_24V_OffDelay)
        {
            gu8Timer_TEM_24V_OffDelay--;
        }

        mu16TEM_TDR_DataBuf = Get_TEM_TDR();

        // 1) 24V → 5 ~ 12V PWM 출력 전환
        // 2) 5 ~ 12V → 24V 출력 전환
        //    → 상기 2가지 조건에서 일 경우, 최소 1ms 출력 딜레이 필요(전장회로개발팀 확인)
        //  출력 값 최소 1분 유지.
        if (gu8TEM_Voltage != gu8TEM_Voltage_Backup)
        {   // TEM 전압 제어가 이전 제어와 다른 경우
            if (gu16TEM_VAR_Delay_Timer)
            {   // 최소 유지 시간 동안은 이전 제어 유지
                gu16TEM_VAR_Delay_Timer--;

                mu16TEM_TDR_DataBuf = gu16TEM_TDR_Data_Backup;
            }
            else
            {   // 유지 시간 이후 변경
                gu16TEM_VAR_Delay_Timer = TEM_VOLTAGE_CHANGE_DELAY_TIME;    // 최소 1분간 유지

                gu8TEM_Voltage_Backup = gu8TEM_Voltage;                     //  출력 전압 값 backup
                gu16TEM_TDR_Data_Backup = mu16TEM_TDR_DataBuf;

            }
        }
        else
        {   // 이전 제어와 같은 경우
            gu16TEM_VAR_Delay_Timer = TEM_VOLTAGE_CHANGE_DELAY_TIME;    // 최소 1분간 유지
            gu8TEM_Voltage_Backup = gu8TEM_Voltage;
            gu16TEM_TDR_Data_Backup = mu16TEM_TDR_DataBuf;
        }

        //  Voltage에 따른, 열전모듈 출력 제어
        // 1) 24V 출력 :
        //    - TEM 24V ON/OFF 포트 : ON
        //    - TEM 12V ON/OFF 포트 : OFF
        // 2) 5 ~ 12V 출력 :
        //    - TEM 24V ON/OFF 포트 : OFF
        //    - TEM 12V ON/OFF 포트 : ON
        // 24V Port와 12V Port 전환시 100ms 지연 시간 있음
        if (gu8TEM_Voltage == TEM_V_ID_24V)
        {   // 24V인 경우
            TURN_ON_TEM_24V();

            if (gu8Timer_TEM_12V_OffDelay == 0)
            {
                TURN_OFF_TEM_12V();
            }

            TEM_PWM_REGISTER = TEM_PWM_OFF_TDR;
            gu8Timer_TEM_24V_OffDelay = TEM_24V_12V_CHANGE_DELAY_TIME;
        }
        else if( (gu8TEM_Voltage >= TEM_V_ID_5V) && (gu8TEM_Voltage <= TEM_V_ID_12V) )
        {
            TURN_ON_TEM_12V();
            TEM_PWM_REGISTER = mu16TEM_TDR_DataBuf;

            if (gu8Timer_TEM_24V_OffDelay == 0)
            {
                TURN_OFF_TEM_24V();
            }
            else
            {
                TURN_ON_TEM_24V();
            }

            gu8Timer_TEM_12V_OffDelay = TEM_12V_24V_CHANGE_DELAY_TIME;
        }
        else
        {
            TURN_OFF_TEM_24V();
            TURN_OFF_TEM_12V();
            TEM_PWM_REGISTER = TEM_PWM_OFF_TDR;
            gu8TEM_Voltage = 0;
        }
    }
}

#endif


// Hal TEM Module ********************************************************************************************

/// @brief      Hal TEM Initilaize Module
/// @details    TEM 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_TEM_Module_Initialize(void)
{
#if (TEM_USE == USE)
    Hal_TEM_Initialize();                   // TEM 제어 관련 초기화
#endif
}


/// @brief      Hal TEM Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 TEM 제어 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_TEM_Module_1ms_Control(void)
{
#if (TEM_USE == USE)
    TEM_ControlTimer();                     // TEM 제어 시간 Counter
#endif
}


/// @brief      Hal TEM Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 TEM 제어를 한다
/// @param      void
/// @return     void
void Hal_TEM_Module_Control(void)
{
#if (TEM_USE == USE)
    Hal_TEM_Control();                      // TEM 제어
#endif
}

