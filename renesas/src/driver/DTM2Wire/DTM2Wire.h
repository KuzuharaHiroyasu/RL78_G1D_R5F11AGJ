/**
 ****************************************************************************************
 *
 * @file		DTM2Wire.h
 *
 * @brief Direct Test Mode 2Wire UART Driver.
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: v1.01.00
 *
 ****************************************************************************************
 */

#ifndef	__DTM2WIRE_INC__
#define	__DTM2WIRE_INC__

#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */
#include	"ke_task.h"
#endif

/**************************** Define Declaration **************************************/
typedef enum RBLE_MODE_eum {
	_RBLE_RUN_MODE_INIT,
	_RBLE_RUN_MODE_DIRECT,
	_RBLE_RUN_MODE_NORMAL
} RBLE_RUNNING_MODE;

/* Tas Infomation */
#define DTM2RBLE_STATE_MAX		2	// Max State Num
#define DTM2RBLE_IDX_MAX		1	// Max ID Num

#define DTM2RBLE_READ_COMPLETE	1	/* Task API ID */


/**************************** Structure Declaration ***********************************/
/**************************** PROTOTYPE Declaration ***********************************/
void DTM2Wire_Init( void );
bool DTM2Wire_Check( void );
bool DTM2Wire_Start( void );
void DTM2Wire_Read_Complete( void );
void DTM2Wire_Write_Complete( void );
void DTM2Wire_Rx_Error( void );

/**************************** Public Memory Declaration ***********************************/
extern volatile RBLE_RUNNING_MODE	rBLE_Run_Mode_Flg;
/* Status Handler */
extern const struct ke_state_handler MDT2Wire_state_handler[ DTM2RBLE_STATE_MAX ];
/* Default Handler */
extern const struct ke_state_handler MDT2Wire_default_handler;
/* Status */
extern ke_state_t DTM2Wire_State[ DTM2RBLE_IDX_MAX ];

#endif // __DTM2WIRE_INC__
