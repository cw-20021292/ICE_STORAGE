/// @file     Hal_DustSensor_PPD42NS.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD42NS 먼지센서 드라이버 구현 파일
/// @details  PPD42NS 먼지센서 제어를 위한 HAL 드라이버 구현

#include "Global_Header.h"

#if (DUST_SENSOR_PPD42NS_USE == USE)

// PPD42NS 센서 관련 전역 변수
static U16 gu16Volume = 0;
static U16 gu16VolumeBuf[5] = {0};
static U16 gu16VolumeSum = 0;
static U16 gu16SkipSensing = 0;
static U16 gu16ParticleInitTime = PARTICLE_INITTIME;
static U16 gu16ErrParticle = PARTICLE_ERRORTIME;
static U8 gu8PPD42NSTimer = 0;

// 에러 플래그
static U8 g_errorFlag = CLEAR;

// 타이머 변수
static U8 gu8DustSensorControlTimer = 0;

// 내부 함수 전방 선언 (기존 패턴에 맞춰 public으로 변경)
void PPD42NS_Init(void);
void PPD42NS_Handler(void);
void PPD42NS_ReadParticleData(void);
void PPD42NS_CalParticleConcent(void);
void PPD42NS_CheckParticleDataError(void);
void initParticleSensorVar(void);

// 유틸리티 함수
/// @brief      PPD42NS 센서 변수 초기화 유틸리티 함수
/// @param      void
/// @return     void
/// @details    PPD42NS 센서의 내부 변수들을 초기화
void initParticleSensorVar(void) {
    gu16Volume = 0;
    gu16VolumeSum = 0;
    gu16SkipSensing = 0;
    gu16ParticleInitTime = PARTICLE_INITTIME;
    gu16ErrParticle = PARTICLE_ERRORTIME;
    MEMSET( (void __FAR *)&gu16VolumeBuf, 0x00, sizeof(gu16VolumeBuf) );
}

// PPD42NS 센서 내부 함수들
/// @brief      PPD42NS 센서 초기화 함수
/// @param      void
/// @return     void
/// @details    PPD42NS 센서의 변수들을 초기화하고 볼륨 버퍼를 클리어
void PPD42NS_Init(void) {
    gu16Volume = 0;
    gu16VolumeSum = 0;
    gu16SkipSensing = 0;
    gu16ParticleInitTime = PARTICLE_INITTIME;
    gu16ErrParticle = PARTICLE_ERRORTIME;
    MEMSET( (void __FAR *)&gu16VolumeBuf, 0x00, sizeof(gu16VolumeBuf) );
}

/// @brief      PPD42NS 센서 메인 핸들러
/// @param      void
/// @return     void
/// @details    PPD42NS 센서의 데이터 수신, 계산, 에러 체크를 순차적으로 수행
void PPD42NS_Handler(void) {
    PPD42NS_ReadParticleData();
    PPD42NS_CalParticleConcent();
    PPD42NS_CheckParticleDataError();
}

/// @brief      PPD42NS 센서 데이터 수신 함수
/// @param      void
/// @return     void
/// @details    PPD42NS 센서의 디지털 포트 상태를 읽어 미세먼지 볼륨 데이터를 수집
void PPD42NS_ReadParticleData(void) {
	static U8 mu8PoweroffInit_F = 0;

    // 1mSec 주기로 호출
	// if(!checkTimeOver(TimerParticleCount))
	// {
	// 	return;
	// }
	// else
	// {
	// 	registTimer(TimerParticleCount, Time1mSec*5);
	// }

	if(PORT_DEFINE_PARTICLE_SENSOR_POW == VALUE_PARTICLE_POWER_OFF)
	{
		if(mu8PoweroffInit_F == 0)
		{
			initParticleSensorVar();
			mu8PoweroffInit_F = 1;
			return;
		}
		else
		{
			return;
		}
	}
	else
	{
		mu8PoweroffInit_F = 0;
	}

	if(gu16SkipSensing)
	{
		gu16SkipSensing--;
		return;
	}

	if(gu16ParticleInitTime)
	{
		gu16ParticleInitTime--;
		return;
	}

	if(PORT_DEFINE_PARTICLE_IN == 0)
	{
		gu16Volume++;
		if(gu16ErrParticle)
		{
			gu16ErrParticle--;
		}
	}
	else
	{
		gu16ErrParticle = PARTICLE_ERRORTIME;
	}
}

/// @brief      PPD42NS 미세먼지 농도 계산 함수
/// @param      void
/// @return     void
/// @details    수집된 볼륨 데이터를 바탕으로 평균 미세먼지 농도를 계산
void PPD42NS_CalParticleConcent(void) {
    static U8 mu8index = 0;
	static U8 mu8BufFull_F = 0;
	U8 mu8tmp = 0;

    // 2초 주기로 호출
	// if(!checkTimeOver(TimerParticleVolumeSum))
	// {
	// 	return;
	// }
	// else
	// {
	// 	registTimer(TimerParticleVolumeSum, Time1Sec*2);
	// }

	if(PORT_DEFINE_PARTICLE_IN == 0)
	{
		mu8index = 0;
		mu8BufFull_F = CLEAR;
		return;
	}

	if(!gu16ParticleInitTime)
	{
		return;
	}

	// 최대값 제한
	if(gu16Volume > 250) gu16Volume = 250;

	gu16VolumeBuf[mu8index] = gu16Volume;
	gu16Volume = 0;
	gu16VolumeSum = 0;
	mu8index++;

	if(mu8BufFull_F)	// Initial time 종료 후, 버퍼에 값이 완전히 찼는지 확인
	{
		for(mu8tmp = 0; mu8tmp < 5; mu8tmp++)
		{
			gu16VolumeSum += gu16VolumeBuf[mu8tmp];
		}
	}
	else	// 버퍼에 값이 완전히 차지 않았다면, 버퍼에 존재하는 값의 평균을 구한 후, 그 평균값을 사용하여 gu16VolumeSum을 확정
	{
		for(mu8tmp = 0; mu8tmp < mu8index; mu8tmp++)
		{
			gu16VolumeSum += gu16VolumeBuf[mu8tmp];
		}
		gu16VolumeSum = (gu16VolumeSum/mu8index)*5;

		if(mu8index == 5)
		{
			mu8BufFull_F = SET;
		}
	}
	mu8index = mu8index % 5;
}

/// @brief      PPD42NS 센서 데이터 에러 체크 함수
/// @param      void
/// @return     void
/// @details    센서 데이터 수신 에러를 체크하여 전체 에러 플래그를 설정
void PPD42NS_CheckParticleDataError(void) {
    if(gu16ErrParticle) {
        g_errorFlag = SET;
    } else {
        g_errorFlag = CLEAR;
    }
}

// 공용 인터페이스 함수들
/// @brief      PPD42NS 먼지센서 초기화
/// @param      void
/// @return     void
/// @details    PPD42NS 센서의 에러 플래그를 클리어하고 내부 초기화 함수 호출
void Hal_DustSensor_PPD42NS_Init(void) {
    g_errorFlag = CLEAR;
    PPD42NS_Init();
}

/// @brief      PPD42NS 먼지센서 전원 ON
/// @param      void
/// @return     void
/// @details    PPD42NS 센서의 전원을 사용 상태로 설정
void Hal_DustSensor_PPD42NS_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

/// @brief      PPD42NS 먼지센서 전원 OFF
/// @param      void
/// @return     void
/// @details    PPD42NS 센서의 전원을 비사용 상태로 설정
void Hal_DustSensor_PPD42NS_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

/// @brief      PPD42NS 먼지센서 데이터 읽기
/// @param      pm1_0 PM1.0 농도 데이터 포인터 (현재 지원안함)
/// @param      pm2_5 PM2.5 농도 데이터 포인터
/// @param      pm10 PM10 농도 데이터 포인터 (현재 지원안함)
/// @param      error 오류 상태 플래그 포인터
/// @return     void
/// @details    PPD42NS 센서에서 측정된 PM2.5 농도 데이터와 에러 상태를 반환
void Hal_DustSensor_PPD42NS_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
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
    *pm1_0 = 0; // PPD42NS는 PM1.0 측정 불가
    *pm2_5 = gu16VolumeSum;
    *pm10 = 0; // PPD42NS는 PM10 측정 불가
}

/// @brief      PPD42NS 먼지센서 핸들러 (외부 인터페이스)
/// @param      void
/// @return     void
/// @details    외부에서 호출 가능한 PPD42NS 센서 핸들러 인터페이스
void Hal_DustSensor_PPD42NS_Handler(void) {
    PPD42NS_Handler();
}

/// @brief      Hal Dust Sensor PPD42NS Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 PPD42NS 센서 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD42NS_Module_1ms_Control(void)
{
    // 1s마다 타이머 카운트 증가
    if (gu8DustSensorControlTimer < PPD42NS_CONTROL_TIME_PERIOD)
    {
        gu8DustSensorControlTimer++;
    }

    if (gu8PPD42NSTimer < PPD42NS_CAL_TIME_PERIOD)
    {
        gu8PPD42NSTimer++;
    }
}

/// @brief      Hal Dust Sensor PPD42NS Module in While Loop
/// @details    Basic Loop 의 While문 안에서 PPD42NS 센서 제어 한다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD42NS_Module_Control(void)
{
    if (gu8DustSensorControlTimer >= PPD42NS_CONTROL_TIME_PERIOD)
    {
        Hal_DustSensor_PPD42NS_Handler();
        gu8DustSensorControlTimer = 0;
    }

    if (gu8PPD42NSTimer >= PPD42NS_CAL_TIME_PERIOD)
    {
        gu8PPD42NSTimer = 0;
        PPD42NS_CalParticleConcent(); // 2초 주기로 호출
    }
    PPD42NS_ReadParticleData(); // 1msec 주기로 호출
}

// Getter 함수들
/// @brief      PPD42NS 볼륨 합계 값 반환
/// @param      void
/// @return     U16 볼륨 합계 값
/// @details    PPD42NS 센서에서 측정된 볼륨 합계 값을 반환
U16 Hal_DustSensor_PPD42NS_GetVolumeSum(void) {
    return gu16VolumeSum;
}

/// @brief      PPD42NS PM2.5 농도 값 반환
/// @param      void
/// @return     U16 PM2.5 농도 값 (볼륨 합계)
/// @details    PPD42NS 센서에서 측정된 PM2.5 농도 값을 반환
U16 Hal_DustSensor_PPD42NS_GetPM2_5(void) {
    return gu16VolumeSum;
}

/// @brief      PPD42NS 센서 에러 플래그 반환
/// @param      void
/// @return     U8 에러 플래그 (SET/CLEAR)
/// @details    PPD42NS 센서의 현재 에러 상태를 반환
U8 Hal_DustSensor_PPD42NS_GetErrorFlag(void) {
    return g_errorFlag;
}

// FUNCTION COMMENT"'*****************************************************
// Function Name : getParticleVolumeSum
// Function ID   : F-019-001-008
// Description   : 최종 합산 카운트값 가져오기
// Return Value  : 최종 합산 카운트값
// FUNCTION COMMENT END "'************************************************
U16 getParticleVolumeSum(void)
{
	return gu16VolumeSum;
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor PPD42NS Initialize Module
/// @details    PPD42NS 먼지센서 관련 모든 것을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DustSensor_PPD42NS_Module_Initialize(void)
{
#if (DUST_SENSOR_PPD42NS_USE == USE)
    Hal_DustSensor_PPD42NS_Init();             // PPD42NS 센서 초기화
#endif
}

#endif  // DUST_SENSOR_PPD42NS_USE == USE
