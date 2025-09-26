/// @file     Basic_Loop.h
/// @date     2025/02/25
/// @author   Jaejin Ham
/// @brief    기본 동작 함수  File

#ifndef _BASIC_Loop_H_
#define _BASIC_Loop_H_


/*
    User가 반드시 자기 코드에 추가해야 할 함수들
    => Base_Initialize();                   // Main 문에서 While에 진입하기 전 모든 동작 함수들을 초기화 하는 함수
    => Base_Timer_1ms();                    // 1ms Timer Interrupt 안에 User가 직접 넣어야 하는 함수
    => Base_Timer_100us();                  // 100us Timer Interrupt 안에 User가 직접 넣어야 하는 함수
    => FunctionProcess_In_WhileLoop();      // HW가 제어되는 함수로 User가 While문 안에 직접 넣어야 하는 함수
    => u8Return = Get_BootComplete();       // 제품 부팅 동작 시간이 완료되었는지 확인하는 함수

    ** 그외 함수들은 User가 Start 시켜야 하는 Timer, Interrupt와 관련된 Start 코드들임
    => Interrupt_Start();                   // Main 문에서 While에 진입하기 전 모든 Interrupt 동작을 시작 시키는 함수
    => Serial_Start();                      // Main 문에서 While에 진입하기 전 모든 Serial(Interrupt) 동작을 시작 시키는 함수
*/

#define WDT_Restart()                   R_Config_WDT_Restart();       // Code Generate된 WDT Restart 함수 배치
#define Timer_1ms_Start()               R_Config_TAU1_0_Start();
#define Timer_100us_Start()             R_Config_TAU1_1_Start();

#define BOOT_TIME                       2000                // Boot 시간 delay 2sec@1ms

void Base_Initialize(void);
void Base_Timer_1ms(void);
void Base_Timer_100us(void);
void FunctionProcess_In_WhileLoop(void);

void Boot_Initialize(void);
void BootTimeCheck(void);
U16 Get_BootTime(void);
U8 Get_BootComplete(void);

void Interrupt_Start(void);
void Serial_Start(void);


#endif

