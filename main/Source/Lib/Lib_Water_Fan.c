/// @file     Lib_Water_Fan.c
/// @date     2025/07/09
/// @author   Jaejin Ham
/// @brief    Fan ���� ���� file

#include "Global_Header.h"


U8 gu8CoolFanControlTimer = 0;                      /// @brief  �ð� Fan ��� ���� ���� �ֱ� Ÿ�̸�

#if (DC_FAN_COUNT > 0)
U8 gu8CoolFanOnOff = 0;                             /// @brief  �ð� Fan ���� ���� - 0(OFF), 1(ON)
#endif

#if (VOLT_CONTROL_DC_FAN_USE == USE)
U8 gu8CoolFanVolt = 0;                              /// @brief  �ð� Fan ���� ���� - 0 ~ 24V
U8 gu8TimerFanDelay = 0;                            /// @brief  �ð� Fan On/Off ��ȯ Delay
U8 gu8FanConMode = 0;                               /// @brief  �ð� Fan ���� ���
#endif


/// @brief      �ð� Fan ���� Initialize
/// @details    �ð� Fan ���� ���� ������ ��� �ʱ�ȭ�Ѵ�
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
/// @details    Cool Fan ��� ���� ���� �ֱ� Ÿ�̸� ī��Ʈ�� �Ѵ�
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
/// @brief      �ð� Fan ���� ���� �Լ�
/// @details    �ð� Fan ������ �����Ѵ�
/// @param      mu8OnOff : Fan ���� ���� ���� - 0(OFF), 1(ON)
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


/// @brief      �ð� Fan ���� ���� Ȯ�� �Լ�
/// @details    �ð� Fan ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return  : Fan ���� ���� ���� - 0(OFF), 1(ON)
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
/// @brief      �ð� Fan ���� ���� ���� �Լ�
/// @details    �ð� Fan ���� ������ �����Ѵ�
/// @param      mu8Volt : Fan ���� ����
/// @return     void
void Set_CoolFanVolt(U8 mu8Volt)
{
    gu8CoolFanVolt = mu8Volt;
}


/// @brief      �ð� Fan ���� ���� ���� Ȯ�� �Լ�
/// @details    �ð� Fan ���� ���� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return  : Fan ���� ����
U8 Get_CoolFanVolt(void)
{
    return  gu8CoolFanVolt;
}


/// @brief      �ð� Fan ���� ��� ���� �Լ�
/// @details    �ð� Fan ���� ��带 �����Ѵ�
/// @param      mu8Data : Fan ���� ���
/// @return     void
void Set_CoolFanActMode(U8 mu8Data)
{
    gu8FanConMode = mu8Data;
}


/// @brief      �ð� Fan ���� ��� Ȯ�� �Լ�
/// @details    �ð� Fan ���� ��� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return  : Fan ���� ���
U8 Get_CoolFanActMode(void)
{
    return  gu8FanConMode;
}
#endif


/// @brief      �ð� Fan ���� �Լ�(@While)
/// @details    �ð� Fan�� �����Ѵ�
/// @param      void
/// @return     void
void Lib_CoolFan_Control(void)
{
#if (DC_FAN_COUNT > 0)          // �ð� DC FAN ����(���� ���� ����)
    U8 mu8CoolFucStatus = 0;
#endif

#if (VOLT_CONTROL_DC_FAN_USE == USE)        // ���� ���� �ð� FAN
    F32 mf32HeatsinkTemp = 0;
    F32 mf32AirTemp = 0;
    U8 mu8PL_Prevent_FanOn = 0;
    U8 mu8TEM_ActMode = 0;
#endif

    if (gu8CoolFanControlTimer >= COOL_FAN_CONTROL_TIME_PERIOD)
    {
        gu8CoolFanControlTimer = 0;

#if (DC_FAN_COUNT > 0)          // �ð� DC FAN ����(���� ���� ����)
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

#if (VOLT_CONTROL_DC_FAN_USE == USE)        // ���� ���� �ð� FAN
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
                    {   // ���� ��� ���� ��
                        if(mf32AirTemp >= 20.0f)
                        {   // �ܱ� �µ� 20�� �̻�
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
/// @details    Cool Fan ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_CoolFan_Module_Initialize(void)
{
#if ( (NORMAL_COMP_USE == USE) || (BLDC_COMP_USE == USE) || (TEM_USE == USE) )
    Lib_CoolFan_Initialize();
#endif
}


/// @brief      Lib CoolFan Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� CoolFan ���� ��� Library
/// @param      void
/// @return     void
void Lib_CoolFan_Module_1ms_Control(void)
{
#if ( (NORMAL_COMP_USE == USE) || (BLDC_COMP_USE == USE) || (TEM_USE == USE) )
    CoolFanControlTimer();
#endif
}


/// @brief      Lib Cool Fan Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Cool Fan ���� ��� Library
/// @param      void
/// @return     void
void Lib_CoolFan_Module_Control(void)
{
#if ( (NORMAL_COMP_USE == USE) || (BLDC_COMP_USE == USE) || (TEM_USE == USE) )
    Lib_CoolFan_Control();
#endif
}
