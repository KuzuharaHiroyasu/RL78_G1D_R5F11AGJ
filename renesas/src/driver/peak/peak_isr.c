/**
 ****************************************************************************************
 *
 * @file peak_isr.c
 *
 * @brief PEAK driver
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.217
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup PEAK
 *
 * @{
 ****************************************************************************************
 */
#if !defined(CLK_SUB_XT1)
 #define USE_LOCO_TIMER
#endif


#if defined(CFG_USE_PEAK) || defined(USE_LOCO_TIMER)
#if defined(_USE_REL_RL78)
#pragma interrupt INTIT peak_timer_isr
#elif defined(_USE_CCRL_RL78)
#include "iodefine.h"
#pragma interrupt peak_timer_isr (vect=INTIT)
#endif
#endif //CFG_USE_PEAK

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "compiler.h"

#if defined(CFG_USE_PEAK) || defined(USE_LOCO_TIMER)
/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
#ifdef USE_LOCO_TIMER
extern void loco_timer_isr_func(void);
#else//USE_LOCO_TIMER
extern void peak_timer_isr_func(void);
#endif//USE_LOCO_TIMER

/**
 ****************************************************************************************
 * @brief  The peak current consumption notification interrupt handler
 ****************************************************************************************
 */
#if defined(_USE_IAR_RL78)
#pragma vector=INTIT_vect
#elif defined(_USE_REL_RL78)
// this pragma should be at the beginning of source file
#elif defined(_USE_CCRL_RL78)
// this pragma should be at the beginning of source file
#else
#warning "No interrupt vector is generated for peak_timer_isr"
#endif
__IRQ void peak_timer_isr (void)
{
#ifdef USE_LOCO_TIMER
    loco_timer_isr_func();
#else//USE_LOCO_TIMER
    peak_timer_isr_func();
#endif//USE_LOCO_TIMER
}
#endif //defined(CFG_USE_PEAK) || defined(USE_LOCO_TIMER)


/// @} PEAK
