
#ifndef _WIFI_COMMON_H_
#define _WIFI_COMMON_H_


#include "WIFI_UserCommon.h"
#include "WIFI_SetCommon.h"
#include "WIFI_UserInterface_Data.h"
#include "WIFI_Control.h"

#include "WIFI_A103x_ErrorList.h"
#include "WIFI_A101x_FunctionList.h"
#include "WIFI_A102x_SensorList.h"
#include "WIFI_A108x_A109x_PartList.h"
#include "WIFI_A20xx_InformationList.h"
#include "WIFI_A10Ax_AccumulateList.h"
#include "WIFI_A105x_ParaList.h"


/***** Setting Start *****************************************************/
/* USER Test Config */
#define TEST_B1_OTA     TEST_MODE_WIFI_OTA // 0:Normal / 1:OTAtest
#define TEST_WIFI_PJT   0 // 0:Normal / 1:Technology Project
#define TEST_MODEM_SSID "coway_iot_01"  //"IQ0F1CA0264"
#define TEST_MODEM_PW   "12345678"

#define UNUSED_BLE_A1011_RX       // BLE DATA(A1011) 수신처리 미사용
#define UNUSED_A9011_RX           // BLE / Wi-Fi DATA (A9011) 수신처리 미사용 

#define TEST_BLE_FCC    TEST_MODE_BLE_FCC // 0:Normal / 1:test Mode (BLE OFF X)
#define BLE_ALWAYS      // (BLE AUTO OFF X)

/***** Define *****************************************************************/
#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef RESET
#define RESET 2
#endif

#ifndef WAIT
#define WAIT 3
#endif

#define DONE 2

#ifndef N_TRUE
#define N_TRUE 0
#endif

#ifndef N_FALSE
#define N_FALSE 1
#endif

#ifndef SET
#define SET 1
#endif

#ifndef CLEAR
#define CLEAR 0
#endif

#ifndef ON
#define ON 1
#endif

#ifndef OFF
#define OFF 0
#endif

#ifndef IoT_GOING
#define IoT_GOING 2
#endif

#ifndef IoT_DONE
#define IoT_DONE 3
#endif

#define WiFi_M_RAM_Read 0
#define WiFi_M_RAM_Write 1

#define Write 1

#define  BLEDATA     1
#define  WIFIDATA    0

#define Read_Only 0
#define CLEAR_ADR 0

#define OneByte 1
#define TwoByte 2

#define Model_0 0
#define Model_1 1

#define ZT_BleCertCancelTime 70  // 100ms

#define W_COUNT_WIFI_POWER    100 // 100ms*100

#define CONCAT(a, b) a##b

#define TURN_ON_WIFI_POW()  do{ P_WIFI_ON = ON; }while(0)
#define TURN_OFF_WIFI_POW() do{ P_WIFI_ON = OFF; }while(0)
#define TURN_ON_WIFI_RST()  do{ P_WIFI_RSTN = ON; }while(0)
#define TURN_OFF_WIFI_RST() do{ P_WIFI_RSTN = OFF; }while(0)

/* Data Length */
#define LENGTH_MODEL_NAME       13
#define LENGTH_MODEL_TYPE       6
#define LENGTH_API_NO           5
#define LENGTH_AT_CMD_MAX       50
#define LENGTH_REQUEST_ID       16  // Request ID Byte
#define LENGTH_WIFI_ID_ONE      5	// OneID+: Max Byte
#define LENGTH_WIFI_DATA_ONE    7	// OneData Max Byte

#define LENGTH_WIFI_DATA_ONE_MAX    12	
#define LENGTH_WIFI_DATA_ID     13	// ID+OneData Total Max Byte
#define LENGTH_WIFI_DATA_ID_AVG 8	// ID+OneData Total Max Byte
#define LENGTH_WIFI_DATA_ID_1B  6   // ID+OneData Total Max Byte (1Byte)
#define LENGTH_WIFI_DATA_ID_3B  8  // ID+OneData Total Max Byte (3Byte)
#define LENGTH_WIFI_DATA_RX_MAX (USER_DATA_MULTI_CONTROL_MAX_SIZE * LENGTH_WIFI_DATA_ID) // Multi Control MAX RX DATA
#define LENGTH_WIFI_DATA_DR_MAX (USER_DIAGNOSIS_MAX_NUM*LENGTH_WIFI_DATA_ID)	// ID Num * One ID Max 13byte  // Diagnosis Result 
#define LENGTH_WIFI_INFO_MAX    (USER_INFO_MAX_NUM*LENGTH_WIFI_DATA_ID)	// ID Num * One ID Max 13byte

#define LENGTH_WIFI_DATA_MAX    (USER_DATA_MAX_NUM*LENGTH_WIFI_DATA_ID)	// ID Num * One ID Max 13byte
#define LENGTH_WIFI_DATA_MAX_N  ((USER_DATA_MAX_NUM_1BYTE*LENGTH_WIFI_DATA_ID_1B)+(USER_DATA_MAX_NUM_3BYTE*LENGTH_WIFI_DATA_ID_3B)+(USER_DATA_MAX_NUM_5BYTE*LENGTH_WIFI_DATA_ID))	// ID Num * One ID Max 13byte
#define LENGTH_WIFI_ALL_MAX     (LENGTH_WIFI_DATA_MAX+100)	// All Data Max Byte (include of At Cmd, Len, ...)
// #define LENGTH_WIFI_ALL_MAX_N   (LENGTH_WIFI_DATA_MAX_N+100)	// All Data Max Byte (include of At Cmd, Len, ...)
// #define LENGTH_WIFI_ALL_MAX_N   (LENGTH_WIFI_DATA_MAX+100)	// All Data Max Byte (include of At Cmd, Len, ...)  
#define LENGTH_WIFI_ALL_MAX_N   ((USER_DATA_MAX_NUM*LENGTH_WIFI_DATA_ID_AVG)+120)	// All Data Max Byte (include of At Cmd, Len, ...)  

#define LENGTH_WIFI_RAM_DATA    15  // RAM DATA 
#define LENGTH_WIFI_RAM_WRITE   20  // A9011 MAX DATA 
#define LENGTH_WIFI_A90xx_PASS  6   // [Z1011.]  PASS
#define LENGTH_RAM_ADDRESS_MAX  30  // RAM ADDRESS MAX COUNT
#define LENGTH_RAM_DATA_MAX     (LENGTH_RAM_ADDRESS_MAX*LENGTH_WIFI_DATA_ID)

#define LENGTH_WIFI_JSON_ONE    (USER_DATA_JSON_ONE_MAX)	// OneData Max Byte (Json)
#define LENGTH_WIFI_JSON_MAX    (USER_DATA_JSON_MAX)	 // Json DATA
// #define LENGTH_WIFI_ALL_RX_MAX  (USER_DATA_JSON_UART_MAX)	// All Data Max Byte (Json)
#define LENGTH_WIFI_ALL_RX_MAX  (LENGTH_WIFI_ALL_MAX_N)	// All Data Max Byte (Json)

/* ASCII */
#define ASCII_STX       0x02
#define ASCII_ETX       0x03
#define ASCII_STAR      '*'
#define ASCII_QUOTE     '"'     // 0x22 // "
#define ASCII_COLON     ':'     // 0x3A // :
#define ASCII_COMMA     ','     // 0x2C // ,
#define ASCII_OPEN      '{'     // 0x7B	// {
#define ASCII_CLOSE     '}'     // 0x7D	// }
#define ASCII_DOT       '.'     // 0x2E	// .
#define ASCII_MINUS     '-'     // 0x2D // -
#define ASCII_NULL      '\0'    // 0x00 // null
#define ASCII_DONE      0x0D    // <0x0D> Data Done
#define ASCII_ATDONE    0x0A    // <0x0A> AT CMD Done
#define ASCII_SPACE     ' '     // 0x20
#define ASCII_UNDERBAR  '_'     //
#define ASCII_NO_0      '0'     // 0x31
#define ASCII_NO_1      '1'     // 0x31
#define ASCII_NO_2      '2'     // 0x31
#define ASCII_Z         'Z'    // 0x5a

/* Etc */
#define COWAY_SSID      "Coway_"
#define WIFI_NULL       0xFF

extern U16 gu16_RAM_Monitor_RESP_SIZE;
extern U16 gu16_START_RAM_Address[LENGTH_RAM_ADDRESS_MAX];
extern U16 gu16_RAM_Address_len[LENGTH_RAM_ADDRESS_MAX];

enum
{
    //CLEAR = 0x00,
    //SET,
    // RESET = 0x02,
    // WAIT,
    START = 0x04,
    RETRY

};


enum
{
    UNUSED = 0x00,
    USED,
    REUSED
};

typedef enum
{
    USER_MODEL_MODEL0 = 0x00,
    USER_MODEL_MODEL1,

} E_USER_MODEL_T;


typedef enum
{
    USER_MODEL_TYPE = 0x00,
    USER_FACTORY_STATUS,
    USER_RX_STATUS,
    USER_SMART_POSSIBLE_STATUS,
    USER_ERROR_STATUS,
    USER_FOTA_POSSIBLE_STATUS,
    USER_SMART_SENSING_STATUS,
    USER_FACTORY_DIRECTFOTA_STATUS,

    USER_SATATUS_MAX
} E_USER_VALUE_T;

typedef enum
{
    USER_DISPLAY_FOTA = 0x00,
    USER_DISPLAY_SELFCHECK,
    USER_DISPLAY_PARTCHECK
} E_USER_DISPLAY_T;

typedef enum
{
    TO_SERVER = 0x00,
    TO_MODULE,
    TO_APMODE,
    TO_VERSION,
    TO_SCONN,
    TO_BLEADV
} E_TARGET_EXCEPT_T;


typedef void (*WifiTxFun_T)(U16 mId, I8 *pBuf);
typedef void (*WifiRxFun_T)(U16 mId, I8 *pBuf);
typedef struct _wifi_tx_list_
{
    U16 Id;
    WifiTxFun_T TxFunc;
    WifiRxFun_T RxFunc;
} WifiTxFuncList_T;
typedef void (*WifiExFun_T)( U8 mExcept, I8* pBuf, U16 mLen );

typedef void (*WifiTxFun_Monitor_T)(U16 mId, I8 *pBuf, U16 mLen);
typedef struct _wifi_Monitor_tx_list_
{
    U16 Id;
    WifiTxFun_Monitor_T TxFunc_M;
    WifiRxFun_T RxFunc_M;
} WifiTxFuncList_Monitor_T;

/* Function / Error Event */
typedef struct _wifi_event_list_
{
    U16 Id;
    U8  mu8Type;
    U16 *mu16PreVal;
    U8  mu8EventForm;
} WifiTxFuncEventList_T;

/* Sensor / Part List */
typedef struct _wifi_normal_list_
{
    U16 Id;
    U8 u8DataType;

} WifiNormalList_T;

typedef enum
{
    EVENT_UNUSED = 0x00,
    EVENT_STATUS_CHANGE,        // 상태변경 시
    EVENT_ZERO_TO_ONE,           // 0->1로 변경 시 또는 ?->0으로 변경 시
    EVENT_ZERO_TO_OTHER,         // 0 -> ? 로 변경 시  (추출물량, 온수출수 온도 등)
    EVENT_SPECIFIC_CONDITION,    // 특정 조건 일 경우 (ex: 1,3,5,7 등 또는 30이하 부터 변경 시)
    
    EVENT_FORM_MAX
} E_EVENT_FORM_T;

typedef enum
{
    TYPE_UNUSED = 0x00,
    TYPE_A1014,  
    TYPE_SMART_DIAGNOSIS_CONTROL,     // 스마트진단 시작/정지 프로토콜        
    EVENT_TYPE_MAX
} E_EVENT_TYPE_FORM_T;

typedef enum
{
    TYPE_UNSIGNED = 0x00,
    TYPE_SIGNED,  

    TYPE_SENSOR_WATT,      // 00.00 Point Data

    TYPE_INFO_NORMAL,
    TYPE_INFO_TIME,
    TYPE_INFO_WEATHER,
    TYPE_INFO_AQI,
    TYPE_INFO_TEMP,
    TYPE_INFO_DIAGNOSIS_RQ,
    TYPE_INFO_JSON,

    TYPE_MAX
} E_TYPE_FORM_T;

#if ( USER_FOTA_ENABLE == TRUE )
#ifndef __far
#define __far       __far
#endif

#ifndef _MEMSET_
#define _MEMSET_ _COM_memset_f  // _COM_memset_f(const void __far *s, int c, size_t n);
#endif

#ifndef _MEMCPY_
#define _MEMCPY_ _COM_memcpy_ff  // memcpy_f( void __far *a, const void __far *b, size_t n );
#endif

#ifndef _MEMMOVE_
#define _MEMMOVE_ _COM_memmove_ff  // memmove_f( void __far *a, const void __far *b, size_t n );
#endif

#ifndef _MEMCMP_
#define _MEMCMP_ _COM_memcmp_ff  // memcmp_f( const void __far *a, const void __far *b, size_t n );
#endif

#ifndef _STRCPY_
#define _STRCPY_ _COM_strcpy_ff  // strcpy_f( char __far *a, const char __far *b );
#endif

#ifndef _STRNCPY_
#define _STRNCPY_ _COM_strncmp_ff  // strncpy_f( char __far *a, const char __far *b, size_t n );
#endif

#ifndef _STRCMP_
#define _STRCMP_ _COM_strcmp_ff  // strcmp_f( const char __far *a, const char __far *b );
#endif

#ifndef _STRNCMP_
#define _STRNCMP_ _COM_strncmp_ff  // strncmp_f( const char __far *a, const char __far *b, size_t n );
#endif

#ifndef _STRLEN_
#define _STRLEN_ _COM_strlen_f  // strlen_f( const char __far *a );
#endif

#ifndef _SPRINTF_
#define _SPRINTF_ sprintf  // sprintf_f( char __far *a, const char __far *format, char __far *b );
#endif

#ifndef _STRSTR_
#define _STRSTR_ _COM_strstr_ff  // char __far * __far _COM_strstr_ff(const char __far *s1, const char __far *s2);
#endif

#ifndef _ATOI_
#define _ATOI_ _COM_atoi_f  // int __far _COM_atoi_f(const char __far* nptr);
#endif
/*
#ifndef _MEMSET_
#define _MEMSET_    memset_f    //memset_f( void __far *a, int b, size_t n );
#endif

#ifndef _MEMCPY_
#define _MEMCPY_    memcpy_f    //memcpy_f( void __far *a, const void __far *b, size_t n );
#endif

#ifndef _MEMMOVE_
#define _MEMMOVE_   memmove_f   //memmove_f( void __far *a, const void __far *b, size_t n );
#endif

#ifndef _MEMCMP_
#define _MEMCMP_    memcmp_f    //memcmp_f( const void __far *a, const void __far *b, size_t n );
#endif

#ifndef _STRCPY_
#define _STRCPY_    strcpy_f    //strcpy_f( char __far *a, const char __far *b );
#endif

#ifndef _STRNCPY_
#define _STRNCPY_   strncpy_f   //strncpy_f( char __far *a, const char __far *b, size_t n );
#endif

#ifndef _STRCMP_
#define _STRCMP_    strcmp_f    //strcmp_f( const char __far *a, const char __far *b );
#endif

#ifndef _STRNCMP_
#define _STRNCMP_   strncmp_f   //strncmp_f( const char __far *a, const char __far *b, size_t n );
#endif

#ifndef _STRLEN_
#define _STRLEN_    strlen_f    //strlen_f( const char __far *a );
#endif

#ifndef _SPRINTF_
#define _SPRINTF_   sprintf_f   //sprintf_f( char __far *a, const char __far *format, char __far *b );
#endif

#ifndef _ATOI_
#define _ATOI_      atoi_f      //atoi_f( const char __far *a );
#endif
*/
#else
#define __far       
#define _MEMSET_    memset      //memset( void *a, int b, size_t n );
#define _MEMCPY_    memcpy      //memcpy( void *a, const void *b, size_t n );
#define _MEMMOVE_   memmove     //memmove( void *a, const void *b, size_t n );
#define _MEMCMP_    memcmp      //memcmp( const void *a, const void *b, size_t n );
#define _STRCPY_    strcpy      //strcpy( char *a, const char *b );
#define _STRNCPY_   strncpy     //strncpy( char *a, const char *b, size_t n );
#define _STRCMP_    strcmp      //strcmp( const char *a, const char *b );
#define _STRNCMP_   strncmp     //strncmp( const char *a, const char *b, size_t n );
#define _STRLEN_    strlen      //strlen( const char *a );
#define _SPRINTF_   sprintf     //sprintf( char *a, const char *f, char *b );
#define _ATOI_      atoi        //atoi( const char *a );

#endif

/***** Function ***************************************************************/
U8 WIFI_HEX2DEC ( U8 mu8HexData );
U8 WIFI_DEC2HEX ( U8 mu8DecData );
U16 WIFI_Bit16HEX2DEC ( U16 mu16HexData );
U16 WIFI_DEC2Bit16HEX ( U16 mu16DecData );
U8 WIFI_HEX2ASCII ( U8 mu8HexData );
U8 WIFI_ASCII2HEX ( U8 mu8AsciiData );
U16 far_sprintf_s ( I8* pBuf, U16 mLen, const I8 __far* pData );

#endif

