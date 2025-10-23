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

/// 수위 센서 감지 시간 초기화
void InitTankConfDetectTime(void);

/// 센서 감지 지연 시간 설정
void SetTankConfDetectTime(U8 id, U8 detectTime);

/// 센서 감지 지연 시간 반환
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

/// 탱크 센서 감지 여부 반환
/// Return :
/// TRUE : 감지
/// FALSE : 미감지
U8 IsDetectTankLevel(U8 id, U8 level);

/// 탱크 만수위 여부 반환
/// Return :
///  TRUE : 탱크 가득
///  FALSE : 탱크 가득 아님
U8 IsTankLevelFull(U8 id);

/// 탱크 센서 에러 상태 검사
/// Return :
///  TRUE : 에러
///  FALSE : 정상
U8 IsErrorTankLevel(U8 level);

void ProcessTankLevel(void);

#endif /* __LEVEL_H__ */
