/// @file     Func_ADC.h
/// @date     2025/05/12
/// @author   Jaejin Ham, Hyunho Kang
/// @brief    ADC Interrupt ���� �Լ�  File


#ifndef _FUNC_ADC_H_
#define _FUNC_ADC_H_


/*
    User�� ADC ���� �߰��� �Լ�
    => ADC_Interrupt();     // r_Config_ADC_interrupt() �Լ� ���� �ڵ带 ��� �����ϰ� ��ſ� �߰�
*/


#if (ADC_USE == USE)

void ADC_Interrupt(void);

#endif


#endif

