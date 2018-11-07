/**
 ****************************************************************************************
 *
 * @file dataflash.c
 *
 * @brief DATAFLASH driver
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.205
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup DATAFLASH
 *
 * @{
 ****************************************************************************************
 */

/*
 * COMPILER SETTING
 ****************************************************************************************
 */
#ifdef _USE_IAR_RL78
#define _DFL_DATA _Pragma("location=\"DFL_DATA\"") __NO_INIT
#define _DFL_CODE _Pragma("location=\"DFL_CODE\"")
#define _DFL_CNST _Pragma("location=\"DFL_CNST\"")
#else
#define _DFL_DATA
#define _DFL_CODE
#define _DFL_CNST

#if !defined(_USE_CCRL_RL78)
#pragma section @@DATA  DFL_DATA
#pragma section @@CODEL DFL_CODE
#pragma section @@CNSTL DFL_CNST
#else
#pragma section bss     DFL_DATA
#pragma section text    DFL_CODE
#pragma section const   DFL_CNST
#endif
#endif

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "dataflash.h"
#ifdef CFG_USE_EEL
 #include "fdl.h"                   /* FAL(FDL) Library Header    */
 #include "eel.h"                   /* EEL      Library Header    */
#endif
#if (EEL_TYPE == 1)
#include "fal_descriptor.h"         /* FAL(FDL) Descriptor Header */
#include "eel_descriptor.h"         /* EEL      Descriptor Header */
#else
#include "fdl_descriptor_t02.h"     /* FDL T02  Descriptor Header */
#include "eel_descriptor_t02.h"     /* EEL T02  Descriptor Header */
#endif

#include "plf.h"            /* for using plf_hoco_start, plf_hoco_stop function */
#include "config.h"         /* for using public_addr_ptr pointer macro          */
#include "co_bt.h"          /* for refferring struct bd_addr definition */

/*
 * DEFINES
 ****************************************************************************************
 */
#ifdef CFG_USE_EEL

#if (EEL_TYPE == 2)
/* rapper for EEL Pack01 to Pack02 */
#define fal_status_t            fdl_status_t
#define fal_descriptor_str      fdl_descriptor_str
#define FAL_Init                FDL_Init
#define FAL_Open                FDL_Open
#define FAL_Close               FDL_Close

#define FAL_OK                  FDL_OK
#endif

typedef struct df_errno
{
    uint8_t eel;
    uint8_t df ;
} df_errno_t;

typedef enum
{
    DF_STAT_UNINIT = 0,
    DF_STAT_STOP  ,
    DF_STAT_START ,
    DF_STAT_PROC  
} df_stat_t;
#endif

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
_DFL_CNST __far static const struct bd_addr testadr = CFG_TEST_BDADDR;
#ifdef CFG_USE_EEL
_DFL_DATA static df_stat_t        df_stat;    /* driver status    */
_DFL_DATA static eel_command_t    df_cmd;     /* driver command   */
_DFL_DATA static eel_request_t    df_req;     /* driver parameter */

_DFL_CNST __far static const df_errno_t errno_list[] =
{
    {(uint8_t)EEL_OK                   , DF_OK                   },
    {(uint8_t)EEL_BUSY                 , DF_OK                   },
    {(uint8_t)EEL_ERR_INITIALIZATION   , DF_ERR_INITIALIZATION   },
    {(uint8_t)EEL_ERR_ACCESS_LOCKED    , DF_ERR_ACCESS_LOCKED    },
    #if (EEL_TYPE == 1)
    {(uint8_t)EEL_ERR_COMMAND          , DF_ERR_COMMAND          },
    {(uint8_t)EEL_ERR_PARAMETER        , DF_ERR_PARAMETER        },
    #else
    {(uint8_t)EEL_ERR_VERIFY           , DF_ERR_COMMAND          },
    {(uint8_t)EEL_ERR_PARAMETER        , DF_ERR_PARAMETER        },
    #endif
    {(uint8_t)EEL_ERR_REJECTED         , DF_ERR_REJECTED         },
    {(uint8_t)EEL_ERR_NO_INSTANCE      , DF_ERR_NO_INSTANCE      },
    {(uint8_t)EEL_ERR_POOL_FULL        , DF_ERR_POOL_FULL        },
    {(uint8_t)EEL_ERR_POOL_INCONSISTENT, DF_ERR_POOL_INCONSISTENT},
    {(uint8_t)EEL_ERR_POOL_EXHAUSTED   , DF_ERR_POOL_EXHAUSTED   },
    {(uint8_t)EEL_ERR_INTERNAL         , DF_ERR_INTERNAL         }
};
#endif

/*
 * FUNCTION PROTOTYPES
 ****************************************************************************************
 */
#ifdef CFG_USE_EEL
static uint8_t dataflash_errno(eel_status_t eel_errno);
#if (EEL_TYPE == 2)
static void dataflash_busy_handler(void);
#else
#define dataflash_busy_handler()
#endif
#endif

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
_DFL_CODE uint8_t dataflash_init(void)
{
    #ifdef CFG_USE_EEL
    uint8_t         errno = DF_ERR_UNKNOWN;
    fal_status_t    fal_status; /* parameter for FAL */

    /* initialize parameter for EEL */
    memset(&df_req, 0, sizeof(df_req));

    /* -- INITIALIZE -- (state : uninitialized -> closed) */
    plf_hoco_start();
    /* initialize FAL */
    fal_status = FAL_Init(&fal_descriptor_str);
    if(fal_status == FAL_OK)
    {
        #if (EEL_TYPE == 2)
        /* Enable access for Data Flash memory */
        FDL_Open();
        #endif
        /* initialize EEL */
        df_req.status_enu = EEL_Init();
        if(df_req.status_enu == EEL_OK)
        {
            /* if initialization succeeded, change state */
            df_stat = DF_STAT_STOP;
        }
        errno = dataflash_errno(df_req.status_enu);
    }
    plf_hoco_stop();

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

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
_DFL_CODE uint8_t dataflash_start(const uint8_t mode)
{
    #ifdef CFG_USE_EEL
    uint8_t errno = DF_ERR_DISALLOWED;

    if(df_stat == DF_STAT_STOP)
    {
        /* -- OPEN -- (state : closed -> opened) */
        plf_hoco_start();
        EEL_Open();

        /* -- START -- (state : opened -> started) */
        df_cmd = EEL_CMD_STARTUP;
        df_req.command_enu = EEL_CMD_STARTUP;
        #if (EEL_TYPE == 1)
        df_req.timeout_u08 = mode;
        #endif
        EEL_Execute(&df_req);

        if(mode == DF_MODE_ENFORCED)
        {
            #if (EEL_TYPE == 2)
            dataflash_busy_handler();
            #endif
            /* when DataFlash block is not initialized */
            if(df_req.status_enu == EEL_ERR_POOL_INCONSISTENT)
            {
                /* execute "EEL_CMD_FORMAT" command */
                df_req.command_enu = EEL_CMD_FORMAT;
                #if (EEL_TYPE == 1)
                df_req.timeout_u08 = EEL_MODE_ENFORCED;
                #endif
                EEL_Execute(&df_req);

                #if (EEL_TYPE == 2)
                dataflash_busy_handler();
                #endif
                if(df_req.status_enu == EEL_OK)
                {
                    /* re-execute "EEL_CMD_STARTUP" command */
                    df_req.command_enu = EEL_CMD_STARTUP;
                    #if (EEL_TYPE == 1)
                    df_req.timeout_u08 = EEL_MODE_ENFORCED;
                    #endif
                    EEL_Execute(&df_req);
                    #if (EEL_TYPE == 2)
                    dataflash_busy_handler();
                    #endif
                }
            }
        }

        errno = dataflash_errno(df_req.status_enu);
        if(errno == DF_OK)
        {
            if(mode == DF_MODE_ENFORCED)
            {
                /* if start processing succeeded, change state */
                df_stat = DF_STAT_START;
            }
            else /* mode == DF_MODE_POLLING */
            {
                /* it is neccesary to execute "dataflash_continue", change state */
                df_stat = DF_STAT_PROC;
            }
        }
        else
        {
            /* if a error happened, stop HOCO */
            plf_hoco_stop();
        }
    }

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

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
_DFL_CODE uint8_t dataflash_stop(const uint8_t mode)
{
    #ifdef CFG_USE_EEL
    uint8_t errno = DF_ERR_DISALLOWED;

    if(df_stat == DF_STAT_START)
    {
        /* -- STOP -- (state : started -> opened) */
        df_cmd = EEL_CMD_SHUTDOWN;
        df_req.command_enu = EEL_CMD_SHUTDOWN;
        #if (EEL_TYPE == 1)
        df_req.timeout_u08 = mode;
        #endif
        EEL_Execute(&df_req);

        errno = dataflash_errno(df_req.status_enu);
        if(errno == DF_OK)
        {
            if(mode == DF_MODE_ENFORCED)
            {
                #if (EEL_TYPE == 2)
                #if 0 // EEL_CMD_SHUTDOWN no need EEL_Handler()
                dataflash_busy_handler();
                #endif
                #endif

                /* -- CLOSE -- (state : opened -> closed) */
                EEL_Close();
                plf_hoco_stop();

                /* if stop processing succeeded, change state */
                df_stat = DF_STAT_STOP;
            }
            else /* mode == DF_MODE_POLLING */
            {
                /* it is neccesary to execute "dataflash_continue", change state */
                df_stat = DF_STAT_PROC;
            }
        }
    }

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

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
_DFL_CODE uint8_t dataflash_rw(const uint8_t mode, const df_rw_t rw, const uint8_t id, uint8_t* addr)
{
    #ifdef CFG_USE_EEL
    uint8_t errno;

    if(df_stat == DF_STAT_START)
    {
        df_cmd = (eel_command_t)((rw == DF_READ) ? (EEL_CMD_READ) : (EEL_CMD_WRITE));
        df_req.address_pu08   = addr;
        df_req.identifier_u08 = id;
        df_req.command_enu    = df_cmd;
        #if (EEL_TYPE == 1)
        df_req.timeout_u08    = mode;
        #endif
        EEL_Execute(&df_req);

        errno = dataflash_errno(df_req.status_enu);
        if(errno == DF_OK)
        {
            if(mode == DF_MODE_POLLING)
            {
                /* it is neccesary to execute "dataflash_continue", change state */
                df_stat = DF_STAT_PROC;
            }
            #if (EEL_TYPE == 2)
            else
            {
                dataflash_busy_handler();
            }
            #endif
        }
    }
    else if(df_stat == DF_STAT_PROC)
    {
        errno = DF_ERR_BUSY;
    }
    else
    {
        errno = DF_ERR_DISALLOWED;
    }

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

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
_DFL_CODE uint8_t dataflash_cleanup(const uint8_t mode)
{
    #ifdef CFG_USE_EEL
    uint8_t errno;

    if(df_stat == DF_STAT_START)
    {
        #if (EEL_TYPE == 1)
        df_cmd = EEL_CMD_CLEANUP;
        df_req.command_enu = EEL_CMD_CLEANUP;
        df_req.timeout_u08 = mode           ;
        #else
        df_cmd = EEL_CMD_REFRESH;
        df_req.command_enu = EEL_CMD_REFRESH;
        #endif
        EEL_Execute(&df_req);

        errno = dataflash_errno(df_req.status_enu);
        if(errno == DF_OK)
        {
            if(mode == DF_MODE_POLLING)
            {
                /* it is neccesary to execute "dataflash_continue", change state */
                df_stat = DF_STAT_PROC;
            }
            #if (EEL_TYPE == 2)
            else
            {
                dataflash_busy_handler();
            }
            #endif
        }
    }
    else if(df_stat == DF_STAT_PROC)
    {
        errno = DF_ERR_BUSY;
    }
    else
    {
        errno = DF_ERR_DISALLOWED;
    }

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

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
_DFL_CODE uint8_t dataflash_format(const uint8_t mode)
{
    #ifdef CFG_USE_EEL
    uint8_t errno;

    if((df_stat == DF_STAT_STOP) || (df_stat == DF_STAT_START))
    {
        if(df_stat == DF_STAT_STOP)
        {
            plf_hoco_start();
            EEL_Open();
        }
        df_cmd = EEL_CMD_FORMAT;
        df_req.command_enu = EEL_CMD_FORMAT;
        #if (EEL_TYPE == 1)
        df_req.timeout_u08 = mode          ;
        #endif
        EEL_Execute(&df_req);

        errno = dataflash_errno(df_req.status_enu);
        if(errno == DF_OK)
        {
            if(mode == DF_MODE_ENFORCED)
            {
                #if (EEL_TYPE == 2)
                dataflash_busy_handler();
                #endif
                /* -- CLOSE -- (state : opened -> closed) */
                EEL_Close();
                plf_hoco_stop();

                /* if format finished, change state */
                df_stat = DF_STAT_STOP;
            }
            else /* mode == DF_MODE_POLLING */
            {
                /* it is neccesary to execute "dataflash_continue", change state */
                df_stat = DF_STAT_PROC;
            }
        }
    }
    else if(df_stat == DF_STAT_PROC)
    {
        errno = DF_ERR_BUSY;
    }
    else
    {
        errno = DF_ERR_DISALLOWED;
    }

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

/**
 ****************************************************************************************
 * @brief   continue to execute processing
 *
 * @param[in] fin_flg   flag indicating whether processing finished or not
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
_DFL_CODE uint8_t dataflash_continue(bool* fin_flg)
{
    #ifdef CFG_USE_EEL
    uint8_t errno = DF_ERR_DISALLOWED;
    #if (EEL_TYPE == 1)
    eel_driver_status_t  drvstat;
    #endif

    if(df_stat == DF_STAT_PROC)
    {
        #if (EEL_TYPE == 1)
        EEL_GetDriverStatus(&drvstat);
        if(drvstat.operationStatus_enu == EEL_OPERATION_BUSY)
        {
            EEL_Handler(EEL_MODE_POLLING);
            EEL_GetDriverStatus(&drvstat);
        }
        #else // EEL_TYPE == 2
        if( df_req.status_enu == EEL_BUSY )
        {
            EEL_Handler();
        }
        #endif
        /* EEL_Execute retains the pointer of df_req, and EEL_Handler updates df_req */
        errno = dataflash_errno(df_req.status_enu);

        #if (EEL_TYPE == 1)
        if(drvstat.operationStatus_enu == EEL_OPERATION_IDLE)
        {
            /* if below processing finished, pass here */
            /*     -> START, READ, WRITE, CLEANUP      */
            *fin_flg = true;
            df_stat = DF_STAT_START;
        }
        else if(drvstat.operationStatus_enu == EEL_OPERATION_PASSIVE)
        {
            /* -- CLOSE -- (state : opened -> closed) */
            EEL_Close();
            plf_hoco_stop();

            /* if below processing finished, pass here */
            /*     -> STOP, FORMAT                     */
            *fin_flg = true;
            df_stat = DF_STAT_STOP;
        }
        else /* drvstat.operationStatus_enu == EEL_OPERATION_BUSY */
        {
            /* processing unfinished */
            *fin_flg = false;
        }
        #else // EEL_TYPE == 2
        if( df_req.status_enu != EEL_BUSY )
        {
            /* processing finished */
            *fin_flg = true;

            switch( df_cmd )
            {
                case EEL_CMD_SHUTDOWN:
                case EEL_CMD_FORMAT:
                    /* -- CLOSE -- (state : opened -> closed) */
                    EEL_Close();
                    plf_hoco_stop();

                    /* if below processing finished, pass here */
                    /*     -> STOP, FORMAT                     */
                    df_stat = DF_STAT_STOP;
                    break;
                default:
                    /* if below processing finished, pass here */
                    /*     -> START, READ, WRITE, CLEANUP      */
                    df_stat = DF_STAT_START;
                    break;
            }
        }
        else
        {
            /* processing unfinished */
            *fin_flg = false;
        }
        #endif
    }

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

/**
 ****************************************************************************************
 * @brief   get size of available space
 *
 * @param[out] wsize    size of available space
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
_DFL_CODE uint8_t dataflash_get_space(uint16_t* wsize)
{
    #ifdef CFG_USE_EEL
    uint8_t errno = DF_ERR_DISALLOWED;
    eel_status_t status;

    if(df_stat == DF_STAT_START)
    {
        status = EEL_GetSpace((eel_u16*)wsize);
        *wsize = (status == EEL_OK) ? (*wsize) : (0);

        errno = dataflash_errno(status);
    }

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

/**
 ****************************************************************************************
 * @brief   get EEL version string
 *
 * @param[out] ver      version string buffer
 *
 * @return  uint8_t     error code
 ****************************************************************************************
 */
_DFL_CODE uint8_t dataflash_getver(uint8_t* ver)
{
    #ifdef CFG_USE_EEL
    uint8_t  errno = DF_ERR_DISALLOWED;
    uint8_t  len;
    uint8_t __far*  string;

    if((df_stat == DF_STAT_STOP) || (df_stat == DF_STAT_START))
    {
        string = EEL_GetVersionString();

        /* copy string to buffer */
        for(len = 0 ; len < 24; len++)
        {
            ver[len] = string[len];
            if(ver[len] == '\0')
            {
                break;
            }
        }
        /* fill by zero after end of string */
        for(len = (uint8_t)(len + 1); len < 24; len++)
        {
            ver[len] = '\0';
        }
        errno = DF_OK;
    }

    return errno;
    #else
    return DF_ERR_UNSUPPORTED;
    #endif
}

/**
 ****************************************************************************************
 * @brief   inform whether dataflash is stopped or not
 *          (if dataflash is started, prohibit STOP/HALT of MCU)
 *
 * @return  bool    true: stopped, false: not stopped (started)
 ****************************************************************************************
 */
_DFL_CODE bool dataflash_is_stopped(void)
{
    #ifdef CFG_USE_EEL
    return ((df_stat == DF_STAT_UNINIT) || (df_stat == DF_STAT_STOP)) ? (true) : (false);
    #else
    return true;
    #endif
}

/**
 ****************************************************************************************
 * @brief   convert from eel error code to dataflash driver error code
 *
 * @param[in] eel_errno     eel error code
 *
 * @return  uint8_t         error code
 ****************************************************************************************
 */
#ifdef CFG_USE_EEL
_DFL_CODE static uint8_t dataflash_errno(const eel_status_t eel_errno)
{
    uint8_t idx;

    for(idx = 0; idx < (sizeof(errno_list) / sizeof(errno_list[0])); idx++)
    {
        if(errno_list[idx].eel == eel_errno)
        {
            return errno_list[idx].df;
        }
    }
    return DF_ERR_UNKNOWN;
}

#if (EEL_TYPE == 2)
/**
 ****************************************************************************************
 * @brief   execute EEL_Handler for busy state (EEL Pack02)
 *
 * @return  none
 ****************************************************************************************
 */
_DFL_CODE static void dataflash_busy_handler(void)
{
    while(df_req.status_enu == EEL_BUSY)
    {
        EEL_Handler();
    }
}
#endif //#if (EEL_TYPE == 2)
#endif

/**
 ****************************************************************************************
 * @brief   get device address from DataFlash
 *          (if device address does not exist in DataFlash, get from CodeFlash)
 *
 * @param[out] bda      device address
 *
 ****************************************************************************************
 */
_DFL_CODE void flash_get_bda(struct bd_addr* bda)
{
    uint8_t ii;
    bool    invalid = true;

    /* try to get device address from DataFlash */
    #ifdef CFG_USE_EEL
    if(dataflash_start(DF_MODE_ENFORCED) == DF_OK)
    {
        /* get BD_address(Public Device Address) from DataFlash */
        if(dataflash_rw(DF_MODE_ENFORCED, DF_READ, EEL_ID_BDA, (uint8_t*)bda) == DF_OK)
        {
            /* check if device address is valid(not all 0xFF) */
            for(ii = 0; ii < BD_ADDR_LEN; ii++)
            {
                if(bda->addr[ii] != 0xFF)
                {
                    invalid = false;
                    break;
                }
            }
        }
        dataflash_stop(DF_MODE_ENFORCED);
    }
    #endif

    if(invalid)
    {
        /* get device adddress from CodeFlash */
        for(ii = 0; ii < BD_ADDR_LEN; ii++)
        {
            bda->addr[ii] = public_addr_ptr->addr[ii];
            /* check if device address is valid(not all 0xFF) */
            if(bda->addr[ii] != 0xFF)
            {
                invalid = false;
            }
        }
    }

    if(invalid)
    {
        /* copy test device address */
        *bda = testadr;
    }
}
