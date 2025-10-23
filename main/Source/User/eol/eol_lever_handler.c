#include "eol_lever_handler.h"
#include "lever_common_handler.h"

#include "eol.h"
#include "ice_door.h"
#include "water_out.h"
#include "ice_water_out.h"
#include "sound.h"
#include "process_display.h"
#include "display_lib.h"

/* WATER OUT */
static U8 LeverOpenTapWater(void);
static U8 LeverCloseTapWater(void);

/* ICE DOOR */
static U8 LeverOpenIceDoor(void);
static U8 LeverCloseIceDoor(void);

static U8 LeverEolTestSeal(void)
{
    StartDisplayInit();
    SetEolMode(EOL_MODE_SEALED_INIT);

    return SOUND_SELECT;
}

const static SLeverEventList_T EOL_LeverEventList[] =
{
    /* Short,               Long(2√ )    Long(8√ )      Pop  */
    {  LeverOpenTapWater,   NULL,       NULL,         LeverCloseTapWater  },
    {  LeverOpenIceDoor,    NULL,       NULL,         LeverCloseIceDoor   },

    // Lever Ice & Key Amout
    {  LeverEolTestSeal,    NULL,       NULL,         NULL                }
};

/* WATER OUT - START */
static U8 LeverOpenTapWater(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        if( (GetEolTestInputVal() & MK_EOL_LEVER_WATER ) != 0UL )
        {
            ClearEolTestInputBit( MK_EOL_LEVER_WATER );
        }
        else
        {
            SetEolTestInputBit( MK_EOL_LEVER_WATER );
        }
        return SOUND_SELECT;
    }

    if( GetEolType() == EOL_TYPE_MAIN_FRONT )
    {
        return SOUND_SELECT;
    }

    if( GetEolType() == EOL_TYPE_SUB_FRONT )
    {
        return SOUND_SELECT;
    }

    if( GetEolMode() != EOL_MODE_READY 
            && GetEolMode() != EOL_MODE_DONE )
    {
        return SOUND_NONE;
    }

    SetEolTestInputBit( MK_EOL_LEVER_WATER );
#if 0
    SetEolTestInputBit( MK_EOL_LEVER_WATER_DP );
#endif
    StartWaterOut();
    return SOUND_SELECT;
}

/* WATER OUT - STOP */
static U8 LeverCloseTapWater(void)
{   
    if( GetEolType() == EOL_TYPE_LOAD )
    {
        StopWaterOut();

        if( GetWaterOutSelect() == SEL_WATER_HOT )
        {
            if( (GetEolTestInputVal() & MK_EOL_KEY_HOT_DP ) != 0UL )
            {
                SetEolTestInputBit( MK_EOL_KEY_HOT_OUT_END );
            }
        }
        else if( GetWaterOutSelect() == SEL_WATER_COLD )
        {
            if( (GetEolTestInputVal() & MK_EOL_KEY_COLD_DP ) != 0UL )
            {
                SetEolTestInputBit( MK_EOL_KEY_COLD_OUT_END );
            }
        }
        else
        {
            if( (GetEolTestInputVal() & MK_EOL_KEY_ROOM_DP ) != 0UL )
            {
                SetEolTestInputBit( MK_EOL_KEY_ROOM_OUT_END );
            }
        }

        ClearEolTestInputBit( MK_EOL_KEY_HOT_DP );
        ClearEolTestInputBit( MK_EOL_KEY_ROOM_DP );
        ClearEolTestInputBit( MK_EOL_KEY_COLD_DP );
        
    }

    //ClearEolTestInputBit( MK_EOL_LEVER_WATER );
    return SOUND_NONE;
}


/* ICE DOOR - PUSHED( OPEN )*/
static U8 LeverOpenIceDoor(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        if( (GetEolTestInputVal() & MK_EOL_LEVER_ICE ) != 0 )
        {
            ClearEolTestInputBit( MK_EOL_LEVER_ICE );
        }
        else
        {
            SetEolTestInputBit( MK_EOL_LEVER_ICE );
        }
        return SOUND_SELECT;
    }

    if( GetEolType() == EOL_TYPE_MAIN_FRONT )
    {
        return SOUND_SELECT;
    }

    if( GetEolType() == EOL_TYPE_SUB_FRONT )
    {
        return SOUND_SELECT;
    }

    if( GetEolMode() != EOL_MODE_READY 
            && GetEolMode() != EOL_MODE_DONE )
    {
        return SOUND_NONE;
    }

    SetEolTestInputBit( MK_EOL_LEVER_ICE );
#if 0
    SetEolTestInputBit( MK_EOL_LEVER_ICE_DP );
#endif

    if( GetIceWaterSelect() == SEL_ICE )
    {
        OpenIceDoor();
    }
    else if( GetIceWaterSelect() == SEL_ICE_WATER )
    {
        OpenIceDoorWaterOut();
    }
    return SOUND_SELECT;
}

/*ICE DOOR - POP ( CLOSE ) */
static U8 LeverCloseIceDoor(void)
{
    if( GetEolType() == EOL_TYPE_LOAD )
    {
        if( GetIceWaterSelect() == SEL_ICE )
        {
            CloseIceDoor();
            return SOUND_EFFLUENT_END;
        }
        else if( GetIceWaterSelect() == SEL_ICE_WATER )
        {
            if( GetIceWaterOutContinue() != TRUE )
            {
                CloseIceDoorWaterOut();
                StartDispTimerId(DISP_TIMER_ID_ERROR);

                if( GetEolMode() == EOL_MODE_DONE )
                {
                    ClearEolTestInputBit( MK_EOL_KEY_ICE_WATER_DONE );
                }
                return SOUND_EFFLUENT_END;
            }
        }
       
#if 0
        ClearEolTestInputBit( MK_EOL_LEVER_ICE_DP );
#endif
    }
    
    return SOUND_NONE;
}

void* GetEolLeverEventList(void)
{
    // return address of lever event table 
    return (void *)EOL_LeverEventList;
}
