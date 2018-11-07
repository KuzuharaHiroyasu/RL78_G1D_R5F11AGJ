/**
 ****************************************************************************************
 *
 * @file	rble_modem_config.h
 *
 * @brief	rBLE Modem Configuration Header File
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef	__RBLE_MODEM_CONFIG_INC__
#define	__RBLE_MODEM_CONFIG_INC__

/**
 * INCLUDE FILES
 ****************************************************************************************
 */

/**
 * TYPE DEFINES
 ****************************************************************************************
 */
typedef RBLE_STATUS (*rble_cmd_exe_func_t)(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);

/**
 * DEFINES
 ****************************************************************************************
 */
extern RBLE_STATUS RBLE_MDM_APPL_PXPM_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_PXPR_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_FMPL_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_FMPT_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_HTPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_HTPT_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_BLPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_BLPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_HGHD_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_HGBH_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_HGRH_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_SPPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_SPPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_HRPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_HRPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_CSCPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_CSCPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_GLPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_GLPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_CPPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_CPPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_TIPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_TIPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_ANPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_ANPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_LNPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_LNPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_PASPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_PASPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_RSCPC_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);
extern RBLE_STATUS RBLE_MDM_APPL_RSCPS_Command_Exe(RBLE_COMMAND_NO cmd_id, uint8_t *data_pnt);

/**
 * STRUCTURE DECLARATIONS
 ****************************************************************************************
 */
typedef struct RBLE_MODEM_SEL_CMD_EXE_t
{
    uint16_t sel_key;                        /* Retrieval key */
    rble_cmd_exe_func_t func_cmd_exe;       /* Command execution function */
}RBLE_MODEM_SEL_CMD_EXE;

/**
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
extern const RBLE_MODEM_SEL_CMD_EXE rble_modem_sel_cmd_exe_table_ent[];

#endif // __RBLE_MODEM_CONFIG_INC__
