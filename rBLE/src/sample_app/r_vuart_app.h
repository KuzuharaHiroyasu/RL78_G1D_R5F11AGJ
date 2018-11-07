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
 * File Name  : r_vuart_app.h
 * Version    : 1.0
 * Description: Application Code File
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 ******************************************************************************/

#ifndef _APP_H
#define _APP_H

/*******************************************************************************
    Includes
********************************************************************************/
#ifndef WIN32
#include "arch.h"
#include "rwke_api.h"
#endif
#include "rble_api.h"

/*******************************************************************************
    Macro Definitions
********************************************************************************/
#define CR_KEY_CODE    (0x0d)
#define LF_KEY_CODE    (0x0a)
#define ESC_KEY_CODE   (0x1b)
#define BS_KEY_CODE    (0x08)

#define RBLE_APP_STATE_MAX	(1)
#define RBLE_APP_IDX_MAX	(1)
#define RBLE_APP_CON_EVT	(KE_FIRST_MSG(TASK_CON_APPL) + 1)
#define RBLE_APP_SLEEP_EVT	(KE_FIRST_MSG(TASK_CON_APPL) + 2)
#define RBLE_APP_ADV_EVT	(KE_FIRST_MSG(TASK_CON_APPL) + 3)

#ifdef APP_DEBUG
#define DPRINT(fmt, ...) Printf(fmt, __VA_ARGS__)
#define EPRINT(fmt, ...) Printf("[error:%s:%d] "fmt, __MODULE__, __LINE__, __VA_ARGS__)
#define CHECK_OK(status)                                                \
    do {                                                                \
        if (RBLE_OK != status) {                                        \
            Printf("[CHECK] status:%d, file:%s, line:%d\n",             \
                   status, __MODULE__, __LINE__);                       \
        }                                                               \
    } while (0)
#else
#ifdef _USE_REL_RL78
#define DPRINT printf
#define EPRINT printf
#define CHECK_OK(status) (status)
#else
#define DPRINT(...)
#define EPRINT(...)
#define CHECK_OK(status) (status)
#endif
#endif

/*******************************************************************************
    Typedef Definitions
********************************************************************************/
typedef struct {
    uint16_t            disc_mode;
    uint16_t            conn_mode;
    RBLE_ADV_INFO       adv_info;
} APP_BROADCAST_ENABLE_PARAM;

/*******************************************************************************
    Global Variables
********************************************************************************/
#ifndef WIN32
extern const struct ke_state_handler rble_app_state_handler[RBLE_APP_STATE_MAX];
extern const struct ke_state_handler rble_app_default_handler;
extern ke_state_t rble_app_state[RBLE_APP_STATE_MAX];
#endif

/*******************************************************************************
    Exported Functions
********************************************************************************/
extern BOOL RBLE_App_Init(void);
extern void R_APP_VUART_Send_Char(const char *chars, uint16_t len);
extern void R_AT_Parse(uint8_t *str, uint16_t len);
extern uint8_t get_ble_connect(void);

#ifdef WIN32
#define EEL_ID_REMOTE_BDA (0x2)
#define EEL_ID_BDA        (0x1)
#endif

#ifdef WIN32
#define Printf printf
#endif

#endif /* _APP_H */
