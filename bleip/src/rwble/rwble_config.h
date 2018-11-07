/**
 ****************************************************************************************
 *
 * @file		rwble_config.h
 *
 * @brief Configuration of the BLE protocol stack (max number of supported connections,
 * type of partitioning, etc.)
 *
 * Copyright(C) RivieraWaves 2009-2012
 *
 * $Rev: 13510 $
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef RWBLE_CONFIG_H_
#define RWBLE_CONFIG_H_

#if 1
#define	CFG_EMB			1
#define	CFG_CENTRAL		1
#define	CFG_ALLROLES	1
#define	CFG_DBG			1
#define CFG_PRF_PXPM    1   /* Proximity Profile Monitor role */
#define CFG_PRF_PXPR    1   /* Proximity Profile Reporter role */
#define CFG_PRF_FMPL    1   /* Find Me Profile Locator role */
#define CFG_PRF_FMPT    1   /* Find Me Profile Target role */
#define CFG_PRF_HTPC    1   /* Health Thermometer Profile Collector role */
#define CFG_PRF_HTPT    1   /* Health Thermometer Profile Thermometer role */
#define CFG_PRF_BLPC    1   /* Blood Pressure Profile Collector role */
#define CFG_PRF_BLPS    1   /* Blood Pressure Profile Sensor role */
#define CFG_PRF_HGHD    1   /* HID over GATT Profile HID Device role */
#define CFG_PRF_HGBH    1   /* HID over GATT Profile Boot Host role */
#define CFG_PRF_HGRH    1   /* HID over GATT Profile Report Host role */
#define CFG_PRF_SCANS   1   /* Scan Parameters Profile Scan Server role */
#define CFG_PRF_SCANC   1   /* Scan Parameters Profile Scan Client role */
#define CFG_PRF_HRPC    1   /* Heart Rate Profile Collector role */
#define CFG_PRF_HRPS    1   /* Heart Rate Profile Sensor role */
#define CFG_PRF_CSCPC   1   /* Cycling Speed and Cadence Profile Collector role */
#define CFG_PRF_CSCPS   1   /* Cycling Speed and Cadence Profile Sensor role */
#define CFG_PRF_GLPC    1   /* Glucose Profile Collector role */
#define CFG_PRF_GLPS    1   /* Glucose Profile Sensor role */
#define CFG_PRF_CPPC    1   /* Cycling Power Profile Collector role */
#define CFG_PRF_CPPS    1   /* Cycling Power Profile Sensor role */
#define CFG_PRF_TIPC    1   /* Time Profile Client role */
#define CFG_PRF_TIPS    1   /* Time Profile Server role */
#define CFG_PRF_ANPC    1   /* Alert Notification Profile Client role */
#define CFG_PRF_ANPS    1   /* Alert Notification Profile Server role */
#define CFG_PRF_LNPC    1   /* Location and Navigation Profile Collector role */
#define CFG_PRF_LNPS    1   /* Location and Navigation Profile Sensor role */
#define CFG_PRF_PASPC   1   /* Phone Alert Status Profile Client role */
#define CFG_PRF_PASPS   1   /* Phone Alert Status Profile Server role */
#define CFG_PRF_RSCPC   1   /* Running Speed and Cadence Profile Collector role */
#define CFG_PRF_RSCPS   1   /* Running Speed and Cadence Profile Sensor role */
#endif

/**
 ****************************************************************************************
 * @addtogroup ROOT
 * @{
 * @name BLE stack configuration
 * @{
 ****************************************************************************************
 */
/// Flag indicating if stack is compiled in dual mode
#if defined (CFG_BT)
    #define RW_BLE_SUPPORT              0
#else
    #define RW_BLE_SUPPORT              1
#endif // defined (CFG_BLE)

#ifndef RW_DM_SUPPORT
    #define RW_DM_SUPPORT               !RW_BLE_SUPPORT
#endif // RW_DM_SUPPORT

/// Flag indicating if stack is compiled for BLE1.0 HW or later
#if defined (CFG_BLECORE_10)
    #define BLE10_HW                    1
#else
    #define BLE10_HW                    0
#endif // defined (CFG_BLECORE_10)


/******************************************************************************************/
/* -------------------------   DEEP SLEEP SETUP      -------------------------------------*/
/******************************************************************************************/

/// DEEP SLEEP enable
#if defined (CFG_SLEEP)
    #define DEEP_SLEEP                              1
#else
    #define DEEP_SLEEP                              0
#endif /* CFG_SLEEP */

/******************************************************************************************/
/* --------------------------   DEBUG SETUP       ----------------------------------------*/
/******************************************************************************************/

/// Flag indicating if debug mode is activated or not
#if defined (CFG_DBG)
    #define BLE_DEBUG                   1
//    #define KE_PROFILING                1
    #define KE_PROFILING                0
#else //BLE_DEBUG
    #define BLE_DEBUG                   0
    #define KE_PROFILING                0
#endif //BLE_DEBUG

/// Flag indicating if Read/Write memory commands are supported or not
#if defined (CFG_DBG_MEM)
    #define BLE_DEBUG_MEM               1
#else //CFG_DBG_MEM
    #define BLE_DEBUG_MEM               0
#endif //CFG_DBG_MEM

/// Flag indicating if CPU stack profiling commands are supported or not
#if defined (CFG_DBG_STACK_PROF)
    #define BLE_DEBUG_STACK_PROF        1
#else
    #define BLE_DEBUG_STACK_PROF        0
#endif // defined (CFG_DBG_STACK_PROF)

/// Partitioning definitions
#if defined(CFG_FULLEMB)
    #define BLE_FULLEMB                 1
    #define BLE_SPLIT                   0
    #define BLE_FULLHOST                0
    #ifdef CFG_EMB
        #define BLE_HOST_PRESENT        1
        #define BLE_EMB_PRESENT         1
        #define BLE_APP_PRESENT         0
    #else
        #define BLE_HOST_PRESENT        0
        #define BLE_EMB_PRESENT         0
        #define BLE_APP_PRESENT         1
    #endif // CFG_EMB
#elif defined(CFG_SPLIT)
    #define BLE_FULLEMB                 0
    #define BLE_FULLHOST                0
    #define BLE_SPLIT                   1
    #ifdef CFG_EMB
        #define BLE_HOST_PRESENT        0
        #define BLE_EMB_PRESENT         1
        #define BLE_APP_PRESENT         0
    #else
        #define BLE_HOST_PRESENT        1
        #define BLE_EMB_PRESENT         0
        #define BLE_APP_PRESENT         1
    #endif // CFG_EMB
#elif defined(CFG_FULLHOST)
    #define BLE_FULLEMB                 0
    #define BLE_FULLHOST                1
    #define BLE_SPLIT                   0
    #define BLE_HOST_PRESENT            1
    #define BLE_EMB_PRESENT             1
    #define BLE_APP_PRESENT             1
#else
    #error "No partitioning defined"
#endif // CFG_FULLHOST, CFG_SPLIT, CFG_FULLEMB

/// Exchange memory presence
#if (BLE_EMB_PRESENT)
    #if defined(CFG_EXMEM_NOT_PRESENT)
        #define BLE_EM_PRESENT          0
    #else
        #define BLE_EM_PRESENT          1
    #endif // CFG_EXMEM_NOT_PRESENT
#else
    #define BLE_EM_PRESENT              0
#endif // BLE_EMB_PRESENT

/// Application role definitions
#define BLE_BROADCASTER   (defined(CFG_BROADCASTER) || defined(CFG_PERIPHERAL) || defined(CFG_ALLROLES))
#define BLE_OBSERVER      (defined(CFG_OBSERVER) || defined(CFG_CENTRAL) || defined(CFG_ALLROLES))
#define BLE_PERIPHERAL    (defined(CFG_PERIPHERAL) || defined(CFG_ALLROLES))
#define BLE_CENTRAL       (defined(CFG_CENTRAL) || defined(CFG_ALLROLES))
#define RW_BLE_USE_CRYPT  (defined(CFG_SECURITY_ON))
#define RW_BLE_WLAN_COEX  (defined(CFG_BLE_WLAN_COEX))

#if (!BLE_BROADCASTER) && (!BLE_OBSERVER) && (!BLE_PERIPHERAL) && (!BLE_CENTRAL)
    #error "No application role defined"
#endif /* #if (!BLE_BROADCASTER) && (!BLE_OBSERVER) && (!BLE_PERIPHERAL) && (!BLE_CENTRAL) */

/// BLE Host presence
#if (BLE_HOST_PRESENT)
    /// RAM Optimization
    #define BLE_RAM_OPTIMIZED           1
    /// SDS Profile
    #define BLE_SDS                     0
#endif  /* #if (BLE_APP_PRESENT) */

#define BLE_TEST_MODE_SUPPORT           1

/// Number of receive buffers in the RX ring. This number defines the interrupt
/// rate during a connection event. An interrupt is asserted every BLE_RX_BUFFER_CNT/2
/// reception. This number has an impact on the size of the exchange memory. This number
/// may have to be increased when CPU is very slow to free the received data, in order not
/// to overflow the RX ring of buffers.
#if (BLE_CENTRAL || BLE_PERIPHERAL)
    #define BLE_RX_BUFFER_CNT           8
#elif (BLE_BROADCASTER)
    #define BLE_RX_BUFFER_CNT           1
#else
    #define BLE_RX_BUFFER_CNT           4
#endif // BLE_CENTRAL || BLE_PERIPHERAL

/// Number of devices capacity for the scan filtering
#define BLE_DUPLICATE_FILTER_MAX        10

/// Max advertising reports before sending the info to the host
#define BLE_ADV_REPORTS_MAX             1

/* end of the heap area */
#define BLE_HEAP_END                    1

/// LL assigned numbers company ID
#define BLE_COMP_ID                     54

/// This define is to keep the backward compatibility with the 1st
/// qualification of the BLE
#define BITE_VERS_1_0_3_0               0

/// Debug - Bluetooth UPF
#define BLE_UPF                         0

/// Debug - Modem back to back setup
#define MODEM2MODEM                     0

//ATt DB,Testing and Qualification related flags
#if (BLE_CENTRAL || BLE_PERIPHERAL)
    #if defined(CFG_QDB)
        /// Flag for Qualification mode Database enable
        #define ATT_QUALIF_MODE             1
    #else
        /// Flag for Qualification mode Database enable
        #define ATT_QUALIF_MODE             0
    #endif

    /// HL Tester behavior flags
    /// Compile flag allowing to not check Mconfirm value on SLave side
    #define BLE_Q_NOCHK_MCFM            0
    /// Compile flag allowing sending of Application Error upon service request
    #define BLE_Q_BREDR_ATT_ERR         0
    /// Compile flag allowing data corruption in Prepare Write Response
    #define BLE_Q_BAD_PREP_WR_RSP       0
    /// Compile Flag allowing to not check start and end handle validity when
    /// sending a request
    #define BLE_Q_NOCHK_HDL             0
    ///Compile flag for changing Temperature Measurement packed value length
    #define BLE_Q_TEMPM_LEN             0
    ///Compile flag for showing profile ATTs for SVC testing (default is hidden)
    #define BLE_Q_SVC_SHOW              0

    /// Proximity Profile Monitor Role
    #if defined(CFG_PRF_PXPM)
        #define BLE_PROX_MONITOR        1
    #else
        #define BLE_PROX_MONITOR        0
    #endif // defined(CFG_PRF_PXPM)

    /// Proximity Profile Reporter Role
    #if defined(CFG_PRF_PXPR)
        #define BLE_PROX_REPORTER       1
    #else
        #define BLE_PROX_REPORTER       0
    #endif // defined(CFG_PRF_PXPR)

    ///Find Me Profile Locator role
    #if defined(CFG_PRF_FMPL)
        #define BLE_FINDME_LOCATOR      1
    #else
        #define BLE_FINDME_LOCATOR      0
    #endif // defined(CFG_PRF_FMPL)

    ///Find Me Profile Target role
    #if defined(CFG_PRF_FMPT)
        #define BLE_FINDME_TARGET       1
    #else
        #define BLE_FINDME_TARGET       0
    #endif // defined(CFG_PRF_FMPT)

    ///Health Thermometer Profile Collector Role
    #if defined(CFG_PRF_HTPC)
        #define BLE_HT_COLLECTOR        1
    #else
        #define BLE_HT_COLLECTOR        0
    #endif // defined(CFG_PRF_HTPC)

    ///Health Thermometer Profile Thermometer Role
    #if defined(CFG_PRF_HTPT)
        #define BLE_HT_THERMOM          1
    #else
        #define BLE_HT_THERMOM          0
    #endif // defined(CFG_PRF_HTPT)

    ///Blood Pressure Profile Collector Role
    #if defined(CFG_PRF_BLPC)
        #define BLE_BL_COLLECTOR        1
    #else
        #define BLE_BL_COLLECTOR        0
    #endif // defined(CFG_PRF_BLPC)

    ///Blood Pressure Profile Sensor Role
    #if defined(CFG_PRF_BLPS)
        #define BLE_BL_SENSOR          1
    #else
        #define BLE_BL_SENSOR          0
    #endif // defined(CFG_PRF_BLPS)

    /// HID over GATT Profile HID Device Role
    #if defined(CFG_PRF_HGHD)
        #define BLE_HOG_HDEVICE        1
        /*** Modified as follows by RSO 20121023 ***
        *** add) HID Multi Service Definition ***/
        #define HID_MULTI_SERVICE
    #else
        #define BLE_HOG_HDEVICE        0
    #endif // defined(CFG_PRF_HGHD)

    /// HID over GATT Profile Boot Host Role
    #if defined(CFG_PRF_HGBH)
        #define BLE_HOG_BHOST          1
    #else
        #define BLE_HOG_BHOST          0
    #endif // defined(CFG_PRF_HGBH)

    /// HID over GATT Profile Report Host Role
    #if defined(CFG_PRF_HGRH)
        #define BLE_HOG_RHOST          1
    #else
        #define BLE_HOG_RHOST          0
    #endif // defined(CFG_PRF_HGRH)

    /// Scan Parameters Profile Server Role
    #if defined(CFG_PRF_SCANS)
        #define BLE_SCANPARAM_SERVER        1
    #else
        #define BLE_SCANPARAM_SERVER        0
    #endif // defined(CFG_PRF_SCANS)

    /// Scan Parameters Profile Client Role
    #if defined(CFG_PRF_SCANC)
        #define BLE_SCANPARAM_CLIENT        1
    #else
        #define BLE_SCANPARAM_CLIENT        0
    #endif // defined(CFG_PRF_SCANC)
    
    ///Heart Rate Profile Collector Role
    #if defined(CFG_PRF_HRPC)
        #define BLE_HR_COLLECTOR        1
    #else
        #define BLE_HR_COLLECTOR        0
    #endif // defined(CFG_PRF_HRPC)

    ///Heart Rate Profile Sensor Role
    #if defined(CFG_PRF_HRPS)
        #define BLE_HR_SENSOR          1
    #else
        #define BLE_HR_SENSOR          0
    #endif // defined(CFG_PRF_HRPS)
    
    ///Cycling Speed and Cadence Profile Collector Role
    #if defined(CFG_PRF_CSCPC)
        #define BLE_CSC_COLLECTOR        1
    #else
        #define BLE_CSC_COLLECTOR        0
    #endif // defined(CFG_PRF_CSCPC)

    ///Cycling Speed and Cadence Profile Sensor Role
    #if defined(CFG_PRF_CSCPS)
        #define BLE_CSC_SENSOR          1
    #else
        #define BLE_CSC_SENSOR          0
    #endif // defined(CFG_PRF_CSCPS)
    
    ///Glucose Profile Collector Role
    #if defined(CFG_PRF_GLPC)
        #define BLE_GL_COLLECTOR        1
    #else
        #define BLE_GL_COLLECTOR        0
    #endif // defined(CFG_PRF_GLPC)

    ///Glucose Profile Sensor Role
    #if defined(CFG_PRF_GLPS)
        #define BLE_GL_SENSOR          1
    #else
        #define BLE_GL_SENSOR          0
    #endif // defined(CFG_PRF_GLPS)

    ///Cycling Power Profile Collector Role
    #if defined(CFG_PRF_CPPC)
        #define BLE_CP_COLLECTOR        1
    #else
        #define BLE_CP_COLLECTOR        0
    #endif // defined(CFG_PRF_CPPC)

    ///Cycling Power Profile Sensor Role
    #if defined(CFG_PRF_CPPS)
        #define BLE_CP_SENSOR          1
    #else
        #define BLE_CP_SENSOR          0
    #endif // defined(CFG_PRF_CPPS)

    ///Time Profile Client Role
    #if defined(CFG_PRF_TIPC)
        #define BLE_TI_CLIENT          1
    #else
        #define BLE_TI_CLIENT          0
    #endif // defined(CFG_PRF_TIPC)

    ///Time Profile Server Role
    #if defined(CFG_PRF_TIPS)
        #define BLE_TI_SERVER          1
    #else
        #define BLE_TI_SERVER          0
    #endif // defined(CFG_PRF_TIPS)

    ///Alert Notification Profile Client Role
    #if defined(CFG_PRF_ANPC)
        #define BLE_AN_CLIENT          1
    #else
        #define BLE_AN_CLIENT          0
    #endif // defined(CFG_PRF_ANPC)

    ///Alert Notification Profile Server Role
    #if defined(CFG_PRF_ANPS)
        #define BLE_AN_SERVER          1
    #else
        #define BLE_AN_SERVER          0
    #endif // defined(CFG_PRF_ANPS)

    ///Location and Navigation Profile Collector Role
    #if defined(CFG_PRF_LNPC)
        #define BLE_LN_COLLECTOR       1
    #else
        #define BLE_LN_COLLECTOR       0
    #endif // defined(CFG_PRF_LNPC)

    ///Location and Navigation Profile Sensor Role
    #if defined(CFG_PRF_LNPS)
        #define BLE_LN_SENSOR          1
    #else
        #define BLE_LN_SENSOR          0
    #endif // defined(CFG_PRF_LNPS)
    
    ///Phone Alert Status Profile Client Role
    #if defined(CFG_PRF_PASPC)
        #define BLE_PAS_CLIENT         1
    #else
        #define BLE_PAS_CLIENT         0
    #endif // defined(CFG_PRF_ANPC)

    ///Phone Alert Status Profile Server Role
    #if defined(CFG_PRF_PASPS)
        #define BLE_PAS_SERVER         1
    #else
        #define BLE_PAS_SERVER         0
    #endif // defined(CFG_PRF_PASPS)

    ///Running Speed and Cadence Profile Collector Role
    #if defined(CFG_PRF_RSCPC)
        #define BLE_RSC_COLLECTOR      1
    #else
        #define BLE_RSC_COLLECTOR      0
    #endif // defined(CFG_PRF_RSCPC)

    ///Running Speed and Cadence Profile Sensor Role
    #if defined(CFG_PRF_RSCPS)
        #define BLE_RSC_SENSOR         1
    #else
        #define BLE_RSC_SENSOR         0
    #endif // defined(CFG_PRF_RSCPS)
    
    /// BLE_CLIENT_PRF indicates if at least one client profile is present
    #if BLE_PROX_MONITOR || BLE_FINDME_LOCATOR || BLE_HT_COLLECTOR || BLE_BL_COLLECTOR || BLE_HOG_BHOST || BLE_HOG_RHOST || BLE_SCANPARAM_CLIENT || BLE_HR_COLLECTOR || BLE_CSC_COLLECTOR || BLE_GL_COLLECTOR || BLE_CP_COLLECTOR || BLE_AN_CLIENT || BLE_LN_COLLECTOR || BLE_PAS_CLIENT || BLE_RSC_COLLECTOR
        #define BLE_CLIENT_PRF          1 
    #else
        #define BLE_CLIENT_PRF          0 
    #endif

    /// BLE_SERVER_PRF indicates if at least one client profile is present
    #if BLE_PROX_REPORTER || BLE_FINDME_TARGET || BLE_HT_THERMOM || BLE_BL_SENSOR || BLE_HOG_HDEVICE || BLE_SCANPARAM_SERVER || BLE_HR_SENSOR || BLE_CSC_SENSOR || BLE_GL_SENSOR || BLE_CP_SENSOR || BLE_AN_SERVER || BLE_LN_SENSOR || BLE_PAS_SERVER || BLE_RSC_SENSOR
        #define BLE_SERVER_PRF          1
    #else
        #define BLE_SERVER_PRF          0
    #endif

    //Force ATT parts depending on profile roles or compile options
    /// Attribute Client
    #if (BLE_CLIENT_PRF || defined(CFG_ATTC))
        #define BLE_ATTC                    1
    #else
        #define BLE_ATTC                    0
    #endif //(BLE_CLIENT_PRF || defined(CFG_ATTC))

    /// Attribute Server
    #if (BLE_SERVER_PRF || defined(CFG_ATTS))
        #define BLE_ATTS                    1
    #else
        #define BLE_ATTS                    0
    #endif //(BLE_SERVER_PRF || defined(CFG_ATTS))


#elif (BLE_OBSERVER || BLE_BROADCASTER)
    /// Flag for Qualification mode Database enable
    #define ATT_QUALIF_MODE             0

    /// HL Tester behavior flags
    /// Compile flag allowing to not check Mconfirm value on SLave side
    #define BLE_Q_NOCHK_MCFM            0
    /// Compile flag allowing sending of Application Error upon service request
    #define BLE_Q_BREDR_ATT_ERR         0
    /// Compile flag allowing data corruption in Prepare Write Response
    #define BLE_Q_BAD_PREP_WR_RSP       0
    /// Compile Flag allowing to not check start and end handle validity when
    /// sending a request
    #define BLE_Q_NOCHK_HDL             0
    ////Compile flag for changing Temperature Measurement packed value length
    #define BLE_Q_TEMPM_LEN             0

    /// Extra Attributes at end DB
    #define BLE_SIGNING_TEST            0

    /// Proximity Profile Monitor Role
    #define BLE_PROX_MONITOR            0
    /// Proximity Profile Reporter Role
    #define BLE_PROX_REPORTER           0
    ///Find Me Profile Locator role
    #define BLE_FINDME_LOCATOR          0
    ///Find Me Profile Target role
    #define BLE_FINDME_TARGET           0
    ///Health Thermometer Profile Collector Role
    #define BLE_HT_COLLECTOR            0
    ///Health Thermometer Profile Thermometer Role
    #define BLE_HT_THERMOM              0
    ///Blood Pressure Profile Collector Role
    #define BLE_BL_COLLECTOR            0
    ///Blood Pressure Profile Sensor Role
    #define BLE_BL_SENSOR               0
    /// HID over GATT Profile HID Device Role
    #define BLE_HOG_HDEVICE             0
    /// HID over GATT Profile Boot Host Role
    #define BLE_HOG_BHOST               0
    /// HID over GATT Profile Report Host Role
    #define BLE_HOG_RHOST               0
    /// Scan Parameters Profile Server Role
    #define BLE_SCANPARAM_SERVER        0
    /// Scan Parameters Profile Client Role
    #define BLE_SCANPARAM_CLIENT        0
    ///Heart Rate Profile Collector Role
    #define BLE_HR_COLLECTOR            0
    ///Heart Rate Profile Sensor Role
    #define BLE_HR_SENSOR               0
    ///Cycling Speed and Cadence Profile Collector Role
    #define BLE_CSC_COLLECTOR           0
    ///Cycling Speed and Cadence Profile Sensor Role
    #define BLE_CSC_SENSOR              0
    ///Glucose Profile Collector Role
    #define BLE_GL_COLLECTOR            0
    ///Glucose Profile Sensor Role
    #define BLE_GL_SENSOR               0
    ///Cycling Power Collector Role
    #define BLE_CP_COLLECTOR            0
    ///Cycling Power Sensor Role
    #define BLE_CP_SENSOR               0
    ///Time Client Role
    #define BLE_TI_CLIENT               0
    ///Time Server Role
    #define BLE_TI_SERVER               0
    ///Alert Notification Profile Client Role
    #define BLE_AN_CLIENT               0
    ///Alert Notification Profile Server Role
    #define BLE_AN_SERVER               0
    ///Location and Navigation Profile Collctor Role
    #define BLE_LN_COLLECTOR            0
    ///Location and Navigation Profile Sensor Role
    #define BLE_LN_SENSOR               0
    ///Phone Alert Profile Client Role
    #define BLE_PAS_CLIENT              0
    ///Phone Alert Profile Server Role
    #define BLE_PAS_SERVER              0
    ///Running Speed and Cadence Profile Collctor Role
    #define BLE_RSC_COLLECTOR           0
    ///Running Speed and Cadence Profile Sensor Role
    #define BLE_RSC_SENSOR              0

    //Force ATT parts to 0
    /// Attribute Client
    #define BLE_ATTC                    0
    /// Attribute Server
    #define BLE_ATTS                    0
#endif

/// Accelerometer Profile
#if (BLE_APP_PRESENT)
    #define BLE_ACCEL                   1
#else
    #define BLE_ACCEL                   0
#endif // BLE_APP_PRESENT

/// ATT Changed Index
#define ATT_CHANGED_IDX                 0

/// PTS for GATT
#define GATT_PTS                        0

/// Direct Test Mode by 2-wire UART interface
#define __DTM2WIRE_UART_USE__           0


/// @} BLE stack configuration
/// @} ROOT

#endif // RWBLE_CONFIG_H_
