/// @file     Lib_EEPROM.h
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    EEPROM ���� ���� ���� file

#ifndef _LIB_EEPROM_H_
#define _LIB_EEPROM_H_


/*  Lib EEPROM Function ����
    => IICA0_CallbackMaster_SendInterrupt_EEPROM();         // iica0_callback_master_sendend �Լ��� �߰�
    => IICA0_CallbackMaster_ReceiveInterrupt_EEPROM();      // iica0_callback_master_receiveend �Լ��� �߰�
    => EEPROM_IIC_Start();                                  // EEPROM ���� IIC ����� ����
*/

#if (EEPROM_USE == USE)

#if (EEPROM_IIC_GPIO_USE == EEPROM_IIC_SFR)
void IICA0_CallbackMaster_SendInterrupt_EEPROM(void);
void IICA0_CallbackMaster_ReceiveInterrupt_EEPROM(void);
void EEPROM_IIC_Start(void);
#endif

#endif

#endif
