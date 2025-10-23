/// @file     Lib_Water_Cool_TEM.h
/// @date     2025/05/29
/// @author   Jaejin Ham
/// @brief    ������� �ð� ���� ���� file

#ifndef _LIB_WATER_COOL_TEM_H_
#define _LIB_WATER_COOL_TEM_H_



/*  Lib Function ����
    => gau16TEM_TDR_DataTable[TEM_V_ID_MAX] ���̺��� TEM PWM ����� �������ָ� SW ���� ����� �˾Ƽ� ������
    => Set_ColdFuncStatus(mu8Status);                       // �ü� ��� ���� ���� ���� : 0(OFF), 1(ON)
    => Get_ColdFuncStatus();                                // �ü� ��� ���� ���� Ȯ�� : 0(OFF), 1(ON)
    => Set_ColdStorng(mu8Status);                           // �ü� ��� ���� ���� ���� ���� : 0(��), 1(��)
    => Get_ColdStrong();                                    // �ü� ��� ���� ���� ���� Ȯ�� : 0(��), 1(��)
*/


#if (TEM_USE == USE)                // ������� �ð� ����


/// @brief      TEM �ð� ���� ��� ����Ʈ
/// @details    TEM �ð� ���� ��带 ���� ����Ʈ�� �����Ѵ�
typedef enum
{
    TEM_MODE_ID_OFF = 0,        // 0 : TEM PWM OFF
    TEM_MODE_ID_RAPID,          // 1 : �޼� ���
    TEM_MODE_ID_RETAIN,         // 2 : ���� ���
    TEM_MODE_ID_MIN,            // 3 : �ּ� ���
    TEM_MODE_ID_ECO,            // 4 : ECO ���
    TEM_MODE_ID_ECO_9V,         // 5 : ECO 9V ���
    TEM_MODE_ID_MAX             // Max
} ETemID_T;


// TEM ���� Volatae ID
typedef enum
{
    TEM_V_ID_0V = 0,            // 0 :
    TEM_V_ID_1V,                // 1 :
    TEM_V_ID_2V,                // 2 :
    TEM_V_ID_3V,                // 3 :
    TEM_V_ID_4V,                // 4 :
    TEM_V_ID_5V,                // 5 :
    TEM_V_ID_6V,                // 6 :
    TEM_V_ID_7V,                // 7 :
    TEM_V_ID_8V,                // 8 :
    TEM_V_ID_9V,                // 9 :
    TEM_V_ID_10V,               // 10 :
    TEM_V_ID_11V,               // 11 :
    TEM_V_ID_12V,               // 12 :
    TEM_V_ID_13V,               // 13 :
    TEM_V_ID_14V,               // 14 :
    TEM_V_ID_15V,               // 15 :
    TEM_V_ID_16V,               // 16 :
    TEM_V_ID_17V,               // 17 :
    TEM_V_ID_18V,               // 18 :
    TEM_V_ID_19V,               // 19 :
    TEM_V_ID_20V,               // 20 :
    TEM_V_ID_21V,               // 21 :
    TEM_V_ID_22V,               // 22 :
    TEM_V_ID_23V,               // 23 :
    TEM_V_ID_24V,               // 24 :
    TEM_V_ID_MAX                // Max
} ETEM_V_ID_T;

#define LIB_TEM_CONTROL_TIME_PERIOD                         100     // TEM ���� �ֱ� 100ms@1ms

#define TEM_RAPID_OFF_DELAY_TIME                            600     // �޼Ӹ�忡�� TEN OFF�� ��⵿ ���� delay �ð� 10min = 600sec@1sec
#define TEM_MODE_CHANGE_CONFIRM_TIME                        3       // ��� ��ȯ�� ��� ��ȯ Ȯ�� üũ �ð� 300ms@100ms
#define TEM_RAPID_MODE_MAX_TIME                             21600   // �޼� ��� ���� �ִ� �ð� 6hour = 360min = 21600sec@1sec

/***************�ü� �����*******************/
#define RAPID_OPERATE_DEGREE                                7.2f    // �޼Ӹ�� ���� �µ�(2024.11.26 ����ȿ�� ��û���� 7.5�� -> 7.2�� ����)
#define RETAIN_JOIN_DEGREE                                  3.5f    // �޼Ӹ�� �� ������� ���� �µ�
#define RAPID_COMPLETE_DEGREE                               6.5f    // �޼Ӹ�� �Ϸ� �µ�
#define COLD_COMPLETE_DEGREE                                2.5f    // �ð� �Ϸ� �µ�
#define	COLD_REOPERATE_DEGREE		                        2.5f    // �ð� ���� �� �ð� �簡�� �µ�

#define TEM_COOLING_RAPID_END_CONDITION_1_TEMP              33.0f    // �޼� �ð� �߰� �⵿ �ð� ������ ���� �ܰ� ����1 ���� �ܱ� �µ�
#define TEM_COOLING_RAPID_END_CONDITION_2_TEMP              30.0f    // �޼� �ð� �߰� �⵿ �ð� ������ ���� �ܰ� ����2 ���� �ܱ� �µ�
#define TEM_COOLING_RAPID_END_CONDITION_3_TEMP              20.0f    // �޼� �ð� �߰� �⵿ �ð� ������ ���� �ܰ� ����3 ���� �ܱ� �µ�
#define TEM_COOLING_RAPID_END_CONDITION_4_TEMP              12.0f    // �޼� �ð� �߰� �⵿ �ð� ������ ���� �ܰ� ����4 ���� �ܱ� �µ�(�� �µ� �̸��� ����5 ����)

#define TEM_COOLING_RAPID_END_CONDITION_1_TIME              4500    // �ܱ� 33�� �̻�� �޼� �ð� �߰� �⵿ �ð� 4500sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_2_TIME              3000    // �ܱ� 30�� �̻�� �޼� �ð� �߰� �⵿ �ð� 3000sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_3_TIME              780     // �ܱ� 20�� �̻�� �޼� �ð� �߰� �⵿ �ð� 780sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_4_TIME              180     // �ܱ� 12�� �̻�� �޼� �ð� �߰� �⵿ �ð� 180sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_5_TIME              200     // �ܱ� 12�� �̸��� �޼� �ð� �߰� �⵿ �ð� 200sec@1sec

#define TEM_COOLING_RAPID_END_CONDITION_1_V                 TEM_V_ID_24V    // �ܱ� 33�� �̻�� �޼� �ð� ���� ���� 24V
#define TEM_COOLING_RAPID_END_CONDITION_2_V                 TEM_V_ID_24V    // �ܱ� 30�� �̻�� �޼� �ð� ���� ���� 24V
#define TEM_COOLING_RAPID_END_CONDITION_3_V                 TEM_V_ID_24V    // �ܱ� 20�� �̻�� �޼� �ð� ���� ���� 24V
#define TEM_COOLING_RAPID_END_CONDITION_4_V                 TEM_V_ID_24V    // �ܱ� 12�� �̻�� �޼� �ð� ���� ���� 24V
#define TEM_COOLING_RAPID_END_CONDITION_5_V                 TEM_V_ID_12V    // �ܱ� 12�� �̸��� �޼� �ð� ���� ���� 12V

/***************�ü� ����*******************/
#define RAPID_OPERATE_DEGREE_LOW_MOE                        13.0f   // �޼Ӹ�� ���� �µ�
#define RETAIN_JOIN_DEGREE_LOW_MODE                         9.0f    // �޼Ӹ�� �� ������� ���� �µ�
#define RAPID_COMPLETE_DEGREE_LOW_MODE                      12.0f   // �޼Ӹ�� �Ϸ� �µ�
#define COLD_COMPLETE_DEGREE_LOW_MODE                       4.0f    // �ð� �Ϸ� �µ�
#define	COLD_REOPERATE_DEGREE_LOW_MODE                      8.0f    // �ð� ���� �� �ð� �簡�� �µ�

#define TEM_COOLING_RAPID_END_CONDITION_1_TEMP_LOW_MODE     33.0f    // �޼� �ð� �߰� �⵿ �ð� ������ ���� �ܰ� ����1 ���� �ܱ� �µ�
#define TEM_COOLING_RAPID_END_CONDITION_2_TEMP_LOW_MODE     30.0f    // �޼� �ð� �߰� �⵿ �ð� ������ ���� �ܰ� ����2 ���� �ܱ� �µ�
#define TEM_COOLING_RAPID_END_CONDITION_3_TEMP_LOW_MODE     20.0f    // �޼� �ð� �߰� �⵿ �ð� ������ ���� �ܰ� ����3 ���� �ܱ� �µ�
#define TEM_COOLING_RAPID_END_CONDITION_4_TEMP_LOW_MODE     12.0f    // �޼� �ð� �߰� �⵿ �ð� ������ ���� �ܰ� ����4 ���� �ܱ� �µ�(�� �µ� �̸��� ����5 ����)

#define TEM_COOLING_RAPID_END_CONDITION_1_TIME_LOW_MODE     1800    // �ܱ� 33�� �̻�� �޼� �ð� �߰� �⵿ �ð� 1800sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_2_TIME_LOW_MODE     1200    // �ܱ� 30�� �̻�� �޼� �ð� �߰� �⵿ �ð� 1200sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_3_TIME_LOW_MODE     780     // �ܱ� 20�� �̻�� �޼� �ð� �߰� �⵿ �ð� 780sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_4_TIME_LOW_MODE     400     // �ܱ� 12�� �̻�� �޼� �ð� �߰� �⵿ �ð� 400sec@1sec
#define TEM_COOLING_RAPID_END_CONDITION_5_TIME_LOW_MODE     100     // �ܱ� 12�� �̸��� �޼� �ð� �߰� �⵿ �ð� 100sec@1sec

#define TEM_COOLING_RAPID_END_CONDITION_1_V_LOW_MODE        TEM_V_ID_24V    // �ܱ� 33�� �̻�� �޼� �ð� ���� ���� 24V
#define TEM_COOLING_RAPID_END_CONDITION_2_V_LOW_MODE        TEM_V_ID_24V    // �ܱ� 30�� �̻�� �޼� �ð� ���� ���� 24V
#define TEM_COOLING_RAPID_END_CONDITION_3_V_LOW_MODE        TEM_V_ID_24V    // �ܱ� 20�� �̻�� �޼� �ð� ���� ���� 24V
#define TEM_COOLING_RAPID_END_CONDITION_4_V_LOW_MODE        TEM_V_ID_24V    // �ܱ� 12�� �̻�� �޼� �ð� ���� ���� 24V
#define TEM_COOLING_RAPID_END_CONDITION_5_V_LOW_MODE        TEM_V_ID_12V    // �ܱ� 12�� �̸��� �޼� �ð� ���� ���� 12V

#define TEM_RAPID_MODE_IN_12V_TIME                          3000    // �޼� ���� �������� �� �ʱ� 12V ������ �����ϴ� �ð� 5min = 300sec@100ms

void Lib_TEM_Initialize(void);
void Lib_TEM_ControlTimer(void);
void Set_ColdFuncStatus(U8 mu8Status);
U8 Get_ColdFuncStatus(void);
void Set_ColdStorng(U8 mu8Status);
U8 Get_ColdStrong(void);
U8 Get_TEM_ActMode(void);
void Lib_TEM_Control(void);

#endif      // ������� �ð� ���� END

void Lib_TEM_Module_Initialize(void);
void Lib_TEM_Module_1ms_Control(void);
void Lib_TEM_Module_Control(void);

#endif
