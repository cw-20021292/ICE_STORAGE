#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "prj_type.h"

typedef enum _eeprom_id_
{
    /* CONFIG */
    EEP_ID_INIT_FIRST,        // EEP INIT FIRST

    EEP_ID_CONF_MAKE_COLD,    // 냉수 사용 설정
    EEP_ID_CONF_MAKE_HOT,     // 온수 사용 설정
    EEP_ID_CONF_MAKE_ICE,     // 제빙 사용 설정
    EEP_ID_CONF_TURBO,        // 쾌속 제빙 사용 설정
    EEP_ID_CONF_POWER_SAVING,  // 절전 사용 설정

    EEP_ID_CONF_STER,         // 살균 사용 설정
    EEP_ID_CONF_DECO_LED,     // DECO LED ON/OFF
    EEP_ID_CONF_CONTY,        // 연속 추출
    EEP_ID_CONF_FIXED_AMOUNT, // 연속 추출양
    EEP_ID_CONF_ICE_WATER_CONTY,        // 얼음물 연속 추출
    EEP_ID_CONF_ICE_WATER_FIXED_AMOUNT, // 얼음물 연속 추출양
    EEP_ID_CONF_MUTE,         // SOUND MUTE
    EEP_ID_HOT_ALTITUDE,      // 온수 고도 설정 

    EEP_ID_SEL_WATER,         // 물 선택 상태
    EEP_ID_SEL_ICE_WATER,     // 얼음물 선택 상태
    EEP_ID_DO_ICING,          // 현재 제빙 상태 ( UNUSED )
    EEP_ID_LOCK_ALL,          // 전체 잠금
    EEP_ID_LOCK_HOT,          // 온수 추출 잠금
    EEP_ID_LOCK_ICE,          // 얼음 추출 잠금
    EEP_ID_LOCK_ICE_WATER,    // 얼음물 추출 잠금
    EEP_ID_LOCK_ICE_WATER_AMOUNT,  // 얼음물 용량 잠금

    EEP_ID_MEMENTO_1,         // 메멘토 에러 1
    EEP_ID_MEMENTO_2,         // 메멘토 에러 2
    EEP_ID_MEMENTO_3,         // 메멘토 에러 3
    EEP_ID_MEMENTO_4,         // 메멘토 에러 4
    EEP_ID_MEMENTO_5,         // 메멘토 에러 5

    EEP_ID_SERVICE_CHECK_DAY,  // 서비스 점검 잔여일

    EEP_ID_FACTORY_SETUP,      // 공장 초기화 RESET 여부   
    EEP_ID_TIME,               // RTC 시간 셋팅 여부

    // WIFI
    EEP_ID_WIFI_POWER,
    EEP_ID_WIFI_FIRST_PARING,
    EEP_ID_WIFI_FOTA_MODULE,
    EEP_ID_WIFI_FOTA_MCU,
    //EEP_ID_WIFI_FOTA_RESET,

    /*************************************************************************/
    EEP_ID_STER_DAY_PASS,           /*살균 경과일*/
    EEP_ID_STER_ADJUST_MODE,        /*살균 시간 보정 모드*/
    EEP_ID_STER_SET_HOUT_CONFIG,    /*살균 예약 시간 설정 여부 */
    EEP_ID_STER_PERIOD,             /*살균 주기 3일or 5일*/
    EEP_ID_STER_PERIOD_AUTO_CHANGE, /*살균 주기 자동 변경*/
    EEP_ID_STER_RESERVATION_HOUR,   /*살균 예약 시*/
    EEP_ID_STER_RESERVATION_MIN,    /*살균 예약 분*/
    EEP_ID_STER_ERR_WATER_OUT,      /*살균 중 에러 */
    EEP_ID_STER_DOING,              /*살균수 투입 조건*/

    /*************************************************************************/

    // NFC
    EEP_ID_NFC_ERROR_HIS_COUNT,
    EEP_ID_NFC_USER_HIS_COUNT,
    EEP_ID_NFC_STER_HIS_COUNT,
    EEP_ID_NFC_WATER_HIS_COUNT,

    // UV
    EEP_ID_CONF_UV_MODE,

    /* 스마트 미사용 절전 기능 ON/OFF 설정 */
    EEP_ID_CONF_UNUSED_SAVE,

    EEP_ID_INIT_END,          // EEPROM INIT END

    EEP_ID_CHKSUM_HIGH,     // CHECK SUM High Byte
    EEP_ID_CHKSUM_LOW,      // CHECK SUM Low Byte

    EEP_ID_MAX
} EEepromId_T;

void InitEeprom(void);

void SaveEepromId(EEepromId_T id);

void EraseEeprom(void);

void ProcessEeprom(void);

U8 TestEeprom(void);

#endif /* __EEPROM_H__ */
