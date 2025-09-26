/// @file     Hal_NormalComp.c
/// @date     2025/02/18
/// @author   Jaejin Ham
/// @brief    Normal Comp Control file


#include "Global_Header.h"


#if (NORMAL_COMP_USE == USE)

U8 gu8CompControlTimer = 0;                         /// @brief  정속형 Comp 제어 주기 시간 타이머
U8 gu8CompStatus = 0;                               /// @brief  정속형 Comp 제어 상태값
U8 gu8CompDelayTime = 0;                            /// @brief  정속형 Comp 동작 제어 전 Delay 시간


/// @brief      정속형 Comp 제어 관련 초기화
/// @details    정속형 Comp 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Comp_Initialize(void)
{
    gu8CompControlTimer = 0;
    gu8CompStatus = 0;
    gu8CompDelayTime = 0;

    Set_Comp(OFF, 0);
}



/// @brief      정속형 Comp 제어 시간 타이머(@1ms interrupt)
/// @details    1ms 인터럽트 안에서 정속형 Comp 제어 시간 타이머를 카운트한다
/// @param      void
/// @return     void
void CompControlTimer(void)
{
    if (gu8CompControlTimer < COMP_CONTROL_TIME_PERIOD)
    {
        gu8CompControlTimer++;
    }
}


/// @brief      정속형 Comp 제어(@While)
/// @details    While문 안에서 정속형 Comp의 HW 제어를 한다
/// @param      void
/// @return     void
void CompControl(void)
{
    if (gu8CompControlTimer >= COMP_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8CompControlTimer = 0;

        if (gu8CompDelayTime)
        {
            gu8CompDelayTime--;
        }

        Hal_Comp_Control();
    }
}


/// @brief      정속형 Comp의 On/Off 설정 함수
/// @details    정속형 Comp의 On/Off와 제어 전 지연시간을 설정한다
/// @param      mu8OnOff : Comp의 동작 상태 - 1(On), 0(Off)
///             mu8Delay : Comp 제어 전 Delay 시간
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


/// @brief      정속형 Comp의 동작 상태 확인 함수
/// @details    정속형 Comp의 동작 상태를 확인하여 반환한다
/// @param      void
/// @return     mu8Return : Comp 동작 상태 - 1(On), 0(Off)
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


/// @brief      정속형 Comp HW 제어 함수
/// @details    정속형 Comp의 HW를 지연시간을 체크한 후 직접 제어한다
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
/// @details    정속형 Comp 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Comp_Module_Initialize(void)
{
#if (NORMAL_COMP_USE == USE)
    Hal_Comp_Initialize();                  // 정속형 Comp 제어 관련 초기화
#endif
}


/// @brief      Hal Comp Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 정속형 Comp 제어 시간 타이머를 카운트 시킨다
/// @param      void
/// @return     void
void Hal_Comp_Module_1ms_Control(void)
{
#if (NORMAL_COMP_USE == USE)
    CompControlTimer();                     // 정속형 Comp 제어 시간 Counter
#endif
}


/// @brief      Hal Comp Module in While Loop
/// @details    Basic Loop 안의 While문 안에서 정속형 Comp를 제어한다
/// @param      void
/// @return     void
void Hal_Comp_Module_Control(void)
{
#if (NORMAL_COMP_USE == USE)
    CompControl();                          // 정속형 Comp Control 함수
#endif
}

