
/*******************************************************************************
&Description :
    A1030 ~ A103x
    Error Protocol Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/

#include "WIFI_A103x_ErrorList.h"
/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/


/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/

/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Event List */
U16 WifiErrorPreVal[USER_ERROR_DATA_MAX_SIZE] = {0};      // Error Protocol count

static const WifiTxFuncEventList_T WifiErrorProtocolList[] =
{   /*  API ID                                   DATA_TYPE                PreVal(Initial)                         EnventForm     */
    {   WIFI_ERROR_0001_LOW_LEVEL,              TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0002_HIGH_LEVEL,             TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0003_COMPLEX_LEVEL,          TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0004_FEED_VALVE,             TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0008_COLD_TEMP,              TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0009_NATURAL_TEMP,           TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_000E_DRAIN_PUMP,             TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_001D_LEAK_SENSOR,            TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0020_ROOM_TEMP,              TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0021_MICRO_SW_DETECT,        TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0022_MICRO_SW_MOVE,          TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_0026_HOT_TANK_TEMP,          TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_003B_MICRO_SW_MOVE2,         TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
    // {   WIFI_ERROR_003C_ROOM_TEMP_OH,           TYPE_UNUSED,           (U16*)(&WifiErrorPreVal),              EVENT_STATUS_CHANGE },
};

#define SZ_ERR_EVENT_LIST    (sizeof(WifiErrorProtocolList)/sizeof(WifiTxFuncEventList_T))

void* GetWifiErrDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiErrorProtocolList;
    }
    else
    {

    }
    return NULL;
}

U16 GetWifiErrEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_ERR_EVENT_LIST;
    }
    else
    {

    }
    return NULL;
}


/* [User Error Data] Product -> Server */
U16 GetUserSystemError ( U16 mu16Err )
{
    U16 mu16Data = 0U;

    switch ( mu16Err ) // 0:Normal / 1:Error
    {
        case WIFI_ERROR_0001_LOW_LEVEL:
            mu16Data = 0;
        break;

        case WIFI_ERROR_0002_HIGH_LEVEL:
            mu16Data = 0;
        break;
        case WIFI_ERROR_0003_COMPLEX_LEVEL:
            mu16Data = 0;
        break;
        case WIFI_ERROR_0004_FEED_VALVE:
            mu16Data = 0;
        break;
        case WIFI_ERROR_0008_COLD_TEMP:
            mu16Data = 0;
        break;
        case WIFI_ERROR_0009_NATURAL_TEMP:
            mu16Data = 0;
        break;
        case WIFI_ERROR_000E_DRAIN_PUMP:
            mu16Data = 0;
        break;
        case WIFI_ERROR_001D_LEAK_SENSOR:
            mu16Data = 0;
        break;
        case WIFI_ERROR_0020_ROOM_TEMP:
            mu16Data = 0;
        break;
        case WIFI_ERROR_0021_MICRO_SW_DETECT:
            mu16Data = 0;
        break;
        case WIFI_ERROR_0022_MICRO_SW_MOVE:
            mu16Data = 0;
        break;
        case WIFI_ERROR_0026_HOT_TANK_TEMP:
            mu16Data = 0;
        break;
        case WIFI_ERROR_003B_MICRO_SW_MOVE2:
            mu16Data = 0;
        break;
        case WIFI_ERROR_003C_ROOM_TEMP_OH:
            mu16Data = 0;
        break;

        default:
            mu16Data = 0U;
        break;
    }
    return mu16Data;
}