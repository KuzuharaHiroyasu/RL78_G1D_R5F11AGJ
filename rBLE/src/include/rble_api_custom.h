/**
 ****************************************************************************************
 *
 * @file		rble_api_custom.h
 *
 * @brief	rBLE I/F Custom API Header file
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 2461 $
 *
 ****************************************************************************************
 */

#ifndef	__RBLE_API_CUSTOM_INC__
#define	__RBLE_API_CUSTOM_INC__

#include "rble_api.h"

/*================================== Sample Custom Profile ===========================*/


/*
 * Services
 *
 * Services are collections of characteristics and relationships to other services 
 * that encapsulate the behavior of part of a device.
 */
#define RBLE_SVC_SAMPLE_CUSTOM	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x01}
/*
 * Characteristics
 *
 * Characteristics are defined attribute types that contain a single logical value.
 */
#define RBLE_CHAR_SCP_NOTIFY	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x02}
#define RBLE_CHAR_SCP_INDICATE	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x03}
#define RBLE_CHAR_SCP_INTERVAL	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x04}
#define RBLE_CHAR_SCP_NTF_LEN	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x05}
#define RBLE_CHAR_SCP_IND_LEN	{0x80,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x06}


/**
 * @brief	connection type
 */
enum RBLE_SCP_CON_enum {
    RBLE_SCP_CON_CFG		= 0x00,
    RBLE_SCP_CON_NORMAL
};

/**
 * @brief	Possible values for setting client configuration characteristics
 */
enum RBLE_SCP_CLIENT_CONFIG_enum {
    RBLE_SCP_STOP_NTFIND	= 0x00,
    RBLE_SCP_START_NTF,
    RBLE_SCP_START_IND,
};

/**
 * @brief	Internal codes for reading a SCS characteristic with one single request
 */
enum RBLE_SCP_CHAR_CODE_enum {
    RBLE_SCP_SCS_NTF_CFG		= 0x00,			/**< Sample Custom Notification Client Cfg. Desc */
    RBLE_SCP_SCS_IND_CFG,						/**< Sample Custom Indication Client Cfg. Desc */
    RBLE_SCP_SCS_INTERVAL,						/**< Sample Custom Interval Char */
    RBLE_SCP_SCS_NTF_LEN,						/**< Sample Custom Notification Length Char */
    RBLE_SCP_SCS_IND_LEN						/**< Sample Custom Indication Length Char */
};

#define RBLE_SCPC_WRITE_CHAR_MAX	0x2

/* ------------------ Sample Custom Profile ------------------*/

/* Sample Custom Profile Server Enable Parameter */
typedef struct RBLE_SCP_SERVER_PARAM_t
{
    uint16_t data_ntf_en;     /* Sample Custom Notification configuration */
    uint16_t data_ind_en;     /* Sample Custom Indication configuration */
}RBLE_SCP_SERVER_PARAM;

/* Sample Custom Profile Notify Info */
typedef struct RBLE_SCP_NOTIFY_INFO_t
{
    uint8_t data_len;	/* data length */
    uint8_t data[RBLE_ATTM_MAX_VALUE];		/* data */
}RBLE_SCP_NOTIFY_INFO;

/* Sample Custom Profile Ind Info */
typedef struct RBLE_SCP_IND_INFO_t
{
    uint8_t data_len;	/* data length */
    uint8_t data[RBLE_ATTM_MAX_VALUE];		/* data */
}RBLE_SCP_IND_INFO;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_SCS_CONTENT_t
{
    uint16_t shdl;
    uint16_t ehdl;
    
    uint16_t ntf_char_hdl;
    uint16_t ntf_val_hdl;
    uint16_t ntf_cfg_hdl;
    uint8_t  ntf_prop;
    uint8_t  reserved;
    
    uint16_t ind_char_hdl;
    uint16_t ind_val_hdl;
    uint16_t ind_cfg_hdl;
    uint8_t  ind_prop;
    uint8_t  reserved2;
    
    uint16_t interval_char_hdl;
    uint16_t interval_val_hdl;
    uint8_t  interval_prop;
    uint8_t  reserved3;
    
    uint16_t ntf_len_char_hdl;
    uint16_t ntf_len_val_hdl;
    uint8_t  ntf_len_prop;
    uint8_t  reserved4;
    
    uint16_t ind_len_char_hdl;
    uint16_t ind_len_val_hdl;
    uint8_t  ind_len_prop;
    uint8_t  reserved5;
}RBLE_SCS_CONTENT;

// Samlpe Custom Profile Call back define
enum RBLE_SCP_EVENT_TYPE_enum {
	RBLE_SCP_EVENT_SERVER_ENABLE_COMP = 0x01,		/* Server enable Complete Event */
	RBLE_SCP_EVENT_SERVER_DISABLE_COMP,				/* Server disable Complete Event */
	RBLE_SCP_EVENT_SERVER_ERROR_IND,				/* Server error Complete Event */
	RBLE_SCP_EVENT_SERVER_SEND_NOTIFY_COMP,			/* Server send Notify Complete Event */
	RBLE_SCP_EVENT_SERVER_SEND_IND_COMP,			/* Server send Indicate Complete Event */
	RBLE_SCP_EVENT_SERVER_CHG_INDNTF_IND,			/* Server change indicate/Notification Cfg Event */
	RBLE_SCP_EVENT_SERVER_CHG_CHAR_IND,				/* Server change characteristic Event */
	RBLE_SCP_EVENT_SERVER_COMMAND_DISALLOWED_IND,	/* Server Command disallowed Complete Event */
	RBLE_SCP_EVENT_CLIENT_ENABLE_COMP = 0x81,		/* Client enable Complete Event */
	RBLE_SCP_EVENT_CLIENT_DISABLE_COMP,				/* Client disable Complete Event */
	RBLE_SCP_EVENT_CLIENT_ERROR_IND,				/* Client error Complete Event */
	RBLE_SCP_EVENT_CLIENT_NOTIFY,					/* Client notify Complete Event */
	RBLE_SCP_EVENT_CLIENT_INDICATE,					/* Client Indicate Complete Event */
	RBLE_SCP_EVENT_CLIENT_READ_CHAR_RESPONSE,		/* Client read Characteristic Complete Event */
	RBLE_SCP_EVENT_CLIENT_WRITE_CHAR_RESPONSE,		/* Client write Characteristic Complete Event */
	RBLE_SCP_EVENT_CLIENT_COMMAND_DISALLOWED_IND	/* Client Command Disallowed Complete Event */
};

typedef uint8_t		RBLE_SCP_EVENT_TYPE;

/*** Server ***/
typedef struct RBLE_SCPS_EVENT_t {
	RBLE_SCP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Scps_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_SCP_Server_Enable_Comp */
		struct RBLE_SCP_Server_Enable_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
	        uint8_t			    reserved;
		}server_enable;

		/* RBLE_EVT_SCP_Server_Disable_Comp */
		struct RBLE_SCP_Server_Disable_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
	        uint8_t			    reserved;
			RBLE_SCP_SERVER_PARAM server_info;
		}server_disable;

		/* RBLE_EVT_SCP_Serverr_Error_Ind */
		struct RBLE_SCP_Server_Error_Ind_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
	        uint8_t			    reserved;
		}error_ind;

		/* RBLE_EVT_SCP_SERVER_SEND_NOTIFY_COMP */
		struct RBLE_SCP_Server_Send_Notify_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
	        uint8_t			    reserved;
		}send_notify;

		/* RBLE_EVT_SCP_SERVER_SEND_IND_COMP */
		struct RBLE_SCP_Server_Send_Indicate_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
	        uint8_t			    reserved;
		}send_ind;

		/* RBLE_EVT_SCP_SERVER_Cfg_Indntf_Ind */
		struct RBLE_SCP_Server_Cfg_Indntf_Ind_t{
		    uint16_t 			conhdl;
		    uint8_t 			char_code;
			uint8_t				reserved;
		    uint16_t 			cfg_val;
		}cfg_indntf;

		/* RBLE_EVT_SCP_SERVER_Cfg_Indntf_Ind */
		struct RBLE_SCP_Server_Write_Chara_Ind_t{
		    uint16_t 			conhdl;
		    uint8_t 			char_code;
			uint8_t				reserved;
		    uint8_t 			value[RBLE_SCPC_WRITE_CHAR_MAX];
		}write_char;

		/* RBLE_EVT_SCP_CMD_DISALLOWED_IND */
		struct RBLE_SCP_Server_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
	        uint8_t		   reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_SCPS_EVENT;

/*** Client ***/
typedef struct RBLE_SCPC_EVENT_t {
	RBLE_SCP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Scpc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_SCP_Client_Enable_Comp */
		struct RBLE_SCP_Client_Enable_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
            uint8_t             reserved;
			RBLE_SCS_CONTENT    scs;
		}client_enable;

		/* RBLE_EVT_SCP_Client_Disable_Comp */
		struct RBLE_SCP_Client_Disable_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
            uint8_t             reserved;
		}client_disable;

		/* RBLE_EVT_SCP_Client_Error_Ind */
		struct RBLE_SCP_Client_Error_Ind_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
            uint8_t             reserved;
		}error_ind;

		/* RBLE_EVT_SCP_Client_Notify_Ind */
		struct RBLE_SCP_Client_Notify_Ind_t{
			uint16_t            conhdl;
			uint8_t             data_len;
			uint8_t             data[RBLE_ATTM_MAX_VALUE];
		}notify;

		/* RBLE_EVT_SCP_Client_Indicate_Ind */
		struct RBLE_SCP_Client_Indicate_Ind_t{
			uint16_t            conhdl;
			uint8_t             data_len;
			uint8_t             data[RBLE_ATTM_MAX_VALUE];
		}ind;
		
		/* RBLE_EVT_SCP_Client_Read_Char_Response */
		struct RBLE_SCP_Client_Read_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
			RBLE_ATT_INFO_DATA data;
		}rd_char_resp;
		
		/* RBLE_EVT_SCP_Client_Write_Char_Response */
		struct RBLE_SCP_Client_Write_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
		}wr_char_resp;

		/* RBLE_EVT_SCP_CMD_DISALLOWED_IND */
		struct RBLE_SCP_Client_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_SCPC_EVENT;

typedef void ( *RBLE_SCPS_EVENT_HANDLER )( RBLE_SCPS_EVENT *event );			/* SC(Server) Event CALL BACK Declaration */
typedef void ( *RBLE_SCPC_EVENT_HANDLER )( RBLE_SCPC_EVENT *event );			/* SC(Client) Event CALL BACK Declaration */

/* ------------------------- Sample Custom Profile-------------------------------*/
RBLE_STATUS RBLE_SCP_Server_Enable( uint16_t conhdl, uint8_t con_type, RBLE_SCP_SERVER_PARAM *param, RBLE_SCPS_EVENT_HANDLER call_back );
RBLE_STATUS RBLE_SCP_Server_Disable( uint16_t conhdl );
RBLE_STATUS RBLE_SCP_Server_Send_Notify( uint16_t conhdl, RBLE_SCP_NOTIFY_INFO *notify_info );
RBLE_STATUS RBLE_SCP_Server_Send_Indicate( uint16_t conhdl, RBLE_SCP_IND_INFO *ind_info );
RBLE_STATUS RBLE_SCP_Client_Enable( uint16_t conhdl, uint8_t con_type, RBLE_SCS_CONTENT *scs, RBLE_SCPC_EVENT_HANDLER call_back );
RBLE_STATUS RBLE_SCP_Client_Disable( uint16_t conhdl );
RBLE_STATUS RBLE_SCP_Client_Read_Char( uint16_t conhdl, uint8_t char_code );
RBLE_STATUS RBLE_SCP_Client_Write_Char( uint16_t conhdl, uint8_t char_code, uint8_t *write_value );

#endif // __RBLE_API_CUSTOM_INC__
