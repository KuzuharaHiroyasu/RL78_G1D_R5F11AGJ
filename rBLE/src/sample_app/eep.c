/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : eep.c														*/
/* 機能         : 時間管理部（関数,RAM,ROM定義）								*/
/* 変更履歴     : 2018.01.25 Axia Soft Design 西島 稔	初版作成		*/
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
void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg );
void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len );
void wait_ms( int ms );
void err_info( int id );
void test_code_eep(void);
void eep_all_erase( void );


/************************************************************/
/* ＲＡＭ定義												*/
/************************************************************/
//RD8001暫定：削除予定
UB eep_rx_data[I2C_RCV_SND_SIZE];        /* iica0 receive buffer */
UB eep_tx_data[I2C_RCV_SND_SIZE];        /* iica0 send buffer */

int i2c_cmplete;





/************************************************************/
/* ＲＯＭ定義												*/
/************************************************************/
extern T_UNIT s_unit;			//RD8001暫定：staticへ変更予定

//RD8001暫定：1msとetc処理は現状未使用。最終的に使用しない場合は削除する事

/************************************************************************/
/* 関数     : eep_init													*/
/* 関数名   : ソフトウェアタイマ初期化処理								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2012.02.16 Axia Soft Design 六車 裕介	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* ソフトウェアタイマ初期化処理											*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void eep_init( void )
{
#if FUNC_DEBUG_EEP_NON == OFF
	//データ初期化
	R_IICA0_Create();
	//test_code_eep();
//	eep_all_erase();
#endif
}

//================================
//EEP関連
//================================
#define			EEP_WAIT		255		//RD8001暫定値

void eep_write( UW wr_adrs, UB* wr_data, UH len, UB wait_flg )
{
	UB device_adrs;
	UB wr_buf[EEP_ACCESS_ONCE_SIZE + EEP_ADRS_SIZE];
	
	// サイズチェック
	if( len > EEP_ACCESS_ONCE_SIZE ){
		err_info(8);
		return;
	}
	
	// アドレスを4→2byteへ変更
	if( wr_adrs < EEP_DATA_SIZE_HALF ){
		device_adrs = EEP_DEVICE_ADR;
	}else if( wr_adrs < EEP_DATA_SIZE_ALL ){
		device_adrs = EEP_DEVICE_ADR | 0x02;
	}else{
		err_info(8);
		return;
	}
	wr_buf[0] = (UB)(( wr_adrs >> 8 ) & 0xff );		//アドレス上位
	wr_buf[1] = (UB)( wr_adrs & 0x00ff );				//アドレス下位
	
	
	memcpy( &wr_buf[2], wr_data, len);
	
	//書き込み
	i2c_write_sub( device_adrs, &wr_buf[0], ( len + EEP_ADRS_SIZE ), wait_flg );	
}


void i2c_write_sub( UB device_adrs, UB* wr_data, UH len, UB wait_flg )
{
	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Send( device_adrs, wr_data, len, EEP_WAIT)){
		err_info(2);
	}else{
		while(1){
			if( 1 == i2c_cmplete ){
				break;
			}
		}
		R_IICA0_StopCondition();
	}
	
	while(0U == SPD0){}

	/* ストップコンディション後の待ち */
#if 1
	if( wait_flg == ON ){
		wait_ms(5);		//RD8001暫定
//		wait_ms(40);
		
	}
#else
    NOP();
	wait_ms(20);		//RD8001暫定
    NOP();
#endif
	
}

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

void i2c_read_sub( UB device_adrs, UH read_adrs, UB* read_data, UH len )
{
	UB adrs[2];
	UB adrs_size = 0;
	UW lcok_cnt = 0;

//	if( EEP_DEVICE_ADR == device_adrs ){
		// EEP時
		adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//アドレス上位
		adrs[1] = (UB)( read_adrs & 0x00ff );			//アドレス下位
		adrs_size  = 2;
//	}else{
//		return;
//	}

	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Send( device_adrs, &adrs[0], adrs_size, EEP_WAIT )){
		err_info(3);
	}else{
		while(1){
			if(( 1 == i2c_cmplete ) || ( lcok_cnt++ >= 10000 )){		//RD8001暫定
//			if( 1 == i2c_cmplete ){		//RD8001暫定
				break;
			}
		}
#if 0	//ストップコンディションあり
//		R_IICA0_StopCondition();
	}
	/* ストップコンディション後の待ち */
//	WAIT_5US();		//RD8001暫定
//	WAIT_5US();		//RD8001暫定
	wait_ms(5);
#else
	}
	//RD8001暫定:バス開放
	R_IICA0_Stop();
	R_IICA0_Create();
	
#endif

	// ストップビットあり
	//RD8001暫定：ストップコンディションを送らないと読み出し失敗する
	adrs[0] = (UB)(( read_adrs >> 8 ) & 0xff );		//アドレス上位
	adrs[1] = (UB)( read_adrs & 0x00ff );		//アドレス下位
		
	i2c_write_sub(device_adrs, &adrs[0], 2 , OFF );
	i2c_cmplete = 0;
	if( 0 != R_IICA0_Master_Receive(device_adrs, read_data, len, EEP_WAIT)){
		err_info(4);
	}else{
		while(1){
			if( 1 == i2c_cmplete ){
				break;
			}
		}
		R_IICA0_StopCondition();
	}
	while(0U == SPD0){}
	
}

int dbg_cnt2 = 0;
void wait_ms( int ms )
{
	int i,j;
	
	for(i = 0; i < ms; i++ ){
		for(j = 0; j < 100; j++ ){
			WAIT_10US()
			WAIT_10US()
			dbg_cnt2++;
		}
	}
}



//void err_info( ERR_ID id )
void err_info( int id )
{
	s_unit.err_cnt++;
#if 0
	while(1){
		// 異常による永久ループ
	}
#endif
}

const UB eep_erase_tbl[256] = {0};

void eep_all_erase( void )
{
	UW adrs = 0;
	UW i = 0;
	
	for( i = 0; i < (EEP_DATA_SIZE_ALL / EEP_ACCESS_ONCE_SIZE); i++ ){
		adrs = i * EEP_ACCESS_ONCE_SIZE;
		eep_write( adrs, (UB*)&eep_erase_tbl[0], EEP_ACCESS_ONCE_SIZE, ON );
	}
}


void test_code_eep(void)
{
	//エラー割り込みは発生するが正常に読み書き出来てそう※ハードによる
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );
//	wait_ms(20);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );
//	wait_ms(20);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );
//	memset( &eep_rx_data, 0, sizeof(eep_rx_data) );
//	wait_ms(20);


#if 1
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
	eep_tx_data[2] = 11;
	eep_tx_data[3] = 22;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(20);
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
	eep_tx_data[2] = 33;
	eep_tx_data[3] = 44;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(20);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
	eep_tx_data[2] = 55;
	eep_tx_data[3] = 66;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(20);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
	eep_tx_data[2] = 77;
	eep_tx_data[3] = 88;
	i2c_write_sub( 0xA0, &eep_tx_data[0], 4 , ON);
//	wait_ms(20);
	
	eep_tx_data[0] = 0;
	eep_tx_data[1] = 0;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 0, &eep_rx_data[0], 2 );
//	wait_ms(20);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 5;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 5, &eep_rx_data[2], 2 );
//	wait_ms(20);

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 8;
///	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 8, &eep_rx_data[4], 2 );

	eep_tx_data[0] = 0;
	eep_tx_data[1] = 13;
//	i2c_write_sub( 0xA0, &eep_tx_data[0], 2 );

	i2c_read_sub( 0xA0, 13, &eep_rx_data[6], 2 );
//	wait_ms(20);

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
	

#endif

#if 0
	//デバッグコード
	s_unit.eep_wr_record_cnt = 0;
	
	s_unit.kokyu_val  = 1111;		
	s_unit.ibiki_val  = 2222;		
	s_unit.sekishoku_val = 333333;	// 差動入力の為に符号あり
	s_unit.sekigaival = -444444;		// 差動入力の為に符号あり
    main_eep_write();
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}
	s_unit.eep_wr_record_cnt = 4096;
	
	s_unit.kokyu_val  = 3333;		
	s_unit.ibiki_val  = 4444;		
	s_unit.sekishoku_val = 555555;	// 差動入力の為に符号あり
	s_unit.sekigaival = -666666;		// 差動入力の為に符号あり
    main_eep_write();
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}



	s_unit.eep_wr_record_cnt = 4095;
	
	s_unit.kokyu_val  = 2222;		
	s_unit.ibiki_val  = 3333;		
	s_unit.sekishoku_val = 444444;		// 差動入力の為に符号あり
	s_unit.sekigaival = -555555;		// 差動入力の為に符号あり
    main_eep_write();
    
	for(i = 0; i < 500; i++ ){
		WAIT_10US()
	}
	s_unit.eep_rd_record_cnt = 0;
    main_eep_read();
	s_unit.eep_rd_record_cnt = 4096;
    main_eep_read();
	s_unit.eep_rd_record_cnt = 4095;
    main_eep_read();
#endif
	
#if 0
	s_unit.eep_rd_record_cnt = 0;
	s_unit.eep_wr_record_cnt = 0;
	s_unit.kokyu_val  = 1111;		
	s_unit.ibiki_val  = 2222;		
	s_unit.sekishoku_val = 333333;	// 差動入力の為に符号あり
	s_unit.sekigaival = -444444;		// 差動入力の為に符号あり
	while(1){
		if( INVALID == main_eep_write() ){
			// 終了
			break;
		}
		//wait_ms(5);
//		for(i = 0; i < 500; i++ ){
//			WAIT_10US()
//		}
	}	
	
#endif	
	
	
}


/************************************************************/
/* END OF TEXT												*/
/************************************************************/
