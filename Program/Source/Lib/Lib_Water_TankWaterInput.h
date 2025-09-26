/// @file     Lib_Water_TankWaterInput.c
/// @date     2025/06/09
/// @author   Jaejin Ham
/// @brief    물탱크 입수 동작 관련 File


#ifndef _LIB_WATER_TANK_WATER_INPUT_H_
#define _LIB_WATER_TANK_WATER_INPUT_H_


#if (LEVEL_SENSOR_COUNT > 0)

void Lib_WaterInputControl(void);

#endif

void Lib_WaterInput_Module_Control(void);

#endif

