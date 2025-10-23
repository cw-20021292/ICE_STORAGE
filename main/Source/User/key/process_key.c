/**
 * File : process_key.c
 * 
 * Process Key
*/
#include "hw.h"
#include "prj_type.h"
#include "process_key.h"
#include "key.h"
#include "sound.h"
#include "ster.h"
#include "drain_water.h"
#include "power_saving.h"
#include "hot_water.h"
#include "process_display.h"
#include "key_common_handler.h"
#include "key_handler.h"
#include "key_test_handler.h"
#include "key_eol_handler.h"
#include "key_fct_handler.h"
#include "smart_saving.h"

/*
 *
 * Param :
 *  -. pValidFunc, : ��ȿ�� �˻� �Լ� ������ 
 *  -. pList, : Ű ��ư �̺�Ʈ ����Ʈ
 *  -. mu8Size : Ű ��ư �̺�Ʈ ����Ʈ ũ��
 * 
 * Return :
 *  -. FALSE : Ű �Է� ó���� �Ȱ� ����.
 *  -. TRUE : Ű �Է� ó���� �Ȱ� ����.
 */
extern U8 gu8IceWaterAmountLock;
extern U8 gu8IceOff;
extern U8 gu8HotOff;
extern U8 gu8ColdOff;
static U8 KeyEventHandler(FPKeyValidFun_T pValidFunc, SKeyEventList_T *pList, U8 mu8Size )
{
    U8   i = 0U;
    U32  keyVal = 0UL;
    U32  keyPopVal = 0UL;
    U8   sound = 0U;

    static U32 prevKeyVal = K_NONE;
    static U8 errKeySkip = FALSE;

    /* Ű �Է� �����͸� �����´�. */
    keyVal = GetKeyVal();
    keyPopVal = GetKeyPopVal();

    /* Reset Unused Saving - HOT WATER 
     * Ű �Է� ��, 
     * -. �¼� �̻�� ���� ����
     * -. ���� ��� ����
     */
    if( keyVal != K_NONE )
    {
        //ResetUnusedSavingTime();
        ResetSmartUnusedSavingTime();    // UnusedSaving
        WakeUpSavingMode();
    }

    if( keyVal != prevKeyVal )
    {
        prevKeyVal = keyVal;
        errKeySkip = FALSE;

        gu8IceWaterAmountLock = FALSE;
        gu8IceOff = FALSE;
        gu8HotOff = FALSE;
        gu8ColdOff = FALSE;   
    }

    if( errKeySkip == TRUE )
    {
        return TRUE;
    }

    if( pList == NULL || mu8Size == 0 )
    {
        return TRUE;
    }

    /* Check Pop Key */
    if( keyVal == K_NONE 
            && IsSetKeyEventFlag(KEY_EVENT_POP) == TRUE )
    {
        ClearKeyEventFlag(KEY_EVENT_POP);
        for( i=0; i<mu8Size; i++ )
        {
            if( (pList+i)->Key == keyPopVal )
            {
                if( (pList+i)->pActionEvent[KEY_ACTION_POP] != NULL )
                {
                    sound = (pList+i)->pActionEvent[KEY_ACTION_POP]();
                    Sound(sound);
                    return TRUE;
                }
            }
        }
    }
    
    /* Check Pusehd Key */
    for( i = 0; i < mu8Size ; i++ )
    {
        if( (pList+i)->Key == keyVal )
        {
            if( IsSetKeyEventFlag(KEY_EVENT_PUSHED_SHORT) == TRUE )
            {
                ClearKeyEventFlag(KEY_EVENT_PUSHED_SHORT);

                if( ResetDrainWater() == TRUE )
                {
                    Sound(SOUND_CANCEL);
                }
                //if( SetSterDone( FALSE ) == TRUE )
                //{
                //    Sound(SOUND_SELECT);
                //}

                /* Ű ��ư �Է� ��ȿ�� �˻� */
                if( keyVal != K_NONE 
                        && pValidFunc != NULL )
                {
                    if( pValidFunc(keyVal) == FALSE )
                    {
                        if( GetSkipInvalidSound() == FALSE )
                        {
                            Sound(SOUND_ERROR);
                        }
                        SetSkipInvalidSound(FALSE);
                        errKeySkip = TRUE;
                        return TRUE;
                    }
                }

                if( (pList+i)->pActionEvent[KEY_ACTION_SHORT] != NULL )
                {
                    sound = (pList+i)->pActionEvent[KEY_ACTION_SHORT]();
                    Sound(sound);
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag(KEY_EVENT_PUSHED_LONG_7S) == TRUE )
            {
                ClearKeyEventFlag(KEY_EVENT_PUSHED_LONG_7S);

                if( (pList+i)->pActionEvent[KEY_ACTION_7_SEC] != NULL )
                {
                    sound = (pList+i)->pActionEvent[KEY_ACTION_7_SEC]();
                    Sound(sound);
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag(KEY_EVENT_PUSHED_LONG_5S) == TRUE )
            {
                ClearKeyEventFlag(KEY_EVENT_PUSHED_LONG_5S);

                if( (pList+i)->pActionEvent[KEY_ACTION_5_SEC] != NULL )
                {
                    sound = (pList+i)->pActionEvent[KEY_ACTION_5_SEC]();
                    Sound(sound);
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag(KEY_EVENT_PUSHED_LONG_3S) == TRUE )
            {
                ClearKeyEventFlag(KEY_EVENT_PUSHED_LONG_3S);

                if( (pList+i)->pActionEvent[KEY_ACTION_3_SEC] != NULL )
                {
                    sound = (pList+i)->pActionEvent[KEY_ACTION_3_SEC]();
                    Sound(sound);
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag( KEY_EVENT_PUSHED_LONG_2S ) == TRUE )
            {
                ClearKeyEventFlag(KEY_EVENT_PUSHED_LONG_2S);

                if( (pList+i)->pActionEvent[KEY_ACTION_2_SEC] != NULL )
                {
                    sound = (pList+i)->pActionEvent[KEY_ACTION_2_SEC]();
                    Sound(sound);
                    return TRUE;
                }
                break;
            }
            else if( IsSetKeyEventFlag(KEY_EVENT_PUSHED_LONG_1S) == TRUE )
            {
                ClearKeyEventFlag(KEY_EVENT_PUSHED_LONG_1S);

                if( (pList+i)->pActionEvent[KEY_ACTION_1_SEC] != NULL )
                {
                    sound = (pList+i)->pActionEvent[KEY_ACTION_1_SEC]();
                    Sound(sound);
                    return TRUE;
                }
                break;
            }
        }
    }

    return FALSE;
}

#define KEY_STABLE_PROCESS_DELAY  (30)    // 300ms @10ms
void ProcessKeyEventHandler(void)
{
    U8 ret = FALSE;
    static U16 delay = 0;     /* Ű �Է� �� ���� �ð� */
    FPKeyValidFun_T pKeyValidFun = NULL;
    SKeyEventList_T *pKeyEventList = NULL;
    U8 keyEventSize = 0;

    /* ���÷��� ǥ�� �ʱ�ȭ  */
    if( GetDisplayInit() == FALSE )
    {
        return ;
    }

    /* ���� Ű �Է� ��, 
     * �ٽ� Ű �Է��� ��� ������� �ʵ��� ���� �ð��� ����
     *
     *  -. ���� ���� �Ϸ� �Ǳ� ����, �������� �����ؼ� �߻��Ǵ�
     *  �������� �����ϱ� ���� �����Ͽ���.
     */
    if( delay != 0 )
    {
        delay--;
        return ;
    }

    // Get Event Handler by case..
    if( GetEolStatus() == TRUE )
    {
        // EOL
        pKeyValidFun = IsValidEolKeyCondition;
        pKeyEventList = (SKeyEventList_T *)GetEolKeyEventList();
        keyEventSize = GetEolKeyEventListSize();
    }
    else if( GetFctStatus() == TRUE )
    {
        // FCT
        pKeyValidFun = IsValidFctKeyCondition;
        pKeyEventList = (SKeyEventList_T *)GetFctKeyEventList();
        keyEventSize = GetFctKeyEventListSize();
    }
    else
    {
        // NORMAL
        pKeyValidFun = IsValidKeyCondition;
        pKeyEventList = (SKeyEventList_T *)GetKeyEventList();
        keyEventSize = GetKeyEventListSize();
    }

#if( CONFIG_TEST_CHECK_FRONT == 1 )
    // TEST - FRONT DISPLAY
    pKeyValidFun = NULL; //IsValidTestKeyCondition;
    pKeyEventList = (SKeyEventList_T *)GetTestKeyEventList();
    keyEventSize = GetTestKeyEventListSize();
#endif

    // Call Key Event Handler
    //pKeyValidFun = NULL;
    ret = KeyEventHandler(pKeyValidFun, pKeyEventList, keyEventSize);
    if( ret == TRUE )
    {
        delay = KEY_STABLE_PROCESS_DELAY; // wait.. 300ms...
    }
}
