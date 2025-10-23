/**
 * File : api_led.h
 * 
 * Application Programming Interface
 * Depend on HAL
**/
#ifndef __API_LED_H__
#define __API_LED_H__

#include "prj_type.h"

void LedInit(void);

void LedOnOffUvCareSystem(U8 onOff);
void LedOnOffUvIceTank(U8 onOff);
void LedOnOffUvIceTray(U8 onOff);
void LedOnOffUvIceFaucet(U8 onOff);
void LedOnOffUvWaterFaucet(U8 onOff);
void LedOnOffStatusIceWater(U8 onOff);
void LedOnOffStatusMax(U8 onOff);
void LedOnOffStatusWater(U8 onOff);
void LedOnOffSeg1A(U8 onOff);
void LedOnOffSeg1B(U8 onOff);
void LedOnOffStatusIcePreparing(U8 onOff);
void LedOnOffStatusIceFull(U8 onOff);
void LedOnOffStatusWaterPreparing(U8 onOff);
void LedOnOffStatusUnitCelsius(U8 onOff);
void LedOnOffSeg2A(U8 onOff);
void LedOnOffSeg2B(U8 onOff);
void LedOnOffSeg2C(U8 onOff);
void LedOnOffSeg2D(U8 onOff);
void LedOnOffSeg2E(U8 onOff);
void LedOnOffSeg2F(U8 onOff);
void LedOnOffSeg2G(U8 onOff);
void LedOnOffSeg3A(U8 onOff);
void LedOnOffSeg3B(U8 onOff);
void LedOnOffSeg3C(U8 onOff);
void LedOnOffSeg3D(U8 onOff);
void LedOnOffSeg3E(U8 onOff);
void LedOnOffSeg3F(U8 onOff);
void LedOnOffSeg3G(U8 onOff);
void LedOnOffSeg4A(U8 onOff);
void LedOnOffSeg4B(U8 onOff);
void LedOnOffSeg4C(U8 onOff);
void LedOnOffSeg4D(U8 onOff);
void LedOnOffSeg4E(U8 onOff);
void LedOnOffSeg4F(U8 onOff);
void LedOnOffSeg4G(U8 onOff);
void LedOnOffStatusBarLow(U8 onOff);
void LedOnOffStatusBarMiddle(U8 onOff);
void LedOnOffStatusBarHigh(U8 onOff);
void LedOnOffStatusBarInfinite(U8 onOff);
void LedOnOffStatusUnitMl(U8 onOff);
void LedOnOffWelcomeRightBlue(U8 onOff);
void LedOnOffWelcomeRightGreen(U8 onOff);
void LedOnOffWelcomeRightRed(U8 onOff);
void LedOnOffWelcomeRightWhite(U8 onOff);
void LedOnOffWelcomeLeftBlue(U8 onOff);
void LedOnOffWelcomeLeftGreen(U8 onOff);
void LedOnOffWelcomeLeftRed(U8 onOff);
void LedOnOffWelcomeLeftWhite(U8 onOff);
void LedOnOffAmount(U8 onOff);
void LedOnOffCold(U8 onOff);
void LedOnOffRoom(U8 onOff);
void LedOnOffHot(U8 onOff);
void LedOnOffIceWaterAmount(U8 onOff);
void LedOnOffIceWater(U8 onOff);
void LedOnOffIce(U8 onOff);
void LedOnOffStausCooling(U8 onOff);
void LedOnOffStausLowWater(U8 onOff);
void LedOnOffStausHeating(U8 onOff);
void LedOnOffStausHotLock(U8 onOff);
void LedOnOffStausIceWaterAmountLock(U8 onOff);
void LedOnOffStausIceWaterLock(U8 onOff);
void LedOnOffStausIceLock(U8 onOff);
void LedOnOffAllLock(U8 onOff);
void LedOnOffColdOff(U8 onOff);
void LedOnOffHotOff(U8 onOff);
void LedOnOffHotLock(U8 onOff);
void LedOnOffIceOff(U8 onOff);
void LedOnOffIceWaterLock(U8 onOff);
void LedOnOffIceLock(U8 onOff);

#endif  /* __API_LED_H__ */
