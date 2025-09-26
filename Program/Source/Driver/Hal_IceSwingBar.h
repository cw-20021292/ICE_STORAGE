/// @file     Hal_IceSwingBar.h
/// @date     2025/07/29
/// @author   Jaejin Ham
/// @brief    제빙 스윙바 Control file


#ifndef _HAL_ICE_SWING_BAR_H_
#define _HAL_ICE_SWING_BAR_H_


/*  얼음 스윙바 제어 Function 사용법
    => Set_SwingBar_Operation(mu8Status);       // 제빙 Swingbar 자동 동작 함수(한번 설정하면 알아서 스윙바 동작 함) : mu8Status - 0(미동작), 1(동작)
                                                // 단, 제빙 동작 불가 상태, E61 발생 시에는 동작 설정 상태라도 동작 안함
    => Get_SwingBar_Operation();                // 현재의 스윙바 동작 여부를 확인 - 0(미동작), 1(동작)

    사용 예)
    Set_SwingBar_Operation(1);                  // 제빙 Swing bar 반복 동작 시작
    Set_SwingBar_Operation(0);                  // 제빙 Swing bar 반복 동작 정지
    u8Return = Get_SwingBar_Operation();        // 현재의 스윙바 동작 여부를 확인 - 0(미동작), 1(동작)
*/

#define ICE_SWING_BAR_USE                   USE         // 제빙 Swing bar 사용 여부 : 0(No Use), 1(Use)

#if (ICE_SWING_BAR_USE == USE)

#define SWING_BAR_ON_TIME                   2           // 전자석 ON 동작 200ms@100ms
#define SWING_BAR_OFF_TIME                  6           // 전자석 OFF 동작 600ms@100ms

#define P_SWING_BAR                         P8_bit.no1
#define TURN_ON_SWING_BAR()                 { P_SWING_BAR = 1; }
#define TURN_OFF_SWING_BAR()                { P_SWING_BAR = 0; }
#define GET_STATUS_SWING_BAR()              ( P_SWING_BAR )

#define SWING_BAR_CONTROL_TIME_PERIOD       100         // 스윙바 동작 제어 주기 100ms@1ms

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

