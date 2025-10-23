#include "preheat_table.h"

/* PREHEAT REGION LIST NUM */
#define  PREHEAT_REGION_1_COUNT   2   // ~13'c
#define  PREHEAT_REGION_2_COUNT   2   // ~20'c
#define	PREHEAT_REGION_NUM	    2

#define COMP_RPS_A    (50)  // ~20'c
#define COMP_RPS_B    (50)  // ~13'c

static const U8 OnOffCountList[ PREHEAT_REGION_NUM ] = 
{
    PREHEAT_REGION_1_COUNT,
    PREHEAT_REGION_2_COUNT
};

// ~ 13'c
static const U16 OnOffTimeList_Level_1[ PREHEAT_REGION_1_COUNT ] = 
{ 
    /* ON , OFF */
    4200U, 0U,          // 420sec @100ms
};

// 13'c ~ 20'c
static const U16 OnOffTimeList_Level_2[ PREHEAT_REGION_2_COUNT ] = 
{ 
    /* ON , OFF */
    2400U, 0U,          // 240sec @100ms
};

U8 GetPreheatRegion( TEMP_T _tAmbi )
{
    if( _tAmbi >= 13.0f )   // over 13'c
    {
        return PREHEAT_REGION_2;
    }
    
    return PREHEAT_REGION_1;   // under 13'c
}

U8 GetPreheatRepeatNum(U8 mu8Region )
{
    return OnOffCountList[ mu8Region ];
}

U8 GetPreheatCmd(U8 mu8Index)
{
    /* Odd:TurnOn, Even:TurnOff */

    if( ( mu8Index % 2 == 0 ) )
    {
        return ON;   // turn on deicing heater
    }

    return OFF;   // turn off deicing heater
}

U16 GetPreheatTime(U8 mu8Region, U8 mu8Index)
{
    U16 mu16Time;

    // Time
    if( mu8Region == PREHEAT_REGION_1 )
    {
        mu16Time = OnOffTimeList_Level_1[ mu8Index ];
    }
    else
    {
        mu16Time = OnOffTimeList_Level_2[ mu8Index ];
    }

    return mu16Time;
}

U8 GetPreheatCompRps(U8 mu8Region)
{
    U8 mu8Rps = COMP_RPS_A;

    // Rps
    if( mu8Region == PREHEAT_REGION_1 )       // under 14'c
    {
        mu8Rps  = COMP_RPS_B;
    }
    else if( mu8Region == PREHEAT_REGION_2 )  // over 14'c
    {
        mu8Rps  = COMP_RPS_A;
    }

    return mu8Rps;
}

