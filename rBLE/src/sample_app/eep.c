/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : eep.c															*/
/* 機能         : EEP（関数,RAM,ROM定義）										*/
/* 変更履歴     : 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/

/********************/
/*     include      */
/********************/
#include	"header.h"		//ユーザー定義


/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
void eep_init( void );
void wait_ms( int ms );
#if 0	//テストコード
STATIC void test_code_eep(void);
#endif

/********************/
/*     内部変数     */
/********************/

/************************************************************/
/* ＲＯＭ定義												*/
/************************************************************/

/************************************************************************/
/* 関数     : eep_init													*/
/* 関数名   : EEP初期化処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : EEP初期化処理													*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void eep_init( void )
{
	// テストコード
	//	test_code_eep();
	//	eep_all_erase();
}


/************************************************************************/
/* 関数     : eep_write													*/
/* 関数名   : EEP書き込み												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : EEP書き込み													*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void eep_write( UW wr_adrs, UB* wr_data, UH len, UB wait_flg )
{
	UB device_adrs;
	UB wr_buf[EEP_ACCESS_ONCE_SIZE + EEP_ADRS_SIZE];
	
	// サイズチェック
	if( len > EEP_ACCESS_ONCE_SIZE ){
		err_info(ERR_ID_EEP);
		return;
	}
	
	// アドレスを4→2byteへ変更
	if( wr_adrs < EEP_DATA_SIZE_HALF ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( wr_adrs < EEP_DATA_SIZE_ALL ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
	}else{
		err_info(ERR_ID_EEP);
		return;
	}
	wr_buf[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//アドレス上位
	wr_buf[1] = (UB)( wr_adrs & 0x00ff );				//アドレス下位
	
	
	memcpy( &wr_buf[2], wr_data, len);
	
	//書き込み
	i2c_write_sub( device_adrs, &wr_buf[0], ( len + EEP_ADRS_SIZE ), wait_flg );	
}

/************************************************************************/
/* 関数     : eep_read													*/
/* 関数名   : EEP読み出し												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : EEP読み出し													*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void eep_read( UW rd_adrs, UB* rd_data, UH len )
{
	UB device_adrs;
	UH read_adrs;
	
	// アドレスを4→2byteへ変更
	if( rd_adrs < EEP_DATA_SIZE_HALF ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( rd_adrs < EEP_DATA_SIZE_ALL ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
	}else{
		return;
	}
	
	read_adrs = (UH)( rd_adrs & 0xFFFF );
	
	i2c_read_sub( device_adrs, read_adrs, rd_data, len );
}

// ==================================
// =========以降テストコード=========
// ==================================
#if 0
#define I2C_RCV_SND_SIZE	20
UB eep_rx_data[I2C_RCV_SND_SIZE];        /* iica0 receive buffer */
UB eep_tx_data[I2C_RCV_SND_SIZE];        /* iica0 send buffer */

STATIC void test_code_eep(void)
{
	//エラー割り込みは発生するが正常に読み書き出来てそう※ハードによる
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );
//	memset( &eep_rx_data, 0, sizeof(eep_rx_data) );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
	eep_tx_data[2] = 11;
	eep_tx_data[3] = 22;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(5);
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
	eep_tx_data[2] = 33;
	eep_tx_data[3] = 44;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
	eep_tx_data[2] = 55;
	eep_tx_data[3] = 66;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
	eep_tx_data[2] = 77;
	eep_tx_data[3] = 88;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(5);
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );
//	wait_ms(5);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );
//	wait_ms(5);

	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	NOP();
	

}
#endif


/************************************************************/
/* END OF TEXT												*/
/************************************************************/
