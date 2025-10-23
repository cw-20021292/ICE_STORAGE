/**
 * File : amount.c
 *
 * Amount ( Ascending, Descending, Step, Qsort )
**/
#include "amount.h"

#include "flow_meter.h"
#include "water_out.h"

typedef struct _amount_Data_
{
    EAmountId_T Id;
    U16 Ml;         /// display
    U32 Hz;         /// output Hz
} SAmountData_T;
const static SAmountData_T gStAmountData[] =
{
    /*     Id            Ml     Hz                */
    {AMOUNT_ID_250ML,    250,   AMOUNT_HZ_250ML   },
#if( CONFIG_TEST_8585_HOT_REPEAT == 1 )
    {AMOUNT_ID_500ML,    250,   AMOUNT_HZ_250ML   },
#else
    {AMOUNT_ID_500ML,    500,   AMOUNT_HZ_500ML   },
#endif
    {AMOUNT_ID_700ML,    700,   AMOUNT_HZ_700ML   },
    {AMOUNT_ID_1000ML,   1000,  AMOUNT_HZ_1000ML  },
    {AMOUNT_ID_1500ML,   1500,  AMOUNT_HZ_1500ML  },
    {AMOUNT_ID_TRAY,     650,   AMOUNT_HZ_650ML   },

    {AMOUNT_ID_ICE_WATER_500ML, 500, AMOUNT_HZ_350ML },
    {AMOUNT_ID_ICE_WATER_700ML, 700, AMOUNT_HZ_500ML },
    {AMOUNT_ID_ICE_WATER_1000ML, 1000, AMOUNT_HZ_700ML },
};
    
U16 GetAmount(EAmountId_T id)
{
    return gStAmountData[id].Ml;
}

U32 GetAmountHz(EAmountId_T id)
{
    return gStAmountData[id].Hz;
}

// 추출량 반환 ( percent )
U8 CalcPercent(U32 target, U32 current)
{
    return (U8)(( (F32)current / (F32)target ) * 100.0f);
}

#define ML_UNIT (10U) /* 최소 단위 10ml */
// 추출량 ml 단위 반환
U16 CalcLiter(U32 amount)
{
    U32 targetAmount;

    targetAmount = AMOUNT_HZ_10ML;
    if( amount >= targetAmount )
    {
        amount = amount / targetAmount;
        amount *= ML_UNIT;   
    }
    else
    {
        amount = 0UL;
    }

    return (U16)(amount);

}

// ml를 추출얄으로 변환
U32 CalcAmount(U16 liter)
{
    U32 amount;

    liter /= ML_UNIT;
    amount = liter * AMOUNT_HZ_10ML;

    return amount;
}

