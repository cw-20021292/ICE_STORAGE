/// @file     Func_External_Interrput.c
/// @date     2025/04/25
/// @author   Jaejin Ham
/// @brief    External Interrput 동작 함수  File


#include "Global_Header.h"


#if (TDS_IN_USE == USE)
/// @brief      외부 인터럽트에 추가할 TDS In interrupt
/// @details    TDS IN 센서와 관련하여 외부 인터럽트가 발생했을 때 동작을 처리한다
/// @param      void
/// @return     void
void External_Interrupt_TDS_In(void)
{
    TDS_In_Interrupt();
}
#endif


#if (FLOWMETER_COUNT >= 1)
/// @brief      외부 인터럽트에 추가할 Flowmeter 1 interrupt
/// @details    Flowmeter 1 센서와 관련하여 외부 인터럽트가 발생했을 때 동작을 처리한다
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_1(void)
{
    Flowmeter_1_Interrupt();
}
#endif


#if (FLOWMETER_COUNT >= 2)
/// @brief      외부 인터럽트에 추가할 Flowmeter 2 interrupt
/// @details    Flowmeter 2 센서와 관련하여 외부 인터럽트가 발생했을 때 동작을 처리한다
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_2(void)
{
    Flowmeter_2_Interrupt();
}
#endif

#if (FLOWMETER_COUNT >= 3)
/// @brief      외부 인터럽트에 추가할 Flowmeter 3 interrupt
/// @details    Flowmeter 3 센서와 관련하여 외부 인터럽트가 발생했을 때 동작을 처리한다
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_3(void)
{
    Flowmeter_3_Interrupt();
}
#endif

#if (FLOWMETER_COUNT >= 4)
/// @brief      외부 인터럽트에 추가할 Flowmeter 4 interrupt
/// @details    Flowmeter 4 센서와 관련하여 외부 인터럽트가 발생했을 때 동작을 처리한다
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_4(void)
{
    Flowmeter_4_Interrupt();
}
#endif

#if (FLOWMETER_COUNT >= 5)
/// @brief      외부 인터럽트에 추가할 Flowmeter 5 interrupt
/// @details    Flowmeter 5 센서와 관련하여 외부 인터럽트가 발생했을 때 동작을 처리한다
/// @param      void
/// @return     void
void External_Interrupt_Flowmeter_5(void)
{
    Flowmeter_5_Interrupt();
}
#endif



