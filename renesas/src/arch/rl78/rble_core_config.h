/**
 ****************************************************************************************
 *
 * @file	rble_core_config.h
 *
 * @brief	rBLE Core Configuration Header File
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef	__RBLE_CORE_CONFIG_INC__
#define	__RBLE_CORE_CONFIG_INC__

/**
 * INCLUDE FILES
 ****************************************************************************************
 */

/**
 * TYPE DEFINES
 ****************************************************************************************
 */
typedef void (*rble_prof_init_func_t)(void);

/**
 * DEFINES
 ****************************************************************************************
 */
extern void RBLE_PXPM_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_PXPR_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_FMPL_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_FMPT_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_HTPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_HTPT_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_BLPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_BLPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_HGHD_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_HGBH_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_HGRH_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_SPPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_SPPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_HRPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_HRPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_CSCPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_CSCPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_GLPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_GLPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_CPPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_CPPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_TIPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_TIPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_ANPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_ANPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_LNPC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_LNPS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_PASC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_PASS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_RSCC_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );
extern void RBLE_RSCS_Cmd_Disallowed_Evt( uint16_t Opcode, uint8_t Status );

extern void RBLE_PXPM_Init( void );
extern void RBLE_PXPR_Init( void );
extern void RBLE_FMPL_Init( void );
extern void RBLE_FMPT_Init( void );
extern void RBLE_HTPC_Init( void );
extern void RBLE_HTPT_Init( void );
extern void RBLE_BLPC_Init( void );
extern void RBLE_BLPS_Init( void );
extern void RBLE_HGHD_Init( void );
extern void RBLE_HGBH_Init( void );
extern void RBLE_HGRH_Init( void );
extern void RBLE_SPPS_Init( void );
extern void RBLE_SPPC_Init( void );
extern void RBLE_HRPC_Init( void );
extern void RBLE_HRPS_Init( void );
extern void RBLE_CSCPC_Init( void );
extern void RBLE_CSCPS_Init( void );
extern void RBLE_GLPC_Init( void );
extern void RBLE_GLPS_Init( void );
extern void RBLE_CPPC_Init( void );
extern void RBLE_CPPS_Init( void );
extern void RBLE_TIPC_Init( void );
extern void RBLE_TIPS_Init( void );
extern void RBLE_ANPC_Init( void );
extern void RBLE_ANPS_Init( void );
extern void RBLE_LNPC_Init( void );
extern void RBLE_LNPS_Init( void );
extern void RBLE_PASPC_Init( void );
extern void RBLE_PASPS_Init( void );
extern void RBLE_RSCPC_Init( void );
extern void RBLE_RSCPS_Init( void );

/**
 * STRUCTURE DECLARATIONS
 ****************************************************************************************
 */
typedef struct RBLE_CORE_SEL_PROF_HANDLER_t
{
    const struct ke_msg_handler* prof_handler;      /* Message handler table for profiles */
    const uint8_t* handler_size_num;                /* The number of elements in the table */
}RBLE_CORE_SEL_PROF_HANDLER;

typedef struct RBLE_CORE_SEL_CMD_TABLE_t
{
    ke_task_id_t id;                                /* Task ID */
    const RBLE_CMD_TO_RWKE_CMD* prof_cmd_table;     /* Command conversion table for proiles */
    const uint8_t* table_size;                      /* The number of elements in the table */
    RBLE_DISALLOWED_FUNC disallowed_evt;            /* Command disallowed function */
}RBLE_CORE_SEL_CMD_TABLE;

typedef struct RBLE_PROF_VERSION_t
{
    const uint8_t* major_ver;
    const uint8_t* minor_ver;
}RBLE_PROF_VERSION;

/**
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
extern const RBLE_CORE_SEL_PROF_HANDLER rble_core_sel_handler_table_ent[];
extern const rble_prof_init_func_t prof_init_table_ent[];
extern const RBLE_PROF_VERSION rble_prof_version_table_ent[];

extern const struct ke_msg_handler rble_pxpm_handler[];
extern const uint8_t rble_pxpm_handler_size;
extern const uint8_t pxpm_major_ver;
extern const uint8_t pxpm_minor_ver;

extern const struct ke_msg_handler rble_pxpr_handler[];
extern const uint8_t rble_pxpr_handler_size;
extern const uint8_t pxpr_major_ver;
extern const uint8_t pxpr_minor_ver;

extern const struct ke_msg_handler rble_fmpl_handler[];
extern const uint8_t rble_fmpl_handler_size;
extern const uint8_t fmpl_major_ver;
extern const uint8_t fmpl_minor_ver;

extern const struct ke_msg_handler rble_fmpt_handler[];
extern const uint8_t rble_fmpt_handler_size;
extern const uint8_t fmpt_major_ver;
extern const uint8_t fmpt_minor_ver;

extern const struct ke_msg_handler rble_htpc_handler[];
extern const uint8_t rble_htpc_handler_size;
extern const uint8_t htpc_major_ver;
extern const uint8_t htpc_minor_ver;

extern const struct ke_msg_handler rble_htpt_handler[];
extern const uint8_t rble_htpt_handler_size;
extern const uint8_t htpt_major_ver;
extern const uint8_t htpt_minor_ver;

extern const struct ke_msg_handler rble_blpc_handler[];
extern const uint8_t rble_blpc_handler_size;
extern const uint8_t blpc_major_ver;
extern const uint8_t blpc_minor_ver;

extern const struct ke_msg_handler rble_blps_handler[];
extern const uint8_t rble_blps_handler_size;
extern const uint8_t blps_major_ver;
extern const uint8_t blps_minor_ver;

extern const struct ke_msg_handler rble_hghd_handler[];
extern const uint8_t rble_hghd_handler_size;
extern const uint8_t hghd_major_ver;
extern const uint8_t hghd_minor_ver;

extern const struct ke_msg_handler rble_hgbh_handler[];
extern const uint8_t rble_hgbh_handler_size;
extern const uint8_t hgbh_major_ver;
extern const uint8_t hgbh_minor_ver;

extern const struct ke_msg_handler rble_hgrh_handler[];
extern const uint8_t rble_hgrh_handler_size;
extern const uint8_t hgrh_major_ver;
extern const uint8_t hgrh_minor_ver;

extern const struct ke_msg_handler rble_spps_handler[];
extern const uint8_t rble_spps_handler_size;
extern const uint8_t spps_major_ver;
extern const uint8_t spps_minor_ver;

extern const struct ke_msg_handler rble_sppc_handler[];
extern const uint8_t rble_sppc_handler_size;
extern const uint8_t sppc_major_ver;
extern const uint8_t sppc_minor_ver;

extern const struct ke_msg_handler rble_hrpc_handler[];
extern const uint8_t rble_hrpc_handler_size;
extern const uint8_t hrpc_major_ver;
extern const uint8_t hrpc_minor_ver;

extern const struct ke_msg_handler rble_hrps_handler[];
extern const uint8_t rble_hrps_handler_size;
extern const uint8_t hrps_major_ver;
extern const uint8_t hrps_minor_ver;

extern const struct ke_msg_handler rble_cscc_handler[];
extern const uint8_t rble_cscc_handler_size;
extern const uint8_t cscc_major_ver;
extern const uint8_t cscc_minor_ver;

extern const struct ke_msg_handler rble_cscs_handler[];
extern const uint8_t rble_cscs_handler_size;
extern const uint8_t cscs_major_ver;
extern const uint8_t cscs_minor_ver;

extern const struct ke_msg_handler rble_glpc_handler[];
extern const uint8_t rble_glpc_handler_size;
extern const uint8_t glpc_major_ver;
extern const uint8_t glpc_minor_ver;

extern const struct ke_msg_handler rble_glps_handler[];
extern const uint8_t rble_glps_handler_size;
extern const uint8_t glps_major_ver;
extern const uint8_t glps_minor_ver;

extern const struct ke_msg_handler rble_cpps_handler[];
extern const uint8_t rble_cpps_handler_size;
extern const uint8_t cpps_major_ver;
extern const uint8_t cpps_minor_ver;

extern const struct ke_msg_handler rble_cppc_handler[];
extern const uint8_t rble_cppc_handler_size;
extern const uint8_t cppc_major_ver;
extern const uint8_t cppc_minor_ver;

extern const struct ke_msg_handler rble_tips_handler[];
extern const uint8_t rble_tips_handler_size;
extern const uint8_t tips_major_ver;
extern const uint8_t tips_minor_ver;

extern const struct ke_msg_handler rble_tipc_handler[];
extern const uint8_t rble_tipc_handler_size;
extern const uint8_t tipc_major_ver;
extern const uint8_t tipc_minor_ver;

extern const struct ke_msg_handler rble_anps_handler[];
extern const uint8_t rble_anps_handler_size;
extern const uint8_t anps_major_ver;
extern const uint8_t anps_minor_ver;

extern const struct ke_msg_handler rble_anpc_handler[];
extern const uint8_t rble_anpc_handler_size;
extern const uint8_t anpc_major_ver;
extern const uint8_t anpc_minor_ver;

extern const struct ke_msg_handler rble_lnps_handler[];
extern const uint8_t rble_lnps_handler_size;
extern const uint8_t lnps_major_ver;
extern const uint8_t lnps_minor_ver;

extern const struct ke_msg_handler rble_lnpc_handler[];
extern const uint8_t rble_lnpc_handler_size;
extern const uint8_t lnpc_major_ver;
extern const uint8_t lnpc_minor_ver;

extern const struct ke_msg_handler rble_pasc_handler[];
extern const uint8_t rble_pasc_handler_size;
extern const uint8_t pasc_major_ver;
extern const uint8_t pasc_minor_ver;

extern const struct ke_msg_handler rble_pass_handler[];
extern const uint8_t rble_pass_handler_size;
extern const uint8_t pass_major_ver;
extern const uint8_t pass_minor_ver;

extern const struct ke_msg_handler rble_rscs_handler[];
extern const uint8_t rble_rscs_handler_size;
extern const uint8_t rscs_major_ver;
extern const uint8_t rscs_minor_ver;

extern const struct ke_msg_handler rble_rscc_handler[];
extern const uint8_t rble_rscc_handler_size;
extern const uint8_t rscc_major_ver;
extern const uint8_t rscc_minor_ver;
#endif // __RBLE_CORE_CONFIG_INC__
