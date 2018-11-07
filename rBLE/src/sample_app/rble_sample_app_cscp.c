/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_cscp.c
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

#ifdef  CFG_SAMPLE

#ifdef CONFIG_EMBEDDED

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

#include    "rble.h"
#include    "rble_app.h"
#include    "menu_sel.h"
#include    "console.h"

#include    "prf_sel.h"

#if (PRF_SEL_CSCC || PRF_SEL_CSCS)
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
#if (PRF_SEL_CSCS)
static void RBLE_APP_CSCPS_CallBack( RBLE_CSCPS_EVENT *event );                             /* CSCP(Sensor) Call Back */
static BOOL RBLE_CSCP_Sensor_Enable_Test( void );                                           /* A CSCP_Sensor_Enable command is executed. */
static BOOL RBLE_CSCP_Sensor_Disable_Test( void );                                          /* A CSCP_Sensor_Disable command is executed. */
static BOOL RBLE_CSCP_Sensor_Send_Measurements_Test( void );                                /* A CSCP_Sensor_Send_Measurements command is executed. */
static BOOL RBLE_CSCP_Sensor_Send_Sc_Control_Point_Test( void );                            /* A CSCP_Sensor_Send_SC_Control_Point command is executed. */
#endif
#if (PRF_SEL_CSCC)
static void RBLE_APP_CSCPC_CallBack( RBLE_CSCPC_EVENT *event );                             /* CSCP(Collector) Call Back */
static BOOL RBLE_CSCP_Collector_Enable_Test( void );                                        /* A CSCP_Collector_Enable command is executed. */
static BOOL RBLE_CSCP_Collector_Disable_Test( void );                                       /* A CSCP_Collector_Disable command is executed. */
static BOOL RBLE_CSCP_Collector_Read_Char_Test( void );                                     /* A CSCP_Collector_Read_Char command is executed. */
static BOOL RBLE_CSCP_Collector_Write_SC_Control_Point_Test( void );                        /* A CSCP_Collector_Write_SC_Control_Point command is executed. */
static BOOL RBLE_CSCP_Collector_Write_Char_Test( void );                                    /* A CSCP_Collector_Write_Char command is executed. */
#endif

#if (PRF_SEL_CSCC)
static void CSCP_MEASURE_Info_Disp( RBLE_CSCP_MEASUREMENTS_INFO *p );
#endif

static void CSCP_SC_CP_Info_Disp( RBLE_CSCP_SC_CONTROL_POINT_INFO *p, uint8_t loc_num, uint8_t *res_param );
static void CSCP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status);
static void CSCP_Event_Disp(char_t *event_str);


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*  Cycling Speed and Cadence Profile Select Menu */
static const MENU_INFO  Test_Prog_CSCP_Menu[] = {
#if (PRF_SEL_CSCS)
    {  1,   RBLE_CSCP_Sensor_Enable_Test,                           NULL,       "1.CSCP Sensor_Enable\n",                       },
    {  2,   RBLE_CSCP_Sensor_Disable_Test,                          NULL,       "2.CSCP Sensor_Disable\n",                      },
    {  3,   RBLE_CSCP_Sensor_Send_Measurements_Test,                NULL,       "3.CSCP Sensor_Send_Measurements\n",            },
    {  4,   RBLE_CSCP_Sensor_Send_Sc_Control_Point_Test,            NULL,       "4.CSCP Sensor_Send_Sc_Control_Point\n",        },
#endif
#if (PRF_SEL_CSCC)
    {  5,   RBLE_CSCP_Collector_Enable_Test,                        NULL,       "5.CSCP Collector_Enable\n",                    },
    {  6,   RBLE_CSCP_Collector_Disable_Test,                       NULL,       "6.CSCP Collector_Disable\n",                   },
    {  7,   RBLE_CSCP_Collector_Read_Char_Test,                     NULL,       "7.CSCP Collector_Read_Char\n",                 },
    {  8,   RBLE_CSCP_Collector_Write_SC_Control_Point_Test,        NULL,       "8.CSCP Collector_Write_Sc_Control_Point\n",    },
    {  9,   RBLE_CSCP_Collector_Write_Char_Test,                    NULL,       "9.CSCP Collector_Write_Char\n",                },
#endif
    {  ESC_KEY, Menu_Exit,                                          NULL,       "ESC Key: Menu exit\n",                         },
    {  0,   NULL,                                                   NULL,       NULL,                                           }
};

const SELECT_MENU Test_Prog_CSCP_Select_Menu = {
    "\n-- BLE Sample Program Cycling Speed and Cadence Profile Test Menu --",
    Test_Prog_CSCP_Menu
};

#if (PRF_SEL_CSCC)
static BOOL                 First_Crank_Flg = TRUE;
static BOOL                 First_Wheel_Flg = TRUE;
static RBLE_CSCS_CONTENT    g_cscs = {0};
static RBLE_DIS_CONTENT     g_cscp_dis = {0};
#endif


/******************************* Program Area *****************************************/
#if (PRF_SEL_CSCS)
/******************************************************************************************/
/**
 *  @brief rBLE CSCP(Sensor) Call Back
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
static void RBLE_APP_CSCPS_CallBack( RBLE_CSCPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_CSCP_EVENT_SENSOR_ENABLE_COMP:
            CSCP_Event_Status_Disp("SENSOR_ENABLE_COMP", event->param.sensor_enable.status);
            Connection_Handle_Disp( event->param.sensor_enable.conhdl );
            break;
        case RBLE_CSCP_EVENT_SENSOR_DISABLE_COMP:
            CSCP_Event_Disp("SENSOR_DISABLE_COMP");
            Connection_Handle_Disp( event->param.sensor_disable.conhdl );
            break;
        case RBLE_CSCP_EVENT_SENSOR_ERROR_IND:
            CSCP_Event_Status_Disp("SENSOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_CSCP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP:
            CSCP_Event_Status_Disp("SENSOR_SEND_MEASUREMENTS_COMP", event->param.send_measurements.status);
            Connection_Handle_Disp( event->param.send_measurements.conhdl );
            break;
        case RBLE_CSCP_EVENT_SENSOR_SEND_SC_CP_COMP:
            CSCP_Event_Status_Disp("SENSOR_SEND_SC_CP_COMP", event->param.send_sc_cp.status);
            Connection_Handle_Disp( event->param.send_sc_cp.conhdl );
            break;
        case RBLE_CSCP_EVENT_SENSOR_CHG_SC_CP_IND:
            CSCP_Event_Disp("SENSOR_CHG_SC_CP_IND");
            CSCP_SC_CP_Info_Disp( &event->param.cscps_chg_sc_cp_ind.sc_cp_info, 0, NULL  );
            break;
        case RBLE_CSCP_EVENT_SENSOR_CFG_INDNTF_IND:
            CSCP_Event_Disp("SENSOR_CFG_INDNTF_IND");
            printf( "  char_code = %s Cfg Value = %s\n", ( event->param.cscps_cfg_indntf_ind.char_code == RBLE_CSCPC_CYCSPD_MEAS_CODE ) ? "MEAS" : "SC_CP"
                                                       , ( event->param.cscps_cfg_indntf_ind.cfg_val == RBLE_PRF_STOP_NTFIND ) ? "STOP" : "START_NTF/IND" );
            break;
        case RBLE_CSCP_EVENT_SENSOR_COMMAND_DISALLOWED_IND:
            CSCP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A BLP_Sensor_Enable command is executed. 
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
static BOOL RBLE_CSCP_Sensor_Enable_Test( void )
{
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_UNAUTH|RBLE_SVC_SEC_ENC,
    };
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    RBLE_CSCP_SENSOR_PARAM   param = {
        RBLE_PRF_STOP_NTFIND,           /* Cycling Speed measurement notification configuration */
        RBLE_PRF_STOP_NTFIND,           /* Cycling Speed SC control point indication configuration */
        RBLE_CSCPC_SENSOR_TOP_OF_SHOE,  /* sensor location */
        0                               /* reserved */
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
        Ret_Status = RBLE_CSCP_Sensor_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_CSCPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CSCP_Sensor_Disable command is executed. 
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
static BOOL RBLE_CSCP_Sensor_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_CSCP_Sensor_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CSCP_Sensor_Send_Measurements command is executed. 
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
static BOOL RBLE_CSCP_Sensor_Send_Measurements_Test( void )
{
    RBLE_CSCP_MEASUREMENTS_INFO  measurements_info = {
        0x03,           /* flags */
        0x00,           /* reserved */
        0x00ff00ff,     /* wheel revolutions */
        0x0010,         /* wheel event time */
        0x0200,         /* crank revolutions */
        0x0030          /* crank event time */
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_CSCP_Sensor_Send_Measurements( Connection_Handle, &measurements_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CSCP_Sensor_Send_SC_Control_Point command is executed. 
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
static BOOL RBLE_CSCP_Sensor_Send_Sc_Control_Point_Test( void )
{
    RBLE_CSCP_SC_CONTROL_POINT_INFO  sc_cp_info = {
        RBLE_CSCP_OP_RESPONSE_CODE,         /* operation code */
        0x00,                               /* reserved */
        0x00000000,                         /* cumulative value */
        0x00,                               /* sensor location */
        RBLE_CSCP_OP_SET_CUMULATIVE_CODE,   /* request op code */
        RBLE_CSCP_RES_SUCCESS_CODE          /* response value */
    };

    RBLE_STATUS Ret_Status;
    int32_t Parameter_No;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = 0;
        }
        switch (Parameter_No) {
        case 0:
        default:
            sc_cp_info.request_op_code = RBLE_CSCP_OP_SET_CUMULATIVE_CODE;
            break;
        case 1:
            sc_cp_info.request_op_code = RBLE_CSCP_OP_START_SC_CODE;
            break;
        case 2:
            sc_cp_info.request_op_code = RBLE_CSCP_OP_UPDATE_SL_CODE;
            break;
        case 3:
            sc_cp_info.request_op_code = RBLE_CSCP_OP_REQ_SUPPORTED_SL_CODE;
            break;
        }

        /* API Call */
        Ret_Status = RBLE_CSCP_Sensor_Send_SC_Control_Point( Connection_Handle, &sc_cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_CSCS)


#if (PRF_SEL_CSCC)
/******************************************************************************************/
/**
 *  @brief rBLE CSCP(Collector) Call Back
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
static void RBLE_APP_CSCPC_CallBack( RBLE_CSCPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_CSCP_EVENT_COLLECTOR_ENABLE_COMP:
            CSCP_Event_Status_Disp("COLLECTOR_ENABLE_COMP", event->param.collector_enable.status);
            Connection_Handle_Disp( event->param.collector_enable.conhdl );
            printf("* Cycling Speed and Cadence Service\n");
            printf(" Start Handle = 0x%04X\n", event->param.collector_enable.cscs.shdl );
            printf(" End Handle   = 0x%04X\n\n", event->param.collector_enable.cscs.ehdl );
            printf(" meas_char_hdl       = 0x%04X\n",   event->param.collector_enable.cscs.cycspd_meas_char_hdl );
            printf(" meas_val_hdl        = 0x%04X\n",   event->param.collector_enable.cscs.cycspd_meas_val_hdl );
            printf(" meas_cfg_hdl        = 0x%04X\n",   event->param.collector_enable.cscs.cycspd_meas_cfg_hdl );
            printf(" meas_prop           = 0x%02X\n\n",event->param.collector_enable.cscs.cycspd_meas_prop );
            printf(" feature_char_hdl    = 0x%04X\n",   event->param.collector_enable.cscs.csc_feature_char_hdl );
            printf(" feature_val_hdl     = 0x%04X\n",   event->param.collector_enable.cscs.csc_feature_val_hdl );
            printf(" feature_prop        = 0x%04X\n\n",event->param.collector_enable.cscs.csc_feature_prop );
            printf(" sensor_loc_char_hdl = 0x%04X\n",   event->param.collector_enable.cscs.sensor_loc_char_hdl );
            printf(" sensor_loc_val_hdl  = 0x%04X\n",   event->param.collector_enable.cscs.sensor_loc_val_hdl );
            printf(" sensor_loc_prop     = 0x%04X\n\n",event->param.collector_enable.cscs.sensor_loc_prop );
            printf(" sc_control_point_char_hdl = 0x%04X\n", event->param.collector_enable.cscs.sc_cp_char_hdl );
            printf(" sc_control_point_val_hdl  = 0x%04X\n", event->param.collector_enable.cscs.sc_cp_val_hdl );
            printf(" sc_control_point_cfg_hdl  = 0x%04X\n", event->param.collector_enable.cscs.sc_cp_cfg_hdl );
            printf(" sc_control_point_prop     = 0x%02X\n\n",event->param.collector_enable.cscs.sc_cp_prop );
            DIS_handle_Info_Disp(&event->param.collector_enable.dis);

            g_cscs = event->param.collector_enable.cscs;
            g_cscp_dis = event->param.collector_enable.dis;
            break;
        case RBLE_CSCP_EVENT_COLLECTOR_DISABLE_COMP:
            CSCP_Event_Status_Disp("COLLECTOR_DISABLE_COMP", event->param.collector_disable.status);
            Connection_Handle_Disp( event->param.collector_disable.conhdl );
            break;
        case RBLE_CSCP_EVENT_COLLECTOR_ERROR_IND:
            CSCP_Event_Status_Disp("COLLECTOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_CSCP_EVENT_COLLECTOR_MEASUREMENTS_NTF:
            CSCP_Event_Disp("COLLECTOR_MEASUREMENTS_NTF");
            CSCP_MEASURE_Info_Disp( &event->param.measurements_ntf.measure_info );
            break;
        case RBLE_CSCP_EVENT_COLLECTOR_SC_CP_IND:
            CSCP_Event_Disp("COLLECTOR_SC_CP_IND");
            CSCP_SC_CP_Info_Disp( &event->param.sc_cp_ind.sc_cp_info, event->param.sc_cp_ind.location_num, &event->param.sc_cp_ind.response_param[0] );
            break;
        case RBLE_CSCP_EVENT_COLLECTOR_READ_CHAR_RESPONSE:
            CSCP_Event_Status_Disp("COLLECTOR_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            event->param.rd_char_resp.data.data[event->param.rd_char_resp.data.len] = '\0';
            printf("(%s)\n", event->param.rd_char_resp.data.data);
            break;
        case RBLE_CSCP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE:
            CSCP_Event_Status_Disp("COLLECTOR_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_CSCP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND:
            CSCP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A CSCP_Collector_Enable command is executed. 
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
static BOOL RBLE_CSCP_Collector_Enable_Test( void )
{

    RBLE_STATUS Ret_Status;
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_cscs.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }

        /* API Call */
        Ret_Status = RBLE_CSCP_Collector_Enable( Connection_Handle, con_type, &g_cscs, &g_cscp_dis, RBLE_APP_CSCPC_CallBack );
        /* set first measurement flang */
        First_Wheel_Flg = TRUE;
        First_Crank_Flg = TRUE;

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CSCP_Collector_Disable command is executed. 
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
static BOOL RBLE_CSCP_Collector_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_CSCP_Collector_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CSCP_Collector_Read_Char command is executed. 
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
static BOOL RBLE_CSCP_Collector_Read_Char_Test( void )
{
    /* CharCode */
    uint8_t         char_code;
    RBLE_STATUS     Ret_Status;
    int32_t         Parameter_No;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = ILLEGAL_NO;
    }
    if ( ( ILLEGAL_NO == Parameter_No ) || ( RBLE_CSCPC_RD_DIS_IEEE < Parameter_No ) ) {
        char_code = RBLE_CSCPC_RD_CSCS_CM_CFG;
    } else {
        char_code = (uint8_t)Parameter_No;
    }

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_CSCP_Collector_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RBLE_CSCP_Collector_Write_SC_Control_Point_Test command is executed. 
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
static BOOL RBLE_CSCP_Collector_Write_SC_Control_Point_Test( void )
{
    RBLE_STATUS     Ret_Status;
    int32_t         Parameter_No;
    RBLE_CSCP_SC_CONTROL_POINT_INFO  sc_cp_info = {
        RBLE_CSCP_OP_SET_CUMULATIVE_CODE,   /* operation code */
        0x00,           /* reserved */
        0x00ff00ff,     /* cumulative value */
        0x00,           /* sensor location */
        0x00,           /* request op code */
        0x00            /* response value */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = 0;
        }
        switch (Parameter_No) {
        case 0:
        default:
            sc_cp_info.OpCode = RBLE_CSCP_OP_SET_CUMULATIVE_CODE;
            sc_cp_info.cumulative_value = 0x00ff00ff;
            break;
        case 1:
            sc_cp_info.OpCode = RBLE_CSCP_OP_START_SC_CODE;
            break;
        case 2:
            sc_cp_info.OpCode = RBLE_CSCP_OP_UPDATE_SL_CODE;
            sc_cp_info.sensor_location = RBLE_CSCPC_SENSOR_FRONT_WHEEL;
            break;
        case 3:
            sc_cp_info.OpCode = RBLE_CSCP_OP_REQ_SUPPORTED_SL_CODE;
            break;
        }
        /* API Call */
        Ret_Status = RBLE_CSCP_Collector_Write_SC_Control_Point( Connection_Handle, &sc_cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CSCP_Collector_Write_Char command is executed. 
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
static BOOL RBLE_CSCP_Collector_Write_Char_Test( void )
{
    RBLE_STATUS                 Ret_Status;
    uint32_t                    Parameter;
    uint32_t                    char_code = RBLE_CSCPC_CYCSPD_MEAS_CODE;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter = 0;
        }
        if ( Parameter == RBLE_PRF_START_NTF) {
            printf( "Start Ntf\n" );
        } else if( Parameter == RBLE_PRF_START_IND) {
            printf( "Start Ind\n" );
        } else {
            Parameter = RBLE_PRF_STOP_NTFIND;
            printf( "Stop Ind/Ntf\n" );
        }
        if (1 < RBLE_Parameter_Cnt) {
            char_code = RBLE_Parameter_Arg[ 1 ];
        }

        /* API Call */
        Ret_Status = RBLE_CSCP_Collector_Write_Char( Connection_Handle, (uint8_t)char_code, (uint16_t)Parameter );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief Display CSCP  Measurements Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      CSCP Measurements Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void CSCP_MEASURE_Info_Disp( RBLE_CSCP_MEASUREMENTS_INFO *p )
{
    uint16_t timeDiff;
    uint32_t paramDiff;
    static RBLE_CSCP_MEASUREMENTS_INFO prevMeasureInfo = {0};
    const char  *type_chr[] =
    {
        "Speed",
        "Cadence"
    };
    printf("Flag    :%02x\n", p->flags);
    /* Wheel Info Present ? */
    if ( 0 != (p->flags & 0x01) ) {
        printf("Wheel Rev    :0x%08lx(%lu)\n", p->wheel_revolutions, p->wheel_revolutions);
        printf("Wheel Ev Time:0x%04x(%u)\n", p->wheel_event_time, p->wheel_event_time);
        if ( First_Wheel_Flg == FALSE ) {
            /* caluculate speed */
            if( prevMeasureInfo.wheel_revolutions >= p->wheel_revolutions ) {
                printf("%s:-- (wheel back)\n", type_chr[0]);
            }
            else {
                float speed;
                if (p->wheel_event_time >= prevMeasureInfo.wheel_event_time) {
                    timeDiff = p->wheel_event_time - prevMeasureInfo.wheel_event_time;
                }
                else {
                    timeDiff = (uint16_t)(0x10000 - prevMeasureInfo.wheel_event_time + p->wheel_event_time);
                }
                if (timeDiff != 0) {
                    paramDiff = p->wheel_revolutions - prevMeasureInfo.wheel_revolutions;
                    /* wheel is 210cm(2.1) */
                    speed = (float)paramDiff / 1000 * 1024 * 60 * 60 / timeDiff * (float)2.1;
                    printf("%s:%f[km/s]  (wheel is 210cm)\n", type_chr[0],speed);
                }
                else {
                    printf("%s:--  (same time)\n", type_chr[0]);
                }
            }
        }
        else {
            printf("%s:--  (first event)\n", type_chr[0]);
            First_Wheel_Flg = FALSE;
        }
        prevMeasureInfo.wheel_revolutions = p->wheel_revolutions;
        prevMeasureInfo.wheel_event_time  = p->wheel_event_time;
    }
    
    /* Crank Info Present ? */
    if ( 0 != (p->flags & 0x02) ) {
        printf("Crank Rev    :0x%04x(%u)\n", p->crank_revolutions, p->crank_revolutions);
        printf("Crank Ev Time:0x%04x(%u)\n", p->crank_event_time, p->crank_event_time);
        if ( First_Crank_Flg == FALSE ) {
            /* caluculate cadence */
            uint16_t cadence;
            if (p->crank_revolutions >= prevMeasureInfo.crank_revolutions) {
                paramDiff = p->crank_revolutions - prevMeasureInfo.crank_revolutions;
            }
            else {
                paramDiff = 0x10000 - prevMeasureInfo.crank_revolutions + p->crank_revolutions;
            }
            if (p->crank_event_time >= prevMeasureInfo.crank_event_time) {
                timeDiff = p->crank_event_time - prevMeasureInfo.crank_event_time;
            }
            else {
                timeDiff = (uint16_t)(0x10000 - prevMeasureInfo.crank_event_time + p->crank_event_time);
            }
            if (timeDiff != 0) {
                cadence = (uint16_t)(paramDiff * 1024 * 60 / timeDiff);
                printf("%s:%u[rpm]\n", type_chr[1], cadence);
            }
            else {
                printf("%s:--  (same time)\n", type_chr[1]);
            }
        }
        else {
            printf("%s:--  (first event)\n", type_chr[1]);
            First_Crank_Flg = FALSE;
        }
        prevMeasureInfo.crank_revolutions = p->crank_revolutions;
        prevMeasureInfo.crank_event_time  = p->crank_event_time;
    }
}
#endif  //#if (PRF_SEL_CSCC)


/******************************************************************************************/
/**
 *  @brief Display CSCP SC Control Point Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      CSCP Measurements Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void CSCP_SC_CP_Info_Disp( RBLE_CSCP_SC_CONTROL_POINT_INFO *p, uint8_t loc_num, uint8_t *res_param )
{
    uint8_t i;
    
    printf("OpCode:%02x\n", p->OpCode);
    
    switch(p->OpCode)
    {
        case RBLE_CSCP_OP_SET_CUMULATIVE_CODE:
            printf("cumulative value :%08lx(%lu)\n",p->cumulative_value, p->cumulative_value);
            break;
        case RBLE_CSCP_OP_START_SC_CODE:
            printf("start sc\n");
            break;
        case RBLE_CSCP_OP_UPDATE_SL_CODE:
            printf("sensor location :%02d\n",p->sensor_location);
            break;
        case RBLE_CSCP_OP_REQ_SUPPORTED_SL_CODE:
            printf("req support sensor location\n");
            break;
        case RBLE_CSCP_OP_RESPONSE_CODE:
            switch(p->request_op_code)
            {
                case RBLE_CSCP_OP_SET_CUMULATIVE_CODE:
                    printf("Set Cumulative res\n");
                    break;
                case RBLE_CSCP_OP_START_SC_CODE:
                    printf("Start SC res\n");
                    break;
                case RBLE_CSCP_OP_UPDATE_SL_CODE:
                    printf("Update Sensor Location res\n");
                    break;
                case RBLE_CSCP_OP_REQ_SUPPORTED_SL_CODE:
                    printf("Req Supported Sensor Location res\n");
                    break;
                default:
                    printf("Invalid req op code(%d)\n",p->request_op_code);
                    break;
            }
            printf("res value:%d\n",p->response_value);
            if((p->request_op_code == RBLE_CSCP_OP_REQ_SUPPORTED_SL_CODE) && (res_param != NULL))
            {
                printf("location num:%d\n",loc_num);
                for(i=0;i<RBLE_CSCP_SENSORE_LOCATION_MAX;i++) {
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
 *  @brief rBLE CSCP Event and Status Disp
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
static void CSCP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE CSCP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}
/******************************************************************************************/
/**
 *  @brief rBLE CSCP Event Disp
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
static void CSCP_Event_Disp(char_t *event_str)
{
    printf( "\nrBLE CSCP EVENT (%s)\n", event_str);
}


#endif  //#if (PRF_SEL_CSCC || PRF_SEL_CSCS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
