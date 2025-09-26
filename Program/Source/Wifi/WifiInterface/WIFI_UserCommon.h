
#ifndef _WIFI_COMMON_MAX_H_
#define _WIFI_COMMON_MAX_H_

#include "r_cg_macrodriver.h"
//#include "r_cg_serial.h"
#include "WIFI_DefUser.h"
#include "WIFI_Config.h"

/***** User headers ****************************************************************/


/***** Type Define *****************************************************************/
typedef unsigned char       U8;
typedef signed char         I8;
typedef unsigned int        U16;
typedef signed int          I16;
typedef const signed char   CI8;
typedef const signed int    CI16;
typedef unsigned long int   U32;
typedef float               F32;
typedef double              D64;

/***** library headers ********************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <math.h>

/********** Model **********/


#define WIFI_VERSION    USER_WIFI_VERSION	
#define MODEL_NAME_CHP  MODEL_NAME_0
#define MODEL_NAME_CP   MODEL_NAME_1


// USER WIFI Port & Register
#define R_TXD           TXD1
#define R_RXD           RXD1
#define R_STIF          STIF1
// #define P_WIFI_ON       P14.5
// #define P_WIFI_RSTN     P0.0

// USER Define (Number of User Data)

#define USER_DATA_MAX_NUM   USER_DATA_MAX_SIZE  // The number of data (Total)

#define USER_DATA_MAX_NUM_1BYTE   10  // The number of data (DATA Size)
#define USER_DATA_MAX_NUM_3BYTE   10  // The number of data (DATA Size)
#define USER_DATA_MAX_NUM_5BYTE   10  // The number of data (DATA Size)

#define USER_INFO_MAX_NUM   USER_INFO_MAX_SZIE  // The number of info

#define USER_DIAGNOSIS_MAX_NUM   USER_DIAGNOSIS_MAX_SIZE  // The number of Diagnosis
//#define USER_USE_ACCUMULATE     // A10A0 used / unused

#if (USER_USE_A2010_INFO_WEATHER == 1)
#define USER_USE_WEATHER        // A2010 Weather used / unused
#endif

#define USER_USE_ACK            // WiFi Ack used / unused


// Define structures for parsed JSON data
#define USER_DATA_JSON_ONE_MAX      200                             // API ID당 JSON 총 최대 길이
// #define USER_DATA_JSON_UART_MAX     (LENGTH_WIFI_ALL_MAX_N)         // JSON UART 최대 길이
// #define USER_DATA_JSON_MAX          400                             // JSON 총 최대 길이
#define USER_MAX_KEY_LEN            5                               // Key 최대 길이
#define USER_MAX_VALUE_LEN          80                              // Value 최대 길이
#define USER_MAX_VALUE_ARRAY_LEN    150                             // Array Value 최대 길이

#define USER_MAX_ITEMS              USER_JSON_MAX_ITEMS              // 최대 키-값 쌍의 수
#define USER_MAX_ARRAY_VALUE        USER_JSON_MAX_ARRAY_VALUE        // 최대 Array 갯수     
#define USER_MAX_ARRAY_OBJECT       USER_JSON_MAX_ARRAY_OBJECT       // Array의 object 수

// USER Define (RomSize/ProductNum)

#if (USER_FOTA_SIZE == 128)
#define FOTA_SIZE       "513"       // FotaSize = Rom Size * 4 + 1
#endif

#if (USER_FOTA_SIZE == 192)
#define FOTA_SIZE       "769"       // FotaSize = Rom Size * 4 + 1
#endif

#if (USER_FOTA_SIZE == 256)
#define FOTA_SIZE       "1025"       // FotaSize = Rom Size * 4 + 1
#endif

// #define FOTA_SIZE       "513"       // FotaSize = Rom Size * 4 + 1
#define FOTA_TYPE_CHP   PRODUCT_CODE_0    // Product Code Number
#define FOTA_TYPE_CP    PRODUCT_CODE_1    // Product Code Number





#define Z_WIFI_rammax		(0xFFFF)        
#define Z_WIFI_rammin		(0x0000)			

// USER Define (EEPROM Address)
enum
{
    EEPROM_ADDR_WIFI_POWER,                      // WIFI On/Off State
    EEPROM_ADDR_FIRST_PARING,                    // WIFI First Paring Check
    EEPROM_ADDR_FOTA_MODULE,                     // WIFI Module Fota State
    EEPROM_ADDR_FOTA_MCU,                        // WIFI MCU Fota State
};

// USER Define (Buzzer)
enum{
    WIFI_BUZZER_SELECT,          // 페어링 (선택음 "띵") 
    WIFI_BUZZER_AP_CONNECT,      // AP연결 (진입음 "띠리링" / Wi-Fi 연결모드가 활성화 되었습니다.)
    WIFI_BUZZER_BLE_CONNECT,     // AP연결 (진입음 "띠리링" / BLE연결모드가 활성화 되었습니다.)
    WIFI_BUZZER_SETUP,           // WIFI ON (설정음 "띵동↗" / 제품의 Wi-Fi가 켜졌습니다.)
    WIFI_BUZZER_SERVER_CONNECT,  // 서버연결 (연결음 "띠리링↗" / 제품과 앱이 성공적으로 연결되었습니다.)
    WIFI_BUZZER_AP_FAIL,         // 페어링30분 경과 (해제음 "동띵" / 제품과 앱의 연결이 실패하였습니다.)
    WIFI_BUZZER_CANCEL,          // WIFI OFF (해제음 "동띵" / 제품의 Wi-Fi가 꺼졌습니다.)
    WIFI_BUZZER_AP_START,        // AP시작음 (설정음 "띵동 ↗")
    WIFI_BUZZER_ERROR,           // 불가음 (에러음 "띵깡띵깡")
    WIFI_BUZZER_WiFi_OFF,         // Wi-Fi 페어링 해제음(동띵) / Wi-Fi 연결 모드가 비활성화 되었습니다.
    WIFI_BUZZER_BLE_OFF,	      // 블루투스 페어링 해제음(동띵) / 블루투스 연결 모드가 비활성화 되었습니다.
    WIFI_BUZZER_BLE_CERT,         // (선택음 "띵") / 블루투스 점유인증 활성화 상태 / 제품인증을 진행 중입니다. 앱에서 가이드하는 버튼을 눌러주세요.
    WIFI_BUZZER_SERVER_FIRST,     // 최초 서버연결 (연결음 "띠리링↗" / 제품과 앱이 성공적으로 연결되었습니다.

    WIFI_BUZZER_SMART_CHECK_START,       // (설정음 "띵동 ↗") / 스마트 진단을 시작합니다. 진행 중에는 제품을 사용할 수 없으니 잠시만 기다려 주세요.
    WIFI_BUZZER_SMART_CHECK_OK,          // (설정음 "띵동 ↗") / 스마트 진단이 완료 되어 제품을 사용하실 수 있습니다.
    WIFI_BUZZER_SMART_CHECK_ERROR,       // 불가음 (에러음 "띵깡띵깡") / 스마트 진단이 완료 되었습니다.
};




#endif

