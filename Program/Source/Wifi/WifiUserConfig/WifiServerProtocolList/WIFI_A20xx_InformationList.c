
/*******************************************************************************
&Description :
    A2010 ~ A201x / A2021
    Information Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/

#include "WIFI_A20xx_InformationList.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/


/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/


/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/

/* (1) Initial or 24H cycle                 : TYPE_INFO_TIME / TYPE_INFO_NORMAL / TYPE_INFO_WEATHER / TYPE_INFO_TEMP / TYPE_INFO_JSON */
/* (2) WifiSendData(WIFI_DATA_INFO) Reqest  : TYPE_INFO_TIME / TYPE_INFO_NORMAL / TYPE_INFO_WEATHER */
/* (3) 20Min cycle                          : TYPE_INFO_NORMAL / TYPE_INFO_WEATHER / TYPE_INFO_TEMP */
/* (4) Smart Daignosis Result receive (Server->Product) : TYPE_INFO_DIAGNOSIS_RQ (Displaying results on LCD)*/
/* (5) GetWifiRequestSAP() Reqest           : TYPE_INFO_JSON */

/* Event List */
static const WifiNormalList_T WifiInfoProtocolList[] =
{   /*  API ID                                      Data Type     */
	{   WIFI_INFO_0000_YEAR,                     TYPE_INFO_TIME         },	
	{   WIFI_INFO_0001_MONTH,                    TYPE_INFO_TIME         },  
	{   WIFI_INFO_0002_DATE,                     TYPE_INFO_TIME         },  
	{   WIFI_INFO_0003_HOUR,                     TYPE_INFO_TIME         },  
	{   WIFI_INFO_0004_MINUTE,                   TYPE_INFO_TIME         },  
	{   WIFI_INFO_0005_SECOND,                   TYPE_INFO_TIME         },  
	{   WIFI_INFO_0006_DAY,                      TYPE_INFO_TIME         },  
    {   WIFI_INFO_0007_PART_PERIOD,              TYPE_INFO_NORMAL       },
    
    {   WIFI_INFO_0010_WEATHER,                  TYPE_INFO_WEATHER      },
    {   WIFI_INFO_0011_AQI,                      TYPE_INFO_AQI          },
    // {   WIFI_INFO_0012_PM10,                     TYPE_INFO_AQI          },
    // {   WIFI_INFO_0013_PM25,                     TYPE_INFO_AQI          },
    {   WIFI_INFO_0014_CURRENT_TEMP,             TYPE_INFO_TEMP         },	
    {   WIFI_INFO_0015_HIGH_TEMP,                TYPE_INFO_TEMP         },	
    {   WIFI_INFO_0016_LOW_TEMP,                 TYPE_INFO_TEMP         },
    
    // {   WIFI_INFO_0020_LAST_HEART_YEAR,          TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0021_LAST_HEART_MONTH,         TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0022_LAST_HEART_DAY,           TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0023_NEXT_HEART_YEAR,          TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0024_NEXT_HEART_MONTH,         TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0025_NEXT_HEART_DAY,           TYPE_INFO_NORMAL       },
    {   WIFI_INFO_0026_FILTER_CHANGE_DAY1,       TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0027_FILTER_CHANGE_DAY2,       TYPE_INFO_NORMAL       },   
    // {   WIFI_INFO_0028_FILTER_CHANGE_CYCLE,      TYPE_INFO_NORMAL       },
    // {   WIFI_INFO_0029_DIAGNOSIS_RESULT_REQUEST, TYPE_INFO_DIAGNOSIS_RQ },
    // {   WIFI_INFO_0030_STERILIZE_DATE,           TYPE_INFO_JSON         },
    // {   WIFI_INFO_0031_SUPPLAY,                  TYPE_INFO_JSON         },
    // {   WIFI_INFO_0032_ENVIRONMENT,              TYPE_INFO_JSON         } 
};


#define SZ_INFO_EVENT_LIST    (sizeof(WifiInfoProtocolList)/sizeof(WifiNormalList_T))

void* GetWifiInfoDataEventList ( void )
{
    return WifiInfoProtocolList;
}

U16 GetWifiInfoEventListSize ( void )
{
    return SZ_INFO_EVENT_LIST;
}


#if (USER_USE_A2021_SAPINFO == 1)
/************************************ Json DATA  / Parsing ************************************/
/*********** JSON SAMPLE ***********/
/*
A2021.0030:{
    c:"S1000",
    fc:{
        ld:"202409070942",
        nd:"202409071542"
    }
} 
*/
// JSON Struct
JSONObject JsonObjectDepth_1; 
JSONObject JsonObjectDepth_2;
JSONArray JsonArray; 
JSONOArrayValue JsonArrayValue;

/* JSON Key / DATA */
/* 0030 STERILIZATION KEY */
const signed char JSON_KEY_LAST_DATE[] 					= "ld"; 	// LastDate
const signed char JSON_KEY_NEXT_DATE[] 					= "nd"; 	// NextDate
const signed char JSON_KEY_UV_FAUCETINFO[] 				= "fc"; 	// FaucetInfo

/* 0031 SUPPLIES KEY */
const signed char JSON_KEY_ARRAY_SUPPLIES[] 			= "sl";  	// Supplies
const signed char JSON_KEY_SUPPLIES_SEQ[] 				= "ss"; 	// Supplies SEQ
const signed char JSON_KEY_SUPPLIES_MEMBERSHIP_TYPE[] 	= "mt"; 	// Membershop type
const signed char JSON_KEY_SUPPLIES_REMAIN[] 			= "fr"; 	// Remain 잔여량 (%)

/* 0032 ENVIRONMENT KEY */
const signed char JSON_KEY_SAVE_PLASTIC[] 				= "spb"; 	// Plastic
const signed char JSON_KEY_CARBON_SAVINGS[] 			= "cs"; 	// Carbon
const signed char JSON_KEY_MATURE_CARBON_TREES[] 		= "mct"; 	// Carbon Trees


WIFI_JSON_VALUE_T gJsonValue;
static const WIFI_JSON_TABLE_T 	WifiJsonDataList[] =
{   //  ID                  			    i16Func                                     												
    {   WIFI_JSON_UV_FAUCET_LD,   			(I16*)&gJsonValue.UV_Faucet.u8JsonDateLD			},
    {   WIFI_JSON_UV_FAUCET_ND,   			(I16*)&gJsonValue.UV_Faucet.u8JsonDateND			},
};

void SetUserSystemJsonObject ( U16 mu16Info, I8 *pBuf )
{
	U8 mu8KeyIndex = 0;
	U8 mu8SAPCode = 0;
	switch ( mu16Info )
    {
		case WIFI_INFO_JSON_0030_UV_DATE:
			_MEMSET_( (void __far*) &JsonObjectDepth_1, 0, sizeof(JSONObject) );
            _MEMSET_( (void __far*) &JsonObjectDepth_2, 0, sizeof(JSONObject) );

			cJsonParseJson(pBuf, &JsonObjectDepth_1, NULL);			// Object O, Array X - Json parsing Depth1 c:"S1000"

			mu8SAPCode = JsonSAPCodeSuccess(&JsonObjectDepth_1);
			if (mu8SAPCode == F1000)                                // SAP 접속 실패 (CODE : F1000, SAP001)
			{
				SetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE, F1000);
				return;
			}
			else if (mu8SAPCode == E9000) // E9000
			{
				SetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE, E9000);
			}
			else	// S1000
			{
				SetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE, S1000);
			}

			mu8KeyIndex = (U8)GetJsonKeyIndexSearch(JSON_KEY_UV_FAUCETINFO, &JsonObjectDepth_1);   // Key "fc"
			if(mu8KeyIndex != (U8)0xFF){ // Index 찾기 완료 시
				cJsonParseJson(JsonObjectDepth_1.items[mu8KeyIndex].value, &JsonObjectDepth_2, NULL); // Depth2 Parsing - Object -> "fc": { Key:Value, Key:Value }
				/* 파우셋 UV 최근 동작시간 */
				GetJsonDateValue(gJsonValue.UV_Faucet.u8JsonDateLD, JSON_KEY_LAST_DATE, &JsonObjectDepth_2);
				SetUserSystemJson(WIFI_JSON_UV_FAUCET_LD);
				/* 파우셋 UV 동작 예정시간 */
				GetJsonDateValue(gJsonValue.UV_Faucet.u8JsonDateND, JSON_KEY_NEXT_DATE, &JsonObjectDepth_2);
				SetUserSystemJson(WIFI_JSON_UV_FAUCET_ND);
			}
		break;
		default:
			
			break;

	}
}

void SetUserSystemJson (U8 mu8JsonKey)
{
	I16 *pi16data = 0;
	U8  *pu8data = 0;

	/* Ex) */
	// U8 	mu8work[6] = {0};
	I16 mi16work = 0;

	WIFI_JSON_TABLE_T *pList = NULL;
	pList = WifiJsonDataList;

	pu8data = (U8*)((pList+mu8JsonKey)->i16data);
	pi16data = (pList+mu8JsonKey)->i16data;

	switch ( mu8JsonKey )
    {
		/* ex: 파우셋 UV 최근 동작시간 : YY MM DD HH MM */
		case WIFI_JSON_UV_FAUCET_LD:
		/* ex)
			mu8work[0] = (U8)pu8data[WIFI_JSON_T1_YEAR];
			mu8work[1] = (U8)pu8data[WIFI_JSON_T1_MONTH];
			mu8work[2] = (U8)pu8data[WIFI_JSON_T1_DAY];
			mu8work[3] = (U8)pu8data[WIFI_JSON_T1_HOUR];
			mu8work[4] = (U8)pu8data[WIFI_JSON_T1_MIN]; */
		break;
		/* 파우셋 UV 동작 예정시간 : YY MM DD HH MM */
		case WIFI_JSON_UV_FAUCET_ND:
		/* ex)
			mu8work[0] = (U8)pu8data[WIFI_JSON_T1_YEAR];
			mu8work[1] = (U8)pu8data[WIFI_JSON_T1_MONTH];
			mu8work[2] = (U8)pu8data[WIFI_JSON_T1_DAY];
			mu8work[3] = (U8)pu8data[WIFI_JSON_T1_HOUR];
			mu8work[4] = (U8)pu8data[WIFI_JSON_T1_MIN]; */
		break;

		default:
		break;
	}

}

#endif