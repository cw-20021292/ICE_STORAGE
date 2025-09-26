/// @file     Hal_BLDC_Comp.h
/// @date     2025/04/11
/// @author   Jaejin Ham
/// @brief    BLDC Comp 동작 제어 헤더 file

#ifndef _HAL_BLDC_COMP_H_
#define _HAL_BLDC_COMP_H_


/*  BLDC Comp 제어 Function 사용법
    => u8Return = Get_BLDC_Comp_Status();                   // Comp 동작 상태(전원 상태) 확인 - 0(OFF), 1(ON)

    1. PWM 제어 방식의 경우
     => Set_BLDC_Comp_RPM(mu16RPM);                         // mu16RPM으로 BLDC Comp 제어
        사용 예)
        Set_BLDC_Comp_RPM(2400);                            // 2400rpm으로 Comp 제어

    2. UART 통신 방식의 경우
     => Set_BLDC_Comp_Hz(mu8Hz);                            // mu8Hz의 주파수로 BLDC Comp 제어
     => u8Return = Get_BLDC_Comp_Hz();                      // 현재 구동중인 제어 Hz 확인
     => Set_BLDC_Comp_Error(mu8Err);                        // 현재 체크된 Error 설정 - 0(No), 1(E81), 2(E82), 3(E83). 4(E84), 5(E85), 6(E86), 7(Communication)
     => u8Return = Get_BLDC_Comp_Error();                   // 현재 체크된 Error 확인 - 0(No), 1(E81), 2(E82), 3(E83). 4(E84), 5(E85), 6(E86), 7(Communication)
        에러 확인 함수는 현재 발생된 에러의 체크 카운트 1회에 해당되는 부분까지만 체크하여 확인 가능하게 함
        즉 Comp 반복 재기동 제어 및 반복 발생 체크, 에러 확정 로직은 별도로 구성해야 하며, Comp 재기동시 Error 체크를 다시 할 수 있도록 Error Clear 해줘야 함.
     => u8Return = Get_BLDC_Comp_ErrorCode();               // 현재 구동 중인 Comp의 발생한 에러 코드 확인
     => u8Return = Get_BLDC_Comp_Temp();                    // 현재 구동 중인 Comp의 온도 확인
     => u8Return = Get_BLDC_Comp_Comm_ErrorDetect();        // 통신 에러 감지 상태 여부 확인 - 0(미감지), 1(감지)
     => u8Return = Get_BLDC_Comp_ErrorDetect();             // 에러 코드 수신 여부 확인 - 0(미수신), 1(수신)

     사용 예)
     Set_BLDC_Comp_Hz(50);                               // 50Hz로 BLDC Comp 제어
     Set_BLDC_Comp_Hz(0);                                // 0Hz(Comp Off)로 BLDC Comp 제어

     mu8Return = Get_BLDC_Comp_Hz();                     // 현재 피드백으로 수신한 구동중인 제어 Hz 확인 - 0이면 Comp OFF 상태임
     mu8Return = Get_BLDC_Comp_Error();                  // 현재 체크된 Error 확인
*/


#define BLDC_COMP_USE                                   USE      // BLDC Comp 사용 여부 - 0(No Use), 1(Use)

#if (BLDC_COMP_USE == USE)

#define SAMSUNG_BLDC_COMP                               0
#define LG_BLDC_COMP                                    1
#define BLDC_COMP_TYPE                                  SAMSUNG_BLDC_COMP

#define BLDC_COMP_PWM_CONTROL_USE                       NO_USE      // BLDC Comp PWM 제어 사용 여부 - 0(No Use), 1(Use PWM)

#if (BLDC_COMP_PWM_CONTROL_USE == USE)
// PWM 제어 방식 Comp
#define BLDC_COMP_PWM_REGISTER                          TDR03                   // PWM Out Register
#define BLDC_COMP_PWM_INTERRUPT_REGISTER                TMMK03                  // Interrupt Enable/Disable Register
#define BLDC_COMP_PWM_START()                           R_Config_TAU0_3_Start();   // Code Generate된 PWM 제어 Start함수 배치
#define BLDC_COMP_PWM_STOP()                            R_Config_TAU0_3_Stop();    // Code Generate된 PWM 제어 Stop함수 배치
#else
// UART 통신 제어 방식 Comp
#define BLDC_COMP_UART_TXD                              TXD0
#define BLDC_COMP_UART_RXD                              RXD0
#define BLDC_COMP_UART_RXD_ERROR                        SIR01
#define BLDC_COMP_UART_START()                          R_Config_UART0_Start();
#define BLDC_COMP_UART_STOP()                           R_Config_UART0_Stop();

#endif

// Comp port 할당, 미사용시 0으로 세팅
#define P_BLDC_COMP_ON                                  P0_bit.no6  // BLDC Comp power on
#define TURN_ON_BLDC_COMP()                             { P_BLDC_COMP_ON = 1; }
#define TURN_OFF_BLDC_COMP()                            { P_BLDC_COMP_ON = 0; }
#define GET_STATUS_BLDC_COMP()                          ( P_BLDC_COMP_ON )

#define BLDC_COMP_CONTROL_TIME_PERIOD                   100         // 100ms@1ms

#if (BLDC_COMP_PWM_CONTROL_USE == USE)
// PWM Duty 제어 Comp
#define BLDC_COMP_ON_CONTROL_TIME                       30          // 3sec@100ms
#define BLDC_COMP_OFF_CONTROL_TIME                      300         // 30sec@100ms

#else
// UART 통신형 Comp
#define BLDC_37Hz                                       0b00100101
#define BLDC_38Hz                                       0b00100110
#define BLDC_39Hz                                       0b00100111
#define BLDC_40Hz                                       0b00101000
#define BLDC_41Hz                                       0b00101001
#define BLDC_42Hz                                       0b00101010
#define BLDC_43Hz                                       0b00101011
#define BLDC_44Hz                                       0b00101100
#define BLDC_45Hz                                       0b00101101
#define BLDC_46Hz                                       0b00101110
#define BLDC_47Hz                                       0b00101111
#define BLDC_48Hz                                       0b00110000
#define BLDC_49Hz                                       0b00110001
#define BLDC_50Hz                                       0b00110010
#define BLDC_51Hz                                       0b00110011
#define BLDC_52Hz                                       0b00110100
#define BLDC_53Hz                                       0b00110101
#define BLDC_54Hz                                       0b00110110
#define BLDC_55Hz                                       0b00110111
#define BLDC_56Hz                                       0b00111000
#define BLDC_57Hz                                       0b00111001
#define BLDC_58Hz                                       0b00111010
#define BLDC_59Hz                                       0b00111011
#define BLDC_60Hz                                       0b00111100
#define BLDC_61Hz                                       0b00111101
#define BLDC_62Hz                                       0b00111110
#define BLDC_63Hz                                       0b00111111
#define BLDC_64Hz                                       0b01000000
#define BLDC_65Hz                                       0b01000001
#define BLDC_66Hz                                       0b01000010
#define BLDC_67Hz                                       0b01000011
#define BLDC_68Hz                                       0b01000100
#define BLDC_69Hz                                       0b01000101
#define BLDC_70Hz                                       0b01000110
#define BLDC_71Hz                                       0b01000111
#define BLDC_72Hz                                       0b01001000
#define BLDC_73Hz                                       0b01001001
#define BLDC_74Hz                                       0b01001010
#define BLDC_75Hz                                       0b01001011

#define BLDC_COMP_ERROR_81                              1           // Error 81
#define BLDC_COMP_ERROR_82                              2           // Error 82
#define BLDC_COMP_ERROR_83                              3           // Error 83
#define BLDC_COMP_ERROR_84                              4           // Error 82
#define BLDC_COMP_ERROR_85                              5           // Error 85
#define BLDC_COMP_ERROR_86                              6           // Error 86
#define BLDC_COMP_ERROR_COMM                            7           // Communication Error

#if (BLDC_COMP_TYPE == LG_BLDC_COMP)            // LG BLDC Comp
#define BLDC_COMP_UART_TIME_PERIOD                      20      // Comp Uart Polling 주기 2sec = 20@100ms
#define BLDC_COMP_TX_DATA_COUNT                         2       // TX 2Byte
#define BLDC_COMP_RX_DATA_COUNT                         3       // RX 3Byte
#define BLDC_COMP_CHECK_CODE                            0x55

#define BLDC_COMP_RX_DATA_ERROR_CODE                    0
#define BLDC_COMP_RX_DATA_OPERATION_FREQUENCY           1
#define BLDC_COMP_RX_DATA_CHECK_SUM                     2

#define LG_BLDC_COMP_E81_CURRENT_SENSING_ERROR          1
#define LG_BLDC_COMP_E82_POSITION_SENSING_ERROR         2
#define LG_BLDC_COMP_E83_OVER_CURRENT_ERROR             6
#define LG_BLDC_COMP_E84_IPM_FAULT_ERROR                7
#define LG_BLDC_COMP_E85_DISCONNECTION_COMM_ERROR       8
#define LG_BLDC_COMP_E86_OVERLOAD_PROTECTION_ERROR      9
#else                                           // SAMSUNG BLDC Comp
#define BLDC_COMP_UART_TIME_PERIOD                      20      // Comp Uart Polling 주기 2sec = 20@100ms
#define BLDC_COMP_TX_DATA_COUNT                         2       // TX 2Byte
#define BLDC_COMP_RX_DATA_COUNT                         4       // RX 4Byte
#define BLDC_COMP_CHECK_CODE                            0x55

#define BLDC_COMP_RX_DATA_ERROR_CODE                    0
#define BLDC_COMP_RX_DATA_OPERATION_FREQUENCY           1
#define BLDC_COMP_RX_DATA_TEMPERATURE                   2
#define BLDC_COMP_RX_DATA_CHECK_SUM                     3

#define SAMSUNG_BLDC_COMP_E81_CURRENT_SENSING_ERROR     2
#define SAMSUNG_BLDC_COMP_E82_STARTING_FAIL_ERROR       1
#define SAMSUNG_BLDC_COMP_E83_OVER_CURRENT_ERROR        3
#define SAMSUNG_BLDC_COMP_E84_OVERHEAT_ERROR            5
#define SAMSUNG_BLDC_COMP_E85_DISCONNECTION_COMM_ERROR  6
#define SAMSUNG_BLDC_COMP_E86_ABNORMAL_VOLTAGE_ERROR    4
#endif  // BLDC Comp Company Option

#define BLDC_COMP_RXD_COMMUNICATION_ERROR_TIME          300     // UART 통신 에러 감지 기준 시간 30sec@100ms
#define BLDC_COMP_RXD_COMMUNICATION_ERROR_COUNT         10      // 통신 에러 확정 재시도 횟수
#define BLDC_COMP_ERROR_CODE_CHECK_DELAY_TIME           600     // Error Code 체크하기 전 Delay 시간 60sec@100ms
#define BLDC_COMP_ERROR_CODE_RECIEVE_COUNT              10      // Error Code 수신으로 에러 감지을 위한 반복 수신 횟수
#define BLDC_COMP_ERROR_CODE_RECIEVE_CONFIRM_COUNT      10      // Error Code 수신으로 에러 감지 후 에러 확정을 위한 감지 반복 횟수
#define BLDC_COMP_ERROR_CODE_COUNT_AUTO_CLEAR_TIME      6000    // Error Code 수신으로 에러 발생 확정 카운트를 자동 초기화 시킬 기준 시간 10min = 600sec@100ms

#endif


void Hal_BLDC_Comp_Initialize(void);
void BLDC_Comp_ControlTimer(void);
void Hal_BLDC_Comp_Control(void);
U8 Get_BLDC_Comp_Status(void);

#if (BLDC_COMP_PWM_CONTROL_USE == USE)
void Set_BLDC_Comp_RPM(U16 mu16RPM);
void BLDC_Comp_OnOff_Control(void);
void BLDC_Comp_On_Control(U16 mu16RPM);
void BLDC_Comp_Off_Control(void);
void BLDC_Comp_Hz_Control(U8 mu8Hz);
#else
void Set_BLDC_Comp_Hz(U8 mu8Hz);
U8 Get_BLDC_Comp_Hz(void);
void Set_BLDC_Comp_Error(U8 mu8Err);
U8 Get_BLDC_Comp_Error(void);
U8 Get_BLDC_Comp_ErrorCode(void);
U8 Get_BLDC_Comp_Temp(void);
U8 Get_BLDC_Comp_Comm_ErrorDetect(void);
U8 Get_BLDC_Comp_ErrorDetect(void);
void BLDC_Comp_Communication(void);
void BLDC_Comp_Rx_Communication(void);
void BLDC_Comp_Tx_Communication(void);
void Receive_BLDC_Comp_Data(U8* pu8RxData);
void BLDC_Comp_Error_Confirm(U8 mu8ErrorCode);
void UART_ISR_BLDC_Comp_Rx(void);
void BLDC_Comp_UART_Tx_Start(void);
void Make_BLDC_Comp_Tx_Packet(void);
void UART_ISR_BLDC_Comp_Tx(void);
U8 BLDC_Comp_Tx_CRC(U8* pu8Msg);
U8 BLDC_Comp_Rx_CRC(U8* pu8Msg);
void Clear_BLDC_Comp_Data(void);
void BLDC_Comp_Comm_Error_Check(void);
void UART_ISR_BLDC_Comp_Rx_Error(void);
#endif

#endif

void Hal_BLDC_Comp_Module_Initialize(void);
void Hal_BLDC_Comp_Module_1ms_Control(void);
void Hal_BLDC_Comp_Module_Control(void);

#endif

