/**
 ****************************************************************************************
 *
 * @file		cpu_com.h
 *
 * @brief Direct Test Mode 2Wire UART Driver.
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: v1.01.00
 *
 ****************************************************************************************
 */

#ifndef	__MAIN_USR_INC__
#define	__MAIN_USR_INC__

#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */
#include	"ke_task.h"
#endif

// ====================================================
// ==================プラットフォーム==================
// ====================================================

/* Tas Infomation */
#define CPU_COM_STATE_MAX		2	// Max State Num
#define CPU_COM_IDX_MAX			1	// Max ID Num

#define USER_MAIN_ID (TASK_USR_0)

//#define USER_MAIN_CYC_ACT			1	/* Task API ID */

#define USER_MAIN_CYC_ACT				KE_FIRST_MSG(USER_MAIN_ID) + 1		/* Task API ID */
#define USER_MAIN_CALC_SEKIGAI			KE_FIRST_MSG(USER_MAIN_ID) + 2		/* Timer API ID */
#define USER_MAIN_CALC_SEKISHOKU		KE_FIRST_MSG(USER_MAIN_ID) + 3		/* Timer API ID */
#define USER_MAIN_CALC_KOKYU			KE_FIRST_MSG(USER_MAIN_ID) + 4		/* Timer API ID */
#define USER_MAIN_CALC_IBIKI			KE_FIRST_MSG(USER_MAIN_ID) + 5		/* Timer API ID */
#define USER_MAIN_CALC_ACL				KE_FIRST_MSG(USER_MAIN_ID) + 6		/* Timer API ID */
#define USER_MAIN_ACT2					KE_FIRST_MSG(USER_MAIN_ID) + 7		/* Timer API ID */







//#define USER_MAIN_CYC_ACT			1								/* Task API ID */
//#define CPU_COM_TIMER_API		2								/* Timer API ID */


#if 0
#define RBLE_APP_CON_SW_EVT			KE_FIRST_MSG(TASK_CON_APPL) + 3	/* SW API ID */
#define RBLE_APP_CON_SEND_EVT		KE_FIRST_MSG(TASK_CON_APPL) + 4	/* SEND API ID */
#endif


#define  KE_EVT_USR_2_BIT			CO_BIT(31 - KE_EVT_USR_2) 
#define  KE_EVT_USR_3_BIT			CO_BIT(31 - KE_EVT_USR_3) 

#if 0
extern volatile RBLE_RUNNING_MODE	rBLE_Run_Mode_Flg;
#endif
/* Status Handler */
extern const struct ke_state_handler cpu_com_state_handler[ CPU_COM_STATE_MAX ];
/* Default Handler */
extern const struct ke_state_handler cpu_com_default_handler;
/* Status */
extern ke_state_t cpu_com_state[ CPU_COM_IDX_MAX ];



// ============================================================
// ==================ユーザーアプリケーション==================
// ============================================================

// システムモード
typedef enum{
	SYSTEM_MODE_IDLE = 0,					// アイドル
	SYSTEM_MODE_SENSING,					// センシング
	SYSTEM_MODE_MOVE,						// 移行
	SYSTEM_MODE_GET,						// データ取得
	SYSTEM_MODE_PRG_HD,						// プログラム更新
	SYSTEM_MODE_MAX
}SYSTEM_MODE;


//#define			MAIN_STATUS_REQ_TIME			50		// ステータス要求時間[500ms]
#define			MAIN_STATUS_REQ_TIME			100		// ステータス要求時間[1s]

#define			SENSING_END_JUDGE_TIME				100		// ステータス要求時間[1s]



#define			SYSTEM_MODE_HD_CHG_NON				0		// なし
#define			SYSTEM_MODE_HD_CHG_SENSING			1		// あり(センシング)
#define			SYSTEM_MODE_HD_CHG_IDLE				2		// あり(アイドル)




#define		MEAS_SEKIGAI_CNT_MAX		140
#define		MEAS_SEKISHOKU_CNT_MAX		140
#define		MEAS_KOKYU_CNT_MAX			200
#define		MEAS_IBIKI_CNT_MAX			200
#define		MEAS_ACL_CNT_MAX			20


// 測定情報
typedef struct{
	union{
		UB	byte[CPU_COM_SND_DATA_SIZE_SENSOR_DATA];
		struct{
			H	sekigaival;		// 差動入力の為に符号あり
			H	sekishoku_val;	// 差動入力の為に符号あり
			UH	kokyu_val;		
			UH	ibiki_val;		
			B acl_x;
			B acl_y;
			B acl_z;
			B dummy;			// 空き[パディング調整]
		}dat;
	}info;
}MEAS;

// 演算情報
typedef struct{
	union{
		UB	byte[EEP_CACL_DATA_SIZE];
		struct{
			UH	ibiki_val;
			UB	state;			/* 状態 */
			UB	myaku_val;
			UB	spo2_val;
			UB	kubi;
			UB	dummy_1;			/* 境界値調整用 */
			UB	dummy_2;			/* 境界値調整用 */
		}dat;
	}info;
}CALC;

// 日時情報
typedef struct{
	UH year;
	UB month;
	UB day;
	UB hour;
	UB min;
	UB sec;
}DATE;


typedef struct{
	
	SYSTEM_MODE system_mode;		/* システムモード */
	SYSTEM_MODE next_system_mode;	/* 次のシステムモード */
	
	DATE date;
	
	ke_time_t last_time;			//前回時間
	ke_time_t last_sensing_data_rcv;		//前回センシングデータ受信
	
	UB denchi_sts;			// 電池状態
	
	// 演算関連
	CALC calc;				// 演算後データ
	UH calc_cnt;			// 演算カウント
	UH max_mukokyu_sec;		// 最大無呼吸[秒]
	
	UB frame_num_read;		// フレーム(枠)の読み出し番号
	UB frame_num_write;		// フレーム(枠)の書き込み番号
	
	// 機器データ(演算前)
	H	sekigai_val[MEAS_SEKIGAI_CNT_MAX];		// 差動入力の為に符号あり
	H	sekishoku_val[MEAS_SEKISHOKU_CNT_MAX];	// 差動入力の為に符号あり
	UH	kokyu_val[MEAS_KOKYU_CNT_MAX];		
	UH	ibiki_val[MEAS_IBIKI_CNT_MAX];		
	B	acl_x[MEAS_ACL_CNT_MAX];
	B	acl_y[MEAS_ACL_CNT_MAX];
	B	acl_z[MEAS_ACL_CNT_MAX];
	// 機器データ(演算前)回数
	UB	sekigai_cnt;
	UB	sekishoku_cnt;	// 差動入力の為に符号あり
	UB	kokyu_cnt;		
	UB	ibiki_cnt;		
	UB	acl_cnt;
	
	
	
	
	// 以降ワーク領域
	UW sec30_cnt;			//30秒カウント
	UW sec10_cnt;			//10秒カウント
	UW sec7_cnt;			//7秒カウント
	
	UB get_mode_status;
	UB get_mode_calc_cnt;
	UH get_wait_cnt;		// RD8001暫定：スマホ側が早すぎるとパケットがくっつく	
	
	
	UW err_cnt;			//異常回数(デバッグ用途)
}T_UNIT;






/*##################################################################*/
/*							CPU間通信部								*/
/*##################################################################*/
typedef struct _DS_CPU_COM_INPUT{
	UB rcv_cmd;											/* 受信コマンド */
	UB rcv_data[CPU_COM_DATA_SIZE_MAX];					/* 受信データ */
	UB cpu_com_send_status;								/* CPU間通信送信ステータス */
														/* 	CPU_COM_SND_STATUS_IDLE			アイドル状態 ※送信可能状態	*/
														/* 	CPU_COM_SND_STATUS_SEND			送信中						*/
														/* 	CPU_COM_SND_STATUS_COMPLETE		送信完了					*/
														/* 	CPU_COM_SND_STATUS_SEND_NG		リトライNG					*/
}DS_CPU_COM_INPUT;

typedef struct _DS_CPU_COM_ORDER{
	CPU_COM_CMD_ID snd_cmd_id;							/* 送信コマンドID */
	UB snd_data[CPU_COM_DATA_SIZE_MAX];					/* 送信データ */
	UH data_size;										/* 送信データ長 */
}DS_CPU_COM_ORDER;

typedef struct _DS_CPU_COM{
	DS_CPU_COM_INPUT input;
	DS_CPU_COM_ORDER order;
}DS_CPU_COM;

typedef struct _CPU_COM_RCV_CMD_TBL{
	UB cmd;							/* 受信コマンド */
//	void (*func)(UB *p_data);		/* 受信処理 */
	void (*func)(void);				/* 受信処理 */
	UB res;							/* 応答有(ON)無(OFF) */
}CPU_COM_RCV_CMD_TBL;

/*##################################################################*/
/*							VUART(BLE)通信部						*/
/*##################################################################*/
#define VUART_DATA_SIZE_MAX				20

typedef struct _DS_VUART_INPUT{
	UB rcv_data[VUART_DATA_SIZE_MAX];					/* 受信データ */
	UB rcv_len;											/* 受信長 */
	UB send_status;										/* 送信状態(ON:送信中、OFF:送信なし) */
}DS_VUART_INPUT;

typedef struct _DS_VUART{
	DS_VUART_INPUT input;
}DS_VUART;


/* DS構造体 */
typedef struct{
	DS_CPU_COM 		cpu_com;		/* CPU間通信部 */
	DS_VUART		vuart;			/* 仮想UART(BLE) */
}DS;


/******************/
/*  外部参照宣言  */
/******************/
extern void codeptr app_evt_usr_2(void);
extern void codeptr app_evt_usr_3(void);
extern void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
extern void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );
extern bool user_main_sleep(void);
extern void ds_set_vuart_data( UB *p_data, UB len );
extern void ds_set_vuart_send_status( UB status );
extern void user_main_init( void );

#endif // __MAIN_USR_INC__
