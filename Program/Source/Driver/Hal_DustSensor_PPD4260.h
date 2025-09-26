#ifndef HAL_DUSTSENSOR_PPD4260_H
#define HAL_DUSTSENSOR_PPD4260_H

/// @file     Hal_DustSensor_PPD4260.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD4260 먼지센서 드라이버 헤더 파일
/// @details  PPD4260 먼지센서 제어를 위한 HAL 드라이버 헤더

// Include standard header for type definitions
#define DUST_SENSOR_PPD4260_USE USE

/*  PPD4260 먼지센서 제어 Function 사용법
    => Hal_DustSensor_PPD4260_Init();                        // PPD4260 센서 초기화
    => Hal_DustSensor_PPD4260_On();                          // PPD4260 센서 전원 ON
    => Hal_DustSensor_PPD4260_Off();                         // PPD4260 센서 전원 OFF
    => Hal_DustSensor_PPD4260_Handler();                     // PPD4260 센서 데이터 처리 (주기적 호출)
    => Hal_DustSensor_PPD4260_ReadData(&pm1_0, &pm2_5, &pm10, &error); // 먼지 데이터 읽기
    => Hal_DustSensor_PPD4260_GetPM2_5();                    // PM2.5 값 읽기
    => Hal_DustSensor_PPD4260_GetPM10();                     // PM10 값 읽기
    => Hal_DustSensor_PPD4260_GetPM2_5Avg();                 // PM2.5 평균값 읽기
    => Hal_DustSensor_PPD4260_GetPM10Avg();                  // PM10 평균값 읽기

    사용 예)
    Hal_DustSensor_PPD4260_Module_Initialize();              // 시스템 초기화 시 호출
    Hal_DustSensor_PPD4260_On();                             // 센서 전원 ON
    
    while(1) {
        Hal_DustSensor_PPD4260_Module_Control();             // 메인 루프에서 호출
        
        // 먼지 농도 읽기 (즉시값)
        U16 pm2_5 = Hal_DustSensor_PPD4260_GetPM2_5();
        U16 pm10 = Hal_DustSensor_PPD4260_GetPM10();
        
        // 먼지 농도 읽기 (평균값)
        D64 pm2_5_avg = Hal_DustSensor_PPD4260_GetPM2_5Avg();
        D64 pm10_avg = Hal_DustSensor_PPD4260_GetPM10Avg();
        
        U8 error = Hal_DustSensor_PPD4260_GetErrorFlag();
    }

    주의사항)
    - DUST_SENSOR_PPD4260_USE가 USE로 설정되어야 함
    - 센서 전원 ON 후 안정화 시간(1초) 필요
    - 아날로그 센서이므로 ADC 설정이 완료되어야 함
    - 농도 계산 및 평균값 처리 기능 내장
    - 에러 체크 시간: 5초
*/

///////////////////////////////////////////////////////////////////////
/// Macro Definitions
//////////////////////////////////////////////////////////////////////

// 공통 포트 및 값 정의
#define PORT_DEFINE_PARTICLE_SENSOR_POW                 P5_bit.no0  // 센서 전원 제어 Port 설정 : P50
#define VALUE_PARTICLE_POWER_ON                         N_ON
#define VALUE_PARTICLE_POWER_OFF                        N_OFF
#define VALUE_PARTICLE_INITTIME                         10000
#define VALUE_PARTICLE_ERRORTIME                        10000

// PPD4260 전용 설정
#define PPD4260_CONTROL_TIME_PERIOD    1000    // 1초 주기
#define PARTICLE_INITTIME            1000     // 초기화 시간
#define PARTICLE_ERRORTIME           5000     // 에러 시간
#define PPD4260_PARTICLE_COUNT_TIME          1
#define PPD4260_PARTICLE_CONCENT_TIME        1000
#define PPD4260_PARTICLE_ERROR_CHECK_TIME    1

///////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////

// 초기화 및 제어 함수
void Hal_DustSensor_PPD4260_Init(void);
void Hal_DustSensor_PPD4260_On(void);
void Hal_DustSensor_PPD4260_Off(void);
void Hal_DustSensor_PPD4260_Handler(void);

// 데이터 읽기 함수
void Hal_DustSensor_PPD4260_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);

// 내부 센서 처리 함수는 static으로 정의되어 외부에서 접근 불가

// 타이머 제어 함수
void Hal_DustSensor_PPD4260_ControlTimer(void);
void Hal_DustSensor_PPD4260_Module_Control(void);

// 데이터 getter 함수
U16 Hal_DustSensor_PPD4260_GetPM2_5(void);
U16 Hal_DustSensor_PPD4260_GetPM10(void);
D64 Hal_DustSensor_PPD4260_GetPM2_5Avg(void);
D64 Hal_DustSensor_PPD4260_GetPM10Avg(void);
U8 Hal_DustSensor_PPD4260_GetErrorFlag(void);

// Module Interface Functions
void Hal_DustSensor_PPD4260_Module_Initialize(void);
void Hal_DustSensor_PPD4260_Module_1ms_Control(void);

#endif // HAL_DUSTSENSOR_PPD4260_H
