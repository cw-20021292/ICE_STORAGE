/// @file     Hal_HumidTempSensor.h
/// @date     2025/03/24
/// @author   SHM
/// @brief    습도/온도 센서(SHT30) 제어를 위한 헤더 파일

#ifndef _HUMID_TEMP_SENSOR_H_
#define _HUMID_TEMP_SENSOR_H_

// ******************************************************************
//                            Humid Temp Sensor 설정
// ******************************************************************
#define HUMID_TEMP_SENSOR_USE                           USE           // Humid Temp Sensor 사용 여부 - 0(No Use), 1(Use)

#define VALUE_TEST                                      10000

#define PORT_DEFINE_TEST                                P5_bit.no0  // 센서 전원 제어 Port 세팅 : P50

#define REGISTER_DEFINE_HUMID_TEMP_IIC_SCL_PIN_MODE     PM5_bit.no6
#define REGISTER_DEFINE_HUMID_TEMP_IIC_SDA_PIN_MODE     PM5_bit.no7

#define PORT_DEFINE_HUMID_TEMP_IIC_SCL_PIN_MODE         P5_bit.no6
#define PORT_DEFINE_HUMID_TEMP_IIC_SDA_PIN_MODE         P5_bit.no7


#if (HUMID_TEMP_SENSOR_USE == 1)

/// @brief    I2C 통신용 SDA 핀 정의
#define HUMI_SDA  PORT_DEFINE_HUMID_TEMP_IIC_SDA_PIN_MODE

/// @brief    I2C 통신용 SCL 핀 정의
#define HUMI_SCL  PORT_DEFINE_HUMID_TEMP_IIC_SCL_PIN_MODE

/// @brief    SDA 핀 모드 제어 레지스터
#define HUMI_SDA_PORT_MODE REGISTER_DEFINE_HUMID_TEMP_IIC_SDA_PIN_MODE

/// @brief    SCL 핀 모드 제어 레지스터
#define HUMI_SCL_PORT_MODE REGISTER_DEFINE_HUMID_TEMP_IIC_SCL_PIN_MODE

/// @brief    I2C 핀 입력 모드 정의
#define HUMI_I2C_PIN_MODE_INPUT      1

/// @brief    I2C 핀 출력 모드 정의
#define HUMI_I2C_PIN_MODE_OUTPUT     0

/// @brief    SDA 핀 제어 매크로
/// @details  SDA 핀(Port B, bit 14)의 상태 제어
#define SDA_LOW()  HUMI_SDA = 0;     /// SDA 핀을 Low로 설정
#define SDA_OPEN() HUMI_SDA = 1;     /// SDA 핀을 Open-drain으로 설정
#define SDA_READ   HUMI_SDA          /// SDA 핀 상태 읽기

/// @brief    SCL 핀 제어 매크로
/// @details  SCL 핀(Port B, bit 13)의 상태 제어
#define SCL_LOW()  HUMI_SCL = 0;     /// SCL 핀을 Low로 설정
#define SCL_OPEN() HUMI_SCL = 1;     /// SCL 핀을 Open-drain으로 설정
#define SCL_READ   HUMI_SCL          /// SCL 핀 상태 읽기

/// @brief    CRC 계산용 다항식
/// @details  P(x) = x^8 + x^5 + x^4 + 1 = 100110001
#define POLYNOMIAL 0x131

/// @brief    I2C 통신 딜레이 시간
#define TIME_DELAY 10

/// @brief    에러 판단을 위한 최대 카운트 값
#define ERROR_CNT  5

/// @brief    SHT30 센서 I2C 주소 정의
#define ADDR_GND    0x44    /// ADDR 핀이 GND에 연결된 경우
#define ADDR_VCC    0x45    /// ADDR 핀이 VCC에 연결된 경우
#define ADDR_WRITE  0x88    /// Write 주소 (0x44 << 1 | 0)
#define ADDR_READ   0x89    /// Read 주소 (0x44 << 1 | 1)

/// @brief    SEN54 센서 I2C 주소 정의
#define SEN54_ADDR_WRITE  0xD2    /// Write 주소 (0x69 << 1 | 0)
#define SEN54_ADDR_READ   0xD3    /// Read 주소 (0x69 << 1 | 1)

/// @brief    I2C 통신 시작 조건 생성
/// @param    void
/// @return   void
void I2c_StartCondition(void);

/// @brief    I2C 통신 정지 조건 생성
/// @param    void
/// @return   void
void I2c_StopCondition(void);

/// @brief    I2C 1바이트 데이터 읽기
/// @param    void
/// @return   읽은 1바이트 데이터
U8 IICByte_Read(void);

/// @brief    I2C 1바이트 데이터 쓰기
/// @param    mu8WriteByte 전송할 1바이트 데이터
/// @return   void
void IIC_Byte_Write(U8 mu8WriteByte);

/// @brief    습도 데이터 계산
/// @param    void
/// @return   void
void calHumidity(void);

/// @brief    온도 데이터 계산
/// @param    void
/// @return   void
void calTemp(void);

/// @brief    습도 센서 초기화
/// @param    void
/// @return   void
void HumidityEnable(void);

/// @brief    습도 센서 메인 제어
/// @param    void
/// @return   void
void HumidityHandler(void);

/// @brief    현재 습도값 반환
/// @param    void
/// @return   현재 습도값 (1~100%)
U8 getHumidity(void);

/// @brief    현재 온도값 반환
/// @param    void
/// @return   현재 온도값 (1~50℃)
U8 getTemp(void);

/// @brief    습도 데이터 CRC 검증
/// @param    void
/// @return   계산된 CRC 값
U8 checkHumidityCRC(void);

/// @brief    온도 데이터 CRC 검증
/// @param    void
/// @return   계산된 CRC 값
U8 checkTempCRC(void);

/// @brief    I2C ACK 신호 생성
/// @param    void
/// @return   void
void I2C_ACK(void);

/// @brief    I2C NACK 신호 생성
/// @param    void
/// @return   void
void I2C_NACK(void);

/// @brief    데이터 CRC 계산
/// @param    Data1 첫 번째 바이트 데이터
/// @param    Data2 두 번째 바이트 데이터
/// @return   계산된 CRC 값
U8 checkCRC(U8 Data1, U8 Data2);

#endif
#endif




