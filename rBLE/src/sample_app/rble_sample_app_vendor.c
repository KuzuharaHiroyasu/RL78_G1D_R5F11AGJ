/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_vendor.c
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
/* ------------------------- Vendor Specific------------------------------------------*/
static void RBLE_APP_VS_CallBack( RBLE_VS_EVENT *event );                                   /* Vendor Specific Call Back */
static BOOL RBLE_VS_Enable_Test( void );                                                    /* Vendor Specific Command Enable */
static BOOL RBLE_VS_Test_Rx_Start_Test( void );                                             /* Direct Test (RxTestStart) */
static BOOL RBLE_VS_Test_Tx_Start_Test( void );                                             /* Direct Test (TxTestStart) */
static BOOL RBLE_VS_Test_End_Test( void );                                                  /* Direct Test End */
static BOOL RBLE_VS_Test_Set_Parameter_Test( void );                                        /* Direct Test Parameter Set */
static BOOL RBLE_VS_Test_Read_RSSI_Test( void );                                            /* Direct Test Read RSSI */
static BOOL RBLE_VS_Write_BdAddress_Test( void );                                           /* Write Bluetooth Address */
static BOOL RBLE_VS_Set_Tx_Power_Test( void );                                              /* Set Tx Power */
static BOOL RBLE_VS_GPIO_Dir_Test( void );                                                  /* GPIO Dir */
static BOOL RBLE_VS_GPIO_Access_Test( void );                                               /* GPIO Access */
static BOOL RBLE_VS_Flash_Management_Test( void );                                          /* Data Flash Management */
static BOOL RBLE_VS_Flash_Access_Test( void );                                              /* Data Flash Access */
static BOOL RBLE_VS_Flash_Operation_Test( void );                                           /* Data Flash Operation */
static BOOL RBLE_VS_Flash_Get_Space_Test( void );                                           /* Data Flash Get Space */
static BOOL RBLE_VS_Flash_Get_EEL_Ver_Test( void );                                         /* Data Flash Get EEL Version */
static BOOL RBLE_VS_Adapt_Enable_Test( void );                                              /* Adaptable Function Enable */
static BOOL RBLE_VS_Set_Params_Test( void );                                                /* Set Parameters */
static BOOL RBLE_VS_RF_Control_Test( void );                                                /* RF Control */

void VS_GPIO_Mask_Disp( void );
void VS_GPIO_Read_Disp( void );


/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */
uint8_t             GPIO_Mode = 0x00;
uint8_t             GPIO_Dir = 0x00;
uint8_t             GPIO_Mask = 0x00;
uint8_t             GPIO_Value = 0x00;
uint8_t             FlashAccessData_buf[6];

/* VS Select Menu */
static const MENU_INFO  Test_Prog_VS_Menu[] = {
    {  1,   RBLE_VS_Enable_Test,                                    NULL,                               "1.VS Enable\n",                                    },
    {  2,   RBLE_VS_Test_Rx_Start_Test,                             NULL,                               "2.VS Test_Rx_Start\n",                             },
    {  3,   RBLE_VS_Test_Tx_Start_Test,                             NULL,                               "3.VS Test_Tx_Start\n",                             },
    {  4,   RBLE_VS_Test_End_Test,                                  NULL,                               "4.VS Test_End\n",                                  },
    {  5,   RBLE_VS_Test_Set_Parameter_Test,                        NULL,                               "5.VS Test_Set_Parameter\n",                        },
    {  6,   RBLE_VS_Test_Read_RSSI_Test,                            NULL,                               "6.VS Test_Read_RSSI\n",                            },
    {  7,   RBLE_VS_Write_BdAddress_Test,                           NULL,                               "7.VS Write BdAddress\n",                           },
    {  8,   RBLE_VS_Set_Tx_Power_Test,                              NULL,                               "8.VS Set_Tx_Power\n",                              },
    {  9,   RBLE_VS_GPIO_Dir_Test,                                  NULL,                               "9.VS GPIO Dir\n",                                  },
    {  10,  RBLE_VS_GPIO_Access_Test,                               NULL,                               "10.VS GPIO Access\n",                              },
    {  11,  RBLE_VS_Flash_Management_Test,                          NULL,                               "11.VS Flash_Management\n",                         },
    {  12,  RBLE_VS_Flash_Access_Test,                              NULL,                               "12.VS Flash_Access\n",                             },
    {  13,  RBLE_VS_Flash_Operation_Test,                           NULL,                               "13.VS Flash_Operation\n",                          },
    {  14,  RBLE_VS_Flash_Get_Space_Test,                           NULL,                               "14.VS Flash_Get_Space\n",                          },
    {  15,  RBLE_VS_Flash_Get_EEL_Ver_Test,                         NULL,                               "15.VS Flash_Get_EEL_Ver\n",                        },
    {  16,  RBLE_VS_Adapt_Enable_Test,                              NULL,                               "16.VS Adapt_Enable\n",                             },
    {  17,  RBLE_VS_Set_Params_Test,                                NULL,                               "17.VS Set_Params\n",                               },
    {  18,  RBLE_VS_RF_Control_Test,                                NULL,                               "18.VS RF_Control\n",                               },
    {  ESC_KEY, Menu_Exit,                                          NULL,                               "ESC Key: Menu exit\n",                             },
    {  0,   NULL,                                                   NULL,                               NULL,                                               }
};

const SELECT_MENU   Test_Prog_VS_Select_Menu = {
    "\n-- BLE Sample Program Vendor Specific Test Menu --",
    Test_Prog_VS_Menu
};

/******************************* Program Area *****************************************/
/******************************************************************************************/
/**
 *  @brief Vendor Specific Call Back
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
static void RBLE_APP_VS_CallBack( RBLE_VS_EVENT *event )
{
    Console_SetTextAttribute( EVENT_COLOR );
    switch( event->type){
        case RBLE_VS_EVENT_TEST_RX_START_COMP:
            printf( "\nrBLE VS EVENT (TEST_RX_START_COMP) Status(%s)\n", Get_Status_Str( event->param.status ) );
            break;
        case RBLE_VS_EVENT_TEST_TX_START_COMP:
            printf( "\nrBLE VS EVENT (TEST_TX_START_COMP) Status(%s)\n", Get_Status_Str( event->param.status ) );
            break;
        case RBLE_VS_EVENT_TEST_END_COMP:
            printf( "\nrBLE VS EVENT (TEST_END_COMP) Status(%s)\n", Get_Status_Str( event->param.test_end_cmp.status ) );
            printf( "ReceivedPacketsCnt = %d\n", event->param.test_end_cmp.nb_packet_received );
            break;
        case RBLE_VS_EVENT_WR_BD_ADDR_COMP:
            printf( "\nrBLE VS EVENT (Write BD Address Complete) Status(%s)\n", Get_Status_Str( event->param.status ) );
            break;
        case RBLE_VS_EVENT_SET_TEST_PARAM_COMP:
            printf( "\nrBLE VS EVENT (Set Test Parameters Complete) Status(%s)\n", Get_Status_Str( event->param.status ) );
            break;
        case RBLE_VS_EVENT_READ_TEST_RSSI_COMP:
            printf( "\nrBLE VS EVENT (Read Test RSSI Complete) Status(%s)\n", Get_Status_Str( event->param.test_rssi_cmp.status ) );
            printf( "RSSI = %d(dBm)\n", (int8_t)(event->param.test_rssi_cmp.rssi) );
            break;
        case RBLE_VS_EVENT_GPIO_DIR_COMP:
            printf( "\nrBLE VS EVENT (GPIO Dir Complete) Status(%s)\n", Get_Status_Str( event->param.gpio_dir_cmp.status ) );
            GPIO_Mask = event->param.gpio_dir_cmp.mask;
            VS_GPIO_Mask_Disp();
            break;
        case RBLE_VS_EVENT_GPIO_ACCESS_COMP:
            printf( "\nrBLE VS EVENT (GPIO Access Complete) Status(%s)\n", Get_Status_Str( event->param.gpio_access_cmp.status ) );
            GPIO_Value = event->param.gpio_access_cmp.value;
            VS_GPIO_Read_Disp();
            break;
        case RBLE_VS_EVENT_FLASH_MANAGEMENT_COMP:
            printf( "\nrBLE VS EVENT (Data Flash Management Complete) Status(%s)\n", Get_Status_Str( event->param.management_comp.status ) );
            if ( RBLE_OK == event->param.management_comp.status ) {
                (RBLE_VS_FLASH_CMD_START==event->param.management_comp.cmd)?printf( "Started.\n" ):printf( "Stopped.\n" );
            }
            break;
        case RBLE_VS_EVENT_FLASH_ACCESS_COMP:
            printf( "\nrBLE VS EVENT (Data Flash Access Complete) Status(%s)\n", Get_Status_Str( event->param.access_comp.status ) );
            if (RBLE_OK == event->param.access_comp.status) {
                (RBLE_VS_FLASH_CMD_WRITE == event->param.access_comp.cmd) ? printf("Command: Write\n") : printf("Command: Read\n");
                printf("ID: %d\n", event->param.access_comp.id);
                printf("Data Size: %d\n", event->param.access_comp.size);
                printf("Data: [%02x:%02x:%02x:%02x:%02x:%02x]\n", event->param.access_comp.addr[0], event->param.access_comp.addr[1], event->param.access_comp.addr[2], event->param.access_comp.addr[3], event->param.access_comp.addr[4], event->param.access_comp.addr[5]);
            }
            break;
        case RBLE_VS_EVENT_FLASH_OPERATION_COMP:
            printf( "\nrBLE VS EVENT (Data Flash Operation Complete) Status(%s)\n", Get_Status_Str( event->param.operation_comp.status ) );
            if ( RBLE_OK == event->param.operation_comp.status ) {
                (RBLE_VS_FLASH_CMD_START==event->param.operation_comp.cmd)?printf( "Cleaned up.\n" ):printf( "Formatted.\n" );
            }
            break;
        case RBLE_VS_EVENT_FLASH_GET_SPACE_COMP:
            printf( "\nrBLE VS EVENT (Data Flash Get Space Complete) Status(%s)\n", Get_Status_Str( event->param.get_space.status ) );
            printf( "Amount of Space: %d word\n", event->param.get_space.size );
            break;
        case RBLE_VS_EVENT_FLASH_GET_EEL_VER_COMP:
            printf( "\nrBLE VS EVENT (Data Flash Get EEL Version Complete) Status(%s)\n", Get_Status_Str( event->param.get_eel_ver.status ) );
            printf( "EEL Version: [%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c%c]\n",
                 event->param.get_eel_ver.version[0], event->param.get_eel_ver.version[1], event->param.get_eel_ver.version[2], event->param.get_eel_ver.version[3],
                 event->param.get_eel_ver.version[4], event->param.get_eel_ver.version[5], event->param.get_eel_ver.version[6], event->param.get_eel_ver.version[7],
                 event->param.get_eel_ver.version[8], event->param.get_eel_ver.version[9], event->param.get_eel_ver.version[10], event->param.get_eel_ver.version[11],
                 event->param.get_eel_ver.version[12], event->param.get_eel_ver.version[13], event->param.get_eel_ver.version[14], event->param.get_eel_ver.version[15],
                 event->param.get_eel_ver.version[16], event->param.get_eel_ver.version[17], event->param.get_eel_ver.version[18], event->param.get_eel_ver.version[19],
                 event->param.get_eel_ver.version[20], event->param.get_eel_ver.version[21], event->param.get_eel_ver.version[22], event->param.get_eel_ver.version[23] );
            break;
        case RBLE_VS_EVENT_ADAPT_ENABLE_COMP:
            printf( "\nrBLE VS EVENT (Adaptable Function Enable Complete) Status(%s)\n", Get_Status_Str( event->param.adapt_enable_cmp.status ) );
            printf( "Spacified Command: 0x%02x\n", event->param.adapt_enable_cmp.cmd );
            break;
        case RBLE_VS_EVENT_ADAPT_STATE_IND:
            printf( "\nrBLE VS EVENT (Adaptable Function State Notification)\n" );
            if ( RBLE_VS_ADAPT_MODE_NEAR   == event->param.adapt_state_ind.state )  printf( "Current State: NEAR\n" );
            if ( RBLE_VS_ADAPT_MODE_MIDDLE == event->param.adapt_state_ind.state )  printf( "Current State: MIDDLE\n" );
            if ( RBLE_VS_ADAPT_MODE_FAR    == event->param.adapt_state_ind.state )  printf( "Current State: FAR\n" );
            break;
        case RBLE_VS_EVENT_RF_CONTROL_COMP:
            printf( "\nrBLE VS EVENT (RF Control Complete) Status(%s)\n", Get_Status_Str( event->param.rf_control_cmp.status ) );
            break;
        case RBLE_EVT_VS_CMD_DISALLOWED_IND:
            printf( "\nrBLE VS EVENT (VS_ERROR) Status(%s)\n", Get_Status_Str( event->param.cmd_disallowed_ind.status ) );
            printf( "Cpecode = %x\n", event->param.cmd_disallowed_ind.opcode );
            break;
        case RBLE_VS_EVENT_SET_TX_POWER_COMP:
            printf( "\nrBLE VS EVENT (Set Tx Power Complete) Status(%s)\n", Get_Status_Str( event->param.status ) );
            break;
        case RBLE_VS_EVENT_SET_PARAMS_COMP:
            printf( "\nrBLE VS EVENT (Set Params Complete) Status(%s)\n", Get_Status_Str( event->param.status ) );
            break;
        default:
            break;
    }
    Prompt_Disp();
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Enable command is executed. 
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
static BOOL RBLE_VS_Enable_Test( void )
{

    RBLE_STATUS     Ret_Status;

    /* API Call */
    Ret_Status = RBLE_VS_Enable( RBLE_APP_VS_CallBack );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Test Rx Start command is executed. 
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
static BOOL RBLE_VS_Test_Rx_Start_Test( void )
{
    RBLE_STATUS     Ret_Status;
    bool            Use_Message_Flag = TRUE;

    if ( 1 == RBLE_Parameter_Cnt ) {
        if ( 39 >= RBLE_Parameter_Arg[ 0 ] ) {
            Use_Message_Flag = FALSE;
        }
    }
    if ( TRUE == Use_Message_Flag ) {
        Console_SetTextAttribute( MESSAGE_COLOR );
        printf( "VS_Test_Rx_Start Useage:TestNo RxFreq(0-39)\n" );
        Console_SetTextAttribute( COMMAND_COLOR );
    } else {
        /* API Call */
        Ret_Status = RBLE_VS_Test_Rx_Start( ( uint8_t )RBLE_Parameter_Arg[ 0 ] );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Test Tx Start command is executed. 
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
static BOOL RBLE_VS_Test_Tx_Start_Test( void )
{
    RBLE_STATUS     Ret_Status;
    bool            Use_Message_Flag = TRUE;

    if ( 3 == RBLE_Parameter_Cnt ) {
        if ( ( ( 39 >= RBLE_Parameter_Arg[ 0 ] )
            && ( ( 1 <= RBLE_Parameter_Arg[ 1 ] ) && ( 37 >= RBLE_Parameter_Arg[ 1 ] ) ) )
            && ( 7 >= RBLE_Parameter_Arg[ 2 ] ) ) {
            Use_Message_Flag = FALSE;
        }
    }
    if ( TRUE == Use_Message_Flag ) {
        Console_SetTextAttribute( MESSAGE_COLOR );
        printf( "VS_Test_Tx_Start Useage:TestNo TxFreq(0-39) DataLen(1-37) PayloadType(0-7)\n" );
        printf( "[PayloadType]\n" );
        printf( "0: Pseudo-Random bit sequence 9\n" );
        printf( "1: Pattern of alternating bits '11110000'\n" );
        printf( "2: Pattern of alternating bits '10101010'\n" );
        printf( "3: Pseudo-Random bit sequence 15\n" );
        printf( "4: Pattern of All '1' bits\n" );
        printf( "5: Pattern of All '0' bits\n" );
        printf( "6: Pattern of alternating bits '00001111'\n" );
        printf( "7: Pattern of alternating bits '0101'\n" );
        Console_SetTextAttribute( COMMAND_COLOR );
    } else {
        /* API Call */
        Ret_Status = RBLE_VS_Test_Tx_Start( ( uint8_t )RBLE_Parameter_Arg[ 0 ], ( uint8_t )RBLE_Parameter_Arg[ 1 ], ( uint8_t )RBLE_Parameter_Arg[ 2 ] );
    
        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Test End command is executed. 
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
static BOOL RBLE_VS_Test_End_Test( void )
{
    RBLE_STATUS     Ret_Status;

    /* API Call */
    Ret_Status = RBLE_VS_Test_End();

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Set Direct Test Parameter is executed. 
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
static BOOL RBLE_VS_Test_Set_Parameter_Test( void )
{
    RBLE_STATUS     Ret_Status;
    bool            Use_Message_Flag = TRUE;

    if ( 3 == RBLE_Parameter_Cnt ) {
        if ( 2 >= RBLE_Parameter_Arg[ 2 ] ) {
            Use_Message_Flag = FALSE;
        }
    }
    if ( TRUE == Use_Message_Flag ) {
        Console_SetTextAttribute( MESSAGE_COLOR );
        printf( "VS_Test_Set_Parameter Useage:TestNo" );
        printf( "rx_max(0-65535) tx_max(0-65535) infinite_setting(0/1/2)\n" );
        Console_SetTextAttribute( COMMAND_COLOR );
    } else {
        /* API Call */
        Ret_Status = RBLE_VS_Set_Test_Parameter( ( uint16_t )RBLE_Parameter_Arg[ 0 ], ( uint16_t )RBLE_Parameter_Arg[ 1 ], ( uint8_t )RBLE_Parameter_Arg[ 2 ] );

        RBLE_Status_Disp( Ret_Status );
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Read RSSI is executed. 
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
static BOOL RBLE_VS_Test_Read_RSSI_Test( void )
{
    RBLE_STATUS     Ret_Status;

    /* API Call */
    Ret_Status = RBLE_VS_Read_Test_RSSI();

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic write Bluetooth Address command is executed. 
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
static BOOL RBLE_VS_Write_BdAddress_Test( void )
{
    static RBLE_BD_ADDR BdAddress = { 0x45, 0x00, 0xDE, 0xFA, 0xFE, 0xCA };
    RBLE_STATUS     Ret_Status;

#if 1
    if ( 2 == RBLE_Parameter_Cnt ) {
        BdAddress.addr[ 0 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 0 ) & 0xff );
        BdAddress.addr[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );
        BdAddress.addr[ 2 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 16 ) & 0xff );
        BdAddress.addr[ 3 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 0 ] >> 0 ) & 0xff );
        BdAddress.addr[ 4 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 0 ] >> 8 ) & 0xff );
        BdAddress.addr[ 5 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 0 ] >> 16 ) & 0xff );
    }
#endif

    /* API Call */
    Ret_Status = RBLE_VS_Write_Bd_Address( &BdAddress );

    BdAddress_Disp( &BdAddress );
    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Set Tx Power command is executed. 
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
static BOOL RBLE_VS_Set_Tx_Power_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint8_t         Tx_Power_Lvl;
    uint8_t         Adapt_State;

    if ( 0x10 == RBLE_Parameter_Arg[0] && 2 <= RBLE_Parameter_Cnt ) {
        Tx_Power_Lvl = (uint8_t)RBLE_Parameter_Arg[1];
        if ( RBLE_VS_TXPW_LV1 > Tx_Power_Lvl ) Tx_Power_Lvl = RBLE_VS_TXPW_LV1;
        if ( RBLE_VS_TXPW_LV9 < Tx_Power_Lvl ) Tx_Power_Lvl = RBLE_VS_TXPW_LV9;

        printf( "Set to level 0x%02x w/o connection\n", Tx_Power_Lvl );
        /* API Call */
        Ret_Status = RBLE_VS_Set_Tx_Power( 0x10, Tx_Power_Lvl, RBLE_VS_TXPW_MODE_NORMAL );

        RBLE_Status_Disp( Ret_Status );
    } else {
        /* Valid Connection Handle ? */
        if ( ILLEGAL_CONNECTION_HANDLE == Connection_Handle ) {
            Illegal_Connection_Print();
        } else {
            if ( 2 >= RBLE_Parameter_Cnt ) {
                Adapt_State = RBLE_VS_TXPW_MODE_NORMAL;
            } else {
                Adapt_State = (uint8_t)RBLE_Parameter_Arg[2];
            }
            if ( 1 >= RBLE_Parameter_Cnt ) {
                Tx_Power_Lvl = RBLE_VS_TXPW_LV1;
            } else {
                Tx_Power_Lvl = (uint8_t)RBLE_Parameter_Arg[1];
                if ( RBLE_VS_TXPW_LV1 > Tx_Power_Lvl ) Tx_Power_Lvl = RBLE_VS_TXPW_LV1;
                if ( RBLE_VS_TXPW_LV9 < Tx_Power_Lvl ) Tx_Power_Lvl = RBLE_VS_TXPW_LV9;
            }
            printf( "Level: 0x%02x, State: 0x%02x\n", Tx_Power_Lvl, Adapt_State );
            /* API Call */
            Ret_Status = RBLE_VS_Set_Tx_Power( Connection_Handle, Tx_Power_Lvl, Adapt_State );
    
            RBLE_Status_Disp( Ret_Status );
        }
    }
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic GPIO Dir command is executed. 
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
static BOOL RBLE_VS_GPIO_Dir_Test( void )
{
    RBLE_STATUS     Ret_Status;

    /* Set GPIO direction */
    if ( 0 == RBLE_Parameter_Cnt ) {
        RBLE_VS_GPIO_DIR_SETTING(GPIO_Dir, RBLE_VS_GPIO_BIT_0, RBLE_VS_GPIO_INPUT);
        RBLE_VS_GPIO_DIR_SETTING(GPIO_Dir, RBLE_VS_GPIO_BIT_1, RBLE_VS_GPIO_OUTPUT);
        RBLE_VS_GPIO_DIR_SETTING(GPIO_Dir, RBLE_VS_GPIO_BIT_2, RBLE_VS_GPIO_INPUT);
        RBLE_VS_GPIO_DIR_SETTING(GPIO_Dir, RBLE_VS_GPIO_BIT_3, RBLE_VS_GPIO_OUTPUT);
    }
    else {
        GPIO_Dir = (uint8_t)RBLE_Parameter_Arg[0];
    }

    printf( "Direction: 0x%02x\n", GPIO_Dir );

    /* API Call */
    Ret_Status = RBLE_VS_GPIO_Dir( GPIO_Dir );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic GPIO Access command is executed. 
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
static BOOL RBLE_VS_GPIO_Access_Test( void )
{
    RBLE_STATUS     Ret_Status;

    if ( 0 == RBLE_Parameter_Cnt ) {
        GPIO_Mode = RBLE_VS_GPIO_OUTPUT_MD;
//      GPIO_Mode = RBLE_VS_GPIO_INPUT_MD;
    }
    else {
        if ( RBLE_VS_GPIO_OUTPUT_MD == RBLE_Parameter_Arg[0] ) {
            GPIO_Mode = RBLE_VS_GPIO_OUTPUT_MD;
        }
        else {
            GPIO_Mode = RBLE_VS_GPIO_INPUT_MD;
        }
    }

    if ( RBLE_VS_GPIO_OUTPUT_MD == GPIO_Mode )
    {
        /* Set GPIO output value */
        if ( 2 <= RBLE_Parameter_Cnt ) {
            GPIO_Value = (uint8_t)RBLE_Parameter_Arg[1];
        }
        else {
            RBLE_VS_GPIO_OUTPUT_SETTING(GPIO_Value, RBLE_VS_GPIO_BIT_0, RBLE_VS_GPIO_LOW);
            RBLE_VS_GPIO_OUTPUT_SETTING(GPIO_Value, RBLE_VS_GPIO_BIT_1, RBLE_VS_GPIO_HIGH);
            RBLE_VS_GPIO_OUTPUT_SETTING(GPIO_Value, RBLE_VS_GPIO_BIT_2, RBLE_VS_GPIO_LOW);
            RBLE_VS_GPIO_OUTPUT_SETTING(GPIO_Value, RBLE_VS_GPIO_BIT_3, RBLE_VS_GPIO_HIGH);
        }
    }
    else
    {
        GPIO_Value = 0x00;
    }

    printf( "Mode: " );
    (RBLE_VS_GPIO_OUTPUT_MD==GPIO_Mode)?printf( "Output Mode\n" ):printf( "Input Mode\n" );
    printf( "Value: 0x%02x\n", GPIO_Value );

    /* API Call */
    Ret_Status = RBLE_VS_GPIO_Access( GPIO_Mode, GPIO_Value );

    RBLE_Status_Disp( Ret_Status );
    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Data Flash Management command is executed. 
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
static BOOL RBLE_VS_Flash_Management_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint8_t         Flash_cmd;

    if ( 0 == RBLE_Parameter_Cnt ) {
        Flash_cmd = RBLE_VS_FLASH_CMD_START;
    }
    else {
        Flash_cmd = (uint8_t)RBLE_Parameter_Arg[0];
    }

    printf( "Command: %d\n", Flash_cmd );

    if ( ( Flash_cmd == RBLE_VS_FLASH_CMD_START ) || ( Flash_cmd == RBLE_VS_FLASH_CMD_STOP ) ) {
        /* API Call */
        Ret_Status = RBLE_VS_Flash_Management( Flash_cmd );
    }
    else {
        printf( "You should specify a parameter: %d (Start) or %d (Stop)\n", RBLE_VS_FLASH_CMD_START, RBLE_VS_FLASH_CMD_STOP );
        Ret_Status = RBLE_PARAM_ERR;
    }
    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Data Flash Access command is executed. 
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
static BOOL RBLE_VS_Flash_Access_Test( void )
{
    RBLE_STATUS     Ret_Status;
    RBLE_VS_FLASH_ACCESS_PARAM param;
    uint8_t         bda[6] = { 0x45, 0x00, 0xDE, 0xFA, 0xFE, 0xCA };

    param.id   = 0x01;
    param.size = 6;

    if ( 0 == RBLE_Parameter_Cnt ) {
        param.cmd  = RBLE_VS_FLASH_CMD_WRITE;
        memcpy( FlashAccessData_buf, bda, 6 );
    }
    else if ( 1 <= RBLE_Parameter_Cnt ) {
        if ( 0 == RBLE_Parameter_Arg[0] ) {
            param.cmd  = RBLE_VS_FLASH_CMD_WRITE;
            memcpy( FlashAccessData_buf, bda, 6 );
        }
        else {
            param.cmd  = RBLE_VS_FLASH_CMD_READ;
            memset( FlashAccessData_buf, 0, 6 );
        }
    }

    param.addr = FlashAccessData_buf;

    /* API Call */
    Ret_Status = RBLE_VS_Flash_Access( &param );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Data Flash Operation command is executed. 
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
static BOOL RBLE_VS_Flash_Operation_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint8_t         Flash_cmd;

    if ( 0 == RBLE_Parameter_Cnt ) {
        Flash_cmd = RBLE_VS_FLASH_CMD_CLEANUP;
    }
    else {
        Flash_cmd = (uint8_t)RBLE_Parameter_Arg[0];
    }

    printf( "Command: %d\n", Flash_cmd );

    if ( ( Flash_cmd == RBLE_VS_FLASH_CMD_CLEANUP ) || ( Flash_cmd == RBLE_VS_FLASH_CMD_FORMAT ) ) {
        /* API Call */
        Ret_Status = RBLE_VS_Flash_Operation( Flash_cmd );
    }
    else {
        printf( "You should specify a parameter: %d (Clean up) or %d (Format)\n", RBLE_VS_FLASH_CMD_CLEANUP, RBLE_VS_FLASH_CMD_FORMAT );
        Ret_Status = RBLE_PARAM_ERR;
    }
    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Data Flash Get Space command is executed. 
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
static BOOL RBLE_VS_Flash_Get_Space_Test( void )
{
    RBLE_STATUS     Ret_Status;

    /* API Call */
    Ret_Status = RBLE_VS_Flash_Get_Space();
    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Data Flash Get EEL Version command is executed. 
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
static BOOL RBLE_VS_Flash_Get_EEL_Ver_Test( void )
{
    RBLE_STATUS     Ret_Status;

    /* API Call */
    Ret_Status = RBLE_VS_Flash_Get_EEL_Ver();
    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic Adaptable Function Enable command is executed. 
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
static BOOL RBLE_VS_Adapt_Enable_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint8_t         Adapt_cmd;

    if ( 0 == RBLE_Parameter_Cnt ) {
        Adapt_cmd = RBLE_VS_ADAPT_CMD_ENABLE;
    }
    else {
        Adapt_cmd = (uint8_t)RBLE_Parameter_Arg[0];
    }

    printf( "Command: 0x%02x  ", Adapt_cmd );
    switch (Adapt_cmd) {
    case RBLE_VS_ADAPT_CMD_DISABLE:
        printf( "(Disable)\n" );
        break;
    case RBLE_VS_ADAPT_CMD_ENABLE:
        printf( "(Enable)\n" );
        break;
    case RBLE_VS_ADAPT_CMD_ENABLE_WO_IND:
        printf( "(Enable Without State Indication)\n" );
        break;
    default:
        printf( "(Not Supported)\n" );
        break;
    }

    /* API Call */
    Ret_Status = RBLE_VS_Adapt_Enable( Adapt_cmd );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic RF Control command is executed. 
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
static BOOL RBLE_VS_RF_Control_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint8_t         rf_ctrl_cmd;

    if ( 0 == RBLE_Parameter_Cnt ) {
        rf_ctrl_cmd = RBLE_VS_RFCNTL_CMD_POWDOWN;
    }
    else {
        rf_ctrl_cmd = (uint8_t)RBLE_Parameter_Arg[0];
    }

    printf( "Command: 0x%02x  ", rf_ctrl_cmd );
    switch (rf_ctrl_cmd) {
    case RBLE_VS_RFCNTL_CMD_POWDOWN:
        printf( "(Pow-down)\n" );
        break;
    case RBLE_VS_RFCNTL_CMD_POWUP_DDCON:
        printf( "(Pow-up:ddcon)\n" );
        break;
    case RBLE_VS_RFCNTL_CMD_POWUP_DDCOFF:
        printf( "(Pow-up:ddcoff)\n" );
        break;
    default:
        printf( "(Not Supported)\n" );
        break;
    }

    /* API Call */
    Ret_Status = RBLE_VS_RF_Control( rf_ctrl_cmd );

    RBLE_Status_Disp( Ret_Status );

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief A Vendor Specfic set parameters command is executed. 
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
static BOOL RBLE_VS_Set_Params_Test( void )
{
    RBLE_STATUS     Ret_Status;
    uint8_t         data[2];

    if ( 2 == RBLE_Parameter_Cnt ) {
        /* API Call */
        data[ 0 ] = ( uint8_t )( RBLE_Parameter_Arg[ 1 ] & 0xff );
        data[ 1 ] = ( uint8_t )( ( RBLE_Parameter_Arg[ 1 ] >> 8 ) & 0xff );

        Ret_Status = RBLE_VS_Set_Params( (uint8_t)RBLE_Parameter_Arg[ 0 ], 2, data );

        RBLE_Status_Disp( Ret_Status );
    } else {
        Console_SetTextAttribute( MESSAGE_COLOR );
        printf( "VS_Set_Params Usage:\n" );
        printf( "param_id(0-12) param_data(0-65535)\n" );
        Console_SetTextAttribute( COMMAND_COLOR );
    }

    return( TRUE );
}

/******************************************************************************************/
/**
 *  @brief Display VS GPIO Mask Info
 *
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval none
 */
/******************************************************************************************/
void VS_GPIO_Mask_Disp( void )
{
    printf( "Port used as GPIO" );
    if ( GPIO_Mask&(RBLE_VS_GPIO_BIT_0|RBLE_VS_GPIO_BIT_1|RBLE_VS_GPIO_BIT_2|RBLE_VS_GPIO_BIT_3) )
    {
        if ( GPIO_Mask & RBLE_VS_GPIO_BIT_0 ) printf( ": GPIO0" );
        if ( GPIO_Mask & RBLE_VS_GPIO_BIT_1 ) printf( ": GPIO1" );
        if ( GPIO_Mask & RBLE_VS_GPIO_BIT_2 ) printf( ": GPIO2" );
        if ( GPIO_Mask & RBLE_VS_GPIO_BIT_3 ) printf( ": GPIO3" );
    }
    else
    {
        printf( ": none." );
    }
    printf( "\n" );
    return;
}

/******************************************************************************************/
/**
 *  @brief Display VS GPIO Reading Value Info
 *
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval none
 */
/******************************************************************************************/
void VS_GPIO_Read_Disp( void )
{
    if ( GPIO_Mode == RBLE_VS_GPIO_INPUT_MD )
    {
        if ( (GPIO_Mask & RBLE_VS_GPIO_BIT_0) && !(GPIO_Dir & RBLE_VS_GPIO_BIT_0) )
        {
            (GPIO_Value & RBLE_VS_GPIO_BIT_0)?printf( "GPIO0: HIGH\n" ):printf( "GPIO0: LOW\n" );
        }
        if ( (GPIO_Mask & RBLE_VS_GPIO_BIT_1) && !(GPIO_Dir & RBLE_VS_GPIO_BIT_1) )
        {
            (GPIO_Value & RBLE_VS_GPIO_BIT_1)?printf( "GPIO1: HIGH\n" ):printf( "GPIO1: LOW\n" );
        }
        if ( (GPIO_Mask & RBLE_VS_GPIO_BIT_2) && !(GPIO_Dir & RBLE_VS_GPIO_BIT_2) )
        {
            (GPIO_Value & RBLE_VS_GPIO_BIT_2)?printf( "GPIO2: HIGH\n" ):printf( "GPIO2: LOW\n" );
        }
        if ( (GPIO_Mask & RBLE_VS_GPIO_BIT_3) && !(GPIO_Dir & RBLE_VS_GPIO_BIT_3) )
        {
            (GPIO_Value & RBLE_VS_GPIO_BIT_3)?printf( "GPIO3: HIGH\n" ):printf( "GPIO3: LOW\n" );
        }
    }
    else
    {
        printf( "It's Output Mode now.\n" );
    }
    return;
}

#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
