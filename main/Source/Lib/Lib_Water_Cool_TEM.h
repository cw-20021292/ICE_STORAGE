/// @file     Lib_Water_Cool_TEM.h
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    열전모듈 냉각 동작 제어 file

#ifndef _LIB_WATER_COOL_TEM_H_
#define _LIB_WATER_COOL_TEM_H_



/*  Lib Function 사용법
    => gau16TEM_TDR_DataTable[TEM_V_ID_MAX] 테이블의 TEM PWM 제어값을 설정해주면 SW 제어 모듈이 알아서 동작함
    => Set_ColdFuncStatus(mu8Status);                       // 냉수 기능 설정 상태 설정 : 0(OFF), 1(ON)
    => Get_ColdFuncStatus();                                // 냉수 기능 설정 상태 확인 : 0(OFF), 1(ON)
    => Set_ColdStorng(mu8Status);                           // 냉수 기능 강약 설정 상태 설정 : 0(약), 1(강)
    => Get_ColdStrong();                                    // 냉수 기능 강약 설정 상태 확인 : 0(약), 1(강)
*/


#if (TEM_USE == USE)                // 열전모듈 냉각 사용시


/// @brief      TEM 냉각 제어 모드 리스트
/// @details    TEM 냉각 제어 모드를 구조 리스트로 정리한다
typedef enum
{
    TEM_MODE_ID_OFF = 0,        // 0 : TEM PWM OFF
    TEM_MODE_ID_RAPID,          // 1 : 급속 모드
    TEM_MODE_ID_RETAIN,         // 2 : 유지 모드
    TEM_MODE_ID_MIN,            // 3 : 최소 모드
    TEM_MODE_ID_ECO,            // 4 : ECO 모드
    TEM_MODE_ID_ECO_9V,         // 5 : ECO 9V 모드
    TEM_MODE_ID_MAX             // Max
} ETemID_T;


// TEM 제어 Volatae ID
typedef enum
{
    TEM_V_ID_0V = 0,            // 0 :
    TEM_V_ID_1V,                // 1 :
    TEM_V_ID_2V,                // 2 :
    TEM_V_ID_3V,                // 3 :
    TEM_V_ID_4V,                // 4 :
    TEM_V_ID_5V,                // 5 :
    TEM_V_ID_6V,                // 6 :
    TEM_V_ID_7V,                // 7 :
    TEM_V_ID_8V,                // 8 :
    TEM_V_ID_9V,                // 9 :
    TEM_V_ID_10V,               // 10 :
    TEM_V_ID_11V,               // 11 :
    TEM_V_ID_12V,               // 12 :
    TEM_V_ID_13V,               // 13 :
    TEM_V_ID_14V,               // 14 :
    TEM_V_ID_15V,               // 15 :
    TEM_V_ID_16V,               // 16 :
    TEM_V_ID_17V,               // 17 :
    TEM_V_ID_18V,               // 18 :
    TEM_V_ID_19V,               // 19 :
    TEM_V_ID_20V,               // 20 :
    TEM_V_ID_21V,               // 21 :
    TEM_V_ID_22V,               // 22 :
    TEM_V_ID_23V,               // 23 :
    TEM_V_ID_24V,               // 24 :
    TEM_V_ID_MAX                // Max
} ETEM_V_ID_T;

#define LIB_TEM_CONTROL_TIME_PERIOD                         100     // TEM 제어 주기 100ms@1ms

#define TEM_RAPID_OFF_DELAY_TIME                            600     // 급속모드에서 TEN OFF시 재기동 지연 delay 시간 10min = 600sec@1sec
#define TEM_MODE_CHANGE_CONFIRM_TIME                        3       // 모드 전환시 모드 변환 확정 체크 시간 300ms@100ms
#define TEM_RAPID_MODE_MAX_TIME                             21600   // 급속 모드 동작 최대 시간 6hour = 360min = 21600sec@1sec

/***************냉수 강모드*******************/
#define RAPID_OPERATE_DEGREE                                7.2f    // 급속모드 진입 온도(2024.11.26 예병효님 요청으로 7.5도 -> 7.2도 변경)
#define RETAIN_JOIN_DEGREE                                  3.5f    // 급속모드 중 유지모드 진입 온도
#define RAPID_COMPLETE_DEGREE                               6.5f    // 급속모드 완료 온도
#define COLD_COMPLETE_DEGREE                                2.5f    // 냉각 완료 온도
#define	COLD_REOPERATE_DEGREE		                        2.5f    // 냉각 종료 중 냉각 재가동 온도

#define TEM_COOLING_RAPID_END_CONDITION_1_TEMP              33.0f    // 급속 냉각 추가 기동 시간 설정을 위한 단계 조건1 기준 외기 온도
#define TEM_COOLING_RAPID_END_CONDITION_2_TEMP              30.0f    // 급속 냉각 추가 기동 시간 설정을 위한 단계 조건2 기준 외기 온도
#define TEM_COOLING_RAPID_END_CONDITION_3_TEMP              20.0f    // 급속 냉각 추가 기동 시간 설정을 위한 단계 조건3 기준 외기 온도
#define TEM_COOLING_RAPID_END_CONDITION_4_TEMP              12.0f    // 급속 냉각 추가 기동 시간 설정을 위한 단계 조건4 기준 외기 온도(이 온도 미만이 조건5 동작)

#define TEM_COOLING_RAPID_END_CONDITION_1_TIME              4500    // 외기 33도 이상시 급속 냉각 추가 기동 시간 4500sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_2_TIME              3000    // 외기 30도 이상시 급속 냉각 추가 기동 시간 3000sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_3_TIME              780     // 외기 20도 이상시 급속 냉각 추가 기동 시간 780sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_4_TIME              180     // 외기 12도 이상시 급속 냉각 추가 기동 시간 180sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_5_TIME              200     // 외기 12도 미만시 급속 냉각 추가 기동 시간 200sec@1sec

#define TEM_COOLING_RAPID_END_CONDITION_1_V                 TEM_V_ID_24V    // 외기 33도 이상시 급속 냉각 제어 전압 24V
#define TEM_COOLING_RAPID_END_CONDITION_2_V                 TEM_V_ID_24V    // 외기 30도 이상시 급속 냉각 제어 전압 24V
#define TEM_COOLING_RAPID_END_CONDITION_3_V                 TEM_V_ID_24V    // 외기 20도 이상시 급속 냉각 제어 전압 24V
#define TEM_COOLING_RAPID_END_CONDITION_4_V                 TEM_V_ID_24V    // 외기 12도 이상시 급속 냉각 제어 전압 24V
#define TEM_COOLING_RAPID_END_CONDITION_5_V                 TEM_V_ID_12V    // 외기 12도 미만시 급속 냉각 제어 전압 12V

/***************냉수 약모드*******************/
#define RAPID_OPERATE_DEGREE_LOW_MOE                        13.0f   // 급속모드 진입 온도
#define RETAIN_JOIN_DEGREE_LOW_MODE                         9.0f    // 급속모드 중 유지모드 진입 온도
#define RAPID_COMPLETE_DEGREE_LOW_MODE                      12.0f   // 급속모드 완료 온도
#define COLD_COMPLETE_DEGREE_LOW_MODE                       4.0f    // 냉각 완료 온도
#define	COLD_REOPERATE_DEGREE_LOW_MODE                      8.0f    // 냉각 종료 중 냉각 재가동 온도

#define TEM_COOLING_RAPID_END_CONDITION_1_TEMP_LOW_MODE     33.0f    // 급속 냉각 추가 기동 시간 설정을 위한 단계 조건1 기준 외기 온도
#define TEM_COOLING_RAPID_END_CONDITION_2_TEMP_LOW_MODE     30.0f    // 급속 냉각 추가 기동 시간 설정을 위한 단계 조건2 기준 외기 온도
#define TEM_COOLING_RAPID_END_CONDITION_3_TEMP_LOW_MODE     20.0f    // 급속 냉각 추가 기동 시간 설정을 위한 단계 조건3 기준 외기 온도
#define TEM_COOLING_RAPID_END_CONDITION_4_TEMP_LOW_MODE     12.0f    // 급속 냉각 추가 기동 시간 설정을 위한 단계 조건4 기준 외기 온도(이 온도 미만이 조건5 동작)

#define TEM_COOLING_RAPID_END_CONDITION_1_TIME_LOW_MODE     1800    // 외기 33도 이상시 급속 냉각 추가 기동 시간 1800sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_2_TIME_LOW_MODE     1200    // 외기 30도 이상시 급속 냉각 추가 기동 시간 1200sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_3_TIME_LOW_MODE     780     // 외기 20도 이상시 급속 냉각 추가 기동 시간 780sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_4_TIME_LOW_MODE     400     // 외기 12도 이상시 급속 냉각 추가 기동 시간 400sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_5_TIME_LOW_MODE     100     // 외기 12도 미만시 급속 냉각 추가 기동 시간 100sec@1sec

#define TEM_COOLING_RAPID_END_CONDITION_1_V_LOW_MODE        TEM_V_ID_24V    // 외기 33도 이상시 급속 냉각 제어 전압 24V
#define TEM_COOLING_RAPID_END_CONDITION_2_V_LOW_MODE        TEM_V_ID_24V    // 외기 30도 이상시 급속 냉각 제어 전압 24V
#define TEM_COOLING_RAPID_END_CONDITION_3_V_LOW_MODE        TEM_V_ID_24V    // 외기 20도 이상시 급속 냉각 제어 전압 24V
#define TEM_COOLING_RAPID_END_CONDITION_4_V_LOW_MODE        TEM_V_ID_24V    // 외기 12도 이상시 급속 냉각 제어 전압 24V
#define TEM_COOLING_RAPID_END_CONDITION_5_V_LOW_MODE        TEM_V_ID_12V    // 외기 12도 미만시 급속 냉각 제어 전압 12V

#define TEM_RAPID_MODE_IN_12V_TIME                          3000    // 급속 모드로 진입했을 때 초기 12V 전압을 유지하는 시간 5min = 300sec@100ms

void Lib_TEM_Initialize(void);
void Lib_TEM_ControlTimer(void);
void Set_ColdFuncStatus(U8 mu8Status);
U8 Get_ColdFuncStatus(void);
void Set_ColdStorng(U8 mu8Status);
U8 Get_ColdStrong(void);
U8 Get_TEM_ActMode(void);
void Lib_TEM_Control(void);

#endif      // 열전모듈 냉각 사용시 END

void Lib_TEM_Module_Initialize(void);
void Lib_TEM_Module_1ms_Control(void);
void Lib_TEM_Module_Control(void);

#endif
