/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_htp.c
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

#if (PRF_SEL_HTPC || PRF_SEL_HTPT)
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
#if (PRF_SEL_HTPT)
static void RBLE_APP_HTPT_CallBack( RBLE_HTPT_EVENT *event );                               /* HTP(Thermometer) Call Back */
static BOOL RBLE_HTP_Thermometer_Enable_Test( void );                                       /* A HTP_Thermometer_Enable command is executed. */
static BOOL RBLE_HTP_Thermometer_Disable_Test( void );                                      /* A HTP_Thermometer_Disable command is executed. */
static BOOL RBLE_HTP_Thermometer_Send_Temp_Test( void );                                    /* A HTP_Thermometer_Send_Temp command is executed. */
static BOOL RBLE_HTP_Thermometer_Req_Measurement_Period_Ind_Test( void );                   /* A HTP_Thermometer_Req_Measurement_Period_Ind command is executed. */
#endif

#if (PRF_SEL_HTPC)
static void RBLE_APP_HTPC_CallBack( RBLE_HTPC_EVENT *event );                               /* HTP(Collector) Call Back */
static BOOL RBLE_HTP_Collector_Enable_Test( void );                                         /* A HTP_Collector_Enable command is executed. */
static BOOL RBLE_HTP_Collector_Disable_Test( void );                                        /* A HTP_Collector_Disable command is executed. */
static BOOL RBLE_HTP_Collector_Read_Char_Test( void );                                      /* A HTP_Collector_Read_Char command is executed. */
static BOOL RBLE_HTP_Collector_Write_Char_Test( void );                                     /* A HTP_Collector_Write_Char command is executed. */
static BOOL RBLE_HTP_Collector_Set_Measurement_Period_Test( void );                         /* A HTP_Collector_Set_Measurement_Period command is executed. */
#endif

#if (PRF_SEL_HTPC)
static void HTP_Info_Disp( RBLE_HTP_TEMP_INFO *p );
#endif

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Health Thermometer Profile Select Menu */
static const MENU_INFO  Test_Prog_HTP_Menu[] = {
#if (PRF_SEL_HTPT)
    {  1,   RBLE_HTP_Thermometer_Enable_Test,                       NULL,       "1.HTP Thermometer_Enable\n",                       },
    {  2,   RBLE_HTP_Thermometer_Disable_Test,                      NULL,       "2.HTP Thermometer_Disable\n",                      },
    {  3,   RBLE_HTP_Thermometer_Send_Temp_Test,                    NULL,       "3.HTP Thermometer_Send_Temp\n",                    },
    {  4,   RBLE_HTP_Thermometer_Req_Measurement_Period_Ind_Test,   NULL,       "4.HTP Thermometer_Req_Measurement_Period_Ind\n",   },
#endif
#if (PRF_SEL_HTPC)
    {  5,   RBLE_HTP_Collector_Enable_Test,                         NULL,       "5.HTP Collector_Enable\n",                         },
    {  6,   RBLE_HTP_Collector_Disable_Test,                        NULL,       "6.HTP Collector_Disable\n",                        },
    {  7,   RBLE_HTP_Collector_Read_Char_Test,                      NULL,       "7.HTP_Collector_Read_Char\n",                      },
    {  8,   RBLE_HTP_Collector_Write_Char_Test,                     NULL,       "8.HTP_Collector_Write_Char\n",                     },
    {  9,   RBLE_HTP_Collector_Set_Measurement_Period_Test,         NULL,       "9.HTP Collector_Set_Measurement_Period\n",         },
#endif
    {  ESC_KEY, Menu_Exit,                                          NULL,       "ESC Key: Menu exit\n",                             },
    {  0,   NULL,                                                   NULL,       NULL,                                               }
};

const SELECT_MENU Test_Prog_HTP_Select_Menu = {
    "\n-- BLE Sample Program Health Thermometer Profile Test Menu --",
    Test_Prog_HTP_Menu
};


#if (PRF_SEL_HTPC)
static RBLE_HTS_CONTENT g_hts = { 0 };
static RBLE_DIS_CONTENT g_htp_dis = { 0 };
#endif

/******************************* Program Area *****************************************/

#if (PRF_SEL_HTPT)
/******************************************************************************************/
/**
 *  @brief rBLE HTP(Thermometer) Call Back
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
static void RBLE_APP_HTPT_CallBack( RBLE_HTPT_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_HTP_EVENT_THERMOMETER_ENABLE_COMP:
            printf( "\nrBLE HTP EVENT (THERMOMETER_ENABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.thermometer_enable.status ) );
            Connection_Handle_Disp( event->param.thermometer_enable.conhdl );
            break;
        case RBLE_HTP_EVENT_THERMOMETER_DISABLE_COMP:
            printf( "\nrBLE HTP EVENT (THERMOMETER_DISABLE_COMP)\n" );
            Connection_Handle_Disp( event->param.thermometer_disable.conhdl );
            break;
        case RBLE_HTP_EVENT_THERMOMETER_ERROR_IND:
            printf( "\nrBLE HTP EVENT (THERMOMETER_ERROR_IND) Status(%s)\n", Get_Status_Str( event->param.error_ind.status ) );
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_HTP_EVENT_THERMOMETER_SEND_TEMP_COMP:
            printf( "\nrBLE HTP EVENT (THERMOMETER_SEND_TEMP_COMP) Status(%s)\n", Get_Status_Str( event->param.send_temp.status ) );
            Connection_Handle_Disp( event->param.send_temp.conhdl );
            break;
        case RBLE_HTP_EVENT_THERMOMETER_REQ_MEASUREMENT_PERIOD_IND_COMP:
            printf( "\nrBLE HTP EVENT (THERMOMETER_REQ_MEASUREMENT_PERIOD_IND_COMP) Status(%s)\n", Get_Status_Str( event->param.send_meas_period.status ) );
            Connection_Handle_Disp( event->param.send_meas_period.conhdl );
            break;
        case RBLE_HTP_EVENT_THERMOMETER_MEAS_INTV_CHG_IND:
            printf( "\nrBLE HTP EVENT (THERMOMETER_MEAS_INTV_CHG_IND)\n" );
            Connection_Handle_Disp( event->param.meas_intv_chg_ind.conhdl );
            printf( "intv = %d\n", event->param.meas_intv_chg_ind.intv );
            break;
        case RBLE_HTP_EVENT_THERMOMETER_CFG_INDNTF_IND:
            printf( "\nrBLE HTP EVENT (THERMOMETER_CFG_INDNTF_IND)\n" );
            Connection_Handle_Disp( event->param.htpt_cfg_indntf_ind.conhdl );
            printf( "Char code = %d\n", event->param.htpt_cfg_indntf_ind.char_code );
            printf( "Cfg val   = %d\n", event->param.htpt_cfg_indntf_ind.cfg_val );
            break;
        case RBLE_HTP_EVENT_THERMOMETER_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE HTP EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A HTP_Thermometer_Enable command is executed. 
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
static BOOL RBLE_HTP_Thermometer_Enable_Test( void )
{
    RBLE_STATUS             Ret_Status;
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_UNAUTH|RBLE_SVC_SEC_ENC,
    };
    uint8_t     select = 0;
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    RBLE_HTP_THERM_PARAM    param = {
        RBLE_PRF_STOP_NTFIND,           /* Temperature measurement indication configuration */
        RBLE_PRF_STOP_NTFIND,           /* Intermediate temperature notification configuration */
        RBLE_PRF_STOP_NTFIND,           /* Measurement interval indication configuration */
        0x0002                          /* Measurement interval */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ((0 != RBLE_Parameter_Cnt) && (2 >= RBLE_Parameter_Arg[ 0 ]))
        {
            select = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        }

        /* API Call */
        Ret_Status = RBLE_HTP_Thermometer_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_HTPT_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HTP_Thermometer_Disable command is executed. 
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
static BOOL RBLE_HTP_Thermometer_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HTP_Thermometer_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HTP_Thermometer_Send_Temp command is executed. 
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
static BOOL RBLE_HTP_Thermometer_Send_Temp_Test( void )
{
    int32_t                         Parameter_No;
    static RBLE_HTP_TEMP_INFO temp_info[] =
    {
        {   /* 0:TC_CI_BV_01_C */
            0x01,           /* Stable or intermediary type of temperature */
            0x06,           /* flags */
            0x00000001,     /* temp value */
            {               /* time stamp */
                0x1234,     /* Year value */
                0x56,       /* Month value */
                0x78,       /* Day value */
                0x9A,       /* Hour value */
                0xBC,       /* Minute value */
                0xDE,       /* Second value */
                0xF0,       /* Reserved */
            },
            0x02,           /* type */
            0x00,           /* Reserved */
        },
        {   /* 1:TC_SP_BV_03_C */
            0x00,           /* Stable or intermediary type of temperature */
            0x06,           /* flags */
            0x00000001,     /* temp value */
            {               /* time stamp */
                0x1234,     /* Year value */
                0x56,       /* Month value */
                0x78,       /* Day value */
                0x9A,       /* Hour value */
                0xBC,       /* Minute value */
                0xDE,       /* Second value */
                0xF0,       /* Reserved */
            },
            0x02,           /* type */
            0x00,           /* Reserved */
        }
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = ILLEGAL_NO;
        }
        if ( ( ILLEGAL_NO == Parameter_No )
          || ( ( sizeof( temp_info ) / sizeof( RBLE_HTP_TEMP_INFO ) ) <= Parameter_No ) ) {
            Select_Param_No_Disp( 0 );
            /* API Call */
            Ret_Status = RBLE_HTP_Thermometer_Send_Temp( Connection_Handle, &temp_info[0] );
            printf( "Value = %d\n", temp_info[0].temp_val );
        } else {
            Select_Param_No_Disp( Parameter_No );
            /* API Call */
            Ret_Status = RBLE_HTP_Thermometer_Send_Temp( Connection_Handle, &temp_info[Parameter_No] );
            printf( "Value = %d\n", temp_info[Parameter_No].temp_val );
        }

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HTP_Thermometer_Req_Measurement_Period_Ind command is executed. 
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
static BOOL RBLE_HTP_Thermometer_Req_Measurement_Period_Ind_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HTP_Thermometer_Req_Measurement_Period_Ind( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_HTPT)

#if (PRF_SEL_HTPC)
/******************************************************************************************/
/**
 *  @brief rBLE HTP(Collector) Call Back
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
static void RBLE_APP_HTPC_CallBack( RBLE_HTPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_HTP_EVENT_COLLECTOR_ENABLE_COMP:
            printf( "\nrBLE HTP EVENT (COLLECTOR_ENABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.collector_enable.status ) );
            Connection_Handle_Disp( event->param.collector_enable.conhdl );
            printf( "* Health Tehrmometer Service\n");
            printf( " Start Handle    = 0x%04X\n",   event->param.collector_enable.hts.shdl );
            printf( " End Handle      = 0x%04X\n\n", event->param.collector_enable.hts.ehdl );
            printf( " temp_meas_char_hdl = 0x%04X\n",   event->param.collector_enable.hts.temp_meas_char_hdl );
            printf( " temp_meas_val_hdl  = 0x%04X\n",   event->param.collector_enable.hts.temp_meas_val_hdl  );
            printf( " temp_meas_cfg_hdl  = 0x%04X\n",   event->param.collector_enable.hts.temp_meas_cfg_hdl  );
            printf( " temp_meas_prop     = 0x%02X\n\n", event->param.collector_enable.hts.temp_meas_prop     );
            printf( " temp_type_char_hdl = 0x%04X\n",   event->param.collector_enable.hts.temp_type_char_hdl );
            printf( " temp_type_val_hdl  = 0x%04X\n",   event->param.collector_enable.hts.temp_type_val_hdl  );
            printf( " temp_type_prop     = 0x%02X\n\n", event->param.collector_enable.hts.temp_type_prop     );
            printf( " interm_temp_char_hdl = 0x%04X\n",   event->param.collector_enable.hts.interm_temp_char_hdl );
            printf( " interm_temp_val_hdl  = 0x%04X\n",   event->param.collector_enable.hts.interm_temp_val_hdl  );
            printf( " interm_temp_cfg_hdl  = 0x%04X\n",   event->param.collector_enable.hts.interm_temp_cfg_hdl  );
            printf( " interm_temp_prop     = 0x%02X\n\n", event->param.collector_enable.hts.interm_temp_prop     );
            printf( " meas_intv_char_hdl = 0x%04X\n",   event->param.collector_enable.hts.meas_intv_char_hdl );
            printf( " meas_intv_val_hdl  = 0x%04X\n",   event->param.collector_enable.hts.meas_intv_val_hdl  );
            printf( " meas_intv_cfg_hdl  = 0x%04X\n",   event->param.collector_enable.hts.meas_intv_cfg_hdl  );
            printf( " valid_range_hdl    = 0x%04X\n",   event->param.collector_enable.hts.valid_range_hdl    );
            printf( " meas_intv_prop     = 0x%02X\n\n", event->param.collector_enable.hts.meas_intv_prop     );
            DIS_handle_Info_Disp(&event->param.collector_enable.dis);

            g_hts = event->param.collector_enable.hts;
            g_htp_dis = event->param.collector_enable.dis;
            break;
        case RBLE_HTP_EVENT_COLLECTOR_DISABLE_COMP:
            printf( "\nrBLE HTP EVENT (COLLECTOR_DISABLE_COMP) Status(%s)\n", Get_Status_Str( event->param.collector_disable.status ) );
            Connection_Handle_Disp( event->param.collector_disable.conhdl );
            break;
        case RBLE_HTP_EVENT_COLLECTOR_ERROR_IND:
            printf( "\nrBLE HTP EVENT (COLLECTOR_ERROR_IND) Status(%s)\n", Get_Status_Str( event->param.error_ind.status ) );
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_HTP_EVENT_COLLECTOR_TEMP_IND:
            printf( "\nrBLE HTP EVENT (COLLECTOR_TEMP_IND)\n" );
            Connection_Handle_Disp( event->param.temp_ind.conhdl );
            HTP_Info_Disp( &event->param.temp_ind.temp_info );
            break;
        case RBLE_HTP_EVENT_COLLECTOR_MEAS_INTV_IND:
            printf( "\nrBLE HTP EVENT (COLLECTOR_MEAS_INTV_IND)\n" );
            Connection_Handle_Disp( event->param.meas_intv_ind.conhdl );
            printf( "intv = %d\n", event->param.meas_intv_ind.intv );
            break;
        case RBLE_HTP_EVENT_COLLECTOR_READ_CHAR_RESPONSE:
            printf( "\nrBLE HTP EVENT (COLLECTOR_READ_CHAR_RESPONSE) Status(%s)\n", Get_Status_Str( event->param.rd_char_resp.att_code ) );
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            break;
        case RBLE_HTP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE:
            printf( "\nrBLE HTP EVENT (COLLECTOR_WRITE_CHAR_RESPONSE) Status(%s)\n", Get_Status_Str( event->param.wr_char_resp.att_code ) );
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_HTP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE HTP EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}
/******************************************************************************************/
/**
 *  @brief A HTP_Collector_Enable command is executed. 
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
static BOOL RBLE_HTP_Collector_Enable_Test( void )
{
    /* Connection_Type */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;


    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if (0x0000 != g_hts.shdl) {
            con_type = RBLE_PRF_CON_NORMAL;
        }

        /* API Call */
        Ret_Status = RBLE_HTP_Collector_Enable( Connection_Handle, con_type, &g_hts, &g_htp_dis, RBLE_APP_HTPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HTP_Collector_Disable command is executed. 
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
static BOOL RBLE_HTP_Collector_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HTP_Collector_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HTP_Collector_Read_Char command is executed. 
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
static BOOL RBLE_HTP_Collector_Read_Char_Test( void )
{
    uint8_t         char_code;
    RBLE_STATUS     Ret_Status;
    int32_t         Parameter_No;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = ILLEGAL_NO;
    }
    if ( ( ILLEGAL_NO == Parameter_No ) || ( RBLE_HTPC_RD_DIS_IEEE < Parameter_No ) ) {
        char_code = RBLE_HTPC_RD_HTS_TM_CFG;
        Select_Param_No_Disp( 0 );
    } else {
        char_code = (uint8_t)Parameter_No;
        Select_Param_No_Disp( Parameter_No );
    }

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HTP_Collector_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HTP_Collector_Write_Char command is executed. 
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
static BOOL RBLE_HTP_Collector_Write_Char_Test( void )
{
    int32_t                         Parameter_No;
    /* CharCode */
    uint8_t char_code = RBLE_HTPC_TEMP_MEAS_CODE;       /*=1*/
//  uint8_t char_code = RBLE_HTPC_INTERM_TEMP_CODE;     /*=2*/
//  uint8_t char_code = RBLE_HTPC_MEAS_INTV_CODE;       /*=3*/
    /* CfgVal */
//  uint16_t cfg_val = RBLE_PRF_STOP_NTFIND;            /*=0*/
//  uint16_t cfg_val = RBLE_PRF_START_NTF;              /*=1*/
    uint16_t cfg_val = RBLE_PRF_START_IND;              /*=2*/

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
        } else {
            Parameter_No = 0;
        }
        switch( Parameter_No ){
        case 0:
        default:
            char_code = RBLE_HTPC_TEMP_MEAS_CODE;       /*=1*/
            cfg_val = RBLE_PRF_START_IND;               /*=2*/
            break;
        case 1:
            char_code = RBLE_HTPC_INTERM_TEMP_CODE;     /*=2*/
            cfg_val = RBLE_PRF_START_NTF;               /*=1*/
            break;
        case 2:
            char_code = RBLE_HTPC_MEAS_INTV_CODE;       /*=3*/
            cfg_val = RBLE_PRF_START_IND;               /*=1*/
            break;
        }
        Select_Param_No_Disp( Parameter_No );
        
        /* API Call */
        Ret_Status = RBLE_HTP_Collector_Write_Char( Connection_Handle, char_code, cfg_val );
        printf( " char_code = %d, cfg_val = 0x%04X\n", char_code, cfg_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HTP_Collector_Set_Measurement_Period command is executed. 
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
static BOOL RBLE_HTP_Collector_Set_Measurement_Period_Test( void )
{
    const uint16_t intv = 0x0003;           /* Interval Range:0x003 - 0x0010 */

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HTP_Collector_Set_Measurement_Period( Connection_Handle, intv );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief Display HTP TEMP Info
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      HTP TEMP Info
 *
 *  @retval none
 */
/******************************************************************************************/
static void HTP_Info_Disp( RBLE_HTP_TEMP_INFO *p )
{
    uint8_t l_cnt,i;
    uint8_t l_val[16] ={0};
    uint8_t *p_l_val = &l_val[0];
    const char  *unit[] = { "C", "F" };
    const char  *type_chr[] = {
        "Reserved",
        "Armpit",
        "Body (general)",
        "Ear (usually ear lobe)",
        "Finger",
        "Gastro-intestinal Tract ",
        "Mouth",
        "Rectum",
        "Toe",
        "Tympanum (ear drum)"
    };
    
    printf("flag_stable_meas = %d\n", p->flag_stable_meas );
    printf("flags            = %d\n\n", p->flags );
    printf("  Temperature      : %3.1f (%s)\n", FLOAT2float(p->temp_val), unit[(p->flags & 0x01)]);

    *p_l_val++ = p->flags;
    *p_l_val++ = (uint8_t)((p->temp_val    )&0x000000FF);
    *p_l_val++ = (uint8_t)((p->temp_val>> 8)&0x000000FF);
    *p_l_val++ = (uint8_t)((p->temp_val>>16)&0x000000FF);
    *p_l_val++ = (uint8_t)((p->temp_val>>24)&0x000000FF);
    l_cnt = 5;
    /* Timestamp Present ? */
    if ( 0 != (p->flags & 0x02) ) {
        printf("  Time Stamp       : %d/%02d/%02d %02d:%02d:%02d\n",    p->stamp.year,
                                                                        p->stamp.month,
                                                                        p->stamp.day,
                                                                        p->stamp.hour,
                                                                        p->stamp.min,
                                                                        p->stamp.sec);
        *p_l_val++ = (uint8_t)((p->stamp.year    )&0x00FF);
        *p_l_val++ = (uint8_t)((p->stamp.year>> 8)&0x00FF);
        *p_l_val++ = p->stamp.month;
        *p_l_val++ = p->stamp.day;
        *p_l_val++ = p->stamp.hour;
        *p_l_val++ = p->stamp.min;
        *p_l_val++ = p->stamp.sec;
        l_cnt += 7;
    }
    /* Temperature Type Present ? */
    if ( 0 != (p->flags & 0x04) ) {
        if(10 > p->type){
            printf("  Temperature Type : %d -> %s\n", p->type, type_chr[p->type] );
        }else{
            printf("  Temperature Type : %d -> %s\n", p->type, type_chr[0] );
        }
        *p_l_val++ = p->type;
        l_cnt += 1;
    }
    printf("  Value            : 0x");
    for(i=0 ; i<l_cnt ; i++){
        printf("%02X", l_val[i] );
    }
    printf("\n");
}
#endif //#if (PRF_SEL_HTPC)


#endif  //#if (PRF_SEL_HTPC || PRF_SEL_HTPT)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
