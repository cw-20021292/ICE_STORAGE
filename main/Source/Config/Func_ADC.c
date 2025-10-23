/// @file     Func_ADC.c
/// @date     2025/05/12
/// @author   Jaejin Ham, Hyunho Kang
/// @brief    ADC Interrupt 동작 함수  File


#include "Global_Header.h"

#if (ADC_USE == USE)

/// @brief      r_Config_ADC_interrupt() 함수 안의 코드를 모두 삭제하고 대신에 추가할 함수
/// @details    ADC Interrupt가 발생될 때 동작을 처리한다
/// @param      void
/// @return     void
void ADC_Interrupt(void)
{
    ADC_Stop();

    Set_ADC_EndStatus(SET);
}


#endif




