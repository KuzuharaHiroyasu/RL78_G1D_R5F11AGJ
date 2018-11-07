/**
 ****************************************************************************************
 *
 * @file        rble_sample_app.c
 *
 * @brief   rBLE Sample Application
 *
 * Copyright(C) 2013-2015 Renesas Electronics Corporation
 *
 * $Rev: 9939 $
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

#ifdef USE_CUSTOM_DEMO
#include    "push_sw.h"
#endif

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
static void RBLE_App_Test_Start( void );            /* Test Start( Menu Select ) */
static BOOL RBLE_APP_Exit( void );
static void RBLE_APP_CallBack(RBLE_MODE mode );


#ifdef USE_CUSTOM_DEMO
extern BOOL RBLE_GAP_Reset_Test(void);
extern void sw_int( void );
#endif

#if (defined(USE_FW_UPDATE_PROFILE) && defined(WIN32))
extern BOOL RBLE_FW_UPDATE_Test( void );
#endif

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */
uint32_t    RBLE_Parameter_Arg[ RBLE_PARAMETER_MAX ];
uint_t      RBLE_Parameter_Cnt;
BOOL        Exit_Flg;

extern const SELECT_MENU Test_Prog_GAP_SM_GATT_Select_Menu;
extern const SELECT_MENU Test_Prog_VS_Select_Menu;
extern const SELECT_MENU Test_Prog_FMP_Select_Menu;
extern const SELECT_MENU Test_Prog_PXP_Select_Menu;
extern const SELECT_MENU Test_Prog_HTP_Select_Menu;
extern const SELECT_MENU Test_Prog_BLP_Select_Menu;
extern const SELECT_MENU Test_Prog_HOGP_Select_Menu;
extern const SELECT_MENU Test_Prog_SCPP_Select_Menu;
extern const SELECT_MENU Test_Prog_HRP_Select_Menu;
extern const SELECT_MENU Test_Prog_CSCP_Select_Menu;
extern const SELECT_MENU Test_Prog_GLP_Select_Menu;
extern const SELECT_MENU Test_Prog_CPP_Select_Menu;
extern const SELECT_MENU Test_Prog_TIP_Select_Menu;
extern const SELECT_MENU Test_Prog_ANP_Select_Menu;
extern const SELECT_MENU Test_Prog_LNP_Select_Menu;
extern const SELECT_MENU Test_Prog_PASP_Select_Menu;
extern const SELECT_MENU Test_Prog_RSCP_Select_Menu;
extern const SELECT_MENU Test_Prog_SCP_Select_Menu;


/* Profile Select Menu */
static const MENU_INFO  Test_Prog_Profile_Menu[] = {
#if (PRF_SEL_FMPL || PRF_SEL_FMPT)
    {  1,       NULL,       &Test_Prog_FMP_Select_Menu,     "1.Find Me Profile\n",                      },
#endif
#if (PRF_SEL_HTPC || PRF_SEL_HTPT)
    {  2,       NULL,       &Test_Prog_HTP_Select_Menu,     "2.Health Thermometer Profile\n",           },
#endif
#if (PRF_SEL_PXPM || PRF_SEL_PXPR)
    {  3,       NULL,       &Test_Prog_PXP_Select_Menu,     "3.Proximity Profile\n",                    },
#endif
#if (PRF_SEL_BLPC || PRF_SEL_BLPS)
    {  4,       NULL,       &Test_Prog_BLP_Select_Menu,     "4.Blood Pressure Profile\n",               },
#endif
#if (PRF_SEL_HGHD || PRF_SEL_HGBH || PRF_SEL_HGRH)
    {  5,       NULL,       &Test_Prog_HOGP_Select_Menu,    "5.HID over GATT Profile\n",                },
#endif
#if (PRF_SEL_SPPC || PRF_SEL_SPPS)
    {  6,       NULL,       &Test_Prog_SCPP_Select_Menu,    "6.Scan Parameters Profile\n",              },
#endif
#if (PRF_SEL_HRPC || PRF_SEL_HRPS)
    {  7,       NULL,       &Test_Prog_HRP_Select_Menu,     "7.Heart Rate Profile\n",                   },
#endif
#if (PRF_SEL_CSCC || PRF_SEL_CSCS)
    {  8,       NULL,       &Test_Prog_CSCP_Select_Menu,    "8.Cycling Speed Profile\n",                },
#endif
#if (PRF_SEL_GLPC || PRF_SEL_GLPS)
    {  9,       NULL,       &Test_Prog_GLP_Select_Menu,     "9.Glucose Profile\n",                      },
#endif
#if (PRF_SEL_CPPC || PRF_SEL_CPPS)
    { 10,       NULL,       &Test_Prog_CPP_Select_Menu,     "10.Cycling Power Profile\n",               },
#endif
#if defined(USE_SAMPLE_PROFILE)
    { 11,       NULL,       &Test_Prog_SCP_Select_Menu,     "11.Sample Custom Profile\n",               },
#endif
#if (PRF_SEL_TIPC || PRF_SEL_TIPS)
    { 12,       NULL,       &Test_Prog_TIP_Select_Menu,     "12.Time Profile\n",                        },
#endif
#if (PRF_SEL_ANPC || PRF_SEL_ANPS)
    { 13,       NULL,       &Test_Prog_ANP_Select_Menu,     "13.Alert Notification Profile\n",          },
#endif
#if (PRF_SEL_LNPC || PRF_SEL_LNPS)
    { 14,       NULL,       &Test_Prog_LNP_Select_Menu,     "14.Location and Navigation Profile\n",     },
#endif
#if (PRF_SEL_PASC || PRF_SEL_PASS)
    { 15,       NULL,       &Test_Prog_PASP_Select_Menu,    "15.Phone Alert Status Profile\n",          },
#endif
#if (PRF_SEL_RSCC || PRF_SEL_RSCS)
    { 16,       NULL,       &Test_Prog_RSCP_Select_Menu,    "16.Running Speed and Cadence Profile\n",   },
#endif
    { ESC_KEY,  Menu_Exit,  NULL,                           "ESC Key: Menu exit\n",                     },
    { 0,        NULL,       NULL,                            NULL,                                      }
};

static  const SELECT_MENU   Test_Prog_Profile_Select_Menu = {
    "\n-- BLE Sample Program Profile Test Menu --",
    Test_Prog_Profile_Menu
};

/* Sample Program Select Menu */
static const MENU_INFO  Test_Prog_Menu[] = {
    {  1,       NULL,                   &Test_Prog_GAP_SM_GATT_Select_Menu, "1.GAP & SM & GATT Test\n", },
    {  2,       NULL,                   &Test_Prog_Profile_Select_Menu,     "2.Profile Test\n",         },
    {  3,       NULL,                   &Test_Prog_VS_Select_Menu,          "3.Vendor Specific Test\n", },
#if defined(USE_FW_UPDATE_PROFILE)&&defined(WIN32)
    {  5,       RBLE_FW_UPDATE_Test,    NULL,                               "5.FW Update Start\n",      },
#endif
    {  ESC_KEY, RBLE_APP_Exit,          NULL,                               "ESC Key: Menu exit\n",     },
    {  0,   NULL,                       NULL,                               NULL,                       }
};

static  const SELECT_MENU   Test_Prog_Select_Menu = {
    "\n-- BLE Sample Program Menu --",
    Test_Prog_Menu
};

/******************************* Program Area *****************************************/
/******************************************************************************************/
/**
 *  @brief rBLE APP Call Back
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] mode      RBLE Condition
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_CallBack( RBLE_MODE mode )
{
    switch(mode){
        case RBLE_MODE_INITIALIZE:
            break;
        case RBLE_MODE_ACTIVE:
            RBLE_App_Test_Start();
            break;
        case RBLE_MODE_RESET:
            Menu_Select_Terminate();
            break;
        case RBLE_MODE_SLEEP:
            break;
        case RBLE_MODE_ERROR:
            RBLE_APP_Exit();
            break;
        default:
            break;
    }
    Console_SetTextAttribute( EVENT_COLOR );
    switch(mode){
        case RBLE_MODE_INITIALIZE:
            printf( "rBLE Mode (INITALIZE)\n" );
            break;
        case RBLE_MODE_ACTIVE:
            printf( "rBLE Mode (ACTIVE)\n" );
#ifdef USE_CUSTOM_DEMO
            /* GAP Reset */
            RBLE_GAP_Reset_Test();
#endif
            break;
        case RBLE_MODE_RESET:
            printf( "rBLE Mode (MODE RESET)\n" );
            break;
        case RBLE_MODE_SLEEP:
            printf( "rBLE Mode (SLEEP)\n" );
            break;
        case RBLE_MODE_ERROR:
            printf( "rBLE Mode (ERROR)\n" );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/**************************************************************************************/
/**
 *  @brief rBLE  APP Initialize
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]  none
 *
 *  @retval
 */
/******************************************************************************************/
BOOL RBLE_App_Init( void )
{
    BOOL        Ret;
    RBLE_STATUS Ret_Status;

    Ret = Menu_Select_Init();
    if ( TRUE == Ret ) {
        /* Initialize rBLE */
        Ret_Status = RBLE_Init(&RBLE_APP_CallBack);
        if ( RBLE_OK != Ret_Status ) {
            Ret = FALSE;
            if ( RBLE_ERR == Ret_Status ) {
                printf( "COM Port error\n" );
            }
        }
#ifdef USE_CUSTOM_DEMO
        push_sw2_start( &sw_int );
#endif
    }
    return( Ret );
}

/******************************************************************************************/
/**
 *  @brief rBLE  Application Test Start
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval
 */
/******************************************************************************************/
void RBLE_App_Test_Start( void )
{
    Menu_Select( &Test_Prog_Select_Menu );
}

/******************************************************************************************/
/**
 *  @brief rBLE  Application Test Exit
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval FALSE     Menu Exit
 */
/******************************************************************************************/
static BOOL RBLE_APP_Exit( void )
{
    Menu_Select_Terminate();
    Exit_Flg = TRUE;
    return( FALSE );
}


/******************************************************************************************/
/**
 *  @brief A Bluetooth address is displayed on a monitor. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *Addr_p     Pointer to Bluetooth Address 
 *
 *  @retval
 */
/******************************************************************************************/
void BdAddress_Disp( RBLE_BD_ADDR *Addr_p )
{

    printf("Addr[%02x:%02x:%02x:%02x:%02x:%02x]\n", Addr_p->addr[ 5 ], Addr_p->addr[ 4 ], Addr_p->addr[ 3 ], 
                                                    Addr_p->addr[ 2 ], Addr_p->addr[ 1 ], Addr_p->addr[ 0 ]);
}

/******************************************************************************************/
/**
 *  @brief An Advertising report is displayed on a monitor.
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *Report_p     Pointer to Advertising report
 *
 *  @retval
 */
/******************************************************************************************/
void AdvRepot_Disp( RBLE_ADV_REPORT *Report_p )
{
    int_t       i;

    printf( "EventType(0x%x), AddressType(0x%x)\n", Report_p->evt_type, Report_p->adv_addr_type );
    BdAddress_Disp( &Report_p->adv_addr );
    printf( "Data(0x%x)\n", Report_p->data_len );
    for ( i = 0;i <  Report_p->data_len;i++ ) {
        if ( 0 != i ) {
            if ( 0 == ( i % 8 ) ) {
                printf( "\n" );
            } else {
                printf( "," );
            }
        }
        printf( "0x%02x", Report_p->data[ i ] );
    }
    printf( "\nRSSI(%d)\n", (int8_t)(Report_p->rssi) );
}


void Uuid128_Disp( uint8_t *p )
{
    printf("%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X", 
                p[15], p[14], p[13], p[12],
                p[11], p[10],
                p[9], p[8],
                p[7], p[6],
                p[5], p[4], p[3], p[2], p[1], p[0]);
}

int_t RBLE_Get_Random( void )
{
    return( rand() );
}


/******************************************************************************************/
/**
 *  @brief A rBLE Status is displayed on a monitor.
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] Status     rBLE Status
 *
 *  @retval
 */
/******************************************************************************************/
void RBLE_Status_Disp( RBLE_STATUS Status )
{
    printf( "Status(%s)\n",Get_Status_Str( Status ) );
}

/******************************************************************************************/
/**
 *  @brief Key Indicate Event Parameter is displayed on a monitor.
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *Key_p     Pointer to Key Indicate Event Parameter
 *
 *  @retval
 */
/******************************************************************************************/
void KeyDisp( struct RBLE_EVT_SM_Key_t *Key_p )
{
    const char_t *Key_Code_Str[] = {
        "No Keys",
        "Encryption key",
        "IRK (ID key)",
        "",
        "CSRK(Signature key)"
    };

    printf( "idx = %d", Key_p->idx );
    printf( ",  ediv = %d", Key_p->ediv );
    printf( ",  key_code = %s\n", Key_Code_Str[ Key_p->key_code ] );
    RandomDataDisp( &Key_p->nb );
    KeyDataDisp( &Key_p->ltk );
}

/******************************************************************************************/
/**
 *  @brief Random Data is displayed on a monitor.
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *p     Pointer to Random Data
 *
 *  @retval
 */
/******************************************************************************************/
void RandomDataDisp( RBLE_RAND_NB *p )
{
    int32_t         i;

    printf( "RandamData:" );
    for ( i = 0;i <  RBLE_RAND_NB_LEN;i++ ) {
        if ( 0 != i ) {
            if ( 0 == ( i % 16 ) ) {
                printf( "\n" );
            } else {
                printf( "," );
            }
        }
        printf( "%02x", p->nb[ i ] );
    }
    printf( "\n" );
}

/******************************************************************************************/
/**
 *  @brief Key Data is displayed on a monitor.
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *p     Pointer to Key Data
 *
 *  @retval
 */
/******************************************************************************************/
void KeyDataDisp( RBLE_KEY_VALUE *p )
{
    int32_t         i;
    printf( "\nKeyData:" );
    for ( i = 0;i <  RBLE_KEY_LEN;i++ ) {
        if ( 0 != i ) {
            if ( 0 == ( i % 16 ) ) {
                printf( "\n" );
            } else {
                printf( "," );
            }
        }
        printf( "%02x", p->key[ i ] );
    }
    printf( "\n" );
}

/******************************************************************************************/
/**
 *  @brief rBLE Status is changed into a character string. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] Status      rBLE Status
 *
 *  @retval Pointer to character string
 */
/******************************************************************************************/
char_t  *Get_Status_Str( RBLE_STATUS Status )
{
    char_t *p;
    switch ( Status ) {
        case RBLE_OK:                                   p = "OK"; break;
        case RBLE_UNKNOWN_HCI_COMMAND:                  p = "UNKNOWN_HCI_COMMAND"; break;
        case RBLE_UNKNOWN_CONNECTION_ID:                p = "UNKNOWN_CONNECTION_ID"; break;
        case RBLE_HARDWARE_FAILURE:                     p = "HARDWARE_FAILURE"; break;
        case RBLE_PAGE_TIMEOUT:                         p = "PAGE_TIMEOUT"; break;
        case RBLE_AUTH_FAILURE:                         p = "AUTH_FAILURE"; break;
        case RBLE_PIN_MISSING:                          p = "PIN_MISSING"; break;
        case RBLE_MEMORY_CAPA_EXCEED:                   p = "MEMORY_CAPA_EXCEED"; break;
        case RBLE_CON_TIMEOUT:                          p = "CON_TIMEOUT"; break;
        case RBLE_CON_LIMIT_EXCEED:                     p = "CON_LIMIT_EXCEED"; break;
        case RBLE_COMMAND_DISALLOWED:                   p = "COMMAND_DISALLOWED"; break;
        case RBLE_CONN_REJ_LIMITED_RESOURCES:           p = "CONN_REJ_LIMITED_RESOURCES"; break;
        case RBLE_CONN_REJ_SECURITY_REASONS:            p = "CONN_REJ_SECURITY_REASONS"; break;
        case RBLE_CONN_REJ_UNACCEPTABLE_BDADDR:         p = "CONN_REJ_UNACCEPTABLE_BDADDR"; break;
        case RBLE_CONN_ACCEPT_TIMEOUT_EXCEED:           p = "CONN_ACCEPT_TIMEOUT_EXCEED"; break;
        case RBLE_UNSUPPORTED:                          p = "UNSUPPORTED"; break;
        case RBLE_INVALID_HCI_PARAM:                    p = "INVALID_HCI_PARAM"; break;
        case RBLE_REMOTE_USER_TERM_CON:                 p = "REMOTE_USER_TERM_CON"; break;
        case RBLE_REMOTE_DEV_TERM_LOW_RESOURCES:        p = "REMOTE_DEV_TERM_LOW_RESOURCES"; break;
        case RBLE_REMOTE_DEV_POWER_OFF:                 p = "REMOTE_DEV_POWER_OFF"; break;
        case RBLE_CON_TERM_BY_LOCAL_HOST:               p = "CON_TERM_BY_LOCAL_HOST"; break;
        case RBLE_REPEATED_ATTEMPTS:                    p = "REPEATED_ATTEMPTS"; break;
        case RBLE_PAIRING_NOT_ALLOWED:                  p = "PAIRING_NOT_ALLOWED"; break;
        case RBLE_UNSUPPORTED_REMOTE_FEATURE:           p = "UNSUPPORTED_REMOTE_FEATURE"; break;
        case RBLE_UNSPECIFIED_ERROR:                    p = "UNSPECIFIED_ERROR"; break;
        case RBLE_LMP_RSP_TIMEOUT:                      p = "LMP_RSP_TIMEOUT"; break;
        case RBLE_ENC_MODE_NOT_ACCEPT:                  p = "ENC_MODE_NOT_ACCEPT"; break;
        case RBLE_LINK_KEY_CANT_CHANGE:                 p = "LINK_KEY_CANT_CHANGE"; break;
        case RBLE_INSTANT_PASSED:                       p = "INSTANT_PASSED"; break;
        case RBLE_PAIRING_WITH_UNIT_KEY_NOT_SUP:        p = "PAIRING_WITH_UNIT_KEY_NOT_SUP"; break;
        case RBLE_DIFF_TRANSACTION_COLLISION:           p = "DIFF_TRANSACTION_COLLISION"; break;
        case RBLE_CHANNEL_CLASS_NOT_SUP:                p = "CHANNEL_CLASS_NOT_SUP"; break;
        case RBLE_INSUFFICIENT_SECURITY:                p = "INSUFFICIENT_SECURITY"; break;
        case RBLE_PARAM_OUT_OF_MAND_RANGE:              p = "PARAM_OUT_OF_MAND_RANGE"; break;
        case RBLE_SP_NOT_SUPPORTED_HOST:                p = "SP_NOT_SUPPORTED_HOST"; break;
        case RBLE_HOST_BUSY_PAIRING:                    p = "HOST_BUSY_PAIRING"; break;
        case RBLE_CONTROLLER_BUSY:                      p = "CONTROLLER_BUSY"; break;
        case RBLE_UNACCEPTABLE_CONN_INT:                p = "UNACCEPTABLE_CONN_INT"; break;
        case RBLE_DIRECT_ADV_TO:                        p = "DIRECT_ADV_TO"; break;
        case RBLE_TERMINATED_MIC_FAILURE:               p = "TERMINATED_MIC_FAILURE"; break;
        case RBLE_CONN_FAILED_TO_BE_ES:                 p = "CONN_FAILED_TO_BE_ES"; break;
        case RBLE_GAP_INVALID_PARAM_ERR:                p = "GAP_INVALID_PARAM_ERR"; break;
        case RBLE_GAP_AUTO_EST_ERR:                     p = "GAP_AUTO_EST_ERR"; break;
        case RBLE_GAP_SELECT_EST_ERR:                   p = "GAP_SELECT_EST_ERR"; break;
        case RBLE_GAP_SET_RECON_ADDR_ERR:               p = "GAP_SET_RECON_ADDR_ERR"; break;
        case RBLE_GAP_SET_PRIVACY_FEAT_ERR:             p = "GAP_SET_PRIVACY_FEAT_ERR"; break;
        case RBLE_GATT_INVALID_PARAM_ERR:               p = "GATT_INVALID_PARAM_ERR"; break;
        case RBLE_GATT_INDICATE_NOT_ALLOWED:            p = "GATT_INDICATE_NOT_ALLOWED"; break;
        case RBLE_GATT_NOTIFY_NOT_ALLOWED:              p = "GATT_NOTIFY_NOT_ALLOWED"; break;
        case RBLE_GATT_INVALID_TYPE_IN_SVC_SEARCH:      p = "INVALID_TYPE_IN_SVC_SEARCH"; break;
        case RBLE_GATT_ATTRIBUTE_CLIENT_MISSING:        p = "ATTRIBUTE_CLIENT_MISSING"; break;
        case RBLE_GATT_ATTRIBUTE_SERVER_MISSING:        p = "ATTRIBUTE_SERVER_MISSING"; break;
        case RBLE_GATT_RELIABLE_WRITE_ERR:              p = "RELIABLE_WRITE_ERR"; break;
        case RBLE_GATT_BUFF_OVER_ERR:                   p = "BUFF_OVER_ERR"; break;
        case RBLE_ATT_INVALID_PARAM_ERR:                p = "ATT_INVALID_PARAM_ERR"; break;
        case RBLE_SM_INVALID_PARAM_ERR:                 p = "SM_INVALID_PARAM_ERR"; break;
        case RBLE_SM_PAIR_ERR_PASSKEY_ENTRY_FAILED:     p = "SM_PAIR_ERR_PASSKEY_ENTRY_FAILED"; break;
        case RBLE_SM_PAIR_ERR_OOB_NOT_AVAILABLE:        p = "SM_PAIR_ERR_OOB_NOT_AVAILABLE"; break;
        case RBLE_SM_PAIR_ERR_AUTH_REQUIREMENTS:        p = "SM_PAIR_ERR_AUTH_REQUIREMENTS"; break;
        case RBLE_SM_PAIR_ERR_CFM_VAL_FAILED:           p = "SM_PAIR_ERR_CFM_VAL_FAILED"; break;
        case RBLE_SM_PAIR_ERR_PAIRING_NOT_SUPPORTED:    p = "SM_PAIR_ERR_PAIRING_NOT_SUPPORTED"; break;
        case RBLE_SM_PAIR_ERR_ENCRYPTION_KEY_SIZE:      p = "SM_PAIR_ERR_ENCRYPTION_KEY_SIZE"; break;
        case RBLE_SM_PAIR_ERR_CMD_NOT_SUPPORTED:        p = "SM_PAIR_ERR_CMD_NOT_SUPPORTED"; break;
        case RBLE_SM_PAIR_ERR_UNSPECIFIED_REASON:       p = "SM_PAIR_ERR_UNSPECIFIED_REASON"; break;
        case RBLE_SM_PAIR_ERR_REPEATED_ATTEMPTS:        p = "SM_PAIR_ERR_REPEATED_ATTEMPTS"; break;
        case RBLE_SM_PAIR_ERR_INVALID_PARAMS:           p = "SM_PAIR_ERR_INVALID_PARAMS"; break;
        case RBLE_L2C_INVALID_PARAM_ERR:                p = "L2C_INVALID_PARAM_ERR"; break;
        case RBLE_PRF_ERR_INVALID_PARAM:                p = "PRF_ERR_INVALID_PARAM"; break;
        case RBLE_PRF_ERR_INEXISTENT_HDL:               p = "PRF_ERR_INEXISTENT_HDL"; break;
        case RBLE_PRF_ERR_STOP_DISC_CHAR_MISSING:       p = "PRF_ERR_STOP_DISC_CHAR_MISSING"; break;
        case RBLE_PRF_ERR_MULTIPLE_IAS:                 p = "PRF_ERR_MULTIPLE_IAS"; break;
        case RBLE_PRF_ERR_INCORRECT_PROP:               p = "PRF_ERR_INCORRECT_PROP"; break;
        case RBLE_PRF_ERR_MULTIPLE_CHAR:                p = "PRF_ERR_MULTIPLE_CHAR"; break;
        case RBLE_PRF_ERR_NOT_WRITABLE:                 p = "PRF_ERR_NOT_WRITABLE"; break;
        case RBLE_PRF_ERR_NOT_READABLE:                 p = "PRF_ERR_NOT_READABLE"; break;
        case RBLE_PRF_ERR_REQ_DISALLOWED:               p = "PRF_ERR_REQ_DISALLOWED"; break;
        case RBLE_PRF_ERR_NTF_DISABLED:                 p = "PRF_ERR_NTF_DISABLED"; break;
        case RBLE_PRF_ERR_IND_DISABLED:                 p = "PRF_ERR_IND_DISABLED"; break;
        case RBLE_PRF_ERR_ATT_NOT_SUPPORTED:            p = "PRF_ERR_ATT_NOT_SUPPORTED"; break;
        case RBLE_ERR:                                  p = "ERR"; break;
        case RBLE_TRANS_ERR:                            p = "TRANS_ERR"; break;
        case RBLE_STATUS_ERROR:                         p = "STATUS_ERROR"; break;
        case RBLE_PARAM_ERR:                            p = "PARAM_ERR"; break;
        case RBLE_BUSY:                                 p = "BUSY"; break;
        case RBLE_SHORTAGE_OF_RESOURCE:                 p = "SHORTAGE_OF_RESOURCE"; break;
        case RBLE_EXIT:                                 p = "EXIT"; break;
        case RBLE_VERSION_FAIL:                         p = "VERSION_FAIL"; break;
        case RBLE_TEST_VERSION:                         p = "TEST_VERSION"; break;
        default:                                        p = "UNKNOWN"; break;
    }
    return( p );
}


char_t  *Get_Att_Code_Str( uint8_t att_code )
{
    char_t  *p;

    switch ( att_code ) {
        case RBLE_ATT_ERR_NO_ERROR:                 p = "NO_ERROR"; break;
        case RBLE_ATT_ERR_INVALID_HANDLE:           p = "INVALID_HANDLE"; break;
        case RBLE_ATT_ERR_READ_NOT_PERMITTED:       p = "RD_NOT_PERMITTED"; break;
        case RBLE_ATT_ERR_WRITE_NOT_PERMITTED:      p = "WR_NOT_PERMITTED"; break;
        case RBLE_ATT_ERR_INVALID_PDU:              p = "INVALID_PDU"; break;
        case RBLE_ATT_ERR_INSUFF_AUTHEN:            p = "INSUFF_AUTHEN"; break;
        case RBLE_ATT_ERR_REQUEST_NOT_SUPPORTED:    p = "REQUEST_NOT_SUPPORTED"; break;
        case RBLE_ATT_ERR_INVALID_OFFSET:           p = "INVALID_OFFSET"; break;
        case RBLE_ATT_ERR_INSUFF_AUTHOR:            p = "INSUFF_AUTHOR"; break;
        case RBLE_ATT_ERR_PREPARE_QUEUE_FULL:       p = "PREPARE_QUEUE_FULL"; break;
        case RBLE_ATT_ERR_ATTRIBUTE_NOT_FOUND:      p = "ATTRIBUTE_NOT_FOUND"; break;
        case RBLE_ATT_ERR_ATTRIBUTE_NOT_LONG:       p = "ATTRIBUTE_NOT_LONG"; break;
        case RBLE_ATT_ERR_INSUFF_ENC_KEY_SIZE:      p = "INSUFF_ENC_KEY_SIZE"; break;
        case RBLE_ATT_ERR_INVALID_ATTRIBUTE_VAL_LEN:p = "INVALID_ATTRIBUTE_VAL_LEN"; break;
        case RBLE_ATT_ERR_UNLIKELY_ERR:             p = "UNLIKELY_ERR"; break;
        case RBLE_ATT_ERR_INSUFF_ENC:               p = "INSUFF_ENC"; break;
        case RBLE_ATT_UNSUPP_GRP_TYPE:              p = "UNSUPP_GRP_TYPE"; break;
        case RBLE_ATT_INSUFF_RESOURCE:              p = "INSUFF_RESOURCE"; break;
        case RBLE_ATT_ERR_APP_ERROR:                p = "APP_ERROR"; break;
        default:                                    p = "UNKNOWN"; break;
    }
    return( p );
}

/******************************************************************************************/
/**
 *  @brief rBLE Status is changed into a character string. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] Status      rBLE Status
 *
 *  @retval Pointer to character string
 */
/******************************************************************************************/
void ATT_Info_Disp( RBLE_ATT_INFO_DATA *p )
{
    printf( "\nATT_Information each_len = %d\n", p->each_len );
    printf( "Value = " );
    Print_Dump( p->data, p->len );
}

void Print_Dump( uint8_t *p, uint16_t len )
{
    uint16_t    i;
    int8_t      c;

    for ( i = 0; i < len; i++ ) {
        if ( 0 != i ) {
            if ( 0 == ( i % 16 ) ) {
                printf( "\n" );
            } else {
                printf( " " );
            }
        }
        printf( "%02X", p[ i ] );
    }
    printf( "\n--\n" );

    for ( i = 0; i < len; i++ ) {
        if ( 0 != i ) {
            if ( 0 == ( i % 16 ) ) {
                printf( "\n" );
            }
        }
        c = p[ i ];
        printf("%c", ( ((c>=32)&&(c<=126)) ? c : '.' ) );
    }
    printf( "\n" );
}

/******************************************************************************************/
/**
 *  @brief Disp select parameter NO
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] Status      rBLE Status
 *
 *  @retval Pointer to character string
 */
/******************************************************************************************/
void Select_Param_No_Disp( uint32_t no )
{
    printf( "Select Parameter No %ld\n",  no );
}

/******************************************************************************************/
/**
 *  @brief Disp Connection handle
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @retval Pointer to character string
 */
/******************************************************************************************/
void Connection_Handle_Disp( uint16_t conhdl )
{
    printf( "Connection Handle = %d\n", conhdl );
}

/******************************************************************************************/
/**
 *  @brief Disp Illegal Connection handle message
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @retval Pointer to character string
 */
/******************************************************************************************/
void Illegal_Connection_Print( void )
{
    printf( "Illegal Connection Handle\n" );
}

#if (PRF_SEL_BLPC || PRF_SEL_GLPC)
/******************************************************************************************/
/**
 *  @brief SFLOAT to float
 *
 *  @b Contents of management:
 *
 *  @param[in] sfloat_val      SFLOAT value
 *
 *  @retval float value
 */
/******************************************************************************************/
float SFLOAT2float( uint16_t sfloat_val )
{
    float   f_val;
    int8_t  exponent, i;

    f_val = (float)( sfloat_val & 0x0FFF );
    exponent = (int8_t)(( int16_t)sfloat_val >> 12);

    if (0 <= exponent)
    {
        for (i = 0; i < exponent;  i++)
        {
            f_val *= (float)10;
        }
    }
    else
    {
        for (i = exponent; i != 0;  i++)
        {
            f_val *= (float)0.1;
        }
    }
    return( f_val );
}
#endif  //#if (PRF_SEL_BLPC || PRF_SEL_GLPC)

#if (PRF_SEL_HTPC)
/******************************************************************************************/
/**
 *  @brief FLOAT to float
 *
 *  @b Contents of management:
 *
 *  @param[in] float_val      FLOAT value
 *
 *  @retval float value
 */
/******************************************************************************************/
float FLOAT2float( uint32_t float_val )
{
    float   f_val;
    int8_t  exponent, i;

    f_val = (float)( float_val & 0x00FFFFFF );
    exponent = (int8_t)( float_val >> 24);

    if (0 <= exponent)
    {
        for (i = 0; i < exponent;  i++)
        {
            f_val *= (float)10;
        }
    }
    else
    {
        for (i = exponent; i != 0;  i++)
        {
            f_val *= (float)0.1;
        }
    }
    return( f_val );
}
#endif  //#if (PRF_SEL_HTPC)


#if (PRF_SEL_HTPC || PRF_SEL_BLPC || PRF_SEL_HRPC || PRF_SEL_CSCC || PRF_SEL_GLPC || PRF_SEL_CPPC || PRF_SEL_LNPC || PRF_SEL_RSCC)
/******************************************************************************************/
/**
 *  @brief Display DIS handle
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      DIS handle Info
 *
 *  @retval none
 */
/******************************************************************************************/
void DIS_handle_Info_Disp(RBLE_DIS_CONTENT *dis)
{
    printf( "* Device Information Service\n");
    printf( "  Start Handle    = 0x%04X\n",        dis->shdl );
    printf( "  End Handle      = 0x%04X\n\n",      dis->ehdl );
    printf( "  sys_id_char_hdl      = 0x%04X\n",   dis->sys_id_char_hdl      );
    printf( "  sys_id_val_hdl       = 0x%04X\n",   dis->sys_id_val_hdl       );
    printf( "  sys_id_prop          = 0x%02X\n\n", dis->sys_id_prop          );
    printf( "  model_nb_char_hdl    = 0x%04X\n",   dis->model_nb_char_hdl    );
    printf( "  model_nb_val_hdl     = 0x%04X\n",   dis->model_nb_val_hdl     );
    printf( "  model_nb_prop        = 0x%02X\n\n", dis->model_nb_prop        );
    printf( "  serial_nb_char_hdl   = 0x%04X\n",   dis->serial_nb_char_hdl   );
    printf( "  serial_nb_val_hdl    = 0x%04X\n",   dis->serial_nb_val_hdl    );
    printf( "  serial_nb_prop       = 0x%02X\n\n", dis->serial_nb_prop       );
    printf( "  fw_rev_char_hdl      = 0x%04X\n",   dis->fw_rev_char_hdl      );
    printf( "  fw_rev_val_hdl       = 0x%04X\n",   dis->fw_rev_val_hdl       );
    printf( "  fw_rev_prop          = 0x%02X\n\n", dis->fw_rev_prop          );
    printf( "  hw_rev_char_hdl      = 0x%04X\n",   dis->hw_rev_char_hdl      );
    printf( "  hw_rev_val_hdl       = 0x%04X\n",   dis->hw_rev_val_hdl       );
    printf( "  hw_rev_prop          = 0x%02X\n\n", dis->hw_rev_prop          );
    printf( "  sw_rev_char_hdl      = 0x%04X\n",   dis->sw_rev_char_hdl      );
    printf( "  sw_rev_val_hdl       = 0x%04X\n",   dis->sw_rev_val_hdl       );
    printf( "  sw_rev_prop          = 0x%02X\n\n", dis->sw_rev_prop          );
    printf( "  manuf_name_char_hdl  = 0x%04X\n",   dis->manuf_name_char_hdl  );
    printf( "  manuf_name_val_hdl   = 0x%04X\n",   dis->manuf_name_val_hdl   );
    printf( "  manuf_name_prop      = 0x%02X\n\n", dis->manuf_name_prop      );
    printf( "  ieee_certif_char_hdl = 0x%04X\n",   dis->ieee_certif_char_hdl );
    printf( "  ieee_certif_val_hdl  = 0x%04X\n",   dis->ieee_certif_val_hdl  );
    printf( "  ieee_certif_prop     = 0x%02X\n\n", dis->ieee_certif_prop     );
}
#endif //#if (PRF_SEL_HTPC || PRF_SEL_BLPC || PRF_SEL_HRPC || PRF_SEL_CSCC || PRF_SEL_GLPC || PRF_SEL_CPPC || PRF_SEL_LNPC || PRF_SEL_RSCC)

#if (PRF_SEL_CPPC || PRF_SEL_LNPC)
/******************************************************************************************/
/**
 *  @brief Display BAS handle
 *
 *  @b Contents of management:
 *
 *  @param[in] *p      DIS handle Info
 *
 *  @retval none
 */
/******************************************************************************************/
void BAS_handle_Info_Disp(RBLE_BATS_CONTENT *bas)
{
    printf( "* Battery Service\n");
    printf( "   Start Handle = 0x%04X\n",   bas->shdl );
    printf( "   End Handle   = 0x%04X\n\n", bas->ehdl );
    printf( "   battery_lvl_char_hdl = 0x%04X\n",   bas->battery_lvl_char_hdl );
    printf( "   battery_lvl_val_hdl  = 0x%04X\n",   bas->battery_lvl_val_hdl );
    printf( "   battery_lvl_cfg_hdl  = 0x%04X\n",   bas->battery_lvl_cfg_hdl );
    printf( "   battery_lvl_prop     = 0x%02X\n",   bas->battery_lvl_prop );
}
#endif  //#if (PRF_SEL_CPPC || PRF_SEL_LNPC)

#if (PRF_SEL_TIPC || PRF_SEL_LNPC)
/******************************************************************************************/
/**
 *  @brief Display Time
 *
 *  @b Contents of management:
 *
 *  @param[in] *stamp     Time Information
 *
 *  @retval none
 */
/******************************************************************************************/
void TIP_Time_Disp(RBLE_DATE_TIME *stamp)
{
    printf("%4d/%2d/%2d %2d:%2d:%2d\n", stamp->year, stamp->month, stamp->day, stamp->hour, stamp->min, stamp->sec);
}
#endif  //#if (PRF_SEL_TIPC || PRF_SEL_LNPC)


#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
