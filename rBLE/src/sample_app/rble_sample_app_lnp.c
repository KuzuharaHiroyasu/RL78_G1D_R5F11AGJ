/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_lnp.c
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

#if (PRF_SEL_LNPC || PRF_SEL_LNPS)
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
#if (PRF_SEL_LNPS)
static void RBLE_APP_LNPS_CallBack( RBLE_LNPS_EVENT *event );                               /* LNP(Sensor) Call Back */
static BOOL RBLE_LNP_Sensor_Enable_Test( void );                                            /* A LNP_Sensor_Enable command is executed */
static BOOL RBLE_LNP_Sensor_Disable_Test( void );                                           /* A LNP_Sensor_Disable command is executed */
static BOOL RBLE_LNP_Sensor_Send_Location_Speed_Test( void );                               /* A LNP_Sensor_Send_Location_Speed command is executed */
static BOOL RBLE_LNP_Sensor_Set_Position_Quality_Test( void );                              /* A LNP_Sensor_Set_Position_Quality command is executed */
static BOOL RBLE_LNP_Sensor_Send_LN_Control_Point_Test( void );                             /* A LNP_Sensor_Send_LN_Control_Point command is executed */
static BOOL RBLE_LNP_Sensor_Send_Navigation_Test( void );                                   /* A LNP_Sensor_Send_Navigation command is executed */
static BOOL RBLE_LNP_Sensor_Send_Battery_Level_Test( void );                                /* A LNP_Sensor_Send_Battery_Level command is executed */
#endif
#if (PRF_SEL_LNPC)
static void RBLE_APP_LNPC_CallBack( RBLE_LNPC_EVENT *event );                               /* LNP(Collector) Call Back */
static BOOL RBLE_LNP_Collector_Enable_Test( void );                                         /* A LNP_Collector_Enable command is executed */
static BOOL RBLE_LNP_Collector_Disable_Test( void );                                        /* A LNP_Collector_Disable command is executed */
static BOOL RBLE_LNP_Collector_Read_Char_Test( void );                                      /* A LNP_Collector_Read_Char command is executed */
static BOOL RBLE_LNP_Collector_Write_LN_Control_Point_Test( void );                         /* A LNP_Collector_Write_LN_Control_Point command is executed */
static BOOL RBLE_LNP_Collector_Write_Char_Test( void );                                     /* A LNP_Collector_Write_Char command is executed */
#endif

#if (PRF_SEL_LNPC)
static void LNP_Location_Speed_Disp (RBLE_LNP_LOCATION_SPEED_INFO *location_speed_info);
static void LNP_Navigation_Disp(RBLE_LNP_NAVIGATION_INFO *navi_info);
static void LNP_LN_Control_Point_Disp(RBLE_LNP_IND_CONTROL_POINT_INFO *ind_cp_info);
#endif

#if (PRF_SEL_LNPS)
static void LNP_Write_Value_Disp(uint8_t opcode, uint32_t value);
static void LNP_Chg_LN_CP_Info_Disp(RBLE_LNP_WR_CONTROL_POINT_INFO *wr_cp_info);
#endif

static void LNP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status);
static void LNP_Event_Disp(char_t *event_str);

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* Location and Navigation Profile Select Menu */
static const MENU_INFO  Test_Prog_LNP_Menu[] = {
#if (PRF_SEL_LNPS)
    {  1,   RBLE_LNP_Sensor_Enable_Test,                        NULL,       "1.LNP Sensor_Enable\n",                        },
    {  2,   RBLE_LNP_Sensor_Disable_Test,                       NULL,       "2.LNP Sensor_Disable\n",                       },
    {  3,   RBLE_LNP_Sensor_Send_Location_Speed_Test,           NULL,       "3.LNP Sensor_Send_Location_Speed\n",           },
    {  4,   RBLE_LNP_Sensor_Set_Position_Quality_Test,          NULL,       "4.LNP Sensor_Set_Position_Quality\n",          },
    {  5,   RBLE_LNP_Sensor_Send_LN_Control_Point_Test,         NULL,       "5.LNP Sensor_Send_LN_Control_Point\n",         },
    {  6,   RBLE_LNP_Sensor_Send_Navigation_Test,               NULL,       "6.LNP Sensor_Send_Navigation\n",               },
    {  7,   RBLE_LNP_Sensor_Send_Battery_Level_Test,            NULL,       "7.LNP Sensor_Send_Battery_Level\n",            },
#endif
#if (PRF_SEL_LNPC)
    {  8,   RBLE_LNP_Collector_Enable_Test,                     NULL,       "8.LNP Collector_Enable\n",                     },
    {  9,   RBLE_LNP_Collector_Disable_Test,                    NULL,       "9.LNP Collector_Disable\n",                    },
    { 10,   RBLE_LNP_Collector_Read_Char_Test,                  NULL,       "10.LNP Collector_Read_Char\n",                 },
    { 11,   RBLE_LNP_Collector_Write_LN_Control_Point_Test,     NULL,       "11.LNP Collector_Write_LN_Control_Point\n",    },
    { 12,   RBLE_LNP_Collector_Write_Char_Test,                 NULL,       "12.LNP Collector_Write_Char\n",                },
#endif
    {  ESC_KEY, Menu_Exit,                                      NULL,       "ESC Key: Menu exit\n",                         },
    {  0,   NULL,                                               NULL,       NULL,                                           }
};

const SELECT_MENU   Test_Prog_LNP_Select_Menu = {
    "\n-- BLE Sample Program LNP Profile Test Menu --",
    Test_Prog_LNP_Menu
};

static const int8_t *LNP_Control_Point_Disp_Info[] = 
{
    "",
    "Set Cumulative",
    "Mask Content",
    "Navigatation Control",
    "Req Num of Route",
    "Req Name of Route",
    "Select Route",
    "Set Fix Rate",
    "Set Elevation"
};

#if (PRF_SEL_LNPC)
static RBLE_LNS_CONTENT g_lns = {0};
static RBLE_DIS_CONTENT g_lnp_dis = {0};
static RBLE_BATS_CONTENT g_lnp_bas = {0};
#endif


/******************************* Program Area *****************************************/
#if (PRF_SEL_LNPS)
/******************************************************************************************/
/**
 *  @brief rBLE LNPS(Sensor) Call Back
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
static void RBLE_APP_LNPS_CallBack( RBLE_LNPS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_LNP_EVENT_SENSOR_ENABLE_COMP:
            LNP_Event_Status_Disp("SENSOR_ENABLE_COMP", event->param.sensor_enable.status);
            Connection_Handle_Disp( event->param.sensor_enable.conhdl );
            break;
        case RBLE_LNP_EVENT_SENSOR_DISABLE_COMP:
            LNP_Event_Disp("SENSOR_DISABLE_COMP");
            Connection_Handle_Disp( event->param.sensor_disable.conhdl );
            break;
        case RBLE_LNP_EVENT_SENSOR_ERROR_IND:
            LNP_Event_Status_Disp("SENSOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_LNP_EVENT_SENSOR_SEND_LOCATION_SPEED_COMP:
            LNP_Event_Status_Disp("SENSOR_SEND_LOCATION_SPEED_COMP", event->param.send_location_speed.status);
            Connection_Handle_Disp( event->param.send_location_speed.conhdl );
            break;
        case RBLE_LNP_EVENT_SENSOR_SET_POSITION_QUALITY_COMP:
            LNP_Event_Status_Disp("SENSOR_SET_POSITION_QUALITY_COMP", event->param.set_position_quality.status);
            break;
        case RBLE_LNP_EVENT_SENSOR_SEND_LN_CP_COMP:
            LNP_Event_Status_Disp("SENSOR_SEND_LN_CP_COMP", event->param.send_ln_cp.status);
            Connection_Handle_Disp( event->param.send_ln_cp.conhdl );
            break;
        case RBLE_LNP_EVENT_SENSOR_SEND_NAVIGATION_COMP:
            LNP_Event_Status_Disp("SENSOR_SEND_NAVIGATION_COMP", event->param.send_navigation.status);
            Connection_Handle_Disp( event->param.send_navigation.conhdl );
            break;
        case RBLE_LNP_EVENT_SENSOR_SEND_BATTERY_LEVEL_COMP:
            LNP_Event_Status_Disp("SENSOR_SEND_BATTERY_LEVEL_COMP", event->param.send_battery_level.status);
            Connection_Handle_Disp( event->param.send_battery_level.conhdl );
            break;
        case RBLE_LNP_EVENT_SENSOR_CHG_LN_CP_IND:
            LNP_Event_Disp("SENSOR_CHG_LN_CP_IND");
            Connection_Handle_Disp( event->param.chg_ln_cp_ind.conhdl );
            LNP_Chg_LN_CP_Info_Disp( &event->param.chg_ln_cp_ind.wr_cp_info );
            break;
        case RBLE_LNP_EVENT_SENSOR_CFG_INDNTF_IND:
            LNP_Event_Disp("SENSOR_CFG_INDNTF_IND");
            Connection_Handle_Disp( event->param.cfg_indntf_ind.conhdl );
            printf("char:%d  cfg:%d\n", event->param.cfg_indntf_ind.char_code, event->param.cfg_indntf_ind.cfg_val);
            break;
        case RBLE_LNP_EVENT_SENSOR_COMMAND_DISALLOWED_IND:
            LNP_Event_Status_Disp("SENSOR_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A LNP_Sensor_Enable command is executed.
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
static BOOL RBLE_LNP_Sensor_Enable_Test( void )
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
    RBLE_LNP_SENSOR_PARAM param = {
        RBLE_PRF_START_NTF,
        RBLE_PRF_START_IND,
        RBLE_PRF_START_NTF,
        RBLE_PRF_START_NTF
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
        Ret_Status = RBLE_LNP_Sensor_Enable( Connection_Handle, sec_lvl[select], con_type, &param, RBLE_APP_LNPS_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A LNP_Sensor_Disable command is executed.
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
static BOOL RBLE_LNP_Sensor_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_LNP_Sensor_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A LNP_Sensor_Send_Location_Speed command is executed.
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
static BOOL RBLE_LNP_Sensor_Send_Location_Speed_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_LNP_LOCATION_SPEED_INFO location_speed_info = {
        0x0000,         /* flags */
        0x3333,         /* instant_speed */
        200000,         /* total_distance */
        1050000000,     /* latitude */
        1430000000,     /* longitude */
        -59,            /* elevation */
        344,            /* heading */
        0x02,           /* rolling_time */
        0x00,           /* reserved */
        {2013,8,22,19,13,42,0}      /* utc_time(2013/8/22 19:13:42) */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            location_speed_info.flags = (uint16_t)RBLE_Parameter_Arg[ 0 ];
            if ( 1 < RBLE_Parameter_Cnt ) {
                location_speed_info.total_distance = RBLE_Parameter_Arg[ 1 ];
            }
        }
        /* API Call */
        Ret_Status = RBLE_LNP_Sensor_Send_Location_Speed( Connection_Handle, &location_speed_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A LNP_Sensor_Set_Position_Quality command is executed.
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
static BOOL RBLE_LNP_Sensor_Set_Position_Quality_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_LNP_POSITION_QUALITY_INFO quality_info = 
    {
        0x0000,         /* flags */
        5,              /* Number of Beacons in Solution */
        4,              /* Number of Beacons in View */
        0x0030,         /* Time to First Fix */
        0xff00ff00,     /* EHPE */
        0xccff00ff,     /* EVPE */
        0x08,           /* HDOP */
        0x09            /* VDOP */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            quality_info.flags = (uint16_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_LNP_Sensor_Set_Position_Quality( &quality_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A LNP_Sensor_Send_LN_Control_Point command is executed.
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
static BOOL RBLE_LNP_Sensor_Send_LN_Control_Point_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_LNP_IND_CONTROL_POINT_INFO ind_cp_info = 
    {
        RBLE_LNP_OP_RESPONSE_CODE,      /* OpCode */
        RBLE_LNP_OP_SET_CUMULATIVE_CODE,/* request op code */
        RBLE_LNP_RES_SUCCESS_CODE,      /* response_value */
        0,                              /* reserved */
        5,                              /* route_num */
        17,                             /* name_sie */
        { 'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q' } /* route_name */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            ind_cp_info.request_op_code = (uint8_t)RBLE_Parameter_Arg[ 0 ];
            
            if( RBLE_Parameter_Cnt > 1 ) {
                ind_cp_info.response_value = (uint8_t)RBLE_Parameter_Arg[ 1 ];
                
                if( RBLE_Parameter_Cnt > 2 ) {
                    switch(ind_cp_info.request_op_code)
                    {
                        case RBLE_LNP_OP_REQ_NUM_OF_ROUTE_CODE:
                            ind_cp_info.route_num = (uint16_t)RBLE_Parameter_Arg[ 2 ];
                            break;
                        case RBLE_LNP_OP_REQ_NAME_OF_ROUTE_CODE:
                            ind_cp_info.name_size = (uint8_t)RBLE_Parameter_Arg[ 2 ];
                            break;
                        case RBLE_LNP_OP_SET_FIX_RATE_CODE:
                        case RBLE_LNP_OP_SET_ELEVATION_CODE:
                        case RBLE_LNP_OP_SELECT_ROUTE_CODE:
                        case RBLE_LNP_OP_SET_CUMULATIVE_CODE:
                        case RBLE_LNP_OP_MASK_LS_CONTENTS_CODE:
                        case RBLE_LNP_OP_NAVIGATION_CONTROL_CODE:
                        default:
                            break;
                    }
                }
            }
        }
        /* API Call */
        Ret_Status = RBLE_LNP_Sensor_Send_LN_Control_Point( Connection_Handle, &ind_cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A LNP_Sensor_Send_Navigation command is executed.
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
static BOOL RBLE_LNP_Sensor_Send_Navigation_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_LNP_NAVIGATION_INFO navigation_info = 
    {
        0x03,                   /* flags */
        18000,                  /* Bearing(180) */
        3000,                   /* Heading(30) */
        40000,                  /* Remaining Distance(4000m) */
        800,                    /* Remaining V Distance(8m) */
        {2013,8,23,16,22,10,0}, /* Estimated time of arrival (2013/8/23 16:22:10) */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            navigation_info.flags = (uint16_t)RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_LNP_Sensor_Send_Navigation( Connection_Handle, &navigation_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A LNP_Sensor_Send_Battery_Level command is executed.
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
static BOOL RBLE_LNP_Sensor_Send_Battery_Level_Test( void )
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
        /* API Call */
        Ret_Status = RBLE_LNP_Sensor_Send_Battery_Level( Connection_Handle, battery_level );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_LNPS)

#if (PRF_SEL_LNPC)
/******************************************************************************************/
/**
 *  @brief rBLE LNPC(Collector) Call Back
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
static void RBLE_APP_LNPC_CallBack( RBLE_LNPC_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_LNP_EVENT_COLLECTOR_ENABLE_COMP:
            LNP_Event_Status_Disp("COLLECTOR_ENABLE_COMP", event->param.collector_enable.status);
            Connection_Handle_Disp( event->param.collector_enable.conhdl );
            printf("\nLocation and Navigation Service\n" );
            printf("   Start Handle = 0x%04x\n", event->param.collector_enable.lns.shdl);
            printf("   End Handle   = 0x%04x\n", event->param.collector_enable.lns.ehdl);
            printf("   ln_feature_char_hdl       = 0x%04x\n", event->param.collector_enable.lns.ln_feature_char_hdl);
            printf("   ln_feature_val_hdl        = 0x%04x\n", event->param.collector_enable.lns.ln_feature_val_hdl);
            printf("   ln_feature_prop           = 0x%04x\n", event->param.collector_enable.lns.ln_feature_prop);
            printf("   location_speed_char_hdl   = 0x%04x\n", event->param.collector_enable.lns.location_speed_char_hdl);
            printf("   location_speed_val_hdl    = 0x%04x\n", event->param.collector_enable.lns.location_speed_val_hdl);
            printf("   location_speed_cfg_hdl    = 0x%04x\n", event->param.collector_enable.lns.location_speed_cfg_hdl);
            printf("   location_speed_prop       = 0x%04x\n", event->param.collector_enable.lns.location_speed_prop);
            printf("   position_quality_char_hdl = 0x%04x\n", event->param.collector_enable.lns.position_quality_char_hdl);
            printf("   position_quality_val_hdl  = 0x%04x\n", event->param.collector_enable.lns.position_quality_val_hdl);
            printf("   position_quality_prop     = 0x%04x\n", event->param.collector_enable.lns.position_quality_prop);
            printf("   ln_cp_char_hdl            = 0x%04x\n", event->param.collector_enable.lns.ln_cp_char_hdl);
            printf("   ln_cp_val_hdl             = 0x%04x\n", event->param.collector_enable.lns.ln_cp_val_hdl);
            printf("   ln_cp_cfg_hdl             = 0x%04x\n", event->param.collector_enable.lns.ln_cp_cfg_hdl);
            printf("   ln_cp_prop                = 0x%04x\n", event->param.collector_enable.lns.ln_cp_prop);
            printf("   navigation_char_hdl       = 0x%04x\n", event->param.collector_enable.lns.navigation_char_hdl);
            printf("   navigation_val_hdl        = 0x%04x\n", event->param.collector_enable.lns.navigation_val_hdl);
            printf("   navigation_cfg_hdl        = 0x%04x\n", event->param.collector_enable.lns.navigation_cfg_hdl);
            printf("   navigation_prop           = 0x%04x\n", event->param.collector_enable.lns.navigation_prop);
            DIS_handle_Info_Disp(&event->param.collector_enable.dis);
            BAS_handle_Info_Disp(&event->param.collector_enable.bas);

            g_lns = event->param.collector_enable.lns;
            g_lnp_dis = event->param.collector_enable.dis;
            g_lnp_bas = event->param.collector_enable.bas;
            break;
        case RBLE_LNP_EVENT_COLLECTOR_DISABLE_COMP:
            LNP_Event_Status_Disp("COLLECTOR_DISABLE_COMP", event->param.collector_disable.status);
            Connection_Handle_Disp( event->param.collector_disable.conhdl );
            break;
        case RBLE_LNP_EVENT_COLLECTOR_ERROR_IND:
            LNP_Event_Status_Disp("COLLECTOR_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_LNP_EVENT_COLLECTOR_LOCATION_SPEED_NTF:
            LNP_Event_Disp("COLLECTOR_LOCATION_SPEED_NTF");
            Connection_Handle_Disp( event->param.location_speed_ntf.conhdl );
            LNP_Location_Speed_Disp( &event->param.location_speed_ntf.location_speed_info );
            break;
        case RBLE_LNP_EVENT_COLLECTOR_LN_CP_IND:
            LNP_Event_Disp("COLLECTOR_LN_CP_IND");
            Connection_Handle_Disp( event->param.ln_cp_ind.conhdl );
            LNP_LN_Control_Point_Disp( &event->param.ln_cp_ind.ind_cp_info );
            break;
        case RBLE_LNP_EVENT_COLLECTOR_NAVIGATION_NTF:
            LNP_Event_Disp("COLLECTOR_NAVIGATION_NTF");
            Connection_Handle_Disp( event->param.navigation_ntf.conhdl );
            LNP_Navigation_Disp( &event->param.navigation_ntf.navigation_info);
            break;
        case RBLE_LNP_EVENT_COLLECTOR_BATTERY_LEVEL_NTF:
            LNP_Event_Disp("COLLECTOR_BATTERY_LEVEL_NTF");
            Connection_Handle_Disp( event->param.battery_level_ntf.conhdl );
            printf( "Battery Level = %d\n", event->param.battery_level_ntf.battery_level );
            break;
        case RBLE_LNP_EVENT_COLLECTOR_READ_CHAR_RESPONSE:
            LNP_Event_Status_Disp("CLIENT_READ_CHAR_RESPONSE", event->param.rd_char_resp.att_code);
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            ATT_Info_Disp( &event->param.rd_char_resp.data );
            event->param.rd_char_resp.data.data[event->param.rd_char_resp.data.len] = '\0';
            printf("(%s)\n", event->param.rd_char_resp.data.data);
            break;
        case RBLE_LNP_EVENT_COLLECTOR_WRITE_CHAR_RESPONSE:
            LNP_Event_Status_Disp("COLLECTOR_WRITE_CHAR_RESPONSE", event->param.wr_char_resp.att_code);
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            break;
        case RBLE_LNP_EVENT_COLLECTOR_COMMAND_DISALLOWED_IND:
            LNP_Event_Status_Disp("COLLECTOR_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A LNP_Collector_Enable command is executed.
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
static BOOL RBLE_LNP_Collector_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_lns.shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_LNP_Collector_Enable( Connection_Handle, con_type, &g_lns, &g_lnp_dis, &g_lnp_bas, RBLE_APP_LNPC_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A LNP_Collector_Disable command is executed.
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
static BOOL RBLE_LNP_Collector_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_LNP_Collector_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A LNP_Collector_Read_Char command is executed.
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
static BOOL RBLE_LNP_Collector_Read_Char_Test( void )
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
        Ret_Status = RBLE_LNP_Collector_Read_Char( Connection_Handle, char_code );
        printf( " char_code = %d\n", char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A LNP_Collector_Write_LN_Control_Point command is executed.
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
static BOOL RBLE_LNP_Collector_Write_LN_Control_Point_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_LNP_WR_CONTROL_POINT_INFO wr_cp_info = 
    {
        RBLE_LNP_OP_SET_CUMULATIVE_CODE,    /* OpCode */
        0,                                  /* reserved */
        80000,                              /* cumulative value */
        0x007f,                             /* content_mask */
        5,                                  /* route_num */
        RBLE_LNP_CNTL_NAVI_START,           /* control_val */
        2,                                  /* fix_rate */
        -10                                 /* elevation */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        wr_cp_info.OpCode = (uint8_t)RBLE_Parameter_Arg[ 0 ];
        if( RBLE_Parameter_Cnt > 1 ) {
            switch(wr_cp_info.OpCode)
            {
                case RBLE_LNP_OP_SET_CUMULATIVE_CODE:
                    wr_cp_info.cumulative_value = (uint32_t)RBLE_Parameter_Arg[ 1 ];
                    break;
                case RBLE_LNP_OP_MASK_LS_CONTENTS_CODE:
                    wr_cp_info.content_mask = (uint16_t)RBLE_Parameter_Arg[ 1 ];
                    break;
                case RBLE_LNP_OP_NAVIGATION_CONTROL_CODE:
                    wr_cp_info.control_val =(uint8_t) RBLE_Parameter_Arg[ 1 ];
                    break;
                case RBLE_LNP_OP_REQ_NAME_OF_ROUTE_CODE:
                case RBLE_LNP_OP_SELECT_ROUTE_CODE:
                    wr_cp_info.route_num =(uint16_t) RBLE_Parameter_Arg[ 1 ];
                    break;
                case RBLE_LNP_OP_SET_FIX_RATE_CODE:
                    wr_cp_info.fix_rate = (uint8_t)RBLE_Parameter_Arg[ 1 ];
                    break;
                case RBLE_LNP_OP_SET_ELEVATION_CODE:
                    wr_cp_info.elevation = (int32_t)RBLE_Parameter_Arg[ 1 ];
                    break;
                case RBLE_LNP_OP_REQ_NUM_OF_ROUTE_CODE:
                default:
                    break;
            }
        }
        /* API Call */
        Ret_Status = RBLE_LNP_Collector_Write_LN_Control_Point( Connection_Handle, &wr_cp_info );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A LNP_Collector_Write_Char command is executed.
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
static BOOL RBLE_LNP_Collector_Write_Char_Test( void )
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
                cfg_val = (uint16_t)RBLE_Parameter_Arg[ 1 ];
            }
        }
        /* API Call */
        Ret_Status = RBLE_LNP_Collector_Write_Char( Connection_Handle, char_code, cfg_val );
        printf( " char_code = %d, cfg_val = 0x%04X\n", char_code, cfg_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_LNPC)



#if (PRF_SEL_LNPC)
/******************************************************************************************/
/**
 *  @brief Display Location and Speed
 *
 *  @b Contents of management:
 *
 *  @param[in] location_speed_info     location and speed information
 *
 *  @retval none
 */
/******************************************************************************************/
static void LNP_Location_Speed_Disp(RBLE_LNP_LOCATION_SPEED_INFO *location_speed_info)
{
    printf("flags:0x%04x\n",location_speed_info->flags);
    
    if(location_speed_info->flags & 0x01)
    {
        printf("instant_speed :0x%04x\n",location_speed_info->instant_speed);
    }
    if(location_speed_info->flags & 0x02)
    {
        printf("total_distance:%lu (0x%08lX)\n", location_speed_info->total_distance, location_speed_info->total_distance);
    }
    if(location_speed_info->flags & 0x04)
    {
        printf("latitude      :%lu (0x%08lX)\n", location_speed_info->latitude, location_speed_info->latitude);
        printf("longitude     :%lu (0x%08lX)\n", location_speed_info->longitude, location_speed_info->longitude);
    }
    if(location_speed_info->flags & 0x08)
    {
        printf("elevation     :%ld (0x%08lX)\n", location_speed_info->elevation, location_speed_info->elevation);
    }
    if(location_speed_info->flags & 0x10)
    {
        printf("heading       :%d(0x%04x)\n",location_speed_info->heading,location_speed_info->heading);
    }
    if(location_speed_info->flags & 0x20)
    {
        printf("rolling time  :%d(0x%02x)\n",location_speed_info->rolling_time,location_speed_info->rolling_time);
    }
    if(location_speed_info->flags & 0x40)
    {
        printf("utc time      :");
        TIP_Time_Disp(&location_speed_info->utc_time);
    }
}

/******************************************************************************************/
/**
 *  @brief Display Navigtation
 *
 *  @b Contents of management:
 *
 *  @param[in] navi_info     Navigtation information
 *
 *  @retval none
 */
/******************************************************************************************/
static void LNP_Navigation_Disp(RBLE_LNP_NAVIGATION_INFO *navi_info)
{
    printf("flags:0x%04x\n",navi_info->flags);
    
    printf("bearing      :%d(0x%04x)\n",navi_info->bearing,navi_info->bearing);
    printf("heading      :%d(0x%04x)\n",navi_info->heading,navi_info->heading);
    if(navi_info->flags & 0x01)
    {
        printf("remain dis   :%lu (0x%08lX)\n", navi_info->remain_dis, navi_info->remain_dis);
    }
    if(navi_info->flags & 0x02)
    {
        printf("remain v dis :%ld (0x%08lX)\n", navi_info->remain_v_dis, navi_info->remain_v_dis);
    }
    if(navi_info->flags & 0x04)
    {
        printf("estimate time:");
        TIP_Time_Disp(&navi_info->estimate_time);
    }
}

/******************************************************************************************/
/**
 *  @brief Display LN Control Point Indication
 *
 *  @b Contents of management:
 *
 *  @param[in] ind_cp_info     LN Control Point Indication information
 *
 *  @retval none
 */
/******************************************************************************************/
static void LNP_LN_Control_Point_Disp(RBLE_LNP_IND_CONTROL_POINT_INFO *ind_cp_info)
{
    if(ind_cp_info->OpCode == RBLE_LNP_OP_RESPONSE_CODE) {
        printf("OpCode = RESPONSE\n");
    }
    else {
        printf("Illegal OpCode = %02x\n", ind_cp_info->OpCode);
    }
    switch(ind_cp_info->request_op_code)
    {
        case RBLE_LNP_OP_SET_CUMULATIVE_CODE:
        case RBLE_LNP_OP_MASK_LS_CONTENTS_CODE:
        case RBLE_LNP_OP_NAVIGATION_CONTROL_CODE:
        case RBLE_LNP_OP_SELECT_ROUTE_CODE:
        case RBLE_LNP_OP_SET_FIX_RATE_CODE:
        case RBLE_LNP_OP_SET_ELEVATION_CODE:
            printf("%s :RES = %d\n", LNP_Control_Point_Disp_Info[ind_cp_info->request_op_code], ind_cp_info->response_value);
            break;
        case RBLE_LNP_OP_REQ_NUM_OF_ROUTE_CODE:
            printf("%s :RES = %d\n", LNP_Control_Point_Disp_Info[ind_cp_info->request_op_code], ind_cp_info->response_value);
            if(ind_cp_info->response_value == RBLE_LNP_RES_SUCCESS_CODE)
            {
                printf("Route Num :%d\n", ind_cp_info->route_num);
            }
            break;
        case RBLE_LNP_OP_REQ_NAME_OF_ROUTE_CODE:
            printf("%s :RES = %d\n", LNP_Control_Point_Disp_Info[ind_cp_info->request_op_code], ind_cp_info->response_value);
            if(ind_cp_info->response_value == RBLE_LNP_RES_SUCCESS_CODE)
            {
                ind_cp_info->route_name[ind_cp_info->name_size] = '\0';
                printf("Route Name:%s\n", ind_cp_info->route_name);
            }
            break;
        default:
            printf("Illegal Req OpCode %d\n",ind_cp_info->request_op_code);
            break;
    }
}

#endif  //#if (PRF_SEL_LNPC)

#if (PRF_SEL_LNPS)
/******************************************************************************************/
/**
 *  @brief Display LN Control Point Write value and Parameter value disp
 *
 *  @b Contents of control point Indication:
 *
 *  @param[in] opcode   request operation code
 *  @param[in] value    parameter value
 *
 *  @retval none
 */
/******************************************************************************************/
static void LNP_Write_Value_Disp(uint8_t opcode, uint32_t value)
{
    if( RBLE_LNP_OP_SET_ELEVATION_CODE == opcode ) {
        printf("%s(%d) :Value = %ld(0x%08lx)\n", LNP_Control_Point_Disp_Info[opcode], opcode, (int32_t)value, value);
    } else {
        printf("%s(%d) :Value = %lu(0x%08lx)\n", LNP_Control_Point_Disp_Info[opcode], opcode, value, value);
    }
}

static void LNP_Chg_LN_CP_Info_Disp(RBLE_LNP_WR_CONTROL_POINT_INFO *wr_cp_info)
{
    switch(wr_cp_info->OpCode)
    {
        case RBLE_LNP_OP_SET_CUMULATIVE_CODE:
            LNP_Write_Value_Disp(wr_cp_info->OpCode, wr_cp_info->cumulative_value);
            break;
        case RBLE_LNP_OP_MASK_LS_CONTENTS_CODE:
            LNP_Write_Value_Disp(wr_cp_info->OpCode, wr_cp_info->content_mask);
            break;
        case RBLE_LNP_OP_NAVIGATION_CONTROL_CODE:
            LNP_Write_Value_Disp(wr_cp_info->OpCode, wr_cp_info->control_val);
            switch(wr_cp_info->control_val)
            {
                case RBLE_LNP_CNTL_NAVI_STOP:
                    printf("NAVI Stop\n");
                    break;
                case RBLE_LNP_CNTL_NAVI_START:
                    printf("NAVI Start\n");
                    break;
                case RBLE_LNP_CNTL_NAVI_PAUSE:
                    printf("NAVI Pause\n");
                    break;
                case RBLE_LNP_CNTL_NAVI_RESUME:
                    printf("NAVI Resume\n");
                    break;
                case RBLE_LNP_CNTL_NAVI_SKIP:
                    printf("NAVI Skip\n");
                    break;
                case RBLE_LNP_CNTL_NAVI_SET_NEAREST:
                    printf("NAVI Set Nearest\n");
                    break;
            }
            break;
        case RBLE_LNP_OP_REQ_NUM_OF_ROUTE_CODE:
            printf("%s(%d)\n", LNP_Control_Point_Disp_Info[wr_cp_info->OpCode], wr_cp_info->OpCode);
            break;
        case RBLE_LNP_OP_REQ_NAME_OF_ROUTE_CODE:
            LNP_Write_Value_Disp(wr_cp_info->OpCode, wr_cp_info->route_num);
            break;
        case RBLE_LNP_OP_SELECT_ROUTE_CODE:
            LNP_Write_Value_Disp(wr_cp_info->OpCode, wr_cp_info->route_num);
            break;
        case RBLE_LNP_OP_SET_FIX_RATE_CODE:
            LNP_Write_Value_Disp(wr_cp_info->OpCode, wr_cp_info->fix_rate);
            break;
        case RBLE_LNP_OP_SET_ELEVATION_CODE:
            LNP_Write_Value_Disp(wr_cp_info->OpCode, ( uint32_t )wr_cp_info->elevation);
            break;
        default:
            printf("Illegal OpCode %d\n",wr_cp_info->OpCode);
            break;
    }
}
#endif  //#if (PRF_SEL_LNPS)

/******************************************************************************************/
/**
 *  @brief rBLE LNP Event and Status Disp
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
static void LNP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE LNP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}
/******************************************************************************************/
/**
 *  @brief rBLE LNP Event Disp
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
static void LNP_Event_Disp(char_t *event_str)
{
    printf( "\nrBLE LNP EVENT (%s)\n", event_str );
}


#endif  //#if (PRF_SEL_LNPC || PRF_SEL_LNPS)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
