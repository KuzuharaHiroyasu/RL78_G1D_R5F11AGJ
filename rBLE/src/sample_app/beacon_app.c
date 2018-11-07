/**
 ****************************************************************************************
 *
 * @file        beacon_app.c
 *
 * @brief       rBLE Sample Application for Beacon mode
 *
 * Copyright(C) 2013-2015 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include    "rwke_api.h"
#include    "rble.h"
#include    "rble_app.h"
#include    "beacon_app.h"

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

#ifdef CFG_SAMPLE_BEACON

/**************************** Define Declaration **************************************/
enum APP_TASK_ID_enum
{
    /* ------------------------- Generic Access Profile ------------------------------*/
    APP_CMD_GAP_RESET = 0       ,
    APP_CMD_GAP_BROADCAST_ENABLE,
};

/*
 * PROTOTYPE DECLARATIONS
 ****************************************************************************************
 */
static void APP_RBLE_CallBack( RBLE_MODE mode );
static void APP_RBLE_Active_Event( void );

/* ---------------------------- Generic Access Profile --------------------------------*/
static int_t APP_GAP_Reset(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t APP_GAP_Broadcast_Enable(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

static void APP_GAP_CallBack( RBLE_GAP_EVENT *event );
static void APP_GAP_Reset_Event( RBLE_GAP_EVENT *event );
static void APP_GAP_Broadcast_Enable_Event( RBLE_GAP_EVENT *event );

/* ------------------------------ Security Manager ------------------------------------*/
static void APP_SM_CallBack( RBLE_SM_EVENT *event );

/* ---------------------------------- Utility -----------------------------------------*/
static void APP_Send_KernelMsg( uint16_t msg_id );

/*
 * VARIABLE DEFINITIONS
 ****************************************************************************************
 */
const struct ke_msg_handler APP_Handler[] =
{
    /* ------------------------- Generic Access Profile ------------------------------*/
    { APP_CMD_GAP_RESET           , (ke_msg_func_t)APP_GAP_Reset           },
    { APP_CMD_GAP_BROADCAST_ENABLE, (ke_msg_func_t)APP_GAP_Broadcast_Enable},
};
const struct ke_state_handler APP_State_Handler[APP_STATE_MAX] =
{
    KE_STATE_HANDLER(APP_Handler)
};
const struct ke_state_handler APP_Default_Handler = KE_STATE_HANDLER_NONE;

ke_state_t APP_State[APP_IDX_MAX];

/* Broadcaster: scannable undirected advertising */
static const uint16_t         broadcast_disc_mode = RBLE_GAP_BROADCASTER;
static const uint16_t         broadcast_conn_mode = 0;
static const RBLE_ADV_INFO    broadcast_adv_info  =
{
    {
        0x00A0,  /* 100msec (0xA0 * 625usec) */     /* AdvIntervalMin */
        0x00A0,                                     /* AdvIntervalMax */
        RBLE_GAP_ADV_DISC_UNDIR,                    /* AdvType        */
        RBLE_ADDR_PUBLIC,                           /* OwnAddrType    */
        RBLE_ADDR_PUBLIC,                           /* DirectAddrType */
        { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 },     /* DirectAddr     */
        RBLE_ADV_ALL_CHANNELS,                      /* AdvChannelMap  */
        RBLE_ADV_ALLOW_SCAN_ANY_CON_ANY,            /* AdvFiltPolicy  */
        0x00,                                       /* Reserved       */
    },
    {
        /* AdvDataLen     */
        (1+1+1)+(1+1+26),
        /* AdvData        */
        1+1,  0x01, 0x04,  /* Flags */ /*  Note: A device in the broadcast mode shall not set the 'LE General Discoverable Mode' flag 
                                                 or the 'LE Limited Discoverable Mode' flag in the Flags */ 
        1+26, 0x08,        /* Shortened Local Name */
        'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',  /* +10 */
        'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't',  /* +10 */
        'u', 'v', 'w', 'x', 'y', 'z'                       /* +6  */
    },
    {
        /* ScanRspDataLen */
        (1+1+29),
        /* ScanData       */
        1+29, 0x09,        /* Complete Local Name */
        'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',  /* +10 */
        'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T',  /* +10 */
        'U', 'V', 'W', 'X', 'Y', 'Z', '0', '1', '2'        /* +9  */
    }
};

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/****************************************************************************************
* Function Name: APP_Init
* Description  : Application Initialization
* Arguments    : None
* Return Value : TRUE if succeeded , FALSE if failed
****************************************************************************************/
BOOL APP_Init( void )
{
    BOOL        ret = TRUE;
    RBLE_STATUS ret_status;

    /* Initialize rBLE */
    ret_status = RBLE_Init(&APP_RBLE_CallBack);
    if ( RBLE_OK != ret_status )
    {
        ret = FALSE;
    }
    return( ret );
}

/****************************************************************************************
* Function Name: APP_GAP_Reset
* Description  : rBLE GAP Reset API Calling
* Arguments    : Kernel Message Handler Arguments
* Return Value : Kernel Message Handler Retun Status
****************************************************************************************/
static int_t APP_GAP_Reset(ke_msg_id_t  const  msgid,
                           void         const *param,
                           ke_task_id_t const  dest_id,
                           ke_task_id_t const  src_id)
{
    RBLE_GAP_Reset( &APP_GAP_CallBack, &APP_SM_CallBack );

    return(KE_MSG_CONSUMED);
}

/****************************************************************************************
* Function Name: APP_GAP_Broadcast_Enable
* Description  : rBLE GAP Broadcast Enable API Calling
* Arguments    : Kernel Message Handler Arguments
* Return Value : Kernel Message Handler Retun Status
****************************************************************************************/
static int_t APP_GAP_Broadcast_Enable(ke_msg_id_t  const  msgid,
                                      void         const *param,
                                      ke_task_id_t const  dest_id,
                                      ke_task_id_t const  src_id)
{

    RBLE_GAP_Broadcast_Enable( broadcast_disc_mode, 
                               broadcast_conn_mode, 
                               (RBLE_ADV_INFO*)&broadcast_adv_info );

    return(KE_MSG_CONSUMED);
}

/****************************************************************************************
* Function Name: APP_Is_Idle
* Description  : Check if Application is Idle Status
* Arguments    : None
* Return Value : TRUE if App is Idle Status, FALSE if it is necessary to keep MCU Running
****************************************************************************************/
BOOL APP_Is_Idle( void )
{
    return( TRUE );
}

/****************************************************************************************
* Function Name: APP_RBLE_CallBack
* Description  : rBLE Mode Event Handling
* Arguments    : mode : rBLE Mode
* Return Value : None
****************************************************************************************/
static void APP_RBLE_CallBack( RBLE_MODE mode )
{
    switch(mode)
    {
        case RBLE_MODE_INITIALIZE:
            break;
        case RBLE_MODE_ACTIVE:
            APP_RBLE_Active_Event();
            break;
        case RBLE_MODE_RESET:
            break;
        case RBLE_MODE_SLEEP:
            break;
        case RBLE_MODE_ERROR:
            break;
        default:
            break;
    }
}

/****************************************************************************************
* Function Name: APP_RBLE_Active_Event
* Description  : rBLE Mode Active Event Handling
* Arguments    : None
* Return Value : None
****************************************************************************************/
static void APP_RBLE_Active_Event( void )
{
    APP_Send_KernelMsg(APP_CMD_GAP_RESET);
}

/* ---------------------------- Generic Access Profile --------------------------------*/

/****************************************************************************************
* Function Name: APP_GAP_CallBack
* Description  : GAP Event Handling
* Arguments    : event : GAP Event
* Return Value : None
****************************************************************************************/
static void APP_GAP_CallBack( RBLE_GAP_EVENT *event )
{
    switch( event->type )
    {
        case RBLE_GAP_EVENT_RESET_RESULT:
            APP_GAP_Reset_Event( event );
            break;
        case RBLE_GAP_EVENT_BROADCAST_ENABLE_COMP:
            APP_GAP_Broadcast_Enable_Event( event );
            break;
        case RBLE_GAP_EVENT_COMMAND_DISALLOWED_IND:
            break;
        default:
            break;
    }
}

/****************************************************************************************
* Function Name: APP_GAP_Reset_Event
* Description  : GAP Reset Complete Event Handling
* Arguments    : event : GAP Event
* Return Value : None
****************************************************************************************/
static void APP_GAP_Reset_Event( RBLE_GAP_EVENT *event )
{
    if(RBLE_OK == event->param.status)
    {
        /* Enable Broadcast */
        APP_Send_KernelMsg(APP_CMD_GAP_BROADCAST_ENABLE);
    }
}

/****************************************************************************************
* Function Name: APP_GAP_Broadcast_Enable_Event
* Description  : GAP Broadcast Enable Complete Event Handling
* Arguments    : event : GAP Event
* Return Value : None
****************************************************************************************/
static void APP_GAP_Broadcast_Enable_Event( RBLE_GAP_EVENT *event )
{
}

/* ----------------------------- Security Manager -------------------------------------*/

/****************************************************************************************
* Function Name: APP_SM_CallBack
* Description  : SM Event Handling
* Arguments    : event : SM Event
* Return Value : None
****************************************************************************************/
static void APP_SM_CallBack( RBLE_SM_EVENT *event )
{
    switch( event->type )
    {
        case RBLE_SM_EVENT_SET_CNF:
            break;
        case RBLE_SM_ENC_START_IND:
            break;
        case RBLE_SM_TK_REQ_IND:
            break;
        case RBLE_SM_LTK_REQ_IND:
            break;
        case RBLE_SM_IRK_REQ_IND:
            break;
        case RBLE_SM_CSRK_REQ_IND:
            break;
        case RBLE_SM_KEY_IND:
            break;
        case RBLE_SM_CHK_BD_ADDR_REQ:
            break;
        case RBLE_SM_TIMEOUT_EVT:
            break;
        case RBLE_SM_EVENT_COMMAND_DISALLOWED_IND:
            break;
        default:
            break;
    }
}

/* ------------------------------------ Utility ---------------------------------------*/

/****************************************************************************************
* Function Name: APP_Send_KernelMsg
* Description  : Send Kernel Message
* Arguments    : msg_id : Kernel Message ID
* Return Value : None
****************************************************************************************/
static void APP_Send_KernelMsg( uint16_t msg_id )
{
    uint8_t *ke_msg;

    ke_msg = ke_msg_alloc(msg_id, TASK_CON_APPL, TASK_CON_APPL, 0);
    ke_msg_send(ke_msg);
}
#endif//#ifndef CFG_SAMPLE_BEACON

