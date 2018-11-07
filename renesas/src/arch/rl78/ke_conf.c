#undef	_USE_RWBLE_SOURCE
/**
 ****************************************************************************************
 *
 * @file        ke_conf.c
 *
 * @brief       This file contains the implementation of the kernel task management.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */
 
#ifdef _USE_IAR_RL78
#define _TSK_DESC _Pragma("location=\"TSK_DESC\"")
#else
#define _TSK_DESC
#if !defined(_USE_CCRL_RL78)
#pragma section @@CNST TSK_DESC
#else
#pragma section const  TSK_DESC
#endif
#endif

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "prf_sel.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */

#include "arch.h"
#include "prf_sel.h"
#include "rwble_config.h"
#include "co_list.h"

#include "ke_config.h"
#include "ke_task.h"
#include "ke_timer.h"
#include "ke_env.h"
#include "ke_queue.h"
#include "ke_event.h"
#include "ke_mem.h"
#if (BLE_SPLIT || BLE_FULLEMB)
#include "hci.h"
#endif
#if (BLE_EMB_PRESENT)
#include "lld_evt.h"
#endif

// Add here the include files for task descriptors
#if (BLE_EMB_PRESENT)
#include "llm_task.h"
#include "llc_task.h"
#include "dbg_task.h"
#endif /* #if (BLE_EMB_PRESENT) */
#if (BLE_HOST_PRESENT)
#include "l2cm_task.h"
#include "l2cc_task.h"
#include "gap_task.h"
#include "gatt_task.h"
#include "attc_task.h"
#include "attm_task.h"
#include "atts_task.h"
#include "svc_task.h"

#if (RW_BLE_USE_CRYPT)
#include "smpm_task.h"
#include "smpc_task.h"
#endif /* #if (RW_BLE_USE_CRYPT) */

#if (BLE_PROX_MONITOR)
#include "proxm_task.h"
#endif /* #if (BLE_PROX_MONITOR) */

#if (BLE_PROX_REPORTER)
#include "proxr_task.h"
#endif /* #if (BLE_PROX_REPORTER) */

#if (BLE_FINDME_LOCATOR)
#include "findl_task.h"
#endif /* #if (BLE_FINDME_LOCATOR) */

#if (BLE_FINDME_TARGET)
#include "findt_task.h"
#endif /* #if (BLE_FINDME_TARGET) */

#if (BLE_HT_COLLECTOR)
#include "htpc_task.h"
#endif /* #if (BLE_HT_COLLECTOR) */

#if (BLE_HT_THERMOM)
#include "htpt_task.h"
#endif /* #if (BLE_HT_THERMOM) */

#if (BLE_BL_COLLECTOR)
#include "blpc_task.h"
#endif /* #if (BLE_BL_COLLECTOR) */

#if (BLE_BL_SENSOR)
#include "blps_task.h"
#endif /* #if (BLE_BL_SENSOR) */

#if (BLE_HOG_HDEVICE)
#include "hghd_task.h"
#endif /* #if (BLE_HOG_HDEVICE) */

#if (BLE_HOG_BHOST)
#include "hgbh_task.h"
#endif /* #if (BLE_HOG_BHOST) */

#if (BLE_HOG_RHOST)
#include "hgrh_task.h"
#endif /* #if (BLE_HOG_RHOST) */

#if (BLE_SCANPARAM_SERVER)
#include "scans_task.h"
#endif /* #if (BLE_SCANPARAM_SERVER) */

#if (BLE_SCANPARAM_CLIENT)
#include "scanc_task.h"
#endif /* #if (BLE_SCANPARAM_CLIENT) */

#if (BLE_HR_SENSOR)
#include "hrps_task.h"
#endif /* #if (BLE_HR_SENSOR) */

#if (BLE_HR_COLLECTOR)
#include "hrpc_task.h"
#endif /* #if (BLE_HR_COLLECTOR) */

#if (BLE_CSC_SENSOR)
#include "cscps_task.h"
#endif /* #if (BLE_CSC_SENSOR) */

#if (BLE_CSC_COLLECTOR)
#include "cscpc_task.h"
#endif /* #if (BLE_CSC_COLLECTOR) */

#if (BLE_GL_SENSOR)
#include "glps_task.h"
#endif /* #if (BLE_GL_SENSOR) */

#if (BLE_GL_COLLECTOR)
#include "glpc_task.h"
#endif /* #if (BLE_GL_COLLECTOR) */

#if (BLE_CP_SENSOR)
#include "cpps_task.h"
#endif /* #if (BLE_CP_SENSOR) */

#if (BLE_CP_COLLECTOR)
#include "cppc_task.h"
#endif /* #if (BLE_CP_COLLECTOR) */

#if (BLE_TI_SERVER)
#include "tips_task.h"
#endif /* #if (BLE_TI_SERVER) */

#if (BLE_TI_CLIENT)
#include "tipc_task.h"
#endif /* #if (BLE_TI_CLIENT) */

#if (BLE_AN_SERVER)
#include "anps_task.h"
#endif /* #if (BLE_AN_SERVER) */

#if (BLE_AN_CLIENT)
#include "anpc_task.h"
#endif /* #if (BLE_AN_CLIENT) */

#if (BLE_LN_SENSOR)
#include "lnps_task.h"
#endif /* #if (BLE_LN_SENSOR) */

#if (BLE_LN_COLLECTOR)
#include "lnpc_task.h"
#endif /* #if (BLE_LN_COLLECTOR) */

#if (BLE_PAS_SERVER)
#include "pasps_task.h"
#endif /* #if (BLE_PAS_SERVER) */

#if (BLE_PAS_CLIENT)
#include "paspc_task.h"
#endif /* #if (BLE_PAS_CLIENT) */

#if (BLE_RSC_SENSOR)
#include "rscps_task.h"
#endif /* #if (BLE_RSC_SENSOR) */

#if (BLE_RSC_COLLECTOR)
#include "rscpc_task.h"
#endif /* #if (BLE_RSC_COLLECTOR) */

#if (BLE_ACCEL)
#include "accel_task.h"
#endif
#endif /* #if (BLE_HOST_PRESENT) */

#if (BLE_APP_PRESENT)
#include "app_task.h"
#endif /* #if (BLE_APP_PRESENT) */

#endif /* !defined(_USE_RWBLE_SOURCE) */

#include "rble_rwke.h"
#ifdef CONFIG_EMBEDDED
#include "rble_app.h"
#include "..\sample_app\console.h"
#else
#include "rscip_api.h"
#endif
#ifdef CFG_SAMPLE_BEACON
#include "beacon_app.h"
#endif

#if	__DTM2WIRE_UART_USE__
#include	"DTM2Wire.h"
#endif

// RD8001ëŒâû
#include	"header.h"		//ÉÜÅ[ÉUÅ[íËã`

/************************************************************************/
/*	PROFILE CONFIGURATION						*/
/************************************************************************/


#if defined(_USE_STD_C99)
#define	DESGN(name)	[ name ] =
#else
#define	DESGN(name)
#endif

ke_state_t llc_state[LLC_IDX_MAX];
ke_state_t l2cc_state[L2CC_IDX_MAX];
ke_state_t attc_state[ATTC_IDX_MAX];
ke_state_t smpc_state[SMPC_IDX_MAX];

/// @addtogroup KE_EVT
/// @{

/// Table of event handlers
_TSK_DESC const evt_ptr_t ke_evt_hdlr_ent[32] =
{
#if (BLE_EMB_PRESENT)
    DESGN(KE_EVT_EVENT_START)  &lld_evt_schedule,
    DESGN(KE_EVT_RX         )  &lld_evt_rx,
    DESGN(KE_EVT_EVENT_END  )  &lld_evt_end,
#else
    DESGN(KE_EVT_EVENT_START )  NULL,
    DESGN(KE_EVT_RX          )  NULL,
    DESGN(KE_EVT_EVENT_END   )  NULL,
#endif

#if (BLE_SPLIT || BLE_FULLEMB)
#ifndef	CONFIG_EMBEDDED
    DESGN(KE_EVT_HCI_TX_DONE)  &RSCIP_Tx_Done,
#else
    DESGN(KE_EVT_HCI_TX_DONE)  NULL,
#endif
#else
    DESGN(KE_EVT_HCI_TX_DONE)  NULL,
#endif

    DESGN(KE_EVT_USR_0      )  NULL,	// place holder for user event (high priority)
    DESGN(KE_EVT_USR_1      )  NULL,	// place holder for user event
    DESGN(KE_EVT_KE_TIMER   )  &ke_timer_schedule,
    DESGN(KE_EVT_KE_MESSAGE )  &ke_task_schedule,

#if BLE_EMB_PRESENT
    DESGN(KE_EVT_CRYPT      )  &llm_encryption_done,
#else
    DESGN(KE_EVT_CRYPT      )  NULL,
#endif
#ifndef	CONFIG_EMBEDDED
    DESGN(KE_EVT_HCI_RX_DONE)  &RSCIP_Rx_Done,
#else
    DESGN(KE_EVT_HCI_RX_DONE)  NULL,
#endif
    DESGN(KE_EVT_USR_2)  app_evt_usr_2,	// place holder for user event
    DESGN(KE_EVT_USR_3)  app_evt_usr_3,	// place holder for user event (low priority)
};

///@} KE_EVT


/// @addtogroup KETASK
/// @{

/// Table grouping the task descriptors
_TSK_DESC const struct ke_task_desc TASK_DESC_ent[] =
{

#if (BLE_EMB_PRESENT)
    DESGN(TASK_LLM)  { llm_state_handler, &llm_default_handler, llm_state, LLM_STATE_MAX, LLM_IDX_MAX },
  #if (BLE_CENTRAL || BLE_PERIPHERAL)
    DESGN(TASK_LLC)  { llc_state_handler, &llc_default_handler, llc_state, LLC_STATE_MAX, LLC_IDX_MAX },
  #else
    DESGN(TASK_LLC)  { NULL, NULL, NULL, 0, 0 },
  #endif
    DESGN(TASK_LLD)  { NULL, NULL, NULL, 1, 1 },
  #if (BLE_DEBUG)
    DESGN(TASK_DBG)  { dbg_state_handler, &dbg_default_handler, dbg_state, DBG_STATE_MAX, DBG_IDX_MAX},
  #else
    DESGN(TASK_DBG)  { NULL, NULL, NULL, 0, 0 },
  #endif
#else /* BLE_EMB_PRESENT */
    DESGN(TASK_LLM)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_LLC)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_LLD)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_DBG)  { NULL, NULL, NULL, 0, 0 },
#endif /* BLE_EMB_PRESENT */

#if (BLE_HOST_PRESENT)
  #if (BLE_CENTRAL || BLE_PERIPHERAL)
    DESGN(TASK_L2CM)  { l2cm_state_handler, &l2cm_default_handler, l2cm_state, L2CM_STATE_MAX, L2CM_IDX_MAX },
    DESGN(TASK_L2CC)  { l2cc_state_handler, &l2cc_default_handler, l2cc_state, L2CC_STATE_MAX, L2CC_IDX_MAX },
    #if (RW_BLE_USE_CRYPT)
    DESGN(TASK_SMPM)  { smpm_state_handler, &smpm_default_handler, smpm_state, SMPM_STATE_MAX, SMPM_IDX_MAX },
    DESGN(TASK_SMPC)  { smpc_state_handler, &smpc_default_handler, smpc_state, SMPC_STATE_MAX, SMPC_IDX_MAX },
    #else
    DESGN(TASK_SMPM)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_SMPC)  { NULL, NULL, NULL, 0, 0 },
    #endif
    DESGN(TASK_ATTM)  { attm_state_handler, &attm_default_handler, attm_state, ATTM_STATE_MAX, ATTM_IDX_MAX },
    DESGN(TASK_ATTC)  { attc_state_handler, &attc_default_handler, attc_state, ATTC_STATE_MAX, ATTC_IDX_MAX },
    #if (BLE_ATTS)
    DESGN(TASK_ATTS)  { atts_state_handler, &atts_default_handler, atts_state, ATTS_STATE_MAX, ATTS_IDX_MAX },
    #else
    DESGN(TASK_ATTS)  { NULL, NULL, NULL, 0, 0 },
    #endif
    DESGN(TASK_SVC)   { svc_state_handler,  &svc_default_handler,  svc_state,  SVC_STATE_MAX,  SVC_IDX_MAX  },

    DESGN(TASK_GATT)  { gatt_state_handler, &gatt_default_handler, gatt_state, GATT_STATE_MAX, GATT_IDX_MAX },
    DESGN(TASK_GAP)   { gap_state_handler,  &gap_default_handler,  gap_state,  GAP_STATE_MAX,  GAP_IDX_MAX },
    
  #else
    DESGN(TASK_L2CM)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_L2CC)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_SMPM)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_SMPC)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_ATTM)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_ATTC)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_ATTS)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_SVC)   { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_GATT)  { gatt_state_handler, &gatt_default_handler, gatt_state, GATT_STATE_MAX, GATT_IDX_MAX },
    DESGN(TASK_GAP)   { gap_state_handler,  &gap_default_handler,  gap_state,  GAP_STATE_MAX,  GAP_IDX_MAX },
  #endif
#else
    DESGN(TASK_L2CM)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_L2CC)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_SMPM)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_SMPC)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_ATTM)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_ATTC)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_ATTS)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_SVC)   { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_GATT)  { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_GAP)   { NULL, NULL, NULL, 0, 0 },
#endif
    /* PLACE HOLDER FOR EMBEDDED USER TASKS : START */
    #if	__DTM2WIRE_UART_USE__
    DESGN(TASK_USR_0)  { MDT2Wire_state_handler, &MDT2Wire_default_handler, DTM2Wire_State, DTM2RBLE_STATE_MAX, DTM2RBLE_IDX_MAX },
    #else
    DESGN(TASK_USR_0)  { cpu_com_state_handler, &cpu_com_default_handler, cpu_com_state, CPU_COM_STATE_MAX, CPU_COM_IDX_MAX },				//RD8001ÔøΩŒâÔøΩ
    #endif
    DESGN(TASK_USR_1)  { NULL, NULL, NULL, 0, 0 },
    /* PLACE HOLDER FOR EMBEDDED USER TASKS : END */

    DESGN(TASK_APP)    { rble_state_handler, &rble_default_handler, rble_state, RBLE_STATE_MAX, RBLE_IDX_MAX },
    #ifndef CONFIG_EMBEDDED
    DESGN(TASK_MDM_APPL)  { rble_timer_state_handler, &rble_timer_default_handler, rble_timer_state, 
				RBLE_TIMER_STATE_MAX, RBLE_TIMER_IDX_MAX },
    #else
    #if     defined(CFG_SAMPLE)
    DESGN(TASK_CON_APPL)  { rble_app_state_handler, &rble_app_default_handler, rble_app_state, RBLE_APP_STATE_MAX, RBLE_APP_IDX_MAX },
    #elif   defined(CFG_SAMPLE_BEACON)
    DESGN(TASK_CON_APPL)  { APP_State_Handler, &APP_Default_Handler, APP_State, APP_STATE_MAX, APP_IDX_MAX },
    #else
    DESGN(TASK_CON_APPL)  { NULL, NULL, NULL, 0, 0 },
    #endif
    #endif

#if (BLE_HOST_PRESENT & (BLE_CENTRAL || BLE_PERIPHERAL))
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_PROX_MONITOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_PXPM))
    DESGN(TASK_PROXM) { proxm_state_handler, &proxm_default_handler, proxm_state, PROXM_STATE_MAX, PROXM_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_PROX_REPORTER)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_PXPR))
    DESGN(TASK_PROXR) { proxr_state_handler, &proxr_default_handler, proxr_state, PROXR_STATE_MAX, PROXR_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_FINDME_LOCATOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_FMPL))
    DESGN(TASK_FINDL) { findl_state_handler, &findl_default_handler, findl_state, FINDL_STATE_MAX, FINDL_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_FINDME_TARGET)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_FMPT))
    DESGN(TASK_FINDT) { findt_state_handler, &findt_default_handler, findt_state, FINDT_STATE_MAX, FINDT_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_HT_COLLECTOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_HTPC))
    DESGN(TASK_HTPC)  { htpc_state_handler, &htpc_default_handler, htpc_state, HTPC_STATE_MAX, HTPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_HT_THERMOM)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_HTPT))
    DESGN(TASK_HTPT)  { htpt_state_handler, &htpt_default_handler, htpt_state, HTPT_STATE_MAX, HTPT_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_BL_COLLECTOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_BLPC))
    DESGN(TASK_BLPC)  { blpc_state_handler, &blpc_default_handler, blpc_state, BLPC_STATE_MAX, BLPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_BL_SENSOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_BLPS))
    DESGN(TASK_BLPS)  { blps_state_handler, &blps_default_handler, blps_state, BLPS_STATE_MAX, BLPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_HOG_HDEVICE)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_HGHD))
    DESGN(TASK_HGHD)  { hghd_state_handler, &hghd_default_handler, hghd_state, HGHD_STATE_MAX, HGHD_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_HOG_BHOST)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_HGBH))
    DESGN(TASK_HGBH)  { hgbh_state_handler, &hgbh_default_handler, hgbh_state, HGBH_STATE_MAX, HGBH_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_HOG_RHOST)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_HGRH))
    DESGN(TASK_HGRH)  { hgrh_state_handler, &hgrh_default_handler, hgrh_state, HGRH_STATE_MAX, HGRH_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_SCANPARAM_CLIENT)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_SPPC))
    DESGN(TASK_SCANC) { scanc_state_handler, &scanc_default_handler, scanc_state, SCANC_STATE_MAX, SCANS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_SCANPARAM_SERVER)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_SPPS))
    DESGN(TASK_SCANS) { scans_state_handler, &scans_default_handler, scans_state, SCANS_STATE_MAX, SCANS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_HR_COLLECTOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_HRPC))
    DESGN(TASK_HRPC)  { hrpc_state_handler, &hrpc_default_handler, hrpc_state, HRPC_STATE_MAX, HRPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_HR_SENSOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_HRPS))
    DESGN(TASK_HRPS)  { hrps_state_handler, &hrps_default_handler, hrps_state, HRPS_STATE_MAX, HRPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_CSC_COLLECTOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_CSCC))
    DESGN(TASK_CSCPC) { cscpc_state_handler, &cscpc_default_handler, cscpc_state, CSCPC_STATE_MAX, CSCPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_CSC_SENSOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_CSCS))
    DESGN(TASK_CSCPS) { cscps_state_handler, &cscps_default_handler, cscps_state, CSCPS_STATE_MAX, CSCPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_GL_COLLECTOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_GLPC))
    DESGN(TASK_GLPC)  { glpc_state_handler, &glpc_default_handler, glpc_state, GLPC_STATE_MAX, GLPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_GL_SENSOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_GLPS))
    DESGN(TASK_GLPS)  { glps_state_handler, &glps_default_handler, glps_state, GLPS_STATE_MAX, GLPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_CP_COLLECTOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_CPPC))
    DESGN(TASK_CPPC)  { cppc_state_handler, &cppc_default_handler, cppc_state, CPPC_STATE_MAX, CPPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_CP_SENSOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_CPPS))
    DESGN(TASK_CPPS)  { cpps_state_handler, &cpps_default_handler, cpps_state, CPPS_STATE_MAX, CPPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_TI_CLIENT)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_TIPC))
    DESGN(TASK_TIPC)  { tipc_state_handler, &tipc_default_handler, tipc_state, TIPC_STATE_MAX, TIPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_TI_SERVER)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_TIPS))
    DESGN(TASK_TIPS)  { tips_state_handler, &tips_default_handler, tips_state, TIPS_STATE_MAX, TIPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_AN_CLIENT)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_ANPC))
    DESGN(TASK_ANPC)  { anpc_state_handler, &anpc_default_handler, anpc_state, ANPC_STATE_MAX, ANPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_AN_SERVER)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_ANPS))
    DESGN(TASK_ANPS)  { anps_state_handler, &anps_default_handler, anps_state, ANPS_STATE_MAX, ANPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_LN_COLLECTOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_LNPC))
    DESGN(TASK_LNPC)  { lnpc_state_handler, &lnpc_default_handler, lnpc_state, LNPC_STATE_MAX, LNPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_LN_SENSOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_LNPS))
    DESGN(TASK_LNPS)  { lnps_state_handler, &lnps_default_handler, lnps_state, LNPS_STATE_MAX, LNPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_PAS_CLIENT)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_PASC))
    DESGN(TASK_PASPC) { paspc_state_handler, &paspc_default_handler, paspc_state, PASPC_STATE_MAX, PASPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_PAS_SERVER)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_PASS))
    DESGN(TASK_PASPS) { pasps_state_handler, &pasps_default_handler, pasps_state, PASPS_STATE_MAX, PASPS_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_RSC_COLLECTOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_RSCC))
    DESGN(TASK_RSCPC) { rscpc_state_handler, &rscpc_default_handler, rscpc_state, RSCPC_STATE_MAX, RSCPC_IDX_MAX },
    #endif
    #if (defined(_USE_RWBLE_SOURCE) && (BLE_RSC_SENSOR)) || (!defined(_USE_RWBLE_SOURCE) && (PRF_SEL_RSCS))
    DESGN(TASK_RSCPS) { rscps_state_handler, &rscps_default_handler, rscps_state, RSCPS_STATE_MAX, RSCPS_IDX_MAX },
    #endif
    #if (BLE_ACCEL)
    DESGN(TASK_ACCEL) { accel_state_handler, &accel_default_handler, accel_state, ACCEL_STATE_MAX, ACCEL_IDX_MAX },
    #endif
#endif
};

/// @}
