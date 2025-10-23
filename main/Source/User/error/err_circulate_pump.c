#include "err_circulate_pump.h"
#include "circulate_pump.h"
#include "cold_water.h"

U8 CheckErrCirculatePump(U8 error)
{
    // 냉수 생성 OFF에서는 에러 검출 없음
    if( GetColdWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }
    return GetCirculatePumpError();
}

U8 ReleaseErrCirculatePump(U8 error)
{
    // 냉수 생성 OFF에서는 에러 검출 없음
    if( GetColdWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }
    return GetCirculatePumpError();
}
