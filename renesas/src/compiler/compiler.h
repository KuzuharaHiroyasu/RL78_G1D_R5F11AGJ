/**
 ****************************************************************************************
 *
 * @file        compiler.h
 *
 * @brief       Definitions of compiler specific directives.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _COMPILER_H_
#define _COMPILER_H_

/* foolproof */
#if ((defined(_USE_GCC_RL78) + defined(_USE_IAR_RL78) + defined(_USE_REL_RL78) + defined(_USE_CCRL_RL78)) != 1)
#warnig "Bad configuration: _USE_*"
#endif

/**************************************************************/
/*  General characteristics of each compiler                  */
/**************************************************************/

#if defined(_USE_REL_RL78)
#define _USE_STD_C89    1   // CubeSuite+ (RL78) only supports C89
#define _USE_PADDING2   1   // -rc reuires explicit paddings
#undef  _USE_INLINE         // inline is not supported by C89
#elif defined(_USE_CCRL_RL78)
#define _USE_STD_C99_C  1   // CC-RL is supporting the part of C99.
#define _USE_PADDING2   1   // -rc reuires explicit paddings
#undef  _USE_INLINE         // don't use this to reduce code size
#elif defined(_USE_IAR_RL78)
#define _USE_STD_C99    1   // Embedded Workbench supports C99
#undef  _USE_PADDING2       // not required
#undef  _USE_INLINE         // don't use this to reduce code size
#elif defined(_USE_GCC_RL78)
#define _USE_STD_C99    1   // GCC can support C99 with -std=c99
#undef  _USE_PADDING2       // not required
#undef  _USE_INLINE         // don't use this to reduce code size
#else
#error "Unknown compiler"
#endif

/* These pragmas should be placed at the beginning of source module... */
#if defined(_USE_REL_RL78)
#pragma sfr     /* sfr support */
#pragma ei      /* builtin function support */
#pragma di
#pragma nop
#pragma halt
#pragma stop
#endif

/**************************************************************/
/*  Include C standard header files                           */
/**************************************************************/

#include <stddef.h>     /* C89 freestanding */
#include <stdarg.h>     /* C89 freestanding */
#include <limits.h>     /* C89 freestanding */

#if defined(_USE_STD_C99)
#include <stdint.h>     /* C99 freestanding */
#include <stdbool.h>    /* C99 freestanding */
#elif defined(_USE_STD_C99_C)
#include <stdint.h>     /* C99 freestanding */
typedef unsigned char bool;
#define true        1
#define false       0
#else
typedef signed char int8_t;
typedef signed short int16_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
#if (ULONG_MAX == 4294967295UL)
typedef signed long int32_t;
typedef unsigned long uint32_t;
#else
#error "define int32_t and uint32_t manually."
#endif
typedef unsigned long uintptr_t;
typedef unsigned char _Bool;
#define bool        _Bool
#define true        1
#define false       0
#endif /* defined(_USE_STD_C99) */

#include <string.h>     /* C89 hosted */
#include <stdlib.h>     /* C89 hosted */

#ifndef _char_t_defined_
typedef char char_t;
#define _char_t_defined_
#endif

/**************************************************************/
/*  KPIT GCC for RL78                                         */
/**************************************************************/

#if defined(_USE_GCC_RL78)

#include "r5f11agj.h"   /* sfr and builtin function support */

#define __BLEIRQ    __attribute__((interrupt))
#define __IRQ       __attribute__((interrupt))
#define __FIQ       __attribute__((interrupt))
#ifdef _USE_INLINE
#define __INLINE    __always_inline__
#endif

#endif /* defined(_USE_GCC_RL78) */

/**************************************************************/
/*  REL CubeSuite+ for RL78                                   */
/**************************************************************/

#if defined(_USE_REL_RL78)

/* Note that bitnum should be an immediate decimal number
   from 0 to 7 (sfr) or from 0 to 15 (sfrp) inclusive. */
#define read_sfr(sfr)               (sfr)
#define read_sfrp(sfrp)             (sfrp)
#define write_sfr(sfr, v)           (sfr = (v))
#define write_sfrp(sfrp, v)         (sfrp = (v))
#define read1_sfr(sfr, bitnum)      (sfr.bitnum)
#define write1_sfr(sfr, bitnum, v)  (sfr.bitnum = (v))
#define write1_sfrbit(sfrbit, v)    (sfrbit = (v))
#define write1_sfr2bit(sfrbit, v)   (sfrbit = (v))
#define toggle1_sfr(sfr, bitnum)    (sfr ^= (1u << (bitnum)))
#define toggle1_sfrbit(sfrbit)      (sfrbit ^= 1)
#define __sfr_function_defined__    1

#define __BLEIRQ        __interrupt
#define __IRQ __interrupt
#define __FIQ           __interrupt

#endif /* defined(_USE_REL_RL78) */

/**************************************************************/
/*  REL CS+ & CC-RL for RL78                                  */
/**************************************************************/

#if defined(_USE_CCRL_RL78)

#include "iodefine.h"   /* sfr and builtin function support */ /* for CC-RL */

/* Note that bitnum should be an immediate decimal number
   from 0 to 7 (sfr) or from 0 to 15 (sfrp) inclusive. */
#define read_sfr(sfr)               (sfr)
#define read_sfrp(sfrp)             (sfrp)
#define write_sfr(sfr, v)           (sfr = (v))
#define write_sfrp(sfrp, v)         (sfrp = (v))
#define read1_sfr(sfr, bitnum)      (sfr.bitnum)
#define write1_sfr(sfr, bitnum, v)  (sfr.bitnum = (v))
#define write1_sfrbit(sfrbit, v)    (sfrbit = (v))
#define write1_sfr2bit(sfrbit, v)   (sfrbit = (v))
#define toggle1_sfr(sfr, bitnum)    (sfr ^= (1u << (bitnum)))
#define toggle1_sfrbit(sfrbit)      (sfrbit ^= 1)
#define __sfr_function_defined__    1

#define __BLEIRQ
#define __IRQ
#define __FIQ

#endif /* defined(_USE_CCRL_RL78) */

/**************************************************************/
/*  IAR Embedded Workbench for RL78                           */
/**************************************************************/

#if defined(_USE_IAR_RL78)

#include <ior5f11agj.h>
#include <ior5f11agj_ext.h> /* sfr support */
#include <intrinsics.h>     /* builtin function support */

#define __BLEIRQ        __interrupt
#define __IRQ           __interrupt
#define __FIQ __interrupt
#define __MODULE__      __BASE_FILE__
#ifdef _USE_INLINE
#define __INLINE        _Pragma("inline=forced")
#endif

#endif /* defined(_USE_IAR_RL78) */

/**************************************************************/
/*  Defaults                                                  */
/**************************************************************/

#if (!defined(__INLINE))
#if defined(_USE_INLINE)
#define __INLINE        static inline
#else
#define __INLINE        /* not supported */
#endif
#endif

#if (!defined(__BLEIRQ))
#define __BLEIRQ        /* not supported */
#define __IRQ           /* not supported */
#define __FIQ           /* not supported */
#endif

#if (!defined(__MODULE__))
#if 0
static const char __module__[];
#define __MODULE__  __module__
#else
#define __MODULE__  __FILE__
#endif
#endif

#if (defined(_USE_REL_RL78))        /* TODO: replace with memory model (large) */
#define codeptr     __far           /* qualifier for code pointer */
#define dataptr     __far           /* qualifier for data pointer */
typedef unsigned long uintcodeptr_t;        /* uintptr_t for code pointer */
typedef unsigned long uintdataptr_t;        /* uintptr_t for data pointer */
#else /* defined(_USE_IAR_RL78) || defined(_USE_GCC_RL78) */ /* TODO: medium */
#define codeptr     /*__far*/       /* qualifier for code pointer */
#define dataptr     __near          /* qualifier for data pointer */
typedef unsigned long uintcodeptr_t;        /* uintptr_t for code pointer */
typedef unsigned int uintdataptr_t;         /* uintptr_t for data pointer */
#endif

#if (!defined(__sfr_function_defined__))
/* core macro for string coupling code(##) */
#define read1_sfr_core(sfr,bitnum)      (sfr ## _bit.no ## bitnum)
#define write1_sfr_core(sfr,bitnum,v)   (sfr ## _bit.no ## bitnum = (v))
#define toggle1_sfr_core(sfr, bitnum)   (sfr ## _bit.no ## bitnum ^= 1)

#define read_sfr(sfr)                   (sfr)
#define read_sfrp(sfrp)                 (sfrp)
#define write_sfr(sfr, v)               (sfr = (v))
#define write_sfrp(sfrp, v)             (sfrp = (v))
#define read1_sfr(sfr, bitnum)          read1_sfr_core(sfr,bitnum)
#define write1_sfr(sfr, bitnum, v)      write1_sfr_core(sfr,bitnum,v)
#define write1_sfrbit(sfrbit, v)        (sfrbit = (v))
#define write1_sfr2bit(sfrbit, v)       (sfrbit = (v))
#define toggle1_sfr(sfr, bitnum)        toggle1_sfr_core(sfr, bitnum)
#define toggle1_sfrbit(sfrbit)          (sfrbit ^= 1)
#endif

/* foolproof */
#if ((defined(_USE_STD_C89) + defined(_USE_STD_C99) + defined(_USE_STD_C99_C)) != 1)
#error "Both _USE_STD_C89 and _USE_STD_C99 are defined."
#endif

#if defined(_USE_IAR_RL78)
/* defined for noinit section */
#define __NO_INIT       __no_init
#else   /* other compiler */
#define __NO_INIT
#endif

#if defined(_USE_IAR_RL78)
#if __VER__ <= 140
    /* Before IAR v1.40 */
    #define IAR_OLD_VERSION
    #define CALLT  __callt
#else
    #define CALLT
#endif
#define DPRTN
#elif defined(_USE_CCRL_RL78)
#define CALLT  __callt
#define DPRTN  __near
#else
#define CALLT  callt
#define DPRTN
#endif

#endif // _COMPILER_H_
