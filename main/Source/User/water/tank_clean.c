#include "tank_clean.h"
#include "water_out.h"
#include "ice.h"
#include "service.h"
#include "hot_water.h"


// �ü� ������ �̰��� ��, �ܼ� ���� �ð�

typedef struct _tank_clean_
{
    /* Clean Tank Mode */
    U8   Out;     // TRUE or FALSE
    U8   OffLoad;    // TRUE or FALSE (���� OFF)
} TankClean_T;


TankClean_T Clean;

void InitTankClean(void)
{
    Clean.Out       = FALSE;
    Clean.OffLoad   = FALSE;
}


/* ��ũ ���� ��� ����/���� */
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
    /* ��ũ ž OPEN,
     * �ü� ����,
     * ���� ���� ���� 
     */
    if( GetServiceCheckTankOpen() == TRUE 
            && GetWaterOutSelect() == SEL_WATER_COLD
            && GetWaterOutContinue() == TRUE )
    {
        return TRUE; // ��ũ ���� ��� ���� ����
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
        // ��ũ ž Ŀ���� ������, �¼� ���� ���⵵ ����
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

