/**
 ****************************************************************************************
 *
 * @file        ll.h
 *
 * @brief       Declaration of low-level (builtin) functions.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _LL_H_
#define _LL_H_

#if defined(_USE_IAR_RL78)
 //IAR header for intrinsic functions of RL78 corresponding to CPU instructions
 #include "intrinsics.h"
#elif defined(_USE_REL_RL78)
 typedef unsigned char __istate_t;
 extern __istate_t __get_interrupt_state(void);
 #define __enable_interrupt()   { EI(); }   // required {} for safety usage
 #define __disable_interrupt()  { DI(); }   // required {} for safety usage
 #define __no_operation()   NOP()
 #define __halt()   HALT()
#elif defined(_USE_CCRL_RL78)
 #include "iodefine.h"
 typedef unsigned char __istate_t;
 #define __get_interrupt_state() __get_psw()
 #define __enable_interrupt()    { __EI(); }    // required {} for safety usage
 #define __disable_interrupt()   { __DI(); }    // required {} for safety usage
 #define __no_operation()        __nop()
#else
 typedef unsigned char __istate_t;
 extern __istate_t __get_interrupt_state(void);
 extern void __enable_interrupt(void);
 extern void __disable_interrupt(void);
 extern void __no_operation(void);
#endif //_USE_IAR

/**
 ****************************************************************************************
 * @brief Enable interrupts globally in the system.
 * This macro must be used when the initialization phase is over and the interrupts
 * can start being handled by the system.
 *
 * **************************************************************************************
 */

#define GLOBAL_INT_START()                                                   \
 do {                                                                        \
     __enable_interrupt();                                                   \
 } while(0)

/**
 *****************************************************************************************
 * @brief Disable interrupts globally in the system.
 * This macro must be used when the system wants to disable all the interrupts
 * it could handle.
 *
 *****************************************************************************************
 */

#define GLOBAL_INT_STOP()                                                    \
 do {                                                                        \
     __disable_interrupt();                                                  \
 } while(0)


/**
 *****************************************************************************************
 * @brief Disable interrupts globally in the system.
 * This macro must be used in conjunction with the @ref GLOBAL_INT_RESTORE macro since this
 * last one will close the brace that the current macro opens.  This means that both
 * macros must be located at the same scope level.
 *
 *****************************************************************************************
 */

#if defined(_USE_IAR_RL78)
#define GLOBAL_INT_DISABLE()                                                 \
 do {                                                                        \
     __istate_t __int_state;                                                 \
     __int_state = __get_interrupt_state();                                  \
     __disable_interrupt();                                                  \

#elif defined(_USE_CCRL_RL78)
#define GLOBAL_INT_DISABLE()                                                 \
 do {                                                                        \
     __istate_t __int_state;                                                 \
     __int_state = __get_psw();                                              \
     __disable_interrupt();                                                  \

#else // defined(_USE_IAR_RL78)
 #define GLOBAL_INT_DISABLE()                                                \
 do {                                                                        \
     __asm("PUSH PSW");                                                      \
     __disable_interrupt();                                                  \

#endif // defined(_USE_IAR_RL78)


/**
 *****************************************************************************************
 * @brief Restore interrupts from the previous global disable.
 * @sa GLOBAL_INT_DISABLE
 *
 *****************************************************************************************
 */

#if defined(_USE_IAR_RL78)
 #define GLOBAL_INT_RESTORE()                                                \
     __set_interrupt_state(__int_state);                                     \
 } while(0)
#elif defined(_USE_CCRL_RL78)
 #define GLOBAL_INT_RESTORE()                                                \
     __set_psw(__int_state);                                                 \
 } while(0)
#else // defined(_USE_IAR_RL78)
 #define GLOBAL_INT_RESTORE()                                                \
     __asm("POP PSW");                                                       \
 } while(0)
#endif // defined(_USE_IAR_RL78)


/**
 *****************************************************************************************
 * @brief Invoke the wait for interrupt procedure of the processor.
 *
 * @warning It is suggested that this macro is called while the interrupts are disabled
 * to have performed the checks necessary to decide to move to sleep mode.
 *****************************************************************************************
 */
#if defined(_USE_CCRL_RL78)
 #define WFI()  __stop();
#else
 #define WFI()  __asm("stop");
#endif

#endif // LL_H_
