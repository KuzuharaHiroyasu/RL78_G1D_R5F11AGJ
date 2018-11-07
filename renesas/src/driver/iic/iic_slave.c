/**
 ****************************************************************************************
 *
 * @file iic_slave.c
 *
 * @brief IIC Slave Driver module.
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup IICA
 * @{
 ****************************************************************************************
 */
#if defined(_USE_REL_RL78)
#pragma interrupt INTIICA0 iica0_isr
#pragma interrupt INTTM07  inttm07_isr

#pragma section @@DATA  UARTDATA
#pragma section @@CODEL UARTCODE
#pragma section @@BASE  UARTBASE

#define _UARTCODE 
#define _UARTDATA
#define _UARTBASE
#elif defined(_USE_CCRL_RL78)
#include "iodefine.h"
#pragma interrupt iica0_isr   (vect=INTIICA0)
#pragma interrupt inttm07_isr (vect=INTTM07)

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
#include "iic_slave.h"

#if (!SERIAL_I_3WIRE) || \
    (SERIAL_C_4WIRE || SERIAL_C_5WIRE) || \
    (SERIAL_U_2WIRE || SERIAL_U_3WIRE || SERIAL_U_DIV_2WIRE)
 #error "Serial setting in serial.h is wrong."
#endif

/*
 * DEFINES MACRO
 ****************************************************************************************
 */
///transmit request signal port operation
#define IIC_REQ_INIT()     write1_sfr(PM2, 3, PORT_OUTPUT)
#define IIC_REQ_ACTIVE()   write1_sfr(P2,  3, 0)
#define IIC_REQ_INACTIVE() write1_sfr(P2,  3, 1)
#define IIC_REQ_PULSE()  \
{                        \
    IIC_REQ_INACTIVE();  \
    __no_operation();    \
    __no_operation();    \
    IIC_REQ_ACTIVE();    \
}

/// callback macro
#define IIC_CALLBACK(cb)        SERIAL_CALLBACK_VOID(cb)
#define IIC_CB_TX_DONE()        IIC_CALLBACK(iic_cb_info.tx_callback)
#define IIC_CB_RX_DONE()        IIC_CALLBACK(iic_cb_info.rx_callback)
#define IIC_CB_RX_ERR()         IIC_CALLBACK(iic_cb_info.err_callback)
#define IIC_CB_RX_FIRST_BYTE()  /* no use */
#define IIC_CB_RX_COMP_CHK()    /* no use */

/// check tx_request state
#define IIC_IS_TX_REQUESTED()   (iic_tx_info.size > 0)

/// initialize IIC info
#define IIC_INFO_INIT(info)   \
{                             \
    info.size = 0;            \
    info.cnt  = 0;            \
}

/*
 * STATIC FUNCTIONS PROTOTYPE
 ****************************************************************************************
 */
static void iic_timer_init(void);
static void iic_tx_request(void);
static void iic_byte_tx(void);
static void iic_byte_rx(void);
static void iic_rx_finish(const uint8_t err_code);
static void iic_tx_finish(const uint8_t err_code);
static void iic_stop_condition(void);

/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
_UARTDATA static SERIAL_EVENT_PARAM     iic_cb_info;
_UARTDATA static IIC_INFO               iic_tx_info;
_UARTDATA static IIC_INFO               iic_rx_info;
_UARTDATA static uint8_t                iic_to_cnt;
_UARTDATA static uint8_t                iic_state;

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
/**
 ****************************************************************************************
 * @brief initialize timer for tx_request timeout
 *****************************************************************************************
 */
#if (IIC_MAX_TX_TO_COUNT)
_UARTCODE static void iic_timer_init(void)
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
#else // Timeout no use
#define iic_timer_init()
#endif

/**
 ****************************************************************************************
 * @brief start timeout timer
 *****************************************************************************************
 */
#if (IIC_MAX_TX_TO_COUNT)
#define iic_start_timer()       \
{                               \
    write1_sfr(TS0L, 7, 1);     \
}
#else // Timeout no use
#define iic_start_timer()
#endif

/**
 ****************************************************************************************
 * @brief stop timeout timer
 *****************************************************************************************
 */
#if (IIC_MAX_TX_TO_COUNT)
#define iic_stop_timer()        \
{                               \
    write1_sfr(TT0L, 7, 1);     \
}
#else // Timeout no use
#define iic_stop_timer()
#endif



/**
 ****************************************************************************************
 * @brief start tx_request
 *****************************************************************************************
 */
_UARTCODE static void iic_tx_request(void)
{
    /* clear timeout counter */
    iic_to_cnt = 0;

    /* start timer */
    iic_start_timer();

    /* Request signal active */
    IIC_REQ_ACTIVE();
}


/**
 ****************************************************************************************
 * @brief send 1byte data.
 *****************************************************************************************
 */
_UARTCODE static void iic_byte_tx(void)
{
    /* If there is a remaining data */
    if( iic_tx_info.cnt < iic_tx_info.size )
    {
        /* If sending the last byte */
        if( (iic_tx_info.cnt + 1) >= iic_tx_info.size )
        {
            /* Request signal inactive */
            IIC_REQ_INACTIVE();
        }
        /* send 1byte data */
        write_sfr(IICA0, iic_tx_info.bufptr[iic_tx_info.cnt++]);
    }
    /* No transmit data */
    else
    {
        /* Request signal inactive */
        IIC_REQ_INACTIVE();
        /* send END_CODE data */
        write_sfr(IICA0, IIC_END_CODE);
    }

}

/**
 ****************************************************************************************
 * @brief receive 1byte data.
 *****************************************************************************************
 */
_UARTCODE static void iic_byte_rx(void)
{
    /* reception buffer has a space */
    if( iic_rx_info.cnt < iic_rx_info.size  )
    {
        /* 1byet data read */
        iic_rx_info.bufptr[iic_rx_info.cnt++] = read_sfr(IICA0);

        /* reception buffer full */
        if( iic_rx_info.cnt >= iic_rx_info.size )
        {
            /* rx_callback */
            IIC_CB_RX_DONE();
        }
        /* reception buffer NOT full */
        else
        {
            /* wait released */
            write1_sfrbit(WREL0, 1);
        }
    }
}


/**
 ****************************************************************************************
 * @brief stop IICA when rx finish was happened.
 *****************************************************************************************
 */
_UARTCODE static void iic_rx_finish(const uint8_t err_code)
{
    switch( err_code )
    {
        case IIC_ERR_NONE:
            /* To callback it has been received */
            if( iic_rx_info.cnt > 0 )
            {
                /* Initialize rx information */
                IIC_INFO_INIT(iic_rx_info);
                /* callback rx_done */
                IIC_CB_RX_DONE();
            }
            break;
        default:
            /* Initialize rx information */
            IIC_INFO_INIT(iic_rx_info);

            /* callback rx_error */
            IIC_CB_RX_ERR();
            break;
    }
}

/**
 ****************************************************************************************
 * @brief stop IICA when tx finish was happened.
 *****************************************************************************************
 */
_UARTCODE static void iic_tx_finish(const uint8_t err_code)
{
    /* Request signal inactive */
    IIC_REQ_INACTIVE();

    switch( err_code )
    {
        case IIC_ERR_NONE:
        case IIC_ERR_NACK:
        case IIC_ERR_TIMEOUT:
        default:
            /* Initialize tx information */
            IIC_INFO_INIT(iic_tx_info);
            /* callback tx_done */
            IIC_CB_TX_DONE();
            break;
    }
}


/**
 ****************************************************************************************
 * @brief stop IICA when stop condition was happened.
 *****************************************************************************************
 */
_UARTCODE static void iic_stop_condition(void)
{
    /* disable interrupt of STOP CONDITION */
    write1_sfrbit(SPIE0, 0);

    /* RX GOING */
    switch( iic_state )
    {
        case IIC_STAT_RX_GOING:
            /* RX finish */
            iic_rx_finish(IIC_ERR_NONE);

            /* If TX requested  */
            if( IIC_IS_TX_REQUESTED() )
            {
                /* restart TX request */
                iic_tx_request();
            }
            break;

        case IIC_STAT_TX_GOING:
            /* If TX requested  */
            if( IIC_IS_TX_REQUESTED() )
            {
                /* send all data */
                if( iic_tx_info.cnt >= iic_tx_info.size )
                {
                    /* TX finish */
                    iic_tx_finish(IIC_ERR_NONE);
                }
                /* rest data */
                else
                {
                    /* start TX request */
                    iic_tx_request();
                }
            }
            break;

        default:
            /* do nothing */
            break;
    }
    /* initialized IIC state */
    iic_state = IIC_STAT_IDLE;
}


/**
 ****************************************************************************************
 * @brief Initialize IIC communication.
 *****************************************************************************************
 */
_UARTCODE void serial_init(SERIAL_EVENT_PARAM *param)
{
    /* keep callback informations */
    memcpy(&iic_cb_info, param, sizeof(SERIAL_EVENT_PARAM));

    /* Initialized valiable */
    IIC_INFO_INIT(iic_tx_info);
    IIC_INFO_INIT(iic_rx_info);

    iic_to_cnt  = 0;
    iic_state   = IIC_STAT_IDLE;

    /* Initialized REQ port */
    IIC_REQ_INIT();
    IIC_REQ_INACTIVE();

    /* Initialized Timer Module */
    iic_timer_init();

    /* Initialized IICA hardware module */
    write1_sfrbit(IICA0EN,  1);    /* supply IICA0 clock */
    write1_sfrbit(IICE0,    0);    /* disable IICA0 operation */
    write1_sfrbit(IICAMK0,  1);    /* disable INTIICA0 interrupt */
    write1_sfrbit(IICAIF0,  0);    /* clear INTIICA0 interrupt flag */

    /* Set INTIICA0 low priority */
    write1_sfrbit(IICAPR10, 1);
    write1_sfrbit(IICAPR00, 1);

    /* Set SCLA0(P60), SDAA0 (P61) pin */
    write_sfr(P6,  (uint8_t)(read_sfr(P6)  & 0xFCU));
    write_sfr(PM6, (uint8_t)(read_sfr(PM6) | 0x03U));

    /* Set Fast mode */
    write1_sfrbit(SMC0,     1);

    /* Set IICWL0/IICWH0 (100kbps to 400kbps) */
    write_sfr(IICWL0,   0x15U);
    write_sfr(IICWH0,   0x14U);

    /* digital filter on */
    write1_sfrbit(DFC0,     1);


#if defined(CLK_FCLK_32MHZ)    /* fMAIN > 20MHz */
    /* Set fCLK/2 */
    write1_sfrbit(PRS0,     1);
#else                          /* fMAIN <= 20MHz */
    /* Set fCLK */
    write1_sfrbit(PRS0,     0);
#endif 

    /* Set Slave Address 7bit*/
    write_sfr(SVA0,     (uint8_t)(IIC_SLAVE_ADDRESS<<1));

    /* After operation is enabled (IICEn = 1), 
       enable generation of a start condition without detecting 
       a stop condition.*/
    write1_sfrbit(STCEN0,   1);

    /* Disable communication reservation */
    write1_sfrbit(IICRSV0,  1);

    /* disable generation of interrupt request 
       when stop condition is detected */
    write1_sfrbit(SPIE0,    0);

    /* Interrupt request is generated at the ninth clock's 
       falling edge */
    write1_sfrbit(WTIM0,    1);

    /* Enable acknowledgment(ACK) */
    write1_sfrbit(ACKE0,    1);

    /* Enable INTIICA0 interrupt */
    write1_sfrbit(IICAMK0,  0);

    /* Enable IICA0 operation */
    write1_sfrbit(IICE0,    1);

    /* Exit from communications */
    write1_sfrbit(LREL0,    1);

    /* Set SCLA0, SDAA0 pin */
    write_sfr(PM6, (uint8_t)(read_sfr(PM6) & 0xFCU));

}
/**
 ****************************************************************************************
 * @brief Start IICA receiving data.
 *        When finished receiving data, calls callback function.
 *
 * @param[in,out] bufptr  destination pointer, start of the space where data is transfered to.
 * @param[in] size        Transfer size, received data size byte by IIC.
 *****************************************************************************************
 */
_UARTCODE void serial_read(uint8_t *bufptr, const uint16_t size)
{
    /* keep rx buffer pointer and size*/
    iic_rx_info.bufptr  = (uint8_t*)bufptr;
    iic_rx_info.size    = (uint16_t)size;

    /* rx counter clear */
    iic_rx_info.cnt     = (uint16_t)0;

    /* Wait released if being received */
    if( iic_state == IIC_STAT_RX_GOING )
    {
        /* Wait released */
        write1_sfrbit(WREL0, 1);
    }
}

/**
 ****************************************************************************************
 * @brief Start IICA transferring data.
 *        When finished transferring data, calls callback function.
 *
 * @param[in] bufptr  source pointer, start of the space where data is transfered from.
 * @param[in] size    Transfer size, transferred data size byte by IIC.
 *****************************************************************************************
 */
_UARTCODE void serial_write(const uint8_t *bufptr, const uint16_t size)
{
    ASSERT_ERR((size > 0) && (size <= IIC_MAX_SIZE));

    /* Already Tx requested  */
    if( IIC_IS_TX_REQUESTED() )
    {
        /* Request signal inactive */
        IIC_REQ_INACTIVE();
    }

    /* keep tx buffer pointer and size*/
    iic_tx_info.bufptr  = (uint8_t*)bufptr;
    iic_tx_info.size    = (uint16_t)size;

    /* tx counter clear */
    iic_tx_info.cnt     = (uint16_t)0;

    if( iic_state == IIC_STAT_IDLE )
    {
        /* start TX request */
        iic_tx_request();
    }
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

    /* disable iica (not called serial_init function), always return true */
    if( read_sfr(IICE0) == 0 ) return true;

    /* IIC IDLE */
    if( iic_state == IIC_STAT_IDLE )
    {
        /* Exit from communications */
        write1_sfrbit(LREL0, 1);

        /* Enables operation of wakeup. */
        write1_sfrbit(WUP0, 1);

        /* Wait 3 clock */
        __no_operation();
        __no_operation();
        __no_operation();

        ret = true;
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
    /* do nothing. */
    return;
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for INTIICA0.
 *****************************************************************************************
 */
#if defined(_USE_IAR_RL78)
#pragma vector = INTIICA0_vect
#elif defined(_USE_REL_RL78)
/* this pragma should be at the beginning of source file */
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is genarated for iica0_isr"
#endif
_UARTBASE __IRQ void iica0_isr(void)
{
    /* Enable wakeup mode */
    if( read_sfr(WUP0) != 0 )
    {
        /* Disable wakeup mode */
        write1_sfrbit(WUP0, 0);

        /* Wait 5 clock */
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
        __no_operation();
    }

    do {

        /* It is not the interrupt for SLAVE */
        if( read_sfr(MSTS0) != 0 )
        {
            /* Ignored */
            break;
        }

        /* STOP CONDITION detected */
        if( read_sfr(SPD0) != 0 )
        {
            iic_stop_condition();
            break;
        }

        /* before own address detection */
        if( iic_state == IIC_STAT_IDLE )
        {
            /* own address not detected */
            if( read_sfr(COI0) == 0 )
            {
                /* Ignored */
                break;
            }

            /* stop timer */
            iic_stop_timer();

            /* enable generation of interrupt request 
               when stop condition is detected */
            write1_sfrbit(SPIE0, 1);

            /* Read request (Slave transmission) */
            if( read_sfr(TRC0) != 0 )
            {
                /* Update IIC state */
                iic_state = IIC_STAT_TX_GOING;

                /* send data */
                iic_byte_tx();

            }
            /* Write request (Slave reception) */
            else
            {
                /* Request signal inactive */
                IIC_REQ_INACTIVE();

                /* Update IIC state */
                iic_state = IIC_STAT_RX_GOING;

                /* reception buffer has a space */
                if( iic_rx_info.cnt < iic_rx_info.size  )
                {
                    /* wait released */
                    write1_sfrbit(WREL0, 1);
                }
            }
        }
        /* after own address detection */
        else
        {
            /* Read request (Slave transmission) */
            if( read_sfr(TRC0) != 0 )
            {
                /* NACK detected */
                if(read_sfr(ACKD0) == 0)  
                {
                    /* wait released */
                    write1_sfrbit(WREL0, 1);
                }
                /* ACK detected */
                else
                {
                    /* send data */
                    iic_byte_tx();
                }
            }
            /* Write request (Slave reception) */
            else
            {
                /* receive data */
                iic_byte_rx();
            }
        }
    }while(0);
}

/**
 ****************************************************************************************
 * @brief Interrupt Service Routine, for INTTM07.
 *****************************************************************************************
 */
#if defined(_USE_IAR_RL78)
#pragma vector=INTTM07_vect
#elif defined(_USE_REL_RL78)
// this pragma should be at the beginning of source file
#elif defined(_USE_CCRL_RL78)
/* this pragma should be at the beginning of source file */
#else
#warning "No interrupt vector is generated for inttm07_isr"
#endif
_UARTBASE __IRQ void inttm07_isr(void)
{
    /* stop timeout timer */
    iic_stop_timer();

    /* clear TAU0 ch7 interrupt request flag */
    write1_sfrbit(TMIF07, IRQ_CLEAR);

    /* tx requested */
    if((iic_state == IIC_STAT_IDLE) && (iic_tx_info.cnt < iic_tx_info.size))
    {
        iic_to_cnt++;
        if(iic_to_cnt < IIC_MAX_TX_TO_COUNT)
        {
            IIC_REQ_PULSE();

            /* restart timeout timer */
            iic_start_timer();
        }
        /* timeout discontinued */
        else
        {
            /* Inform the end of transmission */
            iic_tx_finish(IIC_ERR_TIMEOUT);
        }
    }
}

/// @} module
