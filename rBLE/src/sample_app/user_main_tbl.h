/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : user_main_tbl.h													*/
/* 機能         : 全体管理（テーブル定義）										*/
/* 変更履歴     : 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/
/************************************************************/
/* プラットフォーム											*/
/************************************************************/



static const struct ke_msg_handler cpu_com_handler[] =
{
	{ USER_MAIN_CYC_ACT			, (ke_msg_func_t)cpu_com_evt },
	{ USER_MAIN_CALC_SEKIGAI	, (ke_msg_func_t)main_calc_sekigai },
	{ USER_MAIN_CALC_SEKISHOKU	, (ke_msg_func_t)main_calc_sekishoku },
	{ USER_MAIN_CALC_KOKYU		, (ke_msg_func_t)main_calc_kokyu },
	{ USER_MAIN_CALC_IBIKI		, (ke_msg_func_t)main_calc_ibiki },
	{ USER_MAIN_CALC_ACL		, (ke_msg_func_t)main_calc_acl },
	{ USER_MAIN_ACT2			, (ke_msg_func_t)cpu_com_timer_handler },
};

/* Status Handler */
const struct ke_state_handler cpu_com_state_handler[ CPU_COM_STATE_MAX ] =
{   /* State message handlers */
    KE_STATE_HANDLER(cpu_com_handler)
};

/* Default Handler */
const struct ke_state_handler cpu_com_default_handler = KE_STATE_HANDLER_NONE;


















/************************************************************/
/* ユーザー定義												*/
/************************************************************/
/* バージョン表記の注意事項 */
const B		version_product_tbl[]= {0, 0, 0, 6};				/* ソフトウェアバージョン */
																/* バージョン表記ルール */
																/* ①メジャーバージョン：[0 ～ 99] */
																/* ②マイナーバージョン：[0 ～ 9] */
																/* ③リビジョン：[0 ～ 99] */
																/* ④ビルドバージョン：[0 ～ 99] */


/* 受信データ処理 関数テーブル */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* コマンド */		/* 関数  */					/* 応答有無 */
	{	0x00,			NULL,								OFF	},	/* 【CPU間通信コマンド】コマンド無し				*/
	{	0xE0,			main_cpu_com_rcv_sts_res,			OFF	},	/* 【CPU間通信コマンド】ステータス要求				*/
	{	0xA0,			main_cpu_com_rcv_sensor_res,		OFF	},	/* 【CPU間通信コマンド】センサーデータ更新			*/
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
};


/* モード別処理 */
STATIC void	(* const p_user_main_mode_func[])()			= {					user_main_mode_inital,			// SYSTEM_MODE_INITAL			イニシャル
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


