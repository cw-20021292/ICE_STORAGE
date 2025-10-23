/**
 * File : temp.c
 * 
 * Temp Sensor
*/
#include "temp.h"

#include "temp_table.h"
#include "api_adc.h"

typedef TEMP_T(*FPAction_T)(U16 adc);
typedef struct _temp_list_
{
    U8 Id;                     // Analoge Channel Id 
    FPAction_T pConvAdc2Temp;  // Convert raw adc to temperture

    U8 OverrideFlag;           // Override Flag
    TEMP_T OverrideTemp;       // Overrided Temperture Value

    U32 TimeOut;               // Temp Sensor Power On TimeOut
} TempList_T;

TempList_T TempList[] = 
{
    { ADC_ID_TEMP_AMBIENT,    ConvAdc2Temp_Ambient_,                FALSE,  24.0f, 0 },
    { ADC_ID_TEMP_AMBIENT_2,  ConvAdc2Temp_Ambient_,                FALSE,  24.0f, 0 },
    { ADC_ID_TEMP_ROOM_WATER, ConvAdc2Temp_RoomWater,               FALSE,  24.0f, 0 },
    { ADC_ID_TEMP_COLD_WATER, ConvAdc2Temp_ColdWater_Jamestech,     FALSE,  24.0f, 0 },
    { ADC_ID_TEMP_EVA_1,      ConvAdc2Temp_Eva_,                    FALSE,  24.0f, 0 },
    { ADC_ID_TEMP_EVA_2,      ConvAdc2Temp_Eva_,                    FALSE,  24.0f, 0 },
    { ADC_ID_TEMP_HOT_WATER,  ConvAdc2Temp_HotWater_Jamestech_Tank, FALSE,  24.0f, 0 }
};
#define	SZ_TEMP_LIST		( sizeof(TempList) / sizeof(TempList_T) )

static U8 ConvAdc2Temp(U8 id, U16 adcRaw, TEMP_T *pTemp)
{
    if( pTemp == NULL )
    {
        return FALSE;
    }

    if( TempList[id].pConvAdc2Temp != NULL )
    {
        *pTemp = TempList[id].pConvAdc2Temp(adcRaw);

        return TRUE;
    }

    return FALSE;
}

TEMP_T GetTemp(U8 id)
{
    U16 adc = 0U;
    TEMP_T temp = 0.0f;

    /* Out of range */
    if( id >= TEMP_ID_MAX )
    {
        return 0.0f;
    }

    /* Get Adc value */
    adc = GetAdcData(TempList[id].Id);

    /* Conv Adc to Temperture */
    ConvAdc2Temp(id, adc, &temp);

    /* Override Temp */
    if( TempList[id].OverrideFlag == TRUE )
    {
        return TempList[id].OverrideTemp;
    }

    return temp;
}

U16 GetTempSensorTime(U8 id)
{
    return (U16)( TempList[id].TimeOut / 10UL );
}

// 현재 시간 보다 큰 경우에는 업데이트 해준다.
void TurnOnTempSensor(U8 id, U16 timeOut)
{
    U32 mu32Time;


    mu32Time = (U32)timeOut * 10UL;
    if( mu32Time > TempList[id].TimeOut )
    {
        TurnOnSensingAdc(TempList[id].Id);
        TempList[id].TimeOut = mu32Time;
    }
}

void TurnOffTempSensor(U8 id)
{
    TurnOffSensingAdc(TempList[id].Id);
    TempList[id].TimeOut = 0;
}

// Override
void SetOverrideTemp(U8 id, U8 xOverride, TEMP_T temp )
{
    TempList[id].OverrideFlag = xOverride; 
    TempList[id].OverrideTemp = temp; 
}

TEMP_T GetOverrideTemp(U8 id, U8 xOverride)
{
    return TempList[id].OverrideTemp = xOverride; 
}

TEMP_T  gTempRoom = 24.0f;
TEMP_T  gTempCold = 24.0f;
TEMP_T  gTempAmbi = 24.0f;
TEMP_T  gTempAmbi_2 = 24.0f;
TEMP_T  gTempEva1 = 24.0f;
TEMP_T  gTempEva2 = 24.0f;
TEMP_T  gTempHotWater = 24.0f;

U16 gAdcIceFullHigh = 0;
U16 gAdcIceFullLow = 0;

U16 gAdcUv_drain = 0;
U16 gAdcUv_ice_door = 0;
U16 gAdcUv_water_out = 0;
U16 gAdcUv_ice_tray = 0;
U16 gAdcUv_ice_tank = 0;

void ProcessTemp(void)
{
    U8 i = 0U;

    for( i=0; i<SZ_TEMP_LIST; i++ )
    {
        if( TempList[i].TimeOut != 0UL )
        {
            TempList[i].TimeOut--;
            if( TempList[i].TimeOut == 0UL )
            {
                TurnOffTempSensor(i);
            }
        }
    }

    // FOR DEBUGGING...
    gTempRoom = GetTemp( TEMP_ID_ROOM_WATER );
    gTempCold = GetTemp( TEMP_ID_COLD_WATER );
    gTempAmbi = GetTemp( TEMP_ID_AMBIENT );
    gTempAmbi_2 = GetTemp( TEMP_ID_AMBIENT_2 );
    gTempEva1 = GetTemp( TEMP_ID_EVA_1 );
    gTempEva2 = GetTemp( TEMP_ID_EVA_2 );
    gTempHotWater = GetTemp( TEMP_ID_HOT_WATER );

    gAdcIceFullHigh = GetAdcData(ADC_ID_SENSOR_INFRARED_HIGH);
    gAdcIceFullLow = GetAdcData(ADC_ID_SENSOR_INFRARED_LOW);

    gAdcUv_drain = GetAdcData(ADC_ID_UV_DRAIN_TANK_IAD);
    gAdcUv_ice_door = GetAdcData(ADC_ID_UV_ICE_DOOR_IAD);
    gAdcUv_water_out = GetAdcData(ADC_ID_UV_WATER_OUT_IAD);
    gAdcUv_ice_tray = GetAdcData(ADC_ID_UV_ICE_TRAY_IAD);
    gAdcUv_ice_tank = GetAdcData(ADC_ID_UV_ICE_TANK_IAD); 
}
