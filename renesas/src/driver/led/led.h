/**
 ****************************************************************************************
 *
 * @file        led.h
 *
 * @brief       LED driver: control and defines for simple LED initialization and control.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef LED_H_
#define LED_H_

/**
 ****************************************************************************************
 * @addtogroup PLATFORM_DRIVERS
 * @brief LED driver.
 *
 * This driver allows simple initialisation of LED related registers and generating
 * a simple functionality for each passing in the main FW loop.
 *
 * @{
 ****************************************************************************************
 */


/*
 * DEFINES
 ****************************************************************************************
 */
#if (defined(_USE_REL_RL78) || defined(_USE_GCC_RL78))
 ///LED01 is pin 0 of Port 12
 #define LED01        P12.0
 
 ///LED2 is pin 7 on Port 14
 #define LED02        P14.7
#else
 ///LED01 is pin 0 of Port 12
 #define LED01        P12_bit.no0
 
 ///LED2 is pin 7 on Port 14
 #define LED02        P14_bit.no7
#endif

///LED on value is actually false
#define ON           0

///LED off value is actually true
#define OFF          1

///10ms timer
#define PERIOD_1SEC     100U
#define PERIOD_100MSEC   10U

///TAU Configuration channel 00
#define CK00_FREQ_CPU         0x00
#define CK00_FREQ_CPU_2       0x01
#define CK00_FREQ_CPU_4       0x02
#define CK00_FREQ_CPU_8       0x03
#define CK00_FREQ_CPU_16      0x04
#define CK00_FREQ_CPU_32      0x05
#define CK00_FREQ_CPU_64      0x06
#define CK00_FREQ_CPU_128     0x07
#define CK00_FREQ_CPU_256     0x08
#define CK00_FREQ_CPU_512     0x09
#define CK00_FREQ_CPU_1024    0x0A
#define CK00_FREQ_CPU_2048    0x0B
#define CK00_FREQ_CPU_4096    0x0C
#define CK00_FREQ_CPU_8192    0x0D
#define CK00_FREQ_CPU_16384   0x0E
#define CK00_FREQ_CPU_32768   0x0F


///LED environment context structure
struct led_env_tag
{
    ///Timer elapsed flag
    uint8_t  timer_flag;
    ///LEd blink interval
    uint16_t tick_10ms;
};


/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initialisation for LED related ports, timers and interrupt registers.
 ****************************************************************************************
 */
void led_init(void);

/**
 *****************************************************************************************
 * @brief Static function for Timer 00 initialization needed for LED blinking period and
 *        interrupt.
 *****************************************************************************************
 */
static void led_timer_init(void);

/**
 ****************************************************************************************
 * @brief LED activity function.
 *
 * LEDs have been initialised one off, one on. They change state every second(timer
 * interrupt). By board switch interrupt,one of their periods is increased.
 ****************************************************************************************
 */
void led_blink(void);

/**
 *****************************************************************************************
 * @brief Static function for Port initialization needed for LED related pins.
 *****************************************************************************************
 */
static void led_pin_init(void);

/**
 ****************************************************************************************
 * @brief LED timer interrupt service routine.
 *
 * Upon timer interrupt, the flag is raised and the period grows with 10ms.
 ****************************************************************************************
 */
__IRQ void led_timer_isr(void);

/// @} module

#endif // LED_H_
