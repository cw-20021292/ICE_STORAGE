#ifndef _HAL_DUSTSENSOR_PMS5003_H_
#define _HAL_DUSTSENSOR_PMS5003_H_

/// @file     Hal_DustSensor_PMS5003.h
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    PMS5003 �������� ����̹� ��� ����
/// @details  PMS5003 �������� ��� ���� HAL ����̹� ���

// Include standard header for type definitions
#define DUST_SENSOR_PMS5003_USE USE

/*  PMS5003 �������� ���� Function ����
    => Hal_DustSensor_PMS5003_Init();                        // PMS5003 ���� �ʱ�ȭ
    => Hal_DustSensor_PMS5003_On();                          // PMS5003 ���� ���� ON
    => Hal_DustSensor_PMS5003_Off();                         // PMS5003 ���� ���� OFF
    => Hal_DustSensor_PMS5003_Handler();                     // PMS5003 ���� ������ ó�� (�ֱ��� ȣ��)
    => Hal_DustSensor_PMS5003_ReadData(&pm1_0, &pm2_5, &pm10, &error); // ���� ������ �б�
    => Hal_DustSensor_PMS5003_GetPM1_0();                    // PM1.0 �� �б�
    => Hal_DustSensor_PMS5003_GetPM2_5();                    // PM2.5 �� �б�
    => Hal_DustSensor_PMS5003_GetPM10();                     // PM10 �� �б�

    ��� ��)
    Hal_DustSensor_PMS5003_Module_Initialize();              // �ý��� �ʱ�ȭ �� ȣ��
    Hal_DustSensor_PMS5003_On();                             // ���� ���� ON
    
    while(1) {
        Hal_DustSensor_PMS5003_Module_Control();             // ���� �������� ȣ��
        
        // ���� �� �б�
        U16 pm1_0 = Hal_DustSensor_PMS5003_GetPM1_0();
        U16 pm2_5 = Hal_DustSensor_PMS5003_GetPM2_5();
        U16 pm10 = Hal_DustSensor_PMS5003_GetPM10();
        U8 error = Hal_DustSensor_PMS5003_GetErrorFlag();
    }

    ���ǻ���)
    - DUST_SENSOR_PMS5003_USE�� USE�� �����Ǿ�� ��
    - ���� ���� ON �� ����ȭ �ð�(10��) �ʿ�
    - UART ����� ����ϹǷ� UART ������ �Ϸ�Ǿ�� ��
    - Start Byte: 0x42, 0x4D (�������)
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

// PMS5003 ���� ����
#define PMS5003_CONTROL_TIME_PERIOD    1000    // 1�� �ֱ�
#define PMS5003_DATA_LENGTH          32
#define PMS5003_START_BYTE1          0x42
#define PMS5003_START_BYTE2          0x4d
#define PMS5003_BAUDRATE             9600
#define PMS5003_DATASTREAM_CHECKTIME 10

// PMS5003 �ΰ��� Ÿ�� ���� (Ȯ��� ����)
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

// �ʱ�ȭ �� ���� �Լ�
void Hal_DustSensor_PMS5003_Init(void);
void Hal_DustSensor_PMS5003_On(void);
void Hal_DustSensor_PMS5003_Off(void);
void Hal_DustSensor_PMS5003_Handler(void);

// ������ �б� �Լ�
void Hal_DustSensor_PMS5003_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);

// ���� ���� ó�� �Լ��� static���� ���ǵǾ� �ܺο��� ���� �Ұ�

// Ÿ�̸� ���� �Լ�
void Hal_DustSensor_PMS5003_ControlTimer(void);
void Hal_DustSensor_PMS5003_Module_Control(void);

// ������ getter �Լ�
U16 Hal_DustSensor_PMS5003_GetPM1_0(void);
U16 Hal_DustSensor_PMS5003_GetPM2_5(void);
U16 Hal_DustSensor_PMS5003_GetPM10(void);
U8 Hal_DustSensor_PMS5003_GetErrorFlag(void);

// Module Interface Functions
void Hal_DustSensor_PMS5003_Module_Initialize(void);
void Hal_DustSensor_PMS5003_Module_1ms_Control(void);

#endif // _HAL_DUSTSENSOR_PMS5003_H_
