/**
 * File : input.c
 * 
 * Input ( General Input : Etc - CHP/CP, ReedSw )
*/
#include "input.h"

#include "api_input.h"

typedef U8 (*FPReadPin_T)(void);

typedef struct _input_
{
    EInputId_T id;

    U8 prevVal;
    U8 currentVal;

    U16 delay;
    U16 setupDelay;

    FPReadPin_T read;
} SInput_T;

SInput_T inputList[INPUT_ID_MAX] =
{
    /* Id                      Pre_Val  Current_Val  Delay               SetupDelay         Fun_Read_Pin */
    { INPUT_ID_TANK_OPEN,      0,        0,          INPUT_DELAY_1SEC,   INPUT_DELAY_1SEC,  GetPinTankCover      },
    { INPUT_ID_TANK_OPEN_UV,   0,        0,          INPUT_DELAY_100MS,  INPUT_DELAY_100MS, GetPinTankCover      },
    { INPUT_ID_TRAY_BRACKET,   0,        0,          INPUT_DELAY_100MS,  INPUT_DELAY_100MS, GetPinIceTankBracket },
};

#define SZ_INPUT_LIST        ( sizeof(inputList) / sizeof(SInput_T) )

static U8 FindInputId(EInputId_T id, U8 *index)
{
    U8 i = 0;
    U8 result = FALSE;
    SInput_T *pInputList = NULL;

    for( i=0; i<SZ_INPUT_LIST; i++ )
    {
        pInputList = &inputList[i];
    
        if( pInputList->id == id )
        {
            *index = i;
            result = TRUE;
            break;
        }
    }

    return result;
}

U8 GetCurrentInputVal(EInputId_T id)
{
    U8 findResult = 0;
    U8 index = 0;
    SInput_T *pInputList = NULL;
    
    findResult = FindInputId(id, &index);

    if( findResult == FALSE )
    {
        return;
    }  
    
    pInputList = &inputList[index];

    return pInputList->currentVal;
}

U8 GetPreviousInputVal(EInputId_T id)
{
    U8 findResult = 0;
    U8 index = 0;
    SInput_T *pInputList = NULL;
    
    findResult = FindInputId(id, &index);

    if( findResult == FALSE )
    {
        return;
    }
    
    pInputList = &inputList[index];

    return pInputList->prevVal;
}

void SetInuptConfigDetectTime(EInputId_T id, U16 delay)
{
    U8 findResult = 0;
    U8 index = 0;
    SInput_T *pInputList = NULL;
    
    findResult = FindInputId(id, &index);

    if( findResult == FALSE )
    {
        return;
    }

    pInputList = &inputList[index];

    pInputList->setupDelay = delay;
}

static void ControlInput(void)
{
    U8 i = 0;
    U8 val = 0;
    SInput_T *pInputList = NULL;

    for( i=0; i<SZ_INPUT_LIST; i++ )
    {
        pInputList = &inputList[i];

        if( pInputList->read != NULL )
        {
            val = pInputList->read();

            if( pInputList->prevVal != val )
            {
                pInputList->delay = pInputList->setupDelay;
                pInputList->prevVal = val;
            }
            else
            {
                if( pInputList->delay != 0 )
                {
                    pInputList->delay--;
                }
                else
                {
                    // update new value..
                    pInputList->currentVal = val;
                }
            }
        }
    }
}

void ProcessInput(void)
{
    ControlInput();
}
