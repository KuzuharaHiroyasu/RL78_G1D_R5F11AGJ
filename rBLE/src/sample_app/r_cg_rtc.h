/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only intended for use with Renesas products.
* No other uses are authorized. This software is owned by Renesas Electronics Corporation and is protected under all
* applicable laws, including copyright laws. 
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIESREGARDING THIS SOFTWARE, WHETHER EXPRESS, IMPLIED
* OR STATUTORY, INCLUDING BUT NOT LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY
* LAW, NEITHER RENESAS ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE FOR ANY DIRECT,
* INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR
* ITS AFFILIATES HAVE BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software and to discontinue the availability 
* of this software. By using this software, you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) . All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_rtc.h
* Version      :  
* Device(s)    : R5F11NGG
* Tool-Chain   : CCRL
* Description  : This file implements device driver for RTC module.
* Creation Date: 2018/04/11
***********************************************************************************************************************/
#ifndef RTC_H
#define RTC_H

/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Real-time clock control register 0 (RTCC0) 
*/
/* High-accuracy real-time clock operation control (RTCE) */
#define _00_RTC_COUNTER_STOP                   (0x00U) /* stops counter operation */
#define _80_RTC_COUNTER_START                  (0x80U) /* starts counter operation */
/* RTC1HZ pin output control (RCLOE1) */
#define _00_RTC_RTC1HZ_DISABLE                 (0x00U) /* disables output of the RTC1HZ pin (1 Hz) */
#define _20_RTC_RTC1HZ_ENABLE                  (0x20U) /* enables output of the RTC1HZ pin (1 Hz) */
/* 12-/24-hour system select (AMPM) */
#define _00_RTC_12HOUR_SYSTEM                  (0x00U) /* 12-hour system */
#define _08_RTC_24HOUR_SYSTEM                  (0x08U) /* 24-hour system */
#define _08_RTC_RTCC0_AMPM                     (0x08U) /* AMPM bit status detect */
#define _08_RTC_HOURSYSTEM_CLEAR               (0x08U) /* hour system select clear */
/* Constant-period interrupt (INTRTC) selection (CT2,CT1,CT0) */
#define _00_RTC_INTRTC_NOT_GENERATE            (0x00U) /* does not use fixed-cycle interrupt function */
#define _01_RTC_INTRTC_CLOCK_0                 (0x01U) /* once per 0.5 s */
#define _02_RTC_INTRTC_CLOCK_1                 (0x02U) /* once per 1 s */
#define _03_RTC_INTRTC_CLOCK_2                 (0x03U) /* once per 1 m */
#define _04_RTC_INTRTC_CLOCK_3                 (0x04U) /* once per 1 hour */
#define _05_RTC_INTRTC_CLOCK_4                 (0x05U) /* once per 1 day */
#define _06_RTC_INTRTC_CLOCK_5                 (0x06U) /* once per 1 month */
#define _07_RTC_INTRTC_CLEAR                   (0x07U) /* INTRTC clear */

/*
    Real-time clock control register 1 (RTCC1) 
*/
/* Alarm operation control (WALE) */
#define _00_RTC_ALARM_DISABLE                  (0x00U) /* match operation is invalid */
#define _80_RTC_ALARM_ENABLE                   (0x80U) /* match operation is valid */
/* Control of alarm interrupt function operation (WALIE) */
#define _00_RTC_ALARM_INT_DISABLE              (0x00U) /* does not generate interrupt on matching of alarm */
#define _40_RTC_ALARM_INT_ENABLE               (0x40U) /* generates interrupt on matching of alarm */
/* Correction timing signal interrupt function operation (RITE) */
#define _00_RTC_INTRTIT_INT_DISABLE            (0x00U) /* does not generate interrupt of correction timing signal */
#define _20_RTC_INTRTIT_INT_ENABLE             (0x20U) /* generates interrupt of correction timing signal */
/* Alarm detection status flag (WAFG) */
#define _00_RTC_ALARM_MISMATCH                 (0x00U) /* alarm mismatch */
#define _10_RTC_ALARM_MATCH                    (0x10U) /* detection of matching of alarm */
/* Constant-period interrupt status flag (RIFG) */
#define _00_RTC_INTC_NOTGENERATE_FALG          (0x00U) /* constant-period interrupt is not generated */
#define _08_RTC_INTC_GENERATE_FLAG             (0x08U) /* constant-period interrupt is generated */
/* Wait status flag of high-accuracy real-time clock (RWST) */
#define _00_RTC_COUNTER_OPERATING              (0x00U) /* counter is operating */
#define _02_RTC_COUNTER_STOP                   (0x02U) /* mode to read or write counter value */
/* Wait control of high-accuracy real-time clock (RWAIT) */
#define _00_RTC_COUNTER_SET                    (0x00U) /* sets counter operation */
#define _01_RTC_COUNTER_PAUSE                  (0x01U) /* stops SEC to YEAR counters */


/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
#define RTC_WAITTIME_2FRTC                     (266U) /* change the waiting time according to the system */

/***********************************************************************************************************************
Typedef definitions
***********************************************************************************************************************/
typedef struct 
{
    UB sec;
    UB min;
    UB hour;
    UB day;
    UB week;
    UB month;
    UB year;
}rtc_counter_value_t;

typedef struct
{
    UB alarmwm;
    UB alarmwh;
    UB alarmww;
}rtc_alarm_value_t;

typedef enum
{ 
    HOUR12,
    HOUR24
}rtc_hour_system_t;

typedef enum 
{
    HALFSEC = 1U,
    ONESEC,
    ONEMIN,
    ONEHOUR,
    ONEDAY,
    ONEMONTH
}rtc_int_period_t;

/***********************************************************************************************************************
Global functions
***********************************************************************************************************************/
void R_RTC_Create(void);
void R_RTC_Start(void);
void R_RTC_Stop(void);
MD_STATUS R_RTC_Get_CounterValue(rtc_counter_value_t * const counter_read_val);
MD_STATUS R_RTC_Set_CounterValue(rtc_counter_value_t counter_write_val);
/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
