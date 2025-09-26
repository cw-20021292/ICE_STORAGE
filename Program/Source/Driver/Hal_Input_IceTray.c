/// @file     Hal_Input_IceTray.c
/// @date     2025/07/29
/// @author   Jaejin Ham
/// @brief    ICE TRAY 제어 관련 파일

#include "Global_Header.h"


#if (ICE_TRAY_INPUT_CHECK_USE == USE)

U8 gu8InputIceTrayControlTimer = 0;                 /// @brief  Ice Tray 감지 제어 시간 카운터

U16 gu16IceLevelDelay = 0;                          /// @brief  Ice Tray 감지 확정 Delay 시간

// 얼음받이센서
U8 gu8IceLEV = 0;                                   /// @brief  Ice Tray 현재 위치 상태
U8 gu8BeIceLEV = 0;                                 /// @brief  Ice Tray 이전 위치 상태
U8 gu8IceTrayLEV = 0;                               /// @brief  Ice Tray 확정 위치 상태



/// @brief      Ice Tray 감지 관련 변수 초기화
/// @details    Ice Tray 감지와 관련된 변수를 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Input_IceTray_Initialize(void)
{
    gu8InputIceTrayControlTimer = 0;

    gu16IceLevelDelay = 0;

    gu8IceLEV = 0;
    gu8BeIceLEV = 0;
    gu8IceTrayLEV = 0;
}


/// @brief      Ice Tray control Timer(@1ms interrupt)
/// @details    Ice Tray 동작 제어 주기 시간을 카운트 한다
/// @param      void
/// @return     void
void Input_IceTrayControlTimer(void)
{
    if (gu8InputIceTrayControlTimer < INPUT_ICE_TRAY_CONTROL_TIME_PERIOD)
    {       // 100ms 마다 처리
        gu8InputIceTrayControlTimer++;
    }
}


/// @brief      Ice Tray Micro SW의 감지 동작 함수
/// @details    Ice Tray의 Micro SW의 감지 동작 제어를 한다
/// @param      void
/// @return     void
void Input_IceTray_Micro_SW(void)
{
    U8 mu8MicroSW_HighBuf = 0;
    U8 mu8MicroSW_LowBuf = 0;

    if (gu8InputIceTrayControlTimer >= INPUT_ICE_TRAY_CONTROL_TIME_PERIOD)
    {
        gu8InputIceTrayControlTimer = 0;

        if(gu16IceLevelDelay)
        {
            gu16IceLevelDelay--;            // Ice Tray 딜레이
        }
        else
        {

        }

        //============================================================ 트레이 위치 감지
        mu8MicroSW_HighBuf = GET_STATUS_ICE_TRAY_MICRO_SW_HIGH();
        mu8MicroSW_LowBuf = GET_STATUS_ICE_TRAY_MICRO_SW_LOW();

        if ( (mu8MicroSW_HighBuf == CLEAR) && (mu8MicroSW_LowBuf == SET) )
        {
            gu8IceLEV = ICE_TRAY_POSITION_ICE_MAKING;       // 얼음받음
        }
        else if ( (mu8MicroSW_HighBuf == SET) && (mu8MicroSW_LowBuf == CLEAR) )
        {
            gu8IceLEV = ICE_TRAY_POSITION_ICE_THROW;        // 얼음버림
        }
        else if ( (mu8MicroSW_HighBuf == SET) && (mu8MicroSW_LowBuf == SET) )
        {
            gu8IceLEV = ICE_TRAY_POSITION_MOVING;           // 이동중
        }
        else if ( (mu8MicroSW_HighBuf == CLEAR) && (mu8MicroSW_LowBuf == CLEAR) )
        {
            gu8IceLEV = ICE_TRAY_POSITION_ERROR;            // 에러
        }
        else
        {

        }

        if (gu8IceLEV != gu8BeIceLEV)
        {   // 이전 위치와 현재 위치 상태가 다른 경우
            gu8BeIceLEV = gu8IceLEV;
            gu16IceLevelDelay = ICE_TRAY_DELAY_TIME;           // 트레이 시간 딜레이
        }
        else
        {   // 이전 위치와 현재 위치 상태가 같은 경우
            if(!gu16IceLevelDelay)
            {   // 지연 시간 이후 확정
                gu8IceTrayLEV = gu8IceLEV;
            }
        }
    }
}


/// @brief      Ice Tray 위치 상태 확인 함수
/// @details    Ice Tray 위치 상태를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : Ice Tray 위치 상태 - 0(얼음받음), 1(얼음버림), 2(이동중), 3(에러)
U8 Get_IceTrayPosition(void)
{
    return  gu8IceTrayLEV;
}


#endif


// Hal Water Input Ice Tray Module ***************************************************************************************

/// @brief      Hal Ice Tray 감지 Initilaize Module
/// @details    Ice Tray 감지 제어와 관련된 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Input_IceTray_Module_Initialize(void)
{
#if (ICE_TRAY_INPUT_CHECK_USE == USE)
    Hal_Input_IceTray_Initialize();
#endif
}


/// @brief      Hal Ice Tray 감지 Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 Ice Tray 감지 제어 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_Input_IceTray_Module_1ms_Control(void)
{
#if (ICE_TRAY_INPUT_CHECK_USE == USE)
    Input_IceTrayControlTimer();
#endif
}


/// @brief      Hal Ice Tray 감지 Module in While Loop
/// @details    Basic Loop 안 While문 안에서 Ice Tray 감지 제어를 한다
/// @param      void
/// @return     void
void Hal_Input_IceTray_Module_Control(void)
{
#if (ICE_TRAY_INPUT_CHECK_USE == USE)
    Input_IceTray_Micro_SW();
#endif
}
