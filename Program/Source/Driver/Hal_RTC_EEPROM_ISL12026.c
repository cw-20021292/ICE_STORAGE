/// @file     Hal_RTC_EEPROM_ISL12026.c
/// @date     2025/03/24
/// @author   Jaejin Ham
/// @brief    RTC(EEPROM ����) ���� ���� ���� file

#include  "Global_Header.h"


#if (RTC_USE == USE)

//****************************************************************************
//   RTC ��� ��� Port - IICA0 or GPIO
//****************************************************************************

// RTC, EEPRON ��� IIC ���� ---------------------------------------------------------------

#if (RTC_IIC_GPIO_USE == RTC_IIC_GPIO)      // IIC����� GPIO�� ���� ���� �ϴ� ���

U8 gu8Txbit;                                /// @brief  I2C �۽Ž� Data�� bit
U8 gu8Rxbit;                                /// @brief  I2C ���Ž� Data�� bit
U8 gu8AckFail;                              /// @brief  I2C ���Ž� Ack üũ Flag

U8 gu8IIC_WriteData = 0;                    /// @brief  I2C �۽� ������
U8 gu8IIC_ReadData = 0;                     /// @brief  I2C ���� ������


/// @brief      IIC ��Ž� Pulse �� delay
/// @details    IIC ��Ž� Pulse �߻��� ���� Delay ��� �Ѵ�
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
/// @details    IIC Start Bit ������ �߻���Ų��
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
/// @details    IIC Stop Bit ������ �߻���Ų��
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
/// @details    IIC Data�� ���� Bit ������ �߻���Ų��
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
/// @details    IIC Data�� Bit Input üũ ��� �Ѵ�
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
/// @details    IIC Data �۽� ���� ���� ��� �Ѵ�
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
/// @details    IIC Data ���� ���� ���� ��� �Ѵ�
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
/// @details    IIC Byte Write ���� ��� �Ѵ�
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             mu8Data : Write Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5ȸ
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
/// @details    IIC Byte Write ���� ��� �Ѵ�
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
/// @details    IIC Page Write ���� ��� �Ѵ�
/// @param      mu8Dev : Device Address
///             mu16Addr : Write Address
///             pu8Data : Write Data
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_PageWrite(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5ȸ
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
/// @details    IIC Page Write ���� ��� �Ѵ�
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

    // ������ �۽�
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
/// @details    IIC Byte Read ���� ��� �Ѵ�
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_ByteRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5ȸ
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
/// @details    IIC Byte Read ���� ��� �Ѵ�
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
/// @details    IIC Page Read ���� ��� �Ѵ�
/// @param      mu8Dev : Device Address
///             mu16Addr : Read Address
///             pu8Data : Data Pointer
///             mu8Len : Length
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IIC_SeqRead(U8 mu8Dev, U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    U8 mu8Cnt = 0;
    U8 mu8Status = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5ȸ
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
/// @details    IIC Page Read ���� ��� �Ѵ�
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


/// @brief      EEPROM IIC Write Process �Ϸ� ó�� �Լ�
/// @details    EEPROM IIC Write ���� �Ϸ� ���°��� �����Ѵ�
/// @param      void
/// @return     void
void EEPROM_IICA0_WriteProcess_Done(void)
{
    gu8IICA0_WriteProc = RTC_PROC_START;
}


/// @brief      EEPROM IIC Read Process �Ϸ� ó�� �Լ�
/// @details    EEPROM IIC Read ���� �Ϸ� ���°��� �����Ѵ�
/// @param      void
/// @return     void
void EEPROM_IICA0_ReadProcess_Done(void)
{
    gu8IICA0_ReadProc = RTC_PROC_DONE;
}


/// @brief      IIC Write Cycle Time
/// @details    IIC Write �� Delay ��� �Ѵ�
/// @param      void
/// @return     void
void IICA0_WriteCycleTime(void)
{
    U16 mu16temp = 0;

    /*..hui [17-12-8���� 1:16:26] �� 5.3ms..*/
    /*..hui [19-10-21���� 1:25:35] �� 16ms, typical 12ms, max 20ms..*/
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
/// @details    IIC Byte Write ���� ��� �Ѵ�
/// @param      mu8Dev : Device Address
///             mu16Addr : Address
///             mu8Data : Data
/// @return     return : 0(IIC_FAIL), 1(IIC_OK)
U8 Hal_IICA0_ByteWrite(U8 mu8Dev, U16 mu16Addr, U8 mu8Data)
{
    U8 mu8Cnt = 0;
    U8 mu8ComStatus = 0;

    for (mu8Cnt = 0; mu8Cnt < RTC_IIC_RETRY_COUNT; mu8Cnt++)    // Retry 5ȸ
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
/// @details    IIC Byte Write ���� ��� �Ѵ�
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
/// @details    IIC Page Write ���� ��� �Ѵ�
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
/// @details    IIC Page Write ���� ��� �Ѵ�
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
/// @details    IIC Byte Read ���� ��� �Ѵ�
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
/// @details    IIC Byte Read ���� ��� �Ѵ�
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
/// @details    IIC Page Read ���� ��� �Ѵ�
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
/// @details    IIC Page Read ���� ��� �Ѵ�
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
/// @details    IIC ��ſ��� ��� ���� �Ϸ� ���θ� Ȯ���Ѵ�
/// @param      void
/// @return     mu8Return : IIC ���� �Ϸ� ���� - 0(IIC_FAIL), 1(IIC_OK)
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


// RTC ����  �Լ��� -------------------------------------------------------------------------------------

SRTC_TimeData_T     SRTC_Time;
SRTC_TimeData_T     SRTC_Time_Before;

SRealTimeClock_T  SRTC;

U16 gu16RTC_ProcessControlTimer = 0;                    /// @brief  RTC ���� ���� �ֱ� Ÿ�̸�

U16 gu16RTC_ErrorCnt = 0;                               /// @brief  RTC ���� ���� Ƚ�� ī����
U16 gu16RTC_ErrorTimer = 0;                             /// @brief  RTC ���� ���� üũ �ð�

U8 gu8RTC_ErrorState = 0;                               /// @brief  RTC ���� ���� �߻� ���� - 0(�̹߻�), 1(�߻�)
U8 gu8RTC_Comm_ErrorState = 0;                          /// @brief  RTC ��� ���� �߻� ���� - 0(�̹߻�), 1(�߻�)

U8 gu8RTC_GarbageTimer = 0;                             /// @brief  RTC �� �̻� ���� ���� �ð�

U8 gu8RTC_StopErrorTimer = 0;                           /// @brief  RTC �� �̻� ���� �ð�
U8 gu8RTC_StopErrorState = 0;                           /// @brief  RTC �� �̻� ���� ���� - 0(�̹߻�), 1(�߻�)

U8 gu8RTC_ReadTimer = 0;                                /// @brief  RTC Read �ֱ� �ð�
U8 gu8RTC_TimeRead = 0;                                 /// @brief  RTC Read ���� - 0(Read ����), 1(Read �Ϸ�)


/// @brief      RTC ���� Delay ms
/// @details    RTC ���� ms ���� Delay ��� �Ѵ�
/// @param      void
/// @return     void
void RTC_Delay_ms(U16 mu16ms)
{
    U16 mu16i = 0;
    U8 mu8j = 0;

    /*..hui [19-10-21���� 1:32:34] ���� �� 86.8ms..*/
    for (mu16i = 0 ; mu16i < mu16ms ; mu16i++)
    {
        for (mu8j = 0; mu8j < 4 ; mu8j++)   // 1ms loop delay
        {
            RTC_Delay_us( 250 );
            WDT_Restart();
        }
    }
}


/// @brief      RTC ���� Delay us
/// @details    RTC ���� us ���� Delay ��� �Ѵ�
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


/// @brief      RTC ���� �ʱ�ȭ
/// @details    RTC ���� �������� ��� �ʱ�ȭ ��Ų��
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

    // ���� ���͸� ���� �����ϴ� ���̸�, ��õ�...
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
        // ������ ������ ��� �ʱ�ȭ
        InitRTC_Time();
    }
    else
    {
        ReadRTC_Time();

        if(SRTC.gu8InitSuccess == SET)
        {
            // �ð��� ���� ���� �̿��� ������ Read �Ǵ� ��� ������ �ʱ�ȭ
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
            // ��ſ� ������ ��� ���� �ʱ�ȭ
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
/// @details    RTC�� CCR ���¸� Enable ��Ų��
/// @param      void
/// @return     void
static void EnableRtcCCR(void)
{
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x02);
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x06);
    RTC_Delay_ms(200);
}


/// @brief      Disable RTC CCR
/// @details    RTC�� CCR ���¸� disable ��Ų��
/// @param      void
/// @return     void
static void DisableRtcCCR(void)
{
    HAL_IIC_BYTE_WRITE(DEV_ADDR_RTC, ADDR_RTC_STATUS, 0x00);
}


/// @brief      RTC Reset Check
/// @details    RTC�� Reset ���θ� üũ�Ѵ�
/// @param      void
/// @return     void
U8 IsRTC_Reset(void)
{
    if ( (SRTC.gu8RegStatus & (MASK_RTCF)) == 0 )
    {
        return  CLEAR;   // ����
    }
    else
    {
    }

    return  SET;         //ERROR
}


/// @brief      RTC ���� Process ���� �ð� ī����(@1ms)
/// @details    RTC ���� ���� �ֱ� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void RTC_ProcessControlTimer(void)
{
    if (gu16RTC_ProcessControlTimer < RTC_PROCESS_CONTROL_TIME_PERIOD)
    {
        gu16RTC_ProcessControlTimer++;
    }
}


/// @brief      RTC ���� Process �Լ�(@While)
/// @details    RTC ���� ������ �Ѵ�
/// @param      void
/// @return     void
void ProcessRTC(void)
{
    U8 mu8RtcErr = 0;
    U8 mu8Return = 0;

    if (gu16RTC_ProcessControlTimer >= RTC_PROCESS_CONTROL_TIME_PERIOD)
    {   // 1�ʸ��� ����
        gu16RTC_ProcessControlTimer = 0;

        mu8Return = RTC_OverMaxYear();

        // �⵵ �ִ� ���� ����
        if (mu8Return == SET)
        {   // �ִ� �⵵�� �ʰ��Ǹ� �ʱ�ȭ ��
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
            // 20�� ���� �ѹ��� �б�
            gu8RTC_ReadTimer = 0;

            // Read RTC Status
            HAL_IIC_BYTE_READ(DEV_ADDR_RTC, ADDR_RTC_STATUS, &SRTC.gu8RegStatus);

            /*..hui [20-6-11���� 9:38:33] �����ǰų� ũ����Ż �ҷ��� ����..*/
            mu8RtcErr = IsRTC_Error();

            // RTC ����
            if (mu8RtcErr == SET)
            {
                // RTC ������ ����
                gu16RTC_ErrorCnt++;

                /*..hui [20-2-18���� 10:00:22] ������������ �ð� �ʱ�ȭ�� ���ϴ°ɷ�..*/
                /*..hui [20-2-18���� 10:00:36] �׳� �����ð����� �̾ ���۵ǵ���.. �׷��� ������ �����״�..*/
                /*..hui [20-2-18���� 1:38:42] ���µǸ� RTC ��ü���� �ð� �ʱ�ȭ��..*/
                /*..hui [20-4-17���� 8:41:41] �ִ� 100�������� ����..*/

                gu8RTC_ErrorState = SET;
            }
            else
            {
                // RTC ���� ���� Ŭ����
                gu16RTC_ErrorTimer = 0;
                gu8RTC_ErrorState = CLEAR;
            }

            ReadRTC_Time();
            gu8RTC_TimeRead = SET;

            // �ð��� ���� ���� ���� �̿��� ��쿡�� ������ �ʱ�ȭ
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

                // 9ȸ ���� ������ ���� ���� ��� �ʱ�ȭ
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



/// @brief      RTC�� �⵵ ���� Max ������ �ʰ� ���� Ȯ�� �Լ�
/// @details    RTC�� �⵵ ���� Max ������ ���� �ʰ��ϴ��� ���θ� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : RTC�� �⵵�� Max���� �Ѵ��� ���� - 0(No), 1(Yes)
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



/// @brief      RTC�� Battery ���� ���� ���� Ȯ�� �Լ�
/// @details    RTC�� Battery ���� ���� ���θ� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     mu8Return : ���͸� ���� ���� ���� - 0(Vcc Mode), 1(Battery Backup Mode)
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


/// @brief      RTC���� �ð� Read �Լ�
/// @details    RTC���� �ð����� Read �Ѵ�
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
        // RTC ��� ����
        gu8RTC_Comm_ErrorState = SET;
        return;
    }
    else
    {
    }

    // �������� �б����� �����ð� ���
    while (mu16WaitCycle--)
    {
        ;
    }

    // Read aging time buffer
    mu8Ret_2 = HAL_IIC_SEQ_READ(DEV_ADDR_RTC, ADDR_RTC_SECONDS, SRTC.au8Readbuf2, 8);

    if( mu8Ret_2 == FALSE )
    {
        // RTC ��� ����
        gu8RTC_Comm_ErrorState = SET;
        return;
    }
    else
    {
    }

    // [0] ������ sec ���� �����ϰ� ������ �ð��� �����ϸ� �ð��� ������.
    // read �������� �ð� ���� noise�� ���ؼ� �ջ�Ǵ� ���� ���� ���� ���� �����
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


/// @brief      RTC�� �ð� Write �Լ�
/// @details    RTC�� �ð����� Write �Ѵ�
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


/// @brief      RTC �ð� �ʱ�ȭ �Լ�
/// @details    RTC�� �ð����� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void InitRTC_Time(void)
{
    // RTC read�� �ʱ�ȭ
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


/// @brief      RTC IC ���� Ȯ��
/// @details    RTC IC�� ���ۻ� ���� ���¸� Ȯ���Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� - 0(����), 1(����)
U8 IsRTC_Error(void)
{
    if ( (SRTC.gu8RegStatus & (MASK_OSCF|MASK_RTCF)) == 0 )
    {
        return  CLEAR; // ����
    }
    else
    {
    }

    return  SET; //ERROR
}


/// @brief      RTC IC OSC ���� Ȯ��
/// @details    RTC IC�� OSC�� ���� ���¸� Ȯ���Ѵ�
/// @param      void
/// @return     mu8Return : ���� ���� ���� - 0(����), 1(����)
U8 IsRTC_OSCF_Error(void)
{
    if ( (SRTC.gu8RegStatus & (MASK_OSCF)) == 0 )
    {
        return  CLEAR; // ����
    }
    else
    {
    }

    return  SET; //ERROR
}



/// @brief      RTC Error Check
/// @details    RTC�� ���� ���� ���¸� üũ�Ѵ�
/// @param      void
/// @return     void
void RTC_ErrorCheck(void)
{
    /*..hui [20-6-18���� 3:59:08] RTC �ð��� ������� ���� ��� LCD �ð����� ��ȯ�ϱ� ���� �߰�..*/
    if ( (SRTC_Time.gu8Year == SRTC_Time_Before.gu8Year) &&
         (SRTC_Time.gu8Month == SRTC_Time_Before.gu8Month) &&
         (SRTC_Time.gu8Date == SRTC_Time_Before.gu8Date) &&
         (SRTC_Time.gu8Hour == SRTC_Time_Before.gu8Hour) &&
         (SRTC_Time.gu8Min == SRTC_Time_Before.gu8Min) &&
         (SRTC_Time.gu8Sec == SRTC_Time_Before.gu8Sec) )
    {
        gu8RTC_StopErrorTimer++;

        /*..hui [20-6-19���� 1:38:00] 1�п���..*/
        /*..hui [20-6-19���� 1:46:23] ������ RTC �ٽ� �о����� ������ ������ �׶����� �ٽ� LCD�� �������ָ� �ǹǷ�..*/
        /*..hui [20-6-19���� 1:46:35] �׳� �ѹ��� �����ϰ� ������ LCD �ð� ������ ���ϴ°ɷ�..*/
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



/// @brief      �ð� ���� �Լ�
/// @details    RTC�� �ð����� Write �Ѵ�
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


/// @brief      �ð� Ȯ�� �Լ�
/// @details    RTC���� Read�� �ð����� Ȯ���Ѵ�
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



// EERPOM ����  �Լ��� -------------------------------------------------------------------------------------

U8 gu8EEPROM_CommunicationError;                    /// @brief  EEPROM ��� �ҷ� ���� �߻� ���� - 0(No Error), 1(Error)

/// @brief  EEPROM ���� �ڷ���
typedef struct {
    U16 au16Address[EEPROM_ID_MAX];
    U8 au8Data[EEPROM_ID_MAX];

}   SEEPROM_Data_T;

SEEPROM_Data_T  SEEPROM_Data;


/// @brief      EEPROM Data ��ü Read �Լ�
/// @details    EEPROM �����͸� ��ü Read �Ѵ�
/// @param      void
/// @return     void
void EEPROM_Data_Load(void)
{
    EepromSeqRead(EEPROM_ID_START, SEEPROM_Data.au8Data, EEPROM_ID_MAX);
}


/// @brief      EEPROM Data Byte Write �Լ�
/// @details    EEPROM Data�� Byte Write ���� �Ѵ�
/// @param      mu16ID : �����Ϸ��� ��ġ�� ID(Address)
///             mu8Data : �����Ϸ��� ��
/// @return     void
void EepromByteWrite(U16 mu16ID, U8 mu8Data)
{
    HAL_IIC_BYTE_WRITE(DEV_ADDR_EEP, mu16ID, mu8Data);
}


/// @brief      EEPROM Data Byte Read �Լ�
/// @details    EEPROM Data�� Byte Read ���� �Ѵ�
/// @param      mu16ID : Read �Ϸ��� ��ġ�� ID(Address)
/// @return     void
void EepromByteRead(U16 mu16ID)

{
    U8 mu8EEPROM_ReadDataBuf = 0;

    HAL_IIC_BYTE_READ(DEV_ADDR_EEP, mu16ID, &mu8EEPROM_ReadDataBuf);

    SEEPROM_Data.au8Data[(U8)(mu16ID)] = mu8EEPROM_ReadDataBuf;
}



/// @brief      EEPROM Data Load �Լ�
/// @details    EEPROM �����͸� Page Write ���� �Ѵ�
/// @param      mu16Addr : �����Ϸ��� Address
///             pu8Data : Data Pointer
///             mu8len : Length
/// @return     void
void EepromPageWrite(U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    HAL_IIC_PAGE_WRITE(DEV_ADDR_EEP, mu16Addr, pu8Data, mu8Len);
}


/// @brief      EEPROM Data Load �Լ�
/// @details    EEPROM �����͸� Page Read ������ �Ѵ�
/// @param      mu16Addr : �����Ϸ��� Address
///             pu8Data : Data Pointer
///             mu8len : Length
/// @return     void
void EepromSeqRead(U16 mu16Addr, U8* pu8Data, U8 mu8Len)
{
    HAL_IIC_SEQ_READ(DEV_ADDR_EEP, mu16Addr, pu8Data, mu8Len);
}


/// @brief      EEPROM �̻� ���� üũ �Լ�
/// @details    EEPROM�� ����� �̻� ���θ� üũ�Ͽ� ���� �߻� ���θ� üũ�Ѵ�
/// @param      void
/// @return     return : EEPROM ������ �̻� ���� : 0(����), 1(������)
U8 Get_EepromErrorCheck(void)
{
    U8 mu8EEPROM_CheckA = 0;
    U8 mu8EEPROM_CheckB = 0;

    EepromSeqRead(EEPROM_ID_START, &mu8EEPROM_CheckA, 1);
    EepromSeqRead(EEPROM_ID_END, &mu8EEPROM_CheckB, 1);

    if ( (mu8EEPROM_CheckA != VALUE_DEFINE_EEPROM_DATA_START) || (mu8EEPROM_CheckB != VALUE_DEFINE_EEPROM_DATA_END) )
    {   // EEPROM ������ ������
        return  TRUE;
    }
    else
    {   // EEPRM ������ ����
        return  FALSE;
    }
}


/// @brief      EEPROM Data ���� �Լ�
/// @details    EEPROM Data�� �����ϰ� Write �Ѵ�
/// @param      mu8ID : EEPROM DATA ID
///             mu8Data : Write �Ϸ��� ������
/// @return     void
void Set_EEPROM_Data(U8 mu8ID, U8 mu8Data)
{
    SEEPROM_Data.au8Data[mu8ID] = mu8Data;

    Save_EEPROM_Data(mu8ID);
}


/// @brief      EEPROM�� Data�� �����ϴ� �Լ�
/// @details    EEPROM�� ID�� �ش��ϴ� Address�� �ش��ϴ� �����͸� �����Ѵ�
/// @param      mu8ID : EEPROM DATA ID
///             mu8Data : Write �Ϸ��� ������
/// @return     void
void Save_EEPROM_Data(U8 mu8ID)
{
    EepromByteWrite((U16)(mu8ID), SEEPROM_Data.au8Data[mu8ID]);
}


/// @brief      EEPROM Data Load �Լ�
/// @details    EEPROM �����͸� Ȯ�� �Ѵ�
/// @param      mu8ID : Ȯ���Ϸ��� �ϴ� EEPROM DATA ID
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
/// @details    RTC ���� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_RTC_Module_Initialize(void)
{
#if (RTC_USE == USE)
    RTC_Initialize();                       // RTC ���� �ʱ�ȭ
#endif
}


/// @brief      Hal RTC Module in 1ms Interrupt
/// @details    1ms Interrupt �ȿ��� RTC ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_RTC_Module_1ms_Control(void)
{
#if (RTC_USE == USE)
    RTC_ProcessControlTimer();              // RTC Process Control Time Counter
#endif
}


/// @brief      Hal RTC Module in While Loop
/// @details    Basic Loop �� While �� �ȿ��� RTC ��� �Ѵ�
/// @param      void
/// @return     void
void Hal_RTC_Module_Control(void)
{
#if (RTC_USE == USE)
    ProcessRTC();                           // RTC ���� ���� �Լ�
#endif
}

