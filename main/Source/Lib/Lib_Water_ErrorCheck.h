/// @file     Lib_Water_ErrorCheck.h
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    ������ Error ���� ���� file


#ifndef _LIB_WATER_ERROR_CHECK_H_
#define _LIB_WATER_ERROR_CHECK_H_


/*  Lib Function ����
    => Set_FlowMeterErrorCheckStart(mu8Status);         // ���� ���� ���� üũ ���� ���� ���� - 0(�̽���), 1(����)
    => Get_FlowMeterErrorCheckStart();                  // ���� ���� ���� üũ ���� ���� Ȯ�� - 0(�̽���), 1(����)
    => Set_FlowMeterErrorCheckDelayTime(mu16Time);      // ���� ���� ���� üũ ���� �� Delay �ð� ���� - 100ms����
    => Get_FlowMeterErrorCheckDelayTime();              // ���� ���� ���� üũ ���� �� Delay �ð� Ȯ�� - 100ms����

    => Set_EffluentCloggingErrorCheckStart(mu8Status);  // ���� ���� ���� üũ ���� ���� ���� - 0(�̽���), 1(����)
    => Get_EffluentCloggingErrorCheckStart();           // ���� ���� ���� üũ ���� ���� Ȯ�� - 0(�̽���), 1(����)

    => Set_HotCutOffCheckStart(mu8Status);              // �¼� �ܼ� �� ��� ���� ���� üũ ���� ���� ���� - 0(�̽���), 1(����)
    => Get_HotCutOffCheckStart();                       // �¼� �ܼ� �� ��� ���� ���� üũ ���� ���� Ȯ�� - 0(�̽���), 1(����)

    => Get_ErrorDisplayCode();                          // �켱 ������ ���� ���� ǥ�õǾ�� �ϴ� ������ �ش��ϴ� ���÷��� ǥ�� Code Ȯ��
    => Get_ErrorOccuredCheck();                         // ���� �߻��� �ϳ��� �߻��� ���� �ִ��� Ȯ��
    => Get_ErrorStatus(mu8ID);                          // ID�� �ش��ϴ� ���� �߻� ���� Ȯ�� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
    => Get_ErrorDetect(mu8ID);                          // ID�� �ش��ϴ� ���� ���� ���� Ȯ��(������ �̹߻�) - 0(FALSE, �̰���), 1(TRUE, ����)

    => Get_ErrorAboutWaterInput();                      // �Լ� ���۰� ���õ� ���� �߻� ���� Ȯ�� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
    => Get_AirTempError();                              // �ܱ� �µ� ���� ���� �߻� ���� Ȯ�� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
    => Get_HotError();                                  // �¼� ���� ���� �߻� ���� Ȯ�� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
    => Get_ColdError();                                 // �ü� ���� ���� �߻� ���� Ȯ�� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
    => Get_ErrorAboutIceMaking();                       // ���� ���� ���� �߻� ���� Ȯ�� - 0(FALSE, �̹߻�), 1(TRUE, �߻�)
*/



/// @brief      ������ Error Code List(�̻�� ���� ���� ����)
/// @details    ������ Error Code List�� �߻� �켱 ���� ��� �ۼ��Ѵ�
typedef enum
{
    ERROR_ID_NONE_E00 = 0,                              // 0 : Error None
    ERROR_ID_LEAKAGE_E01,                               // 1 : ����
    ERROR_ID_FLOW_SENSOR_2_E06,                         // 2 : ��������2
    ERROR_ID_FLOW_SENSOR_1_E05,                         // 3 : ��������1
    ERROR_ID_WATER_CUT_OFF_E09,                         // 4 : �ܼ�
    ERROR_ID_ROOM_OVERHEAT_E49,                         // 5 : ���� ����
    ERROR_ID_FILTER_CLOGGING_E07,                       // 6 : ���� ����
    ERROR_ID_EFFLUENT_CLOGGING_E04,                     // 7 : ���� ����(��������3)
    ERROR_ID_HOT_CUT_OFF_E08,                           // 8 : ���� �¼� �ܼ� �� ��� ����
    ERROR_ID_BOOST_PUMP_E95,                            // 9 : �ν�Ʈ����(����� ����)
    ERROR_ID_PRESSURE_SENSOR_E97,                       // 10 : �з� ����
    ERROR_ID_TRAY_MOTOR_E96,                            // 11 : ���� Ʈ���� ����
    ERROR_ID_MICRO_SW_ALL_DETECT_E61,                   // 12 : ����ũ�� SW ���� ����
    ERROR_ID_MICRO_SW_ICE_MAKING_E62,                   // 13 : ����ũ�� SW �̵�(����)
    ERROR_ID_MICRO_SW_DE_ICE_E63,                       // 14 : ����ũ�� SW �̵�(Ż��)
    ERROR_ID_DRAIN_PUMP_E60,                            // 15 : �巹�� ����
    ERROR_ID_SODA_PUMP_E94,                             // 16 : ź�� ����
    ERROR_ID_ABNORMAL_EFFLUENT_E03,                     // 17 : ���� ���� �̻�
    ERROR_ID_LOW_SENSOR_E11,                            // 18 : ������ ����
    ERROR_ID_MIDDLE_SENSOR_E12,                         // 19 : �߼��� ����
    ERROR_ID_HIGH_SENSOR_E13,                           // 20 : ������ ����
    ERROR_ID_COMPLEX_SENSOR_E14,                        // 21 : ���� ���� ����(�߼��� ���� ���)
    ERROR_ID_LOW_MIDDLE_SENSOR_E15,                     // 22 : ��, �߼��� ���� ����
    ERROR_ID_MIDDLE_HIGH_SENSOR_E16,                    // 23 : ��, ������ ���� ����
    ERROR_ID_OVER_FLOW_E17,                             // 24 : OVER FLOW ����(�߼��� �ִ� ���)
    ERROR_ID_FEED_VALVE_E10,                            // 25 : �ǵ� ���
    ERROR_ID_HOT_HIGH_SENSOR_E33,                       // 26 : �¼� ������ ����
    ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20,            // 27 : ���� ��ũ ���� ����
    ERROR_ID_COLD_LOW_SENSOR_E21,                       // 28 : �ü� ������ ����
    ERROR_ID_COLD_HIGH_SENSOR_E23,                      // 29 : �ü� ������ ����
    ERROR_ID_COLD_COMPLEX_SENSOR_E24,                   // 30 : �ü� ���� ����
    ERROR_ID_ROOM_TEMP_2_E52,                           // 31 : ����2 �µ� ����
    ERROR_ID_ROOM_TEMP_1_E42,                           // 32 : ����1 �µ� ����
    ERROR_ID_TANK_HOT_TEMP_E45,                         // 33 : �¼� �µ� ����(��ũ��)
    ERROR_ID_HOT_IN_TEMP_E46,                           // 34 : �¼� �Լ� �µ� ����(�����¼�)
    ERROR_ID_HOT_OUT_TEMP_E47,                          // 35 : �¼� ��� �µ� ����(�����¼�)
    ERROR_ID_HOT_BODY_TEMP_E48,                         // 36 : ���� ���� �µ� ����(�����¼�)
    ERROR_ID_COLD_TEMP_1_E44,                           // 37 : �ü� �µ� ����1
    ERROR_ID_COLD_TEMP_2_E54,                           // 38 : �ü� �µ� ����2
    ERROR_ID_ICE_TRAY_INPUT_TEMP_E56,                   // 39 : ���̽� Ʈ���� �Լ� �µ� ����
    ERROR_ID_AIR_TEMP_1_E43,                            // 40 : �ܱ� �µ� ����1
    ERROR_ID_AIR_TEMP_2_E53,                            // 41 : �ܱ� �µ� ����2
    ERROR_ID_TEM_HEATSINK_TEMP_E25,                     // 42 : TEM �濭�� �µ� ����
    ERROR_ID_HOT_OVERHEAT_E40,                          // 43 : �¼� ����
    ERROR_ID_HOT_PUMP_E30,                              // 44 : �¼� ����
    ERROR_ID_BLDC_COMP_COMMUNICATION_E27,               // 45 : BLDC ���� ��� �ҷ�
    ERROR_ID_COMP_ACT_E29,                              // 46 : �Ϲ� ���� ���� �ҷ�
    ERROR_ID_BLDC_COMP_ACT_1_E81,                       // 47 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x81
    ERROR_ID_BLDC_COMP_ACT_2_E82,                       // 48 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x82
    ERROR_ID_BLDC_COMP_ACT_3_E83,                       // 49 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x83
    ERROR_ID_BLDC_COMP_ACT_4_E84,                       // 50 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x84
    ERROR_ID_BLDC_COMP_ACT_5_E85,                       // 51 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x85
    ERROR_ID_BLDC_COMP_ACT_6_E86,                       // 52 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x86
    ERROR_ID_CIRCULATE_PUMP_E26,                        // 53 : ��ȯ ����
    ERROR_ID_STIRRING_MOTOR_E28,                        // 54 : ���� ����
    ERROR_ID_EW_MODULE_E71,                             // 55 : EW ��� ���
    ERROR_ID_EW_DRAIN_PUMP_E72,                         // 56 : EW ��� �巹�� ����
    ERROR_ID_UV_1_E73,                                  // 57 : UV1(���� ��ũ)
    ERROR_ID_UV_2_E74,                                  // 58 : UV2(�ü� ��ũ ��)
    ERROR_ID_UV_3_E75,                                  // 59 : UV3(���� ��ũ)
    ERROR_ID_UV_4_E76,                                  // 60 : UV4(���� Ʈ����)
    ERROR_ID_UV_5_E77,                                  // 61 : UV5(���� �Ŀ��)
    ERROR_ID_UV_6_E78,                                  // 62 : UV6(���� �Ŀ��)
    ERROR_ID_UV_7_E79,                                  // 63 : UV7(�ü� ��ũ ��)
    ERROR_ID_TDS_IN_TEMP_E90,                           // 64 : TDS IN �µ� ����
    ERROR_ID_TDS_OUT_TEMP_E91,                          // 65 : TDS OUT �µ� ����
    ERROR_ID_TDS_IN_E92,                                // 66 : TDS IN ����
    ERROR_ID_TDS_OUT_E93,                               // 67 : TDS OUT ����
    ERROR_ID_INPUT_CONTAMINATION_E98,                   // 68 : �Լ� ����
    ERROR_ID_DECONTAMINATION_E99,                       // 69 : ������ �ҷ�
    ERROR_ID_MAX
} Error_Data_T;


/// @brief      ���� �߻� ǥ�� Code List(�̻�� ���� ���� ����)
/// @details    ���� �߻� ǥ�� Code �����͵��� ����Ʈ�� �����Ѵ�
__far const static U8 gu8ErrorDisplayCode[ERROR_ID_MAX] = {
    0x00,           // 0 : Error None
    0x01,           // 1 : ����
    0x06,           // 2 : ��������2(��Ʈ�� �𵨿� ����, INNO �Ĵ� ��ġ, ���� ������)
    0x05,           // 3 : ��������1(��Ʈ�� �𵨿� ����, NEO-RO ���� ��ġ, �ܼ� �˻��)
    0x09,           // 4 : �ܼ�(��������)
    0x49,           // 5 : ���� ����
    0x07,           // 6 : ���� ����
    0x04,           // 7 : ���� ����(��������3)
    0x08,           // 8 : ���� �¼� �ܼ� �� ��� ����
    0x95,           // 9 : �ν�Ʈ����(����� ����)
    0x97,           // 10 : �з� ����
    0x96,           // 11 : ���� Ʈ���� ����
    0x61,           // 12 : ����ũ�� SW ���� ����
    0x62,           // 13 : ����ũ�� SW �̵�(����)
    0x63,           // 14 : ����ũ�� SW �̵�(Ż��)
    0x60,           // 15 : �巹�� ����
    0x94,           // 16 : ź�� ����
    0x03,           // 17 : ���� ���� �̻�
    0x11,           // 18 : ������ ����
    0x12,           // 19 : �߼��� ����
    0x13,           // 20 : ������ ����
    0x14,           // 21 : ���� ���� ����(�߼��� ���� ���)
    0x15,           // 22 : ��, �߼��� ���� ����
    0x16,           // 23 : ��, ������ ���� ����
    0x17,           // 24 : OVER FLOW ����(�߼��� �ִ� ���)
    0x10,           // 25 : �ǵ� ���
    0x33,           // 26 : �¼� ������ ����
    0x20,           // 27 : ���� ��ũ ���� ����
    0x21,           // 28 : �ü� ������ ����
    0x23,           // 29 : �ü� ������ ����
    0x24,           // 30 : �ü� ���� ����
    0x52,           // 31 : ����2 �µ� ����
    0x42,           // 32 : ����1 �µ� ����
    0x45,           // 33 : �¼� �µ� ����(��ũ��)
    0x46,           // 34 : �¼� �Լ� �µ� ����(�����¼�)
    0x47,           // 35 : �¼� ��� �µ� ����(�����¼�)
    0x48,           // 36 : ���� ���� �µ� ����(�����¼�)
    0x44,           // 37 : �ü� �µ� ����1
    0x54,           // 38 : �ü� �µ� ����2
    0x56,           // 39 : ���̽� Ʈ���� �Լ� �µ� ����
    0x43,           // 40 : �ܱ� �µ� ����1
    0x53,           // 41 : �ܱ� �µ� ����2
    0x25,           // 42 : TEM �濭�� �µ� ����
    0x40,           // 43 : �¼� ����
    0x30,           // 44 : �¼� ����
    0x27,           // 45 : BLDC ���� ��� �ҷ�
    0x29,           // 46 : �Ϲ� ���� ���� �ҷ�
    0x81,           // 47 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x81
    0x82,           // 48 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x82
    0x83,           // 49 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x83
    0x84,           // 50 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x84
    0x85,           // 51 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x85
    0x86,           // 52 : BLDC ���� ���� �ҷ�(��� ���� �ڵ�) - 0x86
    0x26,           // 53 : ��ȯ ����
    0x28,           // 54 : ���� ����
    0x71,           // 55 : EW ��� ���
    0x72,           // 56 : EW ��� �巹�� ������
    0x73,           // 57 : UV1(���� ��ũ)
    0x74,           // 58 : UV2(�ü� ��ũ ��)
    0x75,           // 59 : UV3(���� ��ũ)
    0x76,           // 60 : UV4(���� Ʈ����)
    0x77,           // 61 : UV5(���� �Ŀ��)
    0x78,           // 62 : UV6(���� �Ŀ��)
    0x79,           // 63 : UV7(�ü� ��ũ ��)
    0x90,           // 64 : TDS IN �µ� ����
    0x91,           // 65 : TDS OUT �µ� ����
    0x92,           // 66 : TDS IN ����
    0x93,           // 67 : TDS OUT ����
    0x98,           // 68 : �Լ� ����
    0x99,           // 69 : ������ �ҷ�
};


#define ERROR_CHECK_PERIOD_TIME                     100         // ���� üũ �ֱ� Ÿ�̸� 100ms@1ms

// ���� ����
#define LEAKAGE_ERROR_CHECK_USE                     USE         // ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define LEAKAGE_ERROR_DETECT_TIME                   30          // ���� ���� �ð� 3sec@100ms
#define LEAKAGE_ERROR_UNDETECT_TIME                 600         // ���� �̰��� �ð� 60sec@100ms

// ��������1, 2 ���� ���� - ��Ʈ�� �𵨿��� ���������� 2�� ����� ��쿡�� ���
#define FLOW_SENSOR_ERROR_CHECK_USE                 USE         // ��������1, ��������2 ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define FLOW_SENSOR_ERROR_CHECK_DELAY_TIME          300         // ��������1, ��������2 ���� ���� ���� �� ���� �ð� 30sec@100ms
#define FLOW_SENSOR_ERROR_DETECT_TIME               600         // ���� ���� ���� ���� �ð� 60sec@100ms
#define FLOW_SENSOR_ERROR_HZ                        5           // ���� ���� �̻� ���� ���� Hz

// �ܼ� ���� ���� - �������� 2�� �̻� ���ÿ��� �̰��� ������� ���� ��������1, 2 ���� üũ�� ����� ��
#define WATER_CUT_OFF_ERROR_CHECK_USE               USE         // �ܼ� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define WATER_CUT_OFF_DETECT_TIME                   100         // �ܼ� ���� �ð� 10sec@100ms
#define WATER_CUT_OFF_HZ                            5           // �ܼ� ���� ���� Hz
#define WTAER_CUT_OFF_AUTO_CLEAR_TIME               600         // �ܼ� ���� �ڵ� ���� �ð� 60sec@100ms

// ���� �µ� ���� ���� ����
#define ROOM_WATER_OVERHEAT_ERROR_CHECK_USE         USE         // ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define ROOM_WATER_OVERHEAT_DETECT_TIME             1800        // ���� ���� ���� �ð� 3min = 180sec@100ms

// ���� ���� ���� ���� - �̻��, ǥ�� ���� ����
#define FILTER_CLOGGING_ERROR_CHECK_USE             NO_USE      // ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)

// ���� ���� ���� ����
#define EFFLUENT_CLOGGING_ERROR_CHECK_USE           USE         // ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define EFFLUENT_CLOGGING_ERROR_FLOWMETER_USE       NO_USE      // ���� ���� ���� ������ �������� ��� ���� - 0(NO_USE), 1(USE)
#if (EFFLUENT_CLOGGING_ERROR_FLOWMETER_USE == USE)
#define EFFLUENT_CLOGGING_CHECK_DELAY_TIME          300         // ���� ���� ���� ���� ���� �� ���� �ð� 30sec@100ms
#define EFFLUENT_CLOGGING_HZ                        5           // ���� ���� ���� ���� ���� Hz
#define EFFLUENT_CLOGGING_DETECT_TIME               30          // ���� ���� ���� ���� �ð� 3sec@100ms
#define EFFLUENT_CLOGGING_AUTO_CLEAR_TIME           600         // ���� ���� ���� �ڵ� ���� �ð� 60sec@100ms
#else
#define EFFLEUNT_CLOGGING_DETECT_TEMP               55.0f       // ���� ���� ���� ���� ���� �µ� 55.0��
#define EFFLEUNT_CLOGGING_START_STANDARD_TEMP       70.0f       // ���� ������ ���� ���� ���۽� ���� �µ� 70.0��
#define EFFLUENT_CLOGGING_DETECT_TIME_OVER_STANDARD     20          // ���� ���۽� ���� �µ� �̻󿡼� ���� ���۽� ���� ���� ���� �ð� 2sec@100ms
#define EFFLUENT_CLOGGING_DETECT_TIME_UNDER_STANDARD    10          // ���� ���۽� ���� �µ� �̸����� ���� ���۽� ���� ���� ���� �ð� 1sec@100ms
#define EFFLUENT_CLOGGING_AUTO_CLEAR_COLD_TEMP      10.0f       // ���� ���� ���� �ڵ� ���� ���� �ü� �µ� 10.0��
#define EFFLUENT_CLOGGING_AUTO_CLEAR_TEMP           15.0f       // ���� ���� ���� �ڵ� ���� ���� ����� �µ� 15.0��
#define EFFLUENT_CLOGGING_AUTO_CLEAR_TIME           30          // ���� ���� ���� �ڵ� ���� �ð� 3sec@100ms
#endif

// �¼� �ܼ� �� ��� ���� ���� ����
#define HOT_CUT_OFF_ERROR_CHECK_USE                 USE         // �¼� �ܼ� �� ��� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define HOT_CUT_OFF_ERROR_CHECK_DELAY_TIME          30          // �¼� �ܼ� �� ��� ���� ���� ���� ���� �� ���� �ð� 3sec@100ms
#define HOT_CUT_OFF_DETECT_TIME                     30          // �¼� �ܼ� �� ��� ���� ���� �ð� 3sec@100ms
#define HOT_CUT_OFF_HZ                              5           // �¼� �ܼ� �� ��� ���� ���� ���� Hz
#define HOT_CUT_OFF_AUTO_CLEAR_TIME                 300         // �¼� �ܼ� �� ��� ���� ���� �ڵ� ���� �ð� 30sec@100ms

// �ν�Ʈ���� ���� ����
#define BOOST_PUMP_ERROR_CHECK_USE                  USE         // �ν�Ʈ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define BOOST_PUMP_ERROR_DETECT_TIME                30          // �ν�Ʈ���� ���� ���� �ð� 3sec@100ms
#define BOOST_PUMP_ERROR_OVER_STANDARD_AD           120         // �ν�Ʈ���� ���� ���� ���� AD �ִ밪
#define BOOST_PUMP_ERROR_UNDER_STANDARD_AD          30          // �ν�Ʈ���� ���� ���� ���� AD �ּҰ�

// �з� ���� ���� ����
#define PRESSURE_SENSOR_ERROR_CHECK_USE             NO_USE      // �з� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)

// ���� Ʈ���� ���� ���� ���� - �̻��, ǥ�� ���� ����
#define TRAY_MOTOR_ERROR_CHECK_USE                  NO_USE      // ���� Ʈ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)

// ����ũ�� ����ġ ���� ���� ���� ����
#define MICRO_SW_ALL_DETECT_ERROR_CHECK_USE         USE         // ����ũ�� ����ġ ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)

// ����ũ�� ����ġ �̵� ����(E62, E63) ����
#define MICRO_SW_ICE_MAKING_ERROR_CHECK_USE         USE         // ����ũ�� ����ġ �̵�(����) ���� ���� ��� ���� - 0(NO_USE), 1(USE)

// �巹�� ���� ���� ����
#define DRAIN_PUMP_ERROR_CHECK_USE                  USE         // �巹�� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define DRAIN_PUMP_ERROR_DETECT_TIME                6000        // �巹�� ���� ���� ���� ���� ���� �ð� 10min = 600sec@100ms
#define DRAIN_PUMP_ERROR_CONFIRM_COUNT              12          // �巹�� ���� ���� Ȯ�� Ƚ�� 12ȸ
#define DRAIN_PUMP_ERROR_CHECK_DELAY                50          // �巹�� ���� ���� ���� ���� ���� �ð� 5sec@100ms

// ź�� ���� ���� ����
#define SODA_PUMP_ERROR_CHECK_USE                   NO_USE      // ź�� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define SODA_PUMP_ERROR_DETECT_TIME                 30          // ź�� ���� ���� ���� �ð� 3sec@100ms
#define SODA_PUMP_ERROR_OVER_STANDARD_AD            120         // ź�� ���� ���� ���� ���� AD �ִ밪
#define SODA_PUMP_ERROR_UNDER_STANDARD_AD           30          // ź�� ���� ���� ���� ���� AD �ּҰ�

// ���� ���� �̻� ���� ���� - ���� �Ŀ�� ��� �𵨿����� ���
#define ABNORMAL_EFFLUENT_ERROR_CHECK_USE           USE         // ���� ���� �̻� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define ABNORMAL_EFFLUENT_DETECT_TIME               36000       // ���� ���� �̻� ���� ���� �ð� 1hour = 60min = 3600sec@100ms

// ���� ��ũ ���� ���� ����
#define LEVEL_ERROR_CHECK_USE                       USE         // ���� ��ũ ���� ���� ���� ��� ���� = 0(NO_USE), 1(USE)

// �¼� ������ ���� ���� ����
#define HOT_HIGH_SENSOR_ERROR_CHECK_USE             USE         // �¼� ������ ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define HOT_HIGH_SENSOR_OVERFLOW_SENSOR_USE         USE         // �¼� ������ ���� �������� OVERFLOW ��� ���� ���� - 0(NO_USE), 1(USE)
#define HOT_HIGH_SENSOR_WATER_INPUT_DETECT_TIME     6000        // �¼� ������ ���� ���� ������ ���� ��ũ �Լ� ���� �ð� 10min = 600sec@100ms

// ���� ��ũ ���� ���� ���� ����
#define ICE_COLD_WATER_TANK_SENSOR_ERROR_CHECK_USE  USE         // ���� ��ũ ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)

// �ü� ���� ���� ���� ����
#define COLD_LEVEL_ERROR_CHECK_USE                  USE         // �ü� ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define COLD_LEVEL_ERROR_LOW_SENSOR_USE             USE         // �ü� ���� ���� �������� �ü� ������ ��� ���� ���� - 0(NO_USE), 1(USE)
#define COLD_LEVEL_ERROR_HIGH_SENSOR_USE            NO_USE      // �ü� ���� ���� �������� �ü� ������ ��� ���� ���� - 0(NO_USE), 1(USE)
#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == NO_USE)
#define COLD_LOW_SENSOR_ERROR_DETECT_TIME           9000        // �ü� ������ �̻��� �ü� ������ ���� ���� ���� ���� �ð� 15min = 900sec@100ms
#endif

// ���� �µ� ���� ���� ����
#define ROOM_TEMP_1_ERROR_CHECK_USE                 USE         // ���� �µ� ����1 ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define ROOM_TEMP_1_ERROR_OVER_STANDARD_AD          1000        // ���� �µ� ����1 ���� ���� ���� �ִ� AD��
#define ROOM_TEMP_1_ERROR_UNDER_STANDARD_AD         20          // ���� �µ� ����1 ���� ���� ���� �ּ� AD��
#define ROOM_TEMP_1_ERROR_DETECT_TIME               30          // ���� �µ� ����1 ���� ���� ���� �ð� 3sec@100ms
#define ROOM_TEMP_2_ERROR_CHECK_USE                 NO_USE      // ���� �µ� ����2 ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define ROOM_TEMP_2_ERROR_OVER_STANDARD_AD          1000        // ���� �µ� ����2 ���� ���� ���� �ִ� AD��
#define ROOM_TEMP_2_ERROR_UNDER_STANDARD_AD         20          // ���� �µ� ����2 ���� ���� ���� �ּ� AD��
#define ROOM_TEMP_2_ERROR_DETECT_TIME               30          // ���� �µ� ����1 ���� ���� ���� �ð� 3sec@100ms

// �¼� ��ũ �µ� ���� ���� ����
#define TANK_HOT_TEMP_ERROR_CHECK_USE               USE         // �¼� ��ũ �µ� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define TANK_HOT_TEMP_ERROR_OVER_STANDARD_AD        1000        // �¼� ��ũ �µ� �µ� ���� ���� ���� ���� �ִ� AD��
#define TANK_HOT_TEMP_ERROR_UNDER_STANDARD_AD       20          // �¼� ��ũ �µ� �µ� ���� ���� ���� ���� �ּ� AD��
#define TANK_HOT_TEMP_ERROR_DETECT_TIME             30          // �¼� ��ũ �µ� �µ� ���� ���� ���� ���� �ð� 3sec@100ms

// ���� �¼� �Լ� �µ� ���� ���� ����
#define HOT_IN_TEMP_ERROR_CHECK_USE                 USE         // ���� �¼� �Լ� �µ� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define HOT_IN_TEMP_ERROR_OVER_STANDARD_AD          1000        // ���� �¼� �Լ� �µ� ���� ���� ���� ���� �ִ� AD��
#define HOT_IN_TEMP_ERROR_UNDER_STANDARD_AD         20          // ���� �¼� �Լ� �µ� ���� ���� ���� ���� �ּ� AD��
#define HOT_IN_TEMP_ERROR_DETECT_TIME               30          // ���� �¼� �Լ� �µ� ���� ���� ���� ���� �ð� 3sec@100ms

// ���� �¼� ��� �µ� ���� ���� ����
#define HOT_OUT_TEMP_ERROR_CHECK_USE                USE         // ���� �¼� ��� �µ� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define HOT_OUT_TEMP_ERROR_OVER_STANDARD_AD         1000        // ���� �¼� ��� �µ� ���� ���� ���� ���� �ִ� AD��
#define HOT_OUT_TEMP_ERROR_UNDER_STANDARD_AD        20          // ���� �¼� ��� �µ� ���� ���� ���� ���� �ּ� AD��
#define HOT_OUT_TEMP_ERROR_DETECT_TIME              30          // ���� �¼� ��� �µ� ���� ���� ���� ���� �ð� 3sec@100ms

// ���� �¼� ���� ���� �µ� ���� ���� ����
#define HOT_BODY_TEMP_ERROR_CHECK_USE               USE         // ���� �¼� ���� ���� �µ� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define HOT_BODY_TEMP_ERROR_OVER_STANDARD_AD        1000        // ���� �¼� ���� ���� �µ� ���� ���� ���� ���� �ִ� AD��
#define HOT_BODY_TEMP_ERROR_UNDER_STANDARD_AD       20          // ���� �¼� ���� ���� �µ� ���� ���� ���� ���� �ּ� AD��
#define HOT_BODY_TEMP_ERROR_DETECT_TIME             30          // ���� �¼� ���� ���� �µ� ���� ���� ���� ���� �ð� 3sec@100ms

// �ü� �µ� ���� ���� ����
#define COLD_TEMP_1_ERROR_CHECK_USE                 USE         // �ü� �µ� ����1 ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define COLD_TEMP_1_ERROR_OVER_STANDARD_AD          1000        // �ü� �µ� ����1 ���� ���� ���� �ִ� AD��
#define COLD_TEMP_1_ERROR_UNDER_STANDARD_AD         20          // �ü� �µ� ����1 ���� ���� ���� �ּ� AD��
#define COLD_TEMP_1_ERROR_DETECT_TIME               30          // �ü� �µ� ����1 ���� ���� ���� �ð� 3sec@100ms
#define COLD_TEMP_2_ERROR_CHECK_USE                 NO_USE      // �ü� �µ� ����2 ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define COLD_TEMP_2_ERROR_OVER_STANDARD_AD          1000        // �ü� �µ� ����2 ���� ���� ���� �ִ� AD��
#define COLD_TEMP_2_ERROR_UNDER_STANDARD_AD         20          // �ü� �µ� ����2 ���� ���� ���� �ּ� AD��
#define COLD_TEMP_2_ERROR_DETECT_TIME               30          // �ü� �µ� ����2 ���� ���� ���� �ð� 3sec@100ms

// ���̽� Ʈ���� �Լ� �µ� ���� ���� ����
#define ICE_TRAY_INPUT_TEMP_ERROR_CHECK_USE         NO_USE      // ���̽� Ʈ���� �Լ� �µ� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define ICE_TRAY_INPUT_TEMP_ERROR_OVER_STANDARD_AD  1000        // ���̽� Ʈ���� �Լ� �µ� ����2 ���� ���� ���� �ִ� AD��
#define ICE_TRAY_INPUT_TEMP_ERROR_UNDER_STANDARD_AD  20         // ���̽� Ʈ���� �Լ� �µ� ����2 ���� ���� ���� �ּ� AD��
#define ICE_TRAY_INPUT_TEMP_ERROR_DETECT_TIME       30          // ���̽� Ʈ���� �Լ� �µ� ����2 ���� ���� ���� �ð� 3sec@100ms

// �ܱ� �µ� ���� ���� ����
#define AIR_TEMP_1_ERROR_CHECK_USE                  USE         // �ܱ� �µ� ����1 ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define AIR_TEMP_1_ERROR_OVER_STANDARD_AD           1000        // �ܱ� �µ� ����1 ���� ���� ���� �ִ� AD��
#define AIR_TEMP_1_ERROR_UNDER_STANDARD_AD          20          // �ܱ� �µ� ����1 ���� ���� ���� �ּ� AD��
#define AIR_TEMP_1_ERROR_DETECT_TIME                30          // �ܱ� �µ� ����1 ���� ���� ���� �ð� 3sec@100ms
#define AIR_TEMP_2_ERROR_CHECK_USE                  NO_USE      // �ܱ� �µ� ����2 ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define AIR_TEMP_2_ERROR_OVER_STANDARD_AD           1000        // �ܱ� �µ� ����2 ���� ���� ���� �ִ� AD��
#define AIR_TEMP_2_ERROR_UNDER_STANDARD_AD          20          // �ܱ� �µ� ����2 ���� ���� ���� �ּ� AD��
#define AIR_TEMP_2_ERROR_DETECT_TIME                30          // �ܱ� �µ� ����2 ���� ���� ���� �ð� 3sec@100ms

// TEM �濭�� �µ� ���� ���� ����
#define TEM_HEATSINK_TEMP_ERROR_CHECK_USE           USE         // TEM �濭�� �µ� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define TEM_HEATSINK_TEMP_ERROR_OVER_STANDARD_TEMP  75.0f       // TEM �濭�� �µ� ���� ���� ���� ���� �ִ� �µ���
#define TEM_HEATSINK_TEMP_ERROR_DETECT_TIME         10          // TEM �濭�� �µ� ���� ���� ���� ���� �ð� 1sec@100ms

// �¼� ���� ���� ����
#define HOT_OVERHEAT_ERROR_CHECK_USE                USE         // �¼� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define HOT_OVERHEAT_ERROR_TANK_USE                 NO_USE         // �¼� ���� ���� ������ �¼� ��ũ������ ���� - 0(NO_USE, ������), 1(USE, ��ũ��)
#if (HOT_OVERHEAT_ERROR_TANK_USE == USE)
#define HOT_OVERHEAT_ERROR_OVER_STANDARD_TEMP       98.0f       // TEM �濭�� �µ� ���� ���� ���� ���� �ִ� �µ���
#define HOT_OVERHEAT_ERROR_DETECT_TIME              1800        // �¼� ���� ���� ���� ���� �ð� 3min = 180sec@100ms
#else
#define HOT_OVERHEAT_ERROR_OVER_STANDARD_TEMP       102.0f       // TEM �濭�� �µ� ���� ���� ���� ���� �ִ� �µ���
#define HOT_OVERHEAT_ERROR_DETECT_TIME              5           // �¼� ���� ���� ���� ���� �ð� 500ms@100ms
#define HOT_OVERHEAT_ERROR_AUTO_CLEAR_TEMP          85.0f       // �¼� ���� ���� �ڵ� ���� ���� �µ���
#endif

// �¼� ���� ���� ����
#define HOT_PUMP_ERROR_CHECK_USE                    NO_USE      // �¼� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define HOT_PUMP_ERROR_DETECT_TIME                  30          // �¼� ���� ���� ���� �ð� 3sec@100ms
#define HOT_PUMP_ERROR_OVER_STANDARD_AD             120         // �¼� ���� ���� ���� ���� AD �ִ밪
#define HOT_PUMP_ERROR_UNDER_STANDARD_AD            30          // �¼� ���� ���� ���� ���� AD �ּҰ�

// BLDC ���� ���� ����
#define BLDC_COMP_ERROR_CHECK_USE                   NO_USE      // BLDC ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)

// �Ϲ� ���� ���� �ҷ� ���� ���� - �̻��, ǥ�� ���� ����
#define COMP_ACT_ERROR_CHECK_USE                    NO_USE      // �Ϲ� ���� ���� �ҷ� ���� ���� ��� ���� - 0(NO_USE), 1(USE)

// ��ȯ ���� ���� ����
#define CIRCULATE_PUMP_ERROR_CHECK_USE              NO_USE      // ��ȯ ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define CIRCULATE_PUMP_ERROR_OVER_STANDARD_AD       1000        // ��ȯ ���� ���� ���� ���� �ִ� AD��
#define CIRCULATE_PUMP_ERROR_UNDER_STANDARD_AD      20          // ��ȯ ���� ���� ���� ���� �ּ� AD��
#define CIRCULATE_PUMP_ERROR_DETECT_TIME            100         // ��ȯ ���� ���� ���� �ð� 10sec@100ms
#define CIRCULATE_PUMP_ERROR_CONFIRM_COUNT          3           // ��ȯ ���� ���� Ȯ�� �ݺ� Ƚ��

// ���� ���� ���� ����
#define STIRRING_MOTOR_ERROR_CHECK_USE              NO_USE      // ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define STIRRING_MOTOR_ERROR_CHECK_DELAY            10          // ���� ���� ���� ���� ���� �� ���� �ð� 1sec@100ms
#define STIRRING_MOTOR_ERROR_OVER_STANDARD_AD       1000        // ���� ���� ���� ���� ���� �ִ� AD��
#define STIRRING_MOTOR_ERROR_UNDER_STANDARD_AD      20          // ���� ���� ���� ���� ���� �ּ� AD��
#define STIRRING_MOTOR_ERROR_DETECT_TIME            30          // ���� ���� ���� ���� �ð� 3sec@100ms
#define STIRRING_MOTOR_ERROR_RE_CHECK_DELAY         18000       // ���� ���� ���� ��˻� �� ���� �ð� 30min = 1800sec@100ms
#define STIRRING_MOTOR_ERROR_CONFIRM_TIME_HOUR      2880        // ���� ���� ���� Ȯ�� �ð� 48hour = 2880min

// EW ��� ��� ���� ����
#define EW_MODULE_ERROR_CHECK_USE                   NO_USE      // EW ��� ��� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define EW_MODULE_ERROR_OVER_STANDARD_AD            1000        // EW ��� ��� ���� ���� ���� �ִ� AD��
#define EW_MODULE_ERROR_UNDER_STANDARD_AD           20          // EW ��� ��� ���� ���� ���� �ּ� AD��
#define EW_MODULE_ERROR_DETECT_TIME                 70          // EW ��� ��� ���� ���� �ð� 7sec@100ms
#define EW_MODULE_ERROR_DETECT_COUNT                3           // EW ��� ��� ���� ���� ī��Ʈ

// EW ��� �巹�� ���� ���� ����
#define EW_DRAIN_PUMP_ERROR_CHECK_USE               NO_USE      // EW ��� �巹�� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define EW_DRAIN_PUMP_ERROR_OVER_STANDARD_AD        1000        // EW ��� �巹�� ���� ���� ���� ���� �ִ� AD��
#define EW_DRAIN_PUMP_ERROR_UNDER_STANDARD_AD       20          // EW ��� �巹�� ���� ���� ���� ���� �ּ� AD��
#define EW_DRAIN_PUMP_ERROR_DETECT_TIME             15000       // EW ��� �巹�� ���� ���� ���� �ð� 25min = 1500sec@100ms

// UV ���� ����
#define UV_ERROR_CHECK_USE                          NO_USE      // UV ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define UV_ERROR_OVER_STANDARD_AD                   1000        // UV ���� ���� ���� �ִ� AD��
#define UV_ERROR_UNDER_STANDARD_AD                  20          // UV ���� ���� ���� �ּ� AD��
#define UV_ERROR_DETECT_TIME                        100         // UV ���� ���� �ð� 10sec@100ms

// TDS IN �µ� ���� ���� ����
#define TDS_IN_TEMP_ERROR_CHECK_USE                 NO_USE      // TDS IN �µ� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define TDS_IN_TEMP_ERROR_OVER_STANDARD_AD          1000        // TDS IN �µ� ���� ���� ���� ���� �ִ� AD��
#define TDS_IN_TEMP_ERROR_UNDER_STANDARD_AD         20          // TDS IN �µ� ���� ���� ���� ���� �ּ� AD��
#define TDS_IN_TEMP_ERROR_DETECT_TIME               30          // TDS IN �µ� ���� ���� ���� �ð� 3sec@100ms

// TDS OUT �µ� ���� ���� ����
#define TDS_OUT_TEMP_ERROR_CHECK_USE                NO_USE      // TDS OUT �µ� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define TDS_OUT_TEMP_ERROR_OVER_STANDARD_AD         1000        // TDS OUT �µ� ���� ���� ���� ���� �ִ� AD��
#define TDS_OUT_TEMP_ERROR_UNDER_STANDARD_AD        20          // TDS OUT �µ� ���� ���� ���� ���� �ּ� AD��
#define TDS_OUT_TEMP_ERROR_DETECT_TIME              30          // TDS OUT �µ� ���� ���� ���� �ð� 3sec@100ms

// TDS IN ���� ���� ����
#define TDS_IN_ERROR_CHECK_USE                      NO_USE      // TDS IN ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define TDS_IN_ERROR_DETECT_TIME                    3           // TDS IN ���� ���� ���� �ð� 300ms@100ms
#define TDS_IN_ERROR_DETECT_COUNT                   5           // TDS IN ���� ���� ���� Ƚ��

// TDS OUT ���� ���� ����
#define TDS_OUT_ERROR_CHECK_USE                     NO_USE      // TDS OUT ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define TDS_OUT_ERROR_DETECT_TIME                   3           // TDS OUT ���� ���� ���� �ð� 300ms@100ms
#define TDS_OUT_ERROR_DETECT_COUNT                  5           // TDS OUT ���� ���� ���� Ƚ��

// ���� ���� ���� ����
#define INPUT_CONTAMINATION_ERROR_CHECK_USE         NO_USE      // ���� ���� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define INPUT_CONTAMINATION_ERROR_DETECT_DELAY_TIME 10          // ���� ���� ���� ���� üũ �� ���� �ð� 1sec@100ms
#define INPUT_CONTAMINATION_ERROR_DETECT_COUNT      3           // ���� ���� ���� ���� Ƚ��

// ������ �ҷ� ���� ����
#define DECONTAMINATION_ERROR_CHECK_USE             NO_USE         // ������ �ҷ� ���� ���� ��� ���� - 0(NO_USE), 1(USE)
#define DECONTAMINATION_ERROR_CHECK_STANDARD_OUT_PPM    15      // ������ �ҷ� üũ�� �ϴ� ���� TDS OUT��(ppm)
#define DECONTAMINATION_ERROR_DETECT_DELAY_TIME     10          // ������ �ҷ� ���� ���� üũ �� ���� �ð� 1sec@100ms
#define DECONTAMINATION_ERROR_DETECT_COUNT          3           // ������ �ҷ� ���� ���� Ƚ��


void Lib_WaterErrorCheck_Initialize(void);
void WaterErrorCheckTimer(void);
void Lib_WaterErrorCheck(void);

void LeakageErrorCheck(void);

#if ( (FLOW_SENSOR_ERROR_CHECK_USE == USE) || (WATER_CUT_OFF_ERROR_CHECK_USE == USE) )
void Set_FlowMeterErrorCheckStart(U8 mu8Status);
U8 Get_FlowMeterErrorCheckStart(void);
void Set_FlowMeterErrorCheckDelayTime(U16 mu16Time);
U16 Get_FlowMeterErrorCheckDelayTime(void);
#endif
void FlowMeterErrorCheck_BuiltInUse(void);

void WaterCutOffErrorCheck(void);
void RoomOverHeatErrorCheck(void);
void FilterCloggingErrorCheck(void);

#if (EFFLUENT_CLOGGING_ERROR_CHECK_USE == USE)
void Set_EffluentCloggingErrorCheckStart(U8 mu8Status);
U8 Get_EffluentCloggingErrorCheckStart(void);
#endif
void EffluentCloggingErrorCheck(void);

#if (HOT_CUT_OFF_ERROR_CHECK_USE == USE)
void Set_HotCutOffCheckStart(U8 mu8Status);
U8 Get_HotCutOffCheckStart(void);
#endif
void HotCutOffErrorCheck(void);

void BoostPumpErrorCheck(void);
void PressureSensorErrorCheck(void);
void TrayMotorErrorCheck(void);
void MicroSW_AllDetectErrorCheck(void);
#if (MICRO_SW_ALL_DETECT_ERROR_CHECK_USE == USE)
void Set_MicroSW_AllDetectErrorCheckStep(U8 mu8Step);
U8 Get_MicroSW_AllDetectErrorCheckStep(void);
#endif
void MicroSW_IceMakingErrorCheck(void);
#if (MICRO_SW_ICE_MAKING_ERROR_CHECK_USE == USE)
void check_error_tray_motor_up_operation(void);
void Set_TrayUp_ErrorCount(U8 mu8Data);
U8 Get_TrayUp_ErrorCount(void);
void check_error_tray_motor_down_retry_operation(void);
void Set_TrayDown_ErrorCount(U8 mu8Data);
U8 Get_TrayDown_ErrorCount(void);
void Clear_E62_E63_CheckTimer(void);
#endif
void DrainPumpErrorCheck(void);
void SodaPumpErrorCheck(void);
void AbnormalEffluentErrorCheck(void);
void RoomLevelErrorCheck(void);
void HotHighLevelErrorCheck(void);
void IceColdWaterTankLevelErrorCheck(void);
void ColdLevelErrorCheck(void);
void RoomTempErrorCheck(void);
void HotTempErrorCheck(void);
void ColdTempErrorCheck(void);
void IceTrayInputTempErrorCheck(void);
void AirTempErrorCheck(void);
void HeatsinkTempErrorCheck(void);
void HotOverheatErrorCheck(void);
void HotPumpErrorCheck(void);
void BLDC_CompErrorCheck(void);
void CompActErrorCheck(void);
void CirculatePumpErrorCheck(void);
void StirringMoterErrorCheck(void);
void EW_ModuleErrorCheck(void);
void EW_DrainPumpErrorCheck(void);
void UVErrorCheck(void);
void TDS_In_TempErrorCheck(void);
void TDS_Out_TempErrorCheck(void);
void TDS_InErrorCheck(void);
void TDS_OutErrorCheck(void);
void InputContaminationErrorCheck(void);
void DecontaminationErrorCheck(void);

U8 Get_ErrorDisplayCode(void);
U8 Get_ErrorOccuredCheck(void);
void Set_ErrorStatus(U8 mu8ID, U8 mu8Status);
U8 Get_ErrorStatus(U8 mu8ID);
void Set_ErrorDetect(U8 mu8ID, U8 mu8Status);
U8 Get_ErrorDetect(U8 mu8ID);
U8 Get_ErrorAboutWaterInput(void);
U8 Get_AirTempError(void);
U8 Get_HotError(void);
U8 Get_ColdError(void);
U8 Get_ErrorAboutIceMaking(void);

void Lib_WaterError_Module_Initialize(void);
void Lib_WaterError_Module_1ms_Control(void);
void Lib_WaterError_Module_Control(void);

#endif

