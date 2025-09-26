/// @file   Lib_Water_Effluent.c
/// @date   2025/06/26
/// @author Jaejin Ham
/// @brief  추출 동작 모드 주관 파일

#include "Global_header.h"


/// @brief      추출 동작 관련 데이터
/// @details    추출 동작 제어를 하기 위한 관련 데이터들을 구조체로 선언한다
typedef struct
{
    U8 gu8WaterSelect;                                  /// @brief  물 종류 선택
    U8 gu8AmountSelect;                                 /// @brief  물 정량 선택
    U8 au8Quantitative[EFFLUENT_ID_MAX];                /// @brief  정량 추출 여부 - 0(미정량), 1(정량)
    U16 au16Time[EFFLUENT_ID_MAX];                      /// @brief  정량 추출 시간
    U16 au16Hz[EFFLUENT_ID_MAX];                        /// @brief  정량 추출 Hz
    U8 au8Status[EFFLUENT_ID_MAX];                      /// @brief  에러 발생 확정 상태 - 0(미발생), 1(발생)
}   SEffluentData_T;

SEffluentData_T    SEffluentData;

U8 gu8EffluentPeriodTimer = 0;                          /// @brief  추출 동작 주기 시간 카운터
U16 au16EffluentTimeCount[EFFLUENT_ID_MAX];             /// @brief  추출 동작 시간 카운터

U8 gu8RoomEffluentStep = 0;                             /// @brief  정수 추출 단계
U16 gu16RoomEffluentStepDelay = 0;                      /// @brief  정수 추출 단계 동작 중 동작 지연 시간
U8 gu8ColdEffluentStep = 0;                             /// @brief  냉수 추출 단계
U16 gu16ColdEffluentStepDelay = 0;                      /// @brief  냉수 추출 단계 동작 중 동작 지연 시간
U8 gu8HotEffluentStep = 0;                              /// @brief  온수 추출 단계
U16 gu16HotEffluentStepDelay = 0;                       /// @brief  온수 추출 단계 동작 중 동작 지연 시간

U8 gu8DefaultWaterSelect = 0;                           /// @brief  냉수, 정수 중 Default 선택 상태
U8 gu8DefaultCupSize = 0;                               /// @brief  컵 사이즈 default값

U16 gu16MyCupAmount = 0;                                /// @brief  마이컵 용량(ml)



/// @brief      추출 동작 관련 변수 초기화 함수
/// @details    추출 동작 관련 변수 데이터들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_Effluent_Initialize(void)
{
    U8 mu8i = 0;

    MEMSET( (void __FAR *)&SEffluentData, 0x00, sizeof(SEffluentData_T) );

    gu8EffluentPeriodTimer = 0;

    for (mu8i = 0 ; mu8i < EFFLUENT_ID_MAX ; mu8i++)
    {
        au16EffluentTimeCount[mu8i] = 0;
    }

    SEffluentData.gu8WaterSelect = EFFLUENT_ID_ROOM;
    SEffluentData.gu8AmountSelect = AMOUNT_ID_HALF_CUP;

    gu8RoomEffluentStep = 0;
    gu16RoomEffluentStepDelay = 0;
    gu8ColdEffluentStep = 0;
    gu16ColdEffluentStepDelay = 0;
    gu8HotEffluentStep = 0;
    gu16HotEffluentStepDelay = 0;

    gu8DefaultWaterSelect = EFFLUENT_ID_ROOM;
    gu8DefaultCupSize = AMOUNT_ID_HALF_CUP;
    gu16MyCupAmount = 130;
}


/// @brief      선택된 물 종류 설정 함수
/// @details    물 종류를 선택하여 설정한다.
/// @param      mu8ID : 물 종류 ID
/// @return     void
void Set_WaterSelect(U8 mu8ID)
{
    SEffluentData.gu8WaterSelect = mu8ID;
}


/// @brief      선택된 물 종류 확인 함수
/// @details    선택되어 있는 물 종류를 확인하여 반환한다.
/// @param      void
/// @return     return : 물 종류 ID
U8 Get_WaterSelect(void)
{
    return  SEffluentData.gu8WaterSelect;
}


/// @brief      기본 선택 물 종류 설정 함수
/// @details    기본 선택 물 종류를 선택하여 설정한다.
/// @param      mu8ID : 물 종류 ID
/// @return     void
void Set_DefaultWaterSelect(U8 mu8ID)
{
    gu8DefaultWaterSelect = mu8ID;
}


/// @brief      기본 선택 물 종류 확인 함수
/// @details    기본 선택 물 종류를 확인하여 반환한다.
/// @param      void
/// @return     return : 물 종류 ID
U8 Get_DefaultWaterSelect(void)
{
    return  gu8DefaultWaterSelect;
}



/// @brief      선택된 물양 설정 함수
/// @details    물양을 선택하여 설정한다.
/// @param      mu8ID : 물양 ID
/// @return     void
void Set_WaterAmount(U8 mu8ID)
{
    SEffluentData.gu8AmountSelect = mu8ID;
}


/// @brief      선택된 물양 확인 함수
/// @details    선택되어 있는 물양을 확인하여 반환한다.
/// @param      void
/// @return     return : 물양 ID
U8 Get_WaterAmount(void)
{
    return  SEffluentData.gu8AmountSelect;
}


/// @brief      기본 선택 물양 설정 함수
/// @details    기본 선택 물양을 설정한다.
/// @param      mu8ID : 물양 ID
/// @return     void
void Set_DefaultWaterAmount(U8 mu8ID)
{
    gu8DefaultCupSize = mu8ID;
}


/// @brief      기본 선택 물양 확인 함수
/// @details    기본 선택 물양을 확인하여 반환한다.
/// @param      void
/// @return     return : 물양 ID
U8 Get_DefaultWaterAmount(void)
{
    return  gu8DefaultCupSize;
}


/// @brief      마이컵 용량 설정 함수
/// @details    마이컵 용량을 설정한다
/// @param      mu16Data : 마이컵 설정 용량 - 130ml ~ 1000ml
/// @return     void
void Set_MyCupAmount(U16 mu16Data)
{
    gu16MyCupAmount = mu16Data;
}


/// @brief      마이컵 용량 설정 상태 확인 함수
/// @details    설정된 마이컵 용량을 확인하여 반환한다
/// @param      void
/// @return     return : 마이컵 설정 용량 - 130ml ~ 1000ml
U16 Get_MyCupAmount(void)
{
    return  gu16MyCupAmount;
}


/// @brief      추출 동작 제어 Timer(@1ms interrupt)
/// @details    추출 동작 제어를 위한 제어 주기 타이머를 카운트 한다
/// @param      void
/// @return     void
void EffluentControlTimer(void)
{
    if (gu8EffluentPeriodTimer < EFFLUENT_CONTROL_PERIOD_TIME)
    {
        gu8EffluentPeriodTimer++;
    }

    EffluentTimeCountdown();    // 추출 시간 카운트 다운 제어
}


/// @brief      추출 동작 Time 카운트 다운(@1ms interrupt)
/// @details    추출 동작 제어 시간을 카운트 다운 한다
/// @param      void
/// @return     void
void EffluentTimeCountdown(void)
{
    U8 mu8i = 0;

    static U8 mu8Timer = 0;

    if (++mu8Timer >= 100)
    {   // 100ms 마다 동작
        mu8Timer = 0;

        for (mu8i = 0 ; mu8i < EFFLUENT_ID_MAX ; mu8i++)
        {
            if (au16EffluentTimeCount[mu8i])
            {
                au16EffluentTimeCount[mu8i]--;
            }
        }
    }
}


/// @brief      추출 동작 시간 카운트 다운 완료 체크 함수
/// @details    추출 동작 시간 카운트 다운이 완료되었는지 확인하여 그 상태를 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 정량 추출 동작 시간 카운트 다운 완료 여부 - 0(미완료), 1(완료)
U8 Get_EffluentTimeCountdownEnd(U8 mu8ID)
{
    if (au16EffluentTimeCount[mu8ID])
    {   // 추출 잔여 시간이 있으면
        return  FALSE;
    }
    else
    {   // 추출 잔여 시간이 없으면
        return  TRUE;
    }
}


/// @brief      추출 동작이 정량 추출인지 설정 함수
/// @details    추출 동작이 정량 추출인지 여부를 설정한다
/// @param      mu8ID : 물 종류 ID
///             mu8Data : 정량 추출 여부 데이터 - 0(미정량), 1(정량)
/// @return     void
void Set_QuantitativeEffluent(U8 mu8ID, U8 mu8Data)
{
    if (mu8Data)
    {
        SEffluentData.au8Quantitative[mu8ID] = SET;
    }
    else
    {
        SEffluentData.au8Quantitative[mu8ID] = CLEAR;
    }
}


/// @brief      추출 동작이 정량 추출인지 확인 함수
/// @details    추출 동작이 정량 추출인지 여부를 확인하여 그 상태를 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 정량 추출 여부 데이터 - 0(미정량), 1(정량)
U8 Get_QuantitativeEffluent(U8 mu8ID)
{
    if (SEffluentData.au8Quantitative[mu8ID])
    {   // 정량 추출인 경우
        return  TRUE;
    }
    else
    {   // 정량 추출이 아닌 경우
        return  FALSE;
    }
}


/// @brief      추출 동작 시간 설정 함수
/// @details    추출 동작을 진행할 시간을 설정한다
/// @param      mu8ID : 물 종류 ID
///             mu8Data : 정량 추출 시간값(100ms단위)
/// @return     void
void Set_EffluentTime(U8 mu8ID, U16 mu16Data)
{
    SEffluentData.au16Time[mu8ID] = mu16Data;   // 설정된 추출 제어 시간

    au16EffluentTimeCount[mu8ID] = mu16Data;    // 실제 카운트 다운할 제어 시간
}


/// @brief      추출 동작 설정 시간 확인 함수
/// @details    추출 동작 설정 시간을 확인하여 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 정량 추출 시간값(100ms단위)
U16 Get_EffluentTime(U8 mu8ID)
{
    return  SEffluentData.au16Time[mu8ID];
}


/// @brief      추출 동작 잔여 시간 확인 함수
/// @details    추출 동작 잔여 시간을 확인하여 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 정량 추출 시간값(100ms단위)
U16 Get_EffluentRemainTime(U8 mu8ID)
{
    return  au16EffluentTimeCount[mu8ID];
}


/// @brief      추출 동작 Hz 설정 함수
/// @details    추출 동작을 진행할 Hz를 설정한다
/// @param      mu8ID : 물 종류 ID
///             mu8Data : 정량 추출 Pulse값
/// @return     void
void Set_EffluentHz(U8 mu8ID, U16 mu16Data)
{
    SEffluentData.au16Hz[mu8ID] = mu16Data;     // 설정된 추출 제어 Hz

#if (EFFLUENT_FLOWMETER_USE == USE)
    Set_FlowmeterCountdownPulse(FLOWMETER_1_ID, mu16Data);      // 실제 카운트 다운할 유량센서 제어 Hz 설정
#endif
}


/// @brief      추출 동작 설정 Hz 확인 함수
/// @details    추출 동작 진행할 설정된 Hz를 확인하여 그 값을 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 정량 추출 Pulse값
U16 Get_EffluentHz(U8 mu8ID)
{
    return  SEffluentData.au16Hz[mu8ID];
}


/// @brief      추출 동작 잔여 Hz 확인 함수
/// @details    추출 동작 진행하는 중의 잔여 Hz를 확인하여 그 값을 반환한다
/// @param      mu8FlowmeterID : 유량센서 ID
/// @return     return : 정량 추출 Pulse값
U16 Get_EffluentRemainHz(U8 mu8FlowmeterID)
{
#if (EFFLUENT_FLOWMETER_USE == USE)
    return  Get_FlowmeterCountdownPulse(mu8FlowmeterID);
#else
    return 0;
#endif
}


/// @brief      추출 동작 Pulse 카운트 다운 완료 체크 함수
/// @details    추출 동작 Pulse 카운트 다운이 완료되었는지 확인하여 그 상태를 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 정량 추출 동작 시간 카운트 다운 완료 여부 - 0(미완료), 1(완료)
U8 Get_EffluentHzCountdownEnd(U8 mu8ID)
{
#if (EFFLUENT_FLOWMETER_USE == USE)
    U8 mu8FlowmeterCountdownEnd = 0;
    U8 mu8ReturnData = 0;

    if (mu8ID == EFFLUENT_ID_ROOM)
    {
        mu8FlowmeterCountdownEnd = Get_FlowmeterCountdownEnd(FLOWMETER_1_ID);

        if (mu8FlowmeterCountdownEnd == TRUE)
        {   // 카운트 다운이 완료된 경우
            mu8ReturnData = TRUE;
        }
        else
        {
            mu8ReturnData = FALSE;
        }
    }

    if (mu8ID == EFFLUENT_ID_COLD)
    {
        mu8FlowmeterCountdownEnd = Get_FlowmeterCountdownEnd(FLOWMETER_2_ID);

        if (mu8FlowmeterCountdownEnd == TRUE)
        {   // 카운트 다운이 완료된 경우
            mu8ReturnData = TRUE;
        }
        else
        {
            mu8ReturnData = FALSE;
        }
    }

    if (mu8ID == EFFLUENT_ID_HOT)
    {
        mu8FlowmeterCountdownEnd = Get_FlowmeterCountdownEnd(FLOWMETER_3_ID);

        if (mu8FlowmeterCountdownEnd == TRUE)
        {   // 카운트 다운이 완료된 경우
            mu8ReturnData = TRUE;
        }
        else
        {
            mu8ReturnData = FALSE;
        }
    }
    return  mu8ReturnData;
#else
    return  TRUE;
#endif
}


/// @brief      추출 동작 설정 함수
/// @details    추출 동작이 동작 중인지 여부를 설정한다
/// @param      mu8ID : 물 종류 ID
///             mu8Data : 추출 상태 여부 데이터 - 0(미추출), 1(추출)
/// @return     void
void Set_EffluentStatus(U8 mu8ID, U8 mu8Data)
{
    if (mu8Data)
    {
        SEffluentData.au8Status[mu8ID] = SET;
    }
    else
    {
        SEffluentData.au8Status[mu8ID] = CLEAR;
    }
}


/// @brief      추출 동작중 확인 함수
/// @details    추출 동작이 진행중인지 확인하여 그 상태를 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 추출 상태 여부 데이터 - 0(미추출), 1(추출)
U8 Get_EffluentStatus(U8 mu8ID)
{
    if (SEffluentData.au8Status[mu8ID])
    {
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      추출 동작 데이터 초기화 함수
/// @details    ID에 해당하는 물 종류의 추출 동작 관련 데이터를 모두 초기화 시킨다
/// @param      mu8ID : 물 종류 ID
/// @return     void
void Set_ClearEffluentData(U8 mu8ID)
{
    if (mu8ID == EFFLUENT_ID_COLD)
    {
        SEffluentData.au8Quantitative[EFFLUENT_ID_COLD] = CLEAR;
        SEffluentData.au16Time[EFFLUENT_ID_COLD] = 0;
        SEffluentData.au16Hz[EFFLUENT_ID_COLD] = 0;
        SEffluentData.au8Status[EFFLUENT_ID_COLD] = CLEAR;
    }
    else if (mu8ID == EFFLUENT_ID_HOT)
    {
        SEffluentData.au8Quantitative[EFFLUENT_ID_HOT] = CLEAR;
        SEffluentData.au16Time[EFFLUENT_ID_HOT] = 0;
        SEffluentData.au16Hz[EFFLUENT_ID_HOT] = 0;
        SEffluentData.au8Status[EFFLUENT_ID_HOT] = CLEAR;
    }
    else
    {
        SEffluentData.au8Quantitative[EFFLUENT_ID_ROOM] = CLEAR;
        SEffluentData.au16Time[EFFLUENT_ID_ROOM] = 0;
        SEffluentData.au16Hz[EFFLUENT_ID_ROOM] = 0;
        SEffluentData.au8Status[EFFLUENT_ID_ROOM] = CLEAR;
    }
}


/// @brief      추출 동작이 완료되었는지 확인 함수
/// @details    추출 동작이 완료되었는지 확인하여 그 상태값을 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 정량 추출 완료 데이터 - 0(미완료), 1(완료)
U8 Get_EffluentEndCheck(U8 mu8ID)
{
    U8 mu8EffluentTimeEnd = 0;
    U8 mu8EffluentHzEnd = 0;

    mu8EffluentTimeEnd = Get_EffluentTimeCountdownEnd(mu8ID);
    mu8EffluentHzEnd = Get_EffluentHzCountdownEnd(mu8ID);

    if ( (mu8EffluentTimeEnd == TRUE) && (mu8EffluentHzEnd == TRUE) )
    {   // 추출 시간, 추출 Hz 모두 0이 되었을 때
        return  TRUE;
    }
    else
    {
        return  FALSE;
    }
}


/// @brief      Effluent 제어 함수(100ms)
/// @details    추출 동작을 상황에 맞춰 제어하는 동작을 한다
/// @param      void
/// @return     void
void Lib_Effluent_Control(void)
{
    U8 mu8EffluentStatus = 0;

    if (gu8EffluentPeriodTimer >= EFFLUENT_CONTROL_PERIOD_TIME)
    {   // 100ms 마다 제어
        gu8EffluentPeriodTimer = 0;

        mu8EffluentStatus = Get_OperatingStatus();

        if (mu8EffluentStatus == OPERATION_ID_EFFLUENT)
        {   // 추출 동작 중이면
            if (SEffluentData.au8Status[EFFLUENT_ID_ROOM] == SET)
            {   // 정수 추출 상태인 경우
                RoomEffluentControl();
            }
            else
            {   // 정수 추출 중이 아니면 정수 추출 관련 데이터 초기화
                Set_ClearEffluentData(EFFLUENT_ID_ROOM);
            }

            if (SEffluentData.au8Status[EFFLUENT_ID_COLD] == SET)
            {   // 냉수 추출 상태인 경우
                ColdEffluentControl();
            }
            else
            {   // 냉수 추출 중이 아니면 냉수 추출 관련 데이터 초기화
                Set_ClearEffluentData(EFFLUENT_ID_COLD);
            }

            if (SEffluentData.au8Status[EFFLUENT_ID_HOT] == SET)
            {   // 온수 추출 상태인 경우
                HotEffluentControl();
            }
            else
            {   // 온수 추출 상태가 아니면 온수 추출 관련 데이터 초기화
                Set_ClearEffluentData(EFFLUENT_ID_HOT);
            }
        }
        else
        {   // 추출 동작 중이 아니면
            if (SEffluentData.au8Status[EFFLUENT_ID_ROOM] == SET)
            {   // 정수 추출 상태인 경우 추출 정지
                RoomEffluentStop();
            }

            if (SEffluentData.au8Status[EFFLUENT_ID_COLD] == SET)
            {   // 냉수 추출 상태인 경우 추출 정지
                ColdEffluentStop();
            }

            if (SEffluentData.au8Status[EFFLUENT_ID_HOT] == SET)
            {   // 온수 추출 상태인 경우 추출 정지
                HotEffluentStop();
            }
        }
    }
}


/// @brief      Effluent 강제 종료 함수
/// @details    추출 동작을 강제로 종료시킨다
/// @param      void
/// @return     void
void Lib_Effluent_Stop(void)
{
    U8 mu8EffluentStatus = 0;

    mu8EffluentStatus = Get_OperatingStatus();

    if (mu8EffluentStatus == OPERATION_ID_EFFLUENT)
    {   // 추출 상태였으면 대기 상태로 전환
        Set_OperatingStatus(OPERATION_ID_STAND_BY);
    }

    if (SEffluentData.au8Status[EFFLUENT_ID_ROOM] == SET)
    {   // 정수 추출 상태인 경우 추출 정지
        RoomEffluentStop();
    }

    if (SEffluentData.au8Status[EFFLUENT_ID_COLD] == SET)
    {   // 냉수 추출 상태인 경우 추출 정지
        ColdEffluentStop();
    }

    if (SEffluentData.au8Status[EFFLUENT_ID_HOT] == SET)
    {   // 온수 추출 상태인 경우 추출 정지
        HotEffluentStop();
    }
}



/// @brief      정수 추출 제어 함수
/// @details    정수 추출 동작을 제어한다
/// @param      void
/// @return     void
void RoomEffluentControl(void)
{
    U8 mu8EffluentEnd = 0;

    switch(gu8RoomEffluentStep)
    {
        case EFFLUENT_PROCESS_STEP_0 :      // 추출 시작
            Set_FlowMeterErrorCheckStart(SET);      // 유량센서 에러 감지 시작
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_1 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_2 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_3 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_4 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_5 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_6 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_7 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_8 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_9 :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_10 :     // 추출 종료
            mu8EffluentEnd = Get_EffluentEndCheck(EFFLUENT_ID_ROOM);

            if (mu8EffluentEnd == TRUE)
            {
                gu16RoomEffluentStepDelay = 0;
                gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_0;

                Set_FlowMeterErrorCheckStart(CLEAR);      // 유량센서 에러 감지 종료
                Set_OperatingStatus(OPERATION_ID_STAND_BY);
                Set_ClearEffluentData(EFFLUENT_ID_ROOM);
            }
            break;

        default :
            gu16RoomEffluentStepDelay = 0;
            gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_0;
            Set_FlowMeterErrorCheckStart(CLEAR);      // 유량센서 에러 감지 종료

            Set_EffluentTime(EFFLUENT_ID_ROOM, 0);
            Set_EffluentHz(EFFLUENT_ID_ROOM, 0);

            Set_OperatingStatus(OPERATION_ID_STAND_BY);
            Set_ClearEffluentData(EFFLUENT_ID_ROOM);
            break;
    }
}


/// @brief      정수 추출 정지 함수
/// @details    정수 추출 동작을 정지한다
/// @param      void
/// @return     void
void RoomEffluentStop(void)
{
    gu16RoomEffluentStepDelay = 0;
    gu8RoomEffluentStep = EFFLUENT_PROCESS_STEP_0;

    Set_FlowMeterErrorCheckStart(CLEAR);      // 유량센서 에러 감지 종료
    Set_EffluentTime(EFFLUENT_ID_ROOM, 0);
    Set_EffluentHz(EFFLUENT_ID_ROOM, 0);

    Set_ClearEffluentData(EFFLUENT_ID_ROOM);
}



/// @brief      냉수 추출 제어 함수
/// @details    냉수 추출 동작을 제어한다
/// @param      void
/// @return     void
void ColdEffluentControl(void)
{
    U8 mu8EffluentEnd = 0;

    switch(gu8ColdEffluentStep)
    {
        case EFFLUENT_PROCESS_STEP_0 :      // 추출 시작
            Set_FlowMeterErrorCheckStart(SET);      // 유량센서 에러 감지 시작
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_1 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_2 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_3 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_4 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_5 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_6 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_7 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_8 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_9 :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_10 :     // 추출 종료
            mu8EffluentEnd = Get_EffluentEndCheck(EFFLUENT_ID_COLD);

            if (mu8EffluentEnd == TRUE)
            {
                gu16ColdEffluentStepDelay = 0;
                gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_0;

                Set_FlowMeterErrorCheckStart(CLEAR);      // 유량센서 에러 감지 종료
                Set_OperatingStatus(OPERATION_ID_STAND_BY);
                Set_ClearEffluentData(EFFLUENT_ID_COLD);
            }
            break;

        default :
            gu16ColdEffluentStepDelay = 0;
            gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_0;
            Set_FlowMeterErrorCheckStart(CLEAR);      // 유량센서 에러 감지 종료

            Set_EffluentTime(EFFLUENT_ID_COLD, 0);
            Set_EffluentHz(EFFLUENT_ID_COLD, 0);

            Set_OperatingStatus(OPERATION_ID_STAND_BY);
            Set_ClearEffluentData(EFFLUENT_ID_COLD);
            break;
    }
}


/// @brief      냉수 추출 정지 함수
/// @details    냉수 추출 동작을 정지한다
/// @param      void
/// @return     void
void ColdEffluentStop(void)
{
    gu16ColdEffluentStepDelay = 0;
    gu8ColdEffluentStep = EFFLUENT_PROCESS_STEP_0;
    Set_FlowMeterErrorCheckStart(CLEAR);      // 유량센서 에러 감지 종료

    Set_EffluentTime(EFFLUENT_ID_COLD, 0);
    Set_EffluentHz(EFFLUENT_ID_COLD, 0);

    Set_ClearEffluentData(EFFLUENT_ID_COLD);
}


/// @brief      온수 추출 제어 함수
/// @details    온수 추출 동작을 제어한다
/// @param      void
/// @return     void
void HotEffluentControl(void)
{
    U8 mu8EffluentEnd = 0;

    switch(gu8HotEffluentStep)
    {
        case EFFLUENT_PROCESS_STEP_0 :      // 추출 시작
            Set_HotCutOffCheckStart(SET);      // 유량센서 에러 감지 시작
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_1 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_2 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_3 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_4 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_5 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_6 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_7 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_8 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_9 :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_10;
            break;

        case EFFLUENT_PROCESS_STEP_10 :     // 추출 종료
            mu8EffluentEnd = Get_EffluentEndCheck(EFFLUENT_ID_HOT);

            if (mu8EffluentEnd == TRUE)
            {
                gu16HotEffluentStepDelay = 0;
                gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_0;

                Set_HotCutOffCheckStart(CLEAR);      // 유량센서 에러 감지 종료

                Set_OperatingStatus(OPERATION_ID_STAND_BY);
                Set_ClearEffluentData(EFFLUENT_ID_HOT);
            }
            break;

        default :
            gu16HotEffluentStepDelay = 0;
            gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_0;

            Set_HotCutOffCheckStart(CLEAR);      // 유량센서 에러 감지 종료

            Set_EffluentTime(EFFLUENT_ID_HOT, 0);
            Set_EffluentHz(EFFLUENT_ID_HOT, 0);

            Set_OperatingStatus(OPERATION_ID_STAND_BY);
            Set_ClearEffluentData(EFFLUENT_ID_HOT);
            break;
    }
}


/// @brief      온수 추출 정지 함수
/// @details    온수 추출 동작을 정지한다
/// @param      void
/// @return     void
void HotEffluentStop(void)
{
    gu16HotEffluentStepDelay = 0;
    gu8HotEffluentStep = EFFLUENT_PROCESS_STEP_0;

    Set_HotCutOffCheckStart(CLEAR);      // 유량센서 에러 감지 종료

    Set_EffluentTime(EFFLUENT_ID_HOT, 0);
    Set_EffluentHz(EFFLUENT_ID_HOT, 0);

    Set_ClearEffluentData(EFFLUENT_ID_HOT);
}


/// @brief      추출 동작의 제어 단계 변경 함수
/// @details    ID에 해당하는 물 종류의 추출 동작 중에 추출 제어 단계를 변경해서 동작시키려고 할 때 사용한다
/// @param      mu8ID : 물 종류 ID
///             mu8Step : 변경하려고 하는 추출 제어 단계(0~10단계)
/// @return     void
void Set_EffluentStep(U8 mu8ID, U8 mu8Step)
{
    if (mu8ID == EFFLUENT_ID_COLD)
    {
        gu16ColdEffluentStepDelay = 0;
        gu8ColdEffluentStep = mu8Step;
    }
    else if (mu8ID == EFFLUENT_ID_HOT)
    {
        gu16HotEffluentStepDelay = 0;
        gu8HotEffluentStep = mu8Step;
    }
    else if (mu8ID == EFFLUENT_ID_ROOM)
    {
        gu16RoomEffluentStepDelay = 0;
        gu8RoomEffluentStep = mu8Step;
    }
}


/// @brief      추출 동작중 단계 확인 함수
/// @details    추출 동작의 진행 단계를 확인하여 그 상태를 반환한다
/// @param      mu8ID : 물 종류 ID
/// @return     return : 추출 동작 단계
U8 Get_EffluentStep(U8 mu8ID)
{
    if (mu8ID == EFFLUENT_ID_COLD)
    {
        return  gu8ColdEffluentStep;
    }
    else if (mu8ID == EFFLUENT_ID_HOT)
    {
        return  gu8HotEffluentStep;
    }
    else if (mu8ID == EFFLUENT_ID_ROOM)
    {
        return  gu8RoomEffluentStep;
    }
    else
    {
        return  0;
    }
}



// Lib Effluent Module *********************************************************************************

/// @brief      Lib Effluent Module Initilaize
/// @details    추출 제어 모듈의 변수들을 초기화 시킨다
/// @param      void
/// @return     void
void Lib_Effluent_Module_Initialize(void)
{
    Lib_Effluent_Initialize();
}


/// @brief      Lib Effluent Module in 1ms Interrupt
/// @details    1ms Interrupt에 추가될 Effluent 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_Effluent_Module_1ms_Control(void)
{
    EffluentControlTimer();
}


/// @brief      Lib BoEffluent Module in While Loop
/// @details    Main 함수 안의 While Loop 안에 추가될 Effluent 제어 모듈 Library
/// @param      void
/// @return     void
void Lib_Effluent_Module_Control(void)
{
    U8 mu8EffluentStatus = 0;

    mu8EffluentStatus = Get_OperatingStatus();

    if (mu8EffluentStatus == OPERATION_ID_EFFLUENT)
    {
        Lib_Effluent_Control();
    }
}


