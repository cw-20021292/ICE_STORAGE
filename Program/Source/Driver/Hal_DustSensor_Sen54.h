#ifndef SENSIRION_SEN54_H
#define SENSIRION_SEN54_H

///////////////////////////////////////////////////////////////////////
/// Pin Definitions
//////////////////////////////////////////////////////////////////////

// #define REGISTER_DEFINE_SEN54_IIC_SCL_PIN_MODE     PM4_bit.no3
// #define REGISTER_DEFINE_SEN54_IIC_SDA_PIN_MODE     PM4_bit.no4

// #define PORT_DEFINE_SEN54_IIC_SCL_PIN_MODE         P4_bit.no3
// #define PORT_DEFINE_SEN54_IIC_SDA_PIN_MODE         P4_bit.no4
#define DUST_SENSOR_SEN54_USE USE

/*  SEN54 �������� ���� Function ����
    => Hal_DustSensor_Sen54_Module_Initialize();             // SEN54 ���� �ʱ�ȭ
    => SEN54_StartMeasurement();                             // SEN54 ���� ����
    => SEN54_Handler();                                      // SEN54 ���� ������ ó�� (�ֱ��� ȣ��)
    => getSen54PM1_0();                                      // PM1.0 �� �б�
    => getSen54PM2_5();                                      // PM2.5 �� �б�
    => getSen54PM10();                                       // PM10 �� �б�
    => getSen54Temp();                                       // �µ� �� �б� (��)
    => getSen54Humid();                                      // ���� �� �б� (%)
    => getSen54Voc();                                        // VOC �� �б�

    ��� ��)
    Hal_DustSensor_Sen54_Module_Initialize();                // �ý��� �ʱ�ȭ �� ȣ��
    SEN54_StartMeasurement();                                // ���� ����
    
    while(1) {
        Hal_DustSensor_Sen54_Module_Control();               // ���� �������� ȣ��
        
        // ���� �� �б�
        U16 pm1_0 = getSen54PM1_0();
        U16 pm2_5 = getSen54PM2_5();
        U16 pm10 = getSen54PM10();
        
        // �½��� �� VOC �б�
        I16 temperature = getSen54Temp();                    // �µ� (�� x 100)
        U16 humidity = getSen54Humid();                      // ���� (% x 100)
        U16 voc_index = getSen54Voc();                       // VOC Index
        
        // �� û�� ��� (�ʿ��)
        SEN54_StartFanClean();                               // �ڵ� �� û�� ����
    }

    ���ǻ���)
    - DUST_SENSOR_SEN54_USE�� USE�� �����Ǿ�� ��
    - I2C ����� ����ϹǷ� I2C ������ �Ϸ�Ǿ�� ��
    - ���� ���� ����ȭ �ð�(3��) �ʿ�
    - �µ����� �������� 100��� ��ȯ�� (2545 �� 25.45��)
    - �������� �������� 100��� ��ȯ�� (5025 �� 50.25%)
    - I2C �ּ�: 0x69 (Write: 0xD2, Read: 0xD3)
*/

#define SEN54_SDA  P4_bit.no4
#define SEN54_SCL  P4_bit.no3
#define SEN54_SDA_PIN_MODE PM4_bit.no4
#define SEN54_SCL_PIN_MODE PM4_bit.no3

#define SEN54_SDA_PORT_MODE SEN54_SDA_PIN_MODE
#define SEN54_SCL_PORT_MODE SEN54_SCL_PIN_MODE

#define SEN54_I2C_PIN_MODE_INPUT      1
#define SEN54_I2C_PIN_MODE_OUTPUT     0

// SDA control macros
#define SEN54_SDA_LOW()  (SEN54_SDA = 0)  // set SDA to low
#define SEN54_SDA_OPEN() (SEN54_SDA = 1)  // set SDA to open-drain
#define SEN54_SDA_READ   SEN54_SDA       // read SDA

// SCL control macros
#define SEN54_SCL_LOW()  (SEN54_SCL = 0)  // set SCL to low
#define SEN54_SCL_OPEN() (SEN54_SCL = 1)  // set SCL to open-drain
#define SEN54_SCL_READ   SEN54_SCL       // read SCL

// Timing and protocol definitions
#define SEN54_Term 10
#define SEN54_POLYNOMIAL 0x131 // P(x) = x^8 + x^5 + x^4 + 1 = 100110001
#define SEN54_CONTROL_TIME_PERIOD 250

// Timer definitions
#define TIME_1SEC           1000    // 1��
#define TIME_2SEC           2000    // 2��
#define TIME_3SEC           3000    // 3��
#define TIME_5SEC           5000    // 5��
#define TIME_10SEC          10000   // 10��
#define TIME_10MSEC         10      // 10ms

// Power control pin
#define PARTICLE_SENSOR_POW   P14_bit.no2	// ���� ���� ���� �ΰ� ��Ʈ

// I2C address definitions
#define SEN54_ADDR_WRITE  0xD2    /// Write �ּ� (0x69 << 1 | 0)
#define SEN54_ADDR_READ   0xD3    /// Read �ּ� (0x69 << 1 | 1)

// Error flags
#define SEN54_FAN_ERR 0x10
#define SEN54_LASER_ERR 0x20
#define SEN54_RHT_ERR 0x40
#define SEN54_GAS_ERR 0x80

///////////////////////////////////////////////////////////////////////
/// Function Prototypes
//////////////////////////////////////////////////////////////////////

// I2C and Basic Control Functions
void SEN54_Delay_E(U8 mu8Time);
void SEN54_I2c_StartCondition(void);
void SEN54_I2c_StopCondition(void);
U8 SEN54_IICByte_Read(void);
void SEN54_I2C_ACK_(void);
void SEN54_I2C_NACK(void);
U8 SEN54_checkCRC(U8 Data1, U8 Data2);
void SEN54_IIC_Byte_Write(U8 mu8WriteByte);

// Sensor Operation Functions
void SEN54_ReadRawdata1(void);
void SEN54_ReadRawdata2(void);
void SEN54_DeviceStateClr(void);
void SEN54_DeviceStateRead_1(void);
void SEN54_DeviceStateRead_2(void);
void SEN54_ReadMeasure_1(void);
void SEN54_ReadMeasure_2(void);
void SEN54_AutoCleanFanOFF(void);
void SEN54_StartFanClean(void);
void SEN54_StartMeasurement(void);
void SEN54_Handler(void);

// Timer Functions
void SEN54_SetTimer(U16 period);
U8 SEN54_IsTimerExpired(void);
void SEN54_ControlTimer(void);
void Hal_DustSensor_Sen54_Module_Control(void);
void SEN54_1ms_Control(void);    // 1ms Timer Control Function

// Getter Functions
I16 getSen54Temp(void);
U16 getSen54Humid(void);
U16 getSen54PM1_0(void);
U16 getSen54PM2_5(void);
U16 getSen54PM10(void);
U16 getSen54Voc(void);

// Module Interface Functions
void Hal_DustSensor_Sen54_Module_Initialize(void);
void Hal_DustSensor_Sen54_Module_1ms_Control(void);

#endif // SENSIRION_SEN54_H



