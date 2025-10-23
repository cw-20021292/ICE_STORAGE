/// @file     Lib_Water_ErrorCheck.h
/// @date     2025/06/10
/// @author   Jaejin Ham
/// @brief    정수기 Error 관련 제어 file


#ifndef _LIB_WATER_ERROR_CHECK_H_
#define _LIB_WATER_ERROR_CHECK_H_


/*  Lib Function 사용법
    => Set_FlowMeterErrorCheckStart(mu8Status);         // 유량 센서 에러 체크 시작 상태 설정 - 0(미시작), 1(시작)
    => Get_FlowMeterErrorCheckStart();                  // 유량 센서 에러 체크 시작 상태 확인 - 0(미시작), 1(시작)
    => Set_FlowMeterErrorCheckDelayTime(mu16Time);      // 유량 센서 에러 체크 제어 전 Delay 시간 설정 - 100ms단위
    => Get_FlowMeterErrorCheckDelayTime();              // 유량 센서 에러 체크 제어 전 Delay 시간 확인 - 100ms단위

    => Set_EffluentCloggingErrorCheckStart(mu8Status);  // 추출 막힘 에러 체크 시작 상태 설정 - 0(미시작), 1(시작)
    => Get_EffluentCloggingErrorCheckStart();           // 추출 막힘 에러 체크 시작 상태 확인 - 0(미시작), 1(시작)

    => Set_HotCutOffCheckStart(mu8Status);              // 온수 단수 및 배수 막힘 에러 체크 시작 상태 설정 - 0(미시작), 1(시작)
    => Get_HotCutOffCheckStart();                       // 온수 단수 및 배수 막힘 에러 체크 시작 상태 확인 - 0(미시작), 1(시작)

    => Get_ErrorDisplayCode();                          // 우선 순위에 따라 현재 표시되어야 하는 에러에 해당하는 디스플레이 표시 Code 확인
    => Get_ErrorOccuredCheck();                         // 에러 발생이 하나라도 발생된 것이 있는지 확인
    => Get_ErrorStatus(mu8ID);                          // ID에 해당하는 에러 발생 상태 확인 - 0(FALSE, 미발생), 1(TRUE, 발생)
    => Get_ErrorDetect(mu8ID);                          // ID에 해당하는 에러 감지 상태 확인(에러는 미발생) - 0(FALSE, 미감지), 1(TRUE, 감지)

    => Get_ErrorAboutWaterInput();                      // 입수 동작과 관련된 에러 발생 여부 확인 - 0(FALSE, 미발생), 1(TRUE, 발생)
    => Get_AirTempError();                              // 외기 온도 센서 에러 발생 여부 확인 - 0(FALSE, 미발생), 1(TRUE, 발생)
    => Get_HotError();                                  // 온수 관련 에러 발생 여부 확인 - 0(FALSE, 미발생), 1(TRUE, 발생)
    => Get_ColdError();                                 // 냉수 관련 에러 발생 여부 확인 - 0(FALSE, 미발생), 1(TRUE, 발생)
    => Get_ErrorAboutIceMaking();                       // 제빙 관련 에러 발생 여부 확인 - 0(FALSE, 미발생), 1(TRUE, 발생)
*/



/// @brief      정수기 Error Code List(미사용 에러 삭제 금지)
/// @details    정수기 Error Code List를 발생 우선 순위 대로 작성한다
typedef enum
{
    ERROR_ID_NONE_E00 = 0,                              // 0 : Error None
    ERROR_ID_LEAKAGE_E01,                               // 1 : 누수
    ERROR_ID_FLOW_SENSOR_2_E06,                         // 2 : 유량센서2
    ERROR_ID_FLOW_SENSOR_1_E05,                         // 3 : 유량센서1
    ERROR_ID_WATER_CUT_OFF_E09,                         // 4 : 단수
    ERROR_ID_ROOM_OVERHEAT_E49,                         // 5 : 정수 과열
    ERROR_ID_FILTER_CLOGGING_E07,                       // 6 : 필터 막힘
    ERROR_ID_EFFLUENT_CLOGGING_E04,                     // 7 : 추출 막힘(유량센서3)
    ERROR_ID_HOT_CUT_OFF_E08,                           // 8 : 순간 온수 단수 및 배수 막힘
    ERROR_ID_BOOST_PUMP_E95,                            // 9 : 부스트펌프(수배관 막힘)
    ERROR_ID_PRESSURE_SENSOR_E97,                       // 10 : 압력 센서
    ERROR_ID_TRAY_MOTOR_E96,                            // 11 : 추출 트레이 모터
    ERROR_ID_MICRO_SW_ALL_DETECT_E61,                   // 12 : 마이크로 SW 동시 감지
    ERROR_ID_MICRO_SW_ICE_MAKING_E62,                   // 13 : 마이크로 SW 이동(제빙)
    ERROR_ID_MICRO_SW_DE_ICE_E63,                       // 14 : 마이크로 SW 이동(탈빙)
    ERROR_ID_DRAIN_PUMP_E60,                            // 15 : 드레인 펌프
    ERROR_ID_SODA_PUMP_E94,                             // 16 : 탄산 펌프
    ERROR_ID_ABNORMAL_EFFLUENT_E03,                     // 17 : 추출 동작 이상
    ERROR_ID_LOW_SENSOR_E11,                            // 18 : 저수위 센서
    ERROR_ID_MIDDLE_SENSOR_E12,                         // 19 : 중수위 센서
    ERROR_ID_HIGH_SENSOR_E13,                           // 20 : 만수위 센서
    ERROR_ID_COMPLEX_SENSOR_E14,                        // 21 : 수위 센서 복합(중수위 없는 경우)
    ERROR_ID_LOW_MIDDLE_SENSOR_E15,                     // 22 : 저, 중수위 센서 복합
    ERROR_ID_MIDDLE_HIGH_SENSOR_E16,                    // 23 : 중, 만수위 센서 복합
    ERROR_ID_OVER_FLOW_E17,                             // 24 : OVER FLOW 센서(중수위 있는 경우)
    ERROR_ID_FEED_VALVE_E10,                            // 25 : 피드 밸브
    ERROR_ID_HOT_HIGH_SENSOR_E33,                       // 26 : 온수 만수위 센서
    ERROR_ID_ICE_COLD_WATER_TANK_SENSOR_E20,            // 27 : 빙축 탱크 수위 센서
    ERROR_ID_COLD_LOW_SENSOR_E21,                       // 28 : 냉수 저수위 센서
    ERROR_ID_COLD_HIGH_SENSOR_E23,                      // 29 : 냉수 만수위 센서
    ERROR_ID_COLD_COMPLEX_SENSOR_E24,                   // 30 : 냉수 수위 복합
    ERROR_ID_ROOM_TEMP_2_E52,                           // 31 : 정수2 온도 센서
    ERROR_ID_ROOM_TEMP_1_E42,                           // 32 : 정수1 온도 센서
    ERROR_ID_TANK_HOT_TEMP_E45,                         // 33 : 온수 온도 센서(탱크형)
    ERROR_ID_HOT_IN_TEMP_E46,                           // 34 : 온수 입수 온도 센서(순간온수)
    ERROR_ID_HOT_OUT_TEMP_E47,                          // 35 : 온수 출수 온도 센서(순간온수)
    ERROR_ID_HOT_BODY_TEMP_E48,                         // 36 : 히터 내부 온도 센서(순간온수)
    ERROR_ID_COLD_TEMP_1_E44,                           // 37 : 냉수 온도 센서1
    ERROR_ID_COLD_TEMP_2_E54,                           // 38 : 냉수 온도 센서2
    ERROR_ID_ICE_TRAY_INPUT_TEMP_E56,                   // 39 : 아이스 트레이 입수 온도 센서
    ERROR_ID_AIR_TEMP_1_E43,                            // 40 : 외기 온도 센서1
    ERROR_ID_AIR_TEMP_2_E53,                            // 41 : 외기 온도 센서2
    ERROR_ID_TEM_HEATSINK_TEMP_E25,                     // 42 : TEM 방열판 온도 센서
    ERROR_ID_HOT_OVERHEAT_E40,                          // 43 : 온수 과열
    ERROR_ID_HOT_PUMP_E30,                              // 44 : 온수 펌프
    ERROR_ID_BLDC_COMP_COMMUNICATION_E27,               // 45 : BLDC 컴프 통신 불량
    ERROR_ID_COMP_ACT_E29,                              // 46 : 일반 컴프 동작 불량
    ERROR_ID_BLDC_COMP_ACT_1_E81,                       // 47 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x81
    ERROR_ID_BLDC_COMP_ACT_2_E82,                       // 48 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x82
    ERROR_ID_BLDC_COMP_ACT_3_E83,                       // 49 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x83
    ERROR_ID_BLDC_COMP_ACT_4_E84,                       // 50 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x84
    ERROR_ID_BLDC_COMP_ACT_5_E85,                       // 51 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x85
    ERROR_ID_BLDC_COMP_ACT_6_E86,                       // 52 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x86
    ERROR_ID_CIRCULATE_PUMP_E26,                        // 53 : 순환 펌프
    ERROR_ID_STIRRING_MOTOR_E28,                        // 54 : 교반 모터
    ERROR_ID_EW_MODULE_E71,                             // 55 : EW 살균 모듈
    ERROR_ID_EW_DRAIN_PUMP_E72,                         // 56 : EW 살균 드레인 펌프
    ERROR_ID_UV_1_E73,                                  // 57 : UV1(정수 탱크)
    ERROR_ID_UV_2_E74,                                  // 58 : UV2(냉수 탱크 뒤)
    ERROR_ID_UV_3_E75,                                  // 59 : UV3(얼음 탱크)
    ERROR_ID_UV_4_E76,                                  // 60 : UV4(얼음 트레이)
    ERROR_ID_UV_5_E77,                                  // 61 : UV5(추출 파우셋)
    ERROR_ID_UV_6_E78,                                  // 62 : UV6(얼음 파우셋)
    ERROR_ID_UV_7_E79,                                  // 63 : UV7(냉수 탱크 앞)
    ERROR_ID_TDS_IN_TEMP_E90,                           // 64 : TDS IN 온도 센서
    ERROR_ID_TDS_OUT_TEMP_E91,                          // 65 : TDS OUT 온도 센서
    ERROR_ID_TDS_IN_E92,                                // 66 : TDS IN 센서
    ERROR_ID_TDS_OUT_E93,                               // 67 : TDS OUT 센서
    ERROR_ID_INPUT_CONTAMINATION_E98,                   // 68 : 입수 오염
    ERROR_ID_DECONTAMINATION_E99,                       // 69 : 제거율 불량
    ERROR_ID_MAX
} Error_Data_T;


/// @brief      에러 발생 표시 Code List(미사용 에러 삭제 금지)
/// @details    에러 발생 표시 Code 데이터들을 리스트로 정의한다
__far const static U8 gu8ErrorDisplayCode[ERROR_ID_MAX] = {
    0x00,           // 0 : Error None
    0x01,           // 1 : 누수
    0x06,           // 2 : 유량센서2(빌트인 모델에 한함, INNO 후단 위치, 정량 추츨용)
    0x05,           // 3 : 유량센서1(빌트인 모델에 한함, NEO-RO 사이 위치, 단수 검사용)
    0x09,           // 4 : 단수(유량센서)
    0x49,           // 5 : 정수 과열
    0x07,           // 6 : 필터 막힘
    0x04,           // 7 : 추출 막힘(유량센서3)
    0x08,           // 8 : 순간 온수 단수 및 배수 막힘
    0x95,           // 9 : 부스트펌프(수배관 막힘)
    0x97,           // 10 : 압력 센서
    0x96,           // 11 : 추출 트레이 모터
    0x61,           // 12 : 마이크로 SW 동시 감지
    0x62,           // 13 : 마이크로 SW 이동(제빙)
    0x63,           // 14 : 마이크로 SW 이동(탈빙)
    0x60,           // 15 : 드레인 펌프
    0x94,           // 16 : 탄산 펌프
    0x03,           // 17 : 추출 동작 이상
    0x11,           // 18 : 저수위 센서
    0x12,           // 19 : 중수위 센서
    0x13,           // 20 : 만수위 센서
    0x14,           // 21 : 수위 센서 복합(중수위 없는 경우)
    0x15,           // 22 : 저, 중수위 센서 복합
    0x16,           // 23 : 중, 만수위 센서 복합
    0x17,           // 24 : OVER FLOW 센서(중수위 있는 경우)
    0x10,           // 25 : 피드 밸브
    0x33,           // 26 : 온수 만수위 센서
    0x20,           // 27 : 빙축 탱크 수위 센서
    0x21,           // 28 : 냉수 저수위 센서
    0x23,           // 29 : 냉수 만수위 센서
    0x24,           // 30 : 냉수 수위 복합
    0x52,           // 31 : 정수2 온도 센서
    0x42,           // 32 : 정수1 온도 센서
    0x45,           // 33 : 온수 온도 센서(탱크형)
    0x46,           // 34 : 온수 입수 온도 센서(순간온수)
    0x47,           // 35 : 온수 출수 온도 센서(순간온수)
    0x48,           // 36 : 히터 내부 온도 센서(순간온수)
    0x44,           // 37 : 냉수 온도 센서1
    0x54,           // 38 : 냉수 온도 센서2
    0x56,           // 39 : 아이스 트레이 입수 온도 센서
    0x43,           // 40 : 외기 온도 센서1
    0x53,           // 41 : 외기 온도 센서2
    0x25,           // 42 : TEM 방열판 온도 센서
    0x40,           // 43 : 온수 과열
    0x30,           // 44 : 온수 펌프
    0x27,           // 45 : BLDC 컴프 통신 불량
    0x29,           // 46 : 일반 컴프 동작 불량
    0x81,           // 47 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x81
    0x82,           // 48 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x82
    0x83,           // 49 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x83
    0x84,           // 50 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x84
    0x85,           // 51 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x85
    0x86,           // 52 : BLDC 컴프 동작 불량(통신 에러 코드) - 0x86
    0x26,           // 53 : 순환 펌프
    0x28,           // 54 : 교반 모터
    0x71,           // 55 : EW 살균 모듈
    0x72,           // 56 : EW 살균 드레인 펌프프
    0x73,           // 57 : UV1(정수 탱크)
    0x74,           // 58 : UV2(냉수 탱크 뒤)
    0x75,           // 59 : UV3(얼음 탱크)
    0x76,           // 60 : UV4(얼음 트레이)
    0x77,           // 61 : UV5(추출 파우셋)
    0x78,           // 62 : UV6(얼음 파우셋)
    0x79,           // 63 : UV7(냉수 탱크 앞)
    0x90,           // 64 : TDS IN 온도 센서
    0x91,           // 65 : TDS OUT 온도 센서
    0x92,           // 66 : TDS IN 센서
    0x93,           // 67 : TDS OUT 센서
    0x98,           // 68 : 입수 오염
    0x99,           // 69 : 제거율 불량
};


#define ERROR_CHECK_PERIOD_TIME                     100         // 에러 체크 주기 타이머 100ms@1ms

// 누수 감지
#define LEAKAGE_ERROR_CHECK_USE                     USE         // 누수 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define LEAKAGE_ERROR_DETECT_TIME                   30          // 누수 감지 시간 3sec@100ms
#define LEAKAGE_ERROR_UNDETECT_TIME                 600         // 누수 미감지 시간 60sec@100ms

// 유량센서1, 2 에러 감지 - 빌트인 모델에서 유량센서를 2개 사용한 경우에만 사용
#define FLOW_SENSOR_ERROR_CHECK_USE                 USE         // 유량센서1, 유량센서2 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define FLOW_SENSOR_ERROR_CHECK_DELAY_TIME          300         // 유량센서1, 유량센서2 에러 감지 시작 전 지연 시간 30sec@100ms
#define FLOW_SENSOR_ERROR_DETECT_TIME               600         // 유량 센서 에러 감지 시간 60sec@100ms
#define FLOW_SENSOR_ERROR_HZ                        5           // 유량 센서 이상 감지 기준 Hz

// 단수 에러 감지 - 유량센서 2개 이상 사용시에는 이것을 사용하지 말고 유량센서1, 2 에러 체크를 사용할 것
#define WATER_CUT_OFF_ERROR_CHECK_USE               USE         // 단수 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define WATER_CUT_OFF_DETECT_TIME                   100         // 단수 감지 시간 10sec@100ms
#define WATER_CUT_OFF_HZ                            5           // 단수 감지 기준 Hz
#define WTAER_CUT_OFF_AUTO_CLEAR_TIME               600         // 단수 에러 자동 해제 시간 60sec@100ms

// 정수 온도 과열 에러 감지
#define ROOM_WATER_OVERHEAT_ERROR_CHECK_USE         USE         // 정수 과열 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define ROOM_WATER_OVERHEAT_DETECT_TIME             1800        // 정수 과열 감지 시간 3min = 180sec@100ms

// 필터 막힘 에러 감지 - 미사용, 표준 동작 없음
#define FILTER_CLOGGING_ERROR_CHECK_USE             NO_USE      // 필터 막힘 에러 감지 사용 여부 - 0(NO_USE), 1(USE)

// 추출 막힘 에러 감지
#define EFFLUENT_CLOGGING_ERROR_CHECK_USE           USE         // 추출 막힘 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define EFFLUENT_CLOGGING_ERROR_FLOWMETER_USE       NO_USE      // 추출 막힘 에러 감지시 유량센서 사용 여부 - 0(NO_USE), 1(USE)
#if (EFFLUENT_CLOGGING_ERROR_FLOWMETER_USE == USE)
#define EFFLUENT_CLOGGING_CHECK_DELAY_TIME          300         // 추출 막힘 에러 감지 시작 전 지연 시간 30sec@100ms
#define EFFLUENT_CLOGGING_HZ                        5           // 추출 막힘 에러 감지 기준 Hz
#define EFFLUENT_CLOGGING_DETECT_TIME               30          // 추출 막힘 에러 감지 시간 3sec@100ms
#define EFFLUENT_CLOGGING_AUTO_CLEAR_TIME           600         // 추출 막힘 에러 자동 해제 시간 60sec@100ms
#else
#define EFFLEUNT_CLOGGING_DETECT_TEMP               55.0f       // 추출 막힘 에러 감지 기준 온도 55.0도
#define EFFLEUNT_CLOGGING_START_STANDARD_TEMP       70.0f       // 에러 감지를 위한 추출 시작시 기준 온도 70.0도
#define EFFLUENT_CLOGGING_DETECT_TIME_OVER_STANDARD     20          // 추출 시작시 기준 온도 이상에서 추출 시작시 에러 감지 기준 시간 2sec@100ms
#define EFFLUENT_CLOGGING_DETECT_TIME_UNDER_STANDARD    10          // 추출 시작시 기준 온도 미만에서 추출 시작시 에러 감지 기준 시간 1sec@100ms
#define EFFLUENT_CLOGGING_AUTO_CLEAR_COLD_TEMP      10.0f       // 추출 막힘 에러 자동 해제 기준 냉수 온도 10.0도
#define EFFLUENT_CLOGGING_AUTO_CLEAR_TEMP           15.0f       // 추출 막힘 에러 자동 해제 기준 추출부 온도 15.0도
#define EFFLUENT_CLOGGING_AUTO_CLEAR_TIME           30          // 추출 막힘 에러 자동 해제 시간 3sec@100ms
#endif

// 온수 단수 및 배수 막힘 에러 감지
#define HOT_CUT_OFF_ERROR_CHECK_USE                 USE         // 온수 단수 및 배수 막힘 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define HOT_CUT_OFF_ERROR_CHECK_DELAY_TIME          30          // 온수 단수 및 배수 막힘 에러 감지 시작 전 지연 시간 3sec@100ms
#define HOT_CUT_OFF_DETECT_TIME                     30          // 온수 단수 및 배수 막힘 감지 시간 3sec@100ms
#define HOT_CUT_OFF_HZ                              5           // 온수 단수 및 배수 막힘 감지 기준 Hz
#define HOT_CUT_OFF_AUTO_CLEAR_TIME                 300         // 온수 단수 및 배수 막힘 에러 자동 해제 시간 30sec@100ms

// 부스트펌프 에러 감지
#define BOOST_PUMP_ERROR_CHECK_USE                  USE         // 부스트펌프 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define BOOST_PUMP_ERROR_DETECT_TIME                30          // 부스트펌프 에러 감지 시간 3sec@100ms
#define BOOST_PUMP_ERROR_OVER_STANDARD_AD           120         // 부스트펌프 에러 감지 기준 AD 최대값
#define BOOST_PUMP_ERROR_UNDER_STANDARD_AD          30          // 부스트펌프 에러 감지 기준 AD 최소값

// 압력 센서 에러 감지
#define PRESSURE_SENSOR_ERROR_CHECK_USE             NO_USE      // 압력 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)

// 추출 트레이 모터 에러 감지 - 미사용, 표준 동작 없음
#define TRAY_MOTOR_ERROR_CHECK_USE                  NO_USE      // 추출 트레이 모터 에러 감지 사용 여부 - 0(NO_USE), 1(USE)

// 마이크로 스위치 동시 감지 에러 감지
#define MICRO_SW_ALL_DETECT_ERROR_CHECK_USE         USE         // 마이크로 스위치 동시 감지 에러 감지 사용 여부 - 0(NO_USE), 1(USE)

// 마이크로 스위치 이동 에러(E62, E63) 감지
#define MICRO_SW_ICE_MAKING_ERROR_CHECK_USE         USE         // 마이크로 스위치 이동(제빙) 에러 감지 사용 여부 - 0(NO_USE), 1(USE)

// 드레인 펌프 에러 감지
#define DRAIN_PUMP_ERROR_CHECK_USE                  USE         // 드레인 펌프 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define DRAIN_PUMP_ERROR_DETECT_TIME                6000        // 드레인 펌프 에러 감지 상태 감지 시간 10min = 600sec@100ms
#define DRAIN_PUMP_ERROR_CONFIRM_COUNT              12          // 드레인 펌프 에러 확정 횟수 12회
#define DRAIN_PUMP_ERROR_CHECK_DELAY                50          // 드레인 펌프 에러 감지 상태 감지 시간 5sec@100ms

// 탄산 펌프 에러 감지
#define SODA_PUMP_ERROR_CHECK_USE                   NO_USE      // 탄산 펌프 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define SODA_PUMP_ERROR_DETECT_TIME                 30          // 탄산 펌프 에러 감지 시간 3sec@100ms
#define SODA_PUMP_ERROR_OVER_STANDARD_AD            120         // 탄산 펌프 에러 감지 기준 AD 최대값
#define SODA_PUMP_ERROR_UNDER_STANDARD_AD           30          // 탄산 펌프 에러 감지 기준 AD 최소값

// 추출 동작 이상 에러 감지 - 기계식 파우셋 사용 모델에서만 사용
#define ABNORMAL_EFFLUENT_ERROR_CHECK_USE           USE         // 추출 동작 이상 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define ABNORMAL_EFFLUENT_DETECT_TIME               36000       // 추출 동작 이상 에러 감지 시간 1hour = 60min = 3600sec@100ms

// 정수 탱크 수위 에러 감지
#define LEVEL_ERROR_CHECK_USE                       USE         // 정수 탱크 수위 에러 감지 사용 여부 = 0(NO_USE), 1(USE)

// 온수 만수위 센서 에러 감지
#define HOT_HIGH_SENSOR_ERROR_CHECK_USE             USE         // 온수 만수위 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define HOT_HIGH_SENSOR_OVERFLOW_SENSOR_USE         USE         // 온수 만수위 센서 에러에서 OVERFLOW 사용 포함 여부 - 0(NO_USE), 1(USE)
#define HOT_HIGH_SENSOR_WATER_INPUT_DETECT_TIME     6000        // 온수 만수위 센서 에러 감지를 위한 탱크 입수 기준 시간 10min = 600sec@100ms

// 빙축 탱크 수위 센서 에러 감지
#define ICE_COLD_WATER_TANK_SENSOR_ERROR_CHECK_USE  USE         // 빙축 탱크 수위 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)

// 냉수 수위 센서 에러 감지
#define COLD_LEVEL_ERROR_CHECK_USE                  USE         // 냉수 수위 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define COLD_LEVEL_ERROR_LOW_SENSOR_USE             USE         // 냉수 수위 센서 에러에서 냉수 저수위 사용 포함 여부 - 0(NO_USE), 1(USE)
#define COLD_LEVEL_ERROR_HIGH_SENSOR_USE            NO_USE      // 냉수 수위 센서 에러에서 냉수 만수위 사용 포함 여부 - 0(NO_USE), 1(USE)
#if (COLD_LEVEL_ERROR_HIGH_SENSOR_USE == NO_USE)
#define COLD_LOW_SENSOR_ERROR_DETECT_TIME           9000        // 냉수 만수위 미사용시 냉수 저수위 센서 에러 감지 기준 시간 15min = 900sec@100ms
#endif

// 정수 온도 센서 에러 감지
#define ROOM_TEMP_1_ERROR_CHECK_USE                 USE         // 정수 온도 센서1 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define ROOM_TEMP_1_ERROR_OVER_STANDARD_AD          1000        // 정수 온도 센서1 에러 감지 기준 최대 AD값
#define ROOM_TEMP_1_ERROR_UNDER_STANDARD_AD         20          // 정수 온도 센서1 에러 감지 기준 최소 AD값
#define ROOM_TEMP_1_ERROR_DETECT_TIME               30          // 정수 온도 센서1 에러 감지 기준 시간 3sec@100ms
#define ROOM_TEMP_2_ERROR_CHECK_USE                 NO_USE      // 정수 온도 센서2 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define ROOM_TEMP_2_ERROR_OVER_STANDARD_AD          1000        // 정수 온도 센서2 에러 감지 기준 최대 AD값
#define ROOM_TEMP_2_ERROR_UNDER_STANDARD_AD         20          // 정수 온도 센서2 에러 감지 기준 최소 AD값
#define ROOM_TEMP_2_ERROR_DETECT_TIME               30          // 정수 온도 센서1 에러 감지 기준 시간 3sec@100ms

// 온수 탱크 온도 센서 에러 감지
#define TANK_HOT_TEMP_ERROR_CHECK_USE               USE         // 온수 탱크 온도 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define TANK_HOT_TEMP_ERROR_OVER_STANDARD_AD        1000        // 온수 탱크 온도 온도 센서 에러 감지 기준 최대 AD값
#define TANK_HOT_TEMP_ERROR_UNDER_STANDARD_AD       20          // 온수 탱크 온도 온도 센서 에러 감지 기준 최소 AD값
#define TANK_HOT_TEMP_ERROR_DETECT_TIME             30          // 온수 탱크 온도 온도 센서 에러 감지 기준 시간 3sec@100ms

// 순간 온수 입수 온도 센서 에러 감지
#define HOT_IN_TEMP_ERROR_CHECK_USE                 USE         // 순간 온수 입수 온도 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define HOT_IN_TEMP_ERROR_OVER_STANDARD_AD          1000        // 순간 온수 입수 온도 센서 에러 감지 기준 최대 AD값
#define HOT_IN_TEMP_ERROR_UNDER_STANDARD_AD         20          // 순간 온수 입수 온도 센서 에러 감지 기준 최소 AD값
#define HOT_IN_TEMP_ERROR_DETECT_TIME               30          // 순간 온수 입수 온도 센서 에러 감지 기준 시간 3sec@100ms

// 순간 온수 출수 온도 센서 에러 감지
#define HOT_OUT_TEMP_ERROR_CHECK_USE                USE         // 순간 온수 출수 온도 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define HOT_OUT_TEMP_ERROR_OVER_STANDARD_AD         1000        // 순간 온수 출수 온도 센서 에러 감지 기준 최대 AD값
#define HOT_OUT_TEMP_ERROR_UNDER_STANDARD_AD        20          // 순간 온수 출수 온도 센서 에러 감지 기준 최소 AD값
#define HOT_OUT_TEMP_ERROR_DETECT_TIME              30          // 순간 온수 출수 온도 센서 에러 감지 기준 시간 3sec@100ms

// 순간 온수 히터 내부 온도 센서 에러 감지
#define HOT_BODY_TEMP_ERROR_CHECK_USE               USE         // 순간 온수 히터 내부 온도 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define HOT_BODY_TEMP_ERROR_OVER_STANDARD_AD        1000        // 순간 온수 히터 내부 온도 센서 에러 감지 기준 최대 AD값
#define HOT_BODY_TEMP_ERROR_UNDER_STANDARD_AD       20          // 순간 온수 히터 내부 온도 센서 에러 감지 기준 최소 AD값
#define HOT_BODY_TEMP_ERROR_DETECT_TIME             30          // 순간 온수 히터 내부 온도 센서 에러 감지 기준 시간 3sec@100ms

// 냉수 온도 센서 에러 감지
#define COLD_TEMP_1_ERROR_CHECK_USE                 USE         // 냉수 온도 센서1 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define COLD_TEMP_1_ERROR_OVER_STANDARD_AD          1000        // 냉수 온도 센서1 에러 감지 기준 최대 AD값
#define COLD_TEMP_1_ERROR_UNDER_STANDARD_AD         20          // 냉수 온도 센서1 에러 감지 기준 최소 AD값
#define COLD_TEMP_1_ERROR_DETECT_TIME               30          // 냉수 온도 센서1 에러 감지 기준 시간 3sec@100ms
#define COLD_TEMP_2_ERROR_CHECK_USE                 NO_USE      // 냉수 온도 센서2 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define COLD_TEMP_2_ERROR_OVER_STANDARD_AD          1000        // 냉수 온도 센서2 에러 감지 기준 최대 AD값
#define COLD_TEMP_2_ERROR_UNDER_STANDARD_AD         20          // 냉수 온도 센서2 에러 감지 기준 최소 AD값
#define COLD_TEMP_2_ERROR_DETECT_TIME               30          // 냉수 온도 센서2 에러 감지 기준 시간 3sec@100ms

// 아이스 트레이 입수 온도 센서 에러 감지
#define ICE_TRAY_INPUT_TEMP_ERROR_CHECK_USE         NO_USE      // 아이스 트레이 입수 온도 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define ICE_TRAY_INPUT_TEMP_ERROR_OVER_STANDARD_AD  1000        // 아이스 트레이 입수 온도 센서2 에러 감지 기준 최대 AD값
#define ICE_TRAY_INPUT_TEMP_ERROR_UNDER_STANDARD_AD  20         // 아이스 트레이 입수 온도 센서2 에러 감지 기준 최소 AD값
#define ICE_TRAY_INPUT_TEMP_ERROR_DETECT_TIME       30          // 아이스 트레이 입수 온도 센서2 에러 감지 기준 시간 3sec@100ms

// 외기 온도 센서 에러 감지
#define AIR_TEMP_1_ERROR_CHECK_USE                  USE         // 외기 온도 센서1 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define AIR_TEMP_1_ERROR_OVER_STANDARD_AD           1000        // 외기 온도 센서1 에러 감지 기준 최대 AD값
#define AIR_TEMP_1_ERROR_UNDER_STANDARD_AD          20          // 외기 온도 센서1 에러 감지 기준 최소 AD값
#define AIR_TEMP_1_ERROR_DETECT_TIME                30          // 외기 온도 센서1 에러 감지 기준 시간 3sec@100ms
#define AIR_TEMP_2_ERROR_CHECK_USE                  NO_USE      // 외기 온도 센서2 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define AIR_TEMP_2_ERROR_OVER_STANDARD_AD           1000        // 외기 온도 센서2 에러 감지 기준 최대 AD값
#define AIR_TEMP_2_ERROR_UNDER_STANDARD_AD          20          // 외기 온도 센서2 에러 감지 기준 최소 AD값
#define AIR_TEMP_2_ERROR_DETECT_TIME                30          // 외기 온도 센서2 에러 감지 기준 시간 3sec@100ms

// TEM 방열판 온도 센서 에러 감지
#define TEM_HEATSINK_TEMP_ERROR_CHECK_USE           USE         // TEM 방열판 온도 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define TEM_HEATSINK_TEMP_ERROR_OVER_STANDARD_TEMP  75.0f       // TEM 방열판 온도 센서 에러 감지 기준 최대 온도값
#define TEM_HEATSINK_TEMP_ERROR_DETECT_TIME         10          // TEM 방열판 온도 센서 에러 감지 기준 시간 1sec@100ms

// 온수 과열 에러 감지
#define HOT_OVERHEAT_ERROR_CHECK_USE                USE         // 온수 과열 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define HOT_OVERHEAT_ERROR_TANK_USE                 NO_USE         // 온수 과열 에러 감지시 온수 탱크형인지 여부 - 0(NO_USE, 직수형), 1(USE, 탱크형)
#if (HOT_OVERHEAT_ERROR_TANK_USE == USE)
#define HOT_OVERHEAT_ERROR_OVER_STANDARD_TEMP       98.0f       // TEM 방열판 온도 센서 에러 감지 기준 최대 온도값
#define HOT_OVERHEAT_ERROR_DETECT_TIME              1800        // 온수 과열 에러 감지 기준 시간 3min = 180sec@100ms
#else
#define HOT_OVERHEAT_ERROR_OVER_STANDARD_TEMP       102.0f       // TEM 방열판 온도 센서 에러 감지 기준 최대 온도값
#define HOT_OVERHEAT_ERROR_DETECT_TIME              5           // 온수 과열 에러 감지 기준 시간 500ms@100ms
#define HOT_OVERHEAT_ERROR_AUTO_CLEAR_TEMP          85.0f       // 온수 과열 에러 자동 해제 기준 온도값
#endif

// 온수 펌프 에러 감지
#define HOT_PUMP_ERROR_CHECK_USE                    NO_USE      // 온수 펌프 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define HOT_PUMP_ERROR_DETECT_TIME                  30          // 온수 펌프 에러 감지 시간 3sec@100ms
#define HOT_PUMP_ERROR_OVER_STANDARD_AD             120         // 온수 펌프 에러 감지 기준 AD 최대값
#define HOT_PUMP_ERROR_UNDER_STANDARD_AD            30          // 온수 펌프 에러 감지 기준 AD 최소값

// BLDC 컴프 에러 감지
#define BLDC_COMP_ERROR_CHECK_USE                   NO_USE      // BLDC 컴프 에러 감지 사용 여부 - 0(NO_USE), 1(USE)

// 일반 컴프 동작 불량 에러 감지 - 미사용, 표준 동작 없음
#define COMP_ACT_ERROR_CHECK_USE                    NO_USE      // 일반 컴프 동작 불량 에러 감지 사용 여부 - 0(NO_USE), 1(USE)

// 순환 펌프 에러 감지
#define CIRCULATE_PUMP_ERROR_CHECK_USE              NO_USE      // 순환 펌프 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define CIRCULATE_PUMP_ERROR_OVER_STANDARD_AD       1000        // 순환 펌프 에러 감지 기준 최대 AD값
#define CIRCULATE_PUMP_ERROR_UNDER_STANDARD_AD      20          // 순환 펌프 에러 감지 기준 최소 AD값
#define CIRCULATE_PUMP_ERROR_DETECT_TIME            100         // 순환 펌프 에러 감지 시간 10sec@100ms
#define CIRCULATE_PUMP_ERROR_CONFIRM_COUNT          3           // 순환 펌프 에러 확정 반복 횟수

// 교반 모터 에러 감지
#define STIRRING_MOTOR_ERROR_CHECK_USE              NO_USE      // 교반 모터 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define STIRRING_MOTOR_ERROR_CHECK_DELAY            10          // 교반 모터 에러 감지 시작 전 지연 시간 1sec@100ms
#define STIRRING_MOTOR_ERROR_OVER_STANDARD_AD       1000        // 교반 모터 에러 감지 기준 최대 AD값
#define STIRRING_MOTOR_ERROR_UNDER_STANDARD_AD      20          // 교반 모터 에러 감지 기준 최소 AD값
#define STIRRING_MOTOR_ERROR_DETECT_TIME            30          // 교반 모터 에러 감지 시간 3sec@100ms
#define STIRRING_MOTOR_ERROR_RE_CHECK_DELAY         18000       // 교반 모터 에러 재검사 전 지연 시간 30min = 1800sec@100ms
#define STIRRING_MOTOR_ERROR_CONFIRM_TIME_HOUR      2880        // 교반 모터 에러 확정 시간 48hour = 2880min

// EW 살균 모듈 에러 감지
#define EW_MODULE_ERROR_CHECK_USE                   NO_USE      // EW 살균 모듈 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define EW_MODULE_ERROR_OVER_STANDARD_AD            1000        // EW 살균 모듈 에러 감지 기준 최대 AD값
#define EW_MODULE_ERROR_UNDER_STANDARD_AD           20          // EW 살균 모듈 에러 감지 기준 최소 AD값
#define EW_MODULE_ERROR_DETECT_TIME                 70          // EW 살균 모듈 에러 감지 시간 7sec@100ms
#define EW_MODULE_ERROR_DETECT_COUNT                3           // EW 살균 모듈 에러 감지 카운트

// EW 살균 드레인 펌프 에러 감지
#define EW_DRAIN_PUMP_ERROR_CHECK_USE               NO_USE      // EW 살균 드레인 펌프 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define EW_DRAIN_PUMP_ERROR_OVER_STANDARD_AD        1000        // EW 살균 드레인 펌프 에러 감지 기준 최대 AD값
#define EW_DRAIN_PUMP_ERROR_UNDER_STANDARD_AD       20          // EW 살균 드레인 펌프 에러 감지 기준 최소 AD값
#define EW_DRAIN_PUMP_ERROR_DETECT_TIME             15000       // EW 살균 드레인 펌프 에러 감지 시간 25min = 1500sec@100ms

// UV 에러 감지
#define UV_ERROR_CHECK_USE                          NO_USE      // UV 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define UV_ERROR_OVER_STANDARD_AD                   1000        // UV 에러 감지 기준 최대 AD값
#define UV_ERROR_UNDER_STANDARD_AD                  20          // UV 에러 감지 기준 최소 AD값
#define UV_ERROR_DETECT_TIME                        100         // UV 에러 감지 시간 10sec@100ms

// TDS IN 온도 센서 에러 감지
#define TDS_IN_TEMP_ERROR_CHECK_USE                 NO_USE      // TDS IN 온도 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define TDS_IN_TEMP_ERROR_OVER_STANDARD_AD          1000        // TDS IN 온도 센서 에러 감지 기준 최대 AD값
#define TDS_IN_TEMP_ERROR_UNDER_STANDARD_AD         20          // TDS IN 온도 센서 에러 감지 기준 최소 AD값
#define TDS_IN_TEMP_ERROR_DETECT_TIME               30          // TDS IN 온도 센서 에러 감지 시간 3sec@100ms

// TDS OUT 온도 센서 에러 감지
#define TDS_OUT_TEMP_ERROR_CHECK_USE                NO_USE      // TDS OUT 온도 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define TDS_OUT_TEMP_ERROR_OVER_STANDARD_AD         1000        // TDS OUT 온도 센서 에러 감지 기준 최대 AD값
#define TDS_OUT_TEMP_ERROR_UNDER_STANDARD_AD        20          // TDS OUT 온도 센서 에러 감지 기준 최소 AD값
#define TDS_OUT_TEMP_ERROR_DETECT_TIME              30          // TDS OUT 온도 센서 에러 감지 시간 3sec@100ms

// TDS IN 센서 에러 감지
#define TDS_IN_ERROR_CHECK_USE                      NO_USE      // TDS IN 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define TDS_IN_ERROR_DETECT_TIME                    3           // TDS IN 센서 에러 감지 시간 300ms@100ms
#define TDS_IN_ERROR_DETECT_COUNT                   5           // TDS IN 센서 에러 감지 횟수

// TDS OUT 센서 에러 감지
#define TDS_OUT_ERROR_CHECK_USE                     NO_USE      // TDS OUT 센서 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define TDS_OUT_ERROR_DETECT_TIME                   3           // TDS OUT 센서 에러 감지 시간 300ms@100ms
#define TDS_OUT_ERROR_DETECT_COUNT                  5           // TDS OUT 센서 에러 감지 횟수

// 원수 오염 에러 감지
#define INPUT_CONTAMINATION_ERROR_CHECK_USE         NO_USE      // 원수 오염 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define INPUT_CONTAMINATION_ERROR_DETECT_DELAY_TIME 10          // 원수 오염 에러 감지 체크 전 지연 시간 1sec@100ms
#define INPUT_CONTAMINATION_ERROR_DETECT_COUNT      3           // 원수 오염 에러 감지 횟수

// 제거율 불량 에러 감지
#define DECONTAMINATION_ERROR_CHECK_USE             NO_USE         // 제거율 불량 에러 감지 사용 여부 - 0(NO_USE), 1(USE)
#define DECONTAMINATION_ERROR_CHECK_STANDARD_OUT_PPM    15      // 제거율 불량 체크를 하는 기준 TDS OUT값(ppm)
#define DECONTAMINATION_ERROR_DETECT_DELAY_TIME     10          // 제거율 불량 에러 감지 체크 전 지연 시간 1sec@100ms
#define DECONTAMINATION_ERROR_DETECT_COUNT          3           // 제거율 불량 에러 감지 횟수


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

