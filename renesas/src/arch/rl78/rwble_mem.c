/**
 ****************************************************************************************
 *
 * @file rwble_mem.c
 *
 * @brief Allocate memory for BLE stack.
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

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
#include "arch.h"
#include "config.h"
#include "rwble_mem.h"


/*
 * DEFINES
 ****************************************************************************************
 */

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @brief Memory allocation for BLE stack.
 *****************************************************************************************
 */
void rwble_set_mem(void)
{
    rwble_set_mem_cont();

    /* gap memory allocate */
	( *rwble_set_mem_gap_func )();

    /* l2cap memory allocate */
	( *rwble_set_mem_l2cap_func )();
    /* gatt memory allocate */
	( *rwble_set_mem_gatt_func )();
    /* att memory allocate */
	( *rwble_set_mem_att_func )();
    /* smp memory allocate */
	( *rwble_set_mem_smp_func )();
}

///@} RWBLE_MEM
