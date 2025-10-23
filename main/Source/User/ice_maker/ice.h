/**
 * File : ice.c
 * 
 * Ice Control
*/
#ifndef __ICE_H__
#define __ICE_H__

#include "prj_type.h"

typedef struct _ice_
{
    U8 Turbo;     // 쾌속 동작 설정 ( 0: 해제, 1: 설정 )

    /* MAKE */
    U8 ConfigMake;   // 제빙 동작 설정 ( 0: 해제, 1:설정 )
    U8 Make;         // 제빙 동작 ( 0: 중지, 1: 동작 )

    /* MODE */
    U16 Mode;         // 제빙 상세 동작 모드

    /* ICE FULL */
    U16 FullCheckTimer; // 만빙 검사 주기 타이머 @1sec
    U16 StopMakeTimer;  // 제빙 중지 타이머 @1sec

    U16 OutageTimer;    // 단수 타이머@1sec

    U8 DummyDeIcing;    // 더미 탈빙 동작 여부( 0: 중지, 1:동작 )

    U16 DeIcingCompTime;    // 강제 탈빙 동작 전 최소 압축기 운전 시간

    U8 IcingCount;          // 제빙횟수 카운트
} SIce_T;

void InitIce(void);

void GetIceData(SIce_T *pIce);

void SetIceTurbo(U8 val);
U8 GetIceTurbo(void);

void SetIceConfigMake(U8 val);
U8 GetIceConfigMake(void);

void SetIceMake(U8 val);
U8 GetIceMake(void);

// 제빙 운전 온도 검사 
U8 IsValidDoIcingAmbiTempRange(void);
U8 IsValidDoIcingColdTempRange(void);

/* MODE */
#define  ICE_MODE_NONE                (0x0000)
#define  ICE_MODE_INIT                (0x0001)
#define  ICE_MODE_PREHEAT             (0x0002)
#define  ICE_MODE_READY               (0x0004)
#define  ICE_MODE_ICING               (0x0008)
#define  ICE_MODE_DEICING             (0x0010)
#define  ICE_MODE_DUMMY_DEICING       (0x0020)
#define  ICE_MODE_OFF                 (0x0040)
#define  ICE_MODE_OFF_LOAD            (0x0080)
#define  ICE_MODE_DEICING_OFF         (0x0100)
#define  ICE_MODE_ICING_MOVE_DEICING  (0x0200)
#define  ICE_MODE_STORAGE             (0x0400)
#define  ICE_MODE_DEL_HUMIDITY        (0x0800)
#define  ICE_MODE_DEFROST             (0x1000)

#define  ICE_MODE_ALL   \
    ( ICE_MODE_INIT | \
      ICE_MODE_PREHEAT | \
      ICE_MODE_READY | \
      ICE_MODE_ICING | \
      ICE_MODE_DEICING | \
      ICE_MODE_ICING_MOVE_DEICING)
void SetIceMakeMode(U16 mode);
void ClearIceMakeMode(U16 mode);
U16 IsSetIceMakeMode(U16 mode);
U16 GetIceMakeMode(void );

// 만빙 검사 주기 시간 설정/업데이트
void SetIceFullCheckTimer(U16 val);

// 더미 탈빙 동작 여부 설정/반환
U8 GetIceDummyDeIcing( void );
void SetIceDummyDeIcing(U8 val);

// 강제 탈빙을 위한 최소 압축기 운전 시간
U16 GetDeIcingCompOnTime(void);
void ResetDeIcingCompOnTime(void);
void UpdateDeIcingCompOnTime(void);

void UpdateIcingCount(void);
void ClearIcingCount(void);
U8 IsMaxIcingCount(void);

void StartIceOutageTimer(void);


void SetIceTrayMovingRetry(void);
void StopIceMakeLoad(void);
void ClearIcingMoveDeicing(void);
// 더미 탈빙 
void SetDummyDeicing(void);

// 제빙 중지
U8 StopIceMake(void);

void  MakeIceFull(void);
void  MakeIce(void);

#endif /* __ICE_H__ */
