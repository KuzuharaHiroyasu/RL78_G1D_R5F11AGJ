/**
 ****************************************************************************************
 *
 * @file        beacon_app.h
 *
 * @brief       rBLE Sample Application for Beacon mode
 *
 * Copyright(C) 2012-2015 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _BEACON_APP_H
#define _BEACON_APP_H

#include    "types.h"
#include    "rble.h"
#include    "arch.h"
#include    "rwke_api.h"

#define APP_STATE_MAX       1
#define APP_IDX_MAX         1

/**************************** PUBLIC Memory Declaration *******************************/
extern const struct ke_state_handler APP_State_Handler[];
extern const struct ke_state_handler APP_Default_Handler;
extern              ke_state_t       APP_State[];

/**************************** PROTOTYPE Declaration ***********************************/
#ifdef __cplusplus
extern "C" {
#endif
BOOL APP_Init( void );
BOOL APP_Is_Idle( void );
#ifdef __cplusplus
}
#endif

#endif /* _BEACON_APP_H */
