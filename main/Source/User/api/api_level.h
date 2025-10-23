/**
 * File : api_level.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_LEVEL_H__
#define __API_LEVEL_H__

#include "prj_type.h"

/// 수위센서 사용 여부
#define CONFIG_USE_LEVEL_ROOM_TANK    (1)
#define CONFIG_USE_LEVEL_HOT_TANK     (1)
#define CONFIG_USE_LEVEL_COLD_TANK    (1)
#define CONFIG_USE_LEVEL_DRAIN_TANK   (1)
#define CONFIG_USE_LEVEL_ICE_TRAY     (1)

/// 수위 센서 감지 플래그
#define LEVEL_DETECT_NONE         (0x00)    /// 센서 감지 없음
#define LEVEL_DETECT_LOW          (0x01)    /// 저수위 센서 감지
#define LEVEL_DETECT_HIGH         (0x02)    /// 만수위 센서 감지
#define LEVEL_DETECT_OVF          (0x04)    /// 물넘침 센서 감지

/// 수위 센서 상태
#define LEVEL_STATUS_LOW          ( LEVEL_DETECT_NONE | LEVEL_DETECT_NONE )
#define LEVEL_STATUS_MID          ( LEVEL_DETECT_LOW | LEVEL_DETECT_NONE )
#define LEVEL_STATUS_ERR_LOW      ( LEVEL_DETECT_NONE | LEVEL_DETECT_HIGH )
#define LEVEL_STATUS_HIGH         ( LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH )

#define LEVEL_STATUS_ERR_COMPLEX  ( LEVEL_DETECT_OVF | LEVEL_STATUS_LOW )
#define LEVEL_STATUS_ERR_HIGH     ( LEVEL_DETECT_OVF | LEVEL_STATUS_MID )
#define LEVEL_STATUS_ERR_OVF_LOW  ( LEVEL_DETECT_OVF | LEVEL_STATUS_ERR_LOW )
#define LEVEL_STATUS_OVF          ( LEVEL_DETECT_OVF | LEVEL_STATUS_HIGH )

#if ( CONFIG_USE_LEVEL_ROOM_TANK == 1)
/// 정수 탱크 수위
U8 GetLevelRoomTank(void);
#endif

#if ( CONFIG_USE_LEVEL_HOT_TANK == 1)
/// 온수 탱크 수위
U8 GetLevelHotTank(void);
#endif

#if ( CONFIG_USE_LEVEL_COLD_TANK == 1)
/// 냉수 탱크 수위
U8 GetLevelColdTank(void);
#endif

#if ( CONFIG_USE_LEVEL_DRAIN_TANK == 1)
/// 드레인 탱크 수위
U8 GetLevelDrainTank(void);
#endif

#if ( CONFIG_USE_LEVEL_ICE_TRAY == 1)
/// 아이스 트레이 수위
U8 GetLevelIceTray(void);
#endif

#endif  /* __API_LEVEL_H__ */
