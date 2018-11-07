/**
 ****************************************************************************************
 *
 * @file codeflash.c
 *
 * @brief CODEFLASH driver
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 * rev.205
 *
 ****************************************************************************************
 */

#ifdef _USE_IAR_RL78
#define _CFL_DATA _Pragma("location=\"CFL_DATA\"") __NO_INIT
#define _CFL_CODE _Pragma("location=\"CFL_CODE\"")
#else
#define _CFL_DATA
#define _CFL_CODE

#if !defined(_USE_CCRL_RL78)
#pragma section @@CODEL CFL_CODE
#pragma section @@DATA  CFL_DATA
#else
#pragma section text    CFL_CODE
#pragma section bss     CFL_DATA
#endif
#endif

/**
 ****************************************************************************************
 * @addtogroup CODEFLASH
 *
 * @{
 ****************************************************************************************
 */
/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include	"rwble_config.h"
#include	"arch.h"

#ifdef USE_FW_UPDATE_PROFILE

#include	"fsl.h"
#include	"codeflash.h"

struct CODEFLASH_INFO
{
	bool enable;
};

_CFL_DATA static struct CODEFLASH_INFO codeflash_info; 

bool codeflash_write_data(uint8_t *data, uint16_t data_size, uint8_t block_num);

#ifdef _USE_CCRL_RL78
/* 
 * Replacing the standard library function and variable 
 * to the access address fixed for CC-RL. (errno, rand)
 */
_CFL_DATA uint16_t errno;
_CFL_DATA uint32_t rand_seed;
_CFL_CODE int __far rand(void)
{
	rand_seed = (rand_seed * 1103515245UL) + 12345UL;
	return (int)(rand_seed & RAND_MAX);
}
_CFL_CODE void __far srand(unsigned int seed)
{
	rand_seed = seed;
}
#endif//_USE_CCRL_RL78

/*
 * IF FUNCTION DEFINITIONS
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @brief Enable FSL Library.
 *
 * @param[in] init_info     setting of fsl library.
 *
 * @return bool         true if success, or false if falied
 ****************************************************************************************
 */
_CFL_CODE bool codeflash_enable(CODEFLASH_INIT_INFO *init_info)
{
	bool ret = true;
	fsl_descriptor_t fsl_des;
	uint8_t fsl_ret;
	
	/* initialise info */
	memset(&codeflash_info, 0, sizeof(codeflash_info));
	
	/* set register */
	write_sfr(DFLCTL, 1);					// DFLCTL bit0:1
	
	/* prepare fsl */
	fsl_des.fsl_flash_voltage_u08	= init_info->voltage_mode;	// Wide Voltage Mode
	fsl_des.fsl_frequency_u08		= init_info->freqency;		// CPU:8MHz
	fsl_des.fsl_auto_status_check_u08 = 1;						// Status Check Internal Mode
	
	fsl_ret = FSL_Init((__far fsl_descriptor_t*)&fsl_des);
	
	if(fsl_ret == FSL_OK) {
		FSL_Open();
		FSL_PrepareFunctions();
		FSL_PrepareExtFunctions();
		
		codeflash_info.enable = true;
	}
	else {
		ret = false;
	}
	
	return ret;
}

/**
 ****************************************************************************************
 * @brief Check block(blank or not).
 *
 * @param[in] block_num  number of block(0-255).
 *
 * @return bool         true if block is blank, or false if block is not blank
 ****************************************************************************************
 */
_CFL_CODE bool codeflash_blankcheck(uint8_t block_num)
{
	bool ret;
	uint8_t fsl_ret;
	if(codeflash_info.enable == true) {
		/* interrupt disable */
		GLOBAL_INT_DISABLE();
		
		fsl_ret = FSL_BlankCheck(block_num);
		if(fsl_ret == FSL_OK) {
			/* block is already erased */
			ret = true;
		}
		else {
			ret = false;
		}
		
		/* interrupt enable */
		GLOBAL_INT_RESTORE();
	}
	return ret;
}

/**
 ****************************************************************************************
 * @brief Erase block.
 *
 * @param[in] block_num  number of block(0-255).
 *
 * @return bool         true if success, or false if falied
 ****************************************************************************************
 */
_CFL_CODE bool codeflash_erase(uint8_t block_num)
{
	bool ret = true;
	uint8_t fsl_ret;
	
	if(codeflash_info.enable == true) {
		/* interrupt disable */
		GLOBAL_INT_DISABLE();

		fsl_ret = FSL_BlankCheck(block_num);
		if(fsl_ret == FSL_OK) {
			/* block is already erased */
		}
		else if(fsl_ret == FSL_ERR_BLANKCHECK) {
			/* erase Block */
			fsl_ret = FSL_Erase(block_num);
		}
		else {
			/* do nothing */
		}
		
		/* interrupt enable */
		GLOBAL_INT_RESTORE();
		
		if(fsl_ret != FSL_OK) {
			ret = false;
		}
	}
	else {
		ret = false;
	}
	
	return ret;
}

/**
 ****************************************************************************************
 * @brief Write data to codeflash rom.
 *
 * @param[in] *data      write data.
 * @param[in] data_size  write data size(4-1024).
 * @param[in] block_num  number of block(0-255).
 *
 * @return bool         true if success, or false if falied
 ****************************************************************************************
 */
_CFL_CODE bool codeflash_write(uint8_t *data, uint16_t data_size, uint8_t block_num)
{
	bool ret = true;
	uint8_t fsl_ret;
	uint8_t i;
	
	if( codeflash_info.enable == true) {
		/* parameter check */
		if((data_size == 0) || (data_size > 1024) || ((data_size & 0x3) != 0)) {
			ret = false;
		}
		else {
			/* interrupt disable */
			GLOBAL_INT_DISABLE();
			
			/* blank check */
			fsl_ret = FSL_BlankCheck(block_num);
			if(fsl_ret != FSL_OK) {
				ret = false;
			}
			else {
				for(i = 0; i < CODEFLASH_WRITE_RETRY_NUM; i++)
				{
					ret = codeflash_write_data(data, data_size, block_num);
					if(ret == true) {
						break;
					}
					else {
						/* write is failed. */
						fsl_ret = FSL_Erase(block_num);
						if(fsl_ret != FSL_OK) {
							break;
						}
					}
				}
				if( (i == CODEFLASH_WRITE_RETRY_NUM) || (fsl_ret != FSL_OK) ) {
					ret = false;
				}
			}
		
			/* interrupt enable */
			GLOBAL_INT_RESTORE();
		}
	}
	else {
		ret = false;
	}
	
	return ret;
}

/**
 ****************************************************************************************
 * @brief Inbert boot flag.
 *
 * @return bool         true if success, or false if falied
 ****************************************************************************************
 */
_CFL_CODE bool codeflash_invertbootflag( void )
{
	bool ret = true;
	uint8_t fsl_ret;
	
	if( codeflash_info.enable == true) {
		/* interrupt disable */
		GLOBAL_INT_DISABLE();
		
		fsl_ret = FSL_InvertBootFlag();
		if( fsl_ret != 0) {
			ret = false;
		}
		
		/* interrupt enable */
		GLOBAL_INT_RESTORE();
	}
	else {
		ret = false;
	}
	
	return ret;
}

/**
 ****************************************************************************************
 * @brief Disable FSL Library.
 *
 * @return bool         true if success, or false if falied
 ****************************************************************************************
 */
_CFL_CODE bool codeflash_disable( void )
{
	bool ret = true;
	
	if( codeflash_info.enable == true) {
		FSL_Close();
		codeflash_info.enable = false;
	}
	else {
		ret = false;
	}
	return ret;
}

/**
 ****************************************************************************************
 * @brief Force reset of the CPU.
 ****************************************************************************************
 */
_CFL_CODE void codeflash_forcereset( void )
{
	FSL_ForceReset();
	
	return;
}

/*
 * Internal FUNCTION DEFINITIONS
 ****************************************************************************************
 */
 
/**
 ****************************************************************************************
 * @brief Write data to codeflash rom.
 *
 * @param[in] *data      write data.
 * @param[in] data_size  write data size(4-1024).
 * @param[in] block_num  number of block(0-255).
 *
 * @return bool         true if success, or false if falied
 ****************************************************************************************
 */
_CFL_CODE bool codeflash_write_data(uint8_t *data, uint16_t data_size, uint8_t block_num)
{
	bool ret = true;
	uint8_t fsl_ret;
	uint8_t write_count;
	fsl_write_t write_param;
	
	uint16_t remained_size;
	uint16_t write_size;
	uint8_t i;
	
	/* write data */
	write_count = (uint8_t)(((data_size - 1) / CODEFLASH_WRITE_DATA_SIZE) + 1);
	remained_size = data_size;
	
	for(i = 0; i < write_count; i++)
	{
		if(remained_size >= CODEFLASH_WRITE_DATA_SIZE) {
			write_size = CODEFLASH_WRITE_DATA_SIZE;
		}
		else {
			write_size = remained_size;
		}
		remained_size -= write_size;
		
		write_param.fsl_data_buffer_p_u08 = &data[i*CODEFLASH_WRITE_DATA_SIZE];
		write_param.fsl_destination_address_u32 = ((uint32_t)block_num * 0x400) + (i * CODEFLASH_WRITE_DATA_SIZE);
		write_param.fsl_word_count_u08 = (uint8_t)(write_size / 4);
		
		fsl_ret = FSL_Write(&write_param);
		if( fsl_ret != FSL_OK ) {
			break;
		}
	}
	if( fsl_ret == FSL_OK ) {
		fsl_ret = FSL_IVerify(block_num);
	}
	
	if(fsl_ret != FSL_OK) {
		ret = false;
	}
	
	return ret;
}
#endif//#ifdef USE_FW_UPDATE_PROFILE

