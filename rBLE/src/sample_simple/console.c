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
 * File Name  : console.c
 * Version    : 1.0
 * Description: console task code file
 *
 * Copyright(C) 2012-2016 Renesas Electronics Corporation
 ***************************************************************************`**/

/*******************************************************************************
 * Include Files
 ******************************************************************************/
#include "console.h"
#include "serial.h"
#include "stdio.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define IN_BUF_MAX      (20)
#define PRINT_BUF_MAX   (200)

#define CONSOLE_TASK_ID (TASK_CON_APPL)

/*******************************************************************************
 * Function Declarations
 ******************************************************************************/
static int_t console_char_in(ke_msg_id_t const msgid, void const *param,
                             ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t console_char_in_invalid(ke_msg_id_t const msgid, void const *param,
                                     ke_task_id_t const dest_id, ke_task_id_t const src_id);

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
static uint8_t in_buf[IN_BUF_MAX];
static uint8_t in_buf_pos;
static char_t print_buf[PRINT_BUF_MAX];
static bool is_write_blocked;
static ke_task_id_t console_in_task_id;
ke_state_t console_state[CONSOLE_STATE_MAX];

struct ke_msg_handler console_in_enable_handler[] = {
    { CONSOLE_MSG_CHAR_IN, (ke_msg_func_t)console_char_in },
};

struct ke_msg_handler console_in_disable_handler[] = {
    { CONSOLE_MSG_CHAR_IN, (ke_msg_func_t)console_char_in_invalid },
};

const struct ke_state_handler console_state_handler[CONSOLE_STATE_MAX] = {
    KE_STATE_HANDLER(console_in_enable_handler),
    KE_STATE_HANDLER(console_in_disable_handler),
};

const struct ke_state_handler console_default_handler = KE_STATE_HANDLER_NONE;

/*******************************************************************************
 * Function Definitions (Internal)
 ******************************************************************************/
static void console_send_msg(void)
{
    CONSOLE_MSG *msg;
    uint16_t size;

    size = sizeof(CONSOLE_MSG) + in_buf_pos;

    msg = (CONSOLE_MSG *)ke_msg_alloc(CONSOLE_MSG_LINE_IN,
                                      console_in_task_id, CONSOLE_TASK_ID, size);
    msg->len = in_buf_pos;
    memcpy(msg->buf, in_buf, in_buf_pos);

    ke_msg_send(msg);

    in_buf_pos = 0;
}

static int_t console_char_in(ke_msg_id_t const msgid, void const *param,
                             ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    uint8_t key;

    key = in_buf[in_buf_pos];

    if ((key == CR_KEY_CODE) || (key == LF_KEY_CODE)) {
        Printf("\n", key);
        console_send_msg();
    }
    else if (key == ESC_KEY_CODE) {
        in_buf_pos++;
        console_send_msg();
    }
    else if (key == BS_KEY_CODE) {
        if (in_buf_pos != 0) {
            Printf("%c %c", BS_KEY_CODE, BS_KEY_CODE);
            in_buf_pos--;
        }
    }
    else {
        Printf("%c", key);
        in_buf_pos++;
    }

    if (in_buf_pos >= IN_BUF_MAX) {
        in_buf_pos = 0;
    }

    serial_read((uint8_t *)&in_buf[in_buf_pos], 1);

    return KE_MSG_CONSUMED;
}

static int_t console_char_in_invalid(ke_msg_id_t const msgid, void const *param,
                                     ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    uint8_t key;

    key = in_buf[in_buf_pos];

    /* ESC key can forcibly enable console input. */
    if (key == ESC_KEY_CODE) {
        ke_state_set(CONSOLE_TASK_ID, CONSOLE_STATE_IN_ENABLE);
        console_send_msg();
    }

    serial_read((uint8_t *)&in_buf[in_buf_pos], 1);

    return KE_MSG_CONSUMED;
}

static void console_rx_done(void)
{
    ke_msg_send_basic(CONSOLE_MSG_CHAR_IN, CONSOLE_TASK_ID, CONSOLE_TASK_ID);
}

static void console_tx_done(void)
{
    is_write_blocked = false;
}

static void console_rx_error(void)
{
    serial_read((uint8_t *)&in_buf[in_buf_pos], 1);
}

/*******************************************************************************
 * Function Definitions (API)
 ******************************************************************************/

/*******************************************************************************
 * Function Name: Printf
 * Description  : Same as printf
 * Arguments    : Same as printf
 * Return Value : None
 *******************************************************************************/
void Printf(const char_t *fmt, ...)
{
    va_list args;

#if !defined(_USE_IAR_RL78)
    args = NULL;
#endif

    while (is_write_blocked) { }

#if defined(_USE_IAR_RL78)
    va_start(args, fmt);
#else
    va_starttop(args, fmt);
#endif

    vsprintf((char *)print_buf, fmt, args);
    va_end(args);

    is_write_blocked = true;

    serial_write((uint8_t *)print_buf, strlen(print_buf));
}

/*******************************************************************************
 * Function Name: console_can_sleep
 * Description  : Return CPU can sleep or not
 * Arguments    : None
 * Return Value : true  - CPU can sleep
                  false - CPU cannot sleep
*******************************************************************************/
bool console_can_sleep(void)
{
    return !is_write_blocked;
}

/*******************************************************************************
 * Function Name: console_init
 * Description  : Initialize console
 * Arguments    : enable_in - Enable console input or not
 *                task_id   - The task ID notified the input event
 * Return Value : None
*******************************************************************************/
void console_init(bool enable_in, ke_task_id_t task_id)
{
    SERIAL_EVENT_PARAM param;

    param.tx_callback     = &console_tx_done;
    param.err_callback    = &console_rx_error;
	param.rx_cmp_callback = NULL;

    if (enable_in) {
        param.rx_callback = &console_rx_done;
        in_buf_pos            = 0;
        console_in_task_id    = task_id;
    } else {
        param.rx_callback = NULL;
    }

    is_write_blocked = false;

    serial_init(&param);

    if (enable_in) {
        serial_read((uint8_t *)&in_buf[0], 1);
        ke_state_set(CONSOLE_TASK_ID, CONSOLE_STATE_IN_ENABLE);
    } else {
        ke_state_set(CONSOLE_TASK_ID, CONSOLE_STATE_IN_DISABLE);
    }
}

/*******************************************************************************
 * Function Name: console_enable_in
 * Description  : Enable console input
 * Arguments    : None
 * Return Value : None
 *******************************************************************************/
void console_enable_in(void)
{
    ke_state_set(CONSOLE_TASK_ID, CONSOLE_STATE_IN_ENABLE);
    serial_read((uint8_t *)&in_buf[in_buf_pos], 1);
}

/*******************************************************************************
 * Function Name: console_disable_in
 * Description  : Disable console input
 * Arguments    : None
 * Return Value : None
 *******************************************************************************/
void console_disable_in(void)
{
    in_buf_pos = 0;
    ke_state_set(CONSOLE_TASK_ID, CONSOLE_STATE_IN_DISABLE);
}
