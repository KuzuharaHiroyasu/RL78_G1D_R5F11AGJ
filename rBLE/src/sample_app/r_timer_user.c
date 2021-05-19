/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer 
*
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/
/*******************************************************************************
* File Name : r_timer_user.c
* Version : 1.0
* Device(s) : R5F100LE
* H/W Platform : QB-R5F100LE-TB
* Description : This file implements device driver for TAU module.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 28.07.2011 1.00 First Release
******************************************************************************/
#include "iodefine.h"
#include "sys.h"

/******************************************************************************
Macro definitions
******************************************************************************/
#pragma interrupt INTTM00 R_TAU0_Channel0_Interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/******************************************************************************
Includes <System Includes> , gProject Includesh
******************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include "arch.h"

/******************************************************************************
* Function Name: R_TAU0_Channel0_Interrupt
* Description : This function is INTTM00 interrupt service routine.
* Arguments : none
* Return Value : none
******************************************************************************/
__interrupt void R_TAU0_Channel0_Interrupt(void)
{
	/* Start user code. Do not edit comment generated here */
	static uint16_t tm00_count = 0;	/* INTTM00 interrupt times counter */
	uint16_t temp_duty;				/* Duty factor calculation */
	
	if(++tm00_count == 5 )		/* Update the duty factor per 500ms */
	{
		tm00_count = 0;				/* Interrupt counter reset */
		P1 ^= 0b00000001;			/* Invert LED2 */

		temp_duty = TDR07;			/* Read the current duty setting */
		if(temp_duty >= _1900_TAU_TDR01_VALUE * 9)
		{										/* If current duty is 90%, */
			temp_duty = _0320_TAU_TDR07_VALUE;	/* Set duty factor to 10% */
		}
		else
		{	/* Increase duty factor by 20% */
			temp_duty += _0320_TAU_TDR07_VALUE;
		}
		TDR07 = temp_duty;			/* Set duty factor */
		
		NO_OPERATION_BREAK_POINT();
	}
	/* End user code. Do not edit comment generated here */
}
/******************************************************************************
End of function R_TAU0_Channel0_Interrupt
******************************************************************************/

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
