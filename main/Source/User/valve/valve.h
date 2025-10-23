/**
 * File : valve.h
 * 
 * Valve Control
**/
#ifndef __VALVE_H__
#define __VALVE_H__

#include "prj_type.h"

void InitValve();

#define VALVE_NOS                (0x0001)
#define VALVE_FEED               (0x0002)
#define VALVE_COLD_IN            (0x0004)
#define VALVE_ROOM_OUT           (0x0008)
#define VALVE_HOT_OUT            (0x0010)
#define VALVE_COLD_CIRCULATE     (0x0020)
#define VALVE_ICE_WATER_OUT      (0x0040)
#define VALVE_ICE_TRAY_IN        (0x0080)
#define VALVE_DRAIN              (0x0100)
#define VALVE_NOS_FLOW           (0x0200)
#define VALVE_EXTRA              (0x0400)
#define MAX_VALVE_NUM            (11)

#define VALVE_ALL ( \
        VALVE_NOS \
        | VALVE_FEED \
        | VALVE_COLD_IN \
        | VALVE_ROOM_OUT \
        | VALVE_HOT_OUT \
        | VALVE_COLD_CIRCULATE \
        | VALVE_ICE_WATER_OUT \
        | VALVE_ICE_TRAY_IN \
        | VALVE_DRAIN \
        | VALVE_NOS_FLOW \
        | VALVE_EXTRA \
        )

#define NORMAL_OPEN_VALVES ( \
        VALVE_NOS \
        | VALVE_NOS_FLOW \
        )

void OpenValve(U16 valve);

void CloseValve(U16 valve);

U16 GetValveStatus(void);

U8 IsOpenValve(U16 valve);

void ControlValve(void);

void TEST_ControlValve(void);

#endif /* __VALVE_H__ */
