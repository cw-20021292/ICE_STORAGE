/// @file   Lib_Water_Effluent.h
/// @date   2025/06/26
/// @author Jaejin Ham
/// @brief  추출 동작 모드 주관 헤더 파일

#ifndef _LIB_WATER_EFFLUENT_H_
#define _LIB_WATER_EFFLUENT_H_


/*  Lib Function 사용법
    => Set_WaterSelect(mu8ID);                  // 물 종류 선택 설정
    => Get_WaterSelect();                       // 선택된 물 종류 확인
    => Set_DefaultWaterSelect(mu8ID);           // 기본 선택 물 종류 설정
    => Get_DefaultWaterSelect();                // 기본 선택 물 종류 설정 상태 확인

    => Set_WaterAmount(mu8ID);                  // 물양 선택 설정
    => Get_WaterAmount();                       // 선택된 물양 확인
    => Set_DefaultWaterAmount(mu8ID);           // 기본 선택 물양 설정
    => Get_DefaultWaterAmount();                // 기본 선택 물양 설정 상태 확인

    => Set_MyCupAmount(mu16Data);               // 마이컵 용량 설정(130ml ~ 1000ml)
    => Get_MyCupAmount();                       // 마이컵 용량 설정 상태 확인

    => Set_EffluentTime(mu8ID, mu16Data);       // 추출 동작을 할 물 종류와 제어 시간을 설정
    => Set_EffluentHz(mu8ID, mu16Data);         // 추출 동작을 할 물 종류와 제어 Hz를 설정

    => Set_EffluentStatus(mu8ID, mu8Data);      // 선택한 물 종류의 추출 상태 여부(0(미추출), 1(추출)) 설정

    => Set_EffluentStep(mu8ID, mu8Step);        // 추출 동작 중 추출 제어 Step 변경
    => Get_EffluentStep(mu8ID);                 // 현재 추출 동작 중인 추출 제어 Step 확인
*/


#define EFFLUENT_CONTROL_PERIOD_TIME                        100     // 추출 동작 제어 주기 타이머 100ms@1ms

#define EFFLUENT_FLOWMETER_USE                              NO_USE     // 추출 동작에 유량 센서 사용 여부 - 0(미사용), 1(사용)


/// @brief      물 선택 상태값
/// @details    물 선택 상태값을 구조 리스트로 정리한다
typedef enum
{
    EFFLUENT_ID_ROOM = 0,                   // 0 : 정수
    EFFLUENT_ID_COLD,                       // 1 : 냉수
    EFFLUENT_ID_HOT,                        // 2 : 온수
    EFFLUENT_ID_MAX                         // Max
} EWaterSelec_T;


/// @brief      물양 선택 상태값
/// @details    물양 선택 상태값을 구조 리스트로 정리한다
typedef enum
{
    AMOUNT_ID_HALF_CUP = 0,                 // 0 :
    AMOUNT_ID_1_CUP,                        // 1 :
    AMOUNT_ID_2_CUP,                        // 2 :
    AMOUNT_ID_CONTINUE,                     // 3 :
    AMOUNT_ID_MAX                           // Max
} EAmountSelect_T;



#define EFFLUENT_PROCESS_STEP_0                             0
#define EFFLUENT_PROCESS_STEP_1                             1
#define EFFLUENT_PROCESS_STEP_2                             2
#define EFFLUENT_PROCESS_STEP_3                             3
#define EFFLUENT_PROCESS_STEP_4                             4
#define EFFLUENT_PROCESS_STEP_5                             5
#define EFFLUENT_PROCESS_STEP_6                             6
#define EFFLUENT_PROCESS_STEP_7                             7
#define EFFLUENT_PROCESS_STEP_8                             8
#define EFFLUENT_PROCESS_STEP_9                             9
#define EFFLUENT_PROCESS_STEP_10                            10

void Lib_Effluent_Initialize(void);
void Set_WaterSelect(U8 mu8ID);
U8 Get_WaterSelect(void);
void Set_DefaultWaterSelect(U8 mu8ID);
U8 Get_DefaultWaterSelect(void);
void Set_WaterAmount(U8 mu8ID);
U8 Get_WaterAmount(void);
void Set_DefaultWaterAmount(U8 mu8ID);
U8 Get_DefaultWaterAmount(void);
void Set_MyCupAmount(U16 mu16Data);
U16 Get_MyCupAmount(void);
void EffluentControlTimer(void);
void EffluentTimeCountdown(void);
U8 Get_EffluentTimeCountdownEnd(U8 mu8ID);
void Set_QuantitativeEffluent(U8 mu8ID, U8 mu8Data);
U8 Get_QuantitativeEffluent(U8 mu8ID);
void Set_EffluentTime(U8 mu8ID, U16 mu16Data);
U16 Get_EffluentTime(U8 mu8ID);
U16 Get_EffluentRemainTime(U8 mu8ID);
void Set_EffluentHz(U8 mu8ID, U16 mu16Data);
U16 Get_EffluentHz(U8 mu8ID);
U16 Get_EffluentRemainHz(U8 mu8FlowmeterID);
U8 Get_EffluentHzCountdownEnd(U8 mu8ID);
void Set_EffluentStatus(U8 mu8ID, U8 mu8Data);
U8 Get_EffluentStatus(U8 mu8ID);
void Set_ClearEffluentData(U8 mu8ID);
U8 Get_EffluentEndCheck(U8 mu8ID);
void Lib_Effluent_Control(void);
void Lib_Effluent_Stop(void);
void RoomEffluentControl(void);
void RoomEffluentStop(void);
void ColdEffluentControl(void);
void ColdEffluentStop(void);
void HotEffluentControl(void);
void HotEffluentStop(void);
void Set_EffluentStep(U8 mu8ID, U8 mu8Step);
U8 Get_EffluentStep(U8 mu8ID);

void Lib_Effluent_Module_Initialize(void);
void Lib_Effluent_Module_1ms_Control(void);
void Lib_Effluent_Module_Control(void);

#endif

