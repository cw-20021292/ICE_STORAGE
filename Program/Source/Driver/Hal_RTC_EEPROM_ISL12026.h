/// @file     Hal_RTC_EEPROM_ISL12026.h
/// @date     2025/03/24
/// @author   Jaejin Ham
/// @brief    RTC(EEPROM 포함) 제어 동작 관련 file

#ifndef _HAL_RTC_EEPROM_H_
#define _HAL_RTC_EEPROM_H_


//****************************************************************************
//  RTC(ISL12026A) 설정 -  EEPROM과 RTC를 둘다 모두 사용으로 설정하면 안됨(주의)
//  RTC, EEPROM IC - ISL12026A
//  EEPROM 통신 사용 Port - IICA0 or GPIO
//****************************************************************************

/*  RTC 제어 Function 사용법
    => Set_RTC_Time(SRTC_TimeData_T* pSTime);       // 포인터 pStime 위치에 입력된 시간 데이터롤 RTC에 Write
    => Get_RTC_Time(SRTC_TimeData_T* pSTime);       // 포인터 pStime 위치에 Read한 시간 데이터를 업데이트

    pSTime의 데이터 구조
        typedef struct {
            U8 gu8Y2K;              // 19 or 20
            U8 gu8DayWeek;          // 0 ~ 6
            U8 gu8Year;             // 0 ~ 99
            U8 gu8Month;            // 1 ~ 12
            U8 gu8Date;             // 1 ~ 31
            U8 gu8Hour;             // 0 ~ 23
            U8 gu8Min;              // 0 ~ 59
            U8 gu8Sec;              // 0 ~ 59
        }   SRTC_TimeData_T;
*/

/*  EEPROM 제어 Function 사용법
    => Get_EepromErrorCheck();                      // EEPROM 통신 및 데이터 Read 에러 발생 체크 확인 - 0(에러 미발생), 1(에러 발생)
    => Set_EEPROM_Data(EEPROM_DATA_ID, Data);       // EEPROM ID(Address)에 Data를 쓰고 관리하는 ID에 대한 Data를 변경 처리
    => Get_EEPROM_Data(EEPROM_DATA_ID);             // ID에 해당하는 EEPROM Read Data 확인

    사용 예)
    u8Return = Get_EepromErrorCheck();
    Set_EEPROM_Data(EEPROM_DATA_ID, 1);             // EEPROM ID(Address)에 1을 쓰고 관리하는 ID에 대한 Data를 1로 처리
    u8Return = Get_EEPROM_Data(EEPROM_DATA_ID);     // ID에 해당하는 EEPROM Read Data 확인
*/


#define RTC_USE                                         USE      // RTC() 사용 여부 설정 - 0(No Use), 1(Use)

#if (RTC_USE == USE)

#define DEV_ADDR_EEP                                    0xAE    // Device EEPROM - ISL12026
#define DEV_ADDR_RTC                                    0xDE    // Device RTC - ISL12026

#define RTC_OP_WR                                       0x00
#define RTC_OP_RD                                       0x01

#define RTC_MAX_SIZE		                            512     // bytes
#define RTC_PAGE_SIZE	                                16      // 16-byte page write
#define RTC_PAGE_NUM	                                32      // 512 bytes / 16 page per bytes = 32 pages

#define RTC_IIC_SFR                                     0
#define RTC_IIC_GPIO                                    1
#define RTC_IIC_GPIO_USE                                RTC_IIC_SFR     // RTC 통신을 GPIO로 직접 제어 사용 여부 - 0(SFR Use), 1(GPIO Use)

#define RTC_IIC_OK                                      1
#define RTC_IIC_FAIL                                    0

#define RTC_IIC_RETRY_COUNT                             5     // EEPROM 데이터 쓰기 재시도 최대 횟수

#if (RTC_IIC_GPIO_USE == RTC_IIC_GPIO)                          // IIC통신을 GPIO로 만들어서 동작 하는 경우

#define IIC_SCL_PORT_MODE_REGISTER                      PM6_bit.no0
#define IIC_SDA_PORT_MODE_REGISTER                      PM6_bit.no1
#define P_IIC_SCL                                       P6_bit.no0
#define P_IIC_SDA                                       P6_bit.no1
#define IIC_PIN_MODE_INPUT                              1
#define IIC_PIN_MODE_OUTPUT                             0

#else                                                   // IIC통신을 IICA0으로 사용하는 경우

#define RTC_PROC_START		                            0
#define RTC_PROC_DONE		                            1

#define IICA0_MASTER_SEND_FUCTION                       R_Config_IICA0_Master_Send      // Code Generate된 IICA0 Master Send 함수 배치
#define IICA0_MASTER_RECEIVE_FUCTION                    R_Config_IICA0_Master_Receive   // Code Generate된 IICA0 Master Receive 함수 배치
#define IICA0_Start()                                   {IICE0 = 1;}    // Enable IICA0 operation

#endif

/* MASK BIT OF STATUS REGISTER */
#define  MASK_BAT                                       0x80
#define  MASK_AL1                                       0x40
#define  MASK_AL0                                       0x20
#define  MASK_OSCF                                      0x10
#define  MASK_RWEL                                      0x04
#define  MASK_WEL                                       0x02
#define  MASK_RTCF                                      0x01

#define  MASK_MIL                                       0x80

/* RTC Memory Map  ----------------------------------------------------------*/
// RTC ADDRESS
#define ADDR_RTC_STATUS                                 0x003F
#define ADDR_RTC_Y2K                                    0x0037
#define ADDR_RTC_DAY                                    0x0036
#define ADDR_RTC_YEAR                                   0x0035
#define ADDR_RTC_MONTH                                  0x0034
#define ADDR_RTC_DATE                                   0x0033
#define ADDR_RTC_HOURS                                  0x0032
#define ADDR_RTC_MINUTES                                0x0031
#define ADDR_RTC_SECONDS                                0x0030

// CONTROL ADDRESS
#define ADDR_CTR_PWR                                    0x0014  // Power
#define ADDR_CTR_DTR                                    0x0013  // Digital Trimming
#define ADDR_CTR_ATR                                    0x0012  // Analog Trimming
#define ADDR_CTR_INT                                    0x0011  // Interrupt Control & Freq Output
#define ADDR_CTR_BL                                     0x0010  // Block Protect Bits

#define RTC_READ_TIME                                   20


// RTC 관리 자료형
typedef struct {
    U8 gu8Y2K;              // 19 or 20
    U8 gu8DayWeek;          // 0 ~ 6
    U8 gu8Year;             // 0 ~ 99
    U8 gu8Month;            // 1 ~ 12
    U8 gu8Date;             // 1 ~ 31
    U8 gu8Hour;             // 0 ~ 23
    U8 gu8Min;              // 0 ~ 59
    U8 gu8Sec;              // 0 ~ 59
}   SRTC_TimeData_T;

typedef struct _real_time_clock_
{
    U8 gu8InitSuccess;
    /* Buffer */
    U8 au8Readbuf1[8];
    U8 au8Readbuf2[8];
    U8 au8writeBuf[8];
    /* STATUS REGISTER */
    U8 gu8RegStatus;
    U8 gu8RegPower;
}   SRealTimeClock_T;


#if (RTC_IIC_GPIO_USE == RTC_IIC_GPIO)      // IIC통신을 GPIO로 만들어서 동작 하는 경우

void Delay_E(U8 mu8Time);
void IIC_Start(void);
void IIC_Stop(void);
void BitOut(void);
void BitIn(void);
void IIC_TXD(void);
void IIC_RXD(void);

U8 Hal_IIC_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data);
U8 IIC_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data);
U8 Hal_IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 Hal_IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data);
U8 IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data);
U8 Hal_IIC_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 IIC_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);

#else

void EEPROM_IICA0_WriteProcess_Done(void);
void EEPROM_IICA0_ReadProcess_Done(void);
void IICA0_WriteCycleTime(void);
U8 Hal_IICA0_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data);
U8 ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data);
U8 Hal_IICA0_PageWrite(U8 mu8Dev, U16 mu16Addr , U8* pu8Data, U8 mu8Len);
U8 PageWrite(U8 mu8Dev, U16 mu16Addr , U8* pu8Data, U8 mu8Len);
U8 Hal_IICA0_ByteRead(U8 mu8Dev, U16 mu16Addr , U8* pu8Data);
U8 ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data);
U8 Hal_IICA0_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 IICA0_Stop_Condition_Generate(void);

#endif

// 동일하게 사용하는 함수들 Define 정의
#if (RTC_IIC_GPIO_USE == RTC_IIC_GPIO)      // IIC통신을 GPIO로 만들어서 동작 하는 경우
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


#define RTC_SETTING_MAX_YEAR                            80
#define RTC_SETTING_MIN_YEAR                            25

#define RTC_DEFAULT_YEAR                                RTC_SETTING_MIN_YEAR
#define RTC_DEFAULT_MONTH                               1
#define RTC_DEFAULT_DATE                                1
#define RTC_DEFAULT_HUOR                                0
#define RTC_DEFAULT_MIN                                 0
#define RTC_DEFAULT_SEC                                 0
#define RTC_DEFAULT_DAYWEEK                             3
#define RTC_DEFAULT_Y2K                                 20

#define DW_MON  0
#define DW_TUE  1
#define DW_WED  2
#define DW_THU  3
#define DW_FRI  4
#define DW_SAT  5
#define DW_SUN  6

#define RTC_PROCESS_CONTROL_TIME_PERIOD                 1000         // 1Sec@1ms


// RTC 관련 함수
void RTC_Delay_ms(U16 mu16ms);
void RTC_Delay_us(U8 mu8us);
void RTC_Initialize(void);
static void EnableRtcCCR(void);
static void DisableRtcCCR(void);
U8 IsRTC_Reset(void);
void RTC_ProcessControlTimer(void);
void ProcessRTC(void);
U8 RTC_OverMaxYear(void);
U8 IsRTCBatBackUpMode(void);
void ReadRTC_Time(void);
void WriteRTC_Time(void);
void InitRTC_Time(void);
U8 IsRTC_Error(void);
U8 IsRTC_OSCF_Error(void);
void RTC_ErrorCheck(void);
void Set_RTC_Time(SRTC_TimeData_T* pSTime);
void Get_RTC_Time(SRTC_TimeData_T* pSTime);


// EEPROM Default Data Define
#define VALUE_DEFINE_EEPROM_DATA_START                  0xAA
#define VALUE_DEFINE_EEPROM_DATA_END                    0xA5


// EEPROM 관련 함수
void EEPROM_Data_Load(void);
void EepromByteWrite(U16 mu16ID, U8 mu8Data);
void EepromByteRead(U16 mu16ID);
void EepromPageWrite(U16 mu16Addr, U8* pu8Data, U8 mu8Len);
void EepromSeqRead(U16 mu16Addr, U8* pu8Data, U8 mu8Len);
U8 Get_EepromErrorCheck(void);
void Set_EEPROM_Data(U8 mu8ID, U8 mu8Data);
void Save_EEPROM_Data(U8 mu8ID);
U8 Get_EEPROM_Data(U8 mu8ID);

#endif


void Hal_RTC_Module_Initialize(void);
void Hal_RTC_Module_1ms_Control(void);
void Hal_RTC_Module_Control(void);

#endif
