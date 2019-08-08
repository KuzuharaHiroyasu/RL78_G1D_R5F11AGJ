/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : user_main_tbl.h												*/
/* 機能         : 全体管理（テーブル定義）										*/
/* 変更履歴     : 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/
/************************************************************/
/* プラットフォーム											*/
/************************************************************/



static const struct ke_msg_handler cpu_com_handler[] =
{
	{ USER_MAIN_CYC_ACT			, (ke_msg_func_t)user_main_cyc },
//	{ USER_MAIN_CALC_SEKIGAI	, (ke_msg_func_t)main_calc_sekigai },
//	{ USER_MAIN_CALC_SEKISHOKU	, (ke_msg_func_t)main_calc_sekishoku },
	{ USER_MAIN_CALC_KOKYU		, (ke_msg_func_t)main_calc_kokyu },
	{ USER_MAIN_CALC_IBIKI		, (ke_msg_func_t)main_calc_ibiki },
	{ USER_MAIN_CALC_ACL		, (ke_msg_func_t)main_calc_acl },
	{ USER_MAIN_CYC_CALC_RESULT	, (ke_msg_func_t)user_main_calc_result_cyc },
	{ USER_MAIN_CYC_BATTERY		, (ke_msg_func_t)battery_level_cyc },
	{ USER_MAIN_CYC_LED			, (ke_msg_func_t)led_cyc },
	{ USER_MAIN_CYC_PHOTOREF	, (ke_msg_func_t)main_calc_photoref },
};

/* Status Handler */
const struct ke_state_handler user_main_state_handler[ USER_MAIN_STATE_MAX ] =
{   /* State message handlers */
    KE_STATE_HANDLER(cpu_com_handler)
};

/* Default Handler */
const struct ke_state_handler user_main_default_handler = KE_STATE_HANDLER_NONE;

/************************************************************/
/* ユーザー定義												*/
/************************************************************/
/* バージョン表記の注意事項 */
const B		version_product_tbl[]= {0, 0, 1, 1};				/* ソフトウェアバージョン */
																/* バージョン表記ルール */
																/* ①メジャーバージョン：[0 ～ 99] */
																/* ②マイナーバージョン：[0 ～ 9] */
																/* ③リビジョン：[0 ～ 99] */
																/* ④ビルドバージョン：[0 ～ 99] */


/* CPU間通信受信データ処理 関数テーブル */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* コマンド */		/* 関数  */					/* 応答有無 */
	{	0x00,			NULL,								OFF	},	/* 【CPU間通信コマンド】コマンド無し				*/
	{	0xE0,			main_cpu_com_rcv_sts_res,			OFF	},	/* 【CPU間通信コマンド】ステータス要求				*/
	{	0xA0,			main_cpu_com_rcv_sensor_res,		OFF	},	/* 【CPU間通信コマンド】センサーデータ更新			*/
	{	0xA1,			NULL,								OFF	},	/* 【CPU間通信コマンド】センシング指示				*/
	{	0xB0,			main_cpu_com_rcv_mode_chg,			OFF	},	/* 【CPU間通信コマンド】状態変更(G1D)				*/
	{	0xF0,			NULL,								OFF	},	/* 【CPU間通信コマンド】PCログ送信(内部コマンド)	*/
	{	0xB1,			main_cpu_com_rcv_date_set,			OFF	},	/* 【CPU間通信コマンド】日時設定					*/
	{	0xD5,			main_cpu_com_rcv_prg_hd_ready,		OFF	},	/* 【CPU間通信コマンド】プログラム転送準備		*/
	{	0xD2,			main_cpu_com_rcv_prg_hd_start,		OFF	},	/* 【CPU間通信コマンド】プログラム転送開始		*/
	{	0xD4,			main_cpu_com_rcv_prg_hd_erase,		OFF	},	/* 【CPU間通信コマンド】プログラム転送消去		*/
	{	0xD0,			main_cpu_com_rcv_prg_hd_data,		OFF	},	/* 【CPU間通信コマンド】プログラム転送データ		*/
	{	0xD1,			main_cpu_com_rcv_prg_hd_reslut,		OFF	},	/* 【CPU間通信コマンド】プログラム転送結果		*/
	{	0xD3,			main_cpu_com_rcv_prg_hd_check,		OFF	},	/* 【CPU間通信コマンド】プログラム転送確認		*/
	{	0xB2,			main_cpu_com_rcv_disp_order,		OFF	},	/* 【CPU間通信コマンド】表示指示				*/
	{	0xB3,			main_cpu_com_rcv_version,			OFF	},	/* 【CPU間通信コマンド】バージョン				*/
};

/* VUART通信受信データ処理 関数テーブル */
STATIC const VUART_RCV_CMD_TBL s_vuart_rcv_func_tbl[VUART_CMD_TYPE_MAX] = {
	/* コマンド */				/* レングス */				/* 関数  */					/* 応答有無 */
	{	0x00,					0,							NULL							},	// なし
	{	VUART_CMD_MODE_CHG,		VUART_CMD_LEN_MODE_CHG,		main_vuart_rcv_mode_chg			},	// 状態変更(G1D)
	{	VUART_CMD_DATE_SET,		VUART_CMD_LEN_DATE_SET,		main_vuart_rcv_date				},	// 日時設定
	{	VUART_CMD_INFO,			VUART_CMD_LEN_INFO,			main_vuart_rcv_info				},	// 情報取得
	{	VUART_CMD_VERSION,		VUART_CMD_LEN_VERSION,		main_vuart_rcv_version			},	// バージョン取得
	{	VUART_CMD_DEVICE_INFO,	VUART_CMD_LEN_DEVICE_INFO,	main_vuart_rcv_device_info		},	// デバイス状況取得
	{	VUART_CMD_DATA_NEXT,	VUART_CMD_LEN_DATA_NEXT,	NULL							},	// NEXT[送信専用]
	{	VUART_CMD_DATA_END,		VUART_CMD_LEN_DATA_END,		main_vuart_rcv_data_end			},	// END[受信はSET時]
	{	VUART_CMD_DATA_FRAME,	VUART_CMD_LEN_DATA_FRAME,	main_vuart_rcv_data_frame		},	// 枠情報(日時等)[受信はSET時]
	{	VUART_CMD_DATA_CALC,	VUART_CMD_LEN_DATA_CALC,	main_vuart_rcv_data_calc		},	// 機器データ[受信はSET時]
	{	VUART_CMD_DATA_FIN,		VUART_CMD_LEN_DATA_FIN,		main_vuart_rcv_data_fin			},	// データ取得完了通知
	{	VUART_CMD_INVALID,		VUART_CMD_LEN_PRG_DATA,		main_vuart_rcv_prg_hd_record	},	// プログラム転送(データ)
	{	VUART_CMD_PRG_RESULT,	VUART_CMD_LEN_PRG_RESULT,	main_vuart_rcv_prg_hd_result	},	// プログラム転送結果
	{	VUART_CMD_PRG_CHECK,	VUART_CMD_LEN_PRG_CHECK,	main_vuart_rcv_prg_hd_update	},	// プログラム更新完了確認
	{	VUART_CMD_DEVICE_SET,	VUART_CMD_LEN_DEVICE_SET,	main_vuart_rcv_device_set		},	// デバイス設定変更
//	{	VUART_CMD_ALARM_SET,	VUART_CMD_LEN_ALARM_SET,	main_vuart_rcv_alarm_set		},	// アラーム設定変更
//	{	VUART_CMD_ALARM_INFO,	0,							NULL							},	// アラーム通知[送信専用]
};

/* モード別処理 */
typedef void (*MODE_FUNC)(void);
STATIC MODE_FUNC p_user_main_mode_func[] = {								user_main_mode_inital,			// SYSTEM_MODE_INITIAL			イニシャル
																			user_main_mode_idle_rest,		// SYSTEM_MODE_IDLE_REST		アイドル_残量表示
																			user_main_mode_idle_com,		// SYSTEM_MODE_IDLE_COM			アイドル_通信待機
																			user_main_mode_sensing,			// SYSTEM_MODE_SENSING			センシング
																			user_main_mode_get,				// SYSTEM_MODE_GET				データ取得
																			user_main_mode_prg_h1d,			// SYSTEM_MODE_PRG_H1D			H1Dプログラム更新
																			user_main_mode_prg_g1d,			// SYSTEM_MODE_PRG_G1D			G1Dプログラム更新
																			user_main_mode_self_check,		// SYSTEM_MODE_PRG_SELF_CHECK	自己診断
																			user_main_mode_move,			// SYSTEM_MODE_MOVE				移行
																			dummy,							// SYSTEM_MODE_NON				なし
};

typedef SYSTEM_MODE ( *EVENT_TABLE )( int event );
STATIC EVENT_TABLE p_event_table[ EVENT_MAX ][ SYSTEM_MODE_MAX ] = {
// モード				INITAL				IDLE_REST			IDLE_COM			SENSING			GET					PRG_H1D				PRG_G1D			SELF_CHECK		MOVE				NON
/*イベントなし		*/	{ evt_non,			evt_non,			evt_non,			evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
///*電源SW(短)		*/	{ evt_idle_rest,	evt_sensing,		evt_sensing_chg,	evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*電源SW(短)		*/	{ evt_non,			evt_non,			evt_non,			evt_bat_check,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
	///*電源SW押下(長)	*/	{ evt_non,			evt_initial,		evt_initial_chg,	evt_initial,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*電源SW押下(長)	*/	{ evt_non,			evt_sensing,		evt_sensing_chg,	evt_idle_com,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*充電検知ポートON	*/	{ evt_idle_com,		evt_idle_com,		evt_non,			evt_idle_com,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*充電検知ポートOFF	*/	{ evt_non,			evt_non,			evt_non,			evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*電池残量低下		*/	{ evt_non,			evt_non,			evt_initial,		evt_initial,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*充電完了			*/	{ evt_non,			evt_non,			evt_initial,		evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*データ取得		*/	{ evt_non,			evt_get,			evt_get,			evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*プログラム(H1D)	*/	{ evt_non,			evt_h1d_prg_denchi,	evt_h1d_prg_denchi,	evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*プログラム(G1D)	*/	{ evt_non,			evt_g1d_prg_denchi,	evt_g1d_prg_denchi,	evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*自己診断(通信)	*/	{ evt_non,			evt_self_check,		evt_self_check,		evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*完了				*/	{ evt_non,			evt_non,			evt_non,			evt_idle_com,	evt_idle_com,		evt_prg_h1d_fin,	evt_idle_com,	evt_idle_com,	evt_non,			evt_non },
/*中断				*/	{ evt_non,			evt_non,			evt_non,			evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*タイムアウト		*/	{ evt_non,			evt_idle_com,		evt_initial_chg,	evt_non,		evt_non,			evt_prg_h1d_time_out,evt_non,		evt_non,		evt_non,			evt_non },
/*検査				*/	{ evt_idle_com_denchi,evt_self_check,	evt_self_check,		evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non }
};

// 1ページ256byteの０埋めテーブル
STATIC const UB s_eep_page0_tbl[EEP_ACCESS_ONCE_SIZE] = { 0 };

