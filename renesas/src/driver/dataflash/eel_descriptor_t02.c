/*******************************************************************************
* Library       : EEPROM Emulation Library (T02)
*
* File Name     : $Source: eel_descriptor_t02.c $
* Lib. Version  : $RL78_EEL_LIB_VERSION_T02_CCRL: V1.00 $
* Mod. Revision : $Revision: 1.2 $
* Mod. Date     : $Date: 2015/03/12 00:09:39JST $
* Device(s)     : RL78
* Description   : contains the user defined EEL-variable descriptor
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


#ifdef CFG_USE_EEL

/*==============================================================================================*/
/* compiler settings                                                                            */
/*==============================================================================================*/
#ifdef _USE_IAR_RL78
#define _EEL_CNST _Pragma("location=\"EEL_CNST\"")
#else//_USE_IAR_RL78
#define _EEL_CNST

#if !defined(_USE_CCRL_RL78)
#pragma section @@CNSTL   EEL_CNST
#else
#pragma section const     EEL_CNST
#endif
#endif//_USE_IAR_RL78


/*==============================================================================================*/
/* include files list                                                                           */
/*==============================================================================================*/
#include  "arch.h"
#include  "co_bt.h"

#define  __EEL_DESCRIPTOR_C
  #include  "eel_types.h"
  #include  "eel_descriptor_t02.h"
#undef   __EEL_DESCRIPTOR_C

#include  "fdl_types.h"


/*==============================================================================================*/
/* import list                                                                                  */
/*==============================================================================================*/
/* empty */


/*==============================================================================================*/
/* data definition                                                                              */
/*==============================================================================================*/
/* empty */



/*********************************************************************************************************/
/*******                                                                                           *******/
/*******      B E G I N    O F    C U S T O M I Z A B L E    D E C L A R A T I O N    A R E A      *******/
/*******                                                                                           *******/
/*********************************************************************************************************/

_EEL_CNST __far const eel_u08 eel_descriptor[EEL_VAR_NO+2] =
{
  (eel_u08)(EEL_VAR_NO),      /* variable count   */  \
  (eel_u08)(BD_ADDR_LEN),     /* id=1: EEL_ID_BDA */  \
  (eel_u08)(0x00),            /* zero terminator  */  \
};

/*********************************************************************************************************/
/*******                                                                                           *******/
/*******      E N D    O F    C U S T O M I Z A B L E    D E C L A R A T I O N    A R E A          *******/
/*******                                                                                           *******/
/*********************************************************************************************************/




/*********************************************************************************************************/
/*******                                                                                           *******/
/*******      B E G I N    O F    U N T O U C H A B L E     D E C L A R A T I O N    A R E A       *******/
/*******                                                                                           *******/
/*********************************************************************************************************/
_EEL_CNST __far const eel_u08   eel_internal_cfg_cau08[]     = {0x40};
/*********************************************************************************************************/
/*******                                                                                           *******/
/*******      E N D    O F    U N T O U C H A B L E     D E C L A R A T I O N    A R E A           *******/
/*******                                                                                           *******/
/*********************************************************************************************************/

#endif //CFG_USE_EEL
