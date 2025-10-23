/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2021, 2025 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name        : Config_TAU1_1.c
* Component Version: 1.7.0
* Device(s)        : R7F100GSLxFB
* Description      : This file implements device driver for Config_TAU1_1.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_TAU1_1.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_Config_TAU1_1_Create
* Description  : This function initializes the TAU1 channel 1 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAU1_1_Create(void)
{
    TPS1 &= _FFF0_TAU_CKM0_CLEAR;
    TPS1 |= _0000_TAU_CKM0_FCLK_0;
    /* Stop channel 1 */
    TT1 |= _0002_TAU_CH1_STOP_TRG_ON;
    /* Mask channel 1 interrupt */
    TMMK11 = 1U;    /* disable INTTM11 interrupt */
    TMIF11 = 0U;    /* clear INTTM11 interrupt flag */
    /* Set INTTM11 low priority */
    TMPR111 = 1U;
    TMPR011 = 1U;
    /* TAU11 used as interval timer */
    TMR11 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_16BITS_MODE | 
            _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_MODE_INTERVAL_TIMER | _0000_TAU_START_INT_UNUSED;
    TDR11 = _0C7F_TAU_TDR11_VALUE;
    TOM1 &= (uint16_t)~_0002_TAU_CH1_SLAVE_OUTPUT;
    TOL1 &= (uint16_t)~_0002_TAU_CH1_OUTPUT_LEVEL_L;
    TO1 &= (uint16_t)~_0002_TAU_CH1_OUTPUT_VALUE_1;
    TOE1 &= (uint16_t)~_0002_TAU_CH1_OUTPUT_ENABLE;
    
    R_Config_TAU1_1_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_TAU1_1_Start
* Description  : This function starts the TAU1 channel 1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAU1_1_Start(void)
{
    TMIF11 = 0U;    /* clear INTTM11 interrupt flag */
    TMMK11 = 0U;    /* enable INTTM11 interrupt */
    TS1 |= _0002_TAU_CH1_START_TRG_ON;
}

/***********************************************************************************************************************
* Function Name: R_Config_TAU1_1_Stop
* Description  : This function stops the TAU1 channel 1 counter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_TAU1_1_Stop(void)
{
    TT1 |= _0002_TAU_CH1_STOP_TRG_ON;
    /* Mask channel 1 interrupt */
    TMMK11 = 1U;    /* disable INTTM11 interrupt */
    TMIF11 = 0U;    /* clear INTTM11 interrupt flag */
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
