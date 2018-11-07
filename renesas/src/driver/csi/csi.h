/**
 ****************************************************************************************
 *
 * @file csi.h
 *
 * @brief CSI Driver header file.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * $Rev: 19805 $
 *
 * rev.205
 *
 ****************************************************************************************
 */

#ifndef _CSI_H_
#define _CSI_H_

/**
 ****************************************************************************************
 * @addtogroup CSI
 * @ingroup DRIVERS
 * @brief CSI driver for communicating APP-MCU.
 *
 * The associated ports used are:
 *  - P10: SCK00
 *  - P11: SI00 (MOSI)
 *  - P12: SO00 (MISO)
 *  - P23: used as Request Signal Output to the Master.
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#ifdef _USE_STD_C99
 #include <stdint.h>
 #include <stdbool.h>
#endif //_USE_STD_C99
#include "serial.h"

/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */
/// status values
enum
{
    UART_STATUS_OK,
    UART_STATUS_ERROR
};


///DMA transfer mode flag - DSTn
enum
{
    DMA_TF_COMPLETE = 0,
    DMA_TF_ONGOING
};

///DMA Transfer start SW trigger - STGn
enum
{
    DMA_SW_NO_TRIG = 0,
    DMA_SW_DEN_TRIG
};


///DMA Transfer direction  - DRSn
enum
{
    DMA_SFR_TO_RAM = 0,
    DMA_RAM_TO_SFR,
    DMA_TF_MAX
};


///DMA transfer data unit size - DSn
enum
{
    DMA_UNIT_8B = 0,
    DMA_UNIT_16B
};


///DMA pending transfer - DWAIT
enum
{
    DMA_TF_NO_PEND = 0,
    DMA_TF_HOLD
};


///DMA start sources - IFC[3:0]
enum
{
    DMA_SRC_INT_SW  = 0x0,
    DMA_SRC_INTTM00 = 0x2,
    DMA_SRC_INTTM01 = 0x3,
    DMA_SRC_INTTM04 = 0x4,
    DMA_SRC_INTTM05 = 0x5,
    DMA_SRC_INTST0  = 0x6,
    DMA_SRC_INTSR0  = 0x7,
    DMA_SRC_INTST1  = 0x8,
    DMA_SRC_INTSR1  = 0x9,
    DMA_SRC_INTST3  = 0xA,
    DMA_SRC_INTSR3  = 0xB,
    DMA_SRC_INTAD   = 0xC,
    DMA_SRC_PROHIB
};


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initializes the port bits, port mode register and pull-ups or latches for CSI
 *        pins.
 *****************************************************************************************
 */
static void csi_port0_init(void);

/**
 ****************************************************************************************
 * @brief Initialize a DMA0 channel for transmission from CSI data register to RAM.
 *****************************************************************************************
 */
static void dma0_init(void);

/**
 ****************************************************************************************
 * @brief Initialize a DMA1 channel for transmission from RAM to CSI data register.
 *****************************************************************************************
 */
static void dma1_init(void);

/**
 ****************************************************************************************
 * @brief stop CSI when rx_error was happened.
 *****************************************************************************************
 */
static void csi_rx_error(void);

/**
 ****************************************************************************************
 * @brief DMA channel 0 transfer done Interrupt Service Routine.
 *
 * DMA0 is used for RX on CSI, so this routine lets the upper stack know that
 * the receiving is done
 ****************************************************************************************
 */
#ifdef _USE_STD_C99
__IRQ void dma0_isr(void);
#else
void dma0_isr(void);
#endif //_USE_STD_C99

/**
 ****************************************************************************************
 * @brief DMA channel 1 transfer done Interrupt Service Routine.
 *
 * DMA1 is used for TX on CSI, so this routine lets the upper stack know that
 * the transfer is done
 ****************************************************************************************
 */
#ifdef _USE_STD_C99
__IRQ void dma1_isr(void);
#else
void dma1_isr(void);
#endif //_USE_STD_C99

/**
 ****************************************************************************************
 * @brief CSI transmit finished Interrupt Service Routine.
 ****************************************************************************************
 */
#ifdef _USE_STD_C99
__IRQ void csi00_isr(void);
#else
void csi00_isr(void);
#endif //_USE_STD_C99

/// @} module

#endif /* _CSI_H_ */
