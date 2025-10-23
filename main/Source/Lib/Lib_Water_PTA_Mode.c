/// @file     Lib_Water_PTA_Mode.c
/// @date     2025/06/27
/// @author   Jaejin Ham
/// @brief    ���� �ڵ�ȭ(PTA) �׽�Ʈ ��� ���� File

#include "Global_Header.h"

#if (PTA_TEST_MODE_USE == USE)

U8 gu8PTA_ControlTimer = 0;                             /// @brief  PTA�˻� ��� ���� �ð� Ÿ�̸�
U16 gu16PTA_Mode_InTimeOut = 0;                         /// @brief  PTA�˻� ��� ���� ���� Time Out

U8 gu8PTA_UART_Rx_OK = 0;                               /// @brief  PTA�˻� ��� ���۽� UART RX �Ϸ� ���� - 0(���� �̿Ϸ�), 1(���� �Ϸ�)
U8 gu8PTA_UART_Rx_Error = 0;                            /// @brief  PTA�˻� ��� ���۽� UART RX Error�߻� ���� - 0(�̹߻�), 1(�߻�)
U8 gu8PCB_Teat_PTA_UART = 0;                            /// @brief  PCB�˻� ��� ���۽� PTA�˻�� UART �˻� ���� ���� ǥ�� ��� ����

U8 ga8PTA_TXD_Data[PTA_UART_DATA_LENGTH];               /// @brief  PTA�˻� UART TX ������ ����
U8 ga8PTA_RXD_Data[PTA_UART_DATA_LENGTH];               /// @brief  PTA�˻� UART RX ������ ����

U8 gu8PTA_UART_Rx_DataCount = 0;                        /// @brief  PTA�˻� UART RX ������ ī��Ʈ
U8 gu8PTA_UART_Tx_DataCount = 0;                        /// @brief  PTA�˻� UART TX ������ ī��Ʈ
U8 gu8PTA_UART_RXD_CheckTimeOut = 0;                    /// @brief  PTA�˻� UART RXD ��� Time out
U8 gu8PTA_UART_TXD_Length = 0;                          /// @brief  PTA�˻� UART TXD ������ Length

U8 gu8FrontPBA_SW_Version = 0;                          /// @brief  Front PBA SW ����
U8 gu8MainPBA_SW_Version = 0;                           /// @brief  Main PBA SW ����

U8 gu8OperatingPTAMode = 0;                             /// @brief  �������� PTA ��� �ܰ�

U8 gu8PTA_LeakTestEnd = 0;                              /// @brief  ���� ���� �׽�Ʈ �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 gu8PTA_KeyTestEnd = 0;                               /// @brief  Ű �׽�Ʈ �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 gu8PTA_Mode1_End = 0;                                /// @brief  PTA �׽�Ʈ ���1 �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 gu8PTA_HeaterTestEnd = 0;                            /// @brief  PTA �׽�Ʈ ����� ���� ���� �׽�Ʈ �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 gu8PTA_TestEnd = 0;                                  /// @brief  PTA �׽�Ʈ �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)

U16 gu16PTA_HeaterActTimer = 0;                         /// @brief  �ڵ�ȭ �˻� ��忡�� ���� �׽�Ʈ ���� Timer

U8 gu8PTA_AT_EEPROM = 0;                                /// @brief  PTA �˻� ���۽� Read�� EEPROM ������
U16 gu16PTA_RXD_Command = 0;                            /// @brief  ������ �׽�Ʈ ���� Command
U8 gu8PTA_RXD_Data1 = 0;                                /// @brief  ������ �׽�Ʈ ������1
U8 gu8PTA_RXD_CS = 0;                                   /// @brief  ������ CheckSum
U8 gu8PTA_TXD_CS = 0;                                   /// @brief  �۽��� CheckSum

U8 gu8PTA_Mode_Boot = 0;                                /// @brief  PTA��� ���� ���� ���� - 0(�̵���), 1(����)
U16 gu16PTA_Mode_BootAct_Timer = 0;                     /// @brief  PTA��� ���� ���� �ð�(@100ms)


/// @brief      �ڵ�ȭ �˻� ���� ������ �ʱ�ȭ �Լ�
/// @details    �ڵ�ȭ �˻� ���� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_PTA_Mode_Initialize(void)
{
    gu8PTA_ControlTimer = 0;
    gu16PTA_Mode_InTimeOut = PTA_MODE_IN_TIME_OUT;              // ���� �ΰ� �� PTA �˻� ���� ���� �ð� üũ

    gu8PTA_UART_Rx_OK = 0;
    gu8PTA_UART_Rx_Error = 0;
    gu8PCB_Teat_PTA_UART = 0;

    MEMSET(ga8PTA_TXD_Data, '\0', sizeof(ga8PTA_TXD_Data));
    MEMSET(ga8PTA_RXD_Data, '\0', sizeof(ga8PTA_RXD_Data));

    gu8PTA_UART_Rx_DataCount = 0;
    gu8PTA_UART_Tx_DataCount = 0;
    gu8PTA_UART_RXD_CheckTimeOut = 0;
    gu8PTA_UART_TXD_Length = 0;

    gu8FrontPBA_SW_Version = 0;
    gu8MainPBA_SW_Version = 0;

    gu8OperatingPTAMode = PTA_MODE_NONE;

    gu8PTA_LeakTestEnd = 0;
    gu8PTA_KeyTestEnd = 0;
    gu8PTA_Mode1_End = CLEAR;
    gu8PTA_HeaterTestEnd = CLEAR;
    gu8PTA_TestEnd = CLEAR;

    gu16PTA_HeaterActTimer = 0;

    gu8PTA_AT_EEPROM = 0;
    gu16PTA_RXD_Command = 0;
    gu8PTA_RXD_Data1 = 0;
    gu8PTA_RXD_CS = 0;
    gu8PTA_TXD_CS = 0;

    gu8PTA_Mode_Boot = 0;
    gu16PTA_Mode_BootAct_Timer = 0;
}


/// @brief      ������ ���� ���� �Լ�
/// @details    ������ ó���� ��� HW ���ϸ� OFF ��Ų��
/// @param      void
/// @return     void
void NoPowerSet(void)
{
    TURN_OFF_HEATER_1();

    TURN_OFF_VALVE_1();

    TURN_OFF_BOOST_OUT();
}


/// @brief      PTA �˻� ��� UART TXD �۽� ���� �Լ�
/// @details    PTA �˻� ��忡�� UART�� TXD �����͸� �۽� �����Ѵ�
/// @param      void
/// @return     void
void PTA_UART_TXD_Start(void)
{
    U8 mu8OperatingModeStatus = 0;

    mu8OperatingModeStatus = Get_OperationMode();          // ���� ���� ���� ��� Ȯ��

    if ( (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_1) ||
         (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_2) ||
         (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_3) )
    {   // PTA �˻� ����� ���
        if (gu8PTA_UART_TXD_Length)
        {   // TX�� �����Ͱ� �ִ� ���
            PTA_UART_TXD_INTERRUPT_REGISTER = 1U;           // disable INTSTx interrupt
            PTA_UART_TXD_DATA_REGISTER = ga8PTA_TXD_Data[gu8PTA_UART_Tx_DataCount];
            gu8PTA_UART_Tx_DataCount++;
            PTA_UART_TXD_INTERRUPT_REGISTER = 0U;           // enable INTSTx interrupt
        }
        else
        {
            gu8PTA_UART_Tx_DataCount = 0;
        }
    }
    else if (mu8OperatingModeStatus == OPERATION_MODE_ID_PCB_TEST)
    {   // PCB �˻� ��� �� ���
        PTA_UART_TXD_INTERRUPT_REGISTER = 1U;               // disable INTSTx interrupt
        PTA_UART_TXD_DATA_REGISTER = AT_RS232_ETX;
        PTA_UART_TXD_INTERRUPT_REGISTER = 0U;               // enable INTSTx interrupt

        gu8PTA_UART_RXD_CheckTimeOut = 3;                   // Rx üũ Time out 300ms
    }
}


/// @brief      PTA �˻� ��� UART TXD Interrupt ó�� �Լ�(@UART Txd Interrupt)
/// @details    PTA �˻��� URAT ��Ű� Tx Interrupt ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART_ISR_PTA_Tx(void)
{
    if (gu8PTA_UART_Tx_DataCount < (PTA_UART_DATA_LENGTH - 1))
    {
        if (gu8PTA_UART_Tx_DataCount <= gu8PTA_UART_TXD_Length)
        {
            PTA_UART_TXD_DATA_REGISTER = ga8PTA_TXD_Data[gu8PTA_UART_Tx_DataCount];
            gu8PTA_UART_Tx_DataCount++;
        }
        else
        {
            gu8PTA_UART_Tx_DataCount = 0;
            MEMSET(ga8PTA_TXD_Data, '\0', sizeof(ga8PTA_TXD_Data));
        }
    }
    else
    {
        gu8PTA_UART_Tx_DataCount = 0;
        MEMSET(ga8PTA_TXD_Data, '\0', sizeof(ga8PTA_TXD_Data));
    }
}


/// @brief      PTA �˻� ��� UART RXD Interrupt ó�� �Լ�(@UART Rxd Interrupt)
/// @details    PTA �˻��� URAT ��Ű� Rx Interrupt ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART_ISR_PTA_Rx(void)
{
    U8 mu8RxData;
    U8 mu8OperatingModeStatus = 0;

    mu8RxData = PTA_UART_RXD_DATA_REGISTER;                 // RXD ������ Read
    mu8OperatingModeStatus = Get_OperationMode();           // ���� ���� ���� ��� Ȯ��

    if (mu8OperatingModeStatus == OPERATION_MODE_ID_PCB_TEST)
    {   // PCB �׽�Ʈ ��忡��
        if (mu8RxData == AT_RS232_ETX)
        {
            gu8PCB_Teat_PTA_UART ^= SET;
            gu8PTA_UART_RXD_CheckTimeOut = 0;
        }
    }
    else
    {   // PCB �׽�Ʈ ��尡 �ƴ� ���
        if (gu8PTA_UART_Rx_OK == CLEAR)
        {   // ó���ؾ��� RX �����Ͱ� ���� ���
            if ( (gu8PTA_UART_Rx_DataCount == 0) && (mu8RxData != AT_RS232_STX) )
            {   // ù��° �����Ͱ� STX�� �ƴ� ���
                MEMSET(ga8PTA_RXD_Data, '\0', sizeof(ga8PTA_RXD_Data));
                gu8PTA_UART_Rx_DataCount = 0;
                gu8PTA_UART_RXD_CheckTimeOut = 0;
            }
            else
            {
                ga8PTA_RXD_Data[gu8PTA_UART_Rx_DataCount] = mu8RxData;

                if (ga8PTA_RXD_Data[gu8PTA_UART_Rx_DataCount] == AT_RS232_ETX)
                {
                    gu8PTA_UART_RXD_CheckTimeOut = 0;
                    gu8PTA_UART_Rx_OK = SET;
                }
                else
                {
                    if (gu8PTA_UART_Rx_DataCount >= PTA_UART_DATA_LENGTH)
                    {   // �ִ� ������ �Ѵ� �����Ͱ� ���ŵǴ� ���
                        MEMSET(ga8PTA_RXD_Data, '\0', sizeof(ga8PTA_RXD_Data));
                        gu8PTA_UART_Rx_DataCount = 0;
                        gu8PTA_UART_RXD_CheckTimeOut = 0;
                    }
                    else
                    {
                        gu8PTA_UART_Rx_DataCount++;
                        gu8PTA_UART_RXD_CheckTimeOut = 3;    // Rx üũ Time out 300ms
                    }
                }
            }
        }
    }
}


/// @brief      PTA UART Rx Time Out üũ(@100ms)
/// @details    PTA �˻� UART Rx �����͸� �����ϴ� Time Out�� ī��Ʈ�Ѵ�
/// @param      void
/// @return     void
void PTA_UART_Rx_TimeOut(void)
{
    U8 mu8OperatingModeStatus = 0;

    mu8OperatingModeStatus = Get_OperationMode();           // ���� ���� ���� ��� Ȯ��

    if (gu8PTA_UART_RXD_CheckTimeOut)
    {
        gu8PTA_UART_RXD_CheckTimeOut--;

        if (gu8PTA_UART_RXD_CheckTimeOut == 0)
        {
            if (mu8OperatingModeStatus == OPERATION_MODE_ID_PCB_TEST)
            {   // PCB�׽�Ʈ ����� ���
                gu8PCB_Teat_PTA_UART = CLEAR;
            }
            else
            {   // PCB�׽�Ʈ ��尡 �ƴ� ���
                MEMSET(ga8PTA_RXD_Data, '\0', sizeof(ga8PTA_RXD_Data));
                gu8PTA_UART_Rx_DataCount = 0;
                gu8PTA_UART_Rx_OK = CLEAR;
            }
        }
    }
}


/// @brief      PTA UART Error Interrupt(Interrupt ��ƾ)
/// @brief      PTA �˻� UART RX Error ���ͷ�Ʈ �߻��� ó���Ѵ�
/// @param      void
/// @return     void
void UART_ISR_PTA_Rx_Error(void)
{
    U8 mu8RxData;

    mu8RxData = PTA_UART_RXD_DATA_REGISTER;

    PTA_UART_RXD_ERROR_INTERRUPT_REGISTER = 0x07;

    MEMSET(ga8PTA_RXD_Data, '\0', sizeof(ga8PTA_RXD_Data));

    gu8PTA_UART_Rx_DataCount = 0;
    gu8PTA_UART_Rx_OK = CLEAR;
}


/// @brief      �ڵ�ȭ �˻� ���۽� EEPROM ���� üũ �Լ�
/// @details    �ڵ�ȭ �˻� ���۽� EEPROM �����͸� �о �� �����͸� �������� �����Ϳ� �Ǿ� ���� �� �ְ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : EEPROM üũ ������(END �ڸ� ������)
U8 Get_PTA_CheckEEPROM(void)
{
    U8 mu8CheckData = 0;
    EepromSeqRead(EEPROM_ID_END, &mu8CheckData, 1);

    gu8PTA_AT_EEPROM = mu8CheckData;

    return  gu8PTA_AT_EEPROM;
}



/// @brief      �ڵ�ȭ �˻�� ���� ���� �� ���� ���� TimeOut �Լ�(@1ms)
/// @details    �ڵ�ȭ �˻� �� ���� �̽��� Time out�� �ɾ ���͸� �����ϴٰ� �����ð� �� OFF ��Ų��
/// @param      void
/// @return     void
void PTA_HeaterTestPowerTimeOut(void)
{
    // ���� ����
    if (gu16PTA_HeaterActTimer == 0)
    {
        TurnOff_Heater();
    }
    else
    {
        gu16PTA_HeaterActTimer--;
        TurnOn_Heater();
    }
}


/// @brief      PTA �˻�� Heater ON �Լ�
/// @details    PTA �˻�� Heater ���ϵ��� ON ��Ų��
/// @param      void
/// @return     void
void TurnOn_Heater(void)
{
    TURN_ON_HEATER_1();
}


/// @brief      PTA �˻�� Heater OFF �Լ�
/// @details    PTA �˻�� Heater ���ϵ��� OFF ��Ų��
/// @param      void
/// @return     void
void TurnOff_Heater(void)
{
    TURN_OFF_HEATER_1();
}



/// @brief      PTA �˻� ��� ���� �ð� ī����(@1ms interrupt)
/// @details    PTA �˻� ��带 �����ϱ� ���� ���� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void PTA_ControlTimer(void)
{
    if (gu8PTA_ControlTimer < PTA_CONTROL_TIME_PERIOD)
    {
        gu8PTA_ControlTimer++;
    }
}


/// @brief      PTA�˻� ���� �� �ð� ó�� ��ƾ �Լ� ����(@While��)
/// @details    PTA�˻� ���� �߿� UART �� ���� ����� �ð� ������ �ʿ��� �Լ����� �ð��� ���� ��� ó���Ѵ�
/// @param      void
/// @return     void
void Lib_PTA_Mode_TimeControl(void)
{
    if (gu8PTA_ControlTimer >= PTA_CONTROL_TIME_PERIOD)
    {   // 100ms ���� ����
        gu8PTA_ControlTimer = 0;

        if (gu16PTA_Mode_InTimeOut)
        {
            gu16PTA_Mode_InTimeOut--;
        }

        PTA_UART_Rx_TimeOut();              // PTA UART ��� RXD ��� Time Out ����
        PTA_HeaterTestPowerTimeOut();       // PTA �׽�Ʈ �� ���� ���� �ð� ���� ����
    }
}


/// @brief      PTA�˻� ���� ���� �ð� ���� Ȯ�� �Լ�
/// @details    PTA�˻� ������ ������ ����(���� ���� �ð��� ����)���� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : ���� ���� ���� ���� - 0(���� �Ұ�), 1(���� ����)
U8 Get_Able_In_PTA_Mode(void)
{
    if (gu16PTA_Mode_InTimeOut)
    {   // ��� ���� ���� �ð��� ���� ���
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      PTA�˻� ���� �� Front PBA�� ���α׷� ���� ���� �Լ�
/// @details    PTA�˻� ���ۿ��� ����� Front PBA�κ��� Ȯ�ε� SW ������ �����Ѵ�
/// @param      mu8Ver : Front PBA SW ����
/// @return     void
void Set_PTA_FrontSW_Ver(U8 mu8Ver)
{
    gu8FrontPBA_SW_Version = mu8Ver;
}


/// @brief      PTA�˻� ���� �� Front PBA�� ���α׷� ���� Ȯ�� �Լ�
/// @details    PTA�˻� ���ۿ��� ����� Front PBA�� SW ������ Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Front PBA SW ����
U8 Get_PTA_FrontSW_Ver(void)
{
    return  gu8FrontPBA_SW_Version;
}


/// @brief      PTA�˻� ���� �� Main PBA�� ���α׷� ���� ���� �Լ�
/// @details    PTA�˻� ���ۿ��� ����� Main PBA SW ������ �����Ѵ�
/// @param      mu8Ver : Main PBA SW ����
/// @return     void
void Set_PTA_MainSW_Ver(U8 mu8Ver)
{
    gu8MainPBA_SW_Version = mu8Ver;
}


/// @brief      PTA�˻� ���� �� Main PBA�� ���α׷� ���� Ȯ�� �Լ�
/// @details    PTA�˻� ���ۿ��� ����� Main PBA�� SW ������ Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : Main PBA SW ����
U8 Get_PTA_MainSW_Ver(void)
{
    return  gu8MainPBA_SW_Version;
}


/// @brief      PTA�˻� 1��� ���� �Ϸ� ���� �Լ�
/// @details    PTA�˻� 1��� ���� �Ϸ� ���¸� �����Ѵ�
/// @param      mu8Data : PTA �˻� 1��� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
/// @return     void
void Set_PTA_Mode1_End(U8 mu8Data)
{
    if (mu8Data)
    {
        gu8PTA_Mode1_End = SET;
    }
    else
    {
        gu8PTA_Mode1_End = CLEAR;
    }
}


/// @brief      PTA�˻� 1��� ���� �Ϸ� Ȯ�� �Լ�
/// @details    PTA�˻� 1��� ���� �Ϸ� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : PTA �˻� 1��� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_PTA_Mode1_End(void)
{
    if (gu8PTA_Mode1_End)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      PTA�˻� 1��忡�� ���� ���� ���� �Ϸ� ���� �Լ�
/// @details    PTA�˻� 1��� ���۽� ���� ���� üũ �Ϸ� ���¸� �����Ѵ�
/// @param      mu8Data : PTA �˻� 1��� ���� ���� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
/// @return     void
void Set_PTA_Mode1_LeakTest(U8 mu8Data)
{
    if (mu8Data)
    {
        gu8PTA_LeakTestEnd = SET;
    }
    else
    {
        gu8PTA_LeakTestEnd = CLEAR;
    }
}

/// @brief      PTA�˻� 1��忡�� ��ư ���� üũ �Ϸ� ���� �Լ�
/// @details    PTA�˻� 1��忡�� ��ư ���� �Ϸ� ���¸� �����Ѵ�
/// @param      mu8Data : PTA �˻� 1��� ��ư ���� üũ �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
/// @return     void
void Set_PTA_Mode1_KeyTest(U8 mu8Data)
{
    if (mu8Data)
    {
        gu8PTA_KeyTestEnd = SET;
    }
    else
    {
        gu8PTA_KeyTestEnd = CLEAR;
    }
}


/// @brief      PTA�˻� 1��忡�� ������ �׽�Ʈ1 ���� �Ϸ� ���� �Լ�(���Ը�� 0x01 ���Ž� ó�� �Լ�)
/// @details    PTA�˻� 1��忡�� ���� �˻� SKIP ���� �Ϸ� ���θ� üũ�Ѵ�
/// @param      void
/// @return     void
void PTA_Mode1_PreTest1End(void)
{
    if (gu8PTA_LeakTestEnd == SET)
    {
        Set_PTA_Mode1_End(SET);
    }
    else
    {
        Set_PTA_Mode1_End(CLEAR);
    }
}


/// @brief      PTA�˻� 1��忡�� ������ �׽�Ʈ2 ���� �Ϸ� ���� �Լ�(���Ը�� 0x02 ���Ž� ó�� �Լ�)
/// @details    PTA�˻� 1��忡�� ���� �˻� ���� �Ϸ� ���θ� üũ�Ѵ�
/// @param      mu8Data : PTA �˻� 1��� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
/// @return     void
void PTA_Mode1_PreTest2End(void)
{
    if ( (gu8PTA_LeakTestEnd == SET) && (gu8PTA_KeyTestEnd == SET) )
    {
        Set_PTA_Mode1_End(SET);
    }
    else
    {
        Set_PTA_Mode1_End(CLEAR);
    }
}


/// @brief      PTA�˻� ���� �Ϸ� ���� �Լ�
/// @details    PTA�˻� ���� �Ϸ� ���¸� �����Ѵ�
/// @param      mu8Data : PTA �˻� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
/// @return     void
void Set_PTA_Test_End(U8 mu8Data)
{
    if (mu8Data)
    {
        gu8PTA_TestEnd = SET;
    }
    else
    {
        gu8PTA_TestEnd = CLEAR;
    }
}


/// @brief      PTA�˻� ���� �Ϸ� Ȯ�� �Լ�
/// @details    PTA�˻� ���� �Ϸ� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : PTA �˻� �Ϸ� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
U8 Get_PTA_Test_End(void)
{
    if (gu8PTA_TestEnd)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      PTA �˻� ��峻 RXD DATA�� ó�� ���� �Լ�(@While��)
/// @details    PTA �˻� ���۽� Rx�� �����͸� �������� ó���Ѵ�
/// @param      void
/// @return     void
void Lib_PTA_Mode_RXD_Data_Control(void)
{
    U8 mu8RXDCheckSum = 0;
    U8 mu8Able_PTA_In = 0;
    U8 mu8OperatingModeStatus = 0;
    U8 mu8PTA_Mode1_End = 0;
    U8 mu8PTA_Test_End = 0;

    mu8OperatingModeStatus = Get_OperationMode();           // ���� ���� ���� ��� Ȯ��

    if (gu8PTA_UART_Rx_OK)
    {   // UART ���� �����Ͱ� �ִ� ���
        if ((ga8PTA_RXD_Data[0] != AT_RS232_STX) || (ga8PTA_RXD_Data[gu8PTA_UART_Rx_DataCount] != AT_RS232_ETX))
        {   // STX�� ETX Ȯ���Ͽ� ���� �ʴ� ��� NACK ȸ���ϰ� return
            Clear_PTA_TxDataBuffer();
            PTA_NACK_Send();
            Clear_PTA_RxDataBuffer();
            return;
        }

        // STX�� ETX�� �´� ���
        // ������ ��ȯ
        gu16PTA_RXD_Command = (((U16)(ASCII2HEX(ga8PTA_RXD_Data[1])) << 12) & 0xF000) + (((U16)(ASCII2HEX(ga8PTA_RXD_Data[2])) << 8) & 0x0F00) +
                              (((U16)(ASCII2HEX(ga8PTA_RXD_Data[3])) << 4) & 0x00F0) + ((U16)(ASCII2HEX(ga8PTA_RXD_Data[4])) & 0x000F);

        if (gu16PTA_RXD_Command == AT_UART_CMD_MODEL_NAME)
        {   // �ʱ� ��� ���� Command�� ���
            gu8PTA_RXD_Data1 = ((ASCII2HEX(ga8PTA_RXD_Data[5]) << 4) & 0xF0);
            gu8PTA_RXD_Data1 |= (ASCII2HEX(ga8PTA_RXD_Data[6]) & 0x0F);

            gu8PTA_RXD_CS = ((ASCII2HEX(ga8PTA_RXD_Data[7]) << 4) & 0xF0) + (ASCII2HEX(ga8PTA_RXD_Data[8]) & 0x0F);     // ������ CheckSum
            mu8RXDCheckSum = (U8)(ga8PTA_RXD_Data[0] ^ ga8PTA_RXD_Data[1] ^ ga8PTA_RXD_Data[2] ^
                                  ga8PTA_RXD_Data[3] ^ ga8PTA_RXD_Data[4] ^ ga8PTA_RXD_Data[5] ^
                                  ga8PTA_RXD_Data[6]);
        }
        else
        {   // �ʱ� ��� ���� Command�� �ƴ� ���
            gu8PTA_RXD_CS = ((ASCII2HEX(ga8PTA_RXD_Data[5]) << 4) & 0xF0) + (ASCII2HEX(ga8PTA_RXD_Data[6]) & 0x0F);     // ������ CheckSum
            mu8RXDCheckSum = (U8)(ga8PTA_RXD_Data[0] ^ ga8PTA_RXD_Data[1] ^ ga8PTA_RXD_Data[2] ^
                                  ga8PTA_RXD_Data[3] ^ ga8PTA_RXD_Data[4]);
        }

        if (mu8RXDCheckSum != gu8PTA_RXD_CS)
        {   // Check Sum�� ������ �Ͱ� ����� ���� �ٸ� ��� NACK ȸ���ϰ� return
            Clear_PTA_TxDataBuffer();
            PTA_NACK_Send();
            Clear_PTA_RxDataBuffer();
            return;
        }

        // ����Ʈ �˻� ��� Skip���� Ȯ��
        if (gu16PTA_RXD_Command == AT_UART_CMD_MODEL_NAME)
        {
            if (mu8OperatingModeStatus < OPERATION_MODE_ID_PTA_TEST_2)
            {   // PTA ��� 2 �̻� �������� ���� ���
                if (gu8PTA_RXD_Data1 == AT_UART_CMD_MODEL_NAME_PRETEST_SKIP)
                {   // ����Ʈ �˻� ��� Skip�� ���
                    mu8Able_PTA_In = Get_Able_In_PTA_Mode();

                    if (mu8Able_PTA_In == TRUE)
                    {   // �׽�Ʈ ��� ���� ���� �ð��� ���
                        if (mu8OperatingModeStatus != OPERATION_MODE_ID_PTA_TEST_1)
                        {   // PTA 1 Mode �̵��� ���� ����
                            ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_1);     // PTA��� 1 ���� ��ȯ
                            gu16PTA_Mode_InTimeOut = 0;         // PTA ��� ���� ���� �ð� 0���� ����

                            gu8PTA_Mode_Boot = SET;             // TEST��� ���� ���� Flag
                            gu16PTA_Mode_BootAct_Timer = 10;    // TEST��� ���� ���� �ð� 1��

                            EEPROM_Initial();
                            gu8PTA_AT_EEPROM = Get_PTA_CheckEEPROM();

                            // ��� ���� CLEAR
                            NoPowerSet();

                            Set_BuzzerSelect(BUZZER_SETUP);
                        }
                    }

                    if (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_1)
                    {   // �ڵ�ȭ �׽�Ʈ ���1 ������ ���
                        PTA_Mode1_PreTest1End();        // �׽�Ʈ ��� 1���� ó���ؾ��� �׽�Ʈ �Ϸ� ���� üũ

                        mu8PTA_Mode1_End = Get_PTA_Mode1_End();     // �׽�Ʈ ���1 �ϷῩ�� üũ

                        if (mu8PTA_Mode1_End == TRUE)
                        {   // PTA ��� 1�� �Ϸ�� ���
                            ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_2);

                            NoPowerSet();
                        }
                        else
                        {    // PTA ��� 1�� �Ϸ���� ���� ���
                            Clear_PTA_RxDataBuffer();
                            return;
                        }
                    }
                }
                else if (gu8PTA_RXD_Data1 == AT_UART_CMD_MODEL_NAME_PRETEST)
                {   // ����Ʈ �˻� ��� �����ϴ� ���
                    mu8Able_PTA_In = Get_Able_In_PTA_Mode();

                    if (mu8Able_PTA_In == TRUE)
                    {   // �׽�Ʈ ��� ���� ���� �ð��� ���
                        if (mu8OperatingModeStatus != OPERATION_MODE_ID_PTA_TEST_1)
                        {   // PTA 1 Mode �̵��� ���� ����
                            ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_1);     // PTA��� 1 ���� ��ȯ
                            gu16PTA_Mode_InTimeOut = 0;         // PTA ��� ���� ���� �ð� 0���� ����

                            gu8PTA_Mode_Boot = SET;             // TEST��� ���� ���� Flag
                            gu16PTA_Mode_BootAct_Timer = 10;    // TEST��� ���� ���� �ð� 1��

                            EEPROM_Initial();
                            gu8PTA_AT_EEPROM = Get_PTA_CheckEEPROM();

                            // ��� ���� CLEAR
                            NoPowerSet();

                            Set_BuzzerSelect(BUZZER_SETUP);
                        }
                    }

                    if (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_1)
                    {   // �ڵ�ȭ �׽�Ʈ ���1 ������ ���
                        PTA_Mode1_PreTest2End();        // �׽�Ʈ ��� 1���� ó���ؾ��� �׽�Ʈ �Ϸ� ���� üũ

                        mu8PTA_Mode1_End = Get_PTA_Mode1_End();     // �׽�Ʈ ���1 �ϷῩ�� üũ

                        if (mu8PTA_Mode1_End == TRUE)
                        {   // PTA ��� 1�� �Ϸ�� ���
                            ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_2);

                            NoPowerSet();
                        }
                        else
                        {    // PTA ��� 1�� �Ϸ���� ���� ���
                            Clear_PTA_RxDataBuffer();
                            return;
                        }
                    }
                }
                else
                {   // �˻� �����Ͱ� 0x01, 0x02�� �ƴ� ��� ���� ������ ó�� ���ϰ� return
                    Clear_PTA_RxDataBuffer();
                    return;
                }
            }
        }
    }
    else
    {   // UART ���� �����Ͱ� ���� ���
        Clear_PTA_RxDataBuffer();
        return;
    }

    mu8PTA_Test_End = Get_PTA_Test_End();       // PTA �˻� �Ϸ� ���� Ȯ��

    if (mu8PTA_Test_End == TRUE)
    {   // �̹� �ѹ��̶� PTA �׽�Ʈ�� �Ϸ�� ��쿡�� �ƹ��� �����͵� ó�� ����
        Clear_PTA_RxDataBuffer();
        return;
    }

    // Check Sum���� ���� ���� ��ġ�ϴ� ���
    Clear_PTA_TxDataBuffer();           // Tx ������ ���� Clear

    switch (gu16PTA_RXD_Command)
    {
        case AT_UART_CMD_MODEL_NAME:                // �� ���� �۽�(0x2000)
            PTA_ACK_CMD_2000_Send();
            break;

        case AT_UART_CMD_BASIC_INFO:                // 0x3000
            PTA_ACK_CMD_3000_Send();
            break;

        case AT_UART_CMD_WIFI_INFO:                 // 0x3010
            PTA_ACK_CMD_3010_Send();
            break;

        case AT_UART_CMD_EW_UV_INFO:                // 0x3011
            PTA_ACK_CMD_3011_Send();
            break;

        case AT_UART_CMD_FLOW_SENSOR_INFO:          // 0x3012
            PTA_ACK_CMD_3012_Send();
            break;

        case AT_UART_CMD_AC_DC_CURRENT_INFO:        // 0x3013
            PTA_ACK_CMD_3013_Send();
            break;

        case AT_UART_CMD_FAN_INFO:                  // 0x3014
            PTA_ACK_CMD_3014_Send();
            break;

        case AT_UART_CMD_INFRARED_SENSOR_INFO:      // 0x3015
            PTA_ACK_CMD_3015_Send();
            break;

        case AT_UART_CMD_STIRRING_MOTOR_INFO:       // 0x3016
            PTA_ACK_CMD_3016_Send();
            break;

        case AT_UART_CMD_PRESSURE_SENSOR_INFO:      // 0x3017
            PTA_ACK_CMD_3017_Send();
            break;

        case AT_UART_CMD_TDS_IN_SENSOR_INFO:        // 0x3018
            PTA_ACK_CMD_3018_Send();
            break;

        case AT_UART_CMD_TDS_OUT_SENSOR_INFO:       // 0x3019
            PTA_ACK_CMD_3019_Send();
            break;

        case AT_UART_CMD_BOOST_PUMP_INFO:           // 0x301A
            PTA_ACK_CMD_301A_Send();
            break;

        case AT_UART_CMD_CIRCULATE_PUMP_INFO:       // 0x301B
            PTA_ACK_CMD_301B_Send();
            break;

        case AT_UART_CMD_BRIGHTNESS_SENSOR_INFO:    // 0x301C
            PTA_ACK_CMD_301C_Send();
            break;

        case AT_UART_CMD_TEMP_SENSOR_INFO:          // 0x3020
            PTA_ACK_CMD_3020_Send();
            break;

        case AT_UART_CMD_WATER_LEVEL_SENSOR_INFO:   // 0x3030
            PTA_ACK_CMD_3030_Send();
            break;

        case AT_UART_CMD_SENSOR_INFO:               // 0x3040
            PTA_ACK_CMD_3040_Send();
            break;

        case AT_UART_CMD_FEEDBACK_INFO:             // 0x3050
            PTA_ACK_CMD_3050_Send();
            break;

        case AT_UART_CMD_NO_POWER_SET:              // 0x5000
            PTA_ACK_CMD_5000_Send();
            break;

        case AT_UART_CMD_AC_HEATER_ON:              // 0x5010
            PTA_ACK_CMD_5010_Send();
            break;

        case AT_UART_CMD_AC_HEATER_OFF:             // 0x5011
            PTA_ACK_CMD_5011_Send();
            break;

        case AT_UART_CMD_DE_ICE_HEATER_ON:          // 0x5012
            PTA_ACK_CMD_5012_Send();
            break;

        case AT_UART_CMD_DE_ICE_HEATER_OFF:         // 0x5013
            PTA_ACK_CMD_5013_Send();
            break;

        case AT_UART_CMD_ROOM_IN_VALVE_ON:          // 0x5100
            PTA_ACK_CMD_5100_Send();
            break;

        case AT_UART_CMD_ROOM_IN_VALVE_OFF:         // 0x5101
            PTA_ACK_CMD_5101_Send();
            break;

        case AT_UART_CMD_COLD_IN_VALVE_ON:          // 0x5102
            PTA_ACK_CMD_5102_Send();
            break;

        case AT_UART_CMD_COLD_IN_VALVE_OFF:         // 0x5103
            PTA_ACK_CMD_5103_Send();
            break;

        case AT_UART_CMD_HOT_IN_VALVE_ON:           // 0x5104
            PTA_ACK_CMD_5104_Send();
            break;

        case AT_UART_CMD_HOT_IN_VALVE_OFF:          // 0x5105
            PTA_ACK_CMD_5105_Send();
            break;

        case AT_UART_CMD_ROOM_OUT_VALVE_ON:         // 0x5106
            PTA_ACK_CMD_5106_Send();
            break;

        case AT_UART_CMD_ROOM_OUT_VALVE_OFF:        // 0x5107
            PTA_ACK_CMD_5107_Send();
            break;

        case AT_UART_CMD_COLD_OUT_VALVE_ON:         // 0x5108
            PTA_ACK_CMD_5108_Send();
            break;

        case AT_UART_CMD_COLD_OUT_VALVE_OFF:        // 0x5109
            PTA_ACK_CMD_5109_Send();
            break;

        case AT_UART_CMD_HOT_OUT_VALVE_ON:          // 0x510A
            PTA_ACK_CMD_510A_Send();
            break;

        case AT_UART_CMD_HOT_OUT_VALVE_OFF:         // 0x510B
            PTA_ACK_CMD_510B_Send();
            break;

        case AT_UART_CMD_TWO_WAY_VALVE_ON:          // 0x510C
            PTA_ACK_CMD_510C_Send();
            break;

        case AT_UART_CMD_TWO_WAY_VALVE_OFF:         // 0x510D
            PTA_ACK_CMD_510D_Send();
            break;

        case AT_UART_CMD_INPUT_FEED_VALVE_ON:       // 0x510E
            PTA_ACK_CMD_510E_Send();
            break;

        case AT_UART_CMD_INPUT_FEED_VALVE_OFF:      // 0x510F
            PTA_ACK_CMD_510F_Send();
            break;

        case AT_UART_CMD_INPUT_NOS_VALVE_ON:        // 0x5110
            PTA_ACK_CMD_5110_Send();
            break;

        case AT_UART_CMD_INPUT_NOS_VALVE_OFF:       // 0x5111
            PTA_ACK_CMD_5111_Send();
            break;

        case AT_UART_CMD_OUTPUT_DRAIN_NOS_VALVE_ON:     // 0x5112
            PTA_ACK_CMD_5112_Send();
            break;

        case AT_UART_CMD_OUTPUT_DRAIN_NOS_VALVE_OFF:    // 0x5113
            PTA_ACK_CMD_5113_Send();
            break;

        case AT_UART_CMD_HOT_DRAIN_VALVE_ON:        // 0x5114
            PTA_ACK_CMD_5114_Send();
            break;

        case AT_UART_CMD_HOT_DRAIN_VALVE_OFF:       // 0x5115
            PTA_ACK_CMD_5115_Send();
            break;

        case AT_UART_CMD_ICE_TRAY_VALVE_ON:         // 0x5116
            PTA_ACK_CMD_5116_Send();
            break;

        case AT_UART_CMD_ICE_TRAY_VALVE_OFF:        // 0x5117
            PTA_ACK_CMD_5117_Send();
            break;

        case AT_UART_CMD_DRAIN_VALVE_ON:            // 0x5118
            PTA_ACK_CMD_5118_Send();
            break;

        case AT_UART_CMD_DRAIN_VALVE_OFF:           // 0x5119
            PTA_ACK_CMD_5119_Send();
            break;

        case AT_UART_CMD_EW_DRAIN_VALVE_ON:         // 0x511A
            PTA_ACK_CMD_511A_Send();
            break;

        case AT_UART_CMD_EW_DRAIN_VALVE_OFF:        // 0x511B
            PTA_ACK_CMD_511B_Send();
            break;

        case AT_UART_CMD_EW_INPUT_VALVE_ON:         // 0x511C
            PTA_ACK_CMD_511C_Send();
            break;

        case AT_UART_CMD_EW_INPUT_VALVE_OFF:        // 0x511D
            PTA_ACK_CMD_511D_Send();
            break;

        case AT_UART_CMD_SODA_DRAIN_VALVE_ON:       // 0x511E
            PTA_ACK_CMD_511E_Send();
            break;

        case AT_UART_CMD_SODA_DRAIN_VALVE_OFF:      // 0x511F
            PTA_ACK_CMD_511F_Send();
            break;

        case AT_UART_CMD_SODA_OUTPUT_VALVE_ON:      // 0x5120
            PTA_ACK_CMD_5120_Send();
            break;

        case AT_UART_CMD_SODA_OUTPUT_VALVE_OFF:     // 0x5121
            PTA_ACK_CMD_5121_Send();
            break;

        case AT_UART_CMD_REFRIGERANT_CONVERSION_VALVE_ON:   // 0x5122
            PTA_ACK_CMD_5122_Send();
            break;

        case AT_UART_CMD_REFRIGERANT_CONVERSION_VALVE_OFF:  // 0x5123
            PTA_ACK_CMD_5123_Send();
            break;

        case AT_UART_CMD_HOT_GAS_VALVE_ON:          // 0x5124
            PTA_ACK_CMD_5124_Send();
            break;

        case AT_UART_CMD_HOT_GAS_VALVE_OFF:         // 0x5125
            PTA_ACK_CMD_5125_Send();
            break;

        case AT_UART_CMD_ION_EXCHANGE_REGIN_VALVE_ON:   // 0x5126
            PTA_ACK_CMD_5126_Send();
            break;

        case AT_UART_CMD_ION_EXCHANGE_REGIN_VALVE_OFF:  // 0x5127
            PTA_ACK_CMD_5127_Send();
            break;

        case AT_UART_CMD_INO_VALVE_ON:              // 0x5128
            PTA_ACK_CMD_5128_Send();
            break;

        case AT_UART_CMD_INO_VALVE_OFF:             // 0x5129
            PTA_ACK_CMD_5129_Send();
            break;

        case AT_UART_CMD_HOT_TANK_OVERFLOW_VALVE_ON:    // 0x512A
            PTA_ACK_CMD_512A_Send();
            break;

        case AT_UART_CMD_HOT_TANK_OVERFLOW_VALVE_OFF:   // 0x512B
            PTA_ACK_CMD_512B_Send();
            break;

        case AT_UART_CMD_COLD_TANK_OVERFLOW_VALVE_ON:   // 0x512C
            PTA_ACK_CMD_512C_Send();
            break;

        case AT_UART_CMD_COLD_TANK_OVERFLOW_VALVE_OFF:  // 0x512D
            PTA_ACK_CMD_512D_Send();
            break;

        case AT_UART_CMD_TOTAL_OUTPUT_VALVE_ON:     // 0x512E
            PTA_ACK_CMD_512E_Send();
            break;

        case AT_UART_CMD_TOTAL_OUTPUT_VALVE_OFF:    // 0x512F
            PTA_ACK_CMD_512F_Send();
            break;

        case AT_UART_CMD_ICE_COLD_TANK_INPUT_VALVE_ON:  // 0x5130
            PTA_ACK_CMD_5130_Send();
            break;

        case AT_UART_CMD_ICE_COLD_TANK_INPUT_VALVE_OFF: // 0x5131
            PTA_ACK_CMD_5131_Send();
            break;

        case AT_UART_CMD_PRESSURIZED_INPUT_VALVE_ON:    // 0x5132
            PTA_ACK_CMD_5132_Send();
            break;

        case AT_UART_CMD_PRESSURIZED_INPUT_VALVE_OFF:   // 0x5133
            PTA_ACK_CMD_5133_Send();
            break;

        case AT_UART_CMD_PRESSURIZED_DRAIN_VALVE_ON:    // 0x5134
            PTA_ACK_CMD_5134_Send();
            break;

        case AT_UART_CMD_PRESSURIZED_DRAIN_VALVE_OFF:   // 0x5135
            PTA_ACK_CMD_5135_Send();
            break;

        case AT_UART_CMD_FAN_ON:                    // 0x5200
            PTA_ACK_CMD_5200_Send();
            break;

        case AT_UART_CMD_FAN_OFF:                   // 0x5201
            PTA_ACK_CMD_5201_Send();
            break;

        case AT_UART_CMD_STEP_MOTOR_ON:             // 0x5202
            PTA_ACK_CMD_5202_Send();
            break;

        case AT_UART_CMD_STEP_MOTOR_OFF:            // 0x5203
            PTA_ACK_CMD_5203_Send();
            break;

        case AT_UART_CMD_ICE_MAKE_MOTOR_ON:         // 0x5204
            PTA_ACK_CMD_5204_Send();
            break;

        case AT_UART_CMD_ICE_MAKE_MOTOR_OFF:        // 0x5205
            PTA_ACK_CMD_5205_Send();
            break;

        case AT_UART_CMD_ICE_TRAY_MOTOR_ON:         // 0x5206
            PTA_ACK_CMD_5206_Send();
            break;

        case AT_UART_CMD_ICE_TRAY_MOTOR_OFF:        // 0x5207
            PTA_ACK_CMD_5207_Send();
            break;

        case AT_UART_CMD_ICE_DOOR_MOTOR_ON:         // 0x5208
            PTA_ACK_CMD_5208_Send();
            break;

        case AT_UART_CMD_ICE_DOOR_MOTOR_OFF:        // 0x5209
            PTA_ACK_CMD_5209_Send();
            break;

        case AT_UART_CMD_ICE_OUTPUT_CW_MOTOR_ON:    // 0x520A
            PTA_ACK_CMD_520A_Send();
            break;

        case AT_UART_CMD_ICE_OUTPUT_CW_MOTOR_OFF:   // 0x520B
            PTA_ACK_CMD_520B_Send();
            break;

        case AT_UART_CMD_ICE_OUTPUT_CCW_MOTOR_ON:   // 0x520C
            PTA_ACK_CMD_520C_Send();
            break;

        case AT_UART_CMD_ICE_OUTPUT_CCW_MOTOR_OFF:  // 0x520D
            PTA_ACK_CMD_520D_Send();
            break;

        case AT_UART_CMD_SELECT_BAR_MOTOR_ON:       // 0x520E
            PTA_ACK_CMD_520E_Send();
            break;

        case AT_UART_CMD_SELECT_BAR_MOTOR_OFF:      // 0x520F
            PTA_ACK_CMD_520F_Send();
            break;

        case AT_UART_CMD_SWING_BAR_MOTOR_ON:        // 0x5210
            PTA_ACK_CMD_5210_Send();
            break;

        case AT_UART_CMD_SWING_BAR_MOTOR_OFF:       // 0x5211
            PTA_ACK_CMD_5211_Send();
            break;

        case AT_UART_CMD_FAUCET_MOTOR_CW_ON:        // 0x5212
            PTA_ACK_CMD_5212_Send();
            break;

        case AT_UART_CMD_FAUCET_MOTOR_CW_OFF:       // 0x5213
            PTA_ACK_CMD_5213_Send();
            break;

        case AT_UART_CMD_STIRRING_MOTOR_ON:         // 0x5214
            PTA_ACK_CMD_5214_Send();
            break;

        case AT_UART_CMD_STIRRING_MOTOR_OFF:        // 0x5215
            PTA_ACK_CMD_5215_Send();
            break;

        case AT_UART_CMD_FAUCET_MOTOR_CCW_ON:       // 0x5216
            PTA_ACK_CMD_5216_Send();
            break;

        case AT_UART_CMD_FAUCET_MOTOR_CCW_OFF:      // 0x5217
            PTA_ACK_CMD_5217_Send();
            break;

        case AT_UART_CMD_BOOST_PUMP_ON:             // 0x5300
            PTA_ACK_CMD_5300_Send();
            break;

        case AT_UART_CMD_BOOST_PUMP_OFF:            // 0x5301
            PTA_ACK_CMD_5301_Send();
            break;

        case AT_UART_CMD_DRAIN_PUMP_ON:             // 0x5302
            PTA_ACK_CMD_5302_Send();
            break;

        case AT_UART_CMD_DRAIN_PUMP_OFF:            // 0x5303
            PTA_ACK_CMD_5303_Send();
            break;

        case AT_UART_CMD_COLD_CIRCULATE_PUMP_ON:    // 0x5304
            PTA_ACK_CMD_5304_Send();
            break;

        case AT_UART_CMD_COLD_CIRCULATE_PUMP_OFF:   // 0x5305
            PTA_ACK_CMD_5305_Send();
            break;

        case AT_UART_CMD_HOT_OUTPUT_PUMP_ON:        // 0x5306
            PTA_ACK_CMD_5306_Send();
            break;

        case AT_UART_CMD_HOT_OUTPUT_PUMP_OFF:       // 0x5307
            PTA_ACK_CMD_5307_Send();
            break;

        case AT_UART_CMD_UV_ON:                     // 0x7000
            PTA_ACK_CMD_7000_Send();
            break;

        case AT_UART_CMD_UV_OFF:                    // 0x7001
            PTA_ACK_CMD_7001_Send();
            break;

        case AT_UART_CMD_LED2_ON:                   // 0x7002
            PTA_ACK_CMD_7002_Send();
            break;

        case AT_UART_CMD_LED2_OFF:                  // 0x7003
            PTA_ACK_CMD_7003_Send();
            break;

        case AT_UART_CMD_LED3_ON:                   // 0x7004
            PTA_ACK_CMD_7004_Send();
            break;

        case AT_UART_CMD_LED3_OFF:                  // 0x7005
            PTA_ACK_CMD_7005_Send();
            break;

        case AT_UART_CMD_LED4_ON:                   // 0x7006
            PTA_ACK_CMD_7006_Send();
            break;

        case AT_UART_CMD_LED4_OFF:                  // 0x7007
            PTA_ACK_CMD_7007_Send();
            break;

        case AT_UART_CMD_LED5_ON:                   // 0x7008
            PTA_ACK_CMD_7008_Send();
            break;

        case AT_UART_CMD_LED5_OFF:                  // 0x7009
            PTA_ACK_CMD_7009_Send();
            break;

        case AT_UART_CMD_COMP_ON:                   // 0x9900
            PTA_ACK_CMD_9900_Send();
            break;

        case AT_UART_CMD_COMP_OFF:                  // 0x9901
            PTA_ACK_CMD_9901_Send();
            break;

        case AT_UART_CMD_ICE_MAKE_SYSTEM_COOL:      // 0x9902
            PTA_ACK_CMD_9902_Send();
            break;

        case AT_UART_CMD_ICE_MAKE_SYSTEM_ICE_MAKE:  // 0x9903
            PTA_ACK_CMD_9903_Send();
            break;

        case AT_UART_CMD_COMP_ICE_MAKE:             // 0x9904
            PTA_ACK_CMD_9904_Send();
            break;

        case AT_UART_CMD_TEST_COMPLETE:             // 0xFF00
            PTA_ACK_CMD_FF00_Send();
            break;

        default:    // �ν� ���ϴ� Command�� ���� ��쿡�� NACK ȸ��
            PTA_NACK_Send();
            break;
    }
    Clear_PTA_RxDataBuffer();       // RXD DATA ó���� Buffer Clear
}



/// @brief      PTA �˻� RX ���� ������ �ʱ�ȭ �Լ�
/// @details    PTA �˻� RX ���� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Clear_PTA_RxDataBuffer(void)
{
    MEMSET(ga8PTA_RXD_Data, '\0', sizeof(ga8PTA_RXD_Data));
    gu16PTA_RXD_Command = 0;
    gu8PTA_RXD_CS = 0;
    gu8PTA_UART_Rx_DataCount = 0;
    gu8PTA_UART_RXD_CheckTimeOut = 0;
    gu8PTA_UART_Rx_OK = CLEAR;
}


/// @brief      PTA �˻� TX ���� ������ �ʱ�ȭ �Լ�
/// @details    PTA �˻� TX ���� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Clear_PTA_TxDataBuffer(void)
{
    MEMSET(ga8PTA_TXD_Data, '\0', sizeof(ga8PTA_TXD_Data));
    gu8PTA_UART_TXD_Length = 0;
    gu8PTA_TXD_CS = 0;
    gu8PTA_UART_Tx_DataCount = 0;
}


/// @brief      PTA �˻� NACK �۽� �Լ�
/// @details    PTA �˻� �� NACK �� �۽� �Ѵ�
/// @param      void
/// @return     void
void PTA_NACK_Send(void)
{
    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_NACK[0];   // NAK 0x15
    ga8PTA_TXD_Data[2] = AT_UART_NACK[1];   // NAK 0x15
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    gu8PTA_TXD_CS = (U8)(AT_RS232_STX ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                    ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                    ga8PTA_TXD_Data[6]);

    ga8PTA_TXD_Data[7] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[8] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[9] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 9;
    PTA_UART_TXD_Start();       // Tx ����
}


/// @brief      PTA �˻� ���� �˻�� ���� ACK �Լ�
/// @details    PTA �˻� ���� �˻�� ���������� ȸ���ϴ� ACK�� �����Ͽ� ����Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_PowerTest(void)
{
    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6]);

    ga8PTA_TXD_Data[7] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[8] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[9] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 9;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x2000 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x2000 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_2000_Send(void)
{
    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];            // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];            // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];        // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];        // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];        // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];        // CMD
    ga8PTA_TXD_Data[7] = AT_UART_MODEL_INFO[0];     // ���ڵ�
    ga8PTA_TXD_Data[8] = AT_UART_MODEL_INFO[1];     // ���ڵ�
    ga8PTA_TXD_Data[9] = HEX2ASCII((gu8PTA_RXD_Data1 >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII(gu8PTA_RXD_Data1 & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11]  = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3000 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3000 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3000_Send(void)
{
    U8 mu8SWVersionBuffer = 0;
    U8 mu8HeightMode = 0;
    U8 mu8ModelType = 0;

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    ga8PTA_TXD_Data[7] = HEX2ASCII((gu8PTA_AT_EEPROM >> 4) & 0x0F);  // EEPROM 0xA5
    ga8PTA_TXD_Data[8] = HEX2ASCII(gu8PTA_AT_EEPROM & 0x0F);         // EEPROM 0xA5

    mu8SWVersionBuffer = (U8)(gu8FrontPBA_SW_Version & 0x0F);
    ga8PTA_TXD_Data[9]  = HEX2ASCII(mu8SWVersionBuffer / 10);    // Front SW VERSION
    ga8PTA_TXD_Data[10] = HEX2ASCII(mu8SWVersionBuffer % 10);    // Front SW VERSION

    mu8SWVersionBuffer = (U8)(gu8MainPBA_SW_Version & 0x0F);
    ga8PTA_TXD_Data[11] = HEX2ASCII(mu8SWVersionBuffer / 10);    // SW VERSION
    ga8PTA_TXD_Data[12] = HEX2ASCII(mu8SWVersionBuffer % 10);    // SW VERSION

    mu8HeightMode = Get_HightModeStatus();
    ga8PTA_TXD_Data[13] = '0';   // �� ���
    ga8PTA_TXD_Data[14] = HEX2ASCII(mu8HeightMode);    // �� ���

    ga8PTA_TXD_Data[15] = AT_UART_MODEL_CONTRY[0];   // ���� ��
    ga8PTA_TXD_Data[16] = AT_UART_MODEL_CONTRY[1];   // ���� ��

//    mu8ModelType = Get_ModelType();     // �� ���� Ȯ��
    if (mu8ModelType == MODEL_CP)
    {
        ga8PTA_TXD_Data[17] = AT_UART_MODEL_TYPE_CP[0];   // �ü� ����
        ga8PTA_TXD_Data[18] = AT_UART_MODEL_TYPE_CP[1];   // �ü� ����
    }
    else
    {
        ga8PTA_TXD_Data[17] = AT_UART_MODEL_TYPE_CHP[0];   // �ÿ¼� ����
        ga8PTA_TXD_Data[18] = AT_UART_MODEL_TYPE_CHP[1];   // �ÿ¼� ����
    }

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10] ^ ga8PTA_TXD_Data[11] ^
                         ga8PTA_TXD_Data[12] ^ ga8PTA_TXD_Data[13] ^ ga8PTA_TXD_Data[14] ^
                         ga8PTA_TXD_Data[15] ^ ga8PTA_TXD_Data[16] ^ ga8PTA_TXD_Data[17] ^
                         ga8PTA_TXD_Data[18]);

    ga8PTA_TXD_Data[19] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[20] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[21] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 21;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3010 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3010 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3010_Send(void)
{
    U8 mu8WiFiTestStatus = 0;

//    mu8WiFiTestStatus = GetWifiStatusValue(WIFI_STATUS_TEST);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    if (mu8WiFiTestStatus == SET)
    {
        ga8PTA_TXD_Data[7] = AT_UART_WIFI_CONNECT[0];   // WIFI ���� ����
        ga8PTA_TXD_Data[8] = AT_UART_WIFI_CONNECT[1];   // WIFI ���� ����
    }
    else
    {
        ga8PTA_TXD_Data[7] = AT_UART_WIFI_NO_CONNECT[0];   // WIFI ���� ����
        ga8PTA_TXD_Data[8] = AT_UART_WIFI_NO_CONNECT[1];   // WIFI ���� ����
    }

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8]);

    ga8PTA_TXD_Data[9] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[11] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 11;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3011 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3011 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3011_Send(void)
{
    U16 mu16AD_EW_FB_I = 0;
    U16 mu16AD_EW_FB_V = 0;
    U16 mu16AD_UV_FB_I = 0;

//    mu16AD_EW_FB_I = Get_ADC_Data(ADC_ID_EW_I_FB);
//    mu16AD_EW_FB_V = Get_ADC_Data(ADC_ID_EW_V_FB);
//    mu16AD_UV_FB_I = Get_ADC_Data(ADC_ID_UV_FB);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // EW Current
    ga8PTA_TXD_Data[7]  = HEX2ASCII((U8)(mu16AD_EW_FB_I >> 12) & 0x0F);
    ga8PTA_TXD_Data[8]  = HEX2ASCII((U8)(mu16AD_EW_FB_I >> 8) & 0x0F);
    ga8PTA_TXD_Data[9]  = HEX2ASCII((U8)(mu16AD_EW_FB_I >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16AD_EW_FB_I) & 0x0F);

    // EW Voltage
    ga8PTA_TXD_Data[11] = HEX2ASCII((U8)(mu16AD_EW_FB_V >> 12) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII((U8)(mu16AD_EW_FB_V >> 8) & 0x0F);
    ga8PTA_TXD_Data[13] = HEX2ASCII((U8)(mu16AD_EW_FB_V >> 4) & 0x0F);
    ga8PTA_TXD_Data[14] = HEX2ASCII((U8)(mu16AD_EW_FB_V) & 0x0F);

    // UV Voltage
    ga8PTA_TXD_Data[15] = HEX2ASCII((U8)(mu16AD_UV_FB_I >> 12) & 0x0F);
    ga8PTA_TXD_Data[16] = HEX2ASCII((U8)(mu16AD_UV_FB_I >> 8) & 0x0F);
    ga8PTA_TXD_Data[17] = HEX2ASCII((U8)(mu16AD_UV_FB_I >> 4) & 0x0F);
    ga8PTA_TXD_Data[18] = HEX2ASCII((U8)(mu16AD_UV_FB_I) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10] ^ ga8PTA_TXD_Data[11] ^
                         ga8PTA_TXD_Data[12] ^ ga8PTA_TXD_Data[13] ^ ga8PTA_TXD_Data[14] ^
                         ga8PTA_TXD_Data[15] ^ ga8PTA_TXD_Data[16] ^ ga8PTA_TXD_Data[17] ^
                         ga8PTA_TXD_Data[18]);

    ga8PTA_TXD_Data[19] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[20] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[21] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 21;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3012 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3012 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3012_Send(void)
{
    U8 mu16FlowTestValue = 0;

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // Flow Sensor1
    mu16FlowTestValue = Get_FlowmeterTestHz(FLOWMETER_1_ID);
    ga8PTA_TXD_Data[7] = HEX2ASCII((mu16FlowTestValue >> 12) & 0x0F);
    ga8PTA_TXD_Data[8] = HEX2ASCII((mu16FlowTestValue >> 8) & 0x0F);
    ga8PTA_TXD_Data[9] = HEX2ASCII((mu16FlowTestValue >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((mu16FlowTestValue) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11]  = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3013 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3013 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3013_Send(void)
{
    U16 mu16AD_DC_Currnet = 0;
    U32 mu32DC_CurrentBuf = 0;

    mu16AD_DC_Currnet = Get_ADC_Data(ADC_ID_V_FB);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // DC Current - 6�ڸ�
    mu32DC_CurrentBuf = (U32)((D64)(mu16AD_DC_Currnet) * 0.67);
    if (mu32DC_CurrentBuf > 999999)
    {
        mu32DC_CurrentBuf = 999999;
    }
    ga8PTA_TXD_Data[7] = HEX2ASCII((U8)(mu32DC_CurrentBuf / 100000) & 0x0F);
    ga8PTA_TXD_Data[8] = HEX2ASCII((U8)((mu32DC_CurrentBuf % 100000) / 10000) & 0x0F);
    ga8PTA_TXD_Data[9] = HEX2ASCII((U8)((mu32DC_CurrentBuf % 10000) / 1000) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)((mu32DC_CurrentBuf % 1000) / 100) & 0x0F);
    ga8PTA_TXD_Data[11] = HEX2ASCII((U8)((mu32DC_CurrentBuf % 100) / 10) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII((U8)(mu32DC_CurrentBuf % 10) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10] ^ ga8PTA_TXD_Data[11] ^
                         ga8PTA_TXD_Data[12]);

    ga8PTA_TXD_Data[13] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[14] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[15] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 15;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3014 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3014 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3014_Send(void)
{
    U16 mu16AD_Fan_FB = 0;

//    mu16AD_Fan_FB = Get_ADC_Data(ADC_ID_FAN_FEEDBACK);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // Fan
    ga8PTA_TXD_Data[7] = HEX2ASCII((mu16AD_Fan_FB >> 12) & 0x0F);
    ga8PTA_TXD_Data[8] = HEX2ASCII((mu16AD_Fan_FB >> 8) & 0x0F);
    ga8PTA_TXD_Data[9] = HEX2ASCII((mu16AD_Fan_FB >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((mu16AD_Fan_FB) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3015 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3015 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3015_Send(void)
{
    U16 mu16AD_InfraredLight = 0;

//    mu16AD_InfraredLight = Get_ADC_Data(ADC_ID_INFRARED_SENSOR);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // Infrared Light
    ga8PTA_TXD_Data[7]  = HEX2ASCII((U8)(mu16AD_InfraredLight >> 12) & 0x0F);
    ga8PTA_TXD_Data[8]  = HEX2ASCII((U8)(mu16AD_InfraredLight >> 8) & 0x0F);
    ga8PTA_TXD_Data[9]  = HEX2ASCII((U8)(mu16AD_InfraredLight >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16AD_InfraredLight) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3016 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3016 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3016_Send(void)
{
    U16 mu16AD_StirringMotor_FB = 0;

//    mu16AD_StirringMotor_FB = Get_ADC_Data(ADC_ID_STIRRING_MOTOR);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // Stirring Motor Feedback
    ga8PTA_TXD_Data[7]  = HEX2ASCII((U8)(mu16AD_StirringMotor_FB >> 12) & 0x0F);
    ga8PTA_TXD_Data[8]  = HEX2ASCII((U8)(mu16AD_StirringMotor_FB >> 8) & 0x0F);
    ga8PTA_TXD_Data[9]  = HEX2ASCII((U8)(mu16AD_StirringMotor_FB >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16AD_StirringMotor_FB) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3017 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3017 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3017_Send(void)
{
    U16 mu16AD_PressureSensor_FB = 0;

//    mu16AD_PressureSensor_FB = Get_ADC_Data(ADC_ID_PRESSURE_SENSOR);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // Prssure Sensor
    ga8PTA_TXD_Data[7]  = HEX2ASCII((U8)(mu16AD_PressureSensor_FB >> 12) & 0x0F);
    ga8PTA_TXD_Data[8]  = HEX2ASCII((U8)(mu16AD_PressureSensor_FB >> 8) & 0x0F);
    ga8PTA_TXD_Data[9]  = HEX2ASCII((U8)(mu16AD_PressureSensor_FB >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16AD_PressureSensor_FB) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3018 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3018 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3018_Send(void)
{
    U16 mu16TDS_In_Value = 0;

//    mu16TDS_In_Value = Get_TDS_In_Value();

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    ga8PTA_TXD_Data[7]  = HEX2ASCII((U8)(mu16TDS_In_Value >> 12) & 0x0F);
    ga8PTA_TXD_Data[8]  = HEX2ASCII((U8)(mu16TDS_In_Value >> 8) & 0x0F);
    ga8PTA_TXD_Data[9]  = HEX2ASCII((U8)(mu16TDS_In_Value >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16TDS_In_Value) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3019 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3019 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3019_Send(void)
{
    U16 mu16TDS_Out_Value = 0;

//    mu16TDS_Out_Value = Get_TDS_Out_Value();

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    ga8PTA_TXD_Data[7]  = HEX2ASCII((U8)(mu16TDS_Out_Value >> 12) & 0x0F);
    ga8PTA_TXD_Data[8]  = HEX2ASCII((U8)(mu16TDS_Out_Value >> 8) & 0x0F);
    ga8PTA_TXD_Data[9]  = HEX2ASCII((U8)(mu16TDS_Out_Value >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16TDS_Out_Value) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x301A Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x301A Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_301A_Send(void)
{
    U16 mu16AD_BoostPump_Currnet = 0;
    U32 mu32BoostPump_CurrentBuf = 0;

//    mu16AD_BoostPump_Currnet = Get_ADC_Data(ADC_ID_BOOST_PUMP);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // Current - 6�ڸ�
    mu32BoostPump_CurrentBuf = (U32)((D64)(mu16AD_BoostPump_Currnet) * 0.8);
    if (mu32BoostPump_CurrentBuf > 999999)
    {
        mu32BoostPump_CurrentBuf = 999999;
    }
    ga8PTA_TXD_Data[7] = HEX2ASCII((U8)(mu32BoostPump_CurrentBuf / 100000) & 0x0F);
    ga8PTA_TXD_Data[8] = HEX2ASCII((U8)((mu32BoostPump_CurrentBuf % 100000) / 10000) & 0x0F);
    ga8PTA_TXD_Data[9] = HEX2ASCII((U8)((mu32BoostPump_CurrentBuf % 10000) / 1000) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)((mu32BoostPump_CurrentBuf % 1000) / 100) & 0x0F);
    ga8PTA_TXD_Data[11] = HEX2ASCII((U8)((mu32BoostPump_CurrentBuf % 100) / 10) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII((U8)(mu32BoostPump_CurrentBuf % 10) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10] ^ ga8PTA_TXD_Data[11] ^
                         ga8PTA_TXD_Data[12]);

    ga8PTA_TXD_Data[13] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[14] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[15] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 15;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x301B Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x301B Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_301B_Send(void)
{
    U16 mu16AD_ColdPump_Currnet = 0;
    U32 mu32ColdPump_CurrentBuf = 0;

//    mu16AD_ColdPump_Currnet = Get_ADC_Data(ADC_ID_CIRCULATION_PUMP_I_FB);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // Current - 6�ڸ�
    mu32ColdPump_CurrentBuf = (U32)((D64)(mu16AD_ColdPump_Currnet) * 0.46);
    if (mu32ColdPump_CurrentBuf > 999999)
    {
        mu32ColdPump_CurrentBuf = 999999;
    }
    ga8PTA_TXD_Data[7] = HEX2ASCII((U8)(mu32ColdPump_CurrentBuf / 100000) & 0x0F);
    ga8PTA_TXD_Data[8] = HEX2ASCII((U8)((mu32ColdPump_CurrentBuf % 100000) / 10000) & 0x0F);
    ga8PTA_TXD_Data[9] = HEX2ASCII((U8)((mu32ColdPump_CurrentBuf % 10000) / 1000) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)((mu32ColdPump_CurrentBuf % 1000) / 100) & 0x0F);
    ga8PTA_TXD_Data[11] = HEX2ASCII((U8)((mu32ColdPump_CurrentBuf % 100) / 10) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII((U8)(mu32ColdPump_CurrentBuf % 10) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10] ^ ga8PTA_TXD_Data[11] ^
                         ga8PTA_TXD_Data[12]);

    ga8PTA_TXD_Data[13] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[14] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[15] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 15;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x301C Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x301C Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_301C_Send(void)
{
    U16 mu16AD_Brightness = 0;

//    mu16AD_Brightness = Get_ADC_Data(ADC_ID_PHOTO_SENSOR);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // ���� ���� AD��
    ga8PTA_TXD_Data[7] = HEX2ASCII((U8)(mu16AD_Brightness >> 12) & 0x0F);
    ga8PTA_TXD_Data[8] = HEX2ASCII((U8)(mu16AD_Brightness >> 8) & 0x0F);
    ga8PTA_TXD_Data[9] = HEX2ASCII((U8)(mu16AD_Brightness >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16AD_Brightness) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

    ga8PTA_TXD_Data[11] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[13] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 13;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3020 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3020 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3020_Send(void)
{
    U16 mu16AD_ColdTemp = 0;
    U16 mu16AD_HotTemp = 0;
    U16 mu16AD_AirTemp = 0;

    F32 mf32ColdTemp = 0;
    F32 mf32HotTemp = 0;
    F32 mf32AirTemp = 0;

    U16 mu16ColdTemp = 0;
    U16 mu16HotTemp = 0;
    U16 mu16AirTemp = 0;

    mu16AD_ColdTemp = Get_ADC_Data(ADC_ID_TH_COOL);
//    mu16AD_HotTemp = Get_ADC_Data(ADC_ID_TH_HOT);
    mu16AD_AirTemp = Get_ADC_Data(ADC_ID_TH_AIR);

    mf32ColdTemp = Get_Temp(ADC_ID_TH_COOL);
    mf32HotTemp = Get_Temp(ADC_ID_TH_HOT_BODY);
    mf32AirTemp = Get_Temp(ADC_ID_TH_AIR);

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // �ü� �µ�
    if ( (mu16AD_ColdTemp < COLD_TEMP_1_ERROR_UNDER_STANDARD_AD) ||
         (mu16AD_ColdTemp > COLD_TEMP_1_ERROR_OVER_STANDARD_AD) )
    {   // ���� ���� �̸�
        ga8PTA_TXD_Data[7] = AT_UART_TEMP_SENSOR_ERROR[0];      // �ü� �µ� ����
        ga8PTA_TXD_Data[8] = AT_UART_TEMP_SENSOR_ERROR[1];      // �ü� �µ� ����
        ga8PTA_TXD_Data[9] = AT_UART_TEMP_SENSOR_ERROR[2];      // �ü� �µ� ����
        ga8PTA_TXD_Data[10] = AT_UART_TEMP_SENSOR_ERROR[3];     // �ü� �µ� ����
    }
    else
    {   // ���� ������ �ƴϸ�
        if (mf32ColdTemp < 0.0f)
        {   // 0������ ������ 0���� ����
            mf32ColdTemp = 0.0f;
        }
        else if (mf32ColdTemp > 99.0f)
        {   // 99������ ������ 99���� ����
            mf32ColdTemp = 99.0f;
        }
        mu16ColdTemp = (U16)(mf32ColdTemp * 100.0f);
        ga8PTA_TXD_Data[7] = HEX2ASCII((U8)(mu16ColdTemp >> 12) & 0x0F);   // �ü� �µ� ����
        ga8PTA_TXD_Data[8] = HEX2ASCII((U8)(mu16ColdTemp >> 8) & 0x0F);    // �ü� �µ� ����
        ga8PTA_TXD_Data[9] = HEX2ASCII((U8)(mu16ColdTemp >> 4) & 0x0F);    // �ü� �µ� ����
        ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16ColdTemp) & 0x0F);        // �ü� �µ� ����
    }

    // �ü� �µ� ���� AD��
    ga8PTA_TXD_Data[11] = HEX2ASCII((U8)(mu16AD_ColdTemp >> 12) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII((U8)(mu16AD_ColdTemp >> 8) & 0x0F);
    ga8PTA_TXD_Data[13] = HEX2ASCII((U8)(mu16AD_ColdTemp >> 4) & 0x0F);
    ga8PTA_TXD_Data[14] = HEX2ASCII((U8)(mu16AD_ColdTemp) & 0x0F);

    // �¼� �µ�
    if ( (mu16AD_HotTemp < TANK_HOT_TEMP_ERROR_UNDER_STANDARD_AD) ||
         (mu16AD_HotTemp > TANK_HOT_TEMP_ERROR_OVER_STANDARD_AD) )
    {   // ���� ���� �̸�
        ga8PTA_TXD_Data[15] = AT_UART_TEMP_SENSOR_ERROR[0];     // �¼� �µ� ����
        ga8PTA_TXD_Data[16] = AT_UART_TEMP_SENSOR_ERROR[1];     // �¼� �µ� ����
        ga8PTA_TXD_Data[17] = AT_UART_TEMP_SENSOR_ERROR[2];     // �¼� �µ� ����
        ga8PTA_TXD_Data[18] = AT_UART_TEMP_SENSOR_ERROR[3];     // �¼� �µ� ����
    }
    else
    {   // ���� ������ �ƴϸ�
        if (mf32HotTemp < 0.0f)
        {   // 0������ ������ 0���� ����
            mf32HotTemp = 0.0f;
        }
        else if (mf32HotTemp > 99.0f)
        {   // 99������ ������ 99���� ����
            mf32HotTemp = 99.0f;
        }
        mu16HotTemp = (U16)(mf32HotTemp * 100.0f);
        ga8PTA_TXD_Data[15] = HEX2ASCII((U8)(mu16HotTemp >> 12) & 0x0F);    // �¼� �µ� ����
        ga8PTA_TXD_Data[16] = HEX2ASCII((U8)(mu16HotTemp >> 8) & 0x0F);     // �¼� �µ� ����
        ga8PTA_TXD_Data[17] = HEX2ASCII((U8)(mu16HotTemp >> 4) & 0x0F);     // �¼� �µ� ����
        ga8PTA_TXD_Data[18] = HEX2ASCII((U8)(mu16HotTemp) & 0x0F);          // �¼� �µ� ����
    }

    // �¼� �µ� ���� AD��
    ga8PTA_TXD_Data[19] = HEX2ASCII((U8)(mu16AD_HotTemp >> 12) & 0x0F);
    ga8PTA_TXD_Data[20] = HEX2ASCII((U8)(mu16AD_HotTemp >> 8) & 0x0F);
    ga8PTA_TXD_Data[21] = HEX2ASCII((U8)(mu16AD_HotTemp >> 4) & 0x0F);
    ga8PTA_TXD_Data[22] = HEX2ASCII((U8)(mu16AD_HotTemp) & 0x0F);

    // �ܱ� �µ�
    if ( (mu16AD_AirTemp < AIR_TEMP_1_ERROR_UNDER_STANDARD_AD) ||
         (mu16AD_AirTemp > AIR_TEMP_1_ERROR_OVER_STANDARD_AD) )
    {   // ���� ���� �̸�
        ga8PTA_TXD_Data[23] = AT_UART_TEMP_SENSOR_ERROR[0];     // �ܱ� �µ� ����
        ga8PTA_TXD_Data[24] = AT_UART_TEMP_SENSOR_ERROR[1];     // �ܱ� �µ� ����
        ga8PTA_TXD_Data[25] = AT_UART_TEMP_SENSOR_ERROR[2];     // �ܱ� �µ� ����
        ga8PTA_TXD_Data[26] = AT_UART_TEMP_SENSOR_ERROR[3];     // �ܱ� �µ� ����
    }
    else
    {   // ���� ������ �ƴϸ�
        if (mf32AirTemp < 0.0f)
        {   // 0������ ������ 0���� ����
            mf32AirTemp = 0.0f;
        }
        else if (mf32AirTemp > 99.0f)
        {   // 99������ ������ 99���� ����
            mf32AirTemp = 99.0f;
        }
        mu16AirTemp = (U16)(mf32AirTemp * 100.0f);
        ga8PTA_TXD_Data[23] = HEX2ASCII((U8)(mu16AirTemp >> 12) & 0x0F);    // �ܱ� �µ� ����
        ga8PTA_TXD_Data[24] = HEX2ASCII((U8)(mu16AirTemp >> 8) & 0x0F);     // �ܱ� �µ� ����
        ga8PTA_TXD_Data[25] = HEX2ASCII((U8)(mu16AirTemp >> 4) & 0x0F);     // �ܱ� �µ� ����
        ga8PTA_TXD_Data[26] = HEX2ASCII((U8)(mu16AirTemp) & 0x0F);          // �ܱ� �µ� ����
    }

    // �ܱ� �µ� ���� AD��
    ga8PTA_TXD_Data[27] = HEX2ASCII((U8)(mu16AD_AirTemp >> 12) & 0x0F);
    ga8PTA_TXD_Data[28] = HEX2ASCII((U8)(mu16AD_AirTemp >> 8) & 0x0F);
    ga8PTA_TXD_Data[29] = HEX2ASCII((U8)(mu16AD_AirTemp >> 4) & 0x0F);
    ga8PTA_TXD_Data[30] = HEX2ASCII((U8)(mu16AD_AirTemp) & 0x0F);

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10] ^ ga8PTA_TXD_Data[11] ^
                         ga8PTA_TXD_Data[12] ^ ga8PTA_TXD_Data[13] ^ ga8PTA_TXD_Data[14] ^
                         ga8PTA_TXD_Data[15] ^ ga8PTA_TXD_Data[16] ^ ga8PTA_TXD_Data[17] ^
                         ga8PTA_TXD_Data[18] ^ ga8PTA_TXD_Data[19] ^ ga8PTA_TXD_Data[20] ^
                         ga8PTA_TXD_Data[21] ^ ga8PTA_TXD_Data[22] ^ ga8PTA_TXD_Data[23] ^
                         ga8PTA_TXD_Data[24] ^ ga8PTA_TXD_Data[25] ^ ga8PTA_TXD_Data[26] ^
                         ga8PTA_TXD_Data[27] ^ ga8PTA_TXD_Data[28] ^ ga8PTA_TXD_Data[29] ^
                         ga8PTA_TXD_Data[30]);

    ga8PTA_TXD_Data[31] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[32] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[33] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 33;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3020 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3020 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3030_Send(void)
{
    U8 mu8LowLevel = 0;
    U8 mu8HighLevel = 0;
    U8 mu8OverFlow = 0;

    // �ش��ϴ� ���� ������ Port�� ���� Ȯ��
    mu8LowLevel = GET_STATUS_LEVEL_SENSOR_1();
    mu8HighLevel = GET_STATUS_LEVEL_SENSOR_2();
    mu8OverFlow = GET_STATUS_LEVEL_SENSOR_3();

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // ���� ������
    if (mu8LowLevel)
    {   // ��ü��
        ga8PTA_TXD_Data[7] = AT_UART_LEVEL_SENSOR_NO_CONNECT[0];
        ga8PTA_TXD_Data[8] = AT_UART_LEVEL_SENSOR_NO_CONNECT[1];
    }
    else
    {   // ü��
        ga8PTA_TXD_Data[7] = AT_UART_LEVEL_SENSOR_CONNECT[0];
        ga8PTA_TXD_Data[8] = AT_UART_LEVEL_SENSOR_CONNECT[1];
    }

    // ���� ������
    if (mu8HighLevel)
    {   // ��ü��
        ga8PTA_TXD_Data[9] = AT_UART_LEVEL_SENSOR_NO_CONNECT[0];
        ga8PTA_TXD_Data[10] = AT_UART_LEVEL_SENSOR_NO_CONNECT[1];
    }
    else
    {   // ü��
        ga8PTA_TXD_Data[9] = AT_UART_LEVEL_SENSOR_CONNECT[0];
        ga8PTA_TXD_Data[10] = AT_UART_LEVEL_SENSOR_CONNECT[1];
    }

    // OverFlow
    if (mu8OverFlow)
    {   // ��ü��
        ga8PTA_TXD_Data[11] = AT_UART_LEVEL_SENSOR_NO_CONNECT[0];
        ga8PTA_TXD_Data[12] = AT_UART_LEVEL_SENSOR_NO_CONNECT[1];
    }
    else
    {   // ü��
        ga8PTA_TXD_Data[11] = AT_UART_LEVEL_SENSOR_CONNECT[0];
        ga8PTA_TXD_Data[12] = AT_UART_LEVEL_SENSOR_CONNECT[1];
    }

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                         ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10] ^ ga8PTA_TXD_Data[11] ^
                         ga8PTA_TXD_Data[12]);

    ga8PTA_TXD_Data[13] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[14] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[15] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 15;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3040 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3040 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3040_Send(void)
{
    U8 mu8FilterCover = 0;

    // �ش��ϴ� ������ Port�� ���� Ȯ��
//    mu8FilterCover = GET_STATUS_FILTER_COVER_1();

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // ���� Ŀ��
    if (mu8FilterCover)
    {   // ��ü��
        ga8PTA_TXD_Data[7] = AT_UART_LEVEL_SENSOR_NO_CONNECT[0];
        ga8PTA_TXD_Data[8] = AT_UART_LEVEL_SENSOR_NO_CONNECT[1];
    }
    else
    {   // ü��
        ga8PTA_TXD_Data[7] = AT_UART_LEVEL_SENSOR_CONNECT[0];
        ga8PTA_TXD_Data[8] = AT_UART_LEVEL_SENSOR_CONNECT[1];
    }

    gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                         ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                         ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8]);

    ga8PTA_TXD_Data[9] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
    ga8PTA_TXD_Data[10] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
    ga8PTA_TXD_Data[11] = AT_RS232_ETX;

    gu8PTA_UART_TXD_Length = 11;
    PTA_UART_TXD_Start();
}


/// @brief      PTA �˻� 0x3050 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x3050 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_3050_Send(void)
{
    U16 mu16AD_Heater = 0;

    if (gu8PTA_HeaterTestEnd == CLEAR)
    {   // ���� �׽�Ʈ ������ �ǽõ��� ���� ���
        gu8PTA_HeaterTestEnd = SET;

        NoPowerSet();

        TurnOn_Heater();                    // ���� ON

        gu16PTA_HeaterActTimer = 2;         // ���� �ִ� ���� �ð� 200ms
    }
    else
    {   // 1ȸ �׽�Ʈ ���� �ǽ� ��
        if (gu16PTA_HeaterActTimer == 0)
        {   // ���� �Ϸ� ��
            NoPowerSet();

            mu16AD_Heater = Get_ADC_Data(ADC_ID_V_FB);     // ���� üũ�� ��

            ga8PTA_TXD_Data[0] = AT_RS232_STX;
            ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
            ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
            ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
            ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
            ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
            ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

            // ���� �ǵ��
            ga8PTA_TXD_Data[7]  = HEX2ASCII((U8)(mu16AD_Heater >> 12) & 0x0F);
            ga8PTA_TXD_Data[8]  = HEX2ASCII((U8)(mu16AD_Heater >> 8) & 0x0F);
            ga8PTA_TXD_Data[9]  = HEX2ASCII((U8)(mu16AD_Heater >> 4) & 0x0F);
            ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16AD_Heater) & 0x0F);

            gu8PTA_TXD_CS = (U8)(ga8PTA_TXD_Data[0] ^ ga8PTA_TXD_Data[1] ^ ga8PTA_TXD_Data[2] ^
                                 ga8PTA_TXD_Data[3] ^ ga8PTA_TXD_Data[4] ^ ga8PTA_TXD_Data[5] ^
                                 ga8PTA_TXD_Data[6] ^ ga8PTA_TXD_Data[7] ^ ga8PTA_TXD_Data[8] ^
                                 ga8PTA_TXD_Data[9] ^ ga8PTA_TXD_Data[10]);

            ga8PTA_TXD_Data[11] = HEX2ASCII((gu8PTA_TXD_CS >> 4) & 0x0F);
            ga8PTA_TXD_Data[12] = HEX2ASCII(gu8PTA_TXD_CS & 0x0F);
            ga8PTA_TXD_Data[13] = AT_RS232_ETX;

            gu8PTA_UART_TXD_Length = 13;
            PTA_UART_TXD_Start();
        }
    }
}


/// @brief      PTA �˻� 0x5000 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5000 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5000_Send(void)
{
    NoPowerSet();       // ��� ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5010 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5010 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5010_Send(void)
{
    if (gu8PTA_HeaterTestEnd == CLEAR)
    {   // ���� �׽�Ʈ ������ �ǽõ��� ���� ���
        gu8PTA_HeaterTestEnd = SET;

        NoPowerSet();

        TurnOn_Heater();                // ���� ON

        gu16PTA_HeaterActTimer = 70;    // ���� �ִ� ���� �ð� 7sec
    }

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5011 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5011 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5011_Send(void)
{
    TurnOff_Heater();               // ���� OFF

    gu16PTA_HeaterActTimer = 0;

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5012 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5012 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5012_Send(void)
{
    // Ż�� ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5013 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5013 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5013_Send(void)
{
    // Ż�� ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5100 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5100 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5100_Send(void)
{
//    TURN_ON_VALVE_1();          // ���� �Լ� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5101 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5101 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5101_Send(void)
{
//    TURN_OFF_VALVE_1();          // ���� �Լ� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5102 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5102 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5102_Send(void)
{
    // �ü� �Լ� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5103 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5103 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5103_Send(void)
{
    // �ü� �Լ� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5104 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5104 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5104_Send(void)
{
    // �¼� �Լ� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5105 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5105 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5105_Send(void)
{
    // �¼� �Լ� ��� OFF
    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5106 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5106 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5106_Send(void)
{
    // ���� ���� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5107 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5107 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5107_Send(void)
{
    // ���� ���� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5108 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5108 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5108_Send(void)
{
    // �ü� ���� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5109 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5109 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5109_Send(void)
{
    // �ü� ���� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x510A Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x510A Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_510A_Send(void)
{
    // �¼� ���� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x510B Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x510B Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_510B_Send(void)
{
    // �¼� ���� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x510C Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x510C Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_510C_Send(void)
{
    // 2Way ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x510D Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x510D Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_510D_Send(void)
{
    // 2Way ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x510E Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x510E Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_510E_Send(void)
{
    // ���� �ǵ� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x510F Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x510F Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_510F_Send(void)
{
    // ���� �ǵ� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5110 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5110 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5110_Send(void)
{
    // ���� NOS ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5111 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5111 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5111_Send(void)
{
    // ���� NOS ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5112 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5112 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5112_Send(void)
{
    // ��� �巹�� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5113 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5113 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5113_Send(void)
{
    // ��� �巹�� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5114 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5114 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5114_Send(void)
{
    // �¼� �巹�� ��� ON
    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5115 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5115 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5115_Send(void)
{
    // �¼� �巹�� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5116 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5116 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5116_Send(void)
{
    // ICE TRAY ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5117 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5117 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5117_Send(void)
{
    // ICE Ʈ���� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5118 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5118 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5118_Send(void)
{
    // �巹�� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5119 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5119 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5119_Send(void)
{
    // �巹�� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x511A Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x511A Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_511A_Send(void)
{
    // ��� ��� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x511B Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x511B Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_511B_Send(void)
{
    // ��� ��� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x511C Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x511C Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_511C_Send(void)
{
    // ��� �Լ� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x511D Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x511D Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_511D_Send(void)
{
    // ��� �Լ� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x511E Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x511E Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_511E_Send(void)
{
    // ź�� ��� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x511F Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x511F Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_511F_Send(void)
{
    // ź�� ��� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5120 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5120 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5120_Send(void)
{
    // ź�� ���� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5121 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5121 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5121_Send(void)
{
    // ź�� ���� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5122 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5122 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5122_Send(void)
{
    // �ø� ��ȯ ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5123 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5123 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5123_Send(void)
{
    // �ø� ��ȯ ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5124 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5124 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5124_Send(void)
{
    // �ְ��� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5125 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5125 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5125_Send(void)
{
    // �ְ��� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5126 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5126 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5126_Send(void)
{
    // �̿� ��ȯ ���� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5127 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5127 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5127_Send(void)
{
    //  �̿� ��ȯ ���� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5128 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5128 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5128_Send(void)
{
    // �̳뼾�� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5129 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5129 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5129_Send(void)
{
    // �̳뼾�� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x512A Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x512A Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_512A_Send(void)
{
    // �¼� ��ũ �����÷ο� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x512B Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x512B Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_512B_Send(void)
{
    // �¼� ��ũ �����÷ο� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x512C Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x512C Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_512C_Send(void)
{
    // �ü� ��ũ �����÷ο� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x512D Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x512D Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_512D_Send(void)
{
    // �ü� ��ũ �����÷ο� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x512E Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x512E Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_512E_Send(void)
{
    // �ÿ��� ���� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x512F Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x512F Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_512F_Send(void)
{
    // �ÿ��� ���� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5130 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5130 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5130_Send(void)
{
    // ����� �Լ� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5131 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5131 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5131_Send(void)
{
    // ����� �Լ� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5132 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5132 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5132_Send(void)
{
    // ���м� �Լ� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5133 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5133 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5133_Send(void)
{
    // ���м� �Լ� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5134 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5134 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5134_Send(void)
{
    // ���м� ��� ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5135 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5135 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5135_Send(void)
{
    // ���м� ��� ��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5200 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5200 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5200_Send(void)
{
    // �ð��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5201 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5201 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5201_Send(void)
{
    // �ð��� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5202 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5202 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5202_Send(void)
{
    // �����θ���1 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5203 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5203 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5203_Send(void)
{
    // �����θ���1 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5204 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5204 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5204_Send(void)
{
    // ICE ���� ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5205 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5205 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5205_Send(void)
{
    // ICE ���� ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5206 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5206 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5206_Send(void)
{
    // ICE TRAY ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5207 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5207 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5207_Send(void)
{
    // ICE TRAY ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5208 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5208 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5208_Send(void)
{
    // ICE ���� ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5209 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5209 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5209_Send(void)
{
    // ICE ���� ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x520A Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x520A Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_520A_Send(void)
{
    // ICE ���� ���� ������ ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x520B Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x520B Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_520B_Send(void)
{
    // ICE ���� ���� ������ OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x520C Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x520C Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_520C_Send(void)
{
    // ICE ���� ���� ������ ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x520D Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x520D Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_520D_Send(void)
{
    // ICE ���� ���� ������ OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x520E Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x520E Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_520E_Send(void)
{
    // ����Ʈ�� ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x520F Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x520F Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_520F_Send(void)
{
    // ����Ʈ�� ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5210 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5210 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5210_Send(void)
{
    // ������ ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5211 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5211 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5211_Send(void)
{
    // ������ OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5212 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5212 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5212_Send(void)
{
    // ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5213 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5213 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5213_Send(void)
{
    // ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5214 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5214 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5214_Send(void)
{
    // ���ݸ��� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5215 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5215 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5215_Send(void)
{
    // ���� ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5216 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5216 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5216_Send(void)
{
    // ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5217 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5217 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5217_Send(void)
{
    // ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5300 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5300 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5300_Send(void)
{
    TURN_ON_BOOST_OUT();        // �ν�Ʈ ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5301 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5301 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5301_Send(void)
{
    TURN_OFF_BOOST_OUT();        // �ν�Ʈ ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5302 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5302 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5302_Send(void)
{
    // �巹�� ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5303 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5303 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5303_Send(void)
{
    // �巹�� ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5304 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5304 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5304_Send(void)
{
    // �ü� ��ȯ ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5305 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5305 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5305_Send(void)
{
    // �ü� ��ȯ ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5306 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5306 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5306_Send(void)
{
    // �¼� ���� ���� ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x5307 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x5307 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_5307_Send(void)
{
    // �¼� ���� ���� OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7000 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7000 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7000_Send(void)
{
    TURN_ON_UV_1();         // UV LED ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7001 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7001 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7001_Send(void)
{
    TURN_OFF_UV_1();         // UV LED OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7002 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7002 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7002_Send(void)
{
    // LED2 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7003 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7003 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7003_Send(void)
{
    // LED2 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7004 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7004 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7004_Send(void)
{
    // LED3 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7005 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7005 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7005_Send(void)
{
    // LED3 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7006 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7006 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7006_Send(void)
{
    // LED4 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7007 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7007 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7007_Send(void)
{
    // LED4 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7008 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7008 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7008_Send(void)
{
    // LED5 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x7009 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x7009 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_7009_Send(void)
{
    // LED5 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x9900 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x9900 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_9900_Send(void)
{
    // COMP ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x9901 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x9901 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_9901_Send(void)
{
    // COMP OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x9902 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x9902 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_9902_Send(void)
{
    // ���� �ý���(�ð�)

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x9903 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x9903 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_9903_Send(void)
{
    // ���� �ý���(����)

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0x9904 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0x9904 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_9904_Send(void)
{
    // COMP ����(����)

    PTA_ACK_PowerTest();
}


/// @brief      PTA �˻� 0xFF00 Command�� ���� ACK �Լ�
/// @details    PTA �˻� 0xFF00 Command�� ���� ACK�� �۽��Ѵ�
/// @param      void
/// @return     void
void PTA_ACK_CMD_FF00_Send(void)
{
    gu8PTA_TestEnd = SET;           // PTA �˻� �Ϸ�

    NoPowerSet();                   // ��� ���� OFF

    ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_3);      // ��� ���� ����
}


/// @brief      PTA �˻� �Ϸ� �� ��� �˻� ���(PTA ���3) ���� �Լ�
/// @details    PTA �˻� �Ϸ� �� ��� �˻� ���(PTA ���3) ������ �Ѵ�
/// @param      void
/// @return     void
void PTA_Mode3_Act(void)
{
    U8 mu8OperationMode = 0;

    mu8OperationMode = Get_OperationMode();

    if (mu8OperationMode == OPERATION_MODE_ID_PTA_TEST_3)
    {	// PTA ���3 ���۽� ���� ���� ���� �߰�
//        TURN_ON_DC_FAN_1();         // �ð� Fan ON
//        TURN_ON_VALVE_1();          // �Լ� Feed Valve ON
    }
}



#endif

// Lib Water Error Module *********************************************************************************

/// @brief      Lib PTA Mode Module Initilaize
/// @details    ������ ���� �ڵ�ȭ �˻� ��� ���� �����͵��� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_PTA_Mode_Module_Initialize(void)
{
#if (PTA_TEST_MODE_USE == USE)
    Lib_PTA_Mode_Initialize();
#endif
}


/// @brief      Lib PTA Mode Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� PTA �˻� ���� ��� Library
/// @param      void
/// @return     void
void Lib_PTA_Mode_Module_1ms_Control(void)
{
#if (PTA_TEST_MODE_USE == USE)
    PTA_ControlTimer();
    PTA_HeaterTestPowerTimeOut();
#endif
}


/// @brief      Lib PTA Mode Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� PTA �˻� ���� ��� Library
/// @param      void
/// @return     void
void Lib_PTA_Mode_Module_Control(void)
{
#if (PTA_TEST_MODE_USE == USE)
    Lib_PTA_Mode_TimeControl();
    Lib_PTA_Mode_RXD_Data_Control();
    PTA_Mode3_Act();
#endif
}



