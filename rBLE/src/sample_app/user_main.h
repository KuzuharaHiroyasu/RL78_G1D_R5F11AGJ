/********************************************************************************/
/* システム名   : RD8001														*/
/* ファイル名   : main.h														*/
/* 機能         : ユーザーメイン(マクロ定義、型定義、関数の外部参照宣言)		*/
/* 変更履歴     : 2017.12.20 Axia Soft Design 西島		初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/

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
#define USER_MAIN_STATE_MAX		2	// Max State Num
#define USER_MAIN_IDX_MAX			1	// Max ID Num

#define USER_MAIN_ID (TASK_USR_0)

#define USER_MAIN_CYC_ACT				KE_FIRST_MSG(USER_MAIN_ID) + 1
#define USER_MAIN_CALC_SEKIGAI			KE_FIRST_MSG(USER_MAIN_ID) + 2
#define USER_MAIN_CALC_SEKISHOKU		KE_FIRST_MSG(USER_MAIN_ID) + 3
#define USER_MAIN_CALC_KOKYU			KE_FIRST_MSG(USER_MAIN_ID) + 4
#define USER_MAIN_CALC_IBIKI			KE_FIRST_MSG(USER_MAIN_ID) + 5
#define USER_MAIN_CALC_ACL				KE_FIRST_MSG(USER_MAIN_ID) + 6
#define USER_MAIN_CYC_CALC_RESULT		KE_FIRST_MSG(USER_MAIN_ID) + 7
#define USER_MAIN_CYC_BATTERY			KE_FIRST_MSG(USER_MAIN_ID) + 8





//#define USER_MAIN_CYC_ACT			1								/* Task API ID */
//#define CPU_COM_TIMER_API		2								/* Timer API ID */


#if 0
#define RBLE_APP_CON_SW_EVT			KE_FIRST_MSG(TASK_CON_APPL) + 3	/* SW API ID */
#define RBLE_APP_CON_SEND_EVT		KE_FIRST_MSG(TASK_CON_APPL) + 4	/* SEND API ID */
#endif


#define  KE_EVT_USR_1_BIT			CO_BIT(31 - KE_EVT_USR_1) 
#define  KE_EVT_USR_2_BIT			CO_BIT(31 - KE_EVT_USR_2) 
#define  KE_EVT_USR_3_BIT			CO_BIT(31 - KE_EVT_USR_3) 

#if 0
extern volatile RBLE_RUNNING_MODE	rBLE_Run_Mode_Flg;
#endif

/* Status Handler */
extern const struct ke_state_handler user_main_state_handler[ USER_MAIN_STATE_MAX ];
/* Default Handler */
extern const struct ke_state_handler user_main_default_handler;
/* Status */
extern ke_state_t user_main_state[ USER_MAIN_IDX_MAX ];



// ============================================================
// ==================ユーザーアプリケーション==================
// ============================================================
// システムモード ※H1D/G1D共通
typedef enum{
	// 仕様上の状態下限
	SYSTEM_MODE_INITIAL = 0,				// イニシャル
	SYSTEM_MODE_IDLE_REST,					// アイドル_残量表示
	SYSTEM_MODE_IDLE_COM,					// アイドル_通信待機
	SYSTEM_MODE_SENSING,					// センシング
	SYSTEM_MODE_GET,						// データ取得
	SYSTEM_MODE_PRG_H1D,					// H1Dプログラム更新
	SYSTEM_MODE_PRG_G1D,					// G1Dプログラム更新
	SYSTEM_MODE_SELF_CHECK,					// 自己診断
	// 仕様上の状態上限
	SYSTEM_MODE_MOVE,						// 移行
	SYSTEM_MODE_NON,						// なし
	SYSTEM_MODE_MAX
}SYSTEM_MODE;


// イベント ※H1D/G1D共通
typedef enum{
	EVENT_NON = 0,				// なし
	EVENT_POW_SW_SHORT,			// 電源SW押下(短)
	EVENT_POW_SW_LONG,			// 電源SW押下(長)
	EVENT_CHG_PORT_ON,			// 充電検知ポートON
	EVENT_CHG_PORT_OFF,			// 充電検知ポートOFF
	EVENT_DENCH_LOW,			// 電池残量低下
	EVENT_CHG_FIN,				// 充電完了
	EVENT_GET_DATA,				// データ取得
	EVENT_H1D_PRG,				// プログラム書き換え(H1D)
	EVENT_G1D_PRG,				// プログラム書き換え(G1D)
	EVENT_SELF_CHECK_COM,		// 自己診断(通信)
	EVENT_COMPLETE,				// 完了
	EVENT_STOP,					// 中断
	EVENT_TIME_OUT,				// タイムアウト
	EVENT_KENSA_ON,				// 検査ポートON
	EVENT_MAX,					// 最大
}EVENT_NUM;

/* 異常ID定義(デバッグ機能) */
typedef enum{
	ERR_ID_EEP = 1					,			/* EEP(汎用) */
	ERR_ID_I2C						,			/* I2C(汎用) */
	ERR_ID_MAIN						,			/* MAIN(汎用) */
	ERR_ID_CPU_COM					,			/* CPU間通信(汎用) */
	ERR_ID_ACL						,			/* 加速度センサ(汎用) */

	ERR_ID_CPU_COM_ERR = 40			,			/* CPU間通信異常 */
	ERR_ID_CPU_COM_RCV_RING			,			/* CPU間通信受信リングバッファ ※演算中はリングバッファを引き取れないので発生してしまう */
	ERR_ID_DRV_UART_OERR = 50		,			/* UARTドライバ(オーバーランエラー) */
	
	// 高レベル異常
	ERR_ID_BLE_SEND_ERR = 97	,				/* BLE送信異常 */
	ERR_ID_CPU_COM_RETRYOUT = 98	,			/* CPU間通信リトライアウト(致命的) */
	ERR_ID_LOGIC = 99				,			/* ロジック不具合(汎用) */

	ERR_ID_MAX									/* 異常ID最大	*/
}ERR_ID;

// 電池残量閾値
#define BAT_LEVEL_1_VAL						(UH)( 1023.0 * (1.95 / 3.0 ))		// 1.95V以上
#define BAT_LEVEL_2_VAL						(UH)( 1023.0 * (1.72 / 3.0 ))		// 1.72V以上
#define BAT_LEVEL_3_VAL						(UH)( 1023.0 * (1.5 / 3.0 ))		// 1.5V以上
#define BAT_LEVEL_CHG_FIN_VAL				(UH)( 1023.0 * (2.0 / 3.0 ))		// 2.0V以上

// 電池残量状態
#define BAT_LEVEL_STS_MAX					0	// 充電満タン
#define BAT_LEVEL_STS_HIGH					1	// 数日持つ
#define BAT_LEVEL_STS_LOW					2	// 残り１日持たない
#define BAT_LEVEL_STS_MIN					3	// 電池残量なし

// 体の向き
#define BODY_DIRECTION_LEFT					0	// 左向き
#define BODY_DIRECTION_UP					1	// 上向き
#define BODY_DIRECTION_RIGHT				2	// 右向き
#define BODY_DIRECTION_DOWN					3	// 下向き

#define BODY_DIRECTION_MASK					3	// bitマスク用
#define BODY_DIRECTION_BIT					2	// 使用bit数

// プログラムシーケンス
typedef enum{
	PRG_SEQ_IDLE = 0,					// アイドル
	PRG_SEQ_READY_WAIT,					// 開始待ち　※ブートへの切替
	PRG_SEQ_START_WAIT,					// 開始待ち　※アプリへの切替
	PRG_SEQ_ERASE_WAIT,					// 消去待ち　※フラッシュ消去
	PRG_SEQ_COMPLETE_WAIT,				// 完了待ち　※結果通知
	PRG_SEQ_MAX
}PRG_SEQ;


// タイマー関連
#define			MAIN_STATUS_REQ_TIME			50		// ステータス要求時間[500ms]
//#define			MAIN_STATUS_REQ_TIME			100		// ステータス要求時間[1s]

#define			SENSING_END_JUDGE_TIME				100		// センシングなし[1s]

#define		TIME_CNT_DISP_SELF_CHECK_ERR			(300)		/* 自己診断異常表示(3秒) */
#define		TIME_CNT_DISP_SELF_CHECK_FIN			(300)		/* 自己診断完了表示(3秒) */


#define		TIME_CNT_BAT_LEVEL_MIN_INTERVAL		(100)		/* 電池残量低下通知インターバル(1秒) */

#define		BAT_LEVEL_GET_CYC						600		// 600秒(10分)



#define		SENSING_CNT_MIN							(40)		/* センシング回数の下限(20分) */
//#define		SENSING_CNT_MIN							(1)		/* センシング回数の下限(30秒)[デバッグ用短縮版] */

// 測定個数
#define		MEAS_SEKIGAI_CNT_MAX		140
#define		MEAS_SEKISHOKU_CNT_MAX		140
#define		MEAS_KOKYU_CNT_MAX			200
#define		MEAS_IBIKI_CNT_MAX			200
#define		MEAS_ACL_CNT_MAX			20

// 演算結果書き込みタイミング
#define		CALC_RESULT_WR_CYC			30			// 30秒
//#define		CALC_RESULT_WR_CYC			3			// 3秒[デバッグ用短縮版]



// 秒タイマー ※+1した値を設定 // RD8001暫定：本来は経過時間[10ms]を使いたい
#define		TIMER_SEC_PRG_READY_WAIT	( 1 + 1 )
#define		TIMER_SEC_PRG_START_WAIT	( 1 + 1 )
#define		TIMER_SEC_PRG_ERASE_WAIT	( 27 + 1 )

// システムモードタイマアウト[10ms]
#define		TIME_OUT_SYSTEM_MODE_IDLE_REST		( 600 )
#define		TIME_OUT_SYSTEM_MODE_IDLE_COM		( 18000 )		
#define		TIME_OUT_SYSTEM_MODE_H1D_PRG		( 60000 )		

// データ秒間フェイズ(いびき,呼吸,体の向き,フォトセンサ共通)
typedef enum{
	SEC_PHASE_0_10 = 0,					// 0〜10秒まで
	SEC_PHASE_10_20,					// 10〜20秒まで
	SEC_PHASE_20_30,					// 20〜30秒まで
	SEC_PHASE_NUM,
}SEC_PHASE;




#define	OK_NOW_UPDATING					0		//更新未了
#define	OK_UPDATE_FIX					1		//正常完了(成功)
#define	NG_UPDATE_FAILURE				2		//異常完了(失敗)

#define	PRG_HD_UPDATE_STATE_NON			0	//更新未了
#define	PRG_HD_UPDATE_STATE_OK			1	//正常完了(成功)
#define	PRG_HD_UPDATE_STATE_NG			2	//異常完了(失敗)




#define	BD_ADRS_NUM						6			// BLEアドレス数


///10ms timer
#define PERIOD_5SEC     500U
#define PERIOD_1SEC     100U
#define PERIOD_10MSEC   1U		//RD8001対応：定義追加
#define PERIOD_20MSEC   2U		//RD8001対応：定義追加
#define PERIOD_50MSEC   5U		//RD8001対応：定義追加
#define PERIOD_100MSEC   10U

///20ms timer
#define	TIME_20MS_CNT_POW_SW_LONG			150				/* 電源SW_長(3秒) */
#define	TIME_20MS_CNT_POW_SW_SHORT			50				/* 電源SW_短(1秒) */


typedef enum program_ver{
	VERSION_MAJOR = 0,
	VERSION_MINOR,
	VERSION_REVISION,
	VERSION_BUILD,
	//-------
	VERSION_NUM
}tag_program_ver;



// 測定情報
typedef struct{
	union{
		UB	byte[CPU_COM_SND_DATA_SIZE_SENSOR_DATA];
		struct{
			W	sekigaival;		// 差動入力の為に符号あり
			W	sekishoku_val;	// 差動入力の為に符号あり
			UH	kokyu_val;		
			UH	ibiki_val;		
			B acl_x;
			B acl_y;
			B acl_z;
			UH photoref_val;			// フォトリフレクタ
		}dat;
	}info;
}MEAS;

// 演算情報
typedef struct{
	union{
		UB	byte[EEP_CACL_DATA_SIZE];
		struct{
			UB	ibiki_val[SEC_PHASE_NUM];	// いびきの大きさ
			UB	state;						// 状態
			UB	body_direct;				// 体の向き
			UB	photoref[SEC_PHASE_NUM];	// フォトセンサー
			UB	dummy[4];					// 境界値調整用
		}dat;
	}info;
}CALC;

// 日時情報
typedef struct{
	UB year;
	UB month;
	UB week;
	UB day;
	UB hour;
	UB min;
	UB sec;
}DATE;

// 警告機能
typedef struct{
	union{
		UB	byte[EEP_ALARM_SIZE];
		struct{
			UB	act_mode;		// 動作モード
			UB	ibiki_sens;		// いびき感度
			UB	yokusei_str;	// 抑制強度
			UB	yokusei_max_time;	// 抑制動作最大継続時間
		}dat;
	}info;
}ALARM;


// 自己診断
typedef struct{
	UW	last_time;
	UH	eep_cnt;		// EEP消去回数
	UB	seq;			// シーケンス
	UB	com_flg;		// 通信での自己診断フラグ
}SELF_CHECK;

// H1D情報
typedef struct{
	UB	ble				:1;		/* 1  BLE接続 */
	UB	dummy1			:1;		/* 2  未定義  */
	UB	dummy2			:1;		/* 3  未定義 */
	UB	dummy3			:1;		/* 4  未定義 */
	UB	dummy4			:1;		/* 5  未定義 */
	UB	dummy5			:1;		/* 6  未定義 */
	UB	dummy6			:1;		/* 7  未定義 */
	UB	dummy7			:1;		/* 8  未定義 */
}BIT_G1D_INFO;
typedef struct{
	union {
		UB	byte;
		BIT_G1D_INFO bit_f;
	}info;
}G1D_INFO;


// H1D情報
typedef struct{
	UB	bat_chg			:1;		/* 1  充電検知ポート */
	UB	kensa			:1;		/* 2  検査ポート */
	UB	dummy1			:1;		/* 3  未定義 */
	UB	dummy2			:1;		/* 4  未定義 */
	UB	dummy3			:1;		/* 5  未定義 */
	UB	dummy4			:1;		/* 6  未定義 */
	UB	dummy5			:1;		/* 7  未定義 */
	UB	dummy6			:1;		/* 8  未定義 */
}BIT_H1D_INFO;
typedef struct{
	union {
		UB	byte;
		BIT_H1D_INFO bit_f;
	}info;
}H1D_INFO;


// フレーム(枠)番号
typedef struct{
	UB read;		// フレーム(枠)の読み出し番号
	UB write;		// フレーム(枠)の書き込み番号
	UB cnt;		// フレーム(枠)の書き込み数　※書き込み数0と最大の区別がつかない為
}FRAME_NUM_INFO;

typedef struct{
	
	SYSTEM_MODE system_mode;			/* システムモード */
	SYSTEM_MODE next_system_mode;		/* 次のシステムモード */
	SYSTEM_MODE last_system_mode;		/* 前回のシステムモード */
	
	UW system_mode_time_out_cnt;		/* システムモードタイムアウトカウント[10ms] */

	DATE date;
	
	UW last_time_sts_req;			//前回時間(ステータス要求)
	UW last_sensing_data_rcv;		//前回センシングデータ受信
	UB sensing_flg;					// センシング中フラグ
	
	UB battery_sts;			// 電池状態
	H1D_INFO h1d;			// H1D情報
	H1D_INFO h1d_last;		// H1D情報(前回)
	
	
	// 演算関連
	CALC calc;				// 演算後データ
	UH calc_cnt;			// 演算カウント(保存データ数)
	UH ibiki_detect_cnt;	// いびき検知数
	UH mukokyu_detect_cnt;	// 無呼吸検知数
	UH ibiki_time;			// いびき時間
	UH mukokyu_time;		// 無呼吸時間
	UH max_mukokyu_sec;		// 最大無呼吸[秒]
	
	UB phase_ibiki;			// 秒間フェイズ(いびき)
	UB phase_kokyu;			// 秒間フェイズ(呼吸)
	UB phase_body_direct;	// 秒間フェイズ(体の向き)
	UB phase_photoref;		// 秒間フェイズ(フォトセンサ)
	
	// フレーム(枠)番号
	FRAME_NUM_INFO frame_num;			// フレーム(枠)番号[EEPコピーエリア] ※EEPとは一致させておく
	FRAME_NUM_INFO frame_num_work;		// フレーム(枠)番号ワーク

	ALARM	alarm;						// 警告機能[EEPコピーエリア] ※EEPとは一致させておく
	
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
	UB	sekishoku_cnt;
	UB	kokyu_cnt;
	UB	ibiki_cnt;
	UB	acl_cnt;
	UB	sekigai_seq;		// 赤外有効/無効切替用のシーケンス
	
	
	UW	timer_sec;			// タイマー[秒]　※カウントダウン
	
	// timer
	UH tick_10ms;
	UH tick_10ms_sec;
	UH tick_10ms_new;
	UW elapsed_time;									/* 経過時間(10ms) ※約1年132日継続して演算可能 */
	UH tick_vib_10ms_sec;
	
	UW last_time_battery_level_min;			// 電池残量低下時間[10ms]
	UW sw_time_cnt;							// 電源SW押下時間カウンタ
	UB pow_sw_last;							// 電源ボタン状態(前回)
	
	// 以降ワーク領域
	UW sec30_cnt;			//30秒カウント
	UW sec10_cnt;			//10秒カウント
	UW sec7_cnt;			//7秒カウント
	UW sec600_cnt;			//10分カウント
	
	UB get_mode_seq;				// GETモードシーケンス
	UH get_mode_calc_cnt;
	UH set_mode_calc_cnt;
	
	// プログラム書き換え用
	UH prg_hd_eep_record_cnt_wr;					// レコードカウント
	UH prg_hd_eep_record_cnt_rd;					// レコードカウント
	UW prg_hd_eep_code_record_sum;					// レコードサム値
	UB prg_hd_update_state;							// プログラム更新状態
	UB prg_hd_seq;									// プログラム更新状態
	UB prg_hd_version[VERSION_NUM];			// プログラムバージョン
	
	// プログラム書き換え(G1D)
	UB prg_g1d_send_ver_flg;				// G1Dバージョン送信
	UB prg_g1d_send_ver_sec;				// G1Dバージョン送信秒
	
	// 自己診断
	SELF_CHECK	self_check;
	
	UB	bd_device_adrs[BD_ADRS_NUM];						// BDデバイスアドレス
	
	// 異常関連デバッグ用途
	UW err_cnt;				//異常回数(デバッグ用途)
	ERR_ID last_err_id;		//前回異常ID(デバッグ用途)
	
	// 
	MEAS meas;				/* 計測値(50ms) */
	UH acl_timing;
	
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
/* CPU間通信 コマンド種別 */
/* 要求・応答のセット */
typedef enum{
	VUART_CMD_TYPE_NONE=0,							// なし
	VUART_CMD_TYPE_MODE_CHG,						// 状態変更(G1D)
	VUART_CMD_TYPE_DATE_SET,						// 日時設定
	VUART_CMD_TYPE_INFO,							// 情報取得
	VUART_CMD_TYPE_VERSION,							// バージョン取得
	VUART_CMD_TYPE_DEVICE_INFO,						// デバイス状況取得
	VUART_CMD_TYPE_DATA_NEXT,						// NEXT
	VUART_CMD_TYPE_DATA_END,						// END
	VUART_CMD_TYPE_DATA_FRAME,						// 枠情報(日時等)
	VUART_CMD_TYPE_DATA_CALC,						// 機器データ
	VUART_CMD_TYPE_DATA_FIN,						// データ取得完了通知
	VUART_CMD_TYPE_PRG_H1D_DATA,					// プログラム転送(データ)
	VUART_CMD_TYPE_PRG_H1D_RESULT,					// プログラム転送結果
	VUART_CMD_TYPE_PRG_H1D_CHECK,					// プログラム更新完了確認
	VUART_CMD_TYPE_DEVICE_SET,						// デバイス設定変更
//	VUART_CMD_TYPE_ALARM_SET,						// 設定変更
//	VUART_CMD_TYPE_ALARM_INFO,						// アラーム通知
	VUART_CMD_TYPE_MAX								// 最大値					
}VUART_CMD_TYPE;

typedef struct{
	UB cmd;							/* 受信コマンド */
	UB len;							/* 受信コマンド */
	void (*func)(void);				/* 受信処理 */
}VUART_RCV_CMD_TBL;


// コマンド
#define	VUART_CMD_MODE_CHG		0xB0
#define	VUART_CMD_SET_CHG		0xC0
#define	VUART_CMD_DATE_SET		0xC1
#define	VUART_CMD_INFO			0xC2
#define	VUART_CMD_DATA_NEXT		0xE0	// NEXT
#define	VUART_CMD_DATA_END		0xE1	// END
#define	VUART_CMD_DATA_FRAME	0xE2	// 枠
#define	VUART_CMD_DATA_CALC		0xE3	// 演算データ
#define	VUART_CMD_DATA_FIN		0xE4	// 完了
#define	VUART_CMD_PRG_RESULT	0xD1
#define	VUART_CMD_PRG_CHECK		0xD3
#define	VUART_CMD_PRG_G1D_START	0xF0
#define	VUART_CMD_PRG_G1D_VER	0xF9

//#define	VUART_CMD_ALARM_SET		0xC0

#define	VUART_CMD_INFO			0xC2
#define	VUART_CMD_VERSION		0xC3
#define	VUART_CMD_DEVICE_INFO	0xC5
//#define	VUART_CMD_ALARM_INFO	0xC4
#define	VUART_CMD_DEVICE_SET	0xC6
#define	VUART_CMD_INVALID		0xFF	// コマンド無し特殊処理


// 受信データ長 ※コマンド部含む
#define	VUART_CMD_LEN_MODE_CHG		2
#define	VUART_CMD_LEN_SET_CHG		3
#define	VUART_CMD_LEN_DATE_SET		8
#define	VUART_CMD_LEN_INFO			1
#define	VUART_CMD_LEN_DATA_NEXT		1	// NEXT
#define	VUART_CMD_LEN_DATA_END		1	// END
#define	VUART_CMD_LEN_DATA_FRAME	10	// 枠
#define	VUART_CMD_LEN_DATA_CALC		7	// 演算データ
#define	VUART_CMD_LEN_DATA_FIN		2	// 演算データ
#define	VUART_CMD_LEN_PRG_RESULT	5
#define	VUART_CMD_LEN_PRG_DATA		20
#define	VUART_CMD_LEN_PRG_CHECK		1
#define	VUART_CMD_LEN_PRG_G1D_START	1
#define	VUART_CMD_LEN_PRG_G1D_VER	1

#define	VUART_CMD_LEN_VERSION		1
#define	VUART_CMD_LEN_DEVICE_INFO	1

//#define	VUART_CMD_LEN_ALARM_SET		8
#define	VUART_CMD_LEN_DEVICE_SET	5

#define	VUART_CMD_ONLY_SIZE			1	// コマンドのみのサイズ

// 送信データ長 ※コマンド部含む
#define	VUART_SND_LEN_INFO			3
#define	VUART_SND_LEN_VERSION		6
#define	VUART_SND_LEN_DEVICE_INFO	16


#define VUART_DATA_SIZE_MAX				20

#define VUART_DATA_RESULT_OK		0
#define VUART_DATA_RESULT_NG		1



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

// 加速度センサ
#define ACL_DEVICE_ADR			0x1C				// 加速度センサデバイスアドレス
#if FUNC_DEBUG_LOG == ON
#define ACL_TIMING_VAL			10					// 加速度センサ・フォトセンサ処理タイミング
#else
#define ACL_TIMING_VAL			100					// 加速度センサ・フォトセンサ処理タイミング
#endif
#define I2C_LOCK_ERR_VAL		1000				// ロック異常判定閾値

#define I2C_WAIT		255					// スタートコンディション待ち ※200us程度なので最大値を設定しておく

// レジスタアドレス
#define ACL_REG_ADR_WHO_AM_I			0x0F				// WHO AM I
#define ACL_REG_ADR_DATA_XYZ			0x06				// XOUT,YOUT,ZOUT
#define ACL_REG_ADR_INT_SRC1			0x16				// INT_SOURCE1
#define ACL_REG_ADR_INT_REL				0x1A				// INT_REL
#define ACL_REG_ADR_CTRL_REG1			0x1B				// CTRL_REG1
#define ACL_REG_ADR_CTRL_REG2			0x1F				// CTRL_REG2


// レジスタデータ
#define ACL_REG_RECOGNITION_CODE		0x35				// 認識コード(0x35)

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
extern void user_system_init( void );
extern void user_main_init( void );
extern void err_info( ERR_ID id );
extern void main_cpu_com_snd_pc_log( UB* data, UB size );
extern void main_set_bd_adrs( UB* bda);
extern void user_main_timer_10ms_set( void );
extern void user_main_timer_cyc( void );
extern UW time_get_elapsed_time( void );
extern void reset_vib_timer(void);
extern void main_set_battery( void );

#endif // __MAIN_USR_INC__
