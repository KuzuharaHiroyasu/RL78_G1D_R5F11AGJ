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
* File Name    : r_cg_iica.c
* Version      :  
* Device(s)    : R5F11AGJ
* Tool-Chain   : CCRL
* Description  : This file implements device driver for IICA module.
* Creation Date: 2018/03/15
***********************************************************************************************************************/

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "header.h"

#include "r_cg_macrodriver.h"
#include "r_cg_iica.h"
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
volatile uint8_t   g_iica0_master_status_flag; /* iica0 master flag */
volatile uint8_t   g_iica0_slave_status_flag;  /* iica0 slave flag */
volatile uint8_t * gp_iica0_rx_address;        /* iica0 receive buffer address */
volatile uint16_t  g_iica0_rx_len;             /* iica0 receive data length */
volatile uint16_t  g_iica0_rx_cnt;             /* iica0 receive data count */
volatile uint8_t * gp_iica0_tx_address;        /* iica0 send buffer address */
volatile uint16_t  g_iica0_tx_cnt;             /* iica0 send data count */
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_IICA0_Create
* Description  : This function initializes the IICA0 module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_Create(void)
{
	
	R_IICA0_Reset();			// ユーザーコード
	
    IICA0EN = 1U; /* supply IICA0 clock */
    IICE0 = 0U; /* disable IICA0 operation */
    IICAMK0 = 1U; /* disable INTIICA0 interrupt */
    IICAIF0 = 0U; /* clear INTIICA0 interrupt flag */
    /* Set INTIICA0 low priority */
    IICAPR10 = 1U;
    IICAPR00 = 1U; 
    /* Set SCLA0, SDAA0 pin */
    P6 &= 0xFCU;
    PM6 |= 0x03U;  
    SMC0 = 1U;
    IICWL0 = _0B_IICA0_IICWL_VALUE;
    IICWH0 = _0A_IICA0_IICWH_VALUE;
    DFC0 = 0U; /* digital filter off */
    IICCTL01 &= (uint8_t)~_01_IICA_fCLK_HALF;
    SVA0 = _10_IICA0_MASTERADDRESS;
    STCEN0 = 1U;
    IICRSV0 = 1U;
    SPIE0 = 0U;
    WTIM0 = 1U;
    ACKE0 = 1U;
    IICAMK0 = 0U;
    IICE0 = 1U;
    LREL0 = 1U;
    /* Set SCLA0, SDAA0 pin */
    P6 &= 0xFCU;
    PM6 &= 0xFCU;
}
/***********************************************************************************************************************
* Function Name: R_IICA0_Stop
* Description  : This function stops IICA0 module operation.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_Stop(void)
{
    IICE0 = 0U;    /* disable IICA0 operation */
}
/***********************************************************************************************************************
* Function Name: R_IICA0_StopCondition
* Description  : This function sets IICA0 stop condition flag.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_IICA0_StopCondition(void)
{
    SPT0 = 1U;     /* set stop condition flag */
}
/***********************************************************************************************************************
* Function Name: R_IICA0_Master_Send
* Description  : This function starts to send data as master mode.
* Arguments    : adr -
*                    send address
*                tx_buf -
*                    transfer buffer pointer
*                tx_num -
*                    buffer size
*                wait -
*                    wait for start condition
* Return Value : status -
*                    MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS R_IICA0_Master_Send(uint8_t adr, uint8_t * const tx_buf, uint16_t tx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

	i2c_set_snd_flg( ON );		//ユーザーコード
	
    IICAMK0 = 1U;  /* disable INTIICA0 interrupt */

    if (1U == IICBSY0)
    {
        /* Check bus busy */
        IICAMK0 = 0U;  /* enable INTIICA0 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        STT0 = 1U; /* send IICA0 start condition */
        IICAMK0 = 0U;  /* enable INTIICA0 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == STD0)
        {
            status = MD_ERROR2;
        }
		
        /* Set parameter */
        g_iica0_tx_cnt = tx_num;
        gp_iica0_tx_address = tx_buf;
        g_iica0_master_status_flag = _00_IICA_MASTER_FLAG_CLEAR;
        adr &= (uint8_t)~0x01U; /* set send mode */
        IICA0 = adr; /* send address */
    }

    return (status);
}
/***********************************************************************************************************************
* Function Name: R_IICA0_Master_Receive
* Description  : This function starts to receive IICA0 data as master mode.
* Arguments    : adr -
*                    receive address
*                rx_buf -
*                    receive buffer pointer
*                rx_num -
*                    buffer size
*                wait -
*                    wait for start condition
* Return Value : status -
*                    MD_OK or MD_ERROR1 or MD_ERROR2
***********************************************************************************************************************/
MD_STATUS R_IICA0_Master_Receive(uint8_t adr, uint8_t * const rx_buf, uint16_t rx_num, uint8_t wait)
{
    MD_STATUS status = MD_OK;

	i2c_set_rcv_flg( ON );		//ユーザーコード

    IICAMK0 = 1U;  /* disable INTIIA0 interrupt */

    if (1U == IICBSY0)
    {
        /* Check bus busy */
        IICAMK0 = 0U;  /* enable INTIIA0 interrupt */
        status = MD_ERROR1;
    }
    else
    {
        STT0 = 1U; /* set IICA0 start condition */
        IICAMK0 = 0U;  /* enable INTIIA0 interrupt */
        
        /* Wait */
        while (wait--)
        {
            ;
        }
        
        if (0U == STD0)
        {
            status = MD_ERROR2;
        }
		
        /* Set parameter */
        g_iica0_rx_len = rx_num;
        g_iica0_rx_cnt = 0U;
        gp_iica0_rx_address = rx_buf;
        g_iica0_master_status_flag  = _00_IICA_MASTER_FLAG_CLEAR;
        adr |= 0x01U; /* set receive mode */
        IICA0 = adr; /* receive address */
    }

    return (status);
}


// ==============================
// 以下ユーザーコード
// ==============================

#define		DRV_EEP_WAIT_THIGH				WAIT_2US();														/* データクロック"H"時間(汎用)		*/
#define		DRV_EEP_WAIT_TLOW				WAIT_2US();														/* データクロック"L"時間(汎用)		*/
#define		DRV_EEP_WAIT_TSU_STA			WAIT_2US();					/* スタートコンディションセットアップ時間	*/
#define		DRV_EEP_WAIT_THD_STA			WAIT_2US();					/* スタートコンディションホールド時間		*/
#define		DRV_EEP_WAIT_TSU_STO			WAIT_2US();					/* ストップコンディションセットアップ時間	*/
#define		DRV_EEP_WAIT_THD_STO			WAIT_2US();					/* ストップコンディションホールド時間 ※マニュアルに記載無し */

#define		DRV_EEP_SCL_LOW					( P6  &= ~0x01 );
#define		DRV_EEP_SCL_HIGH				( P6  |= 0x01 );
#define		DRV_EEP_SDA_LOW					( PM6 &= ~0x02);(P6 &= ~0x02);
#define		DRV_EEP_SDA_HIGH				( PM6 |= 0x02);

#define DRV_EEP_RESET_CNT			9						/* スタートコンディション回数[9] */


static void R_IICA0_Reset(void)
{
	int i = 0;
	
	PM6 &= (~0x01);	// SCL出力
	PM6 |= 0x02;	// SDA入力

#if 0
	// ポート設定
    P6 = 0x00;
    PM6 =  0x02 | 0xF0;
	
	P6  |= 0x01;
	PM6  = 0;
	P6 &= ~0x02;
	
	P6  &= ~0x01;
	PM6 &= ~0x02;
#endif
	
	R_IICA0_Reset_Start();
	
	DRV_EEP_SDA_HIGH;
	
	// ダミークロック9回
	for( i = 0; i < DRV_EEP_RESET_CNT; i++ ){
		DRV_EEP_SCL_LOW;
		DRV_EEP_WAIT_TLOW;
		DRV_EEP_SCL_HIGH;
		DRV_EEP_WAIT_THIGH;
	}
	
	R_IICA0_Reset_Start();
	
	R_IICA0_Reset_Stop();
}

static void R_IICA0_Reset_Start(void)
{
	DRV_EEP_SCL_LOW;
	DRV_EEP_SDA_HIGH;
	DRV_EEP_WAIT_TLOW;			/* データクロック"L"時間					*/

	DRV_EEP_SCL_HIGH;
	DRV_EEP_WAIT_TSU_STA;			/* スタートコンディションセットアップ時間	*/

	DRV_EEP_SDA_LOW;				/* これがSTART BIT						*/
	DRV_EEP_WAIT_THD_STA;			/* スタートコンディションホールド時間		*/

	DRV_EEP_SCL_LOW;
	
}

static void R_IICA0_Reset_Stop(void)
{
	DRV_EEP_SDA_LOW;
	DRV_EEP_WAIT_TLOW;		/* データクロック"L"時間					*/

	DRV_EEP_SCL_HIGH;
	DRV_EEP_WAIT_TSU_STO;		/* ストップコンディションセットアップ時間	*/

	DRV_EEP_SDA_HIGH;			/* これがSTOP BIT						*/
	DRV_EEP_WAIT_THD_STO;		/* ストップコンディションホールド時間	*/

	DRV_EEP_SCL_LOW;
	DRV_EEP_WAIT_TLOW;		/* データクロック"L"時間					*/

	/* SDAは入力HIGH状態で終わる	*/
	
}



/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
