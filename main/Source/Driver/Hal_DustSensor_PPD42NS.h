#ifndef _HAL_DUSTSENSOR_PPD42NS_H_
#define _HAL_DUSTSENSOR_PPD42NS_H_

/// @file     Hal_DustSensor_PPD42NS.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PPD42NS �������� ����̹� ��� ����
/// @details  PPD42NS �������� ��� ���� HAL ����̹� ���

// Include standard header for type definitions
#define DUST_SENSOR_PPD42NS_USE USE

/*  PPD42NS �������� ���� Function ����
    => Hal_DustSensor_PPD42NS_Init();                        // PPD42NS ���� �ʱ�ȭ
    => Hal_DustSensor_PPD42NS_On();                          // PPD42NS ���� ���� ON
    => Hal_DustSensor_PPD42NS_Off();                         // PPD42NS ���� ���� OFF
    => Hal_DustSensor_PPD42NS_Handler();                     // PPD42NS ���� ������ ó�� (�ֱ��� ȣ��)
    => Hal_DustSensor_PPD42NS_ReadData(&pm1_0, &pm2_5, &pm10, &error); // ���� ������ �б�
    => Hal_DustSensor_PPD42NS_GetPM2_5();                    // PM2.5 �� �б�
    => Hal_DustSensor_PPD42NS_GetVolumeSum();                // �����հ� �� �б�
    => getParticleVolumeSum();                               // ���� �����հ� �б�

    ��� ��)
    Hal_DustSensor_PPD42NS_Module_Initialize();              // �ý��� �ʱ�ȭ �� ȣ��
    Hal_DustSensor_PPD42NS_On();                             // ���� ���� ON
    
    while(1) {
        Hal_DustSensor_PPD42NS_Module_Control();             // ���� �������� ȣ��
        
        // ���� �� �б�
        U16 pm2_5 = Hal_DustSensor_PPD42NS_GetPM2_5();
        U16 volume_sum = Hal_DustSensor_PPD42NS_GetVolumeSum();
        U8 error = Hal_DustSensor_PPD42NS_GetErrorFlag();
        
        // ��ƿ��Ƽ �Լ� ���
        U16 particle_volume = getParticleVolumeSum();
    }

    ���ǻ���)
    - DUST_SENSOR_PPD42NS_USE�� USE�� �����Ǿ�� ��
    - ���� ���� ON �� ����ȭ �ð�(1��) �ʿ�
    - �Ƴ��α� �Է� ��� (P9_bit.no4)
    - �����հ� ��� �ֱ�: 2��
    - ���� üũ �ð�: 5��
    - ���� ī��Ʈ �ð�: 5ms
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

// PPD42NS ���� ��Ʈ ����
#define PORT_DEFINE_PARTICLE_IN                         P9_bit.no4

// PPD42NS ���� ����
#define PPD42NS_CONTROL_TIME_PERIOD    1000    // 1�� �ֱ�
#define PPD42NS_CAL_TIME_PERIOD        2000    // 2�� �ֱ�
#define PARTICLE_INITTIME            1000     // �ʱ�ȭ �ð� (���� ������ ����)
#define PARTICLE_ERRORTIME           5000     // ���� �ð� (���� ������ ����)
#define PPD42NS_PARTICLE_COUNT_TIME          5
#define PPD42NS_PARTICLE_VOLUMESUM_TIME      2000
#define PPD42NS_PARTICLE_ERROR_CHECK_TIME    5

///////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////

// �ʱ�ȭ �� ���� �Լ�
void Hal_DustSensor_PPD42NS_Init(void);
void Hal_DustSensor_PPD42NS_On(void);
void Hal_DustSensor_PPD42NS_Off(void);
void Hal_DustSensor_PPD42NS_Handler(void);

// ������ �б� �Լ�
void Hal_DustSensor_PPD42NS_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);

// ���� ���� ó�� �Լ��� static���� ���ǵǾ� �ܺο��� ���� �Ұ�

// Ÿ�̸� ���� �Լ�
void Hal_DustSensor_PPD42NS_ControlTimer(void);
void Hal_DustSensor_PPD42NS_Module_Control(void);

// ������ getter �Լ�
U16 Hal_DustSensor_PPD42NS_GetVolumeSum(void);
U16 Hal_DustSensor_PPD42NS_GetPM2_5(void);
U8 Hal_DustSensor_PPD42NS_GetErrorFlag(void);

// ��ƿ��Ƽ �Լ�
U16 getParticleVolumeSum(void);

// Module Interface Functions
void Hal_DustSensor_PPD42NS_Module_Initialize(void);
void Hal_DustSensor_PPD42NS_Module_1ms_Control(void);

#endif // _HAL_DUSTSENSOR_PPD42NS_H_
