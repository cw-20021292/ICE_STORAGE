/// @file     Hal_IceSwingBar.h
/// @date     2025/07/29
/// @author   Jaejin Ham
/// @brief    ���� ������ Control file


#ifndef _HAL_ICE_SWING_BAR_H_
#define _HAL_ICE_SWING_BAR_H_


/*  ���� ������ ���� Function ����
    => Set_SwingBar_Operation(mu8Status);       // ���� Swingbar �ڵ� ���� �Լ�(�ѹ� �����ϸ� �˾Ƽ� ������ ���� ��) : mu8Status - 0(�̵���), 1(����)
                                                // ��, ���� ���� �Ұ� ����, E61 �߻� �ÿ��� ���� ���� ���¶� ���� ����
    => Get_SwingBar_Operation();                // ������ ������ ���� ���θ� Ȯ�� - 0(�̵���), 1(����)

    ��� ��)
    Set_SwingBar_Operation(1);                  // ���� Swing bar �ݺ� ���� ����
    Set_SwingBar_Operation(0);                  // ���� Swing bar �ݺ� ���� ����
    u8Return = Get_SwingBar_Operation();        // ������ ������ ���� ���θ� Ȯ�� - 0(�̵���), 1(����)
*/

#define ICE_SWING_BAR_USE                   USE         // ���� Swing bar ��� ���� : 0(No Use), 1(Use)

#if (ICE_SWING_BAR_USE == USE)

#define SWING_BAR_ON_TIME                   2           // ���ڼ� ON ���� 200ms@100ms
#define SWING_BAR_OFF_TIME                  6           // ���ڼ� OFF ���� 600ms@100ms

#define P_SWING_BAR                         P8_bit.no1
#define TURN_ON_SWING_BAR()                 { P_SWING_BAR = 1; }
#define TURN_OFF_SWING_BAR()                { P_SWING_BAR = 0; }
#define GET_STATUS_SWING_BAR()              ( P_SWING_BAR )

#define SWING_BAR_CONTROL_TIME_PERIOD       100         // ������ ���� ���� �ֱ� 100ms@1ms

void Hal_SwingBar_Initialize(void);
void SwingBarControlTimer(void);
void Hal_SwingBarControl(void);
void Set_SwingBar_Operation(U8 mu8Status);
U8 Get_SwingBar_Operation(void);

#endif

void Hal_SwingBar_Module_Initialize(void);
void Hal_SwingBar_Module_1ms_Control(void);
void Hal_SwingBar_Module_Control(void);

#endif

