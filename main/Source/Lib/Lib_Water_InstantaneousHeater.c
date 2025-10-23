/// @file     Lib_Water_InstantaneousHeater.c
/// @date     2025/07/03
/// @author   Jaejin Ham
/// @brief    ���� �¼� Heater ���� ���� file

#include "Global_Header.h"


#if (INSTANT_HEATER_USE == USE)


/// @brief      �����¼� ���� ���� ������ ����
/// @details    �����¼� ���� ��� �ϱ� ���� �����͵��� ����ü�� �����Ѵ�
typedef struct
{
    F32 gf32TargetTemp;                     /// @brief  ��ǥ �µ�(������, �Ҽ���)
    U8 gu8RelayStatus;                      /// @brief  Relay ���� ���� - 0(OFF), 1(ON)
    U8 gu8TriacLowStd;                      /// @brief  Triac ���� �ּ� Duty(%)
    U8 gu8TriacHighStd;                     /// @brief  Triac ���� �ִ� Duty(%)
    U16 gu16TagetLowFlow;                   /// @brief  ���� ��ǥ ���� ����(Hz)
    U16 gu16TagetHighFlow;                  /// @brief  ���� ��ǥ ���� ����(Hz)
    U8 gu8FlowConPulse;                     /// @brief  �ѹ� ���� ����� ������ Stepmoter���� Pulse
}   SInstHeatData_T;


/// @brief      �Լ� �µ� 15�� �̸� ���ǿ��� ������ ���� ���� ������ ���̺�
__far const SInstHeatData_T  ga16Hot_InputUnder15[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX] = {
//  ��ǥ�µ�,  Relay ����,  Triac ���� ����,  Triac ���� ����,  ���� ���� ����,  ���� ���� ����,  ���� ���� Pulse
    // �¼� 45�� ����
    {
        {49.0f,     OFF,    80,     100,    20,     30,     10},    // 190V
        {49.0f,     OFF,    80,     100,    20,     30,     10},    // 200V
        {49.0f,     OFF,    100,    100,    20,     30,     10},    // 210V
        {47.0f,     OFF,    80,     100,    20,     30,     10},    // 220V
        {45.0f,     OFF,    80,     100,    20,     30,     10},    // 230V
        {45.0f,     OFF,    80,     100,    20,     30,     10}     // 240V
    },

    // �¼� 70�� ����
    {
        {76.0f,     ON,     80,     100,    20,     30,     20},    // 190V
        {76.0f,     ON,     80,     100,    20,     30,     20},    // 200V
        {73.0f,     ON,     60,     100,    20,     30,     20},    // 210V
        {72.0f,     ON,     40,     70,     20,     30,     20},    // 220V
        {71.0f,     ON,     30,     60,     20,     30,     30},    // 230V
        {70.0f,     ON,     30,     50,     30,     35,     30}     // 240V
    },

    // �¼� 80�� ����
    {
        {91.0f,     ON,     100,    100,    20,     22,     20},    // 190V
        {91.0f,     ON,     100,    100,    20,     22,     20},    // 200V
        {91.0f,     ON,     90,     100,    20,     22,     20},    // 210V
        {89.0f,     ON,     70,     100,    20,     22,     30},    // 220V
        {88.0f,     ON,     80,     100,    22,     26,     30},    // 230V
        {88.0f,     ON,     70,     100,    22,     28,     30}     // 240V
    },

    // �¼� 100�� ����
    {
        {91.0f,     ON,     100,    100,    20,     22,     20},    // 190V
        {91.0f,     ON,     100,    100,    20,     22,     20},    // 200V
        {91.0f,     ON,     90,     100,    20,     22,     20},    // 210V
        {89.0f,     ON,     70,     100,    20,     22,     30},    // 220V
        {88.0f,     ON,     80,     100,    22,     26,     30},    // 230V
        {88.0f,     ON,     70,     100,    22,     28,     30}     // 240V
    }
};


/// @brief      �Լ� �µ� 25�� �̸� ���ǿ��� ������ ���� ���� ������ ���̺�
__far const SInstHeatData_T  ga16Hot_InputUnder25[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX] = {
//  ��ǥ�µ�,  Relay ����,  Triac ���� ����,  Triac ���� ����,  ���� ���� ����,  ���� ���� ����,  ���� ���� Pulse
    // �¼� 45�� ����
    {
        {49.0f,     OFF,    70,     100,    20,     30,     10},   // 190V
        {49.0f,     OFF,    70,     100,    20,     30,     10},   // 200V
        {47.0f,     OFF,    50,     100,    20,     30,     10},   // 210V
        {47.0f,     OFF,    40,     100,    25,     35,     10},   // 220V
        {45.0f,     OFF,    40,     100,    25,     35,     10},   // 230V
        {45.0f,     OFF,    40,     100,    25,     35,     10}    // 240V
    },

    // �¼� 70�� ����
    {
        {74.0f,     ON,     30,     90,     20,     35,     20},    // 190V
        {74.0f,     ON,     30,     90,     20,     35,     20},    // 200V
        {73.0f,     ON,     10,     60,     20,     35,     20},    // 210V
        {72.0f,     ON,     10,     60,     25,     35,     20},    // 220V
        {71.0f,     ON,     10,     60,     25,     35,     30},    // 230V
        {70.0f,     ON,     10,     30,     35,     40,     30}     // 240V
    },

    // �¼� 80�� ����
    {
        {91.0f,     ON,     100,    100,    20,     23,     20},    // 190V
        {91.0f,     ON,     100,    100,    20,     23,     20},    // 200V
        {91.0f,     ON,     100,    100,    20,     25,     30},    // 210V
        {89.0f,     ON,     80,     100,    20,     30,     30},    // 220V
        {87.0f,     ON,     70,     100,    22,     28,     30},    // 230V
        {87.0f,     ON,     60,     100,    26,     32,     30}     // 240V
    },

    // �¼� 100�� ����
    {
        {91.0f,     ON,     100,    100,    20,     23,     20},    // 190V
        {91.0f,     ON,     100,    100,    20,     23,     20},    // 200V
        {91.0f,     ON,     100,    100,    20,     25,     30},    // 210V
        {89.0f,     ON,     80,     100,    20,     30,     30},    // 220V
        {87.0f,     ON,     70,     100,    22,     28,     30},    // 230V
        {87.0f,     ON,     60,     100,    26,     32,     30}     // 240V
    }
};


/// @brief      �Լ� �µ� 25�� �̻� ���ǿ��� ������ ���� ���� ������ ���̺�
__far const SInstHeatData_T  ga16Hot_InputUp25[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX] = {
//  ��ǥ�µ�,  Relay ����,  Triac ���� ����,  Triac ���� ����,  ���� ���� ����,  ���� ���� ����,  ���� ���� Pulse
    // �¼� 45�� ����
    {
        {49.0f,     OFF,    0,      100,    25,     40,     10},   // 190V
        {49.0f,     OFF,    0,      100,    25,     40,     10},   // 200V
        {47.0f,     OFF,    20,     100,    30,     40,     10},   // 210V
        {47.0f,     OFF,    10,     100,    30,     45,     10},   // 220V
        {45.0f,     OFF,    10,     100,    30,     45,     10},   // 230V
        {45.0f,     OFF,    10,     100,    30,     45,     10}    // 240V
    },

    // �¼� 70�� ����
    {
        {74.0f,     ON,     20,     80,     25,     40,     20},    // 190V
        {74.0f,     ON,     20,     80,     25,     40,     20},    // 200V
        {72.0f,     ON,     0,      50,     30,     40,     20},    // 210V
        {72.0f,     ON,     0,      50,     30,     35,     22},    // 220V
        {70.0f,     ON,     0,      50,     30,     40,     30},    // 230V
        {70.0f,     ON,     0,      20,     35,     40,     30}     // 240V
    },

    // �¼� 80�� ����
    {
        {91.0f,     ON,     100,    100,    20,     30,     30},    // 190V
        {91.0f,     ON,     100,    100,    20,     30,     30},    // 200V
        {89.0f,     ON,     100,    100,    25,     35,     30},    // 210V
        {87.0f,     ON,     80,     100,    25,     35,     30},    // 220V
        {86.0f,     ON,     60,     100,    25,     40,     30},    // 230V
        {85.0f,     ON,     40,     100,    30,     40,     30}     // 240V
    },

    // �¼� 100�� ����
    {
        {91.0f,     ON,     100,    100,    20,     30,     30},    // 190V
        {91.0f,     ON,     100,    100,    20,     30,     30},    // 200V
        {89.0f,     ON,     100,    100,    25,     35,     30},    // 210V
        {87.0f,     ON,     80,     100,    25,     35,     30},    // 220V
        {86.0f,     ON,     60,     100,    25,     40,     30},    // 230V
        {85.0f,     ON,     40,     100,    30,     40,     30}     // 240V
    }
};

U8 gu8InstHeaterPeriodTimer = 0;        /// @brief  �����¼� ���� ���� �ֱ� Ÿ�̸�
U8 gu8InstHeatingStep = 0;              /// @brief  �����¼� ���� ���� �ܰ� - enum ����� ����Ʈ ����
U8 gu8InstHeat_HotSelect = 0;           /// @brief  �����¼� �¼� ���� ���� - enum ����� ����Ʈ ����
U8 gu8InstHeater_V_Detect = 0;          /// @brief  �����¼� ���� ��� ���� �Է� ���� ���� �ܰ� - enum ����� ����Ʈ ����

U8 gu8RelayOnData = 0;                  /// @brief  �����¼� ���� Relay ���� ������
U8 gu8TriacActDutyData = 0;             /// @brief  �����¼� ���� Triac ���� ������


/// @brief      ���� �¼� ���� ���� ���� ���� �ʱ�ȭ
/// @details    ���� �¼� ���� ���� ���� �������� ��� �ʱ�ȭ ��Ų��
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
/// @details    �¼� ��ũ ���� ��� ���� ���� �ֱ� Ÿ�̸� ī��Ʈ�� �Ѵ�
/// @param      void
/// @return     void
void InstHeater_ControlTimer(void)
{
    if (gu8InstHeaterPeriodTimer < INST_HEAT_CONTROL_TIME_PERIOD)
    {
        gu8InstHeaterPeriodTimer++;
    }
}


/// @brief      ���� ���� ���� �Լ�
/// @details    ���� �¼� ����� ������ �����ϴ� ������ �Ѵ�
/// @param      mu8OpenClose : ���� ����� ���� Open or Close ���� - 0(Close), 1(Open)
///             mu16Pulse     : ���� ���� ���� �̵� Pulse
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


/// @brief      �����¼� ���� ���� �ܰ� ����
/// @details    �����¼� ���� ���� �ܰ� ������ �Ѵ�
/// @param      mu8Step : �����¼� ���� ���� �ܰ� - enum���� ������ ����Ʈ ����
/// @return     void
void Set_InstHeaterControlStep(U8 mu8Step)
{
    if (mu8Step >= INST_HEATING_STEP_ID_MAX)
    {   // ���� ���� �ܰ谡 ������
        gu8InstHeatingStep = INST_HEATING_STEP_ID_NONE;     // OFF ����
    }
    else
    {
        gu8InstHeatingStep = mu8Step;
    }
}


/// @brief      �����¼� ���� ���� ���� �ܰ� ȹ�� �Լ�
/// @details    �����¼� ���� ���� ���� �ܰ踦 Ȯ���Ͽ� �� ���¸� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �����¼� ���� ���� �ܰ� - enum���� ������ ����Ʈ ����
U8 Get_InstHeaterControlStep(void)
{
    return  gu8InstHeatingStep;
}


/// @brief      �����¼� �¼� �µ� ���� ����
/// @details    �����¼� �¼� �� ������ �µ��� �����Ѵ�
/// @param      mu8Step : �����¼� �¼� ���� �ܰ� - enum���� ������ ����Ʈ ����
/// @return     void
void Set_InstHeater_HotSelect(U8 mu8Select)
{
    if (mu8Select >= HOT_SELECT_ID_MAX)
    {   // ���� ���� �ܰ谡 ������
        gu8InstHeat_HotSelect = HOT_SELECT_ID_100;     // 100�� ���� �ܰ�
    }
    else
    {
        gu8InstHeat_HotSelect = mu8Select;
    }
}


/// @brief      �����¼� �µ� ���� Ȯ��
/// @details    �����¼� �¼� ���� ���¸� Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �����¼� �¼� ���� �ܰ� - enum���� ������ ����Ʈ ����
U8 Get_InstHeater_HotSelect(void)
{
    return  gu8InstHeat_HotSelect;
}


/// @brief      �����¼� ���� ���� �ܰ� ����
/// @details    �����¼� ���� ���� �ܰ� ��� �����Ѵ�
/// @param      void
/// @return     void
void InstHeaterControl_Preheat(void)
{
    F32 mf32HotBodyTemp = 0;
    F32 mf32HotOutTemp = 0;
    F32 mf32HotInTemp = 0;

    mf32HotBodyTemp = Get_Temp(ADC_ID_TH_HOT_BODY);      // �����¼� ���� �µ� üũ
    mf32HotOutTemp = Get_Temp(ADC_ID_TH_HOT_OUT);        // �����¼� ��� �µ� üũ
    mf32HotInTemp = Get_Temp(ADC_ID_TH_HOT_IN);          // �����¼� �Լ� �µ� üũ

    switch (gu8InstHeat_HotSelect)
    {
        case HOT_SELECT_ID_45:
            gu8RelayOnData = ON;

            if (mf32HotInTemp >= 25.0f)
            {   // �Լ� �µ��� 25�� �̻��̸�
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
            {   // �Լ� �µ��� 25�� �̻��̸�
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
            {   // �Լ� �µ��� 25�� �̻��̸�
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
            {   // �Լ� �µ��� 25�� �̻��̸�
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
    {     // �µ��� 98���� ���� �� - ���� ���� ���� ��ġ
        Set_Inst_HeaterControl(OFF, gu8TriacActDutyData, 0);
    }
    else
    {
        Set_Inst_HeaterControl(gu8RelayOnData, gu8TriacActDutyData, 0);
    }
}


/// @brief      �����¼� ���� ���� ���� �� ����
/// @details    �����¼� ���� ��� ���� ���� �� �����Ѵ�
/// @param      void
/// @return     void
void InstHeaterControl_Effluent(void)
{
    F32 mf32HotBodyTemp = 0;
    F32 mf32HotOutTemp = 0;
    F32 mf32HotInTemp = 0;

    F32 mf32TagetHotTemp = 0;               // �¼� ���� ��ǥ �µ�
    U8 mu8HeaterRelayOn = 0;                // ���� Relay On/Off ���°� - 0(OFF), 1(ON)
    U8 mu8TagetTriacLow = 0;                // 1000W ���� ��ǥ ���� Low��
    U8 mu8TagetTriacHigh = 0;               // 1000W ���� ��ǥ ���� High��
    U16 mu16TagetFlowHzLow = 0;             // ���� ��ǥ ���� Low��
    U16 mu16TagetFlowHzHigh = 0;            // ���� ��ǥ ���� High��
    U16 mu8ControlFlow = 0;                 // ���� ��� ���� �ѹ��� �����̴� Stepmotor Pulse��

    U8 mu8StepmotorStatus = 0;              // Stepmotor �̵� ���� ����
    U16 mu16FlowHz = 0;                     // ������ ���� ����

    mf32HotBodyTemp = Get_Temp(ADC_ID_TH_HOT_BODY);      // �����¼� ���� �µ� üũ
    mf32HotOutTemp = Get_Temp(ADC_ID_TH_HOT_OUT);        // �����¼� ��� �µ� üũ
    mf32HotInTemp = Get_Temp(ADC_ID_TH_HOT_IN);          // �����¼� �Լ� �µ� üũ

    if (mf32HotInTemp < 15.0f)
    {   // �Լ� �µ� 15�� �̸��� ���
        mf32TagetHotTemp    = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gf32TargetTemp;
        mu8HeaterRelayOn    = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8RelayStatus;
        mu8TagetTriacLow    = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacLowStd;
        mu8TagetTriacHigh   = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacHighStd;
        mu16TagetFlowHzLow  = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetLowFlow;
        mu16TagetFlowHzHigh = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetHighFlow;
        mu8ControlFlow      = ga16Hot_InputUnder15[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8FlowConPulse;
    }
    else if (mf32HotInTemp < 25.0f)
    {   // �Լ� �µ� 25�� �̸��� ���
        mf32TagetHotTemp    = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gf32TargetTemp;
        mu8HeaterRelayOn    = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8RelayStatus;
        mu8TagetTriacLow    = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacLowStd;
        mu8TagetTriacHigh   = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacHighStd;
        mu16TagetFlowHzLow  = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetLowFlow;
        mu16TagetFlowHzHigh = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetHighFlow;
        mu8ControlFlow      = ga16Hot_InputUnder25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8FlowConPulse;
    }
    else
    {   // �Լ� �µ� 25�� �̻��� ���
        mf32TagetHotTemp    = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gf32TargetTemp;
        mu8HeaterRelayOn    = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8RelayStatus;
        mu8TagetTriacLow    = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacLowStd;
        mu8TagetTriacHigh   = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8TriacHighStd;
        mu16TagetFlowHzLow  = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetLowFlow;
        mu16TagetFlowHzHigh = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu16TagetHighFlow;
        mu8ControlFlow      = ga16Hot_InputUp25[gu8InstHeat_HotSelect][gu8InstHeater_V_Detect].gu8FlowConPulse;
    }

    // ���� ����
    if (mf32HotOutTemp > mf32TagetHotTemp)
    {   // ����µ��� ��ǥ�µ� ���� ������
        if ( (mf32HotOutTemp >= 99.0f) || (mf32HotBodyTemp >= 99.0f) )
        {     // ��� �µ� �Ǵ� ���� ���� �µ��� 99���� ���� ��
            gu8RelayOnData = OFF;               // Relay OFF
            gu8TriacActDutyData = 100;          // Triac 100% Duty
        }
        else
        {
            gu8RelayOnData = mu8HeaterRelayOn;

            if (gu8TriacActDutyData > mu8TagetTriacLow)
            {   // 1000W ���͸� ��ǥ ����ġ ���ر����� ����
                gu8TriacActDutyData = gu8TriacActDutyData - 1;
            }
        }

        // ������ �ִ� �������� ����ǵ��� ����
        mu8StepmotorStatus = Get_Stepmotor_Move_Status(STEPMOTOR_ID_1); // ���Ǹ��� ������ ���� Ȯ��
        mu16FlowHz = Get_Flowmeter_Hz(FLOWMETER_1_ID);          // ����(Hz) ���� Ȯ��

        if (mu8StepmotorStatus == FALSE)
        {   // Stepmotor�� �̵����� �ƴ� ���
            if (mu16FlowHz < mu16TagetFlowHzHigh)
            {   // ������ ����ġ���� ���� ���
                FlowControl(OPEN, mu8ControlFlow);  // ������ �ø�
            }
            else
            {   // ������ ����ġ���� ���� ���
                FlowControl(CLOSE, mu8ControlFlow); // ������ ����
            }
        }
    }
    else
    {   // ��ǥ�µ� ���� ����
        if ( (mf32HotOutTemp >= 99.0f) || (mf32HotBodyTemp >= 99.0f) )
        {     // ��� �µ� �Ǵ� ���� ���� �µ��� 99���� ���� ��
            gu8RelayOnData = OFF;               // Relay OFF
            gu8TriacActDutyData = 100;          // Triac 100% Duty
        }
        else
        {
            gu8RelayOnData = mu8HeaterRelayOn;

            if (gu8TriacActDutyData < mu8TagetTriacHigh)
            {   // 1000W ���͸� ��ǥ ����ġ ���ر����� �ø�
              gu8TriacActDutyData = gu8TriacActDutyData + 1;
            }
        }

        // ������ ���� �������� ����ǵ��� ����
        mu8StepmotorStatus = Get_Stepmotor_Move_Status(STEPMOTOR_ID_1); // ���Ǹ��� ������ ���� Ȯ��
        mu16FlowHz = Get_Flowmeter_Hz(FLOWMETER_1_ID);          // ����(Hz) ���� Ȯ��

        if (mu8StepmotorStatus == FALSE)
        {   // Stepmotor�� �̵����� �ƴ� ���
            if (mu16FlowHz < mu16TagetFlowHzLow)
            {   // ������ ����ġ���� ���� ���
                FlowControl(OPEN, mu8ControlFlow);  // ������ �ø�
            }
            else
            {   // ������ ����ġ���� ���� ���
                FlowControl(CLOSE, mu8ControlFlow); // ������ ����
            }
        }
    }

    Set_Inst_HeaterControl(gu8RelayOnData, gu8TriacActDutyData, 0);
}


/// @brief      �����¼� ���� ���� OFF
/// @details    �����¼� ���� ���� OFF ������ �Ѵ�
/// @param      void
/// @return     void
void InstHeaterOff(void)
{
    gu8RelayOnData = OFF;
    gu8TriacActDutyData = 0;

    Set_Inst_HeaterControl(gu8RelayOnData, gu8TriacActDutyData, 0);
}



/// @brief      �����¼� ���� ��� ���� �Է� ���� üũ
/// @details    �����¼� ���� ��� ���� �Է� ������ üũ�Ͽ� �� �ܰ踦 �����Ѵ�
/// @param      void
/// @return     void
void InstHeater_V_Detect(void)
{
    U8 mu8HeaterStatus = 0;
    U8 mu8TriacDutyStatus = 0;
    U8 mu8AD_V_Detect = 0;

    mu8HeaterStatus = Get_Inst_Heater_Status();
    mu8TriacDutyStatus = Get_Inst_HeaterTriacDuty();

    if ( (mu8HeaterStatus == INST_HEATER_ALL_ON) && (mu8TriacDutyStatus == 100) )   // ���� �ΰ��� �� Full Heating�� ��쿡�� üũ
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


/// @brief      �����¼� ���� ��� ���� �Է� ���� �ܰ� ����
/// @details    �����¼� ���� ��� ���� �Է� ���� �ܰ踦 �����Ѵ�
/// @param      mu8Data : �����¼� ���� ��� ���� �Է� ���� �ܰ� - enum���� ������ ����Ʈ ����
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


/// @brief      �����¼� ���� ��� ���� �Է� ���� �ܰ� Ȯ��
/// @details    �����¼� ���� ��� ���� �Է� ���� �ܰ踦 Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �����¼� ���� ��� ���� �Է� ���� �ܰ� - enum���� ������ ����Ʈ ����
U8 Get_InstHeater_V_Detect(void)
{
    return  gu8InstHeater_V_Detect;
}


/// @brief      �����¼� ���� ��� ���� ���� �¼� ���� �ܰ� ���� ����
/// @details    �����¼� ���� ��� ���� ���� �¼� ���� �ܰ踦  �����Ѵ�
/// @param      mu8Data : �����¼� ���� ��� ���� ���� ���� �ܰ� - enum���� ������ ����Ʈ ����
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


/// @brief      �����¼� ���� ��� ���� ���� �¼� ���� �ܰ� Ȯ��
/// @details    �����¼� ���� ��� ���� ���� �¼� ���� �ܰ踦 Ȯ���Ͽ� ��ȯ�Ѵ�
/// @param      void
/// @return     return : �����¼� ���� ��� ���� ���� ���� �ܰ� - enum���� ������ ����Ʈ ����
U8 Get_InstHeater_ControlStep(void)
{
    return  gu8InstHeatingStep;
}


/// @brief      �����¼� ���� ���� �Լ�(@While ��)
/// @details    �����¼� ���� ���� ������ �Ѵ�.
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
        InstHeater_V_Detect();      // �Է� ���� �ǽð����� ����

        if (gu8InstHeaterPeriodTimer >= INST_HEAT_CONTROL_TIME_PERIOD)
        {   // ���� �ð� ���� ���� ����
            gu8InstHeaterPeriodTimer = 0;

            if (gu8InstHeatingStep == INST_HEATING_STEP_ID_PREHEAT)
            {   // ���� �ܰ�
                InstHeaterControl_Preheat();
            }
            else if (gu8InstHeatingStep == INST_HEATING_STEP_ID_EFFLUENT)
            {   // ���� �ܰ�
                InstHeaterControl_Effluent();
            }
            else
            {   // �̵��� �ܰ�
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
/// @details    �Ϲ� ���� ���� ����� �������� �ʱ�ȭ ��Ų��
/// @param      void
/// @return     void
void Lib_InstHeater_Module_Initialize(void)
{
#if (INSTANT_HEATER_USE == USE)
    Lib_InstHeater_Initialize();
#endif
}


/// @brief      Lib Instantaneous Heater Module in 1ms Interrupt
/// @details    1ms Interrupt�� �߰��� Heater ���� ��� Library
/// @param      void
/// @return     void
void Lib_InstHeater_Module_1ms_Control(void)
{
#if (INSTANT_HEATER_USE == USE)
    InstHeater_ControlTimer();
#endif
}


/// @brief      Lib Instantaneous Heater Module in While Loop
/// @details    Main �Լ� ���� While Loop �ȿ� �߰��� Heater ���� ��� Library
/// @param      void
/// @return     void
void Lib_InstHeater_Module_Control(void)
{
#if (INSTANT_HEATER_USE == USE)
    Lib_InstHeater_Control();
#endif
}

