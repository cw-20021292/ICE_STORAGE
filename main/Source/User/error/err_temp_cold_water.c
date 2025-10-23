#include "err_temp_cold_water.h"
#include "err_temp.h"
#include "api_adc.h"
#include "cold_water.h"
#include "temp.h"

static U8 mu8CountList[4] = 
{ 
    0,
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};

U8 CheckErrTempColdWater(U8 mu8Error)
{
    U16 mu16Adc;

    if( GetColdWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }
    mu16Adc = GetAdcData(ADC_ID_TEMP_COLD_WATER);
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}

U8 ReleaseErrTempColdWater(U8 mu8Error )
{
    if( GetColdWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }

    // ?�러 ?�태?�면, ?�서 ?�태�??�시�??�는??
    TurnOnTempSensor( TEMP_ID_COLD_WATER, 10 );

    return CheckErrTempColdWater( mu8Error );
}
