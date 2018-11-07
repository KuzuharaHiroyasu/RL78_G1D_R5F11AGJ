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
 * File Name  : vuartc.c
 * Version    : 1.0
 * Description: Prifle Code File
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 ******************************************************************************/

#ifdef USE_VUART_PROFILE
/*******************************************************************************
    Includes
********************************************************************************/
#include "vuartc.h"

/*******************************************************************************
    Macro Definitions
********************************************************************************/
#define VUART_FOUND_SVC                 (1 << 0)
#define VUART_FOUND_IND_CHAR            (1 << 1)
#define VUART_FOUND_WRITE_CHAR          (1 << 2)
#define VUART_FOUND_IND_CFG             (1 << 3)

#define VUART_FOUND_ALL_SVC             (VUART_FOUND_SVC)
#define VUART_FOUND_ALL_SVC_CHARS       (VUART_FOUND_ALL_SVC | VUART_FOUND_IND_CHAR | VUART_FOUND_WRITE_CHAR)
#define VUART_FOUND_ALL_SVC_CHARS_DESCS (VUART_FOUND_ALL_SVC_CHARS | VUART_FOUND_IND_CFG)

/*******************************************************************************
    Typedef Definitions
********************************************************************************/
typedef struct VUART_Client_Info {
    uint16_t conhdl;
    RBLE_VUART_EVENT_HANDLER callback;
    uint16_t start_hdl;
    uint16_t end_hdl;
    uint16_t indication_char_hdl;
    uint16_t indication_val_hdl;
    uint16_t indication_cfg_hdl;
    uint16_t write_char_hdl;
    uint16_t write_val_hdl;
    uint16_t found_svc_chars_descs;
    BOOL is_connected;
    BOOL is_sendable;
    char send_buf[VUART_SEND_BUFFER_SIZE];
    uint16_t send_len;
} vuart_client_info_t;

/*******************************************************************************
    Global Variables
********************************************************************************/
static vuart_client_info_t info;

static const uint8_t vuart_service_uuid[RBLE_GATT_128BIT_UUID_OCTET] = RBLE_SVC_VUART;
static const uint8_t vuart_indication_uuid[RBLE_GATT_128BIT_UUID_OCTET] = RBLE_CHAR_VUART_INDICATION;
static const uint8_t vuart_write_uuid[RBLE_GATT_128BIT_UUID_OCTET] = RBLE_CHAR_VUART_WRITE;

/*******************************************************************************
    Function Declarations
********************************************************************************/
static void vuart_client_gatt_callback(RBLE_GATT_EVENT *event);

/*******************************************************************************
    Function Definitions
********************************************************************************/

/*******************************************************************************
 * Function Name: RBLE_VUART_Client_Enable
 * Description  : Enable VUART Client
 * Arguments    : conhdl - Connection handle
 *                callback - VUART Client callback
 * Return Value : RBLE_OK - Success
 *                Other - Fail
*******************************************************************************/
RBLE_STATUS RBLE_VUART_Client_Enable(uint16_t conhdl, RBLE_VUART_EVENT_HANDLER callback)
{
    RBLE_STATUS status;

    status = RBLE_GATT_Enable(vuart_client_gatt_callback);
    if (RBLE_OK == status) {
        RBLE_GATT_DISC_SVC_REQ disc_svc;

        disc_svc.req_type = RBLE_GATT_DISC_BY_UUID_SVC;
        disc_svc.conhdl = conhdl;
        disc_svc.start_hdl = 0x0001u;
        disc_svc.end_hdl = 0xFFFFu;
        disc_svc.desired_svc.value_size = RBLE_GATT_128BIT_UUID_OCTET;
        memcpy(disc_svc.desired_svc.value, &vuart_service_uuid, sizeof(vuart_service_uuid));

        status = RBLE_GATT_Discovery_Service_Request(&disc_svc);
        if (RBLE_OK == status) {
            info.conhdl = conhdl;
            info.callback = callback;
            info.is_connected = FALSE;
            info.is_sendable = TRUE;
            info.send_len = 0;
        }
    }

    return status;
}

/*******************************************************************************
 * Function Name: RBLE_VUART_Client_Diable
 * Description  : Diable VUART Client
 * Arguments    : None
 * Return Value : None
*******************************************************************************/
RBLE_STATUS RBLE_VUART_Client_Disable(void)
{
    RBLE_STATUS status = RBLE_OK;

    memset(&info, 0, sizeof(info));

    return status;
}

/*******************************************************************************
 * Function Name: RBLE_VUART_Client_Send_Chars
 * Description  : Send characters
 * Arguments    : chars - pointer to string
 *                len - characters length
 * Return Value : RBLE_OK - Success
 *                Other - Fail
*******************************************************************************/
RBLE_STATUS RBLE_VUART_Client_Send_Chars(const char *chars, uint16_t len)
{
    RBLE_STATUS status = RBLE_OK;
    RBLE_GATT_WRITE_CHAR_REQ wr_char;

    if (info.is_connected) {
        if ((info.send_len + len) <= VUART_SEND_BUFFER_SIZE) {
            if (chars && (0 != len)) {
                memcpy(&info.send_buf[info.send_len], chars, len);
                info.send_len += len;
            }

            if (info.is_sendable) {
                info.is_sendable = FALSE;

                wr_char.conhdl = info.conhdl;
                wr_char.charhdl = info.write_val_hdl;
                wr_char.val_len = info.send_len;
                wr_char.req_type = RBLE_GATT_WRITE_CHAR;
                wr_char.auto_execute = FALSE;
                memcpy(wr_char.value, info.send_buf, info.send_len);

                info.send_len = 0;

                status = RBLE_GATT_Write_Char_Request(&wr_char);
            }
        } else {
            status = RBLE_GATT_BUFF_OVER_ERR;
        }
    } else {
        status = RBLE_STATUS_ERROR;
    }

    return status;
}

/*******************************************************************************
 * Function Name: vuart_client_disc_char_all_128_cmp
 * Description  : RBLE_GATT_DISC_CHAR_ALL_128_CMP event handler
 * Arguments    : event - GATT event struct
 * Return Value : None
*******************************************************************************/
static void vuart_client_disc_char_all_128_cmp(RBLE_GATT_EVENT *event)
{
    struct RBLE_GATT_Disc_Char_All_128_Comp_t *result =
        (struct RBLE_GATT_Disc_Char_All_128_Comp_t *)&event->param.disc_char_all_128_cmp;

    if (RBLE_OK == result->att_code) {
        if (0 == memcmp(result->list.uuid, vuart_indication_uuid, sizeof(vuart_indication_uuid))) {
            info.indication_char_hdl = result->list.attr_hdl;
            info.indication_val_hdl = result->list.pointer_hdl;
            info.found_svc_chars_descs |= VUART_FOUND_IND_CHAR;

        } else if (0 == memcmp(result->list.uuid, vuart_write_uuid, sizeof(vuart_write_uuid))) {
            info.write_char_hdl = result->list.attr_hdl;
            info.write_val_hdl = result->list.pointer_hdl;
            info.found_svc_chars_descs |= VUART_FOUND_WRITE_CHAR;

        } else {
            /* do nothing */
        }
    }
}

/*******************************************************************************
 * Function Name: vuart_client_disc_svc_by_uuid_cmp
 * Description  : RBLE_GATT_DISC_SVC_BY_UUID_CMP event handler
 * Arguments    : event - GATT event struct
 * Return Value : None
*******************************************************************************/
static void vuart_client_disc_svc_by_uuid_cmp(RBLE_GATT_EVENT *event)
{
    struct RBLE_GATT_Disc_Svc_By_Uuid_Comp_t *result =
        (struct RBLE_GATT_Disc_Svc_By_Uuid_Comp_t *)&event->param.disc_svc_by_uuid_cmp;

    if (RBLE_OK == result->att_code) {
        info.start_hdl = result->list[0].start_hdl;
        info.end_hdl = result->list[0].end_hdl;
        info.found_svc_chars_descs |= VUART_FOUND_SVC;
    }
}

/*******************************************************************************
 * Function Name: vuart_client_handle_value_ind
 * Description  : RBLE_GATT_HANDLE_VALUE_IND event handler
 * Arguments    : event - GATT event struct
 * Return Value : None
*******************************************************************************/
static void vuart_client_handle_value_ind(RBLE_GATT_EVENT *event)
{
    RBLE_VUART_EVENT vuart_evt;

    struct RBLE_GATT_Handle_Value_Ind_t *result =
        (struct RBLE_GATT_Handle_Value_Ind_t *)&event->param.handle_value_ind;

    vuart_evt.type = RBLE_VUART_EVENT_CLIENT_INDICATION;
    vuart_evt.param.client_indication.status = RBLE_OK;
    vuart_evt.param.client_indication.len = result->size;
    memcpy(vuart_evt.param.client_indication.value, result->value, result->size);
    info.callback(&vuart_evt);
}

/*******************************************************************************
 * Function Name: vuart_client_write_char_resp
 * Description  : RBLE_GATT_WRITE_CHAR_RESP event handler
 * Arguments    : event - GATT event struct
 * Return Value : None
*******************************************************************************/
static void vuart_client_write_char_resp(RBLE_GATT_EVENT *event)
{
    RBLE_VUART_EVENT uvart_event;

    struct RBLE_GATT_Write_Char_Resp_t *result =
        (struct RBLE_GATT_Write_Char_Resp_t *)&event->param.write_char_resp;

    if ((RBLE_OK == result->att_code)) {
        if (0 == info.found_svc_chars_descs) {
            uvart_event.type = RBLE_VUART_EVENT_CLIENT_WRITE_RSP;
            uvart_event.param.client_indication.status = RBLE_OK;
            info.callback(&uvart_event);

            info.is_sendable = TRUE;
            if (info.send_len != 0) {
                RBLE_VUART_Client_Send_Chars(NULL, 0);
            }
        } else {
            uvart_event.type = RBLE_VUART_EVENT_CLIENT_ENABLE_CMP;
            uvart_event.param.client_enable_cmp.status = RBLE_OK;
            info.callback(&uvart_event);
            info.found_svc_chars_descs = 0;
            info.is_connected = TRUE;
        }

    }
}

/*******************************************************************************
 * Function Name: vuart_client_complete
 * Description  : RBLE_GATT_COMPLETE event handler
 * Arguments    : event - GATT event handler
 * Return Value : None
*******************************************************************************/
static void vuart_client_complete(RBLE_GATT_EVENT *event)
{
    if (VUART_FOUND_ALL_SVC == info.found_svc_chars_descs) {
        RBLE_GATT_DISC_CHAR_REQ disc_char;
        disc_char.req_type = RBLE_GATT_DISC_ALL_CHAR;
        disc_char.conhdl = info.conhdl;
        disc_char.start_hdl = info.start_hdl;
        disc_char.end_hdl = info.end_hdl;
        info.found_svc_chars_descs |= VUART_FOUND_SVC;
        RBLE_GATT_Discovery_Char_Request(&disc_char);

    } else if (VUART_FOUND_ALL_SVC_CHARS == info.found_svc_chars_descs) {
        RBLE_GATT_DISC_CHAR_DESC_REQ disc_char_desc;
        disc_char_desc.conhdl = info.conhdl;
        disc_char_desc.start_hdl = info.start_hdl;
        disc_char_desc.end_hdl = info.end_hdl;
        RBLE_GATT_Discovery_Char_Descriptor_Request(&disc_char_desc);

    } else {
        RBLE_GATT_WRITE_CHAR_REQ wr_char;
        wr_char.conhdl = info.conhdl;
        wr_char.charhdl = info.indication_cfg_hdl;
        wr_char.req_type = RBLE_GATT_WRITE_DESC;
        wr_char.auto_execute = FALSE;
        wr_char.val_len = 2;
        wr_char.value[0] = 0x02;
        wr_char.value[1] = 0x00;
        RBLE_GATT_Write_Char_Request(&wr_char);
    }
}

/*******************************************************************************
 * Function Name: vuart_disc_char_desc_cmp
 * Description  : RBLE_GATT_DISC_CHAR_DESC_CMP event handler
 * Arguments    : event - GATT event handler
 * Return Value : None
*******************************************************************************/
static void vuart_disc_char_desc_cmp(RBLE_GATT_EVENT *event)
{
    uint8_t i;

    struct RBLE_GATT_Disc_Char_Desc_Comp_t *result =
        (struct RBLE_GATT_Disc_Char_Desc_Comp_t *)&event->param.disc_char_desc_cmp;

    for (i = 0; i < result->nb_entry; i++) {
        info.found_svc_chars_descs |= VUART_FOUND_IND_CFG;
        info.indication_cfg_hdl = result->list[0].attr_hdl;
    }
}

/*******************************************************************************
 * Function Name: vuart_client_gatt_callback
 * Description  : GATT callback
 * Arguments    : event - GATT Event struct
 * Return Value : None
*******************************************************************************/
static void vuart_client_gatt_callback(RBLE_GATT_EVENT *event)
{
    switch (event->type) {
    case RBLE_GATT_EVENT_DISC_CHAR_ALL_128_CMP:
        vuart_client_disc_char_all_128_cmp(event);
        break;

    case RBLE_GATT_EVENT_DISC_SVC_BY_UUID_CMP:
        vuart_client_disc_svc_by_uuid_cmp(event);
        break;

    case RBLE_GATT_EVENT_DISC_CHAR_DESC_CMP:
        vuart_disc_char_desc_cmp(event);
        break;

    case RBLE_GATT_EVENT_WRITE_CHAR_RESP:
        vuart_client_write_char_resp(event);
        break;

    case RBLE_GATT_EVENT_HANDLE_VALUE_IND:
        vuart_client_handle_value_ind(event);
        break;

    case RBLE_GATT_EVENT_COMPLETE:
        vuart_client_complete(event);
        break;

    default:
        /* do nothing */
        break;
    }
}
#endif /* USE_VUART_PROFILE */
