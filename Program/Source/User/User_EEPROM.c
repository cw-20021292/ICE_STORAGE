/// @file     User_EEPROM.c
/// @date     2025/05/20
/// @author   Jaejin Ham
/// @brief    User가 사용할 EEPROM 제어 동작 관련 file

#include  "Global_Header.h"


#if (EEPROM_USE == USE)


/// @brief      EEPROM에 저장힐 Initail Data
/// @details    User가 설정한 Address에 맞게 초기값을 배열에 매칭하여 설정한다
U8 gu8EepromInitialValue[EEPROM_ID_MAX] =
{   // 데이터 초기값
    VALUE_DEFINE_EEPROM_DATA_START,     // EEPROM 데이터 시작 구분자
    0,          // MEMENTO 1
    0,          // MEMENTO 2
    0,          // MEMENTO 3
    0,          // MEMENTO 4
    0,          // MEEMNTO 5
    VALUE_DEFINE_EEPROM_DATA_END        // EEPROM 데이터 끝 구분자
};



/// @brief      EEPROM 초기화(User Code에서 제품 동작 시작 전 배치해야 하는 함수)
/// @details    EEPROM 초기화 동작을 한다
/// @param      void
/// @return     void
void EEPROM_PRIMARY(void)
{
    U8 mu8EEPROM_ErrorStatus = 0;

    mu8EEPROM_ErrorStatus = Get_EepromErrorCheck();        // EEPROM 데이터 이상 여부 확인

    if (mu8EEPROM_ErrorStatus == TRUE)
    {   // EEPROM에 에러가 검출되면
        EEPROM_Initial();               // EEPROM 초기화
        EEPROM_ReadDataCheck();         // Read Data 유효성 검사
    }
    else
    {   // EEPROM에 에러가 없으면
        EEPROM_Data_Load();             // EEPROM Data Read
        EEPROM_ReadDataCheck();         // Read Data 유효성 검사
    }
}


/// @brief      EEPROM 초기상태 Data 설정
/// @details    EEPROM의 초기값들을 Write 시킨다
/// @param      void
/// @return     void
void EEPROM_Initial(void)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < EEPROM_ID_MAX ; mu8i++)
    {
        Set_EEPROM_Data(mu8i, gu8EepromInitialValue[mu8i]);     // 데이터 입력 및 저장
    }
}


/// @brief      EEPROM Read Data 유효성 검사(User가 구성한 코드에 따라 코드 구성)
/// @details    USER가 사용하는 정전보상 데이터들의 유효성 검사를 구성한다
/// @param      void
/// @return     void
void EEPROM_ReadDataCheck(void)
{
/*
    U8 mu8EEPROM_DataBuf = 0;

    // 코드 예시 - 냉수 기능 상태
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
    {   // 유효값을 벗어난 경우 해당 데이터 초기화
        Setup_ColdOnOff(ON);
        Set_EEPROM_Data(EEPROM_ID_COLD_ON_OFF, ON);
    }
*/

#if (MEMENTO_MODE_USE == USE)
    MementoRead();      // 메멘토 데이터 Read
#endif
}


#endif

