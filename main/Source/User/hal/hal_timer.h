/**
 * File : hal_timer.h
 * 
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
**/
#ifndef __HAL_TIMER_H__
#define __HAL_TIMER_H__

#include "r_cg_timer.h"

#define CONFIG_TIMER00       (1)     /// TIMER TAU0 Channel 0 : Interval Timer
#define CONFIG_TIMER01       (0)     /// TIMER TAU0 Channel 1 : Interval Led Dimming Timer
#define CONFIG_TIMER03       (1)     /// TIMER TAU0 Channel 3 : Interval Step Motor Timer
#define CONFIG_TIMER04_05    (1)     /// TIMER TAU0 Channel 4, TAU0 Channel 5 : Single Pwm : Drain Pump, Cycle 10KHz
#define CONFIG_TIMER11       (1)     /// TIMER TAU1 Channel 1 : Square Wave : Buzzer */
#define CONFIG_TIMER12_13    (1)     /// TIMER TAU1 Channel 2, TAU1 Channel 3 : Single Pwm : Ice Water Drain Pump, Cycle 10KHz 

#if ( CONFIG_TIMER00 == 1 )

#define	INTS_BYTE_BASE_TIMER()          unsigned char _ints_base_timer_byte
#define	MASK_BASE_TIMER                 TMMK00
#define	DISABLE_INT_MASK_BASE_TIMER()   do { MASK_BASE_TIMER = 1; }while(0)
#define	ENABLE_INT_MASK_BASE_TIMER()    do { MASK_BASE_TIMER = 0; }while(0)

#define	ENTER_CRITICAL_SECTION_TIMER()	\
    do{ \
        _ints_base_timer_byte = MASK_BASE_TIMER; \
        DISABLE_INT_MASK_BASE_TIMER(); \
    }while(0)

#define	EXIT_CRITICAL_SECTION_TIMER()	\
    do{ \
        MASK_BASE_TIMER = _ints_base_timer_byte; \
    } while(0)

#define HAL_TIMER00_START()     do{ R_TAU0_Channel0_Start(); }while(0)
#define HAL_TIMER00_STOP()      do{ R_TAU0_Channel0_Stop(); TO0 &= ~_0001_TAU_CH0_OUTPUT_VALUE_1; }while(0)

#endif

#if ( CONFIG_TIMER01 == 1 )

#define	HAL_TIMER01_START()		    do{ R_TAU0_Channel1_Start(); }while(0)
#define	HAL_TIMER01_STOP()			  do{ R_TAU0_Channel1_Stop(); TO0 &= ~_0002_TAU_CH1_OUTPUT_VALUE_1;}while(0)

#endif

#if ( CONFIG_TIMER03 == 1 )

#define	INTS_BYTE_STEP_MOTOR_TIMER()        unsigned char _ints_step_motor_byte
#define	MASK_STEP_MOTOR_TIMER               TMMK03
#define	DISABLE_INT_MASK_STEP_MOTOR_TIMER() do { MASK_STEP_MOTOR_TIMER = 1; }while(0)
#define	ENABLE_INT_MASK_STEP_MOTOR_TIMER()  do { MASK_STEP_MOTOR_TIMER = 0; }while(0)

#define	ENTER_CRITICAL_SECTION_STEP_MOTOR_TIMER()	\
    do{ \
        _ints_step_motor_byte = MASK_STEP_MOTOR_TIMER; \
        DISABLE_INT_MASK_STEP_MOTOR_TIMER(); \
    }while(0)

#define	EXIT_CRITICAL_SECTION_STEP_MOTOR_TIMER()	\
    do{ \
        MASK_STEP_MOTOR_TIMER = _ints_step_motor_byte; \
    } while(0)

#define HAL_TIMER03_START()     do{ R_TAU0_Channel3_Start(); }while(0)
#define HAL_TIMER03_STOP()      do{ R_TAU0_Channel3_Stop(); TO0 &= ~_0008_TAU_CH3_OUTPUT_VALUE_1; }while(0)

#endif

#if ( CONFIG_TIMER04_05 == 1 )

#define TIMER04_TDR               TDR04
#define TIMER05_TDR               TDR05

#define	HAL_TIMER04_05_START()		do{R_TAU0_Channel4_Start();}while(0)
#define	HAL_TIMER04_05_STOP()			do{R_TAU0_Channel4_Stop(); TO0 &= ~_0010_TAU_CH4_OUTPUT_VALUE_1; TO0 &= ~_0020_TAU_CH5_OUTPUT_VALUE_1;}while(0)

#define HAL_TIMER05_SET_TDR(data)	do{ TIMER05_TDR = data; }while(0)
#define HAL_TIMER05_GET_TOE()     (TOE0 & _0020_TAU_CH5_OUTPUT_ENABLE )

#endif

#if ( CONFIG_TIMER11 == 1 )

#define TIMER11_TDR               TDR11

#define	HAL_TIMER11_START()		    do{R_TAU1_Channel1_Start();}while(0)
#define	HAL_TIMER11_STOP()			  do{R_TAU1_Channel1_Stop(); TO1 &= ~_0002_TAU_CH1_OUTPUT_VALUE_1;}while(0)

#define HAL_TIMER11_SET_TDR(data)	do{ TIMER11_TDR = data; }while(0)

#endif

#if ( CONFIG_TIMER12_13 == 1 )

#define TIMER12_TDR               TDR12
#define TIMER13_TDR               TDR13

#define	HAL_TIMER12_13_START()		do{R_TAU1_Channel2_Start();}while(0)
#define	HAL_TIMER12_13_STOP()			do{R_TAU1_Channel2_Stop(); TO1 &= ~_0004_TAU_CH2_OUTPUT_VALUE_1; TO1 &= ~_0008_TAU_CH3_OUTPUT_VALUE_1;}while(0)

#define HAL_TIMER13_SET_TDR(data)	do{ TIMER13_TDR = data; }while(0)
#define HAL_TIMER13_GET_TOE()     (TOE1 & _0008_TAU_CH3_OUTPUT_ENABLE )

#endif

#endif
