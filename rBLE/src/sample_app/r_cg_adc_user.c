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
* File Name    : r_cg_adc_user.c
* Version      :  
* Device(s)    : R5F11AGJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for ADC module.
* Creation Date: 2019/05/14
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "header.h"

#include "r_cg_macrodriver.h"
#include "r_cg_adc.h"
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
static unsigned short adc_sub( unsigned char ad_type);

void adc_ibiki_kokyu( uint16_t* ibiki, uint16_t* kokyu )
{
	R_ADC_Set_OperationOn();
	*ibiki = adc_sub( ADCHANNEL2 );
	*kokyu = adc_sub( ADCHANNEL3 );
	R_ADC_Set_OperationOff();
}

/************************************************************************/
/* 関数     : adc_photoreflector										*/
/* 関数名   : フォトセンサーAD値読み出し								*/
/* 引数     : uint16_t*	photoref										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.26 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void adc_photoreflector( uint16_t* photoref )
{
	R_ADC_Set_OperationOn();
	*photoref = adc_sub( ADCHANNEL1 );
	R_ADC_Set_OperationOff();
}

/************************************************************************/
/* 関数     : adc_battery												*/
/* 関数名   : 電池AD値読み出し											*/
/* 引数     : uint16_t*	battery											*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.26 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void adc_battery( uint16_t* battery )
{
	R_ADC_Set_OperationOn();
	*battery = adc_sub( ADCHANNEL18 );
	R_ADC_Set_OperationOff();
}

static unsigned short adc_sub( unsigned char ad_type)
{
	unsigned short ad_val;
	
	ADS = ad_type;
	R_ADC_Start();
	
	while(1){
		if( 0 == ADCS ){
			R_ADC_Get_Result( &ad_val );
			break;
		}
	}
	
	return ad_val;
}


/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
