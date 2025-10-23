
/*******************************************************************************
&Description :
    Analysis Server API
  
&History : ShinHM
    2021.04.22 update
    2021.12.10 update

&History : ssg@coway.com
    2023.05.24 update
*******************************************************************************/


/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_DefUser.h"
#include "WIFI_RxServer.h"
#include "WIFI_TxProtocol.h"
#include "WIFI_Initialize.h"
#include "WIFI_Status.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_SetInformationData.h"
#include "WIFI_SetPartData.h"
#include "WIFI_SetMonitorData.h"
#include "WIFI_UserInterface.h"
#include "WIFI_MonitorFixRAM.h"


/***** Define *****************************************************************/
/* API No (Server -> MCU) */
const signed char __far API_RX_A1002[] = "A1002"; // Disconnect
const signed char __far API_RX_A1011[] = "A1011"; // Function Control
const signed char __far API_RX_A1013[] = "A1013"; // Function Request
const signed char __far API_RX_A1023[] = "A1023"; // Sensor Request
const signed char __far API_RX_A1033[] = "A1033"; // Error Request
const signed char __far API_RX_A1035[] = "A1035"; // Error Selfcheck Request
const signed char __far API_RX_A1051[] = "A1051"; // Parameter Control
const signed char __far API_RX_A1053[] = "A1053"; // Parameter Request
const signed char __far API_RX_A1083[] = "A1083"; // SmartDetect Period Request
const signed char __far API_RX_A1091[] = "A1091"; // SmartDetect Result (Server -> Device)

const signed char __far API_RX_A2011[] = "A2011"; // Information Respond
const signed char __far API_RX_A2021[] = "A2021"; // [JSON] Information Respond
const signed char __far API_RX_A7011[] = "A7011"; // BLE Cert RX (Request)
const signed char __far API_RX_A9011[] = "A9011"; // Monitor CMD (Reqeust Write)
const signed char __far API_RX_A9013[] = "A9013"; // Monitor CMD (Reqeust Read)

/* Wifi Recv Cmd */
#define AT_CMD_RECV "*ICT*AWS_RECV:"
#define AT_CMD_BLE_RECV "*ICT*BLE_RECV:"
const signed char AT_CMD_MAC_RECV[] = "*ICT*MAC:OK ";

/* Length & Position */
#define LENGTH_HEADER 5 // STX + LENGTH
#define POSITION_STX 0 // 0 (1Byte)
#define POSITION_LENGTH 1 // 1~4 (4Byte)
#define POSITION_API 5 // 5~9 (5Byte)
#define POSITION_RQID 10 // 10~25 (16Byte)
#define POSITION_OPEN 26 // 26 (1Byte) '{'
#define POSITION_DATA 27 // 27~ (nByte)


/***** Variable ***************************************************************/
// I8 ai8RxProtocol[LENGTH_WIFI_ALL_MAX_N]; // RX DATA (STX ~ ETX)
I8 ai8RxProtocol[LENGTH_WIFI_ALL_RX_MAX]; // RX DATA (STX ~ ETX)
I8 ai8Mac[17U];

U16 au16InfoData[WIFI_INFO_NUM];
U16 gu16_RAM_Monitor_RESP_SIZE;         // RAM DATA 읽기/쓰기 요청한 갯수 (RAM address 갯수)
U16 gu16_START_RAM_Address[LENGTH_RAM_ADDRESS_MAX];     // RAM ADDRESS 값 (최대갯수 지정)
U16 gu16_RAM_Address_len[LENGTH_RAM_ADDRESS_MAX];
/***** Constant ***************************************************************/


/***** Static Function ********************************************************/
static void RunAnalysis ( void );
static U8 IsRxDataOk ( void );

static U8 RxA1011 ( void );
static U8 RxA1091 ( void );
static U8 RxA2011 ( void );
static U8 RxA2021 ( void );
static U8 RxA7011 ( void );
static U8 RxA9011 ( void );
static U8 RxA9013 ( void );

static void DoFuncData ( I8* pBuf );
static void DoDiagnosisResultData ( I8* pBuf );
static void DoInfoData ( I8* pBuf );
static void DoInfoJsonData ( I8* pBuf );
static void DoMonitorData (U8 rw_sel, I8* pBuf );


static void MakeCheckId ( I8* pBuf, U16 mLen, U16 mId );


/***** Table ******************************************************************/
typedef U8 (*WifiRxApiFun_T)( void );
typedef struct _wifi_rxapi_list_
{
    const CI8 __far* Api;
    U32 Tx;
    WifiRxApiFun_T Func;
} WifiRxApiList_T;

/* Wifi Rx Table */
static const WifiRxApiList_T WifiRxApiList[] =   // 마지막부터 추가.
{   /*  API             Tx                      Function */
    {   API_RX_A1002,   WIFI_TX_NULL,               NULL    },
    // {   API_RX_A1011,   WIFI_TX_RESP_RECV,          RxA1011 },
    {   API_RX_A1011,   WIFI_TX_NULL,               RxA1011 }, // 자체 Event Check
    {   API_RX_A1013,   WIFI_TX_RESP_FUNC,          NULL    },
    {   API_RX_A1091,   WIFI_TX_NULL,               RxA1091 }, // Diagnosis Result
    {   API_RX_A1023,   WIFI_TX_RESP_SENSOR,        NULL    },
    {   API_RX_A1033,   WIFI_TX_RESP_ERR,           NULL    },
    {   API_RX_A1035,   WIFI_TX_SELF_ERR,           NULL    },
    {   API_RX_A1051,   WIFI_TX_SEND_PARA,          NULL    },
    {   API_RX_A1053,   WIFI_TX_RESP_PARA,          NULL    },
    {   API_RX_A1083,   WIFI_TX_RESP_PART,          NULL    },
    {   API_RX_A2011,   WIFI_TX_NULL,               RxA2011 },
    {   API_RX_A2021,   WIFI_TX_NULL,               RxA2021 },
    {   API_RX_A7011,   WIFI_TX_NULL,               RxA7011 },
    {   API_RX_A9011,   WIFI_TX_RESP_MNT_DATA_W,    RxA9011 },
    {   API_RX_A9013,   WIFI_TX_RESP_MNT_DATA_R,    RxA9013 }
};
#define SZ_RX_API_LIST  ( sizeof(WifiRxApiList) / sizeof(WifiRxApiList_T) )


/***** Global Function ********************************************************/
void InitializeWifiRxServer ( void )
{
    _MEMSET_( (void __far*) ai8RxProtocol, '\0', sizeof(ai8RxProtocol) );
    _MEMSET_( (void __far*) au16InfoData, 0, sizeof(au16InfoData) );
}

void WifiInsertRxAPI ( I8* mai8Recv )
{
    U16 mu16Len = 0U;
    U16 mu16Position = 0U;
    U16 i = 0U;

    _MEMSET_( (void __far*) ai8RxProtocol, '\0', sizeof(ai8RxProtocol) );

    /* Check Max Length */
    mu16Len = _STRLEN_( (const char __far*) mai8Recv );
    mu16Position = _STRLEN_( (const char __far*) AT_CMD_RECV );
    if( mu16Len > LENGTH_WIFI_ALL_RX_MAX )
    // if( mu16Len > LENGTH_WIFI_ALL_MAX )
    {
        return;
    }
  	
    /* Insert Data */
    for ( i = 0 ; i < mu16Len ; i++ )
    {   /* STX ~ ETX */
        ai8RxProtocol[i] = mai8Recv[mu16Position + i];
        if ( ai8RxProtocol[i] == ASCII_ETX )
        {
            break;
        }
    }
    
    if ( GetUserInterface( USER_SMART_SENSING_STATUS ) != TRUE ) 
    {
        SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA);
    }
    /* Analysis */
    RunAnalysis();
}
/*Rx BLE DATA */
void WifiInsertRxBLEAPI ( I8* mai8Recv )
{
    U16 mu16Len = 0U;
    U16 mu16Position = 0U;
    U16 i = 0U;

    _MEMSET_( (void __far*) ai8RxProtocol, '\0', sizeof(ai8RxProtocol) );

    /* Check Max Length */
    mu16Len = _STRLEN_( (const char __far*) mai8Recv );
    mu16Position = _STRLEN_( (const char __far*) AT_CMD_BLE_RECV );
    if( mu16Len > LENGTH_WIFI_ALL_RX_MAX )
    // if( mu16Len > LENGTH_WIFI_ALL_MAX_N )
    // if( mu16Len > LENGTH_WIFI_ALL_MAX )
    {
        return;
    }
  	
    /* Insert Data */
    for ( i = 0 ; i < mu16Len ; i++ )
    {   /* STX ~ ETX */
        ai8RxProtocol[i] = mai8Recv[mu16Position + i];
        if ( ai8RxProtocol[i] == ASCII_ETX )
        {
            break;
        }
    }

    /* Analysis */
    SetWifiBLESendStatus(TX_BLE_WIFI, BLEDATA);
    SetWifiBLESendStatus(BLE_RECV_CHECK, SET);
    RunAnalysis();
}

void WifiInsertRxMac( I8* mai8Recv )
{
    U16 mu16Len = 0U;
    U16 mu16Position = 0U;
    U16 i = 0U;

    _MEMSET_( (void __far*) ai8Mac, '\0', sizeof(ai8Mac) );

    /* Check Max Length */
    mu16Len = _STRLEN_( (const char __far*) mai8Recv );
    mu16Position = _STRLEN_( (const char __far*) AT_CMD_MAC_RECV );
    if( mu16Len > LENGTH_WIFI_ALL_RX_MAX )
    // if( mu16Len > LENGTH_WIFI_ALL_MAX )
    {
        return;
    }
  	
    /* Insert Data */
    for ( i = 0 ; i < 17U ; i++ )
    {   /* STX ~ ETX */
        ai8Mac[i] = mai8Recv[mu16Position + i];
    }
}


U16 GetWifiInfoData ( U16 mId )
{
    return au16InfoData[mId];
}

I8 GetWifiMac ( U8 mId )
{
    return ai8Mac[mId];
}

void SetWifiInfoData ( U16 mId, U16 mData )
{
#if (USER_USE_A2010_INFO == 1)
    au16InfoData[mId] = mData;
    if ( mId == WIFI_INFO_0006_DAY )
    {
        SetUserTimeSetting( &au16InfoData[0] );
    }
    if ( ( mId == WIFI_INFO_0007_PART_PERIOD ) && ( GetWifiIniStatus( STATUS_INI_INFO ) != SET ) )
    {
        SetWifiIniStatus( STATUS_INI_INFO, SET );
    }
#else
    SetWifiIniStatus( STATUS_INI_INFO, SET );
#endif
}


/***** Local Function *********************************************************/

static void RunAnalysis ( void )
{
    WifiRxApiList_T *pList = NULL;
    U8 i = 0U;
    static I8 mai8RxAPINo[LENGTH_API_NO+1];
    static I8 mai8RqId[LENGTH_REQUEST_ID+1];
    
    _MEMSET_( (void __far*) mai8RxAPINo, '\0', sizeof(mai8RxAPINo));
    _MEMSET_( (void __far*) mai8RqId, '\0', sizeof(mai8RqId));
    
    /* Validity Check */
    if( IsRxDataOk() != TRUE )
    {
        return;
    }
    
    /* API & RequestID */
    for ( i = 0; i < LENGTH_API_NO ; i++ )
    {
        mai8RxAPINo[i] = ai8RxProtocol[POSITION_API + i];
    }
    for ( i = 0; i < LENGTH_REQUEST_ID ; i++ )
    {
        mai8RqId[i] = ai8RxProtocol[POSITION_RQID + i];
    }
    SetRequestId( mai8RqId );
    
    /* Run */
    pList = WifiRxApiList;

#ifdef UNUSED_BLE_A1011_RX
    if (GetWifiBLESendStatus(TX_BLE_WIFI) == BLEDATA ) // BLE DATA 수신 인 경우
    {
        if ( _STRNCMP_( (const char __far*) API_RX_A1011, (const char __far*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE ) // BLE : A1011 인 경우
        {  
            return;
        }
    }
#endif

#ifdef UNUSED_A9011_RX
    if ( _STRNCMP_( (const char __far*) API_RX_A9011, (const char __far*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE ) // BLE : A9011 인 경우
    {  
        return;
    }
#endif
    if (GetWifiBLESendStatus(TX_BLE_WIFI) == BLEDATA ) 
    {

        if ( _STRNCMP_( (const char __far*) API_RX_A7011, (const char __far*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE ) 
        {  
            SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA); 
        }
    }
    if ( GetUserInterface( USER_SMART_SENSING_STATUS ) == TRUE )
    {
        if ( _STRNCMP_( (const char __far*) API_RX_A1083, (const char __far*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE ) // 스마트진단 중 상시진단 요청 시 예외처리
        {  
            return;
        }
    }
    
    for ( i = 0; i < SZ_RX_API_LIST; i++ )
    {
        if ( _STRNCMP_( (const char __far*) (pList+i)->Api, (const char __far*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE )
        {  
            WifiRxApiFun_T pFun;
            U8 mAck = TRUE;

            pFun = (pList+i)->Func;
            
            if ( pFun != NULL )
            {
                mAck = pFun();
            }
            if ( mAck == TRUE ) 
            {
                if (( (pList+i)->Tx != WIFI_TX_NULL ) )
                {
                    SetWifiSendStatus( TX_DATA, (pList+i)->Tx );
                }
                if ( _STRNCMP_( (const char __far*) API_RX_A1011, (const char __far*) mai8RxAPINo, LENGTH_API_NO ) == N_TRUE )
                { 
                    SetWifiSendStatusAck(TRUE);
                }
            }
            break;
        }
    }
}

static U8 IsRxDataOk ( void )
{
    U16 mu16i = 0U;
    U16 mu16DataLength = 0U;
    U16 mu16DataLengthBuf = 0U;
    U32 mu32CheckSumBuf = 0UL;
    U8 mu8CheckSum = 0U;
    U8 mu8RxCheckSum = 0U;

    /* Check STX */
    if ( ai8RxProtocol[POSITION_STX] != ASCII_STX )
    {
        return FALSE;
    }
    
    /* Check ETX */
    if ( ai8RxProtocol[ _STRLEN_( (const char __far*) ai8RxProtocol ) - 1 ] != ASCII_ETX )
    {
        return FALSE;
    }
    /* Check Length */
    mu16DataLengthBuf = (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_LENGTH]));
    mu16DataLength = (mu16DataLengthBuf << 12) & 0xF000;
    mu16DataLengthBuf = (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_LENGTH+1]));
    mu16DataLength += (mu16DataLengthBuf << 8) & 0x0F00;
    mu16DataLength += (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_LENGTH+2]) << 4) & 0x00F0;
    mu16DataLength += WIFI_ASCII2HEX(ai8RxProtocol[POSITION_LENGTH+3]) & 0x000F;
    
    if ( ( mu16DataLength + LENGTH_HEADER ) != _STRLEN_( (const char __far*) ai8RxProtocol ) )
    {
        return FALSE;
    }
    
    /* Check CHECKSUM */
    for ( mu16i = 0 ; mu16i < ( _STRLEN_( (const char __far*) ai8RxProtocol ) - 3 ) ; mu16i++ )
    { 
        if ( mu16i > LENGTH_WIFI_ALL_RX_MAX )
        // if ( mu16i > LENGTH_WIFI_ALL_MAX_N )
        // if ( mu16i > LENGTH_WIFI_ALL_MAX )
        {
            break;
        }
        else
        {
            mu32CheckSumBuf += ai8RxProtocol[mu16i];
        }
    }
    mu8CheckSum = (U8)mu32CheckSumBuf;
    mu8RxCheckSum = (U8)((WIFI_ASCII2HEX(ai8RxProtocol[_STRLEN_( (const char __far*) ai8RxProtocol)-3]) << 4) & 0xF0);
    mu8RxCheckSum += (U8)(WIFI_ASCII2HEX(ai8RxProtocol[_STRLEN_( (const char __far*) ai8RxProtocol)-2]) & 0x0F);
    if ( mu8RxCheckSum != mu8CheckSum )
    {
        return FALSE;
    }
    return TRUE;
}

static U8 IsRecvPossible ( void )
{
    U16 mu16ExceptId = 0U;

    mu16ExceptId = (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_DATA]) << 12) & 0xF000;
    mu16ExceptId += (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_DATA+1]) << 8) & 0x0F00;
    mu16ExceptId += (WIFI_ASCII2HEX(ai8RxProtocol[POSITION_DATA+2]) << 4) & 0x00F0;
    mu16ExceptId += WIFI_ASCII2HEX(ai8RxProtocol[POSITION_DATA+3]) & 0x000F;

    if ( GetUserInterface( USER_SMART_SENSING_STATUS ) == TRUE )
    { 
        if ( IsExceptId( mu16ExceptId ) == TRUE )
        { 
            return TRUE;
        }
    }
    
    // if ( GetUserInterface( USER_RX_STATUS ) != TRUE ) 
    if ( GetUserInterface( USER_RX_STATUS ) == TRUE )  // A1011 RX IMPOSSIBLE
    {
        if ( IsExceptId( mu16ExceptId ) == TRUE ) 
        {
            // if ( GetUserInterface( USER_SMART_POSSIBLE_STATUS ) == TRUE ) 
            if ( GetUserInterface( USER_SMART_POSSIBLE_STATUS ) != TRUE )  // POSSIBLE
            {
                return TRUE;
            }
            else
            {
                return FALSE; // 정밀진단 중인 경우 정밀진단 종료 지시 이외 다른 입력 금지
            }
        }
    }
    else 
    {
        if ( IsExceptId( mu16ExceptId ) == TRUE ) // A1011 RX POSSIBLE
        {
            // if ( GetUserInterface( USER_SMART_POSSIBLE_STATUS ) != TRUE ) 
            if ( GetUserInterface( USER_SMART_POSSIBLE_STATUS ) == TRUE ) // IMPOSSIBLE
            {
                return FALSE;
            }
        }
        return TRUE;
    }
    return FALSE;
}

static U8 RxA1011 ( void )
{
    U16 i = 0U;
    U8 mData = 0U;
    // static I8 pBuf[LENGTH_WIFI_DATA_ID];
    static I8 pBuf[LENGTH_WIFI_DATA_RX_MAX];

    mData = GetWifiBLESendStatus(TX_BLE_WIFI);

    _MEMSET_( (void __far*) pBuf, '\0', sizeof(pBuf) );    

    /* Validity Check */
    if ( IsRecvPossible() != TRUE )
    {
        SetWifiBuzzSound( WIFI_BUZZER_ERROR );
        if (mData != BLEDATA) // BLE DATA TEST용
        {
            return TRUE;
        }
    }

    /* Run */
    // for ( i = 0 ; i < LENGTH_WIFI_DATA_ID ; i++ )
    for ( i = 0 ; i < LENGTH_WIFI_DATA_RX_MAX ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i];
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }
    DoFuncData( pBuf );
#ifdef USER_USE_ACK
    return TRUE;
#endif
    return FALSE;
}

static U8 RxA1091 ( void ) // Diagnosis RESULT
{
    U16 i = 0U;
    static I8 pBuf[LENGTH_WIFI_DATA_DR_MAX];
    
    _MEMSET_( (void __far*) pBuf, '\0', sizeof(pBuf) );

	/* Run */
    for ( i = 0 ; i < LENGTH_WIFI_DATA_DR_MAX ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i]; // { ~ }
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }
    DoDiagnosisResultData( pBuf );
    return FALSE;
}

static U8 RxA2011 ( void )
{
    U16 i = 0U;
    static I8 pBuf[LENGTH_WIFI_INFO_MAX];
    
    _MEMSET_( (void __far*) pBuf, '\0', sizeof(pBuf) );

    WifiCheckInitializeInfoAck();
    
	/* Run */
    for ( i = 0 ; i < LENGTH_WIFI_INFO_MAX ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i]; // { ~ }
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }
    DoInfoData( pBuf );
    return FALSE;
}

static U8 RxA2021 ( void )
{
#if (USER_USE_A2021_SAPINFO == 1)
    // const char *json_test_str =   "0030:{\"c\": \"S1000\",\"fc\":{\"ld\":\"202303131232\",\"nd\":\"202303131332\"}},0031:{\"c\": \"S1000\",\"sl\":[{\"ss\":\"1\",\"mt\":\"1\",\"ld\":\"20220107\",\"nd\":\"20220109\",\"fr\":\"25.5\"},{\"ss\":\"2\",\"mt\":\"1\",\"ld\":\"20220107\",\"nd\":\"20220109\",\"fr\":\"25.5\"}]},0032:{\"c\":\"S1000\",\"spb\":\"1.2\",\"cs\":\"20.5\",\"mct\":\"2.3\"}}";
                        
    U16 i = 0U;
    U16 mu16InfoCount = 0U;
    U8 mu8AsciiOpenCount = 0U;
    // static I8 pBuf[LENGTH_WIFI_JSON_MAX];
    static I8 pBuf[LENGTH_WIFI_ALL_MAX_N];
    _MEMSET_( (void __far*) pBuf, '\0', sizeof(pBuf) );
	/* Run */
    mu8AsciiOpenCount++;
    for ( i = 0 ; i < LENGTH_WIFI_ALL_MAX_N ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i]; // { ~ }
        if (pBuf[mu16InfoCount] == ASCII_OPEN) // '{' (count +1)
        {
            mu8AsciiOpenCount++;
        }
        else if (pBuf[mu16InfoCount] == ASCII_CLOSE) // '}' (count -1)
        {
            mu8AsciiOpenCount--;
        }
        
        if (( pBuf[i] == ASCII_CLOSE )  && (!mu8AsciiOpenCount))  // { 0:{ },1:{} ... } END Close Check
        {
            break;
        }
    }
    DoInfoJsonData( pBuf ); 
    // DoInfoJsonData( json_test_str );
#endif
    return FALSE;
}

// BEL Cert RX
static U8 RxA7011 ( void )
{
    U16 i = 0U;
    I8 mi8Id[LENGTH_WIFI_DATA_ONE];
    U8 mu8i = 0U;
    I8 mi8Data[LENGTH_WIFI_DATA_ID];
    static I8 pBuf[LENGTH_WIFI_DATA_ID];
    _MEMSET_( (void __far*) pBuf, '\0', sizeof(pBuf) );    
    /* Run */
    for ( i = 0 ; i < LENGTH_WIFI_DATA_ID ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i];
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }

    /* Control */
    
    _MEMSET_( (void __far*) mi8Id, '\0', sizeof(mi8Id) );

    MakeCheckId( mi8Id, 0, WIFI_BLE_CERT_0001); // A7011.0001 인 경우

    if ( _STRNCMP_( (const char __far*) mi8Id, (const char __far*) pBuf, LENGTH_WIFI_ID_ONE ) == N_TRUE ) // ID+:(5Byte) check
    {
        for ( mu8i = 0 ; mu8i < LENGTH_WIFI_DATA_ONE ; mu8i++ )
        {
            if ( ( pBuf[LENGTH_WIFI_ID_ONE+mu8i] == ASCII_COMMA ) || ( pBuf[LENGTH_WIFI_ID_ONE+mu8i] == ASCII_CLOSE ) )
            {
                break;
            }
            else
            {
                mi8Data[mu8i] = pBuf[LENGTH_WIFI_ID_ONE+mu8i];
            }
        }

        if((U8)(mi8Data[0]) == ASCII_NO_1) // Value 가 '1'인 경우
        {
            SetWifiApStatus(STATUS_AP_BLE_CERT,SET);    // 점유인증 가능(조작 대기) 상태로 변경
            SetWifiCertID(WIFI_BLE_CERT_0002); // A7010.0002 :
            SetWifiCertStatus(STATUS_CERT_ON); // 0001         
            SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_BLE_CERT); // 점유인증 성공 전송 (A7010)
            SetWifiBuzzSound( WIFI_BUZZER_BLE_CERT ); // 점유인증 안내 음성
        }
        else // ASCII_NO_0 OR ASCII_NO_2 ?
        {
            // SetWifiApStatus(STATUS_AP_BLE_CERT,CLEAR);      // 점유인증 조작 불가 상태로 변경
            SetWifiCertID(WIFI_BLE_CERT_0002); // A7010.0002 :
            SetWifiCertStatus(STATUS_CERT_FAIL); // 0002          
            SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_BLE_CERT); // 점유인증 실패 전송 (A7010)
        }
    }


    return FALSE;
}

// monitor test
static U8 RxA9011 ( void )
{
    U16 i = 0U;
        static I8 pBuf[LENGTH_WIFI_RAM_WRITE]; // Write ?? ?? Size
        
        _MEMSET_( (void __far*) pBuf, '\0', sizeof(pBuf) );

        /* Run */
        for ( i = 0 ; i < LENGTH_WIFI_RAM_WRITE ; i++ )
        {
            pBuf[i] = ai8RxProtocol[POSITION_DATA + i]; // { ~ }
            if ( pBuf[i] == ASCII_CLOSE )
            {
                break;
            }
        }
        
        DoMonitorData(WiFi_M_RAM_Write, pBuf );
        GetRamData();
    #ifdef USER_USE_ACK
        return TRUE;
    #endif
        return FALSE;
}


// Monitor Ram Read
static U8 RxA9013 ( void )
{
    U16 i = 0U;
    static I8 pBuf[LENGTH_RAM_DATA_MAX]; // Read ?? ?? Size
    _MEMSET_( (void __far*) pBuf, '\0', sizeof(pBuf) );
    
	/* Run */
    for ( i = 0 ; i < LENGTH_RAM_DATA_MAX ; i++ )
    {
        pBuf[i] = ai8RxProtocol[POSITION_DATA + i]; // { ~ }
        if ( pBuf[i] == ASCII_CLOSE )
        {
            break;
        }
    }
    GetRamData();
    DoMonitorData(WiFi_M_RAM_Read, pBuf );
#ifdef USER_USE_ACK
    return TRUE;
#endif
    return FALSE;
}

static void DoFuncData ( I8* pBuf )
{

    WifiTxFuncList_T *pList = NULL;
    WifiTxFuncEventList_T *pListID = NULL;
    U16 i = 0U;
    U16 mu16Size = 0U;
    U16 mu16FuncCount = 0U;
    U8 mi = 0U;
    U8 mu8FuncListCheck = 0U;

    /* Table Matching */
    pList = (WifiTxFuncList_T *)GetWifiFuncEventList(); 
    pListID = (WifiTxFuncEventList_T *)GetWifiFuncDataEventList(); 
    mu16Size = GetWifiFuncEventListSize(); 
    
    /* Control */
    for ( i = 0 ; i < mu16Size; i++ )
    {
        WifiRxFun_T pFun;
        I8 mi8Id[LENGTH_WIFI_DATA_ONE];
        I8 mId[LENGTH_WIFI_ID_ONE+1]; // Ref
        U8 mu8i = 0U;
        U8 mu8Func = 0U;
        U8 mu8FuncFinish = 0U;
        I8 mi8Func[LENGTH_WIFI_DATA_ONE];

        _MEMSET_( (void __far*) mId, '\0', sizeof(mId) );
        _MEMSET_( (void __far*) mi8Id, '\0', sizeof(mi8Id) );
        _MEMSET_( (void __far*) mi8Func, '\0', sizeof(mi8Func) );

        for ( mu8Func = 0 ; mu8Func < LENGTH_WIFI_ID_ONE ; mu8Func++ )
        {
            mi8Id[mu8Func] = pBuf[mu16FuncCount]; // { ~ }
            mu16FuncCount++;
            if ( mi8Id[mu8Func] == ASCII_COLON )
            {
                break;
            }
        }

        for ( mu8Func = 0 ; mu8Func < LENGTH_WIFI_DATA_ONE ; mu8Func++ )
        {
            mi8Func[mu8Func] = pBuf[mu16FuncCount]; // { ~ }
            mu16FuncCount++;
            if ( mi8Func[mu8Func] == ASCII_COMMA)
            {
                mu8FuncListCheck = SET;
            }
            if (mi8Func[mu8Func] == ASCII_CLOSE)
            {
                mu8FuncFinish = SET;
            }
            if ( mi8Func[mu8Func] == ASCII_COMMA || mi8Func[mu8Func] == ASCII_CLOSE )
            {
                mi8Func[mu8Func] = ASCII_NULL;
                break;
            }
        }
        for ( mi = 0 ; mi < mu16Size; mi++ )
        {
            // pFun = (pList+mi)->RxFunc;
            // MakeCheckId( mId, 0, (pList+mi)->Id );        
            pFun = (pList)->RxFunc;
            MakeCheckId( mId, 0, (pListID+mi)->Id );        
        
            if ( _STRNCMP_( (const char __far*) mId, (const char __far*) mi8Id, LENGTH_WIFI_ID_ONE ) == N_TRUE ) // ID+:(5Byte) check
            {
                pFun( (pListID+mi)->Id, mi8Func );

                if ( ( IsExceptId( (pListID+mi)->Id ) == TRUE ) && ( mi8Func[0] == ASCII_NO_1 ) ) // Smart Diagnosis
                {
                    SetRequestIdExcept();
                }
                break;
            }
        }
        if (mu8FuncFinish == SET)
        {
            break;
        }

    }
}

static void DoDiagnosisResultData ( I8* pBuf )
{
    WifiTxFuncList_T *pList = NULL;
    WifiNormalList_T *pListNormalID = NULL;
    U16 i = 0U;
    U16 mu16Size = 0U;
    U16 mu16Count = 0U;
    U8 mi = 0U;
	  

    pList = (WifiTxFuncList_T *)GetWifiPartEventList(); 
    pListNormalID = (WifiNormalList_T *)GetWifiPartDataEventList();
    mu16Size = GetWifiPartEventListSize(); 
    
    
    for ( i = 0 ; i < mu16Size; i++ )
    {
        U8 mu8work = 0U;
        WifiRxFun_T pFun;
        I8 mi8Id[LENGTH_WIFI_ID_ONE+1];
        I8 mId[LENGTH_WIFI_ID_ONE+1]; // Ref
        I8 mi8work[LENGTH_WIFI_DATA_ONE];
        // *ICT*AWS_RECV:00??A109122MCU2SVR0007-01{0000:1,0001:1,0002:1,0003:1,0004:1,0005:1,0006:1,0007:1,0010:1,0011:1,0014:1,0015:1,0016:1,0026:1}
        _MEMSET_( (void __far*) mId, '\0', sizeof(mId) );
        _MEMSET_( (void __far*) mi8Id, '\0', sizeof(mi8Id) );
        _MEMSET_( (void __far*) mi8work, '\0', sizeof(mi8work) );

        for ( mu8work = 0 ; mu8work < LENGTH_WIFI_ID_ONE ; mu8work++ )
        {
            mi8Id[mu8work] = pBuf[mu16Count]; // { ~ }
            mu16Count++;
            if ( mi8Id[mu8work] == ASCII_COLON )
            {
                break;
            }
        }

        for ( mu8work = 0 ; mu8work < LENGTH_WIFI_DATA_ONE ; mu8work++ )
        {
            mi8work[mu8work] = pBuf[mu16Count]; // { ~ }
            mu16Count++;
            if ( mi8work[mu8work] == ASCII_COMMA || mi8work[mu8work] == ASCII_CLOSE )
            {
                mi8work[mu8work] = ASCII_NULL;
                break;
            }
        }

        for ( mi = 0 ; mi < mu16Size; mi++ )
        {
            pFun = (pList)->RxFunc;
            MakeCheckId( mId, 0, (pListNormalID+mi)->Id );        
        
            if ( _STRNCMP_( (const char __far*) mId, (const char __far*) mi8Id, LENGTH_WIFI_ID_ONE ) == N_TRUE ) // ID+:(5Byte) check
            {
                pFun( (pListNormalID+mi)->Id, mi8work );
                break;
            }
        }
    }

}


static void DoInfoData ( I8* pBuf )
{
    WifiTxFuncList_T *pList = NULL;
    WifiNormalList_T *pListNormalID = NULL;
    U16 i = 0U;
    U16 mu16Size = 0U;
    U16 mu16InfoCount = 0U;
    U8 mi = 0U;
    //_MEMSET_( (void __far*) mi8Data, '\0', sizeof(mi8Data) );

    pList = (WifiTxFuncList_T *)GetWifiInformationEventList(); 
    pListNormalID = (WifiNormalList_T *)GetWifiInfoDataEventList();

    // mu16Size = GetWifiInformationListSize(); 
    mu16Size = GetWifiInfoEventListSize(); 
    
    
    for ( i = 0 ; i < mu16Size; i++ )
    {
        U8 mu8Info = 0U;
        WifiRxFun_T pFun;
        I8 mi8Id[LENGTH_WIFI_ID_ONE+1];
        I8 mId[LENGTH_WIFI_ID_ONE+1]; // Ref
        I8 mi8Info[LENGTH_WIFI_DATA_ONE];
        // *ICT*AWS_RECV:0088A201122MCU2SVR0007-01{0000:2022,0001:7,0002:27,0003:9,0004:32,0005:21,0006:3,0007:1,0010:4,0011:2,0014:28,0015:32,0016:26,0026:65535}
        _MEMSET_( (void __far*) mId, '\0', sizeof(mId) );
        _MEMSET_( (void __far*) mi8Id, '\0', sizeof(mi8Id) );
        _MEMSET_( (void __far*) mi8Info, '\0', sizeof(mi8Info) );

        for ( mu8Info = 0 ; mu8Info < LENGTH_WIFI_ID_ONE ; mu8Info++ )
        {
            mi8Id[mu8Info] = pBuf[mu16InfoCount]; // { ~ }
            mu16InfoCount++;
            if ( mi8Id[mu8Info] == ASCII_COLON )
            {
                break;
            }
        }

        for ( mu8Info = 0 ; mu8Info < LENGTH_WIFI_DATA_ONE ; mu8Info++ )
        {
            mi8Info[mu8Info] = pBuf[mu16InfoCount]; // { ~ }
            mu16InfoCount++;
            if ( mi8Info[mu8Info] == ASCII_COMMA || mi8Info[mu8Info] == ASCII_CLOSE )
            {
                mi8Info[mu8Info] = ASCII_NULL;
                break;
            }
        }

        for ( mi = 0 ; mi < mu16Size; mi++ )
        {
            pFun = (pList)->RxFunc;
            MakeCheckId( mId, 0, (pListNormalID+mi)->Id );        
        
            if ( _STRNCMP_( (const char __far*) mId, (const char __far*) mi8Id, LENGTH_WIFI_ID_ONE ) == N_TRUE ) // ID+:(5Byte) check
            {
                pFun( (pListNormalID+mi)->Id, mi8Info );
                break;
            }
        }
    }
}

static void DoInfoJsonData ( I8* pBuf )
{
    WifiTxFuncList_T *pList = NULL;
    WifiNormalList_T *pListNormalID = NULL;
    U16 i = 0U;
    U16 mu16Size = 0U;
    U16 mu16InfoCount = 0U;
    U8 mu8AsciiOpenCount = 0U;
    U8 mi = 0U;

    static I8 mi8Info[LENGTH_WIFI_JSON_ONE];  

    pList = (WifiTxFuncList_T *)GetWifiInformationJsonEventList(); 
    pListNormalID = (WifiNormalList_T *)GetWifiInfoDataEventList();

    // mu16Size = GetWifiInformationJsonListSize(); 
    // mu16Size = GetWifiTxStatus(STATUS_TX_INFO_SIZE);
    mu16Size = GetWifiInfoEventListSize();
    
    for ( i = 0 ; i < mu16Size; i++ )
    {
        U16 mu16Info = 0U;
        WifiRxFun_T pFun;
        I8 mi8Id[LENGTH_WIFI_ID_ONE+1];
        I8 mId[LENGTH_WIFI_ID_ONE+1]; // Ref
        

        _MEMSET_( (void __far*) mId, '\0', sizeof(mId) );
        _MEMSET_( (void __far*) mi8Id, '\0', sizeof(mi8Id) );
        _MEMSET_( (void __far*) mi8Info, '\0', sizeof(mi8Info) );

        for ( mu16Info = 0 ; mu16Info < LENGTH_WIFI_ID_ONE ; mu16Info++ ) // ID
        {
            mi8Id[mu16Info] = pBuf[mu16InfoCount]; // { ~ }
            mu16InfoCount++;
            if ( mi8Id[mu16Info] == ASCII_COLON )
            {
                break;
            }
        }

        for ( mu16Info = 0 ; mu16Info < LENGTH_WIFI_JSON_ONE ; mu16Info++ )
        {
            if (pBuf[mu16InfoCount] == ASCII_OPEN) // '{' (count +1)
            {
                mu8AsciiOpenCount++;
            }
            else if (pBuf[mu16InfoCount] == ASCII_CLOSE) // '}' (count -1)
            {
                mu8AsciiOpenCount--;
            }

            mi8Info[mu16Info] = pBuf[mu16InfoCount]; // { ~ }
            mu16InfoCount++;
            if (( mi8Info[mu16Info] == ASCII_CLOSE ) && (mu8AsciiOpenCount == 0U))  // {{ }} Json END Check
            {
                mu16Info++;
                mi8Info[mu16Info] = ASCII_NULL;
                if (pBuf[mu16InfoCount] == ASCII_COMMA) // 0:{{ }},1:{} << ',' PASS 
                {
                    mu16InfoCount++;
                }
                break;
            }
        }

        for ( mi = 0 ; mi < mu16Size; mi++ )
        {
            pFun = (pList)->RxFunc; 
            MakeCheckId( mId, 0, (pListNormalID+mi)->Id );        
        
            if ( _STRNCMP_( (const char __far*) mId, (const char __far*) mi8Id, LENGTH_WIFI_ID_ONE ) == N_TRUE ) // ID+:(5Byte) check
            {
                pFun((pListNormalID+mi)->Id, mi8Info );
                break;
            }
        }
    }
}

static void DoMonitorData (U8 rw_sel, I8* pBuf )
{
    WifiTxFuncList_Monitor_T *pList = NULL;
    U16 i = 0U;
    U16 mu16Size = 0U;
    U16 mu16DataCount = 0U;
    U8 mi = 0U;
    U8 mcount = 0U;

    pList = (WifiTxFuncList_Monitor_T *)GetWifiMonitorEventList(); 

    for ( i = 0 ; i < LENGTH_WIFI_ALL_RX_MAX; i++ )
    // for ( i = 0 ; i < LENGTH_WIFI_ALL_MAX_N; i++ )
    // for ( i = 0 ; i < LENGTH_WIFI_ALL_MAX; i++ )
    {
        if (pBuf[i] == ASCII_COMMA) 
        {
            mcount++;
        }
        else if (pBuf[i] == ASCII_CLOSE)
        {
            mcount++;
            break;
        }
    }
    mu16Size = mcount; // ',' 갯수를 count 하여 요청 DATA가 몇개인지 확인
    gu16_RAM_Monitor_RESP_SIZE = mu16Size; // 전역변수에 저장 (TxMake 에서 cmd size로 사용)

    for ( i = 0 ; i < mu16Size; i++ ) // RAM Address 찾기
    {
        U8 mu8LenCount = 0U;
        WifiRxFun_T pFun;
        I8 mi8Address[LENGTH_WIFI_ID_ONE+1];
        I8 mi8Data[LENGTH_WIFI_RAM_DATA];
        U16 m16Address_HEX = 0U;
        U16 m16Address_HEX_buf;
        U8 mu8WriteFlag = FALSE;
        U8 mu8ReadFlag = FALSE;

        _MEMSET_( (void __far*) mi8Address, '\0', sizeof(mi8Address) );
        _MEMSET_( (void __far*) mi8Data, '\0', sizeof(mi8Data) );

        for ( mu8LenCount = 0 ; mu8LenCount < LENGTH_WIFI_ID_ONE+1 ; mu8LenCount++ )
        {

            if(pBuf[mu16DataCount] == ASCII_COLON)
            {
                mu16DataCount++;
                break;
            }
            else if (pBuf[mu16DataCount] == ASCII_Z) // [Z1011.] Pass
            {
                mu16DataCount += LENGTH_WIFI_A90xx_PASS;
                mu8LenCount--;
            }
            else{
                mi8Address[mu8LenCount] = pBuf[mu16DataCount]; // {Z1011. ~ }
                mu16DataCount++;
            }
        }

        for ( mu8LenCount = 0 ; mu8LenCount < LENGTH_WIFI_RAM_DATA-1; mu8LenCount++ ) // RAM DATA 값 찾기, [RAM DATA 는 10진수로만 사용}
        {

            mi8Data[mu8LenCount] = pBuf[mu16DataCount]; // { FFFF(adress): ~ }
            mu16DataCount++;
            if ((rw_sel == WiFi_M_RAM_Write) && (mu8WriteFlag == FALSE))
            {
                mi8Data[++mu8LenCount] = 0x31;   // Write A9011 [FFFF(address): ?1(DATA)]
                mu8WriteFlag = TRUE;

            }
            else if ((rw_sel == WiFi_M_RAM_Read) && (mu8ReadFlag == FALSE)) {
                mi8Data[++mu8LenCount] = 0x30;   // Read A9013
                mi8Data[++mu8LenCount] = 0x30;   // Read A9013 [FFFF(address): ?00]
                mu8ReadFlag = TRUE;
            }

            if ( mi8Data[mu8LenCount] == ASCII_COMMA || mi8Data[mu8LenCount] == ASCII_CLOSE )
            {
                mi8Data[mu8LenCount] = ASCII_NULL;
                break;
            }

        }
        mi8Data[LENGTH_WIFI_RAM_DATA-1] = (I8)((mu8LenCount)-2); // Write DATA값 문자열 길이를 맨 마지막 번지에 저장 
                                                         // SetUserSystemRamMonitor에서 ASCII를 10진수로 변환 시 사용
        gu16_RAM_Address_len[i] = WIFI_ASCII2HEX(mi8Data[0]) & 0x000F; //mi8Data[0]; // ADDRESS 별 Len DATA 저장

        for ( mi = 0 ; mi < mu16Size; mi++ )
        {   
            m16Address_HEX_buf = (WIFI_ASCII2HEX(mi8Address[0])) ; // ASCII를 16진수로 변환
            m16Address_HEX = (m16Address_HEX_buf << 12) & 0xF000;
            m16Address_HEX_buf = (WIFI_ASCII2HEX(mi8Address[1])) ;
            m16Address_HEX += (m16Address_HEX_buf << 8) & 0x0F00;
            // m16Address_HEX = (WIFI_ASCII2HEX(mi8Address[0]) << 12) & 0xF000; 
            // m16Address_HEX += (WIFI_ASCII2HEX(mi8Address[1]) << 8) & 0x0F00;
            m16Address_HEX += (WIFI_ASCII2HEX(mi8Address[2]) << 4) & 0x00F0;
            m16Address_HEX += WIFI_ASCII2HEX(mi8Address[3]) & 0x000F;

            gu16_START_RAM_Address[i] = m16Address_HEX;

            pFun = (pList)->RxFunc_M; //RAM Address 갯수만큼 호출하기 때문에 (pList+i)->RxFunc 가 아님.
            if ( (pFun != NULL)&&(rw_sel == WiFi_M_RAM_Write) )
            {
                pFun(m16Address_HEX, mi8Data ); // m8Address_HEX: start ram address(ex: F588)
                break;                         // mi8Data: 2165535 (len(ex: 2), R(0)/W(1), write data:65535)
            }
        }

    }
}

// Make Id (4Byte + 1Byte)
static void MakeCheckId ( I8* pBuf, U16 mLen, U16 mId )
{
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 12) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 8) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId >> 4) & 0x000F));
    pBuf[mLen++] = WIFI_HEX2ASCII((U8)((mId) & 0x000F));
    pBuf[mLen++] = ASCII_COLON; // 0x3A :
}

