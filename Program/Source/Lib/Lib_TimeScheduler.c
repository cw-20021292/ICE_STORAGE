/// @file   Lib_TimeScheduler.c"
/// @date
/// @author Jaejin Ham
/// @brief  Time Scheduler files

#include "Global_header.h"


/// @brief  Time 스케쥴러 관리 자료형
typedef struct {
    void    (*FunctionPointer[MAX_FUNCTION_POINTER])(void);     // 함수 포인터 입력
    U16     mu16Counter[MAX_FUNCTION_POINTER];                  // 함수 실행 주기 카운터, 카운트 되면서 주기적으로 함수를 실행함. Counter > CallTimeCycle에서 리셋됨
    U16     mu16CallTimeCycle[MAX_FUNCTION_POINTER];            // 함수 실행 주기 저장소
    U8      mu8ActCount[MAX_FUNCTION_POINTER];                  // 수행 횟수 제한이 있는지 체크 변수
}   typeTimeSchedule;


/// @brief  Time 스케쥴러 관리 자료형2
typedef struct {
    void    (*FunctionPointer[MAX_FUNCTION_INTP_POINTER])(void);     // 함수 포인터 입력
    U16     mu16Counter[MAX_FUNCTION_INTP_POINTER];                  // 함수 실행 주기 카운터, 카운트 되면서 주기적으로 함수를 실행함. Counter > CallTimeCycle에서 리셋됨
    U16     mu16CallTimeCycle[MAX_FUNCTION_INTP_POINTER];            // 함수 실행 주기 저장소
    U8      mu8ActCount[MAX_FUNCTION_INTP_POINTER];                  // 수행 횟수 제한이 있는지 체크 변수
}   type1msTimeInterruptSchedule;


typeTimeSchedule tTimeSceduleHandler;                           // 타임 스케쥴러 변수(While문에서 수행)
type1msTimeInterruptSchedule t1msTimerInterruptHandler;         // 1ms Timer Interrput 안에서 직접 수행(문제가 생기지 않도록 필요시만 사용하되 처리 루틴이 길지 않은 함수 배치 요망)



/// @brief      1ms TimerInterrupt에서 실행되어 스케쥴러의 기준 시간이 되는 함수
/// @details    1ms Interrupt 안에서 실제적으로 스케쥴러의 Timer를 계산한다
/// @param      void
/// @return     void
void CounterTimeScheduler(void)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        if (tTimeSceduleHandler.mu16CallTimeCycle[mu8i] != 0)
        {
            if (tTimeSceduleHandler.mu16Counter[mu8i] <= tTimeSceduleHandler.mu16CallTimeCycle[mu8i])
            {
                tTimeSceduleHandler.mu16Counter[mu8i]++;
            }
        }
    }

    Go1msTimeInterrputScheduler();      // 1ms Timer Interrput 안에서 직접 수행 처리 Scheduler
}


/// @brief      Time Scheduler Initialize
/// @details    Time 스케쥴러 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void InitializeTimeScheduler(void)
{
    U8 mu8i = 0;
    U8 mu8j = 0;

// While 문 안에서 처리
    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        tTimeSceduleHandler.FunctionPointer[mu8i] = NULL;
        tTimeSceduleHandler.mu16Counter[mu8i] = 1;             // CallTimeCycle과 비교를 위해 1로 세팅
        tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
        tTimeSceduleHandler.mu8ActCount[mu8i] = 0;
    }

// 1ms Interrput 안에서 처리
    for (mu8j = 0 ; mu8j < MAX_FUNCTION_INTP_POINTER ; mu8j++)
    {
        t1msTimerInterruptHandler.FunctionPointer[mu8j] = NULL;
        t1msTimerInterruptHandler.mu16Counter[mu8j] = 1;             // CallTimeCycle과 비교를 위해 1로 세팅
        t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j] = 0;
        t1msTimerInterruptHandler.mu8ActCount[mu8j] = 0;
    }
}


/// @brief      Main 함수 While 문안에서 실행되며 실제 함수를 호출해 주는 함수
/// @details    Time 스케쥴러를 제어하면서 해당 시간에 동작하는 함수들을 호출한다
/// @param      void
/// @return     void
void GoTimeScheduler(void)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        if (tTimeSceduleHandler.mu16CallTimeCycle[mu8i] != 0)
        {
            if (tTimeSceduleHandler.mu16Counter[mu8i] > tTimeSceduleHandler.mu16CallTimeCycle[mu8i])
            {
                tTimeSceduleHandler.mu16Counter[mu8i] = 1;
                tTimeSceduleHandler.FunctionPointer[mu8i]();

                if (tTimeSceduleHandler.mu8ActCount[mu8i])
                {   // 함수 수행 제한 횟수가 있는 경우
                    tTimeSceduleHandler.mu8ActCount[mu8i]--;     // 수행 횟수 차감

                    if (tTimeSceduleHandler.mu8ActCount[mu8i] == 0)
                    {       // 수행 횟수가 모두 차감된 경우 더이상 수행되지 않게 처리
                        tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
                    }
                }
            }
        }
    }
}




/// @brief      스케쥴러에 함수를 등록 하는 함수
/// @details    Time 스케쥴러에 함수와 호출 주기를 설정한다
/// @param      *tFuction : 등록할 함수 포인터
///             mu16Timer : 실행할 주기(시간, 1ms 단위)
///             mu8OneTimeCheck : 1회만 수행하는 함수인지 체크 변수(0이면 계속 수행)
/// @return     void
void SetupTimeScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (tTimeSceduleHandler.FunctionPointer[mu8i] == NULL)
        {
            tTimeSceduleHandler.FunctionPointer[mu8i] = tFuction;
            tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = mu16Timer;
            tTimeSceduleHandler.mu8ActCount[mu8i] = mu8ActionCount;
            return;
        }
    }
}


/// @brief      정지된 함수의 호출을 다시 시작 하는 함수
/// @details    정지된 함수의 주기적으로 다시 동작시키도록 설정하고 함수의 호출 시간을 변경하고자 할 때도 사용된다
/// @param      *tFuction : 시작할 함수 포인터
///             mu16Timer : 실행할 주기(시간, 1ms 단위)
///             mu8OneTimeCheck : 1회만 수행하는 함수인지 체크 변수(0이면 계속 수행)
/// @return     void
void StartTimeScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        if ( (tTimeSceduleHandler.FunctionPointer[mu8i] == tFuction) ||
            (tTimeSceduleHandler.FunctionPointer[mu8i] == NULL) )
        {
            tTimeSceduleHandler.FunctionPointer[mu8i] = tFuction;
            tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = mu16Timer;
            tTimeSceduleHandler.mu8ActCount[mu8i] = mu8ActionCount;
            return;
        }
    }
}


/// @brief      등록된 함수의 호출을 정지 하는 함수
/// @details    등록된 함수의 호출을 정지시킨다(등록된 함수 정보 데이터는 삭제되지는 않는다)
/// @param      *tFuction : 정지할 함수 포인터
/// @return     void
void StopTimeScheduler(void (*tFuction)(void))
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (tTimeSceduleHandler.FunctionPointer[mu8i] == tFuction)
        {
            tTimeSceduleHandler.mu16Counter[mu8i] = 1;
            tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
            tTimeSceduleHandler.mu8ActCount[mu8i] = 0;
            return;
        }
    }
}


/// @brief      등록된 함수와 관련된 Time 스케쥴 데이터를 삭제하는 함수
/// @details    스케쥴러에 등록된 함수에 해당되는 데이터를 찾아서 설정값들을 모두 삭제한다
/// @param      *tFuction : 삭제할 함수 포인터
/// @return      void
void DeleteTimeScheduler(void (*tFuction)(void))
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (tTimeSceduleHandler.FunctionPointer[mu8i] == tFuction)
        {
            tTimeSceduleHandler.FunctionPointer[mu8i] = NULL;
            tTimeSceduleHandler.mu16Counter[mu8i] = 1;
            tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
            tTimeSceduleHandler.mu8ActCount[mu8i] = 0;
            return;
        }
    }
}


/// @brief      1ms Interrupt 안에서 직접 실행되며 함수를 호출해 주는 함수
/// @details    1ms Interrupt 안에서 호출 시간 카운트 및 함수 호출까지 수행한다
/// @param      void
/// @return     void
void Go1msTimeInterrputScheduler(void)
{
    U8 mu8j = 0;

    for (mu8j = 0 ; mu8j < MAX_FUNCTION_INTP_POINTER ; mu8j++)
    {
        if (t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j] != 0)
        {
            if (t1msTimerInterruptHandler.mu16Counter[mu8j] <= t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j])
            {
                t1msTimerInterruptHandler.mu16Counter[mu8j]++;
            }

            if (t1msTimerInterruptHandler.mu16Counter[mu8j] > t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j])
            {
                t1msTimerInterruptHandler.mu16Counter[mu8j] = 1;
                t1msTimerInterruptHandler.FunctionPointer[mu8j]();

                if (t1msTimerInterruptHandler.mu8ActCount[mu8j])
                {   // 함수 수행 제한 횟수가 있는 경우
                    t1msTimerInterruptHandler.mu8ActCount[mu8j]--;     // 수행 횟수 차감

                    if (t1msTimerInterruptHandler.mu8ActCount[mu8j] == 0)
                    {       // 수행 횟수가 모두 차감된 경우 더이상 수행되지 않게 처리
                        t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j] = 0;
                    }
                }
            }
        }
    }
}


/// @brief      스케쥴러에 함수를 등록 하는 함수
/// @details    1ms Interrupt 안에서 수행되는 스케쥴러에 호출되는 값들을 설정한다
/// @param      *tFuction : 등록할 함수 포인터
///             mu16Timer : 실행할 주기(시간, 1ms 단위)
///             mu8OneTimeCheck : 1회만 수행하는 함수인지 체크 변수(0이면 계속 수행)
/// @return     void
void Setup1msTimeInterruptScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (t1msTimerInterruptHandler.FunctionPointer[mu8i] == NULL)
        {
            t1msTimerInterruptHandler.FunctionPointer[mu8i] = tFuction;
            t1msTimerInterruptHandler.mu16CallTimeCycle[mu8i] = mu16Timer;
            t1msTimerInterruptHandler.mu8ActCount[mu8i] = mu8ActionCount;
            return;
        }
    }
}


/// @brief      정지된 함수의 호출을 다시 시작 하는 함수
/// @details    1ms Interrupt 안에서 수행하는 함수의 호출을 다시 시작하고 함수의 호출 시간을 변경하고자 할 때도 사용된다
/// @param      *tFuction : 시작할 함수 포인터
///             mu16Timer : 실행할 주기(시간, 1ms 단위)
///             mu8OneTimeCheck : 1회만 수행하는 함수인지 체크 변수(0이면 계속 수행)
/// @return     void
void Start1msTimeInterruptScheduler(void (*tFuction)(void), U16 mu16Timer, U8 mu8ActionCount)
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        if ( (t1msTimerInterruptHandler.FunctionPointer[mu8i] == tFuction) ||
            (t1msTimerInterruptHandler.FunctionPointer[mu8i] == NULL) )
        {
            t1msTimerInterruptHandler.FunctionPointer[mu8i] = tFuction;
            t1msTimerInterruptHandler.mu16CallTimeCycle[mu8i] = mu16Timer;
            t1msTimerInterruptHandler.mu8ActCount[mu8i] = mu8ActionCount;
            return;
        }
    }
}


/// @brief      등록된 함수의 호출을 정지 하는 함수
/// @details    1ms Interrupt 안에서 호출되는 함수의 호출을 정지시킨다(등록된 함수 정보 데이터는 삭제되지는 않는다)
/// @param      *tFuction : 정지할 함수 포인터
/// @return     void
void Stop1msTimeInterruptScheduler(void (*tFuction)(void))
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (t1msTimerInterruptHandler.FunctionPointer[mu8i] == tFuction)
        {
            t1msTimerInterruptHandler.mu16Counter[mu8i] = 1;
            t1msTimerInterruptHandler.mu16CallTimeCycle[mu8i] = 0;
            t1msTimerInterruptHandler.mu8ActCount[mu8i] = 0;
            return;
        }
    }
}


/// @brief      등록된 함수와 관련된 Time 스케쥴 데이터를 삭제하는 함수
/// @details    1ms Interrupt 스케쥴러에 등록된 함수에 해당되는 데이터를 찾아서 값들을 모두 삭제한다
/// @param      *tFuction : 삭제할 함수 포인터
/// @return      void
void Delete1msTimeScheduler(void (*tFuction)(void))
{
    U8 mu8i = 0;

    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++ )
    {
        if (t1msTimerInterruptHandler.FunctionPointer[mu8i] == tFuction)
        {
            t1msTimerInterruptHandler.FunctionPointer[mu8i] = NULL;
            t1msTimerInterruptHandler.mu16Counter[mu8i] = 1;
            t1msTimerInterruptHandler.mu16CallTimeCycle[mu8i] = 0;
            t1msTimerInterruptHandler.mu8ActCount[mu8i] = 0;
            return;
        }
    }
}



// Lib Time Scheduler Module *********************************************************************************

/// @brief      Lib Time Scheduler Module Initilaize
/// @details    Time Scheduler 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_TimeScheduler_Module_Initialize(void)
{
    InitializeTimeScheduler();              // Time Scheduler 관련 초기화
}


/// @brief      Lib Time Scheduler Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 Time Scheduler 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_TimeScheduler_Module_1ms_Control(void)
{
    CounterTimeScheduler();                 // 스케쥴러의 기준 시간 체크  함수
}


/// @brief      Lib Time Scheduler Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Time Scheduler 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_TimeScheduler_Module_Control(void)
{
    GoTimeScheduler();
}




