/// @file     User_RTC.h
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    User가 사용할 RTC(EEPROM) 제어 동작 관련 file

#ifndef _USER_RTC_H_
#define _USER_RTC_H_


/*  Function 사용법
    => 주기적으로 Read_Time() 함수를 이용하여 RTC의 현재시간을 Read 한다
*/

#if (RTC_USE == USE)       // RTC(ISL12026) 사용시

// EEPROM Data ID
typedef enum
{
    EEPROM_ID_START = 0,                                // 0 : EEPROM ID(Address) Check Start(데이터 시작), 반드시 있어야 함
    EEPROM_ID_COLD_ON_OFF,                              // 1
    EEPROM_ID_VOICE_TYPE,                               // 2
    EEPROM_ID_VOICE_LANGUAGE,                           // 3
    EEPROM_ID_VOICE_VOLUME,                             // 4
    EEPROM_ID_ALL_KEY_LOCK,                             // 5
    EEPROM_ID_NO_USE_POWER_SAVE_ON_OFF,                 // 6
    EEPROM_ID_POWER_SAVE_ON_OFF,                        // 7
    EEPROM_ID_HOT_ON_OFF,                               // 8
    EEPROM_ID_HEIGHT_MODE,                              // 9
    EEPROM_ID_HOT_KEY_LOCK,                             // 10
    EEPROM_ID_MEMENTO_1,                                // 11 : EEPROM ID(Address) Memento 1
    EEPROM_ID_MEMENTO_2,                                // 12 : EEPROM ID(Address) Memento 2
    EEPROM_ID_MEMENTO_3,                                // 13 : EEPROM ID(Address) Memento 3
    EEPROM_ID_MEMENTO_4,                                // 14 : EEPROM ID(Address) Memento 4
    EEPROM_ID_MEMENTO_5,                                // 15 : EEPROM ID(Address) Memento 5
    EEPROM_ID_END,                                      // 16 : EEPROM ID(Address) Check End(데이터 끝), 반드시 있어야 함
    EEPROM_ID_MAX                                       // 255 Max, 255가 넘으면 안됨
} EEPROM_ID_T;


void Read_Time(void);

void EEPROM_PRIMARY(void);
void EEPROM_Initial(void);
void EEPROM_ReadDataCheck(void);

#endif

#endif

