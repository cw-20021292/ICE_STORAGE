/**
 * File : uv.h
 * 
 * Uv
*/
#include "hw.h"
#include "uv.h"

#include "api_uv.h"
#include "util.h"
  
typedef struct _uv_item_
{
    SUvSter_T Uv;
    SUvId_T Id;

    U8 Config;          // 일시 정지
    U8 Pause;           // 일시 정지

    U32 OffTime;        // @100ms
    U32 OnTime;         // @100ms
    U32 ConfOnTime;     // @100ms

    U8 IsLimited;      // LIMITED or UNLIMITED
    U8 Count;          // LIMIT COUNT
} SUvItem_T;

SUvItem_T  UvOnOffList[] = 
{
    /* UV,                     ID,                CONFIG,   PAUSE,  T_OFF,  T_ON,   TC_ON,  LIMIT,      LIMIT COUNTER */
    { UV_STER_ICE_DOOR,        UV_ID_ICE_DOOR,    TRUE,     OFF,    0,      0,      0,      UNLIMITED,  0 },
    { UV_STER_ICE_DOOR_SHOT,   UV_ID_ICE_DOOR,    FALSE,    OFF,    0,      0,      0,      UNLIMITED,  0 },

    { UV_STER_WATER_OUT,       UV_ID_WATER_OUT,   TRUE,     OFF,    0,      0,      0,      UNLIMITED,  0 },
    { UV_STER_WATER_OUT_SHOT,  UV_ID_WATER_OUT,   FALSE,    OFF,    0,      0,      0,      UNLIMITED,  0 },

    { UV_STER_ICE_TRAY,        UV_ID_ICE_TRAY,    TRUE,     OFF,    0,      0,      0,      LIMITED,    6 },
    { UV_STER_ICE_TRAY_SHOT,   UV_ID_ICE_TRAY,    FALSE,    OFF,    0,      0,      0,      UNLIMITED,  0 },

    { UV_STER_ICE_TANK,        UV_ID_ICE_TANK,    TRUE,     OFF,    0,      0,      0,      UNLIMITED,  0 },

    { UV_STER_DRAIN_TANK,      UV_ID_DRAIN_TANK,  TRUE,     OFF,    0,      0,      0,      UNLIMITED,  0 },
};

#define SZ_UV_LIST      ( sizeof(UvOnOffList) / sizeof(SUvItem_T) )

// 0보다 크면, ON 
// 0보다 작으면, OFF
I8 UvOnOff[UV_ID_MAX];
static void InitOnOffUv(void)
{
    U8 i;

    for( i=0; i<UV_ID_MAX; i++ )
    {
        UvOnOff[ i ] = 0;
    }
}

static void TurnOnUv(SUvId_T	id)
{
    UvOnOff[id]++;
}

static void TurnOffUv(SUvId_T	id)
{
    if( UvOnOff[id] > 0 )
    {
        UvOnOff[id]--;
    }
}

static void OnOffUv(void)
{
    U8 i;

    for( i=0; i<UV_ID_MAX; i++ )
    {
        if( UvOnOff[i] > 0 )
        {
            API_TurnOnUv(i);
        }
        else 
        {
            API_TurnOffUv(i);
        }
    }
}

static U8 FindListIndexId(SUvSter_T uv, U8 *pIndex)
{
    U8 i;

    for( i = 0; i < SZ_UV_LIST; i++ )
    {
        if( UvOnOffList[i].Uv == uv )
        {
            *pIndex = i;
             return TRUE;
        }
    }

    return FALSE;
}

void SetConfigUvId(SUvSter_T uv, U8 config)
{
    U8 index;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        UvOnOffList[index].Config = config;
    }
}

U8 GetConfigUvId(SUvSter_T uv)
{
    U8 index;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        return UvOnOffList[index].Config;
    }

    return FALSE;
}


// unlimited 타입이면, 항상 ok(FALSE)
// 그렇지 않은 경우, count에 의해 확인
static U8 IsOverLimitCount(U8 index)
{
    if( UvOnOffList[index].IsLimited == UNLIMITED )
    {
        return FALSE;   
    }

    if( UvOnOffList[index].Count == 0 )
    {
        return TRUE;
    }

    return FALSE;
}

// limit count 확인해서 count가 없으면 on 설정이 불가
void TurnOnUvId(SUvSter_T uv, U32 offTime, U32 onTime )
{
    U8 index;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        if( UvOnOffList[index].Config == FALSE )
        {
            return ;
        }

        if( IsOverLimitCount(index) == TRUE )
        {
            return ;
        }

        DOWN_COUNT(UvOnOffList[index].Count);
        UvOnOffList[index].OffTime = offTime;
        UvOnOffList[index].OnTime = onTime;
        UvOnOffList[index].ConfOnTime = onTime;
    }
}

void TurnOffUvId(SUvSter_T uv)
{
    U8 index;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        UvOnOffList[index].OffTime     = 0;
        UvOnOffList[index].OnTime      = 0;
        UvOnOffList[index].ConfOnTime  = 0;
    }
}

U8  IsTurnOnUvId(SUvSter_T uv)
{
    U8 index;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        if( UvOnOffList[index].OffTime == 0 
            && UvOnOffList[index].OnTime != 0 )
        {
            return TRUE;
        }
    }

    return FALSE;
}

void SetPauseUvId(SUvSter_T uv, U8 status)
{
    U8 index;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        UvOnOffList[index].Pause = status;
    }
}

U8 GetPauseUvId(SUvSter_T uv)
{
    U8 index;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        return UvOnOffList[index].Pause;
    }

    return FALSE;
}

U8 GetUvOnPercent(SUvSter_T uv)
{
    U8 index;
    U32 current;
    U32 target;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        current = UvOnOffList[index].ConfOnTime - UvOnOffList[index].OnTime;
        target  = UvOnOffList[index].ConfOnTime;

        return (U8)(( (F32)current / (F32)target ) * 100.0f);
    }

    return 100;
}

void SetLimitConfUvId(SUvSter_T uv, U8 isLimited)
{
    U8 index;

    if( FindListIndexId(uv, &index) == TRUE )
    {
        UvOnOffList[index].IsLimited = isLimited;
    }
}

void SetLimitCountUvId(SUvSter_T uv, U8 count)
{
    U8 index;

    if( FindListIndexId(uv, &index ) == TRUE )
    {
        UvOnOffList[index].Count = count;
    }
}

void ControlUv(void)
{
    U8 i;
    SUvItem_T *pItem = NULL;

    InitOnOffUv();
    for( i=0; i<SZ_UV_LIST; i++ )
    {
        pItem = &UvOnOffList[i];

        // Check Pause
        if( pItem->Pause == TRUE )
        {
            DOWN_COUNT( pItem->OffTime );
            continue;
        }

        // Check On/Off
        if( pItem->OffTime == 0 
                && pItem->OnTime != 0)
        {
            TurnOnUv(pItem->Id);
            DOWN_COUNT(pItem->OnTime);
        }
        else
        {
            DOWN_COUNT(pItem->OffTime);
        }
    }

    OnOffUv();
}
