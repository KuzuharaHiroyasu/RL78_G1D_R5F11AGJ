/**
 ****************************************************************************************
 *
 * @file        uart.c
 *
 * @brief       UART Driver - UART0/UART1 of RL78 RENESAS 16b MCU.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.209
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup UART
 *
 * @{
 ****************************************************************************************
 */

/*
 * UART CHANNEL SETTING
 ****************************************************************************************
 */
#define UART_CHANNEL    (0)  // 0=UART0, Other=UART1


#ifdef _USE_REL_RL78
#pragma interrupt INTDMA0 dma0_isr NOBANK
#pragma interrupt INTDMA1 dma1_isr NOBANK
#if (UART_CHANNEL == 0)
#pragma interrupt INTST0  st0_isr
#pragma interrupt INTSRE0 sre0_isr
#else // UART_CHANNEL
#pragma interrupt INTST1  st0_isr
#pragma interrupt INTSRE1 sre0_isr
#endif
#pragma section @@DATA  UARTDATA
#pragma section @@CODEL UARTCODE
#pragma section @@BASE  UARTBASE

#define _UARTCODE 
#define _UARTDATA
#define _UARTBASE
#elif defined(_USE_CCRL_RL78)
#include "iodefine.h"
#pragma interrupt dma0_isr (vect=INTDMA0)
#pragma interrupt dma1_isr (vect=INTDMA1)
#if (UART_CHANNEL == 0)
#pragma interrupt st0_isr  (vect=INTST0)
#pragma interrupt sre0_isr (vect=INTSRE0)
#else // UART_CHANNEL
#pragma interrupt st0_isr  (vect=INTST1)
#pragma interrupt sre0_isr (vect=INTSRE1)
#endif

#pragma section bss     UARTDATA
#pragma section text    UARTCODE
#pragma section const   UARTCNST

#define _UARTCODE 
#define _UARTDATA
#define _UARTBASE
#else
#define _UARTDATA _Pragma("location=\"UARTDATA\"") __NO_INIT
#define _UARTCODE _Pragma("location=\"UARTCODE\"")
#define _UARTBASE _Pragma("location=\"UARTBASE\"")

#endif

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"

#if __DTM2WIRE_UART_USE__
#include    "DTM2WIRE.h"
#endif

#include "hw_config.h"
#include "port.h"
#include "uart.h"

#if ((!SERIAL_U_2WIRE && !SERIAL_U_3WIRE && !SERIAL_U_DIV_2WIRE) || \
     (SERIAL_U_2WIRE && SERIAL_U_3WIRE    )                      || \
     (SERIAL_U_2WIRE && SERIAL_U_DIV_2WIRE)                      || \
     (SERIAL_U_3WIRE && SERIAL_U_DIV_2WIRE)                      || \
      SERIAL_C_4WIRE                                             || \
      SERIAL_C_5WIRE                                              )
 #error "Serial setting in serial.h is wrong."
#endif

#if (UART_CHANNEL != 0) && (SERIAL_U_2WIRE) && !defined(CONFIG_EMBEDDED)
 #error "UART1 does not support the SNOOZE mode."
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
///transmit request code
#define UART_REQ_BYTE  (0xC0)

///transmit acknowledge code
#define UART_ACK_BYTE  (0x88)

#define IRQ_SET     1

#if (UART_CHANNEL == 0)         // for UART0

#define UART_VAL_NFEN       0x01U
#define UART_VAL_ST         0x03U
#define UART_VAL_SS         0x03U
#if defined(CLK_FCLK_4MHZ)
#define UART_VAL_SPS_1MHZ   0x02U
#define UART_VAL_SPS_2MHZ   0x01U
#elif defined(CLK_FCLK_8MHZ)
#define UART_VAL_SPS_1MHZ   0x03U
#define UART_VAL_SPS_2MHZ   0x02U
#define UART_VAL_SPS_4MHZ   0x01U
#define UART_VAL_SPS_8MHZ   0x00U
#elif defined(CLK_FCLK_16MHZ)
#define UART_VAL_SPS_1MHZ   0x04U
#define UART_VAL_SPS_2MHZ   0x03U
#define UART_VAL_SPS_4MHZ   0x02U
#define UART_VAL_SPS_8MHZ   0x01U
#elif defined(CLK_FCLK_32MHZ)
#define UART_VAL_SPS_1MHZ   0x05U
#define UART_VAL_SPS_2MHZ   0x04U
#endif
#define UART_VAL_TXD_SMR    0x0022U
#define UART_VAL_RXD_SMR    0x0122U
#define UART_VAL_SOL        0x00U
#define UART_VAL_SOE        0x01U
#define UART_VAL_TX_INT     DMA_SRC_INTST0
#define UART_VAL_RX_INT     DMA_SRC_INTSR0
#define UART_TXD_ADDR       0x10U
#define UART_RXD_ADDR       0x12U
        
#define UART_TXD_PM         PM1
#define UART_TXD_POM        POM1
#define UART_TXD_P          P1
#define UART_TXD_n          2
#define UART_RXD_PM         PM1
#define UART_RXD_PIM        PIM1
#define UART_RXD_PU         PU1
#define UART_RXD_n          1
        
#define UART_NFEN           NFEN0
#define UART_TXD_SDR_L      TXD0
#define UART_RXD_SDR_L      RXD0
#define UART_TXD_SDR        SDR00
#define UART_RXD_SDR        SDR01
#define UART_TXD_SMR        SMR00
#define UART_RXD_SMR        SMR01
#define UART_TXD_SCR        SCR00
#define UART_RXD_SCR        SCR01
#define UART_TXD_SIR_L      SIR00L
#define UART_RXD_SIR_L      SIR01L
#define UART_TXD_SSR_L      SSR00L
#define UART_RXD_SSR_L      SSR01L
#define UART_STPR0          STPR00
#define UART_STPR1          STPR10
#define UART_SRIF           SRIF0
#define UART_SREIF          SREIF0
#define UART_STIF           STIF0
#define UART_STMK           STMK0
#define UART_SRMK           SRMK0
#define UART_SREMK          SREMK0



#else                               // for UART1



#define UART_VAL_NFEN       0x04U
#define UART_VAL_ST         0x0CU
#define UART_VAL_SS         0x0CU
#if defined(CLK_FCLK_4MHZ)
#define UART_VAL_SPS_1MHZ   0x20U
#define UART_VAL_SPS_2MHZ   0x10U
#elif defined(CLK_FCLK_8MHZ)
#define UART_VAL_SPS_1MHZ   0x30U
#define UART_VAL_SPS_2MHZ   0x20U
#elif defined(CLK_FCLK_16MHZ)
#define UART_VAL_SPS_1MHZ   0x40U
#define UART_VAL_SPS_2MHZ   0x30U
#elif defined(CLK_FCLK_32MHZ)
#define UART_VAL_SPS_1MHZ   0x50U
#define UART_VAL_SPS_2MHZ   0x40U
#endif
#define UART_VAL_TXD_SMR    0x8022U
#define UART_VAL_RXD_SMR    0x8122U
#define UART_VAL_SOL        0x00U
#define UART_VAL_SOE        0x04U
#define UART_VAL_TX_INT     DMA_SRC_INTST1
#define UART_VAL_RX_INT     DMA_SRC_INTSR1
#define UART_TXD_ADDR       0x44U
#define UART_RXD_ADDR       0x46U
        
#define UART_TXD_PM         PM0
#define UART_TXD_POM        POM0
#define UART_TXD_P          P0
#define UART_TXD_n          2
#define UART_RXD_PM         PM0
#define UART_RXD_PIM        PIM0
#define UART_RXD_PU         PU0
#define UART_RXD_n          3

#define UART_NFEN           NFEN0
#define UART_TXD_SDR_L      TXD1
#define UART_RXD_SDR_L      RXD1
#define UART_TXD_SDR        SDR02
#define UART_RXD_SDR        SDR03
#define UART_TXD_SMR        SMR02
#define UART_RXD_SMR        SMR03
#define UART_TXD_SCR        SCR02
#define UART_RXD_SCR        SCR03
#define UART_TXD_SIR_L      SIR02L
#define UART_RXD_SIR_L      SIR03L
#define UART_TXD_SSR_L      SSR02L
#define UART_RXD_SSR_L      SSR03L
#define UART_STPR0          STPR01
#define UART_STPR1          STPR11
#define UART_SRIF           SRIF1
#define UART_SREIF          SREIF1
#define UART_STIF           STIF1
#define UART_STMK           STMK1
#define UART_SRMK           SRMK1
#define UART_SREMK          SREMK1

#endif  // UART0/UART1

enum
{
    T_IDLE      = 0,
    T_SUSPENDED = 1,
    T_ACTIVE    = 2
};

enum
{
    R_IDLE      = 0,
    R_ACK       = 1,
    R_ACTIVE    = 2
};


#define UART_CALLBACK_VOID(cb)        SERIAL_CALLBACK_VOID(cb)
#define UART_CALLBACK_BOOL(cb)        SERIAL_CALLBACK_BOOL(cb)


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
#if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
_UARTDATA static uint8_t  tx_stat;
_UARTDATA static uint8_t  rx_stat;
#endif
_UARTDATA static bool stop_flg;
#if SERIAL_U_DIV_2WIRE
_UARTDATA static uint16_t rx_addr;
_UARTDATA static uint16_t rx_size;
_UARTDATA static uint8_t *rx_ptr;
#endif

_UARTDATA static SERIAL_EVENT_PARAM uart_callback;
#if __DTM2WIRE_UART_USE__
_UARTDATA volatile RBLE_RUNNING_MODE rBLE_Run_Mode_Flg;
#endif

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief Initializes the port bits, port mode register and pull-ups or latches for UART
 *        pins.
 *****************************************************************************************
 */
_UARTCODE static void uart_port_init(void)
{
    /* TX pin */
    /* set pin as output */
    write1_sfr(UART_TXD_PM, UART_TXD_n, PORT_OUTPUT);
    /* set output mode */
    write1_sfr(UART_TXD_POM, UART_TXD_n, PORT_OUTPUT_NORMAL);
    /* latch necessary for output pin */
    write1_sfr(UART_TXD_P, UART_TXD_n, PORT_LATCH);

    /* RX pin */
    /* Use of noise filter of RXDn pin */
    write_sfr(UART_NFEN, (uint8_t)UART_VAL_NFEN);
    /* set pin as input */
    write1_sfr(UART_RXD_PM, UART_RXD_n, PORT_INPUT);
    /* set input mode */
    write1_sfr(UART_RXD_PIM, UART_RXD_n, PORT_INPUT_NORMAL);
    /* set pull-up */
    write1_sfr(UART_RXD_PU, UART_RXD_n, PORT_PULLUP);
}

/**
 ****************************************************************************************
 * @brief Initialize a DMA0 channel for transmission from UART0/1 data register to RAM.
 *****************************************************************************************
 */
_UARTCODE static void dma0_init(void)
{
    /* enable DMA0 */
    write1_sfrbit(DEN0, 1);

    /* set SFR address (UART data registers) */
    write_sfr(DSA0, UART_RXD_ADDR);

    /* Transfer mode */
    write1_sfrbit(DRS0, DMA_SFR_TO_RAM);
    write1_sfrbit(DS0, DMA_UNIT_8B);
    write1_sfrbit(DWAIT0, DMA_TF_NO_PEND);

    /* Trigger setting- set to UART0/1 RX interrupt */
    write_sfr(DMC0, (uint8_t)(read_sfr(DMC0) | (UART_VAL_RX_INT & 0x0FU)));

    /* clear flag and unmask the interrupt */
    write1_sfrbit(DMAIF0, IRQ_CLEAR);
    write1_sfrbit(DMAMK0, IRQ_ENABLE);
}

/**
 ****************************************************************************************
 * @brief Initialize a DMA1 channel for transmission from RAM to UART0/1 data register.
 *****************************************************************************************
 */
_UARTCODE static void dma1_init(void)
{
    /* enable DMA1 */
    write1_sfrbit(DEN1, 1);

    /* set SFR address (UART data register) */
    write_sfr(DSA1, UART_TXD_ADDR);

    /* Transfer mode */
    write1_sfrbit(DRS1, DMA_RAM_TO_SFR);
    write1_sfrbit(DS1, DMA_UNIT_8B);
    write1_sfrbit(DWAIT1, DMA_TF_NO_PEND);

    /* Trigger setting - UART0/1 TX transfer end interrupt */
    write_sfr(DMC1, (uint8_t)(read_sfr(DMC1) | (UART_VAL_TX_INT & 0x0FU)));

    /* clear flag and unmask the interrupt */
    write1_sfrbit(DMAIF1, IRQ_CLEAR);
    write1_sfrbit(DMAMK1, IRQ_ENABLE);
}

/**
 ****************************************************************************************
 * @brief Initialize UART communication.(including initializing DMA and interrupt) 
 *        If argument value is wrong, this function does nothing.
 *****************************************************************************************
 */
_UARTCODE void serial_init(SERIAL_EVENT_PARAM *param)
{
    uart_callback = *param;

    /* stop UART */
    write_sfr(ST0L , UART_VAL_ST);

    /* PER0 peripheral enable register - release SAU0 from reset and start clock supply */
    write1_sfr2bit(SAU0EN, 1);

    /* must wait 4 clocks after PER0 setting */
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();

#if __DTM2WIRE_UART_USE__
    if ( _RBLE_RUN_MODE_NORMAL != rBLE_Run_Mode_Flg ) {
        /* MCK = fclk/4 = 2MHz */
        write_sfr(SPS0L, (uint8_t)((read_sfr(SPS0L) | UART_VAL_SPS_2MHZ)));

        /* baudrate 9600bps(when MCK = 2MHz) */
        write_sfrp(UART_TXD_SDR, (uint16_t)0xCE00U);
        write_sfrp(UART_RXD_SDR, (uint16_t)0xCE00U);
    } else {
#endif
      #if (1)
        #ifndef CONFIG_EMBEDDED
        /* MCK = fclk/n = 1MHz */
        write_sfr(SPS0L, (uint8_t)((read_sfr(SPS0L) | UART_VAL_SPS_1MHZ)));

        /* baudrate 4800bps(when MCK = 1MHz) */
        write_sfrp(UART_TXD_SDR, (uint16_t)0xCE00U);
        write_sfrp(UART_RXD_SDR, (uint16_t)0xCE00U);
        #else /*CONFIG_EMBEDDED*/
        /* MCK = fclk/n = 2MHz */
//        write_sfr(SPS0L, (uint8_t)((read_sfr(SPS0L) | UART_VAL_SPS_2MHZ)));
		
		//RD8001対応:ボーレート変更
		#if 0
        /* baudrate 9600bps(when MCK = 2MHz) */
        write_sfrp(UART_TXD_SDR, (uint16_t)0xCE00U);
        write_sfrp(UART_RXD_SDR, (uint16_t)0xCE00U);
		#else
        /* baudrate 250000bps(when MCK = 2MHz) */
		#if defined(CLK_FCLK_8MHZ)
		// 19200bps
        write_sfr(SPS0L, (uint8_t)((read_sfr(SPS0L) | UART_VAL_SPS_4MHZ)));
		write_sfrp(UART_TXD_SDR, (uint16_t)0xCE00U);
		write_sfrp(UART_RXD_SDR, (uint16_t)0xCE00U);
		#else
		//16MHz時
//        write_sfr(SPS0L, (uint8_t)((read_sfr(SPS0L) | UART_VAL_SPS_4MHZ)));	//9600bps
        write_sfr(SPS0L, (uint8_t)((read_sfr(SPS0L) | UART_VAL_SPS_8MHZ)));	//19200bps
		write_sfrp(UART_TXD_SDR, (uint16_t)0xCE00U);
		write_sfrp(UART_RXD_SDR, (uint16_t)0xCE00U);
		#endif
		#endif
        #endif /*CONFIG_EMBEDDED*/
      #else
        /* MCK = fclk/n = 2MHz */
        write_sfr(SPS0L, (uint8_t)((read_sfr(SPS0L) | UART_VAL_SPS_2MHZ)));
        /* baudrate 250000bps(when MCK = 2MHz) */
        write_sfrp(UART_TXD_SDR, (uint16_t)0x0600U);
        write_sfrp(UART_RXD_SDR, (uint16_t)0x0600U);
      #endif
#if __DTM2WIRE_UART_USE__
    }
#endif

    /* set stop permission */
    #if SERIAL_U_2WIRE
    #if (1)
      #ifndef CONFIG_EMBEDDED
      /* if baudrate is 4800bps, set enable */
      stop_flg = true;
      #else /*CONFIG_EMBEDDED*/
      /* if baudrate is over than 4800bps, set disable */
      stop_flg = true;		//RD8001暫定
      #endif /*CONFIG_EMBEDDED*/
    #else
    /* if baudrate is over than 4800bps, set disable */
    stop_flg = false;
    #endif
    #else
    /* initialize stop permission, always enable */
    stop_flg = true;
    #endif

    /* set operation mode for TX channel 0 of SAU0 */
    write_sfrp(UART_TXD_SMR, (uint16_t)UART_VAL_TXD_SMR);

    /* set operation mode for RX channel 1 of SAU0 */
    write_sfrp(UART_RXD_SMR, (uint16_t)UART_VAL_RXD_SMR);

	//RD8001対応:通信設定変更
	#if 0
    /* Serial communication operation settings */
    write_sfrp(UART_TXD_SCR, (uint16_t)0x8097U);		// 8bit,パリティなし,LSB
    write_sfrp(UART_RXD_SCR, (uint16_t)0x4497U);		// 8bit,パリティなし,LSB
    #else
    /* Serial communication operation settings */
    write_sfrp(UART_TXD_SCR, (uint16_t)0x8297U);		// 8bit,偶数パリティ,LSB
    write_sfrp(UART_RXD_SCR, (uint16_t)0x4697U);		// 8bit,偶数パリティ,LSB
    #endif

    /* Serial output level */
    write_sfr(SOL0L, UART_VAL_SOL);

    /* Serial output - initial value to be output */
    write_sfrp(SO0, (uint16_t)0x0f0fU);

    /* Serial output enable for the targeted channel */
    write_sfr(SOE0L, (uint8_t)(read_sfr(SOE0L) | UART_VAL_SOE));

    /* set port and port mode */
    uart_port_init();

    /* clear all error */
    write_sfr(UART_RXD_SIR_L, 0x07);

    /* priority order: dma1_isr > st0_isr > wakeup_isr > dma0_isr */
    /* set interrupt priority as level2 */
    write1_sfrbit(UART_STPR1 , 1);
    write1_sfrbit(UART_STPR0 , 0);
    write1_sfrbit(DMAPR11, 1);
    write1_sfrbit(DMAPR01, 0);

    /* clear interrupt request flags */
    write1_sfrbit(UART_SRIF, IRQ_CLEAR);
    write1_sfrbit(UART_SREIF, IRQ_CLEAR);

    /* mask interrupts */
    write1_sfrbit(UART_STMK, IRQ_DISABLE);
    write1_sfrbit(UART_SRMK, IRQ_DISABLE);
    write1_sfrbit(UART_SREMK, IRQ_ENABLE);

    /* initialize DMA0, and set DMA transfer setting */
    dma0_init();

    /* initialize DMA1, and set DMA transfer setting */
    dma1_init();

    #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
    tx_stat = T_IDLE;
    rx_stat = R_IDLE;
    #endif

    /* start Serial Array Unit 0 channel 0 and 1 operation */
    write_sfr(SS0L, (uint8_t)(read_sfr(SS0L) | UART_VAL_SS));

    return;
}

/**
 ****************************************************************************************
 * @brief Program a DMA transfer about destination address and size.
 *
 * @param[in] dest_addr 16b RAM address, start of the space where data is transfered to.
 * @param[in] size      Transfer size, to set to DMA Byte Count register which counts down.
 *****************************************************************************************
 */
#define uart_dma_rx(dest_addr, size)    \
{                                       \
    /* stop DMA0 */                     \
    write1_sfrbit(DST0, 0);             \
                                        \
    /* set RAM address */               \
    write_sfrp(DRA0, (dest_addr));      \
                                        \
    /* set transfer size */             \
    write_sfrp(DBC0, (size));           \
                                        \
    /* start DMA0 */                    \
    write1_sfrbit(DST0, 1);             \
}

/**
 ****************************************************************************************
 * @brief Program a DMA transfer about source address and size.
 *
 * @param[in] src_addr  16b RAM address, start of the space where data is transfered from.
 * @param[in] size      Transfer size, to set to DMA Byte Count register which counts down.
 *****************************************************************************************
 */
#define uart_dma_tx(src_addr, size)     \
{                                       \
    /* stop DMA1 */                     \
    write1_sfrbit(DST1, 0);             \
                                        \
    /* set RAM address */               \
    write_sfrp(DRA1, (src_addr));       \
                                        \
    /* set transfer size */             \
    write_sfrp(DBC1, (size));           \
}

/**
 ****************************************************************************************
 * @brief Start UART receiving data.
 * When finished receiving data, calls callback function.
 *
 * @param[in,out] bufptr  destination pointer, start of the space where data is transfered to.
 * @param[in] size        Transfer size, receive data size byte by UART.
 *****************************************************************************************
 */
_UARTCODE void serial_read(uint8_t *bufptr, const uint16_t size)
{
	uint8_t BEF = read_sfr(UART_RXD_SSR_L);
	
	if((BEF & 0x20) == 0x20)
	{
	    #if SERIAL_U_DIV_2WIRE
	    /* store the argument parameter */
	    rx_addr = (uint16_t)bufptr;
	    rx_size = size;
	    rx_ptr = bufptr;
	    #endif

	    /* start DMA0 */
	#if 0
	    uart_dma_rx((uint16_t)bufptr, size);
	#else
		*bufptr = RXD0;
	#endif
	}
}

/**
 ****************************************************************************************
 * @brief Start UART transferring data.
 *        When finished receiving data, calls callback function.
 *
 * @param[in] bufptr  destination pointer, start of the space where data is transfered to.
 * @param[in] size    Transfer size, transferred data size byte by UART.
 *****************************************************************************************
 */
_UARTCODE void serial_write(const uint8_t *bufptr, const uint16_t size)
{
#if 1
    /* start DMA1 */
    uart_dma_tx((uint16_t)bufptr, size);

    GLOBAL_INT_DISABLE();

    #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
    if(rx_stat == R_ACK)
    {
        tx_stat = T_SUSPENDED;
    }
    else
    #endif
    {
        #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
        tx_stat = T_ACTIVE;
        #endif

        /* start DMA1 */
        write1_sfrbit(DST1, 1);

        /* start transfer */
        write1_sfrbit(STG1, DMA_SW_DEN_TRIG);
    }

    GLOBAL_INT_RESTORE();
#else
	// ・ｽf・ｽo・ｽb・ｽO0xAA・ｽ・ｽ・ｽM
	TXD0 = 0xAA;


#endif
}

/**
 ****************************************************************************************
 * @brief Get ready to enter the MCU stop mode.
 *
 * @return bool     true if success to ready, or false if failure to ready
 *****************************************************************************************
 */
_UARTCODE bool serial_stop_ready(void)
{
    bool ret = false;

    /* If uart disable (serial_init function not called), always return true. */
    if( (read_sfr(SOE0L) & UART_VAL_SOE) == 0 ) return true;

    /* check if stop is permitted */
    if(stop_flg != false)
    {
        /* check if already received data  or occurred error, or uart is active */
        if(((read_sfr(UART_RXD_SSR_L) & 0x60) == 0) &&
           (read_sfr(UART_SRIF) == 0)          &&
           (read_sfr(UART_SREIF) == 0))
        {
            #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
            stop_flg = false;

            /* stop UART */
            write_sfr(ST0L , UART_VAL_ST);
            #else /* SERIAL_U_2WIRE */
            /* set snooze mode */
            write_sfr(ST0L , UART_VAL_ST);
            write_sfr(SSC0L, 0x00);
            write_sfr(SSC0L, 0x01);
            write_sfr(SS0L , UART_VAL_SS);

            /* unmask INTSR0 interrupt */
            write1_sfrbit(UART_SRMK, IRQ_ENABLE);
            #endif

            /* stop DMA0 */
            write1_sfrbit(DST0, 0);

            ret = true;
        }
    }

    return ret;
}

/**
 ****************************************************************************************
 * @brief Return from the MCU stop mode.
 *****************************************************************************************
 */
_UARTCODE void serial_stop_finish(void)
{
    /* If uart disable (serial_init function not called), do nothing. */
    if( (read_sfr(SOE0L) & UART_VAL_SOE) == 0 ) return;

    /* restart DMA0 */
    write1_sfrbit(DST0, 1);

    #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
    stop_flg = true;

    /* start UART */
    write_sfr(SS0L , UART_VAL_SS);
    #else /* SERIAL_U_2WIRE */
    /* check if received data */
    if(read_sfr(UART_SRIF) != 0)
    {
        /* check if not occurrd error */
        if(read_sfr(UART_SREIF) == 0)
        {
            /* start DMA0 transfer by sw trigger */
            write1_sfrbit(STG0, DMA_SW_DEN_TRIG);
        }

        /* clear INTSR0 interrupt */
        write1_sfrbit(UART_SRIF, 0);
    }
    else if((read_sfr(UART_RXD_SSR_L) & 0x40) == 0)
    {
        /* cancel snooze mode, if no data receiving */
        write_sfr(ST0L , UART_VAL_ST);
        write_sfr(SSC0L, 0x00);
        write_sfr(SS0L , UART_VAL_SS);
    }
    else
    {
        /* DO NOTHING */
    }

    /* mask INTSR0 interrupt */
    write1_sfrbit(UART_SRMK, IRQ_DISABLE);
    #endif
}

#if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
/**
 ****************************************************************************************
 * @brief Inform the Wakeup Response.
 *****************************************************************************************
 */
_UARTCODE void serial_wakeup_response(void)
{
    /* set to keep wakeup */
    stop_flg = false;

    /* check if UART transmission is idle */
    if(tx_stat != T_ACTIVE)
    {
        /* clear and enable INTST0 interrupt */
        write1_sfrbit(UART_STIF, IRQ_CLEAR);
        write1_sfrbit(UART_STMK, IRQ_ENABLE);

        /* transmit the acknoledge byte */
        write_sfr(UART_TXD_SDR_L, UART_ACK_BYTE);

        rx_stat = R_ACK;

        /* <expect for st0_isr> */
    }
    else
    {
        rx_stat = R_ACTIVE;
    }
}
#endif // #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for when finished DMA0 tranfer.
 *****************************************************************************************
 */
#if defined(_USE_IAR_RL78)
#pragma vector = INTDMA0_vect
#elif defined(_USE_REL_RL78)
/* this pragma should be at the beginning of source file */
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is genarated for dma0_isr"
#endif
_UARTBASE __IRQ void dma0_isr(void)
{
    #if defined(_USE_REL_RL78)
    /* backup memory value */
    #asm
    push    ax
    push    bc
    push    de
    push    hl
    movw    de,#_@SEGAX
    movw    ax,[de+0AH]
    push    ax
    movw    ax,[de+08H]
    push    ax
    movw    ax,[de+06H]
    push    ax
    movw    ax,[de+04H]
    push    ax
    movw    ax,[de+02H]
    push    ax
    movw    ax,[de+00H]
    push    ax
    mov     a,ES
    mov     x,a
    mov     a,CS
    push    ax
    #endasm
    #endif

#if __DTM2WIRE_UART_USE__
    if ( _RBLE_RUN_MODE_NORMAL != rBLE_Run_Mode_Flg ) {
        if ( _RBLE_RUN_MODE_INIT == rBLE_Run_Mode_Flg ) {
            if ( true == DTM2Wire_Check() ) {
                rBLE_Run_Mode_Flg = _RBLE_RUN_MODE_NORMAL;
            } else {
                rBLE_Run_Mode_Flg = _RBLE_RUN_MODE_DIRECT;
            }
        } else {
            UART_CALLBACK_VOID(uart_callback.rx_callback);
        }
    } else {
#endif
        #if SERIAL_U_DIV_2WIRE
        /* REQ byte received */
        if((rx_stat == R_IDLE) && (*rx_ptr == UART_REQ_BYTE))
        {
            uart_dma_rx(rx_addr, rx_size);

            /* Inform the wakeup response */
            serial_wakeup_response();
        }
        else
        #endif
        {
            /* Inform the end of transmission */
            UART_CALLBACK_VOID(uart_callback.rx_callback);

            #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
            if( UART_CALLBACK_BOOL(uart_callback.rx_cmp_callback) != false )
            {
                rx_stat = R_IDLE;

                /* permit to stop */
                stop_flg = true;
            }
            else
            {
                /* DO NOTHING */
                /* rx_stat is maintained as R_ACTIVE */
            }
            #endif
        }
#if __DTM2WIRE_UART_USE__
    }
#endif

    /* clear interrupt flag */
    write1_sfrbit(UART_SRIF, IRQ_CLEAR);

    #if defined(_USE_REL_RL78)
    /* restore memory value */
    #asm
    pop     ax
    mov     CS,a
    mov     a,x
    mov     ES,a
    movw    de,#_@SEGAX
    pop     ax
    movw    [de],ax
    pop     ax
    movw    [de+02H],ax
    pop     ax
    movw    [de+04H],ax
    pop     ax
    movw    [de+06H],ax
    pop     ax
    movw    [de+08H],ax
    pop     ax
    movw    [de+0AH],ax
    pop     hl
    pop     de
    pop     bc
    pop     ax
    #endasm
    #endif
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for when finished DMA1 tranfer.
 *****************************************************************************************
 */
#if defined(_USE_IAR_RL78)
#pragma vector = INTDMA1_vect
#elif defined(_USE_REL_RL78)
/* this pragma should be at the beginning of source file */
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is genarated for dma1_isr"
#endif
_UARTBASE __IRQ void dma1_isr(void)
{
    #if defined(_USE_REL_RL78)
    /* backup memory value */
    #asm
    push    ax
    push    bc
    push    de
    push    hl
    movw    de,#_@SEGAX
    movw    ax,[de+0AH]
    push    ax
    movw    ax,[de+08H]
    push    ax
    movw    ax,[de+06H]
    push    ax
    movw    ax,[de+04H]
    push    ax
    movw    ax,[de+02H]
    push    ax
    movw    ax,[de+00H]
    push    ax
    mov     a,ES
    mov     x,a
    mov     a,CS
    push    ax
    #endasm
    #endif

    /* check if UARTn transmission is still going */
    if((read_sfr(UART_TXD_SSR_L) & 0x60) != 0)
    {
        /* clear INTSTn interrupt flag */
        write1_sfrbit(UART_STIF, IRQ_CLEAR);

        /* check if UARTn transmision just finished */
        if((read_sfr(UART_TXD_SSR_L) & 0x60) == 0)
        {
            /* set INTSTn interrupt flag */
            write1_sfrbit(UART_STIF, IRQ_SET);
        }
    }

    write1_sfrbit(UART_STMK, IRQ_ENABLE);

    /* <expect for st0_isr> */

    #if defined(_USE_REL_RL78)
    /* restore memory value */
    #asm
    pop     ax
    mov     CS,a
    mov     a,x
    mov     ES,a
    movw    de,#_@SEGAX
    pop     ax
    movw    [de],ax
    pop     ax
    movw    [de+02H],ax
    pop     ax
    movw    [de+04H],ax
    pop     ax
    movw    [de+06H],ax
    pop     ax
    movw    [de+08H],ax
    pop     ax
    movw    [de+0AH],ax
    pop     hl
    pop     de
    pop     bc
    pop     ax
    #endasm
    #endif
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for when finished UART transmission.
 *****************************************************************************************
 */
#if defined(_USE_IAR_RL78)
#if (UART_CHANNEL == 0)
#pragma vector = INTST0_vect
#else
#pragma vector = INTST1_vect
#endif
#elif defined(_USE_REL_RL78)
/* this pragma should be at the beginning of source file */
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is genarated for st0_isr"
#endif
_UARTBASE __IRQ void st0_isr(void)
{
#if __DTM2WIRE_UART_USE__
    if ( _RBLE_RUN_MODE_NORMAL != rBLE_Run_Mode_Flg ) {
        UART_CALLBACK_VOID(uart_callback.tx_callback);
    } else {
#endif
        #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
        if(rx_stat == R_ACK)
        {
            rx_stat = R_ACTIVE;
        }
        else
        {
            /* DO NOTHING */
            /* rx_stat is maintained as R_IDLE/R_ACTIVE */
        }
        if(tx_stat == T_SUSPENDED)
        {
            tx_stat = T_ACTIVE;

            /* start DMA1 */
            write1_sfrbit(DST1, 1);

            /* start transfer */
            write1_sfrbit(STG1, DMA_SW_DEN_TRIG);
        }
        else if(tx_stat == T_ACTIVE)
        #endif
        {
        #if (SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
            tx_stat = T_IDLE;
        #endif

            /* Inform the end of transmission */
            UART_CALLBACK_VOID(uart_callback.tx_callback);
        }
#if __DTM2WIRE_UART_USE__
    }
#endif

    /* mask interrupts */
    write1_sfrbit(UART_STMK, IRQ_DISABLE);
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for when occurred UART receiving error.
 *****************************************************************************************
 */
#if defined(_USE_IAR_RL78)
#if (UART_CHANNEL == 0)
#pragma vector = INTSRE0_vect
#else
#pragma vector = INTSRE1_vect
#endif
#elif defined(_USE_REL_RL78)
/* this pragma should be at the beginning of source file */
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is genarated for sre0_isr"
#endif


#define DBG_ERR_CHECK		1	//RD8001暫定：異常回数デバッグコード
#if DBG_ERR_CHECK	
long o_err = 0;
long f_err = 0;
long p_err = 0;
#endif
_UARTBASE __IRQ void sre0_isr(void)
{
    volatile uint8_t trash_data;

    /* stop DMA0 */
    write1_sfrbit(DST0, 0);

#if __DTM2WIRE_UART_USE__
    if ( _RBLE_RUN_MODE_NORMAL != rBLE_Run_Mode_Flg ) {
        UART_CALLBACK_VOID(uart_callback.err_callback);
    } else {
#endif
        UART_CALLBACK_VOID(uart_callback.err_callback);
#if __DTM2WIRE_UART_USE__
    }
#endif

    /*  clear DMA0 and SRIF0 interrupt flag */
    write1_sfrbit(DMAIF0, IRQ_CLEAR);
    write1_sfrbit(UART_SRIF , IRQ_CLEAR);

    /* to avoid uart_overrun error */
    trash_data = read_sfr(UART_RXD_SDR_L);

#if DBG_ERR_CHECK	//異常回数デバッグコード
	if( 0x01 & trash_data ){
		o_err++;
	}
	if( 0x02 & trash_data ){
		p_err++;
	}
	if( 0x03 & trash_data ){
		f_err++;
	}
#endif

    /* clear all error flags */
    write_sfr(UART_RXD_SIR_L, 0x07);
}


/// @} UART

