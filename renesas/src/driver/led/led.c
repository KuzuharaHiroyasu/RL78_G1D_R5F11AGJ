/**
 ****************************************************************************************
 *
 * @file        led.c
 *
 * @brief       LED initialisation and behaviour functions.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @addtogroup PLATFORM_DRIVERS
 * @{
 ****************************************************************************************
 */
#ifdef CONFIG_EMBEDDED
  #if defined(_USE_REL_RL78)
    #pragma interrupt INTTM00 led_timer_isr
  #elif defined(_USE_CCRL_RL78)
    #include "iodefine.h"
    #pragma interrupt led_timer_isr (vect=INTTM00)
  #endif
#endif

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"
#include "hw_config.h"

//port sttings utilities
#include "port.h"

//led defines
#include "led.h"


/*
 * DEFINES
 ****************************************************************************************
 */



/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
#ifdef CONFIG_EMBEDDED
static struct led_env_tag led_env;
#endif

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 *****************************************************************************************
 * @brief Static function for Port initialization needed for LED related pins.
 *****************************************************************************************
 */
static void led_pin_init(void)
{
#ifdef CONFIG_EMBEDDED
    // set digital in/out mode
    write1_sfr(PMC12, 0, 0);
    write1_sfr(PMC14, 7, 0);

    //clear port register(extension board LED)
    write1_sfr(P12, 0, 0);
    write1_sfr(P14, 7, 0);

    //use port as output
    write1_sfr(PM12,  0, PORT_OUTPUT);
    write1_sfr(PM14,  7, PORT_OUTPUT);
#else
    // set digital in/out mode
    write1_sfr(PMC12, 0, 0);
    write1_sfr(PMC14, 7, 0);
    write1_sfr(PMC0, 3, 0);

    // output high: LED1-3
    write1_sfr(P12, 0, 1);
    write1_sfr(P14, 7, 1);
    write1_sfr(P0, 3, 1);
    // output low: LED4
    write1_sfr(P6, 0, 0);

    //use port as output
    write1_sfr(PM12,  0, PORT_OUTPUT);
    write1_sfr(PM14,  7, PORT_OUTPUT);
    write1_sfr(PM0, 3, PORT_OUTPUT);
    write1_sfr(PM6, 0, PORT_OUTPUT);
#endif
}


#ifdef CONFIG_EMBEDDED
/**
 *****************************************************************************************
 * @brief Static function for Timer 00 initialization needed for LED blinking period and
 *        interrupt.
 *****************************************************************************************
 */
static void led_timer_init(void)
{
    //switch on timer arry unit input clock
    write1_sfr(PER0, 0, 1);

    //select TAU clock frequency CK00: fx/256
    write_sfrp(TPS0, CK00_FREQ_CPU_256);

    //set timer mode TAU channel CK00:
    write_sfrp(TMR00, 0x0000);    /* - operation clock set by PRS register
                                     - clock specified by CKS0 bit (bit 15)
                                     - slave channel
                                     - software start trigger
                                     - interval timer mode                  */

     //set 10ms interval time
    #if defined(CLK_FCLK_4MHZ)
    write_sfrp(TDR00, 155);       /* TDR00 = (10ms * 4MHz / 256) -1         */
    #elif defined(CLK_FCLK_8MHZ)
    write_sfrp(TDR00, 312);       /* TDR00 = (10ms * 8MHz / 256) -1         */
    #elif defined(CLK_FCLK_16MHZ)
    write_sfrp(TDR00, 624);       /* TDR00 = (10ms * 16MHz / 256) -1         */
    #elif defined(CLK_FCLK_32MHZ)
    write_sfrp(TDR00, 1249);      /* TDR00 = (10ms * 32MHz / 256) -1         */
    #endif

    write_sfrp(TOE0, (read_sfrp(TOE0) & 0xFE));
    write_sfrp(TO0,  (read_sfrp(TO0)  & 0xFE));
    write_sfrp(TOM0, (read_sfrp(TOM0) & 0xFE));
    write_sfrp(TOL0, (read_sfrp(TOL0) & 0xFE));

    //clear TAU ch-0 interrupt request flag
    write1_sfrbit(TMIF00, 0);

    //enable TAU ch-0 interrupt
    write1_sfrbit(TMMK00, IRQ_ENABLE);
    write_sfrp(TS0, (read_sfrp(TS0) | 0x01));
}
#endif

#ifdef _NO_USE
void led2_set(int value)
{
    LED02 = (value != 1);
}
#endif //#ifdef _NO_USE

void led_init(void)
{
    //port settings
    led_pin_init();

    #ifdef CONFIG_EMBEDDED
    //timers
    led_timer_init();

    //initialise LEDs
    write1_sfrbit(LED01, OFF);
    write1_sfrbit(LED02, ON );

    //environment initialisation
    led_env.timer_flag  = 0;
    led_env.tick_10ms   = 0;
    #endif
}


void led_blink(void)
{
#ifdef CONFIG_EMBEDDED
    //new time tick
    if ((uint16_t)led_env.timer_flag == 1)
    {
      //clear timer flag
      led_env.timer_flag = 0;

      if(led_env.tick_10ms >= (uint16_t)PERIOD_100MSEC)
      {
          //toggle led1
          toggle1_sfrbit(LED01);
          //toggle led2
          toggle1_sfrbit(LED02);

          led_env.tick_10ms = 0;  
      }
    }
#endif
}

#ifdef CONFIG_EMBEDDED
#if defined(_USE_IAR_RL78)
#pragma vector=INTTM00_vect
#elif defined(_USE_REL_RL78)
// this pragma should be at the beginning of source file
#elif defined(_USE_CCRL_RL78)
// this pragma should be at the beginning of source file
#else
#warning "No interrupt vector is generated for led_timer_isr"
#endif
__IRQ void led_timer_isr (void)
{
    led_env.timer_flag = 1;
    led_env.tick_10ms++;
}
#endif

/// @} module
