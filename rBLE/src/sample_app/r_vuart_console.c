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
 * File Name  : r_vuart_console.c
 * Version    : 1.0
 * Description: Application Code File
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 ******************************************************************************/

/*******************************************************************************
    Includes
********************************************************************************/
#include "arch.h"
#include "serial.h"
#include "r_vuart_console.h"
#include "r_vuart_app.h"

#include <stdio.h>
#include <stdarg.h>

/*******************************************************************************
    Macro Definitions
********************************************************************************/
#define STREAM_MEMORY_MAX_LINE_SIZE (80)
#define SLEEP_DURATION              (400)

#define		SERIAL_INVALID		// RD8001対応：シリアル機能無効(CPU間通信と競合)

/*******************************************************************************
    Global Variables
********************************************************************************/
#ifdef DISABLE_LOCAL_ECHO_BY_DEFAULT
static BOOL is_echo_enabled = FALSE;
#else
static BOOL is_echo_enabled = TRUE;
#endif
static volatile bool is_sleepable;
static volatile bool is_write_blocked;
static volatile bool is_read_blocked;
#ifndef	SERIAL_INVALID
static uint8_t stream_buffer[STREAM_MEMORY_MAX_LINE_SIZE];
#endif

#define KE_EVT_USR_0_BIT ((uint32_t)0x1U << (31 - KE_EVT_USR_0))
/* RDB_SIZE shall be 2^n */
#define RDB_SIZE (0x40)
uint8_t rdb[RDB_SIZE];

uint16_t rdb_rd_pos  = 0;
uint16_t rdb_wr_pos  = 0;

uint16_t cmd_pos = 0;
uint8_t cmd_buf[RDB_SIZE];

uint16_t print_pos = 0;
uint8_t print_buf[RDB_SIZE];

/*******************************************************************************
    Function Declarations
********************************************************************************/

#ifndef	SERIAL_INVALID
static void console_rx_done( void );
static void console_tx_done( void );
static void console_rx_error( void );
#endif

/*******************************************************************************
 * Function Name: Console_Init
 * Description  : Initialize serial device
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
BOOL Console_Init(void)
{
    BOOL ret = TRUE;
#ifndef	SERIAL_INVALID
    SERIAL_EVENT_PARAM callback = {0};

    callback.rx_callback = &console_rx_done;
    callback.tx_callback = &console_tx_done;
    callback.err_callback = &console_rx_error;
    callback.rx_cmp_callback = NULL;
    serial_init(&callback);
#endif

    is_sleepable = false;
    is_write_blocked = false;
    is_read_blocked = false;
#ifndef	SERIAL_INVALID
    serial_read((uint8_t *)&rdb, 1);
#endif

    return ret;
}

/*******************************************************************************
 * Function Name: Console_Set_Echo
 * Description  : Enable/Disable Console Echo
 * Arguments    : onoff - TRUE for Enable, FALSE for Disable
 * Return Value : None
*******************************************************************************/
void Console_Set_Echo(BOOL onoff)
{
    is_echo_enabled = onoff;
}

/*******************************************************************************
 * Function Name: console_in
 * Description  : Console input event handler
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
void codeptr r_console_in(void)
{
    static BOOL is_at_mode = TRUE;

    uint8_t key;
    uint8_t i;
    uint16_t size;

    /* deter entering sleep for a short time, in order to avoid dropping successive rx chars */
    is_read_blocked = true;
    ke_timer_set(RBLE_APP_SLEEP_EVT, TASK_CON_APPL, SLEEP_DURATION);

    /* postpone the processing of characters, it will be
       processed next event handle */
    if (is_write_blocked) {
        return;
    }

    GLOBAL_INT_DISABLE();
    /* calculate the size of characters processed in this loop */
    if (rdb_rd_pos <= rdb_wr_pos) {
        size = rdb_wr_pos - rdb_rd_pos;
    } else {
        size = RDB_SIZE - rdb_rd_pos + rdb_wr_pos;
    }

    /* clear event */
    ke_evt_clear(KE_EVT_USR_0_BIT);
    GLOBAL_INT_RESTORE();

    for (i = 0; i < size; i++) {
        /* go to next character */
        key = rdb[rdb_rd_pos++];
        rdb_rd_pos &= RDB_SIZE - 1;

        /* before switching the mode, flush cmd/print buffer */
        if (ESC_KEY_CODE == key) {
            break;
        }

        else {
            if (TRUE == is_at_mode) {
                if ((CR_KEY_CODE == key) || (LF_KEY_CODE == key)) {
                    /* flush print buffer */
                    if (TRUE == is_echo_enabled) {
                        Printf("%.*s\n", print_pos, print_buf);
                        print_pos = 0;
                    }

                    /* execute AT Command */
                    R_AT_Parse(cmd_buf, cmd_pos);
                    cmd_pos = 0;

                    continue;
                }
                else if (BS_KEY_CODE == key) {
                    cmd_pos = (cmd_pos == 0) ? 0 : cmd_pos - 1;
                    cmd_buf[cmd_pos] = ' ';
                }
                else {
                    /* push cmd fraction onto cmd buffer */
                    cmd_buf[cmd_pos++] = key;
                    cmd_pos &= RDB_SIZE - 1;
                }
            }

            if ((FALSE == is_at_mode) || (TRUE == is_echo_enabled)) {
                /* set proper char code for print */
                if ((CR_KEY_CODE == key) || (LF_KEY_CODE == key)) {
                    print_buf[print_pos++] = '\n';
                }
                else if (BS_KEY_CODE == key) {
                    if (RDB_SIZE > print_pos + 3) {
                        print_buf[print_pos++] = key;
                        print_buf[print_pos++] = ' ';
                        print_buf[print_pos++] = key;
                    }
                }
                else {
                    print_buf[print_pos++] = key;
                }

                print_pos &= RDB_SIZE - 1;
            }
        }
    }

    /* come here when 'finish processing whole rdb[]' or 'ESC key is in rdb[]' */

    if (is_at_mode) {
        /* print buffer */
        if (TRUE == is_echo_enabled) {
            if (print_pos != 0) {
                Printf("%.*s", print_pos, print_buf);
            }
            print_pos = 0;
        }
    }
    else {
        /* send characters to vuart */
        if (print_pos != 0) {
            R_APP_VUART_Send_Char((char *)print_buf, print_pos);
            print_pos = 0;
        }
    }

    /* switch mode */
    if (ESC_KEY_CODE == key) {
        is_at_mode = (BOOL)(is_at_mode == TRUE ? FALSE : TRUE);

        if (is_at_mode) {
            /* clear cmd buffer */
            cmd_pos = 0;
            Printf("%c%c[AT Command Mode]%c%c", CR_KEY_CODE, LF_KEY_CODE, CR_KEY_CODE, LF_KEY_CODE);
        } else {
            Printf("%c%c[Virtual UART Mode]%c%c", CR_KEY_CODE, LF_KEY_CODE, CR_KEY_CODE, LF_KEY_CODE);
        }
    }

    if (size > i) {
        ke_evt_set(KE_EVT_USR_0_BIT);
    }
}

#ifndef	SERIAL_INVALID
/*******************************************************************************
 * Function Name: console_rx_done
 * Description  : Received callback
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void console_rx_done(void)
{
    ke_evt_set(KE_EVT_USR_0_BIT);

    rdb_wr_pos++;
    rdb_wr_pos &= RDB_SIZE - 1;

    serial_read((uint8_t *)&rdb[rdb_wr_pos], 1);
}

/*******************************************************************************
 * Function Name: console_tx_done
 * Description  : Transmitted callback
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void console_tx_done(void)
{
    is_write_blocked = false;

    if (!is_read_blocked) {
        is_sleepable = true;
    }
}

/*******************************************************************************
 * Function Name: console_rx_error
 * Description  : Receive error callback
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
static void console_rx_error(void)
{
    serial_read((uint8_t *)&rdb[rdb_wr_pos], 1);
}
#endif

/*******************************************************************************
 * Function Name: console_can_sleep
 * Description  : Return CPU can sleep or not
 * Arguments    : None
 * Return Value : TRUE - CPU can sleep
                  FALSE - CPU cannot sleep
*******************************************************************************/
bool console_can_sleep(void)
{
#if 0
    return is_sleepable;
#else
	// RD8001暫定：スリープ出来ないタイミングは現状なし。プラットフォーム
    return true;
#endif
}

/*******************************************************************************
 * Function Name: Printf
 * Description  : Same as printf
 * Arguments    : Same as printf
 * Return Value : None
*******************************************************************************/
void Printf(char *fmt, ...)
{
//maeda FW update delete
# if 0

#ifdef  _USE_IAR_RL78
    va_list args;
#else
    va_list args = 0;
#endif

    while (is_write_blocked) {
        /* busy loop */
    }

    is_sleepable = false;

#ifdef  _USE_IAR_RL78
    va_start(args, fmt);
#else
    va_starttop(args, fmt);
#endif
    vsprintf((char *)stream_buffer, fmt, args);
    va_end(args);

    is_write_blocked = true;

    serial_write((uint8_t *)stream_buffer, strlen((char *)stream_buffer));
#endif
}

/*******************************************************************************
 * Function Name: RBLE_Set_Sleepable
 * Description  : Make CPU sleepable
 * Arguments    : msgid - Message ID
 *                param - parameter (NULL)
 *                dest_id - Destination ID
 *                src_id - Source Id
 * Return Value : Message handling result
*******************************************************************************/
int_t RBLE_Set_Sleepable(ke_msg_id_t const msgid, void const *param,
                         ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
    if (is_write_blocked) {
        ke_timer_set(RBLE_APP_SLEEP_EVT, TASK_CON_APPL, 10);
    } else {
        is_read_blocked = false;
        is_sleepable = true;
    }

    return KE_MSG_CONSUMED;
}
