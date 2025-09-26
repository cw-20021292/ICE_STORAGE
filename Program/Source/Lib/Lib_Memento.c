/// @file   Lib_Memento.c
/// @date   2025/06/25
/// @author Jaejin Ham
/// @brief  Memento ���� ���� ����

#include "Global_header.h"

#if (MEMENTO_MODE_USE == USE)

U8 gu8Memento[MEMENTO_ID_MAX];                /// @brief    ���� �޸��� ����Ʈ


/// @brief      ���� �޸��� Read �Լ�
/// @details    EEPROM�� ����� �޸��䰪 Read�Ͽ� ������ ��ġ��Ų��
/// @param      void
/// @return     void
void MementoRead(void)
{
    MEMSET( (void __FAR *)&gu8Memento, 0x00, sizeof(gu8Memento) );

    gu8Memento[MEMENTO_ID_1] = Get_EEPROM_Data(EEPROM_ID_MEMENTO_1);
    gu8Memento[MEMENTO_ID_2] = Get_EEPROM_Data(EEPROM_ID_MEMENTO_2);
    gu8Memento[MEMENTO_ID_3] = Get_EEPROM_Data(EEPROM_ID_MEMENTO_3);
    gu8Memento[MEMENTO_ID_4] = Get_EEPROM_Data(EEPROM_ID_MEMENTO_4);
    gu8Memento[MEMENTO_ID_5] = Get_EEPROM_Data(EEPROM_ID_MEMENTO_5);
}


/// @brief      ���� �޸��� Ȯ�� �Լ�
/// @details    ID�� �ش��ϴ� �޸��� ���� �����͸� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      mu8ID : �޸��� ID
/// @return     return : ����� ������
U8 Get_MementoData(U8 mu8ID)
{
    return  gu8Memento[mu8ID];
}


/// @brief      ���� ������ ���� �޸��� Ȯ�� �Լ�
/// @details    ���� �߻��Ͽ� ����� �޸��� ���� �����͸� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ����� ������
U8 Get_MementoDataLast(void)
{
    return  gu8Memento[MEMENTO_ID_1];
}


/// @brief      ���� �޸��� ���� �Լ�
/// @details    ���� �߻��� ������ �޸��� ������� �����Ų��
/// @param      mu8ErrorCode : ���� �߻��� ���� �ڵ�(���÷��� �ϴ� ����)
/// @return     void
void MementoWrite(U8 mu8ErrorCode)
{
    if (gu8Memento[MEMENTO_ID_1] != mu8ErrorCode)
    {   // ���� �ֱ� �߻��Ǿ��� ������ �����ڵ尡 �ߺ� �߻����� ���� ���
        gu8Memento[MEMENTO_ID_5] = gu8Memento[MEMENTO_ID_4];
        gu8Memento[MEMENTO_ID_4] = gu8Memento[MEMENTO_ID_3];
        gu8Memento[MEMENTO_ID_3] = gu8Memento[MEMENTO_ID_2];
        gu8Memento[MEMENTO_ID_2] = gu8Memento[MEMENTO_ID_1];
        gu8Memento[MEMENTO_ID_1] = mu8ErrorCode;

        Set_EEPROM_Data(EEPROM_ID_MEMENTO_1, gu8Memento[MEMENTO_ID_1]);
        Set_EEPROM_Data(EEPROM_ID_MEMENTO_2, gu8Memento[MEMENTO_ID_2]);
        Set_EEPROM_Data(EEPROM_ID_MEMENTO_3, gu8Memento[MEMENTO_ID_3]);
        Set_EEPROM_Data(EEPROM_ID_MEMENTO_4, gu8Memento[MEMENTO_ID_4]);
        Set_EEPROM_Data(EEPROM_ID_MEMENTO_5, gu8Memento[MEMENTO_ID_5]);
    }
}

#endif