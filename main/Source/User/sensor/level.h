/**
 * File : level.h
 * 
 * Level
*/
#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "prj_type.h"

#include "api_level.h"

/// LEVEL
#define  LEVEL_LOW            LEVEL_STATUS_LOW        
#define  LEVEL_MID            LEVEL_STATUS_MID        
#define  LEVEL_ERR_LOW        LEVEL_STATUS_ERR_LOW    
#define  LEVEL_HIGH           LEVEL_STATUS_HIGH       
#define  LEVEL_ERR_COMPLEX    LEVEL_STATUS_ERR_COMPLEX
#define  LEVEL_ERR_HIGH       LEVEL_STATUS_ERR_HIGH   
#define  LEVEL_ERR_OVF_LOW    LEVEL_STATUS_ERR_OVF_LOW
#define  LEVEL_OVF            LEVEL_STATUS_OVF        

/*ERROR TYPE */
#define  ERR_TYPE_LOW       (0U)
#define  ERR_TYPE_COMPLEX   (1U)
#define  ERR_TYPE_HIGH      (2U)
#define  ERR_TYPE_OVF       (3U)
#define  ERR_TYPE_MAX       (4U)

/// ���� ���� ���� �ð� �ʱ�ȭ
void InitTankConfDetectTime(void);

/// ���� ���� ���� �ð� ����
void SetTankConfDetectTime(U8 id, U8 detectTime);

/// ���� ���� ���� �ð� ��ȯ
U8 GetTankConfDetectTime(U8 id);

U8 GetTankLevelErrorCount(U8 id, U8 errType);
void SetTankLevelErrorCount(U8 id, U8 errType, U8 count);
void ClearTankLevelErrorCount(void);

/// TANK LEVEL ID 
#define  LEVEL_ID_ROOM        (0U)
#define  LEVEL_ID_COLD        (1U)
#define  LEVEL_ID_HOT         (2U)
#define  LEVEL_ID_DRAIN       (3U)
#define  LEVEL_ID_ICE_TRAY    (4U)
#define  LEVEL_ID_MAX         (5U)
U8 GetTankLevel(U8 id);

/// ��ũ ���� ���� ���� ��ȯ
/// Return :
/// TRUE : ����
/// FALSE : �̰���
U8 IsDetectTankLevel(U8 id, U8 level);

/// ��ũ ������ ���� ��ȯ
/// Return :
///  TRUE : ��ũ ����
///  FALSE : ��ũ ���� �ƴ�
U8 IsTankLevelFull(U8 id);

/// ��ũ ���� ���� ���� �˻�
/// Return :
///  TRUE : ����
///  FALSE : ����
U8 IsErrorTankLevel(U8 level);

void ProcessTankLevel(void);

#endif /* __LEVEL_H__ */
