/************************************************************************/
/* システム名   : RD8001 快眠チェッカー									*/
/* ファイル名   : com_srv.c                                             */
/* 機能         : ログ通信（RS-232C）				                    */
/* 変更履歴     : 2019.05.13 Axia Soft Design mmura		初版作成		*/
/* 注意事項     : なし                                                  */
/************************************************************************/
/********************/
/*     include      */
/********************/
#include "header.h"

#include "serial.h"
/********************/
/* プロトタイプ宣言 */
/********************/
void com_srv_init( void );
void com_srv_log_title( void );
void com_srv_cyc( void );
//STATIC void com_srv_command( UB data );
void com_srv_send( UB* tx_data, UB len );

/********************/
/*     内部定数     */
/********************/


/********************/
/*     内部変数     */
/********************/
//STATIC UB s_drv_cpu_com_snd_status;							/* CPU間通信ドライバ(物理レベル)の送信ステータス */
																/* DRV_CPU_COM_STATUS_CAN_SEND		送信可能状態 */
																/* DRV_CPU_COM_STATUS_SENDING		送信中 */

/********************/
/*     外部参照     */
/********************/
extern const B		version_product_tbl[];				/* ソフトウェアバージョン */

/************************************************************************/
/* 関数     : com_srv_init                                              */
/* 関数名   : 保守アプリ初期化処理                                      */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 保守アプリ初期化処理                                          */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void com_srv_init( void )
{
#if FUNC_DEBUG_LOG == ON
	// プラットフォーム関連の設定
	SERIAL_EVENT_PARAM call_back = {0};
	
	call_back.rx_callback = &com_srv_read_comp;
	call_back.tx_callback = &com_srv_write_comp;
	call_back.err_callback = &com_srv_error_comp;
	
	serial_init( &call_back );
	
	// 変数の初期化
	s_drv_cpu_com_snd_status = DRV_CPU_COM_STATUS_CAN_SEND;
#endif
}

/************************************************************************/
/* 関数     : com_srv_read_comp											*/
/* 関数名   : 読み出し完了(プラットフォーム)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島		初版作成			*/
/************************************************************************/
/* 機能 : 読み出し完了(プラットフォーム)								*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void com_srv_read_comp( void )
{
#if FUNC_DEBUG_LOG == ON
#endif
}

/************************************************************************/
/* 関数     : com_srv_write_comp										*/
/* 関数名   : 書き込み完了(プラットフォーム)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島		初版作成			*/
/************************************************************************/
/* 機能 : 書き込み完了(プラットフォーム)								*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void com_srv_write_comp( void )
{
#if FUNC_DEBUG_LOG == ON
	s_drv_cpu_com_snd_status = DRV_CPU_COM_STATUS_CAN_SEND;
#endif
}

/************************************************************************/
/* 関数     : com_srv_error_comp										*/
/* 関数名   : 異常発生(プラットフォーム)								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島		初版作成			*/
/************************************************************************/
/* 機能 : 異常発生(プラットフォーム)									*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void com_srv_error_comp( void )
{
}

/************************************************************************/
/* 関数     : com_srv_log_title                                         */
/* 関数名   : タイトル出力			                                    */
/* 引数     : なし                                                      */
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : タイトル出力処理の関数化                                      */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void com_srv_log_title( void )
{
#if FUNC_DEBUG_LOG == ON
#if 0
	UB	boot_ver[4];				/* Boot部バージョン情報 */
	

	int len;
	UB tx_data[DRV_UART1_DATA_LENGH] = {0};
	
	len = sprintf((char*)tx_data, "POW ON APL V.%d.%d.%d.%d  ", version_product_tbl[0], version_product_tbl[1],
																	   version_product_tbl[2], version_product_tbl[3] );
	com_srv_send( tx_data, len );
	wait_ms(5);
#endif
#endif
}

/************************************************************************/
/* 関数     : com_srv_send                                              */
/* 関数名   : 送信処理					                                */
/* 引数     : tx_data		送信バッファ								*/
/*          : len			送信長										*/
/* 戻り値   : なし                                                      */
/* 変更履歴 : 2019.05.13  Axia Soft Design mmura     初版作成           */
/************************************************************************/
/* 機能 : 送信処理									                    */
/************************************************************************/
/* 注意事項 : なし                                                      */
/************************************************************************/
void com_srv_send( UB* tx_data, UB len )
{
#if FUNC_DEBUG_LOG == ON
	if(s_drv_cpu_com_snd_status == DRV_CPU_COM_STATUS_CAN_SEND){
		s_drv_cpu_com_snd_status = DRV_CPU_COM_STATUS_SENDING;
		serial_write( tx_data, len );
	}
#endif
}

/*****************************************************************************
 *								 end of text
 *****************************************************************************/
