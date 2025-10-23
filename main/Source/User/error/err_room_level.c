#include "err_room_level.h"
#include "level.h"

/// 정수 에러 최대 카운터
U8 gu8ErrCountLimit = DEFAULT_ROOM_ERR_COUNT;

void SetRoomErrCountLimit(U8 mu8Count)
{
    gu8ErrCountLimit = mu8Count;
}

U8 GetRoomErrCountLimit(void)
{
    return gu8ErrCountLimit;
}


U8 CheckErrRoomOvf(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_OVF );
    if( mu8Count >= gu8ErrCountLimit )
    {
        return TRUE; // ERROR
    }
    return mu8Error;
}

U8 CheckErrRoomComplex(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_COMPLEX );
    if( mu8Count >= gu8ErrCountLimit )
    {
        return TRUE; // ERROR
    }
    return mu8Error;
}

U8 CheckErrRoomHigh(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_HIGH );
    if( mu8Count >= gu8ErrCountLimit )
    {
        return TRUE; // ERROR
    }
    return mu8Error;
}

U8 CheckErrRoomLow(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_LOW );
    if( mu8Count >= 1 )
    {
        return TRUE; // ERROR
    }
    return mu8Error;
}

U8 ReleaseErrRoomLow(U8 mu8Error)
{
    U8 mu8Count;


    mu8Count = GetTankLevelErrorCount( LEVEL_ID_ROOM, ERR_TYPE_LOW );
    if( mu8Count == 0 )
    {
        return FALSE; // ERROR
    }
    return mu8Error;
}
