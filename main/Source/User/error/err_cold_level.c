#include "err_cold_level.h"
#include "level.h"

U8 CheckErrColdLow(U8 error)
{
    U8 mu8Count;

    mu8Count = GetTankLevelErrorCount( LEVEL_ID_COLD, ERR_TYPE_LOW );
    if( mu8Count >= 1 )
    {
        return TRUE; // ERROR
    }
    return error;
}

U8 ReleaseErrColdLow(U8 error)
{
    U8 mu8Count;

    mu8Count = GetTankLevelErrorCount( LEVEL_ID_COLD, ERR_TYPE_LOW );
    if( mu8Count == 0 )
    {
        return FALSE; // ERROR
    }
    return error;
}
