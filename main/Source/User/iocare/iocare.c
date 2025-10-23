#include "iocare.h"
#include "process_nfc.h"
#include "time_short.h"

typedef struct _mapping_table_
{
    U16  IoCare_Mode;
    U16  NFC_Mode;
} MapTable_T;



void InitIoCare(void)
{
#if CONFIG_NFC
    InitNfc();
    SetIoCareMode( IOC_ID_INIT );
    SetIoCareMode( IOC_ID_ERROR );
#endif

}


MapTable_T  MappingTableList[] = 
{
    { IOC_ID_INIT,                  NFC_INIT },
    { IOC_ID_SERVICE_CHECK_DAY,     NFC_SERVICE_CHECK_DAY },
    { IOC_ID_ERROR,                 NFC_ERROR },
    { IOC_ID_UPDATE_SETTING,        NFC_UPDATE_SETTING },
    { IOC_ID_USE_WATER_DATA,        NFC_USE_WATER_DATA },

    { IOC_ID_DAILY_DATA,            NFC_DAILY_DATA },
    { IOC_ID_STER_PASS_DAY,         NFC_STER_PASS_DAY },
    { IOC_ID_STER_HISTORY,          NFC_STER_HISTORY },

    { IOC_ID_ERASE_ALL,             NFC_ERASE_ALL },
};
#define  SZ_LIST  ( sizeof( MappingTableList ) / sizeof( MapTable_T ))

void SetIoCareMode(U16 mu16Mode)
{
    U8 i;

    // 테스트 모드에서는 Skip
    if( GetTimeShortStatus() == TRUE )
    {
        return ;
    }

    for( i = 0; i < SZ_LIST ; i++ )
    {
        if( MappingTableList[ i ].IoCare_Mode == mu16Mode )
        {
            SetNfcMode( MappingTableList[ i ].NFC_Mode );
            return ;
        }
    }
}

void ProcessIoCare(void)
{
#if CONFIG_NFC
    ProcessNfc();
#endif
}


