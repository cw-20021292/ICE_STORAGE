/// @file     Hal_DustSensor_Sen54.c
/// @date     2025/04/10
/// @author   Hyunho Kang
/// @brief    Sensirion SEN54 ���ռ��� ����̹�
/// @details  SEN54 ���� ��� ���� HAL ����̹� ����
	

#include "Global_Header.h"

#if (DUST_SENSOR_SEN54_USE == USE)

// Timer definitions
#define TIME_1SEC           1000    // 1��
#define TIME_2SEC           2000    // 2��
#define TIME_3SEC           3000    // 3��
#define TIME_5SEC           5000    // 5��
#define TIME_10SEC          10000   // 10��
#define TIME_10MSEC         10      // 10ms

static U16 gu16SEN54ControlTimer = 0;
static U16 gu16SEN54CheckTimer = 0;     // SEN54 üũ Ÿ�̸�

static U8 gu8SEN54TimerFlag = 0;        // SEN54 Ÿ�̸� �÷���

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

/// @brief      SEN54 ������ ���� �Լ�
/// @param      mu8Time ���� �ð� (����: WDT ����� �ֱ�)
/// @return     void
/// @details    ������ �ð���ŭ WDT�� ������ϸ鼭 ����
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


/// @brief      I2C Start Condition ����
/// @param      void
/// @return     void
/// @details    I2C ��� ������ ���� Start Condition ��ȣ ����
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

/// @brief      I2C Stop Condition ����
/// @param      void
/// @return     void
/// @details    I2C ��� ���Ḧ ���� Stop Condition ��ȣ ����
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

/// @brief      I2C�κ��� 1����Ʈ ������ �б�
/// @param      void
/// @return     U8 �о�� 1����Ʈ ������
/// @details    I2C ������� �����̺� ����̽��κ��� 1����Ʈ �����͸� �о��
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

/// @brief      I2C ACK ��ȣ ����
/// @param      void
/// @return     void
/// @details    I2C ��ſ��� ������ ���� Ȯ���� ���� ACK ��ȣ ����
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
/// @brief      I2C ACK ��ȣ ���� (��ü ����)
/// @param      void
/// @return     void
/// @details    I2C ��ſ��� ������ ���� Ȯ���� ���� ACK ��ȣ ���� (��ü ����)
void SEN54_I2C_ACK_(void)
{

	SEN54_SDA_LOW();  
	SEN54_SCL_OPEN();
	SEN54_Delay_E(SEN54_Term);
	SEN54_SCL_LOW();
	SEN54_Delay_E(SEN54_Term);
	SEN54_SDA_OPEN();  


}

/// @brief      I2C NACK ��ȣ ����
/// @param      void
/// @return     void
/// @details    I2C ��ſ��� ������ ���� ���Ḧ ���� NACK ��ȣ ����
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

/// @brief      SEN54 CRC üũ�� ���
/// @param      Data1 ù ��° ������ ����Ʈ
/// @param      Data2 �� ��° ������ ����Ʈ  
/// @return     U8 ���� CRC ��
/// @details    SEN54 ������ 2����Ʈ �����Ϳ� ���� CRC üũ�� ���
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


/// @brief      I2C�� 1����Ʈ ������ ����
/// @param      mu8WriteByte ������ 1����Ʈ ������
/// @return     void
/// @details    I2C ������� �����̺� ����̽��� 1����Ʈ �����͸� �����ϰ� ACK Ȯ��
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


/// @brief      SEN54 Raw ������ �б� ��� ���� (1�ܰ�)
/// @param      void
/// @return     void
/// @details    SEN54 ������ Raw ������ �б� ���(0x03D2)�� ����
void SEN54_ReadRawdata1(void)
{
	SEN54_bfackfail=0;	
	
	SEN54_I2c_StartCondition();
	SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x03);}  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xD2);}
	SEN54_I2c_StopCondition();
	

}

/// @brief      SEN54 Raw ������ �б� (2�ܰ�)
/// @param      void
/// @return     void
/// @details    SEN54 �����κ��� Raw �����͸� �о�ͼ� VOC Raw �� ������Ʈ
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



/// @brief      SEN54 ����̽� ���� Ŭ����
/// @param      void
/// @return     void
/// @details    SEN54 ������ ����̽� ���¸� Ŭ�����ϴ� ���(0xD210) ����
void SEN54_DeviceStateClr(void)
{
	SEN54_I2c_StartCondition();
	SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xD2);}  
	if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x10);}
	SEN54_I2c_StopCondition();
}

/// @brief      SEN54 ����̽� ���� �б� ��� ���� (1�ܰ�)
/// @param      void
/// @return     void
/// @details    SEN54 ������ ����̽� ���� �б� ���(0xD206)�� ����
void SEN54_DeviceStateRead_1(void)
{
	
	
	 SEN54_bfackfail=0;	 
	
	 SEN54_I2c_StartCondition();
	 SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);	
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xD2);}	
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x06);}
	 SEN54_I2c_StopCondition();
 

}

/// @brief      SEN54 ����̽� ���� �б� (2�ܰ�)
/// @param      void
/// @return     void
/// @details    SEN54 �����κ��� ����̽� ���� �����͸� �о��
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

/// @brief      SEN54 ���� ������ �б� ��� ���� (1�ܰ�)
/// @param      void
/// @return     void
/// @details    SEN54 ������ ���� ������ �б� ���(0x03C4)�� ����
void SEN54_ReadMeasure_1(void)
{
	  SEN54_I2c_StartCondition();
	  SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
	
	  if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x03);}
	
	  if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0xc4);}
	
	 SEN54_I2c_StopCondition();

}

/// @brief      SEN54 ���� ������ �б� (2�ܰ�)
/// @param      void
/// @return     void
/// @details    SEN54 �����κ��� PM, �½���, VOC �� ��� ���� �����͸� �о��
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



/// @brief      SEN54 �ڵ� �� û�� ��� ��Ȱ��ȭ
/// @param      void
/// @return     void
/// @details    SEN54 ������ �ڵ� �� û�� ����� ��Ȱ��ȭ�ϴ� ��� ����
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

/// @brief      SEN54 ���� ����
/// @param      void
/// @return     void
/// @details    SEN54 ������ ���� ���� ���(0x0021)�� ����
void SEN54_StartMeasurement(void)
{
	 SEN54_I2c_StartCondition();
	 SEN54_IIC_Byte_Write(SEN54_ADDR_WRITE);
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x00);}
	 if(!SEN54_bfackfail){SEN54_IIC_Byte_Write(0x21);}
	 SEN54_I2c_StopCondition();
	
	
	if(SEN54_bfackfail==1)printf("SEN54_StartMeasurement ERROR!\n\r");	
}


/// @brief      SEN54 �� û�� ����
/// @param      void
/// @return     void
/// @details    SEN54 ������ �� û�Ҹ� �����ϴ� ���(0x5607) ����
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

/// @brief      SEN54 ���� ���� �ڵ鷯
/// @param      void
/// @return     void
/// @details    SEN54 ������ ���¿� ���� �ʱ�ȭ, ����, ���� ó�� ���� �����ϴ� ���� ���� �Լ�
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
	        HumidityEnable(); //5������ ��Ʈ����
	      }
	      else
	      {
	        gu8HumiErrCnt = 0;
	        //�������� ���� �߻�
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
		  calHumiCRC = checkHumidityCRC();  //CRC üũ
		  calTempCRC = checkTempCRC();      //CRC üũ
		  
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

/// @brief      Ÿ�̸� ���� �Լ�
/// @param      period: Ÿ�̸� �ֱ� (ms)
void SEN54_SetTimer(U16 period)
{
    gu16SEN54CheckTimer = 0;
    gu8SEN54TimerFlag = 1;
}

/// @brief      Hal Dust Sensor Sen54 Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� Sen54 ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_DustSensor_Sen54_Module_1ms_Control(void)
{
    // 1mSec���� Ÿ�̸� ī��Ʈ ����
    if (gu16SEN54ControlTimer < SEN54_CONTROL_TIME_PERIOD)
    {
        gu16SEN54ControlTimer++;
    }

    // SEN54 üũ Ÿ�̸� ������Ʈ
    if (gu8SEN54TimerFlag)
    {
        gu16SEN54CheckTimer++;
    }

    
}

/// @brief      SEN54 �µ� �� ��ȯ
/// @param      void
/// @return     I16 �µ� �� (��, /200���� �����ϸ���)
/// @details    SEN54 �������� ������ �µ� ���� ��ȯ
I16 getSen54Temp(void)
{
	return(SEN54_Temp/200);
}

/// @brief      SEN54 ���� �� ��ȯ
/// @param      void
/// @return     U16 ���� �� (%RH, /100���� �����ϸ���)
/// @details    SEN54 �������� ������ ���� ���� ��ȯ
U16 getSen54Humid(void)
{
	return(SEN54_Humid/100);
}

/// @brief      SEN54 PM1.0 �� ��ȯ
/// @param      void
/// @return     U16 PM1.0 �� �� (��g/m��, /10���� �����ϸ���)
/// @details    SEN54 �������� ������ PM1.0 �̼����� �� ���� ��ȯ
U16 getSen54PM1_0(void)
{
	return(SEN54_PM1_0/10);
}

/// @brief      SEN54 PM2.5 �� ��ȯ
/// @param      void
/// @return     U16 PM2.5 �� �� (��g/m��, /10���� �����ϸ���)
/// @details    SEN54 �������� ������ PM2.5 �̼����� �� ���� ��ȯ
U16 getSen54PM2_5(void)
{
	return(SEN54_PM2_5/10);
}

/// @brief      SEN54 PM10 �� ��ȯ
/// @param      void
/// @return     U16 PM10 �� �� (��g/m��, /10���� �����ϸ���)
/// @details    SEN54 �������� ������ PM10 �̼����� �� ���� ��ȯ
U16 getSen54PM10(void)
{
	return(SEN54_PM10/10);
}

/// @brief      SEN54 VOC �ε��� �� ��ȯ
/// @param      void
/// @return     U16 VOC �ε��� �� (/10���� �����ϸ���)
/// @details    SEN54 �������� ������ VOC(�ֹ߼� ����ȭ�չ�) �ε��� ���� ��ȯ
U16 getSen54Voc(void)
{
	return(SEN54_Voc/10);
}

/// @brief      Hal Dust Sensor Sen54 Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� ���� ���� ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_DustSensor_Sen54_Module_Control(void)
{
	if(gu16SEN54ControlTimer >= SEN54_CONTROL_TIME_PERIOD)
    {
        SEN54_Handler();                        // Reed SW üũ �Լ�
        gu16SEN54ControlTimer = 0;
    }
}

// Hal Dust Sensor Module
//******************************************************************************************

/// @brief      Hal Dust Sensor Sen54 Initialize Module
/// @details    Sen54 �������� ���� ��� ���� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_DustSensor_Sen54_Module_Initialize(void)
{
#if (DUST_SENSOR_SEN54_USE == USE)
    Sen54Init = 0;                              // Sen54 ���� �ʱ�ȭ ���� ����
    gu16SEN54ControlTimer = 0;                  // Ÿ�̸� �ʱ�ȭ
    gu16SEN54CheckTimer = 0;                    // üũ Ÿ�̸� �ʱ�ȭ
    gu8SEN54TimerFlag = 0;                      // Ÿ�̸� �÷��� �ʱ�ȭ
#endif
}

#endif  // DUST_SENSOR_SEN54_USE == USE

