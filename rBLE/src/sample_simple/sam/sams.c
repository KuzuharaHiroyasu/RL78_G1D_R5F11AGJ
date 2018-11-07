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
 * File Name  : sams.c
 * Version    : 1.0
 * Description: sample custom profile server fole code file
 *
 * Copyright(C) 2012-2016 Renesas Electronics Corporation
 ******************************************************************************/

/*******************************************************************************
 * Include Files
 ******************************************************************************/
#include "sams.h"
#include "rble_api.h"
#include "db_handle.h"

/*******************************************************************************
 * Enumeration Definitions
 ******************************************************************************/
typedef enum {
    SAMS_STATE_IDLE,
    SAMS_STATE_INIT,
    SAMS_STATE_CONNECTED,
} SAMS_STATE;

typedef enum {
    SAMS_INIT_STATE_SWITCH_STATE_CCCD,
    SAMS_INIT_STATE_END,
} SAMS_INIT_STATE;

/*******************************************************************************
 * Global Variables
 ******************************************************************************/
static struct {
    uint16_t conhdl;
    SAMS_STATE state;
    SAMS_INIT_STATE init_state;
    SAMPLE_SERVER_PARAM param;
    SAMPLE_SERVER_EVENT_HANDLER callback;
    RBLE_STATUS status;
    /* for set_data & write_cmd handling */
    uint16_t hdl;
    uint16_t len;
    uint8_t value[20];
    BOOL write_resp;
} sams_info;

/*******************************************************************************
 * Function Declarations
 ******************************************************************************/
static void sams_send_event(SAMPLE_SERVER_EVENT_TYPE type);
static void sams_set_cccd(uint16_t cccd_hdl, uint16_t cccd_val);
static void sams_set_init_data(void);
static void sams_set_data(uint16_t hdl, uint16_t len, uint8_t *val);
static void sams_notify_request(void);
static void sams_write_resp(void);
static void sams_set_data_cmp_handler(RBLE_GATT_EVENT *event);
static void sams_write_cmd_ind_handler(RBLE_GATT_EVENT *event);
static void sams_gatt_callback(RBLE_GATT_EVENT *event);

/*******************************************************************************
 * Function Definitions (Internal)
 ******************************************************************************/
static void sams_send_event(SAMPLE_SERVER_EVENT_TYPE type)
{
    SAMPLE_SERVER_EVENT event;

    event.type   = type;
    event.conhdl = sams_info.conhdl;

    switch (type) {
    case SAMPLE_SERVER_EVENT_ENABLE_COMP:
        event.status = sams_info.status;
        break;

    case SAMPLE_SERVER_EVENT_DISABLE_COMP:
        event.param.disable_comp.param.switch_state_cccd = sams_info.param.switch_state_cccd;
        event.status = sams_info.status;
        break;

    case SAMPLE_SERVER_EVENT_CHG_LED_CONTROL_IND:
        event.param.change_led_control_ind.value = sams_info.value[0];
        event.status = sams_info.status;
        break;

    case SAMPLE_SERVER_EVENT_WRITE_CHAR_RESPONSE:
        event.param.write_char_resp.value = (sams_info.value[1] << 8) | sams_info.value[0];
        event.status = sams_info.status;
        break;

    default:
        Printf("unsupported event: 0x%x\n", event.type);
        break;
    }

    sams_info.callback(&event);
}

static void sams_set_cccd(uint16_t cccd_hdl, uint16_t cccd_val)
{
    sams_set_data(cccd_hdl, SAMPLE_CCCD_SIZE, (uint8_t *)&cccd_val);
}

static void sams_set_init_data(void)
{
    switch (sams_info.init_state) {
    case SAMS_INIT_STATE_SWITCH_STATE_CCCD:
        sams_set_cccd(SAMS_HDL_SWITCH_STATE_CCCD, sams_info.param.switch_state_cccd);
        sams_info.init_state++;
        break;

    case SAMS_INIT_STATE_END:
        sams_info.state  = SAMS_STATE_CONNECTED;
        sams_info.status = RBLE_OK;
        sams_send_event(SAMPLE_SERVER_EVENT_ENABLE_COMP);
        break;

    default:
        Printf("unknown SAMS init state: 0x%x\n", sams_info.init_state);
        break;
    }
}

/* ##### GATT Functions ##### */
static void sams_set_data(uint16_t hdl, uint16_t len, uint8_t *val)
{
    RBLE_GATT_SET_DATA data;

    sams_info.hdl = hdl;
    sams_info.len = len;
    memcpy(sams_info.value, val, len);

    data.val_hdl = hdl;
    data.val_len = len;
    memcpy(data.value, val, len);

    (void)RBLE_GATT_Set_Data(&data);
}

static void sams_notify_request(void)
{
    RBLE_GATT_NOTIFY_REQ ntf;

    ntf.conhdl  = sams_info.conhdl;
    ntf.charhdl = sams_info.hdl;

    (void)RBLE_GATT_Notify_Request(&ntf);
}

static void sams_write_resp(void)
{
    RBLE_GATT_WRITE_RESP resp;

    if (TRUE == sams_info.write_resp) {
        resp.conhdl   = sams_info.conhdl;
        resp.att_hdl  = sams_info.hdl;
        resp.att_code = sams_info.status;
        (void)RBLE_GATT_Write_Response(&resp);
    }
}

/* ##### GATT Event Handlers ##### */
static void sams_set_data_cmp_handler(RBLE_GATT_EVENT *event)
{
    struct RBLE_GATT_Set_Data_Complete_t *result =
        (struct RBLE_GATT_Set_Data_Complete_t *)&event->param.set_data_cmp;

    sams_info.status = result->status;

    if (SAMS_STATE_INIT == sams_info.state) {
        sams_set_init_data();
        return;
    }

    switch (sams_info.hdl) {
    case SAMS_HDL_LED_CONTROL_VAL:
        sams_write_resp();
        sams_send_event(SAMPLE_SERVER_EVENT_CHG_LED_CONTROL_IND);
        break;

    case SAMS_HDL_SWITCH_STATE_CCCD:
        sams_write_resp();
        sams_send_event(SAMPLE_SERVER_EVENT_WRITE_CHAR_RESPONSE);
        sams_info.param.switch_state_cccd = (sams_info.value[1] << 8) | sams_info.value[0];
        break;

    case SAMS_HDL_SWITCH_STATE_VAL:
        sams_notify_request();
        break;

    default:
        Printf("unsupported hdl: 0x%x\n", sams_info.hdl);
        break;
    }
}

static void sams_write_cmd_ind_handler(RBLE_GATT_EVENT *event)
{
    struct RBLE_GATT_Write_Cmd_Ind_t *result =
        (struct RBLE_GATT_Write_Cmd_Ind_t *)&event->param.write_cmd_ind;

    sams_info.hdl        = result->elmt;
    sams_info.len        = result->size;
    sams_info.write_resp = result->resp;

    memset(sams_info.value, 0, sams_info.len);

    switch (result->elmt) {
    case SAMS_HDL_SWITCH_STATE_CCCD:
        sams_info.value[0] = (uint8_t)(result->value[0] & (RBLE_PRF_START_NTF | RBLE_PRF_START_IND));
        sams_info.value[1] = result->value[1];
        break;

	case SAMS_HDL_LED_CONTROL_VAL:
        sams_info.value[0] = (uint8_t)(result->value[0] & SAMPLE_LED_CONTROL_MASK);
        break;

    default:
        Printf("unsupported hdl: 0x%x\n", result->elmt);
        break;
    }

    sams_set_data(sams_info.hdl, sams_info.len, sams_info.value);
}

static void sams_gatt_callback(RBLE_GATT_EVENT *event)
{
    switch (event->type) {
    case RBLE_GATT_EVENT_SET_DATA_CMP:
        sams_set_data_cmp_handler(event);
        break;

    case RBLE_GATT_EVENT_WRITE_CMD_IND:
        sams_write_cmd_ind_handler(event);
        break;

    default:
        Printf("unsupported event: 0x%x\n", event->type);
        break;
    }
}

/*******************************************************************************
 * Function Definitions (API)
 ******************************************************************************/
RBLE_STATUS SAMPLE_Server_Enable(uint16_t conhdl, uint8_t con_type, SAMPLE_SERVER_PARAM *param,
                                 SAMPLE_SERVER_EVENT_HANDLER callback)
{
    RBLE_STATUS status;

    if (!callback || !param) {
        return RBLE_PARAM_ERR;
    }

    if (SAMS_STATE_IDLE != sams_info.state) {
        return RBLE_STATUS_ERROR;
    }

    status = RBLE_GATT_Enable(&sams_gatt_callback);
    if (RBLE_OK != status) {
        return RBLE_STATUS_ERROR;
    }

    memset(&sams_info, 0, sizeof(sams_info));

    sams_info.conhdl     = conhdl;
    sams_info.callback   = callback;
    sams_info.state      = SAMS_STATE_INIT;
    sams_info.init_state = SAMS_INIT_STATE_SWITCH_STATE_CCCD;

    if (RBLE_PRF_CON_NORMAL == con_type) {
        sams_info.param.switch_state_cccd = param->switch_state_cccd;
    }

    sams_set_init_data();

    return RBLE_OK;
}

RBLE_STATUS SAMPLE_Server_Disable(uint16_t conhdl)
{
    if (sams_info.conhdl != conhdl) {
        return RBLE_STATUS_ERROR;
    }

    if (SAMS_STATE_CONNECTED != sams_info.state) {
        return RBLE_STATUS_ERROR;
    }

    sams_info.state = SAMS_STATE_IDLE;

    sams_info.status = RBLE_OK;
    sams_send_event(SAMPLE_SERVER_EVENT_DISABLE_COMP);

    return RBLE_OK;
}

RBLE_STATUS SAMPLE_Server_Send_Switch_State(uint16_t conhdl, uint8_t value)
{
    if (sams_info.conhdl != conhdl) {
        return RBLE_STATUS_ERROR;
    }

    if (SAMS_STATE_CONNECTED != sams_info.state) {
        return RBLE_STATUS_ERROR;
    }

    if ((sams_info.param.switch_state_cccd & RBLE_PRF_START_NTF) != RBLE_PRF_START_NTF) {
        return RBLE_STATUS_ERROR;
    }

    sams_set_data(SAMS_HDL_SWITCH_STATE_VAL, SAMPLE_SWITCH_STATE_SIZE, &value);

    return RBLE_OK;
}
