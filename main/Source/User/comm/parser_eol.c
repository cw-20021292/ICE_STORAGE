/**
 * File : parser_eol.c
 * 
 * Eol (Line Auto Test ) Uart Data Parsing
*/
#include "hw.h"

#include "parser_eol.h"

#include <stdio.h>
#include <stdlib.h>

#include "timer.h"
#include "util.h"
#include "eol.h"
#include "error.h"
#include "valve.h"
#include "comp_bldc.h"
#include "relay.h"
#include "err_temp.h"
#include "temp.h"
#include "front.h"
#include "purifier.h"
#include "leak.h"
#include "input.h"
#include "parser_debug.h"
#include "ice_door.h"
#include "gas_switch_valve.h"
#include "hotgas_switch_valve.h"
#include "drain_pump.h"
#include "circulate_pump.h"
#include "service.h"

#include "api_pump.h"
#include "api_adc.h"
#include "api_uv.h"

#include "process_display.h"

/***********************************************************************************************
 * DEFINITION 
 */
#define PKT_STX                 (0x01)
#define PKT_ETX                 (0x04)

#define PKT_ACK                 (0x06)
#define PKT_NAK                 (0x15)

#define FIELD_STX               (0)
#define FIELD_PACKET_TYPE       (1)
#define FIELD_PAYLOAD           (5)
#define FIELD_CHECKSUM          (7)
#define FIELD_ETX               (9)

#define LENGTH_PACKET_TYPE      (4)
#define	MIN_PKT_SZ              (6)	    /* STX + TYPE + ETX + CRC(2byes) */

static U8	Check_Crc(U8 *buf, I16 len)
{
    U8 i = 0;
    U8 checkSum = 0;

    for( i=0; i<len - 3; i++ )
    {
        checkSum ^= buf[i];
    }

    return checkSum;
}

static U8 IsTrueCheckSum(U8 *buf, I16 len)
{
    U8 cal;
    U8 val;

    cal = Check_Crc(buf, len);
    val = ConvAsc2Byte(buf[len - 3], buf[len - 2]);
    if( cal != val )
    {
        return FALSE;
    }

    return TRUE;
}

// Return Packet type assic to hex
static U16 GetPacketCmd(U8 *buf, U8 len)
{
    U8  high = 0;
    U8  low = 0;
    U16 cmd = 0;

    // Skip DEBUG PACKET TYPE
    high = ConvAsc2Byte(buf[0], buf[1]);
    low = ConvAsc2Byte(buf[2], buf[3]);
    cmd = GET_16_UINT_WORD(low, high);

    return cmd;
}

static U8 IsDebugMsssage(U8 *buf)
{
    U16 cmd = 0;

    // Skip DEBUG PACKET TYPE
    cmd = GetPacketCmd(&buf[ FIELD_PACKET_TYPE ], LENGTH_PACKET_TYPE);
    if( cmd >= EOL_CMD_DEBUG_CONTROL 
            && cmd <= EOL_CMD_DEBUG_7 )
    {
        return TRUE;    // DEBUG MESSAGE
    }
    else
    {
        return FALSE;   // EOL MESSAGE 
    }
}

static U8 IsStartMessage(U8 *buf)
{
    U16 uCmd = 0;

    // Skip DEBUG PACKET TYPE
    uCmd = GetPacketCmd(&buf[FIELD_PACKET_TYPE], LENGTH_PACKET_TYPE);
    if( uCmd == EOL_CMD_MODEL )
    {
        return TRUE;    // CHECK MESSAGE
    }
    else
    {
        return FALSE;   // OTHERS MESSAGE
    }
}

/// ���� ���� ��Ŷ�� ��ȿ�� �˻�
I16 IsValidPktEol(U8 *buf, I16 len)
{
    U8 testMode = 0U;

    if( buf == NULL )
    {
        return FALSE;
    }

    if( len < MIN_PKT_SZ )
    {
        return FALSE;
    }

    if( buf[FIELD_STX] != PKT_STX 
            || buf[len - 1] != PKT_ETX )
    {
        return FALSE;
    }

    if( IsTrueCheckSum(buf, len) == FALSE )
    {
        return FALSE;
    }

    // ����� �޽����� ��� ���� ó��...
    if( IsDebugMsssage(buf) == TRUE )
    {
        return TRUE;    // SKIP DEBUG MESSAGE 
    }

    /// �˻� ���� Ŀ�ǵ�� �Է� ���
    if( GetEolStatus() == FALSE  )
    {
        /// ��, �˻� ���� �ð� �ʰ��Ǹ� SKIP
        if( IsExpiredEolLimitTimer() == TRUE )
        {
            return FALSE;
        }

        if( IsStartMessage(buf) == TRUE )
        {
            // 0x2000 Command ���� ��, ��� �簳�ǵ��� �ؾ� ��
            {
                testMode = ConvAsc2Byte(buf[FIELD_PAYLOAD], buf[FIELD_PAYLOAD+1]);

                GasSwitchInit();
                HotGasSwitchInit();
                
                StartEolLoad();
                
                if( testMode == 1 )
                {
                    return TRUE;
                }
                else if( testMode == 2 )
                {
                    if( GetEolMode() == EOL_TEST_MODE_0 )
                    {
                        SetEolTestMode(EOL_TEST_MODE_1);
                    }
                }
            }
        }

        return FALSE;
    }
    else
    {
        /// �˻� ����  �� ��а˻� �׸��� �˻��� ���� �˻� ��忡���� ��� �޽����� ���� ���� �ʴ´�.
        if( GetEolMode() == EOL_MODE_SEALED_INIT
            || GetEolMode() == EOL_MODE_SEALED
            || GetEolMode() == EOL_MODE_READY
            || GetEolMode() == EOL_MODE_DONE )
        {
            return FALSE;
        }

        /// �׽�Ʈ ��� 2�ܰ谡 �ƴ� ���¿���
        /// PKT_EOL_MODEL ������ �ƴѰ� �ԷµǸ� drop�Ѵ�..
        if( GetEolTestMode() != EOL_TEST_MODE_2 )
        {
            if( IsStartMessage(buf) == FALSE )
            {
                return FALSE;
            }
        }
    }

    return TRUE;
}

/**
 * Received Data Parsing
**/
extern U8 RunCheckHeater;
extern U8 RunCheckIceTray;
static void TurnOffAllLoad(void)
{
    /// step ice door 
    TEST_StopIceDoor();
    TEST_StopGasValve();
    TEST_StopHotGasValve();

    /// stop ice tray
    RunCheckIceTray = FALSE;
    RunCheckHeater  = FALSE;

    /// Turn off Valves ( Exccept Normal Open valves )
    CloseValve(VALVE_ALL);
    // Turn on normal open valves
    OpenValve(NORMAL_OPEN_VALVES);

    /// Turn off Relay
    TurnOffRelayAll();

    /// HOT WATER HEATER
    TURN_OFF_HEATER();

    /// Turn Off Drain Pump
    TurnOffDrainPump();
    TurnOffPump(PUMP_ID_DRAIN);

    /// Turn Off Ice Drain Pump
    TurnOffPump(PUMP_ID_ICE_DRAIN);

    /// Turn Off Circulate Pump
    TurnOffCirculatePumpMakeColdForced();

    /// Turn off Swing Bar
    TURN_OFF_MOTOR_SWING_BAR();

    /// Turn Off UV LED
    API_TurnOffUv(UV_ID_ICE_TANK);
    API_TurnOffUv(UV_ID_ICE_TRAY);
    API_TurnOffUv(UV_ID_ICE_DOOR);
    API_TurnOffUv(UV_ID_WATER_OUT);

    /// Turn off Dc FAN
    TURN_OFF_MOTOR_FAN();
    /// Turn off Comp
    SetCompBldcTargetRps( 0 );    
}

static void ParserStart(U8 *pBuf, I16 len)
{
    U8 testMode = 0U;

    // check with one test 1 mode and test 2mode
    // 0x01 -> Start test 2 mode(Load Test), 0x02 -> Start test 1 mode(Front test)
    testMode = ConvAsc2Byte(pBuf[0], pBuf[1]);

    //StartEolLoad();

    if( testMode == 0x01 )
    {
        SetEolTestMode(EOL_TEST_MODE_2);
        SetEolMode(EOL_MODE_INIT);
    }
#if 0
    else if( testMode == 0x02 )
    {
        if( Get_Eol_Mode() == EOL_TEST_MODE_0 )
        {
            Set_Eol_Test_Mode(EOL_TEST_MODE_1);
        }
        //Set_Eol_Mode(EOL_MODE_READY);
    }
#endif
}

#define COMP_ON_RPS   (65)
static void ParserCompOn(U8 *buf, I16 len)
{
    SetCompBldcTargetRps(COMP_ON_RPS);
}

static void ParserCompOff(U8 *buf, I16 len)
{
    SetCompBldcTargetRps(0);
}

static void ParserIceSystemIce(U8 *buf, I16 len)
{
    GasSwitchIce();
    HotGasSwitchNormal();
}

static void ParserIceSystemCold(U8 *buf, I16 len)
{
    GasSwitchCold();
    HotGasSwitchNormal();
}

static void ParserIceSystemDeicing(U8 *buf, I16 len)
{
    HotGasSwitchHot();
}

static void ParserComplete(U8 *buf, I16 len)
{
    SetEolMode( EOL_MODE_DONE );

    TurnOffAllLoad();
}

static void ParserSensor(U8 *buf, I16 len)
{
    SetEolMode(EOL_MODE_CHECK_SENSOR);
}

static void ParserNoLoad(U8 *buf, I16 len)
{
    SetEolMode(EOL_MODE_CHECK_LOAD);

    TurnOffAllLoad();
}

static void ParserHeaterOn(U8 *buf, I16 len)
{
    RunCheckHeater = TRUE;
}

static void ParserHeaterOff(U8 *buf, I16 len)
{
    RunCheckHeater = FALSE;
}

static void ParserColdInValveOn(U8 *buf, I16 len)
{
    OpenValve(VALVE_COLD_IN);
}

static void ParserColdInValveOff(U8 *buf, I16 len)
{
    CloseValve(VALVE_COLD_IN);
}

static void ParserRoomOutValveOn(U8 *buf, I16 len)
{
    OpenValve(VALVE_ROOM_OUT);
}

static void ParserRoomOutValveOff(U8 *buf, I16 len)
{
    CloseValve(VALVE_ROOM_OUT);
}

static void ParserColdOutValveOn(U8 *buf, I16 len)
{
    OpenValve(VALVE_COLD_CIRCULATE);
}

static void ParserColdOutValveOff(U8 *buf, I16 len)
{
    CloseValve(VALVE_COLD_CIRCULATE);
}

static void ParserHotOutValveOn(U8 *buf, I16 len)
{
    OpenValve(VALVE_HOT_OUT);
}

static void ParserHotOutValveOff(U8 *buf, I16 len)
{
    CloseValve(VALVE_HOT_OUT);
}

static void ParserIceWaterOutValveOn(U8 *buf, I16 len)
{
    OpenValve(VALVE_ICE_WATER_OUT);
}

static void ParserIceWaterOutValveOff(U8 *buf, I16 len)
{
    CloseValve(VALVE_ICE_WATER_OUT);
}

static void ParserFeedValveOn(U8 *buf, I16 len)
{
    OpenValve(VALVE_FEED);
}

static void ParserFeedValveOff(U8 *buf, I16 len)
{
    CloseValve(VALVE_FEED);
}

static void ParserNosValveOn(U8 *buf, I16 len)
{
    CloseValve(VALVE_NOS);
}

static void ParserNosValveOff(U8 *buf, I16 len)
{
    OpenValve(VALVE_NOS);
}

static void ParserIceTrayValveOn(U8 *buf, I16 len)
{
    OpenValve(VALVE_ICE_TRAY_IN);
}

static void ParserIceTrayValveOff(U8 *buf, I16 len)
{
    CloseValve(VALVE_ICE_TRAY_IN);
}

static void ParserDrainValveOn(U8 *buf, I16 len)
{
    OpenValve(VALVE_DRAIN);
}

static void ParserDrainValveOff(U8 *buf, I16 len)
{
    CloseValve(VALVE_DRAIN);
}

static void ParserNosFlowValveOn(U8 *buf, I16 len)
{
    CloseValve(VALVE_NOS_FLOW);
}

static void ParserNosFlowValveOff(U8 *buf, I16 len)
{
    OpenValve(VALVE_NOS_FLOW);
}

static void ParserFanOn(U8 *buf, I16 len)
{
    TURN_ON_MOTOR_FAN();
}

static void ParserFanOff(U8 *buf, I16 len)
{
    TURN_OFF_MOTOR_FAN();
}

static void ParserGasSwitchOn(U8 *buf, I16 len)
{
    TEST_StartGasValve();
}

static void ParserGasSwitchOff(U8 *buf, I16 len)
{
    TEST_StopGasValve();
}

static void ParserIceTrayMotorOn(U8 *buf, I16 len)
{
    RunCheckIceTray = TRUE;
}

static void ParserIceTrayMotorOff(U8 *buf, I16 len)
{
    RunCheckIceTray = FALSE;
}

static void ParserIceDoorMotorOn(U8 *buf, I16 len)
{
    TEST_StartIceDoor();
}

static void ParserIceDoorMotorOff(U8 *buf, I16 len)
{
    TEST_StopIceDoor();
}

static void ParserFeederMotorCwOn(U8 *buf, I16 len)
{
    TurnOnRelay(RELAY_FEEDER_CW);
    TurnOffRelay(RELAY_FEEDER_CCW);
}

static void ParserFeederMotorCwOff(U8 *buf, I16 len)
{
    TurnOffRelay(RELAY_FEEDER_CW);
    TurnOffRelay(RELAY_FEEDER_CCW);
}

static void ParserFeederMotorCcwOn(U8 *buf, I16 len)
{
    TurnOffRelay(RELAY_FEEDER_CW);
    TurnOnRelay(RELAY_FEEDER_CCW);
}

static void ParserFeederMotorCcwOff(U8 *buf, I16 len)
{
    TurnOffRelay(RELAY_FEEDER_CW);
    TurnOffRelay(RELAY_FEEDER_CCW);
}

static void ParserSwingBarOn(U8 *buf, I16 len)
{
    TURN_ON_MOTOR_SWING_BAR();
}

static void ParserSwingBarOff(U8 *buf, I16 len)
{
    TURN_OFF_MOTOR_SWING_BAR();
}

static void ParserHotGasSwitchOn(U8 *buf, I16 len)
{
    TEST_StartHotGasValve();
}

static void ParserHotGasSwitchOff(U8 *buf, I16 len)
{
    TEST_StopHotGasValve();
}

static void ParserIceDrainPumpOn(U8 *buf, I16 len)
{
     TurnOnPump(PUMP_ID_ICE_DRAIN, 100U);
}

static void ParserIceDrainPumpOff(U8 *buf, I16 len)
{
    TurnOffPump(PUMP_ID_ICE_DRAIN);
}

static void ParserDrainPumpOn(U8 *buf, I16 len)
{
    TurnOnPump(PUMP_ID_DRAIN, 100U);
}

static void ParserDrainPumpOff(U8 *buf, I16 len)
{
    TurnOffPump(PUMP_ID_DRAIN);
}

static void ParserColdCirculatePumpOn(U8 *buf, I16 len)
{
    TurnOnCirculatePumpMakeColdForced();
}

static void ParserColdCirculatePumpOff(U8 *buf, I16 len)
{
    TurnOffCirculatePumpMakeColdForced();
}

typedef	void (*FPActionParser_T)(U8 *buf, I16 len);
typedef struct _parser_list_
{
    U16 Type;
    FPActionParser_T  pParserPkt;
} SParserList_T;
const static SParserList_T parserList[] =
{
    /* CHECK COLD/ICE SYSTEM */
    { EOL_CMD_COMP_ON,                  ParserCompOn                },            
    { EOL_CMD_COMP_OFF,                 ParserCompOff               },
    { EOL_CMD_COMP_COMM_CHECK,          NULL                        },
    { PKT_EOL_ICE_SYSTEM_ICE,           ParserIceSystemIce          }, 
    { PKT_EOL_ICE_SYSTEM_COLD,          ParserIceSystemCold         }, 
    { PKT_EOL_ICE_SYSTEM_DEICING,       ParserIceSystemDeicing      },

    { EOL_CMD_COMPLETE,                 ParserComplete              },
    { EOL_CMD_MODEL,                    ParserStart                 },
    { EOL_CMD_SETTING,                  ParserSensor                },             

    /* ETC */
    { EOL_CMD_UV_WATER_OUT,             NULL                        },
    { EOL_CMD_FLOW_SESNOR,              NULL                        },
    { EOL_CMD_AC_DC,                    NULL                        },
    { EOL_CMD_UV_ICE_DOOR,              NULL                        },
    { EOL_CMD_FAN_FB,                   NULL                        },
    { EOL_CMD_PUMP_DRAIN,               NULL                        },
    { EOL_CMD_PUMP_ICE_DRAIN,           NULL                        },
    { EOL_CMD_UV_ICE_TANK,              NULL                        },
    { EOL_CMD_ICE_FULL_LOW,             NULL                        },
    { EOL_CMD_UV_DRAIN_TANK,            NULL                        },
    { EOL_CMD_UV_ICE_TRAY,              NULL                        },
    { EOL_CMD_ICE_FULL_HIGH,            NULL                        },
    { EOL_CMD_PUMP_CIRCULATE,           NULL                        },

    /* SENSOR */
    { EOL_CMD_SENSOR_TEMP,              NULL                        }, 
    { EOL_CMD_SENSOR_LEVEL,             NULL                        }, 
    { EOL_CMD_SENSOR,                   NULL                        }, 

    /* LOAD */
    { EOL_CMD_NO_LOAD,                  ParserNoLoad                },             

    { EOL_CMD_HEATER_ON,                ParserHeaterOn              },    
    { EOL_CMD_HEATER_OFF,               ParserHeaterOff             },   

    { EOL_CMD_COLD_IN_VALVE_ON,         ParserColdInValveOn         },
    { EOL_CMD_COLD_IN_VALVE_OFF,        ParserColdInValveOff        },

    { EOL_CMD_ROOM_OUT_VALVE_ON,        ParserRoomOutValveOn        },
    { EOL_CMD_ROOM_OUT_VALVE_OFF,       ParserRoomOutValveOff       },

    { EOL_CMD_COLD_OUT_VALVE_ON,        ParserColdOutValveOn        },
    { EOL_CMD_COLD_OUT_VALVE_OFF,       ParserColdOutValveOff       },

    { EOL_CMD_HOT_OUT_VALVE_ON,         ParserHotOutValveOn         },
    { EOL_CMD_HOT_OUT_VALVE_OFF,        ParserHotOutValveOff        },

    { EOL_CMD_ICE_WATER_OUT_VALVE_ON,   ParserIceWaterOutValveOn    },
    { EOL_CMD_ICE_WATER_OUT_VALVE_OFF,  ParserIceWaterOutValveOff   },

    { EOL_CMD_FEED_VALVE_ON,            ParserFeedValveOn           },
    { EOL_CMD_FEED_VALVE_OFF,           ParserFeedValveOff          },

    { EOL_CMD_NOS_VALVE_ON,             ParserNosValveOn            },   
    { EOL_CMD_NOS_VALVE_OFF,            ParserNosValveOff           },  

    { EOL_CMD_ICE_TRAY_VALVE_ON,        ParserIceTrayValveOn        },        
    { EOL_CMD_ICE_TRAY_VALVE_OFF,       ParserIceTrayValveOff       },       

    { EOL_CMD_DRAIN_VALVE_ON,           ParserDrainValveOn          },  
    { EOL_CMD_DRAIN_VALVE_OFF,          ParserDrainValveOff         }, 

    { EOL_CMD_NOS_FLOW_ON,              ParserNosFlowValveOn        },
    { EOL_CMD_NOS_FLOW_OFF,             ParserNosFlowValveOff       },

    { EOL_CMD_FAN_MOTOR_ON,             ParserFanOn                 },        
    { EOL_CMD_FAN_MOTOR_OFF,            ParserFanOff                },
    
    { PKT_EOL_GAS_SWITCH_ON,            ParserGasSwitchOn           },
    { PKT_EOL_GAS_SWITCH_OFF,           ParserGasSwitchOff          },
    
    { PKT_EOL_ICE_TRAY_MOTOR_ON,        ParserIceTrayMotorOn        },
    { PKT_EOL_ICE_TRAY_MOTOR_OFF,       ParserIceTrayMotorOff       },
    
    { PKT_EOL_ICE_DOOR_MOTOR_ON,        ParserIceDoorMotorOn        },
    { PKT_EOL_ICE_DOOR_MOTOR_OFF,       ParserIceDoorMotorOff       },
    
    { EOL_CMD_ICE_FEEDER_MOTOR_CW_ON,   ParserFeederMotorCwOn       },
    { EOL_CMD_ICE_FEEDER_MOTOR_CW_OFF,  ParserFeederMotorCwOff      },
    
    { EOL_CMD_ICE_FEEDER_MOTOR_CCW_ON,  ParserFeederMotorCcwOn      },
    { EOL_CMD_ICE_FEEDER_MOTOR_CCW_OFF, ParserFeederMotorCcwOff     },
    
    { PKT_EOL_SWING_BAR_ON,             ParserSwingBarOn            },
    { PKT_EOL_SWING_BAR_OFF,            ParserSwingBarOff           },
    
    { PKT_EOL_HOT_GAS_SWITCH_ON,        ParserHotGasSwitchOn        },
    { PKT_EOL_HOT_GAS_SWITCH_OFF,       ParserHotGasSwitchOff       },

    { EOL_CMD_ICE_DRAIN_PUMP_ON,        ParserIceDrainPumpOn        },
    { EOL_CMD_ICE_DRAIN_PUMP_OFF,       ParserIceDrainPumpOff       },
    
    { EOL_CMD_DRAIN_PUMP_ON,            ParserDrainPumpOn           },
    { EOL_CMD_DRAIN_PUMP_OFF,           ParserDrainPumpOff          },
    
    { EOL_CMD_CIRCULATE_PUMP_ON,        ParserColdCirculatePumpOn   },
    { EOL_CMD_CIRCULATE_PUMP_OFF,       ParserColdCirculatePumpOff  },

    /* DEBUG MESSAGE */
    { EOL_CMD_DEBUG_CONTROL,         ParserDebugControl             },
    { EOL_CMD_DEBUG_1,               NULL                           },
    { EOL_CMD_DEBUG_2,               NULL                           },
    { EOL_CMD_DEBUG_3,               NULL                           },
    { EOL_CMD_DEBUG_4,               NULL                           },
    { EOL_CMD_DEBUG_5,               NULL                           },
    { EOL_CMD_DEBUG_6,               NULL                           },
    { EOL_CMD_DEBUG_7,               NULL                           },
};

#define SZ_PARSER_LIST ( sizeof(parserList) / sizeof(SParserList_T) )

// ?�신 받�? ?�킷 ?�싱
I16 ParserPktEol(U8 *buf, I16 len)
{
    U8 i = 0;
    U16 type = 0;
    FPActionParser_T pParserPkt = NULL;

    // Get Packet
    type = GetPacketCmd( &buf[FIELD_PACKET_TYPE], LENGTH_PACKET_TYPE );
    for( i=0; i<SZ_PARSER_LIST; i++ )
    {
        if( type == parserList[i].Type )
        {
            pParserPkt = parserList[i].pParserPkt;
            if( pParserPkt != NULL )
            {
                pParserPkt(&buf[FIELD_PAYLOAD], len);
            }

            // ACK 
            SetCommHeader(COMM_ID_EOL, type);
            StartTimer(TIMER_USER, TIMER_USER_ID_COMM_EOL_TX, 0);
            break;
        }
    }

    return 0;
}

/**
 * Packet Make
**/
/* Make checksum */
I16	Crc16Eol(U8 *buf, I16 len)
{
    U8 checkSum = 0;
    U8 crcBuf[5];

    if( len < MIN_PKT_SZ )
    {
        return 0; // error..
    }

    checkSum = Check_Crc(buf, len);
    SPRINTF( (char __FAR *)crcBuf, (const char __FAR *)"%02X", checkSum );

    buf[len - 3] = crcBuf[0];
    buf[len - 2] = crcBuf[1];

    return len;
}

static I16 MakePkt_NoPayLoad(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[ len++ ] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%04X", type );
    buf[ len++ ] = 0;     // CRC - UPPER
    buf[ len++ ] = 0;     // CRC - LOWER
    buf[ len++ ] = PKT_ETX;

    return len;
}

static U8 GetCompBldcCommError(void)
{
    if( GetCompBldcErrorComm() == TRUE )
    {
        return FALSE;
    }
  
    return TRUE;    
}

static I16 MakePkt_CompCommCheck(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%02X", GetCompBldcCommError() );
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16	MakePkt_Model(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", MODEL_CODE );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", GetEolTestMode() );
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

U8 GetSystemEol(void)
{
    /// 0x00 : Only Cp
    /// 0x01 : Only Chp
    /// 0x02 : Only Hp

    return 0x01;    // Only CHP
}

static I16 MakePkt_Setting(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", GetEolCheckStatus(EOL_CHECK_ID_EEPROM) );  /// EEPROM �׽�Ʈ �����
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02d", GetFrontVerMiner() );      /// Front ���α׷� ����
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02d", GetMainVerMiner() );       /// Main ���α׷� ����
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", 0 );                       /// UNUSED - ����� ������
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", COUNTRY_CODE );           /// �����ڵ�
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", GetSystemEol() );         /// �ÿ� �� ����          
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16 MakePkt_UvWaterOut(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_WATER_OUT_OFF) ); /// Uv Water Out ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_WATER_OUT) );     /// Uv Water Out ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16	MakePkt_FlowSensor(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_FLOW_METER) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_FLOW_METER_CUR) );
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static U16 CalcCurrent(U16 adc)
{
    F32 fCurrent = 0.0f;

    fCurrent = (F32)adc * 2.37f;
    fCurrent += 0.5f;
    return (U16)(fCurrent);
}

static I16 MakePkt_CurrentAcDc(U8 *buf, U16 type)
{
#if 0
    I16 len = 0;
    U16 rawAdc = 0U;
    U16 current = 0U;

    buf[ len++ ] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%04X", type );

    // 24V Feedback CURRENT
    rawAdc = GetAdcData(ADC_ID_POWER_24V_FB);
    current = CalcCurrent(rawAdc);
    len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%06d", current );    // 24V ?�드�?

    buf[ len++ ] = 0;     // CRC - UPPER
    buf[ len++ ] = 0;     // CRC - LOWER
    buf[ len++ ] = PKT_ETX;

    return len;
#endif

    return 0;
}

static I16 MakePkt_UvIceDoor(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_ICE_DOOR_OFF) ); /// Uv Ice Door ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_ICE_DOOR) );     /// Uv Ice Door ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16	MakePkt_FanFb( U8 *xUptrBuf, U16 xUcmd )
{
    I16 len = 0;
    U16 uRawAdc = 0U;
    U16 uCurrent = 0U;

    xUptrBuf[ len++ ] = PKT_STX;
    len += SPRINTF( (char __FAR *)&xUptrBuf[ len ], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&xUptrBuf[ len ], (const char __FAR *)"%04X", xUcmd );
    len += SPRINTF( (char __FAR *)&xUptrBuf[ len ], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_FAN_OFF) );    /// Fan ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&xUptrBuf[ len ], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_FAN) );        /// Fan ���� �ǵ��
    xUptrBuf[ len++ ] = 0;     // CRC - UPPER
    xUptrBuf[ len++ ] = 0;     // CRC - LOWER
    xUptrBuf[ len++ ] = PKT_ETX;

    return len;
}

static I16 MakePkt_PumpDrain(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_DRAIN_PUMP_OFF) );    /// Drain Pump ���� OFF �ǵ��
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_DRAIN_PUMP) );        /// Drain Pump ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16 MakePkt_PumpIceDrain(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_ICE_DRAIN_PUMP_OFF) );    /// Ice Drain Pump ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_ICE_DRAIN_PUMP) );        /// Ice Drain Pump ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16 MakePkt_UvIceTank(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_ICE_TANK_OFF) ); /// Uv Ice Tank ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_ICE_TANK) );     /// Uv Ice Tank ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16 MakePkt_UvDrainTank(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_DRAIN_TANK_OFF) ); /// Uv Extra ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_DRAIN_TANK) );     /// Uv Extra ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16 MakePkt_UvIceTray(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_ICE_TRAY_OFF) ); /// Uv Ice Tray ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_UV_ICE_TRAY) );     /// Uv Ice Tray ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16 MakePkt_IceFullHigh(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_HIGH_OFF) ); /// Ice Full ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_HIGH) );     /// Ice Full ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16 MakePkt_IceFullLow(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_LOW_OFF) ); /// Ice Full ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_ICE_FULL_LOW) );     /// Ice Full ���� �ǵ��
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

static I16 MakePkt_PumpCirculate(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_CIRCULATE_PUMP_OFF) ); // Circulate Pump ���� OFF �ǵ��    
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetEolCheckStatus(EOL_CHECK_ID_CIRCULATE_PUMP) );     // Circulate Pump ���� �ǵ��    
    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

// �µ��� EOL �������� ��ȯ
#define TEMP_ERROR        (9999)    // open, short
static U16 ConvEolTemp(U8 adcId, TEMP_T xTemp)
{
    U16 temp = TEMP_ERROR;
    U16 adc = 0U;

    adc = GetAdcData(adcId);
    if( GetTempErrorType(adc) != STATUS_NORMAL )
    {
        temp = TEMP_ERROR;
    }
    else
    {
        // min/max => -99.9 ['C] ~ +99.9['C]
        if( xTemp > 99.9f )
        {
            xTemp = 99.9f;
        }
        else if( xTemp < -99.0f )
        {
            xTemp = -99.9f;
        }

        if( xTemp > 0.0f )
        {
            temp = (U16)(xTemp * 100.0f); /// �Ҽ��� ù° �ڸ����� ǥ��
        }
        else
        {
            /// ������ ���... 1xxx�� ǥ��...
            xTemp = -xTemp;                /// ��ȣ ����
            temp = (U16)(xTemp * 100.0f); /// �Ҽ��� ù° �ڸ����� ǥ��
            //uTemp += 1000;
        }
    }

    return temp;
}

static I16 MakePkt_SensorTemp(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type ); 

    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04d", ConvEolTemp(ADC_ID_TEMP_AMBIENT, GetTemp(TEMP_ID_AMBIENT)) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetAdcData(ADC_ID_TEMP_AMBIENT) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04d", ConvEolTemp(ADC_ID_TEMP_COLD_WATER, GetTemp(TEMP_ID_COLD_WATER)) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetAdcData(ADC_ID_TEMP_COLD_WATER) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04d", ConvEolTemp(ADC_ID_TEMP_HOT_WATER,   GetTemp(TEMP_ID_HOT_WATER)) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetAdcData(ADC_ID_TEMP_HOT_WATER) );   
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04d", ConvEolTemp(ADC_ID_TEMP_EVA_1, GetTemp(TEMP_ID_EVA_1)) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetAdcData(ADC_ID_TEMP_EVA_1) );     
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04d", ConvEolTemp(ADC_ID_TEMP_EVA_2, GetTemp(TEMP_ID_EVA_2)) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetAdcData(ADC_ID_TEMP_EVA_2) );     
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04d", ConvEolTemp(ADC_ID_TEMP_AMBIENT_2, GetTemp(TEMP_ID_AMBIENT_2)) );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", GetAdcData(ADC_ID_TEMP_AMBIENT_2) );

    buf[ len++ ] = 0;     // CRC - UPPER
    buf[ len++ ] = 0;     // CRC - LOWER
    buf[ len++ ] = PKT_ETX;

    return len;
}

static I16 MakePkt_Sensor(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );

    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GetServiceCheckTankOpen() );

    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GetCurrentInputVal(INPUT_ID_TRAY_BRACKET) );

    if( GetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_DEICING) == 0 && GetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_ICING) == 0 )
    {
        // ��ü �̰���
        len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%02X", 0 );
    }
    else if( GetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_DEICING) == 0 && GetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_ICING) == 1 )
    {
        // Ż�� �̰���
        len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%02X", 1 );
    }
    else if( GetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_DEICING) == 1 && GetEolCheckStatus(EOL_CHECK_ID_ICE_TRAY_ICING) == 0 )
    {
        // ���� �̰���
        len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%02X", 2 );
    }
    else
    {
        // ����/Ż�� ��� ����
        len += SPRINTF( (char __FAR *)&buf[ len ], (const char __FAR *)"%02X", 3 );
    }

    buf[ len++ ] = 0;     // CRC - UPPER
    buf[ len++ ] = 0;     // CRC - LOWER
    buf[ len++ ] = PKT_ETX;

    return len;
}

// ���� ���� ��ȯ
// ü���̸� 0x01, ��ü�� 0x00
// ���� ���� ��ü�� ���¸� high...
// �� ���� �����̸� high...
// ü�� �����ε� �������̸� low
// �¼����������� open �� undetect (��ü��), short �� detect (ü��)
// �������� ���� �̰����� (��ü��), ������ (ü��)
static I16 MakePkt_SensorLevel(U8 *buf, U16 type)
{
    I16 len = 0;

    buf[len++] = PKT_STX;
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", PKT_ACK );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%04X", type );

    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GET_STATUS_LEVEL_ROOM_TANK_LOW() );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GET_STATUS_LEVEL_ROOM_TANK_HIGH() );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GET_STATUS_LEVEL_COLD_TANK_LOW() );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GET_STATUS_LEVEL_COLD_TANK_HIGH() );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GET_STATUS_LEVEL_DRAIN_TANK_LOW() );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GET_STATUS_LEVEL_DRAIN_TANK_HIGH() );
    len += SPRINTF( (char __FAR *)&buf[len], (const char __FAR *)"%02X", !GET_STATUS_LEVEL_ROOM_TANK_OVERFLOW() );

    buf[len++] = 0;     // CRC - UPPER
    buf[len++] = 0;     // CRC - LOWER
    buf[len++] = PKT_ETX;

    return len;
}

// ........

typedef	I16	(*FPActionMake_T)( U8 *buf, U16 type);
typedef struct _make_list_
{
    U16 Type;
    FPActionMake_T pMakePkt;
} SMakeList_T;
const static SMakeList_T makeList[] = 
{
    /* CHECK COLD/ICE SYSTEM */
    { EOL_CMD_COMP_ON,                  MakePkt_NoPayLoad       },            
    { EOL_CMD_COMP_OFF,                 MakePkt_NoPayLoad       },           
    { EOL_CMD_COMP_COMM_CHECK,          MakePkt_CompCommCheck   },
    { PKT_EOL_ICE_SYSTEM_ICE,           MakePkt_NoPayLoad       }, 
    { PKT_EOL_ICE_SYSTEM_COLD,          MakePkt_NoPayLoad       }, 
    { PKT_EOL_ICE_SYSTEM_DEICING,       MakePkt_NoPayLoad       }, 

    { EOL_CMD_COMPLETE,                 MakePkt_NoPayLoad       },

    { EOL_CMD_MODEL,                    MakePkt_Model           },
    { EOL_CMD_SETTING,                  MakePkt_Setting         },             

    /* ETC */
    { EOL_CMD_UV_WATER_OUT,             MakePkt_UvWaterOut      },
    { EOL_CMD_FLOW_SESNOR,              MakePkt_FlowSensor      },
    { EOL_CMD_AC_DC,                    MakePkt_CurrentAcDc     },
    { EOL_CMD_UV_ICE_DOOR,              MakePkt_UvIceDoor       },
    { EOL_CMD_FAN_FB,                   MakePkt_FanFb           },
    { EOL_CMD_PUMP_DRAIN,               MakePkt_PumpDrain       },
    { EOL_CMD_PUMP_ICE_DRAIN,           MakePkt_PumpIceDrain    },
    { EOL_CMD_UV_ICE_TANK,              MakePkt_UvIceTank       },
    { EOL_CMD_ICE_FULL_LOW,             MakePkt_IceFullLow      },
    { EOL_CMD_UV_DRAIN_TANK,            MakePkt_UvDrainTank     },
    { EOL_CMD_UV_ICE_TRAY,              MakePkt_UvIceTray       },
    { EOL_CMD_ICE_FULL_HIGH,            MakePkt_IceFullHigh     },
    { EOL_CMD_PUMP_CIRCULATE,           MakePkt_PumpCirculate   },

    /* SENSOR */
    { EOL_CMD_SENSOR_TEMP,              MakePkt_SensorTemp      },
    { EOL_CMD_SENSOR,                   MakePkt_Sensor          },
    { EOL_CMD_SENSOR_LEVEL,             MakePkt_SensorLevel     },

    /* LOAD */
    { EOL_CMD_NO_LOAD,                  MakePkt_NoPayLoad },

    { EOL_CMD_HEATER_ON,                MakePkt_NoPayLoad },
    { EOL_CMD_HEATER_OFF,               MakePkt_NoPayLoad },

    { EOL_CMD_COLD_IN_VALVE_ON,         MakePkt_NoPayLoad },
    { EOL_CMD_COLD_IN_VALVE_OFF,        MakePkt_NoPayLoad },

    { EOL_CMD_ROOM_OUT_VALVE_ON,        MakePkt_NoPayLoad },
    { EOL_CMD_ROOM_OUT_VALVE_OFF,       MakePkt_NoPayLoad },

    { EOL_CMD_COLD_OUT_VALVE_ON,        MakePkt_NoPayLoad },
    { EOL_CMD_COLD_OUT_VALVE_OFF,       MakePkt_NoPayLoad },

    { EOL_CMD_HOT_OUT_VALVE_ON,         MakePkt_NoPayLoad },
    { EOL_CMD_HOT_OUT_VALVE_OFF,        MakePkt_NoPayLoad },

    { EOL_CMD_ICE_WATER_OUT_VALVE_ON,   MakePkt_NoPayLoad },
    { EOL_CMD_ICE_WATER_OUT_VALVE_OFF,  MakePkt_NoPayLoad },

    { EOL_CMD_FEED_VALVE_ON,            MakePkt_NoPayLoad },
    { EOL_CMD_FEED_VALVE_OFF,           MakePkt_NoPayLoad },

    { EOL_CMD_NOS_VALVE_ON,             MakePkt_NoPayLoad },   
    { EOL_CMD_NOS_VALVE_OFF,            MakePkt_NoPayLoad },  

    { EOL_CMD_ICE_TRAY_VALVE_ON,        MakePkt_NoPayLoad },        
    { EOL_CMD_ICE_TRAY_VALVE_OFF,       MakePkt_NoPayLoad },       

    { EOL_CMD_DRAIN_VALVE_ON,           MakePkt_NoPayLoad },  
    { EOL_CMD_DRAIN_VALVE_OFF,          MakePkt_NoPayLoad }, 

    { EOL_CMD_NOS_FLOW_ON,              MakePkt_NoPayLoad },   
    { EOL_CMD_NOS_FLOW_OFF,             MakePkt_NoPayLoad },  

    { EOL_CMD_FAN_MOTOR_ON,             MakePkt_NoPayLoad },      
    { EOL_CMD_FAN_MOTOR_OFF,            MakePkt_NoPayLoad },
    
    { PKT_EOL_GAS_SWITCH_ON,            MakePkt_NoPayLoad },
    { PKT_EOL_GAS_SWITCH_OFF,           MakePkt_NoPayLoad },
    
    { PKT_EOL_ICE_TRAY_MOTOR_ON,        MakePkt_NoPayLoad },
    { PKT_EOL_ICE_TRAY_MOTOR_OFF,       MakePkt_NoPayLoad },
    
    { PKT_EOL_ICE_DOOR_MOTOR_ON,        MakePkt_NoPayLoad },
    { PKT_EOL_ICE_DOOR_MOTOR_OFF,       MakePkt_NoPayLoad },
    
    { EOL_CMD_ICE_FEEDER_MOTOR_CW_ON,   MakePkt_NoPayLoad },
    { EOL_CMD_ICE_FEEDER_MOTOR_CW_OFF,  MakePkt_NoPayLoad },
    
    { EOL_CMD_ICE_FEEDER_MOTOR_CCW_ON,  MakePkt_NoPayLoad },
    { EOL_CMD_ICE_FEEDER_MOTOR_CCW_OFF, MakePkt_NoPayLoad },
    
    { PKT_EOL_SWING_BAR_ON,             MakePkt_NoPayLoad },
    { PKT_EOL_SWING_BAR_OFF,            MakePkt_NoPayLoad },
    
    { PKT_EOL_HOT_GAS_SWITCH_ON,        MakePkt_NoPayLoad },
    { PKT_EOL_HOT_GAS_SWITCH_OFF,       MakePkt_NoPayLoad },

    { EOL_CMD_ICE_DRAIN_PUMP_ON,        MakePkt_NoPayLoad },
    { EOL_CMD_ICE_DRAIN_PUMP_OFF,       MakePkt_NoPayLoad },
    
    { EOL_CMD_DRAIN_PUMP_ON,            MakePkt_NoPayLoad },
    { EOL_CMD_DRAIN_PUMP_OFF,           MakePkt_NoPayLoad },
    
    { EOL_CMD_CIRCULATE_PUMP_ON,        MakePkt_NoPayLoad },
    { EOL_CMD_CIRCULATE_PUMP_OFF,       MakePkt_NoPayLoad },     

    /* DEBUG MESSAGE */
    { EOL_CMD_DEBUG_CONTROL,            NULL },
    { EOL_CMD_DEBUG_1,                  MakePkt_Debug_1 },
    { EOL_CMD_DEBUG_2,                  MakePkt_Debug_2 },
    { EOL_CMD_DEBUG_3,                  MakePkt_Debug_3 },
    { EOL_CMD_DEBUG_4,                  MakePkt_Debug_4 },
    { EOL_CMD_DEBUG_5,                  MakePkt_Debug_5 },
    { EOL_CMD_DEBUG_6,                  MakePkt_Debug_6 },
    { EOL_CMD_DEBUG_7,                  MakePkt_Debug_7 },
};
#define	SZ_MAKE_LIST	( sizeof(makeList) / sizeof(SMakeList_T) )

I16	MakePktEol(CommHeader_T *pComm, U8 *buf)
{
    U16 cmd = 0;
    U8 i = 0;
    I16 len	= -1;
    FPActionMake_T pMakePkt = NULL;

    for( i=0; i<SZ_MAKE_LIST; i++ )
    {
        cmd = makeList[i].Type;
        pMakePkt = makeList[i].pMakePkt;

        if( cmd == (U16)pComm )
        {
            if( pMakePkt != NULL )
            {
                len = pMakePkt(buf, cmd);
            }
            break;
        }
    }

    return len;
}
