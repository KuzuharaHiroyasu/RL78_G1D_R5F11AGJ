/**
 ****************************************************************************************
 *
 * @file        arch.h
 *
 * @brief       This file contains the definitions of the macros and functions that are
 *              architecture dependent.  The implementation of those is implemented in the
 *              appropriate architecture directory.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _ARCH_H_
#define _ARCH_H_

/** @defgroup PLATFORM_DRIVERS PLATFORM_DRIVERS
 * @ingroup BLEREFIP
 * @brief Declaration of the platform architecture API.
 * @{
 */

// required to define GLOBAL_INT_** macros as inline assembly
#include "compiler.h"
#include "rwble_config.h"
#include "ll.h"
#ifdef CFG_LOG
#include "log_output.h"
#endif // CFG_LOG

/*
 * CPU WORD SIZE
 ****************************************************************************************
 */
// RENESAS RL78 is a 16-bit CPU
#if (UINT_MAX <= 65535u)
#define CPU_WORD_SIZE   2
#else
#define CPU_WORD_SIZE   4
#endif

/*
 * CPU Endianness
 ****************************************************************************************
 */
// RENESAS RL78 is little endian
#define CPU_LE          1

/// Possible errors detected by FW
#define    RESET_NO_ERROR         0x00000000
#define    RESET_MEM_ALLOC_FAIL   0xF2F2F2F2

/*
 * CPU Clock
 ****************************************************************************************
 */
/* CPU Clock is specified by macro "CLK_HOCO_xMHZ" of compile option in project setting */


/*
 * ASSERTION CHECK
 ****************************************************************************************
 */
void assert_err(const int8_t *condition, const int8_t * file, const int16_t line);
void assert_warn(const int8_t *condition, const int8_t * file, const int16_t line);

//#if BLE_DEBUG
#if 0	/* assert no use */
/// Assertions showing a critical error that could require a full system reset
#define ASSERT_ERR(cond)                              \
    do {                                              \
        if (!(cond)) {                                \
            assert_err(#cond, __MODULE__, __LINE__);  \
        }                                             \
    } while(0)

/// Assertions showing a non-critical problem that has to be fixed by the SW
#define ASSERT_WARN(cond)                             \
    do {                                              \
        if (!(cond)) {                                \
            assert_warn(#cond, __MODULE__, __LINE__); \
        }                                             \
    } while(0)

#else
/// Assertions showing a critical error that could require a full system reset
#define ASSERT_ERR(cond)

/// Assertions showing a non-critical problem that has to be fixed by the SW
#define ASSERT_WARN(cond)

#endif

/*
 * LOG OUTPUT
 ****************************************************************************************
 */
#ifdef CFG_LOG
#define LOG_OUTPUT  log_output_print
#else // CFG_LOG
#define LOG_OUTPUT
#endif // CFG_LOG


/**
 ****************************************************************************************
 * @brief Re-boot FW.
 *
 * This function currently only causes an ASSERT_ERR.
 *
 * This function should be used to re-boot the FW when error has been detected,
 * it is the end of the current FW execution.
 * After waiting transfers on UART to be finished, and storing the information that
 * FW has re-booted by itself in a non-loaded area, the FW restarts by branching at FW
 * entry point.
 *
 * Note: when calling this function, the code after it will not be executed.
 *
 * @param[in] error      Error detected by FW
 ****************************************************************************************
 */
void platform_reset(uint32_t error);

/// @}
#endif // _ARCH_H_
