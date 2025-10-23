#ifndef __EEPROM_H__
#define __EEPROM_H__

#include "prj_type.h"

typedef enum _eeprom_id_
{
    /* CONFIG */
    EEP_ID_INIT_FIRST,        // EEP INIT FIRST

    EEP_ID_CONF_MAKE_COLD,    // �ü� ��� ����
    EEP_ID_CONF_MAKE_HOT,     // �¼� ��� ����
    EEP_ID_CONF_MAKE_ICE,     // ���� ��� ����
    EEP_ID_CONF_TURBO,        // ��� ���� ��� ����
    EEP_ID_CONF_POWER_SAVING,  // ���� ��� ����

    EEP_ID_CONF_STER,         // ��� ��� ����
    EEP_ID_CONF_DECO_LED,     // DECO LED ON/OFF
    EEP_ID_CONF_CONTY,        // ���� ����
    EEP_ID_CONF_FIXED_AMOUNT, // ���� �����
    EEP_ID_CONF_ICE_WATER_CONTY,        // ������ ���� ����
    EEP_ID_CONF_ICE_WATER_FIXED_AMOUNT, // ������ ���� �����
    EEP_ID_CONF_MUTE,         // SOUND MUTE
    EEP_ID_HOT_ALTITUDE,      // �¼� �� ���� 

    EEP_ID_SEL_WATER,         // �� ���� ����
    EEP_ID_SEL_ICE_WATER,     // ������ ���� ����
    EEP_ID_DO_ICING,          // ���� ���� ���� ( UNUSED )
    EEP_ID_LOCK_ALL,          // ��ü ���
    EEP_ID_LOCK_HOT,          // �¼� ���� ���
    EEP_ID_LOCK_ICE,          // ���� ���� ���
    EEP_ID_LOCK_ICE_WATER,    // ������ ���� ���
    EEP_ID_LOCK_ICE_WATER_AMOUNT,  // ������ �뷮 ���

    EEP_ID_MEMENTO_1,         // �޸��� ���� 1
    EEP_ID_MEMENTO_2,         // �޸��� ���� 2
    EEP_ID_MEMENTO_3,         // �޸��� ���� 3
    EEP_ID_MEMENTO_4,         // �޸��� ���� 4
    EEP_ID_MEMENTO_5,         // �޸��� ���� 5

    EEP_ID_SERVICE_CHECK_DAY,  // ���� ���� �ܿ���

    EEP_ID_FACTORY_SETUP,      // ���� �ʱ�ȭ RESET ����   
    EEP_ID_TIME,               // RTC �ð� ���� ����

    // WIFI
    EEP_ID_WIFI_POWER,
    EEP_ID_WIFI_FIRST_PARING,
    EEP_ID_WIFI_FOTA_MODULE,
    EEP_ID_WIFI_FOTA_MCU,
    //EEP_ID_WIFI_FOTA_RESET,

    /*************************************************************************/
    EEP_ID_STER_DAY_PASS,           /*��� �����*/
    EEP_ID_STER_ADJUST_MODE,        /*��� �ð� ���� ���*/
    EEP_ID_STER_SET_HOUT_CONFIG,    /*��� ���� �ð� ���� ���� */
    EEP_ID_STER_PERIOD,             /*��� �ֱ� 3��or 5��*/
    EEP_ID_STER_PERIOD_AUTO_CHANGE, /*��� �ֱ� �ڵ� ����*/
    EEP_ID_STER_RESERVATION_HOUR,   /*��� ���� ��*/
    EEP_ID_STER_RESERVATION_MIN,    /*��� ���� ��*/
    EEP_ID_STER_ERR_WATER_OUT,      /*��� �� ���� */
    EEP_ID_STER_DOING,              /*��ռ� ���� ����*/

    /*************************************************************************/

    // NFC
    EEP_ID_NFC_ERROR_HIS_COUNT,
    EEP_ID_NFC_USER_HIS_COUNT,
    EEP_ID_NFC_STER_HIS_COUNT,
    EEP_ID_NFC_WATER_HIS_COUNT,

    // UV
    EEP_ID_CONF_UV_MODE,

    /* ����Ʈ �̻�� ���� ��� ON/OFF ���� */
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
