/**
 * File : dummy_deicing_table.h
 * 
 * Dummy Deicing Table
*/
#include "dummy_deicing_table.h"

/* DUMMY DE-ICING REGION LIST NUM */
#define  DUMMY_DEICING_REGION_1_COUNT   (2)   // ~13'c
#define  DUMMY_DEICING_REGION_2_COUNT   (2)   // ~22'c
#define  DUMMY_DEICING_REGION_3_COUNT   (2)   // ~30'c
#define  DUMMY_DEICING_REGION_4_COUNT   (2)   // 30'c~
#define	DUMMY_DEICING_REGION_NUM	      (4)

#define DUMMY_DEICING_COMP_ON_TIME      (3U*600U) /* 3min, @100msec */

static const U8 OnOffCountList[DUMMY_DEICING_REGION_NUM] = 
{
    DUMMY_DEICING_REGION_1_COUNT,
    DUMMY_DEICING_REGION_2_COUNT,
    DUMMY_DEICING_REGION_3_COUNT,
    DUMMY_DEICING_REGION_4_COUNT
};

/* Á¦ºù ¸ðµå */
/// ~ 13'c
static const U16 CompOnOffTimeList_Level_1[DUMMY_DEICING_REGION_1_COUNT] = 
{ 
    /* ON , OFF */
    400, 0,            /// 40sec
};

// 13'c ~ 22'c
static const U16 CompOnOffTimeList_Level_2[DUMMY_DEICING_REGION_2_COUNT] = 
{ 
    /* ON , OFF */
    400, 0,            /// 40sec
};

// 22'c ~ 30'c 
static const U16 CompOnOffTimeList_Level_3[DUMMY_DEICING_REGION_3_COUNT] = 
{ 
    /* ON , OFF */
    300, 0,             // 30sec
};

// 30'c ~
static const U16 CompOnOffTimeList_Level_4[DUMMY_DEICING_REGION_4_COUNT] = 
{ 
    /* ON , OFF */
    150, 0,             // 15sec
};

/* ÇÖ°¡½º ¸ðµå */
// ~ 13'c
static const U16 OnOffTimeList_Level_1[DUMMY_DEICING_REGION_1_COUNT] = 
{ 
    /* ON , OFF */
    3000, 0,            // 300sec
};

// 13'c ~ 22'c
static const U16 OnOffTimeList_Level_2[DUMMY_DEICING_REGION_2_COUNT] = 
{ 
    /* ON , OFF */
    1800, 0,            // 180sec
};

// 22'c ~ 30'c 
static const U16 OnOffTimeList_Level_3[DUMMY_DEICING_REGION_3_COUNT] = 
{ 
    /* ON , OFF */
    500, 0,             // 50sec
};

// 30'c ~
static const U16 OnOffTimeList_Level_4[DUMMY_DEICING_REGION_4_COUNT] = 
{ 
    /* ON , OFF */
    200, 0,             // 20sec
};

U8 GetDummyDeIcingRegion(TEMP_T _tAmbi)
{
    if( _tAmbi >= 30.0f )  // over 30'c
    {
        return DUMMY_DEICING_REGION_4;
    }
    else if( _tAmbi >= 22.0f )   // over 22'c
    {
        return DUMMY_DEICING_REGION_3;
    }
    else if( _tAmbi >= 13.0f )   // over 13'c
    {
        return DUMMY_DEICING_REGION_2;
    }
    
    return DUMMY_DEICING_REGION_1;   // under 13'c
}

U8 GetDummyDeIcingRepeatNum(U8 region )
{
    return OnOffCountList[region];
}

U8 GetDummyDeIcingCmd(U8 index)
{
    /* Odd:TurnOn, Even:TurnOff */
    if( ( index % 2 == 0 ) )
    {
        return ON;   // turn on deicing heater
    }

    return OFF;   // turn off deicing heater
}

U16 GetDummyDeIcingTime(U8 region, U8 index)
{
    U16 time;

    // Time
    if( region == DUMMY_DEICING_REGION_1 )
    {
        time = OnOffTimeList_Level_1[index];
    }
    else if( region == DUMMY_DEICING_REGION_2 )
    {
        time = OnOffTimeList_Level_2[index];
    }
    else if( region == DUMMY_DEICING_REGION_3 )
    {
        time = OnOffTimeList_Level_3[index];
    }
    else 
    {
        time = OnOffTimeList_Level_4[index];
    }

    return time;
}

U16 GetDummyDeIcingCompOnTime(U8 region, U8 index)
{
#if 1

    U16 time;

    // Time
    if( region == DUMMY_DEICING_REGION_1 )
    {
        time = CompOnOffTimeList_Level_1[index];
    }
    else if( region == DUMMY_DEICING_REGION_2 )
    {
        time = CompOnOffTimeList_Level_2[index];
    }
    else if( region == DUMMY_DEICING_REGION_3 )
    {
        time = CompOnOffTimeList_Level_3[index];
    }
    else 
    {
        time = CompOnOffTimeList_Level_4[index];
    }

    return time;

#else

    (void)mu8Region;

    return DUMMY_DEICING_COMP_ON_TIME;

#endif
}
