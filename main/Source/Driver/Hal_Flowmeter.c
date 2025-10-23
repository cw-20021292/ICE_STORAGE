/// @file     Hal_Flowmeter.c
/// @date     2025/03/20
/// @author   Jaejin Ham
/// @brief    Flowmeter Control file


#include "Global_Header.h"


#if (FLOWMETER_COUNT > 0)


U16 gu16FlowmeterHzCalculationTimer = 0;                        /// @brief  유량센서 Hz 계산 주기 타이머

U16 gau16FlowmeterTestHz[FLOWMETER_ID_MAX_COUNT];               /// @brief  테스트 모드에서 측정된 유량센서 Hz

/// @brief  Flowmeter 제어 관리 자료형
typedef struct {
    U16 au16FlowMeterPulseCount[FLOWMETER_ID_MAX_COUNT];        /// @brief  유량센서 외부 인터럽트 Pulse 갯수
    U16 au16FlowMeter500msA[FLOWMETER_ID_MAX_COUNT];            /// @brief  유량센서 이전 500ms Pulse 갯수
    U16 au16FlowMeter500msB[FLOWMETER_ID_MAX_COUNT];            /// @brief  유량센서 최근 500ms Pulse 갯수
    U16 au16FlowMeterHz[FLOWMETER_ID_MAX_COUNT];                /// @brief  유량센서 1초간 Pulse 갯수(Hz)
    U16 au16FlowMeterCountDownPulse[FLOWMETER_ID_MAX_COUNT];    /// @brief  유량센서 카운트 다운 Pulse 갯수
}   SFlowmeterData_T;

SFlowmeterData_T   SFlowmeterData;


/// @brief      Flowmeter Initialize
/// @details    Flowmeter 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Flowmeter_Initialize(void)
{
    MEMSET( (void __FAR *)&SFlowmeterData, 0x00, sizeof(SFlowmeterData_T) );

    gu16FlowmeterHzCalculationTimer = 0;
}


#if (FLOWMETER_COUNT >= 1)
/// @brief      Flowmeter 1 Interrupt(@External interrupt)
/// @details    Flowmeter 1 interrupt 제어를 한다
/// @param      void
/// @return     void
void Flowmeter_1_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_1_ID] >= 32767)
    {   // 16bit 최대값(65535)의 절반인 32767까지만 카운트
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_1_ID] = 32767;
    }
    else
    {
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_1_ID]++;
    }

    Flowmeter_Pulse_CountDownHz(FLOWMETER_1_ID);
}
#endif


#if (FLOWMETER_COUNT >= 2)
/// @brief      Flowmeter 2 Interrupt(@External interrupt)
/// @details    Flowmeter 2 interrupt 제어를 한다
/// @param      void
/// @return     void
void Flowmeter_2_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_2_ID] >= 32767)
    {   // 16bit 최대값(65535)의 절반인 32767까지만 카운트
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_2_ID] = 32767;
    }
    else
    {
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_2_ID]++;
    }

    Flowmeter_Pulse_CountDownHz(FLOWMETER_2_ID);
}
#endif


#if (FLOWMETER_COUNT >= 3)
/// @brief      Flowmeter 3 Interrupt(@External interrupt)
/// @details    Flowmeter 3 interrupt 제어를 한다
/// @param      void
/// @return     void
void Flowmeter_3_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_3_ID] >= 32767)
    {   // 16bit 최대값(65535)의 절반인 32767까지만 카운트
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_3_ID] = 32767;
    }
    else
    {
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_3_ID]++;
    }

    Flowmeter_Pulse_CountDownHz(FLOWMETER_3_ID);
}
#endif


#if (FLOWMETER_COUNT >= 4)
/// @brief      Flowmeter 4 Interrupt(@External interrupt)
/// @details    Flowmeter 4 interrupt 제어를 한다
/// @param      void
/// @return     void
void Flowmeter_4_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_4_ID] >= 32767)
    {   // 16bit 최대값(65535)의 절반인 32767까지만 카운트
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_4_ID] = 32767;
    }
    else
    {
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_4_ID]++;
    }

    Flowmeter_Pulse_CountDownHz(FLOWMETER_4_ID);
}
#endif


#if (FLOWMETER_COUNT >= 5)
/// @brief      Flowmeter 5 Interrupt(@External interrupt)
/// @details    Flowmeter 5 interrupt 제어를 한다
/// @param      void
/// @return     void
void Flowmeter_5_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_5_ID] >= 32767)
    {   // 16bit 최대값(65535)의 절반인 32767까지만 카운트
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_5_ID] = 32767;
    }
    else
    {
        SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_5_ID]++;
    }

    Flowmeter_Pulse_CountDownHz(FLOWMETER_5_ID);
}
#endif


/// @brief      Flowmeter Check Control Time(@1ms interrupt)
/// @details    1ms Interrupt 안에서 주기적으로 Flowmeter 체크를 한다
/// @param      void
/// @return     void
void Flowmeter_CheckControl(void)
{
    if (gu16FlowmeterHzCalculationTimer < FLOWMETER_CHECK_CONTROL_TIME_PERIOD)
    {
        gu16FlowmeterHzCalculationTimer++;
    }

    if (gu16FlowmeterHzCalculationTimer >= FLOWMETER_CHECK_CONTROL_TIME_PERIOD)
    {
        gu16FlowmeterHzCalculationTimer = 0;
        Flowmeter_Hz_Calculation();
    }
}


/// @brief      Flowmeter Hz Calculation
/// @details    Flowmeter Hz 를 계산한다
/// @param      void
/// @return     void
void Flowmeter_Hz_Calculation(void)
{
    U8 mu8FlowmeterBuf = 0;

     for (mu8FlowmeterBuf = 1 ; mu8FlowmeterBuf < FLOWMETER_ID_MAX_COUNT ; mu8FlowmeterBuf++)
    {
        SFlowmeterData.au16FlowMeter500msA[mu8FlowmeterBuf] = SFlowmeterData.au16FlowMeterPulseCount[mu8FlowmeterBuf];
        SFlowmeterData.au16FlowMeterPulseCount[mu8FlowmeterBuf] = 0;
        SFlowmeterData.au16FlowMeterHz[mu8FlowmeterBuf] = SFlowmeterData.au16FlowMeter500msB[mu8FlowmeterBuf] + SFlowmeterData.au16FlowMeter500msA[mu8FlowmeterBuf];
        SFlowmeterData.au16FlowMeter500msB[mu8FlowmeterBuf] = SFlowmeterData.au16FlowMeter500msA[mu8FlowmeterBuf];
    }
}


/// @brief      Flowmeter Hz 확인 함수
/// @details    Flowmeter Hz를 확인하여 반환한다
/// @param      mu8FlowmeterID : 확인하려고 하는 Flowmeter의 ID
/// @return     return : 현재 측정된 유속 Hz
U16 Get_Flowmeter_Hz(U8 mu8FlowmeterID)
{
    U16 mu16Return = 0;

    mu16Return = SFlowmeterData.au16FlowMeterHz[mu8FlowmeterID];

    return  mu16Return;
}


/// @brief      Flowmeter 를 통해 카운트 다운 할 Pulse 설정 함수
/// @details    Flowmeter 를 통해 카운트 다운을 진행할 Pulse를 설정한다
/// @param      mu8FlowmeterID : 설정하려고 하는 Flowmeter의 ID
///             mu16Pulse : 카운트 다운 하려는 Pulse 수
/// @return     void
void Set_FlowmeterCountdownPulse(U8 mu8FlowmeterID, U16 mu16Pulse)
{
    SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID] = mu16Pulse;
}

/// @brief      Flowmeter 를 통해 카운트 다운 중인 잔여 Pulse 확인 함수
/// @details    Flowmeter 를 통해 카운트 다운 중인 잔여 Pulse를 확인하여 반환한다
/// @param      mu8FlowmeterID : 확인하려고 하는 Flowmeter의 ID
/// @return     return : 카운트 다운 하는 Pulse 수
U16 Get_FlowmeterCountdownPulse(U8 mu8FlowmeterID)
{
    return  SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID];
}


/// @brief      Flowmeter를 통한 설정 Pulse 카운트 다운 함수
/// @details    Flowmeter를 통해 설정된 Pulse를 카운트 다운 처리한다
/// @param      mu8FlowmeterID : Flowmeter의 ID
/// @return     void
void Flowmeter_Pulse_CountDownHz(U8 mu8FlowmeterID)
{
    if (SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID])
    {
        SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID]--;
    }
}


/// @brief      Flowmeter를 통한 Pulse 카운트 다운 완료 체크 함수
/// @details    Flowmeter를 통해 설정된 Pulse가 카운트 다운 완료되었는지 확인하여 그 상태를 반환한다
/// @param      mu8FlowmeterID : 확인하려고 하는 Flowmeter의 ID
/// @return     return : 카운트다운 완료 상태 - 0(Not Complete), 1(Complete)
U8 Get_FlowmeterCountdownEnd(U8 mu8FlowmeterID)
{
    if (SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID])
    {
        return  FALSE;
    }
    else
    {
        return  TRUE;
    }
}


/// @brief      Flowmeter 테스트 상태 값 설정 함수
/// @details    Flowmeter 를 테스트 진행하고 측정된 Hz값을 설정한다
/// @param      mu8FlowmeterID : Flowmeter의 ID
///             mu16Hz : 측정된 Hz
/// @return     void
void Set_FlowmeterTestHz(U8 mu8FlowmeterID, U16 mu16Hz)
{
    gau16FlowmeterTestHz[mu8FlowmeterID] = mu16Hz;
}

/// @brief      Flowmeter 테스트 진행 상태값 확인 함수
/// @details    Flowmeter 를 테스트 진행하고 측정된 Hz값을 확인하여 반환한다
/// @param      mu8FlowmeterID : 확인하려고 하는 Flowmeter의 ID
/// @return     return : 현재 측정된 유속 Hz
U16 Get_FlowmeterTestHz(U8 mu8FlowmeterID)
{
    return  gau16FlowmeterTestHz[mu8FlowmeterID];
}

#endif


// Hal FlowMeter Module **************************************************************************************

/// @brief      Hal Flowmeter Initilaize Module
/// @details    유량센서 제어 관련 변수들을 모두 초기화 시킨다
/// @param      void
/// @return     void
void Hal_Flowmeter_Module_Initialize(void)
{
#if (FLOWMETER_COUNT > 0)
    Flowmeter_Initialize();                 // 유량 센서 관련 초기화
#endif
}


/// @brief      Hal Flowmeter Module in 1ms Interrupt
/// @details    1ms Interrupt에서 유량센서 제어 관련 타이머를 카운트 한다
/// @param      void
/// @return     void
void Hal_Flowmeter_Module_1ms_Control(void)
{
#if (FLOWMETER_COUNT > 0)
    Flowmeter_CheckControl();               // Flowmeter Check 제어
#endif
}


