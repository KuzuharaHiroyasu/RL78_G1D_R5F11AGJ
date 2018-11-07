/**
 ****************************************************************************************
 *
 * @file codeflash.h
 *
 * @brief CODEFLASH driver
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 * rev.205
 *
 ****************************************************************************************
 */

#ifndef CODEFLASH_H
#define CODEFLASH_H

/*
 * DEFINES
 ****************************************************************************************
 */
 /* write data size */
#define CODEFLASH_WRITE_DATA_SIZE		256
/* retry count */
#define CODEFLASH_WRITE_RETRY_NUM		3

/* voltage modes */
/* Full Speed Mode */
#define CODEFLASH_FULL_SPEED_MODE		0
/* Wide Voltage Mode */
#define CODEFLASH_WIDE_VOLTAGE_MODE		1

/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */
typedef struct {
	uint8_t voltage_mode;
	uint8_t freqency;
}CODEFLASH_INIT_INFO;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
bool codeflash_enable(CODEFLASH_INIT_INFO *init_info);
bool codeflash_blankcheck(uint8_t block_num);
bool codeflash_erase(uint8_t block_num);
bool codeflash_write(uint8_t *data, uint16_t data_size, uint8_t block_num);
bool codeflash_invertbootflag(void);
bool codeflash_disable(void);
void codeflash_forcereset(void);


#endif