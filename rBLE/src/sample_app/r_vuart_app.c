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
*******************************************************************************/

/*******************************************************************************
 * File Name  : r_vuart_app.c
 * Version    : 1.0
 * Description: Application Code File
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 ******************************************************************************/

/*******************************************************************************
    Includes
********************************************************************************/
#ifdef WIN32
#include <windows.h>
#endif

#ifndef WIN32
#include "arch.h"
#include "dataflash.h"
#include "eel_descriptor_t02.h"
#include "r_vuart_console.h"
#else
#include <string.h>
#endif
#include "r_vuart_app.h"
#include "vuart/vuarts.h"
#include "vuart/vuartc.h"

#include "header.h"


/*******************************************************************************
    Macro Definitions
********************************************************************************/
#define ADV_DATA_TYPE_COMPLETE_128BIT_UUID (0x7)

#define APP_SELF_BDA   (EEL_ID_BDA)
#define APP_REMOTE_BDA (EEL_ID_REMOTE_BDA)

#define APP_ADV_LOW_POWER_DURATION (3000)

#define AT_COMMAND_MAX_LEN      (10)
#define AT_COMMAND_LINE_MAX_LEN (50)

#define APP_CON_INTV_MAX  (0x0C80)
#define APP_CON_INTV_MIN  (0x0006)
#define APP_CON_INTV_20MS (0x0010)

/*******************************************************************************
    Typedef Definitions
********************************************************************************/
typedef enum {
    APP_ADVERTISER,
    APP_SCANNER,
    APP_INITIATER,
    APP_CONNECT_MASTER,
    APP_CONNECT_SLAVE,
} app_state_t;

typedef enum {
    APP_ADV_NORMAL_CYCLE,
    APP_ADV_LOW_POWER_CYCLE,
} app_adv_cycle_t;

typedef enum {
    APP_KEY_NONE,
    APP_KEY_DISTRIBUTED,
    APP_KEY_PAIRED,
} app_sm_key_state_t;

typedef enum {
    APP_ADDR_NONE,
    APP_ADDR_INITIAL_SELF,
    APP_ADDR_INITIAL_REMOTE,
    APP_ADDR_GET_SELF,
    APP_ADDR_GET_REMOTE,
    APP_ADDR_SET_SELF,
    APP_ADDR_SET_REMOTE,
} app_addr_state_t;

typedef struct {
    app_state_t  state;
    uint16_t     conhdl;
    uint8_t      conidx;

    /* for address management */
    app_addr_state_t addr_state;
    RBLE_BD_ADDR self_bda;
    RBLE_BD_ADDR remote_bda;

    /* for SM */
    app_sm_key_state_t key_state;
    RBLE_RAND_NB nb;
    RBLE_KEY_VALUE ltk;
    uint16_t ediv;

    /* for AT-C=<addr> command */
    RBLE_BD_ADDR remote_temp_bda;
    BOOL use_remote_temp_bda;

    /* for AT-CI=<val> command */
    uint16_t req_con_intv;
    uint16_t cur_con_intv;
    uint16_t con_latency;
    uint16_t superv_to;

    /* for ATE0, ATE1 */
    BOOL is_echo_enabled;

    /* for AT command */
    BOOL in_exec;

    /* for Low power Advertising */
    uint8_t adv_cycle;
#ifdef WIN32
    UINT_PTR lp_timer_id;
#endif
} app_info_t;

typedef void (*at_command_func_t)(const char *arg);

typedef struct {
    const char str[AT_COMMAND_MAX_LEN];
    at_command_func_t func;
} at_command_t;

/*******************************************************************************
    Global Variables
********************************************************************************/
static app_info_t app;
static const uint8_t app_vuart_service_uuid[] = RBLE_SVC_VUART;

extern const APP_BROADCAST_ENABLE_PARAM g_app_default_broadcast_param[];
extern const RBLE_CREATE_CONNECT_PARAM g_app_default_connection_param[];
extern const RBLE_BOND_PARAM g_app_default_bond_param[];
extern const RBLE_BOND_RESP_PARAM g_app_default_bond_resp_param[];

/*******************************************************************************
    Function Declarations
********************************************************************************/
/* AT Command APIs */
static void at_connect(const char *arg);
static void at_reset(const char *arg);
static void at_search(const char *arg);
static void at_set_self_addr(const char *arg);
static void at_print_self_addr(const char *arg);
static void at_set_remote_addr(const char *arg);
static void at_print_remote_addr(const char *arg);
static void at_print_state(const char *arg);
static void at_con_intv(const char *arg);
static void at_print_con_intv(const char *arg);
static void at_echo_off(const char *arg);
static void at_echo_on(const char *arg);

/* GAP Function */
static void app_gap_reset(void);
static void app_gap_create_connection(void);
static void app_gap_disconnect(void);
static void app_gap_connection_cancel(void);
static void app_gap_broadcast_enable(app_adv_cycle_t index);
static void app_gap_broadcast_disable(void);
static void app_gap_device_search_cancel(void);
static void app_gap_set_bonding_mode(void);
static void app_gap_start_bonding(void);
static void app_gap_device_search(void);
static void app_gap_callback(RBLE_GAP_EVENT *event);

/* SM Functions */
static void app_sm_ltk_req_ind(RBLE_SM_EVENT *event);
static void app_sm_ltk_req_for_enc_ind(RBLE_SM_EVENT *event);
static void app_sm_key_ind(RBLE_SM_EVENT *event);
static void app_sm_callback(RBLE_SM_EVENT *event);

/* VS Functions */
static void app_vs_flash_management_comp(RBLE_VS_EVENT *event);
static void app_vs_flash_access_comp(RBLE_VS_EVENT *event);
static void app_vs_callback(RBLE_VS_EVENT *event);

/* VUART Functions */
static void app_vuart_server_callback(RBLE_VUART_EVENT *event);
static void app_vuart_client_callback(RBLE_VUART_EVENT *event);

/* AT Command Functions */
static void at_print_temp_response(const char *code);
static void at_print_response(const char *code);
static void at_print_event(const char *code);
static void at_print_bda_response(const char *command, RBLE_BD_ADDR *bda);
static void at_str_to_bda(const char *str, RBLE_BD_ADDR *bda);

/* Internal Functions */
static void app_change_state(app_state_t state);
static void app_print_bda(RBLE_BD_ADDR *ptr);

static void app_local_echo(const char *chars, uint16_t len);
static void rble_core_callback(RBLE_MODE mode);

/* Timer Functions */
#ifndef WIN32
static int_t app_set_low_power_advertising(ke_msg_id_t const msgid, void const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id);

const struct ke_msg_handler rble_app_handler[] =
{
    { RBLE_APP_SLEEP_EVT, (ke_msg_func_t)RBLE_Set_Sleepable },
    { RBLE_APP_ADV_EVT, (ke_msg_func_t)app_set_low_power_advertising },
};
const struct ke_state_handler rble_app_state_handler[RBLE_APP_STATE_MAX] =
{
    KE_STATE_HANDLER(rble_app_handler)
};
const struct ke_state_handler rble_app_default_handler = KE_STATE_HANDLER_NONE;
ke_state_t rble_app_state[RBLE_APP_IDX_MAX];
#endif

/*******************************************************************************
    Function Definitions
********************************************************************************/
static at_command_t commands[] = {
    { "AT-C",   at_connect },
    { "AT-R",   at_reset },
    { "AT-DS",  at_search },
    { "AT-AS",  at_set_self_addr },
    { "AT-AS?", at_print_self_addr },
    { "AT-AP",  at_set_remote_addr },
    { "AT-AP?", at_print_remote_addr },
    { "AT-S?",  at_print_state },
    { "AT-CI",  at_con_intv },
    { "AT-CI?", at_print_con_intv },
    { "ATE0",   at_echo_off },
    { "ATE1",   at_echo_on },
};

/*******************************************************************************
 * Function Name: R_AT_Parse
 * Description  : Parse and execute AT command
 * Arguments    : str - AT command
 *                len - AT command length
 * Return Value : None
*******************************************************************************/
void R_AT_Parse(uint8_t *str, uint16_t len)
{
    const char delimiters[] = "\t =";
    char cmd_str[AT_COMMAND_LINE_MAX_LEN] = {' '};
    char *cmd = NULL;
    char *arg = NULL;
    uint16_t i = 0;
    uint16_t size;

    if (TRUE == app.in_exec) {
        at_print_temp_response("ERROR");
        return;
    }

    if (AT_COMMAND_LINE_MAX_LEN < len) {
        at_print_temp_response("ERROR");
        return;
    }

    size = sizeof(commands) / sizeof(commands[0]);

    /* because strtok() changes its first argument */
    memcpy(cmd_str, str, len);

    cmd = (char *)strtok(cmd_str, delimiters);
    arg = (char *)strtok(NULL, delimiters);

    for (i = 0; i < size; i++) {
        if ((strlen(commands[i].str) == strlen(cmd)) &&
            (0 == memcmp(commands[i].str, cmd, strlen(cmd)))) {
            app.in_exec = TRUE;
            commands[i].func(arg);
            break;
        }
    }

    if (size == i) {
        at_print_temp_response("ERROR");
    }
}

/*******************************************************************************
 * Function Name: at_connect
 * Description  : Start connection
 * Arguments    : arg - Bluetooth device address to connect in string format
 * Return Value : None
*******************************************************************************/
static void at_connect(const char *arg)
{
    RBLE_BD_ADDR bda;

    if (NULL != arg) {
        at_str_to_bda(arg, &bda);
        memcpy(&app.remote_temp_bda, &bda, sizeof(bda));
        app.use_remote_temp_bda = TRUE;
    }

    switch (app.state) {
    case APP_ADVERTISER:
        app_change_state(APP_INITIATER);
        app_gap_broadcast_disable();
        at_print_response("OK");
        break;

    case APP_SCANNER:
        app_change_state(APP_INITIATER);
        app_gap_device_search_cancel();
        at_print_response("OK");
        break;

    case APP_INITIATER:
        app_gap_connection_cancel();
        at_print_response("OK");
        break;

    default:
        at_print_response("ERROR");
        break;
    }
}

/*******************************************************************************
 * Function Name: at_reset
 * Description  : Reset application state to ADVERTISER
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_reset(const char *arg)
{
    switch (app.state) {
    case APP_ADVERTISER:
        app_gap_broadcast_disable();
        at_print_response("OK");
        break;

    case APP_SCANNER:
        app_change_state(APP_ADVERTISER);
        app_gap_device_search_cancel();
        at_print_response("OK");
        break;

    case APP_INITIATER:
        app_change_state(APP_ADVERTISER);
        app_gap_connection_cancel();
        at_print_response("OK");
        break;

    case APP_CONNECT_MASTER:
    case APP_CONNECT_SLAVE:
        app_gap_disconnect();
        at_print_response("OK");
        break;

    default:
        at_print_response("ERROR");
        break;
    }
}

/*******************************************************************************
 * Function Name: at_search
 * Description  : Search near bluetoot device which support VUART Profile
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_search(const char *arg)
{
    switch (app.state) {
    case APP_ADVERTISER:
        app_change_state(APP_SCANNER);
        app_gap_broadcast_disable();
        break;

    case APP_SCANNER:
        app_gap_device_search();
        break;

    case APP_INITIATER:
        app_change_state(APP_SCANNER);
        app_gap_connection_cancel();
        break;

    default:
        at_print_response("ERROR");
        break;
    }
}

/*******************************************************************************
 * Function Name: at_set_self_addr
 * Description  : Set self bluetooth device address
 * Arguments    : arg - Bluetooth device address in string format
 * Return Value : None
*******************************************************************************/
static void at_set_self_addr(const char *arg)
{
    if (NULL != arg) {
        at_str_to_bda(arg, &app.self_bda);
    }

    if ((APP_ADVERTISER == app.state) ||
        (APP_SCANNER == app.state) ||
        (APP_INITIATER == app.state)) {

        at_print_response("OK");

        app.addr_state = APP_ADDR_SET_SELF;
        CHECK_OK(RBLE_VS_Flash_Management(RBLE_VS_FLASH_CMD_START));
    } else {
        /* case APP_CONNECT_MASTER: */
        /* case APP_CONNECT_SLAVE: */
        at_print_response("ERROR");
    }
}

/*******************************************************************************
 * Function Name: at_print_self_addr
 * Description  : Print self bluetooth device address
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_print_self_addr(const char *arg)
{
    app.addr_state = APP_ADDR_GET_SELF;
    CHECK_OK(RBLE_VS_Flash_Management(RBLE_VS_FLASH_CMD_START));
}

/*******************************************************************************
 * Function Name: at_set_remote_addr
 * Description  : Set remote bluetooth device address
 * Arguments    : arg - Bluetooth device address in string format
 * Return Value : None
*******************************************************************************/
static void at_set_remote_addr(const char *arg)
{
    if (NULL != arg) {
        at_str_to_bda(arg, &app.remote_bda);
    }

    if ((APP_ADVERTISER == app.state) ||
        (APP_SCANNER == app.state) ||
        (APP_INITIATER == app.state)) {

        at_print_response("OK");

        app.addr_state = APP_ADDR_SET_REMOTE;
        CHECK_OK(RBLE_VS_Flash_Management(RBLE_VS_FLASH_CMD_START));
    } else {
        /* case APP_CONNECT_MASTER: */
        /* case APP_CONNECT_SLAVE: */
        at_print_response("ERROR");
    }
}

/*******************************************************************************
 * Function Name: at_print_remote_addr
 * Description  : Print remote bluetooth device address
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_print_remote_addr(const char *arg)
{
    app.addr_state = APP_ADDR_GET_REMOTE;
    CHECK_OK(RBLE_VS_Flash_Management(RBLE_VS_FLASH_CMD_START));
}

/*******************************************************************************
 * Function Name: at_print_state
 * Description  : Print current connection state
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_print_state(const char *arg)
{
    int state;

    const char *names[] = {
        "CONNECT",
        "DISCONNECT",
    };

    if ((APP_CONNECT_MASTER == app.state) ||
        (APP_CONNECT_SLAVE == app.state)) {
        state = 0;
    } else {
        state = 1;
    }

    at_print_response(names[state]);
    at_print_response("OK");
}

/*******************************************************************************
 * Function Name: at_con_intv
 * Description  : Change connection interval
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_con_intv(const char *arg)
{
    uint16_t con_intv;

    con_intv = (uint16_t)strtol((char *)arg, NULL, 10);

    if ((APP_ADVERTISER == app.state) || (APP_SCANNER == app.state)) {
        if ((con_intv < APP_CON_INTV_MIN) || (con_intv > APP_CON_INTV_MAX)) {
            at_print_response("ERROR");
        }
        else {
            app.req_con_intv = con_intv;
            at_print_response("OK");
        }
    }
    else {
        at_print_response("ERROR");
    }
}

/*******************************************************************************
 * Function Name: at_print_con_intv
 * Description  : Print connection interval
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_print_con_intv(const char *arg)
{
    char str[10] = { 0 };

    switch (app.state) {
    case APP_ADVERTISER:
    case APP_SCANNER:
        sprintf(str, "-CI: %d", app.req_con_intv);
        at_print_response(str);
        at_print_response("OK");
        break;

    case APP_CONNECT_SLAVE:
    case APP_CONNECT_MASTER:
        sprintf(str, "-CI: %d", app.cur_con_intv);
        at_print_response(str);
        at_print_response("OK");
        break;

    default:
        at_print_response("ERROR");
        break;
    }
}

/*******************************************************************************
 * Function Name: at_echo_off
 * Description  : Disable local echo
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_echo_off(const char *arg)
{
    Console_Set_Echo(FALSE);
    app.is_echo_enabled = FALSE;
    at_print_response("OK");
}

/*******************************************************************************
 * Function Name: at_echo_on
 * Description  : Enable local echo
 * Arguments    : arg - Not used
 * Return Value : None
*******************************************************************************/
static void at_echo_on(const char *arg)
{
    Console_Set_Echo(TRUE);
    app.is_echo_enabled = TRUE;
    at_print_response("OK");
}

/*******************************************************************************
 * Function Name: at_print_temp_response
 * Description  : Print AT command response without clearing in_exec flag
 * Arguments    : AT command response code
 * Return Value : None
*******************************************************************************/
static void at_print_temp_response(const char *code)
{
    Printf("%c%c%s%c%c", CR_KEY_CODE, LF_KEY_CODE, code, CR_KEY_CODE, LF_KEY_CODE);
}

/*******************************************************************************
 * Function Name: at_print_response
 * Description  : Print AT command response
 * Arguments    : AT command response code
 * Return Value : None
*******************************************************************************/
static void at_print_response(const char *code)
{
    app.in_exec = FALSE;
    Printf("%c%c%s%c%c", CR_KEY_CODE, LF_KEY_CODE, code, CR_KEY_CODE, LF_KEY_CODE);
}

/*******************************************************************************
 * Function Name: at_print_event
 * Description  : Print AT command event
 * Arguments    : AT command event code
 * Return Value : None
*******************************************************************************/
static void at_print_event(const char *code)
{
    Printf("%c%c%s%c%c", CR_KEY_CODE, LF_KEY_CODE, code, CR_KEY_CODE, LF_KEY_CODE);
}

/*******************************************************************************
 * Function Name: at_print_bda_response
 * Description  : Print AT command response with bluetooth device address
 * Arguments    : command - AT command name
 *                bda - bluetooth device address to print
 * Return Value : None
*******************************************************************************/
static void at_print_bda_response(const char *command, RBLE_BD_ADDR *bda)
{
    Printf("%c%c", CR_KEY_CODE, LF_KEY_CODE);
    Printf("%s: ", command);
    app_print_bda(bda);
    Printf("%c%c", CR_KEY_CODE, LF_KEY_CODE);
}

/*******************************************************************************
 * Function Name: at_str_to_bda
 * Description  : Convet string bluetooth device address to int value
 * Arguments    : str - string to parse
 *                bda - bluetooth device address
 * Return Value : None
*******************************************************************************/
static void at_str_to_bda(const char *str, RBLE_BD_ADDR *bda)
{
    int16_t bd_pos;
    uint16_t str_pos;
    static char bda_str[RBLE_BD_ADDR_LEN*2];

    memset(bda_str, '0', RBLE_BD_ADDR_LEN*2);

    /* firstly make 12 digit bda string */
    if (strlen(str) >= RBLE_BD_ADDR_LEN*2) {
        memcpy(bda_str, &str[strlen(str)-RBLE_BD_ADDR_LEN*2], RBLE_BD_ADDR_LEN*2);
    } else {
        memcpy(&bda_str[RBLE_BD_ADDR_LEN*2-strlen(str)], str, strlen(str));
    }

    /* convert 12 digit bda string to bda number */
    for (bd_pos = RBLE_BD_ADDR_LEN - 1, str_pos = 0; bd_pos >= 0; bd_pos--, str_pos+=2) {
        uint8_t val_str[2];
        val_str[0] = bda_str[str_pos];
        val_str[1] = bda_str[str_pos+1];
        bda->addr[bd_pos] = (uint8_t)strtol((char *)&val_str, NULL, 16);
    }
}


/*******************************************************************************
 * Function Name: app_change_state
 * Description  : Change application state
 * Arguments    : state - state after transit
 * Return Value : None
*******************************************************************************/
static void app_change_state(app_state_t state)
{
#ifdef APP_DEBUG
    const char *names[] = {
        "ADVERTISER",
        "SCANNER",
        "INITIATER",
        "CONNECT_MASTER",
        "CONNECT_SLAVE",
    };

    DPRINT("[%s -> %s]\n", names[app.state], names[state]);
#endif

    app.state = state;
}

/*******************************************************************************
 * Function Name: app_rand
 * Description  : Pseudo-random function
 * Arguments    : None
 * Return Value : Pseudo-random value
*******************************************************************************/
static uint8_t app_rand(void)
{
    return (uint8_t)(rand() & 0xFF);
}

/*******************************************************************************
 * Function Name: app_print_bda
 * Description  : Print bluetooth device address
 * Arguments    : bda - bluetooth address to print
 * Return Value : None
*******************************************************************************/
static void app_print_bda(RBLE_BD_ADDR *bda)
{
    Printf("%02x%02x%02x%02x%02x%02x",
           bda->addr[5], bda->addr[4], bda->addr[3],
           bda->addr[2], bda->addr[1], bda->addr[0]);
}

/*******************************************************************************
 * Function Name: app_local_echo
 * Description  : Print character, CR/LF is repalced by '\n'
 * Arguments    : c - Character to print
 * Return Value : None
*******************************************************************************/
static void app_local_echo(const char *chars, uint16_t len)
{
    if (TRUE == app.is_echo_enabled) {
        Printf("%.*s", len, chars);
    }
}

/*******************************************************************************
 * Function Name: app_set_low_power_advertising
 * Description  : Set low power advertising interval
 * Arguments    : msgid - Message ID
 *                param - parameter (NULL)
 *                dest_id - Destination ID
 *                src_id - Source Id
 * Return Value : Message handling result
*******************************************************************************/
#ifdef WIN32
void CALLBACK app_set_low_power_advertising(HWND hwnd, UINT uMsg, UINT idEvent, DWORD dwTime)
{
    if (APP_ADVERTISER == app.state) {
        app_gap_broadcast_enable(APP_ADV_LOW_POWER_CYCLE);
        KillTimer(NULL, app.lp_timer_id);
    }
}
#else
static int_t app_set_low_power_advertising(ke_msg_id_t const msgid, void const *param,
        ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if (APP_ADVERTISER == app.state) {
        app_gap_broadcast_enable(APP_ADV_LOW_POWER_CYCLE);
    }

    return KE_MSG_CONSUMED;
}
#endif

/*******************************************************************************
 * Function Name: app_gap_reset
 * Description  : Rest GAP system
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_gap_reset(void)
{
    CHECK_OK(RBLE_GAP_Reset(&app_gap_callback, &app_sm_callback));
}

/*******************************************************************************
 * Function Name: app_gap_device_search_cancel
 * Description  : Cancel device search
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_gap_device_search_cancel(void)
{
    CHECK_OK(RBLE_GAP_Device_Search(RBLE_GAP_CANCEL_DISCOVERY, 0));
}

/*******************************************************************************
 * Function Name: app_gap_broadcast_enable
 * Description  : Enable broadcast
 * Arguments    : index - Advertising interval cycle setting
 * Return Value : None
*******************************************************************************/
static void app_gap_broadcast_enable(app_adv_cycle_t index)
{
    APP_BROADCAST_ENABLE_PARAM param;

    memcpy(&param, &g_app_default_broadcast_param[index], sizeof(param));
    app.adv_cycle = index;

    CHECK_OK(RBLE_GAP_Broadcast_Enable(param.disc_mode, param.conn_mode, &param.adv_info));
}

/*******************************************************************************
 * Function Name: app_gap_broadcast_disable
 * Description  : Disable broadcast
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_gap_broadcast_disable(void)
{
    CHECK_OK(RBLE_GAP_Broadcast_Disable());
}

/*******************************************************************************
 * Function Name: app_gap_device_search
 * Description  : Start device search
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_gap_device_search(void)
{
    CHECK_OK(RBLE_GAP_Device_Search(RBLE_GAP_GEN_DISCOVERY_TYPE, RBLE_ADDR_RAND));
}

/*******************************************************************************
 * Function Name: app_gap_create_connection
 * Description  : Create connection to specified bluetooth device address
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_gap_create_connection(void)
{
    RBLE_CREATE_CONNECT_PARAM param;

    memcpy(&param, &g_app_default_connection_param, sizeof(param));

    if (TRUE == app.use_remote_temp_bda) {
        memcpy(&param.peer_addr, &app.remote_temp_bda, RBLE_BD_ADDR_LEN);
    } else {
        memcpy(&param.peer_addr, &app.remote_bda, RBLE_BD_ADDR_LEN);
    }

    param.con_intv_max = app.req_con_intv;
    param.con_intv_min = app.req_con_intv;

    CHECK_OK(RBLE_GAP_Create_Connection(&param));
}

/*******************************************************************************
 * Function Name: app_gap_connection_cancel
 * Description  : Cancel connection
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_gap_connection_cancel(void)
{
    CHECK_OK(RBLE_GAP_Connection_Cancel());
}

/*******************************************************************************
 * Function Name: app_gap_disconnect
 * Description  : RBLE_GAP_Disconnect wrapper with this applicatin settings
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_gap_disconnect(void)
{
    CHECK_OK(RBLE_GAP_Disconnect(app.conhdl));
}

/*******************************************************************************
 * Function Name: app_gap_start_bonding
 * Description  : Start bonding
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_start_bonding(void)
{
    RBLE_BOND_PARAM bond_param;

    memcpy(&bond_param, &g_app_default_bond_param, sizeof(bond_param));

    if (TRUE == app.use_remote_temp_bda) {
        memcpy(&bond_param.addr, &app.remote_temp_bda, RBLE_BD_ADDR_LEN);
    } else {
        memcpy(&bond_param.addr, &app.remote_bda, RBLE_BD_ADDR_LEN);
    }

    app.use_remote_temp_bda = FALSE;

    CHECK_OK(RBLE_GAP_Start_Bonding(&bond_param));
}

/*******************************************************************************
 * Function Name: app_gap_set_bonding_mode
 * Description  : Set bonding mode
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_gap_set_bonding_mode(void)
{
    CHECK_OK(RBLE_GAP_Set_Bonding_Mode(RBLE_GAP_BONDABLE));
}

/*******************************************************************************
 * Function Name: app_start_enc
 * Description  : Start encryption
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_start_enc(void)
{
    CHECK_OK(RBLE_SM_Start_Enc(app.conidx, RBLE_AUTH_REQ_NO_MITM_BOND,
                               app.ediv, &app.nb, &app.ltk));
}

/*******************************************************************************
 * Function Name: app_gap_reset_result
 * Description  : RBLE_GAP_EVENT_RESET_RESULT event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_reset_result(RBLE_GAP_EVENT *event)
{
    /* to start STOP mode timer in console */
    Printf(" \b");

    memset(&app, 0x00, sizeof(app));
    app.req_con_intv = g_app_default_connection_param[0].con_intv_max;

#ifdef DISABLE_LOCAL_ECHO_BY_DEFAULT
    app.is_echo_enabled = FALSE;
#else
    app.is_echo_enabled = TRUE;
#endif

    CHECK_OK(RBLE_VS_Enable(app_vs_callback));

    // RD8001???
//    app.addr_state = APP_ADDR_INITIAL_SELF;
    app.addr_state = APP_ADDR_INITIAL_REMOTE;

    CHECK_OK(RBLE_VS_Flash_Management(RBLE_VS_FLASH_CMD_START));
}

/*******************************************************************************
 * Function Name: app_gap_device_search_comp
 * Description  : RBLE_GAP_EVENT_OBSERVATION_DISABLE_COMP event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_device_search_comp(RBLE_GAP_EVENT *event)
{
    switch (app.state) {
    case APP_ADVERTISER:
        app_gap_broadcast_enable(APP_ADV_NORMAL_CYCLE);
        break;

    case APP_SCANNER:
        at_print_response("OK");
        break;

    case APP_INITIATER:
        app_gap_create_connection();
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_gap_broadcast_enable_comp
 * Description  : RBLE_GAP_EVENT_BROADCAST_ENABLE_COMP event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_broadcast_enable_comp(RBLE_GAP_EVENT *event)
{
    if (APP_ADV_NORMAL_CYCLE == app.adv_cycle) {
#ifdef WIN32
        app.lp_timer_id = SetTimer(NULL, 0, APP_ADV_LOW_POWER_DURATION, app_set_low_power_advertising);
#else
        ke_timer_set(RBLE_APP_ADV_EVT, TASK_CON_APPL, APP_ADV_LOW_POWER_DURATION);
#endif
    }
}

/*******************************************************************************
 * Function Name: app_gap_broadcast_disable_comp
 * Description  : RBLE_GAP_EVENT_BROADCAST_DISABLE_COMP event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_broadcast_disable_comp(RBLE_GAP_EVENT *event)
{
    switch (app.state) {
    case APP_ADVERTISER:
        app_gap_broadcast_enable(APP_ADV_NORMAL_CYCLE);
        break;

    case APP_SCANNER:
        app_gap_device_search();
        break;

    case APP_INITIATER:
        app_gap_create_connection();
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_gap_set_bonding_mode_comp
 * Description  : RBLE_GAP_EVENT_SET_BONDING_MODE_COMP event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_set_bonding_mode_comp(RBLE_GAP_EVENT *event)
{
    switch (app.state) {
    case APP_ADVERTISER:
        app_gap_broadcast_enable(APP_ADV_NORMAL_CYCLE);
        break;

    case APP_SCANNER:
        /* do nothign */
        break;

    case APP_INITIATER:
        app_gap_create_connection();
        break;

    default:
        EPRINT("unknown event: 0x%x", event->type);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_gap_device_search_result_ind
 * Description  : RBLE_GAP_EVENT_DEVICE_SEARCH_RESULT_IND event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_device_search_result_ind(RBLE_GAP_EVENT *event)
{
    RBLE_ADV_REPORT *adv_resp;
    uint8_t uuid[RBLE_GATT_128BIT_UUID_OCTET] = {0};
    uint16_t pos;
    uint16_t len;
    uint16_t type;

    adv_resp = &event->param.dev_search_result.adv_resp;

    for (pos = 0; pos < RBLE_ADV_DATA_LEN; pos += len) {
        len = adv_resp->data[pos] - 1;
        pos++;
        type = adv_resp->data[pos];
        pos++;

        switch (type) {
        case ADV_DATA_TYPE_COMPLETE_128BIT_UUID:
            memcpy(uuid, &adv_resp->data[pos], len);
            break;

        default:
            /* do nothing */
            break;
        }
    }

    /* only print address of device which supports Virtual UART profile */
    if (0 == memcmp(uuid, app_vuart_service_uuid, RBLE_GATT_128BIT_UUID_OCTET)) {
        at_print_bda_response("-DS", &adv_resp->adv_addr);
    }
}

/*******************************************************************************
 * Function Name: app_gap_connect_comp
 * Description  : RBLE_GAP_EVENT_CONNECTION_COMP event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_connect_comp(RBLE_GAP_EVENT *event)
{
    RBLE_CONNECT_INFO *con_info;

    con_info = &event->param.conn_comp.connect_info;

    if (RBLE_OK == con_info->status) {
        app.conhdl = con_info->conhdl;
        app.conidx = con_info->idx;

        if (APP_INITIATER == app.state) {
            app_change_state(APP_CONNECT_MASTER);
        } else {
            app_change_state(APP_CONNECT_SLAVE);
        }

        app.superv_to    = event->param.conn_comp.connect_info.sup_to;
        app.con_latency  = event->param.conn_comp.connect_info.con_latency;
        app.cur_con_intv = event->param.conn_comp.connect_info.con_interval;

        switch (app.state) {
        case APP_CONNECT_MASTER:
            app_gap_start_bonding();
            break;

        case APP_CONNECT_SLAVE:
            CHECK_OK(RBLE_VUART_Server_Enable(app.conhdl, app_vuart_server_callback));
            break;

        default:
            EPRINT("not intended state: 0x%x", app.state);
            break;
        }
    }
}

/*******************************************************************************
 * Function Name: app_gap_connection_cancel_comp
 * Description  : RBLE_GAP_EVENT_CONNECTION_CANCEL_COMP event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_connection_cancel_comp(RBLE_GAP_EVENT *event)
{
    switch (app.state) {
    case APP_ADVERTISER:
        app_gap_broadcast_enable(APP_ADV_NORMAL_CYCLE);
        break;

    case APP_INITIATER:
        app_gap_create_connection();
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_gap_disconnect_comp
 * Description  : RBLE_GAP_EVENT_DISCONNECT_COMP event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_disconnect_comp(RBLE_GAP_EVENT *event)
{
    switch (app.state) {
    case APP_CONNECT_MASTER:
        at_print_event("DISCONNECT");
        app_change_state(APP_ADVERTISER);
        app_gap_broadcast_enable(APP_ADV_NORMAL_CYCLE);
        CHECK_OK(RBLE_VUART_Client_Disable());
        break;

    case APP_CONNECT_SLAVE:
        at_print_event("DISCONNECT");
        app_change_state(APP_ADVERTISER);
        app_gap_broadcast_enable(APP_ADV_NORMAL_CYCLE);
        CHECK_OK(RBLE_VUART_Server_Disable());
        app.key_state = APP_KEY_NONE;
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_gap_bonding_comp
 * Description  : RBLE_GAP_EVENT_BONDING_COMP event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_bonding_comp(RBLE_GAP_EVENT *event)
{
    switch (app.state) {
    case APP_CONNECT_MASTER:
        app_start_enc();
        break;

    case APP_CONNECT_SLAVE:
        app.key_state = APP_KEY_PAIRED;
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_gap_bonding_req_ind
 * Description  : RBLE_GAP_EVENT_BONDING_REQ_IND event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_bonding_req_ind(RBLE_GAP_EVENT *event)
{
    RBLE_BOND_RESP_PARAM res_bond_param;

    memcpy(&res_bond_param, g_app_default_bond_resp_param, sizeof(res_bond_param));
    res_bond_param.conhdl = app.conhdl;

    CHECK_OK(RBLE_GAP_Bonding_Response(&res_bond_param));
}


/*******************************************************************************
 * Function Name: app_gap_change_connection_param_req_ind
 * Description  : RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_REQ_IND event callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_change_connection_param_req_ind(RBLE_GAP_EVENT *event)
{
    CHECK_OK(RBLE_GAP_Change_Connection_Param(app.conhdl, 0x0000,
                                              &event->param.chg_connect_param_req.conn_param,
                                              RBLE_MASTER));
}

/*******************************************************************************
 * Function Name: app_gap_callback
 * Description  : GAP callback
 * Arguments    : event - GAP event struct
 * Return Value : None
*******************************************************************************/
static void app_gap_callback(RBLE_GAP_EVENT *event)
{
    switch (event->type) {
    case RBLE_GAP_EVENT_RESET_RESULT:
        app_gap_reset_result(event);
        break;

    case RBLE_GAP_EVENT_OBSERVATION_DISABLE_COMP:
        app_gap_device_search_comp(event);
        break;

    case RBLE_GAP_EVENT_BROADCAST_ENABLE_COMP:
        app_gap_broadcast_enable_comp(event);
        break;

    case RBLE_GAP_EVENT_BROADCAST_DISABLE_COMP:
        app_gap_broadcast_disable_comp(event);
        break;

    case RBLE_GAP_EVENT_SET_BONDING_MODE_COMP:
        app_gap_set_bonding_mode_comp(event);
        break;

    case RBLE_GAP_EVENT_BONDING_COMP:
        app_gap_bonding_comp(event);
        break;

    case RBLE_GAP_EVENT_DEVICE_SEARCH_COMP:
        app_gap_device_search_comp(event);
        break;

    case RBLE_GAP_EVENT_DEVICE_SEARCH_RESULT_IND:
        app_gap_device_search_result_ind(event);
        break;

    case RBLE_GAP_EVENT_CONNECTION_COMP:
        app_gap_connect_comp(event);
        break;

    case RBLE_GAP_EVENT_CONNECTION_CANCEL_COMP:
        app_gap_connection_cancel_comp(event);
        break;

    case RBLE_GAP_EVENT_DISCONNECT_COMP:
        app_gap_disconnect_comp(event);
        break;

    case RBLE_GAP_EVENT_BONDING_REQ_IND:
        app_gap_bonding_req_ind(event);
        break;

    case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_COMP:
        app.cur_con_intv = event->param.chg_connect_param.con_interval;
        break;

    case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_REQ_IND:
        app_gap_change_connection_param_req_ind(event);
        break;

    case RBLE_GAP_EVENT_CHANGE_CONNECTION_PARAM_RESPONSE:
#ifdef APP_DEBUG
        DPRINT("[rsp] con_param_change status:%d, result:%d, interval:%d, superv_to:%d, latency:%d\n",
               event->param.chg_connect_param_resp.status,
               event->param.chg_connect_param_resp.result,
               app.req_con_intv, app.superv_to, app.con_latency);
#endif
        break;

    default:
        EPRINT("unknown event: 0x%x", event->type);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_vuart_server_enable_comp
 * Description  : RBLE_VUART_EVENT_SERVER_ENABLE_CMP event handler
 * Arguments    : event - VUART event struct
 * Return Value : None
*******************************************************************************/
static void app_vuart_server_enable_comp(RBLE_VUART_EVENT *event)
{
    RBLE_CONN_PARAM con_param;

    /* Apple products have some restriction on connection paramter settings. See Apple Bluetooth Design Guidelines */
    con_param.intv_min = (app.req_con_intv >= (APP_CON_INTV_MIN + APP_CON_INTV_20MS))
        ? app.req_con_intv - APP_CON_INTV_20MS : APP_CON_INTV_MIN;
    con_param.intv_max = app.req_con_intv;
    con_param.latency  = app.con_latency;
    con_param.time_out = app.superv_to;
    RBLE_GAP_Change_Connection_Param(app.conhdl, 0x0000, &con_param, RBLE_SLAVE);
}

/*******************************************************************************
 * Function Name: app_vuart_server_callback
 * Description  : VUART Server callback
 * Arguments    : event - VUART event struct
 * Return Value : None
*******************************************************************************/
static void app_vuart_server_callback(RBLE_VUART_EVENT *event)
{
    switch (event->type) {
    case RBLE_VUART_EVENT_SERVER_ENABLE_CMP:
        at_print_event("CONNECT");
        app_vuart_server_enable_comp(event);
        break;

    case RBLE_VUART_EVENT_SERVER_WRITE_REQ:
    	// RD8001対応：受信完了
    	ds_set_vuart_data( (UB*)&event->param.client_indication.value[0], event->param.client_indication.len );
        Printf("%.*s",
               event->param.server_write_req.len,
               event->param.server_write_req.value);
        break;

    case RBLE_VUART_EVENT_SERVER_INDICATION_CFM:
    	// RD8001対応：送信完了
    	ds_set_vuart_send_status( OFF );
        /* do nothing */
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_vuart_client_callback
 * Description  : VUART Client callback
 * Arguments    : event - VUART event struct
 * Return Value : None
*******************************************************************************/
static void app_vuart_client_callback(RBLE_VUART_EVENT *event)
{
    switch (event->type) {
    case RBLE_VUART_EVENT_CLIENT_ENABLE_CMP:
        at_print_event("CONNECT");
        break;

    case RBLE_VUART_EVENT_CLIENT_INDICATION:
        Printf("%.*s",
               event->param.client_indication.len,
               event->param.client_indication.value);
        break;

    case RBLE_VUART_EVENT_CLIENT_WRITE_RSP:
        /* do nothing */
        break;

    default:
        /* do nothing */
        break;
    }
}

/*******************************************************************************
 * Function Name: R_APP_VUART_Send_Char
 * Description  : Send character via VUART profile
 * Arguments    : c - character to send
 * Return Value : None
*******************************************************************************/
void R_APP_VUART_Send_Char(const char *chars, uint16_t len)
{
    uint16_t pos = 0;
    uint16_t send_len;

    while (pos < len) {
        send_len = ((len - pos) > VUART_SEND_BUFFER_SIZE) ? VUART_SEND_BUFFER_SIZE : len - pos;

        switch (app.state) {
        case APP_CONNECT_MASTER:
            app_local_echo(&chars[pos], send_len);
            RBLE_VUART_Client_Send_Chars((char *)&chars[pos], send_len);
            break;

        case APP_CONNECT_SLAVE:
            app_local_echo(&chars[pos], send_len);
            RBLE_VUART_Server_Send_Indication((char *)&chars[pos], send_len);
            break;

        default:
            Printf("Not connected\n");
            break;
        }

        pos += send_len;
    }
}

/*******************************************************************************
 * Function Name: app_sm_enc_start_ind
 * Description  : RBLE_SM_ENC_START_IND event callback
 * Arguments    : c - character to send
 * Return Value : None
*******************************************************************************/
static void app_sm_enc_start_ind(RBLE_SM_EVENT *event)
{
    switch (app.state) {
    case APP_CONNECT_MASTER:
        CHECK_OK(RBLE_VUART_Client_Enable(app.conhdl, app_vuart_client_callback));
        break;

    case APP_CONNECT_SLAVE:
        /* do nothing */
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_sm_ltk_req_ind
 * Description  : RBLE_SM_LTK_REQ_IND event handler
 * Arguments    : event - SM event struct
 * Return Value : None
*******************************************************************************/
static void app_sm_ltk_req_ind(RBLE_SM_EVENT *event)
{
    uint16_t i;

    switch (app.state) {
    case APP_CONNECT_SLAVE:
        if (APP_KEY_NONE == app.key_state) {
            for (i = 0; i < RBLE_RAND_NB_LEN; i++) {
                app.nb.nb[i] = app_rand();
            }
            for (i = 0; i < RBLE_KEY_LEN; i++) {
                app.ltk.key[i] = app_rand();
            }
            app.ediv = 0x1234;
        }

        CHECK_OK(RBLE_SM_Ltk_Req_Resp(app.conidx, RBLE_OK, RBLE_SMP_KSEC_UNAUTH_NO_MITM,
                                      app.ediv, &app.nb, &app.ltk));
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_sm_ltk_req_for_enc_ind
 * Description  : RBLE_SM_LTK_REQ_FOR_ENC_IND event handler
 * Arguments    : event - SM event struct
 * Return Value : None
*******************************************************************************/
static void app_sm_ltk_req_for_enc_ind(RBLE_SM_EVENT *event)
{
    uint8_t status = RBLE_OK;

    switch (app.state) {
    case APP_CONNECT_SLAVE:
        if ((APP_KEY_PAIRED != app.key_state) ||
            (event->param.ltk_req_for_enc.ediv != app.ediv) ||
            (0 != memcmp(&event->param.ltk_req_for_enc.nb, &app.nb, sizeof(app.nb)))) {
            status = RBLE_ERR;
        }

        CHECK_OK(RBLE_SM_Ltk_Req_Resp(app.conidx, status, RBLE_SMP_KSEC_UNAUTH_NO_MITM,
                                      app.ediv, &app.nb, &app.ltk));
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
    }
}

/*******************************************************************************
 * Function Name: app_sm_key_ind
 * Description  : RBLE_SM_KEY_IND event handler
 * Arguments    : event - SM event struct
 * Return Value : None
*******************************************************************************/
static void app_sm_key_ind(RBLE_SM_EVENT *event)
{
    struct RBLE_EVT_SM_Key_t *result =
        (struct RBLE_EVT_SM_Key_t *)&event->param.key_ind;

    switch (app.state) {
    case APP_CONNECT_MASTER:
        memcpy(&app.nb, &result->nb, RBLE_RAND_NB_LEN);
        memcpy(&app.ltk, &result->ltk, RBLE_KEY_LEN);
        app.ediv = result->ediv;
        break;

    default:
        EPRINT("not intended state: 0x%x", app.state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_sm_callback
 * Description  : Security manager callback
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_sm_callback(RBLE_SM_EVENT *event)
{
    switch (event->type) {
    case RBLE_SM_LTK_REQ_IND:
        app_sm_ltk_req_ind(event);
        break;

    case RBLE_SM_LTK_REQ_FOR_ENC_IND:
        app_sm_ltk_req_for_enc_ind(event);
        break;

    case RBLE_SM_KEY_IND:
        app_sm_key_ind(event);
        break;

    case RBLE_SM_ENC_START_IND:
        app_sm_enc_start_ind(event);
        break;

    default:
        EPRINT("unknown event: 0x%x", event->type);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_access_bda
 * Description  : get/set bluetooth device address from/to flash
 * Arguments    : cmd - RBLE_VS_FLASH_CMD_READ or RBLE_VS_FLASH_CMD_WRITE
 *                id - APP_REMOTE_BDA or APP_SELF_BDA
 * Return Value : None
*******************************************************************************/
static void app_access_bda(uint8_t cmd, uint8_t id)
{
    RBLE_VS_FLASH_ACCESS_PARAM param;

    if ((APP_SELF_BDA != id) && (APP_REMOTE_BDA != id)) {
        return;
    }

    param.cmd = cmd;
    param.id = id;
    param.size = RBLE_BD_ADDR_LEN;
    param.addr = (id == APP_SELF_BDA) ? (uint8_t *)&app.self_bda : (uint8_t *)&app.remote_bda;

    CHECK_OK(RBLE_VS_Flash_Access(&param));
}

/*******************************************************************************
 * Function Name: app_start_flash_access
 * Description  : steps after flash access is started
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_start_flash_access(void)
{
    uint8_t cmd;
    uint8_t id;

    switch (app.addr_state) {
    case APP_ADDR_INITIAL_SELF:
    case APP_ADDR_GET_SELF:
        cmd = RBLE_VS_FLASH_CMD_READ;
        id = APP_SELF_BDA;
        break;

    case APP_ADDR_INITIAL_REMOTE:
    case APP_ADDR_GET_REMOTE:
        cmd = RBLE_VS_FLASH_CMD_READ;
        id = APP_REMOTE_BDA;
        break;

    case APP_ADDR_SET_SELF:
        cmd = RBLE_VS_FLASH_CMD_WRITE;
        id = APP_SELF_BDA;
        break;

    case APP_ADDR_SET_REMOTE:
        cmd = RBLE_VS_FLASH_CMD_WRITE;
        id = APP_REMOTE_BDA;
        break;

    default:
        EPRINT("unknown addr state: 0x%x", app.addr_state);
        break;
    }

    app_access_bda(cmd, id);
}


/*******************************************************************************
 * Function Name: app_stop_flash_access
 * Description  : steps after flash access is stopped
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void app_stop_flash_access(void)
{
    switch (app.addr_state) {
    case APP_ADDR_INITIAL_REMOTE:
        /* start up */
        app_gap_set_bonding_mode();
        app.addr_state = APP_ADDR_NONE;
        break;

    case APP_ADDR_GET_SELF:
        /* AT-AS? */
        at_print_bda_response("-AS", &app.self_bda);
        at_print_response("OK");
        app.addr_state = APP_ADDR_NONE;
        break;

    case APP_ADDR_GET_REMOTE:
        /* AT-AP? */
        at_print_bda_response("-AP", &app.remote_bda);
        at_print_response("OK");
        app.addr_state = APP_ADDR_NONE;
        break;

    case APP_ADDR_SET_SELF:
    case APP_ADDR_SET_REMOTE:
        /* AT-AS=<addr> or AT-AP=<addr> */
        switch (app.state) {
        case APP_ADVERTISER:
            app_gap_broadcast_disable();
            break;
        case APP_SCANNER:
            /* do nothing */
            break;
        case APP_INITIATER:
            app_gap_connection_cancel();
            break;
        default:
            EPRINT("not intended state: 0x%x", app.state);
            break;
        }
        app.addr_state = APP_ADDR_NONE;
        break;

    default:
        EPRINT("unknown addr state: 0x%x", app.addr_state);
        break;
    }
}

/*******************************************************************************
 * Function Name: app_vs_flash_management_comp
 * Description  : RBLE_VS_EVENT_FLASH_MANAGEMENT_COMP event handler
 * Arguments    : event - VS event struct
 * Return Value : None
*******************************************************************************/
static void app_vs_flash_management_comp(RBLE_VS_EVENT *event)
{
    if (RBLE_OK == event->param.management_comp.status) {
        if (RBLE_VS_FLASH_CMD_START == event->param.management_comp.cmd) {
            app_start_flash_access();
        } else { /* if (RBLE_VS_FLASH_CMD_STOP == event->param.management_comp.cmd) */
            app_stop_flash_access();
        }
    }
}

/*******************************************************************************
 * Function Name: app_vs_flash_access_comp
 * Description  : RBLE_VS_EVENT_FLASH_ACCESS_COMP event handler
 * Arguments    : event - VS event struct
 * Return Value : None
*******************************************************************************/
static void app_vs_flash_access_comp(RBLE_VS_EVENT *event)
{
    if (RBLE_OK == event->param.access_comp.status) {
        if (APP_ADDR_INITIAL_SELF == app.addr_state) {
            app.addr_state = APP_ADDR_INITIAL_REMOTE;
            app_access_bda(RBLE_VS_FLASH_CMD_READ, APP_REMOTE_BDA);
        } else {
            CHECK_OK(RBLE_VS_Flash_Management(RBLE_VS_FLASH_CMD_STOP));
        }
    }
    else if (event->param.access_comp.status == DF_ERR_POOL_FULL) {
        CHECK_OK(RBLE_VS_Flash_Operation(RBLE_VS_FLASH_CMD_CLEANUP));
    }
    else {
        CHECK_OK(RBLE_VS_Flash_Management(RBLE_VS_FLASH_CMD_STOP));
    }
}

/*******************************************************************************
 * Function Name: app_vs_flash_operation_comp
 * Description  : RBLE_VS_EVENT_FLASH_OPERATION_COMP event handler
 * Arguments    : event - VS event struct
 * Return Value : None
*******************************************************************************/
static void app_vs_flash_operation_comp(RBLE_VS_EVENT *event)
{
    if ((RBLE_OK == event->param.operation_comp.status) &&
        (RBLE_VS_FLASH_CMD_CLEANUP == event->param.operation_comp.cmd)) {
        app_start_flash_access();
    }
    else {
        CHECK_OK(RBLE_VS_Flash_Management(RBLE_VS_FLASH_CMD_STOP));
    }
}

/*******************************************************************************
 * Function Name: app_vs_callback
 * Description  : Vendor Specific callback
 * Arguments    : event - VS event struct
 * Return Value : None
*******************************************************************************/
static void app_vs_callback(RBLE_VS_EVENT *event)
{
    switch (event->type) {
    case RBLE_VS_EVENT_FLASH_MANAGEMENT_COMP:
        app_vs_flash_management_comp(event);
        break;

    case RBLE_VS_EVENT_FLASH_ACCESS_COMP:
        app_vs_flash_access_comp(event);
        break;

    case RBLE_VS_EVENT_FLASH_OPERATION_COMP:
        app_vs_flash_operation_comp(event);
        break;

    default:
        EPRINT("unknown event: 0x%x", event->type);
        break;
    }
}

/*******************************************************************************
 * Function Name: rble_core_callback
 * Description  : Application state change callback
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void rble_core_callback(RBLE_MODE mode)
{
    switch (mode) {
    case RBLE_MODE_ACTIVE:
        app_change_state(APP_ADVERTISER);
        app_gap_reset();
        break;

    case RBLE_MODE_INITIALIZE:
    case RBLE_MODE_RESET:
    case RBLE_MODE_ERROR:
        /* do nothing */
        break;

    default:
        EPRINT("unknown mode: 0x%d", mode);
        break;
    }
}

/*******************************************************************************
 * Function Name: RBLE_App_Init
 * Description  : Application initialization
 * Arguments    : None
 * Return Value : TRUE  - Success
 *                FALSE - Fail
*******************************************************************************/
BOOL RBLE_App_Init(void)
{
    BOOL ret = TRUE;
    RBLE_STATUS status = RBLE_OK;

    ret = Console_Init();
    if (TRUE == ret) {
        status = RBLE_Init(&rble_core_callback);
        if (RBLE_OK != status) {
            ret = FALSE;
        }
    }

    return ret;
}
