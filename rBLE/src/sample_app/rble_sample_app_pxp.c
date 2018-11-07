/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_pxp.c
 *
 * @brief   rBLE Sample Application
 *
 * Copyright(C) 2016 Renesas Electronics Corporation
 *
 * $Rev: 9242 $
 *
 ****************************************************************************************
 */
#include "rwble_config.h"
#include "arch.h"

#ifdef CFG_SAMPLE
#ifdef CONFIG_EMBEDDED

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

#include    "rble.h"
#include    "rble_app.h"
#include    "menu_sel.h"
#include    "console.h"

#include    "prf_sel.h"

#if (PRF_SEL_PXPM || PRF_SEL_PXPR)
/**
 * DEFINES
 ****************************************************************************************
 */


/**
 * TYPE DEFINES
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/* ------------------------- Proximity Profile----------------------------------------*/
#if (PRF_SEL_PXPR)
static void RBLE_APP_PXPR_CallBack( RBLE_PXPR_EVENT *event );                               /* PXP(Reporter) Call Back */
static BOOL RBLE_PXP_Reporter_Enable_Test( void );                                          /* A PXP_Reporter_Enable command is executed. */
static BOOL RBLE_PXP_Reporter_Disable_Test( void );                                         /* A PXP_Reporter_Disable command is executed. */
#endif

#if (PRF_SEL_PXPM)
static void RBLE_APP_PXPM_CallBack( RBLE_PXPM_EVENT *event );                               /* PXP(Monitor) Call Back */
static BOOL RBLE_PXP_Monitor_Enable_Test( void );                                           /* A PXP_Monitor_Enable command is executed. */
static BOOL RBLE_PXP_Monitor_Disable_Test( void );                                          /* A PXP_Monitor_Disable command is executed. */
static BOOL RBLE_PXP_Monitor_Get_Alert_Level_Test( void );                                  /* A PXP_Monitor_Get_Alert_Level command is executed. */
static BOOL RBLE_PXP_Monitor_Set_Alert_Level_Test( void );                                  /* A PXP_Monitor_Set_Alert_Level command is executed. */
static BOOL RBLE_PXP_Monitor_Get_Tx_Power_Test( void );                                     /* A PXP_Monitor_Get_Tx_Power command is executed. */
#endif

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Proximity Profile Select Menu */
static const MENU_INFO  Test_Prog_PXP_Menu[] = {
#if (PRF_SEL_PXPR)
    {  1,   RBLE_PXP_Reporter_Enable_Test,              NULL,       "1.PXP Reporter_Enable\n",          },
    {  2,   RBLE_PXP_Reporter_Disable_Test,             NULL,       "2.PXP Reporter_Disable\n",         },
#endif
#if (PRF_SEL_PXPM)
    {  3,   RBLE_PXP_Monitor_Enable_Test,               NULL,       "3.PXP Monitor_Enable\n",           },
    {  4,   RBLE_PXP_Monitor_Disable_Test,              NULL,       "4.PXP Monitor_Disable\n",          },
    {  5,   RBLE_PXP_Monitor_Get_Alert_Level_Test,      NULL,       "5.PXP Monitor_Get_Alert_Level\n",  },
    {  6,   RBLE_PXP_Monitor_Set_Alert_Level_Test,      NULL,       "6.PXP Monitor_Set_Alert_Level\n",  },
    {  7,   RBLE_PXP_Monitor_Get_Tx_Power_Test,         NULL,       "7.PXP Monitor_Get_Tx_Power\n",     },
#endif
    {  ESC_KEY, Menu_Exit,                              NULL,       "ESC Key: Menu exit\n",             },
    {  0,   NULL,                                       NULL,       NULL,                               }
};

const SELECT_MENU   Test_Prog_PXP_Select_Menu = {
    "\n-- BLE Sample Program Proximity Profile Test Menu --",
    Test_Prog_PXP_Menu
};

#if (PRF_SEL_PXPM)
static RBLE_PROXI_MON_PARAM g_pxp_service = {
    {
        0x00,       /* LlsShdl */
        0x00,       /* LlsEhdl */
        0x00,       /* LlsCharHdl */
        0x00,       /* LlsValHdl */
        0x00,       /* LlsProp */
        0x00        /* LlsValue */
    },
    {
        0x00,       /* IasShdl */
        0x00,       /* IasEhdl */
        0x00,       /* IasCharHdl */
        0x00,       /* IasValHdl */
        0x00,       /* IasProp */
        0x00        /* IasValue */
    },
    {
        0x00,       /* TpsShdl */
        0x00,       /* TpsEhdl */
        0x00,       /* TpsTxpwLvlChHdl */
        0x00,       /* TpsTxpwLvlValHdl */
        0x00,       /* TpsTxpwLvlCfgHdl */
        0x00,       /* TpsTxpwLvlProp */
        0x00        /* TpsTxpwLvl */
    }
};
#endif


/******************************* Program Area *****************************************/

#if (PRF_SEL_PXPR)
/* ------------------------- Proximity Profile---------------------------------------*/
/******************************************************************************************/
/**
 *  @brief rBLE PXP(Reporter) Call Back
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event          event parameter
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_PXPR_CallBack( RBLE_PXPR_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type ) {
        case RBLE_PXP_EVENT_REPORTER_ENABLE_COMP:
            printf( "\nrBLE PXP EVENT (REPORTER_ENABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.report_enable.status ) );
            Connection_Handle_Disp( event->param.report_enable.conhdl );
            break;
        case RBLE_PXP_EVENT_REPORTER_DISABLE_COMP:
            printf( "\nrBLE PXP EVENT (REPORTER_DISABLE_COMP)\n" );
            Connection_Handle_Disp( event->param.report_disable.conhdl );
            printf( "lls_alert_lvl = %d\n", event->param.report_disable.lls_alert_lvl );
            break;
        case RBLE_PXP_EVENT_REPORTER_ALERT_IND:
            printf( "\nrBLE PXP EVENT (REPORTER_ALERT_IND)\n" );
            Connection_Handle_Disp( event->param.report_alert_ind.conhdl );
            printf( "alert_lvl = %d\n", event->param.report_alert_ind.alert_lvl );
            break;
        case RBLE_PXP_EVENT_REPORTER_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE PXP EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A PXP_Reporter_Enable command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_PXP_Reporter_Enable_Test( void )
{
    /* Alert_Level */
    uint8_t alert_lvl = RBLE_SVC_ALERT_NONE;      /* No alert */
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_UNAUTH|RBLE_SVC_SEC_ENC,
    };

    RBLE_STATUS Ret_Status;
    uint8_t     select = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_PXP_Reporter_Enable( Connection_Handle, alert_lvl, sec_lvl[select], RBLE_APP_PXPR_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PXP_Reporter_Disable command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_PXP_Reporter_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_PXP_Reporter_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_PXPR)


#if (PRF_SEL_PXPM)
/******************************************************************************************/
/**
 *  @brief rBLE PXP(Monitor) Call Back
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event          event parameter
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_PXPM_CallBack( RBLE_PXPM_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type ) {
        case RBLE_PXP_EVENT_MONITOR_ENABLE_COMP:
            printf( "\nrBLE PXP EVENT (MONITOR_ENABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.monitor_enable.status ) );
            Connection_Handle_Disp( event->param.monitor_enable.conhdl );
            printf("* Link Loss Service\n");
            printf(" Start Handle    = 0x%04X\n", event->param.monitor_enable.lls.shdl );
            printf(" End Handle      = 0x%04X\n\n", event->param.monitor_enable.lls.ehdl );
            printf(" Alert level char handle = 0x%04X\n", event->param.monitor_enable.lls.alert_lvl.char_hdl);
            printf(" alert Level value handle= 0x%04X\n", event->param.monitor_enable.lls.alert_lvl.val_hdl);
            printf(" Alert level properties  = 0x%02X\n", event->param.monitor_enable.lls.alert_lvl.prop);
            printf(" Alert value             = 0x%02X\n\n", event->param.monitor_enable.lls.alert_lvl.value);
            printf("* Immediate Alert service\n");
            printf(" Start Handle    = 0x%04X\n",   event->param.monitor_enable.ias.shdl );
            printf(" End Handle      = 0x%04X\n\n", event->param.monitor_enable.ias.ehdl );
            printf(" Alert level char handle = 0x%04X\n", event->param.monitor_enable.ias.alert_lvl.char_hdl);
            printf(" alert Level value handle= 0x%04X\n", event->param.monitor_enable.ias.alert_lvl.val_hdl);
            printf(" Alert level properties  = 0x%02X\n", event->param.monitor_enable.ias.alert_lvl.prop);
            printf(" Alert value             = 0x%02X\n\n", event->param.monitor_enable.ias.alert_lvl.value);
            printf("* Tx Power Service\n");
            printf(" Start Handle    = 0x%04X\n",   event->param.monitor_enable.tps.shdl );
            printf(" End Handle      = 0x%04X\n\n", event->param.monitor_enable.tps.ehdl );
            printf(" txpw_lvl_char_hdl= 0x%04X\n",  event->param.monitor_enable.tps.txpw_lvl_char_hdl);
            printf(" txpw_lvl_val_hdl = 0x%04X\n",  event->param.monitor_enable.tps.txpw_lvl_val_hdl );
            printf(" txpw_lvl_cfg_hdl = 0x%04X\n",  event->param.monitor_enable.tps.txpw_lvl_cfg_hdl );
            printf(" txpw_lvl_prop    = 0x%02X\n",  event->param.monitor_enable.tps.txpw_lvl_prop    );
            printf(" txpw_lvl         = 0x%02X\n\n",event->param.monitor_enable.tps.txpw_lvl         );

            g_pxp_service.lls = event->param.monitor_enable.lls;
            g_pxp_service.ias = event->param.monitor_enable.ias;
            g_pxp_service.tps = event->param.monitor_enable.tps;
            break;
        case RBLE_PXP_EVENT_MONITOR_DISABLE_COMP:
            printf( "\nrBLE PXP EVENT (MONITOR_DISABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.monitor_disale.status ) );
            Connection_Handle_Disp( event->param.monitor_disale.conhdl );
            break;
        case RBLE_PXP_EVENT_MONITOR_ERROR_IND:
            printf( "\nrBLE PXP EVENT (MONITOR_ERROR_IND) Status(%s)\n", Get_Status_Str( event->param.monitor_error_ind.status ) );
            Connection_Handle_Disp( event->param.monitor_error_ind.conhdl );
            break;
        case RBLE_PXP_EVENT_MONITOR_READ_CHAR_RESPONSE:
            printf( "\nrBLE PXP EVENT (MONITOR_READ_CHAR_RESPONSE) Status(%s)\n", Get_Status_Str( event->param.rd_char_resp.att_code ) );
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            break;
        case RBLE_PXP_EVENT_MONITOR_WRITE_CHAR_RESPONSE:
            printf( "\nrBLE PXP EVENT (MONITOR_WRITE_CHAR_RESPONSE) Status(%s)\n", Get_Status_Str( event->param.wr_char_resp.att_code ) );
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_PXP_EVENT_MONITOR_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE PXP EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A PXP_Monitor_Enable command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_PXP_Monitor_Enable_Test( void )
{
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;    /* Discovery type connection */
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0x0000 != g_pxp_service.lls.shdl) || (0x0000 != g_pxp_service.ias.shdl))
        {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_PXP_Monitor_Enable( Connection_Handle, con_type, &g_pxp_service, RBLE_APP_PXPM_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PXP_Monitor_Disable command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_PXP_Monitor_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_PXP_Monitor_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PXP_Monitor_Get_Alert_Level command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_PXP_Monitor_Get_Alert_Level_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_PXP_Monitor_Get_Alert_Level( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PXP_Monitor_Set_Alert_Level command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_PXP_Monitor_Set_Alert_Level_Test( void )
{
    int32_t                         Parameter_No;
    /* SvcCode */
//  uint8_t svc_code = RBLE_PROXM_SET_LK_LOSS_ALERT;    /*=0 LLS Alert Level */
    uint8_t svc_code = RBLE_PROXM_SET_IMMDT_ALERT;  /*=1 IAS Alert Level */

    /* Alert_Level */
//  uint8_t lvl = RBLE_SVC_ALERT_NONE;      /*=0 No alert */
    uint8_t lvl = RBLE_SVC_ALERT_MILD;      /*=1 Mild alert */
//  uint8_t lvl = RBLE_SVC_ALERT_HIGH;      /*=2 High alert */

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = ILLEGAL_NO;
        }
        switch( Parameter_No ){
        case 0:
        case ILLEGAL_NO:
        default:
            Parameter_No = 0;
            svc_code    = RBLE_PROXM_SET_LK_LOSS_ALERT; /*=0 LLS Alert Level */
            lvl         = RBLE_SVC_ALERT_MILD;          /*=1 Mild alert */
            break;
        case 1:
            svc_code    = RBLE_PROXM_SET_IMMDT_ALERT;   /*=1 IAS Alert Level */
            lvl         = RBLE_SVC_ALERT_MILD;          /*=1 Mild alert */
            break;
        case 2:
            svc_code    = RBLE_PROXM_SET_IMMDT_ALERT;   /*=1 IAS Alert Level */
            lvl         = RBLE_SVC_ALERT_NONE;          /*=0 No alert */
            break;
        }
        Select_Param_No_Disp( Parameter_No );
        
        /* API Call */
        Ret_Status = RBLE_PXP_Monitor_Set_Alert_Level( Connection_Handle, svc_code, lvl );
        printf( " code = %d, lvl = %d\n", svc_code, lvl );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PXP_Monitor_Get_Tx_Power command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
static BOOL RBLE_PXP_Monitor_Get_Tx_Power_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_PXP_Monitor_Get_Tx_Power( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_PXPM)


#endif  //#if (PRF_SEL_PXPM || PRF_SEL_PXPR)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
