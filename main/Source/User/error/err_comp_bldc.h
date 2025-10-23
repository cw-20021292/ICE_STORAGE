/**
 * File : error_bldc_comp.h
 * 
 * Bldc Comp Module Error Code Check
**/
#ifndef __ERR_COMP_BLDC_H__
#define __ERR_COMP_BLDC_H__

#include "prj_type.h"

#include "comp_bldc.h"

#define ERR_COMP_PRT    (0)
#define ERR_COMP_ERR    (1)

void InitErrCompBldc(void);

U8 GetErrCompBldcCount(U8 type, ECompBldcErrorId_T error);

// PROTECT
U8 CheckProtectCompBldc(U8 xError);
U8 ReleaseProtectCompBldc(U8 xError);

// ERROR
U8 CheckErrCompBldcErr1(U8 xError);
U8 CheckErrCompBldcErr2(U8 xError);
U8 CheckErrCompBldcErr3(U8 xError);
U8 CheckErrCompBldcErr4(U8 xError);
U8 CheckErrCompBldcErr5(U8 xError);
U8 CheckErrCompBldcErr6(U8 xError);
U8 CheckErrCompBldcErr7(U8 xError);
U8 CheckErrCompBldcErr8(U8 xError);
U8 CheckErrCompBldcErr9(U8 xError);

#endif  /* __ERR_COMP_BLDC_H__ */
