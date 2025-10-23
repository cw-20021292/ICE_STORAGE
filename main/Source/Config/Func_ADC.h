/// @file     Func_ADC.h
/// @date     2025/05/12
/// @author   Jaejin Ham, Hyunho Kang
/// @brief    ADC Interrupt 동작 함수  File


#ifndef _FUNC_ADC_H_
#define _FUNC_ADC_H_


/*
    User가 ADC 사용시 추가할 함수
    => ADC_Interrupt();     // r_Config_ADC_interrupt() 함수 안의 코드를 모두 삭제하고 대신에 추가
*/


#if (ADC_USE == USE)

void ADC_Interrupt(void);

#endif


#endif

