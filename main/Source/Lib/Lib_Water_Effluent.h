/// @file   Lib_Water_Effluent.h
/// @date   2025/06/26
/// @author Jaejin Ham
/// @brief  ���� ���� ��� �ְ� ��� ����

#ifndef _LIB_WATER_EFFLUENT_H_
#define _LIB_WATER_EFFLUENT_H_


/*  Lib Function ����
    => Set_WaterSelect(mu8ID);                  // �� ���� ���� ����
    => Get_WaterSelect();                       // ���õ� �� ���� Ȯ��
    => Set_DefaultWaterSelect(mu8ID);           // �⺻ ���� �� ���� ����
    => Get_DefaultWaterSelect();                // �⺻ ���� �� ���� ���� ���� Ȯ��

    => Set_WaterAmount(mu8ID);                  // ���� ���� ����
    => Get_WaterAmount();                       // ���õ� ���� Ȯ��
    => Set_DefaultWaterAmount(mu8ID);           // �⺻ ���� ���� ����
    => Get_DefaultWaterAmount();                // �⺻ ���� ���� ���� ���� Ȯ��

    => Set_MyCupAmount(mu16Data);               // ������ �뷮 ����(130ml ~ 1000ml)
    => Get_MyCupAmount();                       // ������ �뷮 ���� ���� Ȯ��

    => Set_EffluentTime(mu8ID, mu16Data);       // ���� ������ �� �� ������ ���� �ð��� ����
    => Set_EffluentHz(mu8ID, mu16Data);         // ���� ������ �� �� ������ ���� Hz�� ����

    => Set_EffluentStatus(mu8ID, mu8Data);      // ������ �� ������ ���� ���� ����(0(������), 1(����)) ����

    => Set_EffluentStep(mu8ID, mu8Step);        // ���� ���� �� ���� ���� Step ����
    => Get_EffluentStep(mu8ID);                 // ���� ���� ���� ���� ���� ���� Step Ȯ��
*/


#define EFFLUENT_CONTROL_PERIOD_TIME                        100     // ���� ���� ���� �ֱ� Ÿ�̸� 100ms@1ms

#define EFFLUENT_FLOWMETER_USE                              NO_USE     // ���� ���ۿ� ���� ���� ��� ���� - 0(�̻��), 1(���)


/// @brief      �� ���� ���°�
/// @details    �� ���� ���°��� ���� ����Ʈ�� �����Ѵ�
typedef enum
{
    EFFLUENT_ID_ROOM = 0,                   // 0 : ����
    EFFLUENT_ID_COLD,                       // 1 : �ü�
    EFFLUENT_ID_HOT,                        // 2 : �¼�
    EFFLUENT_ID_MAX                         // Max
} EWaterSelec_T;


/// @brief      ���� ���� ���°�
/// @details    ���� ���� ���°��� ���� ����Ʈ�� �����Ѵ�
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

