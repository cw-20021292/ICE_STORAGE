/**
 * File : api_adc.h
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#ifndef __API_ADC_H__
#define __API_ADC_H__

#include "prj_type.h"

/**
 * UV1 : 2개 병렬 연결, 얼음탱크(제품 후면), 얼음트레이 좌측(제품 오른쪽, 냉수탱크 위치)
 * UV2 : 2개 병렬 연결, 얼음트레이(제품 좌측 2개)
 * UV3 : 1개 연결, 물 추출구
 * UV4 : 1개 연결, 얼음 추출구
 * UV5 : 1개 연결, 배수탱크 상측에 위치, 배수탱크 살균
*/

/**
 * ICE FULL RX-TX 1 : 만빙
 * ICE FULL RX-TX 2 : 저빙
*/

/*
 * EVA1 : Cooling Coil
 * EVA2 : Ice Coil
*/
typedef enum _adc_id_
{
    ADC_ID_PUMP_DRAIN_IAD = 0,      /// 0 : example
    ADC_ID_UV_DRAIN_TANK_IAD,       /// 1 : 
    ADC_ID_TEMP_HOT_WATER,          /// 2 : 
    ADC_ID_UV_ICE_DOOR_IAD,         /// 3 : 
    ADC_ID_UV_WATER_OUT_IAD,        /// 4 : 
    ADC_ID_UV_ICE_TRAY_IAD,         /// 5 : 
    ADC_ID_UV_ICE_TANK_IAD,         /// 6 : 
    ADC_ID_TEMP_COLD_WATER,         /// 7 : 
    ADC_ID_TEMP_ROOM_WATER,         /// 8 : 
    ADC_ID_SENSOR_INFRARED_HIGH,    /// 9 : 
    ADC_ID_TEMP_AMBIENT,            /// 10 : 
    ADC_ID_TEMP_EVA_1,              /// 11 : 
    ADC_ID_TEMP_EVA_2,              /// 12 : 
    ADC_ID_SENSOR_INFRARED_LOW,     /// 13 : 
    ADC_ID_FAN_IAD,                 /// 14 : 
    ADC_ID_PUMP_COLD_VAD,           /// 15 : 
    ADC_ID_PUMP_COLD_IAD,           /// 16 : 
    ADC_ID_PUMP_ICE_DRAIN_IAD,      /// 17 : 
    ADC_ID_TEMP_AMBIENT_2,          /// 18 : 
    ADC_ID_MAX,                     /// 19 : max
} EAdcId_T;

void InitAdc(void);
void SetAdcData(U8 id, U16 adc);
U16 GetAdcData(U8 id);
void TurnOnSensingAdc(U8 id);
void TurnOffSensingAdc(U8 id);
I8 IsOnOffSensingAdc(U8 id);

void ProcessAdc(void);

#endif /* __API_ADC_H__ */