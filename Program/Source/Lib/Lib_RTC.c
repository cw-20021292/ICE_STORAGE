/// @file     Lib_RTC.c
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    RTC(EEPROM) ���� ���� ���� file

#include  "Global_Header.h"


#if (RTC_USE == USE)

#if (RTC_IIC_GPIO_USE == RTC_IIC_SFR)
/// @brief      EEPROM ���� IICA0 Callback Master Interrupt�� �߰��� �Լ�
/// @details    r_Config_IICA0_callback_master_sendend �Լ��� �߰� �ʿ�
/// @param      void
/// @return     void
void IICA0_CallbackMaster_SendInterrupt_EEPROM(void)
{
    EEPROM_IICA0_WriteProcess_Done();        // IICA0 ���� �Ϸ� ó�� �Լ�
}


/// @brief      EEPROM ���� IICA0 Callback Master Interrupt�� �߰��� �Լ�
/// @details    r_Config_IICA0_callback_master_receiveend �Լ��� �߰��� �ʿ��ϴ�
/// @param      void
/// @return     void
void IICA0_CallbackMaster_ReceiveInterrupt_EEPROM(void)
{
    EEPROM_IICA0_ReadProcess_Done();        // IICA0 ���� �Ϸ� ó�� �Լ�
}


/// @brief      RTC ���� IICA0 ����� �����ϵ��� ó���ϴ� �Լ�(User�� ���� �ڵ忡 �߰� �ʿ�)
/// @details    RTC ���� IICA0 ����� �����Ѵ�
/// @param      void
/// @return     void
void RTC_IIC_Start(void)
{
    IICA0_Start();
}

#endif

#endif



