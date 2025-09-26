
#ifndef _WIFI_CONTROL_H_
#define _WIFI_CONTROL_H_

#include "WIFI_DefUser.h"
#include "WIFI_SetFunctionData.h"
#include "WIFI_Status.h"

/***** <User Header> **********************************************************/
/* User 는 아래 1개의 Header 만 참조 
#include "WIFI_Control.h"
*/

/***** <User Function> ********************************************************/
/*
<1> 셋팅 및 주기함수
WifiControlProcess(WIFI_TIME_INI) : 초기화 (EEPROM 이전) 
WifiControlProcess(WIFI_TIME_SET) : 초기셋팅 (EEPROM 이후 While 이전)
WifiControlProcess(WIFI_TIME_FACTORY) : 공장모드 / 출하테스트 후 초기화
WifiControlProcess(WIFI_TIME_WHILE) : while 
WifiControlProcess(WIFI_TIME_100MS) : 100ms 주기
WifiControlProcess(WIFI_TIME_1SEC) : 1sec 주기
WifiControlProcess(WIFI_TIME_1MIN) : 1min 주기
WifiControlProcess(WIFI_TIME_INI_EASY_PAIRING) : 페어링 경험 없음 시 이지페어링(BLE활성화) 시작
*/
void WifiControlProcess ( E_WIFI_TIME_T mu8Time );
void WifiControlProcess100ms ( void );

/*
<2> Wifi Key
WifiKey(WIFI_KEY_AP) : Wifi Pairing (냉수+정수 3초)
WifiKey(WIFI_KEY_BLE) : BLE Pairing (정수 3초)
WifiKey(WIFI_KEY_ON)
WifiKey(WIFI_KEY_OFF)
WifiKey(WIFI_KEY_ONOFF) : Power On/Off (냉수+용량 3초)
WifiKey(WIFI_KEY_TEST) : Factory Test 

WifiKey(WIFI_KEY_BLE_CERT) : 점유인증 키 (BLE 점유인증 대기상태 + 정수3초)
WifiKey(WIFI_KEY_BLE_CERT_CANCEL) : 점유인증 취소 (정수7초) --> 미사용

*/
void WifiKey ( E_WIFI_KEY_T mu8Key );

/*
<3> Wifi Send Data : 엑셀파일 참조
WifiSendData(WIFI_DATA_FUNCTION) : 기능데이터 변경 시 (KEY입력/추출완료/필터리셋/기능변경/고장진단관련...)
WifiSendData(WIFI_DATA_ERROR) : 에러데이터 변경 시 (발생/해지)
WifiSendData(WIFI_DATA_PART) : 상시 고장진단 데이터 (주기)
WifiSendData(WIFI_DATA_EXAMINE) : 정밀 고장진단 완료 시 (이벤트)
*/
void WifiSendData ( E_WIFI_DATA_T mu8Data );
/*
WifiSendDataControl(WIFI_DATA_FUNCTION_CONTROL, ID) : 제품 KEY조작 시 (제품 조작 데이터는 프로토콜 참조)
*/
void WifiSendDataControl ( E_WIFI_DATA_T mu8Data, U16 mu16IdData);
/*
<4> Wifi Get data 
GetWifiStatusValue(WIFI_STATUS_DISP) : WIFI DISPLAY (E_WIFI_DISPLAY_T)
    // Wi-Fi OFF [소등]
    WIFI_DISP_OFF:          

    // Soft AP Pairing MODE [페어링 점멸]
    WIFI_DISP_AP_START:     // Soft AP MODE START     : WiFi 페어링 모드 진입
    WIFI_DISP_AP_STEP0:     // Pairing Mode ing       : Soft AP 페어링 모드 활성화 완료
    WIFI_DISP_AP_STEP1:     // APP Pairing START      : APP과 페어링 시작 (25%)
    WIFI_DISP_AP_STEP2:     // AP Connect             : AP연결완료 (50%)
    WIFI_DISP_AP_STEP3:     // AUTHENTICATE           : 인증서버 확인 중 (75&)

    // BLE Pairing MODE [페어링 점멸]
    WIFI_DISP_BLE_START:    // BLE MODE START         : BLE 페어링 모드 진입
    WIFI_DISP_BLE_STEP0:    // Pairing Mode ing       : Soft AP 페어링 모드 활성화 완료
    WIFI_DISP_BLE_STEP1:    // APP Pairing START      : APP과 페어링 시작 (25%)
    WIFI_DISP_BLE_STEP2:    // AP Connect             : AP연결완료 (50%)
    WIFI_DISP_BLE_STEP3:    // AUTHENTICATE           : 인증서버 확인 중 (75%)

    // Server 연결완료 [점등]
    WIFI_DISP_CONNECT:      // 서버 연결완료          : (100%)

    // Wi-Fi ON & 서버 연결끊김 [점멸]
    WIFI_DISP_AP1:          
    WIFI_DISP_AP2:          
    WIFI_DISP_AP3:          
    WIFI_DISP_AP4:          
    WIFI_DISP_AP5:          
    WIFI_DISP_TRY:          

GetWifiStatusValue(WIFI_STATUS_POWER) : 전원 확인 (0:Off/1:On)
GetWifiStatusValue(WIFI_STATUS_TEST) : TestMode 에서 연결확인 (0:Disconnected/1:Connected)
GetWifiStatusValue(WIFI_STATUS_ERROR) : Wifi Error (0:Normal/1~5:Err) (페어링 시작 후 체크 시 Error)
GetWifiStatusValue(WIFI_STATUS_AP_ERROR) : AP Error (0:Normal/1~5:Err) (페어링중 체크 시 Error)
GetWifiStatusValue(WIFI_STATUS_AP_FIRST) : (0:최초연결X /1:최초연결완료)
GetWifiStatusValue(WIFI_STATUS_AP_MODE) : (0:Normal/1:ApMode)
GetWifiStatusValue(WIFI_STATUS_AP_BLE) : (0:Normal/1:BLEApMode)
GetWifiStatusValue(WIFI_STATUS_BLE_CERT) : (0:점유인증불가상태/1:점유인증가능 상태/2:점유인증완료상태)  // 점유인증 조작 추가!

GetWifiStatusValue(WIFI_STATUS_SERVER_CON) : 0  CLEAR  / 1 SET (연결중) 
GetWifiStatusValue(WIFI_STATUS_PAIRING) :   0  CLEAR  / 1 : BLE활성화 / 2: Wi-Fi 연결모드 활성화 
GetWifiStatusValue(WIFI_STATUS_LOCATION) :  0 APP에서 위치정보 설정 안됨 / 1 : APP에서 위치정보 설정 완료
GetWifiStatusValue(WIFI_STATUS_FOTA) :  0  / 1 : FOTA 중
GetWifiStatusValue(WIFI_STATUS_SAP_CODE_0030_UV_DATE) :  SAP 부가정보 DATA 수신여부 -> Return :(1: S1000 / 2 : DATA 없음 / 0: 실패)
GetWifiStatusValue(WIFI_STATUS_SAP_CODE_0031_SUPPLIES) :  SAP 부가정보 DATA 수신여부 -> Return :(1: S1000 / 2 : DATA 없음 / 0: 실패)
GetWifiStatusValue(WIFI_STATUS_SAP_CODE_0032_ENVIRONMENT) :  SAP 부가정보 DATA 수신여부 -> Return :(1: S1000 / 2 : DATA 없음 / 0: 실패)

*/
U8 GetWifiStatusValue ( E_WIFI_STATUS_T mType );

/*
<5> Wifi Get Request Info 요청값 리턴
GetWifiRequestValue(WIFI_RQST_WEATHER) : 날씨데이터 (1 ~ 12)
GetWifiRequestValue(WIFI_RQST_AQI) : AQI (1 ~ 4)
GetWifiRequestValue(WIFI_RQST_CURRENT_TEMP) : 현재온도 (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_HIGH_TEMP) : 최고온도 (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_LOW_TEMP) : 최저온도 (-50 ~ 50)
GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY1) : 필터1 배송경과일 (0xFFFF 인 경우에는 방문관리 / 배송일이 없는경우 9999 )
GetWifiRequestValue(WIFI_RQST_FILTER_D_DAY2) : 필터2 배송경과일 (0xFFFF 인 경우에는 방문관리 / 배송일이 없는경우 9999 )
GetWifiRequestValue(WIFI_RQST_FILTER_CHANGE_CYCLE) : 필터교환 주기(%) (15Month(기본) X 100%(기본), 1~999 %)
GetWifiRequestValue(WIFI_RQST_PART_PERIOD) : 상시진단 주기 적용 (EX : 1 --> 1시간)

GetWifiRequestValue(WIFI_RQST_YEAR) : 년 (2024)
GetWifiRequestValue(WIFI_RQST_MONTH) : 월 
GetWifiRequestValue(WIFI_RQST_DATE) : 일
GetWifiRequestValue(WIFI_RQST_HOUR) : 시
GetWifiRequestValue(WIFI_RQST_MINUTE) : 분
GetWifiRequestValue(WIFI_RQST_SECOND) : 초
GetWifiRequestValue(WIFI_RQST_DAY) : 요일 (1 월 ~ 7 일)

*/
U16 GetWifiRequestValue ( E_WIFI_RQST_T mId );

/*
GetWifiRequestSAP( void ); : SAP 부가정보 DATA 요청
*/
void GetWifiRequestSAP ( void );

/*
점유인증 상태 
GetWifiRequestApStatus(STATUS_AP_BLE_CERT);  : Return (0: 비활성화 상태, 1:점유인증조작가능상태, 2: 점유인증완료상태)
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

