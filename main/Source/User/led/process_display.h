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

/* �ʱ� FRONT �ʱ� ���� �Ϸ� ���� */
U8 GetDisplayInit(void);
void StartDisplayInit(void);

// �¼� ��� ���� ���� ����/����
void StartDisplayHotLockSet(void);
void StopDisplayHotLockSet(void);

// �¼� ��� ���� ����/����
void StartDisplayHotLock(void);
void StopDisplayHotLock(void);

// ���� ��� ���� ���� ����/����
void StartDisplayIceLockSet(void);
void StopDisplayIceLockSet(void);

// ���� ��� ���� ����/����
void StartDisplayIceLock(void);
void StopDisplayIceLock(void);

// ������ ��� ���� ���� ����/����
void StartDisplayIceWaterLockSet(void);
void StopDisplayIceWaterLockSet(void);

// ������ ��� ���� ����/����
void StartDisplayIceWaterLock(void);
void StopDisplayIceWaterLock(void);

// ������ �뷮 ��� ���� ����/����
void StartDisplayIceWaterAmountLock(void);
void StopDisplayIceWaterAmountLock(void);

/// ���� ���� �� ������ �뷮 ����  ��, �˸�
void StartDisplayIceWaterAmountNoti(void);
void StopDisplayIceWaterAmountNoti(void);

/// ������ ���� ���� �˸�
void StartDisplayNotIceWater(void);
void StopDisplayNotIceWater(void);

/// �ü� ���� ���� �˸�
void StartDisplayNotColdWater(void);
void StopDisplayNotColdWater(void);

// ��ü ��� ���� ���� ����/����
void StartDisplayLockAllSet(void);
void StopDisplayLockAllSet(void);

// ��ü ��� ���� ����/����
void StartDisplayLockAll(void);
void StopDisplayLockAll(void);

// ���� ���� ���� ����/����
void StartDisplayIceMake(void);
void StopDisplayIceMake(void);

// �ü� ���� ���� ����/����
void StartDisplayColdMake(void);
void StopDisplayColdMake(void);

// �¼� ���� ���� ����/����
void StartDisplayHotMake(void);
void StopDisplayHotMake(void);

void StartDisplayKeyIce(void);
void StopDisplayKeyIce(void);
void StartDisplayKeyWater(void);
void StopDisplayKeyWater(void);

/// "COLOR" LED ��� ����
void StartDisplayWaterColorWait(void);
void StopDisplayWaterColorWait(void);
void StartDisplayIceColorWait(void);
void StopDisplayIceColorWait(void);

// SEG ON ����/����
void StartSegDisplaySegOn(void);
void StopSegDisplaySegOn(void);

// SEG OFF ����/����
void StartSegDisplaySegOff(void);
void StopSegDisplaySegOff(void);

// SEG OFF ���� ����/����
void StartSegDisplaySegOffBlink(void);
void StopSegDisplaySegOffBlink(void);

// Fota Set / Clear
void SetDisplayFota(U8 fota);
U8 GetDisplayFota(void);

// ��� �� ��ư Lock ���� ���� / ����
void StartDisplaySter(void);
void StopDisplaySter(void);

/* �¼� �� ���� */
U8 GetAltidueConfig(void);
void SetAltitudeConfig(U8 conf);

/* �ڵ� ��� ����/���� */
U8 GetDispDrainWaterConfig(void);
void SetDispDrainWaterConfig(U8 conf);

/* ���� ��� ���� ��ȿ�� Ÿ�̸� */
U16 IsExpiredConfigTime(void);

/* ���� ǥ�� */
void SetVersionDisp(U8 sec);

/* ���� �޴� */
U8 GetSettingDisp(void);
void SetSettingDisp(U8 conf);

/* �޸��� */
U8 GetMementoDisp(void);
void SetMementoDisp(U8 conf);

/* ���÷��� ���� */
void ProcessDisplay(void);

#endif /* __PROCESS_DISPLAY_H__ */
