/**
 ****************************************************************************************
 *
 * @file        port.h
 *
 * @brief       Port setting utilities.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.205
 *
 ****************************************************************************************
 */

#ifndef PORT_H_
#define PORT_H_

/**
 ****************************************************************************************
 * @addtogroup PORT PORT
 * @ingroup DRIVERS
 * @brief Port settings definitions common for several interface drivers.
 *
 * Output, input mode definitions. Latch and Pull-up values.etc.
 *
 * @{
 ****************************************************************************************
 */


/*
 * DEFINES
 ****************************************************************************************
 */

///Define values for Port pull up setting
#define PORT_PULLUP 1

///Define values for Port latch setting
#define PORT_LATCH  1

///Interrupt flag clear
#define IRQ_CLEAR   0


///Port output mode
enum
{
    PORT_OUTPUT_NORMAL = 0,
    PORT_OUTPUT_NCH
};

///Port input mode
enum
{
    PORT_INPUT_NORMAL = 0,
    PORT_INPUT_TTL
};

///Port direction
enum
{
    PORT_OUTPUT = 0,
    PORT_INPUT
};


///Interrupt enable using mask(mask = 0 enable)
enum
{
    IRQ_ENABLE= 0,
    IRQ_DISABLE
};


///Port output data control
enum
{
    PORT_OUTPUT_0 = 0,
    PORT_OUTPUT_1
};


/// @} module

#endif // PORT_H_
