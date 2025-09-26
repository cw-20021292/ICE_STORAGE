/// @file     Func_External_Interrput.h
/// @date     2025/04/25
/// @author   Jaejin Ham
/// @brief    External Interrput 동작 함수  File

#ifndef _FUNC_EXT_INTERRUPT_H_
#define _FUNC_EXT_INTERRUPT_H_


/*
    User가 외부 인터럽트 사용시 사용하는 함수들
    => External_Interrupt_TDS_In();             // TDS In 센서 사용시 설정된 외부 인터럽트 수행 함수 안에 추가할 함수
    => External_Interrupt_Flowmeter_1();        // 유량센서1 사용시 설정된 외부 인터럽트 수행 함수 안에 추가할 함수
    => External_Interrupt_Flowmeter_2();        // 유량센서2 사용시 설정된 외부 인터럽트 수행 함수 안에 추가할 함수
    => External_Interrupt_Flowmeter_3();        // 유량센서3 사용시 설정된 외부 인터럽트 수행 함수 안에 추가할 함수
    => External_Interrupt_Flowmeter_4();        // 유량센서4 사용시 설정된 외부 인터럽트 수행 함수 안에 추가할 함수
    => External_Interrupt_Flowmeter_5();        // 유량센서5 사용시 설정된 외부 인터럽트 수행 함수 안에 추가할 함수
*/


#if (TDS_IN_USE == USE)       // TDS IN 센서 사용시
void External_Interrupt_TDS_In(void);
#endif

#if (FLOWMETER_COUNT >= 1)  // 유량센서 1 사용시
void External_Interrupt_Flowmeter_1(void);
#endif

#if (FLOWMETER_COUNT >= 2)  // 유량센서 2 사용시
void External_Interrupt_Flowmeter_2(void);
#endif

#if (FLOWMETER_COUNT >= 3)  // 유량센서 3 사용시
void External_Interrupt_Flowmeter_3(void);
#endif

#if (FLOWMETER_COUNT >= 4)  // 유량센서 4 사용시
void External_Interrupt_Flowmeter_4(void);
#endif

#if (FLOWMETER_COUNT >= 5)  // 유량센서 5 사용시
void External_Interrupt_Flowmeter_5(void);
#endif


#endif

