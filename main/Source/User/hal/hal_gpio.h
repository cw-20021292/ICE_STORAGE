/**
 * File : hal_gpio.h
 * 
 * Hardware Abstraction Layer
 * Depend on Renesas MCU Chip
**/
#ifndef __HAL_GPIO_H__
#define __HAL_GPIO_H__

#include "r_cg_intc.h"

#define CONFIG_INTP3          (0)     /* External Gpio INTP3 : NONE */
#define CONFIG_INTP4          (1)     /* External Gpio INTP4 : Tray Flow Meter */
#define CONFIG_INTP5          (0)     /* External Gpio INTP5 : NONE */
#define CONFIG_INTP6          (0)     /* External Gpio INTP6 : NONE */
#define CONFIG_INTP7          (0)     /* External Gpio INTP7 : NONE */

#if ( CONFIG_INTP3 == 1 )

#define	INTS_BYTE_FLOW_METER_INNO()           unsigned char _ints_flow_inno_byte
#define	MASK_FLOW_METER_INNO                  PMK3
#define	DISABLE_INT_MASK_FLOW_METER_INNO()    do { MASK_FLOW_METER_INNO = 1; }while(0)
#define	ENABLE_INT_MASK_FLOW_METER_INNO()     do { MASK_FLOW_METER_INNO = 0; }while(0)

#define	ENTER_CRITICAL_SECTION_FLOW_METER_INNO()	\
    do{ \
        _ints_flow_inno_byte = MASK_FLOW_METER_INNO; \
        DISABLE_INT_MASK_FLOW_METER_INNO(); \
    }while(0)

#define	EXIT_CRITICAL_SECTION_FLOW_METER_INNO()	\
    do{ \
        MASK_FLOW_METER_INNO = _ints_flow_inno_byte; \
    } while(0)

#define HAL_INTP3_START()     do{ R_INTC3_Start(); }while(0)
#define HAL_INTP3_STOP()      do{ R_INTC3_Stop(); }while(0)

#endif

#if ( CONFIG_INTP4 == 1 )
    
#define	INTS_BYTE_FLOW_METER()                unsigned char _ints_flow_byte
#define	MASK_FLOW_METER                       PMK4
#define	DISABLE_INT_MASK_FLOW_METER()         do { MASK_FLOW_METER = 1; }while(0)
#define	ENABLE_INT_MASK_FLOW_METER()          do { MASK_FLOW_METER = 0; }while(0)
    
#define	ENTER_CRITICAL_SECTION_FLOW_METER()	\
        do{ \
            _ints_flow_byte = MASK_FLOW_METER; \
            DISABLE_INT_MASK_FLOW_METER(); \
        }while(0)
    
#define	EXIT_CRITICAL_SECTION_FLOW_METER()	\
        do{ \
            MASK_FLOW_METER = _ints_flow_byte; \
        } while(0)
    
#define HAL_INTP4_START()     do{ R_INTC4_Start(); }while(0)
#define HAL_INTP4_STOP()      do{ R_INTC4_Stop(); }while(0)
    
#endif


#if ( CONFIG_INTP5 == 1 )

#define	INTS_BYTE_FLOW_METER_NEO()            unsigned char _ints_flow_neo_byte
#define	MASK_FLOW_METER_NEO                   PMK5
#define	DISABLE_INT_MASK_FLOW_METER_NEO()     do { MASK_FLOW_METER_NEO = 1; }while(0)
#define	ENABLE_INT_MASK_FLOW_METER_NEO()      do { MASK_FLOW_METER_NEO = 0; }while(0)

#define	ENTER_CRITICAL_SECTION_FLOW_METER_NEO()	\
    do{ \
        _ints_flow_neo_byte = MASK_FLOW_METER_NEO; \
        DISABLE_INT_MASK_FLOW_METER_NEO(); \
    }while(0)

#define	EXIT_CRITICAL_SECTION_FLOW_METER_NEO()	\
    do{ \
        MASK_FLOW_METER_NEO = _ints_flow_neo_byte; \
    } while(0)

#define HAL_INTP5_START()     do{ R_INTC5_Start(); }while(0)
#define HAL_INTP5_STOP()      do{ R_INTC5_Stop(); }while(0)

#endif

#if ( CONFIG_INTP6 == 1 )

#define	INTS_BYTE_FLOW_METER_OUT()           unsigned char _ints_flow_out_byte
#define	MASK_FLOW_METER_OUT                  PMK6
#define	DISABLE_INT_MASK_FLOW_METER_OUT()    do { MASK_FLOW_METER_OUT = 1; }while(0)
#define	ENABLE_INT_MASK_FLOW_METER_OUT()     do { MASK_FLOW_METER_OUT = 0; }while(0)

#define	ENTER_CRITICAL_SECTION_FLOW_METER_OUT()	\
    do{ \
        _ints_flow_out_byte = MASK_FLOW_METER_OUT; \
        DISABLE_INT_MASK_FLOW_METER_OUT(); \
    }while(0)

#define	EXIT_CRITICAL_SECTION_FLOW_METER_OUT()	\
    do{ \
        MASK_FLOW_METER_OUT = _ints_flow_out_byte; \
    } while(0)

#define HAL_INTP6_START()     do{ R_INTC6_Start(); }while(0)
#define HAL_INTP6_STOP()      do{ R_INTC6_Stop(); }while(0)

#endif

#if ( CONFIG_INTP7 == 1 )

#define	INTS_BYTE_TDS_IN()              unsigned char _ints_tds_in_byte
#define	MASK_TDS_IN                     PMK7
#define	DISABLE_INT_MASK_TDS_IN()       do { MASK_TDS_IN = 1; }while(0)
#define	ENABLE_INT_MASK_TDS_IN()        do { MASK_TDS_IN = 0; }while(0)

#define	ENTER_CRITICAL_SECTION_TDS_IN()	\
    do{ \
        _ints_tds_in_byte = MASK_TDS_IN; \
        DISABLE_INT_MASK_TDS_IN(); \
    }while(0)

#define	EXIT_CRITICAL_SECTION_TDS_IN()	\
    do{ \
        MASK_TDS_IN = _ints_tds_in_byte; \
    } while(0)

#define HAL_INTP7_START()     do{ R_INTC7_Start(); }while(0)
#define HAL_INTP7_STOP()      do{ R_INTC7_Stop(); }while(0)

#endif

#endif  /* __HAL_GPIO_H__ */
