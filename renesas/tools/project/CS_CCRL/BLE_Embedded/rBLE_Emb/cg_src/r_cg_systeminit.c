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
* Copyright (C) 2015, 2018 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_systeminit.c
* Version      : Code Generator for RL78/G1D V1.01.02.03 [24 May 2018]
* Device(s)    : R5F11AGJ
* Tool-Chain   : CCRL
* Description  : This file implements system initializing function.
* Creation Date: 2019/10/23
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_cgc.h"
#include "r_cg_wdt.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

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
* Function Name: R_Systeminit
* Description  : This function initializes every macro.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_Systeminit(void)
{
    PIOR = 0x00U;
    PM0 &= 0x8FU;   /* set P04, P05, P06 to output mode */
    PM1 &= 0x7FU;   /* set P17 to output mode */
    PM3 &= 0xFDU;   /* set P31 to output mode */
    PM4 &= 0xF1U;   /* set P41, P42, P43 to output mode */
    PM5 &= 0xC0U;    /* set P50, P51, P52, P53, P54, P55 to output mode */
    PM6 &= 0xF3U;    /* set P62, P63 to output mode */
    PM7 = 0x82U;   /* set P70, P72, P73, P74, P75, P76 to output mode, set P71, P77 to input mode */   
    PM14 &= 0xBDU;  /* set P141, P146 to output mode */
    R_CGC_Get_ResetSource();
    R_CGC_Create();
    R_WDT_Create();
    IAWCTL = 0x00U;
}

/***********************************************************************************************************************
* Function Name: hdwinit
* Description  : This function initializes hardware setting.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void hdwinit(void)
{
    DI();
    R_Systeminit();
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
