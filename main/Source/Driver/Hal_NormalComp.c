/// @file     Hal_NormalComp.c
/// @date     2025/02/18
/// @author   Jaejin Ham
/// @brief    Normal Comp Control file


#include "Global_Header.h"


#if (NORMAL_COMP_USE == USE)

U8 gu8CompControlTimer = 0;                         /// @brief  ������ Comp ���� �ֱ� �ð� Ÿ�̸�
U8 gu8CompStatus = 0;                               /// @brief  ������ Comp ���� ���°�
U8 gu8CompDelayTime = 0;                            /// @brief  ������ Comp ���� ���� �� Delay �ð�


/// @brief      ������ Comp ���� ���� �ʱ�ȭ
/// @details    ������ Comp ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Comp_Initialize(void)
{
    gu8CompControlTimer = 0;
    gu8CompStatus = 0;
    gu8CompDelayTime = 0;

    Set_Comp(OFF, 0);
}



/// @brief      ������ Comp ���� �ð� Ÿ�̸�(@1ms interrupt)
/// @details    1ms ���ͷ�Ʈ �ȿ��� ������ Comp ���� �ð� Ÿ�̸Ӹ� ī��Ʈ�Ѵ�
/// @param      void
/// @return     void
void CompControlTimer(void)
{
    if (gu8CompControlTimer < COMP_CONTROL_TIME_PERIOD)
    {
        gu8CompControlTimer++;
    }
}


/// @brief      ������ Comp ����(@While)
/// @details    While�� �ȿ��� ������ Comp�� HW ��� �Ѵ�
/// @param      void
/// @return     void
void CompControl(void)
{
    if (gu8CompControlTimer >= COMP_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8CompControlTimer = 0;

        if (gu8CompDelayTime)
        {
            gu8CompDelayTime--;
        }

        Hal_Comp_Control();
    }
}


/// @brief      ������ Comp�� On/Off ���� �Լ�
/// @details    ������ Comp�� On/Off�� ���� �� �����ð��� �����Ѵ�
/// @param      mu8OnOff : Comp�� ���� ���� - 1(On), 0(Off)
///             mu8Delay : Comp ���� �� Delay �ð�
/// @return     void
void Set_Comp(U8 mu8OnOff, U8 mu8Delay)
{
    if (mu8OnOff == 1)
    {
        gu8CompStatus = ON;
        gu8CompDelayTime = mu8Delay;
    }
    else
    {
        gu8CompStatus = OFF;
        gu8CompDelayTime = mu8Delay;
    }
}


/// @brief      ������ Comp�� ���� ���� Ȯ�� �Լ�
/// @details    ������ Comp�� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     mu8Return : Comp ���� ���� - 1(On), 0(Off)
U8 Get_Comp_Status(void)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_COMP();

    if (mu8Status)
    {
        mu8Return = TRUE;
    }
    else
    {
        mu8Return = FALSE;
    }

    return  mu8Return;
}


/// @brief      ������ Comp HW ���� �Լ�
/// @details    ������ Comp�� HW�� �����ð��� üũ�� �� ���� �����Ѵ�
/// @param      void
/// @return     void
void Hal_Comp_Control(void)
{
    if (gu8CompDelayTime == 0)
    {
        if (gu8CompStatus)
        {
            TURN_ON_COMP();
        }
        else
        {
            TURN_OFF_COMP();
        }
    }
}

#endif


// Hal Comp Module *******************************************************************************************

/// @brief      Hal Comp Initilaize Module
/// @details    ������ Comp ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Comp_Module_Initialize(void)
{
#if (NORMAL_COMP_USE == USE)
    Hal_Comp_Initialize();                  // ������ Comp ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Comp Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ������ Comp ���� �ð� Ÿ�̸Ӹ� ī��Ʈ ��Ų��
/// @param      void
/// @return     void
void Hal_Comp_Module_1ms_Control(void)
{
#if (NORMAL_COMP_USE == USE)
    CompControlTimer();                     // ������ Comp ���� �ð� Counter
#endif
}


/// @brief      Hal Comp Module in While Loop
/// @details    Basic Loop ���� While�� �ȿ��� ������ Comp�� �����Ѵ�
/// @param      void
/// @return     void
void Hal_Comp_Module_Control(void)
{
#if (NORMAL_COMP_USE == USE)
    CompControl();                          // ������ Comp Control �Լ�
#endif
}

