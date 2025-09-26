/// @file   Lib_TimeScheduler.h
/// @date
/// @author Jaejin Ham
/// @brief  Time Scheduler files


#ifndef _LIB_TIMESCHEDULER_H_
#define _LIB_TIMESCHEDULER_H_


/*
사용 방법 예시>

void main(void)
{
  SetupTimeScheduler(exFunction, 100, ACT_COUNT_INFINITE);                      // 100ms 마다 exFunction 함수가 호출되는 스레드 신규 등록

  StopTimeScheduler(exFunction);                                                // 등록된 스레드 동작 정지
  StartTimeScheduler(exFunction, 200, ACT_COUNT_1_TIME);                        // 200ms 후 1회 수행 함수가 호출되고 정지됨
  DeleteTimeScheduler(exFunction);                                              // 등록된 스레드 정보 삭제


  Setup1msTimeInterruptScheduler(exFunction2, 100, ACT_COUNT_INFINITE);         // 시간이 밀리지   않도록 Interrupt 루딘 안에서 100ms 마다 exFunction2 함수가 호출되는 스레드 신규 등록

  while(1)
  {
    GoTimeScheduler();          // Main 함수에서 실제 함수를 시간에 맞게 호출해 주는 함수
  }
}


interrupt timer_1ms(void)       // 1ms 타이머 인터럽트
{
  CounterTimeScheduler();       // 타이머 인터럽트에 넣어줘야 함
}

// SetupTimerscheduler의 최대 설정 가능 시간은 1분(60000)까지만 가능

*/

#define MAX_FUNCTION_POINTER                    20          // Limit the maximum number of schedulers to 20
#define MAX_FUNCTION_INTP_POINTER               5           // 5 schedulers handled directly within 1ms interrupt


#define ACT_COUNT_INFINITE                      0           // 계속 수행
#define ACT_COUNT_1_TIME                        1           // 1회만 수행되고 수행 정지
#define ACT_COUNT_2_TIME                        2           // 2회만 수행되고 수행 정지


void CounterTimeScheduler(void);
void InitializeTimeScheduler(void);

// While 문 안에서 처리되는 Scheduler ***************************************************
void GoTimeScheduler(void);
void SetupTimeScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount);
void StartTimeScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount);
void StopTimeScheduler(void (*tFuction)(void));
void DeleteTimeScheduler(void (*tFuction)(void));

// 1ms Timer Interrput 안에서 직접 처리되는 Scheduler ***********************************
void Go1msTimeInterrputScheduler(void);
void Setup1msTimeInterruptScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount);
void Start1msTimeInterruptScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount);
void Stop1msTimeInterruptScheduler(void (*tFuction)(void));
void Delete1msTimeScheduler(void (*tFuction)(void));

void Lib_TimeScheduler_Module_Initialize(void);
void Lib_TimeScheduler_Module_1ms_Control(void);
void Lib_TimeScheduler_Module_Control(void);

#endif  // #ifndef ___LIB_TIMESCHEDULER_H___

