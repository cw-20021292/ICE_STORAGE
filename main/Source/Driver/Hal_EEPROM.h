/// @file     Hal_EEPROM.h
/// @date     2025/03/24
/// @author   Jaejin Ham
/// @brief    EEPROM ���� ���� ���� file

#ifndef _HAL_EEPROM_H_
#define _HAL_EEPROM_H_


//****************************************************************************
//  EEPROM ���� - EEPROM�� RTC�� �Ѵ� ��� ������� �����ϸ� �ȵ�(����)
//  EEPROM IC - AT24C02
//  EEPROM ��� ��� Port - IICA0 or GPIO
//****************************************************************************
/*  EEPROM ���� Function ����
    => u8Return = Get_EepromErrorCheck();           // EEPROM ��� �� ������ Read ���� �߻� üũ Ȯ�� - 0(���� �̹߻�), 1(���� �߻�)
    => Set_EEPROM_Data(EEPROM_DATA_ID, Data);       // EEPROM ID(Address)�� Data�� ���� �����ϴ� ID�� ���� Data�� ���� ó��
    => u8Return = Get_EEPROM_Data(EEPROM_DATA_ID);  // ID�� �ش��ϴ� EEPROM Read Data Ȯ��

    ��� ��)
    mu8Return = Get_EepromErrorCheck();             // EEPROM�� �̻��� �ִ��� Ȯ��
    Set_EEPROM_Data(EEPROM_DATA_ID, 1);             // EEPROM ID(Address)�� 1�� ���� �����ϴ� ID�� ���� Data�� 1�� ó��
    mu8Return = Get_EEPROM_Data(EEPROM_DATA_ID);    // ID�� �ش��ϴ� EEPROM Read Data Ȯ��
*/


#define EEPROM_USE                                      NO_USE     // EEPROM ��� ���� ���� - 0(No Use), 1(Use)

#if (EEPROM_USE == USE)

#define EEPROM_8BIT                                     0
#define EEPROM_16BIT                                    1
#define EEPROM_ADDR_BIT_SETTING                         EEPROM_8BIT     // EEPROM�� Address�� Bit �� ���� - 0(8bit), 1(16bit)

// EEPROM Default Data Define
#define VALUE_DEFINE_EEPROM_DATA_START                  0xAA
#define VALUE_DEFINE_EEPROM_DATA_END                    0xA5

#define DEV_ADDR_EEP                                    0xA0    // Device Address EEPROM
#define EEP_OP_WR                                       0x00
#define EEP_OP_RD                                       0x01

#define EEP_MAX_SIZE		                            256     // bytes
#define EEP_PAGE_SIZE	                                16      // 16-byte page write
#define EEP_PAGE_NUM	                                16      // 256 bytes / 16 page per bytes = 16 pages

#define EEPROM_IIC_SFR                                  0
#define EEPROM_IIC_GPIO                                 1
#define EEPROM_IIC_GPIO_USE                             EEPROM_IIC_SFR  // EEPROM ����� GPIO�� ���� ���� ��� ���� - 0(SFR Use), 1(GPIO Use)

#define EEPROM_IIC_OK                                   1
#define EEPROM_IIC_FAIL                                 0

#define EEPROM_IIC_RETRY_COUNT                          5     // EEPROM ������ ���� ��õ� �ִ� Ƚ��


#if (EEPROM_IIC_GPIO_USE == EEPROM_IIC_GPIO)      // IIC����� GPIO�� ���� ���� �ϴ� ���

#define IIC_SCL_PORT_MODE_REGISTER                      PM6_bit.no0
#define IIC_SDA_PORT_MODE_REGISTER                      PM6_bit.no1
#define P_IIC_SCL                                       P6_bit.no0
#define P_IIC_SDA                                       P6_bit.no1
#define IIC_PIN_MODE_INPUT                              1
#define IIC_PIN_MODE_OUTPUT                             0


void Delay_E(U8 mu8Time);
void IIC_Start(void);
void IIC_Stop(void);
void BitOut(void);
void BitIn(void);
void IIC_TXD(void);
void IIC_RXD(void);

#if (EEPROM_ADDR_BIT_SETTING == EEPROM_8BIT)                  // Address�� 8bit
U8 Hal_IIC_ByteWrite(U8 mu8Dev, U8 mu8Addr, U8 mu8Data);
U8 IIC_ByteWrite(U8 mu8Dev, U8 mu8Addr, U8 mu8Data);
U8 Hal_IIC_PageWrite(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len);
U8 IIC_PageWrite(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len);
U8 Hal_IIC_ByteRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data);
U8 IIC_ByteRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data);
U8 Hal_IIC_SeqRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len);
U8 IIC_SeqRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len);
#else
U8 Hal_IIC_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data);
U8 IIC_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data);
U8 Hal_IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 Hal_IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data);
U8 IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data);
U8 Hal_IIC_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 IIC_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
#endif

#else                               // IIC����� IICA0���� ����ϴ� ���

#define EEP_PROC_START		                            0
#define EEP_PROC_DONE		                            1

#define IICA0_MASTER_SEND_FUCTION                       R_Config_IICA0_Master_Send      // Code Generate�� IICA0 Master Send �Լ� ��ġ
#define IICA0_MASTER_RECEIVE_FUCTION                    R_Config_IICA0_Master_Receive   // Code Generate�� IICA0 Master Receive �Լ� ��ġ
#define IICA0_Start()                                   {IICE0 = 1;}    // Enable IICA0 operation


void EEPROM_IICA0_WriteProcess_Done(void);
void EEPROM_IICA0_ReadProcess_Done(void);
void IICA0_WriteCycleTime(void);
#if (EEPROM_ADDR_BIT_SETTING == EEPROM_8BIT)                  // Address�� 8bit
U8 Hal_IICA0_ByteWrite(U8 mu8Dev, U8 mu8Addr, U8 mu8Data);
U8 ByteWrite(U8 mu8Dev, U8 mu8Addr, U8 mu8Data);
U8 Hal_IICA0_PageWrite(U8 mu8Dev, U8 mu8Addr , U8* pu8Data, U8 mu8Len);
U8 PageWrite(U8 mu8Dev, U8 mu8Addr , U8* pu8Data, U8 mu8Len);
U8 Hal_IICA0_ByteRead(U8 mu8Dev, U8 mu8Addr , U8* pu8Data);
U8 ByteRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data);
U8 Hal_IICA0_SeqRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len);
U8 SeqRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len);
#else
U8 Hal_IICA0_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data);
U8 ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data);
U8 Hal_IICA0_PageWrite(U8 mu8Dev, U16 mu16Addr , U8* pu8Data, U8 mu8Len);
U8 PageWrite(U8 mu8Dev, U16 mu16Addr , U8* pu8Data, U8 mu8Len);
U8 Hal_IICA0_ByteRead(U8 mu8Dev, U16 mu16Addr , U8* pu8Data);
U8 ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data);
U8 Hal_IICA0_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
#endif

U8 IICA0_Stop_Condition_Generate(void);

#endif


// �����ϰ� ����ϴ� �Լ��� Define ����
#if (EEPROM_IIC_GPIO_USE == EEPROM_IIC_GPIO)      // IIC����� GPIO�� ���� ���� �ϴ� ���
#define HAL_IIC_BYTE_WRITE                              Hal_IIC_ByteWrite
#define IIC_BYTE_WRITE                                  IIC_ByteWrite
#define HAL_IIC_PAGE_WRITE                              Hal_IIC_PageWrite
#define IIC_PAGE_WRITE                                  IIC_PageWrite
#define HAL_IIC_BYTE_READ                               Hal_IIC_ByteRead
#define IIC_BYTE_READ                                   IIC_ByteRead
#define HAL_IIC_SEQ_READ                                Hal_IIC_SeqRead
#define IIC_SEQ_READ                                    IIC_SeqRead
#else
#define HAL_IIC_BYTE_WRITE                              Hal_IICA0_ByteWrite
#define IIC_BYTE_WRITE                                  ByteWrite
#define HAL_IIC_PAGE_WRITE                              Hal_IICA0_PageWrite
#define IIC_PAGE_WRITE                                  PageWrite
#define HAL_IIC_BYTE_READ                               Hal_IICA0_ByteRead
#define IIC_BYTE_READ                                   ByteRead
#define HAL_IIC_SEQ_READ                                Hal_IICA0_SeqRead
#define IIC_SEQ_READ                                    SeqRead
#endif


void EEPROM_Data_Load(void);

#if (EEPROM_ADDR_BIT_SETTING == EEPROM_8BIT)                  // Address�� 8bit
void EepromByteWrite(U8 mu8ID, U8 mu8Data);
void EepromByteRead(U8 mu8ID);
void EepromPageWrite(U8 mu8Addr, U8* pu8Data, U8 mu8Len);
void EepromSeqRead(U8 mu8Addr, U8* pu8Data, U8 mu8Len);
#else
void EepromByteWrite(U16 mu16ID, U8 mu8Data);
void EepromByteRead(U16 mu16ID);
void EepromPageWrite(U16 mu16Addr, U8* pu8Data, U8 mu8Len);
void EepromSeqRead(U16 mu16Addr, U8* pu8Data, U8 mu8Len);
#endif

U8 Get_EepromErrorCheck(void);
void Set_EEPROM_Data(U8 mu8ID, U8 mu8Data);
void Save_EEPROM_Data(U8 mu8ID);
U8 Get_EEPROM_Data(U8 mu8ID);

#endif

#endif
