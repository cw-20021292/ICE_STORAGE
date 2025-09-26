
/*******************************************************************************
&Description :
    A10A0 ~ A10Ax
    Accumulate Data Table

&History : ssg@coway.com 2024.10.04 update

*******************************************************************************/

#include "WIFI_A10Ax_AccumulateList.h"

/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/

/******************************************************************************/
/***** Static Function ********************************************************/
/******************************************************************************/

/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Protocol List */
static const WifiNormalList_T WifiAccumulateProtocolList[] =
{   /*  API ID        */
    {   0x0001,    TYPE_UNSIGNED   },
};

#define SZ_ACC_EVENT_LIST    (sizeof(WifiAccumulateProtocolList)/sizeof(WifiNormalList_T))

void* GetWifiAccumulateDataEventList ( void )
{
    return WifiAccumulateProtocolList;
}

U16 GetWifiAccumulateEventListSize ( void )
{
    return SZ_ACC_EVENT_LIST;
}


U32 GetUserSystemAccumulate ( U16 mu16Acc )
{
    U32 mu32Data = 0U;

    switch ( mu16Acc )
    {
        case 0x0001 :       /* ID */
            mu32Data = 1;   /* Value */
        break;
    }
    return mu32Data;
}