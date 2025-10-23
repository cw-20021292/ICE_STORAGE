#include "err_temp_eva_2.h"
#include "err_temp.h"
#include "api_adc.h"

static U8 mu8CountList[4] = 
{ 
    0,
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};


U8 CheckErrTempEva_2(U8 mu8Error)
{
    U16 mu16Adc;

    mu16Adc = GetAdcData(ADC_ID_TEMP_EVA_2);
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}


U8 ReleaseErrTempEva_2(U8 mu8Error)
{
    return CheckErrTempEva_2(mu8Error);
}
