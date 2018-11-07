/**
 ****************************************************************************************
 *
 * @file scps.c
 *
 * @brief Sample Custom Profile Server implementation.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 2311 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup SCPSTASK
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#include	"arch.h"
#include	"rwble_config.h"

#include	"rble.h"
#include	"rble_app.h"
#include	"db_handle.h"

#include	<stdio.h>
#include	<string.h>


#ifdef USE_SAMPLE_PROFILE

#define DATA_EVENT_MAX 3

typedef enum SCPS_STATUS_enum {
	SCPS_STATUS_IDLE,
	SCPS_STATUS_CONNECTED
}SCPS_STASUS;

typedef struct SCPS_INFO_t {
	SCPS_STASUS status;
	RBLE_SCPS_EVENT_HANDLER callback;
	uint16_t conhdl;
	uint16_t notify_en;
	uint16_t ind_en;
	uint16_t set_data_hdl[DATA_EVENT_MAX];
	uint16_t wp;
	uint16_t rp;
	RBLE_GATT_EVENT event_data;
}SCPS_INFO;

SCPS_INFO scps_info = {
	SCPS_STATUS_IDLE,		/* status */
	NULL,					/* callback */
	0,						/* conhdl */
	RBLE_SCP_STOP_NTFIND,	/* notify en */
	RBLE_SCP_STOP_NTFIND,	/* ind en */
	{0},					/* set_data_hdl */
	0,						/* wp */
	0,						/* rp */
	{0}						/* event_data */
};

static void scps_send_enable_complete(RBLE_STATUS status);
static void scps_send_chg_IndNtf(uint8_t char_code, uint8_t *value);
static void scps_send_chg_char(uint8_t char_code, uint8_t *value, uint8_t val_len);
static RBLE_STATUS scps_set_data(uint16_t val_hdl, uint8_t *data, uint16_t data_len);
static void scps_send_response(uint16_t att_hdl, RBLE_STATUS result);
static void gatt_callback( RBLE_GATT_EVENT *event );

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
RBLE_STATUS RBLE_SCP_Server_Enable( uint16_t conhdl,
									uint8_t  con_type,
									RBLE_SCP_SERVER_PARAM *param,
									RBLE_SCPS_EVENT_HANDLER call_back )
{
	RBLE_STATUS result = RBLE_OK;
	
	if(scps_info.status == SCPS_STATUS_IDLE)
	{
		/* parameter check */
		if( (param != NULL) && (call_back != NULL) )
		{
			/* initialise GATT */
			result = RBLE_GATT_Enable(gatt_callback);
			if (result == RBLE_OK)
			{
				/* initialise scps info */
				memset(&scps_info,0,sizeof(scps_info));
				scps_info.callback = call_back;
				scps_info.conhdl   = conhdl;
				
				if (con_type == RBLE_SCP_CON_NORMAL)
				{
					scps_info.notify_en = param->data_ntf_en;
					scps_info.ind_en    = param->data_ind_en;
				}
				else
				{
					scps_info.notify_en = RBLE_SCP_STOP_NTFIND;
					scps_info.ind_en    = RBLE_SCP_STOP_NTFIND;
				}
				/* set notify client configuration */
				result = scps_set_data(SCS_HDL_NOTIFY_CFG, (uint8_t*)&scps_info.notify_en, 2);
			}
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

RBLE_STATUS RBLE_SCP_Server_Disable( uint16_t conhdl )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_SCPS_EVENT		 scp_event;
	
	if(scps_info.status == SCPS_STATUS_CONNECTED)
	{
		if(conhdl == scps_info.conhdl)
		{
			scps_info.status = SCPS_STATUS_IDLE;

			/* disable complete callback */
			scp_event.type = RBLE_SCP_EVENT_SERVER_DISABLE_COMP;
			scp_event.param.server_disable.conhdl = conhdl;
			scp_event.param.server_disable.status = RBLE_OK;
			
			scps_info.callback(&scp_event);
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

RBLE_STATUS RBLE_SCP_Server_Send_Notify( uint16_t conhdl,
										 RBLE_SCP_NOTIFY_INFO *notify_info )
{
	RBLE_STATUS result;
	
	if(scps_info.status == SCPS_STATUS_CONNECTED)
	{
		if ((conhdl == scps_info.conhdl) &&
			(scps_info.notify_en & RBLE_SCP_START_NTF))
		{
			/* set data */
			result = scps_set_data(SCS_HDL_NOTIFY_VAL, notify_info->data, notify_info->data_len);
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

RBLE_STATUS RBLE_SCP_Server_Send_Indicate( uint16_t conhdl,
										   RBLE_SCP_IND_INFO *ind_info )
{
	RBLE_STATUS result;
	
	if(scps_info.status == SCPS_STATUS_CONNECTED)
	{
		if ((conhdl == scps_info.conhdl) &&
			(scps_info.ind_en & RBLE_SCP_START_IND))
		{
			/* set data */
			result = scps_set_data(SCS_HDL_IND_VAL, ind_info->data, ind_info->data_len);
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

static void scps_send_enable_complete(RBLE_STATUS status)
{
	RBLE_SCPS_EVENT	scp_event;
	
	if(status == RBLE_OK)
	{
		scps_info.status = SCPS_STATUS_CONNECTED;
	}
	/* enable complete callback */
	scp_event.type = RBLE_SCP_EVENT_SERVER_ENABLE_COMP;
	scp_event.param.server_enable.conhdl = scps_info.conhdl;
	scp_event.param.server_enable.status = status;
	
	scps_info.callback(&scp_event);
}

static void scps_send_chg_IndNtf(uint8_t char_code, uint8_t *value)
{
	RBLE_SCPS_EVENT	scp_event;
	uint16_t tmp_data;
	
	memcpy(&tmp_data, value, 2);
	if(char_code == RBLE_SCP_SCS_NTF_CFG)
	{
		scps_info.notify_en = tmp_data;
	}
	else
	{
		scps_info.ind_en = tmp_data;
	}
	/* send write callback */
	scp_event.type = RBLE_SCP_EVENT_SERVER_CHG_INDNTF_IND;
	scp_event.param.cfg_indntf.conhdl	 = scps_info.conhdl;
	scp_event.param.cfg_indntf.char_code = char_code;
	scp_event.param.cfg_indntf.cfg_val	 = tmp_data;
	scps_info.callback(&scp_event);
}

static void scps_send_chg_char(uint8_t char_code, uint8_t *value, uint8_t val_len)
{
	RBLE_SCPS_EVENT	scp_event;
	scp_event.type = RBLE_SCP_EVENT_SERVER_CHG_CHAR_IND;
	scp_event.param.write_char.conhdl	 = scps_info.conhdl;
	scp_event.param.write_char.char_code = char_code;
	memcpy(&scp_event.param.write_char.value[0], value, val_len);
	/* send write callback */
	scps_info.callback(&scp_event);
}

static RBLE_STATUS scps_set_data(uint16_t val_hdl, uint8_t *data, uint16_t data_len)
{
	RBLE_STATUS result;
	RBLE_GATT_SET_DATA set_data;
	
	set_data.val_len = data_len;
	set_data.val_hdl = val_hdl;
	memcpy(&set_data.value[0], data, data_len);
	
	scps_info.set_data_hdl[scps_info.wp] = val_hdl;
	scps_info.wp = (scps_info.wp + 1) % DATA_EVENT_MAX;
	
	result = RBLE_GATT_Set_Data(&set_data);
	
	return result;
}

static void scps_send_response(uint16_t att_hdl, RBLE_STATUS result)
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
	wr_resp.conhdl   = scps_info.conhdl;
	wr_resp.att_hdl  = att_hdl;
	RBLE_GATT_Write_Response(&wr_resp);
	
	return;
}

static void gatt_callback( RBLE_GATT_EVENT *event )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_SCPS_EVENT		 scp_event;
	uint16_t tmp_data,tmp_rp,val_len;
	
	switch(event->type)
	{
		case RBLE_GATT_EVENT_HANDLE_VALUE_CFM:
			/* send indicate complete event */
			scp_event.type = RBLE_SCP_EVENT_SERVER_SEND_IND_COMP;
			scp_event.param.send_ind.conhdl = scps_info.conhdl;
			scp_event.param.send_ind.status = event->param.handle_value_cfm.status;
			scps_info.callback(&scp_event);
			break;
		case RBLE_GATT_EVENT_WRITE_CMD_IND:
			val_len = 0;
			switch(event->param.write_cmd_ind.elmt)
			{
				case SCS_HDL_NOTIFY_CFG:
					memcpy(&tmp_data, &event->param.write_cmd_ind.value[0], 2);
					if((tmp_data & RBLE_SCP_START_IND) == 0)
					{
						val_len = 2;
					}
					break;
				case SCS_HDL_IND_CFG:
					memcpy(&tmp_data, &event->param.write_cmd_ind.value[0], 2);
					if((tmp_data & RBLE_SCP_START_NTF) == 0)
					{
						val_len = 2;
					}
					break;
				case SCS_HDL_INTERVAL_VAL:
					val_len = 2;
					break;
				case SCS_HDL_NOTIFY_LEN_VAL:
				case SCS_HDL_IND_LEN_VAL:
					val_len = 1;
					break;
				default:
					/* could't come here */
					break;
			}
			if(val_len != 0)
			{
				/* update data base */
				scps_info.event_data = *event;
				result = scps_set_data(event->param.write_cmd_ind.elmt, &event->param.write_cmd_ind.value[0], val_len);
			}
			if((result != RBLE_OK) || (val_len == 0))
			{
				if(event->param.write_cmd_ind.resp == TRUE)
				{
					/* send error response */
					scps_send_response(event->param.write_cmd_ind.elmt, RBLE_ERR);
				}
			}
			break;
		case RBLE_GATT_EVENT_SET_DATA_CMP:
			tmp_rp = scps_info.rp;
			/* update read point */
			scps_info.rp = (scps_info.rp + 1) % DATA_EVENT_MAX;
			
			result = event->param.set_data_cmp.status;
			switch(scps_info.set_data_hdl[tmp_rp])
			{
				case SCS_HDL_NOTIFY_CFG:
					if (scps_info.status == SCPS_STATUS_CONNECTED)
					{
						if(result == RBLE_OK)
						{
							/* send write event to app */
							scps_send_chg_IndNtf(RBLE_SCP_SCS_NTF_CFG, &scps_info.event_data.param.write_cmd_ind.value[0]);
						}
						/* send error response */
						if(scps_info.event_data.param.write_cmd_ind.resp == TRUE)
						{
							scps_send_response(scps_info.set_data_hdl[tmp_rp], result);
						}
					}
					else
					{
						if(result == RBLE_OK)
						{
							/* set indicate client configuration */
							result = scps_set_data(SCS_HDL_IND_CFG, (uint8_t*)&scps_info.ind_en, 2);
						}
						else
						{
							/* enable complete callback */
							scps_send_enable_complete(result);
						}
					}
					break;
				case SCS_HDL_IND_CFG:
					if (scps_info.status == SCPS_STATUS_CONNECTED)
					{
						if(result == RBLE_OK)
						{
							/* send write event to app */
							scps_send_chg_IndNtf(RBLE_SCP_SCS_IND_CFG, &scps_info.event_data.param.write_cmd_ind.value[0]);
						}
						/* send response */
						if(scps_info.event_data.param.write_cmd_ind.resp == TRUE)
						{
							scps_send_response(scps_info.set_data_hdl[tmp_rp], result);
						}
					}
					else
					{
						/* enable complete callback */
						scps_send_enable_complete(result);
					}
					break;
				case SCS_HDL_INTERVAL_VAL:
					if(result == RBLE_OK)
					{
						scps_send_chg_char(RBLE_SCP_SCS_INTERVAL, &scps_info.event_data.param.write_cmd_ind.value[0], 2);
					}
					/* send response */
					if(scps_info.event_data.param.write_cmd_ind.resp == TRUE)
					{
						scps_send_response(scps_info.set_data_hdl[tmp_rp], result);
					}
					break;
				case SCS_HDL_NOTIFY_LEN_VAL:
					if(result == RBLE_OK)
					{
						scps_send_chg_char(RBLE_SCP_SCS_NTF_LEN, &scps_info.event_data.param.write_cmd_ind.value[0], 1);
					}
					/* send response */
					if(scps_info.event_data.param.write_cmd_ind.resp == TRUE)
					{
						scps_send_response(scps_info.set_data_hdl[tmp_rp], result);
					}
					break;
				case SCS_HDL_IND_LEN_VAL:
					if(result == RBLE_OK)
					{
						scps_send_chg_char(RBLE_SCP_SCS_IND_LEN, &scps_info.event_data.param.write_cmd_ind.value[0], 1);
					}
					/* send response */
					if(scps_info.event_data.param.write_cmd_ind.resp == TRUE)
					{
						scps_send_response(scps_info.set_data_hdl[tmp_rp], result);
					}
					break;
				case SCS_HDL_NOTIFY_VAL:
					if(result == RBLE_OK)
					{
						RBLE_GATT_NOTIFY_REQ notify;
						/* send notify */
						notify.conhdl  = scps_info.conhdl;
						notify.charhdl = SCS_HDL_NOTIFY_VAL;
						result = RBLE_GATT_Notify_Request(&notify);
					}
					/* notify complete event */
					scp_event.type = RBLE_SCP_EVENT_SERVER_SEND_NOTIFY_COMP;
					scp_event.param.send_notify.conhdl = scps_info.conhdl;
					scp_event.param.send_notify.status = result;
					scps_info.callback(&scp_event);
					break;
				case SCS_HDL_IND_VAL:
					if(result == RBLE_OK)
					{
						RBLE_GATT_INDICATE_REQ  ind;
						/* send notify */
						ind.conhdl  = scps_info.conhdl;
						ind.charhdl = SCS_HDL_IND_VAL;
						result = RBLE_GATT_Indicate_Request(&ind);
					}
					else
					{
						/* send indicate complete event */
						scp_event.type = RBLE_SCP_EVENT_SERVER_SEND_IND_COMP;
						scp_event.param.send_ind.conhdl = scps_info.conhdl;
						scp_event.param.send_ind.status = result;
						scps_info.callback(&scp_event);
					}
					break;
				default:
					break;
			}
			break;
		case RBLE_GATT_EVENT_RESP_TIMEOUT:
		case RBLE_GATT_EVENT_COMMAND_DISALLOWED_IND:
		default:
			break;
	}
}

#endif /* #ifdef USE_SAMPLE_PROFILE */
