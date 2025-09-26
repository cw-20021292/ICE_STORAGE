/// @file     Hal_Flowmeter.c
/// @date     2025/03/20
/// @author   Jaejin Ham
/// @brief    Flowmeter Control file


#include "Global_Header.h"


#if (FLOWMETER_COUNT > 0)


U16 gu16FlowmeterHzCalculationTimer = 0;                        /// @brief  �������� Hz ��� �ֱ� Ÿ�̸�

U16 gau16FlowmeterTestHz[FLOWMETER_ID_MAX_COUNT];               /// @brief  �׽�Ʈ ��忡�� ������ �������� Hz

/// @brief  Flowmeter ���� ���� �ڷ���
typedef struct {
    U16 au16FlowMeterPulseCount[FLOWMETER_ID_MAX_COUNT];        /// @brief  �������� �ܺ� ���ͷ�Ʈ Pulse ����
    U16 au16FlowMeter500msA[FLOWMETER_ID_MAX_COUNT];            /// @brief  �������� ���� 500ms Pulse ����
    U16 au16FlowMeter500msB[FLOWMETER_ID_MAX_COUNT];            /// @brief  �������� �ֱ� 500ms Pulse ����
    U16 au16FlowMeterHz[FLOWMETER_ID_MAX_COUNT];                /// @brief  �������� 1�ʰ� Pulse ����(Hz)
    U16 au16FlowMeterCountDownPulse[FLOWMETER_ID_MAX_COUNT];    /// @brief  �������� ī��Ʈ �ٿ� Pulse ����
}   SFlowmeterData_T;

SFlowmeterData_T   SFlowmeterData;


/// @brief      Flowmeter Initialize
/// @details    Flowmeter ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Flowmeter_Initialize(void)
{
    MEMSET( (void __FAR *)&SFlowmeterData, 0x00, sizeof(SFlowmeterData_T) );

    gu16FlowmeterHzCalculationTimer = 0;
}


#if (FLOWMETER_COUNT >= 1)
/// @brief      Flowmeter 1 Interrupt(@External interrupt)
/// @details    Flowmeter 1 interrupt ��� �Ѵ�
/// @param      void
/// @return     void
void Flowmeter_1_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_1_ID] >= 32767)
    {   // 16bit �ִ밪(65535)�� ������ 32767������ ī��Ʈ
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
/// @details    Flowmeter 2 interrupt ��� �Ѵ�
/// @param      void
/// @return     void
void Flowmeter_2_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_2_ID] >= 32767)
    {   // 16bit �ִ밪(65535)�� ������ 32767������ ī��Ʈ
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
/// @details    Flowmeter 3 interrupt ��� �Ѵ�
/// @param      void
/// @return     void
void Flowmeter_3_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_3_ID] >= 32767)
    {   // 16bit �ִ밪(65535)�� ������ 32767������ ī��Ʈ
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
/// @details    Flowmeter 4 interrupt ��� �Ѵ�
/// @param      void
/// @return     void
void Flowmeter_4_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_4_ID] >= 32767)
    {   // 16bit �ִ밪(65535)�� ������ 32767������ ī��Ʈ
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
/// @details    Flowmeter 5 interrupt ��� �Ѵ�
/// @param      void
/// @return     void
void Flowmeter_5_Interrupt(void)
{
    if (SFlowmeterData.au16FlowMeterPulseCount[FLOWMETER_5_ID] >= 32767)
    {   // 16bit �ִ밪(65535)�� ������ 32767������ ī��Ʈ
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
/// @details    1ms Interrupt �ȿ��� �ֱ������� Flowmeter üũ�� �Ѵ�
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
/// @details    Flowmeter Hz �� ����Ѵ�
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


/// @brief      Flowmeter Hz Ȯ�� �Լ�
/// @details    Flowmeter Hz�� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8FlowmeterID : Ȯ���Ϸ��� �ϴ� Flowmeter�� ID
/// @return     return : ���� ������ ���� Hz
U16 Get_Flowmeter_Hz(U8 mu8FlowmeterID)
{
    U16 mu16Return = 0;

    mu16Return = SFlowmeterData.au16FlowMeterHz[mu8FlowmeterID];

    return  mu16Return;
}


/// @brief      Flowmeter �� ���� ī��Ʈ �ٿ� �� Pulse ���� �Լ�
/// @details    Flowmeter �� ���� ī��Ʈ �ٿ��� ������ Pulse�� �����Ѵ�
/// @param      mu8FlowmeterID : �����Ϸ��� �ϴ� Flowmeter�� ID
///             mu16Pulse : ī��Ʈ �ٿ� �Ϸ��� Pulse ��
/// @return     void
void Set_FlowmeterCountdownPulse(U8 mu8FlowmeterID, U16 mu16Pulse)
{
    SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID] = mu16Pulse;
}

/// @brief      Flowmeter �� ���� ī��Ʈ �ٿ� ���� �ܿ� Pulse Ȯ�� �Լ�
/// @details    Flowmeter �� ���� ī��Ʈ �ٿ� ���� �ܿ� Pulse�� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8FlowmeterID : Ȯ���Ϸ��� �ϴ� Flowmeter�� ID
/// @return     return : ī��Ʈ �ٿ� �ϴ� Pulse ��
U16 Get_FlowmeterCountdownPulse(U8 mu8FlowmeterID)
{
    return  SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID];
}


/// @brief      Flowmeter�� ���� ���� Pulse ī��Ʈ �ٿ� �Լ�
/// @details    Flowmeter�� ���� ������ Pulse�� ī��Ʈ �ٿ� ó���Ѵ�
/// @param      mu8FlowmeterID : Flowmeter�� ID
/// @return     void
void Flowmeter_Pulse_CountDownHz(U8 mu8FlowmeterID)
{
    if (SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID])
    {
        SFlowmeterData.au16FlowMeterCountDownPulse[mu8FlowmeterID]--;
    }
}


/// @brief      Flowmeter�� ���� Pulse ī��Ʈ �ٿ� �Ϸ� üũ �Լ�
/// @details    Flowmeter�� ���� ������ Pulse�� ī��Ʈ �ٿ� �Ϸ�Ǿ����� Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      mu8FlowmeterID : Ȯ���Ϸ��� �ϴ� Flowmeter�� ID
/// @return     return : ī��Ʈ�ٿ� �Ϸ� ���� - 0(Not Complete), 1(Complete)
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


/// @brief      Flowmeter �׽�Ʈ ���� �� ���� �Լ�
/// @details    Flowmeter �� �׽�Ʈ �����ϰ� ������ Hz���� �����Ѵ�
/// @param      mu8FlowmeterID : Flowmeter�� ID
///             mu16Hz : ������ Hz
/// @return     void
void Set_FlowmeterTestHz(U8 mu8FlowmeterID, U16 mu16Hz)
{
    gau16FlowmeterTestHz[mu8FlowmeterID] = mu16Hz;
}

/// @brief      Flowmeter �׽�Ʈ ���� ���°� Ȯ�� �Լ�
/// @details    Flowmeter �� �׽�Ʈ �����ϰ� ������ Hz���� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      mu8FlowmeterID : Ȯ���Ϸ��� �ϴ� Flowmeter�� ID
/// @return     return : ���� ������ ���� Hz
U16 Get_FlowmeterTestHz(U8 mu8FlowmeterID)
{
    return  gau16FlowmeterTestHz[mu8FlowmeterID];
}

#endif


// Hal FlowMeter Module **************************************************************************************

/// @brief      Hal Flowmeter Initilaize Module
/// @details    �������� ���� ���� �������� ��� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Hal_Flowmeter_Module_Initialize(void)
{
#if (FLOWMETER_COUNT > 0)
    Flowmeter_Initialize();                 // ���� ���� ���� �ʱ�ȭ
#endif
}


/// @brief      Hal Flowmeter Module in 1ms Interrupt
/// @details    1ms Interrupt���� �������� ���� ���� Ÿ�̸Ӹ� ī��Ʈ �Ѵ�
/// @param      void
/// @return     void
void Hal_Flowmeter_Module_1ms_Control(void)
{
#if (FLOWMETER_COUNT > 0)
    Flowmeter_CheckControl();               // Flowmeter Check ����
#endif
}


