    
/*******************************************************************************
&Description :
    A1080 ~ A108x
    A1090 ~ A109x
    Part Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/

/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_A108x_A109x_PartList.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/

/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/

/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Protocol List */
static const WifiNormalList_T WifiPartProtocolList[] =
{   /*  API ID        */
    {   0x0000,              TYPE_UNSIGNED   },
    // {   WIFI_PART_0001_VALVE_NOS,               TYPE_UNSIGNED   },
    // {   WIFI_PART_0002_VALVE_HOT_OUT,           TYPE_UNSIGNED   },
    // {   WIFI_PART_0003_VALVE_PURE_OUT,          TYPE_UNSIGNED   },
    // {   WIFI_PART_0004_VALVE_COLD_OUT,          TYPE_UNSIGNED   },
    // {   WIFI_PART_000C_VALVE_COLD_IN,           TYPE_UNSIGNED   },
    // {   WIFI_PART_000D_VALVE_COLD_DRAIN,        TYPE_UNSIGNED   },
    // {   WIFI_PART_0012_VALVE_DRAIN_NOS,         TYPE_UNSIGNED   },
    // {   WIFI_PART_0015_VALVE_ICE_TRAY_IN,       TYPE_UNSIGNED   },

    // {   WIFI_PART_0105_LEVEL_PURE_OVER,         TYPE_UNSIGNED   },
    // {   WIFI_PART_0106_LEVEL_PURE_HIGH,         TYPE_UNSIGNED   },
    // {   WIFI_PART_0108_LEVEL_PURE_LOW,          TYPE_UNSIGNED   },
    // {   WIFI_PART_010B_LEVEL_DRAIN_HIGH,        TYPE_UNSIGNED   },
    // {   WIFI_PART_010D_LEVEL_DRAIN_LOW,         TYPE_UNSIGNED   },
    // {   WIFI_PART_0112_LEVEL_LOW_ERROR,         TYPE_UNSIGNED   },
    // {   WIFI_PART_0113_LEVEL_HIGH_ERROR,        TYPE_UNSIGNED   },
    // {   WIFI_PART_0114_LEVEL_LOGIC_ERROR,       TYPE_UNSIGNED   },
    // {   WIFI_PART_0115_LEVEL_FEED_ERROR,        TYPE_UNSIGNED   },

    // {   WIFI_PART_0206_COLD_TEMP_1,             TYPE_UNSIGNED   },
    // {   WIFI_PART_0208_COLD_ROOM_TEMP,          TYPE_UNSIGNED   },
    // {   WIFI_PART_020B_COLD_TARGET_TEMP_TIME,   TYPE_UNSIGNED   },

    // {   WIFI_PART_0301_HOT_TEMP,                TYPE_UNSIGNED   },
    // {   WIFI_PART_0308_HOT_TARGET_TEMP_TIME,    TYPE_UNSIGNED   },

    // {   WIFI_PART_0402_SENSOR_LEAK,             TYPE_UNSIGNED   },
    // {   WIFI_PART_0409_SENSOR_TANK_REED,        TYPE_UNSIGNED   },

    // {   WIFI_PART_0501_STER_UV_FAUCET,          TYPE_UNSIGNED   },
    // {   WIFI_PART_0502_STER_UV_FAUCET_ICE,      TYPE_UNSIGNED   },
    // {   WIFI_PART_0503_STER_UV_ICE_TANK1,       TYPE_UNSIGNED   },

    // {   WIFI_PART_0800_ICE_PURE_TEMP,           TYPE_UNSIGNED   },
    // {   WIFI_PART_0801_ICE_FULL_SENSOR,         TYPE_UNSIGNED   },
    // {   WIFI_PART_0802_ICE_SWING_BAR,           TYPE_UNSIGNED   },
    // {   WIFI_PART_0805_ICE_TRAY_SENSING_SW,     TYPE_UNSIGNED   },
    // {   WIFI_PART_0807_ICE_DOOR_STEPMOTOR,      TYPE_UNSIGNED   },
    // {   WIFI_PART_0808_ICE_COURSE_CHANGE_VV,    TYPE_UNSIGNED   },
    // {   WIFI_PART_080A_ICE_DRAIN_PUMP,          TYPE_UNSIGNED   },
    // {   WIFI_PART_080E_ICE_DRAIN_ERROR_E60,     TYPE_UNSIGNED   },
    // {   WIFI_PART_080F_ICE_TRAY_ERROR_E61,      TYPE_UNSIGNED   },
    // {   WIFI_PART_0810_ICE_TRAY_ERROR_E62,      TYPE_UNSIGNED   },
    // {   WIFI_PART_0811_ICE_TRAY_ERROR_E63,      TYPE_UNSIGNED   }
};

#define SZ_PART_EVENT_LIST    (sizeof(WifiPartProtocolList)/sizeof(WifiNormalList_T))

void* GetWifiPartDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiPartProtocolList;
    }
    else
    {

    }
    
    return NULL;
}

U16 GetWifiPartEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_PART_EVENT_LIST;
    }
    else
    {

    }
    
    return NULL;
}

/* [User Part Data] Product -> Server */
U16 GetUserSystemPart ( U16 mu16Part )
{
    U16 mu16Data = 9999U;
switch ( mu16Part )
    {
        case 0x0000 :       /* ID */
            // mu16Data = 1;                    /* Value */
        break;

        case 0x0001 : 

        break;
        case WIFI_PART_0002_VALVE_HOT_OUT : 

        break;
        case WIFI_PART_0003_VALVE_PURE_OUT : 

        break;
        case WIFI_PART_0004_VALVE_COLD_OUT : 

        break;
        case WIFI_PART_000C_VALVE_COLD_IN : 

        break;
        case WIFI_PART_000D_VALVE_COLD_DRAIN : 

        break;
        case WIFI_PART_0012_VALVE_DRAIN_NOS : 

        break;
        case WIFI_PART_0015_VALVE_ICE_TRAY_IN : 

        break;

        case WIFI_PART_0105_LEVEL_PURE_OVER : 

        break;
        case WIFI_PART_0106_LEVEL_PURE_HIGH : 

        break;
        case WIFI_PART_0108_LEVEL_PURE_LOW : 

        break;
        case WIFI_PART_010B_LEVEL_DRAIN_HIGH : 

        break;
        case WIFI_PART_010D_LEVEL_DRAIN_LOW : 

        break;
        case WIFI_PART_0112_LEVEL_LOW_ERROR : 

        break;
        case WIFI_PART_0113_LEVEL_HIGH_ERROR : 

        break;
        case WIFI_PART_0114_LEVEL_LOGIC_ERROR : 

        break;
        case WIFI_PART_0115_LEVEL_FEED_ERROR : 

        break;

        case WIFI_PART_0206_COLD_TEMP_1 : 

        break;
        case WIFI_PART_0208_COLD_ROOM_TEMP : 

        break;
        case WIFI_PART_020B_COLD_TARGET_TEMP_TIME : 

        break;

        case WIFI_PART_0301_HOT_TEMP : 

        break;
        case WIFI_PART_0308_HOT_TARGET_TEMP_TIME : 

        break;
        case WIFI_PART_0402_SENSOR_LEAK : 

        break;
        case WIFI_PART_0409_SENSOR_TANK_REED : 

        break;

        case WIFI_PART_0501_STER_UV_FAUCET : 

        break;
        case WIFI_PART_0502_STER_UV_FAUCET_ICE : 

        break;
        case WIFI_PART_0503_STER_UV_ICE_TANK1 : 

        break;

        case WIFI_PART_0800_ICE_PURE_TEMP : 

        break;
        case WIFI_PART_0801_ICE_FULL_SENSOR : 

        break;
        case WIFI_PART_0802_ICE_SWING_BAR : 

        break;
        case WIFI_PART_0805_ICE_TRAY_SENSING_SW : 

        break;
        case WIFI_PART_0807_ICE_DOOR_STEPMOTOR : 

        break;
        case WIFI_PART_0808_ICE_COURSE_CHANGE_VV : 

        break;
        case WIFI_PART_080A_ICE_DRAIN_PUMP : 

        break;
        case WIFI_PART_080E_ICE_DRAIN_ERROR_E60 : 

        break;
        case WIFI_PART_080F_ICE_TRAY_ERROR_E61 : 

        break;
        case WIFI_PART_0810_ICE_TRAY_ERROR_E62 : 

        break;
        case WIFI_PART_0811_ICE_TRAY_ERROR_E63 : 

        break;


        default:
		mu16Data = 0xFFFF;
        break;
    }

    return mu16Data;
}

/* [A1091] [Diagnosis Result]  Server -> Product */
/* SetWifiSendStatus( TX_DATA, WIFI_TX_RQST_INFO_DRR );  A2010.0029 DiagnosisResultRequest  */ 
void SetUserSystemDiagnosis ( U16 mu16DR, I16 mData ) // [1]: Error 있음, [-100] : 이상1
{
	#define DiagnosisError 	 (1)
	#define DiagnosisAbnormal1 (-100)
	#define DiagnosisAbnormal2 (-200)

	switch ( mu16DR )
    {
		// case 0x0001:
		// 	if (mData == DiagnosisError)
		// 	{

		// 	}
        // 	else if (mData == DiagnosisAbnormal1)
		// 	{

		// 	}
		// 	break;


		default:

        break;
	}
}