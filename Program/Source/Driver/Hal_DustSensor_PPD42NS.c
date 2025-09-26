/// @file     Hal_DustSensor_PPD42NS.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD42NS �������� ����̹� ���� ����
/// @details  PPD42NS �������� ��� ���� HAL ����̹� ����

#include "Global_Header.h"

#if (DUST_SENSOR_PPD42NS_USE == USE)

// PPD42NS ���� ���� ���� ����
static U16 gu16Volume = 0;
static U16 gu16VolumeBuf[5] = {0};
static U16 gu16VolumeSum = 0;
static U16 gu16SkipSensing = 0;
static U16 gu16ParticleInitTime = PARTICLE_INITTIME;
static U16 gu16ErrParticle = PARTICLE_ERRORTIME;
static U8 gu8PPD42NSTimer = 0;

// ���� �÷���
static U8 g_errorFlag = CLEAR;

// Ÿ�̸� ����
static U8 gu8DustSensorControlTimer = 0;

// ���� �Լ� ���� ���� (���� ���Ͽ� ���� public���� ����)
void PPD42NS_Init(void);
void PPD42NS_Handler(void);
void PPD42NS_ReadParticleData(void);
void PPD42NS_CalParticleConcent(void);
void PPD42NS_CheckParticleDataError(void);
void initParticleSensorVar(void);

// ��ƿ��Ƽ �Լ�
/// @brief      PPD42NS ���� ���� �ʱ�ȭ ��ƿ��Ƽ �Լ�
/// @param      void
/// @return     void
/// @details    PPD42NS ������ ���� �������� �ʱ�ȭ
void initParticleSensorVar(void) {
    gu16Volume = 0;
    gu16VolumeSum = 0;
    gu16SkipSensing = 0;
    gu16ParticleInitTime = PARTICLE_INITTIME;
    gu16ErrParticle = PARTICLE_ERRORTIME;
    MEMSET( (void __FAR *)&gu16VolumeBuf, 0x00, sizeof(gu16VolumeBuf) );
}

// PPD42NS ���� ���� �Լ���
/// @brief      PPD42NS ���� �ʱ�ȭ �Լ�
/// @param      void
/// @return     void
/// @details    PPD42NS ������ �������� �ʱ�ȭ�ϰ� ���� ���۸� Ŭ����
void PPD42NS_Init(void) {
    gu16Volume = 0;
    gu16VolumeSum = 0;
    gu16SkipSensing = 0;
    gu16ParticleInitTime = PARTICLE_INITTIME;
    gu16ErrParticle = PARTICLE_ERRORTIME;
    MEMSET( (void __FAR *)&gu16VolumeBuf, 0x00, sizeof(gu16VolumeBuf) );
}

/// @brief      PPD42NS ���� ���� �ڵ鷯
/// @param      void
/// @return     void
/// @details    PPD42NS ������ ������ ����, ���, ���� üũ�� ���������� ����
void PPD42NS_Handler(void) {
    PPD42NS_ReadParticleData();
    PPD42NS_CalParticleConcent();
    PPD42NS_CheckParticleDataError();
}

/// @brief      PPD42NS ���� ������ ���� �Լ�
/// @param      void
/// @return     void
/// @details    PPD42NS ������ ������ ��Ʈ ���¸� �о� �̼����� ���� �����͸� ����
void PPD42NS_ReadParticleData(void) {
	static U8 mu8PoweroffInit_F = 0;

    // 1mSec �ֱ�� ȣ��
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

/// @brief      PPD42NS �̼����� �� ��� �Լ�
/// @param      void
/// @return     void
/// @details    ������ ���� �����͸� �������� ��� �̼����� �󵵸� ���
void PPD42NS_CalParticleConcent(void) {
    static U8 mu8index = 0;
	static U8 mu8BufFull_F = 0;
	U8 mu8tmp = 0;

    // 2�� �ֱ�� ȣ��
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

	// �ִ밪 ����
	if(gu16Volume > 250) gu16Volume = 250;

	gu16VolumeBuf[mu8index] = gu16Volume;
	gu16Volume = 0;
	gu16VolumeSum = 0;
	mu8index++;

	if(mu8BufFull_F)	// Initial time ���� ��, ���ۿ� ���� ������ á���� Ȯ��
	{
		for(mu8tmp = 0; mu8tmp < 5; mu8tmp++)
		{
			gu16VolumeSum += gu16VolumeBuf[mu8tmp];
		}
	}
	else	// ���ۿ� ���� ������ ���� �ʾҴٸ�, ���ۿ� �����ϴ� ���� ����� ���� ��, �� ��հ��� ����Ͽ� gu16VolumeSum�� Ȯ��
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

/// @brief      PPD42NS ���� ������ ���� üũ �Լ�
/// @param      void
/// @return     void
/// @details    ���� ������ ���� ������ üũ�Ͽ� ��ü ���� �÷��׸� ����
void PPD42NS_CheckParticleDataError(void) {
    if(gu16ErrParticle) {
        g_errorFlag = SET;
    } else {
        g_errorFlag = CLEAR;
    }
}

// ���� �������̽� �Լ���
/// @brief      PPD42NS �������� �ʱ�ȭ
/// @param      void
/// @return     void
/// @details    PPD42NS ������ ���� �÷��׸� Ŭ�����ϰ� ���� �ʱ�ȭ �Լ� ȣ��
void Hal_DustSensor_PPD42NS_Init(void) {
    g_errorFlag = CLEAR;
    PPD42NS_Init();
}

/// @brief      PPD42NS �������� ���� ON
/// @param      void
/// @return     void
/// @details    PPD42NS ������ ������ ��� ���·� ����
void Hal_DustSensor_PPD42NS_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

/// @brief      PPD42NS �������� ���� OFF
/// @param      void
/// @return     void
/// @details    PPD42NS ������ ������ ���� ���·� ����
void Hal_DustSensor_PPD42NS_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

/// @brief      PPD42NS �������� ������ �б�
/// @param      pm1_0 PM1.0 �� ������ ������ (���� ��������)
/// @param      pm2_5 PM2.5 �� ������ ������
/// @param      pm10 PM10 �� ������ ������ (���� ��������)
/// @param      error ���� ���� �÷��� ������
/// @return     void
/// @details    PPD42NS �������� ������ PM2.5 �� �����Ϳ� ���� ���¸� ��ȯ
void Hal_DustSensor_PPD42NS_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
    // NULL ������ üũ
    if (!pm1_0 || !pm2_5 || !pm10 || !error) return;

    // ���� ���� ����
    *error = g_errorFlag;

    // ���� ������ ��ȿ�� üũ
    if (g_errorFlag != CLEAR) {
        *pm1_0 = 0;
        *pm2_5 = 0;
        *pm10 = 0;
        return;
    }

    // ���� ������ �Ҵ�
    *pm1_0 = 0; // PPD42NS�� PM1.0 ���� �Ұ�
    *pm2_5 = gu16VolumeSum;
    *pm10 = 0; // PPD42NS�� PM10 ���� �Ұ�
}

/// @brief      PPD42NS �������� �ڵ鷯 (�ܺ� �������̽�)
/// @param      void
/// @return     void
/// @details    �ܺο��� ȣ�� ������ PPD42NS ���� �ڵ鷯 �������̽�
void Hal_DustSensor_PPD42NS_Handler(void) {
    PPD42NS_Handler();
}

/// @brief      Hal Dust Sensor PPD42NS Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� PPD42NS ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_DustSensor_PPD42NS_Module_1ms_Control(void)
{
    // 1s���� Ÿ�̸� ī��Ʈ ����
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
/// @details    Basic Loop �� While�� �ȿ��� PPD42NS ���� ���� �Ѵ�
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
        PPD42NS_CalParticleConcent(); // 2�� �ֱ�� ȣ��
    }
    PPD42NS_ReadParticleData(); // 1msec �ֱ�� ȣ��
}

// Getter �Լ���
/// @brief      PPD42NS ���� �հ� �� ��ȯ
/// @param      void
/// @return     U16 ���� �հ� ��
/// @details    PPD42NS �������� ������ ���� �հ� ���� ��ȯ
U16 Hal_DustSensor_PPD42NS_GetVolumeSum(void) {
    return gu16VolumeSum;
}

/// @brief      PPD42NS PM2.5 �� �� ��ȯ
/// @param      void
/// @return     U16 PM2.5 �� �� (���� �հ�)
/// @details    PPD42NS �������� ������ PM2.5 �� ���� ��ȯ
U16 Hal_DustSensor_PPD42NS_GetPM2_5(void) {
    return gu16VolumeSum;
}

/// @brief      PPD42NS ���� ���� �÷��� ��ȯ
/// @param      void
/// @return     U8 ���� �÷��� (SET/CLEAR)
/// @details    PPD42NS ������ ���� ���� ���¸� ��ȯ
U8 Hal_DustSensor_PPD42NS_GetErrorFlag(void) {
    return g_errorFlag;
}

// FUNCTION COMMENT"'*****************************************************
// Function Name : getParticleVolumeSum
// Function ID   : F-019-001-008
// Description   : ���� �ջ� ī��Ʈ�� ��������
// Return Value  : ���� �ջ� ī��Ʈ��
// FUNCTION COMMENT END "'************************************************
U16 getParticleVolumeSum(void)
{
	return gu16VolumeSum;
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor PPD42NS Initialize Module
/// @details    PPD42NS �������� ���� ��� ���� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_DustSensor_PPD42NS_Module_Initialize(void)
{
#if (DUST_SENSOR_PPD42NS_USE == USE)
    Hal_DustSensor_PPD42NS_Init();             // PPD42NS ���� �ʱ�ȭ
#endif
}

#endif  // DUST_SENSOR_PPD42NS_USE == USE
