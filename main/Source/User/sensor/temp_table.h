/**
 * File : temp_table.h
 * 
 * Temp Sensor Table
*/
#ifndef __TEMP_TABLE_H__
#define __TEMP_TABLE_H__

#include "prj_type.h"

#define CONFIG_TEMP_TABLE_COLD_JAMESTECH        (1)     // 냉수온도센서 / 제임스텍
#define CONFIG_TEMP_TABLE_COLD_RATTRON          (0)     // 냉수온도센서 / 래트론

#define CONFIG_TEMP_TABLE_ROOM                  (1)     // 정수온도센서 / 제임스텍 (냉수온도센서)

#define CONFIG_TEMP_TABLE_HOT_JAMESTECH_TANK    (1)     // 온수온도센서 / 제임스텍 / 탱크식
#define CONFIG_TEMP_TABLE_HOT_BODY_SK           (0)     // 온수온도센서 / SK / 순간온수 히터 바디
#define CONFIG_TEMP_TABLE_HOT_IN_SK             (1)     // 온수온도센서 / SK / 순간온수 입수 ( T1 )
#define CONFIG_TEMP_TABLE_HOT_OUT_SK            (0)     // 온수온도센서 / SK / 순간온수 출수 ( T2 )

#define CONFIG_TEMP_TABLE_HEATSINK              (0)     // 히트싱크 / 제임스텍 / 열전모듈 히트싱크

#define CONFIG_TEMP_TABLE_AMBI                  (1)     // 외기온도센서 / 제임스텍

#define CONFIG_TEMP_TABLE_EVA                   (1)     // 증발기온도센서 / ?

#define CONFIG_TEMP_TABLE_ICE_TRAY_IN           (0)     // 아이스트레이입수온도센서 / ?

#define CONFIG_TEMP_TABLE_TDS                   (0)     // TDS 센서 / ?

#if( CONFIG_TEMP_TABLE_COLD_JAMESTECH == 1 )
TEMP_T ConvAdc2Temp_ColdWater_Jamestech(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_COLD_RATTRON  == 1 )
TEMP_T ConvAdc2Temp_ColdWater_Rattron(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_ROOM  == 1 )
TEMP_T ConvAdc2Temp_RoomWater(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HOT_JAMESTECH_TANK  == 1 )
TEMP_T ConvAdc2Temp_HotWater_Jamestech_Tank(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HOT_BODY_SK  == 1 )
TEMP_T ConvAdc2Temp_HotWater_Sk_Body(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HOT_IN_SK  == 1 )
TEMP_T ConvAdc2Temp_HotWater_In_Sk(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HOT_OUT_SK  == 1 )
TEMP_T ConvAdc2Temp_HotWater_Out_Sk(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_HEATSINK  == 1 )
TEMP_T ConvAdc2Temp_Heatsink_Jamestech(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_AMBI  == 1 )
TEMP_T ConvAdc2Temp_Ambient_(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_EVA  == 1 )
TEMP_T ConvAdc2Temp_Eva_(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_ICE_TRAY_IN  == 1 )
TEMP_T ConvAdc2Temp_IceTray_In_(U16 adc);
#endif

#if( CONFIG_TEMP_TABLE_TDS  == 1 )
TEMP_T ConvAdc2Temp_Tds_In_(U16 adc);
TEMP_T ConvAdc2Temp_Tds_Out_(U16 adc);
#endif

#endif
