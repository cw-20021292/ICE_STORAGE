/// @file     Hal_NormalComp.h
/// @date     2025/02/18
/// @author   Jaejin Ham
/// @brief    Normal Comp Control file


#ifndef _HAL_NORMAL_COMP_H_
#define _HAL_NORMAL_COMP_H_


// ******************************************************************
//                          일반 Comp 설정
// ******************************************************************
/*  일반 정속형 Comp 제어 Function 사용법
    => Set_Comp(On/Off Data, Delay Time);
    => Get_Comp_Status();                           // Comp 동작 상태 확인 - 1(On), 0(Off)

    사용 예)
    Set_Comp(1, 0);                     // Comp 0초 딜레이 후 On
    Set_Comp(ON, 0);                    // Comp 0초 딜레이 후 On
    Set_Comp(0, 0);                     // Comp 0초 딜레이 후 Off
    Set_Comp(OFF, 0);                   // Comp 0초 딜레이 후 Off
    Set_Comp(1, 5);                     // Comp 500ms 딜레이 후 On
    Set_Comp(ON, 5);                    // Comp 500ms 딜레이 후 On
    Set_Comp(0, 5);                     // Comp 500ms 딜레이 후 Off
    Set_Comp(OFF, 5);                   // Comp 500ms 딜레이 후 Off
    u8Return = Get_Comp_Status();       // mu8Return : 1(ON), 0(OFF)
*/

#define NORMAL_COMP_USE                 NO_USE      // 일반 정속형 Comp 사용 여부 - 0(No Use), 1(Use)

#if (NORMAL_COMP_USE == USE)
// Comp port 할당, 미사용시 0으로 세팅
#define P_COMP_ON                       P0_bit.no6  // 정속형 Comp power on
#define TURN_ON_COMP()                  { P_COMP_ON = 1; }
#define TURN_OFF_COMP()                 { P_COMP_ON = 0; }
#define GET_STATUS_COMP()               ( P_COMP_ON )

#define COMP_CONTROL_TIME_PERIOD        100         // 100ms@1ms

void Hal_Comp_Initialize(void);
void CompControlTimer(void);
void CompControl(void);
void Set_Comp(U8 mu8OnOff, U8 mu8Delay);
U8 Get_Comp_Status(void);
void Hal_Comp_Control(void);

#endif

void Hal_Comp_Module_Initialize(void);
void Hal_Comp_Module_1ms_Control(void);
void Hal_Comp_Module_Control(void);

#endif

