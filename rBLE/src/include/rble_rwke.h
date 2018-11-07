/**
 ****************************************************************************************
 *
 * @file		rble_rwke.h
 *
 * @brief	rBLE rwke header file
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * $Rev: 2301 $
 *
 ****************************************************************************************
 */

#ifndef	__RBLE_RWKE_INC__
#define	__RBLE_RWKE_INC__

/**************************** Define Declaration ****************************************/
/* Tas Infomation */
#define RBLE_STATE_MAX	1	// rBLE Max State Num
#define RBLE_IDX_MAX	1	// rBLE Max ID Num

#define RBLE_TIMER_STATE_MAX	1	/* Timer Max State Num */
#define RBLE_TIMER_IDX_MAX		1	/* Timer Max ID Num */

/**************************** Structure Declaration *************************************/

/**************************** PROTOTYPE Declaration *************************************/
extern struct ke_state_handler rble_state_handler[RBLE_STATE_MAX];
extern const struct ke_state_handler rble_default_handler;
extern ke_state_t rble_state[RBLE_STATE_MAX];

extern const struct ke_state_handler rble_timer_state_handler[RBLE_TIMER_STATE_MAX];
extern const struct ke_state_handler rble_timer_default_handler;
extern ke_state_t rble_timer_state[RBLE_TIMER_STATE_MAX];

#endif // __RBLE_RWKE_INC__
