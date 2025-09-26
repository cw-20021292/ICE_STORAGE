/// @file     Hal_IceSwingBar.c
/// @date     2025/07/29
/// @author   Jaejin Ham
/// @brief    제빙 스윙바 Control file


#include "Global_Header.h"


#if (ICE_SWING_BAR_USE == USE)

U8 gu8SwingBarControlTimer = 0;

U8 gu8ActSwingBar = 0;                      /// @brief  스윙바 동작 제어 상태 여부 = 0(미동작), 1(동작)
U8 gu8SwingBarActStatus = 0;                /// @brief  스윙바 동작 상태 - 0(전자석 OFF 방향), 1(전자석 ON 방향)
U8 gu8SwingBarActTimer = 0;                 /// @brief  스윙바 동작 시간


/// @brief      Swing Bar Initialize
/// @details    스윙바 제어 관련 데이터들을 초기화한다
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
/// @details    스윙바 제어를 위한 타이밍을 카운트 한다
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
/// @details    스윙바 제어 동작을 한다
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
            {   // 스윙바 동작 제어 중인 경우
                mu16IceMakeTimeBuf = Get_IceMakeTime();

                if (mu16IceMakeTimeBuf > 5)
                {   // 제빙 동작이 500ms 넘게 남은 경우
                    mu8IceStep = Get_IceStep();

                    if ( (gu8SwingBarActTimer == 0) &&
                         (mu8IceStep == STATE_31_MAIN_ICE_MAKING) )
                    {
                        if (gu8SwingBarActStatus == ON)
                        {   // 스윙바 ON 방향 동작 중이었던 경우
                            gu8SwingBarActStatus = OFF;
                            gu8SwingBarActTimer = SWING_BAR_OFF_TIME;
                            TURN_OFF_SWING_BAR();
                        }
                        else
                        {   // 스윙바 OFF 방향 동작 중이었던 경우
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
                {   // 제빙 동작이 500ms 남은 경우
                    gu8SwingBarActStatus = OFF;
                    gu8SwingBarActTimer = 0;
                    TURN_OFF_SWING_BAR();
                }
            }
            else
            {   // 스윙바 동작 제어가 아닌 경우
                gu8SwingBarActStatus = OFF;
                gu8SwingBarActTimer = 0;
                TURN_OFF_SWING_BAR();
            }
        }
    }
}



/// @brief      스윙바 동작 설정 함수
/// @details    스윙바 동작 여부를 설정한다
/// @param      mu8Status : 스윙바 동작 상태 여부 - 0(미동작), 1(동작)
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


/// @brief      스윙바 동작 상태 확인 함수
/// @details    스윙바 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 스윙바 동작 상태 여부 - 0(미동작), 1(동작)
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
/// @details    얼음 스윙바 제어 관련 변수 모두 초기화
/// @param      void
/// @return     void
void Hal_SwingBar_Module_Initialize(void)
{
#if (ICE_SWING_BAR_USE == USE)
    Hal_SwingBar_Initialize();              // Swing Bar 제어 관련 초기화
#endif
}


/// @brief      Hal Swing Bar Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 얼음 스윙바 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_SwingBar_Module_1ms_Control(void)
{
#if (ICE_SWING_BAR_USE == USE)
    SwingBarControlTimer();                 // Swing Bar  제어 시간 Counter
#endif
}


/// @brief      Hal SWing Bar Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 얼음 스윙바 제어를 한다
/// @param      void
/// @return     void
void Hal_SwingBar_Module_Control(void)
{
#if (ICE_SWING_BAR_USE == USE)
    Hal_SwingBarControl();                  // Swing Bar 제어 함수
#endif
}

