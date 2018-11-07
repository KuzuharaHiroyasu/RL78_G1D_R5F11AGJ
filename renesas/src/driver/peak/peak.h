/**
 ****************************************************************************************
 *
 * @file peak.h
 *
 * @brief PEAK driver
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */
#ifndef PEAK_H_
#define PEAK_H_

/**
 ****************************************************************************************
 * @addtogroup PEAK
 *
 * @{
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */

/*
 * DEFINES
 ****************************************************************************************
 */

// time from peak (msec)
#define PEAK_TIME_OFF       (0)
#define PEAK_TIME_1         (1)
#define PEAK_TIME_2         (2)
#define PEAK_TIME_4         (4)

// error code
#define PEAK_OK             (0)
#define PEAK_ERROR_PARAM    (1)
#define PEAK_ERROR_STATE    (2)

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
uint8_t peak_init(uint16_t  peak_time);
void    peak_start(void);
void    peak_end(void);

/// @} PEAK

#endif //PEAK_H_
