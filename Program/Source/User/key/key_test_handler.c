/**
 * File : key_test_handler.c
 * 
 * Key Test Handler Function
*/
#include "key_test_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "display.h"
#include "sound.h"

static U8 TestHot(void);
static U8 TestRoom(void);
static U8 TestCold(void);
static U8 TestConty(void);
static U8 TestIceMake(void);
static U8 TestIceTurbo(void);
static U8 TestIceLock(void);
static U8 TestSter(void);
static U8 TestHotLock(void);
static U8 TestSave(void);


/* TEST */
SKeyEventList_T TEST_KeyEventList[] =
{
    /* KEY,           Short,        2sec,   3sec,  5sec,  7sec,   Pop,  TS */
    /* SINGLE KEY */
    { K_HOT,         TestHot,       NULL,   NULL,  NULL, NULL, NULL, NULL },
    { K_ROOM,        TestRoom,      NULL,   NULL,  NULL, NULL, NULL, NULL },
    { K_COLD,        TestCold,      NULL,   NULL,  NULL, NULL, NULL, NULL },
    { K_AMOUNT,      TestConty,     NULL,   NULL,  NULL, NULL, NULL, NULL },
    { K_ICE_OFF,     TestIceMake,   NULL,   NULL,  NULL, NULL, NULL, NULL },

    { K_ICE_LOCK,    TestIceLock,   NULL,   NULL,  NULL, NULL, NULL, NULL },
    { K_HOT_LOCK,    TestHotLock,   NULL,   NULL,  NULL, NULL, NULL, NULL },
};

U8 IsValidKeyTestCondition(U32 key)
{
    return TRUE;
}

void* GetTestKeyEventList(void)
{
    return (void *)TEST_KeyEventList;
}

U8 GetTestKeyEventListSize(void)
{
    return ( sizeof(TEST_KeyEventList) / sizeof(SKeyEventList_T) );
}

U8 TestHot(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispHotWater(cmd);

    return SOUND_SELECT;
}

U8 TestRoom(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispRoomWater( cmd );

    return SOUND_SELECT;
}

U8 TestCold(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispColdWater(cmd);

    return SOUND_SELECT;
}

U8 TestConty(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispConty( cmd );
    DispConty1L( cmd );
    DispConty1D5L( cmd );

    return SOUND_SELECT;
}

U8 TestIceMake(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispIce(cmd);
    DispIceMaker(cmd);
    DispIceFullHigh(cmd);
    DispIceOut(cmd);

    return SOUND_SELECT;
}

U8 TestIceTurbo(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispIceTurbo( cmd );

    return SOUND_SELECT;
}

U8 TestIceLock(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispLockIce( cmd );

    return SOUND_SELECT;
}

U8 TestSter(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispSter( cmd );
    DispDrain( cmd );
    if( cmd == ON )
    {
        //DispSterProgressBar( 100 );
    }
    else
    {
        //DispSterProgressBar( 0 );
    }

    return SOUND_SELECT;
}

U8 TestHotLock(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispHotLock(cmd);

    return SOUND_SELECT;
}

U8 TestSave(void)
{
    static U8 toggle = 1;
    U8 cmd = OFF;

    if( toggle )
    {
        cmd = ON;
    }
    toggle = !toggle;

    DispPowerSaving( cmd );
    //DispService( OFF );

    return SOUND_SELECT;
}

