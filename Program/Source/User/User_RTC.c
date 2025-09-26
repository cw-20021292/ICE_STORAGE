/// @file     User_RTC.c
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    User�� ����� RTC(EEPROM) ���� ���� ���� file

#include  "Global_Header.h"


#if (RTC_USE == USE)

SRTC_TimeData_T STime;          /// @brief  ��¥, �ð� ������


/// @brief      EEPROM�� ������ Initail Data
/// @details    User�� ������ Address�� �°� �ʱⰪ�� �迭�� ��Ī�Ͽ� �����Ѵ�
U8 gu8EepromInitialValue[EEPROM_ID_MAX] =
{   // ������ �ʱⰪ
    VALUE_DEFINE_EEPROM_DATA_START,     // EEPROM ������ ���� ������
    0,          // MEMENTO 1
    0,          // MEMENTO 2
    0,          // MEMENTO 3
    0,          // MEMENTO 4
    0,          // MEEMNTO 5
    VALUE_DEFINE_EEPROM_DATA_END        // EEPROM ������ �� ������
};



/// @brief      RTC���� ���� ��¥�� �ð��� Read �ϴ� �Լ�
/// @details    RTC���� ���� ��¥�� �ð��� Read�Ѵ�
/// @param      void
/// @return     void
void Read_Time(void)
{
    Get_RTC_Time(&STime);
}


/// @brief      EEPROM �ʱ�ȭ(User Code���� ��ǰ ���� ���� �� ��ġ�ؾ� �ϴ� �Լ�)
/// @details    EEPROM �ʱ�ȭ ������ �Ѵ�
/// @param      void
/// @return     void
void EEPROM_PRIMARY(void)
{
    U8 mu8EEPROM_ErrorStatus = 0;

    mu8EEPROM_ErrorStatus = Get_EepromErrorCheck();        // EEPROM ������ �̻� ���� Ȯ��

    if (mu8EEPROM_ErrorStatus == TRUE)
    {   // EEPROM�� ������ ����Ǹ�
        EEPROM_Initial();               // EEPROM �ʱ�ȭ
        EEPROM_ReadDataCheck();         // Read Data ��ȿ�� �˻�
    }
    else
    {   // EEPROM�� ������ ������
        EEPROM_Data_Load();             // EEPROM Data Read
        EEPROM_ReadDataCheck();         // Read Data ��ȿ�� �˻�
    }
}


/// @brief      EEPROM �ʱ���� Data ����
/// @details    EEPROM�� �ʱⰪ���� Write ��Ų��
/// @param      void
/// @return     void
void EEPROM_Initial(void)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < EEPROM_ID_MAX ; mu8i++)
    {
        Set_EEPROM_Data(mu8i, gu8EepromInitialValue[mu8i]);     // ������ �Է� �� ����
    }
}


/// @brief      EEPROM Read Data ��ȿ�� �˻�
/// @details    USER�� ����ϴ� �������� �����͵��� ��ȿ�� �˻縦 �����Ѵ�
/// @param      void
/// @return     void
void EEPROM_ReadDataCheck(void)
{
/*
    U8 mu8CheckData = 0;

    // �ڵ� ���� - �ü� ��� ����
    mu8CheckData = Get_EEPROM_Data(EEPROM_ID_COLD_ON);

    if (mu8CheckData == 1)
    {
        gu8ColdOn = ON;
    }
    else if (mu8CheckData == 0)
    {
        gu8ColdOn = OFF;
    }
    else
    {   // ��ȿ���� ��� ��� �ش� ������ �ʱ�ȭ
        Set_EEPROM_Data(EEPROM_ID_COLD_ON, gu8EepromInitialValue[EEPROM_ID_COLD_ON]);
        gu8ColdOn = ON;
    }
*/
}


#endif

