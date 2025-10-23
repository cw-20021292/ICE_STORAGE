/**
 * File : leak.h
 * 
 * Leak Sensor
**/
#ifndef __LEAK_H__
#define __LEAK_H__

#include "prj_type.h"

//#define  LEAK_ADC_LEVEL             (750) /* 3.66v */        // AIS 3.0 SODA 
//#define  LEAK_ADC_LEVEL               (475) /* 2.31v */      // ICON AIS �ʹ� ����. �̰��� �Ǵ� ���� ���� ( guardian )
//#define  LEAK_ADC_LEVEL               (612) /* 2.99v */      // ICON AIS 
#define  LEAK_ADC_LEVEL               (615) /* 3.00v */        // WESTERN BUILT-IN

typedef struct _leakage_
{
    /* Status */
    U8  mUstatus;             // ���� �߻� ���� ( TRUE : ����, FALSE : ���� )
    
    /* CHECK */
    U16 mUcheckNormalTime;    // ���� ���� �ð�  ����
    U16 mUcheckErrorTime;     // ���� ���� �ð� ����
} SLeakage_T;

void InitLeak(void);

void GetLeakData( SLeakage_T *xStptrData );

U8 GetLeakStatus(void);

U8 IsLeakSemi(void);

void ProcessLeak(void);

#endif  /* __LEAK_H__ */
