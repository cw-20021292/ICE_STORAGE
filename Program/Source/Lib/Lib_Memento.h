/// @file   Lib_Memento.h
/// @date   2025/06/25
/// @author Jaejin Ham
/// @brief  Memento 동작 제어 파일

#ifndef _LIB_MEMENTO_H_
#define _LIB_MEMENTO_H_


/*  Lib Memento Function 사용법
    => void MementoRead();                          // 메멘토 저장값을 Read함
    => Get_MementoData(mu8ID);                      // ID에 해당하는 메멘토 데이터값 Read
    => Get_MementoDataLast();                       // 가장 최근에 저장된 메멘토값 확인
    => MementoWrite(mu8ErrorCode);                  // 발생된 에러를 메멘토에 기억함
*/


#define MEMENTO_MODE_USE                            USE     // 메멘토 모드 사용 여부 - 0(NO_USE), 1(USE)

#if (MEMENTO_MODE_USE == USE)

/// @brief      메멘토 ID 구성 리스트
/// @details    메멘토 ID를 구조 리스트로 정리한다
typedef enum
{
    MEMENTO_ID_NONE = 0,                // 0 :
    MEMENTO_ID_1,                       // 1 :
    MEMENTO_ID_2,                       // 2 :
    MEMENTO_ID_3,                       // 3 :
    MEMENTO_ID_4,                       // 4 :
    MEMENTO_ID_5,                       // 5 :
    MEMENTO_ID_MAX                      // 6 :
} EMemontoID_T;


void MementoRead(void);
U8 Get_MementoData(U8 mu8ID);
U8 Get_MementoDataLast(void);
void MementoWrite(U8 mu8ErrorCode);

#endif

#endif

