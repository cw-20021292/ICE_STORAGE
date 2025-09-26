
/*******************************************************************************
&Description :
    WIFI Data Check Operation

&History : ShinHM
    2021.05.17 update
    2021.12.22 update

&History : ssg@coway.com
    2023.03.15 DoSend5min �߰�, CheckUartRx�߰�
    2023.03.30 BLE Tx ó�� �߰�
*******************************************************************************/


/***** Header *****************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_DefUser.h"
#include "WIFI_DataCheck.h"
#include "WIFI_Initialize.h"
#include "WIFI_RxProcess.h"
#include "WIFI_TxMake.h"
#include "WIFI_Fota.h"
#include "WIFI_Status.h"
#include "WIFI_UartInterrupt.h"
#include "WIFI_UserInterface.h"
#include "WIFI_Control.h"


/***** Define *****************************************************************/
typedef U8 (*WifiCheckFun_T)(void);
typedef void (*WifiDoFun_T)(void);

typedef enum
{
    CHK_MODULE_ONOFF = 0x00,    // HW Module
    CHK_ALWAYS_100MS,           // Always

    CHK_BOOT_MODULE,            // Check Booting
    CHK_BOOT_FOTA,              // Check Booting, After Fota

    CHK_APMODE_START,           // Wifi Apmode
    CHK_AP_BLE_START,           // Ble Apmode
    CHK_APMODE_FAIL,            // Check Fail

    CHK_SERVER_CNT,             // Check Server Connect
    CHK_SERVER_DCNT,            // Check Server Disconnect

    CHK_FIRST_DATA,             // First Data matching, After Connect
    CHK_FIRST_FAIL,             // Check Fail 

    CHK_UART_RX,
    CHK_SEND_DATA,              // Check Send Data
    CHK_SEND_ACK,               // Check ACK
    CHK_SEND_1MIN,              // Period 1m 
    CHK_SEND_5MIN,              // Period 5m 
    CHK_RQST_20MIN,             // Period 20m
    CHK_RQST_24HOUR,            // Period 24hr
    CHK_BLE_10MIN,              // BLE 10Min

    CHK_FOTA_START,             // Check Fota
    CHK_WiFi_CONNECT,           // 1m [Wi-Fi ���� ���� ǥ�� üũ 1��]

    CHK_CERT_TIME,              // 1m [������������ ��� �ð� 1��]

    CHK_BLE2_ACK,               // [BLE STATUS2 ACK Ȯ�� �� ��õ� ]
    CHK_BLEUART_RX,             // [BLE STATUS2 ��õ� �� ��⸮��]

    CHK_NUM
} E_WIFI_CHECK_FUN_T;


/***** Variable ***************************************************************/
typedef struct _wificheck_
{
    U16  u16Count;
} WifiCheck_T;
WifiCheck_T WifiCheck[CHK_NUM];

extern WifiRx_T WifiRx;
/***** Constant ***************************************************************/


/***** Static Function ********************************************************/
static U8 CheckModuleOnOff ( void );
static void DoModuleOnOff ( void );
static U8 CheckAlways ( void );
static void DoAlways100ms ( void );

static U8 CheckBootModule ( void );
static U8 CheckBootFota ( void );

static U8 CheckApModeStart ( void );
static U8 CheckApBleStart ( void );
static U8 CheckApModeFail ( void );

static U8 CheckServerConnect ( void );
static U8 CheckServerDisconnect ( void );

static U8 CheckFirstData ( void );
static U8 CheckFirstFail ( void );
static U8 CheckUartRx ( void );

static U8 CheckSendData ( void );
static U8 CheckSendAck ( void );
static U8 CheckConnect ( void );
static U8 CheckBleData ( void );

static U8 CheckFotaStart ( void );
static U8 CheckWiFiConnect ( void );
static U8 CheckCertTime ( void );
static U8 CheckBleStatus2 ( void );
static U8 CheckBleUart ( void );
static void DoReset ( void );
static void ServerCheckReset ( void );
static void DoOff ( void );

static void DoApModeStart ( void );
static void DoApBleStart ( void );
static void DoApModeFail ( void );

static void DoFirstData ( void );
static void DoFirstFail ( void );

static void DoSendData ( void );
static void DoSendOk ( void );
static void DoSend1min ( void );
static void DoSend5min ( void );
static void DoRqst20min ( void );
static void DoRqst24Hour ( void );
static void BleTxOFF ( void );

static void DoFotaStart ( void );
static void DoWifiIndicator ( void );
static void DoCertCancel ( void );
static void BleStatus2Retry ( void );


/***** Table ******************************************************************/
typedef struct _wifi_datacheck_list_
{
    E_WIFI_CHECK_FUN_T	Id;
    U8  u8TypeTime;
    U16 u16Count;
    WifiCheckFun_T CheckFunc;
    WifiDoFun_T DoFunc;
} WifiDataCheckList_T;

static void RunChkData (__far WifiDataCheckList_T *pList, U8 i );

// Check Count Table
__far static const WifiDataCheckList_T WifiDataCheckList[] =
{   //  ID                  DefineTime          Count   CheckFunction           DoFunction
    {   CHK_MODULE_ONOFF,   WIFI_TIME_100MS,    5,      CheckModuleOnOff,       DoModuleOnOff   },
    {   CHK_ALWAYS_100MS,   WIFI_TIME_100MS,    1,      CheckAlways,            DoAlways100ms   },

    {   CHK_BOOT_MODULE,    WIFI_TIME_1SEC,     60,     CheckBootModule,        DoReset         },
    {   CHK_BOOT_FOTA,      WIFI_TIME_1SEC,     90,     CheckBootFota,          DoReset         },

    {   CHK_APMODE_START,   WIFI_TIME_1SEC,     2,      CheckApModeStart,       DoApModeStart   },
    {   CHK_AP_BLE_START,   WIFI_TIME_1SEC,     2,      CheckApBleStart,        DoApBleStart    },
    {   CHK_APMODE_FAIL,    WIFI_TIME_1SEC,     1800,   CheckApModeFail,        DoApModeFail    }, // �� ��� üũ 30��

    {   CHK_SERVER_CNT,     WIFI_TIME_1SEC,     180,    CheckServerConnect,     ServerCheckReset}, // �������� üũ 3�� 
    {   CHK_SERVER_DCNT,    WIFI_TIME_100MS,    30,     CheckServerDisconnect,  DoOff           },

    {   CHK_FIRST_DATA,     WIFI_TIME_100MS,    10,     CheckFirstData,         DoFirstData     },
    {   CHK_FIRST_FAIL,     WIFI_TIME_1SEC,     60,     CheckFirstFail,         DoFirstFail     },

    {   CHK_UART_RX,        WIFI_TIME_1SEC,     480,    CheckUartRx,            DoReset         }, // UART üũ 8��
    {   CHK_SEND_DATA,      WIFI_TIME_100MS,    2,      CheckSendData,          DoSendData      }, // DataCheck�� 100ms -> 200ms�� ����
    {   CHK_SEND_ACK,       WIFI_TIME_1SEC,     480,    CheckSendAck,           DoReset         }, // IND CHECK 480�� ���� ������� �� [CHK_SERVER_CNT] ī��Ʈ ����

    {   CHK_SEND_1MIN,      WIFI_TIME_1SEC,     60,     CheckSendAck,           DoSend1min      },
    {   CHK_SEND_5MIN,      WIFI_TIME_1SEC,     300,    CheckConnect,           DoSend5min      },
    {   CHK_RQST_20MIN,     WIFI_TIME_1SEC,     1201,   CheckConnect,           DoRqst20min     },
    {   CHK_RQST_24HOUR,    WIFI_TIME_1MIN,     1440,   CheckConnect,           DoRqst24Hour    },
    {   CHK_BLE_10MIN,      WIFI_TIME_1SEC,     600,    CheckBleData,           BleTxOFF        },

    {   CHK_FOTA_START,     WIFI_TIME_100MS,    10,     CheckFotaStart,         DoFotaStart     },
    {   CHK_WiFi_CONNECT,   WIFI_TIME_1SEC,     60,     CheckWiFiConnect,       DoWifiIndicator }, // �������� ���� �� ǥ�� ���� 1��
    {   CHK_CERT_TIME,      WIFI_TIME_1SEC,     60,     CheckCertTime,          DoCertCancel    },  
    {   CHK_BLE2_ACK,       WIFI_TIME_1SEC,     10,     CheckBleStatus2,        BleStatus2Retry }, // BLEȰ��ȭ(STATUS2) Retry 10�ʰ������� Retry 
    {   CHK_BLEUART_RX,     WIFI_TIME_1SEC,     60,     CheckBleUart,           DoReset         }  // BLEȰ��ȭ Uart CMD ���� �ȵ� �� ���� (1�� ��õ� ��)
};

#define SZ_CK_LIST  ( sizeof(WifiDataCheckList) / sizeof(WifiDataCheckList_T) )


/***** Global Function ********************************************************/
void WifiDataCheckProcess ( U8 mu8Type )
{
    __far const WifiDataCheckList_T *pList = NULL;
    U8 i = 0U;

    pList = WifiDataCheckList;

    for ( i = 0 ; i < SZ_CK_LIST; i++ )
    {
        if ( mu8Type == (pList+i)->u8TypeTime )
        {
            RunChkData( pList+i, i );
        }
    }
}

/***** Local Function *********************************************************/
static void RunChkData (__far WifiDataCheckList_T *pList, U8 i )
{
    WifiCheckFun_T pChFun;
    WifiDoFun_T pDoFun;
    pChFun = pList->CheckFunc;
    pDoFun = pList->DoFunc;
    if ( pChFun() == TRUE )
    {
        WifiCheck[i].u16Count++;
        if ( WifiCheck[i].u16Count >= pList->u16Count )
        {
            pDoFun();
            WifiCheck[i].u16Count = 0;
        }
    }
    else
    {
        WifiCheck[i].u16Count = 0;
    }
}

static U8 CheckModuleOnOff ( void )
{
    static U16 mu16PowerCount = 0U;
    static U8 mu8EasyPairing = 0U;
    static U8 mu8FactoryTest = 0U;

    mu16PowerCount++;
    if ( mu16PowerCount < W_COUNT_WIFI_POWER )
    {
        if ( GetUserInterface( USER_FACTORY_STATUS ) == SET )
        {
            WifiControlProcess(WIFI_TIME_FACTORY);
            mu16PowerCount = W_COUNT_WIFI_POWER;
            mu8FactoryTest = SET;
            HAL_SetWifiOnOff(ON);
        }
        return FALSE;
    }
    else if (( GetUserInterface( USER_FACTORY_STATUS ) == SET ) 
            && (mu8FactoryTest != SET))
    {
        WifiControlProcess(WIFI_TIME_FACTORY);
        mu8FactoryTest = SET;
        SetWifiApStatus(STATUS_AP_BLE_ALWAYS,CLEAR);
        WifiRx.u8DisplayConnectCount = CLEAR;
        HAL_SetWifiOnOff(RESET);
        return FALSE;
    }
    else
    {
        mu16PowerCount = W_COUNT_WIFI_POWER;
    }

    if ( GetWifiModuleStatus( STATUS_MODULE_POWER ) == OFF )
    {
        SetWifiModuleStatus( STATUS_MODULE_SEQ, MODULE_NULL );

        if ( ( GetWifiApStatus( STATUS_AP_BLE ) == OFF )
        && ( GetWifiApStatus( STATUS_AP_WIFI ) == CLEAR )
        && ( GetWifiFotaStatus( STATUS_FOTA_MODULE ) == CLEAR )
        && ( GetUserInterface( USER_FACTORY_STATUS ) == CLEAR )
        && ( mu8EasyPairing == CLEAR ) )
        {
            mu8EasyPairing = SET;
            IniEasyPairing();
        }
        return FALSE;
    }
    return TRUE;
}
static void DoModuleOnOff ( void )
{
    WifiModuleOnOffSeq();
}

static U8 CheckAlways ( void )
{
    if ( GetWifiControlStatus( STATUS_CON_POWER ) == ON )
    {
        return TRUE;
    }
    return FALSE;
}
static void DoAlways100ms ( void )
{
    WifiUartTxTimer();
    WifiUartRxTimer();
    //WifiRxAckTimer();
}

static U8 CheckBootModule ( void )
{
    if ( ( GetWifiControlStatus( STATUS_CON_POWER ) == ON )
        && ( GetWifiControlStatus( STATUS_CON_READY ) == OFF )
        && ( GetWifiApStatus( STATUS_AP_WIFI ) == CLEAR )
        && ( GetWifiApStatus( STATUS_AP_BLE ) == CLEAR )
        && ( GetWifiFotaStatus( STATUS_FOTA_MODULE ) == CLEAR ) )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckBootFota ( void )
{
    if ( ( GetWifiControlStatus( STATUS_CON_POWER ) == ON )
        && ( GetWifiControlStatus( STATUS_CON_READY ) == OFF )
        && ( GetWifiApStatus( STATUS_AP_WIFI ) == CLEAR )
        && ( GetWifiApStatus( STATUS_AP_BLE ) == CLEAR )
        && ( GetWifiFotaStatus( STATUS_FOTA_MODULE ) == SET ) )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckApModeStart ( void )
{
    if ( ( GetWifiControlStatus( STATUS_CON_READY ) == ON )
        && ( GetWifiApStatus( STATUS_AP_WIFI ) != CLEAR ) )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckApBleStart ( void )
{
    if ( ( GetWifiControlStatus( STATUS_CON_READY ) == ON )
        && ( GetWifiApStatus( STATUS_AP_BLE ) != CLEAR ) )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckApModeFail ( void )
{
    static U8 mu8BleStatus3Check = 0U;
    if ((GetWifiApStatus(STATUS_AP_BLE_CON) == SET) // BLE ���� �� 30�� �����
        && (mu8BleStatus3Check == CLEAR))
    {
        mu8BleStatus3Check = SET;
        return FALSE;
    }
    else if ( ( GetWifiApStatus( STATUS_AP_WIFI ) != CLEAR )
        || ( GetWifiApStatus( STATUS_AP_BLE ) != CLEAR ) )
    {
        return TRUE;
    }
    mu8BleStatus3Check = CLEAR;
    return FALSE;
}

static U8 CheckServerConnect ( void )
{
    if ( ( GetWifiControlStatus( STATUS_CON_READY ) == ON )
        && ( GetWifiApStatus( STATUS_AP_WIFI ) == CLEAR )
        // && ( GetWifiApStatus( STATUS_AP_BLE ) == CLEAR )  // ��� ON �� �������� üũ �� ���� (���� �� BLE �ٽ� ��Ȱ��ȭ)
        && ( GetWifiApStatus( STATUS_AP_BLE_CON ) == CLEAR )  // ��� ON �� Module <-> APP BLE ����Ϸ� ���� ���� ���
        && ( GetWifiServerStatus( STATUS_SVR_CONNECT ) == CLEAR ) )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckServerDisconnect ( void )
{
    if ( GetWifiServerStatus( STATUS_SVR_END ) == SET )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckFirstData ( void )
{
    if ( ( GetWifiControlStatus( STATUS_CON_READY ) == ON )
        && ( GetWifiIniStatus( STATUS_INI_FINISH ) != SET ) )
    {
        return TRUE;
    }
    SetWifiIniStatus( STATUS_INI_SEQ, INI_SEQ_NULL );
    return FALSE;
}

static U8 CheckFirstFail ( void )
{
    if ( ( GetWifiControlStatus( STATUS_CON_READY ) == ON )
        && ( GetWifiApStatus( STATUS_AP_WIFI ) == CLEAR )
        && ( GetWifiApStatus( STATUS_AP_BLE ) == CLEAR )
        && ( GetWifiIniStatus( STATUS_INI_FINISH ) != SET )
        && ( GetWifiServerStatus( STATUS_SVR_CONNECT ) == SET ) )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckUartRx ( void )
{
    if ( GetWifiControlStatus( STATUS_CON_POWER ) == ON ){
        if ( ( GetWifiApStatus( STATUS_AP_WIFI ) != CLEAR ) || ( GetWifiApStatus( STATUS_AP_BLE ) != CLEAR ) )
        {
                return FALSE;
        }
        else
        {   
            if (WifiRx.u8ConnectCheck == SET) {
                
                WifiRx.u8ConnectCheck = CLEAR;
                WifiRx.u16ConnectCheckCount = CLEAR;
                return FALSE;
            }
            WifiRx.u16ConnectCheckCount++;
            return TRUE;
        }
    }
    else{
        return FALSE;
    }

}

static U8 CheckSendData ( void )
{
    if ( ( GetWifiControlStatus( STATUS_CON_READY ) == ON )
        && ( GetWifiTxStatus( STATUS_TX_SEQ ) == WIFI_TX_SEQ_NULL ) )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckSendAck ( void )
{
    if ( ( GetWifiServerStatus( STATUS_SVR_CONNECT ) == SET )
        && ( GetWifiTxStatus( STATUS_TX_STATE ) == SET ) )
    {
        if ( ( GetWifiApStatus( STATUS_AP_WIFI ) == CLEAR ) 
            // && ( GetWifiApStatus( STATUS_AP_BLE ) == CLEAR ) 
            && ( GetWifiApStatus( STATUS_AP_BLE_CON ) == CLEAR ) )
        {
            return TRUE;
        }
    }
    return FALSE;
}

static U8 CheckConnect ( void )
{
    if ( GetWifiServerStatus( STATUS_SVR_CONNECT ) == SET )
    {
        return TRUE;
    }
    ClearUserData();
    return FALSE;
}

static U8 CheckBleData ( void )
{
    if(GetWifiApStatus(STATUS_AP_BLE) != CLEAR)
    {
        SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA);
    }
    else
    {
        if ((GetWifiBLESendStatus(TX_BLE_WIFI) == BLEDATA ) && (GetWifiBLESendStatus(BLE_RECV_CHECK) == CLEAR))
        {
            return TRUE;
        }
    }

    SetWifiBLESendStatus(BLE_RECV_CHECK, CLEAR);
    return FALSE;
}

static U8 CheckFotaStart ( void )
{
    if ( ( GetWifiFotaStatus( STATUS_FOTA_SEQ ) != FOTA_NULL )
        // && ( GetUserInterface( USER_FOTA_POSSIBLE_STATUS ) == TRUE ) )
        && ( GetUserInterface( USER_FOTA_POSSIBLE_STATUS ) != TRUE ) )
    {
        return TRUE;
    }
    return FALSE;
}

static U8 CheckWiFiConnect ( void )
{
    if (( WifiRx.u8DisplayConnectCount == SET) 
     && (GetWifiIndicateStatus(STATUS_DISPLAY) != WIFI_DISP_AP1) 
     && (GetWifiControlStatus(STATUS_CON_POWER) == ON )
     && ( ( GetWifiApStatus(STATUS_AP_WIFI) == CLEAR ) && (GetWifiApStatus(STATUS_AP_BLE) == CLEAR) ))
    {
        return TRUE;
    }
    return FALSE;
}
static U8 CheckCertTime ( void )
{
    if(GetWifiApStatus(STATUS_AP_BLE) != CLEAR) // BLE Ȱ��ȭ�� �ƴ� ���
    {
        if ( GetWifiApStatus(STATUS_AP_BLE_CERT) == SET) //�������� ���� ���ɻ��¿��� ���ѽð�(1��) üũ
        {
            #ifdef BLE_CERT_PASS
            WifiKey(WIFI_KEY_BLE_CERT); // TEST��
            #endif
            return TRUE;
        }
    }
    else
    {
        SetWifiApStatus(STATUS_AP_BLE_CERT,CLEAR);
        SetWifiCertResult(RESULT_CERT_NOTUSED);
    }
    return FALSE;
}
static U8 CheckBleStatus2 ( void )
{
    if((GetWifiApStatus(STATUS_AP_BLE) != CLEAR) && (GetWifiApStatus(STATUS_AP_BLE_STATUS2_RESPONSE) != SET) ) 
    {
        return TRUE;
    }
    return FALSE;
}
static U8 CheckBleUart ( void )
{
    if ((GetWifiApStatus(STATUS_AP_BLE) != CLEAR) && (GetWifiApStatus(STATUS_AP_BLE_STATUS2_RESPONSE) != SET) )
    {
        return TRUE;
    }
    return FALSE;
}

static void DoReset ( void )
{
    SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA);
    HAL_SetWifiOnOff( RESET );
}

static void ServerCheckReset ( void )
{
    if(GetWifiApStatus(STATUS_AP_BLE_ALWAYS) != DONE) // ALWAYS SET ó�� (BLE���ON)
    {
        SetWifiApStatus(STATUS_AP_BLE_ALWAYS,SET);
    }

    SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA);
    HAL_SetWifiOnOff( RESET );
}

static void DoOff ( void )
{
    HAL_SetWifiOnOff( OFF );
}

static void DoApModeStart ( void )
{
    if ( GetWifiApStatus( STATUS_AP_WIFI ) == WAIT )
    {
        SetWifiApStatus( STATUS_AP_WIFI, RESET );
    }
    else if ( GetWifiApStatus( STATUS_AP_WIFI ) == RESET )
    {
        ClearWifiAllStatus();
        SetWifiApStatus( STATUS_AP_WIFI, START );
        SetWifiControlStatus( STATUS_CON_POWER, ON );
        SetWifiControlStatus( STATUS_CON_READY, ON );
        SetWifiIndicateStatus( STATUS_DISPLAY, WIFI_DISP_TRY );
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_APMODE );
    }
    else if ( GetWifiApStatus( STATUS_AP_WIFI ) == START )
    {
        SetWifiApStatus( STATUS_AP_WIFI, RETRY );
    }
    else
    {

    }
}

static void DoApBleStart ( void )
{
    if ( GetWifiApStatus( STATUS_AP_BLE ) == WAIT )
    {
        if(GetWifiApStatus(STATUS_AP_BLE_ALWAYS) != DONE) // �� Ȱ��ȭ ������ ��� BLE STATUS2 CMD ���� �� ALWAYS SET ó��
        {
            SetWifiApStatus(STATUS_AP_BLE_ALWAYS,SET);
        }

        SetWifiApStatus(STATUS_AP_BLE_STATUS2_RESPONSE,CLEAR);
        SetWifiApStatus( STATUS_AP_BLE, START );
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_BLE_ADV );
    }
}

static void DoApModeFail ( void )
{
#ifndef BLE_ALWAYS
    #if ( TEST_BLE_FCC == 0 )
    SetWifiApStatus(STATUS_AP_BLE_ALWAYS,CLEAR); // BLE ��� Ȱ��ȭ ����

    if ( GetWifiServerStatus( STATUS_SVR_CONNECT ) == SET )
    {
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_BLE_OFF );
        SetWifiIndicateStatus( STATUS_PARING, WIFI_AP_STEP_CLEAR );
        SetWifiBuzzSound( WIFI_BUZZER_BLE_OFF );
    }

    else if(WifiRx.u8DisplayConnectCount == DONE)  // BLE ���� Ȱ��ȭ �� 30�� ���� ��
    {
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_BLE_OFF );
        SetWifiIndicateStatus( STATUS_PARING, WIFI_AP_STEP_CLEAR );
        SetWifiIndicateStatus( STATUS_DISPLAY, WIFI_DISP_AP1 ); 
    }
    else
    {
        HAL_SetWifiOnOff( OFF );
        SetWifiBuzzSound( WIFI_BUZZER_AP_FAIL );
    }
    
    #endif
#endif

#ifdef BLE_ALWAYS
    if (GetWifiApStatus( STATUS_AP_WIFI ) != CLEAR ) // Wi-Fi �����(Soft AP) �� ��� OFF
    {
        HAL_SetWifiOnOff( RESET ); // Soft AP 30�� ��� �� BLE �ڵ� Ȱ��ȭ
        // HAL_SetWifiOnOff( OFF );
        // SetWifiBuzzSound( WIFI_BUZZER_AP_FAIL );
        SetWifiApStatus(STATUS_AP_BLE_ALWAYS,DONE); // BLE Ȱ��ȭ �Ϸ� (���ON) CMD��۽� �� ������� X
        SetWifiApStatus(STATUS_AP_BLE,WAIT); // BLE Ȱ��ȭ (���ON) --> OFF�Ǿ �ٽ� ON 
        WifiRx.u8DisplayConnectCount = DONE; // Soft AP��� 30�� ��� �� BLEȰ��ȭ �� ���� ���� �� BLE OFF �ϱ� ���� (�������� �� Clear)
    }
    else if (( GetWifiServerStatus( STATUS_SVR_CONNECT ) == CLEAR ) 
        && (GetWifiApStatus( STATUS_AP_BLE_CON ) == CLEAR ) )// BLE �� ��� �� 30�� ��� �� && ���� �̿��� �� && BLE STATUS 3 (���� ��)�� �ƴ� ���
    {
        SetWifiApStatus(STATUS_AP_BLE_ALWAYS,DONE); // BLE Ȱ��ȭ �Ϸ� (���ON) CMD��۽� �� ������� X
        SetWifiApStatus(STATUS_AP_BLE,WAIT); // BLE Ȱ��ȭ (���ON) --> OFF�Ǿ �ٽ� ON
        WifiRx.u8DisplayConnectCount = DONE; // ���� Ȱ��ȭ & 30�� ��� �� & ���� ���� �� =  BLE OFF �ϱ� ���� (�������� �� Clear)

        if ( GetWifiFotaStatus( STATUS_FOTA_MCU ) == SET ) // BLE FOTA EEPROM �ʱ�ȭ 
        {
            SetWifiFotaStatus( STATUS_FOTA_MCU, CLEAR );
            WifiWriteEep( EEPROM_ADDR_FOTA_MCU, CLEAR );
        }
    }
    #if ( TEST_BLE_FCC == 0 )
    else if((WifiRx.u8DisplayConnectCount == CLEAR ) 
        && (GetWifiServerStatus( STATUS_SVR_CONNECT ) == SET ) 
        && (GetWifiApStatus( STATUS_AP_BLE_CON ) == CLEAR ) )  // BLE ���� Ȱ��ȭ �� 30�� ���� �� && ���� ���� �� && Module <-> APP ���� ���� �� 
    {
        SetWifiApStatus(STATUS_AP_BLE_ALWAYS,CLEAR); // ���ON ���� OFF
        SetWifiSendStatus( TX_ATCMD, WIFI_AT_BLE_OFF ); // BLE OFF CMD
        SetWifiIndicateStatus( STATUS_PARING, WIFI_AP_STEP_CLEAR ); // BLE ǥ�� CLEAR
        SetWifiBuzzSound( WIFI_BUZZER_BLE_OFF ); // BLE OFF ������ ���
    }
    #endif
    else if(GetWifiApStatus( STATUS_AP_BLE_CON ) == SET) 
    {
        SetWifiApStatus(STATUS_AP_BLE_CON,CLEAR);        // BLE STATUS 3 (���� ��)���·� 30�� ���(30�� Ÿ�̸� �簻��) �� ��� CLEAR , ���� ���� ���� Check �� ���� 3�� �� Reset
    }
#endif

}

static void DoFirstData ( void )
{
    WifiStartProcess();
}

static void DoFirstFail ( void )
{
    SetWifiIniStatus( STATUS_INI_SEQ, INI_SEQ_SERVER_CONNECT );
}

static void DoSendData ( void )
{
    WifiTxDataSelect();
}

static void DoSend1min ( void )
{
    SetWifiSendStatus( TX_DATA, WIFI_TX_SEND_SENSOR );
    // SetWifiTxStatus( STATUS_TX_SENSOR_STATE, SET );
}
static void DoSend5min ( void )
{
    SetWifiSendStatus( TX_DATA, WIFI_TX_SEND_SENSOR );
    // SetWifiTxStatus( STATUS_TX_SENSOR_STATE, SET );
    if ((GetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE) == FALSE) // F1000 / SAP001 �� ��� 5�� ���� Retry
        && (GetWifiSapStatus(STATUS_SVR_SAP_0031_SUPPLIES) == FALSE)
        && (GetWifiSapStatus(STATUS_SVR_SAP_0032_ENVIRONMENT) == FALSE))
        {
            GetWifiRequestSAP();
        }
}
static void DoRqst20min ( void )
{
#if (USER_USE_A2010_INFO == 1)
#ifdef USER_USE_WEATHER
    if ( GetWifiIniStatus( STATUS_INI_INFO ) == SET )
    {
        SetWifiSendStatus( TX_DATA, WIFI_TX_RQST_INFO_DATA );
    }
    else
    {
        SetWifiSendStatus( TX_DATA, WIFI_TX_RQST_INFO );
    }
#endif

#else // #ifdef

#endif // #if
}

static void DoRqst24Hour ( void )
{
    SetWifiSendStatus( TX_DATA, WIFI_TX_RQST_INFO );
#if (USER_USE_A2021_SAPINFO == 1)
    SetClearSAPCode();
    SetWifiSendStatus( TX_DATA, WIFI_TX_RQST_INFO_JSON );
#endif
}

static void BleTxOFF ( void )
{
    SetWifiBLESendStatus(TX_BLE_WIFI, WIFIDATA);
}

static void DoFotaStart ( void )
{
    WifiFotaProcess();
}

static void DoWifiIndicator ( void )
{
    SetWifiIndicateStatus( STATUS_DISPLAY, WIFI_DISP_AP1 ); // ����������� ǥ��
    SetWifiApStatus(STATUS_AP_BLE,WAIT); // BLE �� ��� ����
    WifiRx.u8DisplayConnectCount = DONE; // ����������� 1�� ī��Ʈ Ŭ���� (����� ���� �Ϸ�)

    if ( GetWifiFotaStatus( STATUS_FOTA_MCU ) == SET ) // BLE FOTA EEPROM �ʱ�ȭ 
        {
            SetWifiFotaStatus( STATUS_FOTA_MCU, CLEAR );
            WifiWriteEep( EEPROM_ADDR_FOTA_MCU, CLEAR );
        }
}
static void DoCertCancel ( void )
{
    SetWifiCertID(WIFI_BLE_CERT_0003);
    SetWifiCertResult(RESULT_CERT_TIME_OUT);
    SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_BLE_CERT);
    SetWifiApStatus(STATUS_AP_BLE_CERT,CLEAR);      // �������� ���� �Ұ� ���·� ����

}
static void BleStatus2Retry ( void ) // 10�� ���� Retry (BLE ON CMD ���� ���� ��)
{
    SetWifiApStatus(STATUS_AP_BLE_ALWAYS,DONE); // BLE Ȱ��ȭ �Ϸ� (���ON) CMD��۽� �� ������� X
    SetWifiApStatus(STATUS_AP_BLE,WAIT); // BLE �� ��� ����
}


