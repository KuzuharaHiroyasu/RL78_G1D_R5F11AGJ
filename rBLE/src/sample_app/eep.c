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
STATIC void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
STATIC void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void wait_ms( int ms );
#if 0	//テストコード
STATIC void test_code_eep(void);
#endif

/********************/
/*     内部変数     */
/********************/
// I2Cドライバのフラグ ※i2cドライバで設定される
STATIC UB i2c_snd_flg;			//送信フラグ(I2C)
STATIC UB i2c_rcv_flg;			//受信フラグ(I2C)
// RD8001暫定：EEPの異常時の動作が仕様上未定義
STATIC UB i2c_err_flg;			//エラーフラグ(I2C)


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
	//データ初期化
	i2c_snd_flg = OFF;
	i2c_rcv_flg = OFF;
	i2c_err_flg = OFF;
	
	// I2C初期化
	R_IICA0_Create();
	
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
/* 関数     : i2c_write_sub												*/
/* 関数名   : I2C書き込みサブ											*/
/* 引数     : device_adrs	デバイスアドレス							*/
/*          : wr_data		書き込みデータ								*/
/*          : len			データ長									*/
/*          : wait_flg		書き込み待ちフラグ							*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : I2C書き込みサブ												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg )
{
	UW lcok_cnt = 0;

	if( 0 != R_IICA0_Master_Send( device_adrs, wr_data, len, I2C_WAIT)){
		err_info(ERR_ID_EEP);
		i2c_err_flg = ON;
	}else{
		while(1){
			if( OFF == i2c_snd_flg ){
				// 送信完了
				break;
			}
			if( lcok_cnt++ >= EEP_I2C_LOCK_ERR_VAL ){
				// 異常時
				err_info(ERR_ID_EEP);
				i2c_err_flg = ON;
				break;
			}
		}
		if( OFF == i2c_err_flg ){
			R_IICA0_StopCondition();		//　異常発生時は予期せぬ書き込みを防止する為にストップコンディション発行しない
		}
	}
	
	while(0U == SPD0){}

	if( wait_flg == ON ){
		wait_ms(5);			/* Write Cycle Time */
	}
	
	if( ON == i2c_err_flg ){
		i2c_err_flg = OFF;
		R_IICA0_Create();		// I2C初期化
	}
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

/************************************************************************/
/* 関数     : i2c_read_sub												*/
/* 関数名   : I2C読み出しサブ											*/
/* 引数     : device_adrs	デバイスアドレス							*/
/*          : read_adrs		読み出しアドレス							*/
/*          : read_data		読み出しデータ								*/
/*          : len			データ長									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : I2C読み出しサブ												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len )
{
	UB adrs[2];
	UB adrs_size = 0;
	UW lcok_cnt = 0;

	adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//アドレス上位
	adrs[1] = (UB)( read_adrs & 0x00ff );			//アドレス下位
	adrs_size  = 2;

	if( 0 != R_IICA0_Master_Send( device_adrs, &adrs[0], adrs_size, I2C_WAIT )){
		err_info(ERR_ID_EEP);
	}else{
		while(1){
			if( OFF == i2c_snd_flg ){
				// 送信完了
				break;
			}
			if( lcok_cnt++ >= EEP_I2C_LOCK_ERR_VAL ){
				// 異常時
				err_info(ERR_ID_EEP);
				i2c_err_flg = ON;
				break;
			}
		}

// RD8001暫定：どの様(バス開放orストップコンディション)に対応するか問合せ中
#if 0
	//ストップコンディションあり
		R_IICA0_StopCondition();
	}
	/* ストップコンディション後の待ち */
	while(0U == SPD0){}
//	WAIT_5US();
//	WAIT_5US();
//	wait_ms(5);
#else
	}
	//バス開放
	R_IICA0_Stop();
	R_IICA0_Create();
	
#endif

	if( 0 != R_IICA0_Master_Receive(device_adrs, read_data, len, I2C_WAIT)){
		err_info(ERR_ID_EEP);
	}else{
		while(1){
			if( OFF == i2c_rcv_flg ){
				// 受信完了
				break;
			}
			if( lcok_cnt++ >= EEP_I2C_LOCK_ERR_VAL ){
				// 異常時
				err_info(ERR_ID_EEP);
				i2c_err_flg = ON;
				break;
			}
		}
		if( OFF == i2c_err_flg ){
			R_IICA0_StopCondition();		//　異常発生時は予期せぬ書き込みを防止する為にストップコンディション発行しない
		}
	}
	while(0U == SPD0){}
	
	if( ON == i2c_err_flg ){
		i2c_err_flg = OFF;
		R_IICA0_Create();		// I2C初期化
	}
}


/************************************************************************/
/* 関数     : i2c_set_snd_flg											*/
/* 関数名   : 送信フラグ設定											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 送信フラグ設定												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void i2c_set_snd_flg( UB data )
{
	i2c_snd_flg = data;
}

/************************************************************************/
/* 関数     : i2c_set_snd_flg											*/
/* 関数名   : 受信フラグ設定											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 受信フラグ設定												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void i2c_set_rcv_flg( UB data )
{
	i2c_rcv_flg = data;
}

/************************************************************************/
/* 関数     : i2c_set_err_flg											*/
/* 関数名   : エラーフラグ設定											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : エラーフラグ設定												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void i2c_set_err_flg( UB data )
{
	i2c_err_flg = data;
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
