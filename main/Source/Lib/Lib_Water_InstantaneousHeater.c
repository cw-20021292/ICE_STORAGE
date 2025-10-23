/// @file     Lib_Water_InstantaneousHeater.c
/// @date     2025/07/03
/// @author   Jaejin Ham
/// @brief    순간 온수 Heater 동작 제어 file

#include "Global_Header.h"


#if (INSTANT_HEATER_USE == USE)


/// @brief      순간온수 히팅 제어 데이터 구성
/// @details    순간온수 히팅 제어를 하기 위한 데이터들을 구조체로 선언한다
typedef struct
{
    F32 gf32TargetTemp;                     /// @brief  목표 온도(섭씨값, 소수점)
    U8 gu8RelayStatus;                      /// @brief  Relay 제어 상태 - 0(OFF), 1(ON)
    U8 gu8TriacLowStd;                      /// @brief  Triac 제어 최소 Duty(%)
    U8 gu8TriacHighStd;                     /// @brief  Triac 제어 최대 Duty(%)
    U16 gu16TagetLowFlow;                   /// @brief  제어 목표 하향 유량(Hz)
    U16 gu16TagetHighFlow;                  /// @brief  제어 목표 하향 유량(Hz)
    U8 gu8FlowConPulse;                     /// @brief  한번 유량 제어시 움직일 Stepmoter제어 Pulse
}   SInstHeatData_T;


/// @brief      입수 온도 15도 미만 조건에서 추출중 히터 제어 데이터 테이블
__far const SInstHeatData_T  ga16Hot_InputUnder15[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX] = {
//  목표온도,  Relay 제어,  Triac 제어 하한,  Triac 제어 상한,  제어 유량 하한,  제어 유량 상한,  제어 유량 Pulse
    // 온수 45도 선택
    {
        {49.0f,     OFF,    80,     100,    20,     30,     10},    // 190V
        {49.0f,     OFF,    80,     100,    20,     30,     10},    // 200V
        {49.0f,     OFF,    100,    100,    20,     30,     10},    // 210V
        {47.0f,     OFF,    80,     100,    20,     30,     10},    // 220V
        {45.0f,     OFF,    80,     100,    20,     30,     10},    // 230V
        {45.0f,     OFF,    80,     100,    20,     30,     10}     // 240V
    },

    // 온수 70도 선택
    {
        {76.0f,     ON,     80,     100,    20,     30,     20},    // 190V
        {76.0f,     ON,     80,     100,    20,     30,     20},    // 200V
        {73.0f,     ON,     60,     100,    20,     30,     20},    // 210V
        {72.0f,     ON,     40,     70,     20,     30,     20},    // 220V
        {71.0f,     ON,     30,     60,     20,     30,     30},    // 230V
        {70.0f,     ON,     30,     50,     30,     35,     30}     // 240V
    },

    // 온수 80도 선택
    {
        {91.0f,     ON,     100,    100,    20,     22,     20},    // 190V
        {91.0f,     ON,     100,    100,    20,     22,     20},    // 200V
        {91.0f,     ON,     90,     100,    20,     22,     20},    // 210V
        {89.0f,     ON,     70,     100,    20,     22,     30},    // 220V
        {88.0f,     ON,     80,     100,    22,     26,     30},    // 230V
        {88.0f,     ON,     70,     100,    22,     28,     30}     // 240V
    },

    // 온수 100도 선택
    {
        {91.0f,     ON,     100,    100,    20,     22,     20},    // 190V
        {91.0f,     ON,     100,    100,    20,     22,     20},    // 200V
        {91.0f,     ON,     90,     100,    20,     22,     20},    // 210V
        {89.0f,     ON,     70,     100,    20,     22,     30},    // 220V
        {88.0f,     ON,     80,     100,    22,     26,     30},    // 230V
        {88.0f,     ON,     70,     100,    22,     28,     30}     // 240V
    }
};


/// @brief      입수 온도 25도 미만 조건에서 추출중 히터 제어 데이터 테이블
__far const SInstHeatData_T  ga16Hot_InputUnder25[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX] = {
//  목표온도,  Relay 제어,  Triac 제어 하한,  Triac 제어 상한,  제어 유량 하한,  제어 유량 상한,  제어 유량 Pulse
    // 온수 45도 선택
    {
        {49.0f,     OFF,    70,     100,    20,     30,     10},   // 190V
        {49.0f,     OFF,    70,     100,    20,     30,     10},   // 200V
        {47.0f,     OFF,    50,     100,    20,     30,     10},   // 210V
        {47.0f,     OFF,    40,     100,    25,     35,     10},   // 220V
        {45.0f,     OFF,    40,     100,    25,     35,     10},   // 230V
        {45.0f,     OFF,    40,     100,    25,     35,     10}    // 240V
    },

    // 온수 70도 선택
    {
        {74.0f,     ON,     30,     90,     20,     35,     20},    // 190V
        {74.0f,     ON,     30,     90,     20,     35,     20},    // 200V
        {73.0f,     ON,     10,     60,     20,     35,     20},    // 210V
        {72.0f,     ON,     10,     60,     25,     35,     20},    // 220V
        {71.0f,     ON,     10,     60,     25,     35,     30},    // 230V
        {70.0f,     ON,     10,     30,     35,     40,     30}     // 240V
    },

    // 온수 80도 선택
    {
        {91.0f,     ON,     100,    100,    20,     23,     20},    // 190V
        {91.0f,     ON,     100,    100,    20,     23,     20},    // 200V
        {91.0f,     ON,     100,    100,    20,     25,     30},    // 210V
        {89.0f,     ON,     80,     100,    20,     30,     30},    // 220V
        {87.0f,     ON,     70,     100,    22,     28,     30},    // 230V
        {87.0f,     ON,     60,     100,    26,     32,     30}     // 240V
    },

    // 온수 100도 선택
    {
        {91.0f,     ON,     100,    100,    20,     23,     20},    // 190V
        {91.0f,     ON,     100,    100,    20,     23,     20},    // 200V
        {91.0f,     ON,     100,    100,    20,     25,     30},    // 210V
        {89.0f,     ON,     80,     100,    20,     30,     30},    // 220V
        {87.0f,     ON,     70,     100,    22,     28,     30},    // 230V
        {87.0f,     ON,     60,     100,    26,     32,     30}     // 240V
    }
};


/// @brief      입수 온도 25도 이상 조건에서 추출중 히터 제어 데이터 테이블
__far const SInstHeatData_T  ga16Hot_InputUp25[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX] = {
//  목표온도,  Relay 제어,  Triac 제어 하한,  Triac 제어 상한,  제어 유량 하한,  제어 유량 상한,  제어 유량 Pulse
    // 온수 45도 선택
    {
        {49.0f,     OFF,    0,      100,    25,     40,     10},   // 190V
        {49.0f,     OFF,    0,      100,    25,     40,     10},   // 200V
        {47.0f,     OFF,    20,     100,    30,     40,     10},   // 210V
        {47.0f,     OFF,    10,     100,    30,     45,     10},   // 220V
        {45.0f,     OFF,    10,     100,    30,     45,     10},   // 230V
        {45.0f,     OFF,    10,     100,    30,     45,     10}    // 240V
    },

    // 온수 70도 선택
    {
        {74.0f,     ON,     20,     80,     25,     40,     20},    // 190V
        {74.0f,     ON,     20,     80,     25,     40,     20},    // 200V
        {72.0f,     ON,     0,      50,     30,     40,     20},    // 210V
        {72.0f,     ON,     0,      50,     30,     35,     22},    // 220V
        {70.0f,     ON,     0,      50,     30,     40,     30},    // 230V
        {70.0f,     ON,     0,      20,     35,     40,     30}     // 240V
    },

    // 온수 80도 선택
    {
        {91.0f,     ON,     100,    100,    20,     30,     30},    // 190V
        {91.0f,     ON,     100,    100,    20,     30,     30},    // 200V
        {89.0f,     ON,     100,    100,    25,     35,     30},    // 210V
        {87.0f,     ON,     80,     100,    25,     35,     30},    // 220V
        {86.0f,     ON,     60,     100,    25,     40,     30},    // 230V
        {85.0f,     ON,     40,     100,    30,     40,     30}     // 240V
    },

    // 온수 100도 선택
    {
        {91.0f,     ON,     100,    100,    20,     30,     30},    // 190V
        {91.0f,     ON,     100,    100,    20,     30,     30},    // 200V
        {89.0f,     ON,     100,    100,    25,     35,     30},    // 210V
        {87.0f,     ON,     80,     100,    25,     35,     30},    // 220V
        {86.0f,     ON,     60,     100,    25,     40,     30},    // 230V
        {85.0f,     ON,     40,     100,    30,     40,     30}     // 240V
    }
};

U8 gu8InstHeaterPeriodTimer = 0;        /// @brief  순간온수 히터 제어 주기 타이머
U8 gu8InstHeatingStep = 0;              /// @brief  순간온수 히팅 제어 단계 - enum 선언된 리스트 참조
U8 gu8InstHeat_HotSelect = 0;           /// @brief  순간온수 온수 선택 상태 - enum 선언된 리스트 참조
U8 gu8InstHeater_V_Detect = 0;          /// @brief  순간온수 히팅 제어를 위한 입력 전압 감지 단계 - enum 선언된 리스트 참조

U8 gu8RelayOnData = 0;                  /// @brief  순간온수 히터 Relay 제어 데이터
U8 gu8TriacActDutyData = 0;             /// @brief  순간온수 히터 Triac 제어 데이터


/// @brief      순간 온수 히팅 제어 관련 변수 초기화
/// @details    순간 온수 히팅 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Lib_InstHeater_Initialize(void)
{
    gu8InstHeaterPeriodTimer = 0;
    gu8InstHeatingStep = INST_HEATING_STEP_ID_NONE;
    gu8InstHeat_HotSelect = HOT_SELECT_ID_100;
    gu8InstHeater_V_Detect = V_DETECT_ID_220V;

    gu8RelayOnData = 0;
    gu8TriacActDutyData = 0;
}


/// @brief      Heat Tank Control Timer(@1ms)
/// @details    온수 탱크 히팅 제어를 위한 제어 주기 타이머 카운트를 한다
/// @param      void
/// @return     void
void InstHeater_ControlTimer(void)
{
    if (gu8InstHeaterPeriodTimer < INST_HEAT_CONTROL_TIME_PERIOD)
    {
        gu8InstHeaterPeriodTimer++;
    }
}


/// @brief      추출 유량 제어 함수
/// @details    순간 온수 추출시 유량을 제어하는 동작을 한다
/// @param      mu8OpenClose : 유량 제어시 유로 Open or Close 방향 - 0(Close), 1(Open)
///             mu16Pulse     : 유량 제어 모터 이동 Pulse
/// @return     void
void FlowControl(U8 mu8OpenClose, U16 mu16Pulse)
{
    if (mu8OpenClose == OPEN)
    {
        Set_Stepmotor_Move(STEPMOTOR_ID_1, STEPMOTOR_CW, mu16Pulse);
    }
    else
    {
        Set_Stepmotor_Move(STEPMOTOR_ID_1, STEPMOTOR_CCW, mu16Pulse);
    }
}


/// @brief      순간온수 히터 제어 단계 설정
/// @details    순간온수 히터 제어 단계 설정을 한다
/// @param      mu8Step : 순간온수 히팅 제어 단계 - enum으로 구성된 리스트 참조
/// @return     void
void Set_InstHeaterControlStep(U8 mu8Step)
{
    if (mu8Step >= INST_HEATING_STEP_ID_MAX)
    {   // 설정 가능 단계가 넘으면
        gu8InstHeatingStep = INST_HEATING_STEP_ID_NONE;     // OFF 설정
    }
    else
    {
        gu8InstHeatingStep = mu8Step;
    }
}


/// @brief      순간온수 히터 제어 설정 단계 획인 함수
/// @details    순간온수 히터 제어 설정 단계를 확인하여 그 상태를 반환한다
/// @param      void
/// @return     return : 순간온수 히팅 제어 단계 - enum으로 구성된 리스트 참조
U8 Get_InstHeaterControlStep(void)
{
    return  gu8InstHeatingStep;
}


/// @brief      순간온수 온수 온도 선택 설정
/// @details    순간온수 온수 중 선택한 온도를 설정한다
/// @param      mu8Step : 순간온수 온수 선택 단계 - enum으로 구성된 리스트 참조
/// @return     void
void Set_InstHeater_HotSelect(U8 mu8Select)
{
    if (mu8Select >= HOT_SELECT_ID_MAX)
    {   // 설정 가능 단계가 넘으면
        gu8InstHeat_HotSelect = HOT_SELECT_ID_100;     // 100도 선택 단계
    }
    else
    {
        gu8InstHeat_HotSelect = mu8Select;
    }
}


/// @brief      순간온수 온도 선택 확인
/// @details    순간온수 온수 선택 상태를 확인하여 반환한다
/// @param      void
/// @return     return : 순간온수 온수 선택 단계 - enum으로 구성된 리스트 참조
U8 Get_InstHeater_HotSelect(void)
{
    return  gu8InstHeat_HotSelect;
}


/// @brief      순간온수 히터 예열 단계 제어
/// @details    순간온수 히터 예열 단계 제어를 진행한다
/// @param      void
/// @return     void
void InstHeaterControl_Preheat(void)
{
    F32 mf32HotBodyTemp = 0;
    F32 mf32HotOutTemp = 0;
    F32 mf32HotInTemp = 0;

    mf32HotBodyTemp = Get_Temp(ADC_ID_TH_HOT_BODY);      // 순간온수 내부 온도 체크
    mf32HotOutTemp = Get_Temp(ADC_ID_TH_HOT_OUT);        // 순간온수 출수 온도 체크
    mf32HotInTemp = Get_Temp(ADC_ID_TH_HOT_IN);          // 순간온수 입수 온도 체크

    switch (gu8InstHeat_HotSelect)
    {
        case HOT_SELECT_ID_45:
            gu8RelayOnData = ON;

            if (mf32HotInTemp >= 25.0f)
            {   // 입수 온도가 25도 이상이면
                gu8TriacActDutyData = 30;
            }
            else
            {
                gu8TriacActDutyData = 60;
            }
            break;

        case HOT_SELECT_ID_70:
            gu8RelayOnData = ON;

            if (mf32HotInTemp >= 25.0f)
            {   // 입수 온도가 25도 이상이면
                gu8TriacActDutyData = 60;
            }
            else
            {
                gu8TriacActDutyData = 100;
            }
            break;

        case HOT_SELECT_ID_85:
            gu8RelayOnData = ON;

            if (mf32HotInTemp >= 25.0f)
            {   // 입수 온도가 25도 이상이면
                gu8TriacActDutyData = 100;
            }
            else
            {
                gu8TriacActDutyData = 100;
            }
            break;

        case HOT_SELECT_ID_100:
            gu8RelayOnData = ON;

            if (mf32HotInTemp >= 25.0f)
            {   // 입수 온도가 25도 이상이면
                gu8TriacActDutyData = 100;
            }
            else
            {
                gu8TriacActDutyData = 100;
            }
            break;

        default:
            gu8RelayOnData = OFF;
            gu8TriacActDutyData = 0;
            break;
    }

    if ( (mf32HotBodyTemp >= 98.0f) || (mf32HotOutTemp >= 98.0f) )
    {     // 온도가 98도가 넘을 때 - 과열 방지 안전 장치
        Set_Inst_HeaterControl(OFF, gu8TriacActDutyData, 0);
    }
    else
    {
        Set_Inst_HeaterControl(gu8RelayOnData, gu8TriacActDutyData, 0);
    }
}


/// @brief      순간온수 히터 추출 동작 중 제어
/// @details    순간온수 히터 제어를 추출 동작 중 제어한다
/// @param      void
/// @return     void
void InstHeaterControl_Effluent(void)
{
    F32 mf32HotBodyTemp = 0;
    F32 mf32HotOutTemp = 0;
    F32 mf32HotInTemp = 0;

    F32 mf32TagetHotTemp = 0;               // 온수 제어 목표 온도
    U8 mu8HeaterRelayOn = 0;                // 히터 Relay On/Off 상태값 - 0(OFF), 1(ON)
    U8 mu8TagetTriacLow = 0;                // 1000W 제어 목표 전력 Low값
    U8 mu8TagetTriacHigh = 0;               // 1000W 제어 목표 전력 High값
    U16 mu16TagetFlowHzLow = 0;             // 제어 목표 유량 Low값
    U16 mu16TagetFlowHzHigh = 0;            // 제어 목표 유량 High값
    U16 mu8ControlFlow = 0;                 // 유량 제어를 위해 한번에 움직이는 Stepmotor Pulse값

    U8 mu8StepmotorStatus = 0;              // Stepmotor 이동 동작 상태
    U16 mu16FlowHz = 0;                     // 현지의 유속 상태

    mf32HotBodyTemp = Get_Temp(ADC_ID_TH_HOT_BODY);      // 순간온수 내부 온도 체크
    mf32HotOutTemp = Get_Temp(ADC_ID_TH_HOT_OUT);        // 순간온수 출수 온도 체크
    mf32HotInTemp = Get_Temp(ADC_ID_TH_HOT_IN);          // 순간온수 입수 온도 체크

    if (mf32HotInTemp < 15.0f)
    {   // 입수 온도 15도 미만인 경우
        mf32TagetHotTemp    = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gf32TargetTemp;
        mu8HeaterRelayOn    = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8RelayStatus;
        mu8TagetTriacLow    = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacLowStd;
        mu8TagetTriacHigh   = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacHighStd;
        mu16TagetFlowHzLow  = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetLowFlow;
        mu16TagetFlowHzHigh = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetHighFlow;
        mu8ControlFlow      = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8FlowConPulse;
    }
    else if (mf32HotInTemp < 25.0f)
    {   // 입수 온도 25도 미만인 경우
        mf32TagetHotTemp    = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gf32TargetTemp;
        mu8HeaterRelayOn    = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8RelayStatus;
        mu8TagetTriacLow    = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacLowStd;
        mu8TagetTriacHigh   = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacHighStd;
        mu16TagetFlowHzLow  = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetLowFlow;
        mu16TagetFlowHzHigh = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetHighFlow;
        mu8ControlFlow      = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8FlowConPulse;
    }
    else
    {   // 입수 온도 25도 이상인 경우
        mf32TagetHotTemp    = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gf32TargetTemp;
        mu8HeaterRelayOn    = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8RelayStatus;
        mu8TagetTriacLow    = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacLowStd;
        mu8TagetTriacHigh   = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacHighStd;
        mu16TagetFlowHzLow  = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetLowFlow;
        mu16TagetFlowHzHigh = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetHighFlow;
        mu8ControlFlow      = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8FlowConPulse;
    }

    // 히터 제어
    if (mf32HotOutTemp > mf32TagetHotTemp)
    {   // 출수온도가 목표온도 보다 높을때
        if ( (mf32HotOutTemp >= 99.0f) || (mf32HotBodyTemp >= 99.0f) )
        {     // 출수 온도 또는 히터 내부 온도가 99도가 넘을 때
            gu8RelayOnData = OFF;               // Relay OFF
            gu8TriacActDutyData = 100;          // Triac 100% Duty
        }
        else
        {
            gu8RelayOnData = mu8HeaterRelayOn;

            if (gu8TriacActDutyData > mu8TagetTriacLow)
            {   // 1000W 히터를 목표 하한치 수준까지만 낮춤
                gu8TriacActDutyData = gu8TriacActDutyData - 1;
            }
        }

        // 정해진 최대 유량으로 제어되도록 제어
        mu8StepmotorStatus = Get_Stepmotor_Move_Status(STEPMOTOR_ID_1); // 스탭모터 동작중 상태 확인
        mu16FlowHz = Get_Flowmeter_Hz(FLOWMETER_1_ID);          // 유량(Hz) 상태 확인

        if (mu8StepmotorStatus == FALSE)
        {   // Stepmotor가 이동중이 아닌 경우
            if (mu16FlowHz < mu16TagetFlowHzHigh)
            {   // 유량이 상한치보다 낮은 경우
                FlowControl(OPEN, mu8ControlFlow);  // 유량을 늘림
            }
            else
            {   // 유량이 상한치보다 높은 경우
                FlowControl(CLOSE, mu8ControlFlow); // 유량을 줄임
            }
        }
    }
    else
    {   // 목표온도 보다 낮을
        if ( (mf32HotOutTemp >= 99.0f) || (mf32HotBodyTemp >= 99.0f) )
        {     // 출수 온도 또는 히터 내부 온도가 99도가 넘을 때
            gu8RelayOnData = OFF;               // Relay OFF
            gu8TriacActDutyData = 100;          // Triac 100% Duty
        }
        else
        {
            gu8RelayOnData = mu8HeaterRelayOn;

            if (gu8TriacActDutyData < mu8TagetTriacHigh)
            {   // 1000W 히터를 목표 상한치 수준까지만 올림
              gu8TriacActDutyData = gu8TriacActDutyData + 1;
            }
        }

        // 정해진 최저 유량으로 제어되도록 제어
        mu8StepmotorStatus = Get_Stepmotor_Move_Status(STEPMOTOR_ID_1); // 스탭모터 동작중 상태 확인
        mu16FlowHz = Get_Flowmeter_Hz(FLOWMETER_1_ID);          // 유량(Hz) 상태 확인

        if (mu8StepmotorStatus == FALSE)
        {   // Stepmotor가 이동중이 아닌 경우
            if (mu16FlowHz < mu16TagetFlowHzLow)
            {   // 유량이 하한치보다 낮은 경우
                FlowControl(OPEN, mu8ControlFlow);  // 유량을 늘림
            }
            else
            {   // 유량이 하한치보다 높은 경우
                FlowControl(CLOSE, mu8ControlFlow); // 유량을 줄임
            }
        }
    }

    Set_Inst_HeaterControl(gu8RelayOnData, gu8TriacActDutyData, 0);
}


/// @brief      순간온수 히터 제어 OFF
/// @details    순간온수 히터 제어 OFF 동작을 한다
/// @param      void
/// @return     void
void InstHeaterOff(void)
{
    gu8RelayOnData = OFF;
    gu8TriacActDutyData = 0;

    Set_Inst_HeaterControl(gu8RelayOnData, gu8TriacActDutyData, 0);
}



/// @brief      순간온수 히터 제어를 위한 입력 전압 체크
/// @details    순간온수 히터 제어를 위한 입력 전압을 체크하여 그 단계를 설정한다
/// @param      void
/// @return     void
void InstHeater_V_Detect(void)
{
    U8 mu8HeaterStatus = 0;
    U8 mu8TriacDutyStatus = 0;
    U8 mu8AD_V_Detect = 0;

    mu8HeaterStatus = Get_Inst_Heater_Status();
    mu8TriacDutyStatus = Get_Inst_HeaterTriacDuty();

    if ( (mu8HeaterStatus == INST_HEATER_ALL_ON) && (mu8TriacDutyStatus == 100) )   // 히터 두개가 다 Full Heating인 경우에만 체크
    {
        mu8AD_V_Detect = Get_ADC_Data(ADC_ID_V_FB);

        if (mu8AD_V_Detect <= INST_HEATER_INPUT_V_DETECT_STD_190V)     // ~ 198V
        {
            Set_InstHeater_V_Detect(V_DETECT_ID_190V);
        }
        else if ( (mu8AD_V_Detect > INST_HEATER_INPUT_V_DETECT_STD_190V) &&
                  (mu8AD_V_Detect <= INST_HEATER_INPUT_V_DETECT_STD_200V) ) // ~ 205V, 200V
        {
            Set_InstHeater_V_Detect(V_DETECT_ID_200V);
        }
        else if ( (mu8AD_V_Detect > INST_HEATER_INPUT_V_DETECT_STD_200V) &&
                  (mu8AD_V_Detect <= INST_HEATER_INPUT_V_DETECT_STD_210V) ) // 205 ~ 215V, 210V
        {
            Set_InstHeater_V_Detect(V_DETECT_ID_210V);
        }
        else if ( (mu8AD_V_Detect > INST_HEATER_INPUT_V_DETECT_STD_210V) &&
                  (mu8AD_V_Detect <= INST_HEATER_INPUT_V_DETECT_STD_220V) ) // 215 ~ 225V, 220V
        {
            Set_InstHeater_V_Detect(V_DETECT_ID_220V);
        }
        else if ( (mu8AD_V_Detect > INST_HEATER_INPUT_V_DETECT_STD_220V) &&
                  (mu8AD_V_Detect <= INST_HEATER_INPUT_V_DETECT_STD_230V) ) // 225 ~ 235V. 230V
        {
            Set_InstHeater_V_Detect(V_DETECT_ID_230V);
        }
        else
        {
            Set_InstHeater_V_Detect(V_DETECT_ID_240V);
        }
    }
}


/// @brief      순간온수 히터 제어를 위한 입력 전압 단계 설정
/// @details    순간온수 히터 제어를 위한 입력 전압 단계를 설정한다
/// @param      mu8Data : 순간온수 히터 제어를 위한 입력 전압 단계 - enum으로 구성된 리스트 참조
/// @return     void
void Set_InstHeater_V_Detect(U8 mu8Data)
{
    if (mu8Data >= V_DETECT_ID_MAX)
    {
        gu8InstHeater_V_Detect = V_DETECT_ID_240V;
    }
    else
    {
        gu8InstHeater_V_Detect = mu8Data;
    }
}


/// @brief      순간온수 히터 제어를 위한 입력 전압 단계 확인
/// @details    순간온수 히터 제어를 위한 입력 전압 단계를 확인하여 반환한다
/// @param      void
/// @return     return : 순간온수 히터 제어를 위한 입력 전압 단계 - enum으로 구성된 리스트 참조
U8 Get_InstHeater_V_Detect(void)
{
    return  gu8InstHeater_V_Detect;
}


/// @brief      순간온수 히터 제어를 위한 현재 온수 추출 단계 상태 설정
/// @details    순간온수 히터 제어를 위한 현재 온수 추출 단계를  설정한다
/// @param      mu8Data : 순간온수 히터 제어를 위한 현재 제어 단계 - enum으로 구성된 리스트 참조
/// @return     void
void Set_InstHeater_ControlStep(U8 mu8Data)
{
    if (mu8Data < INST_HEATING_STEP_ID_MAX)
    {
        gu8InstHeatingStep = mu8Data;
    }
    else
    {
        gu8InstHeatingStep = INST_HEATING_STEP_ID_NONE;
    }
}


/// @brief      순간온수 히터 제어를 위한 현재 온수 추출 단계 확인
/// @details    순간온수 히터 제어를 위한 현재 온수 추출 단계를 확인하여 반환한다
/// @param      void
/// @return     return : 순간온수 히터 제어를 위한 현재 제어 단계 - enum으로 구성된 리스트 참조
U8 Get_InstHeater_ControlStep(void)
{
    return  gu8InstHeatingStep;
}


/// @brief      순간온수 히터 제어 함수(@While 문)
/// @details    순간온수 히터 제어 동작을 한다.
/// @param      void
/// @return     void
void Lib_InstHeater_Control(void)
{
    U8 mu8HotInTempError = 0;
    U8 mu8HotBodyTempError = 0;
    U8 mu8HotOutTempError = 0;
    mu8HotInTempError = Get_ErrorStatus(ERROR_ID_HOT_IN_TEMP);
    mu8HotBodyTempError = Get_ErrorStatus(ERROR_ID_HOT_BODY_TEMP);
    mu8HotOutTempError = Get_ErrorStatus(ERROR_ID_HOT_OUT_TEMP);

    if ( (mu8HotInTempError == FALSE) &&
         (mu8HotBodyTempError == FALSE) &&
         (mu8HotOutTempError == FALSE) )
    {
        InstHeater_V_Detect();      // 입력 전압 실시간으로 측정

        if (gu8InstHeaterPeriodTimer >= INST_HEAT_CONTROL_TIME_PERIOD)
        {   // 일정 시간 마다 동작 제어
            gu8InstHeaterPeriodTimer = 0;

            if (gu8InstHeatingStep == INST_HEATING_STEP_ID_PREHEAT)
            {   // 예열 단계
                InstHeaterControl_Preheat();
            }
            else if (gu8InstHeatingStep == INST_HEATING_STEP_ID_EFFLUENT)
            {   // 추출 단계
                InstHeaterControl_Effluent();
            }
            else
            {   // 미동작 단계
                InstHeaterOff();
            }
        }
    }
    else
    {
        InstHeaterOff();
    }
}

#endif


// Lib Instantaneous Heater Module ********************************************************************************************

/// @brief      Lib Instantaneous Heater Module Initilaize
/// @details    일반 히터 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_InstHeater_Module_Initialize(void)
{
#if (INSTANT_HEATER_USE == USE)
    Lib_InstHeater_Initialize();
#endif
}


/// @brief      Lib Instantaneous Heater Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 Heater 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_InstHeater_Module_1ms_Control(void)
{
#if (INSTANT_HEATER_USE == USE)
    InstHeater_ControlTimer();
#endif
}


/// @brief      Lib Instantaneous Heater Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Heater 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_InstHeater_Module_Control(void)
{
#if (INSTANT_HEATER_USE == USE)
    Lib_InstHeater_Control();
#endif
}

