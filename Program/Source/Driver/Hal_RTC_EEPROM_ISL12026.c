/// @file     Hal_RTC_EEPROM_ISL12026.c
/// @date     2025/03/24
/// @author   Jaejin Ham
/// @brief    RTC(EEPROM 포함) 제어 동작 관련 file

#include  "Global_Header.h"


#if (RTC_USE == USE)

//****************************************************************************
//   RTC 통신 사용 Port - IICA0 or GPIO
//****************************************************************************

// RTC, EEPRON 통신 IIC 제어 ---------------------------------------------------------------

#if (RTC_IIC_GPIO_USE == RTC_IIC_GPIO)      // IIC통신을 GPIO로 만들어서 동작 하는 경우

U8 gu8Txbit;                                /// @brief  I2C 송신시 Data값 bit
U8 gu8Rxbit;                                /// @brief  I2C 수신시 Data값 bit
U8 gu8AckFail;                              /// @brief  I2C 수신시 Ack 체크 Flag

U8 gu8IIC_WriteData = 0;                    /// @brief  I2C 송신 데이터
U8 gu8IIC_ReadData = 0;                     /// @brief  I2C 수신 데이터


/// @brief      IIC 통신시 Pulse 간 delay
/// @details    IIC 통신시 Pulse 발생를 위한 Delay 제어를 한다
/// @param      mu8Time : delay time counter
/// @return     void
void Delay_E(U8 mu8Time)
{
    while (mu8Time)
    {
        WDT_Restart();

        mu8Time--;
    }
}


/// @brief      IIC Start Bit
/// @details    IIC Start Bit 파형을 발생시킨다
/// @param      void
/// @return     void
void IIC_Start(void)
{
    Delay_E(20);
    IIC_SDA_PORT_MODE_REGISTER = IIC_PIN_MODE_OUTPUT;    // Set data line for output
    P_IIC_SDA = 1;                              // Data is high
    Delay_E(14);                                // Waits to keep the high state on SCL

    //---------------
    // send start-bit
    //---------------

    P_IIC_SCL = 0;                              // make sure clock is low
    Delay_E(4);
    P_IIC_SCL = 1;                              // set clock high
    Delay_E(4);
    P_IIC_SDA = 0;                              // data line goes low during high clock for start bit
    Delay_E(4);
    P_IIC_SCL = 0;
    Delay_E(2);
}


/// @brief      IIC Stop Bit
/// @details    IIC Stop Bit 파형을 발생시킨다
/// @param      void
/// @return     void
void IIC_Stop(void)
{
    IIC_SDA_PORT_MODE_REGISTER = IIC_PIN_MODE_OUTPUT;  // Set data line for output
    Delay_E(10);                              // Waits to keep the high state on SCL

    //---------------
    // send stop-bit
    //---------------
    P_IIC_SDA = 0;                            // make sure data line is low
    Delay_E(4);
    P_IIC_SCL = 1;                            // set clock high
    Delay_E(4);
    P_IIC_SDA = 1;                            // data goes high while clock high for stop bit
    Delay_E(4);
    P_IIC_SCL = 0;
    Delay_E(2);
}


/// @brief      IIC Data Bit Output
/// @details    IIC Data에 따른 Bit 파형을 발생시킨다
/// @param      void
/// @return     void
void BitOut(void)
{
    IIC_SDA_PORT_MODE_REGISTER = IIC_PIN_MODE_OUTPUT;    // Set data line for output
    Delay_E(10);                                // Waits to keep the high state on SCL

    if (gu8IIC_WriteData & 0x80)                // check for state of data bit to xmit
    {
        P_IIC_SDA = 1; // set data line high
    }
    else
    {
        P_IIC_SDA = 0;                            // output a low bit
    }
                                              // go toggle the clock
    P_IIC_SCL = 1;                              // set clock line high
    Delay_E(4);
    P_IIC_SCL = 0;                              // return clock line low
}


/// @brief      IIC Data Bit Input
/// @details    IIC Data의 Bit Input 체크 제어를 한다
/// @param      void
/// @return     void
void BitIn(void)
{
    IIC_SDA_PORT_MODE_REGISTER = IIC_PIN_MODE_INPUT;   // Set data line for Input
    Delay_E(4);

    P_IIC_SCL = 1;                            // set clock line high
    Delay_E(4);

    if (P_IIC_SDA)
    {
        gu8IIC_ReadData += 0x01;
        gu8Rxbit = 1;
    }
    else
    {
        gu8IIC_ReadData += 0x00;
        gu8Rxbit = 0;
    }

    P_IIC_SCL = 0;                             // return clock line low
}


/// @brief      IIC Data Transfer(Write)
/// @details    IIC Data 송신 제어 파형 제어를 한다
/// @param      void
/// @return     void
void IIC_TXD(void)
{
    U8 mu8Count = 0;

    for (mu8Count = 0 ; mu8Count < 8 ; mu8Count++)
    {
        BitOut();     // send the bit to serial EE

        gu8IIC_WriteData = gu8IIC_WriteData << 1;
    }

    BitIn();            // read ack bit

    if (gu8Rxbit)
    {
        gu8AckFail = 1;
    }
}


/// @brief      IIC Data Receive(Read)
/// @details    IIC Data 수신 제어 파형 제어를 한다
/// @param      void
/// @return     void
void IIC_RXD(void)
{
    U8 mu8Count = 0;

    gu8IIC_ReadData = 0;

    for (mu8Count = 0 ; mu8Count < 8 ; mu8Count++)
    {
        gu8IIC_ReadData = gu8IIC_ReadData << 1;

        BitIn();
    }
}


/// @brief      IIC Byte Write Process
/// @details    IIC Byte Write 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             mu8Data : Write Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_ByteWrite(mu8Dev, mu16Addr, mu8Data);

        if (mu8Status == RTC_IIC_OK)
        {
            return RTC_IIC_OK;
        }
        else
        {
        }
    }

    return RTC_IIC_FAIL;
}


/// @brief      IIC Byte Write Process-1
/// @details    IIC Byte Write 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             mu8Data : Write Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data)
{
    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | RTC_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)((mu16Addr >> 8) & 0x00FF);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)(mu16Addr & 0x00FF);;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = mu8Data;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Stop();

    Delay_ms(30);

    return  RTC_IIC_OK;
}


/// @brief      IIC Page Write Process
/// @details    IIC Page Write 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             pu8Data : Write Data
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_PageWrite(mu8Dev, mu16Addr, pu8Data, mu8Len);

        if (mu8Status == RTC_IIC_OK)
        {
            return RTC_IIC_OK;
        }
        else
        {
        }
    }

    return RTC_IIC_FAIL;
}


/// @brief      IIC Page Write Process-1
/// @details    IIC Page Write 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             pu8Data : Write Data
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;

    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | RTC_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)((mu16Addr >> 8) & 0x00FF);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)(mu16Addr & 0x00FF);;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    // 데이터 송신
    for (mu8Cnt = 0 ; mu8Cnt < mu8Len ; mu8Cnt++)
    {
        WDT_Restart();
        gu8IIC_WriteData = *pu8Data;
        IIC_TXD();
        if(gu8AckFail)
        {
            return  RTC_IIC_FAIL;
        }

        pu8Data++;
    }

    WDT_Restart();
    IIC_Stop();

    Delay_ms(30);

    return  RTC_IIC_OK;
}


/// @brief      IIC Byte Read Process
/// @details    IIC Byte Read 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_ByteRead(mu8Dev, mu16Addr, pu8Data);

        if (mu8Status == RTC_IIC_OK)
        {
            return RTC_IIC_OK;
        }
        else
        {
        }
    }

    return RTC_IIC_FAIL;
}


/// @brief      IIC Byte Read Process-1
/// @details    IIC Byte Read 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data)
{
    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    gu8IIC_WriteData = (mu8Dev | RTC_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)((mu16Addr >> 8) & 0x00FF);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)(mu16Addr & 0x00FF);;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | RTC_OP_RD);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    IIC_RXD();
    *pu8Data = gu8IIC_ReadData;

    IIC_Stop();

    return  RTC_IIC_OK;
}



/// @brief      IIC Sequential Read Process
/// @details    IIC Page Read 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_SeqRead(mu8Dev, mu16Addr, pu8Data, mu8Len);

        if (mu8Status == RTC_IIC_OK)
        {
            return RTC_IIC_OK;
        }
        else
        {
        }
    }

    return RTC_IIC_FAIL;
}


/// @brief      IIC Sequential Read Process-1
/// @details    IIC Page Read 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;

    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    gu8IIC_WriteData = (mu8Dev | RTC_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)((mu16Addr >> 8) & 0x00FF);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)(mu16Addr & 0x00FF);;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | RTC_OP_RD);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  RTC_IIC_FAIL;
    }

    for (mu8Cnt = 0 ; mu8Cnt < mu8Len ; mu8Cnt++)
    {
        WDT_Restart();
        IIC_RXD();

        *pu8Data = gu8IIC_ReadData;
        pu8Data++;
    }

    IIC_Stop();

    return  RTC_IIC_OK;
}


#else

U8 gu8IICA0_WriteProc = 0;
U8 gu8IICA0_WriteFailCnt = 0;
U8 gu8IICA0_WriteOkCnt = 0;
U8 gu8IICA0_ReadProc = 0;
U8 gu8IICA0_ReadFailCnt = 0;
U8 gu8IICA0_ReadOkCnt = 0;


/// @brief      EEPROM IIC Write Process 완료 처리 함수
/// @details    EEPROM IIC Write 동작 완료 상태값을 설정한다
/// @param      void
/// @return     void
void EEPROM_IICA0_WriteProcess_Done(void)
{
    gu8IICA0_WriteProc = RTC_PROC_START;
}


/// @brief      EEPROM IIC Read Process 완료 처리 함수
/// @details    EEPROM IIC Read 동작 완료 상태값을 설정한다
/// @param      void
/// @return     void
void EEPROM_IICA0_ReadProcess_Done(void)
{
    gu8IICA0_ReadProc = RTC_PROC_DONE;
}


/// @brief      IIC Write Cycle Time
/// @details    IIC Write 시 Delay 제어를 한다
/// @param      void
/// @return     void
void IICA0_WriteCycleTime(void)
{
    U16 mu16temp = 0;

    /*..hui [17-12-8오후 1:16:26] 약 5.3ms..*/
    /*..hui [19-10-21오후 1:25:35] 약 16ms, typical 12ms, max 20ms..*/
    for (mu16temp = 0 ; mu16temp < 10000 ; mu16temp++)
    {
       NOP(); NOP(); NOP(); NOP(); NOP();
       NOP(); NOP(); NOP(); NOP(); NOP();
       NOP(); NOP(); NOP(); NOP(); NOP();
       NOP(); NOP(); NOP(); NOP(); NOP();
       NOP(); NOP(); NOP(); NOP(); NOP();
       NOP(); NOP(); NOP(); NOP(); NOP();
    }
}


/// @brief      IIC Writing 1-Byte Process
/// @details    IIC Byte Write 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             mu8Data : Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8ComStatus = ByteWrite(mu8Dev, mu16Addr, mu8Data);

        if (mu8ComStatus == RTC_IIC_OK)
        {
            return RTC_IIC_OK;
        }
        else
        {
        }
    }
    return RTC_IIC_FAIL;
}


/// @brief      IIC Writing 1-Byte Process-1
/// @details    IIC Byte Write 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             mu8Data : Data
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data)
{
    U8 mu8buf[3];
    U16 mu16WaitCycle = 10000;
    U8 mu8Return = 0;

    mu8buf[0] = (U8)((mu16Addr >> 8) & 0x00FF);
    mu8buf[1] = (U8)(mu16Addr & 0x00FF);
    mu8buf[2] = mu8Data;

    gu8IICA0_WriteProc = RTC_PROC_START;

    mu8Return = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | RTC_OP_WR), mu8buf, 3, 200) );

    if (mu8Return != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  RTC_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != RTC_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  RTC_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();
    IICA0_WriteCycleTime();

    gu8IICA0_WriteOkCnt++;
    return  RTC_IIC_OK;
}


/// @brief      IIC Write page Process
/// @details    IIC Page Write 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_PageWrite(U8 mu8Dev, U16 mu16Addr , U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = PageWrite(mu8Dev, mu16Addr, pu8Data, mu8Len);

        if (mu8ComStatus == RTC_IIC_OK)
        {
            return RTC_IIC_OK;
        }
        else
        {
        }
    }

    return RTC_IIC_FAIL;
}


/// @brief      IIC Write page Process-1
/// @details    IIC Page Write 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Buf[RTC_PAGE_SIZE + 2];
    U16 mu16WaitCycle = 10000;
    U8 mu8Return = 0;

    mu8Buf[0] = (U8)((mu16Addr >> 8) & 0x00FF);
    mu8Buf[1] = (U8)(mu16Addr & 0x00FF);

    gu8IICA0_WriteProc = RTC_PROC_START;

    // Write Page..
    mu8Return = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | RTC_OP_WR), &mu8Buf[0], (mu8Len + 2), 200 ) );

    if (mu8Return != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;
        IICA0_Stop_Condition_Generate();
        return  RTC_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != RTC_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;
            IICA0_Stop_Condition_Generate();
            return  RTC_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();

    IICA0_WriteCycleTime();

    gu8IICA0_WriteOkCnt++;

    return  RTC_IIC_OK;
}


/// @brief      IIC Reading 1-Byte Process
/// @details    IIC Byte Read 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = ByteRead(mu8Dev, mu16Addr, pu8Data);

        if (mu8ComStatus == RTC_IIC_OK)
        {
            return  RTC_IIC_OK;
        }
        else
        {
        }
    }

    return  RTC_IIC_FAIL;
}


/// @brief      IIC Reading 1-Byte Process-1
/// @details    IIC Byte Read 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data)
{
    U8 mu8Buf[2] = {0};
    U16 mu16WaitCycle = 10000;
    U8 mu8ReturnW = 0;
    U8 mu8ReturnR = 0;

    mu8Buf[0] = (U8)((mu16Addr >> 8) & 0x00FF);
    mu8Buf[1] = (U8)(mu16Addr & 0x00FF);

    gu8IICA0_WriteProc = RTC_PROC_START;

    // Write address..
    mu8ReturnW = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | RTC_OP_WR), mu8Buf, 2, 200 ) );

    if (mu8ReturnW != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  RTC_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != RTC_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  RTC_IIC_FAIL;
        }
        else
        {
        }
    }

    gu8IICA0_ReadProc = RTC_PROC_START;

    mu8ReturnR = (U8)( IICA0_MASTER_RECEIVE_FUCTION( (mu8Dev | RTC_OP_RD), pu8Data, 1, 200 ) );

    // Read data..
    if (mu8ReturnR != MD_OK )
    {
        gu8IICA0_WriteFailCnt++;
        IICA0_Stop_Condition_Generate();
        return  RTC_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_ReadProc != RTC_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_ReadFailCnt++;
            IICA0_Stop_Condition_Generate();
            return  RTC_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();

    gu8IICA0_ReadOkCnt++;
    return  RTC_IIC_OK;
}


/// @brief      IIC Read Sequential Process
/// @details    IIC Page Read 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = SeqRead(mu8Dev, mu16Addr , pu8Data, mu8Len);

        if (mu8ComStatus == RTC_IIC_OK)
        {
            return  RTC_IIC_OK;
        }
        else
        {
        }
    }

    return  RTC_IIC_FAIL;
}


/// @brief      IIC Read Sequential Process-1
/// @details    IIC Page Read 동작 제어를 한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Buf[2] = {0};
    U16 mu16WaitCycle = 10000;
    U8 mu8ReturnW = 0;
    U8 mu8ReturnR = 0;

    mu8Buf[0] = (U8)((mu16Addr >> 8) & 0x00FF);
    mu8Buf[1] = (U8)(mu16Addr & 0x00FF);

    gu8IICA0_WriteProc = RTC_PROC_START;

    mu8ReturnW = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | RTC_OP_WR), mu8Buf, 2, 200 ) );

    /* Send  Address */
    if (mu8ReturnW != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  RTC_IIC_FAIL;
    }

    /* Wait */
    while (gu8IICA0_WriteProc != RTC_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  RTC_IIC_FAIL;
        }
        else
        {
        }
    }

    gu8IICA0_ReadProc = RTC_PROC_START;

    mu8ReturnR = (U8)( IICA0_MASTER_RECEIVE_FUCTION( (mu8Dev | RTC_OP_RD), pu8Data, mu8Len, 200 ) );

    /* Recv Data */
    if (mu8ReturnR != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  RTC_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_ReadProc != RTC_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_ReadFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  RTC_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();
    gu8IICA0_ReadOkCnt++;
    return  RTC_IIC_OK;
}


/// @brief      IIC Communication Stop Condition
/// @details    IIC 통신에서 통신 정상 완료 여부를 확인한다
/// @param      void
/// @return     mu8Return : IIC 동작 완료 상태 - 0(IIC_FAIL), 1(IIC_OK)
U8 IICA0_Stop_Condition_Generate(void)
{
    U8 gu8Status = 0;
    U8 gu8WaitCycle = 0;

    gu8WaitCycle = 50;

    SPT0 = SET;     /* set stop condition flag */

    /* Wait */
    while (SPD0 == CLEAR)
    {
        gu8WaitCycle--;

        if (gu8WaitCycle <= 0)
        {
            gu8Status = RTC_IIC_FAIL;
            return  gu8Status;
        }
        else
        {
        }
    }

    gu8Status = RTC_IIC_OK;

    return  gu8Status;
}


#endif


// RTC 제어  함수들 -------------------------------------------------------------------------------------

SRTC_TimeData_T     SRTC_Time;
SRTC_TimeData_T     SRTC_Time_Before;

SRealTimeClock_T  SRTC;

U16 gu16RTC_ProcessControlTimer = 0;                    /// @brief  RTC 제어 동작 주기 타이머

U16 gu16RTC_ErrorCnt = 0;                               /// @brief  RTC 제어 에러 횟수 카운터
U16 gu16RTC_ErrorTimer = 0;                             /// @brief  RTC 제어 에러 체크 시간

U8 gu8RTC_ErrorState = 0;                               /// @brief  RTC 제어 에러 발생 상태 - 0(미발생), 1(발생)
U8 gu8RTC_Comm_ErrorState = 0;                          /// @brief  RTC 통신 에러 발생 상태 - 0(미발생), 1(발생)

U8 gu8RTC_GarbageTimer = 0;                             /// @brief  RTC 값 이상 감지 상태 시간

U8 gu8RTC_StopErrorTimer = 0;                           /// @brief  RTC 값 이상 감지 시간
U8 gu8RTC_StopErrorState = 0;                           /// @brief  RTC 값 이상 감지 상태 - 0(미발생), 1(발생)

U8 gu8RTC_ReadTimer = 0;                                /// @brief  RTC Read 주기 시간
U8 gu8RTC_TimeRead = 0;                                 /// @brief  RTC Read 상태 - 0(Read 안함), 1(Read 완료)


/// @brief      RTC 관련 Delay ms
/// @details    RTC 관련 ms 단위 Delay 제어를 한다
/// @param      void
/// @return     void
void RTC_Delay_ms(U16 mu16ms)
{
    U16 mu16i = 0;
    U8 mu8j = 0;

    /*..hui [19-10-21오후 1:32:34] 실측 약 86.8ms..*/
    for (mu16i = 0 ; mu16i < mu16ms ; mu16i++)
    {
        for (mu8j = 0; mu8j < 4 ; mu8j++)   // 1ms loop delay
        {
            RTC_Delay_us( 250 );
            WDT_Restart();
        }
    }
}


/// @brief      RTC 관련 Delay us
/// @details    RTC 관련 us 단위 Delay 제어를 한다
/// @param      void
/// @return     void
void RTC_Delay_us(U8 mu8us)
{
    while (mu8us--)
    {
        NOP();
        NOP();
        NOP();
        NOP();
    }
}


/// @brief      RTC 관련 초기화
/// @details    RTC 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void RTC_Initialize(void)
{
    U16 mu16Delay = 0;
    U8 mu8RTC_Err = 0;

    mu16Delay = 30;

    MEMSET( (void __FAR *)&SRTC_Time, 0x00, sizeof(SRTC_TimeData_T) );
    MEMSET( (void __FAR *)&SRTC_Time_Before, 0x00, sizeof(SRTC_TimeData_T) );

    MEMSET( (void __FAR *)&SRTC, 0x00, sizeof(SRealTimeClock_T) );

    gu16RTC_ProcessControlTimer = 0;

    gu16RTC_ErrorCnt = 0;
    gu16RTC_ErrorTimer = 0;

    gu8RTC_ErrorState = 0;
    gu8RTC_Comm_ErrorState = 0;

    gu8RTC_GarbageTimer = 0;

    gu8RTC_StopErrorTimer = 0;
    gu8RTC_StopErrorState = 0;

    gu8RTC_ReadTimer = 0;
    gu8RTC_TimeRead = 0;

    // 만약 배터리 모드로 동작하는 중이면, 재시도...
    do
    {
        RTC_Delay_ms(100);

        mu16Delay--;

        if (mu16Delay == 0)
        {
            break;
        }
        else
        {
        }

    } while (IsRTCBatBackUpMode() == TRUE);

    EnableRtcCCR();
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_CTR_PWR, 0x00);            // Trip Voltage 2.2V
    DisableRtcCCR();
//    Hal_IIC_ByteRead(DEV_ADDR_RTC, ADDR_CTR_PWR, &SRTC.mu8RegPower);

    EnableRtcCCR();
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_CTR_DTR, 0x03);            // Digital Trimming(+30ppm)
    DisableRtcCCR();

    EnableRtcCCR();
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_CTR_ATR, (0x20|0x10));
    DisableRtcCCR();

    EnableRtcCCR();
    //HAL_RTC_ByteWrite(DEV_ADDR_RTC, ADDR_CTR_INT, 0x18);          // Output freq 32.768khz
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_CTR_INT, 0x00);            // Output freq disabled
    DisableRtcCCR();

    EnableRtcCCR();
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_CTR_BL,  0x00);
    DisableRtcCCR();

    // Read RTC Status
    HAL_IIC_BYTE_READ(DEV_ADDR_RTC, ADDR_RTC_STATUS, &SRTC.gu8RegStatus);

    mu8RTC_Err = IsRTC_Reset();

    if (mu8RTC_Err == SET)
    {
        // 방전된 상태인 경우 초기화
        InitRTC_Time();
    }
    else
    {
        ReadRTC_Time();

        if(SRTC.gu8InitSuccess == SET)
        {
            // 시간이 설정 범위 이외의 값으로 Read 되는 경우 무조건 초기화
            if ( ( (SRTC_Time.gu8Year >= RTC_SETTING_MIN_YEAR) && (SRTC_Time.gu8Year <= RTC_SETTING_MAX_YEAR) ) &&
                 ( (SRTC_Time.gu8Month >= 1) && (SRTC_Time.gu8Month <= 12) ) &&
                 ( (SRTC_Time.gu8Date >= 1) && (SRTC_Time.gu8Date  <= 31) ) &&
                 (SRTC_Time.gu8Hour <= 23) &&
                 (SRTC_Time.gu8Min <= 59) &&
                 (SRTC_Time.gu8Sec <= 59) )
            {

            }
            else
            {
                InitRTC_Time();
            }
        }
        else
        {
            // 통신에 실패할 경우 값만 초기화
            SRTC_Time.gu8Year    = RTC_DEFAULT_YEAR;
            SRTC_Time.gu8Month   = RTC_DEFAULT_MONTH;
            SRTC_Time.gu8Date    = RTC_DEFAULT_DATE;
            SRTC_Time.gu8Hour    = RTC_DEFAULT_HUOR;
            SRTC_Time.gu8Min     = RTC_DEFAULT_MIN;
            SRTC_Time.gu8Sec     = RTC_DEFAULT_SEC;
            SRTC_Time.gu8DayWeek = RTC_DEFAULT_DAYWEEK;
            SRTC_Time.gu8Y2K     = RTC_DEFAULT_Y2K;
        }
    }
}


/// @brief      Enable RTC CCR
/// @details    RTC의 CCR 상태를 Enable 시킨다
/// @param      void
/// @return     void
static void EnableRtcCCR(void)
{
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x02);
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x06);
    RTC_Delay_ms(200);
}


/// @brief      Disable RTC CCR
/// @details    RTC의 CCR 상태를 disable 시킨다
/// @param      void
/// @return     void
static void DisableRtcCCR(void)
{
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x00);
}


/// @brief      RTC Reset Check
/// @details    RTC의 Reset 여부를 체크한다
/// @param      void
/// @return     void
U8 IsRTC_Reset(void)
{
    if ( (SRTC.gu8RegStatus & (MASK_RTCF)) == 0 )
    {
        return  CLEAR;   // 정상
    }
    else
    {
    }

    return  SET;         //ERROR
}


/// @brief      RTC 제어 Process 제어 시간 카운터(@1ms)
/// @details    RTC 제어 동작 주기 타이머를 카운트 한다
/// @param      void
/// @return     void
void RTC_ProcessControlTimer(void)
{
    if (gu16RTC_ProcessControlTimer < RTC_PROCESS_CONTROL_TIME_PERIOD)
    {
        gu16RTC_ProcessControlTimer++;
    }
}


/// @brief      RTC 제어 Process 함수(@While)
/// @details    RTC 제어 동작을 한다
/// @param      void
/// @return     void
void ProcessRTC(void)
{
    U8 mu8RtcErr = 0;
    U8 mu8Return = 0;

    if (gu16RTC_ProcessControlTimer >= RTC_PROCESS_CONTROL_TIME_PERIOD)
    {   // 1초마다 수행
        gu16RTC_ProcessControlTimer = 0;

        mu8Return = RTC_OverMaxYear();

        // 년도 최대 상한 제한
        if (mu8Return == SET)
        {   // 최대 년도가 초과되면 초기화 됨
            InitRTC_Time();
            return;
        }
        else
        {
        }

        // Read Time from RTC
        gu8RTC_ReadTimer++;

        if (gu8RTC_ReadTimer >= RTC_READ_TIME)
        {
            // 20초 마다 한번씩 읽기
            gu8RTC_ReadTimer = 0;

            // Read RTC Status
            HAL_IIC_BYTE_READ(DEV_ADDR_RTC, ADDR_RTC_STATUS, &SRTC.gu8RegStatus);

            /*..hui [20-6-11오전 9:38:33] 방전되거나 크리스탈 불량인 상태..*/
            mu8RtcErr = IsRTC_Error();

            // RTC 상태
            if (mu8RtcErr == SET)
            {
                // RTC 방전된 상태
                gu16RTC_ErrorCnt++;

                /*..hui [20-2-18오전 10:00:22] 방전됐을때도 시간 초기화는 안하는걸로..*/
                /*..hui [20-2-18오전 10:00:36] 그냥 이전시간에서 이어서 시작되도록.. 그래야 설정이 편할테니..*/
                /*..hui [20-2-18오후 1:38:42] 리셋되면 RTC 자체에서 시간 초기화됨..*/
                /*..hui [20-4-17오후 8:41:41] 최대 100번까지만 제한..*/

                gu8RTC_ErrorState = SET;
            }
            else
            {
                // RTC 에러 상태 클리어
                gu16RTC_ErrorTimer = 0;
                gu8RTC_ErrorState = CLEAR;
            }

            ReadRTC_Time();
            gu8RTC_TimeRead = SET;

            // 시간이 설정 가능 범위 이외일 경우에는 무조건 초기화
            if ( ( (SRTC_Time.gu8Year >= RTC_SETTING_MIN_YEAR) && (SRTC_Time.gu8Year <= RTC_SETTING_MAX_YEAR) ) &&
                 ( (SRTC_Time.gu8Month >= 1) && (SRTC_Time.gu8Month <= 12) ) &&
                 ( (SRTC_Time.gu8Date >= 1) && (SRTC_Time.gu8Date  <= 31) ) &&
                 (SRTC_Time.gu8Hour <= 23) &&
                 (SRTC_Time.gu8Min <= 59) &&
                 (SRTC_Time.gu8Sec <= 59) )
            {
                gu8RTC_GarbageTimer = 0;

                RTC_ErrorCheck();
            }
            else
            {
                if ( (gu8RTC_Comm_ErrorState == CLEAR) && (gu8RTC_ErrorState == CLEAR) )
                {
                    gu8RTC_GarbageTimer++;
                }
                else
                {
                    gu8RTC_GarbageTimer = 0;
                }

                // 9회 연속 쓰레기 값이 읽힐 경우 초기화
                if (gu8RTC_GarbageTimer >= 9)
                {
                    gu8RTC_GarbageTimer = 0;
                    InitRTC_Time();
                }
                else
                {
                }
            }
        }
        else{}
    }
}



/// @brief      RTC의 년도 값이 Max 설정값 초과 여부 확인 함수
/// @details    RTC의 년도 값이 Max 설정한 값을 초과하는지 여부를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : RTC의 년도가 Max값이 넘는지 여부 - 0(No), 1(Yes)
U8 RTC_OverMaxYear(void)
{
    U8 mu8Return = 0;

    if (SRTC_Time.gu8Year > RTC_SETTING_MAX_YEAR)
    {
        mu8Return = 1;
    }
    else
    {
        mu8Return = 0;
    }

    return  mu8Return;
}



/// @brief      RTC의 Battery 동작 상태 여부 확인 함수
/// @details    RTC의 Battery 동작 상태 여부를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     mu8Return : 배터리 동작 상태 여부 - 0(Vcc Mode), 1(Battery Backup Mode)
U8 IsRTCBatBackUpMode(void)
{
    U8 mu8Ret = 0;
    U8 mu8Reg = 0;

    mu8Ret = HAL_IIC_BYTE_READ(DEV_ADDR_RTC, ADDR_RTC_STATUS, &mu8Reg);

    if (mu8Ret != TRUE)
    {
        return  TRUE;   // Error
    }
    else
    {
    }

    if ( (mu8Reg & MASK_BAT) != 0 )
    {
        return  TRUE;   // Battery BackUp Mode
    }
    else
    {
    }

    return  FALSE;  // Vcc Mode
}


/// @brief      RTC에서 시간 Read 함수
/// @details    RTC에서 시간값을 Read 한다
/// @param      void
/// @return     void
void ReadRTC_Time(void)
{
    U8 mu8Ret_1 = 0;
    U8 mu8Ret_2 = 0;
    U16 mu16WaitCycle = 1000;

    mu8Ret_1 = HAL_IIC_SEQ_READ(DEV_ADDR_RTC, ADDR_RTC_SECONDS, SRTC.au8Readbuf1, 8);

    if (mu8Ret_1 == FALSE)
    {
        // RTC 통신 에러
        gu8RTC_Comm_ErrorState = SET;
        return;
    }
    else
    {
    }

    // 연속으로 읽기전에 일정시간 대기
    while (mu16WaitCycle--)
    {
        ;
    }

    // Read aging time buffer
    mu8Ret_2 = HAL_IIC_SEQ_READ(DEV_ADDR_RTC, ADDR_RTC_SECONDS, SRTC.au8Readbuf2, 8);

    if( mu8Ret_2 == FALSE )
    {
        // RTC 통신 에러
        gu8RTC_Comm_ErrorState = SET;
        return;
    }
    else
    {
    }

    // [0] 번지의 sec 값을 제외하고 나머지 시간이 동일하면 시간을 저장함.
    // read 과정에서 시간 값이 noise에 의해서 손상되는 것을 막기 위한 동작 사양임
    if ( (SRTC.au8Readbuf1[1] == SRTC.au8Readbuf2[1]) &&
         (SRTC.au8Readbuf1[2] == SRTC.au8Readbuf2[2]) &&
         (SRTC.au8Readbuf1[3] == SRTC.au8Readbuf2[3]) &&
         (SRTC.au8Readbuf1[4] == SRTC.au8Readbuf2[4]) &&
         (SRTC.au8Readbuf1[5] == SRTC.au8Readbuf2[5]) &&
         (SRTC.au8Readbuf1[6] == SRTC.au8Readbuf2[6]) &&
         (SRTC.au8Readbuf1[7] == SRTC.au8Readbuf2[7]) )
    {
        SRTC_Time.gu8Sec        = HEX2DEC(SRTC.au8Readbuf2[0]);
        SRTC_Time.gu8Min        = HEX2DEC(SRTC.au8Readbuf2[1]);
        SRTC_Time.gu8Hour       = HEX2DEC(SRTC.au8Readbuf2[2] & 0x3F);
        SRTC_Time.gu8Date       = HEX2DEC(SRTC.au8Readbuf2[3]);
        SRTC_Time.gu8Month      = HEX2DEC(SRTC.au8Readbuf2[4]);
        SRTC_Time.gu8Year       = HEX2DEC(SRTC.au8Readbuf2[5]);
        SRTC_Time.gu8DayWeek    = HEX2DEC(SRTC.au8Readbuf2[6]);
        SRTC_Time.gu8Y2K        = HEX2DEC(SRTC.au8Readbuf2[7]);

        SRTC.gu8InitSuccess = SET;
        gu8RTC_Comm_ErrorState = CLEAR;
    }
    else
    {
    }
}


/// @brief      RTC에 시간 Write 함수
/// @details    RTC에 시간값을 Write 한다
/// @param      void
/// @return     void
void WriteRTC_Time(void)
{
    SRTC.au8writeBuf[0] =  DEC2HEX(SRTC_Time.gu8Sec);
    SRTC.au8writeBuf[1] =  DEC2HEX(SRTC_Time.gu8Min);
    SRTC.au8writeBuf[2] =  DEC2HEX(SRTC_Time.gu8Hour) | MASK_MIL;
    SRTC.au8writeBuf[3] =  DEC2HEX(SRTC_Time.gu8Date);
    SRTC.au8writeBuf[4] =  DEC2HEX(SRTC_Time.gu8Month);
    SRTC.au8writeBuf[5] =  DEC2HEX(SRTC_Time.gu8Year);
    SRTC.au8writeBuf[6] =  DEC2HEX(SRTC_Time.gu8DayWeek);
    SRTC.au8writeBuf[7] =  DEC2HEX(SRTC_Time.gu8Y2K);

    EnableRtcCCR();
    HAL_IIC_PAGE_WRITE(DEV_ADDR_RTC, ADDR_RTC_SECONDS, SRTC.au8writeBuf, 8);
    DisableRtcCCR();
}


/// @brief      RTC 시간 초기화 함수
/// @details    RTC의 시간값을 초기화 시킨다
/// @param      void
/// @return     void
void InitRTC_Time(void)
{
    // RTC read값 초기화
    SRTC_Time.gu8Year = RTC_DEFAULT_YEAR;
    SRTC_Time.gu8Month = RTC_DEFAULT_MONTH;
    SRTC_Time.gu8Date = RTC_DEFAULT_DATE;
    SRTC_Time.gu8Hour = RTC_DEFAULT_HUOR;
    SRTC_Time.gu8Min = RTC_DEFAULT_MIN;
    SRTC_Time.gu8Sec = RTC_DEFAULT_SEC;
    SRTC_Time.gu8DayWeek = RTC_DEFAULT_DAYWEEK;
    SRTC_Time.gu8Y2K = RTC_DEFAULT_Y2K;

    SRTC.au8writeBuf[0] = DEC2HEX(SRTC_Time.gu8Sec);
    SRTC.au8writeBuf[1] = DEC2HEX(SRTC_Time.gu8Min);
    SRTC.au8writeBuf[2] = DEC2HEX(SRTC_Time.gu8Hour) | MASK_MIL;
    SRTC.au8writeBuf[3] = DEC2HEX(SRTC_Time.gu8Date);
    SRTC.au8writeBuf[4] = DEC2HEX(SRTC_Time.gu8Month);
    SRTC.au8writeBuf[5] = DEC2HEX(SRTC_Time.gu8Year);
    SRTC.au8writeBuf[6] = DEC2HEX(SRTC_Time.gu8DayWeek);
    SRTC.au8writeBuf[7] = DEC2HEX(SRTC_Time.gu8Y2K);

    EnableRtcCCR();
    HAL_IIC_PAGE_WRITE(DEV_ADDR_RTC, ADDR_RTC_SECONDS, SRTC.au8writeBuf, 8);
    DisableRtcCCR();
}


/// @brief      RTC IC 오류 확인
/// @details    RTC IC의 동작상 오류 상태를 확인한다
/// @param      void
/// @return     return : 오류 상태 여부 - 0(정상), 1(오류)
U8 IsRTC_Error(void)
{
    if ( (SRTC.gu8RegStatus & (MASK_OSCF|MASK_RTCF)) == 0 )
    {
        return  CLEAR; // 정상
    }
    else
    {
    }

    return  SET; //ERROR
}


/// @brief      RTC IC OSC 오류 확인
/// @details    RTC IC의 OSC의 오류 상태를 확인한다
/// @param      void
/// @return     mu8Return : 오류 상태 여부 - 0(정상), 1(오류)
U8 IsRTC_OSCF_Error(void)
{
    if ( (SRTC.gu8RegStatus & (MASK_OSCF)) == 0 )
    {
        return  CLEAR; // 정상
    }
    else
    {
    }

    return  SET; //ERROR
}



/// @brief      RTC Error Check
/// @details    RTC의 동작 에러 상태를 체크한다
/// @param      void
/// @return     void
void RTC_ErrorCheck(void)
{
    /*..hui [20-6-18오후 3:59:08] RTC 시간이 변경되지 않을 경우 LCD 시간으로 전환하기 위해 추가..*/
    if ( (SRTC_Time.gu8Year == SRTC_Time_Before.gu8Year) &&
         (SRTC_Time.gu8Month == SRTC_Time_Before.gu8Month) &&
         (SRTC_Time.gu8Date == SRTC_Time_Before.gu8Date) &&
         (SRTC_Time.gu8Hour == SRTC_Time_Before.gu8Hour) &&
         (SRTC_Time.gu8Min == SRTC_Time_Before.gu8Min) &&
         (SRTC_Time.gu8Sec == SRTC_Time_Before.gu8Sec) )
    {
        gu8RTC_StopErrorTimer++;

        /*..hui [20-6-19오후 1:38:00] 1분연속..*/
        /*..hui [20-6-19오후 1:46:23] 어차피 RTC 다시 읽었을때 정상값이 읽히면 그때부터 다시 LCD에 업뎃해주면 되므로..*/
        /*..hui [20-6-19오후 1:46:35] 그냥 한번만 동일하게 읽혀도 LCD 시간 업뎃은 안하는걸로..*/
        if( gu8RTC_StopErrorTimer >= 1 )
        {
            gu8RTC_StopErrorTimer = 1;
            gu8RTC_StopErrorState = SET;
        }
        else
        {
        }
    }
    else
    {
        SRTC_Time_Before.gu8Year    = SRTC_Time.gu8Year;
        SRTC_Time_Before.gu8Month   = SRTC_Time.gu8Month;
        SRTC_Time_Before.gu8Date    = SRTC_Time.gu8Date;
        SRTC_Time_Before.gu8Hour    = SRTC_Time.gu8Hour;
        SRTC_Time_Before.gu8Min     = SRTC_Time.gu8Min;
        SRTC_Time_Before.gu8Sec     = SRTC_Time.gu8Sec;

        gu8RTC_StopErrorTimer = 0;
        gu8RTC_StopErrorState = CLEAR;
    }
}



/// @brief      시간 쓰기 함수
/// @details    RTC에 시간값을 Write 한다
/// @param      void
/// @return     void
void Set_RTC_Time(SRTC_TimeData_T* pSTime)
{
    if( ( (pSTime->gu8Year >= RTC_SETTING_MIN_YEAR) && (pSTime->gu8Year <= RTC_SETTING_MAX_YEAR) ) &&
        ( (pSTime->gu8Month >= 1) && (pSTime->gu8Month <= 12) ) &&
        ( (pSTime->gu8Date >= 1) && (pSTime->gu8Date <= 31) ) &&
        (pSTime->gu8Hour <= 23) &&
        (pSTime->gu8Min <= 59) &&
        (pSTime->gu8Sec <= 59) )
    {
        SRTC_Time.gu8Sec        = pSTime->gu8Sec;
        SRTC_Time.gu8Min        = pSTime->gu8Min;
        SRTC_Time.gu8Hour       = pSTime->gu8Hour;
        SRTC_Time.gu8Date       = pSTime->gu8Date;
        SRTC_Time.gu8Month      = pSTime->gu8Month;
        SRTC_Time.gu8Year       = pSTime->gu8Year;
        SRTC_Time.gu8DayWeek    = pSTime->gu8DayWeek;
        SRTC_Time.gu8Y2K        = pSTime->gu8Y2K;

        WriteRTC_Time();
    }
    else
    {
    }
}


/// @brief      시간 확인 함수
/// @details    RTC에서 Read한 시간값을 확인한다
/// @param      void
/// @return     void
void Get_RTC_Time(SRTC_TimeData_T* pSTime)
{
    pSTime->gu8Sec      =  SRTC_Time.gu8Sec;
    pSTime->gu8Min      =  SRTC_Time.gu8Min;
    pSTime->gu8Hour     =  SRTC_Time.gu8Hour;
    pSTime->gu8Date     =  SRTC_Time.gu8Date;
    pSTime->gu8Month    =  SRTC_Time.gu8Month;
    pSTime->gu8Year     =  SRTC_Time.gu8Year;
    pSTime->gu8DayWeek  =  SRTC_Time.gu8DayWeek;
    pSTime->gu8Y2K      =  SRTC_Time.gu8Y2K;
}



// EERPOM 제어  함수들 -------------------------------------------------------------------------------------

U8 gu8EEPROM_CommunicationError;                    /// @brief  EEPROM 통신 불량 에러 발생 변수 - 0(No Error), 1(Error)

/// @brief  EEPROM 관리 자료형
typedef struct {
    U16 au16Address[EEPROM_ID_MAX];
    U8 au8Data[EEPROM_ID_MAX];

}   SEEPROM_Data_T;

SEEPROM_Data_T  SEEPROM_Data;


/// @brief      EEPROM Data 전체 Read 함수
/// @details    EEPROM 데이터를 전체 Read 한다
/// @param      void
/// @return     void
void EEPROM_Data_Load(void)
{
    EepromSeqRead(EEPROM_ID_START, SEEPROM_Data.au8Data, EEPROM_ID_MAX);
}


/// @brief      EEPROM Data Byte Write 함수
/// @details    EEPROM Data를 Byte Write 동작 한다
/// @param      mu16ID : 저장하려는 위치의 ID(Address)
///             mu8Data : 저장하려는 값
/// @return     void
void EepromByteWrite(U16 mu16ID, U8 mu8Data)
{
    HAL_IIC_BYTE_WRITE(DEV_ADDR_EEP, mu16ID, mu8Data);
}


/// @brief      EEPROM Data Byte Read 함수
/// @details    EEPROM Data를 Byte Read 동작 한다
/// @param      mu16ID : Read 하려는 위치의 ID(Address)
/// @return     void
void EepromByteRead(U16 mu16ID)

{
    U8 mu8EEPROM_ReadDataBuf = 0;

    HAL_IIC_BYTE_READ(DEV_ADDR_EEP, mu16ID, &mu8EEPROM_ReadDataBuf);

    SEEPROM_Data.au8Data[(U8)(mu16ID)] = mu8EEPROM_ReadDataBuf;
}



/// @brief      EEPROM Data Load 함수
/// @details    EEPROM 데이터를 Page Write 동작 한다
/// @param      mu16Addr : 저장하려는 Address
///             pu8Data : Data Pointer
///             mu8len : Length
/// @return     void
void EepromPageWrite(U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    HAL_IIC_PAGE_WRITE(DEV_ADDR_EEP, mu16Addr, pu8Data, mu8Len);
}


/// @brief      EEPROM Data Load 함수
/// @details    EEPROM 데이터를 Page Read 동작을 한다
/// @param      mu16Addr : 저장하려는 Address
///             pu8Data : Data Pointer
///             mu8len : Length
/// @return     void
void EepromSeqRead(U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    HAL_IIC_SEQ_READ(DEV_ADDR_EEP, mu16Addr, pu8Data, mu8Len);
}


/// @brief      EEPROM 이상 여부 체크 함수
/// @details    EEPROM의 통신의 이상 여부를 체크하여 에러 발생 여부를 체크한다
/// @param      void
/// @return     return : EEPROM 데이터 이상 여부 : 0(정상), 1(비정상)
U8 Get_EepromErrorCheck(void)
{
    U8 mu8EEPROM_CheckA = 0;
    U8 mu8EEPROM_CheckB = 0;

    EepromSeqRead(EEPROM_ID_START, &mu8EEPROM_CheckA, 1);
    EepromSeqRead(EEPROM_ID_END, &mu8EEPROM_CheckB, 1);

    if ( (mu8EEPROM_CheckA != VALUE_DEFINE_EEPROM_DATA_START) || (mu8EEPROM_CheckB != VALUE_DEFINE_EEPROM_DATA_END) )
    {   // EEPROM 데이터 비정상
        return  TRUE;
    }
    else
    {   // EEPRM 데이터 정상
        return  FALSE;
    }
}


/// @brief      EEPROM Data 설정 함수
/// @details    EEPROM Data를 설정하고 Write 한다
/// @param      mu8ID : EEPROM DATA ID
///             mu8Data : Write 하려는 데이터
/// @return     void
void Set_EEPROM_Data(U8 mu8ID, U8 mu8Data)
{
    SEEPROM_Data.au8Data[mu8ID] = mu8Data;

    Save_EEPROM_Data(mu8ID);
}


/// @brief      EEPROM에 Data를 저장하는 함수
/// @details    EEPROM에 ID에 해당하는 Address에 해당하는 데이터를 저장한다
/// @param      mu8ID : EEPROM DATA ID
///             mu8Data : Write 하려는 데이터
/// @return     void
void Save_EEPROM_Data(U8 mu8ID)
{
    EepromByteWrite((U16)(mu8ID), SEEPROM_Data.au8Data[mu8ID]);
}


/// @brief      EEPROM Data Load 함수
/// @details    EEPROM 데이터를 확인 한다
/// @param      mu8ID : 확인하려고 하는 EEPROM DATA ID
/// @return     return : Read Data
U8 Get_EEPROM_Data(U8 mu8ID)
{
    U8 mu8Return = 0;

    mu8Return = SEEPROM_Data.au8Data[mu8ID];

    return  mu8Return;
}


#endif


// Hal RTC Module ********************************************************************************************

/// @brief      Hal RTC Initilaize Module
/// @details    RTC 관련 변수를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_RTC_Module_Initialize(void)
{
#if (RTC_USE == USE)
    RTC_Initialize();                       // RTC 관련 초기화
#endif
}


/// @brief      Hal RTC Module in 1ms Interrupt
/// @details    1ms Interrupt 안에서 RTC 제어 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_RTC_Module_1ms_Control(void)
{
#if (RTC_USE == USE)
    RTC_ProcessControlTimer();              // RTC Process Control Time Counter
#endif
}


/// @brief      Hal RTC Module in While Loop
/// @details    Basic Loop 안 While 문 안에서 RTC 제어를 한다
/// @param      void
/// @return     void
void Hal_RTC_Module_Control(void)
{
#if (RTC_USE == USE)
    ProcessRTC();                           // RTC 관련 제어 함수
#endif
}

