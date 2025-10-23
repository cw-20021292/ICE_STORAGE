#ifndef __FRONT_H__
#define __FRONT_H__

#include "prj_type.h"

typedef enum _deco_led_
{
    DECO_LED_WELCOME_ICE = 0,    /// 0 :
    DECO_LED_WELCOME_WATER,      /// 0 :
    DECO_LED_MAX,                /// 0 :
} EDecoLed_T;

void InitFront(void);

void SetFactroySetup(U8 setup);
U8 GetFactorySetup(void);
void CheckFactroySetup(void);

void SetConfigDeco(U8 id, U8 onOff);
U8 GetConfigDeco(U8 id);

void SetLockAll(U8 lock);
U8 GetLockAll(void);

void SetLockHot(U8 lock);
U8 GetLockHot(void);

void SetLockIce(U8 lock);
U8 GetLockIce(void);

void SetLockIceWater(U8 lock);
U8 GetLockIceWater(void);

void SetLockIceWaterAmount(U8 lock);
U8 GetLockIceWaterAmount(void);

// 정수기 미사용 시간
// 미사용 시간(분)을 반환
U32 GetSystemUnusedTime(void);  
void ResetUnusedTime(void);
void CountingUnusedTime(void);

#endif /* __FRONT_H__ */
