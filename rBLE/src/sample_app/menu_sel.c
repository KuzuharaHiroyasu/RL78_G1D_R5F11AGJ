/**
 ****************************************************************************************
 *
 * @file		menu_sel.c
 *
 * @brief	Console Menu Sample Source for rBLE Sample Program
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 2986 $
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

#include	"menu_sel.h"
#include	"console.h"
#include    "rble.h"

/**************************** Define Declaration **************************************/
/**************************** PRIVATE PROTOTYPE Declaration ***************************/
static void Menu_Disp( const SELECT_MENU *Menu_Arg_p );

/**************************** PRIVATE Memory Declaration ******************************/
#define	MENU_MAX			4

const SELECT_MENU		*Current_Menu_Mem[ MENU_MAX ];
int32_t					Current_Menu_Cnt;
/******************************* Program Area *************************************************/
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
BOOL Menu_Select_Init( void )
{
	Current_Menu_Cnt = 0;
	return( Console_Init() );
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
void Menu_Select_Terminate( void )
{
	Current_Menu_Cnt = 0;
}

/**********************************************************************************************/
/**
 *  @brief Menu Select
 *
 *  Select Menu for rBLE sample program@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# Input Console
 *    -# The function of the inputted number is called.
 *
 *  @param[in]
 *
 *  @retval		TRUE				Success
 *  @retval		FALSE				Error
 */
/**********************************************************************************************/
void Menu_Select( const SELECT_MENU *Menu_Arg_p )
{
	/* Select Menu Display */
	Menu_Disp( Menu_Arg_p );
	Current_Menu_Mem[ Current_Menu_Cnt ] = Menu_Arg_p;
}

void Menu_Key_Select( int32_t Key_Data )
{
	const SELECT_MENU	*Current_Menu_p;
	const MENU_INFO		*Menu_p;
	char_t				*Command_Message_p;
	char_t				Temp[ 50 ];
	uint_t				Length;

	Current_Menu_p = Current_Menu_Mem[ Current_Menu_Cnt ];
	if ( NULL != Current_Menu_p ) {
		Console_SetTextAttribute( COMMAND_COLOR );
		if ( ERROR_KEY == Key_Data ) {
			Menu_Disp( Current_Menu_p );
		} else if ( CR_KEY == Key_Data ) {
			Menu_Disp( Current_Menu_p );
		} else {
			for ( Menu_p = Current_Menu_p->Menu_p; NULL != Menu_p->Menu_Disp_p ;Menu_p++ ) {
				if ( 0 != Menu_p->Sel_No ) {
					if ( Menu_p->Sel_No == Key_Data ) {
						if ( NULL != Menu_p->Func ) {
							if ( NULL != Menu_p->Menu_Disp_p ) {
								Command_Message_p = strchr( Menu_p->Menu_Disp_p, '.' );
								if ( NULL != Command_Message_p ) {
									Command_Message_p++;
									Length = strlen( Command_Message_p );
									if ( sizeof( Temp ) > ( Length + 2 ) ) {
										memcpy( Temp, Command_Message_p, Length );
										Temp[ Length ] = ' ';
										Temp[ Length + 1 ] = 0;
										Command_Message_p = Temp;
									}
									printf( "\nCMD -> %s", Command_Message_p );
								}
							}
							if ( FALSE == ( ( Menu_p->Func )() ) ) {
								if ( 0 != Current_Menu_Cnt ) {
									Current_Menu_Cnt--;
									Menu_Select( Current_Menu_Mem[ Current_Menu_Cnt ] );
								}
							} else {
								Prompt_Disp();
							}
						} else {
							/* calls again. */
							Current_Menu_Cnt++;
							Menu_Select( Menu_p->Next_Menu_Top );
						}
						break;
					}
				}
			}
		}
	}
}

/**********************************************************************************************/
/**
 *  @brief Menu exit
 *
 *  Select Menu Exit for rBLE sample program@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# FALSE value is returned.
 *
 *  @param[in]
 *
 *  @retval		FALSE				Error
 */
/**********************************************************************************************/
BOOL Menu_Exit( void )
{
	return( FALSE );
}

/**********************************************************************************************/
/**
 *  @brief Prompt Display
 *
 *  Display Prompt for rBLE sample program@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# A menu message is outputted in printf.
 *
 *  @param[in]
 *
 *  @retval
 */
/**********************************************************************************************/
void Prompt_Disp( void )
{
	Console_SetTextAttribute( CONSOLE_COLOR );
	printf( ">> " );
	Console_SetTextAttribute( COMMAND_COLOR );
}

/**********************************************************************************************/
/**
 *  @brief Menu Display
 *
 *  Display Select Menu for rBLE sample program@n
 *  
 *  @attention
 *  
 *  @b Contents of management:
 *    -# A menu message is outputted in printf.
 *
 *  @param[in]
 *
 *  @retval
 */
/**********************************************************************************************/
static void Menu_Disp( const SELECT_MENU *Menu_Arg_p )
{
	const MENU_INFO		*Menu_p;

	Console_SetTextAttribute( TITLE_COLOR );
	/* Title Display */
	printf( Menu_Arg_p->Title_p );
	printf( "\n" );
	for ( Menu_p = Menu_Arg_p->Menu_p; NULL != Menu_p->Menu_Disp_p ;Menu_p++ ) {
		printf( Menu_p->Menu_Disp_p );
	}
	Prompt_Disp();
}

#endif	/* CONFIG_EMBEDDED */
#endif	/* CFG_SAMPLE */
