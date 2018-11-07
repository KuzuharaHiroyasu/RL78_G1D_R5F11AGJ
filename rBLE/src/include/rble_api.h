/**
 ****************************************************************************************
 *
 * @file		rble_api.h
 *
 * @brief	rBLE I/F API Header file
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 9892 $
 *
 ****************************************************************************************
 */

#ifndef	__RBLE_API_INC__
#define	__RBLE_API_INC__

#include "types.h"
#include "rble.h"

/**************************** Define Declaration **************************************/

#define RBLE_BD_ADDR_LEN   		 0x06
#define RBLE_BD_NAME_SIZE  		 0x41
#define RBLE_ADV_DATA_LEN  		 0x1F
#define RBLE_SCAN_RSP_DATA_LEN   0x1F
#define RBLE_KEY_LEN             0x10
#define RBLE_LE_FEATS_LEN        0x08
#define RBLE_LE_CHNL_MAP_LEN     0x05
#define RBLE_ATTM_MAX_VALUE      0x18
#define RBLE_RAND_NB_LEN         0x08

#define RBLE_MASTER				 0x00
#define RBLE_SLAVE				 0x01

#define RBLE_HIDS_INST_MAX       0x02
#define RBLE_BAS_INST_MAX        0x02

#define RBLE_HIDS_REPORT_MAX     0x20

/*================================== GAP ==================================*/
/**
 * @brief	Observation and Connection Establishment Procedure
 */
enum RBLE_GAP_OBSERV_MODE_enum {
	RBLE_GAP_OBSERVER			= 0x0800,		/**< Mode in observer */
	RBLE_GAP_AUTO_CONNECT		= 0x1000,		/**< Mode in auto connection establishment */
	RBLE_GAP_SELECT_CONNECT		= 0x2000		/**< Mode in selective connection establishment */
};

/**
 * @brief	Discovery Mode
 */
enum RBLE_GAP_DISCOVERABLE_MODE_enum {
	RBLE_GAP_NON_DISCOVERABLE	= 0x0001,		/**< Mode in non-discoverable */
	RBLE_GAP_GEN_DISCOVERABLE	= 0x0002,		/**< Mode in general discoverable */
	RBLE_GAP_LIM_DISCOVERABLE	= 0x0004,		/**< Mode in limited discoverable */
};

/**
 * @brief	Bondable Mode
 */
enum RBLE_GAP_BONDABLE_MODE_enum {
	RBLE_GAP_NON_BONDABLE       = 0x0100,       /**< Mode in non-bondable */
	RBLE_GAP_BONDABLE           = 0x0200,		/**< Mode in bondability */
};

/**
 * @brief	Broadcast Mode
 */
enum RBLE_GAP_BROADCAST_MODE_enum {
	RBLE_GAP_BROADCASTER		= 0x0400		/**< Mode in broadcaster */
};

/**
 * @brief	Connectable Mode
 */
enum RBLE_GAP_CONNECTABLE_MODE_enum {
	RBLE_GAP_NON_CONNECTABLE	= 0x0010,		/**< Mode in non-connectable */
	RBLE_GAP_UND_CONNECTABLE	= 0x0020,		/**< Mode in undirected connectable */
	RBLE_GAP_DIR_CONNECTABLE	= 0x0040		/**< Mode in directed connectable */
};


/**
 * @brief	Security Mode
 */
enum RBLE_GAP_SECURITY_MODE_enum {
	RBLE_GAP_NO_SEC				= 0x00,			/**< No security (no authentication and encryption) */
	RBLE_GAP_SEC1_NOAUTH_PAIR_ENC,				/**< Unauthenticated pairing with encryption */
	RBLE_GAP_SEC1_AUTH_PAIR_ENC,				/**< Authenticated pairing with encryption */
	RBLE_GAP_SEC2_NOAUTH_DATA_SGN,				/**< Unauthenticated pairing with data signing */
	RBLE_GAP_SEC2_AUTH_DATA_SGN					/**< Authentication pairing with data signing */
};


/**
 * @brief	Advertising type
 */
enum RBLE_GAP_ADV_TYPE_enum {
	RBLE_GAP_ADV_CONN_UNDIR		= 0x00,			/**< Connectable Undirected advertising */
	RBLE_GAP_ADV_CONN_DIR_HIGH_DUTY,			/**< Connectable high duty cycle directed advertising */
	RBLE_GAP_ADV_DISC_UNDIR,					/**< Discoverable undirected advertising */
	RBLE_GAP_ADV_NONCONN_UNDIR,					/**< Non-connectable undirected advertising */
	RBLE_GAP_ADV_CONN_DIR_LOW_DUTY				/**< Connectable low duty cycle directed advertising */
};

/**
 * @brief	Initiator Filter policy
 */
enum RBLE_GAP_INIT_FILTER_enum {
	RBLE_GAP_INIT_FILT_IGNORE_WLST	= 0x00,		/**< Initiator will ignore White List */
	RBLE_GAP_INIT_FILT_USE_WLST					/**< Initiator will use White List */
};

/**
 * @brief	Advertising channel map
 */
enum RBLE_GAP_ADV_CH_enum {
	RBLE_ADV_CHANNEL_37			= 0x01,			/**< Enable channel 37 use */
	RBLE_ADV_CHANNEL_38			= 0x02,			/**< Enable channel 38 use */
	RBLE_ADV_CHANNEL_39			= 0x04,			/**< Enable channel 39 use */
	RBLE_ADV_ALL_CHANNELS		= 0x07			/**< all channels(37, 38 and 39) enabled */
};

/**
 * @brief	Advertising filter policy
 */
enum RBLE_GAP_ADV_FILTER_enum {
	RBLE_ADV_ALLOW_SCAN_ANY_CON_ANY	= 0x00,		/**< Allow both scan and connection requests from anyone */
	RBLE_ADV_ALLOW_SCAN_WLST_CON_ANY,			/**< Allow both scan req from White List devices only and connection req from anyone */
	RBLE_ADV_ALLOW_SCAN_ANY_CON_WLST,			/**< Allow both scan req from anyone and connection req from White List devices only */
	RBLE_ADV_ALLOW_SCAN_WLST_CON_WLST			/**< Allow scan and connection requests from White List devices only */
};


/**
 * @brief	Address Type
 */
enum RBLE_GAP_ADDR_TYPE_enum {
	RBLE_ADDR_PUBLIC			= 0x00,			/**< Public BD address */
	RBLE_ADDR_RAND,								/**< Random BD Address */
};

/**
 * @brief	Scan Type
 */
enum RBLE_GAP_SCAN_TYPE_enum {
	RBLE_SCAN_PASSIVE			= 0x00,			/**< Passive scan */
	RBLE_SCAN_ACTIVE							/**< Active scan */
};

/**
 * @brief	Scan filter policy
 */
enum RBLE_GAP_SCAN_FILTER_enum {
	RBLE_SCAN_ALLOW_ADV_ALL		= 0x00,			/**< Allow advertising packets from anyone */
	RBLE_SCAN_ALLOW_ADV_WLST					/**< Allow advertising packets from White List devices only */
};

/**
 * @brief	Filter duplicates
 */
enum RBLE_GAP_SCAN_DUPLIC_enum {
	RBLE_SCAN_FILT_DUPLIC_DIS	= 0x00,			/**< Disable filtering of duplicate packets */
	RBLE_SCAN_FILT_DUPLIC_EN					/**< Enable filtering of duplicate packets */
};

/**
 * @brief	Privacy settings
 */
enum RBLE_GAP_PRIV_SETTING_enum {
	RBLE_DEVICE_PRIV_DISABLE	= 0x00,			/**< Device privacy disabled */
	RBLE_CENTRAL_PRIV_ENABLE,					/**< Central privacy enabled */
	RBLE_PH_PRIV_ENABLE,						/**< Peripheral privacy enabled */
	RBLE_BCST_PRIV_ENABLE,						/**< Broadcaster privacy enabled */
	RBLE_OBSERV_PRIV_ENABLE,					/**< Observer privacy enabled */
	RBLE_OBSERV_PRIV_RESOLVE					/**< Observer will resolve address */
};

/**
 * @brief	Key Distribution Flags
 */
enum RBLE_GAP_KEY_DIST_enum {
	RBLE_KEY_DIST_NONE			= 0x00,			/**< No Keys to distribute */
	RBLE_KEY_DIST_ENCKEY		= 0x01,			/**< Encryption key in distribution */
	RBLE_KEY_DIST_IDKEY			= 0x02,			/**< IRK (ID key)in distribution */
	RBLE_KEY_DIST_SIGNKEY		= 0x04			/**< CSRK(Signature key) in distribution */
};

/**
 * @brief	 OOB Data Present Flag
 */
enum RBLE_GAP_OOB_PRESENT_enum {
	RBLE_OOB_AUTH_DATA_NOT_PRESENT = 0x00,		/**< OOB Data not present */
	RBLE_OOB_AUTH_DATA_FROM_REMOTE_DEV_PRESENT	/**< OOB data from remote device present */
};

/**
 * @brief	 IO Capability
 */
enum RBLE_GAP_IO_CAP_enum {
	RBLE_IO_CAP_DISPLAY_ONLY	= 0x00,			/**< Display Only */
	RBLE_IO_CAP_DISPLAY_YES_NO,					/**< Display Yes No */
	RBLE_IO_CAP_KB_ONLY,						/**< Keyoard Only */
	RBLE_IO_CAP_NO_INPUT_NO_OUTPUT,				/**< No Input No Output */
	RBLE_IO_CAP_KB_DISPLAY						/**< Keyboard Display */
};

/**
 * @brief	 Authentication Requirements
 */
enum RBLE_AUTH_REQ_enum {
	RBLE_AUTH_REQ_NO_MITM_NO_BOND	= 0x00,		/**< No MITM No Bonding */
	RBLE_AUTH_REQ_NO_MITM_BOND		= 0x01,		/**< No MITM Bonding */
	RBLE_AUTH_REQ_MITM_NO_BOND		= 0x04,		/**< MITM No Bonding */
	RBLE_AUTH_REQ_MITM_BOND			= 0x05		/**< MITM and Bonding */
};


/**
 * @brief	 Device Discovery Search Type
 */
enum RBLE_GAP_DISCOVERY_TYPE_enum {
    RBLE_GAP_GEN_DISCOVERY_TYPE    = 0x00,		/**< General Discovery */
    RBLE_GAP_LIM_DISCOVERY_TYPE, 				/**< Limited Discovery */
	RBLE_GAP_CANCEL_DISCOVERY,					/**< Cancel Discovery */
};


/**
 * @brief	Bonding information
 */
enum RBLE_GAP_BOND_INFO_enum {
	RBLE_GAP_BOND_ADDED,                      /**< Bonding information is added */
	RBLE_GAP_BOND_REMOVED                     /**< Bonding information is removed */
};

/**
 * @brief	Internal codes for writing a GAP characteristic
 */
enum RBLE_GAP_WR_CHAR_CODE_enum {
	RBLE_GAP_WR_CHAR_NAME			= 0x01,		/**< Code for Device Name Char. */
	RBLE_GAP_WR_CHAR_APPEARANCE,				/**< Code for Appearance Char. */
};

/*================================== GATT ==================================*/

#define RBLE_GATT_MAX_VALUE						0x18			/**< maximum value length */
#define RBLE_GATT_MAX_HDL_LIST					0x08			/**< maximum number of handles returned */
#define RBLE_GATT_MAX_LONG_VALUE				0x48			/**< Long value length */
#define RBLE_GATT_MAX_NB_HDLS					0x04			/**< Maximum number of handles */
#define RBLE_GATT_16BIT_UUID_OCTET				0x02			/**< 16-bit UUID length */
#define RBLE_GATT_32BIT_UUID_OCTET				0x04			/**< 32-bit UUID length */
#define RBLE_GATT_128BIT_UUID_OCTET				0x10			/**< 128-bit UUID length */
#define RBLE_GATT_MAX_RELIABLE_WRITE_CONTENTS	0x10			/**< Maximum contents of reliable write */
#define RBLE_GATT_MAX_RELIABLE_WRITE_NUM		0x04			/**< Maximum number of reliable writes */


/*
 * Declarations
 *
 * Declarations are defined GATT profile attribute types.
 */

#define RBLE_DECL_PRIMARY_SERVICE				0x2800u			/**< Primary Service Declaration */
#define RBLE_DECL_SECONDARY_SERVICE				0x2801u			/**< Secondary Service Declaration */
#define RBLE_DECL_INCLUDE						0x2802u			/**< Include Declaration */
#define RBLE_DECL_CHARACTERISTIC				0x2803u			/**< Characteristic Declaration */


/*
 * Descriptors
 *
 * Descriptors are defined attributes that describe a characteristic value.
 */

#define RBLE_DESC_CHAR_EXT_PROPERTIES			0x2900u			/**< Characteristic Extended Properties */
#define RBLE_DESC_CHAR_USER_DESCRIPTION			0x2901u			/**< Characteristic User Description */
#define RBLE_DESC_CLIENT_CHAR_CONF				0x2902u			/**< Client Characteristic Configuration */
#define RBLE_DESC_SERVER_CHAR_CONF				0x2903u			/**< Server Characteristic Configuration */
#define RBLE_DESC_CHAR_PRESENTATION_FMT			0x2904u			/**< Characteristic Presentation Format */
#define RBLE_DESC_CHAR_AGGREGATE_FMT			0x2905u			/**< Characteristic Aggregate Format */
#define RBLE_DESC_VALID_RANGE					0x2906u			/**< Valid Range */
#define RBLE_DESC_EXT_REPORT_REFERENCE			0x2907u			/**< External Report Reference */
#define RBLE_DESC_REPORT_REFERENCE				0x2908u			/**< Report Reference */


/*
 * Characteristics
 *
 * Characteristics are defined attribute types that contain a single logical value.
 */

#define RBLE_CHAR_GAP_DEVICE_NAME				0x2A00u			/**< Device Name */
#define RBLE_CHAR_GAP_APPEARANCE				0x2A01u			/**< Appearance */
#define RBLE_CHAR_GAP_PH_PRIV_FLAG				0x2A02u			/**< Peripheral Privacy Flag */
#define RBLE_CHAR_GAP_RECONN_ADDRESS			0x2A03u			/**< Reconnection Address */
#define RBLE_CHAR_GAP_PH_PREF_CONN_PARAM		0x2A04u			/**< Peripheral Preferred Connection Parameters */
#define RBLE_CHAR_GATT_SERVICE_CHANGED			0x2A05u			/**< Service Changed */
#define RBLE_CHAR_ALERT_LEVEL					0x2A06u			/**< Alert Level */
#define RBLE_CHAR_TX_POWER_LEVEL				0x2A07u			/**< Tx Power Level */
#define RBLE_CHAR_DATE_TIME						0x2A08u			/**< Date Time */
#define RBLE_CHAR_DAY_OF_WEEK					0x2A09u			/**< Day of Week */
#define RBLE_CHAR_DAY_DATE_TIME					0x2A0Au			/**< Day Date Time */
#define RBLE_CHAR_EXACT_TIME_256				0x2A0Cu			/**< Exact Time 256 */
#define RBLE_CHAR_DST_OFFSET					0x2A0Du			/**< DST Offset */
#define RBLE_CHAR_TIME_ZONE						0x2A0Eu			/**< Time Zone */
#define RBLE_CHAR_LOCAL_TIME_INFO				0x2A0Fu			/**< Local Time Information */
#define RBLE_CHAR_TIME_WITH_DST					0x2A11u			/**< Time with DST */
#define RBLE_CHAR_TIME_ACCURACY					0x2A12u			/**< Time Accuracy */
#define RBLE_CHAR_TIME_SOURCE					0x2A13u			/**< Time Source */
#define RBLE_CHAR_REF_TIME_INFO					0x2A14u			/**< Reference Time Information */
#define RBLE_CHAR_TIME_UPDATE_CTRL_POINT		0x2A16u			/**< Time Update Control Point */
#define RBLE_CHAR_TIME_UPDATE_STATE				0x2A17u			/**< Time Update State */
#define RBLE_CHAR_GLUCOSE_MEASUREMENT			0x2A18u			/**< Glucose Measurement */
#define RBLE_CHAR_BATTERY_LEVEL					0x2A19u			/**< Battery Level */
#define RBLE_CHAR_TEMPERATURE_MEAS				0x2A1Cu			/**< Temperature Measurement */
#define RBLE_CHAR_TEMPERATURE_TYPE				0x2A1Du			/**< Temperature Type */
#define RBLE_CHAR_INTERMEDIATE_TEMP				0x2A1Eu			/**< Intermediate Temperature */
#define RBLE_CHAR_MEAS_INTERVAL					0x2A21u			/**< Measurement Interval */
#define RBLE_CHAR_BOOT_KB_INPUT_REPORT			0x2A22u			/**< Boot Keyboard Input Report */
#define RBLE_CHAR_SYSTEM_ID						0x2A23u			/**< System ID */
#define RBLE_CHAR_MODEL_NUMBER_STRING			0x2A24u			/**< Model Number String */
#define RBLE_CHAR_SERIAL_NUMBER_STRING			0x2A25u			/**< Serial Number String */
#define RBLE_CHAR_FW_REVISION_STRING			0x2A26u			/**< Firmware Revision String */
#define RBLE_CHAR_HW_REVISION_STRING			0x2A27u			/**< Hardware Revision String */
#define RBLE_CHAR_SW_REVISION_STRING			0x2A28u			/**< Software Revision String */
#define RBLE_CHAR_MANUF_NAME_STRING				0x2A29u			/**< Manufacturer Name String */
#define RBLE_CHAR_IEEE_CERTIF					0x2A2Au			/**< IEEE 11073-20601 Regulatory Certification Data List */
#define RBLE_CHAR_CURRENT_TIME					0x2A2Bu			/**< Current Time */
#define RBLE_CHAR_SCAN_REFRESH					0x2A31u			/**< Scan Refresh */
#define RBLE_CHAR_BOOT_KB_OUTPUT_REPORT			0x2A32u			/**< Boot Keyboard Output Report */
#define RBLE_CHAR_BOOT_MOUSE_INPUT_REPORT		0x2A33u			/**< Boot Mouse Input Report */
#define RBLE_CHAR_GLUCOSE_MEAS_CONTEXT			0x2A34u			/**< Glucose Measurement Context */
#define RBLE_CHAR_BLOOD_PRESSURE_MEAS			0x2A35u			/**< Blood Pressure Measurement */
#define RBLE_CHAR_INTERMEDIATE_BLOOD_PRESS		0x2A36u			/**< Intermediate Cuff Pressure */
#define RBLE_CHAR_HEART_RATE_MEAS				0x2A37u			/**< Heart Rate Measurement */
#define RBLE_CHAR_BODY_SENSOR_LOCATION			0x2A38u			/**< Body Sensor Location */
#define RBLE_CHAR_HEART_RATE_CTRL_POINT			0x2A39u			/**< Heart Rate Control Point */
#define RBLE_CHAR_ALERT_STATUS					0x2A3Fu			/**< Alert Status */
#define RBLE_CHAR_RINGER_CTRL_POINT				0x2A40u			/**< Ringer Control Point */
#define RBLE_CHAR_RINGER_SETTING				0x2A41u			/**< Ringer Setting */
#define RBLE_CHAR_AL_CATEGORY_ID_BIT_MASK		0x2A42u			/**< Alert Category ID Bit Mask */
#define RBLE_CHAR_AL_CATEGORY_ID				0x2A43u			/**< Alert Category ID */
#define RBLE_CHAR_AL_NOTIF_CTRL_POINT			0x2A44u			/**< Alert Notification Control Point */
#define RBLE_CHAR_UNREAD_ALERT_STATUS			0x2A45u			/**< Unread Alert Status */
#define RBLE_CHAR_NEW_ALERT						0x2A46u			/**< New Alert */
#define RBLE_CHAR_SUPP_NEW_AL_CATEGORY			0x2A47u			/**< Supported New Alert Category */
#define RBLE_CHAR_SUPP_UNREAD_AL_CATEGORY		0x2A48u			/**< Supported Unread Alert Category */
#define RBLE_CHAR_BLOOD_PRESSURE_FEAT			0x2A49u			/**< Blood Pressure Feature */
#define RBLE_CHAR_HID_INFO						0x2A4Au			/**< HID Information */
#define RBLE_CHAR_REPORT_MAP					0x2A4Bu			/**< Report Map */
#define RBLE_CHAR_HID_CTRL_POINT				0x2A4Cu			/**< HID Control Point */
#define RBLE_CHAR_REPORT						0x2A4Du			/**< Report */
#define RBLE_CHAR_PROTOCOL_MODE					0x2A4Eu			/**< Protocol Mode */
#define RBLE_CHAR_SCAN_INTERVAL_WINDOW			0x2A4Fu			/**< Scan Interval Window */
#define RBLE_CHAR_PNP_ID						0x2A50u			/**< PnP ID */
#define RBLE_CHAR_GLUCOSE_FEATURE				0x2A51u			/**< Glucose Feature */
#define RBLE_CHAR_RECORD_ACCESS_CTRL_POINT		0x2A52u			/**< Record Access Control Point */
#define RBLE_CHAR_RSC_MEAS						0x2A53u			/**< RSC Measurement */
#define RBLE_CHAR_RSC_FEATURE					0x2A54u			/**< RSC Feature */
#define RBLE_CHAR_SC_CNTL_POINT					0x2A55u			/**< SC Control Point */
#define RBLE_CHAR_CSC_MEAS						0x2A5Bu			/**< CSC Measurement */
#define RBLE_CHAR_CSC_FEATURE					0x2A5Cu			/**< CSC Feature */
#define RBLE_CHAR_SENSOR_LOCATION				0x2A5Du			/**< Sensor Location */
#define RBLE_CHAR_CYCLING_POWER_MEAS			0x2A63u			/**< Cycling Power Measurements */
#define RBLE_CHAR_CYCLING_POWER_VECTOR			0x2A64u			/**< Cycling Power Vector */
#define RBLE_CHAR_CYCLING_POWER_FEATURE			0x2A65u			/**< Cycling Power Feature */
#define RBLE_CHAR_CYCLING_POWER_CNTL_POINT		0x2A66u			/**< Cycling Power Control Point */
#define RBLE_CHAR_LOCATION_SPEED				0x2A67u			/**< Location and Speed */
#define RBLE_CHAR_NAVIGATION					0x2A68u			/**< Navigation */
#define RBLE_CHAR_POSITION_QUALITY				0x2A69u			/**< Position Quality */
#define RBLE_CHAR_LN_FEATURE					0x2A6Au			/**< LN Feature */
#define RBLE_CHAR_LN_CNTL_POINT					0x2A6Bu			/**< LN Control Point */

/*
 * Services
 *
 * Services are collections of characteristics and relationships to other services 
 * that encapsulate the behavior of part of a device.
 */

#define RBLE_SVC_GENERIC_ACCESS					0x1800u			/**< Generic Access */
#define RBLE_SVC_GENERIC_ATTRIBUTE				0x1801u			/**< Generic Attribute */
#define RBLE_SVC_IMMEDIATE_ALERT				0x1802u			/**< Immediate Alert */
#define RBLE_SVC_LINK_LOSS						0x1803u			/**< Link Loss */
#define RBLE_SVC_TX_POWER						0x1804u			/**< Tx Power */
#define RBLE_SVC_CURRENT_TIME					0x1805u			/**< Current Time Service */
#define RBLE_SVC_REFERENCE_TIME_UPDATE			0x1806u			/**< Reference Time Update Service */
#define RBLE_SVC_NEXT_DST_CHANGE				0x1807u			/**< Next DST Change Service */
#define RBLE_SVC_GLUCOSE						0x1808u			/**< Glucose */
#define RBLE_SVC_HEALTH_THERMOMETER				0x1809u			/**< Health Thermometer */
#define RBLE_SVC_DEVICE_INFORMATION				0x180Au			/**< Device Information */
#define RBLE_SVC_HEART_RATE						0x180Du			/**< Heart Rate */
#define RBLE_SVC_PHONE_ALERT_STATUS				0x180Eu			/**< Phone Alert Status Service */
#define RBLE_SVC_BATTERY_SERVICE				0x180Fu			/**< Battery Service */
#define RBLE_SVC_BLOOD_PRESSURE					0x1810u			/**< Blood Pressure */
#define RBLE_SVC_ALERT_NOTIFICATION				0x1811u			/**< Alert Notification Service */
#define RBLE_SVC_HUMAN_INTERFACE_DEVICE			0x1812u			/**< Human Interface Device */
#define RBLE_SVC_SCAN_PARAMETERS				0x1813u			/**< Scan Parameters */
#define RBLE_SVC_RUNNING_SPEED					0x1814u			/**< Running Speed and Cadence */
#define RBLE_SVC_CYCLING_SPEED					0x1816u			/**< Cycling Speed and Cadence */
#define RBLE_SVC_CYCLING_POWER					0x1818u			/**< Cycling Power */
#define RBLE_SVC_LOCATION_NAVIGATION			0x1819u			/**< Location and Navigation */

/**
 * @brief	request type
 */
enum RBLE_GATT_REQ_TYPE_enum {
	RBLE_GATT_DISC_ALL_SVC			= 0x00,		/**< Discover all services */
	RBLE_GATT_DISC_BY_UUID_SVC,					/**< Discover services by UUID */
	RBLE_GATT_DISC_INCLUDED_SVC,				/**< Discover included services */
	RBLE_GATT_DISC_ALL_CHAR,					/**< Discover all characteristics */
	RBLE_GATT_DISC_BY_UUID_CHAR,				/**< Discover characteristic by UUID */
	RBLE_GATT_DISC_DESC_CHAR,					/**< Discover characteristic descriptor */

	RBLE_GATT_READ_CHAR,						/**< Read characteristic */
	RBLE_GATT_READ_BY_UUID_CHAR,				/**< Read characteristic by UUID */
	RBLE_GATT_READ_LONG_CHAR,					/**< Read long characteristic */
	RBLE_GATT_READ_MULT_LONG_CHAR,				/**< Read multiple long characteristic */
	RBLE_GATT_READ_DESC,						/**< Read descriptor */
	RBLE_GATT_READ_LONG_DESC,					/**< Read long descriptor */

	RBLE_GATT_WRITE_NO_RESPONSE,				/**< Write no response */
	RBLE_GATT_WRITE_SIGNED,						/**< Write signed */
	RBLE_GATT_WRITE_CHAR,						/**< Write characteristic */
	RBLE_GATT_WRITE_LONG_CHAR,					/**< Write long characteristic */
	RBLE_GATT_WRITE_RELIABLE_CHAR,				/**< Write reliable characteristic */
	RBLE_GATT_WRITE_DESC,						/**< Write descriptor */
	RBLE_GATT_WRITE_LONG_DESC,					/**< Write long descriptor */
	RBLE_GATT_WRITE_CANCEL_CHAR					/**< Write cancel characteristic */
};

/*
 * expected response size - read multiple
 */
#define RBLE_GATT_LEN_UNDEF                     0xFF		    /**< variable length */

/**
 * @brief	 Characteristic Properties
 */
enum RBLE_GATT_CHAR_PROP_enum {
	RBLE_GATT_CHAR_PROP_BCAST		= 0x01,		/**< Permits broadcasts of the Characteristic Value */
	RBLE_GATT_CHAR_PROP_RD			= 0x02,		/**< Permits reads of the Characteristic Value */
	RBLE_GATT_CHAR_PROP_WR_NO_RESP	= 0x04,		/**< Permits writes of the Characteristic Value without response */
	RBLE_GATT_CHAR_PROP_WR			= 0x08,		/**< Permits writes of the Characteristic Value with response */
	RBLE_GATT_CHAR_PROP_NTF			= 0x10,		/**< Permits notifications of the Characteristic Value without acknowledgement */
	RBLE_GATT_CHAR_PROP_IND			= 0x20,		/**< Permits indications of a Characteristic Value with acknowledgement */
	RBLE_GATT_CHAR_PROP_AUTH		= 0x40,		/**< Permits signed writes to the Characteristic Value */
	RBLE_GATT_CHAR_PROP_EXT_PROP	= 0x80		/**< Additional characteristic properties */
};

/**
 * @brief	 Attribute permissions
 */
#define RBLE_GATT_PERM_NONE				0x0000	/**< No permission */
#define RBLE_GATT_PERM_RD				0x0001	/**< Read permission */
#define RBLE_GATT_PERM_RD_UNAUTH		0x0002	/**< Read permission (Unauthentication Required) */
#define RBLE_GATT_PERM_RD_AUTH			0x0004	/**< Read permission (Authentication Required) */
#define RBLE_GATT_PERM_RD_AUTZ			0x0008	/**< Read permission (Authorization Required) */
#define RBLE_GATT_PERM_WR				0x0010	/**< Write permission */
#define RBLE_GATT_PERM_WR_UNAUTH		0x0020	/**< Write permission (Unauthentication Required) */
#define RBLE_GATT_PERM_WR_AUTH			0x0040	/**< Write permission (Authentication Required) */
#define RBLE_GATT_PERM_WR_AUTZ			0x0080	/**< Write permission (Authorization Required) */
#define RBLE_GATT_PERM_NI				0x0100	/**< Notifications/Indications permission */
#define RBLE_GATT_PERM_NI_UNAUTH		0x0200	/**< Notifications/Indications permission (Unauthentication Required) */
#define RBLE_GATT_PERM_NI_AUTH			0x0400	/**< Notifications/Indications permission (Authentication Required) */
#define RBLE_GATT_PERM_NI_AUTZ			0x0800	/**< Notifications/Indications permission (Authorization Required) */
#define RBLE_GATT_PERM_EKS				0x1000	/**< Encryption key size Required */
#define RBLE_GATT_PERM_HIDE				0x2000	/**< Not expose */
#define RBLE_GATT_PERM_ENC				0x4000	/**< Encryption Required */
#define RBLE_GATT_PERM_NOTIFY_COMP_EN	0x8000	/**< Notification Complete Enable */

/**
 * @brief	ATT error code
 */
enum RBLE_ATT_ERR_CODE_enum {
	RBLE_ATT_ERR_NO_ERROR			= 0x00,		/* No error */
	RBLE_ATT_ERR_INVALID_HANDLE,				/* Handle is invalid */
	RBLE_ATT_ERR_READ_NOT_PERMITTED,			/* Read permission disabled */
	RBLE_ATT_ERR_WRITE_NOT_PERMITTED,			/* Write permission disabled */
	RBLE_ATT_ERR_INVALID_PDU,					/* Incorrect PDU */
	RBLE_ATT_ERR_INSUFF_AUTHEN,					/* Authentication privilege not enough */
	RBLE_ATT_ERR_REQUEST_NOT_SUPPORTED,			/* Request not supported or not understood */
	RBLE_ATT_ERR_INVALID_OFFSET,				/* Incorrect offset value */
	RBLE_ATT_ERR_INSUFF_AUTHOR,					/* Authorization privilege not enough */
	RBLE_ATT_ERR_PREPARE_QUEUE_FULL,			/* Capacity queue for reliable write reached */
	RBLE_ATT_ERR_ATTRIBUTE_NOT_FOUND,			/* Attribute requested not existing */
	RBLE_ATT_ERR_ATTRIBUTE_NOT_LONG,			/* Attribute requested not long */
	RBLE_ATT_ERR_INSUFF_ENC_KEY_SIZE,			/* Encryption size not sufficient */
	RBLE_ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN,		/* Invalid length of the attribute value */
	RBLE_ATT_ERR_UNLIKELY_ERR,					/* Operation not fit to condition */
	RBLE_ATT_ERR_INSUFF_ENC,					/* Attribute requires encryption before operation */
	RBLE_ATT_UNSUPP_GRP_TYPE,					/* Attribute grouping not supported */
	RBLE_ATT_INSUFF_RESOURCE,					/* Resources not sufficient to complete the request */
	RBLE_ATT_ERR_APP_ERROR	 			= 0x80,	/* Application Error */
	RBLE_ATT_ERR_IMPROPERLY_CONFIGURED	= 0xFD,	/* Client Characteristic Configuration Descriptor Improperly Configured */
	RBLE_ATT_ERR_ALREADY_IN_PROGRESS	= 0xFE,	/* Procedure Already in Progress */
	RBLE_ATT_ERR_OUT_OF_RANGE			= 0xFF	/* Out of Range */
};


/*================================== Service ==================================*/
/**
 * @brief	 Alert levels
 */
enum RBLE_SVC_ALT_LVL_enum {
	RBLE_SVC_ALERT_NONE				= 0x00,		/**< No alert */
	RBLE_SVC_ALERT_MILD,						/**< Mild aler */
	RBLE_SVC_ALERT_HIGH							/**< High alert */
};

/**
 * @brief	 PnP ID Characteristic vendor ID field values
 */
enum RBLE_SVC_PNP_VENDOR_ID_enum {
	RBLE_SVC_SIG_ASSIGNED_ID		= 0x01,		/**< Company Identifying value assigned by the Bluetooth SIG */
	RBLE_SVC_USB_ASSIGNED_ID					/**< Vendor ID Value assigned by USB-IF */
};

/**
 * @brief	 Characteristic Presentation Format Descriptor Namespace field values
 */
enum RBLE_SVC_PRESEN_NAMESPASE_enum {
	RBLE_SVC_NAMESPACE_SIG		= 0x01,		    /**< Defined by the Bluetooth SIG */
};

/**
 * @brief	 Security level of Service for profile enable
 */
enum RBLE_SVC_SEC_LVL_enum {
    RBLE_SVC_SEC_NONE               = 0x01,		/**< No security */
    RBLE_SVC_SEC_UNAUTH             = 0x02,		/**< Require unauthenticated pairing */
    RBLE_SVC_SEC_AUTH               = 0x04,		/**< Require authenticated pairing */
    RBLE_SVC_SEC_AUTZ               = 0x08,		/**< Require authorization */
    RBLE_SVC_SEC_ENC                = 0x10		/**< Require encryption */
};


/**
 * @brief	connection type for GATT client role
 */
enum RBLE_PRF_CON_enum {
    RBLE_PRF_CON_DISCOVERY	= 0x00,				/**< Discovery type connection */
    RBLE_PRF_CON_NORMAL,						/**< Normal type connection */
};


/**
 * @brief	Possible values for setting client configuration characteristics
 */
enum RBLE_PRF_CLIENT_CONFIG_enum {
    RBLE_PRF_STOP_NTFIND	= 0x00,
    RBLE_PRF_START_NTF,
    RBLE_PRF_START_IND,
};


/**
 * @brief	Possible values for setting server configuration characteristics
 */
enum RBLE_PRF_SERVER_CONFIG_enum {
    RBLE_PRF_STOP_BRD	= 0x00,
    RBLE_PRF_START_BRD
};


/**
 *  16-BIT FLOATING POINT DATA TYPE (SFLOAT-TYPE)
 *
 *        +----------+----------+
 *        | Exponent | Mantissa |
 * +------+----------+----------+
 * | size |  4 bits  | 12 bits  |
 * +------+----------+----------+
 */
typedef uint16_t		sfloat_t;

/*================================== Accuracy =================================*/
/**
 * @brief	 Constant clock accuracy
 */
enum RBLE_SAC_CLOCK_ACCURACY_enum {
    RBLE_SCA_500PPM,							/**< Clock accuracy at 500PPM */
    RBLE_SCA_250PPM,							/**< Clock accuracy at 250PPM */
    RBLE_SCA_150PPM,							/**< Clock accuracy at 150PPM */
    RBLE_SCA_100PPM,						    /**< Clock accuracy at 100PPM */
    RBLE_SCA_75PPM,								/**< Clock accuracy at 75PPM */
    RBLE_SCA_50PPM,								/**< Clock accuracy at 50PPM */
    RBLE_SCA_30PPM,								/**< Clock accuracy at 30PPM */
    RBLE_SCA_20PPM								/**< Clock accuracy at 20PPM */
};


/*================================== Security Manager  ========================*/
/**
 * @brief	Key Distribution Flags
 */
enum RBLE_SMP_KEY_DIST_FLAG_enum {
    RBLE_SMP_KDIST_NONE 	= 0x00,				/**< No Keys to distribute */
    RBLE_SMP_KDIST_ENCKEY	= 0x01,				/**< Encryption key in distribution */
    RBLE_SMP_KDIST_IDKEY	= 0x02,				/**< IRK (ID key)in distribution */
    RBLE_SMP_KDIST_SIGNKEY	= 0x04,				/**< CSRK(Signature key) in distribution */
};

/**
 * @brief	Security Properties for distributed keys(all have the issued STK's properties)
 */
enum RBLE_SMP_KSEC_enum {
    RBLE_SMP_KSEC_NONE 				= 0x00,		/**< No security properties */
    RBLE_SMP_KSEC_UNAUTH_NO_MITM,				/**< Unauthenticated no MITM */
    RBLE_SMP_KSEC_AUTH_MITM,					/**< Authenticated with MITM */
};


/**
 * @brief	Result Status for Check Bluetooth Address Request
 */
enum RBLE_SMP_CHK_BD_REQ_RSP_enum {
    RBLE_SMP_SEC_NONE        = 0x00,            /**< No link security status */
    RBLE_SMP_UNAUTHENTICATED = 0x01,            /**< Unauthenticated */
    RBLE_SMP_AUTHENTICATED   = 0x02,            /**< Authenticated */
    RBLE_SMP_AUTHORIZED      = 0x04,            /**< Authorized */
    RBLE_SMP_BONDED          = 0x08	            /**< Bonded */
};

#define RBLE_SMP_MAX_ENC_SIZE_LEN    0x10       /**< Maximum Encryption Key size */
#define RBLE_SMP_MIN_ENC_SIZE_LEN    0x07       /**< Minimum Encryption key size */

/*================================== Find Me  =================================*/


/*================================== Proximity ================================*/
/**
 * @brief	Link Loss or Immediate Alert code for setting alert through one message
 */
enum RBLE_ALT_CODE_enum {
    RBLE_PROXM_SET_LK_LOSS_ALERT	= 0x00,		/**< Code for LLS Alert Level Char. */
    RBLE_PROXM_SET_IMMDT_ALERT,					/**< Code for IAS Alert Level Char. */
};


/*================================== Health Thermometer =======================*/
/**
 * @brief	Temperature Measurement Flags field bit values
 */
enum RBLE_HTPT_FLAG_enum {
    RBLE_HTPT_FLAG_CELSIUS		= 0x00,
    RBLE_HTPT_FLAG_FAHRENHEIT	= 0x01,
    RBLE_HTPT_FLAG_TIME			= 0x02,
    RBLE_HTPT_FLAG_TYPE			= 0x04,
};

/**
 * @brief	Internal codes for reading a HTS or DIS characteristic with one single request
 */
enum RBLE_HTPC_RD_CHAR_CODE_enum {
    RBLE_HTPC_RD_HTS_TM_CFG = 0x00,             /**< Read HTS Temp. Measurement Client Cfg. Desc */
    RBLE_HTPC_RD_HTS_TT,                        /**< Read HTS Temperature Type */
    RBLE_HTPC_RD_HTS_IT_CFG,                    /**< Read HTS Intermd. Temp. Client Cfg. Desc */
    RBLE_HTPC_RD_HTS_MI,                        /**< Read HTS Measurement Interval */
    RBLE_HTPC_RD_HTS_MI_CFG,                    /**< Read HTS Measurement Intervfal Client Cfg. Desc */
    RBLE_HTPC_RD_HTS_VR,						/**< Read HTS Valid Range */

    RBLE_HTPC_RD_DIS_MANUF,						/**< Read DIS Manufacturer Name */
    RBLE_HTPC_RD_DIS_MODEL,						/**< Read DIS Model Number */
    RBLE_HTPC_RD_DIS_SERNB,						/**< Read DIS Serial number */
    RBLE_HTPC_RD_DIS_HWREV,						/**< Read DIS HW Revision */
    RBLE_HTPC_RD_DIS_FWREV,						/**< Read DIS Fw Revision */
    RBLE_HTPC_RD_DIS_SWREV,						/**< Read DIS SW Revision */
    RBLE_HTPC_RD_DIS_SYSID,						/**< Read DIS system ID */
    RBLE_HTPC_RD_DIS_IEEE,						/**< Read DIS IEEE Certification */
};

/**
 * @brief	HTPC codes for the 3 possible client configuration characteristic descriptors determination in HTS
 */
enum RBLE_HTPC_WR_CHAR_CODE_enum {
    RBLE_HTPC_TEMP_MEAS_CODE	= 0x01,
    RBLE_HTPC_INTERM_TEMP_CODE,
    RBLE_HTPC_MEAS_INTV_CODE,
};


/*================================== Blood Pressure ===========================*/
/**
 * @brief	Internal codes for reading a BLS or DIS characteristic with one single request
 */
enum RBLE_BLPC_RD_CHAR_CODE_enum {
    RBLE_BLPC_RD_BLS_BM_CFG		= 0x00,			/**< Read BLS Blood Pressure Measurement Client Cfg. Desc */
    RBLE_BLPC_RD_BLS_IC_CFG,					/**< Read BLS Intermediate Cuff Pressure Client Cfg. Desc */
    RBLE_BLPC_RD_BLS_BF,						/**< Read BLS Blood Pressure Feature */

    RBLE_BLPC_RD_DIS_MANUF,						/**< Read DIS Manufacturer Name */
    RBLE_BLPC_RD_DIS_MODEL,						/**< Read DIS Model Number */
    RBLE_BLPC_RD_DIS_SERNB,						/**< Read DIS Serial number */
    RBLE_BLPC_RD_DIS_HWREV,						/**< Read DIS HW Revision */
    RBLE_BLPC_RD_DIS_FWREV,						/**< Read DIS Fw Revision */
    RBLE_BLPC_RD_DIS_SWREV,						/**< Read DIS SW Revision */
    RBLE_BLPC_RD_DIS_SYSID,						/**< Read DIS system ID */
    RBLE_BLPC_RD_DIS_IEEE,						/**< Read DIS IEEE Certification */
};

/**
 * @brief	BLPC codes for the 2 possible client configuration characteristic descriptors determination in BLS
 */
enum RBLE_BLPC_WR_CHAR_CODE_enum {
    RBLE_BLPC_BLDPRS_MEAS_CODE		= 0x01,
    RBLE_BLPC_INTERM_CUFPRS_CODE,
};


/*================================== HID over GATT ===========================*/
/**
 * @brief	device type
 */
enum RBLE_HGHD_DEVICE_TYPE_enum {
    RBLE_HGHD_HID_DEVICE		    = 0x01,		/**< HID Device type */
    RBLE_HGHD_BOOT_KEYBOARD,                    /**< Boot Keyboard type */
    RBLE_HGHD_BOOT_MOUSE                        /**< Boot Mouse type */
};

/**
 * @brief	HGHD codes for the 4 possible client configuration characteristic descriptors determination in HIDS
 */
enum RBLE_HGHD_WR_CHAR_CODE_enum {
    RBLE_HGHD_REPORT_INPUT_CODE		= 0x01,
    RBLE_HGHD_KB_REPORT_CODE,
    RBLE_HGHD_MO_REPORT_CODE,
    RBLE_HGHD_BATTERY_LEVEL_CODE
};

/**
 * @brief	Protocol Mode Characteristic values
 */
enum RBLE_HGHD_PROTOCOL_MODE_enum {
    RBLE_HGHD_PROTOCOL_MODE_BOOT    = 0x00,		/**< Boot Protocol mode */
    RBLE_HGHD_PROTOCOL_MODE_REPORT              /**< Report Protocol mode */
};

/**
 * @brief	Report Reference Descriptor values
 */
enum RBLE_HGHD_REPORT_REFERENCE_enum {
    RBLE_HGHD_INPUT_REPORT          = 0x01,		/**< Input Report type */
    RBLE_HGHD_OUTPUT_REPORT,                    /**< Output Report type */
    RBLE_HGHD_FEATURE_REPORT                    /**< Feature Report type */
};

/**
 * @brief	HID Information Characteristic Flags field bit values
 */
enum RBLE_HGHD_HID_INFORMATION_enum {
    RBLE_HGHD_FLAG_REMOTE_WAKE      = 0x01,		/**< Flag of Remote Wake */
    RBLE_HGHD_FLAG_NORMALLY_CONNECTABLE         /**< Flag of Normally Connectable */
};

/**
 * @brief	HID Control Point Characteristic values
 */
enum RBLE_HGHD_CONTROL_POINT_enum {
    RBLE_HGHD_CTRL_POINT_SUSPEND    = 0x00,     /**< Suspend */
    RBLE_HGHD_POINT_EXIT_SUSPEND                /**< Exit Suspend */
};

/**
 * @brief   HGBH Internal codes for reading a HIDS, DIS or BAS characteristic with one single request
 */
enum RBLE_HGBH_RD_CHAR_CODE_enum {
    RBLE_HGBH_RD_HIDS_PM            = 0x00,     /**< Read HIDS Protcol Mode Characteristic */
    RBLE_HGBH_RD_HIDS_KI,                       /**< Read HIDS Boot Keyboard Input Report Characteristic */
    RBLE_HGBH_RD_HIDS_KI_CFG,                   /**< Read HIDS Boot Keyboard Input Report Client Cfg. Desc */
    RBLE_HGBH_RD_HIDS_KO,                       /**< Read HIDS Boot Keyboard Output Report Characteristic */
    RBLE_HGBH_RD_HIDS_MI,                       /**< Read HIDS Boot Mouse Input Report Characteristic */
    RBLE_HGBH_RD_HIDS_MI_CFG,                   /**< Read HIDS Boot Mouse Input Report Client Cfg. Desc */

    RBLE_HGBH_RD_DIS_PNPID,                     /**< Read DIS PnP ID Characteristic */

    RBLE_HGBH_RD_BAS_BL                         /**< Read BAS Battery Level Characteristic */
};

/**
 * @brief   HGRH Internal codes for reading a HIDS, DIS or BAS characteristic with one single request
 */
enum RBLE_HGRH_RD_CHAR_CODE_enum {
    RBLE_HGRH_RD_HIDS_PM            = 0x00,     /**< Read HID Service Protcol Mode Characteristic */
    RBLE_HGRH_RD_HIDS_RI,                       /**< Read HID Service Report(Input) Characteristic */
    RBLE_HGRH_RD_HIDS_RI_CFG,                   /**< Read HID Service Report(Input) Client Cfg. Desc */
    RBLE_HGRH_RD_HIDS_RI_REF,                   /**< Read HID Service Report(Input) Report Reference Desc */
    RBLE_HGRH_RD_HIDS_RO,                       /**< Read HID Service Report(Output) Characteristic */
    RBLE_HGRH_RD_HIDS_RO_REF,                   /**< Read HID Service Report(Output) Report Reference Desc */
    RBLE_HGRH_RD_HIDS_RF,                       /**< Read HID Service Report(Feature) Characteristic */
    RBLE_HGRH_RD_HIDS_RF_REF,                   /**< Read HID Service Report(Feature) Report Reference Desc */
    RBLE_HGRH_RD_HIDS_RM,                       /**< Read HID Service Report MAP Characteristic */
    RBLE_HGRH_RD_HIDS_ER_REF,                   /**< Read HID Service Report MAP Rxternal Report Reference Desc */
    RBLE_HGRH_RD_HIDS_HI,                       /**< Read HID Service HID Information Characteristic */

    RBLE_HGRH_RD_DIS_PNPID,                     /**< Read Device Information Service PnP ID Characteristic */

    RBLE_HGRH_RD_BAS_BL,                        /**< Read Battery Service Battery Level Characteristic */
    RBLE_HGRH_RD_BAS_BL_CFG,                    /**< Read Battery Service Battery Level Client Cfg. Desc */
    RBLE_HGRH_RD_BAS_BL_REF,                    /**< Read Battery Service Battery Level Report Reference Desc */
};


/*================================== Scan Parameters ===========================*/
/**
 * @brief	Scan Refresh Characteristic values
 */
enum RBLE_SCANS_REFRESH_enum {
    RBLE_SCANS_MODE_REFRESH_REQ		        = 0x00,
    RBLE_SCANS_MODE_REFRESH_NON
};

/*================================== Heart Rate ===========================*/
/**
 * @brief	rr interval max num
 */
 #define RBLE_HRP_RR_INTERVAL_MAX 0x09

/**
 * @brief	Internal codes for reading a HRS or DIS characteristic with one single request
 */
enum RBLE_HRPC_RD_CHAR_CODE_enum {
    RBLE_HRPC_RD_HRS_HM_CFG		= 0x00,			/**< Read HRS Heart Rate Measurement Client Cfg. Desc */
    RBLE_HRPC_RD_HRS_BSL,						/**< Read HRS Body Sensor Location */

    RBLE_HRPC_RD_DIS_MANUF,						/**< Read DIS Manufacturer Name */
    RBLE_HRPC_RD_DIS_MODEL,						/**< Read DIS Model Number */
    RBLE_HRPC_RD_DIS_SERNB,						/**< Read DIS Serial number */
    RBLE_HRPC_RD_DIS_HWREV,						/**< Read DIS HW Revision */
    RBLE_HRPC_RD_DIS_FWREV,						/**< Read DIS Fw Revision */
    RBLE_HRPC_RD_DIS_SWREV,						/**< Read DIS SW Revision */
    RBLE_HRPC_RD_DIS_SYSID,						/**< Read DIS system ID */
    RBLE_HRPC_RD_DIS_IEEE,						/**< Read DIS IEEE Certification */
};

/**
 * @brief	Internal codes for set Control Point
 */
enum RBLE_HRPC_WR_CP_CHAR_CODE_enum {
    RBLE_HRPC_HRTRATE_CTRL_POINT_RESET		= 0x01,			/**< Reset Energy */
};

/*============================ Cycling Speed and Cadence =======================*/
/**
 * @brief	max num of supported sensor location
 */
#define RBLE_CSCP_SENSORE_LOCATION_MAX     17

/**
 * @brief	Internal codes for reading a CSCS or DIS characteristic with one single request
 */
enum RBLE_CSCPC_RD_CHAR_CODE_enum {
    RBLE_CSCPC_RD_CSCS_CM_CFG = 0x00,
    RBLE_CSCPC_RD_CSCS_SCCP_CFG,
    RBLE_CSCPC_RD_CSCS_CSC_FEATURE,
    RBLE_CSCPC_RD_CSCS_SL,
    
    RBLE_CSCPC_RD_DIS_MANUF,
    RBLE_CSCPC_RD_DIS_MODEL,
    RBLE_CSCPC_RD_DIS_SERNB,
    RBLE_CSCPC_RD_DIS_HWREV,
    RBLE_CSCPC_RD_DIS_FWREV,
    RBLE_CSCPC_RD_DIS_SWREV,
    RBLE_CSCPC_RD_DIS_SYSID,
    RBLE_CSCPC_RD_DIS_IEEE
};

/**
 * @brief	Possible values for setting client configuration characteristics
 */
enum RBLE_CSCPC_WR_CHAR_CODE_enum {
    RBLE_CSCPC_CYCSPD_MEAS_CODE = 0x01,
    RBLE_CSCPC_SC_CONTROL_POINT_CODE,
};

/**
 * @brief	Possible values for setting location sensor
 */
enum RBLE_CSCPC_SENSOR_LOCATION_enum {
    RBLE_CSCPC_SENSOR_OTHER = 0x00,
    RBLE_CSCPC_SENSOR_TOP_OF_SHOE,
    RBLE_CSCPC_SENSOR_IN_SHOE,
    RBLE_CSCPC_SENSOR_HIP,
    RBLE_CSCPC_SENSOR_FRONT_WHEEL,
    RBLE_CSCPC_SENSOR_LEFT_CRANK,
    RBLE_CSCPC_SENSOR_RIGHT_CRANK,
    RBLE_CSCPC_SENSOR_LEFT_PEDAL,
    RBLE_CSCPC_SENSOR_RIGHT_PEDAL,
    RBLE_CSCPC_SENSOR_FRONT_HUB,
    RBLE_CSCPC_SENSOR_REAR_DROPOUT,
    RBLE_CSCPC_SENSOR_CHAINSTAY,
    RBLE_CSCPC_SENSOR_REAR_WHEEL,
    RBLE_CSCPC_SENSOR_REAR_HUB,
    RBLE_CSCPC_SENSOR_CHEST
};

/**
 * @brief	Possible values for setting operation code
 */
enum RBLE_CSCP_SCCP_OP_CODE_enum {
    RBLE_CSCP_OP_SET_CUMULATIVE_CODE = 0x01,
    RBLE_CSCP_OP_START_SC_CODE,
    RBLE_CSCP_OP_UPDATE_SL_CODE,
    RBLE_CSCP_OP_REQ_SUPPORTED_SL_CODE,
    RBLE_CSCP_OP_RESPONSE_CODE = 0x10
};

/**
 * @brief	Possible values for setting operation response code
 */
enum RBLE_CSCP_SCCP_RES_CODE_enum {
    RBLE_CSCP_RES_SUCCESS_CODE = 0x01,
    RBLE_CSCP_RES_NOT_SUPPORTED_CODE,
    RBLE_CSCP_RES_INVALID_PARAM_CODE,
    RBLE_CSCP_RES_OP_FAILED_CODE,
};


/*============================ Glucose =======================*/
/**
 * @brief	Internal codes for reading a GLS or DIS characteristic with one single request
 */
enum RBLE_GLPC_RD_CHAR_CODE_enum {
    RBLE_GLPC_RD_MEASUREMENT_CFG = 0x00,
    RBLE_GLPC_RD_MEASUREMENT_CONTEXT_CFG,
    RBLE_GLPC_RD_RA_CP_CFG,
    RBLE_GLPC_RD_FEATURE,

    RBLE_GLPC_RD_DIS_MANUF,
    RBLE_GLPC_RD_DIS_MODEL,
    RBLE_GLPC_RD_DIS_SERNB,
    RBLE_GLPC_RD_DIS_HWREV,
    RBLE_GLPC_RD_DIS_FWREV,
    RBLE_GLPC_RD_DIS_SWREV,
    RBLE_GLPC_RD_DIS_SYSID,
    RBLE_GLPC_RD_DIS_IEEE
};

/**
 * @brief	Possible values for setting client configuration characteristics
 */
enum RBLE_GLPC_WR_CHAR_CODE_enum {
    RBLE_GLPC_WR_MEASUREMENT_CODE = 0x01,
    RBLE_GLPC_WR_MEASUREMENT_CONTEXT_CODE,
    RBLE_GLPC_WR_RA_CONTROL_POINT_CODE
};

/**
 * @brief	Possible values for setting Glucose Type
 */
enum RBLE_GLPC_TYPE_enum {
    RBLE_GLP_TYPE_CAPILLARY_WHOLE_BLOOD = 0x01,
    RBLE_GLP_TYPE_CAPILLARY_PLASMA,
    RBLE_GLP_TYPE_VENOUS_WHOLE_BLOOD,
    RBLE_GLP_TYPE_VENOUS_PLASMA,
    RBLE_GLP_TYPE_ARTERIAL_WHOLE_BLOOD,
    RBLE_GLP_TYPE_ARTERIAL_PLASMA,
    RBLE_GLP_TYPE_UNDETERMINED_WHOLE_BLOOD,
    RBLE_GLP_TYPE_UNDETERMINED_PLASMA,
    RBLE_GLP_TYPE_ISF,
    RBLE_GLP_TYPE_CONTROL_SOLUTION
};

/**
 * @brief	Possible values for setting sample location
 */
enum RBLE_GLP_SAMPLELOC_enum {
    RBLE_GLP_SAMPLELOC_FINGER = 0x01,
    RBLE_GLP_SAMPLELOC_AST,
    RBLE_GLP_SAMPLELOC_EARLOBE,
    RBLE_GLP_SAMPLELOC_CONTROL_SOLUTION,
    RBLE_GLP_SAMPLELOC_NOT_AVALABLE
};

/**
 * @brief	Possible values for setting status annuniciation
 */
enum RBLE_GLP_SENSORSTATUS_enum {
    RBLE_GLP_SENSORSTATUS_DEVICE_BATTERY_LOW	= 0x0001,
    RBLE_GLP_SENSORSTATUS_MALFUNCTION_FAULTING	= 0x0002,
    RBLE_GLP_SENSORSTATUS_INSUFFICIENT			= 0x0004,
    RBLE_GLP_SENSORSTATUS_STRIP_INSERTION_ERROR	= 0x0008,
    RBLE_GLP_SENSORSTATUS_STRIP_TYPE_INCORRECT	= 0x0010,
    RBLE_GLP_SENSORSTATUS_RESULT_HIGH			= 0x0020,
    RBLE_GLP_SENSORSTATUS_RESULT_LOW			= 0x0040,
    RBLE_GLP_SENSORSTATUS_TEMPERATURE_HIGH		= 0x0080,
    RBLE_GLP_SENSORSTATUS_TEMPERATURE_LOW		= 0x0100,
    RBLE_GLP_SENSORSTATUS_READ_INTERRUPTED		= 0x0200,
    RBLE_GLP_SENSORSTATUS_GENERAL_DEVICE_FAULT	= 0x0400,
    RBLE_GLP_SENSORSTATUS_TIME_FAULT			= 0x0800
};

/**
 * @brief	Possible values for setting status glucose carbohydrate ID
 */
enum RBLE_GLP_CARBOHYDRATEID_enum {
    RBLE_GLP_CARBOHYDRATEID_BREAKFAST = 0x01,
    RBLE_GLP_CARBOHYDRATEID_LUNCH,
    RBLE_GLP_CARBOHYDRATEID_DINNER,
    RBLE_GLP_CARBOHYDRATEID_SNACK,
    RBLE_GLP_CARBOHYDRATEID_DRINK,
    RBLE_GLP_CARBOHYDRATEID_SUPPER,
    RBLE_GLP_CARBOHYDRATEID_BRUNCH
};

/**
 * @brief	Possible values for setting status glucose meal
 */
enum RBLE_GLP_MEAL_enum {
    RBLE_GLP_MEAL_PREPRANDIAL = 0x01,
    RBLE_GLP_MEAL_POSTPRANDIAL,
    RBLE_GLP_MEAL_FASTING,
    RBLE_GLP_MEAL_CASUAL,
    RBLE_GLP_MEAL_BEDTIME
};

/**
 * @brief	Possible values for setting status glucose tester
 */
enum RBLE_GLP_TESTER_enum {
    RBLE_GLP_TESTER_SELF = 0x01,
    RBLE_GLP_TESTER_HEALTH_CARE_PRO,
    RBLE_GLP_TESTER_LAB_TEST,
    RBLE_GLP_TESTER_NOT_AVAILABLE
};

/**
 * @brief	Possible values for setting status glucose health
 */
enum RBLE_GLP_HEALTH_enum {
    RBLE_GLP_HEALTH_MINOR_ISSUE = 0x01,
    RBLE_GLP_HEALTH_MAJOR_ISSUE,
    RBLE_GLP_HEALTH_DURING_MENSES,
    RBLE_GLP_HEALTH_UNDER_STRESS,
    RBLE_GLP_HEALTH_NO_ISSUE,
    RBLE_GLP_HEALTH_NOT_AVAILABLE
};

/**
 * @brief	Possible values for setting status glucose medication ID
 */
enum RBLE_GLP_MEDICATIONID_enum {
    RBLE_GLP_MEDICATIONID_RAPID_INSULIN = 0x01,
    RBLE_GLP_MEDICATIONID_SHORT_INSULIN,
    RBLE_GLP_MEDICATIONID_INTERMEDIATE_INSULIN,
    RBLE_GLP_MEDICATIONID_LONG_INSULIN,
    RBLE_GLP_MEDICATIONID_PREMIXED_INSULIN
};

/**
 * @brief	Possible values for setting status glucose operation code
 */
enum RBLE_GLP_OPCODE_enum {
    RBLE_GLP_OPCODE_REPORT_RECORDS = 0x01,
    RBLE_GLP_OPCODE_DELETE_RECORDS,
    RBLE_GLP_OPCODE_ABORT_OPERATION,
    RBLE_GLP_OPCODE_REPORT_RECORDS_NUMBER,
    RBLE_GLP_OPCODE_NUMBER_RECORDS_RESPONSE,
    RBLE_GLP_OPCODE_RESPONSE_CODE
};

/**
 * @brief	Possible values for setting status glucose operater
 */
enum RBLE_GLP_OPERATOR_enum {
    RBLE_GLP_OPERATOR_NULL = 0x00,
    RBLE_GLP_OPERATOR_ALL_RECORDS,
    RBLE_GLP_OPERATOR_LESS,
    RBLE_GLP_OPERATOR_GREATER,
    RBLE_GLP_OPERATOR_WITHIN_RANGE,
    RBLE_GLP_OPERATOR_FIRST_RECORD,
    RBLE_GLP_OPERATOR_LAST_RECORD,
    RBLE_GLP_OPERATOR_RESERVED
};

/**
 * @brief	Possible values for setting operation response code
 */
enum RBLE_GLP_OPERAND_RESPONSE_CODE_enum {
    RBLE_GLP_OPERAND_SUCCESS = 0x01,
    RBLE_GLP_OPERAND_OPCODE_NOT_SUPPORTED,
    RBLE_GLP_OPERAND_INVALID_OPERATOR,
    RBLE_GLP_OPERAND_OPERATOR_NOT_SUPPORTED,
    RBLE_GLP_OPERAND_INVALID_OPERAND,
    RBLE_GLP_OPERAND_NO_RECORD,
    RBLE_GLP_OPERAND_ABORT_UNSUCCESSFUL,
    RBLE_GLP_OPERAND_NOT_COMPLETED,
    RBLE_GLP_OPERAND_NOT_SUPPORTED
};

/*================================== Cycling Power ===========================*/
#define RBLE_CPP_MAGNITUDE_MAX			9
#define RBLE_CPP_SENSORE_LOCATION_MAX 	17

/**
 * @brief	CPPC codes for the 5 possible client/server configuration characteristic descriptors determination in CPS/BAS
 */
 enum RBLE_CPPC_WR_CHAR_CODE_enum {
    RBLE_CPPC_CYCPWR_MEAS_CODE			= 0x01,
    RBLE_CPPC_CYCPWR_MEAS_BRD_CODE,
    RBLE_CPPC_CYCPWR_VCTR_CODE,
    RBLE_CPPC_CYCPWR_CONTROL_POINT_CODE,
    RBLE_CPPC_BATTERY_LEVEL_CODE
};

/**
 * @brief	Internal codes for reading a CPS or DIS or BAS characteristic with one single request
 */
enum RBLE_CPPC_RD_CHAR_CODE_enum {
    RBLE_CPPC_RD_CPS_CM_CFG			= 0x00,		/**< Read CPS Cycling Power Measurement Client Cfg. Desc */
    RBLE_CPPC_RD_CPS_CM_BRD_CFG,				/**< Read CPS Cycling Power Measurement Server Cfg. Desc */
    RBLE_CPPC_RD_CPS_CV_CFG,					/**< Read CPS Cycling Power Vector Clinet Cfg. Desc */
    RBLE_CPPC_RD_CPS_CPCP_CFG,					/**< Read CPS Cycling Power Control Point Clinet Cfg. Desc */
    RBLE_CPPC_RD_CPS_CP_FEATURE,				/**< Read CPS Cycling Power Feature */
    RBLE_CPPC_RD_CPS_SL,						/**< Read CPS Cycling Power Sensor Location */
    
    RBLE_CPPC_RD_DIS_MANUF,						/**< Read DIS Manufacturer Name */
    RBLE_CPPC_RD_DIS_MODEL,						/**< Read DIS Model Number */
    RBLE_CPPC_RD_DIS_SERNB,						/**< Read DIS Serial number */
    RBLE_CPPC_RD_DIS_HWREV,						/**< Read DIS HW Revision */
    RBLE_CPPC_RD_DIS_FWREV,						/**< Read DIS Fw Revision */
    RBLE_CPPC_RD_DIS_SWREV,						/**< Read DIS SW Revision */
    RBLE_CPPC_RD_DIS_SYSID,						/**< Read DIS system ID */
    RBLE_CPPC_RD_DIS_IEEE,						/**< Read DIS IEEE Certification */
    
    RBLE_CPPC_RD_BAS_BL,						/**< Read Battery Service Battery Level Characteristic */
    RBLE_CPPC_RD_BAS_BL_CFG						/**< Read Battery Service Battery Level Client Cfg. Desc */
};

/**
 * @brief	 Sensor Location
 */
enum RBLE_CPPC_SENSOR_LOCATION_enum {
    RBLE_CPPC_SENSOR_OTHER = 0x00,
    RBLE_CPPC_SENSOR_TOP_OF_SHOE,
    RBLE_CPPC_SENSOR_IN_SHOE,
    RBLE_CPPC_SENSOR_HIP,
    RBLE_CPPC_SENSOR_FRONT_WHEEL,
    RBLE_CPPC_SENSOR_LEFT_CRANK,
    RBLE_CPPC_SENSOR_RIGHT_CRANK,
    RBLE_CPPC_SENSOR_LEFT_PEDAL,
    RBLE_CPPC_SENSOR_RIGHT_PEDAL,
    RBLE_CPPC_SENSOR_FRONT_HUB,
    RBLE_CPPC_SENSOR_REAR_DROPOUT,
    RBLE_CPPC_SENSOR_CHAINSTAY,
    RBLE_CPPC_SENSOR_REAR_WHEEL,
    RBLE_CPPC_SENSOR_REAR_HUB,
    RBLE_CPPC_SENSOR_CHEST
};

/**
 * @brief	 Control Point Operation Code
 */
enum RBLE_CPP_CP_OP_CODE_enum {
    RBLE_CPP_OP_SET_CUMULATIVE_CODE = 0x01,			/**< Set Cumulative Value Code */
    RBLE_CPP_OP_UPDATE_SL_CODE,						/**< Update Sensor Location */
    RBLE_CPP_OP_REQ_SUPPORTED_SL_CODE,				/**< Request Supported Sensor Locations */
    RBLE_CPP_OP_SET_CRANK_LEN_CODE,					/**< Set Crank Length */
    RBLE_CPP_OP_REQ_CRANK_LEN_CODE,					/**< Request Crank Length */
    RBLE_CPP_OP_SET_CHAIN_LEN_CODE,					/**< Set Chain Length */
    RBLE_CPP_OP_REQ_CHAIN_LEN_CODE,					/**< Request Chain Length */
    RBLE_CPP_OP_SET_CHAIN_WEI_CODE,					/**< Set Chain Weight */
    RBLE_CPP_OP_REQ_CHAIN_WEI_CODE,					/**< Request Chain Weight */
    RBLE_CPP_OP_SET_SPAN_LEN_CODE,					/**< Set Span Length */
    RBLE_CPP_OP_REQ_SPAN_LEN_CODE,					/**< Request Span Length */
    RBLE_CPP_OP_START_OFFSET_COMPENSATION_CODE,		/**< Start Offset Compensation */
    RBLE_CPP_OP_MASK_CP_MEAS_CONTENT_CODE,			/**< Mask Cycling Power Measurement Characteristic Content */
    RBLE_CPP_OP_REQ_SAMPL_RATE_CODE,				/**< Request Sampling Rate */
    RBLE_CPP_OP_REQ_FACTORY_CALIB_DATE_CODE,		/**< Request Factory Calibration Date */
    RBLE_CPP_OP_RESPONSE_CODE = 0x20				/**< Response Code */
};

/**
 * @brief	 Control Point Response Code
 */
enum RBLE_CPP_CP_RES_CODE_enum {
    RBLE_CPP_RES_SUCCESS_CODE = 0x01,				/**< Success */
    RBLE_CPP_RES_NOT_SUPPORTED_CODE,				/**< Response if unsupported Op Code is receivved */
    RBLE_CPP_RES_INVALID_PARAM_CODE,				/**< Response if Parameter received dosen't meet the requirements of the service */
    RBLE_CPP_RES_OP_FAILED_CODE						/**< Response if the requested procedure failed */
};

/* ------------------------- Time Profile-------------------------------*/
/**
 * @brief	Internal codes for reading a CTS or NDCS or RTUS characteristic with one single request
 */
enum RBLE_TIPC_RD_CHAR_CODE_enum {
	RBLE_TIPC_RD_CTS_CRNT_TIME = 0x00,
	RBLE_TIPC_RD_CTS_CRNT_TIME_CFG,
	RBLE_TIPC_RD_CTS_LOCAL_TIME,
	RBLE_TIPC_RD_CTS_REF_TIME,
	RBLE_TIPC_RD_NDCS_TIME_DST,
	RBLE_TIPC_RD_RTUS_UPDATE_STATUS
};

/**
 * @brief	Internal codes for setting a CTS or NDCS or RTUS characteristic with one single request
 */
enum RBLE_TIPS_WR_CHAR_CODE_enum {
	RBLE_TIPS_WR_CTS_CRNT_TIME = 0x00,
	RBLE_TIPS_WR_CTS_LOCAL_TIME,
	RBLE_TIPS_WR_CTS_REF_TIME,
	RBLE_TIPS_WR_NDCS_TIME_DST,
	RBLE_TIPS_WR_RTUS_UPDATE_STATUS
};

/**
 * @brief	 day of week
 */
enum RBLE_TIP_DAY_OF_WEEK_enum {
	RBLE_TIP_TIME_WEEK_UNKNOWN = 0x00,
	RBLE_TIP_TIME_MONDAY,
	RBLE_TIP_TIME_TUESDAY,
	RBLE_TIP_TIME_WEDNESDAY,
	RBLE_TIP_TIME_THURSDAY,
	RBLE_TIP_TIME_FRIDAY,
	RBLE_TIP_TIME_SATURDAY,
	RBLE_TIP_TIME_SUNDAY
};

/**
 * @brief	DST offset
 */
enum RBLE_TIP_DST_OFFSET_enum {
	RBLE_TIP_DST_STANDARD = 0x00,
	RBLE_TIP_DST_HALF_AN_HOUR_DAYLIGHT = 0x02,
	RBLE_TIP_DST_DAYLIGHT = 0x04,
	RBLE_TIP_DST_DOUBLE_DAYLIGHT = 0x08,
	RBLE_TIP_DST_UNKNOWN = 0xff
};

/**
 * @brief	time source
 */
enum RBLE_TIP_TIME_SOURCE_enum {
	RBLE_TIP_TIME_SOURCE_UNKNOWN = 0x00,
	RBLE_TIP_TIME_SOURCE_NET_TIME_PRTCL,
	RBLE_TIP_TIME_SOURCE_GPS,
	RBLE_TIP_TIME_SOURCE_RADIO,
	RBLE_TIP_TIME_SOURCE_MANUAL,
	RBLE_TIP_TIME_SOURCE_ATOMIC,
	RBLE_TIP_TIME_SOURCE_CELLULAR_NET
};

/**
 * @brief	 Time Update Control Point Operation Code
 */
enum RBLE_TIP_TIME_UPDATE_CP_enum {
	RBLE_TIP_CP_REF_UPDATE = 0x01,
	RBLE_TIP_CP_REF_UPDATE_CANCEL
};

/**
 * @brief	 Time Update status
 */
enum RBLE_TIP_TIME_UPDATE_STATUS_enum {
	RBLE_TIP_TIME_UPDATE_IDLE = 0x01,
	RBLE_TIP_TIME_UPDATE_PENDING
};

/**
 * @brief	 Time Update result code
 */
enum RBLE_TIP_TIME_UPDATE_RESULT_enum {
	RBLE_TIP_TIME_UPDATE_SUCCESS = 0x00,
	RBLE_TIP_TIME_UPDATE_CANCELED,
	RBLE_TIP_TIME_UPDATE_NO_CONNECT,
	RBLE_TIP_TIME_UPDATE_REF_ERROR,
	RBLE_TIP_TIME_UPDATE_TIMEOUT,
	RBLE_TIP_TIME_UPDATE_NOT_ATTEMPTED
};

/* ------------------------- Alert Notification Profile-------------------------------*/
#define RBLE_ANP_ALERT_TEXT_MAX 	18

/**
 * @brief	Internal codes for reading a ANS characteristic with one single request
 */
enum RBLE_ANPC_RD_CHAR_CODE_enum {
	RBLE_ANPC_RD_ANS_SUPP_NEW_ALERT = 0x00,
	RBLE_ANPC_RD_ANS_NEW_ALERT_CFG,
	RBLE_ANPC_RD_ANS_SUPP_UNREAD_ALERT,
	RBLE_ANPC_RD_ANS_UNREAD_ALERT_CFG
};

/**
 * @brief	Internal codes for writing a ANS characteristic client configuration
 */
enum RBLE_ANP_WR_CHAR_CODE_enum {
	RBLE_ANP_NEW_ALERT_CODE = 0x00,
	RBLE_ANP_UNREAD_ALERT_CODE
};

/**
 * @brief	 Category ID
 */
enum RBLE_ANP_CATEGORY_ID_enum {
	RBLE_ANP_CATEGORY_SIMPLE_ALERT = 0x00,
	RBLE_ANP_CATEGORY_EMAIL,
	RBLE_ANP_CATEGORY_NEWS,
	RBLE_ANP_CATEGORY_CALL,
	RBLE_ANP_CATEGORY_MISSED_CALL,
	RBLE_ANP_CATEGORY_SMS_MMS,
	RBLE_ANP_CATEGORY_VOICE_MAIL,
	RBLE_ANP_CATEGORY_SCHEDULE,
	RBLE_ANP_CATEGORY_HIGH_PRIORITY_ALERT,
	RBLE_ANP_CATEGORY_INSTANT_MESSAGE,
	RBLE_ANP_CATEGORY_ALL = 0xff
};

/**
 * @brief	Command Id
 */
enum RBLE_ANP_COMMAND_ID_enum {
	RBLE_ANP_NEW_ALERT_ENABLE = 0x00,
	RBLE_ANP_UNREAD_ALERT_ENABLE,
	RBLE_ANP_NEW_ALERT_DISABLE,
	RBLE_ANP_UNREAD_ALERT_DISABLE,
	RBLE_ANP_NEW_ALERT_NTF_REQ,
	RBLE_ANP_UNREAD_ALERT_NTF_REQ
};

/* ------------------------- Location and Navigation Profile-------------------------------*/
#define RBLE_LNP_SENSOR_ROUTE_NAME_MAX 	17

/**
 * @brief	Internal codes for reading LNS or DIS or BAS characteristic with one single request
 */
enum RBLE_LNPC_RD_CHAR_CODE_enum {
	RBLE_LNPC_RD_LNS_LN_FEATURE	= 0x00,
	RBLE_LNPC_RD_LNS_LOCATION_SPEED_CFG,
	RBLE_LNPC_RD_LNS_POSITION_QUALITY,
	RBLE_LNPC_RD_LNS_LN_CP_CFG,
	RBLE_LNPC_RD_LNS_NAVIGATION_CFG,
	RBLE_LNPC_RD_DIS_MANUF,
	RBLE_LNPC_RD_DIS_MODEL,
	RBLE_LNPC_RD_DIS_SERNB,
	RBLE_LNPC_RD_DIS_HWREV,
	RBLE_LNPC_RD_DIS_FWREV,
	RBLE_LNPC_RD_DIS_SWREV,
	RBLE_LNPC_RD_DIS_SYSID,
	RBLE_LNPC_RD_DIS_IEEE,
	RBLE_LNPC_RD_BAS_BL,
	RBLE_LNPC_RD_BAS_BL_CFG
};

/**
 * @brief	Internal codes for writing LNS or BAS characteristic client configuration
 */
enum RBLE_LNPC_WR_CHAR_CODE_enum {
	RBLE_LNP_LOCATION_SPEED_CODE = 0x00,
	RBLE_LNP_LN_CONTROL_POINT_CODE,
	RBLE_LNP_NAVIGATION_CODE,
	RBLE_LNP_BATTERY_LEVEL_CODE
};

/**
 * @brief	 LN Control Point Operation Code
 */
enum RBLE_LNP_LNCP_OP_CODE_enum {
	RBLE_LNP_OP_SET_CUMULATIVE_CODE = 0x01,
	RBLE_LNP_OP_MASK_LS_CONTENTS_CODE,
	RBLE_LNP_OP_NAVIGATION_CONTROL_CODE,
	RBLE_LNP_OP_REQ_NUM_OF_ROUTE_CODE,
	RBLE_LNP_OP_REQ_NAME_OF_ROUTE_CODE,
	RBLE_LNP_OP_SELECT_ROUTE_CODE,
	RBLE_LNP_OP_SET_FIX_RATE_CODE,
	RBLE_LNP_OP_SET_ELEVATION_CODE,
	RBLE_LNP_OP_RESPONSE_CODE = 0x20
};

/**
 * @brief	 Control Point Response Code
 */
enum RBLE_LNP_LNCP_RES_CODE_enum {
	RBLE_LNP_RES_SUCCESS_CODE = 0x01,
	RBLE_LNP_RES_NOT_SUPPORTED_CODE,
	RBLE_LNP_RES_INVALID_PARAM_CODE,
	RBLE_LNP_RES_OP_FAILED_CODE
};

/**
 * @brief	Navigation Control Code
 */
enum RBLE_LNP_CNTL_NAVIGATION_enum {
	RBLE_LNP_CNTL_NAVI_STOP = 0x00,
	RBLE_LNP_CNTL_NAVI_START,
	RBLE_LNP_CNTL_NAVI_PAUSE,
	RBLE_LNP_CNTL_NAVI_RESUME,
	RBLE_LNP_CNTL_NAVI_SKIP,
	RBLE_LNP_CNTL_NAVI_SET_NEAREST
};

/* ------------------------- Phone Alert Status Profile-------------------------------*/
/**
 * @brief	Internal codes for reading PASS characteristic with one single request
 */
enum RBLE_PASPC_RD_CHAR_CODE_enum {
	RBLE_PASPC_RD_PASS_ALERT_STATUS = 0x00,
	RBLE_PASPC_RD_PASS_ALERT_STATUS_CFG,
	RBLE_PASPC_RD_PASS_RINGER_SETTING,
	RBLE_PASPC_RD_PASS_RINGER_SETTING_CFG
};
/**
 * @brief	Internal codes for writing PASS characteristic client configuration
 */
enum RBLE_PASP_WR_CHAR_CODE_enum {
    RBLE_PASP_ALERT_STATUS_CODE = 0x00,
    RBLE_PASP_RINGER_SETTING_CODE
};

/**
 * @brief	 Ringer Setting Code
 */
enum RBLE_PASP_RINGER_SETTING_enum {
	RBLE_PASP_RINGER_SILENT = 0x00,
	RBLE_PASP_RINGER_NORMAL
};

/**
 * @brief	 Ringer Control Point Operation Code
 */
enum RBLE_PASP_RINGER_MODE_enum {
	RBLE_PASP_SILENT_MODE = 0x01,
	RBLE_PASP_MUTE_ONCE,
	RBLE_PASP_CANCEL_SILENT_MODE
};

/**
 * @brief	 Alert Status Code
 */
enum RBLE_PASP_ALERT_STATE_BIT_enum {
	RBLE_PASP_RINGER_STATE_BIT = 0x01,
	RBLE_PASP_VIBRATOR_STATE_BIT = 0x02,
	RBLE_PASP_DISPLAY_ALERT_STATE_BIT= 0x04
};

/* ------------------------- Running Speed and Cadence Profile-------------------------------*/
#define RBLE_RSCP_SENSORE_LOCATION_MAX 	17

/**
 * @brief	Internal codes for reading RSCS or DIS characteristic with one single request
 */
enum RBLE_RSCPC_RD_CHAR_CODE_enum {
	RBLE_RSCPC_RD_RSCS_RM_CFG	= 0x00,
	RBLE_RSCPC_RD_RSCS_SCCP_CFG,
	RBLE_RSCPC_RD_RSCS_RSC_FEATURE,
	RBLE_RSCPC_RD_RSCS_SL,
	RBLE_RSCPC_RD_DIS_MANUF,
	RBLE_RSCPC_RD_DIS_MODEL,
	RBLE_RSCPC_RD_DIS_SERNB,
	RBLE_RSCPC_RD_DIS_HWREV,
	RBLE_RSCPC_RD_DIS_FWREV,
	RBLE_RSCPC_RD_DIS_SWREV,
	RBLE_RSCPC_RD_DIS_SYSID,
	RBLE_RSCPC_RD_DIS_IEEE
};

/**
 * @brief	Internal codes for writing RSCS characteristic client configuration
 */
enum RBLE_RSCPC_WR_CHAR_CODE_enum {
	RBLE_RSCPC_RSC_MEAS_CODE = 0x00,
	RBLE_RSCPC_SC_CONTROL_POINT_CODE
};

/**
 * @brief	 Sensor Location
 */
enum RBLE_RSCPC_SENSOR_LOCATION_enum {
	RBLE_RSCPC_SENSOR_OTHER = 0x00,
	RBLE_RSCPC_SENSOR_TOP_OF_SHOE,
	RBLE_RSCPC_SENSOR_IN_SHOE,
	RBLE_RSCPC_SENSOR_HIP,
	RBLE_RSCPC_SENSOR_FRONT_WHEEL,
	RBLE_RSCPC_SENSOR_LEFT_CRANK,
	RBLE_RSCPC_SENSOR_RIGHT_CRANK,
	RBLE_RSCPC_SENSOR_LEFT_PEDAL,
	RBLE_RSCPC_SENSOR_RIGHT_PEDAL,
	RBLE_RSCPC_SENSOR_FRONT_HUB,
	RBLE_RSCPC_SENSOR_REAR_DROPOUT,
	RBLE_RSCPC_SENSOR_CHAINSTAY,
	RBLE_RSCPC_SENSOR_REAR_WHEEL,
	RBLE_RSCPC_SENSOR_REAR_HUB,
	RBLE_RSCPC_SENSOR_CHEST
};

/**
 * @brief	 Control Point Operation Code
 */
enum RBLE_RSCP_SCCP_OP_CODE_enum {
	RBLE_RSCP_OP_SET_CUMULATIVE_CODE = 0x01,
	RBLE_RSCP_OP_START_CALIBRATION_CODE,
	RBLE_RSCP_OP_UPDATE_SL_CODE,
	RBLE_RSCP_OP_REQ_SUPPORTED_SL_CODE,
	RBLE_RSCP_OP_RESPONSE_CODE = 0x10
};

/**
 * @brief	 Control Point Response Code
 */
enum RBLE_RSCP_SCCP_RES_CODE_enum {
	RBLE_RSCP_RES_SUCCESS_CODE = 0x01,
	RBLE_RSCP_RES_NOT_SUPPORTED_CODE,
	RBLE_RSCP_RES_INVALID_PARAM_CODE,
	RBLE_RSCP_RES_OP_FAILED_CODE
};

/* ------------------------- Vendor Specific-------------------------------*/
#define RBLE_VS_GPIO_BIT_0	0x01
#define RBLE_VS_GPIO_BIT_1	0x02
#define RBLE_VS_GPIO_BIT_2	0x04
#define RBLE_VS_GPIO_BIT_3	0x08

#define RBLE_VS_GPIO_INPUT	0
#define RBLE_VS_GPIO_OUTPUT	1

#define RBLE_VS_GPIO_LOW	0
#define RBLE_VS_GPIO_HIGH	1

#define RBLE_VS_GPIO_DIR_SETTING(val, bit, dir)		val = (uint8_t)(((dir)==RBLE_VS_GPIO_INPUT)\
													?((uint8_t)(val)&~(bit))\
													:((uint8_t)(val)|(bit)))

#define RBLE_VS_GPIO_OUTPUT_SETTING(val, bit, set)	val = (uint8_t)(((set)==RBLE_VS_GPIO_LOW)\
													?((uint8_t)(val)&~(bit))\
													:((uint8_t)(val)|(bit)))

enum RBLE_TEST_DATA_PATTERN_enum {
	RBLE_TEST_DATA_PATTERN_PN9       = 0x00,   /**< Pseudo-Random bit sequence 9 */
	RBLE_TEST_DATA_PATTERN_11110000  = 0x01,   /**< Pattern of alternating bits '11110000' */
	RBLE_TEST_DATA_PATTERN_10101010  = 0x02,   /**< Pattern of alternating bits '10101010' */
	RBLE_TEST_DATA_PATTERN_PN15      = 0x03,   /**< Pseudo-Random bit sequence 15 */
	RBLE_TEST_DATA_PATTERN_ALL1      = 0x04,   /**< Pattern of All '1' bits */
	RBLE_TEST_DATA_PATTERN_ALL0      = 0x05,   /**< Pattern of All '0' bits */
	RBLE_TEST_DATA_PATTERN_00001111  = 0x06,   /**< Pattern of alternating bits '00001111' */
	RBLE_TEST_DATA_PATTERN_01010101  = 0x07,   /**< Pattern of alternating bits '0101' */
};

enum RBLE_VS_TXPW_SET_LEVEL_enum {
	RBLE_VS_TXPW_LV1                 = 0x01,   /**< Level 1 */
	RBLE_VS_TXPW_LV2                 = 0x02,   /**< Level 2 */
	RBLE_VS_TXPW_LV3                 = 0x03,   /**< Level 3 */
	RBLE_VS_TXPW_LV4                 = 0x04,   /**< Level 4 */
	RBLE_VS_TXPW_LV5                 = 0x05,   /**< Level 5 */
	RBLE_VS_TXPW_LV6                 = 0x06,   /**< Level 6 */
	RBLE_VS_TXPW_LV7                 = 0x07,   /**< Level 7 */
	RBLE_VS_TXPW_LV8                 = 0x08,   /**< Level 8 */
	RBLE_VS_TXPW_LV9                 = 0x09    /**< Level 9 */
};

enum RBLE_VS_TXPW_MODE_enum {
	RBLE_VS_TXPW_MODE_NORMAL,
	RBLE_VS_TXPW_MODE_ADAPT_NEAR,
	RBLE_VS_TXPW_MODE_ADAPT_MIDDLE,
	RBLE_VS_TXPW_MODE_ADAPT_FAR
};

enum RBLE_VS_GPIO_MD_enum {
	RBLE_VS_GPIO_INPUT_MD,
	RBLE_VS_GPIO_OUTPUT_MD
};

enum RBLE_VS_FLASH_CMD_enum
{
	RBLE_VS_FLASH_CMD_START,
	RBLE_VS_FLASH_CMD_STOP,
	RBLE_VS_FLASH_CMD_WRITE,
	RBLE_VS_FLASH_CMD_READ,
	RBLE_VS_FLASH_CMD_CLEANUP,
	RBLE_VS_FLASH_CMD_FORMAT
};

enum RBLE_VS_ADAPT_CMD_enum
{
	RBLE_VS_ADAPT_CMD_DISABLE		= 0x00,
	RBLE_VS_ADAPT_CMD_ENABLE		= 0x01,
	RBLE_VS_ADAPT_CMD_ENABLE_WO_IND = 0x81
};

enum RBLE_VS_ADAPT_STATE_enum
{
	RBLE_VS_ADAPT_MODE_NEAR,
	RBLE_VS_ADAPT_MODE_MIDDLE,
	RBLE_VS_ADAPT_MODE_FAR
};

enum RBLE_VS_RFCNTL_CMD_enum
{
	RBLE_VS_RFCNTL_CMD_POWDOWN,
	RBLE_VS_RFCNTL_CMD_POWUP_DDCON,
	RBLE_VS_RFCNTL_CMD_POWUP_DDCOFF,
};

enum RBLE_VS_SET_PARAM_enum {
	RBLE_VS_PARAM_DISC_SCAN_TIME	 = 0x00,	/**< Set gap_discovery_scan_time */
	RBLE_VS_PARAM_DISC_SCAN_INTV,				/**< Set gap_dev_search_scan_intv */
	RBLE_VS_PARAM_DISC_SCAN_WIND,				/**< Set gap_dev_search_scan_window */
	RBLE_VS_PARAM_LIM_ADV_TO,					/**< Set gap_lim_adv_timeout */
	RBLE_VS_PARAM_SCAN_FAST_INTV,				/**< Set gap_scan_fast_intv */
	RBLE_VS_PARAM_SCAN_FAST_WIND,				/**< Set gap_scan_fast_window */
	RBLE_VS_PARAM_CONN_INTV_MIN,				/**< Set gap_init_conn_min_intv */
	RBLE_VS_PARAM_CONN_INTV_MAX,				/**< Set gap_init_conn_max_intv */
	RBLE_VS_PARAM_CONN_CE_MIN,					/**< Set gap_conn_min_ce_length */
	RBLE_VS_PARAM_CONN_CE_MAX,					/**< Set gap_conn_max_ce_length */
	RBLE_VS_PARAM_CONN_SLAVE_LATENCY,			/**< Set gap_conn_slave_latency */
	RBLE_VS_PARAM_CONN_SVTO,					/**< Set gap_dev_supervision_timeout */
	RBLE_VS_PARAM_RPA_INTV,						/**< Set gap_resolvable_private_addr_intv */
	RBLE_VS_PARAM_USER_DEFINED_TOP	= 0x80		/**< User defined parameter top */
};

/**************************** Structure Declaration ****************************************/

typedef struct RBLE_BD_NAME_t
{
    uint8_t  namelen;					/*length for name */
    uint8_t  name[RBLE_BD_NAME_SIZE];	/* array of bytes for name */
}RBLE_BD_NAME;

/* BD Address structure */
typedef struct RBLE_BD_ADDR_t
{
    uint8_t  addr[RBLE_BD_ADDR_LEN];    /* 6-byte array address value */
}RBLE_BD_ADDR;

/* Channel map structure */
typedef struct RBLE_LE_CHNL_MAP_t
{
    uint8_t  map[RBLE_LE_CHNL_MAP_LEN]; /* 5-byte channel map array */
}RBLE_LE_CHNL_MAP;


/*--------------------  Device Mode  -------------------- */

/* Advertising Parameters Command parameters structure */
typedef struct RBLE_SET_ADV_PARAM_t
{
    uint16_t		adv_intv_min;		/* Minimum interval for advertising */
    uint16_t		adv_intv_max;		/* Maximum interval for advertising */
    uint8_t			adv_type;			/* Advertising type */
    uint8_t			own_addr_type;		/* Own address type: public=0x00 /random = 0x01 */
    uint8_t			direct_addr_type;	/* Direct address type: public=0x00 /random = 0x01 */
    RBLE_BD_ADDR	direct_addr;		/* Direct Bluetooth device address */
    uint8_t			adv_chnl_map;		/* Advertising channel map */
    uint8_t			adv_filt_policy;	/* Advertising filter policy */
    uint8_t			reserved;			/* Reserve */
}RBLE_SET_ADV_PARAM;

/* Advertising data structure */
typedef struct RBLE_ADV_DATA_t
{
    uint8_t        data[RBLE_ADV_DATA_LEN];    /* Maximum length data bytes array */
}RBLE_ADV_DATA;

/* Advertising Data Command parameters structure */
typedef struct RBLE_SET_ADV_DATA_t
{
    uint8_t         adv_data_len;   /* Advertising data length */
    RBLE_ADV_DATA	adv_data;		/* Advertising data - maximum 31 bytes */
}RBLE_SET_ADV_DATA;

/* Scan response data structure */
typedef struct RBLE_SCAN_RSP_DATA_t
{
    uint8_t        data[RBLE_SCAN_RSP_DATA_LEN];    /* Maximum length data bytes array */
}RBLE_SCAN_RSP_DATA;

/* Set Scan Response Data Command parameters structure */
typedef struct RBLE_SET_SCAN_RSP_DATA_t
{
    uint8_t				scan_rsp_data_len;		/* Scan response data length */
    RBLE_SCAN_RSP_DATA	data;					/* Scan response data - maximum 31 bytes */
}RBLE_SET_SCAN_RSP_DATA;

/* Advertising information */
typedef struct RBLE_ADV_INFO_t
{
    RBLE_SET_ADV_PARAM		adv_param;		/* Advertising parameter structure */
    RBLE_SET_ADV_DATA		adv_data;		/* Advertising data structure */
    RBLE_SET_SCAN_RSP_DATA	scan_rsp_data;	/* Scan response data structure */
}RBLE_ADV_INFO;

/* Set Scan Parameters Command parameters structure */
typedef struct RBLE_SET_SCAN_PARAMETER_t
{
    uint8_t        scan_type;			/* Scan type - 0=passive / 1=active */
    uint8_t        reserved;			/* Reserve */
    uint16_t       scan_intv;			/* Scan interval */
    uint16_t       scan_window;			/* Scan window size */
    uint8_t        own_addr_type;		/* Own address type - 0=public, 1=random */
    uint8_t        scan_filt_policy;	/* Scan filter policy */
}RBLE_SET_SCAN_PARAMETER;

/* Scanning information referenced */
typedef struct RBLE_SCANNING_INFO_t
{
    RBLE_SET_SCAN_PARAMETER	set_scan;	/* Scan parameter command structure */
    uint8_t					filter_dup;	/* Filtering policy */
    uint8_t					reserved;	/* Reserve */
}RBLE_SCANNING_INFO;


/* Add Device(Remove Device) to White List Command parameters structure */
typedef struct RBLE_DEV_ADDR_INFO_t
{
    uint8_t			dev_addr_type;	/* Type of address of the device to be added to(removed from) the White List - 0=public/1=random */
    RBLE_BD_ADDR	dev_addr;		/* Address of device to be added to(removed from) White List */
}RBLE_DEV_ADDR_INFO;

/* Create Connection Command parameters structure */
typedef struct RBLE_CREATE_CONNECT_PARAM_t
{
    uint16_t       scan_intv;       /* Scan interval */
    uint16_t       scan_window;     /* Scan window size */
    uint8_t        init_filt_policy;/* Initiator filter policy */
    uint8_t        peer_addr_type;  /* Peer address type - 0=public/1=random */
    RBLE_BD_ADDR   peer_addr;       /* Peer BD address */
    uint8_t        own_addr_type;   /* Own address type - 0=public/1=random */
    uint8_t        reserved;        /* Reserved */
    uint16_t       con_intv_min;    /* Minimum of connection interval */
    uint16_t       con_intv_max;    /* Maximum of connection interval */
    uint16_t       con_latency;     /* Connection latency */
    uint16_t       superv_to;       /* Link supervision timeout */
    uint16_t       ce_len_min;      /* Minimum CE length */
    uint16_t       ce_len_max;      /* Maximum CE length */
}RBLE_CREATE_CONNECT_PARAM;

typedef struct RBLE_CONNECT_INFO_t
{
    uint8_t status;					/*  Confirmation status */
    uint8_t role;					/* Role */
    uint16_t conhdl;				/* Connection handle */
    uint8_t peer_addr_type;			/* Peer address type */
    RBLE_BD_ADDR peer_addr;			/* Peer BT address */
    uint8_t idx;					/* Connection Index */
    uint16_t con_interval;			/* Connection interval */
    uint16_t con_latency;			/* Connection latency */
    uint16_t sup_to;				/* Link supervision timeout */
    uint8_t clk_accuracy;			/* Clock accuracy */
    uint8_t reserved3;
}RBLE_CONNECT_INFO;

/* Set Scan Enable Command parameters structure */
typedef struct RBLE_SET_SCAN_EN_t
{
    uint8_t        scan_en;          /* Scan enable - 0=disabled, 1=enabled */
    uint8_t        filter_duplic_en; /* Enable for duplicates filtering - 0 =disabled/ 1=enabled */
}RBLE_SET_SCAN_EN;

/* Bonding Parameter */
typedef struct RBLE_BOND_PARAM_t
{
    RBLE_BD_ADDR addr; /* BT Address to bond */
    uint8_t oob;       /* OOB information */
    uint8_t iocap;     /* IO capabilities */
    uint8_t auth;      /* Authentication requirements */
    uint8_t key_size;  /* Encryption key size */
    uint8_t ikey_dist; /* Initiator key distribution */
    uint8_t rkey_dist; /* Responder key distribution */
}RBLE_BOND_PARAM;

/* Bonding Response Parameter */
typedef struct RBLE_BOND_RESP_PARAM_t
{
    uint16_t conhdl;      /* Connection handle */
    uint8_t accept;       /* accept or reject bonding */
    uint8_t io_cap;       /* IO capabilities */
    uint8_t oob;          /* OOB flag */
    uint8_t auth_req;     /* Authentication Requirements */
    uint8_t max_key_size; /* Max key size */
    uint8_t ikeys;        /* Initiator key distribution */
    uint8_t rkeys;        /* Responder key distribution */
	uint8_t reserved;
}RBLE_BOND_RESP_PARAM;

/* Connection Update Parameter*/
typedef struct RBLE_CONN_PARAM_t
{
    uint16_t intv_min;    /* Connection interval minimum */
    uint16_t intv_max;    /* Connection interval maximum */
    uint16_t latency;     /* Latency */
    uint16_t time_out;    /* Supervision timeout */
}RBLE_CONN_PARAM;

/* Device Version Infomation */
typedef struct RBLE_DEVICE_VER_INFO_t{
    uint8_t  hci_ver;		/* HCI Version */
    uint8_t  lmp_ver;		/* LMP Version */
    uint8_t  host_ver;		/* Host Version */
    uint8_t  reserved;		/* reserved */
    uint16_t hci_subver;	/* HCI Sub Versiong */
    uint16_t lmp_subver;	/* LMP Version */
    uint16_t host_subver;	/* Host Sub Versiong */
    uint16_t company_id;	/* Company Id */
}RBLE_DEVICE_VER_INFO;

/* Supported LE Features structure */
typedef struct RBLE_FEATURES_t
{
    uint8_t feats[RBLE_LE_FEATS_LEN];    /* 8-byte array for LE features */
}RBLE_FEATURES;

/* Advertising report structure */
typedef struct RBLE_ADV_REPORT_t
{
    uint8_t        evt_type;           		/* Event type */
    uint8_t        adv_addr_type;      		/* Advertising address type: public/random */
    RBLE_BD_ADDR   adv_addr;           		/* Advertising address value */
    uint8_t        data_len;           		/* Data length in advertising packet */
    uint8_t        data[RBLE_ADV_DATA_LEN]; /* Data of advertising packet */
    uint8_t        rssi;               		/* RSSI value for advertising packet */
    uint8_t        reserved;
}RBLE_ADV_REPORT;

/* LLM LE advertising report event structure */
typedef struct RBLE_ADV_REPORT_EVT_t
{
    RBLE_ADV_REPORT     adv_rep;        /* Advertising reports structures array */
}RBLE_ADV_REPORT_EVT;

/*-----------------  Security Manager ----------------- */

/* Generic Key Value structure */
typedef struct RBLE_KEY_VALUE_t
{
    uint8_t key[RBLE_KEY_LEN];    /* Key value */
}RBLE_KEY_VALUE;

typedef struct RBLE_RAND_NB_t
{
    uint8_t     nb[RBLE_RAND_NB_LEN];
}RBLE_RAND_NB;

/*-----------------------  GATT ----------------------- */
/**
 * @brief	desired UUID
 */
typedef struct RBLE_GATT_DESIRED_TYPE_t {
	uint16_t value_size;						/**< Size of the UUID */
	uint8_t value[RBLE_GATT_128BIT_UUID_OCTET];	/**< actual UUID */
} RBLE_GATT_DESIRED_TYPE;

/**
 * @brief	UUID with different length Structure
 */
typedef struct RBLE_GATT_UUID_TYPE_t {
	uint8_t value_size;							/**< Size of the UUID */
	uint8_t expect_resp_size;					/**< expected response size - read multiple */
	uint8_t value[RBLE_GATT_128BIT_UUID_OCTET];	/**< actual UUID */
} RBLE_GATT_UUID_TYPE;

/**
 * @brief	Reliable Structure
 */
typedef struct RBLE_GATT_RELIABLE_WRITE_t {
	uint16_t elmt_hdl;							/**< characteristic handle */
	uint16_t size;								/**< value size */
	uint8_t value[RBLE_GATT_MAX_RELIABLE_WRITE_CONTENTS];	/**< value holder */
} RBLE_GATT_RELIABLE_WRITE;


/**
 * @brief	Service Discovery Request Structure
 *
 * Parameters for @ref RBLE_GATT_Discovery_Service_Request
 */
typedef struct RBLE_GATT_DISC_SVC_REQ_t {
	uint8_t req_type;							/**< GATT request type */
	uint8_t reserved;
	uint16_t conhdl;							/**< connection handle */
	uint16_t start_hdl;							/**< start handle range */
	uint16_t end_hdl;							/**< end handle range */
	RBLE_GATT_DESIRED_TYPE desired_svc;			/**< desired service: if 0x0000, discover all */
} RBLE_GATT_DISC_SVC_REQ;

/**
 * @brief	Characteristic Discovery Request Structure
 *
 * Parameters for @ref RBLE_GATT_Discovery_Char_Request
 */
typedef struct RBLE_GATT_DISC_CHAR_REQ_t {
	uint8_t req_type;							/**< GATT request type */
	uint8_t reserved;
	uint16_t conhdl;							/**< connection handle */
	uint16_t start_hdl;							/**< start handle range */
	uint16_t end_hdl;							/**< end handle range */
	RBLE_GATT_DESIRED_TYPE desired_char;		/**< desired UUID in disc service char */
} RBLE_GATT_DISC_CHAR_REQ;

/**
 * @brief	Characteristic Descriptor Discovery Request Structure
 *
 * Parameters for @ref RBLE_GATT_Discovery_Char_Descriptor_Request
 */

typedef struct RBLE_GATT_DISC_CHAR_DESC_REQ_t {
	uint16_t conhdl;							/**< connection handle */
	uint16_t start_hdl;							/**< start handle range */
	uint16_t end_hdl;							/**< end handle range */
} RBLE_GATT_DISC_CHAR_DESC_REQ;

/**
 * @brief	Read Characteristic Values and Descriptor Request Structure
 *
 * Parameters for @ref RBLE_GATT_Read_Char_Request
 */
typedef struct RBLE_GATT_READ_CHAR_REQ_t {
	uint8_t req_type;							/**< request type */
	uint8_t reserved;
	uint16_t offset;							/**< read offset */
	uint16_t conhdl;							/**< connection handle */
	uint16_t start_hdl;							/**< start handle range */
	uint16_t end_hdl;							/**< end handle range */
	uint16_t nb_uuid;							/**< number of UUID */
    RBLE_GATT_UUID_TYPE uuid[RBLE_GATT_MAX_NB_HDLS];/**< characteristic UUID */
} RBLE_GATT_READ_CHAR_REQ;

/**
 * @brief	Write Characteristic Request Structure
 *
 * Parameters for @ref RBLE_GATT_Write_Char_Request
 */
typedef struct RBLE_GATT_WRITE_CHAR_REQ_t {
	uint16_t conhdl;							/**< connection handle */
	uint16_t charhdl;							/**< valid characteristic handle */
	uint16_t wr_offset;							/**< offset to write */
	uint16_t val_len;							/**< size of the value data */
	uint8_t req_type;							/**< request type */
	uint8_t auto_execute;						/**< execute write */
	uint8_t value[RBLE_GATT_MAX_LONG_VALUE];	/**< check, maybe union type is required */
} RBLE_GATT_WRITE_CHAR_REQ;

/**
 * @brief	Write Reliable Characteristic Request Structure
 *
 * Parameters for @ref RBLE_GATT_Write_Reliable_Request
 */
typedef struct RBLE_GATT_WRITE_RELIABLE_REQ_t {
	uint8_t nb_writes;							/**< number of reliable writes */
	uint8_t auto_execute;						/**< automatic execute write */
	uint16_t conhdl;							/**< connection handle */
	RBLE_GATT_RELIABLE_WRITE value[RBLE_GATT_MAX_RELIABLE_WRITE_NUM];	/**< number of reliable */
} RBLE_GATT_WRITE_RELIABLE_REQ;

/**
 * @brief	execute write characteristic request Structure
 *
 * Parameters for @ref RBLE_GATT_Execute_Write_Char_Request
 */
typedef struct RBLE_GATT_EXE_WR_CHAR_REQ_t {
	uint8_t exe_wr_ena;							/**< flag to describe if write or cancel */
	uint8_t reserved;
	uint16_t conhdl;							/**< connection handle */
} RBLE_GATT_EXE_WR_CHAR_REQ;

/**
 * @brief	notify request Structure
 *
 * Parameters for @ref RBLE_GATT_Notify_Request
 */
typedef struct RBLE_GATT_NOTIFY_REQ_t {
	uint16_t conhdl;							/**< connection handle */
	uint16_t charhdl;							/**< characteristic handle */
} RBLE_GATT_NOTIFY_REQ;

/**
 * @brief	Indicate Request Structure
 *
 * Parameters for @ref RBLE_GATT_Indicate_Request
 */
typedef struct RBLE_GATT_INDICATE_REQ_t {
	uint16_t conhdl;							/**< connection handle */
	uint16_t charhdl;							/**< characteristic handle */
} RBLE_GATT_INDICATE_REQ;

/**
 * @brief	Write Response Structure
 *
 * Parameters for @ref RBLE_GATT_Write_Response
 */
typedef struct RBLE_GATT_WRITE_RESP_t {
	uint16_t conhdl;							/**< Connection handle */
	uint16_t att_hdl;							/**< Attribute handle */
	uint8_t att_code;							/**< ATT code */
	uint8_t reserved;
} RBLE_GATT_WRITE_RESP;


/**
 * @brief	Set Permission Structure
 *
 * Parameters for @ref RBLE_GATT_Set_Permission
 */
typedef struct RBLE_GATT_SET_PERM_t {
	uint16_t start_hdl;							/**< start handle range */
	uint16_t end_hdl;							/**< end handle range */
	uint16_t perm;								/**< Permission of attr */
} RBLE_GATT_SET_PERM;

/**
 * @brief	Set Data Structure
 *
 * Parameters for @ref RBLE_GATT_Set_Data
 */
typedef struct RBLE_GATT_SET_DATA_t {
	uint16_t val_hdl;							/**< value handle */
	uint16_t val_len;							/**< size of the value data */
	uint8_t value[RBLE_GATT_MAX_LONG_VALUE];	/**< value data */
} RBLE_GATT_SET_DATA;

/**
 * @brief	Data list for service result
 */
typedef struct RBLE_GATT_SVC_LIST_t {
	uint16_t start_hdl;							/**< start handle */
	uint16_t end_hdl;							/**< end handle */
	uint16_t attr_hdl;							/**< attribute handle */
} RBLE_GATT_SVC_LIST;

/**
 * @brief	Data list for service result - 128bit
 */
typedef struct RBLE_GATT_SVC_128_LIST_t {
	uint16_t start_hdl;							/**< start handle */
	uint16_t end_hdl;							/**< end handle */
	uint8_t attr_hdl[RBLE_GATT_128BIT_UUID_OCTET];	  /**< attribute handle */
} RBLE_GATT_SVC_128_LIST;

/**
 * @brief	service range
 */
typedef struct RBLE_GATT_SVC_RANGE_LIST_t {
	uint16_t start_hdl;							/**< start handle */
	uint16_t end_hdl;							/**< end handle */
} RBLE_GATT_SVC_RANGE_LIST;

/**
 * @brief	Data list for include result
 */
typedef struct RBLE_GATT_INCL_LIST_t {
	uint16_t attr_hdl;							/**< element handle */
	uint16_t start_hdl;							/**< start handle */
	uint16_t end_hdl;							/**< end handle */
	uint16_t uuid;								/**< included service UUID */
} RBLE_GATT_INCL_LIST;

/**
 * @brief	Special data list for include result - 128bit
 */
typedef struct RBLE_GATT_INCL_128_LIST_t {
	uint16_t attr_hdl;							/**< element handle */
	uint16_t start_hdl;							/**< start handle */
	uint16_t end_hdl;							/**< end handle */
	uint8_t uuid[RBLE_GATT_128BIT_UUID_OCTET];	/**< included 128-bit service UUID */
} RBLE_GATT_INCL_128_LIST;

/**
 * @brief	Data list for characteristic result
 */
typedef struct RBLE_GATT_CHAR_LIST_t {
	uint16_t attr_hdl;							/**< database element handle */
	uint8_t prop;								/**< properties */
	uint8_t reserved;
	uint16_t pointer_hdl;						/**< pointer handle to UUID */
	uint16_t uuid;								/**< characteristic UUID */
} RBLE_GATT_CHAR_LIST;

/**
 * @brief	Data list for characteristic result - 128bit
 */
typedef struct RBLE_GATT_CHAR_128_LIST_t {
	uint16_t attr_hdl;							/**< database element handle */
	uint8_t prop;								/**< properties */
	uint8_t reserved;
	uint16_t pointer_hdl;						/**< pointer handle to UUID */
	uint8_t uuid[RBLE_GATT_128BIT_UUID_OCTET];	/**< characteristic UUID */
} RBLE_GATT_CHAR_128_LIST;

/**
 * @brief	Special data list for descriptor result
 */
typedef struct RBLE_GATT_CHAR_DESC_LIST_t {
	uint16_t attr_hdl;							/**< database element handle */
	uint16_t desc_hdl;							/**< descriptor UUID */
} RBLE_GATT_CHAR_DESC_LIST;

/**
 * @brief	Special data list for descriptor result - 128bit
 */
typedef struct RBLE_GATT_CHAR_DESC_128_LIST_t {
	uint16_t attr_hdl;							/**< database element handle */
	uint8_t uuid[RBLE_GATT_128BIT_UUID_OCTET];	/**< 128-bit descriptor UUID */
} RBLE_GATT_CHAR_DESC_128_LIST;

/**
 * @brief	Attribute data holder
 */
typedef struct RBLE_GATT_INFO_DATA_t {
	uint8_t each_len;							/**< each result length */
	uint8_t len;								/**< data length */
	uint8_t data[RBLE_GATT_MAX_VALUE];			/**< data */
} RBLE_GATT_INFO_DATA;

/**
 * @brief	Query result for multiple responses
 */
typedef struct RBLE_GATT_QUERY_RESULT_t {
	uint8_t len;								/**< length of value */
	uint8_t value[RBLE_GATT_MAX_VALUE];			/**< data result from query */
} RBLE_GATT_QUERY_RESULT;


/*------------------ Find Me Profile ------------------ */

/* Immediate Alert service details container */
typedef struct RBLE_FMP_IAS_CONTENT_t
{
    uint16_t shdl;             /* Start Handle */
    uint16_t ehdl;             /* End Handle */
    uint16_t alert_char_hdl;   /* Alert Level Characteristic handle */
    uint16_t alert_val_hdl;    /* Alert Level Characteristic value handle */
    uint8_t  alert_char_prop;  /* Alert Level Characteristic properties */
    uint8_t  reserved;
}RBLE_FMP_IAS_CONTENT;

/* ------------------ Proximity Profile ------------------*/

/* Generic structure for Alert Level Characteristic content */
typedef struct RBLE_ALERT_LVL_CHAR_t
{
    uint16_t char_hdl; /* Alert level char handle */
    uint16_t val_hdl;  /* alert Level value handle */
    uint8_t  prop;     /* Alert level properties */
    uint8_t  value;    /* Alert value */
}RBLE_ALERT_LVL_CHAR;

/* Link Loss Service information */
typedef struct RBLE_LLS_CONTENT_t
{
    uint16_t shdl;                 /* Service start handle */
    uint16_t ehdl;                 /* Service end handle */
    RBLE_ALERT_LVL_CHAR alert_lvl; /* Alert level char handle */
}RBLE_LLS_CONTENT;

/* Immediate Alert Service information */
typedef struct RBLE_PXP_IAS_CONTENT_t
{
    uint16_t shdl;                 /* Service start handle */
    uint16_t ehdl;                 /* Service end handle */
    RBLE_ALERT_LVL_CHAR alert_lvl; /* Alert level char handle */
}RBLE_PXP_IAS_CONTENT;

/* Tx Power Service information */
typedef struct RBLE_TPS_CONTENT_t
{
    uint16_t shdl;              /* Service start handle */
    uint16_t ehdl;              /* Service end handle */
    uint16_t txpw_lvl_char_hdl; /* TX Power level char handle */
    uint16_t txpw_lvl_val_hdl;  /* TX Power Level value handle */
    uint16_t txpw_lvl_cfg_hdl;  /* TX Power Level configuration characteristic value handle */
    uint8_t  txpw_lvl_prop;     /* TX Power level properties */
    uint8_t  txpw_lvl;          /* TX Power level */
}RBLE_TPS_CONTENT;

/* Proximity Monitor Role Param */
typedef struct RBLE_PROXI_MON_PARAM_t
{
	RBLE_LLS_CONTENT    lls;
	RBLE_PXP_IAS_CONTENT ias;
	RBLE_TPS_CONTENT    tps;
}RBLE_PROXI_MON_PARAM;

/* Attribute data holder */
typedef struct RBLE_ATT_INFO_DATA_t
{
    uint8_t each_len;          		   /* each result length */
    uint8_t len;               		   /* data length */
    uint8_t data[RBLE_ATTM_MAX_VALUE]; /* data */
}RBLE_ATT_INFO_DATA;

/* Attribute data holder (Long type) */
typedef struct RBLE_ATT_INFO_LDATA_t
{
    uint8_t val_len;          		    /* size of the value data */
    uint8_t  reserved;
    uint16_t attr_hdl;         		    /* handle */
    uint8_t value[RBLE_ATTM_MAX_VALUE]; /* actual value pairs */
}RBLE_ATT_INFO_LDATA;

/* ------------------ Health Thermometer Profile ------------------*/

/* Health Thermometer Parameter */
typedef struct RBLE_HTP_THERM_PARAM_t
{
    uint16_t temp_meas_ind_en;   /* Temperature measurement indication configuration */
    uint16_t interm_temp_ntf_en; /* Intermediate temperature notification configuration */
    uint16_t meas_intv_ind_en;   /* Measurement interval indication configuration */
    uint16_t meas_intv;         /* Measurement interval */
}RBLE_HTP_THERM_PARAM;

/* date and time structure */
typedef struct RBLE_DATE_TIME_t
{
    uint16_t year;		/* Year value */
    uint8_t  month;		/* Month value */
    uint8_t  day;		/* Day value */
    uint8_t  hour;		/* Hour value */
    uint8_t  min;		/* Minute value */
    uint8_t  sec;		/* Second value */
    uint8_t  reserved;	/* Reserved */
}RBLE_DATE_TIME;

/* Health Tehrmometer Temple Info */
typedef struct RBLE_HTP_TEMP_INFO_t
{
    uint8_t flag_stable_meas;	/* Stable or intermediary type of temperature */
    uint8_t flags;				/* flags */
    int32_t temp_val;			/* temp value */
    RBLE_DATE_TIME stamp;		/* time stamp */
    uint8_t type;				/* type */
    uint8_t  reserved;			/* Reserved */
}RBLE_HTP_TEMP_INFO;


/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_HTS_CONTENT_t
{
    uint16_t shdl;
    uint16_t ehdl;
    uint16_t temp_meas_char_hdl;
    uint16_t temp_meas_val_hdl;
    uint16_t temp_meas_cfg_hdl;
    uint8_t  temp_meas_prop;
    uint8_t  reserved;
    uint16_t temp_type_char_hdl;
    uint16_t temp_type_val_hdl;
    uint8_t  temp_type_prop;
    uint8_t  reserved2;
    uint16_t interm_temp_char_hdl;
    uint16_t interm_temp_val_hdl;
    uint16_t interm_temp_cfg_hdl;
    uint8_t  interm_temp_prop;
    uint8_t  reserved3;
    uint16_t meas_intv_char_hdl;
    uint16_t meas_intv_val_hdl;
    uint16_t meas_intv_cfg_hdl;
    uint16_t valid_range_hdl;
    uint8_t  meas_intv_prop;
    uint8_t  reserved4;
}RBLE_HTS_CONTENT;

/* todo check if we could skip properties because they're all supposed to be readable */
typedef struct RBLE_DIS_CONTENT_t
{
    uint16_t shdl;
    uint16_t ehdl;
    uint16_t sys_id_char_hdl;
    uint16_t sys_id_val_hdl;
    uint8_t  sys_id_prop;
    uint8_t  reserved;
    uint16_t model_nb_char_hdl;
    uint16_t model_nb_val_hdl;
    uint8_t  model_nb_prop;
    uint8_t  reserved2;
    uint16_t serial_nb_char_hdl;
    uint16_t serial_nb_val_hdl;
    uint8_t  serial_nb_prop;
    uint8_t  reserved3;
    uint16_t fw_rev_char_hdl;
    uint16_t fw_rev_val_hdl;
    uint8_t  fw_rev_prop;
    uint8_t  reserved4;
    uint16_t hw_rev_char_hdl;
    uint16_t hw_rev_val_hdl;
    uint8_t  hw_rev_prop;
    uint8_t  reserved5;
    uint16_t sw_rev_char_hdl;
    uint16_t sw_rev_val_hdl;
    uint8_t  sw_rev_prop;
    uint8_t  reserved6;
    uint16_t manuf_name_char_hdl;
    uint16_t manuf_name_val_hdl;
    uint8_t  manuf_name_prop;
    uint8_t  reserved7;
    uint16_t ieee_certif_char_hdl;
    uint16_t ieee_certif_val_hdl;
    uint8_t  ieee_certif_prop;
    uint8_t  reserved8;
}RBLE_DIS_CONTENT;

/* ------------------ Blood Pressure Profile ------------------*/

/* Blood Pressure Parameter */
typedef struct RBLE_BLP_SENSOR_PARAM_t
{
    uint16_t bldprs_meas_ind_en;     /* Blood pressure measurement indication configuration */
    uint16_t interm_cufprs_ntf_en;   /* Intermediate cuff pressure notification configuration */
}RBLE_BLP_SENSOR_PARAM;

/* Blood Pressure Measurements Info */
typedef struct RBLE_BLP_MEASUREMENTS_INFO_t
{
    uint8_t flag_stable_meas;	/* Stable or intermediary type of measurements */
    uint8_t flags;				/* flags */
    int16_t press_val_field1;	/* blood pressure value - systolic or cuff pressure */
    int16_t press_val_field2;	/* blood pressure value - diastolic or subfield1 */
    int16_t press_val_field3;	/* blood pressure value - MAP or subfield2 */
    RBLE_DATE_TIME stamp;		/* time stamp */
    int16_t rate;				/* pulse rate */
    uint8_t id;					/* user ID */
    uint8_t reserved;			/* Reserved */
    uint16_t meas_sts;			/* measurement status */
}RBLE_BLP_MEASUREMENTS_INFO;


/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_BLS_CONTENT_t
{
    uint16_t shdl;
    uint16_t ehdl;
    uint16_t bldprs_meas_char_hdl;
    uint16_t bldprs_meas_val_hdl;
    uint16_t bldprs_meas_cfg_hdl;
    uint8_t  bldprs_meas_prop;
    uint8_t  reserved;
    uint16_t interm_cufprs_char_hdl;
    uint16_t interm_cufprs_val_hdl;
    uint16_t interm_cufprs_cfg_hdl;
    uint8_t  interm_cufprs_prop;
    uint8_t  reserved2;
    uint16_t bldprs_feat_char_hdl;
    uint16_t bldprs_feat_val_hdl;
    uint8_t  bldprs_feat_prop;
    uint8_t  reserved3;
}RBLE_BLS_CONTENT;

/* ------------------ HID over GATT Profile ------------------*/

/* HID Parameter */
typedef struct RBLE_HGP_DEVICE_PARAM_t
{
    uint8_t     hids_inst_num;                              /* Number of HID Service Instance */
    uint8_t     bas_inst_num;                               /* Number of Battery Service Instance */
	uint16_t	report_input_ntf_en[RBLE_HIDS_INST_MAX];    /* Input Report notification configuration (multi-instance) */
	uint16_t	kb_report_ntf_en[RBLE_HIDS_INST_MAX];       /* Boot Keyboard Input Report notification configuration (multi-instance) */
	uint16_t	mo_report_ntf_en[RBLE_HIDS_INST_MAX];       /* Boot Mouse Input Report notification configuration (multi-instance) */
	uint8_t		protocol_mode_val[RBLE_HIDS_INST_MAX];      /* Protorol Mode (multi-instance) */
#if ((RBLE_HIDS_INST_MAX % 2) != 0)
    uint8_t     reserved;                                   /* Reserved */
#endif
	uint16_t	battery_level_ntf_en[RBLE_BAS_INST_MAX];    /* Battery Level notification configuration (multi-instance) */
}RBLE_HGP_DEVICE_PARAM;

/* Report structure */
typedef struct RBLE_HGP_REPORT_DESC_t
{
	uint8_t		device_type;                    /* Device type */
	uint8_t		report_type;                    /* Report type */
	uint8_t		value[RBLE_HIDS_REPORT_MAX];    /* Report values */
#if ((RBLE_HIDS_REPORT_MAX % 2) != 0)
	uint8_t     reserved;	                    /* Reserved */
#endif
    uint16_t    value_size;                     /* Report size */
}RBLE_HGP_REPORT_DESC;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_HIDS_CONTENT_t
{
    uint16_t    shdl;                           /* Service start handle */
    uint16_t    ehdl;                           /* Service end handle */
    uint16_t    protocol_md_char_hdl;           /* Protocol Mode char handle */
    uint16_t    protocol_md_val_hdl;            /* Protocol Mode value handle */
    uint8_t     protocol_md_prop;               /* Protocol Mode properties */
    uint8_t     reserved;	                    /* Reserved */
    uint16_t    report_input_char_hdl;          /* Report(Input) char handle */
    uint16_t    report_input_val_hdl;           /* Report(Input) value handle */
    uint16_t    report_input_cfg_hdl;           /* Report(Input) configuration characteristic value handle */
    uint16_t    input_rep_ref_hdl;              /* Report(Input) report reference value handle */
    uint8_t     report_input_prop;              /* Report(Input) properties */
    uint8_t     reserved1;	                    /* Reserved */
    uint16_t    report_output_char_hdl;         /* Report(Output) char handle */
    uint16_t    report_output_val_hdl;          /* Report(Output) value handle */
    uint16_t    output_rep_ref_hdl;             /* Report(Output) report reference value handle */
    uint8_t     report_output_prop;             /* Report(Output) properties */
    uint8_t     reserved2;	                    /* Reserved */
    uint16_t    report_feature_char_hdl;        /* Report(Feature) char handle */
    uint16_t    report_feature_val_hdl;         /* Report(Feature) value handle */
    uint16_t    feature_rep_ref_hdl;            /* Report(Feature) report reference value handle */
    uint8_t     report_feature_prop;            /* Report(Feature) properties */
    uint8_t     reserved3;	                    /* Reserved */
    uint16_t    report_map_char_hdl;            /* Report Map char handle */
    uint16_t    report_map_val_hdl;             /* Report Map value handle */
    uint16_t    external_rep_ref_hdl;           /* Report Map external report reference value handle */
    uint8_t     report_map_prop;                /* Report Map properties */
    uint8_t     reserved4;	                    /* Reserved */
    uint16_t    bootkb_input_char_hdl;          /* Boot Keyboard Input Report char handle */
    uint16_t    bootkb_input_val_hdl;           /* Boot Keyboard Input Report value handle */
    uint16_t    bootkb_input_cfg_hdl;           /* Boot Keyboard Input Report configuration characteristic value handle */
    uint8_t     bootkb_input_prop;              /* Boot Keyboard Input Report properties */
    uint8_t     reserved5;	                    /* Reserved */
    uint16_t    bootkb_output_char_hdl;         /* Boot Keyboard Output Report char handle */
    uint16_t    bootkb_output_val_hdl;          /* Boot Keyboard Output Report value handle */
    uint8_t     bootkb_output_prop;             /* Boot Keyboard Output Report properties */
    uint8_t     reserved6;	                    /* Reserved */
    uint16_t    bootmo_input_char_hdl;          /* Boot Mouse Input Report char handle */
    uint16_t    bootmo_input_val_hdl;           /* Boot Mouse Input Report value handle */
    uint16_t    bootmo_input_cfg_hdl;           /* Boot Mouse Input Report configuration characteristic value handle */
    uint8_t     bootmo_input_prop;              /* Boot Mouse Input Report properties */
    uint8_t     reserved7;	                    /* Reserved */
    uint16_t    hid_info_char_hdl;              /* HID Information char handle */
    uint16_t    hid_info_val_hdl;               /* HID Information value handle */
    uint8_t     hid_info_prop;                  /* HID Information properties */
    uint8_t     reserved8;	                    /* Reserved */
    uint16_t    hid_cp_char_hdl;                /* HID Control Point char handle */
    uint16_t    hid_cp_val_hdl;                 /* HID Control Point value handle */
    uint8_t     hid_cp_prop;                    /* HID Control Point properties */
    uint8_t     reserved9;	                    /* Reserved */
    uint16_t    include_svc_hdl;                /* Included Service handle */
    uint16_t    include_svc_uuid;               /* Included Service UUID */
    uint16_t    incl_shdl;                      /* Included Service start handle */
    uint16_t    incl_ehdl;                      /* Included Service end handle */
}RBLE_HIDS_CONTENT;

typedef struct RBLE_DIS11_CONTENT_t
{
    uint16_t    shdl;                           /* Service start handle */
    uint16_t    ehdl;                           /* Service end handle */
    uint16_t    pnp_id_char_hdl;                /* PnP ID char handle */
    uint16_t    pnp_id_val_hdl;                 /* PnP ID value handle */
    uint8_t     pnp_id_prop;                    /* PnP ID properties */
    uint8_t     reserved;	                    /* Reserved */
}RBLE_DIS11_CONTENT;

typedef struct RBLE_BAS_CONTENT_t
{
    uint16_t    shdl;                           /* Service start handle */
    uint16_t    ehdl;                           /* Service end handle */
    uint16_t    battery_lvl_char_hdl;           /* Battery Level char handle */
    uint16_t    battery_lvl_val_hdl;            /* Battery Level value handle */
    uint16_t    battery_lvl_cfg_hdl;            /* Battery Level configuration characteristic value handle */
    uint16_t    battery_lvl_rep_ref_hdl;        /* Battery Level report reference value handle */
    uint8_t     battery_lvl_prop;               /* Battery Level properties */
    uint8_t     reserved;	                    /* Reserved */
}RBLE_BAS_CONTENT;

typedef struct RBLE_BATS_CONTENT_t
{
    uint16_t    shdl;                           /* Service start handle */
    uint16_t    ehdl;                           /* Service end handle */
    uint16_t    battery_lvl_char_hdl;           /* Battery Level char handle */
    uint16_t    battery_lvl_val_hdl;            /* Battery Level value handle */
    uint16_t    battery_lvl_cfg_hdl;            /* Battery Level configuration characteristic value handle */
    uint8_t     battery_lvl_prop;               /* Battery Level properties */
    uint8_t     reserved;	                    /* Reserved */
}RBLE_BATS_CONTENT;

/* ------------------ Scan Parameters Profile ------------------*/

/* Scan Parameter */
typedef struct RBLE_SPP_SERVER_PARAM_t
{
	uint16_t	s_refresh_ntf_en;               /* Scan refresh notification configuration */
}RBLE_SPP_SERVER_PARAM;

/* Scan interval window characteristic variable */
typedef struct RBLE_SCANS_INTV_WINDOW_PARAM_t
{
    uint16_t    le_scan_interval;               /* scan interval value */
    uint16_t    le_scan_window;                 /* scan window value */
}RBLE_SCANS_INTV_WINDOW_PARAM;

typedef struct RBLE_SPS_CONTENT_t
{
    uint16_t    shdl;                           /* Service start handle */
    uint16_t    ehdl;                           /* Service end handle */
    uint16_t    intv_window_char_hdl;           /* Scan Interval Window char handle */
    uint16_t    intv_window_val_hdl;            /* Scan Interval Window value handle */
    uint8_t     intv_window_prop;               /* Scan Interval Window properties */
    uint8_t     reserved1;	                    /* Reserved */
    uint16_t    refresh_char_hdl;               /* Scan Refresh char handle */
    uint16_t    refresh_val_hdl;                /* Scan Refresh value handle */
    uint16_t    refresh_cfg_hdl;                /* Scan Refresh configuration characteristic value handle */
    uint8_t     refresh_prop;                   /* Scan Refresh properties */
    uint8_t     reserved2;	                    /* Reserved */
}RBLE_SPS_CONTENT;

/* ------------------ Heart Rate Profile ------------------*/

/* Heart Rate Parameter */
typedef struct RBLE_HRP_SENSOR_PARAM_t
{
    uint16_t hrtrate_meas_ntf_en;     /* Heart Rate measurement indication configuration */
}RBLE_HRP_SENSOR_PARAM;

/* Heart Rate Measurements Info */
typedef struct RBLE_HRP_MEASUREMENTS_INFO_t
{
    uint8_t  flags;					/* flags */
    uint8_t  rr_interval_num;		/* rr interval num */
    uint16_t heart_rate_measure;	/* heart rate value -measurement */
    uint16_t energy_expended;		/* energy(J) */
    uint16_t rr_interval[RBLE_HRP_RR_INTERVAL_MAX];	/* rr interval(s) */
}RBLE_HRP_MEASUREMENTS_INFO;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_HRS_CONTENT_t
{
    uint16_t shdl;
    uint16_t ehdl;

    uint16_t hrtrate_meas_char_hdl;
    uint16_t hrtrate_meas_val_hdl;
    uint16_t hrtrate_meas_cfg_hdl;
    uint8_t  hrtrate_meas_prop;
    uint8_t  reserved1;

    uint16_t body_sensor_loc_char_hdl;
    uint16_t body_sensor_loc_val_hdl;
    uint8_t  body_sensor_loc_prop;
    uint8_t  reserved2;

    uint16_t hrtrate_cp_char_hdl;
    uint16_t hrtrate_cp_val_hdl;
    uint8_t  hrtrate_cp_prop;
    uint8_t  reserved3;
}RBLE_HRS_CONTENT;

/* ------------------ Cycling Speed and Cadence Profile ------------------*/

/* Cycling Speed and Cadence Parameter */
typedef struct RBLE_CSCP_SENSOR_PARAM_t {
    uint16_t	cycspd_meas_ntf_en;
    uint16_t	sc_cp_ind_en;
    uint8_t		sensor_location;
    uint8_t		reserved;
} RBLE_CSCP_SENSOR_PARAM;

/* Cycling Speed and Cadence measurements Info */
typedef struct RBLE_CSCP_MEASUREMENTS_INFO_t {
    uint8_t		flags;
    uint8_t		reserved;
    uint32_t	wheel_revolutions;
    uint16_t	wheel_event_time;
    uint16_t	crank_revolutions;
    uint16_t	crank_event_time;
} RBLE_CSCP_MEASUREMENTS_INFO;

/* Cycling Speed and Cadence SC control point Info */
typedef struct RBLE_CSCP_SC_CONTROL_POINT_INFO_t {
    uint8_t		OpCode;
    uint8_t		reserved1;
    uint32_t	cumulative_value;
    uint8_t		sensor_location;
    uint8_t		request_op_code;
    uint8_t		response_value;
    uint8_t		reserved2;
}RBLE_CSCP_SC_CONTROL_POINT_INFO;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_CSCS_CONTENT_t{	
    uint16_t	shdl;
    uint16_t	ehdl;

    uint16_t	cycspd_meas_char_hdl;
    uint16_t	cycspd_meas_val_hdl;
    uint16_t	cycspd_meas_cfg_hdl;
    uint8_t		cycspd_meas_prop;
    uint8_t		reserved1;

    uint16_t	csc_feature_char_hdl;
    uint16_t	csc_feature_val_hdl;
    uint8_t		csc_feature_prop;
    uint8_t		reserved2;

    uint16_t	sensor_loc_char_hdl;
    uint16_t	sensor_loc_val_hdl;
    uint8_t		sensor_loc_prop;
    uint8_t		reserved3;

    uint16_t	sc_cp_char_hdl;
    uint16_t	sc_cp_val_hdl;
    uint16_t	sc_cp_cfg_hdl;
    uint8_t		sc_cp_prop;
    uint8_t		reserved4;
}RBLE_CSCS_CONTENT;	

/* ------------------ Glucose Profile ------------------*/

/* Glucose Parameter */
typedef struct RBLE_GLP_SENSOR_PARAM_t {
    uint16_t	glp_meas_ntf_en;
    uint16_t	glp_meas_context_ntf_en;
    uint16_t	ra_cp_ind_en;
} RBLE_GLP_SENSOR_PARAM;

/* Glucose measurements Info */
typedef struct RBLE_GLP_MEASUREMENTS_INFO_t {
    uint8_t			flags;
    uint8_t			reserved;
    uint16_t		seq_num;
    RBLE_DATE_TIME	stamp;
    int16_t			time_offset;
    int16_t			concentration;
    uint8_t			type;
    uint8_t			sample_location;
    uint16_t		sensor_status_annun;
} RBLE_GLP_MEASUREMENTS_INFO;

/* Glucose measurements context Info */
typedef struct RBLE_GLP_MEASUREMENTS_CONTEXT_INFO_t {
    uint8_t			flags;
    uint8_t			reserved1;
    uint16_t		seq_num;
    uint8_t			ex_flags;
    uint8_t			carbohydrate_id;
    int16_t			carbohydrate_kg;
    uint8_t			meal;
    uint8_t			tester;
    uint8_t			health;
    uint8_t			reserved2;
    uint16_t		exercise_duration;
    uint8_t			exercise_intensity;
    uint8_t			medication_id;
    int16_t			medication;
    int16_t			HbA1c;
} RBLE_GLP_MEASUREMENTS_CONTEXT_INFO;

/* Glucose RA control point Info */
typedef struct RBLE_GLP_RA_CONTROL_POINT_INFO_t {
    uint8_t			OpCode;
    uint8_t			racp_operator;
    uint8_t			operand_value;
    uint8_t			reserved;
    uint16_t			min_sequence_num;
    uint16_t			max_sequence_num;
    RBLE_DATE_TIME	min_stamp;
    RBLE_DATE_TIME	max_stamp;
}RBLE_GLP_RA_CONTROL_POINT_INFO;

/* Glucose RA control point response Info */
typedef struct RBLE_GLP_RA_CONTROL_POINT_IND_INFO_t {
	uint8_t			OpCode;
	uint8_t			racp_operator;
	uint16_t		num_of_records;
	uint8_t			request_op_code;
	uint8_t			response_code_value;
} RBLE_GLP_RA_CONTROL_POINT_IND_INFO;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_GLS_CONTENT_t{
    uint16_t	shdl;
    uint16_t	ehdl;

    uint16_t	glucose_meas_char_hdl;
    uint16_t	glucose_meas_val_hdl;
    uint16_t	glucose_meas_cfg_hdl;
    uint8_t		glucose_meas_prop;
    uint8_t		reserved1;

    uint16_t	glucose_meas_context_char_hdl;
    uint16_t	glucose_meas_context_val_hdl;
    uint16_t	glucose_meas_context_cfg_hdl;
    uint8_t		glucose_meas_context_prop;
    uint8_t		reserved2;

    uint16_t	glucose_feature_char_hdl;
    uint16_t	glucose_feature_val_hdl;
    uint8_t		glucose_feature_prop;
    uint8_t		reserved3;

    uint16_t	glucose_ra_cp_char_hdl;
    uint16_t	glucose_ra_cp_val_hdl;
    uint16_t	glucose_ra_cp_cfg_hdl;
    uint8_t		glucose_ra_cp_prop;
    uint8_t		reserved4;
}RBLE_GLS_CONTENT;

/* ------------------ Cycling Power Profile ------------------*/
/* CP Parameter */
typedef struct RBLE_CPP_SENSOR_PARAM_t {
    uint16_t	cp_meas_ntf_en;			/* Cycling Power measurement notification configuration */
    uint16_t	cp_meas_brd_en;			/* Cycling Power measurement broadcast configuration */
    uint16_t	cp_vector_ntf_en;		/* Cycling Power vector notification configuration */
    uint16_t	cp_cp_ind_en;			/* CP control point indicat configuration */
    uint16_t	battery_level_ntf_en;	/* Battery Level notification configuration */
    uint8_t		sensor_location;		/* Sensor Location */
    uint8_t		reserved;				/* Reserved */
} RBLE_CPP_SENSOR_PARAM;

/* Cycling Power Measurements Info */
typedef struct RBLE_CPP_MEASUREMENTS_INFO_t {
    uint16_t    flags;					/* flags */
    int16_t     instant_power;			/* Instantaneous power */
    uint8_t     pedal_balance;			/* Pedal Power Balance */
    uint8_t     reserved;				/* Reserved */
    uint16_t    accumulated_torque;		/* Accumulated Torque */
    uint32_t    wheel_revolutions;		/* Wheel Revolution Data(Cumulative Wheel Revolutions) */
    uint16_t    wheel_event;			/* Wheel Revolution Data(Last Wheel Event Time) */
    uint16_t    crank_revolutions;		/* Crank Revolution Data(Cumulative Crank Revolutions) */
    uint16_t    crank_event;			/* Crank Revolution Data(Last Crank Event Time) */
    int16_t     max_force_magnitude;	/* Extreme Force Magnitudes(Maximum Force Magnitude) */
    int16_t     min_force_magnitude;	/* Extreme Force Magnitudes(Minimum Force Magnitude) */
    int16_t     max_torque_magnitude;	/* Extreme Torque Magnitudes(Maximum Torque Magnitude) */
    int16_t     min_torque_magnitude;	/* Extreme Torque Magnitudes(Minimum Torque Magnitude) */
    uint16_t    max_angle;				/* Extreme Angles(Maximum Angle) */
    uint16_t    min_angle;				/* Extreme Angles(Minimum Angle) */
    uint16_t    top_dead_spot;			/* Top Dead Spot Angle */
    uint16_t    bottom_dead_spot;		/* Bottom Dead Spot Angle */
    uint16_t    accumulated_energy;		/* Accumulated Energy */
} RBLE_CPP_MEASUREMENTS_INFO;

/* Cycling Power Vector Info */
typedef struct RBLE_CPP_VECTOR_INFO_t {
    uint8_t     flags;					/* flags */
    uint8_t     reserved1;				/* Reserved */
    uint16_t    crank_revolutions;		/* Crank Revolution Data(Cumulative Crank Revolution) */
    uint16_t    crank_event;			/* Crank Revolution Data(Last Crank Event Time) */
    uint16_t    first_crank_angle;		/* First Crank Measurement Angle */
    uint8_t     array_num;				/* number of magnitude array */
    uint8_t     reserved2;				/* Reserved */
    int16_t     magnitude[RBLE_CPP_MAGNITUDE_MAX];	/* Instantaneous Torque Magnitude Array */
} RBLE_CPP_VECTOR_INFO;

/* Cycling Power Control Point Info(Write characteristic) */
typedef struct RBLE_CPP_WR_CONTROL_POINT_INFO_t {
    uint8_t     OpCode;				/* Operation Code */
    uint8_t     reserved1;			/* reserved */
    uint32_t    cumulative_value;	/* cumulative value */
    uint8_t     sensor_location;	/* sensor location */
    uint8_t     reserved2;			/* reserved */
    uint16_t    crank_length;		/* crank lenght */
    uint16_t    chain_length;		/* chain lenght */
    uint16_t    chain_weight;		/* chain weight */
    uint16_t    span_length;		/* span length */
    uint16_t    mask_meas_content;	/* mask cycling power measurement characteristic content */
}RBLE_CPP_WR_CONTROL_POINT_INFO;

/* Cycling Power Control Point Info(Indication) */
typedef struct RBLE_CPP_IND_CONTROL_POINT_INFO_t {
    uint8_t     OpCode;					/* Operation Code */
    uint8_t     request_op_code;		/* Request Operation Code */
    uint8_t     response_value;			/* Response Value */
    uint8_t     reserved1;				/* Operation Code */
    uint16_t    crank_length;			/* crank lenght */
    uint16_t    chain_length;			/* chain lenght */
    uint16_t    chain_weight;			/* chain weight */
    uint16_t    span_length;			/* span length */
    int16_t     offset_compensation;	/*  */
    uint8_t     sampling_rate;			/* sampling rate */
    uint8_t     reserved2;				/* Operation Code */
    RBLE_DATE_TIME  stamp;				/* time stamp */
}RBLE_CPP_IND_CONTROL_POINT_INFO;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_CPS_CONTENT_t{
    uint16_t    shdl;
    uint16_t    ehdl;
    uint16_t    cp_meas_char_hdl;
    uint16_t    cp_meas_val_hdl;
    uint16_t    cp_meas_cfg_hdl;
    uint16_t    cp_meas_brd_cfg_hdl;
    uint8_t     cp_meas_prop;
    uint8_t     reserved1;
    uint16_t    cp_feature_char_hdl;
    uint16_t    cp_feature_val_hdl;
    uint8_t     cp_feature_prop;
    uint8_t     reserved2;
    uint16_t    sensor_loc_char_hdl;
    uint16_t    sensor_loc_val_hdl;
    uint8_t     sensor_loc_prop;
    uint8_t     reserved3;
    uint16_t    cp_vector_char_hdl;
    uint16_t    cp_vector_val_hdl;
    uint16_t    cp_vector_cfg_hdl;
    uint8_t     cp_vector_prop;
    uint8_t     reserved4;
    uint16_t    cp_cp_char_hdl;
    uint16_t    cp_cp_val_hdl;
    uint16_t    cp_cp_cfg_hdl;
    uint8_t     cp_cp_prop;
    uint8_t     reserved5;
}RBLE_CPS_CONTENT;	

/* ------------------------- Time Profile-------------------------------*/

/* Current Time structure */
typedef struct RBLE_TIP_CURRENT_TIME_t {
	RBLE_DATE_TIME	stamp;
	uint8_t			day_of_week;
	uint8_t			fractions256;
	uint8_t			adjust_reason;
	uint8_t			reserved;
} RBLE_TIP_CURRENT_TIME;

/* Local Time Information structure */
typedef struct RBLE_TIP_LOCAL_TIME_INFO_t {
	int8_t		time_zone;
	uint8_t		dst_offset;
} RBLE_TIP_LOCAL_TIME_INFO;

/* Reference Time Information structure */
typedef struct RBLE_TIP_REFERENCE_TIME_INFO_t {
	uint8_t		time_source;
	uint8_t		accuracy;
	uint8_t		days_since_update;
	uint8_t		hours_since_update;
}RBLE_TIP_REFERENCE_TIME_INFO;

/* Next Time with DST Information structure */
typedef struct RBLE_TIP_NEXT_TIME_DST_INFO_t {
	RBLE_DATE_TIME		stamp;
	uint8_t				dst_offset;
	uint8_t				reserved;
}RBLE_TIP_NEXT_TIME_DST_INFO;

/* Reference Time Update State structure */
typedef struct RBLE_TIP_TIME_UPDATE_STATE_t {
	uint8_t		current_state;
	uint8_t		update_result;
}RBLE_TIP_TIME_UPDATE_STATE;

/* Write Data Dase structure */
typedef struct RBLE_TIPS_WR_DATA_t {
	union Write_Tips_Parameter_u {
		RBLE_TIP_CURRENT_TIME 			current_time;
		RBLE_TIP_LOCAL_TIME_INFO		local_time;
		RBLE_TIP_REFERENCE_TIME_INFO	ref_time;
		RBLE_TIP_NEXT_TIME_DST_INFO		next_dst;
		RBLE_TIP_TIME_UPDATE_STATE		update_state;
	}param;
}RBLE_TIPS_WR_DATA;

/* Structure containing the characteristics handles, value handles and descriptors (Current Time Service) */
typedef struct RBLE_CTS_CONTENT_t{
	uint16_t 	shdl;
	uint16_t 	ehdl;

	uint16_t 	current_time_char_hdl;
	uint16_t 	current_time_val_hdl;
	uint16_t 	current_time_cfg_hdl;
	uint8_t  	current_time_prop;
	uint8_t  	reserved1;

	uint16_t 	local_time_info_char_hdl;
	uint16_t 	local_time_info_val_hdl;
	uint8_t 	local_time_info_prop;
	uint8_t  	reserved2;

	uint16_t 	ref_time_info_char_hdl;
	uint16_t 	ref_time_info_val_hdl;
	uint8_t 	ref_time_info_prop;
	uint8_t  	reserved3;
}RBLE_CTS_CONTENT;

/* Structure containing the characteristics handles, value handles and descriptors (Next DST Change Service) */
typedef struct RBLE_NDCS_CONTENT_t{
	uint16_t 	shdl;
	uint16_t 	ehdl;

	uint16_t 	time_dst_char_hdl;
	uint16_t 	time_dst_val_hdl;
	uint8_t  	time_dst_prop;
	uint8_t  	reserved1;
}RBLE_NDCS_CONTENT;

/* Structure containing the characteristics handles, value handles and descriptors (Reference Time Update Service) */
typedef struct RBLE_RTUS_CONTENT_t{
	uint16_t 	shdl;
	uint16_t 	ehdl;

	uint16_t 	update_cp_char_hdl;
	uint16_t 	update_cp_val_hdl;
	uint8_t  	update_cp_prop;
	uint8_t  	reserved1;

	uint16_t 	update_state_char_hdl;
	uint16_t 	update_state_val_hdl;
	uint8_t  	update_state_prop;
	uint8_t  	reserved2;
}RBLE_RTUS_CONTENT;

/* ------------------------- Alert Notification Profile-------------------------------*/

/* Alert Notification Service structure */
typedef struct RBLE_ANP_SERVER_PARAM_t {
	uint16_t		new_alert_ntf_en;
	uint16_t		unread_alert_ntf_en;
}RBLE_ANP_SERVER_PARAM;

/* New Alert Notification Information structure */
typedef struct RBLE_ANP_NEW_ALERT_INFO_t {
	uint8_t			category_id;
	uint8_t			alert_num;
	uint8_t			text_size;
	uint8_t			text[RBLE_ANP_ALERT_TEXT_MAX];
}RBLE_ANP_NEW_ALERT_INFO;

/* Unread Alert Status Information structure */
typedef struct RBLE_ANP_UNREAD_ALERT_INFO_t {
	uint8_t			category_id;
	uint8_t			unread_count;
}RBLE_ANP_UNREAD_ALERT_INFO;

/* Alert Notification Control Point structure */
typedef struct RBLE_ANP_ALERT_NTF_CP_t {
	uint8_t			command_id;
	uint8_t			category_id;
}RBLE_ANP_ALERT_NTF_CP;

/* Structure containing the characteristics handles, value handles and descriptors (Alert Notification Service) */
typedef struct RBLE_ANS_CONTENT_t{
	uint16_t 	shdl;
	uint16_t 	ehdl;

	uint16_t 	supp_new_alert_char_hdl;
	uint16_t 	supp_new_alert_val_hdl;
	uint8_t  	supp_new_alert_prop;
	uint8_t  	reserved1;

	uint16_t 	new_alert_char_hdl;
	uint16_t 	new_alert_val_hdl;
	uint16_t 	new_alert_cfg_hdl;
	uint8_t 	new_alert_prop;
	uint8_t  	reserved2;

	uint16_t 	supp_unread_alert_char_hdl;
	uint16_t 	supp_unread_alert_val_hdl;
	uint8_t  	supp_unread_alert_prop;
	uint8_t  	reserved3;

	uint16_t 	unread_alert_char_hdl;
	uint16_t 	unread_alert_val_hdl;
	uint16_t 	unread_alert_cfg_hdl;
	uint8_t 	unread_alert_prop;
	uint8_t  	reserved4;

	uint16_t 	alert_ntf_cp_char_hdl;
	uint16_t 	alert_ntf_cp_val_hdl;
	uint8_t 	alert_ntf_cp_prop;
	uint8_t  	reserved5;
}RBLE_ANS_CONTENT;

/* ------------------------- Location and Navigation Profile-------------------------------*/
typedef struct RBLE_LNP_SENSOR_PARAM_t {
    uint16_t	location_speed_ntf_en;	/* Location and Speed notification configuration */
    uint16_t	ln_cp_ind_en;			/* LN control point indication configuration */
    uint16_t	navigation_ntf_en;		/* Navigation notification configuration */
    uint16_t	battery_level_ntf_en;	/* Battery Level notification configuration */
} RBLE_LNP_SENSOR_PARAM;

/* Location and Speed Info */
typedef struct RBLE_LNP_LOCATION_SPEED_INFO_t {
    uint16_t	flags;					/* flags */
    uint16_t	instant_speed;			/* Instantaneous Speed */
    uint32_t	total_distance;			/* Total Distance */
    int32_t		latitude;				/* Location - Latitude */
    int32_t		longitude;				/* Location - Longitude */
    int32_t		elevation;				/* Elevation */
    uint16_t	heading;				/* Heading */
    uint8_t		rolling_time;			/* Rolling Time */
    uint8_t		reserved;				/* reserved */
    RBLE_DATE_TIME  utc_time;			/* UTC Time */
} RBLE_LNP_LOCATION_SPEED_INFO;

/* Position Quality Info */
typedef struct RBLE_LNP_POSITION_QUALITY_INFO_t {
	uint16_t	flags;
	uint8_t		beacon_solution_num;
	uint8_t		beacon_view_num;
	uint16_t	first_fix_time;	
	uint32_t	ehpe;
	uint32_t	evpe;
	uint8_t		hdop;
	uint8_t		vdop;
} RBLE_LNP_POSITION_QUALITY_INFO;

/* LN Control Point Info(Write characteristic) */
typedef struct RBLE_LNP_WR_CONTROL_POINT_INFO_t {
    uint8_t		OpCode;					/* flags */
    uint8_t		reserved;				/* reserved */
    uint32_t	cumulative_value;		/* Set Cumulative */
    uint16_t	content_mask;			/* Mask Location and Speed Characteristic Content */
    uint16_t	route_num;				/* Number of Route */
    uint8_t		control_val;			/* Navigation Control */
    uint8_t		fix_rate;				/* Set Fix Rate */
    int32_t		elevation;				/* Set Elevation */
} RBLE_LNP_WR_CONTROL_POINT_INFO;

/* LN Control Point Info(Indication) */
typedef struct RBLE_LNP_IND_CONTROL_POINT_INFO_t {
    uint8_t		OpCode;					/* Operation Code */
    uint8_t		request_op_code;		/* Request Operation Code */
    uint8_t		response_value;			/* Response Value */
    uint8_t		reserved;				/* reserved */
    uint16_t	route_num;				/* Number of route */
    uint8_t		name_size;				/* Size of route name */
    uint8_t		route_name[RBLE_LNP_SENSOR_ROUTE_NAME_MAX];	/* route name */
}RBLE_LNP_IND_CONTROL_POINT_INFO;

/* Navigation Info */
typedef struct RBLE_LNP_NAVIGATION_INFO_t{
    uint16_t	flags;					/* flags */
    uint16_t	bearing;				/* Bearing */
    uint16_t	heading;				/* Heading */
    uint32_t	remain_dis;				/* Remaining Distance */
    int32_t		remain_v_dis;			/* Remaining Vertical Distance */
    RBLE_DATE_TIME	estimate_time;		/* Estimated Time of Arrival */
} RBLE_LNP_NAVIGATION_INFO;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_LNS_CONTENT_t{
    uint16_t	shdl;						/* Service start handle */
    uint16_t	ehdl;						/* Service end handle */
    uint16_t	ln_feature_char_hdl;		/* LN feature char handle */
    uint16_t	ln_feature_val_hdl;			/* LN feature value handle */
    uint8_t		ln_feature_prop;			/* LN feature properties */
    uint8_t		reserved1;
    uint16_t	location_speed_char_hdl;	/* Location and Speed char handle */
    uint16_t	location_speed_val_hdl; 	/* Location and Speed value handle */
    uint16_t	location_speed_cfg_hdl;		/* Location and Speed configuration characteristic value handle */
    uint8_t		location_speed_prop;		/* Location and Speed properties */
    uint8_t		reserved2;
    uint16_t	position_quality_char_hdl;	/* Position quality char handle */
    uint16_t	position_quality_val_hdl;	/* Position quality value handle */
    uint8_t		position_quality_prop;		/* Position quality properties */
    uint8_t		reserved3;
    uint16_t	ln_cp_char_hdl;				/* LN Control Point char handle */
    uint16_t	ln_cp_val_hdl;				/* LN Control Point value handle */
    uint16_t	ln_cp_cfg_hdl;				/* LN Control Point configuration characteristic value handle */
    uint8_t		ln_cp_prop;					/* LN Control Point properties */
    uint8_t		reserved4;
    uint16_t	navigation_char_hdl;		/* Navigation char handle */
    uint16_t	navigation_val_hdl;			/* Navigation value handle */
    uint16_t	navigation_cfg_hdl;			/* Navigation configuration characteristic value handle */
    uint8_t		navigation_prop;			/* Navigation properties */
    uint8_t		reserved5;
}RBLE_LNS_CONTENT;

/* ------------------ Phone Alert Status ------------------*/
/* Phone Alert Status Parameter */
typedef struct RBLE_PASP_SERVER_PARAM_t
{
    uint16_t alert_status_ntf_en;     /* alert status notification configuration */
    uint16_t ringer_setting_ntf_en;   /* ringer setting notification configuration */
}RBLE_PASP_SERVER_PARAM;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_PASS_CONTENT_t
{
    uint16_t shdl;
    uint16_t ehdl;
    uint16_t alert_status_char_hdl;
    uint16_t alert_status_val_hdl;
    uint16_t alert_status_cfg_hdl;
    uint8_t  alert_status_prop;
    uint8_t  reserved;
    uint16_t ringer_setting_char_hdl;
    uint16_t ringer_setting_val_hdl;
    uint16_t ringer_setting_cfg_hdl;
    uint8_t  ringer_setting_prop;
    uint8_t  reserved2;
    uint16_t ringer_cp_char_hdl;
    uint16_t ringer_cp_val_hdl;
    uint8_t  ringer_cp_prop;
    uint8_t  reserved3;
}RBLE_PASS_CONTENT;

/* ------------------ Runnning Speed and Cadence ------------------*/

typedef struct RBLE_RSCP_SENSOR_PARAM_t {
    uint16_t	rsc_meas_ntf_en;		/* RSC Measurement notification configuration */
    uint16_t	sc_cp_ind_en;			/* SC control point indicat configuration */
    uint8_t		sensor_location;		/* Sensor Location */
    uint8_t		reserved;				/* Reserved */
} RBLE_RSCP_SENSOR_PARAM;

/* RSC Measurements Info */
typedef struct RBLE_RSCP_MEASUREMENTS_INFO_t {
	uint8_t		flags;					/* flags */
	uint8_t		instant_cadence;		/* Instantaneous Cadence */
	uint16_t	instant_speed;			/* Instantaneous speed */
	uint16_t	instant_stride_len;		/* Instantaneous stride length */
	uint32_t	total_distance;			/* Total Distance */
} RBLE_RSCP_MEASUREMENTS_INFO;

/* SC Control Point Info */
typedef struct RBLE_RSCP_SC_CONTROL_POINT_INFO_t {
	uint8_t		OpCode;				/* Operation Code */
	uint8_t		reserved1;			/* reserved */
	uint32_t	cumulative_value;	/* cumulative value */
	uint8_t		sensor_location;	/* sensor location */
	uint8_t		request_op_code;	/* Request Operation Code */
	uint8_t		response_value;		/* Response Value */
	uint8_t		reserved;			/* Reserved */
}RBLE_RSCP_SC_CONTROL_POINT_INFO;

/* Structure containing the characteristics handles, value handles and descriptors */
typedef struct RBLE_RSCS_CONTENT_t{
    uint16_t    shdl;
    uint16_t    ehdl;
    uint16_t    rsc_meas_char_hdl;
    uint16_t    rsc_meas_val_hdl;
    uint16_t    rsc_meas_cfg_hdl;
    uint8_t     rsc_meas_prop;
    uint8_t     reserved1;
    uint16_t    rsc_feature_char_hdl;
    uint16_t    rsc_feature_val_hdl;
    uint8_t     rsc_feature_prop;
    uint8_t     reserved2;
    uint16_t    sensor_loc_char_hdl;
    uint16_t    sensor_loc_val_hdl;
    uint8_t     sensor_loc_prop;
    uint8_t     reserved3;
    uint16_t    sc_cp_char_hdl;
    uint16_t    sc_cp_val_hdl;
    uint16_t    sc_cp_cfg_hdl;
    uint8_t     sc_cp_prop;
    uint8_t     reserved4;
}RBLE_RSCS_CONTENT;

/* ------------------ Vendor Specific ------------------*/

/* Data Flash Access Parameter */
typedef struct RBLE_VS_FLASH_ACCESS_PARAM_t
{
	uint8_t cmd;
	uint8_t id; 
	uint8_t size;
	uint8_t reserved;
	uint8_t *addr;
} RBLE_VS_FLASH_ACCESS_PARAM;

/******************************* Call Back Define **********************************/

// Device Mode Setting Call Back Define

enum RBLE_GAP_EVENT_TYPE_enum {
	RBLE_GAP_EVENT_RESET_RESULT = 1,					/* Reset result Complete Event */
	RBLE_GAP_EVENT_SET_NAME_COMP,						/* Set name Complete Event */
	RBLE_GAP_EVENT_OBSERVATION_ENABLE_COMP,				/* Observation enable Complete Event */
	RBLE_GAP_EVENT_OBSERVATION_DISABLE_COMP,			/* Observation disable Complete Event */
	RBLE_GAP_EVENT_BROADCAST_ENABLE_COMP,				/* Broadcast enable Complete Event */
	RBLE_GAP_EVENT_BROADCAST_DISABLE_COMP,				/* Broadcast disable Complete Event */
	RBLE_GAP_EVENT_SET_BONDING_MODE_COMP,				/* Set bonding mode Complete Event */
	RBLE_GAP_EVENT_SET_SECURITY_REQUEST_COMP,			/* Set security request Complete Event */
	RBLE_GAP_EVENT_GET_DEVICE_INFO_COMP,				/* Get device info Complete Event */
	RBLE_GAP_EVENT_GET_WHITE_LIST_SIZE_COMP,			/* Get white list size Complete Event */
	RBLE_GAP_EVENT_ADD_TO_WHITE_LIST_COMP,				/* Add to white list Complete Event */
	RBLE_GAP_EVENT_DEL_FROM_WHITE_LIST_COMP,			/* Del from white list Complete Event */
	RBLE_GAP_EVENT_GET_REMOTE_DEVICE_NAME_COMP,			/* Get remote device name Complete Event */
	RBLE_GAP_EVENT_GET_REMOTE_DEVICE_INFO_COMP,			/* Get remote device info Complete Event */
	RBLE_GAP_EVENT_DEVICE_SEARCH_COMP,					/* Device search Complete Event */
	RBLE_GAP_EVENT_DEVICE_SEARCH_RESULT_IND,			/* Device search result Complete Event */
	RBLE_GAP_EVENT_RPA_RESOLVED,                		/* RPA resolution procedure Complete Event */
	RBLE_GAP_EVENT_SET_RANDOM_ADDRESS_COMP,				/* Set random address Complete Event */
	RBLE_GAP_EVENT_SET_PRIVACY_FEATURE_COMP,			/* Set privacy feature Complete Event */
	RBLE_GAP_EVENT_CONNECTION_COMP,						/* Connection Complete Event */
	RBLE_GAP_EVENT_CONNECTION_CANCEL_COMP,				/* Connection cancel Complete Event */
	RBLE_GAP_EVENT_DISCONNECT_COMP,						/* Disconnect Complete Event */
	RBLE_GAP_EVENT_ADVERTISING_REPORT_IND,				/* Advertising report Complete Event */
	RBLE_GAP_EVENT_BONDING_COMP,						/* Bonding Complete Event */
	RBLE_GAP_EVENT_BONDING_REQ_IND,						/* Bonding req Complete Event */
	RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_REQ_IND,		/* Change connectionparam req Complete Event */
	RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_COMP,		/* Change connection param Complete Event */
	RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_RESPONSE,	/* Change connection param response Complete Event */
	RBLE_GAP_EVENT_CHANNEL_MAP_REQ_COMP,				/* Channel map req Complete Event */
	RBLE_GAP_EVENT_READ_RSSI_COMP,						/* Read RSSI Complete Event */
	RBLE_GAP_EVENT_WR_CHAR_IND,							/* Write Characteristic Indication Event */
	RBLE_GAP_EVENT_COMMAND_DISALLOWED_IND				/* Command disallowed Complete Event */
};

typedef uint8_t		RBLE_GAP_EVENT_TYPE;

typedef struct RBLE_GAP_EVENT_t {
	RBLE_GAP_EVENT_TYPE		type;			/* Event type */
	uint8_t					reserved;
	union Event_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;
		
		/* RBLE_EVT_GAP_Reset_Result */
		struct RBLE_GAP_Reset_Result_t {
			RBLE_STATUS status;
		    uint8_t  rBLE_major_ver;		/* rBLE Major Version */
		    uint8_t  rBLE_minor_ver;        /* rBLE Minor Version */
		}reset_result;

		/* RBLE_EVT_GAP_Set_Security_Request_Comp */
		struct RBLE_GAP_Set_Security_Request_t{
			RBLE_STATUS    status;
			uint8_t        sec;
		}set_sec_req;
		
		/* RBLE_EVT_GAP_Get_Device_Info_Comp */
		struct RBLE_GAP_Get_Device_Info_t{
			RBLE_STATUS    status;
			RBLE_BD_ADDR   addr;
			uint8_t        reserved;
			RBLE_DEVICE_VER_INFO ver_info;
		}get_dev_ver;

		/* RBLE_EVT_GAP_Get_White_List_Size_Comp */
		struct RBLE_GAP_Get_Wlst_size_t{
			RBLE_STATUS    status;
			uint8_t        wlist_size;
		}get_wlst_size;

		/* RBLE_EVT_GAP_Get_Remote_Device_Name_Comp */
		struct RBLE_GAP_Get_Remote_Device_Name_t{
			RBLE_STATUS    status;
			RBLE_BD_NAME   bd_name;
			uint8_t        reserved;
		}get_remote_dev_name;

		/* RBLE_EVT_GAP_Get_Remote_Device_Info_Comp */
		struct RBLE_GAP_GET_Remote_Device_Info_t{
			RBLE_STATUS    status;
			uint8_t        reserved;
			uint16_t       conhdl;
			uint16_t       vers;
			uint16_t       compid;
			uint16_t       subvers;
			RBLE_FEATURES  feats_used;
		}get_remote_dev_info;

		/* RBLE_EVT_GAP_Device_Search_Result_Ind */
		struct RBLE_GAP_Device_Search_Result_t{
			RBLE_ADV_REPORT adv_resp;
		}dev_search_result;

		/* RBLE_GAP_RPA_Resolved_Evt_t */
		struct RBLE_GAP_RPA_Resolved_Evt_t{
			RBLE_BD_ADDR    res_addr;
			uint8_t         res_addr_type;
			RBLE_BD_ADDR    addr;
			uint8_t         addr_type;
		}rpa_resolved;

		/* RBLE_EVT_GAP_Set_Random_Address_Comp */
		struct RBLE_GAP_Set_Random_Address_t{
			RBLE_STATUS    status;
			RBLE_BD_ADDR   addr;
		}set_rand_adr;

		/* RBLE_EVT_GAP_Connection_Comp */
		struct RBLE_GAP_Connection_t{
			RBLE_CONNECT_INFO connect_info;
		}conn_comp;

		/* RBLE_EVT_GAP_Disconnect_Comp */
		struct RBLE_GAP_Disconnect_t{
			uint8_t        reason;
			RBLE_STATUS    status;
			uint16_t       conhdl;
		}disconnect;

		/* RBLE_EVT_GAP_Advertising_Report_Ind */
		struct RBLE_GAP_Advertising_Report_t{
			RBLE_ADV_REPORT_EVT evt;
			uint8_t    		    reserved;
		}adv_report;

		/* RBLE_EVT_GAP_Bonding_Comp */
		struct RBLE_GAP_Bonding_Comp_t{
			uint16_t conhdl;
			uint8_t idx;
			RBLE_STATUS status;
			uint8_t key_size;
			uint8_t sec_prop;
		}bonding_comp;

		/* RBLE_EVT_GAP_Bonding_Req_Ind */
		struct RBLE_GAP_Bonding_Req_t{
			RBLE_BD_ADDR  bd_addr;
			uint8_t       index;
		    uint8_t       auth_req;
		    uint8_t       io_cap;
		    uint8_t       oob_data_flg;
			uint8_t       max_enc_size;
		    uint8_t       ikey_dist;
		    uint8_t       rkey_dist;
		}bonding_req;

		/* RBLE_EVT_GAP_Change_Connection_Param_Req_Ind */
		struct RBLE_GAP_Change_Connection_Param_Req_Ind_t{
			uint16_t         conhdl;
			RBLE_CONN_PARAM  conn_param;
		}chg_connect_param_req;

		/* RBLE_EVT_GAP_Change_Connection_Param_Comp */
		struct RBLE_GAP_Change_Connection_Param_t{
			RBLE_STATUS      status;
			uint8_t          reserved;
			uint16_t         con_interval;
			uint16_t         con_latency;
			uint16_t         sup_to;
		}chg_connect_param;

		/* RBLE_EVT_GAP_CHANGE_CONNECTION_PARAM_RESPONSE */
		struct RBLE_GAP_Change_Connection_Param_Response_t{
			RBLE_STATUS      status;
			uint8_t          reserved;
			uint16_t		result;
			uint16_t		conhdl;
		}chg_connect_param_resp;

		/* RBLE_EVT_GAP_CHANNEL_MAP_REQ_CMP_EVT */
		struct RBLE_GAP_Channel_Map_Req_Cmp_Evt_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
			RBLE_LE_CHNL_MAP chmap;
		}channel_map_req_cmp;

		/* RBLE_EVT_GAP_READ_RSSI_CMP_EVT */
		struct RBLE_GAP_Read_RSSI_Cmp_Evt_t{
			uint16_t	conhdl;
			RBLE_STATUS	status;
			uint8_t		rssi;
		}read_rssi;

		/* RBLE_EVT_GAP_WR_CHAR_IND_EVT */
		struct RBLE_GAP_Wr_Char_Ind_Evt_t{
			uint16_t	conhdl;
			uint16_t	type;
			union {	
				RBLE_BD_NAME	name;
				uint16_t		appearance;
			} param;
		}wr_char;

		/* RBLE_EVT_GAP_CMD_DISALLOWED_IND */
		struct RBLE_GAP_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
			uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_GAP_EVENT;

typedef void ( *RBLE_GAP_EVENT_HANDLER )( RBLE_GAP_EVENT *event );	/* GAP Event CALL BACK Protetype */

// Security Manager Call Back Define

enum RBLE_SM_EVENT_TYPE_enum {
	RBLE_SM_EVENT_SET_CNF = 1,		/* Device Key Setting Complete Event */
	RBLE_SM_ENC_START_IND,			/* Encryption Start Indicate Event */
	RBLE_SM_TK_REQ_IND,				/* TK Request Event */
	RBLE_SM_LTK_REQ_IND,			/* LTK Request for Key Distribution Event */
	RBLE_SM_LTK_REQ_FOR_ENC_IND,	/* LTK Request for Encryption Event */
	RBLE_SM_IRK_REQ_IND,			/* IRK Request Event */
	RBLE_SM_CSRK_REQ_IND,			/* CSRK Request Event */
	RBLE_SM_KEY_IND,				/* Key Indicate Event */
	RBLE_SM_CHK_BD_ADDR_REQ,		/* BD Address Check Request Event */
	RBLE_SM_TIMEOUT_EVT,			/* Timeout Event */
	RBLE_SM_EVENT_COMMAND_DISALLOWED_IND	/* Command Disallowed Event */
};

typedef uint8_t		RBLE_SM_EVENT_TYPE;

typedef struct RBLE_SM_EVENT_t {
	RBLE_SM_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Sm_Parameter_u {
		/* RBLE_EVT_GAP_Set_Device_Mode_Comp */
		struct RBLE_EVT_SM_Set_Cnf_t{
			RBLE_STATUS    status;
			uint8_t        key_code;
		}set_conf;

		/* RBLE_EVT_SM_Sec_Start_Ind */
		struct RBLE_EVT_SM_Sec_Start_t{
			uint8_t		idx;
			RBLE_STATUS	status;
			uint8_t		key_size;
			uint8_t		sec_prop;
		    uint8_t		bonded;
			uint8_t		reserved;
		}sec_start;

		/* RBLE_EVT_SM_Tk_Req_Ind */
		struct RBLE_EVT_SM_Tk_Req_t{
			uint8_t		idx;
			uint8_t		oob_en;
			uint8_t		disp_en;
		}tk_req;

		/* RBLE_EVT_SM_Ltk_Req_Ind */
		struct RBLE_EVT_SM_Ltk_Req_t{
			uint8_t		idx;
			uint8_t		auth_req;
		}ltk_req;

		/* RBLE_EVT_SM_Ltk_Req_For_Enc_Ind */
		struct RBLE_EVT_SM_Ltk_Req_For_Enc_t{
			uint8_t			idx;
			uint8_t			auth_req;
			uint16_t		ediv;
			RBLE_RAND_NB	nb;
		}ltk_req_for_enc;

		/* RBLE_EVT_SM_Irk_Req_Ind */
		struct RBLE_EVT_SM_Irk_Req_t{
			uint8_t		idx;
		}irk_req;

		/* RBLE_EVT_SM_Csrk_Req_Ind */
		struct RBLE_EVT_SM_Csrk_Req_t{
			uint8_t			idx;
			RBLE_BD_ADDR	addr;
			uint8_t        reserved;
			uint32_t		signcnt;
		}csrk_req;
		
		/* RBLE_EVT_SM_Key_Ind */
		struct RBLE_EVT_SM_Key_t{
			uint8_t			idx;
			uint8_t			key_code;
			uint16_t		ediv;
			RBLE_RAND_NB	nb;
			RBLE_KEY_VALUE	ltk;
		}key_ind;

		/* RBLE_EVT_SM_Chk_Bd_Addr_Req_Ind */
		struct RBLE_EVT_SM_Chk_Bd_Addr_Req_t{
			uint8_t			idx;
			uint8_t			type;
			RBLE_BD_ADDR	addr;
		}chk_bdaddr;

		/* RBLE_EVT_SM_TIMEOUT_EVT */
		struct RBLE_EVT_SM_Timeout_Evt_t{
		    uint8_t			idx;
		}timeout_evt;

		/* RBLE_EVT_SM_CMD_DISALLOWED_IND */
		struct RBLE_EVT_SM_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
			uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	} param;
}RBLE_SM_EVENT;

// Security Manager Call back define
typedef void ( *RBLE_SM_EVENT_HANDLER )(RBLE_SM_EVENT *event);	/* SM Event Call Back Protetype */

// GATT Call Back Define

/**
 * @brief	rBLE GATT Event Enumerated Type
 */
enum RBLE_GATT_EVENT_TYPE_enum {
	RBLE_GATT_EVENT_DISC_SVC_ALL_CMP = 1,			/**< Discovery all services complete event */
	RBLE_GATT_EVENT_DISC_SVC_ALL_128_CMP,			/**< Discovery all 128bit UUID services complete event */
	RBLE_GATT_EVENT_DISC_SVC_BY_UUID_CMP,			/**< Discovery service by UUID complete event */
	RBLE_GATT_EVENT_DISC_SVC_INCL_CMP,				/**< Discovery included services complete event */
	RBLE_GATT_EVENT_DISC_CHAR_ALL_CMP,				/**< Discovery all characteristics complete event */
	RBLE_GATT_EVENT_DISC_CHAR_ALL_128_CMP,			/**< Discovery all 128bit UUID characteristics complete event */
	RBLE_GATT_EVENT_DISC_CHAR_BY_UUID_CMP,			/**< Discovery characteristic by UUID complete event */
	RBLE_GATT_EVENT_DISC_CHAR_BY_UUID_128_CMP,		/**< Discovery characteristic by 128bit UUID complete event */
	RBLE_GATT_EVENT_DISC_CHAR_DESC_CMP,				/**< Discovery characteristic descriptor complete event */
	RBLE_GATT_EVENT_DISC_CHAR_DESC_128_CMP,			/**< Discovery 128bit characteristic descriptor complete event */
	RBLE_GATT_EVENT_READ_CHAR_RESP,					/**< Read characteristic response */
	RBLE_GATT_EVENT_READ_CHAR_LONG_RESP,			/**< Read characteristic long response */
	RBLE_GATT_EVENT_READ_CHAR_MULT_RESP,			/**< Read characteristic multiple response */
	RBLE_GATT_EVENT_READ_CHAR_LONG_DESC_RESP,		/**< Read characteristic long descriptor response */
	RBLE_GATT_EVENT_WRITE_CHAR_RESP,				/**< Write characteristic response */
	RBLE_GATT_EVENT_WRITE_CHAR_RELIABLE_RESP,		/**< Write characteristic reliable response */
	RBLE_GATT_EVENT_CANCEL_WRITE_CHAR_RESP,			/**< Cancel write characteristic response */
	RBLE_GATT_EVENT_HANDLE_VALUE_NOTIF,				/**< Handle Value notification */
	RBLE_GATT_EVENT_HANDLE_VALUE_IND,				/**< Handle Value indication */
	RBLE_GATT_EVENT_HANDLE_VALUE_CFM,				/**< Confirmation response */
	RBLE_GATT_EVENT_DISCOVERY_CMP,					/**< Discovery complete event */
	RBLE_GATT_EVENT_COMPLETE,						/**< Generic complete event */
	RBLE_GATT_EVENT_WRITE_CMD_IND,					/**< Write command indication */
	RBLE_GATT_EVENT_RESP_TIMEOUT,					/**< Response timeout notification */
	RBLE_GATT_EVENT_SET_PERM_CMP,					/**< Set Permission complete event */
	RBLE_GATT_EVENT_SET_DATA_CMP,					/**< Set Data complete event */
	RBLE_GATT_EVENT_NOTIFY_COMP,					/**< Notification complete event */
	RBLE_GATT_EVENT_COMMAND_DISALLOWED_IND			/**< Command Disallowed Event */
};

typedef uint8_t		RBLE_GATT_EVENT_TYPE;		/**< GATT Event Type definition */

/**
 * @brief	rBLE GATT Event Structure
 */
typedef struct RBLE_GATT_EVENT_t {
	RBLE_GATT_EVENT_TYPE		type;			/**< type of GATT event */
	uint8_t						reserved;
	union Event_Gatt_Parameter_u {
		/**
		 * @brief	Discover All Services Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_SVC_ALL_CMP
		 */
		struct RBLE_GATT_Disc_Svc_All_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t nb_resp;									/**< number of value pairs */
			RBLE_GATT_SVC_LIST list[RBLE_GATT_MAX_HDL_LIST];	/**< contain data list */
		} disc_svc_all_cmp;
		/**
		 * @brief	Discover All 128bit UUID Services Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_SVC_ALL_128_CMP
		 */
		struct RBLE_GATT_Disc_Svc_All_128_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			 uint8_t att_code;									/**< complete event status */
			 uint8_t nb_resp;									/**< number of value pairs */
			 RBLE_GATT_SVC_128_LIST list;						/**< contain data list */
		} disc_svc_all_128_cmp;
		/**
		 * @brief	Discover Services by UUID Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_SVC_BY_UUID_CMP
		 */
		struct RBLE_GATT_Disc_Svc_By_Uuid_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t nb_resp;									/**< number of value pairs */
			RBLE_GATT_SVC_RANGE_LIST list[RBLE_GATT_MAX_HDL_LIST];	  /**< list of found services */
		} disc_svc_by_uuid_cmp;
		/**
		 * @brief	Discover Included Services Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_SVC_INCL_CMP
		 */
		struct RBLE_GATT_Disc_Svc_Incl_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t nb_entry;									/**< number of found includes */
			uint8_t entry_len;									/**< size of the entry */
			/** include entries */
			union incl_list_u {
				RBLE_GATT_INCL_128_LIST incl;				 	/**< one include with 128-bit entry */
				RBLE_GATT_INCL_LIST list[RBLE_GATT_MAX_HDL_LIST];	/**< list of include declarations */
			} incl_list;
		} disc_svc_incl_cmp;
		/**
		 * @brief	Discovery All Characteristic Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_CHAR_ALL_CMP
		 */
		struct RBLE_GATT_Disc_Char_All_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t nb_entry;									/**< number of entries */
			RBLE_GATT_CHAR_LIST list[RBLE_GATT_MAX_HDL_LIST];	/**< list of found characteristics */
		} disc_char_all_cmp;
		/**
		 * @brief	Discovery All 128bit UUID Characteristic Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_CHAR_ALL_128_CMP
		 */
		struct RBLE_GATT_Disc_Char_All_128_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t nb_entry;									/**< number of entries */
			RBLE_GATT_CHAR_128_LIST list;						/**< list of found characteristics */
		} disc_char_all_128_cmp;
		/**
		 * @brief	Discover Characteristic by UUID Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_CHAR_BY_UUID_CMP
		 */
		struct RBLE_GATT_Disc_Char_By_Uuid_Comp_t 		{
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t nb_entry;									/**< number of entries */
			RBLE_GATT_CHAR_LIST list[RBLE_GATT_MAX_HDL_LIST];	/**< list of found characteristics with UUID */
		} disc_char_by_uuid_cmp;
		/**
		 * @brief	Discover Characteristic by 128bit UUID Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_CHAR_BY_UUID_128_CMP
		 */
		struct RBLE_GATT_Disc_Char_By_Uuid_128_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t nb_entry;									/**< number of entries */
			RBLE_GATT_CHAR_128_LIST list;						/**< found 128-bit characteristics with UUID */
		} disc_char_by_uuid_128_cmp;
		/**
		 * @brief	Discovery Characteristic Descriptor Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_CHAR_DESC_CMP
		 */
		struct RBLE_GATT_Disc_Char_Desc_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t nb_entry;									/**< number of entries */
			uint8_t reserved;
			RBLE_GATT_CHAR_DESC_LIST list[RBLE_GATT_MAX_HDL_LIST];	/**< list of found characteristic descriptors */
		} disc_char_desc_cmp;
		/**
		 * @brief	Discovery 128bit Characteristic Descriptor Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISC_CHAR_DESC_128_CMP
		 */
		struct RBLE_GATT_Disc_Char_Desc_128_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t nb_entry;									/**< number of entries */
			uint8_t reserved;
			RBLE_GATT_CHAR_DESC_128_LIST list_128;				/**< list of found characteristic descriptors */
		} disc_char_desc_128_cmp;
		/**
		 * @brief	Read Characteristic Response Structure
		 * Parameters for @ref RBLE_GATT_EVENT_READ_CHAR_RESP
		 */
		struct RBLE_GATT_Read_Char_Resp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			RBLE_GATT_INFO_DATA data;							/**< holder of retrieved data */
		} read_char_resp;
		/**
		 * @brief	Read Characteristic Long Response Structure
		 * Parameters for @ref RBLE_GATT_EVENT_READ_CHAR_LONG_RESP
		 */
		struct RBLE_GATT_Read_Char_Long_Resp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t val_len;									/**< size of the value data */
			uint16_t attr_hdl;									/**< handle */
			uint8_t value[RBLE_GATT_MAX_VALUE];					/**< actual value pairs */
		} read_char_long_resp;
		/**
		 * @brief	Read Characteristic Multiple Responses Structure
		 * Parameters for @ref RBLE_GATT_EVENT_READ_CHAR_MULT_RESP
		 */
		struct RBLE_GATT_Read_Char_Mult_Resp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t val_len;									/**< size of the response data */
			RBLE_GATT_QUERY_RESULT data[RBLE_GATT_MAX_NB_HDLS];	/**< actual value pairs */
		} read_char_mult_resp;
		/**
		 * @brief	Read Characteristic Long Descriptor Response Structure
		 * Parameters for @ref RBLE_GATT_EVENT_READ_CHAR_LONG_DESC_RESP
		 */
		struct RBLE_GATT_Read_Char_Long_Desc_Resp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t val_len;									/**< size of the value data */
			uint8_t value[RBLE_GATT_MAX_VALUE];					/**< actual value pairs */
			uint16_t attr_hdl;									/**< handle */
		} read_char_long_desc_resp;
		/**
		 * @brief	Write Characteristics Response Structure
		 * Parameters for @ref RBLE_GATT_EVENT_WRITE_CHAR_RESP
		 */
		struct RBLE_GATT_Write_Char_Resp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t reserved;
		} write_char_resp;
		/**
		 * @brief	Write Reliable Response Structure
		 * Parameters for @ref RBLE_GATT_EVENT_WRITE_CHAR_RELIABLE_RESP
		 */
		struct RBLE_GATT_Write_Reliable_Resp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t reserved;
		} write_reliable_resp;
		/**
		 * @brief	Cancel Write Characteristic Response Structure
		 * Parameters for @ref RBLE_GATT_EVENT_CANCEL_WRITE_CHAR_RESP
		 */
		struct RBLE_GATT_Cancel_Write_Char_Resp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t reserved;
		} cancel_write_resp;
		/**
		 * @brief	Handle Value Notification Structure
		 * Parameters for @ref RBLE_GATT_EVENT_HANDLE_VALUE_NOTIF
		 */
		struct RBLE_GATT_Handle_Value_Notif_t {
			uint16_t conhdl;									/**< connection handle */
			uint16_t charhdl;									/**< attribute handle */
			uint8_t size;										/**< size of data */
			uint8_t value[RBLE_GATT_MAX_VALUE];					/**< value */
			uint8_t reserved;
		} handle_value_notif;
		/**
		 * @brief	Handle Value Indication Structure
		 * Parameters for @ref RBLE_GATT_EVENT_HANDLE_VALUE_IND
		 */
		struct RBLE_GATT_Handle_Value_Ind_t {
			uint16_t conhdl;									/**< connection handle */
			uint16_t charhdl;									/**< attribute handle */
			uint8_t size;										/**< size of data */
			uint8_t value[RBLE_GATT_MAX_VALUE];					/**< value */
			uint8_t reserved;
		} handle_value_ind;
		/**
		 * @brief	Handle Value Confirmation Structure
		 * Parameters for @ref RBLE_GATT_EVENT_HANDLE_VALUE_CFM
		 */
		struct RBLE_GATT_Handle_Value_Cfm_t {
			RBLE_STATUS status;									/**< complete event status */
		} handle_value_cfm;
		/**
		 * @brief	GATT Discovery Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_DISCOVERY_CMP
		 */
		struct RBLE_GATT_Discovery_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t reserved;
		} discovery_cmp;
		/**
		 * @brief	GATT Generic Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_COMPLETE
		 */
		struct RBLE_GATT_Complete_t {
			uint16_t conhdl;									/**< connection handle */
			uint8_t att_code;									/**< complete event status */
			uint8_t reserved;
		} complete;
		/**
		 * @brief	Write Command Indication Structure
		 * Parameters for @ref RBLE_GATT_EVENT_WRITE_CMD_IND
		 */
		struct RBLE_GATT_Write_Cmd_Ind_t {
			uint16_t conhdl;									/**< connection handle */
			uint16_t elmt;										/**< element handle */
			uint16_t size;										/**< length */
			uint8_t offset;										/**< value offset */
			BOOL resp;											/**< response required or not */
			uint8_t value[RBLE_GATT_MAX_VALUE];					/**< value written */
		} write_cmd_ind;
		/**
		 * @brief	GATT Set Permission Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_SET_PERM_CMP
		 */
		struct RBLE_GATT_Set_Perm_Complete_t {
			RBLE_STATUS status;									/**< complete event status */
		} set_perm_cmp;
		/**
		 * @brief	GATT Set Data Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_SET_DATA_CMP
		 */
		struct RBLE_GATT_Set_Data_Complete_t {
			RBLE_STATUS status;									/**< complete event status */
		} set_data_cmp;
		/**
		 * @brief	Notification Complete Event Structure
		 * Parameters for @ref RBLE_GATT_EVENT_NOTIFY_COMP
		 */
		struct RBLE_GATT_Notify_Comp_t {
			uint16_t conhdl;									/**< connection handle */
			uint16_t charhdl;									/**< characteristic handle */
			RBLE_STATUS status;									/**< complete event status */
			uint8_t reserved;
		} notify_cmp;
		/**
		 * @brief	Command Disallowed Indication Structure
		 * Parameters for @ref RBLE_GATT_EVENT_COMMAND_DISALLOWED_IND
		 */
		struct RBLE_EVT_GATT_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
			uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_GATT_EVENT;

typedef void ( *RBLE_GATT_EVENT_HANDLER )( RBLE_GATT_EVENT *event );			/**< GATT Event CALL BACK declaration */


// Find Me Profile Call back define

/*** Locator ***/
enum RBLE_FMP_EVENT_TYPE_enum {
	RBLE_FMP_EVENT_TARGET_ENABLE_COMP = 0x01,		/* Target enable Complete Event */
	RBLE_FMP_EVENT_TARGET_DISABLE_COMP,				/* Target disable Complete Event */
	RBLE_FMP_EVENT_TARGET_ALERT_IND,				/* Target Alert Complete Event */
	RBLE_FMP_EVENT_TARGET_COMMAND_DISALLOWED_IND,	/* Target command disallowed Complete Event */
	RBLE_FMP_EVENT_LOCATOR_ENABLE_COMP = 0x81,		/* Locator enable Complete Event */
	RBLE_FMP_EVENT_LOCATOR_DISABLE_COMP,			/* Locator disable Complete Event */
	RBLE_FMP_EVENT_LOCATOR_ERROR_IND,				/* Locator error Complete Event */
	RBLE_FMP_EVENT_LOCATOR_COMMAND_DISALLOWED_IND	/* Locator command disallowed Complete Event */
};

typedef uint8_t		RBLE_FMP_EVENT_TYPE;

/*** Target ***/
typedef struct RBLE_FMPT_EVENT_t
{
	RBLE_FMP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Fmt_Parameter_u {

		/* RBLE_EVT_FMP_Target_Enable_Comp */
		struct RBLE_FMP_Target_Enable_t{
			RBLE_STATUS         status;
			uint8_t             reserved;
			uint16_t            conhdl;
		}target_enable;

		/* RBLE_EVT_FMP_Target_Disable_Comp */
		struct RBLE_FMP_Target_Disable_t{
			RBLE_STATUS         status;
			uint8_t             reserved;
			uint16_t            conhdl;
		}target_disable;

		/* RBLE_EVT_FMP_Target_Alert_Ind */
		struct RBLE_FMP_Target_Alert_Ind_t{
			uint16_t            conhdl;
			uint8_t             alert_lvl;
			uint8_t				reserved;
		}target_alert_ind;

		/* RBLE_EVT_FMP_CMD_DISALLOWED_IND */
		struct RBLE_FMP_Target_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
			uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_FMPT_EVENT;

typedef struct RBLE_FMPL_EVENT_t {
	RBLE_FMP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Fml_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_FMP_Locator_Enable_Comp */
		struct RBLE_FMP_Locator_Enable_t{
			RBLE_STATUS         status;
			uint8_t             reserved;
			uint16_t            conhdl;
			RBLE_FMP_IAS_CONTENT ias;
		}locator_enable;

		/* RBLE_EVT_FMP_Locator_Disable_Comp */
		struct RBLE_FMP_Locator_Disable_t{
			RBLE_STATUS         status;
			uint8_t             reserved;
			uint16_t            conhdl;
		}locator_disable;

		/* RBLE_EVT_FMP_Locator_Error_Ind */
		struct RBLE_FMP_Locator_Error_Ind_t{
			RBLE_STATUS         status;
			uint8_t             reserved;
			uint16_t            conhdl;
		}locator_error_ind;

		/* RBLE_EVT_FMP_CMD_DISALLOWED_IND */
		struct RBLE_FMP_Locator_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
			uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_FMPL_EVENT;

typedef void ( *RBLE_FMPT_EVENT_HANDLER )( RBLE_FMPT_EVENT *event );			/* FM Target Event CALL BACK Declaration */
typedef void ( *RBLE_FMPL_EVENT_HANDLER )( RBLE_FMPL_EVENT *event );			/* FM Locator Event CALL BACK Declaration */

// Proximity Profile Call back define

/*** Monitor ***/
enum RBLE_PXPM_EVENT_TYPE_enum {
	RBLE_PXP_EVENT_REPORTER_ENABLE_COMP = 0x01,		/* Reporter enable Complete Event */
	RBLE_PXP_EVENT_REPORTER_DISABLE_COMP,			/* Reporter disable Complete Event */
	RBLE_PXP_EVENT_REPORTER_ALERT_IND,				/* Reporter Alert Complete Event */
	RBLE_PXP_EVENT_REPORTER_COMMAND_DISALLOWED_IND,	/* Reporter command disallowed Complete Event */
	RBLE_PXP_EVENT_MONITOR_ENABLE_COMP = 0x81,		/* Monitor enable Complete Event */
	RBLE_PXP_EVENT_MONITOR_DISABLE_COMP,			/* Monitor disable Complete Event */
	RBLE_PXP_EVENT_MONITOR_ERROR_IND,				/* Monitor error Complete Event */
	RBLE_PXP_EVENT_MONITOR_READ_CHAR_RESPONSE,		/* Monitor read Characteristic Complete Event */
	RBLE_PXP_EVENT_MONITOR_WRITE_CHAR_RESPONSE,		/* Monitor write Characteristic Complete Event */
	RBLE_PXP_EVENT_MONITOR_COMMAND_DISALLOWED_IND	/* Monitor command disallowed Complete Event */
};

typedef uint8_t		RBLE_PXP_EVENT_TYPE;

typedef struct RBLE_PXPR_EVENT_t {
	RBLE_PXP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Pmr_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_PXP_Reporter_Enable_Comp */
		struct RBLE_PXP_Reporter_Enable_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
		}report_enable;

		/* RBLE_EVT_PXP_Reporter_Disable_Comp */
		struct RBLE_PXP_Reporter_Disable_t{
			uint16_t            conhdl;
			uint8_t             lls_alert_lvl;
			uint8_t             reserved;
		}report_disable;

		/* RBLE_EVT_PXP_Reporter_Alert_Ind */
		struct RBLE_PXP_Reporter_Alert_Ind_t{
			uint16_t            conhdl;
			uint8_t             alert_lvl;
			uint8_t             reserved;
		}report_alert_ind;

		/* RBLE_EVT_PXP_CMD_DISALLOWED_IND */
		struct RBLE_PXP_Reporter_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_PXPR_EVENT;

typedef struct RBLE_PXPM_EVENT_t {
	RBLE_PXP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Pmm_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_PXP_Monitor_Enable_Comp */
		struct RBLE_PXP_Monitor_Enable_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
			RBLE_LLS_CONTENT    lls;
			RBLE_PXP_IAS_CONTENT ias;
			RBLE_TPS_CONTENT    tps;
		}monitor_enable;

		/* RBLE_EVT_PXP_Monitor_Disable_Comp */
		struct RBLE_PXP_Monitor_Disable_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
		}monitor_disale;

		/* RBLE_EVT_PXP_Monitor_Error_Ind */
		struct RBLE_PXP_Monitor_Error_Ind_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
		}monitor_error_ind;
		
		/* RBLE_EVT_PXP_Monitor_Read_Char_Response */
		struct RBLE_PXP_Monitor_Read_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
			RBLE_ATT_INFO_DATA data;
		}rd_char_resp;
		
		/* RBLE_EVT_PXP_Monitor_Write_Char_Response */
		struct RBLE_PXP_Monitor_Write_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
		}wr_char_resp;

		/* RBLE_EVT_PXP_CMD_DISALLOWED_IND */
		struct RBLE_PXP_Monitor_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_PXPM_EVENT;

typedef void ( *RBLE_PXPR_EVENT_HANDLER )( RBLE_PXPR_EVENT *event );			/* PM(Reporter) Event CALL BACK Declaration */
typedef void ( *RBLE_PXPM_EVENT_HANDLER )( RBLE_PXPM_EVENT *event );			/* PM(Monitor) Event CALL BACK Declaration */

// Health Thermometer Profile Call back define
/*** Collector ***/
enum RBLE_HTP_EVENT_TYPE_enum {
	RBLE_HTP_EVENT_THERMOMETER_ENABLE_COMP = 0x01,		/* Thermometer enable Complete Event */
	RBLE_HTP_EVENT_THERMOMETER_DISABLE_COMP,			/* Thermometer disable Complete Event */
	RBLE_HTP_EVENT_THERMOMETER_ERROR_IND,				/* Thermometer error Complete Event */
	RBLE_HTP_EVENT_THERMOMETER_SEND_TEMP_COMP,			/* Thermometer send temp Complete Event */
	RBLE_HTP_EVENT_THERMOMETER_REQ_MEASUREMENT_PERIOD_IND_COMP,	/* Thermometer indicate measurement interval Complete Event */
	RBLE_HTP_EVENT_THERMOMETER_MEAS_INTV_CHG_IND,		/* Thermometer meas intv chg Complete Event */
	RBLE_HTP_EVENT_THERMOMETER_CFG_INDNTF_IND,			/* Thermometer cfg indntf Complete Event */
	RBLE_HTP_EVENT_THERMOMETER_COMMAND_DISALLOWED_IND,	/* Thermometer command disallowed Complete Event */
	RBLE_HTP_EVENT_COLLECTOR_ENABLE_COMP = 0x81,		/* Collector enable Complete Event */
	RBLE_HTP_EVENT_COLLECTOR_DISABLE_COMP,				/* Collector disable Complete Event */
	RBLE_HTP_EVENT_COLLECTOR_ERROR_IND,					/* Collector error Complete Event */
	RBLE_HTP_EVENT_COLLECTOR_TEMP_IND,					/* Collector temp Complete Event */
	RBLE_HTP_EVENT_COLLECTOR_MEAS_INTV_IND,				/* Collector meas intv Complete Event */
	RBLE_HTP_EVENT_COLLECTOR_READ_CHAR_RESPONSE,		/* Collector read Characteristic Complete Event */
	RBLE_HTP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE,		/* Collector write Characteristic Complete Event */
	RBLE_HTP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND		/* Collector command disallowed Complete Event */
};

typedef uint8_t		RBLE_HTP_EVENT_TYPE;

/*** Thermometer ***/
typedef struct RBLE_HTPT_EVENT_t {
	RBLE_HTP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Htt_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_HTP_Thermometer_Enable_Comp */
		struct RBLE_HTP_Thermometer_Enable_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
		}thermometer_enable;

		/* RBLE_EVT_HTP_Thermometer_Disable_Comp */
		struct RBLE_HTP_Thermometer_Disable_t{
			uint16_t            conhdl;
			RBLE_HTP_THERM_PARAM therm_info;
		}thermometer_disable;

		/* RBLE_EVT_HTP_Thermometer_Error_Ind */
		struct RBLE_HTP_Thermometer_Error_Ind_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
		}error_ind;

		/* RBLE_EVT_HTP_Thermometer_Send_Temp_Comp */
		struct RBLE_HTP_Thermometer_Send_Temp_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
		}send_temp;

		/* RBLE_EVT_HTP_Thermometer_Req_Measurement_Period_Ind_Comp */
		struct RBLE_HTP_Thermometer_Req_Measurement_Period_Ind_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
		}send_meas_period;

		/* RBLE_EVT_HTP_Thermometer_Meas_Intv_Chg_Ind */
		struct RBLE_HTP_Thermometer_Meas_Intv_Chg_Ind_t{
			uint16_t            conhdl;
			uint16_t            intv;
		}meas_intv_chg_ind;

		/* RBLE_EVT_HTP_Thermometer_Cfg_Indntf_Ind */
		struct RBLE_HTP_Thermometer_Cfg_Indntf_Ind_t{
		    uint16_t 			conhdl;
		    uint8_t 			char_code;
			uint8_t				reserved;
		    uint16_t 			cfg_val;
		}htpt_cfg_indntf_ind;

		/* RBLE_EVT_HTP_CMD_DISALLOWED_IND */
		struct RBLE_HTP_Thermometer_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_HTPT_EVENT;

typedef struct RBLE_HTPC_EVENT_t {
	RBLE_HTP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Htc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_HTP_Collector_Enable_Comp */
		struct RBLE_HTP_Collector_Enable_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
			RBLE_HTS_CONTENT    hts;
			RBLE_DIS_CONTENT    dis;
		}collector_enable;

		/* RBLE_EVT_HTP_Collector_Disable_Comp */
		struct RBLE_HTP_Collector_Disable_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
		}collector_disable;

		/* RBLE_EVT_HTP_Collector_Error_Ind */
		struct RBLE_HTP_Collector_Error_Ind_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
		}error_ind;

		/* RBLE_EVT_HTP_Collector_Temp_Ind */
		struct RBLE_HTP_Collector_Temp_Ind_t{
			uint16_t            conhdl;
			RBLE_HTP_TEMP_INFO   temp_info;
		}temp_ind;

		/* RBLE_EVT_HTP_Collector_Meas_Intv_Ind */
		struct RBLE_HTP_Collector_Meas_Intv_Ind_t{
			uint16_t            conhdl;
			uint16_t            intv;
		}meas_intv_ind;
		
		/* RBLE_EVT_HTP_Collector_Read_Char_Response */
		struct RBLE_HTP_Collector_Read_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
			RBLE_ATT_INFO_DATA data;
		}rd_char_resp;
		
		/* RBLE_EVT_HTP_Collector_Write_Char_Response */
		struct RBLE_HTP_Collector_Write_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
		}wr_char_resp;

		/* RBLE_EVT_HTP_CMD_DISALLOWED_IND */
		struct RBLE_HTP_Collector_Command_Disallowed_Ind_t{
			RBLE_STATUS        status;
            uint8_t            reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_HTPC_EVENT;

typedef void ( *RBLE_HTPT_EVENT_HANDLER )( RBLE_HTPT_EVENT *event );			/* HT(Thermometer) Event CALL BACK Declaration */
typedef void ( *RBLE_HTPC_EVENT_HANDLER )( RBLE_HTPC_EVENT *event );			/* HT(Collector) Event CALL BACK Declaration */

// Blood Pressure Profile Call back define
enum RBLE_BLP_EVENT_TYPE_enum {
	RBLE_BLP_EVENT_SENSOR_ENABLE_COMP = 0x01,		/* Sensor enable Complete Event */
	RBLE_BLP_EVENT_SENSOR_DISABLE_COMP,				/* Sensor disable Complete Event */
	RBLE_BLP_EVENT_SENSOR_ERROR_IND,				/* Sensor error Complete Event */
	RBLE_BLP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP,	/* Sensor send measurements Complete Event */
	RBLE_BLP_EVENT_SENSOR_CFG_INDNTF_IND,
	RBLE_BLP_EVENT_SENSOR_COMMAND_DISALLOWED_IND,	/* Sensor Command disallowed Complete Event */
	RBLE_BLP_EVENT_COLLECTOR_ENABLE_COMP = 0x81,	/* Collector enable Complete Event */
	RBLE_BLP_EVENT_COLLECTOR_DISABLE_COMP,			/* Collector disable Complete Event */
	RBLE_BLP_EVENT_COLLECTOR_ERROR_IND,				/* Collector error Complete Event */
	RBLE_BLP_EVENT_COLLECTOR_MEASUREMENTS_IND,		/* Collector measurements Complete Event */
	RBLE_BLP_EVENT_COLLECTOR_READ_CHAR_RESPONSE,	/* Collector read Characteristic Complete Event */
	RBLE_BLP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE,	/* Collector write Characteristic Complete Event */
	RBLE_BLP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND	/* Collector Command Disallowed Complete Event */
};

typedef uint8_t		RBLE_BLP_EVENT_TYPE;

/*** Sensor ***/
typedef struct RBLE_BLPS_EVENT_t {
	RBLE_BLP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Bls_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_BLP_Sensor_Enable_Comp */
		struct RBLE_BLP_Sensor_Enable_t{
			RBLE_STATUS         status;
	        uint8_t			    reserved;
			uint16_t            conhdl;
		}sensor_enable;

		/* RBLE_EVT_BLP_Sensor_Disable_Comp */
		struct RBLE_BLP_Sensor_Disable_t{
			uint16_t            conhdl;
			RBLE_BLP_SENSOR_PARAM sensor_info;
		}sensor_disable;

		/* RBLE_EVT_BLP_Sensor_Error_Ind */
		struct RBLE_BLP_Sensor_Error_Ind_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
		}error_ind;

		/* RBLE_EVT_BLP_SENSOR_SEND_MEASUREMENTS_COMP */
		struct RBLE_BLP_Sensor_Send_Measurements_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
		}send_measurements;

		/* RBLE_EVT_BLP_SENSOR_Cfg_Indntf_Ind */
		struct RBLE_BLP_Sensor_Cfg_Indntf_Ind_t{
		    uint16_t 			conhdl;
		    uint8_t 			char_code;
			uint8_t				reserved;
		    uint16_t 			cfg_val;
		}blps_cfg_indntf_ind;

		/* RBLE_EVT_BLP_CMD_DISALLOWED_IND */
		struct RBLE_BLP_Sensor_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
	        uint8_t		   reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_BLPS_EVENT;

/*** Collector ***/
typedef struct RBLE_BLPC_EVENT_t {
	RBLE_BLP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Blc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_BLP_Collector_Enable_Comp */
		struct RBLE_BLP_Collector_Enable_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
			RBLE_BLS_CONTENT    bls;
			RBLE_DIS_CONTENT    dis;
		}collector_enable;

		/* RBLE_EVT_BLP_Collector_Disable_Comp */
		struct RBLE_BLP_Collector_Disable_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
		}collector_disable;

		/* RBLE_EVT_BLP_Collector_Error_Ind */
		struct RBLE_BLP_Collector_Error_Ind_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            conhdl;
		}error_ind;

		/* RBLE_EVT_BLP_Collector_Measurements_Ind */
		struct RBLE_BLP_Collector_Measurements_Ind_t{
			uint16_t            conhdl;
			RBLE_BLP_MEASUREMENTS_INFO   measurements_info;
		}measurements_ind;
		
		/* RBLE_EVT_BLP_Collector_Read_Char_Response */
		struct RBLE_BLP_Collector_Read_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
			RBLE_ATT_INFO_DATA data;
		}rd_char_resp;
		
		/* RBLE_EVT_BLP_Collector_Write_Char_Response */
		struct RBLE_BLP_Collector_Write_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
		}wr_char_resp;

		/* RBLE_EVT_BLP_CMD_DISALLOWED_IND */
		struct RBLE_BLP_Collector_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_BLPC_EVENT;

typedef void ( *RBLE_BLPS_EVENT_HANDLER )( RBLE_BLPS_EVENT *event );			/* BL(Sensor) Event CALL BACK Declaration */
typedef void ( *RBLE_BLPC_EVENT_HANDLER )( RBLE_BLPC_EVENT *event );			/* BL(Collector) Event CALL BACK Declaration */

// HID over GATT Profile Callback Declaration
enum RBLE_HGP_EVENT_TYPE_enum {
	RBLE_HGP_EVENT_HDEVICE_ENABLE_COMP = 0x01,		/* HID Device enable complete event */
	RBLE_HGP_EVENT_HDEVICE_DISABLE_COMP,			/* HID Device disable complete event */
	RBLE_HGP_EVENT_HDEVICE_ERROR_IND,				/* HID Device error indication event */
	RBLE_HGP_EVENT_HDEVICE_CFG_INDNTF_IND,	        /* Configured value change indication event */
	RBLE_HGP_EVENT_HDEVICE_REPORT_IND,	            /* Report value setting indication event */
	RBLE_HGP_EVENT_HDEVICE_PROTOCOL_MODE_CHG_EVT,   /* Protocol mode change notification event */
	RBLE_HGP_EVENT_HDEVICE_REPORT_EVT,	            /* Report value notification event */
	RBLE_HGP_EVENT_HDEVICE_HID_CP_CHG_EVT,	        /* Control Point change notification event */
	RBLE_HGP_EVENT_HDEVICE_REPORT_COMP,	            /* Report value send completion event */
	RBLE_HGP_EVENT_HDEVICE_SEND_BATTERY_LEVEL_COMP,	/* Battery Level send completion event */
	RBLE_HGP_EVENT_HDEVICE_COMMAND_DISALLOWED_IND,	/* Command disallowed indication event */
	RBLE_HGP_EVENT_BHOST_ENABLE_COMP = 0x81,	    /* Boot Host enable completion event */
	RBLE_HGP_EVENT_BHOST_DISABLE_COMP,			    /* Boot Host disable completion event */
	RBLE_HGP_EVENT_BHOST_ERROR_IND,				    /* Boot Host error indication event */
	RBLE_HGP_EVENT_BHOST_READ_CHAR_RESPONSE,	    /* Characteristic read request response event */
	RBLE_HGP_EVENT_BHOST_WRITE_CHAR_RESPONSE,	    /* Characteristic write request response event */
	RBLE_HGP_EVENT_BHOST_REPORT_NTF,		        /* Report value notification event */
	RBLE_HGP_EVENT_BHOST_COMMAND_DISALLOWED_IND,	/* Command disallowed indication event */
	RBLE_HGP_EVENT_RHOST_ENABLE_COMP = 0xC1,	    /* Report Host enable completion event */
	RBLE_HGP_EVENT_RHOST_DISABLE_COMP,			    /* Report Host disable completion event */
	RBLE_HGP_EVENT_RHOST_ERROR_IND,				    /* Report Host error indication event */
	RBLE_HGP_EVENT_RHOST_READ_CHAR_RESPONSE,	    /* Characteristic read request response event */
	RBLE_HGP_EVENT_RHOST_READ_LONG_CHAR_RESPONSE,	/* Long Characteristic read request response event */
	RBLE_HGP_EVENT_RHOST_WRITE_CHAR_RESPONSE,	    /* Characteristic write request response event */
	RBLE_HGP_EVENT_RHOST_REPORT_NTF,		        /* Report value notification event */
	RBLE_HGP_EVENT_RHOST_BATTERY_LEVEL_NTF,		    /* Battery Level notification event */
	RBLE_HGP_EVENT_RHOST_COMMAND_DISALLOWED_IND	    /* Command disallowed indication event */
};

typedef uint8_t		RBLE_HGP_EVENT_TYPE;

/*** HID Device ***/
typedef struct RBLE_HGHD_EVENT_t {
	RBLE_HGP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Hghd_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_HGP_HDevice_Enable_Comp */
		struct RBLE_HGP_HDevice_Enable_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
		}hdevice_enable;

		/* RBLE_EVT_HGP_HDevice_Disable_Comp */
		struct RBLE_HGP_HDevice_Disable_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
			RBLE_HGP_DEVICE_PARAM device_info;
		}hdevice_disable;

		/* RBLE_EVT_HGP_HDevice_Error_Ind */
		struct RBLE_HGP_HDevice_Error_Ind_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
		}error_ind;

		/* RBLE_EVT_HGP_HDEVICE_CFG_INDNTF_IND */
		struct RBLE_HGP_HDevice_Cfg_Indntf_Ind_t{
		    uint16_t 			    conhdl;
		    uint8_t 			    inst_idx;
		    uint8_t 			    char_code;
		    uint16_t 			    cfg_val;
		}hghd_cfg_indntf_ind;

		/* RBLE_EVT_HGP_HDEVICE_REPORT_IND */
		struct RBLE_HGP_HDevice_Report_Ind_t{
			uint16_t                conhdl;
			uint8_t                 inst_idx;
            uint8_t                 reserved;
            RBLE_HGP_REPORT_DESC    report;
        }report_chg_ind;

		/* RBLE_EVT_HGP_HDEVICE_PROTOCOL_MODE_CHG_EVT */
		struct RBLE_HGP_HDevice_Protocol_Mode_Chg_Evt_t{
			uint16_t                conhdl;
			uint8_t                 inst_idx;
            uint8_t                 protocol_mode_val;
		}protocol_mode_chg_evt;

		/* RBLE_EVT_HGP_HDEVICE_REPORT_EVT */
		struct RBLE_HGP_HDevice_Report_Evt_t{
			uint16_t                conhdl;
			uint8_t                 inst_idx;
            uint8_t                 reserved;
            RBLE_HGP_REPORT_DESC    report;
		}report_chg_evt;

		/* RBLE_EVT_HGP_HDEVICE_HID_CP_CHG_EVT */
		struct RBLE_HGP_HDevice_Hid_Cp_Chg_Evt_t{
			uint16_t                conhdl;
			uint8_t                 inst_idx;
            uint8_t                 control_point_val;
		}hid_cp_chg_evt;

		/* RBLE_EVT_HGP_HDEVICE_REPORT_COMP */
		struct RBLE_HGP_HDevice_Send_Report_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
		}send_report;

		/* RBLE_EVT_HGP_HDEVICE_SEND_BATTERY_LEVEL_COMP */
		struct RBLE_HGP_HDevice_Send_Battery_Level_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
		}send_battery_level;

		/* RBLE_EVT_BLP_CMD_DISALLOWED_IND */
		struct RBLE_HGP_HDevice_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_HGHD_EVENT;

/*** Boot Host ***/
typedef struct RBLE_HGBH_EVENT_t {
	RBLE_HGP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Hgbh_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_HGP_BHost_Enable_Comp */
		struct RBLE_HGP_BHost_Enable_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 hids_inst_num;
            uint8_t                 bas_inst_num;
            uint8_t                 reserved;
			RBLE_HIDS_CONTENT       *hids;
			RBLE_DIS11_CONTENT      dis;
			RBLE_BAS_CONTENT        *bas;
		}bhost_enable;

		/* RBLE_EVT_HGP_BHost_Disable_Comp */
		struct RBLE_HGP_BHost_Disable_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
		}bhost_disable;

		/* RBLE_EVT_HGP_BHost_Error_Ind */
		struct RBLE_HGP_BHost_Error_Ind_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
		}error_ind;

		/* RBLE_EVT_HGP_BHOST_REPORT_NTF */
		struct RBLE_HGP_BHost_Report_Ntf_t{
			uint16_t                conhdl;
            uint8_t                 inst_idx;
            uint8_t                 reserved;
			RBLE_HGP_REPORT_DESC    report;
		}report_ntf;
		
		/* RBLE_EVT_HGP_BHOST_READ_CHAR_RESPONSE */
		struct RBLE_HGP_BHost_Read_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
            uint8_t            reserved;
			RBLE_ATT_INFO_DATA data;
		}rd_char_resp;
		
		/* RBLE_EVT_HGP_BHOST_WRITE_CHAR_RESPONSE */
		struct RBLE_HGP_BHost_Write_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
            uint8_t            reserved;
		}wr_char_resp;

		/* RBLE_EVT_HGP_CMD_DISALLOWED_IND */
		struct RBLE_HGP_BHost_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_HGBH_EVENT;

/*** Report Host ***/
typedef struct RBLE_HGRH_EVENT_t {
	RBLE_HGP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Hgrh_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_HGP_RHost_Enable_Comp */
		struct RBLE_HGP_RHost_Enable_t{
			RBLE_STATUS             status;
            uint8_t                 reserved;
			uint16_t                conhdl;
            uint8_t                 hids_inst_num;
            uint8_t                 bas_inst_num;
			RBLE_HIDS_CONTENT       *hids;
			RBLE_DIS11_CONTENT      dis;
			RBLE_BAS_CONTENT        *bas;
		}rhost_enable;

		/* RBLE_EVT_HGP_RHost_Disable_Comp */
		struct RBLE_HGP_RHost_Disable_t{
			RBLE_STATUS             status;
            uint8_t                 reserved;
			uint16_t                conhdl;
		}rhost_disable;

		/* RBLE_EVT_HGP_RHost_Error_Ind */
		struct RBLE_HGP_RHost_Error_Ind_t{
			RBLE_STATUS             status;
            uint8_t                 reserved;
			uint16_t                conhdl;
		}error_ind;

		/* RBLE_EVT_HGP_RHOST_REPORT_NTF */
		struct RBLE_HGP_RHost_Report_Ntf_t{
			uint16_t                conhdl;
            uint8_t                 inst_idx;
            uint8_t                 reserved;
			RBLE_HGP_REPORT_DESC    report;
		}report_ntf;
		
		/* RBLE_EVT_HGP_RHOST_BATTERY_LEVEL_NTF */
		struct RBLE_HGP_RHost_Battery_Level_Ntf_t{
			uint16_t                conhdl;
            uint8_t                 inst_idx;
            uint8_t                 battery_level;
		}battery_level_ntf;

		/* RBLE_EVT_HGP_RHOST_READ_CHAR_RESPONSE */
		struct RBLE_HGP_RHost_Read_Char_Response_t{
			uint16_t                conhdl;
			uint8_t                 att_code;
            uint8_t                 reserved;
			RBLE_ATT_INFO_DATA      data;
		}rd_char_resp;

		/* RBLE_EVT_HGP_RHOST_READ_LONG_CHAR_RESPONSE */
		struct RBLE_HGP_RHost_Read_Long_Char_Response_t{
			uint16_t                conhdl;
			uint8_t                 att_code;
            uint8_t                 reserved;
			RBLE_ATT_INFO_LDATA     data;
		}rd_long_char_resp;

		/* RBLE_EVT_HGP_RHOST_WRITE_CHAR_RESPONSE */
		struct RBLE_HGP_RHost_Write_Char_Response_t{
			uint16_t                conhdl;
			uint8_t                 att_code;
            uint8_t                 reserved;
		}wr_char_resp;

		/* RBLE_EVT_HGP_CMD_DISALLOWED_IND */
		struct RBLE_HGP_RHost_Command_Disallowed_Ind_t{
			RBLE_STATUS             status;
            uint8_t                 reserved;
			uint16_t                opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_HGRH_EVENT;

typedef void ( *RBLE_HGHD_EVENT_HANDLER )( RBLE_HGHD_EVENT *event );			/* HG(HID Device) Event CALL BACK Declaration */
typedef void ( *RBLE_HGBH_EVENT_HANDLER )( RBLE_HGBH_EVENT *event );			/* HG(Boot Host) Event CALL BACK Declaration */
typedef void ( *RBLE_HGRH_EVENT_HANDLER )( RBLE_HGRH_EVENT *event );			/* HG(Report Host) Event CALL BACK Declaration */

// Scan Parameters Profile Callback Declaration
enum RBLE_SPP_EVENT_TYPE_enum {
	RBLE_SPP_EVENT_SERVER_ENABLE_COMP = 0x01,	    /* Scan Server enable completion event */
	RBLE_SPP_EVENT_SERVER_DISABLE_COMP,			    /* Scan Server disable completion event */
	RBLE_SPP_EVENT_SERVER_CFG_INDNTF_IND,	        /* Configured value change indication event */
	RBLE_SPP_EVENT_SERVER_SEND_REFRESH_COMP,	    /* Scan refresh request send completion event */
	RBLE_SPP_EVENT_SERVER_INTERVAL_CHG_EVT,	        /* Scan interval window notification event */
	RBLE_SPP_EVENT_SERVER_COMMAND_DISALLOWED_IND,	/* Command disallowed indication event */
	RBLE_SPP_EVENT_CLIENT_ENABLE_COMP = 0x81,	    /* Scan Client enable completion event */
	RBLE_SPP_EVENT_CLIENT_DISABLE_COMP,			    /* Scan Client disable completion event */
	RBLE_SPP_EVENT_CLIENT_ERROR_IND,				/* Scan Client error indication event */
	RBLE_SPP_EVENT_CLIENT_WRITE_CHAR_RESPONSE,	    /* Characteristic write request response event */
	RBLE_SPP_EVENT_CLIENT_COMMAND_DISALLOWED_IND,	/* Command disallowed indication event */
};

typedef uint8_t		RBLE_SPP_EVENT_TYPE;

/*** Server ***/
typedef struct RBLE_SPPS_EVENT_t {
	RBLE_SPP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Scans_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_SPP_Server_Enable_Comp */
		struct RBLE_SPP_Server_Enable_t{
			uint16_t                        conhdl;
			RBLE_STATUS                     status;
            uint8_t                         reserved;
		}server_enable;

		/* RBLE_EVT_SPP_Server_Disable_Comp */
		struct RBLE_SPP_Server_Disable_t{
			uint16_t                        conhdl;
			RBLE_STATUS                     status;
            uint8_t                         reserved;
			RBLE_SPP_SERVER_PARAM           device_info;
		}server_disable;

		/* RBLE_EVT_SPP_SERVER_CFG_INDNTF_IND */
		struct RBLE_SPP_Server_Cfg_Indntf_Ind_t{
		    uint16_t 			            conhdl;
		    uint16_t 			            cfg_val;
		}scans_cfg_indntf_ind;

		/* RBLE_EVT_SPP_SERVER_INTERVAL_CHG_EVT */
		struct RBLE_SPP_Server_Interval_Chg_Evt_t{
			uint16_t                        conhdl;
			RBLE_SCANS_INTV_WINDOW_PARAM    scan_param;
		}interval_chg_evt;

		/* RBLE_EVT_SPP_SERVER_SEND_REFRESH_COMP */
		struct RBLE_SPP_Server_Send_Refresh_t{
			uint16_t                        conhdl;
			RBLE_STATUS                     status;
            uint8_t                         reserved;
		}send_refresh;

		/* RBLE_EVT_SPP_SERVER_CMD_DISALLOWED_IND */
		struct RBLE_SPP_Server_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_SPPS_EVENT;

/*** Client ***/
typedef struct RBLE_SPPC_EVENT_t {
	RBLE_SPP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Scanc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_SPP_Client_Enable_Comp */
		struct RBLE_SPP_Client_Enable_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
			RBLE_SPS_CONTENT        sps;
		}client_enable;

		/* RBLE_EVT_SPP_Client_Disable_Comp */
		struct RBLE_SPP_Client_Disable_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
		}client_disable;

		/* RBLE_EVT_SPP_Client_Error_Ind */
		struct RBLE_SPP_Client_Error_Ind_t{
			uint16_t                conhdl;
			RBLE_STATUS             status;
            uint8_t                 reserved;
		}error_ind;

		/* RBLE_EVT_SPP_CLIENT_WRITE_CHAR_RESPONSE */
		struct RBLE_SPP_Client_Write_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
            uint8_t            reserved;
		}wr_char_resp;

		/* RBLE_EVT_SPP_CMD_DISALLOWED_IND */
		struct RBLE_SPP_Client_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_SPPC_EVENT;

typedef void ( *RBLE_SPPS_EVENT_HANDLER )( RBLE_SPPS_EVENT *event );			/* SP(Server) Event CALL BACK Declaration */
typedef void ( *RBLE_SPPC_EVENT_HANDLER )( RBLE_SPPC_EVENT *event );			/* SP(Client) Event CALL BACK Declaration */

// Heart Rate Profile Call back define
enum RBLE_HRP_EVENT_TYPE_enum {
	RBLE_HRP_EVENT_SENSOR_ENABLE_COMP = 0x01,		/* Sensor enable Complete Event */
	RBLE_HRP_EVENT_SENSOR_DISABLE_COMP,				/* Sensor disable Complete Event */
	RBLE_HRP_EVENT_SENSOR_ERROR_IND,				/* Sensor error Complete Event */
	RBLE_HRP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP,	/* Sensor send measurements Complete Event */
	RBLE_HRP_EVENT_SENSOR_CHG_CP_IND,				/* Sensor control point change Complete Event */
	RBLE_HRP_EVENT_SENSOR_CFG_NTF_IND,				/* Sensor configuration change Complete Event */
	RBLE_HRP_EVENT_SENSOR_COMMAND_DISALLOWED_IND,	/* Sensor Command disallowed Complete Event */
	RBLE_HRP_EVENT_COLLECTOR_ENABLE_COMP = 0x81,	/* Collector enable Complete Event */
	RBLE_HRP_EVENT_COLLECTOR_DISABLE_COMP,			/* Collector disable Complete Event */
	RBLE_HRP_EVENT_COLLECTOR_ERROR_IND,				/* Collector error Complete Event */
	RBLE_HRP_EVENT_COLLECTOR_MEASUREMENTS_NTF,		/* Collector measurements Complete Event */
	RBLE_HRP_EVENT_COLLECTOR_READ_CHAR_RESPONSE,	/* Collector read Characteristic Complete Event */
	RBLE_HRP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE,	/* Collector write Characteristic Complete Event */
	RBLE_HRP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND	/* Collector Command Disallowed Complete Event */
};

typedef uint8_t		RBLE_HRP_EVENT_TYPE;

/*** Sensor ***/
typedef struct RBLE_HRPS_EVENT_t {
	RBLE_HRP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Hrs_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_HRP_Sensor_Enable_Comp */
		struct RBLE_HRP_Sensor_Enable_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
			uint8_t			    reserved;
		}sensor_enable;

		/* RBLE_EVT_HRP_Sensor_Disable_Comp */
		struct RBLE_HRP_Sensor_Disable_t{
			uint16_t            conhdl;
			RBLE_HRP_SENSOR_PARAM sensor_info;
		}sensor_disable;

		/* RBLE_EVT_HRP_Sensor_Error_Ind */
		struct RBLE_HRP_Sensor_Error_Ind_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
	        uint8_t             reserved;
		}error_ind;

		/* RBLE_EVT_HRP_SENSOR_SEND_MEASUREMENTS_COMP */
		struct RBLE_HRP_Sensor_Send_Measurements_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
	        uint8_t             reserved;
		}send_measurements;
		
		/* RBLE_EVT_HRP_SENSOR_Chg_Cp_Ind */
		struct RBLE_HRP_Sensor_Chg_Cp_Ind_t{
		    uint16_t 			conhdl;
		    uint8_t 			cp_val;
	        uint8_t             reserved;
		}hrps_chg_cp_ind;

		/* RBLE_EVT_HRP_SENSOR_Cfg_Ntf_Ind */
		struct RBLE_HRP_Sensor_Cfg_Ntf_Ind_t{
		    uint16_t 			conhdl;
		    uint16_t 			cfg_val;
		}hrps_cfg_ntf_ind;

		/* RBLE_EVT_HRP_CMD_DISALLOWED_IND */
		struct RBLE_HRP_Sensor_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
	        uint8_t		   reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_HRPS_EVENT;

/*** Collector ***/
typedef struct RBLE_HRPC_EVENT_t {
	RBLE_HRP_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Hrc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_HRP_Collector_Enable_Comp */
		struct RBLE_HRP_Collector_Enable_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
            uint8_t             reserved;
			RBLE_HRS_CONTENT    hrs;
			RBLE_DIS_CONTENT    dis;
		}collector_enable;

		/* RBLE_EVT_HRP_Collector_Disable_Comp */
		struct RBLE_HRP_Collector_Disable_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
            uint8_t             reserved;
		}collector_disable;

		/* RBLE_EVT_HRP_Collector_Error_Ind */
		struct RBLE_HRP_Collector_Error_Ind_t{
			uint16_t            conhdl;
			RBLE_STATUS         status;
            uint8_t             reserved;
		}error_ind;

		/* RBLE_EVT_HRP_Collector_Measurements_Ntf */
		struct RBLE_HRP_Collector_Measurements_Ntf_t{
			uint16_t            conhdl;
			RBLE_HRP_MEASUREMENTS_INFO   measurements_info;
		}measurements_ntf;
		
		/* RBLE_EVT_HRP_Collector_Read_Char_Response */
		struct RBLE_HRP_Collector_Read_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
			uint8_t            reserved;
			RBLE_ATT_INFO_DATA data;
		}rd_char_resp;
		
		/* RBLE_EVT_HRP_Collector_Write_Char_Response */
		struct RBLE_HRP_Collector_Write_Char_Response_t{
			uint16_t           conhdl;
			uint8_t            att_code;
			uint8_t            reserved;
		}wr_char_resp;

		/* RBLE_EVT_HRP_CMD_DISALLOWED_IND */
		struct RBLE_HRP_Collector_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
			uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_HRPC_EVENT;

typedef void ( *RBLE_HRPS_EVENT_HANDLER )( RBLE_HRPS_EVENT *event );			/* HR(Sensor) Event CALL BACK Declaration */
typedef void ( *RBLE_HRPC_EVENT_HANDLER )( RBLE_HRPC_EVENT *event );			/* HR(Collector) Event CALL BACK Declaration */


// Cycling Speed and Cadence Profile Call back define
enum RBLE_CSCP_EVENT_TYPE_enum {
	RBLE_CSCP_EVENT_SENSOR_ENABLE_COMP = 0x01,			/* Sensor enable Complete Event */
	RBLE_CSCP_EVENT_SENSOR_DISABLE_COMP,				/* Sensor disable Complete Event */
	RBLE_CSCP_EVENT_SENSOR_ERROR_IND,					/* Sensor error Complete Event */
	RBLE_CSCP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP,		/* Sensor send measurements Complete Event */
	RBLE_CSCP_EVENT_SENSOR_SEND_SC_CP_COMP,				/* Sensor send sc control point Complete Event */
	RBLE_CSCP_EVENT_SENSOR_CHG_SC_CP_IND,				/* Sensor sc control point change Complete Event */
	RBLE_CSCP_EVENT_SENSOR_CFG_INDNTF_IND,				/* Sensor configuration change Complete Event */
	RBLE_CSCP_EVENT_SENSOR_COMMAND_DISALLOWED_IND,		/* Sensor Command disallowed Complete Event */
	RBLE_CSCP_EVENT_COLLECTOR_ENABLE_COMP = 0x81,		/* Collector enable Complete Event */
	RBLE_CSCP_EVENT_COLLECTOR_DISABLE_COMP,				/* Collector disable Complete Event */
	RBLE_CSCP_EVENT_COLLECTOR_ERROR_IND,				/* Collector error Complete Event */
	RBLE_CSCP_EVENT_COLLECTOR_MEASUREMENTS_NTF,			/* Collector measurements Complete Event */
	RBLE_CSCP_EVENT_COLLECTOR_SC_CP_IND,				/* Collector sc control point Complete Event */
	RBLE_CSCP_EVENT_COLLECTOR_READ_CHAR_RESPONSE,		/* Collector read Characteristic Complete Event */
	RBLE_CSCP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE,		/* Collector write Characteristic Complete Event */
	RBLE_CSCP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND	/* Collector Command Disallowed Complete Event */
};

typedef uint8_t		RBLE_CSCP_EVENT_TYPE;

/*** Sensor ***/
typedef struct RBLE_CSCPS_EVENT_t {
	RBLE_CSCP_EVENT_TYPE	type;
	uint8_t					reserved;

	union Event_Cscs_Parameter_u {
		/* Generic Event */
		RBLE_STATUS			status;
		
		/* RBLE_EVT_CSCP_Sensor_Enable_Comp */
		struct RBLE_CSCP_Sensor_Enable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}sensor_enable;
		
		/* RBLE_EVT_CSCP_Sensor_Disable_Comp */
		struct RBLE_CSCP_Sensor_Disable_t{
			uint16_t				conhdl;
			RBLE_CSCP_SENSOR_PARAM	sensor_info;
		}sensor_disable;
		
		/* RBLE_EVT_CSCP_Sensor_Error_Ind */
		struct RBLE_CSCP_Sensor_Error_Ind_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}error_ind;
		
		/* RBLE_EVT_CSCP_Sensor_Send_Measurements_COMP */
		struct RBLE_CSCP_Sensor_Send_Measurements_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_measurements;
		
		/* RBLE_EVT_CSCP_Sensor_Send_SC_Control_Point_COMP */
		struct RBLE_CSCP_Sensor_Send_SC_Control_Point_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_sc_cp;
		
		/* RBLE_EVT_CSCP_Sensor_Chg_Sc_Cp_Ind */
		struct RBLE_CSCP_Sensor_Chg_Sc_Cp_Ind_t{
			uint16_t						conhdl;
			RBLE_CSCP_SC_CONTROL_POINT_INFO	sc_cp_info;
		}cscps_chg_sc_cp_ind;
		
		/* RBLE_EVT_CSCP_SENSOR_Cfg_IndNtf_Ind */
		struct RBLE_CSCP_Sensor_Cfg_indntf_Ind_t{
			uint16_t	conhdl;
			uint8_t		char_code;
			uint8_t		reserved;
			uint16_t	cfg_val;
		}cscps_cfg_indntf_ind;
		
		/* RBLE_EVT_CSCP_CMD_DISALLOWED_IND */
		struct RBLE_CSCP_Sensor_Command_Disallowed_Ind_t{
			RBLE_STATUS	status;
			uint8_t		reserved;
			uint16_t	opcode;
		}cmd_disallowed_ind;
	}param;
} RBLE_CSCPS_EVENT;

/*** Collector ***/
typedef struct RBLE_CSCPC_EVENT_t {
	RBLE_CSCP_EVENT_TYPE			type;
	uint8_t			reserved;
	
	union Event_Cscc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;
		
		/* RBLE_EVT_CSCP_Collector_Enable_Comp */
		struct RBLE_CSCP_Collector_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
			RBLE_CSCS_CONTENT	cscs;
			RBLE_DIS_CONTENT	dis;
		}collector_enable;
		
		/* RBLE_EVT_CSCP_Collector_Disable_Comp */
		struct RBLE_CSCP_Collector_Disable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}collector_disable;
		
		/* RBLE_EVT_CSCP_Collector_Error_Ind */
		struct RBLE_CSCP_Collector_Error_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}error_ind;
		
		/* RBLE_EVT_CSCP_Collector_Measurements_Ntf */
		struct RBLE_CSCP_Collector_Measurements_Ntf_t{
			uint16_t					conhdl;
			RBLE_CSCP_MEASUREMENTS_INFO	measure_info;
		}measurements_ntf;
		
		/* RBLE_EVT_CSCP_Collector_SC_CP_Ind */
		struct RBLE_CSCP_Collector_SC_CP_Ind_t{
			uint16_t						conhdl;
			RBLE_CSCP_SC_CONTROL_POINT_INFO	sc_cp_info;
			uint8_t							location_num;
			uint8_t							response_param[RBLE_CSCP_SENSORE_LOCATION_MAX];
		}sc_cp_ind;
		
		/* RBLE_EVT_CSCP_Collector_Read_Char_Response */
		struct RBLE_CSCP_Collector_Read_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			uint8_t				reserved;
			RBLE_ATT_INFO_DATA	data;
		}rd_char_resp;
		
		/* RBLE_EVT_CSCP_Collector_Write_Char_Response */
		struct RBLE_CSCP_Collector_Write_Char_Response_t{
			uint16_t	conhdl;
			uint8_t		att_code;
		}wr_char_resp;
		
		/* RBLE_EVT_CSCP_CMD_DISALLOWED_IND */
		struct RBLE_CSCP_Collector_Command_Disallowed_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_CSCPC_EVENT;

typedef void ( *RBLE_CSCPS_EVENT_HANDLER )( RBLE_CSCPS_EVENT *event );		/* CSC(Sensor) Event CALL BACK Declaration */
typedef void ( *RBLE_CSCPC_EVENT_HANDLER )( RBLE_CSCPC_EVENT *event );		/* CSC(Collector) Event CALL BACK Declaration */

// Glucose Profile Call back define
enum RBLE_GLP_EVENT_TYPE_enum {
	RBLE_GLP_EVENT_SENSOR_ENABLE_COMP = 0x01,				/* Sensor enable Complete Event */
	RBLE_GLP_EVENT_SENSOR_DISABLE_COMP,						/* Sensor disable Complete Event */
	RBLE_GLP_EVENT_SENSOR_ERROR_IND,						/* Sensor error Complete Event */
	RBLE_GLP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP,			/* Sensor send measurements Complete Event */
	RBLE_GLP_EVENT_SENSOR_SEND_MEASUREMENTS_CONTEXT_COMP,	/* Sensor send measurements context Complete Event */
	RBLE_GLP_EVENT_SENSOR_SEND_RA_CP_COMP,					/* Sensor send ra control point Complete Event */
	RBLE_GLP_EVENT_SENSOR_CHG_RA_CP_IND,					/* Sensor ra control point change Complete Event */
	RBLE_GLP_EVENT_SENSOR_CFG_INDNTF_IND,					/* Sensor configuration change Complete Event */
	RBLE_GLP_EVENT_SENSOR_COMMAND_DISALLOWED_IND,			/* Sensor Command disallowed Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_ENABLE_COMP = 0x81,			/* Collector enable Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_DISABLE_COMP,					/* Collector disable Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_ERROR_IND,						/* Collector error Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_MEASUREMENTS_NTF,				/* Collector measurements Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_MEASUREMENTS_CONTEXT_NTF,		/* Collector measurements context Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_RA_CP_IND,						/* Collector sc control point Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_READ_CHAR_RESPONSE,			/* Collector read Characteristic Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE,			/* Collector write Characteristic Complete Event */
	RBLE_GLP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND			/* Collector Command Disallowed Complete Event */
};

typedef uint8_t		RBLE_GLP_EVENT_TYPE;

/*** Sensor ***/
typedef struct RBLE_GLPS_EVENT_t {
	RBLE_GLP_EVENT_TYPE	type;
	uint8_t					reserved;

	union Event_Gls_Parameter_u {
		/* Generic Event */
		RBLE_STATUS			status;
		
		/* RBLE_EVT_GLP_Sensor_Enable_Comp */
		struct RBLE_GLP_Sensor_Enable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}sensor_enable;
		
		/* RBLE_EVT_GLP_Sensor_Disable_Comp */
		struct RBLE_GLP_Sensor_Disable_t{
			uint16_t				conhdl;
			RBLE_GLP_SENSOR_PARAM	sensor_info;
		}sensor_disable;
		
		/* RBLE_EVT_GLP_Sensor_Error_Ind */
		struct RBLE_GLP_Sensor_Error_Ind_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}error_ind;
		
		/* RBLE_EVT_GLP_Sensor_Send_Measurements_COMP */
		struct RBLE_GLP_Sensor_Send_Measurements_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_measurements;
		
		/* RBLE_EVT_GLP_Sensor_Send_Measurements_Context_COMP */
		struct RBLE_GLP_Sensor_Send_Measurements_Context_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_measurements_context;
		
		/* RBLE_EVT_GLP_Sensor_Send_RA_Control_Point_COMP */
		struct RBLE_GLP_Sensor_Send_RA_Control_Point_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_ra_cp;
		
		/* RBLE_EVT_GLP_Sensor_Chg_Ra_Cp_Ind */
		struct RBLE_GLP_Sensor_Chg_Ra_Cp_Ind_t{
			uint16_t						conhdl;
			RBLE_GLP_RA_CONTROL_POINT_INFO	ra_cp_info;
		}glps_chg_ra_cp_ind;
		
		/* RBLE_EVT_GLP_SENSOR_Cfg_IndNtf_Ind */
		struct RBLE_GLP_Sensor_Cfg_indntf_Ind_t{
			uint16_t	conhdl;
			uint8_t		char_code;
			uint8_t		reserved;
			uint16_t	cfg_val;
		}glps_cfg_indntf_ind;
		
		/* RBLE_EVT_GLP_CMD_DISALLOWED_IND */
		struct RBLE_GLP_Sensor_Command_Disallowed_Ind_t{
			RBLE_STATUS	status;
			uint8_t		reserved;
			uint16_t	opcode;
		}cmd_disallowed_ind;
	}param;
} RBLE_GLPS_EVENT;

/*** Collector ***/
typedef struct RBLE_GLPC_EVENT_t {
	RBLE_GLP_EVENT_TYPE			type;
	uint8_t			reserved;
	
	union Event_Glc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;
		
		/* RBLE_EVT_GLP_Collector_Enable_Comp */
		struct RBLE_GLP_Collector_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
			RBLE_GLS_CONTENT	gls;
			RBLE_DIS_CONTENT	dis;
		}collector_enable;
		
		/* RBLE_EVT_GLP_Collector_Disable_Comp */
		struct RBLE_GLP_Collector_Disable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}collector_disable;
		
		/* RBLE_EVT_GLP_Collector_Error_Ind */
		struct RBLE_GLP_Collector_Error_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}error_ind;
		
		/* RBLE_EVT_GLP_Collector_Measurements_Ntf */
		struct RBLE_GLP_Collector_Measurements_Ntf_t{
			uint16_t					conhdl;
			RBLE_GLP_MEASUREMENTS_INFO	measure_info;
		}measurements_ntf;
		
		/* RBLE_EVT_GLP_Collector_Measurements_Context_Ntf */
		struct RBLE_GLP_Collector_Measurements_Context_Ntf_t{
			uint16_t							conhdl;
			RBLE_GLP_MEASUREMENTS_CONTEXT_INFO	measure_context_info;
		}measurements_context_ntf;
		
		/* RBLE_EVT_GLP_Collector_RA_CP_Ind */
		struct RBLE_GLP_Collector_RA_CP_Ind_t{
			uint16_t						conhdl;
			RBLE_GLP_RA_CONTROL_POINT_IND_INFO	ra_cp_ind_info;
		}ra_cp_ind;
		
		/* RBLE_EVT_GLP_Collector_Read_Char_Response */
		struct RBLE_GLP_Collector_Read_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			uint8_t				reserved;
			RBLE_ATT_INFO_DATA	data;
		}rd_char_resp;
		
		/* RBLE_EVT_GLP_Collector_Write_Char_Response */
		struct RBLE_GLP_Collector_Write_Char_Response_t{
			uint16_t	conhdl;
			uint8_t		att_code;
		}wr_char_resp;
		
		/* RBLE_EVT_GLP_CMD_DISALLOWED_IND */
		struct RBLE_GLP_Collector_Command_Disallowed_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_GLPC_EVENT;

typedef void ( *RBLE_GLPS_EVENT_HANDLER )( RBLE_GLPS_EVENT *event );		/* CSC(Sensor) Event CALL BACK Declaration */
typedef void ( *RBLE_GLPC_EVENT_HANDLER )( RBLE_GLPC_EVENT *event );		/* CSC(Collector) Event CALL BACK Declaration */

// Cycling Power Profile Call back define
enum RBLE_CPP_EVENT_TYPE_enum {	
	RBLE_CPP_EVENT_SENSOR_ENABLE_COMP = 0x01,			/* Cycling Power enable Complete Event */
	RBLE_CPP_EVENT_SENSOR_DISABLE_COMP,					/* Cycling Power disable Complete Event */
	RBLE_CPP_EVENT_SENSOR_ERROR_IND,					/* Cycling Power error Complete Event */
	RBLE_CPP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP,		/* Cycling Power send measurements Complete Event */
	RBLE_CPP_EVENT_SENSOR_BROADCAST_MEASUREMENTS_COMP,	/* Cycling Power broadcast measurements Complete Event */
	RBLE_CPP_EVENT_SENSOR_SEND_VECTOR_COMP,				/* Cycling Power send vector Complete Event */
	RBLE_CPP_EVENT_SENSOR_SEND_CP_CP_COMP,				/* Cycling Power send control point Complete Event */
	RBLE_CPP_EVENT_SENSOR_SEND_BATTERY_LEVEL_COMP,		/* Cycling Power send battery levelt Complete Event */
	RBLE_CPP_EVENT_SENSOR_CHG_CP_CP_IND,				/* Cycling Power change control point Complete Event */
	RBLE_CPP_EVENT_SENSOR_CFG_INDNTFBRD_IND,			/* Cycling Power cfg indntfbrd Complete Event */
	RBLE_CPP_EVENT_SENSOR_COMMAND_DISALLOWED_IND,		/* Cycling Power command disallowed Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_ENABLE_COMP = 0x81,		/* Collector enable Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_DISABLE_COMP,				/* Collector disable Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_ERROR_IND,					/* Collector error Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_MEASUREMENTS_NTF,			/* Collector measurement Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_VECTOR_NTF,				/* Collector vector Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_CP_CP_IND,					/* Collector control point Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_BATTERY_LEVEL_NTF,			/* Collector battery level Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_READ_CHAR_RESPONSE,		/* Collector read Characteristic Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE,		/* Collector write Characteristic Complete Event */
	RBLE_CPP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND		/* Collector command disallowed Complete Event */
};

typedef uint8_t		RBLE_CPP_EVENT_TYPE;

/* Cycling Power Sensor */
typedef struct RBLE_CPPS_EVENT_t {
	RBLE_CPP_EVENT_TYPE		type;
	uint8_t					reserved;

	union Event_Cps_Parameter_u {
		RBLE_STATUS		status;

		/* RBLE_CPP_EVENT_SENSOR_ENABLE_COMP */
		struct RBLE_CPP_Sensor_Enable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}sensor_enable;

		/* RBLE_CPP_EVENT_SENSOR_DISABLE_COMP */
		struct RBLE_CPP_Sensor_Disable_t{
			uint16_t				conhdl;
			RBLE_CPP_SENSOR_PARAM	sensor_info;
		}sensor_disable;

		/* RBLE_CPP_EVENT_SENSOR_ERROR_IND */
		struct RBLE_CPP_Sensor_Error_Ind_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}error_ind;

		/* RBLE_CPP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP */
		struct RBLE_CPP_Sensor_Send_Measurements_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_measurements;

		/* RBLE_CPP_EVENT_SENSOR_BROADCAST_MEASUREMENTS_COMP */
		struct RBLE_CPP_Sensor_Broadcast_Measurements_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}broadcast_measurements;

		/* RBLE_CPP_EVENT_SENSOR_SEND_VECTOR_COMP */
		struct RBLE_CPP_Sensor_Send_Vector_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_vector;

		/* RBLE_CPP_EVENT_SENSOR_SEND_CP_CP_COMP */
		struct RBLE_CPP_Sensor_Send_CP_Control_Point_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_cp_cp;

		/* RBLE_CPP_EVENT_SENSOR_SEND_BATTERY_LEVEL_COMP */
		struct RBLE_CPP_Sensor_Send_CP_Battery_Level_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_battery_level;

		/* RBLE_CPP_EVENT_SENSOR_CHG_CP_CP_IND */
		struct RBLE_CPP_Sensor_Chg_Cp_Cp_Ind_t{
			uint16_t						conhdl;
			RBLE_CPP_WR_CONTROL_POINT_INFO	wr_cp_info;
		}chg_cp_cp_ind;

		/* RBLE_CPP_EVENT_SENSOR_CFG_INDNTFBRD_IND */
		struct RBLE_CPP_Sensor_Cfg_Indntfbrd_Ind_t{
			uint16_t	conhdl;
			uint8_t		char_code;
			uint8_t		reserved;
			uint16_t	cfg_val;
		}cfg_indntfbrd_ind;

		/* RBLE_CPP_EVENT_SENSOR_COMMAND_DISALLOWED_IND */
		struct RBLE_CPP_Sensor_Command_Disallowed_Ind_t{
			RBLE_STATUS	status;
			uint8_t		reserved;
			uint16_t	opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_CPPS_EVENT;

/* Cycling Power Collector */
typedef struct RBLE_CPPC_EVENT_t {
	RBLE_CPP_EVENT_TYPE			type;
	uint8_t			reserved;

	union Event_Cpc_Parameter_u {
		RBLE_STATUS		status;

		/* RBLE_CPP_EVENT_COLLECTOR_ENABLE_COMP */
		struct RBLE_CPP_Collector_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
			RBLE_CPS_CONTENT	cps;
			RBLE_DIS_CONTENT	dis;
			RBLE_BATS_CONTENT	bas;
		}collector_enable;

		/* RBLE_CPP_EVENT_COLLECTOR_DISABLE_COMP */
		struct RBLE_CPP_Collector_Disable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}collector_disable;

		/* RBLE_CPP_EVENT_COLLECTOR_ERROR_IND */
		struct RBLE_CPP_Collector_Error_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}error_ind;

		/* RBLE_CPP_EVENT_COLLECTOR_MEASUREMENTS_NTF */
		struct RBLE_CPP_Collector_Measurements_Ntf_t{
			uint16_t						conhdl;
			RBLE_CPP_MEASUREMENTS_INFO		measurements_info;
		}measurements_ntf;

		/* RBLE_CPP_EVENT_COLLECTOR_VECTOR_NTF */
		struct RBLE_CPP_Collector_Vector_Ntf_t{
			uint16_t				conhdl;
			RBLE_CPP_VECTOR_INFO	vector_info;
		}vector_ntf;

		/* RBLE_CPP_EVENT_COLLECTOR_CP_CP_IND */
		struct RBLE_CPP_Collector_Cp_Control_Point_Ind_t{
			uint16_t						conhdl;
			RBLE_CPP_IND_CONTROL_POINT_INFO	ind_cp_info;
			uint8_t							location_num;
			uint8_t  						supported_location[RBLE_CPP_SENSORE_LOCATION_MAX];
		}cp_cp_ind;

		/* RBLE_CPP_EVENT_COLLECTOR_BATTERY_LEVEL_NTF */
		struct RBLE_CPP_Collector_Battery_Level_Ntf_t{
			uint16_t				conhdl;
			uint8_t					battery_level;
			uint8_t					reserved;
		}battery_level_ntf;

		/* RBLE_CPP_EVENT_COLLECTOR_READ_CHAR_RESPONSE */
		struct RBLE_CPP_Collector_Read_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			uint8_t				reserved;
			RBLE_ATT_INFO_DATA	data;
		}rd_char_resp;

		/* RBLE_CPP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE */
		struct RBLE_CPP_Collector_Write_Char_Response_t{
			uint16_t	conhdl;
			uint8_t		att_code;
		}wr_char_resp;

		/* RBLE_CPP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND */
		struct RBLE_CPP_Collector_Command_Disallowed_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_CPPC_EVENT;

typedef void ( *RBLE_CPPS_EVENT_HANDLER )( RBLE_CPPS_EVENT *event );			/* CP(Sensor) Event CALL BACK Declaration */
typedef void ( *RBLE_CPPC_EVENT_HANDLER )( RBLE_CPPC_EVENT *event );			/* CP(Client) Event CALL BACK Declaration */


// Time Profile Call back define

enum RBLE_TIP_EVENT_TYPE_enum {	
	RBLE_TIP_EVENT_SERVER_ENABLE_COMP = 0x01,		/* Server enable Complete Event */
	RBLE_TIP_EVENT_SERVER_DISABLE_COMP,				/* Server disable Complete Event */
	RBLE_TIP_EVENT_SERVER_ERROR_IND,				/* Server error Complete Event */
	RBLE_TIP_EVENT_SERVER_SEND_CURRENT_TIME_COMP,	/* Server send current time Complete Event */
	RBLE_TIP_EVENT_SERVER_WRITE_DATA_COMP,			/* Server send current time Complete Event */
	RBLE_TIP_EVENT_SERVER_CHG_TIME_UPDATE_CP_IND,	/* Server changed time update control point chara Event */
	RBLE_TIP_EVENT_SERVER_CFG_NTF_IND,				/* Server changed notify setting Event */
	RBLE_TIP_EVENT_SERVER_COMMAND_DISALLOWED_IND,	/* Server Command disallowed Complete Event */
	RBLE_TIP_EVENT_CLIENT_ENABLE_COMP = 0x81,		/* Client enable Complete Event */
	RBLE_TIP_EVENT_CLIENT_DISABLE_COMP,				/* Client disable Complete Event */
	RBLE_TIP_EVENT_CLIENT_ERROR_IND,				/* Client error Complete Event */
	RBLE_TIP_EVENT_CLIENT_CURRENT_TIME_NTF,			/* Client Current Time Receive Event */
	RBLE_TIP_EVENT_CLIENT_READ_CHAR_RESPONSE,		/* Client read Characteristic Complete Event */
	RBLE_TIP_EVENT_CLIENT_WRITE_CHAR_RESPONSE,		/* Client write Characteristic Complete Event */
	RBLE_TIP_EVENT_CLIENT_COMMAND_DISALLOWED_IND	/* Client Command Disallowed Complete Event */
};

typedef uint8_t		RBLE_TIP_EVENT_TYPE;

/*** Server ***/
typedef struct RBLE_TIPS_EVENT_t {
	RBLE_TIP_EVENT_TYPE	type;
	uint8_t				reserved;
	
	union Event_Tips_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;
		
		/* RBLE_TIP_EVENT_SERVER_ENABLE_COMP */
		struct RBLE_TIP_Server_Enable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}server_enable;
		
		/* RBLE_TIP_EVENT_SERVER_DISABLE_COMP */
		struct RBLE_TIP_Server_Disable_t{
			uint16_t		conhdl;
			uint16_t		current_time_ntf_en;
		}server_disable;
		
		/* RBLE_TIP_EVENT_SERVER_ERROR_IND */
		struct RBLE_TIP_Server_Error_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}error_ind;
		
		/* RBLE_TIP_EVENT_SERVER_SEND_CURRENT_TIME_COMP */
		struct RBLE_TIP_Server_Send_Current_Time_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}send_current_time;
		
		/* RBLE_TIP_EVENT_SERVER_WRITE_DATA_COMP */
		struct RBLE_TIP_Server_Write_Data_Comp_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
		}write_data;
		
		/* RBLE_TIP_EVENT_SERVER_CHG_TIME_UPDATE_CP_IND */
		struct RBLE_TIP_Server_Chg_Time_Update_Cp_Ind_t{
			uint16_t		conhdl;
			uint8_t			update_cp;
			uint8_t			reserved;
		}chg_cp_ind;
		
		/* RBLE_TIP_EVENT_SERVER_CFG_NTF_IND */
		struct RBLE_TIP_Server_Cfg_ntf_Ind_t{
			uint16_t		conhdl;
			uint16_t		cfg_val;
		}cfg_ntf_ind;
		
		/* RBLE_TIP_EVENT_SERVER_COMMAND_DISALLOWED_IND */
		struct RBLE_TIP_Server_Command_Disallowed_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_TIPS_EVENT;

/*** Client ***/
typedef struct RBLE_TIPC_EVENT_t {
	RBLE_TIP_EVENT_TYPE		type;
	uint8_t					reserved;
	
	union Event_Tipc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;
		
		/* RBLE_TIP_EVENT_CLIENT_ENABLE_COMP */
		struct RBLE_TIP_Client_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
			RBLE_CTS_CONTENT	cts;
			RBLE_NDCS_CONTENT	ndcs;
			RBLE_RTUS_CONTENT	rtus;
		}client_enable;
		
		/* RBLE_TIP_EVENT_CLIENT_DISABLE_COMP */
		struct RBLE_TIP_Client_Disable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
		}client_disable;
		
		/* RBLE_TIP_EVENT_CLIENT_ERROR_IND */
		struct RBLE_TIP_Client_Error_Ind_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
		}error_ind;
		
		/* RBLE_TIP_EVENT_CLIENT_CURRENT_TIME_NTF */
		struct RBLE_TIP_Client_Current_time_Ntf_t{
			uint16_t				conhdl;
			RBLE_TIP_CURRENT_TIME	current_time;
		}current_time_ntf;
		
		/* RBLE_TIP_EVENT_CLIENT_READ_CHAR_RESPONSE */
		struct RBLE_TIP_Client_Read_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			uint8_t				reserved;
			RBLE_ATT_INFO_DATA	data;
		}rd_char_resp;
		
		/* RBLE_TIP_EVENT_CLIENT_WRITE_CHAR_RESPONSE */
		struct RBLE_TIP_Client_Write_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			uint8_t				reserved;
		}wr_char_resp;
		
		/* RBLE_TIP_EVENT_CLIENT_COMMAND_DISALLOWED_IND */
		struct RBLE_TIP_Client_Command_Disallowed_Ind_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_TIPC_EVENT;

typedef void ( *RBLE_TIPS_EVENT_HANDLER )( RBLE_TIPS_EVENT *event );			/* TI(Server) Event CALL BACK Declaration */
typedef void ( *RBLE_TIPC_EVENT_HANDLER )( RBLE_TIPC_EVENT *event );			/* TI(Client) Event CALL BACK Declaration */

// Alert Notification Profile Call back define

enum RBLE_ANP_EVENT_TYPE_enum {	
	RBLE_ANP_EVENT_SERVER_ENABLE_COMP = 0x01,			/* Server enable Complete Event */
	RBLE_ANP_EVENT_SERVER_DISABLE_COMP,					/* Server disable Complete Event */
	RBLE_ANP_EVENT_SERVER_ERROR_IND,					/* Server error Complete Event */
	RBLE_ANP_EVENT_SERVER_SEND_NEW_ALERT_COMP,			/* Server send new alert Complete Event */
	RBLE_ANP_EVENT_SERVER_SEND_UNREAD_ALERT_COMP,		/* Server send unread alert status Complete Event */
	RBLE_ANP_EVENT_SERVER_CHG_ALERT_NTF_CP_IND,			/* Server changed alert notification control point chara Event */
	RBLE_ANP_EVENT_SERVER_CFG_NTF_IND,					/* Server changed notify setting Event */
	RBLE_ANP_EVENT_SERVER_COMMAND_DISALLOWED_IND,		/* Server Command disallowed Complete Event */
	RBLE_ANP_EVENT_CLIENT_ENABLE_COMP = 0x81,			/* Client enable Complete Event */
	RBLE_ANP_EVENT_CLIENT_DISABLE_COMP,					/* Client disable Complete Event */
	RBLE_ANP_EVENT_CLIENT_ERROR_IND,					/* Client error Complete Event */
	RBLE_ANP_EVENT_CLIENT_NEW_ALERT_NTF,				/* Client new alert Receive Event */
	RBLE_ANP_EVENT_CLIENT_UNREAD_ALERT_NTF,				/* Client unread alert status Receive Event */
	RBLE_ANP_EVENT_CLIENT_READ_CHAR_RESPONSE,			/* Client read Characteristic Complete Event */
	RBLE_ANP_EVENT_CLIENT_WRITE_CHAR_RESPONSE,			/* Client write Characteristic Complete Event */
	RBLE_ANP_EVENT_CLIENT_COMMAND_DISALLOWED_IND		/* Client Command Disallowed Complete Event */
};

typedef uint8_t		RBLE_ANP_EVENT_TYPE;

/*** Server ***/
typedef struct RBLE_ANPS_EVENT_t {
	RBLE_ANP_EVENT_TYPE	type;
	uint8_t				reserved;
	
	union Event_Anps_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;
		
		/* RBLE_ANP_EVENT_SERVER_ENABLE_COMP */
		struct RBLE_ANP_Server_Enable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}server_enable;
		
		/* RBLE_ANP_EVENT_SERVER_DISABLE_COMP */
		struct RBLE_ANP_Server_Disable_t{
			uint16_t		conhdl;
			uint16_t		new_alert_ntf_en;
			uint16_t		unread_alert_ntf_en;
		}server_disable;
		
		/* RBLE_ANP_EVENT_SERVER_ERROR_IND */
		struct RBLE_ANP_Server_Error_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}error_ind;
		
		/* RBLE_ANP_EVENT_SERVER_SEND_NEW_ALERT_COMP */
		struct RBLE_ANP_Server_Send_New_Alert_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}send_new_alert;
		
		/* RBLE_ANP_EVENT_SERVER_SEND_UNREAD_ALERT_COMP */
		struct RBLE_ANP_Server_Send_Unread_Alert_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}send_unread_alert;
		
		/* RBLE_ANP_EVENT_SERVER_CHG_ALERT_NTF_CP_IND */
		struct RBLE_ANP_Server_Chg_Alert_Ntf_Cp_Ind_t{
			uint16_t				conhdl;
		 	RBLE_ANP_ALERT_NTF_CP	cp_info;
		}chg_cp_ind;
		
		/* RBLE_ANP_EVENT_SERVER_CFG_NTF_IND */
		struct RBLE_ANP_Server_Cfg_Ntf_Ind_t{
			uint16_t		conhdl;
			uint8_t			char_code;
			uint8_t			reserved;
			uint16_t		cfg_val;
		}cfg_ntf_ind;
		
		/* RBLE_ANP_EVENT_SERVER_COMMAND_DISALLOWED_IND */
		struct RBLE_ANP_Server_Command_Disallowed_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_ANPS_EVENT;

/*** Client ***/
typedef struct RBLE_ANPC_EVENT_t {
	RBLE_ANP_EVENT_TYPE		type;
	uint8_t					reserved;
	
	union Event_Anpc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;
		
		/* RBLE_ANP_EVENT_CLIENT_ENABLE_COMP */
		struct RBLE_ANP_Clien_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
			RBLE_ANS_CONTENT	ans;
		}client_enable;
		
		/* RBLE_ANP_EVENT_CLIENT_DISABLE_COMP */
		struct RBLE_ANP_Client_Disable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
		}client_disable;
		
		/* RBLE_ANP_EVENT_CLIENT_ERROR_IND */
		struct RBLE_ANP_Client_Error_Ind_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
		}error_ind;
		
		/* RBLE_ANP_EVENT_CLIENT_NEW_ALERT_NTF */
		struct RBLE_ANP_Client_New_Alert_Ntf_t{
			uint16_t					conhdl; 
			RBLE_ANP_NEW_ALERT_INFO		new_alert;
		}new_alert_ntf;
		
		/* RBLE_ANP_EVENT_CLIENT_UNREAD_ALERT_NTF */
		struct RBLE_ANP_Client_Unread_Alert_Ntf_t{
			uint16_t					conhdl; 
			RBLE_ANP_UNREAD_ALERT_INFO	unread_alert;
		}unread_alert_ntf;
		
		/* RBLE_ANP_EVENT_CLIENT_READ_CHAR_RESPONSE */
		struct RBLE_ANP_Client_Read_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			uint8_t				reserved;
			RBLE_ATT_INFO_DATA	data;
		}rd_char_resp;
		
		/* RBLE_ANP_EVENT_CLIENT_WRITE_CHAR_RESPONSE */
		struct RBLE_ANP_Client_Write_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
		}wr_char_resp;
		
		/* RBLE_ANP_EVENT_CLIENT_COMMAND_DISALLOWED_IND */
		struct RBLE_ANP_Client_Command_Disallowed_Ind_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_ANPC_EVENT;

typedef void ( *RBLE_ANPS_EVENT_HANDLER )( RBLE_ANPS_EVENT *event );			/* AN(Server) Event CALL BACK Declaration */
typedef void ( *RBLE_ANPC_EVENT_HANDLER )( RBLE_ANPC_EVENT *event );			/* AN(Client) Event CALL BACK Declaration */

// Location and Navigation Profile Call back define

enum RBLE_LNP_EVENT_TYPE_enum {
	RBLE_LNP_EVENT_SENSOR_ENABLE_COMP = 0x01,			/* Location and Navigation enable Complete Event */
	RBLE_LNP_EVENT_SENSOR_DISABLE_COMP,					/* Location and Navigation disable Complete Event */
	RBLE_LNP_EVENT_SENSOR_ERROR_IND,					/* Location and Navigation error Complete Event */
	RBLE_LNP_EVENT_SENSOR_SEND_LOCATION_SPEED_COMP,		/* Location and Navigation send location and speed Complete Event */
	RBLE_LNP_EVENT_SENSOR_SET_POSITION_QUALITY_COMP, 	/* Location and Navigation sey position quality Complete Event */
	RBLE_LNP_EVENT_SENSOR_SEND_LN_CP_COMP,				/* Location and Navigation send control point Complete Event */
	RBLE_LNP_EVENT_SENSOR_SEND_NAVIGATION_COMP,			/* Location and Navigation send navigation Complete Event */
	RBLE_LNP_EVENT_SENSOR_SEND_BATTERY_LEVEL_COMP,		/* Location and Navigation send battery level Complete Event */
	RBLE_LNP_EVENT_SENSOR_CHG_LN_CP_IND,				/* Location and Navigation change control point Complete Event */
	RBLE_LNP_EVENT_SENSOR_CFG_INDNTF_IND,				/* Location and Navigation cfg indntf Complete Event */
	RBLE_LNP_EVENT_SENSOR_COMMAND_DISALLOWED_IND,		/* Location and Navigation command disallowed Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_ENABLE_COMP = 0x81,		/* Collector enable Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_DISABLE_COMP,				/* Collector disable Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_ERROR_IND,					/* Collector error Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_LOCATION_SPEED_NTF, 		/* Collector location and speed Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_LN_CP_IND,					/* Collector control point Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_NAVIGATION_NTF,			/* Collector navigation Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_BATTERY_LEVEL_NTF,			/* Collector battery level Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_READ_CHAR_RESPONSE, 		/* Collector read Characteristic Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE,		/* Collector write Characteristic Complete Event */
	RBLE_LNP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND		/* Collector command disallowed Complete Event */
};

typedef uint8_t		RBLE_LNP_EVENT_TYPE;

/* Location and Navigation Sensor */
typedef struct RBLE_LNPS_EVENT_t {
	RBLE_LNP_EVENT_TYPE	type;
	uint8_t				reserved;
	
	union Event_Lns_Parameter_u {
		RBLE_STATUS	status;
		
		/* RBLE_LNP_EVENT_SENSOR_ENABLE_COMP */
		struct RBLE_LNP_Sensor_Enable_t{
			RBLE_STATUS	status;
			uint8_t		reserved;
			uint16_t	conhdl;
		}sensor_enable;

		/* RBLE_LNP_EVENT_SENSOR_DISABLE_COMP */
		struct RBLE_LNP_Sensor_Disable_t{
			uint16_t	conhdl;
			RBLE_LNP_SENSOR_PARAM sensor_info;
		}sensor_disable;

		/* RBLE_LNP_EVENT_SENSOR_ERROR_IND */
		struct RBLE_LNP_Sensor_Error_Ind_t{
			uint16_t	conhdl;
			RBLE_STATUS	status;
		}error_ind;

		/* RBLE_LNP_EVENT_SENSOR_SEND_LOCATION_SPEED_COMP */
		struct RBLE_LNP_Sensor_Send_Location_Speed_t{
			uint16_t	conhdl;
			RBLE_STATUS	status;
		}send_location_speed;
		
		/* RBLE_LNP_EVENT_SENSOR_SET_POSITION_QUALITY_COMP */
		struct RBLE_LNP_Sensor_Set_Position_Quality_t{
			RBLE_STATUS	status;
		}set_position_quality;

		/* RBLE_LNP_EVENT_SENSOR_SEND_LN_CP_COMP */
		struct RBLE_LNP_Sensor_Send_LN_Control_Point_t{
			uint16_t	conhdl;
			RBLE_STATUS	status;
		}send_ln_cp;

		/* RBLE_LNP_EVENT_SENSOR_SEND_NAVIGATION_COMP */
		struct RBLE_LNP_Sensor_Send_Navigation_t{
			uint16_t	conhdl;
			RBLE_STATUS	status;
		}send_navigation;

		/* RBLE_LNP_EVENT_SENSOR_SEND_BATTERY_LEVEL_COMP */
		struct RBLE_LNP_Sensor_Send_Battery_Level_t{
			uint16_t	conhdl;
			RBLE_STATUS	status;
		}send_battery_level;

		/* RBLE_LNP_EVENT_SENSOR_CHG_LN_CP_IND */
		struct RBLE_LNP_Sensor_Chg_Ln_Cp_Ind_t{
			uint16_t	conhdl;
			RBLE_LNP_WR_CONTROL_POINT_INFO	wr_cp_info;
		}chg_ln_cp_ind;

		/* RBLE_LNP_EVENT_SENSOR_CFG_INDNTF_IND */
		struct RBLE_LNP_Sensor_Cfg_indntf_Ind_t{
			uint16_t	conhdl;
		    uint8_t		char_code;
		    uint8_t		reserved;
		    uint16_t	cfg_val;
		}cfg_indntf_ind;

		/* RBLE_LNP_EVENT_SENSOR_COMMAND_DISALLOWED_IND */
		struct RBLE_LNP_Sensor_Command_Disallowed_Ind_t{
			RBLE_STATUS	status;
			uint8_t		reserved;
			uint16_t	opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_LNPS_EVENT;

/* Location and Navigation Collector */
typedef struct RBLE_LNPC_EVENT_t {
	RBLE_LNP_EVENT_TYPE	type;
	uint8_t				reserved;
	
	union Event_Lnc_Parameter_u {
		RBLE_STATUS	status;

		/* RBLE_LNP_EVENT_COLLECTOR_ENABLE_COMP */
		struct RBLE_LNP_Collector_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
			RBLE_LNS_CONTENT	lns;
			RBLE_DIS_CONTENT	dis;
			RBLE_BATS_CONTENT	bas;
		}collector_enable;

		/* RBLE_LNP_EVENT_COLLECTOR_DISABLE_COMP */
		struct RBLE_LNP_Collector_Disable_t{
			RBLE_STATUS	status;
			uint8_t		reserved;
			uint16_t	conhdl;
		}collector_disable;

		/* RBLE_LNP_EVENT_COLLECTOR_ERROR_IND */
		struct RBLE_LNP_Collector_Error_Ind_t{
			RBLE_STATUS	status;
			uint8_t		reserved;
			uint16_t	conhdl;
		}error_ind;

		/* RBLE_LNP_EVENT_COLLECTOR_LOCATION_SPEED_NTF */
		struct RBLE_LNP_Collector_Location_Speed_Ntf_t{
			uint16_t	conhdl;
			RBLE_LNP_LOCATION_SPEED_INFO location_speed_info;
		}location_speed_ntf;

		/* RBLE_LNP_EVENT_COLLECTOR_LN_CP_IND */
		struct RBLE_LNP_Collector_LN_CP_Ind_t{
			uint16_t	conhdl;
			RBLE_LNP_IND_CONTROL_POINT_INFO ind_cp_info;
		}ln_cp_ind;

		/* RBLE_LNP_EVENT_COLLECTOR_NAVIGATION_NTF */
		struct RBLE_LNP_Collector_Navigation_Ntf_t{
			uint16_t	conhdl;
			RBLE_LNP_NAVIGATION_INFO navigation_info;
		}navigation_ntf;

		/* RBLE_LNP_EVENT_COLLECTOR_BATTERY_LEVEL_NTF */
		struct RBLE_LNP_Collector_Battery_Level_Ntf_t{
			uint16_t	conhdl;
			uint8_t		battery_level;
		}battery_level_ntf;

		/* RBLE_LNP_EVENT_COLLECTOR_READ_CHAR_RESPONSE */
		struct RBLE_LNP_Collector_Read_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			uint8_t				reserved;
			RBLE_ATT_INFO_DATA	data;
		}rd_char_resp;

		/* RBLE_LNP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE */
		struct RBLE_LNP_Collector_Write_Char_Response_t{
			uint16_t	conhdl;
			uint8_t		att_code;
		}wr_char_resp;

		/* RBLE_LNP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND */
		struct RBLE_LNP_Collector_Command_Disallowed_Ind_t{
			RBLE_STATUS	status;
			uint8_t		reserved;
			uint16_t	opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_LNPC_EVENT;

typedef void ( *RBLE_LNPS_EVENT_HANDLER )( RBLE_LNPS_EVENT *event );			/* LN(Sensor) Event CALL BACK Declaration */
typedef void ( *RBLE_LNPC_EVENT_HANDLER )( RBLE_LNPC_EVENT *event );			/* LN(Collector) Event CALL BACK Declaration */

// Phone Alert Status Profile Call back define
enum RBLE_PASP_EVENT_TYPE_enum {
	RBLE_PASP_EVENT_SERVER_ENABLE_COMP = 0x01,			/* Server enable Complete Event */
	RBLE_PASP_EVENT_SERVER_DISABLE_COMP,				/* Server disable Complete Event */
	RBLE_PASP_EVENT_SERVER_ERROR_IND,					/* Server error Complete Event */
	RBLE_PASP_EVENT_SERVER_SEND_ALERT_STATUS_COMP,		/* Server send alert status Complete Event */
	RBLE_PASP_EVENT_SERVER_SEND_RINGER_SETTING_COMP,	/* Server send ringer setting Complete Event */
	RBLE_PASP_EVENT_SERVER_CHG_RINGER_CP_IND,			/* Server changed ringer control point chara Event */
	RBLE_PASP_EVENT_SERVER_CFG_NTF_IND,					/* Server changed notify setting Event */
	RBLE_PASP_EVENT_SERVER_COMMAND_DISALLOWED_IND,		/* Server Command disallowed Complete Event */
	RBLE_PASP_EVENT_CLIENT_ENABLE_COMP = 0x81,			/* Client enable Complete Event */
	RBLE_PASP_EVENT_CLIENT_DISABLE_COMP,				/* Client disable Complete Event */
	RBLE_PASP_EVENT_CLIENT_ERROR_IND,					/* Client error Complete Event */
	RBLE_PASP_EVENT_CLIENT_ALERT_STATUS_NTF,			/* Client alert status Receive Event */
	RBLE_PASP_EVENT_CLIENT_RINGER_SETTING_NTF,			/* Client ringer setting Receive Event */
	RBLE_PASP_EVENT_CLIENT_READ_CHAR_RESPONSE,			/* Client read Characteristic Complete Event */
	RBLE_PASP_EVENT_CLIENT_WRITE_CHAR_RESPONSE,			/* Client write Characteristic Complete Event */
	RBLE_PASP_EVENT_CLIENT_COMMAND_DISALLOWED_IND		/* Client Command Disallowed Complete Event */
};

typedef uint8_t		RBLE_PASP_EVENT_TYPE;

typedef struct RBLE_PASPS_EVENT_t {
	RBLE_PASP_EVENT_TYPE	type;			/* Event Type */
	uint8_t					reserved;
	union Event_Pass_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_PASP_EVENT_SERVER_ENABLE_COMP */
		struct RBLE_PASP_Server_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
		}server_enable;

		/* RBLE_PASP_EVENT_SERVER_DISABLE_COMP */
		struct RBLE_PASP_Server_Disable_t{
			uint16_t			conhdl;
			RBLE_PASP_SERVER_PARAM server_info;
		}server_disable;

		/* RBLE_PASP_EVENT_SERVER_ERROR_IND */
		struct RBLE_PASP_Server_Error_Ind_t{
			uint16_t			conhdl;
			RBLE_STATUS			status;
		}error_ind;

		/* RBLE_PASP_EVENT_SERVER_SEND_ALERT_STATUS_COMP */
		struct RBLE_PASP_Server_Send_Alert_Status_t{
			uint16_t			conhdl;
			RBLE_STATUS			status;
		}send_alert;

		/* RBLE_PASP_EVENT_SERVER_SEND_RINGER_SETTING_COMP */
		struct RBLE_PASP_Server_Send_Ringer_Setting_t{
			uint16_t			conhdl;
			RBLE_STATUS			status;
		}send_ringer;

		/* RBLE_PASP_EVENT_SERVER_CHG_RINGER_CP_IND */
		struct RBLE_PASP_Server_Chg_Ringer_CP_t{
			uint16_t			conhdl;
			uint8_t				cp_val;
		}chg_ringer_cp_ind;

		/* RBLE_PASP_EVENT_SERVER_CFG_NTF_IND */
		struct RBLE_PASP_Server_Cfg_Ntf_Ind_t{
			uint16_t 			conhdl;
			uint8_t 			char_code;
			uint8_t				reserved;
			uint16_t 			cfg_val;
		}cfg_ntf_ind;

		/* RBLE_PASP_EVENT_SERVER_COMMAND_DISALLOWED_IND */
		struct RBLE_PASP_Server_Command_Disallowed_Ind_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_PASPS_EVENT;

/*** Client ***/
typedef struct RBLE_PASPC_EVENT_t {
	RBLE_PASP_EVENT_TYPE		type;			/* Event Type */
	uint8_t						reserved;
	union Event_Pasc_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_PASP_EVENT_CLIENT_ENABLE_COMP */
		struct RBLE_PASP_Client_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
			RBLE_PASS_CONTENT	pass;
		}client_enable;

		/* RBLE_PASP_EVENT_CLIENT_DISABLE_COMP */
		struct RBLE_PASP_Client_Disable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
		}client_disable;

		/* RBLE_PASP_EVENT_CLIENT_ERROR_IND */
		struct RBLE_PASP_Client_Error_Ind_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
		}error_ind;

		/* RBLE_PASP_EVENT_CLIENT_ALERT_STATUS_NTF */
		struct RBLE_PASP_Client_Alert_Status_Ntf_t{
			uint16_t			conhdl;
			uint8_t				alert_status;
		}alert_ntf;

		/* RBLE_PASP_EVENT_CLIENT_RINGER_SETTING_NTF */
		struct RBLE_PASP_Client_Ringer_Setting_Ntf_t{
			uint16_t			conhdl;
			uint8_t				ringer_setting;
		}ringer_ntf;
		
		/* RBLE_PASP_EVENT_CLIENT_READ_CHAR_RESPONSE */
		struct RBLE_PASP_Client_Read_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			RBLE_ATT_INFO_DATA	data;
		}rd_char_resp;
		
		/* RBLE_PASP_EVENT_CLIENT_WRITE_CHAR_RESPONSE */
		struct RBLE_PASP_Client_Write_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
		}wr_char_resp;

		/* RBLE_PASP_EVENT_CLIENT_COMMAND_DISALLOWED_IND */
		struct RBLE_PASP_Client_Command_Disallowed_Ind_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_PASPC_EVENT;

typedef void ( *RBLE_PASPS_EVENT_HANDLER )( RBLE_PASPS_EVENT *event );			/* PAS(Server) Event CALL BACK Declaration */
typedef void ( *RBLE_PASPC_EVENT_HANDLER )( RBLE_PASPC_EVENT *event );			/* PAS(Client) Event CALL BACK Declaration */

// Running Speed and Cadence Profile Call back define
enum RBLE_RSCP_EVENT_TYPE_enum {	
	RBLE_RSCP_EVENT_SENSOR_ENABLE_COMP,					/* Running Speed and Cadence enable Complete Event */
	RBLE_RSCP_EVENT_SENSOR_DISABLE_COMP,				/* Running Speed and Cadence disable Complete Event */
	RBLE_RSCP_EVENT_SENSOR_ERROR_IND,					/* Running Speed and Cadence error Complete Event */
	RBLE_RSCP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP,		/* Running Speed and Cadence send measurements Complete Event */
	RBLE_RSCP_EVENT_SENSOR_SEND_SC_CP_COMP,				/* Running Speed and Cadence send control point Complete Event */
	RBLE_RSCP_EVENT_SENSOR_CHG_SC_CP_IND,				/* Running Speed and Cadence change control point Complete Event */
	RBLE_RSCP_EVENT_SENSOR_CFG_INDNTF_IND,				/* Running Speed and Cadence cfg indntf Complete Event */
	RBLE_RSCP_EVENT_SENSOR_COMMAND_DISALLOWED_IND,		/* Running Speed and Cadence command disallowed Complete Event */
	RBLE_RSCP_EVENT_COLLECTOR_ENABLE_COMP = 0x81,		/* Collector enable Complete Event */
	RBLE_RSCP_EVENT_COLLECTOR_DISABLE_COMP,				/* Collector disable Complete Event */
	RBLE_RSCP_EVENT_COLLECTOR_ERROR_IND,				/* Collector error Complete Event */
	RBLE_RSCP_EVENT_COLLECTOR_MEASUREMENTS_NTF,			/* Collector measurement Complete Event */
	RBLE_RSCP_EVENT_COLLECTOR_SC_CP_IND,				/* Collector control point Complete Event */
	RBLE_RSCP_EVENT_COLLECTOR_READ_CHAR_RESPONSE,		/* Collector read Characteristic Complete Event */
	RBLE_RSCP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE,		/* Collector write Characteristic Complete Event */
	RBLE_RSCP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND	/* Collector command disallowed Complete Event */
};

typedef uint8_t		RBLE_RSCP_EVENT_TYPE;

/* Running Speed and Cadence Sensor */
typedef struct RBLE_RSCPS_EVENT_t {
	RBLE_RSCP_EVENT_TYPE	type;
	uint8_t					reserved;
	
	union Event_Rscs_Parameter_u {
		RBLE_STATUS		status;

		/* RBLE_RSCP_EVENT_SENSOR_ENABLE_COMP */
		struct RBLE_RSCP_Sensor_Enable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}sensor_enable;

		/* RBLE_RSCP_EVENT_SENSOR_DISABLE_COMP */
		struct RBLE_RSCP_Sensor_Disable_t{
			uint16_t				conhdl;
			RBLE_RSCP_SENSOR_PARAM	sensor_info;
		}sensor_disable;

		/* RBLE_RSCP_EVENT_SENSOR_ERROR_IND */
		struct RBLE_RSCP_Sensor_Error_Ind_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}error_ind;

		/* RBLE_RSCP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP */
		struct RBLE_RSCP_Sensor_Send_Measurements_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_measurements;

		/* RBLE_RSCP_EVENT_SENSOR_SEND_SC_CP_COMP */
		struct RBLE_RSCP_Sensor_Send_SC_Control_Point_t{
			uint16_t		conhdl;
			RBLE_STATUS		status;
		}send_sc_cp;

		/* RBLE_RSCP_EVENT_SENSOR_CHG_SC_CP_IND */
		struct RBLE_RSCP_Sensor_Chg_Sc_Cp_Ind_t{
			uint16_t		conhdl;
			RBLE_RSCP_SC_CONTROL_POINT_INFO	sc_cp_info;
		}chg_sc_cp_ind;

		/* RBLE_RSCP_EVENT_SENSOR_CFG_INDNTF_IND */
		struct RBLE_RSCP_Sensor_Cfg_indntf_Ind_t{
			uint16_t		conhdl;
		    uint8_t			char_code;
		    uint8_t			reserved;
		    uint16_t		cfg_val;
		}cfg_indntf_ind;

		/* RBLE_RSCP_EVENT_SENSOR_COMMAND_DISALLOWED_IND */
		struct RBLE_RSCP_Sensor_Command_Disallowed_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_RSCPS_EVENT;

/* Running Speed and Cadence Collector */
typedef struct RBLE_RSCPC_EVENT_t {
	RBLE_RSCP_EVENT_TYPE	type;
	uint8_t					reserved;
	
	union Event_Rscc_Parameter_u {
		RBLE_STATUS		status;

		/* RBLE_RSCP_EVENT_COLLECTOR_ENABLE_COMP */
		struct RBLE_RSCP_Collector_Enable_t{
			RBLE_STATUS			status;
			uint8_t				reserved;
			uint16_t			conhdl;
			RBLE_RSCS_CONTENT	rscs;
			RBLE_DIS_CONTENT	dis;
		}collector_enable;

		/* RBLE_RSCP_EVENT_COLLECTOR_DISABLE_COMP */
		struct RBLE_RSCP_Collector_Disable_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}collector_disable;

		/* RBLE_RSCP_EVENT_COLLECTOR_ERROR_IND */
		struct RBLE_RSCP_Collector_Error_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		conhdl;
		}error_ind;

		/* RBLE_RSCP_EVENT_COLLECTOR_MEASUREMENTS_NTF */
		struct RBLE_RSCP_Collector_Measurements_Ntf_t{
			uint16_t		conhdl;
			RBLE_RSCP_MEASUREMENTS_INFO	measure_info;
		}measurements_ntf;

		/* RBLE_RSCP_EVENT_COLLECTOR_SC_CP_IND */
		struct RBLE_RSCP_Collector_SC_CP_Ind_t{
			uint16_t		conhdl;
			RBLE_RSCP_SC_CONTROL_POINT_INFO sc_cp_info;
			uint8_t			location_num;
			uint8_t			response_param[RBLE_RSCP_SENSORE_LOCATION_MAX];
		}sc_cp_ind;

		/* RBLE_RSCP_EVENT_COLLECTOR_READ_CHAR_RESPONSE */
		struct RBLE_RSCP_Collector_Read_Char_Response_t{
			uint16_t			conhdl;
			uint8_t				att_code;
			uint8_t				reserved;
			RBLE_ATT_INFO_DATA	data;
		}rd_char_resp;

		/* RBLE_RSCP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE */
		struct RBLE_RSCP_Collector_Write_Char_Response_t{
			uint16_t		conhdl;
			uint8_t			att_code;
		}wr_char_resp;

		/* RBLE_RSCP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND */
		struct RBLE_RSCP_Collector_Command_Disallowed_Ind_t{
			RBLE_STATUS		status;
			uint8_t			reserved;
			uint16_t		opcode;
		}cmd_disallowed_ind;
	} param;
} RBLE_RSCPC_EVENT;

typedef void ( *RBLE_RSCPS_EVENT_HANDLER )( RBLE_RSCPS_EVENT *event );			/* RSC(Sensor) Event CALL BACK Declaration */
typedef void ( *RBLE_RSCPC_EVENT_HANDLER )( RBLE_RSCPC_EVENT *event );			/* RSC(Collector) Event CALL BACK Declaration */

// Vendor Specific Call back define
enum RBLE_VS_EVENT_TYPE_enum {
	RBLE_VS_EVENT_TEST_RX_START_COMP = 0x01,	/* Vendor RX Test start Complete Event */
	RBLE_VS_EVENT_TEST_TX_START_COMP,			/* Vendor TX Test start Complete Event */
	RBLE_VS_EVENT_TEST_END_COMP,				/* Vendor Test end Complete Event(RX and TX commonness) */
	RBLE_VS_EVENT_WR_BD_ADDR_COMP,				/* Write Bd Address Command Complete Event */
	RBLE_VS_EVENT_SET_TEST_PARAM_COMP,			/* Set Test Parameters Command Complete Event */
	RBLE_VS_EVENT_READ_TEST_RSSI_COMP,			/* Read Test RSSI Command Complete Event */
	RBLE_VS_EVENT_GPIO_DIR_COMP,				/* GPIO Dir Command Complete Event */
	RBLE_VS_EVENT_GPIO_ACCESS_COMP,				/* GPIO Access Command Complete Event */
	RBLE_VS_EVENT_FLASH_MANAGEMENT_COMP,		/* Data Flash Management Command Complete Event */
	RBLE_VS_EVENT_FLASH_ACCESS_COMP,			/* Data Flash Access Command Complete Event */
	RBLE_VS_EVENT_FLASH_OPERATION_COMP,			/* Data Flash Operation Command Complete Event */
	RBLE_VS_EVENT_FLASH_GET_SPACE_COMP,			/* Data Flash Get Space Command Complete Event */
	RBLE_VS_EVENT_FLASH_GET_EEL_VER_COMP,		/* Data Flash Get EEL Version Command Complete Event */
	RBLE_VS_EVENT_ADAPT_ENABLE_COMP,			/* Adaptable Function Enable Command Complete Event */
	RBLE_VS_EVENT_ADAPT_STATE_IND,				/* Adaptable Function State Notification Event */
	RBLE_VS_EVENT_COMMAND_DISALLOWED_IND,		/* Vendor Command Disallowed Complete Event */
	RBLE_VS_EVENT_SET_TX_POWER_COMP,			/* Set Tx Power Complete Event */
	RBLE_VS_EVENT_SET_PARAMS_COMP,				/* Set Parameter Complete Event */
	RBLE_VS_EVENT_RF_CONTROL_COMP,				/* RF Control Command Complete Event */
};

typedef uint8_t		RBLE_VS_EVENT_TYPE;

typedef struct RBLE_VS_EVENT_t {
	RBLE_VS_EVENT_TYPE		type;			/* Event Type */
	uint8_t					reserved;
	union Event_Vs_Parameter_u {
		/* Generic Event */
		RBLE_STATUS		status;

		/* RBLE_EVT_VS_Test_End_Comp */
		struct RBLE_VS_Test_End_Comp_t{
			RBLE_STATUS         status;
            uint8_t             reserved;
			uint16_t            nb_packet_received;		/* Number of RX packets - null if TX test was the ended one */
		}test_end_cmp;

		/* RBLE_EVT_VS_Read_Test_RSSI_Comp */
		struct RBLE_VS_Read_Test_RSSI_Comp_t {
			RBLE_STATUS			status;
			uint8_t				rssi;
		} test_rssi_cmp;

		/* RBLE_EVT_VS_GPIO_DIR_COMP */
		struct RBLE_VS_GPIO_Dir_Comp_t {
			RBLE_STATUS			status;
			uint8_t				mask;
		} gpio_dir_cmp; 

		/* RBLE_EVT_VS_GPIO_ACCESS_COMP */
		struct RBLE_VS_GPIO_Access_Comp_t { 
			RBLE_STATUS			status;
			uint8_t				value;
		} gpio_access_cmp; 

		/* RBLE_EVT_VS_FLASH_MANAGEMENT_COMP */
		struct RBLE_VS_Flash_Management_Comp_t {
			RBLE_STATUS status;
			uint8_t cmd;
		}management_comp; 

		/* RBLE_EVT_VS_FLASH_ACCESS_COMP */
		struct RBLE_VS_Flash_Access_Comp_t { 
			RBLE_STATUS status;
			uint8_t cmd; 
			uint8_t id;
			uint8_t size; 
			uint8_t *addr;
		}access_comp; 

		/* RBLE_EVT_VS_FLASH_OPERATION_COMP */
		struct RBLE_VS_Flash_Operation_Comp_t { 
			RBLE_STATUS status;
			uint8_t cmd; 
		}operation_comp; 

		/* RBLE_EVT_VS_FLASH_GET_SPACE_COMP */
		struct RBLE_VS_Flash_Get_Space_Comp_t {
			RBLE_STATUS status;
			uint8_t  reserved;
			uint16_t size;
		}get_space; 

		/* RBLE_EVT_VS_FLASH_GET_EEL_VER_COMP */
		struct RBLE_VS_Flash_Get_EEL_Ver_Comp_t {
			RBLE_STATUS status;
			uint8_t version[24];
		}get_eel_ver;

		/* RBLE_EVT_VS_ADAPT_ENABLE_COMP */
		struct RBLE_VS_Adapt_Enable_Comp_t {
			RBLE_STATUS status;
			uint8_t cmd;
		}adapt_enable_cmp;

		/* RBLE_EVT_VS_ADAPT_STATE_IND */
		struct RBLE_VS_Adapt_State_Ind_t {
			uint8_t state;
		}adapt_state_ind;

		/* RBLE_EVT_VS_RF_CONTROL_COMP */
		struct RBLE_VS_RF_Control_Comp_t {
			RBLE_STATUS status;
		}rf_control_cmp; 

		/* RBLE_EVT_VS_CMD_DISALLOWED_IND */
		struct RBLE_VS_Command_Disallowed_Ind_t{
			RBLE_STATUS    status;
            uint8_t        reserved;
			uint16_t       opcode;
		}cmd_disallowed_ind;
	}param;
}RBLE_VS_EVENT;

typedef void ( *RBLE_VS_EVENT_HANDLER )( RBLE_VS_EVENT *event );			/* VS Event CALL BACK Declaration */


// Command Disallowed Function Declaration
typedef void (* RBLE_DISALLOWED_FUNC )( uint16_t Opcode, uint8_t Status );

typedef struct RBLE_CMD_TO_RWKE_CMD_t {
	uint16_t	RBLE_Cmd_Id;
	uint16_t	RWKE_Cmd_Id;
} RBLE_CMD_TO_RWKE_CMD;

/**************************** PROTOTYPE Declaration ***********************************/
RBLE_STATUS RBLE_Init(RBLE_INIT_CB call_back);	/* rBLE initialize */

/* --------------------------- Generic Access Profile --------------------------------*/
RBLE_STATUS RBLE_GAP_Reset(RBLE_GAP_EVENT_HANDLER gap_call_back, RBLE_SM_EVENT_HANDLER sm_call_back);					/* GAP Reset */
RBLE_STATUS RBLE_GAP_Set_Name(RBLE_BD_NAME *dev_name);																	/* Set Device Name */
RBLE_STATUS RBLE_GAP_Observation_Enable(uint16_t mode, RBLE_SCANNING_INFO *set_scan);									/* Observation Enable */
RBLE_STATUS RBLE_GAP_Observation_Disable(void);																			/* Observation Disable */
RBLE_STATUS RBLE_GAP_Broadcast_Enable(uint16_t disc_mode, uint16_t conn_mode, RBLE_ADV_INFO *adv_info);					/* Broadcast Enable */
RBLE_STATUS RBLE_GAP_Broadcast_Disable(void);																			/* Broadcast Disable */
RBLE_STATUS RBLE_GAP_Set_Bonding_Mode(uint16_t mode);																	/* Set Bonding Mode */
RBLE_STATUS RBLE_GAP_Set_Security_Request(uint8_t sec);																	/* Set Security Request */
RBLE_STATUS RBLE_GAP_Get_Device_Info(void);																				/* Device Infomation Get */
RBLE_STATUS RBLE_GAP_Get_White_List_Size(void);																			/* Get White List Size */
RBLE_STATUS RBLE_GAP_Add_To_White_List(RBLE_DEV_ADDR_INFO *dev_info);													/* Device Add To White List */
RBLE_STATUS RBLE_GAP_Del_From_White_List(BOOL all_dev, RBLE_DEV_ADDR_INFO *dev_info);									/* Device Info Delete From White List */
RBLE_STATUS RBLE_GAP_Get_Remote_Device_Name(RBLE_CREATE_CONNECT_PARAM *connect_param);									/* Get Remote Device Name */
RBLE_STATUS RBLE_GAP_Get_Remote_Device_Info(uint16_t conhdl);															/* Get Remote Device Info */
RBLE_STATUS RBLE_GAP_Device_Search(uint8_t discovery_type, uint8_t addr_type);											/* Remote Device Search */
RBLE_STATUS RBLE_GAP_Set_Random_Address(RBLE_BD_ADDR *bd_addr);															/* Random Address Setting */
RBLE_STATUS RBLE_GAP_Set_Privacy_Feature(uint8_t priv_flag, uint8_t set_to_ll);											/* Privacy Feature Setting */
RBLE_STATUS RBLE_GAP_Create_Connection(RBLE_CREATE_CONNECT_PARAM *connect_param);										/* Create Connection */
RBLE_STATUS RBLE_GAP_Connection_Cancel(void);																			/* Create Connection Cancel */
RBLE_STATUS RBLE_GAP_Disconnect(uint16_t conhdl);																		/* Connection Disconnect */
RBLE_STATUS RBLE_GAP_Start_Bonding(RBLE_BOND_PARAM *bond_param);														/* Bonding Start */
RBLE_STATUS RBLE_GAP_Bonding_Info_Ind(uint8_t bond_op, RBLE_BD_ADDR *addr);												/* Bonding Infomatin Indicate */
RBLE_STATUS RBLE_GAP_Bonding_Response(RBLE_BOND_RESP_PARAM *res_bond_param);											/* Resopnse to Bonding Start Request */
RBLE_STATUS RBLE_GAP_Change_Connection_Param(uint16_t conhdl, uint16_t result, RBLE_CONN_PARAM *conn_param, uint8_t role);/* Current Connection Parameter Change */
RBLE_STATUS RBLE_GAP_Channel_Map_Req(BOOL update_map, uint16_t conhdl, RBLE_LE_CHNL_MAP *chmap);						/* Channel Map Request */
RBLE_STATUS RBLE_GAP_Read_RSSI(uint16_t conhdl);																		/* Read RSSI Request */
RBLE_STATUS RBLE_GAP_Authorized_Ind(uint16_t conhdl);																	/* Authorized Information Indicate */

/* --------------------------- Security Manager -------------------------------------*/
RBLE_STATUS RBLE_SM_Set_Key(uint8_t Key_code, RBLE_KEY_VALUE *Key_Value);												/* Device Key Setting */
RBLE_STATUS RBLE_SM_Start_Enc(uint8_t idx, uint8_t auth_req, uint16_t ediv, RBLE_RAND_NB *rand_nb, RBLE_KEY_VALUE *ltk);/* Encryption Start */
RBLE_STATUS RBLE_SM_Tk_Req_Resp(uint8_t idx, uint8_t status, RBLE_KEY_VALUE *tk);										/* Response to TK Request */
RBLE_STATUS RBLE_SM_Ltk_Req_Resp(uint8_t idx, uint8_t status, uint8_t sec_prop, uint16_t ediv, RBLE_RAND_NB *nb, RBLE_KEY_VALUE *ltk);/* Response to LTK Request */
RBLE_STATUS RBLE_SM_Irk_Req_Resp(uint8_t idx, uint8_t status, RBLE_BD_ADDR *orig_addr, RBLE_KEY_VALUE *irk, uint8_t lk_sec_status);	/* Response to Irk Request */
RBLE_STATUS RBLE_SM_Csrk_Req_Resp(uint8_t idx, uint8_t status, RBLE_KEY_VALUE *csrk, uint8_t lk_sec_status);			/* Response to Csrk Request */
RBLE_STATUS RBLE_SM_Chk_Bd_Addr_Req_Resp(uint8_t idx, uint8_t type, uint8_t found_flag, uint8_t lk_sec_status, RBLE_BD_ADDR *addr);		/* Response to Check BD Address Request */

/* ---------------------------------- GATT ------------------------------------------*/
RBLE_STATUS RBLE_GATT_Enable( RBLE_GATT_EVENT_HANDLER callback );														/* GATT Enable */
RBLE_STATUS RBLE_GATT_Discovery_Service_Request( RBLE_GATT_DISC_SVC_REQ *disc_svc );									/* GATT Service discovery request */
RBLE_STATUS RBLE_GATT_Discovery_Char_Request( RBLE_GATT_DISC_CHAR_REQ *disc_char );										/* GATT Characteristic discovery request */
RBLE_STATUS RBLE_GATT_Discovery_Char_Descriptor_Request( RBLE_GATT_DISC_CHAR_DESC_REQ *disc_char_desc );				/* GATT Characteristic descriptor discovery request */
RBLE_STATUS RBLE_GATT_Read_Char_Request( RBLE_GATT_READ_CHAR_REQ *rd_char );											/* GATT Read characteristic request */
RBLE_STATUS RBLE_GATT_Write_Char_Request( RBLE_GATT_WRITE_CHAR_REQ *wr_char );											/* GATT Write characteristic request */
RBLE_STATUS RBLE_GATT_Write_Reliable_Request( RBLE_GATT_WRITE_RELIABLE_REQ *rel_write );								/* GATT Write reliable characteristic request */
RBLE_STATUS RBLE_GATT_Execute_Write_Char_Request( RBLE_GATT_EXE_WR_CHAR_REQ *exe_wr_char );								/* GATT Execute write characteristic request */
RBLE_STATUS RBLE_GATT_Notify_Request( RBLE_GATT_NOTIFY_REQ *notify );													/* GATT Notification request */
RBLE_STATUS RBLE_GATT_Indicate_Request( RBLE_GATT_INDICATE_REQ *indicate );												/* GATT Indication request */
RBLE_STATUS RBLE_GATT_Write_Response( RBLE_GATT_WRITE_RESP *wr_resp );													/* GATT Write Response */
RBLE_STATUS RBLE_GATT_Set_Permission( RBLE_GATT_SET_PERM *set_perm );													/* GATT Set Permission */
RBLE_STATUS RBLE_GATT_Set_Data( RBLE_GATT_SET_DATA *set_data );															/* GATT Set Data */

/* --------------------------- Find Me Profile ---------------------------------------*/
RBLE_STATUS RBLE_FMP_Target_Enable(uint16_t conhdl, uint8_t sec_lvl, RBLE_FMPT_EVENT_HANDLER call_back);				/* Find Me Profile Target Role Enable */
RBLE_STATUS RBLE_FMP_Target_Disable(uint16_t conhdl);																	/* Find Me Profile Target Role Disable */
RBLE_STATUS RBLE_FMP_Locator_Enable(uint16_t conhdl, uint8_t con_type, RBLE_FMP_IAS_CONTENT *ias, RBLE_FMPL_EVENT_HANDLER call_back);	/* Find Me Profile Locator Role Enable */
RBLE_STATUS RBLE_FMP_Locator_Disable(uint16_t conhdl);																	/* Find Me Profile Locator Role Disable */
RBLE_STATUS RBLE_FMP_Locator_Set_Alert(uint16_t conhdl, uint8_t alert_lvl);												/* Alert Setting */

/* ------------------------- Proximity Profile---------------------------------------*/
RBLE_STATUS RBLE_PXP_Reporter_Enable(uint16_t conhdl,uint8_t alert_lvl,uint8_t sec_lvl, RBLE_PXPR_EVENT_HANDLER call_back);	/* Proximity Profile Reporter Role Enable */
RBLE_STATUS RBLE_PXP_Reporter_Disable(uint16_t conhdl);																	/* Proximity Profile Reporter Role disable */
RBLE_STATUS RBLE_PXP_Monitor_Enable(uint16_t conhdl, uint8_t con_type, RBLE_PROXI_MON_PARAM *param, RBLE_PXPM_EVENT_HANDLER call_back);	/* Proximity Profile Monitor Role Enable */
RBLE_STATUS RBLE_PXP_Monitor_Disable(uint16_t conhdl);																	/* Proximity Profile Monitor Role disable */
RBLE_STATUS RBLE_PXP_Monitor_Get_Alert_Level(uint16_t conhdl);															/* Get Alert Level */
RBLE_STATUS RBLE_PXP_Monitor_Set_Alert_Level(uint16_t conhdl,uint8_t svc_code,uint8_t lvl);								/* Set Alert Level */
RBLE_STATUS RBLE_PXP_Monitor_Get_Tx_Power(uint16_t conhdl);																/* Get Tx Power */

/* ------------------------- Health Thermometer Profile-------------------------------*/
RBLE_STATUS RBLE_HTP_Thermometer_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_HTP_THERM_PARAM *param, RBLE_HTPT_EVENT_HANDLER call_back);/* Health Thermometer Profile Thermometer Role Enable */
RBLE_STATUS RBLE_HTP_Thermometer_Disable(uint16_t conhdl);																/* Health Thermometer Profile Thermometer Role disable */
RBLE_STATUS RBLE_HTP_Thermometer_Send_Temp(uint16_t conhdl, RBLE_HTP_TEMP_INFO *temp_info);								/* Health Thermometer Send Temperature Infomation */
RBLE_STATUS RBLE_HTP_Thermometer_Req_Measurement_Period_Ind(uint16_t conhdl);											/* Request Measurement Period Indicate */
RBLE_STATUS RBLE_HTP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_HTS_CONTENT *hts, RBLE_DIS_CONTENT *dis, RBLE_HTPC_EVENT_HANDLER call_back);	/* Health Thermometer Collector Role Enable */
RBLE_STATUS RBLE_HTP_Collector_Disable(uint16_t conhdl);																/* Health Thermometer Collector Role disable */
RBLE_STATUS RBLE_HTP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code);											/* Read Characteristic Infomation */
RBLE_STATUS RBLE_HTP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);						/* Write Characteristic Infomation */
RBLE_STATUS RBLE_HTP_Collector_Set_Measurement_Period(uint16_t conhdl, uint16_t intv);									/* Set Measurement Period */

/* ------------------------- Blood Pressure Profile-------------------------------*/
RBLE_STATUS RBLE_BLP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_BLP_SENSOR_PARAM *param, RBLE_BLPS_EVENT_HANDLER call_back);/* Blood Pressure Profile Sensor Role Enable */
RBLE_STATUS RBLE_BLP_Sensor_Disable(uint16_t conhdl);																	/* Blood Pressure Profile Sensor Role disable */
RBLE_STATUS RBLE_BLP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_BLP_MEASUREMENTS_INFO *measurements_info);			/* Blood Pressure Send Measurements Infomation */
RBLE_STATUS RBLE_BLP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_BLS_CONTENT *bls, RBLE_DIS_CONTENT *dis, RBLE_BLPC_EVENT_HANDLER call_back);	/* Blood Pressure Collector Role Enable */
RBLE_STATUS RBLE_BLP_Collector_Disable(uint16_t conhdl);																/* Blood Pressure Collector Role disable */
RBLE_STATUS RBLE_BLP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code);											/* Read Characteristic Infomation */
RBLE_STATUS RBLE_BLP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);						/* Write Characteristic Infomation */

/* ------------------------- HID over GATT Profile-------------------------------*/
RBLE_STATUS RBLE_HGP_HDevice_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_HGP_DEVICE_PARAM *param, RBLE_HGHD_EVENT_HANDLER call_back);      /* Enables the HID Device */
RBLE_STATUS RBLE_HGP_HDevice_Disable(uint16_t conhdl);																	/* Disables the HID Device */
RBLE_STATUS RBLE_HGP_HDevice_Send_Report(uint16_t conhdl, uint8_t inst_idx, RBLE_HGP_REPORT_DESC *report);				/* Sends the Report */
RBLE_STATUS RBLE_HGP_HDevice_Send_Battery_Level(uint16_t conhdl, uint8_t inst_idx, uint8_t battery_level);				/* Sends the Battery Level */
RBLE_STATUS RBLE_HGP_BHost_Enable(uint16_t conhdl, uint8_t con_type, uint8_t hids_inst_num, uint8_t bas_inst_num, RBLE_HIDS_CONTENT *hids, RBLE_DIS11_CONTENT *dis, RBLE_BAS_CONTENT *bas, RBLE_HGBH_EVENT_HANDLER call_back);  /* Enables the Boot Host */
RBLE_STATUS RBLE_HGP_BHost_Disable(uint16_t conhdl);																    /* Disables the Boot Host */
RBLE_STATUS RBLE_HGP_BHost_Read_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code);								/* Reads the characteristic value */
RBLE_STATUS RBLE_HGP_BHost_Read_By_UUID_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code);						/* Reads the characteristic value specified by UUID */
RBLE_STATUS RBLE_HGP_BHost_Write_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code, uint16_t cfg_val);			/* Writes the characteristic value */
RBLE_STATUS RBLE_HGP_BHost_Set_Report(uint16_t conhdl, uint8_t inst_idx, RBLE_HGP_REPORT_DESC *report);			        /* Sets the Report value */
RBLE_STATUS RBLE_HGP_BHost_Write_Protocol_Mode(uint16_t conhdl, uint8_t inst_idx, uint8_t protocol_mode_val);			/* Sends the Protocol Mode */
RBLE_STATUS RBLE_HGP_BHost_Data_Output(uint16_t conhdl, uint8_t inst_idx, RBLE_HGP_REPORT_DESC *report);			    /* Sends the Report value */
RBLE_STATUS RBLE_HGP_RHost_Enable(uint16_t conhdl, uint8_t con_type, uint8_t hids_inst_num, uint8_t bas_inst_num, RBLE_HIDS_CONTENT *hids, RBLE_DIS11_CONTENT *dis, RBLE_BAS_CONTENT *bas, RBLE_HGRH_EVENT_HANDLER call_back);  /* Enables the Report Host */
RBLE_STATUS RBLE_HGP_RHost_Disable(uint16_t conhdl);																    /* Disables the Report Host */
RBLE_STATUS RBLE_HGP_RHost_Read_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code);								/* Reads the characteristic value */
RBLE_STATUS RBLE_HGP_RHost_Read_By_UUID_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code);						/* Reads the characteristic value specified by UUID */
RBLE_STATUS RBLE_HGP_RHost_Read_Long_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code);						/* Reads the long characteristic value */
RBLE_STATUS RBLE_HGP_RHost_Write_Char(uint16_t conhdl, uint8_t inst_idx, uint8_t char_code, uint16_t cfg_val);			/* Writes the characteristic value */
RBLE_STATUS RBLE_HGP_RHost_Set_Report(uint16_t conhdl, uint8_t inst_idx, RBLE_HGP_REPORT_DESC *report);			        /* Sets the Report value */
RBLE_STATUS RBLE_HGP_RHost_Write_Protocol_Mode(uint16_t conhdl, uint8_t inst_idx, uint8_t protocol_mode_val);			/* Sends the Protocol Mode */
RBLE_STATUS RBLE_HGP_RHost_Data_Output(uint16_t conhdl, uint8_t inst_idx, RBLE_HGP_REPORT_DESC *report);			    /* Sends the Report value */
RBLE_STATUS RBLE_HGP_RHost_Write_Control_Point(uint16_t conhdl, uint8_t inst_idx, uint8_t control_point_val);			/* Sends the Control Point */

/* ------------------------- Scan Parameters Profile-------------------------------*/
RBLE_STATUS RBLE_SPP_Server_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_SPP_SERVER_PARAM *param, RBLE_SPPS_EVENT_HANDLER call_back);      /* Enables Scan Server */
RBLE_STATUS RBLE_SPP_Server_Disable(uint16_t conhdl);																	/* Disables Scan Server */
RBLE_STATUS RBLE_SPP_Server_Send_Refresh(uint16_t conhdl, uint8_t s_refresh_val);				                        /* Sends scan refresh request. */
RBLE_STATUS RBLE_SPP_Client_Enable(uint16_t conhdl, uint8_t con_type, RBLE_SPS_CONTENT *sps, RBLE_SCANS_INTV_WINDOW_PARAM *s_intv_window, RBLE_SPPC_EVENT_HANDLER call_back);  /* Enables Scan Client */
RBLE_STATUS RBLE_SPP_Client_Disable(uint16_t conhdl);																    /* Disables Scan Client */
RBLE_STATUS RBLE_SPP_Client_Write_Char(uint16_t conhdl, uint16_t cfg_val);			                                    /* Writes characteristic */
RBLE_STATUS RBLE_SPP_Client_Write_Interval(uint16_t conhdl, RBLE_SCANS_INTV_WINDOW_PARAM *s_intv_window);			    /* Sends interval window value */

/* ------------------------- Heart Rate Profile-------------------------------*/
RBLE_STATUS RBLE_HRP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_HRP_SENSOR_PARAM *param, RBLE_HRPS_EVENT_HANDLER call_back);	/* Heart Rate Profile Sensor Role Enable */
RBLE_STATUS RBLE_HRP_Sensor_Disable(uint16_t conhdl);																	/* Heart Rate Profile Sensor Role disable */
RBLE_STATUS RBLE_HRP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_HRP_MEASUREMENTS_INFO *measurements_info);			/* Heart Rate Send Measurements Infomation */
RBLE_STATUS RBLE_HRP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_HRS_CONTENT *hrs, RBLE_DIS_CONTENT *dis, RBLE_HRPC_EVENT_HANDLER call_back);	/* Heart Rate Collector Role Enable */
RBLE_STATUS RBLE_HRP_Collector_Disable(uint16_t conhdl);																/* Heart Rate Collector Role disable */
RBLE_STATUS RBLE_HRP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code);											/* Read Characteristic Infomation */
RBLE_STATUS RBLE_HRP_Collector_Write_Control_Point(uint16_t conhdl, uint8_t cp_val);									/* Write Control Point */
RBLE_STATUS RBLE_HRP_Collector_Write_Char(uint16_t conhdl, uint16_t cfg_val);											/* Write Characteristic Infomation */

/* ------------------------- Cycling Speed and Cadence Profile-------------------------------*/
RBLE_STATUS RBLE_CSCP_Sensor_Enable( uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_CSCP_SENSOR_PARAM *param, RBLE_CSCPS_EVENT_HANDLER call_back);	/* Cycling Speed and Cadence Profile Sensor Role Enable */
RBLE_STATUS RBLE_CSCP_Sensor_Disable(uint16_t conhdl);																			/* Cycling Speed and Cadence Profile Sensor Role disable */
RBLE_STATUS RBLE_CSCP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_CSCP_MEASUREMENTS_INFO *measurements_info);				/* Cycling Speed and Cadence Send Measurements Infomation */
RBLE_STATUS RBLE_CSCP_Sensor_Send_SC_Control_Point(uint16_t conhdl, RBLE_CSCP_SC_CONTROL_POINT_INFO *sc_cp_info);				/* Cycling Speed and Cadence Send SC Control Point Infomation */
RBLE_STATUS RBLE_CSCP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_CSCS_CONTENT *cscs, RBLE_DIS_CONTENT *dis, RBLE_CSCPC_EVENT_HANDLER call_back);	/* Cycling Speed and Cadence Collector Role Enable */
RBLE_STATUS RBLE_CSCP_Collector_Disable(uint16_t conhdl);																		/* Cycling Speed and Cadence Collector Role disable */
RBLE_STATUS RBLE_CSCP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code);													/* Read Characteristic Infomation */
RBLE_STATUS RBLE_CSCP_Collector_Write_SC_Control_Point(uint16_t conhdl, RBLE_CSCP_SC_CONTROL_POINT_INFO *sc_cp_info);			/* Write SC Control Point */
RBLE_STATUS RBLE_CSCP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);								/* Write Characteristic Infomation */

/* ------------------------- Glucose Profile-------------------------------*/
RBLE_STATUS RBLE_GLP_Sensor_Enable( uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_GLP_SENSOR_PARAM *param, RBLE_GLPS_EVENT_HANDLER call_back);	/* Glucose Profile Sensor Role Enable */
RBLE_STATUS RBLE_GLP_Sensor_Disable(uint16_t conhdl);																			/* Glucose Profile Sensor Role disable */
RBLE_STATUS RBLE_GLP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_GLP_MEASUREMENTS_INFO *measurements_info);					/* Glucose Send Measurements Infomation */
RBLE_STATUS RBLE_GLP_Sensor_Send_Measurements_Context(uint16_t conhdl, RBLE_GLP_MEASUREMENTS_CONTEXT_INFO *measurements_context_info);	/* Glucose Send Measurements Context Infomation */
RBLE_STATUS RBLE_GLP_Sensor_Send_RA_Control_Point(uint16_t conhdl, RBLE_GLP_RA_CONTROL_POINT_IND_INFO *ra_cp_ind_info);					/* Glucose Send RA Control Point Infomation */
RBLE_STATUS RBLE_GLP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_GLS_CONTENT *gls, RBLE_DIS_CONTENT *dis, RBLE_GLPC_EVENT_HANDLER call_back);	/* Glucose Collector Role Enable */
RBLE_STATUS RBLE_GLP_Collector_Disable(uint16_t conhdl);																		/* Glucose Collector Role disable */
RBLE_STATUS RBLE_GLP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code);													/* Read Characteristic Infomation */
RBLE_STATUS RBLE_GLP_Collector_Write_RA_Control_Point(uint16_t conhdl, RBLE_GLP_RA_CONTROL_POINT_INFO *ra_cp_info);				/* Write RA Control Point */
RBLE_STATUS RBLE_GLP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);								/* Write Characteristic Infomation */

/* ------------------------- Cycling Power Profile-------------------------------*/
RBLE_STATUS RBLE_CPP_Sensor_Enable( uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_CPP_SENSOR_PARAM *param, RBLE_CPPS_EVENT_HANDLER call_back);	/* Enables the Cycling Power Sensor */
RBLE_STATUS RBLE_CPP_Sensor_Disable(uint16_t conhdl);																	/* Disables the Cycling Power Sensor */
RBLE_STATUS RBLE_CPP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_CPP_MEASUREMENTS_INFO *measurements_info);			/* Sends the Measurement */
RBLE_STATUS RBLE_CPP_Sensor_Broadcast_Measurements(uint16_t conhdl, RBLE_CPP_MEASUREMENTS_INFO *measurements_info);		/* Broadcast the Measurement */
RBLE_STATUS RBLE_CPP_Sensor_Send_Vector(uint16_t conhdl, RBLE_CPP_VECTOR_INFO *vector_info);							/* Sends the Vector */
RBLE_STATUS RBLE_CPP_Sensor_Send_CP_Control_Point(uint16_t conhdl, RBLE_CPP_IND_CONTROL_POINT_INFO *ind_cp_info);		/* Write the Control Point */
RBLE_STATUS RBLE_CPP_Sensor_Send_Battery_Level(uint16_t conhdl, uint8_t battery_level);									/* Sends the Battery Level */
RBLE_STATUS RBLE_CPP_Sensor_Send_Write_Response(uint16_t conhdl, uint8_t res_code);										/* Sends the Write Response */
RBLE_STATUS RBLE_CPP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_CPS_CONTENT *cps, RBLE_DIS_CONTENT *dis, RBLE_BATS_CONTENT *bas, RBLE_CPPC_EVENT_HANDLER call_back);	/* Enables the Clinet */
RBLE_STATUS RBLE_CPP_Collector_Disable(uint16_t conhdl);																/* Disables the Clinet */
RBLE_STATUS RBLE_CPP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code);											/* Reads the characteristic value */
RBLE_STATUS RBLE_CPP_Collector_Write_CP_Control_Point(uint16_t conhdl, RBLE_CPP_WR_CONTROL_POINT_INFO *wr_cp_info);		/* Writes the control Point value */
RBLE_STATUS RBLE_CPP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);						/* Writes the characteristic value */

/* ------------------------- Time Profile-------------------------------*/
RBLE_STATUS RBLE_TIP_Server_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, uint16_t current_time_ntf_en, RBLE_TIPS_EVENT_HANDLER call_back);/* Time Profile Server Role Enable */
RBLE_STATUS RBLE_TIP_Server_Disable(uint16_t conhdl);													/* Time Profile Server Role disable */
RBLE_STATUS RBLE_TIP_Server_Send_Current_Time(uint16_t conhdl, RBLE_TIP_CURRENT_TIME *current_time);	/* Time Send Alert Status */
RBLE_STATUS RBLE_TIP_Server_Write_Data(uint8_t char_code, RBLE_TIPS_WR_DATA *wr_data);					/* Time Send Alert Status */
RBLE_STATUS RBLE_TIP_Client_Enable(uint16_t conhdl, uint8_t con_type, RBLE_CTS_CONTENT *cts, RBLE_NDCS_CONTENT *ndcs, RBLE_RTUS_CONTENT *rtus, RBLE_TIPC_EVENT_HANDLER call_back);	/* Time Profile Client Role Enable */
RBLE_STATUS RBLE_TIP_Client_Disable(uint16_t conhdl);													/* Time Profile Client Role disable */
RBLE_STATUS RBLE_TIP_Client_Read_Char(uint16_t conhdl, uint8_t char_code);								/* Read Characteristic Infomation */
RBLE_STATUS RBLE_TIP_Client_Write_Char(uint16_t conhdl, uint16_t cfg_val);								/* Write Characteristic Infomation */
RBLE_STATUS RBLE_TIP_Client_Write_Time_Update_CP(uint16_t conhdl, uint8_t time_update_cp);				/* Write Time Update Control Point value */

/* ------------------------- Alert Notification Profile-------------------------------*/
RBLE_STATUS RBLE_ANP_Server_Enable( uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_ANP_SERVER_PARAM *param, RBLE_ANPS_EVENT_HANDLER call_back);/* Alert Notification Profile Server Role Enable */
RBLE_STATUS RBLE_ANP_Server_Disable(uint16_t conhdl);															/* Alert Notification Profile Server Role disable */
RBLE_STATUS RBLE_ANP_Server_Send_New_Alert(uint16_t conhdl, RBLE_ANP_NEW_ALERT_INFO *alert_info);				/* Send New Alert */
RBLE_STATUS RBLE_ANP_Server_Send_Unread_Alert(uint16_t conhdl, RBLE_ANP_UNREAD_ALERT_INFO *alert_status);		/* Send Unread Alert Status */
RBLE_STATUS RBLE_ANP_Client_Enable(uint16_t conhdl, uint8_t con_type, RBLE_ANS_CONTENT *ans, RBLE_ANPC_EVENT_HANDLER call_back);	/* Alert Notification Profile Client Role Enable */
RBLE_STATUS RBLE_ANP_Client_Disable(uint16_t conhdl);															/* Alert Notification Profile Client Role disable */
RBLE_STATUS RBLE_ANP_Client_Read_Char(uint16_t conhdl, uint8_t char_code);										/* Read Characteristic Infomation */
RBLE_STATUS RBLE_ANP_Client_Write_Alert_Notification_CP(uint16_t conhdl, RBLE_ANP_ALERT_NTF_CP *alert_ntf_cp);	/* Write Characteristic Infomation */
RBLE_STATUS RBLE_ANP_Client_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);					/* Write Alert Notification Control Point value */

/* ------------------------- Location and Navigation Profile-------------------------------*/
RBLE_STATUS RBLE_LNP_Sensor_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_LNP_SENSOR_PARAM *param, RBLE_LNPS_EVENT_HANDLER call_back);	/* Enables the Location and Navigation Sensor */
RBLE_STATUS RBLE_LNP_Sensor_Disable(uint16_t conhdl);																	/* Disables the Location and Navigation Sensor */
RBLE_STATUS RBLE_LNP_Sensor_Send_Location_Speed(uint16_t conhdl, RBLE_LNP_LOCATION_SPEED_INFO *location_speed_info);	/* Sends the location and speed */
RBLE_STATUS RBLE_LNP_Sensor_Set_Position_Quality(RBLE_LNP_POSITION_QUALITY_INFO *quality_info);							/* Sets the position quality */
RBLE_STATUS RBLE_LNP_Sensor_Send_LN_Control_Point(uint16_t conhdl, RBLE_LNP_IND_CONTROL_POINT_INFO *ind_cp_info);		/* Sends the LN control point */
RBLE_STATUS RBLE_LNP_Sensor_Send_Navigation(uint16_t conhdl, RBLE_LNP_NAVIGATION_INFO *navigation_info);				/* Sends the navigation */
RBLE_STATUS RBLE_LNP_Sensor_Send_Battery_Level(uint16_t conhdl, uint8_t battery_level);									/* Sends the battery level */
RBLE_STATUS RBLE_LNP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_LNS_CONTENT *lns, RBLE_DIS_CONTENT *dis, RBLE_BATS_CONTENT *bas, RBLE_LNPC_EVENT_HANDLER call_back);	/* Enables the Collector */
RBLE_STATUS RBLE_LNP_Collector_Disable(uint16_t conhdl);																/* Disables the Clinet */
RBLE_STATUS RBLE_LNP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code);											/* Reads the characteristic value */
RBLE_STATUS RBLE_LNP_Collector_Write_LN_Control_Point(uint16_t conhdl, RBLE_LNP_WR_CONTROL_POINT_INFO *wr_cp_info);		/* Writes the control Point value */
RBLE_STATUS RBLE_LNP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);						/* Writes the characteristic value */

/* ------------------------- Phone Alert Status Profile-------------------------------*/
RBLE_STATUS RBLE_PASP_Server_Enable(uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_PASP_SERVER_PARAM *param, RBLE_PASPS_EVENT_HANDLER call_back);/* Alert Phone Status Profile Server Role Enable */
RBLE_STATUS RBLE_PASP_Server_Disable(uint16_t conhdl);												/* Alert Phone Status Profile Server Role disable */
RBLE_STATUS RBLE_PASP_Server_Send_Alert_Status(uint16_t conhdl, uint8_t alert_status);				/* Alert Phone Status Send Alert Status */
RBLE_STATUS RBLE_PASP_Server_Send_Ringer_Setting(uint16_t conhdl, uint8_t ringer_setting);			/* Alert Phone Status Send Ringer Setting */
RBLE_STATUS RBLE_PASP_Client_Enable(uint16_t conhdl, uint8_t con_type, RBLE_PASS_CONTENT *pass, RBLE_PASPC_EVENT_HANDLER call_back);	/* Alert Phone Status Profile Client Role Enable */
RBLE_STATUS RBLE_PASP_Client_Disable(uint16_t conhdl);												/* Alert Phone Status Profile Client Role disable */
RBLE_STATUS RBLE_PASP_Client_Read_Char(uint16_t conhdl, uint8_t char_code);							/* Read Characteristic Infomation */
RBLE_STATUS RBLE_PASP_Client_Write_Ringer_Control_Point(uint16_t conhdl, uint8_t cp_val);			/* Write Ringer Control Point value */
RBLE_STATUS RBLE_PASP_Client_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);		/* Write Characteristic Infomation */

/* ------------------------- Running Speed and Cadence Profile-------------------------------*/
RBLE_STATUS RBLE_RSCP_Sensor_Enable( uint16_t conhdl, uint8_t sec_lvl, uint8_t con_type, RBLE_RSCP_SENSOR_PARAM *param, RBLE_RSCPS_EVENT_HANDLER call_back);	/* Enables the Running Speed and Cadence Sensor */
RBLE_STATUS RBLE_RSCP_Sensor_Disable(uint16_t conhdl);																	/* Disables the Running Speed and Cadence Sensor */
RBLE_STATUS RBLE_RSCP_Sensor_Send_Measurements(uint16_t conhdl, RBLE_RSCP_MEASUREMENTS_INFO *measurements_info);		/* Sends the Measurement */
RBLE_STATUS RBLE_RSCP_Sensor_Send_SC_Control_Point(uint16_t conhdl, RBLE_RSCP_SC_CONTROL_POINT_INFO *sc_cp_info);		/* Sends the Control Point */
RBLE_STATUS RBLE_RSCP_Collector_Enable(uint16_t conhdl, uint8_t con_type, RBLE_RSCS_CONTENT *rscs, RBLE_DIS_CONTENT *dis, RBLE_RSCPC_EVENT_HANDLER call_back);	/* Enables the Collector */
RBLE_STATUS RBLE_RSCP_Collector_Disable(uint16_t conhdl);																/* Disables the Clinet */
RBLE_STATUS RBLE_RSCP_Collector_Read_Char(uint16_t conhdl, uint8_t char_code);											/* Reads the characteristic value */
RBLE_STATUS RBLE_RSCP_Collector_Write_SC_Control_Point(uint16_t conhdl, RBLE_RSCP_SC_CONTROL_POINT_INFO *wr_cp_info);	/* Writes the control Point value */
RBLE_STATUS RBLE_RSCP_Collector_Write_Char(uint16_t conhdl, uint8_t char_code, uint16_t cfg_val);						/* Writes the characteristic value */

/* ------------------------- Vendor Specific-------------------------------*/
RBLE_STATUS RBLE_VS_Enable( RBLE_VS_EVENT_HANDLER callback );															/* Vendor Specific Command Enable */
RBLE_STATUS RBLE_VS_Test_Rx_Start(uint8_t rx_freq);																		/* Test Rx Start */
RBLE_STATUS RBLE_VS_Test_Tx_Start(uint8_t tx_freq, uint8_t test_data_len, uint8_t pk_payload_type);						/* Test Tx Start */
RBLE_STATUS RBLE_VS_Test_End(void);																						/* Test End */
RBLE_STATUS RBLE_VS_Set_Test_Parameter( uint16_t rx_nb_packet, uint16_t tx_nb_packet, uint8_t infinite_setting );		/* Set Parrameters Command for Direct Test */
RBLE_STATUS RBLE_VS_Read_Test_RSSI( void );																				/* Read RSSI while Direct Test */
RBLE_STATUS RBLE_VS_Write_Bd_Address( RBLE_BD_ADDR *address );															/* Write BD Adsress Command */
RBLE_STATUS RBLE_VS_Set_Tx_Power( uint16_t conhdl, uint8_t power_lvl, uint8_t state );									/* Set Tx Power Command */
RBLE_STATUS RBLE_VS_GPIO_Dir( uint8_t dir );																			/* GPIO Dir Command */
RBLE_STATUS RBLE_VS_GPIO_Access( uint8_t mode, uint8_t value );															/* GPIO Access Command */
RBLE_STATUS RBLE_VS_Flash_Management( uint8_t cmd );																	/* Data Flash Management Command */
RBLE_STATUS RBLE_VS_Flash_Access( RBLE_VS_FLASH_ACCESS_PARAM *param );													/* Data Flash Access Command */
RBLE_STATUS RBLE_VS_Flash_Operation( uint8_t cmd );																		/* Data Flash Operation Command */
RBLE_STATUS RBLE_VS_Flash_Get_Space( void );																			/* Data Flash Get Space Command */
RBLE_STATUS RBLE_VS_Flash_Get_EEL_Ver( void );																			/* Data Flash Get EEL Version Command */
RBLE_STATUS RBLE_VS_Adapt_Enable( uint8_t cmd );																		/* Adaptable Function Enable Command */
RBLE_STATUS RBLE_VS_RF_Control( uint8_t cmd );																			/* RF Control Command */
RBLE_STATUS RBLE_VS_Set_Params( uint8_t param_id, uint8_t param_len, uint8_t *param_data );								/* Set Parameters */
#endif // __RBLE_API_INC__
