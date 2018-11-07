/**
 ****************************************************************************************
 *
 * @file		menu_sel.h
 *
 * @brief	Console Menu Sample Source for rBLE Sample Program
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 2986 $
 *
 ****************************************************************************************
 */

#ifndef _MENU_SEL_H
#define _MENU_SEL_H

#include	"types.h"
#include	"rble.h"

#define	ERROR_KEY				-1
#define	ESC_KEY					( 0x1b * -1 )
#define	CR_KEY					( 0x0d * -1 )
#define	LF_KEY					( 0x0a * -1 )
#define	BS_KEY					( 0x08 * -1 )

typedef	struct MENU_INFO_t {
	int_t						Sel_No;								/* Selection number */
	BOOL						( *Func )( void );					/* After selection call function pointer */
	const struct SELECT_MENU_t	*Next_Menu_Top;						/* Next menu pointer */
	char_t						*Menu_Disp_p;						/* Menu display pointer */
} MENU_INFO;

typedef	struct SELECT_MENU_t {
	char_t 					*Title_p;							/* Title display pointer */
	const MENU_INFO			*Menu_p;							/* Menu data pointer */
} SELECT_MENU;

#ifdef __cplusplus
extern "C" {
#endif
BOOL Menu_Select_Init( void );
void Menu_Select_Terminate( void );
void Menu_Select( const SELECT_MENU *Menu_Arg_p );
void Menu_Key_Select( int32_t Key_Data );
BOOL Menu_Exit( void );
void Prompt_Disp( void );
#ifdef __cplusplus
}
#endif

#endif /* _MENU_SEL_H */
