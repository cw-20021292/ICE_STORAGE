#ifndef __ROOM_WATER_H__
#define __ROOM_WATER_H__

#include "prj_type.h"
#include "level.h"

typedef struct _room_water_
{
    U8 InitFull;
    U8 Level;
    
    U16 FeedTime;
} RoomWater_T;

void  InitRoomWater(void);
void  GetRoomWaterData(RoomWater_T *pData);

void  SetRoomWaterLevel(U8 mu8Val );
U8    GetRoomWaterLevel(void);

/* ���� ��ũ ���� ������ */
void  SetRoomWaterInitFull(U8 mu8Val );

/* ���� ��ũ ���� ������ ���� Ȯ�� 
 * Return :
 *  TRUE : ���� ������ ���� O
 *  FALSE : ���� ������ ���� X
 */
U8    GetRoomWaterInitFull(void);

// ���� �Լ� ���� �ð�
void  SetRoomWaterFeedTime(U16 mu16Time );
U16   GetRoomWaterFeedTime(void);

void  ControlRoomWaterLevel(void);

#endif /* __ROOM_WATER_H__ */
