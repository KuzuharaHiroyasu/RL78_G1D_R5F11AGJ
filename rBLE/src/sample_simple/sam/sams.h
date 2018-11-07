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
 * File Name  : sams.h
 * Version    : 1.0
 * Description: sample custom profile server role header file
 *
 * Copyright(C) 2012-2016 Renesas Electronics Corporation
 ******************************************************************************/
#ifndef __SAMS_H__
#define __SAMS_H__

/*******************************************************************************
 * Include Files
 ******************************************************************************/
#include "arch.h"
#include "rble.h"
#include "sam/sam.h"

/*******************************************************************************
 * Type Definitions
 ******************************************************************************/
typedef enum {
    SAMPLE_SERVER_EVENT_ENABLE_COMP = 0,
    SAMPLE_SERVER_EVENT_DISABLE_COMP,
    SAMPLE_SERVER_EVENT_CHG_LED_CONTROL_IND,
    SAMPLE_SERVER_EVENT_WRITE_CHAR_RESPONSE,
} SAMPLE_SERVER_EVENT_TYPE;

typedef struct {
    uint16_t switch_state_cccd;
} SAMPLE_SERVER_PARAM;

typedef struct {
    SAMPLE_SERVER_EVENT_TYPE type;
    RBLE_STATUS status;
    uint16_t conhdl;
    union {
        struct {
            SAMPLE_SERVER_PARAM param;
        } disable_comp;
        struct {
            uint8_t value;
        } change_led_control_ind;
        struct {
            uint16_t value;
        } write_char_resp;
    } param;
} SAMPLE_SERVER_EVENT;

typedef void (*SAMPLE_SERVER_EVENT_HANDLER)(SAMPLE_SERVER_EVENT *event);

/*******************************************************************************
 * Function Declarations (API)
 ******************************************************************************/
RBLE_STATUS SAMPLE_Server_Enable(uint16_t conhdl, uint8_t con_type,
                                 SAMPLE_SERVER_PARAM *param, SAMPLE_SERVER_EVENT_HANDLER callback);
RBLE_STATUS SAMPLE_Server_Disable(uint16_t conhdl);
RBLE_STATUS SAMPLE_Server_Send_Switch_State(uint16_t conhdl, uint8_t value);

#endif /* __SAMS_H__ */
