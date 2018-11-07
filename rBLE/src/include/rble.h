/**
 ****************************************************************************************
 *
 * @file		rble.h
 *
 * @brief	rBLE Header File
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 9323 $
 *
 ****************************************************************************************
 */

#ifndef	__RBLE_INC__
#define	__RBLE_INC__

#include "types.h"

/**************************** Define Declaration **************************************/
//#define RBLE_DEBUG			/* rBLE Debug Define */

/* rBLE Version */
#define	RBLE_MAJOR_VERSION			0x01
#define	RBLE_MINOR_VERSION			0x21

/* Command Number Define */
enum RBLE_COMMAND_NO_enum {
	/* Generic Access Profile*/
	RBLE_CMD_GAP_TOP = 0x0100,									/* GAP Command top */
	RBLE_CMD_GAP_RESET,											/* Reset Command */
	RBLE_CMD_GAP_SET_NAME,										/* Set name Command */
	RBLE_CMD_GAP_OBSERVATION_ENABLE,							/* Observation enable Command */
	RBLE_CMD_GAP_OBSERVATION_DISABLE,							/* Observation disable Command */
	RBLE_CMD_GAP_BROADCAST_ENABLE,								/* Broadcast enable Command */
	RBLE_CMD_GAP_BROADCAST_DISABLE,								/* Broadcast disable Command */
	RBLE_CMD_GAP_SET_BONDING_MODE,								/* Set bonding mode Command */
	RBLE_CMD_GAP_SET_SECURITY_REQUEST,							/* Set security request Command */
	RBLE_CMD_GAP_GET_DEVICE_INFO,								/* Get device info Command */
	RBLE_CMD_GAP_GET_WHITE_LIST_SIZE,							/* Get white list size Command */
	RBLE_CMD_GAP_ADD_TO_WHITE_LIST,								/* Add to white list Command */
	RBLE_CMD_GAP_DEL_FROM_WHITE_LIST,							/* Del from white list Command */
	RBLE_CMD_GAP_GET_REMOTE_DEVICE_NAME,						/* Get remote device name Command */
	RBLE_CMD_GAP_GET_REMOTE_DEVICE_INFO,						/* Get remote device info Command */
	RBLE_CMD_GAP_DEVICE_SEARCH,									/* Device search Command */
	RBLE_CMD_GAP_SET_RANDOM_ADDRESS,							/* Set random address Command */
	RBLE_CMD_GAP_SET_PRIVACY_FEATURE,							/* Set privacy feature Command */
	RBLE_CMD_GAP_CREATE_CONNECTION,								/* Connection Command */
	RBLE_CMD_GAP_CONNECTION_CANCEL,								/* Connection cancel Command */
	RBLE_CMD_GAP_DISCONNECT,									/* Disconnect Coommand */
	RBLE_CMD_GAP_START_BONDING,									/* Bonding Start Command */
	RBLE_CMD_GAP_BONDING_INFO_IND,								/* Bonding Info Command */
	RBLE_CMD_GAP_BONDING_RESPONSE,								/* Bonding response Command */
	RBLE_CMD_GAP_CHANGE_CONNECTION_PARAM,						/* Change connection param Command */
	RBLE_CMD_GAP_CHANNEL_MAP_REQ,								/* Channel map req Command */
	RBLE_CMD_GAP_READ_RSSI,										/* Read RSSI Command */
	RBLE_CMD_GAP_AUTHORIZED_INFO_IND,							/* Authorized Info Command */

	/* Security Manager */
	RBLE_CMD_SM_TOP = 0x0200,									/* RBLE SM Command top */
	RBLE_CMD_SM_SET_KEY,										/* Device Key Setting Command */
	RBLE_CMD_SM_START_ENC,										/* Encryption Start Command */
	RBLE_CMD_SM_TK_REQ_RESP,									/* TK Reques Command */
	RBLE_CMD_SM_LTK_REQ_RESP,									/* LTK Request Command */
	RBLE_CMD_SM_IRK_REQ_RESP,									/* IRK Request Command */
	RBLE_CMD_SM_CSRK_REQ_RESP,									/* CSRK Request Command */
	RBLE_CMD_SM_CHK_BD_ADDR_REQ_RESP,							/* BD Address Check Request Command */
	/* GATT */
	RBLE_CMD_GATT_TOP = 0x0300,									/* RBLE GATT Command top */
	RBLE_CMD_GATT_ENABLE,										/* GATT Enable Command */
	RBLE_CMD_GATT_DISCOVERY_SERVICE_REQUEST,					/* GATT Service discovery request Command */
	RBLE_CMD_GATT_DISCOVERY_CHAR_REQUEST,						/* GATT Characteristic discovery request Command */
	RBLE_CMD_GATT_DISCOVERY_CHAR_DESCRIPTOR_REQUEST,			/* GATT Characteristic descriptor discovery request Command */
	RBLE_CMD_GATT_READ_CHAR_REQUEST,							/* GATT Read characteristic request Command */
	RBLE_CMD_GATT_WRITE_CHAR_REQUEST,							/* GATT Write characteristic request Command */
	RBLE_CMD_GATT_WRITE_RELIABLE_REQUEST,						/* GATT Write reliable characteristic request Command */
	RBLE_CMD_GATT_EXECUTE_WRITE_CHAR_REQUEST,					/* GATT Execute write characteristic request Command */
	RBLE_CMD_GATT_NOTIFY_REQUEST,								/* GATT Notification request Command */
	RBLE_CMD_GATT_INDICATE_REQUEST,								/* GATT Indication request Command */
	RBLE_CMD_GATT_WRITE_RESP,									/* GATT Write Response Command */
	RBLE_CMD_GATT_SET_PERM,										/* GATT Set Permission Command */
	RBLE_CMD_GATT_SET_DATA,										/* GATT Set Data Command */


	/* Find Me Profile */
	RBLE_CMD_FMP_TARGET_TOP  = 0x1000,							/* Find Me Target Command top */
	RBLE_CMD_FMP_TARGET_ENABLE,									/* Target enable Command */
	RBLE_CMD_FMP_TARGET_DISABLE,								/* Target disable Command */

	RBLE_CMD_FMP_LOCATOR_TOP = 0x1080,							/* Find Me Locator Command top */
	RBLE_CMD_FMP_LOCATOR_ENABLE,								/* Locator enable Command */
	RBLE_CMD_FMP_LOCATOR_DISABLE,								/* Locator disable Command */
	RBLE_CMD_FMP_LOCATOR_SET_ALERT,								/* Locator set alert Command */
	/* Proximity Profile */
	RBLE_CMD_PXP_REPORTER_TOP  = 0x1100,						/* Proximity Reporter Command top */
	RBLE_CMD_PXP_REPORTER_ENABLE,								/* Reporter enable Command */
	RBLE_CMD_PXP_REPORTER_DISABLE,								/* Reporter disable Command */

	RBLE_CMD_PXP_MONITOR_TOP = 0x1180,							/* Proximity Monitor Command top */
	RBLE_CMD_PXP_MONITOR_ENABLE,								/* Monitor enable Command */
	RBLE_CMD_PXP_MONITOR_DISABLE,								/* Monitor disable Command */
	RBLE_CMD_PXP_MONITOR_GET_ALERT_LEVEL,						/* Monitor get alert level Command */
	RBLE_CMD_PXP_MONITOR_SET_ALERT_LEVEL,						/* Monitor set alert level Command */
	RBLE_CMD_PXP_MONITOR_GET_TX_POWER,							/* Monitor get tx power Command */
	/* Health Thermometer */
	RBLE_CMD_HTP_THERMOMETER_TOP = 0x1200,						/* Health Thermometer Thermometer Command top */
	RBLE_CMD_HTP_THERMOMETER_ENABLE,							/* Thermometer enable Command */
	RBLE_CMD_HTP_THERMOMETER_DISABLE,							/* Thermometer disable Command */
	RBLE_CMD_HTP_THERMOMETER_SEND_TEMP,							/* Thermometer send temp Command */
	RBLE_CMD_HTP_THERMOMETER_REQ_MEASUREMENT_PERIOD_IND,		/* Thermometer req measurement period Command */

	RBLE_CMD_HTP_COLLECTOR 		 = 0x1280,						/* Health Thermometer Collector Command top */
	RBLE_CMD_HTP_COLLECTOR_ENABLE,								/* Collector enable Command */
	RBLE_CMD_HTP_COLLECTOR_DISABLE,								/* Collector disable Command */
	RBLE_CMD_HTP_COLLECTOR_READ_CHAR,							/* Collector read Characteristic Command */
	RBLE_CMD_HTP_COLLECTOR_WRITE_CHAR,							/* Collector write Characteristic Command */
	RBLE_CMD_HTP_COLLECTOR_SET_MEASUREMENT_PERIOD,				/* Collector set measurement period Command */
	/* Blood Pressure */
	RBLE_CMD_BLP_SENSOR_TOP = 0x1300,							/* Blood Pressure Sensor Command top */
	RBLE_CMD_BLP_SENSOR_ENABLE,									/* Sensor enable Command */
	RBLE_CMD_BLP_SENSOR_DISABLE,								/* Sensor disable Command */
	RBLE_CMD_BLP_SENSOR_SEND_MEASUREMENTS,						/* Sensor send measurements Command */

	RBLE_CMD_BLP_COLLECTOR 		 = 0x1380,						/* Blood Pressure Collector Command top */
	RBLE_CMD_BLP_COLLECTOR_ENABLE,								/* Collector enable Command */
	RBLE_CMD_BLP_COLLECTOR_DISABLE,								/* Collector disable Command */
	RBLE_CMD_BLP_COLLECTOR_READ_CHAR,							/* Collector read Characteristic Command */
	RBLE_CMD_BLP_COLLECTOR_WRITE_CHAR,							/* Collector write Characteristic Command */
	/* HID over GATT */
	RBLE_CMD_HGP_HDEVICE_TOP = 0x1400,						    /* HID over GATT HID Device Command top */
	RBLE_CMD_HGP_HDEVICE_ENABLE,								/* HID Device enable Command */
	RBLE_CMD_HGP_HDEVICE_DISABLE,							    /* HID Device disable Command */
	RBLE_CMD_HGP_HDEVICE_SEND_REPORT,					        /* HID Device send report value Command */
	RBLE_CMD_HGP_HDEVICE_SEND_BATTERY_LEVEL,					/* HID Device send battery level Command */

	RBLE_CMD_HGP_BHOST_TOP = 0x1480,						    /* HID over GATT Boot Host Command top */
	RBLE_CMD_HGP_BHOST_ENABLE,								    /* Boot Host enable Command */
	RBLE_CMD_HGP_BHOST_DISABLE,								    /* Boot Host disable Command */
	RBLE_CMD_HGP_BHOST_READ_CHAR,						        /* Boot Host read Characteristic Command */
	RBLE_CMD_HGP_BHOST_READ_CHAR_BY_UUID,				        /* Boot Host read Characteristic by UUID Command */
	RBLE_CMD_HGP_BHOST_WRITE_CHAR,					            /* Boot Host write Characteristic Command */
	RBLE_CMD_HGP_BHOST_SET_REPORT,					            /* Boot Host set report value Command */
	RBLE_CMD_HGP_BHOST_SET_PROTOCOL_MODE,			            /* Boot Host set protocol mode Command */
	RBLE_CMD_HGP_BHOST_DATA_OUTPUT,			                    /* Boot Host data output Command */

	RBLE_CMD_HGP_RHOST_TOP = 0x14C0,						    /* HID over GATT Report Host Command top */
	RBLE_CMD_HGP_RHOST_ENABLE,							        /* Report Host enable Command */
	RBLE_CMD_HGP_RHOST_DISABLE,							        /* Report Host disable Command */
	RBLE_CMD_HGP_RHOST_READ_CHAR,						        /* Report Host read Characteristic Command */
	RBLE_CMD_HGP_RHOST_READ_CHAR_BY_UUID,				        /* Report Host read Characteristic by UUID Command */
	RBLE_CMD_HGP_RHOST_READ_LONG_CHAR,				            /* Report Host read Long Characteristic Command */
	RBLE_CMD_HGP_RHOST_WRITE_CHAR,				                /* Report Host write Characteristic Command */
	RBLE_CMD_HGP_RHOST_SET_REPORT,				                /* Report Host set report value Command */
	RBLE_CMD_HGP_RHOST_SET_PROTOCOL_MODE,			            /* Report Host set protocol mode Command */
	RBLE_CMD_HGP_RHOST_DATA_OUTPUT,		                        /* Report Host data output Command */
	RBLE_CMD_HGP_RHOST_SET_CONTROL_POINT,	                    /* Report Host set control point Command */
	/* Scan Parameters */
	RBLE_CMD_SPP_SERVER_TOP = 0x1500,						    /* Scan Parameters Server Command top */
	RBLE_CMD_SPP_SERVER_ENABLE,								    /* Server enable Command */
	RBLE_CMD_SPP_SERVER_DISABLE,							    /* Server disable Command */
	RBLE_CMD_SPP_SERVER_SEND_REFRESH,					        /* Server send report Command */

	RBLE_CMD_SPP_CLIENT_TOP = 0x1580,						    /* Scan Parameters Client Command top */
	RBLE_CMD_SPP_CLIENT_ENABLE,								    /* Client enable Command */
	RBLE_CMD_SPP_CLIENT_DISABLE,								/* Client disable Command */
	RBLE_CMD_SPP_CLIENT_WRITE_CHAR,						        /* Client write Characteristic Command */
	RBLE_CMD_SPP_CLIENT_SET_INTERVAL,				            /* Client set interval Command */
	/* Heart Rate */
	RBLE_CMD_HRP_SENSOR_TOP = 0x1600,							/* Heart Rate Sensor Command top */
	RBLE_CMD_HRP_SENSOR_ENABLE,									/* Sensor enable Command */
	RBLE_CMD_HRP_SENSOR_DISABLE,								/* Sensor disable Command */
	RBLE_CMD_HRP_SENSOR_SEND_MEASUREMENTS,						/* Sensor send measurements Command */

	RBLE_CMD_HRP_COLLECTOR_TOP = 0x1680,						/* Heart Rate Collector Command top */
	RBLE_CMD_HRP_COLLECTOR_ENABLE,								/* Collector enable Command */
	RBLE_CMD_HRP_COLLECTOR_DISABLE,								/* Collector disable Command */
	RBLE_CMD_HRP_COLLECTOR_READ_CHAR,							/* Collector read Characteristic Command */
	RBLE_CMD_HRP_COLLECTOR_WRITE_CONTROL_POINT,					/* Collector write Control Point Command */
	RBLE_CMD_HRP_COLLECTOR_WRITE_CHAR,							/* Collector write Characteristic Command */
	/* Cycling Speed and Cadence */
	RBLE_CMD_CSCP_SENSOR_TOP = 0x1700,							/* Cycling Speed and Cadence Sensor Command top */
	RBLE_CMD_CSCP_SENSOR_ENABLE,								/* Sensor enable Command */
	RBLE_CMD_CSCP_SENSOR_DISABLE,								/* Sensor disable Command */
	RBLE_CMD_CSCP_SENSOR_SEND_MEASUREMENTS,						/* Sensor send measurements Command */
	RBLE_CMD_CSCP_SENSOR_SEND_SC_CONTROL_POINT,					/* Sensor send measurements Command */

	RBLE_CMD_CSCP_COLLECTOR_TOP = 0x1780,						/* Cycling Speed and Cadence Collector Command top */
	RBLE_CMD_CSCP_COLLECTOR_ENABLE,								/* Collector enable Command */
	RBLE_CMD_CSCP_COLLECTOR_DISABLE,							/* Collector disable Command */
	RBLE_CMD_CSCP_COLLECTOR_READ_CHAR,							/* Collector read Characteristic Command */
	RBLE_CMD_CSCP_COLLECTOR_WRITE_SC_CONTROL_POINT,				/* Collector write SC Control Point Command */
	RBLE_CMD_CSCP_COLLECTOR_WRITE_CHAR,							/* Collector write Characteristic Command */
	/* Glucose */
	RBLE_CMD_GLP_SENSOR_TOP = 0x1800,							/* Glucose Sensor Command top */
	RBLE_CMD_GLP_SENSOR_ENABLE,									/* Sensor enable Command */
	RBLE_CMD_GLP_SENSOR_DISABLE,								/* Sensor disable Command */
	RBLE_CMD_GLP_SENSOR_SEND_MEASUREMENTS,						/* Sensor send measurements Command */
	RBLE_CMD_GLP_SENSOR_SEND_MEASUREMENTS_CONTEXT,				/* Sensor send measurements context Command */
	RBLE_CMD_GLP_SENSOR_SEND_RA_CONTROL_POINT,					/* Sensor send ra control point Command */

	RBLE_CMD_GLP_COLLECTOR_TOP = 0x1880,						/* Glucose Collector Command top */
	RBLE_CMD_GLP_COLLECTOR_ENABLE,								/* Collector enable Command */
	RBLE_CMD_GLP_COLLECTOR_DISABLE,								/* Collector disable Command */
	RBLE_CMD_GLP_COLLECTOR_READ_CHAR,							/* Collector read Characteristic Command */
	RBLE_CMD_GLP_COLLECTOR_WRITE_RA_CONTROL_POINT,				/* Collector write RA Control Point Command */
	RBLE_CMD_GLP_COLLECTOR_WRITE_CHAR,							/* Collector write Characteristic Command */
	/* Cycling Power */
	RBLE_CMD_CPP_SENSOR_TOP = 0x1900,							/* Cycling Power Sensor Command top */
	RBLE_CMD_CPP_SENSOR_ENABLE,									/* Sensor enable Command */
	RBLE_CMD_CPP_SENSOR_DISABLE,								/* Sensor disable Command */
	RBLE_CMD_CPP_SENSOR_SEND_MEASUREMENTS,						/* Sensor send measurement Command */
	RBLE_CMD_CPP_SENSOR_BROADCAST_MEASUREMENTS,					/* Sensor broadcast measurement Command */
	RBLE_CMD_CPP_SENSOR_SEND_VECTOR,							/* Sensor send vector Command */
	RBLE_CMD_CPP_SENSOR_SEND_CONTROL_POINT,						/* Sensor send control point Command */
	RBLE_CMD_CPP_SENSOR_SEND_BATTERY_LEVEL,						/* Sensor send battery level Command */
	RBLE_CMD_CPP_SENSOR_SEND_WRITE_RESPONSE,					/* Sensor send write response Command */
	
	RBLE_CMD_CPP_COLLECTOR_TOP = 0x1980,						/* Cycling Power Collector Command top */
	RBLE_CMD_CPP_COLLECTOR_ENABLE,								/* Collector enable Command */
	RBLE_CMD_CPP_COLLECTOR_DISABLE,								/* Collector disable Command */
	RBLE_CMD_CPP_COLLECTOR_READ_CHAR,							/* Collector read Characteristic Command */
	RBLE_CMD_CPP_COLLECTOR_WRITE_CONTROL_POINT,					/* Collector write control point Command */
	RBLE_CMD_CPP_COLLECTOR_WRITE_CHAR,							/* Collector write Characteristic Command */
	
	/* Time */
	RBLE_CMD_TIP_SERVER_TOP = 0x1A00,							/* Timer Server Command top */
	RBLE_CMD_TIP_SERVER_ENABLE,									/* Server enable Command */
	RBLE_CMD_TIP_SERVER_DISABLE,								/* Server disable Command */
	RBLE_CMD_TIP_SERVER_SEND_CURRENT_TIME,						/* Server send current time Command */
	RBLE_CMD_TIP_SERVER_WRITE_DATA,								/* Server write data Command */
	
	RBLE_CMD_TIP_CLIENT_TOP = 0x1A80,							/* Timer Client Command top */
	RBLE_CMD_TIP_CLIENT_ENABLE,									/* Client enable Command */
	RBLE_CMD_TIP_CLIENT_DISABLE,								/* Client disable Command */
	RBLE_CMD_TIP_CLIENT_READ_CHAR,								/* Client read Characteristic Command */
	RBLE_CMD_TIP_CLIENT_WRITE_CHAR,								/* Client write Characteristic Command */
	RBLE_CMD_TIP_CLIENT_WRITE_TIME_UPDATE_CP,					/* Client write time update control point Command */
	
	/* Alert Notification */
	RBLE_CMD_ANP_SERVER_TOP = 0x1B00,							/* Alert Notification Server Command top */
	RBLE_CMD_ANP_SERVER_ENABLE,									/* Server enable Command */
	RBLE_CMD_ANP_SERVER_DISABLE,								/* Server disable Command */
	RBLE_CMD_ANP_SERVER_SEND_NEW_ALERT,							/* Server send new alert Command */
	RBLE_CMD_ANP_SERVER_SEND_UNREAD_ALERT,						/* Server send unread alert status Command */
	
	RBLE_CMD_ANP_CLIENT_TOP = 0x1B80,							/* Alert Notification Client Command top */
	RBLE_CMD_ANP_CLIENT_ENABLE,									/* Client enable Command */
	RBLE_CMD_ANP_CLIENT_DISABLE,								/* Client disable Command */
	RBLE_CMD_ANP_CLIENT_READ_CHAR,								/* Client read Characteristic Command */
	RBLE_CMD_ANP_CLIENT_WRITE_ALERT_NOTIFICATION_CP,			/* Client write alert notification control point Command */
	RBLE_CMD_ANP_CLIENT_WRITE_CHAR,								/* Client write Characteristic Command */
	
	/* Location and Navigation */
	RBLE_CMD_LNP_SENSOR_TOP = 0x1C00,							/* Location and Navigation Sensor Command top */
	RBLE_CMD_LNP_SENSOR_ENABLE,									/* Sensor enable Command */
	RBLE_CMD_LNP_SENSOR_DISABLE,								/* Sensor disable Command */
	RBLE_CMD_LNP_SENSOR_SEND_LOCATION_SPEED,					/* Sensor send location and speed Command */
	RBLE_CMD_LNP_SENSOR_SET_POSITION_QUALITY,					/* Sensor set position quality Command */
	RBLE_CMD_LNP_SENSOR_SEND_LN_CONTROL_POINT,					/* Sensor send LN control point Command */
	RBLE_CMD_LNP_SENSOR_SEND_NAVIGATION,						/* Sensor send navigation Command */
	RBLE_CMD_LNP_SENSOR_SEND_BATTERY_LEVEL,						/* Sensor send battery level Command */
	
	RBLE_CMD_LNP_COLLECTOR_TOP = 0x1C80,						/* Location and Navigation Collector Command top */
	RBLE_CMD_LNP_COLLECTOR_ENABLE,								/* Collector enable Command */
	RBLE_CMD_LNP_COLLECTOR_DISABLE,								/* Collector disable Command */
	RBLE_CMD_LNP_COLLECTOR_READ_CHAR,							/* Collector read Characteristic Command */
	RBLE_CMD_LNP_COLLECTOR_WRITE_LN_CONTROL_POINT,				/* Collector write LN control point Command */
	RBLE_CMD_LNP_COLLECTOR_WRITE_CHAR,							/* Collector write Characteristic Command */
	
	/* Phone Alert Status */
	RBLE_CMD_PASP_SERVER_TOP = 0x1D00,							/* Phone Alert Status Server Command top*/
	RBLE_CMD_PASP_SERVER_ENABLE,								/* Server enable Command */
	RBLE_CMD_PASP_SERVER_DISABLE,								/* Server disable Command */
	RBLE_CMD_PASP_SERVER_SEND_ALERT_STATUS,						/* Server send alert status Command */
	RBLE_CMD_PASP_SERVER_SEND_RINGER_SETTING,					/* Server send ringer setting Command */
	
	RBLE_CMD_PASP_CLIENT_TOP = 0x1D80,							/* Phone Alert Status Client Command top */
	RBLE_CMD_PASP_CLIENT_ENABLE,								/* Client enable Command */
	RBLE_CMD_PASP_CLIENT_DISABLE,								/* Client disable Command */
	RBLE_CMD_PASP_CLIENT_READ_CHAR,								/* Client read Characteristic Command */
	RBLE_CMD_PASP_CLIENT_WRITE_RINGER_CONTROL_POINT,			/* Client write RINGER control point Command */
	RBLE_CMD_PASP_CLIENT_WRITE_CHAR,							/* Client write Characteristic Command */
	
	/* Running Speed and Cadence */
	RBLE_CMD_RSCP_SENSOR_TOP = 0x1E00,							/* Running Speed and Cadence Sensor top */
	RBLE_CMD_RSCP_SENSOR_ENABLE,								/* Sensor enable Command */
	RBLE_CMD_RSCP_SENSOR_DISABLE,								/* Sensor disable Command */
	RBLE_CMD_RSCP_SENSOR_SEND_MEASUREMENTS,						/* Sensor send RSC measurement Command */
	RBLE_CMD_RSCP_SENSOR_SEND_SC_CONTROL_POINT,					/* Sensor send SC control point Command */
	
	RBLE_CMD_RSCP_COLLECTOR_TOP = 0x1E80,						/* Running Speed and Cadence Collector top */
	RBLE_CMD_RSCP_COLLECTOR_ENABLE,								/* Collector enable Command */
	RBLE_CMD_RSCP_COLLECTOR_DISABLE,							/* Collector disable Command */
	RBLE_CMD_RSCP_COLLECTOR_READ_CHAR,							/* Collector read Characteristic Command */
	RBLE_CMD_RSCP_COLLECTOR_WRITE_SC_CONTROL_POINT,				/* Collector write SC control point Command */
	RBLE_CMD_RSCP_COLLECTOR_WRITE_CHAR,							/* Collector write Characteristic Command */
	
	/* Vendor Specific */
	RBLE_CMD_VS_TOP = 0x7F00,									/* Vendor Specific Command top */
	RBLE_CMD_VS_ENABLE,											/* Vendor Specfic Command Enable */
	RBLE_CMD_VS_TEST_RX_START,									/* Vendor Specific RX Test start Command */
	RBLE_CMD_VS_TEST_TX_START,									/* Vendor Specific TX Test start Command */
	RBLE_CMD_VS_TEST_END,										/* Vendor Specific Test end (RX and TX commonness)Command */
	RBLE_CMD_VS_SET_TEST_PARAMETER,								/* Vendor Specific Set Parrameters Command for Direct Test */
	RBLE_CMD_VS_READ_TEST_RSSI,									/* Vendor Specific Read RSSI while Direct Test */
	RBLE_CMD_VS_WRITE_BD_ADDRESS,								/* Vendor Specific Write BD Adsress Command */
	RBLE_CMD_VS_SET_TX_POWER,									/* Vendor Specific Set Tx Poer Command */
	RBLE_CMD_VS_GPIO_DIR,										/* Vendor Specific GPIO Dir Command */
	RBLE_CMD_VS_GPIO_ACCESS,									/* Vendor Specific GPIO Access Command */
	RBLE_CMD_VS_FLASH_MANAGEMENT,								/* Vendor Specific Data Flash Management Command */
	RBLE_CMD_VS_FLASH_ACCESS,									/* Vendor Specific Data Flash Access Command */
	RBLE_CMD_VS_FLASH_OPERATION,								/* Vendor Specific Data Flash Operation Command */
	RBLE_CMD_VS_FLASH_GET_SPACE,								/* Vendor Specific Data Flash Get Space Command */
	RBLE_CMD_VS_FLASH_GET_EEL_VER,								/* Vendor Specific Data Flash Get EEL Version Command */
	RBLE_CMD_VS_ADAPT_ENABLE,									/* Vendor Specific Adaptable Function Enable Command */
	RBLE_CMD_VS_SET_PARAMS,										/* Vendor Specific Set Parameter Command */
	RBLE_CMD_VS_RF_CONTROL,										/* Vendor Specific RF Control Command */
};

/* Type define of the command number */
typedef uint16_t	RBLE_COMMAND_NO;


#define RBLE_EVT_CMN_DISALLOWED		0x3F

/* Event number define */
enum RBLE_EVENT_NO_enum {
	/* Generic Access Profile*/
	RBLE_EVT_GAP_TOP = 0x0100,									/* RBLE GAP EVENT top */
	RBLE_EVT_GAP_RESET_RESULT,  								/* Reset result Complete Event */
	RBLE_EVT_GAP_SET_NAME_COMP,									/* Set name Complete Event */
	RBLE_EVT_GAP_OBSERVATION_ENABLE_COMP,						/* Observation enable Complete Event */
	RBLE_EVT_GAP_OBSERVATION_DISABLE_COMP,						/* Observation disable Complete Event */
	RBLE_EVT_GAP_BROADCAST_ENABLE_COMP,							/* Broadcast enable Complete Event */
	RBLE_EVT_GAP_BROADCAST_DISABLE_COMP,						/* Set bonding mode Complete Event */
	RBLE_EVT_GAP_SET_BONDING_MODE_COMP,							/* Set bonding mode Complete Event */
	RBLE_EVT_GAP_SET_SECURITY_REQUEST_COMP,						/* Set security request Complete Event */
	RBLE_EVT_GAP_GET_DEVICE_INFO_COMP,							/* Get device info Complete Event */
	RBLE_EVT_GAP_GET_WHITE_LIST_SIZE_COMP,						/* Get white list size Complete Event */
	RBLE_EVT_GAP_ADD_TO_WHITE_LIST_COMP,						/* Add to white list Complete Event */
	RBLE_EVT_GAP_DEL_FROM_WHITE_LIST_COMP,						/* Del from white list Complete Event */
	RBLE_EVT_GAP_GET_REMOTE_DEVICE_NAME_COMP,					/* Get remote device name Complete Event */
	RBLE_EVT_GAP_GET_REMOTE_DEVICE_INFO_COMP,					/* Get remote device info Complete Event */
	RBLE_EVT_GAP_DEVICE_SEARCH_COMP,							/* Device search Complete Event */
	RBLE_EVT_GAP_DEVICE_SEARCH_RESULT_IND,						/* Device search result Complete Event */
	RBLE_EVT_GAP_RPA_RESOLVED,     	           	            	/* RPA resolution procedure Complete Event */
	RBLE_EVT_GAP_SET_RANDOM_ADDRESS_COMP,						/* Set random address Complete Event */
	RBLE_EVT_GAP_SET_PRIVACY_FEATURE_COMP,						/* Set privacy feature Complete Event */
	RBLE_EVT_GAP_CONNECTION_COMP,								/* Connection Complete Event */
	RBLE_EVT_GAP_CONNECTION_CANCEL_COMP,						/* Connection cancel Complete Event */
	RBLE_EVT_GAP_DISCONNECT_COMP,								/* Disconnect Complete Event */
	RBLE_EVT_GAP_ADVERTISING_REPORT_IND,						/* Advertising report Complete Event */
	RBLE_EVT_GAP_BONDING_COMP,									/* Bonding Complete Event */
	RBLE_EVT_GAP_BONDING_REQ_IND,								/* Bonding req Complete Event */
	RBLE_EVT_GAP_CHANGE_CONNECTION_PARAM_REQ_IND,				/* Change connectionparam req Complete Event */
	RBLE_EVT_GAP_CHANGE_CONNECTION_PARAM_COMP,					/* Change connection param Complete Event */
	RBLE_EVT_GAP_CHANGE_CONNECTION_PARAM_RESPONSE,				/* Change connection param response Complete Event */
	RBLE_EVT_GAP_CHANNEL_MAP_REQ_CMP_EVT,						/* Channel map req Complete Event */
	RBLE_EVT_GAP_READ_RSSI_COMP,								/* Read RSSI Complete Event */
	RBLE_EVT_GAP_WR_CHAR_IND,									/* Write Characteristic Indication Event */
	RBLE_EVT_GAP_CMD_DISALLOWED_IND = RBLE_EVT_GAP_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Command disallowed Complete Event */
	/* Security Manager */
	RBLE_EVT_SM_TOP = 0x0200,									/* RBLE SM Event top */
	RBLE_EVT_SM_SET_KEY_CNF,									/* Device Key Setting Complete Event */
	RBLE_EVT_SM_SEC_START_IND,									/* Encryption Start Indicate Event */
	RBLE_EVT_SM_TK_REQ_IND,										/* TK Request Evetnt */
	RBLE_EVT_SM_LTK_REQ_IND,									/* LTK Request Evetnt */
	RBLE_EVT_SM_IRK_REQ_IND,									/* IRK Request Evetnt */
	RBLE_EVT_SM_CSRK_REQ_IND,									/* CSRK Request Evetnt */
	RBLE_EVT_SM_KEY_IND,										/* Key Indicate Event */
	RBLE_EVT_SM_CHK_BD_ADDR_REQ_IND,							/* BD Address Check Request Event */
	RBLE_EVT_SM_TIMEOUT_EVT,									/* Timeout Event */
	RBLE_EVT_SM_LTK_REQ_FOR_ENC_IND,							/* LTK Request For Encryption Event */
	RBLE_EVT_SM_CMD_DISALLOWED_IND = RBLE_EVT_SM_TOP | RBLE_EVT_CMN_DISALLOWED,		/* Command Disallowed Event */
	/* GATT */
	RBLE_EVT_GATT_TOP = 0x0300,									/* RBLE GATT Event top */
	RBLE_EVT_GATT_DISC_SVC_ALL_CMP,								/* Discovery all services complete event */
	RBLE_EVT_GATT_DISC_SVC_ALL_128_CMP,							/* Discovery all 128bit UUID services complete event */
	RBLE_EVT_GATT_DISC_SVC_BY_UUID_CMP,							/* Discovery service by UUID complete event */
	RBLE_EVT_GATT_DISC_SVC_INCL_CMP,							/* Discovery included services complete event */
	RBLE_EVT_GATT_DISC_CHAR_ALL_CMP,							/* Discovery all characteristics complete event */
	RBLE_EVT_GATT_DISC_CHAR_ALL_128_CMP,						/* Discovery all 128bit UUID characteristics complete event */
	RBLE_EVT_GATT_DISC_CHAR_BY_UUID_CMP,						/* Discovery characteristic by UUID complete event */
	RBLE_EVT_GATT_DISC_CHAR_BY_UUID_128_CMP,					/* Discovery characteristic by 128bit UUID complete event */
	RBLE_EVT_GATT_DISC_CHAR_DESC_CMP,							/* Discovery characteristic descriptor complete event */
	RBLE_EVT_GATT_DISC_CHAR_DESC_128_CMP,						/* Discovery 128bit characteristic descriptor complete event */
	RBLE_EVT_GATT_READ_CHAR_RESP,								/* Read characteristic response */
	RBLE_EVT_GATT_READ_CHAR_LONG_RESP,							/* Read characteristic long response */
	RBLE_EVT_GATT_READ_CHAR_MULT_RESP,							/* Read characteristic multiple response */
	RBLE_EVT_GATT_READ_CHAR_LONG_DESC_RESP,						/* Read characteristic long descriptor response */
	RBLE_EVT_GATT_WRITE_CHAR_RESP,								/* Write characteristic response */
	RBLE_EVT_GATT_WRITE_CHAR_RELIABLE_RESP,						/* Write characteristic reliable response */
	RBLE_EVT_GATT_CANCEL_WRITE_CHAR_RESP,						/* Cancel write characteristic response */
	RBLE_EVT_GATT_HANDLE_VALUE_NOTIF,							/* Handle Value notification */
	RBLE_EVT_GATT_HANDLE_VALUE_IND,								/* Handle Value indication */
	RBLE_EVT_GATT_HANDLE_VALUE_CFM,								/* Confirmation response */
	RBLE_EVT_GATT_DISCOVERY_CMP,								/* Discovery complete event */
	RBLE_EVT_GATT_COMPLETE,										/* Generic complete event */
	RBLE_EVT_GATT_WRITE_CMD_IND,								/* Write command indication */
	RBLE_EVT_GATT_RESP_TIMEOUT,									/* Response timeout notification */
	RBLE_EVT_GATT_SET_PERM_CMP,									/* Set Permission complete event */
	RBLE_EVT_GATT_SET_DATA_CMP,									/* Set Data complete event */
	RBLE_EVT_GATT_NOTIFY_CMP,									/* Notification complete event */
	RBLE_EVT_GATT_CMD_DISALLOWED_IND = RBLE_EVT_GATT_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Command disallowed Complete Event */
	/* Find Me Profile */
	RBLE_EVT_FMP_TARGET_TOP  = 0x1000,							/* RBLE Find Me Target Event Top */
	RBLE_EVT_FMP_TARGET_ENABLE_COMP,							/* Target enable Complete Event */
	RBLE_EVT_FMP_TARGET_DISABLE_COMP,							/* Target disable Complete Event */
	RBLE_EVT_FMP_TARGET_ALERT_IND,								/* Target Alert Complete Event */
	RBLE_EVT_FMP_TARGET_CMD_DISALLOWED_IND = RBLE_EVT_FMP_TARGET_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Target command disallowed Complete Event */

	RBLE_EVT_FMP_LOCATOR_TOP = 0x1080,							/* RBLE Find Me Locator Event top */
	RBLE_EVT_FMP_LOCATOR_ENABLE_COMP,							/* Locator enable Complete Event */
	RBLE_EVT_FMP_LOCATOR_DISABLE_COMP,							/* Locator disable Complete Event */
	RBLE_EVT_FMP_LOCATOR_ERROR_IND,								/* Locator error Complete Event */
	RBLE_EVT_FMP_LOCATOR_CMD_DISALLOWED_IND = RBLE_EVT_FMP_LOCATOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Locator command disallowed Complete Event */
	/* Proximity Profile */
	RBLE_EVT_PXP_REPORTER_TOP = 0x1100,							/* RBLE Proximity Reporter Event top */
	RBLE_EVT_PXP_REPORTER_ENABLE_COMP,							/* Reporter enable Complete Event */
	RBLE_EVT_PXP_REPORTER_DISABLE_COMP,							/* Reporter disable Complete Event */
	RBLE_EVT_PXP_REPORTER_ALERT_IND,							/* Reporter Alert Complete Event */
	RBLE_EVT_PXP_REPORTER_CMD_DISALLOWED_IND = RBLE_EVT_PXP_REPORTER_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Reporter command disallowed Complete Event */

	RBLE_EVT_PXP_MONITOR_TOP = 0x1180,							/* RBLE Proximity Monitor Event top */
	RBLE_EVT_PXP_MONITOR_ENABLE_COMP,							/* Monitor enable Complete Event */
	RBLE_EVT_PXP_MONITOR_DISABLE_COMP,							/* Monitor disable Complete Event */
	RBLE_EVT_PXP_MONITOR_ERROR_IND,								/* Monitor error Complete Event */
	RBLE_EVT_PXP_MONITOR_READ_CHAR_RESPONSE,					/* Monitor read Characteristic Complete Event */
	RBLE_EVT_PXP_MONITOR_WRITE_CHAR_RESPONSE,					/* Monitor write Characteristic Complete Event */
	RBLE_EVT_PXP_MONITOR_CMD_DISALLOWED_IND = RBLE_EVT_PXP_MONITOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Monitor command disallowed Complete Event */
	/* Health Thermometer */
	RBLE_EVT_HTP_THERMOMETER_TOP = 0x1200,						/* RBLE Health Thermometer Thermometer Event top */
	RBLE_EVT_HTP_THERMOMETER_ENABLE_COMP,						/* Thermometer enable Complete Event */
	RBLE_EVT_HTP_THERMOMETER_DISABLE_COMP,						/* Thermometer disable Complete Event */
	RBLE_EVT_HTP_THERMOMETER_ERROR_IND,							/* Thermometer error Complete Event */
	RBLE_EVT_HTP_THERMOMETER_SEND_TEMP_COMP,					/* Thermometer send temp Complete Event */
	RBLE_EVT_HTP_THERMOMETER_REQ_MEASUREMENT_PERIOD_IND_COMP,	/* Thermometer indicate measurement interval Complete Event */
	RBLE_EVT_HTP_THERMOMETER_MEAS_INTV_CHG_IND,					/* Thermometer meas intv chg Complete Event */
	RBLE_EVT_HTP_THERMOMETER_CFG_INDNTF_IND,					/* Thermometer cfg indntf Complete Event */
	RBLE_EVT_HTP_THERMOMETER_CMD_DISALLOWED_IND = RBLE_EVT_HTP_THERMOMETER_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Thermometer command disallowed Complete Event */

	RBLE_EVT_HTP_COLLECTOR_TOP = 0x1280,						/* RBLE Health Thermometer Collector Event top */
	RBLE_EVT_HTP_COLLECTOR_ENABLE_COMP,							/* Collector enable Complete Event */
	RBLE_EVT_HTP_COLLECTOR_DISABLE_COMP,						/* Collector disable Complete Event */
	RBLE_EVT_HTP_COLLECTOR_ERROR_IND,							/* Collector error Complete Event */
	RBLE_EVT_HTP_COLLECTOR_TEMP_IND,							/* Collector temp Complete Event */
	RBLE_EVT_HTP_COLLECTOR_MEAS_INTV_IND,						/* Collector meas intv Complete Event */
	RBLE_EVT_HTP_COLLECTOR_READ_CHAR_RESPONSE,					/* Collector read Characteristic Complete Event */
	RBLE_EVT_HTP_COLLECTOR_WRITE_CHAR_RESPONSE,					/* Collector write Characteristic Complete Event */
	RBLE_EVT_HTP_COLLECTOR_CMD_DISALLOWED_IND = RBLE_EVT_HTP_COLLECTOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Collector command disallowed Complete Event */
	/* Blood Pressure */
	RBLE_EVT_BLP_SENSOR_TOP = 0x1300,							/* RBLE Blood Pressure Sensor Event top */
	RBLE_EVT_BLP_SENSOR_ENABLE_COMP,							/* Sensor enable Complete Event */
	RBLE_EVT_BLP_SENSOR_DISABLE_COMP,							/* Sensor disable Complete Event */
	RBLE_EVT_BLP_SENSOR_ERROR_IND,								/* Sensor error Complete Event */
	RBLE_EVT_BLP_SENSOR_SEND_MEASUREMENTS_COMP,					/* Sensor send measurements Complete Event */
	RBLE_EVT_BLP_SENSOR_CFG_INDNTF_IND,					        /* Sensor cfg indntf Complete Event */
	RBLE_EVT_BLP_SENSOR_CMD_DISALLOWED_IND = RBLE_EVT_BLP_SENSOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Sensor Command disallowed Complete Event */

	RBLE_EVT_BLP_COLLECTOR_TOP = 0x1380,						/* RBLE Blood Pressure Collector Event top */
	RBLE_EVT_BLP_COLLECTOR_ENABLE_COMP,							/* Collector enable Complete Event */
	RBLE_EVT_BLP_COLLECTOR_DISABLE_COMP,						/* Collector disable Complete Event */
	RBLE_EVT_BLP_COLLECTOR_ERROR_IND,							/* Collector error Complete Event */
	RBLE_EVT_BLP_COLLECTOR_MEASUREMENTS_IND,					/* Collector measurements Complete Event */
	RBLE_EVT_BLP_COLLECTOR_READ_CHAR_RESPONSE,					/* Collector read Characteristic Complete Event */
	RBLE_EVT_BLP_COLLECTOR_WRITE_CHAR_RESPONSE,					/* Collector write Characteristic Complete Event */
	RBLE_EVT_BLP_COLLECTOR_CMD_DISALLOWED_IND = RBLE_EVT_BLP_COLLECTOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Collector Command Disallowed Complete Event */
	/* HID over GATT */
	RBLE_EVT_HGP_HDEVICE_TOP = 0x1400,							/* RBLE HID over GATT HID Device Event top */
	RBLE_EVT_HGP_HDEVICE_ENABLE_COMP,							/* HID Device enable Complete Event */
	RBLE_EVT_HGP_HDEVICE_DISABLE_COMP,							/* HID Device disable Complete Event */
	RBLE_EVT_HGP_HDEVICE_ERROR_IND,								/* HID Device error Complete Event */
	RBLE_EVT_HGP_HDEVICE_CFG_INDNTF_IND,						/* HID Device cfg indntf Complete Event */
	RBLE_EVT_HGP_HDEVICE_REPORT_IND,							/* HID Device set report Complete Event */
	RBLE_EVT_HGP_HDEVICE_PROTOCOL_MODE_CHG_EVT,					/* HID Device protocol mode chg Complete Event */
	RBLE_EVT_HGP_HDEVICE_REPORT_EVT,							/* HID Device data output Complete Event */
	RBLE_EVT_HGP_HDEVICE_HID_CP_CHG_EVT,						/* HID Device HID contro point chg Complete Event */
	RBLE_EVT_HGP_HDEVICE_REPORT_COMP,							/* HID Device send report value Complete Event */
	RBLE_EVT_HGP_HDEVICE_SEND_BATTERY_LEVEL_COMP,				/* HID Device send battery level Complete Event */
	RBLE_EVT_HGP_HDEVICE_CMD_DISALLOWED_IND = RBLE_EVT_HGP_HDEVICE_TOP | RBLE_EVT_CMN_DISALLOWED,	/* HID Device disallowed Complete Event */

	RBLE_EVT_HGP_BHOST_TOP = 0x1480,							/* RBLE HID over GATT Boot Host Event top */
	RBLE_EVT_HGP_BHOST_ENABLE_COMP,								/* Boot Host enable Complete Event */
	RBLE_EVT_HGP_BHOST_DISABLE_COMP,							/* Boot Host disable Complete Event */
	RBLE_EVT_HGP_BHOST_ERROR_IND,								/* Boot Host error Complete Event */
	RBLE_EVT_HGP_BHOST_READ_CHAR_RESPONSE,						/* Boot Host read Characteristic Complete Event */
	RBLE_EVT_HGP_BHOST_WRITE_CHAR_RESPONSE,						/* Boot Host write Characteristic Complete Event */
	RBLE_EVT_HGP_BHOST_REPORT_NTF,								/* Boot Host data input Complete Event */
	RBLE_EVT_HGP_BHOST_CMD_DISALLOWED_IND = RBLE_EVT_HGP_BHOST_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Boot Host Command Disallowed Complete Event */

	RBLE_EVT_HGP_RHOST_TOP = 0x14C0,							/* RBLE HID over GATT Report Host Event top */
	RBLE_EVT_HGP_RHOST_ENABLE_COMP,								/* Report Host enable Complete Event */
	RBLE_EVT_HGP_RHOST_DISABLE_COMP,							/* Report Host disable Complete Event */
	RBLE_EVT_HGP_RHOST_ERROR_IND,								/* Report Host error Complete Event */
	RBLE_EVT_HGP_RHOST_READ_CHAR_RESPONSE,						/* Report Host read Characteristic Complete Event */
	RBLE_EVT_HGP_RHOST_READ_LONG_CHAR_RESPONSE,					/* Report Host read Long Characteristic Complete Event */
	RBLE_EVT_HGP_RHOST_WRITE_CHAR_RESPONSE,						/* Report Host write Characteristic Complete Event */
	RBLE_EVT_HGP_RHOST_REPORT_NTF,								/* Report Host data input Complete Event */
	RBLE_EVT_HGP_RHOST_BATTERY_LEVEL_NTF,						/* Report Host battery level Complete Event */
	RBLE_EVT_HGP_RHOST_CMD_DISALLOWED_IND = RBLE_EVT_HGP_RHOST_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Report Host Command Disallowed Complete Event */
	/* Scan Parameters */
	RBLE_EVT_SPP_SERVER_TOP = 0x1500,							/* RBLE Scan Parameters Server Event top */
	RBLE_EVT_SPP_SERVER_ENABLE_COMP,							/* Server enable Complete Event */
	RBLE_EVT_SPP_SERVER_DISABLE_COMP,							/* Server disable Complete Event */
	RBLE_EVT_SPP_SERVER_CFG_INDNTF_IND,							/* Server cfg indntf Complete Event */
	RBLE_EVT_SPP_SERVER_SEND_REFRESH_COMP,						/* Server send refresh Complete Event */
	RBLE_EVT_SPP_SERVER_INTERVAL_CHG_EVT,						/* Server interval chg Complete Event */
	RBLE_EVT_SPP_SERVER_CMD_DISALLOWED_IND = RBLE_EVT_SPP_SERVER_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Server disallowed Complete Event */

	RBLE_EVT_SPP_CLIENT_TOP = 0x1580,							/* RBLE Scan Parameters Client Event top */
	RBLE_EVT_SPP_CLIENT_ENABLE_COMP,							/* Client enable Complete Event */
	RBLE_EVT_SPP_CLIENT_DISABLE_COMP,							/* Client disable Complete Event */
	RBLE_EVT_SPP_CLIENT_ERROR_IND,								/* Client error Complete Event */
	RBLE_EVT_SPP_CLIENT_WRITE_CHAR_RESPONSE,					/* Client write Characteristic Complete Event */
	RBLE_EVT_SPP_CLIENT_CMD_DISALLOWED_IND = RBLE_EVT_SPP_CLIENT_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Client Command Disallowed Complete Event */
	
	/* Vendor Specific */
	RBLE_EVT_VS_TOP = 0x7F00,									/* Vendor Specific Event top */
	RBLE_EVT_VS_TEST_RX_START_COMP,								/* Vendor RX Test start Complete Event */
	RBLE_EVT_VS_TEST_TX_START_COMP,								/* Vendor TX Test start Complete Event */
	RBLE_EVT_VS_TEST_END_COMP,									/* Vendor Test end Complete Event(RX and TX commonness) */
	RBLE_EVT_VS_WR_BD_ADDR_COMP,								/* Vendor Write Bd Address Command Complete Event */
	RBLE_EVT_VS_SET_TEST_PARAM_COMP,							/* Vendor Set Test Parameters Command Complete Event */
	RBLE_EVT_VS_READ_TEST_RSSI_COMP,							/* Vendor Read Test RSSI Command Complete Event */
	RBLE_EVT_VS_SET_TX_POWER_IND,								/* Vendor Command Set Tx Power Complete Event */
	RBLE_EVT_VS_GPIO_DIR_COMP,									/* Vendor GPIO Dir Command Complete Event */
	RBLE_EVT_VS_GPIO_ACCESS_COMP,								/* Vendor GPIO Access Command Complete Event */
	RBLE_EVT_VS_FLASH_MANAGEMENT_COMP,							/* Vendor Command Data Flash Management Complete Event */
	RBLE_EVT_VS_FLASH_ACCESS_COMP,								/* Vendor Command Data Flash Access Complete Event */
	RBLE_EVT_VS_FLASH_OPERATION_COMP,							/* Vendor Command Data Flash Operation Complete Event */
	RBLE_EVT_VS_FLASH_GET_SPACE_COMP,							/* Vendor Command Data Flash Get Space Complete Event */
	RBLE_EVT_VS_FLASH_GET_EEL_VER_COMP,							/* Vendor Command Data Flash Get EEL Version Complete Event */
	RBLE_EVT_VS_ADAPT_ENABLE_COMP,								/* Vendor Command Adaptable Fuction Enable Complete Event */
	RBLE_EVT_VS_ADAPT_STATE_IND,								/* Vendor Command Adaptable Fuction State Notification Event */
	RBLE_EVT_VS_SET_PARAMS_COMP,								/* Vendor Command Set Parameter Complete Event */
	RBLE_EVT_VS_RF_CONTROL_COMP,								/* Vendor Command RF Control Complete Event */
	RBLE_EVT_VS_CMD_DISALLOWED_IND = RBLE_EVT_VS_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Vendor Command Disallowed Complete Event */
};
enum RBLE_EVENT_ADD_NO_enum {
	/* Heart Rate */
	RBLE_EVT_HRP_SENSOR_TOP = 0x1600,							/* RBLE Heart Rate Sensor Event top */
	RBLE_EVT_HRP_SENSOR_ENABLE_COMP,							/* Sensor enable Complete Event */
	RBLE_EVT_HRP_SENSOR_DISABLE_COMP,							/* Sensor disable Complete Event */
	RBLE_EVT_HRP_SENSOR_ERROR_IND,								/* Sensor error Complete Event */
	RBLE_EVT_HRP_SENSOR_SEND_MEASUREMENTS_COMP,					/* Sensor send measurements Complete Event */
	RBLE_EVT_HRP_SENSOR_CHG_CP_IND,								/* Sensor Control Point change Complete Event */
	RBLE_EVT_HRP_SENSOR_CFG_NTF_IND,							/* Sensor cfg indntf Complete Event */
	RBLE_EVT_HRP_SENSOR_CMD_DISALLOWED_IND = RBLE_EVT_HRP_SENSOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Sensor Command disallowed Complete Event */

	RBLE_EVT_HRP_COLLECTOR_TOP = 0x1680,						/* RBLE Heart Rate Collector Event top */
	RBLE_EVT_HRP_COLLECTOR_ENABLE_COMP,							/* Collector enable Complete Event */
	RBLE_EVT_HRP_COLLECTOR_DISABLE_COMP,						/* Collector disable Complete Event */
	RBLE_EVT_HRP_COLLECTOR_ERROR_IND,							/* Collector error Complete Event */
	RBLE_EVT_HRP_COLLECTOR_MEASUREMENTS_NTF,					/* Collector receive measurements Complete Event */
	RBLE_EVT_HRP_COLLECTOR_READ_CHAR_RESPONSE,					/* Collector read Characteristic Complete Event */
	RBLE_EVT_HRP_COLLECTOR_WRITE_CHAR_RESPONSE,					/* Collector write Characteristic Complete Event */
	RBLE_EVT_HRP_COLLECTOR_CMD_DISALLOWED_IND = RBLE_EVT_HRP_COLLECTOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Collector Command Disallowed Complete Event */
	/* Cycling Speed and Cadence */
	RBLE_EVT_CSCP_SENSOR_TOP = 0x1700,							/* RBLE Cycling Speed and Cadence Sensor Event top */
	RBLE_EVT_CSCP_SENSOR_ENABLE_COMP,							/* Sensor enable Complete Event */
	RBLE_EVT_CSCP_SENSOR_DISABLE_COMP,							/* Sensor disable Complete Event */
	RBLE_EVT_CSCP_SENSOR_ERROR_IND,								/* Sensor error Complete Event */
	RBLE_EVT_CSCP_SENSOR_SEND_MEASUREMENTS_COMP,				/* Sensor send measurements Complete Event */
	RBLE_EVT_CSCP_SENSOR_SEND_SC_CP_COMP,						/* Sensor SC control point Complete Event */
	RBLE_EVT_CSCP_SENSOR_CHG_SC_CP_IND,							/* Sensor Control Point change Complete Event */
	RBLE_EVT_CSCP_SENSOR_CFG_INDNTF_IND,						/* Sensor cfg indntf Complete Event */
	RBLE_EVT_CSCP_SENSOR_CMD_DISALLOWED_IND = RBLE_EVT_CSCP_SENSOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Sensor Command disallowed Complete Event */

	RBLE_EVT_CSCP_COLLECTOR_TOP = 0x1780,						/* RBLE Cycling Speed and Cadence Collector Event top */
	RBLE_EVT_CSCP_COLLECTOR_ENABLE_COMP,						/* Collector enable Complete Event */
	RBLE_EVT_CSCP_COLLECTOR_DISABLE_COMP,						/* Collector disable Complete Event */
	RBLE_EVT_CSCP_COLLECTOR_ERROR_IND,							/* Collector error Complete Event */
	RBLE_EVT_CSCP_COLLECTOR_MEASUREMENTS_NTF,					/* Collector receive measurements Complete Event */
	RBLE_EVT_CSCP_COLLECTOR_SC_CP_IND,							/* Collector receive SC control point Complete Event */
	RBLE_EVT_CSCP_COLLECTOR_READ_CHAR_RESPONSE,					/* Collector read Characteristic Complete Event */
	RBLE_EVT_CSCP_COLLECTOR_WRITE_CHAR_RESPONSE,				/* Collector write Characteristic Complete Event */
	RBLE_EVT_CSCP_COLLECTOR_CMD_DISALLOWED_IND = RBLE_EVT_CSCP_COLLECTOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Collector Command Disallowed Complete Event */
	/* Glucose */
	RBLE_EVT_GLP_SENSOR_TOP = 0x1800,							/* RBLE Glucose Sensor Event top */
	RBLE_EVT_GLP_SENSOR_ENABLE_COMP,							/* Sensor enable Complete Event */
	RBLE_EVT_GLP_SENSOR_DISABLE_COMP,							/* Sensor disable Complete Event */
	RBLE_EVT_GLP_SENSOR_ERROR_IND,								/* Sensor error Complete Event */
	RBLE_EVT_GLP_SENSOR_SEND_MEASUREMENTS_COMP,					/* Sensor send measurements Complete Event */
	RBLE_EVT_GLP_SENSOR_SEND_MEASUREMENTS_CONTEXT_COMP,			/* Sensor send measurements context Complete Event */
	RBLE_EVT_GLP_SENSOR_SEND_RA_CP_COMP,						/* Sensor send control point Complete Event */
	RBLE_EVT_GLP_SENSOR_CHG_RA_CP_IND,							/* Sensor Control Point change Complete Event */
	RBLE_EVT_GLP_SENSOR_CFG_INDNTF_IND,							/* Sensor cfg indntf Complete Event */
	RBLE_EVT_GLP_SENSOR_CMD_DISALLOWED_IND = RBLE_EVT_GLP_SENSOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Sensor Command disallowed Complete Event */

	RBLE_EVT_GLP_COLLECTOR_TOP = 0x1880,						/* RBLE Glucose Collector Event top */
	RBLE_EVT_GLP_COLLECTOR_ENABLE_COMP,							/* Collector enable Complete Event */
	RBLE_EVT_GLP_COLLECTOR_DISABLE_COMP,						/* Collector disable Complete Event */
	RBLE_EVT_GLP_COLLECTOR_ERROR_IND,							/* Collector error Complete Event */
	RBLE_EVT_GLP_COLLECTOR_MEASUREMENTS_NTF,					/* Collector measurements Complete Event */
	RBLE_EVT_GLP_COLLECTOR_MEASUREMENTS_CONTEXT_NTF,			/* Collector measurements context Complete Event */
	RBLE_EVT_GLP_COLLECTOR_RA_CP_IND,							/* Collector control point Complete Event */
	RBLE_EVT_GLP_COLLECTOR_READ_CHAR_RESPONSE,					/* Collector read Characteristic Complete Event */
	RBLE_EVT_GLP_COLLECTOR_WRITE_CHAR_RESPONSE,					/* Collector write Characteristic Complete Event */
	RBLE_EVT_GLP_COLLECTOR_CMD_DISALLOWED_IND = RBLE_EVT_GLP_COLLECTOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Collector Command Disallowed Complete Event */
	/* Cycling Power */
	RBLE_EVT_CPP_SENSOR_TOP = 0x1900,							/* RBLE Cycling Power Sensor Event top */
	RBLE_EVT_CPP_SENSOR_ENABLE_COMP,							/* Sensor enable Complete Event */
	RBLE_EVT_CPP_SENSOR_DISABLE_COMP,							/* Sensor disable Complete Event */
	RBLE_EVT_CPP_SENSOR_ERROR_IND,								/* Sensor error Complete Event */
	RBLE_EVT_CPP_SENSOR_SEND_MEASUREMENTS_COMP,					/* Sensor send measurements Complete Event */
	RBLE_EVT_CPP_SENSOR_BROADCAST_MEASUREMENTS_COMP,			/* Sensor broadcast measurements Complete Event */
	RBLE_EVT_CPP_SENSOR_SEND_VECTOR_COMP,						/* Sensor send vector Complete Event */
	RBLE_EVT_CPP_SENSOR_SEND_CP_CP_COMP,						/* Sensor send control point Complete Event */
	RBLE_EVT_CPP_SENSOR_SEND_BATTERY_LEVEL_COMP,				/* Sensor send battery levelt Complete Event */
	RBLE_EVT_CPP_SENSOR_CHG_CP_CP_IND,							/* Sensor change control point Complete Event */
	RBLE_EVT_CPP_SENSOR_CFG_INDNTFBRD_IND,						/* Sensor cfg indntfbrd Complete Event */
	RBLE_EVT_CPP_SENSOR_CMD_DISALLOWED_IND = RBLE_EVT_CPP_SENSOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Sensor command disallowed Complete Event */
	
	RBLE_EVT_CPP_COLLECTOR_TOP = 0x1980,						/* RBLE Cycling Power Collector Event top */
	RBLE_EVT_CPP_COLLECTOR_ENABLE_COMP,							/* Collector enable Complete Event */
	RBLE_EVT_CPP_COLLECTOR_DISABLE_COMP,						/* Collector disable Complete Event */
	RBLE_EVT_CPP_COLLECTOR_ERROR_IND,							/* Collector error Complete Event */
	RBLE_EVT_CPP_COLLECTOR_MEASUREMENTS_NTF,					/* Collector receive measurement Complete Event */
	RBLE_EVT_CPP_COLLECTOR_VECTOR_NTF,							/* Collector receive vector Complete Event */
	RBLE_EVT_CPP_COLLECTOR_CP_CP_IND,							/* Collector receive control point Complete Event */
	RBLE_EVT_CPP_COLLECTOR_BATTERY_LEVEL_NTF,					/* Collector receive battery level Complete Event */
	RBLE_EVT_CPP_COLLECTOR_READ_CHAR_RESPONSE,					/* Collector read Characteristic Complete Event */
	RBLE_EVT_CPP_COLLECTOR_WRITE_CHAR_RESPONSE,					/* Collector write Characteristic Complete Event */
	RBLE_EVT_CPP_COLLECTOR_CMD_DISALLOWED_IND = RBLE_EVT_CPP_COLLECTOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Collector command disallowed Complete Event */
	
	/* Time */
	RBLE_EVT_TIP_SERVER_TOP = 0x1A00,							/* RBLE Timer Server Event top */
	RBLE_EVT_TIP_SERVER_ENABLE_COMP,							/* Server enable Complete Event */
	RBLE_EVT_TIP_SERVER_DISABLE_COMP,							/* Server disable Complete Event */
	RBLE_EVT_TIP_SERVER_ERROR_IND,								/* Server error Complete Event */
	RBLE_EVT_TIP_SERVER_SEND_CURRENT_TIME_COMP,					/* Server send current time Complete Event */
	RBLE_EVT_TIP_SERVER_WRITE_DATA_COMP,						/* Server write data Complete Event */
	RBLE_EVT_TIP_SERVER_CHG_TIME_UPDATE_CP_IND,					/* Server change time update control point Complete Event */
	RBLE_EVT_TIP_SERVER_CFG_NTF_IND,							/* Server cfg ntf Complete Event */
	RBLE_EVT_TIP_SERVER_CMD_DISALLOWED_IND = RBLE_EVT_TIP_SERVER_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Server command disallowed Complete Event */
	
	RBLE_EVT_TIP_CLIENT_TOP = 0x1A80,							/* RBLE Timer Client Event top */
	RBLE_EVT_TIP_CLIENT_ENABLE_COMP,							/* Client enable Complete Event */
	RBLE_EVT_TIP_CLIENT_DISABLE_COMP,							/* Client disable Complete Event */
	RBLE_EVT_TIP_CLIENT_ERROR_IND,								/* Client error Complete Event */
	RBLE_EVT_TIP_CLIENT_CURRENT_TIME_NTF,						/* Client send current time Complete Event */
	RBLE_EVT_TIP_CLIENT_READ_CHAR_RESPONSE,						/* Client read Characteristic Complete Event */
	RBLE_EVT_TIP_CLIENT_WRITE_CHAR_RESPONSE,					/* Client write Characteristic Complete Event */
	RBLE_EVT_TIP_CLIENT_CMD_DISALLOWED_IND = RBLE_EVT_TIP_CLIENT_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Client command disallowed Complete Event */

	/* Alert Notification */
	RBLE_EVT_ANP_SERVER_TOP = 0x1B00,							/* RBLE Alert Notification Server Event top */
	RBLE_EVT_ANP_SERVER_ENABLE_COMP,							/* Server enable Complete Event */
	RBLE_EVT_ANP_SERVER_DISABLE_COMP,							/* Server disable Complete Event */
	RBLE_EVT_ANP_SERVER_ERROR_IND,								/* Server error Complete Event */
	RBLE_EVT_ANP_SERVER_SEND_NEW_ALERT_COMP,					/* Server send new alert Complete Event */
	RBLE_EVT_ANP_SERVER_SEND_UNREAD_ALERT_COMP,					/* Server sent unread alert status Complete Event */
	RBLE_EVT_ANP_SERVER_CHG_ALERT_NTF_CP_IND,					/* Server change alert notification control point Complete Event */
	RBLE_EVT_ANP_SERVER_CFG_NTF_IND,							/* Server cfg ntf Complete Event */
	RBLE_EVT_ANP_SERVER_CMD_DISALLOWED_IND = RBLE_EVT_ANP_SERVER_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Server command disallowed Complete Event */
	
	RBLE_EVT_ANP_CLIENT_TOP = 0x1B80,							/* RBLE Alert Notification Client Event top */
	RBLE_EVT_ANP_CLIENT_ENABLE_COMP,							/* Client enable Complete Event */
	RBLE_EVT_ANP_CLIENT_DISABLE_COMP,							/* Client disable Complete Event */
	RBLE_EVT_ANP_CLIENT_ERROR_IND,								/* Client error Complete Event */
	RBLE_EVT_ANP_CLIENT_NEW_ALERT_NTF,							/* Client receive new alert Complete Event */
	RBLE_EVT_ANP_CLIENT_UNREAD_ALERT_NTF,						/* Client receive unread alert status Complete Event */
	RBLE_EVT_ANP_CLIENT_READ_CHAR_RESPONSE,						/* Client read Characteristic Complete Event */
	RBLE_EVT_ANP_CLIENT_WRITE_CHAR_RESPONSE,					/* Client write Characteristic Complete Event */
	RBLE_EVT_ANP_CLIENT_CMD_DISALLOWED_IND = RBLE_EVT_ANP_CLIENT_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Client command disallowed Complete Event */
	
	/* Location and Navigation */
	RBLE_EVT_LNP_SENSOR_TOP = 0x1C00,							/* RBLE Location and Navigation Sensor Event top */
	RBLE_EVT_LNP_SENSOR_ENABLE_COMP,							/* Sensor enable Complete Event */
	RBLE_EVT_LNP_SENSOR_DISABLE_COMP,							/* Sensor disable Complete Event */
	RBLE_EVT_LNP_SENSOR_ERROR_IND,								/* Sensor error Complete Event */
	RBLE_EVT_LNP_SENSOR_SEND_LOCATION_SPEED_COMP,				/* Sensor send location and speed Complete Event */
	RBLE_EVT_LNP_SENSOR_SET_POSITION_QUALITY_COMP,				/* Sensor set position quality Complete Event */
	RBLE_EVT_LNP_SENSOR_SEND_LN_CP_COMP,						/* Sensor send LN Control Point Complete Event */
	RBLE_EVT_LNP_SENSOR_SEND_NAVIGATION_COMP,					/* Sensor send navigation Complete Event */
	RBLE_EVT_LNP_SENSOR_SEND_BATTERY_LEVEL_COMP,				/* Sensor send battery level Complete Event */
	RBLE_EVT_LNP_SENSOR_CHG_LN_CP_IND,							/* Sensor change LN control point Complete */
	RBLE_EVT_LNP_SENSOR_CFG_INDNTF_IND,							/* Sensor cfg ntf/ind Complete Event */
	RBLE_EVT_LNP_SENSOR_CMD_DISALLOWED_IND = RBLE_EVT_LNP_SENSOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Sensor command disallowed Complete Event */
	
	RBLE_EVT_LNP_COLLECTOR_TOP = 0x1C80,						/* RBLE Location and Navigation Collector Event top */
	RBLE_EVT_LNP_COLLECTOR_ENABLE_COMP,							/* Collector enable Complete Event */
	RBLE_EVT_LNP_COLLECTOR_DISABLE_COMP,						/* Collector disable Complete Event */
	RBLE_EVT_LNP_COLLECTOR_ERROR_IND,							/* Collector error Complete Event */
	RBLE_EVT_LNP_COLLECTOR_LOCATION_SPEED_NTF,					/* Collector receive location and speed Complete Event */
	RBLE_EVT_LNP_COLLECTOR_LN_CP_IND,							/* Collector receive LN control point Complete Event */
	RBLE_EVT_LNP_COLLECTOR_NAVIGATION_NTF,						/* Collector receive navigation Complete Event */
	RBLE_EVT_LNP_COLLECTOR_BATTERY_LEVEL_NTF,					/* Collector receive battery level Complete Event */
	RBLE_EVT_LNP_COLLECTOR_READ_CHAR_RESPONSE,					/* Collector read Characteristic Complete Event */
	RBLE_EVT_LNP_COLLECTOR_WRITE_CHAR_RESPONSE,					/* Collector write Characteristic Complete Event */
	RBLE_EVT_LNP_COLLECTOR_CMD_DISALLOWED_IND = RBLE_EVT_LNP_COLLECTOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Collector command disallowed Complete Event */
	
	/* Phone Alert Status */
	RBLE_EVT_PASP_SERVER_TOP = 0x1D00,							/* RBLE Phone Alert Status Server Event top */
	RBLE_EVT_PASP_SERVER_ENABLE_COMP,							/* Server enable Complete Event */
	RBLE_EVT_PASP_SERVER_DISABLE_COMP,							/* Server disable Complete Event */
	RBLE_EVT_PASP_SERVER_ERROR_IND,								/* Server error Complete Event */
	RBLE_EVT_PASP_SERVER_SEND_ALERT_STATUS_COMP,				/* Server send alert status Complete Event */
	RBLE_EVT_PASP_SERVER_SEND_RINGER_SETTING_COMP,				/* Server send ringer setting Complete Event */
	RBLE_EVT_PASP_SERVER_CHG_RINGER_CP_IND,						/* Server change ringer control point Complete Event */
	RBLE_EVT_PASP_SERVER_CFG_NTF_IND,							/* Server cfg ntf Complete Event */
	RBLE_EVT_PASP_SERVER_CMD_DISALLOWED_IND = RBLE_EVT_PASP_SERVER_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Server command disallowed Complete Event */
	
	RBLE_EVT_PASP_CLIENT_TOP = 0x1D80,							/* RBLE Phone Alert Status Client Event top */
	RBLE_EVT_PASP_CLIENT_ENABLE_COMP,							/* Client enable Complete Event */
	RBLE_EVT_PASP_CLIENT_DISABLE_COMP,							/* Client disable Complete Event */
	RBLE_EVT_PASP_CLIENT_ERROR_IND,								/* Client error Complete Event */
	RBLE_EVT_PASP_CLIENT_ALERT_STATUS_NTF,						/* Client receive alert status Complete Event */
	RBLE_EVT_PASP_CLIENT_RINGER_SETTING_NTF,					/* Client receive ringer setting Complete Event */
	RBLE_EVT_PASP_CLIENT_READ_CHAR_RESPONSE,					/* Client read Characteristic Complete Event */
	RBLE_EVT_PASP_CLIENT_WRITE_CHAR_RESPONSE,					/* Client write Characteristic Complete Event */
	RBLE_EVT_PASP_CLIENT_CMD_DISALLOWED_IND = RBLE_EVT_PASP_CLIENT_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Client command disallowed Complete Event */
	/* Running Speed and Cadence */
	RBLE_EVT_RSCP_SENSOR_TOP = 0x1E00,							/* RBLE Running Speed and Cadence Sensor Event top */
	RBLE_EVT_RSCP_SENSOR_ENABLE_COMP,							/* Sensor enable Complete Event */
	RBLE_EVT_RSCP_SENSOR_DISABLE_COMP,							/* Sensor disable Complete Event */
	RBLE_EVT_RSCP_SENSOR_ERROR_IND,								/* Sensor error Complete Event */
	RBLE_EVT_RSCP_SENSOR_SEND_MEASUREMENTS_COMP,				/* Sensor send measurements Complete Event */
	RBLE_EVT_RSCP_SENSOR_SEND_SC_CP_COMP,						/* Sensor send SC Control Point Complete Event */
	RBLE_EVT_RSCP_SENSOR_CHG_SC_CP_IND,							/* Sensor Control Point change Complete Event */
	RBLE_EVT_RSCP_SENSOR_CFG_INDNTF_IND,						/* Sensor cfg indntf Complete Event */
	RBLE_EVT_RSCP_SENSOR_CMD_DISALLOWED_IND = RBLE_EVT_RSCP_SENSOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Sensor Command disallowed Complete Event */

	RBLE_EVT_RSCP_COLLECTOR_TOP = 0x1E80,						/* RBLE Running Speed and Cadence Collector Event top */
	RBLE_EVT_RSCP_COLLECTOR_ENABLE_COMP,						/* Collector enable Complete Event */
	RBLE_EVT_RSCP_COLLECTOR_DISABLE_COMP,						/* Collector disable Complete Event */
	RBLE_EVT_RSCP_COLLECTOR_ERROR_IND,							/* Collector error Complete Event */
	RBLE_EVT_RSCP_COLLECTOR_MEASUREMENTS_NTF,					/* Collector receive measurements Complete Event */
	RBLE_EVT_RSCP_COLLECTOR_SC_CP_IND,							/* Collector receive SC control point Complete Event */
	RBLE_EVT_RSCP_COLLECTOR_READ_CHAR_RESPONSE,					/* Collector read Characteristic Complete Event */
	RBLE_EVT_RSCP_COLLECTOR_WRITE_CHAR_RESPONSE,				/* Collector write Characteristic Complete Event */
	RBLE_EVT_RSCP_COLLECTOR_CMD_DISALLOWED_IND = RBLE_EVT_RSCP_COLLECTOR_TOP | RBLE_EVT_CMN_DISALLOWED,	/* Collector Command Disallowed Complete Event */
};

/* Type define of the event number */
typedef uint16_t	RBLE_EVENT_NO;

/* error code */
enum RBLE_STATUS_enum {
	RBLE_OK									= 0x00,
	RBLE_UNKNOWN_HCI_COMMAND				= 0x01,
	RBLE_UNKNOWN_CONNECTION_ID				= 0x02,
	RBLE_HARDWARE_FAILURE					= 0x03,
	RBLE_PAGE_TIMEOUT						= 0x04,
	RBLE_AUTH_FAILURE						= 0x05,
	RBLE_PIN_MISSING						= 0x06,
	RBLE_MEMORY_CAPA_EXCEED					= 0x07,
	RBLE_CON_TIMEOUT						= 0x08,
	RBLE_CON_LIMIT_EXCEED					= 0x09,
	RBLE_COMMAND_DISALLOWED					= 0x0C,
	RBLE_CONN_REJ_LIMITED_RESOURCES			= 0x0D,
	RBLE_CONN_REJ_SECURITY_REASONS			= 0x0E,
	RBLE_CONN_REJ_UNACCEPTABLE_BDADDR		= 0x0F,
	RBLE_CONN_ACCEPT_TIMEOUT_EXCEED			= 0x10,
	RBLE_UNSUPPORTED						= 0x11,
	RBLE_INVALID_HCI_PARAM					= 0x12,
	RBLE_REMOTE_USER_TERM_CON				= 0x13,
	RBLE_REMOTE_DEV_TERM_LOW_RESOURCES		= 0x14,
	RBLE_REMOTE_DEV_POWER_OFF				= 0x15,
	RBLE_CON_TERM_BY_LOCAL_HOST				= 0x16,
	RBLE_REPEATED_ATTEMPTS					= 0x17,
	RBLE_PAIRING_NOT_ALLOWED				= 0x18,
	RBLE_UNSUPPORTED_REMOTE_FEATURE			= 0x1A,
	RBLE_UNSPECIFIED_ERROR					= 0x1F,
	RBLE_LMP_RSP_TIMEOUT					= 0x22,
	RBLE_ENC_MODE_NOT_ACCEPT				= 0x25,
	RBLE_LINK_KEY_CANT_CHANGE				= 0x26,
	RBLE_INSTANT_PASSED						= 0x28,
	RBLE_PAIRING_WITH_UNIT_KEY_NOT_SUP		= 0x29,
	RBLE_DIFF_TRANSACTION_COLLISION			= 0x2A,
	RBLE_CHANNEL_CLASS_NOT_SUP				= 0x2E,
	RBLE_INSUFFICIENT_SECURITY				= 0x2F,
	RBLE_PARAM_OUT_OF_MAND_RANGE			= 0x30,
	RBLE_SP_NOT_SUPPORTED_HOST				= 0x37,
	RBLE_HOST_BUSY_PAIRING					= 0x38,
	RBLE_CONTROLLER_BUSY					= 0x3A,
	RBLE_UNACCEPTABLE_CONN_INT				= 0x3B,
	RBLE_DIRECT_ADV_TO						= 0x3C,
	RBLE_TERMINATED_MIC_FAILURE				= 0x3D,
	RBLE_CONN_FAILED_TO_BE_ES				= 0x3E,

	/* GAP Specific Error */
	RBLE_GAP_INVALID_PARAM_ERR				= 0x40,
	RBLE_GAP_AUTO_EST_ERR,
	RBLE_GAP_SELECT_EST_ERR,
	RBLE_GAP_SET_RECON_ADDR_ERR,
	RBLE_GAP_SET_PRIVACY_FEAT_ERR,

	/* GATT Specific Error */
	RBLE_GATT_INVALID_PARAM_ERR				= 0x50,
	RBLE_GATT_INDICATE_NOT_ALLOWED,
	RBLE_GATT_NOTIFY_NOT_ALLOWED,
	RBLE_GATT_INVALID_TYPE_IN_SVC_SEARCH,
	RBLE_GATT_ATTRIBUTE_CLIENT_MISSING,
	RBLE_GATT_ATTRIBUTE_SERVER_MISSING,
	RBLE_GATT_RELIABLE_WRITE_ERR,
	RBLE_GATT_BUFF_OVER_ERR,

	/* ATT Specific Error */
	RBLE_ATT_INVALID_PARAM_ERR				= 0x60,

	/* SM Specific Error */
	RBLE_SM_INVALID_PARAM_ERR				= 0x70,
	RBLE_SM_PAIR_ERR_PASSKEY_ENTRY_FAILED,
	RBLE_SM_PAIR_ERR_OOB_NOT_AVAILABLE,
	RBLE_SM_PAIR_ERR_AUTH_REQUIREMENTS,
	RBLE_SM_PAIR_ERR_CFM_VAL_FAILED,
	RBLE_SM_PAIR_ERR_PAIRING_NOT_SUPPORTED,
	RBLE_SM_PAIR_ERR_ENCRYPTION_KEY_SIZE,
	RBLE_SM_PAIR_ERR_CMD_NOT_SUPPORTED,
	RBLE_SM_PAIR_ERR_UNSPECIFIED_REASON,
	RBLE_SM_PAIR_ERR_REPEATED_ATTEMPTS,
	RBLE_SM_PAIR_ERR_INVALID_PARAMS,

	/* L2CAP Specific Error */
	RBLE_L2C_INVALID_PARAM_ERR				= 0x80,

	/* Profile Specific Error */
    RBLE_PRF_ERR_INVALID_PARAM              = 0x90,         /* Invalid parameter in request */
    RBLE_PRF_ERR_INEXISTENT_HDL,                            /* Inexistent handle for sending a read/write characteristic request */
    RBLE_PRF_ERR_STOP_DISC_CHAR_MISSING,                    /* Discovery stopped due to missing attribute according to specification */
    RBLE_PRF_ERR_MULTIPLE_IAS,                              /* Multiple IAS instances found-> protocol violation */
    RBLE_PRF_ERR_INCORRECT_PROP,                            /* Incorrect properties for Alert Level Char. */
    RBLE_PRF_ERR_MULTIPLE_CHAR,                             /* Multiple instances of Alert Level Char. found-> protocol violation */
    RBLE_PRF_ERR_NOT_WRITABLE,                              /* Attribute write not allowed */
    RBLE_PRF_ERR_NOT_READABLE,                              /* Attribute read not allowed */
    RBLE_PRF_ERR_REQ_DISALLOWED,                            /* Request not allowed */
    RBLE_PRF_ERR_NTF_DISABLED,                              /* Notification Not Enabled */
    RBLE_PRF_ERR_IND_DISABLED,                              /* Indication Not Enabled */
    RBLE_PRF_ERR_ATT_NOT_SUPPORTED,                         /* Attribute value not supported by profile */

	/*------ When adding it, it's added in after a while. -----*/
	RBLE_ERR								= 0xF0,			/* Error */
	RBLE_TRANS_ERR							= 0xF1,			/* Communication error */
	RBLE_STATUS_ERROR						= 0xF2,			/* Status error */
	RBLE_PARAM_ERR							= 0xF3,			/* Parameter error */
	RBLE_BUSY								= 0xF4,			/* Busy error */
	RBLE_SHORTAGE_OF_RESOURCE				= 0xF5,			/* It was reset by shortage of resource. */
	RBLE_EXIT								= 0xF6,			/* exit */
	RBLE_VERSION_FAIL						= 0xF7,			/* Version does not match. */
	RBLE_TEST_VERSION						= 0xF8			/* Software is a test version. */
};

/* Error code Declaration */
typedef uint8_t	RBLE_STATUS;

/* Type define of status */
enum RBLE_MODE_enum {
	RBLE_MODE_INITIALIZE = 0,	/* INITIALIZE Mode */
	RBLE_MODE_ACTIVE,			/* ACTIVE Mode     */
	RBLE_MODE_RESET,			/* RESET Mode      */
	RBLE_MODE_SLEEP,			/* SLEEP Mode      */
	RBLE_MODE_ERROR				/* Error           */
};

typedef uint8_t		RBLE_MODE;

#define RBLE_EVENT_ID_MASK 0xFF

#ifndef	TRUE
#define	TRUE		1
#endif

#ifndef	FALSE
#define	FALSE		0
#endif

#ifndef	int_t
typedef	signed int		int_t;
#endif
#ifndef	uint_t
typedef	unsigned int	uint_t;
#endif

#ifndef	BOOL
typedef	unsigned char	BOOL;
#endif

/**************************** Structure Declaration ***********************************/
typedef void ( *RBLE_INIT_CB )(RBLE_MODE mode );	/* RBLE EVNT CALL BACK Declaration */

/**************************** PROTOTYPE Declaration ***********************************/
#endif // __RBLE_INC__
