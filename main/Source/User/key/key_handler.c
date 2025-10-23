/**
 * File : key_handler.c
 *
 * Key Normal Main Handler Function
*/
#include "key_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "front.h"
#include "hot_water.h"
#include "cold_water.h"
#include "room_water.h"
#include "water_out.h"
#include "tank_clean.h"
#include "ice.h"
#include "ice_maker.h"
#include "ice_door.h"
#include "ice_tray.h"
#include "power_saving.h"
#include "sound.h"
#include "eeprom.h"
#include "error.h"
#include "process_display.h"
#include "rtc.h"
#include "error.h"
#include "drain.h"
#include "health.h"
#include "ster.h"
#include "drain_water.h"
#include "eol.h"
#include "fct.h"
#include "time_short.h"
#include "timer.h"
#include "relay.h"
#include "display_lib.h"
#include "ice_water_out.h"
#include "smart_saving.h"
#include "service.h"
#include "ice_tray_test.h"
#include "aging.h"

#if CONFIG_TEST
#include "comp.h"
#endif

static U8 SelSound(void);

static U8 StartFeeder(void);

static U8 StartBackFeeder(void);

/* 얼음 버튼 */
static U8 SelIce(void);

/* 얼음물 버튼 */
static U8 SelIceWater(void);

/* 얼음물 용량 버튼 */
static U8 SelIceWaterConty(void);

static U8 OffLockIceWaterAmount(void);
static U8 OnLockIceWaterAmount(void);

/* 온수 버튼 */
static U8 SelHot(void);
static U8 OnHotMake(void);
static U8 OffHotMake(void);
static U8 OnOffHotMake(void);

/* 냉수 버튼 */
static U8 SelCold(void);
static U8 OnColdMake(void);
static U8 OffColdMake(void);
static U8 OnOffColdMake(void);

/* 정수 버튼 */
static U8 SelRoom(void);
static U8 OnDecoLed(void);
static U8 OffDecoLed(void);
static U8 ClearMemento(void);

/* 연속 추출 */
static U8 SelConty(void);
static U8 SelContyInfinity(void);

/* 설정 버튼 */
static U8 SelSetting(void);
static U8 OnOffSetting(void);

/* 제빙 모드 SET */
static U8 SetIceMode(void);

/* 제빙 설정 */
static U8 OnIceMake(void);

/* 제빙 해제 */
static U8 OffIceMake(void);

/* 제빙 설정/해제 */
static U8 OnOffIceMake(void);

/* 쾌속 제빙 설정/해제 */
static U8 OnOffIceTurbo(void);

/* 얼음 잠금 버튼 */
static U8 OnOffIceLock(void);

/* 얼음물 잠금 버튼 */
static U8 OnOffIceWaterLock(void);

/* 온수 잠금 버튼 */
static U8 OnOffHotLock(void);

/* 절전 버튼 */
static U8 OnOffSave(void);

static U8 OnVersion(void);
static U8 OnOffMemento(void);

static U8 OnLockAll(void);
static U8 OffLockAll(void);
static U8 OnOffLockAll(void);

static U8 OnOffMute(void);

/* 온수 고도 설정 */
static U8 OnOffAltitude(void);

/* 아이스 트레이 점검 테스트 */
static U8 OnTestIceTray(void);

/* 강제 살균 종료 */
static U8 OnResetSter(void);

/* 강제 탈빙 */
static U8 OnDeIcing(void);

/* EEPROM 초기화 */
static U8 OnEraseEeprom(void);

/* 에이징 테스트 */
static U8 OnAgingTest(void);

/* 물 정량 */
static void ClearAmountContinue(void);

/* 얼음물 정량 */
static void ClearIceWaterAmountContinue(void);

/* 스마트 미사용 절전 모드 기능 ON/OFF */
static U8 OnOffSmartUnusedSaveMode(void);

/* WIFI */
#if 0
static U8 OnWifiAp(void);
static U8 OnWifiBle(void);
static U8 OnWifiOnOff(void);
#endif

/* HEALTH - 살균/배수 */
static U8 OnOffHealth(void); // 살균/배수 설정/해제 
static U8 OffDrainWater(void); // 배수 해제 
static U8 OnManualSter(void); // 강제 살균
static U8 OnManualDrainWater(void); // 강제 배수
static U8 OnTempMonitor(void);  // 외기1 / 외기2 / 냉수 온도 모니터링
static U8 OnAdjustHour(void);  // 살균 시간 조정
static U8 OnSetHour(void);  // 살균 시간 설정
static U8 OnOffDrainWater(void);  // 자동 배수 설정/해제

/* EOL */
static U8 OnEolStart(void);
static U8 OnMainFrontStart(void);
static U8 OnFrontStart(void);
static U8 OnShipmentStart(void);
static U8 OnFctStart(void);
static U8 OnTimeShort(void);

static U8 OnTestShortCompTime(void);
static U8 OnTestStopDummyDeicing(void);
static U8 OnTestStartSterConty(void);

/* UV TEST CODE */
static U8 OnOffUvIceTank(void);
static U8 OnOffUvDrainTank(void);
static U8 OnOffUvIceTray(void);
static U8 OnOffUvIceOut(void);
static U8 OnOffUvWaterOut(void);

// 일반 모드 리스트
SKeyEventList_T KeyEventList[] =
{
    /* KEY,                Short,              2sec,         3sec,                      5sec,              7sec,                   Pop,  TS */
    /* SINGLE KEY */
#if( CONFIG_TEST_8585_FEEDER == 1 )
    
    { K_ICE,               StartFeeder,        NULL,         OnIceMake,                 NULL,              OffIceMake,             NULL, NULL },

#elif( CONFIG_TEST_8585_BACK_FEEDER == 1 )

    { K_ICE,               StartBackFeeder,    NULL,         OnIceMake,                 NULL,              OffIceMake,             NULL, NULL },
    
#else /* CONFIG_TEST_8585_FEEDER == 0 */

    { K_ICE,               SelIce,             NULL,         OnIceMake,                 NULL,              OffIceMake,             NULL, NULL },    

#endif /* CONFIG_TEST_8585_FEEDER END */
    
    { K_ICE_WATER,         SelIceWater,        NULL,         NULL,                      NULL,              NULL,                   NULL, NULL },

#if( CONFIG_TEST_UV_MANUAL == 1 )    

    { K_ICE_WATER_AMOUNT,  SelIceWaterConty,   NULL,         OffLockIceWaterAmount,     NULL,              OnLockIceWaterAmount,   NULL, OnOffUvDrainTank },    
    { K_HOT,               SelHot,             NULL,         NULL,                      NULL,              NULL,                   NULL, OnOffUvIceOut },
    { K_ROOM,              SelRoom,            NULL,         NULL,                      NULL,              NULL,                   NULL, OnOffUvIceTank },
    { K_COLD,              SelCold,            NULL,         NULL,                      NULL,              NULL,                   NULL, OnOffUvIceTray },
    { K_AMOUNT,            SelConty,           NULL,         NULL,                      NULL,              NULL,                   NULL, OnOffUvWaterOut },

#else /* CONFIG_UV_MANUAL == 0 */

    { K_ICE_WATER_AMOUNT,  SelIceWaterConty,   NULL,         OffLockIceWaterAmount,     NULL,              OnLockIceWaterAmount,   NULL, NULL },    
    { K_HOT,               SelHot,             NULL,         OnHotMake,                 NULL,              OffHotMake,             NULL, NULL },
    { K_ROOM,              SelRoom,            NULL,         NULL,                      NULL,              NULL,                   NULL, NULL },
    { K_COLD,              SelCold,            NULL,         OnColdMake,                NULL,              OffColdMake,            NULL, NULL },
#if( CONFIG_SET_ICE_MODE == 1 )
    { K_AMOUNT,            SelConty,           NULL,         SetIceMode,                NULL,              OnOffSetting,           NULL, NULL },
#else
    { K_AMOUNT,            SelConty,           NULL,         NULL,                      NULL,              OnOffSetting,           NULL, NULL },
#endif

#endif /* CONFIG_UV_MANUAL END */

    { K_ICE_LOCK,          OnOffIceLock,       NULL,         NULL,                      NULL,              NULL,                   NULL, NULL },
    { K_ICE_WATER_LOCK,    OnOffIceWaterLock,  NULL,         NULL,                      NULL,              NULL,                   NULL, NULL },
    { K_HOT_LOCK,          OnOffHotLock,       NULL,         NULL,                      NULL,              NULL,                   NULL, NULL },
    { K_LOCK_ALL,          SelSound,           NULL,         OnOffLockAll,              NULL,              NULL,                   NULL, NULL },

    /* MULTI KEY */
    { K_MUTE,              SelSound,           NULL,         OnOffMute,                 NULL,              NULL,                   NULL, NULL },
    { K_ALTITUDE,          SelSound,           NULL,         OnOffAltitude,             NULL,              NULL,                   NULL, NULL },
    { K_DEICING,           SelSound,           NULL,         OnDeIcing,                 NULL,              NULL,                   NULL, NULL },

    { K_ERASE_EEPROM,      SelSound,           NULL,         OnEraseEeprom,             NULL,              NULL,                   NULL, NULL },
    { K_MANUAL_DRAIN,      SelSound,           NULL,         OnManualDrainWater,        NULL,              NULL,                   NULL, NULL },

#if( CONFIG_ENABLE_ICON_AIS_ICETRAY_TEST == 1 )
    { K_TEST_ICE_TRAY,     SelSound,           NULL,         OnTestIceTray,             NULL,              NULL,                   NULL, NULL },
#endif

    { K_TEMP_MONITOR,      SelSound,           NULL,         NULL,                      OnTempMonitor,     NULL,                   NULL, NULL },
    { K_SMART_UNUSED_SAVE, SelSound,           NULL,         OnOffSmartUnusedSaveMode,  NULL,             NULL,                   NULL, NULL },  

#if( CONFIG_STER == 1 )
    { K_DRAIN_WATER,       SelSound,           NULL,         NULL,                      OffDrainWater,     NULL,                   NULL, NULL },
    { K_MANUAL_STER,       SelSound,           NULL,         NULL,                      OnManualSter,      NULL,                   NULL, NULL },
    //{ K_ADJUST_HOUR,    SelSound,       NULL,            NULL,         OnAdjustHour,        NULL,  NULL, NULL },
    { K_SET_HOUR,          SelSound,           NULL,         OnSetHour,                 NULL,              NULL,                   NULL, NULL },
#endif

    /* TEST */
#if( CONFIG_TEST == 1 )
    { K_TEST_COMP,         SelSound,           OnTestShortCompTime,    NULL,  NULL, NULL, NULL, NULL },
    { K_TEST_DEICING,      SelSound,           OnTestStopDummyDeicing, NULL,  NULL, NULL, NULL, NULL },
    { K_TEST_STER_CONTY,   SelSound,           OnTestStartSterConty,   NULL,  NULL, NULL, NULL, NULL },
#endif

    { K_TEST_TS,           SelSound,           OnTimeShort,            NULL,  NULL, NULL, NULL, NULL },
    { K_TEST_FCT,          SelSound,           OnFctStart,             NULL,  NULL, NULL, NULL, NULL },
    { K_TEST_MAIN_FRONT,   SelSound,           OnMainFrontStart,       NULL,  NULL, NULL, NULL, NULL },
    { K_TEST_FRONT,        SelSound,           OnFrontStart,           NULL,  NULL, NULL, NULL, NULL },    
    { K_TEST_SHIPMENT,     SelSound,           OnShipmentStart,        NULL,  NULL, NULL, NULL, NULL },
};

// 테스트 모드(TIME SHORT) 리스트
SKeyEventList_T KeyEventTimeShortList[] =
{
    /* KEY,               Short,            2sec,            3sec,        5sec,    7sec,         Pop,  TS */
    /* SINGLE KEY */
    { K_ICE,              SelIce,           NULL,            NULL,        NULL,    NULL,         NULL, OnOffIceMake },    
    { K_ICE_WATER,        SelIceWater,      NULL,            NULL,        NULL,    NULL,         NULL, NULL },
    { K_ICE_WATER_AMOUNT, SelIceWaterConty, NULL,            NULL,        NULL,    NULL,         NULL, NULL },    
    { K_HOT,              SelHot,           NULL,            NULL,        NULL,    NULL,         NULL, OnOffHotMake },
    { K_ROOM,             SelRoom,          NULL,            ClearMemento,NULL,    NULL,         NULL, NULL },
    { K_COLD,             SelCold,          NULL,            NULL,        NULL,    NULL,         NULL, OnOffColdMake },
    { K_AMOUNT,           SelConty,         NULL,            NULL,        NULL,    OnOffSetting, NULL, NULL },

    { K_ICE_LOCK,         OnOffIceLock,     NULL,            NULL,        NULL,    NULL,         NULL, NULL },
    { K_ICE_WATER_LOCK,   OnOffIceWaterLock,NULL,            NULL,        NULL,    NULL,         NULL, NULL },
    { K_HOT_LOCK,         OnOffHotLock,     NULL,            NULL,        NULL,    NULL,         NULL, NULL },
    { K_LOCK_ALL,         SelSound,         NULL,            OnOffLockAll,NULL,    NULL,         NULL, NULL },

    /* MULTI KEY */
    { K_VERSION,          SelSound,         OnVersion,       NULL,        NULL,    NULL,         NULL, NULL },
    { K_MEMENTO,          SelSound,         NULL,            OnOffMemento,NULL,    NULL,         NULL, NULL },
    
    { K_MUTE,             SelSound,         NULL,            NULL,        NULL,    NULL,         NULL, OnOffMute        },
    { K_ALTITUDE,         SelSound,         NULL,            NULL,        NULL,    NULL,         NULL, OnOffAltitude    },
    { K_DEICING,          SelSound,         NULL,            NULL,        NULL,    NULL,         NULL, OnDeIcing        },

    { K_ERASE_EEPROM,     SelSound,         OnEraseEeprom,   NULL,        NULL,    NULL,         NULL, NULL },    
    { K_MANUAL_DRAIN,     SelSound,         NULL,            NULL,        NULL,    NULL,         NULL, OnManualDrainWater },
    { K_AGING_TEST,       SelSound,         OnAgingTest,     NULL,        NULL,    NULL,         NULL, NULL },    
    

#if( CONFIG_STER == 1 )
    { K_DRAIN_WATER,      SelSound,         NULL,            NULL,        NULL,    NULL,   NULL, OffDrainWater       },
    { K_MANUAL_STER,      SelSound,         NULL,            NULL,        NULL,    NULL,   NULL, OnManualSter        },
    //{ K_ADJUST_HOUR,    SelSound,         NULL,            NULL,        NULL,    NULL,   NULL, OnAdjustHour        },
    { K_SET_HOUR,         SelSound,         NULL,            NULL,        NULL,    NULL,   NULL, OnSetHour           },
#endif

    /* TEST */
#if( CONFIG_TEST == 1 )
    { K_TEST_COMP,       SelSound,     OnTestShortCompTime,    NULL, NULL, NULL, NULL, NULL },
    { K_TEST_DEICING,    SelSound,     OnTestStopDummyDeicing, NULL, NULL, NULL, NULL, NULL },
#endif
};

U8 IsValidKeyCondition(U32 key)
{
    /* CONFIG HOT ALTITUDE */
    if( GetAltidueConfig() == TRUE )
    {
        if( key != K_ALTITUDE 
                && key != K_HOT )
        {
            return FALSE;
        }
    }

#if 0
    /* CONFIG ADJUST STER TIME */
    if( GetDispAdjustHourConfig() == TRUE )
    {
        if( key != K_ADJUST_HOUR 
                && key != K_COLD
                && key != K_HOT
                && key != K_ROOM )
        {
            return FALSE;
        }
    }
#endif

#if 0
    /* CONFIG SET STER TIME */
    if( GetDispSetHourConfig() == TRUE )
    {
        if( key != K_SET_HOUR 
                && key != K_STER )
        {
            return FALSE;
        }
    }
#endif

#if 0
    /* CONFIG DRAIN WATER ON/OFF */
    if( GetDispDrainWaterConfig() == TRUE )
    {
        if( key != K_DRAIN_WATER 
                && key != K_COLD
                && key != K_ROOM )
        {
            return FALSE;
        }
    }
#endif

    /* MEMENTO DISPLAY */
    if( GetMementoDisp() == TRUE )
    {
        if( key != K_MEMENTO 
                && key != K_ROOM )
        {
            return FALSE;
        }
    }

    /* 전체 잠금 - 해제 버튼만 허용 */
    if( GetLockAll() == LOCK )
    {
        if( key != K_LOCK_ALL )
        {
            StartDisplayLockAll();
            //StartDisplayLockIcon();
            return FALSE;
        }
        else
        {
            return TRUE;
        }
    }

    /* SETTING MENU 에서만 허용 */
    if( GetSettingDisp() == TRUE )
    {
        if( ( key != K_ICE_LOCK )
            && ( key != K_ICE_WATER_LOCK )
            && ( key != K_HOT_LOCK )
            && ( key != K_AMOUNT )
            && ( key != K_LOCK_ALL )
        )
        {
            return FALSE;
        }

        // UPDATE SETTING MENU TIMER..
        StartDispTimerId(DISP_TIMER_ID_SETTING_MENU);
    }
    else
    {

#if 1
        if( ( key == K_HOT_LOCK )
            || ( key == K_ICE_WATER_LOCK )
            || ( key == K_ICE_LOCK )
            || ( key == K_LOCK_ALL )
        )

#else
        if( ( key == K_LOCK_ALL && GetLockAll() == UNLOCK )
            || ( key == K_COLD_OFF )
            || ( key == K_HOT_OFF )
            || ( key == K_HOT_LOCK && GetLockHot() == UNLOCK )
            || ( key == K_ICE_OFF )
            || ( key == K_ICE_WATER_LOCK && GetLockIceWater() == UNLOCK )
            || ( key == K_ICE_LOCK && GetLockIce() == UNLOCK )
        )
#endif
        {
            SetSkipInvalidSound(TRUE);
            return FALSE;
        }
    }

#if 0
    /* 살균 - 해제 버튼만 허용 */
    if( IsStartSter() == TRUE )
    {
        if( key != K_MANUAL_STER 
            && key != K_RESET_STER
#if( CONFIG_TEST == 1 )
                && key != K_ERASE_EEPROM
#endif
                )
        {
            StartDisplaySter();
            return FALSE;
        }
    }
#endif

#if 0
    /* 제빙 OFF 조건에서는, 쾌속 제빙 입력 불허 */
    if( GetIceConfigMake() == FALSE )
    {
        if( mu16Key == K_ICE_TURBO )
        {
            return FALSE;
        }
    }
#endif

    /* 물 추출 중 */
    if( GetWaterOut() == TRUE )
    {
        if( GetWaterOutSelect() == SEL_WATER_COLD )
        {
            if( key != K_AMOUNT
                && key != K_ICE
                && key != K_ICE_WATER )
            {
                return FALSE;
            }
        }
        else
        {
            if( key != K_AMOUNT
                && key != K_ICE 
                && key != K_ICE_WATER 
                && key != K_ICE_WATER_AMOUNT )
            {
                return FALSE;
            }
        }
    }

    /* 얼음 추출 중 */
    if( IsOpenIceDoor() == TRUE )
    {
        /* 얼음물 추출 중 */
        if( IsStartIceDoorWater() == TRUE )
        {
            if( key != K_ICE_WATER_AMOUNT
                && key != K_AMOUNT
                && key != K_ROOM
                && key != K_COLD
                && key != K_HOT )
            {
                return FALSE;
            }
        }
        else
        {
            if( key != K_AMOUNT
                && key != K_ROOM
                && key != K_COLD
                && key != K_HOT )
            {
                return FALSE;
            }
        }
    }

    /* 얼음물 추출 중 */
    if( IsStartIceDoorWater() == TRUE )
    {
        if( key != K_ICE_WATER_AMOUNT
            && key != K_AMOUNT
            && key != K_ROOM
            && key != K_COLD
            && key != K_HOT )
        {
            return FALSE;
        }
    }

    return TRUE;
}

void* GetKeyEventList(void)
{
    if( GetTimeShortStatus() == TRUE )
    {
        return KeyEventTimeShortList;
    }

    return KeyEventList;
}

U8 GetKeyEventListSize(void)
{
    if( GetTimeShortStatus() == TRUE )
    {
        return ( sizeof(KeyEventTimeShortList) / sizeof(SKeyEventList_T) );
    }

    return ( sizeof(KeyEventList) / sizeof(SKeyEventList_T) );
}

static U8 SetSelectWater(U8 id)
{
//    // 추출 중에는, 물 선택을 할 수 없다.
//    if( GetWaterOut() == TRUE )
//    {
//        return FALSE;   // error..
//    }

    SetWaterOutSelect(id);
    return TRUE;
}

static U8 SetSelectIce(U8 id)
{
    SetIceWaterSelect(id);
    return TRUE;
}

static U8 SelSound(void)
{
    return SOUND_SELECT;
}

#if( CONFIG_TEST_8585_FEEDER == 1 )
extern U8 the_8585_feeder;
extern U8 the_8585_step_feeder;
#endif
static U8 StartFeeder(void)
{
#if( CONFIG_TEST_8585_FEEDER == 1 )
    if( the_8585_feeder == FALSE )
    {
        the_8585_feeder = TRUE;
        StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_FEEDER, 100);
    }
    else
    {
        the_8585_feeder = FALSE;
        the_8585_step_feeder = 0;
        TurnOffRelay(RELAY_FEEDER_CW);
        TurnOffRelay(RELAY_FEEDER_CCW);
        StopTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_FEEDER);
    }
#endif
    return SOUND_SELECT;
}

#if( CONFIG_TEST_8585_BACK_FEEDER == 1 )
extern U8 the_8585_back_feeder;
extern U8 the_8585_step_back_feeder;
#endif
static U8 StartBackFeeder(void)
{
#if( CONFIG_TEST_8585_BACK_FEEDER == 1 )
    if( the_8585_back_feeder == FALSE )
    {
        the_8585_back_feeder = TRUE;
        StartTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_BACK_FEEDER, 100);
    }
    else
    {
        the_8585_back_feeder = FALSE;
        the_8585_step_back_feeder = 0;
        TurnOffRelay(RELAY_FEEDER_CW);
        TurnOffRelay(RELAY_FEEDER_CCW);
        StopTimer(TIMER_PROCESS, TIMER_ID_TEST_8585_BACK_FEEDER);
    }
#endif
    return SOUND_SELECT;
}


/* 얼음 버튼 */
static U8 SelIce(void)
{
    /* 얼음 잠금 시, 에러 표시 */
    if( GetLockIce() == TRUE )
    {
        StartDisplayIceLock();
        return SOUND_ERROR;
    }

    if( GetIceConfigMake() == FALSE)
    {
        StartDisplayIceMake();
        StartSegDisplaySegOffBlink();
        return SOUND_ERROR;
    }

    /* 얼음 선택 */
    if( SetSelectIce(SEL_ICE) == TRUE )
    {
        StartDisplayIceColorWait();
        ClearIceWaterAmountContinue();
        return SOUND_SELECT;
    }

    return SOUND_ERROR;
}

/* 얼음물 버튼 */
static U8 SelIceWater(void)
{
    if( GetLockIceWater() == LOCK )
    {
        StartDisplayIceWaterLock();
        return SOUND_ERROR;
    }

    if( GetIceConfigMake() == FALSE)
    {
        StartDisplayIceMake();
        StartSegDisplaySegOffBlink();
        return SOUND_ERROR;
    }

    if( GetWaterOut() == TRUE )
    {
        if( GetWaterOutSelect() == SEL_WATER_COLD )
        {
            StartDisplayNotIceWater();
            return SOUND_ERROR;      
        }
    }

    /* 얼음물 선택 */
    if( SetSelectIce(SEL_ICE_WATER) == TRUE )
    {
        StartDisplayIceColorWait();
        StopDisplayIceWaterAmountNoti();
        return SOUND_SELECT;
    }

    return SOUND_ERROR;
}

/* 온수 버튼 */
static U8 SelHot(void)
{
    /* 온수 고도 설정 변경 */
    if( GetAltidueConfig() == TRUE )
    {
        U8 mu8Altitude;

        mu8Altitude = GetHotWaterAltidue();
        mu8Altitude++;
        if( mu8Altitude >= ALTITUDE_LEVEL_NUM )
        {
            mu8Altitude = ALTITUDE_LEVEL_0;
        }
        SetHotWaterAltidue(mu8Altitude);

        SetAltitudeConfig(TRUE);  // Refreseh Altitude setup time

        return SOUND_SELECT;
    }

    /* Unused save config */
    if(GetSmartSaveConfigMode() == TRUE )
    {
        ResetSmartSaveConfigModeTimer();
           
        if( GetUnusedSavingConfig() == TRUE )
        {
            SetUnusedSavingConfig( FALSE );      
            SaveEepromId( EEP_ID_CONF_UNUSED_SAVE );
    
            return SOUND_CANCEL;
        }
        else
        {
            SetUnusedSavingConfig( TRUE );       
            SaveEepromId( EEP_ID_CONF_UNUSED_SAVE );
               
            return SOUND_SETUP;
        }
    }

#if 0
    // 살균 시간 보정 
    if( GetDispAdjustHourConfig() == TRUE )
    {
        SetHealthAdjustMode( ADJUST_MODE_BEFORE );
        SetDispAdjustHourConfig( TRUE );  // ????쒓컙 reload
        return SOUND_SETUP;
    }
#endif

    if( GetLockHot() == LOCK )
    {
        StartDisplayHotLock();
        return SOUND_ERROR;
    }

    if( GetHotWaterConfigMake() == FALSE)
    {
        StartDisplayHotMake();
        StartSegDisplaySegOffBlink();
        SetSelectWater( SEL_WATER_HOT );  
        return SOUND_ERROR;
    }

    /* 온수 선택 */
    if( SetSelectWater( SEL_WATER_HOT ) == TRUE )
    {
        StartDisplayWaterColorWait();
        return SOUND_SELECT;
    }

    return SOUND_ERROR;
}

/* 얼음물 용량 잠금 OFF */
U8 gu8IceWaterAmountLock = FALSE;
static U8 OffLockIceWaterAmount(void)
{
    if(GetIceWaterSelect() == SEL_ICE)
    {
        return SOUND_NONE;
    }

    if( GetLockIceWaterAmount() == FALSE )
    {
        return SOUND_NONE;
    }

    gu8IceWaterAmountLock = TRUE;
    SetLockIceWaterAmount(FALSE);
    StopDisplayIceWaterAmountLock();

    return SOUND_CONFIG_CANCEL;
}

/* 얼음물 용량 잠금 ON */
static U8 OnLockIceWaterAmount(void)
{
    if(GetIceWaterSelect() == SEL_ICE)
    {
        return SOUND_NONE;
    }

    if( GetLockIceWaterAmount() == TRUE )
    {
        return SOUND_NONE;
    }

    if( gu8IceWaterAmountLock == TRUE )
    {
        return SOUND_NONE;
    }

    SetLockIceWaterAmount(TRUE);
    return SOUND_CONFIG_SETUP;
}

/* 얼음물 용량 잠금 OnOff */
static U8 OnOffLockIceWaterAmount(void)
{
    U8 mu8Sound = SOUND_CONFIG_SETUP;

    if(GetIceWaterSelect() == SEL_ICE)
    {
        return SOUND_NONE;
    }

    if( GetLockIceWaterAmount() == FALSE )
    {
        SetLockIceWaterAmount(TRUE);
    }
    else
    {
        SetLockIceWaterAmount(FALSE);
        StopDisplayIceWaterAmountLock();
        mu8Sound = SOUND_CONFIG_CANCEL;
    }

    return mu8Sound;
}

/* 온수 생성 On */
U8 gu8HotOff = FALSE;
static U8 OnHotMake(void)
{
    if( GetHotWaterConfigMake() == TRUE )
    {
        return SOUND_NONE;
    }

    gu8HotOff = TRUE;
    SetHotWaterConfigMake(TRUE);
    StopDisplayHotMake();
    StartSegDisplaySegOn();

    return SOUND_CONFIG_SETUP;
}

/* 온수 생성 Off */
static U8 OffHotMake(void)
{
    if( GetHotWaterConfigMake() == FALSE )
    {
        return SOUND_NONE;
    }

    if( gu8HotOff == TRUE )
    {
        return SOUND_NONE;
    }

    StartSegDisplaySegOff();
    SetHotWaterConfigMake(FALSE);
    return SOUND_CONFIG_CANCEL;
}

/* 온수 생성 OnOff */
static U8 OnOffHotMake(void)
{
    U8 mu8Sound = SOUND_CONFIG_SETUP;

    if( GetHotWaterConfigMake() == FALSE )
    {
        SetHotWaterConfigMake(TRUE);
        StartSegDisplaySegOn();
    }
    else
    {
        SetHotWaterConfigMake(FALSE);
        StartSegDisplaySegOff();
        mu8Sound = SOUND_CONFIG_CANCEL;
    }

    return mu8Sound;
}

/* 냉수 버튼 */
static U8 SelCold(void)
{
#if 0
    // 살균 시간 보정 
    if( GetDispAdjustHourConfig() == TRUE )
    {
        SetHealthAdjustMode( ADJUST_MODE_AFTER );
        SetDispAdjustHourConfig( TRUE );  // 표시 시간 reload
        return SOUND_SETUP;
    }
#endif

#if 0
    // 자동 배수 해제
    if( GetDispDrainWaterConfig() == TRUE )
    {
        ClearHealthMode(HEALTH_MODE_DRAIN);
        SetDispDrainWaterConfig( TRUE );  // 표시 시간 reload
        return SOUND_SETUP;
    }
#endif

    if( GetColdWaterConfigMake() == FALSE)
    {
        StartDisplayColdMake();
        StartSegDisplaySegOffBlink();
        //SetSelectWater(SEL_WATER_COLD);
        return SOUND_ERROR;
    }

    if( IsOpenIceDoor() == TRUE )
    {
        /* 얼음물 추출 중 */
        if( IsStartIceDoorWater() == TRUE )
        {
            StartDisplayNotColdWater();
            return SOUND_ERROR;
        }
    }

    if( SetSelectWater(SEL_WATER_COLD) == TRUE )
    {
        StartDisplayWaterColorWait();
        return SOUND_SELECT;
    }

    return SOUND_ERROR;
}

/* 냉수 생성 On */
U8 gu8ColdOff = FALSE;
static U8 OnColdMake(void)
{
    if( GetColdWaterConfigMake() == TRUE )
    {
        return SOUND_NONE;
    }

    gu8ColdOff = TRUE;
    SetColdWaterConfigMake(TRUE);
    StopDisplayColdMake();
    StartSegDisplaySegOn();

    return SOUND_CONFIG_SETUP;
}

static U8 OffColdMake(void)
{
    if( GetColdWaterConfigMake() == FALSE )
    {
        return SOUND_NONE;
    }

    if( gu8ColdOff == TRUE )
    {
        return SOUND_NONE;
    }

    StartSegDisplaySegOff();
    SetColdWaterConfigMake(FALSE);

    return SOUND_CONFIG_CANCEL;
}

static U8 OnOffColdMake(void)
{
    U8 sound = SOUND_CONFIG_SETUP;

    if( GetColdWaterConfigMake() == FALSE )
    {
        SetColdWaterConfigMake(TRUE);
        StopDisplayColdMake();
        StartSegDisplaySegOn();
    }
    else
    {
        SetColdWaterConfigMake(FALSE);
        StartSegDisplaySegOff();
        sound = SOUND_CONFIG_CANCEL;
    }

    return sound;
}

/* 정수 버튼 */
static U8 SelRoom(void)
{
#if 0
    // 살균 시간 보정 
    if( GetDispAdjustHourConfig() == TRUE )
    {
        SetHealthAdjustMode( ADJUST_MODE_NONE );
        SetDispAdjustHourConfig( TRUE );  // 표시 시간 reload
        return SOUND_SETUP;
    }
#endif

#if 0
    // 자동 배수 설정
    if( GetDispDrainWaterConfig() == TRUE )
    {
        SetHealthMode(HEALTH_MODE_DRAIN);
        SetDispDrainWaterConfig(TRUE);  // 표시 시간 reload
        return SOUND_SETUP;
    }
#endif

    if( GetMementoDisp() == TRUE )
    {
        return SOUND_NONE;
    }

    if( SetSelectWater(SEL_WATER_ROOM) == TRUE )
    {
        StartDisplayWaterColorWait();
        return SOUND_SELECT;
    }
    return SOUND_ERROR;
}

static U8 OnDecoLed(void)
{
    if( GetConfigDeco(DECO_LED_WELCOME_WATER) == TRUE )
    {
        return SOUND_NONE;
    }

    SetConfigDeco(DECO_LED_WELCOME_WATER, TRUE);
    return SOUND_SETUP;
}

static U8 OffDecoLed(void)
{
    /* DECO LED OFF */
    if( GetConfigDeco(DECO_LED_WELCOME_WATER) == TRUE )
    {
        SetConfigDeco(DECO_LED_WELCOME_WATER, FALSE);
        return SOUND_CANCEL;
    }

    return SOUND_NONE;
}

static U8 ClearMemento(void)
{
    /* MEMENTO CLEAR */
    if( GetMementoDisp() == TRUE )
    {
        ClearMementoError();

        return SOUND_SELECT;
    }

    return SOUND_NONE;
}

// 추출 중?
static U8 IsWaterOutConty(U8 out, U8 conty)
{
    // 추출 중인데,
    // 연속은 연속이고,
    // 레버 입력이 없는 상태
    if( out == TRUE      
            && conty == TRUE  
            && GetLeverWater() == 0   
      )
    {
        return TRUE;    // 추출 중
    }

    return FALSE;   // 추출 없음
}

// 일반 추출 중?
static U8 IsWaterOut(U8 out, U8 conty)
{
    // 추출 중인데,
    // 연속은 아니고,
    // 레버 입력 중인 경우
    if( out == TRUE      
            && conty == FALSE  
            && GetLeverWater() != 0   
      )
    {
        return TRUE;    // 추출 중
    }

    return FALSE;   // 추출 없음
}

// 얼음물 추출 중?
static U8 IsIceWaterOutConty(U8 out, U8 conty)
{
    // 추출 중인데,
    // 연속은 연속이고,
    // 레버 입력이 없는 상태
    if( out == TRUE      
            && conty == TRUE  
            && GetLeverIce() == 0   
      )
    {
        return TRUE;    // 추출 중
    }

    return FALSE;   // 추출 없음
}

// 얼음물 일반 추출 중?
static U8 IsIceWaterOut(U8 out, U8 conty)
{
    // 추출 중인데,
    // 연속은 아니고,
    // 레버 입력 중인 경우
    if( out == TRUE      
            && conty == FALSE  
            && GetLeverIce() != 0   
      )
    {
        return TRUE;    // 추출 중
    }

    return FALSE;   // 추출 없음
}

static void ClearAmountContinue(void)
{
    SetWaterOutContinue(FALSE);
    ClearWaterOutInfinityAmout();
}

static void ClearIceWaterAmountContinue(void)
{
    SetIceWaterOutContinue(FALSE);
    ClearIceWaterOutInfinityAmout();
}

/* 연속 추출 */
static U8 SelConty(void)
{
    U8 sound = SOUND_SELECT;
    U8 waterOut;
    U8 conty;
    U8 amount;

    //SetIceWaterOutContinue(FALSE);
    //ClearIceWaterOutInfinityAmout();

    // 설정 화면 EXIT 
    if( GetSettingDisp() == TRUE )
    {
        SetSettingDisp(FALSE);
        StopDispTimerId(DISP_TIMER_ID_SETTING_MENU);
        //ClearAmountContinue();
        return SOUND_CANCEL;
    }

    if( IsStartIceDoorWater() == TRUE 
            && GetIceWaterOutContinue() == TRUE )
    {
        return SOUND_ERROR;
    }

    SelectWaterPanel();

    waterOut  = GetWaterOut();
    conty = GetWaterOutContinue();
    amount = GetWaterOutAmoutType();


    // 1. 연속 추출 중에, 추출 선택 버튼 선택으로 추출 중지
    if( IsWaterOutConty(waterOut, conty) == TRUE )
    {
        StopWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        SetWaterOutContinue(FALSE);
        ClearWaterOutInfinityAmout();
        return SOUND_EFFLUENT_END;
    }

    // 2. 추출 중에, 연속 추출 모드를 선택하는 경우..
    if( IsWaterOut(waterOut, conty) == TRUE )
    {
        SetWaterOutContinue(TRUE);

        // 탱크 비우기 모드
        if( IsTankCleanMode() == TRUE )
        {
            SetTankClean(TRUE);
            StartDrainPumpOnLevel();
        }

        return SOUND_EFFLUENT;
    }

    // 탱크 비우기 모드 중에는 연속 즉시 해제 
    if( GetTankClean() == TRUE )
    {
        StopWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        SetWaterOutContinue( FALSE );
        return SOUND_EFFLUENT_END;
    }

    // 3. 추출 중이 아닐 때,  연속 추출 및 추출양 선택 
    if( conty == TRUE )
    {
#if 0
        // 연속 추출 해제 
        if( mu8Amount == FIXED_AMOUNT_INFINITY )
        {
            SetWaterOutContinue( FALSE );
            ClearWaterOutInfinityAmout();
            return SOUND_CANCEL;
        }
#endif

        // 정량 추출 선택 변경
        if( amount == FIXED_AMOUNT_INFINITY )
        {
            amount = FIXED_AMOUNT_500ML;
            conty  = FALSE;
            sound = SOUND_CANCEL;
            ClearWaterOutInfinityAmout();
        }
        else if( amount == FIXED_AMOUNT_1D5L )
        {
            amount = FIXED_AMOUNT_INFINITY;
        }
        else if( amount == FIXED_AMOUNT_1L )
        {
            amount = FIXED_AMOUNT_1D5L;
        }
        else if( amount == FIXED_AMOUNT_500ML )
        {
            amount = FIXED_AMOUNT_1L;
        }
        else 
        {
            amount = FIXED_AMOUNT_500ML;
        }
    }
    else
    {
        conty  = TRUE;
    }

    if( waterOut == TRUE )
    {
        if( amount == FIXED_AMOUNT_INFINITY )
        {
            sound = SOUND_EFFLUENT_CONTINUE;
        }
        else
        {
            sound = SOUND_EFFLUENT;
        }
    }

    UpdateWaterOutReturnTime();

    SetWaterOutContinue(conty);
    SetWaterOutFixedAmountType(amount);

    StopSegDisplaySegOffBlink();
    StopSegDisplaySegOff();
    StopSegDisplaySegOn();

    return sound;
}

/* 얼음물 연속 추출 */
static U8 SelIceWaterConty(void)
{
    U8 sound = SOUND_SELECT;
    U8 iceWaterOut;
    U8 iceWaterConty;
    U8 iceWaterAmount;

    /* 얼음 선택 되어있을 때는 얼음물 용량 선택 불가 */
    if(GetIceWaterSelect() == SEL_ICE)
    {
        StartDisplayIceWaterAmountNoti();
        return SOUND_ERROR;
    }

    /* 얼음물 잠금 시 */
    if( GetLockIceWaterAmount() == LOCK )
    {
        StartDisplayIceWaterAmountLock();
        return SOUND_ERROR;
    }

    if( GetWaterOut() == TRUE 
            && GetWaterOutContinue() == TRUE )
    {
        return SOUND_ERROR;
    }

    SelectIcePanel();

    //SetWaterOutContinue(FALSE);
    //ClearWaterOutInfinityAmout();

    iceWaterOut  = IsStartIceDoorWater();
    iceWaterConty = GetIceWaterOutContinue();
    iceWaterAmount = GetIceWaterOutAmoutType();

    // 1. 연속 추출 중에, 추출 선택 버튼 선택으로 추출 중지
    if( IsIceWaterOutConty(iceWaterOut, iceWaterConty) == TRUE )
    {
        CloseIceDoorWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        SetIceWaterOutContinue(FALSE);
        ClearIceWaterOutInfinityAmout();
        return SOUND_EFFLUENT_END;
    }

    // 2. 추출 중에, 연속 추출 모드를 선택하는 경우..
    if( IsIceWaterOut(iceWaterOut, iceWaterConty) == TRUE )
    {
        SetIceWaterOutContinue(TRUE);

        return SOUND_EFFLUENT;
    }

    // 탱크 비우기 모드 중에는 연속 즉시 해제 
    if( GetTankClean() == TRUE )
    {
        StopWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        SetWaterOutContinue( FALSE );
        return SOUND_EFFLUENT_END;
    }

    // 3. 추출 중이 아닐 때,  연속 추출 및 추출양 선택 
    if( iceWaterConty == TRUE )
    {
#if 0
        // 연속 추출 해제 
        if( mu8Amount == FIXED_AMOUNT_INFINITY )
        {
            SetWaterOutContinue( FALSE );
            ClearWaterOutInfinityAmout();
            return SOUND_CANCEL;
        }
#endif

        // 정량 추출 선택 변경
#if 0
        if( iceWaterAmount == FIXED_ICE_WATER_AMOUNT_INFINITY )
        {
            iceWaterAmount = FIXED_ICE_WATER_AMOUNT_500ML;
            iceWaterConty  = FALSE;
            sound = SOUND_CANCEL;
            ClearIceWaterOutInfinityAmout();
        }
#endif
        if( iceWaterAmount == FIXED_ICE_WATER_AMOUNT_1L )
        {
            iceWaterAmount = FIXED_ICE_WATER_AMOUNT_500ML;
            iceWaterConty  = FALSE;
            sound = SOUND_CANCEL;
            ClearIceWaterOutInfinityAmout();
        }
        else if( iceWaterAmount == FIXED_ICE_WATER_AMOUNT_700ML )
        {
            iceWaterAmount = FIXED_ICE_WATER_AMOUNT_1L;
        }
        else if( iceWaterAmount == FIXED_ICE_WATER_AMOUNT_500ML )
        {
            iceWaterAmount = FIXED_ICE_WATER_AMOUNT_700ML;
        }
        else 
        {
            iceWaterAmount = FIXED_ICE_WATER_AMOUNT_500ML;
        }
    }
    else
    {
        iceWaterConty  = TRUE;
    }

    if( iceWaterOut == TRUE )
    {
        if( iceWaterAmount == FIXED_ICE_WATER_AMOUNT_INFINITY )
        {
            sound = SOUND_EFFLUENT_CONTINUE;
        }
        else
        {
            sound = SOUND_EFFLUENT;
        }
    }

    UpdateIceWaterOutReturnTime();

    SetIceWaterOutContinue(iceWaterConty);
    SetIceWaterOutFixedAmountType(iceWaterAmount);

    StopSegDisplaySegOffBlink();
    StopSegDisplaySegOff();
    StopSegDisplaySegOn();

    return sound;
}

/* 연속 추출 무제한 선택 */
static U8 SelContyInfinity(void)
{
    SetWaterOutContinue( TRUE );
    SetWaterOutFixedAmountType(FIXED_AMOUNT_INFINITY);

    if( GetWaterOut() == TRUE )
    {
        return SOUND_EFFLUENT_CONTINUE;
    }

    return SOUND_SETUP;
}

/* 얼음물 연속 추출 무제한 선택 */
static U8 SelIceWaterContyInfinity(void)
{
    SetIceWaterOutContinue( TRUE );
    SetIceWaterOutFixedAmountType(FIXED_AMOUNT_INFINITY);

    if( IsStartIceDoorWater() == TRUE )
    {
        return SOUND_EFFLUENT_CONTINUE;
    }

    return SOUND_SETUP;
}

/* 설정 메뉴 표시 */
static U8 SelSetting(void)
{
    if( GetWaterOut() == TRUE 
            || IsOpenIceDoor() == TRUE )
    {
        return SOUND_NONE;
    }

    // 설정 화면 EXIT 
    if( GetSettingDisp() == TRUE )
    {
        SetSettingDisp(FALSE);
        StopDispTimerId(DISP_TIMER_ID_SETTING_MENU);
        return SOUND_CANCEL;
    }

    return SOUND_SELECT;
}

// 설정 메뉴 진입.
// 시간 설정 중 : 시간 저장 기능.
static U8 OnOffSetting(void)
{
    if( GetWaterOut() == TRUE 
            || IsOpenIceDoor() == TRUE )
    {
        return SOUND_NONE;
    }

    // 설정 화면 EXIT 
    if( GetSettingDisp() == TRUE )
    {
        SetSettingDisp(FALSE);
        StopDispTimerId(DISP_TIMER_ID_SETTING_MENU);
        return SOUND_CANCEL;
    }

    // 설정 화면 표시
    SetSettingDisp(TRUE);
    StartDispTimerId(DISP_TIMER_ID_SETTING_MENU);
    //StopDispTimerId( DISP_TIMER_ID_KEY_AMOUNT );
    //StopDispTimerId( DISP_TIMER_ID_KEY_HOT );
    //StopDispTimerId( DISP_TIMER_ID_KEY_HOT_TEMP );

    ClearAmountContinue();
    ClearIceWaterAmountContinue();

    return SOUND_SETUP;
}

#include "comp.h"
static U8 SetIceMode(void)
{
    SetCompProtectOffTime(5);
    SetIceMakeMode(ICE_MODE_INIT | ICE_MODE_READY | ICE_MODE_ICING | ICE_MODE_DEICING);

    return SOUND_CONFIG_SETUP;
}

/* 얼음 생성 On */
U8 gu8IceOff = FALSE;
static U8 OnIceMake(void)
{
    if( GetIceConfigMake() == TRUE )
    {
        return SOUND_NONE;
    }

    gu8IceOff = TRUE;
    SetIceConfigMake(TRUE);
    StopDisplayIceMake();
    StopTimer(TIMER_PROCESS, TIMER_ID_DRAIN_PUMP_24H);
    StartSegDisplaySegOn();

    return SOUND_CONFIG_SETUP;
}

static U8 OffIceMake(void)
{
    if( GetIceConfigMake() == FALSE )
    {
        return SOUND_NONE;
    }

    if( gu8IceOff == TRUE )
    {
        return SOUND_NONE;
    }

    SetIceConfigMake(FALSE);
    // 강제 도어 닫기
    ForcedCloseIceDoor();
    // 드레인 탱크 배수 
    //TurnOnDrainPump();
    StartDrainPumpOnLevel();
    // 24시간 후, 드레인 탱크 배수
    StartTimer(TIMER_PROCESS, TIMER_ID_DRAIN_PUMP_24H, SEC(86400));

    StartSegDisplaySegOff(); // OFF 표시

    return SOUND_CONFIG_CANCEL;
}

/* 제빙 기능 설정 */
static U8 OnOffIceMake(void)
{
    if( GetIceConfigMake() == TRUE )
    {
        SetIceConfigMake(FALSE);
        // 강제 도어 닫기
        ForcedCloseIceDoor();
        // 드레인 탱크 배수 
        //TurnOnDrainPump();
        StartDrainPumpOnLevel();
        // 24시간 후, 드레인 탱크 배수
        StartTimer(TIMER_PROCESS, TIMER_ID_DRAIN_PUMP_24H, SEC(86400));

        StartSegDisplaySegOff(); // OFF 표시

        return SOUND_CANCEL;
    }

    SetIceConfigMake(TRUE);
    StopDisplayIceMake();
    StopTimer(TIMER_PROCESS, TIMER_ID_DRAIN_PUMP_24H);
    
    StartSegDisplaySegOn(); // On 표시
    return SOUND_SETUP;
}

/* 제빙 쾌속 설정 */
static U8 OnOffIceTurbo(void)
{
#if 0
    // 제빙 OFF이면, 제빙하고 쾌속 모두 ON 시킨다.
    if( GetIceConfigMake() == FALSE )
    {
        SetIceConfigMake( TRUE );
        SetIceTurbo( TRUE );

        StartDisplayIceMake();

        return SOUND_ERROR;
    }
#endif

    if( GetIceTurbo() == TRUE )
    {
        SetIceTurbo(FALSE);
        return SOUND_CANCEL;
    }

    SetIceTurbo(TRUE);
    return SOUND_SETUP;
}


/* 얼음 잠금 설정/해제 */
static U8 OnOffIceLock(void)
{
    if( GetLockIce() == LOCK )
    {
        SetLockIce(UNLOCK);
        StopDisplayIceLock();
        //StopDisplayIceLockSet();
        return SOUND_LOCK_CANCEL;
    }

    SetLockIce(LOCK);

    //StartDisplayIceLockSet();
    //StopDisplayIceWaterLockSet();
    //StopDisplayHotLockSet();
    StopDisplayLockAllSet();

    return SOUND_LOCK_SETUP;
}

/* 얼음물 잠금 설정/해제 */
static U8 OnOffIceWaterLock(void)
{
    if( GetLockIceWater() == LOCK )
    {
        SetLockIceWater(UNLOCK);
        StopDisplayIceWaterLock();
        //StopDisplayIceWaterLockSet();
        return SOUND_LOCK_CANCEL;
    }

    SetLockIceWater(LOCK);

    //StopDisplayIceLockSet();
    //StartDisplayIceWaterLockSet();
    //StopDisplayHotLockSet();
    StopDisplayLockAllSet();

    return SOUND_LOCK_SETUP;
}

/* 온수 잠금 버튼 */
static U8 OnOffHotLock(void)
{
#if 0
    /* 전체 잠금인 경우, 온수 잠금 기능 Skip */
    if( GetLockAll() == LOCK )
    {
        return SOUND_NONE;
    }
#endif

    /* 온수 잠금 / 해제 */
    if( GetLockHot() == UNLOCK )
    {
        // 온수 선택 중이면, 강제로 정수 선택으로 이동 시킨다.
        if( GetWaterOutSelect() == SEL_WATER_HOT )
        {
            SetWaterOutSelect(SEL_WATER_ROOM);
        }
        SetLockHot(LOCK);

        //StopDisplayIceLockSet();
        //StopDisplayIceWaterLockSet();
        //StartDisplayHotLockSet();
        StopDisplayLockAllSet();
        
        return SOUND_LOCK_SETUP;
    }

    SetLockHot(UNLOCK);
    StopDisplayHotLock();
    //StopDisplayHotLockSet();
    return SOUND_LOCK_CANCEL;
}


/* 살균 설정/해제 */
static U8 OnOffHealth(void)
{
#if 0
    // 살균 시간 설정 모드
    if( GetDispSetHourConfig() == TRUE )
    {
        SetHealthHourMode();
        SetDispSetHourConfig(TRUE);  // 표시 시간 reload
        return SOUND_SETUP;
    }

    // 살균 해제 
    if( GetHealthMode() != HEALTH_MODE_NONE  )
    {
        ClearHealthMode(HEALTH_MODE_ALL);
        return SOUND_CANCEL;
    }

    // RTC 에러 , 살균 시간 설정 유무, 살균 동작 가능 유무
    if( IsConfHealth() == FALSE )
    {
        return SOUND_ERROR;
    }

    // 살균 설정
    SetHealthMode(HEALTH_MODE_ALL);
#endif
    return SOUND_SETUP;
}

static U8 OffDrainWater(void)
{
    // 순환 살균 설정상태에서만 제한적으로 설정/해제 가능하다.
    if( IsSetHealthMode( HEALTH_MODE_STER ) == FALSE )
    {
        return SOUND_ERROR;
    }

    // 설정 종료
    if( GetDispDrainWaterConfig() == TRUE )
    {
        SetDispDrainWaterConfig(FALSE); 
        return SOUND_ERROR;
    }

    // 설정 시작
    SetDispDrainWaterConfig(TRUE); 
    return SOUND_SETUP;
}

/* 절전 설정/해제  */
static U8 OnOffSave(void)
{
    if( GetSavingConfig() == TRUE )
    {
        SetSavingConfig(FALSE);
        return SOUND_CANCEL;
    }

    SetSavingConfig(TRUE);
    return SOUND_SETUP;
}

/* 버전 표시 */
static U8 OnVersion(void)
{
    SetVersionDisp(0);

    return SOUND_SELECT;
}

/* 메멘토 표시 */
static U8 OnOffMemento(void)
{
    if( GetMementoDisp() == FALSE )
    {
        SetMementoDisp(TRUE);
    }
    else
    {
        SetMementoDisp(FALSE);
    }

    return SOUND_SELECT;
}

/* 전체 잠금 설정 */
static U8 OnLockAll(void)
{
    if( GetLockAll() == UNLOCK )
    {
        SetLockAll(LOCK);
        StopDisplayLockAll();
        return SOUND_LOCK_SETUP;
    }

    return SOUND_NONE;
}

/* 전체 잠금 해제 */
static U8 OffLockAll(void)
{
    if( GetLockAll() == LOCK )
    {
        SetLockAll(UNLOCK);
        return SOUND_LOCK_CANCEL;
    }

    return SOUND_NONE;
}

static U8 OnOffLockAll(void)
{
    U8 sound = SOUND_LOCK_SETUP;

    if( GetLockAll() == LOCK )
    {
        SetLockAll(UNLOCK);
        StopDisplayLockAll();
        StopDisplayLockAllSet();
        sound = SOUND_LOCK_CANCEL;
    }
    else
    {
        SetLockAll(LOCK);
        StopDisplayHotLockSet();
        StopDisplayIceLockSet();
        StartDisplayLockAllSet();
    }

    return sound;
}

// UnusedSave Config
static U8 OnOffSmartUnusedSaveMode(void)
{
    if( IsExpiredTimeShortLimitTimer() == TRUE )
    {
        if( GetSmartSaveConfigMode() == TRUE )
        {
            SetSmartSaveConfigMode(FALSE);
            return SOUND_CANCEL;
        }
		
        return SOUND_NONE;
    }

    if( GetSmartSaveConfigMode() == FALSE )
    {
        SetSmartSaveConfigMode(TRUE);
		
        return SOUND_SETUP;
    }
    else
    {
        SetSmartSaveConfigMode(FALSE);

        return SOUND_CANCEL;
    }

    return SOUND_NONE;
}

static U8 OnOffMute(void)
{
    if( IsSetSoundMute() == FALSE )
    {
        SetSoundMute();
    }
    else
    {
        ClearSoundMute();
        return SOUND_CANCEL;
    }

    return SOUND_MUTE;
}

/* 온수 고도 설정 */
static U8 OnOffAltitude(void)
{
    U8 sound;

    // 고도 설정 종료
    if( GetAltidueConfig() == TRUE )
    {
        SetAltitudeConfig(FALSE);
        SaveEepromId( EEP_ID_HOT_ALTITUDE );
        
        return SOUND_CANCEL;
    }

    // 고도 설정 진입 전에 시간 확인 후,
    // 설정 가능 시간 만료 이전에만 설정 모드로 변경 가능하다.
    if( IsExpiredConfigTime() == TRUE )
    {
        return SOUND_NONE;
    }

    SetAltitudeConfig(TRUE);
    return SOUND_SETUP;
}

static U8 OnTestIceTray(void)
{
    if( GetIceMakeMode() != ICE_MODE_NONE )
    {
        return SOUND_ERROR;
    }

    if( GetServiceCheckTankOpen() == TRUE )
    {
        if( GetIceTrayTestStatus() == FALSE )
        {
            StopDummyDeIcing();
            StopIceMake();
            StartIceTrayTest();
            return SOUND_SETUP;
        }
    }

    return SOUND_NONE;
}

static U8 OnResetSter(void)
{
#if 0
    if( ResetSter() == TRUE )
    {
        return SOUND_CANCEL;
    }
#endif

    return SOUND_NONE;
}

static U8 OnDeIcing(void)
{
    // 제빙 중일 때, 강제 탈빙을 동작 시킨다. 
    if( StopIceMake() == TRUE )
    {
        return SOUND_SETUP;
    }

    return SOUND_SELECT;
}

static U8 OnEraseEeprom(void)
{
    if( IsExpiredConfigTime() == FALSE )
    {
        EraseEeprom();
        return SOUND_SELECT;
    }

    return SOUND_NONE;
}

#if 0
static U8 OnWifiAp(void)
{
    WifiKey(WIFI_KEY_AP);

    // ?댁쟾 AP ?????ш? ???寃쎌????
    if( GetWifiStatusValue(WIFI_STATUS_ERROR) != 0 )
    {
#if 0
        StartDispTimerId( DISP_TIMER_WIFI_AP_ERROR );
#endif
    }
    return SOUND_NONE;
}

static U8 OnWifiBle(void)
{
    WifiKey(WIFI_KEY_BLE);

    return SOUND_NONE;
}

static U8 OnWifiOnOff(void)
{
    WifiKey(WIFI_KEY_ONOFF);

    return SOUND_NONE;
}
#endif

static U8 OnManualSter(void)       /*Manual Sterilize*/
{
#if 0
    if( IsErrorTypeSter() == TRUE 
            || GetWaterOut() == TRUE 
            || IsOpenIceDoor() == TRUE 
            || IsStartDrainWater() == TRUE )
    {
        return SOUND_ERROR;
    }

    // 살균 시작
    if( IsStartSter() == FALSE )
    {
        // 정수 수위가 물넘침이 아닌 경우, 
        if( GetRoomWaterLevel() != LEVEL_LOW 
                && GetRoomWaterLevel() != LEVEL_MID 
                && GetRoomWaterLevel() != LEVEL_HIGH )
        {
            return SOUND_ERROR;
        }

        StartSterMode(STER_MODE_ALL);
        return SOUND_SETUP;
    }

    // 살균 중지
    if( StopSoftSter() == TRUE )
    {
        return SOUND_CANCEL;
    }

#endif
    return SOUND_ERROR;  
}

static U8 OnManualDrainWater(void)       /*Manual Drain*/
{
    if( IsValidHealth() == FALSE )
    {
        return SOUND_ERROR;
    }

    if( StartDrainWater(FULL_DRAIN) == FALSE )
    {
        return SOUND_ERROR;
    }

    //SetSkipResetDrain(TRUE);

    return SOUND_EFFLUENT;
}

static U8 OnTempMonitor(void)   /* Temp Monitor */
{
    if( GetTempMonitoring() == TRUE )
    {
        SetTempMonitoring(FALSE);
        return SOUND_CANCEL;
    }

    SetTempMonitoring(TRUE);
    return SOUND_SETUP;
}

/* 살균 시간 보정 모드 
 *
 */
static U8 OnAdjustHour(void)       /*Manual Drain*/
{
#if 0
    // 살균 기능이 OFF 상태여야 함.
    if( GetHealthMode() != HEALTH_MODE_NONE )
    {
        return SOUND_ERROR;
    }

    // 설정 종료
    if( GetDispAdjustHourConfig() == TRUE )
    {
        SetDispAdjustHourConfig(FALSE); 
        return SOUND_ERROR;
    }

    // 설정 시작
    SetDispAdjustHourConfig(TRUE); 
    return SOUND_SETUP;
#endif

    return 0;
}

/* 살균 시간 설정 모드 
 *
 */
static U8 OnSetHour(void)       /*Manual Drain*/
{
#if 0
    // 살균 기능이 OFF 상태여야 함.
    if( GetHealthMode() != HEALTH_MODE_NONE )
    {
        return SOUND_ERROR;
    }

#if 0
    // WIFI 연결상태
    if( GetWifiStatusValue(WIFI_STATUS_DISP) != WIFI_DISP_OFF )
    {
        return SOUND_ERROR;
    }
#endif

#if 0
    // WIFI 최초 페어링 상태
    if( GetWifiStatusValue(WIFI_STATUS_AP_FIRST) == TRUE )
    {
        return SOUND_ERROR;
    }
#endif

    // 설정 종료
    if( GetDispSetHourConfig() == TRUE )
    {
        SetDispSetHourConfig( FALSE ); 
        return SOUND_CANCEL;
    }

    // 설정 시작
    SetDispSetHourConfig( TRUE ); 
#endif
    return SOUND_SETUP;
}

// EOL 검사 시작
static U8 OnEolStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // 현재 EOL 테스트 중이면, Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    if( GetServiceCheckTankOpen() == FALSE )
    {
        return SOUND_ERROR;
    }

    // EOL 시작
    StartEol( EOL_TYPE_LOAD );
    StartDisplayInit();
    SetVersionDisp(0);

    return SOUND_SETUP;
}

// 탱크 수위 검사
static U8 CheckTankLevel(void)
{
    // 정수 탱크 수위 검사
    if( GetRoomWaterLevel() != LEVEL_LOW )
    {
        return FALSE;
    }

    // 냉수 탱크 수위 검사
    if( GetColdWaterLevel() != LEVEL_LOW )
    {
        return FALSE;
    }

    // 배수 탱크 수위 검사
    if( GetDrainLevel() != LEVEL_LOW )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 CheckFrontTankLevel(void)
{
    return CheckTankLevel();
}

// 탱크 수위 검사
static U8 CheckKeyTankLevel(void)
{
    return CheckTankLevel();
}

// FRONT 검사 시작
static U8 OnMainFrontStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // 현재 EOL 테스트 중이면, Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    MoveIceTrayIcing();

    // EOL 시작
    StartEol( EOL_TYPE_MAIN_FRONT );
    StartDisplayInit();
    SetVersionDisp(0);

    return SOUND_SETUP;
}

// FRONT 검사 시작
static U8 OnFrontStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // 현재 EOL 테스트 중이면, Pass
    if( GetEolStatus() == TRUE )
    {
        return SOUND_NONE;
    }

    // 수위 센서 불량
    if( CheckFrontTankLevel() == FALSE )
    {
        return SOUND_ERROR;
    }

    // EOL 시작
    StartEol( EOL_TYPE_FRONT );
    StartDisplayInit();
    SetVersionDisp(0);

    return SOUND_SETUP;
}

/// 출하모드 시작
static U8 OnShipmentStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // 강제 배수
    if( IsValidHealth() == FALSE )
    {
        return SOUND_ERROR;
    }

    if( StartDrainWater(FULL_DRAIN) == FALSE )
    {
        return SOUND_ERROR;
    }

    MoveIceTrayIcing();

    TurnOnRelay( RELAY_FEEDER_CW );
    TurnOffRelay( RELAY_FEEDER_CCW );

    //SetSkipResetDrain(TRUE);

    return SOUND_SETUP;
}

// TIME SHORT 테스트 모드
static U8 OnTimeShort(void)
{
    // 부저음 없이 Pass
    if( IsExpiredTimeShortLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // TIME SHORT 시작
    StartTimeShort();
    return SOUND_SELECT;
}

// FCT 검사 시작
static U8 OnFctStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredFctLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // FCT 시작
    StartFct();

    return SOUND_SELECT;
}

static U8 OnTestShortCompTime(void)
{
    // 압축기 구속 시간 5으로 초기화
#if( CONFIG_TEST == 1 )
    SetCompProtectOffTime(5);
#endif
    return SOUND_SELECT;
}

static U8 OnTestStopDummyDeicing(void)
{
    // 더미 탈빙 중지
    StopDummyDeIcing();

    return SOUND_SELECT;
}

static U8 OnTestStartSterConty(void)
{
    static U8 flag = FALSE;


    if( flag == FALSE )
    {
        flag = TRUE;
        StartTimer(TIMER_PROCESS, TIMER_ID_TEST_STER_CONTY, SEC(0));

        return SOUND_CONFIG_SETUP;
    }
    else
    {
        flag = FALSE;
        StopTimer(TIMER_PROCESS, TIMER_ID_TEST_STER_CONTY);

        return SOUND_CONFIG_CANCEL;
    }

    return SOUND_NONE;
}

static U8 OnAgingTest(void)
{
    StartAging();
    
    return SOUND_NONE;
}

/* UV TEST CODE */
#if( CONFIG_TEST_UV_MANUAL == 1 )
extern U8 dbg_uv_ice_door;
extern U8 dbg_uv_water_out;
extern U8 dbg_uv_ice_tray;
extern U8 dbg_uv_ice_tank;
extern U8 dbg_uv_drain_tank;
static U8 OnOffUvIceTank(void)
{
    dbg_uv_ice_tank = !dbg_uv_ice_tank;
    return SOUND_SETUP;
}

static U8 OnOffUvDrainTank(void)
{
    dbg_uv_drain_tank = !dbg_uv_drain_tank;
    return SOUND_SETUP;
}

static U8 OnOffUvIceTray(void)
{
    dbg_uv_ice_tray = !dbg_uv_ice_tray;
    return SOUND_SETUP;
}

static U8 OnOffUvIceOut(void)
{
    dbg_uv_ice_door = !dbg_uv_ice_door;
    return SOUND_SETUP;
}

static U8 OnOffUvWaterOut(void)
{
    dbg_uv_water_out = !dbg_uv_water_out;

    return SOUND_SETUP;
}

#endif
