/**
 ****************************************************************************************
 *
 * @file	prf_config.h
 *
 * @brief	Definition of Configuration Parameters for Profile
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: 9892 $
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _PRF_CONFIG_H_
#define _PRF_CONFIG_H_

/**
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"
#include "rwble_config.h"

#include "co_bt.h"
#include "rble_api.h"

#include "prf_sel.h"

#include "rble_api_custom.h"

/**
 * TYPE DEFINES
 ****************************************************************************************
 */


/**
 * DEFINES
 ****************************************************************************************
 */
#define DB_TYPE_128BIT_UUID					0xFFFFu	/**< entry type 128bit UUID */

#define REPORT_REFERENCE_SIZE				0x02	/**< Report Reference Descriptor Size */
#define HGHD_KB_INPUT_REPORT_MAX			0x08	/**< Boot Keyboard Input Report characteristic value Max Size */
#define HGHD_KB_OUTPUT_REPORT_MAX			0x08	/**< Boot Keyboard Output Report characteristic value Max Size */
#define HGHD_MO_INPUT_REPORT_MAX			0x08	/**< Boot Mouse Input Report characteristic value Max Size */

#define HRPS_PACK_MAX                       0x14    /**< Heart Raete Measurement characteristic value Max Size */
#define HRPS_RR_INTERVAL_MAX                0x09    /**< Heart Raete Measurement characteristic r-r interval value Max Size */

#define CSCPS_MEAS_MAX                      0x0B    /**< CSC Measurement characteristic value Max Size */
#define CSCPS_SC_CP_MAX                     0x14    /**< SC Control Point characteristic value Max Size */
#define CSCPS_LOCATION_MAX                  0x11    /**< CSC Sensor Location characteristic value Max Size */

#define GLPS_MEAS_MAX                       0x11    /**< Glucose Measurement characteristic value Max Size */
#define GLPS_MEAS_CONTEXT_MAX               0x11    /**< Glucose Measurement Context characteristic value Max Size */
#define GLPS_RA_CP_MAX                      0x11    /**< RA Control Point characteristic value Max Size */

#define CPPS_MEAS_MAX                       0x14    /**< Cycling Power Measurement characteristic value Max Size */
#define CPPS_VECTOR_MAX                     0x14    /**< Cycling Power Vector characteristic value Max Size */
#define CPPS_CP_CP_MAX                      0x14    /**< Cycling Power Control Point characteristic value Max Size */
#define CPPS_LOCATION_MAX                   0x11    /**< Sensor Location characteristic value Max Size */

#define ANPS_NEW_ALERT_MAX                  0x14    /**< AN New Alert characteristic value Max Size */

#define LNPS_LN_FEATURE_MAX                 0x04    /**< LN Feature characteristic value Max Size */
#define LNPS_LOCATION_SPEED_MAX             0x14    /**< Location Speed characteristic value Max Size */
#define LNPS_POSITION_QUALITY_MAX           0x10    /**< Position Quality characteristic value Max Size */
#define LNPS_LN_CP_MAX                      0x14    /**< LN Control Point characteristic value Max Size */
#define LNPS_NAVIGATION_MAX                 0x13    /**< Navigation characteristic value Max Size */

#define RSCPS_RSC_MEAS_MAX                  0x0A    /**< RSC Measurement characteristic value Max Size */
#define RSCPS_RSC_FEATURE_MAX               0x02    /**< RSC Feature characteristic value Max Size */
#define RSCPS_SC_CP_MAX                     0x14    /**< SC Control Point characteristic value Max Size */
#define RSCPS_LOCATION_MAX                  0x11    /**< Sensor Location characteristic value Max Size */


/**
 * DEFINES (initial value for GATT and GAP)
 ****************************************************************************************
 */

/*
 * GAP Device name Characteristic
 * Default device name
 */
#define GAP_DEV_NAME					"Renesas-BLE"
#define GAP_DEV_NAME_SIZE				(sizeof(GAP_DEV_NAME) - 1)

/*
 * Scan interval/window for device search (performing Limited/General Discovery procedure)
 */
#define GAP_DEV_SEARCH_TIME				0x0300		/* 7.68sec = 768*10ms */
#define GAP_DEV_SEARCH_SCAN_INTV		0x0012
#define GAP_DEV_SEARCH_SCAN_WINDOW		0x0012

/*
 * Maximum time to remain advertising when in the limited discoverable mode
 * Required value : 180sec
 */
#define GAP_LIM_ADV_TIMEOUT				0x0C00		/* 30.72sec = 3072*10ms */

/*
 * Scan interval in any discovery or connection establishment
 * procedure when user initiated: TGAP(scan_fast_interval)
 * recommended value: 30 to 60 ms; N * 0.625
 */
#define GAP_SCAN_FAST_INTV				0x0030

/*
 * Scan window in any discovery or connection establishment
 * procedure when user initiated: TGAP(scan_fast_window)
 * recommended value: 30 ms; N * 0.625
 */
#define GAP_SCAN_FAST_WINDOW			0x0030

/*
 * Minimum to maximum connection interval upon any connection
 * establishment: TGAP(initial_conn_interval)
 * recommended value: 30 to 50 ms ; N * 1.25 ms
 */
#define GAP_INIT_CONN_MIN_INTV			0x0018
#define GAP_INIT_CONN_MAX_INTV			0x0028

/* 
 * Minimum/Maximum connection event length
 */
#define GAP_CONN_MIN_CE_LENGTH			0x0000
#define GAP_CONN_MAX_CE_LENGTH			0xFFFF

/* 
 * slave latency
 */
#define GAP_CONN_SLAVE_LATENCY			0x0000

/* 
 * supervision timeout
 * recommended value: 20s
 */
#define GAP_DEV_SUPERVISION_TIMEOUT		0x07D0

/*
 * Recommended value : 15min
 * 0x3A98 = 150sec(15000 * 10ms) = 2.5min
 */
#define GAP_RESOLVABLE_PRIVATE_ADDR_INTV	0x3A98

/*
 * Appearance or Icon value
 * see spec, https://developer.bluetooth.org/gatt/characteristics/Pages/CharacteristicViewer.aspx?u=org.bluetooth.characteristic.gap.appearance.xml
 */
#define GAP_APPEARANCE_CHAR_VAL			0x0000

/** Slave Preferred Parameter value */
#define GAP_PPCP_CONN_INTV_MAX			0x0064u
#define GAP_PPCP_CONN_INTV_MIN			0x00C8u
#define GAP_PPCP_SLAVE_LATENCY			0x0000u
#define GAP_PPCP_STO_MULT				0x07D0u

/** Service Changed */
/* Start Handle */
#define GATT_SERVICE_CHANGED_START_HDL	0x0001
/* End Handle */
#define GATT_SERVICE_CHANGED_END_HDL	0xFFFF

/** Service Changed indication configuration */
#define GATT_SERVICE_CHANGED_IND_EN		0x0000


/** Attribute database index */
enum
{
    /* Invalid index*/
	ATT_INVALID_IDX = 0x0000,

	/* Generic Access Profile Service */
	GAP_IDX_PRIM_SVC,
	GAP_IDX_CHAR_DEVNAME,
	GAP_IDX_DEVNAME,
	GAP_IDX_CHAR_ICON,
	GAP_IDX_ICON,
	GAP_IDX_CHAR_SLAVE_PREF_PARAM,
	GAP_IDX_SLAVE_PREF_PARAM,

	/* Generic Attribute Profile Service */
	GATT_IDX_PRIM_SVC,
	GATT_IDX_CHAR_SVC_CHANGED,
	GATT_IDX_SVC_CHANGED,
    GATT_IDX_SVC_CHANGED_IND_CFG,
	
	/* Firmware update Service */
	FWUS_IDX_SVC,
	FWUS_IDX_DATA_CNTL_CHAR,
	FWUS_IDX_DATA_CNTL_VAL,
	FWUS_IDX_DATA_CHAR,
	FWUS_IDX_DATA_VAL,

	/* Link Loss Service */
	LLS_IDX_PRIM_SVC,
	LLS_IDX_ALERT_LVL_CHAR,
	LLS_IDX_ALERT_LVL_VAL,

	/* TX Power Service */
	TPS_IDX_PRIM_SVC,
	TPS_IDX_TXPW_LVL_CHAR,
	TPS_IDX_TXPW_LVL_VAL,

	/* Immediate Alert Service */
	IAS_IDX_PRIM_SVC,
	IAS_IDX_ALERT_LVL_CHAR,
	IAS_IDX_ALERT_LVL_VAL,

	/* Health Thermometer Service */
	HTS_IDX_SVC,
	HTS_IDX_TEMP_MEAS_CHAR,
	HTS_IDX_TEMP_MEAS_VAL,
	HTS_IDX_TEMP_MEAS_IND_CFG,
	HTS_IDX_TEMP_TYPE_CHAR,
	HTS_IDX_TEMP_TYPE_VAL,
	HTS_IDX_INTERM_TEMP_CHAR,
	HTS_IDX_INTERM_TEMP_VAL,
	HTS_IDX_INTERM_TEMP_CFG,
	HTS_IDX_MEAS_INTV_CHAR,
	HTS_IDX_MEAS_INTV_VAL,
	HTS_IDX_MEAS_INTV_CFG,
	HTS_IDX_MEAS_INTV_RANGE_VAL,

	/* Blood Pressure Service */
	BLS_IDX_SVC,
	BLS_IDX_BLDPRS_MEAS_CHAR,
	BLS_IDX_BLDPRS_MEAS_VAL,
	BLS_IDX_BLDPRS_MEAS_IND_CFG,
	BLS_IDX_INTERM_CUFPRS_CHAR,
	BLS_IDX_INTERM_CUFPRS_VAL,
	BLS_IDX_INTERM_CUFPRS_CFG,
	BLS_IDX_BLDPRS_FEAT_CHAR,
	BLS_IDX_BLDPRS_FEAT_VAL,

	/* Device information service */
	DIS_IDX_SVC,
	DIS_IDX_SYS_ID_CHAR,
	DIS_IDX_SYS_ID_VAL,
	DIS_IDX_MODEL_NB_CHAR,
	DIS_IDX_MODEL_NB_VAL,
	DIS_IDX_SERIAL_NB_CHAR,
	DIS_IDX_SERIAL_NB_VAL,
	DIS_IDX_FW_REV_CHAR,
	DIS_IDX_FW_REV_VAL,
	DIS_IDX_HW_REV_CHAR,
	DIS_IDX_HW_REV_VAL,
	DIS_IDX_SW_REV_CHAR,
	DIS_IDX_SW_REV_VAL,
	DIS_IDX_MANUF_NAME_CHAR,
	DIS_IDX_MANUF_NAME_VAL,
	DIS_IDX_IEEE_CERTIF_CHAR,
	DIS_IDX_IEEE_CERTIF_VAL,
	DIS_IDX_PNP_ID_CHAR,
	DIS_IDX_PNP_ID_VAL,

	/* HID Service */
	HIDS1_IDX_SVC,
	HIDS1_IDX_INCLUDE_SVC,
	HIDS1_IDX_PROTOCOL_MD_CHAR,
	HIDS1_IDX_PROTOCOL_MD_VAL,
	HIDS1_IDX_REPORT_INPUT_CHAR,
	HIDS1_IDX_REPORT_INPUT_VAL,
	HIDS1_IDX_REPORT_INPUT_CFG,
	HIDS1_IDX_INPUT_REP_REF,
	HIDS1_IDX_REPORT_OUTPUT_CHAR,
	HIDS1_IDX_REPORT_OUTPUT_VAL,
	HIDS1_IDX_OUTPUT_REP_REF,
	HIDS1_IDX_REPORT_FEATURE_CHAR,
	HIDS1_IDX_REPORT_FEATURE_VAL,
	HIDS1_IDX_FEATURE_REP_REF,
	HIDS1_IDX_REPORT_MAP_CHAR,
	HIDS1_IDX_REPORT_MAP_VAL,
	HIDS1_IDX_EXTERNAL_REP_REF,
	HIDS1_IDX_BOOTKB_INPUT_CHAR,
	HIDS1_IDX_BOOTKB_INPUT_VAL,
	HIDS1_IDX_BOOTKB_INPUT_CFG,
	HIDS1_IDX_BOOTKB_OUTPUT_CHAR,
	HIDS1_IDX_BOOTKB_OUTPUT_VAL,
	HIDS1_IDX_BOOTMO_INPUT_CHAR,
	HIDS1_IDX_BOOTMO_INPUT_VAL,
	HIDS1_IDX_BOOTMO_INPUT_CFG,
	HIDS1_IDX_HID_INFO_CHAR,
	HIDS1_IDX_HID_INFO_VAL,
	HIDS1_IDX_HID_CP_CHAR,
	HIDS1_IDX_HID_CP_VAL,

	HIDS2_IDX_SVC,
	HIDS2_IDX_INCLUDE_SVC,
	HIDS2_IDX_PROTOCOL_MD_CHAR,
	HIDS2_IDX_PROTOCOL_MD_VAL,
	HIDS2_IDX_REPORT_INPUT_CHAR,
	HIDS2_IDX_REPORT_INPUT_VAL,
	HIDS2_IDX_REPORT_INPUT_CFG,
	HIDS2_IDX_INPUT_REP_REF,
	HIDS2_IDX_REPORT_OUTPUT_CHAR,
	HIDS2_IDX_REPORT_OUTPUT_VAL,
	HIDS2_IDX_OUTPUT_REP_REF,
	HIDS2_IDX_REPORT_FEATURE_CHAR,
	HIDS2_IDX_REPORT_FEATURE_VAL,
	HIDS2_IDX_FEATURE_REP_REF,
	HIDS2_IDX_REPORT_MAP_CHAR,
	HIDS2_IDX_REPORT_MAP_VAL,
	HIDS2_IDX_EXTERNAL_REP_REF,
	HIDS2_IDX_BOOTKB_INPUT_CHAR,
	HIDS2_IDX_BOOTKB_INPUT_VAL,
	HIDS2_IDX_BOOTKB_INPUT_CFG,
	HIDS2_IDX_BOOTKB_OUTPUT_CHAR,
	HIDS2_IDX_BOOTKB_OUTPUT_VAL,
	HIDS2_IDX_BOOTMO_INPUT_CHAR,
	HIDS2_IDX_BOOTMO_INPUT_VAL,
	HIDS2_IDX_BOOTMO_INPUT_CFG,
	HIDS2_IDX_HID_INFO_CHAR,
	HIDS2_IDX_HID_INFO_VAL,
	HIDS2_IDX_HID_CP_CHAR,
	HIDS2_IDX_HID_CP_VAL,

	/* Battery Service */
	BAS1_IDX_SVC,
	BAS1_IDX_BATTERY_LVL_CHAR,
	BAS1_IDX_BATTERY_LVL_VAL,
	BAS1_IDX_BATTERY_LVL_CFG,
	BAS1_IDX_CHAR_PRESEN_FMT,
	BAS1_IDX_BATT_REP_REF,

	BAS2_IDX_SVC,
	BAS2_IDX_BATTERY_LVL_CHAR,
	BAS2_IDX_BATTERY_LVL_VAL,
	BAS2_IDX_BATTERY_LVL_CFG,
	BAS2_IDX_CHAR_PRESEN_FMT,
	BAS2_IDX_BATT_REP_REF,

	/* Scan Parameters Service */
	SCANS_IDX_SVC,
	SCANS_IDX_INTV_WINDOW_CHAR,
	SCANS_IDX_INTV_WINDOW_VAL,
	SCANS_IDX_REFRESH_CHAR,
	SCANS_IDX_REFRESH_VAL,
	SCANS_IDX_REFRESH_CFG,

	/* Heart Rate Service */
	HRS_IDX_SVC,
	HRS_IDX_HRTRATE_MEAS_CHAR,
	HRS_IDX_HRTRATE_MEAS_VAL,
	HRS_IDX_HRTRATE_MEAS_NTF_CFG,
	HRS_IDX_HRTRATE_BSL_CHAR,
	HRS_IDX_HRTRATE_BSL_VAL,
	HRS_IDX_HRTRATE_CP_CHAR,
	HRS_IDX_HRTRATE_CP_VAL,

	/* Cycling Speed and Cadence Service */
	CSCS_IDX_SVC,
	CSCS_IDX_CSC_MEAS_CHAR,
	CSCS_IDX_CSC_MEAS_VAL,
	CSCS_IDX_CSC_MEAS_NTF_CFG,
	CSCS_IDX_CSC_FEATURE_CHAR,
	CSCS_IDX_CSC_FEATURE_VAL,
	CSCS_IDX_SENSOR_LOC_CHAR,
	CSCS_IDX_SENSOR_LOC_VAL,
	CSCS_IDX_SC_CP_CHAR,
	CSCS_IDX_SC_CP_VAL,
	CSCS_IDX_SC_CP_IND_CFG,

	/* Glucose Service */
	GLS_IDX_SVC,
	GLS_IDX_MEAS_CHAR,
	GLS_IDX_MEAS_VAL,
	GLS_IDX_MEAS_NTF_CFG,
	GLS_IDX_MEAS_CONTEXT_CHAR,
	GLS_IDX_MEAS_CONTEXT_VAL,
	GLS_IDX_MEAS_CONTEXT_NTF_CFG,
	GLS_IDX_FEATURE_CHAR,
	GLS_IDX_FEATURE_VAL,
	GLS_IDX_RA_CP_CHAR,
	GLS_IDX_RA_CP_VAL,
	GLS_IDX_RA_CP_IND_CFG,

	/* Cycling Power Service */
	CPS_IDX_SVC,
	CPS_IDX_MEAS_CHAR,
	CPS_IDX_MEAS_VAL,
	CPS_IDX_MEAS_NTF_CFG,
	CPS_IDX_MEAS_BRD_CFG,
	CPS_IDX_FEATURE_CHAR,
	CPS_IDX_FEATURE_VAL,
	CPS_IDX_SENSOR_LOCATION_CHAR,
	CPS_IDX_SENSOR_LOCATIONVAL,
	CPS_IDX_VECTOR_CHAR,
	CPS_IDX_VECTOR_VAL,
	CPS_IDX_VECTOR_CFG,
	CPS_IDX_CP_CHAR,
	CPS_IDX_CP_VAL,
	CPS_IDX_CP_CFG,
	
	/* Current Time Service */
	CTS_IDX_SVC,
	CTS_IDX_CURRENT_TIME_CHAR,
	CTS_IDX_CURRENT_TIME_VAL,
	CTS_IDX_CURRENT_TIME_NTF_CFG,
	CTS_IDX_LOCAL_TIME_CHAR,
	CTS_IDX_LOCAL_TIME_VAL,
	CTS_IDX_REF_TIME_CHAR,
	CTS_IDX_REF_TIME_VAL,

	/* Next DST Change Service */
	NDCS_IDX_SVC,
	NDCS_IDX_TIME_DST_CHAR,
	NDCS_IDX_TIME_DST_VAL,

	/* Reference Time Update Service */
	RTUS_IDX_SVC,
	RTUS_IDX_TIME_UPDATE_CP_CHAR,
	RTUS_IDX_TIME_UPDATE_CP_VAL,
	RTUS_IDX_TIME_UPDATE_STATE_CHAR,
	RTUS_IDX_TIME_UPDATE_STATE_VAL,
	
	/* Alert Notification Service */
	ANPS_IDX_SVC,
	ANPS_IDX_SUPP_NEW_AL_CATEGORY_CHAR,
	ANPS_IDX_SUPP_NEW_AL_CATEGORY_VAL,
	ANPS_IDX_NEW_ALERT_CHAR,
	ANPS_IDX_NEW_ALERT_VAL,
	ANPS_IDX_NEW_ALERT_CFG,
	ANPS_IDX_SUPP_UNREAD_CATEGORY_CHAR,
	ANPS_IDX_SUPP_UNREAD_CATEGORY_VAL,
	ANPS_IDX_UNREAD_ALERT_STATUS_CHAR,
	ANPS_IDX_UNREAD_ALERT_STATUS_VAL,
	ANPS_IDX_UNREAD_ALERT_STATUS_CFG,
	ANPS_IDX_AL_NTF_CTRL_POINT_CHAR,
	ANPS_IDX_AL_NTF_CTRL_POINT_VAL,
	
	/* Location and Navigation Service */
	LNS_IDX_SVC,
	LNS_IDX_LN_FEATURE_CHAR,
	LNS_IDX_LN_FEATURE_VAL,
	LNS_IDX_LOC_SPD_CHAR,
	LNS_IDX_LOC_SPD_VAL,
	LNS_IDX_LOC_SPD_NTF_CFG,
	LNS_IDX_POS_QUALITY_CHAR,
	LNS_IDX_POS_QUALITY_VAL,
	LNS_IDX_LN_CP_CHAR,
	LNS_IDX_LN_CP_VAL,
	LNS_IDX_LN_CP_IND_CFG,
	LNS_IDX_NAVI_CHAR,
	LNS_IDX_NAVI_VAL,
	LNS_IDX_NAVI_NTF_CFG

};

enum
{

	/* Phone Alert Status Service */
	PASS_IDX_SVC = LNS_IDX_NAVI_NTF_CFG + 1,
	PASS_IDX_ALERT_STATUS_CHAR,
	PASS_IDX_ALERT_STATUS_VAL,
	PASS_IDX_ALERT_STATUS_NTF_CFG,
	PASS_IDX_RINGER_SETTING_CHAR,
	PASS_IDX_RINGER_SETTING_VAL,
	PASS_IDX_RINGER_SETTING_NTF_CFG,
	PASS_IDX_RINGER_CP_CHAR,
	PASS_IDX_RINGER_CP_VAL,

	/* Running Speed and Cadence Service */
	RSCS_IDX_SVC,
	RSCS_IDX_RSC_MEAS_CHAR,
	RSCS_IDX_RSC_MEAS_VAL,
	RSCS_IDX_RSC_MEAS_NTF_CFG,
	RSCS_IDX_RSC_FEATURE_CHAR,
	RSCS_IDX_RSC_FEATURE_VAL,
	RSCS_IDX_SENSOR_LOC_CHAR,
	RSCS_IDX_SENSOR_LOC_VAL,
	RSCS_IDX_SC_CP_CHAR,
	RSCS_IDX_SC_CP_VAL,
	RSCS_IDX_SC_CP_IND_CFG,
	/* Sample Custom Service */
	SCS_IDX_SVC,
	SCS_IDX_NOTIFY_CHAR,
	SCS_IDX_NOTIFY_VAL,
	SCS_IDX_NOTIFY_CFG,
	SCS_IDX_IND_CHAR,
	SCS_IDX_IND_VAL,
	SCS_IDX_IND_CFG,
	SCS_IDX_INTERVAL_CHAR,
	SCS_IDX_INTERVAL_VAL,
	SCS_IDX_NOTIFY_LEN_CHAR,
	SCS_IDX_NOTIFY_LEN_VAL,
	SCS_IDX_IND_LEN_CHAR,
	SCS_IDX_IND_LEN_VAL,
	/* Simple Sample Custom Service */
	SAMS_IDX_SVC,
	SAMS_IDX_SWITCH_STATE_CHAR,
	SAMS_IDX_SWITCH_STATE_VAL,
	SAMS_IDX_SWITCH_STATE_CCCD,
	SAMS_IDX_LED_CONTROL_CHAR,
	SAMS_IDX_LED_CONTROL_VAL
};
/**
 * STRUCTURE DECLARATIONS
 ****************************************************************************************
 */

/** Devaice Name variable */
struct device_name
{
	/** length for name */
	uint8_t		namelen;
	/** array of bytes for name */
	uint8_t		name[RBLE_BD_NAME_SIZE];
};

/** HID Service variable */
struct hid_service
{
	/** Protocol Mode characteristic value */
	uint8_t		protocol_mode;
	/** Characteristic value HID Control Point */
	uint8_t		hid_cp;
	/** Report characteristic value (Input) */
	uint8_t		input_report[RBLE_HIDS_REPORT_MAX];
	/** Permission settings notification of the characteristic values (Input) Report */
	uint16_t	report_input_ntf_en;
	/** Report Reference descriptor values of characteristic (Input) Report */
	uint8_t		report_input_ref[REPORT_REFERENCE_SIZE];
	/** Report characteristic value (Output) */
	uint8_t		output_report[RBLE_HIDS_REPORT_MAX];
	/** Report Reference descriptor values of property (Output) Report */
	uint8_t		report_output_ref[REPORT_REFERENCE_SIZE];
	/** Report a characteristic value (Feature) */
	uint8_t		feature_report[RBLE_HIDS_REPORT_MAX];
	/** Report Reference descriptor values of characteristic (Feature) Report */
	uint8_t		report_feature_ref[REPORT_REFERENCE_SIZE];
	/** The characteristic values Boot Keyboard Input Report */
	uint8_t		kb_input_report[HGHD_KB_INPUT_REPORT_MAX];
	/** Permit notification of the characteristic values set Boot Keyboard Input Report */
	uint16_t	kb_input_report_ntf_en;
	/** The characteristic values Boot Keyboard Output Report */
	uint8_t		kb_output_report[HGHD_KB_OUTPUT_REPORT_MAX];
	/** The characteristic values Boot Mouse Input Report */
	uint8_t		mo_input_report[HGHD_MO_INPUT_REPORT_MAX];
	/** Permit notification of the characteristic values set Boot Mouse Input Report */
	uint16_t	mo_input_report_ntf_en;
};

/** Battery Service variable */
struct batt_service
{
	/** Battery Level permission settings notification of the characteristic values */
	uint16_t	battery_lvl_ntf_en;
	/** Report Reference descriptor values of property Battery Level */
	uint8_t		battery_lvl_rep_ref[REPORT_REFERENCE_SIZE];
	/** Characteristic value Battery Level */
	uint8_t		battery_lvl;
};

/** Scan interval window characteristic variable */
struct svc_scans_intv_window
{
	/** scan interval value */
	uint16_t le_scan_interval;
	/** scan window value */
	uint16_t le_scan_window;
};

/** Attribute data server entry */
struct atts_desc
{
	/** Element UUID */
	uint16_t type;
	/** Maximum length of the element */
	uint8_t maxlength;
	/** Current length of the element */
	uint8_t length;
	/** TaskId and Attribute Idx of the upper profile */
	ke_task_id_t taskid;
	/** Attribute permission */
	uint16_t perm;
	/** Pointer to the element value */
	void *value;
};

struct atts_elmt_128
{
	/** attribute UUID */
	uint8_t attr_uuid[RBLE_GATT_128BIT_UUID_OCTET];
	/** length of the UUID */
	uint8_t uuid_len;
	/** Pointer to the element value */
	void *value;
};


/**
 * DEFINES
 ****************************************************************************************
 */
#define ATTS_CHAR(prop, handle, type)	{ (uint8_t)((prop)),								\
										{ (uint8_t)((handle)),	(uint8_t)((handle) >> 8)},	\
										{ (uint8_t)((type)),	(uint8_t)((type) >> 8)}}
#define ATTS_INCL(start, end, uuid)		{ { (uint8_t)((start)),	(uint8_t)((start) >> 8)},	\
										  { (uint8_t)((end)),	(uint8_t)((end) >> 8)},		\
										  { (uint8_t)((uuid)),	(uint8_t)((uuid) >> 8)} }

#define TASK_ATTID(taskid, idx)			(uint16_t)((taskid << 10) | idx)

#define ATTS_32BIT_UUID(uuid)			{ 0xFB, 0x34, 0x9B, 0x5F, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00,		\
										 (uint8_t)(0x000000FF & uuid), (uint8_t)((0x0000FF00 & uuid) >> 8),				\
										 (uint8_t)((0x00FF0000 & uuid) >> 16), (uint8_t)((0xFF000000 & uuid) >> 24) }

/**
 * STRUCTURE DECLARATIONS
 ****************************************************************************************
 */

/** Characteristic Value Descriptor */
struct atts_char_desc
{
	/** properties */
	uint8_t prop;
	/** attribute handle */
	uint8_t attr_hdl[ sizeof(uint16_t) ];
	/** attribute type */
	uint8_t attr_type[ RBLE_GATT_16BIT_UUID_OCTET ];
};

/** 128bit Characteristic Value Descriptor */
struct atts_char128_desc
{
	/** properties */
	uint8_t prop;
	/** attribute handle */
	uint8_t attr_hdl[ sizeof(uint16_t) ];
	/** attribute type */
	uint8_t attr_type[ RBLE_GATT_128BIT_UUID_OCTET ];
};

/** include service entry element */
struct att_incl_desc
{
	/** start handle value of included service */
	uint8_t start_hdl[ sizeof(uint16_t) ];
	/** end handle value of included service */
	uint8_t end_hdl[ sizeof(uint16_t) ];
	/** attribute value UUID */
	uint8_t uuid[ RBLE_GATT_16BIT_UUID_OCTET ];
};

/** Slave preferred connection parameters */
struct atts_slv_pref
{
	/** Connection interval maximum */
	uint8_t con_intv_max[ sizeof(uint16_t) ];
	/** Connection interval minimum */
	uint8_t con_intv_min[ sizeof(uint16_t) ];
	/** Slave latency */
	uint8_t slave_latency[ sizeof(uint16_t) ];
	/** Connection supervision timeout multiplier */
	uint8_t conn_timeout[ sizeof(uint16_t) ];
};

/**
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
extern struct atts_desc *atts_desc_list;
extern const struct atts_desc atts_desc_list_prf[];
extern const struct atts_desc atts_desc_list_host[];
extern uint16_t ATT_LAST_IDX;
extern const uint16_t ATT_HOST_DB_SIZE;
extern const uint16_t ATT_PRF_DB_SIZE;

extern uint16_t gap_discovery_scan_time;
extern uint16_t gap_dev_search_scan_intv;
extern uint16_t gap_dev_search_scan_window;
extern uint16_t gap_lim_adv_timeout;
extern uint16_t gap_scan_fast_intv;
extern uint16_t gap_scan_fast_window;
extern uint16_t gap_init_conn_min_intv;
extern uint16_t gap_init_conn_max_intv;
extern uint16_t gap_conn_min_ce_length;
extern uint16_t gap_conn_max_ce_length;
extern uint16_t gap_conn_slave_latency;
extern uint16_t gap_dev_supervision_timeout;
extern uint16_t gap_resolvable_private_addr_intv;

extern uint8_t gatt_service_changed[4];
extern uint16_t gatt_service_changed_ind_en;

extern struct device_name gap_dev_name;
extern uint16_t gap_appearance_char_val;

extern uint8_t svc_lls_alert_lvl;
extern int8_t svc_txpw_lvl;
extern uint8_t svc_ias_alert_lvl;

extern uint8_t hts_packed_temp[13];
extern uint16_t hts_temp_meas_ind_en;
extern uint8_t hts_temp_type;
extern uint16_t hts_interm_temp_ntf_en;
extern uint16_t hts_meas_intv_ind_en;
extern uint16_t hts_meas_intv;
extern uint16_t hts_meas_intv_range[2];

extern uint8_t blps_packed_measurements[19];
extern uint16_t blps_bldprs_meas_ind_en;
extern uint16_t blps_interm_cufprs_ntf_en;

extern struct hid_service hids_inst[RBLE_HIDS_INST_MAX];
extern struct batt_service bas_inst[RBLE_BAS_INST_MAX];

extern struct svc_scans_intv_window scans_intv_window;
extern uint8_t scans_refresh_val;
extern uint16_t scans_refresh_ntf_en;

extern uint8_t hrps_packed_measurements[HRPS_PACK_MAX];
extern uint8_t hrps_hrtrate_control_point;
extern uint16_t hrps_hrtrate_meas_ntf_en;

extern uint8_t cscps_packed_measurements[CSCPS_MEAS_MAX];
extern uint8_t cscps_packed_sc_cp[CSCPS_SC_CP_MAX];
extern const uint8_t	cscps_support_location_num;
extern const uint8_t	cscps_support_sensor_location[CSCPS_LOCATION_MAX];
extern uint8_t cscps_sensor_location;
extern uint16_t cscps_cycspd_meas_ntf_en;
extern uint16_t cscps_sc_cp_ind_en;

extern uint8_t glps_packed_measurements[GLPS_MEAS_MAX];
extern uint8_t glps_packed_measurements_context[GLPS_MEAS_CONTEXT_MAX];
extern uint8_t glps_packed_ra_cp[GLPS_RA_CP_MAX];
extern uint16_t glps_meas_ntf_en;
extern uint16_t glps_meas_context_ntf_en;
extern uint16_t glps_ra_cp_ind_en;

extern uint8_t cpps_packed_measurements[CPPS_MEAS_MAX];
extern uint8_t cpps_packed_vector[CPPS_VECTOR_MAX];
extern uint8_t cpps_packed_cp[CPPS_CP_CP_MAX];
extern uint8_t cpps_sensor_location;
extern uint8_t cpps_supported_location[CPPS_LOCATION_MAX];
extern uint8_t cpps_supported_location_num;
extern uint8_t cpps_sensor_location;
extern uint16_t cpps_cycpwr_meas_ntf_en;
extern uint16_t cpps_cycpwr_meas_brd_en;
extern uint16_t cpps_cycpwr_vector_ntf_en;
extern uint16_t cpps_cycpwr_cp_ind_en;

extern uint8_t cts_current_time[10];
extern uint8_t cts_local_time[2];
extern uint8_t cts_ref_time[4];
extern uint16_t cts_current_time_ntf_en;
extern uint8_t ndcs_time_dst[8];
extern uint8_t rtus_time_update_cp;
extern uint8_t rtus_time_update_state[2];

extern uint8_t ans_new_alert[ANPS_NEW_ALERT_MAX];
extern uint16_t ans_new_alert_ntf_en;
extern uint8_t ans_unread_alert[2];
extern uint16_t ans_unread_alert_ntf_en;
extern uint8_t ans_alert_ntf_cp[2];

extern uint8_t  lns_location_speed[LNPS_LOCATION_SPEED_MAX];
extern uint16_t lns_location_speed_ntf_en;
extern uint8_t  lns_position_quality[LNPS_POSITION_QUALITY_MAX];
extern uint8_t  lns_ln_cp[LNPS_LN_CP_MAX];
extern uint16_t lns_ln_cp_ind_en;
extern uint8_t  lns_navigation[LNPS_NAVIGATION_MAX];
extern uint16_t lns_navigation_ntf_en;

extern uint8_t  pass_alert_status;
extern uint16_t pass_alert_status_ntf_en;
extern uint8_t  pass_ringer_setting;
extern uint16_t pass_ringer_setting_ntf_en;
extern uint8_t  pass_ringer_cp;

extern uint8_t  rscs_rsc_meas[RSCPS_RSC_MEAS_MAX];
extern uint16_t rscs_rsc_meas_ntf_en;
extern const uint8_t rscs_rsc_feature[RSCPS_RSC_FEATURE_MAX];
extern uint8_t  rscs_sensor_loc;
extern uint8_t  rscs_sc_cp[RSCPS_SC_CP_MAX];
extern uint16_t rscs_sc_cp_ind_en;
extern const uint8_t rscps_support_location_num;
extern const uint8_t rscps_support_sensor_location[RSCPS_LOCATION_MAX];

#endif /* _PRF_CONFIG_H_ */
