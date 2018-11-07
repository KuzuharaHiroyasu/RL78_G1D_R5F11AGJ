/*******************************************************************************
 * DISCLAIMER
 * This software is supplied by Renesas Electronics Corporation and is only
 * intended for use with Renesas products. No other uses are authorized. This
 * software is owned by Renesas Electronics Corporation and is protected under
 * all applicable laws, including copyright laws.
 * THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
 * THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
 * LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
 * AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
 * TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
 * ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
 * FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
 * ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
 * BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
 * Renesas reserves the right, without notice, to make changes to this software
 * and to discontinue the availability of this software. By using this software,
 * you agree to the additional terms and conditions found by accessing the
 * following link:
 * http://www.renesas.com/disclaimer
 * (C) 2012-2015 Renesas Electronics Corporation All rights reserved.
 ******************************************************************************/

/******************************************************************************
 * File Name  : rble_sample_app_peripheral.c
 * Version    : 1.0
 * Description: sample application peripheral role source file
 *
 * Copyright(C) 2012-2016 Renesas Electronics Corporation
 ******************************************************************************/

/*******************************************************************************
 * Include Files
 ******************************************************************************/
#include "rble_sample_app_peripheral.h"
#include "console.h"
#include "sam/sams.h"
#include "led_onoff.h"
#include "push_state.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define APP_SWITCH_STATE_CHECK_INTERVAL (50)

/*******************************************************************************
 * Function Declarations
 ******************************************************************************/
static void app_msg_send(ke_msg_id_t id);

static int_t app_reset(ke_msg_id_t const msgid, void const *param,
                       ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t app_advertise_start(ke_msg_id_t const msgid, void const *param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t app_profile_enable(ke_msg_id_t const msgid, void const *param,
                                ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t app_profile_disable(ke_msg_id_t const msgid, void const *param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t app_timer_expired(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id);

static void app_gap_callback(RBLE_GAP_EVENT *event);
static void app_sm_callback(RBLE_SM_EVENT *event);
static void app_sams_callback(SAMPLE_SERVER_EVENT *event);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
static struct {
    uint16_t conhdl;
} app_info;

/* ##### Task Definitions ##### */
ke_state_t app_state[APP_STATE_MAX];

const struct ke_msg_handler app_reset_handler[] = {
    { APP_MSG_BOOTUP,        (ke_msg_func_t)app_reset },
};

const struct ke_msg_handler app_nonconnect_handler[] = {
    { APP_MSG_RESET_COMP,       (ke_msg_func_t)app_advertise_start },
    { APP_MSG_DISCONNECTED,     (ke_msg_func_t)app_profile_disable },
    { APP_MSG_PROFILE_DISABLED, (ke_msg_func_t)app_advertise_start },
};

const struct ke_msg_handler app_connect_handler[] = {
    { APP_MSG_CONNECTED,       (ke_msg_func_t)app_profile_enable },
    /* { APP_MSG_PROFILE_ENABLED, (ke_msg_func_t)NULL }, */
    { APP_MSG_TIMER_EXPIRED,   (ke_msg_func_t)app_timer_expired },
};

const struct ke_state_handler app_state_handler[APP_STATE_MAX] =
{
    KE_STATE_HANDLER(app_reset_handler),
    KE_STATE_HANDLER(app_nonconnect_handler),
    KE_STATE_HANDLER(app_connect_handler),
};

/* ##### Advertise Parameter ##### */
static RBLE_ADV_INFO app_advertise_param = {
    0x20, 0x30,                       /* advertising interval */
    RBLE_GAP_ADV_CONN_UNDIR,          /* advertising channel PDU type */
    RBLE_ADDR_PUBLIC,                 /* own bluetooth address type */
    RBLE_ADDR_PUBLIC,                 /* peer bluetooth address type */
    { 0x0 },                          /* not used */
    RBLE_ADV_ALL_CHANNELS,            /* advertising channel */
    RBLE_ADV_ALLOW_SCAN_ANY_CON_ANY,  /* advertising policy */
    0x00,                             /* reserved */
    {                                 /* advertising data */
        3+9+18,
        2, 0x01, 0x06,
        8, 0x09, 'R', 'E', 'L', '-', 'B', 'L', 'E',
        17, 0x07,
        0x7A, 0x8D, 0xC1, 0x92, 0x36, 0xC4, 0x43, 0x90,
        0xAF, 0x40, 0xF1, 0xA1, 0xF7, 0xB9, 0xC1, 0x5B,
    },
    { 0x00 },                         /* scan response data */
};

const struct ke_state_handler app_default_handler = KE_STATE_HANDLER_NONE;

/*******************************************************************************
 * Function Definitions (Internal)
 ******************************************************************************/
/* ##### Message Handler ##### */
static void app_msg_send(ke_msg_id_t id)
{
    /* Send blank msg to itself. The content is shared through
       app_info global variable. */
    ke_msg_send_basic(id, APP_TASK_ID, APP_TASK_ID);
}

static int_t app_reset(ke_msg_id_t const msgid, void const *param,
                       ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    (void)led_onoff_init(R_LED4);

    (void)RBLE_GAP_Reset(&app_gap_callback, &app_sm_callback);

    return KE_MSG_CONSUMED;
}

static int_t app_advertise_start(ke_msg_id_t const msgid, void const *param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    (void)RBLE_GAP_Broadcast_Enable(
        RBLE_GAP_GEN_DISCOVERABLE, RBLE_GAP_UND_CONNECTABLE,
        &app_advertise_param);

    return KE_MSG_CONSUMED;
}

static int_t app_profile_enable(ke_msg_id_t const msgid, void const *param,
                                ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    SAMPLE_SERVER_PARAM samps_param = {0};

    (void)SAMPLE_Server_Enable(app_info.conhdl, RBLE_PRF_CON_DISCOVERY,
                               &samps_param, &app_sams_callback);

    return KE_MSG_CONSUMED;
}

static int_t app_profile_disable(ke_msg_id_t const msgid, void const *param,
                                 ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    (void)SAMPLE_Server_Disable(app_info.conhdl);

    return KE_MSG_CONSUMED;
}

static int_t app_timer_expired(ke_msg_id_t const msgid, void const *param,
                               ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    R_PUSH_STATE state;
    uint8_t value;

    /* Check SW4 push/release state and send the state to central continuously. */

    push_state(R_SW4, &state);
    value = (uint8_t)((state == R_PUSH_STATE_PUSH) ? SAMPLE_LED_CONTROL_ON : SAMPLE_LED_CONTROL_OFF);

    (void)SAMPLE_Server_Send_Switch_State(app_info.conhdl, value);

    ke_timer_set(APP_MSG_TIMER_EXPIRED, APP_TASK_ID, APP_SWITCH_STATE_CHECK_INTERVAL);

    return KE_MSG_CONSUMED;
}

/* ##### GAP Event Handler ##### */
void app_gap_callback(RBLE_GAP_EVENT *event)
{
    switch (event->type) {
    case RBLE_GAP_EVENT_RESET_RESULT:
        ke_state_set(APP_TASK_ID, APP_NONCONNECT_STATE);
        app_msg_send(APP_MSG_RESET_COMP);
        break;

    case RBLE_GAP_EVENT_BROADCAST_ENABLE_COMP:
    case RBLE_GAP_EVENT_BROADCAST_DISABLE_COMP:
        /* do nothing */
        break;

    case RBLE_GAP_EVENT_CONNECTION_COMP:
        if (RBLE_OK == event->param.status) {
            app_info.conhdl = event->param.conn_comp.connect_info.conhdl;
            ke_state_set(APP_TASK_ID, APP_CONNECT_STATE);
            app_msg_send(APP_MSG_CONNECTED);
        } else {
            ke_state_set(APP_TASK_ID, APP_NONCONNECT_STATE);
            app_msg_send(APP_MSG_DISCONNECTED);
        }
        break;

    case RBLE_GAP_EVENT_DISCONNECT_COMP:
        ke_state_set(APP_TASK_ID, APP_NONCONNECT_STATE);
        app_msg_send(APP_MSG_DISCONNECTED);
        break;

    default:
        Printf("unsupported event: 0x%x\n", event->type);
        break;
    }
}

/* ##### SM Event Handler ##### */
void app_sm_callback(RBLE_SM_EVENT *event)
{
    Printf("unsupported event: 0x%x\n", event->type);
}

/* ##### Sample Custom Profile Handler ##### */
void app_sams_callback(SAMPLE_SERVER_EVENT *event)
{
    switch (event->type) {
    case SAMPLE_SERVER_EVENT_ENABLE_COMP:
        app_msg_send(APP_MSG_PROFILE_ENABLED);
        break;

    case SAMPLE_SERVER_EVENT_DISABLE_COMP:
        app_msg_send(APP_MSG_PROFILE_DISABLED);
        break;

    case SAMPLE_SERVER_EVENT_CHG_LED_CONTROL_IND:
        /* Change LED on/off depends on led_control character value written from central. */
        if (event->param.change_led_control_ind.value == SAMPLE_LED_CONTROL_ON) {
            led_onoff_set(R_LED4, R_LED_STATE_ON);
        } else {
            led_onoff_set(R_LED4, R_LED_STATE_OFF);
        }
        break;

    case SAMPLE_SERVER_EVENT_WRITE_CHAR_RESPONSE:
        /* Start notification timer if switch_state characteristic cccd is set correctly. */
        if (event->param.write_char_resp.value & RBLE_PRF_START_NTF) {
            ke_timer_set(APP_MSG_TIMER_EXPIRED, APP_TASK_ID, APP_SWITCH_STATE_CHECK_INTERVAL);
        } else {
            ke_timer_clear(APP_MSG_TIMER_EXPIRED, APP_TASK_ID);
        }
        break;

    default:
        Printf("unsupported event: 0x%x\n", event->type);
        break;
    }
}

/* ##### Application Handler ##### */
void app_callback(RBLE_MODE mode)
{
    switch (mode) {
    case RBLE_MODE_ACTIVE:
        app_msg_send(APP_MSG_BOOTUP);
        break;

    case RBLE_MODE_INITIALIZE:
    case RBLE_MODE_RESET:
    case RBLE_MODE_SLEEP:
    case RBLE_MODE_ERROR:
        /* do nothing */
        break;

    default:
        Printf("unknown mode: 0x%x\n", mode);
        break;
    }
}

BOOL RBLE_App_Init(void)
{
    RBLE_STATUS status;

    status = RBLE_Init(&app_callback);
    if (RBLE_OK != status) {
        return FALSE;
    }

    ke_state_set(APP_TASK_ID, APP_RESET_STATE);

    console_init(false, 0);

    return TRUE;
}
