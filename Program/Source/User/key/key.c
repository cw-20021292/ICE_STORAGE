/**
 * File : key.c
 * 
 * Key Scan Control
*/
#include "key.h"

//#include <string.h>

/* Status */
typedef enum _key_status_
{
    KEY_STATUS_NONE = 0,    // 0 :
    KEY_STATUS_PUSHED,      // 0 :
    KEY_STATUS_POP,         // 0 :
    KEY_STATUS_TIME_OUT     // 0 :
} EKeyStatus_T;

/* Event Timeout condition @10ms */
#define  EVENT_TIME_OUT_7S          (700)     // 7sec
#define  EVENT_TIME_OUT_5S          (500)     // 5sec
#define  EVENT_TIME_OUT_3S          (300)     // 3sec
#define  EVENT_TIME_OUT_2S          (200)     // 2sec
#define  EVENT_TIME_OUT_1S          (100)     // 1sec
#define  EVENT_TIME_OUT_SHORT       (5)       // 50ms
#define  MAX_EVENT_TIME_OUT         EVENT_TIME_OUT_7S

typedef struct _key_
{
    EKeyStatus_T status;  

    U32 val;           // PUSH KEY VAL
    U32 valPop;        // POP KEY VAL
    U16 pushedTime;    // PUSH TIME...
    U8 eventFlag;      // Event Flag..

    U8 SkipInvalidSound;   // Skip Invalid Sound On / Off ( Error Sound )
} SKey_T;

SKey_T key;

void InitKey(void)
{
    MEMSET( (void __FAR *)&key, 0, sizeof(SKey_T) );
}

U32 GetKeyVal(void)
{
    return key.val;
}

U32 GetKeyPopVal(void)
{
    return key.valPop;
}

void SetKeyEventFlag(U8 event)
{
    key.eventFlag |= event;
}

void ClearKeyEventFlag(U8 event)
{
    key.eventFlag &= ~event;
}

U8 IsSetKeyEventFlag(U8 event)
{
    if( (key.eventFlag & event) ==  event )
    {
        return TRUE;
    }

    return FALSE;
}

void SetSkipInvalidSound(U8 skip)
{
    key.SkipInvalidSound = skip;
}

U8 GetSkipInvalidSound(void)
{
    return key.SkipInvalidSound;
}

/**
 * KEY SCAN 함수
 *
 * Description :
 *  -. KEY 버튼 값을 읽어 온다.
 *  -. 각 KEY 버튼 입력에 대한 EVENT를 설정한다.
 *  -. EVENT는 KEY_EVENT 아래와 같이 7개의 이벤트가 있다.
 *       KEY_EVENT_PUSHED_SHORT  
 *       KEY_EVENT_PUSHED_LONG_7S
 *       KEY_EVENT_PUSHED_LONG_5S
 *       KEY_EVENT_PUSHED_LONG_3S
 *       KEY_EVENT_PUSHED_LONG_2S
 *       KEY_EVENT_PUSHED_LONG_1S
 *       KEY_EVENT_POP           
*/
typedef struct _key_event_
{
    U16 eventTime;
    U8 event;
} SKeyEvent_T;

static SKeyEvent_T keyEventList[] = 
{
    { EVENT_TIME_OUT_7S,    KEY_EVENT_PUSHED_LONG_7S },
    { EVENT_TIME_OUT_5S,    KEY_EVENT_PUSHED_LONG_5S },
    { EVENT_TIME_OUT_3S,    KEY_EVENT_PUSHED_LONG_3S },
    { EVENT_TIME_OUT_2S,    KEY_EVENT_PUSHED_LONG_2S },
    { EVENT_TIME_OUT_1S,    KEY_EVENT_PUSHED_LONG_1S },
    { EVENT_TIME_OUT_SHORT, KEY_EVENT_PUSHED_SHORT   },
};
#define SZ_KEY_EVENT_LIST   ( sizeof(keyEventList)/sizeof(SKeyEvent_T) )

static void FindSetKeyEvent(U16 pushedTime)
{
    U8 i = 0U;

    for( i=0; i<SZ_KEY_EVENT_LIST; i++ )
    {
        if( keyEventList[i].eventTime == pushedTime )
        {
            SetKeyEventFlag(keyEventList[i].event);
            return ;
        }
    }
}

void ProcessScanKey(void)
{
    static U32 prevKeyVal = 0UL;
    U32 keyVal = 0UL;

    // 1. Get Key value 
    keyVal = GetKey();

    if( prevKeyVal != keyVal )
    {
        if( keyVal == KEY_NONE )
        {
            key.valPop = prevKeyVal;
        }

        prevKeyVal = keyVal;
        key.val = keyVal;  

        if( keyVal != KEY_NONE )
        {
            key.status = KEY_STATUS_PUSHED;
            key.pushedTime = 0;  
            ClearKeyEventFlag(KEY_EVENT_ALL);
        }
        else 
        {
            if( key.pushedTime > EVENT_TIME_OUT_SHORT )
            {
                key.status = KEY_STATUS_POP;
                SetKeyEventFlag(KEY_EVENT_POP);
            }
            else
            {
                key.status = KEY_STATUS_NONE;
            }
        }
    }
    
    if( key.status == KEY_STATUS_PUSHED )
    {
        key.pushedTime++;
        FindSetKeyEvent(key.pushedTime);

        if( key.pushedTime > MAX_EVENT_TIME_OUT )
        {
            key.status = KEY_STATUS_TIME_OUT;
        }
    }
}
