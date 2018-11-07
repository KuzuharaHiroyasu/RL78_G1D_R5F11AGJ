/**
 ****************************************************************************************
 *
 * @file        wakeup.c
 *
 * @brief WAKEUP Driver
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.205
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup WAKEUP
 *
 * @{
 ****************************************************************************************
 */
#ifndef CONFIG_EMBEDDED
 #define USE_WAKEUP_SIGNAL_PORT  (0) /* Modem Setting */
#else
 #define USE_WAKEUP_SIGNAL_PORT  (0) /* Embedded Setting */
#endif

#if USE_WAKEUP_SIGNAL_PORT
 #if defined(_USE_REL_RL78)
  #pragma interrupt INTP3 wakeup_isr
 #elif defined(_USE_CCRL_RL78)
  #include "iodefine.h"
  #pragma interrupt wakeup_isr (vect=INTP3)
 #endif
#endif

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "wakeup.h"
#include "serial.h"
#include "port.h"


#if (((!USE_WAKEUP_SIGNAL_PORT) && (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE || SERIAL_C_5WIRE)) || \
     (( USE_WAKEUP_SIGNAL_PORT) && (SERIAL_U_2WIRE || SERIAL_C_4WIRE)))
 #error "USE_WAKEUP_SIGNAL_PORT setting is wrong."
#endif

#ifndef CONFIG_EMBEDDED
extern void RSCIP_Wakeup(void);
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
#define IRQ_SET     1

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Initialize WAKEUP driver.
 *****************************************************************************************
 */
void wakeup_init_ent(void)
{
    #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE || SERIAL_C_5WIRE)
    /* WAKEUP pin */
    write1_sfr(PM3 , 0, PORT_INPUT);
    /* set pull-up */
    write1_sfr(PU3, 0, PORT_PULLUP);
    #endif

    #if (SERIAL_U_3WIRE || SERIAL_C_5WIRE)
    /* detect falling edge */
    write1_sfr(EGP0, 3, 0);
    write1_sfr(EGN0, 3, 1);

    /* clear and enable INTP3 */
    write1_sfrbit(PIF3, IRQ_CLEAR);
    write1_sfrbit(PMK3, IRQ_ENABLE);
    #elif SERIAL_U_DIV_2WIRE
    /* detect both edge */
    write1_sfr(EGP0, 3, 1);
    write1_sfr(EGN0, 3, 1);

    /* disable INTP3 */
    write1_sfrbit(PMK3, IRQ_DISABLE);
    #endif
}

/**
 ****************************************************************************************
 * @brief Get ready for wakeup.
 *
 * @return bool     true if wakeup is ready, or false if not
 *****************************************************************************************
 */
bool wakeup_ready(void)
{
    #if (SERIAL_U_3WIRE || SERIAL_C_5WIRE)
    if((read_sfr(PIF3) != 0) || (read1_sfr(P3, 0) == 0))
    {
        return false;
    }
    /* suspend the serial */
    return serial_stop_ready();
    #elif SERIAL_U_DIV_2WIRE
    if(read1_sfr(P3, 0) == 0)
    {
        return false;
    }
    /* suspend the serial */
    if(serial_stop_ready() == false)
    {
        return false;
    }
    /* clear and enable INTP3 */
    write1_sfrbit(PIF3, IRQ_CLEAR);
    write1_sfrbit(PMK3, IRQ_ENABLE);
    return true;
    #else /* (SERIAL_U_2WIRE || SERIAL_C_4WIRE) */
    /* suspend the serial */
    return serial_stop_ready();
    #endif
}

/**
 ****************************************************************************************
 * @brief Finish wakeup.
 *****************************************************************************************
 */
void wakeup_finish(void)
{
    #if SERIAL_U_DIV_2WIRE
    if(read_sfr(PIF3) == 0)
    {
        /* resume the serial */
        serial_stop_finish();

        /* disable INTP3 */
        write1_sfrbit(PMK3, IRQ_DISABLE);
    }
    else
    {
        /* check if REQ byte is still receiving */
        if(read1_sfr(P3, 0) == 0)
        {
            /* clear INTP3 interrupt flag */
            write1_sfrbit(PIF3, IRQ_CLEAR);

            /* check if REQ byte receiving just finished */
            if(read1_sfr(P3, 0) != 0)
            {
                /* set INTP3 interrupt flag */
                write1_sfrbit(PIF3, IRQ_SET);
            }
        }
    }
    #else /* (SERIAL_U_2WIRE || SERIAL_U_3WIRE || SERIAL_C_4WIRE || SERIAL_C_5WIRE) */
    /* resume the serial */
    serial_stop_finish();
    #endif
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for request to wakeup.
 *****************************************************************************************
 */
#if USE_WAKEUP_SIGNAL_PORT
#if defined(_USE_IAR_RL78)
#pragma vector = INTP3_vect
#elif defined(_USE_REL_RL78)
/* this pragma should be at the beginning of source file */
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is genarated for wakeup_isr"
#endif
__IRQ void wakeup_isr(void)
{
    #if SERIAL_U_DIV_2WIRE
    /* resume the serial */
    serial_stop_finish();

    /* disable INTP3 */
    write1_sfrbit(PMK3, IRQ_DISABLE);
    #endif

    serial_wakeup_response();

    #ifndef CONFIG_EMBEDDED
    RSCIP_Wakeup();
    #endif
}
#endif
