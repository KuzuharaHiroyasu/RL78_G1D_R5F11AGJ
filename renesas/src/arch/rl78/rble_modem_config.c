/**
 ****************************************************************************************
 *
 * @file	rble_modem_config.c
 *
 * @brief	rBLE Modem Configuration File
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */
#include "rwble_config.h"
#include "arch.h"

#include "rble_api.h"

#include "rble_modem_config.h"

#include "prf_sel.h"

/**************************** Define Declaration ****************************************/

/**************************** STRUCTURE DECLARATIOM ****************************************/

/**************************** PUBLIC Memory Declaration ***********************************/
const RBLE_MODEM_SEL_CMD_EXE rble_modem_sel_cmd_exe_table_ent[] =
{
#if (PRF_SEL_PXPM)
    { RBLE_CMD_PXP_MONITOR_TOP,		RBLE_MDM_APPL_PXPM_Command_Exe },
#endif
#if (PRF_SEL_PXPR)
    { RBLE_CMD_PXP_REPORTER_TOP,	RBLE_MDM_APPL_PXPR_Command_Exe },
#endif
#if (PRF_SEL_FMPL)
    { RBLE_CMD_FMP_LOCATOR_TOP,		RBLE_MDM_APPL_FMPL_Command_Exe },
#endif
#if (PRF_SEL_FMPT)
    { RBLE_CMD_FMP_TARGET_TOP,		RBLE_MDM_APPL_FMPT_Command_Exe },
#endif
#if (PRF_SEL_HTPC)
    { RBLE_CMD_HTP_COLLECTOR,		RBLE_MDM_APPL_HTPC_Command_Exe },
#endif
#if (PRF_SEL_HTPT)
    { RBLE_CMD_HTP_THERMOMETER_TOP,	RBLE_MDM_APPL_HTPT_Command_Exe },
#endif
#if (PRF_SEL_BLPC)
    { RBLE_CMD_BLP_COLLECTOR,		RBLE_MDM_APPL_BLPC_Command_Exe },
#endif
#if (PRF_SEL_BLPS)
    { RBLE_CMD_BLP_SENSOR_TOP,		RBLE_MDM_APPL_BLPS_Command_Exe },
#endif
#if (PRF_SEL_HGHD)
    { RBLE_CMD_HGP_HDEVICE_TOP,		RBLE_MDM_APPL_HGHD_Command_Exe },
#endif
#if (PRF_SEL_HGBH)
    { RBLE_CMD_HGP_BHOST_TOP,		RBLE_MDM_APPL_HGBH_Command_Exe },
#endif
#if (PRF_SEL_HGRH)
    { RBLE_CMD_HGP_RHOST_TOP,		RBLE_MDM_APPL_HGRH_Command_Exe },
#endif
#if (PRF_SEL_SPPS)
    { RBLE_CMD_SPP_SERVER_TOP,		RBLE_MDM_APPL_SPPS_Command_Exe },
#endif
#if (PRF_SEL_SPPC)
    { RBLE_CMD_SPP_CLIENT_TOP,		RBLE_MDM_APPL_SPPC_Command_Exe },
#endif
#if (PRF_SEL_HRPC)
    { RBLE_CMD_HRP_COLLECTOR_TOP,	RBLE_MDM_APPL_HRPC_Command_Exe },
#endif
#if (PRF_SEL_HRPS)
    { RBLE_CMD_HRP_SENSOR_TOP,		RBLE_MDM_APPL_HRPS_Command_Exe },
#endif
#if (PRF_SEL_CSCC)
    { RBLE_CMD_CSCP_COLLECTOR_TOP,	RBLE_MDM_APPL_CSCPC_Command_Exe },
#endif
#if (PRF_SEL_CSCS)
    { RBLE_CMD_CSCP_SENSOR_TOP,		RBLE_MDM_APPL_CSCPS_Command_Exe },
#endif
#if (PRF_SEL_GLPC)
    { RBLE_CMD_GLP_COLLECTOR_TOP,	RBLE_MDM_APPL_GLPC_Command_Exe },
#endif
#if (PRF_SEL_GLPS)
    { RBLE_CMD_GLP_SENSOR_TOP,		RBLE_MDM_APPL_GLPS_Command_Exe },
#endif
#if (PRF_SEL_CPPC)
    { RBLE_CMD_CPP_COLLECTOR_TOP,	RBLE_MDM_APPL_CPPC_Command_Exe },
#endif
#if (PRF_SEL_CPPS)
    { RBLE_CMD_CPP_SENSOR_TOP,		RBLE_MDM_APPL_CPPS_Command_Exe },
#endif
#if (PRF_SEL_TIPC)
    { RBLE_CMD_TIP_CLIENT_TOP,		RBLE_MDM_APPL_TIPC_Command_Exe },
#endif
#if (PRF_SEL_TIPS)
    { RBLE_CMD_TIP_SERVER_TOP,		RBLE_MDM_APPL_TIPS_Command_Exe },
#endif
#if (PRF_SEL_ANPC)
    { RBLE_CMD_ANP_CLIENT_TOP,		RBLE_MDM_APPL_ANPC_Command_Exe },
#endif
#if (PRF_SEL_ANPS)
    { RBLE_CMD_ANP_SERVER_TOP,		RBLE_MDM_APPL_ANPS_Command_Exe },
#endif
#if (PRF_SEL_LNPC)
    { RBLE_CMD_LNP_COLLECTOR_TOP,	RBLE_MDM_APPL_LNPC_Command_Exe },
#endif
#if (PRF_SEL_LNPS)
    { RBLE_CMD_LNP_SENSOR_TOP,		RBLE_MDM_APPL_LNPS_Command_Exe },
#endif
#if (PRF_SEL_PASC)
    { RBLE_CMD_PASP_CLIENT_TOP,		RBLE_MDM_APPL_PASPC_Command_Exe },
#endif
#if (PRF_SEL_PASS)
    { RBLE_CMD_PASP_SERVER_TOP,		RBLE_MDM_APPL_PASPS_Command_Exe },
#endif
#if (PRF_SEL_RSCC)
    { RBLE_CMD_RSCP_COLLECTOR_TOP,	RBLE_MDM_APPL_RSCPC_Command_Exe },
#endif
#if (PRF_SEL_RSCS)
    { RBLE_CMD_RSCP_SENSOR_TOP,		RBLE_MDM_APPL_RSCPS_Command_Exe },
#endif
    { 0, NULL },
};

/**************************** PRIVATE Memory Declaration **********************************/

/******************************* PRIVATE PROTOTYPE ****************************************/

/******************************* Program Area *************************************/

