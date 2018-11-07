/**
 ****************************************************************************************
 *
 * @file csi.c
 *
 * @brief CSI module.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * rev.220
 *
 ****************************************************************************************
 */
/*
 * CSI CHANNEL SETTING
 ****************************************************************************************
 */
#define CSI_CHANNEL       (0)  // 0=CSI00, Other=CSI20

/**
 ****************************************************************************************
 * @addtogroup CSI
 * @{
 ****************************************************************************************
 */
#if defined(_USE_REL_RL78)
#pragma interrupt INTDMA0  dma0_isr  NOBANK
#pragma interrupt INTDMA1  dma1_isr  NOBANK
#if (CSI_CHANNEL == 0)
#pragma interrupt INTCSI00 csi00_isr
#else
#pragma interrupt INTCSI20 csi00_isr
#endif
#pragma interrupt INTTM07 timeout_isr

#pragma section @@DATA  UARTDATA
#pragma section @@CODEL UARTCODE
#pragma section @@BASE  UARTBASE

#define _UARTCODE 
#define _UARTDATA
#define _UARTBASE
#elif defined(_USE_CCRL_RL78)
#include "iodefine.h"
#pragma interrupt dma0_isr    (vect=INTDMA0)
#pragma interrupt dma1_isr    (vect=INTDMA1)
#if (CSI_CHANNEL == 0)
#pragma interrupt csi00_isr   (vect=INTCSI00)
#else
#pragma interrupt csi00_isr   (vect=INTCSI20)
#endif
#pragma interrupt timeout_isr (vect=INTTM07)

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
#include "hw_config.h"
#include "port.h"
#include "serial.h"
#include "csi.h"

#if ((!SERIAL_C_4WIRE && !SERIAL_C_5WIRE) || \
     ( SERIAL_C_4WIRE &&  SERIAL_C_5WIRE) || \
     SERIAL_U_2WIRE || SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
 #error "Serial setting in serial.h is wrong."
#endif

#if (CSI_CHANNEL != 0) && !(SERIAL_C_5WIRE)
 #error "CSI20 does not support the SNOOZE mode."
#endif

/*
 * DEFINES
 ****************************************************************************************
 */
///transmit request signal port operation
#define CSI_SDIR_INIT() write1_sfr(PM2, 3, PORT_OUTPUT)
#define CSI_SDIR_LOW()  write1_sfr(P2, 3, 0)
#define CSI_SDIR_HIGH() write1_sfr(P2, 3, 1)
#define CSI_SDIR_PLS()  \
{                       \
    CSI_SDIR_LOW();     \
    __no_operation();   \
    __no_operation();   \
    CSI_SDIR_HIGH();    \
}

///transmit acknowledge code
#define CSI_ACK_BYTE    (0x88)

/// dma max transferring size
#define DMA_MAX_SIZE    (1024U)


#define IRQ_SET     1

#define MAX_TX_TO_COUNT (4)

#define CSI_CALLBACK_VOID(cb)        SERIAL_CALLBACK_VOID(cb)
#define CSI_CALLBACK_BOOL(cb)        SERIAL_CALLBACK_BOOL(cb)

enum
{
    T_IDLE      = 0,
    T_REQUESTED = 1,
    T_ACTIVE    = 2
};

enum
{
    R_IDLE      = 0,
    R_ACTIVE    = 1
};

#if (CSI_CHANNEL == 0 )     // for CSI00

#define CSI_CHANNEL_n       0

#define CSI_VAL_SOE         0x01U
#define CSI_VAL_INTCSI      0x06U
#define CSI_VAL_SIO_ADDR    0x10U
#define CSI_MASK_SPS        0x0FU
#define CSI_VAL_SMR         0x4020U

#define CSI_SCK_PM          PM1
#define CSI_SCK_PIM         PIM1
#define CSI_SCK_PU          PU1
#define CSI_SCK_n           0
#define CSI_SI_PM           PM1
#define CSI_SI_PIM          PIM1
#define CSI_SI_n            1
#define CSI_SO_PM           PM1
#define CSI_SO_POM          POM1
#define CSI_SO_P            P1
#define CSI_SO_n            2


#define CSI_ST_L            ST0L
#define CSI_SS_L            SS0L
#define CSI_SAUEN           SAU0EN
#define CSI_SPS_L           SPS0L
#define CSI_SMR             SMR00
#define CSI_SCR             SCR00
#define CSI_SDR             SDR00
#define CSI_SO              SO0
#define CSI_SOE_L           SOE0L
#define CSI_SSC_L           SSC0L   /* for SNOOZE mode */
#define CSI_SSR_L           SSR00L
#define CSI_SIR_L           SIR00L

#define CSI_CSIMK           CSIMK00
#define CSI_CSIIF           CSIIF00

#else                       // for CSI20

#define CSI_CHANNEL_n       0
#define CSI_VAL_SOE         0x01U
#define CSI_VAL_INTCSI      0x0AU
#define CSI_VAL_SIO_ADDR    0x48U
#define CSI_MASK_SPS        0xF0U
#define CSI_VAL_SMR         0xC020U

#define CSI_SCK_PM          PM1
#define CSI_SCK_PIM         PIM1
#define CSI_SCK_PU          PU1
#define CSI_SCK_n           5
#define CSI_SI_PM           PM1
#define CSI_SI_PIM          PIM1
#define CSI_SI_n            4
#define CSI_SO_PM           PM1
#define CSI_SO_POM          POM1
#define CSI_SO_P            P1
#define CSI_SO_n            3

#define CSI_ST_L            ST1L
#define CSI_SS_L            SS1L
#define CSI_SAUEN           SAU1EN
#define CSI_SPS_L           SPS1L
#define CSI_SMR             SMR10
#define CSI_SCR             SCR10
#define CSI_SDR             SDR10
#define CSI_SO              SO1
#define CSI_SOE_L           SOE1L
#define CSI_SSC_L           SSC1L
#define CSI_SSR_L           SSR10L
#define CSI_SIR_L           SIR10L
        
#define CSI_CSIMK           CSIMK20
#define CSI_CSIIF           CSIIF20

#endif

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
_UARTDATA static uint8_t  tx_stat, rx_stat;
_UARTDATA static uint16_t rx_size, rx_addr;
_UARTDATA static uint8_t *rx_ptr;
_UARTDATA static uint8_t to_cnt;
_UARTDATA static SERIAL_EVENT_PARAM csi_callback;
/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 *****************************************************************************************
 * @brief Static function for setting port pin modes for the CSI00 pins.
 *****************************************************************************************
 */
_UARTCODE static void csi_port_init(void)
{
    /* SCK(Serial Clock Input) pin */
    write1_sfr(CSI_SCK_PM , CSI_SCK_n, PORT_INPUT);
    write1_sfr(CSI_SCK_PIM, CSI_SCK_n, PORT_INPUT_NORMAL);
    write1_sfr(CSI_SCK_PU,  CSI_SCK_n, PORT_PULLUP);

    /* SI(Serial Data Input) Pin */
    write1_sfr(CSI_SI_PM , CSI_SI_n, PORT_INPUT);
    write1_sfr(CSI_SI_PIM, CSI_SI_n, PORT_INPUT_NORMAL);

    /* SO(Serial Data Output) pin */
    write1_sfr(CSI_SO_PM , CSI_SO_n, PORT_OUTPUT);
    write1_sfr(CSI_SO_POM, CSI_SO_n, PORT_OUTPUT_NORMAL);
    write1_sfr(CSI_SO_P  , CSI_SO_n, PORT_LATCH);

    /* SDIR(Serial Direction) pin */
    CSI_SDIR_INIT();
}

/**
 ****************************************************************************************
 * @brief Initialize a DMA0 channel for transmission from CSI data register to RAM.
 *****************************************************************************************
 */
_UARTCODE static void dma0_init(void)
{
    /* enable DMA0 */
    write1_sfrbit(DEN0, 1);

    /* set SFR address */
    write_sfr(DSA0, CSI_VAL_SIO_ADDR);

    /* Transfer mode */
    write1_sfrbit(DRS0, DMA_SFR_TO_RAM);
    write1_sfrbit(DS0, DMA_UNIT_8B);
    write1_sfrbit(DWAIT0, DMA_TF_NO_PEND);

    /* Trigger setting */
    write_sfr(DMC0, (uint8_t)(read_sfr(DMC0) | (CSI_VAL_INTCSI & 0x0FU)));

    /* clear flag and unmask the interrupt */
    write1_sfrbit(DMAIF0, IRQ_CLEAR);
    write1_sfrbit(DMAMK0, IRQ_ENABLE);
}

/**
 ****************************************************************************************
 * @brief Initialize a DMA1 channel for transmission from RAM to CSI data register.
 *****************************************************************************************
 */
_UARTCODE static void dma1_init(void)
{
    /* enable DMA1 */
    write1_sfrbit(DEN1, 1);

    /* set SFR address */
    write_sfr(DSA1, CSI_VAL_SIO_ADDR);

    /* Transfer mode */
    write1_sfrbit(DRS1, DMA_RAM_TO_SFR);
    write1_sfrbit(DS1, DMA_UNIT_8B);
    write1_sfrbit(DWAIT1, DMA_TF_NO_PEND);

    /* Trigger setting */
    write_sfr(DMC1, (uint8_t)(read_sfr(DMC1) | (CSI_VAL_INTCSI & 0x0FU)));

    /* clear flag and unmask the interrupt */
    write1_sfrbit(DMAIF1, IRQ_CLEAR);
    write1_sfrbit(DMAMK1, IRQ_ENABLE);
}

/**
 ****************************************************************************************
 * @brief initialize timer for transmit_request timeout
 *****************************************************************************************
 */
_UARTCODE static void timeout_init(void)
{
    /* stop TAU0 ch7 */
    write1_sfr(TT0L, 7, 1);

    /* switch on timer arry unit input clock */
    write1_sfr2bit(TAU0EN, 1);

    /* select TAU clock frequency CK01: fx/4(2^2) */
    write_sfrp(TPS0, read_sfrp(TPS0) | 0x0020);

    /* set timer mode TAU channel CK01 */
    write_sfrp(TMR07, 0x8000);    /* interval timer mode */

    /* stop TAU0 ch7 */
    write1_sfr(TT0L, 7, 1);

    /* set timeout time: 4096usec */
    #if defined(CLK_FCLK_4MHZ)
    write_sfrp(TDR07, 0x1000);
    #elif defined(CLK_FCLK_8MHZ)
    write_sfrp(TDR07, 0x2000);
    #elif defined(CLK_FCLK_16MHZ)
    write_sfrp(TDR07, 0x4000);
    #elif defined(CLK_FCLK_32MHZ)
    write_sfrp(TDR07, 0x8000);
    #endif

    /* disable timer output port */
    write_sfr(TOE0L, (uint8_t)(read_sfr(TOE0L) & (~0x80)));
    write_sfr(TO0L , (uint8_t)(read_sfr(TO0L ) & (~0x80)));

    /* clear TAU0 ch7 interrupt request flag */
    write1_sfrbit(TMIF07, IRQ_CLEAR);

    /* enable TAU0 ch7 interrupt */
    write1_sfrbit(TMMK07, IRQ_ENABLE);
}

/**
 ****************************************************************************************
 * @brief start timeout timer
 *****************************************************************************************
 */
#define csi_start_timeout()     \
{                               \
    write1_sfr(TS0L, 7, 1);     \
}

/**
 ****************************************************************************************
 * @brief stop timeout timer
 *****************************************************************************************
 */
#define csi_stop_timeout()      \
{                               \
    write1_sfr(TT0L, 7, 1);     \
}

/**
 ****************************************************************************************
 * @brief Initialize CSI communication.(including initializing DMA and interrupt)
 *****************************************************************************************
 */
_UARTCODE void serial_init(SERIAL_EVENT_PARAM *param)
{
    csi_callback = *param;

    /* stop csi */
    write1_sfr(CSI_ST_L, CSI_CHANNEL_n, 1);

    /* PER0 peripheral enable register - release SAUm from reset and start clock supply */
    write1_sfr2bit(CSI_SAUEN, 1);

    /* must wait 4 clocks after PER0 setting */
    __no_operation();
    __no_operation();
    __no_operation();
    __no_operation();

    /* MCK=fclk */
    write_sfr(CSI_SPS_L, (uint8_t)(read_sfr(CSI_SPS_L) & ~CSI_MASK_SPS));

    /* set operation mode : Slave CSI */
    write_sfrp(CSI_SMR, CSI_VAL_SMR);

    /* Serial communication operation settings: Initialize to Rx */
    write_sfrp(CSI_SCR, 0x4007U);

    /* no need to set baudrate (use clock sent by the master) */
    write_sfrp(CSI_SDR, (uint16_t)0x0000U);

    /* Serial output */
    write_sfrp(CSI_SO, read_sfrp(CSI_SO) & (~0x0001U));

    /* enables output by serial communication operation */
    write_sfr(CSI_SOE_L, (uint8_t)(read_sfr(CSI_SOE_L) | CSI_VAL_SOE));

    /* set port and port mode */
    csi_port_init();

    /* mask CSI00 interrupt */
    write1_sfrbit(CSI_CSIMK, IRQ_DISABLE);

    /* initialize DMA0, and set DMA transfer setting */
    dma0_init();

    /* initialize DMA1, and set DMA transfer setting */
    dma1_init();

    /* initialize timeout timer */
    timeout_init();

    /* initialize SDIR */
    CSI_SDIR_HIGH();

    /* initialize status */
    tx_stat = T_IDLE;
    rx_stat = R_IDLE;

    /* start csi */
    write1_sfr(CSI_SS_L, CSI_CHANNEL_n, 1);
}

/**
 *****************************************************************************************
 * @brief Set CSI to RX only mode.
 *****************************************************************************************
 */
#if SERIAL_C_5WIRE
 #define csi_set_rx()                           \
 {                                              \
     /* stop csi */                             \
     write1_sfr(CSI_ST_L, CSI_CHANNEL_n, 1);    \
                                                \
     /* change to rx only */                    \
     write_sfrp(CSI_SCR, 0x4007);               \
                                                \
     /* start csi */                            \
     write1_sfr(CSI_SS_L, CSI_CHANNEL_n, 1);    \
 }
#else /* SERIAL_C_4WIRE */
 #define csi_set_rx()                           \
 {                                              \
     /* stop csi */                             \
     write1_sfr(CSI_ST_L, CSI_CHANNEL_n, 1);    \
                                                \
     /* cancel snooze mode */                   \
     write_sfrp(CSI_SSC_L, 0x00);               \
                                                \
     /* change to rx only */                    \
     write_sfrp(CSI_SCR, 0x4007);               \
                                                \
     /* start csi */                            \
     write1_sfr(CSI_SS_L, CSI_CHANNEL_n, 1);    \
 }
#endif

/**
 *****************************************************************************************
 * @brief Set CSI to TX only mode.
 *****************************************************************************************
 */
#if SERIAL_C_5WIRE
 #define csi_set_tx()                           \
 {                                              \
     /* stop csi */                             \
     write1_sfr(CSI_ST_L, CSI_CHANNEL_n, 1);    \
                                                \
     /* change to tx only */                    \
     write_sfrp(CSI_SCR, 0x8007);               \
                                                \
     /* start csi */                            \
     write1_sfr(CSI_SS_L, CSI_CHANNEL_n, 1);    \
 }
#else /* SERIAL_C_4WIRE */
 #define csi_set_tx()                           \
 {                                              \
     /* stop csi */                             \
     write1_sfr(CSI_ST_L, CSI_CHANNEL_n, 1);    \
                                                \
     /* cancel snooze mode */                   \
     write_sfrp(CSI_SSC_L, 0x00);               \
                                                \
     /* change to tx only */                    \
     write_sfrp(CSI_SCR, 0x8007);               \
                                                \
     /* start csi */                            \
     write1_sfr(CSI_SS_L, CSI_CHANNEL_n, 1);    \
 }
#endif

/**
 *****************************************************************************************
 * @brief Set DMA0 for receiving csi.
 *****************************************************************************************
 */
#define csi_dma_rx(dest_addr, size)             \
{                                               \
    /* stop DMA0 */                             \
    write1_sfrbit(DST0, 0);                     \
                                                \
    /* set RAM address (destination) */         \
    write_sfrp(DRA0, (dest_addr));              \
                                                \
    /* set transfer size */                     \
    write_sfrp(DBC0, (size));                   \
}

/**
 *****************************************************************************************
 * @brief Set DMA1 for transferring csi.
 *****************************************************************************************
 */
#define csi_dma_tx(src_addr, size)              \
{                                               \
    /* stop DMA1 */                             \
    write1_sfrbit(DST1, 0);                     \
                                                \
    /* set RAM address (source) */              \
    write_sfrp(DRA1, (src_addr));               \
                                                \
    /* set transfer size */                     \
    write_sfrp(DBC1, (size));                   \
}

/**
 ****************************************************************************************
 * @brief Start CSI receiving data.
 *        When finished receiving data, calls callback function.
 *
 * @param[in,out] bufptr  destination pointer, start of the space where data is transfered to.
 * @param[in] size        Transfer size, received data size byte by CSI.
 *****************************************************************************************
 */
_UARTCODE void serial_read(uint8_t *bufptr, const uint16_t size)
{
    /* store the argument parameter */
    rx_ptr  = (uint8_t *)bufptr;
    rx_addr = (uint16_t)bufptr;
    rx_size = (size == DMA_MAX_SIZE) ? (0) : (size);

    /* ready DMA0 for Rx */
    csi_dma_rx(rx_addr, rx_size);

    if(tx_stat != T_ACTIVE)
    {
        /* start DMA0 */
        write1_sfrbit(DST0, 1);
    }

    /* <expect for dma0_isr> */
}

/**
 ****************************************************************************************
 * @brief Start CSI transferring data.
 *        When finished transferring data, calls callback function.
 *
 * @param[in] bufptr  source pointer, start of the space where data is transfered from.
 * @param[in] size    Transfer size, transferred data size byte by CSI.
 * @return bool       true  if success to start transmit, or
                      false if failed to start transmit by another communication is going
 *****************************************************************************************
 */
_UARTCODE void serial_write(const uint8_t *bufptr, const uint16_t size)
{
    uint16_t tx_size;

    ASSERT_ERR((size > 0) && (size <= 1024));

    tx_stat = T_REQUESTED;

    /* ready DMA0 for Tx */
    tx_size = (size == DMA_MAX_SIZE) ? (0) : (size);
    csi_dma_tx((uint16_t)bufptr, tx_size);

    GLOBAL_INT_DISABLE();

    /* avoid to output pulse while receiving */
    if((rx_stat == R_IDLE)               &&
       (read_sfr(CSI_CSIIF) == 0)          &&
       ((read_sfrp(CSI_SSR_L) & 0x40) == 0))
    {
        /* inform transmit_request to the master */
        CSI_SDIR_PLS();

        /* start to check timeout */
        to_cnt = 0;
        csi_start_timeout();
    }

    /* <expect for dma0_isr> */

    GLOBAL_INT_RESTORE();

    /* <expect for dma0_isr> */

    return;
}

/**
 ****************************************************************************************
 * @brief stop CSI when rx_error was happened.
 *****************************************************************************************
 */
_UARTCODE static void csi_rx_error(void)
{
    /* stop DMA0 */
    write1_sfrbit(DST0, 0);

    CSI_CALLBACK_VOID(csi_callback.err_callback);

    /* clear error flags */
    write_sfrp(CSI_SIR_L, 0x01);
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

    /* If csi disable (serial_init function not called), always return true. */
    if( (read_sfr(CSI_SOE_L) & CSI_VAL_SOE) == 0 ) return true;

    if(rx_stat == R_IDLE)
    {
        /* check if already received data  or occurred error, or csi is active */
        if(((read_sfrp(CSI_SSR_L) & 0x40) == 0) && (read_sfr(CSI_CSIIF) == 0))
        {
            #if SERIAL_C_4WIRE
            /* set snooze mode */
            write1_sfr(CSI_ST_L, CSI_CHANNEL_n, 1);
            write_sfrp(CSI_SSC_L, 0x00);
            write_sfrp(CSI_SSC_L, 0x01);
            write1_sfr(CSI_SS_L, CSI_CHANNEL_n, 1);
            #endif

            /* stop DMA0 */
            write1_sfrbit(DST0, 0);

            #if SERIAL_C_4WIRE
            /* unmask INTCSI00 interrupt */
            write1_sfrbit(CSI_CSIMK, IRQ_ENABLE);
            #endif

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
    /* If csi disable (serial_init function not called), do nothing. */
    if( (read_sfr(CSI_SOE_L) & CSI_VAL_SOE) == 0 ) return;

    #if SERIAL_C_4WIRE
    /* mask INTCSI00 interrupt */
    write1_sfrbit(CSI_CSIMK, IRQ_DISABLE);
    #endif

    /* restart DMA0 */
    write1_sfrbit(DST0, 1);

    #if SERIAL_C_4WIRE
    /* check if received data */
    if(read_sfr(CSI_CSIIF) != 0)
    {
        /* check if not occurrd error */
        if((read_sfrp(CSI_SSR_L) & 0x01) == 0)
        {
            /* start DMA0 transfer by sw trigger */
            write1_sfrbit(STG0, DMA_SW_DEN_TRIG);
        }
        else
        {
            /* stop CSI when rx_error was happened */
            csi_rx_error();
        }

        /* clear INTCSI00 interrupt */
        write1_sfrbit(CSI_CSIIF, IRQ_CLEAR);
    }
    #endif
}

#if SERIAL_C_5WIRE
/**
 ****************************************************************************************
 * @brief Inform the Wakeup Response.
 *****************************************************************************************
 */
_UARTCODE void serial_wakeup_response(void)
{
    rx_stat = R_ACTIVE;

    /* Initialize to Rx */
    csi_set_rx();

    /* inform ready_for_transmit to the master */
    CSI_SDIR_PLS();
}
#endif // #if SERIAL_C_5WIRE

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

    csi_stop_timeout();

    /* clear INTCSI00 interrupt flag */
    write1_sfrbit(CSI_CSIIF, IRQ_CLEAR);

    /* check if not occurrd error */
    if((read_sfrp(CSI_SSR_L) & 0x01) == 0)
    {
        #if SERIAL_C_4WIRE
        if(rx_stat == R_IDLE)
        #else /* SERIAL_C_5WIRE */
        if( CSI_CALLBACK_BOOL(csi_callback.rx_cmp_callback) != false) 
        #endif
        {
            /* ACK byte received */
            if((*rx_ptr == CSI_ACK_BYTE) && (tx_stat == T_REQUESTED))
            {
                /* transmit_request is accepted */
                tx_stat = T_ACTIVE;

                /* reset DMA0 setting for next packet */
                csi_dma_rx(rx_addr, rx_size);

                /* change Rx->Tx */
                csi_set_tx();
                write1_sfrbit(DST1, 1);

                /* set first byte by sw trigger */
                write1_sfrbit(STG1, DMA_SW_DEN_TRIG);

                /* inform ready_for_transmit to the master */
                CSI_SDIR_LOW();

                /* <expect for dma1_isr> */
            }
            /* first byte received */
            else
            {
#ifndef CONFIG_EMBEDDED
                CSI_CALLBACK_VOID(csi_callback.rx_callback);
#endif  /* CONFIG_EMBEDDED */

                #if SERIAL_C_4WIRE
#ifndef CONFIG_EMBEDDED
                if( CSI_CALLBACK_BOOL(csi_callback.rx_first_byte_callback) != false)
#endif  /* CONFIG_EMBEDDED */
                {
                    rx_stat = R_ACTIVE;

                    /* inform transmit_request to the master */
                    CSI_SDIR_PLS();
                }
                #endif
            }
        }
        /* following byte received */
        else
        {
#ifndef CONFIG_EMBEDDED
            CSI_CALLBACK_VOID(csi_callback.rx_callback);
#endif  /* CONFIG_EMBEDDED */

            /* check finish to receive all of packet data */
#ifndef CONFIG_EMBEDDED
            if( CSI_CALLBACK_BOOL(csi_callback.rx_cmp_callback) != false )
#endif  /* CONFIG_EMBEDDED */
            {
                rx_stat = R_IDLE;

                if(tx_stat == T_REQUESTED)
                {
                    /* inform transmit_request to the master */
                    CSI_SDIR_PLS();

                    /* start to check timeout */
                    to_cnt = 0;
                    csi_start_timeout();
                }
            }
        }
    }
    else
    {
        /* stop CSI when rx_error was happened */
        csi_rx_error();
    }

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

    /* check if CSI00 is still going or waiting to start */
    if((read_sfrp(CSI_SSR_L) & 0x60) != 0)
    {
        /* clear INTCSI00 interrupt flag */
        write1_sfrbit(CSI_CSIIF, IRQ_CLEAR);

        /* check if CSI00 just finished */
        if((read_sfr(CSI_SSR_L) & 0x60) == 0)
        {
            /* set INTCSI00 interrupt flag */
            write1_sfrbit(CSI_CSIIF, IRQ_SET);
        }
    }
    write1_sfrbit(CSI_CSIMK, IRQ_ENABLE);

    /* <expect for csi00_isr> */

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
 * @brief Interrupt Service Routine, for finished to transmit.
 *****************************************************************************************
 */
#if defined(_USE_IAR_RL78)
#if (CSI_CHANNEL == 0 )
#pragma vector = INTCSI00_vect
#else
#pragma vector = INTCSI20_vect
#endif
#elif defined(_USE_REL_RL78)
/* this pragma should be at the beginning of source file */
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is genarated for csi00_isr"
#endif
_UARTBASE __IRQ void csi00_isr(void)
{
    if(tx_stat == T_ACTIVE)
    {
        /* Inform the end of transmission */
#ifndef CONFIG_EMBEDDED
        CSI_CALLBACK_VOID(csi_callback.tx_callback);
#endif  /* CONFIG_EMBEDDED */
    }

    /* finished Tx */
    write1_sfrbit(CSI_CSIMK, IRQ_DISABLE);

    /* change Tx->Rx */
    csi_set_rx();
    write1_sfrbit(DST0, 1);

    /* reset the SDIR */
    CSI_SDIR_HIGH();

    tx_stat = T_IDLE;
}


#if defined(_USE_IAR_RL78)
#pragma vector=INTTM07_vect
#elif defined(_USE_REL_RL78)
// this pragma should be at the beginning of source file
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is generated for timeout_isr"
#endif
_UARTBASE __IRQ void timeout_isr (void)
{
    /* stop timeout timer */
    csi_stop_timeout();

    /* clear TAU0 ch7 interrupt request flag */
    write1_sfrbit(TMIF07, IRQ_CLEAR);

    if(tx_stat == T_REQUESTED)
    {
        to_cnt++;
        if(to_cnt <= MAX_TX_TO_COUNT)
        {
            CSI_SDIR_PLS();

            /* restart timeout timer */
            csi_start_timeout();
        }
        /* timeout discontinued */
        else
        {
            tx_stat = T_IDLE;

            /* Inform the end of transmission */
#ifndef CONFIG_EMBEDDED
            CSI_CALLBACK_VOID(csi_callback.tx_callback);
#endif  /* CONFIG_EMBEDDED */
        }
    }
}

/// @} module
