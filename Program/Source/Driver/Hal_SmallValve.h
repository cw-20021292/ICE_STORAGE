/// @file     Hal_SmallValve.h
/// @date     2025/03/28
/// @author   Jaejin Ham
/// @brief    ���� ��� ���� file


#ifndef _HAL_SMALL_VALVE_H_
#define _HAL_SMALL_VALVE_H_


/*  ���� ���(���� ���� ���� ���) ���� Function ����
    => Set_SmallValveControl(Valve ID, Open/Close data, Delay Time, Change Voltage Delay Time);

    ��� ��)
    Set_SmallValveControl(ValveID, OPEN, 0, 0);     // ValveID �̸��� ������긦 0�� ������ �� OPEN��Ű�� ��� ���� ���� ON
    Set_SmallValveControl(ValveID, OPEN, 5, 0);     // ValveID �̸��� ������긦 500ms ������ �� OPEN��Ű�� ��� ���� ���� ON
    Set_SmallValveControl(ValveID, OPEN, 0, 30);    // ValveID �̸��� ������긦 0�� ������ �� OPEN��Ű�� 3�� �� ���� ���� ON
    Set_SmallValveControl(ValveID, OPEN, 5, 30);    // ValveID �̸��� ������긦 500ms ������ �� OPEN��Ű�� 3�� �� ���� ���� ON

    Set_SmallValveControl(ValveID, CLOSE, 0, 0);    // ValveID �̸��� ������긦 0�� ������ �� CLOSE��Ű�� ��� ���� ���� OFF
    Set_SmallValveControl(ValveID, CLOSE, 5, 0);    // ValveID �̸��� ������긦 500ms ������ �� CLOSE��Ű�� ��� ���� ����OFF

    Hal_AllSmallValve_Close();                      // ��� ���� Valve ��� Close
*/


// ���� ���(���� ���� ����) �ִ� 2�� ��� ����
typedef enum
{
    SMALL_VALVE_ID_NONE = 0,                            // 0 :
    SMALL_VALVE_ID_FAUCET_OUT,                          // 1 :
    SMALL_VALVE_ID_MAX                                  // 3 Max, 3�� ������ �ȵ�
} ESmallValveID_T;

#define SMALL_VALVE_COUNT                               0       // ���� ��� ���� - MAX 2

// ���� ��� port ����
// ������ skip���� ��ġ�Ͽ� ����ϸ�, �̻�� port�� 0���� ó��
// ������ ID�� �����ϰ� ��ġ
// ���� ���
#if (SMALL_VALVE_COUNT >= 1)
#define PORT_DEFINE_SMALL_VALVE_1                       0
#define PORT_DEFINE_SMALL_VALVE_1_TRIG                  0
#endif

#if (SMALL_VALVE_COUNT >= 2)
#define PORT_DEFINE_SMALL_VALVE_2                       0
#define PORT_DEFINE_SMALL_VALVE_2_TRIG                  0
#endif


#if (SMALL_VALVE_COUNT > 0)

#define SMALL_VALVE_ID_MAX_COUNT            SMALL_VALVE_ID_MAX

// Valve Port Define
#define P_SMALL_VALVE_1                     PORT_DEFINE_SMALL_VALVE_1
#define TURN_ON_SMALL_VALVE_1()             { P_SMALL_VALVE_1 = 1; }
#define TURN_OFF_SMALL_VALVE_1()            { P_SMALL_VALVE_1 = 0; }
#define GET_STATUS_SMALL_VALVE_1()          ( P_SMALL_VALVE_1 )

#define P_SMALL_VALVE_1_TRIG                PORT_DEFINE_SMALL_VALVE_1_TRIG
#define TURN_ON_SMALL_VALVE_1_TRIG()        { P_SMALL_VALVE_1_TRIG = 1; }
#define TURN_OFF_SMALL_VALVE_1_TRIG()       { P_SMALL_VALVE_1_TRIG = 0; }
#define GET_STATUS_SMALL_VALVE_1_TRIG()     ( P_SMALL_VALVE_1_TRIG )

#define P_SMALL_VALVE_2                     PORT_DEFINE_SMALL_VALVE_2
#define TURN_ON_SMALL_VALVE_2()             { P_SMALL_VALVE_2 = 1; }
#define TURN_OFF_SMALL_VALVE_2()            { P_SMALL_VALVE_2 = 0; }
#define GET_STATUS_SMALL_VALVE_2()          ( P_SMALL_VALVE_2 )

#define P_SMALL_VALVE_2_TRIG                PORT_DEFINE_SMALL_VALVE_2_TRIG
#define TURN_ON_SMALL_VALVE_2_TRIG()        { P_SMALL_VALVE_2_TRIG = 1; }
#define TURN_OFF_SMALL_VALVE_2_TRIG()       { P_SMALL_VALVE_2_TRIG = 0; }
#define GET_STATUS_SMALL_VALVE_2_TRIG()     ( P_SMALL_VALVE_2_TRIG )

#define SMALL_VALVE_CONTROL_TIME_PERIOD     100         // 100ms@1ms

void Hal_SmallValve_Initialize(void);
void SmallValveControlTimer(void);
void SmallValveControl(void);
void Set_SmallValveControl(U8 mu8ValveID, U8 mu8Status, U8 mu8Delay1, U8 mu8Delay2);
void Hal_AllSmallValve_Close(void);
void Hal_SmallValveControl(U8 mu8ValveID);

#endif

void Hal_SmallValve_Module_Initialize(void);
void Hal_SmallValve_Module_1ms_Control(void);
void Hal_SmallValve_Module_Control(void);

#endif

