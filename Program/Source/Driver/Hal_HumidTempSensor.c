/// @file     Hal_HumidTempSensor.c
/// @date     2025/03/24
/// @author   SHM
/// @brief    습도/온도 센서(SHT30) 제어 동작 관련 file

#include "Global_Header.h"

#if (HUMID_TEMP_SENSOR_USE == 1)

U16 gu16HumidTempSensorControlTimer = 0;

/// @brief    센서로부터 수신한 데이터 저장 버퍼
U8  rxHumiData[6] = {0,0,0,0,0,0};

/// @brief    I2C 통신 ACK 실패 플래그
U8 bfackfail;

/// @brief    현재 습도값 (RH%)
U16 gu16HumidityRH;

/// @brief    현재 온도값
U16 gu16Temp;

/// @brief    습도 데이터 갱신 타이머 (2초 주기)
U8  gu8HumiUpdateTimer = 0;

/// @brief    습도센서 에러 카운트 (5회 누적 시 에러 발생)
U8  gu8HumiErrCnt = 0;

/// @brief    습도 데이터 CRC 체크값
U8  calHumiCRC = 0;

/// @brief    온도 데이터 CRC 체크값
U8  calTempCRC = 0;

U32 Co2DlyCnt=1;

/// @brief    I2C 통신용 딜레이 함수
/// @param    mu8Time 딜레이 시간 카운트
/// @return   void
void Delay_e(U8 mu8Time)
{
	
  while(mu8Time) 
  {
    WDT_Restart();
    --mu8Time;

  }
  
/*
	 R_WDT_Restart();

	Co2DlyCnt =mu8Time*10;
	while(Co2DlyCnt);
	*/
}

/// @brief    I2C 통신 시작 조건 생성
/// @details  SCL이 HIGH인 상태에서 SDA를 HIGH에서 LOW로 변경
/// @param    void
/// @return   void
void I2c_StartCondition(void)
{
  HUMI_SDA_PORT_MODE = HUMI_I2C_PIN_MODE_OUTPUT;  //SDA: output

	SCL_OPEN();
	SDA_OPEN();
	Delay_e(10);
	SDA_LOW();
	Delay_e(10);
	SCL_LOW();


}

/// @brief    I2C 통신 정지 조건 생성
/// @details  SCL이 HIGH인 상태에서 SDA를 LOW에서 HIGH로 변경
/// @param    void
/// @return   void
void I2c_StopCondition(void)
{
  HUMI_SDA_PORT_MODE = HUMI_I2C_PIN_MODE_OUTPUT;  //SDA: output
  
	SDA_LOW();
	Delay_e(10);

	SCL_OPEN();
	Delay_e(10);
	
	SDA_OPEN();
	Delay_e(10);	
}

/// @brief    I2C 1바이트 데이터 읽기
/// @details  8비트 데이터를 MSB부터 순차적으로 읽음
/// @param    void
/// @return   읽은 1바이트 데이터
U8 IICByte_Read(void)
{
  //U8 mu8i = 0;
  //U8 mu8ReturnData = 0;
  
  U8 i, data=0;


  //-----------------------------//
  HUMI_SDA_PORT_MODE = HUMI_I2C_PIN_MODE_INPUT;
  
  for(i=0; i<8; i++)
  {
	  SCL_OPEN();
	  data = data << 1;
  
	  Delay_e(10);
	  if (HUMI_SDA == 1) data |= 0x01;	  
  

  	  SCL_LOW();
   	 Delay_e(10);
  }
  
  HUMI_SDA_PORT_MODE = HUMI_I2C_PIN_MODE_OUTPUT;
  
  return (data);
}

/// @brief    I2C ACK 신호 생성
/// @details  8비트 데이터 수신 후 ACK 신호 생성
/// @param    void
/// @return   void
void I2C_ACK(void)
{

	SDA_LOW();  
	SCL_OPEN();
	Delay_e(10);
	SCL_LOW();
	Delay_e(10);
	SDA_OPEN();  


}

/// @brief    I2C NACK 신호 생성
/// @details  8비트 데이터 수신 후 NACK 신호 생성
/// @param    void
/// @return   void
void I2C_NACK(void)
{
	  SDA_OPEN();  
	  SCL_OPEN();
	  Delay_e(10);
	  SDA_LOW();  
	  Delay_e(10);
	  SDA_OPEN(); 	 
}

/// @brief    I2C 1바이트 데이터 쓰기
/// @details  8비트 데이터를 MSB부터 순차적으로 전송하고 ACK를 확인
/// @param    mu8WriteByte 전송할 1바이트 데이터
/// @return   void
void IIC_Byte_Write(U8 mu8WriteByte)
{
  U8 mu8Count = 0;

  
  for (mu8Count = 0 ; mu8Count < 8 ; mu8Count++)
  {      

    if (mu8WriteByte & 0x80)                // check for state of data bit to xmit
    {
      SDA_OPEN();
    }
    else
    {
      SDA_LOW();
    }
    Delay_e(10);
	
     SCL_OPEN();
	 Delay_e(10);
    mu8WriteByte = mu8WriteByte << 1;
	
	 
	SCL_LOW();
	 Delay_e(10);
  }

  HUMI_SDA_PORT_MODE = HUMI_I2C_PIN_MODE_INPUT;
  Delay_e(10);
	
  SCL_OPEN();
  Delay_e(10);

	
  for (mu8Count = 0 ; mu8Count < 5 ; mu8Count++)
  {
    if (HUMI_SDA)
    {
      bfackfail = 1;
    }
    else
    {
      bfackfail = 0;
      mu8Count = 5;
    }
  }

  SCL_LOW();

   
  HUMI_SDA_PORT_MODE = HUMI_I2C_PIN_MODE_OUTPUT; // LMJ

    SDA_LOW();
}

/// @brief    습도 데이터 계산 함수
/// @details  센서로부터 받은 raw 데이터를 실제 습도값(%)으로 변환
///          - RH = rawValue / (2^16-1) * 100
/// @param    void
/// @return   void
void calHumidity(void)
{
  U16 mu16Humidity = 0;
  
  mu16Humidity = ((U16)rxHumiData[3] << 8);
  mu16Humidity = (mu16Humidity | rxHumiData[4]);
  
  // calculate relative humidity [%RH]
  // RH = rawValue / (2^16-1) * 100
  gu16HumidityRH = (U16)(100.0f * (float)mu16Humidity / 65535.0f);
    // if(DebugNmxRx[7]==1)printf_f((const char* __far)"HUMID: %d\n\r",gu16HumidityRH);
}

/// @brief    온도 데이터 계산 함수
/// @details  센서로부터 받은 raw 데이터를 실제 온도값(℃)으로 변환
///          - T = -45 + 175 * rawValue / (2^16-1)
/// @param    void
/// @return   void
void calTemp(void)
{
  U16 mu16Temp = 0, Temp16=0;
  float TempFloat=0;
  
  mu16Temp = ((U16)rxHumiData[0] << 8);
  mu16Temp = (mu16Temp | rxHumiData[1]);
  
  // calculate temperature [캜]
  // T = -45 + 175 * rawValue / (2^16-1)

  TempFloat = (175.0f * (float)mu16Temp / 65535.0f - 45.0f);
  Temp16  = (U16)((TempFloat)*10);
  
  if((Temp16%10) >=5)  gu16Temp = (U16)TempFloat+1;
  else gu16Temp = (U16)TempFloat;
 
//   if(DebugNmxRx[7]==1)printf_f((const char* __far)"TMEP:%f , %d\n\r",TempFloat,gu16Temp);
  
}

/// @brief    습도 센서 초기화 함수
/// @details  센서를 활성화하고 초기 설정을 수행
///          - 센서 명령어 0x2220 전송
/// @param    void
/// @return   void
void HumidityEnable(void)
{
    
  I2c_StartCondition();
  IIC_Byte_Write(ADDR_WRITE);
  if(!bfackfail){IIC_Byte_Write(0x22);}
  if(!bfackfail){IIC_Byte_Write(0x20);}
  I2c_StopCondition();
  //registTimer(TimerHumiSensorCheck, Time1Sec*2);
}
void Hal_HumidTempSensor_Module_1ms_Control(void)
{
  //HumidityHandler();
  gu16HumidTempSensorControlTimer++;
  if(gu16HumidTempSensorControlTimer >= 1000)
  {
    gu8HumiUpdateTimer ++;
    gu16HumidTempSensorControlTimer = 0;
  }
}

/// @brief    습도 센서 메인 제어 함수
/// @details  2초 주기로 센서 데이터를 읽고 처리
///          - 센서 통신 상태 체크
///          - 데이터 CRC 검증
///          - 온습도 값 계산
/// @param    void
/// @return   void
void HumidityHandler(void)
{
	U8 i = 0, Flag=0;
	
  if(++gu8HumiUpdateTimer >= 2)
  {
    gu8HumiUpdateTimer = 0;
    
    I2c_StartCondition();
    IIC_Byte_Write(ADDR_WRITE);
    if(!bfackfail){IIC_Byte_Write(0xe0);}
    if(!bfackfail){IIC_Byte_Write(0x00);}

	
    I2c_StartCondition();
   // Delay_E(TIME_DELAY);
    if(!bfackfail){IIC_Byte_Write(ADDR_READ);}
   // Delay_E(TIME_DELAY);
    
    // Humidity Error Check //
    if(bfackfail)
    {
      if(gu8HumiErrCnt < ERROR_CNT)
      {
        gu8HumiErrCnt++;
        HumidityEnable(); //5번까지 리트라이
      }
      else
      {
        // gu8HumiErrCnt = 0;
        // //습도센서 에러 발생
		// //gu8ErrorTemp |= SHT30_ERROR;
		
        // return;
      }
    }
    else
    {
      gu8HumiErrCnt = 0;
    }
	
    // Humidity Error Check //
    /*
    for(i = 0; i < 6; i++)
    {
      rxHumiData[i] = IICByte_Read();
    }
	*/
	
	i=0;
	rxHumiData[i++] = IICByte_Read();
	I2C_ACK();
	rxHumiData[i++] = IICByte_Read();
	I2C_ACK();
	rxHumiData[i++] = IICByte_Read();
	I2C_ACK();
	rxHumiData[i++] = IICByte_Read();
	I2C_ACK();
	rxHumiData[i++] = IICByte_Read();
	I2C_ACK();
	rxHumiData[i++] = IICByte_Read();
	I2C_NACK();
    
    I2c_StopCondition();

	// if(DebugNmxRx[7]==1) 
	// {
	// 	printf_f((const char* __far)"SHT30: %x %x %x %x %x %x \n\r",rxHumiData[0],rxHumiData[1],rxHumiData[2],rxHumiData[3],rxHumiData[4],rxHumiData[5] );

	// }

	  calHumiCRC = checkHumidityCRC();  //CRC 체크
	  calTempCRC = checkTempCRC();      //CRC 체크
	  
	  if(rxHumiData[2] == calHumiCRC)
	  {
	    calHumidity(); 
		Flag|=0x01;

	  }
	//   else	printf_f((const char* __far)"ERROR:SHT30_Humid CRC[cal:%x Chk:%x]\n\r",calHumiCRC,rxHumiData[2] );
		
	  if(rxHumiData[5] == calTempCRC)
	  {
	    calTemp();
		Flag|=0x02;
		
	  }
	//   else 	printf_f((const char* __far)"ERROR:SHT30_Temp CRC [cal:%x Chk:%x]\n\r",calTempCRC,rxHumiData[5]);


	//   if(gu8ErrorList & ERROR_PRI_HUMI)
	//   {
	// 	if(Flag==0x03) 

	// 	{
	// 		setErrorStatus(NO_ERROR_HUMIDITY, ERROR_CLEAR);			

	// 	}
	//  }
	
  }  

}

/// @brief    현재 습도값 반환 함수
/// @details  측정된 습도값을 1~100% 범위로 보정하여 반환
/// @param    void
/// @return   현재 습도값 (1~100%)
U8 getHumidity(void)
{
  //표시 범위 설정 (1~100)
  if(gu16HumidityRH == 0){gu16HumidityRH = 1;}
  else if(gu16HumidityRH >= 100){gu16HumidityRH = 100;}
  else{}
  
  return (U8)gu16HumidityRH;
}

/// @brief    현재 온도값 반환 함수
/// @details  측정된 온도값을 1~50℃ 범위로 보정하여 반환
/// @param    void
/// @return   현재 온도값 (1~50℃)
U8 getTemp(void)
{
  //표시 범위 설정 (1~50)
  if(gu16Temp == 0){gu16Temp = 1;}
  else if(gu16Temp >= 50){gu16Temp = 50;}
  else{}
  
  return (U8)gu16Temp;
}

/// @brief    습도 데이터 CRC 검증 함수
/// @details  수신된 습도 데이터의 유효성을 CRC-8로 검증
///          - Polynomial: 0x31 (x8 + x5 + x4 + 1)
///          - Initial Value: 0xFF
/// @param    void
/// @return   계산된 CRC 값
U8 checkHumidityCRC(void)
{
  U8 i;
  U8 crc = 0xFF;
  U8 byteCtr;
  
  for(byteCtr = 0; byteCtr < 2; byteCtr++)
  {
    crc ^= (rxHumiData[byteCtr]);
    for(i = 8; i > 0; --i)
    {
      if(crc & 0x80) crc = (U8)((crc << 1) ^ POLYNOMIAL);
      else           crc = (crc << 1);
    }
  }
  
  return crc;
}

/// @brief    온도 데이터 CRC 검증 함수
/// @details  수신된 온도 데이터의 유효성을 CRC-8로 검증
///          - Polynomial: 0x31 (x8 + x5 + x4 + 1)
///          - Initial Value: 0xFF
/// @param    void
/// @return   계산된 CRC 값
U8 checkTempCRC(void)
{
  U8 i;
  U8 crc = 0xFF;
  U8 byteCtr;
  
  for(byteCtr = 3; byteCtr < 5; byteCtr++)
  {
    crc ^= (rxHumiData[byteCtr]);
    for(i = 8; i > 0; --i)
    {
      if(crc & 0x80) crc = (U8)((crc << 1) ^ POLYNOMIAL);
      else           crc = (crc << 1);
    }
  }
  
  return crc;
}

/// @brief    데이터 CRC 계산 함수
/// @details  두 바이트 데이터에 대한 CRC-8 계산
///          - Polynomial: 0x31 (x8 + x5 + x4 + 1)
///          - Initial Value: 0xFF
/// @param    Data1 첫 번째 바이트 데이터
/// @param    Data2 두 번째 바이트 데이터
/// @return   계산된 CRC 값
U8 checkCRC(U8 Data1, U8 Data2)
{
  U8 i;
  U8 crc = 0xFF;
  
//  for(byteCtr = 0; byteCtr < 2; byteCtr++)
  {
    crc ^= (Data1);
    for(i = 8; i > 0; --i)
    {
      if(crc & 0x80) crc = (U8)((crc << 1) ^ POLYNOMIAL);
      else           crc = (crc << 1);
    }

  crc ^= (Data2);
  for(i = 8; i > 0; --i)
  {
	if(crc & 0x80) crc = (U8)((crc << 1) ^ POLYNOMIAL);
	else		   crc = (crc << 1);
  }
	
  }
  
  return crc;
}

//------------- SEN54 --------------------------------//


#endif
