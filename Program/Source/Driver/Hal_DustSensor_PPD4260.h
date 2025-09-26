#ifndef HAL_DUSTSENSOR_PPD4260_H
#define HAL_DUSTSENSOR_PPD4260_H

/// @file     Hal_DustSensor_PPD4260.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD4260 �������� ����̹� ��� ����
/// @details  PPD4260 �������� ��� ���� HAL ����̹� ���

// Include standard header for type definitions
#define DUST_SENSOR_PPD4260_USE USE

/*  PPD4260 �������� ���� Function ����
    => Hal_DustSensor_PPD4260_Init();                        // PPD4260 ���� �ʱ�ȭ
    => Hal_DustSensor_PPD4260_On();                          // PPD4260 ���� ���� ON
    => Hal_DustSensor_PPD4260_Off();                         // PPD4260 ���� ���� OFF
    => Hal_DustSensor_PPD4260_Handler();                     // PPD4260 ���� ������ ó�� (�ֱ��� ȣ��)
    => Hal_DustSensor_PPD4260_ReadData(&pm1_0, &pm2_5, &pm10, &error); // ���� ������ �б�
    => Hal_DustSensor_PPD4260_GetPM2_5();                    // PM2.5 �� �б�
    => Hal_DustSensor_PPD4260_GetPM10();                     // PM10 �� �б�
    => Hal_DustSensor_PPD4260_GetPM2_5Avg();                 // PM2.5 ��հ� �б�
    => Hal_DustSensor_PPD4260_GetPM10Avg();                  // PM10 ��հ� �б�

    ��� ��)
    Hal_DustSensor_PPD4260_Module_Initialize();              // �ý��� �ʱ�ȭ �� ȣ��
    Hal_DustSensor_PPD4260_On();                             // ���� ���� ON
    
    while(1) {
        Hal_DustSensor_PPD4260_Module_Control();             // ���� �������� ȣ��
        
        // ���� �� �б� (��ð�)
        U16 pm2_5 = Hal_DustSensor_PPD4260_GetPM2_5();
        U16 pm10 = Hal_DustSensor_PPD4260_GetPM10();
        
        // ���� �� �б� (��հ�)
        D64 pm2_5_avg = Hal_DustSensor_PPD4260_GetPM2_5Avg();
        D64 pm10_avg = Hal_DustSensor_PPD4260_GetPM10Avg();
        
        U8 error = Hal_DustSensor_PPD4260_GetErrorFlag();
    }

    ���ǻ���)
    - DUST_SENSOR_PPD4260_USE�� USE�� �����Ǿ�� ��
    - ���� ���� ON �� ����ȭ �ð�(1��) �ʿ�
    - �Ƴ��α� �����̹Ƿ� ADC ������ �Ϸ�Ǿ�� ��
    - �� ��� �� ��հ� ó�� ��� ����
    - ���� üũ �ð�: 5��
*/

///////////////////////////////////////////////////////////////////////
/// Macro Definitions
//////////////////////////////////////////////////////////////////////

// ���� ��Ʈ �� �� ����
#define PORT_DEFINE_PARTICLE_SENSOR_POW                 P5_bit.no0  // ���� ���� ���� Port ���� : P50
#define VALUE_PARTICLE_POWER_ON                         N_ON
#define VALUE_PARTICLE_POWER_OFF                        N_OFF
#define VALUE_PARTICLE_INITTIME                         10000
#define VALUE_PARTICLE_ERRORTIME                        10000

// PPD4260 ���� ����
#define PPD4260_CONTROL_TIME_PERIOD    1000    // 1�� �ֱ�
#define PARTICLE_INITTIME            1000     // �ʱ�ȭ �ð�
#define PARTICLE_ERRORTIME           5000     // ���� �ð�
#define PPD4260_PARTICLE_COUNT_TIME          1
#define PPD4260_PARTICLE_CONCENT_TIME        1000
#define PPD4260_PARTICLE_ERROR_CHECK_TIME    1

///////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////

// �ʱ�ȭ �� ���� �Լ�
void Hal_DustSensor_PPD4260_Init(void);
void Hal_DustSensor_PPD4260_On(void);
void Hal_DustSensor_PPD4260_Off(void);
void Hal_DustSensor_PPD4260_Handler(void);

// ������ �б� �Լ�
void Hal_DustSensor_PPD4260_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);

// ���� ���� ó�� �Լ��� static���� ���ǵǾ� �ܺο��� ���� �Ұ�

// Ÿ�̸� ���� �Լ�
void Hal_DustSensor_PPD4260_ControlTimer(void);
void Hal_DustSensor_PPD4260_Module_Control(void);

// ������ getter �Լ�
U16 Hal_DustSensor_PPD4260_GetPM2_5(void);
U16 Hal_DustSensor_PPD4260_GetPM10(void);
D64 Hal_DustSensor_PPD4260_GetPM2_5Avg(void);
D64 Hal_DustSensor_PPD4260_GetPM10Avg(void);
U8 Hal_DustSensor_PPD4260_GetErrorFlag(void);

// Module Interface Functions
void Hal_DustSensor_PPD4260_Module_Initialize(void);
void Hal_DustSensor_PPD4260_Module_1ms_Control(void);

#endif // HAL_DUSTSENSOR_PPD4260_H
