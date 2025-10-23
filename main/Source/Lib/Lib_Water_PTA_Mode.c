/// @file     Lib_Water_PTA_Mode.c
/// @date     2025/06/27
/// @author   Jaejin Ham
/// @brief    공장 자동화(PTA) 테스트 모드 관련 File

#include "Global_Header.h"

#if (PTA_TEST_MODE_USE == USE)

U8 gu8PTA_ControlTimer = 0;                             /// @brief  PTA검사 모드 제어 시간 타이머
U16 gu16PTA_Mode_InTimeOut = 0;                         /// @brief  PTA검사 모드 진입 가능 Time Out

U8 gu8PTA_UART_Rx_OK = 0;                               /// @brief  PTA검사 모드 동작시 UART RX 완료 상태 - 0(수신 미완료), 1(수신 완료)
U8 gu8PTA_UART_Rx_Error = 0;                            /// @brief  PTA검사 모드 동작시 UART RX Error발생 상태 - 0(미발생), 1(발생)
U8 gu8PCB_Teat_PTA_UART = 0;                            /// @brief  PCB검사 모드 동작시 PTA검사용 UART 검사 수신 상태 표시 토글 변수

U8 ga8PTA_TXD_Data[PTA_UART_DATA_LENGTH];               /// @brief  PTA검사 UART TX 데이터 버퍼
U8 ga8PTA_RXD_Data[PTA_UART_DATA_LENGTH];               /// @brief  PTA검사 UART RX 데이터 버퍼

U8 gu8PTA_UART_Rx_DataCount = 0;                        /// @brief  PTA검사 UART RX 데이터 카운트
U8 gu8PTA_UART_Tx_DataCount = 0;                        /// @brief  PTA검사 UART TX 데이터 카운트
U8 gu8PTA_UART_RXD_CheckTimeOut = 0;                    /// @brief  PTA검사 UART RXD 대기 Time out
U8 gu8PTA_UART_TXD_Length = 0;                          /// @brief  PTA검사 UART TXD 데이터 Length

U8 gu8FrontPBA_SW_Version = 0;                          /// @brief  Front PBA SW 버전
U8 gu8MainPBA_SW_Version = 0;                           /// @brief  Main PBA SW 버잔

U8 gu8OperatingPTAMode = 0;                             /// @brief  동작중인 PTA 모드 단계

U8 gu8PTA_LeakTestEnd = 0;                              /// @brief  누수 감지 테스트 완료 여부 - 0(미완료), 1(완료)
U8 gu8PTA_KeyTestEnd = 0;                               /// @brief  키 테스트 완료 여부 - 0(미완료), 1(완료)
U8 gu8PTA_Mode1_End = 0;                                /// @brief  PTA 테스트 모드1 완료 여부 - 0(미완료), 1(완료)
U8 gu8PTA_HeaterTestEnd = 0;                            /// @brief  PTA 테스트 모드의 히터 동작 테스트 완료 여부 - 0(미완료), 1(완료)
U8 gu8PTA_TestEnd = 0;                                  /// @brief  PTA 테스트 완료 여부 - 0(미완료), 1(완료)

U16 gu16PTA_HeaterActTimer = 0;                         /// @brief  자동화 검사 모드에서 히터 테스트 동작 Timer

U8 gu8PTA_AT_EEPROM = 0;                                /// @brief  PTA 검사 시작시 Read한 EEPROM 데이터
U16 gu16PTA_RXD_Command = 0;                            /// @brief  수신한 테스트 동작 Command
U8 gu8PTA_RXD_Data1 = 0;                                /// @brief  수신한 테스트 데이터1
U8 gu8PTA_RXD_CS = 0;                                   /// @brief  수신한 CheckSum
U8 gu8PTA_TXD_CS = 0;                                   /// @brief  송신할 CheckSum

U8 gu8PTA_Mode_Boot = 0;                                /// @brief  PTA모드 부팅 동작 상태 - 0(미동작), 1(동작)
U16 gu16PTA_Mode_BootAct_Timer = 0;                     /// @brief  PTA모드 부팅 동작 시간(@100ms)


/// @brief      자동화 검사 관련 변수들 초기화 함수
/// @details    자동화 검사 관련 변수를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Lib_PTA_Mode_Initialize(void)
{
    gu8PTA_ControlTimer = 0;
    gu16PTA_Mode_InTimeOut = PTA_MODE_IN_TIME_OUT;              // 전원 인가 후 PTA 검사 진입 가능 시간 체크

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


/// @brief      무부하 상태 설정 함수
/// @details    무부하 처리시 모든 HW 부하를 OFF 시킨다
/// @param      void
/// @return     void
void NoPowerSet(void)
{
    TURN_OFF_HEATER_1();

    TURN_OFF_VALVE_1();

    TURN_OFF_BOOST_OUT();
}


/// @brief      PTA 검사 모드 UART TXD 송신 시작 함수
/// @details    PTA 검사 모드에서 UART로 TXD 데이터를 송신 시작한다
/// @param      void
/// @return     void
void PTA_UART_TXD_Start(void)
{
    U8 mu8OperatingModeStatus = 0;

    mu8OperatingModeStatus = Get_OperationMode();          // 현재 동작 중인 모드 확인

    if ( (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_1) ||
         (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_2) ||
         (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_3) )
    {   // PTA 검사 모드인 경우
        if (gu8PTA_UART_TXD_Length)
        {   // TX할 데이터가 있는 경우
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
    {   // PCB 검사 모드 인 경우
        PTA_UART_TXD_INTERRUPT_REGISTER = 1U;               // disable INTSTx interrupt
        PTA_UART_TXD_DATA_REGISTER = AT_RS232_ETX;
        PTA_UART_TXD_INTERRUPT_REGISTER = 0U;               // enable INTSTx interrupt

        gu8PTA_UART_RXD_CheckTimeOut = 3;                   // Rx 체크 Time out 300ms
    }
}


/// @brief      PTA 검사 모드 UART TXD Interrupt 처리 함수(@UART Txd Interrupt)
/// @details    PTA 검사기와 URAT 통신간 Tx Interrupt 동작을 처리한다
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


/// @brief      PTA 검사 모드 UART RXD Interrupt 처리 함수(@UART Rxd Interrupt)
/// @details    PTA 검사기와 URAT 통신간 Rx Interrupt 동작을 처리한다
/// @param      void
/// @return     void
void UART_ISR_PTA_Rx(void)
{
    U8 mu8RxData;
    U8 mu8OperatingModeStatus = 0;

    mu8RxData = PTA_UART_RXD_DATA_REGISTER;                 // RXD 데이터 Read
    mu8OperatingModeStatus = Get_OperationMode();           // 현재 동작 중인 모드 확인

    if (mu8OperatingModeStatus == OPERATION_MODE_ID_PCB_TEST)
    {   // PCB 테스트 모드에서
        if (mu8RxData == AT_RS232_ETX)
        {
            gu8PCB_Teat_PTA_UART ^= SET;
            gu8PTA_UART_RXD_CheckTimeOut = 0;
        }
    }
    else
    {   // PCB 테스트 모드가 아닌 경우
        if (gu8PTA_UART_Rx_OK == CLEAR)
        {   // 처리해야할 RX 데이터가 없는 경우
            if ( (gu8PTA_UART_Rx_DataCount == 0) && (mu8RxData != AT_RS232_STX) )
            {   // 첫번째 데이터가 STX가 아닌 경우
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
                    {   // 최대 갯수가 넘는 데이터가 수신되는 경우
                        MEMSET(ga8PTA_RXD_Data, '\0', sizeof(ga8PTA_RXD_Data));
                        gu8PTA_UART_Rx_DataCount = 0;
                        gu8PTA_UART_RXD_CheckTimeOut = 0;
                    }
                    else
                    {
                        gu8PTA_UART_Rx_DataCount++;
                        gu8PTA_UART_RXD_CheckTimeOut = 3;    // Rx 체크 Time out 300ms
                    }
                }
            }
        }
    }
}


/// @brief      PTA UART Rx Time Out 체크(@100ms)
/// @details    PTA 검사 UART Rx 데이터를 수신하는 Time Out을 카운트한다
/// @param      void
/// @return     void
void PTA_UART_Rx_TimeOut(void)
{
    U8 mu8OperatingModeStatus = 0;

    mu8OperatingModeStatus = Get_OperationMode();           // 현재 동작 중인 모드 확인

    if (gu8PTA_UART_RXD_CheckTimeOut)
    {
        gu8PTA_UART_RXD_CheckTimeOut--;

        if (gu8PTA_UART_RXD_CheckTimeOut == 0)
        {
            if (mu8OperatingModeStatus == OPERATION_MODE_ID_PCB_TEST)
            {   // PCB테스트 모드인 경우
                gu8PCB_Teat_PTA_UART = CLEAR;
            }
            else
            {   // PCB테스트 모드가 아닌 경우
                MEMSET(ga8PTA_RXD_Data, '\0', sizeof(ga8PTA_RXD_Data));
                gu8PTA_UART_Rx_DataCount = 0;
                gu8PTA_UART_Rx_OK = CLEAR;
            }
        }
    }
}


/// @brief      PTA UART Error Interrupt(Interrupt 루틴)
/// @brief      PTA 검사 UART RX Error 인터럽트 발생시 처리한다
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


/// @brief      자동화 검사 시작시 EEPROM 상태 체크 함수
/// @details    자동화 검사 시작시 EEPROM 데이터를 읽어서 그 데이터를 프로토콜 데이터에 실어 보낼 수 있게 반환한다
/// @param      void
/// @return     return : EEPROM 체크 데이터(END 자리 데이터)
U8 Get_PTA_CheckEEPROM(void)
{
    U8 mu8CheckData = 0;
    EepromSeqRead(EEPROM_ID_END, &mu8CheckData, 1);

    gu8PTA_AT_EEPROM = mu8CheckData;

    return  gu8PTA_AT_EEPROM;
}



/// @brief      자동화 검사시 히터 제어 및 안전 제어 TimeOut 함수(@1ms)
/// @details    자동화 검사 중 안전 이슈로 Time out을 걸어서 히터를 제어하다가 일정시간 후 OFF 시킨다
/// @param      void
/// @return     void
void PTA_HeaterTestPowerTimeOut(void)
{
    // 히터 제어
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


/// @brief      PTA 검사시 Heater ON 함수
/// @details    PTA 검사시 Heater 부하들을 ON 시킨다
/// @param      void
/// @return     void
void TurnOn_Heater(void)
{
    TURN_ON_HEATER_1();
}


/// @brief      PTA 검사시 Heater OFF 함수
/// @details    PTA 검사시 Heater 부하들을 OFF 시킨다
/// @param      void
/// @return     void
void TurnOff_Heater(void)
{
    TURN_OFF_HEATER_1();
}



/// @brief      PTA 검사 모드 제어 시간 카운터(@1ms interrupt)
/// @details    PTA 검사 모드를 제어하기 위해 제어 시간을 카운트 한다
/// @param      void
/// @return     void
void PTA_ControlTimer(void)
{
    if (gu8PTA_ControlTimer < PTA_CONTROL_TIME_PERIOD)
    {
        gu8PTA_ControlTimer++;
    }
}


/// @brief      PTA검사 동작 중 시간 처리 루틴 함수 모음(@While문)
/// @details    PTA검사 동작 중에 UART 및 부하 제어시 시간 제한이 필요한 함수들의 시간에 따른 제어를 처리한다
/// @param      void
/// @return     void
void Lib_PTA_Mode_TimeControl(void)
{
    if (gu8PTA_ControlTimer >= PTA_CONTROL_TIME_PERIOD)
    {   // 100ms 마다 수행
        gu8PTA_ControlTimer = 0;

        if (gu16PTA_Mode_InTimeOut)
        {
            gu16PTA_Mode_InTimeOut--;
        }

        PTA_UART_Rx_TimeOut();              // PTA UART 통신 RXD 대기 Time Out 제어
        PTA_HeaterTestPowerTimeOut();       // PTA 테스트 중 부하 제한 시간 동작 제어
    }
}


/// @brief      PTA검사 진입 가능 시간 상태 확인 함수
/// @details    PTA검사 진입이 가능한 상태(진입 가능 시간이 남음)인지 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 진입 가능 상태 여부 - 0(진입 불가), 1(진입 가능)
U8 Get_Able_In_PTA_Mode(void)
{
    if (gu16PTA_Mode_InTimeOut)
    {   // 모드 진입 가능 시간이 남은 경우
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      PTA검사 동작 중 Front PBA의 프로그램 버전 설정 함수
/// @details    PTA검사 동작에서 사용할 Front PBA로부터 확인된 SW 버전을 설정한다
/// @param      mu8Ver : Front PBA SW 버전
/// @return     void
void Set_PTA_FrontSW_Ver(U8 mu8Ver)
{
    gu8FrontPBA_SW_Version = mu8Ver;
}


/// @brief      PTA검사 동작 중 Front PBA의 프로그램 버전 확인 함수
/// @details    PTA검사 동작에서 사용할 Front PBA의 SW 버전을 확인하여 반환한다
/// @param      void
/// @return     return : Front PBA SW 버전
U8 Get_PTA_FrontSW_Ver(void)
{
    return  gu8FrontPBA_SW_Version;
}


/// @brief      PTA검사 동작 중 Main PBA의 프로그램 버전 설정 함수
/// @details    PTA검사 동작에서 사용할 Main PBA SW 버전을 설정한다
/// @param      mu8Ver : Main PBA SW 버전
/// @return     void
void Set_PTA_MainSW_Ver(U8 mu8Ver)
{
    gu8MainPBA_SW_Version = mu8Ver;
}


/// @brief      PTA검사 동작 중 Main PBA의 프로그램 버전 확인 함수
/// @details    PTA검사 동작에서 사용할 Main PBA의 SW 버전을 확인하여 반환한다
/// @param      void
/// @return     return : Main PBA SW 버전
U8 Get_PTA_MainSW_Ver(void)
{
    return  gu8MainPBA_SW_Version;
}


/// @brief      PTA검사 1모드 동작 완료 설정 함수
/// @details    PTA검사 1모드 동작 완료 상태를 설정한다
/// @param      mu8Data : PTA 검사 1모드 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      PTA검사 1모드 동작 완료 확인 함수
/// @details    PTA검사 1모드 동작 완료 상태를 확인하여 반환한다
/// @param      void
/// @return     return : PTA 검사 1모드 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      PTA검사 1모드에서 누수 감지 동작 완료 설정 함수
/// @details    PTA검사 1모드 동작시 누수 감지 체크 완료 상태를 설정한다
/// @param      mu8Data : PTA 검사 1모드 누수 감지 완료 여부 - 0(미완료), 1(완료)
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

/// @brief      PTA검사 1모드에서 버튼 동작 체크 완료 설정 함수
/// @details    PTA검사 1모드에서 버튼 동작 완료 상태를 설정한다
/// @param      mu8Data : PTA 검사 1모드 버튼 동작 체크 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      PTA검사 1모드에서 진행할 테스트1 동작 완료 설정 함수(진입모드 0x01 수신시 처리 함수)
/// @details    PTA검사 1모드에서 수동 검사 SKIP 동작 완료 여부를 체크한다
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


/// @brief      PTA검사 1모드에서 진행할 테스트2 동작 완료 설정 함수(진입모드 0x02 수신시 처리 함수)
/// @details    PTA검사 1모드에서 수동 검사 동작 완료 여부를 체크한다
/// @param      mu8Data : PTA 검사 1모드 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      PTA검사 동작 완료 설정 함수
/// @details    PTA검사 동작 완료 상태를 설정한다
/// @param      mu8Data : PTA 검사 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      PTA검사 동작 완료 확인 함수
/// @details    PTA검사 동작 완료 상태를 확인하여 반환한다
/// @param      void
/// @return     return : PTA 검사 완료 여부 - 0(미완료), 1(완료)
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


/// @brief      PTA 검사 모드내 RXD DATA를 처리 동작 함수(@While문)
/// @details    PTA 검사 동작시 Rx된 데이터를 동작으로 처리한다
/// @param      void
/// @return     void
void Lib_PTA_Mode_RXD_Data_Control(void)
{
    U8 mu8RXDCheckSum = 0;
    U8 mu8Able_PTA_In = 0;
    U8 mu8OperatingModeStatus = 0;
    U8 mu8PTA_Mode1_End = 0;
    U8 mu8PTA_Test_End = 0;

    mu8OperatingModeStatus = Get_OperationMode();           // 현재 동작 중인 모드 확인

    if (gu8PTA_UART_Rx_OK)
    {   // UART 수신 데이터가 있는 경우
        if ((ga8PTA_RXD_Data[0] != AT_RS232_STX) || (ga8PTA_RXD_Data[gu8PTA_UART_Rx_DataCount] != AT_RS232_ETX))
        {   // STX와 ETX 확인하여 맞지 않는 경우 NACK 회신하고 return
            Clear_PTA_TxDataBuffer();
            PTA_NACK_Send();
            Clear_PTA_RxDataBuffer();
            return;
        }

        // STX와 ETX가 맞는 경우
        // 데이터 변환
        gu16PTA_RXD_Command = (((U16)(ASCII2HEX(ga8PTA_RXD_Data[1])) << 12) & 0xF000) + (((U16)(ASCII2HEX(ga8PTA_RXD_Data[2])) << 8) & 0x0F00) +
                              (((U16)(ASCII2HEX(ga8PTA_RXD_Data[3])) << 4) & 0x00F0) + ((U16)(ASCII2HEX(ga8PTA_RXD_Data[4])) & 0x000F);

        if (gu16PTA_RXD_Command == AT_UART_CMD_MODEL_NAME)
        {   // 초기 통신 시작 Command인 경우
            gu8PTA_RXD_Data1 = ((ASCII2HEX(ga8PTA_RXD_Data[5]) << 4) & 0xF0);
            gu8PTA_RXD_Data1 |= (ASCII2HEX(ga8PTA_RXD_Data[6]) & 0x0F);

            gu8PTA_RXD_CS = ((ASCII2HEX(ga8PTA_RXD_Data[7]) << 4) & 0xF0) + (ASCII2HEX(ga8PTA_RXD_Data[8]) & 0x0F);     // 수신한 CheckSum
            mu8RXDCheckSum = (U8)(ga8PTA_RXD_Data[0] ^ ga8PTA_RXD_Data[1] ^ ga8PTA_RXD_Data[2] ^
                                  ga8PTA_RXD_Data[3] ^ ga8PTA_RXD_Data[4] ^ ga8PTA_RXD_Data[5] ^
                                  ga8PTA_RXD_Data[6]);
        }
        else
        {   // 초기 통신 시작 Command가 아닌 경우
            gu8PTA_RXD_CS = ((ASCII2HEX(ga8PTA_RXD_Data[5]) << 4) & 0xF0) + (ASCII2HEX(ga8PTA_RXD_Data[6]) & 0x0F);     // 수신한 CheckSum
            mu8RXDCheckSum = (U8)(ga8PTA_RXD_Data[0] ^ ga8PTA_RXD_Data[1] ^ ga8PTA_RXD_Data[2] ^
                                  ga8PTA_RXD_Data[3] ^ ga8PTA_RXD_Data[4]);
        }

        if (mu8RXDCheckSum != gu8PTA_RXD_CS)
        {   // Check Sum이 수신한 것과 계산한 것이 다른 경우 NACK 회신하고 return
            Clear_PTA_TxDataBuffer();
            PTA_NACK_Send();
            Clear_PTA_RxDataBuffer();
            return;
        }

        // 프론트 검사 모드 Skip인지 확인
        if (gu16PTA_RXD_Command == AT_UART_CMD_MODEL_NAME)
        {
            if (mu8OperatingModeStatus < OPERATION_MODE_ID_PTA_TEST_2)
            {   // PTA 모드 2 이상 진입하지 않은 경우
                if (gu8PTA_RXD_Data1 == AT_UART_CMD_MODEL_NAME_PRETEST_SKIP)
                {   // 프론트 검사 모드 Skip인 경우
                    mu8Able_PTA_In = Get_Able_In_PTA_Mode();

                    if (mu8Able_PTA_In == TRUE)
                    {   // 테스트 모드 진입 가능 시간인 경우
                        if (mu8OperatingModeStatus != OPERATION_MODE_ID_PTA_TEST_1)
                        {   // PTA 1 Mode 미동작 중인 상태
                            ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_1);     // PTA모드 1 모드로 전환
                            gu16PTA_Mode_InTimeOut = 0;         // PTA 모드 진입 가능 시간 0으로 세팅

                            gu8PTA_Mode_Boot = SET;             // TEST모드 부팅 동작 Flag
                            gu16PTA_Mode_BootAct_Timer = 10;    // TEST모드 부팅 동작 시간 1초

                            EEPROM_Initial();
                            gu8PTA_AT_EEPROM = Get_PTA_CheckEEPROM();

                            // 모든 부하 CLEAR
                            NoPowerSet();

                            Set_BuzzerSelect(BUZZER_SETUP);
                        }
                    }

                    if (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_1)
                    {   // 자동화 테스트 모드1 진입한 경우
                        PTA_Mode1_PreTest1End();        // 테스트 모드 1에서 처리해야할 테스트 완료 여부 체크

                        mu8PTA_Mode1_End = Get_PTA_Mode1_End();     // 테스트 모드1 완료여부 체크

                        if (mu8PTA_Mode1_End == TRUE)
                        {   // PTA 모드 1이 완료된 경우
                            ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_2);

                            NoPowerSet();
                        }
                        else
                        {    // PTA 모드 1이 완료되지 않은 경우
                            Clear_PTA_RxDataBuffer();
                            return;
                        }
                    }
                }
                else if (gu8PTA_RXD_Data1 == AT_UART_CMD_MODEL_NAME_PRETEST)
                {   // 프론트 검사 모드 진행하는 경우
                    mu8Able_PTA_In = Get_Able_In_PTA_Mode();

                    if (mu8Able_PTA_In == TRUE)
                    {   // 테스트 모드 진입 가능 시간인 경우
                        if (mu8OperatingModeStatus != OPERATION_MODE_ID_PTA_TEST_1)
                        {   // PTA 1 Mode 미동작 중인 상태
                            ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_1);     // PTA모드 1 모드로 전환
                            gu16PTA_Mode_InTimeOut = 0;         // PTA 모드 진입 가능 시간 0으로 세팅

                            gu8PTA_Mode_Boot = SET;             // TEST모드 부팅 동작 Flag
                            gu16PTA_Mode_BootAct_Timer = 10;    // TEST모드 부팅 동작 시간 1초

                            EEPROM_Initial();
                            gu8PTA_AT_EEPROM = Get_PTA_CheckEEPROM();

                            // 모든 부하 CLEAR
                            NoPowerSet();

                            Set_BuzzerSelect(BUZZER_SETUP);
                        }
                    }

                    if (mu8OperatingModeStatus == OPERATION_MODE_ID_PTA_TEST_1)
                    {   // 자동화 테스트 모드1 진입한 경우
                        PTA_Mode1_PreTest2End();        // 테스트 모드 1에서 처리해야할 테스트 완료 여부 체크

                        mu8PTA_Mode1_End = Get_PTA_Mode1_End();     // 테스트 모드1 완료여부 체크

                        if (mu8PTA_Mode1_End == TRUE)
                        {   // PTA 모드 1이 완료된 경우
                            ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_2);

                            NoPowerSet();
                        }
                        else
                        {    // PTA 모드 1이 완료되지 않은 경우
                            Clear_PTA_RxDataBuffer();
                            return;
                        }
                    }
                }
                else
                {   // 검사 데이터가 0x01, 0x02가 아닌 경우 수신 데이터 처리 안하고 return
                    Clear_PTA_RxDataBuffer();
                    return;
                }
            }
        }
    }
    else
    {   // UART 수신 데이터가 없는 경우
        Clear_PTA_RxDataBuffer();
        return;
    }

    mu8PTA_Test_End = Get_PTA_Test_End();       // PTA 검사 완료 여부 확인

    if (mu8PTA_Test_End == TRUE)
    {   // 이미 한번이라도 PTA 테스트가 완료된 경우에는 아무런 데이터도 처리 안함
        Clear_PTA_RxDataBuffer();
        return;
    }

    // Check Sum까지 비교한 것이 일치하는 경우
    Clear_PTA_TxDataBuffer();           // Tx 데이터 버퍼 Clear

    switch (gu16PTA_RXD_Command)
    {
        case AT_UART_CMD_MODEL_NAME:                // 모델 정보 송신(0x2000)
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

        default:    // 인식 못하는 Command를 받은 경우에는 NACK 회신
            PTA_NACK_Send();
            break;
    }
    Clear_PTA_RxDataBuffer();       // RXD DATA 처리후 Buffer Clear
}



/// @brief      PTA 검사 RX 관련 변수들 초기화 함수
/// @details    PTA 검사 RX 관련 변수들을 초기화 시킨다
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


/// @brief      PTA 검사 TX 관련 변수들 초기화 함수
/// @details    PTA 검사 TX 관련 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Clear_PTA_TxDataBuffer(void)
{
    MEMSET(ga8PTA_TXD_Data, '\0', sizeof(ga8PTA_TXD_Data));
    gu8PTA_UART_TXD_Length = 0;
    gu8PTA_TXD_CS = 0;
    gu8PTA_UART_Tx_DataCount = 0;
}


/// @brief      PTA 검사 NACK 송신 함수
/// @details    PTA 검사 중 NACK 를 송신 한다
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
    PTA_UART_TXD_Start();       // Tx 시작
}


/// @brief      PTA 검사 부하 검사시 공통 ACK 함수
/// @details    PTA 검사 부하 검사시 공통적으로 회신하는 ACK를 선언하여 사용한다
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


/// @brief      PTA 검사 0x2000 Command에 대한 ACK 함수
/// @details    PTA 검사 0x2000 Command에 대한 ACK를 송신한다
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
    ga8PTA_TXD_Data[7] = AT_UART_MODEL_INFO[0];     // 모델코드
    ga8PTA_TXD_Data[8] = AT_UART_MODEL_INFO[1];     // 모델코드
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


/// @brief      PTA 검사 0x3000 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3000 Command에 대한 ACK를 송신한다
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
    ga8PTA_TXD_Data[13] = '0';   // 고도 모드
    ga8PTA_TXD_Data[14] = HEX2ASCII(mu8HeightMode);    // 고도 모드

    ga8PTA_TXD_Data[15] = AT_UART_MODEL_CONTRY[0];   // 내수 모델
    ga8PTA_TXD_Data[16] = AT_UART_MODEL_CONTRY[1];   // 내수 모델

//    mu8ModelType = Get_ModelType();     // 모델 종류 확인
    if (mu8ModelType == MODEL_CP)
    {
        ga8PTA_TXD_Data[17] = AT_UART_MODEL_TYPE_CP[0];   // 냉수 전용
        ga8PTA_TXD_Data[18] = AT_UART_MODEL_TYPE_CP[1];   // 냉수 전용
    }
    else
    {
        ga8PTA_TXD_Data[17] = AT_UART_MODEL_TYPE_CHP[0];   // 냉온수 전용
        ga8PTA_TXD_Data[18] = AT_UART_MODEL_TYPE_CHP[1];   // 냉온수 전용
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


/// @brief      PTA 검사 0x3010 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3010 Command에 대한 ACK를 송신한다
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
        ga8PTA_TXD_Data[7] = AT_UART_WIFI_CONNECT[0];   // WIFI 연결 상태
        ga8PTA_TXD_Data[8] = AT_UART_WIFI_CONNECT[1];   // WIFI 연결 상태
    }
    else
    {
        ga8PTA_TXD_Data[7] = AT_UART_WIFI_NO_CONNECT[0];   // WIFI 연결 상태
        ga8PTA_TXD_Data[8] = AT_UART_WIFI_NO_CONNECT[1];   // WIFI 연결 상태
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


/// @brief      PTA 검사 0x3011 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3011 Command에 대한 ACK를 송신한다
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


/// @brief      PTA 검사 0x3012 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3012 Command에 대한 ACK를 송신한다
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


/// @brief      PTA 검사 0x3013 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3013 Command에 대한 ACK를 송신한다
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

    // DC Current - 6자리
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


/// @brief      PTA 검사 0x3014 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3014 Command에 대한 ACK를 송신한다
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


/// @brief      PTA 검사 0x3015 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3015 Command에 대한 ACK를 송신한다
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


/// @brief      PTA 검사 0x3016 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3016 Command에 대한 ACK를 송신한다
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


/// @brief      PTA 검사 0x3017 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3017 Command에 대한 ACK를 송신한다
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


/// @brief      PTA 검사 0x3018 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3018 Command에 대한 ACK를 송신한다
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


/// @brief      PTA 검사 0x3019 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3019 Command에 대한 ACK를 송신한다
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


/// @brief      PTA 검사 0x301A Command에 대한 ACK 함수
/// @details    PTA 검사 0x301A Command에 대한 ACK를 송신한다
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

    // Current - 6자리
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


/// @brief      PTA 검사 0x301B Command에 대한 ACK 함수
/// @details    PTA 검사 0x301B Command에 대한 ACK를 송신한다
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

    // Current - 6자리
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


/// @brief      PTA 검사 0x301C Command에 대한 ACK 함수
/// @details    PTA 검사 0x301C Command에 대한 ACK를 송신한다
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

    // 조도 센서 AD값
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


/// @brief      PTA 검사 0x3020 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3020 Command에 대한 ACK를 송신한다
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

    // 냉수 온도
    if ( (mu16AD_ColdTemp < COLD_TEMP_1_ERROR_UNDER_STANDARD_AD) ||
         (mu16AD_ColdTemp > COLD_TEMP_1_ERROR_OVER_STANDARD_AD) )
    {   // 에러 범위 이면
        ga8PTA_TXD_Data[7] = AT_UART_TEMP_SENSOR_ERROR[0];      // 냉수 온도 센서
        ga8PTA_TXD_Data[8] = AT_UART_TEMP_SENSOR_ERROR[1];      // 냉수 온도 센서
        ga8PTA_TXD_Data[9] = AT_UART_TEMP_SENSOR_ERROR[2];      // 냉수 온도 센서
        ga8PTA_TXD_Data[10] = AT_UART_TEMP_SENSOR_ERROR[3];     // 냉수 온도 센서
    }
    else
    {   // 에러 범위가 아니면
        if (mf32ColdTemp < 0.0f)
        {   // 0도보다 낮으면 0도로 고정
            mf32ColdTemp = 0.0f;
        }
        else if (mf32ColdTemp > 99.0f)
        {   // 99도보다 높으면 99도로 고정
            mf32ColdTemp = 99.0f;
        }
        mu16ColdTemp = (U16)(mf32ColdTemp * 100.0f);
        ga8PTA_TXD_Data[7] = HEX2ASCII((U8)(mu16ColdTemp >> 12) & 0x0F);   // 냉수 온도 센서
        ga8PTA_TXD_Data[8] = HEX2ASCII((U8)(mu16ColdTemp >> 8) & 0x0F);    // 냉수 온도 센서
        ga8PTA_TXD_Data[9] = HEX2ASCII((U8)(mu16ColdTemp >> 4) & 0x0F);    // 냉수 온도 센서
        ga8PTA_TXD_Data[10] = HEX2ASCII((U8)(mu16ColdTemp) & 0x0F);        // 냉수 온도 센서
    }

    // 냉수 온도 센서 AD값
    ga8PTA_TXD_Data[11] = HEX2ASCII((U8)(mu16AD_ColdTemp >> 12) & 0x0F);
    ga8PTA_TXD_Data[12] = HEX2ASCII((U8)(mu16AD_ColdTemp >> 8) & 0x0F);
    ga8PTA_TXD_Data[13] = HEX2ASCII((U8)(mu16AD_ColdTemp >> 4) & 0x0F);
    ga8PTA_TXD_Data[14] = HEX2ASCII((U8)(mu16AD_ColdTemp) & 0x0F);

    // 온수 온도
    if ( (mu16AD_HotTemp < TANK_HOT_TEMP_ERROR_UNDER_STANDARD_AD) ||
         (mu16AD_HotTemp > TANK_HOT_TEMP_ERROR_OVER_STANDARD_AD) )
    {   // 에러 범위 이면
        ga8PTA_TXD_Data[15] = AT_UART_TEMP_SENSOR_ERROR[0];     // 온수 온도 센서
        ga8PTA_TXD_Data[16] = AT_UART_TEMP_SENSOR_ERROR[1];     // 온수 온도 센서
        ga8PTA_TXD_Data[17] = AT_UART_TEMP_SENSOR_ERROR[2];     // 온수 온도 센서
        ga8PTA_TXD_Data[18] = AT_UART_TEMP_SENSOR_ERROR[3];     // 온수 온도 센서
    }
    else
    {   // 에러 범위가 아니면
        if (mf32HotTemp < 0.0f)
        {   // 0도보다 낮으면 0도로 고정
            mf32HotTemp = 0.0f;
        }
        else if (mf32HotTemp > 99.0f)
        {   // 99도보다 높으면 99도로 고정
            mf32HotTemp = 99.0f;
        }
        mu16HotTemp = (U16)(mf32HotTemp * 100.0f);
        ga8PTA_TXD_Data[15] = HEX2ASCII((U8)(mu16HotTemp >> 12) & 0x0F);    // 온수 온도 센서
        ga8PTA_TXD_Data[16] = HEX2ASCII((U8)(mu16HotTemp >> 8) & 0x0F);     // 온수 온도 센서
        ga8PTA_TXD_Data[17] = HEX2ASCII((U8)(mu16HotTemp >> 4) & 0x0F);     // 온수 온도 센서
        ga8PTA_TXD_Data[18] = HEX2ASCII((U8)(mu16HotTemp) & 0x0F);          // 온수 온도 센서
    }

    // 온수 온도 센서 AD값
    ga8PTA_TXD_Data[19] = HEX2ASCII((U8)(mu16AD_HotTemp >> 12) & 0x0F);
    ga8PTA_TXD_Data[20] = HEX2ASCII((U8)(mu16AD_HotTemp >> 8) & 0x0F);
    ga8PTA_TXD_Data[21] = HEX2ASCII((U8)(mu16AD_HotTemp >> 4) & 0x0F);
    ga8PTA_TXD_Data[22] = HEX2ASCII((U8)(mu16AD_HotTemp) & 0x0F);

    // 외기 온도
    if ( (mu16AD_AirTemp < AIR_TEMP_1_ERROR_UNDER_STANDARD_AD) ||
         (mu16AD_AirTemp > AIR_TEMP_1_ERROR_OVER_STANDARD_AD) )
    {   // 에러 범위 이면
        ga8PTA_TXD_Data[23] = AT_UART_TEMP_SENSOR_ERROR[0];     // 외기 온도 센서
        ga8PTA_TXD_Data[24] = AT_UART_TEMP_SENSOR_ERROR[1];     // 외기 온도 센서
        ga8PTA_TXD_Data[25] = AT_UART_TEMP_SENSOR_ERROR[2];     // 외기 온도 센서
        ga8PTA_TXD_Data[26] = AT_UART_TEMP_SENSOR_ERROR[3];     // 외기 온도 센서
    }
    else
    {   // 에러 범위가 아니면
        if (mf32AirTemp < 0.0f)
        {   // 0도보다 낮으면 0도로 고정
            mf32AirTemp = 0.0f;
        }
        else if (mf32AirTemp > 99.0f)
        {   // 99도보다 높으면 99도로 고정
            mf32AirTemp = 99.0f;
        }
        mu16AirTemp = (U16)(mf32AirTemp * 100.0f);
        ga8PTA_TXD_Data[23] = HEX2ASCII((U8)(mu16AirTemp >> 12) & 0x0F);    // 외기 온도 센서
        ga8PTA_TXD_Data[24] = HEX2ASCII((U8)(mu16AirTemp >> 8) & 0x0F);     // 외기 온도 센서
        ga8PTA_TXD_Data[25] = HEX2ASCII((U8)(mu16AirTemp >> 4) & 0x0F);     // 외기 온도 센서
        ga8PTA_TXD_Data[26] = HEX2ASCII((U8)(mu16AirTemp) & 0x0F);          // 외기 온도 센서
    }

    // 외기 온도 센서 AD값
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


/// @brief      PTA 검사 0x3020 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3020 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_3030_Send(void)
{
    U8 mu8LowLevel = 0;
    U8 mu8HighLevel = 0;
    U8 mu8OverFlow = 0;

    // 해당하는 수위 센서의 Port의 상태 확인
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

    // 정수 저수위
    if (mu8LowLevel)
    {   // 미체결
        ga8PTA_TXD_Data[7] = AT_UART_LEVEL_SENSOR_NO_CONNECT[0];
        ga8PTA_TXD_Data[8] = AT_UART_LEVEL_SENSOR_NO_CONNECT[1];
    }
    else
    {   // 체결
        ga8PTA_TXD_Data[7] = AT_UART_LEVEL_SENSOR_CONNECT[0];
        ga8PTA_TXD_Data[8] = AT_UART_LEVEL_SENSOR_CONNECT[1];
    }

    // 정수 만수위
    if (mu8HighLevel)
    {   // 미체결
        ga8PTA_TXD_Data[9] = AT_UART_LEVEL_SENSOR_NO_CONNECT[0];
        ga8PTA_TXD_Data[10] = AT_UART_LEVEL_SENSOR_NO_CONNECT[1];
    }
    else
    {   // 체결
        ga8PTA_TXD_Data[9] = AT_UART_LEVEL_SENSOR_CONNECT[0];
        ga8PTA_TXD_Data[10] = AT_UART_LEVEL_SENSOR_CONNECT[1];
    }

    // OverFlow
    if (mu8OverFlow)
    {   // 미체결
        ga8PTA_TXD_Data[11] = AT_UART_LEVEL_SENSOR_NO_CONNECT[0];
        ga8PTA_TXD_Data[12] = AT_UART_LEVEL_SENSOR_NO_CONNECT[1];
    }
    else
    {   // 체결
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


/// @brief      PTA 검사 0x3040 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3040 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_3040_Send(void)
{
    U8 mu8FilterCover = 0;

    // 해당하는 센서의 Port의 상태 확인
//    mu8FilterCover = GET_STATUS_FILTER_COVER_1();

    ga8PTA_TXD_Data[0] = AT_RS232_STX;
    ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
    ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
    ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
    ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
    ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
    ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

    // 필터 커버
    if (mu8FilterCover)
    {   // 미체결
        ga8PTA_TXD_Data[7] = AT_UART_LEVEL_SENSOR_NO_CONNECT[0];
        ga8PTA_TXD_Data[8] = AT_UART_LEVEL_SENSOR_NO_CONNECT[1];
    }
    else
    {   // 체결
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


/// @brief      PTA 검사 0x3050 Command에 대한 ACK 함수
/// @details    PTA 검사 0x3050 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_3050_Send(void)
{
    U16 mu16AD_Heater = 0;

    if (gu8PTA_HeaterTestEnd == CLEAR)
    {   // 히터 테스트 동작이 실시되지 않은 경우
        gu8PTA_HeaterTestEnd = SET;

        NoPowerSet();

        TurnOn_Heater();                    // 히터 ON

        gu16PTA_HeaterActTimer = 2;         // 히터 최대 동작 시간 200ms
    }
    else
    {   // 1회 테스트 동작 실시 후
        if (gu16PTA_HeaterActTimer == 0)
        {   // 동작 완료 후
            NoPowerSet();

            mu16AD_Heater = Get_ADC_Data(ADC_ID_V_FB);     // 전압 체크된 값

            ga8PTA_TXD_Data[0] = AT_RS232_STX;
            ga8PTA_TXD_Data[1] = AT_UART_ACK[0];   // ACK 0x06
            ga8PTA_TXD_Data[2] = AT_UART_ACK[1];   // ACK 0x06
            ga8PTA_TXD_Data[3] = ga8PTA_RXD_Data[1];    // CMD
            ga8PTA_TXD_Data[4] = ga8PTA_RXD_Data[2];    // CMD
            ga8PTA_TXD_Data[5] = ga8PTA_RXD_Data[3];    // CMD
            ga8PTA_TXD_Data[6] = ga8PTA_RXD_Data[4];    // CMD

            // 히터 피드백
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


/// @brief      PTA 검사 0x5000 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5000 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5000_Send(void)
{
    NoPowerSet();       // 모든 부하 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5010 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5010 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5010_Send(void)
{
    if (gu8PTA_HeaterTestEnd == CLEAR)
    {   // 히터 테스트 동작이 실시되지 않은 경우
        gu8PTA_HeaterTestEnd = SET;

        NoPowerSet();

        TurnOn_Heater();                // 히터 ON

        gu16PTA_HeaterActTimer = 70;    // 히터 최대 동작 시간 7sec
    }

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5011 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5011 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5011_Send(void)
{
    TurnOff_Heater();               // 히터 OFF

    gu16PTA_HeaterActTimer = 0;

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5012 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5012 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5012_Send(void)
{
    // 탈빙 히터 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5013 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5013 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5013_Send(void)
{
    // 탈빙 히터 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5100 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5100 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5100_Send(void)
{
//    TURN_ON_VALVE_1();          // 정수 입수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5101 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5101 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5101_Send(void)
{
//    TURN_OFF_VALVE_1();          // 정수 입수 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5102 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5102 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5102_Send(void)
{
    // 냉수 입수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5103 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5103 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5103_Send(void)
{
    // 냉수 입수 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5104 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5104 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5104_Send(void)
{
    // 온수 입수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5105 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5105 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5105_Send(void)
{
    // 온수 입수 밸브 OFF
    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5106 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5106 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5106_Send(void)
{
    // 정수 추출 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5107 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5107 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5107_Send(void)
{
    // 정수 추출 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5108 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5108 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5108_Send(void)
{
    // 냉수 추출 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5109 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5109 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5109_Send(void)
{
    // 냉수 추출 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x510A Command에 대한 ACK 함수
/// @details    PTA 검사 0x510A Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_510A_Send(void)
{
    // 온수 추출 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x510B Command에 대한 ACK 함수
/// @details    PTA 검사 0x510B Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_510B_Send(void)
{
    // 온수 추출 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x510C Command에 대한 ACK 함수
/// @details    PTA 검사 0x510C Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_510C_Send(void)
{
    // 2Way 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x510D Command에 대한 ACK 함수
/// @details    PTA 검사 0x510D Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_510D_Send(void)
{
    // 2Way 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x510E Command에 대한 ACK 함수
/// @details    PTA 검사 0x510E Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_510E_Send(void)
{
    // 원수 피드 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x510F Command에 대한 ACK 함수
/// @details    PTA 검사 0x510F Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_510F_Send(void)
{
    // 원수 피드 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5110 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5110 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5110_Send(void)
{
    // 원수 NOS 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5111 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5111 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5111_Send(void)
{
    // 원수 NOS 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5112 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5112 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5112_Send(void)
{
    // 출수 드레인 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5113 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5113 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5113_Send(void)
{
    // 출수 드레인 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5114 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5114 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5114_Send(void)
{
    // 온수 드레인 밸브 ON
    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5115 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5115 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5115_Send(void)
{
    // 온수 드레인 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5116 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5116 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5116_Send(void)
{
    // ICE TRAY 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5117 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5117 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5117_Send(void)
{
    // ICE 트레이 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5118 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5118 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5118_Send(void)
{
    // 드레인 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5119 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5119 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5119_Send(void)
{
    // 드레인 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x511A Command에 대한 ACK 함수
/// @details    PTA 검사 0x511A Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_511A_Send(void)
{
    // 살균 배수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x511B Command에 대한 ACK 함수
/// @details    PTA 검사 0x511B Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_511B_Send(void)
{
    // 살균 배수 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x511C Command에 대한 ACK 함수
/// @details    PTA 검사 0x511C Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_511C_Send(void)
{
    // 살균 입수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x511D Command에 대한 ACK 함수
/// @details    PTA 검사 0x511D Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_511D_Send(void)
{
    // 살균 입수 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x511E Command에 대한 ACK 함수
/// @details    PTA 검사 0x511E Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_511E_Send(void)
{
    // 탄산 배수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x511F Command에 대한 ACK 함수
/// @details    PTA 검사 0x511F Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_511F_Send(void)
{
    // 탄산 배수 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5120 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5120 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5120_Send(void)
{
    // 탄산 추출 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5121 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5121 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5121_Send(void)
{
    // 탄산 추출 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5122 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5122 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5122_Send(void)
{
    // 냉매 전환 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5123 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5123 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5123_Send(void)
{
    // 냉매 전환 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5124 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5124 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5124_Send(void)
{
    // 핫가스 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5125 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5125 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5125_Send(void)
{
    // 핫가스 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5126 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5126 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5126_Send(void)
{
    // 이온 교환 수지 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5127 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5127 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5127_Send(void)
{
    //  이온 교환 수지 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5128 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5128 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5128_Send(void)
{
    // 이노센스 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5129 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5129 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5129_Send(void)
{
    // 이노센스 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x512A Command에 대한 ACK 함수
/// @details    PTA 검사 0x512A Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_512A_Send(void)
{
    // 온수 탱크 오버플로우 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x512B Command에 대한 ACK 함수
/// @details    PTA 검사 0x512B Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_512B_Send(void)
{
    // 온수 탱크 오버플로우 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x512C Command에 대한 ACK 함수
/// @details    PTA 검사 0x512C Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_512C_Send(void)
{
    // 냉수 탱크 오버플로우 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x512D Command에 대한 ACK 함수
/// @details    PTA 검사 0x512D Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_512D_Send(void)
{
    // 냉수 탱크 오버플로우 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x512E Command에 대한 ACK 함수
/// @details    PTA 검사 0x512E Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_512E_Send(void)
{
    // 냉온정 추출 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x512F Command에 대한 ACK 함수
/// @details    PTA 검사 0x512F Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_512F_Send(void)
{
    // 냉온정 추출 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5130 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5130 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5130_Send(void)
{
    // 빙축수 입수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5131 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5131 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5131_Send(void)
{
    // 빙축수 입수 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5132 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5132 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5132_Send(void)
{
    // 가압수 입수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5133 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5133 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5133_Send(void)
{
    // 가압수 입수 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5134 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5134 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5134_Send(void)
{
    // 가압수 배수 밸브 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5135 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5135 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5135_Send(void)
{
    // 가압수 배수 밸브 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5200 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5200 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5200_Send(void)
{
    // 냉각팬 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5201 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5201 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5201_Send(void)
{
    // 냉각팬 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5202 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5202 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5202_Send(void)
{
    // 스테핑모터1 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5203 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5203 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5203_Send(void)
{
    // 스태핑모터1 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5204 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5204 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5204_Send(void)
{
    // ICE 제빙 모터 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5205 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5205 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5205_Send(void)
{
    // ICE 제빙 모터 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5206 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5206 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5206_Send(void)
{
    // ICE TRAY 모터 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5207 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5207 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5207_Send(void)
{
    // ICE TRAY 모터 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5208 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5208 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5208_Send(void)
{
    // ICE 도어 모터 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5209 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5209 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5209_Send(void)
{
    // ICE 도어 모터 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x520A Command에 대한 ACK 함수
/// @details    PTA 검사 0x520A Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_520A_Send(void)
{
    // ICE 추출 모터 정방향 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x520B Command에 대한 ACK 함수
/// @details    PTA 검사 0x520B Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_520B_Send(void)
{
    // ICE 추출 모터 정방향 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x520C Command에 대한 ACK 함수
/// @details    PTA 검사 0x520C Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_520C_Send(void)
{
    // ICE 추출 모터 역방향 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x520D Command에 대한 ACK 함수
/// @details    PTA 검사 0x520D Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_520D_Send(void)
{
    // ICE 추출 모터 역방향 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x520E Command에 대한 ACK 함수
/// @details    PTA 검사 0x520E Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_520E_Send(void)
{
    // 셀렉트바 모터 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x520F Command에 대한 ACK 함수
/// @details    PTA 검사 0x520F Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_520F_Send(void)
{
    // 셀렉트바 모터 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5210 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5210 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5210_Send(void)
{
    // 스윙바 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5211 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5211 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5211_Send(void)
{
    // 스윙바 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5212 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5212 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5212_Send(void)
{
    // 모터 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5213 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5213 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5213_Send(void)
{
    // 모터 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5214 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5214 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5214_Send(void)
{
    // 교반모터 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5215 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5215 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5215_Send(void)
{
    // 교반 모터 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5216 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5216 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5216_Send(void)
{
    // 모터 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5217 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5217 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5217_Send(void)
{
    // 모터 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5300 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5300 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5300_Send(void)
{
    TURN_ON_BOOST_OUT();        // 부스트 펌프 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5301 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5301 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5301_Send(void)
{
    TURN_OFF_BOOST_OUT();        // 부스트 펌프 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5302 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5302 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5302_Send(void)
{
    // 드레인 펌프 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5303 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5303 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5303_Send(void)
{
    // 드레인 펌프 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5304 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5304 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5304_Send(void)
{
    // 냉수 순환 펌프 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5305 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5305 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5305_Send(void)
{
    // 냉수 순환 펌프 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5306 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5306 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5306_Send(void)
{
    // 온수 추출 펌프 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x5307 Command에 대한 ACK 함수
/// @details    PTA 검사 0x5307 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_5307_Send(void)
{
    // 온수 추출 펌프 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7000 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7000 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7000_Send(void)
{
    TURN_ON_UV_1();         // UV LED ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7001 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7001 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7001_Send(void)
{
    TURN_OFF_UV_1();         // UV LED OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7002 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7002 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7002_Send(void)
{
    // LED2 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7003 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7003 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7003_Send(void)
{
    // LED2 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7004 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7004 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7004_Send(void)
{
    // LED3 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7005 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7005 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7005_Send(void)
{
    // LED3 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7006 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7006 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7006_Send(void)
{
    // LED4 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7007 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7007 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7007_Send(void)
{
    // LED4 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7008 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7008 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7008_Send(void)
{
    // LED5 ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x7009 Command에 대한 ACK 함수
/// @details    PTA 검사 0x7009 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_7009_Send(void)
{
    // LED5 OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x9900 Command에 대한 ACK 함수
/// @details    PTA 검사 0x9900 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_9900_Send(void)
{
    // COMP ON

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x9901 Command에 대한 ACK 함수
/// @details    PTA 검사 0x9901 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_9901_Send(void)
{
    // COMP OFF

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x9902 Command에 대한 ACK 함수
/// @details    PTA 검사 0x9902 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_9902_Send(void)
{
    // 제빙 시스템(냉각)

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x9903 Command에 대한 ACK 함수
/// @details    PTA 검사 0x9903 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_9903_Send(void)
{
    // 제빙 시스템(제빙)

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0x9904 Command에 대한 ACK 함수
/// @details    PTA 검사 0x9904 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_9904_Send(void)
{
    // COMP 동작(제빙)

    PTA_ACK_PowerTest();
}


/// @brief      PTA 검사 0xFF00 Command에 대한 ACK 함수
/// @details    PTA 검사 0xFF00 Command에 대한 ACK를 송신한다
/// @param      void
/// @return     void
void PTA_ACK_CMD_FF00_Send(void)
{
    gu8PTA_TestEnd = SET;           // PTA 검사 완료

    NoPowerSet();                   // 모든 부하 OFF

    ChangeOperationMode(OPERATION_MODE_ID_PTA_TEST_3);      // 기밀 모드로 진입
}


/// @brief      PTA 검사 완료 후 기밀 검사 모드(PTA 모드3) 동작 함수
/// @details    PTA 검사 완료 후 기밀 검사 모드(PTA 모드3) 동작을 한다
/// @param      void
/// @return     void
void PTA_Mode3_Act(void)
{
    U8 mu8OperationMode = 0;

    mu8OperationMode = Get_OperationMode();

    if (mu8OperationMode == OPERATION_MODE_ID_PTA_TEST_3)
    {	// PTA 모드3 동작시 동작 부하 제어 추가
//        TURN_ON_DC_FAN_1();         // 냉각 Fan ON
//        TURN_ON_VALVE_1();          // 입수 Feed Valve ON
    }
}



#endif

// Lib Water Error Module *********************************************************************************

/// @brief      Lib PTA Mode Module Initilaize
/// @details    정수기 공장 자동화 검사 모드 관련 데이터들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Lib_PTA_Mode_Module_Initialize(void)
{
#if (PTA_TEST_MODE_USE == USE)
    Lib_PTA_Mode_Initialize();
#endif
}


/// @brief      Lib PTA Mode Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 PTA 검사 제어 모듈 Library
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
/// @details    Main 함수 안의 While Loop 안에 추가될 PTA 검사 제어 모듈 Library
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



