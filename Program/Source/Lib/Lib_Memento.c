/// @file   Lib_Memento.c
/// @date   2025/06/25
/// @author Jaejin Ham
/// @brief  Memento 동작 제어 파일

#include "Global_header.h"

#if (MEMENTO_MODE_USE == USE)

U8 gu8Memento[MEMENTO_ID_MAX];                /// @brief    에러 메멘토 리스트


/// @brief      에러 메멘토 Read 함수
/// @details    EEPROM에 저장된 메멘토값 Read하여 데이터 배치시킨다
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


/// @brief      에러 메멘토 확인 함수
/// @details    ID에 해당하는 메멘토 에러 데이터를 확인하여 그 값을 반환한다
/// @param      mu8ID : 메멘토 ID
/// @return     return : 저장된 에러값
U8 Get_MementoData(U8 mu8ID)
{
    return  gu8Memento[mu8ID];
}


/// @brief      최종 저장한 에러 메멘토 확인 함수
/// @details    최종 발생하여 저장된 메멘토 에러 데이터를 확인하여 그 값을 반환한다
/// @param      void
/// @return     return : 저장된 에러값
U8 Get_MementoDataLast(void)
{
    return  gu8Memento[MEMENTO_ID_1];
}


/// @brief      에러 메멘토 저장 함수
/// @details    현재 발생한 에러를 메멘토 기능으로 저장시킨다
/// @param      mu8ErrorCode : 현재 발생한 에러 코드(디스플레이 하는 에러)
/// @return     void
void MementoWrite(U8 mu8ErrorCode)
{
    if (gu8Memento[MEMENTO_ID_1] != mu8ErrorCode)
    {   // 가장 최근 발생되었던 에러와 에러코드가 중복 발생되지 않은 경우
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