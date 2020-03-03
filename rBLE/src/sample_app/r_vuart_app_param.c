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
 * File Name  : r_vuart_app_param.c
 * Version    : 1.0
 * Description: Application Code File
 *
 * Copyright(C) 2013-2016 Renesas Electronics Corporation
 ******************************************************************************/

#include "r_vuart_app.h"

const RBLE_CREATE_CONNECT_PARAM g_app_default_connection_param[] = {
    {
        0x30,                                   /* scan_intv: 0x30 * 0.625[ms] = 30[ms] */
        0x30,                                   /* scan_window: 0x30 * 0.625[ms] = 30[ms] */
        RBLE_GAP_INIT_FILT_IGNORE_WLST,         /* init_filt_policy */
        RBLE_ADDR_PUBLIC,                       /* peer_addr_type */
        {0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12},	/* peer_addr */
        RBLE_ADDR_PUBLIC,                       /* own_addr_type */
        0,                                      /* reserved */
        0x06,                                   /* con_intv_min: 0x06 * 1.25[ms] = 7.5[ms] */
        0x06,                                   /* con_intv_max: 0x06 * 1.25[ms] = 7.5[ms] */
        0x00,                                   /* con_latency */
        0x1F4,                                  /* superv_to: 0x1F4 * 10[ms] = 5[s] */
        0x00,                                   /* ce_len_min: 0x0 * 0.625[ms] = 0[ms] */
        0x50                                    /* ce_len_max: 0x50 * 0.625[ms] = 50[ms] */
    }
};

const APP_BROADCAST_ENABLE_PARAM g_app_default_broadcast_param[] = {
    {
        RBLE_GAP_GEN_DISCOVERABLE,      /* disc_mode */
        RBLE_GAP_UND_CONNECTABLE,       /* conn_mode */
        {
            {
                0x5A0,                                  /* adv_intv_min: 0x5A0 * 0.625[ms] =  900[ms] */
                0x640,                                  /* adv_intv_max: 0x640 * 0.625[ms] = 1000[ms] */
                RBLE_GAP_ADV_CONN_UNDIR,                /* adv_type */
                RBLE_ADDR_PUBLIC,                       /* own_addr_type */
                RBLE_ADDR_PUBLIC,                       /* direct_addr_type */
                {0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12},   /* direct_addr */
                RBLE_ADV_ALL_CHANNELS,                  /* adv_chnl_map */
                RBLE_ADV_ALLOW_SCAN_ANY_CON_ANY,        /* adv_filt_policy */
                0x00                                    /* reserved */
            },
            {
                3+8+18,                 /* adv_data_len */
                2, 0x01, 0x06,          /* adv_data */
//                8, 0x09, 'R', 'E', 'L', '-', 'B', 'L', 'E',
                7, 0x09, 'S', 'l', 'e', 'e', 'i', 'm',			/* RD8001‘Î‰ž */
                17, 0x07,
                0x1B, 0xC5, 0xD5, 0xA5, 0x02, 0x00, 0xB8, 0x8C,
                0xE5, 0x11, 0x1B, 0xA2, 0x01, 0x00, 0x8C, 0xD6,
            },
            {
                0,                      /* scan_rsp_data_len */
                0x0, 0x0, 0x0, 0x0,     /* scan_rsp_data */
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0
            }
        }
    }, {
        RBLE_GAP_GEN_DISCOVERABLE,      /* disc_mode */
        RBLE_GAP_UND_CONNECTABLE,       /* conn_mode */
        {
            {
                0x960,                                  /* adv_intv_min: 0x0960 * 0.625[ms] = 1.5[s] */
                0x12C0,                                 /* adv_intv_max: 0x12C0 * 0.625[ms] = 3.0[s] */
                RBLE_GAP_ADV_CONN_UNDIR,                /* adv_type */
                RBLE_ADDR_PUBLIC,                       /* own_addr_type */
                RBLE_ADDR_PUBLIC,                       /* direct_addr_type */
                {0xBC, 0x9A, 0x78, 0x56, 0x34, 0x12},   /* direct_addr */
                RBLE_ADV_ALL_CHANNELS,                  /* adv_chnl_map */
                RBLE_ADV_ALLOW_SCAN_ANY_CON_ANY,        /* adv_filt_policy */
                0x00                                    /* reserved */
            },
            {
                3+8+18,                 /* adv_data_len */
                2, 0x01, 0x06,          /* adv_data */
//                8, 0x09, 'R', 'E', 'L', '-', 'B', 'L', 'E',
                7, 0x09, 'S', 'l', 'e', 'e', 'i', 'm',			/* RD8001‘Î‰ž */
                17, 0x07,

                0x1B, 0xC5, 0xD5, 0xA5, 0x02, 0x00, 0xB8, 0x8C,
                0xE5, 0x11, 0x1B, 0xA2, 0x01, 0x00, 0x8C, 0xD6,
            },
            {
                0,                      /* scan_rsp_data_len */
                0x0, 0x0, 0x0, 0x0,     /* scan_rsp_data */
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0, 0x0,
                0x0, 0x0, 0x0
            }
        }
    }

};

const RBLE_BOND_PARAM g_app_default_bond_param[] = {
    {
        {0},                            /* addr */
        RBLE_OOB_AUTH_DATA_NOT_PRESENT, /* oob */
        RBLE_IO_CAP_NO_INPUT_NO_OUTPUT, /* iocap */
        RBLE_AUTH_REQ_NO_MITM_BOND,     /* auth */
        RBLE_KEY_LEN,                   /* key_size */
        RBLE_KEY_DIST_NONE,             /* ikey_dist */
        RBLE_KEY_DIST_ENCKEY,           /* rkey_dist */
    }
};

const RBLE_BOND_RESP_PARAM g_app_default_bond_resp_param[] = {
    {
        0,                              /* conhdl */
        RBLE_OK,                        /* accept */
        RBLE_IO_CAP_NO_INPUT_NO_OUTPUT, /* io_cap */
        RBLE_OOB_AUTH_DATA_NOT_PRESENT, /* oob */
        RBLE_AUTH_REQ_NO_MITM_BOND,     /* auth_req */
        RBLE_KEY_LEN,                   /* max_key_size */
        RBLE_KEY_DIST_NONE,             /* ikeys */
        RBLE_KEY_DIST_ENCKEY,           /* rkeys */
        0,                              /* reserved */
    }
};
