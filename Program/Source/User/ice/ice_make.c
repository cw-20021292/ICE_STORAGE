
/**
 * @file ice_make.c
 * @author ��������
 * @brief ���� ������ �����ϱ� �̰� ���
 * @version 0.1
 * @date 2025-10-22
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#include "ice_make.h"

ICE_MODE  u8IceMode = 0;
ICE_DUMMY_DETACH_STEP  u8IceDummyDetachStep = 0;
ICE_INIT_STEP  u8IceInitStep = 0;
U8  u8IcePreheatStep = 0;
U8  u8IceReadyStep = 0;
U8  u8IceMakeStep = 0;
U8  u8IceDetachStep = 0;
U8  U8IceTrayRetryStep = 0;
/****************************************************************************************************/
/* APIs */
void SetIceMode(U8 u8_p_mode)
{
    u8IceMode |= u8_p_mode;
}

void ClearIceMode(U8 u8_p_mode)
{
    u8IceMode &= ~(u8_p_mode);
}

U8 GetIceMode(void)
{
    return u8IceMode;
}
/****************************************************************************************************/

void ProcessIceMake(void)
{
    /* ����Ż�� */
    if((GetIceMode() & ICE_MODE_DUMMY_DETACH) == ICE_MODE_DUMMY_DETACH)
    {
        if(ProcessIceDummyDetach() == TRUE)
        {
            /* �Ϸ� �� �ʱ�ȭ */
            IceDummyDetachInit();
            ClearIceMode(ICE_MODE_DUMMY_DETACH);
        }
    }   
    /* �ʱ�ȭ */
    else if((GetIceMode() & ICE_MODE_INIT) == ICE_MODE_INIT)
    {
        if(ProcessIceInit() == TRUE)
        {
            /* �Ϸ� �� �ʱ�ȭ */
            IceInit();
            ClearIceMode(ICE_MODE_INIT);
        }
    }
    /* ���� �� ���� */
    else if((GetIceMode() & ICE_MODE_PREHEAT) == ICE_MODE_PREHEAT)
    {
        
    }
    /* ���� �غ� */
    /* ���� */
    /* Ż�� */
}

/****************************************************************************************************/
/* ����Ż�� */
void IceDummyDetachInit(void)
{
    u8IceDummyDetachStep = 0;
}

U8 ProcessIceDummyDetach(void)
{
    U8 mu8Done = FALSE; 

    switch (u8IceDummyDetachStep)
    {
        case ICE_TRAY_DOWN:
            
            break;
        
        case ICE_TRAY_DOWN_CHECK:
            
            break;
        
        case COMP_STATUS_CHECK:
            
            break;
        
        case ICE_SWITCH:
            
            break;

        case ICE_MELTING:
            
            break;

        case HOT_GAS_SWITCH:
            
            break;

        case HOT_GAS_HZ_SET:
            
            break;

        case HOT_GAS_ON:
            
            break;

        case DUMMY_DETACH_COMPLETE:
            mu8Done = TRUE;
            break;

        default:
            u8IceDummyDetachStep = 0;
            break;
    }

    return mu8Done;
}

/****************************************************************************************************/
/* ���� �� �ʱ�ȭ */
void IceInit(void)
{
    u8IceInitStep = 0;
}

U8 ProcessIceInit(void)
{
    U8 mu8Done = FALSE; 

    switch (u8IceInitStep)
    {
        case STEP_INIT_TRAY_MOVE:
            
            break;
        
        case STEP_INIT_TRAY_DEICING:
            
            break;
        
        case STEP_INIT_TRAY_DONE:
            mu8Done = TRUE; /* PROCESS �Ϸ� */
            break;
        
        default:
            u8IceInitStep = 0;
            break;
    }


    return mu8Done;
}