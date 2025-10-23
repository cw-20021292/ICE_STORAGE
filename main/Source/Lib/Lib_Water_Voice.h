/// @file     Lib_Water_Voice.h
/// @date     2025/04/04
/// @author   Jaejin Ham
/// @brief    Voice Control file


#ifndef _LIB_WATER_VOICE_H_
#define _LIB_WATER_VOICE_H_


#if (VOICE_IC_USE == USE)

#define VOICE_1_SHUTOFF_WATER_FILTER_COVER                       1     // '필터 커버가 열려 물 공급을 차단합니다.'
#define VOICE_2_STOP_ICE_ICETANK_COVER                           2     // '얼음탱크 커버가 열렸습니다. 얼음 생성을 중지합니다.'
#define VOICE_3_SHUTOFF_WATER_SIDE_COVER                         3     // '측면의 필터 커버가 열려 물 공급을 차단합니다. '
#define VOICE_4_SHUTOFF_WATER_TOP_COVER                          4     // '상단의 필터 커버가 열려 물 공급을 차단합니다.'
#define VOICE_5_SHUTOFF_WATER_FRONT_COVER                        5     // '정면의 필터 커버가 열려 물 공급을 차단합니다.'
#define VOICE_6_RESTART_ICE_COVER_OK                             6     // '얼음 탱크 커버가 닫혔습니다. 얼음을 재생성 합니다.'
#define VOICE_7_CALL_AS_COLD_WATER_ERROR                         7     // '냉수 기능에 이상이 감지되었으니 고객센터에 연락해주세요'
#define VOICE_8_CLEAN_DUST_FILTER_COLD_WATER_ERROR               8     // '냉수 기능에 이상이 감지되었으니 제품의 먼지 필터를 세척해주세요.'
#define VOICE_9_CALL_AS_HOT_WATER_ERROR                          9     // '온수 기능에 이상이 감지되었으니 고객센터에 연락해주세요'
#define VOICE_10_CALL_AS_ICE_ERROR                              10     // '얼음 기능에 이상이 감지되었으니 고객센터에 연락해주세요'
#define VOICE_11_CALL_AS_WATER_SUPPLY_ERROR                     11     // '물 공급에 이상이 감지 되었으니, 원수공급 밸브를 확인해주시고 문제가 반복되면 고객센터로 연락해주세요.'
#define VOICE_12_CALL_AS_WATER_LEAKAGE_ERROR                    12     // '"누수가 감지되었으니 원수공급 밸브를 잠궈주시고 문제가 반복되면 고객센터로 연락해주세요."'
#define VOICE_13_REPLACE_ONLY_FILTER_NO1_NO3                    13     // '"1번과 3번 필터만 교체하셔야 합니다.  다시 확인하시고 1번과 3번 필터를 교체해주세요. "'
#define VOICE_14_PRESS_RESET_BUTTON_AFTER_NEW_FILTER            14     // '"필터가 연결되었습니다. 새 필터로 교체하셨으면 리셋버튼을 눌러주세요 "'
#define VOICE_15_REPLACE_ADD_FILTER_NO3_PLEASE                  15     // '"1번과 3번 필터를 교체하셔야 합니다.   3번 필터를 추가로 교체해주세요"'
#define VOICE_16_REPLACE_AFTER_CHECK_FILTER_NO1_NO3             16     // '"1번과 3번 필터를 교체하셔야 합니다.  다시 확인하시고 1번과 3번 필터를 교체해주세요."'
#define VOICE_17_REPLACE_ADD_FILTER_NO1_PLEASE                  17     // '"1번과 3번 필터를 교체하셔야 합니다.  1번 필터를 추가로 교체해주세요 "'
#define VOICE_18_CLOSE_COVER_AFTER_FILTER_NO1_NO3               18     // '1번과 3번 필터를 교체하셨다면, 필터 커버를 닫아주세요'
#define VOICE_19_PRESS_YES_OR_NO_AFTER_FILTER_NO1_NO3           19     // '1번과 3번 필터를 교체하셨다면 필터 리셋을 위해 "출수" 버튼을 눌러주세요. 새 필터로 교체하지 않으셨다면 "냉수" 버튼을 눌러주세요'
#define VOICE_20_REPLACE_AFTER_CHECK_FILTER_NO1_NO3_V2          20     // '"1번과 3번 필터를 교체하셔야 합니다.  다시 확인하시고 1번과 3번 필터를 교체해주세요."' (16번과 영문이다름)
#define VOICE_21_PRESS_YES_OR_NO_AFTER_FILTER_NO1_NO3_WELL      21     // '"1번과 3번 필터를 제대로 교체하셨다면 필터 리셋을 위해 ""출수"" 버튼을 눌러주세요. 새 필터로 교체하지 않으셨다면 ""냉수"" 버튼을 눌러주세요"'
#define VOICE_22_REPLACE_ONLY_FILTER_NO1_NO3_NOT_YET_NO2        22     // '1번과 3번 필터만 교체하셔야 합니다. 2번 필터는 교체 시기가 아니오니 다시 확인해주세요.'(한국어만 다름)
#define VOICE_23_START_FILTER_FLUSHING                          23     // '필터 플러싱을 시작합니다. 플러싱 완료 후 제품 사용이 가능합니다.'
#define VOICE_24_FINISH_FILTER_FLUSHING                         24     // '필터 플러싱이 완료되어 제품 사용이 가능합니다.'
#define VOICE_25_CHECK_FILTER_FLUSHING_MANUAL                   25     // '"필터 플러싱이 선택되지 않았습니다.  필터 교체를 진행하셨다면, 매뉴얼을 참고하여 플러싱을 다시 진행해주세요."'
#define VOICE_26_REPLACE_FILTER_NO1_NO3                         26     // '1번과 3번 필터를 교체해주세요. '(한국어)
#define VOICE_27_INFORM_ACTIVATED_FILTER_NO1_ADD_NO3            27     // '"1번 필터의 리셋 버튼이 활성화 되었습니다. 3번 필터를 추가로 교체해 주세요"'
#define VOICE_28_INFORM_ACTIVATED_FILTER_NO3_ADD_NO1            28     // '"3번 필터의 리셋 버튼이 활성화 되었습니다. 1번 필터를 추가로 교체해 주세요 "'
#define VOICE_29_CLOSE_COVER_FOR_FILTER_FLUSHING                29     // '"필터 교체가 완료되었습니다.  필터 커버를 닫으면 필터 플러싱을 시작합니다."'
#define VOICE_30_START_FILTER_FLUSHING_NO1_NO3                  30     // '1번과 3번 필터의 플러싱을 시작합니다. 플러싱 완료 후 제품 사용이 가능합니다.'(한국어)
#define VOICE_31_PRESS_RESET_BUTTON_AFTER_NEW_FILTER            31     // '필터를 교체 하신 후 리셋버튼을 눌러주세요.'
#define VOICE_32_PRESS_RESET_BUTTON_FILTER_NO1                  32     // '"1번 필터 리셋 버튼이 활성화 되지 않았습니다. 정면의 필터 커버를 열어 1번 필터 리셋버튼을 눌러주세요."'
#define VOICE_33_PRESS_RESET_BUTTON_FILTER_NO3                  33     // '"3번 필터 리셋 버튼이 활성화 되지 않았습니다.  정면의 필터 커버를 열어 3번 필터 리셋버튼을 눌러주세요."'
#define VOICE_34_REPLACE_ADD_FILTER_NO3                         34     // '"1번과 3번 필터를 교체하셔야 합니다.   3번 필터를 추가로 교체해 주시기 바랍니다."' (주세요랑 다름)
#define VOICE_35_REPLACE_ADD_FILTER_NO1                         35     // '"1번과 3번 필터를 교체하셔야 합니다.  1번 필터를 추가로 교체해 주시기 바랍니다. "'(주세요랑 다름)
#define VOICE_36_CANCEL_RESET_FILTER_NO1                        36     // '1번 필터 리셋을 취소합니다.'
#define VOICE_37_CANCEL_RESET_FILTER_NO2                        37     // '2번 필터 리셋을 취소합니다.'
#define VOICE_38_CANCEL_RESET_FILTER_NO3                        38     // '3번 필터 리셋을 취소합니다.'
#define VOICE_39_CANCEL_FILTER_FLUSHING                         39     // '"필터 플러싱이 취소되었습니다. 다시 필터 플러싱을 진행하시려면, 필터를 제거 후 다시 결합하고 리셋버튼을 눌러주세요. ."'
                                                                       // '필터가 연결되었습니다. 새 필터로 교체하셨으면 리셋버튼을 눌러주세요 '(국문)
#define VOICE_40_PRESS_RESET_BUTTON_NO1                         40     // 'If number 1 filter is replaced, please press the reset button.'(영문)
#define VOICE_41_PRESS_RESET_BUTTON_NO2                         41     // 'If number 2 filter is replaced, please press the reset button.'(영문)
#define VOICE_42_PRESS_RESET_BUTTON_NO3                         42     // 'If number 3 filter is replaced, please press the reset button.'(영문)
#define VOICE_43_REPLACE_ADD_FILTER_NO2_NO3_FOR_ALL             43     // '모든 필터를 교체하셔야 하니 2번과 3번 필터를 추가로 교체해주세요'
#define VOICE_44_REPLACE_ADD_FILTER_NO1_NO3_FOR_ALL             44     // '모든 필터를 교체하셔야 하니 1번과 3번 필터를 추가로 교체해주세요'
#define VOICE_45_REPLACE_ADD_FILTER_NO1_NO2_FOR_ALL             45     // '모든 필터를 교체하셔야 하니 1번과 2번 필터를 추가로 교체해주세요'
#define VOICE_46_PRESS_YES_OR_NO_AFTER_ALL_FILTERS              46     // '모든 필터를 교체하셨다면 필터 리셋을 위해 "출수" 버튼을 눌러주세요. 새 필터로 교체하지 않으셨다면 "냉수 버튼을 눌러주세요'
#define VOICE_47_CHECK_REPLACE_ALL_FILTERS                      47     // '모든 필터를 교체하셔야 하니, 다시 확인해주세요.'
#define VOICE_48_NOT_SELECTED_FILTER_FLUSHING                   48     // '필터 플러싱이 선택되지 않았습니다. 바로 제품 사용이 가능합니다.'
#define VOICE_49_REPLACE_FILTER_ALL                             49     // '모든 필터를 교체해주세요.'
#define VOICE_50_INFORM_ACTIVATION_ADD_OTHER_FILTER             50     // '"필터 리셋 버튼이 활성화 되었습니다. 나머지 필터를 추가로 교체해주세요"'

#define VOICE_51_CLOSE_COVER_FOR_FILTER_FLUSHING_V2             51     // '"필터 교체가 완료되었습니다.  필터 커버를 닫으면 필터 플러싱을 시작합니다."' (영문이 다름)

#define VOICE_52_PRESS_RESET_BUTTON_AFTER_REPLACE_FILTER        52     // '필터를 교체하신 후 리셋버튼을 눌러주세요'
#define VOICE_53_PRESS_RESET_BUTTON_DEACTIVATED                 53     // '필터 플러싱을 시작합니다.  플러싱 완료 후 제품 사용이 가능합니다.'
#define VOICE_54_REPLACE_ADD_FILTER_NO3_FOR_ALL                 54     // '"모든 필터를 교체하셔야 합니다.  3번 필터를 추가로 교체해주세요" '
#define VOICE_55_REPLACE_ADD_FILTER_NO1_FOR_ALL                 55     // '"모든 필터를 교체하셔야 합니다.  1번 필터를 추가로 교체해주세요" '
#define VOICE_56_REPLACE_ADD_FILTER_NO2_FOR_ALL                 56     // '"모든 필터를 교체하셔야 합니다.  2번 필터를 추가로 교체해주세요" '

#define VOICE_57_PRESS_DISPENSE_BUTTON_AFTER_WATER_VALVE_OPEN   57     // '"원수 공급 밸브를 확인하시어 공급밸브를 열고 추출버튼을 눌러주세요." '
#define VOICE_58_PRESS_YES_OR_NO_AFTER_FAUCET                   58     // '유로 파우셋을 교체하셨다면 "출수" 버튼을 눌러주세요. 교체하지 않으셨다면 "냉수 버튼을 눌러주세요'
#define VOICE_59_PRESS_YES_OR_NO_AFTER_FILTER_FAUCET            59     // '필터와 유로 파우셋을 교체하셨다면 "출수" 버튼을 눌러주세요. 교체하지 않으셨다면 "냉수 버튼을 눌러주세요'
#define VOICE_60_WAIT_ICE_TANK_CLEANING                         60     // '얼음 탱크 세척 중에는 얼음과 물을 사용하실 수 없습니다'
#define VOICE_61_START_SMART_TESTING_MODE                       61     // '스마트 진단을 시작합니다. 진행 중에는 제품을 사용하실 수 없으니 잠시만 기다려 주세요.'
#define VOICE_62_FINISH_SMART_TESTING_MODE_NO_ERROR             62     // '스마트 진단이 완료 되어 제품을 사용하실 수 있습니다. '
#define VOICE_63_FINISH_SMART_TESTING_MODE_WITH_ERROR           63     // '스마트 진단이 완료 되었습니다.'
#define VOICE_64_INFORM_BLE_MODE_ACTIVATED                      64     // '블루투스 연결 모드가 활성화 되었습니다. '
#define VOICE_65_INFORM_BLE_MODE_DEACTIVATED                    65     // '블루투스 연결 모드가 비활성화 되었습니다.'
#define VOICE_66_INFORM_WIFI_MODE_ACTIVATED                     66     // 'Wi-Fi 연결 모드가 활성화 되었습니다. '
#define VOICE_67_INFORM_WIFI_MODE_DEACTIVATED                   67     // 'Wi-Fi 연결 모드가 비활성화 되었습니다.'
#define VOICE_68_INFORM_WIFI_ON                                 68     // '제품의 Wi-Fi가 켜졌습니다.'
#define VOICE_69_INFORM_WIFI_OFF                                69     // '제품의 Wi-Fi가 꺼졌습니다.'
#define VOICE_70_PRESS_BUTTON_ACCORDING_TO_APP_GUIDE            70     // '"제품 인증을 진행 중입니다. 앱에서 가이드하는 버튼을 눌러 주세요." '
#define VOICE_71_INFORM_APP_PAIRING_SUCCESS                     71     // '제품과 앱이 성공적으로 연결되었습니다.'
#define VOICE_72_INFORM_APP_PAIRING_FAIL                        72     // '제품과 앱의 연결이 실패하였습니다. '
#define VOICE_73_INFORM_APP_DISCONNECTED                        73     // '제품과 앱의 연결이 끊어졌습니다.'
#define VOICE_74_INFORM_WIFI_OFF                                74     // '제품의 Wi-Fi가 꺼졌습니다. '
#define VOICE_75_PRESS_INSTALL_BUTTON                           75     // '정수기 설치를 위해 추출버튼을 눌러주세요'
#define VOICE_76_START_WIFI_AUTOMATIC_CONNECTION                76     // '와이파이 연결을 자동으로 시작합니다.'
#define VOICE_77_WAIT_WATER_PIPE_DRAIN                          77     // '"깨끗한 물 제공을 위해 유로의 물을 비우고 있습니다.   잠시만 기다려주세요"
#define VOICE_78_DISCARD_FIRST_CUP                              78     // '깨끗한 물을 드시기 위해 첫 컵은 버리고 음용해 주세요.
#define VOICE_79_WATCHOUT_HOT_WATER_CUP                         79     // '온수가 뜨거우니 주의하세요.
#define VOICE_80_WATCHOUT_HOT_WATER_CONTINUOUS                  80     // '"온수가 뜨거우니 주의하세요.  추출을 멈추시려면 추출 버튼을 눌러주세요."
#define VOICE_81_SETTING_MODE_CLEAN_CARE                        81     // '클린케어 모드
#define VOICE_82_START_CLEAN_CARE_WATCHOUT_FAUCET               82     // '"클린케어를 진행중에 온수가 튈 수 있으니 파우셋 밑에 알맞은 컵을 놓으시거나 파우셋 추출부를 싱크대로 향하게 해주세요."
#define VOICE_83_PRESS_BUTTON_MANUAL_CLEAN_CARE                 83     // '준비가 완료되었다면 '출수 버튼'을 3초 이상 눌러주세요
#define VOICE_84_START_CLEAN_CARE_WATCHOUT_HOT                  84     // '"클린케어를 시작합니다.  추출부로 뜨거운 물이 추출되니 화상을 입지 않도록 조심하시기 바랍니다."
#define VOICE_85_FINISH_CLEAN_CARE                              85     // '클린케어가 완료되었습니다.
#define VOICE_86_PRESS_BUTTON_MORE_SETTING_MODE                 86     // '설정 버튼을 3초 누르면 정수기의 기능 설정이 가능합니다.
#define VOICE_87_SETTING_MODE                                   87     // '기능 설정 모드
#define VOICE_88_CLOSE_FRONT_COVER                              88     // '전면의 필터 커버를 닫아주세요.
#define VOICE_89_CLOSE_SIDE_COVER                               89     // '측면의 필터 커버를 닫아주세요.
#define VOICE_90_CLOSE_TOP_COVER                                90     // '상단의 커버를 닫아주세요.
#define VOICE_91_CLOSE_FILTER_COVER                             91     // '본체의 필터 커버를 닫아주세요.
#define VOICE_92_PRESS_BUTTON_HOT_LOCK_OFF                      92     // '온수 버튼이 잠겨있으니, '온수 잠금' 버튼을 3초 눌러 해제해주세요
#define VOICE_93_PRESS_SETTING_MODE_HOT_LOCK_OFF                93     // '온수 버튼이 잠겨있으니, 설정 모드의 '온수 잠금' 버튼을 3초 눌러 해제해주세요
#define VOICE_94_PRESS_BUTTON_HOT_FUNCTION_ON                   94     // '온수가 꺼져있으니, 온수버튼을 3초 눌러 ON으로 설정해주세요.
#define VOICE_95_CHECK_SETTING_HOT_FUNCTION                     95     // '온수가 꺼져있으니 설정 모드를 확인해주세요
#define VOICE_96_CHECK_SETTING_COLD_FUNCTION                    96     // '냉수가 꺼져있으니 설정 모드를 확인해주세요
#define VOICE_97_DISPENSE_ICE_WHILE_PRESS                       97     // '얼음 추출 버튼을 누르는 동안 얼음이 추출됩니다.
#define VOICE_98_INFORM_ICE_LOCK_ON                             98     // '얼음 추출 버튼이 잠겨있습니다.
#define VOICE_99_PRESS_BUTTON_ICE_FUNCTION_ON                   99     // '얼음 생성 기능이 꺼져있으니, 얼음 버튼을 3초 눌러 ON으로 설정해주세요.
#define VOICE_100_SET_CLOCK_ECO_MODE                           100     // '에코모드 사용을 위해 wifi를 연결하거나 현재 시간을 설정해주세요.
#define VOICE_101_NO_GUARANTEE_ICE_ECO_MODE                    101     // '에코 모드 상태이므로 얼음이 충분하지 않을 수 있습니다.
#define VOICE_102_PRESS_BUTTON_ALL_LOCK_OFF_A                  102     // '1) 모든 버튼이 잠겨있으니 온수잠금과 정수버튼을 동시에 3초 눌러 해제해주세요
#define VOICE_103_PRESS_SETTING_MODE_ALL_LOCK_OFF              103     // '2) 모든 버튼이 잠겨있으니 설정 모드의 전체잠금 버튼을 눌러 해제해주세요"
#define VOICE_104_PRESS_BUTTON_ALL_LOCK_OFF_B                  104     // '3) 모든 버튼이 잠겨있으니 온수와 정수버튼을 동시에 3초 눌러 해제해주세요
#define VOICE_105_WAIT_CLEAN_CARE                              105     // '"클린케어가 진행중입니다. 클린케어 중에는 정수기를 사용하실 수 없습니다."
#define VOICE_106_SET_APP_AUTO_LOCK_OFF                        106     // '"전체 자동 잠금을 해지하시려면 App 을 통해 기능을 해제해주세요."
#define VOICE_107_NO_GUARANTEE_LOW_WATER                       107     // '"깨끗한 물을 채우는 중입니다.  탱크에 물이 부족할 수 있습니다."
#define VOICE_108_WAIT_LOW_WATER                               108     // '"깨끗한 물을 채우는 중입니다.  잠시만 기다려주세요."
#define VOICE_109_INFORM_NOT_ENOUGH_HOT                        109     // '온수를 가열중이므로, 깜빡이는 온도 선택 시 충분히 뜨겁지 않습니다.
#define VOICE_110_INFORM_DISPENSE_NOT_ENOUGH_HOT               110     // '온수를 가열 중이므로, 현재의 탱크 온도로 출수됩니다 .
#define VOICE_111_INFORM_NOT_ENOUGH_COLD                       111     // '냉수를 냉각 중이므로, 충분히 시원하지 않을 수 있습니다.
#define VOICE_112_INFORM_HOT_LOCK_ON                           112     // '온수 버튼이 잠겼습니다.'
#define VOICE_113_INFORM_HOT_LOCK_OFF                          113     // '온수를 사용하실 수 있습니다. '
#define VOICE_114_SETTING_MODE_JOG_DAIL                        114     // '"설정 모드 입니다. 조그다이얼을 사용하여 기능을 설정하세요."'
#define VOICE_115_SETTING_HOT_TANK_CONTROL_MODE                115     // '온수탱크 온도 제어 모드'
#define VOICE_116_START_HEATING_HOT_TANK                       116     // '설정하신 온도로 가열을 시작합니다.'
#define VOICE_117_START_CONTROL_HOT_TANK                       117     // '설정하신 온도로 제어를 시작합니다. '
#define VOICE_118_INFORM_ENOUGH_HOT                            118     // '탱크의 온도가 설정하신 목표 온도에 도달하였습니다.'
#define VOICE_119_INFORM_TOO_HIGH_HOT                          119     // '온수온도 조절 중이므로, 현재의 탱크 온도로 곧 출수됩니다.'
#define VOICE_120_SETTING_MODE_HOT_TANK                        120     // '온수 탱크 설정 모드'
#define VOICE_121_INFORM_HOT_FUNCTION_ON                       121     // '"온수 기능이 켜졌습니다. 설정하신 온도까지 시간이 다소 소요됩니다."
#define VOICE_122_INFORM_HOT_FUNCTION_OFF                      122     // '"온수 기능이 꺼졌습니다. 온수를 사용하실 수 없습니다."
#define VOICE_123_SETTING_MODE_COLD_TANK                       123     // '냉수 탱크 설정 모드'
#define VOICE_124_INFORM_COLD_FUNCTION_ON                      124     // '"냉수 기능이 켜졌습니다. 설정하신 온도까지 시간이 다소 소요됩니다."
#define VOICE_125_INFORM_COLD_FUNCTION_OFF                     125     // '"냉수 기능이 꺼졌습니다. 냉수를 사용하실 수 없습니다."
#define VOICE_126_SETTING_MODE_ALL_LOCK                        126     // '전체 잠금 설정 모드입니다.
#define VOICE_127_INFORM_ALL_LOCK_ON                           127     // '모든 버튼이 잠겼습니다.
#define VOICE_128_INFORM_ALL_LOCK_OFF                          128     // '전체 버튼 잠금이 해제되었습니다.
#define VOICE_129_VOICE_MODE                                   129     // '음성'
#define VOICE_130_MELODY_MODE                                  130     // '효과음'
#define VOICE_131_MUTE_MODE                                    131     // '무음'
#define VOICE_132_SETTING_MODE_SOUND                           132     // '소리 설정 모드'
#define VOICE_133_MODE_SET_VOICE                               133     // '기능이 설정되었습니다. (음성)
#define VOICE_134_MODE_SET_MELODY                              134     // '기능이 설정되었습니다. (효과음)
#define VOICE_135_MODE_SET_MUTE                                135     // '기능이 설정되었습니다. (무음)
#define VOICE_136_MODE_SET_COLD_ON                             136     // '기능이 설정되었습니다. (냉수 기능 ON)
#define VOICE_137_MODE_SET_COLD_OFF                            137     // '기능이 설정되었습니다. (냉수 기능 OFF)
#define VOICE_138_SETTING_MODE_CLEAN_CARE_TIME                 138     // '클린케어 시간 설정 모드
#define VOICE_139_MODE_SET_CLEAN_CARE_TIME                     139     // '기능이 설정되었습니다. (유로살균 시간 설정완료)
#define VOICE_140_SETTING_MODE_DEFAULT_AMOUNT                  140     // '시작용량 설정 모드
#define VOICE_141_MODE_SET_DEFAULT_AMOUNT                      141     // '기능이 설정되었습니다. (시작 용량 설정완료)
#define VOICE_142_NOT_TIME_REPLACE_FILTER_YET                  142     // '"필터의 교체 시점이 아직 도래하지 않았습니다. 필터의 교체 시점이 도래했을 때 안내드리겠습니다."'
#define VOICE_143_CHECK_TIME_REPLACE_FILTER                    143     // '"필터 교체 정보입니다.필터 별로 필터 교체 시점을 확인해주세요."'
#define VOICE_144_SETTING_MODE_CLOCK                           144     // '시간 설정 모드'
#define VOICE_145_MODE_SET_CLOCK                               145     // '기능이 설정되었습니다. (시작 용량 설정완료)
#define VOICE_146_FINISH_SETTING                               146     // '설정모드를 종료합니다.'
#define VOICE_147_INFORM_ICE_LOCK_ON                           147     // '얼음 추출 버튼이 잠겼습니다.'
#define VOICE_148_INFORM_ICE_LOCK_OFF                          148     // '얼음 추출 버튼을 사용하실 수 있습니다.'
#define VOICE_149_INFORM_ICE_FUNCTION_ON                       149     // '얼음 생성 기능이 켜졌습니다.'
#define VOICE_150_INFORM_ICE_FUNCTION_OFF                      150     // '얼음 생성 기능이 꺼졌습니다.'
#define VOICE_151_SPECIAL_FUNCTION                             151     // '특수모드입니다.' (개발/생산용)
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

