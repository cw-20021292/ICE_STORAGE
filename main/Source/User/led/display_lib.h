/**
 * File : display_lib.h
 * 
 * Led Display For Blink
*/
#ifndef __DISPLAY_LIB_H__
#define __DISPLAY_LIB_H__

#include "prj_type.h"

// TIMER COUNTER 
#define TIMER_100MS       (1U)
#define TIMER_500MS       (5U)
#define TIMER_1SEC        (10U)
#define TIMER_2SEC        (20U)
#define TIMER_3SEC        (30U)
#define TIMER_4SEC        (40U)
#define TIMER_5SEC        (50U)
#define TIMER_6SEC        (60U)
#define TIMER_7SEC        (70U)
#define TIMER_10SEC       (100U)
#define TIMER_20SEC       (200U)
#define TIMER_30SEC       (300U)
#define TIMER_20MIN       (12000U)
#define TIMER_60MIN       (36000U)

// TIMER BLINK INDEX 
#define DISP_TIMER_ID_500MS        (0)    /* LED ���� Ÿ�̸� 0.5sec */
#define DISP_TIMER_ID_1SEC         (1)    /* LED ���� Ÿ�̸� 1sec */
#define DISP_TIMER_ID_3SEC         (2)    /* LED ���� Ÿ�̸� 3sec, 1sec off */
#define DISP_TIMER_ID_4SEC         (3)    /* LED ���� Ÿ�̸� 4sec */
#define DISP_TIMER_ID_100MS        (4)    /* LED ���� Ÿ�̸� 0.1sec */
#define DISP_TIMER_ID_BLINK_MAX    (5)

typedef enum _disp_timer_id_
{
    DISP_TIMER_ID_KEY_AMOUNT,         /* ���� ���� */
    DISP_TIMER_ID_KEY_ICE,            /* ���� ���� */
    DISP_TIMER_ID_KEY_WATER,          /* �� ���� */
    DISP_TIMER_ID_KEY_HOT,            /* �¼� ���� */
    DISP_TIMER_ID_KEY_COLD,           /* �ü� ���� */
    DISP_TIMER_ID_KEY_HOT_TEMP,       /* �¼� ���� */
    DISP_TIMER_ID_CONFIG_TIME,        /* ���� ���� ���� Ÿ�̸� */
    DISP_TIMER_ID_ALARM_SETTING,

    DISP_TIMER_ID_HOT_STATUS,          /* �¼� �µ� ���� */
    DISP_TIMER_ID_COLD_STATUS,         /* �ü� �µ� ���� */

    DISP_TIMER_ID_CONFIG_AMOUNT,       /* ���� ���� */
    DISP_TIMER_ID_VERSION,             /* ���� ǥ�� �ð� */
    DISP_TIMER_ID_MEMENTO,             /* �޸��� Ÿ�̸� */
    DISP_TIMER_ID_SLEEP,              

    DISP_TIMER_ID_COLDMAKE,            /* �ü� ���� �� ���� ��   ���� ǥ�� �ð� */
    DISP_TIMER_ID_HOTMAKE,             /* �¼� ���� �� ���� ��   ���� ǥ�� �ð� */
    DISP_TIMER_ID_ICEMAKE,             /* ���� ���� �� ���� �� ���� ǥ�� �ð� */

    DISP_TIMER_ID_HOTLOCK_SET,         /* HOT LOCK ���� �� ���� ǥ�� �ð� */ 
    DISP_TIMER_ID_ICELOCK_SET,         /* ICE LOCK ���� �� ���� ǥ�� �ð� */ 
    DISP_TIMER_ID_ICEWATERLOCK_SET,    /* ICE LOCK ���� �� ���� ǥ�� �ð� */ 
    DISP_TIMER_ID_ALLLOCK_SET,         /* ALL LOCK ���� �� ���� ǥ�� �ð� */ 

    DISP_TIMER_ID_HOTLOCK,             /* HOT LOCK ���� ǥ�� �ð� */ 
    DISP_TIMER_ID_ICELOCK,             /* ICE LOCK ���� ǥ�� �ð� */ 
    DISP_TIMER_ID_ICEWATERLOCK,        /* ICE WATER LOCK ���� ǥ�� �ð� */ 
    DISP_TIMER_ID_ICEWATERAMOUNTLOCK,  /* ICE WATER AMOUNT LOCK ���� ǥ�� �ð� */
    DISP_TIMER_ID_ICEWATERAMOUNTNOTI,  /* ICE / ICE WATER AMOUNT NOTI */
    DISP_TIMER_ID_NOT_ICEWATER,        /* ������ ���� ���� NOTI */
    DISP_TIMER_ID_NOT_COLDWATER,       /* �ü� ���� ���� NOTI */
    DISP_TIMER_ID_ALLLOCK,             /* ALL LOCK ���� ǥ�� �ð� */ 
    DISP_TIMER_ID_STERLOCK,            /* STER LOCK ���� ǥ�� �ð� */

    DISP_TIMER_ID_ERROR,             
    DISP_TIMER_ID_HOT_ALTITUDE,        /* �¼��� ���� EXIT Ÿ�̸� */
    DISP_TIMER_ID_UNUSED_SAVE,         /* �̻�� ���� ���� EXIT Ÿ�̸� */
    DISP_TIMER_ID_DRAIN_WATER,         /* �ڵ� ��� ���� EXIT Ÿ�̸� */

    DISP_TIMER_ID_SETTING_MENU,        /* ���� �޴� EXIT Ÿ�̸� */
    DISP_TIMER_ID_SOUND_MENU,          /* ���� ���� EXIT Ÿ�̸� */
    DISP_TIMER_ID_CUSTOMER_MENU,       /* ���� ���� EXIT Ÿ�̸� */
    DISP_TIMER_ID_WIFI_AP_ERROR,       /* WIFI - AP ERROR CODE */

    DISP_TIMER_SEGON,              /* ��� ���� �� SEG  ON ǥ�� */
    DISP_TIMER_SEGOFF,             /* ��� ���� �� SEG OFF ǥ�� */

    DISP_TIMER_SEGOFF_BLINK,        /* ��� ���µ� �ش� ��� ���� ���� �� SEG OFF 2�� ���� ǥ�� */

    DISP_TIMER_ID_WATER_SELCET,     /* �¼�, ����, �ü� ���� �� Į�� ǥ�� ���� */
    DISP_TIMER_ID_ICE_SELCET,       /* ���� ,������ ���� �� Į�� ǥ�� ���� */

    DISP_TIMER_ID_MAX
}EDispTimerId_T;

void InitDispTimer(void);
void InitSegDispTimer(void);

U8 IsExpiredDispTimer(U8 id);
U8 IsExpiredSegDispTimer(U8 id);

void SetDispTimer(U8 id, U16 time);
void SetSegDispTimer(U8 id, U16 time);

U8 IsExpiredDispBlinkTimer(U8 id);
U8 IsExpiredSegDispBlinkTimer(U8 id);

U8 BlinkLED(U8 onOff, void (*pFun)(U8 onOff), U8 id);
U8 BlinkSEG(U8 onOff,const char *str, U8 id);


void UpdateDisplayTimer(void);
void UpdateSegDisplayTimer(void);

#endif /* __DISPLAY_LIB_H__ */
