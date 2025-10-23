#include "lever_handler.h"
#include "lever_common_handler.h"

#include "ice_door.h"
#include "water_out.h"
#include "tank_clean.h"
#include "front.h"
#include "process_display.h"
#include "sound.h"
#include "drain.h"
#include "ice.h"
#include "ice_tray_test.h"
#include "health.h"
#include "cold_water.h"
#include "display_lib.h"
#include "ster.h"
#include "eol.h"
#include "service.h"
#include "ice_water_out.h"

/* WATER OUT */
static U8 LeverOpenTapWater(void);
static U8 LeverCloseTapWater(void);

/* ICE DOOR */
static U8 LeverOpenIceDoor(void);
static U8 LeverOpenIceDoorLong(void);
static U8 LeverCloseIceDoor(void);

/* Lever Ice & Key Amount */
static U8 LeverEolStart(void);
static U8 LeverEolStartLong(void);

const static SLeverEventList_T LeverEventList[] =
{
    /* Short,               Long(2초),               Long(8초),       Pop  */
    {  LeverOpenTapWater,   NULL,                   NULL,           LeverCloseTapWater  },
    {  LeverOpenIceDoor,    LeverOpenIceDoorLong,   NULL,           LeverCloseIceDoor   },

    // Lever Ice & Key Amout
    {  LeverEolStart,       LeverEolStartLong,      NULL,           NULL   }
};

/* WATER OUT - START */
static U8 LeverOpenTapWater(void)
{
    U8 mu8Sound = SOUND_EFFLUENT;

    if( IsStartIceDoorWater() == TRUE 
          && GetIceWaterOutContinue() == TRUE )
    {
        // Blink
    }
    else
    {
        SelectWaterPanel();
    }

    // 연속 추출 중이면, 추출 중지
    if( GetWaterOut() == TRUE 
            && GetWaterOutContinue() == TRUE )
    {
        StopWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        return SOUND_EFFLUENT_END;
    }

    // 이중 추출 종료음 발생을 막기 위해서 추가된 코드
    // 저수위에 대한 냉수 추출 종료음은 water_out에서 발생함.
    if( IsTankCleanMode() == FALSE )
    {
        if( GetWaterOutSelect() == SEL_WATER_COLD 
                && GetColdWaterLevel() == LEVEL_LOW )
        {
            return SOUND_EFFLUENT;
        }
    }


#if CONFIG_STER
    if( GetSterErrWaterOut() == TRUE )
    {
        return SOUND_ERROR;
    }
#endif

    // 탱크 비우기 모드
    if( IsTankCleanMode() == TRUE )
    {
        SetTankClean( TRUE );
        StartDrainPumpOnLevel();
    }

    // 연속 추출음 설정
    if( GetWaterOutContinue() == TRUE 
            && GetWaterOutAmoutType() == FIXED_AMOUNT_INFINITY )
    {
        mu8Sound = SOUND_EFFLUENT_CONTINUE;
    }
    
    StartWaterOut();
    return mu8Sound;
}

/* WATER OUT - STOP */
static U8 LeverCloseTapWater(void)
{
    if( GetWaterOutContinue() != TRUE )
    {
        StopWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        return SOUND_EFFLUENT_END;
    }

    return SOUND_NONE;
}


/* ICE DOOR - PUSHED( OPEN )*/
static U8 LeverOpenIceDoor(void)
{
    if( GetWaterOut() == TRUE 
            && GetWaterOutContinue() == TRUE )
    {
        // Blink
    }
    else
    {
        SelectIcePanel();
    }

    /* 얼음 잠금 시, 에러 표시 */
    if( GetLockIce() == TRUE && GetIceWaterSelect() == SEL_ICE )
    {
        StartDisplayIceLock();
        return SOUND_ERROR;
    }
    
    /* 얼음물 잠금 시, 에러 표시 */
    if( GetLockIceWater() == TRUE && GetIceWaterSelect() == SEL_ICE_WATER)
    {
        StartDisplayIceWaterLock();
        return SOUND_ERROR;
    }

    // 연속 추출 중이면, 추출 중지
    if( IsStartIceDoorWater() == TRUE 
            && GetIceWaterOutContinue() == TRUE )
    {
        CloseIceDoorWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        return SOUND_EFFLUENT_END;
    }

#if( CONFIG_ENABLE_ICON_AIS_ICETRAY_TEST == 0 )
    // 아이스 트레이 테스트
    if( IsIceTrayTest() == TRUE )
    {
        StartIceTrayTest();
    }
#endif

    if( GetIceWaterSelect() == SEL_ICE )
    {
        OpenIceDoor();
    }
    else if( GetIceWaterSelect() == SEL_ICE_WATER )
    {
        OpenIceDoorWaterOut();
        return SOUND_EFFLUENT;
    }

    return SOUND_SELECT;
}

/* ICE DOOR - PUSHED LONG 2초 */
static U8 LeverOpenIceDoorLong(void)
{
    U8 mu8Sound = SOUND_NONE;

    /* 얼음 꺼짐 시, 에러 표시 */
    if( GetIceConfigMake() == FALSE )
    {
        StartDisplayIceMake();
        mu8Sound = SOUND_ERROR;
    }

    return mu8Sound;
}

/*ICE DOOR - POP ( CLOSE ) */
static U8 LeverCloseIceDoor(void)
{
    if( GetIceWaterSelect() == SEL_ICE )
    {
        CloseIceDoor();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        return SOUND_EFFLUENT_END;
    }
    else if( GetIceWaterSelect() == SEL_ICE_WATER )
    {
        if( GetIceWaterOutContinue() != TRUE )
        {
            CloseIceDoorWaterOut();
            StartDispTimerId(DISP_TIMER_ID_ERROR);
            return SOUND_EFFLUENT_END;
        }
    }
    
    return SOUND_NONE;
}



/* ICE & KEY AMOUNT - PUSHED( OPEN )*/
static U8 LeverEolStart(void)
{
    // EOL 테스트 진입 시간이 만료되면,
    // 부저음 없이 Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    return SOUND_SELECT;
}

static U8 LeverEolStartLong(void)
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

#if 0
    if( GetServiceCheckTankOpen() == FALSE )
    {
        return SOUND_ERROR;
    }
#endif

    // EOL 시작
    StartEol( EOL_TYPE_LOAD );
    StartDisplayInit();
    SetVersionDisp( 0 );

    return SOUND_SETUP;
}

void* GetLeverEventList(void)
{
    // Return address of lever event table 
    return (void *)LeverEventList;
}

