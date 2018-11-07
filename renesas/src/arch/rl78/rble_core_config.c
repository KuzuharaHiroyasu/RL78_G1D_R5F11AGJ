/**
 ****************************************************************************************
 *
 * @file	rble_core_config.c
 *
 * @brief	rBLE Core Configuration File
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */
#include "rwble_config.h"
#include "arch.h"

#include "rwke_api.h"

#include "rble_api.h"

#include "rble_core_config.h"

#include "prf_sel.h"

/**************************** Define Declaration ****************************************/

/**************************** STRUCTURE DECLARATIOM ****************************************/

/**************************** PUBLIC Memory Declaration ***********************************/
const RBLE_CORE_SEL_PROF_HANDLER rble_core_sel_handler_table_ent[] =
{
#if (PRF_SEL_PXPM)
    { rble_pxpm_handler, &rble_pxpm_handler_size },
#endif
#if (PRF_SEL_PXPR)
    { rble_pxpr_handler, &rble_pxpr_handler_size },
#endif
#if (PRF_SEL_FMPL)
    { rble_fmpl_handler, &rble_fmpl_handler_size },
#endif
#if (PRF_SEL_FMPT)
    { rble_fmpt_handler, &rble_fmpt_handler_size },
#endif
#if (PRF_SEL_HTPC)
    { rble_htpc_handler, &rble_htpc_handler_size },
#endif
#if (PRF_SEL_HTPT)
    { rble_htpt_handler, &rble_htpt_handler_size },
#endif
#if (PRF_SEL_BLPC)
    { rble_blpc_handler, &rble_blpc_handler_size },
#endif
#if (PRF_SEL_BLPS)
    { rble_blps_handler, &rble_blps_handler_size },
#endif
#if (PRF_SEL_HGHD)
    { rble_hghd_handler, &rble_hghd_handler_size },
#endif
#if (PRF_SEL_HGBH)
    { rble_hgbh_handler, &rble_hgbh_handler_size },
#endif
#if (PRF_SEL_HGRH)
    { rble_hgrh_handler, &rble_hgrh_handler_size },
#endif
#if (PRF_SEL_SPPS)
    { rble_spps_handler, &rble_spps_handler_size },
#endif
#if (PRF_SEL_SPPC)
    { rble_sppc_handler, &rble_sppc_handler_size },
#endif
#if (PRF_SEL_HRPC)
    { rble_hrpc_handler, &rble_hrpc_handler_size },
#endif
#if (PRF_SEL_HRPS)
    { rble_hrps_handler, &rble_hrps_handler_size },
#endif
#if (PRF_SEL_CSCC)
    { rble_cscc_handler, &rble_cscc_handler_size },
#endif
#if (PRF_SEL_CSCS)
    { rble_cscs_handler, &rble_cscs_handler_size },
#endif
#if (PRF_SEL_GLPC)
    { rble_glpc_handler, &rble_glpc_handler_size },
#endif
#if (PRF_SEL_GLPS)
    { rble_glps_handler, &rble_glps_handler_size },
#endif
#if (PRF_SEL_CPPC)
    { rble_cppc_handler, &rble_cppc_handler_size },
#endif
#if (PRF_SEL_CPPS)
    { rble_cpps_handler, &rble_cpps_handler_size },
#endif
#if (PRF_SEL_TIPC)
    { rble_tipc_handler, &rble_tipc_handler_size },
#endif
#if (PRF_SEL_TIPS)
    { rble_tips_handler, &rble_tips_handler_size },
#endif
#if (PRF_SEL_ANPC)
    { rble_anpc_handler, &rble_anpc_handler_size },
#endif
#if (PRF_SEL_ANPS)
    { rble_anps_handler, &rble_anps_handler_size },
#endif
#if (PRF_SEL_LNPC)
    { rble_lnpc_handler, &rble_lnpc_handler_size },
#endif
#if (PRF_SEL_LNPS)
    { rble_lnps_handler, &rble_lnps_handler_size },
#endif
#if (PRF_SEL_PASC)
    { rble_pasc_handler, &rble_pasc_handler_size },
#endif
#if (PRF_SEL_PASS)
    { rble_pass_handler, &rble_pass_handler_size },
#endif
#if (PRF_SEL_RSCC)
    { rble_rscc_handler, &rble_rscc_handler_size },
#endif
#if (PRF_SEL_RSCS)
    { rble_rscs_handler, &rble_rscs_handler_size },
#endif
    { NULL, NULL },
};

const rble_prof_init_func_t prof_init_table_ent[] =
{
#if (PRF_SEL_PXPM)
    RBLE_PXPM_Init,
#endif
#if (PRF_SEL_PXPR)
    RBLE_PXPR_Init,
#endif
#if (PRF_SEL_FMPL)
    RBLE_FMPL_Init,
#endif
#if (PRF_SEL_FMPT)
    RBLE_FMPT_Init,
#endif
#if (PRF_SEL_HTPC)
    RBLE_HTPC_Init,
#endif
#if (PRF_SEL_HTPT)
    RBLE_HTPT_Init,
#endif
#if (PRF_SEL_BLPC)
    RBLE_BLPC_Init,
#endif
#if (PRF_SEL_BLPS)
    RBLE_BLPS_Init,
#endif
#if (PRF_SEL_HGHD)
    RBLE_HGHD_Init,
#endif
#if (PRF_SEL_HGBH)
    RBLE_HGBH_Init,
#endif
#if (PRF_SEL_HGRH)
    RBLE_HGRH_Init,
#endif
#if (PRF_SEL_SPPS)
    RBLE_SPPS_Init,
#endif
#if (PRF_SEL_SPPC)
    RBLE_SPPC_Init,
#endif
#if (PRF_SEL_HRPC)
    RBLE_HRPC_Init,
#endif
#if (PRF_SEL_HRPS)
    RBLE_HRPS_Init,
#endif
#if (PRF_SEL_CSCC)
    RBLE_CSCPC_Init,
#endif
#if (PRF_SEL_CSCS)
    RBLE_CSCPS_Init,
#endif
#if (PRF_SEL_GLPC)
    RBLE_GLPC_Init,
#endif
#if (PRF_SEL_GLPS)
    RBLE_GLPS_Init,
#endif
#if (PRF_SEL_CPPC)
    RBLE_CPPC_Init,
#endif
#if (PRF_SEL_CPPS)
    RBLE_CPPS_Init,
#endif
#if (PRF_SEL_TIPC)
    RBLE_TIPC_Init,
#endif
#if (PRF_SEL_TIPS)
    RBLE_TIPS_Init,
#endif
#if (PRF_SEL_ANPC)
    RBLE_ANPC_Init,
#endif
#if (PRF_SEL_ANPS)
    RBLE_ANPS_Init,
#endif
#if (PRF_SEL_LNPC)
    RBLE_LNPC_Init,
#endif
#if (PRF_SEL_LNPS)
    RBLE_LNPS_Init,
#endif
#if (PRF_SEL_PASC)
    RBLE_PASPC_Init,
#endif
#if (PRF_SEL_PASS)
    RBLE_PASPS_Init,
#endif
#if (PRF_SEL_RSCC)
    RBLE_RSCPC_Init,
#endif
#if (PRF_SEL_RSCS)
    RBLE_RSCPS_Init,
#endif
    NULL,
};


const RBLE_PROF_VERSION rble_prof_version_table_ent[] =
{
#if (PRF_SEL_PXPM)
    { &pxpm_major_ver, &pxpm_minor_ver },
#endif
#if (PRF_SEL_PXPR)
    { &pxpr_major_ver, &pxpr_minor_ver },
#endif
#if (PRF_SEL_FMPL)
    { &fmpl_major_ver, &fmpl_minor_ver },
#endif
#if (PRF_SEL_FMPT)
    { &fmpt_major_ver, &fmpt_minor_ver },
#endif
#if (PRF_SEL_HTPC)
    { &htpc_major_ver, &htpc_minor_ver },
#endif
#if (PRF_SEL_HTPT)
    { &htpt_major_ver, &htpt_minor_ver },
#endif
#if (PRF_SEL_BLPC)
    { &blpc_major_ver, &blpc_minor_ver },
#endif
#if (PRF_SEL_BLPS)
    { &blps_major_ver, &blps_minor_ver },
#endif
#if (PRF_SEL_HGHD)
    { &hghd_major_ver, &hghd_minor_ver },
#endif
#if (PRF_SEL_HGBH)
    { &hgbh_major_ver, &hgbh_minor_ver },
#endif
#if (PRF_SEL_HGRH)
    { &hgrh_major_ver, &hgrh_minor_ver },
#endif
#if (PRF_SEL_SPPS)
    { &spps_major_ver, &spps_minor_ver },
#endif
#if (PRF_SEL_SPPC)
    { &sppc_major_ver, &sppc_minor_ver },
#endif
#if (PRF_SEL_HRPC)
    { &hrpc_major_ver, &hrpc_minor_ver },
#endif
#if (PRF_SEL_HRPS)
    { &hrps_major_ver, &hrps_minor_ver },
#endif
#if (PRF_SEL_CSCC)
    { &cscc_major_ver, &cscc_minor_ver },
#endif
#if (PRF_SEL_CSCS)
    { &cscs_major_ver, &cscs_minor_ver },
#endif
#if (PRF_SEL_GLPC)
    { &glpc_major_ver, &glpc_minor_ver },
#endif
#if (PRF_SEL_GLPS)
    { &glps_major_ver, &glps_minor_ver },
#endif
#if (PRF_SEL_CPPC)
    { &cppc_major_ver, &cppc_minor_ver },
#endif
#if (PRF_SEL_CPPS)
    { &cpps_major_ver, &cpps_minor_ver },
#endif
#if (PRF_SEL_TIPC)
    { &tipc_major_ver, &tipc_minor_ver },
#endif
#if (PRF_SEL_TIPS)
    { &tips_major_ver, &tips_minor_ver },
#endif
#if (PRF_SEL_ANPC)
    { &anpc_major_ver, &anpc_minor_ver },
#endif
#if (PRF_SEL_ANPS)
    { &anps_major_ver, &anps_minor_ver },
#endif
#if (PRF_SEL_LNPC)
    { &lnpc_major_ver, &lnpc_minor_ver },
#endif
#if (PRF_SEL_LNPS)
    { &lnps_major_ver, &lnps_minor_ver },
#endif
#if (PRF_SEL_PASC)
    { &pasc_major_ver, &pasc_minor_ver },
#endif
#if (PRF_SEL_PASS)
    { &pass_major_ver, &pass_minor_ver },
#endif
#if (PRF_SEL_RSCC)
    { &rscc_major_ver, &rscc_minor_ver },
#endif
#if (PRF_SEL_RSCS)
    { &rscs_major_ver, &rscs_minor_ver },
#endif
    { NULL, NULL }
};

/**************************** PRIVATE Memory Declaration **********************************/

/******************************* PRIVATE PROTOTYPE ****************************************/

/******************************* Program Area *************************************/

