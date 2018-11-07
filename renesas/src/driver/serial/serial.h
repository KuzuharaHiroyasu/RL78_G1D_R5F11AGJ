/**
 ****************************************************************************************
 *
 * @file serial.h
 *
 * @brief SERIAL Driver - for use UART0 or CSI00 of RL78 RENESAS 16bit MCU.
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.205
 *
 ****************************************************************************************
 */

#ifndef _SERIAL_H_
#define _SERIAL_H_

/**
 ****************************************************************************************
 * @defgroup SERIAL SERIAL
 * @ingroup DRIVERS
 * @brief Serial Communication MCU RL78 driver.
 *
 * This is actually a Wrapper Functions For UART or CSI driver.
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

/*
 * DEFINES
 ****************************************************************************************
 */
#define SERIAL_U_2WIRE      (1)
#define SERIAL_U_3WIRE      (0)
#define SERIAL_U_DIV_2WIRE  (0)
#define SERIAL_C_4WIRE      (0)
#define SERIAL_C_5WIRE      (0)
#define SERIAL_I_3WIRE      (0)

/*
 * STUCTURE DEFINITIONS
 *****************************************************************************************
 */

typedef void (* SERIAL_EVENT_VOID_HANDLER)( void );
typedef bool (* SERIAL_EVENT_BOOL_HANDLER)( void );
typedef struct {
	SERIAL_EVENT_VOID_HANDLER rx_callback;
	SERIAL_EVENT_VOID_HANDLER tx_callback;
	SERIAL_EVENT_VOID_HANDLER err_callback; 
	SERIAL_EVENT_BOOL_HANDLER rx_first_byte_callback;
	SERIAL_EVENT_BOOL_HANDLER rx_cmp_callback;
}SERIAL_EVENT_PARAM;

/*
 * MACRO DECLARATIONS
 ****************************************************************************************
 */
#define SERIAL_CALLBACK_VOID(cb)   if((cb) != NULL) ((SERIAL_EVENT_VOID_HANDLER)(cb))()
#define SERIAL_CALLBACK_BOOL(cb)   ((cb)!=NULL)?(((SERIAL_EVENT_BOOL_HANDLER)(cb))()):(true)


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initialize Serial communication.(including initializing DMA and interrupt)
 *****************************************************************************************
 */
extern void serial_init( SERIAL_EVENT_PARAM *param );

/**
 ****************************************************************************************
 * @brief Start Serial receiving data.
 *        When finished receiving data, calls callback function.
 *
 * @param[in,out] bufptr  destination pointer, start of the space where data is transfered to.
 * @param[in] size        Transfer size, received data size byte by Serial.
 *****************************************************************************************
 */
extern void serial_read(uint8_t *bufptr, const uint16_t size);

/**
 ****************************************************************************************
 * @brief Start Serial transferring data.
 *        When finished transferring data, calls callback function.
 *
 * @param[in] bufptr  source pointer, start of the space where data is transfered from.
 * @param[in] size    Transfer size, transferred data size byte by Serial.
 *****************************************************************************************
 */
extern void serial_write(const uint8_t *bufptr, const uint16_t size);

/**
 ****************************************************************************************
 * @brief Get ready to enter the MCU stop mode.
 *
 * @return bool     true if success to ready, or false if failure to ready
 *****************************************************************************************
 */
extern bool serial_stop_ready(void);

/**
 ****************************************************************************************
 * @brief Return from the MCU stop mode.
 *****************************************************************************************
 */
extern void serial_stop_finish(void);

#if SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE || SERIAL_C_5WIRE
/**
 ****************************************************************************************
 * @brief Inform the Wakeup Response.
 *****************************************************************************************
 */
extern void serial_wakeup_response(void);
#endif


/// @} module

#endif /* _SERIAL_H_ */
