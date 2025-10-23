/**
 * File : lever.h
 * 
 * Lever
*/
#ifndef __LEVER_H__
#define __LEVER_H__

#include "api_key.h"

/* Lever ID */
#define  LEVER_ID_WATER           (0)
#define  LEVER_ID_ICE             (1)
#define  LEVER_ID_ICE_AMOUNT      (2)
#define  LEVER_ID_MAX             (3)

void InitLever(void);

U16 GetLeverVal(U8 id);

U16 GetLeverPopVal(U8 id);

/* EvnetFlag Index */
#define  LEVER_EVENT_PUSHED_SHORT     (0x01)
#define  LEVER_EVENT_PUSHED_LONG_2S   (0x02)
#define  LEVER_EVENT_PUSHED_LONG_8S   (0x04)
#define  LEVER_EVENT_POP              (0x08)
#define  LEVER_EVENT_PUSEHD_ALL       (0x07)
#define  LEVER_EVENT_ALL              (0x0F)
void SetLeverEventFlag(U8 id, U8 event);
void ClearLeverEventFlag(U8 id, U8 event);
U8 IsSetLeverEventFlag(U8 id,  U8 event);

void ProcessScanLever(void);

#endif /* __LEVER_H__ */
