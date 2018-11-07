/**
 ****************************************************************************************
 *
 * @file rwble.h
 *
 * @brief Entry points of the BLE software
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * $Rev: 19804 $
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef RWBLE_H_
#define RWBLE_H_

/**
 ****************************************************************************************
 * @addtogroup ROOT
 * @brief Entry points of the BLE stack
 *
 * This module contains the primitives that allow an application accessing and running the
 * BLE protocol stack
 *
 * @{
 ****************************************************************************************
 */


/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwble_config.h"
#include "co_bt.h"


/*
 * DEFINES
 ****************************************************************************************
 */

/// Definition of the bits preventing the system from sleeping
enum
{
    /// Flag indicating that the wake up process is ongoing
    RWBLE_WAKE_UP_ONGOING   = 0x0001,
    /// Flag indicating that a kernel event is pending
    RWBLE_KEEVENT_ONGOING   = 0x0002,
    /// Flag indicating that an HCI TX transfer is ongoing
    RWBLE_HCI_TX_ONGOING    = 0x0004,
    /// Flag indicating that an HCI RX transfer is ongoing
    RWBLE_HCI_RX_ONGOING    = 0x0008,
    /// Flag forcing to prevent sleeping from LLM
    RWBLE_LLM_PREVENT_SLEEP = 0x0010,
    /// Flag indicating that an encryption is ongoing
    RWBLE_CRYPT_ONGOING     = 0x0020,
    /// Flag indicating that test is ongoing
    RWBLE_TEST_ONGOING      = 0x0040
};

/// Definition of the sleep clock frequency
enum
{
    /// 32KHz sleep clock
    RWBLE_SCF_32000HZ,
    /// 32.768KHz sleep clock
    RWBLE_SCF_32768HZ
};


/*
 * TYPE DEFINITIONS
 ****************************************************************************************
 */

/// RW BLE environment structure
struct rwble_env_tag
{
    #if DEEP_SLEEP
    /// Interrupt mask stored during sleep
    uint32_t irq_mask;
    /// Prevent sleep bit field
    uint16_t prevent_sleep;
    #if RW_BLE_SUPPORT
    /// System wakeup delay (in slots)
    uint8_t wakeup_delay;
    /// Sleep mode enable flag
    bool sleep_enable;
    /// External wakeup enable flag
    bool ext_wakeup_enable;
    #endif //RW_BLE_SUPPORT

    /// Flag indicating if the BLE core is currently in null mode
    bool in_nmode;
    #endif //DEEP_SLEEP

    /// Flag indicating if the BLE core is currently in sleep mode
    bool sleeping;
};

/// API functions of the RF driver that are used by the BLE or BT software
struct rw_rf_api
{
    /// Function called upon HCI reset command reception
    void (*reset)(void);
    /// Function called when TX power has to be increased for a specific link id
    bool (*txpwr_inc)(uint8_t);
    /// Function called when TX power has to be decreased for a specific link id
    bool (*txpwr_dec)(uint8_t);
    /// Function called to execute an EPC request for a specific link id
    uint8_t (*txpwr_epc_req)(uint8_t, uint8_t);
    /// Function called to convert a TX power CS power field into the corresponding value in dBm
    uint8_t (*txpwr_dbm_get)(uint8_t, uint8_t);
    /// Function called to convert a power in dBm into a control structure tx power field
    uint8_t (*txpwr_cs_get)(int8_t);
    /// Function called to convert the RSSI read from the control structure into a real RSSI
    uint8_t (*rssi_convert)(uint8_t);
    /// Function used to handle RF interrupt
    void (*isr)(void);
    /// Function used to read a RF register
    uint16_t (*reg_rd)(uint16_t);
    /// Function used to write a RF register
    void (*reg_wr)(uint16_t, uint16_t);
    /// Function called to put the RF in deep sleep mode
    void (*sleep)(void);
    /// Function used to RF initialized
    void (*init)(void);
    /// Function used to RF disabled
    void (*pwrdown)(void);

    /// Index of ADVERTISE channel and Connnection default TX power
    uint8_t txpwr_adv;
    /// RSSI high threshold
    uint8_t rssi_high_thr;
    /// RSSI low threshold
    uint8_t rssi_low_thr;
    /// interferer threshold
    uint8_t rssi_interf_thr;
    /// RF skew
    uint16_t skew;
};



/*
 * MACROS
 ****************************************************************************************
 */



/*
 * GLOBAL VARIABLE DECLARATIONS
 ****************************************************************************************
 */
/// RW BLE environment
extern struct rwble_env_tag rwble_env;

/// API for RF driver
extern struct rw_rf_api rwble_rf;

/*
 * FUNCTION DECLARATIONS
 ****************************************************************************************
 */

#ifdef _USE_INLINE
#if DEEP_SLEEP
/**
 ****************************************************************************************
 * @brief Get the prevent sleep bit field
 *
 * @return The current value of the prevent sleep field
 ****************************************************************************************
 */
__INLINE uint16_t rwble_prevent_sleep_get(void)
{
    return (rwble_env.prevent_sleep);
}
#endif //DEEP_SLEEP

/**
 ****************************************************************************************
 * @brief RWBLE disable core routine
 *
 * This function disables the RWBLE core.
 ****************************************************************************************
 */
__INLINE void rwble_disable(void)
{
    // Disable the BLE core
    ble_rwble_en_setf(0);
}

/**
****************************************************************************************
* @brief RWBLE enable core routine
*
* This function enables the RWBLE core.
****************************************************************************************
*/
__INLINE void rwble_enable(void)
{
   // Enable the BLE core
    ble_rwble_en_setf(1);
}
#else //_USE_INLINE
#if DEEP_SLEEP
/**
 ****************************************************************************************
 * @brief Get the prevent sleep bit field
 *
 * @return The current value of the prevent sleep field
 ****************************************************************************************
 */
uint16_t rwble_prevent_sleep_get(void);
#endif //DEEP_SLEEP

/**
 ****************************************************************************************
 * @brief RWBLE disable core routine
 *
 * This function disables the RWBLE core.
 ****************************************************************************************
 */
void rwble_disable(void);

/**
****************************************************************************************
* @brief RWBLE enable core routine
*
* This function enables the RWBLE core.
****************************************************************************************
*/
void rwble_enable(void);

#endif //_USE_INLINE

/**
 ****************************************************************************************
 * @brief Initialize the BLE stack.
 *
 * @param[in] bd_addr  BD address
 * @param[in] sca      SCA(Sleep Clock Accuracy)
 ****************************************************************************************
 */
void rwble_init(const struct bd_addr *bd_addr, const uint8_t sca);

/**
****************************************************************************************
* @brief Reset the BLE stack.
*
* This function reset the BLE stack.
****************************************************************************************
*/
void rwble_rf_reset(void);

/**
 ****************************************************************************************
 * @brief Retrieve the slot clock.
 *
 * @return  Slot clock (x 625us)
 ****************************************************************************************
 */
uint32_t rwble_get_clock(void);

#if RW_BLE_SUPPORT

/**
 ****************************************************************************************
 * @brief Run the background processes of the BLE stack
 *
 * This function schedules the internal messages of the BLE protocol stack. It has to be
 * called each time a message is passed from the application to the protocol stack and
 * after an interrupt is generated from the BLE core.
 *
 ****************************************************************************************
 */
void rwble_schedule(void);

/**
 ****************************************************************************************
 * @brief Send an error message to Host.
 *
 * This function is used to send an error message to Host from platform.
 *
 * @param[in] error    Error detected by FW
 ****************************************************************************************
 */
void rwble_send_error_message(uint8_t error);
/**
 ****************************************************************************************
 * @brief Invoke the sleep function.
 *
 * @return  true: processor sleep allowed, false otherwise
 ****************************************************************************************
 */
bool rwble_sleep(void);
#endif //RW_BLE_SUPPORT

#if DEEP_SLEEP
/**
 ****************************************************************************************
 * @brief Set a bit in the prevent sleep bit field, in order to prevent the system from
 *        going to sleep
 *
 * @param[in] prv_slp_bit   Bit to be set in the prevent sleep bit field
 ****************************************************************************************
 */
void rwble_prevent_sleep_set(uint16_t prv_slp_bit);

/**
 ****************************************************************************************
 * @brief Clears a bit in the prevent sleep bit field, in order to allow the system
 *        going to sleep
 *
 * @param[in] prv_slp_bit   Bit to be cleared in the prevent sleep bit field
 ****************************************************************************************
 */
void rwble_prevent_sleep_clear(uint16_t prv_slp_bit);

#if RW_BLE_SUPPORT
/**
 ****************************************************************************************
 * @brief Set the wake-up delay
 *
 * @param[in] wakeup_delay   Wake-up delay in us
 ****************************************************************************************
 */
void rwble_wakeup_delay_set(uint16_t wakeup_delay);

/**
 ****************************************************************************************
 * @brief Check if sleep mode is enable
 *
 * @return    true if sleep is enable, false otherwise
 ****************************************************************************************
 */
bool rwble_sleep_enable(void);

/**
 ****************************************************************************************
 * @brief Check if external wake-up is enable
 *
 * @return    true if external wakeup is enable, false otherwise
 ****************************************************************************************
 */
bool rwble_ext_wakeup_enable(void);

/**
 ****************************************************************************************
 * @brief When a RWBLE_WAKE_UP_ONGOING is set to 1, it schedule the kernel timers.
 *
 ****************************************************************************************
 */
void rwble_wakeup_end(void);
#endif //RW_BLE_SUPPORT
#endif // DEEP_SLEEP

/// @} RWBLE

#endif // RWBLE_H_
