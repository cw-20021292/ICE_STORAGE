/**
 * File : swing_bar.h
 * 
 * Swing Bar ( Static electricity ? )
*/
#ifndef __SWING_BAR_H__
#define __SWING_BAR_H__

#include "prj_type.h"

typedef struct _swing_bar_
{
    U8 OnOff;      // Swing Bar On/Off command
    U8 OnTime;     // Swing Bar On/Off time
    U8 OffTime;    // Swing Bar On/Off time
} SSwingBar_T;

void InitSwingBar(void);

void GetSwingBarData(SSwingBar_T *pData);

void TurnOffSwingBar(void);

void TurnOnSwingBar(void);

void ControlSwingBar(void);

#endif /* __SWING_BAR_H__ */
