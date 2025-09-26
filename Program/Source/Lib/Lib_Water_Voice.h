/// @file     Lib_Water_Voice.h
/// @date     2025/04/04
/// @author   Jaejin Ham
/// @brief    Voice Control file


#ifndef _LIB_WATER_VOICE_H_
#define _LIB_WATER_VOICE_H_


#if (VOICE_IC_USE == USE)

#define VOICE_1_SHUTOFF_WATER_FILTER_COVER                       1     // '���� Ŀ���� ���� �� ������ �����մϴ�.'
#define VOICE_2_STOP_ICE_ICETANK_COVER                           2     // '������ũ Ŀ���� ���Ƚ��ϴ�. ���� ������ �����մϴ�.'
#define VOICE_3_SHUTOFF_WATER_SIDE_COVER                         3     // '������ ���� Ŀ���� ���� �� ������ �����մϴ�. '
#define VOICE_4_SHUTOFF_WATER_TOP_COVER                          4     // '����� ���� Ŀ���� ���� �� ������ �����մϴ�.'
#define VOICE_5_SHUTOFF_WATER_FRONT_COVER                        5     // '������ ���� Ŀ���� ���� �� ������ �����մϴ�.'
#define VOICE_6_RESTART_ICE_COVER_OK                             6     // '���� ��ũ Ŀ���� �������ϴ�. ������ ����� �մϴ�.'
#define VOICE_7_CALL_AS_COLD_WATER_ERROR                         7     // '�ü� ��ɿ� �̻��� �����Ǿ����� �����Ϳ� �������ּ���'
#define VOICE_8_CLEAN_DUST_FILTER_COLD_WATER_ERROR               8     // '�ü� ��ɿ� �̻��� �����Ǿ����� ��ǰ�� ���� ���͸� ��ô���ּ���.'
#define VOICE_9_CALL_AS_HOT_WATER_ERROR                          9     // '�¼� ��ɿ� �̻��� �����Ǿ����� �����Ϳ� �������ּ���'
#define VOICE_10_CALL_AS_ICE_ERROR                              10     // '���� ��ɿ� �̻��� �����Ǿ����� �����Ϳ� �������ּ���'
#define VOICE_11_CALL_AS_WATER_SUPPLY_ERROR                     11     // '�� ���޿� �̻��� ���� �Ǿ�����, �������� ��긦 Ȯ�����ֽð� ������ �ݺ��Ǹ� �����ͷ� �������ּ���.'
#define VOICE_12_CALL_AS_WATER_LEAKAGE_ERROR                    12     // '"������ �����Ǿ����� �������� ��긦 ����ֽð� ������ �ݺ��Ǹ� �����ͷ� �������ּ���."'
#define VOICE_13_REPLACE_ONLY_FILTER_NO1_NO3                    13     // '"1���� 3�� ���͸� ��ü�ϼž� �մϴ�.  �ٽ� Ȯ���Ͻð� 1���� 3�� ���͸� ��ü���ּ���. "'
#define VOICE_14_PRESS_RESET_BUTTON_AFTER_NEW_FILTER            14     // '"���Ͱ� ����Ǿ����ϴ�. �� ���ͷ� ��ü�ϼ����� ���¹�ư�� �����ּ��� "'
#define VOICE_15_REPLACE_ADD_FILTER_NO3_PLEASE                  15     // '"1���� 3�� ���͸� ��ü�ϼž� �մϴ�.   3�� ���͸� �߰��� ��ü���ּ���"'
#define VOICE_16_REPLACE_AFTER_CHECK_FILTER_NO1_NO3             16     // '"1���� 3�� ���͸� ��ü�ϼž� �մϴ�.  �ٽ� Ȯ���Ͻð� 1���� 3�� ���͸� ��ü���ּ���."'
#define VOICE_17_REPLACE_ADD_FILTER_NO1_PLEASE                  17     // '"1���� 3�� ���͸� ��ü�ϼž� �մϴ�.  1�� ���͸� �߰��� ��ü���ּ��� "'
#define VOICE_18_CLOSE_COVER_AFTER_FILTER_NO1_NO3               18     // '1���� 3�� ���͸� ��ü�ϼ̴ٸ�, ���� Ŀ���� �ݾ��ּ���'
#define VOICE_19_PRESS_YES_OR_NO_AFTER_FILTER_NO1_NO3           19     // '1���� 3�� ���͸� ��ü�ϼ̴ٸ� ���� ������ ���� "���" ��ư�� �����ּ���. �� ���ͷ� ��ü���� �����̴ٸ� "�ü�" ��ư�� �����ּ���'
#define VOICE_20_REPLACE_AFTER_CHECK_FILTER_NO1_NO3_V2          20     // '"1���� 3�� ���͸� ��ü�ϼž� �մϴ�.  �ٽ� Ȯ���Ͻð� 1���� 3�� ���͸� ��ü���ּ���."' (16���� �����̴ٸ�)
#define VOICE_21_PRESS_YES_OR_NO_AFTER_FILTER_NO1_NO3_WELL      21     // '"1���� 3�� ���͸� ����� ��ü�ϼ̴ٸ� ���� ������ ���� ""���"" ��ư�� �����ּ���. �� ���ͷ� ��ü���� �����̴ٸ� ""�ü�"" ��ư�� �����ּ���"'
#define VOICE_22_REPLACE_ONLY_FILTER_NO1_NO3_NOT_YET_NO2        22     // '1���� 3�� ���͸� ��ü�ϼž� �մϴ�. 2�� ���ʹ� ��ü �ñⰡ �ƴϿ��� �ٽ� Ȯ�����ּ���.'(�ѱ�� �ٸ�)
#define VOICE_23_START_FILTER_FLUSHING                          23     // '���� �÷����� �����մϴ�. �÷��� �Ϸ� �� ��ǰ ����� �����մϴ�.'
#define VOICE_24_FINISH_FILTER_FLUSHING                         24     // '���� �÷����� �Ϸ�Ǿ� ��ǰ ����� �����մϴ�.'
#define VOICE_25_CHECK_FILTER_FLUSHING_MANUAL                   25     // '"���� �÷����� ���õ��� �ʾҽ��ϴ�.  ���� ��ü�� �����ϼ̴ٸ�, �Ŵ����� �����Ͽ� �÷����� �ٽ� �������ּ���."'
#define VOICE_26_REPLACE_FILTER_NO1_NO3                         26     // '1���� 3�� ���͸� ��ü���ּ���. '(�ѱ���)
#define VOICE_27_INFORM_ACTIVATED_FILTER_NO1_ADD_NO3            27     // '"1�� ������ ���� ��ư�� Ȱ��ȭ �Ǿ����ϴ�. 3�� ���͸� �߰��� ��ü�� �ּ���"'
#define VOICE_28_INFORM_ACTIVATED_FILTER_NO3_ADD_NO1            28     // '"3�� ������ ���� ��ư�� Ȱ��ȭ �Ǿ����ϴ�. 1�� ���͸� �߰��� ��ü�� �ּ��� "'
#define VOICE_29_CLOSE_COVER_FOR_FILTER_FLUSHING                29     // '"���� ��ü�� �Ϸ�Ǿ����ϴ�.  ���� Ŀ���� ������ ���� �÷����� �����մϴ�."'
#define VOICE_30_START_FILTER_FLUSHING_NO1_NO3                  30     // '1���� 3�� ������ �÷����� �����մϴ�. �÷��� �Ϸ� �� ��ǰ ����� �����մϴ�.'(�ѱ���)
#define VOICE_31_PRESS_RESET_BUTTON_AFTER_NEW_FILTER            31     // '���͸� ��ü �Ͻ� �� ���¹�ư�� �����ּ���.'
#define VOICE_32_PRESS_RESET_BUTTON_FILTER_NO1                  32     // '"1�� ���� ���� ��ư�� Ȱ��ȭ ���� �ʾҽ��ϴ�. ������ ���� Ŀ���� ���� 1�� ���� ���¹�ư�� �����ּ���."'
#define VOICE_33_PRESS_RESET_BUTTON_FILTER_NO3                  33     // '"3�� ���� ���� ��ư�� Ȱ��ȭ ���� �ʾҽ��ϴ�.  ������ ���� Ŀ���� ���� 3�� ���� ���¹�ư�� �����ּ���."'
#define VOICE_34_REPLACE_ADD_FILTER_NO3                         34     // '"1���� 3�� ���͸� ��ü�ϼž� �մϴ�.   3�� ���͸� �߰��� ��ü�� �ֽñ� �ٶ��ϴ�."' (�ּ���� �ٸ�)
#define VOICE_35_REPLACE_ADD_FILTER_NO1                         35     // '"1���� 3�� ���͸� ��ü�ϼž� �մϴ�.  1�� ���͸� �߰��� ��ü�� �ֽñ� �ٶ��ϴ�. "'(�ּ���� �ٸ�)
#define VOICE_36_CANCEL_RESET_FILTER_NO1                        36     // '1�� ���� ������ ����մϴ�.'
#define VOICE_37_CANCEL_RESET_FILTER_NO2                        37     // '2�� ���� ������ ����մϴ�.'
#define VOICE_38_CANCEL_RESET_FILTER_NO3                        38     // '3�� ���� ������ ����մϴ�.'
#define VOICE_39_CANCEL_FILTER_FLUSHING                         39     // '"���� �÷����� ��ҵǾ����ϴ�. �ٽ� ���� �÷����� �����Ͻ÷���, ���͸� ���� �� �ٽ� �����ϰ� ���¹�ư�� �����ּ���. ."'
                                                                       // '���Ͱ� ����Ǿ����ϴ�. �� ���ͷ� ��ü�ϼ����� ���¹�ư�� �����ּ��� '(����)
#define VOICE_40_PRESS_RESET_BUTTON_NO1                         40     // 'If number 1 filter is replaced, please press the reset button.'(����)
#define VOICE_41_PRESS_RESET_BUTTON_NO2                         41     // 'If number 2 filter is replaced, please press the reset button.'(����)
#define VOICE_42_PRESS_RESET_BUTTON_NO3                         42     // 'If number 3 filter is replaced, please press the reset button.'(����)
#define VOICE_43_REPLACE_ADD_FILTER_NO2_NO3_FOR_ALL             43     // '��� ���͸� ��ü�ϼž� �ϴ� 2���� 3�� ���͸� �߰��� ��ü���ּ���'
#define VOICE_44_REPLACE_ADD_FILTER_NO1_NO3_FOR_ALL             44     // '��� ���͸� ��ü�ϼž� �ϴ� 1���� 3�� ���͸� �߰��� ��ü���ּ���'
#define VOICE_45_REPLACE_ADD_FILTER_NO1_NO2_FOR_ALL             45     // '��� ���͸� ��ü�ϼž� �ϴ� 1���� 2�� ���͸� �߰��� ��ü���ּ���'
#define VOICE_46_PRESS_YES_OR_NO_AFTER_ALL_FILTERS              46     // '��� ���͸� ��ü�ϼ̴ٸ� ���� ������ ���� "���" ��ư�� �����ּ���. �� ���ͷ� ��ü���� �����̴ٸ� "�ü� ��ư�� �����ּ���'
#define VOICE_47_CHECK_REPLACE_ALL_FILTERS                      47     // '��� ���͸� ��ü�ϼž� �ϴ�, �ٽ� Ȯ�����ּ���.'
#define VOICE_48_NOT_SELECTED_FILTER_FLUSHING                   48     // '���� �÷����� ���õ��� �ʾҽ��ϴ�. �ٷ� ��ǰ ����� �����մϴ�.'
#define VOICE_49_REPLACE_FILTER_ALL                             49     // '��� ���͸� ��ü���ּ���.'
#define VOICE_50_INFORM_ACTIVATION_ADD_OTHER_FILTER             50     // '"���� ���� ��ư�� Ȱ��ȭ �Ǿ����ϴ�. ������ ���͸� �߰��� ��ü���ּ���"'

#define VOICE_51_CLOSE_COVER_FOR_FILTER_FLUSHING_V2             51     // '"���� ��ü�� �Ϸ�Ǿ����ϴ�.  ���� Ŀ���� ������ ���� �÷����� �����մϴ�."' (������ �ٸ�)

#define VOICE_52_PRESS_RESET_BUTTON_AFTER_REPLACE_FILTER        52     // '���͸� ��ü�Ͻ� �� ���¹�ư�� �����ּ���'
#define VOICE_53_PRESS_RESET_BUTTON_DEACTIVATED                 53     // '���� �÷����� �����մϴ�.  �÷��� �Ϸ� �� ��ǰ ����� �����մϴ�.'
#define VOICE_54_REPLACE_ADD_FILTER_NO3_FOR_ALL                 54     // '"��� ���͸� ��ü�ϼž� �մϴ�.  3�� ���͸� �߰��� ��ü���ּ���" '
#define VOICE_55_REPLACE_ADD_FILTER_NO1_FOR_ALL                 55     // '"��� ���͸� ��ü�ϼž� �մϴ�.  1�� ���͸� �߰��� ��ü���ּ���" '
#define VOICE_56_REPLACE_ADD_FILTER_NO2_FOR_ALL                 56     // '"��� ���͸� ��ü�ϼž� �մϴ�.  2�� ���͸� �߰��� ��ü���ּ���" '

#define VOICE_57_PRESS_DISPENSE_BUTTON_AFTER_WATER_VALVE_OPEN   57     // '"���� ���� ��긦 Ȯ���Ͻþ� ���޹�긦 ���� �����ư�� �����ּ���." '
#define VOICE_58_PRESS_YES_OR_NO_AFTER_FAUCET                   58     // '���� �Ŀ���� ��ü�ϼ̴ٸ� "���" ��ư�� �����ּ���. ��ü���� �����̴ٸ� "�ü� ��ư�� �����ּ���'
#define VOICE_59_PRESS_YES_OR_NO_AFTER_FILTER_FAUCET            59     // '���Ϳ� ���� �Ŀ���� ��ü�ϼ̴ٸ� "���" ��ư�� �����ּ���. ��ü���� �����̴ٸ� "�ü� ��ư�� �����ּ���'
#define VOICE_60_WAIT_ICE_TANK_CLEANING                         60     // '���� ��ũ ��ô �߿��� ������ ���� ����Ͻ� �� �����ϴ�'
#define VOICE_61_START_SMART_TESTING_MODE                       61     // '����Ʈ ������ �����մϴ�. ���� �߿��� ��ǰ�� ����Ͻ� �� ������ ��ø� ��ٷ� �ּ���.'
#define VOICE_62_FINISH_SMART_TESTING_MODE_NO_ERROR             62     // '����Ʈ ������ �Ϸ� �Ǿ� ��ǰ�� ����Ͻ� �� �ֽ��ϴ�. '
#define VOICE_63_FINISH_SMART_TESTING_MODE_WITH_ERROR           63     // '����Ʈ ������ �Ϸ� �Ǿ����ϴ�.'
#define VOICE_64_INFORM_BLE_MODE_ACTIVATED                      64     // '������� ���� ��尡 Ȱ��ȭ �Ǿ����ϴ�. '
#define VOICE_65_INFORM_BLE_MODE_DEACTIVATED                    65     // '������� ���� ��尡 ��Ȱ��ȭ �Ǿ����ϴ�.'
#define VOICE_66_INFORM_WIFI_MODE_ACTIVATED                     66     // 'Wi-Fi ���� ��尡 Ȱ��ȭ �Ǿ����ϴ�. '
#define VOICE_67_INFORM_WIFI_MODE_DEACTIVATED                   67     // 'Wi-Fi ���� ��尡 ��Ȱ��ȭ �Ǿ����ϴ�.'
#define VOICE_68_INFORM_WIFI_ON                                 68     // '��ǰ�� Wi-Fi�� �������ϴ�.'
#define VOICE_69_INFORM_WIFI_OFF                                69     // '��ǰ�� Wi-Fi�� �������ϴ�.'
#define VOICE_70_PRESS_BUTTON_ACCORDING_TO_APP_GUIDE            70     // '"��ǰ ������ ���� ���Դϴ�. �ۿ��� ���̵��ϴ� ��ư�� ���� �ּ���." '
#define VOICE_71_INFORM_APP_PAIRING_SUCCESS                     71     // '��ǰ�� ���� ���������� ����Ǿ����ϴ�.'
#define VOICE_72_INFORM_APP_PAIRING_FAIL                        72     // '��ǰ�� ���� ������ �����Ͽ����ϴ�. '
#define VOICE_73_INFORM_APP_DISCONNECTED                        73     // '��ǰ�� ���� ������ ���������ϴ�.'
#define VOICE_74_INFORM_WIFI_OFF                                74     // '��ǰ�� Wi-Fi�� �������ϴ�. '
#define VOICE_75_PRESS_INSTALL_BUTTON                           75     // '������ ��ġ�� ���� �����ư�� �����ּ���'
#define VOICE_76_START_WIFI_AUTOMATIC_CONNECTION                76     // '�������� ������ �ڵ����� �����մϴ�.'
#define VOICE_77_WAIT_WATER_PIPE_DRAIN                          77     // '"������ �� ������ ���� ������ ���� ���� �ֽ��ϴ�.   ��ø� ��ٷ��ּ���"
#define VOICE_78_DISCARD_FIRST_CUP                              78     // '������ ���� ��ñ� ���� ù ���� ������ ������ �ּ���.
#define VOICE_79_WATCHOUT_HOT_WATER_CUP                         79     // '�¼��� �߰ſ�� �����ϼ���.
#define VOICE_80_WATCHOUT_HOT_WATER_CONTINUOUS                  80     // '"�¼��� �߰ſ�� �����ϼ���.  ������ ���߽÷��� ���� ��ư�� �����ּ���."
#define VOICE_81_SETTING_MODE_CLEAN_CARE                        81     // 'Ŭ���ɾ� ���
#define VOICE_82_START_CLEAN_CARE_WATCHOUT_FAUCET               82     // '"Ŭ���ɾ �����߿� �¼��� ƥ �� ������ �Ŀ�� �ؿ� �˸��� ���� �����ðų� �Ŀ�� ����θ� ��ũ��� ���ϰ� ���ּ���."
#define VOICE_83_PRESS_BUTTON_MANUAL_CLEAN_CARE                 83     // '�غ� �Ϸ�Ǿ��ٸ� '��� ��ư'�� 3�� �̻� �����ּ���
#define VOICE_84_START_CLEAN_CARE_WATCHOUT_HOT                  84     // '"Ŭ���ɾ �����մϴ�.  ����η� �߰ſ� ���� ����Ǵ� ȭ���� ���� �ʵ��� �����Ͻñ� �ٶ��ϴ�."
#define VOICE_85_FINISH_CLEAN_CARE                              85     // 'Ŭ���ɾ �Ϸ�Ǿ����ϴ�.
#define VOICE_86_PRESS_BUTTON_MORE_SETTING_MODE                 86     // '���� ��ư�� 3�� ������ �������� ��� ������ �����մϴ�.
#define VOICE_87_SETTING_MODE                                   87     // '��� ���� ���
#define VOICE_88_CLOSE_FRONT_COVER                              88     // '������ ���� Ŀ���� �ݾ��ּ���.
#define VOICE_89_CLOSE_SIDE_COVER                               89     // '������ ���� Ŀ���� �ݾ��ּ���.
#define VOICE_90_CLOSE_TOP_COVER                                90     // '����� Ŀ���� �ݾ��ּ���.
#define VOICE_91_CLOSE_FILTER_COVER                             91     // '��ü�� ���� Ŀ���� �ݾ��ּ���.
#define VOICE_92_PRESS_BUTTON_HOT_LOCK_OFF                      92     // '�¼� ��ư�� ���������, '�¼� ���' ��ư�� 3�� ���� �������ּ���
#define VOICE_93_PRESS_SETTING_MODE_HOT_LOCK_OFF                93     // '�¼� ��ư�� ���������, ���� ����� '�¼� ���' ��ư�� 3�� ���� �������ּ���
#define VOICE_94_PRESS_BUTTON_HOT_FUNCTION_ON                   94     // '�¼��� ����������, �¼���ư�� 3�� ���� ON���� �������ּ���.
#define VOICE_95_CHECK_SETTING_HOT_FUNCTION                     95     // '�¼��� ���������� ���� ��带 Ȯ�����ּ���
#define VOICE_96_CHECK_SETTING_COLD_FUNCTION                    96     // '�ü��� ���������� ���� ��带 Ȯ�����ּ���
#define VOICE_97_DISPENSE_ICE_WHILE_PRESS                       97     // '���� ���� ��ư�� ������ ���� ������ ����˴ϴ�.
#define VOICE_98_INFORM_ICE_LOCK_ON                             98     // '���� ���� ��ư�� ����ֽ��ϴ�.
#define VOICE_99_PRESS_BUTTON_ICE_FUNCTION_ON                   99     // '���� ���� ����� ����������, ���� ��ư�� 3�� ���� ON���� �������ּ���.
#define VOICE_100_SET_CLOCK_ECO_MODE                           100     // '���ڸ�� ����� ���� wifi�� �����ϰų� ���� �ð��� �������ּ���.
#define VOICE_101_NO_GUARANTEE_ICE_ECO_MODE                    101     // '���� ��� �����̹Ƿ� ������ ������� ���� �� �ֽ��ϴ�.
#define VOICE_102_PRESS_BUTTON_ALL_LOCK_OFF_A                  102     // '1) ��� ��ư�� ��������� �¼���ݰ� ������ư�� ���ÿ� 3�� ���� �������ּ���
#define VOICE_103_PRESS_SETTING_MODE_ALL_LOCK_OFF              103     // '2) ��� ��ư�� ��������� ���� ����� ��ü��� ��ư�� ���� �������ּ���"
#define VOICE_104_PRESS_BUTTON_ALL_LOCK_OFF_B                  104     // '3) ��� ��ư�� ��������� �¼��� ������ư�� ���ÿ� 3�� ���� �������ּ���
#define VOICE_105_WAIT_CLEAN_CARE                              105     // '"Ŭ���ɾ �������Դϴ�. Ŭ���ɾ� �߿��� �����⸦ ����Ͻ� �� �����ϴ�."
#define VOICE_106_SET_APP_AUTO_LOCK_OFF                        106     // '"��ü �ڵ� ����� �����Ͻ÷��� App �� ���� ����� �������ּ���."
#define VOICE_107_NO_GUARANTEE_LOW_WATER                       107     // '"������ ���� ä��� ���Դϴ�.  ��ũ�� ���� ������ �� �ֽ��ϴ�."
#define VOICE_108_WAIT_LOW_WATER                               108     // '"������ ���� ä��� ���Դϴ�.  ��ø� ��ٷ��ּ���."
#define VOICE_109_INFORM_NOT_ENOUGH_HOT                        109     // '�¼��� �������̹Ƿ�, �����̴� �µ� ���� �� ����� �߰��� �ʽ��ϴ�.
#define VOICE_110_INFORM_DISPENSE_NOT_ENOUGH_HOT               110     // '�¼��� ���� ���̹Ƿ�, ������ ��ũ �µ��� ����˴ϴ� .
#define VOICE_111_INFORM_NOT_ENOUGH_COLD                       111     // '�ü��� �ð� ���̹Ƿ�, ����� �ÿ����� ���� �� �ֽ��ϴ�.
#define VOICE_112_INFORM_HOT_LOCK_ON                           112     // '�¼� ��ư�� �����ϴ�.'
#define VOICE_113_INFORM_HOT_LOCK_OFF                          113     // '�¼��� ����Ͻ� �� �ֽ��ϴ�. '
#define VOICE_114_SETTING_MODE_JOG_DAIL                        114     // '"���� ��� �Դϴ�. ���״��̾��� ����Ͽ� ����� �����ϼ���."'
#define VOICE_115_SETTING_HOT_TANK_CONTROL_MODE                115     // '�¼���ũ �µ� ���� ���'
#define VOICE_116_START_HEATING_HOT_TANK                       116     // '�����Ͻ� �µ��� ������ �����մϴ�.'
#define VOICE_117_START_CONTROL_HOT_TANK                       117     // '�����Ͻ� �µ��� ��� �����մϴ�. '
#define VOICE_118_INFORM_ENOUGH_HOT                            118     // '��ũ�� �µ��� �����Ͻ� ��ǥ �µ��� �����Ͽ����ϴ�.'
#define VOICE_119_INFORM_TOO_HIGH_HOT                          119     // '�¼��µ� ���� ���̹Ƿ�, ������ ��ũ �µ��� �� ����˴ϴ�.'
#define VOICE_120_SETTING_MODE_HOT_TANK                        120     // '�¼� ��ũ ���� ���'
#define VOICE_121_INFORM_HOT_FUNCTION_ON                       121     // '"�¼� ����� �������ϴ�. �����Ͻ� �µ����� �ð��� �ټ� �ҿ�˴ϴ�."
#define VOICE_122_INFORM_HOT_FUNCTION_OFF                      122     // '"�¼� ����� �������ϴ�. �¼��� ����Ͻ� �� �����ϴ�."
#define VOICE_123_SETTING_MODE_COLD_TANK                       123     // '�ü� ��ũ ���� ���'
#define VOICE_124_INFORM_COLD_FUNCTION_ON                      124     // '"�ü� ����� �������ϴ�. �����Ͻ� �µ����� �ð��� �ټ� �ҿ�˴ϴ�."
#define VOICE_125_INFORM_COLD_FUNCTION_OFF                     125     // '"�ü� ����� �������ϴ�. �ü��� ����Ͻ� �� �����ϴ�."
#define VOICE_126_SETTING_MODE_ALL_LOCK                        126     // '��ü ��� ���� ����Դϴ�.
#define VOICE_127_INFORM_ALL_LOCK_ON                           127     // '��� ��ư�� �����ϴ�.
#define VOICE_128_INFORM_ALL_LOCK_OFF                          128     // '��ü ��ư ����� �����Ǿ����ϴ�.
#define VOICE_129_VOICE_MODE                                   129     // '����'
#define VOICE_130_MELODY_MODE                                  130     // 'ȿ����'
#define VOICE_131_MUTE_MODE                                    131     // '����'
#define VOICE_132_SETTING_MODE_SOUND                           132     // '�Ҹ� ���� ���'
#define VOICE_133_MODE_SET_VOICE                               133     // '����� �����Ǿ����ϴ�. (����)
#define VOICE_134_MODE_SET_MELODY                              134     // '����� �����Ǿ����ϴ�. (ȿ����)
#define VOICE_135_MODE_SET_MUTE                                135     // '����� �����Ǿ����ϴ�. (����)
#define VOICE_136_MODE_SET_COLD_ON                             136     // '����� �����Ǿ����ϴ�. (�ü� ��� ON)
#define VOICE_137_MODE_SET_COLD_OFF                            137     // '����� �����Ǿ����ϴ�. (�ü� ��� OFF)
#define VOICE_138_SETTING_MODE_CLEAN_CARE_TIME                 138     // 'Ŭ���ɾ� �ð� ���� ���
#define VOICE_139_MODE_SET_CLEAN_CARE_TIME                     139     // '����� �����Ǿ����ϴ�. (���λ�� �ð� �����Ϸ�)
#define VOICE_140_SETTING_MODE_DEFAULT_AMOUNT                  140     // '���ۿ뷮 ���� ���
#define VOICE_141_MODE_SET_DEFAULT_AMOUNT                      141     // '����� �����Ǿ����ϴ�. (���� �뷮 �����Ϸ�)
#define VOICE_142_NOT_TIME_REPLACE_FILTER_YET                  142     // '"������ ��ü ������ ���� �������� �ʾҽ��ϴ�. ������ ��ü ������ �������� �� �ȳ��帮�ڽ��ϴ�."'
#define VOICE_143_CHECK_TIME_REPLACE_FILTER                    143     // '"���� ��ü �����Դϴ�.���� ���� ���� ��ü ������ Ȯ�����ּ���."'
#define VOICE_144_SETTING_MODE_CLOCK                           144     // '�ð� ���� ���'
#define VOICE_145_MODE_SET_CLOCK                               145     // '����� �����Ǿ����ϴ�. (���� �뷮 �����Ϸ�)
#define VOICE_146_FINISH_SETTING                               146     // '������带 �����մϴ�.'
#define VOICE_147_INFORM_ICE_LOCK_ON                           147     // '���� ���� ��ư�� �����ϴ�.'
#define VOICE_148_INFORM_ICE_LOCK_OFF                          148     // '���� ���� ��ư�� ����Ͻ� �� �ֽ��ϴ�.'
#define VOICE_149_INFORM_ICE_FUNCTION_ON                       149     // '���� ���� ����� �������ϴ�.'
#define VOICE_150_INFORM_ICE_FUNCTION_OFF                      150     // '���� ���� ����� �������ϴ�.'
#define VOICE_151_SPECIAL_FUNCTION                             151     // 'Ư������Դϴ�.' (����/�����)
//---------------------------------------------------------------
//                 MELODY
//---------------------------------------------------------------
#define VOICE_192_MELODY_DISPENSE_CONTINUOUS                   192
#define VOICE_193_MELODY_DISPENSE_START                        193
#define VOICE_194_MELODY_DISPENSE_FINISH                       194
#define VOICE_195_MELODY_PLUG_IN                               195
#define VOICE_196_MELODY_SELECT                                196
#define VOICE_197_MELODY_WARNING                               197
#define VOICE_198_MELODY_SET_ON                                198
#define VOICE_199_MELODY_SET_OFF                               199
 //==============================================================


U8 IsLevel_Mute(U16 mu8Address);
U8 IsLevel_Melody(U16 mu8Address);
U8 IsLevel_Voice(U16 mu8Address);
U8 IsCriticalVoice(U16 mu8Address);

#endif

#endif

