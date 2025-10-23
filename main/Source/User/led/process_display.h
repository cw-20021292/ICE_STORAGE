/**
 * File : process_display.h
 * 
 * Process Led Display
*/
#ifndef __PROCESS_DISPLAY_H__
#define __PROCESS_DISPLAY_H__

#include "prj_type.h"

void InitDisplay(void);

void StartDispTimerId(U8 id);
void StopDispTimerId(U8 id);

void StartSegDispTimerId(U8 id);
void StopSegDispTimerId(U8 id);

void SetTempMonitoring(U8 monitoring);
U8 GetTempMonitoring(void);

void SelectIcePanel(void);
void SelectWaterPanel(void);

/* 초기 FRONT 초기 점멸 완료 상태 */
U8 GetDisplayInit(void);
void StartDisplayInit(void);

// 온수 잠금 설정 점멸 시작/정지
void StartDisplayHotLockSet(void);
void StopDisplayHotLockSet(void);

// 온수 잠금 점멸 시작/정지
void StartDisplayHotLock(void);
void StopDisplayHotLock(void);

// 얼음 잠금 설정 점멸 시작/정지
void StartDisplayIceLockSet(void);
void StopDisplayIceLockSet(void);

// 얼음 잠금 점멸 시작/정지
void StartDisplayIceLock(void);
void StopDisplayIceLock(void);

// 얼음물 잠금 설정 점멸 시작/정지
void StartDisplayIceWaterLockSet(void);
void StopDisplayIceWaterLockSet(void);

// 얼음물 잠금 점멸 시작/정지
void StartDisplayIceWaterLock(void);
void StopDisplayIceWaterLock(void);

// 얼음물 용량 잠금 점멸 시작/정지
void StartDisplayIceWaterAmountLock(void);
void StopDisplayIceWaterAmountLock(void);

/// 얼음 선택 때 얼음물 용량 선택  시, 알림
void StartDisplayIceWaterAmountNoti(void);
void StopDisplayIceWaterAmountNoti(void);

/// 얼음물 추출 제한 알림
void StartDisplayNotIceWater(void);
void StopDisplayNotIceWater(void);

/// 냉수 추출 제한 알림
void StartDisplayNotColdWater(void);
void StopDisplayNotColdWater(void);

// 전체 잠금 설정 점멸 시작/정지
void StartDisplayLockAllSet(void);
void StopDisplayLockAllSet(void);

// 전체 잠금 점멸 시작/정지
void StartDisplayLockAll(void);
void StopDisplayLockAll(void);

// 얼음 꺼짐 점멸 시작/정지
void StartDisplayIceMake(void);
void StopDisplayIceMake(void);

// 냉수 꺼짐 점멸 시작/정지
void StartDisplayColdMake(void);
void StopDisplayColdMake(void);

// 온수 꺼짐 점멸 시작/정지
void StartDisplayHotMake(void);
void StopDisplayHotMake(void);

void StartDisplayKeyIce(void);
void StopDisplayKeyIce(void);
void StartDisplayKeyWater(void);
void StopDisplayKeyWater(void);

/// "COLOR" LED 대기 시작
void StartDisplayWaterColorWait(void);
void StopDisplayWaterColorWait(void);
void StartDisplayIceColorWait(void);
void StopDisplayIceColorWait(void);

// SEG ON 시작/정지
void StartSegDisplaySegOn(void);
void StopSegDisplaySegOn(void);

// SEG OFF 시작/정지
void StartSegDisplaySegOff(void);
void StopSegDisplaySegOff(void);

// SEG OFF 점멸 시작/정지
void StartSegDisplaySegOffBlink(void);
void StopSegDisplaySegOffBlink(void);

// Fota Set / Clear
void SetDisplayFota(U8 fota);
U8 GetDisplayFota(void);

// 살균 중 버튼 Lock 점멸 시작 / 정지
void StartDisplaySter(void);
void StopDisplaySter(void);

/* 온수 고도 설정 */
U8 GetAltidueConfig(void);
void SetAltitudeConfig(U8 conf);

/* 자동 배수 설정/해제 */
U8 GetDispDrainWaterConfig(void);
void SetDispDrainWaterConfig(U8 conf);

/* 설정 모드 진입 유효성 타이머 */
U16 IsExpiredConfigTime(void);

/* 버전 표시 */
void SetVersionDisp(U8 sec);

/* 설정 메뉴 */
U8 GetSettingDisp(void);
void SetSettingDisp(U8 conf);

/* 메멘토 */
U8 GetMementoDisp(void);
void SetMementoDisp(U8 conf);

/* 디스플레이 제어 */
void ProcessDisplay(void);

#endif /* __PROCESS_DISPLAY_H__ */
