/// @file   Lib_Memento.h
/// @date   2025/06/25
/// @author Jaejin Ham
/// @brief  Memento ���� ���� ����

#ifndef _LIB_MEMENTO_H_
#define _LIB_MEMENTO_H_


/*  Lib Memento Function ����
    => void MementoRead();                          // �޸��� ���尪�� Read��
    => Get_MementoData(mu8ID);                      // ID�� �ش��ϴ� �޸��� �����Ͱ� Read
    => Get_MementoDataLast();                       // ���� �ֱٿ� ����� �޸��䰪 Ȯ��
    => MementoWrite(mu8ErrorCode);                  // �߻��� ������ �޸��信 �����
*/


#define MEMENTO_MODE_USE                            USE     // �޸��� ��� ��� ���� - 0(NO_USE), 1(USE)

#if (MEMENTO_MODE_USE == USE)

/// @brief      �޸��� ID ���� ����Ʈ
/// @details    �޸��� ID�� ���� ����Ʈ�� �����Ѵ�
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

