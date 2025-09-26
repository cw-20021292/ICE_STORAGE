/// @file     Basic_Loop.h
/// @date     2025/02/25
/// @author   Jaejin Ham
/// @brief    �⺻ ���� �Լ�  File

#ifndef _BASIC_Loop_H_
#define _BASIC_Loop_H_


/*
    User�� �ݵ�� �ڱ� �ڵ忡 �߰��ؾ� �� �Լ���
    => Base_Initialize();                   // Main ������ While�� �����ϱ� �� ��� ���� �Լ����� �ʱ�ȭ �ϴ� �Լ�
    => Base_Timer_1ms();                    // 1ms Timer Interrupt �ȿ� User�� ���� �־�� �ϴ� �Լ�
    => Base_Timer_100us();                  // 100us Timer Interrupt �ȿ� User�� ���� �־�� �ϴ� �Լ�
    => FunctionProcess_In_WhileLoop();      // HW�� ����Ǵ� �Լ��� User�� While�� �ȿ� ���� �־�� �ϴ� �Լ�
    => u8Return = Get_BootComplete();       // ��ǰ ���� ���� �ð��� �Ϸ�Ǿ����� Ȯ���ϴ� �Լ�

    ** �׿� �Լ����� User�� Start ���Ѿ� �ϴ� Timer, Interrupt�� ���õ� Start �ڵ����
    => Interrupt_Start();                   // Main ������ While�� �����ϱ� �� ��� Interrupt ������ ���� ��Ű�� �Լ�
    => Serial_Start();                      // Main ������ While�� �����ϱ� �� ��� Serial(Interrupt) ������ ���� ��Ű�� �Լ�
*/

#define WDT_Restart()                   R_Config_WDT_Restart();       // Code Generate�� WDT Restart �Լ� ��ġ
#define Timer_1ms_Start()               R_Config_TAU1_0_Start();
#define Timer_100us_Start()             R_Config_TAU1_1_Start();

#define BOOT_TIME                       2000                // Boot �ð� delay 2sec@1ms

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

