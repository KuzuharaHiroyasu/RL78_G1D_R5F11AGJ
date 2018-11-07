/**
 ****************************************************************************************
 *
 * @file    hw_config.h
 *
 * @brief   configuration file for hardware
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _HW_CONFIG_H_
#define _HW_CONFIG_H_


/*
 * MCU INITIALIZATION SETTING
 ****************************************************************************************
 */
// Main Sysytem Clock
#if defined(CLK_HOCO_4MHZ)  || \
    defined(CLK_HOCO_8MHZ)  || \
    defined(CLK_HOCO_16MHZ) || \
    defined(CLK_HOCO_32MHZ)

#define CLK_HOCO

#elif defined(CLK_X1_4MHZ) || \
      defined(CLK_X1_8MHZ)

#define CLK_X1_LOW

#elif defined(CLK_X1_16MHZ)

#define CLK_X1_HIGH

#elif defined(CLK_EX_RF_4MHZ)  || \
      defined(CLK_EX_RF_8MHZ)  || \
      defined(CLK_EX_RF_16MHZ) || \
      defined(CLK_EX_4MHZ)     || \
      defined(CLK_EX_8MHZ)     || \
      defined(CLK_EX_16MHZ)

#define CLK_EX

#else
#error "an unsupported main system clock configuration."
#endif

// Peripheral Hardware Frequency 
#if defined(CLK_HOCO_4MHZ)  || \
    defined(CLK_EX_RF_4MHZ) || \
    defined(CLK_EX_4MHZ)    || \
    defined(CLK_X1_4MHZ)

#define CLK_FCLK_4MHZ

#elif defined(CLK_HOCO_8MHZ)  || \
      defined(CLK_EX_RF_8MHZ) || \
      defined(CLK_EX_8MHZ)    || \
      defined(CLK_X1_8MHZ)

#define CLK_FCLK_8MHZ

#elif defined(CLK_HOCO_16MHZ)  || \
      defined(CLK_EX_RF_16MHZ) || \
      defined(CLK_EX_16MHZ)    || \
      defined(CLK_X1_16MHZ)

#define CLK_FCLK_16MHZ

#elif defined(CLK_HOCO_32MHZ)

#define CLK_FCLK_32MHZ

#else
#error "an unsupported fCLK configuration."
#endif

// Sub System Clock
#if defined(CLK_SUB_XT1)

#define CLKS_XT1

#elif defined(CLK_SUB_EX_OT)

#define CLKS_EX

#else
/* CFG_CLKS_DISUSE */
#endif

/*
 * RF INITIALIZATION SETTING
 ****************************************************************************************
 */
/* < Sleep Clock source setting >                                                      */
/*  The setting value is the RF_INT_* or RF_EXT_* macro defined by rf.h.               */

#if defined(CLK_SUB_XT1)
#define RF_SLOW_CLK_SET     RF_EXT_16KHZ
#define CFG_PLF_INIT        (PLF_PCLBUZ_16KHZ)
#define CFG_SCA_SET         SCA_50PPM
#elif defined(CLK_SUB_EX_OT)
#define RF_SLOW_CLK_SET     RF_INT_32KHZ
#define CFG_PLF_INIT        (PLF_PCLBUZ_NONE)
#define CFG_SCA_SET         SCA_250PPM
#else
#define RF_SLOW_CLK_SET     RF_INT_32KHZ
#define CFG_PLF_INIT        (PLF_PCLBUZ_NONE)
#define CFG_SCA_SET         SCA_250PPM
#endif

/* < HighSpeed Clock speed output setting >                                            */
/*  The setting value is the macro defined by rf.h.                                    */
#if defined(CLK_EX_RF_4MHZ)
#define RF_CLK_OUTPUT_SET   RF_CLK_4MHZ
#elif defined(CLK_EX_RF_8MHZ)
#define RF_CLK_OUTPUT_SET   RF_CLK_8MHZ
#elif defined(CLK_EX_RF_16MHZ)
#define RF_CLK_OUTPUT_SET   RF_CLK_16MHZ
#else
#define RF_CLK_OUTPUT_SET   RF_CLK_NONE
#endif

/*
 * RF INITIALIZATION SETTING
 ****************************************************************************************
 */
/* < External PowerAmplifier setting >                                                 */
/*  The setting value is the RF_EXPA_* macro defined by rf.h.                          */
/* < DCDC converter setting >                                                          */
/*  The setting value is the RF_DCDC_* macro defined by rf.h.                          */
/* < Sleep Clock source setting >                                                      */
/*  The setting value is the RF_INT_* or RF_EXT_* macro defined by rf.h.               */
/* < HighSpeed Clock speed output setting >                                            */
/*  The setting value is the macro defined by rf.h.                                    */
#define CFG_RF_INIT         (RF_EXPA_OFF | RF_DCDC_ON | RF_SLOW_CLK_SET | RF_CLK_OUTPUT_SET)

/*
 * SCA SETTING
 ****************************************************************************************
 */
/* < SCA(Sleep Clock Accuracy) setting >                                               */
/*  The setting value is SCA_* macro defined by co_bt.h.(except SCA_END)               */
#define CFG_SCA             (CFG_SCA_SET)

/*
 * PERIODIC LOCO CALIBRATION SETTING
 ****************************************************************************************
 */
/* < Sleep Duration Max Value for LOCO CALIBRATION >                                   */
/*  The setting value is unit 625usec and range 0x320 to 0x3E80 (500msec to 10sec).    */
/*  If out of range, periodic LOCO CAL disable.                                        */
#define CFG_LOCO_CAL_PERIOD (0x0640U)

#endif /* _HW_CONFIG_H_ */

