/// @file     Func_Buzzer.c
/// @date     2025/04/09
/// @author   Jaejin Ham
/// @brief    Buzzer ���� ���� file

#include "Global_Header.h"

#if (BUZZER_USE == USE)


/// @brief      Buzzer ��� ���� Initialize
/// @details    Buzzer ��� ���� ���� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Buzzer_Initialize(void)
{
    Set_Buzzer_ON(0);
    Set_Buzzer_PWM(0);
}


/// @brief      Buzzer ��� �Լ�
/// @details    Buzzer ��� PWM�� �����Ѵ�
/// @param      mu16Value : Buzzer ���� PWM �������Ͱ�
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


/// @brief      Buzzer ��� ���� �Լ�
/// @details    Buzzer ��� PWM �������Ϳ� ���� �����Ͽ� ����Ѵ�
/// @param      mu16Value : Buzzer ���� PWM �������Ͱ�
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


/// @brief      Buzzer ON Port ���� �Լ�
/// @details    Buzzer On Port�� �����Ѵ�
/// @param      mu8OnOff : Buzzer ���� On/Off - 0(OFF), 1(ON)
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
/// @details    ���� ���� ���� ������ �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Buzzer_Module_Initialize(void)
{
#if (BUZZER_USE == USE)
    BuzzerInitialize();                     // ������ ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Buzzer Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ���� ��� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_Buzzer_Module_1ms_Control(void)
{
#if (BUZZER_USE == USE)
    BuzzerTimeCounter();                    // ���� ���� ���� �ð� Counter
    BuzzerControl();                        // ������ ��� ���� �Լ�
#endif
}

