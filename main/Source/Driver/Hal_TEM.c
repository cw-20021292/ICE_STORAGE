/// @file     Hal_TEM.c
/// @date     2025/04/10
/// @author   Jaejin Ham
/// @brief    TEM ���� file


#include "Global_Header.h"


#if (TEM_USE == USE)

U16 gu16TEM_ControlTimer = 0;           /// @brief  TEM ���� �ð� Ÿ�̸�

U8 gu8TEM_Voltage = 0;                  /// @brief  ���� �����ϴ� ���а�
U8 gu8TEM_Voltage_Backup = 0;           /// @brief  ���� ���� ���а�
U16 gu16TEM_TDR_Data = 0;               /// @brief  ���� ���� TDR �������Ͱ�
U16 gu16TEM_TDR_Data_Backup = 0;        /// @brief  ���� ���� TDR �������Ͱ�
U16 gu16TEM_VAR_Delay_Timer = 0;        /// @brief  ���� ������ �ּ� ���� �ð� Timer

U8 gu8Timer_TEM_24V_OffDelay = 0;       /// @brief  TEM 24V ���� OFF Delay �ð�
U8 gu8Timer_TEM_12V_OffDelay = 0;       /// @brief  TEM 12V ���� OFF Delay �ð�


/// @brief      TEM ���� �ʱ�ȭ
/// @details    TEM ���� ���� �������� ��� �ʱ�ȭ �Ѵ�
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
/// @details    TEM ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void TEM_ControlTimer(void)
{
    if (gu16TEM_ControlTimer < TEM_CONTROL_TIME_PERIOD)
    {
        gu16TEM_ControlTimer++;
    }
}


/// @brief      TEM ���� ���� ����
/// @details    TEM ��� ������ ���а��� �����Ѵ�
/// @param      mu8Volt : TEM�� ���� ���� ���а� - 0 ~ 24V
/// @return     void
void Set_TEM_Voltage(U8 mu8Volt)
{
    gu8TEM_Voltage = mu8Volt;
}


/// @brief      TEM ���� ���� Ȯ��
/// @details    TEM ���� ���� ���а��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : TEM�� ���� ���� ���а� - 0 ~ 24V
U8 Get_TEM_Voltage(void)
{
    return  gu8TEM_Voltage;
}


/// @brief      TEM ���� PWM ���� �������Ͱ� ����
/// @details    TEM ���� ��� ������ PWM ���� �������Ͱ��� �����Ѵ�
/// @param      mu16Value : TEM�� ���� ���� ���ý��Ͱ�
/// @return     void
void Set_TEM_TDR(U16 mu16Value)
{
    gu16TEM_TDR_Data = mu16Value;
}


/// @brief      TEM ���� PWM ���� �������Ͱ� Ȯ��
/// @details    TEM ���� ��� ������ PWM ���� �������Ͱ��� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : TEM�� ���� ���� ���ý��Ͱ�
U16 Get_TEM_TDR(void)
{
    return  gu16TEM_TDR_Data;
}


/// @brief      TEM ���� ���� �Լ�(@While Loop) - SMPS�� ���� �ڵ带 �����ؾ� �� �� ����
/// @details    TEM ������ While�� �ȿ��� �����Ѵ�
/// @param      mu8Volt : TEM�� ���� ���� ���а�
/// @return     void
void Hal_TEM_Control(void)
{
    U16 mu16TEM_TDR_DataBuf = 0;

    if (gu16TEM_ControlTimer >= TEM_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ����
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

        // 1) 24V �� 5 ~ 12V PWM ��� ��ȯ
        // 2) 5 ~ 12V �� 24V ��� ��ȯ
        //    �� ��� 2���� ���ǿ��� �� ���, �ּ� 1ms ��� ������ �ʿ�(����ȸ�ΰ����� Ȯ��)
        //  ��� �� �ּ� 1�� ����.
        if (gu8TEM_Voltage != gu8TEM_Voltage_Backup)
        {   // TEM ���� ��� ���� ����� �ٸ� ���
            if (gu16TEM_VAR_Delay_Timer)
            {   // �ּ� ���� �ð� ������ ���� ���� ����
                gu16TEM_VAR_Delay_Timer--;

                mu16TEM_TDR_DataBuf = gu16TEM_TDR_Data_Backup;
            }
            else
            {   // ���� �ð� ���� ����
                gu16TEM_VAR_Delay_Timer = TEM_VOLTAGE_CHANGE_DELAY_TIME;    // �ּ� 1�а� ����

                gu8TEM_Voltage_Backup = gu8TEM_Voltage;                     //  ��� ���� �� backup
                gu16TEM_TDR_Data_Backup = mu16TEM_TDR_DataBuf;

            }
        }
        else
        {   // ���� ����� ���� ���
            gu16TEM_VAR_Delay_Timer = TEM_VOLTAGE_CHANGE_DELAY_TIME;    // �ּ� 1�а� ����
            gu8TEM_Voltage_Backup = gu8TEM_Voltage;
            gu16TEM_TDR_Data_Backup = mu16TEM_TDR_DataBuf;
        }

        //  Voltage�� ����, ������� ��� ����
        // 1) 24V ��� :
        //    - TEM 24V ON/OFF ��Ʈ : ON
        //    - TEM 12V ON/OFF ��Ʈ : OFF
        // 2) 5 ~ 12V ��� :
        //    - TEM 24V ON/OFF ��Ʈ : OFF
        //    - TEM 12V ON/OFF ��Ʈ : ON
        // 24V Port�� 12V Port ��ȯ�� 100ms ���� �ð� ����
        if (gu8TEM_Voltage == TEM_V_ID_24V)
        {   // 24V�� ���
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
/// @details    TEM ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_TEM_Module_Initialize(void)
{
#if (TEM_USE == USE)
    Hal_TEM_Initialize();                   // TEM ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal TEM Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� TEM ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_TEM_Module_1ms_Control(void)
{
#if (TEM_USE == USE)
    TEM_ControlTimer();                     // TEM ���� �ð� Counter
#endif
}


/// @brief      Hal TEM Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� TEM ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_TEM_Module_Control(void)
{
#if (TEM_USE == USE)
    Hal_TEM_Control();                      // TEM ����
#endif
}

