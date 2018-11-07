/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_cpp.c
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

#if (PRF_SEL_CPPC || PRF_SEL_CPPS)
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
#if (PRF_SEL_CPPS)
static void RBLE_APP_CPPS_CallBack( RBLE_CPPS_EVENT *event );                               /* CPPS(Sensor) Call Back */
static BOOL RBLE_CPP_Sensor_Enable_Test( void );                                            /* A CPP_Sensor_Enable command is executed. */
static BOOL RBLE_CPP_Sensor_Disable_Test( void );                                           /* A CPP_Sensor_Disable command is executed. */
static BOOL RBLE_CPP_Sensor_Send_Measurements_Test( void );                                 /* A CPP_Sensor_Send_Measurements command is executed. */
static BOOL RBLE_CPP_Sensor_Broadcast_Measurements_Test( void );                            /* A CPP_Sensor_Broadcast_Measurements command is executed. */
static BOOL RBLE_CPP_Sensor_Send_Vector_Test( void );                                       /* A CPP_Sensor_Send_Vector command is executed. */
static BOOL RBLE_CPP_Sensor_Send_CP_Control_Point_Test( void );                             /* A CPP_Sensor_Send_CP_Control_Point command is executed. */
static BOOL RBLE_CPP_Sensor_Send_Battery_Level_Test( void );                                /* A CPP_Sensor_Send_Battery_Level command is executed. */
static BOOL RBLE_CPP_Sensor_Send_Write_Response_Test( void );                               /* A CPP_Sensor_Send_Write_Response command is executed. */
#endif
#if (PRF_SEL_CPPC)
static void RBLE_APP_CPPC_CallBack( RBLE_CPPC_EVENT *event );                               /* CPPC(Collector) Call Back */
static BOOL RBLE_CPP_Collector_Enable_Test( void );                                         /* A CPP_Collector_Enable command is executed. */
static BOOL RBLE_CPP_Collector_Disable_Test( void );                                        /* A CPP_Collector_Disable command is executed. */
static BOOL RBLE_CPP_Collector_Read_Char_Test( void );                                      /* A CPP_Collector_Read_Char command is executed. */
static BOOL RBLE_CPP_Collector_Write_CP_Control_Point_Test( void );                         /* A CPP_Collector_Write_CP_Control_Point command is executed. */
static BOOL RBLE_CPP_Collector_Write_Char_Test( void );                                     /* A CPP_Collector_Write_Char command is executed. */
#endif


#if (PRF_SEL_CPPC)
static void CPP_Meas_Info_Disp(RBLE_CPP_MEASUREMENTS_INFO *meas_info);
static void CPP_Vector_Info_Disp(RBLE_CPP_VECTOR_INFO *vector_info);
static void CPP_Ind_Cp_Info_Disp(RBLE_CPPC_EVENT *event);
#endif
#if (PRF_SEL_CPPS)
static void CPP_Chg_Cp_Info_Disp(RBLE_CPP_WR_CONTROL_POINT_INFO *wr_info);
#endif
static void CPP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status);
static void CPP_Event_Disp(char_t *event_str);


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Cycling Power Profile Select Menu */
static const MENU_INFO  Test_Prog_CPP_Menu[] = {
#if (PRF_SEL_CPPS)
    {  1,   RBLE_CPP_Sensor_Enable_Test,                        NULL,       "1.CPP Sensor_Enable\n",                        },
    {  2,   RBLE_CPP_Sensor_Disable_Test,                       NULL,       "2.CPP Sensor_Disable\n",                       },
    {  3,   RBLE_CPP_Sensor_Send_Measurements_Test,             NULL,       "3.CPP Sensor_Send_Measurements\n",             },
    {  4,   RBLE_CPP_Sensor_Broadcast_Measurements_Test,        NULL,       "4.CPP Sensor_Broadcast_Measurements\n",        },
    {  5,   RBLE_CPP_Sensor_Send_Vector_Test,                   NULL,       "5.CPP Sensor_Send_Vector\n",                   },
    {  6,   RBLE_CPP_Sensor_Send_CP_Control_Point_Test,         NULL,       "6.CPP Sensor_Send_CP_Control_Point\n",         },
    {  7,   RBLE_CPP_Sensor_Send_Battery_Level_Test,            NULL,       "7.CPP Sensor_Send_Battery_Level\n",            },
    {  8,   RBLE_CPP_Sensor_Send_Write_Response_Test,           NULL,       "8.CPP Sensor_Send_Write_Response\n",           },
#endif
#if (PRF_SEL_CPPC)
    {  9,   RBLE_CPP_Collector_Enable_Test,                     NULL,       "9.CPP Collector_Enable\n",                     },
    { 10,   RBLE_CPP_Collector_Disable_Test,                    NULL,       "10.CPP Collector_Disable\n",                   },
    { 11,   RBLE_CPP_Collector_Read_Char_Test,                  NULL,       "11.CPP Collector_Read_Char\n",                 },
    { 12,   RBLE_CPP_Collector_Write_CP_Control_Point_Test,     NULL,       "12.CPP Collector_Write_CP_Control_Point\n",    },
    { 13,   RBLE_CPP_Collector_Write_Char_Test,                 NULL,       "13.CPP Collector_Write_Char\n",                },
#endif
    {  ESC_KEY, Menu_Exit,                                      NULL,       "ESC Key: Menu exit\n",                         },
    {  0,   NULL,                                               NULL,       NULL,                                           }
};

const SELECT_MENU   Test_Prog_CPP_Select_Menu = {
    "\n-- BLE Sample Program Cycling Power Profile Test Menu --",
    Test_Prog_CPP_Menu
};


static const char_t *CPP_Control_Point_Disp_Info[] = {
    "",
    "Set Cumulative",
    "Update Sensor Location",
    "Request Sensor Location",
    "Set Crank Length",
    "Request Crank Length",
    "Set Chain Length",
    "Request Chain Length",
    "Set Chain Weight",
    "Request Chain Weight",
    "Set Span Length",
    "Request Span Length",
    "Start Offset Compensation",
    "Set Mask Measurements Content",
    "Request Sampling Rate",
    "Request Factory Calibration Date"
};


#if (PRF_SEL_CPPC)
uint8_t g_cpp_brd_en = FALSE;
static uint8_t  tmp_brd_en;
static RBLE_CPS_CONTENT  g_cps = { 0 };
static RBLE_DIS_CONTENT  g_cpp_dis = { 0 };
static RBLE_BATS_CONTENT g_cpp_bas = { 0 };
#endif


/******************************* Program Area *****************************************/
#if (PRF_SEL_CPPS)
/******************************************************************************************/
/**
 *  @brief Cycling Power(Collector) Call Back
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
static void RBLE_APP_CPPS_CallBack( RBLE_CPPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_CPP_EVENT_SENSOR_ENABLE_COMP:
            CPP_Event_Status_Disp("SENSOR_ENABLE_COMP", event->param.sensor_enable.status);
            Connection_Handle_Disp( event->param.sensor_enable.conhdl );
            break;
        case RBLE_CPP_EVENT_SENSOR_DISABLE_COMP:
            CPP_Event_Disp("SENSOR_DISABLE_COMP");
            Connection_Handle_Disp( event->param.sensor_disable.conhdl );
            break;
        case RBLE_CPP_EVENT_SENSOR_ERROR_IND:
            CPP_Event_Status_Disp("SENSOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_CPP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP:
            CPP_Event_Status_Disp("SENSOR_SEND_MEASUREMENTS_COMP", event->param.send_measurements.status);
            Connection_Handle_Disp( event->param.send_measurements.conhdl );
            break;
        case RBLE_CPP_EVENT_SENSOR_BROADCAST_MEASUREMENTS_COMP:
            CPP_Event_Status_Disp("SENSOR_BROADCAST_MEASUREMENTS_COMP", event->param.broadcast_measurements.status);
            Connection_Handle_Disp( event->param.broadcast_measurements.conhdl );
            break;
        case RBLE_CPP_EVENT_SENSOR_SEND_VECTOR_COMP:
            CPP_Event_Status_Disp("SENSOR_SEND_VECTOR_COMP", event->param.send_vector.status);
            Connection_Handle_Disp( event->param.send_vector.conhdl );
            break;
        case RBLE_CPP_EVENT_SENSOR_SEND_CP_CP_COMP:
            CPP_Event_Status_Disp("SENSOR_SEND_CP_CP_COMP", event->param.send_cp_cp.status);
            Connection_Handle_Disp( event->param.send_cp_cp.conhdl );
            break;
        case RBLE_CPP_EVENT_SENSOR_SEND_BATTERY_LEVEL_COMP:
            CPP_Event_Status_Disp("SENSOR_SEND_BATTERY_LEVEL_COMP", event->param.send_battery_level.status);
            Connection_Handle_Disp( event->param.send_battery_level.conhdl );
            break;
        case RBLE_CPP_EVENT_SENSOR_CHG_CP_CP_IND:
            Connection_Handle_Disp( event->param.chg_cp_cp_ind.conhdl );
            CPP_Chg_Cp_Info_Disp( &event->param.chg_cp_cp_ind.wr_cp_info );
            break;
        case RBLE_CPP_EVENT_SENSOR_CFG_INDNTFBRD_IND:
            CPP_Event_Disp("SENSOR_CFG_INDNTFBRD_IND");
            printf( "  Char Code = %d\n", event->param.cfg_indntfbrd_ind.char_code );
            printf( "  Cfg Value = %s\n", ( event->param.cfg_indntfbrd_ind.cfg_val == RBLE_PRF_STOP_NTFIND ) ? "STOP" : "START" );
            break;
        case RBLE_CPP_EVENT_SENSOR_COMMAND_DISALLOWED_IND:
            CPP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A CPP_Sensor_Enable command is executed.
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
static BOOL RBLE_CPP_Sensor_Enable_Test( void )
{
    uint8_t     select = 0;
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_AUTH|RBLE_SVC_SEC_ENC,
    };
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    static RBLE_CPP_SENSOR_PARAM    param = {
        RBLE_PRF_START_NTF,         /* Cycling Power measurement notification configuration */
        RBLE_PRF_START_BRD,         /* Cycling Power measurement broadcast configuration */
        RBLE_PRF_START_NTF,         /* Cycling Power vector notification configuration */
        RBLE_PRF_START_IND,         /* Cycling Power control point indication configuration */
        RBLE_PRF_START_NTF,         /* Battery Level notification configuration */
        RBLE_CPPC_SENSOR_REAR_WHEEL /* Sensor Location Info */
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_CPP_Sensor_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_CPPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Sensor_Disable command is executed.
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
static BOOL RBLE_CPP_Sensor_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_CPP_Sensor_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Sensor_Send_Measurements command is executed.
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
static BOOL RBLE_CPP_Sensor_Send_Measurements_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_CPP_MEASUREMENTS_INFO meas_info = {
        0x0000,         /* flags */
        100,            /* instant_power */
        170,            /* pedal_balance */
        0x00,           /* reserved */
        0x1000,         /* accumulated_torque */
        0x22223333,     /* wheel_revolutions */
        0x0111,         /* wheel_event */
        0x0222,         /* crank_revolutions */
        0x0333,         /* crank_event */
        300,            /* max_force_magnitude */
        400,            /* min_force_magnitude */
        800,            /* max_torque_magnitude */
        999,            /* min_torque_magnitude */
        0x0888,         /* max_angle */
        0x0999,         /* min_angle */
        0x0aaa,         /* top_dead_spot */
        0x0bbb,         /* bottom_dead_spot */
        0x0ccc          /* accumulated_energy */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            meas_info.flags = (uint16_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_CPP_Sensor_Send_Measurements( Connection_Handle, &meas_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Sensor_Broadcast_Measurements command is executed.
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
static BOOL RBLE_CPP_Sensor_Broadcast_Measurements_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_CPP_MEASUREMENTS_INFO meas_info = {
        0x0000,         /* flags */
        100,            /* instant_power */
        170,            /* pedal_balance */
        0x00,           /* reserved */
        0x1000,         /* accumulated_torque */
        0x22222222,     /* wheel_revolutions */
        0x0111,         /* wheel_event */
        0x0222,         /* crank_revolutions */
        0x0333,         /* crank_event */
        300,            /* max_force_magnitude */
        400,            /* min_force_magnitude */
        1800,           /* max_torque_magnitude */
        9999,           /* min_torque_magnitude */
        0x0888,         /* max_angle */
        0x0999,         /* min_angle */
        0x0aaa,         /* top_dead_spot */
        0x0bbb,         /* bottom_dead_spot */
        0x0ccc          /* accumulated_energy */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            meas_info.flags = (uint16_t)RBLE_Parameter_Arg[ 0 ];
        }

        /* API Call */
        Ret_Status = RBLE_CPP_Sensor_Broadcast_Measurements( Connection_Handle, &meas_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Sensor_Send_Vector command is executed.
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
static BOOL RBLE_CPP_Sensor_Send_Vector_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_CPP_VECTOR_INFO vector_info = {
        0x00,           /* flags */
        0x00,           /* pad */
        0x1111,         /* crank_revolutions */
        0x2222,         /* crank_event */
        0x3333,         /* first_crank_angle */
        0x09,           /* array_num */
        0x00,           /* pad */
        /* magnitude array */
        {0x0001, 0x0012, 0x0123, 0x1234, 0x2345, 0x3456, 0x4567, 0x5678, 0x6789}
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            vector_info.flags = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }

        /* API Call */
        Ret_Status = RBLE_CPP_Sensor_Send_Vector( Connection_Handle, &vector_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Sensor_Send_CP_Control_Point command is executed.
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
static BOOL RBLE_CPP_Sensor_Send_CP_Control_Point_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_CPP_IND_CONTROL_POINT_INFO cp_info = {0};
    
    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        cp_info.OpCode = RBLE_CPP_OP_RESPONSE_CODE;
        if ( 0 != RBLE_Parameter_Cnt ) {
            cp_info.request_op_code = (uint8_t)RBLE_Parameter_Arg[ 0 ];
            if (RBLE_Parameter_Cnt > 1) {
                cp_info.response_value = (uint8_t)RBLE_Parameter_Arg[ 1 ];
                if (RBLE_Parameter_Cnt > 2) {
                    switch(cp_info.request_op_code) {
                        case RBLE_CPP_OP_REQ_CRANK_LEN_CODE:
                            cp_info.crank_length = (uint16_t)RBLE_Parameter_Arg[2];
                            break;
                        case RBLE_CPP_OP_REQ_CHAIN_LEN_CODE:
                            cp_info.chain_length = (uint16_t)RBLE_Parameter_Arg[2];
                            break;
                        case RBLE_CPP_OP_REQ_CHAIN_WEI_CODE:
                            cp_info.chain_weight = (uint16_t)RBLE_Parameter_Arg[2];
                            break;
                        case RBLE_CPP_OP_REQ_SPAN_LEN_CODE:
                            cp_info.span_length = (uint16_t)RBLE_Parameter_Arg[2];
                            break;
                        case RBLE_CPP_OP_START_OFFSET_COMPENSATION_CODE:
                            cp_info.offset_compensation = (int16_t)RBLE_Parameter_Arg[2];
                            break;
                        case RBLE_CPP_OP_REQ_SAMPL_RATE_CODE:
                            cp_info.sampling_rate = (uint8_t)RBLE_Parameter_Arg[2];
                            break;
                        default:
                            break;
                    }
                }
            } else {
                cp_info.response_value = 1;
            }
        }
        else {
            cp_info.request_op_code     = 1;
            cp_info.response_value      = 1;
            cp_info.crank_length    = 1;
        }
        printf("Req OpCode = %d\n",cp_info.request_op_code);
        if(cp_info.request_op_code == RBLE_CPP_OP_REQ_FACTORY_CALIB_DATE_CODE) {
            cp_info.stamp.year  = 2013;
            cp_info.stamp.month = 3;
            cp_info.stamp.day   = 31;
            cp_info.stamp.hour  = 18;
            cp_info.stamp.min   = 12;
            cp_info.stamp.sec   = 14;
        }
        /* API Call */
        Ret_Status = RBLE_CPP_Sensor_Send_CP_Control_Point( Connection_Handle, &cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Sensor_Send_Battery_Level command is executed.
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
static BOOL RBLE_CPP_Sensor_Send_Battery_Level_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t battery_level = 100;
    
    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            battery_level = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        printf("battery_level = %d\n",battery_level);
        /* API Call */
        Ret_Status = RBLE_CPP_Sensor_Send_Battery_Level( Connection_Handle, battery_level );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Sensor_Send_Write_Response command is executed.
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
static BOOL RBLE_CPP_Sensor_Send_Write_Response_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t res_code = RBLE_ATT_ERR_NO_ERROR;
    
    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            res_code = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_CPP_Sensor_Send_Write_Response( Connection_Handle, res_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_CPPS)

#if (PRF_SEL_CPPC)
/******************************************************************************************/
/**
 *  @brief Cycling Power(Collector) Call Back
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
static void RBLE_APP_CPPC_CallBack( RBLE_CPPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_CPP_EVENT_COLLECTOR_ENABLE_COMP:
            CPP_Event_Status_Disp("COLLECTOR_ENABLE_COMP", event->param.collector_enable.status);
            Connection_Handle_Disp( event->param.collector_enable.conhdl );
            printf( "* Cycling Power Service\n");
            printf( " Start Handle = 0x%04X\n", event->param.collector_enable.cps.shdl );
            printf( " End Handle   = 0x%04X\n\n",   event->param.collector_enable.cps.ehdl );
            printf( " meas_char_hdl       = 0x%04X\n",  event->param.collector_enable.cps.cp_meas_char_hdl );
            printf( " meas_val_hdl        = 0x%04X\n",  event->param.collector_enable.cps.cp_meas_val_hdl );
            printf( " meas_cfg_hdl        = 0x%04X\n",  event->param.collector_enable.cps.cp_meas_cfg_hdl );
            printf( " meas_brd_cfg_hdl    = 0x%04X\n",  event->param.collector_enable.cps.cp_meas_brd_cfg_hdl );
            printf( " meas_prop           = 0x%02X\n\n",    event->param.collector_enable.cps.cp_meas_prop );
            printf( " feature_char_hdl    = 0x%04X\n",  event->param.collector_enable.cps.cp_feature_char_hdl );
            printf( " feature_val_hdl     = 0x%04X\n",  event->param.collector_enable.cps.cp_feature_val_hdl );
            printf( " feature_prop        = 0x%02X\n\n",    event->param.collector_enable.cps.cp_feature_prop );
            printf( " sensor_loc_char_hdl = 0x%04X\n",  event->param.collector_enable.cps.sensor_loc_char_hdl );
            printf( " sensor_loc_val_hdl  = 0x%04X\n",  event->param.collector_enable.cps.sensor_loc_val_hdl );
            printf( " sensor_loc_prop     = 0x%02X\n\n",    event->param.collector_enable.cps.sensor_loc_prop );
            printf( " vector_char_hdl     = 0x%04X\n",  event->param.collector_enable.cps.cp_vector_char_hdl );
            printf( " vector_val_hdl      = 0x%04X\n",  event->param.collector_enable.cps.cp_vector_val_hdl );
            printf( " vector_cfg_hdl      = 0x%04X\n",  event->param.collector_enable.cps.cp_vector_cfg_hdl );
            printf( " vector_prop         = 0x%02X\n\n",    event->param.collector_enable.cps.cp_vector_prop );
            printf( " cp_cp_char_hdl      = 0x%04X\n",  event->param.collector_enable.cps.cp_cp_char_hdl );
            printf( " cp_cp_val_hdl       = 0x%04X\n",  event->param.collector_enable.cps.cp_cp_val_hdl );
            printf( " cp_cp_cfg_hdl       = 0x%04X\n",  event->param.collector_enable.cps.cp_cp_cfg_hdl );
            printf( " cp_cp_prop          = 0x%02X\n\n",    event->param.collector_enable.cps.cp_cp_prop );
            DIS_handle_Info_Disp(&event->param.collector_enable.dis);
            BAS_handle_Info_Disp(&event->param.collector_enable.bas);

            g_cps      = event->param.collector_enable.cps;
            g_cpp_dis  = event->param.collector_enable.dis;
            g_cpp_bas  = event->param.collector_enable.bas;
            break;
        case RBLE_CPP_EVENT_COLLECTOR_DISABLE_COMP:
            CPP_Event_Status_Disp("COLLECTOR_DISABLE_COMP", event->param.collector_disable.status);
            Connection_Handle_Disp( event->param.collector_disable.conhdl );
            break;
        case RBLE_CPP_EVENT_COLLECTOR_ERROR_IND:
            CPP_Event_Status_Disp("COLLECTOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_CPP_EVENT_COLLECTOR_MEASUREMENTS_NTF:
            CPP_Event_Disp("COLLECTOR_MEASUREMENTS_NTF");
            Connection_Handle_Disp( event->param.measurements_ntf.conhdl );
            CPP_Meas_Info_Disp( &event->param.measurements_ntf.measurements_info );
            break;
        case RBLE_CPP_EVENT_COLLECTOR_VECTOR_NTF:
            CPP_Event_Disp("COLLECTOR_VECTOR_NTF");
            Connection_Handle_Disp( event->param.vector_ntf.conhdl );
            CPP_Vector_Info_Disp( &event->param.vector_ntf.vector_info );
            break;
        case RBLE_CPP_EVENT_COLLECTOR_CP_CP_IND:
            CPP_Event_Disp("COLLECTOR_CP_CP_IND");
            Connection_Handle_Disp( event->param.cp_cp_ind.conhdl );
            CPP_Ind_Cp_Info_Disp( event );
            break;
        case RBLE_CPP_EVENT_COLLECTOR_BATTERY_LEVEL_NTF:
            CPP_Event_Disp("COLLECTOR_BATTERY_LEVEL_NTF");
            Connection_Handle_Disp( event->param.battery_level_ntf.conhdl );
            printf( "Battery Level = %d\n", event->param.battery_level_ntf.battery_level );
            break;
        case RBLE_CPP_EVENT_COLLECTOR_READ_CHAR_RESPONSE:
            CPP_Event_Status_Disp("COLLECTOR_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            event->param.rd_char_resp.data.data[event->param.rd_char_resp.data.len] = '\0';
            printf("(%s)\n", event->param.rd_char_resp.data.data);
            break;
        case RBLE_CPP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE:
            CPP_Event_Status_Disp("COLLECTOR_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );

            if (RBLE_ATT_ERR_NO_ERROR == event->param.wr_char_resp.att_code)
            {
                if (0x80 & tmp_brd_en)
                {
                    if (0x01 & tmp_brd_en)
                    {
                        g_cpp_brd_en = TRUE;
                    }
                    else
                    {
                        g_cpp_brd_en = FALSE;
                    }
                }
            }
            tmp_brd_en = 0;
            break;
        case RBLE_CPP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND:
            CPP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A CPP_Collector_Enable command is executed.
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
static BOOL RBLE_CPP_Collector_Enable_Test( void )
{
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_cps.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }

        /* API Call */
        Ret_Status = RBLE_CPP_Collector_Enable( Connection_Handle, con_type, &g_cps, &g_cpp_dis, &g_cpp_bas, RBLE_APP_CPPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Collector_Disable command is executed.
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
static BOOL RBLE_CPP_Collector_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_CPP_Collector_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Collector_Read_Char command is executed.
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
static BOOL RBLE_CPP_Collector_Read_Char_Test( void )
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
    if ( ( ILLEGAL_NO == Parameter_No ) || ( RBLE_CPPC_RD_BAS_BL_CFG < Parameter_No ) ) {
        char_code = RBLE_CPPC_RD_CPS_CM_CFG;
    } else {
        char_code = (uint8_t)Parameter_No;
    }

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_CPP_Collector_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Collector_Write_CP_Control_Point command is executed.
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
static BOOL RBLE_CPP_Collector_Write_CP_Control_Point_Test( void )
{
    RBLE_STATUS     Ret_Status;
    RBLE_CPP_WR_CONTROL_POINT_INFO cp_info = {0};
    int32_t  tmp_value = 0;
    
    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        cp_info.OpCode = RBLE_CPP_OP_RESPONSE_CODE;
        if ( 0 != RBLE_Parameter_Cnt ) {
            cp_info.OpCode = (uint8_t)RBLE_Parameter_Arg[ 0 ];
            if (RBLE_Parameter_Cnt > 1) {
                tmp_value = RBLE_Parameter_Arg[1];
            }
        }
        else {
            cp_info.OpCode  = 1;
            tmp_value       = 1;
        }
        switch(cp_info.OpCode)
        {
            case RBLE_CPP_OP_SET_CUMULATIVE_CODE:
                cp_info.cumulative_value    = (uint32_t)tmp_value;
                break;
            case RBLE_CPP_OP_UPDATE_SL_CODE:
                cp_info.sensor_location     = (uint8_t)tmp_value;
                break;
            case RBLE_CPP_OP_SET_CRANK_LEN_CODE:
                cp_info.crank_length        = (uint16_t)tmp_value;
                break;
            case RBLE_CPP_OP_SET_CHAIN_LEN_CODE:
                cp_info.chain_length        = (uint16_t)tmp_value;
                break;
            case RBLE_CPP_OP_SET_CHAIN_WEI_CODE:
                cp_info.chain_weight        = (uint16_t)tmp_value;
                break;
            case RBLE_CPP_OP_SET_SPAN_LEN_CODE:
                cp_info.span_length         = (uint16_t)tmp_value;
                break;
            case RBLE_CPP_OP_MASK_CP_MEAS_CONTENT_CODE:
                cp_info.mask_meas_content   = (uint16_t)tmp_value;
                break;
            default:
                break;
        }
        printf( "OpCode :%d, value:%d(%x)\n",cp_info.OpCode, tmp_value, tmp_value);
        /* API Call */
        Ret_Status = RBLE_CPP_Collector_Write_CP_Control_Point( Connection_Handle, &cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A CPP_Collector_Write_Char command is executed.
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
static BOOL RBLE_CPP_Collector_Write_Char_Test( void )
{
    RBLE_STATUS                 Ret_Status;
    int32_t                     Parameter_No;
    uint16_t                    cfg_val = RBLE_PRF_STOP_NTFIND;
    uint8_t                     char_code;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if(RBLE_Parameter_Cnt > 1)
            {
                cfg_val = (uint16_t)RBLE_Parameter_Arg[ 1 ];
            }
        } else {
            Parameter_No = 0;
        }

        switch( Parameter_No ){
        case 0:
        default:
            char_code = RBLE_CPPC_CYCPWR_MEAS_CODE;
            break;
        case 1:
            char_code = RBLE_CPPC_CYCPWR_MEAS_BRD_CODE;
            break;
        case 2:
            char_code = RBLE_CPPC_CYCPWR_VCTR_CODE;
            break;
        case 3:
            char_code = RBLE_CPPC_CYCPWR_CONTROL_POINT_CODE;
            break;
        case 4:
            char_code = RBLE_CPPC_BATTERY_LEVEL_CODE;
            break;
        }

        /* API Call */
        Ret_Status = RBLE_CPP_Collector_Write_Char( Connection_Handle, char_code, cfg_val );
        printf( " char_code = %d, cfg_val = 0x%04X\n", char_code, cfg_val );
        tmp_brd_en = 0;
        if (RBLE_CPPC_CYCPWR_MEAS_BRD_CODE == char_code)
        {
            if (RBLE_PRF_START_BRD == cfg_val)
            {
                tmp_brd_en = 0x81;
            }
            else
            {
                tmp_brd_en = 0x80;
            }
        }
        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_CPPC)


#if (PRF_SEL_CPPC)
/******************************************************************************************/
/**
 *  @brief Display CP Measurements Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      CP Measurements Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void CPP_Meas_Info_Disp(RBLE_CPP_MEASUREMENTS_INFO *meas_info)
{
    printf("flags :%04x\n", meas_info->flags);
    printf("Instant Power      :%d(0x%04x)\n", meas_info->instant_power, meas_info->instant_power);
    
    if(meas_info->flags & 0x0001) {
        /* Pedal Power Balance */
        printf("Pedal Power Balance:%u(0x%02x)\n", meas_info->pedal_balance, meas_info->pedal_balance);
    }
    if(meas_info->flags & 0x0004) {
        /* Accumulated Torque */
        printf("Accumulated Torque :%u(0x%04x)\n", meas_info->accumulated_torque, meas_info->accumulated_torque);
    }
    if(meas_info->flags & 0x0010) {
        /* Wheel Revolution Data */
        printf("Wheel Revolution   :%lu(0x%08lx)\n", meas_info->wheel_revolutions, meas_info->wheel_revolutions);
        printf("Wheel Event Time   :%u(0x%04x)\n", meas_info->wheel_event, meas_info->wheel_event);
    }
    if(meas_info->flags & 0x0020) {
        /* Crank Revolution Data */
        printf("Crank Revolution   :%u(0x%04x)\n", meas_info->crank_revolutions, meas_info->crank_revolutions);
        printf("Crank Event Time   :%u(0x%04x)\n", meas_info->crank_event, meas_info->crank_event);
    }
    if(meas_info->flags & 0x0040) {
        /* Extreme Force Magnitudes */
        printf("Max Force Mag      :%d(0x%04x)\n", meas_info->max_force_magnitude, meas_info->max_force_magnitude);
        printf("Min Force Mag      :%d(0x%04x)\n", meas_info->min_force_magnitude, meas_info->min_force_magnitude);
    }
    if(meas_info->flags & 0x0080) {
        /* Extreme Torque Magnitudes */
        printf("Max Torque Mag     :%d(0x%04x)\n", meas_info->max_torque_magnitude, meas_info->max_torque_magnitude);
        printf("Min Torque Mag     :%d(0x%04x)\n", meas_info->min_torque_magnitude, meas_info->min_torque_magnitude);
    }
    if(meas_info->flags & 0x0100) {
        /* Extreme Angles */
        printf("Max Ext Angles     :%u(0x%04x)\n", meas_info->max_angle, meas_info->max_angle);
        printf("Min Ext Angles     :%u(0x%04x)\n", meas_info->min_angle, meas_info->min_angle);
    }
    if(meas_info->flags & 0x0200) {
        /* Top Dead Spot Angle */
        printf("Top Dead Angle     :%u(0x%04x)\n", meas_info->top_dead_spot, meas_info->top_dead_spot);
    }
    if(meas_info->flags & 0x0400) {
        /* Bottom Dead Spot */
        printf("Btm Dead Angle     :%u(0x%04x)\n", meas_info->bottom_dead_spot, meas_info->bottom_dead_spot);
    }
    if(meas_info->flags & 0x0800) {
        /* Accumulated Energy */
        printf("Accumulated Energy :%u(0x%04x)\n", meas_info->accumulated_energy, meas_info->accumulated_energy);
    }
}

/******************************************************************************************/
/**
 *  @brief Display CP Vector Info
 *
 *  @b Contents of vector:
 *
 *  @param[in] *p      CP Vector Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void CPP_Vector_Info_Disp(RBLE_CPP_VECTOR_INFO *vector_info)
{
    uint8_t i;
    
    printf("flags :%02x\n", vector_info->flags);
    if(vector_info->flags & 0x01) {
        /* Crank Revolution Data */
        printf("Crank Rev   :0x%x(%u)\n", vector_info->crank_revolutions, vector_info->crank_revolutions);
        printf("Crank Event :0x%x(%u)\n", vector_info->crank_event, vector_info->crank_event);
    }
    
    if(vector_info->flags & 0x02) {
        /* First Crank Measurement Angle Data */
        printf("first crank angle  :%u\n", vector_info->first_crank_angle);
    }
    
    if((vector_info->flags & 0x04) || (vector_info->flags & 0x08)) {
        /* Instantaneous Force/Torque Magnitude Array  Data */
        if(vector_info->flags & 0x04) {
            printf("instant force mag  :");
        }
        else {
            printf("instant torque mag :");
        }
        for(i = 0; i < vector_info->array_num; i++)
        {
            printf("0x%04x(%d) ",vector_info->magnitude[i],vector_info->magnitude[i]);
        }
        printf("\n");
    }
}

/******************************************************************************************/
/**
 *  @brief Display Control Point Response value and Parameter value disp
 *
 *  @b Contents of control point Indication:
 *
 *  @param[in] req_opcode   request operation code
 *  @param[in] res_value    response value
 *  @param[in] value        parameter value
 *
 *  @retval none
 */
/******************************************************************************************/
static void CPP_Res_Value_Disp(uint8_t req_opcode, uint8_t res_value, int32_t value)
{
    printf("%s :RES = %d, Value = %d\n", CPP_Control_Point_Disp_Info[req_opcode], res_value, value);
}

/******************************************************************************************/
/**
 *  @brief Display CP Control Point Indication Info
 *
 *  @b Contents of control point Indication:
 *
 *  @param[in] *event   CP Control Point Info(Indication)
 *
 *  @retval none
 */
/******************************************************************************************/
static void CPP_Ind_Cp_Info_Disp(RBLE_CPPC_EVENT *event)
{
    RBLE_CPP_IND_CONTROL_POINT_INFO *ind_cp_info = &event->param.cp_cp_ind.ind_cp_info;
    
    if(ind_cp_info->OpCode == RBLE_CPP_OP_RESPONSE_CODE) {
        printf("OpCode = RESPONSE\n");
    }
    else {
        printf("Illegal OpCode = %02x\n", ind_cp_info->OpCode);
    }
    switch(ind_cp_info->request_op_code)
    {
        case RBLE_CPP_OP_SET_CUMULATIVE_CODE:
        case RBLE_CPP_OP_SET_CRANK_LEN_CODE:
        case RBLE_CPP_OP_UPDATE_SL_CODE:
        case RBLE_CPP_OP_SET_CHAIN_LEN_CODE:
        case RBLE_CPP_OP_SET_CHAIN_WEI_CODE:
        case RBLE_CPP_OP_SET_SPAN_LEN_CODE:
        case RBLE_CPP_OP_MASK_CP_MEAS_CONTENT_CODE:
        case RBLE_CPP_OP_REQ_SUPPORTED_SL_CODE:
        case RBLE_CPP_OP_REQ_FACTORY_CALIB_DATE_CODE:
            printf("%s :RES = %d\n", CPP_Control_Point_Disp_Info[ind_cp_info->request_op_code], ind_cp_info->response_value);
            if(ind_cp_info->response_value == RBLE_CPP_RES_SUCCESS_CODE) {
                if(ind_cp_info->request_op_code == RBLE_CPP_OP_REQ_SUPPORTED_SL_CODE) {
                    printf("location num%d\n", event->param.cp_cp_ind.location_num);
                    Print_Dump( &event->param.cp_cp_ind.supported_location[0] ,event->param.cp_cp_ind.location_num );
                }
                else if(ind_cp_info->request_op_code == RBLE_CPP_OP_REQ_FACTORY_CALIB_DATE_CODE) {
                    RBLE_DATE_TIME *stamp = &ind_cp_info->stamp;
                    printf("%04d/%02d/%02d %02d:%02d:%02d\n", stamp->year, stamp->month, stamp->day, stamp->hour, stamp->min, stamp->sec);
                }
            }
            break;
        case RBLE_CPP_OP_REQ_CRANK_LEN_CODE:
            CPP_Res_Value_Disp(ind_cp_info->request_op_code,ind_cp_info->response_value, ind_cp_info->crank_length);
            break;
        case RBLE_CPP_OP_REQ_CHAIN_LEN_CODE:
            CPP_Res_Value_Disp(ind_cp_info->request_op_code,ind_cp_info->response_value, ind_cp_info->chain_length);
            break;
        case RBLE_CPP_OP_REQ_CHAIN_WEI_CODE:
            CPP_Res_Value_Disp(ind_cp_info->request_op_code,ind_cp_info->response_value, ind_cp_info->chain_weight);
            break;
        case RBLE_CPP_OP_REQ_SPAN_LEN_CODE:
            CPP_Res_Value_Disp(ind_cp_info->request_op_code,ind_cp_info->response_value, ind_cp_info->span_length);
            break;
        case RBLE_CPP_OP_START_OFFSET_COMPENSATION_CODE:
            CPP_Res_Value_Disp(ind_cp_info->request_op_code,ind_cp_info->response_value, ind_cp_info->offset_compensation);
            break;
        case RBLE_CPP_OP_REQ_SAMPL_RATE_CODE:
            CPP_Res_Value_Disp(ind_cp_info->request_op_code,ind_cp_info->response_value, ind_cp_info->sampling_rate);
            break;
        default:
            break;
    }
}

/******************************************************************************************/
/**
 *  @brief Display Broadcast data
 *
 *  @b Contents of management:
 *
 *  @param[in] *adv_rep_p   Advertising report
 *
 *  @retval none
 */
/******************************************************************************************/
void CPP_Broadcast_Rcv(RBLE_ADV_REPORT *adv_rep_p )
{
    uint8_t     len, ad_struct_len;
    uint16_t    uuid;
    uint8_t     *p;
    RBLE_CPP_MEASUREMENTS_INFO meas_info;

    len = adv_rep_p->data_len;
    p = &adv_rep_p->data[0];
    for (; 0 != len;) {
        ad_struct_len = *p;
        /* ServiceData? */
        if (0x16 == *(p+1)) {
            printf("Received Measurements Broadcast -- ");
            BdAddress_Disp( &adv_rep_p->adv_addr );
            p+=2;   /* Length + Type */
            uuid = (uint16_t)( *p | (*(p+1) << 8) );
            p+=2;   /* uuid */
            if ((RBLE_SVC_CYCLING_POWER == uuid) || (RBLE_CHAR_CYCLING_POWER_MEAS == uuid/* for PTS bug */)) {
                memset(&meas_info, 0, sizeof(meas_info));
                meas_info.flags = (uint16_t)( *p | (*(p+1) << 8) );
                p+=2;
                meas_info.instant_power = (int16_t)( *p | (*(p+1) << 8) );
                p+=2;
                if(meas_info.flags & 0x0001) {
                    /* Pedal Power Balance */
                    meas_info.pedal_balance = *p;
                    p++;
                }
                if(meas_info.flags & 0x0004) {
                    /* Accumulated Torque */
                    meas_info.accumulated_torque = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                if(meas_info.flags & 0x0010) {
                    /* Wheel Revolution Data */
                    meas_info.wheel_revolutions = (uint32_t)( *p | (*(p+1) << 8)  | ((uint32_t)*(p+2) << 16) | ((uint32_t)*(p+3) << 24));
                    p+=4;
                    meas_info.wheel_event = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                if(meas_info.flags & 0x0020) {
                    /* Crank Revolution Data */
                    meas_info.crank_revolutions = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                    meas_info.crank_event = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                if(meas_info.flags & 0x0040) {
                    /* Extreme Force Magnitudes */
                    meas_info.max_force_magnitude = (int16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                    meas_info.min_force_magnitude = (int16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                if(meas_info.flags & 0x0080) {
                    /* Extreme Torque Magnitudes */
                    meas_info.max_torque_magnitude = (int16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                    meas_info.min_torque_magnitude = (int16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                if(meas_info.flags & 0x0100) {
                    /* Extreme Angles */
                    meas_info.max_angle = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                    meas_info.min_angle = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                if(meas_info.flags & 0x0200) {
                    /* Top Dead Spot Angle */
                    meas_info.top_dead_spot = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                if(meas_info.flags & 0x0400) {
                    /* Bottom Dead Spot */
                    meas_info.bottom_dead_spot = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                if(meas_info.flags & 0x0800) {
                    /* Accumulated Energy */
                    meas_info.accumulated_energy = (uint16_t)( *p | (*(p+1) << 8) );
                    p+=2;
                }
                CPP_Meas_Info_Disp(&meas_info);
            } else {
                Print_Dump(p, ad_struct_len-2);
                p = p + (ad_struct_len - 2);
            }
        } else {
            p = p + (ad_struct_len + 1);
        }

        if (len >= (ad_struct_len + 1)) {
            len = (uint8_t)(len - (ad_struct_len + 1));
        } else {
            len = 0;
        }
    }
}
#endif  //#if (PRF_SEL_CPPC)

#if (PRF_SEL_CPPS)
/******************************************************************************************/
/**
 *  @brief Display Control Point Write value and Parameter value disp
 *
 *  @b Contents of control point Indication:
 *
 *  @param[in] opcode   request operation code
 *  @param[in] value    parameter value
 *
 *  @retval none
 */
/******************************************************************************************/
static void CPP_Write_Value_Disp(uint8_t opcode, int32_t value)
{
    printf("%s :Value = %d\n", CPP_Control_Point_Disp_Info[opcode], value);
}

/******************************************************************************************/
/**
 *  @brief Display CP Control Point Info
 *
 *  @b Contents of control point(Write):
 *
 *  @param[in] *p      CP Control Point Info(Write)
 *
 *  @retval none
 */
/******************************************************************************************/
static void CPP_Chg_Cp_Info_Disp(RBLE_CPP_WR_CONTROL_POINT_INFO *wr_info)
{
    switch(wr_info->OpCode)
    {
        case RBLE_CPP_OP_REQ_SUPPORTED_SL_CODE:
        case RBLE_CPP_OP_REQ_CRANK_LEN_CODE:
        case RBLE_CPP_OP_REQ_CHAIN_LEN_CODE:
        case RBLE_CPP_OP_REQ_CHAIN_WEI_CODE:
        case RBLE_CPP_OP_REQ_SPAN_LEN_CODE:
        case RBLE_CPP_OP_START_OFFSET_COMPENSATION_CODE:
        case RBLE_CPP_OP_REQ_SAMPL_RATE_CODE:
        case RBLE_CPP_OP_REQ_FACTORY_CALIB_DATE_CODE:
            printf("%s\n", CPP_Control_Point_Disp_Info[wr_info->OpCode]);
            break;      
        case RBLE_CPP_OP_SET_CUMULATIVE_CODE:
            CPP_Write_Value_Disp(wr_info->OpCode, wr_info->cumulative_value);
            break;
        case RBLE_CPP_OP_UPDATE_SL_CODE:
            CPP_Write_Value_Disp(wr_info->OpCode, wr_info->sensor_location);
            break;
        case RBLE_CPP_OP_SET_CRANK_LEN_CODE:
            CPP_Write_Value_Disp(wr_info->OpCode, wr_info->crank_length);
            break;
        case RBLE_CPP_OP_SET_CHAIN_LEN_CODE:
            CPP_Write_Value_Disp(wr_info->OpCode, wr_info->chain_length);
            break;
        case RBLE_CPP_OP_SET_CHAIN_WEI_CODE:
            CPP_Write_Value_Disp(wr_info->OpCode, wr_info->chain_weight);
            break;
        case RBLE_CPP_OP_SET_SPAN_LEN_CODE:
            CPP_Write_Value_Disp(wr_info->OpCode, wr_info->span_length);
            break;
        case RBLE_CPP_OP_MASK_CP_MEAS_CONTENT_CODE:
            CPP_Write_Value_Disp(wr_info->OpCode, wr_info->mask_meas_content);
            break;
        default:
            printf("Illegal OpCode %d\n",wr_info->OpCode);
            break;
    }
}
#endif  //#if (PRF_SEL_CPPS)

/******************************************************************************************/
/**
 *  @brief rBLE CPP Event and Status Disp
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
static void CPP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE CPP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}
/******************************************************************************************/
/**
 *  @brief rBLE CPP Event Disp
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
static void CPP_Event_Disp(char_t *event_str)
{
    printf( "\nrBLE CPP EVENT (%s)\n", event_str );
}


#endif  //#if (PRF_SEL_CPPC || PRF_SEL_CPPS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
