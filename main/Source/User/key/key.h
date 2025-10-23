/**
 * File : key.h
 * 
 * Key Scan Control
*/
#ifndef __KEY_H__
#define __KEY_H__

#include "api_key.h"

/// SINGLE KEY
#define K_NONE              KEY_NONE

/// Front
/// Setting - Hidden
#define K_N_LOCK_ALL        KEY_1   /// PBA ->K_LOCK_ALL (UNUSED)
#define K_COLD_OFF          KEY_2   /// PBA ->K_COLD_OFF (UNUSED)
#define K_HOT_LOCK          KEY_3   /// PBA ->K_HOT_LOCK
#define K_ICE_WATER_LOCK    KEY_4   /// PBA ->K_ICE_WATER_LOCK
#define K_ICE_LOCK          KEY_5   /// PBA ->K_ICE_LOCK
#define K_HOT_OFF           KEY_6   /// PBA ->K_HOT_OFF (UNUSED)
#define K_ICE_OFF           KEY_7   /// PBA ->K_ICE_OFF (UNUSED)

/// Printed Key
#define K_ICE_WATER_AMOUNT  KEY_8
#define K_HOT               KEY_9
#define K_ROOM              KEY_10
#define K_COLD              KEY_11
#define K_AMOUNT            KEY_12
#define K_ICE               KEY_13
#define K_ICE_WATER         KEY_14

#define K_ALL               KEY_ALL

/** 
 * FUNCTION KEY 
 * (IOT) WIFI �� - "���� + �ü�" 3��
 * (IOT) BLE �� - "����" 3��
 * (IOT) WIFI ON/OFF - "�ü� + ���뷮" 3��
 * (�ڵ�����) �ڵ� ���� ���� - "�¼�+����+���뷮" 3��
*/
#define K_LOCK_ALL         ( K_ICE_LOCK | K_HOT_LOCK )
#define K_VERSION          ( K_COLD | K_HOT )
#define K_MEMENTO          ( K_ROOM | K_HOT  | K_COLD )
#define K_MUTE             ( K_ROOM | K_AMOUNT )
#define K_ALTITUDE         ( K_HOT | K_AMOUNT )               /// �¼� �� ����
#define K_ERASE_EEPROM     ( K_ICE_WATER | K_HOT )            /// EEPROM �ʱ�ȭ
#define K_DEICING          ( K_ICE | K_COLD )                 /// ���� Ż��

#define K_MANUAL_DRAIN     ( K_HOT | K_COLD | K_AMOUNT )      /// �������

#define K_TEST_ICE_TRAY    ( K_ICE | K_ROOM )                 /// ���̽� Ʈ���� �˻�

#define K_TEMP_MONITOR     ( K_ICE | K_ICE_WATER | K_COLD )   /// �ܱ�µ�1 / �ܱ�µ�2 / �ü��µ� ����͸�

#define K_AGING_TEST       ( K_ICE | K_ROOM | K_AMOUNT )      /// ����¡ �׽�Ʈ

#define K_SMART_UNUSED_SAVE ( K_ROOM | K_COLD | K_AMOUNT )    /// ����Ʈ�̻������ ��� OFF

/* FUNCTION KEY - TEST */
#define K_TEST_FCT         ( K_ICE_WATER | K_COLD )           /// TEST MAIN PBA
#define K_TEST_FRONT       ( K_ICE | K_AMOUNT )               /// TEST EOL - FRONT
#define K_TEST_MAIN_FRONT  ( K_ICE_WATER | K_AMOUNT )         /// TEST EOL - MAIN FRONT
#define K_TEST_TS          ( K_ICE | K_HOT )                  /// TEST MODE ( TIME SHORT )
#define K_TEST_SHIPMENT    ( K_ICE | K_ICE_WATER | K_AMOUNT ) /// SHIPMENT MODE - MANUAL DRAIN START, FEEDER ALWAYS CW ON

#define K_TEST_COMP        ( K_ICE | K_ROOM | K_COLD )        /// TEST, ����� ���ӽð� �ʱ�ȭ 
#define K_TEST_DEICING     ( K_ICE | K_ICE_WATER | K_HOT )    /// TEST, ���� Ż�� ����
#define K_TEST_STER_CONTY  ( K_ICE | K_ICE_WATER | K_ROOM )   /// TEST,

/* WIFI */
//#define K_WIFI_AP        ( K_COLD | K_ROOM )
//#define K_WIFI_ONOFF     ( K_COLD | K_AMOUNT )

//#define K_DRAIN_WATER     ( K_STER | K_HOT_LOCK )     /// �ڵ� ����� ����
//#define K_MANUAL_STER     ( K_STER | K_ROOM )         /// �������
//#define K_ADJUST_HOUR     ( K_STER | K_HOT )          /// ��� �ð� ����
//#define K_SET_HOUR        ( K_STER | K_ICE_TURBO )    /// ��� �ð� ����
//#define K_RESET_STER      ( K_HOT_LOCK | K_ICE_LOCK | K_SAVE | K_STER )   /// ��� ��� ������� ��� ����
//#define K_TEST_KEY        ( K_SAVE | K_HOT_LOCK )     /// TEST EOL - KEY
//#define K_TEST_EOL        ( K_ROOM | K_AMOUNT )       /// TEST EOL - LOAD 

/* EvnetFlag Index */
#define  KEY_EVENT_PUSHED_SHORT     (0x01)
#define  KEY_EVENT_PUSHED_LONG_7S   (0x02)
#define  KEY_EVENT_PUSHED_LONG_5S   (0x04)
#define  KEY_EVENT_PUSHED_LONG_3S   (0x08)
#define  KEY_EVENT_PUSHED_LONG_2S   (0x10)
#define  KEY_EVENT_PUSHED_LONG_1S   (0x20)
#define  KEY_EVENT_POP              (0x40)
#define  KEY_EVENT_ALL              (0x7F)

void InitKey(void);
U32 GetKeyVal(void);
U32 GetKeyPopVal(void);

void SetKeyEventFlag(U8 event);
void ClearKeyEventFlag(U8 event);
U8 IsSetKeyEventFlag(U8 event);

void SetSkipInvalidSound(U8 skip);
U8 GetSkipInvalidSound(void);

void ProcessScanKey(void);

#endif /* __KEY_H__ */
