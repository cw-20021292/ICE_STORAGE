/**
 * File : hot_water.c
 * 
 * Hot Water
*/
#include "hot_water.h"
#include "room_water.h"
#include "cold_water.h"
#include "level.h"
#include "temp.h"
#include "eeprom.h"
#include "health.h"
#include "power_saving.h"
#include "error.h"
#include "room_water.h"
#include "front.h"
#include "drain_water.h"
#include "tank_clean.h"
#include "valve.h"
#include "service.h"
#include "circulate_pump.h"
#include "smart_saving.h"


/* 미사용 절전 외기 온도 20'c 조건에서 해제 */
#define USE_SAVING_TEMP    (1)      /* 1 : 활성화, 0: 비활성화 */


/* TARGET OFF TEMP - PROTECTION OVER-HEATING */
#define PROTECT_HOT_TEMP   (98.0f)

#define WAIT_TIME          (5)          // @1sec
#define AIR_FLOW_TIME      (3000)       // @100ms
#define WAIT_HEATING_TIME  (900)        // @1sec, 15x60=300
#define WAIT_HEATING_TIME_TANK_OPEN   (300) // @1sec, 5x60=300

/* 미사용 절전 TEMP STATUS */
#define OVER_TEMP         (0)       
#define UNDER_TEMP        (1)
#define DEFAULT_TEMP_TIME (5)   // @1sec, 5sec
#define AMBI_LIMIT_TEMP   (20.0f)

/* TARGET TEMP - NORMAL */
//static const TEMP_T OnTempList[ ALTITUDE_LEVEL_NUM ]  = { 88.0f, 83.0f, 80.0f };
//static const TEMP_T OffTempList[ ALTITUDE_LEVEL_NUM ] = { 97.0f, 88.0f, 84.0f };

static const TEMP_T OnTempList[ ALTITUDE_LEVEL_NUM ]  = { 88.0f, 83.0f, 80.0f };
static const TEMP_T OffTempList[ ALTITUDE_LEVEL_NUM ] = { 93.0f, 88.0f, 84.0f };

//static const TEMP_T OnTempList[ ALTITUDE_LEVEL_NUM ]  = { 85.0f, 83.0f, 80.0f };
//static const TEMP_T OffTempList[ ALTITUDE_LEVEL_NUM ] = { 91.0f, 88.0f, 84.0f };

/* TARGET TEMP - Power Saving */
static const TEMP_T OnTempPowerSavingList[ ALTITUDE_LEVEL_NUM ]  = { 80.0f, 77.0f, 77.0f };
static const TEMP_T OffTempPowerSavingList[ ALTITUDE_LEVEL_NUM ] = { 93.0f, 88.0f, 84.0f };

/* TARGET TEMP - Unsued Power Saving */
static const TEMP_T OnTempUnsuedSavingList[ ALTITUDE_LEVEL_NUM ]  = { 60.0f, 60.0f, 60.0f };
static const TEMP_T OffTempUnusedSavingList[ ALTITUDE_LEVEL_NUM ] = { 85.0f, 80.0f, 76.0f };

HotWater_T Hot;

void InitHotWater(void)
{
    Hot.InitWaitTime = WAIT_TIME;
    Hot.InitFull     = FALSE;
    Hot.Level        = LEVEL_LOW;
    Hot.ConfigMake   = TRUE;
    Hot.Make         = FALSE;
    Hot.MakeDelay    = WAIT_HEATING_TIME;
    Hot.MakeDelayTankOpen = 0;
    Hot.TempLevel    = 0U;
    Hot.Altitude     = ALTITUDE_LEVEL_0;
    Hot.AirFlowStatus  = TRUE; 
    Hot.IsAirFlow      = FALSE;  // 동작 됨.
    Hot.CheckAir       = FALSE;  
    Hot.AirFlowTime    = AIR_FLOW_TIME;
}

void  GetHotWaterData(HotWater_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Hot, sizeof( HotWater_T ));
}

void  SetHotWaterInitFull(U8 val)
{
    Hot.InitFull = val;
}

U8 GetHotWaterInitFull(void)
{
    return Hot.InitFull;
}

void SetHotWaterLevel(U8 val)
{
    Hot.Level = val;
}

U8 GetHotWaterLevel(void)
{
    return Hot.Level;
}

void SetHotWaterConfigMake(U8 val)
{
    if( val != TRUE && val != FALSE )
    {
        return ;
    }
    Hot.ConfigMake = val;

    SaveEepromId(EEP_ID_CONF_MAKE_HOT);
}

U8 GetHotWaterConfigMake(void)
{
    return Hot.ConfigMake;
}

void  SetHotWaterMake(U8 val)
{
    Hot.Make = val;
}

U8 GetHotWaterMake(void)
{
    return Hot.Make;
}

void SetHotWaterAltidue(U8 val)
{
    if( val < ALTITUDE_LEVEL_NUM )
    {
        Hot.Altitude = val;
    }
}

U8 GetHotWaterAltidue(void)
{
    return Hot.Altitude;
}

static void DoingAirFlow(void)
{
    if( Hot.IsAirFlow == FALSE )
    {
        return ;
    }

    if( Hot.AirFlowTime == 0 )
    {
        Hot.IsAirFlow = FALSE;
        return ;
    }

    Hot.AirFlowTime--;
    if( Hot.AirFlowTime == 1 )
    {
        OpenValve(VALVE_NOS_FLOW);
        CloseValve(VALVE_ICE_TRAY_IN);
        TurnOffCirculatePumpTrayIn();
    }
    else if( Hot.AirFlowTime > 0 )
    {
        CloseValve(VALVE_NOS_FLOW);
        OpenValve(VALVE_ICE_TRAY_IN);
        TurnOnCirculatePumpTrayIn();
    }
}

void StartHotWaterAirFlow(void)
{
    Hot.AirFlowStatus = TRUE;
    Hot.CheckAir = FALSE;
}

static void StartDoingAirFlow(void)
{
    Hot.IsAirFlow   = TRUE;
    Hot.AirFlowTime = AIR_FLOW_TIME;
}

static void StopDoingAirFlow(void)
{
    if( Hot.IsAirFlow == TRUE )
    {
        if( Hot.AirFlowTime > 2 )
        {
            Hot.AirFlowTime = 2;
        }
    }
}

static void ControlAirFlow(void)
{
    static U16 delay = 50;  // 5sec...@100ms

    // 전원 Reset 후, 지연 시간 적용
    // 정수 수위 안정화 시간
    if( delay != 0 )
    {
        delay--;
        return ;
    }

#if CONFIG_STER
    if( IsStartHealth() == TRUE )
    {
        return ;
    }
#endif

    // 에어 빼기 제어
    DoingAirFlow();


    if( GetRoomWaterInitFull() == TRUE || Hot.AirFlowStatus == FALSE )
    {
        // 정수 최초 만수위 이면, 중지
        // 이미 한 번 동작 했으면 중지
        Hot.CheckAir = TRUE;    // 에어 빼기 동작 가능 상태로 전환
        StopDoingAirFlow();
        return ;
    }


    if( IsDetectTankLevel( LEVEL_ID_ROOM, LEVEL_DETECT_LOW ) == FALSE )
    {
        // 저수위 미감지인데, 동작한 적이 없으면 동작 
        if( Hot.CheckAir == FALSE )
        {
            Hot.CheckAir = TRUE;
            StartDoingAirFlow();
        }
    }
    else if( IsDetectTankLevel( LEVEL_ID_ROOM, LEVEL_DETECT_LOW ) == TRUE
            || IsDetectTankLevel( LEVEL_ID_ROOM, LEVEL_DETECT_HIGH ) == TRUE 
            || IsDetectTankLevel( LEVEL_ID_ROOM, LEVEL_DETECT_OVF ) == TRUE )
    {
        // 저수위 미감지가 아니면, 동작 조건 해제 시킨다.
        // 동작 중이면 중지도 함꼐 시킨다.
        Hot.CheckAir = TRUE;
        Hot.AirFlowStatus = FALSE;

        StopDoingAirFlow();
    }
}


void UpdateHotMakeDelay(void)
{
    Hot.MakeDelay = 0U;
}

void ClearHotMakeDelayTankOpen(void)
{
    Hot.MakeDelayTankOpen = 0U;
}

void UpdateHotAirFlowTime(void)
{
    Hot.AirFlowTime = 5U;
}


void  ControlHotWaterLevel(void)
{
    /* Update Water Level */
    Hot.Level = GetTankLevel( LEVEL_ID_HOT );

    if( Hot.Level == LEVEL_HIGH )
    {
        Hot.InitFull = TRUE;
    }
#if CONFIG_HOT_WATER_AIRFLOW
    ControlAirFlow();
#endif
}

#define TANK_OPEN_LIMIT_TIME  (7)  // 7s @1000ms
static U8 IsValidMake(void)
{
    if( Hot.InitWaitTime != 0 )
    {
        return FALSE;
    }

    /* Turn Off make, dont want to make */
    if( Hot.ConfigMake == FALSE )
    {
        return FALSE ;
    }

    /* Turn Off make, no water */
    if( Hot.Level != LEVEL_HIGH )
    {
        return FALSE ;
    }

    /* LOCK ALL */
    if( GetLockAll() == LOCK )
    {
        return FALSE;
    }

#if 0
    /* Room Tank */
    if( GetRoomWaterInitFull() == FALSE )
    {
        return FALSE;
    }
#endif

    /* Turn Off make, error */
    if( IsErrorType(ERROR_TYPE_HOT_MAKE) == TRUE )
    {
        return FALSE;
    }

#if 0
    // 탱크 탑 커버 OPEN이고 7초이후엔  히팅 중지
    if( GetServiceCheckTankOpen() == TRUE )
    {
        if( GetServiceOpenTime() >= TANK_OPEN_LIMIT_TIME )
        {
            return FALSE;
        }
    }
#endif

    // HEALTH 동작 중 ( 살균/배수 )
    if( IsStartHealth() == TRUE )
    {
        return FALSE;
    }

    // 냉수 청소 모드
    if( GetTankCleanOffLoad() == TRUE )
    {
        return FALSE;
    }

    return TRUE;  // Yes, valid
}

static TEMP_T GetTargetOnTemp(U8 altitude)
{
    if( altitude >= ALTITUDE_LEVEL_NUM )
    {
        altitude = 0;
    }

    // Smart 미사용 절전
    if( GetUnusedSavingStatus() == TRUE )
    {
        return OnTempUnsuedSavingList[altitude];
    }

    // 절전 
    if( GetSavingStatus() == STATUS_SLEEP )
    {
        return OnTempPowerSavingList[altitude];
    }

    return OnTempList[altitude];
}

static TEMP_T GetTargetOffTemp(U8 altitude)
{
    if( altitude >= ALTITUDE_LEVEL_NUM )
    {
        altitude = 0;
    }

    // Smart 미사용 절전
    if( GetUnusedSavingStatus() == TRUE )
    {
        return OffTempUnusedSavingList[altitude];
    }

    // 절전 
    if( GetSavingStatus() == STATUS_SLEEP )
    {
        return OffTempPowerSavingList[altitude];
    }

    return OffTempList[altitude];
}

TEMP_T GetHotWaterTargetOffTemp(void)
{
    return GetTargetOffTemp(Hot.Altitude);
}

static U8 CheckLevelOnHeatingAtTankOpen(void)
{
    static U8 prevStatus = LEVEL_LOW;
    U8 curStatus;

    // Check condi, detect low level
    curStatus = GetTankLevel( LEVEL_ID_ROOM );
    if( prevStatus != curStatus )
    {
        if( GetServiceCheckTankOpen() == TRUE )
        {
            // 저수위 -> 저수위가 아닌 상태로 전환되면 지연시간 등록
            if( prevStatus == LEVEL_LOW 
                    && curStatus != LEVEL_LOW )
            {
                Hot.MakeDelayTankOpen = WAIT_HEATING_TIME_TANK_OPEN;
            }
        }
        prevStatus = curStatus;
    }

    // Counting delay..
    if( Hot.MakeDelayTankOpen != 0 )
    {
        Hot.MakeDelayTankOpen--;
        return FALSE;   // Wait Heating..
    }

    return TRUE;  // Ok heating..
}

// 온수 가열 조건 검사 ( 수위 )
// 가열 가능 조건이면 TRUE 반환, 그렇지 않으면 FALSE
static U8 CheckLevelOnHeating(void)
{
    static U8 prevStatus = LEVEL_LOW;
    U8 curStatus;


    // Check condi, detect low level
    curStatus = GetTankLevel( LEVEL_ID_ROOM );
    if( prevStatus != curStatus )
    {
        // 저수위 -> 저위가 아닌 상태로 전환되면 히터 OFF
        if( prevStatus == LEVEL_LOW 
                && curStatus != LEVEL_LOW )
        {
            Hot.MakeDelay = WAIT_HEATING_TIME;
        }
        prevStatus = curStatus;
    }

    // Check condi, init full 
    if( GetRoomWaterInitFull() == FALSE )
    {
        Hot.MakeDelay = WAIT_HEATING_TIME;

        return FALSE;
    }

    // Counting delay..
    if( Hot.MakeDelay != 0 )
    {
        Hot.MakeDelay--;
        Hot.Make = FALSE;

        return FALSE;   // Wait Heating..
    }

    return TRUE;    // Ok heating..
}

static void UpdateTime(void)
{
    /* 전원 Reset 후, 대기 시간 */
    // 온도 센서 에러인 경우, 순간적으로 히터를 ON/OFF 제어함
    if( Hot.InitWaitTime != 0 )
    {
        Hot.InitWaitTime--;
    }
}

void MakeHotWater(void)
{
    UpdateTime();

    // Get Target On , Target Off, Current Temp 
    Hot.TempTargetOn  = GetTargetOnTemp(Hot.Altitude);
    Hot.TempTargetOff = GetTargetOffTemp(Hot.Altitude);
    Hot.TempCurrent = GetTemp( TEMP_ID_HOT_WATER );

#if 0
    /* 정수 최초 만수위 후, 5분 지연 시간*/

    if( CheckLevelOnHeating() == FALSE )
    {
        Hot.Make = FALSE;
        return ;
    }
#endif

    /* 탱크 커버가 열리 상태에서는 정수 저수위에서 저수위가 아닌 상태가 되었을 때엔 5분 지연 후 히팅 되도록 함 */
    if( CheckLevelOnHeatingAtTankOpen() == FALSE )
    {
        Hot.Make = FALSE;
        return ;
    }

    if( IsValidMake() == FALSE )
    {
        Hot.Make = FALSE;
        return ;
    }

    // Check Turn Off 
    if( Hot.Make == TRUE )
    {
        if( Hot.TempTargetOff <= Hot.TempCurrent )
        {
            Hot.Make = FALSE;
        }
    }
    // Check Turn On 
    else
    {
        if( Hot.TempTargetOn >= Hot.TempCurrent )
        {
            Hot.Make = TRUE;
        }
    }

    /* Protection, OVER-HEAINTG */
    if( Hot.TempCurrent >= PROTECT_HOT_TEMP )
    {
        Hot.Make = FALSE;
    }
}
