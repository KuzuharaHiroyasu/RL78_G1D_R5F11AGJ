/**
 ****************************************************************************************
 *
 * @file        prf_config_host.c
 *
 * @brief   Setting of Configuration Parameters for Profile
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: 3835 $
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifdef _USE_IAR_RL78
#define _HDB_DATA _Pragma("location=\"HDB_DATA\"") __NO_INIT
#define _HDB_CNST _Pragma("location=\"HDB_CNST\"")
#else
#define _HDB_DATA
#define _HDB_CNST

#if !defined(_USE_CCRL_RL78)
#pragma section @@DATA HDB_DATA
#pragma section @@CNST HDB_CNST
#else
#pragma section bss    HDB_DATA
#pragma section const  HDB_CNST
#endif
#endif

/**
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"
#include "rwke_api.h"

#include "prf_config.h"
#include "db_handle.h"
#include "rble_api.h"
#ifdef USE_FW_UPDATE_PROFILE
#include "rble_api_fwup.h"
#endif
/*********************************
 * GAP service                   *
 *********************************/

/*-----------------------------------------------------------------------------*
 * Device Serach(Discovery procedure) Parameters 
 *-----------------------------------------------------------------------------*/
/*
 * Minimum time to perform scanning when performing the Discovery procedure
 * recommended value : 10.24s: (1024*10ms)
 */
_HDB_DATA uint16_t gap_discovery_scan_time;
/*
 * Scan interval/window for device search (performing Limited/General Discovery procedure)
 */
_HDB_DATA uint16_t gap_dev_search_scan_intv;
_HDB_DATA uint16_t gap_dev_search_scan_window;


/*-----------------------------------------------------------------------------*
 * limited discoverable mode timer TGAP(lim_adv_timeout)
 *-----------------------------------------------------------------------------*/
/*
 * Maximum time to remain advertising when in the limited discoverable mode
 * Required value : 180sec
 */
_HDB_DATA uint16_t gap_lim_adv_timeout;

/*-----------------------------------------------------------------------------*
 * Connection Parameters for Auto/Selective connection establishment procedure
 *-----------------------------------------------------------------------------*/
/*
 * Scan interval in any discovery or connection establishment
 * procedure when user initiated: TGAP(scan_fast_interval)
 * recommended value: 30 to 60 ms; N * 0.625
 */
_HDB_DATA uint16_t gap_scan_fast_intv;
/*
 * Scan window in any discovery or connection establishment
 * procedure when user initiated: TGAP(scan_fast_window)
 * recommended value: 30 ms; N * 0.625
 */
_HDB_DATA uint16_t gap_scan_fast_window;
/*
 * Minimum to maximum connection interval upon any connection
 * establishment: TGAP(initial_conn_interval)
 * recommended value: 30 to 50 ms ; N * 1.25 ms
 */
_HDB_DATA uint16_t gap_init_conn_min_intv;
_HDB_DATA uint16_t gap_init_conn_max_intv;
/* 
 * Minimum/Maximum connection event length
 */
_HDB_DATA uint16_t gap_conn_min_ce_length;
_HDB_DATA uint16_t gap_conn_max_ce_length;
/* 
 * slave latency
 */
_HDB_DATA uint16_t gap_conn_slave_latency;
/* 
 * supervision timeout
 * recommended value: 20s
 */
_HDB_DATA uint16_t gap_dev_supervision_timeout;


/*-----------------------------------------------------------------------------*
 * Minimum time interval between private address change TGAP(lim_adv_timeout)
 *-----------------------------------------------------------------------------*/
/*
 * Recommended value : 15min
 * 0x3A98 = 150sec(15000 * 10ms) = 2.5min
 */
_HDB_DATA uint16_t gap_resolvable_private_addr_intv;


/** GAP Service */
_HDB_CNST static const uint16_t gap_svc = RBLE_SVC_GENERIC_ACCESS;

/** Device Name Characteristic */
_HDB_CNST static const struct atts_char_desc devname = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR,
                                                GAP_HDL_DEVNAME,
                                                RBLE_CHAR_GAP_DEVICE_NAME);
/** Device Name Characteristic Value */
_HDB_DATA struct device_name gap_dev_name;


/** Appearance or Icon Characteristic */
_HDB_CNST static const struct atts_char_desc icon = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD | RBLE_GATT_CHAR_PROP_WR,
                                             GAP_HDL_ICON,
                                             RBLE_CHAR_GAP_APPEARANCE);

/** Appearance or Icon value */
_HDB_DATA uint16_t gap_appearance_char_val;

#if (BLE_PERIPHERAL)

/** Slave Parameters Characteristic */
_HDB_CNST static const struct atts_char_desc slave_param_ch = ATTS_CHAR(RBLE_GATT_CHAR_PROP_RD,
                                                       GAP_HDL_SLAVE_PREF_PARAM,
                                                       RBLE_CHAR_GAP_PH_PREF_CONN_PARAM);

/** Slave Preferred Parameter value */
_HDB_CNST static const struct atts_slv_pref slave_param = {
    { (uint8_t)(GAP_PPCP_CONN_INTV_MAX), (uint8_t)(GAP_PPCP_CONN_INTV_MAX >> 8) },
    { (uint8_t)(GAP_PPCP_CONN_INTV_MIN), (uint8_t)(GAP_PPCP_CONN_INTV_MIN >> 8) },
    { (uint8_t)(GAP_PPCP_SLAVE_LATENCY), (uint8_t)(GAP_PPCP_SLAVE_LATENCY >> 8) },
    { (uint8_t)(GAP_PPCP_STO_MULT),      (uint8_t)(GAP_PPCP_STO_MULT >> 8) }
};

#endif /* #if (BLE_PERIPHERAL) */

/*********************************
 * GATT service                  *
 *********************************/
/** GATT Primary Service */
_HDB_CNST static const uint16_t gatt_svc = RBLE_SVC_GENERIC_ATTRIBUTE;


/** Service Changed Characteristic */
_HDB_CNST static const struct atts_char_desc gatt_svc_changed = ATTS_CHAR(RBLE_GATT_CHAR_PROP_IND,
                                                         GATT_HDL_SVC_CHANGED,
                                                         RBLE_CHAR_GATT_SERVICE_CHANGED);
/** Service Changed */
_HDB_DATA uint8_t gatt_service_changed[4];

/** Service Changed indication configuration */
_HDB_DATA uint16_t gatt_service_changed_ind_en;

#ifdef USE_FW_UPDATE_PROFILE

_HDB_CNST static const uint8_t fw_update_svc[] = RBLE_SVC_FW_UPDATE;

_HDB_CNST static const struct atts_char128_desc fwus_data_cntl_char = { RBLE_GATT_CHAR_PROP_WR,
                                                              {(uint8_t)(FWUS_HDL_DATA_CNTL_VAL & 0xff), (uint8_t)((FWUS_HDL_DATA_CNTL_VAL >> 8) & 0xFF)},
                                                              RBLE_CHAR_FWUP_DATA_CTRL};

_HDB_CNST static const struct atts_char128_desc fwus_data_char = { RBLE_GATT_CHAR_PROP_WR_NO_RESP,
                                                         {(uint8_t)(FWUS_HDL_DATA_VAL & 0xff), (uint8_t)((FWUS_HDL_DATA_VAL >> 8) & 0xFF)},
                                                         RBLE_CHAR_FWUP_DATA};


_HDB_DATA uint8_t fwus_data_cntl_val[4];
_HDB_DATA uint8_t fwus_data_val[20];

_HDB_CNST const struct atts_elmt_128 fwus_data_cntl_val_elmt    = { RBLE_CHAR_FWUP_DATA_CTRL,
                                                                    RBLE_GATT_128BIT_UUID_OCTET,
                                                                    &fwus_data_cntl_val[0]};

_HDB_CNST const struct atts_elmt_128 fwus_data_val_elmt = { RBLE_CHAR_FWUP_DATA,
                                                            RBLE_GATT_128BIT_UUID_OCTET,
                                                            &fwus_data_val[0]};
#endif /* #ifdef USE_FW_UPDATE_PROFILE */

_HDB_DATA struct atts_desc *atts_desc_list;

_HDB_DATA uint16_t ATT_LAST_IDX;

/** Attribute Database */
_HDB_CNST const struct atts_desc atts_desc_list_host[] =
{
    /* Reserved */
    {0,0,0,0,0,0},

    /*********************************
     * GAP service                   *
     *********************************/
    { RBLE_DECL_PRIMARY_SERVICE,
        sizeof(gap_svc),            sizeof(gap_svc),        TASK_ATTID(TASK_GAP, GAP_IDX_PRIM_SVC),             RBLE_GATT_PERM_RD,                      (void *)&gap_svc },
    /* device name declaration */
    { RBLE_DECL_CHARACTERISTIC,
        sizeof(devname),            sizeof(devname),        TASK_ATTID(TASK_GAP, GAP_IDX_CHAR_DEVNAME),         RBLE_GATT_PERM_RD,                      (void *)&devname },
    /* device name definition */
    { RBLE_CHAR_GAP_DEVICE_NAME,
        RBLE_BD_NAME_SIZE,          0,                      TASK_ATTID(TASK_GAP, GAP_IDX_DEVNAME),              (RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR_UNAUTH),   (void *)gap_dev_name.name },
    /* appearance declaration */
    { RBLE_DECL_CHARACTERISTIC,
        sizeof(icon),               sizeof(icon),           TASK_ATTID(TASK_GAP, GAP_IDX_CHAR_ICON),            RBLE_GATT_PERM_RD,                      (void *)&icon },
    /* appearance */
    { RBLE_CHAR_GAP_APPEARANCE,
        sizeof(gap_appearance_char_val),sizeof(gap_appearance_char_val),TASK_ATTID(TASK_GAP, GAP_IDX_ICON),     (RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR_UNAUTH),   (void *)&gap_appearance_char_val },
    #if (BLE_PERIPHERAL)
    /* Peripheral parameters declaration */
    { RBLE_DECL_CHARACTERISTIC,
        sizeof(slave_param_ch),     sizeof(slave_param_ch), TASK_ATTID(TASK_GAP, GAP_IDX_CHAR_SLAVE_PREF_PARAM),RBLE_GATT_PERM_RD,                      (void *)&slave_param_ch },
    /* Peripheral parameters definition */
    { RBLE_CHAR_GAP_PH_PREF_CONN_PARAM,
        sizeof(slave_param),        sizeof(slave_param),    TASK_ATTID(TASK_GAP, GAP_IDX_SLAVE_PREF_PARAM),     RBLE_GATT_PERM_RD,                      (void *)&slave_param },
    #endif /* #if (BLE_PERIPHERAL)*/

    /*********************************
     * GATT service                  *
     *********************************/
    { RBLE_DECL_PRIMARY_SERVICE,
        sizeof(gatt_svc),           sizeof(gatt_svc),               TASK_ATTID(TASK_GATT, GATT_IDX_PRIM_SVC),           RBLE_GATT_PERM_HIDE|RBLE_GATT_PERM_RD,                      (void *)&gatt_svc },
    /* Service Changed declaration */
    { RBLE_DECL_CHARACTERISTIC,
        sizeof(gatt_svc_changed),   sizeof(gatt_svc_changed),       TASK_ATTID(TASK_GATT, GATT_IDX_CHAR_SVC_CHANGED),   RBLE_GATT_PERM_HIDE|RBLE_GATT_PERM_RD,                      (void *)&gatt_svc_changed },
    /* Service Changed definition */
    { RBLE_CHAR_GATT_SERVICE_CHANGED,
        sizeof(gatt_service_changed),sizeof(gatt_service_changed),  TASK_ATTID(TASK_GATT, GATT_IDX_SVC_CHANGED),        RBLE_GATT_PERM_HIDE|(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_NI),  (void *)&gatt_service_changed[0] },
    /* Service Changed Indication Cfg Value */
    { RBLE_DESC_CLIENT_CHAR_CONF,
        sizeof(gatt_service_changed_ind_en), sizeof(gatt_service_changed_ind_en),
                                                                    TASK_ATTID(TASK_GATT, GATT_IDX_SVC_CHANGED_IND_CFG),RBLE_GATT_PERM_HIDE|(RBLE_GATT_PERM_RD|RBLE_GATT_PERM_WR),  (void *)&gatt_service_changed_ind_en },
    #ifdef USE_FW_UPDATE_PROFILE
    /*********************************
     * FW update Service             *
     *********************************/
    { RBLE_DECL_PRIMARY_SERVICE,
        sizeof(fw_update_svc),              sizeof(fw_update_svc),          TASK_ATTID(TASK_RBLE, FWUS_IDX_SVC),                RBLE_GATT_PERM_RD,              (void *)&fw_update_svc },
    /* FW update data control Char */
    { RBLE_DECL_CHARACTERISTIC,
        sizeof(fwus_data_cntl_char),        sizeof(fwus_data_cntl_char),    TASK_ATTID(TASK_RBLE, FWUS_IDX_DATA_CNTL_CHAR),     RBLE_GATT_PERM_RD,              (void *)&fwus_data_cntl_char },
    /* FW update data control Value */
    { DB_TYPE_128BIT_UUID,
        sizeof(fwus_data_cntl_val),         sizeof(fwus_data_cntl_val),     TASK_ATTID(TASK_RBLE, FWUS_IDX_DATA_CNTL_VAL),      RBLE_GATT_PERM_WR,              (void *)&fwus_data_cntl_val_elmt },
    /* FW update data Char */
    { RBLE_DECL_CHARACTERISTIC,
        sizeof(fwus_data_char),             sizeof(fwus_data_char),         TASK_ATTID(TASK_RBLE, FWUS_IDX_DATA_CHAR),          RBLE_GATT_PERM_RD,              (void *)&fwus_data_char },
    /* FW update data Value */
    { DB_TYPE_128BIT_UUID,
        sizeof(fwus_data_val),              sizeof(fwus_data_val),          TASK_ATTID(TASK_RBLE, FWUS_IDX_DATA_VAL),           RBLE_GATT_PERM_WR,              (void *)&fwus_data_val_elmt },
    #endif /* #ifdef USE_FW_UPDATE_PROFILE */
};

_HDB_CNST const uint16_t ATT_HOST_DB_SIZE = sizeof(atts_desc_list_host);

