/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : eep.h															*/
/* 機能         : EEP(マクロ定義、型定義、関数の外部参照宣言)					*/
/* 変更履歴     : 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef		_EEP_H_			/* 二重定義防止 */
#define		_EEP_H_

/************************************************************/
/* マクロ													*/
/************************************************************/
// デバイス関連
#define EEP_DEVICE_ADR			0xA0				// デバイスアドレス
#define EEP_RECORD_SIZE			8					// 1レコードサイズ ※256の約数とするページ跨ぎを行いたくない為
#define EEP_ADRS_SIZE			2					// アドレスサイズ

#define EEP_DATA_SIZE_ALL		131072				// EEP全サイズ
#define EEP_DATA_SIZE_HALF		( 131072 / 2 )		// EEP半分サイズ

#define EEP_ACCESS_ONCE_SIZE	256					// アクセスサイズ１回

#define EEP_PAGE_CNT_MAX		( EEP_DATA_SIZE_ALL / EEP_ACCESS_ONCE_SIZE )		//ページ数最大


// データ数
#define EEP_CACL_DATA_NUM			1440
#define EEP_FRAME_MAX				10

// EEPデータサイズ
#define EEP_CACL_DATA_SIZE			8		//ダミー含む
#define EEP_FRAM_ADD_SIZE			256		// フレーム用付加情報(時間,最高無呼吸,演算回数)
#define EEP_FRAME_SIZE				(UW)(( EEP_CACL_DATA_SIZE * 1440 ) + EEP_FRAM_ADD_SIZE )
#define EEP_SETTING_SIZE			3		
#define EEP_ALARM_SIZE				8
#define EEP_DATE_SIZE				7
#define EEP_MUKOKYU_TIME_SIZE		2



// 先頭アドレス
#define EEP_ADRS_TOP_FRAME				0													// フレームの先頭
#define EEP_ADRS_TOP_FRAME_CALC_CNT		(UW)( ( EEP_CACL_DATA_SIZE * 1440 ) + 10 )
#define EEP_ADRS_TOP_FRAME_DATE			(UW)( ( EEP_CACL_DATA_SIZE * 1440 ) + 0 )
#define EEP_ADRS_TOP_FRAME_MUKOKYU_TIME	(UW)( ( EEP_CACL_DATA_SIZE * 1440 ) + 8 )


#define EEP_ADRS_TOP_SETTING			(UW)( EEP_FRAME_SIZE * EEP_FRAME_MAX )				// 設定
#define EEP_ADRS_TOP_ALARM				(UW)( EEP_ADRS_TOP_SETTING + EEP_SETTING_SIZE )		// 警告機能
#define EEP_ADRS_DATA_TYPE				(UW)( EEP_DATA_SIZE_ALL - 1 )						// EEP種別(最終アドレス) ※通常時(0x00),プログラム転送(0xAA)

// ラベル
#define EEP_DATA_TYPE_NORMAL			0x00		// 通常時(0x00)
#define EEP_DATA_TYPE_PRG				0xAA		// プログラム転送(0xAA)


// RD8001暫定：デバッグコード
#define I2C_RCV_SND_SIZE	20		//RD8001暫定：サイズ要調整




/************************************************************/
/* 型定義													*/
/************************************************************/


/************************************************************/
/* 外部参照宣言												*/
/************************************************************/
extern void eep_init( void );

extern void R_IICA0_Create(void);
extern void R_IICA0_StopCondition(void);
extern void err_info( int id );
extern void eep_write( UW wr_adrs, UB* wr_data, UH len, UB wait_flg );
extern void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
extern void eep_read( UW rd_adrs, UB* rd_data, UH len );
extern void_eep_all_erase( void );


#endif

/************************************************************/
/* END OF TEXT												*/
/************************************************************/

