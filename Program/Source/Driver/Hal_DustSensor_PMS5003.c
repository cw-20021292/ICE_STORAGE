/// @file     Hal_DustSensor_PMS5003.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PMS5003 �������� ����̹� ���� ����
/// @details  PMS5003 �������� ��� ���� HAL ����̹� ����

#include "Global_Header.h"

#if (DUST_SENSOR_PMS5003_USE == USE)

// PMS5003 ���� ���� ���� ����
static volatile U8 s_packet_received_flag = 0;           // ���� �Ϸ� �÷���
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
// ���� ���� �������� ���� Ȯ�� �� ���� �� ����

// ���� ������ ���� ����
static U16 gu16PM1_0Finish = 0;
static U16 gu16PM2_5Finish = 0;
static U16 gu16PM10Finish = 0;

// ���� �÷���
static U8 g_errorFlag = CLEAR;

// Ÿ�̸� ����
static U8 gu8DustSensorControlTimer = 0;

// ���� �Լ� ���� ���� (���� ���Ͽ� ���� public���� ����)
void PMS5003_Init(void);
void PMS5003_Handler(void);
void PMS5003_RxParticleData(void);
void PMS5003_CalcParticleData(void);
void PMS5003_CheckError(void);
void PMS5003_CheckDataStream(void);
// ��ƿ��Ƽ �Լ� ���� ���� ����

// ��ƿ��Ƽ �Լ����� static���� ���ǵǾ� ���ο����� ���

// PMS5003 ���� ���� �Լ���
/// @brief      PMS5003 ���� �ʱ�ȭ �Լ�
/// @param      void
/// @return     void
/// @details    PMS5003 ������ �������� �ʱ�ȭ�ϰ� UART ���� ���۸� Ŭ����
void PMS5003_Init(void) {
    gu8UARTOK_F = 0;
    MEMSET( (void __FAR *)&gu8ParticleRxDataBuffer, 0x00, sizeof(gu8ParticleRxDataBuffer) );
}

/// @brief      PMS5003 ���� ���� �ڵ鷯
/// @param      void
/// @return     void
/// @details    PMS5003 ������ ������ ����, ���, ���� üũ�� ���������� ����
void PMS5003_Handler(void) {
    PMS5003_RxParticleData();   // �������� ������ ����
    PMS5003_CalcParticleData(); // �������� ������ ���
    PMS5003_CheckError();        // �������� ������ ���� üũ
    PMS5003_CheckDataStream();   // �������� ������ ��Ʈ�� ���� üũ
}

/// @brief      PMS5003 ���� ������ ���� �Լ�
/// @param      void
/// @return     void
/// @details    UART2�� ���� PMS5003 �����κ��� 32����Ʈ �����͸� ����
void PMS5003_RxParticleData(void) {
    MD_STATUS status = R_Config_UART2_Receive((U8*)gu8ParticleRxDataBuffer, 32);
    if (status != MD_OK) {
        // ���� ó��: ���� ����� ����
    }
}

/// @brief      PMS5003 �̼����� ������ ��� �Լ�
/// @param      void
/// @return     void
/// @details    ���ŵ� �����͸� �������� PM1.0, PM2.5, PM10 �󵵿� ���� ������ ���
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

        // ������ ������Ʈ
        gu16PM1_0Finish = gu16PM1p0Data;
        gu16PM2_5Finish = gu16PM2p5ConcentDisplayFinal;
        gu16PM10Finish = gu16PM10ConcentDisplayFinal;
    }
}

/// @brief      PMS5003 ���� ���� üũ �Լ�
/// @param      void
/// @return     void
/// @details    PMS5003 ������ ���� ���¸� üũ (���� �̱���)
void PMS5003_CheckError(void) {
    // �̱���
}

/// @brief      PMS5003 ���� �����ͽ�Ʈ�� üũ �Լ�
/// @param      void
/// @return     void
/// @details    PMS5003 ������ ������ ��Ʈ�� ���¸� üũ (���� �̱���)
void PMS5003_CheckDataStream(void) {
    // �̱���
}

// ���� �������̽� �Լ���
/// @brief      PMS5003 �������� �ʱ�ȭ
/// @param      void
/// @return     void
/// @details    PMS5003 ������ ���� �÷��׸� Ŭ�����ϰ� ���� �ʱ�ȭ �Լ� ȣ��
void Hal_DustSensor_PMS5003_Init(void) {
    g_errorFlag = CLEAR;
    PMS5003_Init();
}

/// @brief      PMS5003 �������� ���� ON
/// @param      void
/// @return     void
/// @details    PMS5003 ������ ������ ��� ���·� ����
void Hal_DustSensor_PMS5003_On(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_ON;
}

/// @brief      PMS5003 �������� ���� OFF
/// @param      void
/// @return     void
/// @details    PMS5003 ������ ������ ���� ���·� ����
void Hal_DustSensor_PMS5003_Off(void) {
    PORT_DEFINE_PARTICLE_SENSOR_POW = VALUE_PARTICLE_POWER_OFF;
}

/// @brief      PMS5003 �������� ������ �б�
/// @param      pm1_0 PM1.0 �� ������ ������
/// @param      pm2_5 PM2.5 �� ������ ������
/// @param      pm10 PM10 �� ������ ������
/// @param      error ���� ���� �÷��� ������
/// @return     void
/// @details    PMS5003 �������� ������ PM1.0, PM2.5, PM10 �� �����Ϳ� ���� ���¸� ��ȯ
void Hal_DustSensor_PMS5003_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error) {
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
    *pm1_0 = gu16PM1_0Finish;
    *pm2_5 = gu16PM2_5Finish;
    *pm10 = gu16PM10Finish;
}

/// @brief      PMS5003 �������� �ڵ鷯 (�ܺ� �������̽�)
/// @param      void
/// @return     void
/// @details    �ܺο��� ȣ�� ������ PMS5003 ���� �ڵ鷯 �������̽�
void Hal_DustSensor_PMS5003_Handler(void) {
    PMS5003_Handler();
}

/// @brief      Hal Dust Sensor PMS5003 Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� PMS5003 ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_DustSensor_PMS5003_Module_1ms_Control(void)
{
    // 1s���� Ÿ�̸� ī��Ʈ ����
    if (gu8DustSensorControlTimer < PMS5003_CONTROL_TIME_PERIOD)
    {
        gu8DustSensorControlTimer++;
    }
}

/// @brief      Hal Dust Sensor PMS5003 Module in While Loop
/// @details    Basic Loop �� While�� �ȿ��� PMS5003 ���� ���� �Ѵ�
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

// Getter �Լ���
/// @brief      PMS5003 PM1.0 �� �� ��ȯ
/// @param      void
/// @return     U16 PM1.0 �� �� (��g/m��)
/// @details    PMS5003 �������� ������ PM1.0 �� ���� ��ȯ
U16 Hal_DustSensor_PMS5003_GetPM1_0(void) {
    return gu16PM1_0Finish;
}

/// @brief      PMS5003 PM2.5 �� �� ��ȯ
/// @param      void
/// @return     U16 PM2.5 �� �� (��g/m��)
/// @details    PMS5003 �������� ������ PM2.5 �� ���� ��ȯ
U16 Hal_DustSensor_PMS5003_GetPM2_5(void) {
    return gu16PM2_5Finish;
}

/// @brief      PMS5003 PM10 �� �� ��ȯ
/// @param      void
/// @return     U16 PM10 �� �� (��g/m��)
/// @details    PMS5003 �������� ������ PM10 �� ���� ��ȯ
U16 Hal_DustSensor_PMS5003_GetPM10(void) {
    return gu16PM10Finish;
}

/// @brief      PMS5003 ���� ���� �÷��� ��ȯ
/// @param      void
/// @return     U8 ���� �÷��� (SET/CLEAR)
/// @details    PMS5003 ������ ���� ���� ���¸� ��ȯ
U8 Hal_DustSensor_PMS5003_GetErrorFlag(void) {
    return g_errorFlag;
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor PMS5003 Initialize Module
/// @details    PMS5003 �������� ���� ��� ���� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_DustSensor_PMS5003_Module_Initialize(void)
{
#if (DUST_SENSOR_PMS5003_USE == USE)
    Hal_DustSensor_PMS5003_Init();             // PMS5003 ���� �ʱ�ȭ
#endif
}

#endif  // DUST_SENSOR_PMS5003_USE == USE
