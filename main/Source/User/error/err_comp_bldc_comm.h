/**
 * File : error_comp_bldc_comm.h
 * 
 * Bldc Comp Module Error Commnunication Check
**/
#ifndef __ERROR_COMP_BLDC_COMM_H__
#define __ERROR_COMP_BLDC_COMM_H__

#include "prj_type.h"

void InitErrCompBldcComm(void);
void ResetCompBldcCommErrorCount(void);

// PROTECT
U8 CheckProtectCompBldcComm(U8 error);
U8 ReleaseProtectCompBldcComm(U8 error);

// ERROR
U8 CheckErrCompBldcComm(U8 error);

#endif  /* __ERROR_COMP_BLDC_COMM_H__ */
