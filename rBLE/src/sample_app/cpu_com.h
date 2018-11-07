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

#ifndef	__CPU_COM_INC__
#define	__CPU_COM_INC__

#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */
#include	"ke_task.h"
#endif


/**************************** Structure Declaration ***********************************/
/**************************** PROTOTYPE Declaration ***********************************/
void cpu_com_init( void );
//bool DTM2Wire_Check( void );
//bool DTM2Wire_Start( void );
void cpu_com_read_comp( void );
void cpu_com_write_comp( void );
void cpu_com_error_comp( void );
extern void codeptr app_evt_usr0(void); 

// ========================================
// H1D共通コード
// ========================================



//RD8001暫定：DSをどうするか検討

/******************/
/* 定数マクロ定義 */
/******************/
#define CPU_COM_SND_DATA_SIZE_STATUS_REQ			4				/* ステータス要求			*/
#define CPU_COM_SND_DATA_SIZE_MODE_CHG				1				/* 状態変更(G1D)			*/
#define CPU_COM_SND_DATA_SIZE_PC_LOG				10				/* PCログ送信(内部コマンド)	*/
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			15				/* センサーデータサイズ		*/
#define CPU_COM_SND_DATA_SIZE_DATE_SET				7				/* 時刻設定					*/
#define CPU_COM_SND_DATA_SIZE_DISP_ORDER			1				/* 表示指示					*/
#define CPU_COM_SND_DATA_SIZE_SENSING_ORDER			1				/* センシング指示			*/

/* データ長範囲 */
#define CPU_COM_DATA_SIZE_MAX					(262+1)		/* データの最大長 */
															/* 最大データ 表示データ更新 詳細スランプグラフ 種別No(2)+データ長(2)+データ(308) */
															
															
// 各データサイズ
#define CPU_COM_CMD_SIZE						(1)			/* コマンドデータサイズ 1byte */
#define CPU_COM_SEQ_SIZE						(1)			/* SEQNoデータサイズ 1byte */
#define CPU_COM_CRC_SIZE						(2)			/* CRCデータサイズ 2byte */
#define CPU_COM_STX_SIZE						(2)			/* STXデータサイズ 2byte */
#define CPU_COM_ETX_SIZE						(2)			/* ETXデータサイズ 2byte */

/* メッセージの最小 */
#define CPU_COM_MSG_SIZE_MIN					( CPU_COM_CMD_SIZE + \
												  CPU_COM_SEQ_SIZE + \
												  CPU_COM_CRC_SIZE + \
												  CPU_COM_STX_SIZE + \
												  CPU_COM_ETX_SIZE )


/* メッセージの最大 コマンド(1)+SEQ(2)+データ(262)+SUM(2)+CRC(2) */
#define CPU_COM_MSG_SIZE_MAX					(CPU_COM_DATA_SIZE_MAX + CPU_COM_MSG_SIZE_MIN)

/* 送信ステータス */
#define CPU_COM_SND_STATUS_IDLE					0			/* 送信可能状態 */
#define CPU_COM_SND_STATUS_RCV_WAIT				1			/* 応答受信待ち状態 */
#define CPU_COM_SND_STATUS_RENSOU				2			/* 連送中状態 */
#define CPU_COM_SND_STATUS_COMPLETE				3			/* 送信完了状態 */
#define CPU_COM_SND_STATUS_SEND_NG				4			/* リトライNG */
#define CPU_COM_SND_STATUS_SEND_WAIT			5			/* 送信待ち */

/* CPU間通信受信待ち送信リクエスト結果 */
#define CPU_COM_SND_RES_OK						0			/* 応答正常受信 */
#define CPU_COM_SND_RES_BUSY_NG					1			/* CPU間通信BUSY状態(送信中or受信待ち)NG */
#define CPU_COM_SND_RES_RETRY_OUT_NG			2			/* リトライアウト */
#define CPU_COM_SND_RES_COM_NG					3			/* 送信コマンドNG */


/* CPU間通信 コマンド種別 */
/* 要求・応答のセット */
typedef enum _CPU_COM_CMD_ID{
	CPU_COM_CMD_NONE=0,							/* 【CPU間通信コマンド】コマンド無し				*/
	CPU_COM_CMD_STATUS_REQ,						/* 【CPU間通信コマンド】ステータス要求				*/
	CPU_COM_CMD_SENSOR_DATA,					/* 【CPU間通信コマンド】センサーデータ更新			*/
	CPU_COM_CMD_SENSING_ORDER,					/* 【CPU間通信コマンド】センシング指示				*/
	CPU_COM_CMD_MODE_CHG,						/* 【CPU間通信コマンド】状態変更(G1D)				*/
	CPU_COM_CMD_PC_LOG,							/* 【CPU間通信コマンド】PCログ送信(内部コマンド)	*/
	CPU_COM_CMD_DATE_SET,						/* 【CPU間通信コマンド】日時設定					*/

	CPU_COM_CMD_PRG_DOWNLORD_READY,				/* 【CPU間通信コマンド】プログラム転送準備		*/
	CPU_COM_CMD_PRG_DOWNLORD_START,				/* 【CPU間通信コマンド】プログラム転送開始		*/
	CPU_COM_CMD_PRG_DOWNLORD_ERASE,				/* 【CPU間通信コマンド】プログラム転送消去		*/
	CPU_COM_CMD_PRG_DOWNLORD_DATA,				/* 【CPU間通信コマンド】プログラム転送データ	*/
	CPU_COM_CMD_PRG_DOWNLORD_RESLUT,			/* 【CPU間通信コマンド】プログラム転送結果		*/
	CPU_COM_CMD_PRG_DOWNLORD_CHECK,				/* 【CPU間通信コマンド】プログラム転送確認		*/
	CPU_COM_CMD_DISP_ORDER,						/* 【CPU間通信コマンド】表示指示				*/
	CPU_COM_CMD_VERSION,						/* 【CPU間通信コマンド】バージョン				*/
	CPU_COM_CMD_MAX								/* 【CPU間通信コマンド】最大値					*/
}CPU_COM_CMD_ID;



/* コマンドタイプ */
/* ※マスター専用 */
#define CPU_COM_CMD_TYPE_ONESHOT_SEND			0					/* 単発送信コマンド */
#define CPU_COM_CMD_TYPE_RETRY					1					/* リトライ */
#define CPU_COM_CMD_TYPE_RENSOU					2					/* 連送 */
#define CPU_COM_CMD_TYPE_ONESHOT_RCV			3					/* 単発受信コマンド */


// RD8001暫定：ドライバへ移動
/* 送信ステータス */
#define DRV_CPU_COM_STATUS_CAN_SEND		(0)			/* 送信可能状態 */
#define DRV_CPU_COM_STATUS_SENDING		(1)			/* 送信中 */
//#define DRV_CPU_COM_STATUS_SEND_END		(2)			/* 送信終了 */ /* 送信終了状態から5ms後に送信可能となる */



/* =====通信仕様関連===== */
// 通信データに於ける制御コード
#define CPU_COM_CTRL_CODE_STX			0x02					/* STX */
#define CPU_COM_CTRL_CODE_ETX			0x03					/* ETX */
#define CPU_COM_CTRL_CODE_DLE			0x10					/* DLE */

/* 読み出し(文字列、ファイル)結果 */
typedef enum{
	CPU_COM_RCV_MSG_STATE_STX_WAIT = 0,			/* STX待ち */
	CPU_COM_RCV_MSG_STATE_ETX_WAIT,				/* ETX待ち */
}CPU_COM_RCV_MSG_STATE;

// メッセージ配列の添え字
#define CPU_COM_MSG_TOP_POS_CMD					2				/* コマンド */
#define CPU_COM_MSG_TOP_POS_SEQ					3				/* シーケンス */
#define CPU_COM_MSG_TOP_POS_DATA				4				/* データ部 */




// =========================
// データ種別
// =========================
// 表示指示
#define CPU_COM_DISP_ORDER_NON				0	// 表示なし
#define CPU_COM_DISP_ORDER_SELF_CHECK_ERR	1	// 異常
#define CPU_COM_DISP_ORDER_SELF_CHECK_FIN	2	// 完了


/******************/
/*   構造体定義   */
/******************/
typedef struct _T_CPU_COM_CMD_INFO {
	UB cmd;					/* コマンド */
	UB cmd_type;			/* コマンドタイプ */
	UB retry_cnt;			/* リトライ回数 */
	UW retry_time;			/* リトライ間隔 */
	UB rensou_cnt;			/* 連送回数 */
} T_CPU_COM_CMD_INFO;

typedef struct{
	UB buf[CPU_COM_MSG_SIZE_MAX];		/* バッファ */
	UH pos;								/* 受信位置 */
	UB last_dle_flg;					/* 前回受信データがDLE */
	UB state;							/* 前回受信データがDLE */
}CPU_COM_RCV_MSG;



/******************/
/*  外部参照宣言  */
/******************/
extern void cpu_com_init_sub(void);
extern void cpu_com_proc(void);
extern void cpu_com_sub_reset(void);
extern UB cpu_com_get_status(void);
extern UH cpu_com_dle_extension( UB* data, UH size );
extern UB cpu_com_file_write( UB* p_buf, UH size, UW block, UB file_no );
extern UB cpu_com_file_read( UB* p_buf, UH size, UW block, UB file_no );
extern void drv_uart0_data_init( void );
//extern RING_BUF* drv_uart0_get_snd_ring( void );
//extern RING_BUF* drv_uart0_get_rcv_ring( void );
extern UB cpu_com_get_busy( void );




#endif // __CPU_COM_INC__
