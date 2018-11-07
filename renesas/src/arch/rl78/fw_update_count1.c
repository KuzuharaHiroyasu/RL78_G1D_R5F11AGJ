/**
 ****************************************************************************************
 *
 * @file        fw_update_count1.c
 *
 * @brief       This file contains the definition of the fw update count1.
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifdef _USE_IAR_RL78
#define  _MAIN_CN1 _Pragma("location=\"MAIN_CN1\"")
#else
#define _MAIN_CN1
#if !defined(_USE_CCRL_RL78)
#pragma section @@CNSTL MAIN_CN1
#else
#pragma section const   MAIN_CN1
#endif
#endif

#include "compiler.h"

_MAIN_CN1 const __far uint16_t fw_update_count_1 = 0;

