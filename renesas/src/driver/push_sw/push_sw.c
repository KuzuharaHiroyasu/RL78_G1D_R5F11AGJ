/**
 ****************************************************************************************
 *
 * @file push_sw.c
 *
 * @brief Application Component initialisation and behaviour functions.
 *
 * Copyright (C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 0000 $
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup PLATFORM_DRIVERS
 * @{
 ****************************************************************************************
 */
#if defined(USE_SAMPLE_PROFILE) | defined(USE_FW_UPDATE_PROFILE)
 #if defined(_USE_REL_RL78)
  #pragma interrupt INTP5  push_sw2_interrupt
 #elif defined(_USE_CCRL_RL78)
  #include "iodefine.h"
  #pragma interrupt push_sw2_interrupt (vect=INTP5)
 #endif
#endif /* #ifdef USE_SAMPLE_PROFILE */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"

//port sttings utilities
#include "port.h"

#include "push_sw.h"


#if defined(USE_SAMPLE_PROFILE) | defined(USE_FW_UPDATE_PROFILE)

static void dummy_cb( void );

static PUSH_CB Call_Back = dummy_cb;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

void push_sw2_start( PUSH_CB cb )
{
    Call_Back = cb;

    //enable INTP5 function
    PIOR &= 0xEF;
    //use port P1.6 as input
    write1_sfr(PM1, 6, PORT_INPUT);
    //enable internal pull-up
    write1_sfr(PU1, 6, PORT_PULLUP);

    PMK5 = 1U;  /* disable INTP5 operation */
    PIF5 = 0U;  /* clear INTP5 interrupt flag */

    /* Set INTP5 high priority */
    PPR15 = 0U;
    PPR05 = 0U;

    write1_sfr(EGP0, 5, 1);    /* select rising edge */
    write1_sfr(EGN0, 5, 0);    /* select rising edge */

    PIF5 = 0U;  /* clear INTP5 interrupt flag */
    PMK5 = 0U;  /* enable INTP5 interrupt */

}

static void dummy_cb( void )
{

}

#if defined(_USE_IAR_RL78)
#pragma vector = INTP5_vect
#elif defined(_USE_REL_RL78)
/* this pragma should be at the beginning of source file */
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is genarated for push_sw2_interrupt"
#endif
__IRQ void push_sw2_interrupt(void)
{
    Call_Back();
}

#endif /* #ifdef USE_SAMPLE_PROFILE */
/// @} module
