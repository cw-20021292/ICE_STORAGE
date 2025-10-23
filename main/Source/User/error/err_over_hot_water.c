#include "err_over_hot_water.h"
#include "hot_water.h"
#include "relay.h"
#include "valve.h"

#define HEATER_ON_3HOURS    ( 3UL * 60UL * 600UL )  /* @100msec, 3hour x 60min x 60sec */

// 온수 과열 에러 발생 에러 카운트
U32 gu32HeaterOnTime = HEATER_ON_3HOURS;    // @100msec
U8 CheckErrOverHotWater(U8 mu8Error)
{
    // 온수 미사용이면, 에러 카운트 초기화
    if( GetHotWaterConfigMake() == FALSE )
    {
        gu32HeaterOnTime = HEATER_ON_3HOURS;
        return FALSE; // no error
    }


    // 히터가 OFF이면, 에러 카운터 초기화
    if( IsTurnOnRelay( RELAY_HEATER ) == FALSE )
    {
        gu32HeaterOnTime = HEATER_ON_3HOURS;
    }

    // 온수 추출이면, 에러 카운트 초기화
    if( IsOpenValve( VALVE_HOT_OUT ) == TRUE )
    {
        gu32HeaterOnTime = HEATER_ON_3HOURS;
    }


    // 에러 카운트가 0이면, 에러 발생
    gu32HeaterOnTime--;
    if( gu32HeaterOnTime == 0UL )
    {
        return TRUE;    // error
    }

    return FALSE;   // no error
}
