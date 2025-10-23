#include "err_circulate_pump.h"
#include "circulate_pump.h"
#include "cold_water.h"

U8 CheckErrCirculatePump(U8 error)
{
    // �ü� ���� OFF������ ���� ���� ����
    if( GetColdWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }
    return GetCirculatePumpError();
}

U8 ReleaseErrCirculatePump(U8 error)
{
    // �ü� ���� OFF������ ���� ���� ����
    if( GetColdWaterConfigMake() == FALSE )
    {
        return FALSE; // no error
    }
    return GetCirculatePumpError();
}
