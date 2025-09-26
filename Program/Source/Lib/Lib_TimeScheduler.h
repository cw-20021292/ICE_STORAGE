/// @file   Lib_TimeScheduler.h
/// @date
/// @author Jaejin Ham
/// @brief  Time Scheduler files


#ifndef _LIB_TIMESCHEDULER_H_
#define _LIB_TIMESCHEDULER_H_


/*
��� ��� ����>

void main(void)
{
  SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);                      // 100ms ���� exFunction �Լ��� ȣ��Ǵ� ������ �ű� ���

  StopTimeScheduler(exFunction);                                                // ��ϵ� ������ ���� ����
  StartTimeScheduler(exFunction, 200, ACT_COUNT_1_TIME);                        // 200ms �� 1ȸ ���� �Լ��� ȣ��ǰ� ������
  DeleteTimeScheduler(exFunction);                                              // ��ϵ� ������ ���� ����


  Setup1msTimeInterruptScheduler(exFunction2, 100, ACT_COUNT_INFINITE);         // �ð��� �и���   �ʵ��� Interrupt ��� �ȿ��� 100ms ���� exFunction2 �Լ��� ȣ��Ǵ� ������ �ű� ���

  while(1)
  {
    GoTimeScheduler();          // Main �Լ����� ���� �Լ��� �ð��� �°� ȣ���� �ִ� �Լ�
  }
}


interrupt timer_1ms(void)       // 1ms Ÿ�̸� ���ͷ�Ʈ
{
  CounterTimeScheduler();       // Ÿ�̸� ���ͷ�Ʈ�� �־���� ��
}

// SetupTimerscheduler�� �ִ� ���� ���� �ð��� 1��(60000)������ ����

*/

#define MAX_FUNCTION_POINTER                    20          // Limit the maximum number of schedulers to 20
#define MAX_FUNCTION_INTP_POINTER               5           // 5 schedulers handled directly within 1ms interrupt


#define ACT_COUNT_INFINITE                      0           // ��� ����
#define ACT_COUNT_1_TIME                        1           // 1ȸ�� ����ǰ� ���� ����
#define ACT_COUNT_2_TIME                        2           // 2ȸ�� ����ǰ� ���� ����


void CounterTimeScheduler(void);
void InitializeTimeScheduler(void);

// While �� �ȿ��� ó���Ǵ� Scheduler ***************************************************
void GoTimeScheduler(void);
void SetupTimeScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount);
void StartTimeScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount);
void StopTimeScheduler(void (*tFuction)(void));
void DeleteTimeScheduler(void (*tFuction)(void));

// 1ms Timer Interrput �ȿ��� ���� ó���Ǵ� Scheduler ***********************************
void Go1msTimeInterrputScheduler(void);
void Setup1msTimeInterruptScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount);
void Start1msTimeInterruptScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount);
void Stop1msTimeInterruptScheduler(void (*tFuction)(void));
void Delete1msTimeScheduler(void (*tFuction)(void));

void Lib_TimeScheduler_Module_Initialize(void);
void Lib_TimeScheduler_Module_1ms_Control(void);
void Lib_TimeScheduler_Module_Control(void);

#endif  // #ifndef ___LIB_TIMESCHEDULER_H___

