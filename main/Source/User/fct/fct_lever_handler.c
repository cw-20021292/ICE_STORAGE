#include "lever_handler.h"
#include "lever_common_handler.h"

#include "fct.h"
#include "sound.h"

/* WATER OUT */
static U8 LeverOpenTapWater(void);
static U8 LeverCloseTapWater(void);

/* ICE DOOR */
static U8 LeverOpenIceDoor(void);
static U8 LeverCloseIceDoor(void);

const static SLeverEventList_T FCT_LeverEventList[] =
{
    /* Short,               Long(2ì´?,     Long(8ì´?,     Pop  */
    {  LeverOpenTapWater,   NULL,         NULL,         LeverCloseTapWater  },
    {  LeverOpenIceDoor,    NULL,         NULL,         LeverCloseIceDoor   },

    // Lever Ice & Key Amout
    {  NULL,                NULL,         NULL,         NULL                }
};

/* WATER OUT - START */
static U8 LeverOpenTapWater(void)
{
    SetFctTestInputBit( MK_FCT_LEVER_WATER );

    return SOUND_SELECT;
}

static U8 LeverCloseTapWater(void)
{
    ClearFctTestInputBit( MK_FCT_LEVER_WATER );

    return SOUND_SELECT;
}


/* ICE DOOR - PUSHED( OPEN )*/
static U8 LeverOpenIceDoor(void)
{
    SetFctTestInputBit( MK_FCT_LEVER_ICE );

    return SOUND_SELECT;
}

static U8 LeverCloseIceDoor(void)
{
    ClearFctTestInputBit( MK_FCT_LEVER_ICE );

    return SOUND_SELECT;
}

void* GetFctLeverEventList(void)
{
    // return address of lever event table 
    return (void *)FCT_LeverEventList;
}

