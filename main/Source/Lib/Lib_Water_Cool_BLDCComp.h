/// @file     Lib_Water_Cool_BLDCComp.h
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    냉각 동작 제어 file

#ifndef _LIB_WATER_COOL_BLDC_COMP_H_
#define _LIB_WATER_COOL_BLDC_COMP_H_


/*  Lib Function 사용법
    => SCoolData[POWERSAVE_ID_MAX][COOL_STEP_ID_MAX] 테이블의 제어값을 설정해주면 SW 제어 모듈이 알아서 동작함
    => Set_ColdFuncStatus(mu8Status);                       // 냉수 기능 설정 상태 설정 : 0(OFF), 1(ON)
    => Get_ColdFuncStatus();                                // 냉수 기능 설정 상태 확인 : 0(OFF), 1(ON)
    => Set_ColdStorng(mu8Status);                           // 냉수 기능 강약 설정 상태 설정 : 0(약), 1(강)
    =>  Get_ColdStrong();                                   // 냉수 기능 강약 설정 상태 확인 : 0(약), 1(강)
*/


#if (BLDC_COMP_USE == USE)              // BLDC Comp 사용시


/// @brief      외기 온도에 따른 냉각 제어 단계 리스트
/// @details    외기 온도 조건에 따른 냉각 제어 단계를 구조 리스트로 정리한다
typedef enum
{
    COOL_STEP_ID_AIR_TEMP_ERROR = 0,        // 0 : 외기 온도 에러 발생시
    COOL_STEP_ID_STEP_1,                    // 1 : 외기 온도에 따른 냉각 제어 1단계
    COOL_STEP_ID_STEP_2,                    // 2 : 외기 온도에 따른 냉각 제어 2단계
    COOL_STEP_ID_STEP_3,                    // 3 : 외기 온도에 따른 냉각 제어 3단계
    COOL_STEP_ID_STEP_4,                    // 4 : 외기 온도에 따른 냉각 제어 2단계
    COOL_STEP_ID_MAX                        // Max
} ECoolStepID_T;

#define BLDC_COMP_CONTROL_STEP1_ENV_TEMP            10.0f           // 냉각 제어 동작 1단계, 2단계 구분 기준 외기 온도
#define BLDC_COMP_CONTROL_STEP2_ENV_TEMP            20.0f           // 냉각 제어 동작 2단계, 3단계 구분 기준 외기 온도
#define BLDC_COMP_CONTROL_STEP3_ENV_TEMP            30.0f           // 냉각 제어 동작 3단계, 4단계 구분 기준 외기 온도

#define BLDC_COMP_FIRST_ACT_CHECK_COLD_TEMP         20.0f           // 냉각 동작 시작시 초기 기동 여부 판단 기준 냉수 온도

#define BLDC_COMP_CONTROL_TIME_PERIOD               100             // 냉각 제어 시간 주기 : 100ms(@1ms)

#define COOL_HEAT_SIMULTANEOUS_ON_DELAY_TIME        10              // 냉각기와 히터가 동시에 기동 시작할 수 있는 조건에 히터 ON 전 Delay 시간 1sec@100ms

#define DELAY_TIME_BEFORE_COMP_ON                   300             // Comp 동작 전 지연 시간 5min = 300sec@1sec
#define COMP_ON_MAX_TIME                            9000            // Comp 동작 최대 시간 150min = 9000sec@1sec

#define COMP_OFF_CHECK_MAX_TIME                     1440            // 냉각 OFF 상태로 유지된 체크 최대 시간 24Hour = 1440min@1min


void Lib_BLDC_Comp_Initialize(void);
void BLDC_Comp_ControlTimer(void);
void Set_ColdFuncStatus(U8 mu8Status);
U8 Get_ColdFuncStatus(void);
void Set_ColdStorng(U8 mu8Status);
U8 Get_ColdStrong(void);
void Manual_BLDC_CompTest_Control(void);
void Set_Manual_BLDC_CompTest(U8 mu8Status);
U8 Get_Manual_BLDC_CompTest(void);
void Set_Manual_BLDC_CompTestAct(U8 mu8OnOff);
U8 Get_Manual_BLDC_CompTestAct(void);
void BLDC_Comp_ActTimeCounter(void);
U8 Get_ColdOffMaxTimeEnd(void);
U8 Get_CompActMaxTimeEnd(void);
U8 Get_CompOnDelayTimeEnd(void);
U16 Get_CompOnDelayTime(void);
void CoolStepCheck_AirTemp(void);
U8 Get_CoolStep(void);
void Lib_BLDC_Comp_Control(void);

#endif      // BLDC Comp 사용시 END

void Lib_BLDC_Comp_Module_Initialize(void);
void Lib_BLDC_Comp_Module_1ms_Control(void);
void Lib_BLDC_Comp_Module_Control(void);

#endif

