/// @file     Lib_Water_InstantaneousHeater.h
/// @date     2025/07/03
/// @author   Jaejin Ham
/// @brief    순간 온수 Heater 동작 제어 file

#ifndef _LIB_INSTANTANEOUS_HEATER_H_
#define _LIB_INSTANTANEOUS_HEATER_H_


/*  Lib Function 사용법
    => ga16Hot_InputUnder15[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX], ga16Hot_InputUnder25[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX],
       ga16Hot_InputUp25[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX] 테이블의 설정값에 따라서 SW 제어 모듈에서 알아서 동작함
       제어 온도 단계 추가시 배열 증대 필요

    => Set_InstHeater_HotSelect(mu8Select);                 // 순간 온수 온도 선택 상태 설정
    => Get_InstHeater_HotSelect();                          // 순간 온수 온도 선택 상태 확인

    => Get_InstHeater_V_Detect();                           // 순간 온수 히터 감지 전압 상태 확인
*/


#if (INSTANT_HEATER_USE == USE)


/// @brief      온수 선택 온도 테이블
/// @details    온수 중 선택할 수 있는 온도의 종류를 열거한 리스트를 정의한다
typedef enum
{
    HOT_SELECT_ID_45 = 0,                   // 0 : 45도
    HOT_SELECT_ID_70,                       // 1 : 70도
    HOT_SELECT_ID_85,                       // 2 : 85도
    HOT_SELECT_ID_100,                      // 3 : 100도
    HOT_SELECT_ID_MAX                       // Max
} EHotSelectTable_T;


/// @brief      입력 AC 전압 감지 상태 테이블
/// @details    입력 AC 전압 감지 상태값을 구조 리스트를 정의한다
typedef enum
{
    V_DETECT_ID_190V = 0,                   // 0 : 190V
    V_DETECT_ID_200V,                       // 1 : 200V
    V_DETECT_ID_210V,                       // 2 : 210V
    V_DETECT_ID_220V,                       // 3 : 220V
    V_DETECT_ID_230V,                       // 4 : 230V
    V_DETECT_ID_240V,                       // 5 : 240V
    V_DETECT_ID_MAX                         // Max
} EVoltageDetectTable_T;


/// @brief      순간 온수 히터 제어 단계
/// @details    순간 온수 히터 제어 단계를 구조 리스트로 정의한다
typedef enum
{
    INST_HEATING_STEP_ID_NONE = 0,          // 0 : 동작 없음
    INST_HEATING_STEP_ID_PREHEAT,           // 1 : 예열
    INST_HEATING_STEP_ID_EFFLUENT,          // 2 : 추출중 가열
    INST_HEATING_STEP_ID_MAX                // Max
} EHeatingStepTable_T;

#define INST_HEAT_CONTROL_TIME_PERIOD                   100         // 순간온수 히터 제어 주기 타이머 100ms@1ms

#define INST_HEATER_INPUT_V_DETECT_STD_190V             840         // 순간온수 히터 입력 전압 190V 판정 기준 AD값
#define INST_HEATER_INPUT_V_DETECT_STD_200V             870         // 순간온수 히터 입력 전압 200V 판정 기준 AD값
#define INST_HEATER_INPUT_V_DETECT_STD_210V             920         // 순간온수 히터 입력 전압 210V 판정 기준 AD값
#define INST_HEATER_INPUT_V_DETECT_STD_220V             960         // 순간온수 히터 입력 전압 220V 판정 기준 AD값
#define INST_HEATER_INPUT_V_DETECT_STD_230V             1000        // 순간온수 히터 입력 전압 230V 판정 기준 AD값(240V는 230V 기준값 초과로 판단)


void Lib_InstHeater_Initialize(void);
void InstHeater_ControlTimer(void);
void FlowControl(U8 mu8OpenClose, U16 mu16Pulse);
void Set_InstHeaterControlStep(U8 mu8Step);
U8 Get_InstHeaterControlStep(void);
void Set_InstHeater_HotSelect(U8 mu8Select);
U8 Get_InstHeater_HotSelect(void);
void InstHeaterControl_Preheat(void);
void InstHeaterControl_Effluent(void);
void InstHeaterOff(void);
void InstHeater_V_Detect(void);
void Set_InstHeater_V_Detect(U8 mu8Data);
U8 Get_InstHeater_V_Detect(void);
void Set_InstHeater_ControlStep(U8 mu8Data);
U8 Get_InstHeater_ControlStep(void);
void Lib_InstHeater_Control(void);

#endif

void Lib_InstHeater_Module_Initialize(void);
void Lib_InstHeater_Module_1ms_Control(void);
void Lib_InstHeater_Module_Control(void);

#endif

