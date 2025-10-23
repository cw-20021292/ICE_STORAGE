/// @file     Hal_AC_Motor_IceMaking.h
/// @date     2025/07/30
/// @author   Jaejin Ham
/// @brief    얼음 기능 관련 AC Motor Control file

#ifndef _LIB_AC_MOTOR_ICE_MAKING_H_
#define _LIB_AC_MOTOR_ICE_MAKING_H_


/*  제빙 관련 AC 모터 제어 Function 사용법
    => Set_IceOutIntervalTime(mu8Data);             // 얼음 추출 시작시 얼음 토출 모터 대기 시간 설정(100ms단위)
    => u8Return = Get_IceOutIntervalTime();         // 얼음 추출 시작시 얼음 토출 모터 대기 시간 확인(100ms단위)

    => Set_IceOutBackStatus(mu8Status);             // 얼음 추출 후 피더 스크류 역동작 상태 Flag 설정 - 0(미동작), 1(동작)
    => u8Return = Get_IceOutBackStatus();           // 얼음 추출 후 피더 스크류 역동작 상태 Flag 확인 - 0(미동작), 1(동작)

    => Set_IceStuckBackStatus(mu8Status);           // 얼음 걸림 해제 동작 상태 여부 설정 - 0(미동작), 1(동작)
    => u8Return = Get_IceStuckBackStatus();         // 얼음 걸림 해제 동작 상태 여부 확인 - 0(미동작), 1(동작)

    => Set_TrayCCW_DelayEnd(mu8Data);               // Tray CCW 동작 지연 완료 여부 설정 - 0(미완료), 1(완료)
    => u8Return = Get_TrayCCW_DelayEnd();           // Tray CCW 동작 지연 완료 여부 확인 - 0(미완료), 1(완료)

    => Set_TrayCW_DelayEnd(mu8Data);                // Tray CW 동작 지연 완료 여부 설정 - 0(미완료), 1(완료)
    => u8Return = Get_TrayCW_DelayEnd();            // Tray CW 동작 지연 완료 여부 확인 - 0(미완료), 1(완료)

    => Set_TrayUpMoveReset(mu8Data);                // Tray Up 초기화 동작 여부 설정 - 0(미동작), 1(동작)
    => u8Return = Get_TrayUpMoveReset();            // Tray Up 초기화 동작 여부 확인 - 0(미동작), 1(동작)

    => Set_TrayDownMoveReset(mu8Data);              // Tray Down 초기화 동작 여부 설정 - 0(미동작), 1(동작)
    => u8Return = Get_TrayDownMoveReset();          // Tray Down 초기화 동작 여부 확인 - 0(미동작), 1(동작)

    => Set_TrayMotorUpStatus(mu8Data);              // Tray Up 동작 여부 설정 - 0(미동작), 1(동작)
    => u8Return = Get_TrayMotorUpStatus();          // Tray Up 동작 여부 확인 - 0(미동작), 1(동작)

    => Set_TrayMotorDownStatus(mu8Data);            // Tray Down 동작 여부 설정 - 0(미동작), 1(동작)
    => u8Return = Get_TrayMotorDownStatus();        // Tray Down 동작 여부 확인 - 0(미동작), 1(동작)

    => Set_TrayMotorPreUpStatus(mu8Data);           // Tray Up 이전 동작 여부 설정 - 0(미동작), 1(동작)
    => u8Return = Get_TrayMotorPreUpStatus();       // Tray Up 이전 동작 여부 확인 - 0(미동작), 1(동작)

    => Set_TrayMotorPreDownStatus(mu8Data);         // Tray Down 이전 동작 여부 설정 - 0(미동작), 1(동작)
    => u8Return = Get_TrayMotorPreDownStatus();     // Tray Down 이전 동작 여부 확인 - 0(미동작), 1(동작)
*/

#define AC_MOTOR_ICE_MAKING_USE                     USE

#if (AC_MOTOR_ICE_MAKING_USE == USE)

#define AC_MOTOR_ICE_MAKING_CONTROL_TIME_PERIOD     100     // 제빙 동작에서 사용되는 AC 모터들의 제어 주기 타이머 100ms@1ms

#define ICETRAY_CW_TIME                             250     // Ice Tray CW 25Sec@100ms
#define ICETRAY_CCW_TIME                            500     // Ice Tray CCW 50Sec@100ms

#define TRAY_MOTOR_STOP                             0
#define TRAY_MOTOR_CW_DIRECTION                     1
#define TRAY_MOTOR_CCW_DIRECTION                    2

#define TRAY_ERROR_STANDBY_TIME                     6000    // 트레이 에러 대기 시간 10min@100ms

#define TRAY_UP_ICE_STUCK_DELAY_TIME                600

#define TRAY_UP_ICE_STUCK_COUNT                     5

/*..hui [23-9-21오후 3:53:57] 제빙 방향 대기시간 90분으로 변경.. 아이콘 아이스 사양..*/
#define TRAY_UP_RESET_DELAY_TIME                    54000

#define TRAY_UP_RETRY_COUNT                         9

#define P_MOTOR_ICE_OUT_CCW                         P9_bit.no5
#define TURN_ON_MOTOR_ICE_OUT_CCW()                 { P_MOTOR_ICE_OUT_CCW = 1; }
#define TURN_OFF_MOTOR_ICE_OUT_CCW()                { P_MOTOR_ICE_OUT_CCW = 0; }
#define GET_STATUS_MOTOR_ICE_OUT_CCW()              ( P_MOTOR_ICE_OUT_CCW )

#define P_MOTOR_ICE_OUT_CW                           P9_bit.no6
#define TURN_ON_MOTOR_ICE_OUT_CW()                  { P_MOTOR_ICE_OUT_CW = 1; }
#define TURN_OFF_MOTOR_ICE_OUT_CW()                 { P_MOTOR_ICE_OUT_CW = 0; }
#define GET_STATUS_MOTOR_ICE_OUT_CW()               ( P_MOTOR_ICE_OUT_CW )

void Hal_AC_Motor_IceMaking_Initialize(void);
void AC_Motor_IceMaking_ControlTimer(void);
void Output_AC_Motor_IceMaking(void);
void Set_IceOutIntervalTime(U8 mu8Data);
U8 Get_IceOutIntervalTime(void);
void Set_IceOutBackStatus(U8 mu8Status);
U8 Get_IceOutBackStatus(void);
void Set_IceStuckBackStatus(U8 mu8Status);
U8 Get_IceStuckBackStatus(void);
void Output_Ice_Feeder_Motor(void);
void Output_Ice_Tray_Motor(void);
void reset_micro_sw_move_err_check_state(void);
void inverse_direction_time_check(void);
void ice_make_system_up_move_reset(void);
void ice_make_system_down_move_reset(void);
void over_ice_making_check(void);
void over_ice_making_melt_proc(void);
void down_tray_motor(void);
void up_tray_motor(void);
void Set_TrayCCW_DelayEnd(U8 mu8Data);
U8 Get_TrayCCW_DelayEnd(void);
void Set_TrayCW_DelayEnd(U8 mu8Data);
U8 Get_TrayCW_DelayEnd(void);
void Set_TrayUpMoveReset(U8 mu8Data);
U8 Get_TrayUpMoveReset(void);
void Set_TrayDownMoveReset(U8 mu8Data);
U8 Get_TrayDownMoveReset(void);
void Set_TrayMotorUpStatus(U8 mu8Data);
U8 Get_TrayMotorUpStatus(void);
void Set_TrayMotorDownStatus(U8 mu8Data);
U8 Get_TrayMotorDownStatus(void);
void Set_TrayMotorPreUpStatus(U8 mu8Data);
U8 Get_TrayMotorPreUpStatus(void);
void Set_TrayMotorPreDownStatus(U8 mu8Data);
U8 Get_TrayMotorPreDownStatus(void);

#endif

void Hal_AC_Motor_IceMaking_Module_Initialize(void);
void Hal_AC_Motor_IceMaking_Module_1ms_Control(void);
void Hal_AC_Motor_IceMaking_Module_Control(void);

#endif
