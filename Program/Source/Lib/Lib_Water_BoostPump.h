/// @file     Lib_Water_BoostPump.h
/// @date     2025/06/02
/// @author   Jaejin Ham
/// @brief    Booster Pump ���� ���� Header File


#ifndef _LIB_WATER_BOOST_PUMP_H_
#define _LIB_WATER_BOOST_PUMP_H_

/*  Lib Boost Pump Function ����
    => Set_BoostPumpControlChangeStep(mu8Step);         // ��ũ�� �����⿡�� Boost Pump ���� �ܰ踦 ���� �����Ͽ� ���� ����
*/


#if (BOOST_PUMP_USE == USE)

#define BOOST_PUMP_CONTROL_PERIOD           1000        // 1sec@1ms

#define BOOST_CONTROL_STEP1                 0
#define BOOST_CONTROL_STEP2                 1
#define BOOST_CONTROL_STEP3                 2
#define BOOST_CONTROL_STEP4                 3

#define BOOST_CONTROL_STEP1_TIME            2400        // 40min = 2400sec(@1sec)
#define BOOST_CONTROL_STEP2_TIME            1200        // 20min = 1200sec(@1sec)
#define BOOST_CONTROL_STEP3_TIME            2400        // 40min = 2400sec(@1sec)
#define BOOST_CONTROL_STEP4_TIME            7200        // 120min = 7200sec(@1sec)


void Lib_BoostPump_Initialize(void);
void BoostPump_ControlTimer(void);
void Set_BoostPumpControlChangeStep(U8 mu8Step);
void Lib_BoostPump_Control(void);

#endif

void Lib_BoostPump_Module_Initialize(void);
void Lib_BoostPump_Module_1ms_Control(void);
void Lib_BoostPump_Module_Control(void);

#endif

