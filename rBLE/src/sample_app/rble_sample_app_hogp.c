/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_hogp.c
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

#if (PRF_SEL_HGHD || PRF_SEL_HGBH || PRF_SEL_HGRH)
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
#if (PRF_SEL_HGHD)
static void RBLE_APP_HGHD_CallBack( RBLE_HGHD_EVENT *event );                               /* HOGP(HostDevice) Call Back */
static BOOL RBLE_HGP_HDevice_Enable_Test( void );                                           /* A HGP_HDevice_Enable command is executed. */
static BOOL RBLE_HGP_HDevice_Disable_Test( void );                                          /* A HGP_HDevice_Disable command is executed. */
static BOOL RBLE_HGP_HDevice_Send_Report_Test( void );                                      /* A HGP_HDevice_Send_Report command is executed. */
static BOOL RBLE_HGP_HDevice_Send_Battery_Level_Test( void );                               /* A HGP_HDevice_Send_Battery_Level command is executed. */
#endif
#if (PRF_SEL_HGBH)
static void RBLE_APP_HGBH_CallBack( RBLE_HGBH_EVENT *event );                               /* HOGP(BootHost) Call Back */
static BOOL RBLE_HGP_BHost_Enable_Test( void );                                             /* A HGP_BHost_Enable command is executed. */
static BOOL RBLE_HGP_BHost_Disable_Test( void );                                            /* A HGP_BHost_Disable command is executed. */
static BOOL RBLE_HGP_BHost_Read_Char_Test( void );                                          /* A HGP_BHost_Read_Char command is executed. */
static BOOL RBLE_HGP_BHost_Read_By_UUID_Char_Test( void );                                  /* A HGP_BHost_Read_By_UUID_Char command is executed. */
static BOOL RBLE_HGP_BHost_Write_Char_Test( void );                                         /* A HGP_BHost_Write_Char command is executed. */
static BOOL RBLE_HGP_BHost_Set_Report_Test( void );                                         /* A HGP_BHost_Set_Report command is executed. */
static BOOL RBLE_HGP_BHost_Write_Protocol_Mode_Test( void );                                /* A HGP_BHost_Write_Protocol_Mode command is executed. */
static BOOL RBLE_HGP_BHost_Data_Output_Test( void );                                        /* A HGP_BHost_Data_Output command is executed. */
#endif
#if (PRF_SEL_HGRH)
static void RBLE_APP_HGRH_CallBack( RBLE_HGRH_EVENT *event );                               /* HOGP(ReportHost) Call Back */
static BOOL RBLE_HGP_RHost_Enable_Test( void );                                             /* A HGP_RHost_Enable command is executed. */
static BOOL RBLE_HGP_RHost_Disable_Test( void );                                            /* A HGP_RHost_Disable command is executed. */
static BOOL RBLE_HGP_RHost_Read_Char_Test( void );                                          /* A HGP_RHost_Read_Char command is executed. */
static BOOL RBLE_HGP_RHost_Read_By_UUID_Char_Test( void );                                  /* A HGP_RHost_Read_By_UUID_Char command is executed. */
static BOOL RBLE_HGP_RHost_Read_Long_Char_Test( void );                                     /* A HGP_RHost_Read_Long_Char command is executed. */
static BOOL RBLE_HGP_RHost_Write_Char_Test( void );                                         /* A HGP_RHost_Write_Char command is executed. */
static BOOL RBLE_HGP_RHost_Set_Report_Test( void );                                         /* A HGP_RHost_Set_Report command is executed. */
static BOOL RBLE_HGP_RHost_Write_Protocol_Mode_Test( void );                                /* A HGP_RHost_Write_Protocol_Mode command is executed. */
static BOOL RBLE_HGP_RHost_Data_Output_Test( void );                                        /* A HGP_RHost_Data_Output command is executed. */
static BOOL RBLE_HGP_RHost_Write_Control_Point_Test( void );                                /* A HGP_RHost_Write_Control_Point command is executed. */
#endif

#if (PRF_SEL_HGBH || PRF_SEL_HGRH)
static void HGP_Info_Disp( RBLE_HIDS_CONTENT *hids, RBLE_DIS11_CONTENT *dis, RBLE_BAS_CONTENT *bas, uint8_t hids_inst_num, uint8_t bas_inst_num );
#endif
static void HGP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status);
static void HGP_Event_Disp(char_t *event_str);


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/* HID over GATT Profile Select Menu */
static MENU_INFO    Test_Prog_HOGP_Menu[] = {
#if (PRF_SEL_HGHD)
    {  1,   RBLE_HGP_HDevice_Enable_Test,                   NULL,       " 1.HGP_HDevice_Enable\n",              },
    {  2,   RBLE_HGP_HDevice_Disable_Test,                  NULL,       " 2.HGP_HDevice_Disable\n",             },
    {  3,   RBLE_HGP_HDevice_Send_Report_Test,              NULL,       " 3.HGP_HDevice_Send_Report\n",         },
    {  4,   RBLE_HGP_HDevice_Send_Battery_Level_Test,       NULL,       " 4.HGP_HDevice_Send_Battery_Level\n",  },
#endif
#if (PRF_SEL_HGBH)
    {  5,   RBLE_HGP_BHost_Enable_Test,                     NULL,       " 5.HGP_BHost_Enable\n",                },
    {  6,   RBLE_HGP_BHost_Disable_Test,                    NULL,       " 6.HGP_BHost_Disable\n",               },
    {  7,   RBLE_HGP_BHost_Read_Char_Test,                  NULL,       " 7.HGP_BHost_Read_Char\n",             },
    {  8,   RBLE_HGP_BHost_Read_By_UUID_Char_Test,          NULL,       " 8.HGP_BHost_Read_By_UUID_Char\n",     },
    {  9,   RBLE_HGP_BHost_Write_Char_Test,                 NULL,       " 9.HGP_BHost_Write_Char\n",            },
    {  10,  RBLE_HGP_BHost_Set_Report_Test,                 NULL,       "10.HGP_BHost_Set_Report\n",            },
    {  11,  RBLE_HGP_BHost_Write_Protocol_Mode_Test,        NULL,       "11.HGP_BHost_Write_Protocol_Mode\n",   },
    {  12,  RBLE_HGP_BHost_Data_Output_Test,                NULL,       "12.HGP_BHost_Data_Output\n",           },
#endif
#if (PRF_SEL_HGRH)
    {  13,  RBLE_HGP_RHost_Enable_Test,                     NULL,       "13.HGP_RHost_Enable\n",                },
    {  14,  RBLE_HGP_RHost_Disable_Test,                    NULL,       "14.HGP_RHost_Disable\n",               },
    {  15,  RBLE_HGP_RHost_Read_Char_Test,                  NULL,       "15.HGP_RHost_Read_Char\n",             },
    {  16,  RBLE_HGP_RHost_Read_By_UUID_Char_Test,          NULL,       "16.HGP_RHost_Read_By_UUID_Char\n",     },
    {  17,  RBLE_HGP_RHost_Read_Long_Char_Test,             NULL,       "17.HGP_RHost_Read_Long_Char\n",        },
    {  18,  RBLE_HGP_RHost_Write_Char_Test,                 NULL,       "18.HGP_RHost_Write_Char\n",            },
    {  19,  RBLE_HGP_RHost_Set_Report_Test,                 NULL,       "19.HGP_RHost_Set_Report\n",            },
    {  20,  RBLE_HGP_RHost_Write_Protocol_Mode_Test,        NULL,       "20.HGP_RHost_Write_Protocol_Mode\n",   },
    {  21,  RBLE_HGP_RHost_Data_Output_Test,                NULL,       "21.HGP_RHost_Data_Output\n",           },
    {  22,  RBLE_HGP_RHost_Write_Control_Point_Test,        NULL,       "22.HGP_RHost_Write_Control_Point\n",   },
#endif
    {  ESC_KEY, Menu_Exit,                                  NULL,       "ESC Key: Menu exit\n",                 },
    {  0,   NULL,                                           NULL,       NULL,                                   }
};

const SELECT_MENU Test_Prog_HOGP_Select_Menu = {
    "\n-- BLE Sample Program HID over GATT Profile Test Menu --",
    Test_Prog_HOGP_Menu
};


#if (PRF_SEL_HGBH || PRF_SEL_HGRH)
static RBLE_HIDS_CONTENT g_hids[RBLE_HIDS_INST_MAX] = {0};
static RBLE_DIS11_CONTENT g_hogp_dis = {0};
static RBLE_BAS_CONTENT g_hogp_bas[RBLE_BAS_INST_MAX] = {0};
#endif


/******************************* Program Area *****************************************/

#if (PRF_SEL_HGHD)
/******************************************************************************************/
/**
 *  @brief HID over GATT(HGHD) Profile Call Back
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
static void RBLE_APP_HGHD_CallBack( RBLE_HGHD_EVENT *event )
{
    uint16_t i;
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_HGP_EVENT_HDEVICE_ENABLE_COMP:
            HGP_Event_Status_Disp("HDEVICE_ENABLE_COMP", event->param.hdevice_enable.status);
            Connection_Handle_Disp( event->param.hdevice_enable.conhdl );
            break;
        case RBLE_HGP_EVENT_HDEVICE_DISABLE_COMP:
            HGP_Event_Status_Disp("HDEVICE_DISABLE_COMP", event->param.hdevice_disable.status);
            Connection_Handle_Disp( event->param.hdevice_disable.conhdl );
            printf( "hids_inst_num     = %d\n", event->param.hdevice_disable.device_info.hids_inst_num );
            printf( "bas_inst_num      = %d\n", event->param.hdevice_disable.device_info.bas_inst_num );
            printf( "kb_report_ntf_en[RBLE_HIDS_INST_MAX(=%d)]     =", RBLE_HIDS_INST_MAX );
            for(i=0;i<RBLE_HIDS_INST_MAX;i++){
                printf( " 0x%04X", event->param.hdevice_disable.device_info.kb_report_ntf_en[i] );
            }
            printf( "\n" );
            printf( "mo_report_ntf_en[RBLE_HIDS_INST_MAX(=%d)]     =", RBLE_HIDS_INST_MAX );
            for(i=0;i<RBLE_HIDS_INST_MAX;i++){
                printf( " 0x%04X", event->param.hdevice_disable.device_info.mo_report_ntf_en[i] );
            }
            printf( "\n" );
            printf( "protocol_mode_val[RBLE_HIDS_INST_MAX(=%d)]    =", RBLE_HIDS_INST_MAX );
            for(i=0;i<RBLE_HIDS_INST_MAX;i++){
                printf( " 0x%02X", event->param.hdevice_disable.device_info.protocol_mode_val[i] );
            }
            printf( "\n" );
            printf( "battery_level_ntf_en[RBLE_HIDS_INST_MAX(=%d)] =", RBLE_HIDS_INST_MAX );
            for(i=0;i<RBLE_HIDS_INST_MAX;i++){
                printf( " 0x%04X", event->param.hdevice_disable.device_info.battery_level_ntf_en[i] );
            }
            printf( "\n" );
            break;
        case RBLE_HGP_EVENT_HDEVICE_ERROR_IND:
            HGP_Event_Disp("HDEVICE_ERROR_IND");
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_HGP_EVENT_HDEVICE_CFG_INDNTF_IND:
            HGP_Event_Disp("HDEVICE_CFG_INDNTF_IND");
            Connection_Handle_Disp( event->param.hghd_cfg_indntf_ind.conhdl );
            printf( "inst_idx          = %d\n", event->param.hghd_cfg_indntf_ind.inst_idx );
            printf( "char_code         = %d\n", event->param.hghd_cfg_indntf_ind.char_code );
            printf( "cfg_val           = %d\n", event->param.hghd_cfg_indntf_ind.cfg_val );
            break;
        case RBLE_HGP_EVENT_HDEVICE_REPORT_IND:
            HGP_Event_Disp("HDEVICE_REPORT_IND");
            Connection_Handle_Disp( event->param.report_chg_ind.conhdl );
            printf( "inst_idx          = %d\n", event->param.report_chg_ind.inst_idx );
            printf( "device_type       = %d\n", event->param.report_chg_ind.report.device_type );
            printf( "report_type       = %d\n", event->param.report_chg_ind.report.report_type );
            printf( "value_size        = %d\n", event->param.report_chg_ind.report.value_size );
            printf( "value[ %d ] =\n"         , event->param.report_chg_ind.report.value_size );
            for(i=0;i<event->param.report_chg_ind.report.value_size;i++){
                printf( " 0x%02X", event->param.report_chg_ind.report.value[i] );
            }
            printf( "\n" );
            break;
        case RBLE_HGP_EVENT_HDEVICE_PROTOCOL_MODE_CHG_EVT:
            HGP_Event_Disp("HDEVICE_PROTOCOL_MODE_CHG_EVT");
            Connection_Handle_Disp( event->param.protocol_mode_chg_evt.conhdl );
            printf( "inst_idx          = %d\n", event->param.protocol_mode_chg_evt.inst_idx );
            printf( "protocol_mode_val = %d\n", event->param.protocol_mode_chg_evt.protocol_mode_val );
            break;
        case RBLE_HGP_EVENT_HDEVICE_REPORT_EVT:
            HGP_Event_Disp("HDEVICE_REPORT_EVT");
            Connection_Handle_Disp( event->param.report_chg_evt.conhdl );
            printf( "inst_idx          = %d\n", event->param.report_chg_evt.inst_idx );
            printf( "device_type       = %d\n", event->param.report_chg_evt.report.device_type );
            printf( "report_type       = %d\n", event->param.report_chg_evt.report.report_type );
            printf( "value_size        = %d\n", event->param.report_chg_evt.report.value_size );
            printf( "value[ %d ] =\n"         , event->param.report_chg_evt.report.value_size );
            for(i=0;i<event->param.report_chg_evt.report.value_size;i++){
                printf( " 0x%02X", event->param.report_chg_evt.report.value[i] );
            }
            printf( "\n" );
            break;
        case RBLE_HGP_EVENT_HDEVICE_HID_CP_CHG_EVT:
            HGP_Event_Disp("HDEVICE_HID_CP_CHG_EVT");
            Connection_Handle_Disp( event->param.hid_cp_chg_evt.conhdl );
            printf( "inst_idx          = %d\n", event->param.hid_cp_chg_evt.inst_idx );
            printf( "control_point_val = %d\n", event->param.hid_cp_chg_evt.control_point_val );
            break;
        case RBLE_HGP_EVENT_HDEVICE_REPORT_COMP:
            HGP_Event_Status_Disp("HDEVICE_REPORT_COMP", event->param.send_report.status);
            Connection_Handle_Disp( event->param.send_report.conhdl );
            break;
        case RBLE_HGP_EVENT_HDEVICE_SEND_BATTERY_LEVEL_COMP:
            HGP_Event_Status_Disp("HDEVICE_SEND_BATTERY_LEVEL_COMP", event->param.send_battery_level.status);
            Connection_Handle_Disp( event->param.send_battery_level.conhdl );
            break;
        case RBLE_HGP_EVENT_HDEVICE_COMMAND_DISALLOWED_IND:
            HGP_Event_Status_Disp("HDEVICE_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A HGP_HDevice_Enable command is executed.
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
static BOOL RBLE_HGP_HDevice_Enable_Test( void )
{
    RBLE_STATUS  Ret_Status;
    uint8_t     select = 0;
    /* security level */
    uint8_t sec_lvl[] = {
        RBLE_SVC_SEC_NONE,
        RBLE_SVC_SEC_UNAUTH,
        RBLE_SVC_SEC_UNAUTH|RBLE_SVC_SEC_ENC,
    };
    /* ConType */
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;

    RBLE_HGP_DEVICE_PARAM Param = {
        RBLE_HIDS_INST_MAX,         /* Number of HID Service Instance */
        RBLE_BAS_INST_MAX,          /* Number of Battery Service Instance */
        {
            RBLE_PRF_STOP_NTFIND,               /* Input Report notification configuration */
            #if (RBLE_HIDS_INST_MAX == 2)
            RBLE_PRF_STOP_NTFIND,
            #endif
        },
        {
            RBLE_PRF_STOP_NTFIND,               /* Boot Keyboard Input Report notification configuration */
            #if (RBLE_HIDS_INST_MAX == 2)
            RBLE_PRF_STOP_NTFIND,
            #endif
        },
        {
            RBLE_PRF_STOP_NTFIND,               /* Boot Mouse Input Report notification configuration */
            #if (RBLE_HIDS_INST_MAX == 2)
            RBLE_PRF_STOP_NTFIND,
            #endif
        },
        {
            0,                                  /* Protorol Mode */
            #if (RBLE_HIDS_INST_MAX == 2)
            0,
            #endif
        },
        #if ((RBLE_HIDS_INST_MAX % 2) != 0)
        0,                                      /* Reserved */
        #endif
        {
            RBLE_PRF_STOP_NTFIND,               /* Battery Level notification configuration */
            #if (RBLE_BAS_INST_MAX == 2)
            RBLE_PRF_STOP_NTFIND,
            #endif
        }
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
        Ret_Status = RBLE_HGP_HDevice_Enable( Connection_Handle, sec_lvl[select], con_type, &Param, RBLE_APP_HGHD_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_HDevice_Disable command is executed.
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
static BOOL RBLE_HGP_HDevice_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HGP_HDevice_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_HDevice_Send_Report command is executed.
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
static BOOL RBLE_HGP_HDevice_Send_Report_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    RBLE_HGP_REPORT_DESC report = {
        RBLE_HGHD_HID_DEVICE,                               /* Device type */
        RBLE_HGHD_INPUT_REPORT,                             /* Report type */
        { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 }, /* Report values */
        8                                                   /* Report size */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HGP_HDevice_Send_Report( Connection_Handle, inst_idx, &report );
        printf( " inst = %d, Dev type = %d, Report type = %d\n", inst_idx, report.device_type, report.report_type );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_HDevice_Send_Battery_Level command is executed.
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
static BOOL RBLE_HGP_HDevice_Send_Battery_Level_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint8_t         inst_idx = 0;
    uint8_t         battery_level = 100;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HGP_HDevice_Send_Battery_Level( Connection_Handle, inst_idx, battery_level);
        printf( " Battery Level = %d\n", battery_level );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_HGHD)


#if (PRF_SEL_HGBH)
/******************************************************************************************/
/**
 *  @brief HID over GATT(HGBH) Profile Call Back
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
static void RBLE_APP_HGBH_CallBack( RBLE_HGBH_EVENT *event )
{
    uint16_t i;
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_HGP_EVENT_BHOST_ENABLE_COMP:
            HGP_Event_Status_Disp("BHOST_ENABLE_COMP", event->param.bhost_enable.status);
            printf( "ConHdl=%d, HIDS Inst=%d, BAS Inst=%d\n", event->param.bhost_enable.conhdl,
                                                              event->param.bhost_enable.hids_inst_num,
                                                              event->param.bhost_enable.bas_inst_num );
            HGP_Info_Disp(  event->param.bhost_enable.hids,
                            &event->param.bhost_enable.dis,
                            event->param.bhost_enable.bas,
                            event->param.bhost_enable.hids_inst_num,
                            event->param.bhost_enable.bas_inst_num );
            g_hids[0] = event->param.bhost_enable.hids[0];
            g_hogp_dis = event->param.bhost_enable.dis;
            g_hogp_bas[0] = event->param.bhost_enable.bas[0];
            #if (RBLE_HIDS_INST_MAX == 2)
            if (2 == event->param.bhost_enable.hids_inst_num) {
                g_hids[1] = event->param.bhost_enable.hids[1];
            }
            if (2 == event->param.bhost_enable.bas_inst_num) {
                g_hogp_bas[1] = event->param.bhost_enable.bas[1];
            }
            #endif
            break;
        case RBLE_HGP_EVENT_BHOST_DISABLE_COMP:
            HGP_Event_Status_Disp("BHOST_DISABLE_COMP", event->param.bhost_disable.status);
            Connection_Handle_Disp( event->param.bhost_disable.conhdl );
            break;
        case RBLE_HGP_EVENT_BHOST_ERROR_IND:
            HGP_Event_Status_Disp("BHOST_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_HGP_EVENT_BHOST_READ_CHAR_RESPONSE:
            HGP_Event_Disp("BHOST_READ_CHAR_RESPONSE");
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            printf( "att_code          = %d\n", event->param.rd_char_resp.att_code );
            printf( "each_len          = %d\n", event->param.rd_char_resp.data.each_len );
            printf( "len               = %d\n", event->param.rd_char_resp.data.len );
            printf( "data[ %d ] =\n"          , event->param.rd_char_resp.data.len );
            for(i=0;i<event->param.rd_char_resp.data.len;i++){
                printf( " 0x%02X", event->param.rd_char_resp.data.data[i] );
            }
            printf( "\n" );
            break;
        case RBLE_HGP_EVENT_BHOST_WRITE_CHAR_RESPONSE:
            HGP_Event_Disp("BHOST_WRITE_CHAR_RESPONSE");
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            printf( "att_code          = %d\n", event->param.wr_char_resp.att_code );
            break;
        case RBLE_HGP_EVENT_BHOST_REPORT_NTF:
            HGP_Event_Disp("BHOST_REPORT_NTF");
            Connection_Handle_Disp( event->param.report_ntf.conhdl );
            printf( "inst_idx          = %d\n", event->param.report_ntf.inst_idx );
            printf( "device_type       = %d\n", event->param.report_ntf.report.device_type );
            printf( "report_type       = %d\n", event->param.report_ntf.report.report_type );
            printf( "value_size        = %d\n", event->param.report_ntf.report.value_size );
            printf( "value[ %d ] =\n"         , event->param.report_ntf.report.value_size );
            for(i=0;i<event->param.report_ntf.report.value_size;i++){
                printf( " 0x%02X", event->param.report_ntf.report.value[i] );
            }
            printf( "\n" );
            break;
        case RBLE_HGP_EVENT_BHOST_COMMAND_DISALLOWED_IND:
            HGP_Event_Status_Disp("BHOST_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A HGP_BHost_Enable command is executed.
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
static BOOL RBLE_HGP_BHost_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;
    uint8_t hids_inst_num = RBLE_HIDS_INST_MAX;
    uint8_t bas_inst_num  = RBLE_BAS_INST_MAX;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_hids[0].shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_HGP_BHost_Enable( Connection_Handle, con_type, hids_inst_num, bas_inst_num, &g_hids[0], &g_hogp_dis, &g_hogp_bas[0], RBLE_APP_HGBH_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_BHost_Disable command is executed.
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
static BOOL RBLE_HGP_BHost_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HGP_BHost_Disable( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_BHost_Read_Char command is executed.
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
static BOOL RBLE_HGP_BHost_Read_Char_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;

    uint8_t     inst_idx = 1;
    uint8_t     char_code = RBLE_HGBH_RD_HIDS_PM;           /*= 0x00*//**< Read HIDS Protcol Mode Characteristic */

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if (( 2 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 1 ] < RBLE_HIDS_INST_MAX)) {
                inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
            }
        } else {
            Parameter_No = 0;
        }
        switch( Parameter_No ){
        case RBLE_HGBH_RD_HIDS_PM:           /*= 0x00*//**< Read HIDS Protcol Mode Characteristic */
        default:
            Parameter_No = 0;
            break;
        case RBLE_HGBH_RD_HIDS_KI:           /*= 0x01*//**< Read HIDS Boot Keyboard Input Report Characteristic */
        case RBLE_HGBH_RD_HIDS_KI_CFG:       /*= 0x02*//**< Read HIDS Boot Keyboard Input Report Client Cfg. Desc */
        case RBLE_HGBH_RD_HIDS_KO:           /*= 0x03*//**< Read HIDS Boot Keyboard Output Report Characteristic */
        case RBLE_HGBH_RD_HIDS_MI:           /*= 0x04*//**< Read HIDS Boot Mouse Input Report Characteristic */
        case RBLE_HGBH_RD_HIDS_MI_CFG:       /*= 0x05*//**< Read HIDS Boot Mouse Input Report Client Cfg. Desc */
        case RBLE_HGBH_RD_DIS_PNPID:         /*= 0x06*//**< Read DIS PnP ID Characteristic */
        case RBLE_HGBH_RD_BAS_BL:            /*= 0x07*//**< Read BAS Battery Level Characteristic */
            break;
        }

        /* API Call */
        char_code = (uint8_t)Parameter_No;
        Ret_Status = RBLE_HGP_BHost_Read_Char( Connection_Handle, inst_idx, char_code );
        printf( " inst = %d, char code = %d\n", inst_idx, char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_BHost_Read_By_UUID_Char command is executed.
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
static BOOL RBLE_HGP_BHost_Read_By_UUID_Char_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;

    uint8_t     inst_idx = 0;
    uint8_t     char_code = RBLE_HGBH_RD_HIDS_PM;           /*= 0x00*//**< Read HIDS Protcol Mode Characteristic */

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if (( 2 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 1 ] < RBLE_HIDS_INST_MAX)) {
                inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
            }
        } else {
            Parameter_No = 0;
        }
        switch( Parameter_No ){
        case RBLE_HGBH_RD_HIDS_PM:           /*= 0x00*//**< Read HIDS Protcol Mode Characteristic */
        default:
            Parameter_No = 0;
            break;
        case RBLE_HGBH_RD_HIDS_KI:           /*= 0x01*//**< Read HIDS Boot Keyboard Input Report Characteristic */
        case RBLE_HGBH_RD_HIDS_KO:           /*= 0x03*//**< Read HIDS Boot Keyboard Output Report Characteristic */
        case RBLE_HGBH_RD_HIDS_MI:           /*= 0x04*//**< Read HIDS Boot Mouse Input Report Characteristic */
        case RBLE_HGBH_RD_DIS_PNPID:         /*= 0x06*//**< Read DIS PnP ID Characteristic */
        case RBLE_HGBH_RD_BAS_BL:            /*= 0x07*//**< Read BAS Battery Level Characteristic */
            break;
        }

        /* API Call */
        char_code = (uint8_t)Parameter_No;
        Ret_Status = RBLE_HGP_BHost_Read_By_UUID_Char( Connection_Handle, inst_idx, char_code );
        printf( " inst = %d, char code = %d\n", inst_idx, char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_BHost_Write_Char command is executed.
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
static BOOL RBLE_HGP_BHost_Write_Char_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    uint8_t     char_code = RBLE_HGHD_KB_REPORT_CODE;
    uint16_t    cfg_val = RBLE_PRF_START_NTF;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 1 <= RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if ( 2 <= RBLE_Parameter_Cnt ) {
                if ( RBLE_Parameter_Arg[1] == 0) {
                    cfg_val = RBLE_PRF_STOP_NTFIND;
                }
                else {
                    cfg_val = RBLE_PRF_START_NTF;
                }
                if (( 3 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 2 ] < RBLE_HIDS_INST_MAX)) {
                    inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 2 ];
                }
            }
        } else {
            Parameter_No = 0;
        }
        switch( Parameter_No ){
        case 0:
        case 1:
        default:
            char_code = RBLE_HGHD_REPORT_INPUT_CODE;
            break;
        case 2:
            char_code = RBLE_HGHD_KB_REPORT_CODE;
            break;
        case 3:
            char_code = RBLE_HGHD_MO_REPORT_CODE;
            break;
        }

        /* API Call */
        Ret_Status = RBLE_HGP_BHost_Write_Char( Connection_Handle, inst_idx, char_code, cfg_val );
        printf( " inst = %d, char_code = %d, cfg_val = %d\n", inst_idx, char_code, cfg_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_BHost_Set_Report command is executed.
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
static BOOL RBLE_HGP_BHost_Set_Report_Test( void )
{
    RBLE_STATUS Ret_Status;
    int32_t     Parameter_No;
    uint8_t     inst_idx = 0;
    RBLE_HGP_REPORT_DESC report = {
        RBLE_HGHD_BOOT_KEYBOARD,                            /* Device type */
        RBLE_HGHD_INPUT_REPORT,                             /* Report type */
        { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 }, /* Report values */
        8                                                   /* Report size */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if (( 2 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 1 ] < RBLE_HIDS_INST_MAX)) {
                inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
            }
        } else {
            Parameter_No = 0;
        }
        switch( Parameter_No ){
        case 0:
        case 2:
        default:
            report.device_type = RBLE_HGHD_BOOT_KEYBOARD;
            break;
        case 1:
            report.device_type = RBLE_HGHD_HID_DEVICE;
            break;
        case 3:
            report.device_type = RBLE_HGHD_BOOT_MOUSE;
            break;
        }

        /* API Call */
        Ret_Status = RBLE_HGP_BHost_Set_Report( Connection_Handle, inst_idx, &report );
        printf( " inst = %d, Dev type = %d, Report type = %d\n", inst_idx, report.device_type, report.report_type );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_BHost_Write_Protocol_Mode command is executed.
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
static BOOL RBLE_HGP_BHost_Write_Protocol_Mode_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    uint8_t     protocol_mode_val = RBLE_HGHD_PROTOCOL_MODE_BOOT;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if (( 1 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 0 ] < RBLE_HIDS_INST_MAX)) {
            inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_HGP_BHost_Write_Protocol_Mode( Connection_Handle, inst_idx, protocol_mode_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_BHost_Data_Output command is executed.
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
static BOOL RBLE_HGP_BHost_Data_Output_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    RBLE_HGP_REPORT_DESC report = {
        RBLE_HGHD_BOOT_KEYBOARD,                            /* Device type */
        RBLE_HGHD_OUTPUT_REPORT,                            /* Report type */
        { 0x10, 0x11, 0x12, 0x13, 0x14, 0x15, 0x16, 0x17 }, /* Report values */
        8                                                   /* Report size */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if (( 1 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 0 ] < RBLE_HIDS_INST_MAX)) {
            inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 0 ];
        }

        /* API Call */
        Ret_Status = RBLE_HGP_BHost_Data_Output( Connection_Handle, inst_idx, &report );
        printf( " inst = %d, Dev type = %d, Report type = %d\n", inst_idx, report.device_type, report.report_type );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_HGBH)


#if (PRF_SEL_HGRH)
/******************************************************************************************/
/**
 *  @brief HID over GATT(HGRH) Profile Call Back
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
static void RBLE_APP_HGRH_CallBack( RBLE_HGRH_EVENT *event )
{
    uint16_t i;
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_HGP_EVENT_RHOST_ENABLE_COMP:
            HGP_Event_Status_Disp("RHOST_ENABLE_COMP", event->param.rhost_enable.status);
            printf( "ConHdl=%d, HIDS Inst=%d, BAS Inst=%d\n", event->param.rhost_enable.conhdl,
                                                              event->param.rhost_enable.hids_inst_num,
                                                              event->param.rhost_enable.bas_inst_num );
            HGP_Info_Disp(  event->param.rhost_enable.hids,
                            &event->param.rhost_enable.dis,
                            event->param.rhost_enable.bas,
                            event->param.rhost_enable.hids_inst_num,
                            event->param.rhost_enable.bas_inst_num );

            g_hids[0] = event->param.rhost_enable.hids[0];
            g_hogp_dis = event->param.rhost_enable.dis;
            g_hogp_bas[0] = event->param.rhost_enable.bas[0];
            #if (RBLE_HIDS_INST_MAX == 2)
            if (2 == event->param.rhost_enable.hids_inst_num) {
                g_hids[1] = event->param.rhost_enable.hids[1];
            }
            if (2 == event->param.rhost_enable.bas_inst_num) {
                g_hogp_bas[1] = event->param.rhost_enable.bas[1];
            }
            #endif
            break;
        case RBLE_HGP_EVENT_RHOST_DISABLE_COMP:
            HGP_Event_Status_Disp("RHOST_DISABLE_COMP", event->param.rhost_disable.status);
            Connection_Handle_Disp( event->param.rhost_disable.conhdl );
            break;
        case RBLE_HGP_EVENT_RHOST_ERROR_IND:
            HGP_Event_Status_Disp("RHOST_ERROR_IND", event->param.error_ind.status);
            Connection_Handle_Disp( event->param.error_ind.conhdl );
            break;
        case RBLE_HGP_EVENT_RHOST_READ_CHAR_RESPONSE:
            HGP_Event_Disp("RHOST_READ_CHAR_RESPONSE");
            Connection_Handle_Disp( event->param.rd_char_resp.conhdl );
            printf( "att_code          = %d\n", event->param.rd_char_resp.att_code );
            printf( "each_len          = %d\n", event->param.rd_char_resp.data.each_len );
            printf( "len               = %d\n", event->param.rd_char_resp.data.len );
            printf( "data[ %d ] =\n"          , event->param.rd_char_resp.data.len );
            for(i=0;i<event->param.rd_char_resp.data.len;i++){
                printf( " 0x%02X", event->param.rd_char_resp.data.data[i] );
            }
            printf( "\n" );
            break;
        case RBLE_HGP_EVENT_RHOST_READ_LONG_CHAR_RESPONSE:
            HGP_Event_Disp("RHOST_READ_LONG_CHAR_RESPONSE");
            Connection_Handle_Disp( event->param.rd_long_char_resp.conhdl );
            printf( "att_code          = %d\n", event->param.rd_long_char_resp.att_code );
            printf( "val_len           = %d\n", event->param.rd_long_char_resp.data.val_len );
            printf( "attr_hdl          = %d\n", event->param.rd_long_char_resp.data.attr_hdl );
            printf( "value[ %d ] =\n"         , event->param.rd_long_char_resp.data.val_len );
            for(i=0;i<event->param.rd_long_char_resp.data.val_len;i++){
                printf( " 0x%02X", event->param.rd_long_char_resp.data.value[i] );
            }
            printf( "\n" );
            break;
        case RBLE_HGP_EVENT_RHOST_WRITE_CHAR_RESPONSE:
            HGP_Event_Disp("RHOST_WRITE_CHAR_RESPONSE");
            Connection_Handle_Disp( event->param.wr_char_resp.conhdl );
            printf( "att_code          = %d\n", event->param.wr_char_resp.att_code );
            break;
        case RBLE_HGP_EVENT_RHOST_REPORT_NTF:
            HGP_Event_Disp("RHOST_REPORT_NTF");
            Connection_Handle_Disp( event->param.report_ntf.conhdl );

            printf( "inst_idx          = %d\n", event->param.report_ntf.inst_idx );
            printf( "device_type       = %d\n", event->param.report_ntf.report.device_type );
            printf( "report_type       = %d\n", event->param.report_ntf.report.report_type );
            printf( "value_size        = %d\n", event->param.report_ntf.report.value_size );
            printf( "value[ %d ] =\n"         , event->param.report_ntf.report.value_size );
            for(i=0;i<event->param.report_ntf.report.value_size;i++){
                printf( " 0x%02X", event->param.report_ntf.report.value[i] );
            }
            printf( "\n" );
            break;
        case RBLE_HGP_EVENT_RHOST_BATTERY_LEVEL_NTF:
            HGP_Event_Disp("RHOST_BATTERY_LEVEL_NTF");
            Connection_Handle_Disp( event->param.battery_level_ntf.conhdl );
            printf( "inst_idx          = %d\n", event->param.battery_level_ntf.inst_idx );
            printf( "battery_level     = %d\n", event->param.battery_level_ntf.battery_level );
            break;
        case RBLE_HGP_EVENT_RHOST_COMMAND_DISALLOWED_IND:
            HGP_Event_Status_Disp("RHOST_COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Enable command is executed.
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
static BOOL RBLE_HGP_RHost_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t con_type = RBLE_PRF_CON_DISCOVERY;
    uint8_t hids_inst_num = RBLE_HIDS_INST_MAX;
    uint8_t bas_inst_num  = RBLE_BAS_INST_MAX;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0x0000 != g_hids[0].shdl ) {
            con_type = RBLE_PRF_CON_NORMAL;
        }
        /* API Call */
        Ret_Status = RBLE_HGP_RHost_Enable( Connection_Handle, con_type, hids_inst_num, bas_inst_num, &g_hids[0], &g_hogp_dis, &g_hogp_bas[0], RBLE_APP_HGRH_CallBack );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Disable command is executed.
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
static BOOL RBLE_HGP_RHost_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_HGP_RHost_Disable( Connection_Handle);

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Read_Char command is executed.
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
static BOOL RBLE_HGP_RHost_Read_Char_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    uint8_t     char_code = RBLE_HGRH_RD_HIDS_PM;            /* Read HID Service Protcol Mode Characteristic */

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if (( 2 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 1 ] < RBLE_HIDS_INST_MAX)) {
                inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
            }
        } else {
            Parameter_No = 0;
        }
        switch( Parameter_No ){
        case RBLE_HGRH_RD_HIDS_PM:           /*= 0x00*//**< Read HIDS Protcol Mode Characteristic */
        default:
            Parameter_No = 0;
            break;
        case RBLE_HGRH_RD_HIDS_RI:            /*=0x01*//**< Read HID Service Report(Input) Characteristic */
        case RBLE_HGRH_RD_HIDS_RI_CFG:        /*=0x02*//**< Read HID Service Report(Input) Client Cfg. Desc */
        case RBLE_HGRH_RD_HIDS_RI_REF:        /*=0x03*//**< Read HID Service Report(Input) Report Reference Desc */
        case RBLE_HGRH_RD_HIDS_RO:            /*=0x04*//**< Read HID Service Report(Output) Characteristic */
        case RBLE_HGRH_RD_HIDS_RO_REF:        /*=0x05*//**< Read HID Service Report(Output) Report Reference Desc */
        case RBLE_HGRH_RD_HIDS_RF:            /*=0x06*//**< Read HID Service Report(Feature) Characteristic */
        case RBLE_HGRH_RD_HIDS_RF_REF:        /*=0x07*//**< Read HID Service Report(Feature) Report Reference Desc */
        case RBLE_HGRH_RD_HIDS_RM:            /*=0x08*//**< Read HID Service Report MAP Characteristic */
        case RBLE_HGRH_RD_HIDS_ER_REF:        /*=0x09*//**< Read HID Service Report MAP Rxternal Report Reference Desc */
        case RBLE_HGRH_RD_HIDS_HI:            /*=0x0A*//**< Read HID Service HID Information Characteristic */
        case RBLE_HGRH_RD_DIS_PNPID:          /*=0x0B*//**< Read Device Information Service PnP ID Characteristic */
        case RBLE_HGRH_RD_BAS_BL:             /*=0x0C*//**< Read Battery Service Battery Level Characteristic */
        case RBLE_HGRH_RD_BAS_BL_CFG:         /*=0x0D*//**< Read Battery Service Battery Level Client Cfg. Desc */
        case RBLE_HGRH_RD_BAS_BL_REF:         /*=0x0E*//**< Read Battery Service Battery Level Report Reference Desc */
            break;
        }

        /* API Call */
        char_code = (uint8_t)Parameter_No;
        Ret_Status = RBLE_HGP_RHost_Read_Char( Connection_Handle, inst_idx, char_code);

        printf( " inst = %d, char code = %d\n", inst_idx, char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Read_By_UUID_Char command is executed.
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
static BOOL RBLE_HGP_RHost_Read_By_UUID_Char_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    uint8_t     char_code = RBLE_HGRH_RD_BAS_BL;             /*=0x0C*//**< Read Battery Service Battery Level Characteristic */

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if (( 2 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 1 ] < RBLE_HIDS_INST_MAX)) {
                inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
            }
        } else {
            Parameter_No = ILLEGAL_NO;
        }
        switch( Parameter_No ){
        case RBLE_HGRH_RD_BAS_BL:             /*=0x0C*//**< Read Battery Service Battery Level Characteristic */
        default:
            Parameter_No = RBLE_HGRH_RD_BAS_BL;             /*=0x0C*//**< Read Battery Service Battery Level Characteristic */
            break;
        case RBLE_HGRH_RD_DIS_PNPID:          /*=0x0B*//**< Read Device Information Service PnP ID Characteristic */
            break;
        }

        /* API Call */
        char_code = (uint8_t)Parameter_No;
        Ret_Status = RBLE_HGP_RHost_Read_By_UUID_Char( Connection_Handle, inst_idx, char_code );
        printf( " inst = %d, char code = %d\n", inst_idx, char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Read_Long_Char command is executed.
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
static BOOL RBLE_HGP_RHost_Read_Long_Char_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    uint8_t     char_code = RBLE_HGRH_RD_HIDS_RM;            /*=0x08*//**< Read HID Service Report MAP Characteristic */

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if (( 2 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 1 ] < RBLE_HIDS_INST_MAX)) {
                inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
            }
        } else {
            Parameter_No = ILLEGAL_NO;
        }
        switch( Parameter_No ){
        case RBLE_HGRH_RD_HIDS_RM:            /*=0x08*//**< Read HID Service Report MAP Characteristic */
        case ILLEGAL_NO:
        default:
            Parameter_No = RBLE_HGRH_RD_HIDS_RM;            /*=0x08*//**< Read HID Service Report MAP Characteristic */
            break;
        case RBLE_HGRH_RD_HIDS_RI:            /*=0x01*//**< Read HID Service Report(Input) Characteristic */
        case RBLE_HGRH_RD_HIDS_RO:            /*=0x04*//**< Read HID Service Report(Output) Characteristic */
        case RBLE_HGRH_RD_HIDS_RF:            /*=0x06*//**< Read HID Service Report(Feature) Characteristic */
            break;
        }

        /* API Call */
        char_code = (uint8_t)Parameter_No;
        Ret_Status = RBLE_HGP_RHost_Read_Long_Char( Connection_Handle, inst_idx, char_code );
        printf( " inst = %d, char code = %d\n", inst_idx, char_code );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Write_Char command is executed.
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
static BOOL RBLE_HGP_RHost_Write_Char_Test( void )
{
    int32_t     Parameter_No;
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    uint8_t     char_code = RBLE_HGHD_REPORT_INPUT_CODE;
    uint16_t    cfg_val = RBLE_PRF_START_NTF;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if ( 2 <= RBLE_Parameter_Cnt ) {
                if ( RBLE_Parameter_Arg[1] == 0) {
                    cfg_val = RBLE_PRF_STOP_NTFIND;
                }
                else {
                    cfg_val = RBLE_PRF_START_NTF;
                }
                if (( 3 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 2 ] < RBLE_HIDS_INST_MAX)) {
                    inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 2 ];
                }
            }
        } else {
            Parameter_No = ILLEGAL_NO;
        }
        switch( Parameter_No ){
        case RBLE_HGHD_REPORT_INPUT_CODE:
        case ILLEGAL_NO:
        default:
            Parameter_No = RBLE_HGHD_REPORT_INPUT_CODE;
            break;
        case RBLE_HGHD_BATTERY_LEVEL_CODE:
            break;
        }

        /* API Call */
        char_code = (uint8_t)Parameter_No;
        Ret_Status = RBLE_HGP_RHost_Write_Char( Connection_Handle, inst_idx, char_code, cfg_val);
        printf( " inst = %d, char_code = %d, cfg_val = %d\n", inst_idx, char_code, cfg_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Set_Report command is executed.
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
static BOOL RBLE_HGP_RHost_Set_Report_Test( void )
{
    RBLE_STATUS Ret_Status;
    int32_t     Parameter_No;
    uint8_t     inst_idx = 0;
    RBLE_HGP_REPORT_DESC report = {
        RBLE_HGHD_HID_DEVICE,                       /* Device type */
        RBLE_HGHD_INPUT_REPORT,                     /* Report type */
        {0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00}, /* Report values */
        16                                          /* Report size */
    };

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            Parameter_No = RBLE_Parameter_Arg[ 0 ];
            if (( 2 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 1 ] < RBLE_HIDS_INST_MAX)) {
                inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
            }
        } else {
            Parameter_No = ILLEGAL_NO;
        }

        switch( Parameter_No ){
        case 0:
        case 1:
        default:
            report.report_type = RBLE_HGHD_INPUT_REPORT;
            break;
        case 2:
            report.report_type = RBLE_HGHD_OUTPUT_REPORT;
            break;
        case 3:
            report.report_type = RBLE_HGHD_FEATURE_REPORT;
            break;
        }

        /* API Call */
        Ret_Status = RBLE_HGP_RHost_Set_Report( Connection_Handle, inst_idx, &report );
        printf( " inst = %d, Dev type = %d, Report type = %d\n", inst_idx, report.device_type, report.report_type );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Write_Protocol_Mode command is executed.
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
static BOOL RBLE_HGP_RHost_Write_Protocol_Mode_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    uint8_t     protocol_mode_val = RBLE_HGHD_PROTOCOL_MODE_REPORT;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if (( 1 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 0 ] < RBLE_HIDS_INST_MAX)) {
            inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 0 ];
        }
        /* API Call */
        Ret_Status = RBLE_HGP_RHost_Write_Protocol_Mode( Connection_Handle, inst_idx, protocol_mode_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Data_Output command is executed.
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
static BOOL RBLE_HGP_RHost_Data_Output_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    RBLE_HGP_REPORT_DESC report = {
        RBLE_HGHD_HID_DEVICE,                       /* Device type */
        RBLE_HGHD_OUTPUT_REPORT,                    /* Report type */
        {0x0F, 0x0E, 0x0D, 0x0C, 0x0B, 0x0A, 0x09, 0x08, 0x07, 0x06, 0x05, 0x04, 0x03, 0x02, 0x01, 0x00}, /* Report values */
        16                                          /* Report size */
    };


    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if (( 1 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 0 ] < RBLE_HIDS_INST_MAX)) {
            inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 0 ];
        }

        /* API Call */
        Ret_Status = RBLE_HGP_RHost_Data_Output( Connection_Handle, inst_idx, &report );
        printf( " inst = %d, Dev type = %d, Report type = %d\n", inst_idx, report.device_type, report.report_type );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A HGP_RHost_Write_Control_Point command is executed.
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
static BOOL RBLE_HGP_RHost_Write_Control_Point_Test( void )
{
    RBLE_STATUS Ret_Status;
    uint8_t     inst_idx = 0;
    uint8_t     control_point_val = RBLE_HGHD_CTRL_POINT_SUSPEND;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        if ( 0 != RBLE_Parameter_Cnt ) {
            if ( RBLE_Parameter_Arg[ 0 ] == RBLE_HGHD_POINT_EXIT_SUSPEND ) {
                control_point_val = RBLE_HGHD_POINT_EXIT_SUSPEND;
            } else {
                control_point_val = RBLE_HGHD_CTRL_POINT_SUSPEND;
            }
            if (( 2 <= RBLE_Parameter_Cnt ) && (RBLE_Parameter_Arg[ 1 ] < RBLE_HIDS_INST_MAX)) {
                inst_idx = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
            }
        }

        /* API Call */
        Ret_Status = RBLE_HGP_RHost_Write_Control_Point( Connection_Handle, inst_idx, control_point_val );
        printf( " inst = %d, CP val = 0x%02X\n", inst_idx, control_point_val );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}
#endif  //#if (PRF_SEL_HGRH)


#if (PRF_SEL_HGBH || PRF_SEL_HGRH)
/******************************************************************************************/
/**
 *  @brief Display HGP CONTENT Info
 *
 *  @b Contents of management:
 *
 *  @param[in] 
 *
 *  @retval none
 */
/******************************************************************************************/
static void HGP_Info_Disp( RBLE_HIDS_CONTENT *hids, RBLE_DIS11_CONTENT *dis, RBLE_BAS_CONTENT *bas, uint8_t hids_inst_num, uint8_t bas_inst_num )
{
    uint8_t i;

    for(i=0 ; i<hids_inst_num ; i++){
        printf( "* HIDS(Inst:%d), Start=%X, End=%X \n", i, hids->shdl, hids->ehdl );
        printf( " Prtcl Md AttHdl=%X, Prop=%X, Hdl=%X\n", hids->protocol_md_char_hdl, hids->protocol_md_prop, hids->protocol_md_val_hdl );
        printf( " Rep.In AttHdl=%X, Prop=%X, Hdl=%X, Cfg=%X, Ref=%X\n", hids->report_input_char_hdl, hids->report_input_prop, hids->report_input_val_hdl,
                                                                          hids->report_input_cfg_hdl, hids->input_rep_ref_hdl);
        printf( " Rep.Out AttHdl=%X, Prop=%X, Hdl=%X, Ref=%X\n", hids->report_output_char_hdl, hids->report_output_prop, hids->report_output_val_hdl,
                                                                   hids->output_rep_ref_hdl );
        printf( " Rep.Feat AttHdl=%X, Prop=%X, Hdl=%X, Ref=%X\n", hids->report_feature_char_hdl, hids->report_feature_prop, hids->report_feature_val_hdl,
                                                                  hids->feature_rep_ref_hdl );
        printf( " Rep.Map AttHdl=%X, Prop=%X, Hdl=%X, Ref=%X\n", hids->report_map_char_hdl, hids->report_map_prop, hids->report_map_val_hdl,
                                                                 hids->external_rep_ref_hdl );
        printf( " BKB In AttHdl=%X, Prop=%X, Hdl=%X, Cfg=%X\n", hids->bootkb_input_char_hdl, hids->bootkb_input_prop, hids->bootkb_input_val_hdl,
                                                                     hids->bootkb_input_cfg_hdl );
        printf( " BKB Out AttHdl=%X, Prop=%X, Hdl=%X\n", hids->bootkb_output_char_hdl, hids->bootkb_output_prop, hids->bootkb_output_val_hdl);
        printf( " BMO In AttHdl=%X, Prop=%X, Hdl=%X, Cfg=%X\n", hids->bootmo_input_char_hdl, hids->bootmo_input_prop, hids->bootmo_input_val_hdl,
                                                                     hids->bootmo_input_cfg_hdl );
        printf( " HID Info AttHdl=%X, Prop=%X, Hdl=%X\n", hids->hid_info_char_hdl, hids->hid_info_prop, hids->hid_info_val_hdl );
        printf( " HID CtrlPt AttHdl=%X, Prop=%X, Hdl=%X\n", hids->hid_cp_char_hdl, hids->hid_cp_prop, hids->hid_cp_val_hdl );
        printf( " InclSvc AttHdl=%X, UUID=%X, Start=%X, End=%X\n", hids->include_svc_hdl, hids->include_svc_uuid, hids->incl_shdl, hids->incl_ehdl );
        hids++;     //Pointer Increase
    }

    printf( "* DIS: Start=%X, End=%X, PnP ID AttHdl=%X, Prop=%X, Hdl=%X\n",
                                                            dis->shdl, dis->ehdl, dis->pnp_id_char_hdl, dis->pnp_id_prop, dis->pnp_id_val_hdl);

    for(i=0 ; i<bas_inst_num ; i++){
        printf( "* BAS(Inst:%d), Start=%X, End=%X\n", i, bas->shdl, bas->ehdl );
        printf( " Batt.Lvl AttHdl=%X, Prop=%X, Hdl=%X, Cfg=%X, Ref=%X\n", bas->battery_lvl_char_hdl, bas->battery_lvl_prop,
                                                                         bas->battery_lvl_val_hdl, bas->battery_lvl_cfg_hdl,
                                                                         bas->battery_lvl_rep_ref_hdl);
        bas++;      //Pointer Increase
    }

}
#endif  //#if (PRF_SEL_HGBH || PRF_SEL_HGRH)

/******************************************************************************************/
/**
 *  @brief rBLE HOGP Event and Status Disp
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
static void HGP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE HGP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}

/******************************************************************************************/
/**
 *  @brief rBLE HOGP Event Disp
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
static void HGP_Event_Disp(char_t *event_str)
{
    printf( "\nrBLE HGP EVENT (%s)\n", event_str );
}

#endif  //#if (PRF_SEL_HGHD || PRF_SEL_HGBH || PRF_SEL_HGRH)
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
