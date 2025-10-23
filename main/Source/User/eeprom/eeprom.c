#include "eeprom.h"
#include "util.h"
#include "rtc.h"

#include "cold_water.h"
#include "hot_water.h"
#include "water_out.h"
#include "ice.h"
#include "power_saving.h"
#include "error.h"
#include "front.h"
#include "sound.h"
#include "service.h"
#include "health.h"
#include "ster.h" 
#include "drain_water.h" 
#include "util.h" 
#include "time_short.h" 
#include "process_display.h" 
#include "crc16.h" 
#include "rtc_comm.h"
#include "ice_water_out.h"
#include "smart_saving.h"

#define DEFAULT_EEPROM_INIT_FIRST_VAL    (0xAA)
#define DEFAULT_EEPROM_INIT_END_VAL      (0xBB)

#define EEP_ADDR_OFFSET                  (0x40)

typedef struct _eeprom_
{
    /* Init / Write  */
    U8 Init;
    U8 Write;
    U8 Erase;
    U8 FactoryReset;        // 공장 초기화 되면, TRUE 그렇지 않으면 FALSE

    U16 DelayTime;
    U8 ReadData[EEP_ID_MAX];
    U8 WriteData[EEP_ID_MAX];
} SEeprom_T;

SEeprom_T  Eeprom;

static U8 LoadData(void);
static U8 WriteData(void);

void InitEeprom(void)
{
    Eeprom.Init = FALSE;
    Eeprom.Erase = FALSE;
    Eeprom.FactoryReset = FALSE;

    if( LoadData() == FALSE )
    {
        Eeprom.FactoryReset = TRUE;
        WriteData();
    }

    // Write하고 Delay Time은 LoadData() 이후에 실행되어야 한다.
    // LoadData 동작 중에, Set함수들에 대해서 Eep.Write가 TRUE가되기 때문이다.
    // Eep.Write가 TRUE가되면 불필요한 eeprom write 동작이 한 번 실행되는 것을 막기 위해서이다.
    Eeprom.Write = FALSE;
    Eeprom.DelayTime = 0;
}

void SaveEepromId(EEepromId_T id)
{
    if( id < EEP_ID_MAX )
    {
        Eeprom.DelayTime = 1;  // 1sec..
        Eeprom.Write = TRUE;  
    }
}

void EraseEeprom(void)
{
    Eeprom.Write = TRUE;
    Eeprom.Erase = TRUE;
}


static U8 EepromPageWrite(U8 addr, U8 *pData)
{
    //if( IsRtcBatBackUpMode() == TRUE )
    //{
    //    return FALSE;
    //}

    addr += EEP_ADDR_OFFSET;
    return RTC_PageWrite(DEV_ADDR_EEP, addr, pData, EEP_PAGE_SIZE);
}

static U8 EepromSeqRead(U8 addr, U8 *pData, U8 length)
{
    //if( IsRtcBatBackUpMode() == TRUE )
    //{
    //    return FALSE;
    //}

    addr += EEP_ADDR_OFFSET;
    return RTC_SeqRead(DEV_ADDR_EEP, addr, pData, length);
}

static U8 LoadData(void)
{
    U8 mu8Ret;
    U16 mu16SrcChksum = 0U;
    U16 mu16MakeChksum = 0U;
    SMemento_T memento;

    // Read Data
    mu8Ret = EepromSeqRead(EEP_ID_INIT_FIRST, &Eeprom.ReadData[0], EEP_ID_MAX);
    if( mu8Ret == FALSE )
    {
        return FALSE;
    }

    // Checksum
    mu16SrcChksum  = GET_16_UINT_WORD( Eeprom.ReadData[ EEP_ID_CHKSUM_LOW], Eeprom.ReadData[EEP_ID_CHKSUM_HIGH] ); 
    mu16MakeChksum = crc16_cal( &Eeprom.ReadData[0], (EEP_ID_MAX - 2) );
    if( mu16SrcChksum != mu16MakeChksum )
    {
        return FALSE;
    }

    // Check 
    if( Eeprom.ReadData[ EEP_ID_INIT_FIRST ] != DEFAULT_EEPROM_INIT_FIRST_VAL 
            || Eeprom.ReadData[ EEP_ID_INIT_END ] != DEFAULT_EEPROM_INIT_END_VAL )
    {

        Eeprom.Init = FALSE;
        return FALSE;
    }

    Eeprom.Init = TRUE;

    // Load Data
    SetColdWaterConfigMake( Eeprom.ReadData[EEP_ID_CONF_MAKE_COLD] );
    SetHotWaterConfigMake( Eeprom.ReadData[EEP_ID_CONF_MAKE_HOT] );
    SetIceConfigMake( Eeprom.ReadData[EEP_ID_CONF_MAKE_ICE] );
    SetIceTurbo( Eeprom.ReadData[EEP_ID_CONF_TURBO] );
    SetSavingConfig( Eeprom.ReadData[EEP_ID_CONF_POWER_SAVING] );

    SetHealthMode( Eeprom.ReadData[EEP_ID_CONF_STER] );
    SetConfigDeco(DECO_LED_WELCOME_WATER, Eeprom.ReadData[EEP_ID_CONF_DECO_LED] );
    //SetWaterOutContinue( Eeprom.ReadData[EEP_ID_CONF_CONTY] );
    //SetWaterOutContinue( Eeprom.ReadData[EEP_ID_CONF_ICE_WATER_CONTY] );
    SetWaterOutFixedAmountType( Eeprom.ReadData[EEP_ID_CONF_FIXED_AMOUNT] );
    SetIceWaterOutFixedAmountType( Eeprom.ReadData[EEP_ID_CONF_ICE_WATER_FIXED_AMOUNT] );
    if( Eeprom.ReadData[EEP_ID_CONF_MUTE] != 0 )
    {
        SetSoundMute();
    }
    SetHotWaterAltidue( Eeprom.ReadData[EEP_ID_HOT_ALTITUDE] );

    SetWaterOutSelect( Eeprom.ReadData[EEP_ID_SEL_WATER] );
    SetIceWaterSelect( Eeprom.ReadData[EEP_ID_SEL_ICE_WATER] );
    ClearIceWaterOutReturnTime();
    ClearWaterOutReturnTime();
    //Eep.ReadData[EEP_ID_DO_ICING]
    SetLockAll( Eeprom.ReadData[EEP_ID_LOCK_ALL] );
    SetLockHot( Eeprom.ReadData[EEP_ID_LOCK_HOT] );
    SetLockIce( Eeprom.ReadData[EEP_ID_LOCK_ICE] );
    SetLockIceWater( Eeprom.ReadData[EEP_ID_LOCK_ICE_WATER] );
    SetLockIceWaterAmount( Eeprom.ReadData[EEP_ID_LOCK_ICE_WATER_AMOUNT] );
    
#if 0
    SetMementoError( 0, Eeprom.ReadData[ EEP_ID_MEMENTO_1 ] );
    SetMementoError( 1, Eeprom.ReadData[ EEP_ID_MEMENTO_2 ] );
    SetMementoError( 2, Eeprom.ReadData[ EEP_ID_MEMENTO_3 ] );
    SetMementoError( 3, Eeprom.ReadData[ EEP_ID_MEMENTO_4 ] );
    SetMementoError( 4, Eeprom.ReadData[ EEP_ID_MEMENTO_5 ] );
#endif
    
    SetServiceCheckDay( Eeprom.ReadData[ EEP_ID_SERVICE_CHECK_DAY ] );

    SetFactroySetup( Eeprom.ReadData[ EEP_ID_FACTORY_SETUP ] );
    SetRtcConfigInit( Eeprom.ReadData[ EEP_ID_TIME ] );

    SetUnusedSavingConfig( Eeprom.ReadData[ EEP_ID_CONF_UNUSED_SAVE ] );

    //SetSterPassDay( Eeprom.ReadData[EEP_ID_STER_DAY_PASS] );
    //SetSterPeriodDay ( Eeprom.ReadData[EEP_ID_STER_PERIOD] );
    //SetHealthAdjustMode ( Eeprom.ReadData[EEP_ID_STER_ADJUST_MODE] );
    //SetHealthHourConfig ( Eeprom.ReadData[EEP_ID_STER_SET_HOUT_CONFIG] );
    
    //SetSterPeriodMode( Eeprom.ReadData[ EEP_ID_STER_PERIOD_AUTO_CHANGE ] );
    //SetHealthHour( Eeprom.ReadData[ EEP_ID_STER_RESERVATION_HOUR ] );
    //SetHealthMin( Eeprom.ReadData[ EEP_ID_STER_RESERVATION_MIN ] );
    //SetSterGoing( Eeprom.ReadData[ EEP_ID_STER_DOING ] );
    //SetSterErrWaterOut( Eeprom.ReadData[ EEP_ID_STER_ERR_WATER_OUT ] );

    return TRUE;
}

static U8 WriteData(void)
{
    U16 mu16CheckSum = 0U;
    
    U8 waterSel = SEL_WATER_ROOM;
    U8 iceWaterSel = SEL_ICE;

    // Make Data..
    Eeprom.WriteData[EEP_ID_INIT_FIRST]         = DEFAULT_EEPROM_INIT_FIRST_VAL;

    Eeprom.WriteData[EEP_ID_CONF_MAKE_COLD]     = GetColdWaterConfigMake();
    Eeprom.WriteData[EEP_ID_CONF_MAKE_HOT]      = GetHotWaterConfigMake();   
    Eeprom.WriteData[EEP_ID_CONF_MAKE_ICE]      = GetIceConfigMake();
    Eeprom.WriteData[EEP_ID_CONF_TURBO]         = GetIceTurbo();
    Eeprom.WriteData[EEP_ID_CONF_POWER_SAVING]   = GetSavingConfig();

    Eeprom.WriteData[EEP_ID_CONF_STER]          = GetHealthMode();       
    Eeprom.WriteData[EEP_ID_CONF_DECO_LED]      = GetConfigDeco(DECO_LED_WELCOME_WATER);
    Eeprom.WriteData[EEP_ID_CONF_CONTY]         = GetWaterOutContinue();
    Eeprom.WriteData[EEP_ID_CONF_ICE_WATER_CONTY] = GetIceWaterOutContinue();
    Eeprom.WriteData[EEP_ID_CONF_FIXED_AMOUNT]  = GetWaterOutFixedAmountType();
    Eeprom.WriteData[EEP_ID_CONF_ICE_WATER_FIXED_AMOUNT] = GetIceWaterOutFixedAmountType();
    Eeprom.WriteData[EEP_ID_CONF_MUTE]          = GetSoundMute();       
    Eeprom.WriteData[EEP_ID_HOT_ALTITUDE]       = GetHotWaterAltidue();

    waterSel = GetWaterOutSelect();
    if( waterSel == SEL_WATER_HOT )
    {
        waterSel = SEL_WATER_ROOM;
    }
    else if( waterSel == SEL_WATER_COLD && GetColdWaterConfigMake() == FALSE ) 
    {
        waterSel = SEL_WATER_ROOM;
    }
    Eeprom.WriteData[EEP_ID_SEL_WATER]          = waterSel;       

    iceWaterSel = GetIceWaterSelect();
    if( iceWaterSel == SEL_ICE_WATER )
    {
        iceWaterSel = SEL_ICE;
    }
    Eeprom.WriteData[EEP_ID_SEL_ICE_WATER]      = iceWaterSel;       

    Eeprom.WriteData[EEP_ID_DO_ICING]           = 0;//GetIceConfigMake();
    Eeprom.WriteData[EEP_ID_LOCK_ALL]           = GetLockAll();
    Eeprom.WriteData[EEP_ID_LOCK_HOT]           = GetLockHot();
    Eeprom.WriteData[EEP_ID_LOCK_ICE]           = GetLockIce();
    Eeprom.WriteData[EEP_ID_LOCK_ICE_WATER]     = GetLockIceWater();
    Eeprom.WriteData[EEP_ID_LOCK_ICE_WATER_AMOUNT]     = GetLockIceWaterAmount();

#if 0
    Eeprom.WriteData[EEP_ID_MEMENTO_1]          = GetMementoError( 0 );
    Eeprom.WriteData[EEP_ID_MEMENTO_2]          = GetMementoError( 1 );
    Eeprom.WriteData[EEP_ID_MEMENTO_3]          = GetMementoError( 2 );
    Eeprom.WriteData[EEP_ID_MEMENTO_4]          = GetMementoError( 3 );
    Eeprom.WriteData[EEP_ID_MEMENTO_5]          = GetMementoError( 4 );
#endif

    Eeprom.WriteData[EEP_ID_SERVICE_CHECK_DAY]  = GetServiceCheckDay();

    Eeprom.WriteData[EEP_ID_FACTORY_SETUP]  = GetFactorySetup();
    Eeprom.WriteData[EEP_ID_TIME]  = GetRtcConfigInit();

    //Eeprom.WriteData[EEP_ID_STER_DAY_PASS]        = GetSterPassDay();
    //Eeprom.WriteData[EEP_ID_STER_PERIOD]          = GetSterPeriodDay();
    //Eeprom.WriteData[EEP_ID_STER_ADJUST_MODE] = GetHealthAdjustMode();
    //Eeprom.WriteData[EEP_ID_STER_SET_HOUT_CONFIG] = GetHealthHourConfig();
    
    //Eeprom.WriteData[EEP_ID_STER_PERIOD_AUTO_CHANGE] = GetSterPeriodMode();
    //Eeprom.WriteData[EEP_ID_STER_RESERVATION_HOUR]   = GetHealthHour();
    //Eeprom.WriteData[EEP_ID_STER_RESERVATION_MIN]    = GetHealthMin();
    //Eeprom.WriteData[EEP_ID_STER_DOING]              = GetSterGoing();
    //Eeprom.WriteData[EEP_ID_STER_ERR_WATER_OUT]      = GetSterErrWaterOut();

    Eeprom.WriteData[EEP_ID_CONF_UNUSED_SAVE]  = GetUnusedSavingConfig();
    
    // End
    Eeprom.WriteData[EEP_ID_INIT_END]           = DEFAULT_EEPROM_INIT_END_VAL;

    // Checksum
    mu16CheckSum = crc16_cal( &Eeprom.WriteData[0], (EEP_ID_MAX - 2) );
    Eeprom.WriteData[EEP_ID_CHKSUM_HIGH] = GET_16_HIGH_BYTE( mu16CheckSum );
    Eeprom.WriteData[EEP_ID_CHKSUM_LOW] = GET_16_LOW_BYTE( mu16CheckSum );

    // Write data..
    {
        U8 i;
        U8 mu8PageNum;

        mu8PageNum = EEP_ID_MAX + EEP_PAGE_SIZE;
        mu8PageNum /= EEP_PAGE_SIZE;
        for( i = 0; i < mu8PageNum; i++ )
        {
            U8 mu8Index;

            mu8Index = i * EEP_PAGE_SIZE;
            EepromPageWrite( mu8Index, &Eeprom.WriteData[ mu8Index ] );
        }
    }

    return TRUE;
}

void ProcessEeprom(void)
{
    // TimeShort 모드에서는 eeprom 기능 비활성화
    // 단, 메멘토 모드인 경우에는 허용한다.
    if( GetTimeShortStatus() == TRUE 
    && GetMementoDisp() == FALSE )
    {
        Eeprom.Write = FALSE;
        return;
    }

    // Write 명령 조건 검사
    if( Eeprom.Write == FALSE )
    {
        return;
    }

    // Write 시간 지연 확인
    if( Eeprom.DelayTime != 0 )
    {
        Eeprom.DelayTime--;
        return ;
    }

    // Write Data
    //if( IsRtcBatBackUpMode() == FALSE )
    {
        if( Eeprom.Erase == TRUE )
        {
            // eeprom 초기화는 초기화 후, 시스템 RESET
            if( RTC_EEPROM_AllErase() == TRUE )
            {
                Eeprom.Write = FALSE;
                Eeprom.Erase = FALSE;
                Reset();
            }
        }
        else
        {
            WriteData();
            Eeprom.Write = FALSE;
        }
    }

}

U8 TestEeprom(void)
{
    U8 mu8Ret = 0;
    U8 mu8Read = 0;

    // 0x00 번지에 0x12값을 라이팅 하고 다시 읽어봄으로써 
    // eeprom 정상동작이 되는지 확인함...
    RTC_ByteWrite( DEV_ADDR_EEP, 0x40, 0x12 );
    RTC_ByteRead( DEV_ADDR_EEP, 0x40, &mu8Read );

    if( mu8Read == 0x12 )
    {
        return TRUE;
    }

    return FALSE;
}

