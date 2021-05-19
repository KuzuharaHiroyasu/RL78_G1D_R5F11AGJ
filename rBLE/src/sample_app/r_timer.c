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
* File Name : r_timer.c
* Version : 1.0
* Device(s) : R5F100LE
* H/W Platform : QB-R5F100LE-TB
* Description : This file implements device driver for TAU module.
******************************************************************************/
/******************************************************************************
* History : DD.MM.YYYY Version Description
* : 28.07.2011 1.00 First Release
******************************************************************************/

/******************************************************************************
Includes <System Includes> , ÅgProject IncludesÅh
******************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_timer.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"
#include "vibration.h"

/******************************************************************************
* Function Name: R_TAU0_Create
* Description : This function initializes the TAU0 module.
* Arguments : none
* Return Value : none
******************************************************************************/
void R_TAU0_Create(void)
{
	TAU0EN = 1U;		/* supplies input clock */
//	TPS0 = _0000_TAU_CKM0_FCLK_0 | _0000_TAU_CKM1_FCLK_0 | _0000_TAU_CKM2_FCLK_1 | _0000_TAU_CKM3_FCLK_8;
	/* Stop all channels */
	TT0 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON | _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON | _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON | _0200_TAU_CH1_H8_STOP_TRG_ON | _0800_TAU_CH3_H8_STOP_TRG_ON;
	/* Mask channel 0 interrupt */
	TMMK00 = 1U;	/* disable INTTM00 interrupt */
	TMIF00 = 0U;	/* clear INTTM00 interrupt flag */
	/* Mask channel 1 interrupt */
	TMMK01 = 1U;	/* disable INTTM01 interrupt */
	TMIF01 = 0U;	/* clear INTTM01 interrupt flag */
	/* Mask channel 1 higher 8 bits interrupt */
	TMMK01H = 1U;	/* disable INTTM01H interrupt */
	TMIF01H = 0U;	/* clear INTTM01H interrupt flag */
	/* Mask channel 2 interrupt */
	TMMK02 = 1U;	/* disable INTTM02 interrupt */
	TMIF02 = 0U;	/* clear INTTM02 interrupt flag */
	/* Mask channel 3 interrupt */
	TMMK03 = 1U;	/* disable INTTM03 interrupt */
	TMIF03 = 0U;	/* clear INTTM03 interrupt flag */
	/* Mask channel 3 higher 8 bits interrupt */
	TMMK03H = 1U;	/* disable INTTM03H interrupt */
	TMIF03H = 0U;	/* clear INTTM03H interrupt flag */
	/* Mask channel 4 interrupt */
	TMMK04 = 1U;	/* disable INTTM04 interrupt */
	TMIF04 = 0U;	/* clear INTTM04 interrupt flag */
	/* Mask channel 5 interrupt */
	TMMK05 = 1U;	/* disable INTTM05 interrupt */
	TMIF05 = 0U;	/* clear INTTM05 interrupt flag */
	/* Mask channel 6 interrupt */
	TMMK06 = 1U;	/* disable INTTM06 interrupt */
	TMIF06 = 0U;	/* clear INTTM06 interrupt flag */
	/* Mask channel 7 interrupt */
	TMMK07 = 1U;	/* disable INTTM07 interrupt */
	TMIF07 = 0U;	/* clear INTTM07 interrupt flag */
	/* Set INTTM00 low priority */
	TMPR100 = 1U;
	TMPR000 = 1U;
	/* Channel 0 is used as master channel for PWM output function */
	TMR00 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0800_TAU_COMBINATION_MASTER | _0000_TAU_TRIGGER_SOFTWARE | _0001_TAU_MODE_PWM_MASTER;
	TDR00 = _F9FF_TAU_TDR00_VALUE;
	TO0 &= ~_0001_TAU_CH0_OUTPUT_VALUE_1;
	TOE0 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
	/* Channel 1 is used as slave channel for PWM output function */
#if 0
	TMR01 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE | _0400_TAU_TRIGGER_MASTER_INT | _0009_TAU_MODE_PWM_SLAVE;
	TDR01 = _1900_TAU_TDR01_VALUE;
	TOM0 |= _0002_TAU_CH1_OUTPUT_COMBIN;
	TOL0 &= ~_0002_TAU_CH1_OUTPUT_LEVEL_L;
	TO0 &= ~_0002_TAU_CH1_OUTPUT_VALUE_1;
	TOE0 |= _0002_TAU_CH1_OUTPUT_ENABLE;
	/* Set TO01 pin */
	P1 &= 0xBFU;
	PM1 &= 0xBFU;
#else
	TMR07 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE | _0400_TAU_TRIGGER_MASTER_INT | _0009_TAU_MODE_PWM_SLAVE;
	TDR07 = _1900_TAU_TDR01_VALUE;
	TOM0 |= _0080_TAU_CH7_OUTPUT_COMBIN;
	TOL0 &= ~_0080_TAU_CH7_OUTPUT_LEVEL_L;
	TO0 &= ~_0080_TAU_CH7_OUTPUT_VALUE_1;
	TOE0 |= _0080_TAU_CH7_OUTPUT_ENABLE;
	/* Set TO07 pin */
	P1 &= 0xFEU;
	PM1 &= 0xFEU;
	PIOR |= 0x01;
	POM1 &= 0xFEU;
#endif
}
/******************************************************************************
End of function R_TAU0_Create
******************************************************************************/

/******************************************************************************
* Function Name: R_TAU0_Channel0_Start
* Description : This function starts TAU0 channel 0 counter.
* Arguments : none
* Return Value : none
******************************************************************************/
void R_TAU0_Channel0_Start(void)
{
	TMIF00 = 0U;	/* clear INTTM00 interrupt flag */
	TMMK00 = 0U;	/* enable INTTM00 interrupt */
#if 0
	TOE0 |= _0002_TAU_CH1_OUTPUT_ENABLE;
	TS0 |= _0001_TAU_CH0_START_TRG_ON | _0002_TAU_CH1_START_TRG_ON;
#else
	TOE0 |= _0080_TAU_CH7_OUTPUT_ENABLE;
	TS0 |= _0001_TAU_CH0_START_TRG_ON | _0080_TAU_CH7_START_TRG_ON;
	VIB_ENA = 1;
#endif
}
/******************************************************************************
End of function R_TAU0_Channel0_Start
******************************************************************************/

/******************************************************************************
* Function Name: R_TAU0_Channel0_Stop
* Description : This function stops TAU0 channel 0 counter.
* Arguments : none
* Return Value : none
******************************************************************************/
void R_TAU0_Channel0_Stop(void)
{
#if 0
	TT0 |= _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON;
	TOE0 &= ~_0002_TAU_CH1_OUTPUT_ENABLE;
#else
	TT0 |= _0001_TAU_CH0_STOP_TRG_ON | _0080_TAU_CH7_START_TRG_ON;
	TOE0 &= ~_0080_TAU_CH7_OUTPUT_ENABLE;
#endif
	/* Mask channel 0 interrupt */
	TMMK00 = 1U;	/* disable INTTM00 interrupt */
	TMIF00 = 0U;	/* clear INTTM00 interrupt flag */
}
/******************************************************************************
End of function R_TAU0_Channel0_Stop
******************************************************************************/

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
