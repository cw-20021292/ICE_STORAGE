#include "process_nfc.h"
#include "nfc.h"
#include "eeprom.h"
#include "rtc.h"
#include "util.h"

#include "error.h"
#include "service.h"
#include "health.h"
#include "ster.h"
#include "water_out.h"

#include <stdio.h>
#include <string.h>


#define  DEFAULT_NFC_ERROR_COUNT    10
NFC_Control_T  nfc;

NFC_Info_T     gInfo;
NFC_TagTime_T  gTagTime;
NFC_GreenPlug_T   gGreenPlug;
NFC_WaterUsedDay_T gWaterUsedDay;

static U8 ConvErrorCode(ErrorId_T mErr);


void InitNfc(void)
{
    nfc.Mode   = 0;
    nfc.Error  = FALSE;
    nfc.ErrorCount = DEFAULT_NFC_ERROR_COUNT;

    nfc.ErrorHistoryCount  = 0;
    nfc.UserHistoryCount   = 0;   
    nfc.SterHistoryCount   = 0;
    nfc.WaterHistoryCount  = 0;

    // EEPROM에서 읽어온 TAG 타임..
    nfc.TagTime.Year  = 0xFF; // Default value
    nfc.TagTime.Month = 0xFF;
    nfc.TagTime.Date  = 0xFF;
    nfc.TagTime.Hour  = 0xFF;
    nfc.TagTime.Min   = 0xFF;
    nfc.TagTime.Sec   = 0xFF;
}

void SetNfcMode(U16 mu16Mode)
{
    nfc.Mode |= mu16Mode;
}

U8 GetNfcError(void)
{
    return nfc.Error;
}


U8 GetNfcErrorHisCount(void)
{
    return nfc.ErrorHistoryCount;
}

void SetNfcErrorHisCount(U8 mu8Count)
{
    nfc.ErrorHistoryCount = mu8Count;
    SaveEepromId( EEP_ID_NFC_ERROR_HIS_COUNT );
}


U8 GetNfcUserHisCount(void)
{
    return nfc.UserHistoryCount;
}

void SetNfcUserHisCount(U8 mu8Count)
{
    nfc.UserHistoryCount = mu8Count;
    SaveEepromId( EEP_ID_NFC_USER_HIS_COUNT );
}


U8 GetNfcSterHisCount(void)
{
    return nfc.SterHistoryCount;
}

void SetNfcSterHisCount(U8 mu8Count)
{
    nfc.SterHistoryCount = mu8Count;
    SaveEepromId( EEP_ID_NFC_STER_HIS_COUNT );
}

U8 GetNfcWaterHisCount(void)
{
    return nfc.WaterHistoryCount;
}

void SetNfcWaterHisCount(U8 mu8Count)
{
    nfc.WaterHistoryCount = mu8Count;
    SaveEepromId( EEP_ID_NFC_WATER_HIS_COUNT );
}


// Get/Set Tag Time
void GetNfcTagTime( NFC_TagTime_T *pTime)
{
    MEMCPY( (void __FAR *)pTime, (const void __FAR *)&nfc.TagTime, sizeof( NFC_TagTime_T ));
}

void SetNfcTagTime( NFC_TagTime_T *pTime)
{
    MEMCPY( (void __FAR *)&nfc.TagTime, (const void __FAR *)pTime, sizeof( NFC_TagTime_T ));
}


static U8 IsSetMode(U16 mu16Mode)
{
    if( (nfc.Mode & mu16Mode) != 0 )
    {
        return TRUE;
    }

    return FALSE;
}

static void ClearMode(U16 mu16Mode)
{
    nfc.Mode &= ~mu16Mode;
}



typedef U8 (*Action_T)(void);
typedef struct _nfc_event_
{
    U16  Mode;
    Action_T  Action;

} NfcEvent_T;

static U8 Evt_InitInfo(void);
static U8 Evt_EraseAll(void);
static U8 Evt_ServiceCheckDay(void);
static U8 Evt_Error(void);
static U8 Evt_UpdateSetting(void);
static U8 Evt_DailyData(void);
static U8 Evt_UseWaterData(void);
static U8 Evt_SterPassDay(void);
static U8 Evt_SterHistory(void);

NfcEvent_T NfcEventList[] = 
{
    { NFC_INIT,              Evt_InitInfo  },
    { NFC_ERASE_ALL,         Evt_EraseAll  },
    { NFC_SERVICE_CHECK_DAY, Evt_ServiceCheckDay  },
    { NFC_ERROR,             Evt_Error  },
    { NFC_UPDATE_SETTING,    Evt_UpdateSetting  },
    { NFC_DAILY_DATA,        Evt_DailyData  },
    { NFC_USE_WATER_DATA,    Evt_UseWaterData  },
    { NFC_STER_PASS_DAY,     Evt_SterPassDay  },
    { NFC_STER_HISTORY,      Evt_SterHistory  }
};
#define  SZ_LIST  ( sizeof(NfcEventList) / sizeof(NfcEvent_T) )


void ProcessNfc(void)
{
    static U8 mu8CheckError = FALSE;    // 최초 통신 성공 여부 확인
    U8 mu8i;
    U8 mu8Ret;
    U16 mu16Mode;


    if( nfc.Error == TRUE )
    {
        return ;
    }

    for( mu8i = 0; mu8i < SZ_LIST ; mu8i++ )
    {
        mu16Mode = NfcEventList[ mu8i ].Mode;

        if( IsSetMode( mu16Mode ) == TRUE )
        {
            mu8Ret = TRUE;
            if( NfcEventList[ mu8i ].Action != NULL )
            {
                mu8Ret = NfcEventList[ mu8i ].Action();
            }

            // Clear Mode 
            if( mu8Ret == TRUE )
            {
                mu8CheckError = TRUE;
                ClearMode( mu16Mode );
                nfc.ErrorCount = DEFAULT_NFC_ERROR_COUNT;
            }
            else if( mu8CheckError == FALSE )
            {
                // NFC 통신이 최초 1회 정상 통신하면 이후에는 NFC 통신 에러 검출을 하지 않음.
                if( nfc.ErrorCount == 0 )
                {
                    nfc.Error = TRUE;     // NFC 통신 에러 발생
                }
                else
                {
                    nfc.ErrorCount--;
                }
            }
        }
    }
}

static U8 Evt_InitInfo(void)
{
    U8 mu8Ret;
    U8 mu8Val;

    mu8Ret = NFC_ReadInfor( &gInfo );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    if( gInfo.Start != NFC_START_CODE 
            || gInfo.End != NFC_END_CODE )
    {
        // Tag Time
        mu8Ret = NFC_InitLastTagTime();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitGreenPlug();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitWaterUsedDay();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitWaterState();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }
        mu8Ret = NFC_InitSterPassedDay();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitSterConf( 1, 0, 0, 5 );
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitUserUseState();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Val = GetErrorId();
        mu8Val = ConvErrorCode( mu8Val );
        mu8Ret = NFC_InitErrorState( mu8Val );
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitServiceState( GetServiceCheckDay() );
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitWaterHistory();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }
        mu8Ret = NFC_InitSterHistory();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitErrorHistory();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        mu8Ret = NFC_InitUserHistory();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        // Dev Info
        mu8Ret = NFC_InitInfo();
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }
    }

    return TRUE;
}

static U8 Evt_EraseAll(void)
{
    return NFC_EraseAll(); 
}

static U8 Evt_ServiceCheckDay(void)
{
    NFC_ServiceState_T mService;


    // 내부에서는 90일에서 downcount
    // NFC에는 최초 1일 기준으로해서 91일까지 updount
    mService.CheckDay = 91 - GetServiceCheckDay();
    return NFC_WriteServiceState( &mService );
}

// 에러 코드 변환 함수
// 에러 코드를 NFC 저장 코드 형식으로 변환 시킨다.
// return 타입은 hex 형식으로 반환한다.
static U8 ConvErrorCode(ErrorId_T mErr)
{
    U8 mu8ErrCode = 0x00;

    switch( mErr )
    {
        case ERR_ROOM_LOW_LEVEL:    mu8ErrCode = 0x01; break;
        case ERR_ROOM_HIGH_LEVEL:   mu8ErrCode = 0x02; break;
        case ERR_ROOM_COMPLEX:      mu8ErrCode = 0x03; break;
        case ERR_ROOM_OVF:          mu8ErrCode = 0x04; break;

        case ERR_TEMP_ROOM_WATER:   mu8ErrCode = 0x06; break;
        case ERR_TEMP_HOT_WATER:    mu8ErrCode = 0x07; break;
        case ERR_OVER_HOT_WATER:    mu8ErrCode = 0x07; break;
        case ERR_TEMP_COLD_WATER:   mu8ErrCode = 0x08; break;
        case ERR_TEMP_AMBIENT:      mu8ErrCode = 0x09; break;

        case ERR_DRAIN_PUMP:        mu8ErrCode = 0x0E; break;
        case ERR_STR_MOUDLE:        mu8ErrCode = 0x0F; break;
        case ERR_CIRCULATE_PUMP:    mu8ErrCode = 0x15; break;
        case ERR_COLD_LOW_LEVEL:    mu8ErrCode = 0x17; break;

        case ERR_MICRO_SW_DETECT:   mu8ErrCode = 0x18; break;
        case ERR_MICRO_SW_MOVE:     mu8ErrCode = 0x19; break;
        case ERR_GAS_SW_VALVE:      mu8ErrCode = 0x20; break;

        default:                    mu8ErrCode = 0x00; break;
    }

    return mu8ErrCode;
}


static U8 Evt_Error(void)
{
    U8 mu8Ret;
    ErrorId_T  mNewError;
    NFC_ErrorState_T mError;
    NFC_ErrorHis_T mErrorHis;
    TimeData_T mRTC_Time;
    U8 mu8Count;


    // RTC 오류이면, 저장하지 않고 완료 처리
    if( IsRTC_Error() == TRUE )
    {
        return TRUE;
    }

    mNewError = GetErrorId();
    //mError.Error = mNewError;
    mError.Error = ConvErrorCode( mNewError );

    // Setp 1. Save new error state
    mu8Ret = NFC_WriteErrorState( &mError );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // Step 2. Skip if error state is released.
    if( mNewError == ERR_NONE )
    {
        return TRUE; // No error.
    }

    // Step 3. Save new error history list
    mu8Count = GetNfcErrorHisCount();
    GetRtcTime( &mRTC_Time );
    mErrorHis.Year   = Dec2Hex(mRTC_Time.Year); 
    mErrorHis.Month  = Dec2Hex(mRTC_Time.Month);
    mErrorHis.Date   = Dec2Hex(mRTC_Time.Date); 
    mErrorHis.Hour   = Dec2Hex(mRTC_Time.Hour);  
    mErrorHis.Error  = ConvErrorCode( mNewError );
    mu8Ret = NFC_WriteErrorHistory( mu8Count, &mErrorHis );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Count++;
    if( mu8Count >= 10 )
    {
        mu8Count = 0;
    }
    SetNfcErrorHisCount( mu8Count );
    return TRUE;
}

static U8 IsValidDate( NFC_TagTime_T *pTag )
{
    if(  pTag->Year <= 99
            && pTag->Month <= 12
            && pTag->Date  <= 31
            && pTag->Hour  <= 23
            && pTag->Min   <= 59
            && pTag->Sec   <= 59 )
    {
        return TRUE;    // 유효성 정상
    }

    return FALSE;   // 유효성 에러
}

// 
// NFC에서 TAG TIME 시간을 읽어와서 RTC에 저장한다.
// TAG TIME이 이전 TAG TIME과 다르면 설정 정보도 업데이트 한다.
//
static U8 Evt_UpdateSetting(void)
{
    I8 mLastedDate;
    U8 mu8Ret;
    TimeData_T mRTC_Time;
    NFC_TagTime_T mTemp_TagTime;
    NFC_TagTime_T mNFC_TagTime;
    NFC_SterConf_T mSterConf;


    // Step 1. Read Tag Time NFC 
    mu8Ret = NFC_ReadLastTagTime( &mNFC_TagTime );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // Step 2. Read Ster Config
    mu8Ret = NFC_ReadSterConf( &mSterConf );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // Step 3. Clear Tag Date
    MEMSET( (void __FAR *)&mTemp_TagTime, 0xFF, sizeof(mTemp_TagTime) );
    mu8Ret = NFC_WriteLastTagTime( &mTemp_TagTime );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mNFC_TagTime.Year = Hex2Dec( mNFC_TagTime.Year );
    mNFC_TagTime.Month = Hex2Dec( mNFC_TagTime.Month );
    mNFC_TagTime.Date = Hex2Dec( mNFC_TagTime.Date );
    mNFC_TagTime.Hour = Hex2Dec( mNFC_TagTime.Hour );
    mNFC_TagTime.Min = Hex2Dec( mNFC_TagTime.Min );
    mNFC_TagTime.Sec = Hex2Dec( mNFC_TagTime.Sec );

    if( IsValidDate( &mNFC_TagTime ) == TRUE )
    {

        // Step 4. Update new RTC Timer( RTC )
        mRTC_Time.Y2K   = 1;
        mRTC_Time.Year  = mNFC_TagTime.Year;
        mRTC_Time.Month = mNFC_TagTime.Month;
        mRTC_Time.Date  = mNFC_TagTime.Date;
        mRTC_Time.Hour  = mNFC_TagTime.Hour;
        mRTC_Time.Min   = mNFC_TagTime.Min;
        mRTC_Time.Sec   = mNFC_TagTime.Sec;

        // Check Valid date by month 
        mLastedDate = GetLastDateByMonth( mRTC_Time.Year, mRTC_Time.Month );
        if( mLastedDate > 0 && mRTC_Time.Date <= mLastedDate )
        {
            SetRtcTime( &mRTC_Time );
        }
    }

    // Step 5. Update new Ster Config
    SetSterPeriodMode( Hex2Dec(mSterConf.Mode) );
    SetHealthHour( Hex2Dec(mSterConf.Hour) );
    SetHealthMin( Hex2Dec(mSterConf.Min) );
    SetSterPeriodDay( Hex2Dec(mSterConf.Period) );

    return TRUE;
}

static void ResetWaterOutDailyTime(void)
{
    SetWaterOutDailyTime( SEL_WATER_ROOM, 0 );
    SetWaterOutDailyTime( SEL_WATER_COLD, 0 );
    SetWaterOutDailyTime( SEL_WATER_HOT, 0 );
}

static U8 UpdateWaterOutDailTime(U8 mu8Day )
{
    U8 mu8Ret;
    NFC_WaterHis_T mWaterHis;


    mWaterHis.Cold   = GetWaterOutDailyCupNum( SEL_WATER_COLD ); 
    mWaterHis.Hot    = GetWaterOutDailyCupNum( SEL_WATER_HOT );
    mWaterHis.Room   = GetWaterOutDailyCupNum( SEL_WATER_ROOM ); 
    return NFC_WriteWaterHistory( mu8Day, &mWaterHis );
}

// 물 사용일 ( 60일 )
// 일일 냉/온/정 물사용량 저장
static U8 Evt_DailyData(void)
{
    U8 mu8Ret;
    NFC_WaterUsedDay_T mDay;


    // 물 사용일 값을 읽어 온다.
    mu8Ret = NFC_ReadWaterUsedDay( &mDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    if( mDay.Over_60_Day == TRUE 
            && mDay.UsedDay >= 60 )
    {
        ResetWaterOutDailyTime();
        return TRUE; // 더 이상 날짜를 카운트 할 수 없음..
    }

    // 하루 물 사용 총량을 저장한다.
    mu8Ret = UpdateWaterOutDailTime( mDay.UsedDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // 물 사용일 계산하고 저장한다.
    mDay.UsedDay++;
    if( mDay.UsedDay >= 60 
            && mDay.Over_60_Day == FALSE )
    {
        mDay.UsedDay = 1;
        mDay.Over_60_Day = TRUE;
    }

    mu8Ret = NFC_WriteWaterUsedDay( &mDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    ResetWaterOutDailyTime();
    return TRUE;
}

static U8 GetNfcWaterType(void)
{
    return GetWaterOutLastedSelect() + 1;
}

static U8 GetNfcWaterAmout(void)
{
    U8 mu8Id;

    mu8Id = GetWaterOutLastedSelect();
    return GetWaterOutLastedCupNum( mu8Id );
}

static U8 Evt_UseWaterData(void)
{
    U8 mu8Ret;
    U8 mu8Count;
    NFC_WaterState_T mWater;
    NFC_UserHis_T mUserHis;
    TimeData_T mRTC_Time;
    NFC_WaterUsedDay_T mDay;


    // RTC 오류이면, 저장하지 않고 완료 처리
    if( IsRTC_Error() == TRUE )
    {
        return TRUE;
    }

    GetRtcTime( &mRTC_Time );
    mu8Count = GetNfcWaterHisCount();

    // 마지막 물 사용 히스토리 
    mWater.WaterType    = GetNfcWaterType();
    mWater.WaterAmount  = GetNfcWaterAmout();
    mu8Ret = NFC_WriteWaterState( &mWater );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // 사용자 사용 히스토리 저장 ( 250회 )
    mUserHis.Year  = Dec2Hex(mRTC_Time.Year);
    mUserHis.Month = Dec2Hex(mRTC_Time.Month);
    mUserHis.Date  = Dec2Hex(mRTC_Time.Date);
    mUserHis.Hour  = Dec2Hex(mRTC_Time.Hour);
    mUserHis.User  = 0; // NO SELECTED 
    mUserHis.WaterType = GetNfcWaterType();
    mUserHis.WaterAmount = GetNfcWaterAmout();
    mu8Ret = NFC_WriteUserHistory( mu8Count, &mUserHis );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // 물 사용일 값을 읽어 온다.
    mu8Ret = NFC_ReadWaterUsedDay( &mDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // 하루 물 사용 총량을 저장한다.
    mu8Ret = UpdateWaterOutDailTime( mDay.UsedDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Count++;
    if( mu8Count >= 250 )
    {
        mu8Count = 0;
    }
    SetNfcWaterHisCount( mu8Count );

    return TRUE;
}

static U8 Evt_SterPassDay(void)
{
    U8 mu8Ret;
    NFC_SterPassedDay_T mSterDay;


    mSterDay.PassedDay = GetSterPassDay();
    mu8Ret = NFC_WriteSterPassedDay( &mSterDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 Evt_SterHistory(void)
{
    U8 mu8Ret;
    U8 mu8Count;
    NFC_SterHis_T mSterHis;
    TimeData_T mRTC_Time;


    // RTC 오류이면, 저장하지 않고 완료 처리
    if( IsRTC_Error() == TRUE )
    {
        return TRUE;
    }

    mu8Count = GetNfcSterHisCount();
    GetRtcTime( &mRTC_Time );
    mSterHis.Year   = Dec2Hex(mRTC_Time.Year); 
    mSterHis.Month  = Dec2Hex(mRTC_Time.Month);
    mSterHis.Date   = Dec2Hex(mRTC_Time.Date); 
#if CONFIG_TEST
    mSterHis.Type   = Dec2Hex(mRTC_Time.Hour);
#else
    mSterHis.Type   = NFC_STER_CONF_TYPE_CIRCULATE;
#endif
    mu8Ret = NFC_WriteSterHistory( mu8Count, &mSterHis );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Count++;
    if( mu8Count >= 80 )
    {
        mu8Count = 0;
    }
    SetNfcSterHisCount( mu8Count );

    return TRUE;
}
