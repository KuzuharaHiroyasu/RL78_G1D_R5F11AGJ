/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_hrp.c
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

#if (PRF_SEL_HRPC || PRF_SEL_HRPS)
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
#if (PRF_SEL_HRPS)
static void RBLE_APP_HRPS_CallBack( RBLE_HRPS_EVENT *event );                               /* HRP(Sensor) Call Back */
static BOOL RBLE_HRP_Sensor_Enable_Test( void );                                            /* A HRP_Sensor_Enable command is executed. */
static BOOL RBLE_HRP_Sensor_Disable_Test( void );                                           /* A HRP_Sensor_Disable command is executed. */
static BOOL RBLE_HRP_Sensor_Send_Measurements_Test( void );                                 /* A HRP_Sensor_Send_Measurements command is executed. */
#endif
#if (PRF_SEL_HRPC)
static void RBLE_APP_HRPC_CallBack( RBLE_HRPC_EVENT *event );                               /* HRP(Collector) Call Back */
static BOOL RBLE_HRP_Collector_Enable_Test( void );                                         /* A HRP_Collector_Enable command is executed. */
static BOOL RBLE_HRP_Collector_Disable_Test( void );                                        /* A HRP_Collector_Disable command is executed. */
static BOOL RBLE_HRP_Collector_Read_Char_Test( void );                                      /* A HRP_Collector_Read_Char command is executed. */
static BOOL RBLE_HRP_Collector_Write_Control_Point_Test( void );                            /* A HRP_Collector_Write_Control_Point command is executed. */
static BOOL RBLE_HRP_Collector_Write_Char_Test( void );                                     /* A HRP_Collector_Write_Char command is executed. */
#endif

#if (PRF_SEL_HRPC)
static void HRP_Info_Disp( RBLE_HRP_MEASUREMENTS_INFO *p );
#endif

static void HRP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status);
static void HRP_Event_Disp(char_t *event_str);

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Heart Rate Profile Select Menu */
static const MENU_INFO  Test_Prog_HRP_Menu[] = {
#if (PRF_SEL_HRPS)
    {  1,   RBLE_HRP_Sensor_Enable_Test,                    NULL,       "1.HRP Sensor_Enable\n",                    },
    {  2,   RBLE_HRP_Sensor_Disable_Test,                   NULL,       "2.HRP Sensor_Disable\n",                   },
    {  3,   RBLE_HRP_Sensor_Send_Measurements_Test,         NULL,       "3.HRP Sensor_Send_Measurements\n",         },
#endif
#if (PRF_SEL_HRPC)
    {  4,   RBLE_HRP_Collector_Enable_Test,                 NULL,       "4.HRP Collector_Enable\n",                 },
    {  5,   RBLE_HRP_Collector_Disable_Test,                NULL,       "5.HRP Collector_Disable\n",                },
    {  6,   RBLE_HRP_Collector_Read_Char_Test,              NULL,       "6.HRP Collector_Read_Char\n",              },
    {  7,   RBLE_HRP_Collector_Write_Control_Point_Test,    NULL,       "7.HRP Collector_Write_Control_Point\n",    },
    {  8,   RBLE_HRP_Collector_Write_Char_Test,             NULL,       "8.HRP Collector_Write_Char\n",             },
#endif
    {  ESC_KEY, Menu_Exit,                                  NULL,       "ESC Key: Menu exit\n",                     },
    {  0,   NULL,                                           NULL,       NULL,                                       }
};

const SELECT_MENU Test_Prog_HRP_Select_Menu = {
    "\n-- BLE Sample Program Heart Rate Profile Test Menu --",
    Test_Prog_HRP_Menu
};


#if (PRF_SEL_HRPC)
static RBLE_HRS_CONTENT g_hrs = { 0 };
static RBLE_DIS_CONTENT g_hrp_dis = { 0 };
#endif

/******************************* Program Area *****************************************/
#if (PRF_SEL_HRPS)
/******************************************************************************************/
/**
 *  @brief rBLE HRP(Sensor) Call Back
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
static void RBLE_APP_HRPS_CallBack( RBLE_HRPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_HRP_EVENT_SENSOR_ENABLE_COMP:
            HRP_Event_Status_Disp("SENSOR_ENABLE_COMP", event->param.sensor_enable.status);
            Connection_Handle_Disp( event->param.sensor_enable.conhdl );
            break;
        case RBLE_HRP_EVENT_SENSOR_DISABLE_COMP:
            HRP_Event_Disp("SENSOR_DISABLE_COMP");
            Connection_Handle_Disp( event->param.sensor_disable.conhdl );
            break;
        case RBLE_HRP_EVENT_SENSOR_ERROR_IND:
            HRP_Event_Status_Disp("SENSOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_HRP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP:
            HRP_Event_Status_Disp("SENSOR_SEND_MEASUREMENTS_COMP", event->param.send_measurements.status);
            Connection_Handle_Disp( event->param.send_measurements.conhdl );
            break;
        case RBLE_HRP_EVENT_SENSOR_CHG_CP_IND:
            HRP_Event_Disp("SENSOR_CHG_CP_IND");
            printf( "  CP Value = %s\n", ( event->param.hrps_chg_cp_ind.cp_val == RBLE_HRPC_HRTRATE_CTRL_POINT_RESET ) ? "RESET" : "OTHER" );
            break;
        case RBLE_HRP_EVENT_SENSOR_CFG_NTF_IND:
            HRP_Event_Disp("SENSOR_CFG_NTF_IND");
            printf( "  Cfg Value = %s\n", ( event->param.hrps_cfg_ntf_ind.cfg_val == RBLE_PRF_STOP_NTFIND ) ? "STOP" : "START_NTFD" );
            break;
        case RBLE_HRP_EVENT_SENSOR_COMMAND_DISALLOWED_IND:
            HRP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A HRP_Sensor_Enable command is executed. 
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
static BOOL RBLE_HRP_Sensor_Enable_Test( void )
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

    RBLE_HRP_SENSOR_PARAM    param = {
        RBLE_PRF_STOP_NTFIND      /* Heart Rate measurement notification configuration */
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
        Ret_Status = RBLE_HRP_Sensor_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_HRPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HRP_Sensor_Disable command is executed. 
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
static BOOL RBLE_HRP_Sensor_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HRP_Sensor_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HRP_Sensor_Send_Measurements command is executed. 
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
static BOOL RBLE_HRP_Sensor_Send_Measurements_Test( void )
{
    RBLE_HRP_MEASUREMENTS_INFO   measurements_info = {
        0x18,           /* flags */
        0x01,           /* rr interval num */
        80,             /* heart rate measurements */
        0x0010,         /* energy expended */
        {0x000a}        /* rr interval */
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {

        /* API Call */
        Ret_Status = RBLE_HRP_Sensor_Send_Measurements( Connection_Handle, &measurements_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_HRPS)

#if (PRF_SEL_HRPC)
/******************************************************************************************/
/**
 *  @brief rBLE HRP(Collector) Call Back
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
static void RBLE_APP_HRPC_CallBack( RBLE_HRPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_HRP_EVENT_COLLECTOR_ENABLE_COMP:
            HRP_Event_Status_Disp("COLLECTOR_ENABLE_COMP", event->param.collector_enable.status);
            Connection_Handle_Disp( event->param.collector_enable.conhdl );
            printf("* Heart Rate Service\n");
            printf(" Start Handle = 0x%04X\n", event->param.collector_enable.hrs.shdl );
            printf(" End Handle   = 0x%04X\n\n", event->param.collector_enable.hrs.ehdl );
            printf(" meas_char_hdl = 0x%04X\n", event->param.collector_enable.hrs.hrtrate_meas_char_hdl );
            printf(" meas_val_hdl  = 0x%04X\n", event->param.collector_enable.hrs.hrtrate_meas_val_hdl );
            printf(" meas_cfg_hdl  = 0x%04X\n", event->param.collector_enable.hrs.hrtrate_meas_cfg_hdl );
            printf(" meas_prop     = 0x%02X\n\n", event->param.collector_enable.hrs.hrtrate_meas_prop );
            printf(" body_sensor_loc_char_hdl = 0x%04X\n", event->param.collector_enable.hrs.body_sensor_loc_char_hdl );
            printf(" body_sensor_loc_val_hdl  = 0x%04X\n", event->param.collector_enable.hrs.body_sensor_loc_val_hdl );
            printf(" body_sensor_loc_prop     = 0x%02X\n\n", event->param.collector_enable.hrs.body_sensor_loc_prop );
            printf(" control_point_char_hdl = 0x%04X\n", event->param.collector_enable.hrs.hrtrate_cp_char_hdl );
            printf(" control_point_val_hdl  = 0x%04X\n", event->param.collector_enable.hrs.hrtrate_cp_val_hdl );
            printf(" control_point_prop     = 0x%02X\n\n", event->param.collector_enable.hrs.hrtrate_cp_prop );
            DIS_handle_Info_Disp(&event->param.collector_enable.dis);

            g_hrs = event->param.collector_enable.hrs;
            g_hrp_dis = event->param.collector_enable.dis;
            break;
        case RBLE_HRP_EVENT_COLLECTOR_DISABLE_COMP:
            HRP_Event_Status_Disp("COLLECTOR_DISABLE_COMP", event->param.collector_disable.status);
            Connection_Handle_Disp( event->param.collector_disable.conhdl );
            break;
        case RBLE_HRP_EVENT_COLLECTOR_ERROR_IND:
            HRP_Event_Status_Disp("COLLECTOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_HRP_EVENT_COLLECTOR_MEASUREMENTS_NTF:
            HRP_Event_Disp("COLLECTOR_MEASUREMENTS_NTF");
            HRP_Info_Disp( &event->param.measurements_ntf.measurements_info );
            break;
        case RBLE_HRP_EVENT_COLLECTOR_READ_CHAR_RESPONSE:
            HRP_Event_Status_Disp("COLLECTOR_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            event->param.rd_char_resp.data.data[event->param.rd_char_resp.data.len] = '\0';
            printf("(%s)\n", event->param.rd_char_resp.data.data);
            break;
        case RBLE_HRP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE:
            HRP_Event_Status_Disp("COLLECTOR_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_HRP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND:
            HRP_Event_Status_Disp("COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A HRP_Collector_Enable command is executed. 
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
static BOOL RBLE_HRP_Collector_Enable_Test( void )
{
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_hrs.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_HRP_Collector_Enable( Connection_Handle, con_type, &g_hrs, &g_hrp_dis, RBLE_APP_HRPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HRP_Collector_Disable command is executed. 
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
static BOOL RBLE_HRP_Collector_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HRP_Collector_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HRP_Collector_Read_Char command is executed. 
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
static BOOL RBLE_HRP_Collector_Read_Char_Test( void )
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
    if ( ( ILLEGAL_NO == Parameter_No ) || ( RBLE_HRPC_RD_DIS_IEEE < Parameter_No ) ) {
        char_code = RBLE_HRPC_RD_HRS_HM_CFG;
    } else {
        char_code = (uint8_t)Parameter_No;
    }

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HRP_Collector_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A RBLE_HRP_Collector_Write_Control_Point_Test command is executed. 
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
static BOOL RBLE_HRP_Collector_Write_Control_Point_Test( void )
{
    RBLE_STATUS                 Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HRP_Collector_Write_Control_Point( Connection_Handle, RBLE_HRPC_HRTRATE_CTRL_POINT_RESET );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HRP_Collector_Write_Char command is executed. 
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
static BOOL RBLE_HRP_Collector_Write_Char_Test( void )
{
    RBLE_STATUS                 Ret_Status;
    int32_t                     Parameter;

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
            printf( "Start Ntf(1)\n" );
        } else {
            Parameter = RBLE_PRF_STOP_NTFIND;
            printf( "Stop Ntf(0)\n" );
        }
        /* API Call */
        Ret_Status = RBLE_HRP_Collector_Write_Char( Connection_Handle, (uint16_t)Parameter );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief Display HRP  Measurements Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      HRP Measurements Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void HRP_Info_Disp( RBLE_HRP_MEASUREMENTS_INFO *p )
{
    uint8_t i;
    
    /* mesurement is 16bit? */
    if ( 0 != (p->flags & 0x01) ) {
        printf("Measure:%04x(%d)\n", p->heart_rate_measure, p->heart_rate_measure);
    }
    else {
        printf("Measure:%02x(%d)\n", p->heart_rate_measure, p->heart_rate_measure);
    }

    /* Energy Exended Present ? */
    if ( 0 != (p->flags & 0x08) ) {
        printf("Energy:%04x(%d)\n", p->energy_expended, p->energy_expended);
    }

    /* interval value Present ? */
    if ( 0 != (p->flags & 0x10) ) {
        for(i=0; i < p->rr_interval_num;i++) {
            printf("RR Interval%02d:%04x(%d)\n", i, p->rr_interval[i], p->rr_interval[i]);
        }
    }
}
#endif  //#if (PRF_SEL_HRPS)

/******************************************************************************************/
/**
 *  @brief rBLE HRP Event Status Disp
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
static void HRP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE HRP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}
/******************************************************************************************/
/**
 *  @brief rBLE HRP Event Disp
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
static void HRP_Event_Disp(char_t *event_str)
{
    printf( "\nrBLE HRP EVENT (%s)\n", event_str);
}


#endif  //#if (PRF_SEL_HRPC || PRF_SEL_HRPS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
