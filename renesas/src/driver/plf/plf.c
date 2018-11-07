/**
 ****************************************************************************************
 *
 * @file plf.c
 *
 * @brief This file contains the Low Level RENESAS RL78 MCU clock initialisation functions.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 * $Rev: 19805 $
 *
 * rev.214
 *
 ****************************************************************************************
 */

/**
 ****************************************************************************************
 * @addtogroup PLATFORM_DRIVERS
 * @{
 ****************************************************************************************
 */
#ifdef _USE_IAR_RL78
#define _CLK_TBL _Pragma("location=\"CLK_TBL\"")
#define _PLF_CODE _Pragma("location=\"PLF_CODE\"")
#else // _USE_IAR_RL78
#define _CLK_TBL
#define _PLF_CODE

#if !defined(_USE_CCRL_RL78)
#pragma section @@CODEL PLF_CODE
#pragma section @@CNST  CLK_TBL
#else
#pragma section text    PLF_CODE
#pragma section const   CLK_TBL
#endif
#endif // _USE_IAR_RL78


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "arch.h"

///Include corresponding header file
#include "ll.h"

#include "port.h"
#include "plf.h"
#include "hw_config.h"

/* CMC register settings(1/2) */
#if defined(CLK_HOCO)
/* High-speed on-chip oscillator */
#define SFR_VAL_CMC_MSYSCLK (0x00U)
#elif defined(CLK_X1_LOW)
/* X1 oscillator (fX <= 10MHz) */
#define SFR_VAL_CMC_MSYSCLK (0x40U)
#elif defined(CLK_X1_HIGH)
/* X1 oscillator (fX > 10MHz) */
#define SFR_VAL_CMC_MSYSCLK (0x41U)
#elif defined(CLK_EX)
/* External main system clock */
#define SFR_VAL_CMC_MSYSCLK (0xC0U)
#else
#error "an unsupported main system clock configuration."
#endif

/* CMC register settings(2/2) */
#if defined(CLKS_XT1)
/* XT1 oscillator */
#define SFR_VAL_CMC_SSYSCLK (0x10U)
#elif defined(CLKS_EX)
/* External clock input */
#define SFR_VAL_CMC_SSYSCLK (0x30U)
#else
#define SFR_VAL_CMC_SSYSCLK (0x00U)
#endif

/* OSTS register setting */
#define SFR_VAL_OSTS        (0x02U)
/* Format of Oscillation Stabilization Time Counter Status */
/* compare OSTC register and this value                    */
#define VAL_X1_OSCSTAB_MSK  (0xE0U)

#if defined(CLK_HOCO)
#define SFRB_VAL_MSTOP      1
#define SFRB_VAL_MCM0       0
#define SFRB_VAL_HIOSTOP    0
#else
#define SFRB_VAL_MSTOP      0
#define SFRB_VAL_MCM0       1
#define SFRB_VAL_HIOSTOP    1
#endif

#if defined(CLKS_XT1) || \
    defined(CLKS_EX)
#define SFRB_VAL_XTSTOP     0
#else
#define SFRB_VAL_XTSTOP     1
#endif

#if defined(CLKS_XT1) || \
    defined(CLKS_EX)
/* Enables supply of subsystem clock to peripheral functions */
#define SFR_VAL_OSMC        (0x00U)
#else
/* Selection of count clock for real-time clock and 12-bit   */
/* interval timer: Low-speed on-chip oscillator clock (fIL)  */
#define SFR_VAL_OSMC        (0x10U)
#endif

#define SFRB_VAL_MCS_fMX    1

#if   defined(CLK_FCLK_4MHZ)
    #define TIMERDIV_977HZ      (0x0C)
    #define FRQ_HZ              FRQ_4MHZ
#elif defined(CLK_FCLK_8MHZ)
    #define TIMERDIV_977HZ      (0x0D)
    #define FRQ_HZ              FRQ_8MHZ
#elif defined(CLK_FCLK_16MHZ)
    #define TIMERDIV_977HZ      (0x0E)
    #define FRQ_HZ              FRQ_16MHZ
#elif defined(CLK_FCLK_32MHZ)
    #define TIMERDIV_977HZ      (0x0F)
    #define FRQ_HZ              FRQ_32MHZ
#else
    #error "an unsupported fCLK configuration."
#endif
#define WAITTIME_XT1            (977)  /* XT1 oscillation stabilization wait time: 977count@977Hz = 1sec */


/*
 * GLOBAL VARIABLE DEFINITIONS
 ****************************************************************************************
 */
#ifndef CFG_USE_APLILET
#ifdef _USE_IAR_RL78
///Option Byte section definition
__root __far const uint8_t ll_option_bytes[OPT_BYTES_SIZE]  @ 0x00C0=
                                                                       {WATCHDOG_DISABLED,
                                                                        LVD_MODE_1_63V,
                                                                        FRQ_HZ,
                                                                        OCD_ENABLED,};

///Security Byte Definition
__root __far const uint8_t ll_secu_id[SECU_ID_SIZE] @ 0x00C4=
                                                         { 0xff, 0xff, 0xff, 0xff, 0xff,
                                                           0xff, 0xff, 0xff, 0xff, 0xff };
#endif //_USE_STD_C99
#endif //CFG_USE_APLILET

#define CLK_OPTIONBYTE  ((__far uint8_t *)(0x0000C0))

/*
 * FUNCTION DEFINITIONS
 ****************************************************************************************
 */
#ifdef USE_FW_UPDATE_PROFILE
extern bool check_fw_update( void );
#endif
#ifdef CFG_USE_APLILET
extern void R_Systeminit(void);
#endif

#if defined(CLKS_XT1)
/****************************************************************************************
 * @brief Static function for wait by timer within HALT state
****************************************************************************************/
_PLF_CODE static void plf_timer_wait(const uint16_t div, const uint16_t cnt)
{
    uint8_t  tau0en;
    uint16_t tps0;

    /* Clock Setting: CK01=div */

    tau0en = (uint8_t)read_sfr(TAU0EN);
    tps0   = (read_sfrp(TPS0) & (~0x00F0)) | (div << 4);

    GLOBAL_INT_DISABLE();

    /* start timer */
    write1_sfrbit(TAU0EN, 1);       /* clock on         */
    write1_sfrbit(TMMK01, 1);       /* interrupt mask   */
    write_sfr(TT0L, 2);             /* timer stop       */
    write_sfrp(TPS0, tps0);         /* clock setting    */
    write_sfrp(TMR01, 0x8000);      /* mode set         */
    write_sfrp(TDR01, cnt);         /* counter set      */
    write1_sfrbit(TMIF01, 0);       /* interrupt clear  */
    write1_sfrbit(TMMK01, 0);       /* interrupt unmask */
    write_sfr(TS0L, 2);             /* timer start      */

    __halt();

    /* wait timer expired */
    while (0 == read_sfr(TMIF01));

    /* stop timer */
    write_sfr(TT0L, 2);             /* timer stop       */
    write1_sfrbit(TMMK01, 1);       /* interrupt mask   */
    write1_sfrbit(TMIF01, 0);       /* interrupt clear  */
    write1_sfrbit(TAU0EN, tau0en);  /* clock off        */

    GLOBAL_INT_RESTORE();
}
#endif

/**
 *****************************************************************************************
 * @brief Static function for setting for the BLE HW interrupt
 *****************************************************************************************
 */
_PLF_CODE static void plf_irq_init(void)
{
    //falling edge detection
    write1_sfr(EGN1, 3, 1);

    //set interrupt priority as level2
    write1_sfrbit(PPR111, 1);
    write1_sfrbit(PPR011, 0);

    //clear interrupt request flag
    write1_sfrbit(PIF11, IRQ_CLEAR);

    //enable ext. INTP11 interrupt
    write1_sfrbit(PMK11, IRQ_ENABLE);
}

/**
 *****************************************************************************************
 * @brief Static function for the RF to setting Clock Output as sleep clock
 *****************************************************************************************
 */
_PLF_CODE static void plf_clkoutput_init(const uint8_t plf_flg)
{
    //Configuration of Clock Output
    switch(plf_flg)
    {
        case PLF_PCLBUZ_16KHZ:
            write_sfr(CKS0, 0x09); /* Supply of 16.384kHz clock */
            break;
        case PLF_PCLBUZ_32KHZ:
            write_sfr(CKS0, 0x08); /* Supply of 32.768kHz clock */
            break;
        default:
            /* no clock output */
            return;
    }

#ifdef CLKS_XT1
    write1_sfr(P7,  6, 1);
    write1_sfr(PM7, 6, 0);
#endif

    //Output port settings
    write1_sfr(PM14, 0, 0);
    write1_sfr(P14, 0, 0);

    write1_sfrbit(PCLOE0, 1); /* Output enable */
}

/**
 *****************************************************************************************
 * @brief Static function for port setting
 *****************************************************************************************
 */
_PLF_CODE static void plf_port_init(void)
{
#ifndef CONFIG_EMBEDDED
    /* set the serial output pin, to avoid falling low-level */
    write1_sfr(P1, 2, PORT_LATCH);  // for UART-TxD0
    /* set the serial direction pin, to avoid falling low-level */
    write1_sfr(P2, 3, PORT_LATCH);  // for SDIR
#endif

    //P0.2 Digital i/o
    write1_sfr(PMC0, 2, 0);
    //P0.3 Digital i/o
    write1_sfr(PMC0, 3, 0);
    //A/D port configuration
    write_sfr(ADPC, 0x01);

#ifndef CFG_USE_APLILET
    #if !defined(PM8)
    #define PM8         (*(volatile __near unsigned char  *)0xFF28)
    #endif
    #if !defined(PM10)
    #define PM10        (*(volatile __near unsigned char  *)0xFF2A)
    #endif
    #if !defined(PM11)
    #define PM11        (*(volatile __near unsigned char  *)0xFF2B)
    #endif
    #if !defined(PM15)
    #define PM15        (*(volatile __near unsigned char  *)0xFF2F)
    #endif

    /*Pin I/O mode setting*/
    /* Below setting is for the case of ADPC != 0x00. */
    /* If ADPC == 0x00, it is not necessary to set PM24-PM27 and PM150-PM156 to output mode. */
    write_sfr(PM0,  0xEF);
    write_sfr(PM1,  0x7F);
    write_sfr(PM2,  0x0F);
    write_sfr(PM3,  0xFD);
    write_sfr(PM4,  0x01);
    write_sfr(PM5,  0x00);
    write_sfr(PM6,  0x03);
    write_sfr(PM7,  0xF7);
    write_sfr(PM8,  0x00);
    write_sfr(PM10, 0xF8);
    write_sfr(PM11, 0xFC);
    write_sfr(PM12, 0xFF);
    write_sfr(PM14, 0x81);
    write_sfr(PM15, 0x80);
#endif
}

_PLF_CODE void plf_init(const uint8_t plf_flg)
{
    uint8_t     op_byte = (uint8_t)(CLK_OPTIONBYTE[2]);
#if defined(CLK_X1_LOW) || \
    defined(CLK_X1_HIGH)
    uint8_t     temp_stab_set, temp_stab_wait;
#endif

    // chg mcu clock
    write_sfr(HOCODIV, (uint8_t)(op_byte & 0x07));      // equal optionbyte

    // calc waitloop
    clk_waitinit();

    //port settings for port mode(A/D)
    plf_port_init();

#ifdef CFG_USE_APLILET
    R_Systeminit();
#else //CFG_USE_APLILET
    /* CMC(Clock operation mode control) register */
    write_sfr(CMC, (SFR_VAL_CMC_MSYSCLK | SFR_VAL_CMC_SSYSCLK));

#if defined(CLK_X1_LOW) || \
    defined(CLK_X1_HIGH)
    /* OSTS(Oscillation stabilization time select) register */
    write_sfr(OSTS, SFR_VAL_OSTS);
#endif

    /* MSTOP bit                                      */
    /* 0: X1 oscillator or External main system clock */
    /* 1: High-speed on-chip oscillator               */
    write1_sfrbit(MSTOP, SFRB_VAL_MSTOP);

#if defined(CLK_X1_LOW) || \
    defined(CLK_X1_HIGH)
    /* X1 Oscillation stabilization wait */
    temp_stab_set = VAL_X1_OSCSTAB_MSK;
    do
    {
        temp_stab_wait  = read_sfr(OSTC);
        temp_stab_wait &= temp_stab_set;
    }
    while (temp_stab_wait != temp_stab_set);
#endif

    /* MCM0 bit (Main system clock (fMAIN) operation control)                                    */
    /* 0: Selects the high-speed on-chip oscillator clock (fIH) as the main system clock (fMAIN) */
    /* 1: Selects the high-speed system clock (fMX) as the main system clock (fMAIN)             */
    write1_sfrbit(MCM0, SFRB_VAL_MCM0);

    /* XTSTOP bit (Subsystem clock operation control)                         */
    /* 0: XT1 oscillator operating or External clock from EXCLKS pin is valid */
    /* 1: XT1 oscillator stopped or External clock from EXCLKS pin is invalid */
    write1_sfrbit(XTSTOP, SFRB_VAL_XTSTOP);

#if defined(CLKS_XT1)
    // XT1 oscillation stabilization wait time.
    /* Change the waiting time according to the system */
    plf_timer_wait(TIMERDIV_977HZ, WAITTIME_XT1);
#endif

    /* OSMC(Subsystem clock supply mode control) register */
    write_sfr(OSMC, SFR_VAL_OSMC);

    /* CSS bit (Selection of CPU/peripheral hardware clock) */
    /* always Main system clock (fMAIN)                     */
    write1_sfrbit(CSS, 0);

#ifdef USE_FW_UPDATE_PROFILE
    if( true == check_fw_update() )
    {
        // When the FW update, use the High-speed on-chip oscillator clock
        write1_sfrbit(HIOSTOP, 0);
    }
    else
    {
#endif
        /* HIOSTOP bit                                */
        /* Set fIH                                    */
        /* 0: High-speed on-chip oscillator operating */
        /* 1: High-speed on-chip oscillator stopped   */
        write1_sfrbit(HIOSTOP, SFRB_VAL_HIOSTOP);
#ifdef USE_FW_UPDATE_PROFILE
    }
#endif
#endif //CFG_USE_APLILET

    //port settings for interrupt pin from BLE core
    plf_irq_init();

    if(plf_flg != PLF_PCLBUZ_NONE)
    {
        //Configuration of Clock Output
        plf_clkoutput_init(plf_flg);
    }

    return;
}

_PLF_CODE void plf_hoco_start(void)
{
#if defined(CLK_HOCO)
    /* if Status of Main system clock (fMAIN) is High-speed system clock (fMX) */
    if(read1_sfr(CKC, 5) == SFRB_VAL_MCS_fMX)
    {
        /* High-speed on-chip oscillator operating */
        write1_sfrbit(HIOSTOP, 0);
        /* stabilization wait time */
        clk_waitfunc(CLK_WAIT_30_USEC);
    }
#else /* CLK_HOCO */
    /* High-speed on-chip oscillator operating */
    write1_sfrbit(HIOSTOP, 0);
    /* stabilization wait time */
    clk_waitfunc(CLK_WAIT_30_USEC);
#endif/* CLK_HOCO */
}

_PLF_CODE void plf_hoco_stop(void)
{
#if defined(CLK_HOCO)
    /* if Status of Main system clock (fMAIN) is High-speed system clock (fMX) */
    if(read1_sfr(CKC, 5) == SFRB_VAL_MCS_fMX)
    {
        /* High-speed on-chip oscillator stopped   */
        write1_sfrbit(HIOSTOP, 1);
    }
#else /* CLK_HOCO */
    #if defined(USE_FW_UPDATE_PROFILE)
    if( false == check_fw_update() )
    #endif/* USE_FW_UPDATE_PROFILE */
    {
        /* High-speed on-chip oscillator stopped   */
        write1_sfrbit(HIOSTOP, 1);
    }
#endif/* CLK_HOCO */
}


_CLK_TBL const uint16_t clk_table_ent[6] = {
#ifdef _USE_REL_RL78
#if defined(CLK_FCLK_4MHZ)
    8, 40,286, 70,188, 6
#elif defined(CLK_FCLK_8MHZ)
    5, 80,143, 28, 94, 4
#elif defined(CLK_FCLK_16MHZ)
    2,160,123, 10, 74, 3
#elif defined(CLK_FCLK_32MHZ)
    2,320,113, 10, 64, 2
#else
    #error "check CLK_FCLK_xxMHZ"
#endif
#endif /* _USE_REL_RL78 */

#ifdef _USE_CCRL_RL78
#if   defined(CLK_FCLK_4MHZ)
    7, 40,120, 50,188, 5
#elif defined(CLK_FCLK_8MHZ)
    3, 80,113, 10, 94, 3
#elif defined(CLK_FCLK_16MHZ)
    2,160,113,  9, 74, 3
#elif defined(CLK_FCLK_32MHZ)
    2,320,113,  9, 64, 2
#else
    #error "check CLK_FCLK_xxMHZ"
#endif
#endif /* _USE_CCRL_RL78 */

#ifdef _USE_IAR_RL78
#if defined(CLK_FCLK_4MHZ)
    7, 40,120, 60,188, 5
#elif defined(CLK_FCLK_8MHZ)
    3, 80,113, 15, 97, 4
#elif defined(CLK_FCLK_16MHZ)
    2,160,113,  3, 75, 3
#elif defined(CLK_FCLK_32MHZ)
    2,320,113, 12, 65, 2
#else
    #error "check CLK_FCLK_xxMHZ"
#endif
#endif /* _USE_IAR_RL78 */
};


/// @} module
