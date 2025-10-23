/**
 * File : process_display.c
 * 
 * Process Led Display
*/
#include "display.h"
#include "process_display.h"

#include "front.h"
#include "room_water.h"
#include "cold_water.h"
#include "hot_water.h"
#include "water_out.h"
#include "tank_clean.h"
#include "ice.h"
#include "ice_full.h"
#include "ice_low.h"
#include "power_saving.h"
#include "health.h"
#include "health_maker.h"
#include "ster.h"
#include "drain_water.h"

#include "api_key.h"
#include "temp.h"

#include "error.h"
#include "sound.h"
#include "eeprom.h"
#include "valve.h"
#include "key.h"
#include "service.h"
#include "ice_door.h"
#include "uv.h"
#include "amount.h"
#include "ice_water_out.h"
#include "purifier.h"
#include "smart_saving.h"

#include "display_lib.h"
#include "display_eol.h"
#include "display_error.h"
#include "aging.h"
#include "segment.h"
#include "led.h"

typedef struct _display_timer_list_
{
    U8  Id;
    U16 Time;
} SDispTimerList_T;

SDispTimerList_T DispTimerList[] = 
{
    { DISP_TIMER_ID_KEY_ICE,            TIMER_7SEC  },
    { DISP_TIMER_ID_KEY_WATER,          TIMER_7SEC  },
    { DISP_TIMER_ID_HOT_STATUS,         TIMER_7SEC  },
    { DISP_TIMER_ID_COLD_STATUS,        TIMER_7SEC  },
    { DISP_TIMER_ID_CONFIG_AMOUNT,      TIMER_7SEC  },
    { DISP_TIMER_ID_COLDMAKE,           TIMER_2SEC  },  
    { DISP_TIMER_ID_HOTMAKE,            TIMER_2SEC  },
    { DISP_TIMER_ID_ICEMAKE,            TIMER_2SEC  },
    { DISP_TIMER_ID_HOTLOCK_SET,        TIMER_2SEC  },
    { DISP_TIMER_ID_ICELOCK_SET,        TIMER_2SEC  },
    { DISP_TIMER_ID_ICEWATERLOCK_SET,   TIMER_2SEC  },
    { DISP_TIMER_ID_ALLLOCK_SET,        TIMER_2SEC  },
    { DISP_TIMER_ID_HOTLOCK,            TIMER_2SEC  },
    { DISP_TIMER_ID_ICELOCK,            TIMER_2SEC  },
    { DISP_TIMER_ID_ICEWATERLOCK,       TIMER_2SEC  },
    { DISP_TIMER_ID_ICEWATERAMOUNTLOCK, TIMER_2SEC  },
    { DISP_TIMER_ID_ICEWATERAMOUNTNOTI, TIMER_2SEC  },
    { DISP_TIMER_ID_NOT_ICEWATER,       TIMER_2SEC  }, 
    { DISP_TIMER_ID_NOT_COLDWATER,      TIMER_2SEC  }, 
    { DISP_TIMER_ID_ALLLOCK,            TIMER_2SEC  },
    { DISP_TIMER_ID_STERLOCK,           TIMER_2SEC  },
    { DISP_TIMER_ID_ERROR,              TIMER_3SEC  },
    { DISP_TIMER_ID_HOT_ALTITUDE,       TIMER_10SEC },
    { DISP_TIMER_ID_UNUSED_SAVE,        TIMER_10SEC },
    { DISP_TIMER_ID_DRAIN_WATER,        TIMER_5SEC  },
    { DISP_TIMER_ID_SETTING_MENU,       TIMER_20SEC },  
    { DISP_TIMER_SEGOFF_BLINK,          TIMER_2SEC  },
    { DISP_TIMER_ID_WATER_SELCET,       TIMER_500MS },
    { DISP_TIMER_ID_ICE_SELCET,         TIMER_500MS },
};
    
#define SZ_DISP_TIMER_LIST ( sizeof(DispTimerList)/sizeof(SDispTimerList_T) )

static U16 FindTimeVal(U8 id)
{
    U8 i = 0U;

    for( i=0; i<SZ_DISP_TIMER_LIST; i++ )
    {
        if( id == DispTimerList[i].Id )
        {
            return DispTimerList[i].Time;
        }
    }

    return 0;   // timer 0...
}

// Display Timer start/stop
void StartDispTimerId(U8 id)
{
    U16 time = 0U;

    time = FindTimeVal(id);
    SetDispTimer(id, time);
}

void StopDispTimerId(U8 id)
{
    SetDispTimer(id, 0);
}

// seg
void StartSegDispTimerId(U8 id)
{
    U16 time = 0U;

    time = FindTimeVal(id);
    SetSegDispTimer(id, time);
}

void StopSegDispTimerId(U8 id)
{
    SetSegDispTimer(id, 0);
}

typedef struct _display_
{
    U8 Mode;       // NORMAL / VERSION / UART TEST / ...

    U8 Init;            /* FALSE : 초기화 전, TRUE : 초기화 완료 */
    U8 Version;         /* 버전 표시 FLAG */
    U8 VersionStep;     /* 버전 표시 Step */

    //U16 BlinkTime_5s;   /* LED 점멸 타이머 0.5sec */
    //U16 BlinkTime_1s;   /* LED 점멸 타이머 1sec */
    //U16 BlinkTime_4s;   /* LED 점멸 타이머 3sec on, 1sec off */

    /* 고도 설정 */
    U8 AltiConfig;      /* 고도 설정 동작 FLAG */
    //U16 AltiTimeOut;    /* 고도 설정 자동 해제 시간 타이머 ( @100ms ) */

    /* 살균 시간 변경 */
    U8 AdjustHourConfig;
    //U16 AdjustHourTimeOut;

    /* 살균 시간 설정 */
    U8 SetHourConfig;

    /* 자동 배수 설정/해제 */
    U8 DrainWaterConfig;
    //U16 DrainWaterTimeOut;

    /* 살균 중 잠금 */ 
    U8 SterLock;         /* 살균시 버튼 잠금 */

    /* Setting Menu */
    U8 Setting;          /* 설정 화면 활성화 상태 */

    /* Memento */
    U8 Memento;         /* 메멘토 동작 FLAG */
    U8 MementoStep;     /* 메멘토 표시 동작 STEP */
    U8 MementoTimer;    /* 메멘토 표시 동작 시간 타이머 */

    /* 온수 잠금 셋팅 */
    U8 HotLockSet;

    /* 온수 잠금 */
    U8 HotLock;         /* 온수 잠금 시, 온수 선택 */

    /* 얼음 잠금 셋팅 */
    U8 IceLockSet;

    /* 얼음 잠금 */
    U8 IceLock;         /* 얼음 잠금 시, 얼음 추출 */

    /* 얼음물 잠금 셋팅 */
    U8 IceWaterLockSet;

    /* 얼음물 잠금 */
    U8 IceWaterLock;    /* 얼음물 잠금 시, 얼음 추출 */

    /* 얼음물 용량 잠금 */
    U8 IceWaterAmountLock; /* 얼음물 용량 잠금 시, 얼음물 용량 선택 */

    /* 얼음에서 얼음물 용량 선택 시 알림 */
    U8 IceWaterAmountNoti;  /* 얼음 선택 때 얼음물 용량 선택  시, 알림 */

    /* 얼음물 추출 제한    알림 */
    U8 NotIceWater;         /* 얼음물 추출 제한    알림 */

    /* 냉수 추출 제한    알림 */
    U8 NotColdWater;        /* 냉수 추출 제한    알림 */

    /* 전체 잠금 셋팅 */
    U8 AllLockSet;

    /* 전체 잠금 */
    U8 AllLock;         /* 전체 잠금 시, 전체 버튼   Lock */

    /* 얼음 꺼짐 */
    U8 IceMake;         /* 얼음 꺼짐 시, 얼음 추출 레버  를 길게 선택 시 점멸 */

    /* 냉수 꺼짐 */
    U8 ColdMake;         /* 냉수 꺼짐 시, 냉수 버튼 선택 시 점멸 */

    /* 온수 꺼짐 */
    U8 HotMake;         /* 온수 꺼짐 시, 온수 버튼 선택 시 점멸 */

    /* FOTA */
    U8 Fota;

    U8 IceOut;

    U8 IsDispContyTime;           /* 정량 추출 프로그래스 표시 OFF 시간 */
    U8 IsDispIceWaterContyTime;   /* 얼음물 정량 추출 프로그래스 표시 OFF 시간 */

    /* DIAGNOSIS */
    U8 Diagnosis;
    U8 DiagnosisCnt;

    /* Disp Temp */
    TEMP_T TempRoom;
    TEMP_T TempCold;
    TEMP_T TempHot;
    U8 TempMonitoring;
    TEMP_T TempAmbi1;
    TEMP_T TempAmbi2;

    U8 IcePanel;
    U8 WaterPanel;
} SDisplay_T;

SDisplay_T Disp;

static void DisplaySelWater(void);
static void DisplayWaterConty(void);
static void DisplayWelcomLightWater(void);
static void DisplayWelcomLightIce(void);
static void DisplayStandBy(void);

static void DisplayWaterTempLevel(void);
static void DisplayIceMake(void);
static void DisplayIceMakeBlink(void);
static void DisplayColdMakeBlink(void);
static void DisplayHotMakeBlink(void);
static void DisplayAllLockBlink(void);
static void DisplayIceTurbo(void);
static void DisplayWaterPreparing(void);
static void DisplayIceFull(void);
static void DisplayDecoIceMaker(void);
static void DisplayLockHot(void);
static void DisplayLockIce(void);
static void DisplayLockIceWater(void);
static void DisplayLockAll(void);
static void DisplayLockIcon(void);
static void DisplayLockIceSet(void);
static void DisplayLockIceWaterSet(void);
static void DisplayLockHotSet(void);
static void DisplayLockAllSet(void);
static void DisplayLock(void);
static void DisplayPowerSaving(void);
static void DisplayServiceMode(void);
static void DisplayHealth(void);
static void DisplayUv(void);

static void ProcessDisplayBoot(void);
static void ProcessDisplaySettingMode(void);
static void ProcessDisplayConfHotAltitude(void);
static void ProcessDisplayConfUnusedSave(void);
static void ProcessDisplayNormalMode(void);
static void ProcessDisplayErrorMode(void);
static void ProcessDisplayVersionMode(void);
static void ProcessDisplayMementoMode(void);
static void ProcessDisplayLockAll(void);
static void ProcessDisplayConfEnergySave(void);
static void ProcessDisplaySmartSaveConfig(void);

//static void ProcessDisplayConfAdjustHour(void);
//static void ProcessDisplayConfSetHour(void);
static void ProcessDisplayConfDrainWater(void);
static void ProcessDisplaySter(void);

void InitDisplay(void)
{
    InitDispTimer();       // LED
    InitSegDispTimer();    // SEG
    SetDispTimer(DISP_TIMER_ID_CONFIG_TIME, TIMER_30SEC);

    /* Booting Display - Showing Blink Front Display */
    Disp.Init = FALSE;

    /* Hot Water Config Altitude */
    Disp.AltiConfig  = FALSE;
    
    /* 살균 시간 조정 */
    Disp.AdjustHourConfig  = FALSE;

    /* 살균 시간 설정 */
    Disp.SetHourConfig  = FALSE;

    /* 자동 배수 설정/해제 */
    Disp.DrainWaterConfig = FALSE;

    /* Version */
    Disp.Version = FALSE;

    /* Hot Lock */
    Disp.HotLock     = FALSE;

    /* Ice Lock */
    Disp.IceLock     = FALSE;

    /* Ice Water Lock */
    Disp.IceWaterLock = FALSE;

    /* Ice Water Amount Lock */
    Disp.IceWaterAmountLock = FALSE;

    /* Ice / Ice Water Amount Noti */
    Disp.IceWaterAmountNoti = FALSE;

    /* Ice / Ice Water Amount Noti */
    Disp.NotIceWater = FALSE;

    /* Ice / Ice Water Amount Noti */
    Disp.NotColdWater = FALSE;

    /* All Lock */
    Disp.AllLock     = FALSE;

    /* Ice Make */
    Disp.IceMake     = FALSE;

    /* Cold Off */
    Disp.ColdMake      = FALSE;

    /* Cold Off */
    Disp.HotMake      = FALSE;
    
    /* Fota */
    Disp.Fota       = FALSE;

    Disp.IceOut = FALSE;

    Disp.IsDispContyTime = 0U;
    Disp.IsDispIceWaterContyTime = 0U;

    /* Diagnosis */
    Disp.Diagnosis  = FALSE;
    Disp.DiagnosisCnt = 0;

    Disp.TempRoom = 0.0f;
    Disp.TempCold = 0.0f;
    Disp.TempHot = 0.0f;

    Disp.TempMonitoring = FALSE;
    Disp.TempAmbi1 = 0.0f;
    Disp.TempAmbi2 = 0.0f;    

    Disp.IcePanel = TRUE;
    Disp.WaterPanel = TRUE;
    
    /* TURN OFF ALL LED */
    TurnOffAllLED();
}

#define TEMP_DISP_TIME  (300U)    /// 30sec @100ms
static void UpdateDispTemp(void)
{
    static U16 time = TEMP_DISP_TIME;   /// 30sec @100ms

    if( time != 0 )
    {
        time--;
        return;
    }

    time = TEMP_DISP_TIME;
    
    Disp.TempRoom = GetTemp(TEMP_ID_ROOM_WATER);
    Disp.TempCold = GetTemp(TEMP_ID_COLD_WATER);
    Disp.TempHot = GetTemp(TEMP_ID_HOT_WATER);
    Disp.TempAmbi1 = GetTemp(TEMP_ID_AMBIENT);
    Disp.TempAmbi2 = GetTemp(TEMP_ID_AMBIENT_2);
}

void SetTempMonitoring(U8 monitoring)
{
    Disp.TempMonitoring = monitoring;
}

U8 GetTempMonitoring(void)
{
    return Disp.TempMonitoring;
}

void SelectIcePanel(void)
{
    Disp.IcePanel = TRUE;
    Disp.WaterPanel = FALSE;
}

void SelectWaterPanel(void)
{
    Disp.IcePanel = FALSE;
    Disp.WaterPanel = TRUE;
}

static void ClearPanel(void)
{
    Disp.IcePanel = TRUE;
    Disp.WaterPanel = TRUE;
}

U8 GetDisplayInit(void)
{
    return Disp.Init;
}

void StartDisplayInit(void)
{
    Disp.Init = FALSE;
}

// 온수 잠금 설정 점멸 시작/정지
void StartDisplayHotLockSet(void)
{
    Disp.HotLockSet = TRUE;
    StartDispTimerId(DISP_TIMER_ID_HOTLOCK_SET);
}

void StopDisplayHotLockSet(void)
{
    Disp.HotLockSet = FALSE;
    StopDispTimerId(DISP_TIMER_ID_HOTLOCK_SET);
}

// 온수 잠금 점멸 시작/정지
void StartDisplayHotLock(void)
{
    Disp.HotLock= TRUE;
    StartDispTimerId(DISP_TIMER_ID_HOTLOCK);
}

void StopDisplayHotLock(void)
{
    Disp.HotLock = FALSE;
    StopDispTimerId(DISP_TIMER_ID_HOTLOCK);
}

// 얼음 잠금 설정 점멸 시작/정지
void StartDisplayIceLockSet(void)
{
    Disp.IceLockSet = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ICELOCK_SET);
}

void StopDisplayIceLockSet(void)
{
    Disp.IceLockSet = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ICELOCK_SET);
}

// 얼음 잠금 점멸 시작/정지
void StartDisplayIceLock(void)
{
    Disp.IceLock = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ICELOCK);
}

void StopDisplayIceLock(void)
{
    Disp.IceLock = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ICELOCK);
}

// 얼음물 잠금 설정 점멸 시작/정지
void StartDisplayIceWaterLockSet(void)
{
    Disp.IceWaterLockSet = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ICEWATERLOCK_SET);
}

void StopDisplayIceWaterLockSet(void)
{
    Disp.IceWaterLockSet = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ICEWATERLOCK_SET);
}

// 얼음물 잠금 점멸 시작/정지
void StartDisplayIceWaterLock(void)
{
    Disp.IceWaterLock = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ICEWATERLOCK);
}

void StopDisplayIceWaterLock(void)
{
    Disp.IceWaterLock = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ICEWATERLOCK);
}

// 얼음물 용량 잠금 점멸 시작/정지
void StartDisplayIceWaterAmountLock(void)
{
    Disp.IceWaterAmountLock = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ICEWATERAMOUNTLOCK);
}

void StopDisplayIceWaterAmountLock(void)
{
    Disp.IceWaterAmountLock = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ICEWATERAMOUNTLOCK);
}

/// 얼음 선택 때 얼음물 용량 선택  시, 알림
void StartDisplayIceWaterAmountNoti(void)
{
    Disp.IceWaterAmountNoti = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ICEWATERAMOUNTNOTI);
}

void StopDisplayIceWaterAmountNoti(void)
{
    Disp.IceWaterAmountNoti = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ICEWATERAMOUNTNOTI);
}

/// 얼음물 추출 제한 알림
void StartDisplayNotIceWater(void)
{
    Disp.NotIceWater = TRUE;
    StartDispTimerId(DISP_TIMER_ID_NOT_ICEWATER);
}

void StopDisplayNotIceWater(void)
{
    Disp.NotIceWater = FALSE;
    StopDispTimerId(DISP_TIMER_ID_NOT_ICEWATER);
}

/// 냉수 추출 제한 알림
void StartDisplayNotColdWater(void)
{
    Disp.NotColdWater = TRUE;
    StartDispTimerId(DISP_TIMER_ID_NOT_COLDWATER);
}

void StopDisplayNotColdWater(void)
{
    Disp.NotColdWater = FALSE;
    StopDispTimerId(DISP_TIMER_ID_NOT_COLDWATER);
}

// 전체 잠금 설정 점멸 시작/정지
void StartDisplayLockAllSet(void)
{
    Disp.AllLockSet = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ALLLOCK_SET);
}

void StopDisplayLockAllSet(void)
{
    Disp.AllLockSet = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ALLLOCK_SET);
}

// 전체 잠금 점멸 시작/정지
void StartDisplayLockAll(void)
{
    Disp.AllLock = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ALLLOCK);
}

void StopDisplayLockAll(void)
{
    Disp.AllLock = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ALLLOCK);
}

// 살균 중 버튼 Lock 점멸 시작 / 정지
void StartDisplaySter(void)
{
    Disp.SterLock= TRUE;
    StartDispTimerId(DISP_TIMER_ID_STERLOCK);
}

void StopDisplaySter(void)
{
    Disp.SterLock = FALSE;
    StopDispTimerId(DISP_TIMER_ID_STERLOCK);
}

// 얼음 꺼짐 점멸 시작/정지
void StartDisplayIceMake(void)
{
    Disp.IceMake = TRUE;
    StartDispTimerId(DISP_TIMER_ID_ICEMAKE);
}

void StopDisplayIceMake(void)
{
    Disp.IceMake = FALSE;
    StopDispTimerId(DISP_TIMER_ID_ICEMAKE);
}

// 냉수 꺼짐 점멸 시작/정지
void StartDisplayColdMake(void)
{
    Disp.ColdMake = TRUE;
    StartDispTimerId(DISP_TIMER_ID_COLDMAKE);
}

void StopDisplayColdMake(void)
{
    Disp.ColdMake = FALSE;
    StopDispTimerId(DISP_TIMER_ID_COLDMAKE);
}

// 온수 꺼짐 점멸 시작/정지
void StartDisplayHotMake(void)
{
    Disp.HotMake = TRUE;
    StartDispTimerId(DISP_TIMER_ID_HOTMAKE);
}

void StopDisplayHotMake(void)
{
    Disp.HotMake = FALSE;
    StopDispTimerId(DISP_TIMER_ID_HOTMAKE);
}

/// 얼음/얼음물/얼음물용량 선택 시작
void StartDisplayKeyIce(void)
{
    StartDispTimerId(DISP_TIMER_ID_KEY_ICE);
}

// 얼음/얼음물 선택 정지
void StopDisplayKeyIce(void)
{
    StopDispTimerId(DISP_TIMER_ID_KEY_ICE);
}

/// 온수/정수/냉수/물용량 선택 시작
void StartDisplayKeyWater(void)
{
    StartDispTimerId(DISP_TIMER_ID_KEY_WATER);
}

// 온수/정수/냉수/물용량 선택 정지
void StopDisplayKeyWater(void)
{
    StopDispTimerId(DISP_TIMER_ID_KEY_WATER);
}

/// "COLOR" LED 대기 시작
void StartDisplayWaterColorWait(void)
{
    StartDispTimerId(DISP_TIMER_ID_WATER_SELCET);
}

// "COLOR" LED 대기 정지
void StopDisplayWaterColorWait(void)
{
    StopDispTimerId(DISP_TIMER_ID_WATER_SELCET);
}

/// "COLOR" LED 대기 시작
void StartDisplayIceColorWait(void)
{
    StartDispTimerId(DISP_TIMER_ID_ICE_SELCET);
}

// "COLOR" LED 대기 정지
void StopDisplayIceColorWait(void)
{
    StopDispTimerId(DISP_TIMER_ID_ICE_SELCET);
}

// "OFF" 점멸 시작/정지
void StartSegDisplaySegOffBlink(void)
{
    StartSegDispTimerId(DISP_TIMER_SEGOFF_BLINK);
}

// "OFF" 점멸 시작/정지
void StopSegDisplaySegOffBlink(void)
{
    StopSegDispTimerId(DISP_TIMER_SEGOFF_BLINK);
}

// SEG ON 시작/정지_5sec
void StartSegDisplaySegOn(void)
{
    SetSegDispTimer( DISP_TIMER_SEGON, TIMER_5SEC);
}
void StopSegDisplaySegOn(void)
{
    SetSegDispTimer( DISP_TIMER_SEGON, 0);
}

// SEG OFF 시작/정지_5sec
void StartSegDisplaySegOff(void)
{
    SetSegDispTimer( DISP_TIMER_SEGOFF, TIMER_5SEC);
}

void StopSegDisplaySegOff(void)
{
    SetSegDispTimer( DISP_TIMER_SEGOFF, 0);
}

// Fota Set / Clear
void SetDisplayFota(U8 fota)
{
#if 0
    if( Disp.Fota !=  mu8Fota)
    {
        SaveEepromId ( EEP_ID_WIFI_FOTA_RESET );
    }
#endif

    Disp.Fota = fota;
}

U8 GetDisplayFota(void)
{
    return Disp.Fota;
}

/* 온수 고도 설정 */
U8 GetAltidueConfig(void)
{
    return Disp.AltiConfig;
}

void SetAltitudeConfig(U8 conf)
{
    Disp.AltiConfig = conf;
    if ( conf == TRUE )
    {
        StartDispTimerId(DISP_TIMER_ID_HOT_ALTITUDE);
    }
}

/* 드레인 설정 */
U8 GetDispDrainWaterConfig(void)
{
    return Disp.DrainWaterConfig;
}

void SetDispDrainWaterConfig(U8 conf)
{
    Disp.DrainWaterConfig = conf;
    if ( conf == TRUE )
    {
        StartDispTimerId(DISP_TIMER_ID_DRAIN_WATER);
    }
}

U16 IsExpiredConfigTime(void)
{
    if( IsExpiredDispTimer(DISP_TIMER_ID_CONFIG_TIME) == FALSE )
    {
        return FALSE;   // 만료 안됨.
    }

    return TRUE;  // 만료됨.
}

/* 버전 표시 */
void SetVersionDisp(U8 sec)
{
    Disp.Version = TRUE;
    Disp.VersionStep = 0;
    SetDispTimer(DISP_TIMER_ID_VERSION, sec);
}

/* 설정 메뉴 선택 상태 */
void SetSettingDisp(U8 conf)
{
    Disp.Setting = conf;
}

U8 GetSettingDisp(void)
{
    return Disp.Setting;
}

/* 메멘토 설정 */
U8 GetMementoDisp(void)
{
    return Disp.Memento;
}

void SetMementoDisp(U8 conf)
{
    Disp.Memento = conf;
    Disp.MementoStep = 0;
    Disp.MementoTimer = 0;
}

static void DisplaySelWater(void)
{
    U8  sel;

    sel = GetWaterOutSelect();

    DispWaterSel(sel);

    if( IsExpiredWaterOutReturnTime() == TRUE )
    {
        DispWelcomWaterWhite(OFF);
        DispWelcomWaterRed(OFF);
        DispWelcomWaterGreen(OFF);
        DispWelcomWaterBlue(OFF);
        return;
    }

    if( IsExpiredDispTimer(DISP_TIMER_ID_WATER_SELCET) == FALSE )
    {
        DispWelcomWaterWhite(OFF);
        DispWelcomWaterRed(OFF);
        DispWelcomWaterGreen(OFF);
        DispWelcomWaterBlue(OFF);
    }
}

static void DisplaySelIce(void)
{
    U8  sel;

    // 제빙 OFF이면, 얼음 관련 표시 안하
    if( GetIceConfigMake() == FALSE )
    {
        return;
    }

    sel = GetIceWaterSelect();

    DispIceSel(sel);

    if( IsExpiredIceWaterOutReturnTime() == TRUE )
    {
        DispWelcomIceWhite(OFF);
        DispWelcomIceRed(OFF);
        DispWelcomIceGreen(OFF);
        DispWelcomIceBlue(OFF);
        return;
    }

    if( IsExpiredDispTimer(DISP_TIMER_ID_ICE_SELCET) == FALSE )
    {
        DispWelcomIceWhite(OFF);
        DispWelcomIceRed(OFF);
        DispWelcomIceGreen(OFF);
        DispWelcomIceBlue(OFF);
    }
}

static void DisplayIceWaterConty(void)
{
    U8 onOff = OFF;
    U8 amount = 0;

    DispIceAmountLedOffDuty();

    /* Conty LED */
    if( GetIceWaterOutContinue() == TRUE )
    {
        onOff = ON;
    }

    /* Conty Amount LED */
    if( onOff == OFF )
    {
        DispIceWaterAmount(OFF);
        DispStatusIceWater(OFF);
        DispStatusMax(OFF);
        DispConty500mL(OFF);
        DispConty1L(OFF);
        DispConty1D5L(OFF);
        DispContyInfinity(OFF);
        DispUnitMl(OFF);
    }
    else
    {
        amount = GetIceWaterOutAmoutType();
        if( amount == FIXED_ICE_WATER_AMOUNT_500ML )
        {
            DispSegDigits(GetAmount(AMOUNT_ID_500ML));
            DispIceWaterAmount(ON);
            DispStatusIceWater(ON);
            DispStatusMax(ON);
            OnOffLed(LED_ID_STATUS_BAR_LOW, ON);
            OnOffLed(LED_ID_STATUS_BAR_MIDDLE, OFF);
            OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
            OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
            DispUnitMl(ON);
        }
        else if( amount == FIXED_ICE_WATER_AMOUNT_700ML )
        {
            DispSegDigits(GetAmount(AMOUNT_ID_700ML));
            DispIceWaterAmount(ON);
            DispStatusIceWater(ON);
            DispStatusMax(ON);
            OnOffLed(LED_ID_STATUS_BAR_LOW, OFF);
            OnOffLed(LED_ID_STATUS_BAR_MIDDLE, ON);
            OnOffLed(LED_ID_STATUS_BAR_HIGH, OFF);
            OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
            DispUnitMl(ON);
        }
        else if( amount == FIXED_ICE_WATER_AMOUNT_1L )
        {
            DispSegDigits(GetAmount(AMOUNT_ID_1000ML));
            DispIceWaterAmount(ON);
            DispStatusIceWater(ON);
            DispStatusMax(ON);
            OnOffLed(LED_ID_STATUS_BAR_LOW, OFF);
            OnOffLed(LED_ID_STATUS_BAR_MIDDLE, OFF);
            OnOffLed(LED_ID_STATUS_BAR_HIGH, ON);
            OnOffLed(LED_ID_STATUS_BAR_INFINITE, OFF);
            DispUnitMl(ON);
        }
    }
}

static void DisplayWaterConty(void)
{
    U8 onOff = OFF;
    static U8 blinkOnOff = ON;
    U8 amount = 0;

    // 탱크 비우기 모드
    if( GetTankClean() == TRUE )
    {
        blinkOnOff = BlinkLED( blinkOnOff, DispTankCleanBlink, DISP_TIMER_ID_500MS);

        return ;
    }
    blinkOnOff = ON;

    DispAmountLedOffDuty();

    /* Conty LED */
    if( GetWaterOutContinue() == TRUE )
    {
        onOff = ON;
    }

    /* Conty Amount LED */
    if( onOff == OFF )
    {
        DispAmount(OFF);
        DispStatusWater(OFF);
        DispConty500mL(OFF);
        DispConty1L(OFF);
        DispConty1D5L(OFF);
        DispContyInfinity(OFF);
        DispUnitMl(OFF);
    }
    else
    {
        amount = GetWaterOutAmoutType();
        if( amount == FIXED_AMOUNT_500ML )
        {
            DispSegDigits(GetAmount(AMOUNT_ID_500ML));
            DispAmount(ON);
            DispStatusWater(ON);
            DispConty500mL(ON);
            DispConty1L(OFF);
            DispConty1D5L(OFF);
            DispContyInfinity(OFF);
            DispUnitMl(ON);
        }
        else if( amount == FIXED_AMOUNT_1L )
        {
            DispSegDigits(GetAmount(AMOUNT_ID_1000ML));
            DispAmount(ON);
            DispStatusWater(ON);
            DispConty500mL(OFF);
            DispConty1L(ON);
            DispConty1D5L(OFF);
            DispContyInfinity(OFF);
            DispUnitMl(ON);
        }
        else if( amount == FIXED_AMOUNT_1D5L )
        {
            DispSegDigits(GetAmount(AMOUNT_ID_1500ML));
            DispAmount(ON);
            DispStatusWater(ON);
            DispConty500mL(OFF);
            DispConty1L(OFF);
            DispConty1D5L(ON);
            DispContyInfinity(OFF);
            DispUnitMl(ON);
        }
        else if( amount == FIXED_AMOUNT_INFINITY )
        {
            DispSegInfinity();
            DispAmount(ON);
            DispStatusWater(ON);
            DispConty500mL(OFF);
            DispConty1L(OFF);
            DispConty1D5L(OFF);
            DispContyInfinity(ON);
        }
    }
}

#define DEFAULT_WELCOM_LED_DELAY (10)  /* 1sec */
static void DisplayWelcomLightWater(void)
{
    U8 onOff = OFF;

    // 레버 입력 지연 시간 적용 ( protect chattering )
    if( IsOpenValve(VALVE_ROOM_OUT) == TRUE 
            || IsOpenValve(VALVE_COLD_CIRCULATE) == TRUE 
            || IsOpenValve(VALVE_HOT_OUT) == TRUE  )
    {
        onOff = ON;
    }
    DispWaterOut(onOff);
}

static void DisplayWelcomLightIce(void)
{
    U8 status;
    U8 onOff = OFF;

    status = GetIceDoorStatus();
    if( status == ICE_DOOR_OPENING 
            || status == ICE_DOOR_OPEN )
    {
        onOff = ON;
    }

    DispIceOut(onOff);
}

static void DisplayStandBy(void)
{
    DispUvCareSystem(ON);
}

static void DisplayTempLevelCold(U8 onOff);
static void DisplayTempLevelHot(U8 onOff);
static void DisplayWaterTempLevel(void)
{
    /* HOT WATER TEMP LEVEL */
    if( GetHotWaterConfigMake() == ON ) 
    {
        DisplayTempLevelHot(ON);
    }
    else  
    {
        DisplayTempLevelHot(OFF);
    }

    /* COLD WATER TEMP LEVEL */
    if( GetColdWaterConfigMake() == ON ) 
    {
        DisplayTempLevelCold(ON);
    }
    else 
    {
        DisplayTempLevelCold(OFF);
    }
}

static void DisplayIceMake(void)
{
#if 0
    U8 onOff = OFF;

    if( GetIceConfigMake() == TRUE) 
    {
        onOff = ON;
    }

    DispIceMake(onOff);
#endif
}

#if 0
static void DisplayIceMakeBlink(void)
{
    static U8 blinkOnOff = ON;

    /* 얼음 꺼짐 상태에서 선택 시, 점멸 */
    if( Disp.IceMake == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_ICEMAKE) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispIce, DISP_TIMER_ID_500MS);
            return ;
        }

        blinkOnOff = ON;
        Disp.IceMake = FALSE;
    }
}

static void DisplayColdMakeBlink(void)
{
    static U8 blinkOnOff = ON;

    /* 냉수 꺼짐 상태에서 선택 시, 점멸 */
    if( Disp.ColdMake == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_COLDMAKE) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispColdWater, DISP_TIMER_ID_500MS);
            
            return ;
        }

        blinkOnOff = ON;
        Disp.ColdMake = FALSE;
    }
}

static void DisplayHotMakeBlink(void)
{
    static U8 blinkOnOff = ON;

    /* 냉수 꺼짐 상태에서 선택 시, 점멸 */
    if( Disp.HotMake == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_HOTMAKE) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispHotWater, DISP_TIMER_ID_500MS);
            return ;
        }

        blinkOnOff = ON;
        Disp.HotMake = FALSE;
    }
}
#endif

static void DisplayIceMakeBlink(void)
{
    static U8 mu8Counter = 10;

    /* 얼음 꺼짐 상태에서 선택 시, 점멸 */
    if( IsExpiredDispTimer(DISP_TIMER_ID_ICEMAKE) == FALSE )
    {
        DispUnitCelsius(OFF);
    
        if( mu8Counter <= 5 )
        {
            DispSegStr(" OFF");
            OnOffLed(LED_ID_ICE, ON);
        }
        else
        {
            AllOffGroupASeg();
            OnOffLed(LED_ID_ICE, OFF);
        }  
        
        mu8Counter--;
        if( mu8Counter == 0 )
        {
            mu8Counter = 10;
        }
    }
}


static void DisplayColdMakeBlink(void)
{
    static U8 mu8Counter = 10;

    /* 냉수 꺼짐 상태에서 선택 시, 점멸 */
    if( IsExpiredDispTimer(DISP_TIMER_ID_COLDMAKE) == FALSE )
    {
        DispUnitCelsius(OFF);
        
        if( mu8Counter <= 5 )
        {
            DispSegStr(" OFF");
            OnOffLed(LED_ID_COLD, ON);
        }
        else
        {
            AllOffGroupASeg();
            OnOffLed(LED_ID_COLD, OFF);
        }  
        
        mu8Counter--;
        if( mu8Counter == 0 )
        {
            mu8Counter = 10;
        }
    }
}

static void DisplayHotMakeBlink(void)
{
    static U8 mu8Counter = 10;

    /* 온수 꺼짐 상태에서 선택 시, 점멸 */
    if( IsExpiredDispTimer(DISP_TIMER_ID_HOTMAKE) == FALSE )
    {
        DispUnitCelsius(OFF);
        
        if( mu8Counter <= 5 )
        {
            DispSegStr(" OFF");
            OnOffLed(LED_ID_HOT, ON);
        }
        else
        {
            AllOffGroupASeg();
            OnOffLed(LED_ID_HOT, OFF);
        }  
        
        mu8Counter--;
        if( mu8Counter == 0 )
        {
            mu8Counter = 10;
        }
    }

}

static void DisplayAllLockBlink(void)
{
    static U8 mu8Counter = 10;

    /* 온수 꺼짐 상태에서 선택 시, 점멸 */
    if( IsExpiredDispTimer(DISP_TIMER_ID_ALLLOCK) == FALSE )
    {
        if( mu8Counter <= 5 )
        {
            DispSegStr(" Loc");
            OnOffLed(LED_ID_ICE_LOCK, ON);
            OnOffLed(LED_ID_HOT_LOCK, ON);
        }
        else
        {
            AllOffGroupASeg();
            OnOffLed(LED_ID_ICE_LOCK, OFF);
            OnOffLed(LED_ID_HOT_LOCK, OFF);
        }  
        
        mu8Counter--;
        if( mu8Counter == 0 )
        {
            mu8Counter = 10;
        }
    }
    else
    {
        mu8Counter = 10;
    }
}

static void DisplayIceTurbo(void)
{
    U8 onOff = OFF;

    if( GetIceTurbo() == TRUE )
    {
        onOff = TRUE;
    }

    /* 제빙 OFF이면, TURBO도 OFF */
    if( GetIceConfigMake() == FALSE) 
    {
        onOff = OFF;
    }

    DispIceTurbo(onOff);
}

static void DisplayWaterPreparing(void)
{
    U8 onOff = OFF;

    if( IsDetectTankLevel(LEVEL_ID_ROOM, LEVEL_DETECT_LOW) == FALSE )
    {
        onOff = ON;
    }

    DispWaterPreparing(onOff);
}

static void DisplayIceFull(void)
{
    U8 onOff = OFF;

    if( GetIceFullStatus() == TRUE )
    {
        onOff = ON;
    }

    // 제빙 OFF이면, 만빙 표시도 OFF 
    if( GetIceConfigMake() == FALSE )
    {
        onOff = OFF;
    }
    DispIceFullHigh(onOff);
}

static void DisplayIceFullLow(void)
{
    U8 onOff = OFF;

    if( GetIceLowStatus() == FALSE )
    {
        onOff = ON;
    }

    // 제빙 OFF이면, 저빙 표시도 OFF 
    if( GetIceConfigMake() == FALSE )
    {
        onOff = OFF;
    }
    
    if( GetIceFullPowerOnCheck() == FALSE )
    {
        onOff = OFF;
    }

    if( GetIceFullStatus() == TRUE )
    {
        onOff = OFF;
    }
    
    DispIceFullLow(onOff);
}


static void DisplayDecoIceMaker(void)
{
    U8 onOff = ON;

    if( GetIceConfigMake() == FALSE) 
    {
        onOff = OFF;
    }

    if( GetConfigDeco(DECO_LED_WELCOME_WATER) == FALSE )
    {
        onOff = OFF;
    }

    DispIceMaker(onOff);
}

static void DisplayLockHot(void)
{
    U8 onOff = OFF;

    /* 온수 잠금 LED ON/OFF */
    if( GetLockHot() == LOCK  )
    {
        onOff = ON;
    }
  
    DispHotLock(onOff);
}

static void DisplayLockIce(void)
{
    U8 onOff = OFF;

    if( GetLockIce() == LOCK )
    {
        onOff = ON;
    }

    DispLockIce(onOff);
}

static void DisplayLockIceWater(void)
{
    U8 onOff = OFF;

    if( GetLockIceWater() == LOCK )
    {
        onOff = ON;
    }

    DispLockIceWater(onOff);
}

static void DisplayLockIceWaterAmount(void)
{
    U8 onOff = OFF;

    if( GetLockIceWaterAmount() == LOCK )
    {
        onOff = ON;
    }

    DispLockIceWaterAmount(onOff);
}

static void DisplayLockAll(void)
{
    U8 onOff = OFF;
    
    if( GetLockAll() == LOCK )
    {
        onOff = ON;
    }

    DispLockAll(onOff);
}

static void DisplayLockIcon(void)
{
    U8 onOff = OFF;
    
    if( GetLockHot() == LOCK
        || GetLockIce() == LOCK
        || GetLockAll() == LOCK )
    {
        onOff = ON;
    }

    DispLock(onOff);
}

static void DisplayLockIceSet(void)
{
    U8 onOff = OFF;
    static U8 blinkOnOff = ON;

    /* 얼음 잠금 설정 시, 점멸 */
    if( Disp.IceLockSet == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_ICELOCK_SET) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispLockIceSetBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOff = ON;
            Disp.IceLockSet = FALSE;
        }
    }
}

static void DisplayLockIceWaterSet(void)
{
    U8 onOff = OFF;
    static U8 blinkOnOff = ON;

    /* 얼음물 잠금 설정 시, 점멸 */
    if( Disp.IceWaterLockSet == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_ICEWATERLOCK_SET) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispLockIceWaterSetBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOff = ON;
            Disp.IceWaterLockSet = FALSE;
        }
    }
}

static void DisplayLockHotSet(void)
{
    U8 onOff = OFF;
    static U8 blinkOnOff = ON;

    /* 온수 잠금 설정 시, 점멸 */
    if( Disp.HotLockSet == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_HOTLOCK_SET) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispLockHotSetBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOff = ON;
            Disp.HotLockSet = FALSE;
        }
    }
}

static void DisplayNotIceWater(void)
{
    U8 onOff = OFF;
    static U8 blinkOnOff = ON;

    /* 얼음물 선택 시, 점멸 */
    if( Disp.NotIceWater == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_NOT_ICEWATER) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispIceWater, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOff = ON;
            Disp.NotIceWater = FALSE;
        }
    }
}

static void DisplayNotColdWater(void)
{
    U8 onOff = OFF;
    static U8 blinkOnOff = ON;

    /* 얼음물 선택 시, 점멸 */
    if( Disp.NotColdWater == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_NOT_COLDWATER) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispColdWater, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOff = ON;
            Disp.NotColdWater = FALSE;
        }
    }
}

static void DisplayLockAllSet(void)
{
    U8 onOff = OFF;
    static U8 blinkOnOff = ON;

    /* 전체 잠금 설정 시, 점멸 */
    if( Disp.AllLockSet == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_ALLLOCK_SET) == FALSE )
        {
            blinkOnOff = BlinkLED(blinkOnOff, DispLockAllSetBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOff = ON;
            Disp.AllLockSet = FALSE;
        }
    }
}

static void DisplayLock(void)
{
    U8 onOff = OFF;
    static U8 blinkOnOffHotLock = ON;
    static U8 blinkOnOffIceLock = ON;
    static U8 blinkOnOffIceWaterLock = ON;
    static U8 blinkOnOffIceWaterAmountLock = ON;
    static U8 blinkOnOffIceWaterNoti = ON;

    /* 온수 잠금 상태에서 선택 시, 점멸 */
    if( Disp.HotLock == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_HOTLOCK) == FALSE )
        {
            blinkOnOffHotLock = BlinkLED(blinkOnOffHotLock, DispSettingHotLockBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOffHotLock = ON;
            Disp.HotLock = FALSE;
        }
    }

    /* 얼음 잠금 상태에서 선택 시, 점멸 */
    if( Disp.IceLock == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_ICELOCK) == FALSE )
        {
            blinkOnOffIceLock = BlinkLED(blinkOnOffIceLock, DispSettingIceLockBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOffIceLock = ON;
            Disp.IceLock = FALSE;
        }
    }

    /* 얼음물 잠금 상태에서 선택 시, 점멸 */
    if( Disp.IceWaterLock == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_ICEWATERLOCK) == FALSE )
        {
            blinkOnOffIceWaterLock = BlinkLED(blinkOnOffIceWaterLock, DispSettingIceWaterLockBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOffIceWaterLock = ON;
            Disp.IceWaterLock = FALSE;
        }
    }

    /* 얼음물 용량   잠금 상태에서 선택 시, 점멸 */
    if( Disp.IceWaterAmountLock == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_ICEWATERAMOUNTLOCK) == FALSE )
        {
            blinkOnOffIceWaterAmountLock = BlinkLED(blinkOnOffIceWaterAmountLock, DispSettingIceWaterAmountLockBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOffIceWaterAmountLock = ON;
            Disp.IceWaterAmountLock = FALSE;
        }
    }

    /* 얼음 선택 때 얼음물 용량 선택  시, 알림 */
    if( Disp.IceWaterAmountNoti == TRUE )
    {
        if( IsExpiredDispTimer(DISP_TIMER_ID_ICEWATERAMOUNTNOTI) == FALSE )
        {
            blinkOnOffIceWaterNoti = BlinkLED(blinkOnOffIceWaterNoti, DispSettingIceWaterAmountNotiBlink, DISP_TIMER_ID_500MS);
        }
        else
        {
            blinkOnOffIceWaterNoti = ON;
            Disp.IceWaterAmountNoti = FALSE;
        }
    }    
}

#if 0
static void DisplayPowerSaving(void)
{
    static U8 mu8Timer = 40;
    U8 mu8OnOff = OFF;
    U8 mu8Status = STATUS_WAKE_UP;


    if( GetSavingConfig() == TRUE )
    {
        mu8OnOff = ON;
    }

    mu8Status = GetSavingStatus();
    if( mu8Status == STATUS_WAKE_UP )
    {
        mu8Timer = 40;
    }
    else
    {
        if( mu8Timer <= 10 )
        {
            mu8OnOff = OFF;
        }

        mu8Timer--;
        if( mu8Timer == 0 )
        {
            mu8Timer = 40;
        }
    }

    DispPowerSaving( mu8OnOff );
}
#else
static void DisplayPowerSaving(void)
{
    static U8 blinkOnOff = ON;

    if( GetSavingConfig() == FALSE )
    {
        DispPowerSaving( OFF );
        blinkOnOff = ON;
        return ;
    }

    if( GetSavingStatus() == STATUS_WAKE_UP )
    {
        DispPowerSaving( ON );
        blinkOnOff = ON;
        return ;
    }

    blinkOnOff = BlinkLED(blinkOnOff, DispPowerSaving, DISP_TIMER_ID_3SEC);
}
#endif

static void DisplayServiceMode(void)
{
    U8 onOff = OFF;

    if( GetServiceCheckStatus() == TRUE )
    {
        onOff = ON;
    }
    DispService(onOff);
}

static void DisplayHealth(void)
{
    static U8 blinkOnOff = ON;

    // 배수 중
    if( IsStartDrainWater() == TRUE )
    {
        blinkOnOff = BlinkLED(blinkOnOff, DispRoomWater, DISP_TIMER_ID_3SEC);
    }
    else
    {
        blinkOnOff = ON;
    }

#if 0
    U8 onOff = OFF;
    static U8 blinkOnOff = ON;

    if( GetSterDone() == TRUE )
    {
        //DispSterProgressBar(DISPLAY_STEP_8);
        //DispHorizontalBar(ON);
        //DispSterFauset(OFF);
        //DispSterIceRoom(OFF);
        //DispSterFlowPath(ON);
    }

    if( GetHealthMode() != HEALTH_MODE_NONE )
    {
        onOff = ON;
    }

    //DispDrain( ON );
    DispSter(onOff);

    // 배수 중
    if( IsStartDrainWater() == TRUE )
    {
        blinkOnOff = BlinkLED(blinkOnOff, DispSter, DISP_TIMER_ID_3SEC);
    }
    else
    {
        blinkOnOff = ON;
    }

#endif
    
#if 0
    static U16 mu16Delay = 5;  // @100ms


    // 살균 완료
    if( GetSterDone() == TRUE )
    {
        DispDrain( OFF );
        DispSter( ON );
        DispSterProgressBar( 100 );
        return ;
    }

    // 배수 중
    if( IsStartDrainWater() == TRUE )
    {
        if( mu16Delay != 0 )
        {
            mu16Delay--;
        }
        else
        {
            //BlinkLED( DispDrain, Disp.BlinkTime_4s, 10 );
            DispSter( OFF );
            DispSterProgressBar( 0 );
        }
        return ;
    }
    // 살균 중
    else if( IsStartSter() == TRUE )
    {
        if( mu16Delay != 0 )
        {
            mu16Delay--;
        }
        else
        {
            DispDrain( OFF );
            //BlinkLED( DispSter, Disp.BlinkTime_4s, 10 );
        }
        return ;
    }
    // 살균 설정
    else if( GetHealthMode() != HEALTH_MODE_NONE )
    {

        DispDrain( ON );
        DispSter( OFF );
        DispSterProgressBar( 0 );
        mu16Delay = 5;
        return ;
    }

    // 살균 해제
    DispDrain( OFF );
    DispSter( OFF );
    DispSterProgressBar( 0 );
    mu16Delay = 5;

#endif
}

#if( CONFIG_TEST_UV_MANUAL == 1 )
extern U8 dbg_uv_ice_door;
extern U8 dbg_uv_water_out;
extern U8 dbg_uv_ice_tray;
extern U8 dbg_uv_ice_tank;
extern U8 dbg_uv_drain_tank;
#include "led.h"
static void Test_DisplayUv(void)
{
    OnOffLed(LED_ID_UV_ICE_FAUCET, dbg_uv_ice_door);
    OnOffLed(LED_ID_UV_WATER_FAUCET, dbg_uv_water_out);
    OnOffLed(LED_ID_UV_ICE_TRAY, dbg_uv_ice_tray);
    OnOffLed(LED_ID_UV_ICE_TANK, dbg_uv_ice_tank);
    OnOffLed(LED_ID_UV_CARE_SYSTEM, dbg_uv_drain_tank);
}
#endif

static void DisplayUv(void)
{
    U8 percent = 0U;
    U8 water_out = FALSE;
    U8 ice_out = FALSE;
    U8 ice_tank = FALSE;
    U8 ice_tray = FALSE;

    DispUvLedOffDuty();
    DispSterWaterFaucet(OFF);
    DispSterIceFaucet(OFF);
    DispSterIceRoom(OFF);
    DispSterIceTray(OFF);

    /// WATER OUT
    if( IsTurnOnUvId(UV_STER_WATER_OUT) == TRUE
        && GetPauseUvId(UV_STER_WATER_OUT) == FALSE )
    {
        percent = GetUvOnPercent(UV_STER_WATER_OUT);

        if( percent >= 0 )
        {
            //DispUvCareSystem(ON);
            DispSterWaterFaucet(ON);
            //DispSterIceFaucet(OFF);
            //DispSterIceRoom(OFF);
            //DispSterIceTray(OFF);
            water_out = TRUE;
        }
    }
    else if( IsTurnOnUvId(UV_STER_WATER_OUT_SHOT) == TRUE
              && GetPauseUvId(UV_STER_WATER_OUT_SHOT) == FALSE )
    {
        percent = GetUvOnPercent(UV_STER_WATER_OUT_SHOT);

        if( percent >= 0 )
        {
            //DispUvCareSystem(ON);
            DispSterWaterFaucet(ON);
            //DispSterIceFaucet(OFF);
            //DispSterIceRoom(OFF);
            //DispSterIceTray(OFF);
            water_out = TRUE;
        }
    }
        
    /// ICE OUT
    if( IsTurnOnUvId(UV_STER_ICE_DOOR) == TRUE
         && GetPauseUvId(UV_STER_ICE_DOOR) == FALSE )
    {
        percent = GetUvOnPercent(UV_STER_ICE_DOOR);

        if( percent >= 0 )
        {
            //DispUvCareSystem(ON);
            //DispSterWaterFaucet(OFF);
            DispSterIceFaucet(ON);
            //DispSterIceRoom(OFF);
            //DispSterIceTray(OFF);
            ice_out = TRUE;
        }
    }
    else if( IsTurnOnUvId(UV_STER_ICE_DOOR_SHOT) == TRUE
              && GetPauseUvId(UV_STER_ICE_DOOR_SHOT) == FALSE )
    {
        percent = GetUvOnPercent(UV_STER_ICE_DOOR_SHOT);

        if( percent >= 0 )
        {
            //DispUvCareSystem(ON);
            //DispSterWaterFaucet(OFF);
            DispSterIceFaucet(ON);
            //DispSterIceRoom(OFF);
            //DispSterIceTray(OFF);
            ice_out = TRUE;
        }
    }

    /// ICE TANK
    if( IsTurnOnUvId(UV_STER_ICE_TANK) == TRUE
          && GetPauseUvId(UV_STER_ICE_TANK) == FALSE )
    {
        percent = GetUvOnPercent(UV_STER_ICE_TANK);

        if( percent >= 0 )
        {
            //DispUvCareSystem(ON);
            //DispSterWaterFaucet(OFF);
            //DispSterIceFaucet(OFF);
            DispSterIceRoom(ON);
            //DispSterIceTray(OFF);
            ice_tank = TRUE;
        }
    }

    /// ICE TARY      
    if( IsTurnOnUvId(UV_STER_ICE_TRAY) == TRUE 
            //&& GetIceFullStatus() == TRUE
            && GetPauseUvId(UV_STER_ICE_TRAY) == FALSE )
    {
        percent = GetUvOnPercent(UV_STER_ICE_TRAY);

        if( percent >= 0 )
        {
            //DispUvCareSystem(ON);
            //DispSterWaterFaucet(OFF);
            ///DispSterIceFaucet(OFF);
            //DispSterIceRoom(OFF);
            DispSterIceTray(ON);
            ice_tray = TRUE;
        }
    }
    else if( IsTurnOnUvId(UV_STER_ICE_TRAY_SHOT) == TRUE
              && GetPauseUvId(UV_STER_ICE_TRAY_SHOT) == FALSE )
    {
        percent = GetUvOnPercent(UV_STER_ICE_TRAY_SHOT);

        if( percent >= 0 )
        {
            //DispUvCareSystem(ON);
            //DispSterWaterFaucet(OFF);
            //DispSterIceFaucet(OFF);
            //DispSterIceRoom(OFF);
            DispSterIceTray(ON);
            ice_tray = TRUE;
        }
    }

    if( water_out == FALSE 
        && ice_out == FALSE
        && ice_tank == FALSE 
        && ice_tray == FALSE 
        )
    {
        //DispUvCareSystem(OFF);

        DispSterWaterFaucet(OFF);
        DispSterIceFaucet(OFF);
        DispSterIceRoom(OFF);
        DispSterIceTray(OFF);
    }
}

//""SUBR COMMENT""************************************************************
// ID     : DisplayTempLevelHot
// 기능	  : 온수 수위 레벨 표시  
//----------------------------------------------------------------------------
// 개요      :  
//----------------------------------------------------------------------------
//""SUBR COMMENT END""********************************************************
static void DisplayTempLevelHot(U8 onOff)
{
    U8 tempStatus = TEMP_STATUS_NONE;

    if( onOff == OFF )
    {
        DispHotTempLevel(OFF);
        return ;
    }

    tempStatus = DispTempStatus(SEL_WATER_HOT, Disp.TempHot);

    if( tempStatus == TEMP_STATUS_HEATING
        || tempStatus == TEMP_STATUS_HEATING_DIM )
    {
        DispHotTempLevel(ON);
    }
    else
    {
        DispHotTempLevel(OFF);
    }
}

//""SUBR COMMENT""************************************************************
// ID     : DisplayTempLevelCold
// 기능	  : 냉수 수위 레벨 표시  
//----------------------------------------------------------------------------
// 개요      :  
//----------------------------------------------------------------------------
//""SUBR COMMENT END""********************************************************
static void DisplayTempLevelCold(U8 onOff)
{
    U8 tempStatus = TEMP_STATUS_NONE;

    if( onOff == OFF )
    {
        DispColdTempLevel(OFF);
        return ;
    }
    
    tempStatus = DispTempStatus(SEL_WATER_COLD, Disp.TempCold);

    if( tempStatus == TEMP_STATUS_COOLING
        || tempStatus == TEMP_STATUS_COOLING_DIM )
    {
        DispColdTempLevel(ON);
    }
    else
    {
        DispColdTempLevel(OFF);
    }
}

static void DispWaterOutInfinity(void)
{
    if( GetTankClean() == TRUE )
    {
        return;
    }
  
    DispWaterOutAmountInfinity();
}

#define DISP_WATER_OUT_PROGRESS_1 (33)    /// 33%
#define DISP_WATER_OUT_PROGRESS_2 (66)    /// 66%
static void DispWaterOutPercent(void)
{
    U8 percent = GetWaterOutPercent();

    SetLedAllOffDuty();

    if( GetTankClean() == TRUE )
    {
        return;
    }

    if( Disp.IsDispContyTime != 0U )
    {
        Disp.IsDispContyTime--;

        DispContyInfinity(OFF);
        DispConty1D5L(OFF);
        DispConty1L(OFF);
        DispConty500mL(OFF);
        return;
    }

    DispWaterOutBarLedOffDuty();

    if( percent > DISP_WATER_OUT_PROGRESS_2 )
    {
        DispContyInfinity(OFF);
        DispConty1D5L(ON);
        DispConty1L(ON);
        DispConty500mL(ON);
    }
    else if( percent > DISP_WATER_OUT_PROGRESS_1 )
    {
        DispContyInfinity(OFF);
        DispConty1D5L(OFF);
        DispConty1L(ON);
        DispConty500mL(ON);
    }
    else
    {
        DispContyInfinity(OFF);
        DispConty1D5L(OFF);
        DispConty1L(OFF);
        DispConty500mL(ON);        
    }  
}

static void DispIceWaterOutPercent(void)
{
#if( CONFIG_USE_FLOW_METER_AT_ICE_WATER == 1 )
    U8 percent = GetIceWaterOutAmountPercent();
#else
    U8 percent = GetIceWaterOutPercent();
#endif

    SetLedAllOffDuty();

    if( GetTankClean() == TRUE )
    {
        return;
    }

    if( Disp.IsDispIceWaterContyTime != 0U )
    {
        Disp.IsDispIceWaterContyTime--;

        DispContyInfinity(OFF);
        DispConty1D5L(OFF);
        DispConty1L(OFF);
        DispConty500mL(OFF);
        return;
    }

    DispWaterOutBarLedOffDuty();

    if( percent > DISP_WATER_OUT_PROGRESS_2 )
    {
        DispContyInfinity(OFF);
        DispConty1D5L(ON);
        DispConty1L(ON);
        DispConty500mL(ON);
    }
    else if( percent > DISP_WATER_OUT_PROGRESS_1 )
    {
        DispContyInfinity(OFF);
        DispConty1D5L(OFF);
        DispConty1L(ON);
        DispConty500mL(ON);
    }
    else
    {
        DispContyInfinity(OFF);
        DispConty1D5L(OFF);
        DispConty1L(OFF);
        DispConty500mL(ON);        
    }  
}

static void DispIceOutEffect(void)
{
    SetLedType(LED_ID_SEG_1A, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_1B, LED_TYPE_DUTY);

    SetLedType(LED_ID_SEG_2A, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_2B, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_2C, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_2D, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_2E, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_2F, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_2G, LED_TYPE_DUTY);

    SetLedType(LED_ID_SEG_3A, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_3B, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_3C, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_3D, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_3E, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_3F, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_3G, LED_TYPE_DUTY);

    SetLedType(LED_ID_SEG_4A, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_4B, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_4C, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_4D, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_4E, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_4F, LED_TYPE_DUTY);
    SetLedType(LED_ID_SEG_4G, LED_TYPE_DUTY);

    if( Disp.IceOut == 0 )
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ICE_1 );
    }
    else if( Disp.IceOut == 1 )
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ICE_2 );
    }
    else if( Disp.IceOut == 2 )
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ICE_3 );
    }
    else
    {
        DispSegmentChar( SEGMENT_CHAR_ID_ICE_4 );
    }

    if( IsExpiredDispBlinkTimer( DISP_TIMER_ID_500MS ) == TRUE )
    {
        Disp.IceOut++;
        if( Disp.IceOut > 3 )
        {
            Disp.IceOut = 0;
        }
    }
}

static void Disp7Seg_IceOut(void)
{
    DispIceOutEffect();
    // 얼음 추출과 연속 선택 버튼이 겹쳐있을 때
    // 얼음 추출이 종료되면 연속 선택 효과를 초기화
    ResetInfinityCount();
    ResetLeverCount();
}

static void Disp7Seg_IceWaterOut(void)
{
    DispStatusIceWater(ON);
    DispIceOutEffect();
    // 얼음 추출과 연속 선택 버튼이 겹쳐있을 때
    // 얼음 추출이 종료되면 연속 선택 효과를 초기화
    ResetInfinityCount();
    ResetLeverCount();
}

static TEMP_T GetDispTemp(void)
{
    if( GetWaterOutSelect() == SEL_WATER_COLD )
    {
        return Disp.TempCold;
    }

    if( GetWaterOutSelect() == SEL_WATER_HOT )
    {
        if( GetTempMonitoring() == TRUE )
        {
            return Disp.TempAmbi2;
        }

        return Disp.TempHot;
    }

    if( GetTempMonitoring() == TRUE )
    {
        return Disp.TempAmbi1;
    }

    return Disp.TempRoom;
}

static void Disp7Seg_Standby(void)
{
    static U8 init = FALSE;
    TEMP_T temp;   

    if( init == FALSE )
    {
        init = TRUE;
        Disp.TempRoom = GetTemp(TEMP_ID_ROOM_WATER);
        Disp.TempCold = GetTemp(TEMP_ID_COLD_WATER);
        Disp.TempHot = GetTemp(TEMP_ID_HOT_WATER);
        Disp.TempAmbi1 = GetTemp(TEMP_ID_AMBIENT);
        Disp.TempAmbi2 = GetTemp(TEMP_ID_AMBIENT_2);
    }

    ResetInfinityCount();
    ResetLeverCount();

    if( GetWaterOutSelect() == SEL_WATER_COLD )
    {
        if( GetTempMonitoring() == FALSE )
        {
            if( GetColdWaterConfigMake() == FALSE )
            {
                DispSegStr(" OFF");
                return;
            }
        }
    }

    if( GetWaterOutSelect() == SEL_WATER_HOT )
    {
        if( GetTempMonitoring() == FALSE )
        {
            if( GetHotWaterConfigMake() == FALSE )
            {
                DispSegStr(" OFF");
                return;
            }
        }
    }

    //DispSegStr("TEP");
    temp = GetDispTemp();
    if( temp < 0.0f )
    {
        DispSegNegativeDigits((I16)temp);        
    }
    else
    {
        DispSegDigits((U16)temp);
    }
    
    DispUnitCelsius(ON);
}


/* For aging test, Number of aging cycle repetitions */
static void Disp7SegAgingTestNum(void)
{
    U16 mu16Val;

    mu16Val = (U16)GetCycleCount();

    DispSegDigits( mu16Val );
}

#define DISP_CONTY_TIME     (5U) /// 500ms @100ms
static void Display7Segment(void)
{
    /* SEG OFF 표시 해야 할 상황 일 때 */
    if( IsExpiredSegDispTimer( DISP_TIMER_SEGOFF ) == FALSE )
    {
        DispSegStr(" OFF");
        return;
    }

    /* SEG ON 표시 해야 할 상황 일 때 */
    if( IsExpiredSegDispTimer( DISP_TIMER_SEGON ) == FALSE )
    {
        DispSegStr("  On");
        return;
    }

    /* Aging Test */
    if( GetAging() == TRUE )
    {
        Disp7SegAgingTestNum(); // Display AgingTest
        return;
    }

    /* 물 추출(얼음물 제외) */
    if( Disp.WaterPanel == TRUE )
    {
        if( GetWaterOut() == TRUE )
        {
            if( GetWaterOutContinue() == TRUE )
            {
                ResetLeverCount();
                DisplayWaterConty();

                if( GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
                {
                    /// 애니메이션
                    DispWaterOutInfinity();
                }
                else
                {
                   /// 용량바로 단계 표시 ( 33%, 66%, 100% )
                   DispWaterOutPercent();
                }
            }
            else
            {
                DispWaterOutLever();
            }
            
            return;
        }
        else
        {
            Disp.IsDispContyTime = DISP_CONTY_TIME;       
            ResetInfinityCount();
            ResetLeverCount();

            if( GetWaterOutContinue() == TRUE )
            {
                DisplayWaterConty();
                return;
            }
        }
    }

    /* 얼음, 얼음물 추출 */
    if( Disp.IcePanel == TRUE )
    {
        if( IsOpenIceDoor() == TRUE )
        {
            if( IsStartIceDoorWater() == TRUE )    /// ICE WATER
            {
                if( GetIceWaterOutContinue() == TRUE )
                {
                    DisplayIceWaterConty();
                   
                    DispIceWaterOutPercent(); /// 용량바로 단계 표시 ( 33%, 66%, 100% )
                }
                else
                {
                    Disp7Seg_IceWaterOut();
                }
            }
            else                              /// ICE
            {
                Disp7Seg_IceOut();
            }
              
            return;
        }
        else if( IsStartIceDoorWater() == TRUE )    /// ICE WATER
        {
            if( GetIceWaterOutContinue() == TRUE )
            {
                DisplayIceWaterConty();
               
                DispIceWaterOutPercent(); /// 용량바로 단계 표시 ( 33%, 66%, 100% )
            }
            else
            {
                Disp7Seg_IceWaterOut();
            }

            return;
        }
        else
        {
            Disp.IsDispIceWaterContyTime = DISP_CONTY_TIME;
            Disp.IceOut = 0U;
          
            if( GetIceWaterOutContinue() == TRUE )
            {
                DisplayIceWaterConty();
                return;
            }
        }
    }

    if( Disp.IcePanel == FALSE
        || Disp.WaterPanel == FALSE )
    {
        ClearPanel();
        return;
    }   

    Disp7Seg_Standby();
}

enum 
{
    DISP_VERSION_START,
    DISP_VERSION_MODEL,
    DISP_VERSION_VERSION,
    DISP_VERSION_END,
};

static void ProcessDisplayVersionMode(void)
{
    if( IsExpiredDispTimer(DISP_TIMER_ID_VERSION) == FALSE )
    {
        return;
    }

    /* 1. TURN OFF ALL LED */
    TurnOffAllLED();	

    /// 1. ALL OFF LED - 1 SEC
    /// 2. DISPLAY VERSION - 1 SEC
    /// 3. DISPLAY SYSTEM TYPE(CHP or CP) - 1 SEC
    /// 1. ALL OFF LED
    switch( Disp.VersionStep )
    {
        case DISP_VERSION_START: 
            ExportVersion(PROGRAM_VERSION);

            SetDispTimer( DISP_TIMER_ID_VERSION, TIMER_1SEC );
            Disp.VersionStep++;
            break;

        case DISP_VERSION_MODEL:
            DispMainVersionBar();
            DispVersion(GetMainVerMiner());
            SetDispTimer( DISP_TIMER_ID_VERSION, TIMER_1SEC );
            Disp.VersionStep++;
            break;

        case DISP_VERSION_VERSION:
            DispFrontVersionBar();
            DispVersion(GetFrontVerMiner());
            SetDispTimer( DISP_TIMER_ID_VERSION, TIMER_1SEC );
            Disp.VersionStep++;
            break;

        case DISP_VERSION_END:
            TurnOffAllLED();	
            Disp.Version     = FALSE;
            Disp.VersionStep = 0;
            break;

        default:
            Disp.Version     = FALSE;
            Disp.VersionStep = 0;
            break;

    }
}

static void ProcessDisplayMementoMode(void)
{
    SMemento_T memento;
    EErrorId_T error;

    if( Disp.MementoTimer != 0 )
    {
        Disp.MementoTimer--;
        return ;
    }

    // 1. ALL ON LED - 1 SEC
    // 2. ALL OFF LED - 1 SEC
    // 3. DISPLAY ERROR 5SEC & SOUND 1~5
    // 3. DISPLAY ERROR 5SEC & SOUND 1
    switch( Disp.MementoStep )
    {
        case 0 : 
            TurnOnAllLED();	
            Disp.MementoTimer = 10;

            Disp.MementoStep++;
            break;

        case 1:
            TurnOffAllLED();
            Disp.MementoTimer = 10;

            Disp.MementoStep++;
            break;

        case 2:
            GetMementoError(4, &memento);
            error = memento.Error;
            if( error == ERR_ID_NONE )
            {
                Disp.MementoStep = 0;

            }
            else
            {
                DisplayError(error, ON);
                Sound(SOUND_MEMENTO_1);
                Disp.MementoTimer = 50;

                Disp.MementoStep++;
            }
            break;

        case 3:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep++;
            break;

        case 4:
            GetMementoError(3, &memento);
            error = memento.Error;
            if( error == ERR_ID_NONE )
            {
                Disp.MementoStep = 0;

            }
            else
            {
                DisplayError(error, ON);
                Sound(SOUND_MEMENTO_2);
                Disp.MementoTimer = 50;

                Disp.MementoStep++;
            }
            break;

        case 5:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep++;
            break;

        case 6:
            GetMementoError(2, &memento);
            error = memento.Error;
            if( error == ERR_ID_NONE )
            {
                Disp.MementoStep = 0;

            }
            else
            {
                DisplayError(error, ON);
                Sound(SOUND_MEMENTO_3);
                Disp.MementoTimer = 50;

                Disp.MementoStep++;
            }
            break;

        case 7:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep++;
            break;

        case 8:
            GetMementoError(1, &memento);
            error = memento.Error;
            if( error == ERR_ID_NONE )
            {
                Disp.MementoStep = 0;

            }
            else
            {
                DisplayError(error, ON);
                Sound(SOUND_MEMENTO_4);
                Disp.MementoTimer = 50;

                Disp.MementoStep++;
            }
            break;

        case 9:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep++;
            break;

        case 10:
            GetMementoError(0, &memento);
            error = memento.Error;
            if( error == ERR_ID_NONE )
            {
                Disp.MementoStep = 0;

            }
            else
            {
                DisplayError(error, ON);
                Sound(SOUND_MEMENTO_5);
                Disp.MementoTimer = 50;

                Disp.MementoStep++;
            }

            break;

        case 11:
            TurnOffAllLED();
            Disp.MementoTimer = 10;
            Disp.MementoStep = 0;
            break;

    }

    return;
}

/* 디스플레이 초기화 표시 */
static void ProcessDisplayBoot(void)
{
    static U8 count = 5;
    static U8 toggle = 0;

    //if( Disp.BlinkTime_5s == 0 )
    if( IsExpiredDispBlinkTimer(DISP_TIMER_ID_500MS) == TRUE )
    {
        toggle = !toggle;

        count--;
        if( count == 0 )
        {
            TurnOffAllLED();
            
            Disp.Init = TRUE;
            SetVersionDisp(0);
            count = 5;
            return;
        }
    }

    if( toggle == 0 )
    {
        TurnOffAllLED();
    }
    else
    {
        TurnOnAllLED();
    }
}

/* 디스플레이 온수 고도 설정 */
static void ProcessDisplayConfHotAltitude(void)
{
    U8 altitude;
    static U8 blinkOnOff = ON;

    /* 1. SETUP ALTITUDE */
    altitude = GetHotWaterAltidue();
    TurnOffAllLED();	

    switch( altitude )
    {
        case ALTITUDE_LEVEL_0 :
            DispSegStr("-1-");
            blinkOnOff = BlinkLED( blinkOnOff, DispHotWater, DISP_TIMER_ID_500MS );
            break;

        case ALTITUDE_LEVEL_1 :
            DispSegStr("-2-");
            blinkOnOff = BlinkLED( blinkOnOff, DispRoomWater, DISP_TIMER_ID_500MS );
            break;

        case ALTITUDE_LEVEL_2 :
            DispSegStr("-3-");
            blinkOnOff = BlinkLED( blinkOnOff, DispColdWater, DISP_TIMER_ID_500MS );
            break;

        default : 
            SetHotWaterAltidue( ALTITUDE_LEVEL_0 );
            break;
    }

    /* 2. RELEASE ALTITUDE MODE WHEN TIMEOUT */
    //if( Disp.AltiTimeOut == 0 )
    if( IsExpiredDispTimer(DISP_TIMER_ID_HOT_ALTITUDE) == TRUE )
    {
        Disp.AltiConfig = FALSE; 

        blinkOnOff = ON;

        Sound(SOUND_CANCEL);

        /* Save EEPROM */
        SaveEepromId(EEP_ID_HOT_ALTITUDE);
    }
    else
    {
        //Disp.AltiTimeOut--;
    }
}

static void ProcessDisplayLockAll(void)
{
    TurnOffAllLED();

    DispSegStr("Loc");

    DisplayLockAll();

    DisplayAllLockBlink();

    return ;
}

static void ProcessDisplaySettingMode(void)
{
    TurnOffAllLED();

    DispAmount(ON);
    DispSettingIceLock(ON);
    DispSettingIceWaterLock(ON);
    DispSettingHotLock(ON);

    /* Lock Ice */
    DisplayLockIce();

    /* Lock Ice Water */
    DisplayLockIceWater();

    /* Lock Hot Water */
    DisplayLockHot();

    /* Ice Lock 설정 점멸 */
    DisplayLockIceSet();
    
    /* Ice Lock 설정 점멸 */
    DisplayLockIceWaterSet();

    /* Hot Lock 설정 점멸 */
    DisplayLockHotSet();

    /* All Lock 설정 점멸 */
    //DisplayLockAllSet();

    // 일정 시간 버튼 입력이 없으면 설정 메뉴에서 빠져나옴.
    if( IsExpiredDispTimer( DISP_TIMER_ID_SETTING_MENU ) == TRUE )
    {
        SetSettingDisp( FALSE );
    }

#if 0

  // ICE 4.0

    // SETTING 
    if( GetSettingUserAmount() == TRUE )
    {
        U16 mu16Liter;

        mu16Liter= GetWaterOutUserAmountLiter();
        DispWaterOutAmount( mu16Liter );

        DisplaySelAmount();
    }
#endif
}

/* 자동 배수 설정/해제 */
static void ProcessDisplayConfDrainWater(void)
{
    U8 mode;
    static U8 blinkOnOff = ON;

    TurnOffAllLED();	

    if( IsSetHealthMode(HEALTH_MODE_DRAIN) == TRUE )
    {
        blinkOnOff = BlinkLED(blinkOnOff, DispRoomWater, DISP_TIMER_ID_500MS);
    }
    else
    {
        blinkOnOff = BlinkLED(blinkOnOff, DispColdWater, DISP_TIMER_ID_500MS);
    }

    /* 2. RELEASE WHEN TIMEOUT */
    //if( Disp.DrainWaterTimeOut == 0 )
    if( IsExpiredDispTimer(DISP_TIMER_ID_DRAIN_WATER) == TRUE )
    {
        Disp.DrainWaterConfig = FALSE; 

        blinkOnOff = ON;

        Sound( SOUND_CANCEL );
    }
    else
    {
        //Disp.DrainWaterTimeOut--;
    }
}

static void ProcessDisplaySmartSaveConfig(void)
{
    U8 mu8Conf;
    TurnOffAllLED();	
    OnOffLed(LED_ID_HOT, ON);
        
    mu8Conf = GetUnusedSavingConfig();
    if( mu8Conf == TRUE )
    {
        DispSegStr("On");
    }
    else
    {
        DispSegStr("OFF");
    }

    /* 2. If there is no key input for more than 10 sec, then unused save mode is canceled */
    if( GetSmartSaveConfigModeTimer() == 0 )
    {
        SetSmartSaveConfigMode(FALSE);	
        Sound( SOUND_SETUP );     // Why sound setup? -> It's just mode out, config has been set
    }
    else
    {
        UpdateSmartSaveConfigModeTimer();
    }
}

static void ProcessDisplayNormalMode(void)
{
#if 1
    TurnOffAllLED();

    // 설정 메뉴 OFF
    //DispSetting( OFF );
#endif

    /* 트리플 안심 케어 */
    DisplayStandBy();
    
    /* Lock All */
    //DisplayLockAll();

    /* Lock Ice */
    DisplayLockIce();

    /* Lock Ice Water */
    DisplayLockIceWater();
    DisplayLockIceWaterAmount();

    /* Lock Hot Water */
    DisplayLockHot();

    /* Lock Icon */
    DisplayLockIcon();

    /* Ice Make */
    DisplayIceMake();
    
    /* Ice Turbo */
    DisplayIceTurbo();

    /* Uv */
#if( CONFIG_TEST_UV_MANUAL == 1 )
    Test_DisplayUv();
#else
    DisplayUv();
#endif

    /* Water Preparing */
    DisplayWaterPreparing();

    /* Ice Full */
    DisplayIceFull();

    /* Ice Low */
    DisplayIceFullLow();

    /* Save Mode */
    DisplayPowerSaving();

    /* Water Set , Out */ 
    DisplaySelWater();

    /* Ice Set , Out */ 
    DisplaySelIce();

    /* Water Conty */
    //DisplayWaterConty();

    /* Health - Ster/DrainWater */
    //DisplayHealth();

    /* SERVICE MODE */
    DisplayServiceMode();

    /* WELCOME LED - WATER */
    //DisplayWelcomLightWater();

    /* WELCOME LED - ICE */
    //DisplayWelcomLightIce();

    /* ICE MAKE DECO */
    DisplayDecoIceMaker();

    /* Segment 디스플레이 */
    Display7Segment();

    /* Water Temp Level - HOT / COLD */
    DisplayWaterTempLevel();

    /* Health - Force Drain Water 점멸 */
    DisplayHealth();

    /* Ice Make 꺼짐 점멸 */
    DisplayIceMakeBlink();

    /* Cold Make 꺼짐 점멸 */
    DisplayColdMakeBlink();

    /* Hot Make 꺼짐 점멸 */
    DisplayHotMakeBlink();

    /* Ice Lock 설정 점멸 */
    DisplayLockIceSet();
    
    /* Ice Water Lock 설정 점멸 */
    DisplayLockIceWaterSet();

    /* Hot Lock 설정 점멸 */
    DisplayLockHotSet();

    /* 얼음물 추출 제한 점멸 */
    DisplayNotIceWater();
    
    /* 냉수 추출 제한 점멸 */
    DisplayNotColdWater();

    /* All Lock 설정 점멸 */
    //DisplayLockAllSet();

    /* Lock 점멸 */
    DisplayLock();
}

static void ProcessDisplayErrorMode(void)
{
    static EErrorId_T prevErr = 0;
    EErrorId_T curErr = 0;

    static U8 cmd = 0;
    static U8 counter = 10;

    if( IsExpiredDispTimer(DISP_TIMER_ID_ERROR) == TRUE )
    {
        cmd = ON;
        return;
    }

    /* Get error code */
    curErr = GetErrorId();

    /* Display error code */
    if( curErr == ERR_ID_NONE )
    {
        cmd = 0;
        counter = 10;
    }
    else
    {
        counter--;
        if( counter == 0 )
        {
            counter = 10;
        }

        if( counter <= 5 )
        {
            cmd = 0;
        }
        else
        {
            cmd = 1;
        }

        DispUnitCelsius(OFF);
        DisplayError(curErr, cmd);
    }

    /* Sound new error code */
    if( prevErr != curErr )
    {
        prevErr = curErr;
    }
}

static void ProcessDisplaySter(void)
{
#if 0
    U8 onOff = OFF;
    U16 mode  = STER_MODE_NONE;
    U8 sterInitStep;
    static U8 blinkOnOff = ON;

    SetLedAllType();
    SetLedAllOnDuty();

    TurnOffAllLED();

    mode  = GetSterMode();
    sterInitStep = (U8)GetSterInit();
#endif
    // 살균 진행 상태 표시
    //DispSterilizeProcess(ON, mode, sterInitStep);
    //DispTripleSafeCare(ON);
    //DispHorizontalBar(OFF);
    //DispSterFauset(OFF);
    //DispSterIceRoom(OFF);
    //DispSterFlowPath(ON);
}

void ProcessDisplay(void)
{
    UpdateDisplayTimer();   
    UpdateSegDisplayTimer();
    UpdateDispTemp();

    SetLedParameter();

    /* BOOT */
    if( Disp.Init == FALSE )
    {
        ProcessDisplayBoot();
        return ;
    }

#if( CONFIG_TEST_CHECK_FRONT == 1 )
    return ;
#endif

    /* DISPLAY VERSION */
    if( Disp.Version == TRUE )
    {
        ProcessDisplayVersionMode();
        return ;
    }

    /* TEST - EOL */
    if( GetEolStatus() == TRUE )
    {
        ProcessDisplayEol();
        return ;
    }

    /* CONFIG - HOT WATER ALTITUDE */
    if( Disp.AltiConfig == TRUE )
    {
        ProcessDisplayConfHotAltitude();
        return ;
    }

    /* CONFIG - HEALTH ADJUST HOUR */
    if( Disp.AdjustHourConfig == TRUE )
    {
        //ProcessDisplayConfAdjustHour();
        return ;
    }

    if( Disp.SetHourConfig == TRUE )
    {
        //ProcessDisplayConfSetHour();
        return ;
    }

    /* CONFIG - DRAIN WATER ON/OFF */
    if( Disp.DrainWaterConfig == TRUE )
    {
        ProcessDisplayConfDrainWater();
        return ;
    }

    /* CONFIG - SMART UNUSED SAVE */
    if( GetSmartSaveConfigMode() == TRUE )
    {
        ProcessDisplaySmartSaveConfig();
        return ;
    }

    /* MEMENTO */
    if( Disp.Memento == TRUE )
    {
        ProcessDisplayMementoMode();
        return ;
    }

    /* 전체 잠금 */
    if( GetLockAll() == LOCK )
    {
        ProcessDisplayLockAll();
        return ;
    }

    /* SETTING */
    if( Disp.Setting == TRUE )
    {
        ProcessDisplaySettingMode();
        return ;
    }

#if 0
    /* STER */
    if ( IsStartSter() == TRUE )
    {
        ProcessDisplaySter();
        ProcessDisplayErrorMode();
        return;
    }    
#endif

    /* NORMAL MODE */
    ProcessDisplayNormalMode();

    /* ERRROR MODE */
    ProcessDisplayErrorMode();
}
