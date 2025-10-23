#include "err_temp_room_water.h"
#include "err_temp.h"
#include "api_adc.h"
#include "temp.h"

static U8 mu8CountList[4] = 
{ 
    0,         /* Previous status */
    COUNT_NUM, 
    COUNT_NUM, 
    COUNT_NUM 
};

U8 CheckErrTempRoomWater(U8 mu8Error)
{
    U16 mu16Adc;

    mu16Adc = GetAdcData(ADC_ID_TEMP_ROOM_WATER);
    return CheckErrorTemp( mu8Error, mu16Adc, mu8CountList );
}


U8 ReleaseErrTempRoomWater(U8 mu8Error)
{
    TurnOnTempSensor( TEMP_ID_ROOM_WATER, 10 );

    return CheckErrTempRoomWater(mu8Error);
}
