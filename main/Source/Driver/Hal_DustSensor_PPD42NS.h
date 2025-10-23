#ifndef _HAL_DUSTSENSOR_PPD42NS_H_
#define _HAL_DUSTSENSOR_PPD42NS_H_

/// @file     Hal_DustSensor_PPD42NS.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD42NS 먼지센서 드라이버 헤더 파일
/// @details  PPD42NS 먼지센서 제어를 위한 HAL 드라이버 헤더

// Include standard header for type definitions
#define DUST_SENSOR_PPD42NS_USE USE

/*  PPD42NS 먼지센서 제어 Function 사용법
    => Hal_DustSensor_PPD42NS_Init();                        // PPD42NS 센서 초기화
    => Hal_DustSensor_PPD42NS_On();                          // PPD42NS 센서 전원 ON
    => Hal_DustSensor_PPD42NS_Off();                         // PPD42NS 센서 전원 OFF
    => Hal_DustSensor_PPD42NS_Handler();                     // PPD42NS 센서 데이터 처리 (주기적 호출)
    => Hal_DustSensor_PPD42NS_ReadData(&pm1_0, &pm2_5, &pm10, &error); // 먼지 데이터 읽기
    => Hal_DustSensor_PPD42NS_GetPM2_5();                    // PM2.5 값 읽기
    => Hal_DustSensor_PPD42NS_GetVolumeSum();                // 볼륨합계 값 읽기
    => getParticleVolumeSum();                               // 입자 볼륨합계 읽기

    사용 예)
    Hal_DustSensor_PPD42NS_Module_Initialize();              // 시스템 초기화 시 호출
    Hal_DustSensor_PPD42NS_On();                             // 센서 전원 ON
    
    while(1) {
        Hal_DustSensor_PPD42NS_Module_Control();             // 메인 루프에서 호출
        
        // 먼지 농도 읽기
        U16 pm2_5 = Hal_DustSensor_PPD42NS_GetPM2_5();
        U16 volume_sum = Hal_DustSensor_PPD42NS_GetVolumeSum();
        U8 error = Hal_DustSensor_PPD42NS_GetErrorFlag();
        
        // 유틸리티 함수 사용
        U16 particle_volume = getParticleVolumeSum();
    }

    주의사항)
    - DUST_SENSOR_PPD42NS_USE가 USE로 설정되어야 함
    - 센서 전원 ON 후 안정화 시간(1초) 필요
    - 아날로그 입력 사용 (P9_bit.no4)
    - 볼륨합계 계산 주기: 2초
    - 에러 체크 시간: 5초
    - 입자 카운트 시간: 5ms
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

// PPD42NS 전용 포트 정의
#define PORT_DEFINE_PARTICLE_IN                         P9_bit.no4

// PPD42NS 전용 설정
#define PPD42NS_CONTROL_TIME_PERIOD    1000    // 1초 주기
#define PPD42NS_CAL_TIME_PERIOD        2000    // 2초 주기
#define PARTICLE_INITTIME            1000     // 초기화 시간 (기존 값으로 복원)
#define PARTICLE_ERRORTIME           5000     // 에러 시간 (기존 값으로 복원)
#define PPD42NS_PARTICLE_COUNT_TIME          5
#define PPD42NS_PARTICLE_VOLUMESUM_TIME      2000
#define PPD42NS_PARTICLE_ERROR_CHECK_TIME    5

///////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////

// 초기화 및 제어 함수
void Hal_DustSensor_PPD42NS_Init(void);
void Hal_DustSensor_PPD42NS_On(void);
void Hal_DustSensor_PPD42NS_Off(void);
void Hal_DustSensor_PPD42NS_Handler(void);

// 데이터 읽기 함수
void Hal_DustSensor_PPD42NS_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);

// 내부 센서 처리 함수는 static으로 정의되어 외부에서 접근 불가

// 타이머 제어 함수
void Hal_DustSensor_PPD42NS_ControlTimer(void);
void Hal_DustSensor_PPD42NS_Module_Control(void);

// 데이터 getter 함수
U16 Hal_DustSensor_PPD42NS_GetVolumeSum(void);
U16 Hal_DustSensor_PPD42NS_GetPM2_5(void);
U8 Hal_DustSensor_PPD42NS_GetErrorFlag(void);

// 유틸리티 함수
U16 getParticleVolumeSum(void);

// Module Interface Functions
void Hal_DustSensor_PPD42NS_Module_Initialize(void);
void Hal_DustSensor_PPD42NS_Module_1ms_Control(void);

#endif // _HAL_DUSTSENSOR_PPD42NS_H_
