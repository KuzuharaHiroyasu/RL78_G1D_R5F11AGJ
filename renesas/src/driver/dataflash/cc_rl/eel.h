/*******************************************************************************
* Library       : EEPROM Emulation Library (T02)
*
* File Name     : $Source: eel.h $
* Lib. Version  : $RL78_EEL_LIB_VERSION_T02_CCRL: V1.00 $
* Mod. Revision : $Revision: 1.2 $
* Mod. Date     : $Date: 2015/03/12 00:09:40JST $
* Device(s)     : RL78
* Description   : API definition of the EEPROM Emulation Library T02 (EEL)
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
* Copyright (C) 2015 Renesas Electronics Corporation. All rights reserved.
*******************************************************************************/


#ifndef __EEL_H_INCLUDED
#define __EEL_H_INCLUDED

/* Include type definitions */
#include  "eel_types.h"


/*******************************************************************************
 * Function name:  EEL_Init
 ******************************************************************************/
/**
 * Initialization of the EEPROM Emulation Library (EEL).
 *
 * @return         eel_status_t - Initialization status
 */
/******************************************************************************/
extern eel_status_t __far EEL_Init(void);



/*******************************************************************************
 * Function name:  EEL_Open
 ******************************************************************************/
/**
 * Activate the EEL environment.
 */
/******************************************************************************/
extern void __far EEL_Open(void);



/*******************************************************************************
 * Function name:  EEL_Close
 ******************************************************************************/
/**
 * Deactivate the EEL environment.
 *
 */
/******************************************************************************/
extern void __far EEL_Close(void);



/*******************************************************************************
 * Function name:  EEL_Execute
 ******************************************************************************/
/**
 * Starting of execution of the corresponding EEL request.
 *
 * @param[in,out]   request_pstr - Pointer to the EEL request variable
 */
/******************************************************************************/
extern void __far EEL_Execute(__near eel_request_t* request_pstr);



/*******************************************************************************
 * Function name:  EEL_Handler
 ******************************************************************************/
/**
 * This function takes care for the execution of the running command
 *
 */
/******************************************************************************/
extern void __far EEL_Handler(void);



/*******************************************************************************
 * Function name:  EEL_GetDriverStatus
 ******************************************************************************/
/**
 * This function provides the internal status of the EEL.
 *
 * @param[in,out]   eel_driver_status_penu - modified data
 */
/******************************************************************************/
extern void __far EEL_GetDriverStatus(__near eel_driver_status_t* eel_driver_status_penu);



/*******************************************************************************
 * Function name:  EEL_GetSpace
 ******************************************************************************/
/**
 * Provides the currently available space inside the active block.
 *
 * @param[in,out]   __near eel_u16* - address of the space variable
 * @return          eel_status_t    - Return status of the function.
 */
/******************************************************************************/
extern eel_status_t __far EEL_GetSpace(__near eel_u16* space_pu16);



/*******************************************************************************
 * Function name:  EEL_GetVersionString
 ******************************************************************************/
/**
 * This function returns the pointer to the "zero-terminated" version string.
 *
 * @return          __far  eel_u08* - address of the version string
 */
/******************************************************************************/
extern __far eel_u08* __far EEL_GetVersionString(void);


#endif
