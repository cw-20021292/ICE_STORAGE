/**
 * File : err_temp.c
 * 
 * Error Temp
*/
#include "err_temp.h"

/*
 * �µ� ������ ����, ���� 0.2V �������� �µ� ���� ���� ����
 * ������ OPEN ����, SHORT ���� ����
 *
 */

#define ADC_12BIT_TOP      (985)   /* 4.8V */
#define ADC_12BIT_BOTTOM   (41)    /* 0.2V */

U8 GetTempErrorType(U16 adc)
{
    if( adc > ADC_12BIT_TOP )
    {
        return STATUS_SHORT;    /* ERROR */
    }
    else if( adc < ADC_12BIT_BOTTOM )
    {
        return STATUS_OPEN;   /* ERROR */
    }

    return STATUS_NORMAL; /* NORMAL */
}


U8 CheckErrorTemp(U8 error, U16 adc, U8 *pBuf)
{
    U8 index;

    index = GetTempErrorType( adc );
    if( index != pBuf[0] )
    {
        pBuf[0] = index;

        pBuf[1] = COUNT_NUM;   /* NORMAL COUNT */
        pBuf[2] = COUNT_NUM;   /* OPEN ERROR COUNT */
        pBuf[3] = COUNT_NUM;   /* SHORT ERROR COUNT */
        return error;
    }

    if( pBuf[index] != 0 )
    {
        pBuf[index]--;
    }

    if( pBuf[index] == 0 )
    {
        if( index == STATUS_NORMAL )
        {
            error = FALSE; // NORMAL
        }
        else
        {
            error = TRUE;  // ERROR 
        }
    }

    return error;
}
