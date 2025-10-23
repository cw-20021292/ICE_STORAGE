/// @file     Lib_BLDCMotor.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    BLDC ���� ��� ���� ���̺귯�� ����

#include "Global_Header.h"

#if (BLDC_MOTOR_USE == USE)

// ���� ����
U8 gu8BLDCMotorControlTimer = 0;            /// @brief ���� ���� Ÿ�̸� (1ms ����)
U16 gu16BLDCRPMSignalTimer = 0;              /// @brief RPM ��ȣ �б� Ÿ�̸�
U16 gu16UserSetRPM = 0;                     /// @brief ����� ���� RPM
U8 gu8EventMotor = FAULT_NONE;              /// @brief ���� �̺�Ʈ/���� ����
U16 gu16checkRpmErrorCnt = ERROR_CHECK_CNT_RPM;    /// @brief RPM ���� üũ ī����
U8 gu8HallSensorCheckCnt = ERROR_CHECK_CNT_HALL;   /// @brief Ȧ���� ���� üũ ī����
U8 u8MotSetFlag = 0;                        /// @brief ���� ���� ���� �÷���
U16 gu16PWMValue = 0;                       /// @brief ���� PWM duty ��
U16 gu16TargetVal = 0;
U16 gu16AdjustDelay = 0;

// �⵿���� ���� ����
static U8 u8StartupTimer = 0;               /// @brief �⵿�ð� ī���� (100ms ����)
static U8 u8StartupMode = 0;                /// @brief �⵿��� �÷���

/// @brief    BLDC ���� ���̺귯�� �ʱ�ȭ �Լ�
/// @param    void
/// @return   void
void Lib_BLDCMotor_Initialize(void)
{
    // ���� ���� �ʱ�ȭ
    gu8BLDCMotorControlTimer = 0;
    gu16BLDCRPMSignalTimer = 0;
    gu16UserSetRPM = 0;
    gu8EventMotor = FAULT_NONE;
    gu16checkRpmErrorCnt = ERROR_CHECK_CNT_RPM;
    gu8HallSensorCheckCnt = ERROR_CHECK_CNT_HALL;
    u8MotSetFlag = 0;
    
    // �⵿���� ���� ���� �ʱ�ȭ
    u8StartupTimer = 0;
    u8StartupMode = 0;
    // gu16RPMValue = VALUE_PWM_DUTY_START;

    // �ϵ���� �ʱ�ȭ �Լ� ȣ��
    Hal_BLDC_Motor_Initialize();
}

/// @brief    BLDC ���� ���� Ÿ�̸� �Լ� (1ms ���ͷ�Ʈ)
/// @param    void
/// @return   void
void Lib_BLDCMotor_Module_1ms_Control(void)
{
    // 1ms���� Ÿ�̸� ī��Ʈ ����
    gu8BLDCMotorControlTimer++;
    gu16BLDCRPMSignalTimer++;

    // 100ms �ֱ�� ���� ���� �Լ� ����
    if (gu8BLDCMotorControlTimer >= BLDC_MOTOR_CONTROL_TIME_PERIOD)
    {
        Lib_BLDCMotor_Control();
        gu8BLDCMotorControlTimer = 0;
    }

    // 1000ms �ֱ�� Ȧ���� ��ȣ�� �б�
    if (gu16BLDCRPMSignalTimer >= RPM_SIGNAL_READ_INTERVAL)
    {
        gu16TargetVal = GetHallSensorSignalValue();
        gu16BLDCRPMSignalTimer = 0;
        // gu8BLDCRPMSignalTimer = 0;
    }
}

/// @brief    BLDC ���� ���� ���� �Լ�
/// @param    void
/// @return   void
void Lib_BLDCMotor_Control(void)
{
    AdjustMotorRPM(gu16UserSetRPM/5);
    // ���� üũ
    CheckMotorError();
    
}

/// @brief    ���� RPM ���� �Լ�
/// @param    mu16RPM ������ RPM ��
/// @return   void
void SetMotorRPM(U16 mu16RPM)
{
    if (gu16UserSetRPM != mu16RPM) 
    {
        u8MotSetFlag = 1;
        gu16UserSetRPM = mu16RPM;
    }
}

/// @brief    ���� ������ ���� RPM �� ��ȯ �Լ�
/// @param    void
/// @return   U16 ���� ������ RPM ��
U16 GetMotorRPM(void)
{
    return gu16UserSetRPM;
}

/// @brief    ���� ���� üũ �Լ�
/// @param    void
/// @return   void
void CheckMotorError(void)
{
    // RPM ���� üũ
    if (gu16checkRpmErrorCnt == 0)
    {
        if ((gu8EventMotor & FAULT_RPM_ERROR) != FAULT_RPM_ERROR)
        {
            gu8EventMotor |= FAULT_RPM_ERROR;
        }
    }
    else
    {
        gu16checkRpmErrorCnt--;
        if ((gu8EventMotor & FAULT_RPM_ERROR) == FAULT_RPM_ERROR)
        {
            gu8EventMotor &= ~FAULT_RPM_ERROR;
        }
    }

    // Ȧ���� ���� üũ
    if (gu8HallSensorCheckCnt == 0)
    {
        if ((gu8EventMotor & FAULT_HALL_SENSOR) != FAULT_HALL_SENSOR)
        {
            gu8EventMotor |= FAULT_HALL_SENSOR;
        }
    }
    else
    {
        gu8HallSensorCheckCnt--;
        if ((gu8EventMotor & FAULT_HALL_SENSOR) == FAULT_HALL_SENSOR)
        {
            gu8EventMotor &= ~FAULT_HALL_SENSOR;
        }
    }
}

/// @brief    ���� RPM �ڵ� ���� �Լ�
/// @param    mu16SetRPM ��ǥ RPM ��
/// @return   void
void AdjustMotorRPM(U16 mu16SetRPM)
{
    I16 mi16Gap = 0;
    U16 mu16Gap = 0;

    // �⵿���� ���� ó��
    if (u8StartupMode)
    {
            if (u8StartupTimer > 0)
    {
        u8StartupTimer--;
        Set_BLDCMotor_PWM(VALUE_PWM_DUTY_START);  // �⵿��� �� PWM ���� ���
        return;
    }
    else
    {
        u8StartupMode = 0;  // �⵿��� ����
    }
    }

    // ���ο� RPM ���� �� �⵿���� ���� ���� ����
    if (u8MotSetFlag == 1)
    {
        u8MotSetFlag = 0;
        if (gu16PWMValue < VALUE_PWM_DUTY_START)
        {
            // �⵿��� ����
            u8StartupMode = 1;
            u8StartupTimer = STARTUP_VOLTAGE_TIME;
            Set_BLDCMotor_PWM(VALUE_PWM_DUTY_START);
            return;
        }
    }

    mi16Gap = gu16TargetVal - mu16SetRPM;
    mu16Gap = abs(mi16Gap);

    if (mu16Gap <= 20)
    {
        gu16checkRpmErrorCnt = ERROR_CHECK_CNT_RPM;
    }

    if (gu16AdjustDelay == 0)
    {
        if (mi16Gap < 0)  // RPM ���� �ʿ�
        {
            if (mu16Gap > 30)
            {
                gu16PWMValue += 200;
                gu16AdjustDelay = 1;
            }
            else if (mu16Gap > 10)
            {
                gu16PWMValue += 100;
                gu16AdjustDelay = 3;
            }
            else if (mu16Gap > 5)
            {
                gu16PWMValue += 50;
                gu16AdjustDelay = 5;
            }
            else if (mu16Gap > 2)
            {
                gu16PWMValue += 10;
                gu16AdjustDelay = 10;
            }
        }
        else  // RPM ���� �ʿ�
        {
            if (mu16Gap > 30)
            {
                gu16PWMValue -= 200;
                gu16AdjustDelay = 1;
            }
            else if (mu16Gap > 10)
            {
                gu16PWMValue -= 100;
                gu16AdjustDelay = 3;
            }
            else if (mu16Gap > 5)
            {
                gu16PWMValue -= 50;
                gu16AdjustDelay = 5;
            }
            else if (mu16Gap > 2)
            {
                gu16PWMValue -= 10;
                gu16AdjustDelay = 10;
            }
        }
        Set_BLDCMotor_PWM(gu16PWMValue);
    }
    else
    {
        gu16AdjustDelay--;
    }
}

#endif  // BLDC_MOTOR_USE == USE

