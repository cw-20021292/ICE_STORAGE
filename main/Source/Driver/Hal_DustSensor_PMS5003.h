#ifndef _HAL_DUSTSENSOR_PMS5003_H_
#define _HAL_DUSTSENSOR_PMS5003_H_

/// @file     Hal_DustSensor_PMS5003.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PMS5003 먼지센서 드라이버 헤더 파일
/// @details  PMS5003 먼지센서 제어를 위한 HAL 드라이버 헤더

// Include standard header for type definitions
#define DUST_SENSOR_PMS5003_USE USE

/*  PMS5003 먼지센서 제어 Function 사용법
    => Hal_DustSensor_PMS5003_Init();                        // PMS5003 센서 초기화
    => Hal_DustSensor_PMS5003_On();                          // PMS5003 센서 전원 ON
    => Hal_DustSensor_PMS5003_Off();                         // PMS5003 센서 전원 OFF
    => Hal_DustSensor_PMS5003_Handler();                     // PMS5003 센서 데이터 처리 (주기적 호출)
    => Hal_DustSensor_PMS5003_ReadData(&pm1_0, &pm2_5, &pm10, &error); // 먼지 데이터 읽기
    => Hal_DustSensor_PMS5003_GetPM1_0();                    // PM1.0 값 읽기
    => Hal_DustSensor_PMS5003_GetPM2_5();                    // PM2.5 값 읽기
    => Hal_DustSensor_PMS5003_GetPM10();                     // PM10 값 읽기

    사용 예)
    Hal_DustSensor_PMS5003_Module_Initialize();              // 시스템 초기화 시 호출
    Hal_DustSensor_PMS5003_On();                             // 센서 전원 ON
    
    while(1) {
        Hal_DustSensor_PMS5003_Module_Control();             // 메인 루프에서 호출
        
        // 먼지 농도 읽기
        U16 pm1_0 = Hal_DustSensor_PMS5003_GetPM1_0();
        U16 pm2_5 = Hal_DustSensor_PMS5003_GetPM2_5();
        U16 pm10 = Hal_DustSensor_PMS5003_GetPM10();
        U8 error = Hal_DustSensor_PMS5003_GetErrorFlag();
    }

    주의사항)
    - DUST_SENSOR_PMS5003_USE가 USE로 설정되어야 함
    - 센서 전원 ON 후 안정화 시간(10초) 필요
    - UART 통신을 사용하므로 UART 설정이 완료되어야 함
    - Start Byte: 0x42, 0x4D (순서대로)
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

// PMS5003 전용 설정
#define PMS5003_CONTROL_TIME_PERIOD    1000    // 1초 주기
#define PMS5003_DATA_LENGTH          32
#define PMS5003_START_BYTE1          0x42
#define PMS5003_START_BYTE2          0x4d
#define PMS5003_BAUDRATE             9600
#define PMS5003_DATASTREAM_CHECKTIME 10

// PMS5003 민감도 타입 정의 (확장된 버전)
typedef enum
{
	PMS5003_SENSITIVITY_NULL,
	PMS5003_SENSITIVITY_SHARP_HIGH,
	PMS5003_SENSITIVITY_SHARP_MID,
	PMS5003_SENSITIVITY_SHARP_LOW,
	PMS5003_SENSITIVITY_NORMAL,
	PMS5003_SENSITIVITY_DULL_LOW,
	PMS5003_SENSITIVITY_DULL_MID,
	PMS5003_SENSITIVITY_DULL_HIGH,
} PMS5003_SENSITIVITY_T;

///////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////

// 초기화 및 제어 함수
void Hal_DustSensor_PMS5003_Init(void);
void Hal_DustSensor_PMS5003_On(void);
void Hal_DustSensor_PMS5003_Off(void);
void Hal_DustSensor_PMS5003_Handler(void);

// 데이터 읽기 함수
void Hal_DustSensor_PMS5003_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);

// 내부 센서 처리 함수는 static으로 정의되어 외부에서 접근 불가

// 타이머 제어 함수
void Hal_DustSensor_PMS5003_ControlTimer(void);
void Hal_DustSensor_PMS5003_Module_Control(void);

// 데이터 getter 함수
U16 Hal_DustSensor_PMS5003_GetPM1_0(void);
U16 Hal_DustSensor_PMS5003_GetPM2_5(void);
U16 Hal_DustSensor_PMS5003_GetPM10(void);
U8 Hal_DustSensor_PMS5003_GetErrorFlag(void);

// Module Interface Functions
void Hal_DustSensor_PMS5003_Module_Initialize(void);
void Hal_DustSensor_PMS5003_Module_1ms_Control(void);

#endif // _HAL_DUSTSENSOR_PMS5003_H_
