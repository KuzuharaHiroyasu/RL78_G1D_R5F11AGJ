/**
 ****************************************************************************************
 *
 * @file		DTM2Wire.c
 *
 * @brief Direct Test Mode 2Wire UART Driver.
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwble_config.h"
#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */
#include	"ke_task.h"
#endif

#include	"rble.h"
#include	"rble_api.h"
#include	"uart.h"
#include	"DTM2Wire.h"
#include	"port.h"

#if	__DTM2WIRE_UART_USE__

/**************************** Define Declaration **************************************/
#define		DTM2WIRE_MAX_BUF_CNT			2
#define		DTM2WIRE_STATUS_OK				0
#define		DTM2WIRE_STATUS_ERR				1
#define		DTM2WIRE_STATUS_EVENT_BIT		0x0000
#define		DTM2WIRE_REPORT_EVENT_BIT		0x8000

#define		DTM2WIRE_CMD_RESET				0x0000
#define		DTM2WIRE_CMD_RX_START			0x4000
#define		DTM2WIRE_CMD_TX_START			0x8000
#define		DTM2WIRE_CMD_END				0xC000
#define		DTM2WIRE_CMD_MASK				0xC000
#define		DTM2WIRE_FREQ_MASK				0x3f00
#define		DTM2WIRE_LENGTH_MASK			0x00fc
#define		DTM2WIRE_PKT_TYPE_MASK			0x0003
#define		DTM2WIRE_GET_FREQ( Cmd )		( uint8_t )( ( Cmd & DTM2WIRE_FREQ_MASK ) >> 8 )
#define		DTM2WIRE_GET_LENGTH( Cmd )		( uint8_t )( ( Cmd & DTM2WIRE_LENGTH_MASK ) >> 2 )
#define		DTM2WIRE_GET_PKT_TYPE( Cmd )	( uint8_t )( ( Cmd & DTM2WIRE_PKT_TYPE_MASK ) >> 0 )

//	SLIP CODE
#define END_CODE		0xC0

typedef enum DTM2WIRE_INIT_STATE_Enum {
	_DTM2WIRE_INIT_STATE_START,
	_DTM2WIRE_INIT_STATE_RESET,
	_DTM2WIRE_INIT_STATE_COMPLETE,
} DTM2WIRE_INIT_STATE;

/******************************* PRIVATE PROTOTYPE ************************************/
static int_t DTM2Wire_Alalize_Evt(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static void DTM2Wire_Alalize( uint16_t Cmd );
static void RBLE_APP_CallBack(RBLE_MODE mode );
static void RBLE_APP_GAP_CallBack( RBLE_GAP_EVENT *event );									/* GAP Call Back */
static void RBLE_APP_SM_CallBack( RBLE_SM_EVENT *event );									/* SM Call Back */
static void RBLE_APP_VS_CallBack( RBLE_VS_EVENT *event );									/* Vendor Specific Call Back */
static void DTM2Wire_Error( void );
static void DTM2Wire_bswap_serial_write( uint16_t event );

/**************************** PRIVATE Memory Declaration ******************************/
static const struct ke_msg_handler DTM2Wire_handler[] =
{
	{ DTM2RBLE_READ_COMPLETE, (ke_msg_func_t)DTM2Wire_Alalize_Evt },
};

/**************************** PUBLIC Memory Declaration *******************************/
DTM2WIRE_INIT_STATE			DTM2Wire_Init_Flg;
uint8_t						DTM2Wire_Data_Wr_Point;
uint8_t						DTM2Wire_Data_Rd_Point;
uint16_t					DTM2Wire_Data[ DTM2WIRE_MAX_BUF_CNT ];

/* Status Handler */
const struct ke_state_handler MDT2Wire_state_handler[ DTM2RBLE_STATE_MAX ] =
{   /* State message handlers */
    KE_STATE_HANDLER(DTM2Wire_handler)
};

/* Default Handler */
const struct ke_state_handler MDT2Wire_default_handler = KE_STATE_HANDLER_NONE;

/* Status */
ke_state_t DTM2Wire_State[ DTM2RBLE_IDX_MAX ] = {0};

/******************************* Program Area *****************************************/
void DTM2Wire_Init( void )
{
#ifndef CONFIG_EMBEDDED
	SERIAL_EVENT_PARAM call_back = {0};
#endif

	rBLE_Run_Mode_Flg = _RBLE_RUN_MODE_INIT;

	DTM2Wire_Data_Wr_Point = 0;
	DTM2Wire_Data_Rd_Point = 0;
#ifndef CONFIG_EMBEDDED
	
	call_back.rx_callback = &DTM2Wire_Read_Complete;
	call_back.tx_callback = &DTM2Wire_Write_Complete;
	call_back.err_callback = &DTM2Wire_Rx_Error;

	serial_init( &call_back );

	serial_read( ( uint8_t * )&DTM2Wire_Data[ 0 ], sizeof( uint8_t ) );
#endif
}

bool DTM2Wire_Check( void )
{
	bool	Ret;

#ifdef CONFIG_EMBEDDED
#if	1
	/* SW2 CHECK */
	write1_sfr( PM1, 6, PORT_INPUT );
	write1_sfr( PU1, 6, PORT_PULLUP );

    __no_operation();
    __no_operation();
    __no_operation();
	if ( 0 != read1_sfr( P1, 6 ) ) {
		Ret = true;
	} else {
		Ret = false;
	}
#else
	/* VBUS CHECK */
	write1_sfr( PM3, 0, PORT_INPUT );
	write1_sfr( PU3, 0, PORT_PULLUP );

    __no_operation();
    __no_operation();
    __no_operation();
	if ( 0 != read1_sfr( P3, 0 ) ) {
		Ret = true;
	} else {
		Ret = false;
	}
#endif
#else
#if 0 // Note: Enabling this code when performing mode switching at the same baudrate.
	if ( END_CODE == ( DTM2Wire_Data[ 0 ] & 0xff ) ) {
		Ret = true;
	} else 
#endif
	{
		serial_read( ( ( ( uint8_t * )&DTM2Wire_Data[ 0 ] ) + 1 ), sizeof( uint8_t ) );
		Ret = false;
	}
#endif
	return( Ret );

}

bool DTM2Wire_Start( void )
{
	bool	Ret;

#ifdef CONFIG_EMBEDDED
	SERIAL_EVENT_PARAM call_back = {0};
	call_back.rx_callback = &DTM2Wire_Read_Complete;
	call_back.tx_callback = &DTM2Wire_Write_Complete;
	call_back.err_callback = &DTM2Wire_Rx_Error;
	serial_init(&call_back);
	serial_read( ( uint8_t * )&DTM2Wire_Data[ 0 ], sizeof( uint16_t ) );
#endif
	DTM2Wire_Init_Flg = _DTM2WIRE_INIT_STATE_START;
	/* Initialize rBLE */
	if( RBLE_OK == RBLE_Init( &RBLE_APP_CallBack ) ) {
		Ret = true;
	} else {
		Ret = false;
	}
	return( Ret );
}

void DTM2Wire_Read_Complete( void )
{
	uint8_t *ke_msg;

	DTM2Wire_Data_Wr_Point++;
	serial_read( ( uint8_t * )&DTM2Wire_Data[ ( DTM2Wire_Data_Wr_Point & ( DTM2WIRE_MAX_BUF_CNT - 1 ) ) ], sizeof( uint16_t ) );

	if ( _DTM2WIRE_INIT_STATE_COMPLETE == DTM2Wire_Init_Flg ) {
	    ke_msg = ke_msg_alloc( DTM2RBLE_READ_COMPLETE, TASK_USR_0, TASK_USR_0, 0 );
    	ke_msg_send(ke_msg);
	}
}

void DTM2Wire_Write_Complete( void )
{
}

void DTM2Wire_Rx_Error( void )
{
	if ( _RBLE_RUN_MODE_INIT == rBLE_Run_Mode_Flg ) {
		rBLE_Run_Mode_Flg = _RBLE_RUN_MODE_NORMAL;
	} else {
		serial_read( ( uint8_t * )&DTM2Wire_Data[ ( DTM2Wire_Data_Wr_Point & ( DTM2WIRE_MAX_BUF_CNT - 1 ) ) ], sizeof( uint16_t ) );
	}
}

static int_t DTM2Wire_Alalize_Evt(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	DTM2Wire_Alalize( DTM2Wire_Data[ ( DTM2Wire_Data_Rd_Point & ( DTM2WIRE_MAX_BUF_CNT - 1 ) ) ] );
	return (KE_MSG_CONSUMED);
}

static void DTM2Wire_Alalize( uint16_t Cmd )
{
    static uint16_t val16;

	DTM2Wire_Data_Rd_Point++;

	val16 = ( ( ( Cmd << 8 ) & 0xFF00 ) | ( ( Cmd >> 8 ) & 0x00FF ) );
	
	switch( val16 & DTM2WIRE_CMD_MASK ) {
		case DTM2WIRE_CMD_RESET:
			if ( RBLE_OK != RBLE_GAP_Reset( &RBLE_APP_GAP_CallBack, &RBLE_APP_SM_CallBack ) ) {
				/* Error */
				DTM2Wire_Error();
			}
			break;
		case DTM2WIRE_CMD_RX_START:
			if ( RBLE_OK != RBLE_VS_Test_Rx_Start( DTM2WIRE_GET_FREQ( val16 ) ) ) {
				/* Error */
				DTM2Wire_Error();
			}
			break;
		case DTM2WIRE_CMD_TX_START:
			if ( RBLE_OK != RBLE_VS_Test_Tx_Start( DTM2WIRE_GET_FREQ( val16 ), DTM2WIRE_GET_LENGTH( val16 ), DTM2WIRE_GET_PKT_TYPE( val16 ) ) ) {
				/* Error */
				DTM2Wire_Error();
			}
			break;
		case DTM2WIRE_CMD_END:
			if ( RBLE_OK != RBLE_VS_Test_End() ) {
				/* Error */
				DTM2Wire_Error();
			}
			break;
	}
}

/******************************************************************************************/
/**
 *  @brief rBLE APP Call Back
 *
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] mode      RBLE Condition
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_CallBack( RBLE_MODE Mode )
{
	switch( Mode ) {
		case RBLE_MODE_INITIALIZE:
			break;
		case RBLE_MODE_ACTIVE:
			DTM2Wire_Init_Flg = _DTM2WIRE_INIT_STATE_RESET;
			if ( RBLE_OK != RBLE_GAP_Reset( &RBLE_APP_GAP_CallBack, &RBLE_APP_SM_CallBack ) ) {
				/* Error */
				DTM2Wire_Error();
			} else if ( RBLE_OK != RBLE_VS_Enable( RBLE_APP_VS_CallBack ) ) {
				/* Error */
				DTM2Wire_Error();
			}
			break;
		case RBLE_MODE_RESET:
			break;
		case RBLE_MODE_SLEEP:
			break;
		case RBLE_MODE_ERROR:
			break;
		default:
			break;
	}
}

/******************************************************************************************/
/**
 *  @brief rBLE GAP Call Back
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event          event parameter
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_GAP_CallBack( RBLE_GAP_EVENT *event )
{
	uint16_t	DTM2Wire_Event;
	
	switch( event->type ) {
		case RBLE_GAP_EVENT_RESET_RESULT:
			if ( _DTM2WIRE_INIT_STATE_RESET == DTM2Wire_Init_Flg ) {
				DTM2Wire_Init_Flg = _DTM2WIRE_INIT_STATE_COMPLETE;
				if ( DTM2Wire_Data_Wr_Point != DTM2Wire_Data_Rd_Point ) {
					DTM2Wire_Alalize( DTM2Wire_Data[ ( DTM2Wire_Data_Rd_Point & ~( DTM2WIRE_MAX_BUF_CNT - 1 ) ) ] );
				}
			} else {
				DTM2Wire_Event = DTM2WIRE_STATUS_EVENT_BIT;
				if (( RBLE_OK == event->param.reset_result.status ) || ( RBLE_TEST_VERSION == event->param.reset_result.status )) {
					DTM2Wire_Event |= DTM2WIRE_STATUS_OK;
				} else {
					DTM2Wire_Event |= DTM2WIRE_STATUS_ERR;
				}
				DTM2Wire_bswap_serial_write( DTM2Wire_Event );
			}
			break;
		default:
			break;
	}
}

/* --------------------------- Security Manager -------------------------------------*/
/******************************************************************************************/
/**
 *  @brief rBLE SM Call Back
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event          event parameter
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_SM_CallBack( RBLE_SM_EVENT *event )
{
}

/* ------------------------- Vendor Specific-------------------------------*/
/******************************************************************************************/
/**
 *  @brief Vendor Specific Call Back
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in] *event          event parameter
 *
 *  @retval
 */
/******************************************************************************************/
static void RBLE_APP_VS_CallBack( RBLE_VS_EVENT *event )
{
	uint16_t	DTM2Wire_Event;

	switch( event->type){
		case RBLE_VS_EVENT_TEST_RX_START_COMP:
			DTM2Wire_Event = DTM2WIRE_STATUS_EVENT_BIT;
			if ( RBLE_OK == event->param.status ) {
				DTM2Wire_Event |= DTM2WIRE_STATUS_OK;
			} else {
				DTM2Wire_Event |= DTM2WIRE_STATUS_ERR;
			}
			DTM2Wire_bswap_serial_write( DTM2Wire_Event );
			break;
		case RBLE_VS_EVENT_TEST_TX_START_COMP:
			DTM2Wire_Event = DTM2WIRE_STATUS_EVENT_BIT;
			if ( RBLE_OK == event->param.status ) {
				DTM2Wire_Event |= DTM2WIRE_STATUS_OK;
			} else {
				DTM2Wire_Event |= DTM2WIRE_STATUS_ERR;
			}
			DTM2Wire_bswap_serial_write( DTM2Wire_Event );
			break;
		case RBLE_VS_EVENT_TEST_END_COMP:
			if ( RBLE_OK == event->param.status ) {
				DTM2Wire_Event = DTM2WIRE_REPORT_EVENT_BIT;
				DTM2Wire_Event |= event->param.test_end_cmp.nb_packet_received;
			} else {
				DTM2Wire_Event = DTM2WIRE_STATUS_EVENT_BIT;
				DTM2Wire_Event |= DTM2WIRE_STATUS_ERR;
			}
			DTM2Wire_bswap_serial_write( DTM2Wire_Event );
			break;
		case RBLE_VS_EVENT_WR_BD_ADDR_COMP:
			break;
		case RBLE_VS_EVENT_SET_TEST_PARAM_COMP:
			break;
		case RBLE_VS_EVENT_READ_TEST_RSSI_COMP:
			break;
		case RBLE_EVT_VS_CMD_DISALLOWED_IND:
			break;
		case RBLE_VS_EVENT_SET_TX_POWER_COMP:
			break;
		default:
			break;
    }
}

static void DTM2Wire_Error( void )
{
	uint16_t	DTM2Wire_Event;

	DTM2Wire_Event = DTM2WIRE_STATUS_EVENT_BIT;
	DTM2Wire_Event |= DTM2WIRE_STATUS_ERR;
	DTM2Wire_bswap_serial_write( DTM2Wire_Event );
}

static void DTM2Wire_bswap_serial_write( uint16_t event )
{
    static uint16_t val16;

	val16 = ( ( ( event << 8 ) & 0xFF00 ) | ( ( event >> 8 ) & 0x00FF ) );
	
	serial_write( ( const uint8_t * )&val16, sizeof( uint16_t ) );
}

#endif
