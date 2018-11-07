/**
 ****************************************************************************************
 *
 * @file iic_slave.h
 *
 * @brief IIC Slave Driver header file.
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _IIC_SLAVE_H_
#define _IIC_SLAVE_H_

/**
 ****************************************************************************************
 * @addtogroup IIC
 * @ingroup DRIVERS
 * @brief IIC Slave driver for communicating APP-MCU.
 *
 * The associated ports used are:
 *  - P60: SCK (Serial ClocK line)
 *  - P61: SDA (Serial DatA  line)
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
 * DEFINITIONS
 *****************************************************************************************
 */
/// iic slave address(0x00-0x7F)
#define IIC_SLAVE_ADDRESS       (0x50)

/// max transferring size(16bits)
#define IIC_MAX_SIZE            (1024U)

/// max tx retry count(8bits)
#define IIC_MAX_TX_TO_COUNT     (4)    /* 0 = Timeout no use */

/// IIC end code
#define IIC_END_CODE            (0xC0)

/*
 * ENUMERATION DEFINITIONS
 *****************************************************************************************
 */
/// IIC interrupt state
enum
{
    IIC_STAT_IDLE      = 0,
    IIC_STAT_RX_GOING     ,
    IIC_STAT_TX_GOING     ,
};

/// error code values
enum
{
    IIC_ERR_NONE       = 0,
    IIC_ERR_NACK          ,
    IIC_ERR_TIMEOUT
};

/*
 * STUCTURE DEFINITIONS
 *****************************************************************************************
 */
/// IIC Information structure
typedef struct {
    uint16_t    size;
    uint16_t    cnt;
    uint8_t     *bufptr;
} IIC_INFO;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for INTIICA0.
 *****************************************************************************************
 */
#ifdef _USE_STD_C99
__IRQ void iica0_isr(void);
#else
void iica0_isr(void);
#endif //_USE_STD_C99

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for INTTM07.
 *****************************************************************************************
 */
#ifdef _USE_STD_C99
__IRQ void inttm07_isr(void);
#else
void inttm07_isr(void);
#endif //_USE_STD_C99

/// @} module

#endif /* _IIC_SLAVE_H_ */
