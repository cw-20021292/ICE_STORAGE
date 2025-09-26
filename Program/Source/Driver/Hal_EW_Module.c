/// @file     Hal_EW_Module.c
/// @date     2025/03/12
/// @author   Jaejin Ham
/// @brief    EW ��� ��� ���� ���� ���� file

#include "Global_Header.h"


#if (EW_MODULE_USE == 1)


U8 gu8EW_ControTimer = 0;                       /// @brief  EW ��� ���� Ÿ�̸�
U8 gu8EW_Status = 0;                            /// @brief  EW ��� ���� ���� - 0(OFF), 1(ON)


/// @brief      EW Initialize
/// @details    EW ��� ����� ���õ� ������ �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_EW_Initialize(void)
{
    gu8EW_ControTimer = 0;
    gu8EW_Status = 0;
}


/// @brief      EW control Set function
/// @details    EW ��� ���� ���¸� �����Ѵ�
/// @param      mu8Status : 1(ON), 0(OFF)
/// @return     void
void Set_EW_Control(U8 mu8Status)
{
    gu8EW_Status = mu8Status;
}


/// @brief      EW control Timer(@1ms interrupt)
/// @details    EW ��� ���� �ֱ� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void EW_ControlTimer(void)
{
    if (gu8EW_ControTimer < EW_CONTROL_TIME_PERIOD)
    {       // 10ms ���� ó��
        gu8EW_ControTimer++;
    }
}


/// @brief      EW control function(@While)
/// @details    EW ��� ��� �Ѵ�
/// @param      void
/// @return     void
void EW_Control(void)
{
    if (gu8EW_ControTimer >= EW_CONTROL_TIME_PERIOD)
    {       // 10ms ���� ó��
        gu8EW_ControTimer = 0;

        Hal_EW_Control();
    }
}


/// @brief      EW Control �Լ�(10ms)
/// @details    EW ��� ���� ���¿� ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void Hal_EW_Control(void)
{
#if (EW_PWM_CONTROL_USE == EW_PWM_CON_REGISTER_USE)
    U8 mu8Status = 0;
    U16 mu16EW_I_FB_AD = 0;
    U16 mu16EW_V_FB_AD = 0;
#endif

    if (gu8EW_Status)
    {
        TURN_ON_EW();
#if (EW_PWM_CONTROL_USE == EW_PWM_CON_TRIG_USE)
        TURN_ON_EW_TRIG();
#endif
    }
    else
    {
#if (EW_PWM_CONTROL_USE == EW_PWM_CON_TRIG_USE)
        TURN_OFF_EW_TRIG();
#endif
        TURN_OFF_EW();
    }

#if (EW_PWM_CONTROL_USE == EW_PWM_CON_REGISTER_USE)
    // EW ���� ����
    mu8Status = GET_STATUS_EW();

    if (mu8Status == ON)
    {
        EW_MODULE_PWM_START();

        mu16EW_V_FB_AD = Get_ADC_Data(EW_MODULE_V_FB_ID);
        mu16EW_I_FB_AD = Get_ADC_Data(EW_MODULE_I_FB_ID);

        if (mu16EW_V_FB_AD < 150)    // 300TDS, �� 90mA ~ 100mA
        {
            if (mu16EW_I_FB_AD < 210)
            {
                if (EW_PWM_REGISTER < REGISTER_EW_MODULE_PWM_MAX_VALUE)
                {
                    EW_PWM_REGISTER++;
                }
            }

            if (mu16EW_I_FB_AD > 230)
            {
                if (EW_PWM_REGISTER)
                {
                    EW_PWM_REGISTER--;
                }
            }
        }
        else if ((mu16EW_V_FB_AD >= 150) && (mu16EW_V_FB_AD < 250))   // 100TDS, �� 100mA ~ 130mA
        {
            if (mu16EW_I_FB_AD < 300)
            {
                if (EW_PWM_REGISTER < REGISTER_EW_MODULE_PWM_MAX_VALUE)
                {
                    EW_PWM_REGISTER++;
                }
            }

            if (mu16EW_I_FB_AD > 320)
            {
                if (EW_PWM_REGISTER)
                {
                    EW_PWM_REGISTER--;
                }
            }
        }
        else                // 50TDS, �� 130mA ~ 150mA
        {
            if (mu16EW_I_FB_AD < 340)
            {
                if (EW_PWM_REGISTER < REGISTER_EW_MODULE_PWM_MAX_VALUE)
                {
                    EW_PWM_REGISTER++;
                }
            }

            if (mu16EW_I_FB_AD > 360)
            {
                if (EW_PWM_REGISTER)
                {
                    EW_PWM_REGISTER--;
                }
            }
        }

        if ( (EW_PWM_REGISTER == 0) && (mu16EW_I_FB_AD > 600) )
        {
            TURN_OFF_EW();
        }
    }
    else
    {
        EW_MODULE_PWM_STOP();
        EW_PWM_REGISTER = REGISTER_EW_MODULE_PWM_INITIAL_VALUE;  // PWM ����, �ʱ�ġ
    }

#endif
}


#endif


// Hal EW Module *********************************************************************************************

/// @brief      Hal EW Initilaize Module
/// @details    EW ��� ��� ����� ���õ� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_EW_Module_Initialize(void)
{
#if (EW_MODULE_USE == USE)
    Hal_EW_Initialize();                    // EW ��� ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal EW Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� EW ��� ��� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_EW_Module_1ms_Control(void)
{
#if (EW_MODULE_USE == USE)
    EW_ControlTimer();                      // EW ��� ���� �ð� Counter
#endif
}


/// @brief      Hal EW Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� EW ��� ��� ������ �����Ѵ�
/// @param      void
/// @return     void
void Hal_EW_Module_Control(void)
{
#if (EW_MODULE_USE == USE)
    EW_Control();                           // EW ��� ���� �Լ�
#endif
}

