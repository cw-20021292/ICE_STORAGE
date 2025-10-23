/**
 * File : process_uv.c
 *
 * Process Uv
*/
#include "prj_type.h"
#include "process_uv.h"
#include "uv.h"
#include "util.h"
#include "valve.h"

#include "ice.h"
#include "ice_full.h"
#include "ice_door.h"
#include "ice_tray.h"
#include "water_out.h"
#include "input.h"
#include "api_uv.h"
#include "power_saving.h"
#include "health.h"
#include "drain.h"

// ON/OFF TIME @100ms
#define UV_3_SEC           (30UL)         // 3sec @100ms
#define UV_7_SEC           (70UL)         // 7sec @100ms
#define UV_2_MIN           (1200UL)       // 2 min * * 60 sec 
#define UV_15_MIN          (9000UL)       // 15 min * * 60 sec 
#define UV_30_MIN          (18000UL)      // 30 min * * 60 sec 
#define UV_45_MIN          (27000UL)      // 45 min * * 60 sec 
#define UV_1_HOUR          (36000UL)      // 1 hour * 60min * 60sec
#define UV_2_HOUR          (72000UL)      // 2 hour * 60min * 60sec
#define UV_4_HOUR          (144000UL)     // 4 hour * 60min * 60sec
#define UV_6_HOUR          (216000UL)     // 6 hour * 60min * 60sec
#define UV_24_HOUR         (864990UL)     // 24 hour * 60min * 60sec

// 일일 최대 동작 횟수 
//#define LIMIT_COUNT_ICE_DOOR      3
//#define LIMIT_COUNT_WATER_OUT     3
//#define LIMIT_COUNT_ICE_TANK      3
//#define LIMIT_COUNT_DRAIN_TANK    3
#define LIMIT_COUNT_ICE_TRAY      6

typedef struct _uv_
{
    U32 ResetTimer;

    U32 OutRegularTimer;    // 추출구 정기 살균(얼음/물)
    U32 IceRegularTimer;    // 아이스 탱크
    U32 DrainRegularTimer;  // 드레인  탱크
}SUv_T;

SUv_T Uv;

static void InitUvLimitCount(void);
void InitUv(void)
{
    Uv.ResetTimer = UV_24_HOUR;
    Uv.OutRegularTimer = UV_6_HOUR;
    Uv.IceRegularTimer = UV_6_HOUR;
    Uv.DrainRegularTimer = UV_6_HOUR; 

    // CONFIG UV LIMITED
    SetLimitConfUvId(UV_STER_ICE_DOOR,  UNLIMITED);
    SetLimitConfUvId(UV_STER_WATER_OUT, UNLIMITED);
    SetLimitConfUvId(UV_STER_ICE_TANK,  UNLIMITED);
    SetLimitConfUvId(UV_STER_DRAIN_TANK,  UNLIMITED);
    SetLimitConfUvId(UV_STER_ICE_TRAY,  LIMITED);

    SetLimitConfUvId(UV_STER_ICE_DOOR_SHOT,  UNLIMITED);
    SetLimitConfUvId(UV_STER_WATER_OUT_SHOT, UNLIMITED);
    SetLimitConfUvId(UV_STER_ICE_TRAY_SHOT,  UNLIMITED);

    // CONFIG UV LIMITED NUM
    InitUvLimitCount();
}

static void InitUvLimitCount(void)
{
    //SetLimitCountUvId( UV_STER_ICE_DOOR,  LIMIT_COUNT_ICE_DOOR );
    //SetLimitCountUvId( UV_STER_WATER_OUT, LIMIT_COUNT_WATER_OUT );
    //SetLimitCountUvId( UV_STER_ICE_TANK,  LIMIT_COUNT_ICE_TANK );
    //SetLimitCountUvId( UV_STER_DRAIN_TANK,  LIMIT_COUNT_DRAIN_TANK );
    SetLimitCountUvId(UV_STER_ICE_TRAY,  LIMIT_COUNT_ICE_TRAY);
}

static void ResetLimitCount(void)
{
    if( Uv.ResetTimer != 0 )
    {
        Uv.ResetTimer--;
    }
    else
    {
        Uv.ResetTimer = UV_24_HOUR;

        InitUvLimitCount();
    }
}


typedef struct _uv_ctrl_
{
    SUvSter_T Uv;
    U32 OffTime;
    U32 OnTime;

    U8 PrevStatus;
    U8 CurrentStatus;
    U8 TriggerStatus;
    U8 (*GetStatus)(void);
} SUvCtrl_T;

static U8 IsWaterOut(void);
static U8 IsIceOut(void);
SUvCtrl_T    CtrlList[] = 
{
    { UV_STER_WATER_OUT_SHOT, UV_3_SEC, UV_7_SEC,  0, 0, FALSE, IsWaterOut },
    { UV_STER_ICE_DOOR_SHOT,  UV_3_SEC, UV_7_SEC,  0, 0, FALSE, IsIceOut },
};
#define SZ_CTRL_LIST    ( sizeof(CtrlList)/sizeof(SUvCtrl_T) )


void CheckUv(void)
{
    U8 i;
    SUvCtrl_T *pItem;

    for( i=0; i<SZ_CTRL_LIST; i++ )
    {
        pItem = &CtrlList[ i ];

        pItem->CurrentStatus = pItem->GetStatus();
        if( pItem->PrevStatus != pItem->CurrentStatus )
        {
            if( pItem->CurrentStatus == pItem->TriggerStatus )
            {
                TurnOnUvId(pItem->Uv, pItem->OffTime, pItem->OnTime);
            }
            else
            {
                TurnOffUvId(pItem->Uv);
            }

            pItem->PrevStatus = pItem->CurrentStatus;
        }
    }
}

static U8 IsWaterOut(void)
{
    if( GetWaterOut() == TRUE )
    {
        if( IsOpenValve( VALVE_ROOM_OUT ) == TRUE 
                || IsOpenValve( VALVE_COLD_CIRCULATE ) == TRUE 
                || IsOpenValve( VALVE_HOT_OUT ) == TRUE )
        {
            return TRUE;
        }
    }

    return FALSE;
}

static U8 IsIceOut(void)
{
    return IsOpenIceDoor();
}

static U8 IsDrainWater(void)
{
    U8 lvlDrain = FALSE;

    lvlDrain = GetDrainLevel();

    if( lvlDrain == LEVEL_LOW )
    {
        return TRUE;
    }

    return TRUE;
}

// 얼음 추출, 물 추출구 정기 살균
static void CheckRegularOutUv(void)
{
    if( Uv.OutRegularTimer != 0 )
    {
        Uv.OutRegularTimer--;
    }
    else
    {
        Uv.OutRegularTimer = UV_6_HOUR; //+ UV_45_MIN;

        TurnOnUvId(UV_STER_WATER_OUT, 0, UV_1_HOUR);
        TurnOnUvId(UV_STER_ICE_DOOR,  0, UV_45_MIN);
    }
}

// 얼음 탱크 정기 살균
static void CheckRegularIceUv(void)
{
    if( Uv.IceRegularTimer != 0 )
    {
        Uv.IceRegularTimer--;
    }
    else
    {
        Uv.IceRegularTimer = UV_6_HOUR + UV_2_HOUR;

        TurnOnUvId(UV_STER_ICE_TANK, 0, UV_2_HOUR);
    }
}

/// 드레인  탱크 정기 살균
static void CheckRegularDrainUv(void)
{
    if( Uv.DrainRegularTimer != 0 )
    {
        Uv.DrainRegularTimer--;
    }
    else
    {
        Uv.DrainRegularTimer = UV_6_HOUR + UV_2_HOUR;

        TurnOnUvId(UV_STER_DRAIN_TANK, 0, UV_2_HOUR);
    }
}

// 아이스 트레이 정기 살균
// 최초 만빙시, 1시간 살균 동작
// 아이스 트레이 위치가 이동하면 취소
#if 0
static void CheckRegularIceTrayUv(void)
{
    static U8 mu8CheckAction = FALSE;


    // 취침 동작 중이 아니면 만빙될 떄까지 대기 했다가 동작
    mu8CheckAction = GetAlaramByNoneRTC( mu8CheckAction );
    if( mu8CheckAction == TRUE )
    {
        if( GetSavingStatus() == TRUE )
        {
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                mu8CheckAction = FALSE;
                TurnOnUvId( UV_ICE_TRAY, 0, UV_1_HOUR );
            }
        }
        else
        {
            if( GetIceFullStatus() == TRUE )
            {
                mu8CheckAction = FALSE;
                TurnOnUvId( UV_ICE_TRAY, 0, UV_1_HOUR );
            }
        }
    }

    // 트레이가 탈빙 위치가 아니면 중지
    if( GetIceTrayStatus() != ICE_TRAY_DEICING )
    {
        TurnOffUvId( UV_ICE_TRAY );
    }
}
#else
// 아이스 트레이 정기 살균
// 만빙시, 1시간 살균 동작
// 아이스 트레이 위치가 이동하면 취소
static void CheckRegularIceTrayUv(void)
{
    static U8 prev = FALSE;
    U8 cur;

    cur = GetIceFullStatus();
    if( prev != cur )
    {
        prev = cur;
        if( cur == TRUE )
        {
            if( GetIceTrayStatus() == ICE_TRAY_DEICING )
            {
                TurnOnUvId(UV_STER_ICE_TRAY, 0, UV_1_HOUR);
            }
        }
    }

    // 트레이가 탈빙 위치가 아니면 중지
    if( GetIceTrayStatus() != ICE_TRAY_DEICING )
    {
        TurnOffUvId(UV_STER_ICE_TRAY);
    }
}
#endif

static void CheckPauseUv(void)
{
    U8 pause = FALSE;

    // 1. ICE TANK
    if( IsIceOut() == TRUE )
    {
        pause = TRUE;
    }

    if( GetCurrentInputVal(INPUT_ID_TANK_OPEN_UV) == HIGH )
    {
        pause = TRUE;
    }

    SetPauseUvId(UV_STER_ICE_TANK, pause);
    SetPauseUvId(UV_STER_ICE_TRAY, pause);
    SetPauseUvId(UV_STER_ICE_TRAY_SHOT, pause);


    // 2. WATER OUT 
    if( IsWaterOut() == TRUE )
    {
        SetPauseUvId(UV_STER_WATER_OUT, TRUE);
    }
    else
    {
        SetPauseUvId(UV_STER_WATER_OUT, FALSE);
    }

    // 3. ICE OUT 
    if( IsIceOut() == TRUE )
    {
        SetPauseUvId(UV_STER_ICE_DOOR, TRUE);
    }
    else
    {
        SetPauseUvId(UV_STER_ICE_DOOR, FALSE);
    }

    // 4. DRIAN TANK 
    if( IsDrainWater() == FALSE )
    {
        SetPauseUvId(UV_STER_DRAIN_TANK, TRUE);
    }
    else
    {
        SetPauseUvId(UV_STER_DRAIN_TANK, FALSE);
    }
}

static void CheckConfigUv(void)
{
    if( GetIceConfigMake() == FALSE )
    {
        SetConfigUvId(UV_STER_ICE_TANK, FALSE);
        SetConfigUvId(UV_STER_ICE_TRAY, FALSE);
        SetConfigUvId(UV_STER_ICE_TRAY_SHOT, FALSE);
        SetConfigUvId(UV_STER_ICE_DOOR, FALSE);
    }
    else
    {
        SetConfigUvId(UV_STER_ICE_TANK, TRUE);
        SetConfigUvId(UV_STER_ICE_TRAY, TRUE);
        SetConfigUvId(UV_STER_ICE_TRAY_SHOT,  TRUE);
        SetConfigUvId(UV_STER_ICE_DOOR, TRUE);
    }
}

/////////////////////////////////////////////////////////////////////////
// UV 수동 제어 테스트 코드 
////////////////////////////////////////////////////////////////////////
#if( CONFIG_TEST_UV_MANUAL == 1 )
U8 dbg_uv_ice_door  = 0;        
U8 dbg_uv_water_out = 0;
U8 dbg_uv_ice_tray  = 0;
U8 dbg_uv_ice_tank  = 0;
U8 dbg_uv_drain_tank = 0;
static void TestUvManual(void)
{
    if( dbg_uv_ice_door != 0 )
    {
        API_TurnOnUv(UV_ID_ICE_DOOR);
    }
    else
    {
        API_TurnOffUv(UV_ID_ICE_DOOR);
    }

    if( dbg_uv_water_out != 0 )
    {
        API_TurnOnUv(UV_ID_WATER_OUT);
    }
    else
    {
        API_TurnOffUv(UV_ID_WATER_OUT);
    }

    if( dbg_uv_ice_tray != 0 )
    {
        API_TurnOnUv(UV_ID_ICE_TRAY);
    }
    else
    {
        API_TurnOffUv(UV_ID_ICE_TRAY);
    }

    if( dbg_uv_ice_tank != 0 )
    {
        API_TurnOnUv(UV_ID_ICE_TANK);
    }
    else
    {
        API_TurnOffUv(UV_ID_ICE_TANK);
    }

    if( dbg_uv_drain_tank != 0 )
    {
        API_TurnOnUv(UV_ID_DRAIN_TANK);
    }
    else
    {
        API_TurnOffUv(UV_ID_DRAIN_TANK);
    }
}
#endif

void ProcessUv(void)
{
#if( CONFIG_TEST_UV_MANUAL == 1 )
    TestUvManual();
#else

    CheckConfigUv();
    CheckUv();
    CheckRegularOutUv();
    CheckRegularIceUv();
    CheckRegularDrainUv();
    CheckRegularIceTrayUv();
    CheckPauseUv();

    ControlUv();

    ResetLimitCount();
#endif
}
