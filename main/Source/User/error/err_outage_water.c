/**
 * File : err_outage_water.c
 * 
 * Error Outage Water Check
**/
#include "err_outage_water.h"

#include "flow_meter.h"

#define AUTO_RELEASE_TIME     (600U)          // 60sec

// �ܼ� ���� ǥ�� ���� �ð� 1��
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
    // 1�� �� �ܼ� ���� �ڵ� ����
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
