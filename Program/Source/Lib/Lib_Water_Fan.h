/// @file   Lib_Water_Fan.h
/// @date   2025/07/09
/// @author Jaejin Ham
/// @brief  Fan 동작 제어 관련 헤더 file

#ifndef _LIB_WATER_FAN_H_
#define _LIB_WATER_FAN_H_


/*  Lib Function 사용법
    => Set_CoolFanOnOff(mu8OnOff);                  // 냉각 Fan 동작 상태 설정 - 0(OFF), 1(ON)
    => Get_CoolFanOnOff();                          // 냉각 Fan 동작 상태 확인 - 0(OFF), 1(ON)

    => Set_CoolFanVolt(mu8Volt);                    // 냉각 Fan 동작 제어 Volt 설정
    => Get_CoolFanVolt();                           // 냉각 Fan 동작 제어 Volt 확인
    => Set_CoolFanActMode(mu8Data);                 // 냉각 Fan 동작 제어 Mode 설정
    => Get_CoolFanActMode();                        // 냉각 Fan 동작 제어 Mode 확인
*/


#define COOL_FAN_CONTROL_TIME_PERIOD            100     // 냉각팬 제어를 위한 제어 주가 타이머 100ms@1ms

#define FAN_DELAY_TIME                          10      // 냉각팬 전압 전환시 Delay 시간 1sec@100ms

void Lib_CoolFan_Initialize(void);
void CoolFanControlTimer(void);

#if (DC_FAN_COUNT > 0)
void Set_CoolFanOnOff(U8 mu8OnOff);
U8 Get_CoolFanOnOff(void);
#endif

#if (VOLT_CONTROL_DC_FAN_USE == USE)
/// @brief      냉각 FAN 제어 모드 리스트
/// @details    냉각 FAN 제어 모드를 구조 리스트로 정리한다
typedef enum
{
    FAN_ID_OFF = 0,        // 0 :
    FAN_ID_18V_ON,         // 1 :
    FAN_ID_21V_ON,         // 2 :
    FAN_ID_MAX             // Max
} EFanID_T;

void Set_CoolFanVolt(U8 mu8Volt);
U8 Get_CoolFanVolt(void);
void Set_CoolFanActMode(U8 mu8Data);
U8 Get_CoolFanActMode(void);
#endif

void Lib_CoolFan_Control(void);

void Lib_CoolFan_Module_Initialize(void);
void Lib_CoolFan_Module_1ms_Control(void);
void Lib_CoolFan_Module_Control(void);

#endif

