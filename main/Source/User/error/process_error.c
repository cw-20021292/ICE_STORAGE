#include "process_error.h"
#include "error.h"

#include "eeprom.h"
#include "service.h"
#include "level.h"
#include "ster.h"

#include "err_temp_cold_water.h"
#include "err_temp_room_water.h"
#include "err_temp_hot_water.h"
#include "err_temp_eva_1.h"
#include "err_temp_eva_2.h"
#include "err_temp_ambient.h"
#include "err_room_level.h"
#include "err_cold_level.h"
#include "err_cold_level.h"
#include "err_cold_level.h"
#include "err_leak.h"
#include "err_ice_tray.h"
#include "err_drain_pump.h"
#include "err_circulate_pump.h"
#include "err_csi_module.h"
#include "err_gas_switch_valve.h"
#include "err_over_hot_water.h"
#include "err_comp_bldc.h"
#include "err_comp_bldc_comm.h"
#include "err_outage_water.h"

typedef U8 (*ErrFun_T)(U8 mu8Error);
typedef struct _error_list_
{
    EErrorId_T Id;
    U8 PrevError;
    U8 Error;
    U8 SterActive;
    U8 DispActive;      // 에러 검출은 하되, Display 여부 

    ErrFun_T Check;
    ErrFun_T Release;
} ErrorList_T;

void InitErrorList(void)
{
    InitError();
    InitErrGasSwitchValve();
}


// 일반 운전 조건에서 에러 검출
ErrorList_T ErrList[] = 
{
    // ( 인덱스가 높을수록 우선순위가 높다 )

    /*ID                      PrevError  Error   Ster      Disp     Check error                    Check release error */
    //{ ERR_TEMP_EVA_1,         FALSE,  FALSE,  FALSE,    FALSE,    CheckErrTempEva_1,             ReleaseErrTempEva_1 },
    //{ ERR_TEMP_EVA_2,         FALSE,  FALSE,  FALSE,    FALSE,    CheckErrTempEva_2,             ReleaseErrTempEva_2 },

    //{ ERR_ID_STR_MOUDLE,         FALSE,  FALSE,  TRUE,     TRUE,     CheckErrCSI,                   ReleaseErrCSI },    
#if CONFIG_GAS_SW
    { ERR_ID_GAS_SW_VALVE,          FALSE,  FALSE,  FALSE,    TRUE,     CheckErrGasSwitchValve,        NULL },  // There is no release error
#endif

    { ERR_ID_CIRCULATE_PUMP,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCirculatePump,         NULL },  // There is no release error
    { ERR_ID_DRAIN_PUMP,            FALSE,  FALSE,  TRUE,     TRUE,     CheckErrDrainPump,             NULL },  // There is no release error
    { ERR_ID_MICRO_SW_MOVE_DEICING, FALSE,  FALSE,  TRUE,     TRUE,     CheckErrIceTrayMovingDeicing,  NULL },  // There is no release error
    { ERR_ID_MICRO_SW_MOVE,         FALSE,  FALSE,  TRUE,     TRUE,     CheckErrIceTrayMoving,         NULL },  // There is no release error
    { ERR_ID_MICRO_SW_DETECT,       FALSE,  FALSE,  TRUE,     TRUE,     CheckErrIceTrayMicroSwitch,    NULL },  // There is no release error

    { ERR_ID_COMP_BLDC_ERR9,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr9,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_ERR8,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr8,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_ERR7,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr7,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_ERR6,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr6,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_ERR5,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr5,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_ERR4,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr4,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_ERR3,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr3,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_ERR2,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr2,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_ERR1,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcErr1,          NULL },  // There is no release error
    { ERR_ID_COMP_BLDC_COMM,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrCompBldcComm,          NULL },  // There is no release error
    { ERR_ID_PROTECT_COMP,          FALSE,  FALSE,  FALSE,    TRUE,     CheckProtectCompBldc,          ReleaseProtectCompBldc },
    { ERR_ID_PROTECT_COMP_COMM,     FALSE,  FALSE,  FALSE,    TRUE,     CheckProtectCompBldcComm,      ReleaseProtectCompBldcComm },    
    { ERR_ID_OVER_HOT_WATER,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrOverHotWater,          NULL },  // There is no release error

    { ERR_ID_TEMP_AMBIENT_2,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrTempAmbient2,          ReleaseErrTempAmbient2 },    
    { ERR_ID_TEMP_AMBIENT,          FALSE,  FALSE,  FALSE,    FALSE,    CheckErrTempAmbient,           ReleaseErrTempAmbient },    
    { ERR_ID_TEMP_COLD_WATER,       FALSE,  FALSE,  FALSE,    TRUE,     CheckErrTempColdWater,         ReleaseErrTempColdWater },
    { ERR_ID_TEMP_HOT_WATER,        FALSE,  FALSE,  FALSE,    TRUE,     CheckErrTempHotWater,          ReleaseErrTempHotWater },
    { ERR_ID_TEMP_ROOM_WATER,       FALSE,  FALSE,  FALSE,    TRUE,     CheckErrTempRoomWater,         ReleaseErrTempRoomWater },

    { ERR_ID_COLD_LOW_LEVEL,        FALSE,  FALSE,  TRUE,     TRUE,     CheckErrColdLow,               ReleaseErrColdLow },
    { ERR_ID_ROOM_OVF,              FALSE,  FALSE,  TRUE,     TRUE,     CheckErrRoomOvf,               NULL },  // There is no release error
    { ERR_ID_ROOM_COMPLEX,          FALSE,  FALSE,  TRUE,     TRUE,     CheckErrRoomComplex,           NULL },  // There is no release error
    { ERR_ID_ROOM_HIGH_LEVEL,       FALSE,  FALSE,  TRUE,     TRUE,     CheckErrRoomHigh,              NULL },  // There is no release error
    { ERR_ID_ROOM_LOW_LEVEL,        FALSE,  FALSE,  TRUE,     TRUE,     CheckErrRoomLow,               ReleaseErrRoomLow },

    { ERR_ID_OUTAGE_WATER,          FALSE,  FALSE,  TRUE,     FALSE,    CheckErrorOutageWater,         ReleaseErrorOutageWater },

#if CONFIG_LEAK
    { ERR_ID_LEAK,                  FALSE,  FALSE,  TRUE,     TRUE,     CheckErrLeak,                  ReleaseErrLeak },
#endif
};
#define SZ_ERR_LIST      (sizeof(ErrList)/sizeof(ErrorList_T))

U8 dbg_iot_err = 0;
static EErrorId_T FindCheckError(ErrorList_T *pErrList, U16 mu16Size)
{
    U8 i;
    ErrFun_T pCheck;
    ErrFun_T pRelease;
    EErrorId_T mId = ERR_ID_NONE;
    EErrorId_T mNewId = ERR_ID_NONE;
    U8 mu8Error = FALSE;
    SErrorDate_T mDate;
    ErrorList_T *pList = NULL;
    U8 mu8IsChangedErrorStatus = FALSE;

    GetErrorDate( &mDate );
    for( i = 0; i < mu16Size; i++ ) 
    {
        pList = (pErrList + i );

        // Mapping error status..
        mId      = pList->Id;
        mu8Error = pList->Error;
        pCheck   = pList->Check;
        pRelease = pList->Release;

#if CONFIG_STER
        // 살균 중인 경우, 살균 시 에러 검출 대상만 검사
        if( IsStartSter() == TRUE )
        {
            if( pList->SterActive == FALSE )
            {
                continue;
            }
        }
#endif
        // 현재 상태가  에러이면, 에러 해제 조건 검사 
        if( mu8Error == TRUE )
        {
            // 살균 중 에러가 발생된 경우, 에러 해제 조건 없음.
            // 계속해서 에러 상태 표시
#if CONFIG_STER
            if( GetSterErrWaterOut() == FALSE )
#endif
            {
                if( pRelease != NULL )
                {
                    mu8Error = pRelease( mu8Error );
                }
            }
        }
        // 현재 상태가 정상이면, 에러 발생 조건 감사
        else
        {
            if( pCheck != NULL )
            {
                mu8Error = pCheck( mu8Error );
            }
        }

        // Update new error status
#if 1
        
        pList->Error = mu8Error;
        if( pList->PrevError != pList->Error )
        {
            pList->PrevError = pList->Error;

            //SetErrorStatus( mId, pList->Error );
            SetErrorStatusDate( mId, pList->Error, &mDate );

            // WIFI (화면에 표시되는 에러만 서버에 전달 )
            if( pList->DispActive == TRUE )
            {
               mu8IsChangedErrorStatus = TRUE; 
            }
        }

#else
        pList->Error = mu8Error;

        // Mapping error status..
        mId = (pList + i)->Id;
        SetErrorStatus( mId, (pList +i)->Error );
#endif

        if( pList->DispActive == TRUE )
        {
            //  Top priority Error
            if( GetErrorStatus( mId ) == TRUE )
            {
                mNewId = mId;
            }
        }
    }

#if 0
    // WIFI     //kdh 삭제
    if( mu8IsChangedErrorStatus == TRUE )
    {
        WifiSendData(WIFI_DATA_ERROR);
        dbg_iot_err++;
    }
#endif

    return mNewId;

}


// 정수 탱크가 이전에는 OPEN이었다가 현재 OPEN이면, 
// 수위 에러 카운트 초기화
static void ResetRoomLevelErrorCount(void)
{
    static U8 mu8PrevTankOpen = TRUE;
    static U8 mu8CurTankOpen = TRUE;


    mu8CurTankOpen = GetServiceCheckTankOpen();
    if( mu8PrevTankOpen == TRUE 
            &&  mu8CurTankOpen== FALSE )
    {
        ClearTankLevelErrorCount();
    }

    if( mu8PrevTankOpen != mu8CurTankOpen )
    {
        mu8PrevTankOpen = mu8CurTankOpen;
    }
}

void ProcessError(void)
{
#if( CONFIG_USE_ERROR == 1 )

    EErrorId_T mNewId = ERR_ID_NONE;

    // Check Error
    mNewId = FindCheckError( &ErrList[0], SZ_ERR_LIST );

    // Update New Error Id
    UpdateNewError( mNewId );

    // 탱크 수위 에러 적산 카운터 초기화
    ResetRoomLevelErrorCount();

    // 냉매 전환 밸브 에러 검사 
#if CONFIG_GAS_SW
    ProcessErrGasSwitchValve();
#endif

#endif
}
