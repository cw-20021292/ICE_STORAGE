
#include "Global_header.h"


/// @brief    delay �Լ�
/// @details  ������ �ð���ŭ Delay ������ �Ѵ�
/// @param    mu8Time : delay �ð�(�� 8us����)
/// @return   void
void Delay(U16 mu8Time)
{
  U16 mu8i;

  for (mu8i = 0 ; mu8i <= mu8Time ; mu8i++)
  {
    R_Config_WDT_Restart();
  }
}


/// @brief    delay ms �Լ�
/// @details  ������ �ð�(�� 1ms����)��ŭ Delay ������ �Ѵ�
/// @param    mu8Time : delay �ð�(�� 1ms����)
/// @return   void
void Delay_ms(U16 mu16Time)
{
  U16 mu16i = 0;
  U16 mu16j = 0;

  for (mu16i = 0 ; mu16i <= (mu16Time*2) ; mu16i++)
  {
    for (mu16j = 0 ; mu16j <= 830 ; mu16j++)
    {   // constant value for 1msec counter = 830
      R_Config_WDT_Restart();
    }
  }
}


// ******************************************************************************
// *                                                                            *
// *                        Program Main Fuction                                *
// *                                                                            *
// ******************************************************************************
/// @brief    Main function
/// @details  Main �Լ� �ȿ� ��ġ�Ͽ� ����Ͽ� ���۽�Ų��
/// @param    void
/// @return   void
void User_Main(void)
{
    DI();

    Interrupt_Start();      // ���� Interrupt�� ���� ����

    Delay_ms(1000);         // ���� ����ȭ �ð� Ȯ��. While�� ���� ���Ŀ��� Delay �Լ� ��� ����(Time Scheduler �и� �� ����)

    Base_Initialize();

    EI();

    Serial_Start();

    WifiControlProcess(WIFI_TIME_INI);    // Wi-Fi Initialize. EEPROM Initialize ������ ����

    /* --- EEPROM Initialize START --- */
#if ( (EEPROM_USE == USE) || (RTC_USE == USE) )
    EEPROM_PRIMARY();     // EEPROM Initialize
    /* --- EEPROM Initialize END--- */
#endif

    WifiControlProcess(WIFI_TIME_SET);    // Wi-Fi WifiOnOffSetting. EEPROM Initialize ���Ŀ� ����
    ChangeOperationMode(OPERATION_MODE_ID_NORMAL);     // �Ϲ� ���� ���� ���� �� �� �ֵ��� ����


    while (1)
    {
        WDT_Restart();
        OperationModeSetup();       // ���� ��� ����
        GoTimeScheduler();          // Time Scheduler ���� ����
        FunctionProcess_In_WhileLoop();     // ���� ���� Loop
        WifiControlProcess(WIFI_TIME_WHILE);    // Wi-Fi While ����

        
    }
}





