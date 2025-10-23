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
    /* Short,               Long(2��),               Long(8��),       Pop  */
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

    // ���� ���� ���̸�, ���� ����
    if( GetWaterOut() == TRUE 
            && GetWaterOutContinue() == TRUE )
    {
        StopWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        return SOUND_EFFLUENT_END;
    }

    // ���� ���� ������ �߻��� ���� ���ؼ� �߰��� �ڵ�
    // �������� ���� �ü� ���� �������� water_out���� �߻���.
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

    // ��ũ ���� ���
    if( IsTankCleanMode() == TRUE )
    {
        SetTankClean( TRUE );
        StartDrainPumpOnLevel();
    }

    // ���� ������ ����
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

    /* ���� ��� ��, ���� ǥ�� */
    if( GetLockIce() == TRUE && GetIceWaterSelect() == SEL_ICE )
    {
        StartDisplayIceLock();
        return SOUND_ERROR;
    }
    
    /* ������ ��� ��, ���� ǥ�� */
    if( GetLockIceWater() == TRUE && GetIceWaterSelect() == SEL_ICE_WATER)
    {
        StartDisplayIceWaterLock();
        return SOUND_ERROR;
    }

    // ���� ���� ���̸�, ���� ����
    if( IsStartIceDoorWater() == TRUE 
            && GetIceWaterOutContinue() == TRUE )
    {
        CloseIceDoorWaterOut();
        StartDispTimerId(DISP_TIMER_ID_ERROR);
        return SOUND_EFFLUENT_END;
    }

#if( CONFIG_ENABLE_ICON_AIS_ICETRAY_TEST == 0 )
    // ���̽� Ʈ���� �׽�Ʈ
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

/* ICE DOOR - PUSHED LONG 2�� */
static U8 LeverOpenIceDoorLong(void)
{
    U8 mu8Sound = SOUND_NONE;

    /* ���� ���� ��, ���� ǥ�� */
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
    // EOL �׽�Ʈ ���� �ð��� ����Ǹ�,
    // ������ ���� Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    return SOUND_SELECT;
}

static U8 LeverEolStartLong(void)
{
    // EOL �׽�Ʈ ���� �ð��� ����Ǹ�,
    // ������ ���� Pass
    if( IsExpiredEolLimitTimer() == TRUE )
    {
        return SOUND_NONE;
    }

    // ���� EOL �׽�Ʈ ���̸�, Pass
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

    // EOL ����
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

