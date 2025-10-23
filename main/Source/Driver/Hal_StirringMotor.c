/// @file     Hal_StirringMotor.c
/// @date     2025/04/08
/// @author   Jaejin Ham
/// @brief    ���� ���� Control file


#include "Global_Header.h"


#if (STIRRING_MOTOR_COUNT > 0)

U8 gu8StirringMotorControTimer = 0;                 /// @brief  ���� ���� ���� ���� �ֱ� Ÿ�̸�

#if (STIRRING_MOTOR_1_PWM_USE > STIRRING_MOTOR_PWM_NO_USE)
U16 gu16StirringMotor1PWMValue = 0;                 /// @brief  ���� ����1 PWM ���� �������� ������
U8 gu8StirringMotor1OnDelay = 0;                    /// @brief  PWM ���� �� ���� ON ������ Delay
U8 gu8StirringMotor1OffDelay = 0;                   /// @brief  PWM ���� �� ���� OFF ������ Delay
#endif

#if (STIRRING_MOTOR_2_PWM_USE > STIRRING_MOTOR_PWM_NO_USE)
U16 gu16StirringMotor2PWMValue = 0;                 /// @brief  ���� ����2 PWM ���� �������� ������
U8 gu8StirringMotor2OnDelay = 0;                    /// @brief  PWM ���� �� ���� ON ������ Delay
U8 gu8StirringMotor2OffDelay = 0;                   /// @brief  PWM ���� �� ���� OFF ������ Delay
#endif

#if (STIRRING_MOTOR_3_PWM_USE > STIRRING_MOTOR_PWM_NO_USE)
U16 gu16StirringMotor3PWMValue = 0;                 /// @brief  ���� ����3 PWM ���� �������� ������
U8 gu8StirringMotor3OnDelay = 0;                    /// @brief  PWM ���� �� ���� ON ������ Delay
U8 gu8StirringMotor3OffDelay = 0;                   /// @brief  PWM ���� �� ���� OFF ������ Delay
#endif

#if (STIRRING_MOTOR_4_PWM_USE > STIRRING_MOTOR_PWM_NO_USE)
U16 gu16StirringMotor4PWMValue = 0;                 /// @brief  ���� ����4 PWM ���� �������� ������
U8 gu8StirringMotor4OnDelay = 0;                    /// @brief  PWM ���� �� ���� ON ������ Delay
U8 gu8StirringMotor4OffDelay = 0;                   /// @brief  PWM ���� �� ���� OFF ������ Delay
#endif

#if (STIRRING_MOTOR_5_PWM_USE > STIRRING_MOTOR_PWM_NO_USE)
U16 gu16StirringMotor5PWMValue = 0;                 /// @brief  ���� ����5 PWM ���� �������� ������
U8 gu8StirringMotor5OnDelay = 0;                    /// @brief  PWM ���� �� ���� ON ������ Delay
U8 gu8StirringMotor5OffDelay = 0;                   /// @brief  PWM ���� �� ���� OFF ������ Delay
#endif


/// @brief  ���� ���� ���� ���� �ڷ���
typedef struct {
    U8  au8Status[STIRRING_MOTOR_ID_MAX_COUNT];             // Status(1:ON, 0:OFF)
    U16  au16DelayTime[STIRRING_MOTOR_ID_MAX_COUNT];        // Control Delay Time
}   SStirringMotorData_T;

SStirringMotorData_T      SStirringMotorControlData;        // ���� ���� Control Data


/// @brief      ���� ����  Initialize
/// @details    ���� ���� ����� ���õ� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_StirringMotor_Initialize(void)
{
    MEMSET( (void __FAR *)&SStirringMotorControlData, 0x00, sizeof(SStirringMotorData_T) );

    Hal_All_StirringMotor_Off();

    gu8StirringMotorControTimer = 0;

#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
    gu16StirringMotor1PWMValue = 0;
    gu8StirringMotor1OnDelay = STIRRING_MOTOR_1_PWM_ON_DELAY;
    gu8StirringMotor1OffDelay = STIRRING_MOTOR_1_PWM_OFF_DELAY;
#elif(STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
    gu8StirringMotor1OnDelay = STIRRING_MOTOR_1_TRIG_ON_DELAY;
    gu8StirringMotor1OffDelay = STIRRING_MOTOR_1_TRIG_OFF_DELAY;
#else
#endif

#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
    gu16StirringMotor2PWMValue = 0;
    gu8StirringMotor2OnDelay = STIRRING_MOTOR_2_PWM_ON_DELAY;
    gu8StirringMotor2OffDelay = STIRRING_MOTOR_2_PWM_OFF_DELAY;
#elif(STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
    gu8StirringMotor2OnDelay = STIRRING_MOTOR_2_TRIG_ON_DELAY;
    gu8StirringMotor2OffDelay = STIRRING_MOTOR_2_TRIG_OFF_DELAY;
#else
#endif

#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
    gu16StirringMotor3PWMValue = 0;
    gu8StirringMotor3OnDelay = STIRRING_MOTOR_3_PWM_ON_DELAY;
    gu8StirringMotor3OffDelay = STIRRING_MOTOR_3_PWM_OFF_DELAY;
#elif(STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
    gu8StirringMotor3OnDelay = STIRRING_MOTOR_3_TRIG_ON_DELAY;
    gu8StirringMotor3OffDelay = STIRRING_MOTOR_3_TRIG_OFF_DELAY;
#else
#endif

#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
    gu16StirringMotor4PWMValue = 0;
    gu8StirringMotor4OnDelay = STIRRING_MOTOR_4_PWM_ON_DELAY;
    gu8StirringMotor4OffDelay = STIRRING_MOTOR_4_PWM_OFF_DELAY;
#elif(STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
    gu8StirringMotor4OnDelay = STIRRING_MOTOR_4_TRIG_ON_DELAY;
    gu8StirringMotor4OffDelay = STIRRING_MOTOR_4_TRIG_OFF_DELAY;
#else
#endif

#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
    gu16StirringMotor5PWMValue = 0;
    gu8StirringMotor5OnDelay = STIRRING_MOTOR_5_PWM_ON_DELAY;
    gu8StirringMotor5OffDelay = STIRRING_MOTOR_5_PWM_OFF_DELAY;
#elif(STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
    gu8StirringMotor5OnDelay = STIRRING_MOTOR_5_TRIG_ON_DELAY;
    gu8StirringMotor5OffDelay = STIRRING_MOTOR_5_TRIG_OFF_DELAY;
#else
#endif
}


/// @brief      ���� ����  control Timer(@1ms interrupt)
/// @details    ���� ���� ���� �ֱ� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void StirringMotorControlTimer(void)
{
    if (gu8StirringMotorControTimer < STIRRING_MOTOR_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8StirringMotorControTimer++;
    }
}


/// @brief      ���� ���� control function(@While)
/// @details    ���� ���� ���� ������ �Ѵ�
/// @param      void
/// @return     void
void StirringMotorControl(void)
{
    U8 mu8MotorIDBuf = 0;

    if (gu8StirringMotorControTimer >= STIRRING_MOTOR_CONTROL_TIME_PERIOD)
    {       // 100ms ���� ó��
        gu8StirringMotorControTimer = 0;

        for (mu8MotorIDBuf = 1 ; mu8MotorIDBuf < STIRRING_MOTOR_ID_MAX_COUNT ; mu8MotorIDBuf++)
        {
            if (SStirringMotorControlData.au16DelayTime[mu8MotorIDBuf])
            {
                SStirringMotorControlData.au16DelayTime[mu8MotorIDBuf]--;
            }

            Hal_StirringMotorControl(mu8MotorIDBuf);
        }
    }
}


/// @brief      ���� ���� control function
/// @details    ���� ���� ���� ���°��� �����Ѵ�
/// @param      mu8MotorID : defined Motor ID
///             mu8Status : 1(ON), 0(OFF)
///             mu16Delay : Control Delay Time(@100ms)
/// @return     void
void Set_StirringMotorControl(U8 mu8MotorID, U8 mu8Status, U16 mu16Delay)
{
    if (mu8Status == ON)
    {
        SStirringMotorControlData.au8Status[mu8MotorID] = ON;
        SStirringMotorControlData.au16DelayTime[mu8MotorID] = mu16Delay;
    }
    else
    {
        SStirringMotorControlData.au8Status[mu8MotorID] = OFF;
        SStirringMotorControlData.au16DelayTime[mu8MotorID] = mu16Delay;
    }
}


/// @brief      All ���� ����  OFF
/// @details    ��� ���� ���͸� OFF ���� ��Ų��
/// @param      void
/// @return     void
void Hal_All_StirringMotor_Off(void)
{
    U8 mu8MotorIDBuf = 0;

    for (mu8MotorIDBuf = 1 ; mu8MotorIDBuf < STIRRING_MOTOR_ID_MAX_COUNT ; mu8MotorIDBuf++)
    {
        Set_StirringMotorControl(mu8MotorIDBuf, OFF, 0);
    }
}


/// @brief      ���� ���� Port control function(100ms)
/// @details    ���� ���� HW ��� �Ѵ�
/// @param      mu8MotorID : defined Motor ID
/// @return     void
void Hal_StirringMotorControl(U8 mu8MotorID)
{
    switch (mu8MotorID)
    {
        case 1:     // ID 1
#if (STIRRING_MOTOR_COUNT >= 1)
            if (SStirringMotorControlData.au16DelayTime[mu8MotorID] == 0)
            {
                if (SStirringMotorControlData.au8Status[mu8MotorID] == ON)
                {       // On �̸�
                    Hal_StirringMotor_1_On();
                }
                else
                {       // Off �̸�
                    Hal_StirringMotor_1_Off();
                }
            }
#endif
            break;

        case 2:     // ID 2
#if (STIRRING_MOTOR_COUNT >= 2)
            if (SStirringMotorControlData.au16DelayTime[mu8MotorID] == 0)
            {
                if (SStirringMotorControlData.au8Status[mu8MotorID] == ON)
                {
                    Hal_StirringMotor_2_On();
                }
                else
                {
                    Hal_StirringMotor_2_Off();
                }
            }
#endif
            break;

        case 3:     // ID 3
#if (STIRRING_MOTOR_COUNT >= 3)
            if (SStirringMotorControlData.au16DelayTime[mu8MotorID] == 0)
            {
                if (SStirringMotorControlData.au8Status[mu8MotorID] == ON)
                {
                    Hal_StirringMotor_3_On();
                }
                else
                {
                    Hal_StirringMotor_3_Off();
                }
            }
#endif
            break;

        case 4:     // ID 4
#if (STIRRING_MOTOR_COUNT >= 4)
            if (SStirringMotorControlData.au16DelayTime[mu8MotorID] == 0)
            {
                if (SStirringMotorControlData.au8Status[mu8MotorID] == ON)
                {
                    Hal_StirringMotor_4_On();
                }
                else
                {
                    Hal_StirringMotor_4_Off();
                }
            }
#endif
            break;

        case 5:     // ID 5
#if (STIRRING_MOTOR_COUNT >= 5)
            if (SStirringMotorControlData.au16DelayTime[mu8MotorID] == 0)
            {
                if (SStirringMotorControlData.au8Status[mu8MotorID] == ON)
                {
                    Hal_StirringMotor_5_On();
                }
                else
                {
                    Hal_StirringMotor_5_Off();
                }
            }
#endif
            break;

        default:
#if (STIRRING_MOTOR_COUNT >= 1)
            Hal_StirringMotor_1_Off();
#endif
#if (STIRRING_MOTOR_COUNT >= 2)
            Hal_StirringMotor_2_Off();
#endif
#if (STIRRING_MOTOR_COUNT >= 3)
            Hal_StirringMotor_3_Off();
#endif
#if (STIRRING_MOTOR_COUNT >= 4)
            Hal_StirringMotor_4_Off();
#endif
#if (STIRRING_MOTOR_COUNT >= 5)
            Hal_StirringMotor_5_Off();
#endif
            break;
    }
}


/// @brief      ���� ������ ���� HW ���� ���� Ȯ�� �Լ�
/// @details    ID �� �ش��ϴ� ���� ������ ���� HW ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8MotorID : defined Motor ID
/// @return     return : ���� ���� - 0(FALSE, �̵���), 1(TRUE, ����)
U8 Get_StirringMotorStatus_HW(U8 mu8MotorID)
{
    U8 mu8Return = 0;
    U8 mu8Status = 0;

    switch (mu8MotorID)
    {
        case 1:     // Motor ID 1
#if (STIRRING_MOTOR_COUNT >= 1)
            mu8Status = GET_STATUS_STIRRING_MOTOR_1();
#endif
            break;

        case 2:     // Motor ID 2
#if (STIRRING_MOTOR_COUNT >= 2)
            mu8Status = GET_STATUS_STIRRING_MOTOR_2();
#endif
            break;

        case 3:     // Motor ID 3
#if (STIRRING_MOTOR_COUNT >= 3)
            mu8Status = GET_STATUS_STIRRING_MOTOR_3();
#endif
            break;

        case 4:     // Motor ID 4
#if (STIRRING_MOTOR_COUNT >= 4)
            mu8Status = GET_STATUS_STIRRING_MOTOR_4();
#endif
            break;

        case 5:     // Motor ID 5
#if (STIRRING_MOTOR_COUNT >= 5)
            mu8Status = GET_STATUS_STIRRING_MOTOR_5();
#endif
            break;

        default:
            mu8Status = 0;
            break;
    }

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


/// @brief      ���� ���� PWM control function
/// @details    ���� ���� ���� PWM ��� �Ѵ�
/// @param      mu8PumpID : defined Pump ID
///             mu16RegValue : Register Value
/// @return     void
void Set_StirringMotorPWM(U8 mu8MotorID, U16 mu16RegValue)
{
    switch (mu8MotorID)
    {
        case 1:
#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor1PWMValue = mu16RegValue;
#endif
            break;

        case 2:
#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor2PWMValue = mu16RegValue;
#endif
            break;

        case 3:
#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor3PWMValue = mu16RegValue;
#endif
            break;

        case 4:
#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor4PWMValue = mu16RegValue;
#endif
            break;

        case 5:
#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor5PWMValue = mu16RegValue;
#endif
            break;

        default:
#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor1PWMValue = 0;
#endif
#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor2PWMValue = 0;
#endif
#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor3PWMValue = 0;
#endif
#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor4PWMValue = 0;
#endif
#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
            gu16StirringMotor5PWMValue = 0;
#endif
            break;
    }
}


#if (STIRRING_MOTOR_COUNT >= 1)

/// @brief      ���� ���� 1 ON
/// @details    ���� ���� 1 ON ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_1_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_1();

    if (mu8Status == OFF)
    {   // Off ���¸�
#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor1OffDelay = STIRRING_MOTOR_1_PWM_OFF_DELAY;

        if (gu8StirringMotor1OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� On
            if (REGISTER_DEFINE_STIRRING_MOTOR_1_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_1();
                REGISTER_DEFINE_STIRRING_MOTOR_1_PWM = gu16StirringMotor1PWMValue;
                STIRRING_MOTOR_1_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_1();
                REGISTER_DEFINE_STIRRING_MOTOR_1_PWM = gu16StirringMotor1PWMValue;
            }

            TURN_ON_STIRRING_MOTOR_1();
        }
        else
        {
            gu8StirringMotor1OnDelay--;

            if (REGISTER_DEFINE_STIRRING_MOTOR_1_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_1();
                REGISTER_DEFINE_STIRRING_MOTOR_1_PWM = gu16StirringMotor1PWMValue;
                STIRRING_MOTOR_1_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_1();
                REGISTER_DEFINE_STIRRING_MOTOR_1_PWM = gu16StirringMotor1PWMValue;
            }

            TURN_OFF_STIRRING_MOTOR_1();
        }
#elif (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor1OffDelay = STIRRING_MOTOR_1_TRIG_OFF_DELAY;

        if (gu8StirringMotor1OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� Pump On
            TURN_ON_STIRRING_MOTOR_1_TRIG();
            TURN_ON_STIRRING_MOTOR_1();
        }
        else
        {
            gu8StirringMotor1OnDelay--;

            TURN_ON_STIRRING_MOTOR_1_TRIG();
            TURN_OFF_STIRRING_MOTOR_1();
        }
#else
        TURN_ON_STIRRING_MOTOR_1();
#endif
    }
    else
    {   // On �����̸�
#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor1OffDelay = STIRRING_MOTOR_1_PWM_OFF_DELAY;
        TURN_ON_VARIABLE_VOLTAGE_1();
        REGISTER_DEFINE_STIRRING_MOTOR_1_PWM = gu16StirringMotor1PWMValue;
        TURN_ON_STIRRING_MOTOR_1();
#elif (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor1OffDelay = STIRRING_MOTOR_1_TRIG_OFF_DELAY;
        TURN_ON_STIRRING_MOTOR_1_TRIG();
        TURN_ON_STIRRING_MOTOR_1();
#else
        TURN_ON_STIRRING_MOTOR_1();
#endif
    }
}


/// @brief      ���� ���� 1 OFF
/// @details    ���� ���� 1 OFF ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_1_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_1();

    if (mu8Status = ON)
    {   // On �����̸�
#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor1OnDelay = STIRRING_MOTOR_1_PWM_ON_DELAY;

        if (gu8StirringMotor1OffDelay == 0)
        {   // ���� ���� Off ���� �� ���� Delay �� Off
            TURN_OFF_VARIABLE_VOLTAGE_1();
            REGISTER_DEFINE_STIRRING_MOTOR_1_PWM = 0;
            STIRRING_MOTOR_1_PWM_STOP();

            TURN_OFF_STIRRING_MOTOR_1();
        }
        else
        {
            gu8StirringMotor1OffDelay--;

            TURN_OFF_VARIABLE_VOLTAGE_1();
            REGISTER_DEFINE_STIRRING_MOTOR_1_PWM = 0;
            STIRRING_MOTOR_1_PWM_STOP();

            TURN_ON_STIRRING_MOTOR_1();
        }
#elif (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor1OnDelay = STIRRING_MOTOR_1_TRIG_ON_DELAY;

        if (gu8StirringMotor1OffDelay == 0)
        {   // Off �� ���� Delay �� Trig port Off
            TURN_OFF_STIRRING_MOTOR_1_TRIG();
            TURN_OFF_STIRRING_MOTOR_1();
        }
        else
        {
            gu8StirringMotor1OffDelay--;

            TURN_ON_STIRRING_MOTOR_1_TRIG();
            TURN_OFF_STIRRING_MOTOR_1();
        }
#else
        TURN_OFF_STIRRING_MOTOR_1();
#endif
    }
    else
    {   // Off �����̸�
#if (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        TURN_OFF_VARIABLE_VOLTAGE_1();
        gu8StirringMotor1OnDelay = STIRRING_MOTOR_1_PWM_ON_DELAY;
        REGISTER_DEFINE_STIRRING_MOTOR_1_PWM = 0;
        STIRRING_MOTOR_1_PWM_STOP();
        TURN_OFF_STIRRING_MOTOR_1();
#elif (STIRRING_MOTOR_1_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor1OnDelay = STIRRING_MOTOR_1_TRIG_ON_DELAY;
        TURN_OFF_STIRRING_MOTOR_1_TRIG();
        TURN_OFF_STIRRING_MOTOR_1();
#else
        TURN_OFF_STIRRING_MOTOR_1();
#endif
    }
}

#endif


#if (STIRRING_MOTOR_COUNT >= 2)

/// @brief      ���� ���� 2 ON
/// @details    ���� ���� 2 ON ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_2_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_2();

    if (mu8Status == OFF)
    {   // Off ���¸�
#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor2OffDelay = STIRRING_MOTOR_2_PWM_OFF_DELAY;

        if (gu8StirringMotor2OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� On
            if (REGISTER_DEFINE_STIRRING_MOTOR_2_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_2();
                REGISTER_DEFINE_STIRRING_MOTOR_2_PWM = gu16StirringMotor2PWMValue;
                STIRRING_MOTOR_2_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_2();
                REGISTER_DEFINE_STIRRING_MOTOR_2_PWM = gu16StirringMotor2PWMValue;
            }

            TURN_ON_STIRRING_MOTOR_2();
        }
        else
        {
            gu8StirringMotor2OnDelay--;

            if (REGISTER_DEFINE_STIRRING_MOTOR_2_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_2();
                REGISTER_DEFINE_STIRRING_MOTOR_2_PWM = gu16StirringMotor2PWMValue;
                STIRRING_MOTOR_2_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_2();
                REGISTER_DEFINE_STIRRING_MOTOR_2_PWM = gu16StirringMotor2PWMValue;
            }

            TURN_OFF_STIRRING_MOTOR_2();
        }
#elif (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor2OffDelay = STIRRING_MOTOR_2_TRIG_OFF_DELAY;

        if (gu8StirringMotor2OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� Pump On
            TURN_ON_STIRRING_MOTOR_2_TRIG();
            TURN_ON_STIRRING_MOTOR_2();
        }
        else
        {
            gu8StirringMotor2OnDelay--;

            TURN_ON_STIRRING_MOTOR_2_TRIG();
            TURN_OFF_STIRRING_MOTOR_2();
        }
#else
        TURN_ON_STIRRING_MOTOR_2();
#endif
    }
    else
    {   // On �����̸�
#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor2OffDelay = STIRRING_MOTOR_2_PWM_OFF_DELAY;
        TURN_ON_VARIABLE_VOLTAGE_2();
        REGISTER_DEFINE_STIRRING_MOTOR_2_PWM = gu16StirringMotor2PWMValue;
        TURN_ON_STIRRING_MOTOR_2();
#elif (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor2OffDelay = STIRRING_MOTOR_2_TRIG_OFF_DELAY;
        TURN_ON_STIRRING_MOTOR_2_TRIG();
        TURN_ON_STIRRING_MOTOR_2();
#else
        TURN_ON_STIRRING_MOTOR_2();
#endif
    }
}


/// @brief      ���� ���� 2 OFF
/// @details    ���� ���� 2 OFF ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_2_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_2();

    if (mu8Status == ON)
    {   // On �����̸�
#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor2OnDelay = STIRRING_MOTOR_2_PWM_ON_DELAY;

        if (gu8StirringMotor2OffDelay == 0)
        {   // ���� ���� Off ���� �� ���� Delay �� Off
            TURN_OFF_VARIABLE_VOLTAGE_2();
            REGISTER_DEFINE_STIRRING_MOTOR_2_PWM = 0;
            STIRRING_MOTOR_2_PWM_STOP();

            TURN_OFF_STIRRING_MOTOR_2();
        }
        else
        {
            gu8StirringMotor2OffDelay--;

            TURN_OFF_VARIABLE_VOLTAGE_2();
            REGISTER_DEFINE_STIRRING_MOTOR_2_PWM = 0;
            STIRRING_MOTOR_2_PWM_STOP();

            TURN_ON_STIRRING_MOTOR_2();
        }
#elif (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor2OnDelay = STIRRING_MOTOR_2_TRIG_ON_DELAY;

        if (gu8StirringMotor2OffDelay == 0)
        {   // Off �� ���� Delay �� Trig port Off
            TURN_OFF_STIRRING_MOTOR_2_TRIG();
            TURN_OFF_STIRRING_MOTOR_2();
        }
        else
        {
            gu8StirringMotor2OffDelay--;

            TURN_ON_STIRRING_MOTOR_2_TRIG();
            TURN_OFF_STIRRING_MOTOR_2();
        }
#else
        TURN_OFF_STIRRING_MOTOR_2();
#endif
    }
    else
    {   // Off �����̸�
#if (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        TURN_OFF_VARIABLE_VOLTAGE_2();
        gu8StirringMotor2OnDelay = STIRRING_MOTOR_2_PWM_ON_DELAY;
        REGISTER_DEFINE_STIRRING_MOTOR_2_PWM = 0;
        STIRRING_MOTOR_2_PWM_STOP();
        TURN_OFF_STIRRING_MOTOR_2();
#elif (STIRRING_MOTOR_2_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor2OnDelay = STIRRING_MOTOR_2_TRIG_ON_DELAY;
        TURN_OFF_STIRRING_MOTOR_2_TRIG();
        TURN_OFF_STIRRING_MOTOR_2();
#else
        TURN_OFF_STIRRING_MOTOR_2();
#endif
    }
}

#endif


#if (STIRRING_MOTOR_COUNT >= 3)

/// @brief      ���� ���� 3 ON
/// @details    ���� ���� 3 ON ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_3_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_3();

    if (mu8Status == OFF)
    {   // Off ���¸�
#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor3OffDelay = STIRRING_MOTOR_3_PWM_OFF_DELAY;

        if (gu8StirringMotor3OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� On
            if (REGISTER_DEFINE_STIRRING_MOTOR_3_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_3();
                REGISTER_DEFINE_STIRRING_MOTOR_3_PWM = gu16StirringMotor3PWMValue;
                STIRRING_MOTOR_3_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_3();
                REGISTER_DEFINE_STIRRING_MOTOR_3_PWM = gu16StirringMotor3PWMValue;
            }

            TURN_ON_STIRRING_MOTOR_3();
        }
        else
        {
            gu8StirringMotor3OnDelay--;

            if (REGISTER_DEFINE_STIRRING_MOTOR_3_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_3();
                REGISTER_DEFINE_STIRRING_MOTOR_3_PWM = gu16StirringMotor3PWMValue;
                STIRRING_MOTOR_3_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_3();
                REGISTER_DEFINE_STIRRING_MOTOR_3_PWM = gu16StirringMotor3PWMValue;
            }

            TURN_OFF_STIRRING_MOTOR_3();
        }
#elif (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor3OffDelay = STIRRING_MOTOR_3_TRIG_OFF_DELAY;

        if (gu8StirringMotor3OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� Pump On
            TURN_ON_STIRRING_MOTOR_3_TRIG();
            TURN_ON_STIRRING_MOTOR_3();
        }
        else
        {
            gu8StirringMotor3OnDelay--;

            TURN_ON_STIRRING_MOTOR_3_TRIG();
            TURN_OFF_STIRRING_MOTOR_3();
        }
#else
        TURN_ON_STIRRING_MOTOR_3();
#endif
    }
    else
    {   // On �����̸�
#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor3OffDelay = STIRRING_MOTOR_3_PWM_OFF_DELAY;
        TURN_ON_VARIABLE_VOLTAGE_3();
        REGISTER_DEFINE_STIRRING_MOTOR_3_PWM = gu16StirringMotor3PWMValue;
        TURN_ON_STIRRING_MOTOR_3();
#elif (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor3OffDelay = STIRRING_MOTOR_3_TRIG_OFF_DELAY;
        TURN_ON_STIRRING_MOTOR_3_TRIG();
        TURN_ON_STIRRING_MOTOR_3();
#else
        TURN_ON_STIRRING_MOTOR_3();
#endif
    }
}


/// @brief      ���� ���� 3 OFF
/// @details    ���� ���� 3 OFF ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_3_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_3();

    if (mu8Status == ON)
    {   // On �����̸�
#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor3OnDelay = STIRRING_MOTOR_3_PWM_ON_DELAY;

        if (gu8StirringMotor3OffDelay == 0)
        {   // ���� ���� Off ���� �� ���� Delay �� Off
            TURN_OFF_VARIABLE_VOLTAGE_3();
            REGISTER_DEFINE_STIRRING_MOTOR_3_PWM = 0;
            STIRRING_MOTOR_3_PWM_STOP();

            TURN_OFF_STIRRING_MOTOR_3();
        }
        else
        {
            gu8StirringMotor3OffDelay--;

            TURN_OFF_VARIABLE_VOLTAGE_3();
            REGISTER_DEFINE_STIRRING_MOTOR_3_PWM = 0;
            STIRRING_MOTOR_3_PWM_STOP();

            TURN_ON_STIRRING_MOTOR_3();
        }
#elif (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor3OnDelay = STIRRING_MOTOR_3_TRIG_ON_DELAY;

        if (gu8StirringMotor3OffDelay == 0)
        {   // Off �� ���� Delay �� Trig port Off
            TURN_OFF_STIRRING_MOTOR_3_TRIG();
            TURN_OFF_STIRRING_MOTOR_3();
        }
        else
        {
            gu8StirringMotor3OffDelay--;

            TURN_ON_STIRRING_MOTOR_3_TRIG();
            TURN_OFF_STIRRING_MOTOR_3();
        }
#else
        TURN_OFF_STIRRING_MOTOR_3();
#endif
    }
    else
    {   // Off �����̸�
#if (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        TURN_OFF_VARIABLE_VOLTAGE_3();
        gu8StirringMotor3OnDelay = STIRRING_MOTOR_3_PWM_ON_DELAY;
        REGISTER_DEFINE_STIRRING_MOTOR_3_PWM = 0;
        STIRRING_MOTOR_3_PWM_STOP();
        TURN_OFF_STIRRING_MOTOR_3();
#elif (STIRRING_MOTOR_3_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor3OnDelay = STIRRING_MOTOR_3_TRIG_ON_DELAY;
        TURN_OFF_STIRRING_MOTOR_3_TRIG();
        TURN_OFF_STIRRING_MOTOR_3();
#else
        TURN_OFF_STIRRING_MOTOR_3();
#endif
    }
}

#endif

#if (STIRRING_MOTOR_COUNT >= 4)

/// @brief      ���� ���� 4 ON
/// @details    ���� ���� 4 ON ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_4_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_4();

    if (mu8Status == OFF)
    {   // Off ���¸�
#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor4OffDelay = STIRRING_MOTOR_4_PWM_OFF_DELAY;

        if (gu8StirringMotor4OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� On
            if (REGISTER_DEFINE_STIRRING_MOTOR_4_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_4();
                REGISTER_DEFINE_STIRRING_MOTOR_4_PWM = gu16StirringMotor4PWMValue;
                STIRRING_MOTOR_4_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_4();
                REGISTER_DEFINE_STIRRING_MOTOR_4_PWM = gu16StirringMotor4PWMValue;
            }

            TURN_ON_STIRRING_MOTOR_4();
        }
        else
        {
            gu8StirringMotor4OnDelay--;

            if (REGISTER_DEFINE_STIRRING_MOTOR_4_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_4();
                REGISTER_DEFINE_STIRRING_MOTOR_4_PWM = gu16StirringMotor4PWMValue;
                STIRRING_MOTOR_4_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_4();
                REGISTER_DEFINE_STIRRING_MOTOR_4_PWM = gu16StirringMotor4PWMValue;
            }

            TURN_OFF_STIRRING_MOTOR_4();
        }
#elif (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor4OffDelay = STIRRING_MOTOR_4_TRIG_OFF_DELAY;

        if (gu8StirringMotor4OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� Pump On
            TURN_ON_STIRRING_MOTOR_4_TRIG();
            TURN_ON_STIRRING_MOTOR_4();
        }
        else
        {
            gu8StirringMotor4OnDelay--;

            TURN_ON_STIRRING_MOTOR_4_TRIG();
            TURN_OFF_STIRRING_MOTOR_4();
        }
#else
        TURN_ON_STIRRING_MOTOR_4();
#endif
    }
    else
    {   // On �����̸�
#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor4OffDelay = STIRRING_MOTOR_4_PWM_OFF_DELAY;
        TURN_ON_VARIABLE_VOLTAGE_4();
        REGISTER_DEFINE_STIRRING_MOTOR_4_PWM = gu16StirringMotor4PWMValue;
        TURN_ON_STIRRING_MOTOR_4();
#elif (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor4OffDelay = STIRRING_MOTOR_4_TRIG_OFF_DELAY;
        TURN_ON_STIRRING_MOTOR_4_TRIG();
        TURN_ON_STIRRING_MOTOR_4();
#else
        TURN_ON_STIRRING_MOTOR_4();
#endif
    }
}


/// @brief      ���� ���� 4 OFF
/// @details    ���� ���� 4 OFF ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_4_Off(void)
{
    U8 mu8Status = 0l

    mu8Status = GET_STATUS_STIRRING_MOTOR_4();

    if (P_STIRRING_MOTOR_4)
    {   // On �����̸�
#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor4OnDelay = STIRRING_MOTOR_4_PWM_ON_DELAY;

        if (gu8StirringMotor4OffDelay == 0)
        {   // ���� ���� Off ���� �� ���� Delay �� Off
            TURN_OFF_VARIABLE_VOLTAGE_4();
            REGISTER_DEFINE_STIRRING_MOTOR_4_PWM = 0;
            STIRRING_MOTOR_4_PWM_STOP();

            TURN_OFF_STIRRING_MOTOR_4();
        }
        else
        {
            gu8StirringMotor4OffDelay--;

            TURN_OFF_VARIABLE_VOLTAGE_4();
            REGISTER_DEFINE_STIRRING_MOTOR_4_PWM = 0;
            STIRRING_MOTOR_4_PWM_STOP();

            TURN_ON_STIRRING_MOTOR_4();
        }
#elif (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor4OnDelay = STIRRING_MOTOR_4_TRIG_ON_DELAY;

        if (gu8StirringMotor4OffDelay == 0)
        {   // Off �� ���� Delay �� Trig port Off
            TURN_OFF_STIRRING_MOTOR_4_TRIG();
            TURN_OFF_STIRRING_MOTOR_4();
        }
        else
        {
            gu8StirringMotor4OffDelay--;

            TURN_ON_STIRRING_MOTOR_4_TRIG();
            TURN_OFF_STIRRING_MOTOR_4();
        }
#else
        TURN_OFF_STIRRING_MOTOR_4();
#endif
    }
    else
    {   // Off �����̸�
#if (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        TURN_OFF_VARIABLE_VOLTAGE_4();
        gu8StirringMotor4OnDelay = STIRRING_MOTOR_4_PWM_ON_DELAY;
        REGISTER_DEFINE_STIRRING_MOTOR_4_PWM = 0;
        STIRRING_MOTOR_4_PWM_STOP();
        TURN_OFF_STIRRING_MOTOR_4();
#elif (STIRRING_MOTOR_4_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor4OnDelay = STIRRING_MOTOR_4_TRIG_ON_DELAY;
        TURN_OFF_STIRRING_MOTOR_4_TRIG();
        TURN_OFF_STIRRING_MOTOR_4();
#else
        TURN_OFF_STIRRING_MOTOR_4();
#endif
    }
}


#endif

#if (STIRRING_MOTOR_COUNT >= 5)

/// @brief      ���� ���� 5 ON
/// @details    ���� ���� 5 ON ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_5_On(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_5();

    if (mu8Status == OFF)
    {   // Off ���¸�
#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor5OffDelay = STIRRING_MOTOR_5_PWM_OFF_DELAY;

        if (gu8StirringMotor5OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� On
            if (REGISTER_DEFINE_STIRRING_MOTOR_5_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_5();
                REGISTER_DEFINE_STIRRING_MOTOR_5_PWM = gu16StirringMotor5PWMValue;
                STIRRING_MOTOR_5_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_5();
                REGISTER_DEFINE_STIRRING_MOTOR_5_PWM = gu16StirringMotor5PWMValue;
            }

            TURN_ON_STIRRING_MOTOR_5();
        }
        else
        {
            gu8StirringMotor5OnDelay--;

            if (REGISTER_DEFINE_STIRRING_MOTOR_5_PWM == 0)
            {
                TURN_ON_VARIABLE_VOLTAGE_5();
                REGISTER_DEFINE_STIRRING_MOTOR_5_PWM = gu16StirringMotor5PWMValue;
                STIRRING_MOTOR_5_PWM_START();
            }
            else
            {
                TURN_ON_VARIABLE_VOLTAGE_5();
                REGISTER_DEFINE_STIRRING_MOTOR_5_PWM = gu16StirringMotor5PWMValue;
            }

            TURN_OFF_STIRRING_MOTOR_5();
        }
#elif (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor5OffDelay = STIRRING_MOTOR_5_TRIG_OFF_DELAY;

        if (gu8StirringMotor5OnDelay == 0)
        {   // ���� ���� ON ���� �� ���� Delay �� Pump On
            TURN_ON_STIRRING_MOTOR_5_TRIG();
            TURN_ON_STIRRING_MOTOR_5();
        }
        else
        {
            gu8StirringMotor5OnDelay--;

            TURN_ON_STIRRING_MOTOR_5_TRIG();
            TURN_OFF_STIRRING_MOTOR_5();
        }
#else
        TURN_ON_STIRRING_MOTOR_5();
#endif
    }
    else
    {   // On �����̸�
#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor5OffDelay = STIRRING_MOTOR_5_PWM_OFF_DELAY;
        TURN_ON_VARIABLE_VOLTAGE_5();
        REGISTER_DEFINE_STIRRING_MOTOR_5_PWM = gu16StirringMotor5PWMValue;
        TURN_ON_STIRRING_MOTOR_5();
#elif (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor5OffDelay = STIRRING_MOTOR_5_TRIG_OFF_DELAY;
        TURN_ON_STIRRING_MOTOR_5_TRIG();
        TURN_ON_STIRRING_MOTOR_5();
#else
        TURN_ON_STIRRING_MOTOR_5();
#endif
    }
}


/// @brief      ���� ���� 5 OFF
/// @details    ���� ���� 5 OFF ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_5_Off(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_STIRRING_MOTOR_5();

    if (mu8Status == ON)
    {   // On �����̸�
#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        gu8StirringMotor5OnDelay = STIRRING_MOTOR_5_PWM_ON_DELAY;

        if (gu8StirringMotor5OffDelay == 0)
        {   // ���� ���� Off ���� �� ���� Delay �� Off
            TURN_OFF_VARIABLE_VOLTAGE_5();
            REGISTER_DEFINE_STIRRING_MOTOR_5_PWM = 0;
            STIRRING_MOTOR_5_PWM_STOP();

            TURN_OFF_STIRRING_MOTOR_5();
        }
        else
        {
            gu8StirringMotor5OffDelay--;

            TURN_OFF_VARIABLE_VOLTAGE_5();
            REGISTER_DEFINE_STIRRING_MOTOR_5_PWM = 0;
            STIRRING_MOTOR_5_PWM_STOP();

            TURN_ON_STIRRING_MOTOR_5();
        }
#elif (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor5OnDelay = STIRRING_MOTOR_5_TRIG_ON_DELAY;

        if (gu8StirringMotor5OffDelay == 0)
        {   // Off �� ���� Delay �� Trig port Off
            TURN_OFF_STIRRING_MOTOR_5_TRIG();
            TURN_OFF_STIRRING_MOTOR_5();
        }
        else
        {
            gu8StirringMotor5OffDelay--;

            TURN_ON_STIRRING_MOTOR_5_TRIG();
            TURN_OFF_STIRRING_MOTOR_5();
        }
#else
        TURN_OFF_STIRRING_MOTOR_5();
#endif
    }
    else
    {   // Off �����̸�
#if (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_REGISTER_USE)
        TURN_OFF_VARIABLE_VOLTAGE_5();
        gu8StirringMotor5OnDelay = STIRRING_MOTOR_5_PWM_ON_DELAY;
        REGISTER_DEFINE_STIRRING_MOTOR_5_PWM = 0;
        STIRRING_MOTOR_5_PWM_STOP();
        TURN_OFF_STIRRING_MOTOR_5();
#elif (STIRRING_MOTOR_5_PWM_USE == STIRRING_MOTOR_PWM_TRIG_USE)
        gu8StirringMotor5OnDelay = STIRRING_MOTOR_5_TRIG_ON_DELAY;
        TURN_OFF_STIRRING_MOTOR_5_TRIG();
        TURN_OFF_STIRRING_MOTOR_5();
#else
        TURN_OFF_STIRRING_MOTOR_5();
#endif
    }
}


#endif

#endif


// Hal Stirring Moter Module *********************************************************************************

/// @brief      Hal Stirring Motor Initilaize Module
/// @details    ���� ���� ���� ���� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_StirringMoter_Module_Initialize(void)
{
#if (STIRRING_MOTOR_COUNT > 0)
    Hal_StirringMotor_Initialize();         // ���� ���� ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Stirring Motor Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� ���� ���� ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_Module_1ms_Control(void)
{
#if (STIRRING_MOTOR_COUNT > 0)
    StirringMotorControlTimer();            // ���ݸ��� ���� �ð� Counter
#endif
}


/// @brief      Hal Stirring Motor Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_StirringMotor_Module_Control(void)
{
#if (STIRRING_MOTOR_COUNT > 0)
    StirringMotorControl();                 // ���� ���� ���� �Լ�
#endif
}

