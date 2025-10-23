#include "key_eol_handler.h"
#include "key.h"
#include "key_common_handler.h"
#include "display.h"
#include "sound.h"
#include "drain_water.h"
#include "water_out.h"
#include "ice_water_out.h"
#include "process_eol.h"
#include "process_display.h"
#include "valve.h"
#include "circulate_pump.h"
#include "gas_switch_valve.h"
#include "hotgas_switch_valve.h"

static void SetKeyBit(U32 mu32MaskBit );

/* EOL */
// KEY EVENT LIST - EOL FRONT TEST 
static U8 EOL_Ice(void);
static U8 EOL_Room(void);
static U8 EOL_Cold(void);
static U8 EOL_Hot(void);
static U8 EOL_Amount(void);
static U8 EOL_IceWater(void);
static U8 EOL_IceWaterAmount(void);
static U8 EOL_IceLock(void);
static U8 EOL_IceWaterLock(void);
static U8 EOL_HotLock(void);

/* Front 검사 */
const static SKeyEventList_T EOL_KeyFrontEventList[] =
{
    /* KEY,               Short,              2sec,   3sec,  5sec,  7sec,  Pop,  TS */
    /* SINGLE KEY */
    { K_ICE,              EOL_Ice,            NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_HOT,              EOL_Hot,            NULL,   NULL,  NULL,  NULL,  NULL, NULL },   
    { K_ROOM,             EOL_Room,           NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_COLD,             EOL_Cold,           NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_AMOUNT,           EOL_Amount,         NULL,   NULL,  NULL,  NULL,  NULL, NULL },    
    { K_ICE_WATER,        EOL_IceWater,       NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_ICE_WATER_AMOUNT, EOL_IceWaterAmount, NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_ICE_LOCK,         EOL_IceLock,        NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_ICE_WATER_LOCK,   EOL_IceWaterLock,   NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_HOT_LOCK,         EOL_HotLock,        NULL,   NULL,  NULL,  NULL,  NULL, NULL },
};

// KEY EVENT LIST - EOL MAIN FRONT TEST 
static U8 EOL_MainIce(void);
static U8 EOL_MainRoom(void);
static U8 EOL_MainCold(void);
static U8 EOL_MainHot(void);
static U8 EOL_MainAmount(void);
static U8 EOL_MainIceWater(void);
static U8 EOL_MainIceWaterAmount(void);
static U8 EOL_MainIceLock(void);
static U8 EOL_MainIceWaterLock(void);
static U8 EOL_MainHotLock(void);

const static SKeyEventList_T EOL_KeyMainFrontEventList[] =
{
    /* KEY,               Short,                 2sec,   3sec,  5sec,  7sec,  Pop,  TS */
    /* SINGLE KEY */
    { K_ICE,              EOL_MainIce,            NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼음 선택
    { K_HOT,              EOL_MainHot,            NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 온수 선택
    { K_ROOM,             EOL_MainRoom,           NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 정수 선택
    { K_COLD,             EOL_MainCold,           NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 냉수 선택
    { K_AMOUNT,           EOL_MainAmount,         NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 용량 선택
    { K_ICE_WATER,        EOL_MainIceWater,       NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물  선택
    { K_ICE_WATER_AMOUNT, EOL_MainIceWaterAmount, NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물  용량 선택
    { K_ICE_LOCK,         EOL_MainIceLock,        NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을잠금  선택
    { K_ICE_WATER_LOCK,   EOL_MainIceWaterLock,   NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물잠금  선택
    { K_HOT_LOCK,         EOL_MainHotLock,        NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 온수잠금  선택
};

// KEY EVENT LIST - EOL SUB FRONT TEST 
static U8 EOL_SubIce(void);
static U8 EOL_SubRoom(void);
static U8 EOL_SubCold(void);
static U8 EOL_SubHot(void);
static U8 EOL_SubAmount(void);
static U8 EOL_SubIceWater(void);
static U8 EOL_SubIceWaterAmount(void);
static U8 EOL_SubIceLock(void);
static U8 EOL_SubIceWaterLock(void);
static U8 EOL_SubHotLock(void);

/* Sub Front 검사 */
const static SKeyEventList_T EOL_KeySubFrontEventList[] =
{
    /* KEY,               Short,              2sec,   3sec,  5sec,  7sec,  Pop,  TS */
    /* SINGLE KEY */
    { K_ICE,              EOL_SubIce,            NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_HOT,              EOL_SubHot,            NULL,   NULL,  NULL,  NULL,  NULL, NULL },   
    { K_ROOM,             EOL_SubRoom,           NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_COLD,             EOL_SubCold,           NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_AMOUNT,           EOL_SubAmount,         NULL,   NULL,  NULL,  NULL,  NULL, NULL },    
    { K_ICE_WATER,        EOL_SubIceWater,       NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_ICE_WATER_AMOUNT, EOL_SubIceWaterAmount, NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_ICE_LOCK,         EOL_SubIceLock,        NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_ICE_WATER_LOCK,   EOL_SubIceWaterLock,   NULL,   NULL,  NULL,  NULL,  NULL, NULL },
    { K_HOT_LOCK,         EOL_SubHotLock,        NULL,   NULL,  NULL,  NULL,  NULL, NULL },
};

// KEY EVENT LIST - EOL LOAD TEST 
static U8 EOL_SelIce(void);
static U8 EOL_SelHot(void);
static U8 EOL_SelRoom(void);
static U8 EOL_SelCold(void);
static U8 EOL_SelAmount(void);
static U8 EOL_SelIceWater(void);
static U8 EOL_SelIceWaterAmount(void);
static U8 EOL_SelIceLock(void);
static U8 EOL_SelIceWaterLock(void);
static U8 EOL_SelHotLock(void);
static U8 EOL_Circulate(void);
static U8 EOL_TestSeal(void);
const static SKeyEventList_T EOL_KeyLoadEventList[] =
{
    /* KEY,               Short,              2sec,   3sec,  5sec,  7sec,  Pop,  TS */
    /* SINGLE KEY */
    { K_ICE,              EOL_SelIce,            NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼음 선택
    { K_HOT,              EOL_SelHot,            NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 온수 선택
    { K_ROOM,             EOL_SelRoom,           NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 정수 선택
    { K_COLD,             EOL_SelCold,           NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 냉수 선택
    { K_AMOUNT,           EOL_SelAmount,         NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 용량 선택
    { K_ICE_WATER,        EOL_SelIceWater,       NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물  선택
    { K_ICE_WATER_AMOUNT, EOL_SelIceWaterAmount, NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물  용량 선택
    { K_ICE_LOCK,         EOL_SelIceLock,        NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을잠금  선택
    { K_ICE_WATER_LOCK,   EOL_SelIceWaterLock,   NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물잠금  선택
    { K_HOT_LOCK,         EOL_SelHotLock,        NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 온수잠금  선택
};

// 진공 질소 검사
static U8 EOL_SealDrain(void);
static U8 EOL_CompOnOff(void);
static U8 EOL_MakeIcing(void);
static U8 EOL_MakeDeIcing(void);
static U8 EOL_MakeCold(void);
const static SKeyEventList_T EOL_KeySealdLoadEventList[] =
{
    /* KEY,               Short,                 2sec,   3sec,  5sec,  7sec,  Pop,  TS */
    /* SINGLE KEY */
    { K_ICE,              EOL_MakeIcing,         NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼음 선택
    { K_HOT,              EOL_MakeDeIcing,       NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 온수 선택
    { K_ROOM,             NULL,                  NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 정수 선택
    { K_COLD,             EOL_MakeCold,          NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 냉수 선택
    { K_AMOUNT,           EOL_CompOnOff,         NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 용량 선택
    { K_ICE_WATER,        NULL,                  NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물  선택
    { K_ICE_WATER_AMOUNT, EOL_SealDrain,         NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물  용량 선택
    { K_ICE_LOCK,         NULL,                  NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을잠금  선택
    { K_ICE_WATER_LOCK,   NULL,                  NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 얼을물잠금  선택
    { K_HOT_LOCK,         NULL,                  NULL,   NULL,  NULL,  NULL,  NULL, NULL },            /// 온수잠금  선택
};

static U8 IsValidkeyEol(U32 key)
{
    if( GetEolMode() != EOL_MODE_READY 
        && GetEolMode() != EOL_MODE_SEALED 
        && GetEolMode() != EOL_MODE_DONE )
    {
        return FALSE;   
    }

    // 추출 중이면, 입력을 받지 않는다.
    if( GetWaterOut() == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}

static U8 IsValidKeyFront(U32 key)
{
    return IsValidkeyEol(key);
}

U8 IsValidEolKeyCondition(U32 key)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        return IsValidKeyFront( key );
    }

    return IsValidkeyEol( key );
}

void* GetEolKeyEventList(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        return (void *)EOL_KeyFrontEventList;
    }

    if( GetEolType() == EOL_TYPE_MAIN_FRONT )
    {
        return (void *)EOL_KeyMainFrontEventList;
    }

    if( GetEolType() == EOL_TYPE_SUB_FRONT )
    {
        return (void *)EOL_KeySubFrontEventList;
    }

    /// 진공검사
    if( GetEolMode() == EOL_MODE_SEALED )
    {
        return (void *)EOL_KeySealdLoadEventList;
    }    

    return (void *)EOL_KeyLoadEventList;
}

U8 GetEolKeyEventListSize(void)
{
    if( GetEolType() == EOL_TYPE_FRONT )
    {
        return ( sizeof(EOL_KeyFrontEventList) / sizeof(SKeyEventList_T) );
    }

    if( GetEolType() == EOL_TYPE_MAIN_FRONT )
    {
        return ( sizeof(EOL_KeyMainFrontEventList) / sizeof(SKeyEventList_T) );
    }

    if( GetEolType() == EOL_TYPE_SUB_FRONT )
    {
        return ( sizeof(EOL_KeySubFrontEventList) / sizeof(SKeyEventList_T) );
    }

    /// 진공검사
    if( GetEolMode() == EOL_MODE_SEALED )
    {
        return ( sizeof(EOL_KeySealdLoadEventList) / sizeof(SKeyEventList_T) );
    }

    return ( sizeof(EOL_KeyLoadEventList) / sizeof(SKeyEventList_T) );
}

static void SetKeyBit(U32 maskBit)
{
    U32 inputVal;

    inputVal = GetEolTestInputVal();
    if( (inputVal & maskBit ) != 0 )
    {
        ClearEolTestInputBit(maskBit);
    }
    else
    {
        SetEolTestInputBit(maskBit);
    }
}

static U8 EOL_Ice(void)
{
    SetKeyBit(MK_EOL_KEY_ICE);
    SetEolTestInputCnt(EOL_ID_KEY_ICE, 5);

    return SOUND_SELECT;
}

static U8 EOL_Hot(void)
{
    SetKeyBit(MK_EOL_KEY_HOT);
    
    return SOUND_SELECT;
}

static U8 EOL_Room(void)
{
    SetKeyBit( MK_EOL_KEY_ROOM );

    return SOUND_SELECT;
}

static U8 EOL_Cold(void)
{
    SetKeyBit( MK_EOL_KEY_COLD );

    return SOUND_SELECT;
}

static U8 EOL_Amount(void)
{
    SetKeyBit(MK_EOL_KEY_AMOUNT);
    SetEolTestInputCnt(EOL_ID_KEY_AMOUNT, 5);

    return SOUND_SELECT;
}

static U8 EOL_IceWater(void)
{
    SetKeyBit(MK_EOL_KEY_ICE_WATER);

    return SOUND_SELECT;
}

static U8 EOL_IceWaterAmount(void)
{
    SetKeyBit(MK_EOL_KEY_ICE_WATER_AMOUNT);

    return SOUND_SELECT;
}


static U8 EOL_IceLock(void)
{
    SetKeyBit( MK_EOL_KEY_ICE_LOCK );

    return SOUND_SELECT;
}

static U8 EOL_IceWaterLock(void)
{
    SetKeyBit( MK_EOL_KEY_ICE_WATER_LOCK );
    
    return SOUND_SELECT;
}

static U8 EOL_HotLock(void)
{
    SetKeyBit( MK_EOL_KEY_HOT_LOCK );

    return SOUND_SELECT;
}

// KEY EVENT LIST - EOL MAIN FRONT TEST 
static U8 EOL_MainIce(void)
{
    SetEolTestInputBit(MK_EOL_KEY_ICE);

    return SOUND_SELECT;
}

static U8 EOL_MainRoom(void)
{
    SetEolTestInputBit(MK_EOL_KEY_ROOM);

    return SOUND_SELECT;
}

static U8 EOL_MainCold(void)
{
    SetEolTestInputBit(MK_EOL_KEY_COLD);

    return SOUND_SELECT;
}

static U8 EOL_MainHot(void)
{
    SetEolTestInputBit(MK_EOL_KEY_HOT);

    return SOUND_SELECT;
}

static U8 EOL_MainAmount(void)
{
    SetEolTestInputBit(MK_EOL_KEY_AMOUNT);

    return SOUND_SELECT;
}

static U8 EOL_MainIceWater(void)
{
    SetEolTestInputBit(MK_EOL_KEY_ICE_WATER);

    return SOUND_SELECT;
}

static U8 EOL_MainIceWaterAmount(void)
{
    SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_AMOUNT);

    return SOUND_SELECT;
}

static U8 EOL_MainIceLock(void)
{
    SetEolTestInputBit(MK_EOL_KEY_ICE_LOCK);

    return SOUND_SELECT;
}

static U8 EOL_MainIceWaterLock(void)
{
    SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_LOCK);

    return SOUND_SELECT;
}

static U8 EOL_MainHotLock(void)
{
    SetEolTestInputBit(MK_EOL_KEY_HOT_LOCK);

    return SOUND_SELECT;
}

// KEY EVENT LIST - EOL SUB FRONT TEST 
static U8 EOL_SubIce(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP2) == MK_EOL_SUB_FRONT_STEP2 )
    {
        SetEolTestInputBit(MK_EOL_KEY_ICE);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubRoom(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP6) == MK_EOL_SUB_FRONT_STEP6 )
    {
        SetEolTestInputBit(MK_EOL_KEY_ROOM);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubCold(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP7) == MK_EOL_SUB_FRONT_STEP7 )
    {
        SetEolTestInputBit(MK_EOL_KEY_COLD);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubHot(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP5) == MK_EOL_SUB_FRONT_STEP5 )
    {
        SetEolTestInputBit(MK_EOL_KEY_HOT);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubAmount(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP8) == MK_EOL_SUB_FRONT_STEP8 )
    {
        SetEolTestInputBit(MK_EOL_KEY_AMOUNT);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubIceWater(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP3) == MK_EOL_SUB_FRONT_STEP3 )
    {
        SetEolTestInputBit(MK_EOL_KEY_ICE_WATER);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubIceWaterAmount(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP4) == MK_EOL_SUB_FRONT_STEP4 )
    {
        SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_AMOUNT);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubIceLock(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP9) == MK_EOL_SUB_FRONT_STEP9 )
    {
        SetEolTestInputBit(MK_EOL_KEY_ICE_LOCK);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubIceWaterLock(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP10) == MK_EOL_SUB_FRONT_STEP10 )
    {
        SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_LOCK);
    }

    return SOUND_SELECT;
}

static U8 EOL_SubHotLock(void)
{
    if( (GetEolTestInputVal() & MK_EOL_SUB_FRONT_STEP11) == MK_EOL_SUB_FRONT_STEP11 )
    {
        SetEolTestInputBit(MK_EOL_KEY_HOT_LOCK);
    }

    return SOUND_SELECT;
}

// KEY EVENT LIST - EOL LOAD TEST 
static U8 EOL_TestSeal(void)
{
    StartDisplayInit();
    SetEolMode(EOL_MODE_SEALED_INIT);

    return SOUND_SELECT;
}

static U8 EOL_SelIce(void)
{
    static U8 keyOnOff = OFF;

    // Select water type
    SetIceWaterSelect( SEL_ICE );

    if( (GetEolTestInputVal() & MK_EOL_KEY_ICE) == 0 )
    {
        SetEolTestInputBit(MK_EOL_KEY_ICE);
    }
    else
    {
        if( (GetEolMode() == EOL_MODE_READY)
            || (GetEolMode() == EOL_MODE_DONE) )
        {
            /// 완료모드에서는 아이스트레이 밸브 동작 ON / OFF
            if( keyOnOff == OFF )
            {
                keyOnOff = ON;

                CloseValve(VALVE_NOS_FLOW);
                OpenValve(VALVE_ICE_TRAY_IN);
                TurnOnCirculatePumpTrayIn();
                SetEolTestInputBit(MK_EOL_KEY_ICE_DONE);
                SetEolTestInputBit(MK_EOL_KEY_ICE_TRAY_FLOW);
            }
            else
            {
               keyOnOff = OFF;

               OpenValve(VALVE_NOS_FLOW);
               CloseValve(VALVE_ICE_TRAY_IN);
               TurnOffCirculatePumpTrayIn();   
               ClearEolTestInputBit(MK_EOL_KEY_ICE_DONE);
            }
        }
    }
    
    return SOUND_SELECT;
}

static U8 EOL_SelHot(void)
{
    // Select water type
    SetWaterOutSelect( SEL_WATER_HOT );

    if( (GetEolTestInputVal() & MK_EOL_KEY_HOT) == 0 )
    {
        SetEolTestInputBit( MK_EOL_KEY_HOT );
    }
    else
    {
        SetEolTestInputBit(MK_EOL_KEY_HOT_DP);
        ClearEolTestInputBit(MK_EOL_KEY_ROOM_DP);
        ClearEolTestInputBit(MK_EOL_KEY_COLD_DP);
    }

    return SOUND_SELECT;
}

static U8 EOL_SelRoom(void)
{
    // Select water type
    SetWaterOutSelect( SEL_WATER_ROOM );

    if( (GetEolTestInputVal() & MK_EOL_KEY_ROOM) == 0 )
    {
        SetEolTestInputBit( MK_EOL_KEY_ROOM );
    }
    else
    {
        ClearEolTestInputBit(MK_EOL_KEY_HOT_DP);
        SetEolTestInputBit(MK_EOL_KEY_ROOM_DP);
        ClearEolTestInputBit(MK_EOL_KEY_COLD_DP);
    }
    
    return SOUND_SELECT;
}

static U8 EOL_SelCold(void)
{
    // Select water type
    SetWaterOutSelect( SEL_WATER_COLD );

    if( (GetEolTestInputVal() & MK_EOL_KEY_COLD) == 0 )
    {
        SetEolTestInputBit( MK_EOL_KEY_COLD );
    }
    else
    {
        ClearEolTestInputBit(MK_EOL_KEY_HOT_DP);
        ClearEolTestInputBit(MK_EOL_KEY_ROOM_DP);
        SetEolTestInputBit(MK_EOL_KEY_COLD_DP);
    }

    return SOUND_SELECT;
}

static U8 EOL_SelAmount(void)
{
    SetEolTestInputBit( MK_EOL_KEY_AMOUNT );
    
    return SOUND_SELECT;
}

static U8 EOL_SelIceWater(void)
{
    // Select water type
    SetIceWaterSelect( SEL_ICE_WATER );

    if( (GetEolTestInputVal() & MK_EOL_KEY_ICE_WATER) == 0 )
    {
        SetEolTestInputBit( MK_EOL_KEY_ICE_WATER );
    }
    else
    {
        if( GetEolMode() == EOL_MODE_READY )
        {
            if( GetIceWaterOut() == FALSE )
            {
                CloseValve(VALVE_NOS_FLOW);
                StartIceWaterOut();
                SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_DONE);
                SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_FLOW);
            }
            else
            {
                OpenValve(VALVE_NOS_FLOW);
                StopIceWaterOut();
                ClearEolTestInputBit(MK_EOL_KEY_ICE_WATER_DONE);
            }
        }
    }

    if( GetEolMode() == EOL_MODE_DONE )
    {
        SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_DONE);
    }
    
    return SOUND_SELECT;
}

static U8 EOL_SelIceWaterAmount(void)
{
    SetEolTestInputBit( MK_EOL_KEY_ICE_WATER_AMOUNT );

    if( GetEolMode() == EOL_MODE_DONE )
    {
        if( ((GetEolTestInputVal() & MK_EOL_KEY_ICE_TRAY_FLOW ) != 0UL )
             && ((GetEolTestInputVal() & MK_EOL_KEY_ICE_WATER_FLOW ) != 0UL )
           )
        {
            /// 완료모드에서는 물 전체 배수 ON/OFF
            EOL_Drain();
            SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_AMOUNT_DONE);
        }
        else
        {
            return SOUND_ERROR;
        }
    }
    
    return SOUND_SELECT;
}

static U8 EOL_SelIceLock(void)
{
    SetEolTestInputBit( MK_EOL_KEY_ICE_LOCK );
    return SOUND_SELECT;
}

static U8 EOL_SelIceWaterLock(void)
{
    SetEolTestInputBit( MK_EOL_KEY_ICE_WATER_LOCK );
    return SOUND_SELECT;
}

static U8 EOL_SelHotLock(void)
{
    SetEolTestInputBit( MK_EOL_KEY_HOT_LOCK );
    return SOUND_SELECT;
}

static U8 EOL_Circulate(void)
{
    if( GetEolCirculate() == FALSE )
    {
        // 냉수 순환 시작
        SetEolTestInputBit( MK_EOL_KEY_HOT_LOCK );
        StartEolCirculate();
        return SOUND_SETUP;
    }

    // 냉수 순환 정지
    ClearEolTestInputBit( MK_EOL_KEY_HOT_LOCK );
    StopEolCirculate();
    return SOUND_CANCEL;
}

/// KEY EVENT LIST - EOL SEALD TEST 
static U8 EOL_SealDrain(void)
{
    U8 val = 0U;

    val = EOL_Drain();
    SetEolTestInputBit(MK_EOL_KEY_ICE_WATER_AMOUNT_DONE);

    return val;
}

extern U8 EOL_PrevComp;
extern U8 EOL_Comp;
extern U8 EOL_GasSwitch;
extern U8 EOL_HotGasSwitch;
extern U16 EOL_CompOffDelay;
extern U8 EOL_Switch;
static U8 EOL_CompOnOff(void)
{
    if( EOL_CompOffDelay != 0 )
    {
        return SOUND_ERROR;
    }

    if( EOL_Comp == OFF )
    {
        EOL_Comp = ON;
    }
    else
    {
        EOL_Comp = OFF;
    }
    return SOUND_SELECT;
}

static U8 EOL_MakeIcing(void)
{
    EOL_Switch = TRUE;
    EOL_GasSwitch = GAS_SWITCH_ICE;
    EOL_HotGasSwitch = HOTGAS_SWITCH_NORMAL;
    return SOUND_SELECT;
}

static U8 EOL_MakeDeIcing(void)
{
    EOL_Switch = TRUE;
    EOL_GasSwitch = GAS_SWITCH_ICE;
    EOL_HotGasSwitch = HOTGAS_SWITCH_HOT;
    return SOUND_SELECT;
}

static U8 EOL_MakeCold(void)
{
    EOL_Switch = TRUE;
    EOL_GasSwitch = GAS_SWITCH_COLD;
    EOL_HotGasSwitch = HOTGAS_SWITCH_NORMAL;
    return SOUND_SELECT;
}
