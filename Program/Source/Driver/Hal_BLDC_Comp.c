/// @file     Hal_BLDC_Comp.c
/// @date     2025/04/11
/// @author   Jaejin Ham
/// @brief    BLDC Comp 동작 제어 file

#include "Global_Header.h"

#if (BLDC_COMP_USE == USE)

U8 gu8BLDCComp_ControlTimer = 0;

#if (BLDC_COMP_PWM_CONTROL_USE == USE)
// PWM 제어 방식 Comp
U16 gu16CompOnControlTimer = 0;                         /// @brief  Comp On 제어 타이머
U16 gu16CompOnControlRPM = 0;                           /// @brief  Comp On 제어 RPM
U8 gu8CompOnControlHz = 0;                              /// @brief  Comp On 제어 Hz
U16 gu16CompOffControlTimer = 0;                        /// @brief  Comp Off 제어 타이머
#else
// UART 통신 방식 Comp
U8 au8BLDC_Comp_TxData[BLDC_COMP_TX_DATA_COUNT];        /// @brief  BLDC Comp와 UART통신 TXD 데이터 버퍼
U8 au8BLDC_Comp_RxData[BLDC_COMP_RX_DATA_COUNT];        /// @brief  BLDC Comp와 UART통신 RXD 데이터 버퍼

U8 gu8BLDC_Comp_Tx_Timer = 0;                           /// @brief  BLDC Comp와 UART통신 Tx 주기 타이머
U8 gu8BLDC_Comp_Tx_Count = 0;                           /// @brief  BLDC Comp와 UART통신 Tx Data Byte Count
U8 gu8BLDC_Comp_Target_Hz = 0;                          /// @brief  BLDC Comp의 제어 요청 Hz
U8 gu8BLDC_Comp_Tx_Request = 0;                         /// @brief  BLDC Comp와 UART통신 Tx 가능 상태

U8 gu8BLDC_Comp_Rx_Receive_Count = 0;                   /// @brief  BLDC Comp와 UART통신 Rx Data Byte Count
U8 gu8BLDC_Comp_Rx_Wait_TimeOut = 0;                    /// @brief  BLDC Comp의 Rx 대기 Time out
U8 gu8BLDC_Comp_Rx_Complete = 0;                        /// @brief  BLDC Comp의 Rx 완료 상태
U8 gu8BLDC_Comp_Check_Error = 0;                        /// @brief  BLDC Comp의 발생한 에러
U8 gu8BLDC_Comp_ErrorCode = 0;                          /// @brief  BLDC Comp의 에러 수신 코드
U8 gu8BLDC_Comp_ErrorCodeBuffer = 0;                    /// @brief  BLDC Comp의 에러 수신 코드 버퍼
U8 gu8BLDC_Comp_Opration_Hz = 0;                        /// @brief  BLDC Comp의 동작 중인 피드백 Hz
U8 gu8BLDC_Comp_Temperature = 0;                        /// @brief  BLDC Comp의 동작 중인 Comp의 온도 상태값

U16 gu16BLDC_Comp_Comm_Error_Timer = 0;                 /// @brief  통신 상태 불량 감지 시간
U8 gu8BLDC_Comp_Comm_Error_Count = 0;                   /// @brief  통신 상태 불량 감지 카운트
U8 gu8BLDC_Comp_Comm_Error_Detect = 0;                  /// @brief  통신 상태 불량 감지 상태
U16 gu16BLDC_Comp_Error_Check_Timer = 0;                /// @brief  Comp 에러 감지 타이머

U8 gu8BLDC_Comp_Self_Count = 0;                         /// @brief  에러 코드 수신 횟수
U8 gu8BLDC_Comp_ErrorCodeDetect = 0;                    /// @brief  에러 코드 수신으로 에러 감지 상태 여부
U8 gu8BLDC_Comp_ErrorCodeDetectCount = 0;               /// @brief  에러 코드 수신으로 에러 감지 상태 반복 횟수
U16 gu16BLDC_Comp_ErrorCountClearTime = 0;              /// @brief  에러 코드 카운트 초기화 기준 시간

U8 gu8BLDC_Comp_Comm_Error_Check_End = 0;               /// @brief  현재 통신 에러 발생 체크 완료 여부
U8 gu8BLDC_Comp_Ooeration_ErrorCode_Check_End = 0;      /// @brief  현재 발생한 Comp Error Code 체크 완료 여부


/// @brief      BLDC Comp 동작 제어 Hz 테이블
/// @details    BLDC Comp 동작 제어 Hz를 테이블로 구성해 놓는다
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


/// @brief      BLDC Comp 동작 관련 초기화
/// @details    BLDC Comp 동작과 관련된 변수를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Initialize(void)
{
    gu8BLDCComp_ControlTimer = 0;

#if (BLDC_COMP_PWM_CONTROL_USE == USE)
    // PWM 제어 방식 Comp
    gu16CompOnControlTimer = 0;                 // Comp On 제어 타이머
    gu16CompOnControlRPM = 0;                   // Comp On 제어 RPM
    gu8CompOnControlHz = 0;                     // Comp On 제어 Hz
    gu16CompOffControlTimer = 0;                // Comp Off 제어 타이머
#else
    // UART 통신 방식 Comp
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

    gu16BLDC_Comp_Comm_Error_Timer = 0;         // 통신 상태 불량 감지 시간
    gu8BLDC_Comp_Comm_Error_Count = 0;          // 통신 상태 불량 감지 카운트
    gu8BLDC_Comp_Comm_Error_Detect = 0;         // 통신 상태 불량 감지 상태
    gu16BLDC_Comp_Error_Check_Timer = 0;        // Comp 에러 감지 타이머

    gu8BLDC_Comp_Self_Count = 0;                // 에러 코드 수신 횟수
    gu8BLDC_Comp_ErrorCodeDetect = 0;           // 에러 코드 수신으로 에러 감지 상태 여부
    gu8BLDC_Comp_ErrorCodeDetectCount = 0;      // 에러 코드 수신으로 에러 감지 상태 반복 횟수
    gu16BLDC_Comp_ErrorCountClearTime = 0;      // 에러 코드 카운트 초기화 기준 시간

    gu8BLDC_Comp_Comm_Error_Check_End = 0;               // 현재 통신 에러 발생 체크 완료 여부
    gu8BLDC_Comp_Ooeration_ErrorCode_Check_End = 0;      // 현재 발생한 Comp Error Code 체크 완료 여부
#endif
}


/// @brief      BLDC 콤프레셔 제어 시간 카운터(@1ms interrupt)
/// @details    BLDC Comp를 제어 시간 마다 제어하기 위해 제어 시간을 카운트 한다
/// @param      void
/// @return     void
void BLDC_Comp_ControlTimer(void)
{
    if (gu8BLDCComp_ControlTimer < BLDC_COMP_CONTROL_TIME_PERIOD)
    {
        gu8BLDCComp_ControlTimer++;
    }
}


/// @brief      BLDC 콤프레셔 제어 함수(@While Loop)
/// @details    While문 안에서 BLDC Comp 제어 동작을 한다
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


/// @brief      BLDC 콤프레셔 동작 상태 확인 함수
/// @details    BLDC Comp 동작 상태를 확인하여 반환 한다
/// @param      void
/// @return     return : Comp 동작 상태 - 0(OFF), 1(ON)
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


#if (BLDC_COMP_PWM_CONTROL_USE == USE)        // PWM 제어 방식 Comp 제어

/// @brief      BLDC 콤프레셔 제어 RPM 세팅 함수
/// @details    BLDC Comp 제어를 할 때 RPM 제어를 위한 값을 설정한다
/// @param      mu16RPM : 제어 RPM값
/// @return     void
void Set_BLDC_Comp_RPM(U16 mu16RPM)
{
    gu16CompOnControlRPM = mu16RPM;
}


/// @brief      BLDC 콤프레셔 ON/OFF 제어 함수(100ms)
/// @details    BLDC Comp 동작을 제어한다
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


/// @brief      BLDC 콤프레셔 ON 제어 함수(100ms)
/// @details    BLDC Comp가 동작 중인 상태에서 설정된 RPM 값으로 제어한다
/// @param      mu16RPM : 제어하고자 하는 Comp RPM
/// @return     void
void BLDC_Comp_On_Control(U16 mu16RPM)
{
    TURN_ON_BLDC_COMP();

    if (gu16CompOnControlTimer >= BLDC_COMP_ON_CONTROL_TIME)
    {   // Comp On 후 3초 경과
        gu8CompOnControlHz = (U8)( (mu16RPM / 60) * 2);
        BLDC_Comp_Hz_Control(gu8CompOnControlHz);
    }
    else
    {
        gu16CompOnControlTimer++;
    }
}


/// @brief      BLDC 콤프레셔 OFF 제어 함수(100ms)
/// @details    BLDC Comp를 0Hz까지 동작을 낮춘 다음 전원을 OFF 시킨다
/// @param      void
/// @return     void
void BLDC_Comp_Off_Control(void)
{
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_BLDC_COMP();

    if (mu8Status)
    {   // Comp가 켜진 상태
        gu8CompOnControlHz = 0;
        BLDC_Comp_Hz_Control(gu8CompOnControlHz);      // 0 Hz로 PWM Stop

        if (gu16CompOffControlTimer >= BLDC_COMP_OFF_CONTROL_TIME)
        {   // 30초 경과 후
            TURN_OFF_BLDC_COMP();
        }
        else
        {
            gu16CompOffControlTimer++;
        }
    }
    else
    {   // Comp가 꺼진 상태
        gu8CompOnControlHz = 0;
        BLDC_Comp_Hz_Control(gu8CompOnControlHz);      // 0 Hz로 PWM Stop
    }
}


/// @brief      BLDC 콤프레셔 Hz 제어 함수(100ms)
/// @details    BLDC Comp를 설정된 Hz값으로 제어한다
/// @param      mu8Hz : 제어하고자 하는 Comp Hz
/// @return     void
void BLDC_Comp_Hz_Control(U8 mu8Hz)
{
    if (mu8Hz < 44)
    {   // Hz가 44Hz 미만인 경우 44로 고정
        mu8Hz = 44;
    }
    else if ( mu8Hz > 150)
    {   // Hz가 150Hz 초과인 경우 150으로 고정
        mu8Hz = 150;
    }

    if (mu8Hz)
    {   // 제어 Hz가 0이 아니면
        // 100Hz => TDR11 = 19999
        BLDC_COMP_PWM_REGISTER = (U16)((((D64)(100) / mu8Hz) * 20000) - 1);

        if (BLDC_COMP_PWM_INTERRUPT_REGISTER)
        {   // INTTM11 interrupt가 disable 상태이면
            BLDC_COMP_PWM_START();
        }
    }
    else
    {   // 제어 Hz가 0이면
        BLDC_COMP_PWM_REGISTER = 0;

        if (!BLDC_COMP_PWM_INTERRUPT_REGISTER)
        {   // INTTM11 interrupt가 enable 상태이면
            BLDC_COMP_PWM_STOP();
        }
    }
}


#else       // UART 통신 방식 Comp 제어

/// @brief      BLDC 콤프레셔 제어 Hz 세팅 함수
/// @details    BLDC Comp 동작 시킬 Hz를 설정한다
/// @param      mu8Hz : 제어하고자 하는 Comp Hz
/// @return     void
void Set_BLDC_Comp_Hz(U8 mu8Hz)
{
    gu8BLDC_Comp_Target_Hz = mu8Hz;
}


/// @brief      BLDC 콤프레셔 동작 중인 Hz 확인 함수
/// @details    BLDC Comp가 동작 중인 Hz를 확인하여 반환한다
/// @param      void
/// @return     mu8Return : 현재 구동 중인 Comp의 Hz
U8 Get_BLDC_Comp_Hz(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8BLDC_Comp_Opration_Hz;

    return  mu8Return;
}


/// @brief      BLDC 콤프레셔 동작 중에 발생한 에러값 설정 함수
/// @details    BLDC Comp에서 발생한 에러를 기억해 놓는다
/// @param      mu8Err : 현재 체크된 Error
/// @return     void
void Set_BLDC_Comp_Error(U8 mu8Err)
{
    gu8BLDC_Comp_Check_Error = mu8Err;
}


/// @brief      BLDC 콤프레셔 동작 중에 발생한 에러 확인 함수
/// @details    BLDC Comp 동작 중 발생한 에러를 확인하여 반환한다
/// @param      void
/// @return     mu8Return : 현재 구동 중인 Comp의 발생한 에러 코드
U8 Get_BLDC_Comp_Error(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8BLDC_Comp_Check_Error;

    return  mu8Return;
}


/// @brief      BLDC 콤프레셔 동작 중에 발생한 에러 코드 확인 함수
/// @details    BLDC Comp 동작 중 수신한 에러 코드를 확인하여 반환한다
/// @param      void
/// @return     mu8Return : 현재 구동 중인 Comp의 발생한 에러 코드
U8 Get_BLDC_Comp_ErrorCode(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8BLDC_Comp_ErrorCode;

    return  mu8Return;
}


/// @brief      BLDC 콤프레셔 동작 중 Comp 온도 확인 함수
/// @details    BLDC Comp 동작 중 수신한 Comp 온도 값을 확인하여 반환한다
/// @param      void
/// @return     mu8Return : 현재 구동 중인 Comp의 온도
U8 Get_BLDC_Comp_Temp(void)
{
    U8 mu8Return = 0;

    mu8Return = gu8BLDC_Comp_Temperature;

    return  mu8Return;
}


/// @brief      BLDC 콤프레셔 동작 중 통신 에러 감지 상태 확인 함수
/// @details    BLDC Comp 동작 중 통신 에러 감지 상태를 확인하여 반환 한다
/// @param      void
/// @return     return : 통신 에러 감지 상태 여부 - 0(미감지), 1(감지)
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


/// @brief      BLDC 콤프레셔 동작 중 에러 코드 수신 상태 확인 함수
/// @details    BLDC Comp 동작 중 에러 코드 수신 상태를 확인하여 반환 한다
/// @param      void
/// @return     return : 에러 코드 수신 여부 - 0(미수신), 1(수신)
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


/// @brief      BLDC 콤프레셔 제어 함수(100ms)
/// @details    BLDC Comp의 UART 통신을 제어한다
/// @param      void
/// @return     void
void BLDC_Comp_Communication(void)
{
    U8 mu8CompOperationHzBuff = 0;
    U8 mu8Status = 0;

    mu8Status = GET_STATUS_BLDC_COMP();

    // Comp 전원 제어
    if (mu8Status)
    {   // Comp ON 상태
        if (gu8BLDC_Comp_Target_Hz == 0)
        {   // 제어 Hz가 0인 경우
            mu8CompOperationHzBuff = Get_BLDC_Comp_Hz();

            if (mu8CompOperationHzBuff == 0)
            {   // 동작중인 Hz가 0인 경우
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
    {   // Comp 전원 OFF 상태
        if (gu8BLDC_Comp_Target_Hz != 0)
        {   // 제어 Hz가 0이 아닌 경우
            TURN_ON_BLDC_COMP();
        }
        else
        {
            TURN_OFF_BLDC_COMP();
        }
    }

    // Comp 통신 제어
    if (mu8Status)
    {   // Comp 전원 ON 인 경우
        BLDC_Comp_Comm_Error_Check();

        BLDC_Comp_Rx_Communication();
        BLDC_Comp_Tx_Communication();
    }
    else
    {   // Comp 전원 OFF인 경우
        Clear_BLDC_Comp_Data();
    }
}


/// @brief      BLDC Comp RXD 처리 함수
/// @details    BLDC Comp와 UART 통신간 수신 데이터를 처리한다
/// @param      void
/// @return     void
void BLDC_Comp_Rx_Communication(void)
{
    U8 mu8Cal_CRC = 0;

    if (gu8BLDC_Comp_Rx_Complete == SET)
    {   // 수신 완료된 데이터가 있는 경우
        mu8Cal_CRC = BLDC_Comp_Rx_CRC(au8BLDC_Comp_RxData);

        if (au8BLDC_Comp_RxData[BLDC_COMP_RX_DATA_CHECK_SUM] == mu8Cal_CRC)
        {
            gu8BLDC_Comp_Tx_Request = SET;                      // Tx Data 송신 가능
            Receive_BLDC_Comp_Data(au8BLDC_Comp_RxData);        // 수신한 데이터 처리

            gu16BLDC_Comp_Comm_Error_Timer = 0;     // 통신 에러 체크 타이머 Clear
            gu8BLDC_Comp_Comm_Error_Count = 0;      // 통신 에러 체크 카운트 Clear
            gu8BLDC_Comp_Comm_Error_Detect = 0;     // 통신 상태 불량 감지 상태 Clear
        }
        else
        {
        }

        gu8BLDC_Comp_Rx_Complete = CLEAR;       // 수신 데이터 처리 완료
        MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));
    }
    else
    {   // 수신 완료된 데이터가 없는 경우

    }
}


/// @brief      BLDC Comp TXD 처리 함수
/// @details    BLDC Comp와 UART 통신간 송신 데이터를 처리한다
/// @param      void
/// @return     void
void BLDC_Comp_Tx_Communication(void)
{
    gu8BLDC_Comp_Tx_Timer++;

    if (gu8BLDC_Comp_Tx_Timer >= BLDC_COMP_UART_TIME_PERIOD)
    {   // Polling 주기 마다
        gu8BLDC_Comp_Tx_Timer = 0;

        BLDC_Comp_UART_Tx_Start();
    }
}


/// @brief      BLDC Comp RX Data 처리 함수
/// @details    BLDC Comp 에서 수신한 Rx 데이터를 처리한다
/// @param      pu8RxData : 수신한 Rx Data 배열
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
    {   // 통신을 시작하고 1분 경과 후부터 에러 코드 체크 시작
        if (gu8BLDC_Comp_ErrorCodeBuffer != 0)
        {   // 에러 코드가 수신된 경우
            if (gu8BLDC_Comp_ErrorCode != gu8BLDC_Comp_ErrorCodeBuffer)
            {   // 이전 수신 에러 코드와 다른 경우
                gu8BLDC_Comp_Self_Count = 0;
                gu8BLDC_Comp_ErrorCode = gu8BLDC_Comp_ErrorCodeBuffer;
            }
            else
            {   // 이전 수신 에러 코드와 같은 경우
                if (gu8BLDC_Comp_Self_Count >= BLDC_COMP_ERROR_CODE_RECIEVE_COUNT)
                {   // 같은 에러 코드를 연속 10회 이상 수신한 경우
                    if (!gu8BLDC_Comp_ErrorCodeDetect)
                    {   // 에러 미감지 상태이면
                        gu8BLDC_Comp_ErrorCodeDetect = SET;     // BLDC Comp 에러 감지 상태 설정

                        if (gu8BLDC_Comp_ErrorCodeDetectCount >= BLDC_COMP_ERROR_CODE_RECIEVE_CONFIRM_COUNT)
                        {   // 에러 감지 상태 반복이 기준 횟수 이상이면
                            BLDC_Comp_Error_Confirm(gu8BLDC_Comp_ErrorCode);    // 에러 코드 확정
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
        {   // 에러 코드 미수신 시
            gu8BLDC_Comp_Self_Count = 0;    // 에러 반복 수신 카운트 초기화
            gu8BLDC_Comp_ErrorCode = 0;

            if (gu8BLDC_Comp_ErrorCodeDetectCount)
            {   // 에러 코드 감지 상태 반복 카운트 값이 있는 경우
                if (gu16BLDC_Comp_ErrorCountClearTime >= BLDC_COMP_ERROR_CODE_COUNT_AUTO_CLEAR_TIME)
                {   // 기준 시간 동안 에러 코드 미수신이면
                    gu16BLDC_Comp_ErrorCountClearTime = 0;
                    gu8BLDC_Comp_ErrorCodeDetectCount = 0;  // 에러 반복 감지 카운트 초기화
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
    {   // 통신을 시작하고 1분 경과 전
        gu8BLDC_Comp_Self_Count = 0;    // 에러 반복 수신 카운트 초기화
        gu8BLDC_Comp_ErrorCode = 0;
        gu16BLDC_Comp_ErrorCountClearTime = 0;
    }
}


/// @brief      BLDC Comp RX Error Code Confirm 함수
/// @details    BLDC Comp 와 UART 통신으로 수신한 에러 코드를 확정 처리한다
/// @param      mu8ErrorCode : 수신한 에러 코드
/// @return     void
void BLDC_Comp_Error_Confirm(U8 mu8ErrorCode)
{
    if (gu8BLDC_Comp_Ooeration_ErrorCode_Check_End == CLEAR)
    {       // 한 주기 한번만 체크 되도록 하기 위한 조건문
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

/// @brief      BLDC Comp Uart Rxd Interrupt 처리 함수(@UART Rxd Interrupt)
/// @details    BLDC Comp와 UART 통신시 Rx Interrupt가 발생했을 때 수신 데이터를 처리한다
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
            gu8BLDC_Comp_Rx_Complete = SET;     // 데이터 패킷 수신 완료
        }
        else
        {
            gu8BLDC_Comp_Rx_Receive_Count++;
            gu8BLDC_Comp_Rx_Wait_TimeOut = 5;      // 데이터간 사이에 500ms RX 대기 Time out
        }
    }
    else
    {
        gu8BLDC_Comp_Rx_Receive_Count = 0;
        MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));
    }
}


/// @brief      BLDC Comp Uart Tx 시작 함수
/// @details    BLDC Comp와 UART 통신간 TXD를 시작 처리한다
/// @param      void
/// @return     void
void BLDC_Comp_UART_Tx_Start(void)
{
    if (gu8BLDC_Comp_Tx_Request == SET)
    {
        // Tx 데이터를 송신하기 전 Rxd Data 모두 초기화
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


/// @brief      BLDC Comp Uart Tx Packet 구성 함수
/// @details    BLDC Comp와 통신하는 Tx 데이터 Packet을 구성한다
/// @param      void
/// @return     void
void Make_BLDC_Comp_Tx_Packet(void)
{
    U8 mu8_crc = 0;

    au8BLDC_Comp_TxData[0] = au8BLDC_FrequencyData[gu8BLDC_Comp_Target_Hz - 37];;

    mu8_crc = BLDC_Comp_Tx_CRC(au8BLDC_Comp_TxData);

    au8BLDC_Comp_TxData[1] = mu8_crc;
}


/// @brief      BLDC Comp Uart Txd Interrupt 처리 함수(@UART Txd Interrupt)
/// @details    BLDC Comp와 URAT 통신간 Tx Interrupt 동작을 처리한다
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
/// @details    BLDC Comp와 UART 통신간 송신 데이터의 체크섬을 계산한다
/// @param      pu8Msg : 체크섬을 계산할 데이터 배열 
/// @return     return : 체크섬 계산 데이터
U8 BLDC_Comp_Tx_CRC(U8* pu8Msg)
{
    U8 mu8CRC = 0;

    mu8CRC = pu8Msg[0] ^ BLDC_COMP_CHECK_CODE;

    return mu8CRC;
}


/// @brief      BLDC Comp Uart Rxd Data CheckSum Calculate
/// @details    BLDC Comp 와 UART 통신간 수신 데이터의 체크섬을 계산한다
/// @param      pu8Msg : 체크섬을 계산할 데이터 배열 
/// @return     return : 체크섬 계산 데이터
U8 BLDC_Comp_Rx_CRC(U8* pu8Msg)
{
    U8 mu8CRC = 0;

    mu8CRC = (pu8Msg[0] + pu8Msg[1] + pu8Msg[2]) ^ BLDC_COMP_CHECK_CODE;

    return mu8CRC;
}


/// @brief      BLDC Comp Data Clear
/// @details    BLDC Comp 제어와 관련된 변수들을 모두 초기화 시킨다
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

    gu16BLDC_Comp_Comm_Error_Timer = 0;         // 통신 상태 불량 감지 시간
    gu16BLDC_Comp_Error_Check_Timer = 0;        // Comp 에러 감지 타이머
    gu8BLDC_Comp_Comm_Error_Detect = 0;         // 통신 상태 불량 감지 상태

    gu8BLDC_Comp_Self_Count = 0;                // 에러 코드 수신 횟수
    gu8BLDC_Comp_ErrorCodeDetect = 0;          // 에러 코드 수신으로 에러 감지 상태 여부

    gu8BLDC_Comp_Comm_Error_Check_End = 0;               // 현재 통신 에러 발생 체크 완료 여부
    gu8BLDC_Comp_Ooeration_ErrorCode_Check_End = 0;      // 현재 발생한 Comp Error Code 체크 완료 여부
}


/// @brief      BLDC Comp Error 통신 에러 체크 함수
/// @details    BLDC Comp 통신 에러 체크를 한다
/// @param      void
/// @return     void
void BLDC_Comp_Comm_Error_Check(void)
{
    if (gu8BLDC_Comp_Rx_Wait_TimeOut)
    {   // Rx 패킷이 중간에 끊어져서  끝까지 제대로 수신되지 않는 경우
        gu8BLDC_Comp_Rx_Wait_TimeOut--;

        if (gu8BLDC_Comp_Rx_Wait_TimeOut == 0)
        {
            gu8BLDC_Comp_Rx_Receive_Count = 0;
            MEMSET(au8BLDC_Comp_RxData, 0x00, sizeof(au8BLDC_Comp_RxData));
        }
    }

    if (gu8BLDC_Comp_Rx_Complete == SET)
    {   // 수신 완료된 데이터가 있는 경우
        // Error Check Timer는 수신 데이터 정상 여부 확인 한 후 Clear 처리함
    }
    else
    {
        if (gu16BLDC_Comp_Comm_Error_Timer >= BLDC_COMP_RXD_COMMUNICATION_ERROR_TIME)
        {   // 30초 동안 통신이 없는 경우
            if (gu8BLDC_Comp_Comm_Error_Check_End == CLEAR)
            {
                gu8BLDC_Comp_Comm_Error_Check_End = SET;

                if (gu8BLDC_Comp_Comm_Error_Detect == CLEAR)
                {   // 통신 에러 미감지 상태인 경우
                    gu8BLDC_Comp_Comm_Error_Detect = SET;

                    if (gu8BLDC_Comp_Comm_Error_Count >= BLDC_COMP_RXD_COMMUNICATION_ERROR_COUNT)
                    {   // 일정 횟수 반복하여 에러 재발생시
                        Set_BLDC_Comp_Error(BLDC_COMP_ERROR_COMM);      // 통신 불량 에러로 결정
                    }
                    else
                    {
                        gu8BLDC_Comp_Comm_Error_Count++;
                    }
                }
                else
                {   // 통신 에러 감지 상태인 경우
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
    {   // Comp On 이후 1분 이후부터 Error Code 체크
        gu16BLDC_Comp_Error_Check_Timer++;
    }
    else
    {
    }
}


/// @brief      BLDC COMP UART Error Interrupt
/// @details    BLDC Comp UART 통신 중 Error Interrput 발생시 처리 함수
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
/// @details    Hal BLDC Comp 제어 관련 데이터를 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Module_Initialize(void)
{
#if (BLDC_COMP_USE == USE)
    Hal_BLDC_Comp_Initialize();             // BLDC Comp 제어 관련 초기화
#endif
}


/// @brief      Hal BLDC Comp Module in 1ms Interrupt
/// @details    Hal BLDC Comp 제어 타이머를 1ms마다 카운트 시킨다
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Module_1ms_Control(void)
{
#if (BLDC_COMP_USE == USE)
    BLDC_Comp_ControlTimer();               // BLDC Comp 제어 시간 Counter
#endif
}


/// @brief      Hal BLDC Comp Module in While Loop
/// @details    Basic Loop의 While 문 안에서 BLDC Comp를 제어한다
/// @param      void
/// @return     void
void Hal_BLDC_Comp_Module_Control(void)
{
#if (BLDC_COMP_USE == USE)
    Hal_BLDC_Comp_Control();                // BLDC Comp Control 함수
#endif
}

