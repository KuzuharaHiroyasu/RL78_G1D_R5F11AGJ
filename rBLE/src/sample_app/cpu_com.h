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
#define CPU_COM_SND_DATA_SIZE_PC_LOG				7				/* PCログ送信(内部コマンド)	*/
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			11				/* センサーデータサイズ */

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

/* CPU間通信イベントトリガID */
typedef enum _CPU_COM_EVENT_TRIGGER_ID {
	CPU_COM_EVENT_TRG_ID_NONE					=0	,	/* イベント無し */
	CPU_COM_EVENT_TRG_ID_NORMAL						,	/* [通常表示]通常表示 */
	CPU_COM_EVENT_TRG_ID_ATARI						,	/* [通常表示]大当り開始 */
	CPU_COM_EVENT_TRG_ID_KAKUHEN					,	/* [通常表示]確変開始 */
	CPU_COM_EVENT_TRG_ID_BB							,	/* [通常表示]BB開始 */
	CPU_COM_EVENT_TRG_ID_RB							,	/* [通常表示]RB開始 */
	CPU_COM_EVENT_TRG_ID_ART						,	/* [通常表示]ART開始 */
	CPU_COM_EVENT_TRG_ID_ATARI_END					,	/* [通常表示]大当り終了 */
	CPU_COM_EVENT_TRG_ID_KAKUHEN_END				,	/* [通常表示]確変終了 */
	CPU_COM_EVENT_TRG_ID_BB_END						,	/* [通常表示]BB終了 */
	CPU_COM_EVENT_TRG_ID_RB_END						,	/* [通常表示]RB終了 */
	CPU_COM_EVENT_TRG_ID_ART_END					,	/* [通常表示]ART終了 */
	CPU_COM_EVENT_TRG_ID_HIGH_DATA					,	/* [通常表示]過去最高データ */
	CPU_COM_EVENT_TRG_ID_DAY_BEFORE_DATA			,	/* [通常表示]過去データ */
	CPU_COM_EVENT_TRG_ID_HIST_DATA					,	/* [通常表示]履歴データ */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_HIST_DATA_NORMAL			,	/* [通常表示]履歴データ(通常タイプ) */ /* 通常⇔スランプグラフタイプ切替用 */
	CPU_COM_EVENT_TRG_ID_HIST_DATA_SLUMP			,	/* [通常表示]履歴データ(スランプグラフタイプ) */ /* 通常⇔スランプグラフタイプ切替用 */
	CPU_COM_EVENT_TRG_ID_GRAPH_DATA					,	/* [通常表示]スランプグラフ */
	CPU_COM_EVENT_TRG_ID_RANKING_DATA				,	/* [通常表示]ランキングデータ */
	CPU_COM_EVENT_TRG_ID_OTHER_DAI_DATA				,	/* [通常表示]周辺台データ */
	CPU_COM_EVENT_TRG_ID_KISYU_SPEC_DATA			,	/* [通常表示]機種スペック */
	CPU_COM_EVENT_TRG_ID_OSHIRASE_DATA				,	/* [通常表示]お知らせ */
	CPU_COM_EVENT_TRG_ID_ZONE_DATA					,	/* [通常表示]ゾーンデータ */
	CPU_COM_EVENT_TRG_ID_CALENDER_DATA				,	/* [通常表示]カレンダー */
	CPU_COM_EVENT_TRG_ID_PRIVATE_DATA				,	/* [通常表示]プライベートデータ */
	CPU_COM_EVENT_TRG_ID_ONETOUCH_MENU				,	/* [通常表示]ワンタッチメニュー */
	CPU_COM_EVENT_TRG_ID_ONETOUCH_ORDER				,	/* [通常表示]ワンタッチ注文画面 */
	CPU_COM_EVENT_TRG_ID_SADAMA_SAMAI				,	/* [通常表示]差玉・差枚表示 */
	CPU_COM_EVENT_TRG_ID_TEST_PLAY					,	/* [通常表示]テスト打ちモード */
	CPU_COM_EVENT_TRG_ID_NORMAL_RETURN				,	/* [通常表示]通常表示に戻る */
	CPU_COM_EVENT_TRG_ID_TROUBLE					,	/* [トラブル]トラブル */
	CPU_COM_EVENT_TRG_ID_TROUBLE_TO					,	/* [トラブル]トラブルタイムオーバー */
	CPU_COM_EVENT_TRG_ID_SERVICE1					,	/* [サービス]サービス1 */
	CPU_COM_EVENT_TRG_ID_SERVICE2					,	/* [サービス]サービス2 */
	CPU_COM_EVENT_TRG_ID_LUNCH						,	/* [ランチ]ランチ */
	CPU_COM_EVENT_TRG_ID_UNIT_LUNCH					,	/* [ランチ]ユニットランチ */
	CPU_COM_EVENT_TRG_ID_LUNCH_TO					,	/* [ランチ]ランチタイムオーバー */
	CPU_COM_EVENT_TRG_ID_UNIT_LUNCH_TO				,	/* [ランチ]ユニットランチタイムオーバー */
	CPU_COM_EVENT_TRG_ID_DATA_RANKING_CHG_ATARI		,	/* [データ画面]ランキング大当り */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_DATA_RANKING_CHG_KEIZOKU	,	/* [データ画面]ランキング継続 */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_DATA_RANKING_CHG_KAKUTOKU	,	/* [データ画面]ランキング獲得 */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_DATA_CLR_READY				,	/* [データ操作]データクリア準備 */
	CPU_COM_EVENT_TRG_ID_DATA_CLR_RUN				,	/* [データ操作]データクリア実行 */
	CPU_COM_EVENT_TRG_ID_DATA_ALL_CLR_READY			,	/* [データ操作]データオールクリア準備 */
	CPU_COM_EVENT_TRG_ID_DATA_ALL_CLR_RUN			,	/* [データ操作]データオールクリア実行 */
	CPU_COM_EVENT_TRG_ID_FEED_DAY_READY				,	/* [データ操作]日送り準備 */
	CPU_COM_EVENT_TRG_ID_FEED_DAY_RUN				,	/* [データ操作]日送り実行 */
	CPU_COM_EVENT_TRG_ID_RETURN_DAY_READY			,	/* [データ操作]日戻し準備 */
	CPU_COM_EVENT_TRG_ID_RETURN_DAY_RUN				,	/* [データ操作]日戻し実行 */
	CPU_COM_EVENT_TRG_ID_NIGHT_READY				,	/* [ショートカット]夜間監視準備 */
	CPU_COM_EVENT_TRG_ID_BONUS_START_CLR			,	/* [ショートカット]ボーナス間スタート回数クリア */
	CPU_COM_EVENT_TRG_ID_BONUS_START_CLR_LINE		,	/* [ショートカット]一列ボーナス間スタート回数クリア */
	CPU_COM_EVENT_TRG_ID_HIGH_KEEP_DATA_CLR			,	/* [ショートカット]過去最高以外データクリア */
	CPU_COM_EVENT_TRG_ID_GRAPH_DATA_CLR				,	/* [ショートカット]グラフデータクリア */
	CPU_COM_EVENT_TRG_ID_DEMO_MODE					,	/* [ショートカット]デモモード移行 */
	CPU_COM_EVENT_TRG_ID_ART_SET					,	/* [ショートカット]ART設定 */
	CPU_COM_EVENT_TRG_ID_DATA_SET_MODE				,	/* [データ打ち込みモード]データ打ち込みモード移行 */
	CPU_COM_EVENT_TRG_ID_DATA_SET_HIGH				,	/* [データ打ち込みモード]過去最高データ */
	CPU_COM_EVENT_TRG_ID_DATA_SET_DAY				,	/* [データ打ち込みモード]過去データ */
	CPU_COM_EVENT_TRG_ID_ERR_TROUBLE_MODE			,	/* [エラー/トラブル表示]エラー/トラブル表示移行 */
	CPU_COM_EVENT_TRG_ID_ALL_ON						,	/* [全灯モード]全灯モード */
	CPU_COM_EVENT_TRG_ID_VERSION					,	/* [バージョン表示]バージョン表示 */
	CPU_COM_EVENT_TRG_ID_NEWS_CHK_MODE				,	/* [ニュース内容確認モード]ニュース内容確認モード移行 */
	CPU_COM_EVENT_TRG_ID_ADDR_CLR					,	/* [アドレス設定]アドレス情報初期化 */
	CPU_COM_EVENT_TRG_ID_ADDR_SET_HOST				,	/* [アドレス設定]アドレス設定(ホスト) */
	CPU_COM_EVENT_TRG_ID_ADDR_SET_SLV				,	/* [アドレス設定]アドレス設定(スレーブ) */
	CPU_COM_EVENT_TRG_ID_ADDR_SET_HOST_MANU			,	/* [アドレス設定]手動アドレス取得(ホスト) */
	CPU_COM_EVENT_TRG_ID_ADDR_SET_SLV_MANU			,	/* [アドレス設定]手動アドレス取得(スレーブ) */
	CPU_COM_EVENT_TRG_ID_LAMP_CHK					,	/* [ランプ動作確認モード]ランプ動作確認モード遷移 */
	CPU_COM_EVENT_TRG_ID_LAMP_CHK_CHG				,	/* [ランプ動作確認モード]ランプ動作駆動切り替え */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_INPUT			,	/* [初期設定モード]初期設定入力モード移行 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_ALONE_SEL		,	/* [初期設定モード]個別設定受信選択 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_ALONE_ON			,	/* [初期設定モード]個別設定受信有効 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_ALONE_RCV		,	/* [初期設定モード]設定受信 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_INPUT_VALUE		,	/* [初期設定モード]任意設定値入力モード */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ4_INPUT		,	/* [初期設定モード]セキュリティID(4桁)入力 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ4_INPUT_RE	,	/* [初期設定モード]セキュリティID(4桁)入力(2回目) */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ6_INPUT		,	/* [初期設定モード]セキュリティID(6桁)入力 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ6_INPUT_RE	,	/* [初期設定モード]セキュリティID(6桁)入力(2回目) */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_INPUT_ERR		,	/* [初期設定モード]入力エラー */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEND_READY		,	/* [初期設定モード]送信モード */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_CHK				,	/* [初期設定モード]確認モード */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_CLEAR			,	/* [初期設定モード]初期設定オールクリア */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_CLEAR_RUN		,	/* [初期設定モード]初期設定オールクリア実行 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_VERSION			,	/* [初期設定モード]バージョン表示 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ_CHK			,	/* [初期設定モード]セキュリティID確認 */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_RUN				,	/* [初期設定モード]実行 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_ALONE_SEND		,	/* [初期設定モード]単体初期設定送信 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_MAINTE_1			,	/* [初期設定モード]メンテナンスモード1 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_MAINTE_2			,	/* [初期設定モード]メンテナンスモード2 */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_RETURN			,	/* [初期設定モード]入力モードに戻る */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_PASS				,	/* [自己診断モード]パスワード入力画面移行 */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_START				,	/* [自己診断モード]起動画面移行 */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_NEXT				,	/* [自己診断モード]次の画面 */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_ALL_ON			,	/* [自己診断モード]点灯チェック(全点灯) */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_ALL_RED			,	/* [自己診断モード]点灯チェック(赤点灯) */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_ALL_BLUE			,	/* [自己診断モード]点灯チェック(青点灯) */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_ALL_GREEN			,	/* [自己診断モード]点灯チェック(緑点灯) */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_SW_CHECK			,	/* [自己診断モード]SW入力チェック */
	CPU_COM_EVENT_TRG_ID_PRG_DL_PASS				,	/* [プログラム転送]パスワード入力画面移行 */
	CPU_COM_EVENT_TRG_ID_PRG_DL_BOOT_CHG			,	/* [プログラム転送]書換開始画面移行 */
	CPU_COM_EVENT_TRG_ID_PRG_DL_ERASE				,	/* [プログラム転送]イレース中画面移行 */
	CPU_COM_EVENT_TRG_ID_PRG_DL_TRANS_RUN			,	/* [プログラム転送]転送中画面移行 */
	CPU_COM_EVENT_TRG_ID_PRG_DL_TRANS_END			,	/* [プログラム転送]転送完了画面移行 */
	CPU_COM_EVENT_TRG_ID_PRG_DL_CHECK_SUM			,	/* [プログラム転送]チェックサム照合画面移行 */
	CPU_COM_EVENT_TRG_ID_PRG_DL_NG					,	/* [プログラム転送]書換失敗画面移行 */
	CPU_COM_EVENT_TRG_ID_IMAGE_SEND_START			,	/* [画像転送]画像転送開始 */				/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_MODE				,	/* [SD画像書換]SD画像書換画面移行 */		/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_RUN				,	/* [SD画像転送]画像転送開始 */				/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_ERR				,	/* [SD画像転送]アクセスエラー */			/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_MAINTE_MODE				,	/* [SD画像転送]メンテナンスモード */		/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_OK				,	/* [SD画像転送]画像書換完了 */				/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_NG				,	/* [SD画像転送]画像書換失敗 */				/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_PRG_DL					,	/* [SDメンテナンス]プログラム転送 */		/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_DEF_IMAGE				,	/* [SDメンテナンス]デフォルト画像書換 */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_SH_PRG_DL				,	/* [SDメンテナンス]SHプログラム転送 */		/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_MAINTE_ERR				,	/* [SDメンテナンス]アクセスエラー */		/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE				,	/* [SDデータ移行]SDデータ移行モード */		/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_CHG			,	/* [SDデータ移行]SDデータ移行表示切り替え */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_DUMMY			,	/* [SDデータ移行]SDデータ移行ダミー ※ 未使用トリガ */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_DATA			,	/* [SDデータ移行]データ */					/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_PICT			,	/* [SDデータ移行]画像 */					/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_ALL			,	/* [SDデータ移行]全て */					/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_MODE_MENU		,	/* [USBメンテナンス]メモリカードメンテナンスモードメニュー画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_RX_RUN	,	/* [USBメンテナンス]プログラム書換モード(RX)　書き換え中画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_RX_ERR	,	/* [USBメンテナンス]プログラム書換モード(RX)　アクセス失敗画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_RX_OK	,	/* [USBメンテナンス]プログラム書換モード(RX)　書換完了画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_RX_NG	,	/* [USBメンテナンス]プログラム書換モード(RX)　書換失敗画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_AN_RUN	,	/* [USBメンテナンス]プログラム書換モード(android)　書き換え中画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_AN_ERR	,	/* [USBメンテナンス]プログラム書換モード(android)　アクセス失敗画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_AN_OK	,	/* [USBメンテナンス]プログラム書換モード(android)　書換完了画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_AN_NG	,	/* [USBメンテナンス]プログラム書換モード(android)　書換失敗画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_MENU	,	/* [USBメンテナンス]画像転送モード　画像選択画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_DISP	,	/* [USBメンテナンス]画像転送モード　選択画像確認画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_RUN	,	/* [USBメンテナンス]画像転送モード　画像書き換え中画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_ERR	,	/* [USBメンテナンス]画像転送モード　画像書き換えアクセス失敗画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_OK		,	/* [USBメンテナンス]画像転送モード　画像書き換え完了画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_NG		,	/* [USBメンテナンス]画像転送モード　画像書き換え失敗画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_ONEMEMO_MENU	,	/* [USBメンテナンス]ワンメモモード　メニュー画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_ONEMEMO_READ	,	/* [USBメンテナンス]ワンメモモード　読み込み画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_ONEMEMO_WRITE	,	/* [USBメンテナンス]ワンメモモード　書き換え画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_BACKUP_MENU		,	/* [USBメンテナンス]バックアップモード　メニュー画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_BACKUP_READ		,	/* [USBメンテナンス]バックアップモード　バックアップ画面 */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_BACKUP_WRITE	,	/* [USBメンテナンス]バックアップモード　リカバリ画面 */
	CPU_COM_EVENT_TRG_ID_HARD_CHK_MODE				,	/* [ハード検査モード]ハード検査起動画面 */		/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_HARD_CHK_IOBOX				,	/* [ハード検査モード]入出力BOX検査 */			/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_HARD_CHK_FUNC				,	/* [ハード検査モード]ファンクション検査 */		/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_MODE				,	/* [メモリ移行モード]メモリ移行モード移行 */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_READ				,	/* [メモリ移行モード]読み出し */				/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_WRITE				,	/* [メモリ移行モード]書き込み */				/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_BACK				,	/* [メモリ移行モード]バックアップ書き込み */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_CLEAR				,	/* [メモリ移行モード]クリア */					/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_ALL_ON_ON					,	/* [全灯モード]全灯モード点灯 */
	CPU_COM_EVENT_TRG_ID_ALL_ON_OFF					,	/* [全灯モード]全灯モード消灯 */
	CPU_COM_EVENT_TRG_ID_MAIN_MENU_ON				,	/* [共通]メインメニューON */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_MAIN_MENU_OFF				,	/* [共通]メインメニューOFF */	/* 未使用 削除せず残す */
	CPU_COM_EVENT_TRG_ID_MACHINE_SET_HOST			,	/* [台番号設定]台番号設定(ホスト) */
	CPU_COM_EVENT_TRG_ID_MACHINE_SET_SLV			,	/* [台番号設定]台番号設定(スレーブ) */
	CPU_COM_EVENT_TRG_ID_STARTUP_FIN				,	/* [起動通知]起動完了通知 */
	CPU_COM_EVENT_TRG_ID_MAX							/* 最大値 */
}CPU_COM_EVENT_TRIGGER_ID;

/* CPU間通信表示データ更新トリガID */
typedef enum _CPU_COM_DATA_TRIGGER_ID {
	CPU_COM_DATA_TRG_ID_NONE=0,					/* データ更新無し */
	CPU_COM_DATA_TRG_ID_COM_ADDR,				/* 通信アドレス */
	CPU_COM_DATA_TRG_ID_PARA_A,					/* 初期設定A項目 */
	CPU_COM_DATA_TRG_ID_PARA_B,					/* 初期設定B項目 */
	CPU_COM_DATA_TRG_ID_PARA_C,					/* 初期設定C項目 */
	CPU_COM_DATA_TRG_ID_PARA_D,					/* 初期設定D項目 */
	CPU_COM_DATA_TRG_ID_PARA_F,					/* 初期設定F項目 */
	CPU_COM_DATA_TRG_ID_PARA_H,					/* 初期設定H項目 */
	CPU_COM_DATA_TRG_ID_PARA_J,					/* 初期設定J項目 */
	CPU_COM_DATA_TRG_ID_PARA_L,					/* 初期設定L項目 */
	CPU_COM_DATA_TRG_ID_PARA_P,					/* 初期設定P項目 */
	CPU_COM_DATA_TRG_ID_PARA_S,					/* 初期設定S項目 */
	CPU_COM_DATA_TRG_ID_PARA_OPT,				/* 初期設定任意設定値 */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_00,			/* スランプグラフ0日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_00,		/* スランプグラフ0日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_01,			/* スランプグラフ1日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_01,		/* スランプグラフ1日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_02,			/* スランプグラフ2日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_02,		/* スランプグラフ2日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_03,			/* スランプグラフ3日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_03,		/* スランプグラフ3日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_04,			/* スランプグラフ4日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_04,		/* スランプグラフ4日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_05,			/* スランプグラフ5日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_05,		/* スランプグラフ5日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_06,			/* スランプグラフ6日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_06,		/* スランプグラフ6日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_07,			/* スランプグラフ7日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_07,		/* スランプグラフ7日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_08,			/* スランプグラフ8日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_08,		/* スランプグラフ8日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_09,			/* スランプグラフ9日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_09,		/* スランプグラフ9日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_10,			/* スランプグラフ10日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_10,		/* スランプグラフ10日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_11,			/* スランプグラフ11日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_11,		/* スランプグラフ11日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_12,			/* スランプグラフ12日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_12,		/* スランプグラフ12日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_13,			/* スランプグラフ13日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_13,		/* スランプグラフ13日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_14,			/* スランプグラフ14日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_14,		/* スランプグラフ14日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_15,			/* スランプグラフ15日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_15,		/* スランプグラフ15日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_16,			/* スランプグラフ16日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_16,		/* スランプグラフ16日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_17,			/* スランプグラフ17日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_17,		/* スランプグラフ17日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_18,			/* スランプグラフ18日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_18,		/* スランプグラフ18日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_19,			/* スランプグラフ19日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_19,		/* スランプグラフ19日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_20,			/* スランプグラフ20日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_20,		/* スランプグラフ20日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_21,			/* スランプグラフ21日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_21,		/* スランプグラフ21日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_22,			/* スランプグラフ22日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_22,		/* スランプグラフ22日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_23,			/* スランプグラフ23日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_23,		/* スランプグラフ23日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_24,			/* スランプグラフ24日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_24,		/* スランプグラフ24日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_25,			/* スランプグラフ25日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_25,		/* スランプグラフ25日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_26,			/* スランプグラフ26日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_26,		/* スランプグラフ26日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_27,			/* スランプグラフ27日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_27,		/* スランプグラフ27日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_28,			/* スランプグラフ28日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_28,		/* スランプグラフ28日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_29,			/* スランプグラフ29日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_29,		/* スランプグラフ29日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_30,			/* スランプグラフ30日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_30,		/* スランプグラフ30日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_31,			/* スランプグラフ31日前プロット及びスケール */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_31,		/* スランプグラフ31日前情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_GRAPH_COLOR_00,			/* スランプグラフ色情報 0日前 */
	CPU_COM_DATA_TRG_ID_GRAPH_COLOR_01,			/* スランプグラフ色情報 1日前 */
	CPU_COM_DATA_TRG_ID_GRAPH_COLOR_02,			/* スランプグラフ色情報 2日前 */
	CPU_COM_DATA_TRG_ID_GRAPH_COLOR_03,			/* スランプグラフ色情報 3日前 */
	CPU_COM_DATA_TRG_ID_HIGH_GRAPH_COLOR,		/* 最高スランプグラフ色情報 */
	CPU_COM_DATA_TRG_ID_GRAPH_HIST_00,			/* スランプグラフ履歴データ対応情報 0日前 */
	CPU_COM_DATA_TRG_ID_GRAPH_HIST_01,			/* スランプグラフ履歴データ対応情報 1日前 */
	CPU_COM_DATA_TRG_ID_GRAPH_HIST_02,			/* スランプグラフ履歴データ対応情報 2日前 */
	CPU_COM_DATA_TRG_ID_GRAPH_HIST_03,			/* スランプグラフ履歴データ対応情報 3日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_00,			/* 過去データ0日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_01,			/* 過去データ1日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_02,			/* 過去データ2日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_03,			/* 過去データ3日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_04,			/* 過去データ4日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_05,			/* 過去データ5日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_06,			/* 過去データ6日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_07,			/* 過去データ7日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_08,			/* 過去データ8日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_09,			/* 過去データ9日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_10,			/* 過去データ10日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_11,			/* 過去データ11日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_12,			/* 過去データ12日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_13,			/* 過去データ13日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_14,			/* 過去データ14日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_15,			/* 過去データ15日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_16,			/* 過去データ16日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_17,			/* 過去データ17日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_18,			/* 過去データ18日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_19,			/* 過去データ19日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_20,			/* 過去データ20日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_21,			/* 過去データ21日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_22,			/* 過去データ22日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_23,			/* 過去データ23日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_24,			/* 過去データ24日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_25,			/* 過去データ25日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_26,			/* 過去データ26日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_27,			/* 過去データ27日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_28,			/* 過去データ28日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_29,			/* 過去データ29日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_30,			/* 過去データ30日前 */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_31,			/* 過去データ31日前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_01,			/* 履歴データ1回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_02,			/* 履歴データ2回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_03,			/* 履歴データ3回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_04,			/* 履歴データ4回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_05,			/* 履歴データ5回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_06,			/* 履歴データ6回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_07,			/* 履歴データ7回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_08,			/* 履歴データ8回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_09,			/* 履歴データ9回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_10,			/* 履歴データ10回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_11,			/* 履歴データ11回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_12,			/* 履歴データ12回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_13,			/* 履歴データ13回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_14,			/* 履歴データ14回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_15,			/* 履歴データ15回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_16,			/* 履歴データ16回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_17,			/* 履歴データ17回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_18,			/* 履歴データ18回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_19,			/* 履歴データ19回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_20,			/* 履歴データ20回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_21,			/* 履歴データ21回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_22,			/* 履歴データ22回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_23,			/* 履歴データ23回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_24,			/* 履歴データ24回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_25,			/* 履歴データ25回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_26,			/* 履歴データ26回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_27,			/* 履歴データ27回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_28,			/* 履歴データ28回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_29,			/* 履歴データ29回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_30,			/* 履歴データ30回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_31,			/* 履歴データ31回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_32,			/* 履歴データ32回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_33,			/* 履歴データ33回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_34,			/* 履歴データ34回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_35,			/* 履歴データ35回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_36,			/* 履歴データ36回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_37,			/* 履歴データ37回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_38,			/* 履歴データ38回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_39,			/* 履歴データ39回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_40,			/* 履歴データ40回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_41,			/* 履歴データ41回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_42,			/* 履歴データ42回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_43,			/* 履歴データ43回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_44,			/* 履歴データ44回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_45,			/* 履歴データ45回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_46,			/* 履歴データ46回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_47,			/* 履歴データ47回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_48,			/* 履歴データ48回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_49,			/* 履歴データ49回前 */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_50,			/* 履歴データ50回前 */
	CPU_COM_DATA_TRG_ID_HIGH_DATA,				/* 過去最高データ */
	CPU_COM_DATA_TRG_ID_HIGH_DATA_GRAPH_PROT,	/* 過去最高グラフプロット及びスケール */
	CPU_COM_DATA_TRG_ID_HIGH_DATA_GRAPH_TAMA_MAI,	/* 過去最高グラフ情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_DAY_DATA_00,			/* 当日データ日戻しブロック 0日前 */
	CPU_COM_DATA_TRG_ID_DAY_DATA_01,			/* 当日データ日戻しブロック 1日前 */
	CPU_COM_DATA_TRG_ID_DAY_DATA_02,			/* 当日データ日戻しブロック 2日前 */
	CPU_COM_DATA_TRG_ID_TODAY_DATA,				/* 当日データ */
	CPU_COM_DATA_TRG_ID_MARS_OUT,				/* マース用総アウト */
	CPU_COM_DATA_TRG_ID_MARS_SAFE,				/* マース用総セーフ */
	CPU_COM_DATA_TRG_ID_PRIVATE,				/* プライベートデータ */
	CPU_COM_DATA_TRG_ID_PRIVATE_GRAPH_PROT,		/* プライベートデータグラフプロット及びスケール */
	CPU_COM_DATA_TRG_ID_PRIVATE_GRAPH_TAMA_MAI,	/* プライベートデータグラフ情報(玉数/枚数) */
	CPU_COM_DATA_TRG_ID_ZONE,					/* ゾーンデータ */
	CPU_COM_DATA_TRG_ID_CALENDER,				/* カレンダー */
	CPU_COM_DATA_TRG_ID_POWER_FAIL,				/* 瞬停状態 */
	CPU_COM_DATA_TRG_ID_STATUS,					/* 状態データ */
	CPU_COM_DATA_TRG_ID_ERR_EQP,				/* 通信異常 */
	CPU_COM_DATA_TRG_ID_ERR_CNT,				/* エラー回数 */
	CPU_COM_DATA_TRG_ID_ERR_SAND,				/* サンド通信エラー */
	CPU_COM_DATA_TRG_ID_MAINTE_RELATION,		/* メンテナンス関係 */
	CPU_COM_DATA_TRG_ID_ERR_IOBOX,				/* 入出力BOX通信エラー */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_01,			/* ニュース1情報データ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_02,			/* ニュース2情報データ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_03,			/* ニュース3情報データ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_04,			/* ニュース4情報データ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_05,			/* ニュース5情報データ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_06,			/* ニュース6情報データ */
	CPU_COM_DATA_TRG_ID_COM_LINE,				/* 島端接続台数 */
	CPU_COM_DATA_TRG_ID_VERSION_INFO,			/* バージョン情報 */
	CPU_COM_DATA_TRG_ID_TROUBLE_INFO,			/* トラブル情報 */
	CPU_COM_DATA_TRG_ID_LUNCH_INFO,				/* ランチ情報 */
	CPU_COM_DATA_TRG_ID_AWAY_STATUS,			/* 離席状態 */
	CPU_COM_DATA_TRG_ID_KUUSEKI_STATUS,			/* 空席状態 */
	CPU_COM_DATA_TRG_ID_ONETOUCH_STATUS,		/* ワンタッチ状態 */
	CPU_COM_DATA_TRG_ID_EVENT_STATUS,			/* イベント情報 */
	CPU_COM_DATA_TRG_ID_RANKING_ATARI,			/* ランキング(大当り)情報 */
	CPU_COM_DATA_TRG_ID_RANKING_KEIZOKU,		/* ランキング(継続)情報 */
	CPU_COM_DATA_TRG_ID_RANKING_KAKUTOKU,		/* ランキング(獲得)情報 */
	CPU_COM_DATA_TRG_ID_OTHER_DAI,				/* 周辺台データ */
	CPU_COM_DATA_TRG_ID_OTHER_DAI_DETAIL,		/* 周辺詳細データ */
	CPU_COM_DATA_TRG_ID_OTHER_DAI_B_BLOCK,		/* 周辺前後データ */
	CPU_COM_DATA_TRG_ID_NEWS_INFO,				/* ニュース配信情報 */
	CPU_COM_DATA_TRG_ID_CUT_IN_INFO,			/* カットイン情報 */
	CPU_COM_DATA_TRG_ID_LABEL_INFO,				/* 札差表示情報 */
	CPU_COM_DATA_TRG_ID_SAND_INFO,				/* サンド通信情報 */
	CPU_COM_DATA_TRG_ID_INFORMATION,			/* インフォメーション配信情報 */
	CPU_COM_DATA_TRG_ID_KADOU_STATUS,			/* ○稼働ステータス */
	CPU_COM_DATA_TRG_ID_INIT_CONF_DATA,			/* 初期設定モード一時データ */
	CPU_COM_DATA_TRG_ID_DATA_SET_DATA,			/* ○データ打ち込みモード一時データ */
	CPU_COM_DATA_TRG_ID_ADDR_GET_DATA,			/* ○アドレス取得モード一時データ */
	CPU_COM_DATA_TRG_ID_LAMP_CHK_DATA,			/* ランプ動作確認モード一時データ */
	CPU_COM_DATA_TRG_ID_MEM_MOVE_DATA,			/* メモリ移行モード一時データ */
	CPU_COM_DATA_TRG_ID_SD_IMAGE_DATA,			/* SD画像転送一時データ */
	CPU_COM_DATA_TRG_ID_NEWS_CHK_DATA,			/* ニュース内容確認モード一時データ */
	CPU_COM_DATA_TRG_ID_DATA_ORDER,				/* データ表示指示情報 */
	CPU_COM_DATA_TRG_ID_MAINTE_STATUS,			/* メンテナンス用表示情報 */
	CPU_COM_DATA_TRG_ID_GRAPH_STATE,			/* グラフ状態表示情報 */
	CPU_COM_DATA_TRG_ID_MACHINE_SET_DATA,		/* 台番号設定モード一時データ */
	CPU_COM_DATA_TRG_ID_DATE_INFO,				/* 日付情報(時刻含む) */
	CPU_COM_DATA_TRG_ID_MAX						/* 最大値 */
	/* データを追加する際は以下のテーブルを合わせて変更する事 */
	/* ・cpu_com_ram_data_update[CPU_COM_RAM_UPDATE_CHK_MAX] */
	/* ・cpu_com_data_data_table[CPU_COM_DATA_TRG_ID_MAX] */
}CPU_COM_DATA_TRIGGER_ID;

/* CPU間通信送信要求トリガID */
/* 汎用処理関連 ※特別なものは除く */
/* 複数要求が発生するものは除く 例)表示データ更新、イベント通知、音声再生等 */
/* 特別は処理をしているものは除く 例)プログラム転送等 */
typedef enum _CPU_COM_REQ_TRIGGER_ID {
	CPU_COM_REQ_TRG_ID_NONE=0,					/* 【送信要求トリガID】要求無し */
	CPU_COM_REQ_TRG_ID_STRING_WRITE,			/* 【送信要求トリガID】文字列書き込み */
	CPU_COM_REQ_TRG_ID_STRING_READ,				/* 【送信要求トリガID】文字列読み出し */
	CPU_COM_REQ_TRG_ID_FILE_READ,				/* 【送信要求トリガID】ファイル読み出し */
//	CPU_COM_REQ_TRG_ID_SOUND_REQ,				/* 【送信要求トリガID】音声再生要求 */	/* 音声要求バッファリング対応のためトリガIDから移動 */
	CPU_COM_REQ_TRG_ID_MAX						/* 【送信要求トリガID】最大値 */
}CPU_COM_REQ_TRIGGER_ID;

/* CPU間通信画面制御情報 種別ID */
typedef enum _CPU_COM_CONTROL_STATUS_ID {
	CPU_COM_CONTROL_STATUS_ID_NONE=0,			/* 【画面制御情報 種別ID】状態無し */
	CPU_COM_CONTROL_STATUS_ID_NEWS_END,			/* 【画面制御情報 種別ID】ニュース配信終了 */
	CPU_COM_CONTROL_STATUS_ID_ANIME_END,		/* 【画面制御情報 種別ID】アニメ再生終了 */
	CPU_COM_CONTROL_STATUS_ID_MAX				/* 【画面制御情報 種別ID】最大値 */
}CPU_COM_CONTROL_STATUS_ID;


/* CPU間通信 コマンド種別 */
/* 要求・応答のセット */
typedef enum _CPU_COM_CMD_ID{
	CPU_COM_CMD_NONE=0,							/* 【CPU間通信コマンド】コマンド無し				*/
	CPU_COM_CMD_STATUS_REQ,						/* 【CPU間通信コマンド】ステータス要求				*/
	CPU_COM_CMD_SENSOR_DATA,					/* 【CPU間通信コマンド】センサーデータ更新			*/
	CPU_COM_CMD_MODE_CHG,						/* 【CPU間通信コマンド】状態変更(G1D)				*/
	CPU_COM_CMD_PC_LOG,							/* 【CPU間通信コマンド】PCログ送信(内部コマンド)	*/
	
	CPU_COM_CMD_PRG_DOWNLORD_REQ,				/* 【CPU間通信コマンド】プログラム転送開始		*/
	CPU_COM_CMD_PRG_DOWNLORD_DATA_REQ,			/* 【CPU間通信コマンド】プログラム転送データ要求 */
	CPU_COM_CMD_PRG_DOWNLORD_DATA_RCV,			/* 【CPU間通信コマンド】プログラム転送(受信)	*/
	CPU_COM_CMD_PRG_DOWNLORD_SUM_REQ,			/* 【CPU間通信コマンド】プログラム転送サム値要求 */
	CPU_COM_CMD_PRG_DOWNLORD_RESLUT_REQ,		/* 【CPU間通信コマンド】プログラム転送結果要求	*/
	CPU_COM_CMD_FILE_REQ,						/* 【CPU間通信コマンド】ファイル転送開始		*/
	CPU_COM_CMD_FILE,							/* 【CPU間通信コマンド】ファイル転送			*/
	CPU_COM_CMD_FILE_BLOCK_RESULT_REQ,			/* 【CPU間通信コマンド】ブロック転送結果要求	*/
	CPU_COM_CMD_FILE_RESLUT_REQ,				/* 【CPU間通信コマンド】ファイル転送結果要求	*/
	CPU_COM_CMD_MAX								/* 【CPU間通信コマンド】最大値					*/
}CPU_COM_CMD_ID;



/* コマンドタイプ */
/* ※マスター専用 */
#define CPU_COM_CMD_TYPE_ONESHOT_SEND			0					/* 単発送信コマンド */
#define CPU_COM_CMD_TYPE_RETRY					1					/* リトライ */
#define CPU_COM_CMD_TYPE_RENSOU					2					/* 連送 */
#define CPU_COM_CMD_TYPE_ONESHOT_RCV			3					/* 単発受信コマンド */


/* CPU間通信ダウンロード */
/* RD1402暫定 ブロック番号は0始まりでSH側でオフセットをかける */
/* ブロック番号最大値は暫定値のため要調整 */
#define CPU_COM_BLOCK_NUM_MIN			(0x00000000)		/* CPU間通信ブロック番号 最小値 */
#define CPU_COM_BLOCK_NUM_MAX			(0x00FFFFFF)		/* CPU間通信ブロック番号 最大値 */

// RD8001暫定：ドライバへ移動
/* 送信ステータス */
#define DRV_CPU_COM_STATUS_CAN_SEND		(0)			/* 送信可能状態 */
#define DRV_CPU_COM_STATUS_SENDING		(1)			/* 送信中 */
//#define DRV_CPU_COM_STATUS_SEND_END		(2)			/* 送信終了 */ /* 送信終了状態から5ms後に送信可能となる */



/* 読み出し(文字列、ファイル)結果 */
typedef enum{
	READ_RESULT_NO_ERROR = 0,		/* 正常終了 */
	READ_RESULT_ERROR_READ,			/* 読み出し失敗 */
	READ_RESULT_FILE_NON,			/* ファイルなし */
	READ_RESULT_MAX
} ENUM_READ_RESULT;


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



//サブCPU通信異常判定回数
#define CPU_COM_SUB_CNT_OPE_START			( 20 )					/* 開始用約1秒[10ms] */
#define CPU_COM_SUB_CNT_NORMAL				( 1 * 60 * 100 )		/* 通常約1分[10ms] */
#define CPU_COM_SUB_CNT_PRG					( 6 * 60 * 100 )		/* プログラム書換約6分[10ms] */

#define CPU_COM_SUB_OPE_END_TIME_PRG		7000	/* 操作完了確認周期[70秒] ※プログラム */
#define CPU_COM_SUB_OPE_END_TIME			500		/* 操作完了確認周期[5秒] ※その他 */


// 操作終了状態
typedef enum{
	CPU_COM_SUB_OPE_END_RESULT_OK = 0,			/* 0：正常終了*/
	CPU_COM_SUB_OPE_END_RESULT_NG,				/* 1：異常終了 */
	CPU_COM_SUB_OPE_END_RESULT_CONT,			/* 2：処理中 */
}CPU_COM_SUB_OPE_END_RESULT;

/* サブマイコン操作 */
typedef enum{
	CPU_COM_SUB_OPE_IMAGE = 0,			/* 画像転送(メモリーカード) */
	CPU_COM_SUB_OPE_PRG,				/* アンドロイド(サブマイコン)プログラム更新 */
	CPU_COM_SUB_OPE_BACKUP,				/* バックアップ(アンドロイド→メモリーカード) */
	CPU_COM_SUB_OPE_RECOVERY,			/* リカバリ(メモリーカード→アンドロイド) */
	CPU_COM_SUB_OPE_NON					/* なし */
}CPU_COM_SUB_OPE;

/* サブマイコン操作状態 */
typedef enum{
	CPU_COM_SUB_STATE_NON = 0,			/* なし */
	CPU_COM_SUB_STATE_START,			/* 操作開始(アンドロイド→メモリーカード) */
	CPU_COM_SUB_STATE_END,				/* 操作終了(アンドロイド→メモリーカード) */
	CPU_COM_SUB_STATE_RX_PRG,			/* RX(メインマイコン)プログラム更新開始 */
}CPU_COM_SUB_STATE;

//サブCPU操作
typedef struct{
	CPU_COM_SUB_OPE ope;		// 操作
	CPU_COM_SUB_STATE state;	// 状態
	UW last_time;				// 時間
	UW no_res_cnt;				// 未応答回数
}CPU_COM_SUB;

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
	UH file_num;					/* ファイル番号	2byte */
	UW block_num;					/* ブロック番号	4byte */
	UB* p_data;						/* 書き換えデータの先頭ポインタ	256byteまで */
	UH len;							/* データ長 *///RD1402暫定
}T_CPU_COM_CMD_FILE_DATA;


typedef struct{
	UW block_num;					/* ブロック番号	4byte */
}T_CPU_COM_CMD_FILE_RESULT_BLOCK;

typedef struct{
	UW sum;							/* サム値	4byte */
}T_CPU_COM_CMD_FILE_RESULT;

// ファイル読み出し
typedef struct{
	UH file_num;					/* ファイル番号	2byte */
	UW block_num;					/* ブロック番号	4byte */
}T_CPU_COM_CMD_READ_FILE;




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
