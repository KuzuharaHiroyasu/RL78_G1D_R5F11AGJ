/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
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
* Copyright (C) . All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_port.c
* Version      :  
* Device(s)    : R5F11AGJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for Port module.
* Creation Date: 2018/04/12
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "sys.h"
#include "r_cg_macrodriver.h"
#include "r_cg_port.h"
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
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_PORT_Create(void)
{
#if 0
	// 自動生成
    P0 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0;
    P1 = _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0;
    P2 = _00_Pn0_OUTPUT_0;
    ADPC = _01_ADPC_DI_ON;
    PM0 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _04_PMn2_NOT_USE | _08_PMn3_NOT_USE | _80_PM0_DEFAULT;
    PM1 = _01_PMn0_NOT_USE | _02_PMn1_NOT_USE | _04_PMn2_NOT_USE | _08_PMn3_MODE_INPUT | _00_PMn4_MODE_OUTPUT | 
          _00_PMn5_MODE_OUTPUT | _40_PMn6_MODE_INPUT;
    PM2 = _00_PMn0_MODE_OUTPUT | _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _08_PMn3_MODE_INPUT | _F0_PM2_DEFAULT;
#else
	// プラットフォームと競合しない様に修正
	//出力初期値
    P0_bit.no3 = 0;
    P1_bit.no0 = 0;
    POM0 |= _08_POMn3_NCH_ON;
    PMC0 &= ~0x08;
    PM0 &= ~0x08;
    PM1 &= ~0x01;
    
    
#endif

}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
