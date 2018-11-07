/**
 ****************************************************************************************
 *
 * @file         rwke_api.h
 *
 * @brief        This file contains definitions for the folowings:
 *               - RWKE APIs
 *               - some useful functions
 *               - task descriptor table
 *               - event handler table
 *
 * Copyright (C) RivieraWaves 2009-2012
 *
 * Copyright(C) 2012-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

#ifndef _RWKE_API_H_
#define _RWKE_API_H_

#include "arch.h"
#include "config.h"
#include "prf_sel.h"

/************************************************/
/*  SUPPRESS THE INCLUSION OF HEADER FILES      */
/************************************************/

/* @file ke/ke_*.h */
#define _KE_CONFIG_H_           /* ke_config.h */
#define _KE_TASK_H_             /* ke_task.h */
#define _KE_TIMER_H_            /* ke_timer.h */
#define _KE_ENV_H_              /* ke_env.h */
#define _KE_QUEUE_H_            /* ke_queue.h */
#define _KE_EVENT_H_            /* ke_event.h */
#define _KE_MEM_H_              /* ke_mem.h */
#define _KE_MSG_H_              /* ke_msg.h */

/* @file common/co_*.h */
#define _CO_VERSION_H_          /* co_version.h */
#define _CO_ERROR_H_            /* co_error.h */
#define _CO_ENDIAN_H_           /* co_endian.h */
#define _CO_MATH_H_             /* co_math.h */
#define _CO_UTILS_H_            /* co_utils.h */
#define _CO_LIST_H_             /* co_list.h */

#if 0
#define _CO_BT_H_               /* co_bt.h (is exposed to user) */
#define _CO_BUF_H_              /* co_buf.h (is internal use only) */
#endif /* 0 */


/************************************************/
/*  DEFINITIONS FOR THE COMMON UTILITIES        */
/************************************************/

/* @file co_version.h */
#define RWBT_SW_VERSION_MAJOR                   6       /* major version number */
#define RWBT_SW_VERSION_MINOR                   0x02    /* minor version number */
#define RWBT_SW_VERSION_BUILD                   0x07    /* build number */
#define RWBT_SW_VERSION_SUB_BUILD               0       /* sub-build number */

/* @file co_error.h */
#define CO_ERROR_NO_ERROR                       0x00
#define CO_ERROR_UNKNOWN_HCI_COMMAND            0x01
#define CO_ERROR_UNKNOWN_CONNECTION_ID          0x02
#define CO_ERROR_HARDWARE_FAILURE               0x03
#define CO_ERROR_PAGE_TIMEOUT                   0x04
#define CO_ERROR_AUTH_FAILURE                   0x05
#define CO_ERROR_PIN_MISSING                    0x06
#define CO_ERROR_MEMORY_CAPA_EXCEED             0x07
#define CO_ERROR_CON_TIMEOUT                    0x08
#define CO_ERROR_CON_LIMIT_EXCEED               0x09
#define CO_ERROR_COMMAND_DISALLOWED             0x0C
#define CO_ERROR_CONN_REJ_LIMITED_RESOURCES     0x0D
#define CO_ERROR_CONN_REJ_SECURITY_REASONS      0x0E
#define CO_ERROR_CONN_REJ_UNACCEPTABLE_BDADDR   0x0F
#define CO_ERROR_CONN_ACCEPT_TIMEOUT_EXCEED     0x10
#define CO_ERROR_UNSUPPORTED                    0x11
#define CO_ERROR_INVALID_HCI_PARAM              0x12
#define CO_ERROR_REMOTE_USER_TERM_CON           0x13
#define CO_ERROR_REMOTE_DEV_TERM_LOW_RESOURCES  0x14
#define CO_ERROR_REMOTE_DEV_POWER_OFF           0x15
#define CO_ERROR_CON_TERM_BY_LOCAL_HOST         0x16
#define CO_ERROR_REPEATED_ATTEMPTS              0x17
#define CO_ERROR_PAIRING_NOT_ALLOWED            0x18
#define CO_ERROR_UNSUPPORTED_REMOTE_FEATURE     0x1A
#define CO_ERROR_UNSPECIFIED_ERROR              0x1F
#define CO_ERROR_LMP_RSP_TIMEOUT                0x22
#define CO_ERROR_ENC_MODE_NOT_ACCEPT            0x25
#define CO_ERROR_LINK_KEY_CANT_CHANGE           0x26
#define CO_ERROR_INSTANT_PASSED                 0x28
#define CO_ERROR_PAIRING_WITH_UNIT_KEY_NOT_SUP  0x29
#define CO_ERROR_DIFF_TRANSACTION_COLLISION     0x2A
#define CO_ERROR_CHANNEL_CLASS_NOT_SUP          0x2E
#define CO_ERROR_INSUFFICIENT_SECURITY          0x2F
#define CO_ERROR_PARAM_OUT_OF_MAND_RANGE        0x30
#define CO_ERROR_SP_NOT_SUPPORTED_HOST          0x37
#define CO_ERROR_HOST_BUSY_PAIRING              0x38
#define CO_ERROR_CONTROLLER_BUSY                0x3A
#define CO_ERROR_UNACCEPTABLE_CONN_INT          0x3B
#define CO_ERROR_DIRECT_ADV_TO                  0x3C
#define CO_ERROR_TERMINATED_MIC_FAILURE         0x3D
#define CO_ERROR_CONN_FAILED_TO_BE_EST          0x3E

#define CO_ERROR_HW_UART_OUT_OF_SYNC            0x00
#define CO_ERROR_HW_MEM_ALLOC_FAIL              0x01
#define CO_ERROR_HW_LLCHMAP_ERR                 0x02
#define CO_ERROR_HW_CSTXPTR_ERR                 0x03
#define CO_ERROR_HW_CSFORMAT_ERR                0x04
#define CO_ERROR_HW_TXCRYPT_ERR                 0x81
#define CO_ERROR_HW_PKTCNTL_EMACC_ERR           0x82
#define CO_ERROR_HW_TXDESC_ERR                  0x84
#define CO_ERROR_HW_WHITELIST_ERR               0x88
#define CO_ERROR_HW_RXCRYPT_ERR                 0x90
#define CO_ERROR_HW_IFS_UNDERRUN                0xA0
#define CO_ERROR_HW_RADIO_EMACC_ERR             0xC0

#define CO_ERROR_HW_RF_WAKEUP_ERR               0xFE
#define CO_ERROR_HW_RF_INITIAL_ERR              0xFF

/* @file co_endian.h */
extern uint32_t co_bswap32(uint32_t val32);
CALLT extern uint16_t co_bswap16(uint16_t val16);
#if !CPU_LE     /* net byte order is big endian */
#define co_htonl(hostlong)      (hostlong)
#define co_htons(hostshort)     (hostshort)
#define co_ntohl(netlong)       (netlong)
#define co_ntohs(netshort)      (netshort)
#else
#define co_htonl(hostlong)      co_bswap32(hostlong)
#define co_htons(hostshort)     co_bswap16(hostshort)
#define co_ntohl(netlong)       co_bswap32(netlong)
#define co_ntohs(netshort)      co_bswap16(netshort)
#endif
#if CPU_LE     /* BT byte order is little endian */
#define co_htobl(hostlong)      (hostlong)
#define co_htobs(hostshort)     (hostshort)
#define co_btohl(btlong)        (btlong)
#define co_btohs(btshort)       (btshort)
#else
#define co_htobl(hostlong)      co_bswap32(hostlong)
#define co_htobs(hostshort)     co_bswap16(hostshort)
#define co_btohl(btlong)        co_bswap32(btlong)
#define co_btohs(btshort)       co_bswap16(btshort)
#endif

/* @file co_math.h */
extern int co_clz(uint32_t val);
extern void co_random_init(uint32_t seed);
extern uint8_t co_rand_byte(void);
extern uint16_t co_rand_hword(void);
extern uint32_t co_rand_word(void);
extern uint32_t co_min(uint32_t a, uint32_t b);
extern uint32_t co_max(uint32_t a, uint32_t b);
extern int co_abs(int val);
#define CO_BIT(pos)             (1UL << (pos))
#define CO_BIT8(pos)            ((uint8_t)(1U <<(pos)))
#define CO_BIT16(pos)           ((uint16_t)(1U <<(pos)))
#define CO_ALIGN4_HI(val)       (((val) + 0x3) & ~0x3)
#define CO_ALIGN4_LO(val)       ((val) & ~0x3)
#define CO_ALIGN2_HI(val)       (((val) + 0x1) & ~0x1)
#define CO_ALIGN2_LO(val)       ((val) & ~0x1)

/* @file co_utils.h */
extern uint32_t co_read32(void const *ptr32);
extern uint16_t co_read16(void const *ptr16);
extern void co_write32(void const *ptr32, uint32_t val32);
extern void co_write16(void const *ptr16, uint16_t val16);
extern void co_write8(void const *ptr8, uint8_t val8);
extern uint32_t co_read32p(void const *ptr32);
CALLT extern uint16_t co_read16p(void const *ptr16);
extern void co_write32p(void const *ptr32, uint32_t val32);
extern void co_write16p(void const *ptr16, uint16_t val16);
#define CO_MAX(a, b)            (((a) > (b)) ? (a) : (b))
#define CO_MIN(a, b)            (((a) < (b)) ? (a) : (b))
#define CO_GET_INDEX(__element_ptr, __array_ptr) \
                                ((__element_ptr) - (__array_ptr))

/* @file co_list.h */
struct co_list_hdr
{
    struct co_list_hdr *next;
};
#if 1                           /* DEBUG PURPOSE ONLY */
struct co_list
{
    struct co_list_hdr *first;
    struct co_list_hdr *last;
#if KE_PROFILING
    uint32_t cnt;
    uint32_t maxcnt;
    uint32_t mincnt;
#endif /* KE_PROFILING */
};
extern bool co_list_is_empty(const struct co_list *list);
extern struct co_list_hdr *co_list_pick(const struct co_list *list);
extern struct co_list_hdr *co_list_next(const struct co_list *list);
#endif /* 1 */


/************************************************************************/
/*                                                                      */
/*  DEFINITIONS FOR THE RWKE API                                        */
/*                                                                      */
/************************************************************************/

/************************************************/
/*  RWKE TYPES                                  */
/************************************************/

typedef uint32_t evt_field_t;
typedef void codeptr (*evt_ptr_t)(void);
typedef uint16_t ke_state_t;
typedef uint16_t ke_task_id_t;
typedef uint16_t ke_msg_id_t;
typedef int codeptr (*ke_msg_func_t)(const ke_msg_id_t msg, const void *param, const ke_task_id_t dst, const ke_task_id_t src);
typedef uint16_t ke_time_t;

/************************************************/
/*  RWKE APIS                                   */
/************************************************/

/* @file ke_mem.h */
#if 1                           /* DEBUG PURPOSE ONLY */
struct mblock_used
{
    size_t size;
};
struct mblock_free
{
    struct mblock_free *next;
    size_t size;
};
#endif /* 1 */
extern uint8_t *ke_mem_heap;
extern uint8_t *ke_mem_heap_end;
extern struct block_free *ke_mem_init(void);
CALLT extern void * DPRTN ke_malloc(size_t size);
extern void ke_free(void *mem_ptr);

/* @file ke_env.h */
#if 1                           /* DEBUG PURPOSE ONLY */
struct ke_env_tag
{
    volatile evt_field_t evt_field;
    struct co_list queue_send;
    struct co_list queue_saved;
    struct co_list queue_timer;
    struct mblock_free *mblock_first;
#if KE_PROFILING
    uint32_t max_heap_used;
#endif /* KE_PROFILING */
#if defined(CFG_FW_NAK)
    uint32_t heap_used;
    bool nak_req;
#endif /* defined(CFG_FW_NAK) */
};
extern struct ke_env_tag ke_env;
#endif /* 1 */

/* @file ke_event.h */
extern evt_field_t ke_evt_get(void);
extern void ke_evt_set(evt_field_t evt);
extern void ke_evt_clear(evt_field_t evt);
extern void ke_evt_init(void);
extern void ke_evt_flush(void);
extern void ke_evt_schedule(void);

/* @file ke_msg.h */
enum ke_msg_status_tag
{
    KE_MSG_CONSUMED = 0,
    KE_MSG_NO_FREE,
    KE_MSG_SAVED
};
struct ke_msg
{
    struct co_list_hdr hdr;
#if (BLE_SPLIT || BLE_FULLEMB)
    uint8_t hci_type;
    int8_t hci_off;
    uint16_t hci_len;
#endif /* (BLE_SPLIT || BLE_FULLEMB) */
    ke_msg_id_t id;
    ke_task_id_t dest_id;
    ke_task_id_t src_id;
    uint16_t param_len;
    uint32_t param[1];
};
extern struct ke_msg *ke_param2msg(const void *param_ptr);
extern void *ke_msg2param(const struct ke_msg *msg);
CALLT extern void * DPRTN ke_msg_alloc(ke_msg_id_t id, ke_task_id_t dest_id, ke_task_id_t src_id, uint16_t param_len);
CALLT extern void ke_msg_send(const void *param_ptr);
CALLT extern void ke_msg_send_basic(ke_msg_id_t id, ke_task_id_t dest_id, ke_task_id_t src_id);
extern void ke_msg_forward(const void *param_ptr, ke_task_id_t dest_id, ke_task_id_t src_id);
extern void ke_msg_free(const struct ke_msg *msg);
#define KE_BUILD_ID(type, index) \
                                ((ke_task_id_t)(((index) << 8) | (type)))
#define KE_TYPE_GET(ke_task_id) ((ke_task_id) & 0xFF)
#define KE_IDX_GET(ke_task_id)  (((ke_task_id) >> 8) & 0xFF)
#define KE_MSG_ALLOC(id, dest, src, param_ptr) \
                                (struct param_str *)ke_msg_alloc(id, dest, src, sizeof(struct param_str))
#define KE_MSG_FREE(param_ptr)  ke_msg_free(ke_param2msg(param_ptr))

/* @file ke_task.h */
struct ke_msg_handler
{
    ke_msg_id_t id;
    ke_msg_func_t func;
};
struct ke_state_handler
{
    const struct ke_msg_handler *msg_table;
    uint16_t msg_cnt;
};
#define KE_STATE_HANDLER(hdl)   { (hdl), (sizeof(hdl) / sizeof(struct ke_msg_handler)) }
#define KE_STATE_HANDLER_NONE   { NULL, 0 }
struct ke_task_desc
{
    const struct ke_state_handler *state_handler;
    const struct ke_state_handler *default_handler;
    ke_state_t *state;
    const uint16_t state_max;
    const uint16_t idx_max;
};
extern bool ke_task_local(ke_task_id_t id);
extern ke_state_t ke_state_get(ke_task_id_t id);
extern void ke_state_set(ke_task_id_t id, ke_state_t state_id);
extern void codeptr ke_task_schedule(void);
#if 0                           /* INTERNAL USE ONLY */
extern int ke_msg_discard(ke_msg_id_t msgid, const void *param, ke_task_id_t dest_id, ke_task_id_t src_id);
extern int ke_msg_save(ke_msg_id_t msgid, const void *param, ke_task_id_t dest_id, ke_task_id_t src_id);
#endif /* 0 */
#define KE_FIRST_MSG(task)      ((ke_msg_id_t)((task) << 10))
#define MSG_T(msg)              ((ke_task_id_t)(((msg) >> 10) & 0x3F))
#define MSG_I(msg)              ((msg) & 0x3FF)

/* @file ke_queue.h (nothing exposed to user)*/

/* @file ke_timer.h */
struct ke_timer
{
    struct ke_timer *next;
    ke_msg_id_t id;
    ke_task_id_t task;
    ke_time_t time;
};
extern ke_time_t ke_time(void);
#define ke_time_get             ke_time     /* alias of ke_time */
extern bool ke_time_cmp(ke_time_t newer, ke_time_t older);
extern bool ke_time_past(uint32_t time);
extern void ke_timer_set(ke_msg_id_t timerid, ke_task_id_t task, ke_time_t delay);
extern void ke_timer_clear(ke_msg_id_t timerid, ke_task_id_t task);
extern void codeptr ke_timer_schedule(void);
#if 0                           /* INTERNAL USE ONLY */
extern void ke_timer_irq(void);
#endif /* 0 */


/************************************************************************/
/*                                                                      */
/*  DEFINITIONS FOR THE TASK DESCRIPTOR TABLE                           */
/*                                                                      */
/************************************************************************/

/* @file ke_task.h */
enum
{
    TASK_NONE = (uint8_t)(-1),

    TASK_LLM = 0,               //< Controller Tasks
    TASK_LLC,
    TASK_LLD,
    TASK_DBG,

    TASK_L2CM,                  //< Host tasks
    TASK_L2CC,
    TASK_SMPM,
    TASK_SMPC,
    TASK_ATTM,
    TASK_ATTC,
    TASK_ATTS,
    TASK_SVC,
    TASK_GATT,
    TASK_GAP,

    // User Task (Embeded Portion)
    TASK_USR_0,                 //< for user task
    TASK_USR_1,                 //< for user task
    TASK_RBLE,                  //< rBLE

    TASK_APP = TASK_RBLE,       //< API task
#ifndef CONFIG_EMBEDDED
    TASK_MDM_APPL,
#else
    TASK_CON_APPL,
#endif

#if PRF_SEL_PXPM
    TASK_PROXM,
#endif
#if PRF_SEL_PXPR
    TASK_PROXR,
#endif
#if PRF_SEL_FMPL
    TASK_FINDL,
#endif
#if PRF_SEL_FMPT
    TASK_FINDT,
#endif
#if PRF_SEL_HTPC
    TASK_HTPC,
#endif
#if PRF_SEL_HTPT
    TASK_HTPT,
#endif
#if PRF_SEL_BLPC
    TASK_BLPC,
#endif
#if PRF_SEL_BLPS
    TASK_BLPS,
#endif
#if PRF_SEL_HGHD
    TASK_HGHD,
#endif
#if PRF_SEL_HGBH
    TASK_HGBH,
#endif
#if PRF_SEL_HGRH
    TASK_HGRH,
#endif
#if PRF_SEL_SPPC
    TASK_SCANC,
#endif
#if PRF_SEL_SPPS
    TASK_SCANS,
#endif
#if PRF_SEL_HRPC
    TASK_HRPC,
#endif
#if PRF_SEL_HRPS
    TASK_HRPS,
#endif
#if PRF_SEL_CSCC
    TASK_CSCPC,
#endif
#if PRF_SEL_CSCS
    TASK_CSCPS,
#endif
#if PRF_SEL_GLPC
    TASK_GLPC,
#endif
#if PRF_SEL_GLPS
    TASK_GLPS,
#endif
#if PRF_SEL_CPPC
    TASK_CPPC,
#endif
#if PRF_SEL_CPPS
    TASK_CPPS,
#endif
#if PRF_SEL_TIPC
    TASK_TIPC,
#endif
#if PRF_SEL_TIPS
    TASK_TIPS,
#endif
#if PRF_SEL_ANPC
    TASK_ANPC,
#endif
#if PRF_SEL_ANPS
    TASK_ANPS,
#endif
#if PRF_SEL_LNPC
    TASK_LNPC,
#endif
#if PRF_SEL_LNPS
    TASK_LNPS,
#endif
#if PRF_SEL_PASC
    TASK_PASPC,
#endif
#if PRF_SEL_PASS
    TASK_PASPS,
#endif
#if PRF_SEL_RSCC
    TASK_RSCPC,
#endif
#if PRF_SEL_RSCS
    TASK_RSCPS,
#endif
#if BLE_ACCEL
    TASK_ACCEL,
#endif

    TASK_MAX,                   //< the last task running on the BLE MCU
    TASK_LAST_EMB = (TASK_MAX - 1),
};

/* @file llm_task.h */
enum
{
    LLM_IDLE,
    LLM_ADVERTISING,
    LLM_SCANNING,
    LLM_INITIATING,
    LLM_STOPPING,
    LLM_TEST,
    LLM_STATE_MAX
};
#define LLM_IDX_MAX             1
extern const struct ke_state_handler llm_state_handler[LLM_STATE_MAX];
extern const struct ke_state_handler llm_default_handler;
extern ke_state_t llm_state[LLM_IDX_MAX];

/* @file llc_task.h */
enum
{
    LLC_FREE,
    LLC_CONNECTED,
    LLC_FEAT_WAIT_RSP,
    LLC_VERS_WAIT_IND,
    LLC_CON_UPD_WAIT_INSTANT,
    LLC_MAP_UPD_WAIT_INSTANT,
    LLC_TERM_WAIT_ACK,
    LLC_ENC_PAUSING_TRAFFIC,
    LLC_ENC_TRAFFIC_PAUSED,
    LLC_ENC_WAIT_RSP,
    LLC_ENC_WAIT_START_REQ,
    LLC_ENC_WAIT_START_RSP,
    LLC_ENC_WAIT_SK,
    LLC_ENC_WAIT_LTK,
    LLC_ENC_WAIT_PAUSE_RSP,
    LLC_ENC_PAUSED,
    LLC_ENC_WAIT_REQ,
    LLC_PAUSED,
    LLC_WAIT_SK,
    LLC_DISC,
    LLC_STOPPING,
    LLCP_WAIT_ACK,
    LLC_STATE_MAX
};
#define LLC_IDX_MAX		BLE_CONN_MAX
extern const struct ke_state_handler llc_state_handler[LLC_STATE_MAX];
extern const struct ke_state_handler llc_default_handler;
extern ke_state_t llc_state[LLC_IDX_MAX];

/* @file dbg_task.h */
enum
{
    DBG_IDLE,
    DBG_STATE_MAX
};
#define DBG_IDX_MAX             1
extern const struct ke_state_handler dbg_state_handler[DBG_STATE_MAX];
extern const struct ke_state_handler dbg_default_handler;
extern ke_state_t dbg_state[DBG_IDX_MAX];

/* @file l2cm_task.h */
enum
{
    L2CM_IDLE,
    L2CM_W4RESP,
    L2CM_BUSY,
    L2CM_STATE_MAX
};
#define L2CM_IDX_MAX            1
extern const struct ke_state_handler l2cm_state_handler[L2CM_STATE_MAX];
extern const struct ke_state_handler l2cm_default_handler;
extern ke_state_t l2cm_state[L2CM_IDX_MAX];

/* @file l2cc_task.h */
enum
{
    L2CC_CONNECTED,
    L2CC_STATE_MAX
};
#define L2CC_IDX_MAX		BLE_CONN_MAX
extern const struct ke_state_handler l2cc_state_handler[L2CC_STATE_MAX];
extern const struct ke_state_handler l2cc_default_handler;
extern ke_state_t l2cc_state[L2CC_IDX_MAX];

/* @file gap_task.h */
enum
{
    GAP_IDLE,
    GAP_DEV_SETUP,
    GAP_RAND_ADDR_SETUP,
    GAP_CONN_SETUP,
    GAP_ADV_SETUP,
    GAP_ADV,
    GAP_SCAN_SETUP,
    GAP_SCAN,
    GAP_SCONNECTED,
    GAP_MCONNECTED,
    GAP_STATE_MAX
};
#define GAP_IDX_MAX             1
extern const struct ke_state_handler gap_state_handler[GAP_STATE_MAX];
extern const struct ke_state_handler gap_default_handler;
extern ke_state_t gap_state[GAP_IDX_MAX];

/* @file gatt_task.h */
enum
{
    GATT_IDLE,
    GATT_BUSY,
    GATT_STATE_MAX
};
#define GATT_IDX_MAX            1
extern const struct ke_state_handler gatt_state_handler[GATT_STATE_MAX];
extern const struct ke_state_handler gatt_default_handler;
extern ke_state_t gatt_state[GATT_IDX_MAX];

/* @file attc_task.h */
enum
{
    ATTC_CONNECTED,
    ATTC_DISC_SVC,
    ATTC_RELIABLE_WR,
    ATTC_BUSY,
    ATTC_FREE,
    ATTC_STATE_MAX
};
#define ATTC_IDX_MAX            BLE_CONN_MAX
extern const struct ke_state_handler attc_state_handler[ATTC_STATE_MAX];
extern const struct ke_state_handler attc_default_handler;
extern ke_state_t attc_state[ATTC_IDX_MAX];

/* @file attm_task.h */
enum
{
    ATTM_IDLE,
    ATTM_STATE_MAX
};
#define ATTM_IDX_MAX            1
extern const struct ke_state_handler attm_state_handler[ATTM_STATE_MAX];
extern const struct ke_state_handler attm_default_handler;
extern ke_state_t attm_state[ATTM_IDX_MAX];

/* @file atts_task.h */
enum
{
    ATTS_IDLE,
    ATTS_STATE_MAX
};
#define ATTS_IDX_MAX            1
extern const struct ke_state_handler atts_state_handler[ATTS_STATE_MAX];
extern const struct ke_state_handler atts_default_handler;
extern ke_state_t atts_state[ATTS_IDX_MAX];

/* @file svc_task.h */
enum
{
    SVC_IDLE,
    SVC_STATE_MAX
};
#define SVC_IDX_MAX             1
extern const struct ke_state_handler svc_state_handler[SVC_STATE_MAX];
extern const struct ke_state_handler svc_default_handler;
extern ke_state_t svc_state[SVC_IDX_MAX];

/* @file smpm_task.h */
enum
{
    SMPM_IDLE,
    SMPM_BUSY,
    SMPM_STATE_MAX
};
#define SMPM_IDX_MAX            1
extern const struct ke_state_handler smpm_state_handler[SMPM_STATE_MAX];
extern const struct ke_state_handler smpm_default_handler;
extern ke_state_t smpm_state[SMPM_IDX_MAX];

/* @file smpc_task.h */
enum
{
    SMPC_FREE,
    SMPC_IDLE,
    SMPC_PAIRING,
    SMPC_ENCRYPTING_STK,
    SMPC_TKDP,
    SMPC_ENCRYPTING_LTK,
    SMPC_DISC,
    SMPC_STATE_MAX
};
#define SMPC_IDX_MAX            BLE_CONN_MAX
extern const struct ke_state_handler smpc_state_handler[SMPC_STATE_MAX];
extern const struct ke_state_handler smpc_default_handler;
extern ke_state_t smpc_state[SMPC_IDX_MAX];

/* @file prf_types.h */
enum {
	PRF_STATE_IDLE,
	PRF_STATE_CONNECTED,
	PRF_STATE_DISCOVERING
};

/* @file proxm_task.h */
#define PROXM_STATE_MAX	PRF_STATE_DISCOVERING + 1

#define PROXM_IDX_MAX           1
extern const struct ke_state_handler proxm_state_handler[PROXM_STATE_MAX];
extern const struct ke_state_handler proxm_default_handler;
extern ke_state_t proxm_state[PROXM_IDX_MAX];

/* @file proxr_task.h */
#define PROXR_STATE_MAX	PRF_STATE_CONNECTED + 1

#define PROXR_IDX_MAX           1
extern const struct ke_state_handler proxr_state_handler[PROXR_STATE_MAX];
extern const struct ke_state_handler proxr_default_handler;
extern ke_state_t proxr_state[PROXR_IDX_MAX];

/* @file findl_task.h */
#define FINDL_STATE_MAX		PRF_STATE_CONNECTED + 1

#define FINDL_IDX_MAX           1
extern const struct ke_state_handler findl_state_handler[FINDL_STATE_MAX];
extern const struct ke_state_handler findl_default_handler;
extern ke_state_t findl_state[FINDL_IDX_MAX];

/* @file findt_task.h */
#define FINDT_STATE_MAX		PRF_STATE_CONNECTED + 1

#define FINDT_IDX_MAX		1
extern const struct ke_state_handler findt_state_handler[FINDT_STATE_MAX];
extern const struct ke_state_handler findt_default_handler;
extern ke_state_t findt_state[FINDT_IDX_MAX];

/* @file htpc_task.h */
#define HTPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define HTPC_IDX_MAX		1
extern const struct ke_state_handler htpc_state_handler[HTPC_STATE_MAX];
extern const struct ke_state_handler htpc_default_handler;
extern ke_state_t htpc_state[HTPC_IDX_MAX];

/* @file htpt_task.h */
#define HTPT_STATE_MAX		PRF_STATE_CONNECTED + 1

#define HTPT_IDX_MAX            1
extern const struct ke_state_handler htpt_state_handler[HTPT_STATE_MAX];
extern const struct ke_state_handler htpt_default_handler;
extern ke_state_t htpt_state[HTPT_IDX_MAX];

/* @file blpc_task.h */
#define BLPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define BLPC_IDX_MAX            1
extern const struct ke_state_handler blpc_state_handler[BLPC_STATE_MAX];
extern const struct ke_state_handler blpc_default_handler;
extern ke_state_t blpc_state[BLPC_IDX_MAX];

/* @file blps_task.h */
#define BLPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define BLPS_IDX_MAX            1
extern const struct ke_state_handler blps_state_handler[BLPS_STATE_MAX];
extern const struct ke_state_handler blps_default_handler;
extern ke_state_t blps_state[BLPS_IDX_MAX];

/* @file hghd_task.h */
#define HGHD_STATE_MAX		PRF_STATE_CONNECTED + 1

#define HGHD_IDX_MAX            1
extern const struct ke_state_handler hghd_state_handler[HGHD_STATE_MAX];
extern const struct ke_state_handler hghd_default_handler;
extern ke_state_t hghd_state[HGHD_IDX_MAX];

/* @file hgbh_task.h */
#define HGBH_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define HGBH_IDX_MAX            1
extern const struct ke_state_handler hgbh_state_handler[HGBH_STATE_MAX];
extern const struct ke_state_handler hgbh_default_handler;
extern ke_state_t hgbh_state[HGBH_IDX_MAX];

/* @file hgrh_task.h */
#define HGRH_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define HGRH_IDX_MAX            1
extern const struct ke_state_handler hgrh_state_handler[HGRH_STATE_MAX];
extern const struct ke_state_handler hgrh_default_handler;
extern ke_state_t hgrh_state[HGRH_IDX_MAX];

/* @file scans_task.h */
#define SCANS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define SCANS_IDX_MAX           1
extern const struct ke_state_handler scans_state_handler[SCANS_STATE_MAX];
extern const struct ke_state_handler scans_default_handler;
extern ke_state_t scans_state[SCANS_IDX_MAX];

/* @file scanc_task.h */
#define SCANC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define SCANC_IDX_MAX           1
extern const struct ke_state_handler scanc_state_handler[SCANC_STATE_MAX];
extern const struct ke_state_handler scanc_default_handler;
extern ke_state_t scanc_state[SCANS_IDX_MAX];

/* @file hrpc_task.h */
#define HRPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define HRPC_IDX_MAX            1
extern const struct ke_state_handler hrpc_state_handler[HRPC_STATE_MAX];
extern const struct ke_state_handler hrpc_default_handler;
extern ke_state_t hrpc_state[HRPC_IDX_MAX];

/* @file hrps_task.h */
#define HRPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define HRPS_IDX_MAX            1
extern const struct ke_state_handler hrps_state_handler[HRPS_STATE_MAX];
extern const struct ke_state_handler hrps_default_handler;
extern ke_state_t hrps_state[HRPS_IDX_MAX];

/* @file cscpc_task.h */
#define CSCPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define CSCPC_IDX_MAX            1
extern const struct ke_state_handler cscpc_state_handler[CSCPC_STATE_MAX];
extern const struct ke_state_handler cscpc_default_handler;
extern ke_state_t cscpc_state[CSCPC_IDX_MAX];

/* @file cscps_task.h */
#define CSCPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define CSCPS_IDX_MAX            1
extern const struct ke_state_handler cscps_state_handler[CSCPS_STATE_MAX];
extern const struct ke_state_handler cscps_default_handler;
extern ke_state_t cscps_state[CSCPS_IDX_MAX];

/* @file glpc_task.h */
#define GLPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define GLPC_IDX_MAX            1
extern const struct ke_state_handler glpc_state_handler[GLPC_STATE_MAX];
extern const struct ke_state_handler glpc_default_handler;
extern ke_state_t glpc_state[GLPC_IDX_MAX];

/* @file glps_task.h */
#define GLPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define GLPS_IDX_MAX            1
extern const struct ke_state_handler glps_state_handler[GLPS_STATE_MAX];
extern const struct ke_state_handler glps_default_handler;
extern ke_state_t glps_state[GLPS_IDX_MAX];

/* @file cppc_task.h */
#define CPPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define CPPC_IDX_MAX           1
extern const struct ke_state_handler cppc_state_handler[CPPC_STATE_MAX];
extern const struct ke_state_handler cppc_default_handler;
extern ke_state_t cppc_state[CPPC_IDX_MAX];

/* @file cpps_task.h */
#define CPPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define CPPS_IDX_MAX           1
extern const struct ke_state_handler cpps_state_handler[CPPS_STATE_MAX];
extern const struct ke_state_handler cpps_default_handler;
extern ke_state_t cpps_state[CPPS_IDX_MAX];

/* @file tipc_task.h */
#define TIPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define TIPC_IDX_MAX           1
extern const struct ke_state_handler tipc_state_handler[TIPC_STATE_MAX];
extern const struct ke_state_handler tipc_default_handler;
extern ke_state_t tipc_state[TIPC_IDX_MAX];

/* @file tips_task.h */
#define TIPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define TIPS_IDX_MAX           1
extern const struct ke_state_handler tips_state_handler[TIPS_STATE_MAX];
extern const struct ke_state_handler tips_default_handler;
extern ke_state_t tips_state[TIPS_IDX_MAX];

/* @file anpc_task.h */
#define	ANPC_STATE_MAX	PRF_STATE_DISCOVERING + 1

#define ANPC_IDX_MAX           1
extern const struct ke_state_handler anpc_state_handler[ANPC_STATE_MAX];
extern const struct ke_state_handler anpc_default_handler;
extern ke_state_t anpc_state[ANPC_IDX_MAX];

/* @file anps_task.h */
#define ANPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define ANPS_IDX_MAX           1
extern const struct ke_state_handler anps_state_handler[ANPS_STATE_MAX];
extern const struct ke_state_handler anps_default_handler;
extern ke_state_t anps_state[ANPS_IDX_MAX];

/* @file lnpc_task.h */
#define LNPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define LNPC_IDX_MAX           1
extern const struct ke_state_handler lnpc_state_handler[LNPC_STATE_MAX];
extern const struct ke_state_handler lnpc_default_handler;
extern ke_state_t lnpc_state[LNPC_IDX_MAX];

/* @file lnps_task.h */
#define LNPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define LNPS_IDX_MAX           1
extern const struct ke_state_handler lnps_state_handler[LNPS_STATE_MAX];
extern const struct ke_state_handler lnps_default_handler;
extern ke_state_t lnps_state[LNPS_IDX_MAX];

/* @file paspc_task.h */
#define PASPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define PASPC_IDX_MAX           1
extern const struct ke_state_handler paspc_state_handler[PASPC_STATE_MAX];
extern const struct ke_state_handler paspc_default_handler;
extern ke_state_t paspc_state[PASPC_IDX_MAX];

/* @file pasps_task.h */
#define PASPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define PASPS_IDX_MAX           1
extern const struct ke_state_handler pasps_state_handler[PASPS_STATE_MAX];
extern const struct ke_state_handler pasps_default_handler;
extern ke_state_t pasps_state[PASPS_IDX_MAX];

/* @file rscpc_task.h */
#define RSCPC_STATE_MAX		PRF_STATE_DISCOVERING + 1

#define RSCPC_IDX_MAX           1
extern const struct ke_state_handler rscpc_state_handler[RSCPC_STATE_MAX];
extern const struct ke_state_handler rscpc_default_handler;
extern ke_state_t rscpc_state[RSCPC_IDX_MAX];

/* @file rscps_task.h */
#define RSCPS_STATE_MAX		PRF_STATE_CONNECTED + 1

#define RSCPS_IDX_MAX           1
extern const struct ke_state_handler rscps_state_handler[RSCPS_STATE_MAX];
extern const struct ke_state_handler rscps_default_handler;
extern ke_state_t rscps_state[RSCPS_IDX_MAX];

/* @file accel_task.h */
enum
{
    ACCEL_IDLE,
    ACCEL_ACTIVE,
    ACCEL_STATE_MAX
};

#define ACCEL_IDX_MAX           1
extern const struct ke_state_handler accel_state_handler[ACCEL_STATE_MAX];
extern const struct ke_state_handler accel_default_handler;
extern ke_state_t accel_state[ACCEL_IDX_MAX];

#if 0                           /* NOT MERGED YET */
/* @file console.h */
#define RBLE_APP_STATE_MAX      1
#define RBLE_APP_IDX_MAX        1
extern const struct ke_state_handler rble_app_state_handler[RBLE_APP_STATE_MAX];
extern const struct ke_state_handler rble_app_default_handler;
extern ke_state_t rble_app_state[RBLE_SPP_IDX_MAX];
#endif /* 0 */

#if 0                           /* NOT MERGED YET */
/* @file rble_rwke.h */
#define RBLE_STATE_MAX          1
#define RBLE_IDX_MAX            1
extern const struct ke_state_handler rble_state_handler[RBLE_STATE_MAX];
extern const struct ke_state_handler rble_default_handler;
extern ke_state_t rble_state[RBLE_IDX_MAX];
#define RBLE_TIMER_STATE_MAX    1
#define RBLE_TIMER_IDX_MAX      1
extern const struct ke_state_handler rble_timer_state_handler[RBLE_TIMER_STATE_MAX];
extern const struct ke_state_handler rble_timer_default_handler;
extern ke_state_t rble_timer_state[RBLE_TIMER_IDX_MAX];
#endif /* 0 */


/************************************************************************/
/*                                                                      */
/*  DEFINITIONS FOR THE EVENT HANDLER TABLE                             */
/*                                                                      */
/************************************************************************/

enum
{
#if BLE_EMB_PRESENT
    KE_EVT_EVENT_START,         //< for lld_evt_schedule
    KE_EVT_RX,                  //< for lld_evt_rx
    KE_EVT_EVENT_END,           //< for lld_evt_end
#endif
#if (BLE_SPLIT || BLE_FULLEMB)
    KE_EVT_HCI_TX_DONE,         //< used by rBLE (RSCIP_Tx_Done)
#endif
    KE_EVT_USR_0,               //< place holder for user event (high priority)
    KE_EVT_USR_1,               //< place holder for user event
    KE_EVT_KE_TIMER,            //< for ke_timer_schedule
    KE_EVT_KE_MESSAGE,          //< for ke_task_schedule
#if BLE_EMB_PRESENT
    KE_EVT_CRYPT,               //< for llm_encryption_done
#endif
    KE_EVT_HCI_RX_DONE,         //< used by rBLE (RSCIP_Rx_Done)
    KE_EVT_USR_2,               //< place holder for user event
    KE_EVT_USR_3,               //< place holder for user event (low priority)
    KE_EVT_MAX
};

/* @file lld_env.h */
extern void codeptr lld_evt_schedule(void);
extern void codeptr lld_evt_rx(void);
extern void codeptr lld_evt_end(void);
extern uint32_t lld_evt_time_get(void);

/* @file llm.h */
extern void codeptr llm_encryption_done(void);

#if 0                           /* NOT SUPPORTED */
/* @file hci.h */
extern void hci_tx_done(void);
#endif /* 0 */

#if 0                           /* NOT MERGED YET */
/* @file rscip_api.h */
extern void codeptr RSCIP_Tx_Done(void);
extern void codeptr RSCIP_Rx_Done(void);
#endif /* 0 */

/************************************************/
/*  END OF HEADER FILE                          */
/************************************************/
#endif /* _RWKE_API_H_ */

