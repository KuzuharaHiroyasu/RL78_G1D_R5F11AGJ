/**
 ****************************************************************************************
 *
 * @file		rscip_api.h
 *
 * @brief	RSCIP control module header file.
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * $Rev: 2301 $
 *
 ****************************************************************************************
 */

#ifndef RSCIP_API_H_
#define RSCIP_API_H_

#include "rble.h"

/*
 * INCLUDE FILES
 ****************************************************************************************
 */


/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * MACROS
 ****************************************************************************************
 */


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

void codeptr RSCIP_Tx_Done(void);
void codeptr RSCIP_Rx_Done(void);

void RSCIP_Uart_Tx_Done(void);
void RSCIP_Uart_Rx_Done(void);
void RSCIP_Uart_Rx_Error(void);
BOOL RSCIP_Uart_Rx_FirstByte(void);
BOOL RSCIP_Uart_Rx_Complete(void);

BOOL RSCIP_Can_Sleep(void);
void RSCIP_Wakeup(void);

#endif	//	RSCIP_API_H_
