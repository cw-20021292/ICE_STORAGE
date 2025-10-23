#include "smart_check.h"

#include "hal_input.h"
#include "level.h"
#include "temp.h"
#include "error.h"
#include "water_out.h"
#include "service.h"
#include "monitoring.h"
#include "hal_adc.h"
#include "hot_water.h"
#include "drain_water.h"
#include "ster.h"
#include "eol.h"
#include "fct.h"
#include "ice_door.h"
#include "sound.h"
#include "err_temp.h"

#include "diagnosis.h"

#include "WIFI_Control.h"

#define LEVEL_UNDETECT      0U
#define LEVEL_DETECT        1U

#define DIAG_STATUS_NORMAL       0U
#define DIAG_STATUS_ERROR        1U

typedef enum _sc_status_
{
    SC_STATUS_NORMAL,
    SC_STATUS_START,
    SC_STATUS_GOING,
    SC_STATUS_COMPLETE,
    SC_STATUS_STOP,
    SC_STATUS_INVALID,
} SC_STATUS_T;

// @1min 1�ð� ( 1min * 60 = 1�ð� )
#define TIME_FOR_REQUEST (60)

typedef struct _smart_check_info_
{
    U8 Start;      // run , stop
    SC_STATUS_T Status;     // Normol, Start, Going, Complete, Stop, Invalid

    U16 ContinueCheckTime;
} SmartInfo_T;

SmartInfo_T SmartCheck;


void InitSmartCheck(void)
{
    SmartCheck.Start = FALSE;
    SmartCheck.Status = FALSE;

    SmartCheck.ContinueCheckTime = TIME_FOR_REQUEST;
}

// return false or true
U8 IsValidStartSmartCheck(void)
{
    if( IsStartDrainWater() == TRUE )
    {
        return FALSE;
    }
    
    if( IsStartSter() == TRUE )
    {
        return FALSE;
    }

    if( IsOpenIceDoor() == TRUE )
    {
        return FALSE;
    }

    if( GetWaterOut() == TRUE )
    {
        return FALSE;
    }

    if( GetEolStatus() == TRUE
        || GetFctStatus() == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

I8 GetIsValidStartSmartCheck(void)
{
    I8 m8IsValid = -1;
    static U16 PreIsValidStartCheck = FALSE;
    U8 CurrentIsValidStartCheck = FALSE;

    CurrentIsValidStartCheck = IsValidStartSmartCheck();

    if( PreIsValidStartCheck != CurrentIsValidStartCheck )
    {
        PreIsValidStartCheck = CurrentIsValidStartCheck;

        if( CurrentIsValidStartCheck == TRUE )
        {
            m8IsValid = TRUE;
        }
        else
        {
            m8IsValid = FALSE;
        }
    }

    return m8IsValid;
}

void StartSmartCheck(void)
{
    SmartCheck.Start = TRUE;
}

void StopSmartCheck(void)
{
    SmartCheck.Start = FALSE;
}

U8 GetSmartCheck(void)
{
    return SmartCheck.Start;
}

void SetSmartCheckStatus(U8 mu8Status)
{
    SmartCheck.Status = mu8Status;
}

U8 GetSmartCheckStatus(void)
{
    return SmartCheck.Status;
}

static I8 IsStartSmartCheck(void)
{
    I8 m8IsStart = -1;
    static U16 PreSmartCheckStart = FALSE;

    if( PreSmartCheckStart != SmartCheck.Start )
    {
        PreSmartCheckStart = SmartCheck.Start;
    
        if( SmartCheck.Start == TRUE )
        {
            m8IsStart = TRUE;
        }
        else
        {
            m8IsStart = FALSE;
        }

    }

    return m8IsStart;
}

I8 GetCompleteSmartCheck(void)
{
    I8 m8IsComplete = -1;
    static U16 PreCompleteStartCheck = FALSE;
    U8 CurrentCompleteStartCheck = FALSE;

    CurrentCompleteStartCheck = GetCompleteDiagnosis();

    if( PreCompleteStartCheck != CurrentCompleteStartCheck )
    {
        PreCompleteStartCheck = CurrentCompleteStartCheck;

        if( CurrentCompleteStartCheck == TRUE )
        {
            m8IsComplete = TRUE;
        }
        else
        {
            m8IsComplete = FALSE;
        }
    }

    return m8IsComplete;
}

void ControlSmartCheck(void)
{
    U8 IsValid = FALSE;
    U8 IsComplete = FALSE;
    U8 IsStart = FALSE;

    IsValid = GetIsValidStartSmartCheck();
    IsComplete = GetCompleteSmartCheck();
    IsStart = IsStartSmartCheck();

    

    if( IsValid == TRUE )
    {
        SmartCheck.Status = SC_STATUS_NORMAL;  
    }
    else if( IsValid == FALSE )
    {
        SmartCheck.Status = SC_STATUS_INVALID;
    }

    if( IsComplete == TRUE )
    {
        SmartCheck.Status = SC_STATUS_COMPLETE;
        WifiSendData(WIFI_DATA_EXAMINE);
    }

    if( IsStart == TRUE )
    {
        // 진단 Part Set
        StartDiagnosis();
        SmartCheck.Status = SC_STATUS_START;
    }
    else if( IsStart == FALSE )
    {
        // 진단 Part Clear
        StopDiagnosis();

        SetDiagnosisProgress(0, 0);

        if( SmartCheck.Status == SC_STATUS_START
            || SmartCheck.Status == SC_STATUS_GOING )
        {
            SmartCheck.Status = SC_STATUS_STOP;
            Sound( SOUND_CANCEL );
        }
    }   

    if( SmartCheck.Status == SC_STATUS_NORMAL)
    {
        if( SmartCheck.Start == TRUE )
        {
            SmartCheck.Start = FALSE;
            SetDiagnosisProgress(0, 0);
        }
    
        if( GetCompleteDiagnosis() == TRUE )
        {
            ClearCompleteDiagnosis();
        }
    }
}



void ControlContinueSmartCheck(void)
{
    // ?��?진단 �?
    if( IsStartDiagnosis() == TRUE )
    {
        SmartCheck.ContinueCheckTime = TIME_FOR_REQUEST;
        return;
    }

    if( SmartCheck.ContinueCheckTime != 0)
    {
        SmartCheck.ContinueCheckTime--;
        return;
    }
    else
    {
        SmartCheck.ContinueCheckTime = TIME_FOR_REQUEST;
    }
    
    WifiSendData(WIFI_DATA_PART);
}



static U16 SmartCheckRqst(void);
static U16 SmartCheckStatus(void);
static U16 SmartCheckProgress(void);
typedef struct _smart_check_status_
{
    E_API_A1010_T   Id;
    U16 Data;
    U16 (*pFun)(void);
} CheckStatusList_T;
CheckStatusList_T StatusList[] = 
{
    { WIFI_FUNC_0040_SMART_CHECK_RQST,      0,  SmartCheckRqst },
    { WIFI_FUNC_0041_SMART_CHECK_STATUS,    0,  SmartCheckStatus },
    { WIFI_FUNC_0042_SMART_CHECK_PROGRESS,  0,  SmartCheckProgress },
};

#define SZ_SMART_CHECK_STATUS_LIST     ( sizeof(StatusList) / sizeof(CheckStatusList_T))

static U8 FindIndexStatusId( E_API_A1010_T id, U8 *pIndex )
{
    U8 i = 0; 

    for( i = 0; i < SZ_SMART_CHECK_STATUS_LIST; i++ )
    {
        if( StatusList[ i ].Id == id )
        {
            if( pIndex != NULL )
            {
                *pIndex = i;
            }

            return TRUE;
        }   
    }
    
    return FALSE;
}

U16 GetSmartCheckStatusId( E_API_A1010_T id )
{
    U8 index = 0;
    U16 mu16Data = 0;

    if( FindIndexStatusId( id, &index ) == TRUE )
    {
        if( StatusList[ index ].pFun != NULL )
        {
            StatusList[ index ].Data = StatusList[ index ].pFun();
        }
        mu16Data = StatusList[ index ].Data;
    }

    return mu16Data;
}

static U16 SmartCheckRqst(void)
{
    U16 mu16Data = 0;

    if( SmartCheck.Start == TRUE )
    {
        mu16Data = 1;
    }

    return mu16Data;
}

static U16 SmartCheckStatus(void)
{
    U16 mu16Data = 0;

    mu16Data = SmartCheck.Status;
 
    if( SmartCheck.Status == SC_STATUS_START )
    {       
        SmartCheck.Status = SC_STATUS_GOING;
    }
    else if( SmartCheck.Status == SC_STATUS_COMPLETE
              || SmartCheck.Status == SC_STATUS_STOP )
    {
        SmartCheck.Status = SC_STATUS_NORMAL;
    }    
 
    return mu16Data;
}

static U16 SmartCheckProgress(void)
{
    U16 mu16Data = 0;

    mu16Data = GetDiagnosisProgress();

    return mu16Data;
}







// DATA TYPE
#define D_TYPE_DATA     0
#define D_TYPE_STATUS   1
typedef U16(*Action_T)(E_API_A1080_T  Id);
typedef struct _smart_check_item_
{
    E_API_A1080_T   Id;
    U16 Data;
    U16 SetData;
    Action_T    pFun;
} CheckItemList_T;

static U16 GetMonitoringLoad(E_API_A1080_T Id);
static U16 ColdHigh(E_API_A1080_T Id);
static U16 ColdLow(E_API_A1080_T Id);
static U16 PureOver(E_API_A1080_T Id);
static U16 PureHigh(E_API_A1080_T Id);
static U16 PureLow(E_API_A1080_T Id);
static U16 DrainHigh(E_API_A1080_T Id);
static U16 DrainLow(E_API_A1080_T Id);
static U16 ColdTemp_1(E_API_A1080_T Id);
static U16 AmbiTemp(E_API_A1080_T Id);
static U16 HotTemp(E_API_A1080_T Id);
static U16 SensorTankReed(E_API_A1080_T Id);
static U16 SensorLight(E_API_A1080_T Id);
static U16 IcePureTemp(E_API_A1080_T Id);
static U16 SensorTrayIcingSw(E_API_A1080_T Id);

CheckItemList_T ItemList[] = 
{
    { WIFI_PART_0000_VALVE_FEED,              INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },         
    { WIFI_PART_0001_VALVE_NOS,               INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },         
    { WIFI_PART_0002_VALVE_HOT_OUT,           INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0003_VALVE_PURE_OUT,          INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_000C_VALVE_COLD_IN,           INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_000D_VALVE_COLD_DRAIN,        INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0015_VALVE_ICE_TRAY_IN,       INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0016_VALVE_COLD_ROTATE,       INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0011_VALVE_STERILIZE,         INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0017_VALVE_EW_IN,             INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },

    { WIFI_PART_0101_LEVEL_COLD_HIGH,         INIT_DATA,  INIT_DATA,      ColdHigh            },
    { WIFI_PART_0103_LEVEL_COLD_LOW,          INIT_DATA,  INIT_DATA,      ColdLow             },
    { WIFI_PART_0105_LEVEL_PURE_OVER,         INIT_DATA,  INIT_DATA,      PureOver            },
    { WIFI_PART_0106_LEVEL_PURE_HIGH,         INIT_DATA,  INIT_DATA,      PureHigh            },
    { WIFI_PART_0108_LEVEL_PURE_LOW,          INIT_DATA,  INIT_DATA,      PureLow             },
    { WIFI_PART_010B_LEVEL_DRAIN_HIGH,        INIT_DATA,  INIT_DATA,      DrainHigh           },
    { WIFI_PART_010D_LEVEL_DRAIN_LOW,         INIT_DATA,  INIT_DATA,      DrainLow            },

    { WIFI_PART_0200_COLD_COMP,               INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0206_COLD_TEMP_1,             0,          0,              ColdTemp_1          },
    { WIFI_PART_0208_COLD_ROOM_TEMP,          0,          0,              AmbiTemp            },
    { WIFI_PART_0209_COLD_ROTATE_PUMP,        INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },

    { WIFI_PART_0300_HOT_TANK_HEATER,         INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0301_HOT_TEMP,                0,          0,              HotTemp             },

    { WIFI_PART_0409_SENSOR_TANK_REED,        INIT_DATA,  INIT_DATA,      SensorTankReed      },
    { WIFI_PART_040A_SENSOR_LIGHT,            INIT_DATA,  INIT_DATA,      SensorLight         },

    { WIFI_PART_0500_STER_CSI_MODULE,         INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0501_STER_UV_FAUCET,          INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0502_STER_UV_FAUCET_ICE,      INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0503_STER_UV_ICE_TANK,        INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0504_STER_UV_ICE_TRAY,        INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },

    { WIFI_PART_0800_ICE_PURE_TEMP,           0,          0,              IcePureTemp         },
    { WIFI_PART_0801_ICE_FULL_SENSOR,         INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0802_ICE_SWING_BAR,           INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0803_ICE_TRAY_AC,             INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0805_ICE_TRAY_SENSING_SW,     INIT_DATA,  INIT_DATA,      SensorTrayIcingSw   },
    { WIFI_PART_0806_ICE_MOTOR_OUT,           INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0807_ICE_DOOR_STEPMOTOR,      INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0808_ICE_COURSE_CHANGE_VV,    INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_0809_ICE_HOT_GAS_VV,          INIT_DATA,  INIT_DATA,      GetMonitoringLoad   },
    { WIFI_PART_080A_ICE_DRAIN_PUMP,          INIT_DATA,  INIT_DATA,      GetMonitoringLoad   }
};

#define SZ_SMART_CHECK_LIST     ( sizeof(ItemList) / sizeof(CheckItemList_T))


static U8 FindIndexId( E_API_A1080_T id, U8 *pIndex )
{
    U8 i = 0; 

    for( i = 0; i < SZ_SMART_CHECK_LIST; i++ )
    {
        if( ItemList[ i ].Id == id )
        {
            if( pIndex != NULL )
            {
                *pIndex = i;
            }

            return TRUE;
        }        
    }

    return FALSE;
}


U8 IsValidPartID( E_API_A1080_T id )
{
    return FindIndexId( id, NULL );
}

U16 GetSmartCheckDataId( E_API_A1080_T id )
{
    U8 index = 0;
    U16 mu16Data = 0;
    E_API_A1080_T   Id;

    if( FindIndexId( id, &index ) == TRUE )
    {
        if( ItemList[ index ].Data != ItemList[ index ].SetData )
        {
            mu16Data = ItemList[ index ].Data;
        }
        else
        {    
            Id = ItemList[ index ].Id;
            if( ItemList[ index ].pFun != NULL )
            {
                ItemList[ index ].Data = ItemList[ index ].pFun( Id );
            }
            mu16Data = ItemList[ index ].Data;
            ItemList[ index ].Data = ItemList[ index ].SetData;
        }
    }

    return mu16Data;
}

void SetSmartCheckDataId( E_API_A1080_T id, U16 mu16Data )
{
    U8 index = 0;

    if( FindIndexId( id, &index ) == TRUE )
    {
        ItemList[ index ].Data = mu16Data;
    }
}

static U16 GetMonitoringLoad( E_API_A1080_T   Id)
{
    U16 mData;

    mData = GetLoadCurrent( Id );
    SetLoadAdc( Id, 0 );
    SetLoadCurrent( Id, INIT_DATA );
    return mData;
}

static U16 ColdHigh(E_API_A1080_T   Id)
{
    if( GetTankLevel( LEVEL_ID_COLD ) == LEVEL_HIGH
            || GetTankLevel( LEVEL_ID_COLD ) == LEVEL_ERR_LOW )
    {
        return LEVEL_DETECT;   // 감�?
    }

    return LEVEL_UNDETECT;   // 미감지
}

static U16 ColdLow(E_API_A1080_T Id)
{
    if( GetTankLevel( LEVEL_ID_COLD ) == LEVEL_MID
            || GetTankLevel( LEVEL_ID_COLD ) == LEVEL_HIGH )
    {
        return LEVEL_DETECT;   // 감�?
    }

    return LEVEL_UNDETECT;   // 미감지
}

static U16 PureOver(E_API_A1080_T Id)
{
    if( GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_OVF
            || GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_ERR_OVF_LOW
            || GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_ERR_HIGH
            || GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_ERR_COMPLEX )
    {
        return LEVEL_DETECT;   // 감�?
    }

    return LEVEL_UNDETECT;   // 미감지
}

static U16 PureHigh(E_API_A1080_T Id)
{
    if( GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_HIGH
            || GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_ERR_LOW
            || GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_ERR_OVF_LOW
            || GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_OVF )
    {
        return LEVEL_DETECT;   // 감�?
    }

    return LEVEL_UNDETECT;   // 미감지
}

static U16 PureLow(E_API_A1080_T Id)
{
    if( GetTankLevel( LEVEL_ID_ROOM ) == LEVEL_MID
            || GetTankLevel( LEVEL_ID_ROOM ) == LEVEL_HIGH
            || GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_ERR_HIGH
            || GetTankLevel( LEVEL_ID_ROOM ) == HAL_LEVEL_OVF )
    {
        return LEVEL_DETECT;   // 감�?
    }

    return 0;   // 미감지
}

static U16 DrainHigh(E_API_A1080_T Id)
{
    if( GetTankLevel( LEVEL_ID_DRAIN ) == LEVEL_HIGH
            || GetTankLevel( LEVEL_ID_DRAIN ) == LEVEL_ERR_LOW )
    {
        return LEVEL_DETECT;   // 감�?
    }

    return LEVEL_UNDETECT;   // 미감지
}

static U16 DrainLow(E_API_A1080_T Id)
{
    if( GetTankLevel( LEVEL_ID_DRAIN ) == LEVEL_MID
            || GetTankLevel( LEVEL_ID_DRAIN ) == LEVEL_HIGH )
    {
        return LEVEL_DETECT;   // 감�?
    }

    return LEVEL_UNDETECT;   // 미감지
}

#define TEMP_OPEN_SHORT     255
static U16 GetTempDiag(U8 adcId, U8 tempId)
{
    U16 mu16Adc;

    mu16Adc = HAL_GetAdcValue( adcId );
    if( GetTempErrorType( mu16Adc ) != STATUS_NORMAL )
    {
        return TEMP_OPEN_SHORT;
    }

    return GetTemp( tempId );
}

static U16 ColdTemp_1(E_API_A1080_T Id)
{
    return GetTempDiag( ANI_TEMP_COLD_WATER, TEMP_ID_COLD_WATER );
}

static U16 AmbiTemp(E_API_A1080_T Id)
{
    return GetTempDiag( ANI_TEMP_AMBIENT, TEMP_ID_AMBIENT );
}

static U16 HotTemp(E_API_A1080_T Id)
{
    return GetTempDiag( ANI_TEMP_HOT_WATER, TEMP_ID_HOT_WATER );
}

static U16 SensorTankReed(E_API_A1080_T Id)
{
    if( HAL_GetInputValue( IN_TANK_OPEN ) == FALSE )
    {
        return DIAG_STATUS_NORMAL;
    }

    return DIAG_STATUS_ERROR;
}

static U16 SensorLight(E_API_A1080_T Id)
{
    return HAL_GetAdcValue(ANI_SAVING_PHOTO);
}

static U16 IcePureTemp(E_API_A1080_T Id)
{
    return GetTempDiag( ANI_TEMP_ROOM_WATER, TEMP_ID_ROOM_WATER );
}

static U16 SensorTrayIcingSw(E_API_A1080_T Id)
{
    return 0;
}

