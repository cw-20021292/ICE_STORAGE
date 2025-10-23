/**
 * File : key_common_handler.h
 * 
 * Key Handler Command Header
*/
#ifndef __KEY_COMMON_HANDLER_H__
#define __KEY_COMMON_HANDLER_H__

#include "prj_type.h"

/* Ű ��ư �Է� ��ȿ�� ���� �˻� �Լ� �������̽� */
typedef U8 (*FPKeyValidFun_T)(U32 valKey);

/* Ű �̺�Ʈ ����Ʈ �������̽� */
typedef enum _key_action_
{
    KEY_ACTION_SHORT = 0,    /// 0 : Short Key
    KEY_ACTION_2_SEC,        /// 1 : Long Key 2sec
    KEY_ACTION_3_SEC,        /// 2 : Long Key 3sec
    KEY_ACTION_5_SEC,        /// 3 : Long Key 5sec
    KEY_ACTION_7_SEC,        /// 4 : Long Key 7sec
    KEY_ACTION_POP,          /// 5 : Pop Key
    KEY_ACTION_1_SEC,        /// 6 : Long Key 1sec ( For Test Mode Time Short )
    KEY_ACTION_MAX,          /// 7 : Key Action Max 
} EKeyAction_T;

typedef U8(*FPKeyFun_T)(void);
typedef struct _key_event_list_
{
    U32 Key;
    FPKeyFun_T pActionEvent[KEY_ACTION_MAX];
} SKeyEventList_T;

// KEY EVENT LIST SIZE 
typedef U8 (*KeyEventSize_T)(void);

#endif /* __KEY_COMMON_HANDLER_H__ */
