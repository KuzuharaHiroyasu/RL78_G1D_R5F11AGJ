/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_rscp.c
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

#if (PRF_SEL_RSCC || PRF_SEL_RSCS)
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
#if (PRF_SEL_RSCS)
static void RBLE_APP_RSCPS_CallBack( RBLE_RSCPS_EVENT *event );                             /* RSCP(Sensor) Call Back */
static BOOL RBLE_RSCP_Sensor_Enable_Test( void );                                           /* A RSCP_Sensor_Enable command is executed */
static BOOL RBLE_RSCP_Sensor_Disable_Test( void );                                          /* A RSCP_Sensor_Disable command is executed */
static BOOL RBLE_RSCP_Sensor_Send_Measurements_Test( void );                                /* A RSCP_Sensor_Send_Measurements command is executed */
static BOOL RBLE_RSCP_Sensor_Send_SC_Control_Point_Test( void );                            /* A RSCP_Sensor_Send_SC_Control_Point command is executed */
#endif
#if (PRF_SEL_RSCC)
static void RBLE_APP_RSCPC_CallBack( RBLE_RSCPC_EVENT *event );                             /* RSCP(Collector) Call Back */
static BOOL RBLE_RSCP_Collector_Enable_Test( void );                                        /* A RSCP_Collector_Enable command is executed */
static BOOL RBLE_RSCP_Collector_Disable_Test( void );                                       /* A RSCP_Collector_Disable command is executed */
static BOOL RBLE_RSCP_Collector_Read_Char_Test( void );                                     /* A RSCP_Collector_Read_Char command is executed */
static BOOL RBLE_RSCP_Collector_Write_SC_Control_Point_Test( void );                        /* A RSCP_Collector_Write_SC_Control_Point command is executed */
static BOOL RBLE_RSCP_Collector_Write_Char_Test( void );                                    /* A RSCP_Collector_Write_Char command is executed */
#endif

#if (PRF_SEL_RSCC)
static void RSCP_MEASURE_Info_Disp( RBLE_RSCP_MEASUREMENTS_INFO *p );
#endif
static void RSCP_SC_CP_Info_Disp( RBLE_RSCP_SC_CONTROL_POINT_INFO *p, uint8_t loc_num, uint8_t *res_param );
static void RSCP_Event_Disp(char_t *event_str, RBLE_STATUS status);


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Running Speed and Cadence Profile Select Menu */
static const MENU_INFO  Test_Prog_RSCP_Menu[] = {
#if (PRF_SEL_RSCS)
    {  1,   RBLE_RSCP_Sensor_Enable_Test,                       NULL,       "1.RSCP Sensor_Enable\n",                       },
    {  2,   RBLE_RSCP_Sensor_Disable_Test,                      NULL,       "2.RSCP Sensor_Disable\n",                      },
    {  3,   RBLE_RSCP_Sensor_Send_Measurements_Test,            NULL,       "3.RSCP Sensor_Send_Measurements\n",            },
    {  4,   RBLE_RSCP_Sensor_Send_SC_Control_Point_Test,        NULL,       "4.RSCP Sensor_Send_SC_Control_Point\n",        },
#endif
#if (PRF_SEL_RSCC)
    {  5,   RBLE_RSCP_Collector_Enable_Test,                    NULL,       "5.RSCP Collector_Enable\n",                    },
    {  6,   RBLE_RSCP_Collector_Disable_Test,                   NULL,       "6.RSCP Collector_Disable\n",                   },
    {  7,   RBLE_RSCP_Collector_Read_Char_Test,                 NULL,       "7.RSCP Collector_Read_Char\n",                 },
    {  8,   RBLE_RSCP_Collector_Write_SC_Control_Point_Test,    NULL,       "8.RSCP Collector_Write_SC_Control_Point\n",    },
    {  9,   RBLE_RSCP_Collector_Write_Char_Test,                NULL,       "9.RSCP Collector_Write_Char\n",                },
#endif
    {  ESC_KEY, Menu_Exit,                                      NULL,       "ESC Key: Menu exit\n",                         },
    {  0,   NULL,                                               NULL,       NULL,                                           }
};

const SELECT_MENU   Test_Prog_RSCP_Select_Menu = {
    "\n-- BLE Sample Program Running Speed and Cadence Profile Test Menu --",
    Test_Prog_RSCP_Menu
};

#if (PRF_SEL_RSCC)
static RBLE_RSCS_CONTENT g_rscs = {0};
static RBLE_DIS_CONTENT g_rscp_dis = {0};
#endif


/******************************* Program Area *****************************************/
#if (PRF_SEL_RSCS)
/******************************************************************************************/
/**
 *  @brief rBLE RSCPS(Sensor) Call Back
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
static void RBLE_APP_RSCPS_CallBack( RBLE_RSCPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_RSCP_EVENT_SENSOR_ENABLE_COMP:
            RSCP_Event_Disp("SENSOR_ENABLE_COMP", event->param.sensor_enable.status);
            Connection_Handle_Disp( event->param.sensor_enable.conhdl );
            break;
        case RBLE_RSCP_EVENT_SENSOR_DISABLE_COMP:
            printf("\nrBLE RSCP EVENT (SENSOR_DISABLE_COMP)\n" );
            Connection_Handle_Disp( event->param.sensor_disable.conhdl );
            printf("meas cfg:%d sc cp cfg:%d\n", event->param.sensor_disable.sensor_info.rsc_meas_ntf_en, event->param.sensor_disable.sensor_info.sc_cp_ind_en);
            printf("sensor location:%d\n", event->param.sensor_disable.sensor_info.sensor_location);
            break;
        case RBLE_RSCP_EVENT_SENSOR_ERROR_IND:
            RSCP_Event_Disp("SENSOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_RSCP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP:
            RSCP_Event_Disp("SENSOR_SEND_MEASUREMENTS_COMP", event->param.send_measurements.status);
            Connection_Handle_Disp( event->param.send_measurements.conhdl );
            break;
        case RBLE_RSCP_EVENT_SENSOR_SEND_SC_CP_COMP:
            RSCP_Event_Disp("SENSOR_SEND_SC_CP_COMP", event->param.send_sc_cp.status);
            Connection_Handle_Disp( event->param.send_sc_cp.conhdl );
            break;
        case RBLE_RSCP_EVENT_SENSOR_CHG_SC_CP_IND:
            printf("\nrBLE RSCP EVENT (SENSOR_CHG_SC_CP_IND)\n" );
            Connection_Handle_Disp( event->param.chg_sc_cp_ind.conhdl );
            RSCP_SC_CP_Info_Disp( &event->param.chg_sc_cp_ind.sc_cp_info, 0, NULL );
            break;
        case RBLE_RSCP_EVENT_SENSOR_CFG_INDNTF_IND:
            printf("\nrBLE RSCP EVENT (SENSOR_CFG_INDNTF_IND)\n" );
            Connection_Handle_Disp( event->param.cfg_indntf_ind.conhdl );
            printf("char:%d  cfg:%d\n", event->param.cfg_indntf_ind.char_code, event->param.cfg_indntf_ind.cfg_val);
            break;
        case RBLE_RSCP_EVENT_SENSOR_COMMAND_DISALLOWED_IND:
            RSCP_Event_Disp("SENSOR_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Sensor_Enable command is executed.
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
static BOOL RBLE_RSCP_Sensor_Enable_Test( void )
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
    RBLE_RSCP_SENSOR_PARAM param = {0};

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_RSCP_Sensor_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_RSCPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Sensor_Disable command is executed.
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
static BOOL RBLE_RSCP_Sensor_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_RSCP_Sensor_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Sensor_Send_Measurements command is executed.
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
static BOOL RBLE_RSCP_Sensor_Send_Measurements_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_RSCP_MEASUREMENTS_INFO measurements_info = 
    {
        0x03,           /* flags */
        0x12,           /* Instantaneous Cadence */
        0x0034,         /* Instantaneous speed */
        0x0056,         /* Instantaneous stride length */
        0x12345678      /* Total Distance */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_RSCP_Sensor_Send_Measurements( Connection_Handle, &measurements_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Sensor_Send_SC_Control_Point command is executed.
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
static BOOL RBLE_RSCP_Sensor_Send_SC_Control_Point_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_RSCP_SC_CONTROL_POINT_INFO sc_cp_info =
    {
        RBLE_RSCP_OP_RESPONSE_CODE,
        0x00,                                   /* reserved */
        0x00000000,                             /* cumulative value */
        0x00,                                   /* sensor location */
        RBLE_RSCP_OP_REQ_SUPPORTED_SL_CODE,     /* request op code */
        RBLE_RSCP_RES_SUCCESS_CODE              /* response value */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 < RBLE_Parameter_Cnt ) {
            sc_cp_info.request_op_code = (uint8_t)RBLE_Parameter_Arg[0];
            if ( 1 < RBLE_Parameter_Cnt ) {
                sc_cp_info.response_value  = (uint8_t)RBLE_Parameter_Arg[1];
            }
        }
        /* API Call */
        Ret_Status = RBLE_RSCP_Sensor_Send_SC_Control_Point( Connection_Handle, &sc_cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_RSCS)


#if (PRF_SEL_RSCC)
/******************************************************************************************/
/**
 *  @brief rBLE RSCPC(Collector) Call Back *
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
static void RBLE_APP_RSCPC_CallBack( RBLE_RSCPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_RSCP_EVENT_COLLECTOR_ENABLE_COMP:
            RSCP_Event_Disp("COLLECTOR_ENABLE_COMP", event->param.collector_enable.status);
            Connection_Handle_Disp( event->param.collector_enable.conhdl );
            printf("\nRunning Speed and Cadence Service\n" );
            printf("   Start Handle = 0x%04x\n", event->param.collector_enable.rscs.shdl);
            printf("   End Handle   = 0x%04x\n", event->param.collector_enable.rscs.ehdl);
            printf("   rsc_meas_char_hdl         = 0x%04x\n", event->param.collector_enable.rscs.rsc_meas_char_hdl);
            printf("   rsc_meas_val_hdl          = 0x%04x\n", event->param.collector_enable.rscs.rsc_meas_val_hdl);
            printf("   rsc_meas_cfg_hdl          = 0x%04x\n", event->param.collector_enable.rscs.rsc_meas_cfg_hdl);
            printf("   rsc_meas_prop             = 0x%04x\n", event->param.collector_enable.rscs.rsc_meas_prop);
            printf("   rsc_feature_char_hdl      = 0x%04x\n", event->param.collector_enable.rscs.rsc_feature_char_hdl);
            printf("   rsc_feature_val_hdl       = 0x%04x\n", event->param.collector_enable.rscs.rsc_feature_val_hdl);
            printf("   rsc_feature_prop          = 0x%04x\n", event->param.collector_enable.rscs.rsc_feature_prop);
            printf("   sensor_loc_char_hdl       = 0x%04x\n", event->param.collector_enable.rscs.sensor_loc_char_hdl);
            printf("   sensor_loc_val_hdl        = 0x%04x\n", event->param.collector_enable.rscs.sensor_loc_val_hdl);
            printf("   sensor_loc_prop           = 0x%04x\n", event->param.collector_enable.rscs.sensor_loc_prop);
            printf("   sc_cp_char_hdl            = 0x%04x\n", event->param.collector_enable.rscs.sc_cp_char_hdl);
            printf("   sc_cp_val_hdl             = 0x%04x\n", event->param.collector_enable.rscs.sc_cp_val_hdl);
            printf("   sc_cp_cfg_hdl             = 0x%04x\n", event->param.collector_enable.rscs.sc_cp_cfg_hdl);
            printf("   sc_cp_prop                = 0x%04x\n", event->param.collector_enable.rscs.sc_cp_prop);
            DIS_handle_Info_Disp(&event->param.collector_enable.dis);

            g_rscs = event->param.collector_enable.rscs;
            g_rscp_dis = event->param.collector_enable.dis;
            break;
        case RBLE_RSCP_EVENT_COLLECTOR_DISABLE_COMP:
            RSCP_Event_Disp("COLLECTOR_DISABLE_COMP", event->param.collector_disable.status);
            Connection_Handle_Disp( event->param.collector_disable.conhdl );
            break;
        case RBLE_RSCP_EVENT_COLLECTOR_ERROR_IND:
            RSCP_Event_Disp("COLLECTOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_RSCP_EVENT_COLLECTOR_MEASUREMENTS_NTF:
            printf("\nrBLE RSCP EVENT (COLLECTOR_MEASUREMENTS_NTF)\n" );
            Connection_Handle_Disp( event->param.measurements_ntf.conhdl );
            RSCP_MEASURE_Info_Disp( &event->param.measurements_ntf.measure_info );
            break;
        case RBLE_RSCP_EVENT_COLLECTOR_SC_CP_IND:
            printf("\nrBLE RSCP EVENT (COLLECTOR_SC_CP_IND)\n" );
            Connection_Handle_Disp( event->param.sc_cp_ind.conhdl );
            RSCP_SC_CP_Info_Disp( &event->param.sc_cp_ind.sc_cp_info, event->param.sc_cp_ind.location_num, event->param.sc_cp_ind.response_param );
            break;
        case RBLE_RSCP_EVENT_COLLECTOR_READ_CHAR_RESPONSE:
            RSCP_Event_Disp("COLLECTOR_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            event->param.rd_char_resp.data.data[event->param.rd_char_resp.data.len] = '\0';
            printf("(%s)\n", event->param.rd_char_resp.data.data);
            break;
        case RBLE_RSCP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE:
            RSCP_Event_Disp("COLLECTOR_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_RSCP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND:
            RSCP_Event_Disp("COLLECTOR_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Collector_Enable command is executed.
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
static BOOL RBLE_RSCP_Collector_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_rscs.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_RSCP_Collector_Enable( Connection_Handle, con_type, &g_rscs, &g_rscp_dis, RBLE_APP_RSCPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Collector_Disable command is executed.
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
static BOOL RBLE_RSCP_Collector_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_RSCP_Collector_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Collector_Read_Char command is executed.
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
static BOOL RBLE_RSCP_Collector_Read_Char_Test( void )
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
        Ret_Status = RBLE_RSCP_Collector_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Collector_Write_SC_Control_Point command is executed.
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
static BOOL RBLE_RSCP_Collector_Write_SC_Control_Point_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_RSCP_SC_CONTROL_POINT_INFO wr_cp_info = 
    {
        RBLE_RSCP_OP_SET_CUMULATIVE_CODE,   /* operation code */
        0x00,           /* reserved */
        0x00ff00ff,     /* cumulative value */
        0x02,           /* sensor location */
        0x00,           /* request op code */
        0x00,           /* response value */
        0x00            /* reserved */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            wr_cp_info.OpCode = (uint8_t)RBLE_Parameter_Arg[ 0 ];
            if (1 < RBLE_Parameter_Cnt) {
                switch(wr_cp_info.OpCode)
                {
                    case RBLE_RSCP_OP_SET_CUMULATIVE_CODE:
                        wr_cp_info.cumulative_value = RBLE_Parameter_Arg[1];
                        break;
                    case RBLE_RSCP_OP_UPDATE_SL_CODE:
                        wr_cp_info.sensor_location = (uint8_t)RBLE_Parameter_Arg[1];
                        break;
                    default:
                        break;
                }
            }
        }
        /* API Call */
        Ret_Status = RBLE_RSCP_Collector_Write_SC_Control_Point( Connection_Handle, &wr_cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RSCP_Collector_Write_Char command is executed.
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
static BOOL RBLE_RSCP_Collector_Write_Char_Test( void )
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
        Ret_Status = RBLE_RSCP_Collector_Write_Char( Connection_Handle, char_code, cfg_val );
        printf( " char_code = %d, cfg_val = 0x%04X\n", char_code, cfg_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief Display RSCP RSC Measurements Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      RSC Measurements Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void RSCP_MEASURE_Info_Disp( RBLE_RSCP_MEASUREMENTS_INFO *p )
{
    printf("Flag :%02x\n", p->flags);

    printf("Instant Speed   :0x%04x(%u)\n", p->instant_speed, p->instant_speed);
    printf("Instant Cadence :0x%02x(%u)\n", p->instant_cadence, p->instant_cadence);
    if((p->flags & 0x1) == 0x1) {
        printf("Stride Length   :0x%04x(%u)\n", p->instant_stride_len, p->instant_stride_len);
    }
    if((p->flags & 0x2) == 0x2) {
        printf("Total Distance  :%ld\n", p->total_distance);
    }
}
#endif  //#if (PRF_SEL_RSCC)

/******************************************************************************************/
/**
 *  @brief Display RSCP SC Control Point Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      RSCP Measurements Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void RSCP_SC_CP_Info_Disp( RBLE_RSCP_SC_CONTROL_POINT_INFO *p, uint8_t loc_num, uint8_t *res_param )
{
    uint8_t i;
    
    printf("OpCode:%02x\n", p->OpCode);
    
    switch(p->OpCode)
    {
        case RBLE_RSCP_OP_SET_CUMULATIVE_CODE:
            printf("cumulative value :%08x(%lu)\n",p->cumulative_value, p->cumulative_value);
            break;
        case RBLE_RSCP_OP_START_CALIBRATION_CODE:
            printf("start sc\n");
            break;
        case RBLE_RSCP_OP_UPDATE_SL_CODE:
            printf("update sensor location :%02d\n",p->sensor_location);
            break;
        case RBLE_RSCP_OP_REQ_SUPPORTED_SL_CODE:
            printf("req support sensor location\n");
            break;
        case RBLE_RSCP_OP_RESPONSE_CODE:
            switch(p->request_op_code)
            {
                case RBLE_RSCP_OP_SET_CUMULATIVE_CODE:
                    printf("Set Cumulative res\n");
                    break;
                case RBLE_RSCP_OP_START_CALIBRATION_CODE:
                    printf("Start SC res\n");
                    break;
                case RBLE_RSCP_OP_UPDATE_SL_CODE:
                    printf("Update Sensor Location res\n");
                    break;
                case RBLE_RSCP_OP_REQ_SUPPORTED_SL_CODE:
                    printf("Req Supported Sensor Location res\n");
                    break;
                default:
                    printf("Invalid req op code(%d)\n",p->request_op_code);
                    break;
            }
            printf("res value:%d\n",p->response_value);
            if((p->request_op_code == RBLE_RSCP_OP_REQ_SUPPORTED_SL_CODE) && (res_param != NULL))
            {
                printf("location num:%d\n",loc_num);
                for(i=0;i<RBLE_RSCP_SENSORE_LOCATION_MAX;i++) {
                    printf("%02d ",res_param[i]);
                }
                printf("\n");
            }
            break;
        default:
            break;
    }
}


/******************************************************************************************/
/**
 *  @brief rBLE RSCP Event Disp
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
static void RSCP_Event_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE RSCP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}


#endif  //#if (PRF_SEL_RSCC || PRF_SEL_RSCS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
