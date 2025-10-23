/// @file     Lib_Water_Cool_TEM.c
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    ������� �ð� ���� ���� file

#include "Global_Header.h"


#if (TEM_USE == USE)                // ������� �ð� ����


/// @brief  TEM PWM ���� TDR �������� ������ ���̺�
__far const U16 gau16TEM_TDR_DataTable[TEM_V_ID_MAX] =
{
    32000,  32000,  32000,  28500,  27500,  26500,  25600,  24800,  24000,  23200,      // 0V ~ 9V
    22000,  21500,  20600,  19800,  18800,  17800,  16700,  15500,  14200,  12600,      // 10V ~ 19V
    10800,  8800,   6200,   3200,   0                                                   // 20V ~ 24V
};

U8 gu8TemControlPeriodTimer = 0;            /// @brief  TEM ��� ���� �ֱ� Ÿ�̸� ī����

U8 gu8ColdOnOff = 0;                        /// @brief  �ü� ��� On/Off ����
U8 gu8ColdStrong = 0;                       /// @brief  �ü� ��� ���� ����

U8 gu8TEMControlMode = 0;                   /// @brief  TEM ���� ���

U16 gu16TEM_RapidModeTime = 0;              /// @brief  TEM �޼� ��� ���� �ð�
U8 gu8TEM_RapidModeConfirmTimer = 0;        /// @brief  TEM �޼� ��� ��ȯ Ȯ�� Ÿ�̸�
U8 gu8TEM_RetainModeConfirmTimer = 0;       /// @brief  TEM ���� ��� ��ȯ Ȯ�� Ÿ�̸�
U8 gu8TEM_OFFModeConfirmTimer = 0;          /// @brief  TEM ���� ���� ��ȯ Ȯ�� Ÿ�̸�
U8 gu8TEM_ECOModeConfirmTimer = 0;          /// @brief  TEM ECO ��� ��ȯ Ȯ�� Ÿ�̸�

U8 gu8TEM_TimeCoolingRapidCheck = 0;        /// @brief  TEM �޼� ��忡�� �߰� ���� �ð� üũ ���� ���� - 0(��üũ��), 1(üũ��)
U16 gu16TEM_TimerCoolingRapid = 0;          /// @brief  TEM �޼� ��忡�� �߰� ���� �ð�

U16 gu16TEM_OnDelayRapidOff = 0;            /// @brief  TEM �޼� ��� 6�ð� �⵿���� TEM OFF�� ��� TEM ��⵿ �� ��� �ð�

U8 gu8TEM_HeatsinkTempCheckTime = 0;        /// @brief  TEM Heatsink�� �µ��� 80���� �Ѵ� �ð� üũ Ÿ�̸�

U16 gu16TimerRapidInCon12V = 0;             /// @brief  �ð� �޼� ���� ���Խ� �ʱ� ���� 12V �ΰ� �ð�



/// @brief      TEM �ð� ���� �ʱ�ȭ
/// @details    TEM �ð� ���� �������� ��� �ʱ�ȭ �Ѵ�
/// @param      void
/// @return     void
void Lib_TEM_Initialize(void)
{
    gu8TemControlPeriodTimer = 0;

    gu8ColdOnOff = ON;                      // �ü� ��� ON
    gu8ColdStrong = SET;                    // �����

    gu8TEMControlMode = TEM_MODE_ID_OFF;    // TEM ���� ��� OFF

    gu16TEM_RapidModeTime = 0;
    gu8TEM_RapidModeConfirmTimer = 0;
    gu8TEM_RetainModeConfirmTimer = 0;
    gu8TEM_OFFModeConfirmTimer = 0;
    gu8TEM_ECOModeConfirmTimer = 0;

    gu8TEM_TimeCoolingRapidCheck = 0;
    gu16TEM_TimerCoolingRapid = 0;

    gu16TEM_OnDelayRapidOff = 0;

    gu8TEM_HeatsinkTempCheckTime = 0;

    gu16TimerRapidInCon12V = 0;
}


/// @brief      TEM ���� �ð� Ÿ�̸�(@1ms)
/// @details    1ms ���ͷ�Ʈ �ȿ��� TEM ���� �ð� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Lib_TEM_ControlTimer(void)
{
    if (gu8TemControlPeriodTimer < LIB_TEM_CONTROL_TIME_PERIOD)
    {
        gu8TemControlPeriodTimer++;
    }
}


/// @brief      �ü� ��� ON/OFF ���� ���� �Լ�
/// @details    �ü� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8Status : �ü� ��� ���� ���� - 0(OFF), 1(ON)
/// @return     void
void Set_ColdFuncStatus(U8 mu8Status)
{
    gu8ColdOnOff = mu8Status;
}


/// @brief      �ü� ��� ON/OFF ���� Ȯ�� �Լ�
/// @details    �ü� ��� ���°� ON���� OFF ���� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ü� ��� ���� ���� - 0(OFF), 1(ON)
U8 Get_ColdFuncStatus(void)
{
    return  gu8ColdOnOff;
}


/// @brief      �ü� ��� ���� ���� ���� �Լ�
/// @details    �ü� ��� ���°� ������ ������ �����Ѵ�
/// @param      mu8Status : �ü� ��� ���� ���� ���� - 0(��), 1(��)
/// @return     void
void Set_ColdStorng(U8 mu8Status)
{
    gu8ColdStrong = mu8Status;
}


/// @brief      �ü� ��� ���� ���� Ȯ�� �Լ�
/// @details    �ü� ��� ���°� ������ �� ���� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ü� ��� ���� ���� - 0(��), 1(��)
U8 Get_ColdStrong(void)
{
    return  gu8ColdStrong;
}


/// @brief      �ü� ��� TEM ������ ���� Ȯ�� �Լ�
/// @details    �ü� ��� TEM ������ ���¸� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : TEM ������ ���� - ������� enum���� ����� ��� ����Ʈ ����
U8 Get_TEM_ActMode(void)
{
    return  gu8TEMControlMode;
}


/// @brief      TEM ����
/// @details    TEM ���� ������ �Ѵ�
/// @param      void
/// @return     void
void Lib_TEM_Control(void)
{
    static U8 mu8TemConTimerSec = 0;

    U8 mu8ColdOnOff = 0;
    U8 mu8ColdStrong = 0;
    F32 mf32AirTemp = 0;
    U8 mu8AirError = 0;
    F32 mf32ColdTemp = 0;
    U8 mu8ColdError = 0;
    U8 mu8TEM_VoltageBuf = 0;
    U16 mu16TEM_TDR_DataBuff = 0;
    U8 mu8LeakError = 0;
    F32 mf32HeatsinkTemp = 0;

    mu8TEM_VoltageBuf = Get_TEM_Voltage();

    if (gu8TemControlPeriodTimer >= LIB_TEM_CONTROL_TIME_PERIOD)
    {   // 100ms ����
        gu8TemControlPeriodTimer = 0;

        mf32HeatsinkTemp = Get_Temp(ADC_ID_TH_HEATSINK);   // TEM �濭�� �µ� ���� �µ��� Ȯ��
        if (mf32HeatsinkTemp > 80.0f)
        {   // TEM Heatsink�� 80���� ������
            if (gu8TEM_HeatsinkTempCheckTime)
            {
                gu8TEM_HeatsinkTempCheckTime--;
            }
        }
        else
        {   // 80���ϸ�
            gu8TEM_HeatsinkTempCheckTime = 10;      // 80�� �ʰ� üũ 1sec
        }

        if (gu8TEMControlMode != TEM_MODE_ID_RAPID)
        {
            gu16TEM_RapidModeTime = TEM_RAPID_MODE_MAX_TIME;
        }

        if (++mu8TemConTimerSec >= 10)
        {   // 1�� ���� ����ؾ� �� ������ ó��
            mu8TemConTimerSec = 0;

            if (gu16TEM_OnDelayRapidOff)
            {   // �޼Ӹ�忡�� ����Ǿ� ��⵿ �� �����ð��� �ִ� ���
                gu16TEM_OnDelayRapidOff--;
            }

            if (gu8TEMControlMode == TEM_MODE_ID_RAPID)
            {
                if (gu16TEM_RapidModeTime)
                {   // �޼Ӹ�� ���� �ð� üũ
                    gu16TEM_RapidModeTime--;
                }
            }

            if (gu8TEM_TimeCoolingRapidCheck)
            {   // �޼Ӹ�� �߰� �⵿ �ð� üũ ��
                if (gu16TEM_TimerCoolingRapid)
                {
                    gu16TEM_TimerCoolingRapid--;
                }
            }
        }

        mu8ColdOnOff = Get_ColdOnOff();                             // �ü� ��� ON/OFF ���� Ȯ��
        mu8ColdStrong = Get_ColdStrong();                           // �ü� ��� ���� ���� Ȯ��
        mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);                  // �ܱ� �µ� Ȯ��
        mu8AirError = Get_ErrorStatus(ERROR_ID_AIR_TEMP_1_E43);         // �ܱ� �µ� ���� ���� ���� Ȯ��
        mf32ColdTemp = Get_Temp(ADC_ID_TH_COOL);              // �ü� �µ� Ȯ��
        mu8ColdError = Get_ErrorStatus(ADC_ID_TH_COOL);       // �ü� �µ� ���� ���� ���� Ȯ��
        mu8LeakError = Get_ErrorStatus(ERROR_ID_LEAKAGE_E01);           // ���� ���� ���� Ȯ��

        if ( (mu8ColdOnOff == TRUE) && (mu8ColdError == FALSE) )
        {   // �ü� ��� ON��
            // TEM ���� ��� �Ǵ� ===========================================================================================================
            /***************�ü� �����*******************/
            if (mu8ColdStrong == TRUE)
            {   // ����� ������ ���
                switch (gu8TEMControlMode)
                {
                    case TEM_MODE_ID_OFF:       // TEM OFF ���
                        gu8TEM_OFFModeConfirmTimer = 0;

                        if (gu16TEM_OnDelayRapidOff == 0)
                        {   // �޼Ӹ�� �ִ� ���۽ð����� OFF�� �߻��ϴ� �絿�� �� �����ð��� ���� ��� ���� �Ǵ�
                            // �޼� ��� ���� �Ǵ�
                            if (mf32ColdTemp > RAPID_OPERATE_DEGREE)
                            {   // �޼� ��� ��ȯ �Ǵ� �µ� ������ �����ϸ�
                                if (gu8TEM_RapidModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                                {   // Ȯ�� �ð���ŭ ��� �� ��� ��ȯ
                                    gu8TEM_OFFModeConfirmTimer = 0;
                                    gu8TEM_RapidModeConfirmTimer = 0;           // �޼� ��� ��ȯ�� �޼� ��� ��ȯ üũ �ð� �ʱ�ȭ
                                    gu8TEM_RetainModeConfirmTimer = 0;          // �޼� ��� ��ȯ�� ���� ��� ��ȯ üũ �ð� �ʱ�ȭ
                                    gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                    gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                    gu8TEMControlMode = TEM_MODE_ID_RAPID;      // �޼� ��� ����
                                }
                                else
                                {
                                    gu8TEM_RapidModeConfirmTimer++;
                                }
                            }
                            else
                            {   // �޼� ��� ��ȯ �Ǵ� �µ� ������ �������� ������
                                gu8TEM_RapidModeConfirmTimer = 0;

                                // ���� ��� ���� �Ǵ�
                                if (mf32ColdTemp >= COLD_REOPERATE_DEGREE)
                                {   // ���� ��� ����� �µ� ������ �����ϸ�
                                    if (gu8TEM_RetainModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                                    {   // Ȯ�� �ð���ŭ ��� �� ��� ��ȯ
                                        gu8TEM_OFFModeConfirmTimer = 0;
                                        gu8TEM_RapidModeConfirmTimer = 0;
                                        gu8TEM_RetainModeConfirmTimer = 0;
                                        gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                        gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                        gu8TEMControlMode = TEM_MODE_ID_RETAIN;      // ���� ��� ����
                                    }
                                    else
                                    {
                                        gu8TEM_RetainModeConfirmTimer++;
                                    }
                                }
                                else
                                {
                                    gu8TEM_RetainModeConfirmTimer = 0;
                                }
                            }
                        }
                        break;

                    case TEM_MODE_ID_RAPID:     // �޼� ���
                        gu8TEM_RapidModeConfirmTimer = 0;

                        // �޼� ��� ���� �µ� �������� ���� ��� ���� �Ǵ�
                        if (mf32ColdTemp < RAPID_COMPLETE_DEGREE)
                        {   // �޼� ��� ���� �µ� ���� �� �߰� ���� �ð����� ���� ��� ��ȯ �Ǵ�
                            if (gu8TEM_TimeCoolingRapidCheck == CLEAR)
                            {   // �޼� ��� �߰� ���� �ð��� ������ ��� �ð� ����
                                gu8TEM_TimeCoolingRapidCheck = SET;

                                if (mu8AirError == FALSE)
                                {   // �ܱ� �µ� ���� ���� �߻���
                                    gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_3_TIME;
                                }
                                else
                                {   // �ܱ� �µ� ���� ���� �̹߻���
                                    if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_1_TEMP)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_1_TIME;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_2_TEMP)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_2_TIME;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_3_TEMP)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_3_TIME;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_4_TEMP)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_4_TIME;
                                    }
                                    else
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_5_TIME;
                                    }
                                }
                            }
                            else
                            {   // �޼� ��� �߰� ������ ������ ����
                                if (gu16TEM_TimerCoolingRapid == 0)
                                {   // �߰� �ð� �ð� �Ϸ��
                                    gu8TEM_OFFModeConfirmTimer = 0;
                                    gu8TEM_RapidModeConfirmTimer = 0;
                                    gu8TEM_RetainModeConfirmTimer = 0;
                                    gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                    gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                    gu8TEMControlMode = TEM_MODE_ID_RETAIN;     // ���� ��� ����
                                }
                            }
                        }

                        // ���� ��� ��ȯ �ü� �µ� �������� ���� ���� ��ȯ �Ǵ�
                        if (mf32ColdTemp < RETAIN_JOIN_DEGREE)
                        {   // ���� ��� ��ȯ �Ǵ� ������ �����ϸ�
                            if (gu8TEM_RetainModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // Ȯ�� �ð���ŭ ��� ��
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                gu8TEMControlMode = TEM_MODE_ID_RETAIN;     // ���� ��� ����
                            }
                            else
                            {
                                gu8TEM_RetainModeConfirmTimer++;
                            }
                        }
                        else
                        {   // ���� ��� ��ȯ �Ǵ� ������ �������� ������
                            gu8TEM_RetainModeConfirmTimer = 0;
                        }

                        // �޼� ��忡�� �ð� ���� �Ǵ�
                        if ( (mf32ColdTemp <= COLD_COMPLETE_DEGREE) ||
                             (mu8ColdError == TRUE) ||
                             ( (mf32ColdTemp < 6.0f) && (mf32AirTemp < 6.0f) ) ||
                             (mu8LeakError == TRUE) ||
                             (gu8TEM_HeatsinkTempCheckTime == 0) ||
                             (gu16TEM_RapidModeTime == 0) )
                        {   // �ð� ���� ������ ���
                            if (gu8TEM_OFFModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // Ȯ�� �ð���ŭ ��� ��
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                gu8TEMControlMode = TEM_MODE_ID_OFF;

                                if (gu16TEM_RapidModeTime == 0)
                                {   // �޼Ӹ�� �ִ� ���۽ð����� TEM OFF�Ǵ� ���
                                    gu16TEM_OnDelayRapidOff = TEM_RAPID_OFF_DELAY_TIME;     // �޼Ӹ�忡�� ����� �簡�� �� ���� �ð� ����
                                }
                            }
                            else
                            {
                                gu8TEM_OFFModeConfirmTimer++;
                            }
                        }
                        else
                        {
                            gu8TEM_OFFModeConfirmTimer = 0;
                        }
                        break;

                    case TEM_MODE_ID_RETAIN:        // ���� ���
                        gu8TEM_RetainModeConfirmTimer = 0;

                        // �޼� ��� ���� �Ǵ�
                        if (mf32ColdTemp > RAPID_OPERATE_DEGREE)
                        {   // �޼� ��� ��ȯ �Ǵ� �µ� ������ �����ϸ�
                            if (gu8TEM_RapidModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // Ȯ�� �ð���ŭ ��� �� ��� ��ȯ
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;           // �޼� ��� ��ȯ�� �޼� ��� ��ȯ üũ �ð� �ʱ�ȭ
                                gu8TEM_RetainModeConfirmTimer = 0;          // �޼� ��� ��ȯ�� ���� ��� ��ȯ üũ �ð� �ʱ�ȭ
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                gu8TEMControlMode = TEM_MODE_ID_RAPID;      // �޼� ��� ����
                            }
                            else
                            {
                                gu8TEM_RapidModeConfirmTimer++;
                            }
                        }
                        else
                        {   // �޼� ��� ��ȯ �Ǵ� �µ� ������ �������� ������
                            gu8TEM_RapidModeConfirmTimer = 0;
                        }

                        // ���� ��忡�� �ð� ���� �Ǵ�
                        if ( (mf32ColdTemp <= COLD_COMPLETE_DEGREE) ||
                             (mu8ColdError == TRUE) ||
                             ( (mf32ColdTemp < 6.0f) && (mf32AirTemp < 6.0f) ) ||
                             (mu8LeakError == TRUE) ||
                             (gu8TEM_HeatsinkTempCheckTime == 0) )
                        {   // �ð� ���� ������ ���
                            if (gu8TEM_OFFModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // Ȯ�� �ð���ŭ ��� ��
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                gu8TEMControlMode = TEM_MODE_ID_OFF;
                            }
                            else
                            {
                                gu8TEM_OFFModeConfirmTimer++;
                            }
                        }
                        else
                        {
                            gu8TEM_OFFModeConfirmTimer = 0;
                        }
                        break;

                    case TEM_MODE_ID_MIN:           // �ּ� ���
                        break;

                    case TEM_MODE_ID_ECO:           // ECO ���
                        break;

                    case TEM_MODE_ID_ECO_9V:        // ECO 9V ���� ���
                        break;

                    default:
                        gu8TEM_OFFModeConfirmTimer = 0;
                        gu8TEM_RapidModeConfirmTimer = 0;
                        gu8TEM_RetainModeConfirmTimer = 0;
                        gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                        gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                        gu8TEMControlMode = TEM_MODE_ID_OFF;
                        break;
                }
            }
            else    /***************�ü� ����*******************/
            {   // ���� ������ ���
                switch (gu8TEMControlMode)
                {
                    case TEM_MODE_ID_OFF:       // TEM OFF ���
                        gu8TEM_OFFModeConfirmTimer = 0;

                        if (gu16TEM_OnDelayRapidOff == 0)
                        {   // �޼Ӹ�� �ִ� ���۽ð����� OFF�� �߻��ϴ� �絿�� �� �����ð��� ���� ��� ���� �Ǵ�
                            // �޼� ��� ���� �Ǵ�
                            if (mf32ColdTemp > RAPID_OPERATE_DEGREE_LOW_MOE)
                            {   // �޼� ��� ��ȯ �Ǵ� �µ� ������ �����ϸ�
                                if (gu8TEM_RapidModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                                {   // Ȯ�� �ð���ŭ ��� �� ��� ��ȯ
                                    gu8TEM_OFFModeConfirmTimer = 0;
                                    gu8TEM_RapidModeConfirmTimer = 0;           // �޼� ��� ��ȯ�� �޼� ��� ��ȯ üũ �ð� �ʱ�ȭ
                                    gu8TEM_RetainModeConfirmTimer = 0;          // �޼� ��� ��ȯ�� ���� ��� ��ȯ üũ �ð� �ʱ�ȭ
                                    gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                    gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                    gu8TEMControlMode = TEM_MODE_ID_RAPID;      // �޼� ��� ����
                                }
                                else
                                {
                                    gu8TEM_RapidModeConfirmTimer++;
                                }
                            }
                            else
                            {   // �޼� ��� ��ȯ �Ǵ� �µ� ������ �������� ������
                                gu8TEM_RapidModeConfirmTimer = 0;

                                // ���� ��� ���� �Ǵ�
                                if (mf32ColdTemp >= COLD_REOPERATE_DEGREE_LOW_MODE)
                                {   // ���� ��� ����� �µ� ������ �����ϸ�
                                    if (gu8TEM_RetainModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                                    {   // Ȯ�� �ð���ŭ ��� �� ��� ��ȯ
                                        gu8TEM_OFFModeConfirmTimer = 0;
                                        gu8TEM_RapidModeConfirmTimer = 0;
                                        gu8TEM_RetainModeConfirmTimer = 0;
                                        gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                        gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                        gu8TEMControlMode = TEM_MODE_ID_RETAIN;      // ���� ��� ����
                                    }
                                    else
                                    {
                                        gu8TEM_RetainModeConfirmTimer++;
                                    }
                                }
                                else
                                {
                                    gu8TEM_RetainModeConfirmTimer = 0;
                                }
                            }
                        }
                        break;

                    case TEM_MODE_ID_RAPID:     // �޼� ���
                        gu8TEM_RapidModeConfirmTimer = 0;

                        // �޼� ��� ���� �µ� �������� ���� ��� ���� �Ǵ�
                        if (mf32ColdTemp < RAPID_COMPLETE_DEGREE_LOW_MODE)
                        {   // �޼� ��� ���� �µ� ���� �� �߰� ���� �ð����� ���� ��� ��ȯ �Ǵ�
                            if (gu8TEM_TimeCoolingRapidCheck == CLEAR)
                            {   // �޼� ��� �߰� ���� �ð��� ������ ��� �ð� ����
                                gu8TEM_TimeCoolingRapidCheck = SET;

                                if (mu8AirError == FALSE)
                                {   // �ܱ� �µ� ���� ���� �߻���
                                    gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_3_TIME_LOW_MODE;
                                }
                                else
                                {   // �ܱ� �µ� ���� ���� �̹߻���
                                    if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_1_TEMP_LOW_MODE)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_1_TIME_LOW_MODE;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_2_TEMP_LOW_MODE)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_2_TIME_LOW_MODE;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_3_TEMP_LOW_MODE)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_3_TIME_LOW_MODE;
                                    }
                                    else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_4_TEMP_LOW_MODE)
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_4_TIME_LOW_MODE;
                                    }
                                    else
                                    {
                                        gu16TEM_TimerCoolingRapid = TEM_COOLING_RAPID_END_CONDITION_5_TIME_LOW_MODE;
                                    }
                                }
                            }
                            else
                            {   // �޼� ��� �߰� ������ ������ ����
                                if (gu16TEM_TimerCoolingRapid == 0)
                                {   // �߰� �ð� �ð� �Ϸ��
                                    gu8TEM_OFFModeConfirmTimer = 0;
                                    gu8TEM_RapidModeConfirmTimer = 0;
                                    gu8TEM_RetainModeConfirmTimer = 0;
                                    gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                    gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                    gu8TEMControlMode = TEM_MODE_ID_RETAIN;     // ���� ��� ����
                                }
                            }
                        }

                        // ���� ��� ��ȯ �ü� �µ� �������� ���� ���� ��ȯ �Ǵ�
                        if (mf32ColdTemp < RETAIN_JOIN_DEGREE_LOW_MODE)
                        {   // ���� ��� ��ȯ �Ǵ� ������ �����ϸ�
                            if (gu8TEM_RetainModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // Ȯ�� �ð���ŭ ��� ��
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                gu8TEMControlMode = TEM_MODE_ID_RETAIN;     // ���� ��� ����
                            }
                            else
                            {
                                gu8TEM_RetainModeConfirmTimer++;
                            }
                        }
                        else
                        {   // ���� ��� ��ȯ �Ǵ� ������ �������� ������
                            gu8TEM_RetainModeConfirmTimer = 0;
                        }

                        // �޼� ��忡�� �ð� ���� �Ǵ�
                        if ( (mf32ColdTemp <= COLD_COMPLETE_DEGREE_LOW_MODE) ||
                             (mu8ColdError == TRUE) ||
                             ( (mf32ColdTemp < 7.0f) && (mf32AirTemp < 6.0f) ) ||
                             (mu8LeakError == TRUE) ||
                             (gu8TEM_HeatsinkTempCheckTime == 0) ||
                             (gu16TEM_RapidModeTime == 0) )
                        {   // �ð� ���� ������ ���
                            if (gu8TEM_OFFModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // Ȯ�� �ð���ŭ ��� ��
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                gu8TEMControlMode = TEM_MODE_ID_OFF;

                                if (gu16TEM_RapidModeTime == 0)
                                {   // �޼Ӹ�� �ִ� ���۽ð����� TEM OFF�Ǵ� ���
                                    gu16TEM_OnDelayRapidOff = TEM_RAPID_OFF_DELAY_TIME;     // �޼Ӹ�忡�� ����� �簡�� �� ���� �ð� ����
                                }
                            }
                            else
                            {
                                gu8TEM_OFFModeConfirmTimer++;
                            }
                        }
                        else
                        {
                            gu8TEM_OFFModeConfirmTimer = 0;
                        }
                        break;

                    case TEM_MODE_ID_RETAIN:        // ���� ���
                        gu8TEM_RetainModeConfirmTimer = 0;

                        // �޼� ��� ���� �Ǵ�
                        if (mf32ColdTemp > RAPID_OPERATE_DEGREE_LOW_MOE)
                        {   // �޼� ��� ��ȯ �Ǵ� �µ� ������ �����ϸ�
                            if (gu8TEM_RapidModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // Ȯ�� �ð���ŭ ��� �� ��� ��ȯ
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;           // �޼� ��� ��ȯ�� �޼� ��� ��ȯ üũ �ð� �ʱ�ȭ
                                gu8TEM_RetainModeConfirmTimer = 0;          // �޼� ��� ��ȯ�� ���� ��� ��ȯ üũ �ð� �ʱ�ȭ
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                gu8TEMControlMode = TEM_MODE_ID_RAPID;      // �޼� ��� ����
                            }
                            else
                            {
                                gu8TEM_RapidModeConfirmTimer++;
                            }
                        }
                        else
                        {   // �޼� ��� ��ȯ �Ǵ� �µ� ������ �������� ������
                            gu8TEM_RapidModeConfirmTimer = 0;
                        }

                        // ���� ��忡�� �ð� ���� �Ǵ�
                        if ( (mf32ColdTemp <= COLD_COMPLETE_DEGREE_LOW_MODE) ||
                             (mu8ColdError == TRUE) ||
                             ( (mf32ColdTemp < 7.0f) && (mf32AirTemp < 6.0f) ) ||
                             (mu8LeakError == TRUE) ||
                             (gu8TEM_HeatsinkTempCheckTime == 0) )
                        {   // �ð� ���� ������ ���
                            if (gu8TEM_OFFModeConfirmTimer >= TEM_MODE_CHANGE_CONFIRM_TIME)
                            {   // Ȯ�� �ð���ŭ ��� ��
                                gu8TEM_OFFModeConfirmTimer = 0;
                                gu8TEM_RapidModeConfirmTimer = 0;
                                gu8TEM_RetainModeConfirmTimer = 0;
                                gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
                                gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
                                gu8TEMControlMode = TEM_MODE_ID_OFF;
                            }
                            else
                            {
                                gu8TEM_OFFModeConfirmTimer++;
                            }
                        }
                        else
                        {
                            gu8TEM_OFFModeConfirmTimer = 0;
                        }
                        break;

                    case TEM_MODE_ID_MIN:           // �ּ� ���
                        break;

                    case TEM_MODE_ID_ECO:           // ECO ���
                        break;

                    case TEM_MODE_ID_ECO_9V:        // ECO 9V ���� ���
                        break;

                    default:
                        break;
                }
            }   // End �ü����� ��忡�� TEM ���� ��� �Ǵ�
        }
        else
        {   // �ü� ��� OFF ��
            gu8TEM_OFFModeConfirmTimer = 0;
            gu8TEM_RapidModeConfirmTimer = 0;
            gu8TEM_RetainModeConfirmTimer = 0;
            gu8TEM_TimeCoolingRapidCheck = CLEAR;       // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� üũ ���� �ʱ�ȭ
            gu16TEM_TimerCoolingRapid = 0;              // �޼� ��� ���� �� �Ϸ� �� �߰� �⵿ �ð� �ʱ�ȭ
            gu8TEMControlMode = TEM_MODE_ID_OFF;
        }

        // TEM ���� ��庰 TEM ���� ���� ===================================================================================================
        switch (gu8TEMControlMode)
        {
            case TEM_MODE_ID_OFF:
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // �޼Ӹ�� ���� �ʱ� 12V ���� ���� �ð� ����

                mu8TEM_VoltageBuf = TEM_V_ID_0V;
                mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];
                Set_CoolFanActMode(FAN_ID_OFF);
                break;

            case TEM_MODE_ID_RAPID:
                /*  12���̻� : 24v ���
                 *  6�� �̻� ~ 12�� �̸� : 12V���
                 *  �ܱ�µ����� ���� : 12�� �̻� ~ 30�ɹ̸� �������� ���
                 */

                //  �߰�����ΰ��� ���� ���� ���� FLAG ������
                if (mu8ColdStrong == TRUE)
                {   // �� �ð�
                    if (mu8AirError == FALSE)
                    {   // �ܱ� �µ� ���� ���� �ƴ� ���
                        if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_1_TEMP)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_1_V;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_2_TEMP)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_2_V;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_3_TEMP)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_3_V;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_4_TEMP)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_4_V;
                        }
                        else
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_5_V;
                        }
                    }
                    else
                    {   // �ܱ� �µ� ���� ������ ���
                        mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_3_V;
                    }

                    Set_CoolFanActMode(FAN_ID_21V_ON);
                }
                else        /***************�ü� ����*******************/
                {
                    //  �ܱ�µ����� �ƴѰ��,
                    if (mu8AirError == FALSE)
                    {   // �ܱ� �µ� ���� ���� �ƴ� ���
                        if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_1_TEMP_LOW_MODE)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_1_V_LOW_MODE;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_2_TEMP_LOW_MODE)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_2_V_LOW_MODE;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_3_TEMP_LOW_MODE)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_3_V_LOW_MODE;
                        }
                        else if (mf32AirTemp >= TEM_COOLING_RAPID_END_CONDITION_4_TEMP_LOW_MODE)
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_4_V_LOW_MODE;
                        }
                        else
                        {
                            mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_5_V_LOW_MODE;
                        }
                    }
                    else
                    {   // �ܱ� �µ� ���� ������ ���
                        mu8TEM_VoltageBuf = TEM_COOLING_RAPID_END_CONDITION_3_V_LOW_MODE;
                    }

                    Set_CoolFanActMode(FAN_ID_21V_ON);
                }

                //  �޼Ӹ�� ���� ��, �ʱ� 5�а��� 12v ���
                if (gu16TimerRapidInCon12V)
                {
                    gu16TimerRapidInCon12V--;
                    mu8TEM_VoltageBuf = TEM_V_ID_12V;
                    mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];
                }
                else
                {
                    mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];
                }
                break;

            case TEM_MODE_ID_RETAIN:                //  �������
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // �޼Ӹ�� ���� �ʱ� 12V ���� ���� �ð� ����

                if (mu8ColdStrong == TRUE)
                {   // �� �ð�
                    if (mf32ColdTemp >= 5.0f)
                    {
                        mu8TEM_VoltageBuf = TEM_V_ID_12V;
                    }
                    else
                    {
                        mu8TEM_VoltageBuf = TEM_V_ID_5V;
                    }
                }
                else
                {   // �� �ð�
                    if (mf32ColdTemp >= 12.0f)
                    {
                        mu8TEM_VoltageBuf = TEM_V_ID_12V;
                    }
                    else
                    {
                        mu8TEM_VoltageBuf = TEM_V_ID_5V;
                    }
                }

                mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];

                Set_CoolFanActMode(FAN_ID_18V_ON);
                break;

            case TEM_MODE_ID_ECO:
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // �޼Ӹ�� ���� �ʱ� 12V ���� ���� �ð� ����
                break;

            case TEM_MODE_ID_ECO_9V:
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // �޼Ӹ�� ���� �ʱ� 12V ���� ���� �ð� ����
                break;

            case TEM_MODE_ID_MIN:
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // �޼Ӹ�� ���� �ʱ� 12V ���� ���� �ð� ����
                break;

            default:    // TEM OFF�� ����
                gu16TimerRapidInCon12V = TEM_RAPID_MODE_IN_12V_TIME;        // �޼Ӹ�� ���� �ʱ� 12V ���� ���� �ð� ����

                mu8TEM_VoltageBuf = TEM_V_ID_0V;
                mu16TEM_TDR_DataBuff = gau16TEM_TDR_DataTable[mu8TEM_VoltageBuf];
                Set_CoolFanActMode(FAN_ID_OFF);
                break;
        }

        Set_TEM_Voltage(mu8TEM_VoltageBuf);
        Set_TEM_TDR(mu16TEM_TDR_DataBuff);
    }
}

#endif

// Lib TEM Module ********************************************************************************************

/// @brief      Lib TEM Module Initilaize
/// @details    TEM ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_TEM_Module_Initialize(void)
{
#if (TEM_USE == USE)
    Lib_TEM_Initialize();
#endif
}


/// @brief      Lib TEM Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� TEM ���� ��� Library
/// @param      void
/// @return     void
void Lib_TEM_Module_1ms_Control(void)
{
#if (TEM_USE == USE)
    Lib_TEM_ControlTimer();
#endif
}


/// @brief      Lib TEM Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� TEM ���� ��� Library
/// @param      void
/// @return     void
void Lib_TEM_Module_Control(void)
{
#if (TEM_USE == USE)
    Lib_TEM_Control();
#endif
}
