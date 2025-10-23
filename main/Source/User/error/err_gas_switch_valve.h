#ifndef __ERR_GAS_SWITCH_VALVE_H__
#define __ERR_GAS_SWITCH_VALVE_H__

#include "prj_type.h"

void InitErrGasSwitchValve(void);

U8 CheckErrGasSwitchValve(U8 mu8Error);
U8 ReleaseErrGasSwitchValve(U8 mu8Error);

void ProcessErrGasSwitchValve(void);

#endif /* __ERR_GAS_SWITCH_VALVE_H__ */
