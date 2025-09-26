#ifndef HAL_DUSTSENSOR_PPD71_H
#define HAL_DUSTSENSOR_PPD71_H

/*'"FILE COMMENT"'***************************************************
@ FILE Name   : Hal_DustSensor_PPD71.h
@ FILE ID     : F-000
@ Programmer  : AUTO GENERATED
@ Description : PPD71 �������� ����̹� ��� ����
"FILE COMMENT END "'*************************************************/

// Include standard header for type definitions
#define DUST_SENSOR_PPD71_USE USE

/*  PPD71 �������� ���� Function ����
    => Hal_DustSensor_PPD71_Init();                          // PPD71 ���� �ʱ�ȭ
    => Hal_DustSensor_PPD71_On();                            // PPD71 ���� ���� ON
    => Hal_DustSensor_PPD71_Off();                           // PPD71 ���� ���� OFF
    => Hal_DustSensor_PPD71_Handler();                       // PPD71 ���� ������ ó�� (�ֱ��� ȣ��)
    => Hal_DustSensor_PPD71_ReadData(&pm1_0, &pm2_5, &pm10, &error); // ���� ������ �б�
    => Hal_DustSensor_PPD71_SetSensitivity(�ΰ���);          // ���� �ΰ��� ����
    => Hal_DustSensor_PPD71_GetPM2_5();                      // PM2.5 �� �б�
    => Hal_DustSensor_PPD71_GetPM10();                       // PM10 �� �б�

    ��� ��)
    Hal_DustSensor_PPD71_Module_Initialize();                // �ý��� �ʱ�ȭ �� ȣ��
    Hal_DustSensor_PPD71_On();                               // ���� ���� ON
    
    // �ΰ��� ���� (���û���)
    Hal_DustSensor_PPD71_SetSensitivity(PPD71_SENSITIVITY_NORMAL);
    
    while(1) {
        Hal_DustSensor_PPD71_Module_Control();               // ���� �������� ȣ��
        
        // ���� �� �б�
        U16 pm2_5 = Hal_DustSensor_PPD71_GetPM2_5();
        U16 pm10 = Hal_DustSensor_PPD71_GetPM10();
        U8 error = Hal_DustSensor_PPD71_GetErrorFlag();
    }

    ���ǻ���)
    - DUST_SENSOR_PPD71_USE�� USE�� �����Ǿ�� ��
    - ���� ���� ON �� ����ȭ �ð�(10��) �ʿ�
    - UART ����� ����ϹǷ� UART ������ �Ϸ�Ǿ�� ��
    - �ΰ���: PPD71_SENSITIVITY_SHARP(����), NORMAL(����), DULL(����)
*/

// PPD71 �ΰ��� ���� Ÿ�� ����
typedef enum
{
    PPD71_SENSITIVITY_SHARP,    // ���� ���� ����
    PPD71_SENSITIVITY_NORMAL,   // ���� ���� ����
    PPD71_SENSITIVITY_DULL      // ���� ���� ����
} PPD71_SENSITIVITY_T;

///////////////////////////////////////////////////////////////////////
/// Macro Definitions
//////////////////////////////////////////////////////////////////////

// ���� ��Ʈ �� �� ����
#define PORT_DEFINE_PARTICLE_SENSOR_POW                 P5_bit.no0  // ���� ���� ���� Port ���� : P50
#define VALUE_PARTICLE_POWER_ON                         N_ON
#define VALUE_PARTICLE_POWER_OFF                        N_OFF
#define VALUE_PARTICLE_INITTIME                         10000
#define VALUE_PARTICLE_ERRORTIME                        10000

// PPD71 ���� ����
#define PPD71_CONTROL_TIME_PERIOD         1000    // 1�� �ֱ�
#define PPD71_DATASTREAM_CHECKTIME        30      // ������ ��Ʈ�� üũ �ð�
#define PPD71_DATA_LENGTH            32
#define PPD71_START_BYTE             0x42
#define PPD71_BAUDRATE               9600
#define RXD_STX 0x02
#define DATA_LENGTH 29

// ���� ��� ���� (������ �� ���⼺ ������) - ���� ������ ����
#define REF_COEFFICIENT_2 0.0021f // ���� ������ ���� ��� (2�� ���׽� ���)
#define REF_COEFFICIENT_1 -0.3221f // ���� ������ ���� ��� (1�� ���׽� ���)
#define REF_COEFFICIENT_0 4.3636f // ���� ������ ���� ��� (�����)
#define REF_AREA_MAX 140.f // ���� ������ ���� ���� �ִ밪(���Ѽ�)
#define REF_AREA_MIN 15.f // ���� ������ ���� ���� �ּҰ�(���Ѽ�)

// �ΰ��� ���� ��� ���� - ���� ������ ����
#define SEN_COEFFICIENT_2 -0.0009f // ���� ������ ���� ��� (2�� ���׽� ���)
#define SEN_COEFFICIENT_1 0.1787f // ���� ������ ���� ��� (1�� ���׽� ���)
#define SEN_COEFFICIENT_0 1.286f // ���� ������ ���� ��� (�����)
#define SEN_AREA_MAX 200.f // ���� ������ ���� ���� �ִ밪(���Ѽ�)
#define SEN_AREA_MIN 0.f // ���� ������ ���� ���� �ּҰ�(���Ѽ�)

///////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////

// �ʱ�ȭ �� ���� �Լ�
void Hal_DustSensor_PPD71_Init(void);
void Hal_DustSensor_PPD71_On(void);
void Hal_DustSensor_PPD71_Off(void);
void Hal_DustSensor_PPD71_Handler(void);

// ������ �б� �Լ�
void Hal_DustSensor_PPD71_ReadData(U16* pm1_0, U16* pm2_5, U16* pm10, U8* error);

// �ΰ��� ���� �Լ�
void Hal_DustSensor_PPD71_SetSensitivity(PPD71_SENSITIVITY_T sensitivity);
PPD71_SENSITIVITY_T Hal_DustSensor_PPD71_GetSensitivity(void);

// ���� ���� ó�� �Լ��� static���� ���ǵǾ� �ܺο��� ���� �Ұ�

// Ÿ�̸� ���� �Լ�
void Hal_DustSensor_PPD71_ControlTimer(void);
void Hal_DustSensor_PPD71_Module_Control(void);

// ������ getter �Լ�
U16 Hal_DustSensor_PPD71_GetPM2_5(void);
U16 Hal_DustSensor_PPD71_GetPM10(void);
D64 Hal_DustSensor_PPD71_GetPM2_5Final(void);
D64 Hal_DustSensor_PPD71_GetPM10Final(void);
U8 Hal_DustSensor_PPD71_GetErrorFlag(void);

// Module Interface Functions
void Hal_DustSensor_PPD71_Module_Initialize(void);
void Hal_DustSensor_PPD71_Module_1ms_Control(void);

#endif // HAL_DUSTSENSOR_PPD71_H
