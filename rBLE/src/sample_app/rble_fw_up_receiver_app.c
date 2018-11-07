/**
 ****************************************************************************************
 *
 * @file		rble_fw_up_receiver_app.c
 *
 * @brief	rBLE FW Update Sample Application(Receiver)
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 * $Rev: 3867 $
 *
 ****************************************************************************************
 */
#ifdef _USE_IAR_RL78
#define _MAINCNST _Pragma("location=\"MAINCNST\"")
#define _MAINCODE _Pragma("location=\"MAINCODE\"")
#define _MAINDATA _Pragma("location=\"MAINDATA\"") __NO_INIT
#else
#define _MAINCNST
#define _MAINCODE
#define _MAINDATA

#if !defined(_USE_CCRL_RL78)
#pragma section @@CNST MAINCNST
#pragma section @@CODEL MAINCODE
#pragma section @@DATA MAINDATA
#else
#pragma section const  MAINCNST
#pragma section text   MAINCODE
#pragma section bss    MAINDATA
#endif
#endif

#include "rwble_config.h"
#include "arch.h"

#ifdef USE_FW_UPDATE_PROFILE
#include	<stdio.h>
#include	<string.h>
#include	<stdlib.h>

#include	"rble.h"
#include	"rble_app.h"

#include	"prf_sel.h"
#include	"rble_api_fwup.h"
#include	"codeflash.h"
#include	"rwke_api.h"
#include    "hw_config.h"

/**************************** Define Declaration **************************************/
#define FWUP_CNG_CNST_BLOCK		(0xA800 / 0x400)
#ifdef _USE_IAR_RL78

#if   defined(_USE_R5F11AGG)
#define FWUP_STD_START_BLK		(0x1A000 / 0x400)
#define FWUP_STD_END_BLK		(0x1B400 / 0x400)

#define FWUP_TMP_STD_START_BLK	(0x1E400 / 0x400)
#define FWUP_TMP_STD_END_BLK	(0x1F800 / 0x400)

#elif defined(_USE_R5F11AGH)

#define FWUP_STD_START_BLK		(0x2A000 / 0x400)
#define FWUP_STD_END_BLK		(0x2B400 / 0x400)

#define FWUP_TMP_STD_START_BLK	(0x2E400 / 0x400)
#define FWUP_TMP_STD_END_BLK	(0x2F800 / 0x400)

#else // R5F11AGJ

#define FWUP_STD_START_BLK		(0x2A000 / 0x400)
#define FWUP_STD_END_BLK		(0x2B400 / 0x400)

#define FWUP_TMP_STD_START_BLK	(0x3E400 / 0x400)
#define FWUP_TMP_STD_END_BLK	(0x3F800 / 0x400)
#endif
#endif

#define FW_UP_BLOCK_SIZE	1024
#define FW_UP_WRITE_SIZE	256

#define FW_UP_STD_BLOCK_NUM	7

#if defined(CLK_FCLK_4MHZ)
    #define FSL_FREQ_MHZ_SET    (4)
    #define FSL_VOL_MODE_SET    CODEFLASH_WIDE_VOLTAGE_MODE
#elif defined(CLK_FCLK_8MHZ)
    #define FSL_FREQ_MHZ_SET    (8)
    #define FSL_VOL_MODE_SET    CODEFLASH_WIDE_VOLTAGE_MODE
#elif defined(CLK_FCLK_16MHZ)
    #define FSL_FREQ_MHZ_SET    (16)
    #define FSL_VOL_MODE_SET    CODEFLASH_FULL_SPEED_MODE
#elif defined(CLK_FCLK_32MHZ)
    #define FSL_FREQ_MHZ_SET    (32)
    #define FSL_VOL_MODE_SET    CODEFLASH_FULL_SPEED_MODE
#else
    #error "an unsupported fCLK configuration."
#endif

enum {
	FW_UP_STATE_IDLE,					/* idle */
	FW_UP_STATE_WAIT_START,				/* wait start command */
	FW_UP_STATE_RECEIVE_DATA,			/* receive data */
	FW_UP_STATE_RECEIVE_DATA_COMP,		/* receive data comp */
	FW_UP_STATE_WAIT_WRITE_CHECK,		/* wait write check */
	FW_UP_STATE_RECEIVE_DATA_FINISH,	/* receive data finish */
	FW_UP_STATE_WAIT_UPDATE_CHECK		/* wait update check */
};

/**************************** Function definition **************************************/
static void RBLE_FWUP_GAP_CallBack( RBLE_GAP_EVENT *event );
static void RBLE_FWUP_SM_CallBack( RBLE_SM_EVENT *event );
static void RBLE_APP_FWUPR_CallBack( RBLE_FWUPR_EVENT *event);
#ifdef _USE_IAR_RL78
static RBLE_STATUS FW_Update_Std_Write( void );
#endif

/**************************** PRIVATE Memory Declaration ******************************/
_MAINCNST	const	uint16_t			RBLE_FWUP_Broadcast_Enable_Param_disc_mode = RBLE_GAP_GEN_DISCOVERABLE;
_MAINCNST	const	uint16_t			RBLE_FWUP_Broadcast_Enable_Param_conn_mode = RBLE_GAP_UND_CONNECTABLE;
_MAINCNST	const	RBLE_ADV_INFO		RBLE_FWUP_Broadcast_Enable_Param_adv_info  = 
{	/* General Discoverable & Undirected Connectable -- L2CAP:15, GAP:9 */
	{
		0x80,									/* AdvIntervalMin ( 1.28s = 0x800 * 0.625msec ) Range:0x0020-0x4000 */
		0x80,									/* AdvIntervalMax ( 1.28s = 0x800 * 0.625msec ) Range:0x0020-0x4000 */
		RBLE_GAP_ADV_CONN_UNDIR,				/* AdvType */
		RBLE_ADDR_PUBLIC,						/* OwnAddrType */
		RBLE_ADDR_PUBLIC,						/* DirectAddrType */
		{ 0xf1, 0x00, 0xde, 0xfa, 0xfe, 0xca }, /* DirectAddr */
		RBLE_ADV_ALL_CHANNELS,					/* AdvChannelMap */
		RBLE_ADV_ALLOW_SCAN_ANY_CON_ANY,		/* AdvFiltPolicy */
		0x00,									/* Pad */
	},
	{
		/* AdvDataLen */
		3+13,
		/* AdvData */
		2,  0x01, 0x06,															/* Flags ( LE General Discoverable Mode | BR/EDR Not Supported ) */ 
		12, 0x09, 'R', 'e', 'n', 'e', 's', 'a', 's', '-', 'B', 'L', 'E',		/* Complete local name */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	},
	{
		/* ScanRspDataLen */
		0,
		/* ScanData */
		0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
	}
};

_MAINDATA uint16_t fw_up_conhdl;

_MAINDATA uint8_t fw_up_state;

_MAINDATA uint16_t exp_size;
_MAINDATA uint8_t write_block;

_MAINDATA uint16_t receive_size;
_MAINDATA uint8_t *receive_data;

_MAINDATA uint8_t *std_check_data;
	
_MAINDATA uint8_t *std_data;

extern const __far uint16_t fw_update_count_0;
extern const __far uint16_t fw_update_count_1;

extern bool check_fw_update( void );

/******************************* Program Area *****************************************/

/******************************************************************************************/
/**
 *  @brief FW Update  Application Test Start
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE static RBLE_STATUS FW_Update_Receiver_Start( void )
{
	bool codeflash_ret;
	RBLE_STATUS ret = RBLE_OK;
	CODEFLASH_INIT_INFO fsl_info;
	uint8_t block_num;
	
	fw_up_state = FW_UP_STATE_IDLE;
	fw_up_conhdl = ILLEGAL_CONNECTION_HANDLE;
	
	/* init FSL IF */
	fsl_info.voltage_mode = FSL_VOL_MODE_SET;
	fsl_info.freqency = FSL_FREQ_MHZ_SET;
	codeflash_ret = codeflash_enable(&fsl_info);
    
	if( codeflash_ret == true ) {
		if(false == check_fw_update()) {
			/* Erase Boot cluster 1 */
			for(block_num = 4; block_num < 8; block_num++ )
			{
				codeflash_ret = codeflash_erase(block_num);
				if(codeflash_ret != true) {
					break;
				}
			}
#ifdef _USE_IAR_RL78
			if( codeflash_ret == true ) {
				/* Erase tmp standard library block */
				for(block_num = FWUP_TMP_STD_START_BLK; block_num <= FWUP_TMP_STD_END_BLK;block_num++)
				{
					codeflash_ret = codeflash_erase(block_num);
					if(codeflash_ret != true) {
						break;
					}
				}
			}
#endif
			if( codeflash_ret == true ) {
				codeflash_forcereset();
			}
			else {
				codeflash_disable();
				ret = RBLE_ERR;
			}
		}
		else {
#ifdef _USE_IAR_RL78
			std_check_data = (uint8_t *)(FWUP_TMP_STD_END_BLK * 0x400 + 0x3FF);
			if((*std_check_data) == 0x01) {
				ret = FW_Update_Std_Write();
			}
			else
#endif
			{
				receive_data = (uint8_t*)ke_malloc(FW_UP_BLOCK_SIZE);
				RBLE_GAP_Reset( &RBLE_FWUP_GAP_CallBack, &RBLE_FWUP_SM_CallBack );
			}
		}
	}
	else {
		ret = RBLE_ERR;
	}
	return ret;
}

/******************************************************************************************/
/**
 *  @brief FW Update Profile Enable
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE static void FW_Update_Receiver_Enable( void )
{
	RBLE_STATUS ret;
	
	ret = RBLE_FWUP_Receiver_Enable(fw_up_conhdl, RBLE_APP_FWUPR_CallBack );
	
	if( ret != RBLE_OK ) {
//		printf("FWUP Receiver Enable Error!! (%x)\n", ret);
		codeflash_disable();
	}
}

/******************************************************************************************/
/**
 *  @brief Write FW Update data.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE static RBLE_STATUS FW_Update_Write_FSL( void )
{
	RBLE_STATUS ret = RBLE_OK;
	bool codeflash_ret;
	
	if(write_block < 4) {
		write_block += 4;
	}
	else if( write_block == FWUP_CNG_CNST_BLOCK ) {
		if( fw_update_count_0 % 2 == 0 ) {
			write_block += 1;
		}
	}
#ifdef _USE_IAR_RL78
	else if ( (FWUP_STD_START_BLK <= write_block) && (write_block <= FWUP_STD_END_BLK) ) {
		write_block = write_block - FWUP_STD_START_BLK + FWUP_TMP_STD_START_BLK;
	}
#endif

	/* FSL Erase */
	codeflash_ret = codeflash_erase(write_block);
	
	if( codeflash_ret != true ) {
//		printf("FSL Erase Error!!\n");
		ret = RBLE_ERR;
	}
	else {
		if( write_block == 7 ) {
			*((uint16_t*)(&receive_data[0x400-2])) = fw_update_count_0 + 1;
		}
#ifdef _USE_IAR_RL78
		if( write_block == FWUP_TMP_STD_END_BLK ) {
			receive_data[0x400-1] = 0x01;
		}
#endif

		codeflash_ret = codeflash_write(&receive_data[0], receive_size, write_block);
		if( codeflash_ret == true ) {
//			printf("Write Comp(%d)\n", write_block);
			fw_up_state = FW_UP_STATE_WAIT_WRITE_CHECK;
		}
		else {
//			printf("FSL Write Error!!(%d)\n", write_block);
			ret = RBLE_ERR;
		}
		(void)RBLE_GAP_Broadcast_Enable(RBLE_FWUP_Broadcast_Enable_Param_disc_mode, 
										RBLE_FWUP_Broadcast_Enable_Param_conn_mode, 
										(RBLE_ADV_INFO *)(&RBLE_FWUP_Broadcast_Enable_Param_adv_info) );
	}
	return ret;
}

#ifdef _USE_IAR_RL78
/******************************************************************************************/
/**
 *  @brief Write Standard library data.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE static RBLE_STATUS FW_Update_Std_Write( void )
{
	uint8_t block_num;
	RBLE_STATUS ret = RBLE_OK;
	bool codeflash_ret;

	std_data = (uint8_t *)ke_malloc(0x400);
	for(block_num = 0; block_num < (FWUP_TMP_STD_END_BLK - FWUP_TMP_STD_START_BLK + 1); block_num++)
	{
		if(codeflash_blankcheck(FWUP_TMP_STD_START_BLK + block_num) != true)
		{
			codeflash_ret = codeflash_erase(FWUP_STD_START_BLK + block_num);
			if(codeflash_ret != true) {
				break;
			}
			else {
				uint16_t i;
				
				/* read write data */
				for(i = 0; i < 0x400;i++) {
					std_data[i] = *(__far uint8_t *)(((FWUP_TMP_STD_START_BLK + block_num) * 0x400) + i);
				}
				codeflash_ret = codeflash_write( std_data, 0x400, FWUP_STD_START_BLK + block_num);
				if(codeflash_ret != true) {
					break;
				}
				else {
					/* erase tmp block */
					codeflash_ret = codeflash_erase(FWUP_TMP_STD_START_BLK + block_num);
					if(codeflash_ret != true) {
						break;
					}
				}
			}
		}
	}
	if(codeflash_ret != true) {
		ret = RBLE_ERR;
	}

	ke_free(std_data);
	return ret;
}
#endif

/******************************************************************************************/
/**
 *  @brief FW Update  Application interrupt(from sw2)
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE void FW_Update_push_sw_int( void )
{
	(void)FW_Update_Receiver_Start();
}

/******************************************************************************************/
/**
 *  @brief Callback of FW Update profile.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE void RBLE_FWUP_CallBack( RBLE_MODE Mode )
{
	switch( Mode ) {
		case RBLE_MODE_INITIALIZE:
			break;
		case RBLE_MODE_ACTIVE:
			(void)FW_Update_Receiver_Start();
			break;
		case RBLE_MODE_RESET:
			break;
		case RBLE_MODE_SLEEP:
			break;
		case RBLE_MODE_ERROR:
			break;
		default:
			break;
	}
}

/******************************************************************************************/
/**
 *  @brief Callback of FW Update profile.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE static void RBLE_APP_FWUPR_CallBack( RBLE_FWUPR_EVENT *event)
{
	switch( event->type){
		case RBLE_FWUP_EVENT_RECEIVER_ENABLE_COMP:
			if( event->param.receiver_enable.status != RBLE_OK ) {
				/* update end */
//				printf("enable error!!  update end\n");
				codeflash_disable();
			}
			else {
				if(fw_up_state == FW_UP_STATE_IDLE) {
					fw_up_state = FW_UP_STATE_WAIT_START;
				}
				else {
					/* do nothing */
				}
			}
			break;
		case RBLE_FWUP_EVENT_RECEIVER_CHG_DATA_CNTL_IND:
			switch(event->param.data_cntl_ind.type)
			{
				case RBLE_FWUP_DATA_SEND_START:
					if((fw_up_state == FW_UP_STATE_WAIT_START) || (FW_UP_STATE_WAIT_WRITE_CHECK)) {
						write_block = event->param.data_cntl_ind.block_num;
						exp_size	= event->param.data_cntl_ind.data_size;
						receive_size = 0;
						memset(&receive_data[0], 0xff, sizeof(receive_data));
						
						RBLE_FWUP_Receiver_Send_Data_Cntl_Res(fw_up_conhdl, RBLE_OK);
						
						fw_up_state = FW_UP_STATE_RECEIVE_DATA;
					}
					else {
//						printf(" error status (%d)\n", fw_up_state);
					}
					break;
				case RBLE_FWUP_DATA_SEND_COMP:
					if( (fw_up_state == FW_UP_STATE_RECEIVE_DATA) &&
					    (exp_size == receive_size) ) {
						RBLE_FWUP_Receiver_Send_Data_Cntl_Res(fw_up_conhdl, RBLE_OK);
						fw_up_state = FW_UP_STATE_RECEIVE_DATA_COMP;
						
						RBLE_FWUP_Receiver_Disable(fw_up_conhdl);
					}
					else {
						RBLE_FWUP_Receiver_Send_Data_Cntl_Res(fw_up_conhdl, RBLE_ERR);
#if 0
						printf("received size = %d\n",receive_size);
						{
							uint16_t i;
							for(i = 0; i < receive_size; i++) {
								printf("0x%02x ", receive_data[i]);
								if(i % 16 == 15) {
									printf("\n");
								}
							}
						}
#endif
						fw_up_state = FW_UP_STATE_WAIT_START;
					}
					break;
				case RBLE_FWUP_DATA_CHECK_WRITE:
					if(fw_up_state == FW_UP_STATE_WAIT_WRITE_CHECK) {
						RBLE_FWUP_Receiver_Send_Data_Cntl_Res(fw_up_conhdl, RBLE_OK);
						fw_up_state = FW_UP_STATE_WAIT_START;
					}
					else {
						RBLE_FWUP_Receiver_Send_Data_Cntl_Res(fw_up_conhdl, RBLE_ERR);
						fw_up_state = FW_UP_STATE_WAIT_START;
					}
					break;
				case RBLE_FWUP_DATA_SEND_FINISH:
					if(fw_up_state == FW_UP_STATE_WAIT_WRITE_CHECK) {
						RBLE_FWUP_Receiver_Send_Data_Cntl_Res(fw_up_conhdl, RBLE_OK);
						fw_up_state = FW_UP_STATE_RECEIVE_DATA_FINISH;
						
						RBLE_FWUP_Receiver_Disable(fw_up_conhdl);
					}
					else {
						RBLE_FWUP_Receiver_Send_Data_Cntl_Res(fw_up_conhdl, RBLE_ERR);
						fw_up_state = FW_UP_STATE_WAIT_START;
					}
					break;
				default:
					break;
			}
			break;
		case RBLE_FWUP_EVENT_RECEIVER_CHG_DATA_IND:
			if(fw_up_state == FW_UP_STATE_RECEIVE_DATA) {
				uint8_t tmp=0,i;
				for(i = 0; i < (event->param.data_ind.data_size - 1); i++) {
					tmp += event->param.data_ind.data[i];
				}
				if(tmp == event->param.data_ind.data[event->param.data_ind.data_size-1]) {
					memcpy(&receive_data[receive_size], &(event->param.data_ind.data[0]), event->param.data_ind.data_size-1);
					receive_size += event->param.data_ind.data_size-1;
				}
			}
			break;
		default:
			break;
	}
}

/******************************************************************************************/
/**
 *  @brief Callback of GAP.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE static void RBLE_FWUP_GAP_CallBack( RBLE_GAP_EVENT *event )
{
	switch( event->type ) {
		case RBLE_GAP_EVENT_RESET_RESULT:
			/* create connection */
			(void)RBLE_GAP_Broadcast_Enable(RBLE_FWUP_Broadcast_Enable_Param_disc_mode,
											RBLE_FWUP_Broadcast_Enable_Param_conn_mode,
											(RBLE_ADV_INFO *)(&RBLE_FWUP_Broadcast_Enable_Param_adv_info) );
			break;
		case RBLE_GAP_EVENT_CONNECTION_COMP:
			fw_up_conhdl = event->param.conn_comp.connect_info.conhdl;
			
			/* prepare FW update profile */
			FW_Update_Receiver_Enable();
			break;
		case RBLE_GAP_EVENT_DISCONNECT_COMP:
			switch(fw_up_state)
			{
				case FW_UP_STATE_RECEIVE_DATA_COMP:
					/* write receive data */
					FW_Update_Write_FSL();
					break;
				case FW_UP_STATE_RECEIVE_DATA_FINISH:
					{
						RBLE_STATUS ret;
						
#ifdef _USE_IAR_RL78
						/* write standard library data */
						ret = FW_Update_Std_Write();
#else
						ret = RBLE_OK;
#endif
						if(ret == RBLE_OK) {
							(void)codeflash_invertbootflag();
							codeflash_forcereset();
						}
					}
					break;
				case FW_UP_STATE_IDLE:
				/* fw update is finish */
					codeflash_forcereset();
					break;
				default:
			/* create connection */
					RBLE_GAP_Reset( &RBLE_FWUP_GAP_CallBack, &RBLE_FWUP_SM_CallBack );
					break;
			}
			break;
		default:
			break;
	}
}

/******************************************************************************************/
/**
 *  @brief Callback of SM.
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
_MAINCODE static void RBLE_FWUP_SM_CallBack( RBLE_SM_EVENT *event )
{
	;
}

#endif /* USE_FW_UPDATE_PROFILE */
