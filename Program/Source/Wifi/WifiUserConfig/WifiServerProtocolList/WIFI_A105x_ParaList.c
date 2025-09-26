    
/*******************************************************************************
&Description :
    A1050 ~ A105x

    Parameter Table

&History : ssg@coway.com 2024.10.04 update


*******************************************************************************/

/******************************************************************************/
/***** Header *****************************************************************/
/******************************************************************************/
#include "WIFI_A105x_ParaList.h"

/******************************************************************************/
/***** User Header ************************************************************/
/******************************************************************************/

/******************************************************************************/
/***** User Function **********************************************************/
/******************************************************************************/

/******************************************************************************/
/***** Table ******************************************************************/
/******************************************************************************/
/* Protocol List */
static const WifiNormalList_T WifiParaProtocolList[] =
{   /*  API ID        */
    {   0x0001,    TYPE_UNSIGNED   },
    {   0x0002,    TYPE_UNSIGNED   },
    {   0x0003,    TYPE_UNSIGNED   },
    {   0x0004,    TYPE_UNSIGNED   },
    {   0x0005,    TYPE_UNSIGNED   },
    {   0x0006,    TYPE_UNSIGNED   },
    {   0x0007,    TYPE_UNSIGNED   },
    {   0x0008,    TYPE_UNSIGNED   },
    {   0x0009,    TYPE_UNSIGNED   },
    {   0x000A,    TYPE_UNSIGNED   },
};

#define SZ_PART_EVENT_LIST    (sizeof(WifiParaProtocolList)/sizeof(WifiNormalList_T))

void* GetWifiPaatDataEventList ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return WifiParaProtocolList;
    }
    else
    {

    }
    return NULL;
}

U16 GetWifiParaEventListSize ( void )
{
    if (IsUserModelType() == Model_0)
    {
        return SZ_PART_EVENT_LIST;
    }
    else
    {

    }
    return NULL;
}

/* [User Para Data] Product -> Server */
U16 GetUserSystemPara ( U16 mu16Para )
{
    U16 mu16Data = 0U;
    switch ( mu16Para )
    {
        case 0x0001 :       /* ID */
            mu16Data = 1;   /* Value */
        break;

        case 0x0002 : 
            mu16Data = 2;
        break;

        default:
		mu16Data = 0xFFFF;
        break;
    }

    return mu16Data;
}

