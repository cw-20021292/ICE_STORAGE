/// @file     Hal_GeneralADC.h
/// @date     2025/03/18
/// @author   Jaejin Ham
/// @brief    통합 ADC Control file


#ifndef _HAL_GENERAL_ADC_H_
#define _HAL_GENERAL_ADC_H_


/*  Hal AD 제어 Function 사용법
    => Set_CoolThermistor_PowerControl(mu8Mode);           // 냉수 온도 센서 전원 제어 모드 설정 : mu8Mode - 0(OFF), 1(일반 동작시), 2(Test 동작시), 3(Error 발생시)
    => Set_Hal_ADC_Check(mu8ADC_ID, mu8Start);             // ADC 측정을 하려는 ID의 측정 평균 계산 시작 세팅 함수 : mu8Start - 0(No Chcek), 1(Check Start)
                                                            // ※ 본 함수로 반드시 Start로 세팅을 해줘야만 해당 ID의 ADC 데이터 평균 계산 함수가 동작함.
    => Set_All_ADC_CheckStart();                           // 모든 ADC 측정 평균 계산을 시작하게 세팅하는 함수(시작으로 세팅해야 평균 계산을 시작함)
    => Average_ADC(mu8ID, mu16AD_Value);                   // ID에 해당하는 AD값 평균 계산 함수(AD Convert 함수에 사용시 추가 필요)
    => Get_ADC_Data(mu8ID);                                // ID에 해당하는 AD 평균값 확인, mu8Return : AD 평균 데이터

    => Get_Temp(mu8ID);                                    // ID에 해당하는 온도센서의 온도 섭씨값 확인, f32Return : 온도 섭씨값(소수점 포함)

    사용 예)
    Set_CoolTH_PowerControl(COOL_TH_OFF_MODE);              // 냉수 온도 센서 전원 제어 OFF
    Set_CoolTH_PowerControl(COOL_TH_NORMAL_MODE);           // 냉수 온도 센서 전원 일반 동작 제어
    Set_CoolTH_PowerControl(COOL_TH_TEST_MODE);             // 냉수 온도 센서 전원 테스트 모드 동작 제어
    Set_Hal_ADC_Check(ADC_TH_HOT, 1);                       // ADC_TH_HOT ID의 ADC 동작을 시작 : mu8Start - 0(Chcek Stop), 1(Check Start)
    u8Return = Get_ADC_Data(ADC_TH_COOL);                   // ID ADC_TH_COOL인 온도 센서 AD 평균값 확인, mu8Return : AD 평균 데이터
    f32Temp = Get_Temp(ADC_TH_COOL);                        // ID ADC_TH_COOL인 온도 센서의 섭씨값 확인, 소수점 1자리 수 포함 반환
*/

// ADC 최대 25개 사용 가능

// ADC 종류 Count 설정
#define ADC_USE                                         USE

#if (ADC_USE == USE)

#define COOL_THERMISTOR_POWER_CONTROL                   USE             // 냉수 온도 센서 전원 제어 기능 여부 설정 - 0(No Control), 1(Control)

#if (COOL_THERMISTOR_POWER_CONTROL == USE)                              // 냉수 온도 센서 전원 제어 시

#define COOL_THERMISTOR_COUNT                           1               // 냉수 온도 센서 갯수 : 1(1개), 2(2개 Max)

#endif      // if - 냉수 온도 센서 전원 제어시


// 온도 센서 전원 제어 관련
#if (COOL_THERMISTOR_POWER_CONTROL == USE)
#define TH_OFF_MODE                                     0
#define TH_NORMAL_MODE                                  1
#define TH_TEST_MODE                                    2
#define TH_ERROR_MODE                                   3

#define P_COOL1_TH_ON                                   P6_bit.no2      // 냉수 온도 센서 1 전원 제어 Port, 미사용시 0 처리
#define COOL1_TARGET_TEMP                               6.0f            // 냉각 OFF 제어 조건 온도(섭씨값)

#if (COOL_THERMISTOR_COUNT == 2)
#define P_COOL2_TH_ON                                   0               // 냉수 온도 센서 2 전원 제어 Port, 미사용시 0 처리
#define COOL2_TARGET_TEMP                               0.0f            // 냉각 OFF 제어 조건 온도(섭씨값)
#endif

// Cool Thermistor 전원 제어 동작 모드
#define COOL_TH_OFF_MODE                                0           // 냉수 온도 센서 전원 OFF 제어 모드
#define COOL_TH_NORMAL_MODE                             1           // 일반 동작
#define COOL_TH_TEST_MODE                               2           // 테스트 모드 동작
#define COOL_TH1_ERROR_CHECK_MODE                       3           // 냉수 센서 1 에러 발생 체크시
#define COOL_TH2_ERROR_CHECK_MODE                       4           // 냉수 센서 2 에러 발생 체크시
#define COOL_TH_ALL_ERROR_CHECK_MODE                    5           // 냉수 센서 1, 2 모두  에러 발생 체크시

#define COOL_TH_POWER_CONTROL_TIME_PERIOD               100         // 전원 제어 Control 함수 호출 주기(100ms@1ms)

#endif

#define ADC_DATA_MAX                                    AD_CONV_MAX

#define ADC_AVERAGE_COUNT_0                             10              // ADC 1 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_1                             10              // ADC 1 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_2                             10              // ADC 2 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_3                             10              // ADC 3 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_4                             10              // ADC 4 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_5                             10              // ADC 5 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_6                             10              // ADC 6 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_7                             10              // ADC 7 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_8                             10              // ADC 8 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_9                             10              // ADC 9 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_10                            10              // ADC 10 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_11                            10              // ADC 11 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_12                            10              // ADC 12 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_13                            10              // ADC 13 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_14                            10              // ADC 14 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_15                            10              // ADC 15 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_16                            10              // ADC 16 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_17                            10              // ADC 17 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_18                            10              // ADC 18 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_19                            10              // ADC 19 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_20                            10              // ADC 20 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_21                            10              // ADC 21 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_22                            10              // ADC 22 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_23                            10              // ADC 23 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_24                            10              // ADC 24 ADC의 평균 산출 데이터 수
#define ADC_AVERAGE_COUNT_25                            10              // ADC 25 ADC의 평균 산출 데이터 수


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

