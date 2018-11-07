/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_pasp.c
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

#if (PRF_SEL_PASC || PRF_SEL_PASS)
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
#if (PRF_SEL_PASS)
static void RBLE_APP_PASPS_CallBack( RBLE_PASPS_EVENT *event );                             /* PASP(Server) Call Back */
static BOOL RBLE_PASP_Server_Enable_Test( void );                                           /* A PASP_Server_Enable command is executed */
static BOOL RBLE_PASP_Server_Disable_Test( void );                                          /* A PASP_Server_Disable command is executed */
static BOOL RBLE_PASP_Server_Send_Alert_Status_Test( void );                                /* A PASP_Server_Send_Alert_Status command is executed */
static BOOL RBLE_PASP_Server_Send_Ringer_Setting_Test( void );                              /* A PASP_Server_Send_Ringer_Setting command is executed */
#endif
#if (PRF_SEL_PASC)
static void RBLE_APP_PASPC_CallBack( RBLE_PASPC_EVENT *event );                             /* PASP(Client) Call Back */
static BOOL RBLE_PASP_Client_Enable_Test( void );                                           /* A PASP_Client_Enable command is executed */
static BOOL RBLE_PASP_Client_Disable_Test( void );                                          /* A PASP_Client_Disable command is executed */
static BOOL RBLE_PASP_Client_Read_Char_Test( void );                                        /* A PASP_Client_Read_Char command is executed */
static BOOL RBLE_PASP_Client_Write_Ringer_Control_Point_Test( void );                       /* A PASP_Client_Write_Ringer_Control_Point command is executed */
static BOOL RBLE_PASP_Client_Write_Char_Test( void );                                       /* A PASP_Client_Write_Char command is executed */
#endif

static void PASP_Event_Disp(char_t *event_str, RBLE_STATUS status);


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Phone Alert Status Profile Select Menu */
static const MENU_INFO  Test_Prog_PASP_Menu[] = {
#if (PRF_SEL_PASS)
    {  1,   RBLE_PASP_Server_Enable_Test,                       NULL,       "1.PASP Server_Enable\n",                       },
    {  2,   RBLE_PASP_Server_Disable_Test,                      NULL,       "2.PASP Server_Disable\n",                      },
    {  3,   RBLE_PASP_Server_Send_Alert_Status_Test,            NULL,       "3.PASP Server_Send_Alert_Status\n",            },
    {  4,   RBLE_PASP_Server_Send_Ringer_Setting_Test,          NULL,       "4.PASP Server_Send_Ringer_Setting\n",          },
#endif
#if (PRF_SEL_PASC)
    {  5,   RBLE_PASP_Client_Enable_Test,                       NULL,       "5.PASP Client_Enable\n",                       },
    {  6,   RBLE_PASP_Client_Disable_Test,                      NULL,       "6.PASP Client_Disable\n",                      },
    {  7,   RBLE_PASP_Client_Read_Char_Test,                    NULL,       "7.PASP Client_Read_Char\n",                    },
    {  8,   RBLE_PASP_Client_Write_Ringer_Control_Point_Test,   NULL,       "8.PASP Client_Write_Ringer_Control_Point\n",   },
    {  9,   RBLE_PASP_Client_Write_Char_Test,                   NULL,       "9.PASP Client_Write_Char\n",                   },
#endif
    {  ESC_KEY, Menu_Exit,                                      NULL,       "ESC Key: Menu exit\n",                         },
    {  0,   NULL,                                               NULL,       NULL,                                           }
};

const SELECT_MENU   Test_Prog_PASP_Select_Menu = {
    "\n-- BLE Sample Program Phone Alert Status Profile Test Menu --",
    Test_Prog_PASP_Menu
};

#if (PRF_SEL_PASC)
static RBLE_PASS_CONTENT g_pass = {0};
#endif


/******************************* Program Area *****************************************/
#if (PRF_SEL_PASS)
/******************************************************************************************/
/**
 *  @brief rBLE PASPS(Server) Call Back
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
static void RBLE_APP_PASPS_CallBack( RBLE_PASPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_PASP_EVENT_SERVER_ENABLE_COMP:
            PASP_Event_Disp("SERVER_ENABLE_COMP", event->param.server_enable.status);
            Connection_Handle_Disp( event->param.server_enable.conhdl );
            break;
        case RBLE_PASP_EVENT_SERVER_DISABLE_COMP:
            printf("\nrBLE PASP EVENT (SERVER_DISABLE_COMP)\n" );
            Connection_Handle_Disp( event->param.server_disable.conhdl );
            printf("alert cfg:%d ringer cfg:%d\n", event->param.server_disable.server_info.alert_status_ntf_en, event->param.server_disable.server_info.ringer_setting_ntf_en);
            break;
        case RBLE_PASP_EVENT_SERVER_ERROR_IND:
            PASP_Event_Disp("SERVER_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_PASP_EVENT_SERVER_SEND_ALERT_STATUS_COMP:
            PASP_Event_Disp("SERVER_SEND_ALERT_STATUS_COMP", event->param.send_alert.status);
            Connection_Handle_Disp( event->param.send_alert.conhdl );
            break;
        case RBLE_PASP_EVENT_SERVER_SEND_RINGER_SETTING_COMP:
            PASP_Event_Disp("SERVER_SEND_RINGER_SETTING_COMP", event->param.send_ringer.status);
            Connection_Handle_Disp( event->param.send_ringer.conhdl );
            break;
        case RBLE_PASP_EVENT_SERVER_CHG_RINGER_CP_IND:
            printf("\nrBLE PASP EVENT (SERVER_CHG_RINGER_CP_IND)\n" );
            Connection_Handle_Disp( event->param.chg_ringer_cp_ind.conhdl );
            printf("cp val:%d\n", event->param.chg_ringer_cp_ind.cp_val);
            break;
        case RBLE_PASP_EVENT_SERVER_CFG_NTF_IND:
            printf("\nrBLE PASP EVENT (SERVER_CFG_NTF_IND)\n" );
            Connection_Handle_Disp( event->param.cfg_ntf_ind.conhdl );
            printf("char:%d  cfg:%d\n", event->param.cfg_ntf_ind.char_code, event->param.cfg_ntf_ind.cfg_val);
            break;
        case RBLE_PASP_EVENT_SERVER_COMMAND_DISALLOWED_IND:
            PASP_Event_Disp("SERVER_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A PASP_Server_Enable command is executed.
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
static BOOL RBLE_PASP_Server_Enable_Test( void )
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
    RBLE_PASP_SERVER_PARAM param = {0};

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_PASP_Server_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_PASPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PASP_Server_Disable command is executed.
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
static BOOL RBLE_PASP_Server_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_PASP_Server_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PASP_Server_Send_Alert_Status command is executed.
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
static BOOL RBLE_PASP_Server_Send_Alert_Status_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t alert_status = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            alert_status = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_PASP_Server_Send_Alert_Status( Connection_Handle, alert_status );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PASP_Server_Send_Ringer_Setting command is executed.
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
static BOOL RBLE_PASP_Server_Send_Ringer_Setting_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t ringer_setting = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            ringer_setting = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_PASP_Server_Send_Ringer_Setting( Connection_Handle, ringer_setting );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_PASS)


#if (PRF_SEL_PASC)
/******************************************************************************************/
/**
 *  @brief rBLE PASPC(Client) Call Back
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
static void RBLE_APP_PASPC_CallBack( RBLE_PASPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_PASP_EVENT_CLIENT_ENABLE_COMP:
            PASP_Event_Disp("CLIENT_ENABLE_COMP", event->param.client_enable.status);
            Connection_Handle_Disp( event->param.client_enable.conhdl );
            printf("\nPhone Alert Status Service\n" );
            printf("   Start Handle = 0x%04x\n", event->param.client_enable.pass.shdl);
            printf("   End Handle   = 0x%04x\n", event->param.client_enable.pass.ehdl);
            printf("   alert_status_char_hdl     = 0x%04x\n", event->param.client_enable.pass.alert_status_char_hdl);
            printf("   alert_status_val_hdl      = 0x%04x\n", event->param.client_enable.pass.alert_status_val_hdl);
            printf("   alert_status_cfg_hdl      = 0x%04x\n", event->param.client_enable.pass.alert_status_cfg_hdl);
            printf("   alert_status_prop         = 0x%04x\n", event->param.client_enable.pass.alert_status_prop);
            printf("   ringer_setting_char_hdl   = 0x%04x\n", event->param.client_enable.pass.ringer_setting_char_hdl);
            printf("   ringer_setting_val_hdl    = 0x%04x\n", event->param.client_enable.pass.ringer_setting_val_hdl);
            printf("   ringer_setting_cfg_hdl    = 0x%04x\n", event->param.client_enable.pass.ringer_setting_cfg_hdl);
            printf("   ringer_setting_prop       = 0x%04x\n", event->param.client_enable.pass.ringer_setting_prop);
            printf("   ringer_cp_char_hdl        = 0x%04x\n", event->param.client_enable.pass.ringer_cp_char_hdl);
            printf("   ringer_cp_val_hdl         = 0x%04x\n", event->param.client_enable.pass.ringer_cp_val_hdl);
            printf("   ringer_cp_prop            = 0x%04x\n", event->param.client_enable.pass.ringer_cp_prop);

            g_pass = event->param.client_enable.pass;
            break;
        case RBLE_PASP_EVENT_CLIENT_DISABLE_COMP:
            PASP_Event_Disp("CLIENT_DISABLE_COMP", event->param.client_disable.status);
            Connection_Handle_Disp( event->param.client_disable.conhdl );
            break;
        case RBLE_PASP_EVENT_CLIENT_ERROR_IND:
            PASP_Event_Disp("CLIENT_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_PASP_EVENT_CLIENT_ALERT_STATUS_NTF:
            printf("\nrBLE PASP EVENT (CLIENT_ALERT_STATUS_NTF)\n" );
            Connection_Handle_Disp( event->param.alert_ntf.conhdl );
            printf( "Alert Status = 0x%x\n", event->param.alert_ntf.alert_status );
            break;
        case RBLE_PASP_EVENT_CLIENT_RINGER_SETTING_NTF:
            printf("\nrBLE PASP EVENT (CLIENT_RINGER_SETTING_NTF)\n" );
            Connection_Handle_Disp( event->param.ringer_ntf.conhdl );
            printf( "Ringer Setting = 0x%x\n", event->param.ringer_ntf.ringer_setting );
            break;
        case RBLE_PASP_EVENT_CLIENT_READ_CHAR_RESPONSE:
            PASP_Event_Disp("CLIENT_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            event->param.rd_char_resp.data.data[event->param.rd_char_resp.data.len] = '\0';
            printf("(%s)\n", event->param.rd_char_resp.data.data);
            break;
        case RBLE_PASP_EVENT_CLIENT_WRITE_CHAR_RESPONSE:
            PASP_Event_Disp("CLIENT_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_PASP_EVENT_CLIENT_COMMAND_DISALLOWED_IND:
            PASP_Event_Disp("CLIENT_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A PASP_Client_Enable command is executed.
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
static BOOL RBLE_PASP_Client_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_pass.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_PASP_Client_Enable( Connection_Handle, con_type, &g_pass, RBLE_APP_PASPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PASP_Client_Disable command is executed.
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
static BOOL RBLE_PASP_Client_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_PASP_Client_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PASP_Client_Read_Char command is executed.
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
static BOOL RBLE_PASP_Client_Read_Char_Test( void )
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
        Ret_Status = RBLE_PASP_Client_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PASP_Client_Write_Ringer_Control_Point command is executed.
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
static BOOL RBLE_PASP_Client_Write_Ringer_Control_Point_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t cp_val = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            cp_val = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_PASP_Client_Write_Ringer_Control_Point( Connection_Handle, cp_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A PASP_Client_Write_Char command is executed.
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
static BOOL RBLE_PASP_Client_Write_Char_Test( void )
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
            if ( 1 < RBLE_Parameter_Cnt ) {
                cfg_val = (uint8_t)RBLE_Parameter_Arg[ 1 ];
            }
        }
        /* API Call */
        Ret_Status = RBLE_PASP_Client_Write_Char( Connection_Handle, char_code, cfg_val );
        printf( " char_code = %d, cfg_val = 0x%04X\n", char_code, cfg_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_PASS)


/******************************************************************************************/
/**
 *  @brief rBLE PASP Event Disp
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
static void PASP_Event_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE PASP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}


#endif  //#if (PRF_SEL_PASC || PRF_SEL_PASS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
