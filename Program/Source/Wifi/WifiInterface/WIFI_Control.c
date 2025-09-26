
/*******************************************************************************
&Description :
  제품에서 사용하는 WIFI 함수 File
  
&History : 
  ShinHM 2021.07.09 update
  ssg@coway.com 2023.05.30 update
  - 점유인증 추가
  - Fix RAM DATA 처리 추가
  - A1014 DATA 처리 추가
  ssg@coway.com 2024.05.29 update
  - BLE 상시 ON 기능 추가
*******************************************************************************/


/***** Header *****************************************************************/
#include "WIFI_Common.h"
#include "WIFI_SetCommon.h"
#include "WIFI_DefUser.h"
#include "WIFI_Fota.h"
#include "WIFI_DataCheck.h"
#include "WIFI_RxProcess.h"
#include "WIFI_TxProtocol.h"
#include "WIFI_RxServer.h"
#include "WIFI_Status.h"
#include "WIFI_UartInterrupt.h"
#include "WIFI_UserInterface.h"
#include "WIFI_MonitorFixRAM.h"
#include "WIFI_Control.h"


/***** Define *****************************************************************/


/***** Variable ***************************************************************/
extern WifiRx_T WifiRx;
// extern U8 u8_TimerPossibleWifiBleCertCancel = CLEAR;
/***** Constant ***************************************************************/


/***** Static Function ********************************************************/
static void InitializeWifiAllValue ( void );
void IniEasyPairing ( void );
static void WifiDualApModeKey ( void );
static void WifiApModekey ( void );
static void WifiApBLEkey ( void );
static void WifiOnOffKey ( void );
static void WifiBleCertKey ( void );
static void WifiBleCertCancelKey ( void );
static void WifiTestKey ( void );
static void WifiOnKey ( void );
static void WifiOffKey ( void );


/***** Table ******************************************************************/


/***** Global Function ********************************************************/
void WifiControlProcess ( E_WIFI_TIME_T mu8Time )
{
    static U8 mu8Counter1s=0; 						    /*	1s COUNTER 				*/
	static U8 mu8Counter1min=0; 						/*	1min COUNTER			*/
	static U16 mu16A1080Period=1; 						/*	1H Default              */    
	static U8 mu8A1080Counter=0; 						/*	COUNTER			        */
    if( mu8Time == WIFI_TIME_INI )
    {
        InitializeWifiAllValue();
    }
    else if ( mu8Time == WIFI_TIME_INI_EASY_PAIRING) 
    {
        IniEasyPairing();
    }
    else if ( mu8Time == WIFI_TIME_SET ) 
    {
        WifiOnOffSetting();
    }
    else if ( mu8Time == WIFI_TIME_FACTORY ) 
    {
        WifiFactorySetting();
    }
    else if ( mu8Time == WIFI_TIME_WHILE )
    {
        WifiRxDataProcess();
        ReceiveWifiUart();
    }
    else if ( mu8Time == WIFI_TIME_1MIN )
    {
        WifiDataCheckProcess(mu8Time);
    }
    else if ( mu8Time == WIFI_TIME_1SEC )
    {
        WifiDataCheckProcess(mu8Time);
        // SystemFunctionEventCheck();
    }
    else if ( mu8Time == WIFI_TIME_100MS )
    {
        WifiDataCheckProcess(mu8Time);
        mu8Counter1s++; 							/*		1s COUNTER +1			*/
		if (mu8Counter1s >= 10)					    /*		1s 경과한 경우			*/
		{
			mu8Counter1s = 0; 						/*			1s COUNTER CLR		*/
            WifiDataCheckProcess(WIFI_TIME_1SEC);
            SystemFunctionEventCheck();
            mu8Counter1min++; 							/*		1M COUNTER +1			*/
            if (mu8Counter1min >= 60)					/*		1M 경과한 경우			*/
            {
                mu8Counter1min = 0;                     
                WifiDataCheckProcess(WIFI_TIME_1MIN);

                /* 상시진단 주기 계산 */
                if (GetWifiServerStatus(STATUS_SVR_CONNECT) == SET)
                {
                    mu16A1080Period = GetWifiRequestValue(WIFI_RQST_PART_PERIOD);	
                    mu8A1080Counter++;
                    if (mu16A1080Period <= 0)
                    {
                        mu16A1080Period = 1;
                    }
                    if (mu16A1080Period > 0)
                    {
                        if (mu8A1080Counter >= (mu16A1080Period*60))   /* Unit : 1Min */
                        {
                            WifiSendData(WIFI_DATA_PART);
                            mu8A1080Counter = 0;
                        }
                    }
                }
                else
                {
                    mu8A1080Counter = 0;
                }
            }
        }
    }
}

void WifiControlProcess100ms ( void )
{
    WifiControlProcess(WIFI_TIME_100MS);
}

void WifiKey ( E_WIFI_KEY_T mu8Key )
{
    if ( mu8Key == WIFI_KEY_AP )
    {
        WifiApModekey();
    }
    else if ( mu8Key == WIFI_KEY_BLE )
    {
        WifiApBLEkey();
    }
    else if ( mu8Key == WIFI_KEY_ON )
    {
        WifiOnKey();
    }
    else if ( mu8Key == WIFI_KEY_OFF )
    {
        WifiOffKey();
        SetWifiBuzzSound(WIFI_BUZZER_CANCEL);
    }
    else if ( mu8Key == WIFI_KEY_ONOFF )
    {
        WifiOnOffKey();
    }
    else if ( mu8Key == WIFI_KEY_TEST )
    {
        WifiTestKey();
    }
    else if ( mu8Key == WIFI_KEY_BLE_CERT )
    {
        WifiBleCertKey();
    }
    else if ( mu8Key == WIFI_KEY_BLE_CERT_CANCEL )
    {
        WifiBleCertCancelKey();
    }
    else
    {
        
    }
}

void WifiSendData ( E_WIFI_DATA_T mu8Data )
{
    // if ( GetWifiServerStatus(STATUS_SVR_CONNECT) != SET )
    if (( GetWifiServerStatus(STATUS_SVR_CONNECT) != SET ) && ( GetWifiBLESendStatus(TX_BLE_WIFI) == WIFIDATA ))
    { 
        return;
    }
    
    if ( mu8Data == WIFI_DATA_FUNCTION )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_FUNC);
    }
    if ( mu8Data == WIFI_DATA_FUNCTION_EVENT )
    {
        if (GetWifiSendStatusAck() == TRUE)
        {
            SetWifiSendStatus(TX_DATA,WIFI_TX_RESP_RECV);
        }
        else
        {
            SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_FUNC);
        }
    }
    else if ( mu8Data == WIFI_DATA_SENSOR )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_SENSOR);
    }
    else if ( mu8Data == WIFI_DATA_ERROR )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_ERR);
    }
    else if ( mu8Data == WIFI_DATA_ERROR_EVENT )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_ERR);
        if ( GetUserInterface( USER_SMART_SENSING_STATUS ) != TRUE )
        {
            SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_PART);
        }
    }
    else if ( mu8Data == WIFI_DATA_PART )
    {
        if ( GetUserInterface( USER_SMART_SENSING_STATUS ) != TRUE )
        {
            SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_PART);
        }
    }
    else if ( mu8Data == WIFI_DATA_EXAMINE )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_EXCEPT_FUNC);
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_EXAMINE);
    }
    else if ( mu8Data == WIFI_DATA_INFO )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_RQST_INFO_USER);
    }
    // add
    else if ( mu8Data == WIFI_DATA_ACCUMULATE )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_ACCUMULATE);
    }
    else if ( mu8Data == WIFI_DATA_PARA )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_PARA);
    }
}

void WifiSendDataControl ( E_WIFI_DATA_T mu8Data, U16 mu16IdData )
{
    if ( GetWifiServerStatus(STATUS_SVR_CONNECT) != SET )
    { 
        return;
    }
    
    if ( mu8Data == WIFI_DATA_FUNCTION_CONTROL )
    {
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_FUNC_CONTROL);
        SetWifiSendStatusControl(mu16IdData);

    }
}

U8 GetWifiStatusValue ( E_WIFI_STATUS_T mType )
{
    U8 mValue = 0U;
    U8 mApStep = 0U;

    if ( mType == WIFI_STATUS_DISP ) /* WIFI Display */
    {
        mApStep = GetWifiIndicateStatus(STATUS_PARING);
        if ( GetWifiApStatus(STATUS_AP_WIFI) != CLEAR )
        {
            if ( mApStep == WIFI_AP_STEP_WAIT )
            {
                mValue = WIFI_DISP_AP_STEP0;
            }
            else if ( mApStep == WIFI_AP_STEP_START )
            {
                mValue = WIFI_DISP_AP_STEP1;
            }
            else if ( mApStep == WIFI_AP_STEP_MODEM )
            {
                mValue = WIFI_DISP_AP_STEP2;
            }
            else if ( mApStep == WIFI_AP_STEP_ROOTCA )
            {
                mValue = WIFI_DISP_AP_STEP3;
            }
            else
            {
                mValue = WIFI_DISP_AP_START;
            }
        }
        else if ( GetWifiApStatus(STATUS_AP_BLE) != CLEAR )
        {
            if ( mApStep == WIFI_AP_STEP_WAIT )
            {
                mValue = WIFI_DISP_BLE_STEP0;
            }
            else if ( mApStep == WIFI_AP_STEP_START )
            {
                mValue = WIFI_DISP_BLE_STEP1;
            }
            else if ( mApStep == WIFI_AP_STEP_MODEM )
            {
                mValue = WIFI_DISP_BLE_STEP2;
            }
            else if ( mApStep == WIFI_AP_STEP_ROOTCA )
            {
                mValue = WIFI_DISP_BLE_STEP3;
            }
            else
            {
                mValue = WIFI_DISP_BLE_START;
            }
        }
        else
        {
            mValue = GetWifiIndicateStatus(STATUS_DISPLAY);    
        }
    }
    else if ( mType == WIFI_STATUS_POWER ) 
    {
        mValue = GetWifiControlStatus(STATUS_CON_POWER);
    }
    else if ( mType == WIFI_STATUS_TEST ) 
    {
        mValue = GetWifiIndicateStatus(STATUS_FACTORY);
    }
    else if ( mType == WIFI_STATUS_ERROR ) /* WIFI Error */
    {
        mValue = GetWifiIndicateStatus(STATUS_ERROR);
    }
    else if ( mType == WIFI_STATUS_AP_FIRST )
    {
        mValue = GetWifiApStatus(STATUS_AP_FIRST);
    }
    else if ( mType == WIFI_STATUS_AP_MODE ) /* AP MODE  (DISPLAY) */
    {
        if ( GetWifiApStatus(STATUS_AP_WIFI) != CLEAR )
        {
            mValue = SET;
        }
    }
    else if ( mType == WIFI_STATUS_AP_BLE ) /* BLE AP MODE(DISPLAY) */
    {
        if ( GetWifiApStatus(STATUS_AP_BLE) != CLEAR )
        {
            mValue = SET;
        }
    }
    else if ( mType == WIFI_STATUS_BLE_CERT ) /* BLE CERT MODE (0:Unauthenticated status, 1: Authenticateable Status, 2:Authentication completion status ) */
    {
        if (GetWifiApStatus(STATUS_AP_BLE) != CLEAR)  // BLE가 활성화 되어 있는 경우
        {
            if ( GetWifiApStatus(STATUS_AP_BLE_CERT) == SET )
            {
                mValue = SET;
            }
            else if (GetWifiApStatus(STATUS_AP_BLE_CERT) == DONE )
            {
                mValue = DONE;
            }
            else
            {

            }
        }
        else
        {

        }
    }
    else if ( mType == WIFI_STATUS_AP_STEP ) /* 페어링 스텝 확인 (DISPLAY) */
    {
        mValue = GetWifiIndicateStatus(STATUS_PARING);
    }  
    else if ( mType == WIFI_STATUS_AP_ERROR ) /* WIFI AP Mode Error */
    {
        if ( GetWifiIndicateStatus(STATUS_PARING) >= WIFI_AP_STEP_START )
        {
            mValue = GetWifiIndicateStatus(STATUS_AP_ERROR);
        }
    }
    else if ( mType == WIFI_STATUS_SERVER_CON ) 
    {
        // mValue = GetWifiServerStatus(STATUS_SVR_CONNECT);

        /* ???? ? 1??? ?? ? ?? ?? ?? */
        if ((GetWifiIndicateStatus(STATUS_DISPLAY) >= WIFI_DISP_AP1) // 
            && (GetWifiIndicateStatus(STATUS_DISPLAY) <= WIFI_DISP_TRY))
        {
            if (GetWifiServerStatus(STATUS_SVR_CONNECT) != SET)
            {
                mValue = 0U;
            }
        }
        else if ((GetWifiIndicateStatus(STATUS_DISPLAY) == WIFI_DISP_CONNECT) || (GetWifiServerStatus(STATUS_SVR_CONNECT) == SET))
        {
            mValue = 1U;
        }
        else
        {
            mValue = 0U;
        }
    }
    else if ( mType == WIFI_STATUS_PAIRING ) 
    {
        if(GetWifiApStatus(STATUS_AP_BLE) != CLEAR)
        {
            mValue = 1U;
        }
        else if (GetWifiApStatus(STATUS_AP_WIFI) != CLEAR)
        {
            mValue = 2U;
        }
        else
        {
            mValue = 0U;
        }
        
    }
    else if ( mType == WIFI_STATUS_LOCATION ) 
    {
        if ((GetWifiInfoData(WIFI_RQST_WEATHER) != 0U)
            && (GetWifiInfoData(WIFI_RQST_AQI) != 0U))
        {
            mValue = 1U;
        }
        else
        {
            mValue = 0U;
        }
    }
    else if ( mType == WIFI_STATUS_SAP_CODE_0030_UV_DATE ) 
    {
        mValue = GetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE);
    }
    else if ( mType == WIFI_STATUS_SAP_CODE_0031_SUPPLIES ) 
    {
        mValue = GetWifiSapStatus(STATUS_SVR_SAP_0031_SUPPLIES);
    }
    else if ( mType == WIFI_STATUS_SAP_CODE_0032_ENVIRONMENT ) 
    {
        mValue = GetWifiSapStatus(STATUS_SVR_SAP_0032_ENVIRONMENT);
    }
    else if ( mType == WIFI_STATUS_FOTA ) 
    {
        if (GetWifiFotaStatus(STATUS_FOTA_SEQ) >= FOTA_CHECK)
        {
            mValue = 1U;
        }
    }
    
    return mValue;
}

U16 GetWifiRequestValue ( E_WIFI_RQST_T mId )
{
    U16 mValue = 0;
    mValue = GetWifiInfoData(mId);
    return mValue;
}
void GetWifiRequestSAP ( void )
{
#if (USER_USE_A2021_SAPINFO == 1)
    SetClearSAPCode();
    SetWifiSendStatus( TX_DATA, WIFI_TX_RQST_INFO_JSON );
#endif
}

U8 GetWifiRequestApStatus ( E_WIFI_STATUS_AP_T mId )
{
    return GetWifiApStatus(mId);
}

void SetClearSAPCode (void)
{
    SetWifiSapStatus(STATUS_SVR_SAP_0030_UV_DATE, CLEAR);
    SetWifiSapStatus(STATUS_SVR_SAP_0031_SUPPLIES, CLEAR);
    SetWifiSapStatus(STATUS_SVR_SAP_0032_ENVIRONMENT, CLEAR);
}

void InterruptUartWifiTx ( void )
{
    IntWifiUartTx();
}

void InterruptUartWifiRx ( void ) //U8 mu8RxData )
{
    IntWifiUartRx();//mu8RxData);
}

void InterruptUartWifiError ( void )
{
    U8 rx_data = 0;
    volatile U8 err_type;

    rx_data = R_RXD;
    err_type = (U8)(SSR03 & 0x0007U);
    SIR03 = (U16)err_type;
}


/***** Local Function *********************************************************/
static void InitializeWifiAllValue ( void )
{
    InitializeWifiFota();
    InitializeWifiStatus();
    InitializeWifiRxProcess();
    InitializeWifiRxServer();
    InitializeWifiTxProtocol();
    InitializeWifiUart();
    InitializeWifiUserInterface();
    
}
void IniEasyPairing ( void )
{
    U8 mu8EepData = 0U;
    mu8EepData = WifiReadEep(EEPROM_ADDR_FIRST_PARING);
    
    if (mu8EepData != SET )
    {
        if ( GetWifiControlStatus(STATUS_CON_POWER) != ON )
        {
            HAL_SetWifiOnOff(ON);
        }
        SetWifiApStatus(STATUS_AP_BLE,WAIT);
        WifiRx.u8DisplayConnectCount = DONE; // BLE 자동활성화 (초기페어링)
    }
}

static void WifiDualApModeKey ( void )
{
    static U8 mu8Dual = 0;
    /* BLE -> Off -> WiFi -> Off -> BLE... repeat */
    if ( GetWifiControlStatus(STATUS_CON_POWER) != ON )
    {
        if ( mu8Dual  == CLEAR )
        {
            mu8Dual = SET;
        }
        else
        {
            mu8Dual = CLEAR;
        }
    }

    if ( mu8Dual == SET )
    {
        WifiApModekey();
    }
    else
    {
        WifiApBLEkey();
    }

}

// WIFI AP MODE KEY
static void WifiApModekey ( void )
{
    // if ( ( GetWifiApStatus(STATUS_AP_WIFI) != CLEAR ) || GetWifiApStatus(STATUS_AP_BLE) != CLEAR )
    if (GetWifiApStatus(STATUS_AP_WIFI) != CLEAR )
    { 
        SetWifiApStatus(STATUS_AP_WIFI, CLEAR);
        SetWifiApStatus(STATUS_AP_BLE, CLEAR);
        WifiOffKey();
        SetWifiBuzzSound(WIFI_BUZZER_WiFi_OFF);
        return;
    }    
    else if (GetWifiApStatus(STATUS_AP_BLE) != CLEAR )
    {
        // HAL_SetWifiOnOff(RESET); //
        SetWifiBuzzSound(WIFI_BUZZER_ERROR);
        return;
    }

    if ( GetWifiControlStatus(STATUS_CON_POWER) != ON )
    {
        HAL_SetWifiOnOff(ON);
        SetWifiApStatus(STATUS_AP_WIFI,RESET);
    }
    else
    {
        if ( GetWifiServerStatus(STATUS_SVR_CONNECT) == SET )
        { 
            SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_STOP);
            SetWifiApStatus(STATUS_AP_WIFI,WAIT);
        }
        else
        {
            SetWifiApStatus(STATUS_AP_WIFI,RESET);
        }
    }
    ClearRxStatus();
    SetWifiApStatus(STATUS_AP_BLE_ALWAYS,CLEAR); // Soft AP KEY 입력 시 서버연결 시 상시ON 상태 OFF
    SetWifiIndicateStatus(STATUS_AP_ERROR,WIFI_DISP_OFF);
    SetWifiBuzzSound(WIFI_BUZZER_AP_START);
}

static void WifiApBLEkey ( void )
{
    
    if(GetWifiApStatus(STATUS_AP_BLE_CERT) == SET) // 점유인증 가능 (조작 대기) 상태인 경우
    {
        WifiBleCertKey();
        return;
    }
    else if (GetWifiApStatus(STATUS_AP_BLE_CERT) == DONE) 
    {
        // return; // 삭제
    }

    if (GetWifiApStatus(STATUS_AP_BLE) != CLEAR )
    { 
        SetWifiApStatus(STATUS_AP_BLE, CLEAR);
        SetWifiApStatus(STATUS_AP_BLE_ALWAYS,CLEAR); // BLE OFF KEY 입력 시 서버연결 시 상시ON 상태 OFF
        if ( GetWifiServerStatus(STATUS_SVR_CONNECT) == SET )
        {
            SetWifiSendStatus(TX_ATCMD,WIFI_AT_BLE_OFF);
            SetWifiIndicateStatus(STATUS_PARING,WIFI_AP_STEP_CLEAR);
            SetWifiBuzzSound(WIFI_BUZZER_BLE_OFF);
        }
        else
        {
            // WifiOffKey();
            // SetWifiBuzzSound(WIFI_BUZZER_BLE_OFF);

            SetWifiSendStatus(TX_ATCMD,WIFI_AT_BLE_OFF);
            SetWifiIndicateStatus(STATUS_PARING,WIFI_AP_STEP_CLEAR);
            SetWifiBuzzSound(WIFI_BUZZER_BLE_OFF);
        }
        return;
    }    
    else if ( GetWifiApStatus(STATUS_AP_WIFI) != CLEAR )
    {
        // HAL_SetWifiOnOff(RESET);
        SetWifiBuzzSound(WIFI_BUZZER_ERROR);
        return;
    }
    
    if ( GetWifiControlStatus(STATUS_CON_POWER) != ON )
    {
        HAL_SetWifiOnOff(ON);
    }
    else
    {
        //SetWifiSendStatus(TX_ATCMD,WIFI_AT_BLE_ADV);
    }

    // BLE ON 조작 시 WiFi Power ON
    if (WifiReadEep(EEPROM_ADDR_WIFI_POWER) != SET)
    { 
        WifiWriteEep( EEPROM_ADDR_WIFI_POWER, SET );
    }
    
    SetWifiApStatus(STATUS_AP_BLE,WAIT);
    SetWifiBuzzSound(WIFI_BUZZER_AP_START);
    WifiRx.u8DisplayConnectCount = CLEAR;
}

// WIFI ON/OFF KEY
static void WifiOnOffKey ( void )
{
    if ( GetWifiControlStatus(STATUS_CON_POWER) != ON )
    {
        WifiOnKey();
    }
    else
    {
        WifiOffKey();
        SetWifiBuzzSound(WIFI_BUZZER_CANCEL);
    }
}

// BLE CERT KEY
static void WifiBleCertKey ( void )
{
    if ((GetWifiStatusValue(WIFI_STATUS_AP_BLE) == SET) && (GetWifiApStatus(STATUS_AP_BLE_CERT) == SET)) // BLE활성화 && 점유인증 가능상태
    {
        // Send Authentication completed
        SetWifiCertID(WIFI_BLE_CERT_0003);
        SetWifiCertResult(RESULT_CERT_SUCCESS);
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_BLE_CERT);
        
        SetWifiApStatus(STATUS_AP_BLE_CERT,DONE);
        SetWifiBuzzSound(WIFI_BUZZER_SELECT);
    }

}

// BLE CERT CANCEL KEY
static void WifiBleCertCancelKey ( void )
{
    // Send cancellation of authentication
    if ((GetWifiStatusValue(WIFI_STATUS_AP_BLE) == SET) && (GetWifiApStatus(STATUS_AP_BLE_CERT) == SET)) // BLE활성화 && 점유인증 가능상태 
    {
        // Send Authentication cancel
        SetWifiCertID(WIFI_BLE_CERT_0003);
        SetWifiCertResult(RESULT_CERT_CANCEL);
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_BLE_CERT);

        SetWifiApStatus(STATUS_AP_BLE_CERT,CLEAR);
        SetWifiBuzzSound(WIFI_BUZZER_SELECT);
    }
    else if (GetWifiApStatus(STATUS_AP_BLE_CERT) != SET)
    {
        SetWifiBuzzSound(WIFI_BUZZER_ERROR);
    }
}


// Test Key Operation
static void WifiTestKey ( void )
{
    HAL_SetWifiOnOff(ON);
    SetWifiBuzzSound(WIFI_BUZZER_SELECT);
}

// On Key Operation
static void WifiOnKey ( void )
{
    if ( GetWifiControlStatus(STATUS_CON_POWER) == ON )
    {
        SetWifiBuzzSound(WIFI_BUZZER_ERROR);
        return;
    }
    
    if ( ( GetUserInterface(USER_FACTORY_STATUS) == SET ) || ( GetWifiApStatus(STATUS_AP_FIRST) == SET ) )
    {
        HAL_SetWifiOnOff(ON);
        WifiWriteEep(EEPROM_ADDR_WIFI_POWER, ON);
        SetWifiBuzzSound(WIFI_BUZZER_SETUP);
    }
    else
    {
        SetWifiBuzzSound(WIFI_BUZZER_ERROR);
    }
    ClearRxStatus();
}

// Off Key Operation
static void WifiOffKey ( void )
{
    if ( GetWifiControlStatus(STATUS_CON_POWER) == OFF )
    {
        SetWifiBuzzSound(WIFI_BUZZER_ERROR);
        return;
    }
    
    if ( GetWifiServerStatus(STATUS_SVR_CONNECT) == SET )
    { 
        SetWifiServerStatus(STATUS_SVR_END,SET);
        SetWifiSendStatus(TX_DATA,WIFI_TX_SEND_STOP);
        SetWifiIndicateStatus(STATUS_DISPLAY,WIFI_DISP_OFF);
    }
    else
    {
        HAL_SetWifiOnOff(OFF);
    }
    // SetWifiBuzzSound(WIFI_BUZZER_CANCEL);
    ClearRxStatus();
    SetWifiApStatus(STATUS_AP_BLE_ALWAYS,CLEAR); // 상시ON 상태 OFF
}


void WifiTimerControl(void)
{
    // if ( GetWifiApStatus(STATUS_AP_BLE_CERT) != DONE) // 점유인증 완료 상태
    // {
        // u8_TimerPossibleWifiBleCertCancel = ZT_BleCertCancelTime; // 7초 타이머 셋 (다운타이머)
    // }
}

void SetWifiSystemStatus (E_WIFI_USER_VALUE_T mStatus, U8 mVal )
{
    SetUserSystemStatus(mStatus, mVal);
}

