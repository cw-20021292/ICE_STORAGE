/// @file     Hal_BLDC_Motor.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    BLDC 모터 하드웨어 제어를 위한 드라이버 파일
/// @details  BLDC 모터의 초기화, PWM 제어, RPM 제어, 에러 체크 등의 기능을 제공합니다.

#include "Global_Header.h"

#if (BLDC_MOTOR_USE == USE)

/// @brief    Legacy motor control code (통합됨)
/// @details  기존 Motor.c 코드가 통합되어 포함됨

// 에러 체크 관련 상수
#define ERROR_CHECK_CNT_HALL         60      /// @brief 홀센서 에러 체크 카운트 (6초)

// RPM 제어 관련 변수
U16 gu16CounterFeedbackBLDC01 = 0;  /// @brief 홀센서 카운트 값
U16 gu16CounterFeedbackValue = 0;  /// @brief 홀센서 카운트 값

/// @brief    모터 조정 지연 시간 정의
typedef enum{
	ADJ_DELAY_00	= 0,    /// @brief 지연 시간 0
	ADJ_DELAY_01	= 1,    /// @brief 지연 시간 1
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

#define RPM_SIGNAL_READ_INTERVAL    1000    // 1초(1000ms) 간격으로 RPM 신호 읽기


/// @brief    BLDC 모터 하드웨어 초기화 함수
/// @param    void
/// @return   void
void Hal_BLDC_Motor_Initialize(void)
{
    // PWM 출력 초기화
    SetPwmOutLow();
    Set_BLDCMotor_Power(MOTOR_OFF);
}

/// @brief    PWM 출력을 Low로 설정하는 함수
/// @param    void
/// @return   void
void SetPwmOutLow(void)
{
    TOL1 &= ~_0002_TAU_CH1_OUTPUT_LEVEL_L;
    TO1 &= ~_0002_TAU_CH1_OUTPUT_VALUE_1;
}

/// @brief    모터 PWM duty 값 변경 함수
/// @param    mu16Duty PWM duty 값
/// @return   void
void Set_BLDCMotor_PWM(U16 mu16Duty)
{
    // 범위 체크 추가
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

/// @brief    모터 전원 제어 함수
/// @param    mu8Act 모터 전원 제어 명령 (MOTOR_ON/MOTOR_OFF)
/// @return   void
void Set_BLDCMotor_Power(U8 mu8Act)
{
    P_MOTOR_POWER = mu8Act;
}

/// @brief    홀센서 신호 처리 함수 (인터럽트용)
/// @param    void
/// @return   void
void Hal_BLDCMotor_IncrementRPMSignal(void)
{
    gu16CounterFeedbackBLDC01++;                   // 홀센서 신호 카운트 증가
}

/// @brief    모터 하드웨어 모듈 1ms 인터럽트 처리 함수
/// @param    void
/// @return   void
void Hal_BLDC_Motor_Module_1ms_Control(void)
{
    static U16 gu16Timer1Sec = 0;

    if (++gu16Timer1Sec >= 1000)  // 1초마다
    {
        gu16Timer1Sec = 0;
        gu16CounterFeedbackValue = gu16CounterFeedbackBLDC01;  // 홀센서 카운트 값 저장
        gu16CounterFeedbackBLDC01 = 0;  // 홀센서 카운트 값 초기화
    }
}
/// @brief    홀센서 신호 값(RPM) 반환 함수
/// @param    void
/// @return   U16 1초 동안 수신된 홀센서 카운트 값
U16 GetHallSensorSignalValue(void)
{
    return gu16CounterFeedbackValue;
}

#endif  // BLDC_MOTOR_USE == USE



