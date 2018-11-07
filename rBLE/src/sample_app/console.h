/**
 ****************************************************************************************
 *
 * @file		console.h
 *
 * @brief	Console Sample Source for rBLE Sample Program
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * $Rev: 2301 $
 *
 ****************************************************************************************
 */

#ifndef _CONSOLE_H
#define _CONSOLE_H

#include	"types.h"
#include	"rble.h"
#include	"prf_sel.h"
#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */
#include	"ke_task.h"
#endif

#define	COLOR_LIGHT				0x80
#define	COLOR_BLACK				0x00
#define	COLOR_DARK_GRAY			(COLOR_LIGHT|COLOR_BLACK)
#define	COLOR_BLUE				0x01
#define	COLOR_LIGHT_BLUE		(COLOR_LIGHT|COLOR_BLUE)
#define	COLOR_GREEN				0x02
#define	COLOR_LIGHT_GREEN		(COLOR_LIGHT|COLOR_GREEN)
#define	COLOR_CYAN				(COLOR_GREEN|COLOR_BLUE)
#define	COLOR_LIGHT_CYAN		(COLOR_LIGHT|COLOR_CYAN)
#define	COLOR_RED				0x04
#define	COLOR_LIGHT_RED			(COLOR_LIGHT|COLOR_RED)
#define	COLOR_MAGENTA			(COLOR_RED|COLOR_BLUE)
#define	COLOR_LIGHT_MAGENTA		(COLOR_LIGHT|COLOR_MAGENTA)
#define	COLOR_YELLOW			(COLOR_RED|COLOR_GREEN)
#define	COLOR_LIGHT_YELLOW		(COLOR_LIGHT|COLOR_YELLOW)
#define	COLOR_LIGHT_GRAY		(COLOR_BLUE|COLOR_GREEN|COLOR_RED)
#define	COLOR_WHITE				(COLOR_LIGHT|COLOR_LIGHT_GRAY)


#define	TITLE_COLOR				COLOR_WHITE
#define	CONSOLE_COLOR			COLOR_WHITE
#define	EVENT_COLOR				COLOR_LIGHT_YELLOW
#define	COMMAND_COLOR			COLOR_LIGHT_CYAN
#define	MESSAGE_COLOR			COLOR_WHITE

#ifdef __cplusplus
extern "C" {
#endif
BOOL Console_Init(void);
void Console_Terminate(void);
void Console_SetInput( int16_t KeyNo );
void Console_SetInput_ESCKEY( void );
void Console_SetTextAttribute( uint32_t Set_Attribute );

#ifdef __cplusplus
}
#endif

//#define	__USE_PUTCHAR__
#ifndef	__USE_PUTCHAR__
#define	printf		Printf

void Printf( const char_t *Format, ... );
#else
#if defined(_USE_CCRL_RL78) && defined(USE_FW_UPDATE_PROFILE)
#define	printf		printf_tiny
#endif
#endif

/* Tas Infomation */
#define RBLE_APP_STATE_MAX	1	// Max State Num
#define RBLE_APP_IDX_MAX	1	// Max ID Num

#define RBLE_APP_CON_EVT			KE_FIRST_MSG(TASK_CON_APPL) + 1	/* Task API ID */

#ifdef USE_CUSTOM_DEMO
#define RBLE_APP_CON_TIMER_API		KE_FIRST_MSG(TASK_CON_APPL) + 2	/* Timer API ID */
#define RBLE_APP_CON_SW_EVT			KE_FIRST_MSG(TASK_CON_APPL) + 3	/* SW API ID */
#define RBLE_APP_CON_SEND_EVT		KE_FIRST_MSG(TASK_CON_APPL) + 4	/* SEND API ID */

typedef void ( *RBLE_CON_FUNC )( void );			/* Driver Task Function */
#endif
/**************************** Takse Declaration ***************************************/
extern const struct ke_state_handler rble_app_state_handler[RBLE_APP_STATE_MAX];
extern const struct ke_state_handler rble_app_default_handler;
extern ke_state_t rble_app_state[RBLE_APP_STATE_MAX];

void console_rx_done( void );
void console_tx_done( void );
void console_rx_error( void );
bool console_can_sleep(void);

#ifdef USE_CUSTOM_DEMO
void Console_Send_Timer_msg( RBLE_CON_FUNC Func );
bool Console_Set_Timer( uint16_t Timer_Cnt );
bool Console_Clear_Timer( void );
void Console_Send_msg( void );
#endif

#endif /* _CONSOLE_H */
