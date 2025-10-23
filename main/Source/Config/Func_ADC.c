/// @file     Func_ADC.c
/// @date     2025/05/12
/// @author   Jaejin Ham, Hyunho Kang
/// @brief    ADC Interrupt ���� �Լ�  File


#include "Global_Header.h"

#if (ADC_USE == USE)

/// @brief      r_Config_ADC_interrupt() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰��� �Լ�
/// @details    ADC Interrupt�� �߻��� �� ������ ó���Ѵ�
/// @param      void
/// @return     void
void ADC_Interrupt(void)
{
    ADC_Stop();

    Set_ADC_EndStatus(SET);
}


#endif




