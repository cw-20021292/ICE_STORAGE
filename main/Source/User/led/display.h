/**
 * File : display.h
 * 
 * Led Display
*/
#ifndef __DISPLAY_H__
#define __DISPLAY_H__

#include "prj_type.h"

typedef enum _temp_status_
{
    TEMP_STATUS_NONE = 0,           // 0 :
    TEMP_STATUS_HEATING,            // 0 :
    TEMP_STATUS_HEATING_DIM,        // 0 :
    TEMP_STATUS_COOLING,            // 0 :
    TEMP_STATUS_COOLING_DIM,        // 0 :
} ETempStatus_T;

/* Functinos */
void SetLedParameter(void);
void SetLedAllType(void);
void SetLedAllOnDuty(void);
void SetLedSpecificOnDuty(U8 led, U8 duty);
void SetLedAllOffDuty(void);
void SetLedSpecificOffDuty(U8 led, U8 duty);
void SetLedDispSavingAllOnDuty(void);

void DispUvLedOffDuty(void);
void DispIceAmountLedOffDuty(void);
void DispAmountLedOffDuty(void);
void DispWaterOutBarLedOffDuty(void);

void TurnOffAllLED(void);
void TurnOnAllLED(void);

void DispLedDuty(void);

// 얼음 추출
// 0 : circle OFF
// 1 ~ 3 circle ON
void DispIceOut(U8 onOff);

// 물 추출
// 0 : circle OFF
// 1 ~ 3 circle ON
void DispWaterOut(U8 onOff);

// 냉온정 선택
void DispWaterSel(U8 sel);

// 얼음 선택
void DispIceSel(U8 sel);

// Welcome
void DispWelcomWaterWhite(U8 onOff);
void DispWelcomWaterRed(U8 onOff);
void DispWelcomWaterGreen(U8 onOff);
void DispWelcomWaterBlue(U8 onOff);

void DispWelcomIceWhite(U8 onOff);
void DispWelcomIceRed(U8 onOff);
void DispWelcomIceGreen(U8 onOff);
void DispWelcomIceBlue(U8 onOff);

/// UV Care System
void DispUvCareSystem(U8 onOff);

// 물양
void DispAmountSel(U8 amount);

void DispIceWaterAmount(U8 onOff);
void DispAmount(U8 amount);

/// 얼음 잠김
void DispIceLock(U8 onOff);
/// 얼음물 잠김
void DispIceWaterLock(U8 onOff);
/// 온수 잠김
void DispHotLock(U8 onOff);

// 온수 종류 - 변경 중

// 온수 종류 표시 - 최종 상태

// 온수 표시 OFF

void DispBarSter(U8 onOff);

void DispBarIceTank(U8 onOff);

void DispUvFaucetShot(U8 onOff);

void DispUvIceDoorShot(U8 onOff);

U8 DispTempStatus(U8 sel, TEMP_T temp);
void DispColdTempLevel(U8 onOff);
void DispHotTempLevel(U8 onOff);

void DispRoomWater(U8 onOff);

void DispPowerSaving(U8 onOff);

void DispSter(U8 onOff);
//void DispSterFauset(U8 onOff);
void DispSterWaterFaucet(U8 onOff);
void DispSterIceFaucet(U8 onOff);
void DispSterIceRoom(U8 onOff);
void DispSterIceTray(U8 onOff);
//void DispSterFlowPath(U8 onOff);

void DispStatusIceWater(U8 onOff);
void DispStatusMax(U8 onOff);
void DispStatusWater(U8 onOff);

void DispDrain(U8 onOff);

void DispColdWater(U8 onOff);

void DispService(U8 onOff);

void DispConty(U8 onOff);

void DispColdTemp_1(U8 onOff);

void DispIce(U8 onOff);

void DispIceWater(U8 onOff);

void DispIceMake(U8 onOff);

void DispColdMake(U8 onOff);

void DispUnitMl(U8 onOff);

void DispIceTurbo(U8 onOff);

void DispSettingIceLock(U8 onOff);

void DispSettingIceLockBlink(U8 onOff);

void DispSettingIceWaterLock(U8 onOff);

void DispSettingIceWaterLockBlink(U8 onOff);

void DispSettingIceWaterAmountLockBlink(U8 onOff);

void DispSettingIceWaterAmountNotiBlink(U8 onOff);

void DispSettingHotLock(U8 onOff);

void DispSettingHotLockBlink(U8 onOff);

void DispConty500mL(U8 onOff);

void DispConty1L(U8 onOff);

void DispConty1D5L(U8 onOff);

void DispContyInfinity(U8 onOff);

void DispTankCleanBlink(U8 onOff);

void DispIceFullHigh(U8 onOff);

void DispIceFullLow(U8 onOff);

void DispWaterPreparing(U8 onOff);

void DispUnitCelsius(U8 onOff);

void DispHotWater(U8 onOff);

void DispConfUnusedSaveTrueBlink(U8 onOff);

void DispConfUnusedSaveFalseBlink(U8 onOff);

void DispLockIce(U8 onOff);

void DispLockIceWater(U8 onOff);

void DispLockIceWaterAmount(U8 onOff);

void DispLockAll(U8 onOff);

void DispLockAllSetBlink(U8 onOff);

void DispLockAllBlink(U8 onOff);

void DispLock(U8 onOff);

void DispIceMaker(U8 onOff);

void DispLockIceSetBlink(U8 onOff);

void DispLockIceWaterSetBlink(U8 onOff);

void DispLockHotSetBlink(U8 onOff);

void DispMainVersionBar(void);
void DispFrontVersionBar(void);

void DispVersion(U8 val);

// VERSION ON time 2sec..
// return ture : time out
// return false : goning on..
U8 DispVersionOnTime(void);
U8 DispFrontVersionOnTime(void);

void AllOnSegment(void);
void AllOffSegment(void);

void DispSegStr(const char *str);
void DispSegDigits(U16   amount);
void DispSegNegativeDigits(I16    amount);
void DispSegInfinity(void);
void DispSegError(U8 code);
void DispSegStrDigit(const char *str, U8 digits);

void ResetInfinityCount(void);
void DispWaterOutAmountInfinity(void);
void ResetLeverCount(void);
void DispWaterOutLever(void);

// 7세그먼트 문자 표시 ID
enum
{
    SEGMENT_CHAR_ID_ON,           /* On */
    SEGMENT_CHAR_ID_OFF,          /* OFF */
    SEGMENT_CHAR_ID_CON,          /* Con */
    SEGMENT_CHAR_ID_STACK_BAR_CENTER,   /* --- */
    SEGMENT_CHAR_ID_STACK_BAR_TOP,      /* ``` */
    SEGMENT_CHAR_ID_BAR_BOTTOM,   /* ___ */
    SEGMENT_CHAR_ID_BAR_CENTER,   /* --- */
    SEGMENT_CHAR_ID_BAR_TOP,      /* ``` */
    SEGMENT_CHAR_ID_CHP,          /* CHP */
    SEGMENT_CHAR_ID_CP,           /* CP  */
    SEGMENT_CHAR_ID_CIRCLE,       /* CIRCLE  */
    SEGMENT_CHAR_ID_SPACE,        /* SPACE  */
    SEGMENT_CHAR_ID_ICE_1,         /* ICE OUT-EFFECT 1  */
    SEGMENT_CHAR_ID_ICE_2,         /* ICE OUT-EFFECT 2 */
    SEGMENT_CHAR_ID_ICE_3,         /* ICE OUT-EFFECT 3 */
    SEGMENT_CHAR_ID_ICE_4,         /* ICE OUT-EFFECT 4 */
    SEGMENT_CHAR_ID_CENTER_1,      /* SOUND VOL -1- */
    SEGMENT_CHAR_ID_CENTER_2,      /* SOUND VOL -2- */
    SEGMENT_CHAR_ID_CENTER_3,      /* SOUND VOL -3- */
    SEGMENT_CHAR_ID_SE,             /* CUSTOMER MODE - SELF */
    SEGMENT_CHAR_ID_CO,             /* CUSTOMER MODE - COWAY(?) - VISIT */
    SEGMENT_CHAR_ID_STE,            /* 살균 - StE */
    MAX_SEGMENT_CHAR_ID,        
};
void DispSegmentChar(U8 id);

#endif /* __DISPLAY_H__ */
