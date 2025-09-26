/// @file     Hal_LeakSensor.h
/// @date     2025/04/09
/// @author   Jaejin Ham
/// @brief    누수 센서 Control file


#ifndef _HAL_LEAK_SENSOR_H_
#define _HAL_LEAK_SENSOR_H_


/*   누수 센서 제어 Function 사용법
     => Set_LeakSensor_Control(mu8OnOff);         // 누수 센서 전원 제어 : mu8OnOff - 0(OFF), 1(ON)

     사용 예)
     Set_LeakSensor_Control(ON);                  // 누수 센서의 전원을 ON
*/

#define LEAK_SENSOR_POWER_CONTROL               USE     // 누수 센서 전원 제어 여부 - 0(No Contorl), 1(Control)

#if (LEAK_SENSOR_POWER_CONTROL == USE)
// 누수 센서 전원 port 할당
#define PORT_DEFINE_LEAK_SENSOR_POWER           P10_bit.no6
#endif

#if (LEAK_SENSOR_POWER_CONTROL == USE)

// 누수 센서 Port Define
#define P_LEAK_SENSOR                           PORT_DEFINE_LEAK_SENSOR_POWER
#define TURN_ON_LEAK_SENSOR()                   { P_LEAK_SENSOR = 1; }
#define TURN_OFF_LEAK_SENSOR()                  { P_LEAK_SENSOR = 0; }
#define GET_STATUS_LEAK_SENSOR()                ( P_LEAK_SENSOR )


void Hal_LeakSensor_Initialize(void);
void LeakSensorPowerControl(void);
void Set_LeakSensor_Control(U8 mu8OnOff);

#endif


void Hal_LeakSensor_Module_Initialize(void);
void Hal_LeakSensor_Module_Control(void);

#endif

