/**
 ****************************************************************************************
 *
 * @file dataflash.h
 *
 * @brief DATAFLASH driver
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.205
 *
 ****************************************************************************************
 */

#ifndef DATAFLASH_H
#define DATAFLASH_H

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"
#include "co_bt.h"  /* for refferring struct bd_addr definition */

/*
 * DEFINES
 ****************************************************************************************
 */
#define EEL_TYPE      (2)          /* EEPROM Emulation Library Pack02 */
 
#define EEL_MODE_POLLING            (0x00)  /* Polling  Mode */
#define EEL_MODE_ENFORCED           (0xFF)  /* Enforced Mode */

#define DF_MODE_POLLING             (EEL_MODE_POLLING )
#define DF_MODE_ENFORCED            (EEL_MODE_ENFORCED)

typedef enum
{
    DF_READ = 0,
    DF_WRITE
} df_rw_t;

/* error code */
enum
{
    DF_OK                    = 0,   /* 0x00 */
    DF_ERR_BUSY                 ,   /* 0x01 */
    DF_ERR_INVALID_PARAM        ,   /* 0x02 */
    DF_ERR_DISALLOWED           ,   /* 0x03 */
    DF_ERR_UNSUPPORTED          ,   /* 0x04 */
    DF_ERR_INITIALIZATION       ,   /* 0x05 */
    DF_ERR_ACCESS_LOCKED        ,   /* 0x06 */
    DF_ERR_COMMAND              ,   /* 0x07 */
    DF_ERR_PARAMETER            ,   /* 0x08 */
    DF_ERR_REJECTED             ,   /* 0x09 */
    DF_ERR_NO_INSTANCE          ,   /* 0x0A */
    DF_ERR_POOL_FULL            ,   /* 0x0B */
    DF_ERR_POOL_INCONSISTENT    ,   /* 0x0C */
    DF_ERR_POOL_EXHAUSTED       ,   /* 0x0D */
    DF_ERR_INTERNAL             ,   /* 0x0E */
    DF_ERR_UNKNOWN                  /* 0x0F */
};

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
 * @brief   initialize dataflash library(EEL/FAL)
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_init(void);

/**
 ****************************************************************************************
 * @brief   open and start dataflash library(EEL/FAL)
 *
 * @param[in] mode      execution mode
 *                       - DF_MODE_ENFORCED
 *                          continue to execute until processing finished.
 *                          when DataFlash is not formatted, execute to format.
 *                       - DF_MODE_POLLING
 *                          start to processing.
 *                          need to call "dataflash_continue" until processing finished.
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_start(const uint8_t mode);

/**
 ****************************************************************************************
 * @brief   stop and close dataflash library(EEL/FAL)
 *
 * @param[in ] mode     execution mode
 *                       - DF_MODE_ENFORCED
 *                          continue to execute until processing finished.
 *                          when DataFlash is not formatted, execute to format.
 *                       - DF_MODE_POLLING
 *                          start to processing.
 *                          need to call "dataflash_continue" until processing finished.
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_stop(const uint8_t mode);

/**
 ****************************************************************************************
 * @brief   read data from DataFlash / write data to DataFlash
 *
 * @param[in ] mode     execution mode
 *                       - DF_MODE_ENFORCED
 *                          continue to execute until processing finished.
 *                          when DataFlash is not formatted, execute to format.
 *                       - DF_MODE_POLLING
 *                          start to processing.
 *                          need to call "dataflash_continue" until processing finished.
 * @param[in ] rw       read or write (DF_READ/DF_WRITE)
 * @param[in ] id       data id(registered to EEL in order to specify data)
 * @param[out] addr     destination RAM address
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_rw(const uint8_t mode, const df_rw_t rw, const uint8_t id, uint8_t* addr);

/**
 ****************************************************************************************
 * @brief   cleanup DataFlash
 *
 * @param[in ] mode     execution mode
 *                       - DF_MODE_ENFORCED
 *                          continue to execute until processing finished.
 *                          when DataFlash is not formatted, execute to format.
 *                       - DF_MODE_POLLING
 *                          start to processing.
 *                          need to call "dataflash_continue" until processing finished.
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_cleanup(const uint8_t mode);

/**
 ****************************************************************************************
 * @brief   format DataFlash
 *
 * @param[in ] mode     execution mode
 *                       - DF_MODE_ENFORCED
 *                          continue to execute until processing finished.
 *                          when DataFlash is not formatted, execute to format.
 *                       - DF_MODE_POLLING
 *                          start to processing.
 *                          need to call "dataflash_continue" until processing finished.
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_format(const uint8_t mode);

/**
 ****************************************************************************************
 * @brief   continue to execute processing
 *
 * @param[in] fin_flg   flag indicating whether processing finished or not
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_continue(bool* fin_flg);

/**
 ****************************************************************************************
 * @brief   get size of available space
 *
 * @param[out] wsize    size of available space
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_get_space(uint16_t* wsize);

/**
 ****************************************************************************************
 * @brief   get EEL version string
 *
 * @param[out] ver      version string buffer
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
uint8_t dataflash_getver(uint8_t* ver);

/**
 ****************************************************************************************
 * @brief   inform whether dataflash is stopped or not
 *          (if dataflash is started, prohibit STOP/HALT of MCU)
 *
 * @return  bool    true: stopped, false: not stopped
 ****************************************************************************************
 */
bool dataflash_is_stopped(void);

/**
 ****************************************************************************************
 * @brief   get device address from DataFlash
 *          (if device address does not exist in DataFlash, get from CodeFlash)
 *
 * @param[out] bda      device address
 *
 ****************************************************************************************
 */
void flash_get_bda(struct bd_addr* bda);

#endif /* DATAFLASH_H */
