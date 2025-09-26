/// @file     Hal_DustSensor_PMS5003.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PMS5003 먼지센서 드라이버 구현 파일
/// @details  PMS5003 먼지센서 제어를 위한 HAL 드라이버 구현

#include "Global_Header.h"

#if (DUST_SENSOR_PMS5003_USE == USE)

// PMS5003 센서 관련 전역 변수
static volatile U8 s_packet_received_flag = 0;           // 수신 완료 플래그
static U8 gu8UARTOK_F = 0;
static U8 gu8ParticleRxDataBuffer[32] = {0};
static U16 gu16PM1p0Data = 0;
static U16 gu16PM1p0DataStd = 0;
static U16 gu16PM2p5DataStd = 0;
static U16 gu16PM10DataStd = 0;
static U16 gu16PM2p5ConcentDisplayFinal = 0;
static U16 gu16PM10ConcentDisplayFinal = 0;
static U16 gu16Dust0p3CntData = 0;
static U16 gu16Dust0p5CntData = 0;
static U16 gu16Dust1p0CntData = 0;
static U16 gu16Dust2p5CntData = 0;
static U16 gu16Dust5p0CntData = 0;
static U16 gu16Dust10CntData = 0;
// 에러 관련 변수들은 향후 확장 시 사용될 수 있음

// 최종 데이터 저장 변수
static U16 gu16PM1_0Finish = 0;
static U16 gu16PM2_5Finish = 0;
static U16 gu16PM10Finish = 0;

// 에러 플래그
static U8 g_errorFlag = CLEAR;

// 타이머 변수
static U8 gu8DustSensorControlTimer = 0;

// 내부 함수 전방 선언 (기존 패턴에 맞춰 public으로 변경)
void PMS5003_Init(void);
void PMS5003_Handler(void);
void PMS5003_RxParticleData(void);
void PMS5003_CalcParticleData(void);
void PMS5003_CheckError(void);
void PMS5003_CheckDataStream(void);
// 유틸리티 함수 전방 선언 제거

// 유틸리티 함수들은 static으로 정의되어 내부에서만 사용

// PMS5003 센서 내부 함수들
/// @brief      PMS5003 센서 초기화 함수
/// @param      void
/// @return     void
/// @details    PMS5003 센서의 변수들을 초기화하고 UART 수신 버퍼를 클리어
void PMS5003_Init(void) {
    gu8UARTOK_F = 0;
    MEMSET( (void __FAR *)&gu8ParticleRxDataBuffer, 0x00, sizeof(gu8ParticleRxDataBuffer) );
}

/// @brief      PMS5003 센서 메인 핸들러
/// @param      void
/// @return     void
/// @details    PMS5003 센서의 데이터 수신, 계산, 에러 체크를 순차적으로 수행
void PMS5003_Handler(void) {
    PMS5003_RxParticleData();   // 먼지센서 데이터 수신
    PMS5003_CalcParticleData(); // 먼지센서 데이터 계산
    PMS5003_CheckError();        // 먼지센서 데이터 오류 체크
    PMS5003_CheckDataStream();   // 먼지센서 데이터 스트림 오류 체크
}

/// @brief      PMS5003 센서 데이터 수신 함수
/// @param      void
/// @return     void
/// @details    UART2를 통해 PMS5003 센서로부터 32바이트 데이터를 수신
void PMS5003_RxParticleData(void) {
    MD_STATUS status = R_Config_UART2_Receive((U8*)gu8ParticleRxDataBuffer, 32);
    if (status != MD_OK) {
        // 에러 처리: 수신 재시작 실패
    }
}

/// @brief      PMS5003 미세먼지 데이터 계산 함수
/// @param      void
/// @return     void
/// @details    수신된 데이터를 바탕으로 PM1.0, PM2.5, PM10 농도와 입자 개수를 계산
void PMS5003_CalcParticleData(void) {
    U16 Temp16 = 0;

    if(gu8UARTOK_F) {

        gu8UARTOK_F = 0;

        gu16PM1p0DataStd = gu8ParticleRxDataBuffer[4];
        gu16PM1p0DataStd <<= 8;
        gu16PM1p0DataStd |= gu8ParticleRxDataBuffer[5];

        gu16PM2p5DataStd = gu8ParticleRxDataBuffer[6];
        gu16PM2p5DataStd <<= 8;
        gu16PM2p5DataStd |= gu8ParticleRxDataBuffer[7];

        gu16PM10DataStd = gu8ParticleRxDataBuffer[8];
        gu16PM10DataStd <<= 8;
        gu16PM10DataStd |= gu8ParticleRxDataBuffer[9];

        gu16PM1p0Data = gu8ParticleRxDataBuffer[10];
        gu16PM1p0Data <<= 8;
        gu16PM1p0Data |= gu8ParticleRxDataBuffer[11];

        Temp16 = gu8ParticleRxDataBuffer[12];
        Temp16 <<= 8;
        Temp16 |= gu8ParticleRxDataBuffer[13];
        gu16PM2p5ConcentDisplayFinal = Temp16;

        Temp16 = gu8ParticleRxDataBuffer[14];
        Temp16 <<= 8;
        Temp16 |= gu8ParticleRxDataBuffer[15];
        gu16PM10ConcentDisplayFinal = Temp16;

        gu16Dust0p3CntData = gu8ParticleRxDataBuffer[16];
        gu16Dust0p3CntData <<= 8;
        gu16Dust0p3CntData |= gu8ParticleRxDataBuffer[17];

        gu16Dust0p5CntData = gu8ParticleRxDataBuffer[18];
        gu16Dust0p5CntData <<= 8;
        gu16Dust0p5CntData |= gu8ParticleRxDataBuffer[19];

        gu16Dust1p0CntData = gu8ParticleRxDataBuffer[20];
        gu16Dust1p0CntData <<= 8;
        gu16Dust1p0CntData |= gu8ParticleRxDataBuffer[21];

        gu16Dust2p5CntData = gu8ParticleRxDataBuffer[22];
        gu16Dust2p5CntData <<= 8;
        gu16Dust2p5CntData |= gu8ParticleRxDataBuffer[23];

        gu16Dust5p0CntData = gu8ParticleRxDataBuffer[24];
        gu16Dust5p0CntData <<= 8;
        gu16Dust5p0CntData |= gu8ParticleRxDataBuffer[25];

        gu16Dust10CntData = gu8ParticleRxDataBuffer[26];
        gu16Dust10CntData <<= 8;
        gu16Dust10CntData |= gu8ParticleRxDataBuffer[27];

        // 최종값 업데이트
        gu16PM1_0Finish = gu16PM1p0Data;
        gu16PM2_5Finish = gu16PM2p5ConcentDisplayFinal;
        gu16PM10Finish = gu16PM10ConcentDisplayFinal;
    }
}

/// @brief      PMS5003 센서 에러 체크 함수
/// @param      void
/// @return     void
/// @details    PMS5003 센서의 에러 상태를 체크 (현재 미구현)
void PMS5003_CheckError(void) {
    // 미구현
}

/// @brief      PMS5003 센서 데이터스트림 체크 함수
/// @param      void
/// @return     void
/// @details    PMS5003 센서의 데이터 스트림 상태를 체크 (현재 미구현)
void PMS5003_CheckDataStream(void) {
    // 미구현
}

// 공용 인터페이스 함수들
/// @brief      PMS5003 먼지센서 초기화
/// @param      void
/// @return     void
/// @details    PMS5003 센서의 에러 플래그를 클리어하고 내부 초기화 함수 호출
void Hal_DustSensor_PMS5003_Init(void) {
    g_errorFlag = CLEAR;
    PMS5003_Init();
}

/// @brief      PMS5003 먼지센서 전원 ON
/// @param      void
/// @return     void
/// @details    PMS5003 센서의 전원을 사용 상태로 설정
void Hal_DustSensor_PMS5003_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

/// @brief      PMS5003 먼지센서 전원 OFF
/// @param      void
/// @return     void
/// @details    PMS5003 센서의 전원을 비사용 상태로 설정
void Hal_DustSensor_PMS5003_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

/// @brief      PMS5003 먼지센서 데이터 읽기
/// @param      pm1_0 PM1.0 농도 데이터 포인터
/// @param      pm2_5 PM2.5 농도 데이터 포인터
/// @param      pm10 PM10 농도 데이터 포인터
/// @param      error 오류 상태 플래그 포인터
/// @return     void
/// @details    PMS5003 센서에서 측정된 PM1.0, PM2.5, PM10 농도 데이터와 에러 상태를 반환
void Hal_DustSensor_PMS5003_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
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
    *pm1_0 = gu16PM1_0Finish;
    *pm2_5 = gu16PM2_5Finish;
    *pm10 = gu16PM10Finish;
}

/// @brief      PMS5003 먼지센서 핸들러 (외부 인터페이스)
/// @param      void
/// @return     void
/// @details    외부에서 호출 가능한 PMS5003 센서 핸들러 인터페이스
void Hal_DustSensor_PMS5003_Handler(void) {
    PMS5003_Handler();
}

/// @brief      Hal Dust Sensor PMS5003 Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 PMS5003 센서 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PMS5003_Module_1ms_Control(void)
{
    // 1s마다 타이머 카운트 증가
    if (gu8DustSensorControlTimer < PMS5003_CONTROL_TIME_PERIOD)
    {
        gu8DustSensorControlTimer++;
    }
}

/// @brief      Hal Dust Sensor PMS5003 Module in While Loop
/// @details    Basic Loop 의 While문 안에서 PMS5003 센서 제어 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PMS5003_Module_Control(void)
{
    if (gu8DustSensorControlTimer >= PMS5003_CONTROL_TIME_PERIOD)
    {
        Hal_DustSensor_PMS5003_Handler();
        gu8DustSensorControlTimer = 0;
    }
}

// Getter 함수들
/// @brief      PMS5003 PM1.0 농도 값 반환
/// @param      void
/// @return     U16 PM1.0 농도 값 (μg/m³)
/// @details    PMS5003 센서에서 측정된 PM1.0 농도 값을 반환
U16 Hal_DustSensor_PMS5003_GetPM1_0(void) {
    return gu16PM1_0Finish;
}

/// @brief      PMS5003 PM2.5 농도 값 반환
/// @param      void
/// @return     U16 PM2.5 농도 값 (μg/m³)
/// @details    PMS5003 센서에서 측정된 PM2.5 농도 값을 반환
U16 Hal_DustSensor_PMS5003_GetPM2_5(void) {
    return gu16PM2_5Finish;
}

/// @brief      PMS5003 PM10 농도 값 반환
/// @param      void
/// @return     U16 PM10 농도 값 (μg/m³)
/// @details    PMS5003 센서에서 측정된 PM10 농도 값을 반환
U16 Hal_DustSensor_PMS5003_GetPM10(void) {
    return gu16PM10Finish;
}

/// @brief      PMS5003 센서 에러 플래그 반환
/// @param      void
/// @return     U8 에러 플래그 (SET/CLEAR)
/// @details    PMS5003 센서의 현재 에러 상태를 반환
U8 Hal_DustSensor_PMS5003_GetErrorFlag(void) {
    return g_errorFlag;
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor PMS5003 Initialize Module
/// @details    PMS5003 먼지센서 관련 모든 것을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DustSensor_PMS5003_Module_Initialize(void)
{
#if (DUST_SENSOR_PMS5003_USE == USE)
    Hal_DustSensor_PMS5003_Init();             // PMS5003 센서 초기화
#endif
}

#endif  // DUST_SENSOR_PMS5003_USE == USE
