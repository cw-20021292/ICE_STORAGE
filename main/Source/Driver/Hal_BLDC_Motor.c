/// @file     Hal_BLDC_Motor.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    BLDC ���� �ϵ���� ��� ���� ����̹� ����
/// @details  BLDC ������ �ʱ�ȭ, PWM ����, RPM ����, ���� üũ ���� ����� �����մϴ�.

#include "Global_Header.h"

#if (BLDC_MOTOR_USE == USE)

/// @brief    Legacy motor control code (���յ�)
/// @details  ���� Motor.c �ڵ尡 ���յǾ� ���Ե�

// ���� üũ ���� ���
#define ERROR_CHECK_CNT_HALL         60      /// @brief Ȧ���� ���� üũ ī��Ʈ (6��)

// RPM ���� ���� ����
U16 gu16CounterFeedbackBLDC01 = 0;  /// @brief Ȧ���� ī��Ʈ ��
U16 gu16CounterFeedbackValue = 0;  /// @brief Ȧ���� ī��Ʈ ��

/// @brief    ���� ���� ���� �ð� ����
typedef enum{
	ADJ_DELAY_00	= 0,    /// @brief ���� �ð� 0
	ADJ_DELAY_01	= 1,    /// @brief ���� �ð� 1
	ADJ_DELAY_02,
	ADJ_DELAY_03,
	ADJ_DELAY_04,
	ADJ_DELAY_05,
	ADJ_DELAY_06,
	ADJ_DELAY_07,
	ADJ_DELAY_08,
	ADJ_DELAY_09,
	ADJ_DELAY_10,
	ADJ_DELAY_11,
	ADJ_DELAY_12,
	ADJ_DELAY_13,
	ADJ_DELAY_14,
	ADJ_DELAY_15,
	ADJ_DELAY_16,
	ADJ_DELAY_17,
	ADJ_DELAY_18,
	ADJ_DELAY_19,
	ADJ_DELAY_20,
}EAdjustDelay;

#define RPM_SIGNAL_READ_INTERVAL    1000    // 1��(1000ms) �������� RPM ��ȣ �б�


/// @brief    BLDC ���� �ϵ���� �ʱ�ȭ �Լ�
/// @param    void
/// @return   void
void Hal_BLDC_Motor_Initialize(void)
{
    // PWM ��� �ʱ�ȭ
    SetPwmOutLow();
    Set_BLDCMotor_Power(MOTOR_OFF);
}

/// @brief    PWM ����� Low�� �����ϴ� �Լ�
/// @param    void
/// @return   void
void SetPwmOutLow(void)
{
    TOL1 &= ~_0002_TAU_CH1_OUTPUT_LEVEL_L;
    TO1 &= ~_0002_TAU_CH1_OUTPUT_VALUE_1;
}

/// @brief    ���� PWM duty �� ���� �Լ�
/// @param    mu16Duty PWM duty ��
/// @return   void
void Set_BLDCMotor_PWM(U16 mu16Duty)
{
    // ���� üũ �߰�
    if (mu16Duty > VALUE_PWM_DUTY_MAX) {
        mu16Duty = VALUE_PWM_DUTY_MAX;
    }
#if (VALUE_PWM_DUTY_MIN > 0)
    else if (mu16Duty < VALUE_PWM_DUTY_MIN) {
        mu16Duty = VALUE_PWM_DUTY_MIN;
    }
#endif

    PWM_DUTY_REGISTER = mu16Duty;
}

/// @brief    ���� ���� ���� �Լ�
/// @param    mu8Act ���� ���� ���� ��� (MOTOR_ON/MOTOR_OFF)
/// @return   void
void Set_BLDCMotor_Power(U8 mu8Act)
{
    P_MOTOR_POWER = mu8Act;
}

/// @brief    Ȧ���� ��ȣ ó�� �Լ� (���ͷ�Ʈ��)
/// @param    void
/// @return   void
void Hal_BLDCMotor_IncrementRPMSignal(void)
{
    gu16CounterFeedbackBLDC01++;                   // Ȧ���� ��ȣ ī��Ʈ ����
}

/// @brief    ���� �ϵ���� ��� 1ms ���ͷ�Ʈ ó�� �Լ�
/// @param    void
/// @return   void
void Hal_BLDC_Motor_Module_1ms_Control(void)
{
    static U16 gu16Timer1Sec = 0;

    if (++gu16Timer1Sec >= 1000)  // 1�ʸ���
    {
        gu16Timer1Sec = 0;
        gu16CounterFeedbackValue = gu16CounterFeedbackBLDC01;  // Ȧ���� ī��Ʈ �� ����
        gu16CounterFeedbackBLDC01 = 0;  // Ȧ���� ī��Ʈ �� �ʱ�ȭ
    }
}
/// @brief    Ȧ���� ��ȣ ��(RPM) ��ȯ �Լ�
/// @param    void
/// @return   U16 1�� ���� ���ŵ� Ȧ���� ī��Ʈ ��
U16 GetHallSensorSignalValue(void)
{
    return gu16CounterFeedbackValue;
}

#endif  // BLDC_MOTOR_USE == USE



