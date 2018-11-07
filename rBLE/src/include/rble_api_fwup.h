/**
 ****************************************************************************************
 *
 * @file		rble_api_fwup.h
 *
 * @brief	rBLE I/F Fwup API Header file
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 * $Rev: 2461 $
 *
 ****************************************************************************************
 */

#ifndef	__RBLE_API_FWUP_INC__
#define	__RBLE_API_FWUP_INC__

#include "rble_api.h"

/*================================== firmware update Profile ===========================*/


/*
 * Services
 *
 * Services are collections of characteristics and relationships to other services 
 * that encapsulate the behavior of part of a device.
 */
#define RBLE_SVC_FW_UPDATE				{0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x01}
/*
 * Characteristics
 *
 * Characteristics are defined attribute types that contain a single logical value.
 */
#define RBLE_CHAR_FWUP_DATA_CTRL		{0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02}
#define RBLE_CHAR_FWUP_DATA				{0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x03}

/**
 * @brief	connection type
 */
enum RBLE_FWUP_CLIENT_CONFIG_enum {
	RBLE_FWUP_CON_CFG		= 0x00,
	RBLE_FWUP_CON_NORMAL
};

/**
 * @brief	Internal codes for writing a data control characteristic with one single request
 */
enum RBLE_FWUP_DATA_SEND_enum {
	RBLE_FWUP_DATA_SEND_START		= 0x00,
	RBLE_FWUP_DATA_SEND_COMP,
	RBLE_FWUP_DATA_CHECK_WRITE,
	RBLE_FWUP_DATA_SEND_FINISH,
	RBLE_FWUP_DATA_CHECK_UPDATE
};

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_FWUS_CONTENT_t{
	uint16_t shdl;
	uint16_t ehdl;

	uint16_t data_cntl_char_hdl;
	uint16_t data_cntl_val_hdl;
	uint8_t  data_cntl_prop;
	uint8_t  reserved1;

	uint16_t data_char_hdl;
	uint16_t data_val_hdl;
	uint8_t  data_prop;
	uint8_t  reserved2;
}RBLE_FWUS_CONTENT;

// FW Update Profile Call back define
enum RBLE_FWUP_EVENT_TYPE_enum {
	RBLE_FWUP_EVENT_RECEIVER_ENABLE_COMP = 0x01,	/* Receiver enable Complete Event */
	RBLE_FWUP_EVENT_RECEIVER_DISABLE_COMP,			/* Receiver disable Complete Event */
	RBLE_FWUP_EVENT_RECEIVER_CHG_DATA_CNTL_IND,		/* Receiver change data control characteristic Event */
	RBLE_FWUP_EVENT_RECEIVER_CHG_DATA_IND,			/* Receiver change data characteristic Event */
	RBLE_FWUP_EVENT_SENDER_ENABLE_COMP = 0x81,		/* Sender enable Complete Event */
	RBLE_FWUP_EVENT_SENDER_DISABLE_COMP,			/* Sender disable Complete Event */
	RBLE_FWUP_EVENT_SENDER_EVENT_IND,				/* Sender Event Indicate Complete Event */
	RBLE_FWUP_EVENT_SENDER_WRITE_CHAR_RES			/* Sender write Characteristic Complete Event */
};

typedef uint8_t		RBLE_FWUP_EVENT_TYPE;

#define RBLE_FWUP_DATA_MAX	0x14		/* 20(MTU-3) */
/*** Receiver ***/
typedef struct RBLE_FWUPR_EVENT_t {
	RBLE_FWUP_EVENT_TYPE	type;			/* Event Type */
	uint8_t					reserved;
	union Event_Fwupr_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_FWUP_Receiver_Enable_Comp */
		struct RBLE_FWUP_Receiver_Enable_t{
			uint16_t			conhdl;
			RBLE_STATUS			status;
		}receiver_enable;

		/* RBLE_EVT_FWUP_Receiver_Disable_Comp */
		struct RBLE_FWUP_Receiver_Disable_t{
			uint16_t			conhdl;
			RBLE_STATUS 		status;
		}receiver_disable;

		/* RBLE_EVT_FWUP_Receiver_Chg_Data_Cntl_Ind */
		struct RBLE_FWUP_Receiver_Chg_Data_Cntl_Ind_t{
		    uint16_t 			conhdl;
		    uint8_t 			type;
			uint8_t				block_num;
		    uint16_t 			data_size;
		}data_cntl_ind;

		/* RBLE_EVT_FWUP_Receiver_Chg_Data_Ind */
		struct RBLE_FWUP_Receiver_Chg_Data_Ind_t{
		    uint16_t 			conhdl;
		    uint8_t 			data_size;
		    uint8_t 			data[RBLE_FWUP_DATA_MAX];
		}data_ind;
	}param;
}RBLE_FWUPR_EVENT;

/*** Sender ***/
typedef struct RBLE_FWUPS_EVENT_t {
	RBLE_FWUP_EVENT_TYPE	type;			/* Event Type */
	uint8_t					reserved;
	union Event_Fwups_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_FWUP_Sender_Enable_Comp */
		struct RBLE_FWUP_Sender_Enable_t{
			uint16_t 			conhdl;
			RBLE_STATUS			status;
			uint8_t				reserved;
			RBLE_FWUS_CONTENT	fwus;
		}sender_enable;

		/* RBLE_EVT_FWUP_Sender_Disable_Comp */
		struct RBLE_FWUP_Sender_Disable_t{
			uint16_t			conhdl;
			RBLE_STATUS			status;
		}sender_disable;

		/* RBLE_EVT_FWUP_Sender_Event_Ind */
		struct RBLE_FWUP_Sender_Event_Ind_t{
			uint16_t			conhdl;
			uint8_t				event;
		}event_ind;
		
		/* RBLE_EVT_FWUP_Sender_Write_Char_Res */
		struct RBLE_FWUP_Sender_Write_Char_Res_t{
			uint16_t           conhdl;
			uint8_t            att_code;
		}wr_char_resp;
	}param;
}RBLE_FWUPS_EVENT;

typedef void ( *RBLE_FWUPR_EVENT_HANDLER )( RBLE_FWUPR_EVENT *event );			/* FWUP(Receiver) Event CALL BACK Declaration */
typedef void ( *RBLE_FWUPS_EVENT_HANDLER )( RBLE_FWUPS_EVENT *event );			/* FWUP(Sender) Event CALL BACK Declaration */

/* ------------------------- Firmware Update Profile-------------------------------*/
/* Sender */
RBLE_STATUS RBLE_FWUP_Sender_Enable(uint16_t conhdl, uint8_t  con_type, RBLE_FWUS_CONTENT *fwus, RBLE_FWUPS_EVENT_HANDLER call_back );
RBLE_STATUS RBLE_FWUP_Sender_Disable(uint16_t conhdl);
RBLE_STATUS RBLE_FWUP_Sender_Write_Data_Cntl(uint16_t conhdl, uint8_t type, uint8_t block_num, uint16_t data_size);
RBLE_STATUS RBLE_FWUP_Sender_Write_Data(uint16_t conhdl, uint8_t *data,  uint8_t data_size);

/* Receiver */
RBLE_STATUS RBLE_FWUP_Receiver_Enable( uint16_t conhdl, RBLE_FWUPR_EVENT_HANDLER call_back);
RBLE_STATUS RBLE_FWUP_Receiver_Disable( uint16_t conhdl );
RBLE_STATUS RBLE_FWUP_Receiver_Send_Data_Cntl_Res( uint16_t conhdl, RBLE_STATUS status);

#endif // __RBLE_API_FWUP_INC__
