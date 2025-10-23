#include "key_fct_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "display.h"
#include "sound.h"
#include "process_fct.h"

static void SetKeyBit(U32 maskBit);
static U8 FCT_Ice(void);
static U8 FCT_Hot(void);
static U8 FCT_Room(void);
static U8 FCT_Cold(void);
static U8 FCT_Amount(void);
static U8 FCT_IceWater(void);
static U8 FCT_IceWaterAmount(void);
static U8 FCT_IceLock(void);
static U8 FCT_IceWaterLock(void);
static U8 FCT_HotLock(void);

/* FCT */
const static SKeyEventList_T FCT_KeyEventList[] =
{
    /* KEY,               Short,              2sec,   3sec,  5sec,  7sec,  Pop,  TS */
    /* SINGLE KEY */
    { K_ICE,              FCT_Ice,            NULL,   NULL, NULL,   NULL, NULL, NULL },
    { K_HOT,              FCT_Hot,            NULL,   NULL, NULL,   NULL, NULL, NULL },  
    { K_ROOM,             FCT_Room,           NULL,   NULL, NULL,   NULL, NULL, NULL },
    { K_COLD,             FCT_Cold,           NULL,   NULL, NULL,   NULL, NULL, NULL },  
    { K_AMOUNT,           FCT_Amount,         NULL,   NULL, NULL,   NULL, NULL, NULL },
    { K_ICE_WATER,        FCT_IceWater,       NULL,   NULL, NULL,   NULL, NULL, NULL },
    { K_ICE_WATER_AMOUNT, FCT_IceWaterAmount, NULL,   NULL, NULL,   NULL, NULL, NULL },
    { K_ICE_LOCK,         FCT_IceLock,        NULL,   NULL, NULL,   NULL, NULL, NULL },
    { K_ICE_WATER_LOCK,   FCT_IceWaterLock,   NULL,   NULL, NULL,   NULL, NULL, NULL },
    { K_HOT_LOCK,         FCT_HotLock,        NULL,   NULL, NULL,   NULL, NULL, NULL },
};

U8 IsValidFctKeyCondition(U32 key)
{

    return TRUE;
}

void* GetFctKeyEventList(void)
{
    return (void *)FCT_KeyEventList;
}

U8 GetFctKeyEventListSize(void)
{
    return ( sizeof(FCT_KeyEventList) / sizeof(SKeyEventList_T) );
}

static void SetKeyBit(U32 maskBit)
{
    U32 mu32InputVal;


    mu32InputVal = GetFctTestInputVal();
    if( (mu32InputVal & maskBit ) != 0 )
    {
        ClearFctTestInputBit(maskBit);
    }
    else
    {
        SetFctTestInputBit(maskBit);
    }
}

static U8 FCT_Room(void)
{
    SetKeyBit(MK_FCT_KEY_ROOM);

    return SOUND_SELECT;
}

static U8 FCT_Cold(void)
{
    SetKeyBit(MK_FCT_KEY_COLD);

    return SOUND_SELECT;
}

static U8 FCT_Hot(void)
{
    SetKeyBit(MK_FCT_KEY_HOT);
    return SOUND_SELECT;
}

static U8 FCT_Amount(void)
{
    SetKeyBit(MK_FCT_KEY_AMOUNT);

    return SOUND_SELECT;
}

static U8 FCT_Ice(void)
{
    SetKeyBit(MK_FCT_KEY_ICE);

    return SOUND_SELECT;
}

static U8 FCT_IceWaterAmount(void)
{
    SetKeyBit(MK_FCT_KEY_ICE_WATER_AMOUNT);

    return SOUND_SELECT;
}

static U8 FCT_IceWater(void)
{
    SetKeyBit(MK_FCT_KEY_ICE_WATER);

    return SOUND_SELECT;
}

static U8 FCT_IceLock(void)
{
    SetKeyBit(MK_FCT_KEY_ICE_LOCK);

    return SOUND_SELECT;
}

static U8 FCT_IceWaterLock(void)
{
    SetKeyBit(MK_FCT_KEY_ICE_WATER_LOCK);

    return SOUND_SELECT;
}

static U8 FCT_HotLock(void)
{
    SetKeyBit(MK_FCT_KEY_HOT_LOCK);

    return SOUND_SELECT;
}
