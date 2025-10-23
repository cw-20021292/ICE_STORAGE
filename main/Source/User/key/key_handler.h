/**
 * File : key_handler.h
 * 
 * Key Normal Main Handler Function
**/
#ifndef __KEY_HANDLER_H__
#define __KEY_HANDLER_H__

#include "prj_type.h"

U8 IsValidKeyCondition(U32 key);
void* GetKeyEventList(void);

U8 GetKeyEventListSize(void);

#endif /* __KEY_HANDLER_H__ */
