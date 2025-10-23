/**
 * File : api_input.h
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#ifndef __API_INPUT_H__
#define __API_INPUT_H__

#include "prj_type.h"

U8 GetPinModelType(void);

U8 GetPinTankCover(void);
U8 GetPinIceTankBracket(void);

U8 GetPinLevelRoomTankOverflow(void);
U8 GetPinLevelRoomTankLow(void);
U8 GetPinLevelRoomTankHigh(void);

U8 GetPinLevelColdTankLow(void);
U8 GetPinLevelColdTankHigh(void);

U8 GetPinLevelDrainTankLow(void);
U8 GetPinLevelDrainTankHigh(void);

#endif /* __API_INPUT_H__ */