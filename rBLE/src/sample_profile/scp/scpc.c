/**
 ****************************************************************************************
 *
 * @file scpc.c
 *
 * @brief Sample Custom Profile Client implementation.
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

#include	<stdio.h>
#include	<string.h>


#ifdef USE_SAMPLE_PROFILE

#define RBLE_ATT_SVC_SMPL_CSTM		0xFFFF

typedef enum SCPC_STATUS_enum {
	SCPC_STATUS_IDLE,
	SCPC_STATUS_DISCOVERING,
	SCPC_STATUS_CONNECTED
}SCPC_STATUS;

enum
{
	SCPC_DSC_NONE = 0x00,
	SCPC_DSC_NOTIFY_CFG,
	SCPC_DSC_IND_CFG,
	SCPC_DSC_DISCOVERY_DONE = 0xFF
};

enum
{
	SCPC_CHAR_NOTIFY = 0x00,
	SCPC_CHAR_INDICATE,
	SCPC_CHAR_INTERVAL,
	SCPC_CHAR_NOTIFY_LEN,
	SCPC_CHAR_INDICATE_LEN,
	SCPC_CHAR_OTHER = 0xFF
};

typedef struct SCPC_INFO_t {
	uint16_t				conhdl;
	SCPC_STATUS				status;
	uint8_t					cfg_dsc_idx;
	uint16_t				last_uuid_req;
	RBLE_SCPC_EVENT_HANDLER	callback;
	RBLE_SCS_CONTENT		scs;
}SCPC_INFO;

/* internal function definitions */
static void gatt_cmp_evt_handler( RBLE_GATT_EVENT *event );
static void gatt_disc_svc_by_uuid_evt_handler( RBLE_GATT_EVENT *event );
static void gatt_disc_char_all_evt_handler( RBLE_GATT_EVENT *event);
static void gatt_disc_char_desc_evt_handler( RBLE_GATT_EVENT *event );
static void gatt_callback( RBLE_GATT_EVENT *event );

static void scpc_disc_svc_send( void );
static void scpc_disc_char_all_send( uint16_t shdl, uint16_t ehdl );
static void scpc_disc_char_desc_send( uint16_t shdl );
static uint16_t scpc_get_next_char_hdl( uint16_t shdl );
static uint8_t scpc_get_char_index( uint8_t *uuid );

/* global variables definitions */
SCPC_INFO scpc_info = {
	0,						/* conhdl */
	SCPC_STATUS_IDLE,		/* status */
	SCPC_DSC_NONE,			/* cfg_dsc_idx */
	0,						/* last_uuid_req */
	NULL,					/* callback */
	{0}						/* scs */
};

/* 128bit UUID definitions */
const uint8_t scpc_service_uuid[16]		= RBLE_SVC_SAMPLE_CUSTOM;
const uint8_t scpc_notify_char_uuid[16]	= RBLE_CHAR_SCP_NOTIFY;
const uint8_t scpc_indicate_char_uuid[16]	= RBLE_CHAR_SCP_INDICATE;
const uint8_t scpc_interval_char_uuid[16]	= RBLE_CHAR_SCP_INTERVAL;
const uint8_t scpc_ntf_len_char_uuid[16]	= RBLE_CHAR_SCP_NTF_LEN;
const uint8_t scpc_ind_len_char_uuid[16]	= RBLE_CHAR_SCP_IND_LEN;

RBLE_STATUS RBLE_SCP_Client_Enable( uint16_t conhdl,
									uint8_t  con_type,
									RBLE_SCS_CONTENT *scs,
									RBLE_SCPC_EVENT_HANDLER call_back )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_SCPC_EVENT	scp_event;
	
	if(scpc_info.status == SCPC_STATUS_IDLE)
	{
		/* parameter check */
		if(call_back != NULL)
		{
			/* initialise GATT */
			result = RBLE_GATT_Enable(gatt_callback);
			if (result == RBLE_OK)
			{
				/* Reset the SCPC environment */
				memset(&scpc_info, 0, sizeof(scpc_info));
				
				/* regist callback function */
				scpc_info.callback = call_back;
				
				/* set conhdl */
				scpc_info.conhdl   = conhdl;

				/* config connection, start discovering */
				if(con_type == RBLE_SCP_CON_CFG)
				{
					/* start discovering SCS on peer */
					scpc_disc_svc_send();
				}
				/* normal connection, get saved att details */
				else
				{
					scpc_info.scs = *scs;

					scpc_info.status = SCPC_STATUS_CONNECTED;

					/* enable complete callback */
					scp_event.type = RBLE_SCP_EVENT_CLIENT_ENABLE_COMP;
					scp_event.param.client_enable.conhdl = conhdl;
					scp_event.param.client_enable.status = RBLE_OK;
					scp_event.param.client_enable.scs    = scpc_info.scs;
					
					scpc_info.callback(&scp_event);
				}
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

RBLE_STATUS RBLE_SCP_Client_Disable( uint16_t conhdl )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_SCPC_EVENT	scp_event;
	
	if(scpc_info.status == SCPC_STATUS_CONNECTED)
	{
		if(conhdl == scpc_info.conhdl)
		{
			scpc_info.status = SCPC_STATUS_IDLE;
			
			/* disable complete callback */
			scp_event.type = RBLE_SCP_EVENT_CLIENT_DISABLE_COMP;
			scp_event.param.client_disable.conhdl = conhdl;
			scp_event.param.client_disable.status = RBLE_OK;
			
			scpc_info.callback(&scp_event);
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

RBLE_STATUS RBLE_SCP_Client_Read_Char( uint16_t conhdl,
									   uint8_t  char_code )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_GATT_READ_CHAR_REQ rd_char;
	uint16_t    search_hdl;
	
	if(scpc_info.status == SCPC_STATUS_CONNECTED)
	{
		if(conhdl == scpc_info.conhdl)
		{
			switch(char_code)
			{
				case RBLE_SCP_SCS_NTF_CFG:
					search_hdl = scpc_info.scs.ntf_cfg_hdl;
					rd_char.req_type	= RBLE_GATT_READ_DESC;
					break;
				case RBLE_SCP_SCS_IND_CFG:
					search_hdl = scpc_info.scs.ind_cfg_hdl;
					rd_char.req_type	= RBLE_GATT_READ_DESC;
					break;
				case RBLE_SCP_SCS_INTERVAL:
					search_hdl = scpc_info.scs.interval_val_hdl;
					rd_char.req_type	= RBLE_GATT_READ_CHAR;
					break;
				case RBLE_SCP_SCS_NTF_LEN:
					search_hdl = scpc_info.scs.ntf_len_val_hdl;
					rd_char.req_type	= RBLE_GATT_READ_CHAR;
					break;
				case RBLE_SCP_SCS_IND_LEN:
					search_hdl = scpc_info.scs.ind_len_val_hdl;
					rd_char.req_type	= RBLE_GATT_READ_CHAR;
					break;
				default:
					result = RBLE_PARAM_ERR;
					break;
			}
			if(result == RBLE_OK)
			{
				rd_char.conhdl		= conhdl;
				rd_char.uuid[0].value_size = 2;
				memcpy(&rd_char.uuid[0].value[0], &search_hdl, 2);
				
				result = RBLE_GATT_Read_Char_Request(&rd_char);
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

RBLE_STATUS RBLE_SCP_Client_Write_Char( uint16_t conhdl,
										uint8_t  char_code,
										uint8_t  *write_value )
{
	RBLE_STATUS result = RBLE_OK;
	RBLE_GATT_WRITE_CHAR_REQ wr_char;
	
	if(scpc_info.status == SCPC_STATUS_CONNECTED)
	{
		if(conhdl == scpc_info.conhdl)
		{
			switch(char_code)
			{
				case RBLE_SCP_SCS_NTF_CFG:
					memcpy(&wr_char.charhdl, &scpc_info.scs.ntf_cfg_hdl, 2);
					wr_char.req_type	= RBLE_GATT_WRITE_DESC;
					wr_char.val_len		= 2;
					memcpy(&wr_char.value[0], &write_value[0], 2);
					break;
				case RBLE_SCP_SCS_IND_CFG:
					memcpy(&wr_char.charhdl, &scpc_info.scs.ind_cfg_hdl, 2);
					wr_char.req_type	= RBLE_GATT_WRITE_DESC;
					wr_char.val_len		= 2;
					memcpy(&wr_char.value[0], &write_value[0], 2);
					break;
				case RBLE_SCP_SCS_INTERVAL:
					memcpy(&wr_char.charhdl, &scpc_info.scs.interval_val_hdl, 2);
					wr_char.req_type	= RBLE_GATT_WRITE_CHAR;
					wr_char.val_len		= 2;
					memcpy(&wr_char.value[0], &write_value[0], 2);
					break;
				case RBLE_SCP_SCS_NTF_LEN:
					memcpy(&wr_char.charhdl, &scpc_info.scs.ntf_len_val_hdl, 2);
					wr_char.req_type	= RBLE_GATT_WRITE_CHAR;
					wr_char.val_len		= 1;
					wr_char.value[0] = write_value[0];
					break;
				case RBLE_SCP_SCS_IND_LEN:
					memcpy(&wr_char.charhdl, &scpc_info.scs.ind_len_val_hdl, 2);
					wr_char.req_type	= RBLE_GATT_WRITE_CHAR;
					wr_char.val_len		= 1;
					wr_char.value[0] = write_value[0];
					break;
				default:
					result = RBLE_PARAM_ERR;
					break;
			}
			if(result == RBLE_OK)
			{
				wr_char.conhdl		 = conhdl;
				wr_char.wr_offset	 = 0;
				wr_char.auto_execute = FALSE;
				
				result = RBLE_GATT_Write_Char_Request(&wr_char);
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

static void gatt_cmp_evt_handler( RBLE_GATT_EVENT *event )
{
	if( (event->param.complete.att_code == RBLE_ATT_ERR_ATTRIBUTE_NOT_FOUND) ||
		(event->param.complete.att_code == RBLE_ATT_ERR_NO_ERROR))
	{
		switch(scpc_info.last_uuid_req)
		{
			case RBLE_ATT_SVC_SMPL_CSTM:
				scpc_disc_char_all_send(scpc_info.scs.shdl, scpc_info.scs.ehdl);
				break;
			case RBLE_DECL_CHARACTERISTIC:
				scpc_disc_char_desc_send(scpc_info.scs.ntf_char_hdl);
				scpc_info.cfg_dsc_idx = SCPC_DSC_NOTIFY_CFG;
				break;
			case RBLE_DESC_CLIENT_CHAR_CONF:
				if (scpc_info.cfg_dsc_idx == SCPC_DSC_NOTIFY_CFG)
				{
					scpc_disc_char_desc_send(scpc_info.scs.ind_char_hdl);
					scpc_info.cfg_dsc_idx = SCPC_DSC_IND_CFG;
				}
				else
				{
					RBLE_SCPC_EVENT	scp_event;
					
					scpc_info.cfg_dsc_idx = SCPC_DSC_DISCOVERY_DONE;
					scpc_info.status = SCPC_STATUS_CONNECTED;
					
					/* enable complete callback */
					scp_event.type = RBLE_SCP_EVENT_CLIENT_ENABLE_COMP;
					scp_event.param.client_enable.conhdl = scpc_info.conhdl;
					scp_event.param.client_enable.status = RBLE_OK;
					scp_event.param.client_enable.scs    = scpc_info.scs;
					
					scpc_info.callback(&scp_event);
				}
				break;
			default:
				break;
		}
	}
}

static void gatt_disc_svc_by_uuid_evt_handler( RBLE_GATT_EVENT *event )
{
	if(event->param.disc_svc_by_uuid_cmp.att_code == RBLE_OK)
	{
		scpc_info.scs.shdl = event->param.disc_svc_by_uuid_cmp.list[0].start_hdl;
		scpc_info.scs.ehdl = event->param.disc_svc_by_uuid_cmp.list[0].end_hdl;
	}
}

static void gatt_disc_char_all_evt_handler( RBLE_GATT_EVENT *event)
{
	uint8_t char_idx;
	
	if(event->param.disc_char_all_128_cmp.att_code == RBLE_OK)
	{
		char_idx = scpc_get_char_index(&event->param.disc_char_all_128_cmp.list.uuid[0]);
		
		switch(char_idx)
		{
			case SCPC_CHAR_NOTIFY:
				scpc_info.scs.ntf_char_hdl		= event->param.disc_char_all_128_cmp.list.attr_hdl;
				scpc_info.scs.ntf_prop			= event->param.disc_char_all_128_cmp.list.prop;
				scpc_info.scs.ntf_val_hdl		= event->param.disc_char_all_128_cmp.list.pointer_hdl;
				break;
			case SCPC_CHAR_INDICATE:
				scpc_info.scs.ind_char_hdl		= event->param.disc_char_all_128_cmp.list.attr_hdl;
				scpc_info.scs.ind_prop			= event->param.disc_char_all_128_cmp.list.prop;
				scpc_info.scs.ind_val_hdl		= event->param.disc_char_all_128_cmp.list.pointer_hdl;
				break;
			case SCPC_CHAR_INTERVAL:
				scpc_info.scs.interval_char_hdl	= event->param.disc_char_all_128_cmp.list.attr_hdl;
				scpc_info.scs.interval_prop		= event->param.disc_char_all_128_cmp.list.prop;
				scpc_info.scs.interval_val_hdl	= event->param.disc_char_all_128_cmp.list.pointer_hdl;
				break;
			case SCPC_CHAR_NOTIFY_LEN:
				scpc_info.scs.ntf_len_char_hdl	= event->param.disc_char_all_128_cmp.list.attr_hdl;
				scpc_info.scs.ntf_len_prop		= event->param.disc_char_all_128_cmp.list.prop;
				scpc_info.scs.ntf_len_val_hdl	= event->param.disc_char_all_128_cmp.list.pointer_hdl;
				break;
			case SCPC_CHAR_INDICATE_LEN:
				scpc_info.scs.ind_len_char_hdl	= event->param.disc_char_all_128_cmp.list.attr_hdl;
				scpc_info.scs.ind_len_prop		= event->param.disc_char_all_128_cmp.list.prop;
				scpc_info.scs.ind_len_val_hdl	= event->param.disc_char_all_128_cmp.list.pointer_hdl;
				break;
			default:
				break;
		}
	}
}

static void gatt_disc_char_desc_evt_handler( RBLE_GATT_EVENT *event )
{
	uint8_t i;
	
	for(i = 0; i < event->param.disc_char_desc_cmp.nb_entry; i++)
	{
		if(event->param.disc_char_desc_cmp.list[i].desc_hdl == RBLE_DESC_CLIENT_CHAR_CONF)
		{
			switch(scpc_info.cfg_dsc_idx)
			{
				case SCPC_DSC_NOTIFY_CFG:
					scpc_info.scs.ntf_cfg_hdl = event->param.disc_char_desc_cmp.list[i].attr_hdl;
					break;
				case SCPC_DSC_IND_CFG:
					scpc_info.scs.ind_cfg_hdl = event->param.disc_char_desc_cmp.list[i].attr_hdl;
					break;
				default:
					break;
			}
		}
	}
}

static void gatt_callback( RBLE_GATT_EVENT *event )
{
	RBLE_SCPC_EVENT		 scp_event;
	
	switch(event->type)
	{
		case RBLE_GATT_EVENT_COMPLETE:
			gatt_cmp_evt_handler(event);
			break;
		case RBLE_GATT_EVENT_DISC_SVC_BY_UUID_CMP:
			gatt_disc_svc_by_uuid_evt_handler(event);
			break;
		case RBLE_GATT_EVENT_DISC_CHAR_ALL_128_CMP:
			gatt_disc_char_all_evt_handler(event);
			break;
		case RBLE_GATT_EVENT_DISC_CHAR_DESC_CMP:
			gatt_disc_char_desc_evt_handler(event);
			break;
		case RBLE_GATT_EVENT_READ_CHAR_RESP:
			scp_event.type = RBLE_SCP_EVENT_CLIENT_READ_CHAR_RESPONSE;
			scp_event.param.rd_char_resp.conhdl   = scpc_info.conhdl;
			scp_event.param.rd_char_resp.att_code = event->param.read_char_resp.att_code;
			scp_event.param.rd_char_resp.data.len = event->param.read_char_resp.data.len;
			scp_event.param.rd_char_resp.data.each_len = event->param.read_char_resp.data.each_len;
			memcpy(&scp_event.param.rd_char_resp.data.data[0], &event->param.read_char_resp.data.data[0], event->param.read_char_resp.data.len);
			
			scpc_info.callback(&scp_event);
			break;
		case RBLE_GATT_EVENT_WRITE_CHAR_RESP:
			scp_event.type = RBLE_SCP_EVENT_CLIENT_WRITE_CHAR_RESPONSE;
			scp_event.param.wr_char_resp.conhdl   = scpc_info.conhdl;
			scp_event.param.wr_char_resp.att_code = event->param.write_char_resp.att_code;
			
			scpc_info.callback(&scp_event);
			break;
		case RBLE_GATT_EVENT_HANDLE_VALUE_NOTIF:
			scp_event.type = RBLE_SCP_EVENT_CLIENT_NOTIFY;
			scp_event.param.notify.conhdl	= scpc_info.conhdl;
			scp_event.param.notify.data_len	= event->param.handle_value_notif.size;
			memcpy(&scp_event.param.notify.data[0], &event->param.handle_value_notif.value[0], event->param.handle_value_notif.size);
			
			scpc_info.callback(&scp_event);
			break;
		case RBLE_GATT_EVENT_HANDLE_VALUE_IND:
			scp_event.type = RBLE_SCP_EVENT_CLIENT_INDICATE;
			scp_event.param.ind.conhdl	 = scpc_info.conhdl;
			scp_event.param.ind.data_len = event->param.handle_value_ind.size;
			memcpy(&scp_event.param.ind.data[0], &event->param.handle_value_ind.value[0], event->param.handle_value_ind.size);
			
			scpc_info.callback(&scp_event);
			break;
		case RBLE_GATT_EVENT_RESP_TIMEOUT:
		case RBLE_GATT_EVENT_COMMAND_DISALLOWED_IND:
		default:
			scp_event.type = RBLE_SCP_EVENT_CLIENT_INDICATE;
			break;
	}
}

static void scpc_disc_svc_send( void )
{
	RBLE_GATT_DISC_SVC_REQ disc_svc;
	
	disc_svc.req_type	= RBLE_GATT_DISC_BY_UUID_SVC;
	disc_svc.conhdl		= scpc_info.conhdl;
	disc_svc.start_hdl	= 0x0001u;
	disc_svc.end_hdl	= 0xFFFFu;
	disc_svc.desired_svc.value_size = 16;
	memcpy(&disc_svc.desired_svc.value[0], &scpc_service_uuid, sizeof(scpc_service_uuid));
	
	(void)RBLE_GATT_Discovery_Service_Request(&disc_svc);
	
	scpc_info.last_uuid_req = RBLE_ATT_SVC_SMPL_CSTM;
}

static void scpc_disc_char_all_send( uint16_t shdl, uint16_t ehdl )
{
	RBLE_GATT_DISC_CHAR_REQ disc_char = {0};
	
	disc_char.req_type	= RBLE_GATT_DISC_ALL_CHAR;
	disc_char.conhdl	= scpc_info.conhdl;
	disc_char.start_hdl	= shdl;
	disc_char.end_hdl	= ehdl;
	
	(void)RBLE_GATT_Discovery_Char_Request(&disc_char);
	
	scpc_info.last_uuid_req = RBLE_DECL_CHARACTERISTIC;
}


static void scpc_disc_char_desc_send( uint16_t shdl )
{
	RBLE_GATT_DISC_CHAR_DESC_REQ disc_char_desc = {0};
	
	disc_char_desc.conhdl	 = scpc_info.conhdl;
	disc_char_desc.start_hdl = shdl;
	disc_char_desc.end_hdl	 = scpc_get_next_char_hdl(shdl);
	
	(void)RBLE_GATT_Discovery_Char_Descriptor_Request(&disc_char_desc);
	
	scpc_info.last_uuid_req = RBLE_DESC_CLIENT_CHAR_CONF;
}

static uint16_t scpc_get_next_char_hdl( uint16_t shdl )
{
	uint16_t hdlarr[5];
	uint16_t ret_hdl = scpc_info.scs.ehdl;
	uint8_t  i;

	hdlarr[0]= scpc_info.scs.ntf_char_hdl;
	hdlarr[1]= scpc_info.scs.ind_char_hdl;
	hdlarr[2]= scpc_info.scs.interval_char_hdl;
	hdlarr[3]= scpc_info.scs.ntf_len_char_hdl;
	hdlarr[4]= scpc_info.scs.ind_len_char_hdl;

	//go through list to sort
	for(i=0; i<5; i++)
	{
		if( (shdl < hdlarr[i]) && (hdlarr[i] < ret_hdl) ) {
			ret_hdl = hdlarr[i];
		}
	}
	return ret_hdl;
}

static uint8_t scpc_get_char_index( uint8_t *uuid )
{
	uint8_t idx = SCPC_CHAR_OTHER;
	
	if(0 == memcmp(uuid, &scpc_notify_char_uuid[0], sizeof(scpc_notify_char_uuid)))
	{
		idx = SCPC_CHAR_NOTIFY;
	}
	else if(0 == memcmp(uuid, &scpc_indicate_char_uuid[0], sizeof(scpc_indicate_char_uuid)))
	{
		idx = SCPC_CHAR_INDICATE;
	}
	else if(0 == memcmp(uuid, &scpc_interval_char_uuid[0], sizeof(scpc_interval_char_uuid)))
	{
		idx = SCPC_CHAR_INTERVAL;
	}
	else if(0 == memcmp(uuid, &scpc_ntf_len_char_uuid[0], sizeof(scpc_ntf_len_char_uuid)))
	{
		idx = SCPC_CHAR_NOTIFY_LEN;
	}
	else if(0 == memcmp(uuid, &scpc_ind_len_char_uuid[0], sizeof(scpc_ind_len_char_uuid)))
	{
		idx = SCPC_CHAR_INDICATE_LEN;
	}
	else
	{
		idx = SCPC_CHAR_OTHER;
	}
	
	return idx;
}

#endif /* #ifdef USE_SAMPLE_PROFILE */
