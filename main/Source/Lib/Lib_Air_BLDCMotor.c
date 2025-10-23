/// @file     Lib_BLDCMotor.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    BLDC 모터 제어를 위한 라이브러리 파일

#include "Global_Header.h"

#if (BLDC_MOTOR_USE == USE)

// 전역 변수
U8 gu8BLDCMotorControlTimer = 0;            /// @brief 모터 제어 타이머 (1ms 단위)
U16 gu16BLDCRPMSignalTimer = 0;              /// @brief RPM 신호 읽기 타이머
U16 gu16UserSetRPM = 0;                     /// @brief 사용자 설정 RPM
U8 gu8EventMotor = FAULT_NONE;              /// @brief 모터 이벤트/에러 상태
U16 gu16checkRpmErrorCnt = ERROR_CHECK_CNT_RPM;    /// @brief RPM 에러 체크 카운터
U8 gu8HallSensorCheckCnt = ERROR_CHECK_CNT_HALL;   /// @brief 홀센서 에러 체크 카운터
U8 u8MotSetFlag = 0;                        /// @brief 모터 설정 변경 플래그
U16 gu16PWMValue = 0;                       /// @brief 현재 PWM duty 값
U16 gu16TargetVal = 0;
U16 gu16AdjustDelay = 0;

// 기동전압 관련 변수
static U8 u8StartupTimer = 0;               /// @brief 기동시간 카운터 (100ms 단위)
static U8 u8StartupMode = 0;                /// @brief 기동모드 플래그

/// @brief    BLDC 모터 라이브러리 초기화 함수
/// @param    void
/// @return   void
void Lib_BLDCMotor_Initialize(void)
{
    // 전역 변수 초기화
    gu8BLDCMotorControlTimer = 0;
    gu16BLDCRPMSignalTimer = 0;
    gu16UserSetRPM = 0;
    gu8EventMotor = FAULT_NONE;
    gu16checkRpmErrorCnt = ERROR_CHECK_CNT_RPM;
    gu8HallSensorCheckCnt = ERROR_CHECK_CNT_HALL;
    u8MotSetFlag = 0;
    
    // 기동전압 관련 변수 초기화
    u8StartupTimer = 0;
    u8StartupMode = 0;
    // gu16RPMValue = VALUE_PWM_DUTY_START;

    // 하드웨어 초기화 함수 호출
    Hal_BLDC_Motor_Initialize();
}

/// @brief    BLDC 모터 제어 타이머 함수 (1ms 인터럽트)
/// @param    void
/// @return   void
void Lib_BLDCMotor_Module_1ms_Control(void)
{
    // 1ms마다 타이머 카운트 증가
    gu8BLDCMotorControlTimer++;
    gu16BLDCRPMSignalTimer++;

    // 100ms 주기로 모터 제어 함수 실행
    if (gu8BLDCMotorControlTimer >= BLDC_MOTOR_CONTROL_TIME_PERIOD)
    {
        Lib_BLDCMotor_Control();
        gu8BLDCMotorControlTimer = 0;
    }

    // 1000ms 주기로 홀센서 신호값 읽기
    if (gu16BLDCRPMSignalTimer >= RPM_SIGNAL_READ_INTERVAL)
    {
        gu16TargetVal = GetHallSensorSignalValue();
        gu16BLDCRPMSignalTimer = 0;
        // gu8BLDCRPMSignalTimer = 0;
    }
}

/// @brief    BLDC 모터 제어 메인 함수
/// @param    void
/// @return   void
void Lib_BLDCMotor_Control(void)
{
    AdjustMotorRPM(gu16UserSetRPM/5);
    // 에러 체크
    CheckMotorError();
    
}

/// @brief    모터 RPM 설정 함수
/// @param    mu16RPM 설정할 RPM 값
/// @return   void
void SetMotorRPM(U16 mu16RPM)
{
    if (gu16UserSetRPM != mu16RPM) 
    {
        u8MotSetFlag = 1;
        gu16UserSetRPM = mu16RPM;
    }
}

/// @brief    현재 설정된 모터 RPM 값 반환 함수
/// @param    void
/// @return   U16 현재 설정된 RPM 값
U16 GetMotorRPM(void)
{
    return gu16UserSetRPM;
}

/// @brief    모터 에러 체크 함수
/// @param    void
/// @return   void
void CheckMotorError(void)
{
    // RPM 에러 체크
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

    // 홀센서 에러 체크
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

/// @brief    모터 RPM 자동 조정 함수
/// @param    mu16SetRPM 목표 RPM 값
/// @return   void
void AdjustMotorRPM(U16 mu16SetRPM)
{
    I16 mi16Gap = 0;
    U16 mu16Gap = 0;

    // 기동전압 로직 처리
    if (u8StartupMode)
    {
            if (u8StartupTimer > 0)
    {
        u8StartupTimer--;
        Set_BLDCMotor_PWM(VALUE_PWM_DUTY_START);  // 기동모드 중 PWM 지속 출력
        return;
    }
    else
    {
        u8StartupMode = 0;  // 기동모드 종료
    }
    }

    // 새로운 RPM 설정 시 기동전압 적용 여부 결정
    if (u8MotSetFlag == 1)
    {
        u8MotSetFlag = 0;
        if (gu16PWMValue < VALUE_PWM_DUTY_START)
        {
            // 기동모드 시작
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
        if (mi16Gap < 0)  // RPM 증가 필요
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
        else  // RPM 감소 필요
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

