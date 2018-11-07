/**
 ****************************************************************************************
 *
 * @file rwble_mem.h
 *
 * @brief Definitions of functions and global value.
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _RWBLE_MEM_H_
#define _RWBLE_MEM_H_

/**
 ****************************************************************************************
 * @addtogroup RWBLE_MEM
 * @ingroup RWBLE_MEM
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/**
 * DEFINES
 ****************************************************************************************
 */
typedef void (*RWBLE_SET_MEM_FUNC)( void );	/**< allcate memory function */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
extern const RWBLE_SET_MEM_FUNC  rwble_set_mem_l2cap_func;
extern const RWBLE_SET_MEM_FUNC  rwble_set_mem_gap_func;
extern const RWBLE_SET_MEM_FUNC  rwble_set_mem_gatt_func;
extern const RWBLE_SET_MEM_FUNC  rwble_set_mem_att_func;
extern const RWBLE_SET_MEM_FUNC  rwble_set_mem_smp_func;

extern int16_t  ble_connection_max; // BLE_CONN_MAX


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
extern void rwble_set_mem(void);
extern void rwble_set_mem_cont(void);
extern void ke_init(void);


/// @} RWBLE_MEM

#endif /* _RWBLE_MEM_H_ */
