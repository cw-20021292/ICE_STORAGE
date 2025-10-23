/**
 * File : uv.h
 * 
 * Uv
*/
#ifndef __UV_H__
#define __UV_H__
  
#include "prj_type.h"
  
// UV ID 
typedef enum _uv_ster_
{
    UV_STER_ICE_DOOR,            // 정기 살균
    UV_STER_ICE_DOOR_SHOT,       // 얼음 추출 종료 후    ( AIS MAX는 살균인증 문제로 제어 안하기로 함 )

    UV_STER_WATER_OUT,           // 정기 살균
    UV_STER_WATER_OUT_SHOT,      // 물 추출 종료 후    ( AIS MAX는 살균인증 문제로 제어 안하기로 함 )

    UV_STER_ICE_TRAY,            // 만빙 일 때,
    UV_STER_ICE_TRAY_SHOT,       // 탈빙 중이 때,

    UV_STER_ICE_TANK,            // 정기 살균

    UV_STER_DRAIN_TANK,          // 정기 살균
    
    UV_STER_MAX
} SUvSter_T;

// config
void SetConfigUvId(SUvSter_T uv, U8 config);
U8 GetConfigUvId(SUvSter_T uv);

// @100ms
void TurnOnUvId(SUvSter_T uv, U32 offTime, U32 onTime);
void TurnOffUvId(SUvSter_T uv);
U8  IsTurnOnUvId(SUvSter_T uv);

// status 
// - true : pause,
// - false : run
void SetPauseUvId(SUvSter_T uv, U8 status);
U8 GetPauseUvId(SUvSter_T uv);

U8 GetUvOnPercent(SUvSter_T id);

// IsLimited
#define UNLIMITED   (0)
#define LIMITED     (1)
void SetLimitConfUvId(SUvSter_T uv, U8 isLimited);

void SetLimitCountUvId(SUvSter_T uv, U8 count);

void ControlUv(void);

#endif   /* __UV_H__ */
