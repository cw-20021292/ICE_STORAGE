/// @file     Hal_AC_Motor_IceMaking.h
/// @date     2025/07/30
/// @author   Jaejin Ham
/// @brief    ���� ��� ���� AC Motor Control file

#ifndef _LIB_AC_MOTOR_ICE_MAKING_H_
#define _LIB_AC_MOTOR_ICE_MAKING_H_


/*  ���� ���� AC ���� ���� Function ����
    => Set_IceOutIntervalTime(mu8Data);             // ���� ���� ���۽� ���� ���� ���� ��� �ð� ����(100ms����)
    => u8Return = Get_IceOutIntervalTime();         // ���� ���� ���۽� ���� ���� ���� ��� �ð� Ȯ��(100ms����)

    => Set_IceOutBackStatus(mu8Status);             // ���� ���� �� �Ǵ� ��ũ�� ������ ���� Flag ���� - 0(�̵���), 1(����)
    => u8Return = Get_IceOutBackStatus();           // ���� ���� �� �Ǵ� ��ũ�� ������ ���� Flag Ȯ�� - 0(�̵���), 1(����)

    => Set_IceStuckBackStatus(mu8Status);           // ���� �ɸ� ���� ���� ���� ���� ���� - 0(�̵���), 1(����)
    => u8Return = Get_IceStuckBackStatus();         // ���� �ɸ� ���� ���� ���� ���� Ȯ�� - 0(�̵���), 1(����)

    => Set_TrayCCW_DelayEnd(mu8Data);               // Tray CCW ���� ���� �Ϸ� ���� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
    => u8Return = Get_TrayCCW_DelayEnd();           // Tray CCW ���� ���� �Ϸ� ���� Ȯ�� - 0(�̿Ϸ�), 1(�Ϸ�)

    => Set_TrayCW_DelayEnd(mu8Data);                // Tray CW ���� ���� �Ϸ� ���� ���� - 0(�̿Ϸ�), 1(�Ϸ�)
    => u8Return = Get_TrayCW_DelayEnd();            // Tray CW ���� ���� �Ϸ� ���� Ȯ�� - 0(�̿Ϸ�), 1(�Ϸ�)

    => Set_TrayUpMoveReset(mu8Data);                // Tray Up �ʱ�ȭ ���� ���� ���� - 0(�̵���), 1(����)
    => u8Return = Get_TrayUpMoveReset();            // Tray Up �ʱ�ȭ ���� ���� Ȯ�� - 0(�̵���), 1(����)

    => Set_TrayDownMoveReset(mu8Data);              // Tray Down �ʱ�ȭ ���� ���� ���� - 0(�̵���), 1(����)
    => u8Return = Get_TrayDownMoveReset();          // Tray Down �ʱ�ȭ ���� ���� Ȯ�� - 0(�̵���), 1(����)

    => Set_TrayMotorUpStatus(mu8Data);              // Tray Up ���� ���� ���� - 0(�̵���), 1(����)
    => u8Return = Get_TrayMotorUpStatus();          // Tray Up ���� ���� Ȯ�� - 0(�̵���), 1(����)

    => Set_TrayMotorDownStatus(mu8Data);            // Tray Down ���� ���� ���� - 0(�̵���), 1(����)
    => u8Return = Get_TrayMotorDownStatus();        // Tray Down ���� ���� Ȯ�� - 0(�̵���), 1(����)

    => Set_TrayMotorPreUpStatus(mu8Data);           // Tray Up ���� ���� ���� ���� - 0(�̵���), 1(����)
    => u8Return = Get_TrayMotorPreUpStatus();       // Tray Up ���� ���� ���� Ȯ�� - 0(�̵���), 1(����)

    => Set_TrayMotorPreDownStatus(mu8Data);         // Tray Down ���� ���� ���� ���� - 0(�̵���), 1(����)
    => u8Return = Get_TrayMotorPreDownStatus();     // Tray Down ���� ���� ���� Ȯ�� - 0(�̵���), 1(����)
*/

#define AC_MOTOR_ICE_MAKING_USE                     USE

#if (AC_MOTOR_ICE_MAKING_USE == USE)

#define AC_MOTOR_ICE_MAKING_CONTROL_TIME_PERIOD     100     // ���� ���ۿ��� ���Ǵ� AC ���͵��� ���� �ֱ� Ÿ�̸� 100ms@1ms

#define ICETRAY_CW_TIME                             250     // Ice Tray CW 25Sec@100ms
#define ICETRAY_CCW_TIME                            500     // Ice Tray CCW 50Sec@100ms

#define TRAY_MOTOR_STOP                             0
#define TRAY_MOTOR_CW_DIRECTION                     1
#define TRAY_MOTOR_CCW_DIRECTION                    2

#define TRAY_ERROR_STANDBY_TIME                     6000    // Ʈ���� ���� ��� �ð� 10min@100ms

#define TRAY_UP_ICE_STUCK_DELAY_TIME                600

#define TRAY_UP_ICE_STUCK_COUNT                     5

/*..hui [23-9-21���� 3:53:57] ���� ���� ���ð� 90������ ����.. ������ ���̽� ���..*/
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
