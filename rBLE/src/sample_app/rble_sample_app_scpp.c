/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_scpp.c
 *
 * @brief   rBLE Sample Application
 *
 * Copyright(C) 2016 Renesas Electronics Corporation
 *
 * $Rev: 9242 $
 *
 ****************************************************************************************
 */

/**
 * INCLUDE FILES
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

#if (PRF_SEL_SPPC || PRF_SEL_SPPS)
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
#if (PRF_SEL_SPPS)
static void RBLE_APP_SPPS_CallBack( RBLE_SPPS_EVENT *event );                               /* SCPP(Client) Call Back */
static BOOL RBLE_SPP_Server_Enable_Test( void );                                            /* A SPP_Server_Enable command is executed. */
static BOOL RBLE_SPP_Server_Disable_Test( void );                                           /* A SPP_Server_Disable command is executed. */
static BOOL RBLE_SPP_Server_Send_Refresh_Test( void );                                      /* A SPP_Server_Send_Refresh command is executed. */
#endif
#if (PRF_SEL_SPPC)
static void RBLE_APP_SPPC_CallBack( RBLE_SPPC_EVENT *event );                               /* SCPP(Server) Call Back */
static BOOL RBLE_SPP_Client_Enable_Test( void );                                            /* A SPP_Client_Enable command is executed. */
static BOOL RBLE_SPP_Client_Disable_Test( void );                                           /* A SPP_Client_Disable command is executed. */
static BOOL RBLE_SPP_Client_Write_Char_Test( void );                                        /* A SPP_Client_Write_Char command is executed. */
static BOOL RBLE_SPP_Client_Write_Interval_Test( void );                                    /* A SPP_Client_Write_Interval command is executed. */
#endif

static void SPP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status);

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Scan Parameters Profile Select Menu */
static const MENU_INFO  Test_Prog_SCPP_Menu[] = {
#if (PRF_SEL_SPPS)
    {  1,   RBLE_SPP_Server_Enable_Test,                NULL,       "1.SPP_Server_Enable\n",            },
    {  2,   RBLE_SPP_Server_Disable_Test,               NULL,       "2.SPP_Server_Disabel\n",           },
    {  3,   RBLE_SPP_Server_Send_Refresh_Test,          NULL,       "3.SPP_Server_Send_Refresh\n",      },
#endif
#if (PRF_SEL_SPPC)
    {  4,   RBLE_SPP_Client_Enable_Test,                NULL,       "4.SPP_Client_Enable\n",            },
    {  5,   RBLE_SPP_Client_Disable_Test,               NULL,       "5.SPP_Client_Disable\n",           },
    {  6,   RBLE_SPP_Client_Write_Char_Test,            NULL,       "6.SPP_Client_Write_Char\n",        },
    {  7,   RBLE_SPP_Client_Write_Interval_Test,        NULL,       "7.SPP_Client_Write_Interval\n",    },
#endif
    {  ESC_KEY, Menu_Exit,                              NULL,       "ESC Key: Menu exit\n",             },
    {  0,   NULL,                                       NULL,       NULL,                               }
};

const SELECT_MENU   Test_Prog_SCPP_Select_Menu = {
    "\n-- BLE Sample Program Scan Parameters Profile Test Menu --",
    Test_Prog_SCPP_Menu
};

/******************************* Program Area *****************************************/
#if (PRF_SEL_SPPS)
/******************************************************************************************/
/**
 *  @brief Scan Parameters(Server) Profile Call Back
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
static void RBLE_APP_SPPS_CallBack( RBLE_SPPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_SPP_EVENT_SERVER_ENABLE_COMP:             /* Scan Server enable completion event */
            SPP_Event_Status_Disp("SERVER_ENABLE_COMP", event->param.server_enable.status);
            Connection_Handle_Disp( event->param.server_enable.conhdl );
            break;
        case RBLE_SPP_EVENT_SERVER_DISABLE_COMP:            /* Scan Server disable completion event */
            SPP_Event_Status_Disp("SERVER_DISABLE_COMP", event->param.server_disable.status);
            Connection_Handle_Disp( event->param.server_disable.conhdl );
            printf( "  Refresh Ntf Cfg = %04X(%s)\n", event->param.server_disable.device_info.s_refresh_ntf_en, ( event->param.server_disable.device_info.s_refresh_ntf_en == RBLE_PRF_STOP_NTFIND ) ? "STOP" : "START_NTF_IND" );
            break;
        case RBLE_SPP_EVENT_SERVER_CFG_INDNTF_IND:          /* Configured value change indication event */
            printf( "\nrBLE SPP EVENT (SERVER_CFG_INDNTF_IND)\n" );
            Connection_Handle_Disp( event->param.scans_cfg_indntf_ind.conhdl );
            printf( "  Cfg Value = %04X(%s)\n", event->param.scans_cfg_indntf_ind.cfg_val, ( event->param.scans_cfg_indntf_ind.cfg_val == RBLE_PRF_STOP_NTFIND ) ? "STOP" : "START_NTF_IND" );
            break;
        case RBLE_SPP_EVENT_SERVER_INTERVAL_CHG_EVT:        /* Scan interval window notification event */
            printf( "\nrBLE SPP EVENT (SERVER_INTERVAL_CHG_EVT)\n" );
            Connection_Handle_Disp( event->param.interval_chg_evt.conhdl );
            printf( "  interval value  = %04X\n", event->param.interval_chg_evt.scan_param.le_scan_interval );
            printf( "  window value    = %04X\n", event->param.interval_chg_evt.scan_param.le_scan_window );
            break;
        case RBLE_SPP_EVENT_SERVER_SEND_REFRESH_COMP:       /* Scan refresh request send completion event */
            SPP_Event_Status_Disp("SERVER_SEND_REFRESH_COMP", event->param.send_refresh.status);
            Connection_Handle_Disp( event->param.send_refresh.conhdl );
            break;
        case RBLE_SPP_EVENT_SERVER_COMMAND_DISALLOWED_IND:  /* Command disallowed indication event */
            SPP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "  opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A SPP_Server_Enable command is executed.
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
static BOOL RBLE_SPP_Server_Enable_Test( void )
{
    RBLE_STATUS             Ret_Status;
    uint8_t     select = 0;
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_UNAUTH|RBLE_SVC_SEC_ENC,
    };
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;
    RBLE_SPP_SERVER_PARAM   param = { 0 };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_SPP_Server_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_SPPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SPP_Server_Disable command is executed.
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
static BOOL RBLE_SPP_Server_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SPP_Server_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SPP_Server_Send_Refresh command is executed.
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
static BOOL RBLE_SPP_Server_Send_Refresh_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint8_t         s_refresh_val = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SPP_Server_Send_Refresh( Connection_Handle, s_refresh_val );
        printf( " refresh value = %d\n", s_refresh_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_SPPS)

#if (PRF_SEL_SPPC)
/******************************************************************************************/
/**
 *  @brief Scan Parameters(Client) Profile Call Back
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
static void RBLE_APP_SPPC_CallBack( RBLE_SPPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_SPP_EVENT_CLIENT_ENABLE_COMP:             /* Scan Client enable completion event */
            SPP_Event_Status_Disp("CLIENT_ENABLE_COMP", event->param.client_enable.status);
            Connection_Handle_Disp( event->param.client_enable.conhdl );
            printf("* Scan Parameters Service\n");
            printf(" Start Handle         = 0x%04X\n",   event->param.client_enable.sps.shdl );
            printf(" End Handle           = 0x%04X\n\n", event->param.client_enable.sps.ehdl );
            printf(" intv_window_char_hdl = 0x%04X\n",   event->param.client_enable.sps.intv_window_char_hdl );
            printf(" intv_window_val_hdl  = 0x%04X\n",   event->param.client_enable.sps.intv_window_val_hdl  );
            printf(" intv_window_prop     = 0x%02X\n\n", event->param.client_enable.sps.intv_window_prop     );
            printf(" refresh_char_hdl     = 0x%04X\n",   event->param.client_enable.sps.refresh_char_hdl     );
            printf(" refresh_val_hdl      = 0x%04X\n",   event->param.client_enable.sps.refresh_val_hdl      );
            printf(" refresh_cfg_hdl      = 0x%04X\n",   event->param.client_enable.sps.refresh_cfg_hdl      );
            printf(" refresh_prop         = 0x%02X\n\n", event->param.client_enable.sps.refresh_prop         );
            break;
        case RBLE_SPP_EVENT_CLIENT_DISABLE_COMP:            /* Scan Client disable completion event */
            SPP_Event_Status_Disp("CLIENT_DISABLE_COMP", event->param.client_disable.status);
            Connection_Handle_Disp( event->param.client_disable.conhdl );
            break;
        case RBLE_SPP_EVENT_CLIENT_ERROR_IND:               /* Scan Client error indication event */
            SPP_Event_Status_Disp("CLIENT_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_SPP_EVENT_CLIENT_WRITE_CHAR_RESPONSE:     /* Characteristic write request response event */
            SPP_Event_Status_Disp("CLIENT_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_SPP_EVENT_CLIENT_COMMAND_DISALLOWED_IND:  /* Command disallowed indication event */
            SPP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "  opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A SPP_Client_Enable command is executed.
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
static BOOL RBLE_SPP_Client_Enable_Test( void )
{
    RBLE_STATUS                     Ret_Status;
    uint8_t                         con_type = RBLE_PRF_CON_DISCOVERY;
    RBLE_SPS_CONTENT                sps = { 0 };
    RBLE_SCANS_INTV_WINDOW_PARAM    s_intv_window = { 0 };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SPP_Client_Enable( Connection_Handle, con_type, &sps, &s_intv_window, RBLE_APP_SPPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SPP_Client_Disable command is executed.
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
static BOOL RBLE_SPP_Client_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SPP_Client_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SPP_Client_Write_Char command is executed.
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
static BOOL RBLE_SPP_Client_Write_Char_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;
    uint16_t    cfg_val = RBLE_PRF_START_NTF;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = 0;
        }
        switch( Parameter_No ){
        case 0:
        default:
            cfg_val = RBLE_PRF_STOP_NTFIND;
            break;
        case 1:
            cfg_val = RBLE_PRF_START_NTF;
            break;
        }

        /* API Call */
        Ret_Status = RBLE_SPP_Client_Write_Char( Connection_Handle, cfg_val );
        printf( "  Cfg Value = %04X(%s)\n", cfg_val, ( cfg_val == RBLE_PRF_STOP_NTFIND ) ? "STOP" : "START_NTF_IND" );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SPP_Client_Write_Interval command is executed.
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
static BOOL RBLE_SPP_Client_Write_Interval_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;
    RBLE_SCANS_INTV_WINDOW_PARAM        s_intv_window = { 48, 48 };

    /*
     *  scan interval value
     *   Range: 0x0004 to 0x4000
     *   Time = N * 0.625 msec
     *   Time Range: 2.5 msec to 10240 msec
     *
     *  scan window value
     *   Range: 0x0004 to 0x4000
     *   Time = N * 0.625 msec
     *   Time Range: 2.5 msec to 10240 msec
     */

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = 0;
        }
        if ( 0 != Parameter_No ) {
            s_intv_window.le_scan_interval = 96;               /* scan interval value */
            s_intv_window.le_scan_window   = 96;               /* scan window value */
        }
        /* API Call */
        Ret_Status = RBLE_SPP_Client_Write_Interval( Connection_Handle, &s_intv_window );
        printf( "  interval value = %04X, window value = %04X\n", s_intv_window.le_scan_interval, s_intv_window.le_scan_window );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_SPPC)

/******************************************************************************************/
/**
 *  @brief rBLE SCP Event Status Disp
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event_str      event strings
 *  @param[in] status          status
 *
 *  @retval
 */
/******************************************************************************************/
static void SPP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE SPP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}


#endif  //#if (PRF_SEL_SPPC || PRF_SEL_SPPS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
