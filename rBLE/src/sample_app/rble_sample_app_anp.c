/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_anp.c
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

#if (PRF_SEL_ANPC || PRF_SEL_ANPS)
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
#if (PRF_SEL_ANPS)
static void RBLE_APP_ANPS_CallBack( RBLE_ANPS_EVENT *event );                               /* ANP(Server) Call Back */
static BOOL RBLE_ANP_Server_Enable_Test( void );                                            /* A ANP_Server_Enable command is executed */
static BOOL RBLE_ANP_Server_Disable_Test( void );                                           /* A ANP_Server_Disable command is executed */
static BOOL RBLE_ANP_Server_Send_New_Alert_Test( void );                                    /* A ANP_Server_Send_New_Alert command is executed */
static BOOL RBLE_ANP_Server_Send_Unread_Alert_Test( void );                                 /* A ANP_Server_Send_Unread_Alert command is executed */
#endif
#if (PRF_SEL_ANPC)
static void RBLE_APP_ANPC_CallBack( RBLE_ANPC_EVENT *event );                               /* ANP(Client) Call Back */
static BOOL RBLE_ANP_Client_Enable_Test( void );                                            /* A ANP_Client_Enable command is executed */
static BOOL RBLE_ANP_Client_Disable_Test( void );                                           /* A ANP_Client_Disable command is executed */
static BOOL RBLE_ANP_Client_Read_Char_Test( void );                                         /* A ANP_Client_Read_Char command is executed */
static BOOL RBLE_ANP_Client_Write_Alert_Notification_CP_Test( void );                       /* A ANP_Client_Write_Alert_Notification_CP command is executed */
static BOOL RBLE_ANP_Client_Write_Char_Test( void );                                        /* A ANP_Client_Write_Char command is executed */
#endif

#if (PRF_SEL_ANPC)
static void ANP_Alert_Disp (RBLE_ANP_NEW_ALERT_INFO *alert_info);
#endif
static void ANP_Event_Disp(char_t *event_str, RBLE_STATUS status);


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Alert Notification Profile Select Menu */
static const MENU_INFO  Test_Prog_ANP_Menu[] = {
#if (PRF_SEL_ANPS)
    {  1,   RBLE_ANP_Server_Enable_Test,                        NULL,       "1.ANP Server_Enable\n",                        },
    {  2,   RBLE_ANP_Server_Disable_Test,                       NULL,       "2.ANP Server_Disable\n",                       },
    {  3,   RBLE_ANP_Server_Send_New_Alert_Test,                NULL,       "3.ANP Server_Send_New_Alert\n",                },
    {  4,   RBLE_ANP_Server_Send_Unread_Alert_Test,             NULL,       "4.ANP Server_Send_Unread_Alert\n",             },
#endif
#if (PRF_SEL_ANPC)
    {  5,   RBLE_ANP_Client_Enable_Test,                        NULL,       "5.ANP Client_Enable\n",                        },
    {  6,   RBLE_ANP_Client_Disable_Test,                       NULL,       "6.ANP Client_Disable\n",                       },
    {  7,   RBLE_ANP_Client_Read_Char_Test,                     NULL,       "7.ANP Client_Read_Char\n",                     },
    {  8,   RBLE_ANP_Client_Write_Alert_Notification_CP_Test,   NULL,       "8.ANP Client_Write_Alert_Notification_CP\n",   },
    {  9,   RBLE_ANP_Client_Write_Char_Test,                    NULL,       "9.ANP Client_Write_Char\n",                    },
#endif
    {  ESC_KEY, Menu_Exit,                                      NULL,       "ESC Key: Menu exit\n",                         },
    {  0,   NULL,                                               NULL,       NULL,                                           }
};

const SELECT_MENU   Test_Prog_ANP_Select_Menu = {
    "\n-- BLE Sample Program ANP Profile Test Menu --",
    Test_Prog_ANP_Menu
};

#if (PRF_SEL_ANPC)
static RBLE_ANS_CONTENT g_ans = {0};
#endif

/******************************* Program Area *****************************************/
#if (PRF_SEL_ANPS)
/******************************************************************************************/
/**
 *  @brief rBLE ANPS(Server) Call Back
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
static void RBLE_APP_ANPS_CallBack( RBLE_ANPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_ANP_EVENT_SERVER_ENABLE_COMP:
            ANP_Event_Disp("SERVER_ENABLE_COMP", event->param.server_enable.status);
            Connection_Handle_Disp( event->param.server_enable.conhdl );
            break;
        case RBLE_ANP_EVENT_SERVER_DISABLE_COMP:
            printf("\nrBLE ANP EVENT (SERVER_DISABLE_COMP)\n" );
            Connection_Handle_Disp( event->param.server_disable.conhdl );
            break;
        case RBLE_ANP_EVENT_SERVER_ERROR_IND:
            ANP_Event_Disp("SERVER_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_ANP_EVENT_SERVER_SEND_NEW_ALERT_COMP:
            ANP_Event_Disp("SERVER_SEND_NEW_ALERT_COMP", event->param.send_new_alert.status);
            Connection_Handle_Disp( event->param.send_new_alert.conhdl );
            break;
        case RBLE_ANP_EVENT_SERVER_SEND_UNREAD_ALERT_COMP:
            ANP_Event_Disp("SERVER_SEND_UNREAD_ALERT_COMP", event->param.send_unread_alert.status);
            Connection_Handle_Disp( event->param.send_unread_alert.conhdl );
            break;
        case RBLE_ANP_EVENT_SERVER_CHG_ALERT_NTF_CP_IND:
            printf("\nrBLE ANP EVENT (SERVER_CHG_ALERT_NTF_CP_IND)\n" );
            Connection_Handle_Disp( event->param.chg_cp_ind.conhdl );
            printf("Command ID:%d  Category ID:%d\n", event->param.chg_cp_ind.cp_info.command_id, event->param.chg_cp_ind.cp_info.category_id);
            break;
        case RBLE_ANP_EVENT_SERVER_CFG_NTF_IND:
            printf("\nrBLE ANP EVENT (SERVER_CFG_NTF_IND)\n" );
            Connection_Handle_Disp( event->param.cfg_ntf_ind.conhdl );
            printf("char:%d  cfg:%d\n", event->param.cfg_ntf_ind.char_code, event->param.cfg_ntf_ind.cfg_val);
            break;
        case RBLE_ANP_EVENT_SERVER_COMMAND_DISALLOWED_IND:
            ANP_Event_Disp("SERVER_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A ANP_Server_Enable command is executed.
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
static BOOL RBLE_ANP_Server_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t     select = 0;
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_AUTH|RBLE_SVC_SEC_ENC,
    };
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;
    RBLE_ANP_SERVER_PARAM param = {0};

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_ANP_Server_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_ANPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A ANP_Server_Disable command is executed.
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
static BOOL RBLE_ANP_Server_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_ANP_Server_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A ANP_Server_Send_New_Alert command is executed.
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
static BOOL RBLE_ANP_Server_Send_New_Alert_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_ANP_NEW_ALERT_INFO alert_info = {
        0,
        2,
        12,
        "from Renesas"
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            alert_info.category_id = (uint8_t)RBLE_Parameter_Arg[ 0 ];
            if( RBLE_Parameter_Cnt > 1 ) {
                alert_info.alert_num = (uint8_t)RBLE_Parameter_Arg[ 1 ];
            }
        }
        /* API Call */
        Ret_Status = RBLE_ANP_Server_Send_New_Alert( Connection_Handle, &alert_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A ANP_Server_Send_Unread_Alert command is executed.
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
static BOOL RBLE_ANP_Server_Send_Unread_Alert_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_ANP_UNREAD_ALERT_INFO alert_status = {0};

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            alert_status.category_id = (uint8_t)RBLE_Parameter_Arg[ 0 ];
            if( RBLE_Parameter_Cnt > 1 ) {
                alert_status.unread_count = (uint8_t)RBLE_Parameter_Arg[ 1 ];
            }
        }
        /* API Call */
        Ret_Status = RBLE_ANP_Server_Send_Unread_Alert( Connection_Handle, &alert_status );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_ANPS)

#if (PRF_SEL_ANPC)
/******************************************************************************************/
/**
 *  @brief rBLE ANPC(Client) Call Back
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
static void RBLE_APP_ANPC_CallBack( RBLE_ANPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_ANP_EVENT_CLIENT_ENABLE_COMP:
            ANP_Event_Disp("CLIENT_ENABLE_COMP", event->param.client_enable.status);
            Connection_Handle_Disp( event->param.client_enable.conhdl );
            printf("\nAlert Notification Service\n" );
            printf(" Start Handle = 0x%04x\n", event->param.client_enable.ans.shdl);
            printf(" End Handle   = 0x%04x\n", event->param.client_enable.ans.ehdl);
            printf(" supp_new_alert_char_hdl    = 0x%04x\n", event->param.client_enable.ans.supp_new_alert_char_hdl);
            printf(" supp_new_alert_val_hdl     = 0x%04x\n", event->param.client_enable.ans.supp_new_alert_val_hdl);
            printf(" supp_new_alert_prop        = 0x%04x\n", event->param.client_enable.ans.supp_new_alert_prop);
            printf(" new_alert_char_hdl         = 0x%04x\n", event->param.client_enable.ans.new_alert_char_hdl);
            printf(" new_alert_val_hdl          = 0x%04x\n", event->param.client_enable.ans.new_alert_val_hdl);
            printf(" new_alert_cfg_hdl          = 0x%04x\n", event->param.client_enable.ans.new_alert_cfg_hdl);
            printf(" new_alert_prop             = 0x%04x\n", event->param.client_enable.ans.new_alert_prop);
            printf(" supp_unread_alert_char_hdl = 0x%04x\n", event->param.client_enable.ans.supp_unread_alert_char_hdl);
            printf(" supp_unread_alert_val_hdl  = 0x%04x\n", event->param.client_enable.ans.supp_unread_alert_val_hdl);
            printf(" supp_unread_alert_prop     = 0x%04x\n", event->param.client_enable.ans.supp_unread_alert_prop);
            printf(" unread_alert_char_hdl      = 0x%04x\n", event->param.client_enable.ans.unread_alert_char_hdl);
            printf(" unread_alert_val_hdl       = 0x%04x\n", event->param.client_enable.ans.unread_alert_val_hdl);
            printf(" unread_alert_cfg_hdl       = 0x%04x\n", event->param.client_enable.ans.unread_alert_cfg_hdl);
            printf(" unread_alert_prop          = 0x%04x\n", event->param.client_enable.ans.unread_alert_prop);
            printf(" alert_ntf_cp_char_hdl      = 0x%04x\n", event->param.client_enable.ans.alert_ntf_cp_char_hdl);
            printf(" alert_ntf_cp_val_hdl       = 0x%04x\n", event->param.client_enable.ans.alert_ntf_cp_val_hdl);
            printf(" alert_ntf_cp_prop          = 0x%04x\n", event->param.client_enable.ans.alert_ntf_cp_prop);

            g_ans = event->param.client_enable.ans;
            break;
        case RBLE_ANP_EVENT_CLIENT_DISABLE_COMP:
            ANP_Event_Disp("CLIENT_DISABLE_COMP", event->param.client_disable.status);
            Connection_Handle_Disp( event->param.client_disable.conhdl );
            break;
        case RBLE_ANP_EVENT_CLIENT_ERROR_IND:
            ANP_Event_Disp("CLIENT_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_ANP_EVENT_CLIENT_NEW_ALERT_NTF:
            printf("\nrBLE ANP EVENT (CLIENT_NEW_ALERT_NTF)\n" );
            Connection_Handle_Disp( event->param.new_alert_ntf.conhdl );
            ANP_Alert_Disp( &event->param.new_alert_ntf.new_alert );
            break;
        case RBLE_ANP_EVENT_CLIENT_UNREAD_ALERT_NTF:
            printf("\nrBLE ANP EVENT (CLIENT_UNREAD_ALERT_NTF)\n" );
            Connection_Handle_Disp( event->param.unread_alert_ntf.conhdl );
            printf("Category ID:%d  Unread Count:%d\n", event->param.unread_alert_ntf.unread_alert.category_id, event->param.unread_alert_ntf.unread_alert.unread_count);
            break;
        case RBLE_ANP_EVENT_CLIENT_READ_CHAR_RESPONSE:
            ANP_Event_Disp("CLIENT_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            event->param.rd_char_resp.data.data[event->param.rd_char_resp.data.len] = '\0';
            printf("(%s)\n", event->param.rd_char_resp.data.data);
            break;
        case RBLE_ANP_EVENT_CLIENT_WRITE_CHAR_RESPONSE:
            ANP_Event_Disp("CLIENT_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_ANP_EVENT_CLIENT_COMMAND_DISALLOWED_IND:
            ANP_Event_Disp("CLIENT_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A ANP_Client_Enable command is executed.
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
static BOOL RBLE_ANP_Client_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_ans.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_ANP_Client_Enable( Connection_Handle, con_type, &g_ans, RBLE_APP_ANPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A ANP_Client_Disable command is executed.
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
static BOOL RBLE_ANP_Client_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_ANP_Client_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A ANP_Client_Read_Char command is executed.
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
static BOOL RBLE_ANP_Client_Read_Char_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t char_code = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            char_code = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_ANP_Client_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A ANP_Client_Write_Alert_Notification_CP command is executed.
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
static BOOL RBLE_ANP_Client_Write_Alert_Notification_CP_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_ANP_ALERT_NTF_CP alert_ntf_cp = {0};

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            alert_ntf_cp.command_id = (uint8_t)RBLE_Parameter_Arg[ 0 ];
            if( RBLE_Parameter_Cnt > 1 ) {
                alert_ntf_cp.category_id = (uint8_t)RBLE_Parameter_Arg[ 1 ];
            }
        }
        /* API Call */
        Ret_Status = RBLE_ANP_Client_Write_Alert_Notification_CP( Connection_Handle, &alert_ntf_cp );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A ANP_Client_Write_Char command is executed.
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
static BOOL RBLE_ANP_Client_Write_Char_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t char_code = 0;
    uint16_t cfg_val = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            char_code = (uint8_t)RBLE_Parameter_Arg[ 0 ];
            if( RBLE_Parameter_Cnt > 1 ) {
                cfg_val = (uint8_t)RBLE_Parameter_Arg[ 1 ];
            }
        }
        /* API Call */
        Ret_Status = RBLE_ANP_Client_Write_Char( Connection_Handle, char_code, cfg_val );
        printf( " char_code = %d, cfg_val = 0x%04X\n", char_code, cfg_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief Display Alert Notify
 *
 *  @b Contents of management:
 *
 *  @param[in] alert_info     alert information
 *
 *  @retval none
 */
/******************************************************************************************/
static void ANP_Alert_Disp (RBLE_ANP_NEW_ALERT_INFO *alert_info)
{
    uint8_t i;
    
    printf("Category Id:%d\n", alert_info->category_id);
    printf("Alert Num  :%d\n", alert_info->alert_num);
    
    printf("Text:");
    for(i = 0; alert_info->text_size > i ; i++ )
    {
        printf("%c",alert_info->text[i]);
    }
    printf("\n");
}
#endif  //#if (PRF_SEL_ANPC)

/******************************************************************************************/
/**
 *  @brief rBLE ANP Event Disp
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
static void ANP_Event_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE ANP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}



#endif  //#if (PRF_SEL_ANPC || PRF_SEL_ANPS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
