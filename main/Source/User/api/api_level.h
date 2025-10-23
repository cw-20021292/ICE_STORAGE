/**
 * File : api_level.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_LEVEL_H__
#define __API_LEVEL_H__

#include "prj_type.h"

/// �������� ��� ����
#define CONFIG_USE_LEVEL_ROOM_TANK    (1)
#define CONFIG_USE_LEVEL_HOT_TANK     (1)
#define CONFIG_USE_LEVEL_COLD_TANK    (1)
#define CONFIG_USE_LEVEL_DRAIN_TANK   (1)
#define CONFIG_USE_LEVEL_ICE_TRAY     (1)

/// ���� ���� ���� �÷���
#define LEVEL_DETECT_NONE         (0x00)    /// ���� ���� ����
#define LEVEL_DETECT_LOW          (0x01)    /// ������ ���� ����
#define LEVEL_DETECT_HIGH         (0x02)    /// ������ ���� ����
#define LEVEL_DETECT_OVF          (0x04)    /// ����ħ ���� ����

/// ���� ���� ����
#define LEVEL_STATUS_LOW          ( LEVEL_DETECT_NONE | LEVEL_DETECT_NONE )
#define LEVEL_STATUS_MID          ( LEVEL_DETECT_LOW | LEVEL_DETECT_NONE )
#define LEVEL_STATUS_ERR_LOW      ( LEVEL_DETECT_NONE | LEVEL_DETECT_HIGH )
#define LEVEL_STATUS_HIGH         ( LEVEL_DETECT_LOW | LEVEL_DETECT_HIGH )

#define LEVEL_STATUS_ERR_COMPLEX  ( LEVEL_DETECT_OVF | LEVEL_STATUS_LOW )
#define LEVEL_STATUS_ERR_HIGH     ( LEVEL_DETECT_OVF | LEVEL_STATUS_MID )
#define LEVEL_STATUS_ERR_OVF_LOW  ( LEVEL_DETECT_OVF | LEVEL_STATUS_ERR_LOW )
#define LEVEL_STATUS_OVF          ( LEVEL_DETECT_OVF | LEVEL_STATUS_HIGH )

#if ( CONFIG_USE_LEVEL_ROOM_TANK == 1)
/// ���� ��ũ ����
U8 GetLevelRoomTank(void);
#endif

#if ( CONFIG_USE_LEVEL_HOT_TANK == 1)
/// �¼� ��ũ ����
U8 GetLevelHotTank(void);
#endif

#if ( CONFIG_USE_LEVEL_COLD_TANK == 1)
/// �ü� ��ũ ����
U8 GetLevelColdTank(void);
#endif

#if ( CONFIG_USE_LEVEL_DRAIN_TANK == 1)
/// �巹�� ��ũ ����
U8 GetLevelDrainTank(void);
#endif

#if ( CONFIG_USE_LEVEL_ICE_TRAY == 1)
/// ���̽� Ʈ���� ����
U8 GetLevelIceTray(void);
#endif

#endif  /* __API_LEVEL_H__ */
