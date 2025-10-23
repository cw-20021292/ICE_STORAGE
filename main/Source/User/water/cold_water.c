#include "cold_water.h"
#include "room_water.h"
#include "level.h"
#include "temp.h"
#include "valve.h"
#include "error.h"
#include "health.h"
#include "drain_water.h"
#include "ster.h"
#include "power_saving.h"
#include "comp.h"
#include "gas_switch_valve.h"
#include "eeprom.h"
#include "front.h"
#include "tank_clean.h"
#include "water_out.h"
#include "smart_saving.h"
#include "ice_water_out.h"
#include "ice.h"

/* TARGET OFF TEMP - PROTECTION SUB-COOL */
#define PROTECT_COLD_TEMP   4.0f


#define COLD_ON_SAVING_TEMP      10.0f
/* TARGET TEMP - Power Saving */
static const TEMP_T OnTempPowerSaving = COLD_ON_SAVING_TEMP;
/* TARGET TEMP - Power Saving */
static const TEMP_T OnTempUnusedPowerSaving = COLD_ON_SAVING_TEMP;

// Starting Mode
#define MODE_INIT_STARTING      0
#define MODE_RESTARTING         1
#define TEMP_STARTING           20.0f
#define ON_DELAY_TIME           10      // 10sec @1sec

// Target Rps by Cold Temp Region...
#define REGION_COLD_A           0
#define REGION_COLD_B           1

// COLD MAKE TABLE 
#define COLD_MAKE_TABLE_A       0       // COLD TEMP UNDER 20'C
#define COLD_MAKE_TABLE_B       1       // COLD TEMP OVER 20'C
#define COLD_MAKE_TABLE_NUM     2

// INTI STARTING TEMP ( TEMP COLD WATER )
#define TEMP_INIT_STARTING      20.0f   // Cold water temp, for init starting....

// TEMP REGION ( TEMP AMBIENT )
#define REGION_AMBIENT_A        0       // Ambient temp =< 10
#define REGION_AMBIENT_B        1       // Ambient temp =< 20
#define REGION_AMBIENT_C        2       // Ambient temp =< 30
#define REGION_AMBIENT_D        3       // Ambient temp > 30
#define REGION_NUM              4

// EXTRA MAKE TIME @1sec
#define EXTRA_MAKE_TIME_0MIN    0U         // 0sec
#define EXTRA_MAKE_TIME_2MIN    120U       // 2min = 2 x 60sec 
#define EXTRA_MAKE_TIME_5MIN    300U       // 5min = 5 x 60sec 
#define EXTRA_MAKE_TIME_8MIN    480U       // 8min = 8 x 60sec
#define EXTRA_MAKE_TIME_10MIN   600U       // 10min = 10 x 60sec   
#define EXTRA_MAKE_TIME_12MIN   720U       // 12min = 12 * 60sec
#define EXTRA_MAKE_TIME_15MIN   900U       // 15min = 15 x 60sec
#define EXTRA_MAKE_TIME_20MIN   1200U      // 20min = 20 x 60sec
#define EXTRA_MAKE_TIME_25MIN   1500U      // 25min = 25 x 60sec

// EXTRA MAKE COLD TEMP
#define TEMP_ON_7D5             7.5f
#define TEMP_ON_6D5             6.5f
#define TEMP_OFF_5D8            5.8f
#define TEMP_OFF_6D0            6.0f

/* COLD IN DELAY TIME, WHEN COLD TANK LEVEL IS RELEASED HIGH LEVEL AND COLD OUT */
//#define COLD_TANK_FEED_TIME 1200U /* 2min @100ms */
#define COLD_TANK_FEED_TIME 1800U /* 3min @100ms */

typedef struct _cold_water_make_table_
{
    U8      RegionAmbi;

    TEMP_T  TempOn;
    TEMP_T  TempOff;

    TEMP_T  TempCompRps;        // cold temp 
    RPS_T   RpsA;               // cold temp lower region comp rps
    RPS_T   RpsB;               // cold temp upper region comp rps

    U16     ExtraMakeTime;      // @1sec

    U8      FanOnoff;           // Fan On Off
} ColdMakeTable_T;

static const ColdMakeTable_T     ColdMakeTableList[ COLD_MAKE_TABLE_NUM ][ REGION_NUM ] = 
{
    /* COLD WATER >= TEMP_INIT_STARTING */
    {
        {   REGION_AMBIENT_A, TEMP_ON_7D5,   TEMP_OFF_6D0,   0.0f,   35,   35,    EXTRA_MAKE_TIME_0MIN,  OFF },
        {   REGION_AMBIENT_B, TEMP_ON_7D5,   TEMP_OFF_6D0,   0.0f,   35,   35,    EXTRA_MAKE_TIME_5MIN,  OFF },
        {   REGION_AMBIENT_C, TEMP_ON_7D5,   TEMP_OFF_6D0,   10.0f,  35,   35,    EXTRA_MAKE_TIME_10MIN, OFF },
        {   REGION_AMBIENT_D, TEMP_ON_7D5,   TEMP_OFF_6D0,   10.0f,  35,   35,    EXTRA_MAKE_TIME_10MIN, ON  }
    },

    /* COLD WATER < TEMP_INIT_STARTING */
    {
        {   REGION_AMBIENT_A, TEMP_ON_7D5,   TEMP_OFF_6D0,   0.0f,   35,   35,    EXTRA_MAKE_TIME_0MIN,  OFF },
        {   REGION_AMBIENT_B, TEMP_ON_7D5,   TEMP_OFF_6D0,   0.0f,   35,   35,    EXTRA_MAKE_TIME_0MIN,  OFF },
        {   REGION_AMBIENT_C, TEMP_ON_7D5,   TEMP_OFF_6D0,   6.0f,   35,   35,    EXTRA_MAKE_TIME_5MIN,  OFF },
        {   REGION_AMBIENT_D, TEMP_ON_7D5,   TEMP_OFF_6D0,   6.0f,   35,   35,    EXTRA_MAKE_TIME_5MIN,  ON  }
    }
};

ColdMakeTable_T *pColdMake = NULL;
ColdWater_T Cold;




static void UpdateColdMakeData(void);

void  InitColdWater(void)
{
    Cold.InitFull       = FALSE;
    Cold.Level          = LEVEL_LOW;
    Cold.ConfigMake     = TRUE;
    Cold.Make           = FALSE;
    Cold.Mode           = COLD_MODE_INIT;

    // Make
    Cold.StartingMode       = MODE_INIT_STARTING;
    Cold.StartingModeTime   = 0U;

    UpdateColdMakeData();
    Cold.RegionAmbi         = REGION_AMBIENT_C;         // ��� 24�� ����
    Cold.TempTargetOn   = TEMP_ON_7D5;
    Cold.TempTargetOff  = TEMP_OFF_6D0;
    Cold.TempCurrent    = GetTemp( TEMP_ID_COLD_WATER );

    Cold.RegionCold         = REGION_COLD_A;
    Cold.TargetRps          = 45;

    Cold.FeedTime       = COLD_TANK_FEED_TIME;
    Cold.ExtraFeedTime  = EXTRA_FEED_TIME;

    Cold.TempLevel      = 0U;

    Cold.ExtraMake      = FALSE;
    Cold.ExtraMakeTime  = EXTRA_MAKE_TIME_0MIN;

    Cold.FanOnOff = OFF;
}

void  GetColdWaterData(ColdWater_T *pData)
{
    MEMCPY( (void __FAR *)pData, (const void __FAR *)&Cold, sizeof( ColdWater_T ));
}

void  SetColdWaterInitFull(U8 mu8Val )
{
    Cold.InitFull = mu8Val;
}

U8    GetColdWaterInitFull(void)
{
    return Cold.InitFull;
}


void SetColdWaterLevel(U8 val)
{
    Cold.Level = val;
}

U8 GetColdWaterLevel(void)
{
    return Cold.Level;
}

void  SetColdWaterConfigMake(U8 mu8Val )
{
    if( mu8Val != TRUE && mu8Val != FALSE )
    {
        return ;
    }

    Cold.ConfigMake = mu8Val;

    SaveEepromId( EEP_ID_CONF_MAKE_COLD );
}

U8 GetColdWaterConfigMake(void)
{
    return Cold.ConfigMake;
}

void SetColdWaterMake(U8 val )
{
    Cold.Make = val;
}

U8 GetColdWaterMake(void)
{
    return Cold.Make;
}

// �ü� �Լ� ���� �ð�
void  SetColdWaterFeedTime(U16 mu16Time )
{
    Cold.FeedTime = mu16Time;
}

U16    GetColdWaterFeedTime(void)
{
    return Cold.FeedTime;
}

void SetColdWaterMakeMode(U8 mu8Mode )
{
    Cold.Mode |= mu8Mode;
}

void ClearColdWaterMakeMode(U8 mu8Mode )
{
    Cold.Mode &= ~mu8Mode;
}

U8 IsSetColdWaterMakeMode(U8 mu8Mode )
{
    if( (Cold.Mode & mu8Mode ) == mu8Mode )
    {
        return TRUE;
    }

    return FALSE;
}

U8 GetColdWaterMakeMode(void )
{
    return Cold.Mode;
}

U8 GetColdWaterMakeFan(void)
{
    return Cold.FanOnOff;
}

RPS_T GetColdWaterMakeCompRps(void)
{
    return Cold.TargetRps;
}

// �ü� �Լ� ���� �ð�
void  SetColdWaterExtraMakeTime(U16 mu16Time )
{
    Cold.ExtraMakeTime = mu16Time;
}

U16   GetColdWaterExtraMakeTime(void)
{
    return Cold.ExtraMakeTime;
}



/*
 * �ü� ��ũ ���� ����
 */
#if( CONFIG_USE_ICING_MAX_COUNT === 1 )
#include "ice.h"
#endif
void  ControlColdWaterLevel(void)
{
    U8 mu8RoomLevel;

    /* Update Watere Level */
    Cold.Level = GetTankLevel( LEVEL_ID_COLD );

    /* ���� �ϰ��� 10�� ī��Ʈ �ʱ�ȭ �� �ٷ� ������ */
    if( ((GetWaterOut() == TRUE) && (GetWaterOutSelect() == SEL_WATER_COLD))
        || (GetIceWaterOut() == TRUE) )
    {
        ClearIcingCount();
    }
    
    //if( Cold.Level == LEVEL_HIGH )
    if( IsDetectTankLevel( LEVEL_ID_COLD, LEVEL_DETECT_HIGH ) == TRUE )
    {
        SetColdWaterInitFull( TRUE );
        Cold.FeedTime = COLD_TANK_FEED_TIME;

        ClearWaterOutAccumulateTime(SEL_WATER_COLD);
        ClearIceWaterOutAccumulateTime();
    }
    else
    {
        Cold.ExtraFeedTime = EXTRA_FEED_TIME;
    }

    // ��� ������ FULL Ÿ���� ������ �Ѵ�...
    if( IsStartDrainWater() == TRUE 
            && IsDrainWaterHalf() == FALSE )
    {
        return ;
    }

#if 0
    // ���
    if( IsStartSter() == TRUE )
    {
        return ;
    }
#endif

    /* ERROR */
    if( IsError(ERR_ID_COLD_LOW_LEVEL) == TRUE )
    {
        CloseValve( VALVE_COLD_IN );
        return ;
    }

    // ���� ��ũ ������ ���, �Լ� ����
#if 0
    mu8RoomLevel = GetRoomWaterLevel();
    if( mu8RoomLevel != LEVEL_LOW 
            && mu8RoomLevel != LEVEL_MID 
            && mu8RoomLevel != LEVEL_HIGH )
    {
        CloseValve( VALVE_COLD_IN );
        return ;
    }
#endif
    // �ü��� �������϶�, ������ ������ �����̸� 
    if( GetRoomWaterLevel() == LEVEL_LOW 
            && GetColdWaterLevel() == LEVEL_LOW )
    {
        SetRoomWaterInitFull( FALSE );
        SetColdWaterInitFull( FALSE );
    }

    // ���� ��ũ �ʱ� �������� �ƴϸ�, �Լ� ����
    if( GetRoomWaterInitFull() == FALSE )
    {
        CloseValve( VALVE_COLD_IN );
        return ;
    }

#if( CONFIG_USE_ICING_MAX_COUNT === 1 )
    if( IsMaxIcingCount() == FALSE )
    {
        CloseValve( VALVE_COLD_IN );
        return ;
    }
#endif

    // �ü� ��ũ �ʱ� �������� �ƴ� ���, ���������� �Լ�
    if( GetColdWaterInitFull() == FALSE 
            && Cold.Level != LEVEL_HIGH )
    {
        OpenValve( VALVE_COLD_IN );
        return ;
    }

    // ���� �������� �ƴϸ� Feed Time�� 0���� �ʱ�ȭ ��Ų��.
    // �߼��� ���ǿ��� Feed Time���� ���� �������� �ȵǴ� ������ ���� �� �ֱ� �����̴�.
    if( Cold.InitFull == FALSE )
    {
        Cold.FeedTime = 0;
    }

    /* 1. ������ ��� �Լ� */
    if( Cold.Level == LEVEL_LOW )
    {
        OpenValve( VALVE_COLD_IN );
    }
    /* 2. �߼���, ���Լ� �ð� ���� �� �Լ� */
    else if( Cold.Level == LEVEL_MID && Cold.FeedTime == 0 )
    {
        OpenValve( VALVE_COLD_IN );
    }
    /* 3. ������ �Ǵ� ����, �Լ� ���� */
    else if( Cold.Level == LEVEL_HIGH )
    {
        // �ü� �߰� �Լ� 
        if( Cold.ExtraFeedTime == 0 )
        {
            if( IsOpenValve( VALVE_COLD_IN ) == TRUE )
            {
                CloseValve( VALVE_COLD_IN );
            }
        }
    }

    /* CHECK FEED TIME DELAY 
     *  -. ��ȯ ������ �����ϸ� �ü��� �����.
     *  -. �ü� ���� �ð��� ���� �ð� �̻��� �� ��쿡�� �ü��� �ٽ� �Լ� ��Ų��. 
     */
    if( Cold.Level <= LEVEL_MID )
    {
        if( IsOpenValve( VALVE_COLD_CIRCULATE ) == FALSE
            && IsOpenValve( VALVE_ICE_WATER_OUT ) == FALSE  )
        {
            if( Cold.FeedTime != 0 )
            {
                Cold.FeedTime--;
            }
        }
    }

    // ������ ���� ��, �߰� �Լ� �ð�
    if( Cold.Level == LEVEL_HIGH )
    {
        if( Cold.ExtraFeedTime != 0 )
        {
            Cold.ExtraFeedTime--;
        }
    }
}

static U8 IsValidMake(void)
{
    /* Turn Off make, dont want to make */
    if( GetIceConfigMake() == FALSE )
    {
        if( Cold.ConfigMake == FALSE )
        {
            return FALSE;
        }
    }

    /* Turn Off make, no water */
    if( Cold.Level == LEVEL_LOW )
    {
        return FALSE;
    }

    ///* ���� ��ȯ ��� */
    //if( IsGoingOnDrainWaterFull() == TRUE )
    //{
    //    return FALSE;
    //}

    /* �ü� ��ũ�� ���� �������� ���� ���� ��� */
    if( Cold.InitFull == FALSE )
    {
        return FALSE;
    }

    /* ����� ���� �ð� */
    if( IsExpiredCompProtectTime() == FALSE )
    {
        return FALSE;
    }

    /* ����� ��ȣ �ð� �ʰ� */
    if( IsOverCompProtectOnTime() == TRUE )
    {
        return FALSE;
    }
    
    /* Turn Off make, Error */
    if( IsErrorType(ERROR_TYPE_COLD_MAKE) == TRUE )
    {
        return FALSE;
    }

    // HEALTH ���� �� ( ���/��� )
    if( IsStartHealth() == TRUE )
    {
        return FALSE;
    }

    // �ü� û�� ���
    if( GetTankCleanOffLoad() == TRUE )
    {
        return FALSE;
    }

    return TRUE;  // Yes, valid
}

static TEMP_T GetTargetOnTemp(void)
{
    /* Smart �̻�� ���� */
    if( GetUnusedSavingStatus() == TRUE )
    {
        return OnTempUnusedPowerSaving;
    }

    // ����
    if( GetSavingStatus() == STATUS_SLEEP )
    {
        return OnTempPowerSaving;
    }
    

    return pColdMake->TempOn;
}

static TEMP_T GetTargetOffTemp(void)
{
    return pColdMake->TempOff;
}


// �ð� �߰� Ÿ�̸� decounter
static void UpdateExtraMakeTime(void)
{
    if( GetCompOnOff() == OFF )
    {
        return ;
    }

    if( GetGasSwitchStatus() != GAS_SWITCH_COLD )
    {
        return ;
    }

    if( Cold.ExtraMakeTime != 0 )
    {
        Cold.ExtraMakeTime--;
    }
}

// �߰� �ð� ���� Ȯ��
// TRUE ��ȯ�̸� �߰� �ð�
// FALSE �̸� �߰� �ð� ����.
static U8 IsExtraMake(void)
{
    // �ܱ� �µ� ���� ���� �߰� �ð� ����
    if( IsError(ERR_ID_TEMP_AMBIENT_2) == TRUE )
    {
        return FALSE;   // NO EXTRA MAKE
    }

    // �ܱ� �µ� ����ġ ���� �߰� �ð� ����
    if( pColdMake->ExtraMakeTime == EXTRA_MAKE_TIME_0MIN )
    {
        return FALSE; // NO EXTRA MAKE;
    }

    return TRUE;   // EXTRA MAKE
}

// Starting Mode ����
// �ð� ���� ����
static U8 CheckStartMake(void)
{
    U8 mu8Make = FALSE;

    // �ü� �µ� 20�� ���ǿ����� ��� �ð�
    if( Cold.TempCurrent >= TEMP_STARTING )
    {
        Cold.StartingMode = MODE_INIT_STARTING;
        mu8Make = TRUE;
    }
    else 
    {
        // �ü� �µ� 20�� ���� ���ǿ����� ���� ���� ��쿡��
        // 10�� ���� �� �ð� ���� ����.
        if( Cold.TempCurrent >= Cold.TempTargetOn )
        {
            // �ü� ���� �߿��� �ð� �缳��
            if( GetWaterOut() == TRUE 
                    && GetWaterOutSelect() == SEL_WATER_COLD )
            {
                Cold.StartingModeTime = ON_DELAY_TIME;  
            }

            // ��� ���� �ð��� ����Ǹ� �ð� ���� �� 
            if( Cold.StartingModeTime != 0 )
            {
                Cold.StartingModeTime--;
            }
            else
            {
                // ��⵿ ��带 ����
                Cold.StartingMode = MODE_RESTARTING;
                mu8Make = TRUE;
            }
        }
        else
        {
            Cold.StartingModeTime = ON_DELAY_TIME;
        }
    }

    return mu8Make;
}


static U8  CheckStopMake(void)
{
    U8 mu8Make = TRUE;      // �ð�


    if( Cold.TempTargetOff >= Cold.TempCurrent )
    {
        Cold.ExtraMake      = TRUE;
        Cold.ExtraMakeTime  = pColdMake->ExtraMakeTime;
        if( Cold.ExtraMakeTime == 0 )
        {
            // �߰� �ð� �ð��� �����Ǿ� ���� ������ OFF
            Cold.ExtraMake  = FALSE;    
            mu8Make         = FALSE;    
        }
    }

    return mu8Make;
}

// �ܱ� �µ� ���� ����
static void UpdateColdMakeData(void)
{
    TEMP_T  mTempAmbi;


    mTempAmbi = GetTemp( TEMP_ID_AMBIENT_2 );
    if( mTempAmbi <= 10.0f )
    {
        Cold.RegionAmbi = REGION_AMBIENT_A;
    }
    else if( mTempAmbi <= 20.0f )
    {
        Cold.RegionAmbi = REGION_AMBIENT_B;
    }
    else if( mTempAmbi <= 30.0f )
    {
        Cold.RegionAmbi = REGION_AMBIENT_C;
    }
    else 
    {
        Cold.RegionAmbi = REGION_AMBIENT_D;
    }


    pColdMake = &ColdMakeTableList[ Cold.StartingMode ][ Cold.RegionAmbi ];
}

// �ܱ� �µ��� ���� Fan ���� ���θ� ����
static U8 CheckFan(void)
{
    return pColdMake->FanOnoff;
}

// �ü� �µ��� ���� ��ǥ RPS�� ����
static RPS_T CheckCompRps(void)
{
    RPS_T   mTargetRps;
    TEMP_T  mTempA;             // Lower
    TEMP_T  mTempB;             // Upper


    // Set Cold Temp Region
    mTempA = pColdMake->TempCompRps;
    mTempB = pColdMake->TempCompRps + 0.5f;
    if( Cold.RegionCold == REGION_COLD_A )
    {
        if( mTempB < Cold.TempCurrent )
        {
            Cold.RegionCold = REGION_COLD_B;
        }
    }
    else
    {
        if( Cold.TempCurrent <= mTempA  )
        {
            Cold.RegionCold = REGION_COLD_A;
        }

    }

    // Set Target RPS
    if( Cold.RegionCold == REGION_COLD_A )
    {
        mTargetRps = pColdMake->RpsA;
    }
    else
    {
        mTargetRps = pColdMake->RpsB;
    }

    return mTargetRps;
}

// �ü� ���� ���� �˻�
void  MakeColdWater(void)
{
    U8 mu8Make = FALSE;


    UpdateExtraMakeTime();

    UpdateColdMakeData();

    // Get Current Make Status
    mu8Make = GetColdWaterMake();

    // Get Temperture
    Cold.TempTargetOn  = GetTargetOnTemp();
    Cold.TempTargetOff = GetTargetOffTemp();
    Cold.TempCurrent   = GetTemp( TEMP_ID_COLD_WATER );


    // ���� �߰� ���� ������ ���,
    // �µ� ���� ���ǿ����� FALSE �������� �˻��ؾ��Ѵ�. 
    if( Cold.ExtraMake == TRUE )
    {
         // �߰� �ð� ���̸�, �Ϲ� �ð� ������ OFF�� ����
        mu8Make = FALSE;
    }

    if( mu8Make == TRUE )
    {
        mu8Make = CheckStopMake();
    }
    else
    {
        // Check On
         mu8Make = CheckStartMake();
    }

    // Extra Make Cold 
    if( Cold.ExtraMake == TRUE )
    {
        if( Cold.ExtraMakeTime != 0 )
        {
            mu8Make = TRUE;
        }
        else
        {
            Cold.ExtraMake = FALSE;
        }
    }

    // Comp rps
    if( mu8Make == TRUE )
    {
        Cold.TargetRps = CheckCompRps();
        Cold.FanOnOff = CheckFan();
    }

    // Protectino Off
    if( Cold.TempCurrent <= PROTECT_COLD_TEMP )
    {
        mu8Make = FALSE;
        Cold.ExtraMakeTime = 0;
    }

    // Config Off
    if( IsValidMake() == FALSE )
    {
        mu8Make = FALSE;
        Cold.ExtraMakeTime = 0;
    }

    // Set new make status
    SetColdWaterMake( mu8Make );
}
