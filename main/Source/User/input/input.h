/**
 * File : input.h
 * 
 * Input ( General Input : Etc - CHP/CP, ReedSw )
*/
#ifndef __INPUT_H__
#define __INPUT_H__

#include "prj_type.h"

#define INPUT_DELAY_30MS   (3U)        /// 30ms @10ms
#define INPUT_DELAY_100MS  (10U)       /// 100ms @10ms
#define INPUT_DELAY_300MS  (30U)       /// 300ms @10ms
#define INPUT_DELAY_500MS  (50U)       /// 500ms @10ms
#define INPUT_DELAY_1SEC   (100U)      /// 1sec @10ms

typedef enum _input_id_
{
    INPUT_ID_TANK_OPEN = 0,    /// 0 : Tank Cover
    INPUT_ID_TANK_OPEN_UV,     /// 1 : Tank Cover Uv
    INPUT_ID_TRAY_BRACKET,     /// 2 : Tray Bracket
    INPUT_ID_MAX               /// 3 : Max
} EInputId_T;

U8 GetCurrentInputVal(EInputId_T id);
U8 GetPreviousInputVal(EInputId_T id);
void SetInuptConfigDetectTime(EInputId_T id, U16 delay);

void ProcessInput(void);

#endif
