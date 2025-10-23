/// @file     Hal_GeneralADC.h
/// @date     2025/03/18
/// @author   Jaejin Ham
/// @brief    ���� ADC Control file


#ifndef _HAL_GENERAL_ADC_H_
#define _HAL_GENERAL_ADC_H_


/*  Hal AD ���� Function ����
    => Set_CoolThermistor_PowerControl(mu8Mode);           // �ü� �µ� ���� ���� ���� ��� ���� : mu8Mode - 0(OFF), 1(�Ϲ� ���۽�), 2(Test ���۽�), 3(Error �߻���)
    => Set_Hal_ADC_Check(mu8ADC_ID, mu8Start);             // ADC ������ �Ϸ��� ID�� ���� ��� ��� ���� ���� �Լ� : mu8Start - 0(No Chcek), 1(Check Start)
                                                            // �� �� �Լ��� �ݵ�� Start�� ������ ����߸� �ش� ID�� ADC ������ ��� ��� �Լ��� ������.
    => Set_All_ADC_CheckStart();                           // ��� ADC ���� ��� ����� �����ϰ� �����ϴ� �Լ�(�������� �����ؾ� ��� ����� ������)
    => Average_ADC(mu8ID, mu16AD_Value);                   // ID�� �ش��ϴ� AD�� ��� ��� �Լ�(AD Convert �Լ��� ���� �߰� �ʿ�)
    => Get_ADC_Data(mu8ID);                                // ID�� �ش��ϴ� AD ��հ� Ȯ��, mu8Return : AD ��� ������

    => Get_Temp(mu8ID);                                    // ID�� �ش��ϴ� �µ������� �µ� ������ Ȯ��, f32Return : �µ� ������(�Ҽ��� ����)

    ��� ��)
    Set_CoolTH_PowerControl(COOL_TH_OFF_MODE);              // �ü� �µ� ���� ���� ���� OFF
    Set_CoolTH_PowerControl(COOL_TH_NORMAL_MODE);           // �ü� �µ� ���� ���� �Ϲ� ���� ����
    Set_CoolTH_PowerControl(COOL_TH_TEST_MODE);             // �ü� �µ� ���� ���� �׽�Ʈ ��� ���� ����
    Set_Hal_ADC_Check(ADC_TH_HOT, 1);                       // ADC_TH_HOT ID�� ADC ������ ���� : mu8Start - 0(Chcek Stop), 1(Check Start)
    u8Return = Get_ADC_Data(ADC_TH_COOL);                   // ID ADC_TH_COOL�� �µ� ���� AD ��հ� Ȯ��, mu8Return : AD ��� ������
    f32Temp = Get_Temp(ADC_TH_COOL);                        // ID ADC_TH_COOL�� �µ� ������ ������ Ȯ��, �Ҽ��� 1�ڸ� �� ���� ��ȯ
*/

// ADC �ִ� 25�� ��� ����

// ADC ���� Count ����
#define ADC_USE                                         USE

#if (ADC_USE == USE)

#define COOL_THERMISTOR_POWER_CONTROL                   USE             // �ü� �µ� ���� ���� ���� ��� ���� ���� - 0(No Control), 1(Control)

#if (COOL_THERMISTOR_POWER_CONTROL == USE)                              // �ü� �µ� ���� ���� ���� ��

#define COOL_THERMISTOR_COUNT                           1               // �ü� �µ� ���� ���� : 1(1��), 2(2�� Max)

#endif      // if - �ü� �µ� ���� ���� �����


// �µ� ���� ���� ���� ����
#if (COOL_THERMISTOR_POWER_CONTROL == USE)
#define TH_OFF_MODE                                     0
#define TH_NORMAL_MODE                                  1
#define TH_TEST_MODE                                    2
#define TH_ERROR_MODE                                   3

#define P_COOL1_TH_ON                                   P6_bit.no2      // �ü� �µ� ���� 1 ���� ���� Port, �̻��� 0 ó��
#define COOL1_TARGET_TEMP                               6.0f            // �ð� OFF ���� ���� �µ�(������)

#if (COOL_THERMISTOR_COUNT == 2)
#define P_COOL2_TH_ON                                   0               // �ü� �µ� ���� 2 ���� ���� Port, �̻��� 0 ó��
#define COOL2_TARGET_TEMP                               0.0f            // �ð� OFF ���� ���� �µ�(������)
#endif

// Cool Thermistor ���� ���� ���� ���
#define COOL_TH_OFF_MODE                                0           // �ü� �µ� ���� ���� OFF ���� ���
#define COOL_TH_NORMAL_MODE                             1           // �Ϲ� ����
#define COOL_TH_TEST_MODE                               2           // �׽�Ʈ ��� ����
#define COOL_TH1_ERROR_CHECK_MODE                       3           // �ü� ���� 1 ���� �߻� üũ��
#define COOL_TH2_ERROR_CHECK_MODE                       4           // �ü� ���� 2 ���� �߻� üũ��
#define COOL_TH_ALL_ERROR_CHECK_MODE                    5           // �ü� ���� 1, 2 ���  ���� �߻� üũ��

#define COOL_TH_POWER_CONTROL_TIME_PERIOD               100         // ���� ���� Control �Լ� ȣ�� �ֱ�(100ms@1ms)

#endif

#define ADC_DATA_MAX                                    AD_CONV_MAX

#define ADC_AVERAGE_COUNT_0                             10              // ADC 1 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_1                             10              // ADC 1 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_2                             10              // ADC 2 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_3                             10              // ADC 3 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_4                             10              // ADC 4 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_5                             10              // ADC 5 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_6                             10              // ADC 6 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_7                             10              // ADC 7 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_8                             10              // ADC 8 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_9                             10              // ADC 9 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_10                            10              // ADC 10 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_11                            10              // ADC 11 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_12                            10              // ADC 12 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_13                            10              // ADC 13 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_14                            10              // ADC 14 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_15                            10              // ADC 15 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_16                            10              // ADC 16 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_17                            10              // ADC 17 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_18                            10              // ADC 18 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_19                            10              // ADC 19 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_20                            10              // ADC 20 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_21                            10              // ADC 21 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_22                            10              // ADC 22 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_23                            10              // ADC 23 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_24                            10              // ADC 24 ADC�� ��� ���� ������ ��
#define ADC_AVERAGE_COUNT_25                            10              // ADC 25 ADC�� ��� ���� ������ ��


void Hal_ADC_Initialize(void);
U16 Get_ADC_Data(U8 mu8Channel);
void Set_Hal_ADC_Check(U8 mu8Channel, U8 mu8Start);
void Set_Hal_All_ADC_CheckStart(void);
void Average_ADC(U8 mu8Channel, U16 mu16AD_Value);

#if (COOL_THERMISTOR_POWER_CONTROL == USE)
void CoolTH_ControlInitialize(void);
void CoolTH_ControlTimer(void);
void Set_CoolTH_PowerControl(U8 mu8Mode);
void Cool1TH_Control(U8 mu8ControlMode);
#if (COOL_THERMISTOR_COUNT == 2)
void Cool2TH_Control(U8 mu8ControlMode);
#endif
void CoolTH_ADC_CheckStartControl(void);
void CoolTH_PowerControl(void);
#endif

#endif


void Hal_ADC_Module_Initialize(void);

void Hal_ColdTH_Module_Initialize(void);
void Hal_ColdTH_Module_1ms_Control(void);
void Hal_ColdTH_Module_Control(void);

#endif

