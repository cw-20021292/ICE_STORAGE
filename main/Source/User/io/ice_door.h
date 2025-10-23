#ifndef __ICE_DOOR_H__
#define __ICE_DOOR_H__

#include "prj_type.h"


void InitIceDoor(void);

// 일반 제어용 열림 감지 시간
void ClearIceDoorOpeningTime(void);
U16 GetIceDoorOpeningTime(void);
void SetIceDoorOpeningTime(U16 mu16Time);

// IOT용 열림 감지 시간
U16 GetIceDoorOpeningTimeIot(void);
void SetIceDoorOpeningTimeIot(U16 mu16Time);

void CountingIceDoorOpeningTime(void);

/* FORCED CLOSE DOOR */
#define  ICE_CLOSE_TIME    86400UL /* @1sec,  60sec x 60min x 24hour */
void SetIceDoorConfClosedTime(U32 mu16Time);
U32 GetIceDoorConfClosedTime(void);

#define  ICE_DOOR_OPEN     0     // FULL OPEND
#define  ICE_DOOR_OPENING  1     // OPEING..
#define  ICE_DOOR_CLOSE    2     // FULL CLOSED
#define  ICE_DOOR_CLOSING  3     // CLSOING...
U8 GetIceDoorStatus(void);

#define  ICE_DOOR_WATER_START    0     // WATER START
#define  ICE_DOOR_WATER_STATING  1     // WATER STATING
#define  ICE_DOOR_WATER_STOP     2     // WATER STOP
#define  ICE_DOOR_WATER_STOPING  3     // WATER STOPING
U8 GetIceDoorWaterStatus(void);

// 아이스 도어 추출 여부
// RETURN
// TRUE - 추출 중
// FALSE - 추출 중지
U8 IsOpenIceDoor(void);

U8 IsStartIceDoorWater(void);

void OpenIceDoor(void);
void CloseIceDoor(void);
void ForcedCloseIceDoor(void);

void ControlForcedCloseIceDoor(void);
void ControlIceDoor(void);

void OpenIceDoorWaterOut(void);
void CloseIceDoorWaterOut(void);


#if 0
void TestStartIceDoor(void);
void TestStopIceDoor(void);
#endif

void TEST_StartIceDoor(void);
void TEST_StopIceDoor(void);
void TEST_RepeatIceDoor(void);

#endif /* __ICE_DOOR_H__ */
