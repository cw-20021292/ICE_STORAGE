/// @file     Func_External_Interrput.c
/// @date     2025/04/25
/// @author   Jaejin Ham
/// @brief    External Interrput ���� �Լ�  File


#include "Global_Header.h"


#if (TDS_IN_USE == USE)
/// @brief      �ܺ� ���ͷ�Ʈ�� �߰��� TDS In interrupt
/// @details    TDS IN ������ �����Ͽ� �ܺ� ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void External_Interrupt_TDS_In(void)
{
    TDS_In_Interrupt();
}
#endif


#if (FLOWMETER_COUNT >= 1)
/// @brief      �ܺ� ���ͷ�Ʈ�� �߰��� Flowmeter 1 interrupt
/// @details    Flowmeter 1 ������ �����Ͽ� �ܺ� ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_1(void)
{
    Flowmeter_1_Interrupt();
}
#endif


#if (FLOWMETER_COUNT >= 2)
/// @brief      �ܺ� ���ͷ�Ʈ�� �߰��� Flowmeter 2 interrupt
/// @details    Flowmeter 2 ������ �����Ͽ� �ܺ� ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_2(void)
{
    Flowmeter_2_Interrupt();
}
#endif

#if (FLOWMETER_COUNT >= 3)
/// @brief      �ܺ� ���ͷ�Ʈ�� �߰��� Flowmeter 3 interrupt
/// @details    Flowmeter 3 ������ �����Ͽ� �ܺ� ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_3(void)
{
    Flowmeter_3_Interrupt();
}
#endif

#if (FLOWMETER_COUNT >= 4)
/// @brief      �ܺ� ���ͷ�Ʈ�� �߰��� Flowmeter 4 interrupt
/// @details    Flowmeter 4 ������ �����Ͽ� �ܺ� ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_4(void)
{
    Flowmeter_4_Interrupt();
}
#endif

#if (FLOWMETER_COUNT >= 5)
/// @brief      �ܺ� ���ͷ�Ʈ�� �߰��� Flowmeter 5 interrupt
/// @details    Flowmeter 5 ������ �����Ͽ� �ܺ� ���ͷ�Ʈ�� �߻����� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_5(void)
{
    Flowmeter_5_Interrupt();
}
#endif



