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
 * File Name  : console.h
 * Version    : 1.0
 * Description: console task header file
 *
 * Copyright(C) 2012-2016 Renesas Electronics Corporation
 ***************************************************************************`**/

#ifndef __CONSOLE_H__
#define __CONSOLE_H__

/*******************************************************************************
 * Include Files
 ******************************************************************************/
#include "arch.h"
#include "rble.h"
#include "rwke_api.h"

/*******************************************************************************
 * Macro Definitions
 ******************************************************************************/
#define	BS_KEY_CODE  (0x08)
#define	CR_KEY_CODE  (0x0D)
#define	LF_KEY_CODE  (0x0A)
#define	ESC_KEY_CODE (0x1B)

#define CONSOLE_MSG_CHAR_IN (KE_FIRST_MSG(TASK_CON_APPL) + 1)
#define CONSOLE_MSG_LINE_IN (KE_FIRST_MSG(TASK_CON_APPL) + 2)

#define CONSOLE_IDX_MAX (1)

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/
typedef enum {
    CONSOLE_STATE_IN_ENABLE,
    CONSOLE_STATE_IN_DISABLE,
    CONSOLE_STATE_MAX,
} CONSOLE_STATE;

typedef struct {
    uint8_t len;
    char_t  buf[1]; /* run over */
} CONSOLE_MSG;

/*******************************************************************************
 * Variable Declarations
 ******************************************************************************/
extern ke_state_t console_state[];
extern const struct ke_state_handler console_state_handler[];
extern const struct ke_state_handler console_default_handler;

/*******************************************************************************
 * Function Declarations (API)
 ******************************************************************************/
void console_init(bool enable_in, ke_task_id_t task_id);
bool console_can_sleep(void);
void console_enable_in(void);
void console_disable_in(void);
void Printf(const char_t *fmt, ...);

#endif /* __CONSOLE_H__ */
