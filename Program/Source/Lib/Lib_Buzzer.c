/// @file     Lib_Buzzer.c
/// @date     2025/04/10
/// @author   Jaejin Ham
/// @brief    Buzzer ���� ���� file

#include "Global_Header.h"


#if (BUZZER_USE == USE)

U8 gu8SilentStatus = 0;                             /// @brief  ������� ���� ���� - 0(���� ���� �ƴ�), 1(���� ����)

U8 gu8BuzzerOrder = 0;                              /// @brief  Buzzer �߻� ���� �ܰ�
U8 gu8BuzzerMode = 0;                               /// @brief  Buzzer ��� ����

U8 gu8BuzzerControlTime = 0;                        /// @brief  Buzzer �߻� ���� Ÿ�̸�
U8 gu8BuzzerTime = 0;                               /// @brief  Buzzer���� ���� ���� Ÿ�̸�



/// @brief      Buzzer ���� Initialize
/// @details    Buzzer ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void BuzzerInitialize(void)
{
    gu8SilentStatus = 0;           // ���ҰŸ�� Clear

    gu8BuzzerOrder = 0;
    gu8BuzzerMode = 0;

    gu8BuzzerControlTime = 0;
    gu8BuzzerTime = 0;

    Hal_Buzzer_Initialize();
}


/// @brief      Buzzer �� ��� ���� ���� �Լ�
/// @details    Buzzer �� ��� ���¸� �����Ѵ�
/// @param      mu8Mode : 0(�������), 1(�� ��� ���)
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


/// @brief      Buzzer ���� ���� �Լ�
/// @details    Buzzer �߻��� ������ �����Ѵ�
/// @param      mu8Type : Buzzer ����� ����
/// @return     void
void Set_BuzzerSelect(U8 mu8Type)
{
    gu8BuzzerTime = 0;
    gu8BuzzerOrder = 1;
    gu8BuzzerMode = mu8Type;
}


/// @brief      Buzzer Time Count ���� �Լ�(@1ms)
/// @details    Buzzer ���� ���� �ð����� ī��Ʈ �Ѵ�
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


/// @brief      Buzzer ���� �Լ�(@1ms)
/// @details    Buzzer �� ��� ��� �Ѵ�
/// @param      void
/// @return     void
void BuzzerControl(void)
{
#if (BUZZER_PRODUCT == BUZZER_WATER)
// ������ Buzzer **************************************************************************************************
    if (!gu8SilentStatus)
    {
        switch (gu8BuzzerMode)
        {
            case BUZZER_OFF:
                gu8BuzzerTime = 0;
                Set_Buzzer_Out(0);
                break;

            case BUZZER_POWER_ON:       // ���� ON
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

        case BUZZER_EFFLUENT:       // ���������
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

        case BUZZER_EFFLUENT_END:   // ����������
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

        case BUZZER_COOL_SETUP:         // �ð� ON��
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

        case BUZZER_COOL_CANCEL:        // �ð� OFF��
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

        case BUZZER_SELECT:         // ������
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

        case BUZZER_ERROR:          // ����(���)��
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

        case BUZZER_SETUP:          // ������
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

        case BUZZER_CANCEL:         // ������
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

        case BUZZER_EFFLUENT_CONTINUE:      // ����������
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

        case BUZZER_STERILIZE_START:    // ��ս�����
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

        case BUZZER_STERILIZE_END:      // ���������
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

        case BUZZER_AP_CONNECT:         // AP ���� ������
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

        case BUZZER_SERVER_CONNECT:     // ���� ���� ������
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

        case BUZZER_EXTRA_HEAT_END:     // �ʰ�¼� ���� �Ϸ���
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
    {       // ���ҰŸ�� ����
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

            case BUZZER_SET:        // ����
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

            case BUZZER_CLEAR:      // ����
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

            case BUZZER_ERROR:      // ���� �߻���
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

            case BUZZER_SERVER_OK:      // ���� ���� �Ϸ�
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

            case BUZZER_AP_OK:      // AP ���� �Ϸ�
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
        {       // ���ҰŸ�� ����
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
