/**
 * File : err_temp.h
 * 
 * Error Temp
*/
#ifndef __ERR_TEMP_H__
#define __ERR_TEMP_H__

#include "prj_type.h"

#define STATUS_NORMAL      (1)
#define STATUS_OPEN        (2) 
#define STATUS_SHORT       (3)
U8 GetTempErrorType(U16 adc);

#define COUNT_NUM          (30)    /* 3sec, @100ms */
U8 CheckErrorTemp(U8 error, U16 adc, U8 *pBuf);

#endif /* __ERR_TEMP_H__ */
