/**
 * File : display_error.c
 * 
 * Display Error
*/
#include "led.h"
#include "display.h"

#include "error.h"
#include "display_error.h"

static void CommonErrCode(EErrorId_T id, U8 cmd);
static U8 GetErrDispId(EErrorId_T id);

typedef struct _error_display_list_
{
    EErrorId_T Id;
} SErrDisplay_T;

SErrDisplay_T errDisplayList[] = 
{    
    { ERR_ID_MICRO_SW_MOVE    },
    { ERR_ID_MICRO_SW_DETECT  },

    { ERR_ID_OVER_HOT_WATER   },
    { ERR_ID_TEMP_HOT_WATER   },
    { ERR_ID_TEMP_AMBIENT_2   },
    { ERR_ID_TEMP_AMBIENT     },
    { ERR_ID_TEMP_ROOM_WATER  },
    { ERR_ID_TEMP_COLD_WATER  },

    { ERR_ID_COMP_BLDC_ERR9   },
    { ERR_ID_COMP_BLDC_ERR8   },
    { ERR_ID_COMP_BLDC_ERR7   },
    { ERR_ID_COMP_BLDC_ERR6   },
    { ERR_ID_COMP_BLDC_ERR5   },
    { ERR_ID_COMP_BLDC_ERR4   },
    { ERR_ID_COMP_BLDC_ERR3   },
    { ERR_ID_COMP_BLDC_ERR2   },
    { ERR_ID_COMP_BLDC_ERR1   },
    { ERR_ID_COMP_BLDC_COMM   },

    { ERR_ID_COLD_LOW_LEVEL   },

    { ERR_ID_ROOM_HIGH_LEVEL  },
    { ERR_ID_ROOM_LOW_LEVEL   },
    { ERR_ID_ROOM_COMPLEX     },
    { ERR_ID_ROOM_OVF         },  
    
    { ERR_ID_CIRCULATE_PUMP   },
    { ERR_ID_DRAIN_PUMP       },

    { ERR_ID_LEAK             },
};

#define SZ_ERR_LIST  ( sizeof(errDisplayList) / sizeof(SErrDisplay_T) )

void DisplayError(U8 errCode, U8 cmd)
{
    U8 i;

    for( i=0; i<SZ_ERR_LIST; i++ )
    {
        if( errDisplayList[i].Id == errCode )
        {
            CommonErrCode(errCode, cmd);
            break;
        }
    }
}

static void CommonErrCode(EErrorId_T id, U8 cmd)
{
    U8 errCode;

    errCode = GetErrDispId(id);

    if( cmd == ON )
    {
        DispSegError(errCode);
    }
    else
    {
        DispSegError(errCode);
        //AllOffSegment();
    }
}

static U8 GetErrDispId(EErrorId_T id)
{
    U8 errCode = 0;

    switch( id )
    {
        case ERR_ID_LEAK:             errCode = 1; break;
        
        case ERR_ID_ROOM_LOW_LEVEL:   errCode = 11; break;
        case ERR_ID_ROOM_HIGH_LEVEL:  errCode = 13; break;
        case ERR_ID_ROOM_COMPLEX:     errCode = 14; break;
        case ERR_ID_ROOM_OVF:         errCode = 17; break;

        case ERR_ID_COLD_LOW_LEVEL:   errCode = 21; break;
        case ERR_ID_CIRCULATE_PUMP:   errCode = 26; break;
        case ERR_ID_COMP_BLDC_COMM:   errCode = 29; break;

        case ERR_ID_OVER_HOT_WATER:   errCode = 40; break;
        case ERR_ID_TEMP_ROOM_WATER:  errCode = 42; break;
        case ERR_ID_TEMP_AMBIENT:     errCode = 43; break;
        case ERR_ID_TEMP_COLD_WATER:  errCode = 44; break;
        case ERR_ID_TEMP_HOT_WATER:   errCode = 45; break;
        case ERR_ID_TEMP_AMBIENT_2:   errCode = 53; break;
        
        case ERR_ID_DRAIN_PUMP:       errCode = 60; break;
        case ERR_ID_MICRO_SW_DETECT:  errCode = 61; break;
        case ERR_ID_MICRO_SW_MOVE:          errCode = 62; break;
        case ERR_ID_MICRO_SW_MOVE_DEICING:  errCode = 63; break;

        case ERR_ID_COMP_BLDC_ERR9:   errCode = 89; break;
        case ERR_ID_COMP_BLDC_ERR8:   errCode = 88; break;
        case ERR_ID_COMP_BLDC_ERR7:   errCode = 87; break;
        case ERR_ID_COMP_BLDC_ERR6:   errCode = 86; break;
        case ERR_ID_COMP_BLDC_ERR5:   errCode = 85; break;
        case ERR_ID_COMP_BLDC_ERR4:   errCode = 84; break;
        case ERR_ID_COMP_BLDC_ERR3:   errCode = 83; break;
        case ERR_ID_COMP_BLDC_ERR2:   errCode = 82; break;
        case ERR_ID_COMP_BLDC_ERR1:   errCode = 81; break;

        default:                      errCode = 0; break;
    }

    return errCode;
}
