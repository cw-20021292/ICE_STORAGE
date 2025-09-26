/// @file     Lib_BLDCMotor.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    BLDC 모터 제어를 위한 라이브러리 헤더 파일

#ifndef _LIB_BLDC_MOTOR_H_
#define _LIB_BLDC_MOTOR_H_


/*  BLDC Motor 제어 Function 사용법
    => Lib_BLDCMotor_Initialize();                           // BLDC 모터 라이브러리 초기화
    => Lib_BLDCMotor_Module_1ms_Control();                   // 1ms 타이머 제어 (인터럽트에서 호출)
    => Lib_BLDCMotor_Control();                              // 모터 메인 제어 (100ms 주기 자동 호출)
    => SetMotorRPM(RPM값);                                   // 목표 RPM 설정 (300~1600)
    => GetMotorRPM();                                        // 현재 설정된 RPM 읽기
    => AdjustMotorRPM(RPM값);                                // 모터 RPM 자동 조정 (내부 사용)
    => CheckMotorError();                                    // 모터 에러 체크 (내부 사용)

    사용 예)
    Lib_BLDCMotor_Initialize();                              // 시스템 초기화 시 호출
    
    // Basic_Loop의 1ms 타이머에서 호출 (Base_Timer_1ms(void))
        Lib_BLDCMotor_Module_1ms_Control();                  // 1ms마다 호출 (자동으로 100ms 주기로 Control 함수 실행)
    
    while(1) {
        // 100ms 주기로 Lib_BLDCMotor_Control()이 자동 호출됨
        
        // RPM 설정
        SetMotorRPM(RPM_ONE);                                // 1단 RPM(580) 설정
        SetMotorRPM(RPM_THREE);                              // 3단 RPM(1200)으로 변경
        
        // 현재 설정 RPM 확인
        U16 current_rpm = GetMotorRPM();                     // 현재 설정된 RPM 읽기
        
        // 미리 정의된 RPM 레벨 사용
        SetMotorRPM(RPM_SILENT);    // 무음모드(320)
        SetMotorRPM(RPM_TURBO);     // 터보모드(1600)
        SetMotorRPM(0);             // 모터 정지
    }

    주의사항)
    - BLDC_MOTOR_USE가 USE로 설정되어야 함
    - 1ms 타이머 인터럽트에서 Lib_BLDCMotor_Module_1ms_Control() 호출 필수
    - 100ms 주기로 자동으로 모터 제어 및 에러 체크 수행
    - 기동전압 자동 적용: 3초간 VALUE_PWM_DUTY_START 적용
    - 하드웨어 인터페이스: Hal_BLDC_Motor.c의 함수들 사용
*/


// 모터 사용 설정
#define BLDC_MOTOR_USE                  USE    // BLDC Motor 사용 여부 - 0(No Use), 1(Use)

// 모터 제어 관련 상수
#define ERROR_CHECK_CNT_RPM          6000    /// @brief RPM 에러 체크 카운트 (60초)
#define ERROR_CHECK_CNT_HALL         60      /// @brief 홀센서 에러 체크 카운트 (6초)
#define VALUE_MOTOR_STABILIZATION    5       /// @brief 모터 안정화 시간 (500ms)
#define BLDC_MOTOR_CONTROL_TIME_PERIOD 100   /// @brief 모터 제어 주기 (100ms)
#define RPM_SIGNAL_READ_INTERVAL     1000    /// @brief RPM 신호 읽기 주기 (1000ms)

// RPM 제어 관련 상수
#define VALUE_RPM_MAX               1600    /// @brief 최대 RPM
#define VALUE_RPM_MIN               300     /// @brief 최소 RPM
#define VALUE_RPM_OFFSET_VALUE      0       /// @brief RPM 오프셋
#define VALUE_MOTOR_ON_DELAY        5       /// @brief 모터 켜짐 지연 시간 (5 * 100msec)
#define VALUE_MOTOR_OFF_DELAY       5       /// @brief 모터 꺼짐 지연 시간 (5 * 100msec)

// #define MOTOR_ON 1
// #define MOTOR_OFF 0

// RPM 레벨 정의 (실제 RPM 값)
#define RPM_SILENT      320     /// @brief 무음 모드 RPM
#define RPM_ONE         580     /// @brief 1단 RPM  
#define RPM_TWO         680     /// @brief 2단 RPM
#define RPM_THREE       1200    /// @brief 3단 RPM
#define RPM_FOUR        1580    /// @brief 4단 RPM
#define RPM_TURBO       1600    /// @brief 터보 모드 RPM

// 기동전압 관련 상수
#define VALUE_PWM_DUTY_START      11000   /// @brief 기동전압 PWM 값
#define STARTUP_VOLTAGE_TIME      30      /// @brief 기동전압 적용 시간 (100ms * 30 = 3초)

// 모터 상태 정의
typedef enum {
    MotorWait    = 0,    /// @brief 대기 상태 - 모터 정지 및 초기화 상태
    MotorStart,          /// @brief 시작 상태 - 모터 구동 시작 단계
    MotorRunning,        /// @brief 동작 중 상태 - 정상 구동 중인 상태
    MotorStop            /// @brief 정지 상태 - 모터 정지 단계
} EMotorStatus;

// 모터 에러 상태 정의
typedef enum {
    FAULT_NONE           = 0x00,   /// @brief 정상 상태 - 에러 없음
    FAULT_HALL_SENSOR    = 0x01,   /// @brief 홀센서 에러 - 센서 신호 비정상
    FAULT_RPM_ERROR      = 0x02    /// @brief RPM 에러 - 목표 RPM과 실제 RPM 차이가 큰 경우
} EMotorEvent;

/// @brief    모터 전원 상태 정의
/// @details  모터의 전원 On/Off 상태를 나타내는 열거형입니다.
typedef enum {
    MOTOR_ON     = 1,    /// @brief 모터 전원 On - 구동 가능 상태
    MOTOR_OFF    = 0     /// @brief 모터 전원 Off - 전원 차단 상태
} EMotorOnOff;


// 라이브러리 제어 함수 (실제 구현된 함수들만)
void Lib_BLDCMotor_Initialize(void);
void Lib_BLDCMotor_Control(void);
void Lib_BLDCMotor_Module_1ms_Control(void);

// 모터 RPM 제어 함수
void SetMotorRPM(U16 mu16RPM);
U16 GetMotorRPM(void);

// 내부 제어 함수 (라이브러리 내부에서 자동 호출)
void CheckMotorError(void);
void AdjustMotorRPM(U16 mu16SetRPM);

// 하드웨어 인터페이스 함수 (Hal_BLDC_Motor.c에 구현)
void Set_BLDCMotor_PWM(U16 mu16Duty);    // PWM duty 직접 제어
void Set_BLDCMotor_Power(U8 mu8Act);    // 전원 제어
void SetPwmOutLow(void);              // PWM 출력 Low 설정
U16 GetHallSensorSignalValue(void);    // 홀센서 신호값 읽기
void Hal_BLDC_Motor_Initialize(void);   // 하드웨어 초기화

#endif // _LIB_BLDC_MOTOR_H_

