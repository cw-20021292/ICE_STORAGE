/// @file     Lib_Water_Heater.h
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    Heat Tank 동작 제어 file

#ifndef _LIB_WATER_HEATER_H_
#define _LIB_WATER_HEATER_H_


/*  Lib Function 사용법
    => SSmartPowerSaveHeatTempData[HEIGHT_MODE_COUNT], SPowerSaveHeatTempData[HEIGHT_MODE_COUNT], SHeatTempData[HEIGHT_MODE_COUNT]
       테이블의 설정값에 따라서 SW 제어 모듈에서 알아서 동작함

    => Set_HotFuncStatus(mu8Status);                    // 온수 기능 설정 여부 설정 - 0(OFF), 1(ON)
    => Get_HotFuncStatus();                             // 온수 기능 설정 여부 확인 - 0(OFF), 1(ON)

    => Set_HotOnDelay(mu8Time);                         // 온수 히팅 제어 시작 전 Delay 시간 설정 - 100ms 단위

    => Set_HeightMode(mu8Step);                         // 고도 모드 단계 설정 - 0(1단계), 1(2단계), 2(3단계), 3(4단계)
    => Get_HeightMode();                                // 고도 모드 단계 확인 - 0(1단계), 1(2단계), 2(3단계), 3(4단계)
*/



#define HEIGHT_MODE_COUNT                           4       // 고도 모드의 모드 갯수

#define HEIGHT_MODE_1                               0       // default
#define HEIGHT_MODE_2                               1
#define HEIGHT_MODE_3                               2
#define HEIGHT_MODE_4                               3

#if (NORMAL_HEATER_USE == USE)                  // 일반 히터를 사용하는 경우

#define HEAT_TANK_CONTROL_TIME_PERIOD               100     // 온수 탱크 제어 시간 주기 : 100ms(@1ms)

#define HEAT_OFF_DELAY_TIME                         0       // 히터 OFF 전 Delay 시간 0초 (@100ms)
#define HEAT_ON_DELAY_TIME_LOW_LEVEL                100     // 저수위 센서 미감지 후 재감지시 히터 ON 전 Delay 시간 10초 (@100ms)

void Lib_HeatTank_Initialize(void);
void HeatTankControlTimer(void);
void Set_HotFuncStatus(U8 mu8Status);
U8 Get_HotFuncStatus(void);
void Set_HotOnDelay(U8 mu8Time);
void Set_LowWaterHeatDelay(U8 mu8Time);
void Set_HeightMode(U8 mu8Step);
U8 Get_HeightMode(void);
void Lib_HeatTank_Control(void);


#endif      // 일반형 히터 사용 여부 조건 END

void Lib_Heater_Module_Initialize(void);
void Lib_Heater_Module_1ms_Control(void);
void Lib_Heater_Module_Control(void);

#endif

