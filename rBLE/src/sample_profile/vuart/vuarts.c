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
 * File Name  : vuarts.c
 * Version    : 1.0
 * Description: Prifle Code File
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 ******************************************************************************/

#ifdef USE_VUART_PROFILE
/*******************************************************************************
    Includes
********************************************************************************/
#include "vuarts.h"

/*******************************************************************************
    Macro Definitions
********************************************************************************/
typedef enum {
    VUART_SERVER_INDICATION_CFG_CHANGED,
    VUART_SERVER_WRITE_CHAR_UPDATED,
} vuart_server_set_data_state;

/*******************************************************************************
    Typedef Definitions
********************************************************************************/
typedef struct VUART_Server_Info {
    uint16_t conhdl;
    RBLE_VUART_EVENT_HANDLER callback;
    BOOL is_sendable;
    char send_buf[VUART_SEND_BUFFER_SIZE];
    int send_len;
    BOOL initialized;
    vuart_server_set_data_state set_data_state;
    uint16_t indication_config;
} vuart_server_info_t;

/*******************************************************************************
    Global Variables
********************************************************************************/
static vuart_server_info_t info;

/*******************************************************************************
    Function Declarations
********************************************************************************/
static void vuart_server_gatt_callback(RBLE_GATT_EVENT *event);

/*******************************************************************************
    Function Definitions
********************************************************************************/

/*******************************************************************************
 * Function Name: RBLE_VUART_Server_Enable
 * Description  : Enable VUART Server
 * Arguments    : conhdl - Connection handle
 *                callback - VUART Server callback
 * Return Value : RBLE_OK - Success
 *                Other - Fail
*******************************************************************************/
RBLE_STATUS RBLE_VUART_Server_Enable(uint16_t conhdl, RBLE_VUART_EVENT_HANDLER callback)
{
    RBLE_STATUS status;
    RBLE_GATT_SET_DATA set_data;

    status = RBLE_GATT_Enable(vuart_server_gatt_callback);
    if (RBLE_OK == status) {
        info.conhdl = conhdl;
        info.callback = callback;
        info.is_sendable = TRUE;
        info.send_len = 0;
        info.initialized = FALSE;
        info.indication_config = 0;

        set_data.val_hdl = VUART_HDL_INDICATION_CFG;
        set_data.val_len = 2;
        set_data.value[0] = 0x0;
        set_data.value[1] = 0x0;

        info.set_data_state = VUART_SERVER_INDICATION_CFG_CHANGED;
        RBLE_GATT_Set_Data(&set_data);
    }

    return status;
}

/*******************************************************************************
 * Function Name: RBLE_VUART_Server_Diable
 * Description  : Diable VUART Server
 * Arguments    : None
 * Return Value : RBLE_OK - Success
 *                Other - Fail
*******************************************************************************/
RBLE_STATUS RBLE_VUART_Server_Disable(void)
{
    RBLE_STATUS status = RBLE_OK;

    memset(&info, 0, sizeof(info));

    return status;
}

/*******************************************************************************
 * Function Name: RBLE_VUART_Server_Send_Indication
 * Description  : Send indication to VUART client
 * Arguments    : chars - pointer to string
 *                len - characters length
 * Return Value : RBLE_OK - Success
 *                Other - Fail
*******************************************************************************/
RBLE_STATUS RBLE_VUART_Server_Send_Indication(const char *chars, uint16_t len)
{
    RBLE_STATUS status = RBLE_OK;
    RBLE_GATT_SET_DATA set_data;

    if ((TRUE == info.initialized) && (info.indication_config & 0x02)) {
        if ((info.send_len + len) <= VUART_SEND_BUFFER_SIZE) {
            if (chars && (0 != len)) {
                memcpy(&info.send_buf[info.send_len], chars, len);
                info.send_len += len;
            }

            if (info.is_sendable) {
                info.is_sendable = FALSE;

                set_data.val_hdl = VUART_HDL_INDICATION_VAL;
                set_data.val_len = info.send_len;
                memcpy(set_data.value, info.send_buf, info.send_len);

                info.send_len = 0;

                info.set_data_state = VUART_SERVER_WRITE_CHAR_UPDATED;
                status = RBLE_GATT_Set_Data(&set_data);
            }
        } else {
            status = RBLE_GATT_BUFF_OVER_ERR;
        }
    } else {
        status = RBLE_GATT_INDICATE_NOT_ALLOWED;
    }

    return status;
}

/*******************************************************************************
 * Function Name: vuart_server_handle_value_cfm
 * Description  : RBLE_GATT_HANDLE_VALUE_CFM event handler
 * Arguments    : event - GATT event handler
 * Return Value : None
*******************************************************************************/
static void vuart_server_handle_value_cfm(RBLE_GATT_EVENT *event)
{
    RBLE_VUART_EVENT vuart_evt;

    vuart_evt.type = RBLE_VUART_EVENT_SERVER_INDICATION_CFM;
    vuart_evt.param.server_write_req.status = RBLE_OK;
    info.callback(&vuart_evt);

    info.is_sendable = TRUE;

    if (info.send_len != 0) {
        RBLE_VUART_Server_Send_Indication(NULL, 0);
    }
}

/*******************************************************************************
 * Function Name: vuart_server_write_cmd_ind
 * Description  : RBLE_GATT_WRITE_CMD_IND event handler
 * Arguments    : event - GATT event handler
 * Return Value : None
*******************************************************************************/
static void vuart_server_write_cmd_ind(RBLE_GATT_EVENT *event)
{
    RBLE_GATT_WRITE_RESP wr_resp;
    RBLE_GATT_SET_DATA set_data;
    RBLE_VUART_EVENT vuart_evt;

    struct RBLE_GATT_Write_Cmd_Ind_t *result =
        (struct RBLE_GATT_Write_Cmd_Ind_t *)&event->param.write_cmd_ind;

    wr_resp.conhdl = info.conhdl;
    wr_resp.att_hdl = result->elmt;

    switch (result->elmt) {
    case VUART_HDL_WRITE_VAL:
        wr_resp.att_code = RBLE_ATT_ERR_NO_ERROR;
        RBLE_GATT_Write_Response(&wr_resp);
        vuart_evt.type = RBLE_VUART_EVENT_SERVER_WRITE_REQ;
        vuart_evt.param.server_write_req.status = RBLE_OK;
        vuart_evt.param.server_write_req.len = result->size;
        memcpy(vuart_evt.param.server_write_req.value, result->value, result->size);
        info.callback(&vuart_evt);
        break;

    case VUART_HDL_INDICATION_CFG:
        /* notification is not supported */
        if (0 != (result->value[0] & 0x1)) {
            wr_resp.att_code = RBLE_ATT_ERR_IMPROPERLY_CONFIGURED;
            RBLE_GATT_Write_Response(&wr_resp);
        } else {
            wr_resp.att_code = RBLE_ATT_ERR_NO_ERROR;
            RBLE_GATT_Write_Response(&wr_resp);
            set_data.val_hdl = VUART_HDL_INDICATION_CFG;
            set_data.val_len = 2;
            set_data.value[0] = result->value[0];
            set_data.value[1] = result->value[1];
            info.indication_config = (set_data.value[1] << 8) | set_data.value[0];

            info.set_data_state = VUART_SERVER_INDICATION_CFG_CHANGED;
            RBLE_GATT_Set_Data(&set_data);
        }
        break;

    default:
        /* do nothing */
        break;
    }
}

/*******************************************************************************
 * Function Name: vuart_server_set_data_comp
 * Description  : RBLE_GATT_SET_DATA_CMP event handler
 * Arguments    : event - GATT event handler
 * Return Value : None
*******************************************************************************/
static void vuart_server_set_data_comp(RBLE_GATT_EVENT *event)
{
    RBLE_GATT_INDICATE_REQ ind;
    RBLE_VUART_EVENT vuart_evt;

    switch (info.set_data_state) {
    case VUART_SERVER_INDICATION_CFG_CHANGED:
        /* generate RBLE_VUART_EVENT_SERVER_ENABLE_CMP event when indication is
           enabled at the first time after the connection */
        if ((FALSE == info.initialized) && (info.indication_config & 0x02)) {
            vuart_evt.type = RBLE_VUART_EVENT_SERVER_ENABLE_CMP;
            vuart_evt.param.server_write_req.status = RBLE_OK;
            info.callback(&vuart_evt);
            info.initialized = TRUE;
        }
        break;

    case VUART_SERVER_WRITE_CHAR_UPDATED:
        /* info.initialized and info.indication_configa are checked
           in RBLE_VUART_Server_Send_Indication */
        ind.conhdl = info.conhdl;
        ind.charhdl = VUART_HDL_INDICATION_VAL;
        RBLE_GATT_Indicate_Request(&ind);
        break;

    default:
        /* do nothing */
        break;
    }
}

/*******************************************************************************
 * Function Name: vuart_server_gatt_callback
 * Description  : GATT callback
 * Arguments    : event - GATT Event struct
 * Return Value : None
*******************************************************************************/
static void vuart_server_gatt_callback(RBLE_GATT_EVENT *event)
{
    switch (event->type) {
    case RBLE_GATT_EVENT_HANDLE_VALUE_CFM:
        vuart_server_handle_value_cfm(event);
        break;

    case RBLE_GATT_EVENT_WRITE_CMD_IND:
        vuart_server_write_cmd_ind(event);
        break;

    case RBLE_GATT_EVENT_SET_DATA_CMP:
        vuart_server_set_data_comp(event);
        break;

    default:
        /* do nothing */
        break;
    }
}
#endif /* USE_VUART_PROFILE */
