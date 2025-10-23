/**
 * File : err_outage_water.c
 * 
 * Error Outage Water Check
**/
#include "err_outage_water.h"

#include "flow_meter.h"

#define AUTO_RELEASE_TIME     (600U)          // 60sec

// 단수 에러 표시 유지 시간 1분
U16 gUautoReleaseTime = AUTO_RELEASE_TIME;
U8 CheckErrorOutageWater(U8 error)
{
    if( GetOutageStatus(OUTAGE_TYPE_ICE_WATER) == FLOW_OUTAGE )
    {
        return TRUE;          // ERROR
    }

    gUautoReleaseTime = AUTO_RELEASE_TIME;
    return FALSE;             // NO ERROR
}

// call back interval : @100ms
U8 ReleaseErrorOutageWater(U8 error)
{
    // 1분 후 단수 에러 자동 해제
    if( gUautoReleaseTime == 0 )
    {
        SetOutageStatus(OUTAGE_TYPE_ICE_WATER, FLOW_SUPPLY);
    }
    else
    {
        gUautoReleaseTime--;
    }

    return CheckErrorOutageWater(error);
}
