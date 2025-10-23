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
#define DISP_TIMER_ID_500MS        (0)    /* LED 점멸 타이머 0.5sec */
#define DISP_TIMER_ID_1SEC         (1)    /* LED 점멸 타이머 1sec */
#define DISP_TIMER_ID_3SEC         (2)    /* LED 점멸 타이머 3sec, 1sec off */
#define DISP_TIMER_ID_4SEC         (3)    /* LED 점멸 타이머 4sec */
#define DISP_TIMER_ID_100MS        (4)    /* LED 점멸 타이머 0.1sec */
#define DISP_TIMER_ID_BLINK_MAX    (5)

typedef enum _disp_timer_id_
{
    DISP_TIMER_ID_KEY_AMOUNT,         /* 물양 선택 */
    DISP_TIMER_ID_KEY_ICE,            /* 얼음 선택 */
    DISP_TIMER_ID_KEY_WATER,          /* 물 선택 */
    DISP_TIMER_ID_KEY_HOT,            /* 온수 선택 */
    DISP_TIMER_ID_KEY_COLD,           /* 냉수 선택 */
    DISP_TIMER_ID_KEY_HOT_TEMP,       /* 온수 선택 */
    DISP_TIMER_ID_CONFIG_TIME,        /* 설정 진입 금지 타이머 */
    DISP_TIMER_ID_ALARM_SETTING,

    DISP_TIMER_ID_HOT_STATUS,          /* 온수 온도 상태 */
    DISP_TIMER_ID_COLD_STATUS,         /* 냉수 온도 상태 */

    DISP_TIMER_ID_CONFIG_AMOUNT,       /* 물양 설정 */
    DISP_TIMER_ID_VERSION,             /* 버전 표시 시간 */
    DISP_TIMER_ID_MEMENTO,             /* 메멘토 타이머 */
    DISP_TIMER_ID_SLEEP,              

    DISP_TIMER_ID_COLDMAKE,            /* 냉수 꺼짐 후 선택 시   점멸 표시 시간 */
    DISP_TIMER_ID_HOTMAKE,             /* 온수 꺼짐 후 선택 시   점멸 표시 시간 */
    DISP_TIMER_ID_ICEMAKE,             /* 얼음 꺼짐 후 선택 시 점멸 표시 시간 */

    DISP_TIMER_ID_HOTLOCK_SET,         /* HOT LOCK 설정 시 점멸 표시 시간 */ 
    DISP_TIMER_ID_ICELOCK_SET,         /* ICE LOCK 설정 시 점멸 표시 시간 */ 
    DISP_TIMER_ID_ICEWATERLOCK_SET,    /* ICE LOCK 설정 시 점멸 표시 시간 */ 
    DISP_TIMER_ID_ALLLOCK_SET,         /* ALL LOCK 설정 시 점멸 표시 시간 */ 

    DISP_TIMER_ID_HOTLOCK,             /* HOT LOCK 점멸 표시 시간 */ 
    DISP_TIMER_ID_ICELOCK,             /* ICE LOCK 점멸 표시 시간 */ 
    DISP_TIMER_ID_ICEWATERLOCK,        /* ICE WATER LOCK 점멸 표시 시간 */ 
    DISP_TIMER_ID_ICEWATERAMOUNTLOCK,  /* ICE WATER AMOUNT LOCK 점멸 표시 시간 */
    DISP_TIMER_ID_ICEWATERAMOUNTNOTI,  /* ICE / ICE WATER AMOUNT NOTI */
    DISP_TIMER_ID_NOT_ICEWATER,        /* 얼음물 추출 제한 NOTI */
    DISP_TIMER_ID_NOT_COLDWATER,       /* 냉수 추출 제한 NOTI */
    DISP_TIMER_ID_ALLLOCK,             /* ALL LOCK 점멸 표시 시간 */ 
    DISP_TIMER_ID_STERLOCK,            /* STER LOCK 점멸 표시 시간 */

    DISP_TIMER_ID_ERROR,             
    DISP_TIMER_ID_HOT_ALTITUDE,        /* 온수고도 설정 EXIT 타이머 */
    DISP_TIMER_ID_UNUSED_SAVE,         /* 미사용 절전 설정 EXIT 타이머 */
    DISP_TIMER_ID_DRAIN_WATER,         /* 자동 배수 설정 EXIT 타이머 */

    DISP_TIMER_ID_SETTING_MENU,        /* 설정 메뉴 EXIT 타이머 */
    DISP_TIMER_ID_SOUND_MENU,          /* 사운드 설정 EXIT 타이머 */
    DISP_TIMER_ID_CUSTOMER_MENU,       /* 사운드 설정 EXIT 타이머 */
    DISP_TIMER_ID_WIFI_AP_ERROR,       /* WIFI - AP ERROR CODE */

    DISP_TIMER_SEGON,              /* 기능 켰을 때 SEG  ON 표시 */
    DISP_TIMER_SEGOFF,             /* 기능 껐을 때 SEG OFF 표시 */

    DISP_TIMER_SEGOFF_BLINK,        /* 기능 껐는데 해당 기능 선택 했을 때 SEG OFF 2번 점멸 표시 */

    DISP_TIMER_ID_WATER_SELCET,     /* 온수, 정수, 냉수 선택 시 칼라 표시 지연 */
    DISP_TIMER_ID_ICE_SELCET,       /* 얼음 ,얼음물 선택 시 칼라 표시 지연 */

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
