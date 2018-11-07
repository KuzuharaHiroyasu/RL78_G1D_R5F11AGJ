/*******************************************************************************
* Library       : Data Flash Access Library T02
*
* File Name     : $Source: fdl.h $
* Lib. Version  : $RL78_FDL_LIB_VERSION_T02_CCRL: V1.01 $
* Mod. Revision : $Revision: 1.3 $
* Mod. Date     : $Date: 2015/03/19 16:17:46JST $
* Device(s)     : RL78
* Description   : API definition of the Flash Data Library (data flash only)
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


#ifndef __FDL_H_INCLUDED
#define __FDL_H_INCLUDED

/*================================================================================================*/
/* include files list                                                                             */
/*================================================================================================*/
#include "fdl_types.h"


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Initialization of the Flash Data Library.                                                       */
/*                Several checks are performed during the initialization:                                         */
/*                - plausibility check of the pool configuration                                                  */
/*                - frequency parameter check against supported device-specific range                             */
/*                - initialization of all internal variables                                                      */
/*                - initialization of the flash firmware                                                          */
/*                - configuration of HOCO                                                                         */
/*                After initialization the FDL remains passive.                                                   */
/*                FDL_Open() has to be executed to open access to the FDL pool.                                   */
/*  Input:        &descriptor_pstr far pointer to the descriptor                                                  */
/*  Output:       -                                                                                               */
/*  Return:       status: configuration related error code                                                        */
/*                - FDL_OK:                 initialization done without problems                                  */
/*                - FDL_ERR_CONFIGURATION:  FDL pool-configuration error or HOCO isn't started                    */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  fdl_status_t  __far FDL_Init(const __far fdl_descriptor_t* descriptor_pstr);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Enable data flash access.                                                                       */
/*                Pre-condition: The initialization must be done before however no check is performed here.       */
/*                If FDL is not yet initialized, FDL_Open() has no functionality.                                 */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       -                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  void  __far FDL_Open(void);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Disable data flash access.                                                                      */
/*                Pre-condition: The initialization of the library must "opened" by the function FDL_Open.        */
/*                If FDL is not yet "opened", the FDL_Close() has no functionality.                               */
/*                If any firmware command is just executed, FDL_Close will stop it by FW command first.           */
/*                After that the flash clock is switched OFF.                                                     */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       -                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  void  __far FDL_Close(void);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Initiating execution of the FDL request related to the FDL pool part only.                      */
/*                The corresponding request variable has to be parameterized before.                              */
/*                                                                                                                */
/*                my_request->index_u16        :   byte-index or block-number within FDL-pool                     */
/*                my_request->data_pu08        :   start address of the RAM data that should be read/written      */
/*                my_request->bytecount_u16    :   number of bytes has to be passed in case of byte               */
/*                                                 oriented commands                                              */
/*                my_request->command_enu      :   command code                                                   */
/*                my_request->status_enu       :   request status                                                 */
/*                                                                                                                */
/*  Input:        &my_request_pstr - pointer to FDL request variable                                              */
/*  Output:       status inside the request variable                                                              */
/*  Return:       -                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  void  __far FDL_Execute(__near fdl_request_t* request_pstr);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Continuation and status update of requests being under execution.                               */
/*                When no FDL request is being executed, this function has no functionality (idle).               */
/*  Input:        -                                                                                               */
/*  Output:       Status inside the request variable for command executed via FDL_Execute                         */
/*  Return:       -                                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  void  __far FDL_Handler(void);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      This function aborts immediately a potentially running block command (ERASE) independent        */
/*                of the affected pool.                                                                           */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       status: configuration related error code                                                        */
/*                - FDL_OK:                 Either no command is running or erase has been aborted.               */
/*                - FDL_BUSY:               Byte oriented command is still running.                               */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  fdl_status_t  __far FDL_Abort(void);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      This function stops the clock of the data flash and enters the library into StandBy mode.       */
/*                Note :  It is not allowed to call any FDL function other than FDL_WakeUp when FDL is in         */
/*                        StandBy mode.                                                                           */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       status: configuration related error code                                                        */
/*                - FDL_OK:                 FDL entered StandBy mode.                                             */
/*                - FDL_BUSY:               Any command is still running.                                         */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  fdl_status_t  __far FDL_StandBy(void);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Continuation and status update of requests being under execution.                               */
/*                When no FDL request is being executed, this function has no functionality (idle).               */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       status: configuration related error code                                                        */
/*                - FDL_OK:                 FDL is up and running.                                                */
/*                - FDL_ERR_REJECTED:       WakeUp cannot be performed.                                           */
/*                                          REASON: Library isn't in StandBy mode.                                */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  fdl_status_t  __far FDL_WakeUp(void);


/* ---------------------------------------------------------------------------------------------------------------*/
/*  Block type:   standard function                                                                               */
/* ---------------------------------------------------------------------------------------------------------------*/
/*  Purpose:      Returns the start address of the library version string                                         */
/*                                                                                                                */
/*  Input:        -                                                                                               */
/*  Output:       -                                                                                               */
/*  Return:       starting address of the zero-terminated version string                                          */
/* ---------------------------------------------------------------------------------------------------------------*/
extern  __far fdl_u08* __far FDL_GetVersionString(void);


#endif
