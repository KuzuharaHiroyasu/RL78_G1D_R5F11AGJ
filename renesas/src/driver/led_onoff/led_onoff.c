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
 * File Name  : led_onoff.c
 * Version    : 1.0
 * Description: LED driver for LED3/4 source file
 *
 * Copyright(C) 2012-2016 Renesas Electronics Corporation
 ******************************************************************************/

/*******************************************************************************
 * Include Files
 ******************************************************************************/
#include "arch.h"
#include "hw_config.h"
#include "port.h"
#include "led_onoff.h"

/*******************************************************************************
 * Function Definitions (API)
 ******************************************************************************/
int led_onoff_init(R_LED_DEV led)
{
    switch (led) {
    case R_LED3:
        write1_sfr(PM0, 3, PORT_OUTPUT);
        write1_sfr(PMC0, 3, 0);
        write1_sfr(P0, 3, 1);
        break;

    case R_LED4:
        write1_sfr(PM6, 0, PORT_OUTPUT);
        write1_sfr(P6, 0, 1);
        break;

    default:
        return -1;
    }

    return 0;
}

int led_onoff_set(R_LED_DEV led, R_LED_STATE state)
{
    uint8_t set;

    set = (uint8_t)((state == R_LED_STATE_ON) ? 0 : 1);

    switch (led) {
    case R_LED3:
        write1_sfr(P0, 3, set);
        break;

    case R_LED4:
        write1_sfr(P6, 0, set);
        break;

    default:
        return -1;
    }

    return 0;
}

int led_onoff_state(R_LED_DEV led, R_LED_STATE *state)
{
    uint16_t val;

    switch (led) {
    case R_LED3:
        val = read1_sfr(P0, 3);
        break;

    case R_LED4:
        val = read1_sfr(P6, 0);
        break;

    default:
        return -1;
    }

    *state = (R_LED_STATE)((val == 0) ? R_LED_STATE_ON : R_LED_STATE_OFF);

    return 0;
}
