/// @file     Hal_DustSensor_Sen54.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    Sensirion SEN54 복합센서 드라이버
/// @details  SEN54 센서 제어를 위한 HAL 드라이버 구현
	

#include "Global_Header.h"

#if (DUST_SENSOR_SEN54_USE == USE)

// Timer definitions
#define TIME_1SEC           1000    // 1초
#define TIME_2SEC           2000    // 2초
#define TIME_3SEC           3000    // 3초
#define TIME_5SEC           5000    // 5초
#define TIME_10SEC          10000   // 10초
#define TIME_10MSEC         10      // 10ms

static U16 gu16SEN54ControlTimer = 0;
static U16 gu16SEN54CheckTimer = 0;     // SEN54 체크 타이머

static U8 gu8SEN54TimerFlag = 0;        // SEN54 타이머 플래그

U8 Sen54Init=0;
U16 SEN54_PM1_0=0, SEN54_PM2_5=0,SEN54_PM10=0,SEN54_Humid=0, SEN54_Voc=0;
U16 SEN54_VocRaw=0;

I16 SEN54_Temp=0;
U8 SEN54_bfackfail;

U8  rxSen54Data[60] = {0,0,0,0,0,0};  //RX Data from Sensor
U8 gu8Sen54ErrParticle_F=0;
U8 gu8Sen54ErrParticle1=0;
U8 gu8Sen54DustErrorCode=0;

U8 gu8SEN54_GainType=0; // SEN54 GAIN  0:0.3, 1:1.03 

U16 gu16DustAdd=0;

U8 gu8FanCleanF=0;
U8 gu8Sen54DevChkF=0;
U8 gu8Sen54ErrCode=0;

U8 u8Sen54CrcErrCnt=0;

U16 gu16Sen54GasResult = 0;  // SEN54 Gas Sensor Result

/// @brief      SEN54 센서용 지연 함수
/// @param      mu8Time 지연 시간 (단위: WDT 재시작 주기)
/// @return     void
/// @details    지정된 시간만큼 WDT를 재시작하면서 지연
void SEN54_Delay_E(U8 mu8Time)
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


/// @brief      I2C Start Condition 생성
/// @param      void
/// @return     void
/// @details    I2C 통신 시작을 위한 Start Condition 신호 생성
void SEN54_I2c_StartCondition(void)
{
  SEN54_SDA_PORT_MODE = SEN54_I2C_PIN_MODE_OUTPUT;  //SDA: output

	SEN54_SCL_OPEN();
	SEN54_SDA_OPEN();
	SEN54_Delay_E(SEN54_Term);
	SEN54_SDA_LOW();
	SEN54_Delay_E(SEN54_Term);
	SEN54_SCL_LOW();


}

/// @brief      I2C Stop Condition 생성
/// @param      void
/// @return     void
/// @details    I2C 통신 종료를 위한 Stop Condition 신호 생성
void SEN54_I2c_StopCondition(void)
{
  SEN54_SDA_PORT_MODE = SEN54_I2C_PIN_MODE_OUTPUT;  //SDA: output
  
	SEN54_SDA_LOW();
	SEN54_Delay_E(SEN54_Term);

	SEN54_SCL_OPEN();
	SEN54_Delay_E(SEN54_Term);
	
	SEN54_SDA_OPEN();
	SEN54_Delay_E(SEN54_Term);	
}

/// @brief      I2C로부터 1바이트 데이터 읽기
/// @param      void
/// @return     U8 읽어온 1바이트 데이터
/// @details    I2C 통신으로 슬레이브 디바이스로부터 1바이트 데이터를 읽어옴
U8 SEN54_IICByte_Read(void)
{
  U8 i, data=0;

  SEN54_SDA_PORT_MODE = SEN54_I2C_PIN_MODE_INPUT;
  
  for(i=0; i<8; i++)
  {
	  SEN54_SCL_OPEN();
	  data = data << 1;
  
	  SEN54_Delay_E(SEN54_Term);
	  if (SEN54_SDA == 1) data |= 0x01;	  
  
  	  SEN54_SCL_LOW();
   	  SEN54_Delay_E(SEN54_Term);
  }
  
  SEN54_SDA_PORT_MODE = SEN54_I2C_PIN_MODE_OUTPUT;
  
  return (data);
}

/// @brief      I2C ACK 신호 전송
/// @param      void
/// @return     void
/// @details    I2C 통신에서 데이터 수신 확인을 위한 ACK 신호 전송
void SEN54_I2C_ACK(void)
{
/*
	SEN54_SDA_LOW();  
	SEN54_SCL_OPEN();
	SEN54_Delay_E(SEN54_Term);
	SEN54_SCL_LOW();
	SEN54_Delay_E(SEN54_Term);
	//SEN54_SDA_OPEN();  
*/
		SEN54_SDA_LOW();
		SEN54_Delay_E(SEN54_Term);
		SEN54_SCL_OPEN();
		SEN54_Delay_E(SEN54_Term);
		SEN54_SCL_LOW();
		SEN54_Delay_E(SEN54_Term);
		SEN54_SDA_OPEN();
}
/// @brief      I2C ACK 신호 전송 (대체 버전)
/// @param      void
/// @return     void
/// @details    I2C 통신에서 데이터 수신 확인을 위한 ACK 신호 전송 (대체 구현)
void SEN54_I2C_ACK_(void)
{

	SEN54_SDA_LOW();  
	SEN54_SCL_OPEN();
	SEN54_Delay_E(SEN54_Term);
	SEN54_SCL_LOW();
	SEN54_Delay_E(SEN54_Term);
	SEN54_SDA_OPEN();  


}

/// @brief      I2C NACK 신호 전송
/// @param      void
/// @return     void
/// @details    I2C 통신에서 데이터 수신 종료를 위한 NACK 신호 전송
void SEN54_I2C_NACK(void)
{
	  SEN54_SDA_OPEN();  
	  SEN54_Delay_E(SEN54_Term);
	  SEN54_SCL_OPEN();
	  SEN54_Delay_E(SEN54_Term);
	  SEN54_SCL_LOW();  
	  SEN54_Delay_E(SEN54_Term);
	  SEN54_SDA_LOW();
	 	 
}

/// @brief      SEN54 CRC 체크섬 계산
/// @param      Data1 첫 번째 데이터 바이트
/// @param      Data2 두 번째 데이터 바이트  
/// @return     U8 계산된 CRC 값
/// @details    SEN54 센서의 2바이트 데이터에 대한 CRC 체크섬 계산
U8 SEN54_checkCRC(U8 Data1, U8 Data2)
{
  U8 i;
  U8 crc = 0xFF;

  
//  for(byteCtr = 0; byteCtr < 2; byteCtr++)
  {
    crc ^= (Data1);
    for(i = 8; i > 0; --i)
    {
      if(crc & 0x80) crc = (U8)((crc << 1) ^ SEN54_POLYNOMIAL);
      else           crc = (crc << 1);
    }

  crc ^= (Data2);
  for(i = 8; i > 0; --i)
  {
	if(crc & 0x80) crc = (U8)((crc << 1) ^ SEN54_POLYNOMIAL);
	else		   crc = (crc << 1);
  }
	
  }
  
  return crc;
}


/// @brief      I2C로 1바이트 데이터 전송
/// @param      mu8WriteByte 전송할 1바이트 데이터
/// @return     void
/// @details    I2C 통신으로 슬레이브 디바이스에 1바이트 데이터를 전송하고 ACK 확인
void SEN54_IIC_Byte_Write(U8 mu8WriteByte)
{
  U8 mu8Count = 0;

  
  for (mu8Count = 0 ; mu8Count < 8 ; mu8Count++)
  {      

    if (mu8WriteByte & 0x80)                // check for state of data bit to xmit
    {
      SEN54_SDA_OPEN();
    }
    else
    {
      SEN54_SDA_LOW();
    }
    SEN54_Delay_E(SEN54_Term);
	
     SEN54_SCL_OPEN();
	 SEN54_Delay_E(SEN54_Term);
    mu8WriteByte = mu8WriteByte << 1;
	
	 
	SEN54_SCL_LOW();
	 SEN54_Delay_E(SEN54_Term);
  }

  SEN54_SDA_PORT_MODE = SEN54_I2C_PIN_MODE_INPUT;
  SEN54_Delay_E(SEN54_Term);
	
  SEN54_SCL_OPEN();
  SEN54_Delay_E(SEN54_Term);

	
  for (mu8Count = 0 ; mu8Count < 5 ; mu8Count++)
  {
    if (SEN54_SDA)
    {
      SEN54_bfackfail = 1;
    }
    else
    {
      SEN54_bfackfail = 0;
      mu8Count = 5;
    }
  }

  SEN54_SCL_LOW();

   
  SEN54_SDA_PORT_MODE = SEN54_I2C_PIN_MODE_OUTPUT; // LMJ

    SEN54_SDA_LOW();
}


/// @brief      SEN54 Raw 데이터 읽기 명령 전송 (1단계)
/// @param      void
/// @return     void
/// @details    SEN54 센서에 Raw 데이터 읽기 명령(0x03D2)을 전송
void SEN54_ReadRawdata1(void)
{
	SEN54_bfackfail=0;	
	
	SEN54_I2c_StartCondition();
	SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x03);}  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xD2);}
	SEN54_I2c_StopCondition();
	

}

/// @brief      SEN54 Raw 데이터 읽기 (2단계)
/// @param      void
/// @return     void
/// @details    SEN54 센서로부터 Raw 데이터를 읽어와서 VOC Raw 값 업데이트
void SEN54_ReadRawdata2(void)
{
	U8 i=0;
	U8 CalCrc=0;
	
	SEN54_I2c_StartCondition();  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(SEN54_ADDR_READ);}
	
	for(i=0; i<11; i++)
	{
		rxSen54Data[i] = SEN54_IICByte_Read();
		SEN54_I2C_ACK();
	}
	rxSen54Data[i] = SEN54_IICByte_Read();
	SEN54_I2C_NACK();
	
	SEN54_I2c_StopCondition();
	
	//    if(DebugNmxRx[7]==1) 
	//    {
	// 	//    printf_f((const char* __far)"SEN54_RAW: ");
	
	// 	   for(i=0;i<12;i++)
	// 	   {
	// 		//    printf_f((const char* __far)"%x ",rxSen54Data[i]);
	// 		   printf("%x ",rxSen54Data[i]);
	
	// 	   }
	// 	//    printf_f((const char* __far)"\n\r");
	// 	   printf("\n\r");
	//    } 

	    

	    CalCrc = SEN54_checkCRC(rxSen54Data[6],rxSen54Data[7]); // 
	    if(CalCrc == rxSen54Data[8])
	    {
			SEN54_VocRaw=rxSen54Data[6];
			SEN54_VocRaw<<=8;
			SEN54_VocRaw|=rxSen54Data[7];
			// printf_f((const char* __far)"VocRaw %d\n\r",SEN54_VocRaw);
			// printf("VocRaw %d\n\r",SEN54_VocRaw);

	    }
		else
		{
			// printf_f((const char* __far)"VocRaw CRC Error\n\r");
			// printf("VocRaw CRC Error\n\r");
		}		

}



/// @brief      SEN54 디바이스 상태 클리어
/// @param      void
/// @return     void
/// @details    SEN54 센서의 디바이스 상태를 클리어하는 명령(0xD210) 전송
void SEN54_DeviceStateClr(void)
{
	SEN54_I2c_StartCondition();
	SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xD2);}  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x10);}
	SEN54_I2c_StopCondition();
}

/// @brief      SEN54 디바이스 상태 읽기 명령 전송 (1단계)
/// @param      void
/// @return     void
/// @details    SEN54 센서에 디바이스 상태 읽기 명령(0xD206)을 전송
void SEN54_DeviceStateRead_1(void)
{
	
	
	 SEN54_bfackfail=0;	 
	
	 SEN54_I2c_StartCondition();
	 SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);	
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xD2);}	
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x06);}
	 SEN54_I2c_StopCondition();
 

}

/// @brief      SEN54 디바이스 상태 읽기 (2단계)
/// @param      void
/// @return     void
/// @details    SEN54 센서로부터 디바이스 상태 데이터를 읽어옴
void SEN54_DeviceStateRead_2(void)
{	
	U8 i=0;
	
	SEN54_I2c_StartCondition();  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(SEN54_ADDR_READ);}
	
	for(i=0; i<5; i++)
	{
		rxSen54Data[i] = SEN54_IICByte_Read();
		SEN54_I2C_ACK();
	}
	rxSen54Data[i] = SEN54_IICByte_Read();
	SEN54_I2C_NACK();
	
	SEN54_I2c_StopCondition();
	
	//    if(DebugNmxRx[7]==1) 
	//    {
	// 	   printf("SEN54_DEV: ");
	
	// 	   for(i=0;i<6;i++)
	// 	   {
	// 		//    printf_f((const char* __far)"%x ",rxSen54Data[i]);
	// 		   printf("%x ",rxSen54Data[i]);
	
	// 	   }
	// 	   printf("\n\r");
	//    } 

}

/// @brief      SEN54 측정 데이터 읽기 명령 전송 (1단계)
/// @param      void
/// @return     void
/// @details    SEN54 센서에 측정 데이터 읽기 명령(0x03C4)을 전송
void SEN54_ReadMeasure_1(void)
{
	  SEN54_I2c_StartCondition();
	  SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
	
	  if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x03);}
	
	  if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xc4);}
	
	 SEN54_I2c_StopCondition();

}

/// @brief      SEN54 측정 데이터 읽기 (2단계)
/// @param      void
/// @return     void
/// @details    SEN54 센서로부터 PM, 온습도, VOC 등 모든 측정 데이터를 읽어옴
void SEN54_ReadMeasure_2(void)
{

	U8 i = 0;
	U8 CalCrc=0;
	
	SEN54_I2c_StartCondition();
	  
		 // Delay_E(TIME_DELAY);
		  if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(SEN54_ADDR_READ);}
		 // Delay_E(TIME_DELAY);
		
		  // Humidity Error Check //
		  if(SEN54_bfackfail)
		  {
			//    printf("SEN54 ERROR-2!\n\r");	
			   return;
		  }
		  else
		  {
			//gu8HumiErrCnt = 0;
		  }
		  
	  
		  for(i=0; i<23; i++)
		  {
			  rxSen54Data[i] = SEN54_IICByte_Read();
			  I2C_ACK();
		  }
		  rxSen54Data[i] = SEN54_IICByte_Read();
		  SEN54_I2C_NACK();
		  
		  SEN54_I2c_StopCondition();
	
		//   if(DebugNmxRx[7]==1) 
		//   {
		// 	  printf("SEN54: ");
	
		// 	  for(i=0;i<24;i++)
		// 	  {
		// 		//   printf_f((const char* __far)"%x ",rxSen54Data[i]);
		// 		  printf("%x ",rxSen54Data[i]);
	
		// 	  }
		// 	  printf("\n\r");
		//   }
	
		  CalCrc = SEN54_checkCRC(rxSen54Data[0],rxSen54Data[1]); // PM1.0 CRC Check
		  if(CalCrc == rxSen54Data[2])
		  {
	
			  if((rxSen54Data[0]==0xFF)&&(rxSen54Data[1]==0xFF))
			  {
				  Sen54Init=0;
			  }
			  else
			  { 		  
				  SEN54_PM1_0=rxSen54Data[0];
				  SEN54_PM1_0<<=8;
				  SEN54_PM1_0 |= rxSen54Data[1];
			  } 		  
		  }
		  else 
		  {
			  Sen54Init=0;
			//   printf("PM1.0 CRC Error\n\r");
		  }
	
		  CalCrc = SEN54_checkCRC(rxSen54Data[3],rxSen54Data[4]); // PM2.5 CRC Check
		  if(CalCrc == rxSen54Data[5])
		  {
			  if((rxSen54Data[3]==0xFF)&&(rxSen54Data[4]==0xFF))
			  {
				  Sen54Init=0;
			  }
			  else
			  { 		  
				  SEN54_PM2_5=rxSen54Data[3];
				  SEN54_PM2_5<<=8;
				  SEN54_PM2_5 |= rxSen54Data[4];
			  }
		  }
		  else
		  {
			  Sen54Init=0;
			//   printf("PM2.5 CRC Error\n\r");
		  }
	
		  CalCrc = SEN54_checkCRC(rxSen54Data[9],rxSen54Data[10]); // PM10 CRC Check
		  if(CalCrc == rxSen54Data[11])
		  {
	
			  if((rxSen54Data[9]==0xFF)&&(rxSen54Data[10]==0xFF))
			  {
				  Sen54Init=0;
			  }
			  else
			  { 		  
				  SEN54_PM10=rxSen54Data[9];
				  SEN54_PM10<<=8;
				  SEN54_PM10 |= rxSen54Data[10];
			  }
	
			  
		  }
		  else
		  {
			  Sen54Init=0;
			//   printf("PM10 CRC Error\n\r");
		  }
	
		  CalCrc = SEN54_checkCRC(rxSen54Data[12],rxSen54Data[13]); // Humid CRC Check
		  if(CalCrc == rxSen54Data[14])
		  {
			  if((rxSen54Data[12]==0x7F)&&(rxSen54Data[13]==0xFF))
			  {
				  Sen54Init=0;
			  }
			  else
			  { 		  
				  SEN54_Humid=rxSen54Data[12];
				  SEN54_Humid<<=8;
				  SEN54_Humid |= rxSen54Data[13];
			  }
	
			  
		  }
		  else
		  {
			  Sen54Init=0;
			  printf("Humid CRC Error\n\r");
		  }
				  
		  CalCrc = SEN54_checkCRC(rxSen54Data[15],rxSen54Data[16]); // Temp CRC Check
		  if(CalCrc == rxSen54Data[17])
		  {
	
			  if((rxSen54Data[15]==0x7F)&&(rxSen54Data[16]==0xFF))
			  {
				  Sen54Init=0;
			  }
			  else
			  { 		  
				  SEN54_Temp=rxSen54Data[15];
				  SEN54_Temp<<=8;
				  SEN54_Temp |= rxSen54Data[16];
			  }
	
			  
		  }
		  else 
		  {
			  Sen54Init=0;
			//   printf("Temp CRC Error\n\r");  
		  }
	
		  CalCrc = SEN54_checkCRC(rxSen54Data[18],rxSen54Data[19]); // VocCRC Check
		  if(CalCrc == rxSen54Data[20])
		  {
	
			  if((rxSen54Data[18]==0x7F)&&(rxSen54Data[19]==0xFF))
			  {
				  Sen54Init=0;
			  }
			  else
			  { 		  
				  SEN54_Voc=rxSen54Data[18];
				  SEN54_Voc<<=8;
				  SEN54_Voc |= rxSen54Data[19];
			  } 	  
			  
		  }
		  else 
		  {
			  Sen54Init=0;
			//   printf("VOC CRC Error\n\r"); 
		  }
		  
		//   if(DebugNmxRx[7]==1) 
		//   {
			//   printf("SEN54: PM1.0: %d PM2.5: %d PM10: %d VOC: %d Temp: %d Humid: %d \n\r\n\r",SEN54_PM1_0/10 ,SEN54_PM2_5/10,SEN54_PM10/10,SEN54_Voc/10,SEN54_Temp/200,SEN54_Humid/100);	  
	
		//   }


}



/// @brief      SEN54 자동 팬 청소 기능 비활성화
/// @param      void
/// @return     void
/// @details    SEN54 센서의 자동 팬 청소 기능을 비활성화하는 명령 전송
void SEN54_AutoCleanFanOFF(void)
{

 U8 Temp8[6]={0,0,0,0,0,0};

 Temp8[2] = SEN54_checkCRC(Temp8[0],Temp8[1]);
 Temp8[5] = SEN54_checkCRC(Temp8[3],Temp8[4]);
 
 SEN54_I2c_StartCondition();
 SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x80);}
 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x04);}

 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(Temp8[0]);}
 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(Temp8[1]);}
 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(Temp8[2]);}
 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(Temp8[3]);}
 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(Temp8[4]);}
 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(Temp8[5]);}

 
 SEN54_I2c_StopCondition();	

 if(SEN54_bfackfail==1) printf("SEN54_AutoCleanFanOFF ERROR\n\r");

}

/// @brief      SEN54 측정 시작
/// @param      void
/// @return     void
/// @details    SEN54 센서에 측정 시작 명령(0x0021)을 전송
void SEN54_StartMeasurement(void)
{
	 SEN54_I2c_StartCondition();
	 SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x00);}
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x21);}
	 SEN54_I2c_StopCondition();
	
	
	if(SEN54_bfackfail==1)printf("SEN54_StartMeasurement ERROR!\n\r");	
}


/// @brief      SEN54 팬 청소 시작
/// @param      void
/// @return     void
/// @details    SEN54 센서의 팬 청소를 시작하는 명령(0x5607) 전송
void SEN54_StartFanClean(void)
{
	 SEN54_I2c_StartCondition();
	 SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x56);}
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x07);}
	 SEN54_I2c_StopCondition();	
	
	if(SEN54_bfackfail==1) printf("SEN54_StartFanClean ERROR\n\r");
}

U8 SEN54_ComErrCnt=0;

/// @brief      SEN54 센서 메인 핸들러
/// @param      void
/// @return     void
/// @details    SEN54 센서의 상태에 따라 초기화, 측정, 오류 처리 등을 수행하는 메인 제어 함수
void SEN54_Handler(void)
{
	U8 i = 0;
	U8 CalCrc=0;
	U8 ErrFlag=0;
	U8 temp8=0;
		
	float floatTemp=0, floatIndex=0;

	

  SEN54_bfackfail=0;

  if(Sen54Init==0)
  {


	
	SEN54_I2c_StartCondition();
	SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x00);}
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x21);}
	SEN54_I2c_StopCondition();


	 if(SEN54_bfackfail==0) 
	 {
		// Sen54Init=1;
		Sen54Init=99;
		SEN54_ComErrCnt=0;	
		SEN54_SetTimer(TIME_2SEC);
	 }
	 else
	 {
	 	SEN54_ComErrCnt++;
		if(SEN54_ComErrCnt > 5)
		{
			if((SEN54_ComErrCnt%5)==0)
			{
				Sen54Init=100;
				SEN54_SetTimer(TIME_10MSEC);

			}
			gu8Sen54ErrParticle_F = SET;
			// printf("DUST Sensor Error Set!\n\r");	// Set Dust Error!	
		}
		// printf("SEN54 ERROR-1!\n\r");
	 }

	 

  }
  else
  {
	if(Sen54Init==1)
	{
		
	 // if(++gu8HumiUpdateTimer >= 2)
	  
	   // gu8HumiUpdateTimer = 0;
	    SEN54_bfackfail=0;
	    SEN54_I2c_StartCondition();
	    SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
	
	    if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x03);}
	
	    if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xc4);}

	  SEN54_I2c_StopCondition();

		if(SEN54_bfackfail==0)	Sen54Init=2;
		else
		{

			Sen54Init=0;

		}
	 }
	else if(Sen54Init==2)
	{

		Sen54Init=3;

		
	    SEN54_I2c_StartCondition();
	
	   // Delay_E(TIME_DELAY);
	    if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(SEN54_ADDR_READ);}
	   // Delay_E(TIME_DELAY);
	  
	    // Humidity Error Check //
	    if(SEN54_bfackfail)
	    {
			/*
	      if(gu8HumiErrCnt < ERROR_CNT)
	      {
	        gu8HumiErrCnt++;
	        HumidityEnable(); //5번까지 리트라이
	      }
	      else
	      {
	        gu8HumiErrCnt = 0;
	        //습도센서 에러 발생
	     	 if(gu8Power && (gu8EntryStatus==COVER_CLOSE))   setErrorStatus(NO_ERROR_HUMIDITY, ERROR_SET);
			//gu8ErrorTemp |= SHT30_ERROR;
			
	        return;
	      }
		  */
		  Sen54Init=0;
			 printf("SEN54 ERROR-2!\n\r");
			

			 return;
	    }
	    else
	    {
	      //gu8HumiErrCnt = 0;
	    }
		
	    // Humidity Error Check //
	    /*
	    for(i = 0; i < 6; i++)
	    {
	      rxHumiData[i] = IICByte_Read();
	    }
		*/
		
	
		for(i=0; i<23; i++)
		{
			rxSen54Data[i] = SEN54_IICByte_Read();
			SEN54_I2C_ACK();
		}
		rxSen54Data[i] = SEN54_IICByte_Read();
		SEN54_I2C_NACK();
	    
	    SEN54_I2c_StopCondition();

		// if(DebugNmxRx[7]==1) 
		// {
		// 	printf("SEN54: ");

		// 	for(i=0;i<24;i++)
		// 	{
		// 		printf("%x ",rxSen54Data[i]);

		// 	}
		// 	printf("\n\r");
		// }

		CalCrc = SEN54_checkCRC(rxSen54Data[0],rxSen54Data[1]); // PM1.0 CRC Check
		if(CalCrc == rxSen54Data[2])
		{

			if((rxSen54Data[0]==0xFF)&&(rxSen54Data[1]==0xFF))
			{
				Sen54Init=0;
				ErrFlag=1;
			}
			else
			{
				SEN54_PM1_0=rxSen54Data[0];
				SEN54_PM1_0<<=8;
				SEN54_PM1_0 |= rxSen54Data[1];
			}	

			u8Sen54CrcErrCnt=0;
		}
		else 
		{
			u8Sen54CrcErrCnt++;
			Sen54Init=0;
			ErrFlag=1;
			//printf("PM1.0 CRC Error\n\r");
		}

		CalCrc = SEN54_checkCRC(rxSen54Data[3],rxSen54Data[4]); // PM2.5 CRC Check
		if(CalCrc == rxSen54Data[5])
		{
			if((rxSen54Data[3]==0xFF)&&(rxSen54Data[4]==0xFF))
			{
				Sen54Init=0;
				ErrFlag=1;
			}
			else
			{			
				SEN54_PM2_5=rxSen54Data[3];
				SEN54_PM2_5<<=8;
				SEN54_PM2_5 |= rxSen54Data[4];
			}

			u8Sen54CrcErrCnt=0;
		}
		else
		{
			u8Sen54CrcErrCnt++;
			Sen54Init=0;
			ErrFlag=1;
			// printf("PM2.5 CRC Error\n\r");
		}

		CalCrc = SEN54_checkCRC(rxSen54Data[9],rxSen54Data[10]); // PM10 CRC Check
		if(CalCrc == rxSen54Data[11])
		{

			if((rxSen54Data[9]==0xFF)&&(rxSen54Data[10]==0xFF))
			{
				Sen54Init=0;
				ErrFlag=1;
			}
			else
			{			
				SEN54_PM10=rxSen54Data[9];
				SEN54_PM10<<=8;
				SEN54_PM10 |= rxSen54Data[10];
			}
			u8Sen54CrcErrCnt=0;			
		}
		else
		{
			u8Sen54CrcErrCnt++;
			Sen54Init=0;
			ErrFlag=1;
			// printf("PM10 CRC Error\n\r");
		}

		CalCrc = SEN54_checkCRC(rxSen54Data[12],rxSen54Data[13]); // Humid CRC Check
		if(CalCrc == rxSen54Data[14])
		{
			if((rxSen54Data[12]==0x7F)&&(rxSen54Data[13]==0xFF))
			{
				Sen54Init=0;
				ErrFlag=1;
			}
			else
			{			
				SEN54_Humid=rxSen54Data[12];
				SEN54_Humid<<=8;
				SEN54_Humid |= rxSen54Data[13];
			}
			u8Sen54CrcErrCnt=0;			
		}
		else
		{
			u8Sen54CrcErrCnt++;
			Sen54Init=0;
			ErrFlag=1;
			// printf("Humid CRC Error\n\r");
		}
				
		CalCrc = SEN54_checkCRC(rxSen54Data[15],rxSen54Data[16]); // Temp CRC Check
		if(CalCrc == rxSen54Data[17])
		{

			if((rxSen54Data[15]==0x7F)&&(rxSen54Data[16]==0xFF))
			{
				Sen54Init=0;
				ErrFlag=1;
			}
			else
			{			
				SEN54_Temp=rxSen54Data[15];
				SEN54_Temp<<=8;
				SEN54_Temp |= rxSen54Data[16];
			}
			u8Sen54CrcErrCnt=0;			
		}
		else 
		{
			u8Sen54CrcErrCnt++;
			Sen54Init=0;
			ErrFlag=1;
			// printf("Temp CRC Error\n\r");	
		}

		CalCrc = SEN54_checkCRC(rxSen54Data[18],rxSen54Data[19]); // VocCRC Check
		if(CalCrc == rxSen54Data[20])
		{

			if((rxSen54Data[18]==0x7F)&&(rxSen54Data[19]==0xFF))
			{
				Sen54Init=0;
				ErrFlag=1;
			}
			else
			{			
				SEN54_Voc=rxSen54Data[18];
				SEN54_Voc<<=8;
				SEN54_Voc |= rxSen54Data[19];

			}		
			u8Sen54CrcErrCnt=0;
		}
		else 
		{
			u8Sen54CrcErrCnt++;
			Sen54Init=0;
			ErrFlag=1;
			// printf("VOC CRC Error\n\r"); 

			if(u8Sen54CrcErrCnt >= 30) 
			{
				gu8Sen54ErrParticle_F = SET;
				// printf("DUST Sensor Error Set!\n\r");	// Set Dust Error!	
			}
		}
		
		// if(DebugNmxRx[7]==1) 
		// {
			// printf("SEN54: PM1.0: %d PM2.5: %d PM10: %d VOC: %d Temp: %d Humid: %d \n\r\n\r",SEN54_PM1_0/10 ,SEN54_PM2_5/10,SEN54_PM10/10,SEN54_Voc/10,SEN54_Temp/200,SEN54_Humid/100);	

		// }

		if(ErrFlag==0)
		{
			if( (gu8Sen54ErrParticle_F) && (gu8Sen54ErrParticle1 ==CLEAR))
			{
				gu8Sen54ErrParticle_F = CLEAR;
				// printf("DUST Sensor Error Clear!\n\r");  // Clear Dust Error!	

				// printf(" SEN54 Comm OK\n\r");			
			}

		}
			
/*
		  calHumiCRC = checkHumidityCRC();  //CRC 체크
		  calTempCRC = checkTempCRC();      //CRC 체크
		  
		  if(rxHumiData[2] == calHumiCRC)
		  {
		    calHumidity(); 
			Flag|=0x01;

		  }
		  else	printf_f((const char* __far)"ERROR:SHT30_Humid CRC[cal:%x Chk:%x]\n\r",calHumiCRC,rxHumiData[2] );
			
		  if(rxHumiData[5] == calTempCRC)
		  {
		    calTemp();
			Flag|=0x02;
			
		  }
		  else 	printf_f((const char* __far)"ERROR:SHT30_Temp CRC [cal:%x Chk:%x]\n\r",calTempCRC,rxHumiData[5]);


		  if(gu8ErrorList & ERROR_PRI_HUMI)
		  {
			if(Flag==0x03) 

			{
				setErrorStatus(NO_ERROR_HUMIDITY, ERROR_CLEAR);			

			}
		 }

		  */
	}
	else if(Sen54Init==3)
	{	
		Sen54Init=4;
	
		SEN54_bfackfail=0;
		SEN54_I2c_StartCondition();
		SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
	
		if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x03);}
	
		if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xD2);}

		SEN54_I2c_StopCondition();

		if(SEN54_bfackfail==0)	Sen54Init=4;
		else Sen54Init=0;
	}
	else if(Sen54Init==4)
	{

		Sen54Init=1;
		SEN54_bfackfail=0;

		if(gu8Sen54DevChkF==1)
		{
			gu8Sen54DevChkF=0;
			Sen54Init=5;
		}

		SEN54_I2c_StartCondition();  
		if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(SEN54_ADDR_READ);}
		
		for(i=0; i<11; i++)
		{
			rxSen54Data[i] = SEN54_IICByte_Read();
			SEN54_I2C_ACK();
		}
		rxSen54Data[i] = SEN54_IICByte_Read();
		SEN54_I2C_NACK();
		
		SEN54_I2c_StopCondition();
		
		// if(DebugNmxRx[7]==1) 
		// {
		//    printf("SEN54_RAW: ");
	
		//    for(i=0;i<12;i++)
		//    {
		// 	   printf("%x ",rxSen54Data[i]);
	
		//    }
		//    printf("\n\r");
		// }			

		CalCrc = SEN54_checkCRC(rxSen54Data[6],rxSen54Data[7]); // 
		if(CalCrc == rxSen54Data[8])
		{
			SEN54_VocRaw=rxSen54Data[6];
			SEN54_VocRaw<<=8;
			SEN54_VocRaw|=rxSen54Data[7];

			if(SEN54_VocRaw > 33500)
			{
				if(gu8SEN54_GainType!=1)
				{
					gu8SEN54_GainType=1;
					// userEEPByteWrite(EEP_ADD_USER28, gu8SEN54_GainType);
				}
			}			
			
			floatIndex = (float)SEN54_VocRaw;
			floatTemp= exp( -(((floatIndex-33500.0f)-1200.0f)/2450.0f) );
			if(gu8SEN54_GainType==0)floatTemp*=0.3f;
			else floatTemp*=1.03f;							
			floatTemp+=1.10f;
		

			gu16Sen54GasResult= (U16)(floatTemp*100);		
			if(gu16Sen54GasResult>500) gu16Sen54GasResult=500;
			//printf_f((const char* __far)"VocRaw %u\n\r",SEN54_VocRaw);
		}
		else
		{
			Sen54Init=0;
			// printf("VocRaw CRC Error\n\r");
		}	
		
	}
	else if(Sen54Init==5)
	{
		
		SEN54_DeviceStateClr();

	
		
		if(SEN54_bfackfail==1) Sen54Init=0;
		else
		{			
			Sen54Init=6;
			SEN54_SetTimer(TIME_2SEC);		

			
		}

	}
	else if(Sen54Init==6)
	{
	
		SEN54_DeviceStateRead_1();
		
		if(SEN54_bfackfail==1) Sen54Init=0;
		else
		{			
			Sen54Init = 7;
			SEN54_SetTimer(TIME_2SEC);
		}
	}
	else if(Sen54Init==7)
	{
		
		SEN54_DeviceStateRead_2();

		if(SEN54_bfackfail==1) Sen54Init=0;
		else
		{		
			
			temp8=rxSen54Data[4];

			if( (temp8&SEN54_FAN_ERR) || (temp8&SEN54_LASER_ERR))
			{
				printf("DUST Sensor Error Set!\n\r");
				gu8Sen54ErrCode=1;
			}
			else
			{
				printf("DUST Sensor Error Clear!\n\r");
				gu8Sen54ErrCode=0;
				if(gu8Sen54ErrParticle_F==0)
				{
					printf("DUST Sensor Error Clear!\n\r");
				}

			}		
			
			if(temp8&SEN54_RHT_ERR)
			{
				printf("HUMIDITY Sensor Error Set!\n\r");

			}
			else
			{
				printf("HUMIDITY Sensor Error Clear!\n\r");

			}

			if(temp8&SEN54_GAS_ERR)
			{
				printf("GAS Sensor Error Set!\n\r");

			}
			else
			{
				printf("GAS Sensor Error Clear!\n\r");
			}

			

			if(gu8FanCleanF==1)
			{
				gu8FanCleanF=0;
				Sen54Init = 8;
			}	
			else Sen54Init=1;		
		}
	}	
	
	else if(Sen54Init==8)
	{
		
		Sen54Init=1;
		SEN54_StartFanClean();

		if(SEN54_bfackfail==1) Sen54Init=0;
		else
		{			
			SEN54_SetTimer(TIME_2SEC);
		}

	}
	else if(Sen54Init==99)
	{

		SEN54_AutoCleanFanOFF();
		if(SEN54_bfackfail==1) Sen54Init=0;
		else 
		{
			Sen54Init=1;
			SEN54_SetTimer(TIME_2SEC);
			// printf("SEN54_AutoCleanFanOFF OK!\n\r");
		}
	}
	else if(Sen54Init==100)
	{
		PARTICLE_SENSOR_POW = 1; // DUST OFF
		Sen54Init=101;
		SEN54_SetTimer(TIME_2SEC);
		// printf("SEN54 PWR OFF!\n\r");
		SEN54_SDA=0;
		SEN54_SCL=0;


	}
	else if(Sen54Init==101)
	{
		PARTICLE_SENSOR_POW = 0; // DUST ON
		SEN54_SDA=1;
		SEN54_SCL=1;
		Sen54Init=0;
		SEN54_SetTimer(TIME_2SEC);
		// printf("SEN54 PWR ON!\n\r");
	}
  
  }

}

/// @brief      타이머 설정 함수
/// @param      period: 타이머 주기 (ms)
void SEN54_SetTimer(U16 period)
{
    gu16SEN54CheckTimer = 0;
    gu8SEN54TimerFlag = 1;
}

/// @brief      Hal Dust Sensor Sen54 Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 Sen54 센서 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_DustSensor_Sen54_Module_1ms_Control(void)
{
    // 1mSec마다 타이머 카운트 증가
    if (gu16SEN54ControlTimer < SEN54_CONTROL_TIME_PERIOD)
    {
        gu16SEN54ControlTimer++;
    }

    // SEN54 체크 타이머 업데이트
    if (gu8SEN54TimerFlag)
    {
        gu16SEN54CheckTimer++;
    }

    
}

/// @brief      SEN54 온도 값 반환
/// @param      void
/// @return     I16 온도 값 (℃, /200으로 스케일링됨)
/// @details    SEN54 센서에서 측정된 온도 값을 반환
I16 getSen54Temp(void)
{
	return(SEN54_Temp/200);
}

/// @brief      SEN54 습도 값 반환
/// @param      void
/// @return     U16 습도 값 (%RH, /100으로 스케일링됨)
/// @details    SEN54 센서에서 측정된 습도 값을 반환
U16 getSen54Humid(void)
{
	return(SEN54_Humid/100);
}

/// @brief      SEN54 PM1.0 값 반환
/// @param      void
/// @return     U16 PM1.0 농도 값 (μg/m³, /10으로 스케일링됨)
/// @details    SEN54 센서에서 측정된 PM1.0 미세먼지 농도 값을 반환
U16 getSen54PM1_0(void)
{
	return(SEN54_PM1_0/10);
}

/// @brief      SEN54 PM2.5 값 반환
/// @param      void
/// @return     U16 PM2.5 농도 값 (μg/m³, /10으로 스케일링됨)
/// @details    SEN54 센서에서 측정된 PM2.5 미세먼지 농도 값을 반환
U16 getSen54PM2_5(void)
{
	return(SEN54_PM2_5/10);
}

/// @brief      SEN54 PM10 값 반환
/// @param      void
/// @return     U16 PM10 농도 값 (μg/m³, /10으로 스케일링됨)
/// @details    SEN54 센서에서 측정된 PM10 미세먼지 농도 값을 반환
U16 getSen54PM10(void)
{
	return(SEN54_PM10/10);
}

/// @brief      SEN54 VOC 인덱스 값 반환
/// @param      void
/// @return     U16 VOC 인덱스 값 (/10으로 스케일링됨)
/// @details    SEN54 센서에서 측정된 VOC(휘발성 유기화합물) 인덱스 값을 반환
U16 getSen54Voc(void)
{
	return(SEN54_Voc/10);
}

/// @brief      Hal Dust Sensor Sen54 Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 먼지 센서 제어를 한다
/// @param      void
/// @return     void
void Hal_DustSensor_Sen54_Module_Control(void)
{
	if(gu16SEN54ControlTimer >= SEN54_CONTROL_TIME_PERIOD)
    {
        SEN54_Handler();                        // Reed SW 체크 함수
        gu16SEN54ControlTimer = 0;
    }
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor Sen54 Initialize Module
/// @details    Sen54 먼지센서 관련 모든 것을 초기화 시킨다
/// @param      void
/// @return     void
void Hal_DustSensor_Sen54_Module_Initialize(void)
{
#if (DUST_SENSOR_SEN54_USE == USE)
    Sen54Init = 0;                              // Sen54 센서 초기화 상태 리셋
    gu16SEN54ControlTimer = 0;                  // 타이머 초기화
    gu16SEN54CheckTimer = 0;                    // 체크 타이머 초기화
    gu8SEN54TimerFlag = 0;                      // 타이머 플래그 초기화
#endif
}

#endif  // DUST_SENSOR_SEN54_USE == USE

