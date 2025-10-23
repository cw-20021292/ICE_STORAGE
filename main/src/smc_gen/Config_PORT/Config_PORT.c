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
* File Name        : Config_PORT.c
* Component Version: 1.7.0
* Device(s)        : R7F100GSLxFB
* Description      : This file implements device driver for Config_PORT.
***********************************************************************************************************************/
/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Config_PORT.h"
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
* Function Name: R_Config_PORT_Create
* Description  : This function initializes the port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_PORT_Create(void)
{
    volatile uint16_t w_count;

    CCDE = _00_P63_OUTPUT_CURRENT_OFF | _00_P62_OUTPUT_CURRENT_OFF | _00_P61_OUTPUT_CURRENT_OFF | 
           _00_P60_OUTPUT_CURRENT_OFF | _00_P50_OUTPUT_CURRENT_OFF | _00_P51_OUTPUT_CURRENT_OFF | 
           _00_P17_OUTPUT_CURRENT_OFF | _00_P16_OUTPUT_CURRENT_OFF;

    /* Wait for stable time (10us) */
    for (w_count = 0U; w_count < PORT_STABLE_WAITTIME; w_count++)
    {
        NOP();
    }
 
    /* Set PORT0 registers */
    P0 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PDIDIS0 = _00_PDIDISn4_INPUT_BUFFER_ON | _00_PDIDISn3_INPUT_BUFFER_ON | _00_PDIDISn2_INPUT_BUFFER_ON | 
              _00_PDIDISn0_INPUT_BUFFER_ON;
    POM0 = _00_POMn4_NCH_OFF | _00_POMn3_NCH_OFF | _00_POMn2_NCH_OFF | _00_POMn0_NCH_OFF;
    PMCA0 = _F3_PMCA0_DEFAULT | _08_PMCAn3_NOT_USE | _04_PMCAn2_NOT_USE;
    PMCT0 = _00_PMCTn6_DIGITAL_ON | _00_PMCTn5_DIGITAL_ON | _00_PMCTn3_NOT_USE | _00_PMCTn2_NOT_USE | 
            _00_PMCTn1_DIGITAL_ON | _00_PMCTn0_DIGITAL_ON;
    PMCE0 = _00_PMCEn1_DIGITAL_ON;
    PM0 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
          _08_PMn3_NOT_USE | _04_PMn2_NOT_USE | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    /* Set PORT1 registers */
    P1 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PDIDIS1 = _00_PDIDISn7_INPUT_BUFFER_ON | _00_PDIDISn5_INPUT_BUFFER_ON | _00_PDIDISn4_INPUT_BUFFER_ON | 
              _00_PDIDISn3_INPUT_BUFFER_ON | _00_PDIDISn2_INPUT_BUFFER_ON | _00_PDIDISn1_INPUT_BUFFER_ON | 
              _00_PDIDISn0_INPUT_BUFFER_ON;
    POM1 = _00_POMn7_NCH_OFF | _00_POMn5_NCH_OFF | _00_POMn4_NCH_OFF | _00_POMn3_NCH_OFF | _00_POMn2_NCH_OFF | 
           _00_POMn1_NCH_OFF | _00_POMn0_NCH_OFF;
    PMCA1 = _F7_PMCA1_DEFAULT | _08_PMCAn3_NOT_USE;
    PMCE1 = _00_PMCEn7_DIGITAL_ON | _00_PMCEn6_DIGITAL_ON | _00_PMCEn5_DIGITAL_ON | _00_PMCEn4_DIGITAL_ON | 
            _00_PMCEn3_DIGITAL_ON | _00_PMCEn2_DIGITAL_ON | _00_PMCEn1_DIGITAL_ON | _00_PMCEn0_DIGITAL_ON;
    PM1 = _00_PMn7_MODE_OUTPUT | _40_PMn6_NOT_USE | _00_PMn5_MODE_OUTPUT | _10_PMn4_NOT_USE | _08_PMn3_NOT_USE | 
          _04_PMn2_NOT_USE | _02_PMn1_NOT_USE | _00_PMn0_MODE_OUTPUT;
    /* Set PORT3 registers */
    P3 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PU3 = _00_PUn7_PULLUP_OFF | _00_PUn6_PULLUP_OFF | _00_PUn5_PULLUP_OFF | _00_PUn4_PULLUP_OFF | 
          _00_PUn3_PULLUP_OFF | _00_PUn2_PULLUP_OFF | _00_PUn1_PULLUP_OFF | _00_PUn0_PULLUP_OFF;
    PIM3 = _00_PIMn3_TTL_OFF;
    PDIDIS3 = _00_PDIDISn4_INPUT_BUFFER_ON;
    POM3 = _00_POMn4_NCH_OFF;
    PMCA3 = _1F_PMCA3_DEFAULT | _00_PMCAn7_DIGITAL_ON | _00_PMCAn6_DIGITAL_ON | _00_PMCAn5_DIGITAL_ON;
    PMCT3 = _00_PMCTn1_NOT_USE | _00_PMCTn0_NOT_USE;
    PM3 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
          _00_PMn3_MODE_OUTPUT | _04_PMn2_MODE_INPUT | _02_PMn1_NOT_USE | _01_PMn0_NOT_USE;
    /* Set PORT4 registers */
    P4 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PU4 = _00_PUn7_PULLUP_OFF | _00_PUn6_PULLUP_OFF | _00_PUn5_PULLUP_OFF | _00_PUn4_PULLUP_OFF | 
          _00_PUn3_PULLUP_OFF | _00_PUn2_PULLUP_OFF | _00_PUn1_PULLUP_OFF | _01_PUn0_PULLUP_ON;
    PIM4 = _00_PIMn4_TTL_OFF | _00_PIMn3_TTL_OFF | _00_PIMn1_TTL_OFF;
    PDIDIS4 = _00_PDIDISn5_INPUT_BUFFER_ON | _00_PDIDISn4_INPUT_BUFFER_ON | _00_PDIDISn3_INPUT_BUFFER_ON | 
              _00_PDIDISn2_INPUT_BUFFER_ON;
    POM4 = _00_POMn5_NCH_OFF | _00_POMn4_NCH_OFF | _00_POMn3_NCH_OFF | _00_POMn2_NCH_OFF;
    PM4 = _80_PMn7_NOT_USE | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
          _08_PMn3_MODE_INPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _01_PMn0_NOT_USE;
    /* Set PORT5 registers */
    P5 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PDIDIS5 = _00_PDIDISn5_INPUT_BUFFER_ON | _00_PDIDISn4_INPUT_BUFFER_ON | _00_PDIDISn3_INPUT_BUFFER_ON | 
              _00_PDIDISn2_INPUT_BUFFER_ON | _00_PDIDISn0_INPUT_BUFFER_ON;
    POM5 = _00_POMn5_NCH_OFF | _00_POMn4_NCH_OFF | _00_POMn3_NCH_OFF | _00_POMn2_NCH_OFF | _00_POMn0_NCH_OFF;
    PMCT5 = _00_PMCTn0_DIGITAL_ON;
    PMCE5 = _00_PMCEn1_DIGITAL_ON | _00_PMCEn0_DIGITAL_ON;
    PM5 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    /* Set PORT6 registers */
    P6 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PMCT6 = _00_PMCTn7_DIGITAL_ON | _00_PMCTn6_DIGITAL_ON | _00_PMCTn5_DIGITAL_ON | _00_PMCTn4_DIGITAL_ON;
    PMCE6 = _00_PMCEn1_DIGITAL_ON | _00_PMCEn0_DIGITAL_ON;
    PM6 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _02_PMn1_NOT_USE | _01_PMn0_NOT_USE;
    /* Set PORT7 registers */
    P7 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PDIDIS7 = _00_PDIDISn4_INPUT_BUFFER_ON | _00_PDIDISn2_INPUT_BUFFER_ON | _00_PDIDISn1_INPUT_BUFFER_ON;
    POM7 = _00_POMn4_NCH_OFF | _00_POMn2_NCH_OFF | _00_POMn1_NCH_OFF;
    PMCT7 = _00_PMCTn7_DIGITAL_ON | _00_PMCTn6_DIGITAL_ON | _00_PMCTn5_DIGITAL_ON | _00_PMCTn4_DIGITAL_ON | 
            _00_PMCTn3_DIGITAL_ON | _00_PMCTn2_DIGITAL_ON | _00_PMCTn1_DIGITAL_ON | _00_PMCTn0_DIGITAL_ON;
    PM7 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    /* Set PORT8 registers */
    P8 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PDIDIS8 = _00_PDIDISn3_INPUT_BUFFER_ON | _00_PDIDISn2_INPUT_BUFFER_ON | _00_PDIDISn1_INPUT_BUFFER_ON | 
              _00_PDIDISn0_INPUT_BUFFER_ON;
    POM8 = _00_POMn3_NCH_OFF | _00_POMn2_NCH_OFF | _00_POMn1_NCH_OFF | _00_POMn0_NCH_OFF;
    PM8 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    /* Set PORT9 registers */
    P9 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
         _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PU9 = _00_PUn7_PULLUP_OFF | _00_PUn6_PULLUP_OFF | _00_PUn5_PULLUP_OFF | _00_PUn4_PULLUP_OFF | 
          _00_PUn3_PULLUP_OFF | _00_PUn2_PULLUP_OFF | _00_PUn1_PULLUP_OFF | _00_PUn0_PULLUP_OFF;
    PDIDIS9 = _00_PDIDISn6_INPUT_BUFFER_ON;
    POM9 = _00_POMn6_NCH_OFF;
    PM9 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _20_PMn5_MODE_INPUT | _10_PMn4_MODE_INPUT | 
          _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    /* Set PORT10 registers */
    P10 = _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn2_OUTPUT_0 | 
          _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PMCA10 = _FE_PMCA10_DEFAULT | _00_PMCAn0_DIGITAL_ON;
    PM10 = _80_PM10_DEFAULT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | _08_PMn3_NOT_USE | 
           _00_PMn2_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    /* Set PORT11 registers */
    P11 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
          _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PU11 = _00_PUn7_PULLUP_OFF | _00_PUn6_PULLUP_OFF | _00_PUn5_PULLUP_OFF | _00_PUn4_PULLUP_OFF | 
           _00_PUn3_PULLUP_OFF | _00_PUn2_PULLUP_OFF | _00_PUn1_PULLUP_OFF | _00_PUn0_PULLUP_OFF;
    PMCA11 = _1F_PMCA11_DEFAULT | _00_PMCAn7_DIGITAL_ON | _00_PMCAn6_DIGITAL_ON | _00_PMCAn5_DIGITAL_ON;
    PM11 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn4_MODE_OUTPUT | 
           _00_PMn3_MODE_OUTPUT | _04_PMn2_MODE_INPUT | _00_PMn1_MODE_OUTPUT | _00_PMn0_MODE_OUTPUT;
    /* Set PORT12 registers */
    P12 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | 
          _00_Pn0_OUTPUT_0;
    PU12 = _00_PUn7_PULLUP_OFF | _00_PUn6_PULLUP_OFF | _00_PUn5_PULLUP_OFF | _00_PUn2_PULLUP_OFF | 
           _00_PUn1_PULLUP_OFF | _00_PUn0_PULLUP_OFF;
    PDIDIS12 = _00_PDIDISn0_INPUT_BUFFER_ON;
    POM12 = _00_POMn0_NCH_OFF;
    PMCA12 = _FE_PMCA12_DEFAULT | _01_PMCAn0_NOT_USE;
    PM12 = _18_PM12_DEFAULT | _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _10_PMn4_NOT_USE | 
           _08_PMn3_NOT_USE | _04_PMn2_MODE_INPUT | _00_PMn1_MODE_OUTPUT | _01_PMn0_NOT_USE;
    /* Set PORT13 registers */
    P13 = _00_Pn0_OUTPUT_0;
    PDIDIS13 = _00_PDIDISn7_INPUT_BUFFER_ON;
    /* Set PORT14 registers */
    P14 = _00_Pn7_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | 
          _00_Pn2_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PDIDIS14 = _00_PDIDISn4_INPUT_BUFFER_ON | _00_PDIDISn3_INPUT_BUFFER_ON | _00_PDIDISn2_INPUT_BUFFER_ON;
    POM14 = _00_POMn4_NCH_OFF | _00_POMn3_NCH_OFF | _00_POMn2_NCH_OFF;
    PMCA14 = _7F_PMCA14_DEFAULT | _00_PMCAn7_DIGITAL_ON;
    PM14 = _00_PMn7_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _20_PMn5_NOT_USE | _00_PMn4_MODE_OUTPUT | 
           _00_PMn3_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _02_PMn1_NOT_USE | _00_PMn0_MODE_OUTPUT;
    /* Set PORT15 registers */
    P15 = _00_Pn6_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn2_OUTPUT_0 | 
          _00_Pn1_OUTPUT_0 | _00_Pn0_OUTPUT_0;
    PMCA15 = _80_PMCA15_DEFAULT | _00_PMCAn6_DIGITAL_ON | _00_PMCAn5_DIGITAL_ON | _10_PMCAn4_NOT_USE | 
             _08_PMCAn3_NOT_USE | _04_PMCAn2_NOT_USE | _02_PMCAn1_NOT_USE | _01_PMCAn0_NOT_USE;
    PMCT15 = _00_PMCTn5_DIGITAL_ON | _00_PMCTn4_NOT_USE | _00_PMCTn3_NOT_USE | _00_PMCTn2_NOT_USE | 
             _00_PMCTn1_NOT_USE | _00_PMCTn0_NOT_USE;
    PM15 = _80_PM15_DEFAULT | _00_PMn6_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _10_PMn4_NOT_USE | _08_PMn3_NOT_USE | 
           _04_PMn2_NOT_USE | _02_PMn1_NOT_USE | _01_PMn0_NOT_USE;

    R_Config_PORT_Create_UserInit();
}

/***********************************************************************************************************************
* Function Name: R_Config_PORT_ReadPmnValues
* Description  : This function specifies the value in the output latch for a port is read when the pin is in output mode.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Config_PORT_ReadPmnValues(void)
{
    PMS = _00_PMN_VALUES;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
