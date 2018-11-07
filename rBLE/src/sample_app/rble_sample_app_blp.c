/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_blp.c
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

#if (PRF_SEL_BLPC || PRF_SEL_BLPS)
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
#if (PRF_SEL_BLPS)
static void RBLE_APP_BLPS_CallBack( RBLE_BLPS_EVENT *event );                               /* BLP(Sensor) Call Back */
static BOOL RBLE_BLP_Sensor_Enable_Test( void );                                            /* A BLP_Sensor_Enable command is executed. */
static BOOL RBLE_BLP_Sensor_Disable_Test( void );                                           /* A BLP_Sensor_Disable command is executed. */
static BOOL RBLE_BLP_Sensor_Send_Measurements_Test( void );                                 /* A BLP_Sensor_Send_Measurements command is executed. */
#endif

#if (PRF_SEL_BLPC)
static void RBLE_APP_BLPC_CallBack( RBLE_BLPC_EVENT *event );                               /* BLP(Collector) Call Back */
static BOOL RBLE_BLP_Collector_Enable_Test( void );                                         /* A BLP_Collector_Enable command is executed. */
static BOOL RBLE_BLP_Collector_Disable_Test( void );                                        /* A BLP_Collector_Disable command is executed. */
static BOOL RBLE_BLP_Collector_Read_Char_Test( void );                                      /* A BLP_Collector_Read_Char command is executed. */
static BOOL RBLE_BLP_Collector_Write_Char_Test( void );                                     /* A BLP_Collector_Write_Char command is executed. */
#endif

#if (PRF_SEL_BLPC)
static void BLP_Info_Disp( RBLE_BLP_MEASUREMENTS_INFO *p );
#endif

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Blood Pressure Profile Select Menu */
static const MENU_INFO  Test_Prog_BLP_Menu[] = {
#if (PRF_SEL_BLPS)
    {  1,   RBLE_BLP_Sensor_Enable_Test,                NULL,       "1.BLP Sensor_Enable\n",                },
    {  2,   RBLE_BLP_Sensor_Disable_Test,               NULL,       "2.BLP Sensor_Disable\n",               },
    {  3,   RBLE_BLP_Sensor_Send_Measurements_Test,     NULL,       "3.BLP Sensor_Send_Measurements\n",     },
#endif
#if (PRF_SEL_BLPC)
    {  4,   RBLE_BLP_Collector_Enable_Test,             NULL,       "4.BLP Collector_Enable\n",             },
    {  5,   RBLE_BLP_Collector_Disable_Test,            NULL,       "5.BLP Collector_Disable\n",            },
    {  6,   RBLE_BLP_Collector_Read_Char_Test,          NULL,       "6.BLP Collector_Read_Char\n",          },
    {  7,   RBLE_BLP_Collector_Write_Char_Test,         NULL,       "7.BLP Collector_Write_Char\n",         },
#endif
    {  ESC_KEY, Menu_Exit,                              NULL,       "ESC Key: Menu exit\n",                 },
    {  0,   NULL,                                       NULL,       NULL,                                   }
};

const SELECT_MENU Test_Prog_BLP_Select_Menu = {
    "\n-- BLE Sample Program Blood Pressure Profile Test Menu --",
    Test_Prog_BLP_Menu
};


#if (PRF_SEL_BLPC)
static RBLE_BLS_CONTENT g_bls = { 0 };
static RBLE_DIS_CONTENT g_blp_dis = { 0 };
#endif

/******************************* Program Area *****************************************/

#if (PRF_SEL_BLPS)
/******************************************************************************************/
/**
 *  @brief rBLE BLP(Sensor) Call Back
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
static void RBLE_APP_BLPS_CallBack( RBLE_BLPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_BLP_EVENT_SENSOR_ENABLE_COMP:
            printf( "\nrBLE BLP EVENT (SENSOR_ENABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.sensor_enable.status ) );
            Connection_Handle_Disp( event->param.sensor_enable.conhdl );
            break;
        case RBLE_BLP_EVENT_SENSOR_DISABLE_COMP:
            printf( "\nrBLE BLP EVENT (SENSOR_DISABLE_COMP)\n" );
            Connection_Handle_Disp( event->param.sensor_disable.conhdl );
            break;
        case RBLE_BLP_EVENT_SENSOR_ERROR_IND:
            printf( "\nrBLE BLP EVENT (SENSOR_ERROR_IND) Status(%s)\n", Get_Status_Str( event->param.error_ind.status ) );
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_BLP_EVENT_SENSOR_SEND_MEASUREMENTS_COMP:
            printf( "\nrBLE BLP EVENT (SENSOR_SEND_MEASUREMENTS_COMP) Status(%s)\n", Get_Status_Str( event->param.send_measurements.status ) );
            Connection_Handle_Disp( event->param.send_measurements.conhdl );
            break;
        case RBLE_BLP_EVENT_SENSOR_CFG_INDNTF_IND:
            printf( "\nrBLE BLP EVENT (SENSOR_CFG_INDNTF_IND)\n" );
            printf( "  Char Code = %s\n", ( event->param.blps_cfg_indntf_ind.char_code == RBLE_BLPC_BLDPRS_MEAS_CODE ) ? "BLDPRS_MEAS" :"INTERM_CUFPRS" );
            printf( "  Cfg Value = %s\n", ( event->param.blps_cfg_indntf_ind.cfg_val == RBLE_PRF_STOP_NTFIND ) ? "STOP" : "START_NTF_IND" );
            break;
        case RBLE_BLP_EVENT_SENSOR_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE BLP EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
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
static BOOL RBLE_BLP_Sensor_Enable_Test( void )
{
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_UNAUTH|RBLE_SVC_SEC_ENC,
    };
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    RBLE_BLP_SENSOR_PARAM    param = {
        RBLE_PRF_STOP_NTFIND,     /* Blood pressure measurement indication configuration */
        RBLE_PRF_STOP_NTFIND,     /* Intermediate cuff pressure notification configuration */
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
        Ret_Status = RBLE_BLP_Sensor_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_BLPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A BLP_Sensor_Disable command is executed. 
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
static BOOL RBLE_BLP_Sensor_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_BLP_Sensor_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A BLP_Sensor_Send_Measurements command is executed. 
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
static BOOL RBLE_BLP_Sensor_Send_Measurements_Test( void )
{
    static RBLE_BLP_MEASUREMENTS_INFO   measurements_info = {
        0x01,           /* Stable or intermediary type of measurements */
        0x1e,           /* flags */
        0x007b,         /* blood pressure value - systolic */
        0x0055,         /* blood pressure value - diastolic */
        0x0067,         /* blood pressure value - MAP */
        {               /* time stamp */
            /* 2012/09/01 12:34:56 */
            0x07dc,       /* Year value */
            0x09,       /* Month value */
            0x01,       /* Day value */
            0x0c,       /* Hour value */
            0x22,       /* Minute value */
            0x38,       /* Second value */
            0x00,       /* Reserved */
        },
        0xF321,         /* pulse rate 80.1 */
        0x01,           /* user ID */
        0x00,           /* Reserved */
        0x0008,         /* measurement status */
    };


    RBLE_STATUS Ret_Status;
    int32_t             Parameter_No;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = ILLEGAL_NO;
        }

        if ( 1 != Parameter_No ) {
            measurements_info.flag_stable_meas = 0;
            printf( "Select Parameter -> Measurement is in progress\n" );
        } else {
            measurements_info.flag_stable_meas = 1;
            printf( "Select Parameter -> Stable\n" );
        }
        /* API Call */
        Ret_Status = RBLE_BLP_Sensor_Send_Measurements( Connection_Handle, &measurements_info );

        RBLE_Status_Disp( Ret_Status );
        if ( RBLE_OK == Ret_Status ) {
            measurements_info.rate++;
        }
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_BLPS)

#if (PRF_SEL_BLPC)
/******************************************************************************************/
/**
 *  @brief rBLE BLP(Collector) Call Back
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
static void RBLE_APP_BLPC_CallBack( RBLE_BLPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_BLP_EVENT_COLLECTOR_ENABLE_COMP:
            printf( "\nrBLE BLP EVENT (COLLECTOR_ENABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.collector_enable.status ) );
            Connection_Handle_Disp( event->param.collector_enable.conhdl );
            printf( "* Blood Pressure Service\n");
            printf( " Start Handle = 0x%04X\n", event->param.collector_enable.bls.shdl );
            printf( " End Handle   = 0x%04X\n\n", event->param.collector_enable.bls.ehdl );
            printf( " meas_char_hdl = 0x%04X\n", event->param.collector_enable.bls.bldprs_meas_char_hdl );
            printf( " meas_val_hdl  = 0x%04X\n", event->param.collector_enable.bls.bldprs_meas_val_hdl );
            printf( " meas_cfg_hdl  = 0x%04X\n", event->param.collector_enable.bls.bldprs_meas_cfg_hdl );
            printf( " meas_prop     = 0x%02X\n\n", event->param.collector_enable.bls.bldprs_meas_prop );
            printf( " cufprs_char_hdl = 0x%04X\n", event->param.collector_enable.bls.interm_cufprs_char_hdl );
            printf( " cufprs_val_hdl  = 0x%04X\n", event->param.collector_enable.bls.interm_cufprs_val_hdl );
            printf( " cufprs_cfg_hdl  = 0x%04X\n", event->param.collector_enable.bls.interm_cufprs_cfg_hdl );
            printf( " cufprs_prop     = 0x%02X\n\n", event->param.collector_enable.bls.interm_cufprs_prop );
            printf( " feat_char_hdl = 0x%04X\n", event->param.collector_enable.bls.bldprs_feat_char_hdl );
            printf( " feat_val_hdl  = 0x%04X\n", event->param.collector_enable.bls.bldprs_feat_val_hdl );
            printf( " feat_prop     = 0x%02X\n\n", event->param.collector_enable.bls.bldprs_feat_prop );
            DIS_handle_Info_Disp(&event->param.collector_enable.dis);

            g_bls = event->param.collector_enable.bls;
            g_blp_dis = event->param.collector_enable.dis;
            break;
        case RBLE_BLP_EVENT_COLLECTOR_DISABLE_COMP:
            printf( "\nrBLE BLP EVENT (COLLECTOR_DISABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.collector_disable.status ) );
            Connection_Handle_Disp( event->param.collector_disable.conhdl );
            break;
        case RBLE_BLP_EVENT_COLLECTOR_ERROR_IND:
            printf( "\nrBLE BLP EVENT (COLLECTOR_ERROR_IND) Status(%s)\n", Get_Status_Str( event->param.error_ind.status ) );
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_BLP_EVENT_COLLECTOR_MEASUREMENTS_IND:
            printf( "\nrBLE BLP EVENT (COLLECTOR_MEASUREMENTS_IND)\n" );
            Connection_Handle_Disp( event->param.measurements_ind.conhdl );
            BLP_Info_Disp( &event->param.measurements_ind.measurements_info );
            break;
        case RBLE_BLP_EVENT_COLLECTOR_READ_CHAR_RESPONSE:
            printf( "\nrBLE BLP EVENT (COLLECTOR_READ_CHAR_RESPONSE) Status(%s)\n", Get_Status_Str( event->param.rd_char_resp.att_code ) );
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            break;
        case RBLE_BLP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE:
            printf( "\nrBLE BLP EVENT (COLLECTOR_WRITE_CHAR_RESPONSE) Status(%s)\n", Get_Status_Str( event->param.wr_char_resp.att_code ) );
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_BLP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE BLP EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A BLP_Collector_Enable command is executed. 
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
static BOOL RBLE_BLP_Collector_Enable_Test( void )
{
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_bls.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }

        /* API Call */
        Ret_Status = RBLE_BLP_Collector_Enable( Connection_Handle, con_type, &g_bls, &g_blp_dis, RBLE_APP_BLPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A BLP_Collector_Disable command is executed. 
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
static BOOL RBLE_BLP_Collector_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_BLP_Collector_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A BLP_Collector_Read_Char command is executed. 
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
static BOOL RBLE_BLP_Collector_Read_Char_Test( void )
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
    if ( ( ILLEGAL_NO == Parameter_No ) || ( RBLE_BLPC_RD_DIS_IEEE < Parameter_No ) ) {
        char_code = RBLE_BLPC_RD_BLS_BM_CFG;
    } else {
        char_code = (uint8_t)Parameter_No;
    }

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_BLP_Collector_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A BLP_Collector_Write_Char command is executed. 
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
static BOOL RBLE_BLP_Collector_Write_Char_Test( void )
{
    RBLE_STATUS     Ret_Status;
    int32_t         Parameter_No;
    uint8_t         char_code;
    uint16_t        cfg_val;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = 0;
        }
        /***
         * Args
         * 0: Start Indication   RBLE_BLPC_BLDPRS_MEAS_CODE,   RBLE_PRF_START_IND
         * 1: Stop Indication    RBLE_BLPC_BLDPRS_MEAS_CODE,   RBLE_PRF_STOP_NTFIND
         * 2: Start Notification RBLE_BLPC_INTERM_CUFPRS_CODE, RBLE_PRF_START_NTF
         * 3: Stop Notification  RBLE_BLPC_INTERM_CUFPRS_CODE, RBLE_PRF_STOP_NTFIND
         */

        switch( Parameter_No ){
        case 1:
            char_code = RBLE_BLPC_BLDPRS_MEAS_CODE;
            cfg_val = RBLE_PRF_STOP_NTFIND;
            break;
        case 2:
            char_code = RBLE_BLPC_INTERM_CUFPRS_CODE;
            cfg_val = RBLE_PRF_START_NTF;
            break;
        case 3:
            char_code = RBLE_BLPC_INTERM_CUFPRS_CODE;
            cfg_val = RBLE_PRF_STOP_NTFIND;
            break;
        case 0:
        default:
            char_code = RBLE_BLPC_BLDPRS_MEAS_CODE;
            cfg_val = RBLE_PRF_START_IND;
            break;
        }

        /* API Call */
        Ret_Status = RBLE_BLP_Collector_Write_Char( Connection_Handle, char_code, cfg_val );
        printf( " char_code = %d, cfg_val = 0x%04X\n", char_code, cfg_val );
        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief Display BLP  Measurements Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      BLP Measurements Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void BLP_Info_Disp( RBLE_BLP_MEASUREMENTS_INFO *p )
{
    const char *unit[] = { "mmHg", "kPa" };
    float p1, p2, p3;

    if ( 0 != p->flag_stable_meas ) {
        p1 = SFLOAT2float( p->press_val_field1 );
        p2 = SFLOAT2float( p->press_val_field2 );
        p3 = SFLOAT2float( p->press_val_field3 );
        printf("S:%3.1f, D:%3.1f, M:%3.1f (%s)\n",p1,p2,p3, unit[(p->flags & 0x01)]);
    } else {
        p1 = SFLOAT2float( p->press_val_field1 );
        printf("  Cuff      : %3.1f (%s)\n", p1, unit[(p->flags & 0x01)]);
    }
    
    /* Timestamp Present ? */
    if ( 0 != (p->flags & 0x02) ) {
        printf("TS: %d/%02d/%02d %02d:%02d:%02d\n",  p->stamp.year,
                                                     p->stamp.month,
                                                     p->stamp.day,
                                                     p->stamp.hour,
                                                     p->stamp.min,
                                                     p->stamp.sec);
    }

    /* Pulse Rate Present ? */
    if ( 0 != (p->flags & 0x04) ) {
        p1 = SFLOAT2float( p->rate );
        printf("Rate:%3.1f\n", p1);
    }

    /* User ID Present ? */
    if ( 0 != (p->flags & 0x08) ) {
        printf("UserID:%d\n", p->id);
    }

    /* Measurement Status Present ? */
    if ( 0 != (p->flags & 0x10) ) {
        printf("M.Sts:%04X\n", p->meas_sts);
    }
}
#endif  //#if (PRF_SEL_BLPC)


#endif  //#if (PRF_SEL_BLPC || PRF_SEL_BLPS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
