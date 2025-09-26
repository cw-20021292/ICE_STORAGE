/// @file     Lib_Water_InstantaneousHeater.h
/// @date     2025/07/03
/// @author   Jaejin Ham
/// @brief    ���� �¼� Heater ���� ���� file

#ifndef _LIB_INSTANTANEOUS_HEATER_H_
#define _LIB_INSTANTANEOUS_HEATER_H_


/*  Lib Function ����
    => ga16Hot_InputUnder15[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX], ga16Hot_InputUnder25[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX],
       ga16Hot_InputUp25[HOT_SELECT_ID_MAX][V_DETECT_ID_MAX] ���̺��� �������� ���� SW ���� ��⿡�� �˾Ƽ� ������
       ���� �µ� �ܰ� �߰��� �迭 ���� �ʿ�

    => Set_InstHeater_HotSelect(mu8Select);                 // ���� �¼� �µ� ���� ���� ����
    => Get_InstHeater_HotSelect();                          // ���� �¼� �µ� ���� ���� Ȯ��

    => Get_InstHeater_V_Detect();                           // ���� �¼� ���� ���� ���� ���� Ȯ��
*/


#if (INSTANT_HEATER_USE == USE)


/// @brief      �¼� ���� �µ� ���̺�
/// @details    �¼� �� ������ �� �ִ� �µ��� ������ ������ ����Ʈ�� �����Ѵ�
typedef enum
{
    HOT_SELECT_ID_45 = 0,                   // 0 : 45��
    HOT_SELECT_ID_70,                       // 1 : 70��
    HOT_SELECT_ID_85,                       // 2 : 85��
    HOT_SELECT_ID_100,                      // 3 : 100��
    HOT_SELECT_ID_MAX                       // Max
} EHotSelectTable_T;


/// @brief      �Է� AC ���� ���� ���� ���̺�
/// @details    �Է� AC ���� ���� ���°��� ���� ����Ʈ�� �����Ѵ�
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


/// @brief      ���� �¼� ���� ���� �ܰ�
/// @details    ���� �¼� ���� ���� �ܰ踦 ���� ����Ʈ�� �����Ѵ�
typedef enum
{
    INST_HEATING_STEP_ID_NONE = 0,          // 0 : ���� ����
    INST_HEATING_STEP_ID_PREHEAT,           // 1 : ����
    INST_HEATING_STEP_ID_EFFLUENT,          // 2 : ������ ����
    INST_HEATING_STEP_ID_MAX                // Max
} EHeatingStepTable_T;

#define INST_HEAT_CONTROL_TIME_PERIOD                   100         // �����¼� ���� ���� �ֱ� Ÿ�̸� 100ms@1ms

#define INST_HEATER_INPUT_V_DETECT_STD_190V             840         // �����¼� ���� �Է� ���� 190V ���� ���� AD��
#define INST_HEATER_INPUT_V_DETECT_STD_200V             870         // �����¼� ���� �Է� ���� 200V ���� ���� AD��
#define INST_HEATER_INPUT_V_DETECT_STD_210V             920         // �����¼� ���� �Է� ���� 210V ���� ���� AD��
#define INST_HEATER_INPUT_V_DETECT_STD_220V             960         // �����¼� ���� �Է� ���� 220V ���� ���� AD��
#define INST_HEATER_INPUT_V_DETECT_STD_230V             1000        // �����¼� ���� �Է� ���� 230V ���� ���� AD��(240V�� 230V ���ذ� �ʰ��� �Ǵ�)


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

