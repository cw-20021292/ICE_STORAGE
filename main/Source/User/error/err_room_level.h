#ifndef __ERR_ROOM_LEVEL_H__
#define __ERR_ROOM_LEVEL_H__

#include "prj_type.h"


#define  DEFAULT_ROOM_ERR_COUNT     3
void SetRoomErrCountLimit(U8 mu8Count);
U8 GetRoomErrCountLimit(void);


U8 CheckErrRoomOvf(U8 mu8Error);

U8 CheckErrRoomComplex(U8 mu8Error);

U8 CheckErrRoomLow(U8 mu8Error);

U8 CheckErrRoomHigh(U8 mu8Error);

U8 ReleaseErrRoomLow(U8 mu8Error);

#endif /* __ERR_ROOM_LEVEL_H__ */
