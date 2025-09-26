/// @file     Hal_IceSwingBar.c
/// @date     2025/07/29
/// @author   Jaejin Ham
/// @brief    ���� ������ Control file


#include "Global_Header.h"


#if (ICE_SWING_BAR_USE == USE)

U8 gu8SwingBarControlTimer = 0;

U8 gu8ActSwingBar = 0;                      /// @brief  ������ ���� ���� ���� ���� = 0(�̵���), 1(����)
U8 gu8SwingBarActStatus = 0;                /// @brief  ������ ���� ���� - 0(���ڼ� OFF ����), 1(���ڼ� ON ����)
U8 gu8SwingBarActTimer = 0;                 /// @brief  ������ ���� �ð�


/// @brief      Swing Bar Initialize
/// @details    ������ ���� ���� �����͵��� �ʱ�ȭ�Ѵ�
/// @param      void
/// @return     void
void Hal_SwingBar_Initialize(void)
{
    gu8SwingBarControlTimer = 0;

    gu8ActSwingBar = 0;
    gu8SwingBarActStatus = 0;
    gu8SwingBarActTimer = 0;
}


/// @brief      Swing Bar Control Timer(@1ms interrupt)
/// @details    ������ ��� ���� Ÿ�̹��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void SwingBarControlTimer(void)
{
    if (gu8SwingBarControlTimer < SWING_BAR_CONTROL_TIME_PERIOD)
    {
        gu8SwingBarControlTimer++;
    }
}


/// @brief      Swing Bar Control(@While Loop)
/// @details    ������ ���� ������ �Ѵ�
/// @param      void
/// @return     void
void Hal_SwingBarControl(void)
{
    U16 mu16IceMakeTimeBuf = 0;
    U8 mu8IceMakingDisableStatus = 0;
    U8 mu8Error61_Buf = 0;
    U8 mu8IceSafetyRoutineBuf = 0;

    U8 mu8IceStep = 0;


    if (gu8SwingBarControlTimer >= SWING_BAR_CONTROL_TIME_PERIOD)
    {
        gu8SwingBarControlTimer = 0;

        if (gu8SwingBarActTimer)
        {
            gu8SwingBarActTimer--;
        }

        mu8IceMakingDisableStatus = Get_IceMakingDisable();
        mu8Error61_Buf = Get_ErrorDetect(ERROR_ID_MICRO_SW_ALL_DETECT_E61);
        mu8IceSafetyRoutineBuf = Get_IceSafetyRoutine();

        if ( (mu8IceMakingDisableStatus == TRUE) ||
             (mu8Error61_Buf == TRUE) ||
             (mu8IceSafetyRoutineBuf == TRUE) )
        {
            gu8SwingBarActStatus = OFF;
            gu8SwingBarActTimer = 0;
            TURN_OFF_SWING_BAR();
        }
        else
        {
            if (gu8ActSwingBar == SET)
            {   // ������ ���� ���� ���� ���
                mu16IceMakeTimeBuf = Get_IceMakeTime();

                if (mu16IceMakeTimeBuf > 5)
                {   // ���� ������ 500ms �Ѱ� ���� ���
                    mu8IceStep = Get_IceStep();

                    if ( (gu8SwingBarActTimer == 0) &&
                         (mu8IceStep == STATE_31_MAIN_ICE_MAKING) )
                    {
                        if (gu8SwingBarActStatus == ON)
                        {   // ������ ON ���� ���� ���̾��� ���
                            gu8SwingBarActStatus = OFF;
                            gu8SwingBarActTimer = SWING_BAR_OFF_TIME;
                            TURN_OFF_SWING_BAR();
                        }
                        else
                        {   // ������ OFF ���� ���� ���̾��� ���
                            gu8SwingBarActStatus = ON;
                            gu8SwingBarActTimer = SWING_BAR_ON_TIME;
                            TURN_ON_SWING_BAR();
                        }
                    }
                    else
                    {

                    }
                }
                else
                {   // ���� ������ 500ms ���� ���
                    gu8SwingBarActStatus = OFF;
                    gu8SwingBarActTimer = 0;
                    TURN_OFF_SWING_BAR();
                }
            }
            else
            {   // ������ ���� ��� �ƴ� ���
                gu8SwingBarActStatus = OFF;
                gu8SwingBarActTimer = 0;
                TURN_OFF_SWING_BAR();
            }
        }
    }
}



/// @brief      ������ ���� ���� �Լ�
/// @details    ������ ���� ���θ� �����Ѵ�
/// @param      mu8Status : ������ ���� ���� ���� - 0(�̵���), 1(����)
/// @return     void
void Set_SwingBar_Operation(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8ActSwingBar = SET;
    }
    else
    {
        gu8ActSwingBar = CLEAR;
    }
}


/// @brief      ������ ���� ���� Ȯ�� �Լ�
/// @details    ������ ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ ���� ���� ���� - 0(�̵���), 1(����)
U8 Get_SwingBar_Operation(void)
{
    if (gu8ActSwingBar)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}

#endif


// Hal Swing Bar Module **************************************************************************************

/// @brief      Hal Swing Bar Initilaize Module
/// @details    ���� ������ ���� ���� ���� ��� �ʱ�ȭ
/// @param      void
/// @return     void
void Hal_SwingBar_Module_Initialize(void)
{
#if (ICE_SWING_BAR_USE == USE)
    Hal_SwingBar_Initialize();              // Swing Bar ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Swing Bar Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ���� ������ ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_SwingBar_Module_1ms_Control(void)
{
#if (ICE_SWING_BAR_USE == USE)
    SwingBarControlTimer();                 // Swing Bar  ���� �ð� Counter
#endif
}


/// @brief      Hal SWing Bar Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� ���� ������ ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_SwingBar_Module_Control(void)
{
#if (ICE_SWING_BAR_USE == USE)
    Hal_SwingBarControl();                  // Swing Bar ���� �Լ�
#endif
}

