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
 * File Name  : vuart.h
 * Version    : 1.0
 * Description: Prifle Code File
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 ******************************************************************************/

#ifndef _VUART_H
#define _VUART_H

/*******************************************************************************
    Includes
********************************************************************************/
#ifndef WIN32
#include "arch.h"
#include "rwke_api.h"
#endif
#include <stdio.h>
#include "rble.h"
#include "rble_api_custom.h"
#include "rble_api.h"
#include "db_handle.h"

/*******************************************************************************
    Macro Definitions
********************************************************************************/
#define RBLE_SVC_VUART              {0x1B,0xC5, 0xD5,0xA5,0x02,0x00,0xB8,0x8C,0xE5,0x11,0x1B,0xA2,0x01,0x00,0x8C,0xD6}
#define RBLE_CHAR_VUART_INDICATION  {0x1B,0xC5, 0xD5,0xA5,0x02,0x00,0xB8,0x8C,0xE5,0x11,0x1B,0xA2,0x02,0x00,0x8C,0xD6}
#define RBLE_CHAR_VUART_WRITE       {0x1B,0xC5, 0xD5,0xA5,0x02,0x00,0xB8,0x8C,0xE5,0x11,0x1B,0xA2,0x03,0x00,0x8C,0xD6}

#define VUART_SEND_BUFFER_SIZE (20)

/*******************************************************************************
    Typedef Definitions
********************************************************************************/
typedef enum {
    RBLE_VUART_EVENT_SERVER_ENABLE_CMP = 0x01,
    RBLE_VUART_EVENT_SERVER_WRITE_REQ,
    RBLE_VUART_EVENT_SERVER_INDICATION_CFM,
    RBLE_VUART_EVENT_CLIENT_ENABLE_CMP = 0x81,
    RBLE_VUART_EVENT_CLIENT_INDICATION,
    RBLE_VUART_EVENT_CLIENT_WRITE_RSP,
} RBLE_VUART_EVENT_TYPE;

typedef struct RBLE_VUART_EVENT_t {
    RBLE_VUART_EVENT_TYPE type;
    union Event_Vuart_Paramter_u {
        struct {
            RBLE_STATUS status;
        } server_enable_cmp;
        struct {
            RBLE_STATUS status;
            char value[VUART_SEND_BUFFER_SIZE];
            uint16_t len;
        } server_write_req;
        struct {
            RBLE_STATUS status;
        } server_indicate_cnf;
        struct {
            RBLE_STATUS status;
        } client_enable_cmp;
        struct {
            RBLE_STATUS status;
            char value[VUART_SEND_BUFFER_SIZE];
            uint16_t len;
        } client_indication;
        struct {
            RBLE_STATUS status;
        } client_write_rsp;
    } param;

} RBLE_VUART_EVENT;

typedef void (*RBLE_VUART_EVENT_HANDLER)(RBLE_VUART_EVENT *event);

#endif /* _VUART_H */
