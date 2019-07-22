/**
 ****************************************************************************************
 *
 * @file        arch_main.c
 *
 * @brief       Main loop of the application.
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */
 
#ifdef _USE_IAR_RL78
#define _ACS_TBL _Pragma("location=\"ACS_TBL\"")
#else
#define _ACS_TBL
#if !defined(_USE_CCRL_RL78)
#pragma section @@CNST ACS_TBL
#else
#pragma section const  ACS_TBL
#endif
#endif

#include "arch.h"
#include "config.h"
#include "hw_config.h"
#include "plf.h"

#include "led.h"

#include "rwble.h"

#include "ll.h"

#include "serial.h"
#include "wakeup.h"

#include "rf.h"
#include "rwke_api.h"
#include "dataflash.h"
#include "push_sw.h"

#include "rble.h"
#ifndef CONFIG_EMBEDDED
extern RBLE_STATUS RBLE_Application( void );
#include "rble_api.h"
#include "rscip_api.h"
#include "rble_modem_config.h"
#else
#include "rble_app.h"
#include "r_vuart_console.h"
#endif
#ifdef CFG_SAMPLE_BEACON
#include "beacon_app.h"
#endif

#include "rwble_mem.h"
#include "prf_config.h"
#include "rble_core_config.h"

#if __DTM2WIRE_UART_USE__
#include "DTM2wire.h"
#endif

#include "pktmon.h"

//RD8001対応
#include	"header.h"		//ユーザー定義


void arch_main_ent(void);
void variables_init(void);
static void host_db_init(void);
void platform_reset_ent(uint32_t error);
bool sleep_cont_ent(void);
RBLE_STATUS RBLE_User_Set_Params( uint8_t param_id, uint8_t param_len, uint8_t *param_data );

extern void spi_init(void);
extern void sleep_init(void);
extern void sleep_load_data(void);
extern bool sleep_check_enable(void);
extern bool peak_init(uint16_t  peak_time);
extern void FW_Update_push_sw_int(void);
extern void input_rand_value(uint16_t val, uint32_t uinfo_top);


extern const evt_ptr_t ke_evt_hdlr_ent[];
extern const struct ke_task_desc TASK_DESC_ent[];
extern const uint16_t clk_table_ent[];

extern RBLE_CORE_SEL_PROF_HANDLER *rble_core_sel_handler_table;
#ifdef CONFIG_MODEM
extern RBLE_MODEM_SEL_CMD_EXE *rble_modem_sel_cmd_exe_table;
#endif
extern rble_prof_init_func_t *prof_init_table;
extern RBLE_PROF_VERSION *rble_prof_version_table;
extern uint16_t loco_cal_period;
extern uint16_t task_id_max;
extern PacketMonitorFunc PacketMonitorFunc_Ptr;

/**
 * @addtogroup PLATFORM_DRIVERS
 * @{
 */

uint8_t ke_mem_heap_ent[BLE_HEAP_SIZE];

_ACS_TBL const uint32_t access_table_ent[] = 
{
    (uint32_t)arch_main_ent,
    (uint32_t)platform_reset_ent,
    (uint32_t)sleep_cont_ent,
#ifdef CONFIG_MODEM
    (uint32_t)wakeup_init_ent,
#else
    0,
#endif
    (uint32_t)RBLE_User_Set_Params,
    (uint32_t)&clk_table_ent[0],
    (uint32_t)&TASK_DESC_ent[0],
    (uint32_t)&ke_evt_hdlr_ent[0],
    (uint32_t)&ke_mem_heap_ent[0],
    (uint32_t)&ke_mem_heap_ent[BLE_HEAP_SIZE]
};



/**
 ****************************************************************************************
 * @brief Print the assertion error reason and loop forever.
 *
 * @param condition C string containing the condition.
 * @param file C string containing file where the assertion is located.
 * @param line Line number in the file where the assertion is located.
 ****************************************************************************************
 */
void assert_err(const int8_t *condition, const int8_t * file, const int16_t line)
{
    volatile uint8_t  dbg_assert_block = 1;
    volatile uint32_t assert_time;

#ifdef CFG_LOG
    LOG_OUTPUT("ASSERT_ERROR! %s %s %d\r\n", condition, file, line);
#endif // CFG_LOG
    GLOBAL_INT_STOP();
    assert_time = lld_evt_time_get();
    while(dbg_assert_block);
}

/**
 ****************************************************************************************
 * @brief Print the assertion warning reason.
 *
 * @param condition C string containing the condition.
 * @param file C string containing file where the assertion is located.
 * @param line Line number in the file where the assertion is located.
 ****************************************************************************************
 */
void assert_warn(const int8_t *condition, const int8_t * file, const int16_t line)
{
#ifdef CFG_LOG
    LOG_OUTPUT("ASSERT_WARN %s %s %d\r\n", condition, file, line);
#endif // CFG_LOG
}

void platform_reset_ent(uint32_t error)
{
typedef void (* RESET_FUNC )( void );

    uint16_t        Func_Data;
    RESET_FUNC      Func;

    switch( error )
    {
        /* fatal problem in software design (HEAP area is insufficient) */
        case CO_ERROR_HW_MEM_ALLOC_FAIL:
            /* LED1/2 OFF */
//            write1_sfrbit(LED01, OFF);
//            write1_sfrbit(LED02, OFF);
            break;

        /* fatal problem in board circuit design */
        /* (6.8M ohm must be implemented between port XTAL1_RF and GND(port AVSS_RF)) */
        case CO_ERROR_HW_RF_INITIAL_ERR:
        case CO_ERROR_HW_RF_WAKEUP_ERR:
            /* LED1/2 ON */
//            write1_sfrbit(LED01, ON);
//            write1_sfrbit(LED02, ON);
            break;

        /* other error */
        default:
            break;
    }

	/* dummy wait 1sec */
    for( Func_Data = 0; Func_Data < 1000; Func_Data++ )
    {
        clk_waitfunc( CLK_WAIT_1000_USEC );
    }

    /* RF RESET Function */

    /* Reset by illegal structure */
    Func_Data = 0xffff;
    Func = ( RESET_FUNC )&Func_Data;
    ( *Func )();

    //should reset firmware after ending all UART transfers and saving some context
    ASSERT_ERR(0);
}

	
/**
 ****************************************************************************************
 * @brief Main BLE Ref IP entry point.
 *
 * This function is called right after the booting process has completed.
 ****************************************************************************************
 */
void arch_main_ent(void)
{
   struct bd_addr public_addr;     /* Public Device Address */

    /* Disable parity error resets */
    RPECTL = 0x80;

//  rwble_set_mem();

    /*
     ************************************************************************************
     * BLE configuration initialization
     ************************************************************************************
     */
    ble_connection_max  = BLE_CONN_MAX;

    // Initialize heap memory
    ke_init();
    rwble_set_mem();

    /*
     ************************************************************************************
     * Platform initialization
     ************************************************************************************
     */
     user_system_init();		// RD8001対応：ユーザーシステムドライバ初期化 ※BLE処理前に行う
     
    // init global variables
    variables_init();

    // init host database
    host_db_init();

    // init peak time
#ifndef CFG_USE_PEAK
    peak_init( 0 );     // not peak
#else
    peak_init( 1 );     // 1ms peak
#endif

    //init MCU clocks
    plf_init(CFG_PLF_INIT);

    //init LED
//    led_init();

    // Initialize the CSI21 module
    spi_init();

#ifdef CFG_LOG
    log_output_init();
#endif // CFG_LOG
#ifdef CFG_PKTMON
    PacketMonitorFunc_Ptr = pkt_mon_output;
    pkt_mon_init();
#else
    PacketMonitorFunc_Ptr = NULL;
#endif // CFG_PKTMON

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

    /*
     ************************************************************************************
     * BLE initializations
     ************************************************************************************
     */
    // Disable the BLE core
    rwble_disable();

    // Initialize RF
    rf_init(CFG_RF_INIT);

    // input user rundom seed
    input_rand_value(0, userinfo_top);

    // Initialize BLE stack
    rwble_init(&public_addr, CFG_SCA);

    // Enable the BLE core
    rwble_enable();

    // finally start interrupt handling
    GLOBAL_INT_START();
    
#ifdef USE_FW_UPDATE_PROFILE
//    push_sw2_start( &FW_Update_push_sw_int );
#endif

    // rBLE Initialize
#ifndef CONFIG_EMBEDDED
#if __DTM2WIRE_UART_USE__
{
    DTM2Wire_Init();

    for ( ; _RBLE_RUN_MODE_INIT == rBLE_Run_Mode_Flg; ) {
    }
    if ( _RBLE_RUN_MODE_NORMAL == rBLE_Run_Mode_Flg ) {
#endif
        RBLE_Application();
#if __DTM2WIRE_UART_USE__
    } else {
        DTM2Wire_Start();
    }
}
#endif
#else
#if __DTM2WIRE_UART_USE__
{
    if ( true == DTM2Wire_Check() ) {
        rBLE_Run_Mode_Flg = _RBLE_RUN_MODE_NORMAL;
#endif
#ifdef  CFG_SAMPLE
        RBLE_App_Init();
#elif defined(CFG_SAMPLE_BEACON)
        APP_Init();
#endif
#if __DTM2WIRE_UART_USE__
    } else {
        rBLE_Run_Mode_Flg = _RBLE_RUN_MODE_DIRECT;
        DTM2Wire_Start();
    }
}
#endif
#endif
	//RD8001対応：ユーザーアプリ初期化
	user_main_init();
	main_set_bd_adrs( (UB*)&public_addr.addr[0] );

    // And loop forever
    for (;;)
    {
        //LED activity
//        led_blink();
		// ユーザータイマー処理
		user_main_timer_cyc();

        // schedule the BLE stack
        // rwble_schedule();

        // Checks for sleep have to be done with interrupt disabled
        GLOBAL_INT_DISABLE();
        // Check if the processor clock can be gated
        if ((uint16_t)rwble_sleep() != false)
        {
            // check CPU can sleep
            if ((uint16_t)sleep_check_enable() != false)
            {
                 /* Before CPU enters stop mode, this function must be called */
                if ((uint16_t)wakeup_ready() != false)
                 {
                    // Wait for interrupt
                    NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
//					write1_sfr(P1, 5, 1);		// 空きポートによるスリープ計測
                    WFI();
                    NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
//					write1_sfr(P1, 5, 0);		// 空きポートによるスリープ計測
                    /* After CPU is released stop mode, this function must be called immediately */
                    wakeup_finish();
                }
            }
        }
        // Checks for sleep have to be done with interrupt disabled
        GLOBAL_INT_RESTORE();

        sleep_load_data();
    }
}

void variables_init(void)
{
    /* variables initialization */
    loco_cal_period = CFG_LOCO_CAL_PERIOD;
    task_id_max = TASK_MAX;
    
    /* make databese */
    atts_desc_list = (struct atts_desc *)ke_malloc(ATT_HOST_DB_SIZE + ATT_PRF_DB_SIZE);
    
    memcpy(&atts_desc_list[0], &atts_desc_list_host[0], ATT_HOST_DB_SIZE);
    memcpy(&atts_desc_list[ATT_HOST_DB_SIZE/sizeof(struct atts_desc)], &atts_desc_list_prf[0], ATT_PRF_DB_SIZE);
    
    ATT_LAST_IDX = (ATT_HOST_DB_SIZE + ATT_PRF_DB_SIZE) / sizeof(struct atts_desc);
    
    rble_core_sel_handler_table = (RBLE_CORE_SEL_PROF_HANDLER *)rble_core_sel_handler_table_ent;
#ifdef CONFIG_MODEM
    rble_modem_sel_cmd_exe_table = (RBLE_MODEM_SEL_CMD_EXE *)rble_modem_sel_cmd_exe_table_ent;
#endif
    prof_init_table = (rble_prof_init_func_t *)prof_init_table_ent;
    rble_prof_version_table = (RBLE_PROF_VERSION *)rble_prof_version_table_ent;
}

static void host_db_init(void)
{
    if((devce_name_ptr->namelen > 0) && (devce_name_ptr->namelen <= RBLE_BD_NAME_SIZE))
    {
        gap_dev_name = *devce_name_ptr;
    }
    else
    {
        gap_dev_name.namelen = GAP_DEV_NAME_SIZE;
        memcpy(&gap_dev_name.name[0], GAP_DEV_NAME, GAP_DEV_NAME_SIZE);
    }
    atts_desc_list[GAP_HDL_DEVNAME].length = gap_dev_name.namelen;

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

/* User Sample */
bool sleep_check_enable(void)
{
#if 0
	return true;
#else
	bool ret;
	//RD8001暫定：スリープ条件を入れる
	//むやみにスリープに入れると正常に動作しない事があった　※デファインとの組み合わせ
	ret = user_main_sleep();
	
	return ret;
#endif
}

/* sleep controll */
bool sleep_cont_ent(void)
{
    bool    ret = true;
    #ifndef CONFIG_EMBEDDED
    ret = RSCIP_Can_Sleep();
    #else // #ifndef CONFIG_EMBEDDED
    #ifdef  CFG_SAMPLE
    ret = console_can_sleep();
    #elif defined(CFG_SAMPLE_BEACON)
    ret = APP_Is_Idle();
    #endif /* #ifndef CFG_SAMPLE */
    #endif // #ifndef CONFIG_EMBEDDED

    #ifdef CFG_USE_EEL
    if(ret == true)
    {
        /* if dataflash is started, it is unable to execute stop */
        ret = dataflash_is_stopped();
    }
    #endif
    return ret;
}

RBLE_STATUS RBLE_User_Set_Params( uint8_t param_id, uint8_t param_len, uint8_t *param_data )
{
    return RBLE_OK;
}


/// @}
