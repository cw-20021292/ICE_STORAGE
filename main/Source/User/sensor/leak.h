/**
 * File : leak.h
 * 
 * Leak Sensor
**/
#ifndef __LEAK_H__
#define __LEAK_H__

#include "prj_type.h"

//#define  LEAK_ADC_LEVEL             (750) /* 3.66v */        // AIS 3.0 SODA 
//#define  LEAK_ADC_LEVEL               (475) /* 2.31v */      // ICON AIS 너무 낮음. 미감지 되는 현상 있음 ( guardian )
//#define  LEAK_ADC_LEVEL               (612) /* 2.99v */      // ICON AIS 
#define  LEAK_ADC_LEVEL               (615) /* 3.00v */        // WESTERN BUILT-IN

typedef struct _leakage_
{
    /* Status */
    U8  mUstatus;             // 누수 발생 상태 ( TRUE : 누수, FALSE : 정상 )
    
    /* CHECK */
    U16 mUcheckNormalTime;    // 정상 기준 시간  범위
    U16 mUcheckErrorTime;     // 에러 기준 시간 범위
} SLeakage_T;

void InitLeak(void);

void GetLeakData( SLeakage_T *xStptrData );

U8 GetLeakStatus(void);

U8 IsLeakSemi(void);

void ProcessLeak(void);

#endif  /* __LEAK_H__ */
