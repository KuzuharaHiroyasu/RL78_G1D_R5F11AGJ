/**
 ****************************************************************************************
 *
 * @file        ke_conf_simple.c
 *
 * @brief       This file contains the implementation of the kernel task management.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2016 Renesas Electronics Corporation
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

#include "arch.h"
#include "prf_sel.h"
#include "rwke_api.h"

#include "rble_rwke.h"
#ifdef CONFIG_EMBEDDED
#include "rble_app.h"
#include "rble_sample_app_peripheral.h"
#include "console.h"
#else
#include "rscip_api.h"
#endif

/************************************************************************/
/*  PROFILE CONFIGURATION                                               */
/************************************************************************/


#if defined(_USE_STD_C99)
#define DESGN(name) [ name ] =
#else
#define DESGN(name)
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
    DESGN(KE_EVT_EVENT_START)  &lld_evt_schedule,
    DESGN(KE_EVT_RX         )  &lld_evt_rx,
    DESGN(KE_EVT_EVENT_END  )  &lld_evt_end,

#ifndef CONFIG_EMBEDDED
    DESGN(KE_EVT_HCI_TX_DONE)  &RSCIP_Tx_Done,
#else
    DESGN(KE_EVT_HCI_TX_DONE)  NULL,
#endif

    DESGN(KE_EVT_USR_0      )  NULL,    // place holder for user event (high priority)
    DESGN(KE_EVT_USR_1      )  NULL,    // place holder for user event

    DESGN(KE_EVT_KE_TIMER   )  &ke_timer_schedule,
    DESGN(KE_EVT_KE_MESSAGE )  &ke_task_schedule,
    DESGN(KE_EVT_CRYPT      )  &llm_encryption_done,

#ifndef CONFIG_EMBEDDED
    DESGN(KE_EVT_HCI_RX_DONE)  &RSCIP_Rx_Done,
#else
    DESGN(KE_EVT_HCI_RX_DONE)  NULL,
#endif

    DESGN(KE_EVT_USR_2      )  NULL,    // place holder for user event
    DESGN(KE_EVT_USR_3      )  NULL,    // place holder for user event (low priority)
};

///@} KE_EVT


/// @addtogroup KETASK
/// @{

/// Table grouping the task descriptors
_TSK_DESC const struct ke_task_desc TASK_DESC_ent[] =
{

    DESGN(TASK_LLM)   { llm_state_handler, &llm_default_handler, llm_state, LLM_STATE_MAX, LLM_IDX_MAX },
    DESGN(TASK_LLC)   { llc_state_handler, &llc_default_handler, llc_state, LLC_STATE_MAX, LLC_IDX_MAX },
    DESGN(TASK_LLD)   { NULL, NULL, NULL, 1, 1 },
    DESGN(TASK_DBG)   { dbg_state_handler, &dbg_default_handler, dbg_state, DBG_STATE_MAX, DBG_IDX_MAX },

    DESGN(TASK_L2CM)  { l2cm_state_handler, &l2cm_default_handler, l2cm_state, L2CM_STATE_MAX, L2CM_IDX_MAX },
    DESGN(TASK_L2CC)  { l2cc_state_handler, &l2cc_default_handler, l2cc_state, L2CC_STATE_MAX, L2CC_IDX_MAX },
    DESGN(TASK_SMPM)  { smpm_state_handler, &smpm_default_handler, smpm_state, SMPM_STATE_MAX, SMPM_IDX_MAX },
    DESGN(TASK_SMPC)  { smpc_state_handler, &smpc_default_handler, smpc_state, SMPC_STATE_MAX, SMPC_IDX_MAX },
    DESGN(TASK_ATTM)  { attm_state_handler, &attm_default_handler, attm_state, ATTM_STATE_MAX, ATTM_IDX_MAX },
    DESGN(TASK_ATTC)  { attc_state_handler, &attc_default_handler, attc_state, ATTC_STATE_MAX, ATTC_IDX_MAX },
    DESGN(TASK_ATTS)  { atts_state_handler, &atts_default_handler, atts_state, ATTS_STATE_MAX, ATTS_IDX_MAX },
    DESGN(TASK_SVC)   { svc_state_handler,  &svc_default_handler,  svc_state,  SVC_STATE_MAX,  SVC_IDX_MAX  },
    DESGN(TASK_GATT)  { gatt_state_handler, &gatt_default_handler, gatt_state, GATT_STATE_MAX, GATT_IDX_MAX },
    DESGN(TASK_GAP)   { gap_state_handler,  &gap_default_handler,  gap_state,  GAP_STATE_MAX,  GAP_IDX_MAX  },

    DESGN(TASK_USR_0) { app_state_handler, &app_default_handler, app_state, APP_STATE_MAX, APP_IDX_MAX },
    DESGN(TASK_USR_1) { NULL, NULL, NULL, 0, 0 },
    DESGN(TASK_APP)   { rble_state_handler, &rble_default_handler, rble_state, RBLE_STATE_MAX, RBLE_IDX_MAX },

    #ifndef CONFIG_EMBEDDED
    DESGN(TASK_MDM_APPL)  { rble_timer_state_handler, &rble_timer_default_handler, rble_timer_state, RBLE_TIMER_STATE_MAX, RBLE_TIMER_IDX_MAX },
    #else
    DESGN(TASK_CON_APPL)  { console_state_handler, &console_default_handler, console_state, CONSOLE_STATE_MAX, CONSOLE_IDX_MAX },
    #endif

    #if (PRF_SEL_PXPM)
    DESGN(TASK_PROXM) { proxm_state_handler, &proxm_default_handler, proxm_state, PROXM_STATE_MAX, PROXM_IDX_MAX },
    #endif
    #if (PRF_SEL_PXPR)
    DESGN(TASK_PROXR) { proxr_state_handler, &proxr_default_handler, proxr_state, PROXR_STATE_MAX, PROXR_IDX_MAX },
    #endif
    #if (PRF_SEL_FMPL)
    DESGN(TASK_FINDL) { findl_state_handler, &findl_default_handler, findl_state, FINDL_STATE_MAX, FINDL_IDX_MAX },
    #endif
    #if (PRF_SEL_FMPT)
    DESGN(TASK_FINDT) { findt_state_handler, &findt_default_handler, findt_state, FINDT_STATE_MAX, FINDT_IDX_MAX },
    #endif
    #if (PRF_SEL_HTPC)
    DESGN(TASK_HTPC)  { htpc_state_handler, &htpc_default_handler, htpc_state, HTPC_STATE_MAX, HTPC_IDX_MAX },
    #endif
    #if (PRF_SEL_HTPT)
    DESGN(TASK_HTPT)  { htpt_state_handler, &htpt_default_handler, htpt_state, HTPT_STATE_MAX, HTPT_IDX_MAX },
    #endif
    #if (PRF_SEL_BLPC)
    DESGN(TASK_BLPC)  { blpc_state_handler, &blpc_default_handler, blpc_state, BLPC_STATE_MAX, BLPC_IDX_MAX },
    #endif
    #if (PRF_SEL_BLPS)
    DESGN(TASK_BLPS)  { blps_state_handler, &blps_default_handler, blps_state, BLPS_STATE_MAX, BLPS_IDX_MAX },
    #endif
    #if (PRF_SEL_HGHD)
    DESGN(TASK_HGHD)  { hghd_state_handler, &hghd_default_handler, hghd_state, HGHD_STATE_MAX, HGHD_IDX_MAX },
    #endif
    #if (PRF_SEL_HGBH)
    DESGN(TASK_HGBH)  { hgbh_state_handler, &hgbh_default_handler, hgbh_state, HGBH_STATE_MAX, HGBH_IDX_MAX },
    #endif
    #if (PRF_SEL_HGRH)
    DESGN(TASK_HGRH)  { hgrh_state_handler, &hgrh_default_handler, hgrh_state, HGRH_STATE_MAX, HGRH_IDX_MAX },
    #endif
    #if (PRF_SEL_SPPC)
    DESGN(TASK_SCANC) { scanc_state_handler, &scanc_default_handler, scanc_state, SCANC_STATE_MAX, SCANS_IDX_MAX },
    #endif
    #if (PRF_SEL_SPPS)
    DESGN(TASK_SCANS) { scans_state_handler, &scans_default_handler, scans_state, SCANS_STATE_MAX, SCANS_IDX_MAX },
    #endif
    #if (PRF_SEL_HRPC)
    DESGN(TASK_HRPC)  { hrpc_state_handler, &hrpc_default_handler, hrpc_state, HRPC_STATE_MAX, HRPC_IDX_MAX },
    #endif
    #if (PRF_SEL_HRPS)
    DESGN(TASK_HRPS)  { hrps_state_handler, &hrps_default_handler, hrps_state, HRPS_STATE_MAX, HRPS_IDX_MAX },
    #endif
    #if (PRF_SEL_CSCC)
    DESGN(TASK_CSCPC) { cscpc_state_handler, &cscpc_default_handler, cscpc_state, CSCPC_STATE_MAX, CSCPC_IDX_MAX },
    #endif
    #if (PRF_SEL_CSCS)
    DESGN(TASK_CSCPS) { cscps_state_handler, &cscps_default_handler, cscps_state, CSCPS_STATE_MAX, CSCPS_IDX_MAX },
    #endif
    #if (PRF_SEL_GLPC)
    DESGN(TASK_GLPC)  { glpc_state_handler, &glpc_default_handler, glpc_state, GLPC_STATE_MAX, GLPC_IDX_MAX },
    #endif
    #if (PRF_SEL_GLPS)
    DESGN(TASK_GLPS)  { glps_state_handler, &glps_default_handler, glps_state, GLPS_STATE_MAX, GLPS_IDX_MAX },
    #endif
    #if (PRF_SEL_CPPC)
    DESGN(TASK_CPPC)  { cppc_state_handler, &cppc_default_handler, cppc_state, CPPC_STATE_MAX, CPPC_IDX_MAX },
    #endif
    #if (PRF_SEL_CPPS)
    DESGN(TASK_CPPS)  { cpps_state_handler, &cpps_default_handler, cpps_state, CPPS_STATE_MAX, CPPS_IDX_MAX },
    #endif
    #if (PRF_SEL_TIPC)
    DESGN(TASK_TIPC)  { tipc_state_handler, &tipc_default_handler, tipc_state, TIPC_STATE_MAX, TIPC_IDX_MAX },
    #endif
    #if (PRF_SEL_TIPS)
    DESGN(TASK_TIPS)  { tips_state_handler, &tips_default_handler, tips_state, TIPS_STATE_MAX, TIPS_IDX_MAX },
    #endif
    #if (PRF_SEL_ANPC)
    DESGN(TASK_ANPC)  { anpc_state_handler, &anpc_default_handler, anpc_state, ANPC_STATE_MAX, ANPC_IDX_MAX },
    #endif
    #if (PRF_SEL_ANPS)
    DESGN(TASK_ANPS)  { anps_state_handler, &anps_default_handler, anps_state, ANPS_STATE_MAX, ANPS_IDX_MAX },
    #endif
    #if (PRF_SEL_LNPC)
    DESGN(TASK_LNPC)  { lnpc_state_handler, &lnpc_default_handler, lnpc_state, LNPC_STATE_MAX, LNPC_IDX_MAX },
    #endif
    #if (PRF_SEL_LNPS)
    DESGN(TASK_LNPS)  { lnps_state_handler, &lnps_default_handler, lnps_state, LNPS_STATE_MAX, LNPS_IDX_MAX },
    #endif
    #if (PRF_SEL_PASC)
    DESGN(TASK_PASPC) { paspc_state_handler, &paspc_default_handler, paspc_state, PASPC_STATE_MAX, PASPC_IDX_MAX },
    #endif
    #if (PRF_SEL_PASS)
    DESGN(TASK_PASPS) { pasps_state_handler, &pasps_default_handler, pasps_state, PASPS_STATE_MAX, PASPS_IDX_MAX },
    #endif
    #if (PRF_SEL_RSCC)
    DESGN(TASK_RSCPC) { rscpc_state_handler, &rscpc_default_handler, rscpc_state, RSCPC_STATE_MAX, RSCPC_IDX_MAX },
    #endif
    #if (PRF_SEL_RSCS)
    DESGN(TASK_RSCPS) { rscps_state_handler, &rscps_default_handler, rscps_state, RSCPS_STATE_MAX, RSCPS_IDX_MAX },
    #endif
    #if (BLE_ACCEL)
    DESGN(TASK_ACCEL) { accel_state_handler, &accel_default_handler, accel_state, ACCEL_STATE_MAX, ACCEL_IDX_MAX },
    #endif
};

/// @}
