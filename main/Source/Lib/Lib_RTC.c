/// @file     Lib_RTC.c
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    RTC(EEPROM) 제어 동작 관련 file

#include  "Global_Header.h"


#if (RTC_USE == USE)

#if (RTC_IIC_GPIO_USE == RTC_IIC_SFR)
/// @brief      EEPROM 사용시 IICA0 Callback Master Interrupt에 추가할 함수
/// @details    r_Config_IICA0_callback_master_sendend 함수에 추가 필요
/// @param      void
/// @return     void
void IICA0_CallbackMaster_SendInterrupt_EEPROM(void)
{
    EEPROM_IICA0_WriteProcess_Done();        // IICA0 동작 완료 처리 함수
}


/// @brief      EEPROM 사용시 IICA0 Callback Master Interrupt에 추가할 함수
/// @details    r_Config_IICA0_callback_master_receiveend 함수에 추가가 필요하다
/// @param      void
/// @return     void
void IICA0_CallbackMaster_ReceiveInterrupt_EEPROM(void)
{
    EEPROM_IICA0_ReadProcess_Done();        // IICA0 동작 완료 처리 함수
}


/// @brief      RTC 사용시 IICA0 통신을 시작하도록 처리하는 함수(User가 본인 코드에 추가 필요)
/// @details    RTC 사용시 IICA0 통신을 시작한다
/// @param      void
/// @return     void
void RTC_IIC_Start(void)
{
    IICA0_Start();
}

#endif

#endif



