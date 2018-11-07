/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_custom.c
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

#ifdef USE_SAMPLE_PROFILE

#ifdef USE_CUSTOM_DEMO
#include    "push_sw.h"
#endif

/**
 * DEFINES
 ****************************************************************************************
 */
#define    USE_SCPC_SAMPLE
#define    USE_SCPS_SAMPLE


/**
 * TYPE DEFINES
 ****************************************************************************************
 */
#ifdef USE_CUSTOM_DEMO
typedef struct {
    BOOL        Notify_en;
    BOOL        Indicate_en;
    BOOL        Timer_en;
    uint16_t    Timer_interval;
    uint8_t     Notify_len;
    uint8_t     Indicate_len;
} RBLE_SCP_SAMPLE_INFO;
#endif


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
#ifdef USE_SCPS_SAMPLE
static void RBLE_APP_SCPS_CallBack( RBLE_SCPS_EVENT *event );                               /* SCP(Server) Call Back */
       BOOL RBLE_SCP_Server_Enable_Test( void );                                            /* A SCP_Server_Enable command is executed. */
       BOOL RBLE_SCP_Server_Disable_Test( void );                                           /* A SCP_Server_Disable command is executed. */
static BOOL RBLE_SCP_Server_Send_Notify_Test( void );                                       /* A SCP_Server_Send_Notify command is executed. */
static BOOL RBLE_SCP_Server_Send_Indicate_Test( void );                                     /* A SCP_Server_Send_Indicate command is executed. */
#endif
#ifdef USE_SCPC_SAMPLE
static void RBLE_APP_SCPC_CallBack( RBLE_SCPC_EVENT *event );                               /* SCP(Client) Call Back */
static BOOL RBLE_SCP_Client_Enable_Test( void );                                            /* A SCP_Client_Enable command is executed. */
static BOOL RBLE_SCP_Client_Disable_Test( void );                                           /* A SCP_Client_Disable command is executed. */
static BOOL RBLE_SCP_Client_Read_Char_Test( void );                                         /* A SCP_Client_Read_Char command is executed. */
static BOOL RBLE_SCP_Client_Write_Char_Test( void );                                        /* A SCP_Client_Write_Char command is executed. */
#endif

static void SCP_Event_Disp(char_t *event_str, RBLE_STATUS status);


#ifdef USE_CUSTOM_DEMO
static void send_data( void );
static void timer_int( void );

extern BOOL RBLE_GAP_Broadcast_Enable_Test(void);
#endif

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*  Sample Custom Profile Select Menu */
static const MENU_INFO  Test_Prog_SCP_Menu[] = {
#ifdef  USE_SCPS_SAMPLE
    {  1,   RBLE_SCP_Server_Enable_Test,            NULL,       "1.SCP Server_Enable\n",            },
    {  2,   RBLE_SCP_Server_Disable_Test,           NULL,       "2.SCP Server_Disable\n",           },
    {  3,   RBLE_SCP_Server_Send_Notify_Test,       NULL,       "3.SCP Server_Send_Notify\n",       },
    {  4,   RBLE_SCP_Server_Send_Indicate_Test,     NULL,       "4.SCP Server_Send_Indicate\n",     },
#endif
#ifdef  USE_SCPC_SAMPLE
    {  5,   RBLE_SCP_Client_Enable_Test,            NULL,       "5.SCP Client_Enable\n",            },
    {  6,   RBLE_SCP_Client_Disable_Test,           NULL,       "6.SCP Client_Disable\n",           },
    {  7,   RBLE_SCP_Client_Read_Char_Test,         NULL,       "7.SCP Client_Read_Char\n",         },
    {  8,   RBLE_SCP_Client_Write_Char_Test,        NULL,       "8.SCP Client_Write_Char\n",        },
#endif
    {  ESC_KEY, Menu_Exit,                          NULL,       "ESC Key: Menu exit\n",             },
    {  0,   NULL,                                   NULL,       NULL,                               }
};

const SELECT_MENU Test_Prog_SCP_Select_Menu = {
    "\n-- BLE Sample Program Sample Custom Profile Test Menu --",
    Test_Prog_SCP_Menu
};


#ifdef USE_CUSTOM_DEMO
RBLE_SCP_SAMPLE_INFO    scp_sample_info = {
    FALSE,      /* notify enable flag */
    FALSE,      /* indicate enable flag */
    FALSE,      /* timer interrupt enable flag */
    100,        /* timer interval time(100*10ms) */
    5,          /* notify data length */
    5           /* indicate data length */
};
#endif

#ifdef  USE_SCPC_SAMPLE
static RBLE_SCS_CONTENT g_sample_custom_svc = { 0 };
#endif


/******************************* Program Area *****************************************/
#ifdef USE_CUSTOM_DEMO
static void send_data( void )
{
    RBLE_STATUS    Ret_Status;
    static uint8_t notify_count = 0;
    static uint8_t indicate_count = 0;
    
    RBLE_SCP_NOTIFY_INFO    notify_info = {
        0x05,   /* length */
        {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,0x11,0x12,0x13}   /* data */
    };
    RBLE_SCP_IND_INFO   ind_info = {
        0x05,   /* length */
        {0x00,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8a,0x8b,0x8c,0x8d,0x8e,0x8f,0x90,0x91,0x92,0x93}   /* data */
    };
    
    /* indicate/notify */
    if (scp_sample_info.Notify_en == TRUE) {
        notify_info.data_len = scp_sample_info.Notify_len;
        notify_info.data[0]  = notify_count;
        Ret_Status = RBLE_SCP_Server_Send_Notify( Connection_Handle, &notify_info );
        if(scp_sample_info.Timer_interval != 0) {
            printf( "Send Notify");
            RBLE_Status_Disp( Ret_Status );
        }
        notify_count++;
    }
    if (scp_sample_info.Indicate_en == TRUE) {
        ind_info.data_len = scp_sample_info.Indicate_len;
        ind_info.data[0]  = indicate_count;
        Ret_Status = RBLE_SCP_Server_Send_Indicate( Connection_Handle, &ind_info );
        if(scp_sample_info.Timer_interval != 0) {
            printf( "Send Indicate");
            RBLE_Status_Disp( Ret_Status );
        }
        indicate_count++;
    }
}

static void timer_int( void )
{
    send_data();
    
    if (scp_sample_info.Timer_en == TRUE) {
        if(scp_sample_info.Timer_interval != 0) {
            Console_Set_Timer(scp_sample_info.Timer_interval);
        }
        else {
            Console_Send_msg();
        }
    }
}

void sw_int( void )
{
    if (scp_sample_info.Timer_en == TRUE) {
        scp_sample_info.Timer_en = FALSE;
    }
    else {
        scp_sample_info.Timer_en = TRUE;
        Console_Send_Timer_msg(&timer_int);
    }
}
#endif

#ifdef USE_SCPS_SAMPLE
/******************************************************************************************/
/**
 *  @brief rBLE SCP(Server) Call Back
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
static void RBLE_APP_SCPS_CallBack( RBLE_SCPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_SCP_EVENT_SERVER_ENABLE_COMP:
            SCP_Event_Disp("SERVER_ENABLE_COMP", event->param.server_enable.status);
            printf( "Connect Handle = %d\n", event->param.server_enable.conhdl );
            break;
        case RBLE_SCP_EVENT_SERVER_DISABLE_COMP:
            printf( "\nrBLE SCP EVENT (SERVER_DISABLE_COMP)\n" );
            printf( "Connect Handle = %d\n", event->param.server_disable.conhdl );
#ifdef USE_CUSTOM_DEMO
            // restart advertising for reconnection
            RBLE_GAP_Broadcast_Enable_Test();
#endif
            break;
        case RBLE_SCP_EVENT_SERVER_ERROR_IND:
            SCP_Event_Disp("SERVER_ERROR_IND", event->param.error_ind.status);
            printf( "Connect Handle = %d\n", event->param.error_ind.conhdl );
            break;
        case RBLE_SCP_EVENT_SERVER_SEND_NOTIFY_COMP:
            SCP_Event_Disp("SERVER_SEND_NOTIFY_COMP", event->param.send_notify.status);
            printf( "Connect Handle = %d\n", event->param.send_notify.conhdl );
            break;
        case RBLE_SCP_EVENT_SERVER_SEND_IND_COMP:
            SCP_Event_Disp("SERVER_SEND_IND_COMP", event->param.send_ind.status);
            printf( "Connect Handle = %d\n", event->param.send_ind.conhdl );
            break;
        case RBLE_SCP_EVENT_SERVER_CHG_INDNTF_IND:
            printf( "\nrBLE SCP EVENT (SERVER_CFG_INDNTF_IND)\n" );
            printf( " Char Code = %s\n", ( event->param.cfg_indntf.char_code == RBLE_SCP_SCS_NTF_CFG ) ? "NOTIFY" :"INDICATE" );
            printf( " Cfg Value = %s\n", ( event->param.cfg_indntf.cfg_val == RBLE_SCP_STOP_NTFIND ) ? "STOP" : "START_NTF/IND" );
#ifdef USE_CUSTOM_DEMO
            /* update info */
            if (event->param.cfg_indntf.char_code == RBLE_SCP_SCS_NTF_CFG) {
                if(event->param.cfg_indntf.cfg_val & RBLE_SCP_START_NTF) {
                    scp_sample_info.Notify_en = TRUE;
                }
                else {
                    scp_sample_info.Notify_en = FALSE;
                }
            }
            else if (event->param.cfg_indntf.char_code == RBLE_SCP_SCS_IND_CFG) {
                if(event->param.cfg_indntf.cfg_val & RBLE_SCP_START_IND) {
                    scp_sample_info.Indicate_en = TRUE;
                }
                else {
                    scp_sample_info.Indicate_en = FALSE;
                }
            }
#endif
            break;
        case RBLE_SCP_EVENT_SERVER_CHG_CHAR_IND:
            printf( "\nrBLE SCP EVENT (SERVER_CHG_CHAR_IND)\n" );
            printf( " Char Code   = %d\n", event->param.write_char.char_code);
            if(event->param.write_char.char_code == RBLE_SCP_SCS_INTERVAL) {
                printf( " Write Value = 0x%02x, 0x%02x\n", event->param.write_char.value[0], event->param.write_char.value[1]);
#ifdef USE_CUSTOM_DEMO
                /* update info */
                scp_sample_info.Timer_interval = event->param.write_char.value[0] | (uint16_t)(event->param.write_char.value[1] << 8);
                printf( " Timer Interval = %d\n", scp_sample_info.Timer_interval);
#endif
            }
            else {
                printf( " Write Value = 0x%02x\n", event->param.write_char.value[0]);
#ifdef USE_CUSTOM_DEMO
                if(event->param.write_char.char_code == RBLE_SCP_SCS_NTF_LEN) {
                    scp_sample_info.Notify_len = event->param.write_char.value[0];
                    printf( " Notify Length = %d\n", scp_sample_info.Notify_len);
                }
                else if(event->param.write_char.char_code == RBLE_SCP_SCS_IND_LEN) {
                    scp_sample_info.Indicate_len = event->param.write_char.value[0];
                    printf( " Indicate Length = %d\n", scp_sample_info.Indicate_len);
                }
#endif
            }
            break;
        case RBLE_SCP_EVENT_SERVER_COMMAND_DISALLOWED_IND:
            SCP_Event_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A SCP_Server_Enable command is executed. 
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
BOOL RBLE_SCP_Server_Enable_Test( void )
{
    /* ConType */
    static uint8_t con_type = RBLE_SCP_CON_CFG;
//  static uint8_t con_type = RBLE_SCP_CON_NORMAL;

    static RBLE_SCP_SERVER_PARAM    param = {
        RBLE_SCP_START_NTF,
        RBLE_SCP_START_IND,
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SCP_Server_Enable( Connection_Handle, con_type, &param, RBLE_APP_SCPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SCP_Server_Disable command is executed. 
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
BOOL RBLE_SCP_Server_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

#ifdef USE_CUSTOM_DEMO
    scp_sample_info.Timer_en = FALSE;
    scp_sample_info.Notify_en = FALSE;
    scp_sample_info.Indicate_en = FALSE;
#endif

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SCP_Server_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SCP_Server_Send_Notify command is executed. 
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
static BOOL RBLE_SCP_Server_Send_Notify_Test( void )
{
    static RBLE_SCP_NOTIFY_INFO     notify_info = {
        0x05,                       /* length */
        {0x01,0x02,0xff,0xfe,0xfd}  /* data */
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SCP_Server_Send_Notify( Connection_Handle, &notify_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SCP_Server_Send_Indicate command is executed. 
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
static BOOL RBLE_SCP_Server_Send_Indicate_Test( void )
{
    static RBLE_SCP_IND_INFO    ind_info = {
        0x18,                       /* length */
        {0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f,0x10,
         0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18}   /* data */
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SCP_Server_Send_Indicate( Connection_Handle, &ind_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif

#ifdef USE_SCPC_SAMPLE
/******************************************************************************************/
/**
 *  @brief rBLE SCP(Client) Call Back
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
static void RBLE_APP_SCPC_CallBack( RBLE_SCPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_SCP_EVENT_CLIENT_ENABLE_COMP:
            SCP_Event_Disp("CLIENT_ENABLE_COMP", event->param.client_enable.status);
            printf( "Connect Handle = %d\n", event->param.client_enable.conhdl );
            printf( "* Sample Custom Service\n");
            printf( "   Start Handle = 0x%04X\n", event->param.client_enable.scs.shdl );
            printf( "   End Handle   = 0x%04X\n\n", event->param.client_enable.scs.ehdl );
            printf( "   ntf_char_hdl = 0x%04X\n", event->param.client_enable.scs.ntf_char_hdl );
            printf( "   ntf_val_hdl  = 0x%04X\n", event->param.client_enable.scs.ntf_val_hdl );
            printf( "   ntf_cfg_hdl  = 0x%04X\n", event->param.client_enable.scs.ntf_cfg_hdl );
            printf( "   ntf_prop     = 0x%02X\n\n", event->param.client_enable.scs.ntf_prop );
            
            printf( "   ind_char_hdl = 0x%04X\n", event->param.client_enable.scs.ind_char_hdl );
            printf( "   ind_val_hdl  = 0x%04X\n", event->param.client_enable.scs.ind_val_hdl );
            printf( "   ind_cfg_hdl  = 0x%04X\n", event->param.client_enable.scs.ind_cfg_hdl );
            printf( "   ind_prop     = 0x%02X\n\n", event->param.client_enable.scs.ind_prop );
            
            printf( "   interval_char_hdl = 0x%04X\n", event->param.client_enable.scs.interval_char_hdl );
            printf( "   interval_val_hdl  = 0x%04X\n", event->param.client_enable.scs.interval_val_hdl );
            printf( "   interval_prop     = 0x%02X\n\n", event->param.client_enable.scs.interval_prop );
            
            printf( "   ntf_len_char_hdl  = 0x%04X\n", event->param.client_enable.scs.ntf_len_char_hdl );
            printf( "   ntf_len_val_hdl   = 0x%04X\n", event->param.client_enable.scs.ntf_len_val_hdl );
            printf( "   ntf_len_prop      = 0x%02X\n\n", event->param.client_enable.scs.ntf_len_prop );
            
            printf( "   ind_len_char_hdl  = 0x%04X\n", event->param.client_enable.scs.ind_len_char_hdl );
            printf( "   ind_len_val_hdl   = 0x%04X\n", event->param.client_enable.scs.ind_len_val_hdl );
            printf( "   ind_len_prop      = 0x%02X\n\n", event->param.client_enable.scs.ind_len_prop );

            g_sample_custom_svc = event->param.client_enable.scs;
            break;
        case RBLE_SCP_EVENT_CLIENT_DISABLE_COMP:
            SCP_Event_Disp("CLIENT_DISABLE_COMP", event->param.client_disable.status);
            printf( "Connect Handle = %d\n", event->param.client_disable.conhdl );
            break;
        case RBLE_SCP_EVENT_CLIENT_ERROR_IND:
            SCP_Event_Disp("CLIENT_ERROR_IND", event->param.error_ind.status);
            printf( "Connect Handle = %d\n", event->param.error_ind.conhdl );
            break;
        case RBLE_SCP_EVENT_CLIENT_NOTIFY:
            printf( "\nrBLE SCP EVENT (CLIENT_NOTIFY)\n" );
            printf( "Connect Handle = %d\n", event->param.notify.conhdl );
            Print_Dump( event->param.notify.data, (uint16_t)event->param.notify.data_len );
            break;
        case RBLE_SCP_EVENT_CLIENT_INDICATE:
            printf( "\nrBLE SCP EVENT (CLIENT_INDICATE)\n" );
            printf( "Connect Handle = %d\n", event->param.ind.conhdl );
            Print_Dump( event->param.ind.data, (uint16_t)event->param.ind.data_len );
            break;
        case RBLE_SCP_EVENT_CLIENT_READ_CHAR_RESPONSE:
            SCP_Event_Disp("CLIENT_READ_CHAR_RES", event->param.rd_char_resp.att_code);
            printf( "Connect Handle = %d\n", event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            break;
        case RBLE_SCP_EVENT_CLIENT_WRITE_CHAR_RESPONSE:
            SCP_Event_Disp("CLIENT_WRITE_CHAR_RES", event->param.wr_char_resp.att_code);
            printf( "Connect Handle = %d\n", event->param.wr_char_resp.conhdl );
            break;
        case RBLE_SCP_EVENT_CLIENT_COMMAND_DISALLOWED_IND:
            SCP_Event_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A SCP_Client_Enable command is executed. 
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
static BOOL RBLE_SCP_Client_Enable_Test( void )
{
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if (0x0000 != g_sample_custom_svc.shdl) {
            con_type = RBLE_PRF_CON_NORMAL;
        }

        /* API Call */
        Ret_Status = RBLE_SCP_Client_Enable( Connection_Handle, con_type, &g_sample_custom_svc, RBLE_APP_SCPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SCP_Client_Disable command is executed. 
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
static BOOL RBLE_SCP_Client_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SCP_Client_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SCP_Client_Read_Char command is executed. 
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
static BOOL RBLE_SCP_Client_Read_Char_Test( void )
{
    /* CharCode */
    uint8_t         char_code;
    RBLE_STATUS     Ret_Status;
    int32_t         Parameter_No;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = 0;
    }
    switch (Parameter_No) {
    case 0:
    default:
        char_code = RBLE_SCP_SCS_NTF_CFG;
        break;
    case 1:
        char_code = RBLE_SCP_SCS_IND_CFG;
        break;
    case 2:
        char_code = RBLE_SCP_SCS_INTERVAL;
        break;
    case 3:
        char_code = RBLE_SCP_SCS_NTF_LEN;
        break;
    case 4:
        char_code = RBLE_SCP_SCS_IND_LEN;
        break;
    }
    printf( "Select Char:%d\n",  char_code );

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_SCP_Client_Read_Char( Connection_Handle, char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SCP_Client_Write_Char command is executed. 
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
static BOOL RBLE_SCP_Client_Write_Char_Test( void )
{
    RBLE_STATUS                 Ret_Status;
    uint8_t                     write_value[2] = {0};
    uint8_t                     char_code;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {

            switch (RBLE_Parameter_Arg[ 0 ]) {
            case 0:
            default:
                char_code = RBLE_SCP_SCS_NTF_CFG;
                break;
            case 1:
                char_code = RBLE_SCP_SCS_IND_CFG;
                break;
            case 2:
                char_code = RBLE_SCP_SCS_INTERVAL;
                break;
            case 3:
                char_code = RBLE_SCP_SCS_NTF_LEN;
                break;
            case 4:
                char_code = RBLE_SCP_SCS_IND_LEN;
                break;
            }
            if (RBLE_Parameter_Cnt > 1) {
                write_value[0] = (uint8_t)(RBLE_Parameter_Arg[ 1 ] & 0xff);
                write_value[1] = (uint8_t)((RBLE_Parameter_Arg[ 1 ] >> 8) & 0xff);
            }
        } else {
            char_code = RBLE_SCP_SCS_NTF_CFG;
        }

        /* API Call */
        Ret_Status = RBLE_SCP_Client_Write_Char( Connection_Handle, char_code, &write_value[0] );
        printf( " char_code = %d, write_val = 0x%02X%02X\n", char_code, write_value[1], write_value[0] );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif

/******************************************************************************************/
/**
 *  @brief rBLE SCP Event Disp
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
static void SCP_Event_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE SCP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}


#endif  //#ifdef USE_SAMPLE_PROFILE
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
