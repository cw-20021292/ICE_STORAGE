/// @file     Lib_Water_LevelSensor.h
/// @date     2025/06/09
/// @author   Jaejin Ham
/// @brief    수위 체크 동작 관련 File


#ifndef _LIB_WATER_LEVEL_SENSOR_H_
#define _LIB_WATER_LEVEL_SENSOR_H_


/*  Lib Function 사용법
    => Get_ColdWaterLevel();                            // 냉수 탱크의 수위 상태 확인 - 0(저수위), 1(중수위), 2(만수위)
    => Get_HotWaterLevel();                             // 온수 탱크의 수위 상태 확인 - 0(저수위), 1(중수위), 2(만수위)
    => Get_WaterLevel();                                // 정수 탱크의 수위 상태 확인
                                                           - 중수위 있는 경우 0(저수위), 1(중저수위), 2(중고수위), 3(만수위)
                                                           - 중수위 없는 경우 0(저수위), 1(중수위), 2(만수위)
    => Get_FirstHighLevelCheck();                       // 최초 만수위 감지 여부 확인 - 0(미감지), 1(감지)
    => Get_FirstLowLevelCheck();                        // 최초 저수위 감지 여부 확인 - 0(미감지), 1(감지)
*/


#if (LEVEL_SENSOR_COUNT > 0)

// 수위
#define COLD_LOW_SENSOR_USE             USE           // 냉수 저수위 센서 사용
#define MIDDLE_SENSOR_USE               NO_USE        // 중수위 센서 사용
#define HOT_HIGH_SENSOR_USE             USE           // 온수 만수위 센서 사용

#define COLD_WATER_LEVEL_LOW            0x00
#define COLD_WATER_LEVEL_MIDDLE         0x01
#define COLD_WATER_LEVEL_HIGH           0x02

#if (MIDDLE_SENSOR_USE == USE)
#define WATER_LEVEL_LOW                 0x00
#define WATER_LEVEL_MIDDLE_LOW          0x01
#define WATER_LEVEL_MIDDLE_HIGH         0x02
#define WATER_LEVEL_HIGH                0x03
#else                                   // 중수위 센서 미사용시
#define WATER_LEVEL_LOW                 0x00
#define WATER_LEVEL_MIDDLE              0x01
#define WATER_LEVEL_HIGH                0x02
#endif

#define HOT_WATER_LEVEL_LOW             0x00
#define HOT_WATER_LEVEL_MIDDLE          0x01
#define HOT_WATER_LEVEL_HIGH            0x02


void Lib_WaterLevel_Initialize(void);
void Lib_WaterLevelCheck(void);

U8 Get_ColdWaterLevel(void);
U8 Get_HotWaterLevel(void);
U8 Get_WaterLevel(void);
U8 Get_FirstHighLevelCheck(void);
U8 Get_FirstLowLevelCheck(void);

#endif      // 수위 센서가 있는 경우 END


void Lib_WaterLevel_Module_Initialize(void);
void Lib_WaterLevel_Module_Control(void);

#endif

