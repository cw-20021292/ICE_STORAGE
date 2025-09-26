/// @file     Hal_CoolingFan.c
/// @date     2025/02/21
/// @author   Jaejin Ham
/// @brief    Cooling Fan Control file


#include "Global_Header.h"


#if (LEAK_SENSOR_POWER_CONTROL == USE)


U8 gu8LeakSensorPower = 0;                      /// @brief  누수 감지 센서 감지 전원 제어 상태 - 0(OFF), 1(ON)


/// @brief      누수 센서 제어 초기화 함수
/// @details    누수 센서 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_LeakSensor_Initialize(void)
{
    gu8LeakSensorPower = SET;
}


/// @brief      누수 센서 제어 함수(@while)
/// @details    누수 센서 전원 제어를 한다
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


/// @brief      누수 센서 전원 제어 설정 함수
/// @details    누수 센서 전원 제어 상태값을 설정한다
/// @param      mu8OnOff : 누수 센서 전원 상태 - 0(Off), 1(On)
/// @return     void
void Set_LeakSensor_Control(U8 mu8OnOff)
{
    gu8LeakSensorPower = mu8OnOff;
}


#endif


// Hal Leak Sensor Module ************************************************************************************

/// @brief      Hal Leak Sensor Initilaize Module
/// @details    누수 감지 센저 전원 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_LeakSensor_Module_Initialize(void)
{
#if (LEAK_SENSOR_POWER_CONTROL == USE)
    Hal_LeakSensor_Initialize();            // 누수 센서 전원 제어 관련 초기화
#endif
}


/// @brief      Hal Leak Sensor Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 누수 감지 센서 전원 제어를 한다
/// @param      void
/// @return     void
void Hal_LeakSensor_Module_Control(void)
{
#if (LEAK_SENSOR_POWER_CONTROL == USE)
    LeakSensorPowerControl();               // 누수 센서 전원 제어 함수
#endif
}


