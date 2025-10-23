//'""FILE COMMENT""****************************************************
//@ FILE Name : DustSensor.h
//@ Description : 미세먼지 센서 드라이버 관련 헤더 파일
//""FILE COMMENT END ""************************************************

#ifndef _DUST_SENSOR_H_
#define _DUST_SENSOR_H_


// ******************************************************************
//                            Dust Sensor 설정
// ******************************************************************
#define DUST_SENSOR_USE                                 USE           // Dust Sensor 사용 여부 - 0(No Use), 1(Use)
#define USE_PPD42NS                                     NO_USE           // PPD42NS 사용 여부 - 0(No Use), 1(Use)
#define USE_PPD4260                                     USE           // PPD4260 사용 여부 - 0(No Use), 1(Use)
#define USE_PPD71                                       NO_USE           // PPD71 사용 여부 - 0(No Use), 1(Use)
#define USE_PMS5003                                     NO_USE           // PMS5003 사용 여부 - 0(No Use), 1(Use)

#define VALUE_PARTICLE_INITTIME                         10000
#define VALUE_PARTICLE_ERRORTIME                        10000
#define VALUE_PARTICLE_DATA_CHECKTIME                   20          // 100msec * 20 = 2sec
#define VALUE_PARTICLE_POWER_ON                         N_ON
#define VALUE_PARTICLE_POWER_OFF                        N_OFF
#define VALUE_PARTICLE_DATA_CHECKTIME                   20 //100ms*20 (2sec)
#define VALUE_PARTICLE_DATA_TIMEOUTCOUNT                5
#define VALUE_PARTICLE_DATA_ERRORCOUNT                  5
#define VALUE_PARTICLE_DATASTREAM_CHECKTIME             20 //10ms*20 (200ms)

#define PORT_DEFINE_PARTICLE_SENSOR_POW                 P5_bit.no0  // 센서 전원 제어 Port 세팅 : P50

#if (USE_PPD42NS == 1)
#define PORT_DEFINE_PARTICLE_IN                         P9_bit.no4
#endif


#if (DUST_SENSOR_USE == 1)

// 1. 센서 타입 정의 - 사용할 센서 타입을 선택
// #define USE_PMS5003
// #define USE_PPD71
// #define USE_PPD42NS
// #define USE_PPD4260

#define DUST_SENSOR_CONTROL_TIME_PERIOD 1000 // 1sec

// 센서 타입에 따른 설정값 정의
#if defined(USE_PMS5003)
    #define CURRENT_SENSOR_TYPE SENSOR_TYPE_PMS5003
#elif defined(USE_PPD71)
    #define CURRENT_SENSOR_TYPE SENSOR_TYPE_PPD71
#elif defined(USE_PPD42NS)
    #define CURRENT_SENSOR_TYPE SENSOR_TYPE_PPD42NS
    #define PPD42NS_CAL_TIME_PERIOD 2000 // 2sec
    #define PPD42NS_READ_TIME_PERIOD 1 // 1mSec

#elif defined(USE_PPD4260)
    #define CURRENT_SENSOR_TYPE SENSOR_TYPE_PPD4260
#else
    #define CURRENT_SENSOR_TYPE SENSOR_TYPE_NONE
    #warning "No dust sensor type selected. Please define one of USE_PMS5003, USE_PPD71, USE_PPD42NS, or USE_PPD4260."
#endif

// 2. 센서 타입 열거형
typedef enum {
    SENSOR_TYPE_NONE = 0,
    SENSOR_TYPE_PMS5003,
    SENSOR_TYPE_PPD71,
    SENSOR_TYPE_PPD42NS,
    SENSOR_TYPE_PPD4260,
    SENSOR_TYPE_MAX
} SENSOR_TYPE_T;
#if(USE_PMS5003 == 1)
typedef enum
{
	SENSITIVITY_NULL,
	SENSITIVITY_SHARP_HIGH,
	SENSITIVITY_SHARP_MID,
	SENSITIVITY_SHARP_LOW,
	SENSITIVITY_NORMAL,
	SENSITIVITY_DULL_LOW,
	SENSITIVITY_DULL_MID,
	SENSITIVITY_DULL_HIGH,
} SENSITIVITY_T;
#elif (USE_PPD42NS == 1 || USE_PPD71 == 1 || USE_PPD4260 == 1)
typedef enum
{
    SENSITIVITY_SHARP,    // 센서 감도 높음
    SENSITIVITY_NORMAL,   // 센서 감도 보통
    SENSITIVITY_DULL      // 센서 감도 낮음
} SENSITIVITY_T;
#endif

// 3. 센서 드라이버 함수 프로토타입
void DustSensor_Init(void);
void DustSensor_On(void);
void DustSensor_Off(void);
void DustSensor_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);
void DustSensor_SetSensitivity(SENSITIVITY_T sensitivity);
SENSITIVITY_T DustSensor_GetSensitivity(void);
void DustSensor_Handler(void);
SENSOR_TYPE_T DustSensor_GetType(void);
void DustSensorControlTimer(void);
void DustSensor_SetType(SENSOR_TYPE_T type);


// 4. 센서별 설정값 정의
//미세먼지 데이터
#define PARTICLE_DATA_CHECKTIME 20 //100ms*20 (2sec)
#define PARTICLE_DATA_TIMEOUTCOUNT 5
#define PARTICLE_DATA_ERRORCOUNT 5
#define PARTICLE_DATASTREAM_CHECKTIME 20 //10ms*20 (200ms)

#define REF_COEFFICIENT_2 0.0021f // 기준 센서의 보정 계수 (2차 다항식 계수)
#define REF_COEFFICIENT_1 -0.3221f // 기준 센서의 보정 계수 (1차 다항식 계수)
#define REF_COEFFICIENT_0 4.3636f // 기준 센서의 보정 계수 (상수항)
#define REF_AREA_MAX 140.f // 기준 센서의 측정 범위 최대값(상한선)
#define REF_AREA_MIN 15.f // 기준 센서의 측정 범위 최소값(하한선)

#define SEN_COEFFICIENT_2 -0.0009f // 보정 센서의 보정 계수 (2차 다항식 계수)
#define SEN_COEFFICIENT_1 0.1787f // 보정 센서의 보정 계수 (1차 다항식 계수)
#define SEN_COEFFICIENT_0 1.286f // 보정 센서의 보정 계수 (상수항)
#define SEN_AREA_MAX 200.f // 보정 센서의 측정 범위 최대값(상한선)
#define SEN_AREA_MIN 0.f // 보정 센서의 측정 범위 최소값(하한선)

#define RXD_STX 0x02
#define DATA_LENGTH 29

#if (USE_PMS5003 == 1)
    // PMS5003 설정
    #define PMS5003_DATA_LENGTH          32
    #define PMS5003_START_BYTE1          0x42
    #define PMS5003_START_BYTE2          0x4d
    #define PMS5003_BAUDRATE             9600
    #define PMS5003_DATASTREAM_CHECKTIME 10
#endif

#if (USE_PPD71 == 1)
    // PPD71 설정
    #define PPD71_DATA_LENGTH            32
    #define PPD71_START_BYTE             0x42
    #define PPD71_BAUDRATE               9600
    #define PPD71_DATASTREAM_CHECKTIME   10
    #define RXD_STX 0x02
    #define DATA_LENGTH 29
#endif

#if (USE_PPD42NS == 1)
    // PPD42NS 설정
    #define PARTICLE_INITTIME            1000
    #define PARTICLE_ERRORTIME           5000
    #define PARTICLE_COUNT_TIME          5
    #define PARTICLE_VOLUMESUM_TIME      2000
    #define PARTICLE_ERROR_CHECK_TIME    5
#endif

#if (USE_PPD4260 == 1)
    // PPD4260 설정
    #define PARTICLE_INITTIME            1000
    #define PARTICLE_ERRORTIME           5000
    #define PARTICLE_COUNT_TIME          1
    #define PARTICLE_CONCENT_TIME        1000
    #define PARTICLE_ERROR_CHECK_TIME    1
#endif

// 5. 타이머 타입 열거형
typedef enum {
    TimerParticleDataStreamError = 0,
    TimerParticleCount,
    TimerParticleVolumeSum,
    TimerParticleConcent,
    TimerParticleError,
    TimerParticle_MAX
} TIMER_T;

#endif /* _DUST_SENSOR_H_ */
#endif /* DUST_SENSOR_USE */