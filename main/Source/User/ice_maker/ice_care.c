#include "ice_care.h"

#include "ice.h"
#include "ice_door.h"

typedef struct _ice_care_
{
    U16 Mode;

    U8 ConfigCareBreak;
    U8 ConfigCarePosition;
} IceCare_T;

IceCare_T IceCare;

void InitIceCare(void)
{
    IceCare.Mode = ICE_CARE_MODE_NONE;

    IceCare.ConfigCareBreak = TRUE;
    IceCare.ConfigCarePosition = TRUE;
}

void SetIceCareMode(U16 mu16Mode)
{
    IceCare.Mode |= mu16Mode;
}

void ClearIceCareMode(U16 mu16Mode)
{
    IceCare.Mode &= ~mu16Mode;
}

U16 IsSetIceCareMode(U16 mu16Mode )
{
    if( (IceCare.Mode & mu16Mode ) == mu16Mode )
    {
        return TRUE;
    }

    return FALSE;
}

U16 GetIceCareMode(void)
{
    return IceCare.Mode;
}

void SetConfigCareBreak(U8 mu8Config)
{
    IceCare.ConfigCareBreak = mu8Config;
}

U8 GetConfigCareBreak(void)
{
    return IceCare.ConfigCareBreak;
}

void SetConfigCarePosition(U8 mu8Config)
{
    IceCare.ConfigCarePosition = mu8Config;
}

U16 GetConfigCarePosition(void)
{
    return IceCare.ConfigCarePosition;
}

U8 IsValidIceCare(void)
{
    if( GetIceConfigMake() == FALSE )
    {
        return FALSE;
    }

    if( IsOpenIceDoor() == TRUE )
    {
        return FALSE;
    }

    return TRUE;
}


