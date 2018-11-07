/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_glp.c
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

#if (PRF_SEL_GLPC || PRF_SEL_GLPS)
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
#if (PRF_SEL_GLPS)
static void RBLE_APP_GLPS_CallBack( RBLE_GLPS_EVENT *event );                               /* GLP(Sensor) Call Back */
static BOOL RBLE_GLP_Sensor_Enable_Test( void );                                            /* A GLP_Sensor_Enable command is executed. */
static BOOL RBLE_GLP_Sensor_Disable_Test( void );                                           /* A GLP_Sensor_Disable command is executed. */
static BOOL RBLE_GLP_Sensor_Send_Measurements_Test( void );                                 /* A GLP_Sensor_Send_Measurements command is executed. */
static BOOL RBLE_GLP_Sensor_Send_Measurements_Context_Test( void );                         /* A GLP_Sensor_Send_Measurements_Context command is executed. */
static BOOL RBLE_GLP_Sensor_Send_RA_Control_Point_Test( void );                             /* A GLP_Sensor_Send_RA_Control_Point command is executed. */
#endif
#if (PRF_SEL_GLPC)
static void RBLE_APP_GLPC_CallBack( RBLE_GLPC_EVENT *event );                               /* GLP(Collector) Call Back */
static BOOL RBLE_GLP_Collector_Enable_Test( void );                                         /* A GLP_Collector_Enable command is executed. */
static BOOL RBLE_GLP_Collector_Disable_Test( void );                                        /* A GLP_Collector_Disable command is executed. */
static BOOL RBLE_GLP_Collector_Read_Char_Test( void );                                      /* A GLP_Collector_Read_Char command is executed. */
static BOOL RBLE_GLP_Collector_Write_RA_Control_Point_Test( void );                         /* A GLP_Collector_Write_RA_Control_Point command is executed. */
static BOOL RBLE_GLP_Collector_Write_Char_Test( void );                                     /* A GLP_Collector_Write_Char command is executed. */
#endif


#if (PRF_SEL_GLPC)
static void GLP_MEASURE_Info_Disp( RBLE_GLP_MEASUREMENTS_INFO *meas );
static void GLP_MEASURE_Context_Info_Disp( RBLE_GLP_MEASUREMENTS_CONTEXT_INFO *meas );
#endif

static void GLP_RA_CP_Info_Disp( RBLE_GLP_RA_CONTROL_POINT_INFO *cp );
static void GLP_RA_CP_Ind_Info_Disp( RBLE_GLP_RA_CONTROL_POINT_IND_INFO *cp );
static void GLP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status);
static void GLP_Event_Disp(char_t *event_str);


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Glucose Profile Select Menu */
static const MENU_INFO  Test_Prog_GLP_Menu[] = {
#if (PRF_SEL_GLPS)
    {  1,   RBLE_GLP_Sensor_Enable_Test,                        NULL,       "1.GLP Sensor_Enable\n",                        },
    {  2,   RBLE_GLP_Sensor_Disable_Test,                       NULL,       "2.GLP Sensor_Disable\n",                       },
    {  3,   RBLE_GLP_Sensor_Send_Measurements_Test,             NULL,       "3.GLP Sensor_Send_Measurements\n",             },
    {  4,   RBLE_GLP_Sensor_Send_Measurements_Context_Test,     NULL,       "4.GLP Sensor_Send_Measurements_Context\n",     },
    {  5,   RBLE_GLP_Sensor_Send_RA_Control_Point_Test,         NULL,       "5.GLP Sensor_Send_Ra_Control_Point\n",         },
#endif
#if (PRF_SEL_GLPC)
    {  6,   RBLE_GLP_Collector_Enable_Test,                     NULL,       "6.GLP Collector_Enable\n",                     },
    {  7,   RBLE_GLP_Collector_Disable_Test,                    NULL,       "7.GLP Collector_Disable\n",                    },
    {  8,   RBLE_GLP_Collector_Read_Char_Test,                  NULL,       "8.GLP Collector_Read_Char\n",                  },
    {  9,   RBLE_GLP_Collector_Write_RA_Control_Point_Test,     NULL,       "9.GLP Collector_Write_Ra_Control_Point\n",     },
    { 10,   RBLE_GLP_Collector_Write_Char_Test,                 NULL,       "10.GLP Collector_Write_Char\n",                },
#endif
    {  ESC_KEY, Menu_Exit,                                      NULL,       "ESC Key: Menu exit\n",                         },
    {  0,   NULL,                                               NULL,       NULL,                                           }
};

const SELECT_MENU Test_Prog_GLP_Select_Menu = {
    "\n-- BLE Sample Program Glucose Profile Test Menu --",
    Test_Prog_GLP_Menu
};


#if (PRF_SEL_GLPC)
static RBLE_GLS_CONTENT    g_gls = {0};
static RBLE_DIS_CONTENT    g_glp_dis = {0};
#endif


/******************************* Program Area *****************************************/
#if (PRF_SEL_GLPS)
/******************************************************************************************/
/**
 *  @brief rBLE GLP(Sensor) Call Back
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
static void RBLE_APP_GLPS_CallBack( RBLE_GLPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_GLP_EVENT_SENSOR_ENABLE_COMP:
            GLP_Event_Status_Disp("SENSOR_ENABLE_COMP", event->param.sensor_enable.status);
            Connection_Handle_Disp( event->param.sensor_enable.conhdl );
            break;
        case RBLE_GLP_EVENT_SENSOR_DISABLE_COMP:
            GLP_Event_Disp("SENSOR_DISABLE_COMP");
            Connection_Handle_Disp( event->param.sensor_disable.conhdl );
            break;
        case RBLE_GLP_EVENT_SENSOR_ERROR_IND:
            GLP_Event_Status_Disp("SENSOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_GLP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP:
            GLP_Event_Status_Disp("SENSOR_SEND_MEASUREMENTS_COMP", event->param.send_measurements.status);
            Connection_Handle_Disp( event->param.send_measurements.conhdl );
            break;
        case RBLE_GLP_EVENT_SENSOR_SEND_MEASUREMENTS_CONTEXT_COMP:
            GLP_Event_Status_Disp("SENSOR_SEND_MEASUREMENTS_CONTEXT_COMP", event->param.send_measurements_context.status);
            Connection_Handle_Disp( event->param.send_measurements_context.conhdl );
            break;
        case RBLE_GLP_EVENT_SENSOR_SEND_RA_CP_COMP:
            GLP_Event_Status_Disp("SENSOR_SEND_RA_CP_COMP", event->param.send_ra_cp.status);
            Connection_Handle_Disp( event->param.send_ra_cp.conhdl );
            break;
        case RBLE_GLP_EVENT_SENSOR_CHG_RA_CP_IND:
            GLP_Event_Disp("SENSOR_CHG_RA_CP_IND");
            GLP_RA_CP_Info_Disp( &event->param.glps_chg_ra_cp_ind.ra_cp_info );
            break;
        case RBLE_GLP_EVENT_SENSOR_CFG_INDNTF_IND:
            GLP_Event_Disp("SENSOR_CFG_INDNTF_IND");
            printf( "  char_code = %d Cfg Value = %s\n", event->param.glps_cfg_indntf_ind.char_code, 
                    ( event->param.glps_cfg_indntf_ind.cfg_val == RBLE_PRF_STOP_NTFIND ) ? "STOP" : "START" );
            break;
        case RBLE_GLP_EVENT_SENSOR_COMMAND_DISALLOWED_IND:
            GLP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
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
static BOOL RBLE_GLP_Sensor_Enable_Test( void )
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

    RBLE_GLP_SENSOR_PARAM   param = {
        RBLE_PRF_STOP_NTFIND,         /* Glucose measurement notification configuration */
        RBLE_PRF_STOP_NTFIND,         /* Glucose measurement context notification configuration */
        RBLE_PRF_STOP_NTFIND,         /* Glucose ra control point indication configuration */
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
        Ret_Status = RBLE_GLP_Sensor_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_GLPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GLP_Sensor_Disable command is executed. 
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
static BOOL RBLE_GLP_Sensor_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GLP_Sensor_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GLP_Sensor_Send_Measurements command is executed. 
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
static BOOL RBLE_GLP_Sensor_Send_Measurements_Test( void )
{
    static RBLE_GLP_MEASUREMENTS_INFO  measurements_info = 
    {
        0x01,           /* flags */
        0x00,           /* reserved */
        0,              /* seq_num */
        {               /* stamp */
            2013,       /* year */
            3,          /* month */
            28,         /* day */
            15,         /* hour */
            27,         /* min */
            41          /* sec */
        },
        0,              /* time offset */
        0xb321,         /* concentration 801*10^-5*/
        RBLE_GLP_TYPE_VENOUS_WHOLE_BLOOD,   /* type */
        RBLE_GLP_SAMPLELOC_EARLOBE,         /* sample location */
        RBLE_GLP_SENSORSTATUS_RESULT_HIGH   /* sensor status annun */
    };
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        printf( "flags = %02x  seq_num = %d\n", measurements_info.flags, measurements_info.seq_num);
        /* API Call */
        Ret_Status = RBLE_GLP_Sensor_Send_Measurements( Connection_Handle, &measurements_info);
        measurements_info.seq_num++;
        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GLP_Sensor_Send_Measurements_Context command is executed. 
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
static BOOL RBLE_GLP_Sensor_Send_Measurements_Context_Test( void )
{
    RBLE_GLP_MEASUREMENTS_CONTEXT_INFO  measurements_context_info = 
    {
        0x00,           /* flags */
        0x00,           /* reserved */
        0x0008,         /* seq_num */
        0xff,           /* ex_flags */
        RBLE_GLP_CARBOHYDRATEID_DINNER,         /* carbohydrate_id */
        0xd111,         /* carbohydrate_kg */
        RBLE_GLP_MEAL_POSTPRANDIAL,             /* meal */
        RBLE_GLP_TESTER_SELF,                   /* tester */
        RBLE_GLP_HEALTH_NO_ISSUE,               /* health */
        0x00,           /* reserved */
        0x2000,         /* exercise_duration */
        0x11,           /* exercise_intensity */
        RBLE_GLP_MEDICATIONID_RAPID_INSULIN,    /* medication_id */
        0xa321,         /* medication */
        0x0021          /* HbA1c */
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GLP_Sensor_Send_Measurements_Context( Connection_Handle, &measurements_context_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GLP_Sensor_Send_Ra_Control_Point command is executed. 
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
static BOOL RBLE_GLP_Sensor_Send_RA_Control_Point_Test( void )
{
    RBLE_GLP_RA_CONTROL_POINT_IND_INFO  ra_cp_ind_info = 
    {
        RBLE_GLP_OPCODE_RESPONSE_CODE,      /* operation code */
        RBLE_GLP_OPERATOR_NULL,             /* operator */
        0x0003,                             /* number of records */
        RBLE_GLP_OPCODE_REPORT_RECORDS,     /* Request Op Code */
        RBLE_GLP_OPERAND_SUCCESS,           /* Response Code Value */
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GLP_Sensor_Send_RA_Control_Point( Connection_Handle, &ra_cp_ind_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_GLPS)


#if (PRF_SEL_GLPC)
/******************************************************************************************/
/**
 *  @brief rBLE GLP(Collector) Call Back
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
static void RBLE_APP_GLPC_CallBack( RBLE_GLPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_GLP_EVENT_COLLECTOR_ENABLE_COMP:
            GLP_Event_Status_Disp("COLLECTOR_ENABLE_COMP", event->param.collector_enable.status);
            Connection_Handle_Disp( event->param.collector_enable.conhdl );
            printf("* Glucose Service\n");
            printf(" Start Handle = 0x%04X\n", event->param.collector_enable.gls.shdl );
            printf(" End Handle   = 0x%04X\n\n", event->param.collector_enable.gls.ehdl );
            printf(" meas_char_hdl             = 0x%04X\n", event->param.collector_enable.gls.glucose_meas_char_hdl );
            printf(" meas_val_hdl              = 0x%04X\n", event->param.collector_enable.gls.glucose_meas_val_hdl );
            printf(" meas_cfg_hdl              = 0x%04X\n", event->param.collector_enable.gls.glucose_meas_cfg_hdl );
            printf(" meas_prop                 = 0x%02X\n\n",   event->param.collector_enable.gls.glucose_meas_prop );
            printf(" meas_context_char_hdl     = 0x%04X\n", event->param.collector_enable.gls.glucose_meas_context_char_hdl );
            printf(" meas_context_val_hdl      = 0x%04X\n", event->param.collector_enable.gls.glucose_meas_context_val_hdl );
            printf(" meas_context_cfg_hdl      = 0x%04X\n", event->param.collector_enable.gls.glucose_meas_context_cfg_hdl );
            printf(" meas_context_prop         = 0x%02X\n\n",   event->param.collector_enable.gls.glucose_meas_context_prop );
            printf(" feature_char_hdl          = 0x%04X\n", event->param.collector_enable.gls.glucose_feature_char_hdl );
            printf(" feature_val_hdl           = 0x%04X\n", event->param.collector_enable.gls.glucose_feature_val_hdl );
            printf(" feature_prop              = 0x%04X\n\n",   event->param.collector_enable.gls.glucose_feature_prop );
            printf(" ra_control_point_char_hdl = 0x%04X\n", event->param.collector_enable.gls.glucose_ra_cp_char_hdl );
            printf(" ra_control_point_val_hdl  = 0x%04X\n", event->param.collector_enable.gls.glucose_ra_cp_val_hdl );
            printf(" ra_control_point_cfg_hdl  = 0x%04X\n", event->param.collector_enable.gls.glucose_ra_cp_cfg_hdl );
            printf(" ra_control_point_prop     = 0x%02X\n\n",event->param.collector_enable.gls.glucose_ra_cp_prop );
            DIS_handle_Info_Disp(&event->param.collector_enable.dis);

            g_gls = event->param.collector_enable.gls;
            g_glp_dis = event->param.collector_enable.dis;
            break;
        case RBLE_GLP_EVENT_COLLECTOR_DISABLE_COMP:
            GLP_Event_Status_Disp("COLLECTOR_DISABLE_COMP", event->param.collector_disable.status);
            Connection_Handle_Disp( event->param.collector_disable.conhdl );
            break;
        case RBLE_GLP_EVENT_COLLECTOR_ERROR_IND:
            GLP_Event_Status_Disp("COLLECTOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_GLP_EVENT_COLLECTOR_MEASUREMENTS_NTF:
            GLP_Event_Disp("COLLECTOR_MEASUREMENTS_NTF");
            GLP_MEASURE_Info_Disp( &event->param.measurements_ntf.measure_info );
            break;
        case RBLE_GLP_EVENT_COLLECTOR_MEASUREMENTS_CONTEXT_NTF:
            GLP_Event_Disp("COLLECTOR_MEASUREMENTS_CONTEXT_NTF");
            GLP_MEASURE_Context_Info_Disp( &event->param.measurements_context_ntf.measure_context_info );
            break;
        case RBLE_GLP_EVENT_COLLECTOR_RA_CP_IND:
            GLP_Event_Disp("COLLECTOR_RA_CP_IND");
            GLP_RA_CP_Ind_Info_Disp( &event->param.ra_cp_ind.ra_cp_ind_info);
            break;
        case RBLE_GLP_EVENT_COLLECTOR_READ_CHAR_RESPONSE:
            GLP_Event_Status_Disp("COLLECTOR_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            event->param.rd_char_resp.data.data[event->param.rd_char_resp.data.len] = '\0';
            printf("(%s)\n", event->param.rd_char_resp.data.data);
            break;
        case RBLE_GLP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE:
            GLP_Event_Status_Disp("COLLECTOR_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_GLP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND:
            GLP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A GLP_Collector_Enable command is executed. 
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
static BOOL RBLE_GLP_Collector_Enable_Test( void )
{

    RBLE_STATUS Ret_Status;
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_gls.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }

        /* API Call */
        Ret_Status = RBLE_GLP_Collector_Enable( Connection_Handle, con_type, &g_gls, &g_glp_dis, RBLE_APP_GLPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GLP_Collector_Disable command is executed. 
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
static BOOL RBLE_GLP_Collector_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GLP_Collector_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GLP_Collector_Read_Char command is executed. 
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
static BOOL RBLE_GLP_Collector_Read_Char_Test( void )
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
    if ( ( ILLEGAL_NO == Parameter_No ) || ( RBLE_GLPC_RD_DIS_IEEE < Parameter_No ) ) {
        char_code = RBLE_GLPC_RD_MEASUREMENT_CFG;
    } else {
        char_code = (uint8_t)Parameter_No;
    }

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GLP_Collector_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RBLE_GLP_Collector_Write_RA_Control_Point_Test command is executed. 
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
static BOOL RBLE_GLP_Collector_Write_RA_Control_Point_Test( void )
{
    RBLE_STATUS     Ret_Status;

    static RBLE_GLP_RA_CONTROL_POINT_INFO   ra_cp_info = 
    {
        RBLE_GLP_OPCODE_REPORT_RECORDS,     /* operation code */
        RBLE_GLP_OPERATOR_ALL_RECORDS,      /* oprator */
        0x00,           /* operand value */
        0x00,           /* min sequence */
        0x50,           /* max sequence */
        0x00,
        {               /* min_stamp */
            2013,
            3,
            29,
            14,
            23,
            45
        },
        {               /* max_stamp */
            2013,
            3,
            29,
            14,
            25,
            36
        }
    };
    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GLP_Collector_Write_RA_Control_Point( Connection_Handle, &ra_cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GLP_Collector_Write_Char command is executed. 
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
static BOOL RBLE_GLP_Collector_Write_Char_Test( void )
{
    RBLE_STATUS                 Ret_Status;
    uint32_t                    Parameter;
    uint32_t                    char_code = RBLE_GLPC_WR_MEASUREMENT_CODE;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            char_code = RBLE_Parameter_Arg[ 0 ];
        }
        if (1 < RBLE_Parameter_Cnt) {
            Parameter = RBLE_Parameter_Arg[ 1 ];
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

        /* API Call */
        Ret_Status = RBLE_GLP_Collector_Write_Char( Connection_Handle, (uint8_t)char_code, (uint16_t)Parameter );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_GLPC)


#if (PRF_SEL_GLPC)
/******************************************************************************************/
/**
 *  @brief Display GLP Measurement Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *meas      GLP Measurement Info
 *
 *  @retval none
 */
/******************************************************************************************/
void GLP_MEASURE_Info_Disp( RBLE_GLP_MEASUREMENTS_INFO *meas )
{
    float tmp_value;
    const char *unit[] = { "kg", "mol" };
    
    printf("Flag:%02x  Seq Num:%04x\n", meas->flags, meas->seq_num);
    printf("Base Time  :%04d/%02d/%02d %02d:%02d:%02d\n", meas->stamp.year, meas->stamp.month, meas->stamp.day, meas->stamp.hour, meas->stamp.min, meas->stamp.sec );
    
    if ( 0 != (meas->flags & 0x01) ) {
        printf("Time offset:%d\n", meas->time_offset);
    }
    if ( 0 != (meas->flags & 0x02) ) {
        tmp_value = SFLOAT2float(meas->concentration);
        printf("concentrate:%x %f(%s/l)\n", (uint16_t)meas->concentration, tmp_value, unit[((meas->flags & 0x04) >> 2)] );
        printf("Type       :%d\n", meas->type);
        printf("Location   :%d\n", meas->sample_location);
    }
    if ( 0 != (meas->flags & 0x08) ) {
        printf("status Annun:%d\n", meas->sensor_status_annun);
    }
}

/******************************************************************************************/
/**
 *  @brief Display GLP Measurement Context Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *meas      GLP Measurement Context Info
 *
 *  @retval none
 */
/******************************************************************************************/
void GLP_MEASURE_Context_Info_Disp( RBLE_GLP_MEASUREMENTS_CONTEXT_INFO *meas )
{
    const char *unit[] = { "kg", "l" };
    float tmp_value;
    
    printf("Flag:%02x Seq Num:%04x\n", meas->flags, meas->seq_num);
    
    if ( 0 != (meas->flags & 0x80) ) {
        printf("Ext Flags    :%04x\n", meas->ex_flags);
    }
    if ( 0 != (meas->flags & 0x01) ) {
        printf("CarboHyd ID  :%04x\n", meas->carbohydrate_id);
        tmp_value = SFLOAT2float(meas->carbohydrate_kg);
        printf("CarboHyd     :%04x %f(kg)\n", (uint16_t)meas->carbohydrate_kg, tmp_value);
    }
    if ( 0 != (meas->flags & 0x02) ) {
        printf("Meal         :%02d\n", meas->meal);
    }
    if ( 0 != (meas->flags & 0x04) ) {
        printf("Tester       :%02d\n", meas->tester);
        printf("Health       :%02d\n", meas->health);
    }
    if ( 0 != (meas->flags & 0x08) ) {
        printf("Exercise Duration :%04x\n", meas->exercise_duration);
        printf("Exercise Intensity:%02x\n", meas->exercise_intensity);
    }
    if ( 0 != (meas->flags & 0x10) ) {
        tmp_value = SFLOAT2float(meas->medication);
        printf("Medication ID:%02d\n", meas->medication_id);
        printf("Medication   :%x %f(%s)\n", (uint16_t)meas->medication, tmp_value, unit[((meas->flags & 0x20)>>5)] );
    }
    if ( 0 != (meas->flags & 0x40) ) {
        tmp_value = SFLOAT2float(meas->HbA1c);
        printf("HbA1c        :%x %f\n", meas->HbA1c, tmp_value);
    }
}
#endif  //#if (PRF_SEL_GLPC)


/******************************************************************************************/
/**
 *  @brief Display GLP RA Control Point Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *cp      GLP RA Control Point Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void GLP_RA_CP_Info_Disp( RBLE_GLP_RA_CONTROL_POINT_INFO *cp )
{
    char *opcode[] = { "", "Report stored records", "Delete stored records", "Abort operation",
                       "Report num of stored records", "Num of stored records rese", "Res Code",  };
    char *racp_operator[] = { "--", "All records", "Less or equal", "Greater or equal", "Within range",
                              "First record", "Last record" };
    
    printf("OpCode:%s (%d)\n", opcode[cp->OpCode],cp->OpCode);
    printf("Operator:%s (%d)\n", racp_operator[cp->racp_operator],cp->racp_operator);
    printf("seq_num min:%d  max:%d\n", cp->min_sequence_num, cp->max_sequence_num);
    printf("min:%04d/%02d/%02d %02d:%02d:%02d  max:%04d/%02d/%02d %02d:%02d:%02d\n",
            cp->min_stamp.year,cp->min_stamp.month,cp->min_stamp.day,cp->min_stamp.hour,cp->min_stamp.min,cp->min_stamp.sec,
            cp->max_stamp.year,cp->max_stamp.month,cp->max_stamp.day,cp->max_stamp.hour,cp->max_stamp.min,cp->max_stamp.sec);
}
/******************************************************************************************/
/**
 *  @brief Display GLP RA Control Point Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *cp      GLP RA Control Point Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void GLP_RA_CP_Ind_Info_Disp( RBLE_GLP_RA_CONTROL_POINT_IND_INFO *cp )
{
    char *opcode[] = { "", "Report stored records", "Delete stored records", "Abort operation",
                       "Report num of stored records", "Num of stored records rese", "Res Code",  };
    char *racp_operator[] = { "--", "All records", "Less or equal", "Greater or equal", "Within range",
                              "First record", "Last record" };
    
    printf("OpCode:%s\n", opcode[cp->OpCode]);
    printf("Operator:%s\n", racp_operator[cp->racp_operator]);
    printf("Num of Records:%d\n", cp->num_of_records);
    printf("Req OpCode:%s\n", opcode[cp->request_op_code]);
    printf("Res Code:%d\n", cp->response_code_value);
}

/******************************************************************************************/
/**
 *  @brief rBLE GLP Event and Status Disp
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
static void GLP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE GLP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}
/******************************************************************************************/
/**
 *  @brief rBLE GLP Event Disp
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
static void GLP_Event_Disp(char_t *event_str)
{
    printf( "\nrBLE GLP EVENT (%s)\n", event_str );
}


#endif  //#if (PRF_SEL_GLPC || PRF_SEL_GLPS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
