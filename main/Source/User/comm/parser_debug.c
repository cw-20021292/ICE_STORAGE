/**
 * File : parser_debug.c
 * 
 * Eol (Line Auto Test ) Uart Debug Data Parsing
**/
#include "hw.h"

#include "parser_debug.h"

#include <stdio.h>
#include <stdlib.h>

#include "comp.h"
#include "comp.h"
#include "comp_bldc.h"
#include "purifier.h"
#include "front.h"
#include "flow_meter.h"
#include "relay.h"
#include "heater.h"
#include "error.h"
#include "temp.h"
#include "service.h"
#include "rtc.h"
#include "power_saving.h"
#include "water_out.h"
#include "ster.h"
#include "room_water.h"
#include "cold_water.h"
#include "hot_water.h"
#include "drain.h"

#include "api_adc.h"

#define PKT_STX                 (0x01)
#define PKT_ETX                 (0x04)

#define PKT_ACK                 (0x06)
#define PKT_NAK                 (0x15)

#define DBG_RESET_COMP_PROTECT_TIME     '1'
#define DBG_RESET_UNUSED_SAVING_TIME    '2'
#define DBG_RESET_SAVING_TIME           '3'
#define DBG_RESET_STER                  '4'
#define DBG_RESET_NEO_TIME              '5'

#define DBG_RESET_NEO_AMOUNT            '6'
#define DBG_RESET_INNO_TIME             '7'
#define DBG_RESET_INNO_AMOUNT           '8'
#define DBG_RESET_RO_TIME               '9'
#define DBG_RESET_RO_AMOUNT             'A'

#define DBG_RESET_FLUSHING_TIME         'B'

void ParserDebugControl(U8 *buf, I16 len)
{
 
}

static U8 CheckFlushingOutMode(void)
{
  
    return 0;
}

static U8 GetDebugMainMajorVer(void)
{
    
    return 1;
}

static U8 GetDebugFrontMajorVer(void)
{
    

    return 1;
}

I16 MakePkt_Debug_0(U8 *buf, U16 type)
{
    I16 len = 0;

    return len;
}

/// Command : 00E1, Data : Adc, Temp, Current
I16 MakePkt_Debug_1(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%d,", GET_STATUS_TEMP_COLD(), GET_STATUS_TEMP_ROOM());
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_SENSOR_INFRARED_HIGH), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_SENSOR_INFRARED_LOW), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_TEMP_AMBIENT), GetTemp(TEMP_ID_AMBIENT));
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_TEMP_AMBIENT_2), GetTemp(TEMP_ID_AMBIENT_2));
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_TEMP_HOT_WATER), GetTemp(TEMP_ID_HOT_WATER));
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_TEMP_ROOM_WATER), GetTemp(TEMP_ID_ROOM_WATER));
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_TEMP_COLD_WATER), GetTemp(TEMP_ID_COLD_WATER));
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_TEMP_EVA_1), GetTemp(TEMP_ID_EVA_1));
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_TEMP_EVA_2), GetTemp(TEMP_ID_EVA_2));
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_PUMP_DRAIN_IAD), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_PUMP_COLD_VAD), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_PUMP_COLD_IAD), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_FAN_IAD), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_UV_DRAIN_TANK_IAD), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_UV_ICE_DOOR_IAD), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_UV_WATER_OUT_IAD), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f,", GetAdcData(ADC_ID_UV_ICE_TRAY_IAD), 0);
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%0.2f", GetAdcData(ADC_ID_UV_ICE_TANK_IAD), 0);
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

/// Command : 00E2, Data : Valve, Relay, Pump
I16 MakePkt_Debug_2(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
                                              !GET_STATUS_VALVE_NOS(),
                                              GET_STATUS_VALVE_FEED(),
                                              GET_STATUS_VALVE_COLD_IN(),
                                              GET_STATUS_VALVE_ROOM_OUT(),
                                              GET_STATUS_VALVE_HOT_OUT(),
                                              GET_STATUS_VALVE_COLD_CIRCULATE(),
                                              GET_STATUS_VALVE_ICE_WATER_OUT(),
                                              GET_STATUS_VALVE_ICE_TRAY_IN(),
                                              GET_STATUS_VALVE_DRAIN(),
                                              GET_STATUS_VALVE_NOS_FLOW()
                                              );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%d,%d,%d,%d,%d",
                                              IsTurnOnRelay(RELAY_COMP),
                                              IsTurnOnRelay(RELAY_HEATER),
                                              IsTurnOnRelay(RELAY_FEEDER_CW),
                                              IsTurnOnRelay(RELAY_FEEDER_CCW),
                                              IsTurnOnRelay(RELAY_ICE_TRAY_CW),
                                              IsTurnOnRelay(RELAY_ICE_TRAY_CCW)
                                              );
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

/// Command : 00E3, Data : Sensor( Level - RoomTank, ColdTank, HotTank, DrainTank )
I16 MakePkt_Debug_3(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,", GetTankConfDetectTime(LEVEL_ID_ROOM));
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%d,%d,%d,%d,",
                                              GetRoomWaterLevel(),
                                              GetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_LOW),
                                              GetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_COMPLEX),
                                              GetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_HIGH),
                                              GetTankLevelErrorCount(LEVEL_ID_ROOM, ERR_TYPE_OVF)
                                              );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%d,%d,%d,%d,",
                                              GetColdWaterLevel(),
                                              GetTankLevelErrorCount(LEVEL_ID_COLD, ERR_TYPE_LOW),
                                              GetTankLevelErrorCount(LEVEL_ID_COLD, ERR_TYPE_COMPLEX),
                                              GetTankLevelErrorCount(LEVEL_ID_COLD, ERR_TYPE_HIGH),
                                              GetTankLevelErrorCount(LEVEL_ID_COLD, ERR_TYPE_OVF)
                                              );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%d,%d,%d,%d,",
                                              GetHotWaterLevel(),
                                              GetTankLevelErrorCount(LEVEL_ID_HOT, ERR_TYPE_LOW),
                                              GetTankLevelErrorCount(LEVEL_ID_HOT, ERR_TYPE_COMPLEX),
                                              GetTankLevelErrorCount(LEVEL_ID_HOT, ERR_TYPE_HIGH),
                                              GetTankLevelErrorCount(LEVEL_ID_HOT, ERR_TYPE_OVF)
                                              );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%d,%d,%d,%d,%d",
                                              GetDrainLevel(),
                                              GetTankLevelErrorCount(LEVEL_ID_DRAIN, ERR_TYPE_LOW),
                                              GetTankLevelErrorCount(LEVEL_ID_DRAIN, ERR_TYPE_COMPLEX),
                                              GetTankLevelErrorCount(LEVEL_ID_DRAIN, ERR_TYPE_HIGH),
                                              GetTankLevelErrorCount(LEVEL_ID_DRAIN, ERR_TYPE_OVF)
                                              );
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

I16 MakePkt_Debug_4(U8 *buf, U16 type)
{
    I16 len = 0;

    return len;
}

I16 MakePkt_Debug_5(U8 *buf, U16 type)
{
    I16 len = 0;

    return len;
}

I16 MakePkt_Debug_6(U8 *buf, U16 type)
{
    I16 len = 0;

    return len;
}

I16 MakePkt_Debug_7(U8 *buf, U16 type)
{
    I16 len = 0;
    
    return len;
}
