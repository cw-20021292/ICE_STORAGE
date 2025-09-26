/// @file     Lib_Water_PTA_Mode.h
/// @date     2025/06/27
/// @author   Jaejin Ham
/// @brief    ���� �ڵ�ȭ(PTA) �׽�Ʈ ��� ���� Header File

#ifndef _LIB_WATER_PTA_MODE_H_
#define _LIB_WATER_PTA_MODE_H_


/*  PTA �˻� ��� Function ����
     => void PTA_UART_TXD_Start();          // PTA UART TX �����͸� �۽� ������ �� ���� �Լ�(���� �ĺ��ʹ� ���ͷ�Ʈ���� ó��)
     => UART_ISR_PTA_Tx();                  // PTA ��� UART�� TXD ���ͷ�Ʈ �Լ� �ȿ� ���� �Լ�
     => UART_ISR_PTA_Rx();                  // PTA ��� UART�� RXD ���ͷ�Ʈ �Լ� �ȿ� ���� �Լ�
     => UART_ISR_PTA_Rx_Error();            // PTA ��� UART�� RXD Error ���ͷ�Ʈ �Լ� �ȿ� ���� �Լ�
     => mu8Data = Get_PTA_CheckEEPROM();    // PTA �˻� ���Խ� EEPROM���� END ���� ���� �п��� Ȯ���ϴ� �Լ�(PTA �˻�� ������ EEPROM ������)
     => NoPowerSet();                       // ������ ������ �� ��� ���ϸ� OFF���Ѿ� �ϹǷ� ��� ���� OFF��Ű�� �ڵ带 �Լ� ���� �߰��ؾ� �ϴ� �Լ�
     => TurnOff_Heater();                   // ���ѽð� ���� �� ���͸� ������ �� OFF���Ѿ� �ϴ� ���� ��� �߰��ؾ� �ϴ� �Լ�


     => PTA_ACK_CMD_xxxx_Send() �� ���·� ������ �� Command�� ���� �Լ��� ���� ������ �߰��ϸ� ��
*/


#define PTA_TEST_MODE_USE                                   USE

#if (PTA_TEST_MODE_USE == USE)


#define PTA_UART_TXD_DATA_REGISTER                          TXD1
#define PTA_UART_TXD_INTERRUPT_REGISTER                     STMK1
#define PTA_UART_RXD_DATA_REGISTER                          RXD1
#define PTA_UART_RXD_ERROR_INTERRUPT_REGISTER               SIR03
#define PTA_UART_START()                                    R_Config_UART1_Start();
#define PTA_UART_STOP()                                     R_Config_UART1_Stop();

#define PTA_UART_DATA_LENGTH                                50          // PTA ��� UART ��� DATA �ִ� Length

#define PTA_MODE_NONE                                       0
#define PTA_MODE_1                                          1
#define PTA_MODE_2                                          2
#define PTA_MODE_3                                          3

#define PTA_CONTROL_TIME_PERIOD                             100         // PTA �˻� ��� ���� �ð� Ÿ�̸� 100ms@1ms
#define PTA_MODE_IN_TIME_OUT                                300         // PTA �˻� ��� ���� ���� �ð� Ÿ�̸� 30sec@100ms

// Factory Test Protocol
#define AT_RS232_STX                                        0x01
#define AT_RS232_ETX                                        0x04

#define AT_UART_ACK                                         "06"        // ACk Data
#define AT_UART_NACK                                        "15"        // NACK Data

// Command 0x200X - �� ���� Ȯ��
// �˻� ��� ����
// - Data Format : 0x2000 + (0x01 or 0x02)
// - 0x01 : �ڵ��˻� ���� ��� (�˻� 2���)
// - 0x02 : �ڵ��˻� ���� �� ��� (�˻� 1���)
//          : ����Ű �Ǵ� ������� �ڵ� ����
//          : ���� �˻� ���� �Ϸ� �� �˻� 2��� ���� �� �˻��� Ack ����
#define AT_UART_CMD_MODEL_NAME                              0x2000
#define AT_UART_CMD_MODEL_NAME_PRETEST_SKIP                 0x01        // ��� ���Խ� ���� �׽�Ʈ SKIP ���� ������
#define AT_UART_CMD_MODEL_NAME_PRETEST                      0x02        // ��� ���Խ� ���� �׽�Ʈ ���� ������

// Command 0x300X - ������ Ȯ��
#define AT_UART_CMD_BASIC_INFO                              0x3000      // ������ �˻�
#define AT_UART_MODEL_INFO                                  "34"        // �� ����
#define AT_UART_MODEL_CONTRY                                "00"        // ���� ����
#define AT_UART_MODEL_TYPE_CP                               "00"
#define AT_UART_MODEL_TYPE_CHP                              "01"

// Command 0x301X - ��� �˻�
#define AT_UART_CMD_WIFI_INFO                               0x3010      // Wi-Fi ���
#define AT_UART_WIFI_NO_CONNECT                             "00"
#define AT_UART_WIFI_CONNECT                                "01"

#define AT_UART_CMD_EW_UV_INFO                              0x3011      // EW, UV ��� �ǵ��
#define AT_UART_CMD_FLOW_SENSOR_INFO                        0x3012      // �������� �ǵ��
#define AT_UART_CMD_AC_DC_CURRENT_INFO                      0x3013      // AC/DC ����
#define AT_UART_CMD_FAN_INFO                                0x3014      // FAN ����
#define AT_UART_CMD_INFRARED_SENSOR_INFO                    0x3015      // ���ܼ� ���� ����
#define AT_UART_CMD_STIRRING_MOTOR_INFO                     0x3016      // ���� ���� ����
#define AT_UART_CMD_PRESSURE_SENSOR_INFO                    0x3017      // �з� ���� ����
#define AT_UART_CMD_TDS_IN_SENSOR_INFO                      0x3018      // ���� TDS ���� ����
#define AT_UART_CMD_TDS_OUT_SENSOR_INFO                     0x3019      // ��� TDS ���� ����
#define AT_UART_CMD_BOOST_PUMP_INFO                         0x301A      // �ν�Ʈ ���� ����
#define AT_UART_CMD_CIRCULATE_PUMP_INFO                     0x301B      // ��ȯ ���� ����
#define AT_UART_CMD_BRIGHTNESS_SENSOR_INFO                  0x301C      // ���� ���� ����

// Command 0x302X - �µ� ���� �˻�
// 1. �µ����� Hexa �� �µ� ��ȯ�� ����, ���� ǥ��(��� 0XXX, ���� 1XXX), �������۽� �ü��µ� 1�ʴ��� ����, �Ҽ��� 1�ڸ� ǥ��, ���� �� 9999
// 2. �µ����� Hexa �� �µ� ��ȯ�� ����, �������۽� �ü��µ� 1�ʴ��� ����, �Ҽ��� 2�ڸ� ǥ��, ���� �� 9999
#define AT_UART_CMD_TEMP_SENSOR_INFO                        0x3020      // �µ� ���� ����
#define AT_UART_TEMP_SENSOR_ERROR                           "9999"      // �µ� ���� ���� ���°�

// Command 0x303X - ���� ���� ü�� �˻�
// �������� ��ü�� ���� �ǵ��� HW ���� - Active High
#define AT_UART_CMD_WATER_LEVEL_SENSOR_INFO                 0x3030      // ���� ���� ü�� ����
#define AT_UART_LEVEL_SENSOR_NO_CONNECT                     "00"        // ���� ���� ��ü�� ���°�
#define AT_UART_LEVEL_SENSOR_CONNECT                        "01"        // ���� ���� ü�� ���°�

// Command 0x304X - �Ϲ� ���� ü�� �˻�
#define AT_UART_CMD_SENSOR_INFO                             0x3040      // �Ϲ� ���� ü�� ����
#define AT_UART_SENSOR_NO_CONNECT                           "00"        // �Ϲ� ���� ��ü�� ���°�
#define AT_UART_SENSOR_CONNECT                              "01"        // �Ϲ� ���� ü�� ���°�

// Command 0x304X - ü�� �ǵ�� �˻�
#define AT_UART_CMD_FEEDBACK_INFO                           0x3050

// Command 0x50XX - ���� ���� �˻�
// ��� Ʈ���� ������ ���� �¼����� ���� ������ ��ǰ �� Off Time out �� ���� �̵��� ����
// => �����¼� Type: 0.25 Sec / �Ϲݿ¼� Type : 7 Sec
#define AT_UART_CMD_NO_POWER_SET                            0x5000      // ������ ����
#define AT_UART_CMD_AC_HEATER_ON                            0x5010      // �¼� ���� ON
#define AT_UART_CMD_AC_HEATER_OFF                           0x5011      // �¼� ���� OFF
#define AT_UART_CMD_DE_ICE_HEATER_ON                        0x5012      // Ż�� ���� ON
#define AT_UART_CMD_DE_ICE_HEATER_OFF                       0x5013      // Ż�� ���� OFF

// Command 0x51XX - ��� �˻�
#define AT_UART_CMD_ROOM_IN_VALVE_ON                        0x5100      // ���� �Լ� ��� ON
#define AT_UART_CMD_ROOM_IN_VALVE_OFF                       0x5101      // ���� �Լ� ��� OFF
#define AT_UART_CMD_COLD_IN_VALVE_ON                        0x5102      // �ü� �Լ� ��� ON
#define AT_UART_CMD_COLD_IN_VALVE_OFF                       0x5103      // �ü� �Լ� ��� OFF
#define AT_UART_CMD_HOT_IN_VALVE_ON                         0x5104      // �¼� �Լ� ��� ON
#define AT_UART_CMD_HOT_IN_VALVE_OFF                        0x5105      // �¼� �Լ� ��� OFF
#define AT_UART_CMD_ROOM_OUT_VALVE_ON                       0x5106      // ���� ���� ��� ON
#define AT_UART_CMD_ROOM_OUT_VALVE_OFF                      0x5107      // ���� ���� ��� OFF
#define AT_UART_CMD_COLD_OUT_VALVE_ON                       0x5108      // �ü� ���� ��� ON
#define AT_UART_CMD_COLD_OUT_VALVE_OFF                      0x5109      // �ü� ���� ��� OFF
#define AT_UART_CMD_HOT_OUT_VALVE_ON                        0x510A      // �¼� ���� ��� ON
#define AT_UART_CMD_HOT_OUT_VALVE_OFF                       0x510B      // �¼� ���� ��� OFF
#define AT_UART_CMD_TWO_WAY_VALVE_ON                        0x510C      // 2Way ��� ON
#define AT_UART_CMD_TWO_WAY_VALVE_OFF                       0x510D      // 2Way ��� OFF
#define AT_UART_CMD_INPUT_FEED_VALVE_ON                     0x510E      // ���� �Լ� Feed ��� ON
#define AT_UART_CMD_INPUT_FEED_VALVE_OFF                    0x510F      // ���� �Լ� Feed ��� OFF
#define AT_UART_CMD_INPUT_NOS_VALVE_ON                      0x5110      // ���� �Լ� NOS ��� ON
#define AT_UART_CMD_INPUT_NOS_VALVE_OFF                     0x5111      // ���� �Լ� NOS ��� OFF
#define AT_UART_CMD_OUTPUT_DRAIN_NOS_VALVE_ON               0x5112      // ��� �巹�� NOS ��� ON
#define AT_UART_CMD_OUTPUT_DRAIN_NOS_VALVE_OFF              0x5113      // ��� �巹�� NOS ��� OFF
#define AT_UART_CMD_HOT_DRAIN_VALVE_ON                      0x5114      // �¼� �巹�� ��� ON
#define AT_UART_CMD_HOT_DRAIN_VALVE_OFF                     0x5115      // �¼� �巹�� ��� OFF
#define AT_UART_CMD_ICE_TRAY_VALVE_ON                       0x5116      // ���̽� Ʈ���� ��� ON
#define AT_UART_CMD_ICE_TRAY_VALVE_OFF                      0x5117      // ���̽� Ʈ���� ��� OFF
#define AT_UART_CMD_DRAIN_VALVE_ON                          0x5118      // �巹�� ��� ON
#define AT_UART_CMD_DRAIN_VALVE_OFF                         0x5119      // �巹�� ��� OFF
#define AT_UART_CMD_EW_DRAIN_VALVE_ON                       0x511A      // ��� �巹�� ��� ON
#define AT_UART_CMD_EW_DRAIN_VALVE_OFF                      0x511B      // ��� �巹�� ��� OFF
#define AT_UART_CMD_EW_INPUT_VALVE_ON                       0x511C      // ��� �Լ� ��� ON
#define AT_UART_CMD_EW_INPUT_VALVE_OFF                      0x511D      // ��� �Լ� ��� OFF
#define AT_UART_CMD_SODA_DRAIN_VALVE_ON                     0x511E      // ź�� ��� ��� ON
#define AT_UART_CMD_SODA_DRAIN_VALVE_OFF                    0x511F      // ź�� ��� ��� OFF
#define AT_UART_CMD_SODA_OUTPUT_VALVE_ON                    0x5120      // ź�� ��� ��� ON
#define AT_UART_CMD_SODA_OUTPUT_VALVE_OFF                   0x5121      // ź�� ��� ��� OFF
#define AT_UART_CMD_REFRIGERANT_CONVERSION_VALVE_ON         0x5122      // �ø� ��ȯ ��� ON
#define AT_UART_CMD_REFRIGERANT_CONVERSION_VALVE_OFF        0x5123      // �ø� ��ȯ ��� OFF
#define AT_UART_CMD_HOT_GAS_VALVE_ON                        0x5124      // �ְ��� ��� ON
#define AT_UART_CMD_HOT_GAS_VALVE_OFF                       0x5125      // �ְ��� ��� OFF
#define AT_UART_CMD_ION_EXCHANGE_REGIN_VALVE_ON             0x5126      // �̿� ��ȯ ���� ��� ON
#define AT_UART_CMD_ION_EXCHANGE_REGIN_VALVE_OFF            0x5127      // �̿� ��ȯ ���� ��� OFF
#define AT_UART_CMD_INO_VALVE_ON                            0x5128      // �̳� ���� ��� ON
#define AT_UART_CMD_INO_VALVE_OFF                           0x5129      // �̳� ���� ��� OFF
#define AT_UART_CMD_HOT_TANK_OVERFLOW_VALVE_ON              0x512A      // �¼� ��ũ �����÷ο� ��� ON
#define AT_UART_CMD_HOT_TANK_OVERFLOW_VALVE_OFF             0x512B      // �¼� ��ũ �����÷ο� ��� OFF
#define AT_UART_CMD_COLD_TANK_OVERFLOW_VALVE_ON             0x512C      // �ü� ��ũ �����÷ο� ��� ON
#define AT_UART_CMD_COLD_TANK_OVERFLOW_VALVE_OFF            0x512D      // �ü� ��ũ �����÷ο� ��� OFF
#define AT_UART_CMD_TOTAL_OUTPUT_VALVE_ON                   0x512E      // �ÿ��� ���� ��� ON
#define AT_UART_CMD_TOTAL_OUTPUT_VALVE_OFF                  0x512F      // �ÿ��� ���� ��� OFF
#define AT_UART_CMD_ICE_COLD_TANK_INPUT_VALVE_ON            0x5130      // ������ũ ���� ��� ON
#define AT_UART_CMD_ICE_COLD_TANK_INPUT_VALVE_OFF           0x5131      // ������ũ ���� ��� OFF
#define AT_UART_CMD_PRESSURIZED_INPUT_VALVE_ON              0x5132      // ���м� �Լ� ��� ON
#define AT_UART_CMD_PRESSURIZED_INPUT_VALVE_OFF             0x5133      // ���м� �Լ� ��� OFF
#define AT_UART_CMD_PRESSURIZED_DRAIN_VALVE_ON              0x5134      // ���м� ��� ��� ON
#define AT_UART_CMD_PRESSURIZED_DRAIN_VALVE_OFF             0x5135      // ���м� ��� ��� OFF

// Command 0x52XX - ���� �˻�
#define AT_UART_CMD_FAN_ON                                  0x5200      // �ð��� ON
#define AT_UART_CMD_FAN_OFF                                 0x5201      // �ð��� OFF
#define AT_UART_CMD_STEP_MOTOR_ON                           0x5202      // ���ܸ��� ON
#define AT_UART_CMD_STEP_MOTOR_OFF                          0x5203      // ���ܸ��� OFF
#define AT_UART_CMD_ICE_MAKE_MOTOR_ON                       0x5204      // ���̽� ���� ���� ON
#define AT_UART_CMD_ICE_MAKE_MOTOR_OFF                      0x5205      // ���̽� ���� ���� OFF
#define AT_UART_CMD_ICE_TRAY_MOTOR_ON                       0x5206      // ���̽� Ʈ���� ���� ON
#define AT_UART_CMD_ICE_TRAY_MOTOR_OFF                      0x5207      // ���̽� Ʈ���� ���� OFF
#define AT_UART_CMD_ICE_DOOR_MOTOR_ON                       0x5208      // ���̽� ���� ���� ON
#define AT_UART_CMD_ICE_DOOR_MOTOR_OFF                      0x5209      // ���̽� ���� ���� OFF
#define AT_UART_CMD_ICE_OUTPUT_CW_MOTOR_ON                  0x520A      // ���̽� ���� ����-�� ON
#define AT_UART_CMD_ICE_OUTPUT_CW_MOTOR_OFF                 0x520B      // ���̽� ���� ����-�� OFF
#define AT_UART_CMD_ICE_OUTPUT_CCW_MOTOR_ON                 0x520C      // ���̽� ���� ����-�� ON
#define AT_UART_CMD_ICE_OUTPUT_CCW_MOTOR_OFF                0x520D      // ���̽� ���� ����-�� OFF
#define AT_UART_CMD_SELECT_BAR_MOTOR_ON                     0x520E      // ����Ʈ�� ���� ON
#define AT_UART_CMD_SELECT_BAR_MOTOR_OFF                    0x520F      // ����Ʈ�� ���� OFF
#define AT_UART_CMD_SWING_BAR_MOTOR_ON                      0x5210      // ������ ���� ON
#define AT_UART_CMD_SWING_BAR_MOTOR_OFF                     0x5211      // ������ ���� OFF
#define AT_UART_CMD_FAUCET_MOTOR_CW_ON                      0x5212      // �Ŀ�� ���� ������ ON
#define AT_UART_CMD_FAUCET_MOTOR_CW_OFF                     0x5213      // �Ŀ�� ���� ������ OFF
#define AT_UART_CMD_STIRRING_MOTOR_ON                       0x5214      // ���� ���� ON
#define AT_UART_CMD_STIRRING_MOTOR_OFF                      0x5215      // ���� ���� OFF
#define AT_UART_CMD_FAUCET_MOTOR_CCW_ON                     0x5216      // �Ŀ�� ���� ������ ON
#define AT_UART_CMD_FAUCET_MOTOR_CCW_OFF                    0x5217      // �Ŀ�� ���� ������ OFF

// Command 0x53XX - ���� �˻�
#define AT_UART_CMD_BOOST_PUMP_ON                           0x5300      // �ν�Ʈ ���� ON
#define AT_UART_CMD_BOOST_PUMP_OFF                          0x5301      // �ν�Ʈ ���� OFF
#define AT_UART_CMD_DRAIN_PUMP_ON                           0x5302      // �巹�� ���� ON
#define AT_UART_CMD_DRAIN_PUMP_OFF                          0x5303      // �巹�� ���� OFF
#define AT_UART_CMD_COLD_CIRCULATE_PUMP_ON                  0x5304      // �ü� ��ȯ ���� ON
#define AT_UART_CMD_COLD_CIRCULATE_PUMP_OFF                 0x5305      // �ü� ��ȯ ���� OFF
#define AT_UART_CMD_HOT_OUTPUT_PUMP_ON                      0x5306      // �¼� ���� ���� ON
#define AT_UART_CMD_HOT_OUTPUT_PUMP_OFF                     0x5307      // �¼� ���� ���� OFF

// Command 0x60XX - Ű �˻�

// Command 0x70XX - ���÷��� �˻�
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

// Command 0x99XX - �ð� �˻�(�����˻� ���� ����)
#define AT_UART_CMD_COMP_ON                                 0x9900      // ���� ���� ON
#define AT_UART_CMD_COMP_OFF                                0x9901      // ���� ���� OFF
#define AT_UART_CMD_ICE_MAKE_SYSTEM_COOL                    0x9902      // ���� �ý���(�ð�)
#define AT_UART_CMD_ICE_MAKE_SYSTEM_ICE_MAKE                0x9903      // ���� �ý���(����)
#define AT_UART_CMD_COMP_ICE_MAKE                           0x9904      // ���� ����(����)


// Command 0xFFXX - �׽�Ʈ �Ϸ�
// (�ǵ�ġ ���� �ڵ� ����� ���� ���� ž��)
#define AT_UART_CMD_TEST_COMPLETE                           0xFF00      // �׽�Ʈ ����


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

