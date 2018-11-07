/**
 ****************************************************************************************
 *
 * @file        prf_sel.h
 *
 * @brief   Setting of Profile Selection
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 9242 $
 *
 ****************************************************************************************
 */

#ifndef _PRF_SEL_H_
#define _PRF_SEL_H_

/**
 * INCLUDE FILES
 ****************************************************************************************
 */

/**
 * TYPE DEFINES
 ****************************************************************************************
 */

/**
 * DEFINES
 ****************************************************************************************
 */
#ifdef USE_SAMPLE_PROFILE
#define USE_CUSTOM_DEMO
#endif

/* for Profile Selection (Set to 0 if the profile is not selected.) */
#ifndef USE_SAMPLE_PROFILE
#define PRF_SEL_PXPM    1   /* Proximity Profile Monitor role */
#define PRF_SEL_PXPR    1   /* Proximity Profile Reporter role */
#define PRF_SEL_FMPL    1   /* Find Me Profile Locator role */
#define PRF_SEL_FMPT    1   /* Find Me Profile Target role */
#define PRF_SEL_HTPC    0   /* Health Thermometer Profile Collector role */
#define PRF_SEL_HTPT    0   /* Health Thermometer Profile Thermometer role */
#define PRF_SEL_BLPC    0   /* Blood Pressure Profile Collector role */
#define PRF_SEL_BLPS    0   /* Blood Pressure Profile Sensor role */
#define PRF_SEL_HGHD    0   /* HID over GATT Profile HID Device role */
#define PRF_SEL_HGBH    0   /* HID over GATT Profile Boot Host role */
#define PRF_SEL_HGRH    0   /* HID over GATT Profile Report Host role */
#define PRF_SEL_SPPC    0   /* Scan Parameters Profile Scan Client role */
#define PRF_SEL_SPPS    0   /* Scan Parameters Profile Scan Server role */
#define PRF_SEL_HRPC    0   /* Heart Rate Profile Collector role */
#define PRF_SEL_HRPS    0   /* Heart Rate Profile Sensor role */
#define PRF_SEL_CSCC    0   /* Cycling Speed and Cadence Profile Collector role */
#define PRF_SEL_CSCS    0   /* Cycling Speed and Cadence Profile Sensor role */
#define PRF_SEL_GLPC    0   /* Glucose Profile Collector role */
#define PRF_SEL_GLPS    0   /* Glucose Profile Sensor role */
#define PRF_SEL_CPPC    0   /* Cycling Power Profile Collector role */
#define PRF_SEL_CPPS    0   /* Cycling Power Profile Sensor role */
#define PRF_SEL_TIPC    0   /* Time Profile Client role */
#define PRF_SEL_TIPS    0   /* Time Profile Server role */
#define PRF_SEL_ANPC    1   /* Alert Notification Profile Client role */
#define PRF_SEL_ANPS    1   /* Alert Notification Profile Server role */
#define PRF_SEL_LNPS    0   /* Location and Navigation Profile Sensor role */
#define PRF_SEL_LNPC    0   /* Location and Navigation Profile Collector role */
#define PRF_SEL_PASC    0   /* Phone Alert Status Profile Client role */
#define PRF_SEL_PASS    0   /* Phone Alert Status Profile Server role */
#define PRF_SEL_RSCC    0   /* Running Speed and Cadence Profile Collector role */
#define PRF_SEL_RSCS    0   /* Running Speed and Cadence Profile Sensor role */
#else
#define PRF_SEL_PXPM    0   /* Proximity Profile Monitor role */
#define PRF_SEL_PXPR    0   /* Proximity Profile Reporter role */
#define PRF_SEL_FMPL    0   /* Find Me Profile Locator role */
#define PRF_SEL_FMPT    0   /* Find Me Profile Target role */
#define PRF_SEL_HTPC    0   /* Health Thermometer Profile Collector role */
#define PRF_SEL_HTPT    0   /* Health Thermometer Profile Thermometer role */
#define PRF_SEL_BLPC    0   /* Blood Pressure Profile Collector role */
#define PRF_SEL_BLPS    0   /* Blood Pressure Profile Sensor role */
#define PRF_SEL_HGHD    0   /* HID over GATT Profile HID Device role */
#define PRF_SEL_HGBH    0   /* HID over GATT Profile Boot Host role */
#define PRF_SEL_HGRH    0   /* HID over GATT Profile Report Host role */
#define PRF_SEL_SPPC    0   /* Scan Parameters Profile Scan Client role */
#define PRF_SEL_SPPS    0   /* Scan Parameters Profile Scan Server role */
#define PRF_SEL_HRPC    0   /* Heart Rate Profile Collector role */
#define PRF_SEL_HRPS    0   /* Heart Rate Profile Sensor role */
#define PRF_SEL_CSCC    0   /* Cycling Speed and Cadence Profile Collector role */
#define PRF_SEL_CSCS    0   /* Cycling Speed and Cadence Profile Sensor role */
#define PRF_SEL_GLPC    0   /* Glucose Profile Collector role */
#define PRF_SEL_GLPS    0   /* Glucose Profile Sensor role */
#define PRF_SEL_CPPC    0   /* Cycling Power Profile Collector role */
#define PRF_SEL_CPPS    0   /* Cycling Power Profile Sensor role */
#define PRF_SEL_TIPC    0   /* Time Profile Client role */
#define PRF_SEL_TIPS    0   /* Time Profile Server role */
#define PRF_SEL_ANPC    0   /* Alert Notification Profile Client role */
#define PRF_SEL_ANPS    0   /* Alert Notification Profile Server role */
#define PRF_SEL_LNPC    0   /* Location and Navigation Profile Collector role */
#define PRF_SEL_LNPS    0   /* Location and Navigation Profile Sensor role */
#define PRF_SEL_PASC    0   /* Phone Alert Status Profile Client role */
#define PRF_SEL_PASS    0   /* Phone Alert Status Profile Server role */
#define PRF_SEL_RSCC    0   /* Running Speed and Cadence Profile Collector role */
#define PRF_SEL_RSCS    0   /* Running Speed and Cadence Profile Sensor role */
#endif

#if PRF_SEL_HGHD
#define HID_MULTI_INSTANCE
#endif

/* Flag indicating if use Link Loss Service */
#if (PRF_SEL_PXPR)
    #define USE_LINK_LOSS_SERVICE           1
#else
    /* If use LLS without PXP, set to 1. */
    #define USE_LINK_LOSS_SERVICE           0
#endif

/* Flag indicating if use Tx Power Service */
#if (PRF_SEL_PXPR)
    #define USE_TX_POWER_SERVICE            1
#else
    /* If use TPS without PXP, set to 1. */
    #define USE_TX_POWER_SERVICE            0
#endif

/* Flag indicating if use Immediate Alert Service */
#if (PRF_SEL_PXPR || PRF_SEL_FMPT)
    #define USE_IMMEDIATE_ALERT_SERVICE     1
#else
    /* If use IAS without PXP and FMP, set to 1. */
    #define USE_IMMEDIATE_ALERT_SERVICE     0
#endif

/* Flag indicating if use Device Information Service */
#if (PRF_SEL_HTPT || PRF_SEL_BLPS || PRF_SEL_HRPS || PRF_SEL_CSCS || PRF_SEL_GLPS || PRF_SEL_CPPS || PRF_SEL_LNPS || PRF_SEL_RSCS)
    #define USE_DEV_INFO_SERVICE            1
#else
    /* If use DIS without HTP, BLP and so on, set to 1. */
    #define USE_DEV_INFO_SERVICE            0
#endif

/* Flag indicating if use Device Information Service (PnP characteristic) */
#if (PRF_SEL_HGHD)
    #define USE_DEV_INFO_SERVICE_PNP        1
#else
    /* If use DIS(PnP) without HOGP, set to 1. */
    #define USE_DEV_INFO_SERVICE_PNP        0
#endif

/* Flag indicating if use Battery Service */
#if (PRF_SEL_HGHD || PRF_SEL_CPPS || PRF_SEL_LNPS)
    #define USE_BATTERY_SERVICE             1
#else
    /* If use BAS without HOGP, CPP, LNP, set to 1. */
    #define USE_BATTERY_SERVICE             0
#endif

/* Flag indicating if use Scan Parameters Service */
#if (PRF_SEL_SPPS)
    #define USE_SCAN_PARAMETER_SERVICE      1
#else
    /* If use ScPS without ScPP, set to 1. */
    #define USE_SCAN_PARAMETER_SERVICE      0
#endif



#if (PRF_SEL_PXPM)
    #define PXPM_TABLE_SIZE     5
#else
    #define PXPM_TABLE_SIZE     0
#endif
#if (PRF_SEL_PXPR)
    #define PXPR_TABLE_SIZE     3
#else
    #define PXPR_TABLE_SIZE     0
#endif
#if (PRF_SEL_FMPL)
    #define FMPL_TABLE_SIZE     3
#else
    #define FMPL_TABLE_SIZE     0
#endif
#if (PRF_SEL_FMPT)
    #define FMPT_TABLE_SIZE     3
#else
    #define FMPT_TABLE_SIZE     0
#endif
#if (PRF_SEL_HTPC)
    #define HTPC_TABLE_SIZE     7
#else
    #define HTPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_HTPT)
    #define HTPT_TABLE_SIZE     7
#else
    #define HTPT_TABLE_SIZE     0
#endif
#if (PRF_SEL_BLPC)
    #define BLPC_TABLE_SIZE     6
#else
    #define BLPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_BLPS)
    #define BLPS_TABLE_SIZE     5
#else
    #define BLPS_TABLE_SIZE     0
#endif
#if (PRF_SEL_HGHD)
    #define HGHD_TABLE_SIZE     10
#else
    #define HGHD_TABLE_SIZE     0
#endif
#if (PRF_SEL_HGBH)
    #define HGBH_TABLE_SIZE     6
#else
    #define HGBH_TABLE_SIZE     0
#endif
#if (PRF_SEL_HGRH)
    #define HGRH_TABLE_SIZE     8
#else
    #define HGRH_TABLE_SIZE     0
#endif
#if (PRF_SEL_SPPS)
    #define SPPS_TABLE_SIZE     5
#else
    #define SPPS_TABLE_SIZE     0
#endif
#if (PRF_SEL_SPPC)
    #define SPPC_TABLE_SIZE     4
#else
    #define SPPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_HRPC)
    #define HRPC_TABLE_SIZE     6
#else
    #define HRPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_HRPS)
    #define HRPS_TABLE_SIZE     6
#else
    #define HRPS_TABLE_SIZE     0
#endif
#if (PRF_SEL_CSCC)
    #define CSCC_TABLE_SIZE     7
#else
    #define CSCC_TABLE_SIZE     0
#endif
#if (PRF_SEL_CSCS)
    #define CSCS_TABLE_SIZE     7
#else
    #define CSCS_TABLE_SIZE     0
#endif
#if (PRF_SEL_GLPC)
    #define GLPC_TABLE_SIZE     8
#else
    #define GLPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_GLPS)
    #define GLPS_TABLE_SIZE     8
#else
    #define GLPS_TABLE_SIZE     0
#endif
#if (PRF_SEL_CPPS)
    #define CPPS_TABLE_SIZE     10
#else
    #define CPPS_TABLE_SIZE     0
#endif
#if (PRF_SEL_CPPC)
    #define CPPC_TABLE_SIZE     9
#else
    #define CPPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_TIPS)
    #define TIPS_TABLE_SIZE     7
#else
    #define TIPS_TABLE_SIZE     0
#endif
#if (PRF_SEL_TIPC)
    #define TIPC_TABLE_SIZE     6
#else
    #define TIPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_ANPS)
    #define ANPS_TABLE_SIZE     7
#else
    #define ANPS_TABLE_SIZE     0
#endif
#if (PRF_SEL_ANPC)
    #define ANPC_TABLE_SIZE     7
#else
    #define ANPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_LNPS)
    #define LNPS_TABLE_SIZE     10
#else
    #define LNPS_TABLE_SIZE     0
#endif
#if (PRF_SEL_LNPC)
    #define LNPC_TABLE_SIZE     9
#else
    #define LNPC_TABLE_SIZE     0
#endif
#if (PRF_SEL_PASS)
    #define PASS_TABLE_SIZE     7
#else
    #define PASS_TABLE_SIZE     0
#endif
#if (PRF_SEL_PASC)
    #define PASC_TABLE_SIZE     7
#else
    #define PASC_TABLE_SIZE     0
#endif
#if (PRF_SEL_RSCS)
    #define RSCS_TABLE_SIZE     7
#else
    #define RSCS_TABLE_SIZE     0
#endif
#if (PRF_SEL_RSCC)
    #define RSCC_TABLE_SIZE     7
#else
    #define RSCC_TABLE_SIZE     0
#endif

#define HOST_TABLE_SIZE         88

#define GATT_DB_STRUCT_SIZE     6

#define RBLE_TABLE_SIZE  ((PXPM_TABLE_SIZE + PXPR_TABLE_SIZE + FMPL_TABLE_SIZE + FMPT_TABLE_SIZE \
                         + HTPC_TABLE_SIZE + HTPT_TABLE_SIZE + BLPC_TABLE_SIZE + BLPS_TABLE_SIZE \
                         + HGHD_TABLE_SIZE + HGBH_TABLE_SIZE + HGRH_TABLE_SIZE                   \
                         + SPPS_TABLE_SIZE + SPPC_TABLE_SIZE + HRPC_TABLE_SIZE + HRPS_TABLE_SIZE \
                         + CSCC_TABLE_SIZE + CSCS_TABLE_SIZE + GLPC_TABLE_SIZE + GLPS_TABLE_SIZE \
                         + CPPS_TABLE_SIZE + CPPC_TABLE_SIZE + TIPS_TABLE_SIZE + TIPC_TABLE_SIZE \
                         + ANPS_TABLE_SIZE + ANPC_TABLE_SIZE + LNPS_TABLE_SIZE + LNPC_TABLE_SIZE \
                         + PASS_TABLE_SIZE + PASC_TABLE_SIZE + RSCS_TABLE_SIZE + RSCC_TABLE_SIZE \
                         + HOST_TABLE_SIZE) * GATT_DB_STRUCT_SIZE)


/**
 * STRUCTURE DECLARATIONS
 ****************************************************************************************
 */

/**
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

#endif /* _PRF_SEL_H_ */
