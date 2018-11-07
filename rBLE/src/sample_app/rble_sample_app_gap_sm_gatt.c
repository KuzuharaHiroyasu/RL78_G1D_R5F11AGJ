/**
 ****************************************************************************************
 *
 * @file       rble_sample_app_gap.c
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

/**
 * DEFINES
 ****************************************************************************************
 */
#define SVC_UUID(uuid)          (uint8_t)((uuid)), (uint8_t)((uuid) >> 8)

#define SERVICE_LIST_LEN        ((USE_LINK_LOSS_SERVICE * 2)            \
                                 + (USE_TX_POWER_SERVICE * 2)           \
                                 + (USE_IMMEDIATE_ALERT_SERVICE * 2)    \
                                 + (PRF_SEL_HTPT * 2)                   \
                                 + (PRF_SEL_BLPS * 2)                   \
                                 + (USE_DEV_INFO_SERVICE * 2)           \
                                 + (PRF_SEL_HGHD * 2)                   \
                                 + (USE_SCAN_PARAMETER_SERVICE * 2)     \
                                 + (USE_BATTERY_SERVICE * 2)            \
                                 + (PRF_SEL_HRPS * 2)                   \
                                 + (PRF_SEL_CSCS * 2)                   \
                                 + (PRF_SEL_GLPS * 2)                   \
                                 + (PRF_SEL_CPPS * 2)                   \
                                 + (PRF_SEL_TIPS * 2)                   \
                                 + (PRF_SEL_ANPS * 2)                   \
                                 + (PRF_SEL_LNPS * 2)                   \
                                 + (PRF_SEL_PASS * 2)                   \
                                 + (PRF_SEL_RSCS * 2))


/**
 * TYPE DEFINES
 ****************************************************************************************
 */
enum RBLE_APP_KEY_FLAG_enum {
    RBLE_APP_LTK,
    RBLE_APP_ENCKEY,
    RBLE_APP_IDKEY,
    RBLE_APP_SIGNKEY
};

typedef struct {
    BOOL                            Key_Set_Flags;
    struct RBLE_EVT_SM_Key_t        EncKey;
    struct RBLE_EVT_SM_Key_t        IrKey;
    struct RBLE_EVT_SM_Key_t        SignKey;
} RBLE_APP_KEY_DATA;


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/* --------------------------- Generic Access Profile --------------------------------*/
static void RBLE_APP_GAP_CallBack( RBLE_GAP_EVENT *event );                                 /* GAP Call Back */
       BOOL RBLE_GAP_Reset_Test( void );                                                    /* A GAP_Reset command is executed. */
static BOOL RBLE_GAP_Set_Name_Test( void );                                                 /* A GAP_Set_Name command is executed. */
static BOOL RBLE_GAP_Observation_Enable_Test( void );                                       /* A GAP_Observation_Enable command is executed. */
static BOOL RBLE_GAP_Observation_Disable_Test( void );                                      /* A GAP_Observation_Disable command is executed. */
       BOOL RBLE_GAP_Broadcast_Enable_Test( void );                                         /* A GAP_Broadcast_Enable command is executed. */
static BOOL RBLE_GAP_Broadcast_Disable_Test( void );                                        /* A GAP_Broadcast_Disable command is executed. */
static BOOL RBLE_GAP_Set_Bonding_Mode_Test( void );                                         /* A GAP_Set_Bonding_Mode command is executed. */
static BOOL RBLE_GAP_Set_Security_Request_Test( void );                                     /* A GAP_Set_Security_Request command is executed. */
static BOOL RBLE_GAP_Get_Device_Info_Test( void );                                          /* A GAP_Get_Device_Info command is executed. */
static BOOL RBLE_GAP_Get_White_List_Size_Test( void );                                      /* A GAP_Get_White_List_Size command is executed. */
static BOOL RBLE_GAP_Add_To_White_List_Test( void );                                        /* A GAP_Add_To_White_List command is executed. */
static BOOL RBLE_GAP_Del_From_White_List_Test( void );                                      /* A GAP_Del_From_White_List command is executed. */
static BOOL RBLE_GAP_Get_Remote_Device_Name_Test( void );                                   /* A GAP_Get_Remote_Device_Name command is executed. */
static BOOL RBLE_GAP_Get_Remote_Device_Info_Test( void );                                   /* A GAP_Get_Remote_Device_Info command is executed. */
static BOOL RBLE_GAP_Device_Search_Test( void );                                            /* A GAP_Device_Search command is executed. */
static BOOL RBLE_GAP_Set_Random_Address_Test( void );                                       /* A GAP_Set_Random_Address command is executed. */
static BOOL RBLE_GAP_Set_Privacy_Feature_Test( void );                                      /* A GAP_Set_Privacy_Feature command is executed. */
static BOOL RBLE_GAP_Create_Connection_Test( void );                                        /* A GAP_Create_Connection command is executed. */
static BOOL RBLE_GAP_Connection_Cancel_Test( void );                                        /* A GAP_Connection_Cancel command is executed. */
static BOOL RBLE_GAP_Disconnect_Test( void );                                               /* A GAP_Disconnect command is executed. */
static BOOL RBLE_GAP_Start_Bonding_Test( void );                                            /* A GAP_Start_Bonding command is executed. */
static BOOL RBLE_GAP_Bonding_Info_Ind_Test( void );                                         /* A GAP_Bonding_Info_Ind command is executed. */
static BOOL RBLE_GAP_Bonding_Response_Test( void );                                         /* A GAP_Bonding_Response command is executed. */
static BOOL RBLE_GAP_Change_Connection_Param_Test( void );                                  /* A GAP_Change_Connection_Param command is executed. */
static BOOL RBLE_GAP_Channel_Map_Req_Test( void );                                          /* A GAP_Channel_Map_Req command is executed. */
static BOOL RBLE_GAP_Read_RSSI_Test( void );                                                /* A GAP_Read RSSI Request command is excuted. */
static BOOL RBLE_GAP_Authorized_Ind_Test( void );                                           /* A GAP_Authorized_Ind command is excuted. */

/* --------------------------- Security Manager --------------------------------------*/
static void RBLE_APP_SM_CallBack( RBLE_SM_EVENT *event );                                   /* SM Call Back */
static BOOL RBLE_SM_Set_Key_Test( void );                                                   /* A SM_Set_Key command is executed. */
static BOOL RBLE_SM_Set_Key_Test_Csrk( void );                                              /* A SM_Set_Key_Test command is executed. */
static BOOL RBLE_SM_Start_Enc_Test( void );                                                 /* A SM_Start_Enc command is executed. */
static BOOL RBLE_SM_Tk_Req_Resp_Test( void );                                               /* A SM_Tk_Req_Resp command is executed. */
static BOOL RBLE_SM_Ltk_Req_Resp_Test( void );                                              /* A SM_Ltk_Req_Resp command is executed. */
static BOOL RBLE_SM_Irk_Req_Resp_Test( void );                                              /* A SM_Irk_Req_Resp command is executed. */
static BOOL RBLE_SM_Csrk_Req_Resp_Test( void );                                             /* A SM_Csrk_Req_Resp command is executed. */
static BOOL RBLE_SM_Chk_Bd_Addr_Req_Resp_Test( void );                                      /* A SM_Chk_Bd_Addr_Req_Resp command is executed. */

/* ---------------------------------- GATT ------------------------------------------*/
static void RBLE_APP_GATT_CallBack( RBLE_GATT_EVENT *event );                               /* GATT Call Back */
static BOOL RBLE_GATT_Enable_Test( void );                                                  /* A GATT Enable command is executed. */
static BOOL RBLE_GATT_Discovery_Service_Request_Test( void );
static BOOL RBLE_GATT_Discovery_Char_Request_Test( void );
static BOOL RBLE_GATT_Discovery_Char_Descriptor_Request_Test( void );
static BOOL RBLE_GATT_Read_Char_Request_Test( void );
static BOOL RBLE_GATT_Write_Char_Request_Test( void );
static BOOL RBLE_GATT_Write_Reliable_Request_Test( void );
static BOOL RBLE_GATT_Execute_Write_Char_Request_Test( void );
static BOOL RBLE_GATT_Notify_Request_Test( void );
static BOOL RBLE_GATT_Indicate_Request_Test( void );
static BOOL RBLE_GATT_Write_Response_Test( void );
static BOOL RBLE_GATT_Set_Permission_Test( void );
static BOOL RBLE_GATT_Set_Data_Test( void );

#ifdef USE_CUSTOM_DEMO
extern BOOL RBLE_SCP_Server_Enable_Test(void);
extern BOOL RBLE_SCP_Server_Disable_Test(void);
#endif

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */
RBLE_BD_ADDR        Remote_Device = { 0x5F, 0xB6, 0x05, 0xDC, 0x1B, 0x00 };
RBLE_DEV_ADDR_INFO  Add_White_List_Dev_info;
uint16_t            Connection_Handle = ILLEGAL_CONNECTION_HANDLE;
uint8_t             Role_Status = RBLE_MASTER;
uint8_t             Connection_Idx = 0xff;
uint8_t             Connection_Type = 0xff;
uint16_t            Search_Attribute_Type = RBLE_CHAR_GAP_DEVICE_NAME;
RBLE_APP_KEY_DATA   RBLE_APP_Key;
uint8_t             Own_Addr_Type = RBLE_ADDR_PUBLIC;
uint8_t             Peer_Addr_Type = RBLE_ADDR_PUBLIC;
uint8_t             LTK_Req_For_Enc_Flag = FALSE;
uint16_t            Req_EDIV;


RBLE_CONN_PARAM     change_conn_param = {
    0x00c8,     /* Connection interval minimum ( 250msec = 0xc8 * 1.25msec ) Range:0x0006-0x0c80 */
    0x00f0,     /* Connection interval maximum ( 300msec = 0xf0 * 1.25msec ) Range:0x0006-0x0c80 */
    0x0000,     /* Latency Range:0x0000-0x03e8 */
    0x0320,     /* Supervision timeout ( 8.0s = 0x320 * 10msec ) Range:0x000a-0x0c80 */
};

static const RBLE_ADV_INFO adv_info_base = {
    {
        0x30,                                   /* AdvIntervalMin ( 30msec = 0x30 * 0.625msec ) Range:0x0020-0x4000 */
        0x60,                                   /* AdvIntervalMax ( 60msec = 0x60 * 0.625msec ) Range:0x0020-0x4000 */
        RBLE_GAP_ADV_CONN_UNDIR,                /* AdvType */
        RBLE_ADDR_PUBLIC,                       /* OwnAddrType */
        RBLE_ADDR_PUBLIC,                       /* DirectAddrType */
        { 0xf1, 0x00, 0xde, 0xfa, 0xfe, 0xca }, /* DirectAddr */
        RBLE_ADV_ALL_CHANNELS,                  /* AdvChannelMap */
        RBLE_ADV_ALLOW_SCAN_ANY_CON_ANY,        /* AdvFiltPolicy */
        0x00,                                   /* Pad */
    },
    {
        /* AdvDataLen */
        3+13,
        /* AdvData */
        2,  0x01, 0x06,                                                         /* Flags ( LE General Discoverable Mode | BR/EDR Not Supported ) */ 
        12, 0x09, 'R', 'e', 'n', 'e', 's', 'a', 's', '-', 'B', 'L', 'E',        /* Complete local name */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    },
#if (SERVICE_LIST_LEN != 0) && (SERVICE_LIST_LEN <= 29)
    {
        /* ScanRspDataLen */
        SERVICE_LIST_LEN + 2,
        /* ScanData */
        SERVICE_LIST_LEN + 1,
        0x03,
        #if (USE_LINK_LOSS_SERVICE)
            SVC_UUID(RBLE_SVC_LINK_LOSS),
        #endif
        #if (USE_TX_POWER_SERVICE)
            SVC_UUID(RBLE_SVC_TX_POWER),
        #endif
        #if (USE_IMMEDIATE_ALERT_SERVICE)
            SVC_UUID(RBLE_SVC_IMMEDIATE_ALERT),
        #endif
        #if (PRF_SEL_HTPT)
            SVC_UUID(RBLE_SVC_HEALTH_THERMOMETER),
        #endif
        #if (PRF_SEL_BLPS)
            SVC_UUID(RBLE_SVC_BLOOD_PRESSURE),
        #endif
        #if (USE_DEV_INFO_SERVICE)
            SVC_UUID(RBLE_SVC_DEVICE_INFORMATION),
        #endif
        #if (PRF_SEL_HGHD)
            SVC_UUID(RBLE_SVC_HUMAN_INTERFACE_DEVICE),
        #endif
        #if (USE_SCAN_PARAMETER_SERVICE)
            SVC_UUID(RBLE_SVC_SCAN_PARAMETERS),
        #endif
        #if (USE_BATTERY_SERVICE)
            SVC_UUID(RBLE_SVC_BATTERY_SERVICE),
        #endif
        #if (PRF_SEL_HRPS)
            SVC_UUID(RBLE_SVC_HEART_RATE),
        #endif
        #if (PRF_SEL_CSCS)
            SVC_UUID(RBLE_SVC_CYCLING_SPEED),
        #endif
        #if (PRF_SEL_GLPS)
            SVC_UUID(RBLE_SVC_GLUCOSE),
        #endif
        #if (PRF_SEL_CPPS)
            SVC_UUID(RBLE_SVC_CYCLING_POWER),
        #endif
        #if (PRF_SEL_TIPS)
            SVC_UUID(RBLE_SVC_TX_POWER),
        #endif
        #if (PRF_SEL_ANPS)
            SVC_UUID(RBLE_SVC_ALERT_NOTIFICATION),
        #endif
        #if (PRF_SEL_LNPS)
            SVC_UUID(RBLE_SVC_LOCATION_NAVIGATION),
        #endif
        #if (PRF_SEL_PASS)
            SVC_UUID(RBLE_SVC_PHONE_ALERT_STATUS),
        #endif
        #if (PRF_SEL_RSCS)
            SVC_UUID(RBLE_SVC_RUNNING_SPEED),
        #endif
    }
#else
    {
        /* ScanRspDataLen */
        0,
        /* ScanData */
        0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00
    }
#endif
};

static const RBLE_SET_ADV_DATA beacon_data = {
    /* Advertising data length */
    1+3+1+14,
    /* Advertising data*/
    {
    /*------------------------------------------------------------------------- */
        3,                      /* Length of this structure */
        0x03,                   /* AD Type: Complete list of 16-bit Service UUIDs */
        0xAA, 0xFE,             /* Service UUID: Eddystone ID */
    /*------------------------------------------------------------------------- */
        14,                     /* Length of this structure */
        0x16,                   /* AD Type: Service Data */
        0xAA, 0xFE,             /* Eddystone ID */
        0x10,                   /* Frame type: URL */
        0xF6,                   /* Tx Power -10 dBm @ 1m */
        0x01,                   /* https://www. */
        'r',
        'e',
        'n',
        'e',
        's',
        'a',
        's',
        0x07,                   /* .com */
    }
};

static RBLE_ADV_INFO adv_info;


/* GAP, SM, GATT Select Menu */
static const MENU_INFO  Test_Prog_GAP_SM_GATT_Menu[] = {
    {   1,  RBLE_GAP_Reset_Test,                                NULL,      " 1.GAP Reset\n",                                },
    {   2,  RBLE_GAP_Set_Name_Test,                             NULL,      " 2.GAP Set_Name\n",                             },
    {   3,  RBLE_GAP_Observation_Enable_Test,                   NULL,      " 3.GAP Observation_Enable\n",                   },
    {   4,  RBLE_GAP_Observation_Disable_Test,                  NULL,      " 4.GAP Observation_Disable\n",                  },
    {   5,  RBLE_GAP_Broadcast_Enable_Test,                     NULL,      " 5.GAP Broadcast_Enable\n",                     },
    {   6,  RBLE_GAP_Broadcast_Disable_Test,                    NULL,      " 6.GAP Broadcast_Disable\n",                    },
    {   7,  RBLE_GAP_Set_Bonding_Mode_Test,                     NULL,      " 7.GAP Set_Bonding_Mode\n",                     },
    {   8,  RBLE_GAP_Set_Security_Request_Test,                 NULL,      " 8.GAP Set_Security_Request\n",                 },
    {   9,  RBLE_GAP_Get_Device_Info_Test,                      NULL,      " 9.GAP Get_Device_Info\n",                      },
    {  10,  RBLE_GAP_Get_White_List_Size_Test,                  NULL,      "10.GAP Get_White_List_Size\n",                  },
    {  11,  RBLE_GAP_Add_To_White_List_Test,                    NULL,      "11.GAP Add_To_White_List\n",                    },
    {  12,  RBLE_GAP_Del_From_White_List_Test,                  NULL,      "12.GAP Del_From_White_List\n",                  },
    {  13,  RBLE_GAP_Get_Remote_Device_Name_Test,               NULL,      "13.GAP Get_Remote_Device_Name\n",               },
    {  14,  RBLE_GAP_Get_Remote_Device_Info_Test,               NULL,      "14.GAP Get_Remote_Device_Info\n",               },
    {  15,  RBLE_GAP_Device_Search_Test,                        NULL,      "15.GAP Device_Search\n",                        },
    {  17,  RBLE_GAP_Set_Random_Address_Test,                   NULL,      "17.GAP Set_Random_Address\n",                   },
    {  18,  RBLE_GAP_Set_Privacy_Feature_Test,                  NULL,      "18.GAP Set_Privacy_Feature\n",                  },
    {  20,  RBLE_GAP_Create_Connection_Test,                    NULL,      "20.GAP Create_Connection\n",                    },
    {  21,  RBLE_GAP_Connection_Cancel_Test,                    NULL,      "21.GAP Connection_Cancel\n",                    },
    {  22,  RBLE_GAP_Disconnect_Test,                           NULL,      "22.GAP Disconnect\n",                           },
    {  23,  RBLE_GAP_Start_Bonding_Test,                        NULL,      "23.GAP Start_Bonding\n",                        },
    {  24,  RBLE_GAP_Bonding_Info_Ind_Test,                     NULL,      "24.GAP Bonding_Info_Ind\n",                     },
    {  25,  RBLE_GAP_Bonding_Response_Test,                     NULL,      "25.GAP Bonding_Response\n",                     },
    {  26,  RBLE_GAP_Change_Connection_Param_Test,              NULL,      "26.GAP Change_Connection_Param\n",              },
    {  27,  RBLE_GAP_Channel_Map_Req_Test,                      NULL,      "27.GAP Channel_Map_Req\n",                      },
    {  28,  RBLE_GAP_Read_RSSI_Test,                            NULL,      "28.GAP Read_RSSI\n",                            },
    {  16,  RBLE_GAP_Authorized_Ind_Test,                       NULL,      "16.GAP Authorized_Ind\n",                       },
    {  29,  RBLE_SM_Set_Key_Test,                               NULL,      "29.SM Set_Key\n",                               },
    {  30,  RBLE_SM_Start_Enc_Test,                             NULL,      "30.SM Start_Enc\n",                             },
    {  31,  RBLE_SM_Tk_Req_Resp_Test,                           NULL,      "31.SM Tk_Req_Resp\n",                           },
    {  32,  RBLE_SM_Ltk_Req_Resp_Test,                          NULL,      "32.SM Ltk_Req_Resp\n",                          },
    {  33,  RBLE_SM_Irk_Req_Resp_Test,                          NULL,      "33.SM Irk_Req_Resp\n",                          },
    {  34,  RBLE_SM_Csrk_Req_Resp_Test,                         NULL,      "34.SM Csrk_Req_Resp\n",                         },
    {  35,  RBLE_SM_Chk_Bd_Addr_Req_Resp_Test,                  NULL,      "35.SM Chk_Bd_Addr_Req_Resp\n",                  },
    {  36,  RBLE_GATT_Enable_Test,                              NULL,      "36.GATT Enable\n",                              },
    {  37,  RBLE_GATT_Discovery_Char_Request_Test,              NULL,      "37.GATT Discovery_Char_Request\n",              },
    {  38,  RBLE_GATT_Discovery_Service_Request_Test,           NULL,      "38.GATT Discovery_Service_Request\n",           },
    {  39,  RBLE_GATT_Discovery_Char_Descriptor_Request_Test,   NULL,      "39.GATT Discovery_Char_Descriptor_Request\n",   },
    {  40,  RBLE_GATT_Read_Char_Request_Test,                   NULL,      "40.GATT Read_Char_Request\n",                   },
    {  41,  RBLE_GATT_Write_Char_Request_Test,                  NULL,      "41.GATT Write_Char_Request\n",                  },
    {  42,  RBLE_GATT_Write_Reliable_Request_Test,              NULL,      "42.GATT Write_Reliable_Request\n",              },
    {  43,  RBLE_GATT_Execute_Write_Char_Request_Test,          NULL,      "43.GATT Execute_Write_Char_Request\n",          },
    {  44,  RBLE_GATT_Notify_Request_Test,                      NULL,      "44.GATT Notify_Request\n",                      },
    {  45,  RBLE_GATT_Indicate_Request_Test,                    NULL,      "45.GATT Indicate_Request\n",                    },
    {  47,  RBLE_GATT_Write_Response_Test,                      NULL,      "47.GATT Write_Response\n",                      },
    {  48,  RBLE_GATT_Set_Permission_Test,                      NULL,      "48.GATT Set_Permission\n",                      },
    {  49,  RBLE_GATT_Set_Data_Test,                            NULL,      "49.GATT Set_Data\n",                            },
    {  ESC_KEY, Menu_Exit,                                      NULL,      "ESC Key: Menu exit\n",                          },
    {  0,   NULL,                                               NULL,       NULL,                                           }
};

const SELECT_MENU   Test_Prog_GAP_SM_GATT_Select_Menu = {
    "\n-- BLE Sample Program GAP & SM & GATT Test Menu --",
    Test_Prog_GAP_SM_GATT_Menu
};

#if (PRF_SEL_CPPC)
extern uint8_t g_cpp_brd_en;
#endif

/******************************* Program Area *****************************************/

/******************************************************************************************/
/**
 *  @brief rBLE GAP Event and Status Disp
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
static void GAP_Event_Status_Disp(char_t *event_str, RBLE_STATUS status)
{
    printf( "\nrBLE GAP EVENT (%s) Status(%s)\n", event_str, Get_Status_Str( status ) );
}
/******************************************************************************************/
/**
 *  @brief rBLE GAP Event Disp
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
static void GAP_Event_Disp(char_t *event_str)
{
    printf( "\nrBLE GAP EVENT (%s)\n", event_str );
}

static void GAP_ChangeConn_Param_Disp(RBLE_CONN_PARAM *param_p)
{
    printf(" conn intv min  = %f.2 msec\n", (float)(param_p->intv_min * 1.25));
    printf(" conn intv max  = %f.2 msec\n", (float)(param_p->intv_max * 1.25));
    printf(" slave latency  = %d events\n", param_p->latency);
    printf(" supervision to = %ld msec\n", (param_p->time_out * 10));
}

/******************************************************************************************/
/**
 *  @brief rBLE GAP Call Back
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
static void RBLE_APP_GAP_CallBack( RBLE_GAP_EVENT *event )
{
    RBLE_ADV_REPORT     *Adv_Rep_p;
    RBLE_BD_NAME        *Bd_Name_p;
    RBLE_CONNECT_INFO   *Con_Info_p;

    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type ) {
        case RBLE_GAP_EVENT_RESET_RESULT:
            GAP_Event_Status_Disp( "RESET RESULT", event->param.reset_result.status);
            printf( "rBLE Version = Major(%02X),Minor(%02X)\n", event->param.reset_result.rBLE_major_ver, event->param.reset_result.rBLE_minor_ver );
            if ( RBLE_MAJOR_VERSION != event->param.reset_result.rBLE_major_ver ) {
                printf( "Warning:Please confirm a version.\n" );
            }
#ifdef USE_CUSTOM_DEMO
            RBLE_GAP_Broadcast_Enable_Test();
#endif
            break;
        case RBLE_GAP_EVENT_SET_NAME_COMP:
            GAP_Event_Status_Disp( "SET_NAME_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_OBSERVATION_ENABLE_COMP:
            GAP_Event_Status_Disp( "OBSERVATION_ENABLE_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_OBSERVATION_DISABLE_COMP:
            GAP_Event_Status_Disp( "OBSERVATION_DISABLE_COMP", event->param.status );
            break;
        case RBLE_GAP_EVENT_BROADCAST_ENABLE_COMP:
            GAP_Event_Status_Disp( "BROADCAST_ENABLE_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_BROADCAST_DISABLE_COMP:
            GAP_Event_Status_Disp( "BROADCAST_DISABLE_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_SET_BONDING_MODE_COMP:
            GAP_Event_Status_Disp( "SET_BONDING_MODE_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_SET_SECURITY_REQUEST_COMP:
            printf( "\nrBLE GAP EVENT (SET_SECURITY_REQUEST_COMP) Status(%s), SEC(%d)\n", Get_Status_Str( event->param.set_sec_req.status ), event->param.set_sec_req.sec );
            break;
        case RBLE_GAP_EVENT_GET_DEVICE_INFO_COMP:
            GAP_Event_Status_Disp( "GET_DEVICE_INFO_COMP", event->param.get_dev_ver.status);
            BdAddress_Disp( &event->param.get_dev_ver.addr );
            printf( "HCI  Version(0x%02x), HCI  SubVersion(0x%04x)\n", event->param.get_dev_ver.ver_info.hci_ver, event->param.get_dev_ver.ver_info.hci_subver );
            printf( "LMP  Version(0x%02x), LMP  SubVersion(0x%04x)\n", event->param.get_dev_ver.ver_info.lmp_ver, event->param.get_dev_ver.ver_info.lmp_subver );
            printf( "HOST Version(0x%02x), HOST SubVersion(0x%04x)\n", event->param.get_dev_ver.ver_info.host_ver, event->param.get_dev_ver.ver_info.host_subver );
            printf( "Manufacture Name(0x%x)\n", event->param.get_dev_ver.ver_info.company_id );
            break;
        case RBLE_GAP_EVENT_RPA_RESOLVED:
            GAP_Event_Disp("RPA_RESOLVED");
            printf( "Resolved: Type(%s), ", ((event->param.rpa_resolved.res_addr_type == RBLE_ADDR_RAND) ? "Random" : "Public") );
            BdAddress_Disp( &event->param.rpa_resolved.res_addr );
            printf( "Original: Type(%s), ", ((event->param.rpa_resolved.addr_type == RBLE_ADDR_RAND) ? "Random" : "Public") );
            BdAddress_Disp( &event->param.rpa_resolved.addr );
            break;
        case RBLE_GAP_EVENT_GET_WHITE_LIST_SIZE_COMP:
            printf( "\nrBLE GAP EVENT (GET_WHITE_LIST_SIZE_COMP) Status(%s), Size(%d)\n", Get_Status_Str( event->param.get_wlst_size.status ), event->param.get_wlst_size.wlist_size );
            break;
        case RBLE_GAP_EVENT_ADD_TO_WHITE_LIST_COMP:
            GAP_Event_Status_Disp( "ADD_TO_WHITE_LIST_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_DEL_FROM_WHITE_LIST_COMP:
            GAP_Event_Status_Disp( "DEL_FROM_WHITE_LIST_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_GET_REMOTE_DEVICE_NAME_COMP:
            GAP_Event_Disp("GET_REMOTE_DEVICE_NAME_COMP");
            Bd_Name_p = &event->param.get_remote_dev_name.bd_name;
            Bd_Name_p->name[ Bd_Name_p->namelen ] = '\0';
            printf( "Device Name = %s\n", Bd_Name_p->name );
            break;
        case RBLE_GAP_EVENT_GET_REMOTE_DEVICE_INFO_COMP:
            GAP_Event_Status_Disp( "GET_REMOTE_DEVICE_INFO_COMP", event->param.get_remote_dev_info.status);
            Connection_Handle_Disp( event->param.get_remote_dev_info.conhdl );
            printf( "LMP Version = 0x%04x\n", event->param.get_remote_dev_info.vers );
            printf( "LMP Sub Version = 0x%04x\n", event->param.get_remote_dev_info.subvers );
            printf( "Manufacturer = 0x%04x\n", event->param.get_remote_dev_info.compid );
            printf( "Features[%02x:%02x:%02x:%02x:%02x:%02x:%02x:%02x]\n", event->param.get_remote_dev_info.feats_used.feats[ 0 ], event->param.get_remote_dev_info.feats_used.feats[ 1 ], 
                                                                                    event->param.get_remote_dev_info.feats_used.feats[ 2 ], event->param.get_remote_dev_info.feats_used.feats[ 3 ], 
                                                                                    event->param.get_remote_dev_info.feats_used.feats[ 4 ], event->param.get_remote_dev_info.feats_used.feats[ 5 ], 
                                                                                    event->param.get_remote_dev_info.feats_used.feats[ 6 ], event->param.get_remote_dev_info.feats_used.feats[ 7 ] );
            break;
        case RBLE_GAP_EVENT_DEVICE_SEARCH_COMP:
            GAP_Event_Disp("DEVICE_SEARCH_COMP");
            break;
        case RBLE_GAP_EVENT_DEVICE_SEARCH_RESULT_IND:
            GAP_Event_Disp("DEVICE_SEARCH_RESULT_IND");
            Adv_Rep_p = &event->param.dev_search_result.adv_resp;
            Peer_Addr_Type = Adv_Rep_p->adv_addr_type;
            AdvRepot_Disp( Adv_Rep_p );
            /* The last device is saved. */
            Remote_Device = Adv_Rep_p->adv_addr;

            Add_White_List_Dev_info.dev_addr_type = Peer_Addr_Type;
            Add_White_List_Dev_info.dev_addr = Remote_Device;
            break;
        case RBLE_GAP_EVENT_SET_RANDOM_ADDRESS_COMP:
            GAP_Event_Status_Disp( "SET_RANDOM_ADDRESS_COMP", event->param.set_rand_adr.status);
            BdAddress_Disp( &event->param.set_rand_adr.addr );
            break;
        case RBLE_GAP_EVENT_SET_PRIVACY_FEATURE_COMP:
            GAP_Event_Status_Disp( "SET_PRIVACY_FEATURE_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_CONNECTION_COMP:
            GAP_Event_Status_Disp( "CONNECTION_COMP", event->param.conn_comp.connect_info.status);
            if ( RBLE_OK == event->param.conn_comp.connect_info.status ) {
                Con_Info_p = &event->param.conn_comp.connect_info;
                Connection_Handle = Con_Info_p->conhdl;
                Remote_Device = Con_Info_p->peer_addr;
                Peer_Addr_Type = Con_Info_p->peer_addr_type;
                printf( "Connection Handle = 0x%04X,  ", Con_Info_p->conhdl );
                printf( "Index  = 0x%02X,  ", Con_Info_p->idx );
                BdAddress_Disp( &Con_Info_p->peer_addr );
                Role_Status = Con_Info_p->role;
            }

            Add_White_List_Dev_info.dev_addr_type = Peer_Addr_Type;
            Add_White_List_Dev_info.dev_addr = Remote_Device;

#ifdef USE_CUSTOM_DEMO
            RBLE_SCP_Server_Enable_Test();
#endif
            break;
        case RBLE_GAP_EVENT_CONNECTION_CANCEL_COMP:
            GAP_Event_Status_Disp( "CONNECTION_CANCEL_COMP", event->param.status);
            break;
        case RBLE_GAP_EVENT_DISCONNECT_COMP:
            GAP_Event_Status_Disp( "DISCONNECT_COMP", event->param.disconnect.status);
            printf( "reason = %s\n", Get_Status_Str( event->param.disconnect.reason ) );
#ifdef USE_CUSTOM_DEMO
            RBLE_SCP_Server_Disable_Test();
#endif
            break;
        case RBLE_GAP_EVENT_ADVERTISING_REPORT_IND:
#if (PRF_SEL_CPPC)
            if (TRUE == g_cpp_brd_en)
            {
                CPP_Broadcast_Rcv(&event->param.adv_report.evt.adv_rep);
                return;
            }
#endif
            GAP_Event_Disp("ADVERTISING_REPORT_IND");
            AdvRepot_Disp( &event->param.adv_report.evt.adv_rep );
            break;
        case RBLE_GAP_EVENT_BONDING_COMP:
            GAP_Event_Status_Disp( "BONDING_COMP", event->param.bonding_comp.status);
            Connection_Idx = event->param.bonding_comp.idx;
            break;
        case RBLE_GAP_EVENT_BONDING_REQ_IND:
            GAP_Event_Disp("BONDING_REQ_IND");
            BdAddress_Disp( &event->param.bonding_req.bd_addr );
            break;
        case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_REQ_IND:
            GAP_Event_Disp("CHANGE_CONNECTION_PARAM_REQ_IND");
            change_conn_param = event->param.chg_connect_param_req.conn_param;
            GAP_ChangeConn_Param_Disp(&change_conn_param);
            break;
        case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_COMP:
            GAP_Event_Status_Disp( "CHANGE_CONNECTION_PARAM_COMP", event->param.chg_connect_param.status);
            break;
        case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_RESPONSE:
            GAP_Event_Status_Disp( "CHANGE_CONNECTION_PARAM_RESPONSE", event->param.chg_connect_param_resp.status);
            break;
        case RBLE_GAP_EVENT_CHANNEL_MAP_REQ_COMP:
            GAP_Event_Disp("CHANNEL_MAP_REQ_COMP");
            break;
        case RBLE_GAP_EVENT_READ_RSSI_COMP:
            GAP_Event_Status_Disp( "RBLE_GAP_EVENT_READ_RSSI_COMP", event->param.read_rssi.status);
            printf( "RSSI = %d(dBm)\n", (int8_t)(event->param.read_rssi.rssi) );
            break;
        case RBLE_GAP_EVENT_WR_CHAR_IND:
            printf( "\nrBLE GAP EVENT (RBLE_GAP_EVENT_WR_CHAR_IND) conhdl(0x%04X)\n", event->param.wr_char.conhdl );
            if (RBLE_GAP_WR_CHAR_NAME == event->param.wr_char.type) {
                printf( "Name = %s\n", event->param.wr_char.param.name.name );
            } else {
                printf( "Appearance = 0x%04X\n",  event->param.wr_char.param.appearance );
            }
            break;
        case RBLE_GAP_EVENT_COMMAND_DISALLOWED_IND:
            GAP_Event_Status_Disp( "COMMAND_DISALLOWED_IND", event->param.cmd_disallowed_ind.status);
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            printf( "rBLE App Gap Receive 0x%x\n", event->type );
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A GAP_Reset command is executed. 
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
BOOL RBLE_GAP_Reset_Test( void )
{

    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Reset( &RBLE_APP_GAP_CallBack, &RBLE_APP_SM_CallBack );
    
    Connection_Handle = ILLEGAL_CONNECTION_HANDLE;
    
    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Set_Name command is executed. 
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
static BOOL RBLE_GAP_Set_Name_Test( void )
{
    static RBLE_BD_NAME bd_name_param = {
        sizeof( "rBLE Test Device" ),   /* length for name */
        "rBLE Test Device"              /* array of bytes for name */
    };
    
    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Set_Name( &bd_name_param );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Observation_Enable command is executed. 
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
static BOOL RBLE_GAP_Observation_Enable_Test( void )
{
    RBLE_STATUS                             Ret_Status;
    RBLE_SCANNING_INFO                      Param = {
        {
            RBLE_SCAN_ACTIVE,           /* Active Scan */
            0,                          /* Reserved */
            0x60,                       /* ScanInterval ( 60msec = 0x60 * 0.625msec ) Range:0x0004-0x4000 */
            0x30,                       /* ScanWindow   ( 30msec = 0x30 * 0.625msec ) Range:0x0004-0x4000 */
            RBLE_ADDR_PUBLIC,           /* OwnAddrType */
            RBLE_SCAN_ALLOW_ADV_ALL     /* ScanFiltPolicy */
        },
        RBLE_SCAN_FILT_DUPLIC_DIS,      /* FilterDup */
        0                               /* Reserved */
    };

    Param.set_scan.own_addr_type = Own_Addr_Type;

    /* API Call */
    Ret_Status = RBLE_GAP_Observation_Enable(RBLE_GAP_OBSERVER, &Param);

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Observation_Disable command is executed. 
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
static BOOL RBLE_GAP_Observation_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Observation_Disable();

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Broadcast_Enable command is executed. 
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
BOOL RBLE_GAP_Broadcast_Enable_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint16_t        disc_mode = RBLE_GAP_GEN_DISCOVERABLE;
    uint16_t        conn_mode;
    int32_t         Parameter_No;

    adv_info = adv_info_base;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = 0;
    }

    switch (Parameter_No)
    {
        case 1: // Broadcaster
            disc_mode = RBLE_GAP_BROADCASTER;
            conn_mode = 0;
            adv_info.adv_param.adv_type = RBLE_GAP_ADV_DISC_UNDIR;
            adv_info.adv_param.adv_intv_min = 160;    /* AdvIntervalMin ( 100msec = 160 * 0.625msec ) */
            adv_info.adv_param.adv_intv_max = 160;    /* AdvIntervalMax ( 100msec = 160 * 0.625msec ) */
            adv_info.adv_data = beacon_data;
            break;
        case 2: // Directed connectable
            conn_mode = RBLE_GAP_DIR_CONNECTABLE;
            adv_info.adv_param.adv_type = RBLE_GAP_ADV_CONN_DIR_HIGH_DUTY;
            adv_info.adv_param.direct_addr_type = Peer_Addr_Type;
            adv_info.adv_param.direct_addr = Remote_Device;
            break;
        case 0: // General Discoverable & Undirected Connectable
        default:
            disc_mode = RBLE_GAP_GEN_DISCOVERABLE;
            conn_mode = RBLE_GAP_UND_CONNECTABLE;
            break;
    }

    /* API Call */
    Ret_Status = RBLE_GAP_Broadcast_Enable(disc_mode, conn_mode, &adv_info);

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Broadcast_Disable command is executed. 
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
static BOOL RBLE_GAP_Broadcast_Disable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Broadcast_Disable();

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Set_Bonding_Mode command is executed. 
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
static BOOL RBLE_GAP_Set_Bonding_Mode_Test( void )
{
    const uint16_t mode = RBLE_GAP_BONDABLE;        /* GAP_BONDABLE */

    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Set_Bonding_Mode( mode );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Set_Security_Request command is executed. 
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
static BOOL RBLE_GAP_Set_Security_Request_Test( void )
{
//  const uint8_t sec = RBLE_GAP_NO_SEC;                    /**< No security (no authentication and encryption) */
    const uint8_t sec = RBLE_GAP_SEC1_NOAUTH_PAIR_ENC;      /**< Unauthenticated pairing with encryption */
//  const uint8_t sec = RBLE_GAP_SEC1_AUTH_PAIR_ENC;        /**< Authenticated pairing with encryption */
//  const uint8_t sec = RBLE_GAP_SEC2_NOAUTH_DATA_SGN;      /**< Unauthenticated pairing with data signing */
//  const uint8_t sec = RBLE_GAP_SEC2_AUTH_DATA_SGN;        /**< Authentication pairing with data signing */

    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Set_Security_Request( sec );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Get_Device_Info command is executed. 
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
static BOOL RBLE_GAP_Get_Device_Info_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Get_Device_Info();

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Get_White_List_Size command is executed. 
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
static BOOL RBLE_GAP_Get_White_List_Size_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Get_White_List_Size();

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Add_To_White_List command is executed. 
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
static BOOL RBLE_GAP_Add_To_White_List_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Add_To_White_List( &Add_White_List_Dev_info );

    BdAddress_Disp( &Add_White_List_Dev_info.dev_addr );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Del_From_White_List command is executed. 
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
static BOOL RBLE_GAP_Del_From_White_List_Test( void )
{
    const BOOL all_dev = FALSE; 

    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Del_From_White_List( all_dev, &Add_White_List_Dev_info );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Get_Remote_Device_Name command is executed. 
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
static BOOL RBLE_GAP_Get_Remote_Device_Name_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_CREATE_CONNECT_PARAM param;

    /* Connection Parameter Set*/
    param.scan_intv         = 48;                               /* Scan interval    ( 30msec = 48 * 0.625msec ) Range:0x0004-0x4000 */
    param.scan_window       = 48;                               /* Scan window size ( 30msec = 48 * 0.625msec ) Range:0x0004-0x4000 */
    param.init_filt_policy  = RBLE_GAP_INIT_FILT_IGNORE_WLST;   /* Initiator filter policy */
    param.peer_addr_type    = Peer_Addr_Type;                   /* Peer address type */
    param.peer_addr         = Remote_Device;                    /* Peer BD address */
    param.own_addr_type     = Own_Addr_Type;                    /* Own address type - 0=public/1=random */
    param.con_intv_min      = 40;                               /* Minimum of connection interval ( 50msec = 40 * 1.25msec ) Range:0x0006-0x0c80 */
    param.con_intv_max      = 40;                               /* Maximum of connection interval ( 50msec = 40 * 1.25msec ) Range:0x0006-0x0c80 */
    param.con_latency       = 0x00;                             /* Connection latency Range:0x0000-0x01f4 */
    param.superv_to         = 0x1f4;                            /* Link supervision timeout ( 5.0s = 0x1f4 * 10msec ) Range:0x000a-0x0c80 */
    param.ce_len_min        = 0x00;                             /* Minimum CE length ( 00msec = 0x00 * 0.625msec ) Range:0x0000-0xffff */
    param.ce_len_max        = 80;                               /* Maximum CE length ( 50msec = 80 * 0.625msec ) Range:0x0000-0xffff */

    /* API Call */
    Ret_Status = RBLE_GAP_Get_Remote_Device_Name( &param );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Get_Remote_Device_Info command is executed. 
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
static BOOL RBLE_GAP_Get_Remote_Device_Info_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        Ret_Status = RBLE_GAP_Get_Remote_Device_Info( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Device_Search command is executed. 
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
static BOOL RBLE_GAP_Device_Search_Test( void )
{
    const char_t *search_type[] = { "General", "Limited", "Cancel" };

    RBLE_STATUS     Ret_Status;
    uint8_t         discovery_type;

    if ( 0 != RBLE_Parameter_Cnt ) {
        discovery_type = (uint8_t)RBLE_Parameter_Arg[ 0 ];
    } else {
        discovery_type = RBLE_GAP_GEN_DISCOVERY_TYPE;
    }

    if ( discovery_type > RBLE_GAP_CANCEL_DISCOVERY ) {
        discovery_type = RBLE_GAP_GEN_DISCOVERY_TYPE;
    }

    printf( "Execute %s Discovery\n", search_type[discovery_type] );

    /* API Call */
    Ret_Status = RBLE_GAP_Device_Search( discovery_type, Own_Addr_Type );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Set_Random_Address command is executed. 
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
static BOOL RBLE_GAP_Set_Random_Address_Test( void )
{
    static RBLE_BD_ADDR bd_addr = {
        0xaa, 0x00, 0xde, 0xfa, 0xfe, 0xca,     /* static addresss */
    };

    RBLE_STATUS     Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Set_Random_Address( &bd_addr );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Set_Privacy_Feature command is executed. 
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
static BOOL RBLE_GAP_Set_Privacy_Feature_Test( void )
{
    RBLE_STATUS                 Ret_Status;
    int32_t                     Parameter_No;
    uint8_t                     priv_flag;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = 0;
    }
    priv_flag = (uint8_t)Parameter_No;

    switch (priv_flag)
    {
        case RBLE_CENTRAL_PRIV_ENABLE:
        case RBLE_PH_PRIV_ENABLE:
        case RBLE_BCST_PRIV_ENABLE:
        case RBLE_OBSERV_PRIV_ENABLE:
            Own_Addr_Type = RBLE_ADDR_RAND;
            break;
        case RBLE_DEVICE_PRIV_DISABLE:
        case RBLE_OBSERV_PRIV_RESOLVE:
            Own_Addr_Type = RBLE_ADDR_PUBLIC;
            break;
        default:
            priv_flag = RBLE_DEVICE_PRIV_DISABLE;
            break;
    }

    /* API Call */
    Ret_Status = RBLE_GAP_Set_Privacy_Feature(priv_flag, TRUE);

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Create_Connection command is executed. 
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
static BOOL RBLE_GAP_Create_Connection_Test( void )
{
    RBLE_STATUS Ret_Status;
    RBLE_CREATE_CONNECT_PARAM param;

    /* Connection Parameter Set*/
    param.scan_intv         = 48;                               /* Scan interval    ( 30msec = 48 * 0.625msec ) Range:0x0004-0x4000 */
    param.scan_window       = 48;                               /* Scan window size ( 30msec = 48 * 0.625msec ) Range:0x0004-0x4000 */
    param.init_filt_policy  = RBLE_GAP_INIT_FILT_IGNORE_WLST;   /* Initiator filter policy */
    param.peer_addr_type    = Peer_Addr_Type;                   /* Peer address type */
    param.peer_addr         = Remote_Device;                    /* Peer BD address */
    param.own_addr_type     = Own_Addr_Type;                    /* Own address type - 0=public/1=random */
    param.con_intv_min      = 40;                               /* Minimum of connection interval ( 50msec = 40 * 1.25msec ) Range:0x0006-0x0c80 */
    param.con_intv_max      = 40;                               /* Maximum of connection interval ( 50msec = 40 * 1.25msec ) Range:0x0006-0x0c80 */
    param.con_latency       = 0x00;                             /* Connection latency Range:0x0000-0x01f4 */
    param.superv_to         = 0x1f4;                            /* Link supervision timeout ( 5.0s = 0x1f4 * 10msec ) Range:0x000a-0x0c80 */
    param.ce_len_min        = 0x00;                             /* Minimum CE length ( 00msec = 0x00 * 0.625msec ) Range:0x0000-0xffff */
    param.ce_len_max        = 80;                               /* Maximum CE length ( 50msec = 80 * 0.625msec ) Range:0x0000-0xffff */

    if ( 0 != RBLE_Parameter_Cnt ) {
        param.init_filt_policy  = RBLE_GAP_INIT_FILT_USE_WLST;
    }

    /* API Call */
    Ret_Status = RBLE_GAP_Create_Connection(&param);

    BdAddress_Disp( &param.peer_addr );
    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Connection_Cancel command is executed. 
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
static BOOL RBLE_GAP_Connection_Cancel_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GAP_Connection_Cancel();

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Disconnect command is executed. 
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
static BOOL RBLE_GAP_Disconnect_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GAP_Disconnect( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Start_Bonding command is executed. 
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
static BOOL RBLE_GAP_Start_Bonding_Test( void )
{
    RBLE_BOND_PARAM      Param = {
        { 0 },                                  /* BT Addresss to bond */
        RBLE_OOB_AUTH_DATA_NOT_PRESENT,         /* OOB flag */
        RBLE_IO_CAP_NO_INPUT_NO_OUTPUT,         /* IO capabilities */
        RBLE_AUTH_REQ_NO_MITM_BOND,             /* Authentication Requirements */
        RBLE_SMP_MAX_ENC_SIZE_LEN,              /* Encryption key size */
        0,                                      /* Initiator key distribution */
        RBLE_KEY_DIST_ENCKEY,                   /* Responder key distribution */
    };
    RBLE_STATUS         Ret_Status;

    Param.addr = Remote_Device;

    if (RBLE_ADDR_RAND == Peer_Addr_Type)
    {
        Param.ikey_dist = RBLE_KEY_DIST_IDKEY;
    }

    if ( 0 != RBLE_Parameter_Cnt ) {
        /* Passkey Entry (Display) */
        Param.iocap = RBLE_IO_CAP_DISPLAY_ONLY;
        Param.auth = RBLE_AUTH_REQ_MITM_BOND;
    }

    /* API Call */
    Ret_Status = RBLE_GAP_Start_Bonding(&Param);

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}
/******************************************************************************************/
/**
 *  @brief A GAP_Bonding_Info_Ind command is executed. 
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
static BOOL RBLE_GAP_Bonding_Info_Ind_Test( void )
{
    RBLE_BD_ADDR        addr;

    const uint8_t bond_op = RBLE_GAP_BOND_ADDED;

    RBLE_STATUS Ret_Status;

    addr = Remote_Device;

    /* API Call */
    Ret_Status = RBLE_GAP_Bonding_Info_Ind( bond_op, &addr );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}


/******************************************************************************************/
/**
 *  @brief A GAP_Bonding_Response command is executed. 
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
static BOOL RBLE_GAP_Bonding_Response_Test( void )
{
    RBLE_BOND_RESP_PARAM    Param = {
        0x0000,                                 /* Connection handle */
        RBLE_OK,                                /* accept(RBLE_OK) or reject(RBLE_CONN_REJ_UNACCEPTABLE_BDADDR) bonding */
        RBLE_IO_CAP_NO_INPUT_NO_OUTPUT,         /* IO capabilities */
        RBLE_OOB_AUTH_DATA_NOT_PRESENT,         /* OOB flag */
        RBLE_AUTH_REQ_NO_MITM_BOND,             /* Authentication Requirements */
        RBLE_SMP_MAX_ENC_SIZE_LEN,              /* Encryption key size */
        0,                                      /* Initiator key distribution */
        RBLE_KEY_DIST_ENCKEY,                   /* Responder key distribution */
        0x00                                    /* Reserved */
    };
    RBLE_STATUS             Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {

        if (RBLE_ADDR_RAND == Peer_Addr_Type)
        {
            Param.ikeys = RBLE_KEY_DIST_IDKEY;
        }
        Param.conhdl = Connection_Handle;

        if ( 0 != RBLE_Parameter_Cnt ) {
            /* Passkey Entry (Display) */
            Param.io_cap = RBLE_IO_CAP_DISPLAY_ONLY;
            Param.auth_req = RBLE_AUTH_REQ_MITM_BOND;
        }

        /* API Call */
        Ret_Status = RBLE_GAP_Bonding_Response(&Param);

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Change_Connection_Param command is executed. 
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
static BOOL RBLE_GAP_Change_Connection_Param_Test( void )
{
    uint16_t result = 0x0000;
    uint8_t role;

    RBLE_STATUS Ret_Status;

    role = Role_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        GAP_ChangeConn_Param_Disp(&change_conn_param);
        Ret_Status = RBLE_GAP_Change_Connection_Param(Connection_Handle, result, &change_conn_param, role);

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Channel_Map_Req command is executed. 
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
static BOOL RBLE_GAP_Channel_Map_Req_Test( void )
{
    const BOOL update_map = 0;
    static RBLE_LE_CHNL_MAP chmap = {
        0xff,
        0xff,
        0xff,
        0xff,
        0xff,
    };

    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GAP_Channel_Map_Req(update_map, Connection_Handle, &chmap);

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Read_RSSI command is executed. 
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
static BOOL RBLE_GAP_Read_RSSI_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GAP_Read_RSSI( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GAP_Authorized_Ind command is executed. 
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
static BOOL RBLE_GAP_Authorized_Ind_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* Valid Connection Handle ? */
    if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
        Illegal_Connection_Print();
    } else {
        /* API Call */
        Ret_Status = RBLE_GAP_Authorized_Ind( Connection_Handle );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/* --------------------------- Security Manager -------------------------------------*/
/******************************************************************************************/
/**
 *  @brief rBLE SM Call Back
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
static void RBLE_APP_SM_CallBack( RBLE_SM_EVENT *event )
{
    static uint8_t  Csrl_Flag = FALSE;
    
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type ) {
        case RBLE_SM_EVENT_SET_CNF:         /* Device Key Setting Complete Event */
            printf( "\nrBLE SM EVENT(EVENT_SET_CNF) Status(%s)\n", Get_Status_Str( event->param.set_conf.status ) );
            printf( "key_code = %d\n", event->param.set_conf.key_code );
            if ( FALSE == Csrl_Flag ) {
                Csrl_Flag = TRUE;
                RBLE_SM_Set_Key_Test_Csrk();
            } else {
                Csrl_Flag = FALSE;
            }
            break;
        case RBLE_SM_ENC_START_IND:         /* Encryption Start Indicate Event */
            printf( "\nrBLE SM EVENT(ENC_START_IND) Status(%s)\n", Get_Status_Str( event->param.sec_start.status ) );
            printf( "idx = %d", event->param.sec_start.idx );
            printf( ",  key_size = %d", event->param.sec_start.key_size );
            printf( ",  sec_prop = %d", event->param.sec_start.sec_prop );
            printf( ",  bonded = %d\n", event->param.sec_start.bonded );
            break;
        case RBLE_SM_TK_REQ_IND:            /* TK Request Evetnt */
            printf( "\nrBLE SM EVENT(TK_REQ_IND)\n" );
            printf( "idx = %d", event->param.tk_req.idx );
            printf( ",  oob_en = %d", event->param.tk_req.oob_en );
            printf( ",  disp_en = %d\n", event->param.tk_req.disp_en );
            Connection_Idx = event->param.tk_req.idx;
            break;
        case RBLE_SM_LTK_REQ_IND:           /* LTK Request Evetnt */
            printf( "\nrBLE SM EVENT(LTK_REQ_IND)\n" );
            printf( "idx = %d\n", event->param.ltk_req.idx );
            Connection_Idx = event->param.ltk_req.idx;
            LTK_Req_For_Enc_Flag = FALSE;
            break;
        case RBLE_SM_LTK_REQ_FOR_ENC_IND:   /* LTK Request for Encryption Event */
            printf( "\nrBLE SM EVENT(LTK_REQ_FOR_ENC_IND)\n" );
            printf( "idx = %d\n", event->param.ltk_req_for_enc.idx );
            printf( "EDIV = 0x%04X\n", event->param.ltk_req_for_enc.ediv );
            RandomDataDisp( &event->param.ltk_req_for_enc.nb );
            Connection_Idx = event->param.ltk_req_for_enc.idx;
            Req_EDIV = event->param.ltk_req_for_enc.ediv;
            LTK_Req_For_Enc_Flag = TRUE;
            break;
        case RBLE_SM_IRK_REQ_IND:           /* IRK Request Evetnt */
            printf( "\nrBLE SM EVENT(IRK_REQ_IND)\n" );
            printf( "idx = %d\n", event->param.irk_req.idx );
            Connection_Idx = event->param.irk_req.idx;
            break;
        case RBLE_SM_CSRK_REQ_IND:          /* CSRK Request Evetnt */
            Connection_Idx = event->param.csrk_req.idx;
            printf( "\nrBLE SM EVENT(CSRK_REQ_IND)\n" );
            printf( "idx = %d", event->param.csrk_req.idx );
            printf( ",  signcnt = %ld,  ", event->param.csrk_req.signcnt );
            BdAddress_Disp( &event->param.csrk_req.addr );
            break;
        case RBLE_SM_KEY_IND:               /* Key Indicate Event */
            printf( "\nrBLE SM EVENT(KEY_IND)\n" );
            KeyDisp( &event->param.key_ind );
            Connection_Idx = event->param.key_ind.idx;
            if ( RBLE_SMP_KDIST_ENCKEY == event->param.key_ind.key_code )
            {
                if (RBLE_MASTER == Role_Status)
                {
                    /* Store Slave's LTK */
                    RBLE_APP_Key.EncKey = event->param.key_ind;
                    RBLE_APP_Key.Key_Set_Flags |= ( 1 << RBLE_APP_ENCKEY );
                }
            }
            else if ( RBLE_SMP_KDIST_IDKEY == event->param.key_ind.key_code )
            {
                RBLE_APP_Key.IrKey = event->param.key_ind;
                RBLE_APP_Key.Key_Set_Flags |= ( 1 << RBLE_APP_IDKEY );
            }
            else if ( RBLE_SMP_KDIST_SIGNKEY == event->param.key_ind.key_code )
            {
                RBLE_APP_Key.SignKey = event->param.key_ind;
                RBLE_APP_Key.Key_Set_Flags |= ( 1 << RBLE_APP_SIGNKEY );
            }
            break;
        case RBLE_SM_CHK_BD_ADDR_REQ:       /* BD Address Check Request Event */
            printf( "\nrBLE SM EVENT(BD_ADDR_REQ_IND)\n" );
            printf( "idx = %d", event->param.chk_bdaddr.idx );
            printf( ",  type = %d,  ", event->param.chk_bdaddr.type );
            Connection_Idx = event->param.chk_bdaddr.idx;
            Connection_Type = event->param.chk_bdaddr.type;
            BdAddress_Disp( &event->param.chk_bdaddr.addr );
            /* The remote device Bluetooth Address is saved. */
            Remote_Device = event->param.chk_bdaddr.addr;
            break;
        case RBLE_SM_TIMEOUT_EVT:
            printf( "\nrBLE SM EVENT(TIMEOUT)\n" );
            printf( "idx = %d\n", event->param.timeout_evt.idx );
            break;
        case RBLE_SM_EVENT_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE SM EVENT (COMMAND_DISALLOWED_IND) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "opcode = 0x%04x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A SM_Set_Key command is executed. 
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
static BOOL RBLE_SM_Set_Key_Test( void )
{
    const uint8_t Key_code = RBLE_KEY_DIST_IDKEY;       /* KeyCode */
    static RBLE_KEY_VALUE Key_Value = {
        0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF, 0x01, 0x23, 0x45, 0x67, 0x89, 0xAB, 0xCD, 0xEF
    };

    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_SM_Set_Key( Key_code, &Key_Value );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SM_Set_Key_Test command is executed. 
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
static BOOL RBLE_SM_Set_Key_Test_Csrk( void )
{
    const uint8_t Key_code = RBLE_KEY_DIST_SIGNKEY;     /* KeyCode */
    static RBLE_KEY_VALUE Key_Value = {
        0x34, 0xd4, 0x47, 0x97, 0xea, 0xa4, 0x2c, 0xae, 0x4a, 0xd9, 0x6e, 0x2a, 0xab, 0x99, 0xbc, 0x7c      /* NewKeyCsrkKey */
    };

    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_SM_Set_Key( Key_code, &Key_Value );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SM_Start_Enc command is executed. 
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
static BOOL RBLE_SM_Start_Enc_Test( void )
{
    const uint8_t auth_req = 0x01;      /* AuthReq */

    uint16_t            ediv;
    RBLE_RAND_NB        rand_nb;
    RBLE_KEY_VALUE      ltk;

    RBLE_STATUS Ret_Status;

    ediv = RBLE_APP_Key.EncKey.ediv;
    rand_nb = RBLE_APP_Key.EncKey.nb;
    ltk = RBLE_APP_Key.EncKey.ltk;
    /* API Call */
    Ret_Status = RBLE_SM_Start_Enc( Connection_Idx, auth_req, ediv, &rand_nb, &ltk );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SM_Tk_Req_Resp command is executed. 
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
static BOOL RBLE_SM_Tk_Req_Resp_Test( void )
{
    const uint8_t status = RBLE_OK;
    static RBLE_KEY_VALUE tk[] = {
        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFE, 0x12, 0x03, 0x6E, 0x5A, 0x88, 0x9F, 0x4D      /* TkKey(for OOB) */
        },

        {
            0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00      /* TkKey(for PassKey) */
        }
    };
    RBLE_KEY_VALUE      *Key_p;
    RBLE_STATUS         Ret_Status;
    int32_t             Parameter_No;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = ILLEGAL_NO;
    }

    if ( ( ILLEGAL_NO != Parameter_No ) && ( 0 != Parameter_No ) ) {
        Key_p = &tk[1];
        memset(Key_p->key, 0, RBLE_KEY_LEN);
        Key_p->key[13] = ( uint8_t )( ( Parameter_No & 0xff0000 ) >> 16 );
        Key_p->key[14] = ( uint8_t )( ( Parameter_No & 0x00ff00 ) >> 8 );
        Key_p->key[15] = ( uint8_t )( Parameter_No & 0x0000ff );
    } else {
        Key_p = &tk[0];
    }

    /* API Call */
    Ret_Status = RBLE_SM_Tk_Req_Resp( Connection_Idx, status, Key_p );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SM_Ltk_Req_Resp command is executed. 
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
static BOOL RBLE_SM_Ltk_Req_Resp_Test( void )
{
    uint8_t         sec_prop = 0x01;        /* SecProp */
    uint8_t         status = RBLE_ERR;
    uint16_t        ediv = 0;               /* Ediv */
    RBLE_RAND_NB    nb;                     /* Rand */
    RBLE_KEY_VALUE  ltk;                    /* LTK */
    RBLE_STATUS     Ret_Status;
    int_t           i;

    if (TRUE == LTK_Req_For_Enc_Flag)
    {
        if (RBLE_SLAVE == Role_Status)
        {
            /*** master initiate encryption ***/
            if ( 0u != ( RBLE_APP_Key.Key_Set_Flags & ( 1 <<  RBLE_APP_ENCKEY ) ) ) {
                /* check EDIV */
                if (Req_EDIV == RBLE_APP_Key.EncKey.ediv)
                {
                    status = RBLE_OK;
                }
            }
        }
        else
        {
            /*** slave requests security ***/
            if ( 0u != ( RBLE_APP_Key.Key_Set_Flags & ( 1 <<  RBLE_APP_ENCKEY ) ) ) {
                status = RBLE_OK;
            }
        }

        /* Stored LTK */
        if (RBLE_OK == status)
        {
            ediv = RBLE_APP_Key.EncKey.ediv;
            nb = RBLE_APP_Key.EncKey.nb;
            ltk = RBLE_APP_Key.EncKey.ltk;
        }
    }
    else
    {
        /***  Key Distribution Phase ***/

        /* Generate EDIV */
        ediv = (uint16_t)RBLE_Get_Random();
        /* Generate Rand */
        for ( i = 0; i <  RBLE_RAND_NB_LEN; i++ ) {
            nb.nb[ i ] = ( uint8_t )RBLE_Get_Random();
        }
        /* Generate LTK */
        for ( i = 0; i <  RBLE_KEY_LEN; i++ ) {
           ltk.key[ i ] = ( uint8_t )RBLE_Get_Random();
        }

        if (RBLE_SLAVE == Role_Status)
        {
            /* Store Own(Slave's) LTK */
            RBLE_APP_Key.Key_Set_Flags |= ( 1 << RBLE_APP_ENCKEY );
            RBLE_APP_Key.EncKey.ediv = ediv;
            RBLE_APP_Key.EncKey.nb = nb;
            RBLE_APP_Key.EncKey.ltk = ltk;
        }
        status = RBLE_OK;
    }
    printf("EDIV: 0x%04X\n", ediv);
    RandomDataDisp( &nb );
    KeyDataDisp( &ltk );

    /* API Call */
    Ret_Status = RBLE_SM_Ltk_Req_Resp( Connection_Idx, status, sec_prop, ediv, &nb, &ltk );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SM_Irk_Req_Resp command is executed. 
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
static BOOL RBLE_SM_Irk_Req_Resp_Test( void )
{
    uint8_t         status;
    RBLE_KEY_VALUE  *irk_p = &RBLE_APP_Key.IrKey.ltk;
    RBLE_STATUS     Ret_Status;
    uint8_t         lk_sec_status = RBLE_SMP_UNAUTHENTICATED | RBLE_SMP_BONDED;

    if ( 0u != ( RBLE_APP_Key.Key_Set_Flags & ( 1 <<  RBLE_APP_IDKEY ) ) ) {
        status = RBLE_OK;
    } else {
        /* Not Stored IRK */
        status = RBLE_ERR;
    }

    /* API Call */
    Ret_Status = RBLE_SM_Irk_Req_Resp( Connection_Idx, status, &Remote_Device, irk_p, lk_sec_status );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SM_Csrk_Req_Resp command is executed. 
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
static BOOL RBLE_SM_Csrk_Req_Resp_Test( void )
{
    uint8_t status;
    RBLE_KEY_VALUE  *csrk_p = &RBLE_APP_Key.SignKey.ltk;
    uint8_t lk_sec_status = RBLE_SMP_UNAUTHENTICATED | RBLE_SMP_BONDED;
    RBLE_STATUS Ret_Status;

    if ( 0u != ( RBLE_APP_Key.Key_Set_Flags & ( 1 <<  RBLE_APP_SIGNKEY ) ) ) {
        status = RBLE_OK;
    } else {
        /* Not Stored CSRK */
        status = RBLE_ERR;
    }

    /* API Call */
    Ret_Status = RBLE_SM_Csrk_Req_Resp( Connection_Idx, status, csrk_p, lk_sec_status );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A SM_Chk_Bd_Addr_Req_Resp command is executed. 
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
static BOOL RBLE_SM_Chk_Bd_Addr_Req_Resp_Test( void )
{
    const uint8_t found_flag = TRUE;
    const uint8_t lk_sec_status = ( RBLE_SMP_BONDED | RBLE_SMP_UNAUTHENTICATED );

    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_SM_Chk_Bd_Addr_Req_Resp( Connection_Idx, Connection_Type, found_flag, lk_sec_status, &Remote_Device );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/* ---------------------------------- GATT ------------------------------------------*/
/******************************************************************************************/
/**
 *  @brief rBLE GATT Call Back
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
static void RBLE_APP_GATT_CallBack( RBLE_GATT_EVENT *event )
{
    static RBLE_GATT_SET_DATA setdata;
    int_t       i;

    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_GATT_EVENT_DISC_SVC_ALL_CMP:
            printf( "\nrBLE GATT EVENT (DISC_SVC_ALL_CMP) Att_Code(%x), NbResp(%d)\n", event->param.disc_svc_all_cmp.att_code, event->param.disc_svc_all_cmp.nb_resp );
            for ( i = 0;i < event->param.disc_svc_all_cmp.nb_resp;i++ ) {
                printf( "conhdl:(%04X)\n", event->param.disc_svc_all_cmp.conhdl );
                printf( "start_hdl:(%04X)", event->param.disc_svc_all_cmp.list[ i ].start_hdl );
                printf( ", end_hdl:(%04X)", event->param.disc_svc_all_cmp.list[ i ].end_hdl );
                printf( ", UUID:(%04X)\n", event->param.disc_svc_all_cmp.list[ i ].attr_hdl );
            }
            break;
        case RBLE_GATT_EVENT_DISC_SVC_ALL_128_CMP:
            printf( "\nrBLE GATT EVENT (DISC_SVC_ALL_128_CMP) Att_Code(%x), NbResp(%d)\n", event->param.disc_svc_all_128_cmp.att_code, event->param.disc_svc_all_128_cmp.nb_resp );
            printf( "conhdl:(%04X)\n", event->param.disc_svc_all_128_cmp.conhdl );
            printf( "start_hdl:(%04X)", event->param.disc_svc_all_128_cmp.list.start_hdl );
            printf( ", end_hdl:(%04X), \n", event->param.disc_svc_all_128_cmp.list.end_hdl );
            printf( "UUID:(");
            Uuid128_Disp( &event->param.disc_svc_all_128_cmp.list.attr_hdl[ 0 ] );
            printf( ")\n"); 
            break;
        case RBLE_GATT_EVENT_DISC_SVC_BY_UUID_CMP:
            printf( "\nrBLE GATT EVENT (DISC_SVC_BY_UUID_CMP) Att_Code(%x), NbResp(%d)\n", event->param.disc_svc_by_uuid_cmp.att_code, event->param.disc_svc_by_uuid_cmp.nb_resp );
            for ( i = 0;i < event->param.disc_svc_by_uuid_cmp.nb_resp;i++ ) {
                printf( "conhdl:(%04X)\n", event->param.disc_svc_by_uuid_cmp.conhdl );
                printf( "start_hdl:(%04X)", event->param.disc_svc_by_uuid_cmp.list[ i ].start_hdl );
                printf( ", end_hdl:(%04X)\n", event->param.disc_svc_by_uuid_cmp.list[ i ].end_hdl );
            }
            break;
        case RBLE_GATT_EVENT_DISC_SVC_INCL_CMP:
            printf( "\nrBLE GATT EVENT (DISC_SVC_INCL_CMP) Nb_Entry(%d)\n", event->param.disc_svc_incl_cmp.nb_entry );
            printf( "conhdl:(%04X)\n", event->param.disc_svc_incl_cmp.conhdl );
            if ( RBLE_GATT_128BIT_UUID_OCTET == event->param.disc_svc_incl_cmp.entry_len ) {
                printf( "attr_hdl:(%04X),", event->param.disc_svc_incl_cmp.incl_list.incl.attr_hdl );
                printf( ", start_hdl:(%04X)", event->param.disc_svc_incl_cmp.incl_list.incl.start_hdl );
                printf( ", end_hdl:(%04X),\n", event->param.disc_svc_incl_cmp.incl_list.incl.end_hdl );
                printf( "UUID:(");
                Uuid128_Disp( &event->param.disc_svc_incl_cmp.incl_list.incl.uuid[ 0 ] );
                printf( ")\n"); 
            } else {
                for ( i = 0;i < event->param.disc_svc_incl_cmp.nb_entry;i++ ) {
                    printf( "attr_hdl:(%04X)", event->param.disc_svc_incl_cmp.incl_list.list[ i ].attr_hdl );
                    printf( ", start_hdl:(%04X)", event->param.disc_svc_incl_cmp.incl_list.list[ i ].start_hdl );
                    printf( ", end_hdl:(%04X)", event->param.disc_svc_incl_cmp.incl_list.list[ i ].end_hdl );
                    printf( ", UUID:(%04X)\n", event->param.disc_svc_incl_cmp.incl_list.list[ i ].uuid );
                }
            }
            break;
        case RBLE_GATT_EVENT_DISC_CHAR_ALL_CMP:
            printf( "\nrBLE GATT EVENT (DISC_CHAR_ALL_CMP) Att_Code(%x), NbResp(%d)\n", event->param.disc_char_all_cmp.att_code, event->param.disc_char_all_cmp.nb_entry );
            printf( "conhdl:(%04X)\n", event->param.disc_char_all_cmp.conhdl );
            for ( i = 0;i < event->param.disc_char_all_cmp.nb_entry;i++ ) {
                printf( "attribute_handle:(%04X)", event->param.disc_char_all_cmp.list[ i ].attr_hdl );
                printf( ",  prop:(%02X)", event->param.disc_char_all_cmp.list[ i ].prop );
                printf( ",  pointer_hdl:(%04X)", event->param.disc_char_all_cmp.list[ i ].pointer_hdl );
                printf( ",  uuid:(%04X)\n", event->param.disc_char_all_cmp.list[ i ].uuid );
            }
            break;
        case RBLE_GATT_EVENT_DISC_CHAR_ALL_128_CMP:
            printf( "\nrBLE GATT EVENT (DISC_CHAR_ALL_128_CMP) Att_Code(%x), NbEntry(%d)\n", event->param.disc_char_all_128_cmp.att_code, event->param.disc_char_all_128_cmp.nb_entry );
            printf( "conhdl:(%04X)\n", event->param.disc_char_all_128_cmp.conhdl );
            printf( "attribute_handle:(%04X)", event->param.disc_char_all_128_cmp.list.attr_hdl );
            printf( ",  prop:(%02X)", event->param.disc_char_all_128_cmp.list.prop );
            printf( ",  pointer_hdl:(%04X),\n", event->param.disc_char_all_128_cmp.list.pointer_hdl );
            printf( "UUID:(");
            Uuid128_Disp( &event->param.disc_char_all_128_cmp.list.uuid[ 0 ] );
            printf( ")\n"); 
            break;
        case RBLE_GATT_EVENT_DISC_CHAR_BY_UUID_CMP:
            printf( "\nrBLE GATT EVENT (DISC_CHAR_BY_UUID_CMP) Att_Code(%x), NbEntry(%d)\n", event->param.disc_char_by_uuid_cmp.att_code, event->param.disc_char_by_uuid_cmp.nb_entry );
            printf( "conhdl:(%04X)\n", event->param.disc_char_by_uuid_cmp.conhdl );
            for ( i = 0;i < event->param.disc_char_by_uuid_cmp.nb_entry;i++ ) {
                printf( "attribute_handle:(%04X)", event->param.disc_char_by_uuid_cmp.list[ i ].attr_hdl );
                printf( ",  prop:(%02X)", event->param.disc_char_by_uuid_cmp.list[ i ].prop );
                printf( ",  pointer_hdl:(%04X)", event->param.disc_char_by_uuid_cmp.list[ i ].pointer_hdl );
                printf( ",  uuid:(%04X)\n", event->param.disc_char_by_uuid_cmp.list[ i ].uuid );
            }
            break;
        case RBLE_GATT_EVENT_DISC_CHAR_BY_UUID_128_CMP:
            printf( "\nrBLE GATT EVENT (DISC_CHAR_BY_UUID_128_CMP) Att_Code(%x), NbEntry(%d)\n", event->param.disc_char_by_uuid_128_cmp.att_code, event->param.disc_char_by_uuid_128_cmp.nb_entry );
            printf( "conhdl:(%04X)\n", event->param.disc_char_by_uuid_128_cmp.conhdl );
            printf( "attribute_handle:(%04X)", event->param.disc_char_by_uuid_128_cmp.list.attr_hdl );
            printf( ",  prop:(%02X)", event->param.disc_char_by_uuid_128_cmp.list.prop );
            printf( ",  pointer_hdl:(%04X),\n", event->param.disc_char_by_uuid_128_cmp.list.pointer_hdl );
            printf( "UUID:(");
            Uuid128_Disp( &event->param.disc_char_by_uuid_128_cmp.list.uuid[ 0 ] );
            printf( ")\n"); 
            break;
        case RBLE_GATT_EVENT_DISC_CHAR_DESC_CMP:
            printf( "\nrBLE GATT EVENT (DISC_CHAR_DESC_CMP) NbEntry(%d)\n", event->param.disc_char_desc_cmp.nb_entry );
            printf( "conhdl:(%04X)\n", event->param.disc_char_desc_cmp.conhdl );
            for ( i = 0;i < event->param.disc_char_desc_cmp.nb_entry;i++ ) {
                printf( "attr_hdl:(%04X)", event->param.disc_char_desc_cmp.list[ i ].attr_hdl );
                printf( ", desc_hdl:(%04X)\n", event->param.disc_char_desc_cmp.list[ i ].desc_hdl );
            }
            break;
        case RBLE_GATT_EVENT_DISC_CHAR_DESC_128_CMP:
            printf( "\nrBLE GATT EVENT (DISC_CHAR_DESC_128_CMP) NbEntry(%d)\n", event->param.disc_char_desc_128_cmp.nb_entry );
            printf( "conhdl:(%04X)\n", event->param.disc_char_desc_128_cmp.conhdl );
            printf( "attr_hdl:(%04X),\n", event->param.disc_char_desc_128_cmp.list_128.attr_hdl );
            printf( "UUID:(");
            Uuid128_Disp( &event->param.disc_char_desc_128_cmp.list_128.uuid[ 0 ] );
            printf( ")\n"); 
            break;
        case RBLE_GATT_EVENT_READ_CHAR_RESP:
            printf( "\nrBLE GATT EVENT (READ_CHAR_RESP) Att_Code(%s)\n", Get_Att_Code_Str( event->param.read_char_resp.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.read_char_resp.conhdl );
            if ( RBLE_ATT_ERR_NO_ERROR == event->param.read_char_resp.att_code ) {
                if ( 0 != event->param.read_char_resp.data.each_len ) {
                    uint16_t    handle;
                    uint8_t     *p = &event->param.read_char_resp.data.data[ 0 ];
                    uint8_t     len = event->param.read_char_resp.data.len;

                    for ( i = 1; len != 0; i++ ) {
                        handle = ( p[ 0 ] | ( p[ 1 ] << 8 ) );
                        printf( "%d) Handle = %04X\n", i, handle );
                        printf( "Value:\n");
                        Print_Dump( &p[ 2 ], event->param.read_char_resp.data.each_len - sizeof( uint16_t ) );
                        len -= event->param.read_char_resp.data.each_len;
                        p = &p[ i * event->param.read_char_resp.data.each_len ];
                    }
                } else {
                    printf( "Data:\n");
                    Print_Dump( &event->param.read_char_resp.data.data[ 0 ], event->param.read_char_resp.data.len );
                }
            }
            break;
        case RBLE_GATT_EVENT_READ_CHAR_LONG_RESP:
            printf( "\nrBLE GATT EVENT (READ_CHAR_LONG_RESP) Att_Code(%s)\n", Get_Att_Code_Str( event->param.read_char_long_resp.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.read_char_long_resp.conhdl );
            if ( RBLE_ATT_ERR_NO_ERROR == event->param.read_char_long_resp.att_code ) {
                printf( "val_len:(%X)", event->param.read_char_long_resp.val_len );
                printf( ",  attr_hdl:(%04X),\n", event->param.read_char_long_resp.attr_hdl );
                printf( "Data:\n");
                Print_Dump( &event->param.read_char_long_resp.value[ 0 ], event->param.read_char_long_resp.val_len );
            }
            break;
        case RBLE_GATT_EVENT_READ_CHAR_MULT_RESP:
            printf( "\nrBLE GATT EVENT (READ_CHAR_MULT_RESP) Att_Code(%s)\n", Get_Att_Code_Str( event->param.read_char_mult_resp.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.read_char_mult_resp.conhdl );
            if ( RBLE_ATT_ERR_NO_ERROR == event->param.read_char_mult_resp.att_code ) {
                uint8_t len = event->param.read_char_mult_resp.val_len;

                printf( "val_len:(%X)\n", event->param.read_char_mult_resp.val_len );
                for ( i = 0 ; ((i < RBLE_GATT_MAX_NB_HDLS) && (len != 0)); i++ ) {
                    printf("%d) val_len:(%X)\n", i+1, event->param.read_char_mult_resp.data[ i ].len );
                    printf( "Data:\n");
                    Print_Dump( &event->param.read_char_mult_resp.data[ i ].value[ 0 ], event->param.read_char_mult_resp.data[ i ].len );
                    len -= event->param.read_char_mult_resp.data[ i ].len;
                }
            }
            break;
        case RBLE_GATT_EVENT_READ_CHAR_LONG_DESC_RESP:
            printf( "\nrBLE GATT EVENT (READ_CHAR_LONG_DESC_RESP) Att_Code(%s)\n", Get_Att_Code_Str( event->param.read_char_long_desc_resp.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.read_char_long_desc_resp.conhdl );
            if ( RBLE_ATT_ERR_NO_ERROR == event->param.read_char_long_desc_resp.att_code ) {
                printf( "val_len:(%X), ", event->param.read_char_long_desc_resp.val_len );
                printf( "attr_hdl:(%04X),\n", event->param.read_char_long_desc_resp.attr_hdl );
                printf( "Data:\n");
                Print_Dump( &event->param.read_char_long_desc_resp.value[ 0 ], event->param.read_char_long_desc_resp.val_len );
            }
            break;
        case RBLE_GATT_EVENT_WRITE_CHAR_RESP:
            printf( "\nrBLE GATT EVENT (WRITE_CHAR_RESP) Att_Code(%s)\n", Get_Att_Code_Str( event->param.write_char_resp.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.write_char_resp.conhdl );
            break;
        case RBLE_GATT_EVENT_WRITE_CHAR_RELIABLE_RESP:
            printf( "\nrBLE GATT EVENT (WRITE_CHAR_RELIABLE_RESP) Att_Code(%s)\n", Get_Att_Code_Str( event->param.write_reliable_resp.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.write_reliable_resp.conhdl );
            break;
        case RBLE_GATT_EVENT_CANCEL_WRITE_CHAR_RESP:
            printf( "\nrBLE GATT EVENT (CANCEL_WRITE_CHAR_RESP) Att_Code(%s)\n", Get_Att_Code_Str( event->param.cancel_write_resp.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.cancel_write_resp.conhdl );
            break;
        case RBLE_GATT_EVENT_HANDLE_VALUE_NOTIF:
            printf( "\nrBLE GATT EVENT (HANDLE_VALUE_NOTIF)\n");
            printf( "conhdl:(%04X), ", event->param.handle_value_notif.conhdl );
            printf( "charhdl:(%04X), ", event->param.handle_value_notif.charhdl );
            printf( "size:(%X),\n", event->param.handle_value_notif.size );
            printf( "Data:\n");
            Print_Dump( &event->param.handle_value_notif.value[ 0 ], event->param.handle_value_notif.size );
            break;
        case RBLE_GATT_EVENT_HANDLE_VALUE_IND:
            printf( "\nrBLE GATT EVENT (HANDLE_VALUE_IND)\n");
            printf( "conhdl:(%04X), ", event->param.handle_value_ind.conhdl );
            printf( "charhdl:(%04X), ", event->param.handle_value_ind.charhdl );
            printf( "size:(%X),\n", event->param.handle_value_ind.size );
            printf( "Data:\n");
            Print_Dump( &event->param.handle_value_ind.value[ 0 ], event->param.handle_value_ind.size );
            break;
        case RBLE_GATT_EVENT_HANDLE_VALUE_CFM:
            printf( "\nrBLE GATT EVENT (HANDLE_VALUE_CFM) Status(%s)\n", Get_Status_Str( event->param.handle_value_cfm.status ) );
            break;
        case RBLE_GATT_EVENT_DISCOVERY_CMP:
            printf( "\nrBLE GATT EVENT (DISCOVERY_CMP) Att_Code(%s)\n", Get_Att_Code_Str( event->param.discovery_cmp.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.discovery_cmp.conhdl );
            break;
        case RBLE_GATT_EVENT_COMPLETE:
            printf( "\nrBLE GATT EVENT (COMPLETE) Att_Code(%s)\n", Get_Att_Code_Str( event->param.complete.att_code ) );
            printf( "conhdl:(%04X)\n", event->param.complete.conhdl );
            break;
        case RBLE_GATT_EVENT_WRITE_CMD_IND:
            setdata.val_hdl = event->param.write_cmd_ind.elmt;
            setdata.val_len = event->param.write_cmd_ind.size + event->param.write_cmd_ind.offset;
            memcpy(&setdata.value[ event->param.write_cmd_ind.offset ], &event->param.write_cmd_ind.value[ 0 ], event->param.write_cmd_ind.size);
            RBLE_GATT_Set_Data( &setdata );

            printf( "\nrBLE GATT EVENT (WRITE_CMD_IND)\n");
            printf( "conhdl:(%04X)\n", event->param.write_cmd_ind.conhdl );
            printf( "elmt hdl:(%04X), ", event->param.write_cmd_ind.elmt );
            printf( "size:(%d), ", event->param.write_cmd_ind.size );
            printf( "offset:(%d), ", event->param.write_cmd_ind.offset );
            printf( "resp:(%s)\n", ( ( event->param.write_cmd_ind.resp == TRUE ) ? "TRUE" : "FALSE" ) );
            printf( "Data:\n");
            Print_Dump( &event->param.write_cmd_ind.value[ 0 ], event->param.write_cmd_ind.size );
            break;
        case RBLE_GATT_EVENT_RESP_TIMEOUT:
            printf( "\nrBLE GATT EVENT (RESP_TIMEOUT)\n");
            break;
        case RBLE_GATT_EVENT_SET_PERM_CMP:
            printf( "\nrBLE GATT EVENT (SET_PERM_CMP) Status(%s)\n", Get_Status_Str( event->param.set_perm_cmp.status ) );
            break;
        case RBLE_GATT_EVENT_SET_DATA_CMP:
            printf( "\nrBLE GATT EVENT (SET_DATA_CMP) Status(%s)\n", Get_Status_Str( event->param.set_data_cmp.status ) );
            break;
        case RBLE_GATT_EVENT_NOTIFY_COMP:
            printf( "\nrBLE GATT EVENT (NOTIFY_COMP) Status(%s)\n", Get_Status_Str( event->param.notify_cmp.status ) );
            printf( "conhdl:(%04X), ", event->param.notify_cmp.conhdl );
            printf( "charhdl:(%04X)\n", event->param.notify_cmp.charhdl );
            break;
        case RBLE_GATT_EVENT_COMMAND_DISALLOWED_IND:
            printf( "\nrBLE GATT EVENT (COMMAND_DISALLOWED_IND)\n");
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A GATT_Enable command is executed. 
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
static BOOL RBLE_GATT_Enable_Test( void )
{
    RBLE_STATUS Ret_Status;

    /* API Call */
    Ret_Status = RBLE_GATT_Enable( RBLE_APP_GATT_CallBack );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

static BOOL RBLE_GATT_Discovery_Service_Request_Test( void )
{
    const uint8_t       Value[ RBLE_GATT_128BIT_UUID_OCTET ] = {
        0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xA0, 0x00, 0x00
    };
    static RBLE_GATT_DISC_SVC_REQ   Disc_Svc;
    RBLE_STATUS             Ret_Status;
    int32_t                 Parameter_No;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = ILLEGAL_NO;
    }

    switch( Parameter_No ) {
        case 1:
            Select_Param_No_Disp( 1 );
            Disc_Svc.req_type = RBLE_GATT_DISC_BY_UUID_SVC;
            Disc_Svc.start_hdl = 0x0001;
            Disc_Svc.end_hdl = 0xffff;
            if ( ( 3 == RBLE_Parameter_Cnt ) && ( 128 == RBLE_Parameter_Arg[ 1 ] ) ) {
                Disc_Svc.desired_svc.value_size = RBLE_GATT_128BIT_UUID_OCTET;
                memcpy( &Disc_Svc.desired_svc.value[ 0 ], &Value[ 0 ], RBLE_GATT_128BIT_UUID_OCTET );
                Disc_Svc.desired_svc.value[ 12 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 0 ) & 0xff );
                Disc_Svc.desired_svc.value[ 13 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 8 ) & 0xff );
            } else {
                Disc_Svc.desired_svc.value_size = RBLE_GATT_16BIT_UUID_OCTET;
                Disc_Svc.desired_svc.value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
                Disc_Svc.desired_svc.value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
            }
            break;
        case 2:
            Select_Param_No_Disp( 2 );
            Disc_Svc.req_type = RBLE_GATT_DISC_INCLUDED_SVC;
            if ( 3 == RBLE_Parameter_Cnt ) {
                Disc_Svc.start_hdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
                Disc_Svc.end_hdl   = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
            } else {
                Disc_Svc.start_hdl = 0x0001;
                Disc_Svc.end_hdl   = 0xffff;
            }
            break;
        case 0:
        default:
            Select_Param_No_Disp( 0 );
            Disc_Svc.req_type = RBLE_GATT_DISC_ALL_SVC;
            Disc_Svc.desired_svc.value_size = RBLE_GATT_16BIT_UUID_OCTET;
            Disc_Svc.desired_svc.value[ 0 ] = 0x00;
            Disc_Svc.desired_svc.value[ 1 ] = 0x00;
            break;
    }

    Disc_Svc.conhdl = Connection_Handle;

    /* API Call */
    Ret_Status = RBLE_GATT_Discovery_Service_Request( &Disc_Svc );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A GATT_Discovery_Char_Request command is executed. 
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
static BOOL RBLE_GATT_Discovery_Char_Request_Test( void )
{
    const uint8_t       Value[ RBLE_GATT_128BIT_UUID_OCTET ] = {
        0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xA0, 0x00, 0x00
    };
    RBLE_GATT_DISC_CHAR_REQ Request_Data;
    RBLE_STATUS             Ret_Status;
    int32_t                 Parameter_No;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = ILLEGAL_NO;
    }

    switch( Parameter_No ) {
        case 1:
            Request_Data.req_type = RBLE_GATT_DISC_BY_UUID_CHAR;
            Request_Data.start_hdl = 0x0001;
            Request_Data.end_hdl = 0xffff;
            if ( ( 3 <= RBLE_Parameter_Cnt ) && ( 128 == RBLE_Parameter_Arg[ 1 ] ) ) {
                Request_Data.desired_char.value_size = RBLE_GATT_128BIT_UUID_OCTET;
                memcpy( &Request_Data.desired_char.value[ 0 ], &Value[ 0 ], RBLE_GATT_128BIT_UUID_OCTET );
                Request_Data.desired_char.value[ 12 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 0 ) & 0xff );
                Request_Data.desired_char.value[ 13 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 8 ) & 0xff );
                if ( 5 == RBLE_Parameter_Cnt ) {
                    Request_Data.start_hdl = ( uint16_t )RBLE_Parameter_Arg[ 3 ];
                    Request_Data.end_hdl = ( uint16_t )RBLE_Parameter_Arg[ 4 ];
                }
            } else {
                Request_Data.desired_char.value_size = RBLE_GATT_16BIT_UUID_OCTET;
                Request_Data.desired_char.value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
                Request_Data.desired_char.value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
                if ( 4 == RBLE_Parameter_Cnt ) {
                    Request_Data.start_hdl = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
                    Request_Data.end_hdl = ( uint16_t )RBLE_Parameter_Arg[ 3 ];
                }
            }
            break;
        case 2:
            Request_Data.req_type = RBLE_GATT_DISC_ALL_CHAR;
            if ( 3 == RBLE_Parameter_Cnt ) {
                Request_Data.start_hdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
                Request_Data.end_hdl = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
            } else {
                Request_Data.start_hdl = 0x0001;
                Request_Data.end_hdl = 0xffff;
            }
            break;
        case 0:
        default:
            Request_Data.req_type = RBLE_GATT_DISC_BY_UUID_CHAR;
            Request_Data.start_hdl = 0x0001;
            Request_Data.end_hdl = 0xffff;
            Request_Data.desired_char.value_size = RBLE_GATT_16BIT_UUID_OCTET;
            Request_Data.desired_char.value[ 0 ] = ( uint8_t )( ( Search_Attribute_Type >> 0 ) & 0xff );
            Request_Data.desired_char.value[ 1 ] = ( uint8_t )( ( Search_Attribute_Type >> 8 ) & 0xff );
            break;
    }

    Request_Data.conhdl = Connection_Handle;

    /* API Call */
    Ret_Status = RBLE_GATT_Discovery_Char_Request( &Request_Data );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

static BOOL RBLE_GATT_Discovery_Char_Descriptor_Request_Test( void )
{
    RBLE_GATT_DISC_CHAR_DESC_REQ    Disc_Char_Desc;
    RBLE_STATUS                     Ret_Status;

    if ( 2 == RBLE_Parameter_Cnt ) {
        Disc_Char_Desc.start_hdl = ( uint16_t )RBLE_Parameter_Arg[ 0 ];
        Disc_Char_Desc.end_hdl   = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
    } else {
        Disc_Char_Desc.start_hdl = 0x0001;
        Disc_Char_Desc.end_hdl = 0xffff;
    }
    Disc_Char_Desc.conhdl = Connection_Handle;

    /* API Call */
    Ret_Status = RBLE_GATT_Discovery_Char_Descriptor_Request( &Disc_Char_Desc );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Read_Char_Request_Test( void )
{
    const uint8_t       Value[ RBLE_GATT_128BIT_UUID_OCTET ] = {
        0xEF, 0xCD, 0xAB, 0x89, 0x67, 0x45, 0x23, 0x01, 0x00, 0x00, 0x00, 0x00, 0x0C, 0xA0, 0x00, 0x00
    };
    RBLE_GATT_READ_CHAR_REQ     Rd_Char;
    RBLE_STATUS                 Ret_Status;
    uint16_t                    Handle = 0x1234;
    int32_t                     Parameter_No;

    memset(&Rd_Char, 0, sizeof(Rd_Char));

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = ILLEGAL_NO;
    }

    switch( Parameter_No ) {
        case 0:
            Rd_Char.req_type = RBLE_GATT_READ_CHAR;
            /* char handle */
            Rd_Char.uuid[ 0 ].value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
            Rd_Char.uuid[ 0 ].value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
            Rd_Char.uuid[ 0 ].value_size = RBLE_GATT_16BIT_UUID_OCTET;
            Rd_Char.nb_uuid = 1;
            break;
        case 1:
            Rd_Char.req_type = RBLE_GATT_READ_BY_UUID_CHAR;
            Rd_Char.start_hdl = 0x0001;
            Rd_Char.end_hdl = 0xffff;
            if ( ( 3 <= RBLE_Parameter_Cnt ) && ( 128 == RBLE_Parameter_Arg[ 1 ] ) ) {
                Rd_Char.uuid[ 0 ].value_size = RBLE_GATT_128BIT_UUID_OCTET;
                memcpy( &Rd_Char.uuid[ 0 ].value[ 0 ], &Value[ 0 ], RBLE_GATT_128BIT_UUID_OCTET );
                Rd_Char.uuid[ 0 ].value[ 12 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 0 ) & 0xff );
                Rd_Char.uuid[ 0 ].value[ 13 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 8 ) & 0xff );
                if ( 5 == RBLE_Parameter_Cnt ) {
                    Rd_Char.start_hdl = ( uint16_t )RBLE_Parameter_Arg[ 3 ];
                    Rd_Char.end_hdl = ( uint16_t )RBLE_Parameter_Arg[ 4 ];
                }
            } else {
                Rd_Char.uuid[ 0 ].value_size = RBLE_GATT_16BIT_UUID_OCTET;
                Rd_Char.uuid[ 0 ].value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
                Rd_Char.uuid[ 0 ].value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
                if ( 4 == RBLE_Parameter_Cnt ) {
                    Rd_Char.start_hdl = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
                    Rd_Char.end_hdl = ( uint16_t )RBLE_Parameter_Arg[ 3 ];
                }
            }
            Rd_Char.nb_uuid = 1;
            break;
        case 2:
            Rd_Char.req_type = RBLE_GATT_READ_LONG_CHAR;
            Rd_Char.offset = 0x0000;
            /* char handle */
            Rd_Char.uuid[ 0 ].value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
            Rd_Char.uuid[ 0 ].value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
            if ( 3 == RBLE_Parameter_Cnt ) {
                Rd_Char.offset = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
            }
            Rd_Char.uuid[ 0 ].value_size = RBLE_GATT_16BIT_UUID_OCTET;
            Rd_Char.nb_uuid = 1;
            break;
        case 3:
            Rd_Char.req_type = RBLE_GATT_READ_MULT_LONG_CHAR;
            Rd_Char.uuid[ 0 ].value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
            Rd_Char.uuid[ 0 ].value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
            Rd_Char.uuid[ 1 ].value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 0 ) & 0xff );
            Rd_Char.uuid[ 1 ].value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 8 ) & 0xff );

            Rd_Char.uuid[ 0 ].expect_resp_size = 1;
            Rd_Char.uuid[ 0 ].value_size = RBLE_GATT_16BIT_UUID_OCTET;
            Rd_Char.uuid[ 1 ].expect_resp_size = RBLE_GATT_LEN_UNDEF;
            Rd_Char.uuid[ 1 ].value_size = RBLE_GATT_16BIT_UUID_OCTET;

            Rd_Char.nb_uuid = 2;
            break;
        case 4:
            Rd_Char.req_type = RBLE_GATT_READ_DESC;
            /* char handle */
            Rd_Char.uuid[ 0 ].value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
            Rd_Char.uuid[ 0 ].value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
            Rd_Char.uuid[ 0 ].value_size = RBLE_GATT_16BIT_UUID_OCTET;
            Rd_Char.nb_uuid = 1;
            break;
        case 5:
            Rd_Char.req_type = RBLE_GATT_READ_LONG_DESC;
            Rd_Char.offset = 0x0000;
            /* char handle */
            Rd_Char.uuid[ 0 ].value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
            Rd_Char.uuid[ 0 ].value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
            if ( 3 == RBLE_Parameter_Cnt ) {
                Rd_Char.offset = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
            }
            Rd_Char.uuid[ 0 ].value_size = RBLE_GATT_16BIT_UUID_OCTET;
            Rd_Char.nb_uuid = 1;
            break;
        default:
            Rd_Char.req_type = RBLE_GATT_READ_CHAR;
            /* char handle */
            Rd_Char.uuid[ 0 ].value[ 0 ] = ( uint8_t )( ( Handle >> 0 ) & 0xff );
            Rd_Char.uuid[ 0 ].value[ 1 ] = ( uint8_t )( ( Handle >> 8 ) & 0xff );
            Rd_Char.uuid[ 0 ].value_size = RBLE_GATT_16BIT_UUID_OCTET;
            Rd_Char.nb_uuid = 1;
            break;
    }

    Rd_Char.conhdl = Connection_Handle;

    /* API Call */
    Ret_Status = RBLE_GATT_Read_Char_Request( &Rd_Char );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Write_Char_Request_Test( void )
{
    RBLE_GATT_WRITE_CHAR_REQ    Wr_Char;
    RBLE_STATUS                 Ret_Status;
    int32_t                     Parameter_No;
    uint8_t                     Data;
    uint16_t                    i;
    uint16_t                    len = 0;
    uint16_t                    Value = 0x1234;
    uint8_t                     Not_Use_IncData= 0;

    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = ILLEGAL_NO;
    }

    switch( Parameter_No ) {
        case 0:
            Wr_Char.req_type = RBLE_GATT_WRITE_NO_RESPONSE;
            Wr_Char.charhdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
            if ( 3 == RBLE_Parameter_Cnt ) {
                len = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
            } else {
                len = 1;
            }
            break;
        case 1:
            Wr_Char.req_type = RBLE_GATT_WRITE_SIGNED;
            Wr_Char.charhdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
            if ( 3 <= RBLE_Parameter_Cnt ) {
                len = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
                if ( 4 == RBLE_Parameter_Cnt ) {
                    Wr_Char.value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 3 ] >> 0 ) & 0xff );
                    Wr_Char.value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 3 ] >> 8 ) & 0xff );
                    Not_Use_IncData= 1;
                }
            } else {
                len = 1;
            }
            break;
        case 2:
            Wr_Char.req_type = RBLE_GATT_WRITE_CHAR;
            Wr_Char.charhdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
            if ( 3 == RBLE_Parameter_Cnt ) {
                len = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
            } else {
                len = 1;
            }
            break;
        case 3:
            Wr_Char.req_type = RBLE_GATT_WRITE_LONG_CHAR;
            Wr_Char.charhdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
            len = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
            Wr_Char.wr_offset = ( uint16_t )RBLE_Parameter_Arg[ 3 ];
            Wr_Char.auto_execute = ( uint8_t )RBLE_Parameter_Arg[ 4 ];
            break;
        case 4:
            Wr_Char.req_type = RBLE_GATT_WRITE_DESC;
            Wr_Char.charhdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
            if ( 3 <= RBLE_Parameter_Cnt ) {
                len = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
                if ( 4 == RBLE_Parameter_Cnt ) {
                    Wr_Char.value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 3 ] >> 0 ) & 0xff );
                    Wr_Char.value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 3 ] >> 8 ) & 0xff );
                    Not_Use_IncData= 1;
                }
            } else {
                len = 1;
            }
            break;
        case 5:
            Wr_Char.req_type = RBLE_GATT_WRITE_LONG_DESC;
            Wr_Char.charhdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
            len = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
            Wr_Char.wr_offset = ( uint16_t )RBLE_Parameter_Arg[ 3 ];
            Wr_Char.auto_execute = ( uint8_t )RBLE_Parameter_Arg[ 4 ];
            break;
        default:
            Wr_Char.req_type = RBLE_GATT_WRITE_NO_RESPONSE;
            Wr_Char.charhdl = 0x1234;
            break;
    }

    Wr_Char.conhdl = Connection_Handle;

    if ( 0 == len ) {
        Wr_Char.val_len = 2;
        Wr_Char.value[ 0 ] = ( uint8_t )( ( Value >> 0 ) & 0xff );
        Wr_Char.value[ 1 ] = ( uint8_t )( ( Value >> 8 ) & 0xff );
    } else {
        if ( 0 == Not_Use_IncData ) {
            for ( i = 0, Data = 0; i < len; i++, Data++ ) {
                Wr_Char.value[ i ] = Data;
            }
        }
        Wr_Char.val_len = len;
    }

    /* API Call */
    Ret_Status = RBLE_GATT_Write_Char_Request( &Wr_Char );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Write_Reliable_Request_Test( void )
{
    static RBLE_GATT_WRITE_RELIABLE_REQ     Rel_Write;
    RBLE_STATUS                             Ret_Status;
    uint16_t                                Total_Size, Remain;
    uint16_t                                Handle;
    uint16_t                                i, j;
    uint8_t                                 Data = 0;

    memset( &Rel_Write, 0, sizeof( RBLE_GATT_WRITE_RELIABLE_REQ ) );

    if ( 3 == RBLE_Parameter_Cnt ) {
        Handle       = ( uint16_t )RBLE_Parameter_Arg[ 0 ];
        Total_Size   = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
        Rel_Write.auto_execute = ( uint8_t )RBLE_Parameter_Arg[ 2 ];
        Remain = Total_Size;
        Rel_Write.nb_writes = 0;

        for ( i = 0; (( i < RBLE_GATT_MAX_RELIABLE_WRITE_NUM ) && ( Remain != 0 )); i++ ) {
            if ( RBLE_GATT_MAX_RELIABLE_WRITE_CONTENTS < Remain ) {
                Rel_Write.value[ i ].size = RBLE_GATT_MAX_RELIABLE_WRITE_CONTENTS;
            } else {
                Rel_Write.value[ i ].size = Remain;
            }
            Remain -= Rel_Write.value[ i ].size;
            Rel_Write.nb_writes++;
            Rel_Write.value[ i ].elmt_hdl = Handle;
            for ( j = 0; j < Rel_Write.value[ i ].size; j++, Data++ ) {
                Rel_Write.value[ i ].value[ j ] = Data;
            }
        }
    } else {
        Rel_Write.nb_writes = 1;
        Rel_Write.auto_execute = TRUE;
        Rel_Write.value[ 0 ].elmt_hdl = 0x1234;
        Rel_Write.value[ 0 ].size = 1;
        Rel_Write.value[ 0 ].value[ 0 ] = 0x34;
        Rel_Write.value[ 0 ].value[ 1 ] = 0x12;
    }

    Rel_Write.conhdl = Connection_Handle;

    /* API Call */
    Ret_Status = RBLE_GATT_Write_Reliable_Request( &Rel_Write );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Execute_Write_Char_Request_Test( void )
{
    RBLE_GATT_EXE_WR_CHAR_REQ   Exe_Wr_Char;
    RBLE_STATUS                 Ret_Status;
    int32_t                     Parameter_No;


    if ( 0 != RBLE_Parameter_Cnt ) {
        Parameter_No = RBLE_Parameter_Arg[ 0 ];
    } else {
        Parameter_No = ILLEGAL_NO;
    }

    switch( Parameter_No ) {
        case 1:
            Exe_Wr_Char.exe_wr_ena = TRUE;
            break;
        case 0:
        default:
            Exe_Wr_Char.exe_wr_ena = FALSE;
            break;
    }

    Exe_Wr_Char.conhdl = Connection_Handle;

    /* API Call */
    Ret_Status = RBLE_GATT_Execute_Write_Char_Request( &Exe_Wr_Char );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Notify_Request_Test( void )
{
    RBLE_GATT_NOTIFY_REQ    Notify;
    RBLE_STATUS             Ret_Status;
    uint16_t                hdl = 0x0001;

    if ( 0 != RBLE_Parameter_Cnt ) {
        hdl = ( uint16_t )RBLE_Parameter_Arg[ 0 ];
    }
    Notify.conhdl = Connection_Handle;
    Notify.charhdl = hdl;

    /* API Call */
    Ret_Status = RBLE_GATT_Notify_Request( &Notify );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Indicate_Request_Test( void )
{
    RBLE_GATT_INDICATE_REQ  Indicate;
    RBLE_STATUS             Ret_Status;
    uint16_t                hdl = 0x0001;

    if ( 0 != RBLE_Parameter_Cnt ) {
        hdl = ( uint16_t )RBLE_Parameter_Arg[ 0 ];
    }
    Indicate.conhdl = Connection_Handle;
    Indicate.charhdl = hdl;

    /* API Call */
    Ret_Status = RBLE_GATT_Indicate_Request( &Indicate );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Write_Response_Test( void )
{
    RBLE_GATT_WRITE_RESP    Wr_Resp;
    RBLE_STATUS             Ret_Status;

    if ( 2 == RBLE_Parameter_Cnt ) {
        Wr_Resp.att_hdl = ( uint16_t )RBLE_Parameter_Arg[ 0 ];
        Wr_Resp.att_code = ( uint8_t )RBLE_Parameter_Arg[ 1 ];
    } else {
        Wr_Resp.att_hdl = 0x0001;
        Wr_Resp.att_code = RBLE_ATT_ERR_NO_ERROR;
    }
    Wr_Resp.conhdl = Connection_Handle;

    /* API Call */
    Ret_Status = RBLE_GATT_Write_Response( &Wr_Resp );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Set_Permission_Test( void )
{
    RBLE_GATT_SET_PERM      Set_Perm;
    RBLE_STATUS             Ret_Status;

    if ( 3 == RBLE_Parameter_Cnt ) {
        Set_Perm.start_hdl = ( uint16_t )RBLE_Parameter_Arg[ 0 ];
        Set_Perm.end_hdl = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
        Set_Perm.perm = ( uint16_t )RBLE_Parameter_Arg[ 2 ];
    } else {
        Set_Perm.start_hdl = 0x0001;
        Set_Perm.end_hdl = 0x0001;
        Set_Perm.perm = RBLE_GATT_PERM_RD;
    }

    /* API Call */
    Ret_Status = RBLE_GATT_Set_Permission( &Set_Perm );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

static BOOL RBLE_GATT_Set_Data_Test( void )
{
    RBLE_GATT_SET_DATA      Set_Data;
    RBLE_STATUS             Ret_Status;

    if ( 3 == RBLE_Parameter_Cnt ) {
        Set_Data.val_hdl = ( uint16_t )RBLE_Parameter_Arg[ 0 ];
        Set_Data.val_len = ( uint16_t )RBLE_Parameter_Arg[ 1 ];
        Set_Data.value[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 0  ) & 0xff );
        Set_Data.value[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 8  ) & 0xff );
        Set_Data.value[ 2 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 16 ) & 0xff );
        Set_Data.value[ 3 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 2 ] >> 24 ) & 0xff );
    } else {
        Set_Data.val_hdl = 0x0001;
        Set_Data.val_len = 0x0001;
        Set_Data.value[ 0 ] = 0x01;
    }

    /* API Call */
    Ret_Status = RBLE_GATT_Set_Data( &Set_Data );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
