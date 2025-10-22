
#ifndef __ICE_MAKE_H__
#define __ICE_MAKE_H__

#include "r_smc_entry.h"
#include "Type_Define.h"

typedef enum {
    ICE_MODE_DUMMY_DETACH = 0x01,
    ICE_MODE_INIT = 0x02,
    ICE_MODE_PREHEAT = 0x04,
    ICE_MODE_READY = 0x08,
    ICE_MODE_MAKE = 0x10,
    ICE_MODE_DETACH = 0x20,
    ICE_MODE_TRAY_RETRY = 0x40,     // 안쓸수도

    // ICE_MODE_INIT = 0x40,
    // ICE_MODE_INIT = 0x80,
} ICE_MODE;

typedef enum {
    ICE_TRAY_DOWN,          // 트레이 탈빙 이동
    ICE_TRAY_DOWN_CHECK,    // 트레이 탈빙 이동완료 확인 (트레이 이동에러 체크)
    COMP_STATUS_CHECK,      // 컴프레셔 동작 가능 상태 확인
    ICE_SWITCH,             // 냉매 제빙 위치 이동
    ICE_MELTING,            // 얼음 녹이기 시간적용
    HOT_GAS_SWITCH,         // 냉매 핫가스로 이동
    HOT_GAS_HZ_SET,         // 핫가스 HZ 적용
    HOT_GAS_ON,             // 핫가스 ON (시간 적용)
    DUMMY_DETACH_COMPLETE   // 더미탈빙 끝    
} ICE_DUMMY_DETACH_STEP;

/* INIT */
typedef enum
{
    STEP_INIT_TRAY_MOVE,        // 트레이 제빙 위치로 이동
    STEP_INIT_TRAY_DEICING,     // 트레이 제빙 위치 체크 (트레이 이동에러 체크)
    STEP_INIT_TRAY_DONE         // 트레이 이동 완료
} ICE_INIT_STEP;

void SetIceMode(U8 u8_p_mode);
void ClearIceMode(U8 u8_p_mode);
U8 GetIceMode(void);

void IceInit(void);
U8 ProcessIceInit(void);

#endif
