/// @file     User_EEPROM.c
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    User�� ����� EEPROM ���� ���� ���� file

#include  "Global_Header.h"


#if (EEPROM_USE == USE)


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


/// @brief      EEPROM Read Data ��ȿ�� �˻�(User�� ������ �ڵ忡 ���� �ڵ� ����)
/// @details    USER�� ����ϴ� �������� �����͵��� ��ȿ�� �˻縦 �����Ѵ�
/// @param      void
/// @return     void
void EEPROM_ReadDataCheck(void)
{
/*
    U8 mu8EEPROM_DataBuf = 0;

    // �ڵ� ���� - �ü� ��� ����
    mu8EEPROM_DataBuf = Get_EEPROM_Data(EEPROM_ID_COLD_ON);

    if (mu8EEPROM_DataBuf == ON)
    {
        Setup_ColdOnOff(ON);
    }
    else if (mu8EEPROM_DataBuf == OFF)
    {
        Setup_ColdOnOff(OFF);
    }
    else
    {   // ��ȿ���� ��� ��� �ش� ������ �ʱ�ȭ
        Setup_ColdOnOff(ON);
        Set_EEPROM_Data(EEPROM_ID_COLD_ON_OFF, ON);
    }
*/

#if (MEMENTO_MODE_USE == USE)
    MementoRead();      // �޸��� ������ Read
#endif
}


#endif

