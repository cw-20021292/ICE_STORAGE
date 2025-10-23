/**
 * File : api_key.c
 * 
 * Application Programming Interface
 * Depend on HAL
*/
#include "api_key.h"

#define KEY_PUSHED        LOW
#define LEVER_PUSHED      LOW

#define DEBOUNCE_TIMEOUT  (50)    /// Debounce Timeout, 50ms @1ms

typedef struct _api_key_
{
    U32 FrontKey;
    U32 Key;

    U8  LeverWater;
    U8  LeverIce;
} SApiKey_T;

SApiKey_T ApiKey;

/* Part For Control Key In Front */

/**
 * @ brief     Function For Initialize Key Info
 * @ detail    none
 * @ param   none
 * @ return   none
**/
void InitApiKey(void)
{
    ApiKey.FrontKey = 0UL;
    ApiKey.Key = 0UL;

    ApiKey.LeverWater = 0U;
    ApiKey.LeverIce = 0U;
}

/**
 * @ brief            Function For Set Key Info (from front)
 * @ detail           none
 * @ param   xUkey    Key Info From Front
 * @ return           none
**/
void SetFrontKey(U32 key)
{
    ApiKey.FrontKey = key;
}

/**
 * @ brief            Function For Get Key Info (from front)
 * @ detail           Front (Uart)
 * @ param            none
 * @ return           Key Info From Front
**/
U32 GetFrontKey(void)
{
    return ApiKey.FrontKey;
}

/**
 * @ brief            Function For Get Key Info
 * @ detail           none
 * @ param            none
 * @ return           Key Info From Front
**/
U32 GetKey(void)
{
    ApiKey.Key = ApiKey.FrontKey;

    return ApiKey.Key;
}

/**
 * @ brief               Function For Set Key Info (from front)
 * @ detail              Front ( Uart ), For Lever Control
 * @ param   xUkeyOut    Key Info From Front
 * @ return              none
**/
void SetLeverWater(U8 lever)
{
    ApiKey.LeverWater = lever;
}

/**
 * @ brief            Function For Get Key Info (from front)
 * @ detail           Front (Uart), For Lever Control
 * @ param            none
 * @ return           Key Info From Front
**/
U8 GetLeverWater(void)
{
    return ApiKey.LeverWater;
}

/**
 * @ brief                Function For Set Key Info (from front)
 * @ detail               Front ( Uart ), For Lever Control
 * @ param   xUkeyOut     Key Info From Front
 * @ return               none
**/
void SetLeverIce(U8 lever)
{
    ApiKey.LeverIce = lever;
}

/**
 * @ brief            Function For Get Key Info (from front)
 * @ detail           Front (Uart), For Lever Control
 * @ param            none
 * @ return           Key Info From Front
**/
U8 GetLeverIce(void)
{
    return ApiKey.LeverIce;
}

U8 GetLeverIceAmount(void)
{
    /// 용량 키 상태 확인
    if( GetKey() != KEY_12 )
    {
        return FALSE;
    }

    /// 레버 얼음 상태 확인
    if( GetLeverIce() != 1 )
    {
        return FALSE;
    }

    return TRUE;
}
