/// @file     Lib_Water_Cool_BLDCComp.h
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    �ð� ���� ���� file

#ifndef _LIB_WATER_COOL_BLDC_COMP_H_
#define _LIB_WATER_COOL_BLDC_COMP_H_


/*  Lib Function ����
    => SCoolData[POWERSAVE_ID_MAX][COOL_STEP_ID_MAX] ���̺��� ����� �������ָ� SW ���� ����� �˾Ƽ� ������
    => Set_ColdFuncStatus(mu8Status);                       // �ü� ��� ���� ���� ���� : 0(OFF), 1(ON)
    => Get_ColdFuncStatus();                                // �ü� ��� ���� ���� Ȯ�� : 0(OFF), 1(ON)
    => Set_ColdStorng(mu8Status);                           // �ü� ��� ���� ���� ���� ���� : 0(��), 1(��)
    =>  Get_ColdStrong();                                   // �ü� ��� ���� ���� ���� Ȯ�� : 0(��), 1(��)
*/


#if (BLDC_COMP_USE == USE)              // BLDC Comp ����


/// @brief      �ܱ� �µ��� ���� �ð� ���� �ܰ� ����Ʈ
/// @details    �ܱ� �µ� ���ǿ� ���� �ð� ���� �ܰ踦 ���� ����Ʈ�� �����Ѵ�
typedef enum
{
    COOL_STEP_ID_AIR_TEMP_ERROR = 0,        // 0 : �ܱ� �µ� ���� �߻���
    COOL_STEP_ID_STEP_1,                    // 1 : �ܱ� �µ��� ���� �ð� ���� 1�ܰ�
    COOL_STEP_ID_STEP_2,                    // 2 : �ܱ� �µ��� ���� �ð� ���� 2�ܰ�
    COOL_STEP_ID_STEP_3,                    // 3 : �ܱ� �µ��� ���� �ð� ���� 3�ܰ�
    COOL_STEP_ID_STEP_4,                    // 4 : �ܱ� �µ��� ���� �ð� ���� 2�ܰ�
    COOL_STEP_ID_MAX                        // Max
} ECoolStepID_T;

#define BLDC_COMP_CONTROL_STEP1_ENV_TEMP            10.0f           // �ð� ���� ���� 1�ܰ�, 2�ܰ� ���� ���� �ܱ� �µ�
#define BLDC_COMP_CONTROL_STEP2_ENV_TEMP            20.0f           // �ð� ���� ���� 2�ܰ�, 3�ܰ� ���� ���� �ܱ� �µ�
#define BLDC_COMP_CONTROL_STEP3_ENV_TEMP            30.0f           // �ð� ���� ���� 3�ܰ�, 4�ܰ� ���� ���� �ܱ� �µ�

#define BLDC_COMP_FIRST_ACT_CHECK_COLD_TEMP         20.0f           // �ð� ���� ���۽� �ʱ� �⵿ ���� �Ǵ� ���� �ü� �µ�

#define BLDC_COMP_CONTROL_TIME_PERIOD               100             // �ð� ���� �ð� �ֱ� : 100ms(@1ms)

#define COOL_HEAT_SIMULTANEOUS_ON_DELAY_TIME        10              // �ð���� ���Ͱ� ���ÿ� �⵿ ������ �� �ִ� ���ǿ� ���� ON �� Delay �ð� 1sec@100ms

#define DELAY_TIME_BEFORE_COMP_ON                   300             // Comp ���� �� ���� �ð� 5min = 300sec@1sec
#define COMP_ON_MAX_TIME                            9000            // Comp ���� �ִ� �ð� 150min = 9000sec@1sec

#define COMP_OFF_CHECK_MAX_TIME                     1440            // �ð� OFF ���·� ������ üũ �ִ� �ð� 24Hour = 1440min@1min


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

#endif      // BLDC Comp ���� END

void Lib_BLDC_Comp_Module_Initialize(void);
void Lib_BLDC_Comp_Module_1ms_Control(void);
void Lib_BLDC_Comp_Module_Control(void);

#endif

