/**
 ****************************************************************************************
 *
 * @file		Console.c
 *
 * @brief	Console Sample Source for rBLE Sample Program
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * $Rev: 2516 $
 *
 ****************************************************************************************
 */

#include "rwble_config.h"
#include "arch.h"

#ifdef	CFG_SAMPLE

#ifdef CONFIG_EMBEDDED
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <assert.h>
#include <stdarg.h>

#include	"console.h"
#include	"menu_sel.h"
#include	"rble_app.h"
#include	"serial.h"

/**************************** Define Declaration **************************************/
#define	CR_KEY_CODE				0x0d
#define	LF_KEY_CODE				0x0a
#define	ESC_KEY_CODE			0x1b
#define	BS_KEY_CODE				0x08

/**************************** PRIVATE PROTOTYPE Declaration ***************************/
static int_t RBLE_Console_In(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

#ifdef USE_CUSTOM_DEMO
static int_t console_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t RBLE_Timer_In(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t RBLE_Send_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
#endif
/**************************** PRIVATE Memory Declaration ******************************/
const struct ke_msg_handler rble_app_handler[] =
{
	{ RBLE_APP_CON_EVT, (ke_msg_func_t)RBLE_Console_In },
#ifdef USE_CUSTOM_DEMO
	{ RBLE_APP_CON_TIMER_API, (ke_msg_func_t)console_timer_handler },
	{ RBLE_APP_CON_SW_EVT, (ke_msg_func_t)RBLE_Timer_In },
	{ RBLE_APP_CON_SEND_EVT, (ke_msg_func_t)RBLE_Send_handler },
#endif
};

/* Status Handler */
const struct ke_state_handler rble_app_state_handler[RBLE_APP_STATE_MAX] =
{   /* State message handlers */
    KE_STATE_HANDLER(rble_app_handler)
};

/* Default Handler */
const struct ke_state_handler rble_app_default_handler = KE_STATE_HANDLER_NONE;

/* Status */
ke_state_t rble_app_state[RBLE_APP_IDX_MAX];

#define KEY_BUF_MAX			100

#ifndef	__USE_PUTCHAR__
#define		STREAM_MEMORY_MAX_LINE_SIZE	80
#endif

char_t			Key_Buf[ KEY_BUF_MAX ];
int_t			Key_Cnt;
#ifndef	__USE_PUTCHAR__
char_t			Stream_Buffer[ STREAM_MEMORY_MAX_LINE_SIZE ];
#endif
volatile bool	Send_Flg;

#ifdef USE_CUSTOM_DEMO
static RBLE_CON_FUNC Timer_func;
#endif
/******************************* Program Area *************************************************/
/**********************************************************************************************/
/**
 *  @brief Console input function
 *
 *  Input console for rBLE sample program@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Input Console
 *
 *  @param[in]
 *
 *  @retval			KE_MSG_CONSUMED		msg consumed
 */
/**********************************************************************************************/
static int_t RBLE_Console_In(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	int16_t			Key_Data;
	int32_t			Ret_Key;
	char_t			*Result_p;
	char_t			*Result_Tmp_p;
	int32_t			Ret_Temp;
	int_t			Base;

	Key_Data = Key_Buf[ Key_Cnt ];
	if ( BS_KEY_CODE == Key_Data ) {
		if ( 0 != Key_Cnt ) {
			printf( "%c %c", BS_KEY_CODE, BS_KEY_CODE );
			Key_Cnt--;
			Key_Buf[ Key_Cnt ] = '\0';
		}
	} else if ( ESC_KEY_CODE == Key_Data ) {
		Ret_Key = ESC_KEY;
		Key_Cnt = 0;
		Menu_Key_Select( Ret_Key );
	} else {
		printf( "%c", Key_Data );
		if ( ( CR_KEY_CODE == Key_Data )
			|| ( LF_KEY_CODE == Key_Data ) ) {
			if ( 0 != Key_Cnt ) {
				RBLE_Parameter_Cnt = 0;
				Ret_Key = strtol( Key_Buf, &Result_p, 10 );
				if ( Key_Buf == Result_p ) {
					Ret_Key = CR_KEY;
				} else if ( &Key_Buf[ Key_Cnt ] != Result_p ) {
					for ( ; ( ( ' ' == ( *Result_p ) ) && ( RBLE_Parameter_Cnt < RBLE_PARAMETER_MAX ) ); ) {
						if ( ( '0' == Result_p[ 1 ] ) && ( 'x' == Result_p[ 2 ] ) ) {
							Base = 16;
						} else {
							Base = 10;
						}
						Ret_Temp = strtol( Result_p, &Result_Tmp_p, Base );
						if ( Result_p != Result_Tmp_p ) {
							RBLE_Parameter_Arg[ RBLE_Parameter_Cnt ] = Ret_Temp;
							RBLE_Parameter_Cnt++;
							Result_p = Result_Tmp_p;
						} else {
							break;
						}
					}
				}
				memset( Key_Buf, 0, Key_Cnt );
				Key_Cnt = 0;
			} else {
				Ret_Key = CR_KEY;
			}
			Menu_Key_Select( Ret_Key );
		} else {
			Key_Cnt++;
			Key_Buf[ Key_Cnt ] = '\0';
		}
	}
	serial_read( ( uint8_t * )&Key_Buf[ Key_Cnt ], 1 );
	return (KE_MSG_CONSUMED);
}

/**********************************************************************************************/
/**
 *  @brief Initialize Console Program
 *
 *  Initialize Console Program for rBLE sample program@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Parameter Check
 *    -# Create Event
 *
 *  @param[in]
 *
 *  @retval		TRUE				Success
 *  @retval		FALSE				Error
 */
/**********************************************************************************************/
BOOL Console_Init( void )
{
	uint16_t		Ret = TRUE;
	SERIAL_EVENT_PARAM call_back ={0};

	call_back.rx_callback = &console_rx_done;
	call_back.tx_callback = &console_tx_done;
	call_back.err_callback = &console_rx_error;
	call_back.rx_cmp_callback = NULL;
	//	Initialization of a UART driver.(T.B.D)
	serial_init( &call_back );

	Send_Flg = false;
	Key_Cnt = 0;
	serial_read( ( uint8_t * )&Key_Buf[ 0 ], 1 );
    return( Ret );
}

/**********************************************************************************************/
/**
 *  @brief Terminate Console Program
 *
 *  Terminate Console Program for rBLE sample program@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Terminate Thread
 *    -# Close Handle
 *
 *  @param[in]
 *
 *  @retval
 */
/**********************************************************************************************/
void Console_Terminate( void )
{
}

void Console_SetInput( int16_t KeyNo )
{
}

void Console_SetInput_ESCKEY( void )
{
}

void Console_SetTextAttribute( uint32_t Set_Attribute )
{
	switch ( Set_Attribute ) {
		case EVENT_COLOR:
			printf("\x1b[33m");
			break;
		case COMMAND_COLOR:
			printf("\x1b[36m");
			break;
		case MESSAGE_COLOR:
		default:
			printf("\x1b[39;49m");
			break;
	}
}

#ifndef	__USE_PUTCHAR__

#ifdef _USE_CCRL_RL78
 #if 1 //#ifdef USE_FW_UPDATE_PROFILE
  #define __USE_CUSTOM_VSPRINTF
 #endif//USE_FW_UPDATE_PROFILE

#ifdef __USE_CUSTOM_VSPRINTF
#define KEY_NULL		'\0'
#define PAD_ZERO		(0x8000)
#define PAD_LONG		(0x4000)
#define PAD_FLOAT		(0x2000)
#define PAD_INT_MASK	(0x000F)
#define PAD_FLT_MASK 	(0x00F0)
#define TEMP_BUFF_SIZE	(10+1)

#define FLAOT_DEC_POINT	(6)
#define FLAOT_DP_COEF	(1000000UL)

static uint16_t copystr(char_t *dest, char_t *src);
static uint16_t padstr(char_t *dest, char_t *numstr, uint16_t len, uint16_t pad);
static uint16_t dec2str(char_t *dest, int32_t data, uint16_t pad, bool unsign);
static uint16_t hex2str(char_t *dest, uint32_t data, uint16_t pad, bool small);
static uint16_t flt2str(char_t *dest, float data, uint16_t pad);
static void     custom_vsprintf(char_t *buff, const char_t *format, va_list list);

static uint16_t copystr(char_t *dest, char_t *src)
{
	uint16_t ret = 0;

	if( src != NULL )
	{
		while( *src != KEY_NULL )
		{
			*dest++ = *src++;
			ret++;
		}
	}

	return ret;
}

static uint16_t padstr(char_t *dest, char_t *numstr, uint16_t len, uint16_t pad)
{
	char_t   buff[TEMP_BUFF_SIZE];
	char_t  *top;
	char_t  *end = &buff[sizeof(buff)-1];
	uint16_t ret = 0;
	char_t   pad_chr = (char_t)((pad & PAD_ZERO)?'0':' ');

	pad &= PAD_INT_MASK;

	if( (numstr == NULL) || (len == 0) )
	{
		return 0;
	}

	memset(buff, pad_chr, sizeof(buff));
	*end-- = KEY_NULL;

	top = end - len + 1;
	memcpy(top, numstr, len);

	for(ret = len; ret < pad; ret++)
	{
		top--;
	}
	
	copystr(dest, top);

	return ret;
}

static uint16_t dec2str(char_t *dest, int32_t data, uint16_t pad, bool unsign)
{
	char_t buff[TEMP_BUFF_SIZE];
	char_t *top = &buff[sizeof(buff)-1];
	uint32_t usdata = (uint32_t)data;
	int32_t  sdata  = data;
	if( sdata < 0 ) sdata *=-1;

	*top = KEY_NULL;

	do {
		top--;
		if( unsign == false )
		{
			*(top) = (char_t)((char_t)(sdata % 10) + (char_t)'0');
			sdata /= 10;
			if( sdata == 0 )
			{
				break;
			}
		}
		else
		{
			*(top) = (char_t)((char_t)(usdata % 10) + (char_t)'0');
			usdata /= 10;
			if( usdata == 0 )
			{
				break;
			}
		}
	}while(top > &buff[0]);

	if( (unsign == false) && (data < 0) )
	{
		*--top = '-';
	}

	return padstr(dest,top,strlen(top),pad);
}


static uint16_t hex2str(char_t *dest, uint32_t data, uint16_t pad, bool small)
{
	char_t buff[TEMP_BUFF_SIZE];
	char_t *top = &buff[sizeof(buff)-1];
	char_t a_chr = (char_t)((small)?'a':'A');

	*top = KEY_NULL;

	do {
		top--;
		*top = (char_t)(data&0x0F);
		// A to F
		if( *top > 9 )
		{
			*top += (char_t)(a_chr - 10);
		}
		// 0 to 9
		else
		{
			*top += (char_t)'0';
		}
		data >>= 4;
	}while( (data != 0) && (top > &buff[0]));

	return padstr(dest,top,strlen(top),pad);
}

static uint16_t flt2str(char_t *dest, float data, uint16_t pad)
{
	int32_t  temp;
	uint16_t ret = 0;
	uint16_t i;

	// Rounding (decimals point size zero)
	if( (pad & PAD_FLT_MASK) == PAD_FLT_MASK )
	{
		data += 0.5;
	}
	// integer part
	temp = (int32_t)(data);
	if( (temp == 0) && (data < 0) )
	{
		*dest++ = '-';
	}
	ret = dec2str(dest, temp, 0, false);
	dest += ret;
	if( data < 0 )
	{
		if( temp == 0 )
		{
			// add '-' 1byte
			ret++;
		}
		// convert ABS
		data *= -1;
	}
	// decimals point part (Note: don't care fluctuations)
	temp = (int32_t)(data * FLAOT_DP_COEF);
	temp %= FLAOT_DP_COEF;
	// padding
	if( (pad & PAD_FLT_MASK) != PAD_FLT_MASK )
	{
		pad &= PAD_FLT_MASK;
		pad >>= 4;
		if( (pad > FLAOT_DEC_POINT) || (pad == 0) )
		{
			pad = FLAOT_DEC_POINT;
		}
		*dest++ = '.';
		ret++;
		// Rounding loop
		for( i = pad; i < FLAOT_DEC_POINT; i++ ) 
		{
			temp += 5;
			temp /= 10;
		}
		ret += dec2str(dest, temp, (pad | PAD_ZERO), false);
	}

	return ret;
}

/*
 *	supported format: %[flag][field-width][precision][size][type-specification-character]
 *		(1) Flag
 *			-, +, #, or space cannot be specified.
 *		(2) Field width
 *			"*" cannot be specified.
 *		(3) Precision
 *			"*" cannot be specified.
 *		(4) Size
 *			h, ll or L cannot be specified.
 *		(5) Type specification character
 *			p, e, E, g, G or n cannot be specified.
 */
static void custom_vsprintf(char_t *buff, const char_t *format, va_list list)
{
	char_t  *p;
	char_t  *s;
	int32_t  temp;
	uint16_t pad;
	float    temp_f;
	bool     loop;

	if( *format == KEY_NULL ) return;

	for(p = (char_t*)format; *p != KEY_NULL ; ++p)
	{
		if( *p != '%' )
		{
			*buff++ = *p;
			continue;
		}

		pad = 0;
		loop = true;
		while( loop ) {
			if(*++p == KEY_NULL) break;

			switch( *p )
			{
			case 'x' :	// hex Lowercase
			case 'X' :	// hex Uppercase
				// long size
				if( pad & PAD_LONG )
				{
					temp = (int32_t)va_arg(list, uint32_t);
				}
				// short size
				else
				{
					temp = (int16_t)va_arg(list, uint16_t);
					temp &= 0xFFFF;
				}
				// Lowercase
				if( *p == 'x' )
				{
					buff += hex2str(buff,(uint32_t)temp,pad,true);
				}
				// Uppercase
				else
				{
					buff += hex2str(buff,(uint32_t)temp,pad,false);
				}
				loop = false;
				break;
			case 'd' :	// signed int
			case 'i' :	// signed int
			case 'u' :	// unsigned int
				// long size
				if(  pad & PAD_LONG )
				{
					temp = (int32_t)va_arg(list, int32_t);
				}
				// short size
				else
				{
					temp = (int16_t)va_arg(list, int16_t);
				}
				// unsigned
				if( *p == 'u' )
				{
					if( (temp < 0) && !(pad & PAD_LONG) )
					{
						temp &= 0xFFFF;
					}
					buff += dec2str(buff,temp,pad,true);
				}
				// signed
				else
				{
					buff += dec2str(buff,temp,pad,false);
				}
				loop = false;
				break;
			case 's' :	// string
				s = (char_t*)(char_t __far*)va_arg(list, int32_t);
				buff += copystr(buff, s);
				loop = false;
				break;
			case 'c':	// character
				*buff++ = (char_t)va_arg(list, char_t);
				loop = false;
				break;
			case 'f':	// float
				temp_f = (float)va_arg(list, float);
				buff += flt2str(buff,temp_f,pad);
				loop = false;
				break;
			/**** not supported ****/
			//case 'p':	// pointer
			//case 'o': // octet
			//case 'e': // double
			//case 'E': // double
			//case 'g': // double
			//case 'G': // double
			//case 'n': // number of count
			default  :
				if( *p == '0' ) 
				{
					if( pad & PAD_FLOAT )
					{
						pad |= PAD_FLT_MASK;
					}
					else
					{
						pad |= PAD_ZERO;
					}
				}
				else if( (*p >= '1') && (*p <= '9') ) 
				{
					if( pad & PAD_FLOAT )
					{
						pad &= ~PAD_FLT_MASK;
						pad |= (uint16_t)((*p - '0')) << 4;
					}
					else
					{
						pad &= ~PAD_INT_MASK;
						pad |= (uint16_t)((*p - '0'));
					}
				}
				else if( *p == 'l' ) 
				{
					pad |= PAD_LONG;
				}
				else if( *p == '.' )
				{
					pad |= PAD_FLOAT | PAD_FLT_MASK;
				}
				else
				{
					*buff++ = *p;
					loop = false;
				}
				break;
			}
		}
	}
	*buff = KEY_NULL;
}
#endif//__USE_CUSTOM_VSPRINTF
#endif//_USE_CCRL_RL78

void Printf( const char_t *Format, ... )
{
	va_list 	Args;
#ifndef _USE_IAR_RL78
	Args = NULL;
#endif//_USE_IAR_RL78
	for ( ;; ) {
		if ( false == Send_Flg ) {
	            break;
		}
	}

#ifdef _USE_REL_RL78
	va_starttop( Args, Format );
#else
	va_start( Args, Format );
//	va_arg( Args, int8_t* );
#endif
#ifdef __USE_CUSTOM_VSPRINTF
	custom_vsprintf( &Stream_Buffer[ 0 ], Format, Args );
#else //__USE_CUSTOM_VSPRINTF
	vsprintf( &Stream_Buffer[ 0 ], Format, Args );
#endif//__USE_CUSTOM_VSPRINTF
	Send_Flg = true;
	serial_write( ( uint8_t * )&Stream_Buffer[ 0 ], strlen( &Stream_Buffer[ 0 ] ) );
#ifndef _USE_REL_RL78
	va_end( Args );
#endif
}
#else
int_t putchar( int_t c )
{
	uint8_t	Data;

	for ( ;; ) {
		if ( false == Send_Flg ) {
	            break;
		}
	}
	Send_Flg = true;

	Data = c;
	serial_write( &Data, ( uint16_t )1 );
	return( 1 );
}

#endif

void console_rx_done( void )
{
	uint8_t *ke_msg;

    ke_msg = ke_msg_alloc( RBLE_APP_CON_EVT, TASK_CON_APPL, TASK_CON_APPL, 0 );

    ke_msg_send(ke_msg);
}

void console_tx_done( void )
{
	Send_Flg = false;
}

void console_rx_error( void )
{
	serial_read( ( uint8_t * )&Key_Buf[ Key_Cnt ], 1 );
}

bool console_can_sleep(void)
{
	return( false );
}

#ifdef USE_CUSTOM_DEMO
static int_t RBLE_Timer_In(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	Console_Set_Timer(10);
	
	return (KE_MSG_CONSUMED);
}

void Console_Send_Timer_msg( RBLE_CON_FUNC Func )
{
	uint8_t *ke_msg;
	
	Timer_func = Func;
	
	/* start timer msg */
	ke_msg = ke_msg_alloc(RBLE_APP_CON_SW_EVT, TASK_CON_APPL, TASK_CON_APPL, 0);
	
	ke_msg_send(ke_msg);
}

/******************************************************************************************/
/**
 *  @brief Timer Set
 *
 *  Timer Set@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# timer set
 *
 *  @param[in]				timer count
 *
 *  @retval		TRUE		Success
 */
/******************************************************************************************/
bool Console_Set_Timer( uint16_t Timer_Cnt )
{
	if ( 0 == Timer_Cnt ) {
		return( false );
	}

	ke_timer_set(RBLE_APP_CON_TIMER_API, TASK_CON_APPL, Timer_Cnt);
	return( true );
}

/******************************************************************************************/
/**
 *  @brief Timer Clear
 *
 *  Timer Clear@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# clear timer
 *
 *  @param[in]				none
 *
 *  @retval		TRUE		Success
 */
/******************************************************************************************/
bool Console_Clear_Timer( void )
{
	ke_timer_clear(RBLE_APP_CON_TIMER_API, TASK_CON_APPL);
	return( true );
}

/******************************************************************************************/
/**
 *  @brief Timer Handler
 *
 *  Timer Handler@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# timer handler
 *
 *  @param[in]		msgid			not use
 *                  *param			not use
 *                  dest_id			not use
 *                  src_id			not use
 *
 *  @retval			KE_MSG_CONSUMED		msg consumed
 */
/******************************************************************************************/
static int_t console_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	/* call func specified by APP */
	if (0 != Timer_func) {
		Timer_func();
	}
	return (KE_MSG_CONSUMED);
}

void Console_Send_msg( void )
{
	uint8_t *ke_msg;
	
	/* start timer msg */
	ke_msg = ke_msg_alloc(RBLE_APP_CON_SEND_EVT, TASK_CON_APPL, TASK_CON_APPL, 0);
	
	ke_msg_send(ke_msg);
}

static int_t RBLE_Send_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	/* call func specified by APP */
	if (0 != Timer_func) {
		Timer_func();
	}
	return (KE_MSG_CONSUMED);
}
#endif /* USE_CUSTOM_DEMO */

#endif	/* CONFIG_EMBEDDED */
#endif	/* CFG_SAMPLE */
