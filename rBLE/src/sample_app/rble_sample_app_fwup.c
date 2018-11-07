/**
 ****************************************************************************************
 *
 * @file        rble_sample_app_fwup.c
 *
 * @brief   rBLE Sample Application
 *
 * Copyright(C) 2016 Renesas Electronics Corporation
 *
 * $Rev: 9242 $
 *
 ****************************************************************************************
 */
/**
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwble_config.h"
#include "arch.h"

#ifdef CFG_SAMPLE
#ifdef CONFIG_EMBEDDED

#include    <stdio.h>
#include    <string.h>
#include    <stdlib.h>

#include    "rble.h"
#include    "rble_app.h"
#include    "menu_sel.h"
#include    "console.h"

#include    "prf_sel.h"

#if (defined(USE_FW_UPDATE_PROFILE) && defined(WIN32))

/**
 * DEFINES
 ****************************************************************************************
 */


/**
 * TYPE DEFINES
 ****************************************************************************************
 */


/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

#if defined(USE_FW_UPDATE_PROFILE)&&defined(WIN32)
BOOL RBLE_FW_UPDATE_Test( void );                                                    /* FW update start */

extern RBLE_STATUS FW_Update_Sender_Start( uint8_t file_num, uint8_t start_area );
#endif

/**
 * VARIABLE DECLARATIONS
 ****************************************************************************************
 */


/******************************* Program Area *****************************************/
/******************************************************************************************/
/**
 *  @brief Firmware Update start command is executed. 
 *
 *
 *  @attention
 *  
 *  @b Contents of management:
 *
 *  @param[in]
 *
 *  @retval TRUE     Menu Continue
 */
/******************************************************************************************/
BOOL RBLE_FW_UPDATE_Test()
{
    uint8_t file_num = 0;
    uint8_t start_area = 0;
    
    if ( 0 < RBLE_Parameter_Cnt ) {
        if ( 1 < RBLE_Parameter_Cnt ) {
            start_area = (uint8_t)RBLE_Parameter_Arg[ 1 ];
        }
        file_num = (uint8_t)RBLE_Parameter_Arg[ 0 ];
    }
            
    /* start fw update */
    FW_Update_Sender_Start(file_num, start_area);
    
    return( TRUE );
}

#endif  //#if (defined(USE_FW_UPDATE_PROFILE) && defined(WIN32))
#endif  /* CONFIG_EMBEDDED */
#endif  /* CFG_SAMPLE */
