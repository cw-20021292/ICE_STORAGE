/// @file     Hal_DustSensor_PPD4260.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD4260 �������� ����̹� ���� ����
/// @details  PPD4260 �������� ��� ���� HAL ����̹� ����

#include "Global_Header.h"

#if (DUST_SENSOR_PPD4260_USE == USE)

// PPD4260 ���� ���� ���� ����
static U16 gu16VolumeP1 = 0;
static U16 gu16VolumeP2 = 0;
static U16 gu16SkipSensing = 0;
static D64 gd64CalAvgPM2p5 = 0;
static D64 gd64CalAvgPM10 = 0;

// ���� �÷���
static U8 g_errorFlag = CLEAR;

// Ÿ�̸� ����
static U8 gu8DustSensorControlTimer = 0;

// ���� �Լ� ���� ���� (���� ���Ͽ� ���� public���� ����)
void PPD4260_Init(void);
void PPD4260_Handler(void);
void PPD4260_ReadParticleData(void);
void PPD4260_CalParticleConcent(void);

// PPD4260 ���� ���� �Լ���
/// @brief      PPD4260 ���� �ʱ�ȭ �Լ�
/// @param      void
/// @return     void
/// @details    PPD4260 ������ �������� �ʱ�ȭ
void PPD4260_Init(void) {
    gu16VolumeP1 = 0;
    gu16VolumeP2 = 0;
    gu16SkipSensing = 0;
    gd64CalAvgPM2p5 = 0;
    gd64CalAvgPM10 = 0;
}

/// @brief      PPD4260 ���� ���� �ڵ鷯
/// @param      void
/// @return     void
/// @details    PPD4260 ������ ������ ���Ű� ����� ���������� ����
void PPD4260_Handler(void) {
    PPD4260_ReadParticleData();
    PPD4260_CalParticleConcent();
    // PPD4260_CheckParticleDataError();
}

/// @brief      PPD4260 ���� ������ ���� �Լ�
/// @param      void
/// @return     void
/// @details    PPD4260 ������ P1, P2 ��Ʈ�κ��� ������ ��ȣ�� ����
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

/// @brief      PPD4260 �̼����� �� ��� �Լ�
/// @param      void
/// @return     void
/// @details    ���ŵ� ���� �����͸� �������� PM2.5�� PM10 �󵵸� ���
void PPD4260_CalParticleConcent(void) {
    // PPD4260�� �� ���
    D64 d64Temp = 0;

    // PM2.5 ���
    d64Temp = (D64)gu16VolumeP1 * 1000.0 / 30000.0;
    gd64CalAvgPM2p5 = d64Temp;

    // PM10 ���
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

// ���� �������̽� �Լ���
/// @brief      PPD4260 �������� �ʱ�ȭ
/// @param      void
/// @return     void
/// @details    PPD4260 ������ ���� �÷��׸� Ŭ�����ϰ� ���� �ʱ�ȭ �Լ� ȣ��
void Hal_DustSensor_PPD4260_Init(void) {
    g_errorFlag = CLEAR;
    PPD4260_Init();
}

/// @brief      PPD4260 �������� ���� ON
/// @param      void
/// @return     void
/// @details    PPD4260 ������ ������ ��� ���·� ����
void Hal_DustSensor_PPD4260_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

/// @brief      PPD4260 �������� ���� OFF
/// @param      void
/// @return     void
/// @details    PPD4260 ������ ������ ���� ���·� ����
void Hal_DustSensor_PPD4260_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

/// @brief      PPD4260 �������� ������ �б�
/// @param      pm1_0 PM1.0 �� ������ ������ (���� ��������)
/// @param      pm2_5 PM2.5 �� ������ ������
/// @param      pm10 PM10 �� ������ ������
/// @param      error ���� ���� �÷��� ������
/// @return     void
/// @details    PPD4260 �������� ������ PM2.5, PM10 �� �����Ϳ� ���� ���¸� ��ȯ
void Hal_DustSensor_PPD4260_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
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
    *pm1_0 = 0; // PPD4260�� PM1.0 ���� �Ұ�
    *pm2_5 = (U16)gd64CalAvgPM2p5;
    *pm10 = (U16)gd64CalAvgPM10;
}

/// @brief      PPD4260 �������� �ڵ鷯 (�ܺ� �������̽�)
/// @param      void
/// @return     void
/// @details    �ܺο��� ȣ�� ������ PPD4260 ���� �ڵ鷯 �������̽�
void Hal_DustSensor_PPD4260_Handler(void) {
    PPD4260_Handler();
}

/// @brief      Hal Dust Sensor PPD4260 Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� PPD4260 ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_DustSensor_PPD4260_Module_1ms_Control(void)
{
    // 1s���� Ÿ�̸� ī��Ʈ ����
    if (gu8DustSensorControlTimer < PPD4260_CONTROL_TIME_PERIOD)
    {
        gu8DustSensorControlTimer++;
    }
}

/// @brief      Hal Dust Sensor PPD4260 Module in While Loop
/// @details    Basic Loop �� While�� �ȿ��� PPD4260 ���� ���� �Ѵ�
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

// Getter �Լ���
/// @brief      PPD4260 PM2.5 �� �� ��ȯ
/// @param      void
/// @return     U16 PM2.5 �� �� (��g/m��)
/// @details    PPD4260 �������� ������ PM2.5 �� ���� ��ȯ
U16 Hal_DustSensor_PPD4260_GetPM2_5(void) {
    return (U16)gd64CalAvgPM2p5;
}

/// @brief      PPD4260 PM10 �� �� ��ȯ
/// @param      void
/// @return     U16 PM10 �� �� (��g/m��)
/// @details    PPD4260 �������� ������ PM10 �� ���� ��ȯ
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
/// @details    PPD4260 �������� ���� ��� ���� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_DustSensor_PPD4260_Module_Initialize(void)
{
#if (DUST_SENSOR_PPD4260_USE == USE)
    Hal_DustSensor_PPD4260_Init();             // PPD4260 ���� �ʱ�ȭ
#endif
}

#endif  // DUST_SENSOR_PPD4260_USE == USE
