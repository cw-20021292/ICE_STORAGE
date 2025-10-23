
#ifndef _WIFI_SetInfoList_H_
#define _WIFI_SetInfoList_H_

/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
// #include "WIFI_Config.h"
#include "WIFI_Common.h"
#include "WIFI_Status.h"
#include "WIFI_cJSON.h"


/******************************************************************************/
/***** Function ***************************************************************/
/******************************************************************************/
void* GetWifiInfoDataEventList ( void );
U16 GetWifiInfoEventListSize ( void );
/******************************************************************************/
/***** Define *****************************************************************/
/******************************************************************************/

typedef enum
{
    WIFI_INFO_0000_YEAR = 0x0000,
    WIFI_INFO_0001_MONTH,
    WIFI_INFO_0002_DATE,
    WIFI_INFO_0003_HOUR,
    WIFI_INFO_0004_MINUTE,
    WIFI_INFO_0005_SECOND,
    WIFI_INFO_0006_DAY,
    WIFI_INFO_0007_PART_PERIOD,
    
    WIFI_INFO_0010_WEATHER = 0x0010,
    WIFI_INFO_0011_AQI,
    WIFI_INFO_0012_PM10,
    WIFI_INFO_0013_PM25,
    WIFI_INFO_0014_CURRENT_TEMP,
    WIFI_INFO_0015_HIGH_TEMP,
    WIFI_INFO_0016_LOW_TEMP,

    WIFI_INFO_0020_LAST_HEART_YEAR = 0x0020,
    WIFI_INFO_0021_LAST_HEART_MONTH,
    WIFI_INFO_0022_LAST_HEART_DAY,
    WIFI_INFO_0023_NEXT_HEART_YEAR,
    WIFI_INFO_0024_NEXT_HEART_MONTH,
    WIFI_INFO_0025_NEXT_HEART_DAY,
    WIFI_INFO_0026_FILTER_CHANGE_DAY1,
    WIFI_INFO_0027_FILTER_CHANGE_DAY2,
    WIFI_INFO_0028_FILTER_CHANGE_CYCLE,
    WIFI_INFO_0029_DIAGNOSIS_RESULT_REQUEST, 

    WIFI_INFO_0030_STERILIZE_DATE = 0x0030, 
    WIFI_INFO_0031_SUPPLAY, 
    WIFI_INFO_0032_ENVIRONMENT, 
    
    WIFI_INFO_NUM
}E_API_A2010_T;

typedef enum
{
    WIFI_INFO_JSON_0030_UV_DATE     = 0x0030,
    WIFI_INFO_JSON_0031_SUPPLIES,
    WIFI_INFO_JSON_0032_ENVIRONMENT,

    WIFI_INFO_JSON_NUM
}E_API_A2020_T;

#if (USER_USE_A2021_SAPINFO == 1)
/************************************ Json DATA  / Parsing ************************************/
// SAP 응답 CODE
#define F1000  0U  //SAP001 포함
#define S1000  1U
#define E9000  2U

typedef enum{  // Json KEY
    WIFI_JSON_UV_FAUCET_LD = 0,
    WIFI_JSON_UV_FAUCET_ND,
    WIFI_JSON_SUPPLY_FILTER_SEQ ,
    WIFI_JSON_SUPPLY_FILTER_MT ,
    WIFI_JSON_SUPPLY_FILTER_LD ,
    WIFI_JSON_SUPPLY_FILTER_ND ,
    WIFI_JSON_SUPPLY_FILTER_FR ,
    WIFI_JSON_SUPPLY_FLOWMODULE_SEQ,
    WIFI_JSON_SUPPLY_FLOWMODULE_MT,
    WIFI_JSON_SUPPLY_FLOWMODULE_LD,
    WIFI_JSON_SUPPLY_FLOWMODULE_ND,
    WIFI_JSON_SUPPLY_FLOWMODULE_FR,
    WIFI_JSON_ENVIRONMENT_SPB,
    WIFI_JSON_ENVIRONMENT_SCS,
    WIFI_JSON_ENVIRONMENT_MCT,

    WIFI_JSON_KEY_MAX
}E_WIFI_JSON_KEY_T;

enum{  // Json UV DAY
    WIFI_JSON_UV_LASTDAY = 0,
    WIFI_JSON_UV_NEXTDAY,
    WIFI_JSON_UV_DAY_MAX
};

enum{  // Json Supply KEY
    WIFI_JSON_ARRAY_FILTER = 0,
    WIFI_JSON_ARRAY_FLOWMODULE,

    WIFI_JSON_SUPPLY_KEY_MAX
};

enum{  // JSON SUPPLY 
    WIFI_JSON_SUPPLY_LASTDAY= 0,
    WIFI_JSON_SUPPLY_NEXTDAY,
    WIFI_JSON_SEQ,
    WIFI_JSON_MEMBERSHIP,
    WIFI_JSON_REMAIN,

    WIFI_JSON_SUPPLY_VALUE_MAX
};

enum{ // JSON ENVIRONMENT
    WIFI_JSON_SAVED_PLASTIC_BOTTLES = 0,
    WIFI_JSON_CARBON_SAVINGS,
    WIFI_JSON_MATURE_CARBON_TREES,

    WIFI_JSON_ENVIRONMENT_MAX
};

typedef enum{
    WIFI_JSON_T1_YEAR = 0,
    WIFI_JSON_T1_MONTH,
    WIFI_JSON_T1_DAY,
    WIFI_JSON_T1_HOUR,
    WIFI_JSON_T1_MIN,

    WIFI_JSON_DATE_YMDHM_MAX
}E_WIFI_JSON_DATE_YMDHM_T;

typedef enum{
    WIFI_JSON_T2_YEAR = 0,
    WIFI_JSON_T2_MONTH,
    WIFI_JSON_T2_DAY,

    WIFI_JSON_DATE_YMD_MAX
}E_WIFI_JSON_DATE_YMD_T;

// JSON Value Def
typedef struct{
	U8 u8JsonDateLD[WIFI_JSON_DATE_YMDHM_MAX]; // YY MM DD HH MM
	U8 u8JsonDateND[WIFI_JSON_DATE_YMDHM_MAX]; // YY MM DD HH MM
} WIFI_JSON_DATE_YMDHM_T;
typedef struct{
	U8 u8JsonDateLD[WIFI_JSON_DATE_YMD_MAX]; // YY MM DD HH MM
	U8 u8JsonDateND[WIFI_JSON_DATE_YMD_MAX]; // YY MM DD HH MM
} WIFI_JSON_DATE_YMD_T;

typedef struct{
	WIFI_JSON_DATE_YMD_T 	Date_YMD;

	I16 i16JsonSEQ;
	I16 i16JsonMembership;
	I16 i16JsonRemain;
} WIFI_JSON_SUPPLY_T;

typedef struct{
	I16 i16JsonSavedPlasticBottles;
	I16 i16JsonCarbonSavings;
	I16 i16JsonMatureCarbonTrees;
} WIFI_JSON_ENVIRONMENT_T;
typedef struct
{
    WIFI_JSON_DATE_YMDHM_T  UV_Faucet;
	WIFI_JSON_SUPPLY_T		Supply[WIFI_JSON_SUPPLY_KEY_MAX];
	WIFI_JSON_ENVIRONMENT_T Environment;

} WIFI_JSON_VALUE_T;

typedef struct
{
    E_WIFI_JSON_KEY_T		Id;
	I16						*i16data;

} WIFI_JSON_TABLE_T;




#endif

#endif


