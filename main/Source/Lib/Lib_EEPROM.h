/// @file     Lib_EEPROM.h
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    EEPROM 제어 동작 관련 file

#ifndef _LIB_EEPROM_H_
#define _LIB_EEPROM_H_


/*  Lib EEPROM Function 사용법
    => IICA0_CallbackMaster_SendInterrupt_EEPROM();         // iica0_callback_master_sendend 함수에 추가
    => IICA0_CallbackMaster_ReceiveInterrupt_EEPROM();      // iica0_callback_master_receiveend 함수에 추가
    => EEPROM_IIC_Start();                                  // EEPROM 사용시 IIC 통신을 시작
*/

#if (EEPROM_USE == USE)

#if (EEPROM_IIC_GPIO_USE == EEPROM_IIC_SFR)
void IICA0_CallbackMaster_SendInterrupt_EEPROM(void);
void IICA0_CallbackMaster_ReceiveInterrupt_EEPROM(void);
void EEPROM_IIC_Start(void);
#endif

#endif

#endif
