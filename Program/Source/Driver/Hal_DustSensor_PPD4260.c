/// @file     Hal_DustSensor_PPD4260.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD4260 먼지센서 드라이버 구현 파일
/// @details  PPD4260 먼지센서 제어를 위한 HAL 드라이버 구현

#include "Global_Header.h"

#if (DUST_SENSOR_PPD4260_USE == USE)

// PPD4260 센서 관련 전역 변수
static U16 gu16VolumeP1 = 0;
static U16 gu16VolumeP2 = 0;
static U16 gu16SkipSensing = 0;
static D64 gd64CalAvgPM2p5 = 0;
static D64 gd64CalAvgPM10 = 0;

// 에러 플래그
static U8 g_errorFlag = CLEAR;

// 타이머 변수
static U8 gu8DustSensorControlTimer = 0;

// 내부 함수 전방 선언 (기존 패턴에 맞춰 public으로 변경)
void PPD4260_Init(void);
void PPD4260_Handler(void);
void PPD4260_ReadParticleData(void);
void PPD4260_CalParticleConcent(void);

// PPD4260 센서 내부 함수들
/// @brief      PPD4260 센서 초기화 함수
/// @param      void
/// @return     void
/// @details    PPD4260 센서의 변수들을 초기화
void PPD4260_Init(void) {
    gu16VolumeP1 = 0;
    gu16VolumeP2 = 0;
    gu16SkipSensing = 0;
    gd64CalAvgPM2p5 = 0;
    gd64CalAvgPM10 = 0;
}

/// @brief      PPD4260 센서 메인 핸들러
/// @param      void
/// @return     void
/// @details    PPD4260 센서의 데이터 수신과 계산을 순차적으로 수행
void PPD4260_Handler(void) {
    PPD4260_ReadParticleData();
    PPD4260_CalParticleConcent();
    // PPD4260_CheckParticleDataError();
}

/// @brief      PPD4260 센서 데이터 수신 함수
/// @param      void
/// @return     void
/// @details    PPD4260 센서의 P1, P2 포트로부터 디지털 신호를 수신
void PPD4260_ReadParticleData(void) {
    if(gu16SkipSensing < PARTICLE_INITTIME) {
        gu16SkipSensing++;
        return;
    }

    // if(getGPIOState(GPIO_PARTICLE_SENSOR_P1)) {
    //     gu16VolumeP1++;
    // }

    // if(getGPIOState(GPIO_PARTICLE_SENSOR_P2)) {
    //     gu16VolumeP2++;
    // }
}

/// @brief      PPD4260 미세먼지 농도 계산 함수
/// @param      void
/// @return     void
/// @details    수신된 볼륨 데이터를 바탕으로 PM2.5와 PM10 농도를 계산
void PPD4260_CalParticleConcent(void) {
    // PPD4260의 농도 계산
    D64 d64Temp = 0;

    // PM2.5 계산
    d64Temp = (D64)gu16VolumeP1 * 1000.0 / 30000.0;
    gd64CalAvgPM2p5 = d64Temp;

    // PM10 계산
    d64Temp = (D64)gu16VolumeP2 * 1000.0 / 30000.0;
    gd64CalAvgPM10 = d64Temp;
}

// static void PPD4260_CheckParticleDataError(void) {
//     if(gu16SkipSensing >= PARTICLE_ERRORTIME) {
//         g_errorFlag = SET;
//     } else {
//         g_errorFlag = CLEAR;
//     }
// }

// 공용 인터페이스 함수들
/// @brief      PPD4260 먼지센서 초기화
/// @param      void
/// @return     void
/// @details    PPD4260 센서의 에러 플래그를 클리어하고 내부 초기화 함수 호출
void Hal_DustSensor_PPD4260_Init(void) {
    g_errorFlag = CLEAR;
    PPD4260_Init();
}

/// @brief      PPD4260 먼지센서 전원 ON
/// @param      void
/// @return     void
/// @details    PPD4260 센서의 전원을 사용 상태로 설정
void Hal_DustSensor_PPD4260_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

/// @brief      PPD4260 먼지센서 전원 OFF
/// @param      void
/// @return     void
/// @details    PPD4260 센서의 전원을 비사용 상태로 설정
void Hal_DustSensor_PPD4260_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

/// @brief      PPD4260 먼지센서 데이터 읽기
/// @param      pm1_0 PM1.0 농도 데이터 포인터 (현재 지원안함)
/// @param      pm2_5 PM2.5 농도 데이터 포인터
/// @param      pm10 PM10 농도 데이터 포인터
/// @param      error 오류 상태 플래그 포인터
/// @return     void
/// @details    PPD4260 센서에서 측정된 PM2.5, PM10 농도 데이터와 에러 상태를 반환
void Hal_DustSensor_PPD4260_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
    // NULL 포인터 체크
    if (!pm1_0 || !pm2_5 || !pm10 || !error) return;

    // 에러 상태 설정
    *error = g_errorFlag;

    // 센서 데이터 유효성 체크
    if (g_errorFlag != CLEAR) {
        *pm1_0 = 0;
        *pm2_5 = 0;
        *pm10 = 0;
        return;
    }

    // 정상 데이터 할당
    *pm1_0 = 0; // PPD4260은 PM1.0 측정 불가
    *pm2_5 = (U16)gd64CalAvgPM2p5;
    *pm10 = (U16)gd64CalAvgPM10;
}

/// @brief      PPD4260 먼지센서 핸들러 (외부 인터페이스)
/// @param      void
/// @return     void
/// @details    외부에서 호출 가능한 PPD4260 센서 핸들러 인터페이스
void Hal_DustSensor_PPD4260_Handler(void) {
    PPD4260_Handler();
}

/// @brief      Hal Dust Sensor PPD4260 Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 PPD4260 센서 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD4260_Module_1ms_Control(void)
{
    // 1s마다 타이머 카운트 증가
    if (gu8DustSensorControlTimer < PPD4260_CONTROL_TIME_PERIOD)
    {
        gu8DustSensorControlTimer++;
    }
}

/// @brief      Hal Dust Sensor PPD4260 Module in While Loop
/// @details    Basic Loop 의 While문 안에서 PPD4260 센서 제어 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD4260_Module_Control(void)
{
    if (gu8DustSensorControlTimer >= PPD4260_CONTROL_TIME_PERIOD)
    {
        Hal_DustSensor_PPD4260_Handler();
        gu8DustSensorControlTimer = 0;
    }
}

// Getter 함수들
/// @brief      PPD4260 PM2.5 농도 값 반환
/// @param      void
/// @return     U16 PM2.5 농도 값 (μg/m³)
/// @details    PPD4260 센서에서 측정된 PM2.5 농도 값을 반환
U16 Hal_DustSensor_PPD4260_GetPM2_5(void) {
    return (U16)gd64CalAvgPM2p5;
}

/// @brief      PPD4260 PM10 농도 값 반환
/// @param      void
/// @return     U16 PM10 농도 값 (μg/m³)
/// @details    PPD4260 센서에서 측정된 PM10 농도 값을 반환
U16 Hal_DustSensor_PPD4260_GetPM10(void) {
    return (U16)gd64CalAvgPM10;
}

D64 Hal_DustSensor_PPD4260_GetPM2_5Avg(void) {
    return gd64CalAvgPM2p5;
}

D64 Hal_DustSensor_PPD4260_GetPM10Avg(void) {
    return gd64CalAvgPM10;
}

U8 Hal_DustSensor_PPD4260_GetErrorFlag(void) {
    return g_errorFlag;
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor PPD4260 Initialize Module
/// @details    PPD4260 먼지센서 관련 모든 것을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD4260_Module_Initialize(void)
{
#if (DUST_SENSOR_PPD4260_USE == USE)
    Hal_DustSensor_PPD4260_Init();             // PPD4260 센서 초기화
#endif
}

#endif  // DUST_SENSOR_PPD4260_USE == USE
