#include "hw.h"
#include "hal_i2c_nfc.h"
#include "nfc.h"
#include "util.h"

// ADDRESS 
enum
{
    // INFORMATION
    NFC_ADDR_START         = 0x0000,
    NFC_ADDR_GROUP         = 0x0002,
    NFC_ADDR_MODEL_CODE    = 0x0003,
    NFC_ADDR_END           = 0x000F,

    // LAST TAGGING TIME
    NFC_TAG_TIME_SEC       = 0x0040,
    NFC_TAG_TIME_MIN,
    NFC_TAG_TIME_HOUR,
    NFC_TAG_TIME_DATE      = 0x0044,
    NFC_TAG_TIME_MONTH,
    NFC_TAG_TIME_YEAR,

    // GREEN PLUG
    NFC_USED_WATER_DAY       = 0x0050,
    NFC_GP_WEEK,   
    NFC_GP_DAY,     
    NFC_GP_WEEK_FLAG,        
    NFC_IS_USE_WATER_60DAYS = 0x0058,

    // BAR-CODE
    NFC_BAR_CODE            = 0x0060,

    // LAST USED STATE
    NFC_USED_WATER_AMOUNT   = 0x00A0,
    NFC_USED_WATER_TYPE,     
    NFC_USED_PASSED_DAY, 
    NFC_USED_GP_TIME,

    // STER - CONFIG INFO
    NFC_STER_CONF_MODE           = 0x00A4,
    NFC_STER_CONF_HOUR,
    NFC_STER_CONF_MIN,
    NFC_STER_CONF_CYCLE,

    // USER - WATER
    NFC_USER_WATER_AMOUNT   = 0x00A8,
    NFC_USER_WATER_TYPE,
    NFC_USER,

    // WATER TOTAL AMOUNT
    NFC_TOTAL_AMOUNT_COLD   = 0x0100,
    NFC_TOTAL_AMOUNT_HOT,
    NFC_TOTAL_AMOUNT_ROOM,


    // STER HISTORY
    NFC_STER_HIS_YEAR    = 0x01B4,
    NFC_STER_HIS_MONTH,
    NFC_STER_HIS_DAY,
    NFC_STER_HIS_TYPE,
    
    // GREEN PLUG HISTORY
    NFC_GP_HIS           = 0x11E0,

        
    // ERROR HISTORY
    NFC_ERROR_HIS_YEAR   = 0x1780,
    NFC_ERROR_HIS_MONTH,
    NFC_ERROR_HIS_DAY,
    NFC_ERROR_HIS_HOUR,
    NFC_ERROR_HIS_CODE,

    // STATE
    NFC_ERROR_CODE         = 0x17B4,
    NFC_SERVICE_CHECK      = 0x17B6,

    //  USER HISTORY
    NFC_USER_HIS_YEAR      = 0x17B8,
    NFC_USER_HIS_MONTH,
    NFC_USER_HIS_DAY, 
    NFC_USER_HIS_HOUR, 
    NFC_USER_HIS_USER, 
    NFC_USER_HIS_TYPE, 
    NFC_USER_HIS_AMOUNT, 
};


static U8 WritePageDummyData(U16 mu16StartAddr, U16 mu16EndAddr, U8 mu8Data)
{
    U8 mu8Ret;
    U8 mu8Buf[4];
    U16 mu16Addr;


    mu8Buf[0] = mu8Data;
    mu8Buf[1] = mu8Data;
    mu8Buf[2] = mu8Data;
    mu8Buf[3] = mu8Data;

    for( mu16Addr = mu16StartAddr; mu16Addr <= mu16EndAddr ; )
    {
        mu8Ret  = HAL_NFC_PageWrite( mu16Addr, mu8Buf, 4 );
        if( mu8Ret == FALSE )
        {
            return FALSE;
        }

        // Count next address..
        mu16Addr += 4;
    }

    return TRUE;
}


U8 NFC_InitInfo( void )
{
    NFC_Info_T mData;

    mData.Start = NFC_START_CODE;
    mData.Group = NFC_GROUP_TYPE;
    MEMCPY( (void __FAR *)&mData.Model[0], (const void __FAR *)NFC_MODEL_1, 5 );
    MEMCPY( (void __FAR *)&mData.Model[5], (const void __FAR *)NFC_MODEL_2, 5 );
    mData.End = NFC_END_CODE;

    return NFC_WriteInfo( &mData );
}

U8 NFC_WriteInfo(NFC_Info_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 4 ];
    

    mu8Buf[ 0 ] = pData->Start;
    mu8Buf[ 1 ] = 0xFF;    // Unused
    mu8Buf[ 2 ] = pData->Group;
    mu8Buf[ 3 ] = pData->Model[0];
    mu8Ret = HAL_NFC_PageWrite( NFC_ADDR_START, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Buf[ 0 ] = pData->Model[1];
    mu8Buf[ 1 ] = pData->Model[2];
    mu8Buf[ 2 ] = pData->Model[3];
    mu8Buf[ 3 ] = pData->Model[4];
    mu8Ret = HAL_NFC_PageWrite( NFC_ADDR_START + 0x04, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Buf[ 0 ] = pData->Model[5];
    mu8Buf[ 1 ] = pData->Model[6];
    mu8Buf[ 2 ] = pData->Model[7];
    mu8Buf[ 3 ] = pData->Model[8];
    mu8Ret = HAL_NFC_PageWrite( NFC_ADDR_START + 0x08, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Buf[ 0 ] = pData->Model[9];
    mu8Buf[ 1 ] = 0xFF; // Unused
    mu8Buf[ 2 ] = 0xFF; // Unused
    mu8Buf[ 3 ] = pData->End;
    mu8Ret = HAL_NFC_PageWrite( NFC_ADDR_START + 0x0C, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}


U8 NFC_ReadInfor(NFC_Info_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 16 ];
    

    mu8Ret = HAL_NFC_SeqRead( NFC_ADDR_START, mu8Buf, 16 );
    if( mu8Ret == TRUE )
    {
        pData->Start    = mu8Buf[ 0 ];
        // mu8Buf[1] unused
        pData->Group    = mu8Buf[ 2 ];
        pData->Model[0] = mu8Buf[ 3 ];
        pData->Model[1] = mu8Buf[ 4 ];
        pData->Model[2] = mu8Buf[ 5 ];
        pData->Model[3] = mu8Buf[ 6 ];
        pData->Model[4] = mu8Buf[ 7 ];
        pData->Model[5] = mu8Buf[ 8 ];
        pData->Model[6] = mu8Buf[ 9 ];
        pData->Model[7] = mu8Buf[ 10 ];
        pData->Model[8] = mu8Buf[ 11 ];
        pData->Model[9] = mu8Buf[ 12 ];
        // mu8Buf[13] unused
        // mu8Buf[14] unused
        pData->End      = mu8Buf[ 15 ];
    }

    return mu8Ret;
}


// NFC 마지막 테깅 시간
U8 NFC_InitLastTagTime(void)
{
    return WritePageDummyData( NFC_TAG_TIME_SEC, 0x0044, 0xFF );
}

U8 NFC_WriteLastTagTime(NFC_TagTime_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 4 ];
    

    mu8Buf[ 0 ] = pData->Sec;
    mu8Buf[ 1 ] = pData->Min;
    mu8Buf[ 2 ] = pData->Hour;
    mu8Buf[ 3 ] = 0; // Unused
    mu8Ret = HAL_NFC_PageWrite( NFC_TAG_TIME_SEC, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Buf[ 0 ] = pData->Date;
    mu8Buf[ 1 ] = pData->Month;
    mu8Buf[ 2 ] = pData->Year;
    mu8Buf[ 3 ] = 0; // Unused 
    mu8Ret = HAL_NFC_PageWrite( NFC_TAG_TIME_DATE, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

// NFC 마지막 테깅 시간
U8 NFC_ReadLastTagTime(NFC_TagTime_T *pData)
{

    U8 mu8Ret;
    U8 mu8Buf[ 8 ];
    

    mu8Ret = HAL_NFC_SeqRead( NFC_TAG_TIME_SEC, mu8Buf, 8 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    pData->Sec    = mu8Buf[ 0 ];
    pData->Min    = mu8Buf[ 1 ];
    pData->Hour   = mu8Buf[ 2 ];
    // mu8Buf[3] unused
    pData->Date   = mu8Buf[ 4 ];
    pData->Month  = mu8Buf[ 5 ];
    pData->Year   = mu8Buf[ 6 ];
    // mu8Buf[7] unused


    return TRUE;
}

// Water GreenPlug Date
U8 NFC_InitGreenPlug(void)
{
    NFC_GreenPlug_T  mData;

    mData.Week      = 1;
    mData.Day       = 0;
    mData.WeekFlag  = 0;
    mData.StartTime = 0;    // GP TIME
    return NFC_WriteGreenPlug( &mData );
}


U8 NFC_WriteGreenPlug(NFC_GreenPlug_T *pData)
{
    U8 mu8Ret;
    

    mu8Ret = HAL_NFC_ByteWrite( NFC_GP_WEEK, pData->Week );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Ret = HAL_NFC_ByteWrite( NFC_GP_DAY, pData->Day );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Ret = HAL_NFC_ByteWrite( NFC_GP_WEEK_FLAG, pData->WeekFlag );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Ret = HAL_NFC_ByteWrite( NFC_USED_GP_TIME, pData->StartTime );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}


// Water GreenPlug Date
U8 NFC_ReadGreenPlug(NFC_GreenPlug_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 4 ];
    

    mu8Ret = HAL_NFC_SeqRead( NFC_USED_WATER_DAY, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    pData->Week     = mu8Buf[1];
    pData->Day      = mu8Buf[2];
    pData->WeekFlag = mu8Buf[3];

    mu8Ret = HAL_NFC_ByteRead( NFC_USED_GP_TIME, &mu8Buf[0] );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    pData->StartTime =  mu8Buf[0];

    return TRUE;
}


// Water Used State
U8 NFC_InitWaterUsedDay(void)
{
    NFC_WaterUsedDay_T mData;

    mData.UsedDay       = 1;
    mData.Over_60_Day   = 0;
    return NFC_WriteWaterUsedDay( &mData );
}

U8 NFC_WriteWaterUsedDay(NFC_WaterUsedDay_T *pData)
{
    U8 mu8Ret;
    

    mu8Ret = HAL_NFC_ByteWrite( NFC_USED_WATER_DAY, pData->UsedDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    mu8Ret = HAL_NFC_ByteWrite( NFC_IS_USE_WATER_60DAYS, pData->Over_60_Day );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

U8 NFC_ReadWaterUsedDay(NFC_WaterUsedDay_T *pData)
{
    U8 mu8Ret;
    U8 mu8Data;


    mu8Ret = HAL_NFC_ByteRead( NFC_USED_WATER_DAY, &mu8Data );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    pData->UsedDay =  mu8Data;

    mu8Ret = HAL_NFC_ByteRead( NFC_IS_USE_WATER_60DAYS, &mu8Data );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    pData->Over_60_Day =  mu8Data;


    return TRUE;
}


// Water State
U8 NFC_InitWaterState(void)
{ 
    NFC_WaterState_T mData;


    mData.WaterAmount = 0xFF;
    mData.WaterType   = 0xFF;
    return NFC_WriteWaterState( &mData );
}

U8 NFC_WriteWaterState(NFC_WaterState_T *pData)
{
    U8 mu8Ret;
    

    mu8Ret = HAL_NFC_ByteWrite( NFC_USED_WATER_AMOUNT, pData->WaterAmount );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    
    mu8Ret = HAL_NFC_ByteWrite( NFC_USED_WATER_TYPE, pData->WaterType );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

U8 NFC_ReadWaterState(NFC_WaterState_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 2 ];


    mu8Ret = HAL_NFC_SeqRead( NFC_USED_WATER_AMOUNT, mu8Buf, 2 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    pData->WaterAmount    = mu8Buf[0];
    pData->WaterType      = mu8Buf[1];

    return TRUE;
}


// Ster 
U8 NFC_InitSterPassedDay(void)
{
    NFC_SterPassedDay_T mData;

    mData.PassedDay = 1;
    return NFC_WriteSterPassedDay( &mData );
}

U8 NFC_WriteSterPassedDay(NFC_SterPassedDay_T *pData)
{
    U8 mu8Ret;
    

    mu8Ret = HAL_NFC_ByteWrite( NFC_USED_PASSED_DAY, pData->PassedDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    
    return TRUE;
}

U8 NFC_ReadSterPassedDay(NFC_SterPassedDay_T *pData)
{
    U8 mu8Ret;


    mu8Ret = HAL_NFC_ByteRead( NFC_USED_PASSED_DAY, &pData->PassedDay );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}


// Ster Config
U8 NFC_InitSterConf(U8 mu8Mode, U8 mu8Hour, U8 mu8Min, U8 mu8Period )
{
    NFC_SterConf_T mData;


    mData.Mode = mu8Mode;
    mData.Hour = mu8Hour;
    mData.Min  = mu8Min;
    mData.Period = mu8Period;
    return NFC_WriteSterConf( &mData );
}

U8 NFC_WriteSterConf(NFC_SterConf_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 4 ];
    

    mu8Buf[ 0 ] = pData->Mode;
    mu8Buf[ 1 ] = pData->Hour;
    mu8Buf[ 2 ] = pData->Min;
    mu8Buf[ 3 ] = pData->Period;

    mu8Ret = HAL_NFC_PageWrite( NFC_STER_CONF_MODE, mu8Buf, 4 );

    return mu8Ret;
}

U8 NFC_ReadSterConf(NFC_SterConf_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 4 ];
    

    mu8Ret = HAL_NFC_SeqRead( NFC_STER_CONF_MODE, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    pData->Mode  = mu8Buf[0];
    pData->Hour  = mu8Buf[1];
    pData->Min   = mu8Buf[2];
    pData->Period = mu8Buf[3];
    return TRUE;
}

// laste user use state
U8 NFC_InitUserUseState(void)
{
    return WritePageDummyData( NFC_USER_WATER_AMOUNT, 0x00B0U, 0xFFU );
}

U8 NFC_WriteUserUseState(U16 mu16User, NFC_UserUseState_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 4 ];
    U16 mu16Offset;
    

    mu8Buf[ 0 ] = pData->CupType;
    mu8Buf[ 1 ] = pData->WaterType;
    mu8Buf[ 2 ] = pData->User;
    mu8Buf[ 3 ] = 0xFF; // unused

    mu16Offset = 4 * mu16User;
    mu8Ret = HAL_NFC_PageWrite( NFC_USER_WATER_AMOUNT + mu16Offset, mu8Buf, 4 );

    return mu8Ret;
}

U8 NFC_ReadUserUseState(U16 mu16User, NFC_UserUseState_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 4 ];
    U16 mu16Offset;
    
    mu16Offset = 4 * mu16User;
    mu8Ret = HAL_NFC_SeqRead( NFC_USER_WATER_AMOUNT + mu16Offset, mu8Buf, 4 );
    if( mu8Ret == TRUE )
    {
        pData->CupType     = mu8Buf[0];
        pData->WaterType   = mu8Buf[1];
        pData->User        = mu8Buf[2];
        //mu8Buf[3]  unused
    }

    return mu8Ret;
}


// Water History
U8 NFC_InitWaterHistory(void)
{
    return WritePageDummyData( NFC_TOTAL_AMOUNT_COLD, 0x01B0U, 0xFFU );
}

U8 NFC_WriteWaterHistory(U16 mu16Day, NFC_WaterHis_T *pData)
{
    U8 mu8Ret = FALSE;
    U16 mu16Addr;


    if( mu16Day < 1 )
    {
        return FALSE;
    }

    mu16Addr = NFC_TOTAL_AMOUNT_COLD;
    mu16Addr += (mu16Day - 1) * sizeof( NFC_WaterHis_T );
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr, pData->Cold );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 1U, pData->Hot );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 2U, pData->Room );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

// mu8StartDay : 1 ~ 60
// mu16Num : 1 ~ 60
U8 NFC_ReadWaterHistory(U16 mu16StartDay,  U16 mu16Num, NFC_WaterHis_T *pData)
{
    U8 mu8Ret;
    U16 mu16Len;
    U16 mu16Addr;


    if( mu16StartDay < 1 )
    {
        return FALSE;
    }
    
    mu16Addr = NFC_TOTAL_AMOUNT_COLD;
    mu16Addr += (mu16StartDay - 1) * sizeof( NFC_WaterHis_T );
    mu16Len = mu16Num * (U16)mu16StartDay;
    mu8Ret = HAL_NFC_SeqRead( mu16Addr, (U8 *)pData, mu16Len );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}


// Ster History
U8 NFC_InitSterHistory(void)
{
    return WritePageDummyData( NFC_STER_HIS_YEAR, 0x02F0U, 0xFFU );
}

U8 NFC_WriteSterHistory(U16 mu16Day, NFC_SterHis_T *pData)
{
    U8 mu8Ret;
    U8 mu8Buf[ 4 ];
    U16 mu16Offset;
    

    mu8Buf[ 0 ] = pData->Year;
    mu8Buf[ 1 ] = pData->Month;
    mu8Buf[ 2 ] = pData->Date;
    mu8Buf[ 3 ] = pData->Type;

    mu16Offset = mu16Day * sizeof( NFC_SterHis_T );
    mu8Ret = HAL_NFC_PageWrite( NFC_STER_HIS_YEAR + mu16Offset, mu8Buf, 4 );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

// mu8StartDay : 1 ~ 80
// mu16Num : 1 ~ 80
U8 NFC_ReadSterHistory(U16 mu16StartDay, U16 mu16Num, NFC_SterHis_T *pData)
{
    U8 mu8Ret;
    U16 mu16Len;
    U16 mu16Offset;
    
    mu16Offset = (mu16StartDay - 1) * sizeof( NFC_SterHis_T );
    mu16Len = mu16Num * mu16StartDay;
    mu8Ret = HAL_NFC_SeqRead( NFC_STER_HIS_YEAR + mu16Offset, (U8 *)pData, mu16Len ); 
    return mu8Ret;
}


// Error History
U8 NFC_InitErrorHistory(void)
{
    return WritePageDummyData( NFC_ERROR_HIS_YEAR, 0x17B0U, 0xFFU );
}

U8 NFC_WriteErrorHistory(U16 mu16Count, NFC_ErrorHis_T *pData)
{
    U8 mu8Ret = FALSE;
    U16 mu16Addr;


    mu16Addr = NFC_ERROR_HIS_YEAR;
    mu16Addr += mu16Count * sizeof( NFC_ErrorHis_T );
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr, pData->Year );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 1U, pData->Month );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 2U, pData->Date );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 3U, pData->Hour );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 4U, pData->Error );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

U8 NFC_ReadErrorHistory(U16 mu16StartDay, U16 mu16Num, NFC_ErrorHis_T *pData)
{
    U8 mu8Ret;
    U16 mu16Len;
    U16 mu16Addr;
    

    mu16Addr = NFC_ERROR_HIS_YEAR;
    mu16Addr += ((mu16StartDay - 1) * sizeof( NFC_ErrorHis_T ));
    mu16Len = mu16Num * (U16)mu16StartDay;
    mu8Ret = HAL_NFC_SeqRead( mu16Addr, (U8 *)pData, mu16Len );

    return mu8Ret;
}

// Error State
U8 NFC_InitErrorState(U8 mu8Val)
{
    NFC_ErrorState_T mError;

    mError.Error = mu8Val;

    return NFC_WriteErrorState( &mError );
}

U8 NFC_WriteErrorState(NFC_ErrorState_T *pData)
{
    U8 mu8Ret;
    

    mu8Ret = HAL_NFC_ByteWrite( NFC_ERROR_CODE, pData->Error );

    return mu8Ret;
}

U8 NFC_ReadErrorState(NFC_ErrorState_T *pData)
{
    U8 mu8Ret;
    U8 mu8Data;
    

    mu8Ret = HAL_NFC_ByteRead( NFC_ERROR_CODE, &mu8Data );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    pData->Error = mu8Data;

    return TRUE;
}


// Service State
U8 NFC_InitServiceState(U8 mu8Val)
{
    NFC_ServiceState_T mService;


    mService.CheckDay = mu8Val;
    return NFC_WriteServiceState(  &mService );
}

U8 NFC_WriteServiceState(NFC_ServiceState_T *pData)
{
    U8 mu8Ret;
    

    mu8Ret = HAL_NFC_ByteWrite( NFC_SERVICE_CHECK, pData->CheckDay );

    return mu8Ret;
}

U8 NFC_ReadServiceState(NFC_ServiceState_T *pData)
{
    U8 mu8Ret;
    U8 mu8Data;
    

    mu8Ret = HAL_NFC_ByteRead( NFC_SERVICE_CHECK, &mu8Data );
    if( mu8Ret == TRUE )
    {
        pData->CheckDay  = mu8Data;
    }

    return mu8Ret;
}


// User History
U8 NFC_InitUserHistory(void)
{
    return WritePageDummyData( NFC_USER_HIS_YEAR, 0x1EBCU, 0xFFU );
}

U8 NFC_WriteUserHistory(U16 mu16Count, NFC_UserHis_T *pData)
{
    U8 mu8Ret = FALSE;
    U16 mu16Addr;


    mu16Addr = NFC_USER_HIS_YEAR;
    mu16Addr += (U16)mu16Count * sizeof( NFC_UserHis_T );
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr, pData->Year );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 1, pData->Month );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 2, pData->Date );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 3, pData->Hour );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 4, pData->User );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 5, pData->WaterType );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }
    mu8Ret = HAL_NFC_ByteWrite( mu16Addr + 6, pData->WaterAmount );
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    return TRUE;
}

U8 NFC_ReadUserHistory(U16 mu16StartDay, U16 mu16Num, NFC_UserHis_T *pData)
{
    U8 mu8Ret;
    U16 mu16Len;
    U16 mu16Addr;
    
    mu16Addr = NFC_USER_HIS_YEAR;
    mu16Addr += ((mu16StartDay - 1) * sizeof( NFC_UserHis_T ));
    mu16Len = mu16Num * (U16)mu16StartDay;
    mu8Ret = HAL_NFC_SeqRead( mu16Addr, (U8 *)pData, mu16Len );

    return mu8Ret;
}


U8 NFC_EraseAll(void)
{
    return WritePageDummyData( 0x0000U, 0x1E8CU, 0xFFU);
}


