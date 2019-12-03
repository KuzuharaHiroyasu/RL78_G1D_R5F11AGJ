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
#define EEP_CALC_DATA_NUM			1440	// 12時間分(1データ/30秒)
#define EEP_FRAME_MAX				7		// 1週間分

// EEPデータサイズ
#define EEP_CALC_DATA_SIZE						8		// 
#define EEP_RESERVE_SIZE						5760	// 保存データ1に対して4バイト空きを設ける（4バイト × 1440個 = 5760バイト)
#define EEP_FRAM_ADD_SIZE						128		// フレーム用付加情報(時間,いびき検知数～最高無呼吸,演算回数)
#define EEP_FRAME_SIZE							(UW)(( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + EEP_FRAM_ADD_SIZE )
#define EEP_SETTING_SIZE						3
#define EEP_ALARM_SIZE							5
#define EEP_DATE_SIZE							7
#define EEP_IBIKI_DETECT_CNT_SIZE				2
#define EEP_MUKOKYU_DETECT_CNT_SIZE				2
#define EEP_IBIKI_TIME_SIZE						2
#define EEP_MUKOKYU_TIME_SIZE					2
#define EEP_MAX_MUKOKYU_TIME_SIZE				2
#define EEP_DEVICE_SET_INFO_SIZE				1
#define EEP_DEVICE_SET_SUPPRESS_START_TIME_SIZE	1


// 先頭アドレス
#define EEP_ADRS_TOP_FRAME										0													// フレームの先頭
#define EEP_ADRS_TOP_FRAME_DATE									(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 0 )
#define EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT						(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 8 )
#define EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT					(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 10 )
#define EEP_ADRS_TOP_FRAME_IBIKI_TIME							(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 12 )
#define EEP_ADRS_TOP_FRAME_MUKOKYU_TIME							(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 14 )
#define EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME						(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 16 )
#define EEP_ADRS_TOP_FRAME_DEVICE_SET_INFO						(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 18 )
#define EEP_ADRS_TOP_FRAME_DEVICE_SET_SUPPRESS_START_TIME		(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 19 )
#define EEP_ADRS_TOP_FRAME_CALC_CNT								(UW)( ( EEP_CALC_DATA_SIZE * 1440 ) + EEP_RESERVE_SIZE + 20 )

#define EEP_ADRS_TOP_SETTING			(UW)( EEP_FRAME_SIZE * EEP_FRAME_MAX )				// 設定
#define EEP_ADRS_TOP_ALARM				(UW)( EEP_ADRS_TOP_SETTING + EEP_SETTING_SIZE )		// 警告機能
#define EEP_ADRS_DATA_TYPE				(UW)( EEP_DATA_SIZE_ALL - 1 )						// EEP種別(最終アドレス) ※通常時(0x00),プログラム転送(0xAA)



// ラベル
#define EEP_DATA_TYPE_NORMAL				0x11		// 通常時(0x11)
#define EEP_DATA_TYPE_PRG_H1D				0xAA		// H1Dプログラム転送(0xAA)


#define EEP_I2C_LOCK_ERR_VAL			10000		// I2C異常判定回数(ロック)


/************************************************************/
/* 型定義													*/
/************************************************************/


/************************************************************/
/* 外部参照宣言												*/
/************************************************************/
extern void eep_init( void );

extern void eep_write( UW wr_adrs, UB* wr_data, UH len, UB wait_flg );
extern void eep_read( UW rd_adrs, UB* rd_data, UH len );
extern void eep_setting_area_erase( void );

#endif

/************************************************************/
/* END OF TEXT												*/
/************************************************************/

