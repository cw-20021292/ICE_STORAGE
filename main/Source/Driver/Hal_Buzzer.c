/// @file     Func_Buzzer.c
/// @date     2025/04/09
/// @author   Jaejin Ham
/// @brief    Buzzer 동작 제어 file

#include "Global_Header.h"

#if (BUZZER_USE == USE)


/// @brief      Buzzer 출력 관련 Initialize
/// @details    Buzzer 출력 제어 관련 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Buzzer_Initialize(void)
{
    Set_Buzzer_ON(0);
    Set_Buzzer_PWM(0);
}


/// @brief      Buzzer 출력 함수
/// @details    Buzzer 출력 PWM을 제어한다
/// @param      mu16Value : Buzzer 제어 PWM 레지스터값
/// @return     void
void Set_Buzzer_Out(U16 mu16Value)
{
    if (mu16Value)
    {
        Set_Buzzer_PWM(mu16Value);
        Set_Buzzer_ON(1);
    }
    else
    {
        Set_Buzzer_ON(0);
        Set_Buzzer_PWM(0);
    }
}


/// @brief      Buzzer 출력 제어 함수
/// @details    Buzzer 출력 PWM 레지스터에 값을 설정하여 출력한다
/// @param      mu16Value : Buzzer 제어 PWM 레지스터값
/// @return     void
void Set_Buzzer_PWM(U16 mu16Value)
{
    if (mu16Value)
    {
        BUZZER_REGISTER = mu16Value;
        BUZZER_START();
    }
    else
    {
        BUZZER_STOP();
    }
}


/// @brief      Buzzer ON Port 제어 함수
/// @details    Buzzer On Port를 제어한다
/// @param      mu8OnOff : Buzzer 전원 On/Off - 0(OFF), 1(ON)
/// @return     void
void Set_Buzzer_ON(U8 mu8OnOff)
{
    if (mu8OnOff)
    {
        TURN_ON_BUZZER();
    }
    else
    {
        TURN_OFF_BUZZER();
    }
}

#endif


// Hal Buzzer Module *****************************************************************************************

/// @brief      Hal Buzzer Initilaize Module
/// @details    부저 제어 관련 변수를 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Buzzer_Module_Initialize(void)
{
#if (BUZZER_USE == USE)
    BuzzerInitialize();                     // 부저음 제어 관련 초기화
#endif
}


/// @brief      Hal Buzzer Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 부저 출력 제어를 한다
/// @param      void
/// @return     void
void Hal_Buzzer_Module_1ms_Control(void)
{
#if (BUZZER_USE == USE)
    BuzzerTimeCounter();                    // 부저 제어 관련 시간 Counter
    BuzzerControl();                        // 부저음 출력 제어 함수
#endif
}

