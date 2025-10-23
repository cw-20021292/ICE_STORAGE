/**
 * File : deicing_table.h
 * 
 * Deicing Table
*/
#include "deicing_table.h"

/* DE-ICING REGION LIST NUM */
#define  DEICING_REGION_1_COUNT   (2)   // ~13'c
#define  DEICING_REGION_2_COUNT   (2)   // ~22'c
#define  DEICING_REGION_3_COUNT   (2)   // ~30'c
#define  DEICING_REGION_4_COUNT   (2)   // 30'c~
#define	DEICING_REGION_NUM	      (4)

#define COMP_RPS_A    (25)  // 30'c~
#define COMP_RPS_B    (25)  // ~30'c
#define COMP_RPS_C    (35)  // ~22'c
#define COMP_RPS_D    (45)  // ~13'c

static const U8 OnOffCountList[ DEICING_REGION_NUM ] = 
{
    DEICING_REGION_1_COUNT,
    DEICING_REGION_2_COUNT,
    DEICING_REGION_3_COUNT,
    DEICING_REGION_4_COUNT
};

// ~ 13'c
static const U16 OnOffTimeList_Level_1[ DEICING_REGION_1_COUNT ] = 
{ 
    /* ON , OFF */
    2500U, 0U,          // 250sec
};

// 13'c ~ 22'c
static const U16 OnOffTimeList_Level_2[ DEICING_REGION_2_COUNT ] = 
{ 
    /* ON , OFF */
    700U, 0U,          // 70sec
};

// 22'c ~ 30'c 
static const U16 OnOffTimeList_Level_3[ DEICING_REGION_3_COUNT ] = 
{ 
    /* ON , OFF */
    300U, 0U,          // 30sec
};

// 30'c ~
static const U16 OnOffTimeList_Level_4[ DEICING_REGION_4_COUNT ] = 
{ 
    /* ON , OFF */
    200U, 0U,          // 20sec
};


U8 GetDeIcingRegion(TEMP_T _tAmbi)
{
    if( _tAmbi >= 30.0f )  // over 30'c
    {
        return DEICING_REGION_4;
    }
    else if( _tAmbi >= 22.0f )   // over 22'c
    {
        return DEICING_REGION_3;
    }
    else if( _tAmbi >= 13.0f )   // over 13'c
    {
        return DEICING_REGION_2;
    }
    
    return DEICING_REGION_1;   // under 13'c
}

U8 GetDeIcingRepeatNum(U8 region)
{
    return OnOffCountList[region];
}

U8 GetDeIcingCmd(U8 index)
{
    /* Odd:TurnOn, Even:TurnOff */

    if( (index % 2 == 0) )
    {
        return ON;   // turn on deicing heater
    }

    return OFF;   // turn off deicing heater
}

U16 GetDeIcingTime(U8 region, U8 index)
{
    U16 time;

    // Time
    if( region == DEICING_REGION_1 )
    {
        time = OnOffTimeList_Level_1[index];
    }
    else if( region == DEICING_REGION_2 )
    {
        time = OnOffTimeList_Level_2[index];
    }
    else if( region == DEICING_REGION_3 )
    {
        time = OnOffTimeList_Level_3[index];
    }
    else 
    {
        time = OnOffTimeList_Level_4[index];
    }

    return time;
}

U8 GetDeIcingCompRps(U8 region)
{
    U8 rps = COMP_RPS_A;

    // Rps
    if( region == DEICING_REGION_1 )       // under 13'c
    {
        rps  = COMP_RPS_D;
    }
    else if( region == DEICING_REGION_2 )  // over 13'c
    {
        rps  = COMP_RPS_C;
    }
    else if( region == DEICING_REGION_3 )  // over 22'c
    {
        rps  = COMP_RPS_B;
    }
    else if( region == DEICING_REGION_4 )  // over 30'c
    {
        rps  = COMP_RPS_A;
    }

    return rps;
}

U8 GetDeIcingFanOnOff(U8 region)
{
    U8 fanOnOff = OFF;

    if( region == DEICING_REGION_1 )
    {
        fanOnOff  = OFF;
    }
    else if( region == DEICING_REGION_2 )
    {
        fanOnOff  = OFF;
    }
    else if( region == DEICING_REGION_3 )
    {
        fanOnOff  = OFF;
    }
    else if( region == DEICING_REGION_4 )
    {
        fanOnOff  = OFF;
    }

    return fanOnOff;
}
