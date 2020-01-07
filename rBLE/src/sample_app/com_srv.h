/****************************************************************************/
/* システム名   : RD8001 快眠チェッカー										*/
/* ファイル名   : com_srv.h													*/
/* 機能         : ログ通信(RS232C)											*/
/* 変更履歴		: 2019.05.13 Axia Soft Design mmura		初版作成			*/
/* 注意事項     : なし														*/
/****************************************************************************/
#ifndef _COM_SRV_H_
#define _COM_SRV_H_


/************************************************************/
/* 定数定義													*/
/************************************************************/
#define	DRV_UART1_DATA_LENGH					150									/* RS232C通信データ長(送受、共通) ヘッダ８バイト＋オプションデータ92バイト[予約] */
#define	DRV_UART1_RCV_RING_LENGTH				( DRV_UART1_DATA_LENGH * 1 + 1 )		/* リングバッファ長(1メッセージ分＋１) ※＋１の理由はリングバッファの注意事項参照 */
#define	DRV_UART1_SND_RING_LENGTH				( DRV_UART1_DATA_LENGH * 1 + 1 )		/* リングバッファ長(1メッセージ分＋１) ※＋１の理由はリングバッファの注意事項参照 */

#define COM_STR_MAX				DRV_UART1_DATA_LENGH

#define COM_ANA_MSG_MAX			10		// 解析最長10バイト
#define COM_RCV_NON_CNT_MAX		200		// 受信なし判定カウント

/******************/
/*     型定義     */
/******************/
typedef struct{
	UB len;
	UB data[COM_ANA_MSG_MAX];
	UB rcv_non;
}COM_SRV_ANA;



/******************/
/*  外部参照宣言  */
/******************/

extern void com_srv_init( void );
extern void com_srv_log_title( void );
extern void com_srv_cyc( void );
extern void com_srv_send( UB* tx_data, UB len );
extern void com_srv_rcv( UB* rx_data, UB len );
extern void com_srv_puts( const B* pMsg);
extern bool com_srv_read_comp( void );
extern void com_srv_write_comp( void );
extern void com_srv_error_comp( void );
extern void com_srv_isread( void );
extern bool com_srv_first_data( void );
extern UB com_get_read_status(void);

/******************/
/* 関数マクロ定義 */
/******************/
	
#endif
