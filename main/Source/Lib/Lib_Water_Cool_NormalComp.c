/// @file     Lib_Water_Cool_NormalComp.c
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    �Ϲ� Comp �ð� ���� ���� file

#include "Global_Header.h"


#if (NORMAL_COMP_USE == USE)                // �Ϲ� Comp �ð� ���� ����

/// @brief      ���� ���� �µ� ���� ������ ����
/// @details    ���� ��� �ϱ� ���� ���� �µ� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    F32 gf32TempFirstCoolOff;               /// @brief  ù �⵿�� �ð��� OFF �µ�(������, �Ҽ���)
    F32 gf32TempFirstCoolOn;                /// @brief  ù �⵿�� �ð��� ON �µ�(������, �Ҽ���)
    U16 gu16FirstCoolOffDelay;              /// @brief  �ð��� OFF�� Delay(�߰� �⵿) �ð�(@100ms)
    F32 gf32TempCoolOff;                    /// @brief  ��⵿�� �ð��� OFF �µ�(������, �Ҽ���)
    F32 gf32TempCoolOn;                     /// @brief  ��⵿�� �ð��� ON �µ�(������, �Ҽ���)
    U16 gu16CoolOffDelay;                   /// @brief  �ð��� OFF�� Delay(�߰� �⵿) �ð�(@100ms)
}   SCoolTable_T;


/// @brief      �ð��� ���� �µ� ���� ���̺�
/// @details    �ܱ�µ� �� ���� ��� ���¿� ���� �ܰ躰 �ð� On, Off ���� �µ��� �� �ð� Off�� �����ð��� �����Ѵ�
__far const SCoolTable_T  SCoolData[POWERSAVE_ID_MAX][COOL_STEP_ID_MAX] = {
// ù�⵿ �ð� OFF �µ�, ù�⵿ �ð� ON �µ�, ù�⵿ �ð� OFF Delay �ð�, ��⵿ �ð� OFF �µ�, ��⵿ �ð� ON �µ�, ��⵿ �ð� OFF Delay �ð�
    // �Ϲ� ����(���� ����X)
    {
        {8.0f,      6.0f,   0,          8.0f,       6.0f,   0    },     // �ܱ� �µ� ������
        {8.0f,      6.0f,   9000,       8.0f,       6.0f,   9000 },     // �ܱ� �µ��� ���� ���� 1�ܰ�
        {8.0f,      6.0f,   9000,       8.0f,       6.0f,   9000 },     // �ܱ� �µ��� ���� ���� 2�ܰ�
        {8.0f,      6.0f,   12000,      8.0f,       6.0f,   12000},     // �ܱ� �µ��� ���� ���� 3�ܰ�
        {8.0f,      6.0f,   12000,      8.0f,       6.0f,   12000}      // �ܱ� �µ��� ���� ���� 4�ܰ�
    },

    // �Ϲ� ����
    {
        {10.0f,     6.0f,   0,          10.0f,      6.0f,   0    },     // �ܱ� �µ� ������
        {10.0f,     6.0f,   9000,       10.0f,      6.0f,   9000 },     // �ܱ� �µ��� ���� ���� 1�ܰ�
        {10.0f,     6.0f,   9000,       10.0f,      6.0f,   9000 },     // �ܱ� �µ��� ���� ���� 2�ܰ�
        {10.0f,     6.0f,   12000,      10.0f,      6.0f,   12000},     // �ܱ� �µ��� ���� ���� 3�ܰ�
        {10.0f,     6.0f,   12000,      10.0f,      6.0f,   12000}      // �ܱ� �µ��� ���� ���� 4�ܰ�
    },

    // �̻�� ����
    {
        {10.0f,     6.0f,   0,          10.0f,      6.0f,   0    },     // �ܱ� �µ� ������
        {10.0f,     6.0f,   9000,       10.0f,      6.0f,   9000 },     // �ܱ� �µ��� ���� ���� 1�ܰ�
        {10.0f,     6.0f,   9000,       10.0f,      6.0f,   9000 },     // �ܱ� �µ��� ���� ���� 2�ܰ�
        {10.0f,     6.0f,   12000,      10.0f,      6.0f,   12000},     // �ܱ� �µ��� ���� ���� 3�ܰ�
        {10.0f,     6.0f,   12000,      10.0f,      6.0f,   12000}      // �ܱ� �µ��� ���� ���� 4�ܰ�
    }
};

U8 gu8ColdOnOff = 0;                        /// @brief  �ü� ��� On/Off ����
U8 gu8ColdStrong = 0;                       /// @brief  �ü� ��� ���� ����

U8 gu8CoolControlPeriodTimer = 0;           /// @brief  �ð� ���� �ð� �ֱ� Ÿ�̸�

U8 gu8FirstCompAct = 0;                     /// @brief  �ð� ù �⵿ ���� - 0(ù �⵿ �ƴ�), 1(ù �⵿��)

U8 gu8ColdActStep = 0;                      /// @brief  �ܱ� �µ� ���ǿ� ���� �ð� ���� Step - 0~4�ܰ�
F32 gf32ColdTargetHighTemp = 0;             /// @brief  �ð� ���� OFF ���� �µ�
F32 gf32ColdTargetLowTemp = 0;              /// @brief  �ð� ���� ON ���� �µ�

U16 gu16CoolDelayTimer = 0;                 /// @brief  �ð� ���� �� ���� �ð� Ÿ�̸�
U16 gu16CoolerActTimer = 0;                 /// @brief  �ð� ���� ���� �ð� Ÿ�̸�
U16 gu16CoolMoreActTimer = 0;               /// @brief  �ð� ��ǥ �µ� ���� �� �߰� ���� �ð�(�ð� OFF�� ���� �ð�) Ÿ�̸�

U8 gu8ManualCoolTestModeTest = 0;           /// @brief  ���� �ð� �׽�Ʈ ���� ���� - 0(�̵���), 1(����)
U8 gu8ManualCoolTestModeAct = 0;            /// @brief  ���� �ð� �׽�Ʈ �� Comp ���� ���� - 0(OFF), 1(ON)

U16 gu16ColdOffTime = 0;                    /// @brief  �ü� ��� OFF �� ����ð� Ÿ�̸�



/// @brief      ������ Comp �ð� ���� ���� Flag & ���� �ʱ�ȭ
/// @details    ������ Comp �ð� ���۰� ���õ� �������� �ʱ�ȭ �Ѵ�
/// @param      void
/// @return     void
void Lib_NormalComp_Initialize(void)
{
    gu8ColdOnOff = 0;
    gu8ColdStrong = 0;

    gu8CoolControlPeriodTimer = 0;

    gu8FirstCompAct = 0;

    gu8ColdActStep = 0;
    gf32ColdTargetHighTemp = 0;
    gf32ColdTargetLowTemp = 0;

    gu16CoolDelayTimer = 0;
    gu16CoolerActTimer = 0;
    gu16CoolMoreActTimer = 0;

    gu8ManualCoolTestModeTest = 0;
    gu8ManualCoolTestModeAct = 0;

    gu16ColdOffTime = 0;
}


/// @brief      ������ Comp ���� �ð� Ÿ�̸�(@1ms)
/// @details    1ms ���ͷ�Ʈ �ȿ��� ������ Comp ���� �ð� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void NormalComp_ControlTimer(void)
{
    if (gu8CoolControlPeriodTimer < NORMAL_COMP_CONTROL_TIME_PERIOD)
    {
        gu8CoolControlPeriodTimer++;
    }
}


/// @brief      �ü� ��� ON/OFF ���� ���� �Լ�
/// @details    �ü� ��� ���°� ON���� OFF���� �����Ѵ�
/// @param      mu8Status : �ü� ��� ���� ���� : 0(OFF), 1(ON)
/// @return     void
void Set_ColdFuncStatus(U8 mu8Status)
{
    gu8ColdOnOff = mu8Status;
}


/// @brief      �ü� ��� ON/OFF ���� Ȯ�� �Լ�
/// @details    �ü� ��� ���°� ON���� OFF ���� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ü� ��� ���� ���� : 0(OFF), 1(ON)
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


/// @brief      ������ Comp  ���� �׽�Ʈ ���� �Լ�(@100ms)
/// @details    ������ Comp ���� �׽�Ʈ ������ �����Ѵ�
/// @param      void
/// @return     void
void ManualCompTest_Control(void)
{
    U8 mu8CompStatus = 0;

    if (gu8ManualCoolTestModeTest == SET)
    {       // �ð� �׽�Ʈ ���� ��
        if (gu8ManualCoolTestModeAct == SET)
        {   // �ð� �׽�Ʈ ���� ��
            if (gu16CoolDelayTimer >= DELAY_TIME_BEFORE_COMP_ON)
            {
                Set_Comp(ON, 0);
            }
        }
        else
        {   // �ð� �׽�Ʈ ���� ����
            mu8CompStatus = Get_Comp_Status();
            if (mu8CompStatus == TRUE)
            {
                Set_Comp(OFF, 0);
                gu16CoolDelayTimer = 0;   // ��ð� ���� Delay 5min
            }
        }
    }
}


/// @brief      ������ Comp ���� �׽�Ʈ ���� ���� ����
/// @details    ������ Comp ���� �׽�Ʈ ���� ���θ� �����Ѵ�
/// @param      mu8Status : ������ Comp ���� �׽�Ʈ ���� - 0(�̵���), 1(����)
/// @return     void
void Set_ManualCompTest(U8 mu8Status)
{
    if (mu8Status)
    {
        gu8ManualCoolTestModeTest = SET;
    }
    else
    {
        gu8ManualCoolTestModeTest = CLEAR;
    }
}


/// @brief      ������ Comp ���� �׽�Ʈ ���� ���� Ȯ��
/// @details    ������ Comp ���� �׽�Ʈ ���� ���θ� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ Comp ���� �׽�Ʈ ���� - 0(�̵���), 1(����)
U8 Get_ManualCompTest(void)
{
    if (gu8ManualCoolTestModeTest)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      ������ Comp ���� �׽�Ʈ �� Comp On/Off ����
/// @details    ������ Comp ���� �׽�Ʈ �� Comp On/Off ���¸� �����Ѵ�
/// @param      mu8Status : ������ Comp ���� - 0(OFF), 1(ON)
/// @return     void
void Set_ManualCompTestAct(U8 mu8OnOff)
{
    if (mu8OnOff)
    {
        gu8ManualCoolTestModeAct = SET;
        gu16CoolDelayTimer = DELAY_TIME_BEFORE_COMP_ON;         // �ٷ� �׽�Ʈ ������ �� �ֵ��� Comp ���� �� Delay�� ������ �ִ�� ����
    }
    else
    {
        gu8ManualCoolTestModeAct = CLEAR;
    }
}


/// @brief      ������ Comp ���� �׽�Ʈ �� Comp On/Off ���� ���� Ȯ��
/// @details    ������ Comp ���� �׽�Ʈ �� Comp On/Off ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ������ Comp ���� - 0(OFF), 1(ON)
U8 Get_ManualCompTestAct(void)
{
    if (gu8ManualCoolTestModeAct)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      �ð� ���� ���� �ð� Counter �Լ�(@100ms)
/// @details    �ð� ���� ���� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void CompActTimeCounter(void)
{
    U8 mu8CompOn = 0;
    U8 mu8ColdOn = 0;
    F32 mf32ColdTemp = 0;

    static U8 mu8CompActTimerSec = 0;
    static U16 mu16CompActTimerMin = 0;

    if (++mu8CompActTimerSec >= 10)
    {   // 1�� ����
        mu8CompActTimerSec = 0;

        mu8CompOn = Get_Comp_Status();      // Comp On/Off ���� Ȯ��
        mf32ColdTemp = Get_Temp(COOL_THERMISTOR_1_ID);      // �ü� �µ� ���� ���� Ȯ��
        mu8ColdOn = Get_ColdOnOff();        // �ð� ��� ON/OFF ���� Ȯ��

        // �ð� ���� �� ���� �ð� ī���� ***********************************************************
        if (mu8CompOn == FALSE)
        {   // Comp Off ���¸�
            if (gu16CoolDelayTimer < DELAY_TIME_BEFORE_COMP_ON)
            {
                gu16CoolDelayTimer++;
            }
        }

        // Comp ���� �ִ� �ð� ��� ī���� ********************************************************
        if (mu8CompOn == TRUE)
        {   // Comp On ���¸�
            if (gu16CoolerActTimer < COMP_ON_MAX_TIME)
            {
                gu16CoolerActTimer++;

                if (mf32ColdTemp < 1.0f)
                {   // �ü� �µ� ������ 1�� �̸��� �Ǵ� ���
                    gu16CoolerActTimer = COMP_ON_MAX_TIME;   // �ִ� �ð� ���� ����
                }
            }
        }
        else
        {   // Comp Off ���¸�
            gu16CoolerActTimer = 0;
        }

        // �ð� ��� OFF ��� �ð� ī���� **********************************************************
        if (++mu16CompActTimerMin >= 60)
        {   // 1�� ����
            if (mu8ColdOn == FALSE)
            {   // �ð� ����� ���� ���
                if (gu16ColdOffTime < COMP_OFF_CHECK_MAX_TIME)
                {
                    gu16ColdOffTime++;
                }
            }
            else
            {   // �ð� ����� ���� ���
                gu16ColdOffTime = 0;
            }
        }
    }
}


/// @brief      �ü� ��� OFF ���� ��� �ð� �ִ� ��� �Ϸ� Ȯ�� �Լ�
/// @details    �ü� ����� OFF�� ���·� ������ �ִ� ����ð��� ����Ͽ����� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ü� ��� OFF ���·� ���� �ִ� �ð� ��� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_ColdOffMaxTimeEnd(void)
{
    if (gu16ColdOffTime >= COMP_OFF_CHECK_MAX_TIME)
    {   // �ִ� ��� �ð� ��� �Ϸ�
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Comp ���� �ִ� ���� �ð� ��� �Ϸ� Ȯ�� �Լ�
/// @details    Comp ���� �ִ� ���� �ð��� ����Ͽ����� ���θ� üũ�Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Comp ���� �ִ� �ð� ��� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_CompActMaxTimeEnd(void)
{
    if (gu16CoolerActTimer >= COMP_ON_MAX_TIME)
    {   // �ִ� ��� �ð� ��� �Ϸ�
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Comp ���� �� ���� �ð� ��� �Ϸ� Ȯ�� �Լ�
/// @details    Comp ���� �� ���� �ð� ����� �Ϸ��Ͽ����� ���θ� üũ�Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Comp ���� �� ���� �ð� ��� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_CompOnDelayTimeEnd(void)
{
    if (gu16CoolDelayTimer >= DELAY_TIME_BEFORE_COMP_ON)
    {   // ��� �ð� ��� �Ϸ�
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Comp ���� �� ���� ��� �ð� Ȯ�� �Լ�
/// @details    Comp ���� �� ���� ��� �ð��� Ȯ���Ͽ� �� ���� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Comp ���� �� ���� ��� �ð�(1Sec ����)
U16 Get_CompOnDelayTime(void)
{
    return  gu16CoolDelayTimer;
}


/// @brief      �ܱ� �µ��� ���� �ð� ���� Step ���� �Լ�
/// @details    �ܱ� �µ� ���¿� ���� �ð� ���� Step�� �����Ѵ�
/// @param      void
/// @return     void
void CoolStepCheck_AirTemp(void)
{
    U8 mu8AirError = 0;
    F32 mf32AirTemp = 0;

    mu8AirError = Get_ErrorStatus(ERROR_ID_AIR_TEMP_1);     // �ܱ� �µ� ���� ���� �߻� ���� Ȯ��

    if (mu8AirError == TRUE)
    {   // �ܱ� �µ� ���� �߻���
        gu8ColdActStep = COOL_STEP_ID_AIR_TEMP_ERROR;
    }
    else
    {
        mf32AirTemp = Get_Temp(AIR_THERMISTOR_ID);          // �ܱ� �µ� Ȯ��

        if (mf32AirTemp < NORMAL_COMP_CONTROL_STEP1_ENV_TEMP)
        {   // �ܱ� 10�� �̸�
            gu8ColdActStep = COOL_STEP_ID_STEP_1;
        }
        else if (mf32AirTemp < NORMAL_COMP_CONTROL_STEP2_ENV_TEMP)
        {   // �ܱ� 20�� �̸�
            gu8ColdActStep = COOL_STEP_ID_STEP_2;
        }
        else if (mf32AirTemp < NORMAL_COMP_CONTROL_STEP3_ENV_TEMP)
        {   // �ܱ� 30�� �̸�
            gu8ColdActStep = COOL_STEP_ID_STEP_3;
        }
        else
        {   // �ܱ� 30�� �̻�
            gu8ColdActStep = COOL_STEP_ID_STEP_4;
        }
    }
}


/// @brief      �ܱ� �µ��� ���� �ð� ���� Step Ȯ�� �Լ�
/// @details    �ܱ� �µ� ���¿� ���� �ð� ���� Step�� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �ܱ� �µ� ���¿� ���� �ð� ���� Step - 0~4�ܰ�
U8 Get_CoolStep(void)
{
    return  gu8ColdActStep;
}


/// @brief      �ð� �������� ���� �Լ�(@100ms)
/// @details    �ð� Comp ���� ������ �Ѵ�
/// @param      void
/// @return     void
void Lib_NormalComp_Control(void)
{
    U8 mu8ManualTestStatus = 0;
    U8 mu8ColdOn = 0;
    U8 mu8CompOnDelayEnd = 0;
    U8 mu8ErrorAboutCold = 0;
    U8 mu8FirstLowLevel = 0;
    U8 mu8PowerSaveStatus = 0;
    F32 mf32ColdTemp = 0;
    U8 mu8CoolActMaxEnd = 0;
    U8 mu8CompOn = 0;

#if (NORMAL_HEATER_USE == USE)
    U8 mu8HeaterOn = 0;
#endif

    if (gu8CoolControlPeriodTimer > NORMAL_COMP_CONTROL_TIME_PERIOD)
    {
        gu8CoolControlPeriodTimer = 0;

        CompActTimeCounter();                               // �ð� ���� ���� �ð� Ÿ�̸� ī���� �Լ�

        mu8ManualTestStatus = Get_ManualCompTest();         // ���� �׽�Ʈ ���� ���� Ȯ��
        mu8ColdOn = Get_ColdOnOff();                        // �ü� ��� ON/OFF ���� Ȯ��
        mu8CompOnDelayEnd = Get_CompOnDelayTimeEnd();       // �ð� �� �����ð� �Ϸ� Ȯ��

        mu8ErrorAboutCold = Get_ColdError();                // �ð� ���� ���� �߻� ���� Ȯ��
        mf32ColdTemp = Get_Temp(COOL_THERMISTOR_1_ID);      // �ü� �µ� Ȯ��
        mu8FirstLowLevel = Get_FirstLowLevelCheck();        // ���� �� ���� ������ ���� ���� Ȯ��

        mu8PowerSaveStatus = Get_PowerSaveMode();           // �������� ���� ��� ���� Ȯ��
        mu8CoolActMaxEnd = Get_CompActMaxTimeEnd();         // �ð� �ִ� ���� �ð� ��� �Ϸ� Ȯ��
        mu8CompOn = Get_Comp_Status();                      // Comp On/Off ���� Ȯ��

#if (NORMAL_HEATER_USE == USE)
        mu8HeaterOn = Get_Heater_Status(HEATER_ID_HOT_TANK);    // ���� On/Off ���� Ȯ��
#endif

        if (mu8ManualTestStatus == FALSE)
        {   // �ð� ���� �׽�Ʈ ���� ���� �ƴ� ���
            if (mu8ColdOn == TRUE)
            {   // �ð� ��� ����
                if (mu8CompOnDelayEnd == TRUE)
                {   // Delay �Ϸ� ��
                    if (mu8CoolActMaxEnd == FALSE)
                    {   // ���� �⵿ �ִ� �ð� �̰����
                        if ( (mu8ColdOn == TRUE) && (mu8FirstLowLevel == TRUE) )
                        {   // �ü� ��� ON, ���� ������ ���� ���� �Ϸ��� ���
                            CoolStepCheck_AirTemp();        // �ܱ� �µ��� �ð� ���� �ܰ� ����

                            if (mu8CompOn == FALSE)
                            {   // Comp �̵��� ������ ���
                                if (mf32ColdTemp >= NORMAL_COMP_FIRST_ACT_CHECK_COLD_TEMP)
                                {   // �ʱ� �⵿ ������ ���
                                    gu8FirstCompAct = SET;
                                }
                                else
                                {
                                    gu8FirstCompAct = CLEAR;
                                }
                            }

                            if (mu8ErrorAboutCold == FALSE)
                            {   // �ü� ���� ���� �� �߻���
                                if (gu8FirstCompAct == SET)
                                {
                                    gf32ColdTargetLowTemp   = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gf32TempFirstCoolOff;
                                    gf32ColdTargetHighTemp  = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gf32TempFirstCoolOn;
                                    gu16CoolMoreActTimer    = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gu16FirstCoolOffDelay;
                                }
                                else
                                {   // ��⵿�� ���
                                    gu8FirstCompAct = CLEAR;

                                    gf32ColdTargetLowTemp   = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gf32TempCoolOff;
                                    gf32ColdTargetHighTemp  = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gf32TempCoolOn;
                                    gu16CoolMoreActTimer    = SCoolData[mu8PowerSaveStatus][gu8ColdActStep].gu16CoolOffDelay;
                                }

                                if (mf32ColdTemp > gf32ColdTargetHighTemp)
                                {   // �ü� �µ��� ���غ��� ���� ���
                                    if (mu8CompOn == FALSE)
                                    {   // Comp �̵��� ������ ���
                                        Set_Comp(ON, 0);
#if (NORMAL_HEATER_USE == USE)
                                        if (mu8HeaterOn == FALSE)
                                        {   // ���Ͱ� ���� �ִ� ���
                                            Set_HotOnDelay(10);         // ���� ON delay 1��
                                        }
#endif
                                    }
                                }
                                else if (mf32ColdTemp < gf32ColdTargetLowTemp)
                                {   // �ü� �µ��� ���غ��� ���� ���
                                    if (mu8CompOn == TRUE)
                                    {   // Comp ���� ���̸�
                                        if (!gu16CoolMoreActTimer)
                                        {
                                            gu16CoolDelayTimer = 0;
                                            Set_Comp(OFF, 0);
                                        }
                                        else
                                        {
                                            gu16CoolMoreActTimer--;
                                        }
                                    }
                                }
                                else
                                {

                                }
                            }
                            else
                            {   // �ü� ���� ���� �߻���
                                if (mu8CompOn == TRUE)
                                {   // Comp ���� ���̸�
                                    gu16CoolDelayTimer = 0;
                                    gu16CoolMoreActTimer = 0;           // �ð� �߰� �⵿ �ð� 0
                                    Set_Comp(OFF, 0);
                                }
                            }
                        }
                        else
                        {   // �ü� ��� OFF �Ǵ� ���� ������ ���� ���� �̿Ϸ��� ���
                            if (mu8CompOn == TRUE)
                            {   // Comp ���� ���̸�
                                gu16CoolDelayTimer = 0;
                                gu16CoolMoreActTimer = 0;           // �ð� �߰� �⵿ �ð� 0
                                Set_Comp(OFF, 0);
                            }
                        }
                    }
                    else
                    {   // ���� �⵿ �ִ� �ð� �Ϸ��
                        if (mu8CompOn == TRUE)
                        {   // Comp ���� ���̸�
                            gu16CoolDelayTimer = 0;
                            gu16CoolMoreActTimer = 0;           // �ð� �߰� �⵿ �ð� 0
                            Set_Comp(OFF, 0);
                        }
                    }
                }
                else
                {   // ��ð� Delay ��
                    gu16CoolMoreActTimer = 0;           // �ð� �߰� �⵿ �ð� 0
                    Set_Comp(OFF, 0);                   // Comp OFF
                }
            }
            else
            {   // �ð� ��� �̻���
                if (mu8CompOn == TRUE)
                {   // Comp ���� ���̸�
                    gu16CoolDelayTimer = 0;
                    gu16CoolMoreActTimer = 0;           // �ð� �߰� �⵿ �ð� 0
                    Set_Comp(OFF, 0);
                }
            }

        }
    }
}

#endif

// Lib NormalComp Module ********************************************************************************************

/// @brief      Lib NormalComp Module Initilaize
/// @details    ������ Comp ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_NormalComp_Module_Initialize(void)
{
#if (NORMAL_COMP_USE == USE)
    Lib_NormalComp_Initialize();
#endif
}


/// @brief      Lib NormalComp Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� ������ Comp ���� ��� Library
/// @param      void
/// @return     void
void Lib_NormalComp_Module_1ms_Control(void)
{
#if (NORMAL_COMP_USE == USE)
    NormalComp_ControlTimer();
#endif
}


/// @brief      Lib NormalComp Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� ������ Comp ���� ��� Library
/// @param      void
/// @return     void
void Lib_NormalComp_Module_Control(void)
{
#if (NORMAL_COMP_USE == USE)
    Lib_NormalComp_Control();
#endif
}

