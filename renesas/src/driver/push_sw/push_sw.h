/**
 ****************************************************************************************
 *
 * @file push_sw.h
 *
 * @brief Push Switch Driver
 *
 * Copyright (C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: 0000 $
 *
 ****************************************************************************************
 */
#ifndef PUSH_SW_H_
#define PUSH_SW_H_

/**
 * TYPE DEFINES
 ****************************************************************************************
 */
typedef	void ( *PUSH_CB )( void );

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */
void push_sw2_start( PUSH_CB cb );

#endif /* PUSH_SW_H_ */
