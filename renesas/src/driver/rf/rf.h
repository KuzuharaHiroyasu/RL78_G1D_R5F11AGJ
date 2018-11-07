/**
 ****************************************************************************************
 *
 * @file        rf.h
 *
 * @brief       Common header file for all radios.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.205
 *
 ****************************************************************************************
 */

#ifndef RF_H_
#define RF_H_

/**
 ****************************************************************************************
 * @addtogroup RF
 * @ingroup PLATFORM_DRIVERS
 * @brief Common definitions for radio modules.
 *
 * This module declares the functions and constants that have to be defined for all RF.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */
#define RF_EXPA_OFF      (0x0000)   /* not use external PowerAmplifier           */
#define RF_EXPA_ON       (0x0001)   /*     use external PowerAmplifier           */

#define RF_DCDC_ON       (0x0000)   /* use internal DCDC converter               */
#define RF_DCDC_OFF      (0x0002)   /* use external DCDC converter               */

#define RF_INT_32KHZ     (0x0000)   /* use internal 32KHz oscilator              */
#define RF_EXT_32KHZ     (0x0020)   /* use external 32KHz clock                  */
#define RF_EXT_16KHZ     (0x0040)   /* use external 16KHz clock                  */

#define RF_SLK_MASK      (0x0070)   /* bit mask for Sleep Clock                  */

#define RF_CLK_NONE      (0x0000)   /* not output HighSpeed Clock                */
#define RF_CLK_16MHZ     (0x0300)   /*     output HighSpeed Clock 16MHz          */
#define RF_CLK_8MHZ      (0x0400)   /*     output HighSpeed Clock  8MHz          */
#define RF_CLK_4MHZ      (0x0500)   /*     output HighSpeed Clock  4MHz          */
#define RF_CLK_2MHZ      (0x0600)   /*     output HighSpeed Clock  2MHz          */
#define RF_CLK_1MHZ      (0x0700)   /*     output HighSpeed Clock  1MHz          */

#define RF_CLK_MASK      (0x0700)   /* bit mask for HighSpeed Clock              */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 *****************************************************************************************
 * @brief Initialization of RF.
 *
 * This function initializes the RF and fills the structure.
 *
 * @param[in]   rf_flg  indicate initialization setting
 *
 *****************************************************************************************
 */
void rf_init(const uint16_t rf_flg);

/// @}

#endif // RF_H_
