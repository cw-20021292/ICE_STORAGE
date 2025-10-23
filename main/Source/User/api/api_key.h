/**
 * File : api_key.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_KEY_H__
#define __API_KEY_H__

#include "prj_type.h"

#define KEY_NONE       (0x00000000UL)

/// Front Key
#define KEY_1           (0x00000001UL)      /// Front - ALL LOCK
#define KEY_2           (0x00000002UL)      /// Front - COLD OFF
#define KEY_3           (0x00000004UL)      /// Front - HOT OFF
#define KEY_4           (0x00000008UL)      /// Front - HOT LOCK
#define KEY_5           (0x00000010UL)      /// Front - ICE OFF
#define KEY_6           (0x00000020UL)      /// Front - ICE WATER LOCK
#define KEY_7           (0x00000040UL)      /// Front - ICE LOCK
#define KEY_8           (0x00000080UL)      /// Front - ICE WATER AMOUNT
#define KEY_9           (0x00000100UL)      /// Front - HOT
#define KEY_10          (0x00000200UL)      /// Front - ROOM
#define KEY_11          (0x00000400UL)      /// Front - COLD
#define KEY_12          (0x00000800UL)      /// Front - AMOUNT
#define KEY_13          (0x00001000UL)      /// Front - ICE
#define KEY_14          (0x00002000UL)      /// Front - ICE WATER

/// Out Key
#define KEY_15          (0x00004000UL)      // Front - OUT ICE
#define KEY_16          (0x00008000UL)      // Front - OUT WATER

#define FRONT_KEY       ( \
                        KEY_1|KEY_2|KEY_3|KEY_4|KEY_5 \
                        KEY_6|KEY_7|KEY_8|KEY_9|KEY_10 \
                        KEY_11|KEY_12|KEY_13|KEY_14 \
                        )
#define OUT_KEY         (KEY_15|KEY_16)
#define KEY_LEVER_ICE   (KEY_15)
#define KEY_LEVER_WATER (KEY_16)

#define KEY_UNUSED      (KEY_1|KEY_2|KEY_6|KEY_7)
#define KEY_ALL         (0x0000FFFFUL & ~KEY_UNUSED)

void InitApiKey(void);

void SetFrontKey(U32 key);
U32 GetFrontKey(void);

U32 GetKey(void);

void SetLeverWater(U8 lever);
U8 GetLeverWater(void);

void SetLeverIce(U8 lever);
U8 GetLeverIce(void);

U8 GetLeverIceAmount(void);

#endif  /* __API_KEY_H__ */
