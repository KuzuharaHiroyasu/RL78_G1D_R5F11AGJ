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
* File Name    : r_cg_port_user.c
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
/************************************************************************/
/* �֐�     : drv_i_port_inspection										*/
/* �֐���   : �������[�h												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
UB drv_i_port_inspection( void )
{
	UB ret = OFF;
	
	if( HIGH == DRV_I_PORT_INSPECTION ){
		ret = ON;
	}
	
	return ret;
}

/************************************************************************/
/* �֐�     : drv_i_port_bat_chg_detect									*/
/* �֐���   : �[�d���m													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
UB drv_i_port_bat_chg_detect( void )
{
	UB ret = OFF;
	
	if( HIGH == DRV_I_PORT_BAT_CHG_DETECT ){
		ret = ON;
	}
	
	return ret;
}

/************************************************************************/
/* �֐�     : drv_i_port_read_pow_sw(									*/
/* �֐���   : �[�d���m													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.26  Axia Soft Design �a�c �k��	���ō쐬		*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
UB drv_i_port_read_pow_sw(void)
{
	UB port1;
	UB port2;
	UB ret;
	
	// �|�[�g�ǂݏo�� ��2���v�Ŕ�����(��v�������Ȃ����͒v���I�ُ�Ȃ̂ōl�����Ȃ�)
	do{
		port1 = DRV_I_PORT_POW_SW;
		port2= DRV_I_PORT_POW_SW;
	}while( port1 != port2 );

	// �_���Ή�
	if( HIGH == port1 ){
		ret = ON;
	}else{
		ret = OFF;
	}
	
	return ret;
}

