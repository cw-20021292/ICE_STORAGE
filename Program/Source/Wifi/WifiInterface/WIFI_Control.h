
#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_

#include "WIFI_DefUser.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_Status.h"

/***** <User Header> **********************************************************/
/* User �� �Ʒ� 1���� Header �� ���� 
#include "WIFI_Control.h"
*/

/***** <User Function> ********************************************************/
/*
<1> ���� �� �ֱ��Լ�
WifiControlProcess(WIFI_TIME_INI) : �ʱ�ȭ (EEPROM ����) 
WifiControlProcess(WIFI_TIME_SET) : �ʱ���� (EEPROM ���� While ����)
WifiControlProcess(WIFI_TIME_FACTORY) : ������ / �����׽�Ʈ �� �ʱ�ȭ
WifiControlProcess(WIFI_TIME_WHILE) : while 
WifiControlProcess(WIFI_TIME_100MS) : 100ms �ֱ�
WifiControlProcess(WIFI_TIME_1SEC) : 1sec �ֱ�
WifiControlProcess(WIFI_TIME_1MIN) : 1min �ֱ�
WifiControlProcess(WIFI_TIME_INI_EASY_PAIRING) : �� ���� ���� �� ������(BLEȰ��ȭ) ����
*/
void WifiControlProcess ( E_WIFI_TIME_T mu8Time );
void WifiControlProcess100ms ( void );

/*
<2> Wifi Key
WifiKey(WIFI_KEY_AP) : Wifi Pairing (�ü�+���� 3��)
WifiKey(WIFI_KEY_BLE) : BLE Pairing (���� 3��)
WifiKey(WIFI_KEY_ON)
WifiKey(WIFI_KEY_OFF)
WifiKey(WIFI_KEY_ONOFF) : Power On/Off (�ü�+�뷮 3��)
WifiKey(WIFI_KEY_TEST) : Factory Test 

WifiKey(WIFI_KEY_BLE_CERT) : �������� Ű (BLE �������� ������ + ����3��)
WifiKey(WIFI_KEY_BLE_CERT_CANCEL) : �������� ��� (����7��) --> �̻��

*/
void WifiKey ( E_WIFI_KEY_T mu8Key );

/*
<3> Wifi Send Data : �������� ����
WifiSendData(WIFI_DATA_FUNCTION) : ��ɵ����� ���� �� (KEY�Է�/����Ϸ�/���͸���/��ɺ���/�������ܰ���...)
WifiSendData(WIFI_DATA_ERROR) : ���������� ���� �� (�߻�/����)
WifiSendData(WIFI_DATA_PART) : ��� �������� ������ (�ֱ�)
WifiSendData(WIFI_DATA_EXAMINE) : ���� �������� �Ϸ� �� (�̺�Ʈ)
*/
void WifiSendData ( E_WIFI_DATA_T mu8Data );
/*
WifiSendDataControl(WIFI_DATA_FUNCTION_CONTROL, ID) : ��ǰ KEY���� �� (��ǰ ���� �����ʹ� �������� ����)
*/
void WifiSendDataControl ( E_WIFI_DATA_T mu8Data, U16 mu16IdData);
/*
<4> Wifi Get data 
GetWifiStatusValue(WIFI_STATUS_DISP) : WIFI DISPLAY (E_WIFI_DISPLAY_T)
    // Wi-Fi OFF [�ҵ�]
    WIFI_DISP_OFF:          

    // Soft AP Pairing MODE [�� ����]
    WIFI_DISP_AP_START:     // Soft AP MODE START     : WiFi �� ��� ����
    WIFI_DISP_AP_STEP0:     // Pairing Mode ing       : Soft AP �� ��� Ȱ��ȭ �Ϸ�
    WIFI_DISP_AP_STEP1:     // APP Pairing START      : APP�� �� ���� (25%)
    WIFI_DISP_AP_STEP2:     // AP Connect             : AP����Ϸ� (50%)
    WIFI_DISP_AP_STEP3:     // AUTHENTICATE           : �������� Ȯ�� �� (75&)

    // BLE Pairing MODE [�� ����]
    WIFI_DISP_BLE_START:    // BLE MODE START         : BLE �� ��� ����
    WIFI_DISP_BLE_STEP0:    // Pairing Mode ing       : Soft AP �� ��� Ȱ��ȭ �Ϸ�
    WIFI_DISP_BLE_STEP1:    // APP Pairing START      : APP�� �� ���� (25%)
    WIFI_DISP_BLE_STEP2:    // AP Connect             : AP����Ϸ� (50%)
    WIFI_DISP_BLE_STEP3:    // AUTHENTICATE           : �������� Ȯ�� �� (75%)

    // Server ����Ϸ� [����]
    WIFI_DISP_CONNECT:      // ���� ����Ϸ�          : (100%)

    // Wi-Fi ON & ���� ������� [����]
    WIFI_DISP_AP1:          
    WIFI_DISP_AP2:          
    WIFI_DISP_AP3:          
    WIFI_DISP_AP4:          
    WIFI_DISP_AP5:          
    WIFI_DISP_TRY:          

GetWifiStatusValue(WIFI_STATUS_POWER) : ���� Ȯ�� (0:Off/1:On)
GetWifiStatusValue(WIFI_STATUS_TEST) : TestMode ���� ����Ȯ�� (0:Disconnected/1:Connected)
GetWifiStatusValue(WIFI_STATUS_ERROR) : Wifi Error (0:Normal/1~5:Err) (�� ���� �� üũ �� Error)
GetWifiStatusValue(WIFI_STATUS_AP_ERROR) : AP Error (0:Normal/1~5:Err) (���� üũ �� Error)
GetWifiStatusValue(WIFI_STATUS_AP_FIRST) : (0:���ʿ���X /1:���ʿ���Ϸ�)
GetWifiStatusValue(WIFI_STATUS_AP_MODE) : (0:Normal/1:ApMode)
GetWifiStatusValue(WIFI_STATUS_AP_BLE) : (0:Normal/1:BLEApMode)
GetWifiStatusValue(WIFI_STATUS_BLE_CERT) : (0:���������Ұ�����/1:������������ ����/2:���������Ϸ����)  // �������� ���� �߰�!

GetWifiStatusValue(WIFI_STATUS_SERVER_CON) : 0  CLEAR  / 1 SET (������) 
GetWifiStatusValue(WIFI_STATUS_PAIRING) :   0  CLEAR  / 1 : BLEȰ��ȭ / 2: Wi-Fi ������ Ȱ��ȭ 
GetWifiStatusValue(WIFI_STATUS_LOCATION) :  0 APP���� ��ġ���� ���� �ȵ� / 1 : APP���� ��ġ���� ���� �Ϸ�
GetWifiStatusValue(WIFI_STATUS_FOTA) :  0  / 1 : FOTA ��
GetWifiStatusValue(WIFI_STATUS_SAP_CODE_0030_UV_DATE) :  SAP �ΰ����� DATA ���ſ��� -> Return :(1: S1000 / 2 : DATA ���� / 0: ����)
GetWifiStatusValue(WIFI_STATUS_SAP_CODE_0031_SUPPLIES) :  SAP �ΰ����� DATA ���ſ��� -> Return :(1: S1000 / 2 : DATA ���� / 0: ����)
GetWifiStatusValue(WIFI_STATUS_SAP_CODE_0032_ENVIRONMENT) :  SAP �ΰ����� DATA ���ſ��� -> Return :(1: S1000 / 2 : DATA ���� / 0: ����)

*/
U8 GetWifiStatusValue ( E_WIFI_STATUS_T mType );

/*
<5> Wifi Get Request Info ��û�� ����
GetWifiRequestValue(WIFI_RQST_WEATHER) : ���������� (1 ~ 12)
GetWifiRequestValue(WIFI_RQST_AQI) : AQI (1 ~ 4)
GetWifiRequestValue(WIFI_RQST_CURRENT_TEMP) : ����µ� (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_HIGH_TEMP) : �ְ�µ� (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_LOW_TEMP) : �����µ� (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY1) : ����1 ��۰���� (0xFFFF �� ��쿡�� �湮���� / ������� ���°�� 9999 )
GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY2) : ����2 ��۰���� (0xFFFF �� ��쿡�� �湮���� / ������� ���°�� 9999 )
GetWifiRequestValue(WIFI_RQST_FILTER_CHANGE_CYCLE) : ���ͱ�ȯ �ֱ�(%) (15Month(�⺻) X 100%(�⺻), 1~999 %)
GetWifiRequestValue(WIFI_RQST_PART_PERIOD) : ������� �ֱ� ���� (EX : 1 --> 1�ð�)

GetWifiRequestValue(WIFI_RQST_YEAR) : �� (2024)
GetWifiRequestValue(WIFI_RQST_MONTH) : �� 
GetWifiRequestValue(WIFI_RQST_DATE) : ��
GetWifiRequestValue(WIFI_RQST_HOUR) : ��
GetWifiRequestValue(WIFI_RQST_MINUTE) : ��
GetWifiRequestValue(WIFI_RQST_SECOND) : ��
GetWifiRequestValue(WIFI_RQST_DAY) : ���� (1 �� ~ 7 ��)

*/
U16 GetWifiRequestValue ( E_WIFI_RQST_T mId );

/*
GetWifiRequestSAP( void ); : SAP �ΰ����� DATA ��û
*/
void GetWifiRequestSAP ( void );

/*
�������� ���� 
GetWifiRequestApStatus(STATUS_AP_BLE_CERT);  : Return (0: ��Ȱ��ȭ ����, 1:�����������۰��ɻ���, 2: ���������Ϸ����)
*/
U8 GetWifiRequestApStatus (E_WIFI_STATUS_AP_T mId);


/* r_uart1_interrupt_send */
void InterruptUartWifiTx ( void );
/* r_uart1_interrupt_receive */
void InterruptUartWifiRx ( void );// U8 mu8RxData );
/* r_uart1_interrupt_error */
void InterruptUartWifiError ( void );

void WifiTimerControl ( void );
void IniEasyPairing ( void );
void SetClearSAPCode (void);

void SetWifiSystemStatus (E_WIFI_USER_VALUE_T mStatus, U8 mVal );
#endif

