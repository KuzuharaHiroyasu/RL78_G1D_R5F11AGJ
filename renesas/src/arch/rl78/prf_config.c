/**
 ****************************************************************************************
 *
 * @file		prf_config.c
 *
 * @brief	Setting of Configuration Parameters for Profile
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: 9932 $
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

/**
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"
#include "rwke_api.h"

#include "prf_config.h"
#include "db_handle.h"
#include "rble_api.h"

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */
 
 /* profile task ID */
#if PRF_SEL_PXPM
const ke_task_id_t proxm_task_id = TASK_PROXM;
#endif
#if PRF_SEL_PXPR
const ke_task_id_t proxr_task_id = TASK_PROXR;
#endif
#if PRF_SEL_FMPL
const ke_task_id_t findl_task_id = TASK_FINDL;
#endif
#if PRF_SEL_FMPT
const ke_task_id_t findt_task_id = TASK_FINDT;
#endif
#if PRF_SEL_HTPC
const ke_task_id_t htpc_task_id = TASK_HTPC;
#endif
#if PRF_SEL_HTPT
const ke_task_id_t htpt_task_id = TASK_HTPT;
#endif
#if PRF_SEL_BLPC
const ke_task_id_t blpc_task_id = TASK_BLPC;
#endif
#if PRF_SEL_BLPS
const ke_task_id_t blps_task_id = TASK_BLPS;
#endif
#if PRF_SEL_HGHD
const ke_task_id_t hghd_task_id = TASK_HGHD;
#endif
#if PRF_SEL_HGBH
const ke_task_id_t hgbh_task_id = TASK_HGBH;
#endif
#if PRF_SEL_HGRH
const ke_task_id_t hgrh_task_id = TASK_HGRH;
#endif
#if PRF_SEL_SPPS
const ke_task_id_t scans_task_id = TASK_SCANS;
#endif
#if PRF_SEL_SPPC
const ke_task_id_t scanc_task_id = TASK_SCANC;
#endif
#if PRF_SEL_HRPC
const ke_task_id_t hrpc_task_id = TASK_HRPC;
#endif
#if PRF_SEL_HRPS
const ke_task_id_t hrps_task_id = TASK_HRPS;
#endif
#if PRF_SEL_CSCC
const ke_task_id_t cscpc_task_id = TASK_CSCPC;
#endif
#if PRF_SEL_CSCS
const ke_task_id_t cscps_task_id = TASK_CSCPS;
#endif
#if PRF_SEL_GLPC
const ke_task_id_t glpc_task_id = TASK_GLPC;
#endif
#if PRF_SEL_GLPS
const ke_task_id_t glps_task_id = TASK_GLPS;
#endif
#if PRF_SEL_CPPC
const ke_task_id_t cppc_task_id = TASK_CPPC;
#endif
#if PRF_SEL_CPPS
const ke_task_id_t cpps_task_id = TASK_CPPS;
#endif
#if PRF_SEL_TIPC
const ke_task_id_t tipc_task_id = TASK_TIPC;
#endif
#if PRF_SEL_TIPS
const ke_task_id_t tips_task_id = TASK_TIPS;
#endif
#if PRF_SEL_ANPC
const ke_task_id_t anpc_task_id = TASK_ANPC;
#endif
#if PRF_SEL_ANPS
const ke_task_id_t anps_task_id = TASK_ANPS;
#endif
#if PRF_SEL_LNPC
const ke_task_id_t lnpc_task_id = TASK_LNPC;
#endif
#if PRF_SEL_LNPS
const ke_task_id_t lnps_task_id = TASK_LNPS;
#endif
#if PRF_SEL_PASC
const ke_task_id_t paspc_task_id = TASK_PASPC;
#endif
#if PRF_SEL_PASS
const ke_task_id_t pasps_task_id = TASK_PASPS;
#endif
#if PRF_SEL_RSCC
const ke_task_id_t rscpc_task_id = TASK_RSCPC;
#endif
#if PRF_SEL_RSCS
const ke_task_id_t rscps_task_id = TASK_RSCPS;
#endif

#if (USE_LINK_LOSS_SERVICE)
/*********************************
 * Link Loss service             *
 *********************************/
/** Link Loss Service */
static const uint16_t lls_svc = RBLE_SVC_LINK_LOSS;

/** Link Loss Service alert level characteristic */
static const struct atts_char_desc lls_alert_lvl_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD|RBLE_GATT_CHAR_PROP_WR,
                                                                  LLS_HDL_ALERT_LVL_VAL,
                                                                  RBLE_CHAR_ALERT_LEVEL);
/** LLS alert level value */
uint8_t svc_lls_alert_lvl = 0;

#endif	/* #if (USE_LINK_LOSS_SERVICE) */

#if (USE_TX_POWER_SERVICE)
/*********************************
 * TX Power service              *
 *********************************/
/** TX Power Service */
static const uint16_t tps_svc = RBLE_SVC_TX_POWER;

/** TX Power Service TX Power level characteristic (can be notifiable if BR/EDR) */
static const struct atts_char_desc tps_txpw_lvl_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                                 TPS_HDL_TXPW_LVL_VAL,
                                                                 RBLE_CHAR_TX_POWER_LEVEL);
/** TPS Tx Power level - signed */
int8_t svc_txpw_lvl = 0;

#endif	/* #if (USE_LINK_LOSS_SERVICE) */

#if (USE_IMMEDIATE_ALERT_SERVICE)
/*********************************
 * Immediate Alert service       *
 *********************************/
/** Immediate Alert Service */
static const uint16_t ias_svc = RBLE_SVC_IMMEDIATE_ALERT;

/** Immediate Alert Service alert level characteristic */
static const struct atts_char_desc ias_alert_lvl_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                           IAS_HDL_ALERT_LVL_VAL,
                                                           RBLE_CHAR_ALERT_LEVEL);
/** IAS alert level value */
uint8_t svc_ias_alert_lvl = 0;

#endif	/* #if (USE_IMMEDIATE_ALERT_SERVICE) */

#if (PRF_SEL_HTPT)
/*********************************
 * Health Thermometer service    *
 *********************************/
/** Health Thermometer Service */
static const uint16_t htpt_hlth_therm_svc = RBLE_SVC_HEALTH_THERMOMETER;

/** Health Thermometer Service Temperature Measurement Characteristic */
static const struct atts_char_desc htpt_temp_meas_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_IND,
                                                            HTS_HDL_TEMP_MEAS_VAL,
                                                            RBLE_CHAR_TEMPERATURE_MEAS);
/** HTS Temperature measurement / Intermediate Temperature value */
uint8_t hts_packed_temp[13];

/** Health Thermometer Service Temperature Type Characteristic */
static const struct atts_char_desc htpt_temp_type_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                            HTS_HDL_TEMP_TYPE_VAL,
                                                            RBLE_CHAR_TEMPERATURE_TYPE);

/** Health Thermometer Service Intermediate Temperature Characteristic */
static const struct atts_char_desc interm_temp_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                              HTS_HDL_INTERM_TEMP_VAL,
                                                              RBLE_CHAR_INTERMEDIATE_TEMP);

/** Health Thermometer Service Measurement Interval Characteristic */
static const struct atts_char_desc htpt_meas_intv_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD|RBLE_GATT_CHAR_PROP_WR|RBLE_GATT_CHAR_PROP_IND,
                                                              HTS_HDL_MEAS_INTV_VAL,
                                                              RBLE_CHAR_MEAS_INTERVAL);
/** HTS Temperature measurement indication configuration */
uint16_t hts_temp_meas_ind_en;
/** HTS Intermediate temperature notification configuration value */
uint16_t hts_interm_temp_ntf_en;
/** HTS measurement interval indication configuration value */
uint16_t hts_meas_intv_ind_en;

/**
 * Temperature Type Characteristic Value - indicates where the temperature was measured.
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.temperature_type.xml
 */
uint8_t hts_temp_type = 0x02;   // Body (general)

/**
 * Measurement Interval Characteristic Value - time between measurements
 * renage: 1 to 65535(seconds)
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.characteristic.measurement_interval.xml
 */
uint16_t hts_meas_intv = 1; // 1second

/**
 * Valid Range Descriptor Value - valid range of values for the Measurement Interval Characteristic value
 * https://www.bluetooth.com/specifications/gatt/viewer?attributeXmlFile=org.bluetooth.descriptor.valid_range.xml
 */
uint16_t hts_meas_intv_range[2] = {
    0x0001,     // Minimum Measurement Interval  1 second
    0x000A      // Maximum Measurement Interval 10 seconds
};

#endif	/* #if (PRF_SEL_HTPT) */

#if (PRF_SEL_BLPS)
/*********************************
 * Blood Pressure service        *
 *********************************/
/** Blood Pressure Service */
const uint16_t blps_bldprs_svc = RBLE_SVC_BLOOD_PRESSURE;

/** Blood Pressure Service Blood Pressure Measurement Characteristic */
const struct atts_char_desc blps_bldprs_meas_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_IND,
                                                            BLS_HDL_BLDPRS_MEAS_VAL,
                                                            RBLE_CHAR_BLOOD_PRESSURE_MEAS);
/** BLPS Blood Pressure Measurement / Intermediate Cuff Pressure Value */
uint8_t blps_packed_measurements[19];

/** Blood Pressure Service Intermediate Cuff Pressure Characteristic */
const struct atts_char_desc blps_interm_cufprs_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                              BLS_HDL_INTERM_CUFPRS_VAL,
                                                              RBLE_CHAR_INTERMEDIATE_BLOOD_PRESS);

/** Blood Pressure Service Blood Pressure Feature Characteristic */
const struct atts_char_desc blps_bldprs_feat_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                            BLS_HDL_BLDPRS_FEAT_VAL,
                                                            RBLE_CHAR_BLOOD_PRESSURE_FEAT);
/** BLPS Blood Pressure Feature */
static const uint16_t blps_bldprs_feat = 0x20;	/* Multiple Bond Support bit */

/** BLPS Blood Pressure measurement indication configuration value */
uint16_t blps_bldprs_meas_ind_en;
/** BLPS intermediate cuff pressure notification configuration value */
uint16_t blps_interm_cufprs_ntf_en;

#endif	/* #if (PRF_SEL_BLPS) */


#if (USE_DEV_INFO_SERVICE || USE_DEV_INFO_SERVICE_PNP)
/*********************************
 * Device Information service    *
 *********************************/
/** Device Information Service */
static const uint16_t dis_svc = RBLE_SVC_DEVICE_INFORMATION;

#if (USE_DEV_INFO_SERVICE)
/** System ID Characteristic */
static const struct atts_char_desc dis_sys_id_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                         DIS_HDL_SYS_ID_VAL,
                                                         RBLE_CHAR_SYSTEM_ID);
/** System ID value (8octets) */
static const uint8_t dis_sys_id_desc[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };

/** Model Number Characteristic */
static const struct atts_char_desc dis_model_nb_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                          DIS_HDL_MODEL_NB_VAL,
                                                          RBLE_CHAR_MODEL_NUMBER_STRING);
/** Model Number String */
static const uint8_t dis_model_nb_desc[]	= "rBLE-Sample";

/** Serial Number characteristic */
static const struct atts_char_desc dis_serial_nb_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                           DIS_HDL_SERIAL_NB_VAL,
                                                           RBLE_CHAR_SERIAL_NUMBER_STRING);
/** Serial Number String */
static const uint8_t dis_serial_nb_desc[] = "1.0.0.0-LE";

/** Firmware Revision Characteristic */
static const struct atts_char_desc dis_fw_rev_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                         DIS_HDL_FW_REV_VAL,
                                                         RBLE_CHAR_FW_REVISION_STRING);
/** Firmware Revision String */
static const uint8_t dis_fw_rev_desc[] = "1.0.0";

/** Hardware Revision Characteristic */
static const struct atts_char_desc dis_hw_rev_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                         DIS_HDL_HW_REV_VAL,
                                                         RBLE_CHAR_HW_REVISION_STRING);
/** Hardware Revision String **/
static const uint8_t dis_hw_rev_desc[] = "1.0.0";

/** Software Revision Characteristic */
static const struct atts_char_desc dis_sw_rev_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                         DIS_HDL_SW_REV_VAL,
                                                         RBLE_CHAR_SW_REVISION_STRING);
/** Software Revision String */
static const uint8_t dis_sw_rev_desc[] = "1.0.0";

/** Manufacturer Name Characteristic **/
static const struct atts_char_desc dis_manuf_name_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                       DIS_HDL_MANUF_NAME_VAL,
                                                       RBLE_CHAR_MANUF_NAME_STRING);
/** Manufacturer Name String */
static const uint8_t dis_manuf_name_desc[] = "Renesas Electronics";

/** IEEE 11073-20601 Regulatory Certification Data List */
static const struct atts_char_desc dis_ieee_certif_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                              DIS_HDL_IEEE_CERTIF_VAL,
                                                              RBLE_CHAR_IEEE_CERTIF);
/** IEEE 11073-20601 Regulatory Certification Data List Value (Min:6octets) */
static const uint8_t dis_ieee_certif_desc[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

#endif /* #if (USE_DEV_INFO_SERVICE) */

#if (USE_DEV_INFO_SERVICE_PNP)

#define PNP_ID(vendor_id_src, vendor_id, product_id, product_ver)	{ (uint8_t)((vendor_id_src)),	\
										(uint8_t)((vendor_id)),		(uint8_t)((vendor_id) >> 8),	\
										(uint8_t)((product_id)),	(uint8_t)((product_id) >> 8),	\
										(uint8_t)((product_ver)),	(uint8_t)((product_ver) >> 8)}

#define VENDOR_ID_SRC	RBLE_SVC_SIG_ASSIGNED_ID
#define VENDOR_ID		0x0000u
#define PRODUCT_ID		0x0000u
#define PRODUCT_VER		0x0000u

/** PnP ID characteristic */
static const struct atts_char_desc dis_pnp_id_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                        DIS_HDL_PNP_ID_VAL,
                                                        RBLE_CHAR_PNP_ID);

/** PnP ID Value */
static const uint8_t dis_pnp_id[] = PNP_ID(VENDOR_ID_SRC, VENDOR_ID, PRODUCT_ID, PRODUCT_VER);

#endif	/* #if (!USE_DEV_INFO_SERVICE_PNP) */
#endif /* #if (USE_DEV_INFO_SERVICE || USE_DEV_INFO_SERVICE_PNP) */

#if (PRF_SEL_HGHD)
/*********************************
 * HID Service(Instance 1)       *
 *********************************/
/** HID Service(Instance 1) */
static const uint16_t hghd_hid_svc1 = RBLE_SVC_HUMAN_INTERFACE_DEVICE;
/** Include Service(Instance 1) */
static const struct att_incl_desc hghd_hid_incl_svc1 = ATTS_INCL(BAS1_HDL_SVC,
																 BAS1_HDL_BATT_REP_REF,
																 RBLE_SVC_BATTERY_SERVICE);
/** Protocol Mode characteristic(Instance 1) */
static const struct atts_char_desc hghd_protocol_md_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                               HIDS1_HDL_PROTOCOL_MD_VAL,
                                                               RBLE_CHAR_PROTOCOL_MODE);
/** Report(Input) characteristic(Instance 1) */
static const struct atts_char_desc hghd_report_input_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_NTF,
                                                                HIDS1_HDL_REPORT_INPUT_VAL,
                                                                RBLE_CHAR_REPORT);
/** Report(Output) characteristic(Instance 1) */
static const struct atts_char_desc hghd_report_output_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                                 HIDS1_HDL_REPORT_OUTPUT_VAL,
                                                                 RBLE_CHAR_REPORT);
/** Report(Feature) characteristic(Instance 1) */
static const struct atts_char_desc hghd_report_feature_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR,
                                                                  HIDS1_HDL_REPORT_FEATURE_VAL,
                                                                  RBLE_CHAR_REPORT);
/** Report Map characteristic(Instance 1) */
static const struct atts_char_desc hghd_report_map_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                              HIDS1_HDL_REPORT_MAP_VAL,
                                                              RBLE_CHAR_REPORT_MAP);
#if (HIDS_INST1 == HID_BOOT_KB)
/** Boot Keyboard Input Report characteristic(Instance 1) */
static const struct atts_char_desc hghd_bootkb_input_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_NTF,
                                                                HIDS1_HDL_BOOTKB_INPUT_VAL,
                                                                RBLE_CHAR_BOOT_KB_INPUT_REPORT);
/** Boot Keyboard Output Report characteristic(Instance 1) */
static const struct atts_char_desc hghd_bootkb_output_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                                 HIDS1_HDL_BOOTKB_OUTPUT_VAL,
                                                                 RBLE_CHAR_BOOT_KB_OUTPUT_REPORT);
#elif (HIDS_INST1 == HID_BOOT_MO)
/** Boot Mouse Input Report characteristic(Instance 1) */
static const struct atts_char_desc hghd_bootmo_input_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_NTF,
                                                                HIDS1_HDL_BOOTMO_INPUT_VAL,
                                                                RBLE_CHAR_BOOT_MOUSE_INPUT_REPORT);
#endif

/** HID Information characteristic(Instance 1) */
static const struct atts_char_desc hghd_hid_info_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                            HIDS1_HDL_HID_INFO_VAL,
                                                            RBLE_CHAR_HID_INFO);
/** HID Control Point characteristic(Instance 1) */
static const struct atts_char_desc hghd_hid_cp_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                          HIDS1_HDL_HID_CP_VAL,
                                                          RBLE_CHAR_HID_CTRL_POINT);

#ifdef HID_MULTI_INSTANCE
/** HID Service(Instance 2) */
static const uint16_t hghd_hid_svc2 = RBLE_SVC_HUMAN_INTERFACE_DEVICE;
/** Include Service(Instance 2) */
static const struct att_incl_desc hghd_hid_incl_svc2 = ATTS_INCL(BAS2_HDL_SVC,
																 BAS2_HDL_BATT_REP_REF,
																 RBLE_SVC_BATTERY_SERVICE);
/** Protocol Mode characteristic(Instance 2) */
static const struct atts_char_desc hghd_protocol_md_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                               HIDS2_HDL_PROTOCOL_MD_VAL,
                                                               RBLE_CHAR_PROTOCOL_MODE);
/** Report(Input) characteristic(Instance 2) */
static const struct atts_char_desc hghd_report_input_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_NTF,
                                                                HIDS2_HDL_REPORT_INPUT_VAL,
                                                                RBLE_CHAR_REPORT);
/** Report(Output) characteristic(Instance 2) */
static const struct atts_char_desc hghd_report_output_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                                 HIDS2_HDL_REPORT_OUTPUT_VAL,
                                                                 RBLE_CHAR_REPORT);
/** Report(Feature) characteristic(Instance 2) */
static const struct atts_char_desc hghd_report_feature_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR,
                                                                  HIDS2_HDL_REPORT_FEATURE_VAL,
                                                                  RBLE_CHAR_REPORT);
/** Report Map characteristic(Instance 2) */
static const struct atts_char_desc hghd_report_map_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                              HIDS2_HDL_REPORT_MAP_VAL,
                                                              RBLE_CHAR_REPORT_MAP);
#if (HIDS_INST2 == HID_BOOT_KB)
/** Boot Keyboard Input Report characteristic(Instance 2) */
static const struct atts_char_desc hghd_bootkb_input_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_NTF,
                                                                HIDS2_HDL_BOOTKB_INPUT_VAL,
                                                                RBLE_CHAR_BOOT_KB_INPUT_REPORT);
/** Boot Keyboard Output Report characteristic(Instance 2) */
static const struct atts_char_desc hghd_bootkb_output_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                                 HIDS2_HDL_BOOTKB_OUTPUT_VAL,
                                                                 RBLE_CHAR_BOOT_KB_OUTPUT_REPORT);
#elif (HIDS_INST2 == HID_BOOT_MO)
/** Boot Mouse Input Report characteristic(Instance 2) */
static const struct atts_char_desc hghd_bootmo_input_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR | RBLE_GATT_CHAR_PROP_NTF,
                                                                HIDS2_HDL_BOOTMO_INPUT_VAL,
                                                                RBLE_CHAR_BOOT_MOUSE_INPUT_REPORT);
#endif

/** HID Information characteristic(Instance 2) */
static const struct atts_char_desc hghd_hid_info_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                            HIDS2_HDL_HID_INFO_VAL,
                                                            RBLE_CHAR_HID_INFO);
/** HID Control Point characteristic(Instance 2) */
static const struct atts_char_desc hghd_hid_cp_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                          HIDS2_HDL_HID_CP_VAL,
                                                          RBLE_CHAR_HID_CTRL_POINT);
#endif /* #ifdef HID_MULTI_INSTANCE */


struct hid_service hids_inst[RBLE_HIDS_INST_MAX] = {
	/* Instance 1 */
	{
		RBLE_HGHD_PROTOCOL_MODE_REPORT,	/* Protocol Mode */
		0,								/* Characteristic value HID Control Point */
		{ 0 },							/* Input Report characteristic value */
		0x0000,							/* Input Report notification configuration value */
		{ 1, RBLE_HGHD_INPUT_REPORT },	/* Report Reference descriptor values of characteristic (Input) Report */
		{ 0 },							/* Output Report characteristic value */
		{ 1, RBLE_HGHD_OUTPUT_REPORT },	/* Report Reference descriptor values of property (Output) Report */
		{ 0 },							/* Feature Report characteristic value */
		{ 1, RBLE_HGHD_FEATURE_REPORT },/* Report Reference descriptor values of property (Feature) Report */
		{ 0 },							/* Boot Keyboard Input Report Value */
		0x0000,							/* Boot Keyboard Input Report notification configuration value */
		{ 0 },							/* Boot Keyboard Output Report Value */
		{ 0 },							/* Boot Mouse Input Report Value */
		0x0000,							/* Boot Mouse Input Report notification configuration value */
	},
	/* Instance 2 */
	{
		RBLE_HGHD_PROTOCOL_MODE_REPORT,	/* Protocol Mode */
		0,								/* Characteristic value HID Control Point */
		{ 0 },							/* Input Report characteristic value */
		0x0000,							/* Input Report notification configuration value */
		{ 1, RBLE_HGHD_INPUT_REPORT },	/* Report Reference descriptor values of characteristic (Input) Report */
		{ 0 },							/* Output Report characteristic value */
		{ 1, RBLE_HGHD_OUTPUT_REPORT },	/* Report Reference descriptor values of property (Output) Report */
		{ 0 },							/* Feature Report characteristic value */
		{ 1, RBLE_HGHD_FEATURE_REPORT },/* Report Reference descriptor values of property (Feature) Report */
		{ 0 },							/* Boot Keyboard Input Report Value */
		0x0000,							/* Boot Keyboard Input Report notification configuration value */
		{ 0 },							/* Boot Keyboard Output Report Value */
		{ 0 },							/* Boot Mouse Input Report Value */
		0x0000,							/* Boot Mouse Input Report notification configuration value */
	}
};
/** Report Map characteristic(Instance 1) Value */
static const uint8_t hids1_report_map[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/** Report Map characteristic(Instance 1) External Report Reference Value */
static const uint8_t hids1_ext_report_ref[] = { 
	(uint8_t)RBLE_SVC_BATTERY_SERVICE,
	(uint8_t)(RBLE_SVC_BATTERY_SERVICE >> 8u)
};

/** Characteristic value HID Information (Instance 1) Value */
static const uint8_t hids1_hid_info[] = {
	0x00, 0x00,		/* bcdHID */
	0x00,			/* bCountryCode */
	0x00			/* Flags (Bit0: RemoteWake, Bit1: NormallyConnectable) */
};


#ifdef HID_MULTI_INSTANCE
/** Report Map characteristic(Instance 2) Value */
static const uint8_t hids2_report_map[] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };

/** Report Map characteristic(Instance 2) External Report Reference Value */
static const uint8_t hids2_ext_report_ref[] = {
	(uint8_t)RBLE_SVC_BATTERY_SERVICE,
	(uint8_t)(RBLE_SVC_BATTERY_SERVICE >> 8u)
};

/** Characteristic value HID Information (Instance 2) Value */
static const uint8_t hids2_hid_info[] = {
	0x00, 0x00,		/* bcdHID */
	0x00,			/* bCountryCode */
	0x00			/* Flags (Bit0: RemoteWake, Bit1: NormallyConnectable) */
};

#endif	/* #ifdef HID_MULTI_INSTANCE */
#endif	/* #if (PRF_SEL_HGHD) */

#if (USE_BATTERY_SERVICE)
/*********************************
 * Battery Service(Instance 1)   *
 *********************************/
/** Battery Service(Instance 1) **/
static const uint16_t bas_svc1 = RBLE_SVC_BATTERY_SERVICE;
/** Battery Level characteristic(Instance 1) **/
static const struct atts_char_desc bas_battery_lvl_char1 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_NTF,
                                                              BAS1_HDL_BATTERY_LVL_VAL,
                                                              RBLE_CHAR_BATTERY_LEVEL);

/** Battery Level characteristic(Instance 1) Presentation Format */
static const uint8_t bas1_char_presen[] = {
	0x00,			/* Format */
	0x00,			/* Exponent */
	0x00, 0x00,		/* Unit(UUID) */
	0x01,			/* Namespace */
	0x00, 0x00		/* Description */
};

#ifdef HID_MULTI_INSTANCE
/*********************************
 * Battery Service(Instance 2)   *
 *********************************/
/** Battery Service(Instance 2) **/
static const uint16_t bas_svc2 = RBLE_SVC_BATTERY_SERVICE;
/** Battery Level characteristic(Instance 2) **/
static const struct atts_char_desc bas_battery_lvl_char2 = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_NTF,
                                                              BAS2_HDL_BATTERY_LVL_VAL,
                                                              RBLE_CHAR_BATTERY_LEVEL);

/** Battery Level characteristic(Instance 2) Presentation Format */
static const uint8_t bas2_char_presen[] = {
	0x00,			/* Format */
	0x00,			/* Exponent */
	0x00, 0x00,		/* Unit(UUID) */
	0x01,			/* Namespace */
	0x00, 0x00		/* Description */
};

#endif	/* #ifdef HID_MULTI_INSTANCE */

struct batt_service bas_inst[RBLE_BAS_INST_MAX] = {
	/* Instance 1 */
	{
		0x0000u,						/* Battery Level permission settings notification of the characteristic values */
		{ 0, RBLE_HGHD_INPUT_REPORT },	/* Report Reference descriptor values of property Battery Level */
		0								/* Characteristic value Battery Level */
	},
	/* Instance 2 */
	{
		0x0000u,						/* Battery Level permission settings notification of the characteristic values */
		{ 0, RBLE_HGHD_INPUT_REPORT },	/* Report Reference descriptor values of property Battery Level */
		0								/* Characteristic value Battery Level */
	}
};

#endif	/* #if (USE_BATTERY_SERVICE) */

#if (USE_SCAN_PARAMETER_SERVICE)
/*********************************
 * Scan Parameters Service       *
 *********************************/
/** Scan Parameters Service */
static const uint16_t scanparam_svc = RBLE_SVC_SCAN_PARAMETERS;

/** Scan Parameters Service Scan Interval Window characteristic */
static const struct atts_char_desc scans_intv_window_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                               SCANS_HDL_INTV_WINDOW_VAL,
                                                               RBLE_CHAR_SCAN_INTERVAL_WINDOW);
/** Scan Parameters Service Scan Refresh characteristic */
static const struct atts_char_desc scans_refresh_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                           SCANS_HDL_REFRESH_VAL,
                                                           RBLE_CHAR_SCAN_REFRESH);
/** Scan interval window characteristic variable */
struct svc_scans_intv_window scans_intv_window = { 0x0000u, 0x0000u };
/** Scan refresh characteristic value */
uint8_t scans_refresh_val = 0;
/** Scan refresh notification configuration */
uint16_t scans_refresh_ntf_en = 0x0000u;
#endif	/* #if (USE_SCAN_PARAMETER_SERVICE) */

#if (PRF_SEL_HRPS)
/*********************************
 * Heart Rate Service            *
 *********************************/

/** Heart Rate Service */
const uint16_t hrps_heart_rate_svc = RBLE_SVC_HEART_RATE;

/** Heart Rate Service Heart Raete Measurement Characteristic */
const struct atts_char_desc hrps_hrtrate_meas_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                            HRS_HDL_HRTRATE_MEAS_VAL,
                                                            RBLE_CHAR_HEART_RATE_MEAS);
/** HRPS Heart Raete Measurement Value */
uint8_t hrps_packed_measurements[HRPS_PACK_MAX];

/** Heart Rate Service Body Sensor Location Characteristic */
const struct atts_char_desc hrps_body_sensor_location_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                              HRS_HDL_HRTRATE_BSL_VAL,
                                                              RBLE_CHAR_BODY_SENSOR_LOCATION);

/** Heart Rate Service Heart Rate Control Point Characteristic */
const struct atts_char_desc hrps_hrtrate_cp_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_WR,
                                                            HRS_HDL_HRTRATE_CP_VAL,
                                                            RBLE_CHAR_HEART_RATE_CTRL_POINT);
/** HRPS Body Sensor Location Value */
static const uint8_t hrps_body_sensor_location = 0x01;	/* 0:other 1:Chest 2:Wrist 3:Finger 4:Hand 5:EarLobe 6:Foot 7-:Reserved */

/** HRPS Heart Rate Control Point Value */
uint8_t hrps_hrtrate_control_point = 0x00u;

/** HRPS Heart Rate measurement notification configuration value */
uint16_t hrps_hrtrate_meas_ntf_en = 0x0000u;
#endif /* #if (PRF_SEL_HRPS) */

#if (PRF_SEL_CSCS)
/*************************************
 * Cycling Speed and Cadence Service *
 *************************************/
/* csc original error code */
#define CSCP_ALREADY_PROGRESS_ERR_CODE     0x80
#define CSCP_CFG_DES_IMPROPERLY_ERR_CODE   0x81

/** Cycling Speed Service */
const uint16_t cscps_cycling_speed_svc = RBLE_SVC_CYCLING_SPEED;

/** Cycling Speed Service Cycling Speed Measurement Characteristic */
const struct atts_char_desc cscps_csc_meas_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                            CSCS_HDL_CSC_MEAS_VAL,
                                                            RBLE_CHAR_CSC_MEAS);
/** CSCPS Cycling Speed Measurement Value */
uint8_t cscps_packed_measurements[CSCPS_MEAS_MAX];

/** Cycling Speed Service CSC Feature Characteristic */
const struct atts_char_desc cscps_csc_feature_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                              CSCS_HDL_CSC_FEATURE_VAL,
                                                              RBLE_CHAR_CSC_FEATURE);

/** Cycling Speed Service Sensor Location Characteristic */
const struct atts_char_desc cscps_sensor_location_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                            CSCS_HDL_SENSOR_LOC_VAL,
                                                            RBLE_CHAR_SENSOR_LOCATION);

/** Cycling Speed Service SC Control Point Characteristic */
const struct atts_char_desc cscps_sc_cp_char = ATTS_CHAR((RBLE_GATT_CHAR_PROP_IND | RBLE_GATT_CHAR_PROP_WR),
                                                            CSCS_HDL_SC_CP_VAL,
                                                            RBLE_CHAR_SC_CNTL_POINT);

/** CSCPS SC Control Point Value */
uint8_t cscps_packed_sc_cp[CSCPS_SC_CP_MAX];

/** CSCPS Sensor Location Value */
/* 0:other 1:top of shoe 2:in shoe 3:hip 4:front wheel 5: left crank 6:right crank 7:left pedal 8:right pedal
   9:front hub 10:rear dropout 11:chainstay 12:rear wheel 13:rear hub 14:chest 15-:reserved */
uint8_t cscps_sensor_location = 0x04;

/** Number of support sensor location */
const uint8_t	cscps_support_location_num	= 15;

/** Support Sensor Location */
const uint8_t	cscps_support_sensor_location[CSCPS_LOCATION_MAX] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0xff,0xff};

/** CSCPS CSC Feature Value */
static const uint16_t cscps_csc_feature = 0x07;		/* bit0:wheel revolution bit1:crank revolution bit2:multi sensor location */

/** CSCPS Cycling Speed measurement notification configuration value */
uint16_t cscps_cycspd_meas_ntf_en = 0x0000u;

/** CSCPS SC Control Point indication configuration value */
uint16_t cscps_sc_cp_ind_en = 0x0000u;
#endif /* #if (PRF_SEL_CSCS) */

/*Glucose on development begin*/
#if (PRF_SEL_GLPS)
/*************************************
 *       Glucose Service             *
 *************************************/
/* glucose original error code */
#define GLP_ALREADY_PROGRESS_ERR_CODE     0x80
#define GLP_CFG_DES_IMPROPERLY_ERR_CODE   0x81

/** Glucose Service */
const uint16_t glps_glucose_svc = RBLE_SVC_GLUCOSE;

/** Glucose Service Glucose Measurement Characteristic */
const struct atts_char_desc glps_meas_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                            GLS_HDL_GL_MEAS_VAL,
                                                            RBLE_CHAR_GLUCOSE_MEASUREMENT);

/** Glucose Service Glucose Measurement Context Characteristic */
const struct atts_char_desc glps_meas_context_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                            GLS_HDL_GL_MEAS_CONTEXT_VAL,
                                                            RBLE_CHAR_GLUCOSE_MEAS_CONTEXT);

/** GLPS Glucose Measurement Value */
uint8_t glps_packed_measurements[GLPS_MEAS_MAX];

/** GLPS Glucose Measurement Context Value */
uint8_t glps_packed_measurements_context[GLPS_MEAS_CONTEXT_MAX];

/** Glucose Service Feature Characteristic */
const struct atts_char_desc glps_feature_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                              GLS_HDL_GL_FEATURE_VAL,
                                                              RBLE_CHAR_GLUCOSE_FEATURE);

/** Glucose Service RA Control Point Characteristic */
const struct atts_char_desc glps_ra_cp_char = ATTS_CHAR((RBLE_GATT_CHAR_PROP_IND | RBLE_GATT_CHAR_PROP_WR),
                                                            GLS_HDL_RA_CP_VAL,
                                                            RBLE_CHAR_RECORD_ACCESS_CTRL_POINT);

/** GLPS RA Control Point Value */
uint8_t glps_packed_ra_cp[GLPS_RA_CP_MAX];

/** GLPS Glucose Feature Value */
static const uint16_t glps_feature = 0x07FF;

/** GLPS Glucose measurement notification configuration value */
uint16_t glps_meas_ntf_en = 0x0000u;

/** GLPS Glucose measurement context notification configuration value */
uint16_t glps_meas_context_ntf_en = 0x0000u;

/** GLPS RA Control Point indication configuration value */
uint16_t glps_ra_cp_ind_en = 0x0000u;
#endif /* #if (PRF_SEL_GLPS) */
/*Glucose on development end*/

#if (PRF_SEL_CPPS)
/*********************************
 * Cycling Power service        *
 *********************************/
/** Cycling Power Service */
const uint16_t cpps_cycpwr_svc = RBLE_SVC_CYCLING_POWER;

/** Cycling Power Service Cycling Power Measurement Characteristic */
const struct atts_char_desc cpps_cycpwr_meas_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF | RBLE_GATT_CHAR_PROP_BCAST,
                                                              CPS_HDL_MEAS_VAL,
                                                              RBLE_CHAR_CYCLING_POWER_MEAS);

/** Cycling Power Service Cycling Power Feature Characteristic */
const struct atts_char_desc cpps_cycpwr_feature_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                                 CPS_HDL_FEATURE_VAL,
                                                                 RBLE_CHAR_CYCLING_POWER_FEATURE);

/** Cycling Power Service Cycling Power Sensor Location Characteristic */
const struct atts_char_desc cpps_sensor_location_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                                  CPS_HDL_SENSOR_LOCATION_VAL,
                                                                  RBLE_CHAR_SENSOR_LOCATION);

/** Cycling Power Service Cycling Power Vector Characteristic */
const struct atts_char_desc cpps_cycpwr_vector_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                                CPS_HDL_VECTOR_VAL,
                                                                RBLE_CHAR_CYCLING_POWER_VECTOR);

/** Cycling Power Service Cycling Power Control Point Characteristic */
const struct atts_char_desc cpps_cycpwr_cp_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_IND | RBLE_GATT_CHAR_PROP_WR,
                                                            CPS_HDL_CP_VAL,
                                                            RBLE_CHAR_CYCLING_POWER_CNTL_POINT);

/** CPPS Cycling Power Measurement Value */
uint8_t cpps_packed_measurements[CPPS_MEAS_MAX];
/** CPPS Cycling Power Vector Value */
uint8_t cpps_packed_vector[CPPS_VECTOR_MAX];
/** CPPS Cycling Power Control Point Value */
uint8_t cpps_packed_cp[CPPS_CP_CP_MAX];

/** CPPS Cycling Power Feature */
static const uint32_t cpps_cycpwr_feat = 0x0006ffff;		/* Sensor Measurement Context:0(bit16), other bits is all 1 */

/** CPPS Cycling Power Sensor Location */
uint8_t cpps_sensor_location = 0x04;	/* 0:other 1:top of shoe 2:in shoe 3:hip 4:front wheel 5: left crank 6:right crank 7:left pedal */
										/* 8:right pedal 9:front hub 10:rear dropout 11:chainstay 12:rear wheel 13:rear hub 14:chest 15-:reserved */

/* Sensor Location:Front Wheel(0x04) */
uint8_t cpps_supported_location[CPPS_LOCATION_MAX] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0xff,0xff};

/* Supported Sensor Location Num */
uint8_t cpps_supported_location_num = 15;

/** CPPS Cycling Power measurement notification configuration value */
uint16_t cpps_cycpwr_meas_ntf_en = 0x0000u;
/** CPPS Cycling Power measurement broadcast configuration value */
uint16_t cpps_cycpwr_meas_brd_en = 0x0000u;
/** CPPS Cycling Power vector notification configuration value */
uint16_t cpps_cycpwr_vector_ntf_en = 0x0000u;
/** CPPS Cycling Power control power indication configuration value */
uint16_t cpps_cycpwr_cp_ind_en = 0x0000u;

#endif	/* #if (PRF_SEL_CPPS) */

#if (PRF_SEL_TIPS)
/*********************************
 * Curent Time service            *
 *********************************/
/** Current Time Service */
const uint16_t cts_current_time_svc = RBLE_SVC_CURRENT_TIME;

/** Current Time Service Current Time Characteristic */
const struct atts_char_desc cts_current_time_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF | RBLE_GATT_CHAR_PROP_RD,
                                                              CTS_HDL_CURRENT_TIME_VAL,
                                                              RBLE_CHAR_CURRENT_TIME);

/** Current Time Service Local Time Information Characteristic */
const struct atts_char_desc cts_local_time_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                            CTS_HDL_LOCAL_TIME_VAL,
                                                            RBLE_CHAR_LOCAL_TIME_INFO);
/** Current Time Service Reference Time Information Characteristic */
const struct atts_char_desc cts_ref_time_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                          CTS_HDL_REF_TIME_VAL,
                                                          RBLE_CHAR_REF_TIME_INFO);
/* CTS current time value */
uint8_t cts_current_time[10] =
{
	0xdd, 0x07,	/* year(2013) */
	0x05,		/* month(5) */
	0x18,		/* day(24) */
	0x0a,		/* hour(10) */
	0x1b,		/* min(27) */
	0x23,		/* sec(35) */
	0x01,		/* day of week(monday) */
	0x00,		/* fractions256 */
	0x00		/* adjust reason */
};

/* CTS local time information value */
uint8_t cts_local_time[2] = {0, 0};		/* time zone UTC+0:00 ,DST offset standard time */

/* CTS reference time information value */
uint8_t cts_ref_time[4] =
{
	4,		/* source(manual) */
	0,		/* accuracy */
	0,		/* day since updata */
	0		/* hours since update */
};

/** CTS current time notification configuration value */
uint16_t cts_current_time_ntf_en = 0x0000u;

/*********************************
 * Next DST Change service        *
 *********************************/
/** Next DST Change Service */
const uint16_t ndcs_next_dst_chg_svc = RBLE_SVC_NEXT_DST_CHANGE;

/**  Next DST Change Service Time With DST Characteristic */
const struct atts_char_desc ndcs_time_dst_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                           NDCS_HDL_TIME_DST_VAL,
                                                           RBLE_CHAR_TIME_WITH_DST);
/* NDCS time with dst value */
uint8_t ndcs_time_dst[8] =
{
	0xdd, 0x07,	/* year(2013) */
	0x05,		/* month(5) */
	0x18,		/* day(24) */
	0x12,		/* hour(18) */
	0x31,		/* min(49) */
	0x15,		/* sec(21) */
	0x04		/* dst offset(daylight time) */
};

/*********************************
 * Reference Time Update service  *
 *********************************/
/** Reference Time Update Service */
const uint16_t rtus_ref_time_update_svc = RBLE_SVC_REFERENCE_TIME_UPDATE;

/** Reference Time Update Service Time Update Control Point Characteristic */
const struct atts_char_desc rtus_time_update_cp_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                                 RTUS_HDL_TIME_UPDATE_CP_VAL,
                                                                 RBLE_CHAR_TIME_UPDATE_CTRL_POINT);
uint8_t rtus_time_update_cp = 0;

/** Reference Time Update Service Time Update State Characteristic */
const struct atts_char_desc rtus_time_update_state_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                                    RTUS_HDL_TIME_UPDATE_STATE_VAL,
                                                                    RBLE_CHAR_TIME_UPDATE_STATE);
uint8_t rtus_time_update_state[2] = {0, 0};

#endif

#if (PRF_SEL_ANPS)
/*********************************
 * Alert Notification service     *
 *********************************/

/** Alert Notification Service */
const uint16_t ans_alert_ntf_svc = RBLE_SVC_ALERT_NOTIFICATION;

/** Alert Notification Service Supported New Alert Category Characteristic */
const struct atts_char_desc ans_supp_new_alert_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                                ANPS_HDL_SUPP_NEW_AL_CATEGORY_VAL,
                                                                RBLE_CHAR_SUPP_NEW_AL_CATEGORY);

/** Alert Notification Service New Alert Category Characteristic */
const struct atts_char_desc ans_new_alert_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                          ANPS_HDL_NEW_ALERT_VAL,
                                                            RBLE_CHAR_NEW_ALERT);

/** Alert Notification Service Supported Unread Alert Status Characteristic */
const struct atts_char_desc ans_supp_unread_alert_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                                   ANPS_HDL_SUPP_UNREAD_CATEGORY_VAL,
                                                                   RBLE_CHAR_SUPP_UNREAD_AL_CATEGORY);

/** Alert Notification Service Unread Alert Status Characteristic */
const struct atts_char_desc ans_unread_alert_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                              ANPS_HDL_UNREAD_ALERT_STATUS_VAL,
                                                              RBLE_CHAR_UNREAD_ALERT_STATUS);

/** Alert Notification Service Alert Notification Control Point Characteristic */
const struct atts_char_desc ans_alert_ntf_cp_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_WR,
                                                              ANPS_HDL_AL_NTF_CTRL_POINT_VAL,
                                                              RBLE_CHAR_AL_NOTIF_CTRL_POINT);
/* ANS supported new alert category value */
uint8_t ans_new_alert[ANPS_NEW_ALERT_MAX] = {0};

/* ANS supported new alert category value */
uint8_t ans_unread_alert[2] = {0x00, 0x00};

/* ANS supported new alert category value */
uint8_t ans_alert_ntf_cp[2] = {0x00, 0x00};

/* ANS supported new alert category value */
static const uint8_t ans_supp_new_alert[2] = {0xff, 0x03};		/* all bits is 1 */

/* ANS supported unread alert category value */
static const uint8_t ans_supp_unread_alert[2] = {0xff, 0x03};	/* all bits is 1 */

/** ANS new alert notification configuration value */
uint16_t ans_new_alert_ntf_en = 0x0000u;

/** ANS unread alert status notification configuration value */
uint16_t ans_unread_alert_ntf_en = 0x0000u;
#endif

#if (PRF_SEL_LNPS)
/***************************************
 * Location and Navigation service     *
 **************************************/

/** Location and Navigation Service */
const uint16_t lns_location_navi_svc = RBLE_SVC_LOCATION_NAVIGATION;

/** Location and Navigation Service LN Feature Characteristic */
const struct atts_char_desc lns_ln_feature_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                            LNS_HDL_LN_FEATURE_VAL,
                                                            RBLE_CHAR_LN_FEATURE);

/** Location and Navigation Service Location and Speed Characteristic */
const struct atts_char_desc lns_location_speed_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                                LNS_HDL_LOC_SPD_VAL,
                                                                RBLE_CHAR_LOCATION_SPEED);

/** Location and Navigation Service Position Quality Characteristic */
const struct atts_char_desc lns_position_quality_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                                  LNS_HDL_POS_QUALITY_VAL,
                                                                  RBLE_CHAR_POSITION_QUALITY);

/** Location and Navigation Service LN Control Point Characteristic */
const struct atts_char_desc lns_ln_cp_char = ATTS_CHAR((RBLE_GATT_CHAR_PROP_IND | RBLE_GATT_CHAR_PROP_WR),
                                                       LNS_HDL_LN_CP_VAL,
                                                       RBLE_CHAR_LN_CNTL_POINT);

/** Location and Navigation Service Navigation Characteristic */
const struct atts_char_desc lns_navigation_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
                                                            LNS_HDL_NAVI_VAL,
                                                            RBLE_CHAR_NAVIGATION);

/* LNS ln feature value */
static const uint8_t lns_ln_feature[LNPS_LN_FEATURE_MAX] = {0xff,0xff,0x1f,0x00};		/* all bits is 1 (0x001fffff) */

/* LNS location and speed value */
uint8_t lns_location_speed[LNPS_LOCATION_SPEED_MAX] = {0};

/* LNS position quality value */
uint8_t lns_position_quality[LNPS_POSITION_QUALITY_MAX] =
{
	0x00,0x00,				/* Flags(all present flags is 0, position status = No Position) */
	0,						/* number of beacons in solution */
	0,						/* number of beacons in view */
	0x00,0x00,				/* Time to First Fix */
	0x00,0x00,0x00,0x00,	/* EHPE */
	0x00,0x00,0x00,0x00,	/* EVPE */
	0,						/* HDOP */
	0						/* VDOP */
};

/* LNS LN control point value */
uint8_t lns_ln_cp[LNPS_LN_CP_MAX] = {0};

/* LNS navigation value */
uint8_t lns_navigation[LNPS_NAVIGATION_MAX] = {0};

/** LNS location and speed notification configuration value */
uint16_t lns_location_speed_ntf_en = 0x0000u;

/** LNS ln control point indication configuration value */
uint16_t lns_ln_cp_ind_en = 0x0000u;

/** LNS navigation notification configuration value */
uint16_t lns_navigation_ntf_en = 0x0000u;
#endif

#if (PRF_SEL_PASS)
/***************************************
* Phone Alert Status Service           *
 **************************************/

/** Phone Alert Status Service */
const uint16_t pass_phone_alert_status_svc = RBLE_SVC_PHONE_ALERT_STATUS;

/** Phone Alert Status Service Alert Status Characteristic */
const struct atts_char_desc pass_alert_status_char = ATTS_CHAR( (RBLE_GATT_CHAR_PROP_RD|RBLE_GATT_CHAR_PROP_NTF),
																PASS_HDL_ALERT_STATUS_VAL,
																RBLE_CHAR_ALERT_STATUS);

/** Phone Alert Status Service Ringer Setting Characteristic */
const struct atts_char_desc pass_ringer_setting_char = ATTS_CHAR((RBLE_GATT_CHAR_PROP_RD|RBLE_GATT_CHAR_PROP_NTF),
																 PASS_HDL_RINGER_SETTING_VAL,
																 RBLE_CHAR_RINGER_SETTING);

/** Phone Alert Status Service Ringer Ctrl Point Characteristic */
const struct atts_char_desc pass_ringer_cp_char = ATTS_CHAR( RBLE_GATT_CHAR_PROP_WR_NO_RESP,
															PASS_HDL_RINGER_CP_VAL,
															RBLE_CHAR_RINGER_CTRL_POINT);

/* PASS Alert Status value */
uint8_t pass_alert_status = 0x00;

/* PASS Alert Status notification configuration value */
uint16_t pass_alert_status_ntf_en = RBLE_PRF_STOP_NTFIND;

/* PASS Ringer Setting value */
uint8_t pass_ringer_setting = RBLE_PASP_RINGER_SILENT;

/* PASS Ringer Setting notification configuration value */
uint16_t pass_ringer_setting_ntf_en = RBLE_PRF_STOP_NTFIND;

/* PASS Ringer Ctrl Point value */
uint8_t pass_ringer_cp;
#endif

#if (PRF_SEL_RSCS)
/***************************************
* Running Speed and Cadence Service    *
 **************************************/

/** Running Speed and Cadence Service */
const uint16_t rscs_running_speed_svc = RBLE_SVC_RUNNING_SPEED;

/** Running Speed and Cadence Service Rsc Meas Characteristic */
const struct atts_char_desc rscs_rsc_meas_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_NTF,
														  RSCS_HDL_RSC_MEAS_VAL,
														  RBLE_CHAR_RSC_MEAS);

/** Running Speed and Cadence Service Rsc Feature Characteristic */
const struct atts_char_desc rscs_rsc_feature_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
															 RSCS_HDL_RSC_FEATURE_VAL,
															 RBLE_CHAR_RSC_FEATURE);

/** Running Speed and Cadence Service Sensor Location Characteristic */
const struct atts_char_desc rscs_sensor_loc_char = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
															RSCS_HDL_SENSOR_LOC_VAL,
															RBLE_CHAR_SENSOR_LOCATION);

/** Running Speed and Cadence Service Sc Cntl Point Characteristic */
const struct atts_char_desc rscs_sc_cp_char = ATTS_CHAR((RBLE_GATT_CHAR_PROP_WR|RBLE_GATT_CHAR_PROP_IND),
														RSCS_HDL_SC_CP_VAL,
														RBLE_CHAR_SC_CNTL_POINT);

/* RSCS Rsc Meas value */
uint8_t rscs_rsc_meas[RSCPS_RSC_MEAS_MAX] = {0};

/* RSCS Rsc Meas notification configuration value */
uint16_t rscs_rsc_meas_ntf_en = RBLE_PRF_STOP_NTFIND;

/* RSCS Rsc Feature value */
const uint8_t rscs_rsc_feature[RSCPS_RSC_FEATURE_MAX] = {0x1f, 0x00};	/* all bit is 1 (0x001f) */

/* RSCS Sensor Location value */
uint8_t rscs_sensor_loc = RBLE_RSCPC_SENSOR_TOP_OF_SHOE;				/* 1:Top of shoe */

/* RSCS Sc Cntl Point value */
uint8_t rscs_sc_cp[RSCPS_SC_CP_MAX] = {0};

/* RSCS Sc Cntl Point indication configuration value */
uint16_t rscs_sc_cp_ind_en = RBLE_PRF_STOP_NTFIND;

/* Supported Sensor Location Num */
const uint8_t rscps_support_location_num = 5;

/* Supported Sensor Location */
/* 0:other 1:top of shoe 2:in shoe 3:hip 14:chest 15-:reserved */
const uint8_t rscps_support_sensor_location[RSCPS_LOCATION_MAX] = {RBLE_RSCPC_SENSOR_OTHER,RBLE_RSCPC_SENSOR_TOP_OF_SHOE,
																   RBLE_RSCPC_SENSOR_IN_SHOE,RBLE_RSCPC_SENSOR_HIP,RBLE_RSCPC_SENSOR_CHEST,
																   0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF};

#endif /* (PRF_SEL_RSCS) */

#ifdef USE_SAMPLE_PROFILE
/*******************************
 * Sample Custom Service       *
 *******************************/
/* Sample Custom Service */
static const uint8_t samplecustom_svc[RBLE_GATT_128BIT_UUID_OCTET] = RBLE_SVC_SAMPLE_CUSTOM;

/** Sample Custom Service Notify characteristic */
static const struct atts_char128_desc scps_notify_char = { RBLE_GATT_CHAR_PROP_NTF,
                                                           {(uint8_t)(SCS_HDL_NOTIFY_VAL & 0xff),(uint8_t)((SCS_HDL_NOTIFY_VAL >> 8) & 0xff)},
                                                           RBLE_CHAR_SCP_NOTIFY};

uint8_t  scps_notify_char_val[RBLE_ATTM_MAX_VALUE] = {0};
struct atts_elmt_128 scps_notify_char_val_elmt = { RBLE_CHAR_SCP_NOTIFY,
												   RBLE_GATT_128BIT_UUID_OCTET,
												   &scps_notify_char_val[0] };

uint16_t scps_notify_en = 0x0000u;

/** Sample Custom Service Indicate characteristic */
static const struct atts_char128_desc scps_ind_char = { RBLE_GATT_CHAR_PROP_IND,
                                                        {(uint8_t)(SCS_HDL_IND_VAL & 0xff),(uint8_t)((SCS_HDL_IND_VAL >> 8) & 0xff)},
                                                        RBLE_CHAR_SCP_INDICATE};

uint8_t scps_ind_char_val[RBLE_ATTM_MAX_VALUE] = {0};
struct atts_elmt_128 scps_ind_char_val_elmt = { RBLE_CHAR_SCP_INDICATE,
												RBLE_GATT_128BIT_UUID_OCTET,
												&scps_ind_char_val[0] };
uint16_t scps_ind_en = 0x0000u;

/** Sample Custom Service Interval characteristic */
static const struct atts_char128_desc scps_interval_char = { RBLE_GATT_CHAR_PROP_WR|RBLE_GATT_CHAR_PROP_RD,
                                                             {(uint8_t)(SCS_HDL_INTERVAL_VAL & 0xff),(uint8_t)((SCS_HDL_INTERVAL_VAL >> 8) & 0xff)},
                                                             RBLE_CHAR_SCP_INTERVAL};
uint16_t scps_interval_char_val = 0x0064u;
struct atts_elmt_128 scps_interval_char_val_elmt = { RBLE_CHAR_SCP_INTERVAL,
													 RBLE_GATT_128BIT_UUID_OCTET,
													 &scps_interval_char_val };

/** Sample Custom Service Notify Length characteristic */
static const struct atts_char128_desc scps_notify_len_char = { RBLE_GATT_CHAR_PROP_WR|RBLE_GATT_CHAR_PROP_RD,
                                                               {(uint8_t)(SCS_HDL_NOTIFY_LEN_VAL & 0xff),(uint8_t)((SCS_HDL_NOTIFY_LEN_VAL >> 8) & 0xff)},
                                                               RBLE_CHAR_SCP_NTF_LEN};
uint8_t scps_notify_len_char_val = 0x05u;
struct atts_elmt_128 scps_notify_len_char_val_elmt = {  RBLE_CHAR_SCP_NTF_LEN,
														RBLE_GATT_128BIT_UUID_OCTET,
														&scps_notify_len_char_val };

/** Sample Custom Service Indicate Length characteristic */
static const struct atts_char128_desc scps_ind_len_char = { RBLE_GATT_CHAR_PROP_WR|RBLE_GATT_CHAR_PROP_RD,
                                                            {(uint8_t)(SCS_HDL_IND_LEN_VAL & 0xff),(uint8_t)((SCS_HDL_IND_LEN_VAL >> 8) & 0xff)},
                                                            RBLE_CHAR_SCP_IND_LEN};
uint8_t scps_ind_len_char_val = 0x05u;
struct atts_elmt_128 scps_ind_len_char_val_elmt = { RBLE_CHAR_SCP_IND_LEN,
													RBLE_GATT_128BIT_UUID_OCTET,
													&scps_ind_len_char_val };
#endif /* #ifdef USE_SAMPLE_PROFILE */

#ifdef USE_SIMPLE_SAMPLE_PROFILE

#include "sam/sam.h"

/*******************************
 * Simple Sample Service       *
 *******************************/
/* Service (sams) */
static const uint8_t sams_svc[RBLE_GATT_128BIT_UUID_OCTET] = RBLE_SVC_SAMPLE_CUSTOM_SVC;

/* Characteristic(sams:switch_state) */
static const struct atts_char128_desc switch_state_char = {
	RBLE_GATT_CHAR_PROP_NTF,
	{(uint8_t)(SAMS_HDL_SWITCH_STATE_VAL & 0xff),(uint8_t)((SAMS_HDL_SWITCH_STATE_VAL >> 8) & 0xff)},
	RBLE_CHAR_SAMS_SWITCH_STATE};

uint8_t switch_state_char_val[RBLE_ATTM_MAX_VALUE] = {0};

struct atts_elmt_128 switch_state_char_val_elmt = {
	RBLE_CHAR_SAMS_SWITCH_STATE,
	RBLE_GATT_128BIT_UUID_OCTET,
	&switch_state_char_val[0] };

uint16_t switch_state_cccd = 0x0000u;

/* Characteristic(sams:led_control) */
static const struct atts_char128_desc led_control_char = {
	RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR,
	{(uint8_t)(SAMS_HDL_LED_CONTROL_VAL & 0xff),(uint8_t)((SAMS_HDL_LED_CONTROL_VAL >> 8) & 0xff)},
	RBLE_CHAR_SAMS_LED_CONTROL};

uint8_t led_control_char_val[1] = {0};

struct atts_elmt_128 led_control_char_val_elmt = {
	RBLE_CHAR_SAMS_LED_CONTROL,
	RBLE_GATT_128BIT_UUID_OCTET,
	&led_control_char_val[0] };

#endif /* #ifdef USE_SIMPLE_SAMPLE_PROFILE */

/** Attribute Database */
const struct atts_desc atts_desc_list_prf[] =
{
	#if (USE_LINK_LOSS_SERVICE)
	/*********************************
	 * Link Loss service             *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(lls_svc),			sizeof(lls_svc),			TASK_ATTID(TASK_SVC, LLS_IDX_PRIM_SVC),			RBLE_GATT_PERM_RD,						(void *)&lls_svc },
	/*  Link Loss Alert Level characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(lls_alert_lvl_char),	sizeof(lls_alert_lvl_char),	TASK_ATTID(TASK_SVC, LLS_IDX_ALERT_LVL_CHAR),	RBLE_GATT_PERM_RD,						(void *)&lls_alert_lvl_char },
	/* Link Loss Alert Level Value */
	{ RBLE_CHAR_ALERT_LEVEL,
		sizeof(svc_lls_alert_lvl),	sizeof(svc_lls_alert_lvl),	TASK_ATTID(TASK_SVC, LLS_IDX_ALERT_LVL_VAL),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&svc_lls_alert_lvl },
	#endif /* #if (USE_LINK_LOSS_SERVICE) */

	#if (USE_TX_POWER_SERVICE)
	/*********************************
	 * TX Power service              *
	 *********************************/
	/*  TX Power service */
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(tps_svc),			sizeof(tps_svc),			TASK_ATTID(TASK_SVC, TPS_IDX_PRIM_SVC),			RBLE_GATT_PERM_RD,			(void *)&tps_svc },
	/*  TX Power TX Power Level characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(tps_txpw_lvl_char),	sizeof(tps_txpw_lvl_char),	TASK_ATTID(TASK_SVC, TPS_IDX_TXPW_LVL_CHAR),	RBLE_GATT_PERM_RD,			(void *)&tps_txpw_lvl_char },
	/* TX Power TX Power Level Value */
	{ RBLE_CHAR_TX_POWER_LEVEL,
		sizeof(svc_txpw_lvl),		sizeof(svc_txpw_lvl),		TASK_ATTID(TASK_SVC, TPS_IDX_TXPW_LVL_VAL),		RBLE_GATT_PERM_RD,			(void *)&svc_txpw_lvl },
	#endif /* #if (USE_TX_POWER_SERVICE) */

	#if (USE_IMMEDIATE_ALERT_SERVICE)
	/*********************************
	 * Immediate Alert service       *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(ias_svc),			sizeof(ias_svc),			TASK_ATTID(TASK_SVC, IAS_IDX_PRIM_SVC),			RBLE_GATT_PERM_RD,			(void *)&ias_svc },
	/*  Immediate Alert Alert Level characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(ias_alert_lvl_char),	sizeof(ias_alert_lvl_char),	TASK_ATTID(TASK_SVC, IAS_IDX_ALERT_LVL_CHAR),	RBLE_GATT_PERM_RD,			(void *)&ias_alert_lvl_char },
	/* Immediate Alert Alert Level Value */
	{ RBLE_CHAR_ALERT_LEVEL,
		sizeof(svc_ias_alert_lvl),	sizeof(svc_ias_alert_lvl),	TASK_ATTID(TASK_SVC, IAS_IDX_ALERT_LVL_VAL),	RBLE_GATT_PERM_WR,			(void *)&svc_ias_alert_lvl },
	#endif /* #if (USE_IMMEDIATE_ALERT_SERVICE) */

	#if (PRF_SEL_HTPT)
	/*********************************
	 * Health Thermometer service    *
	 *********************************/
	/* Health Thermometer Service */
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(htpt_hlth_therm_svc),	sizeof(htpt_hlth_therm_svc),	TASK_ATTID(TASK_HTPT, HTS_IDX_SVC),					RBLE_GATT_PERM_RD,						(void *)&htpt_hlth_therm_svc },
	/*  Temperature Measurement characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(htpt_temp_meas_char),	sizeof(htpt_temp_meas_char),	TASK_ATTID(TASK_HTPT, HTS_IDX_TEMP_MEAS_CHAR),		RBLE_GATT_PERM_RD,						(void *)&htpt_temp_meas_char },
	/* Temperature Measurement Value */
	{ RBLE_CHAR_TEMPERATURE_MEAS,
		sizeof(hts_packed_temp),		sizeof(hts_packed_temp),		TASK_ATTID(TASK_HTPT, HTS_IDX_TEMP_MEAS_VAL),		RBLE_GATT_PERM_NI,						(void *)&hts_packed_temp[0] },
	/* Temperature Measurement Indication Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hts_temp_meas_ind_en),	sizeof(hts_temp_meas_ind_en),	TASK_ATTID(TASK_HTPT, HTS_IDX_TEMP_MEAS_IND_CFG),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hts_temp_meas_ind_en },
	/*  Temperature Type characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(htpt_temp_type_char),	sizeof(htpt_temp_type_char),	TASK_ATTID(TASK_HTPT, HTS_IDX_TEMP_TYPE_CHAR),		RBLE_GATT_PERM_RD,						(void *)&htpt_temp_type_char },
	/* Temperature Type Value */
	{ RBLE_CHAR_TEMPERATURE_TYPE,
		sizeof(hts_temp_type),			sizeof(hts_temp_type),			TASK_ATTID(TASK_HTPT, HTS_IDX_TEMP_TYPE_VAL),		RBLE_GATT_PERM_RD,						(void *)&hts_temp_type },
	/*  Intermediate Temperature characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(interm_temp_char),		sizeof(interm_temp_char),		TASK_ATTID(TASK_HTPT, HTS_IDX_INTERM_TEMP_CHAR),	RBLE_GATT_PERM_RD,						(void *)&interm_temp_char },
	/* Intermediate Temperature Value */
	{ RBLE_CHAR_INTERMEDIATE_TEMP,
		sizeof(hts_packed_temp),		sizeof(hts_packed_temp),		TASK_ATTID(TASK_HTPT, HTS_IDX_INTERM_TEMP_VAL),		RBLE_GATT_PERM_NI,						(void *)&hts_packed_temp[0] },
	/* Intermediate Temperature Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hts_interm_temp_ntf_en),	sizeof(hts_interm_temp_ntf_en),	TASK_ATTID(TASK_HTPT, HTS_IDX_INTERM_TEMP_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hts_interm_temp_ntf_en },
	/*  Measurement Interval characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(htpt_meas_intv_char),	sizeof(htpt_meas_intv_char),	TASK_ATTID(TASK_HTPT, HTS_IDX_MEAS_INTV_CHAR),		RBLE_GATT_PERM_RD,						(void *)&htpt_meas_intv_char },
	/* Measurement Interval Value */
	{ RBLE_CHAR_MEAS_INTERVAL,
		sizeof(hts_meas_intv),			sizeof(hts_meas_intv),			TASK_ATTID(TASK_HTPT, HTS_IDX_MEAS_INTV_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR_UNAUTH|RBLE_GATT_PERM_NI),	(void *)&hts_meas_intv },
	/* Measurement Interval Indication Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hts_meas_intv_ind_en),	sizeof(hts_meas_intv_ind_en),	TASK_ATTID(TASK_HTPT, HTS_IDX_MEAS_INTV_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hts_meas_intv_ind_en },
	/* Valid Range Value */
	{ RBLE_DESC_VALID_RANGE,
		sizeof(hts_meas_intv_range),	sizeof(hts_meas_intv_range),	TASK_ATTID(TASK_HTPT, HTS_IDX_MEAS_INTV_RANGE_VAL),	RBLE_GATT_PERM_RD,						(void *)&hts_meas_intv_range[0] },
	#endif /* #if (PRF_SEL_HTPT) */

	#if (PRF_SEL_BLPS)
	/*********************************
	 * Blood Pressure service        *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(blps_bldprs_svc),			sizeof(blps_bldprs_svc),			TASK_ATTID(TASK_BLPS, BLS_IDX_SVC),					RBLE_GATT_PERM_RD,				(void *)&blps_bldprs_svc },
	/*  Blood Pressure Measurement characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(blps_bldprs_meas_char),		sizeof(blps_bldprs_meas_char),		TASK_ATTID(TASK_BLPS, BLS_IDX_BLDPRS_MEAS_CHAR),	RBLE_GATT_PERM_RD,				(void *)&blps_bldprs_meas_char },
	/* Blood Pressure Measurement Value */
	{ RBLE_CHAR_BLOOD_PRESSURE_MEAS,
		sizeof(blps_packed_measurements),	sizeof(blps_packed_measurements),	TASK_ATTID(TASK_BLPS, BLS_IDX_BLDPRS_MEAS_VAL),		RBLE_GATT_PERM_NI,				(void *)&blps_packed_measurements[0] },
	/* Blood Pressure Measurement Indication Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(blps_bldprs_meas_ind_en),	sizeof(blps_bldprs_meas_ind_en),	TASK_ATTID(TASK_BLPS, BLS_IDX_BLDPRS_MEAS_IND_CFG),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&blps_bldprs_meas_ind_en },
	/*  Intermediate Cuff Pressure characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(blps_interm_cufprs_char),	sizeof(blps_interm_cufprs_char),	TASK_ATTID(TASK_BLPS, BLS_IDX_INTERM_CUFPRS_CHAR),	RBLE_GATT_PERM_RD,				(void *)&blps_interm_cufprs_char },
	/* Intermediate Cuff Pressure Value */
	{ RBLE_CHAR_INTERMEDIATE_BLOOD_PRESS,
		sizeof(blps_packed_measurements),	sizeof(blps_packed_measurements),	TASK_ATTID(TASK_BLPS, BLS_IDX_INTERM_CUFPRS_VAL),	RBLE_GATT_PERM_NI,				(void *)&blps_packed_measurements[0] },
	/* Intermediate Cuff Pressure Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(blps_interm_cufprs_ntf_en),	sizeof(blps_interm_cufprs_ntf_en),	TASK_ATTID(TASK_BLPS, BLS_IDX_INTERM_CUFPRS_CFG),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&blps_interm_cufprs_ntf_en },
	/*  Blood Pressure Feature characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(blps_bldprs_feat_char),		sizeof(blps_bldprs_feat_char),		TASK_ATTID(TASK_BLPS, BLS_IDX_BLDPRS_FEAT_CHAR),	RBLE_GATT_PERM_RD,				(void *)&blps_bldprs_feat_char },
	/* Blood Pressure Feature Value */
	{ RBLE_CHAR_BLOOD_PRESSURE_FEAT,
		sizeof(blps_bldprs_feat),			sizeof(blps_bldprs_feat),			TASK_ATTID(TASK_BLPS, BLS_IDX_BLDPRS_FEAT_VAL),		RBLE_GATT_PERM_RD,				(void *)&blps_bldprs_feat },
	#endif /* #if (PRF_SEL_BLPS) */

	#if (USE_DEV_INFO_SERVICE || USE_DEV_INFO_SERVICE_PNP)
	/*********************************
	 * Device Information service    *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(dis_svc),				sizeof(dis_svc),				TASK_ATTID(TASK_SVC, DIS_IDX_SVC),				RBLE_GATT_PERM_RD,		(void *)&dis_svc },
	#if (USE_DEV_INFO_SERVICE)
	/*  System ID characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_sys_id_char),		sizeof(dis_sys_id_char),		TASK_ATTID(TASK_SVC, DIS_IDX_SYS_ID_CHAR),		RBLE_GATT_PERM_RD,		(void *)&dis_sys_id_char },
	/* System ID Value */
	{ RBLE_CHAR_SYSTEM_ID,
		sizeof(dis_sys_id_desc),		sizeof(dis_sys_id_desc),		TASK_ATTID(TASK_SVC, DIS_IDX_SYS_ID_VAL),		RBLE_GATT_PERM_RD,		(void *)&dis_sys_id_desc[0] },
	/*  Model Number characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_model_nb_char),		sizeof(dis_model_nb_char),		TASK_ATTID(TASK_SVC, DIS_IDX_MODEL_NB_CHAR),	RBLE_GATT_PERM_RD,		(void *)&dis_model_nb_char },
	/* Model Number  Value */
	{ RBLE_CHAR_MODEL_NUMBER_STRING,
		sizeof(dis_model_nb_desc),		sizeof(dis_model_nb_desc),		TASK_ATTID(TASK_SVC, DIS_IDX_MODEL_NB_VAL),		RBLE_GATT_PERM_RD,		(void *)&dis_model_nb_desc[0] },
	/*  Serial Number characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_serial_nb_char),		sizeof(dis_serial_nb_char),		TASK_ATTID(TASK_SVC, DIS_IDX_SERIAL_NB_CHAR),	RBLE_GATT_PERM_RD,		(void *)&dis_serial_nb_char },
	/* Serial Number  Value */
	{ RBLE_CHAR_SERIAL_NUMBER_STRING,
		sizeof(dis_serial_nb_desc),		sizeof(dis_serial_nb_desc),		TASK_ATTID(TASK_SVC, DIS_IDX_SERIAL_NB_VAL),	RBLE_GATT_PERM_RD,		(void *)&dis_serial_nb_desc[0] },
	/*  Firmware Revision characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_fw_rev_char),		sizeof(dis_fw_rev_char),		TASK_ATTID(TASK_SVC, DIS_IDX_FW_REV_CHAR),		RBLE_GATT_PERM_RD,		(void *)&dis_fw_rev_char },
	/* Firmware Revision Value */
	{ RBLE_CHAR_FW_REVISION_STRING,
		sizeof(dis_fw_rev_desc),		sizeof(dis_fw_rev_desc),		TASK_ATTID(TASK_SVC, DIS_IDX_FW_REV_VAL),		RBLE_GATT_PERM_RD,		(void *)&dis_fw_rev_desc[0] },
	/*  Hardware Revision characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_hw_rev_char),		sizeof(dis_hw_rev_char),		TASK_ATTID(TASK_SVC, DIS_IDX_HW_REV_CHAR),		RBLE_GATT_PERM_RD,		(void *)&dis_hw_rev_char },
	/* Hardware Revision Value */
	{ RBLE_CHAR_HW_REVISION_STRING,
		sizeof(dis_hw_rev_desc),		sizeof(dis_hw_rev_desc),		TASK_ATTID(TASK_SVC, DIS_IDX_HW_REV_VAL),		RBLE_GATT_PERM_RD,		(void *)&dis_hw_rev_desc[0] },
	/*  Software Revision characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_sw_rev_char),		sizeof(dis_sw_rev_char),		TASK_ATTID(TASK_SVC, DIS_IDX_SW_REV_CHAR),		RBLE_GATT_PERM_RD,		(void *)&dis_sw_rev_char },
	/* Software Revision Value */
	{ RBLE_CHAR_SW_REVISION_STRING,
		sizeof(dis_sw_rev_desc),		sizeof(dis_sw_rev_desc),		TASK_ATTID(TASK_SVC, DIS_IDX_SW_REV_VAL),		RBLE_GATT_PERM_RD,		(void *)&dis_sw_rev_desc[0] },
	/*  Manufacturer Name characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_manuf_name_char),	sizeof(dis_manuf_name_char),	TASK_ATTID(TASK_SVC, DIS_IDX_MANUF_NAME_CHAR),	RBLE_GATT_PERM_RD,		(void *)&dis_manuf_name_char },
	/*  Manufacturer Name  Value */
	{ RBLE_CHAR_MANUF_NAME_STRING,
		sizeof(dis_manuf_name_desc),	sizeof(dis_manuf_name_desc),	TASK_ATTID(TASK_SVC, DIS_IDX_MANUF_NAME_VAL),	RBLE_GATT_PERM_RD,		(void *)&dis_manuf_name_desc[0] },
	/*  IEEE Certifications List characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_ieee_certif_char),	sizeof(dis_ieee_certif_char),	TASK_ATTID(TASK_SVC, DIS_IDX_IEEE_CERTIF_CHAR),	RBLE_GATT_PERM_RD,		(void *)&dis_ieee_certif_char },
	/* IEEE Certifications List Value */
	{ RBLE_CHAR_IEEE_CERTIF,
		sizeof(dis_ieee_certif_desc),	sizeof(dis_ieee_certif_desc),	TASK_ATTID(TASK_SVC, DIS_IDX_IEEE_CERTIF_VAL),	RBLE_GATT_PERM_RD,		(void *)&dis_ieee_certif_desc[0] },
	#endif /* #if (USE_DEV_INFO_SERVICE) */
	#if (USE_DEV_INFO_SERVICE_PNP)
	/*  PnP ID characteristic definition*/
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(dis_pnp_id_char),		sizeof(dis_pnp_id_char),		TASK_ATTID(TASK_SVC, DIS_IDX_PNP_ID_CHAR),		RBLE_GATT_PERM_RD,		(void *)&dis_pnp_id_char },
	/* PnP ID Value */
	{ RBLE_CHAR_PNP_ID,
		sizeof(dis_pnp_id),				sizeof(dis_pnp_id),				TASK_ATTID(TASK_SVC, DIS_IDX_PNP_ID_VAL),		RBLE_GATT_PERM_RD,		(void *)&dis_pnp_id[0] },
	#endif /* #if (USE_DEV_INFO_SERVICE_PNP) */
	#endif /* #if (USE_DEV_INFO_SERVICE || USE_DEV_INFO_SERVICE_PNP) */

	#if (PRF_SEL_HGHD)
	/*********************************
	 * HID Service(Instance 1)       *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(hghd_hid_svc1),					sizeof(hghd_hid_svc1),						TASK_ATTID(TASK_HGHD, HIDS1_IDX_SVC),					RBLE_GATT_PERM_RD,			(void *)&hghd_hid_svc1 },
	/* Include Service(Instance 1) */
	{ RBLE_DECL_INCLUDE,
		sizeof(hghd_hid_incl_svc1),				sizeof(hghd_hid_incl_svc1),					TASK_ATTID(TASK_HGHD, HIDS1_IDX_INCLUDE_SVC),			RBLE_GATT_PERM_RD,			(void *)&hghd_hid_incl_svc1 },
	/* Protocol Mode characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_protocol_md_char1),			sizeof(hghd_protocol_md_char1),				TASK_ATTID(TASK_HGHD, HIDS1_IDX_PROTOCOL_MD_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_protocol_md_char1 },
	/* Protocol Mode characteristic(Instance 1) Value */
	{ RBLE_CHAR_PROTOCOL_MODE,
		sizeof(hids_inst[0].protocol_mode),		sizeof(hids_inst[0].protocol_mode),			TASK_ATTID(TASK_HGHD, HIDS1_IDX_PROTOCOL_MD_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[0].protocol_mode },
	/* Report(Input) characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_report_input_char1),		sizeof(hghd_report_input_char1),			TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_INPUT_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_report_input_char1 },
	/* Report(Input) characteristic(Instance 1) Value */
	{ RBLE_CHAR_REPORT,
		sizeof(hids_inst[0].input_report),		0,											TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_INPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI), (void *)&hids_inst[0].input_report[0] },
	/* Report(Input) characteristic(Instance 1) Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hids_inst[0].report_input_ntf_en),sizeof(hids_inst[0].report_input_ntf_en),	TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_INPUT_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[0].report_input_ntf_en },
	/* Report(Input) characteristic(Instance 1) Report Reference Value */
	{ RBLE_DESC_REPORT_REFERENCE,
		sizeof(hids_inst[0].report_input_ref),	sizeof(hids_inst[0].report_input_ref),		TASK_ATTID(TASK_HGHD, HIDS1_IDX_INPUT_REP_REF),			RBLE_GATT_PERM_RD,			(void *)&hids_inst[0].report_input_ref[0] },
	/* Report(Output) characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_report_output_char1),		sizeof(hghd_report_output_char1),			TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_OUTPUT_CHAR),	RBLE_GATT_PERM_RD,			(void *)&hghd_report_output_char1 },
	/* Report(Output) characteristic(Instance 1) Value */
	{ RBLE_CHAR_REPORT,
		sizeof(hids_inst[0].output_report),		0,											TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_OUTPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[0].output_report[0] },
	/* Report(Output) characteristic(Instance 1) Report Reference Value */
	{ RBLE_DESC_REPORT_REFERENCE,
		sizeof(hids_inst[0].report_output_ref),	sizeof(hids_inst[0].report_output_ref),		TASK_ATTID(TASK_HGHD, HIDS1_IDX_OUTPUT_REP_REF),		RBLE_GATT_PERM_RD,			(void *)&hids_inst[0].report_output_ref[0] },
	/* Report(Feature) characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_report_feature_char1),		sizeof(hghd_report_feature_char1),			TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_FEATURE_CHAR),	RBLE_GATT_PERM_RD,			(void *)&hghd_report_feature_char1 },
	/* Report(Feature) characteristic(Instance 1) Value */
	{ RBLE_CHAR_REPORT,
		sizeof(hids_inst[0].feature_report),	0,											TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_FEATURE_VAL),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[0].feature_report[0] },
	/* Report(Feature) characteristic(Instance 1) Report Reference Value */
	{ RBLE_DESC_REPORT_REFERENCE,
		sizeof(hids_inst[0].report_feature_ref),sizeof(hids_inst[0].report_feature_ref),	TASK_ATTID(TASK_HGHD, HIDS1_IDX_FEATURE_REP_REF),		RBLE_GATT_PERM_RD,			(void *)&hids_inst[0].report_feature_ref[0] },
	/* Report Map characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_report_map_char1),			sizeof(hghd_report_map_char1),				TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_MAP_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_report_map_char1 },
	/* Report Map characteristic(Instance 1) Value */
	{ RBLE_CHAR_REPORT_MAP,
		sizeof(hids1_report_map),				sizeof(hids1_report_map),					TASK_ATTID(TASK_HGHD, HIDS1_IDX_REPORT_MAP_VAL),		RBLE_GATT_PERM_RD,			(void *)&hids1_report_map[0] },
	/* Report Map characteristic(Instance 1) External Report Reference Value */
	{ RBLE_DESC_EXT_REPORT_REFERENCE,
		sizeof(hids1_ext_report_ref),			sizeof(hids1_ext_report_ref),				TASK_ATTID(TASK_HGHD, HIDS1_IDX_EXTERNAL_REP_REF),		RBLE_GATT_PERM_RD,			(void *)&hids1_ext_report_ref[0] },
	#if (HIDS_INST1 == HID_BOOT_KB)
	/* Boot Keyboard Input Report characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_bootkb_input_char1),		sizeof(hghd_bootkb_input_char1),			TASK_ATTID(TASK_HGHD, HIDS1_IDX_BOOTKB_INPUT_CHAR), 	RBLE_GATT_PERM_RD,			(void *)&hghd_bootkb_input_char1 },
	/* Boot Keyboard Input Report characteristic(Instance 1) Value */
	{ RBLE_CHAR_BOOT_KB_INPUT_REPORT,
		sizeof(hids_inst[0].kb_input_report),	0,											TASK_ATTID(TASK_HGHD, HIDS1_IDX_BOOTKB_INPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI), (void *)&hids_inst[0].kb_input_report[0] },
	/* Boot Keyboard Input Report characteristic(Instance 1) Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hids_inst[0].kb_input_report_ntf_en),sizeof(hids_inst[0].kb_input_report_ntf_en), TASK_ATTID(TASK_HGHD, HIDS1_IDX_BOOTKB_INPUT_CFG), (RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[0].kb_input_report_ntf_en },
	/* Boot Keyboard Output Report characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_bootkb_output_char1),		sizeof(hghd_bootkb_output_char1),			TASK_ATTID(TASK_HGHD, HIDS1_IDX_BOOTKB_OUTPUT_CHAR),	RBLE_GATT_PERM_RD,			(void *)&hghd_bootkb_output_char1 },
	/* Boot Keyboard Output Report characteristic(Instance 1) Value */
	{ RBLE_CHAR_BOOT_KB_OUTPUT_REPORT,
		sizeof(hids_inst[0].kb_output_report),	0,											TASK_ATTID(TASK_HGHD, HIDS1_IDX_BOOTKB_OUTPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[0].kb_output_report[0] },
	#elif (HIDS_INST1 == HID_BOOT_MO)
	/* Boot Mouse Input Report characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_bootmo_input_char1),		sizeof(hghd_bootmo_input_char1),			TASK_ATTID(TASK_HGHD, HIDS1_IDX_BOOTMO_INPUT_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_bootmo_input_char1 },
	/* Boot Mouse Input Report characteristic(Instance 1) Value */
	{ RBLE_CHAR_BOOT_MOUSE_INPUT_REPORT,
		sizeof(hids_inst[0].mo_input_report),	0,											TASK_ATTID(TASK_HGHD, HIDS1_IDX_BOOTMO_INPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI), (void *)&hids_inst[0].mo_input_report[0] },
	/* Boot Mouse Input Report characteristic(Instance 1) Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hids_inst[0].mo_input_report_ntf_en),sizeof(hids_inst[0].mo_input_report_ntf_en), TASK_ATTID(TASK_HGHD, HIDS1_IDX_BOOTMO_INPUT_CFG), (RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[0].mo_input_report_ntf_en },
	#endif
	/* HID Information characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_hid_info_char1),			sizeof(hghd_hid_info_char1),				TASK_ATTID(TASK_HGHD, HIDS1_IDX_HID_INFO_CHAR),			RBLE_GATT_PERM_RD,			(void *)&hghd_hid_info_char1 },
	/* HID Information characteristic(Instance 1) Value */
	{ RBLE_CHAR_HID_INFO,
		sizeof(hids1_hid_info),					sizeof(hids1_hid_info),						TASK_ATTID(TASK_HGHD, HIDS1_IDX_HID_INFO_VAL),			RBLE_GATT_PERM_RD,			(void *)&hids1_hid_info[0] },
	/* HID Control Point characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_hid_cp_char1),				sizeof(hghd_hid_cp_char1),					TASK_ATTID(TASK_HGHD, HIDS1_IDX_HID_CP_CHAR),			RBLE_GATT_PERM_RD,			(void *)&hghd_hid_cp_char1 },
	/* HID Control Point characteristic(Instance 1) Value */
	{ RBLE_CHAR_HID_CTRL_POINT,
		sizeof(hids_inst[0].hid_cp),			sizeof(hids_inst[0].hid_cp),				TASK_ATTID(TASK_HGHD, HIDS1_IDX_HID_CP_VAL),			RBLE_GATT_PERM_WR,			(void *)&hids_inst[0].hid_cp },
	#ifdef HID_MULTI_INSTANCE
	/*********************************
	 * HID Service(Instance 2)       *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(hghd_hid_svc2),					sizeof(hghd_hid_svc2),						TASK_ATTID(TASK_HGHD, HIDS2_IDX_SVC),					RBLE_GATT_PERM_RD,			(void *)&hghd_hid_svc2 },
	/* Include Service(Instance 2) */
	{ RBLE_DECL_INCLUDE,
		sizeof(hghd_hid_incl_svc2),				sizeof(hghd_hid_incl_svc2),					TASK_ATTID(TASK_HGHD, HIDS2_IDX_INCLUDE_SVC),			RBLE_GATT_PERM_RD,			(void *)&hghd_hid_incl_svc2 },
	/* Protocol Mode characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_protocol_md_char2),			sizeof(hghd_protocol_md_char2),				TASK_ATTID(TASK_HGHD, HIDS2_IDX_PROTOCOL_MD_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_protocol_md_char2 },
	/* Protocol Mode characteristic(Instance 2) Value */
	{ RBLE_CHAR_PROTOCOL_MODE,
		sizeof(hids_inst[1].protocol_mode),		sizeof(hids_inst[1].protocol_mode),			TASK_ATTID(TASK_HGHD, HIDS2_IDX_PROTOCOL_MD_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[1].protocol_mode },
	/* Report(Input) characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_report_input_char2),		sizeof(hghd_report_input_char2),			TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_INPUT_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_report_input_char2 },
	/* Report(Input) characteristic(Instance 2) Value */
	{ RBLE_CHAR_REPORT,
		sizeof(hids_inst[1].input_report),		0,											TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_INPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI), (void *)&hids_inst[1].input_report[0] },
	/* Report(Input) characteristic(Instance 2) Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hids_inst[1].report_input_ntf_en),sizeof(hids_inst[1].report_input_ntf_en),	TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_INPUT_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[1].report_input_ntf_en },
	/* Report(Input) characteristic(Instance 2) Report Reference Value */
	{ RBLE_DESC_REPORT_REFERENCE,
		sizeof(hids_inst[1].report_input_ref),	sizeof(hids_inst[1].report_input_ref),		TASK_ATTID(TASK_HGHD, HIDS2_IDX_INPUT_REP_REF),			RBLE_GATT_PERM_RD,			(void *)&hids_inst[1].report_input_ref[0] },
	/* Report(Output) characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_report_output_char2),		sizeof(hghd_report_output_char2),			TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_OUTPUT_CHAR),	RBLE_GATT_PERM_RD,			(void *)&hghd_report_output_char2 },
	/* Report(Output) characteristic(Instance 2) Value */
	{ RBLE_CHAR_REPORT,
		sizeof(hids_inst[1].output_report),		0,											TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_OUTPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[1].output_report[0] },
	/* Report(Output) characteristic(Instance 2) Report Reference Value */
	{ RBLE_DESC_REPORT_REFERENCE,
		sizeof(hids_inst[1].report_output_ref),	sizeof(hids_inst[1].report_output_ref),		TASK_ATTID(TASK_HGHD, HIDS2_IDX_OUTPUT_REP_REF),		RBLE_GATT_PERM_RD,			(void *)&hids_inst[1].report_output_ref[0] },
	/* Report(Feature) characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_report_feature_char2),		sizeof(hghd_report_feature_char2),			TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_FEATURE_CHAR),	RBLE_GATT_PERM_RD,			(void *)&hghd_report_feature_char2 },
	/* Report(Feature) characteristic(Instance 2) Value */
	{ RBLE_CHAR_REPORT,
		sizeof(hids_inst[1].feature_report),	0,											TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_FEATURE_VAL),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[1].feature_report[0] },
	/* Report(Feature) characteristic(Instance 2) Report Reference Value */
	{ RBLE_DESC_REPORT_REFERENCE,
		sizeof(hids_inst[1].report_feature_ref),sizeof(hids_inst[1].report_feature_ref),	TASK_ATTID(TASK_HGHD, HIDS2_IDX_FEATURE_REP_REF),		RBLE_GATT_PERM_RD,			(void *)&hids_inst[1].report_feature_ref[0] },
	/* Report Map characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_report_map_char2),			sizeof(hghd_report_map_char2),				TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_MAP_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_report_map_char2 },
	/* Report Map characteristic(Instance 2) Value */
	{ RBLE_CHAR_REPORT_MAP,
		sizeof(hids2_report_map),				sizeof(hids2_report_map),					TASK_ATTID(TASK_HGHD, HIDS2_IDX_REPORT_MAP_VAL),		RBLE_GATT_PERM_RD,			(void *)&hids2_report_map[0] },
	/* Report Map characteristic(Instance 2) External Report Reference Value */
	{ RBLE_DESC_EXT_REPORT_REFERENCE,
		sizeof(hids2_ext_report_ref),			sizeof(hids2_ext_report_ref),				TASK_ATTID(TASK_HGHD, HIDS2_IDX_EXTERNAL_REP_REF),		RBLE_GATT_PERM_RD,			(void *)&hids2_ext_report_ref[0] },
	#if (HIDS_INST2 == HID_BOOT_KB)
	/* Boot Keyboard Input Report characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_bootkb_input_char2),		sizeof(hghd_bootkb_input_char2),			TASK_ATTID(TASK_HGHD, HIDS2_IDX_BOOTKB_INPUT_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_bootkb_input_char2 },
	/* Boot Keyboard Input Report characteristic(Instance 2) Value */
	{ RBLE_CHAR_BOOT_KB_INPUT_REPORT,
		sizeof(hids_inst[1].kb_input_report),	0,											TASK_ATTID(TASK_HGHD, HIDS2_IDX_BOOTKB_INPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI), (void *)&hids_inst[1].kb_input_report[0] },
	/* Boot Keyboard Input Report characteristic(Instance 2) Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hids_inst[1].kb_input_report_ntf_en), sizeof(hids_inst[1].kb_input_report_ntf_en), TASK_ATTID(TASK_HGHD, HIDS2_IDX_BOOTKB_INPUT_CFG),(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[1].kb_input_report_ntf_en },
	/* Boot Keyboard Output Report characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_bootkb_output_char2), 		sizeof(hghd_bootkb_output_char2),			TASK_ATTID(TASK_HGHD, HIDS2_IDX_BOOTKB_OUTPUT_CHAR),	RBLE_GATT_PERM_RD,			(void *)&hghd_bootkb_output_char2 },
	/* Boot Keyboard Output Report characteristic(Instance 2) Value */
	{ RBLE_CHAR_BOOT_KB_OUTPUT_REPORT,
		sizeof(hids_inst[1].kb_output_report),	0,											TASK_ATTID(TASK_HGHD, HIDS2_IDX_BOOTKB_OUTPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[1].kb_output_report[0] },
	#elif (HIDS_INST2 == HID_BOOT_MO)
	/* Boot Mouse Input Report characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_bootmo_input_char2),		sizeof(hghd_bootmo_input_char2),			TASK_ATTID(TASK_HGHD, HIDS2_IDX_BOOTMO_INPUT_CHAR),		RBLE_GATT_PERM_RD,			(void *)&hghd_bootmo_input_char2 },
	/* Boot Mouse Input Report characteristic(Instance 2) Value */
	{ RBLE_CHAR_BOOT_MOUSE_INPUT_REPORT,
		sizeof(hids_inst[1].mo_input_report),	0,											TASK_ATTID(TASK_HGHD, HIDS2_IDX_BOOTMO_INPUT_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI), (void *)&hids_inst[1].mo_input_report[0] },
	/* Boot Mouse Input Report characteristic(Instance 2) Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hids_inst[1].mo_input_report_ntf_en),sizeof(hids_inst[1].mo_input_report_ntf_en), TASK_ATTID(TASK_HGHD, HIDS2_IDX_BOOTMO_INPUT_CFG),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hids_inst[1].mo_input_report_ntf_en },
	#endif
	/* HID Information characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_hid_info_char2),			sizeof(hghd_hid_info_char2),				TASK_ATTID(TASK_HGHD, HIDS2_IDX_HID_INFO_CHAR),			RBLE_GATT_PERM_RD,			(void *)&hghd_hid_info_char2 },
	/* HID Information characteristic(Instance 2) Value */
	{ RBLE_CHAR_HID_INFO,
		sizeof(hids2_hid_info),					sizeof(hids2_hid_info),						TASK_ATTID(TASK_HGHD, HIDS2_IDX_HID_INFO_VAL),			RBLE_GATT_PERM_RD,			(void *)&hids2_hid_info[0] },
	/* HID Control Point characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hghd_hid_cp_char2),				sizeof(hghd_hid_cp_char2),					TASK_ATTID(TASK_HGHD, HIDS2_IDX_HID_CP_CHAR),			RBLE_GATT_PERM_RD,			(void *)&hghd_hid_cp_char2 },
	/* HID Control Point characteristic(Instance 2) Value */
	{ RBLE_CHAR_HID_CTRL_POINT,
		sizeof(hids_inst[1].hid_cp),			sizeof(hids_inst[1].hid_cp),				TASK_ATTID(TASK_HGHD, HIDS2_IDX_HID_CP_VAL),			RBLE_GATT_PERM_WR,			(void *)&hids_inst[1].hid_cp },
	#endif /* #ifdef HID_MULTI_INSTANCE */
	#endif /* #if (PRF_SEL_HGHD) */

	#if (USE_BATTERY_SERVICE)
	/*********************************
	 * Battery Service(Instance 1)   *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(bas_svc1),						sizeof(bas_svc1),						TASK_ATTID(TASK_SVC, BAS1_IDX_SVC),					RBLE_GATT_PERM_RD,			(void *)&bas_svc1 },
	/* Battery Level characteristic(Instance 1) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(bas_battery_lvl_char1),			sizeof(bas_battery_lvl_char1),			TASK_ATTID(TASK_SVC, BAS1_IDX_BATTERY_LVL_CHAR),	RBLE_GATT_PERM_RD,			(void *)&bas_battery_lvl_char1 },
	/* Battery Level characteristic(Instance 1) Value */
	{ RBLE_CHAR_BATTERY_LEVEL,
		sizeof(bas_inst[0].battery_lvl),		sizeof(bas_inst[0].battery_lvl),		TASK_ATTID(TASK_SVC, BAS1_IDX_BATTERY_LVL_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_NI),	(void *)&bas_inst[0].battery_lvl },
	/* Battery Level characteristic(Instance 1) Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(bas_inst[0].battery_lvl_ntf_en), sizeof(bas_inst[0].battery_lvl_ntf_en),	TASK_ATTID(TASK_SVC, BAS1_IDX_BATTERY_LVL_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&bas_inst[0].battery_lvl_ntf_en },
	/* Battery Level characteristic(Instance 1) Presentation Format */
	{ RBLE_DESC_CHAR_PRESENTATION_FMT,
		sizeof(bas1_char_presen),				sizeof(bas1_char_presen),				TASK_ATTID(TASK_SVC, BAS1_IDX_CHAR_PRESEN_FMT),		RBLE_GATT_PERM_RD,			(void *)&bas1_char_presen[0] },
	#if (PRF_SEL_HGHD)
	/* Battery Level characteristic(Instance 1) Report Reference Value */
	{ RBLE_DESC_REPORT_REFERENCE,
		sizeof(bas_inst[0].battery_lvl_rep_ref),sizeof(bas_inst[0].battery_lvl_rep_ref),TASK_ATTID(TASK_SVC, BAS1_IDX_BATT_REP_REF),		RBLE_GATT_PERM_RD,			(void *)&bas_inst[0].battery_lvl_rep_ref[0] },
	#endif /* #if (PRF_SEL_HGHD) */
	#ifdef HID_MULTI_INSTANCE
	/*********************************
	 * Battery Service(Instance 2)   *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(bas_svc2),						sizeof(bas_svc2),						TASK_ATTID(TASK_SVC, BAS2_IDX_SVC),					RBLE_GATT_PERM_RD,			(void *)&bas_svc2 },
	/* Battery Level characteristic(Instance 2) */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(bas_battery_lvl_char2),			sizeof(bas_battery_lvl_char2),			TASK_ATTID(TASK_SVC, BAS2_IDX_BATTERY_LVL_CHAR),	RBLE_GATT_PERM_RD,			(void *)&bas_battery_lvl_char2 },
	/* Battery Level characteristic(Instance 2) Value */
	{ RBLE_CHAR_BATTERY_LEVEL,
		sizeof(bas_inst[1].battery_lvl),		sizeof(bas_inst[1].battery_lvl),		TASK_ATTID(TASK_SVC, BAS2_IDX_BATTERY_LVL_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_NI),	(void *)&bas_inst[1].battery_lvl },
	/* Battery Level characteristic(Instance 2) Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(bas_inst[1].battery_lvl_ntf_en),	sizeof(bas_inst[1].battery_lvl_ntf_en),	TASK_ATTID(TASK_SVC, BAS2_IDX_BATTERY_LVL_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&bas_inst[1].battery_lvl_ntf_en },
	/* Battery Level characteristic(Instance 2) Presentation Format */
	{ RBLE_DESC_CHAR_PRESENTATION_FMT,
		sizeof(bas2_char_presen),				sizeof(bas2_char_presen),				TASK_ATTID(TASK_SVC, BAS2_IDX_CHAR_PRESEN_FMT),		RBLE_GATT_PERM_RD,			(void *)&bas2_char_presen[0] },
	#if (PRF_SEL_HGHD)
	/* Battery Level characteristic(Instance 2) Report Reference Value */
	{ RBLE_DESC_REPORT_REFERENCE,
		sizeof(bas_inst[1].battery_lvl_rep_ref),sizeof(bas_inst[1].battery_lvl_rep_ref),TASK_ATTID(TASK_SVC, BAS2_IDX_BATT_REP_REF),		RBLE_GATT_PERM_RD,			(void *)&bas_inst[1].battery_lvl_rep_ref[0] },
	#endif /* #if (PRF_SEL_HGHD) */
	#endif /* #ifdef HID_MULTI_INSTANCE */
	#endif /* #if (USE_BATTERY_SERVICE) */

	#if (USE_SCAN_PARAMETER_SERVICE)
	/*********************************
	 * Scan Parameters Service       *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(scanparam_svc),				sizeof(scanparam_svc),			TASK_ATTID(TASK_SVC, SCANS_IDX_SVC),				RBLE_GATT_PERM_RD,				(void *)&scanparam_svc },
	/* Scan Interval Window characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(scans_intv_window_char),		sizeof(scans_intv_window_char),	TASK_ATTID(TASK_SVC, SCANS_IDX_INTV_WINDOW_CHAR),	RBLE_GATT_PERM_RD,				(void *)&scans_intv_window_char },
	/* Scan Interval Window Value */
	{ RBLE_CHAR_SCAN_INTERVAL_WINDOW,
		sizeof(scans_intv_window),			sizeof(scans_intv_window),		TASK_ATTID(TASK_SVC, SCANS_IDX_INTV_WINDOW_VAL),	RBLE_GATT_PERM_WR,				(void *)&scans_intv_window },
	/* Scan Refresh characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(scans_refresh_char),			sizeof(scans_refresh_char),		TASK_ATTID(TASK_SVC, SCANS_IDX_REFRESH_CHAR),		RBLE_GATT_PERM_RD,				(void *)&scans_refresh_char },
	/* Scan Refresh Value */
	{ RBLE_CHAR_SCAN_REFRESH,
		sizeof(scans_refresh_val),			sizeof(scans_refresh_val),		TASK_ATTID(TASK_SVC, SCANS_IDX_REFRESH_VAL),		RBLE_GATT_PERM_NI,				(void *)&scans_refresh_val },
	/* Scan Refresh Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(scans_refresh_ntf_en),		sizeof(scans_refresh_ntf_en),	TASK_ATTID(TASK_SVC, SCANS_IDX_REFRESH_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&scans_refresh_ntf_en },
	#endif /* #if (USE_SCAN_PARAMETER_SERVICE) */

	#if (PRF_SEL_HRPS)
	/*********************************
	 * Heart Rate Service            *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(hrps_heart_rate_svc),		sizeof(hrps_heart_rate_svc),	TASK_ATTID(TASK_HRPS, HRS_IDX_SVC),					RBLE_GATT_PERM_RD,				(void *)&hrps_heart_rate_svc },
	/* Heart Rate Measurement characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hrps_hrtrate_meas_char),		sizeof(hrps_hrtrate_meas_char),	TASK_ATTID(TASK_HRPS, HRS_IDX_HRTRATE_MEAS_CHAR),	RBLE_GATT_PERM_RD,				(void *)&hrps_hrtrate_meas_char },
	/* Heart Rate Measurement Value */
	{ RBLE_CHAR_HEART_RATE_MEAS,
		sizeof(hrps_packed_measurements),	sizeof(hrps_packed_measurements),TASK_ATTID(TASK_HRPS, HRS_IDX_HRTRATE_MEAS_VAL),	RBLE_GATT_PERM_NI,				(void *)&hrps_packed_measurements[0] },
	/* Heart Rate Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(hrps_hrtrate_meas_ntf_en),	sizeof(hrps_hrtrate_meas_ntf_en),TASK_ATTID(TASK_HRPS, HRS_IDX_HRTRATE_MEAS_NTF_CFG),(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&hrps_hrtrate_meas_ntf_en },
	/* Body Sensor Location characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hrps_body_sensor_location_char),	sizeof(hrps_body_sensor_location_char),	TASK_ATTID(TASK_HRPS, HRS_IDX_HRTRATE_BSL_CHAR),RBLE_GATT_PERM_RD,		(void *)&hrps_body_sensor_location_char },
	/* Body Sensor Location Value */
	{ RBLE_CHAR_BODY_SENSOR_LOCATION,
		sizeof(hrps_body_sensor_location),	sizeof(hrps_body_sensor_location),	TASK_ATTID(TASK_HRPS, HRS_IDX_HRTRATE_BSL_VAL),	RBLE_GATT_PERM_RD,				(void *)&hrps_body_sensor_location },
	/* Heart Rate Control Point characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(hrps_hrtrate_cp_char),		sizeof(hrps_hrtrate_cp_char),	TASK_ATTID(TASK_HRPS, HRS_IDX_HRTRATE_CP_CHAR),		RBLE_GATT_PERM_RD,				(void *)&hrps_hrtrate_cp_char },
	/* Heart Rate Control Point Value */
	{ RBLE_CHAR_HEART_RATE_CTRL_POINT,
		sizeof(hrps_hrtrate_control_point),	sizeof(hrps_hrtrate_control_point),	TASK_ATTID(TASK_HRPS, HRS_IDX_HRTRATE_CP_VAL),	RBLE_GATT_PERM_WR,				(void *)&hrps_hrtrate_control_point },
	#endif /* #if (PRF_SEL_HRPS) */

	#if (PRF_SEL_CSCS)
	/*************************************
	 * Cycling Speed and Cadence Service *
	 *************************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
	    sizeof(cscps_cycling_speed_svc),    sizeof(cscps_cycling_speed_svc),TASK_ATTID(TASK_CSCPS, CSCS_IDX_SVC),               RBLE_GATT_PERM_RD,              (void*) &cscps_cycling_speed_svc },
	/* CSC Measurement characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
	    sizeof(cscps_csc_meas_char),        sizeof(cscps_csc_meas_char),    TASK_ATTID(TASK_CSCPS, CSCS_IDX_CSC_MEAS_CHAR),     RBLE_GATT_PERM_RD,              (void*) &cscps_csc_meas_char },
	/* CSC Measurement Value */
	{ RBLE_CHAR_CSC_MEAS,
	    sizeof(cscps_packed_measurements),  sizeof(cscps_packed_measurements), TASK_ATTID(TASK_CSCPS, CSCS_IDX_CSC_MEAS_VAL),   RBLE_GATT_PERM_NI,              (void*) &cscps_packed_measurements[0] },
	/* CSC Measurements Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
	    sizeof(cscps_cycspd_meas_ntf_en),   sizeof(cscps_cycspd_meas_ntf_en),TASK_ATTID(TASK_CSCPS, CSCS_IDX_CSC_MEAS_NTF_CFG), (RBLE_GATT_PERM_RD | RBLE_GATT_PERM_WR),(void*) &cscps_cycspd_meas_ntf_en },
	/* CSC Feature characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
	    sizeof(cscps_csc_feature_char),     sizeof(cscps_csc_feature_char), TASK_ATTID(TASK_CSCPS, CSCS_IDX_CSC_FEATURE_CHAR),  RBLE_GATT_PERM_RD,              (void*) &cscps_csc_feature_char },
	/* CSC Feature Value */
	{ RBLE_CHAR_CSC_FEATURE,
	    sizeof(cscps_csc_feature),          sizeof(cscps_csc_feature),      TASK_ATTID(TASK_CSCPS, CSCS_IDX_CSC_FEATURE_VAL),   RBLE_GATT_PERM_RD,              (void*) &cscps_csc_feature },
	/* Sensor Location characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
	    sizeof(cscps_sensor_location_char), sizeof(cscps_sensor_location_char), TASK_ATTID(TASK_CSCPS, CSCS_IDX_SENSOR_LOC_CHAR), RBLE_GATT_PERM_RD,            (void*) &cscps_sensor_location_char },
	/* Sensor Location Value */
	{ RBLE_CHAR_SENSOR_LOCATION,
	    sizeof(cscps_sensor_location),      sizeof(cscps_sensor_location), TASK_ATTID(TASK_CSCPS, CSCS_IDX_SENSOR_LOC_VAL),     RBLE_GATT_PERM_RD,              (void*) &cscps_sensor_location },
	/* SC Control Point characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
	    sizeof(cscps_sc_cp_char),           sizeof(cscps_sc_cp_char),       TASK_ATTID(TASK_CSCPS, CSCS_IDX_SC_CP_CHAR),        RBLE_GATT_PERM_RD,              (void*) &cscps_sc_cp_char },
	/* SC Control Point Value */
	{ RBLE_CHAR_SC_CNTL_POINT,
	    sizeof(cscps_packed_sc_cp),         sizeof(cscps_packed_sc_cp),     TASK_ATTID(TASK_CSCPS, CSCS_IDX_SC_CP_VAL),         (RBLE_GATT_PERM_WR | RBLE_GATT_PERM_NI),(void*) &cscps_packed_sc_cp[0] },
	/* SC Control Point Indication Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
	    sizeof(cscps_sc_cp_ind_en),         sizeof(cscps_sc_cp_ind_en),     TASK_ATTID(TASK_CSCPS, CSCS_IDX_SC_CP_IND_CFG),     (RBLE_GATT_PERM_RD | RBLE_GATT_PERM_WR),(void*) &cscps_sc_cp_ind_en },
	#endif /* #if (PRF_SEL_CSCS) */
	
	#if (PRF_SEL_GLPS)
	/*************************************
	 * Glucose Service                   *
	 *************************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
	    sizeof(glps_glucose_svc),    sizeof(glps_glucose_svc),TASK_ATTID(TASK_GLPS, GLS_IDX_SVC),               RBLE_GATT_PERM_RD,              (void*) &glps_glucose_svc },
	/* Glucose Measurement characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
	    sizeof(glps_meas_char),        sizeof(glps_meas_char),    TASK_ATTID(TASK_GLPS, GLS_IDX_MEAS_CHAR),     RBLE_GATT_PERM_RD,              (void*) &glps_meas_char },

	/* Glucose Measurement Value */
	{ RBLE_CHAR_GLUCOSE_MEASUREMENT,
	    sizeof(glps_packed_measurements),  sizeof(glps_packed_measurements), TASK_ATTID(TASK_GLPS, GLS_IDX_MEAS_VAL),   RBLE_GATT_PERM_NI,      (void*) &glps_packed_measurements[0] },

	/* Glucose Measurements Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
	    sizeof(glps_meas_ntf_en),   sizeof(glps_meas_ntf_en),TASK_ATTID(TASK_GLPS, GLS_IDX_MEAS_NTF_CFG), (RBLE_GATT_PERM_RD | RBLE_GATT_PERM_WR),(void*) &glps_meas_ntf_en },

	/* Glucose Measurement Context characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
	    sizeof(glps_meas_context_char),    sizeof(glps_meas_context_char),    TASK_ATTID(TASK_GLPS, GLS_IDX_MEAS_CONTEXT_CHAR),     RBLE_GATT_PERM_RD,   (void*) &glps_meas_context_char },

	/* Glucose Measurement Value */
	{ RBLE_CHAR_GLUCOSE_MEAS_CONTEXT,
	    sizeof(glps_packed_measurements_context),  sizeof(glps_packed_measurements_context), TASK_ATTID(TASK_GLPS, GLS_IDX_MEAS_CONTEXT_VAL),   RBLE_GATT_PERM_NI,  (void*) &glps_packed_measurements_context[0] },

	/* Glucose Measurements Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
	    sizeof(glps_meas_context_ntf_en),   sizeof(glps_meas_context_ntf_en),TASK_ATTID(TASK_GLPS, GLS_IDX_MEAS_CONTEXT_NTF_CFG), (RBLE_GATT_PERM_RD | RBLE_GATT_PERM_WR),(void*) &glps_meas_context_ntf_en },

	/* Glucose Feature characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
	    sizeof(glps_feature_char),     sizeof(glps_feature_char), TASK_ATTID(TASK_GLPS, GLS_IDX_FEATURE_CHAR),  RBLE_GATT_PERM_RD,              (void*) &glps_feature_char },

	/* Glucose Feature Value */
	{ RBLE_CHAR_GLUCOSE_FEATURE,
	    sizeof(glps_feature),          sizeof(glps_feature),      TASK_ATTID(TASK_GLPS, GLS_IDX_FEATURE_VAL),   RBLE_GATT_PERM_RD,              (void*) &glps_feature },

	/* RA Control Point characteristic */
	{ RBLE_DECL_CHARACTERISTIC,
	    sizeof(glps_ra_cp_char),           sizeof(glps_ra_cp_char),       TASK_ATTID(TASK_GLPS, GLS_IDX_RA_CP_CHAR),        RBLE_GATT_PERM_RD,              (void*) &glps_ra_cp_char },

	/* RA Control Point Value */
	{ RBLE_CHAR_RECORD_ACCESS_CTRL_POINT,
	    sizeof(glps_packed_ra_cp),         sizeof(glps_packed_ra_cp),     TASK_ATTID(TASK_GLPS, GLS_IDX_RA_CP_VAL),         (RBLE_GATT_PERM_WR_UNAUTH | RBLE_GATT_PERM_NI),(void*) &glps_packed_ra_cp[0] },

	/* RA Control Point Indication Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
	    sizeof(glps_ra_cp_ind_en),         sizeof(glps_ra_cp_ind_en),     TASK_ATTID(TASK_GLPS, GLS_IDX_RA_CP_IND_CFG),     (RBLE_GATT_PERM_RD | RBLE_GATT_PERM_WR),(void*) &glps_ra_cp_ind_en },

	#endif /* #if (PRF_SEL_GLPS) */
	
	#if (PRF_SEL_CPPS)
	/*********************************
	 * Cycling Power Service       *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(cpps_cycpwr_svc),			sizeof(cpps_cycpwr_svc),			TASK_ATTID(TASK_CPPS, CPS_IDX_SVC),				RBLE_GATT_PERM_RD,				(void *)&cpps_cycpwr_svc },
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(cpps_cycpwr_meas_char),		sizeof(cpps_cycpwr_meas_char),		TASK_ATTID(TASK_CPPS, CPS_IDX_MEAS_CHAR),		RBLE_GATT_PERM_RD,				(void *)&cpps_cycpwr_meas_char },
	/* Cycling Power Measurement Value */
	{ RBLE_CHAR_CYCLING_POWER_MEAS,
		sizeof(cpps_packed_measurements),	sizeof(cpps_packed_measurements),	TASK_ATTID(TASK_CPPS, CPS_IDX_MEAS_VAL),		RBLE_GATT_PERM_NI,				(void *)&cpps_packed_measurements[0] },
	/* Cycling Power Measurement Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(cpps_cycpwr_meas_ntf_en),	sizeof(cpps_cycpwr_meas_ntf_en),	TASK_ATTID(TASK_CPPS, CPS_IDX_MEAS_NTF_CFG),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&cpps_cycpwr_meas_ntf_en },
	/* Cycling Power Measurement Broadcast Cfg Value */
	{ RBLE_DESC_SERVER_CHAR_CONF,
		sizeof(cpps_cycpwr_meas_brd_en),	sizeof(cpps_cycpwr_meas_brd_en),	TASK_ATTID(TASK_CPPS, CPS_IDX_MEAS_BRD_CFG),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&cpps_cycpwr_meas_brd_en },
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(cpps_cycpwr_feature_char),	sizeof(cpps_cycpwr_feature_char),	TASK_ATTID(TASK_CPPS, CPS_IDX_FEATURE_CHAR),	RBLE_GATT_PERM_RD,				(void *)&cpps_cycpwr_feature_char },
	/* Cycling Power Feature Value */
	{ RBLE_CHAR_CYCLING_POWER_FEATURE,
		sizeof(cpps_cycpwr_feat),			sizeof(cpps_cycpwr_feat),			TASK_ATTID(TASK_CPPS, CPS_IDX_FEATURE_VAL),		RBLE_GATT_PERM_RD,				(void *)&cpps_cycpwr_feat },
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(cpps_sensor_location_char),	sizeof(cpps_sensor_location_char),	TASK_ATTID(TASK_CPPS, CPS_IDX_SENSOR_LOCATION_CHAR),	RBLE_GATT_PERM_RD,		(void *)&cpps_sensor_location_char },
	/* Sensor Location Value */
	{ RBLE_CHAR_SENSOR_LOCATION,
		sizeof(cpps_sensor_location),		sizeof(cpps_sensor_location),		TASK_ATTID(TASK_CPPS, CPS_IDX_SENSOR_LOCATIONVAL),		RBLE_GATT_PERM_RD,		(void *)&cpps_sensor_location },
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(cpps_cycpwr_vector_char),	sizeof(cpps_cycpwr_vector_char),	TASK_ATTID(TASK_CPPS, CPS_IDX_VECTOR_CHAR),		RBLE_GATT_PERM_RD,				(void *)&cpps_cycpwr_vector_char },
	/* Cycling Power Vector Value */
	{ RBLE_CHAR_CYCLING_POWER_VECTOR,
		sizeof(cpps_packed_vector),			sizeof(cpps_packed_vector),			TASK_ATTID(TASK_CPPS, CPS_IDX_VECTOR_VAL),		RBLE_GATT_PERM_NI,				(void *)&cpps_packed_vector[0] },
	/* Cycling Power Vector Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(cpps_cycpwr_vector_ntf_en),	sizeof(cpps_cycpwr_vector_ntf_en),	TASK_ATTID(TASK_CPPS, CPS_IDX_VECTOR_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&cpps_cycpwr_vector_ntf_en },
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(cpps_cycpwr_cp_char),		sizeof(cpps_cycpwr_cp_char),		TASK_ATTID(TASK_CPPS, CPS_IDX_CP_CHAR),			RBLE_GATT_PERM_RD,				(void *)&cpps_cycpwr_cp_char },
	/* Cycling Power Control Point Value */
	{ RBLE_CHAR_CYCLING_POWER_CNTL_POINT,
		sizeof(cpps_packed_cp),				sizeof(cpps_packed_cp),				TASK_ATTID(TASK_CPPS, CPS_IDX_CP_VAL),			(RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI),	(void *)&cpps_packed_cp[0] },
	/* Cycling Power Control Point Indication Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(cpps_cycpwr_cp_ind_en),		sizeof(cpps_cycpwr_cp_ind_en),		TASK_ATTID(TASK_CPPS, CPS_IDX_CP_CFG),			(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&cpps_cycpwr_cp_ind_en },
	#endif /* #if (PRF_SEL_CPPS) */
	
	#if (PRF_SEL_TIPS)
	/*********************************
	 * Current Time Service          *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(cts_current_time_svc),		sizeof(cts_current_time_svc),		TASK_ATTID(TASK_TIPS, CTS_IDX_SVC),					RBLE_GATT_PERM_RD,			(void *)&cts_current_time_svc },
	/* Current Time Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(cts_current_time_char),		sizeof(cts_current_time_char),		TASK_ATTID(TASK_TIPS, CTS_IDX_CURRENT_TIME_CHAR),	RBLE_GATT_PERM_RD,			(void *)&cts_current_time_char },
	/* Current Time Value */
	{ RBLE_CHAR_CURRENT_TIME,
		sizeof(cts_current_time),			sizeof(cts_current_time),			TASK_ATTID(TASK_TIPS, CTS_IDX_CURRENT_TIME_VAL),	(RBLE_GATT_PERM_NI|RBLE_GATT_PERM_RD),	(void *)&cts_current_time[0] },
	/* Current Time Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(cts_current_time_ntf_en),	sizeof(cts_current_time_ntf_en),	TASK_ATTID(TASK_TIPS, CTS_IDX_CURRENT_TIME_NTF_CFG),(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&cts_current_time_ntf_en },
	/* Local Time Information Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(cts_local_time_char),		sizeof(cts_local_time_char),		TASK_ATTID(TASK_TIPS, CTS_IDX_LOCAL_TIME_CHAR),		RBLE_GATT_PERM_RD,			(void *)&cts_local_time_char },
	/* Local Time Information Value */
	{ RBLE_CHAR_LOCAL_TIME_INFO,
		sizeof(cts_local_time),				sizeof(cts_local_time),				TASK_ATTID(TASK_TIPS, CTS_IDX_LOCAL_TIME_VAL),		RBLE_GATT_PERM_RD,			(void *)&cts_local_time[0] },
	/* Reference Time Information Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(cts_ref_time_char),			sizeof(cts_ref_time_char),			TASK_ATTID(TASK_TIPS, CTS_IDX_REF_TIME_CHAR),		RBLE_GATT_PERM_RD,			(void *)&cts_ref_time_char },
	/* Current Time Value */
	{ RBLE_CHAR_REF_TIME_INFO,
		sizeof(cts_ref_time),				sizeof(cts_ref_time),				TASK_ATTID(TASK_TIPS, CTS_IDX_REF_TIME_VAL),		RBLE_GATT_PERM_RD,			(void *)&cts_ref_time[0] },

	/*********************************
	 * Next DST Chage Service         *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(ndcs_next_dst_chg_svc),		sizeof(ndcs_next_dst_chg_svc),		TASK_ATTID(TASK_TIPS, NDCS_IDX_SVC),				RBLE_GATT_PERM_RD,			(void *)&ndcs_next_dst_chg_svc },
	/* Time with DST Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(ndcs_time_dst_char),			sizeof(ndcs_time_dst_char),			TASK_ATTID(TASK_TIPS, NDCS_IDX_TIME_DST_CHAR),		RBLE_GATT_PERM_RD,			(void *)&ndcs_time_dst_char },
	/* Time with DST Value */
	{ RBLE_CHAR_TIME_WITH_DST,
		sizeof(ndcs_time_dst),				sizeof(ndcs_time_dst),				TASK_ATTID(TASK_TIPS, NDCS_IDX_TIME_DST_VAL),		RBLE_GATT_PERM_RD,			(void *)&ndcs_time_dst[0] },

	/*********************************
	 * Reference Time Update Service  *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(rtus_ref_time_update_svc),	sizeof(rtus_ref_time_update_svc),	TASK_ATTID(TASK_TIPS, RTUS_IDX_SVC),				RBLE_GATT_PERM_RD,			(void *)&rtus_ref_time_update_svc },
	/* Time update Control Point Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(rtus_time_update_cp_char),	sizeof(rtus_time_update_cp_char),	TASK_ATTID(TASK_TIPS, RTUS_IDX_TIME_UPDATE_CP_CHAR),RBLE_GATT_PERM_RD,			(void *)&rtus_time_update_cp_char },
	/* Time update Control Point Value */
	{ RBLE_CHAR_TIME_UPDATE_CTRL_POINT,
		sizeof(rtus_time_update_cp),		sizeof(rtus_time_update_cp),		TASK_ATTID(TASK_TIPS, RTUS_IDX_TIME_UPDATE_CP_VAL),	RBLE_GATT_PERM_WR,			(void *)&rtus_time_update_cp },
	/* Time update State Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(rtus_time_update_state_char),	sizeof(rtus_time_update_state_char),	TASK_ATTID(TASK_TIPS, RTUS_IDX_TIME_UPDATE_STATE_CHAR),	RBLE_GATT_PERM_RD,	(void *)&rtus_time_update_state_char },
	/* Time update State Value */
	{ RBLE_CHAR_TIME_UPDATE_STATE,
		sizeof(rtus_time_update_state),		sizeof(rtus_time_update_state),		TASK_ATTID(TASK_TIPS, RTUS_IDX_TIME_UPDATE_STATE_VAL),	RBLE_GATT_PERM_RD,		(void *)&rtus_time_update_state[0] },
	#endif
	
	#if (PRF_SEL_ANPS)
	/*********************************
	 * Alert Notification Service     *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(ans_alert_ntf_svc),			sizeof(ans_alert_ntf_svc),			TASK_ATTID(TASK_ANPS, ANPS_IDX_SVC),					RBLE_GATT_PERM_RD,			(void *)&ans_alert_ntf_svc },
	/* Supported New Alert Category Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(ans_supp_new_alert_char),	sizeof(ans_supp_new_alert_char),	TASK_ATTID(TASK_ANPS, ANPS_IDX_SUPP_NEW_AL_CATEGORY_CHAR),	RBLE_GATT_PERM_RD,	(void *)&ans_supp_new_alert_char },
	/* Supported New Alert Category Value */
	{ RBLE_CHAR_SUPP_NEW_AL_CATEGORY,
		sizeof(ans_supp_new_alert),			sizeof(ans_supp_new_alert),			TASK_ATTID(TASK_ANPS, ANPS_IDX_SUPP_NEW_AL_CATEGORY_VAL),	RBLE_GATT_PERM_RD,	(void *)&ans_supp_new_alert[0] },
	/* New Alert Category Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(ans_new_alert_char),			sizeof(ans_new_alert_char),			TASK_ATTID(TASK_ANPS, ANPS_IDX_NEW_ALERT_CHAR),		RBLE_GATT_PERM_RD,			(void *)&ans_new_alert_char },
	/* New Alert Category Value */
	{ RBLE_CHAR_NEW_ALERT,
		sizeof(ans_new_alert),				sizeof(ans_new_alert),				TASK_ATTID(TASK_ANPS, ANPS_IDX_NEW_ALERT_VAL),		RBLE_GATT_PERM_NI,			(void *)&ans_new_alert[0] },
	/* New Alert Category Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(ans_new_alert_ntf_en),		sizeof(ans_new_alert_ntf_en),		TASK_ATTID(TASK_ANPS, ANPS_IDX_NEW_ALERT_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&ans_new_alert_ntf_en },
	/* Supported Unread Alert Status Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(ans_supp_unread_alert_char),	sizeof(ans_supp_unread_alert_char),	TASK_ATTID(TASK_ANPS, ANPS_IDX_SUPP_UNREAD_CATEGORY_CHAR),	RBLE_GATT_PERM_RD,	(void *)&ans_supp_unread_alert_char },
	/* Supported Unread Alert Status Value */
	{ RBLE_CHAR_SUPP_UNREAD_AL_CATEGORY,
		sizeof(ans_supp_unread_alert),		sizeof(ans_supp_unread_alert),		TASK_ATTID(TASK_ANPS, ANPS_IDX_SUPP_UNREAD_CATEGORY_VAL),	RBLE_GATT_PERM_RD,	(void *)&ans_supp_unread_alert[0] },
	/* Unread Alert Status Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(ans_unread_alert_char),		sizeof(ans_unread_alert_char),		TASK_ATTID(TASK_ANPS, ANPS_IDX_UNREAD_ALERT_STATUS_CHAR),	RBLE_GATT_PERM_RD,	(void *)&ans_unread_alert_char },
	/* Unread Alert Status Value */
	{ RBLE_CHAR_UNREAD_ALERT_STATUS,
		sizeof(ans_unread_alert),			sizeof(ans_unread_alert),			TASK_ATTID(TASK_ANPS, ANPS_IDX_UNREAD_ALERT_STATUS_VAL),	RBLE_GATT_PERM_NI,	(void *)&ans_unread_alert[0] },
	/* Unread Alert Status Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(ans_unread_alert_ntf_en),	sizeof(ans_unread_alert_ntf_en),	TASK_ATTID(TASK_ANPS, ANPS_IDX_UNREAD_ALERT_STATUS_CFG),(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&ans_unread_alert_ntf_en },
	/* Alert Notification Control Point Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(ans_alert_ntf_cp_char),		sizeof(ans_alert_ntf_cp_char),		TASK_ATTID(TASK_ANPS, ANPS_IDX_AL_NTF_CTRL_POINT_CHAR),	RBLE_GATT_PERM_RD,		(void *)&ans_alert_ntf_cp_char },
	/* Alert Notification Control Point Value */
	{ RBLE_CHAR_AL_NOTIF_CTRL_POINT,
		sizeof(ans_alert_ntf_cp),			sizeof(ans_alert_ntf_cp),			TASK_ATTID(TASK_ANPS, ANPS_IDX_AL_NTF_CTRL_POINT_VAL),	RBLE_GATT_PERM_WR,		(void *)&ans_alert_ntf_cp[0] },
	#endif
	
	#if (PRF_SEL_LNPS)
	/***************************************
	 * Location and Navigation Service     *
	 **************************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(lns_location_navi_svc),		sizeof(lns_location_navi_svc),		TASK_ATTID(TASK_LNPS, LNS_IDX_SVC),						RBLE_GATT_PERM_RD,		(void *)&lns_location_navi_svc },
	/* LN Feature Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(lns_ln_feature_char),		sizeof(lns_ln_feature_char),		TASK_ATTID(TASK_LNPS, LNS_IDX_LN_FEATURE_CHAR),			RBLE_GATT_PERM_RD,		(void *)&lns_ln_feature_char },
	/* LN Feature Value */
	{ RBLE_CHAR_LN_FEATURE,
		sizeof(lns_ln_feature),				sizeof(lns_ln_feature),				TASK_ATTID(TASK_LNPS, LNS_IDX_LN_FEATURE_VAL),			RBLE_GATT_PERM_RD,		(void *)&lns_ln_feature[0] },
	/* Location and Speed Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(lns_location_speed_char),	sizeof(lns_location_speed_char),	TASK_ATTID(TASK_LNPS, LNS_IDX_LOC_SPD_CHAR),			RBLE_GATT_PERM_RD,		(void *)&lns_location_speed_char },
	/* Location and Speed Value */
	{ RBLE_CHAR_LOCATION_SPEED,
		sizeof(lns_location_speed),			sizeof(lns_location_speed),			TASK_ATTID(TASK_LNPS, LNS_IDX_LOC_SPD_VAL),				RBLE_GATT_PERM_NI,		(void *)&lns_location_speed[0] },
	/* Location and Speed Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(lns_location_speed_ntf_en),	sizeof(lns_location_speed_ntf_en),	TASK_ATTID(TASK_LNPS, LNS_IDX_LOC_SPD_NTF_CFG),			(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&lns_location_speed_ntf_en },
	/* Position Quality Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(lns_position_quality_char),	sizeof(lns_position_quality_char),	TASK_ATTID(TASK_LNPS, LNS_IDX_POS_QUALITY_CHAR),		RBLE_GATT_PERM_RD,		(void *)&lns_position_quality_char },
	/* Position Quality Value */
	{ RBLE_CHAR_POSITION_QUALITY,
		sizeof(lns_position_quality),		sizeof(lns_position_quality),		TASK_ATTID(TASK_LNPS, LNS_IDX_POS_QUALITY_VAL),			RBLE_GATT_PERM_RD,		(void *)&lns_position_quality[0] },
	/* LN Control Point Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(lns_ln_cp_char),				sizeof(lns_ln_cp_char),				TASK_ATTID(TASK_LNPS, LNS_IDX_LN_CP_CHAR),				RBLE_GATT_PERM_RD,		(void *)&lns_ln_cp_char },
	/* LN Control Point Value */
	{ RBLE_CHAR_LN_CNTL_POINT,
		sizeof(lns_ln_cp),					sizeof(lns_ln_cp),					TASK_ATTID(TASK_LNPS, LNS_IDX_LN_CP_VAL),				(RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI),	(void *)&lns_ln_cp[0] },
	/* LN Control Point Indication Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(lns_ln_cp_ind_en),			sizeof(lns_ln_cp_ind_en),			TASK_ATTID(TASK_LNPS, LNS_IDX_LN_CP_IND_CFG),			(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&lns_ln_cp_ind_en },
	/* Navigation Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(lns_navigation_char),		sizeof(lns_navigation_char),		TASK_ATTID(TASK_LNPS, LNS_IDX_NAVI_CHAR),				RBLE_GATT_PERM_RD,		(void *)&lns_navigation_char },
	/* Navigation Value */
	{ RBLE_CHAR_NAVIGATION,
		sizeof(lns_navigation),				sizeof(lns_navigation),				TASK_ATTID(TASK_LNPS, LNS_IDX_NAVI_VAL),				RBLE_GATT_PERM_NI,		(void *)&lns_navigation[0] },
	/* Navigation Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(lns_navigation_ntf_en),		sizeof(lns_navigation_ntf_en),		TASK_ATTID(TASK_LNPS, LNS_IDX_NAVI_NTF_CFG),			(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&lns_navigation_ntf_en },
	#endif
	
	#if (PRF_SEL_PASS)
	/***************************************
	 * Phone Alert Status Service          *
	 **************************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(pass_phone_alert_status_svc),	sizeof(pass_phone_alert_status_svc),	TASK_ATTID(TASK_PASPS, PASS_IDX_SVC),			RBLE_GATT_PERM_RD,		(void *)&pass_phone_alert_status_svc },
	/* Alert Status Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(pass_alert_status_char),		sizeof(pass_alert_status_char),		TASK_ATTID(TASK_PASPS, PASS_IDX_ALERT_STATUS_CHAR),		RBLE_GATT_PERM_RD,		(void *)&pass_alert_status_char },
	/* Alert Status Value */
	{ RBLE_CHAR_ALERT_STATUS,
		sizeof(pass_alert_status),			sizeof(pass_alert_status),			TASK_ATTID(TASK_PASPS, PASS_IDX_ALERT_STATUS_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_NI),	(void *)&pass_alert_status },
	/* Alert Status Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(pass_alert_status_ntf_en),	sizeof(pass_alert_status_ntf_en),	TASK_ATTID(TASK_PASPS, PASS_IDX_ALERT_STATUS_NTF_CFG),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&pass_alert_status_ntf_en },
	/* Ringer Setting Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(pass_ringer_setting_char),	sizeof(pass_ringer_setting_char),	TASK_ATTID(TASK_PASPS, PASS_IDX_RINGER_SETTING_CHAR),	RBLE_GATT_PERM_RD,		(void *)&pass_ringer_setting_char },
	/* Ringer Setting Value */
	{ RBLE_CHAR_RINGER_SETTING,
		sizeof(pass_ringer_setting),		sizeof(pass_ringer_setting),		TASK_ATTID(TASK_PASPS, PASS_IDX_RINGER_SETTING_VAL),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_NI),	(void *)&pass_ringer_setting },
	/* Ringer Setting Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(pass_ringer_setting_ntf_en),	sizeof(pass_ringer_setting_ntf_en),	TASK_ATTID(TASK_PASPS, PASS_IDX_RINGER_SETTING_NTF_CFG),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&pass_ringer_setting_ntf_en },
	/* Ringer Ctrl Point Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(pass_ringer_cp_char),		sizeof(pass_ringer_cp_char),		TASK_ATTID(TASK_PASPS, PASS_IDX_RINGER_CP_CHAR),		RBLE_GATT_PERM_RD,		(void *)&pass_ringer_cp_char },
	/* Ringer Ctrl Point Value */
	{ RBLE_CHAR_RINGER_CTRL_POINT,
		sizeof(pass_ringer_cp),				sizeof(pass_ringer_cp),				TASK_ATTID(TASK_PASPS, PASS_IDX_RINGER_CP_VAL),			RBLE_GATT_PERM_WR,		(void *)&pass_ringer_cp },
	#endif /* (PRF_SEL_PASS) */
	
	#if (PRF_SEL_RSCS)
	/***************************************
	 * Running Speed and Cadence Service   *
	 **************************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(rscs_running_speed_svc),		sizeof(rscs_running_speed_svc),		TASK_ATTID(TASK_RSCPS, RSCS_IDX_SVC),					RBLE_GATT_PERM_RD,		(void *)&rscs_running_speed_svc },
	/* Rsc Meas Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(rscs_rsc_meas_char),			sizeof(rscs_rsc_meas_char),			TASK_ATTID(TASK_RSCPS, RSCS_IDX_RSC_MEAS_CHAR),			RBLE_GATT_PERM_RD,		(void *)&rscs_rsc_meas_char },
	/* Rsc Meas Value */
	{ RBLE_CHAR_RSC_MEAS,
		sizeof(rscs_rsc_meas),				sizeof(rscs_rsc_meas),				TASK_ATTID(TASK_RSCPS, RSCS_IDX_RSC_MEAS_VAL),			RBLE_GATT_PERM_NI,		(void *)&rscs_rsc_meas[0] },
	/* Rsc Meas Notification Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(rscs_rsc_meas_ntf_en),		sizeof(rscs_rsc_meas_ntf_en),		TASK_ATTID(TASK_RSCPS, RSCS_IDX_RSC_MEAS_NTF_CFG),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&rscs_rsc_meas_ntf_en },
	/* Rsc Feature Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(rscs_rsc_feature_char),		sizeof(rscs_rsc_feature_char),		TASK_ATTID(TASK_RSCPS, RSCS_IDX_RSC_FEATURE_CHAR),		RBLE_GATT_PERM_RD,		(void *)&rscs_rsc_feature_char },
	/* Rsc Feature Value */
	{ RBLE_CHAR_RSC_FEATURE,
		sizeof(rscs_rsc_feature),			sizeof(rscs_rsc_feature),			TASK_ATTID(TASK_RSCPS, RSCS_IDX_RSC_FEATURE_VAL),		RBLE_GATT_PERM_RD,		(void *)&rscs_rsc_feature[0] },
	/* Sensor Location Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(rscs_sensor_loc_char),		sizeof(rscs_sensor_loc_char),		TASK_ATTID(TASK_RSCPS, RSCS_IDX_SENSOR_LOC_CHAR),		RBLE_GATT_PERM_RD,		(void *)&rscs_sensor_loc_char },
	/* Sensor Location Value */
	{ RBLE_CHAR_SENSOR_LOCATION,
		sizeof(rscs_sensor_loc),			sizeof(rscs_sensor_loc),			TASK_ATTID(TASK_RSCPS, RSCS_IDX_SENSOR_LOC_VAL),		RBLE_GATT_PERM_RD,		(void *)&rscs_sensor_loc },
	/* Sc Cntl Point Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(rscs_sc_cp_char),			sizeof(rscs_sc_cp_char),			TASK_ATTID(TASK_RSCPS, RSCS_IDX_SC_CP_CHAR),			RBLE_GATT_PERM_RD,		(void *)&rscs_sc_cp_char },
	/* Sc Cntl Point Value */
	{ RBLE_CHAR_SC_CNTL_POINT,
		sizeof(rscs_sc_cp),					sizeof(rscs_sc_cp),					TASK_ATTID(TASK_RSCPS, RSCS_IDX_SC_CP_VAL),				(RBLE_GATT_PERM_WR|RBLE_GATT_PERM_NI),	(void *)&rscs_sc_cp[0] },
	/* Sc Cntl Point Indication Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(rscs_sc_cp_ind_en),			sizeof(rscs_sc_cp_ind_en),			TASK_ATTID(TASK_RSCPS, RSCS_IDX_SC_CP_IND_CFG),			(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&rscs_sc_cp_ind_en },
	#endif /* (PRF_SEL_RSCS) */

	#ifdef USE_SAMPLE_PROFILE
	/*********************************
	 * Sample Custom Service         *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
		sizeof(samplecustom_svc),			sizeof(samplecustom_svc),		TASK_ATTID(TASK_RBLE, SCS_IDX_SVC),					RBLE_GATT_PERM_RD,				(void *)&samplecustom_svc },
	/* Sample Custom Notify Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(scps_notify_char),			sizeof(scps_notify_char),		TASK_ATTID(TASK_RBLE, SCS_IDX_NOTIFY_CHAR),			RBLE_GATT_PERM_RD,				(void *)&scps_notify_char },
	/* Sample Custom Notify Value */
	{ DB_TYPE_128BIT_UUID,
		sizeof(scps_notify_char_val),		sizeof(scps_notify_char_val),	TASK_ATTID(TASK_RBLE, SCS_IDX_NOTIFY_VAL),			RBLE_GATT_PERM_NI,				(void *)&scps_notify_char_val_elmt },
	/* Sample Custom Notify Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(scps_notify_en),				sizeof(scps_notify_en),			TASK_ATTID(TASK_RBLE, SCS_IDX_NOTIFY_CFG),			(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&scps_notify_en },
	/* Sample Custom Indicate Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(scps_ind_char),				sizeof(scps_ind_char),			TASK_ATTID(TASK_RBLE, SCS_IDX_IND_CHAR),			RBLE_GATT_PERM_RD,				(void *)&scps_ind_char },
	/* Sample Custom Indicate Value */
	{ DB_TYPE_128BIT_UUID,
		sizeof(scps_ind_char_val),			sizeof(scps_ind_char_val),		TASK_ATTID(TASK_RBLE, SCS_IDX_IND_VAL),				RBLE_GATT_PERM_NI,				(void *)&scps_ind_char_val_elmt },
	/* Sample Custom Indicate Cfg Value */
	{ RBLE_DESC_CLIENT_CHAR_CONF,
		sizeof(scps_ind_en),				sizeof(scps_ind_en),			TASK_ATTID(TASK_RBLE, SCS_IDX_IND_CFG),				(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&scps_ind_en },
	/* Sample Custom Interval Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(scps_interval_char),			sizeof(scps_interval_char),		TASK_ATTID(TASK_RBLE, SCS_IDX_INTERVAL_CHAR),		RBLE_GATT_PERM_RD,				(void *)&scps_interval_char },
	/* Sample Custom Interval Value */
	{ DB_TYPE_128BIT_UUID,
		sizeof(scps_interval_char_val),		sizeof(scps_interval_char_val),	TASK_ATTID(TASK_RBLE, SCS_IDX_INTERVAL_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&scps_interval_char_val_elmt },
	/* Sample Custom Notify Length Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(scps_notify_len_char),		sizeof(scps_notify_len_char),	TASK_ATTID(TASK_RBLE, SCS_IDX_NOTIFY_LEN_CHAR),		RBLE_GATT_PERM_RD,				(void *)&scps_notify_len_char },
	/* Sample Custom Notify Length Value */
	{ DB_TYPE_128BIT_UUID,
		sizeof(scps_notify_len_char_val),	sizeof(scps_notify_len_char_val),TASK_ATTID(TASK_RBLE, SCS_IDX_NOTIFY_LEN_VAL),		(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&scps_notify_len_char_val_elmt },
	/* Sample Custom Indicate Length Char */
	{ RBLE_DECL_CHARACTERISTIC,
		sizeof(scps_ind_len_char),			sizeof(scps_ind_len_char),		TASK_ATTID(TASK_RBLE, SCS_IDX_IND_LEN_CHAR),		RBLE_GATT_PERM_RD,				(void *)&scps_ind_len_char },
	/* Sample Custom Indicate Length Value */
	{ DB_TYPE_128BIT_UUID,
		sizeof(scps_ind_len_char_val),		sizeof(scps_ind_len_char_val),	TASK_ATTID(TASK_RBLE, SCS_IDX_IND_LEN_VAL),			(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&scps_ind_len_char_val_elmt },
	#endif /* #ifdef USE_SAMPLE_PROFILE */

	#ifdef USE_SIMPLE_SAMPLE_PROFILE
	/*********************************
	 * Simple Sample Service         *
	 *********************************/
	{ RBLE_DECL_PRIMARY_SERVICE,
        sizeof(sams_svc),	sizeof(sams_svc),	TASK_ATTID(TASK_RBLE, SAMS_IDX_SVC),	RBLE_GATT_PERM_RD,	(void *)&sams_svc },
	/* Characteristic: switch_state */
	{ RBLE_DECL_CHARACTERISTIC,
        sizeof(switch_state_char),	sizeof(switch_state_char),	TASK_ATTID(TASK_RBLE,SAMS_IDX_SWITCH_STATE_CHAR),	RBLE_GATT_PERM_RD,	(void *)&switch_state_char },
	{ DB_TYPE_128BIT_UUID,
        sizeof(switch_state_char_val),	sizeof(switch_state_char_val),	TASK_ATTID(TASK_RBLE,SAMS_IDX_SWITCH_STATE_VAL),	(RBLE_GATT_PERM_NI),	(void *)&switch_state_char_val_elmt },
	{ RBLE_DESC_CLIENT_CHAR_CONF,
        sizeof(switch_state_cccd),	sizeof(switch_state_cccd),	TASK_ATTID(TASK_RBLE,SAMS_IDX_SWITCH_STATE_CCCD),	(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),	(void *)&switch_state_cccd },
	/* Characteristic: led_control */
	{ RBLE_DECL_CHARACTERISTIC,
        sizeof(led_control_char),	sizeof(led_control_char),	TASK_ATTID(TASK_RBLE,SAMS_IDX_LED_CONTROL_CHAR),	RBLE_GATT_PERM_RD,	(void *)&led_control_char },
	{ DB_TYPE_128BIT_UUID,
        sizeof(led_control_char_val),	sizeof(led_control_char_val),	TASK_ATTID(TASK_RBLE,SAMS_IDX_LED_CONTROL_VAL),	(RBLE_GATT_PERM_RD | RBLE_GATT_PERM_WR),	(void *)&led_control_char_val_elmt },
	#endif /* #ifdef USE_SIMPLE_SAMPLE_PROFILE */

	/* Reserved */
	{0,0,0,0,0,0}
};

const uint16_t ATT_PRF_DB_SIZE = sizeof(atts_desc_list_prf);


