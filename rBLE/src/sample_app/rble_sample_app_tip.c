/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_tip.c
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

#if (PRF_SEL_TIPC || PRF_SEL_TIPS)
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

#if (PRF_SEL_TIPS)
static void RBLE_APP_TIPS_CallBack( RBLE_TIPS_EVENT *event );                               /* TIP(Server) Call Back */
static BOOL RBLE_TIP_Server_Enable_Test( void );                                            /* A TIP_Server_Enable command is executed */
static BOOL RBLE_TIP_Server_Disable_Test( void );                                           /* A TIP_Server_Disable command is executed */
static BOOL RBLE_TIP_Server_Send_Current_Time_Test( void );                                 /* A TIP_Server_Send_Current_Time command is executed */
static BOOL RBLE_TIP_Server_Write_Data_Test( void );                                        /* A TIP_Server_Write_Data command is executed */
#endif
#if (PRF_SEL_TIPC)
static void RBLE_APP_TIPC_CallBack( RBLE_TIPC_EVENT *event );                               /* TIP(Client) Call Back */
static BOOL RBLE_TIP_Client_Enable_Test( void );                                            /* A TIP_Client_Enable command is executed */
static BOOL RBLE_TIP_Client_Disable_Test( void );                                           /* A TIP_Client_Disable command is executed */
static BOOL RBLE_TIP_Client_Read_Char_Test( void );                                         /* A TIP_Client_Read_Char command is executed */
static BOOL RBLE_TIP_Client_Write_Char_Test( void );                                        /* A TIP_Client_Write_Char command is executed */
static BOOL RBLE_TIP_Client_Write_Time_Update_CP_Test( void );                              /* A TIP_Client_Write_Time_Update_CP command is executed */
#endif

#if (PRF_SEL_TIPC)
void TIP_Info_Disp(uint8_t char_code, RBLE_ATT_INFO_DATA *att_data);
#endif
static void TIP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status);
static void TIP_Event_Disp(char_t *event_str);


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Time Profile Select Menu */
static const MENU_INFO  Test_Prog_TIP_Menu[] = {
#if (PRF_SEL_TIPS)
    {  1,   RBLE_TIP_Server_Enable_Test,                    NULL,       "1.TIP Server_Enable\n",                },
    {  2,   RBLE_TIP_Server_Disable_Test,                   NULL,       "2.TIP Server_Disable\n",               },
    {  3,   RBLE_TIP_Server_Send_Current_Time_Test,         NULL,       "3.TIP Server_Send_Current_Time\n",     },
    {  4,   RBLE_TIP_Server_Write_Data_Test,                NULL,       "4.TIP Server_Write_Data\n",            },
#endif
#if (PRF_SEL_TIPC)
    {  5,   RBLE_TIP_Client_Enable_Test,                    NULL,       "5.TIP Client_Enable\n",                },
    {  6,   RBLE_TIP_Client_Disable_Test,                   NULL,       "6.TIP Client_Disable\n",               },
    {  7,   RBLE_TIP_Client_Read_Char_Test,                 NULL,       "7.TIP Client_Read_Char\n",             },
    {  8,   RBLE_TIP_Client_Write_Char_Test,                NULL,       "8.TIP Client_Write_Char\n",            },
    {  9,   RBLE_TIP_Client_Write_Time_Update_CP_Test,      NULL,       "9.TIP Client_Write_Time_Update_CP\n",  },
#endif
    {  ESC_KEY, Menu_Exit,                                  NULL,       "ESC Key: Menu exit\n",                 },
    {  0,   NULL,                                           NULL,       NULL,                                   }
};

const SELECT_MENU   Test_Prog_TIP_Select_Menu = {
    "\n-- BLE Sample Program Time Profile Test Menu --",
    Test_Prog_TIP_Menu
};


#if (PRF_SEL_TIPC)
static RBLE_CTS_CONTENT g_cts = {0};
static RBLE_NDCS_CONTENT g_ndcs = {0};
static RBLE_RTUS_CONTENT g_rtus = {0};
#endif

static uint8_t Tip_Read_Char_Code;


/******************************* Program Area *****************************************/
#if (PRF_SEL_TIPS)
/******************************************************************************************/
/**
 *  @brief rBLE TIPS(Server) Call Back
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
static void RBLE_APP_TIPS_CallBack( RBLE_TIPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_TIP_EVENT_SERVER_ENABLE_COMP:
            TIP_Event_Status_Disp("SERVER_ENABLE_COMP", event->param.server_enable.status);
            Connection_Handle_Disp( event->param.server_enable.conhdl );
            break;
        case RBLE_TIP_EVENT_SERVER_DISABLE_COMP:
            TIP_Event_Disp("SERVER_DISABLE_COMP");
            Connection_Handle_Disp( event->param.server_disable.conhdl );
            break;
        case RBLE_TIP_EVENT_SERVER_ERROR_IND:
            TIP_Event_Status_Disp("SERVER_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_TIP_EVENT_SERVER_SEND_CURRENT_TIME_COMP:
            TIP_Event_Status_Disp("SERVER_SEND_CURRENT_TIME_COMP", event->param.send_current_time.status);
            Connection_Handle_Disp( event->param.send_current_time.conhdl );
            break;
        case RBLE_TIP_EVENT_SERVER_WRITE_DATA_COMP:
            TIP_Event_Status_Disp("SERVER_WRITE_DATA_COMP", event->param.write_data.status);
            break;
        case RBLE_TIP_EVENT_SERVER_CHG_TIME_UPDATE_CP_IND:
            TIP_Event_Disp("SERVER_CHG_TIME_UPDATE_CP_IND");
            Connection_Handle_Disp( event->param.chg_cp_ind.conhdl );
            printf("update cp:%d\n", event->param.chg_cp_ind.update_cp);
            break;
        case RBLE_TIP_EVENT_SERVER_CFG_NTF_IND:
            TIP_Event_Disp("SERVER_CFG_NTF_IND");
            Connection_Handle_Disp( event->param.cfg_ntf_ind.conhdl );
            printf("cfg:%d\n", event->param.cfg_ntf_ind.cfg_val);
            break;
        case RBLE_TIP_EVENT_SERVER_COMMAND_DISALLOWED_IND:
            TIP_Event_Status_Disp("SERVER_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A TIP_Server_Enable command is executed.
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
static BOOL RBLE_TIP_Server_Enable_Test( void )
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
    uint16_t current_time_ntf_en = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_TIP_Server_Enable( Connection_Handle, sec_lvl[select], con_type, current_time_ntf_en, RBLE_APP_TIPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A TIP_Server_Disable command is executed.
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
static BOOL RBLE_TIP_Server_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_TIP_Server_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A TIP_Server_Send_Current_Time command is executed.
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
static BOOL RBLE_TIP_Server_Send_Current_Time_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_TIP_CURRENT_TIME current_time = 
    {{2013,5,29,13,40, 20},0,0,0};

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            current_time.adjust_reason = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_TIP_Server_Send_Current_Time( Connection_Handle, &current_time );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A TIP_Server_Write_Data command is executed.
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
static BOOL RBLE_TIP_Server_Write_Data_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t char_code = 0;
    RBLE_TIPS_WR_DATA wr_data = {0};

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            char_code = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        switch(char_code)
        {
            case RBLE_TIPS_WR_CTS_CRNT_TIME:
                wr_data.param.current_time.stamp.year   = 2013;
                wr_data.param.current_time.stamp.month  = 5;
                wr_data.param.current_time.stamp.day    = 29;
                wr_data.param.current_time.stamp.hour   = 11;
                wr_data.param.current_time.stamp.min    = 20;
                wr_data.param.current_time.stamp.sec    = 15;
                wr_data.param.current_time.day_of_week = RBLE_TIP_TIME_WEDNESDAY;
                wr_data.param.current_time.fractions256 = 0xff;
                wr_data.param.current_time.adjust_reason = 4;
                break;
            case RBLE_TIPS_WR_CTS_LOCAL_TIME:
                wr_data.param.local_time.time_zone  = 32;
                wr_data.param.next_dst.dst_offset = RBLE_TIP_DST_DAYLIGHT;
                break;
            case RBLE_TIPS_WR_CTS_REF_TIME:
                wr_data.param.ref_time.time_source  = RBLE_TIP_TIME_SOURCE_GPS;
                wr_data.param.ref_time.accuracy     = 10;
                wr_data.param.ref_time.days_since_update    = 10;
                wr_data.param.ref_time.hours_since_update   = 5;
                break;
            case RBLE_TIPS_WR_NDCS_TIME_DST:
                wr_data.param.next_dst.stamp.year   = 2013;
                wr_data.param.next_dst.stamp.month  = 10;
                wr_data.param.next_dst.stamp.day    = 27;
                wr_data.param.next_dst.stamp.hour   = 1;
                wr_data.param.next_dst.stamp.min    = 0;
                wr_data.param.next_dst.stamp.sec    = 0;
                wr_data.param.next_dst.dst_offset   = RBLE_TIP_DST_STANDARD;
                break;
            case RBLE_TIPS_WR_RTUS_UPDATE_STATUS:
                wr_data.param.update_state.current_state    = RBLE_TIP_TIME_UPDATE_PENDING;
                wr_data.param.update_state.update_result    = RBLE_TIP_TIME_UPDATE_NOT_ATTEMPTED;
                break;
            default:
                break;
        }
        
        Ret_Status = RBLE_TIP_Server_Write_Data( char_code, &wr_data );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_TIPS)

#if (PRF_SEL_TIPC)
/******************************************************************************************/
/**
 *  @brief rBLE TIPC(Client) Call Back
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
static void RBLE_APP_TIPC_CallBack( RBLE_TIPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    
    switch( event->type){
        case RBLE_TIP_EVENT_CLIENT_ENABLE_COMP:
            TIP_Event_Status_Disp("CLIENT_ENABLE_COMP", event->param.client_enable.status);
            Connection_Handle_Disp( event->param.client_enable.conhdl );
            printf("\n*Current Time Service\n");
            printf(" Start Handle = 0x%04X\n",  event->param.client_enable.cts.shdl );
            printf(" End Handle   = 0x%04X\n",  event->param.client_enable.cts.ehdl );
            printf(" current_time_char_hdl = 0x%04x\n", event->param.client_enable.cts.current_time_char_hdl);
            printf(" current_time_val_hdl  = 0x%04x\n", event->param.client_enable.cts.current_time_val_hdl);
            printf(" current_time_cfg_hdl  = 0x%04x\n", event->param.client_enable.cts.current_time_cfg_hdl);
            printf(" current_time_prop     = 0x%02x\n", event->param.client_enable.cts.current_time_prop);
            printf(" local_time_char_hdl   = 0x%04x\n", event->param.client_enable.cts.local_time_info_char_hdl);
            printf(" local_time_val_hdl    = 0x%04x\n", event->param.client_enable.cts.local_time_info_val_hdl);
            printf(" local_time_prop       = 0x%02x\n", event->param.client_enable.cts.local_time_info_prop);
            printf(" ref_time_char_hdl     = 0x%04x\n", event->param.client_enable.cts.ref_time_info_char_hdl);
            printf(" ref_time_val_hdl      = 0x%04x\n", event->param.client_enable.cts.ref_time_info_val_hdl);
            printf(" ref_time_prop         = 0x%02x\n", event->param.client_enable.cts.ref_time_info_prop);
            printf("\n*Next DST Change Service\n");
            printf(" Start Handle = 0x%04X\n",  event->param.client_enable.ndcs.shdl );
            printf(" End Handle   = 0x%04X\n",  event->param.client_enable.ndcs.ehdl );
            printf(" time_dst_char_hdl     = 0x%04x\n", event->param.client_enable.ndcs.time_dst_char_hdl);
            printf(" time_dst_val_hdl      = 0x%04x\n", event->param.client_enable.ndcs.time_dst_val_hdl);
            printf(" time_dst_prop         = 0x%02x\n", event->param.client_enable.ndcs.time_dst_prop);
            printf("\n*Reference Time Update Service\n");
            printf(" Start Handle = 0x%04X\n",  event->param.client_enable.rtus.shdl );
            printf(" End Handle   = 0x%04X\n",  event->param.client_enable.rtus.ehdl );
            printf(" update_cp_char_hdl    = 0x%04x\n", event->param.client_enable.rtus.update_cp_char_hdl);
            printf(" update_cp_val_hdl     = 0x%04x\n", event->param.client_enable.rtus.update_cp_val_hdl);
            printf(" update_cp_prop        = 0x%02x\n", event->param.client_enable.rtus.update_cp_prop);
            printf(" update_state_char_hdl = 0x%04x\n", event->param.client_enable.rtus.update_state_char_hdl);
            printf(" update_state_val_hdl  = 0x%04x\n", event->param.client_enable.rtus.update_state_val_hdl);
            printf(" update_state_prop     = 0x%02x\n", event->param.client_enable.rtus.update_state_prop);

            g_cts  = event->param.client_enable.cts;
            g_ndcs = event->param.client_enable.ndcs;
            g_rtus = event->param.client_enable.rtus;
            break;
        case RBLE_TIP_EVENT_CLIENT_DISABLE_COMP:
            TIP_Event_Status_Disp("CLIENT_DISABLE_COMP", event->param.client_disable.status);
            Connection_Handle_Disp( event->param.client_disable.conhdl );
            break;
        case RBLE_TIP_EVENT_CLIENT_ERROR_IND:
            TIP_Event_Status_Disp("CLIENT_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_TIP_EVENT_CLIENT_CURRENT_TIME_NTF:
            TIP_Event_Disp("CLIENT_CURRENT_TIME_NTF");
            Connection_Handle_Disp( event->param.current_time_ntf.conhdl );
            TIP_Time_Disp(&event->param.current_time_ntf.current_time.stamp);
            printf("day of week   = %d\n", event->param.current_time_ntf.current_time.day_of_week);
            printf("fractions256  = %d\n", event->param.current_time_ntf.current_time.fractions256);
            printf("adjust reason = %x\n", event->param.current_time_ntf.current_time.adjust_reason);
            break;
        case RBLE_TIP_EVENT_CLIENT_READ_CHAR_RESPONSE:
            TIP_Event_Status_Disp("CLIENT_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            TIP_Info_Disp(Tip_Read_Char_Code, &event->param.rd_char_resp.data);
            break;
        case RBLE_TIP_EVENT_CLIENT_WRITE_CHAR_RESPONSE:
            TIP_Event_Status_Disp("CLIENT_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_TIP_EVENT_CLIENT_COMMAND_DISALLOWED_IND:
            TIP_Event_Status_Disp("CLIENT_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A TIP_Client_Enable command is executed.
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
static BOOL RBLE_TIP_Client_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_cts.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_TIP_Client_Enable( Connection_Handle, con_type, &g_cts, &g_ndcs, &g_rtus, RBLE_APP_TIPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A TIP_Client_Disable command is executed.
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
static BOOL RBLE_TIP_Client_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_TIP_Client_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A TIP_Client_Read_Char command is executed.
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
static BOOL RBLE_TIP_Client_Read_Char_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Tip_Read_Char_Code = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        else {
            Tip_Read_Char_Code = 0;
        }
        /* API Call */
        Ret_Status = RBLE_TIP_Client_Read_Char( Connection_Handle, Tip_Read_Char_Code );
        printf( " char_code = %d\n", Tip_Read_Char_Code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A TIP_Client_Write_Char command is executed.
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
static BOOL RBLE_TIP_Client_Write_Char_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint16_t cfg_val = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            cfg_val = (uint16_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_TIP_Client_Write_Char( Connection_Handle, cfg_val );
        printf( " cfg_val = 0x%04X\n", cfg_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A TIP_Client_Write_Time_Update_CP command is executed.
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
static BOOL RBLE_TIP_Client_Write_Time_Update_CP_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t time_update_cp = 0;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            time_update_cp = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_TIP_Client_Write_Time_Update_CP( Connection_Handle, time_update_cp );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief Display Time Profile reading charcteristic value
 *
 *  @b Contents of management:
 *
 *  @param[in] char_code     read characteristic code
 *  @param[in] att_data      read data
 *
 *  @retval none
 */
/******************************************************************************************/
void TIP_Info_Disp(uint8_t char_code, RBLE_ATT_INFO_DATA *att_data)
{
    switch(char_code)
    {
        case RBLE_TIPC_RD_CTS_CRNT_TIME:
            printf("Read Current Time\n");
            {
                RBLE_DATE_TIME stamp;
                stamp.year  = att_data->data[0] | (att_data->data[1] << 8);
                stamp.month = att_data->data[2];
                stamp.day   = att_data->data[3];
                stamp.hour  = att_data->data[4];
                stamp.min   = att_data->data[5];
                stamp.sec   = att_data->data[6];
                TIP_Time_Disp(&stamp);
                printf("day of week   = %d\n", att_data->data[7]);
                printf("fractions256  = %d\n", att_data->data[8]);
                printf("adjust reason = %x\n", att_data->data[9]);
            }
            break;
        case RBLE_TIPC_RD_CTS_CRNT_TIME_CFG:
            printf("Read Current Time Client Config %d\n", att_data->data[0] | (att_data->data[1] << 8));
            break;
        case RBLE_TIPC_RD_CTS_LOCAL_TIME:
            printf("Read Local Time Info\n");
            printf("time zone:%d  DST_offset:%d\n", (int8_t)(att_data->data[0]), att_data->data[1] );
            break;
        case RBLE_TIPC_RD_CTS_REF_TIME:
            printf("Read Reference Time Info\n");
            printf("time source      :%d  accuracy          :%d\n", att_data->data[0], att_data->data[1] );
            printf("days_since_update:%d  hours_since_update:%d\n", att_data->data[2], att_data->data[3] );
            break;
        case RBLE_TIPC_RD_NDCS_TIME_DST:
            printf("Read Next Update DST Time Info\n");
            {
                RBLE_DATE_TIME stamp;
                stamp.year  = att_data->data[0] | (att_data->data[1] << 8);
                stamp.month = att_data->data[2];
                stamp.day   = att_data->data[3];
                stamp.hour  = att_data->data[4];
                stamp.min   = att_data->data[5];
                stamp.sec   = att_data->data[6];
                TIP_Time_Disp(&stamp);
                printf("dst offset = %d\n", att_data->data[7]);
            }
            break;
        case RBLE_TIPC_RD_RTUS_UPDATE_STATUS:
            printf("Read Time Update Status\n");
            printf("current state:%d  update_result:%d\n", att_data->data[0], att_data->data[1] );
            break;
        default:
            break;
    }
}
#endif  //#if (PRF_SEL_TIPC)

/******************************************************************************************/
/**
 *  @brief rBLE TIP Event and Status Disp
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
static void TIP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE TIP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}

/******************************************************************************************/
/**
 *  @brief rBLE TIP Event Disp
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event_str      event strings
 *
 *  @retval
 */
/******************************************************************************************/
static void TIP_Event_Disp(char_t *event_str)
{
    printf( "\nrBLE TIP EVENT (%s)\n", event_str);
}

#endif  //#if (PRF_SEL_TIPC || PRF_SEL_TIPS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
