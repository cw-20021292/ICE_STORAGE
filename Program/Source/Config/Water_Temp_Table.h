/// @file     Temp_Table.h
/// @date     2025/05/21
/// @author   Jaejin Ham
/// @brief    온도 센서 Table


#ifndef _TEMP_TABLE_H_
#define _TEMP_TABLE_H_


/*  제어 Function 사용법
    => 사용하는 온도 센서를 USE로 설정, 미사용 온도 센서는 NO_USE로 설정
    => Get_Temp(mu8ID);                         // ID에 해당하는 온도 센서의 온도값(Float형) 확인 - 예) 20도 = 20.0f
*/


// 냉수 - 주의사항: 두 온도 센서 동시 사용 설정 불가
#define CONFIG_TEMP_TABLE_COLD_JAMESTECH        USE     // 냉수온도센서 / 제임스텍
#define CONFIG_TEMP_TABLE_COLD_RATTRON          USE     // 냉수온도센서 / 래트론

// 정수
#define CONFIG_TEMP_TABLE_ROOM_JAMESTECH        USE     // 정수온도센서 / 제임스텍

// 온수 탱크
#define CONFIG_TEMP_TABLE_HOT_TANK_JAMESTECH    NO_USE     // 온수온도센서 / 제임스텍 / 탱크식

// 순간온수
#define CONFIG_TEMP_TABLE_HOT_IN_SK             USE     // 온수온도센서 / SK / 순간온수 입수(T1)
#define CONFIG_TEMP_TABLE_HOT_BODY_SK           USE     // 온수온도센서 / SK / 순간온수 히터 Body(= 순간온수 출수(T2))
#define CONFIG_TEMP_TABLE_HOT_OUT_SK            USE     // 온수온도센서 / SK / 순간온수 출수(T2)

// 열전모듈 히터싱크
#define CONFIG_TEMP_TABLE_HEATSINK_JAMESTECH    USE     // 히트싱크 / 제임스텍 / 열전모듈 히트싱크

// 외기
#define CONFIG_TEMP_TABLE_AIR_JAMESTECH         USE     // 외기온도센서 / 제임스텍

// Evaporator
#define CONFIG_TEMP_TABLE_EVA                   NO_USE     // 증발기온도센서

// FAUCET
#define CONFIG_TEMP_TABLE_FAUCET_OUT_SK         NO_USE     // 파우셋 추출부 온도 센서(빌트인 모델 적용) (= 순간온수 입수)

// TDS
#define CONFIG_TEMP_TABLE_TDS_IN_SK             NO_USE     // TDS 센서 / SK
#define CONFIG_TEMP_TABLE_TDS_OUT_SK            NO_USE     // TDS 센서 / SK


// 사용하는 온도 센서들의 ADC ID들을 배정
#define COOL_THERMISTOR_1_ID                    ADC_ID_TH_COOL
#define ROOM_THERMISTOR_ID                      0
#define HOT_THERMISTOR_ID                       0
#define HOT_IN_THERMISTOR_ID                    ADC_ID_TH_HOT_IN
#define HOT_BODY_THERMISTOR_ID                  ADC_ID_TH_HOT_BODY
#define HOT_OUT_THERMISTOR_ID                   ADC_ID_TH_HOT_OUT
#define TEM_HEATSINK_THERMISTOR_ID              ADC_ID_TH_HEATSINK
#define AIR_THERMISTOR_ID                       ADC_ID_TH_AIR
#define FAUCET_OUT_THERMISTOR_ID                0
#define TDS_IN_THERMISTOR_ID                    0
#define TDS_OUT_THERMISTOR_ID                   0
#define EVA_THERMISTOR_ID                       0


F32 ConvInt2Temp(I16 mi16temp);
U8 ConvTemp2Char(F32 mf32temp);

#if ( (CONFIG_TEMP_TABLE_COLD_JAMESTECH == USE) || (CONFIG_TEMP_TABLE_COLD_RATTRON == USE) )
F32 ConvAdc2Temp_ColdWater(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_ROOM_JAMESTECH == USE)
F32 ConvAdc2Temp_Room(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HOT_TANK_JAMESTECH == USE)
F32 ConvAdc2Temp_HotTank(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HOT_IN_SK == USE)
F32 ConvAdc2Temp_HotIn(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HOT_BODY_SK == USE)
F32 ConvAdc2Temp_HotBody(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HOT_OUT_SK == USE)
F32 ConvAdc2Temp_HotOut(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_HEATSINK_JAMESTECH == USE)
F32 ConvAdc2Temp_Heatsink(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_AIR_JAMESTECH == USE)
F32 ConvAdc2Temp_Air(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_FAUCET_OUT_SK == USE)
F32 ConvAdc2Temp_FaucetOut(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_EVA == USE)
F32 ConvAdc2Temp_Eva(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_TDS_IN_SK == USE)
F32 ConvAdc2Temp_TdsIn(U16 mu16Adc);
#endif

#if (CONFIG_TEMP_TABLE_TDS_OUT_SK == USE)
F32 ConvAdc2Temp_TdsOut(U16 mu16Adc);
#endif

F32 Get_Temp(U8 mu8ID);

#endif
