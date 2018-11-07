/*******************************************************************************
* Library       : Data Flash Access Library T02
*
* File Name     : $Source: fdl_descriptor_t02.h $
* Lib. Version  : $RL78_FDL_LIB_VERSION_T02_CCRL: V1.01 $
* Mod. Revision : $Revision: 1.2 $
* Mod. Date     : $Date: 2015/03/19 16:17:45JST $
* Device(s)     : RL78
* Description   : User specific library configuration parameters
*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only
* intended for use with Renesas products. No other uses are authorized. This
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2014, 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/



#ifndef __FDL_DESCRIPTOR_H_INCLUDED
#define __FDL_DESCRIPTOR_H_INCLUDED

#ifdef CFG_USE_EEL
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "fdl_types.h"
#include "hw_config.h"

/*
 * DEFINES
 ****************************************************************************************
 */
#if     defined(CLK_FCLK_4MHZ)
    /* specify the CPU frequency in [Hz] */
    #define   FDL_SYSTEM_FREQUENCY      (4000000)
    /* programming voltage mode : wide voltage mode */
    #define   FDL_WIDE_VOLTAGE_MODE
#elif   defined(CLK_FCLK_8MHZ)
    /* specify the CPU frequency in [Hz] */
    #define   FDL_SYSTEM_FREQUENCY      (8000000)
    /* programming voltage mode : wide voltage mode */
    #define   FDL_WIDE_VOLTAGE_MODE
#elif   defined(CLK_FCLK_16MHZ)
    #define   FDL_SYSTEM_FREQUENCY      (16000000)
    /* programming voltage mode : Full speed mode */
    /*#define   FDL_WIDE_VOLTAGE_MODE*/
#elif   defined(CLK_FCLK_32MHZ)
    #define   FDL_SYSTEM_FREQUENCY      (32000000)
    /* programming voltage mode : Full speed mode */
    /*#define   FDL_WIDE_VOLTAGE_MODE*/
#else
    #error "an unsupported fCLK configuration."
#endif


/* specify the size of the FDL pool expressed in number of blocks                                 */
#define   FDL_POOL_BLOCKS               (0)

/* specify the size of the EEL pool inside the FDL pool expressed in blocks (min. value is 2)     */
#define   EEL_POOL_BLOCKS               (8)

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
/* external reference to descriptor */
extern    __far const fdl_descriptor_t  fdl_descriptor_str;

#endif // CFG_USE_EEL
#endif
