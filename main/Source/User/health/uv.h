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
    UV_STER_ICE_DOOR,            // ���� ���
    UV_STER_ICE_DOOR_SHOT,       // ���� ���� ���� ��    ( AIS MAX�� ������� ������ ���� ���ϱ�� �� )

    UV_STER_WATER_OUT,           // ���� ���
    UV_STER_WATER_OUT_SHOT,      // �� ���� ���� ��    ( AIS MAX�� ������� ������ ���� ���ϱ�� �� )

    UV_STER_ICE_TRAY,            // ���� �� ��,
    UV_STER_ICE_TRAY_SHOT,       // Ż�� ���� ��,

    UV_STER_ICE_TANK,            // ���� ���

    UV_STER_DRAIN_TANK,          // ���� ���
    
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
