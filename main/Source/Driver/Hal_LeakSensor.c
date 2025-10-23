/// @file     Hal_CoolingFan.c
/// @date     2025/02/21
/// @author   Jaejin Ham
/// @brief    Cooling Fan Control file


#include "Global_Header.h"


#if (LEAK_SENSOR_POWER_CONTROL == USE)


U8 gu8LeakSensorPower = 0;                      /// @brief  ���� ���� ���� ���� ���� ���� ���� - 0(OFF), 1(ON)


/// @brief      ���� ���� ���� �ʱ�ȭ �Լ�
/// @details    ���� ���� ����� ���õ� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_LeakSensor_Initialize(void)
{
    gu8LeakSensorPower = SET;
}


/// @brief      ���� ���� ���� �Լ�(@while)
/// @details    ���� ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void LeakSensorPowerControl(void)
{
    if (gu8LeakSensorPower)
    {
        TURN_ON_LEAK_SENSOR();
    }
    else
    {
        TURN_OFF_LEAK_SENSOR();
    }
}


/// @brief      ���� ���� ���� ���� ���� �Լ�
/// @details    ���� ���� ���� ���� ���°��� �����Ѵ�
/// @param      mu8OnOff : ���� ���� ���� ���� - 0(Off), 1(On)
/// @return     void
void Set_LeakSensor_Control(U8 mu8OnOff)
{
    gu8LeakSensorPower = mu8OnOff;
}


#endif


// Hal Leak Sensor Module ************************************************************************************

/// @brief      Hal Leak Sensor Initilaize Module
/// @details    ���� ���� ���� ���� ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_LeakSensor_Module_Initialize(void)
{
#if (LEAK_SENSOR_POWER_CONTROL == USE)
    Hal_LeakSensor_Initialize();            // ���� ���� ���� ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Leak Sensor Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� ���� ���� ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_LeakSensor_Module_Control(void)
{
#if (LEAK_SENSOR_POWER_CONTROL == USE)
    LeakSensorPowerControl();               // ���� ���� ���� ���� �Լ�
#endif
}


