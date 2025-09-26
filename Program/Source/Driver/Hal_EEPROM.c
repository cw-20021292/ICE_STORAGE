/// @file     Hal_EEPROM.c
/// @date     2025/03/24
/// @author   Jaejin Ham
/// @brief    EEPROM 제어 동작 관련 file

#include  "Global_Header.h"


#if (EEPROM_USE == USE)

//****************************************************************************
//  EEPROM 통신 사용 Port - IICA0 or GPIO
//****************************************************************************

/// @brief  EEPROM 관리 자료형
typedef struct {
    U8 au8Address[EEPROM_ID_MAX];
    U8 au8Data[EEPROM_ID_MAX];

}   SEEPROM_Data_T;

SEEPROM_Data_T  SEEPROM_Data;


#if (EEPROM_IIC_GPIO_USE == EEPROM_IIC_GPIO)      // IIC통신을 GPIO로 만들어서 동작 하는 경우

U8 gu8Txbit;                        /// @brief  I2C 송신시 Data값 Bit
U8 gu8Rxbit;                        /// @brief  I2C 수신시 Data값 Bit
U8 gu8AckFail;                      /// @brief  I2C 수신시 Ack 체크 Flag

U8 gu8IIC_WriteData = 0;            /// @brief  I2C 송신 데이터
U8 gu8IIC_ReadData = 0;             /// @brief  I2C 수신 데이터


/// @brief      IIC 통신시 Pulse 간 delay
/// @details    IIC 통신시 Pulse간 Delay 동작을 한다
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
/// @details    IIC Stat Bit 신호를 생성한다
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
/// @details    IIC Stop Bit 신호를 생성한다
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
/// @details    IIC Data Bit 신호를 생성한다
/// @return     void
/// @param      void
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
/// @details    IIC Data Bit 신호를 체크한다
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
/// @details    IIC Data를 Write한다
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
/// @details    IIC Data를 Read한다
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

#if (EEPROM_ADDR_BIT_SETTING == EEPROM_8BIT)                  // Address가 8bit

/// @brief      IIC Byte Write Process
/// @details    IIC Byte Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Write Address
///             mu8Data : Write Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_ByteWrite(U8 mu8Dev, U8 mu8Addr, U8 mu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_ByteWrite(mu8Dev, mu8Addr, mu8Data);

        if (mu8Status == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Byte Write Process-1
/// @details    IIC Byte Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Write Address
///             mu8Data : Write Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_ByteWrite(U8 mu8Dev, U8 mu8Addr, U8 mu8Data)
{
    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | EEP_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = mu8Addr;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = mu8Data;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Stop();

    Delay_ms(30);

    return  EEPROM_IIC_OK;
}


/// @brief      IIC Page Write Process
/// @details    IIC Page Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Write Address
///             pu8Data : Write Data 배열
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_PageWrite(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_PageWrite(mu8Dev, mu8Addr, pu8Data, mu8Len);

        if (mu8Status == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Page Write Process-1
/// @details    IIC Page Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Write Address
///             pu8Data : Write Data 배열
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_PageWrite(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;

    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | EEP_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = mu8Addr;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    // 데이터 송신
    for (mu8Cnt = 0 ; mu8Cnt < mu8Len ; mu8Cnt++)
    {
        WDT_Restart();
        gu8IIC_WriteData = *pu8Data;
        IIC_TXD();
        if(gu8AckFail)
        {
            return  EEPROM_IIC_FAIL;
        }

        pu8Data++;
    }

    WDT_Restart();
    IIC_Stop();

    Delay_ms(30);

    return  EEPROM_IIC_OK;
}


/// @brief      IIC Byte Read Process
/// @details    IIC Byte Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Read Address
///             pu8Data : Data Pointer
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_ByteRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_ByteRead(mu8Dev, mu8Addr, pu8Data);

        if (mu8Status == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Byte Read Process-1
/// @details    IIC Byte Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Read Address
///             pu8Data : Data Pointer
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_ByteRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data)
{
    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    gu8IIC_WriteData = (mu8Dev | EEP_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = mu8Addr;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | EEP_OP_RD);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    IIC_RXD();
    *pu8Data = gu8IIC_ReadData;

    IIC_Stop();

    return  EEPROM_IIC_OK;
}



/// @brief      IIC Sequential Read Process
/// @details    IIC Page Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Read Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_SeqRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_SeqRead(mu8Dev, mu8Addr, pu8Data, mu8Len);

        if (mu8Status == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Sequential Read Process-1
/// @details    IIC Page Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Read Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_SeqRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;

    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    gu8IIC_WriteData = (mu8Dev | EEP_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = mu8Addr;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | EEP_OP_RD);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    for (mu8Cnt = 0 ; mu8Cnt < mu8Len ; mu8Cnt++)
    {
        WDT_Restart();
        IIC_RXD();

        *pu8Data = gu8IIC_ReadData;
        pu8Data++;
    }

    IIC_Stop();

    return  EEPROM_IIC_OK;
}

#else                                   // Address가 16bit

/// @brief      IIC Byte Write Process
/// @details    IIC Byte Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             mu8Data : Write Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_ByteWrite(mu8Dev, mu16Addr, mu8Data);

        if (mu8Status == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Byte Write Process-1
/// @details    IIC Byte Write 동작을 수행한다
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
    gu8IIC_WriteData = (mu8Dev | EEP_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)((mu16Addr >> 8) & 0x00FF);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)(mu16Addr & 0x00FF);;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = mu8Data;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Stop();

    Delay_ms(30);

    return  EEPROM_IIC_OK;
}


/// @brief      IIC Page Write Process
/// @details    IIC Page Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             pu8Data : Write Data 배열
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_PageWrite(mu8Dev, mu16Addr, pu8Data, mu8Len);

        if (mu8Status == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Page Write Process-1
/// @details    IIC Page Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             pu8Data : Write Data 배열
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;

    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | EEP_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)((mu16Addr >> 8) & 0x00FF);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)(mu16Addr & 0x00FF);;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    // 데이터 송신
    for (mu8Cnt = 0 ; mu8Cnt < mu8Len ; mu8Cnt++)
    {
        WDT_Restart();
        gu8IIC_WriteData = *pu8Data;
        IIC_TXD();
        if(gu8AckFail)
        {
            return  EEPROM_IIC_FAIL;
        }

        pu8Data++;
    }

    WDT_Restart();
    IIC_Stop();

    Delay_ms(30);

    return  EEPROM_IIC_OK;
}


/// @brief      IIC Byte Read Process
/// @details    IIC Byte Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_ByteRead(mu8Dev, mu16Addr, pu8Data);

        if (mu8Status == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Byte Read Process-1
/// @details    IIC Byte Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data)
{
    gu8AckFail = 0;

    WDT_Restart();
    IIC_Start();

    gu8IIC_WriteData = (mu8Dev | EEP_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)((mu16Addr >> 8) & 0x00FF);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)(mu16Addr & 0x00FF);;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | EEP_OP_RD);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    IIC_RXD();
    *pu8Data = gu8IIC_ReadData;

    IIC_Stop();

    return  EEPROM_IIC_OK;
}



/// @brief      IIC Sequential Read Process
/// @details    IIC Page Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8Status = IIC_SeqRead(mu8Dev, mu16Addr, pu8Data, mu8Len);

        if (mu8Status == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Sequential Read Process-1
/// @details    IIC Page Read 동작을 수행한다
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

    gu8IIC_WriteData = (mu8Dev | EEP_OP_WR);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)((mu16Addr >> 8) & 0x00FF);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    gu8IIC_WriteData = (U8)(mu16Addr & 0x00FF);;
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    WDT_Restart();
    IIC_Start();

    WDT_Restart();
    gu8IIC_WriteData = (mu8Dev | EEP_OP_RD);
    IIC_TXD();
    if(gu8AckFail)
    {
        return  EEPROM_IIC_FAIL;
    }

    for (mu8Cnt = 0 ; mu8Cnt < mu8Len ; mu8Cnt++)
    {
        WDT_Restart();
        IIC_RXD();

        *pu8Data = gu8IIC_ReadData;
        pu8Data++;
    }

    IIC_Stop();

    return  EEPROM_IIC_OK;
}

#endif

#else

U8 gu8IICA0_WriteProc = 0;                      /// @brief  IIC Write 진행 단계
U8 gu8IICA0_WriteFailCnt = 0;                   /// @brief  IIC Write 실패 횟수
U8 gu8IICA0_WriteOkCnt = 0;                     /// @brief  IIC Write OK 횟수
U8 gu8IICA0_ReadProc = 0;                       /// @brief  IIC Read 진행 단계
U8 gu8IICA0_ReadFailCnt = 0;                    /// @brief  IIC Read 실패 횟수
U8 gu8IICA0_ReadOkCnt = 0;                      /// @brief  IIC Read OK 횟수


/// @brief      EEPROM IIC Write Process 완료 처리 함수
/// @details    r_Config_IICA0_callback_master_sendend 함수에 써서 EEPROM IIC Write 동작 완료 처리를 한다
/// @param      void
/// @return     void
void EEPROM_IICA0_WriteProcess_Done(void)
{
    gu8IICA0_WriteProc = EEP_PROC_DONE;
}


/// @brief      EEPROM IIC Read Process 완료 처리 함수
/// @details    r_Config_IICA0_callback_master_receiveend 함수에 써서 EEPROM IIC Read 동작 완료 처리를 한다
/// @param      void
/// @return     void
void EEPROM_IICA0_ReadProcess_Done(void)
{
    gu8IICA0_ReadProc = EEP_PROC_DONE;
}


/// @brief      IIC Write Cycle Time
/// @details    IIC Wtite Cycle 시간 Delay 동작을 수행한다
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
    }
}


#if (EEPROM_ADDR_BIT_SETTING == EEPROM_8BIT)                  // Address가 8bit

/// @brief      IIC Writing 1-Byte Process
/// @details    IIC Byte Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Address
///             mu8Data : Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_ByteWrite(U8 mu8Dev, U8 mu8Addr, U8 mu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8ComStatus = ByteWrite(mu8Dev, mu8Addr, mu8Data);

        if (mu8ComStatus == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }
    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Writing 1-Byte Process-1
/// @details    IIC Byte Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Address
///             mu8Data : Data
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 ByteWrite(U8 mu8Dev, U8 mu8Addr, U8 mu8Data)
{
    U8 mu8buf[2];
    U16 mu16WaitCycle = 10000;
    U8 mu8Return = 0;

    mu8buf[0] = mu8Addr;
    mu8buf[1] = mu8Data;

    gu8IICA0_WriteProc = EEP_PROC_START;

    mu8Return = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | EEP_OP_WR), mu8buf, 2, 200) );

    if (mu8Return != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();
    IICA0_WriteCycleTime();

    gu8IICA0_WriteOkCnt++;
    return  EEPROM_IIC_OK;
}


/// @brief      IIC Write page Process
/// @details    IIC Page Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_PageWrite(U8 mu8Dev, U8 mu8Addr , U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = PageWrite(mu8Dev, mu8Addr, pu8Data, mu8Len);

        if (mu8ComStatus == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Write page Process-1
/// @details    IIC Page Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 PageWrite(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Buf[EEP_PAGE_SIZE + 2];
    U16 mu16WaitCycle = 10000;
    U8 mu8Return = 0;

    mu8Buf[0] = mu8Addr;

    gu8IICA0_WriteProc = EEP_PROC_START;

    // Write Page..
    mu8Return = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | EEP_OP_WR), &mu8Buf[0], (mu8Len + 1), 200 ) );

    if (mu8Return != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;
        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;
            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();

    IICA0_WriteCycleTime();

    gu8IICA0_WriteOkCnt++;

    return  EEPROM_IIC_OK;
}


/// @brief      IIC Reading 1-Byte Process
/// @details    IIC Byte Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Address
///             pu8Data : Data Pointer
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_ByteRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = ByteRead(mu8Dev, mu8Addr, pu8Data);

        if (mu8ComStatus == EEPROM_IIC_OK)
        {
            return  EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return  EEPROM_IIC_FAIL;
}


/// @brief      IIC Reading 1-Byte Process-1
/// @details    IIC Byte Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Address
///             pu8Data : Data Pointer
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 ByteRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data)
{
    U8 mu8Buf[2] = {0};
    U16 mu16WaitCycle = 10000;
    U8 mu8ReturnW = 0;
    U8 mu8ReturnR = 0;

    mu8Buf[0] = mu8Addr;

    gu8IICA0_WriteProc = EEP_PROC_START;

    // Write address..
    mu8ReturnW = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | EEP_OP_WR), mu8Buf, 1, 200 ) );

    if (mu8ReturnW != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    gu8IICA0_ReadProc = EEP_PROC_START;

    mu8ReturnR = (U8)( IICA0_MASTER_RECEIVE_FUCTION( (mu8Dev | EEP_OP_RD), pu8Data, 1, 200 ) );

    // Read data..
    if (mu8ReturnR != MD_OK )
    {
        gu8IICA0_WriteFailCnt++;
        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_ReadProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_ReadFailCnt++;
            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();

    gu8IICA0_ReadOkCnt++;
    return  EEPROM_IIC_OK;
}


/// @brief      IIC Read Sequential Process
/// @details    IIC Page Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_SeqRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = SeqRead(mu8Dev, mu8Addr , pu8Data, mu8Len);

        if (mu8ComStatus == EEPROM_IIC_OK)
        {
            return  EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return  EEPROM_IIC_FAIL;
}


/// @brief      IIC Read Sequential Process-1
/// @details    IIC Page Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu8Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 SeqRead(U8 mu8Dev, U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Buf[2] = {0};
    U16 mu16WaitCycle = 10000;
    U8 mu8ReturnW = 0;
    U8 mu8ReturnR = 0;

    mu8Buf[0] = mu8Addr;

    gu8IICA0_WriteProc = EEP_PROC_START;

    mu8ReturnW = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | EEP_OP_WR), mu8Buf, 1, 200 ) );

    /* Send  Address */
    if (mu8ReturnW != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }

    /* Wait */
    while (gu8IICA0_WriteProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    gu8IICA0_ReadProc = EEP_PROC_START;

    mu8ReturnR = (U8)( IICA0_MASTER_RECEIVE_FUCTION( (mu8Dev | EEP_OP_RD), pu8Data, mu8Len, 200 ) );

    /* Recv Data */
    if (mu8ReturnR != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_ReadProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_ReadFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();
    gu8IICA0_ReadOkCnt++;
    return  EEPROM_IIC_OK;
}

#else                                   // Address가 16bit

/// @brief      IIC Writing 1-Byte Process
/// @details    IIC Byte Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             mu8Data : Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5회
    {
        mu8ComStatus = ByteWrite(mu8Dev, mu16Addr, mu8Data);

        if (mu8ComStatus == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }
    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Writing 1-Byte Process-1
/// @details    IIC Byte Write 동작을 수행한다
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

    gu8IICA0_WriteProc = EEP_PROC_START;

    mu8Return = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | EEP_OP_WR), mu8buf, 3, 200) );

    if (mu8Return != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();
    IICA0_WriteCycleTime();

    gu8IICA0_WriteOkCnt++;
    return  EEPROM_IIC_OK;
}


/// @brief      IIC Write page Process
/// @details    IIC Page Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_PageWrite(U8 mu8Dev, U16 mu16Addr , U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = PageWrite(mu8Dev, mu16Addr, pu8Data, mu8Len);

        if (mu8ComStatus == EEPROM_IIC_OK)
        {
            return EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return EEPROM_IIC_FAIL;
}


/// @brief      IIC Write page Process-1
/// @details    IIC Page Write 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Buf[EEP_PAGE_SIZE + 2];
    U16 mu16WaitCycle = 10000;
    U8 mu8Return = 0;

    mu8Buf[0] = (U8)((mu16Addr >> 8) & 0x00FF);
    mu8Buf[1] = (U8)(mu16Addr & 0x00FF);

    gu8IICA0_WriteProc = EEP_PROC_START;

    // Write Page..
    mu8Return = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | EEP_OP_WR), &mu8Buf[0], (mu8Len + 2), 200 ) );

    if (mu8Return != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;
        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;
            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();

    IICA0_WriteCycleTime();

    gu8IICA0_WriteOkCnt++;

    return  EEPROM_IIC_OK;
}


/// @brief      IIC Reading 1-Byte Process
/// @details    IIC Byte Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = ByteRead(mu8Dev, mu16Addr, pu8Data);

        if (mu8ComStatus == EEPROM_IIC_OK)
        {
            return  EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return  EEPROM_IIC_FAIL;
}


/// @brief      IIC Reading 1-Byte Process-1
/// @details    IIC Byte Read 동작을 수행한다
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

    gu8IICA0_WriteProc = EEP_PROC_START;

    // Write address..
    mu8ReturnW = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | EEP_OP_WR), mu8Buf, 2, 200 ) );

    if (mu8ReturnW != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_WriteProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if (mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    gu8IICA0_ReadProc = EEP_PROC_START;

    mu8ReturnR = (U8)( IICA0_MASTER_RECEIVE_FUCTION( (mu8Dev | EEP_OP_RD), pu8Data, 1, 200 ) );

    // Read data..
    if (mu8ReturnR != MD_OK )
    {
        gu8IICA0_WriteFailCnt++;
        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_ReadProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_ReadFailCnt++;
            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();

    gu8IICA0_ReadOkCnt++;
    return  EEPROM_IIC_OK;
}


/// @brief      IIC Read Sequential Process
/// @details    IIC Page Read 동작을 수행한다
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < EEPROM_IIC_RETRY_COUNT; mu8Cnt++)
    {
        mu8ComStatus = SeqRead(mu8Dev, mu16Addr , pu8Data, mu8Len);

        if (mu8ComStatus == EEPROM_IIC_OK)
        {
            return  EEPROM_IIC_OK;
        }
        else
        {
        }
    }

    return  EEPROM_IIC_FAIL;
}


/// @brief      IIC Read Sequential Process-1
/// @details    IIC Page Read 동작을 수행한다
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

    gu8IICA0_WriteProc = EEP_PROC_START;

    mu8ReturnW = (U8)( IICA0_MASTER_SEND_FUCTION( (mu8Dev | EEP_OP_WR), mu8Buf, 2, 200 ) );

    /* Send  Address */
    if (mu8ReturnW != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }

    /* Wait */
    while (gu8IICA0_WriteProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_WriteFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    gu8IICA0_ReadProc = EEP_PROC_START;

    mu8ReturnR = (U8)( IICA0_MASTER_RECEIVE_FUCTION( (mu8Dev | EEP_OP_RD), pu8Data, mu8Len, 200 ) );

    /* Recv Data */
    if (mu8ReturnR != MD_OK)
    {
        gu8IICA0_WriteFailCnt++;

        IICA0_Stop_Condition_Generate();
        return  EEPROM_IIC_FAIL;
    }
    else
    {
    }

    while (gu8IICA0_ReadProc != EEP_PROC_DONE)
    {
        mu16WaitCycle--;

        if(mu16WaitCycle <= 0)
        {
            gu8IICA0_ReadFailCnt++;

            IICA0_Stop_Condition_Generate();
            return  EEPROM_IIC_FAIL;
        }
        else
        {
        }
    }

    IICA0_Stop_Condition_Generate();
    gu8IICA0_ReadOkCnt++;
    return  EEPROM_IIC_OK;
}

#endif

/// @brief      IIC Communication Stop Condition
/// @details    IIC Stop 상태를 처리한다
/// @param      void
/// @return     mu8Return : 0(IIC_FAIL), 1(IIC_OK)
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
            gu8Status = EEPROM_IIC_FAIL;
            return  gu8Status;
        }
        else
        {
        }
    }

    gu8Status = EEPROM_IIC_OK;

    return  gu8Status;
}


#endif


/// @brief      EEPROM Data 전체 Read 함수
/// @details    EEPROM의 Data를 Page Read 한다
/// @param      void
/// @return     void
void EEPROM_Data_Load(void)
{
    EepromSeqRead(EEPROM_ID_START, SEEPROM_Data.au8Data, EEPROM_ID_MAX);
}


#if (EEPROM_ADDR_BIT_SETTING == EEPROM_8BIT)                  // Address가 8bit

/// @brief      EEPROM Data Byte Write 함수
/// @details    EEPROM에 Data를 Byte Write 처리한다
/// @param      mu8ID : 저장하려는 위치의 ID(Address)
///             mu8Data : 저장하려는 값
/// @return     void
void EepromByteWrite(U8 mu8ID, U8 mu8Data)
{
    HAL_IIC_BYTE_WRITE(DEV_ADDR_EEP, mu8ID, mu8Data);
}


/// @brief      EEPROM Data Byte Read 함수
/// @details    EEPROM에 Data를 Byte Read 처리한다
/// @param      mu8ID : Read 하려는 위치의 ID(Address)
/// @return     void
void EepromByteRead(U8 mu8ID)
{
    U8 mu8EEPROM_ReadDataBuf = 0;

    HAL_IIC_BYTE_READ(DEV_ADDR_EEP, mu8ID, &mu8EEPROM_ReadDataBuf);

    SEEPROM_Data.au8Data[mu8ID] = mu8EEPROM_ReadDataBuf;
}



/// @brief      EEPROM Data Page Write 함수
/// @details    EEPROM에 Data를 Page Write 처리한다
/// @param      mu8Addr : 저장하려는 Address
///             pu8Data : Data Pointer
///             mu8len : Length
/// @return     void
void EepromPageWrite(U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    HAL_IIC_PAGE_WRITE(DEV_ADDR_EEP, mu8Addr, pu8Data, mu8Len);
}


/// @brief      EEPROM Data Page Load 함수
/// @details    EEPROM에 Data를 Page Read 처리한다
/// @param      mu8Addr : 저장하려는 Address
///             pu8Data : Data Pointer
///             mu8len : Length
/// @return     void
void EepromSeqRead(U8 mu8Addr, U8* pu8Data, U8 mu8Len)
{
    HAL_IIC_SEQ_READ(DEV_ADDR_EEP, mu8Addr, pu8Data, mu8Len);
}


#else                                   // Address가 16bit


/// @brief      EEPROM Data Byte Write 함수
/// @details    EEPROM에 Data를 Byte Write 처리한다
/// @param      mu16ID : 저장하려는 위치의 ID(Address)
///             mu8Data : 저장하려는 값
/// @return     void
void EepromByteWrite(U16 mu16ID, U8 mu8Data)
{
    HAL_IIC_BYTE_WRITE(DEV_ADDR_EEP, mu16ID, mu8Data);
}


/// @brief      EEPROM Data Byte Read 함수
/// @details    EEPROM에 Data를 Byte Read 처리한다
/// @param      mu16ID : Read 하려는 위치의 ID(Address)
/// @return     void
void EepromByteRead(U16 mu16ID)
{
    U8 mu8EEPROM_ReadDataBuf = 0;

    HAL_IIC_BYTE_READ(DEV_ADDR_EEP, mu16ID, &mu8EEPROM_ReadDataBuf);

    SEEPROM_Data.au8Data[(U8)(mu16ID)] = mu8EEPROM_ReadDataBuf;
}



/// @brief      EEPROM Data Page Write 함수
/// @details    EEPROM에 Data를 Page Write 처리한다
/// @param      mu16Addr : 저장하려는 Address
///             pu8Data : Data Pointer
///             mu8len : Length
/// @return     void
void EepromPageWrite(U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    HAL_IIC_PAGE_WRITE(DEV_ADDR_EEP, mu16Addr, pu8Data, mu8Len);
}


/// @brief      EEPROM Data Load 함수
/// @details    EEPROM에 Data를 Page Read 처리한다
/// @param      mu16Addr : 저장하려는 Address
///             pu8Data : Data Pointer
///             mu8len : Length
/// @return     void
void EepromSeqRead(U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    HAL_IIC_SEQ_READ(DEV_ADDR_EEP, mu16Addr, pu8Data, mu8Len);
}


#endif


/// @brief      EEPROM 이상 여부 체크 함수
/// @details    EEPROM의 통신의 이상 여부를 체크하여 에러 발생 여부를 체크한다
/// @param      void
/// @return     return : EEPROM 데이터 이상 여부 - 0(정상), 1(비정상)
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


/// @brief      EEPROM Data 저장 함수
/// @details    EEPROM에 Data를 저장한다
/// @param      mu8ID : EEPROM DATA ID
///             mu8Data : Write 하려는 데이터
/// @return     void
void Set_EEPROM_Data(U8 mu8ID, U8 mu8Data)
{
    SEEPROM_Data.au8Data[mu8ID] = mu8Data;

    Save_EEPROM_Data(mu8ID);
}


/// @brief      EEPROM에 Data를 실제 저장 동작하는 함수
/// @details    EEPROM에 ID에 해당하는 Address에 해당하는 데이터를 저장 처리 한다
/// @param      mu8ID : EEPROM DATA ID
///             mu8Data : Write 하려는 데이터
/// @return     void
void Save_EEPROM_Data(U8 mu8ID)
{
    EepromByteWrite(mu8ID, SEEPROM_Data.au8Data[mu8ID]);
}


/// @brief      EEPROM Data Load 함수
/// @details    EEPROM에 저장된 데이터를 확인한다
/// @param      mu8ID : 확인하려고 하는 EEPROM DATA ID
/// @return     mu8Return : Read Data
U8 Get_EEPROM_Data(U8 mu8ID)
{
    U8 mu8Return = 0;

    mu8Return = SEEPROM_Data.au8Data[mu8ID];

    return  mu8Return;
}


#endif

