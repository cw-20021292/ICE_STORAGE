/// @file     Lib_Water_IceMaking.h
/// @date     2025/08/04
/// @author   Jaejin Ham
/// @brief    제빙 동작 제어 file


#ifndef _LIB_WATER_ICE_MAKING_H_
#define _LIB_WATER_ICE_MAKING_H_

#define LIB_ICE_MAKING_USE                                  USE

#if (LIB_ICE_MAKING_USE == USE)


/// @brief      제빙 동작 제어 Step 리스트
/// @details    제빙 동작 제어 Step을 리스트로 정의한다
typedef enum
{
    STATE_0_STANDBY = 0,

    STATE_5_PREHEAT_HOTGAS_MOVE = 5,
    STATE_6_CALC_PREHEAT_TIME = 6,
    STATE_7_PREHEAT_OPERATION = 7,

    STATE_10_ICE_TRAY_MOVE_UP = 10,
    STATE_11_WAIT_ROOM_WATER_FULL = 11,
    STATE_12_CONT_ICE_SWITCH_MOVE = 12,
    STATE_13_CONT_RPS_SETTING = 13,
    STATE_14_CHECK_ICE_TRAY_HZ = 14,

    STATE_20_WATER_IN_ICE_TRAY = 20,
    STATE_21_ICE_SWITCH_MOVE = 21,

    STATE_30_CALC_ICE_MAKING_TIME = 30,
    STATE_31_MAIN_ICE_MAKING = 31,

    STATE_40_ICE_TRAY_MOVE_DOWN = 40,
    STATE_41_GAS_SWITCH_HOT_GAS = 41,
    STATE_42_CALC_HOT_GAS_TIME = 42,
    STATE_43_ICE_TAKE_OFF = 43,
    /*STATE_44_FEEDER_OPERATION = 44,*/

    STATE_50_ICE_FULL_IR_CHECK = 50,
    STATE_51_FINISH_ICE_MAKE = 51
} ICE_STEP;


/// @brief      Ice Tray 상태 리스트
/// @details    Ice Tray 상태을 리스트로 정의한다
typedef enum
{
    ICETRAY_STATE_ICEMAKING = 0,
    ICETRAY_STATE_ICETHROW,
    ICETRAY_STATE_MOVING,
    ICETRAY_STATE_ERROR,
} ICETRAY_STATE;


/// @brief      얼음 녹이는 동작 상태 리스트
/// @details    얼음 녹이는 동작 상태을 리스트로 정의한다
typedef enum
{
    STATE_0_ICE_MELT_STANDBY_STATE = 0,

    STATE_10_ICE_MELT_COLD_FEED_STATE = 10,

    STATE_20_ICE_MELT_HOT_GAS_OPERATE_STATE = 20,

    STATE_30_ICE_MELT_HOT_GAS_WAIT_STATE = 30,

    STATE_40_ICE_MELT_ADD_COLD_FEED_STATE = 40,

    STATE_50_ICE_MELT_REPEAT_DECISION_STATE = 50

} ICE_MELT_STEP;


#define P_IR_POWER                              P3_bit.no7
#define TURN_ON_IR_POWER()                      { P_IR_POWER = 1; }
#define TURN_OFF_IR_POWER()                     { P_IR_POWER = 0; }
#define GET_STATUS_IR_POWER()                   ( P_IR_POWER )


/*#define ICEFULL_LEV 125*/                         // 만빙 인식 레벨 1V(204)->0.6V(125)
/*#define ICEFULL_LEV 250*/                         // 만빙 인식 레벨 1V(204)->0.6V(125)
/*#define ICEFULL_LEV 700*/                         // 만빙 인식 레벨 1V(204)->0.6V(125)

//#define ICEFULL_LEV 500                           // 만빙 인식 레벨 1V(204)->0.6V(125)

//#define ICEFULL_LEV 150                             // 만빙 인식 레벨 1V(204)->0.6V(125)

#define ICEFULL_LEV                                 250     // 만빙 인식 레벨 1V(204)->0.6V(125)
#define IR_INTERVAL                                 9000    // 만빙 인식 주기 15min = 900sec@100ms

#define DUMMY_ICE_MELT_TIME                         1800

#define ICE_INIT_ERROR_CANCEL_TIME                  72000   // 제빙 초기화 동작 취소 시간 2hr = 120min = 7200sec@100ms

#define C_ICE_TRAY_FILL_200CC                       585

#define ICE_MAKE_TIME                               899         /* 외기 25'C, 시수 온도 25'C 기준 */
#define PREHEAT_AMB_TEMP                            25.0f

#define HOT_GAS_NOISE_REDUCE_TIME                   300

#define HEAT_TORAL_TIME_10_UNDER                    5150
#define HEAT_TORAL_TIME_15_UNDER                    3600
#define HEAT_TORAL_TIME_20_UNDER                    3000
#define HEAT_TORAL_TIME_25_UNDER                    2700
#define HEAT_TORAL_TIME_30_UNDER                    2700
#define HEAT_TORAL_TIME_35_UNDER                    2700
#define HEAT_TORAL_TIME_35_OVER                     2700

#define HOT_GAS_TIME_9_UNDER_765S                   7650
#define HOT_GAS_TIME_13_UNDER_600S                  6000
#define HOT_GAS_TIME_19_UNDER_180S                  1800
#define HOT_GAS_TIME_24_UNDER_30S                   300
#define HOT_GAS_TIME_29_UNDER_20S                   200
#define HOT_GAS_TIME_30_OVER_15S                    150

#define PREHEAT_TIME_14_UNDER_600S                  6000
#define PREHEAT_TIME_20_UNDER_360S                  3600
#define PREHEAT_TIME_25_UNDER_360S                  3600


void Lib_IceMaking_Initialize(void);
void IceMakingControlTimer(void);
void Set_IceOnOff(U16 mu8Status);
U8 Get_IceOnOff(void);
void Set_IcePreheat(U16 mu8Status);
U8 Get_IcePreheat(void);
void Set_AD_IceFull(U16 mu16AD_Data);
U16 Get_AD_IceFull(void);
void check_ice_full(void);
void ice_init_operation(void);
void Ice_Make_Process(void);
void ice_make_operation(void);
U16 calc_ice_make_time(F32 mf32AirTemp, F32 mf32RoomTemp);
U8 hot_gas_operation(void);
void recovery_ice_tray(void);
void reduce_hot_gas_noise(void);
U8 get_ice_mode_comp_rps(void);
U8 get_hotgas_mode_comp_rps(void);
U8 get_preheat_mode_comp_rps(void);
U16 get_hotgas_time(void);
U16 get_preheat_time(void);
void ice_system_reset(void);
void ice_stuck_reset(void);
void ice_system_stop(void);
void Set_IceValve(U8 mu8Status);
U8 Get_IceValve(void);
void Set_IceSafetyRoutine(U8 mu8Status);
U8 Get_IceSafetyRoutine(void);
void Set_IceFullCheck(U8 mu8Status);
U8 Get_IceFullCheck(void);
void Set_IceFullStatus(U8 mu8Status);
U8 Get_IceFullStatus(void);
void Set_InitIceStatus(U8 mu8Status);
U8 Get_InitIceStatus(void);
void Set_InitIceStep(U8 mu8Step);
U8 Get_InitIceStep(void);
void Set_IceStep(U8 mu8Step);
U8 Get_IceStep(void);
void Set_IceMakingDisable(U8 mu8Status);
U8 Get_IceMakingDisable(void);
void Set_IceOutCCWStatus(U8 mu8Status);
U8 Get_IceOutCCWStatus(void);
void Set_IceOutStatus(U8 mu8Status);
U8 Get_IceOutStatus(void);
void Set_IceOutContinueStatus(U8 mu8Status);
U8 Get_IceOutContinueStatus(void);
void Check_AbleIceMaking(void);
U16 Get_IceMakeTime(void);
void check_ice_system_ok(void);
void Set_IceSystemStatus(U8 mu8Status);
U8 Get_IceSystemStatus(void);


#endif

void Lib_IceMaking_Module_Initialize(void);
void Lib_IceMaking_Module_1ms_Control(void);
void Lib_IceMaking_Module_Control(void);

#endif
