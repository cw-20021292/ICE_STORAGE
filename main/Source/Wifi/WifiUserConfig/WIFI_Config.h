
#ifndef _WIFI_CONFIG_MAX_H_
#define _WIFI_CONFIG_MAX_H_

/** SDK Version V1.00 **/

/***** headers ****************************************************************/
#include "WIFI_Common.h"
/******************************************************************************/

/***** User Settings **********************************************************/

/* User TEST Mode */
#define TEST_MODE_WIFI_OTA   0 // 0:Normal / 1: OTA Test Mode       (WiFi OFF/RESET X )
#define TEST_MODE_BLE_FCC    0 // 0:Normal / 1: BLE FCC Test Mode   (BLE OFF X)


/********** Version / Model Name / POP Code **********/
#define USER_WIFI_VERSION   "V0.0.0.2"	// MCU Version
#define MODEL_NAME_0        "CHP-7212N" // Model name
#define MODEL_NAME_1        "CP-7212N"  // Model name - Optional

#define PRODUCT_CODE_0      "113838"    // Main Product Code Number (POP)
#define PRODUCT_CODE_1      "113418"    // Main Product Code Number (POP) - Optional

/* User Request ID Setting */
#define WIFI_REQUEST_ID  "25MCU2SVR"

/* USER WIFI H/W Port & Uart Register */
/* ------------------------------------
Uart Baudrate			£º115200bps				
Data bit Len		    £º8bit				
Start bit				£º1bit				
Stop bit				£º1bit				
Parity Check			£ºNone
R_TXD                   : TXD1
R_RXD                   : RXD1
R_STIF                  : STIF1			
---------------------------------------*/
#define P_WIFI_ON       P14_bit.no5
#define P_WIFI_RSTN     P0_bit.no0

/* USER FOTA Define  (FALSE(0) :Unused / TRUE(1) :Used Fota) */
#define USER_FOTA_ENABLE                        1  
#define USER_FOTA_SIZE                          256 //(K)  MCU Rom Size   (Option : 128(K) / 192(K) / 256(K))

/* USER INFO Define (FALSE(0) : unused / TRUE(1): used ) */ 
#define USER_USE_A2010_INFO                     1   // Infomation A2010      
#define USER_USE_A2010_INFO_WEATHER             1   // Infomation A2010 (WEATHER)              
#define USER_USE_A2021_SAPINFO                  0   // Json Info (A2021)                 

/* USER DATA SIZE */ 
#define USER_DATA_MAX_SIZE                      45  // The number of A1010 data (Total RX/TX Buffer Size)
#define USER_ERROR_DATA_MAX_SIZE                20  // The number of A1030 data 
#define USER_DATA_MULTI_CONTROL_MAX_SIZE        10  // The number of A1011 data (Data received at once - Group Control)
#define USER_INFO_MAX_SZIE                      15  // The number of A2010 info
#define USER_DIAGNOSIS_MAX_SIZE                 10  // The number of A1091 Diagnosis (When using json data(A1091))

/* Define structures for parsed JSON data - When using json data(A2021)*/  
#define USER_JSON_MAX_ITEMS                     5   // Maximum number of key-value pairs  :  { Key1 : Val1, Key2 : Val2, ... Key5 : Val5 }
#define USER_JSON_MAX_ARRAY_VALUE               1   // Maximum number of Arrays           :  [ 1 ], [ 2 ]
#define USER_JSON_MAX_ARRAY_OBJECT              2   // Number of objects in the array     :  [ {1}, {2} ]



#endif


