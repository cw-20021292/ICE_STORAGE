/**
 * File : key_test_handler.h
 * 
 * Key Test Handler Function
*/
#ifndef __KEY_TEST_HANDLER_H__
#define __KEY_TEST_HANDLER_H__

#include "prj_type.h"

U8 IsValidKeyTestCondition(U32 key);
void* GetTestKeyEventList(void);
U8 GetTestKeyEventListSize(void);

#endif /* __KEY_TEST_HANDLER_H__ */
