/// @file     Lib_Water_PTA_Mode.h
/// @date     2025/06/27
/// @author   Jaejin Ham
/// @brief    공장 자동화(PTA) 테스트 모드 관련 Header File

#ifndef _LIB_WATER_PTA_MODE_H_
#define _LIB_WATER_PTA_MODE_H_


/*  PTA 검사 모드 Function 사용법
     => void PTA_UART_TXD_Start();          // PTA UART TX 데이터를 송신 시작할 때 쓰는 함수(시작 후부터는 인터럽트에서 처리)
     => UART_ISR_PTA_Tx();                  // PTA 통신 UART의 TXD 인터럽트 함수 안에 쓰는 함수
     => UART_ISR_PTA_Rx();                  // PTA 통신 UART의 RXD 인터럽트 함수 안에 쓰는 함수
     => UART_ISR_PTA_Rx_Error();            // PTA 통신 UART의 RXD Error 인터럽트 함수 안에 쓰는 함수
     => mu8Data = Get_PTA_CheckEEPROM();    // PTA 검사 진입시 EEPROM에서 END 번지 값을 읽여서 확인하는 함수(PTA 검사시 리턴할 EEPROM 데이터)
     => NoPowerSet();                       // 무부하 상태일 때 모든 부하를 OFF시켜야 하므로 모든 부하 OFF시키는 코드를 함수 내에 추가해야 하는 함수
     => TurnOff_Heater();                   // 제한시간 동작 후 히터를 종료할 때 OFF시켜야 하는 히터 제어를 추가해야 하는 함수


     => PTA_ACK_CMD_xxxx_Send() 의 형태로 구성된 각 Command별 동작 함수에 제어 동작을 추가하면 됨
*/


#define PTA_TEST_MODE_USE                                   USE

#if (PTA_TEST_MODE_USE == USE)


#define PTA_UART_TXD_DATA_REGISTER                          TXD1
#define PTA_UART_TXD_INTERRUPT_REGISTER                     STMK1
#define PTA_UART_RXD_DATA_REGISTER                          RXD1
#define PTA_UART_RXD_ERROR_INTERRUPT_REGISTER               SIR03
#define PTA_UART_START()                                    R_Config_UART1_Start();
#define PTA_UART_STOP()                                     R_Config_UART1_Stop();

#define PTA_UART_DATA_LENGTH                                50          // PTA 모드 UART 통신 DATA 최대 Length

#define PTA_MODE_NONE                                       0
#define PTA_MODE_1                                          1
#define PTA_MODE_2                                          2
#define PTA_MODE_3                                          3

#define PTA_CONTROL_TIME_PERIOD                             100         // PTA 검사 모드 제어 시간 타이머 100ms@1ms
#define PTA_MODE_IN_TIME_OUT                                300         // PTA 검사 모드 진입 가능 시간 타이머 30sec@100ms

// Factory Test Protocol
#define AT_RS232_STX                                        0x01
#define AT_RS232_ETX                                        0x04

#define AT_UART_ACK                                         "06"        // ACk Data
#define AT_UART_NACK                                        "15"        // NACK Data

// Command 0x200X - 모델 정보 확인
// 검사 모드 선택
// - Data Format : 0x2000 + (0x01 or 0x02)
// - 0x01 : 자동검사 진입 모드 (검사 2모드)
// - 0x02 : 자동검사 진입 전 모드 (검사 1모드)
//          : 조합키 또는 통신으로 자동 진입
//          : 수동 검사 동작 완료 후 검사 2모드 진입 및 검사기로 Ack 전달
#define AT_UART_CMD_MODEL_NAME                              0x2000
#define AT_UART_CMD_MODEL_NAME_PRETEST_SKIP                 0x01        // 모드 진입시 수동 테스트 SKIP 진행 데이터
#define AT_UART_CMD_MODEL_NAME_PRETEST                      0x02        // 모드 진입시 수동 테스트 진행 데이터

// Command 0x300X - 설정값 확인
#define AT_UART_CMD_BASIC_INFO                              0x3000      // 설정값 검사
#define AT_UART_MODEL_INFO                                  "34"        // 모델 정보
#define AT_UART_MODEL_CONTRY                                "00"        // 향지 국가
#define AT_UART_MODEL_TYPE_CP                               "00"
#define AT_UART_MODEL_TYPE_CHP                              "01"

// Command 0x301X - 기능 검사
#define AT_UART_CMD_WIFI_INFO                               0x3010      // Wi-Fi 기능
#define AT_UART_WIFI_NO_CONNECT                             "00"
#define AT_UART_WIFI_CONNECT                                "01"

#define AT_UART_CMD_EW_UV_INFO                              0x3011      // EW, UV 모듈 피드백
#define AT_UART_CMD_FLOW_SENSOR_INFO                        0x3012      // 유량센서 피드백
#define AT_UART_CMD_AC_DC_CURRENT_INFO                      0x3013      // AC/DC 전류
#define AT_UART_CMD_FAN_INFO                                0x3014      // FAN 동작
#define AT_UART_CMD_INFRARED_SENSOR_INFO                    0x3015      // 적외선 센서 동작
#define AT_UART_CMD_STIRRING_MOTOR_INFO                     0x3016      // 교반 모터 동작
#define AT_UART_CMD_PRESSURE_SENSOR_INFO                    0x3017      // 압력 센서 동작
#define AT_UART_CMD_TDS_IN_SENSOR_INFO                      0x3018      // 원수 TDS 센서 동작
#define AT_UART_CMD_TDS_OUT_SENSOR_INFO                     0x3019      // 출수 TDS 센서 동작
#define AT_UART_CMD_BOOST_PUMP_INFO                         0x301A      // 부스트 펌프 동작
#define AT_UART_CMD_CIRCULATE_PUMP_INFO                     0x301B      // 순환 펌프 동작
#define AT_UART_CMD_BRIGHTNESS_SENSOR_INFO                  0x301C      // 조도 센서 동작

// Command 0x302X - 온도 센서 검사
// 1. 온도센서 Hexa → 온도 변환값 전송, 양음 표시(양수 0XXX, 음수 1XXX), 콤프동작시 냉수온도 1초단위 전송, 소수점 1자리 표시, 에러 시 9999
// 2. 온도센서 Hexa → 온도 변환값 전송, 콤프동작시 냉수온도 1초단위 전송, 소수점 2자리 표시, 에러 시 9999
#define AT_UART_CMD_TEMP_SENSOR_INFO                        0x3020      // 온도 센서 상태
#define AT_UART_TEMP_SENSOR_ERROR                           "9999"      // 온도 센서 에러 상태값

// Command 0x303X - 수위 센서 체결 검사
// 수위센서 미체결 검출 되도록 HW 설계 - Active High
#define AT_UART_CMD_WATER_LEVEL_SENSOR_INFO                 0x3030      // 수위 센서 체결 상태
#define AT_UART_LEVEL_SENSOR_NO_CONNECT                     "00"        // 수위 센서 미체결 상태값
#define AT_UART_LEVEL_SENSOR_CONNECT                        "01"        // 수위 센서 체결 상태값

// Command 0x304X - 일반 센서 체결 검사
#define AT_UART_CMD_SENSOR_INFO                             0x3040      // 일반 센서 체결 상태
#define AT_UART_SENSOR_NO_CONNECT                           "00"        // 일반 센서 미체결 상태값
#define AT_UART_SENSOR_CONNECT                              "01"        // 일반 센서 체결 상태값

// Command 0x304X - 체결 피드백 검사
#define AT_UART_CMD_FEEDBACK_INFO                           0x3050

// Command 0x50XX - 히터 부하 검사
// 통신 트러블 등으로 인한 온수히터 과열 방지를 제품 내 Off Time out 및 연속 미동작 설정
// => 순간온수 Type: 0.25 Sec / 일반온수 Type : 7 Sec
#define AT_UART_CMD_NO_POWER_SET                            0x5000      // 무부하 설정
#define AT_UART_CMD_AC_HEATER_ON                            0x5010      // 온수 히터 ON
#define AT_UART_CMD_AC_HEATER_OFF                           0x5011      // 온수 히터 OFF
#define AT_UART_CMD_DE_ICE_HEATER_ON                        0x5012      // 탈빙 히터 ON
#define AT_UART_CMD_DE_ICE_HEATER_OFF                       0x5013      // 탈빙 히터 OFF

// Command 0x51XX - 밸브 검사
#define AT_UART_CMD_ROOM_IN_VALVE_ON                        0x5100      // 정수 입수 밸브 ON
#define AT_UART_CMD_ROOM_IN_VALVE_OFF                       0x5101      // 정수 입수 밸브 OFF
#define AT_UART_CMD_COLD_IN_VALVE_ON                        0x5102      // 냉수 입수 밸브 ON
#define AT_UART_CMD_COLD_IN_VALVE_OFF                       0x5103      // 냉수 입수 밸브 OFF
#define AT_UART_CMD_HOT_IN_VALVE_ON                         0x5104      // 온수 입수 밸브 ON
#define AT_UART_CMD_HOT_IN_VALVE_OFF                        0x5105      // 온수 입수 밸브 OFF
#define AT_UART_CMD_ROOM_OUT_VALVE_ON                       0x5106      // 정수 추출 밸브 ON
#define AT_UART_CMD_ROOM_OUT_VALVE_OFF                      0x5107      // 정수 추출 밸브 OFF
#define AT_UART_CMD_COLD_OUT_VALVE_ON                       0x5108      // 냉수 추출 밸브 ON
#define AT_UART_CMD_COLD_OUT_VALVE_OFF                      0x5109      // 냉수 추출 밸브 OFF
#define AT_UART_CMD_HOT_OUT_VALVE_ON                        0x510A      // 온수 추출 밸브 ON
#define AT_UART_CMD_HOT_OUT_VALVE_OFF                       0x510B      // 온수 추출 밸브 OFF
#define AT_UART_CMD_TWO_WAY_VALVE_ON                        0x510C      // 2Way 밸브 ON
#define AT_UART_CMD_TWO_WAY_VALVE_OFF                       0x510D      // 2Way 밸브 OFF
#define AT_UART_CMD_INPUT_FEED_VALVE_ON                     0x510E      // 원수 입수 Feed 밸브 ON
#define AT_UART_CMD_INPUT_FEED_VALVE_OFF                    0x510F      // 원수 입수 Feed 밸브 OFF
#define AT_UART_CMD_INPUT_NOS_VALVE_ON                      0x5110      // 원수 입수 NOS 밸브 ON
#define AT_UART_CMD_INPUT_NOS_VALVE_OFF                     0x5111      // 원수 입수 NOS 밸브 OFF
#define AT_UART_CMD_OUTPUT_DRAIN_NOS_VALVE_ON               0x5112      // 출수 드레인 NOS 밸브 ON
#define AT_UART_CMD_OUTPUT_DRAIN_NOS_VALVE_OFF              0x5113      // 출수 드레인 NOS 밸브 OFF
#define AT_UART_CMD_HOT_DRAIN_VALVE_ON                      0x5114      // 온수 드레인 밸브 ON
#define AT_UART_CMD_HOT_DRAIN_VALVE_OFF                     0x5115      // 온수 드레인 밸브 OFF
#define AT_UART_CMD_ICE_TRAY_VALVE_ON                       0x5116      // 아이스 트레이 밸브 ON
#define AT_UART_CMD_ICE_TRAY_VALVE_OFF                      0x5117      // 아이스 트레이 밸브 OFF
#define AT_UART_CMD_DRAIN_VALVE_ON                          0x5118      // 드레인 밸브 ON
#define AT_UART_CMD_DRAIN_VALVE_OFF                         0x5119      // 드레인 밸브 OFF
#define AT_UART_CMD_EW_DRAIN_VALVE_ON                       0x511A      // 살균 드레인 밸브 ON
#define AT_UART_CMD_EW_DRAIN_VALVE_OFF                      0x511B      // 살균 드레인 밸브 OFF
#define AT_UART_CMD_EW_INPUT_VALVE_ON                       0x511C      // 살균 입수 밸브 ON
#define AT_UART_CMD_EW_INPUT_VALVE_OFF                      0x511D      // 살균 입수 밸브 OFF
#define AT_UART_CMD_SODA_DRAIN_VALVE_ON                     0x511E      // 탄산 배수 밸브 ON
#define AT_UART_CMD_SODA_DRAIN_VALVE_OFF                    0x511F      // 탄산 배수 밸브 OFF
#define AT_UART_CMD_SODA_OUTPUT_VALVE_ON                    0x5120      // 탄산 출수 밸브 ON
#define AT_UART_CMD_SODA_OUTPUT_VALVE_OFF                   0x5121      // 탄산 출수 밸브 OFF
#define AT_UART_CMD_REFRIGERANT_CONVERSION_VALVE_ON         0x5122      // 냉매 전환 밸브 ON
#define AT_UART_CMD_REFRIGERANT_CONVERSION_VALVE_OFF        0x5123      // 냉매 전환 밸브 OFF
#define AT_UART_CMD_HOT_GAS_VALVE_ON                        0x5124      // 핫가스 밸브 ON
#define AT_UART_CMD_HOT_GAS_VALVE_OFF                       0x5125      // 핫가스 밸브 OFF
#define AT_UART_CMD_ION_EXCHANGE_REGIN_VALVE_ON             0x5126      // 이온 교환 수지 밸브 ON
#define AT_UART_CMD_ION_EXCHANGE_REGIN_VALVE_OFF            0x5127      // 이온 교환 수지 밸브 OFF
#define AT_UART_CMD_INO_VALVE_ON                            0x5128      // 이노 센스 밸브 ON
#define AT_UART_CMD_INO_VALVE_OFF                           0x5129      // 이노 센스 밸브 OFF
#define AT_UART_CMD_HOT_TANK_OVERFLOW_VALVE_ON              0x512A      // 온수 탱크 오버플로우 밸브 ON
#define AT_UART_CMD_HOT_TANK_OVERFLOW_VALVE_OFF             0x512B      // 온수 탱크 오버플로우 밸브 OFF
#define AT_UART_CMD_COLD_TANK_OVERFLOW_VALVE_ON             0x512C      // 냉수 탱크 오버플로우 밸브 ON
#define AT_UART_CMD_COLD_TANK_OVERFLOW_VALVE_OFF            0x512D      // 냉수 탱크 오버플로우 밸브 OFF
#define AT_UART_CMD_TOTAL_OUTPUT_VALVE_ON                   0x512E      // 냉온정 추출 밸브 ON
#define AT_UART_CMD_TOTAL_OUTPUT_VALVE_OFF                  0x512F      // 냉온정 추출 밸브 OFF
#define AT_UART_CMD_ICE_COLD_TANK_INPUT_VALVE_ON            0x5130      // 빙축탱크 추출 밸브 ON
#define AT_UART_CMD_ICE_COLD_TANK_INPUT_VALVE_OFF           0x5131      // 빙축탱크 추출 밸브 OFF
#define AT_UART_CMD_PRESSURIZED_INPUT_VALVE_ON              0x5132      // 가압수 입수 밸브 ON
#define AT_UART_CMD_PRESSURIZED_INPUT_VALVE_OFF             0x5133      // 가압수 입수 밸브 OFF
#define AT_UART_CMD_PRESSURIZED_DRAIN_VALVE_ON              0x5134      // 가압수 배수 밸브 ON
#define AT_UART_CMD_PRESSURIZED_DRAIN_VALVE_OFF             0x5135      // 가압수 배수 밸브 OFF

// Command 0x52XX - 모터 검사
#define AT_UART_CMD_FAN_ON                                  0x5200      // 냉각팬 ON
#define AT_UART_CMD_FAN_OFF                                 0x5201      // 냉각팬 OFF
#define AT_UART_CMD_STEP_MOTOR_ON                           0x5202      // 스텝모터 ON
#define AT_UART_CMD_STEP_MOTOR_OFF                          0x5203      // 스텝모터 OFF
#define AT_UART_CMD_ICE_MAKE_MOTOR_ON                       0x5204      // 아이스 제빙 모터 ON
#define AT_UART_CMD_ICE_MAKE_MOTOR_OFF                      0x5205      // 아이스 제빙 모터 OFF
#define AT_UART_CMD_ICE_TRAY_MOTOR_ON                       0x5206      // 아이스 트레이 모터 ON
#define AT_UART_CMD_ICE_TRAY_MOTOR_OFF                      0x5207      // 아이스 트레이 모터 OFF
#define AT_UART_CMD_ICE_DOOR_MOTOR_ON                       0x5208      // 아이스 도어 모터 ON
#define AT_UART_CMD_ICE_DOOR_MOTOR_OFF                      0x5209      // 아이스 도어 모터 OFF
#define AT_UART_CMD_ICE_OUTPUT_CW_MOTOR_ON                  0x520A      // 아이스 추출 모터-정 ON
#define AT_UART_CMD_ICE_OUTPUT_CW_MOTOR_OFF                 0x520B      // 아이스 추출 모터-정 OFF
#define AT_UART_CMD_ICE_OUTPUT_CCW_MOTOR_ON                 0x520C      // 아이스 추출 모터-역 ON
#define AT_UART_CMD_ICE_OUTPUT_CCW_MOTOR_OFF                0x520D      // 아이스 추출 모터-역 OFF
#define AT_UART_CMD_SELECT_BAR_MOTOR_ON                     0x520E      // 셀렉트바 모터 ON
#define AT_UART_CMD_SELECT_BAR_MOTOR_OFF                    0x520F      // 셀렉트바 모터 OFF
#define AT_UART_CMD_SWING_BAR_MOTOR_ON                      0x5210      // 스윙바 모터 ON
#define AT_UART_CMD_SWING_BAR_MOTOR_OFF                     0x5211      // 스윙바 모터 OFF
#define AT_UART_CMD_FAUCET_MOTOR_CW_ON                      0x5212      // 파우셋 모터 정방향 ON
#define AT_UART_CMD_FAUCET_MOTOR_CW_OFF                     0x5213      // 파우셋 모터 정방향 OFF
#define AT_UART_CMD_STIRRING_MOTOR_ON                       0x5214      // 교반 모터 ON
#define AT_UART_CMD_STIRRING_MOTOR_OFF                      0x5215      // 교반 모터 OFF
#define AT_UART_CMD_FAUCET_MOTOR_CCW_ON                     0x5216      // 파우셋 모터 역방향 ON
#define AT_UART_CMD_FAUCET_MOTOR_CCW_OFF                    0x5217      // 파우셋 모터 역방향 OFF

// Command 0x53XX - 펌프 검사
#define AT_UART_CMD_BOOST_PUMP_ON                           0x5300      // 부스트 펌프 ON
#define AT_UART_CMD_BOOST_PUMP_OFF                          0x5301      // 부스트 펌프 OFF
#define AT_UART_CMD_DRAIN_PUMP_ON                           0x5302      // 드레인 펌프 ON
#define AT_UART_CMD_DRAIN_PUMP_OFF                          0x5303      // 드레인 펌프 OFF
#define AT_UART_CMD_COLD_CIRCULATE_PUMP_ON                  0x5304      // 냉수 순환 펌프 ON
#define AT_UART_CMD_COLD_CIRCULATE_PUMP_OFF                 0x5305      // 냉수 순환 펌프 OFF
#define AT_UART_CMD_HOT_OUTPUT_PUMP_ON                      0x5306      // 온수 추출 펌프 ON
#define AT_UART_CMD_HOT_OUTPUT_PUMP_OFF                     0x5307      // 온수 추출 펌프 OFF

// Command 0x60XX - 키 검사

// Command 0x70XX - 디스플레이 검사
#define AT_UART_CMD_UV_ON                                   0x7000      // UV LED ON
#define AT_UART_CMD_UV_OFF                                  0x7001      // UV LED OFF
#define AT_UART_CMD_LED2_ON                                 0x7002      // LED2 ON
#define AT_UART_CMD_LED2_OFF                                0x7003      // LED2 OFF
#define AT_UART_CMD_LED3_ON                                 0x7004      // LED3 ON
#define AT_UART_CMD_LED3_OFF                                0x7005      // LED3 OFF
#define AT_UART_CMD_LED4_ON                                 0x7006      // LED4 ON
#define AT_UART_CMD_LED4_OFF                                0x7007      // LED4 OFF
#define AT_UART_CMD_LED5_ON                                 0x7008      // LED5 ON
#define AT_UART_CMD_LED5_OFF                                0x7009      // LED5 OFF

// Command 0x99XX - 냉각 검사(전류검사 이후 진행)
#define AT_UART_CMD_COMP_ON                                 0x9900      // 콤프 동작 ON
#define AT_UART_CMD_COMP_OFF                                0x9901      // 콤프 동작 OFF
#define AT_UART_CMD_ICE_MAKE_SYSTEM_COOL                    0x9902      // 제빙 시스템(냉각)
#define AT_UART_CMD_ICE_MAKE_SYSTEM_ICE_MAKE                0x9903      // 제빙 시스템(제빙)
#define AT_UART_CMD_COMP_ICE_MAKE                           0x9904      // 콤프 동작(제빙)


// Command 0xFFXX - 테스트 완료
// (의도치 않은 자동 재시작 예방 로직 탑재)
#define AT_UART_CMD_TEST_COMPLETE                           0xFF00      // 테스트 종료


void Lib_PTA_Mode_Initialize(void);
void NoPowerSet(void);
void PTA_UART_TXD_Start(void);
void UART_ISR_PTA_Tx(void);
void UART_ISR_PTA_Rx(void);
void PTA_UART_Rx_TimeOut(void);
void UART_ISR_PTA_Rx_Error(void);
U8 Get_PTA_CheckEEPROM(void);
void PTA_HeaterTestPowerTimeOut(void);
void TurnOn_Heater(void);
void TurnOff_Heater(void);
void PTA_ControlTimer(void);
void Lib_PTA_Mode_TimeControl(void);
U8 Get_Able_In_PTA_Mode(void);
void Set_PTA_FrontSW_Ver(U8 mu8Ver);
U8 Get_PTA_FrontSW_Ver(void);
void Set_PTA_MainSW_Ver(U8 mu8Ver);
U8 Get_PTA_MainSW_Ver(void);
void Set_PTA_Mode1_End(U8 mu8Data);
U8 Get_PTA_Mode1_End(void);
void Set_PTA_Mode1_LeakTest(U8 mu8Data);
void Set_PTA_Mode1_KeyTest(U8 mu8Data);
void PTA_Mode1_PreTest1End(void);
void PTA_Mode1_PreTest2End(void);
void Set_PTA_Test_End(U8 mu8Data);
U8 Get_PTA_Test_End(void);
void Lib_PTA_Mode_RXD_Data_Control(void);

void Clear_PTA_RxDataBuffer(void);
void Clear_PTA_TxDataBuffer(void);

void PTA_NACK_Send(void);
void PTA_ACK_PowerTest(void);

void PTA_ACK_CMD_2000_Send(void);

void PTA_ACK_CMD_3000_Send(void);

void PTA_ACK_CMD_3010_Send(void);
void PTA_ACK_CMD_3011_Send(void);
void PTA_ACK_CMD_3012_Send(void);
void PTA_ACK_CMD_3013_Send(void);
void PTA_ACK_CMD_3014_Send(void);
void PTA_ACK_CMD_3015_Send(void);
void PTA_ACK_CMD_3016_Send(void);
void PTA_ACK_CMD_3017_Send(void);
void PTA_ACK_CMD_3018_Send(void);
void PTA_ACK_CMD_3019_Send(void);
void PTA_ACK_CMD_301A_Send(void);
void PTA_ACK_CMD_301B_Send(void);
void PTA_ACK_CMD_301C_Send(void);

void PTA_ACK_CMD_3020_Send(void);

void PTA_ACK_CMD_3030_Send(void);

void PTA_ACK_CMD_3040_Send(void);

void PTA_ACK_CMD_3050_Send(void);

void PTA_ACK_CMD_5000_Send(void);
void PTA_ACK_CMD_5010_Send(void);
void PTA_ACK_CMD_5011_Send(void);
void PTA_ACK_CMD_5012_Send(void);
void PTA_ACK_CMD_5013_Send(void);

void PTA_ACK_CMD_5100_Send(void);
void PTA_ACK_CMD_5101_Send(void);
void PTA_ACK_CMD_5102_Send(void);
void PTA_ACK_CMD_5103_Send(void);
void PTA_ACK_CMD_5104_Send(void);
void PTA_ACK_CMD_5105_Send(void);
void PTA_ACK_CMD_5106_Send(void);
void PTA_ACK_CMD_5107_Send(void);
void PTA_ACK_CMD_5108_Send(void);
void PTA_ACK_CMD_5109_Send(void);
void PTA_ACK_CMD_510A_Send(void);
void PTA_ACK_CMD_510B_Send(void);
void PTA_ACK_CMD_510C_Send(void);
void PTA_ACK_CMD_510D_Send(void);
void PTA_ACK_CMD_510E_Send(void);
void PTA_ACK_CMD_510F_Send(void);

void PTA_ACK_CMD_5110_Send(void);
void PTA_ACK_CMD_5111_Send(void);
void PTA_ACK_CMD_5112_Send(void);
void PTA_ACK_CMD_5113_Send(void);
void PTA_ACK_CMD_5114_Send(void);
void PTA_ACK_CMD_5115_Send(void);
void PTA_ACK_CMD_5116_Send(void);
void PTA_ACK_CMD_5117_Send(void);
void PTA_ACK_CMD_5118_Send(void);
void PTA_ACK_CMD_5119_Send(void);
void PTA_ACK_CMD_511A_Send(void);
void PTA_ACK_CMD_511B_Send(void);
void PTA_ACK_CMD_511C_Send(void);
void PTA_ACK_CMD_511D_Send(void);
void PTA_ACK_CMD_511E_Send(void);
void PTA_ACK_CMD_511F_Send(void);

void PTA_ACK_CMD_5120_Send(void);
void PTA_ACK_CMD_5121_Send(void);
void PTA_ACK_CMD_5122_Send(void);
void PTA_ACK_CMD_5123_Send(void);
void PTA_ACK_CMD_5124_Send(void);
void PTA_ACK_CMD_5125_Send(void);
void PTA_ACK_CMD_5126_Send(void);
void PTA_ACK_CMD_5127_Send(void);
void PTA_ACK_CMD_5128_Send(void);
void PTA_ACK_CMD_5129_Send(void);
void PTA_ACK_CMD_512A_Send(void);
void PTA_ACK_CMD_512B_Send(void);
void PTA_ACK_CMD_512C_Send(void);
void PTA_ACK_CMD_512D_Send(void);
void PTA_ACK_CMD_512E_Send(void);
void PTA_ACK_CMD_512F_Send(void);

void PTA_ACK_CMD_5130_Send(void);
void PTA_ACK_CMD_5131_Send(void);
void PTA_ACK_CMD_5132_Send(void);
void PTA_ACK_CMD_5133_Send(void);
void PTA_ACK_CMD_5134_Send(void);
void PTA_ACK_CMD_5135_Send(void);

void PTA_ACK_CMD_5200_Send(void);
void PTA_ACK_CMD_5201_Send(void);
void PTA_ACK_CMD_5202_Send(void);
void PTA_ACK_CMD_5203_Send(void);
void PTA_ACK_CMD_5204_Send(void);
void PTA_ACK_CMD_5205_Send(void);
void PTA_ACK_CMD_5206_Send(void);
void PTA_ACK_CMD_5207_Send(void);
void PTA_ACK_CMD_5208_Send(void);
void PTA_ACK_CMD_5209_Send(void);
void PTA_ACK_CMD_520A_Send(void);
void PTA_ACK_CMD_520B_Send(void);
void PTA_ACK_CMD_520C_Send(void);
void PTA_ACK_CMD_520D_Send(void);
void PTA_ACK_CMD_520E_Send(void);
void PTA_ACK_CMD_520F_Send(void);

void PTA_ACK_CMD_5210_Send(void);
void PTA_ACK_CMD_5211_Send(void);
void PTA_ACK_CMD_5212_Send(void);
void PTA_ACK_CMD_5213_Send(void);
void PTA_ACK_CMD_5214_Send(void);
void PTA_ACK_CMD_5215_Send(void);
void PTA_ACK_CMD_5216_Send(void);
void PTA_ACK_CMD_5217_Send(void);

void PTA_ACK_CMD_5300_Send(void);
void PTA_ACK_CMD_5301_Send(void);
void PTA_ACK_CMD_5302_Send(void);
void PTA_ACK_CMD_5303_Send(void);
void PTA_ACK_CMD_5304_Send(void);
void PTA_ACK_CMD_5305_Send(void);
void PTA_ACK_CMD_5306_Send(void);
void PTA_ACK_CMD_5307_Send(void);

void PTA_ACK_CMD_7000_Send(void);
void PTA_ACK_CMD_7001_Send(void);
void PTA_ACK_CMD_7002_Send(void);
void PTA_ACK_CMD_7003_Send(void);
void PTA_ACK_CMD_7004_Send(void);
void PTA_ACK_CMD_7005_Send(void);
void PTA_ACK_CMD_7006_Send(void);
void PTA_ACK_CMD_7007_Send(void);
void PTA_ACK_CMD_7008_Send(void);
void PTA_ACK_CMD_7009_Send(void);

void PTA_ACK_CMD_9900_Send(void);
void PTA_ACK_CMD_9901_Send(void);
void PTA_ACK_CMD_9902_Send(void);
void PTA_ACK_CMD_9903_Send(void);
void PTA_ACK_CMD_9904_Send(void);

void PTA_ACK_CMD_FF00_Send(void);

void PTA_Mode3_Act(void);

#endif

void Lib_PTA_Mode_Module_Initialize(void);
void Lib_PTA_Mode_Module_1ms_Control(void);
void Lib_PTA_Mode_Module_Control(void);

#endif

