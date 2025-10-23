/**
 * File : fct.h
 * 
 * Fct
*/
#ifndef __FCT_H__
#define __FCT_H__

#include "prj_type.h"

void InitFct(void);

void StartFct(void);
void StopFct(void);
U8 IsExpiredFctLimitTimer(void);
U8 GetFctStatus(void);

void SetFctTestTimer(U16 time);
U16 GetFctTestTimer(void);

// FCT INPUT TEST MASK BIT
// FRONT KEY
#define MK_FCT_KEY_ICE               (0x00000001UL)
#define MK_FCT_KEY_HOT               (0x00000002UL)
#define MK_FCT_KEY_ROOM              (0x00000004UL)
#define MK_FCT_KEY_COLD              (0x00000008UL)
#define MK_FCT_KEY_AMOUNT            (0x00000010UL)
#define MK_FCT_KEY_ICE_WATER         (0x00000020UL)
#define MK_FCT_KEY_ICE_WATER_AMOUNT  (0x00000040UL)
#define MK_FCT_KEY_ICE_LOCK          (0x00000080UL)

#define MK_FCT_KEY_ICE_WATER_LOCK    (0x00000100UL)
#define MK_FCT_KEY_HOT_LOCK          (0x00000200UL)
#define MK_FCT_EEPROM                (0x00000400UL)
#define MK_FCT_COMM_COMP             (0x00000800UL)
#define MK_FCT_COMM_EOL              (0x00001000UL)
#define MK_FCT_LEVER_WATER           (0x00002000UL)
#define MK_FCT_LEVER_ICE             (0x00004000UL)
#define MK_FCT_TEMP_COLD             (0x00008000UL)

#define MK_FCT_TEMP_ROOM             (0x00010000UL)
#define MK_FCT_TEMP_AMBI             (0x00020000UL)
#define MK_FCT_TEMP_AMBI_2           (0x00040000UL)
#define MK_FCT_TEMP_EVA_1            (0x00080000UL)
#define MK_FCT_TEMP_EVA_2            (0x00100000UL)
#define MK_FCT_TEMP_HOT              (0x00200000UL)

#define MK_FCT_ALL                   (0x003FFFFFUL)

void SetFctTestInputBit(U32 maskBit);
void ClearFctTestInputBit(U32 maskBit);
void SetFctTestInputVal(U32 maskBit);
U8 IsSetFctTestInputVal(U32 maskBit);
U32 GetFctTestInputVal(void);

void UpdateFctTimer(void); // @1sec

#endif /* __FCT_H__ */
