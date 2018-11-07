/**
 ****************************************************************************************
 *
 * @file        main.c
 *
 * @brief       Main function.
 *
 * Copyright(C) 2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */
 
#ifdef _USE_IAR_RL78
#define _MAINCNST _Pragma("location=\"MAINCNST\"")
#define _MAINCODE _Pragma("location=\"MAINCODE\"")
#define _MAINDATA _Pragma("location=\"MAINDATA\"") __NO_INIT
#else
#define _MAINCNST
#define _MAINCODE
#define _MAINDATA

#if !defined(_USE_CCRL_RL78)
#pragma section @@CNST MAINCNST
#pragma section @@CODEL MAINCODE
#pragma section @@DATA MAINDATA
#else
#pragma section const MAINCNST
#pragma section text  MAINCODE
#pragma section bss   MAINDATA
#endif
#endif

#include "arch.h"
#include "ll.h"

#include "rwke_api.h"

#include "prf_config.h"
#include "db_handle.h"
#include "rwble_mem.h"
#include "dataflash.h"

#include "rwble.h"

#include "ll.h"
#include "led.h"

#include "serial.h"
#include "wakeup.h"

#include "rf.h"
#include "plf.h"

#include "rble_core_config.h"

#ifdef CONFIG_MODEM
#include "rble_modem_config.h"
#endif

#ifdef USE_FW_UPDATE_PROFILE
#include "hw_config.h"
#endif

#include "pktmon.h"


#define MIRROR_FUNC_OFFSET_ADDR     0x400
#define ACCESS_TABLE_ADDR           0x0A800

enum {
    /* function */
    DMAIN_ARCH_MAIN_IDX,
    DMAIN_PLATFORM_RESET_IDX,
    DMAIN_SLEEP_CONT_IDX,
    DMAIN_WAKEUP_INIT_IDX,
    DMAIN_RBLE_USR_SET_PARAM_IDX,
    /* global num */
    DMAIN_CLK_TABLE_IDX,
    DMAIN_ATTS_DESC_LIST_IDX,
    DMAIN_KE_EVT_HDLR_IDX,
    DMAIN_KE_MEM_HEAP_IDX,
    DMAIN_KE_MEM_HEAP_END_IDX
};

typedef void        (*DMAIN_ARCH_MAIN)(void);
typedef void        (*DMAIN_PLATFORM_RESET)(uint32_t error);
typedef bool        (*DMAIN_SLEEP_CONT)(void);
typedef void        (*DMAIN_WAKEUP_INIT)(void);
typedef RBLE_STATUS (*DMAIN_RBLE_USR_SET_PARAM)( uint8_t param_id, uint8_t param_len, uint8_t *param_data );


_MAINDATA uint32_t *access_table;

_MAINDATA evt_ptr_t *ke_evt_hdlr;
_MAINDATA struct ke_task_desc *TASK_DESC;
_MAINDATA uint8_t *ke_mem_heap;
_MAINDATA uint8_t *ke_mem_heap_end;
_MAINDATA uint16_t *clk_table;
_MAINDATA int16_t  ble_connection_max;

extern void arch_main(void);
#ifdef USE_FW_UPDATE_PROFILE
bool check_fw_update( void );
static void host_db_init(void);

extern const __far uint16_t fw_update_count_0;
extern const __far uint16_t fw_update_count_1;

extern uint16_t ATT_LAST_IDX;
extern struct atts_desc *atts_desc_list;

extern uint16_t loco_cal_period;
extern uint16_t task_id_max;

/** Attribute Database */
extern const struct atts_desc atts_desc_list_host[];
extern const uint16_t ATT_HOST_DB_SIZE;

extern PacketMonitorFunc PacketMonitorFunc_Ptr;
#endif

extern const uint32_t access_table_ent[];

extern rble_prof_init_func_t *prof_init_table;
extern RBLE_PROF_VERSION *rble_prof_version_table;
extern RBLE_CORE_SEL_PROF_HANDLER *rble_core_sel_handler_table;
extern struct ke_msg_handler *rble_core_handler;

#ifdef CONFIG_MODEM
extern RBLE_MODEM_SEL_CMD_EXE *rble_modem_sel_cmd_exe_table;

_MAINCNST const uint16_t    rscip_wait_timer_count = 30;        // RSCIP Wait Timer count (300msec = 30 * 10msec)
#endif

/* external function */
extern bool peak_init(uint16_t  peak_time);
CALLT extern uint16_t atts_get_attrhdl(uint16_t idx);
extern void spi_init(void);
extern void sleep_init(void);
#ifdef USE_FW_UPDATE_PROFILE
extern void RBLE_FWUP_CallBack( RBLE_MODE Mode );

_MAINCODE static void runtime_dummy_func(void)
{
#if defined(_USE_CCRL_RL78)
    volatile float i=0,l=3;
    volatile int32_t j=0;
    volatile uint32_t k=3;
#else
    float i=0,l=3;
    int32_t j=0;
    uint32_t k=3;
#endif
    /*fdiv*/
    i=i/l;
    /*lstof*/
    i=(float)j;
    /*lutof*/
    i=(float)k;
    /* fadd */
    i=i+l;
    /*ftols*/
    j=(int32_t)i;
    /*fmul*/
    i=i*l;
    j=j>>1;
}
#endif

#ifndef _USE_IAR_RL78
  #if   defined(_USE_R5F11AGG)
    #define HOST_AREA_START 0xFCF00
  #elif defined(_USE_R5F11AGH)
    #define HOST_AREA_START 0xFBF00
  #else
    #define HOST_AREA_START 0xFB310
  #endif
    #ifdef USE_FW_UPDATE_PROFILE
        #ifdef CONFIG_MODEM
            #define HOST_AREA_SIZE  0x0F00
        #else
            #define HOST_AREA_SIZE  0x0A00
        #endif
    #else
        #ifdef CONFIG_MODEM
            #define HOST_AREA_SIZE  0x0E60
        #else
            #define HOST_AREA_SIZE  0x0960
        #endif
    #endif
#endif

_MAINCODE void init_data_section_area( void )
{
#ifdef _USE_IAR_RL78
#if __VER__ <= 140
    /* Before IAR v1.40 */
    uint32_t sec_top;
    uint32_t sec_end;
    #pragma section = "UARTDATA"
    #pragma section = "CNT_DATA"

    sec_top = (uint32_t)(__section_begin("CNT_DATA"));
    sec_end = (uint32_t)(__section_end("UARTDATA"));

    memset((uint8_t*)(uint8_t __far*)(sec_top), 0, sec_end-sec_top);
#endif
#elif defined(_USE_CCRL_RL78)
    struct bsec_t {
        uint8_t __near *ram_sectop; /* section top address */
        uint8_t __near *ram_secend; /* section end address + 1 */
    } bsec_table[] = {
        { (uint8_t __near *)__sectop("CNT_DATA_n"), (uint8_t __near *)__secend("CNT_DATA_n") },
        //{ (uint8_t __near *)__sectop("MAINDATA_n"), (uint8_t __near *)__secend("MAINDATA_n") },
        { (uint8_t __near *)__sectop("HDB_DATA_n"), (uint8_t __near *)__secend("HDB_DATA_n") },
        { (uint8_t __near *)__sectop("HST_DATA_n"), (uint8_t __near *)__secend("HST_DATA_n") },
        { (uint8_t __near *)__sectop("RBL_DATA_n"), (uint8_t __near *)__secend("RBL_DATA_n") },
        #ifdef USE_FW_UPDATE_PROFILE
        { (uint8_t __near *)__sectop("CFL_DATA_n"), (uint8_t __near *)__secend("CFL_DATA_n") },
        #endif//USE_FW_UPDATE_PROFILE
        #ifdef CFG_USE_EEL
        { (uint8_t __near *)__sectop("DFL_DATA_n"), (uint8_t __near *)__secend("DFL_DATA_n") },
        #endif//CFG_USE_EEL
        { (uint8_t __near *)__sectop("UARTDATA_n"), (uint8_t __near *)__secend("UARTDATA_n") },

        { NULL, NULL }  /* stopper */
    };
    struct bsec_t   *target;
    uint8_t __near  *ram_p;
    for( target = &bsec_table[0] ; target->ram_sectop != NULL; target++ )
    {
        ram_p = target->ram_sectop;
        for( ; ram_p != target->ram_secend; ram_p++ )
        {
            *ram_p = 0;
        }
    }
    #ifdef USE_FW_UPDATE_PROFILE
    /* Runtime library Force link for F/W updates */
    runtime_dummy_func();
    #endif
#else
    memset((uint8_t *)HOST_AREA_START, 0, HOST_AREA_SIZE);
#endif
}

_MAINCODE void main( void )
{
    uint32_t offset_addr = 0;
    
    init_data_section_area();
    
#ifdef USE_FW_UPDATE_PROFILE
    /* Initializing the address table, depending on the times of FW updates. */
    if( fw_update_count_0 % 2 == 1 ) {
        offset_addr += MIRROR_FUNC_OFFSET_ADDR;
    }
#endif
    access_table        = (uint32_t *)(uint32_t __far*)((uint32_t)access_table_ent + offset_addr);

    /* rom */
    clk_table           = (uint16_t *)(uint16_t  __far*)(access_table[DMAIN_CLK_TABLE_IDX] + offset_addr);
    ke_evt_hdlr         = (evt_ptr_t *)(evt_ptr_t __far*)(access_table[DMAIN_KE_EVT_HDLR_IDX] + offset_addr);
    TASK_DESC           = (struct ke_task_desc *)(struct ke_task_desc __far*)(access_table[DMAIN_ATTS_DESC_LIST_IDX] + offset_addr);
    
    /* ram */
    ke_mem_heap         = (uint8_t *)(uint8_t __far*)(access_table[DMAIN_KE_MEM_HEAP_IDX]);
    ke_mem_heap_end     = (uint8_t *)(uint8_t __far*)(access_table[DMAIN_KE_MEM_HEAP_END_IDX]);
    
    /* init table address */
    rble_core_handler = NULL;
    rble_core_sel_handler_table = NULL;
#ifdef CONFIG_MODEM
    rble_modem_sel_cmd_exe_table = NULL;
#endif
    prof_init_table = NULL;
    rble_prof_version_table = NULL;
#ifdef USE_FW_UPDATE_PROFILE
    /* during FW update? */
    if( true == check_fw_update() ) {
        /* Initializing BLE stack */
        struct bd_addr public_addr;     /* Public Device Address, set to rwble_init() */

        /* Disable parity error resets */
        RPECTL = 0x80;

        ble_connection_max = 1;

        // Initialize heap memory
        ke_init();
        {
            rwble_set_mem_cont();

            /* gap memory allocate */
            ( *rwble_set_mem_gap_func )();

            /* l2cap memory allocate */
            ( *rwble_set_mem_l2cap_func )();
            /* gatt memory allocate */
            ( *rwble_set_mem_gatt_func )();
            /* att memory allocate */
            ( *rwble_set_mem_att_func )();
            /* smp memory allocate */
            ( *rwble_set_mem_smp_func )();
        }

        /* initialise LOCO CAL period */
        loco_cal_period = CFG_LOCO_CAL_PERIOD;
        /* initialise max task id */
        task_id_max = TASK_APP+1;
        /* initialise data base(host) */
        ATT_LAST_IDX = ATT_HOST_DB_SIZE / sizeof(struct atts_desc);
        
        atts_desc_list = (struct atts_desc *)ke_malloc(ATT_HOST_DB_SIZE);
        memcpy(atts_desc_list, atts_desc_list_host, ATT_HOST_DB_SIZE);
        
        host_db_init();
        
        // init peak time
        peak_init( 0 );     // not peak

        //init MCU clocks
        plf_init(CFG_PLF_INIT);

        // Initialize the CSI21 module
        spi_init();

#ifdef CFG_LOG
        log_output_init();
#endif // CFG_LOG

		// Do not use PacketMonitor
		PacketMonitorFunc_Ptr = NULL;

        /* Initialize sleep driver */
        sleep_init();

        /* init dataflash driver */
        dataflash_init();
    
        /* get device address                                                       */
        /*  - If device address is registered in DataFlash, get from DataFlash.     */
        /*  - If device address is not registered in DataFlash, get from CodeFlash. */
        /*  - If device address is all 0xFF after above processing,                 */
        /*    get from CFG_TEST_BDADDR macro defined in config.h.                   */
        flash_get_bda(&public_addr);

        // Disable the BLE core
        rwble_disable();

        // Initialize RF
        rf_init(CFG_RF_INIT);

        // Initialize BLE stack
        rwble_init(&public_addr, CFG_SCA);

        // Enable the BLE core
        rwble_enable();

        // finally start interrupt handling
        GLOBAL_INT_START();
        
        RBLE_Init(&RBLE_FWUP_CallBack);
        
        // And loop forever
        for (;;)
        {
            // schedule the BLE stack
            rwble_schedule();
        }
    }
    else
#endif
    {
        /* call arch main */
        arch_main();
    }
}

#ifdef USE_FW_UPDATE_PROFILE
_MAINCODE bool check_fw_update( void )
{   
    if( (fw_update_count_1 > fw_update_count_0) || (fw_update_count_1 == 0xFFFF) ) {
        /* during FW update */
        return true;
    }
    else {
        return false;
    }
}

_MAINCODE static void host_db_init(void)
{
    gap_dev_name.namelen = 0;
    atts_desc_list[GAP_HDL_DEVNAME].length = 0;

    gap_discovery_scan_time             = GAP_DEV_SEARCH_TIME;
    gap_dev_search_scan_intv            = GAP_DEV_SEARCH_SCAN_INTV;
    gap_dev_search_scan_window          = GAP_DEV_SEARCH_SCAN_WINDOW;
    gap_lim_adv_timeout                 = GAP_LIM_ADV_TIMEOUT;
    gap_scan_fast_intv                  = GAP_SCAN_FAST_INTV;
    gap_scan_fast_window                = GAP_SCAN_FAST_WINDOW;
    gap_init_conn_min_intv              = GAP_INIT_CONN_MIN_INTV;
    gap_init_conn_max_intv              = GAP_INIT_CONN_MAX_INTV;
    gap_conn_min_ce_length              = GAP_CONN_MIN_CE_LENGTH;
    gap_conn_max_ce_length              = GAP_CONN_MAX_CE_LENGTH;
    gap_conn_slave_latency              = GAP_CONN_SLAVE_LATENCY;
    gap_dev_supervision_timeout         = GAP_DEV_SUPERVISION_TIMEOUT;
    gap_resolvable_private_addr_intv    = GAP_RESOLVABLE_PRIVATE_ADDR_INTV;
    gap_appearance_char_val             = GAP_APPEARANCE_CHAR_VAL;
    gatt_service_changed_ind_en         = GATT_SERVICE_CHANGED_IND_EN;
    gatt_service_changed[0]             = (uint8_t)(GATT_SERVICE_CHANGED_START_HDL);
    gatt_service_changed[1]             = (uint8_t)(GATT_SERVICE_CHANGED_START_HDL >> 8);   /* start handle */
    gatt_service_changed[2]             = (uint8_t)(GATT_SERVICE_CHANGED_END_HDL);
    gatt_service_changed[3]             = (uint8_t)(GATT_SERVICE_CHANGED_END_HDL >> 8);     /* end handle */
}
#endif //USE_FW_UPDATE_PROFILE

_MAINCODE void arch_main(void)
{
    uint32_t func_addr;
    
    func_addr = access_table[DMAIN_ARCH_MAIN_IDX];
    ((DMAIN_ARCH_MAIN)func_addr)();
    return;
}

_MAINCODE void platform_reset(uint32_t error)
{
    uint32_t func_addr;
    
    func_addr = access_table[DMAIN_PLATFORM_RESET_IDX];
    ((DMAIN_PLATFORM_RESET)(func_addr))(error);
    return;
}


_MAINCODE void wakeup_init(void)
{
#ifdef CONFIG_MODEM
    uint32_t func_addr;

    #ifdef USE_FW_UPDATE_PROFILE
    if( false == check_fw_update() )
    #endif
    {
        func_addr = access_table[DMAIN_WAKEUP_INIT_IDX];
        ((DMAIN_WAKEUP_INIT)(func_addr))();
    }
#endif
}

_MAINCODE bool sleep_cont(void)
{
    uint32_t func_addr;
    
    func_addr = access_table[DMAIN_SLEEP_CONT_IDX];
    return ((DMAIN_SLEEP_CONT)(func_addr))();
}

/* User Sample */
_MAINCODE void    peak_start(void)
{
#ifdef CFG_USE_PEAK
    /* OUTPUT PORT ON */
    write1_sfrbit(LED01, OFF);
#endif
}

/* User Sample */
_MAINCODE void    peak_end(void)
{
#ifdef CFG_USE_PEAK
    /* OUTPUT PORT OFF */
    write1_sfrbit(LED01, ON );
#endif
}

_MAINCODE RBLE_STATUS RBLE_User_Set_Params_call_func( uint8_t param_id, uint8_t param_len, uint8_t *param_data )
{
    uint32_t func_addr;
    
    func_addr = access_table[DMAIN_RBLE_USR_SET_PARAM_IDX];
    return ((DMAIN_RBLE_USR_SET_PARAM)(func_addr))( param_id, param_len, param_data );
}

