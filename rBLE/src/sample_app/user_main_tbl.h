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
#if FUNC_DEBUG_LOG != ON
	{ USER_MAIN_CYC_ACT			, (ke_msg_func_t)user_main_cyc },
	{ USER_MAIN_CALC_KOKYU		, (ke_msg_func_t)main_calc_kokyu },
	{ USER_MAIN_CALC_IBIKI		, (ke_msg_func_t)main_calc_ibiki },
#endif
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
const B		version_product_tbl[]= {0, 0, 1,14};				/* ソフトウェアバージョン */
																/* バージョン表記ルール */
																/* �@メジャーバージョン：[0 〜 99] */
																/* �Aマイナーバージョン：[0 〜 9] */
																/* �Bリビジョン：[0 〜 99] */
																/* �Cビルドバージョン：[0 〜 99] */

/* VUART通信受信データ処理 関数テーブル */
STATIC const VUART_RCV_CMD_TBL s_vuart_rcv_func_tbl[VUART_CMD_TYPE_MAX] = {
	/* コマンド */					/* レングス */				/* 関数  */					/* 応答有無 */
	{	0x00,						0,							NULL							},	// なし
	{	VUART_CMD_MODE_CHG,			VUART_CMD_LEN_MODE_CHG,		main_vuart_rcv_mode_chg			},	// 状態変更(G1D)
	{	VUART_CMD_DATE_SET,			VUART_CMD_LEN_DATE_SET,		main_vuart_rcv_date				},	// 日時設定
	{	VUART_CMD_INFO,				VUART_CMD_LEN_INFO,			main_vuart_rcv_info				},	// 情報取得
	{	VUART_CMD_VERSION,			VUART_CMD_LEN_VERSION,		main_vuart_rcv_version			},	// バージョン取得
	{	VUART_CMD_DEVICE_INFO,		VUART_CMD_LEN_DEVICE_INFO,	main_vuart_rcv_device_info		},	// デバイス状況取得
	{	VUART_CMD_DATA_NEXT,		VUART_CMD_LEN_DATA_NEXT,	NULL							},	// NEXT[送信専用]
	{	VUART_CMD_DATA_END,			VUART_CMD_LEN_DATA_END,		main_vuart_rcv_data_end			},	// END[受信はSET時]
	{	VUART_CMD_DATA_FRAME,		VUART_CMD_LEN_DATA_FRAME,	main_vuart_rcv_data_frame		},	// 枠情報(日時等)[受信はSET時]
	{	VUART_CMD_DATA_CALC,		VUART_CMD_LEN_DATA_CALC,	main_vuart_rcv_data_calc		},	// 機器データ[受信はSET時]
	{	VUART_CMD_DATA_FIN,			VUART_CMD_LEN_DATA_FIN,		main_vuart_rcv_data_fin			},	// データ取得完了通知
	{	VUART_CMD_DEVICE_SET,		VUART_CMD_LEN_DEVICE_SET,	main_vuart_rcv_device_set		},	// デバイス設定変更
	{	VUART_CMD_TYPE_VIB_CONFIRM, VUART_CMD_LEN_VIB_CONFIRM,	main_vuart_rcv_vib_confirm		},	// バイブ動作確認
};

/* モード別処理 */
typedef void (*MODE_FUNC)(void);
STATIC MODE_FUNC p_user_main_mode_func[] = {								user_main_mode_inital,			// SYSTEM_MODE_INITIAL			イニシャル
																			user_main_mode_idle_com,		// SYSTEM_MODE_IDLE_COM			アイドル_通信待機
																			user_main_mode_sensing,			// SYSTEM_MODE_SENSING			センシング
																			user_main_mode_get,				// SYSTEM_MODE_GET				データ取得
																			user_main_mode_prg_g1d,			// SYSTEM_MODE_PRG_G1D			G1Dプログラム更新
																			user_main_mode_self_check,		// SYSTEM_MODE_PRG_SELF_CHECK	自己診断
																			user_main_mode_move,			// SYSTEM_MODE_MOVE				移行
																			dummy,							// SYSTEM_MODE_NON				なし
};

typedef SYSTEM_MODE ( *EVENT_TABLE )( int event );
STATIC EVENT_TABLE p_event_table[ EVENT_MAX ][ SYSTEM_MODE_MAX ] = {
// モード				INITAL				IDLE_COM			SENSING			GET					PRG_G1D			SELF_CHECK		MOVE				NON
/*イベントなし		*/	{ evt_non,			evt_non,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*電源SW(短)		*/	{ evt_non,			evt_bat_check,		evt_bat_check,	evt_bat_check,		evt_non,		evt_non,		evt_non,			evt_non },
/*電源SW押下(長)	*/	{ evt_non,			evt_sensing_chg,	evt_idle_com,	evt_sensing_chg,	evt_non,		evt_non,		evt_non,			evt_non },
/*充電検知ポートON	*/	{ evt_idle_com,		evt_non,			evt_idle_com,	evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*充電検知ポートOFF	*/	{ evt_non,			evt_non,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*電池残量低下		*/	{ evt_non,			evt_non,			evt_idle_com,	evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*充電完了			*/	{ evt_non,			evt_non,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*データ取得		*/	{ evt_non,			evt_get,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*プログラム(G1D)	*/	{ evt_non,			evt_g1d_prg_denchi,	evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*自己診断(通信)	*/	{ evt_non,			evt_self_check,		evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*完了				*/	{ evt_non,			evt_non,			evt_idle_com,	evt_idle_com,		evt_idle_com,	evt_idle_com,	evt_non,			evt_non },
/*中断				*/	{ evt_non,			evt_non,			evt_idle_com,	evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*タイムアウト		*/	{ evt_non,			evt_non,			evt_non,		evt_time_out,		evt_non,		evt_non,		evt_non,			evt_non },
/*検査				*/	{ evt_idle_com_denchi,evt_self_check,	evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*取り外れﾀｲﾑｱｳﾄ	*/	{ evt_non,			evt_non,			evt_remove,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
};

// 1ページ256byteの０埋めテーブル
STATIC const UB s_eep_page0_tbl[EEP_ACCESS_ONCE_SIZE] = { 0 };

