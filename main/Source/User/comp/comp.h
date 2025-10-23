/**
 * File : comp.h
 * 
 * Comp Module
*/
#ifndef __COMP_H__
#define __COMP_H__

#include "prj_type.h"

#include "comp_bldc.h"

/// æ–√‡±‚ TYPE
#define COMP_TYPE_RELAY             (0)
#define COMP_TYPE_BLDC              (1)
#define CONFIG_BLDC_COMP_TYPE       COMP_TYPE_BLDC

#define MAX_COMP_RPS                (70U)

typedef struct _compressor_
{
    /// COMMAND
    U8 mode;         // Command mode
    U8 coldOnOff;    // Cold mode - onoff command
    U8 iceOnOff;     // Ice mode - onoff command
    U8 forcedOnOff;  // Forced mode - onoff command

    U16 protectOffTime;

    U32 onTime;
    U32 offTime;
} SCompressor_T;

void InitComp(void);

void GetCompData(SCompressor_T *pData);

#define  COMP_MODE_NONE     (0U)
#define  COMP_MODE_FORCED   (1U)
void SetCompMode(U8 mode);
U8 GetCompMode(U8 mode);

void SetCompProtectOffTime(U16 val);
U16 GetCompProtectOffTime(void);
U8 IsOverCompProtectOnTime(void);
U8 IsOverCompProtectSecondOnTime(void);

/// COMP TYPE
#define  COMP_TYPE_COLD    (0U)
#define  COMP_TYPE_ICE     (1U)
void TurnOnCompRps(U8 type, RPS_T rps);
void TurnOnComp(U8 type);
void TurnOffComp(U8 type);
U8 GetCompOnOffType(U8 type);

void TurnOnForcedComp(void);
void TurnOffForcedComp(void);

U32 GetCompOnTime(void);
U32 GetCompOffTime(void);

// Comp On/Off Command
// RETURN : ON or OFF
U8 GetCompOnOff(void);

U8 IsExpiredCompProtectTime(void);
void ProcessComp(void);

#endif /* __COMP_H__ */
