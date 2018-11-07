/**
 ****************************************************************************************
 *
 * @file		config.h
 *
 * @brief	configuration file for ble stack
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.214
 *
 ****************************************************************************************
 */

#ifndef _ARCH_CONFIG_H_
#define _ARCH_CONFIG_H_

#include "compiler.h"
#include "prf_sel.h"
#include "db_handle.h"

/*
 * HEAP SIZE DEFINITIONS
 ****************************************************************************************
 */
/// Maximum number of simultaneous connections
#define BLE_CONN_MAX          CFG_CON

#if (BLE_CONN_MAX == 1)
#define BLE_HEAP_CONT       (1809)
#elif (BLE_CONN_MAX == 2)
#define BLE_HEAP_CONT       (2224)
#elif (BLE_CONN_MAX == 3)
#define BLE_HEAP_CONT       (2732)
#elif (BLE_CONN_MAX == 4)
#define BLE_HEAP_CONT       (3244)
#elif (BLE_CONN_MAX == 5)
#define BLE_HEAP_CONT       (3752)
#elif (BLE_CONN_MAX == 6)
#define BLE_HEAP_CONT       (4264)
#elif (BLE_CONN_MAX == 7)
#define BLE_HEAP_CONT       (4772)
#elif (BLE_CONN_MAX == 8)
#define BLE_HEAP_CONT       (5284)
#else 
#error "an unsupported number of connections."
#endif

//#define BLE_HEAP_CONT		1500
#define BLE_HEAP_HOST		720

/* for database(host database + profile database) */
/* size of struct atts_desc */
#define BLE_DB_STRUCT_SIZE	10
/* size of database(host database + profile database) */
#define BLE_DB_SIZE			DB_HDL_MAX * BLE_DB_STRUCT_SIZE

/// Size of the heap
#define BLE_HEAP_SIZE       ((BLE_CONN_MAX * 256) + 256                     \
                              + ( BLE_HEAP_CONT )                           \
                              + ( BLE_HEAP_HOST * BLE_CONN_MAX )            \
                              + ( BLE_DB_SIZE )                             \
                              + ( RBLE_TABLE_SIZE )                         \
                            )

/*
 * POINTER DEFINITIONS
 ****************************************************************************************
 */
/* User Device Information Pointer in CodeFlash */
#if   defined(_USE_R5F11AGG)
#define userinfo_top        (0x1FC00)   /* for R5F11AGG(ROM128KB) */
#elif defined(_USE_R5F11AGH)
#define userinfo_top        (0x2FC00)   /* for R5F11AGH(ROM192KB) */
#else // !defined(_USE_R5F11AGG) && !defined(_USE_R5F11AGH)
#define userinfo_top        (0x3FC00)   /* for R5F11AGJ(ROM256KB) */
#endif
#define public_addr_ptr     ((__far const struct bd_addr*    )(userinfo_top+0))
#define devce_name_ptr      ((__far const struct device_name*)(userinfo_top+6))

/*
 * TEST DEVICE ADDRESS DEFINITIONS
 ****************************************************************************************
 */
/* Public Device Address for test and development use */
#define CFG_TEST_BDADDR     {{0xbc, 0x9a, 0x78, 0x56, 0x34, 0x12}}

#endif /* _ARCH_CONFIG_H_ */

