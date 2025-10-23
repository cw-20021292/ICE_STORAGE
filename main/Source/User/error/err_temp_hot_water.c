#include "err_temp_hot_water.h"
#include "err_temp.h"
#include "api_adc.h"
#include "hot_water.h"

static U8 mu8CountList[4] = 
{ 
    0,
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};

U8 CheckErrTempHotWater(U8 mu8Error)
{
    U16 mu16Adc;

    if( GetHotWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }
    mu16Adc = GetAdcData(ADC_ID_TEMP_HOT_WATER);
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}

U8 ReleaseErrTempHotWater(U8 mu8Error)
{
    // �¼� ������ OFF�̸�, ������ ����
    if( GetHotWaterConfigMake() == FALSE )
    {
        return FALSE;   // no error
    }

    return CheckErrTempHotWater(mu8Error);
}
