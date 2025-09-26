/// @file     Lib_Water_IceMaking_ModeDecision.h
/// @date     2025/08/05
/// @author   Jaejin Ham
/// @brief    제빙 관련 모드 제어 관련


#ifndef _LIB_WATER_ICE_MAKING_MODE_DECISION_H_
#define _LIB_WATER_ICE_MAKING_MODE_DECISION_H_


#define LIB_ICE_MAKING_MODE_DECISION_USE                    USE

#if (LIB_ICE_MAKING_MODE_DECISION_USE == USE)

#define MODE_DECISION_CONTROL_TIME_PERIOD                   100     // 100ms@1ms

extern UTYPE_WORD  U16ColdOperationW;
#define u16ColdOperation                                    U16ColdOperationW.word
#define u8ColdOperation_L                                   U16ColdOperationW.byte[0]
#define u8ColdOperation_H                                   U16ColdOperationW.byte[1]
#define Bit0_Cold_Setting                                   U16ColdOperationW.Bit.b0
#define Bit1_Cold_Temp_Protect                              U16ColdOperationW.Bit.b1
#define Bit2_Cold_Temp_Control                              U16ColdOperationW.Bit.b2
#define Bit3_Ice_Init_Cold_Stop                             U16ColdOperationW.Bit.b3
#define Bit4_Cold_Error                                     U16ColdOperationW.Bit.b4
#define Bit5_Cold_Flushing_Stop                             U16ColdOperationW.Bit.b5
#define Bit6_Cold_Ice_Ster_Stop                             U16ColdOperationW.Bit.b6
#define Bit7_Cody_Service_Cold_Off                          U16ColdOperationW.Bit.b7

extern UTYPE_WORD  U16IceOperationW;
#define u16IceOperation                                     U16IceOperationW.word
#define u8IceOperation_L                                    U16IceOperationW.byte[0]
#define u8IceOperation_H                                    U16IceOperationW.byte[1]
#define Bit0_Ice_Setting                                    U16IceOperationW.Bit.b0
#define Bit1_Ice_Temp_Protect                               U16IceOperationW.Bit.b1
#define Bit2_Ice_Full                                       U16IceOperationW.Bit.b2
#define Bit3_Ice_Stop_Six_Hour                              U16IceOperationW.Bit.b3
#define Bit4_Ice_Init_Ice_Stop                              U16IceOperationW.Bit.b4
#define Bit5_Ice_Stop_Safty_Routine                         U16IceOperationW.Bit.b5
#define Bit6_Ice_Error                                      U16IceOperationW.Bit.b6
#define Bit7_Cody_Service_Ice_Off                           U16IceOperationW.Bit.b7
#define Bit8_Ice_Stop_Flushing                              U16IceOperationW.Bit.b8
#define Bit9_Ice_Stop_Ice_Ster                              U16IceOperationW.Bit.b9
#define Bit10_Fast_Ice_Make_Limit                           U16IceOperationW.Bit.b10

extern UTYPE_BYTE  U8ColdTempControlB;
#define gu8_Cold_Temp_Control                               U8ColdTempControlB.byte
#define Bit0_Temp_Control                                   U8ColdTempControlB.Bit.b0
#define Bit1_Temp_Add_Control                               U8ColdTempControlB.Bit.b1

extern UTYPE_BYTE  U8MakeModeB;
#define gu8_Make_Mode                                       U8MakeModeB.byte
#define Bit0_Cold_Make_Go                                   U8MakeModeB.Bit.b0
#define Bit1_Ice_Make_Go                                    U8MakeModeB.Bit.b1


#define MAKE_STOP                                           0
#define MAKE_COLD                                           1
#define MAKE_ICE                                            2
#define MAKE_DUAL                                           3

#define COLD_FIRST_ICE_MAKE                                 0
#define ICE_FIRST_ICE_MAKE                                  1

#define NEW_FAST_ICE                                        0
#define RECOVER_FAST_ICE                                    1

#define COLD_ON_TEMP_NORMAL                                 6.8f
#define COLD_ON_TEMP_SAVING                                 10.0f

#define COLD_OFF_REF_TEMP_AMB                               30.0f
#define COLD_OFF_TEMP_4_DEGREE                              4.0f
#define COLD_OFF_TEMP_5_DEGREE                              5.0f
#define PROTECT_COLD_TEMP                                   2.0f

/*#define COLD_FIRST_OPERATION_20_DEGREE      20*/
/*..hui [25-5-23오후 2:09:32] 버그수정..*/
#define COLD_FIRST_OPERATION_20_DEGREE                      20.0f

#define DUAL_OP_TEMP_AMB_20_DEGREE                          20.0f

#define COLD_ADD_OPERATION_TIME                             18000

/*#define DRAIN_FULL_ICE_STOP_TIME    3000*/
#define DRAIN_FULL_ICE_STOP_TIME                            6000

#define ICE_MAKE_GOGOGO                                     0x7FF
#define COLD_MAKE_GOGOGO                                    0xFF

/*..hui [24-4-11오후 4:01:39] 얼음우선 변경제어..*/
/*#define FAST_ICE_MAKE_COUNT          4*/
#define FAST_ICE_MAKE_COUNT                                 3
#define FAST_ICE_MAKE_ENABLE                                0x3FF
#define FAST_ICE_COLD_MAX_OP_TIME                           36000

#define DRAIN_LEVEL_EMPTY                                   1
#define DRAIN_LEVEL_LOW                                     2
#define DRAIN_LEVEL_HIGH                                    4
#define DRAIN_LEVEL_ERROR                                   8


void Lib_ModeDecision_Initialize(void);
void IceMakingModeDecisionControlTimer(void);
void Make_Mode_Decision(void);
void cold_mode_decision(void);
void Set_IceDrainTankLevel(U16 mu8Status);
U8 Get_IceDrainTankLevel(void);
void Set_FastIceMakeStatus(U16 mu8Status);
U8 Get_FastIceMakeStatus(void);
void ice_mode_decision(void);
void count_ice_make(void);
U8 cold_comp_test(void);
void final_mode_decision(void);
void cold_priority_decision(void);
void ice_priority_decision(void);

#endif

void Lib_IceMaking_ModeDecision_Module_Initialize(void);
void Lib_IceMaking_ModeDecision_Module_1ms_Control(void);
void Lib_IceMaking_ModeDecision_Module_Control(void);

#endif
