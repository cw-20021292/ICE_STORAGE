/**
 * File : led.h
 * 
 * Led
**/
#include "hw.h"
#include "led.h"

#include "api_led.h"

#define MAX_DUTY           (100U)     // Max Duty 100%  (Tick = 10)
#define MIN_DUTY           (1U)       // Min Duty 1%    (Tick = 0)
#define MAX_DUTY_TICK      (10U)      // 10ms / 100Hz @1ms intterupt
#define MAX_SYNC_DELAY     (25U)      // 25ms / 40Hz @1ms interrupt

typedef struct _led_cnt_
{
    U16 LedCntCycle;
    U16 LedCntSyncDelay;
} SLedCnt_T;

SLedCnt_T LedCnt;

void InitLed(void)
{
    LedCnt.LedCntCycle = MAX_DUTY_TICK;
    LedCnt.LedCntSyncDelay = MAX_SYNC_DELAY;

    LedInit();
}

static U16 ConvDuty2Tick(U8 duty)
{
    U16 tick = 0U;

    if( duty > MAX_DUTY )
    {
        duty = MAX_DUTY;
    }
    else if( duty < MIN_DUTY )
    {
        duty = MIN_DUTY;
    }

    tick = (U16)((MAX_DUTY_TICK * (U16)duty) / MAX_DUTY);

    return tick;
}

typedef void (*FPLedOnOff_T)(U8 onOff);
typedef struct _led_list_
{
    ELedId_T        Led;
    U16             TargetTick;
    U16             CurrentTick;
    FPLedOnOff_T    LedOnOff;
} SLedList_T;

SLedList_T LedList[] = 
{
    /* led_id                              Target_Tick  CurrentTick  Fun_LedOnOff */
    { LED_ID_UV_CARE_SYSTEM,                0,          0,           LedOnOffUvCareSystem            },
    { LED_ID_UV_ICE_TANK,                   0,          0,           LedOnOffUvIceTank            },
    { LED_ID_UV_ICE_TRAY,                   0,          0,           LedOnOffUvIceTray             },
    { LED_ID_UV_ICE_FAUCET,                 0,          0,           LedOnOffUvIceFaucet          },
    { LED_ID_UV_WATER_FAUCET,               0,          0,           LedOnOffUvWaterFaucet   },
    { LED_ID_STATUS_ICE_WATER,              0,          0,           LedOnOffStatusIceWater  },
    { LED_ID_STATUS_MAX,                    0,          0,           LedOnOffStatusMax  },
    { LED_ID_STATUS_WATER,                  0,          0,           LedOnOffStatusWater },
    { LED_ID_STATUS_ICE_PREPARING,          0,          0,           LedOnOffStatusIcePreparing },
    { LED_ID_STATUS_ICE_FULL,               0,          0,           LedOnOffStatusIceFull },
    { LED_ID_STATUS_WATER_PREPARING,        0,          0,           LedOnOffStatusWaterPreparing },
    { LED_ID_STATUS_UNIT_CELSIUS,           0,          0,           LedOnOffStatusUnitCelsius },
    { LED_ID_STATUS_BAR_LOW,                0,          0,           LedOnOffStatusBarLow },
    { LED_ID_STATUS_BAR_MIDDLE,             0,          0,           LedOnOffStatusBarMiddle },
    { LED_ID_STATUS_BAR_HIGH,               0,          0,           LedOnOffStatusBarHigh },
    { LED_ID_STATUS_BAR_INFINITE,           0,          0,           LedOnOffStatusBarInfinite },
    { LED_ID_STATUS_UNIT_ML,                0,          0,           LedOnOffStatusUnitMl },
    { LED_ID_WELCOME_RIGHT_BLUE,            0,          0,           LedOnOffWelcomeRightBlue },
    { LED_ID_WELCOME_RIGHT_GREEN,           0,          0,           LedOnOffWelcomeRightGreen },
    { LED_ID_WELCOME_RIGHT_RED,             0,          0,           LedOnOffWelcomeRightRed },
    { LED_ID_WELCOME_RIGHT_WHITE,           0,          0,           LedOnOffWelcomeRightWhite },
    { LED_ID_WELCOME_LEFT_BLUE,             0,          0,           LedOnOffWelcomeLeftBlue },
    { LED_ID_WELCOME_LEFT_GREEN,            0,          0,           LedOnOffWelcomeLeftGreen },
    { LED_ID_WELCOME_LEFT_RED,              0,          0,           LedOnOffWelcomeLeftRed },
    { LED_ID_WELCOME_LEFT_WHITE,            0,          0,           LedOnOffWelcomeLeftWhite },
    { LED_ID_AMOUNT,                        0,          0,           LedOnOffAmount },
    { LED_ID_COLD,                          0,          0,           LedOnOffCold },
    { LED_ID_ROOM,                          0,          0,           LedOnOffRoom },
    { LED_ID_HOT,                           0,          0,           LedOnOffHot },
    { LED_ID_ICE_WATER_AMOUNT,              0,          0,           LedOnOffIceWaterAmount },
    { LED_ID_ICE_WATER,                     0,          0,           LedOnOffIceWater },
    { LED_ID_ICE,                           0,          0,           LedOnOffIce },
    { LED_ID_STATUS_COOLING,                0,          0,           LedOnOffStausCooling },
    { LED_ID_STATUS_LOW_WATER,              0,          0,           LedOnOffStausLowWater },
    { LED_ID_STATUS_HEATING,                0,          0,           LedOnOffStausHeating },
    { LED_ID_STATUS_HOT_LOCK,               0,          0,           LedOnOffStausHotLock },
    { LED_ID_STATUS_ICE_WATER_AMOUNT_LOCK,  0,          0,           LedOnOffStausIceWaterAmountLock },
    { LED_ID_STATUS_ICE_WATER_LOCK,         0,          0,           LedOnOffStausIceWaterLock },
    { LED_ID_STATUS_ICE_LOCK,               0,          0,           LedOnOffStausIceLock },
    { LED_ID_ALL_LOCK,                      0,          0,           LedOnOffAllLock },
    { LED_ID_COLD_OFF,                      0,          0,           LedOnOffColdOff },
    { LED_ID_HOT_OFF,                       0,          0,           LedOnOffHotOff },
    { LED_ID_HOT_LOCK,                      0,          0,           LedOnOffHotLock },
    { LED_ID_ICE_OFF,                       0,          0,           LedOnOffIceOff },
    { LED_ID_ICE_WATER_LOCK,                0,          0,           LedOnOffIceWaterLock },
    { LED_ID_ICE_LOCK,                      0,          0,           LedOnOffIceLock },
    
    { LED_ID_FND_1_A,                       0,          0,           NULL},
    { LED_ID_FND_1_B,                       0,          0,           NULL},   
    { LED_ID_FND_1_C,                       0,          0,           NULL},   
    { LED_ID_FND_1_D,                       0,          0,           NULL},   
    { LED_ID_FND_1_E,                       0,          0,           NULL},   
    { LED_ID_FND_1_F,                       0,          0,           NULL},   
    { LED_ID_FND_1_G,                       0,          0,           NULL},

    { LED_ID_FND_2_A,                       0,          0,           NULL},
    { LED_ID_FND_2_B,                       0,          0,           NULL},   
    { LED_ID_FND_2_C,                       0,          0,           NULL},   
    { LED_ID_FND_2_D,                       0,          0,           NULL},   
    { LED_ID_FND_2_E,                       0,          0,           NULL},   
    { LED_ID_FND_2_F,                       0,          0,           NULL},   
    { LED_ID_FND_2_G,                       0,          0,           NULL},

    { LED_ID_FND_3_A,                       0,          0,           NULL},
    { LED_ID_FND_3_B,                       0,          0,           NULL},   
    { LED_ID_FND_3_C,                       0,          0,           NULL},   
    { LED_ID_FND_3_D,                       0,          0,           NULL},   
    { LED_ID_FND_3_E,                       0,          0,           NULL},   
    { LED_ID_FND_3_F,                       0,          0,           NULL},   
    { LED_ID_FND_3_G,                       0,          0,           NULL},

    { LED_ID_FND_4_A,                       0,          0,           NULL},
    { LED_ID_FND_4_B,                       0,          0,           NULL},   
    { LED_ID_FND_4_C,                       0,          0,           NULL},   
    { LED_ID_FND_4_D,                       0,          0,           NULL},   
    { LED_ID_FND_4_E,                       0,          0,           NULL},   
    { LED_ID_FND_4_F,                       0,          0,           NULL},   
    { LED_ID_FND_4_G,                       0,          0,           NULL},

    { LED_ID_FND_5_A,                       0,          0,           NULL},
    { LED_ID_FND_5_B,                       0,          0,           NULL},   
    { LED_ID_FND_5_C,                       0,          0,           NULL},   
    { LED_ID_FND_5_D,                       0,          0,           NULL},   
    { LED_ID_FND_5_E,                       0,          0,           NULL},   
    { LED_ID_FND_5_F,                       0,          0,           NULL},   
    { LED_ID_FND_5_G,                       0,          0,           NULL},

    { LED_ID_FND_6_A,                       0,          0,           NULL},
    { LED_ID_FND_6_B,                       0,          0,           NULL},   
    { LED_ID_FND_6_C,                       0,          0,           NULL},   
    { LED_ID_FND_6_D,                       0,          0,           NULL},   
    { LED_ID_FND_6_E,                       0,          0,           NULL},   
    { LED_ID_FND_6_F,                       0,          0,           NULL},   
    { LED_ID_FND_6_G,                       0,          0,           NULL},
};

#define SZ_LED_LIST       ( sizeof(LedList) / sizeof(SLedList_T) )

static void OnOffTick(SLedList_T *ledList, U16 ledCntCycle)
{
    if( ledCntCycle < ledList->CurrentTick )
    {
        ledList->LedOnOff(ON);
    }
    else
    {
        ledList->LedOnOff(OFF);
    }
}

static void Sync(SLedList_T *ledList)
{
    if( ledList->TargetTick > ledList->CurrentTick )
    {
        ledList->CurrentTick++;
    }
    else if( ledList->TargetTick < ledList->CurrentTick )
    {
        ledList->CurrentTick--;
    }
}

void OnOffLed(ELedId_T id, U8 duty)
{
    SLedList_T *pLedList = NULL;
    U8         ledType = 0U;
    U8         ledDuty = 0U;

    ledType = ( duty & (0xF0) );
    ledDuty = ConvertProtocol2Duty( duty & (0x0F) );

    pLedList = &LedList[id];

    pLedList->TargetTick = ConvDuty2Tick(ledDuty);

    if( ledType == LED_TYPE_DUTY )
    {
        pLedList->CurrentTick = ConvDuty2Tick(ledDuty);    
    }
}

void AllOffLed(void)
{
    U8 i = 0U;

    for( i=0; i<LED_ID_MAX; i++ )
    {
        OnOffLed((ELedId_T)i, 0U);
    }
}

U8 ConvertProtocol2Duty(U8 data)
{
    U8 result = 0U;

    switch( data )
    {
        case PROTOCOL_LED_DUTY_100 :
            result = 100;
            break;

        case PROTOCOL_LED_DUTY_90 :
            result = 90;
            break;

        case PROTOCOL_LED_DUTY_80 :
            result = 80;
            break;

        case PROTOCOL_LED_DUTY_70 :
            result = 70;
            break;

        case PROTOCOL_LED_DUTY_60 :
            result = 60;
            break;

        case PROTOCOL_LED_DUTY_50 :
            result = 50;
            break;

        case PROTOCOL_LED_DUTY_40 :
            result = 40;
            break;

        case PROTOCOL_LED_DUTY_30 :
            result = 30;
            break;

        case PROTOCOL_LED_DUTY_20 :
            result = 20;
            break;

        case PROTOCOL_LED_DUTY_10 :
            result = 10;
            break;

        default :
            result = 0;
            break;
    }

    return result;
}

void ControlLed(void)
{
    U8 i = 0U;
    
    LedCnt.LedCntCycle++;
    if( LedCnt.LedCntCycle > MAX_DUTY_TICK )
    {
        LedCnt.LedCntCycle = 0U;
    }

    LedCnt.LedCntSyncDelay--;

    for(i=0; i<SZ_LED_LIST; i++ )
    {
        OnOffTick(&LedList[i], LedCnt.LedCntCycle);

        if( LedCnt.LedCntSyncDelay == 0 )
        {
            Sync(&LedList[i]);
        }
    }

    if( LedCnt.LedCntSyncDelay == 0 )
    {
        LedCnt.LedCntSyncDelay = MAX_SYNC_DELAY;
    }
}
