/// @file     Hal_Input_IceTray.h
/// @date     2025/07/29
/// @author   Jaejin Ham
/// @brief    ICE TRAY 제어 관련 헤더 파일

#ifndef _HAL_INPUT_ICE_TRAY_H_
#define _HAL_INPUT_ICE_TRAY_H_


/*  Ice Tray 위치 감지 제어 Function 사용법
    => Get_IceTrayPosition();                       // Ice Tray 위치 상태 확인 - 0(얼음받음), 1(얼음버림), 2(이동중), 3(에러)

    사용 예)
    u8Return = Get_IceTrayPosition();
*/

#define ICE_TRAY_INPUT_CHECK_USE                    USE

#if (ICE_TRAY_INPUT_CHECK_USE == USE)

#define INPUT_ICE_TRAY_CONTROL_TIME_PERIOD          100     // Ice Tray 감지 제어 주기 100ms@1ms

#define ICE_TRAY_POSITION_ICE_MAKING                0       // 얼음받음
#define ICE_TRAY_POSITION_ICE_THROW                 1       // 얼음버림
#define ICE_TRAY_POSITION_MOVING                    2       // 이동중
#define ICE_TRAY_POSITION_ERROR                     3       // 에러

#define ICE_TRAY_DELAY_TIME                         5       // 트레이 시간 딜레이 500ms@100ms

// Port Define
#define P_ICE_TRAY_MICRO_SW_HIGH                    P12_bit.no0
#define GET_STATUS_ICE_TRAY_MICRO_SW_HIGH()         ( P_ICE_TRAY_MICRO_SW_HIGH )
#define P_ICE_TRAY_MICRO_SW_LOW                     P12_bit.no1
#define GET_STATUS_ICE_TRAY_MICRO_SW_LOW()          ( P_ICE_TRAY_MICRO_SW_LOW )

void Hal_Input_IceTray_Initialize(void);
void Input_IceTrayControlTimer(void);
void Input_IceTray_Micro_SW(void);
U8 Get_IceTrayPosition(void);

#endif

void Hal_Input_IceTray_Module_Initialize(void);
void Hal_Input_IceTray_Module_1ms_Control(void);
void Hal_Input_IceTray_Module_Control(void);


#endif
