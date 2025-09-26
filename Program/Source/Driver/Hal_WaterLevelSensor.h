/// @file     Hal_WaterLevelSensor.h
/// @date     2025/03/14
/// @author   Jaejin Ham
/// @brief    Water Level Sensor Control file


#ifndef _HAL_WATER_LEVEL_SENSOR_H_
#define _HAL_WATER_LEVEL_SENSOR_H_


/*  ���� ���� ���� Function ����
    => Get_WaterLevel_Status(Level ID);                // ID�� �ش��ϴ� ���������� ���°� Ȯ��
    => Get_ElecLevel_Status();                         // ���غ� ���������� ���°� Ȯ��

    ��� ��)
    u8Return = Get_WaterLevel_Status(LOW_SENSOR_ID);   // LOW SENSOR ID�� ���� Ȯ�� ���� ��û, mu8Return : 1(Detect), 0(Undetect)
    u8Return = Get_ElecLevel_Status();                 // ���غ� ���������� ���� Ȯ�� ���� ��û, mu8Return : 1(Detect), 0(Undetect)
*/


// Water Level Sensor �ִ� 10�� ��� ����
typedef enum
{
    LEVEL_ID_NONE = 0,                                  // 0 :
    LEVEL_ID_COLD_LOW,                                  // 1 : �ü� ������
    LEVEL_ID_LOW,                                       // 2 : ������
    LEVEL_ID_MODDLE,                                    // 3 : �߼���
    LEVEL_ID_HIGH,                                      // 4 : ������
    LEVEL_ID_OVERFLOW,                                  // 5 : �����÷ο�
    LEVEL_ID_HOT_HIGH,                                  // 6 : �¼� ������
    LEVEL_ID_HOT_OVERFLOW,                              // 7 : �¼� �����÷ο�
    LEVEL_ID_ICD_COLD_WATER,                            // 8 : ���� ��ũ ����
    LEVEL_ID_MAX                                        // 11 Max, 11�� ������ �ȵ�
} ELevelSensorID_T;

#define LEVEL_SENSOR_COUNT                              5       // �Ϲ� ���� ���� ���� - MAX 10

#if (LEVEL_SENSOR_COUNT > 0)
// Level Sensor port �Ҵ�
// ������ skip���� ��ġ�Ͽ� ����ϸ�, �̻�� port�� 0���� ó��
// ������ ID�� �����ϰ� ��ġ
#define PORT_DEFINE_LEVEL_SENSOR_1                      P8_bit.no0  // ���� ���� ID 1 port
#define PORT_DEFINE_LEVEL_SENSOR_2                      P8_bit.no1
#define PORT_DEFINE_LEVEL_SENSOR_3                      P8_bit.no2
#define PORT_DEFINE_LEVEL_SENSOR_4                      P8_bit.no3
#define PORT_DEFINE_LEVEL_SENSOR_5                      P8_bit.no4
#define PORT_DEFINE_LEVEL_SENSOR_6                      0
#define PORT_DEFINE_LEVEL_SENSOR_7                      0
#define PORT_DEFINE_LEVEL_SENSOR_8                      0
#define PORT_DEFINE_LEVEL_SENSOR_9                      0
#define PORT_DEFINE_LEVEL_SENSOR_10                     0

// ���� ���� Ȯ�� üũ �ð�
#define TIME_DEFINE_LEVEL_SENSOR_1_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_2_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_3_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_4_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_5_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_6_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_7_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_8_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_9_CHATTERING_TIME      20          // 2sec@100ms
#define TIME_DEFINE_LEVEL_SENSOR_10_CHATTERING_TIME     20          // 2sec@100ms

// ���غ� ���� ������ 1���� ��� �����ϰ� �Ϲ� ���� ������ ������ üũ��
#define ELECTRODE_LEVEL_SENSOR_USE                      USE      // ���غ� ���� ���� ��� ���� - 0(No Use), 1(Use)

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)
#define ID_DEFINE_ELECTRODE_LEVEL_SENSOR_ADC_ID         ADC_ID_ELEC_LEVEL_HOT_TANK
#define PORT_DEFINE_ELEC_LEVEL_SENSOR_POWER_1           P9_bit.no0
#define PORT_DEFINE_ELEC_LEVEL_SENSOR_POWER_2           P9_bit.no1

#define TIME_DEFINE_ELEC_LEVEL_SENSOR_CHATTERING_TIME   10          // 1sec@100ms

#define VALUE_DEFINE_ELEC_LEVE_SENSOR_HIGH_LEVEL_VALUE  650         // ���غ� ���� ���� ���� ���� AD��
#define VALUE_DEFINE_ELEC_LEVE_SENSOR_LOW_LEVEL_VALUE   800         // ���غ� ���� �̰��� ���� ���� AD��
#endif


#define LEVEL_SENSOR_ID_MAX_COUNT               LEVEL_ID_MAX

// Level Sensor Port Define
#define P_LEVEL_SENSOR_1                        PORT_DEFINE_LEVEL_SENSOR_1
#define GET_STATUS_LEVEL_SENSOR_1()             ( P_LEVEL_SENSOR_1 )

#define P_LEVEL_SENSOR_2                        PORT_DEFINE_LEVEL_SENSOR_2
#define GET_STATUS_LEVEL_SENSOR_2()             ( P_LEVEL_SENSOR_2 )

#define P_LEVEL_SENSOR_3                        PORT_DEFINE_LEVEL_SENSOR_3
#define GET_STATUS_LEVEL_SENSOR_3()             ( P_LEVEL_SENSOR_3 )

#define P_LEVEL_SENSOR_4                        PORT_DEFINE_LEVEL_SENSOR_4
#define GET_STATUS_LEVEL_SENSOR_4()             ( P_LEVEL_SENSOR_4 )

#define P_LEVEL_SENSOR_5                        PORT_DEFINE_LEVEL_SENSOR_5
#define GET_STATUS_LEVEL_SENSOR_5()             ( P_LEVEL_SENSOR_5 )

#define P_LEVEL_SENSOR_6                        PORT_DEFINE_LEVEL_SENSOR_6
#define GET_STATUS_LEVEL_SENSOR_6()             ( P_LEVEL_SENSOR_6 )

#define P_LEVEL_SENSOR_7                        PORT_DEFINE_LEVEL_SENSOR_7
#define GET_STATUS_LEVEL_SENSOR_7()             ( P_LEVEL_SENSOR_7 )

#define P_LEVEL_SENSOR_8                        PORT_DEFINE_LEVEL_SENSOR_8
#define GET_STATUS_LEVEL_SENSOR_8()             ( P_LEVEL_SENSOR_8 )

#define P_LEVEL_SENSOR_9                        PORT_DEFINE_LEVEL_SENSOR_9
#define GET_STATUS_LEVEL_SENSOR_9()             ( P_LEVEL_SENSOR_9 )

#define P_LEVEL_SENSOR_10                       PORT_DEFINE_LEVEL_SENSOR_10
#define GET_STATUS_LEVEL_SENSOR_10()             ( P_LEVEL_SENSOR_10 )

#define LEVEL_SENSOR_CONTROL_TIME_PERIOD        100         // 100ms@1ms

#define LEVEL_SENSOR_1_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_1_CHATTERING_TIME
#define LEVEL_SENSOR_2_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_2_CHATTERING_TIME
#define LEVEL_SENSOR_3_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_3_CHATTERING_TIME
#define LEVEL_SENSOR_4_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_4_CHATTERING_TIME
#define LEVEL_SENSOR_5_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_5_CHATTERING_TIME
#define LEVEL_SENSOR_6_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_6_CHATTERING_TIME
#define LEVEL_SENSOR_7_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_7_CHATTERING_TIME
#define LEVEL_SENSOR_8_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_8_CHATTERING_TIME
#define LEVEL_SENSOR_9_CHATTERING_TIME          TIME_DEFINE_LEVEL_SENSOR_9_CHATTERING_TIME
#define LEVEL_SENSOR_10_CHATTERING_TIME         TIME_DEFINE_LEVEL_SENSOR_10_CHATTERING_TIME

// Level Sensor Detect Data
#define LEVEL_SENSOR_1_DETECT                   0x0001
#define LEVEL_SENSOR_2_DETECT                   0x0002
#define LEVEL_SENSOR_3_DETECT                   0x0004
#define LEVEL_SENSOR_4_DETECT                   0x0008
#define LEVEL_SENSOR_5_DETECT                   0x0010
#define LEVEL_SENSOR_6_DETECT                   0x0020
#define LEVEL_SENSOR_7_DETECT                   0x0040
#define LEVEL_SENSOR_8_DETECT                   0x0080
#define LEVEL_SENSOR_9_DETECT                   0x0100
#define LEVEL_SENSOR_10_DETECT                  0x0200
#endif

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)
#define ELECTRODE_LEVEL_SENSOR_ADC_ID           ID_DEFINE_ELECTRODE_LEVEL_SENSOR_ADC_ID
#define P_ELEC_LEVEL_SENSOR_POWER_1             PORT_DEFINE_ELEC_LEVEL_SENSOR_POWER_1
#define TURN_ON_ELEC_LEVEL_SENSOR_POWER_1()     { P_ELEC_LEVEL_SENSOR_POWER_1 = 1; }
#define TURN_OFF_ELEC_LEVEL_SENSOR_POWER_1()    { P_ELEC_LEVEL_SENSOR_POWER_1 = 0; }
#define GET_STATUS_ELEC_LEVEL_SENSOR_POWER_1()  ( P_ELEC_LEVEL_SENSOR_POWER_1 )
#define P_ELEC_LEVEL_SENSOR_POWER_2             PORT_DEFINE_ELEC_LEVEL_SENSOR_POWER_2
#define TURN_ON_ELEC_LEVEL_SENSOR_POWER_2()     { P_ELEC_LEVEL_SENSOR_POWER_2 = 1; }
#define TURN_OFF_ELEC_LEVEL_SENSOR_POWER_2()    { P_ELEC_LEVEL_SENSOR_POWER_2 = 0; }
#define GET_STATUS_ELEC_LEVEL_SENSOR_POWER_2()  ( P_ELEC_LEVEL_SENSOR_POWER_2 )

#define ELEC_LEVEL_SENSOR_CHATTERING_TIME       TIME_DEFINE_ELEC_LEVEL_SENSOR_CHATTERING_TIME

#define ELEC_LEVEL_SENSOR_HIGH_LEVEL_VALUE      VALUE_DEFINE_ELEC_LEVE_SENSOR_HIGH_LEVEL_VALUE
#define ELEC_LEVEL_SENSOR_LOW_LEVEL_VALUE       VALUE_DEFINE_ELEC_LEVE_SENSOR_LOW_LEVEL_VALUE

#define ELEC_LEVEL_SENSOR_CONTROL_TIME_PERIOD   100         // 100ms@1ms
#define ELEC_LEVEL_SENSOR_CHECK_TIME_PERIOD     100         // 100ms@1ms
#define ELEC_LEVEL_SENSOR_CHECK_DELAY_TIME      10          // 10ms@1ms
#endif


void Hal_LevelSensor_Initialize(void);
void LevelSensor_ControlTimer(void);

#if (LEVEL_SENSOR_COUNT > 0)
void Reset_LevelSensorChatteringTime(U8 mu8LevelID);
void Hal_Read_LevelSensor(void);
void LevelSensorControl(void);
void ConfirmLevelSensorData(U8 mu8LevelID, U8 mu8Value);
U8 Get_WaterLevel_Status(U8 mu8SensorID);
#endif

#if (ELECTRODE_LEVEL_SENSOR_USE == USE)
void Elec_Level_CheckControl(void);
void Elec_LevelSensorConfirm(void);
U8 Get_ElecLevel_Status(void);
#endif


void Hal_LevelSensor_Module_Initialize(void);
void Hal_LevelSensor_Module_1ms_Control(void);
void Hal_LevelSensor_Module_Control(void);

#endif

