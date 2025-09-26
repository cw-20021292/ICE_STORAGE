/// @file     Lib_RTC.h
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    RTC(EEPROM) 제어 동작 관련 file

#ifndef _LIB_RTC_H_
#define _LIB_RTC_H_


/*  Lib RTC Function 사용법
    => IICA0_CallbackMaster_SendInterrupt_EEPROM();         // r_Config_IICA0_callback_master_sendend 함수에 추가
    => IICA0_CallbackMaster_ReceiveInterrupt_EEPROM();      // r_Config_IICA0_callback_master_receiveend 함수에 추가
    => RTC_IIC_Start();                                     // RTC 사용시 IIC 통신을 시작
*/


#if (RTC_USE == USE)       // RTC(ISL12026) 사용시

#if (RTC_IIC_GPIO_USE == RTC_IIC_SFR)
void IICA0_CallbackMaster_SendInterrupt_EEPROM(void);
void IICA0_CallbackMaster_ReceiveInterrupt_EEPROM(void);
void RTC_IIC_Start(void);
#endif

#endif

#endif

