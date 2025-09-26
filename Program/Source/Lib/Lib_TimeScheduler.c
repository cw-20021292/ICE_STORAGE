/// @file   Lib_TimeScheduler.c"
/// @date
/// @author Jaejin Ham
/// @brief  Time Scheduler files

#include "Global_header.h"


/// @brief  Time �����췯 ���� �ڷ���
typedef struct {
    void    (*FunctionPointer[MAX_FUNCTION_POINTER])(void);     // �Լ� ������ �Է�
    U16     mu16Counter[MAX_FUNCTION_POINTER];                  // �Լ� ���� �ֱ� ī����, ī��Ʈ �Ǹ鼭 �ֱ������� �Լ��� ������. Counter > CallTimeCycle���� ���µ�
    U16     mu16CallTimeCycle[MAX_FUNCTION_POINTER];            // �Լ� ���� �ֱ� �����
    U8      mu8ActCount[MAX_FUNCTION_POINTER];                  // ���� Ƚ�� ������ �ִ��� üũ ����
}   typeTimeSchedule;


/// @brief  Time �����췯 ���� �ڷ���2
typedef struct {
    void    (*FunctionPointer[MAX_FUNCTION_INTP_POINTER])(void);     // �Լ� ������ �Է�
    U16     mu16Counter[MAX_FUNCTION_INTP_POINTER];                  // �Լ� ���� �ֱ� ī����, ī��Ʈ �Ǹ鼭 �ֱ������� �Լ��� ������. Counter > CallTimeCycle���� ���µ�
    U16     mu16CallTimeCycle[MAX_FUNCTION_INTP_POINTER];            // �Լ� ���� �ֱ� �����
    U8      mu8ActCount[MAX_FUNCTION_INTP_POINTER];                  // ���� Ƚ�� ������ �ִ��� üũ ����
}   type1msTimeInterruptSchedule;


typeTimeSchedule tTimeSceduleHandler;                           // Ÿ�� �����췯 ����(While������ ����)
type1msTimeInterruptSchedule t1msTimerInterruptHandler;         // 1ms Timer Interrput �ȿ��� ���� ����(������ ������ �ʵ��� �ʿ�ø� ����ϵ� ó�� ��ƾ�� ���� ���� �Լ� ��ġ ���)



/// @brief      1ms TimerInterrupt���� ����Ǿ� �����췯�� ���� �ð��� �Ǵ� �Լ�
/// @details    1ms Interrupt �ȿ��� ���������� �����췯�� Timer�� ����Ѵ�
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

    Go1msTimeInterrputScheduler();      // 1ms Timer Interrput �ȿ��� ���� ���� ó�� Scheduler
}


/// @brief      Time Scheduler Initialize
/// @details    Time �����췯 ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void InitializeTimeScheduler(void)
{
    U8 mu8i = 0;
    U8 mu8j = 0;

// While �� �ȿ��� ó��
    for (mu8i = 0 ; mu8i < MAX_FUNCTION_POINTER ; mu8i++)
    {
        tTimeSceduleHandler.FunctionPointer[mu8i] = NULL;
        tTimeSceduleHandler.mu16Counter[mu8i] = 1;             // CallTimeCycle�� �񱳸� ���� 1�� ����
        tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
        tTimeSceduleHandler.mu8ActCount[mu8i] = 0;
    }

// 1ms Interrput �ȿ��� ó��
    for (mu8j = 0 ; mu8j < MAX_FUNCTION_INTP_POINTER ; mu8j++)
    {
        t1msTimerInterruptHandler.FunctionPointer[mu8j] = NULL;
        t1msTimerInterruptHandler.mu16Counter[mu8j] = 1;             // CallTimeCycle�� �񱳸� ���� 1�� ����
        t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j] = 0;
        t1msTimerInterruptHandler.mu8ActCount[mu8j] = 0;
    }
}


/// @brief      Main �Լ� While ���ȿ��� ����Ǹ� ���� �Լ��� ȣ���� �ִ� �Լ�
/// @details    Time �����췯�� �����ϸ鼭 �ش� �ð��� �����ϴ� �Լ����� ȣ���Ѵ�
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
                {   // �Լ� ���� ���� Ƚ���� �ִ� ���
                    tTimeSceduleHandler.mu8ActCount[mu8i]--;     // ���� Ƚ�� ����

                    if (tTimeSceduleHandler.mu8ActCount[mu8i] == 0)
                    {       // ���� Ƚ���� ��� ������ ��� ���̻� ������� �ʰ� ó��
                        tTimeSceduleHandler.mu16CallTimeCycle[mu8i] = 0;
                    }
                }
            }
        }
    }
}




/// @brief      �����췯�� �Լ��� ��� �ϴ� �Լ�
/// @details    Time �����췯�� �Լ��� ȣ�� �ֱ⸦ �����Ѵ�
/// @param      *tFuction : ����� �Լ� ������
///             mu16Timer : ������ �ֱ�(�ð�, 1ms ����)
///             mu8OneTimeCheck : 1ȸ�� �����ϴ� �Լ����� üũ ����(0�̸� ��� ����)
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


/// @brief      ������ �Լ��� ȣ���� �ٽ� ���� �ϴ� �Լ�
/// @details    ������ �Լ��� �ֱ������� �ٽ� ���۽�Ű���� �����ϰ� �Լ��� ȣ�� �ð��� �����ϰ��� �� ���� ���ȴ�
/// @param      *tFuction : ������ �Լ� ������
///             mu16Timer : ������ �ֱ�(�ð�, 1ms ����)
///             mu8OneTimeCheck : 1ȸ�� �����ϴ� �Լ����� üũ ����(0�̸� ��� ����)
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


/// @brief      ��ϵ� �Լ��� ȣ���� ���� �ϴ� �Լ�
/// @details    ��ϵ� �Լ��� ȣ���� ������Ų��(��ϵ� �Լ� ���� �����ʹ� ���������� �ʴ´�)
/// @param      *tFuction : ������ �Լ� ������
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


/// @brief      ��ϵ� �Լ��� ���õ� Time ������ �����͸� �����ϴ� �Լ�
/// @details    �����췯�� ��ϵ� �Լ��� �ش�Ǵ� �����͸� ã�Ƽ� ���������� ��� �����Ѵ�
/// @param      *tFuction : ������ �Լ� ������
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


/// @brief      1ms Interrupt �ȿ��� ���� ����Ǹ� �Լ��� ȣ���� �ִ� �Լ�
/// @details    1ms Interrupt �ȿ��� ȣ�� �ð� ī��Ʈ �� �Լ� ȣ����� �����Ѵ�
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
                {   // �Լ� ���� ���� Ƚ���� �ִ� ���
                    t1msTimerInterruptHandler.mu8ActCount[mu8j]--;     // ���� Ƚ�� ����

                    if (t1msTimerInterruptHandler.mu8ActCount[mu8j] == 0)
                    {       // ���� Ƚ���� ��� ������ ��� ���̻� ������� �ʰ� ó��
                        t1msTimerInterruptHandler.mu16CallTimeCycle[mu8j] = 0;
                    }
                }
            }
        }
    }
}


/// @brief      �����췯�� �Լ��� ��� �ϴ� �Լ�
/// @details    1ms Interrupt �ȿ��� ����Ǵ� �����췯�� ȣ��Ǵ� ������ �����Ѵ�
/// @param      *tFuction : ����� �Լ� ������
///             mu16Timer : ������ �ֱ�(�ð�, 1ms ����)
///             mu8OneTimeCheck : 1ȸ�� �����ϴ� �Լ����� üũ ����(0�̸� ��� ����)
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


/// @brief      ������ �Լ��� ȣ���� �ٽ� ���� �ϴ� �Լ�
/// @details    1ms Interrupt �ȿ��� �����ϴ� �Լ��� ȣ���� �ٽ� �����ϰ� �Լ��� ȣ�� �ð��� �����ϰ��� �� ���� ���ȴ�
/// @param      *tFuction : ������ �Լ� ������
///             mu16Timer : ������ �ֱ�(�ð�, 1ms ����)
///             mu8OneTimeCheck : 1ȸ�� �����ϴ� �Լ����� üũ ����(0�̸� ��� ����)
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


/// @brief      ��ϵ� �Լ��� ȣ���� ���� �ϴ� �Լ�
/// @details    1ms Interrupt �ȿ��� ȣ��Ǵ� �Լ��� ȣ���� ������Ų��(��ϵ� �Լ� ���� �����ʹ� ���������� �ʴ´�)
/// @param      *tFuction : ������ �Լ� ������
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


/// @brief      ��ϵ� �Լ��� ���õ� Time ������ �����͸� �����ϴ� �Լ�
/// @details    1ms Interrupt �����췯�� ��ϵ� �Լ��� �ش�Ǵ� �����͸� ã�Ƽ� ������ ��� �����Ѵ�
/// @param      *tFuction : ������ �Լ� ������
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
/// @details    Time Scheduler ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_TimeScheduler_Module_Initialize(void)
{
    InitializeTimeScheduler();              // Time Scheduler ���� �ʱ�ȭ
}


/// @brief      Lib Time Scheduler Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� Time Scheduler ���� ��� Library
/// @param      void
/// @return     void
void Lib_TimeScheduler_Module_1ms_Control(void)
{
    CounterTimeScheduler();                 // �����췯�� ���� �ð� üũ  �Լ�
}


/// @brief      Lib Time Scheduler Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Time Scheduler ���� ��� Library
/// @param      void
/// @return     void
void Lib_TimeScheduler_Module_Control(void)
{
    GoTimeScheduler();
}




