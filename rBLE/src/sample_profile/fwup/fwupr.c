/**
 ****************************************************************************************
 *
 * @file fwupr.c
 *
 * @brief Software Update Profile Receiver implementation.
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 * $Rev: 2311 $
 *
 ****************************************************************************************
 */
 
/*
 * INCLUDE FILES
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

#include	"rble.h"
#include	"rble_api.h"
#include	"rble_app.h"

#include	"rble_api_fwup.h"
#include	"db_handle.h"

#include	<stdio.h>
#include	<string.h>


#ifdef USE_FW_UPDATE_PROFILE

#define DATA_EVENT_MAX 3

typedef enum FWUPR_STATUS_enum {
	FWUPR_STATUS_IDLE,
	FWUPR_STATUS_CONNECTED
}FWUPR_STATUS;

typedef struct FWUPR_INFO_t {
	FWUPR_STATUS status;
	RBLE_FWUPR_EVENT_HANDLER callback;
	uint16_t conhdl;
	uint16_t ind_en;
	uint16_t set_data_hdl[DATA_EVENT_MAX];
	uint16_t wp;
	uint16_t rp;
	RBLE_GATT_EVENT event_data;
}FWUPR_INFO;

_MAINDATA FWUPR_INFO fwupr_info;

static void fwupr_send_chg_data_cntl(uint8_t *value, uint8_t val_len);
static void fwupr_send_chg_data(uint8_t *value, uint8_t val_len);
//static RBLE_STATUS fwupr_set_data(uint16_t val_hdl, uint8_t *data, uint16_t data_len);
static void fwupr_send_response(uint16_t att_hdl, RBLE_STATUS result);
static void gatt_callback( RBLE_GATT_EVENT *event );

/*
 * IF FUNCTION DEFINITIONS
 ****************************************************************************************
 */
_MAINCODE RBLE_STATUS RBLE_FWUP_Receiver_Enable( uint16_t conhdl,
									 RBLE_FWUPR_EVENT_HANDLER call_back )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_FWUPR_EVENT fwup_event;
	
	memset(&fwupr_info, 0, sizeof(fwupr_info));
	/* parameter check */
	if( call_back != NULL )
	{
		/* initialise GATT */
		result = RBLE_GATT_Enable(gatt_callback);
		if (result == RBLE_OK)
		{
			/* initialise fwupr info */
			memset(&fwupr_info,0,sizeof(fwupr_info));
			fwupr_info.callback = call_back;
			fwupr_info.conhdl   = conhdl;
			
			fwupr_info.status = FWUPR_STATUS_CONNECTED;
			
			/* enable complete callback */
			fwup_event.type = RBLE_FWUP_EVENT_RECEIVER_ENABLE_COMP;
			fwup_event.param.receiver_enable.conhdl = fwupr_info.conhdl;
			fwup_event.param.receiver_enable.status = result;
			
			fwupr_info.callback(&fwup_event);
		}
	}
	else
	{
		/* parameter error */
		result = RBLE_PARAM_ERR;
	}
	
	return result;
}

_MAINCODE RBLE_STATUS RBLE_FWUP_Receiver_Disable( uint16_t conhdl )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_FWUPR_EVENT	 fwup_event;
	
	if(fwupr_info.status == FWUPR_STATUS_CONNECTED)
	{
		if(conhdl == fwupr_info.conhdl)
		{
			fwupr_info.status = FWUPR_STATUS_IDLE;

			/* disable complete callback */
			fwup_event.type = RBLE_FWUP_EVENT_RECEIVER_DISABLE_COMP;
			fwup_event.param.receiver_disable.conhdl = conhdl;
			fwup_event.param.receiver_disable.status = RBLE_OK;
			
			fwupr_info.callback(&fwup_event);
		}
		else
		{
			/* parameter error */
			result = RBLE_PARAM_ERR;
		}
	}
	else
	{
		/* status error */
		result = RBLE_STATUS_ERROR;
	}
	
	return result;
}

_MAINCODE RBLE_STATUS RBLE_FWUP_Receiver_Send_Data_Cntl_Res( uint16_t conhdl,
										 		 RBLE_STATUS status )
{
	RBLE_STATUS result = RBLE_OK;
	
	if(fwupr_info.status == FWUPR_STATUS_CONNECTED)
	{
		if(conhdl == fwupr_info.conhdl)
		{
			fwupr_send_response(FWUS_HDL_DATA_CNTL_VAL,status);
		}
		else {
			/* parameter error */
			result = RBLE_PARAM_ERR;
		}
	}
	else
	{
		/* status error */
		result = RBLE_STATUS_ERROR;
	}
	
	return result;
}

/*
 * Internal FUNCTION DEFINITIONS
 ****************************************************************************************
 */
_MAINCODE static void fwupr_send_chg_data_cntl(uint8_t *value, uint8_t val_len)
{
	RBLE_FWUPR_EVENT	fwup_event;
	fwup_event.type = RBLE_FWUP_EVENT_RECEIVER_CHG_DATA_CNTL_IND;
	fwup_event.param.data_cntl_ind.conhdl	  = fwupr_info.conhdl;
	fwup_event.param.data_cntl_ind.type = value[0];
	fwup_event.param.data_cntl_ind.block_num = value[1];
	memcpy(&fwup_event.param.data_cntl_ind.data_size, &value[2], 2);
	
	/* send write callback */
	fwupr_info.callback(&fwup_event);
}

_MAINCODE static void fwupr_send_chg_data(uint8_t *value, uint8_t val_len)
{
	RBLE_FWUPR_EVENT	fwup_event;
	fwup_event.type = RBLE_FWUP_EVENT_RECEIVER_CHG_DATA_IND;
	fwup_event.param.data_ind.conhdl	  = fwupr_info.conhdl;
	fwup_event.param.data_ind.data_size = val_len;
	memcpy(&fwup_event.param.data_ind.data, &value[0], val_len);
	
	/* send write callback */
	fwupr_info.callback(&fwup_event);
}
#if 0
_MAINCODE static RBLE_STATUS fwupr_set_data(uint16_t val_hdl, uint8_t *data, uint16_t data_len)
{
	RBLE_STATUS result;
	RBLE_GATT_SET_DATA set_data;
	
	set_data.val_len = data_len;
	set_data.val_hdl = val_hdl;
	memcpy(&set_data.value[0], data, data_len);
	
	fwupr_info.set_data_hdl[fwupr_info.wp] = val_hdl;
	fwupr_info.wp = (fwupr_info.wp + 1) % DATA_EVENT_MAX;
	
	result = RBLE_GATT_Set_Data(&set_data);
	
	return result;
}
#endif
_MAINCODE static void fwupr_send_response(uint16_t att_hdl, RBLE_STATUS result)
{
	RBLE_GATT_WRITE_RESP wr_resp;
	
	if(result != RBLE_OK)
	{
		wr_resp.att_code = RBLE_ATT_ERR_APP_ERROR;
	}
	else
	{
		wr_resp.att_code = RBLE_ATT_ERR_NO_ERROR;
	}
	wr_resp.conhdl   = fwupr_info.conhdl;
	wr_resp.att_hdl  = att_hdl;
	RBLE_GATT_Write_Response(&wr_resp);
	
	return;
}

_MAINCODE static void gatt_callback( RBLE_GATT_EVENT *event )
{
	switch(event->type)
	{
			case RBLE_GATT_EVENT_WRITE_CMD_IND:
				switch(event->param.write_cmd_ind.elmt)
				{
					case FWUS_HDL_DATA_CNTL_VAL:
						fwupr_send_chg_data_cntl(event->param.write_cmd_ind.value, (uint8_t)event->param.write_cmd_ind.size);
						break;
					case FWUS_HDL_DATA_VAL:
						/* without response */
						fwupr_send_chg_data(event->param.write_cmd_ind.value, (uint8_t)event->param.write_cmd_ind.size);
						break;
					default:
						/* could't come here */
						break;
				}
			break;
		case RBLE_GATT_EVENT_RESP_TIMEOUT:
		case RBLE_GATT_EVENT_COMMAND_DISALLOWED_IND:
		default:
			break;
	}
}

#endif /* #ifdef USE_FW_UPDATE_PROFILE */
