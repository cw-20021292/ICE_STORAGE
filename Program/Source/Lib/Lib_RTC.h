/// @file     Lib_RTC.h
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    RTC(EEPROM) ���� ���� ���� file

#ifndef _LIB_RTC_H_
#define _LIB_RTC_H_


/*  Lib RTC Function ����
    => IICA0_CallbackMaster_SendInterrupt_EEPROM();         // r_Config_IICA0_callback_master_sendend �Լ��� �߰�
    => IICA0_CallbackMaster_ReceiveInterrupt_EEPROM();      // r_Config_IICA0_callback_master_receiveend �Լ��� �߰�
    => RTC_IIC_Start();                                     // RTC ���� IIC ����� ����
*/


#if (RTC_USE == USE)       // RTC(ISL12026) ����

#if (RTC_IIC_GPIO_USE == RTC_IIC_SFR)
void IICA0_CallbackMaster_SendInterrupt_EEPROM(void);
void IICA0_CallbackMaster_ReceiveInterrupt_EEPROM(void);
void RTC_IIC_Start(void);
#endif

#endif

#endif

