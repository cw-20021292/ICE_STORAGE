#include "tank_clean.h"
#include "water_out.h"
#include "ice.h"
#include "service.h"
#include "hot_water.h"


// 냉수 저수위 미감지 후, 잔수 제거 시간

typedef struct _tank_clean_
{
    /* Clean Tank Mode */
    U8   Out;     // TRUE or FALSE
    U8   OffLoad;    // TRUE or FALSE (부하 OFF)
} TankClean_T;


TankClean_T Clean;

void InitTankClean(void)
{
    Clean.Out       = FALSE;
    Clean.OffLoad   = FALSE;
}


/* 탱크 비우기 모드 설정/해제 */
void SetTankClean(U8 mu8Mode)
{
    Clean.Out = mu8Mode;

    if( mu8Mode == TRUE )
    {
        Clean.OffLoad = TRUE;
        StopIceMake();
    }
}

U8 GetTankClean(void )
{
    return Clean.Out;
}

U8 IsTankCleanMode(void)
{
    /* 탱크 탑 OPEN,
     * 냉수 선택,
     * 연속 추출 선택 
     */
    if( GetServiceCheckTankOpen() == TRUE 
            && GetWaterOutSelect() == SEL_WATER_COLD
            && GetWaterOutContinue() == TRUE )
    {
        return TRUE; // 탱크 비우기 모드 조건 만족
    }

    return FALSE; 
}

U8 GetTankCleanOffLoad(void)
{
    return Clean.OffLoad;
}

void ClearTankCleanOffLoad(void)
{
    if( Clean.OffLoad == TRUE )
    {
        // 탱크 탑 커버가 닫히면, 온수 에어 뺴기도 동작
        StartHotWaterAirFlow(); 
    }

    Clean.OffLoad = FALSE;
}

void ProcessTankClean(void)
{
    if( GetServiceCheckTankOpen() == FALSE 
            && GetTankClean() == FALSE )
    {
        ClearTankCleanOffLoad();
    }
}

