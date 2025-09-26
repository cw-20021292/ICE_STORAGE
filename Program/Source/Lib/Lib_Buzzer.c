/// @file     Lib_Buzzer.c
/// @date     2025/04/10
/// @author   Jaejin Ham
/// @brief    Buzzer 동작 제어 file

#include "Global_Header.h"


#if (BUZZER_USE == USE)

U8 gu8SilentStatus = 0;                             /// @brief  무음모드 상태 여부 - 0(무음 상태 아님), 1(무음 상태)

U8 gu8BuzzerOrder = 0;                              /// @brief  Buzzer 발생 동작 단계
U8 gu8BuzzerMode = 0;                               /// @brief  Buzzer 밸생 종류

U8 gu8BuzzerControlTime = 0;                        /// @brief  Buzzer 발생 제어 타이머
U8 gu8BuzzerTime = 0;                               /// @brief  Buzzer음별 동작 유지 타이머



/// @brief      Buzzer 제어 Initialize
/// @details    Buzzer 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void BuzzerInitialize(void)
{
    gu8SilentStatus = 0;           // 음소거모드 Clear

    gu8BuzzerOrder = 0;
    gu8BuzzerMode = 0;

    gu8BuzzerControlTime = 0;
    gu8BuzzerTime = 0;

    Hal_Buzzer_Initialize();
}


/// @brief      Buzzer 음 출력 상태 설정 함수
/// @details    Buzzer 음 출력 상태를 설정한다
/// @param      mu8Mode : 0(무음모드), 1(음 출력 모드)
/// @return     void
void Set_BuzzerMode(U8 mu8Mode)
{
    if (mu8Mode)
    {
        gu8SilentStatus = 0;
    }
    else
    {
        gu8SilentStatus = 1;
    }
}


/// @brief      Buzzer 종류 설정 함수
/// @details    Buzzer 발생음 종류를 설정한다
/// @param      mu8Type : Buzzer 출력음 종류
/// @return     void
void Set_BuzzerSelect(U8 mu8Type)
{
    gu8BuzzerTime = 0;
    gu8BuzzerOrder = 1;
    gu8BuzzerMode = mu8Type;
}


/// @brief      Buzzer Time Count 제어 함수(@1ms)
/// @details    Buzzer 동작 제어 시간들을 카운트 한다
/// @param      void
/// @return     void
void BuzzerTimeCounter(void)
{
    if (gu8BuzzerControlTime >= BUZZER_CONTROL_TIME_PERIOD)
    {
        gu8BuzzerControlTime = 0;

        if (gu8BuzzerTime)
        {   // @10ms
            gu8BuzzerTime--;
        }
    }
    else
    {
        gu8BuzzerControlTime++;
    }
}


/// @brief      Buzzer 제어 함수(@1ms)
/// @details    Buzzer 음 출력 제어를 한다
/// @param      void
/// @return     void
void BuzzerControl(void)
{
#if (BUZZER_PRODUCT == BUZZER_WATER)
// 정수기 Buzzer **************************************************************************************************
    if (!gu8SilentStatus)
    {
        switch (gu8BuzzerMode)
        {
            case BUZZER_OFF:
                gu8BuzzerTime = 0;
                Set_Buzzer_Out(0);
                break;

            case BUZZER_POWER_ON:       // 전원 ON
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SI5);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(PAS6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 5;
                }
                if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 65;
                }
                if ( (gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 2;
                }
                if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 112;
                }
                if ( (gu8BuzzerOrder >= 11) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerTime = 0;
                }
                break;

        case BUZZER_EFFLUENT:       // 추출시작음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE7);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_EFFLUENT_END:   // 추출종료음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(PAS6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 30;
            }
            if ( (gu8BuzzerOrder >= 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_COOL_SETUP:         // 냉각 ON음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(PAS6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 5;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 19;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE7);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 38;
            }
            if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_COOL_CANCEL:        // 냉각 OFF음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(SOLS5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(SI5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(MI5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 38;
            }
            if ( (gu8BuzzerOrder >= 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_SELECT:         // 선택음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 28;
            }
            if ( (gu8BuzzerOrder >= 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_ERROR:          // 에러(경고)음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 2;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE7);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 28;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 2;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE7);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 28;
            }
            if ( (gu8BuzzerOrder >= 9) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_SETUP:          // 설정음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(SOL6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 5;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 19;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE7);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 38;
            }
            if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_CANCEL:         // 해제음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE7);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 5;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 19;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(SOL6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 38;
            }
            if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_EFFLUENT_CONTINUE:      // 연속추출음
            if ((gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE7);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(DOS7);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(MI6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 11) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 12) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 13) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 14) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 15) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 16) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder >= 17) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_STERILIZE_START:    // 살균시작음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(MI6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(DOS6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 38;
            }
            if ( (gu8BuzzerOrder >= 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_STERILIZE_END:      // 살균종료음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 28;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(DOS6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 8;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 28;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(DOS6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 21;
            }
            if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(PAS6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 38;
            }
            if ( (gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(MI6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 14;
            }
            if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 50;
            }
            if ( (gu8BuzzerOrder >= 11) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_AP_CONNECT:         // AP 접속 성공음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(DO6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 5;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(RE6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 5;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(MI6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 5;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 40;
            }
            if ( (gu8BuzzerOrder >= 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_SERVER_CONNECT:     // 서버 접속 성공음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(MI6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(SI5);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ((gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0))
            {
                Set_Buzzer_Out(MI6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 40;
            }
            if ( (gu8BuzzerOrder >= 11) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_EXTRA_HEAT_END:     // 초고온수 가열 완료음
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(SOLS6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 20;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(MI6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 50;
            }
            if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 3;
            }
            if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 50;
            }
            if ( (gu8BuzzerOrder >= 9) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_MEMENTO_1:      // Memento1
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 50;
            }
            if ( (gu8BuzzerOrder >= 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_MEMENTO_2:      // Memento2
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 50;
            }
            if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_MEMENTO_3:      // Memento3
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 50;
            }
            if ( (gu8BuzzerOrder >= 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_MEMENTO_4:      // Memento4
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 50;
            }
            if ( (gu8BuzzerOrder >= 9) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        case BUZZER_MEMENTO_5:      // Memento5
            if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 10;
            }
            if ( (gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 6;
            }
            if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_ON(0);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 50;
            }
            if ( (gu8BuzzerOrder >= 11) && (gu8BuzzerTime == 0) )
            {
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
            }
            break;

        default:
            Set_Buzzer_Out(0);
            gu8BuzzerOrder = 0;
            gu8BuzzerTime = 0;
            gu8BuzzerMode = 0;
            break;
        }
    }
    else
    {       // 음소거모드 동작
        Set_Buzzer_Out(0);
        gu8BuzzerOrder = 0;
        gu8BuzzerTime = 0;
        gu8BuzzerMode = 0;
    }

#elif (BUZZER_PRODUCT == BUZZER_AIR)

    if (!gu8SilentStatus)
    {
        switch (gu8BuzzerMode)
        {
            case BUZZER_OFF:
                gu8BuzzerTime = 0;
                Set_Buzzer_Out(0);
                break;

            case BUZZER_POWER_ON:       // Power On
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SI5);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(PAS6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 5;
                }
                if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 65;
                }
                if ( (gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 110;
                }
                if ( (gu8BuzzerOrder >= 11) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerTime = 0;
                }
                break;

            case BUZZER_POWER_OFF:      // Power Off
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(LA6);
                gu8BuzzerOrder++;
                gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(MI6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(PAS6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 5;
                }
                if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 65;
                }
                if ( (gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 110;
                }
                if ( (gu8BuzzerOrder >= 11) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_SET:        // 설정
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 5;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 70;
                }
                if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_CLEAR:      // 해제
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 5;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 70;
                }
                if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_KEY_IN:
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 90;
                }
                if ( (gu8BuzzerOrder >= 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_ERROR:      // 에러 발생음
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 2;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 8;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 28;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 2;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 8;
                }
                if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 90;
                }
                if ( (gu8BuzzerOrder >= 9) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_SERVER_OK:      // 서버 연결 완료
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(LA6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 20;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(MI6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 20;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(LA6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 20;
                }
                if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SI6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 20;
                }
                if ( (gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(MI6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 80;
                }
                if ( (gu8BuzzerOrder >= 11) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_AP_OK:      // AP 접속 완료
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(DO7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 4;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 100;
                }
                if ( (gu8BuzzerOrder >= 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_SERVER_START:
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 90;
                }
                if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_MEMENTO_1:      // Memento1
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 50;
                }
                if ( (gu8BuzzerOrder >= 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_MEMENTO_2:      // Memento2
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 50;
                }
                if ( (gu8BuzzerOrder >= 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_MEMENTO_3:      // Memento3
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 50;
                }
                if ( (gu8BuzzerOrder >= 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_MEMENTO_4:      // Memento4
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 50;
                }
                if ( (gu8BuzzerOrder >= 9) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_MEMENTO_5:      // Memento5
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 8) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 10;
                }
                if ( (gu8BuzzerOrder == 9) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(SOL6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 10) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 50;
                }
                if ( (gu8BuzzerOrder >= 11) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            case BUZZER_AC_ON:
                if ( (gu8BuzzerOrder == 1) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(LA6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 2) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 3) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(PAS6);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 4) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 19;
                }
                if ( (gu8BuzzerOrder == 5) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(RE7);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 6;
                }
                if ( (gu8BuzzerOrder == 6) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_ON(0);
                    gu8BuzzerOrder++;
                    gu8BuzzerTime = 50;
                }
                if ( (gu8BuzzerOrder >= 7) && (gu8BuzzerTime == 0) )
                {
                    Set_Buzzer_Out(0);
                    gu8BuzzerOrder = 0;
                    gu8BuzzerTime = 0;
                    gu8BuzzerMode = 0;
                }
                break;

            default:
                Set_Buzzer_Out(0);
                gu8BuzzerOrder = 0;
                gu8BuzzerTime = 0;
                gu8BuzzerMode = 0;
                break;
            }
        }
        else
        {       // 음소거모드 동작
            Set_Buzzer_Out(0);
            gu8BuzzerOrder = 0;
            gu8BuzzerTime = 0;
            gu8BuzzerMode = 0;
        }
    }

#else

#endif
}

#endif
