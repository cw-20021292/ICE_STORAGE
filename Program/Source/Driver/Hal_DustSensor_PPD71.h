#ifndef HAL_DUSTSENSOR_PPD71_H
#define HAL_DUSTSENSOR_PPD71_H

/*'"FILE COMMENT"'***************************************************
@ FILE Name   : Hal_DustSensor_PPD71.h
@ FILE ID     : F-000
@ Programmer  : AUTO GENERATED
@ Description : PPD71 먼지센서 드라이버 헤더 파일
"FILE COMMENT END "'*************************************************/

// Include standard header for type definitions
#define DUST_SENSOR_PPD71_USE USE

/*  PPD71 먼지센서 제어 Function 사용법
    => Hal_DustSensor_PPD71_Init();                          // PPD71 센서 초기화
    => Hal_DustSensor_PPD71_On();                            // PPD71 센서 전원 ON
    => Hal_DustSensor_PPD71_Off();                           // PPD71 센서 전원 OFF
    => Hal_DustSensor_PPD71_Handler();                       // PPD71 센서 데이터 처리 (주기적 호출)
    => Hal_DustSensor_PPD71_ReadData(&pm1_0, &pm2_5, &pm10, &error); // 먼지 데이터 읽기
    => Hal_DustSensor_PPD71_SetSensitivity(민감도);          // 센서 민감도 설정
    => Hal_DustSensor_PPD71_GetPM2_5();                      // PM2.5 값 읽기
    => Hal_DustSensor_PPD71_GetPM10();                       // PM10 값 읽기

    사용 예)
    Hal_DustSensor_PPD71_Module_Initialize();                // 시스템 초기화 시 호출
    Hal_DustSensor_PPD71_On();                               // 센서 전원 ON
    
    // 민감도 설정 (선택사항)
    Hal_DustSensor_PPD71_SetSensitivity(PPD71_SENSITIVITY_NORMAL);
    
    while(1) {
        Hal_DustSensor_PPD71_Module_Control();               // 메인 루프에서 호출
        
        // 먼지 농도 읽기
        U16 pm2_5 = Hal_DustSensor_PPD71_GetPM2_5();
        U16 pm10 = Hal_DustSensor_PPD71_GetPM10();
        U8 error = Hal_DustSensor_PPD71_GetErrorFlag();
    }

    주의사항)
    - DUST_SENSOR_PPD71_USE가 USE로 설정되어야 함
    - 센서 전원 ON 후 안정화 시간(10초) 필요
    - UART 통신을 사용하므로 UART 설정이 완료되어야 함
    - 민감도: PPD71_SENSITIVITY_SHARP(높음), NORMAL(보통), DULL(낮음)
*/

// PPD71 민감도 설정 타입 정의
typedef enum
{
    PPD71_SENSITIVITY_SHARP,    // 센서 감도 높음
    PPD71_SENSITIVITY_NORMAL,   // 센서 감도 보통
    PPD71_SENSITIVITY_DULL      // 센서 감도 낮음
} PPD71_SENSITIVITY_T;

///////////////////////////////////////////////////////////////////////
/// Macro Definitions
//////////////////////////////////////////////////////////////////////

// 공통 포트 및 값 정의
#define PORT_DEFINE_PARTICLE_SENSOR_POW                 P5_bit.no0  // 센서 전원 제어 Port 설정 : P50
#define VALUE_PARTICLE_POWER_ON                         N_ON
#define VALUE_PARTICLE_POWER_OFF                        N_OFF
#define VALUE_PARTICLE_INITTIME                         10000
#define VALUE_PARTICLE_ERRORTIME                        10000

// PPD71 전용 설정
#define PPD71_CONTROL_TIME_PERIOD         1000    // 1초 주기
#define PPD71_DATASTREAM_CHECKTIME        30      // 데이터 스트림 체크 시간
#define PPD71_DATA_LENGTH            32
#define PPD71_START_BYTE             0x42
#define PPD71_BAUDRATE               9600
#define RXD_STX 0x02
#define DATA_LENGTH 29

// 보정 계수 정의 (계측기 농도 지향성 보정용) - 기존 값으로 복원
#define REF_COEFFICIENT_2 0.0021f // 기준 센서의 보정 계수 (2차 다항식 계수)
#define REF_COEFFICIENT_1 -0.3221f // 기준 센서의 보정 계수 (1차 다항식 계수)
#define REF_COEFFICIENT_0 4.3636f // 기준 센서의 보정 계수 (상수항)
#define REF_AREA_MAX 140.f // 기준 센서의 측정 범위 최대값(상한선)
#define REF_AREA_MIN 15.f // 기준 센서의 측정 범위 최소값(하한선)

// 민감도 보정 계수 정의 - 기존 값으로 복원
#define SEN_COEFFICIENT_2 -0.0009f // 보정 센서의 보정 계수 (2차 다항식 계수)
#define SEN_COEFFICIENT_1 0.1787f // 보정 센서의 보정 계수 (1차 다항식 계수)
#define SEN_COEFFICIENT_0 1.286f // 보정 센서의 보정 계수 (상수항)
#define SEN_AREA_MAX 200.f // 보정 센서의 측정 범위 최대값(상한선)
#define SEN_AREA_MIN 0.f // 보정 센서의 측정 범위 최소값(하한선)

///////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////

// 초기화 및 제어 함수
void Hal_DustSensor_PPD71_Init(void);
void Hal_DustSensor_PPD71_On(void);
void Hal_DustSensor_PPD71_Off(void);
void Hal_DustSensor_PPD71_Handler(void);

// 데이터 읽기 함수
void Hal_DustSensor_PPD71_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);

// 민감도 설정 함수
void Hal_DustSensor_PPD71_SetSensitivity(PPD71_SENSITIVITY_T sensitivity);
PPD71_SENSITIVITY_T Hal_DustSensor_PPD71_GetSensitivity(void);

// 내부 센서 처리 함수는 static으로 정의되어 외부에서 접근 불가

// 타이머 제어 함수
void Hal_DustSensor_PPD71_ControlTimer(void);
void Hal_DustSensor_PPD71_Module_Control(void);

// 데이터 getter 함수
U16 Hal_DustSensor_PPD71_GetPM2_5(void);
U16 Hal_DustSensor_PPD71_GetPM10(void);
D64 Hal_DustSensor_PPD71_GetPM2_5Final(void);
D64 Hal_DustSensor_PPD71_GetPM10Final(void);
U8 Hal_DustSensor_PPD71_GetErrorFlag(void);

// Module Interface Functions
void Hal_DustSensor_PPD71_Module_Initialize(void);
void Hal_DustSensor_PPD71_Module_1ms_Control(void);

#endif // HAL_DUSTSENSOR_PPD71_H
