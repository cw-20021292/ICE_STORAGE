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
* File Name        : Pin.c
* Version          : 1.0.0
* Device(s)        : R7F100GSLxFB
* Description      : This file implements SMC pin code generation.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
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
* Function Name: R_Pins_Create
* Description  : This function initializes Smart Configurator pins
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Pins_Create(void)
{
    /* Set ANI0 pin */
    PMCA2 |= 0x01U;
    PM2 |= 0x01U;

    /* Set ANI1 pin */
    PMCA2 |= 0x02U;
    PM2 |= 0x02U;

    /* Set ANI2 pin */
    PMCA2 |= 0x04U;
    PMCT2 &= 0xFBU;
    PM2 |= 0x04U;

    /* Set ANI3 pin */
    PMCA2 |= 0x08U;
    PMCT2 &= 0xF7U;
    PM2 |= 0x08U;

    /* Set ANI4 pin */
    PMCA2 |= 0x10U;
    PMCT2 &= 0xEFU;
    PM2 |= 0x10U;

    /* Set ANI5 pin */
    PMCA2 |= 0x20U;
    PMCT2 &= 0xDFU;
    PM2 |= 0x20U;

    /* Set ANI6 pin */
    PMCA2 |= 0x40U;
    PMCT2 &= 0xBFU;
    PM2 |= 0x40U;

    /* Set ANI7 pin */
    PMCA2 |= 0x80U;
    PMCT2 &= 0x7FU;
    PM2 |= 0x80U;

    /* Set ANI8 pin */
    PMCA15 |= 0x01U;
    PMCT15 &= 0xFEU;
    PM15 |= 0x01U;

    /* Set ANI9 pin */
    PMCA15 |= 0x02U;
    PMCT15 &= 0xFDU;
    PM15 |= 0x02U;

    /* Set ANI10 pin */
    PMCA15 |= 0x04U;
    PMCT15 &= 0xFBU;
    PM15 |= 0x04U;

    /* Set ANI11 pin */
    PMCA15 |= 0x08U;
    PMCT15 &= 0xF7U;
    PM15 |= 0x08U;

    /* Set ANI12 pin */
    PMCA15 |= 0x10U;
    PMCT15 &= 0xEFU;
    PM15 |= 0x10U;

    /* Set ANI19 pin */
    PMCA12 |= 0x01U;
    PM12 |= 0x01U;

    /* Set INTP2 pin */
    PM4 |= 0x80U;

    /* Set TO01 pin */
    PMCE1 &= 0xBFU;
    PFOE0 |= 0x02U;
    CCDE &= 0xFEU;
    P1 &= 0xBFU;
    PM1 &= 0xBFU;

    /* Set TO14 pin */
    P10 &= 0xF7U;
    PM10 &= 0xF7U;

}

