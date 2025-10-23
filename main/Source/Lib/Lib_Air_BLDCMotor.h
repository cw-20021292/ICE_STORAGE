/// @file     Lib_BLDCMotor.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    BLDC ���� ��� ���� ���̺귯�� ��� ����

#ifndef _LIB_BLDC_MOTOR_H_
#define _LIB_BLDC_MOTOR_H_


/*  BLDC Motor ���� Function ����
    => Lib_BLDCMotor_Initialize();                           // BLDC ���� ���̺귯�� �ʱ�ȭ
    => Lib_BLDCMotor_Module_1ms_Control();                   // 1ms Ÿ�̸� ���� (���ͷ�Ʈ���� ȣ��)
    => Lib_BLDCMotor_Control();                              // ���� ���� ���� (100ms �ֱ� �ڵ� ȣ��)
    => SetMotorRPM(RPM��);                                   // ��ǥ RPM ���� (300~1600)
    => GetMotorRPM();                                        // ���� ������ RPM �б�
    => AdjustMotorRPM(RPM��);                                // ���� RPM �ڵ� ���� (���� ���)
    => CheckMotorError();                                    // ���� ���� üũ (���� ���)

    ��� ��)
    Lib_BLDCMotor_Initialize();                              // �ý��� �ʱ�ȭ �� ȣ��
    
    // Basic_Loop�� 1ms Ÿ�̸ӿ��� ȣ�� (Base_Timer_1ms(void))
        Lib_BLDCMotor_Module_1ms_Control();                  // 1ms���� ȣ�� (�ڵ����� 100ms �ֱ�� Control �Լ� ����)
    
    while(1) {
        // 100ms �ֱ�� Lib_BLDCMotor_Control()�� �ڵ� ȣ���
        
        // RPM ����
        SetMotorRPM(RPM_ONE);                                // 1�� RPM(580) ����
        SetMotorRPM(RPM_THREE);                              // 3�� RPM(1200)���� ����
        
        // ���� ���� RPM Ȯ��
        U16 current_rpm = GetMotorRPM();                     // ���� ������ RPM �б�
        
        // �̸� ���ǵ� RPM ���� ���
        SetMotorRPM(RPM_SILENT);    // �������(320)
        SetMotorRPM(RPM_TURBO);     // �ͺ����(1600)
        SetMotorRPM(0);             // ���� ����
    }

    ���ǻ���)
    - BLDC_MOTOR_USE�� USE�� �����Ǿ�� ��
    - 1ms Ÿ�̸� ���ͷ�Ʈ���� Lib_BLDCMotor_Module_1ms_Control() ȣ�� �ʼ�
    - 100ms �ֱ�� �ڵ����� ���� ���� �� ���� üũ ����
    - �⵿���� �ڵ� ����: 3�ʰ� VALUE_PWM_DUTY_START ����
    - �ϵ���� �������̽�: Hal_BLDC_Motor.c�� �Լ��� ���
*/


// ���� ��� ����
#define BLDC_MOTOR_USE                  USE    // BLDC Motor ��� ���� - 0(No Use), 1(Use)

// ���� ���� ���� ���
#define ERROR_CHECK_CNT_RPM          6000    /// @brief RPM ���� üũ ī��Ʈ (60��)
#define ERROR_CHECK_CNT_HALL         60      /// @brief Ȧ���� ���� üũ ī��Ʈ (6��)
#define VALUE_MOTOR_STABILIZATION    5       /// @brief ���� ����ȭ �ð� (500ms)
#define BLDC_MOTOR_CONTROL_TIME_PERIOD 100   /// @brief ���� ���� �ֱ� (100ms)
#define RPM_SIGNAL_READ_INTERVAL     1000    /// @brief RPM ��ȣ �б� �ֱ� (1000ms)

// RPM ���� ���� ���
#define VALUE_RPM_MAX               1600    /// @brief �ִ� RPM
#define VALUE_RPM_MIN               300     /// @brief �ּ� RPM
#define VALUE_RPM_OFFSET_VALUE      0       /// @brief RPM ������
#define VALUE_MOTOR_ON_DELAY        5       /// @brief ���� ���� ���� �ð� (5 * 100msec)
#define VALUE_MOTOR_OFF_DELAY       5       /// @brief ���� ���� ���� �ð� (5 * 100msec)

// #define MOTOR_ON 1
// #define MOTOR_OFF 0

// RPM ���� ���� (���� RPM ��)
#define RPM_SILENT      320     /// @brief ���� ��� RPM
#define RPM_ONE         580     /// @brief 1�� RPM  
#define RPM_TWO         680     /// @brief 2�� RPM
#define RPM_THREE       1200    /// @brief 3�� RPM
#define RPM_FOUR        1580    /// @brief 4�� RPM
#define RPM_TURBO       1600    /// @brief �ͺ� ��� RPM

// �⵿���� ���� ���
#define VALUE_PWM_DUTY_START      11000   /// @brief �⵿���� PWM ��
#define STARTUP_VOLTAGE_TIME      30      /// @brief �⵿���� ���� �ð� (100ms * 30 = 3��)

// ���� ���� ����
typedef enum {
    MotorWait    = 0,    /// @brief ��� ���� - ���� ���� �� �ʱ�ȭ ����
    MotorStart,          /// @brief ���� ���� - ���� ���� ���� �ܰ�
    MotorRunning,        /// @brief ���� �� ���� - ���� ���� ���� ����
    MotorStop            /// @brief ���� ���� - ���� ���� �ܰ�
} EMotorStatus;

// ���� ���� ���� ����
typedef enum {
    FAULT_NONE           = 0x00,   /// @brief ���� ���� - ���� ����
    FAULT_HALL_SENSOR    = 0x01,   /// @brief Ȧ���� ���� - ���� ��ȣ ������
    FAULT_RPM_ERROR      = 0x02    /// @brief RPM ���� - ��ǥ RPM�� ���� RPM ���̰� ū ���
} EMotorEvent;

/// @brief    ���� ���� ���� ����
/// @details  ������ ���� On/Off ���¸� ��Ÿ���� �������Դϴ�.
typedef enum {
    MOTOR_ON     = 1,    /// @brief ���� ���� On - ���� ���� ����
    MOTOR_OFF    = 0     /// @brief ���� ���� Off - ���� ���� ����
} EMotorOnOff;


// ���̺귯�� ���� �Լ� (���� ������ �Լ��鸸)
void Lib_BLDCMotor_Initialize(void);
void Lib_BLDCMotor_Control(void);
void Lib_BLDCMotor_Module_1ms_Control(void);

// ���� RPM ���� �Լ�
void SetMotorRPM(U16 mu16RPM);
U16 GetMotorRPM(void);

// ���� ���� �Լ� (���̺귯�� ���ο��� �ڵ� ȣ��)
void CheckMotorError(void);
void AdjustMotorRPM(U16 mu16SetRPM);

// �ϵ���� �������̽� �Լ� (Hal_BLDC_Motor.c�� ����)
void Set_BLDCMotor_PWM(U16 mu16Duty);    // PWM duty ���� ����
void Set_BLDCMotor_Power(U8 mu8Act);    // ���� ����
void SetPwmOutLow(void);              // PWM ��� Low ����
U16 GetHallSensorSignalValue(void);    // Ȧ���� ��ȣ�� �б�
void Hal_BLDC_Motor_Initialize(void);   // �ϵ���� �ʱ�ȭ

#endif // _LIB_BLDC_MOTOR_H_

