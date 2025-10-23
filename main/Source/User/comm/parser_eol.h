/**
 * File : parser_eol.h
 * 
 * Eol (Line Auto Test ) Uart Data Parsing
*/
#ifndef __PARSER_EOL_H__
#define __PARSER_EOL_H__

#include "prj_type.h"
#include "comm.h"

#define EOL_CMD_NONE                  (0x0000)

#define EOL_CMD_MODEL                 (0x2000)

#define EOL_CMD_SETTING               (0x3000)

/* ETC */
#define EOL_CMD_UV_WATER_OUT          (0x3011)
#define EOL_CMD_FLOW_SESNOR           (0x3012)
#define EOL_CMD_AC_DC                 (0x3013)
#define EOL_CMD_UV_ICE_DOOR           (0x3014)
#define EOL_CMD_FAN_FB                (0x3016)
#define EOL_CMD_PUMP_DRAIN            (0x3017)
#define EOL_CMD_PUMP_ICE_DRAIN        (0x3018)
#define EOL_CMD_UV_ICE_TANK           (0x3019)
#define EOL_CMD_ICE_FULL_LOW          (0x301A)
#define EOL_CMD_UV_DRAIN_TANK         (0x301B)
#define EOL_CMD_UV_ICE_TRAY           (0x301C)
#define EOL_CMD_ICE_FULL_HIGH         (0x301D)
#define EOL_CMD_PUMP_CIRCULATE        (0x301F)

/* SENSOR */
#define EOL_CMD_SENSOR_TEMP           (0x3020)
#define EOL_CMD_SENSOR_LEVEL          (0x3030)
#define EOL_CMD_SENSOR                (0x3040)

/* LOAD */
#define EOL_CMD_NO_LOAD                 (0x5000)

#define EOL_CMD_HEATER_ON               (0x5010)      /// ¼ø°£ ¿Â¼ö, ÀÏ¹Ý ¿Â¼ö
#define EOL_CMD_HEATER_OFF              (0x5011)

#define EOL_CMD_COLD_IN_VALVE_ON        (0x5102)
#define EOL_CMD_COLD_IN_VALVE_OFF       (0x5103)

#define EOL_CMD_ROOM_OUT_VALVE_ON       (0x5106)
#define EOL_CMD_ROOM_OUT_VALVE_OFF      (0x5107)

#define EOL_CMD_COLD_OUT_VALVE_ON       (0x5108)    /// ³Ã¼ö ¼øÈ¯ ¹ëºê
#define EOL_CMD_COLD_OUT_VALVE_OFF      (0x5109)    /// ³Ã¼ö ¼øÈ¯ ¹ëºê

#define EOL_CMD_HOT_OUT_VALVE_ON        (0x510A)
#define EOL_CMD_HOT_OUT_VALVE_OFF       (0x510B)

#define EOL_CMD_ICE_WATER_OUT_VALVE_ON  (0x510C)
#define EOL_CMD_ICE_WATER_OUT_VALVE_OFF (0x510D)

#define EOL_CMD_FEED_VALVE_ON           (0x510E)
#define EOL_CMD_FEED_VALVE_OFF          (0x510F)

#define EOL_CMD_NOS_VALVE_ON            (0x5110)
#define EOL_CMD_NOS_VALVE_OFF           (0x5111)

#define EOL_CMD_ICE_TRAY_VALVE_ON       (0x5116)
#define EOL_CMD_ICE_TRAY_VALVE_OFF      (0x5117)

#define EOL_CMD_DRAIN_VALVE_ON          (0x5118)
#define EOL_CMD_DRAIN_VALVE_OFF         (0x5119)

#define EOL_CMD_NOS_FLOW_ON             (0x5122)
#define EOL_CMD_NOS_FLOW_OFF            (0x5123)

#define EOL_CMD_FAN_MOTOR_ON             (0x5200)
#define EOL_CMD_FAN_MOTOR_OFF            (0x5201)

#define PKT_EOL_GAS_SWITCH_ON            (0x5202)
#define PKT_EOL_GAS_SWITCH_OFF           (0x5203)

#define PKT_EOL_ICE_TRAY_MOTOR_ON        (0x5206)
#define PKT_EOL_ICE_TRAY_MOTOR_OFF       (0x5207)

#define PKT_EOL_ICE_DOOR_MOTOR_ON        (0x5208)
#define PKT_EOL_ICE_DOOR_MOTOR_OFF       (0x5209)

#define EOL_CMD_ICE_FEEDER_MOTOR_CW_ON   (0x520A)
#define EOL_CMD_ICE_FEEDER_MOTOR_CW_OFF  (0x520B)

#define EOL_CMD_ICE_FEEDER_MOTOR_CCW_ON  (0x520C)
#define EOL_CMD_ICE_FEEDER_MOTOR_CCW_OFF (0x520D)

#define PKT_EOL_SWING_BAR_ON          (0x5210)
#define PKT_EOL_SWING_BAR_OFF         (0x5211)

#define PKT_EOL_HOT_GAS_SWITCH_ON     (0x5212)
#define PKT_EOL_HOT_GAS_SWITCH_OFF    (0x5213)

#define EOL_CMD_ICE_DRAIN_PUMP_ON     (0x5300)
#define EOL_CMD_ICE_DRAIN_PUMP_OFF    (0x5301)

#define EOL_CMD_DRAIN_PUMP_ON         (0x5302)
#define EOL_CMD_DRAIN_PUMP_OFF        (0x5303)

#define EOL_CMD_CIRCULATE_PUMP_ON     (0x5304)
#define EOL_CMD_CIRCULATE_PUMP_OFF    (0x5305)

/* CHECK COLD/ICE SYSTEM */
#define EOL_CMD_COMP_ON               (0x9900)
#define EOL_CMD_COMP_OFF              (0x9901)

#define PKT_EOL_ICE_SYSTEM_ICE        (0x9902)
#define PKT_EOL_ICE_SYSTEM_COLD       (0x9903)
#define PKT_EOL_ICE_SYSTEM_DEICING    (0x9904)

#define EOL_CMD_COMP_COMM_CHECK       (0x9910)

#define EOL_CMD_COMPLETE              (0xFF00)

/* DEBUG MESSAGE */
#define EOL_CMD_DEBUG_CONTROL         (0x00E0)
#define EOL_CMD_DEBUG_1               (0x00E1)
#define EOL_CMD_DEBUG_2               (0x00E2)
#define EOL_CMD_DEBUG_3               (0x00E3)
#define EOL_CMD_DEBUG_4               (0x00E4)
#define EOL_CMD_DEBUG_5               (0x00E5)
#define EOL_CMD_DEBUG_6               (0x00E6)
#define EOL_CMD_DEBUG_7               (0x00E7)

I16 IsValidPktEol(U8 *buf, I16 len);
I16 ParserPktEol(U8 *buf, I16 len);
I16 Crc16Eol(U8 *buf, I16 len);
I16 MakePktEol(CommHeader_T *pComm, U8 *buf);

#endif /* __PARSER_EOL_H__ */
