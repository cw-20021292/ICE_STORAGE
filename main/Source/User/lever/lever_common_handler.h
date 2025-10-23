/**
 * File : lever_common_handler.h
 * 
 * Lever Common Hnadler
*/
#ifndef __LEVER_COMMON_HANDLER_H__
#define __LEVER_COMMON_HANDLER_H__

#include "prj_type.h"

typedef U8(*FPLeverFun_T)(void);
typedef struct _lever_event_
{
    FPLeverFun_T pActionEvent_1;   // short 
    FPLeverFun_T pActionEvent_2;   // long 2sec
    FPLeverFun_T pActionEvent_3;   // long 8sec
    FPLeverFun_T pActionEvent_4;   // pop
} SLeverEventList_T;

#endif /* __LEVER_COMMON_HANDLER_H__ */
