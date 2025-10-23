/// @file     Hal_HumidTempSensor.h
/// @date     2025/03/24
/// @author   SHM
/// @brief    ����/�µ� ����(SHT30) ��� ���� ��� ����

#ifndef _HUMID_TEMP_SENSOR_H_
#define _HUMID_TEMP_SENSOR_H_

// ******************************************************************
//                            Humid Temp Sensor ����
// ******************************************************************
#define HUMID_TEMP_SENSOR_USE                           USE           // Humid Temp Sensor ��� ���� - 0(No Use), 1(Use)

#define VALUE_TEST                                      10000

#define PORT_DEFINE_TEST                                P5_bit.no0  // ���� ���� ���� Port ���� : P50

#define REGISTER_DEFINE_HUMID_TEMP_IIC_SCL_PIN_MODE     PM5_bit.no6
#define REGISTER_DEFINE_HUMID_TEMP_IIC_SDA_PIN_MODE     PM5_bit.no7

#define PORT_DEFINE_HUMID_TEMP_IIC_SCL_PIN_MODE         P5_bit.no6
#define PORT_DEFINE_HUMID_TEMP_IIC_SDA_PIN_MODE         P5_bit.no7


#if (HUMID_TEMP_SENSOR_USE == 1)

/// @brief    I2C ��ſ� SDA �� ����
#define HUMI_SDA  PORT_DEFINE_HUMID_TEMP_IIC_SDA_PIN_MODE

/// @brief    I2C ��ſ� SCL �� ����
#define HUMI_SCL  PORT_DEFINE_HUMID_TEMP_IIC_SCL_PIN_MODE

/// @brief    SDA �� ��� ���� ��������
#define HUMI_SDA_PORT_MODE REGISTER_DEFINE_HUMID_TEMP_IIC_SDA_PIN_MODE

/// @brief    SCL �� ��� ���� ��������
#define HUMI_SCL_PORT_MODE REGISTER_DEFINE_HUMID_TEMP_IIC_SCL_PIN_MODE

/// @brief    I2C �� �Է� ��� ����
#define HUMI_I2C_PIN_MODE_INPUT      1

/// @brief    I2C �� ��� ��� ����
#define HUMI_I2C_PIN_MODE_OUTPUT     0

/// @brief    SDA �� ���� ��ũ��
/// @details  SDA ��(Port B, bit 14)�� ���� ����
#define SDA_LOW()  HUMI_SDA = 0;     /// SDA ���� Low�� ����
#define SDA_OPEN() HUMI_SDA = 1;     /// SDA ���� Open-drain���� ����
#define SDA_READ   HUMI_SDA          /// SDA �� ���� �б�

/// @brief    SCL �� ���� ��ũ��
/// @details  SCL ��(Port B, bit 13)�� ���� ����
#define SCL_LOW()  HUMI_SCL = 0;     /// SCL ���� Low�� ����
#define SCL_OPEN() HUMI_SCL = 1;     /// SCL ���� Open-drain���� ����
#define SCL_READ   HUMI_SCL          /// SCL �� ���� �б�

/// @brief    CRC ���� ���׽�
/// @details  P(x) = x^8 + x^5 + x^4 + 1 = 100110001
#define POLYNOMIAL 0x131

/// @brief    I2C ��� ������ �ð�
#define TIME_DELAY 10

/// @brief    ���� �Ǵ��� ���� �ִ� ī��Ʈ ��
#define ERROR_CNT  5

/// @brief    SHT30 ���� I2C �ּ� ����
#define ADDR_GND    0x44    /// ADDR ���� GND�� ����� ���
#define ADDR_VCC    0x45    /// ADDR ���� VCC�� ����� ���
#define ADDR_WRITE  0x88    /// Write �ּ� (0x44 << 1 | 0)
#define ADDR_READ   0x89    /// Read �ּ� (0x44 << 1 | 1)

/// @brief    SEN54 ���� I2C �ּ� ����
#define SEN54_ADDR_WRITE  0xD2    /// Write �ּ� (0x69 << 1 | 0)
#define SEN54_ADDR_READ   0xD3    /// Read �ּ� (0x69 << 1 | 1)

/// @brief    I2C ��� ���� ���� ����
/// @param    void
/// @return   void
void I2c_StartCondition(void);

/// @brief    I2C ��� ���� ���� ����
/// @param    void
/// @return   void
void I2c_StopCondition(void);

/// @brief    I2C 1����Ʈ ������ �б�
/// @param    void
/// @return   ���� 1����Ʈ ������
U8 IICByte_Read(void);

/// @brief    I2C 1����Ʈ ������ ����
/// @param    mu8WriteByte ������ 1����Ʈ ������
/// @return   void
void IIC_Byte_Write(U8 mu8WriteByte);

/// @brief    ���� ������ ���
/// @param    void
/// @return   void
void calHumidity(void);

/// @brief    �µ� ������ ���
/// @param    void
/// @return   void
void calTemp(void);

/// @brief    ���� ���� �ʱ�ȭ
/// @param    void
/// @return   void
void HumidityEnable(void);

/// @brief    ���� ���� ���� ����
/// @param    void
/// @return   void
void HumidityHandler(void);

/// @brief    ���� ������ ��ȯ
/// @param    void
/// @return   ���� ������ (1~100%)
U8 getHumidity(void);

/// @brief    ���� �µ��� ��ȯ
/// @param    void
/// @return   ���� �µ��� (1~50��)
U8 getTemp(void);

/// @brief    ���� ������ CRC ����
/// @param    void
/// @return   ���� CRC ��
U8 checkHumidityCRC(void);

/// @brief    �µ� ������ CRC ����
/// @param    void
/// @return   ���� CRC ��
U8 checkTempCRC(void);

/// @brief    I2C ACK ��ȣ ����
/// @param    void
/// @return   void
void I2C_ACK(void);

/// @brief    I2C NACK ��ȣ ����
/// @param    void
/// @return   void
void I2C_NACK(void);

/// @brief    ������ CRC ���
/// @param    Data1 ù ��° ����Ʈ ������
/// @param    Data2 �� ��° ����Ʈ ������
/// @return   ���� CRC ��
U8 checkCRC(U8 Data1, U8 Data2);

#endif
#endif




