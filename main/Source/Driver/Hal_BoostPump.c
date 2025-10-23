/// @file     Hal_BoostPump.c
/// @date     2025/03/10
/// @author   Jaejin Ham
/// @brief    Boost Pump Control file


#include "Global_Header.h"


#if (BOOST_PUMP_USE == USE)

U8 gu8BoostPumpControlTimer = 0;        /// @brief  �ν�Ʈ ���� ���� �ð� Ÿ�̸�
U8 gu8BoostPumpStatus = 0;              /// @brief  �ν�Ʈ ���� ���� ����
U8 gu8BoostPumpDelayTime = 0;           /// @brief  �ν�Ʈ ���� ���� ���� �� Delay �ð�
U8 gu8BoostInDetectTimer = 0;           /// @brief  �ν�Ʈ ���� ���� üũ Ÿ�̸�
U8 gu8BoostInUndetectTimer = 0;         /// @brief  �ν�Ʈ ���� �̿��� üũ Ÿ�̸�
U8 gu8BoostInStatus = 0;                /// @brief  �ν�Ʈ ���� ���� ����


/// @brief      Boost Pump Port Initialize
/// @details    �ν�Ʈ ���� ���� ���� ������ ��� �ʱ�ȭ ��Ų��
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
/// @details    �ν�Ʈ ���� ���� ���� �ֱ� Ÿ�̸Ӹ� ī��Ʈ ��Ų��
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
/// @details    �ν�Ʈ ���� ������ �����Ѵ�
/// @param      void
/// @return     void
void BoostPumpControl(void)
{
    U8 mu8BoostIn = 0;

    if (gu8BoostPumpControlTimer >= BOOST_PUMP_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8BoostPumpControlTimer = 0;

        BoostInCheck();

        mu8BoostIn = Get_BoostInStatus();

        if (mu8BoostIn == TRUE)
        {       // �ν�Ʈ ������ ������ ���� Ȯ���� ���
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
/// @details    �ν�Ʈ ���� ���� ���¸� üũ�Ѵ�
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


/// @brief      Boost Pump ���� ���� Ȯ�� �Լ�
/// @details    Boost Pump ���� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ν�Ʈ ���� ���� ���� - 0(�̿���), 1(����)
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
/// @details    �ν�Ʈ ���� ���� ��� �����Ѵ�
/// @param      mu8OnOff : �ν�Ʈ ���� ���� ���� - 1(On), 0(Off)
///             mu8Delay : �ν�Ʈ ���� ���� ���� �� Delay Time
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
/// @details    �ν�Ʈ ���� ���� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ν�Ʈ ���� ���� ���� - 1(On), 0(Off)
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
/// @details    �ν�Ʈ ���� ������ �����Ѵ�
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
/// @details    �ν�Ʈ ���� ���� ���� �������� �ʱ�ȭ ��Ų��.
/// @param      void
/// @return     void
void Hal_BoostPump_Module_Initialize(void)
{
#if (BOOST_PUMP_USE == USE)
    Hal_BoostPump_Initialize();             // �ν�Ʈ ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Boost Pump Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� �ν�Ʈ ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_BoostPump_Module_1ms_Control(void)
{
#if (BOOST_PUMP_USE == USE)
    BoostPumpControlTimer();                // Boost Pump ���� �ð� Counter
#endif
}


/// @brief      Hal Boost Pump Module in While Loop
/// @details    Basic Loop �� While�� �ȿ��� �ν�Ʈ ������ �����Ѵ�
/// @param      void
/// @return     void
void Hal_BoostPump_Module_Control(void)
{
#if (BOOST_PUMP_USE == USE)
    BoostPumpControl();                     // �ν�Ʈ ���� ���� �Լ�
#endif
}
