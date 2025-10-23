/**
 * File : amount.h
 *
 * Amount ( Ascending, Descending, Step, Qsort )
**/
#ifndef __AMOUNT_H__
#define __AMOUNT_H__

#include "prj_type.h"

typedef enum _amount_id_
{
    AMOUNT_ID_250ML = 0,
    AMOUNT_ID_500ML,
    AMOUNT_ID_700ML,
    AMOUNT_ID_1000ML,
    AMOUNT_ID_1500ML,
    AMOUNT_ID_TRAY,

    AMOUNT_ID_ICE_WATER_500ML,
    AMOUNT_ID_ICE_WATER_700ML,
    AMOUNT_ID_ICE_WATER_1000ML,
} EAmountId_T;

void InitAmount(void);

U16 GetAmount(EAmountId_T id);
U32 GetAmountHz(EAmountId_T id);

U8 CalcPercent(U32 target, U32 current);
U16 CalcLiter(U32 amount);
U32 CalcAmount(U16 liter);

#endif  /* __AMOUNT_H__ */
