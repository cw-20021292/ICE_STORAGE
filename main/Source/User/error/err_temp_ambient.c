#include "err_temp_ambient.h"
#include "err_temp.h"
#include "api_adc.h"

static U8 mu8CountList[4] = 
{ 
    0, 
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};

U8 CheckErrTempAmbient(U8 xError)
{
    U16 mu16Adc;

    mu16Adc = GetAdcData(ADC_ID_TEMP_AMBIENT);
    return CheckErrorTemp( xError, mu16Adc, mu8CountList );
}


U8 ReleaseErrTempAmbient(U8 xError)
{
    return CheckErrTempAmbient(xError);
}

U8 CheckErrTempAmbient2(U8 xError)
{
    U16 mu16Adc;

    mu16Adc = GetAdcData(ADC_ID_TEMP_AMBIENT_2);
    return CheckErrorTemp( xError, mu16Adc, mu8CountList );
}

U8 ReleaseErrTempAmbient2(U8 xError)
{
    return CheckErrTempAmbient(xError);
}
