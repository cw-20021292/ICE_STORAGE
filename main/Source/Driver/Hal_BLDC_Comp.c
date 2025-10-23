/// @file     Hal_BLDC_Comp.c
/// @date     2025/04/11
/// @author   Jaejin Ham
/// @brief    BLDC Comp ���� ���� file

#include "Global_Header.h"

#if (BLDC_COMP_USE == USE)

U8 gu8BLDCComp_ControlTimer = 0;

#if (BLDC_COMP_PWM_CONTROL_USE == USE)
// PWM ���� ��� Comp
U16 gu16CompOnControlTimer = 0;                         /// @brief  Comp On ���� Ÿ�̸�
U16 gu16CompOnControlRPM = 0;                           /// @brief  Comp On ���� RPM
U8 gu8CompOnControlHz = 0;                              /// @brief  Comp On ���� Hz
U16 gu16CompOffControlTimer = 0;                        /// @brief  Comp Off ���� Ÿ�̸�
#else
// UART ��� ��� Comp
U8 au8BLDC_Comp_TxData[BLDC_COMP_TX_DATA_COUNT];        /// @brief  BLDC Comp�� UART��� TXD ������ ����
U8 au8BLDC_Comp_RxData[BLDC_COMP_RX_DATA_COUNT];        /// @brief  BLDC Comp�� UART��� RXD ������ ����

U8 gu8BLDC_Comp_Tx_Timer = 0;                           /// @brief  BLDC Comp�� UART��� Tx �ֱ� Ÿ�̸�
U8 gu8BLDC_Comp_Tx_Count = 0;                           /// @brief  BLDC Comp�� UART��� Tx Data Byte Count
U8 gu8BLDC_Comp_Target_Hz = 0;                          /// @brief  BLDC Comp�� ���� ��û Hz
U8 gu8BLDC_Comp_Tx_Request = 0;                         /// @brief  BLDC Comp�� UART��� Tx ���� ����

U8 gu8BLDC_Comp_Rx_Receive_Count = 0;                   /// @brief  BLDC Comp�� UART��� Rx Data Byte Count
U8 gu8BLDC_Comp_Rx_Wait_TimeOut = 0;                    /// @brief  BLDC Comp�� Rx ��� Time out
U8 gu8BLDC_Comp_Rx_Complete = 0;                        /// @brief  BLDC Comp�� Rx �Ϸ� ����
U8 gu8BLDC_Comp_Check_Error = 0;                        /// @brief  BLDC Comp�� �߻��� ����
U8 gu8BLDC_Comp_ErrorCode = 0;                          /// @brief  BLDC Comp�� ���� ���� �ڵ�
U8 gu8BLDC_Comp_ErrorCodeBuffer = 0;                    /// @brief  BLDC Comp�� ���� ���� �ڵ� ����
U8 gu8BLDC_Comp_Opration_Hz = 0;                        /// @brief  BLDC Comp�� ���� ���� �ǵ�� Hz
U8 gu8BLDC_Comp_Temperature = 0;                        /// @brief  BLDC Comp�� ���� ���� Comp�� �µ� ���°�

U16 gu16BLDC_Comp_Comm_Error_Timer = 0;                 /// @brief  ��� ���� �ҷ� ���� �ð�
U8 gu8BLDC_Comp_Comm_Error_Count = 0;                   /// @brief  ��� ���� �ҷ� ���� ī��Ʈ
U8 gu8BLDC_Comp_Comm_Error_Detect = 0;                  /// @brief  ��� ���� �ҷ� ���� ����
U16 gu16BLDC_Comp_Error_Check_Timer = 0;                /// @brief  Comp ���� ���� Ÿ�̸�

U8 gu8BLDC_Comp_Self_Count = 0;                         /// @brief  ���� �ڵ� ���� Ƚ��
U8 gu8BLDC_Comp_ErrorCodeDetect = 0;                    /// @brief  ���� �ڵ� �������� ���� ���� ���� ����
U8 gu8BLDC_Comp_ErrorCodeDetectCount = 0;               /// @brief  ���� �ڵ� �������� ���� ���� ���� �ݺ� Ƚ��
U16 gu16BLDC_Comp_ErrorCountClearTime = 0;              /// @brief  ���� �ڵ� ī��Ʈ �ʱ�ȭ ���� �ð�

U8 gu8BLDC_Comp_Comm_Error_Check_End = 0;               /// @brief  ���� ��� ���� �߻� üũ �Ϸ� ����
U8 gu8BLDC_Comp_Ooeration_ErrorCode_Check_End = 0;      /// @brief  ���� �߻��� Comp Error Code üũ �Ϸ� ����


/// @brief      BLDC Comp ���� ���� Hz ���̺�
/// @details    BLDC Comp ���� ���� Hz�� ���̺�� ������ ���´�
__far const U8 au8BLDC_FrequencyData[39] =
{
    BLDC_37Hz, BLDC_38Hz, BLDC_39Hz, BLDC_40Hz, BLDC_41Hz,
    BLDC_42Hz, BLDC_43Hz, BLDC_44Hz, BLDC_45Hz, BLDC_46Hz,
    BLDC_47Hz, BLDC_48Hz, BLDC_49Hz, BLDC_50Hz, BLDC_51Hz,
    BLDC_52Hz, BLDC_53Hz, BLDC_54Hz, BLDC_55Hz, BLDC_56Hz,
    BLDC_57Hz, BLDC_58Hz, BLDC_59Hz, BLDC_60Hz, BLDC_61Hz,
    BLDC_62Hz, BLDC_63Hz, BLDC_64Hz, BLDC_65Hz, BLDC_66Hz,
    BLDC_67Hz, BLDC_68Hz, BLDC_69Hz, BLDC_70Hz, BLDC_71Hz,
    BLDC_72Hz, BLDC_73Hz, BLDC_74Hz, BLDC_75Hz
};
#endif


/// @brief      BLDC Comp ���� ���� �ʱ�ȭ
/// @details    BLDC Comp ���۰� ���õ� ������ ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Initialize(void)
{
    gu8BLDCComp_ControlTimer = 0;

#if (BLDC_COMP_PWM_CONTROL_USE == USE)
    // PWM ���� ��� Comp
    gu16CompOnControlTimer = 0;                 // Comp On ���� Ÿ�̸�
    gu16CompOnControlRPM = 0;                   // Comp On ���� RPM
    gu8CompOnControlHz = 0;                     // Comp On ���� Hz
    gu16CompOffControlTimer = 0;                // Comp Off ���� Ÿ�̸�
#else
    // UART ��� ��� Comp
    MEMSET(au8BLDC_Comp_TxData, 0x00, sizeof(au8BLDC_Comp_TxData));
    MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));

    gu8BLDC_Comp_Tx_Timer = 0;
    gu8BLDC_Comp_Tx_Count = 0;
    gu8BLDC_Comp_Target_Hz = 0;
    gu8BLDC_Comp_Tx_Request = 0;

    gu8BLDC_Comp_Rx_Receive_Count = 0;
    gu8BLDC_Comp_Rx_Wait_TimeOut = 0;
    gu8BLDC_Comp_Rx_Complete = 0;
    gu8BLDC_Comp_Check_Error = 0;
    gu8BLDC_Comp_ErrorCode = 0;
    gu8BLDC_Comp_ErrorCodeBuffer = 0;
    gu8BLDC_Comp_Opration_Hz = 0;
    gu8BLDC_Comp_Temperature = 0;

    gu16BLDC_Comp_Comm_Error_Timer = 0;         // ��� ���� �ҷ� ���� �ð�
    gu8BLDC_Comp_Comm_Error_Count = 0;          // ��� ���� �ҷ� ���� ī��Ʈ
    gu8BLDC_Comp_Comm_Error_Detect = 0;         // ��� ���� �ҷ� ���� ����
    gu16BLDC_Comp_Error_Check_Timer = 0;        // Comp ���� ���� Ÿ�̸�

    gu8BLDC_Comp_Self_Count = 0;                // ���� �ڵ� ���� Ƚ��
    gu8BLDC_Comp_ErrorCodeDetect = 0;           // ���� �ڵ� �������� ���� ���� ���� ����
    gu8BLDC_Comp_ErrorCodeDetectCount = 0;      // ���� �ڵ� �������� ���� ���� ���� �ݺ� Ƚ��
    gu16BLDC_Comp_ErrorCountClearTime = 0;      // ���� �ڵ� ī��Ʈ �ʱ�ȭ ���� �ð�

    gu8BLDC_Comp_Comm_Error_Check_End = 0;               // ���� ��� ���� �߻� üũ �Ϸ� ����
    gu8BLDC_Comp_Ooeration_ErrorCode_Check_End = 0;      // ���� �߻��� Comp Error Code üũ �Ϸ� ����
#endif
}


/// @brief      BLDC �������� ���� �ð� ī����(@1ms interrupt)
/// @details    BLDC Comp�� ���� �ð� ���� �����ϱ� ���� ���� �ð��� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void BLDC_Comp_ControlTimer(void)
{
    if (gu8BLDCComp_ControlTimer < BLDC_COMP_CONTROL_TIME_PERIOD)
    {
        gu8BLDCComp_ControlTimer++;
    }
}


/// @brief      BLDC �������� ���� �Լ�(@While Loop)
/// @details    While�� �ȿ��� BLDC Comp ���� ������ �Ѵ�
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Control(void)
{
    if (gu8BLDCComp_ControlTimer >= BLDC_COMP_CONTROL_TIME_PERIOD)
    {   // 100ms
        gu8BLDCComp_ControlTimer = 0;

#if (BLDC_COMP_PWM_CONTROL_USE == USE)
        BLDC_Comp_OnOff_Control();
#else
        BLDC_Comp_Communication();
#endif
    }
}


/// @brief      BLDC �������� ���� ���� Ȯ�� �Լ�
/// @details    BLDC Comp ���� ���¸� Ȯ���Ͽ� ��ȯ �Ѵ�
/// @param      void
/// @return     return : Comp ���� ���� - 0(OFF), 1(ON)
U8 Get_BLDC_Comp_Status(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_BLDC_COMP();

    if (mu8Status)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


#if (BLDC_COMP_PWM_CONTROL_USE == USE)        // PWM ���� ��� Comp ����

/// @brief      BLDC �������� ���� RPM ���� �Լ�
/// @details    BLDC Comp ��� �� �� RPM ��� ���� ���� �����Ѵ�
/// @param      mu16RPM : ���� RPM��
/// @return     void
void Set_BLDC_Comp_RPM(U16 mu16RPM)
{
    gu16CompOnControlRPM = mu16RPM;
}


/// @brief      BLDC �������� ON/OFF ���� �Լ�(100ms)
/// @details    BLDC Comp ������ �����Ѵ�
/// @param      void
/// @return     void
void BLDC_Comp_OnOff_Control(void)
{
    if (gu16CompOnControlRPM)
    {
        gu16CompOffControlTimer = 0;

        BLDC_Comp_On_Control(gu16CompOnControlRPM);
    }
    else
    {
        gu16CompOnControlTimer = 0;

        BLDC_Comp_Off_Control();
    }
}


/// @brief      BLDC �������� ON ���� �Լ�(100ms)
/// @details    BLDC Comp�� ���� ���� ���¿��� ������ RPM ������ �����Ѵ�
/// @param      mu16RPM : �����ϰ��� �ϴ� Comp RPM
/// @return     void
void BLDC_Comp_On_Control(U16 mu16RPM)
{
    TURN_ON_BLDC_COMP();

    if (gu16CompOnControlTimer >= BLDC_COMP_ON_CONTROL_TIME)
    {   // Comp On �� 3�� ���
        gu8CompOnControlHz = (U8)( (mu16RPM / 60) * 2);
        BLDC_Comp_Hz_Control(gu8CompOnControlHz);
    }
    else
    {
        gu16CompOnControlTimer++;
    }
}


/// @brief      BLDC �������� OFF ���� �Լ�(100ms)
/// @details    BLDC Comp�� 0Hz���� ������ ���� ���� ������ OFF ��Ų��
/// @param      void
/// @return     void
void BLDC_Comp_Off_Control(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_BLDC_COMP();

    if (mu8Status)
    {   // Comp�� ���� ����
        gu8CompOnControlHz = 0;
        BLDC_Comp_Hz_Control(gu8CompOnControlHz);      // 0 Hz�� PWM Stop

        if (gu16CompOffControlTimer >= BLDC_COMP_OFF_CONTROL_TIME)
        {   // 30�� ��� ��
            TURN_OFF_BLDC_COMP();
        }
        else
        {
            gu16CompOffControlTimer++;
        }
    }
    else
    {   // Comp�� ���� ����
        gu8CompOnControlHz = 0;
        BLDC_Comp_Hz_Control(gu8CompOnControlHz);      // 0 Hz�� PWM Stop
    }
}


/// @brief      BLDC �������� Hz ���� �Լ�(100ms)
/// @details    BLDC Comp�� ������ Hz������ �����Ѵ�
/// @param      mu8Hz : �����ϰ��� �ϴ� Comp Hz
/// @return     void
void BLDC_Comp_Hz_Control(U8 mu8Hz)
{
    if (mu8Hz < 44)
    {   // Hz�� 44Hz �̸��� ��� 44�� ����
        mu8Hz = 44;
    }
    else if ( mu8Hz > 150)
    {   // Hz�� 150Hz �ʰ��� ��� 150���� ����
        mu8Hz = 150;
    }

    if (mu8Hz)
    {   // ���� Hz�� 0�� �ƴϸ�
        // 100Hz => TDR11 = 19999
        BLDC_COMP_PWM_REGISTER = (U16)((((D64)(100) / mu8Hz) * 20000) - 1);

        if (BLDC_COMP_PWM_INTERRUPT_REGISTER)
        {   // INTTM11 interrupt�� disable �����̸�
            BLDC_COMP_PWM_START();
        }
    }
    else
    {   // ���� Hz�� 0�̸�
        BLDC_COMP_PWM_REGISTER = 0;

        if (!BLDC_COMP_PWM_INTERRUPT_REGISTER)
        {   // INTTM11 interrupt�� enable �����̸�
            BLDC_COMP_PWM_STOP();
        }
    }
}


#else       // UART ��� ��� Comp ����

/// @brief      BLDC �������� ���� Hz ���� �Լ�
/// @details    BLDC Comp ���� ��ų Hz�� �����Ѵ�
/// @param      mu8Hz : �����ϰ��� �ϴ� Comp Hz
/// @return     void
void Set_BLDC_Comp_Hz(U8 mu8Hz)
{
    gu8BLDC_Comp_Target_Hz = mu8Hz;
}


/// @brief      BLDC �������� ���� ���� Hz Ȯ�� �Լ�
/// @details    BLDC Comp�� ���� ���� Hz�� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     mu8Return : ���� ���� ���� Comp�� Hz
U8 Get_BLDC_Comp_Hz(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8BLDC_Comp_Opration_Hz;

    return  mu8Return;
}


/// @brief      BLDC �������� ���� �߿� �߻��� ������ ���� �Լ�
/// @details    BLDC Comp���� �߻��� ������ ����� ���´�
/// @param      mu8Err : ���� üũ�� Error
/// @return     void
void Set_BLDC_Comp_Error(U8 mu8Err)
{
    gu8BLDC_Comp_Check_Error = mu8Err;
}


/// @brief      BLDC �������� ���� �߿� �߻��� ���� Ȯ�� �Լ�
/// @details    BLDC Comp ���� �� �߻��� ������ Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     mu8Return : ���� ���� ���� Comp�� �߻��� ���� �ڵ�
U8 Get_BLDC_Comp_Error(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8BLDC_Comp_Check_Error;

    return  mu8Return;
}


/// @brief      BLDC �������� ���� �߿� �߻��� ���� �ڵ� Ȯ�� �Լ�
/// @details    BLDC Comp ���� �� ������ ���� �ڵ带 Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     mu8Return : ���� ���� ���� Comp�� �߻��� ���� �ڵ�
U8 Get_BLDC_Comp_ErrorCode(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8BLDC_Comp_ErrorCode;

    return  mu8Return;
}


/// @brief      BLDC �������� ���� �� Comp �µ� Ȯ�� �Լ�
/// @details    BLDC Comp ���� �� ������ Comp �µ� ���� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     mu8Return : ���� ���� ���� Comp�� �µ�
U8 Get_BLDC_Comp_Temp(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8BLDC_Comp_Temperature;

    return  mu8Return;
}


/// @brief      BLDC �������� ���� �� ��� ���� ���� ���� Ȯ�� �Լ�
/// @details    BLDC Comp ���� �� ��� ���� ���� ���¸� Ȯ���Ͽ� ��ȯ �Ѵ�
/// @param      void
/// @return     return : ��� ���� ���� ���� ���� - 0(�̰���), 1(����)
U8 Get_BLDC_Comp_Comm_ErrorDetect(void)
{
    if (gu8BLDC_Comp_Comm_Error_Detect)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      BLDC �������� ���� �� ���� �ڵ� ���� ���� Ȯ�� �Լ�
/// @details    BLDC Comp ���� �� ���� �ڵ� ���� ���¸� Ȯ���Ͽ� ��ȯ �Ѵ�
/// @param      void
/// @return     return : ���� �ڵ� ���� ���� - 0(�̼���), 1(����)
U8 Get_BLDC_Comp_ErrorDetect(void)
{
    if (gu8BLDC_Comp_ErrorCodeDetect)
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      BLDC �������� ���� �Լ�(100ms)
/// @details    BLDC Comp�� UART ����� �����Ѵ�
/// @param      void
/// @return     void
void BLDC_Comp_Communication(void)
{
    U8 mu8CompOperationHzBuff = 0;
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_BLDC_COMP();

    // Comp ���� ����
    if (mu8Status)
    {   // Comp ON ����
        if (gu8BLDC_Comp_Target_Hz == 0)
        {   // ���� Hz�� 0�� ���
            mu8CompOperationHzBuff = Get_BLDC_Comp_Hz();

            if (mu8CompOperationHzBuff == 0)
            {   // �������� Hz�� 0�� ���
                TURN_OFF_BLDC_COMP();
            }
            else
            {
                TURN_ON_BLDC_COMP();
            }
        }
        else
        {
            TURN_ON_BLDC_COMP();
        }
    }
    else
    {   // Comp ���� OFF ����
        if (gu8BLDC_Comp_Target_Hz != 0)
        {   // ���� Hz�� 0�� �ƴ� ���
            TURN_ON_BLDC_COMP();
        }
        else
        {
            TURN_OFF_BLDC_COMP();
        }
    }

    // Comp ��� ����
    if (mu8Status)
    {   // Comp ���� ON �� ���
        BLDC_Comp_Comm_Error_Check();

        BLDC_Comp_Rx_Communication();
        BLDC_Comp_Tx_Communication();
    }
    else
    {   // Comp ���� OFF�� ���
        Clear_BLDC_Comp_Data();
    }
}


/// @brief      BLDC Comp RXD ó�� �Լ�
/// @details    BLDC Comp�� UART ��Ű� ���� �����͸� ó���Ѵ�
/// @param      void
/// @return     void
void BLDC_Comp_Rx_Communication(void)
{
    U8 mu8Cal_CRC = 0;

    if (gu8BLDC_Comp_Rx_Complete == SET)
    {   // ���� �Ϸ�� �����Ͱ� �ִ� ���
        mu8Cal_CRC = BLDC_Comp_Rx_CRC(au8BLDC_Comp_RxData);

        if (au8BLDC_Comp_RxData[BLDC_COMP_RX_DATA_CHECK_SUM] == mu8Cal_CRC)
        {
            gu8BLDC_Comp_Tx_Request = SET;                      // Tx Data �۽� ����
            Receive_BLDC_Comp_Data(au8BLDC_Comp_RxData);        // ������ ������ ó��

            gu16BLDC_Comp_Comm_Error_Timer = 0;     // ��� ���� üũ Ÿ�̸� Clear
            gu8BLDC_Comp_Comm_Error_Count = 0;      // ��� ���� üũ ī��Ʈ Clear
            gu8BLDC_Comp_Comm_Error_Detect = 0;     // ��� ���� �ҷ� ���� ���� Clear
        }
        else
        {
        }

        gu8BLDC_Comp_Rx_Complete = CLEAR;       // ���� ������ ó�� �Ϸ�
        MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));
    }
    else
    {   // ���� �Ϸ�� �����Ͱ� ���� ���

    }
}


/// @brief      BLDC Comp TXD ó�� �Լ�
/// @details    BLDC Comp�� UART ��Ű� �۽� �����͸� ó���Ѵ�
/// @param      void
/// @return     void
void BLDC_Comp_Tx_Communication(void)
{
    gu8BLDC_Comp_Tx_Timer++;

    if (gu8BLDC_Comp_Tx_Timer >= BLDC_COMP_UART_TIME_PERIOD)
    {   // Polling �ֱ� ����
        gu8BLDC_Comp_Tx_Timer = 0;

        BLDC_Comp_UART_Tx_Start();
    }
}


/// @brief      BLDC Comp RX Data ó�� �Լ�
/// @details    BLDC Comp ���� ������ Rx �����͸� ó���Ѵ�
/// @param      pu8RxData : ������ Rx Data �迭
/// @return     void
void Receive_BLDC_Comp_Data(U8* pu8RxData)
{
#if (BLDC_COMP_TYPE == LG_BLDC_COMP)
    gu8BLDC_Comp_ErrorCodeBuffer = pu8RxData[BLDC_COMP_RX_DATA_ERROR_CODE];
    gu8BLDC_Comp_Opration_Hz = pu8RxData[BLDC_COMP_RX_DATA_OPERATION_FREQUENCY];
    gu8BLDC_Comp_Temperature = 0;
#else
    gu8BLDC_Comp_ErrorCodeBuffer = pu8RxData[BLDC_COMP_RX_DATA_ERROR_CODE];
    gu8BLDC_Comp_Opration_Hz = pu8RxData[BLDC_COMP_RX_DATA_OPERATION_FREQUENCY];
    gu8BLDC_Comp_Temperature = pu8RxData[BLDC_COMP_RX_DATA_TEMPERATURE];
#endif

    if (gu16BLDC_Comp_Error_Check_Timer >= BLDC_COMP_ERROR_CODE_CHECK_DELAY_TIME)
    {   // ����� �����ϰ� 1�� ��� �ĺ��� ���� �ڵ� üũ ����
        if (gu8BLDC_Comp_ErrorCodeBuffer != 0)
        {   // ���� �ڵ尡 ���ŵ� ���
            if (gu8BLDC_Comp_ErrorCode != gu8BLDC_Comp_ErrorCodeBuffer)
            {   // ���� ���� ���� �ڵ�� �ٸ� ���
                gu8BLDC_Comp_Self_Count = 0;
                gu8BLDC_Comp_ErrorCode = gu8BLDC_Comp_ErrorCodeBuffer;
            }
            else
            {   // ���� ���� ���� �ڵ�� ���� ���
                if (gu8BLDC_Comp_Self_Count >= BLDC_COMP_ERROR_CODE_RECIEVE_COUNT)
                {   // ���� ���� �ڵ带 ���� 10ȸ �̻� ������ ���
                    if (!gu8BLDC_Comp_ErrorCodeDetect)
                    {   // ���� �̰��� �����̸�
                        gu8BLDC_Comp_ErrorCodeDetect = SET;     // BLDC Comp ���� ���� ���� ����

                        if (gu8BLDC_Comp_ErrorCodeDetectCount >= BLDC_COMP_ERROR_CODE_RECIEVE_CONFIRM_COUNT)
                        {   // ���� ���� ���� �ݺ��� ���� Ƚ�� �̻��̸�
                            BLDC_Comp_Error_Confirm(gu8BLDC_Comp_ErrorCode);    // ���� �ڵ� Ȯ��
                        }
                        else
                        {
                            gu8BLDC_Comp_ErrorCodeDetectCount++;
                        }
                    }
                }
                else
                {
                    gu8BLDC_Comp_Self_Count++;
                }
            }
        }
        else
        {   // ���� �ڵ� �̼��� ��
            gu8BLDC_Comp_Self_Count = 0;    // ���� �ݺ� ���� ī��Ʈ �ʱ�ȭ
            gu8BLDC_Comp_ErrorCode = 0;

            if (gu8BLDC_Comp_ErrorCodeDetectCount)
            {   // ���� �ڵ� ���� ���� �ݺ� ī��Ʈ ���� �ִ� ���
                if (gu16BLDC_Comp_ErrorCountClearTime >= BLDC_COMP_ERROR_CODE_COUNT_AUTO_CLEAR_TIME)
                {   // ���� �ð� ���� ���� �ڵ� �̼����̸�
                    gu16BLDC_Comp_ErrorCountClearTime = 0;
                    gu8BLDC_Comp_ErrorCodeDetectCount = 0;  // ���� �ݺ� ���� ī��Ʈ �ʱ�ȭ
                }
                else
                {
                    gu16BLDC_Comp_ErrorCountClearTime++;
                }
            }
            else
            {
                gu16BLDC_Comp_ErrorCountClearTime = 0;
            }
        }
    }
    else
    {   // ����� �����ϰ� 1�� ��� ��
        gu8BLDC_Comp_Self_Count = 0;    // ���� �ݺ� ���� ī��Ʈ �ʱ�ȭ
        gu8BLDC_Comp_ErrorCode = 0;
        gu16BLDC_Comp_ErrorCountClearTime = 0;
    }
}


/// @brief      BLDC Comp RX Error Code Confirm �Լ�
/// @details    BLDC Comp �� UART ������� ������ ���� �ڵ带 Ȯ�� ó���Ѵ�
/// @param      mu8ErrorCode : ������ ���� �ڵ�
/// @return     void
void BLDC_Comp_Error_Confirm(U8 mu8ErrorCode)
{
    if (gu8BLDC_Comp_Ooeration_ErrorCode_Check_End == CLEAR)
    {       // �� �ֱ� �ѹ��� üũ �ǵ��� �ϱ� ���� ���ǹ�
        gu8BLDC_Comp_Ooeration_ErrorCode_Check_End  = SET;

        switch (mu8ErrorCode)
        {
#if (BLDC_COMP_TYPE == LG_BLDC_COMP)            // LG BLDC Comp
            case LG_BLDC_COMP_E81_CURRENT_SENSING_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_81);
                break;

            case LG_BLDC_COMP_E82_POSITION_SENSING_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_82);
                break;

            case LG_BLDC_COMP_E83_OVER_CURRENT_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_83);
                break;

            case LG_BLDC_COMP_E84_IPM_FAULT_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_84);
                break;

            case LG_BLDC_COMP_E85_DISCONNECTION_COMM_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_85);
                break;

            case LG_BLDC_COMP_E86_OVERLOAD_PROTECTION_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_86);
                break;
#else
            case SAMSUNG_BLDC_COMP_E81_CURRENT_SENSING_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_81);
                break;

            case SAMSUNG_BLDC_COMP_E82_STARTING_FAIL_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_82);
                break;

            case SAMSUNG_BLDC_COMP_E83_OVER_CURRENT_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_83);
                break;

            case SAMSUNG_BLDC_COMP_E84_OVERHEAT_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_84);
                break;

            case SAMSUNG_BLDC_COMP_E85_DISCONNECTION_COMM_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_85);
                break;

            case SAMSUNG_BLDC_COMP_E86_ABNORMAL_VOLTAGE_ERROR:
                Set_BLDC_Comp_Error(BLDC_COMP_ERROR_86);
                break;
#endif
            default:
                break;
        }
    }
    else
    {
    }
}

/// @brief      BLDC Comp Uart Rxd Interrupt ó�� �Լ�(@UART Rxd Interrupt)
/// @details    BLDC Comp�� UART ��Ž� Rx Interrupt�� �߻����� �� ���� �����͸� ó���Ѵ�
/// @param      void
/// @return     void
void UART_ISR_BLDC_Comp_Rx(void)
{
    U8 mu8RxBuff = 0;

    mu8RxBuff = BLDC_COMP_UART_RXD;

    if ( (gu8BLDC_Comp_Rx_Complete == CLEAR) && (gu8BLDC_Comp_Tx_Request == CLEAR) )
    {
        au8BLDC_Comp_RxData[gu8BLDC_Comp_Rx_Receive_Count] = mu8RxBuff;

        if (gu8BLDC_Comp_Rx_Receive_Count >= (U8)(BLDC_COMP_RX_DATA_COUNT - 1))
        {
            gu8BLDC_Comp_Rx_Receive_Count = 0;
            gu8BLDC_Comp_Rx_Wait_TimeOut = 0;
            gu8BLDC_Comp_Rx_Complete = SET;     // ������ ��Ŷ ���� �Ϸ�
        }
        else
        {
            gu8BLDC_Comp_Rx_Receive_Count++;
            gu8BLDC_Comp_Rx_Wait_TimeOut = 5;      // �����Ͱ� ���̿� 500ms RX ��� Time out
        }
    }
    else
    {
        gu8BLDC_Comp_Rx_Receive_Count = 0;
        MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));
    }
}


/// @brief      BLDC Comp Uart Tx ���� �Լ�
/// @details    BLDC Comp�� UART ��Ű� TXD�� ���� ó���Ѵ�
/// @param      void
/// @return     void
void BLDC_Comp_UART_Tx_Start(void)
{
    if (gu8BLDC_Comp_Tx_Request == SET)
    {
        // Tx �����͸� �۽��ϱ� �� Rxd Data ��� �ʱ�ȭ
        gu8BLDC_Comp_Rx_Receive_Count = 0;
        gu8BLDC_Comp_Rx_Wait_TimeOut = 0;
        MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));

        Make_BLDC_Comp_Tx_Packet();
        BLDC_COMP_UART_TXD = au8BLDC_Comp_TxData[0];
        gu8BLDC_Comp_Tx_Count = 1;
    }
    else
    {
        MEMSET(au8BLDC_Comp_TxData, 0x00, sizeof(au8BLDC_Comp_TxData));
    }
}


/// @brief      BLDC Comp Uart Tx Packet ���� �Լ�
/// @details    BLDC Comp�� ����ϴ� Tx ������ Packet�� �����Ѵ�
/// @param      void
/// @return     void
void Make_BLDC_Comp_Tx_Packet(void)
{
    U8 mu8_crc = 0;

    au8BLDC_Comp_TxData[0] = au8BLDC_FrequencyData[gu8BLDC_Comp_Target_Hz - 37];;

    mu8_crc = BLDC_Comp_Tx_CRC(au8BLDC_Comp_TxData);

    au8BLDC_Comp_TxData[1] = mu8_crc;
}


/// @brief      BLDC Comp Uart Txd Interrupt ó�� �Լ�(@UART Txd Interrupt)
/// @details    BLDC Comp�� URAT ��Ű� Tx Interrupt ������ ó���Ѵ�
/// @param      void
/// @return     void
void UART_ISR_BLDC_Comp_Tx(void)
{
    if (gu8BLDC_Comp_Tx_Count < BLDC_COMP_TX_DATA_COUNT)
    {
        BLDC_COMP_UART_TXD = au8BLDC_Comp_TxData[gu8BLDC_Comp_Tx_Count];
        gu8BLDC_Comp_Tx_Count++;
    }
    else
    {
        gu8BLDC_Comp_Tx_Request = CLEAR;
        gu8BLDC_Comp_Tx_Count = 0;
        MEMSET(au8BLDC_Comp_TxData, 0x00, sizeof(au8BLDC_Comp_TxData));
    }
}


/// @brief      BLDC Comp Uart Txd Data CheckSum Calculate
/// @details    BLDC Comp�� UART ��Ű� �۽� �������� üũ���� ����Ѵ�
/// @param      pu8Msg : üũ���� ����� ������ �迭 
/// @return     return : üũ�� ��� ������
U8 BLDC_Comp_Tx_CRC(U8* pu8Msg)
{
    U8 mu8CRC = 0;

    mu8CRC = pu8Msg[0] ^ BLDC_COMP_CHECK_CODE;

    return mu8CRC;
}


/// @brief      BLDC Comp Uart Rxd Data CheckSum Calculate
/// @details    BLDC Comp �� UART ��Ű� ���� �������� üũ���� ����Ѵ�
/// @param      pu8Msg : üũ���� ����� ������ �迭 
/// @return     return : üũ�� ��� ������
U8 BLDC_Comp_Rx_CRC(U8* pu8Msg)
{
    U8 mu8CRC = 0;

    mu8CRC = (pu8Msg[0] + pu8Msg[1] + pu8Msg[2]) ^ BLDC_COMP_CHECK_CODE;

    return mu8CRC;
}


/// @brief      BLDC Comp Data Clear
/// @details    BLDC Comp ����� ���õ� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Clear_BLDC_Comp_Data(void)
{
    MEMSET(au8BLDC_Comp_TxData, 0x00, sizeof(au8BLDC_Comp_TxData));
    MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));

    gu8BLDC_Comp_Tx_Timer = 0;
    gu8BLDC_Comp_Tx_Count = 0;
    gu8BLDC_Comp_Target_Hz = 0;
    gu8BLDC_Comp_Tx_Request = 0;

    gu8BLDC_Comp_Rx_Receive_Count = 0;
    gu8BLDC_Comp_Rx_Wait_TimeOut = 0;
    gu8BLDC_Comp_Rx_Complete = 0;
    gu8BLDC_Comp_Check_Error = 0;
    gu8BLDC_Comp_ErrorCode = 0;
    gu8BLDC_Comp_ErrorCodeBuffer = 0;
    gu8BLDC_Comp_Opration_Hz = 0;
    gu8BLDC_Comp_Temperature = 0;

    gu16BLDC_Comp_Comm_Error_Timer = 0;         // ��� ���� �ҷ� ���� �ð�
    gu16BLDC_Comp_Error_Check_Timer = 0;        // Comp ���� ���� Ÿ�̸�
    gu8BLDC_Comp_Comm_Error_Detect = 0;         // ��� ���� �ҷ� ���� ����

    gu8BLDC_Comp_Self_Count = 0;                // ���� �ڵ� ���� Ƚ��
    gu8BLDC_Comp_ErrorCodeDetect = 0;          // ���� �ڵ� �������� ���� ���� ���� ����

    gu8BLDC_Comp_Comm_Error_Check_End = 0;               // ���� ��� ���� �߻� üũ �Ϸ� ����
    gu8BLDC_Comp_Ooeration_ErrorCode_Check_End = 0;      // ���� �߻��� Comp Error Code üũ �Ϸ� ����
}


/// @brief      BLDC Comp Error ��� ���� üũ �Լ�
/// @details    BLDC Comp ��� ���� üũ�� �Ѵ�
/// @param      void
/// @return     void
void BLDC_Comp_Comm_Error_Check(void)
{
    if (gu8BLDC_Comp_Rx_Wait_TimeOut)
    {   // Rx ��Ŷ�� �߰��� ��������  ������ ����� ���ŵ��� �ʴ� ���
        gu8BLDC_Comp_Rx_Wait_TimeOut--;

        if (gu8BLDC_Comp_Rx_Wait_TimeOut == 0)
        {
            gu8BLDC_Comp_Rx_Receive_Count = 0;
            MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));
        }
    }

    if (gu8BLDC_Comp_Rx_Complete == SET)
    {   // ���� �Ϸ�� �����Ͱ� �ִ� ���
        // Error Check Timer�� ���� ������ ���� ���� Ȯ�� �� �� Clear ó����
    }
    else
    {
        if (gu16BLDC_Comp_Comm_Error_Timer >= BLDC_COMP_RXD_COMMUNICATION_ERROR_TIME)
        {   // 30�� ���� ����� ���� ���
            if (gu8BLDC_Comp_Comm_Error_Check_End == CLEAR)
            {
                gu8BLDC_Comp_Comm_Error_Check_End = SET;

                if (gu8BLDC_Comp_Comm_Error_Detect == CLEAR)
                {   // ��� ���� �̰��� ������ ���
                    gu8BLDC_Comp_Comm_Error_Detect = SET;

                    if (gu8BLDC_Comp_Comm_Error_Count >= BLDC_COMP_RXD_COMMUNICATION_ERROR_COUNT)
                    {   // ���� Ƚ�� �ݺ��Ͽ� ���� ��߻���
                        Set_BLDC_Comp_Error(BLDC_COMP_ERROR_COMM);      // ��� �ҷ� ������ ����
                    }
                    else
                    {
                        gu8BLDC_Comp_Comm_Error_Count++;
                    }
                }
                else
                {   // ��� ���� ���� ������ ���
                    gu8BLDC_Comp_Comm_Error_Count = 0;
                }
            }
        }
        else
        {
            gu16BLDC_Comp_Comm_Error_Timer++;
        }
    }

    if (gu16BLDC_Comp_Error_Check_Timer < BLDC_COMP_ERROR_CODE_CHECK_DELAY_TIME)
    {   // Comp On ���� 1�� ���ĺ��� Error Code üũ
        gu16BLDC_Comp_Error_Check_Timer++;
    }
    else
    {
    }
}


/// @brief      BLDC COMP UART Error Interrupt
/// @details    BLDC Comp UART ��� �� Error Interrput �߻��� ó�� �Լ�
/// @param      void
/// @return     void
void UART_ISR_BLDC_Comp_Rx_Error(void)
{
    U8 mu8RxBuff = 0;

    mu8RxBuff = BLDC_COMP_UART_RXD;
    BLDC_COMP_UART_RXD_ERROR = 0x07;

    gu8BLDC_Comp_Rx_Wait_TimeOut = 0;
    gu8BLDC_Comp_Rx_Receive_Count = 0;
    MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));
}

#endif

#endif


// Hal Comp Module *******************************************************************************************

/// @brief      Hal BLDC Comp Initilaize Module
/// @details    Hal BLDC Comp ���� ���� �����͸� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Module_Initialize(void)
{
#if (BLDC_COMP_USE == USE)
    Hal_BLDC_Comp_Initialize();             // BLDC Comp ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal BLDC Comp Module in 1ms Interrupt
/// @details    Hal BLDC Comp ���� Ÿ�̸Ӹ� 1ms���� ī��Ʈ ��Ų��
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Module_1ms_Control(void)
{
#if (BLDC_COMP_USE == USE)
    BLDC_Comp_ControlTimer();               // BLDC Comp ���� �ð� Counter
#endif
}


/// @brief      Hal BLDC Comp Module in While Loop
/// @details    Basic Loop�� While �� �ȿ��� BLDC Comp�� �����Ѵ�
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Module_Control(void)
{
#if (BLDC_COMP_USE == USE)
    Hal_BLDC_Comp_Control();                // BLDC Comp Control �Լ�
#endif
}

