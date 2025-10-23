
/*******************************************************************************
&Description :
    A1010 ~ A101x
    Function Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/

#include "WIFI_A101x_FunctionList.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/
// #include "include/_type_define.h"
// #include "_fix_ram.h"
// #include "cold.h"
/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Event List */
U16 WifiFuncEventPreVal[USER_DATA_MAX_SIZE] = {0};

static const WifiTxFuncEventList_T WifiFuncEventList[] = 
{   /*  API ID                                      DATA_TYPE                                 PreVal(Initial)                         EnventForm     */
    {   WIFI_FUNC_0002_COLD_SEL,                TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0003_HOT_LOCK_SEL,            TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },   
    {   WIFI_FUNC_0005_LOCK_SEL,                TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },   
    // {   WIFI_FUNC_0006_MUTE_SEL,                TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0008_ICE_SEL,                 TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_000B_WATER_SEL,               TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_000D_WATER_OUTQUANTITY,       TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   // 0 -> ??
    {   WIFI_FUNC_0024_HEART_TIME4,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   // 0 -> ?? 
    {   WIFI_FUNC_002C_SILVER_CARE,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_0030_ICE_MAKETIME,            TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   // 0 -> ??
    // {   WIFI_FUNC_0032_ICELOCK_SEL,             TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0035_COVER1_OPEN,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0039_HOT_SEL,                 TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_003B_HOT_TEMP_SEL,            TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    {   WIFI_FUNC_0040_SMART_CHECK_RQST,        TYPE_SMART_DIAGNOSIS_CONTROL,        (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0041_SMART_CHECK_STATUS,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0042_SMART_CHECK_PROGRESS,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0047_WATER_QUANTITY_SEL,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0049_OUT_BUTTON_LIGHT,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0050_UV_STERILIZE_STATUS,     TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_0051_QUANTITY_USE_SEL,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0059_ICE_ONOFF_SEL,           TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_005C_SMART_CHECK_RESULT,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_005E_SLEEP_MODE_SEL,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_005F_SLEEP_START_HOUR,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_0060_SLEEP_START_MIN,         TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_0061_SLEEP_END_HOUR,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_0062_SLEEP_END_MIN,           TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL,    TYPE_A1014,                          (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0077_HOT_TARGET_TEMP_TIME,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   //0 -> ??
    // {   WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME,   TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   //0 -> ??
    // {   WIFI_FUNC_008B_HEATER_STATUS,           TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_008C_COMP_STATUS,             TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_STATUS_CHANGE },
    // {   WIFI_FUNC_0093_ICEMAKING_STAUTS,        TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME,  TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   //0 -> ??
    // {   WIFI_FUNC_0096_DEICING_COMPLET_TIME,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_OTHER },   //0 -> ??
    // {   WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL,   TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_00A0_COLD_TANK_TEMP,          TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_00A1_HOT_TANK_TEMP,           TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_UNUSED        },
    // {   WIFI_FUNC_00B5_FILTER_FLUSHING,         TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_00CB_WATER_EXTRACT_STATUS,    TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_00CC_ICE_EXTRACT_STATUS,      TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
    // {   WIFI_FUNC_00CD_SETUP_MODE_STATUS,       TYPE_UNUSED,                         (U16*)(&WifiFuncEventPreVal),         EVENT_ZERO_TO_ONE   },
};

#define SZ_FUNC_EVENT_LIST    (sizeof(WifiFuncEventList)/sizeof(WifiTxFuncEventList_T))

void* GetWifiFuncDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiFuncEventList;
    }
    else
    {

    }
    return NULL;
}
U16 GetWifiFuncEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_FUNC_EVENT_LIST;
    }
    else
    {

    }
    return NULL;
}


/* [User Function Data] Product -> Server */
U16 GetUserSystemFunction ( U16 mu16Func )
{
    U16 mu16Data = 0U;
    U16 buf = 0;


    switch ( mu16Func )
    {
        case WIFI_FUNC_0002_COLD_SEL :       /* ID */

            // mu16Data = 1;                /* Value */

        break;
        case WIFI_FUNC_0003_HOT_LOCK_SEL : 

        break;
        case WIFI_FUNC_0005_LOCK_SEL : 
            
        break;
        case WIFI_FUNC_0006_MUTE_SEL : 
           
        break;
        case WIFI_FUNC_0008_ICE_SEL : 
            
        break;
        case WIFI_FUNC_000B_WATER_SEL : 
            
        break;
        case WIFI_FUNC_000D_WATER_OUTQUANTITY : 
            
        break;
        case WIFI_FUNC_0024_HEART_TIME4 : 
            
        break;
        case WIFI_FUNC_002C_SILVER_CARE : 
            
        break;
        case WIFI_FUNC_0030_ICE_MAKETIME : 
            
        break;
        case WIFI_FUNC_0032_ICELOCK_SEL : 
            
        break;
        case WIFI_FUNC_0035_COVER1_OPEN : 
            
        break;
        case WIFI_FUNC_0039_HOT_SEL : 
            
        break;
        case WIFI_FUNC_0040_SMART_CHECK_RQST : 
            
        break;
        case WIFI_FUNC_0041_SMART_CHECK_STATUS : 
            
        break;
        case WIFI_FUNC_0042_SMART_CHECK_PROGRESS : 
            
        break;
        case WIFI_FUNC_0047_WATER_QUANTITY_SEL : 
            
        break;
        case WIFI_FUNC_0049_OUT_BUTTON_LIGHT : 
            
        break;
        case WIFI_FUNC_0050_UV_STERILIZE_STATUS : 
            
        break;
        case WIFI_FUNC_0051_QUANTITY_USE_SEL : 
            
        break;
        case WIFI_FUNC_0059_ICE_ONOFF_SEL : 
            
        break;
        case WIFI_FUNC_005C_SMART_CHECK_RESULT : 
            
        break;
        case WIFI_FUNC_005D_UV_ICE_FAUCET_STATUS : 
            
        break;
        case WIFI_FUNC_005E_SLEEP_MODE_SEL : 
            
        break;
        case WIFI_FUNC_005F_SLEEP_START_HOUR : 
            
        break;
        case WIFI_FUNC_0060_SLEEP_START_MIN : 
            
        break;
        case WIFI_FUNC_0061_SLEEP_END_HOUR : 
            
        break;
        case WIFI_FUNC_0062_SLEEP_END_MIN : 
            
        break;
        case WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL : 
            
        break;
        case WIFI_FUNC_0077_HOT_TARGET_TEMP_TIME : 
            
        break;
        case WIFI_FUNC_0078_COLD_TARGET_TEMP_TIME : 
            
        break;
        case WIFI_FUNC_008B_HEATER_STATUS : 
            
        break;
        case WIFI_FUNC_008C_COMP_STATUS : 
            
        break;
        case WIFI_FUNC_0093_ICEMAKING_STAUTS : 
            
        break;
        case WIFI_FUNC_0095_ICEMAKING_COMPLET_TIME : 
            
        break;
        case WIFI_FUNC_0096_DEICING_COMPLET_TIME : 
            
        break;
        case WIFI_FUNC_009B_DRAINTANK_LOW_WLEVEL : 
            
        break;
        case WIFI_FUNC_009C_DRAINTANK_HIGH_WLEVEL : 
            
        break;
        case WIFI_FUNC_00A0_COLD_TANK_TEMP : 
            
        break;
        case WIFI_FUNC_00A1_HOT_TANK_TEMP : 
            
        break;
        case WIFI_FUNC_00B5_FILTER_FLUSHING : 
            
        break;
        case WIFI_FUNC_00CB_WATER_EXTRACT_STATUS : 
            
        break;
        case WIFI_FUNC_00CC_ICE_EXTRACT_STATUS : 
            
        break;
        case WIFI_FUNC_00CD_SETUP_MODE_STATUS : 
            
        break;

        default:
            mu16Data = 0U;
        break;
    }
    return mu16Data;
}


/* [Set User Function Data] App Control(Server) -> Product */
void SetUserSystemFunction ( U16 mu16Func, U16 mData )
{
    switch ( mu16Func )
    {
        case WIFI_FUNC_0002_COLD_SEL :

        break;
        case WIFI_FUNC_0003_HOT_LOCK_SEL :

        break;
        case WIFI_FUNC_0005_LOCK_SEL :

        break;
        case WIFI_FUNC_0006_MUTE_SEL :

        break;
        case WIFI_FUNC_0008_ICE_SEL :

        break;
        case WIFI_FUNC_0032_ICELOCK_SEL :

        break;
        case WIFI_FUNC_0039_HOT_SEL :

        break;

        case WIFI_FUNC_0040_SMART_CHECK_RQST :

        break;
        case WIFI_FUNC_0047_WATER_QUANTITY_SEL :

        break;
        case WIFI_FUNC_0049_OUT_BUTTON_LIGHT :

        break;
        case WIFI_FUNC_0051_QUANTITY_USE_SEL :

        break;
        case WIFI_FUNC_0059_ICE_ONOFF_SEL :

        break;
        case WIFI_FUNC_005C_SMART_CHECK_RESULT :

        break;
        case WIFI_FUNC_005E_SLEEP_MODE_SEL :

        break;
        case WIFI_FUNC_005F_SLEEP_START_HOUR :

        break;
        case WIFI_FUNC_0060_SLEEP_START_MIN :

        break;
        case WIFI_FUNC_0061_SLEEP_END_HOUR :

        break;
        case WIFI_FUNC_0062_SLEEP_END_MIN :

        break;
        case WIFI_FUNC_0066_DEFAULT_QUANTITY_SEL :

        break;


        default:
        break;
    }
}

/* User Data Clear (Called when wifi is not connected) */
void ClearUserSystemDataA1010 ( void )
{
    
}