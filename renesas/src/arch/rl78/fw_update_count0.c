/**
 ****************************************************************************************
 *
 * @file        fw_update_count0.c
 *
 * @brief       This file contains the definition of the fw update count0.
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifdef _USE_IAR_RL78
#define  _MAIN_CN0 _Pragma("location=\"MAIN_CN0\"")
#else
#define _MAIN_CN0
#if !defined(_USE_CCRL_RL78)
#pragma section @@CNSTL MAIN_CN0
#else
#pragma section const   MAIN_CN0
#endif
#endif

#include "compiler.h"

_MAIN_CN0 const __far uint16_t fw_update_count_0 = 0;

