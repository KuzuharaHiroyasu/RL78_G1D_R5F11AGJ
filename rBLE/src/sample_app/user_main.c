/**
 ****************************************************************************************
 *
 * @file		DTM2Wire.c
 *
 * @brief Direct Test Mode 2Wire UART Driver.
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 ****************************************************************************************
 */

/*
 * INCLUDE FILES
 ****************************************************************************************
 */
#include "rwble_config.h"
#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */
#include	"ke_task.h"
#endif

#include	"rble.h"
#include	"rble_api.h"

#include	"port.h"
#include	"uart.h"

#include	"header.h"				//ユーザー定義

#include	"r_vuart_app.h"

// RD8001暫定：OS関連の名前は暫定なので後で修正予定





// プロトタイプ宣言
static int_t cpu_com_evt(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t cpu_com_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
STATIC void user_main_mode_idle(void);
STATIC void user_main_mode_sensing(void);
STATIC void user_main_mode_move(void);
STATIC void user_main_mode_get(void);
STATIC void user_main_mode_prg_hd(void);
STATIC void user_main_req_cyc( void );
STATIC void main_cpu_com_snd_sts_req( void );
STATIC void main_cpu_com_snd_mode_chg( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts_res( void );
STATIC void main_cpu_com_rcv_sensor_res( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode );
STATIC void user_main_mode( void );
STATIC void main_cpu_com_snd_pc_log( UB* data, UB size );
STATIC void main_vuart_proc(void);
STATIC void main_cpu_com_rcv_get_mode( void );
STATIC void user_main_calc_result( void );
STATIC void user_main_mode_sensing_before( void );
STATIC void user_main_mode_sensing_after( void );
STATIC UB user_main_mode_get_before( void );
STATIC void user_main_mode_get_after( void );
STATIC void user_main_eep_read(void);
STATIC void main_send_vuart( UB *p_data, UB len );
STATIC UB main_prg_hd_eep_code_record( void );
STATIC void main_prg_hd_result(void);
STATIC void main_prg_hd_update(void);
STATIC void main_cpu_com_rcv_prg_hd_ready(void);
STATIC void main_cpu_com_rcv_prg_hd_start(void);
STATIC void main_cpu_com_rcv_prg_hd_erase(void);
STATIC void main_cpu_com_rcv_prg_hd_data(void);
STATIC void main_cpu_com_rcv_prg_hd_reslut(void);
STATIC void main_cpu_com_rcv_prg_hd_check(void);
STATIC void main_prg_hd_read_eep_record( void );
STATIC void main_cpu_com_rcv_date_set( void );
void main_vuart_rcv_set_mode( void );
void main_vuart_rcv_data_frame( void );
void main_vuart_rcv_data_calc( void );
void main_vuart_rcv_data_end( void );
void main_vuart_rcv_date( void );



// 以降演算部の処理
static int_t main_calc_sekigai(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_sekishoku(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);


#include	"user_main_tbl.h"		// ユーザーテーブル実態定義


extern void test_cpu_com_send( void );



//変数定義
/* Status */
ke_state_t cpu_com_state[ CPU_COM_IDX_MAX ] = {0};


//STATIC T_UNIT s_unit;
T_UNIT s_unit;					//RD8001暫定：staticへ変更予定
STATIC DS s_ds;



// OS関連
// イベント解析処理(OS)　※通信バッファを使用している

// 1秒毎周期処理
void codeptr app_evt_usr_2(void) 
{ 
	uint8_t *ke_msg;
	
	// 秒タイマーカウントダウン
	DEC_MIN( s_unit.timer_sec ,0 );

	if( SYSTEM_MODE_SENSING != s_unit.system_mode ){
		ke_evt_clear(KE_EVT_USR_2_BIT);
		return;
	}


	s_unit.sec30_cnt++;
	if( s_unit.sec30_cnt >= 30 ){		// 30秒
//	if( s_unit.sec30_cnt >= 3 ){		// デバッグ3秒版
		
		s_unit.sec30_cnt = 0;
		
		// 演算結果
		user_main_calc_result();
	}
	

	ke_msg = ke_msg_alloc( USER_MAIN_CALC_ACL, USER_MAIN_ID, USER_MAIN_ID, 0 );
	ke_msg_send(ke_msg);

	ke_evt_clear(KE_EVT_USR_2_BIT); 
}

// 50ms周期処理
void codeptr app_evt_usr_3(void) 
{ 
	ke_evt_clear(KE_EVT_USR_3_BIT); 

	{
		uint8_t *ke_msg;

		ke_msg = ke_msg_alloc( USER_MAIN_CYC_ACT, USER_MAIN_ID, USER_MAIN_ID, 0 );

		ke_msg_send(ke_msg);
	}
//	cpu_com_proc();

}


static int_t cpu_com_evt(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	// RD8001対応：OSイベント対応(定期的に呼ばない様)
	cpu_com_proc();			// 通信サービスミドル
	
	main_vuart_proc();
	
	main_cpu_com_proc();	//通信サービスアプリ
	user_main_mode();
	{
		uint8_t *ke_msg;

		ke_msg = ke_msg_alloc( USER_MAIN_ACT2, USER_MAIN_ID, USER_MAIN_ID, 0 );

		ke_msg_send(ke_msg);
	}
//	test_cpu_com_send();
	return (KE_MSG_CONSUMED);
}


static int_t cpu_com_timer_handler(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	/* call func specified by APP */
//	if (0 != Timer_func) {
//		Timer_func();
//	}
//	cpu_com_proc();
#if 0
	uint8_t *ke_msg;

	ke_msg = ke_msg_alloc( USER_MAIN_CYC_ACT, USER_MAIN_ID, USER_MAIN_ID, 0 );

	ke_msg_send(ke_msg);
#endif
//	ke_timer_set(USER_MAIN_ACT2, USER_MAIN_ID, CPU_COM_CYC_TIME);
	
	return (KE_MSG_CONSUMED);
}

void user_system_init( void )
{
	R_PORT_Create();
	R_INTC_Create();
	R_IT_Create();
//	R_RTC_Create();
	
	//RD8001暫定：H1D側リセット解除タイミング要検討　サブクロックが止まるとBLE関連が死ぬ
	// H1Dのリセット解除はBLE処理前に必要
	drv_o_port_h1d_reset( OFF );
}

void user_main_init( void )
{
	// ミドル初期化
	cpu_com_init();
	eep_init();
	
	
	memset( &s_unit, 0, sizeof(s_unit) );
	
	//EEP リード
	user_main_eep_read();
	
	s_unit.last_time = ke_time();
	
	// 状態設定
	ke_state_set(USER_MAIN_ID, 0);
}

STATIC void user_main_calc_result( void )
{
	UW wr_adrs = 0;
#if 0
	//演算結果をPC送付
	main_cpu_com_snd_pc_log( &s_unit.calc.info.byte[0], CPU_COM_SND_DATA_SIZE_PC_LOG );
#endif
	
	// フレーム位置とデータ位置からEEPアドレスを算出
	wr_adrs = ( s_unit.frame_num_write * EEP_FRAME_SIZE ) + ( s_unit.calc_cnt * EEP_CACL_DATA_SIZE );

	eep_write( wr_adrs, (UB*)&s_unit.calc, EEP_CACL_DATA_SIZE, OFF );
	
	s_unit.calc_cnt++;
	//範囲チェック
	if( s_unit.calc_cnt >= EEP_CACL_DATA_NUM ){
		s_unit.calc_cnt = 0;
		err_info(11);
	}
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	
	
	
	
}


STATIC void user_main_mode( void )
{
	// 各種モード処理
	p_user_main_mode_func[s_unit.system_mode]();
}

STATIC void user_main_mode_sensing_before( void )
{
	UW wr_adrs = 0;

	// 日時情報書き込み
	wr_adrs = ( s_unit.frame_num_write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
	eep_write( wr_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE, ON );

	s_unit.calc_cnt = 0;
}

STATIC void user_main_mode_sensing_after( void )
{
	UB oikosi_flg = OFF;
	UW wr_adrs = 0;
	UB wr_data[2] = {0};
	
	wait_ms(5);		//RD8001暫定；待ち
	
	// 演算回数書き込み
	wr_adrs = ( s_unit.frame_num_write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_CALC_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.calc_cnt, 2, ON );
	
	
	// 書き込み枠番号を進める
	INC_MAX_INI(s_unit.frame_num_write, (EEP_FRAME_MAX - 1), 0);
	
	// 追い越し判定
	if( s_unit.frame_num_write == s_unit.frame_num_read ){
		// 追い越されてしまうので読み出しポインタを進める
		INC_MAX_INI(s_unit.frame_num_read, (EEP_FRAME_MAX  -1), 0);
		oikosi_flg = ON;
	}
	
	wr_adrs = EEP_ADRS_TOP_SETTING;
	if( OFF == oikosi_flg ){
		// 書き込みポインタ
		eep_write( wr_adrs + 1, &s_unit.frame_num_write, 1, ON );
	}else{
		// 書き込み、読み出しポインタ
		wr_data[0] = s_unit.frame_num_read;
		wr_data[1] = s_unit.frame_num_write;
		eep_write( wr_adrs, &wr_data[0], 2, ON );
	}
}

STATIC void user_main_mode_prg_hd_before( void )
{
	UB eep_data = EEP_DATA_TYPE_PRG;
	
	// データ初期化
	s_unit.prg_hd_eep_record_cnt_wr = 0;
	s_unit.prg_hd_eep_record_cnt_rd = 0;
	s_unit.prg_hd_eep_code_record_sum = 0;
	s_unit.prg_hd_update_state = 0;
	s_unit.prg_hd_seq = 0;
	
	// EEPプログラムモード
	eep_write( EEP_ADRS_DATA_TYPE, &eep_data, 1, ON );
	
	// BLE返信
	{
		UB tx[10] = {0};
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = 0x00;
	
		main_send_vuart( &tx[0], 2 );
	}
}


STATIC UB user_main_mode_get_before( void )
{
	UB ret = ON;
	UW rd_adrs = 0;
	UH calc_cnt;
	
	s_unit.get_mode_status = 0;
	s_unit.get_mode_calc_cnt = 0;
	
	
	// フレーム位置とデータ位置からEEPアドレスを算出
	rd_adrs = ( s_unit.frame_num_read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_CALC_CNT;
	
	eep_read( rd_adrs, (UB*)&calc_cnt, 2 );
	
	if( calc_cnt > EEP_CACL_DATA_NUM ){
		err_info(10);
		calc_cnt = 0;
	}
	
	s_unit.calc_cnt = calc_cnt;
	
	return ret;
}

STATIC void user_main_mode_get_after( void )
{

	s_unit.system_mode = SYSTEM_MODE_IDLE;
}


STATIC void user_main_mode_idle(void)
{
	//ステータス要求通知
	user_main_req_cyc();
	
	
	
	
}


STATIC void user_main_mode_sensing(void)
{
	ke_time_t now_time;
	
	now_time = ke_time();
	
	if(( now_time - s_unit.last_sensing_data_rcv ) >= SENSING_END_JUDGE_TIME ){
		// 規定値時間センシングなし
		user_main_req_cyc();
	}else{
		// センシングあり
	}
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
}


STATIC void user_main_mode_move(void)
{
	// 要求送信
	main_cpu_com_snd_mode_chg();
	
}

STATIC void user_main_mode_get(void)
{
	UB tx[10];
	UW wr_adrs;
	UW rd_adrs;
	CALC calc_eep;				// 演算後データ
	UH year;
	
	
	// 送信中の場合はウェイト
	if( ON == s_ds.vuart.input.send_status ){
		return;
	}
	
	
	if( 0 == s_unit.get_mode_status ){
#if 0
		tx[0] = 'S';
		tx[1] = 'T';
		tx[2] = 'A';
		tx[3] = 'R';
		tx[4] = 'T';
		tx[5] = s_unit.denchi_sts;
//		s_ds.vuart.input.send_status = ON;
//		R_APP_VUART_Send_Char( &tx[0], 6 );
		main_send_vuart( &tx[0], 6 );
#endif
		if( s_unit.frame_num_write == s_unit.frame_num_read ){
			// RD8001暫定：データなし通知仕様が無い為に暫定
			{
				UB tx[10];
//				tx[0] = 'E';
//				tx[1] = 'N';
//				tx[2] = 'D';
				tx[0] = 0xE1;		// END
	//			s_ds.vuart.input.send_status = ON;
	//			R_APP_VUART_Send_Char( &tx[0], 3 );
				main_send_vuart( &tx[0], 1 );
			}
			s_unit.get_mode_status = 6;
		}else{
			user_main_mode_get_before();
			s_unit.get_mode_status = 1;
		}
	}else if( 1 == s_unit.get_mode_status ){
		// 日時読み出し
		rd_adrs = ( s_unit.frame_num_read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
		eep_read( rd_adrs, (UB*)&s_unit.date, EEP_CACL_DATA_SIZE );
		// 無呼吸時間読み出し
		rd_adrs = ( s_unit.frame_num_read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MUKOKYU_TIME_SIZE );
		s_unit.get_mode_status = 2;
	}else if( 2 == s_unit.get_mode_status ){

#if 0
		tx[0] = (UB)( s_unit.date.year & 0x00FF );
		tx[1] = (UB)(( s_unit.date.year & 0xFF00 ) >> 8);		// リトルエンディアン
#else	
		// RD8001暫定：
		tx[0] = 0xe2;
		tx[1] = 0x07;
//		s_unit.date.year;
#endif
		tx[0] = 0xe2;
		tx[1] = s_unit.date.year;	
		tx[2] = s_unit.date.month;	
		tx[3] = s_unit.date.week;	
		tx[4] = s_unit.date.day;	
		tx[5] = s_unit.date.hour;	
		tx[6] = s_unit.date.min;	
		tx[7] = s_unit.date.sec;
		tx[8] =  ( s_unit.max_mukokyu_sec & 0x00ff );
		tx[9] = (( s_unit.max_mukokyu_sec & 0xff00 ) >> 8 );
		
//		s_ds.vuart.input.send_status = ON;
//		R_APP_VUART_Send_Char( &tx[0], 7 );
		main_send_vuart( &tx[0], 10 );
		s_unit.get_mode_status = 3;
		
	
	}else if( 3 == s_unit.get_mode_status ){
		if( s_unit.calc_cnt <= s_unit.get_mode_calc_cnt ){
			
			s_unit.get_mode_status = 4;
		}else{
			// EEP読み出し
			// フレーム位置とデータ位置からEEPアドレスを算出
			rd_adrs = ( s_unit.frame_num_read * EEP_FRAME_SIZE ) + ( s_unit.get_mode_calc_cnt * EEP_CACL_DATA_SIZE );
			
//			i2c_read_sub( EEP_DEVICE_ADR, rd_adrs, &rx[0], EEP_CACL_DATA_SIZE );
			eep_read( rd_adrs, (UB*)&calc_eep, EEP_CACL_DATA_SIZE );
			
			// VUART(BLE)送信
			// スマホのIFに合わせる
			tx[0] = 0xE3;
			tx[1] = calc_eep.info.dat.state;
			tx[2] =  ( calc_eep.info.dat.ibiki_val & 0x00ff );
			tx[3] = (( calc_eep.info.dat.ibiki_val & 0xff00 ) >> 8 );
			tx[4] = calc_eep.info.dat.myaku_val;
			tx[5] = calc_eep.info.dat.spo2_val;
			tx[6] = calc_eep.info.dat.kubi;
			
//			s_ds.vuart.input.send_status = ON;
//			R_APP_VUART_Send_Char( &tx[0], 7 );
			main_send_vuart( &tx[0], 7 );
			main_cpu_com_snd_pc_log( (UB*)&tx[0], CPU_COM_SND_DATA_SIZE_PC_LOG );		// デバッグ
			
			s_unit.get_mode_calc_cnt++;
		}
	}else if( 4 == s_unit.get_mode_status ){
		//読み出し枠番号進める
		INC_MAX_INI(s_unit.frame_num_read, ( EEP_FRAME_MAX - 1), 0);
		if( s_unit.frame_num_write == s_unit.frame_num_read ){
			// 終了
			s_unit.get_mode_status = 5;
		}else{
			//継続
			tx[0] = 0xE0;		// NEXT
//			s_ds.vuart.input.send_status = ON;
//			R_APP_VUART_Send_Char( &tx[0], 4 );
			main_send_vuart( &tx[0], 1 );
			user_main_mode_get_before();
			s_unit.get_mode_status = 1;
		}
	}else if( 5 == s_unit.get_mode_status ){
		wr_adrs = EEP_ADRS_TOP_SETTING;
		eep_write( wr_adrs, &s_unit.frame_num_read, 1, ON );
		tx[0] = 0xE1;		// END
//		s_ds.vuart.input.send_status = ON;
//		R_APP_VUART_Send_Char( &tx[0], 3 );
		main_send_vuart( &tx[0], 1 );
		
		s_unit.get_mode_status = 6;
	}else{
		user_main_mode_get_after();
	}
}

STATIC void user_main_mode_prg_hd(void)
{
	if( PRG_SEQ_READY_WAIT == s_unit.prg_hd_seq ){
		if( 0 == s_unit.timer_sec ){
			//消去コマンド送信
			s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_ERASE;
			s_ds.cpu_com.order.data_size = 0;
			s_unit.prg_hd_seq = PRG_SEQ_IDLE;
		}
	}

	
	
	if( PRG_SEQ_ERASE_WAIT == s_unit.prg_hd_seq ){
		if( 0 == s_unit.timer_sec ){
			// データ送信
			main_prg_hd_read_eep_record();
			s_unit.prg_hd_seq = PRG_SEQ_IDLE;
		}
	}
	
	if( PRG_SEQ_START_WAIT == s_unit.prg_hd_seq ){
		if( 0 == s_unit.timer_sec ){
			//確認コマンド送信
			s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_CHECK;
			s_ds.cpu_com.order.data_size = 0;
			s_unit.prg_hd_seq = PRG_SEQ_IDLE;
		}
	}
}



STATIC void user_main_req_cyc( void )
{
	ke_time_t now_time;
	
	now_time = ke_time();
	
	// ステータス要求送信周期
	if(( now_time - s_unit.last_time ) >= MAIN_STATUS_REQ_TIME ){
		s_unit.last_time = now_time;
		main_cpu_com_snd_sts_req();	/* ステータス要求 */
	}
}


// =============================================================
// CPU通信関連
// =============================================================

STATIC void main_cpu_com_snd_sts_req( void )
{
	
	if( CPU_COM_SND_STATUS_IDLE == s_ds.cpu_com.input.cpu_com_send_status ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS_REQ;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode;
		s_ds.cpu_com.order.snd_data[1] = 0;
		s_ds.cpu_com.order.snd_data[2] = 0;
		s_ds.cpu_com.order.snd_data[3] = 0;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		
//		test_cpu_com_send();

	}else{
		err_info(7);
	}
}


STATIC void main_cpu_com_snd_mode_chg( void )
{
	
	if( CPU_COM_SND_STATUS_IDLE == s_ds.cpu_com.input.cpu_com_send_status ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_MODE_CHG;
		s_ds.cpu_com.order.snd_data[0] = s_unit.next_system_mode;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_MODE_CHG;
	}
}



STATIC void main_cpu_com_snd_pc_log( UB* data, UB size )
{
	int i = 0;
	
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PC_LOG;
	for( i =  0; i < size; i++ ){
		s_ds.cpu_com.order.snd_data[i] = data[i];
	}
	s_ds.cpu_com.order.data_size = size;
}


/************************************************************************/
/* 関数     : main_cpu_com_proc											*/
/* 関数名   : CPU間通信周期処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.06.05  Axia Soft Design 宮本		初版作成			*/
/*          : 2014.06.27  Axia Soft Design 吉居							*/
/*          :						状態に応じて送信トリガを切り換える	*/
/* 			: 2016.05.19  Axia Soft Design 西島　リトライアウト時の処理追加(CPU間通信異常対応) */
/************************************************************************/
/* 機能 : 周期処理														*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_proc(void)
{
	int i = 0;

	/* 受信データチェック */
	if( 0x00 != s_ds.cpu_com.input.rcv_cmd ){
		//何かデータ受信  旧アライブ
//		s_unit.cpu_com_success_once = ON;
//		time_get_elapsed_time( &s_unit.cpu_com_rcv_int_elapsed_time );
		/* 受信コマンド有り */
		for( i=CPU_COM_CMD_NONE; i<CPU_COM_CMD_MAX; i++){
			if( s_cpu_com_rcv_func_tbl[i].cmd == s_ds.cpu_com.input.rcv_cmd ){
				/* 受信コマンドとコマンドテーブルが一致 */
				if( NULL != s_cpu_com_rcv_func_tbl[i].func ){
					/* 受信処理有り */
//					s_cpu_com_rcv_func_tbl[i].func(&s_ds.cpu_com.input.rcv_data[0]);
					s_cpu_com_rcv_func_tbl[i].func();
				}
			}
		}
		// 受信コマンドクリア ※暫定
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
	
}

STATIC void main_cpu_com_rcv_date_set( void )
{
	UB tx[10] = {0};
	
	tx[0] = VUART_CMD_DATE_SET;
	tx[1] = s_ds.cpu_com.input.rcv_data[0];		// CPU間の応答をそのまま入れる
	main_send_vuart( &tx[0], 2 );
	
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
}

STATIC void main_cpu_com_rcv_sts_res( void )
{
	/* 以降要求 */
	if( SYSTEM_MODE_HD_CHG_SENSING == s_ds.cpu_com.input.rcv_data[0] ){
		// 以降状態へ変更
		main_chg_system_mode( SYSTEM_MODE_SENSING );
	}else if( SYSTEM_MODE_HD_CHG_IDLE == s_ds.cpu_com.input.rcv_data[0] ){
		main_chg_system_mode( SYSTEM_MODE_IDLE );
	}else{
		// 何もしない
	}
	
	//電池状態更新
	s_unit.denchi_sts = 0xAA;
	
	s_unit.date.year	 = s_ds.cpu_com.input.rcv_data[3];
	s_unit.date.month	 = s_ds.cpu_com.input.rcv_data[4];
	s_unit.date.week	 = s_ds.cpu_com.input.rcv_data[5];
	s_unit.date.day		 = s_ds.cpu_com.input.rcv_data[6];
	s_unit.date.hour	 = s_ds.cpu_com.input.rcv_data[7];
	s_unit.date.min		 = s_ds.cpu_com.input.rcv_data[8];
	s_unit.date.sec		 = s_ds.cpu_com.input.rcv_data[9];
}

STATIC void main_cpu_com_rcv_sensor_res( void )
{
//	ke_time_t now_time;
	uint8_t *ke_msg;
	MEAS meas;
	
	// 受信日時格納
	s_unit.last_sensing_data_rcv = ke_time();
	
	// センサーデータ格納
	memcpy( &meas.info.byte[0], &s_ds.cpu_com.input.rcv_data[0], CPU_COM_SND_DATA_SIZE_SENSOR_DATA );
	
	if( s_unit.sekigai_cnt < MEAS_SEKIGAI_CNT_MAX ){
		s_unit.sekigai_val[s_unit.sekigai_cnt] = meas.info.dat.sekigaival;
	}
	if( s_unit.sekishoku_cnt < MEAS_SEKISHOKU_CNT_MAX ){
		s_unit.sekishoku_val[s_unit.sekishoku_cnt] = meas.info.dat.sekishoku_val;
	}
	if( s_unit.kokyu_cnt < MEAS_KOKYU_CNT_MAX ){
		s_unit.kokyu_val[s_unit.kokyu_cnt] = meas.info.dat.kokyu_val;
	}
	if( s_unit.ibiki_cnt < MEAS_IBIKI_CNT_MAX ){
		s_unit.ibiki_val[s_unit.ibiki_cnt] = meas.info.dat.ibiki_val;
	}
	if( s_unit.acl_cnt < MEAS_ACL_CNT_MAX ){
		s_unit.acl_x[s_unit.acl_cnt] = meas.info.dat.acl_x;
		s_unit.acl_y[s_unit.acl_cnt] = meas.info.dat.acl_y;
		s_unit.acl_z[s_unit.acl_cnt] = meas.info.dat.acl_z;
	}
	
	
	// データフルで演算呼出
	if( s_unit.sekishoku_cnt >= ( DATA_SIZE_SPO2 - 1 )){
		// 赤色→赤外の順番
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_SEKISHOKU, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	if( s_unit.sekigai_cnt >= ( DATA_SIZE_SPO2 - 1 )){
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_SEKIGAI, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	if( s_unit.kokyu_cnt >= ( DATA_SIZE_APNEA - 1 )){
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_KOKYU, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}

	if( s_unit.ibiki_cnt >= ( DATA_SIZE_APNEA - 1 )){
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_IBIKI, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	INC_MAX( s_unit.sekigai_cnt, MEAS_SEKIGAI_CNT_MAX );
	INC_MAX( s_unit.sekishoku_cnt, MEAS_SEKISHOKU_CNT_MAX );
	INC_MAX( s_unit.kokyu_cnt, MEAS_KOKYU_CNT_MAX );		
	INC_MAX( s_unit.ibiki_cnt, MEAS_IBIKI_CNT_MAX );		
	INC_MAX( s_unit.acl_cnt, MEAS_ACL_CNT_MAX );

	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	
	
	
	
}

STATIC void main_cpu_com_rcv_mode_chg( void )
{
	if( SYSTEM_MODE_MOVE != s_unit.system_mode){
		return;
	}
	
	if( 0 == s_ds.cpu_com.input.rcv_data[0] ){		// 正常終了
		s_unit.system_mode = s_unit.next_system_mode;		// モード変更
		
	}else{
		s_unit.system_mode = SYSTEM_MODE_IDLE;				// モード変更
		s_unit.next_system_mode = SYSTEM_MODE_IDLE;			// モード変更
		err_info(5);
	}
	
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		user_main_mode_sensing_before();
	}
	if( SYSTEM_MODE_IDLE == s_unit.system_mode ){
		user_main_mode_sensing_after();
	}
	
	if( SYSTEM_MODE_PRG_HD == s_unit.system_mode ){
		user_main_mode_prg_hd_before();
	}
	
	
	
}

// モード変更
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode )
{
	s_unit.next_system_mode = next_mode;
	s_unit.system_mode = SYSTEM_MODE_MOVE;
}

/************************************************************************/
/* 関数     : main_vuart_proc											*/
/* 関数名   : VUART周期処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 : 周期処理														*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_vuart_proc(void)
{
	// RD8001暫定：受信テーブル化に修正
	if( 0 != s_ds.vuart.input.rcv_len  ){
		if(( 2 == s_ds.vuart.input.rcv_len  ) &&
		         ( VUART_CMD_MODE_CHG == s_ds.vuart.input.rcv_data[0] )){
			if( 2 == s_ds.vuart.input.rcv_data[1] ){
				main_chg_system_mode( SYSTEM_MODE_PRG_HD );
				main_cpu_com_snd_mode_chg();
			}else if( 3 == s_ds.vuart.input.rcv_data[1] ){
				main_cpu_com_rcv_get_mode();
			}else if( 4 == s_ds.vuart.input.rcv_data[1] ){
				main_vuart_rcv_set_mode();
			}else{
				// 何もしない
			}
		}else if(( VUART_CMD_LEN_DATA_FRAME == s_ds.vuart.input.rcv_len  ) &&
		         ( VUART_CMD_DATA_FRAME == s_ds.vuart.input.rcv_data[0] )){
			main_vuart_rcv_data_frame();
			
		}else if(( VUART_CMD_LEN_DATA_CALC == s_ds.vuart.input.rcv_len  ) &&
		         ( VUART_CMD_DATA_CALC == s_ds.vuart.input.rcv_data[0] )){
			main_vuart_rcv_data_calc();
			
		}else if(( VUART_CMD_LEN_DATA_END == s_ds.vuart.input.rcv_len  ) &&
		         ( VUART_CMD_DATA_END == s_ds.vuart.input.rcv_data[0] )){
			main_vuart_rcv_data_end();
		}else if(( VUART_CMD_LEN_DATE_SET == s_ds.vuart.input.rcv_len  ) &&
		         ( VUART_CMD_DATE_SET == s_ds.vuart.input.rcv_data[0] )){
			main_vuart_rcv_date();

		}else if( VUART_CMD_LEN_PRG_DATA == s_ds.vuart.input.rcv_len  ){
			// プログラムデータ書き換え
			main_prg_hd_eep_code_record();
		}else if(( 5 == s_ds.vuart.input.rcv_len  ) &&
		         ( VUART_CMD_PRG_RESULT == s_ds.vuart.input.rcv_data[0] )){
			main_prg_hd_result();
		}else if(( 1 == s_ds.vuart.input.rcv_len  ) && 
		         ( VUART_CMD_PRG_CHECK == s_ds.vuart.input.rcv_data[0] )){
			main_prg_hd_update();
		}else{
			// 該当コマンドなし
			
		}
		
		
		
		
		
		
		
		// 受信長クリア
		s_ds.vuart.input.rcv_len = 0;
	}
}

/************************************************************************/
/* 関数     : ds_set_vuart_data											*/
/* 関数名   : CPU間通信データセット										*/
/* 引数     : CPU間通信データ格納ポインタ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.06.05 Axia Soft Design 宮本 和幹	初版作成			*/
/*          : 2014.06.12 Axia Soft Design 吉居							*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信ミドルデータセット取得										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_send_vuart( UB *p_data, UB len )
{
	// Vuart送信中は
	if( ON == s_ds.vuart.input.send_status ){
//		err_info(1);
		return;
	}
	
	s_ds.vuart.input.send_status = ON;
	R_APP_VUART_Send_Char( (char *)p_data, len );
}

STATIC void main_cpu_com_rcv_get_mode( void )
{
	if( SYSTEM_MODE_IDLE != s_unit.system_mode){
#if 0
		// RD8001暫定：デバッグ
		{
			UB tx[10];
			tx[0] = 'S';
			tx[1] = 'T';
			tx[2] = 'A';
			tx[3] = 'R';
			tx[4] = 'T';
			tx[5] = 0xDD;		//RD8001暫定：データなし
			s_ds.vuart.input.send_status = ON;
			R_APP_VUART_Send_Char( &tx[0], 6 );
		}
#endif
//		return;
	}
	{
		UB tx[10] = {0};
		
		// OK応答
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = 0x00;
		
		s_ds.vuart.input.send_status = OFF;
		main_send_vuart( &tx[0], 2 );
	}

	

#if 0
	if( OFF == user_main_mode_get_before() ){
		return;		// デバッグ無効
	}
#endif

#if 0
	s_unit.date.year	= (( s_ds.vuart.input.rcv_data[4] << 8 ) + s_ds.vuart.input.rcv_data[3] );
	s_unit.date.month	= s_ds.vuart.input.rcv_data[5];
	s_unit.date.day		= s_ds.vuart.input.rcv_data[6];
	s_unit.date.hour	= s_ds.vuart.input.rcv_data[7];
	s_unit.date.min		= s_ds.vuart.input.rcv_data[8];
	s_unit.date.sec		= s_ds.vuart.input.rcv_data[9];
#endif
	// 暫定
#if 0
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DATE_SET;
	s_ds.cpu_com.order.snd_data[0] = (s_ds.vuart.input.rcv_data[3] % 100);
	s_ds.cpu_com.order.snd_data[1] = s_ds.vuart.input.rcv_data[5];
	s_ds.cpu_com.order.snd_data[2] = 1;
	s_ds.cpu_com.order.snd_data[3] = s_ds.vuart.input.rcv_data[6];
	s_ds.cpu_com.order.snd_data[4] = s_ds.vuart.input.rcv_data[7];
	s_ds.cpu_com.order.snd_data[5] = s_ds.vuart.input.rcv_data[8];
	s_ds.cpu_com.order.snd_data[6] = s_ds.vuart.input.rcv_data[9];
	
	s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DATE_SET;
#endif
	
	s_unit.system_mode = SYSTEM_MODE_GET;
	s_unit.get_mode_status = 0;
//	s_ds.vuart.input.send_status = OFF;
}


void main_vuart_rcv_set_mode( void )
{
	UB tx[10] = {0};
	
	
	// OK応答
	tx[0] = VUART_CMD_MODE_CHG;
	tx[1] = 0x00;
	
	main_send_vuart( &tx[0], 2 );

	s_unit.calc_cnt = 0;
}

void main_vuart_rcv_data_frame( void )
{
	UW wr_adrs;
	
	s_unit.date.year = s_ds.vuart.input.rcv_data[1];
	s_unit.date.month = s_ds.vuart.input.rcv_data[2];
	s_unit.date.week = s_ds.vuart.input.rcv_data[3];
	s_unit.date.day = s_ds.vuart.input.rcv_data[4];
	s_unit.date.hour = s_ds.vuart.input.rcv_data[5];
	s_unit.date.min = s_ds.vuart.input.rcv_data[6];
	s_unit.date.sec = s_ds.vuart.input.rcv_data[7];
	
	s_unit.max_mukokyu_sec  = s_ds.vuart.input.rcv_data[9] << 8;
	s_unit.max_mukokyu_sec  |= s_ds.vuart.input.rcv_data[8];
	
	// 日時情報書き込み
	wr_adrs = ( s_unit.frame_num_write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
	eep_write( wr_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE, ON );
	
	// 無呼吸時間書き込み
	wr_adrs = ( s_unit.frame_num_write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MUKOKYU_TIME_SIZE, ON );
	
}
void main_vuart_rcv_data_calc( void )
{

	s_unit.calc.info.dat.state = s_ds.vuart.input.rcv_data[1];
	s_unit.calc.info.dat.ibiki_val = s_ds.vuart.input.rcv_data[3];
	s_unit.calc.info.dat.ibiki_val <<= 8;
	s_unit.calc.info.dat.ibiki_val += s_ds.vuart.input.rcv_data[2];
	s_unit.calc.info.dat.myaku_val = s_ds.vuart.input.rcv_data[4];
	s_unit.calc.info.dat.spo2_val = s_ds.vuart.input.rcv_data[5];
	s_unit.calc.info.dat.kubi = s_ds.vuart.input.rcv_data[6];
	
//	memcpy(&s_unit.calc.info.byte[0], &s_ds.vuart.input.rcv_data[1], ( VUART_CMD_LEN_DATA_CALC - VUART_CMD_ONLY_SIZE ));
	user_main_calc_result();
}


void main_vuart_rcv_data_end( void )
{
	user_main_mode_sensing_after();
}

void main_vuart_rcv_date( void )
{
	s_ds.cpu_com.order.snd_cmd_id  = CPU_COM_CMD_DATE_SET;
	s_ds.cpu_com.order.snd_data[0] = s_ds.vuart.input.rcv_data[1];
	s_ds.cpu_com.order.snd_data[1] = s_ds.vuart.input.rcv_data[2];
	s_ds.cpu_com.order.snd_data[2] = s_ds.vuart.input.rcv_data[3];
	s_ds.cpu_com.order.snd_data[3] = s_ds.vuart.input.rcv_data[4];
	s_ds.cpu_com.order.snd_data[4] = s_ds.vuart.input.rcv_data[5];
	s_ds.cpu_com.order.snd_data[5] = s_ds.vuart.input.rcv_data[6];
	s_ds.cpu_com.order.snd_data[6] = s_ds.vuart.input.rcv_data[7];
	s_ds.cpu_com.order.data_size   = CPU_COM_SND_DATA_SIZE_DATE_SET;
}

/************************************************************************/
/* 関数     : ds_get_cpu_com_order										*/
/* 関数名   : CPU間通信用データ取得										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.05.15 Axia Soft Design 吉居 久和	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信用データ取得												*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data )
{
	*p_data = &s_ds.cpu_com.order;
}
/************************************************************************/
/* 関数     : ds_set_cpu_com_input										*/
/* 関数名   : CPU間通信データセット										*/
/* 引数     : CPU間通信データ格納ポインタ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.06.05 Axia Soft Design 宮本 和幹	初版作成			*/
/*          : 2014.06.12 Axia Soft Design 吉居							*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信ミドルデータセット取得										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data )
{
	s_ds.cpu_com.input = *p_data;
}

/************************************************************************/
/* 関数     : ds_set_vuart_data											*/
/* 関数名   : CPU間通信データセット										*/
/* 引数     : CPU間通信データ格納ポインタ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.06.05 Axia Soft Design 宮本 和幹	初版作成			*/
/*          : 2014.06.12 Axia Soft Design 吉居							*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信ミドルデータセット取得										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void ds_set_vuart_data( UB *p_data, UB len )
{
	s_ds.vuart.input.rcv_len = len;
	memcpy( &s_ds.vuart.input.rcv_data[0], &p_data[0], s_ds.vuart.input.rcv_len );
}

/************************************************************************/
/* 関数     : ds_set_vuart_send_status									*/
/* 関数名   : CPU間通信データセット										*/
/* 引数     : CPU間通信データ格納ポインタ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2014.06.05 Axia Soft Design 宮本 和幹	初版作成			*/
/*          : 2014.06.12 Axia Soft Design 吉居							*/
/************************************************************************/
/* 機能 :																*/
/* CPU間通信ミドルデータセット取得										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void ds_set_vuart_send_status( UB status )
{
	s_ds.vuart.input.send_status = status;
}

// ============================
// 以降演算部の処理
// ============================
static int_t main_calc_sekigai(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	calculator_pulse_oximeter_inf(&s_unit.sekigai_val[0]);
	s_unit.sekigai_cnt = 0;
	s_unit.calc.info.dat.spo2_val = get_spo2();
#else
	// ダミーデータ
	s_unit.sekigai_cnt = 0;
	s_unit.calc.info.dat.spo2_val = (UB)s_unit.sekigai_val[0];
#endif
	return (KE_MSG_CONSUMED);
}


static int_t main_calc_sekishoku(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	calculator_pulse_oximeter_red(&s_unit.sekishoku_val[0]);
	s_unit.sekishoku_cnt = 0;
	s_unit.calc.info.dat.myaku_val = get_sinpak();
#else
	// ダミーデータ
	s_unit.sekishoku_cnt = 0;
	s_unit.calc.info.dat.spo2_val = (UB)s_unit.sekishoku_val[0];
#endif
	
	return (KE_MSG_CONSUMED);
}

static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	calculator_apnea(&s_unit.kokyu_val[0]);
	s_unit.kokyu_cnt = 0;
	s_unit.calc.info.dat.state = get_state();
#endif
	
	return (KE_MSG_CONSUMED);
}


static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	//演算正規処理
	int ii;
	int max = s_unit.ibiki_val[0];
	for(ii=0;ii<s_unit.ibiki_cnt;++ii){
		if(max < s_unit.ibiki_val[ii]){
			max = s_unit.ibiki_val[ii];
		}
	}
	s_unit.calc.info.dat.ibiki_val = max;

	s_unit.ibiki_cnt = 0;

#else
	//デバッグ用ダミー処理
	__no_operation();		// RD8001暫定：ブレイク貼り用
	__no_operation();		// RD8001暫定：ブレイク貼り用
	__no_operation();		// RD8001暫定：ブレイク貼り用
	__no_operation();		// RD8001暫定：ブレイク貼り用
	__no_operation();		// RD8001暫定：ブレイク貼り用





	// ダミーデータ
	s_unit.ibiki_cnt = 0;
	s_unit.calc.info.dat.spo2_val = (UB)s_unit.ibiki_val[0];
#endif

	return (KE_MSG_CONSUMED);
}

static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	B	acc_x;
//	B	acc_y;		//現状未使用
	B	acc_z;
	
	// 最新のデータを使う
	acc_x = s_unit.acl_x[s_unit.acl_cnt];
//	acc_y = s_unit.acl_y[s_unit.acl_cnt];		//現状未使用
	acc_z = s_unit.acl_z[s_unit.acl_cnt];
	
	s_unit.acl_cnt = 0;
	
	// データ格納
#if 0
	// 45°刻み
	if( acc_z > 48 ){
		s_unit.calc.info.dat.kubi = 0;
	}else if( acc_z > 16 ){
		if( acc_x < 0 ){
			s_unit.calc.info.dat.kubi = 1;	// 45°
		}else{
			s_unit.calc.info.dat.kubi = 7;	// 315°
		}
	}else if( acc_z > -16 ){
		if( acc_x < 0 ){
			s_unit.calc.info.dat.kubi = 2;	// 90°
		}else{
			s_unit.calc.info.dat.kubi = 6;	// 270°
		}
	}else if( acc_z > -48 ){
		if( acc_x < 0 ){
			s_unit.calc.info.dat.kubi = 3;	// 180°
		}else{
			s_unit.calc.info.dat.kubi = 5;	// 225°
		}
	}else{
		s_unit.calc.info.dat.kubi = 4;	// 180°
	}
#else
	// 90°刻み ※64～-64の129分割 0の分配が1多い
	if( acc_z >= 32 ){
		s_unit.calc.info.dat.kubi = 0;		// 0°～45°,316°～360°
	}else if( acc_z >= -32 ){
		if( acc_x < 0 ){
			s_unit.calc.info.dat.kubi = 1;	// 46°～135°
		}else{
			s_unit.calc.info.dat.kubi = 3;	// 136°～225°
		}
	}else{
		s_unit.calc.info.dat.kubi = 2;	// 226°～315°
	}
#endif

	
	return (KE_MSG_CONSUMED);
}


// BLE以外のユーザーアプリのスリープチェック
bool user_main_sleep(void)
{
#if FUNC_DEBUG_SLEEP_NON == ON
	return false;		//SLEEP無効
#else
	bool ret = true;
	
	if( ON == drv_intp_read_h1d_int() ){
		ret = false;
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
	}else{
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
	}
	
	if( ON == cpu_com_get_busy() ){
		ret = false;
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
	}else{
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
	}
	
	// RD8001暫定：プログラム書き換え中はスリープし無くて良いか
	if( SYSTEM_MODE_PRG_HD == s_unit.system_mode ){
		ret = false;
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
	}
	
	
	
	
	
	
	if( ret == true ){
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
	}
	
	return ret;
#endif

}


STATIC void user_main_eep_read(void)
{
	eep_read( EEP_ADRS_TOP_SETTING, &s_unit.frame_num_read, 1 );
	eep_read( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num_write, 1 );
	
	// 範囲チェック
	if(( s_unit.frame_num_read > ( EEP_FRAME_MAX - 1)) ||
	   ( s_unit.frame_num_write > ( EEP_FRAME_MAX - 1))){
		err_info(11);
		// 範囲外なら初期化
		s_unit.frame_num_read = 0;
		s_unit.frame_num_write = 0;
		eep_write( EEP_ADRS_TOP_SETTING, &s_unit.frame_num_read, 1, ON );
		eep_write( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num_write, 1, ON );
	}
	
}


// =====================================
// プログラム転送コード
// =====================================
// 島君コード

/*EEPが64Kybte×２面となっているので、
1面に書き込める容量は64*1024÷20=3276.8レコード
0.8レコード(16byte)は使わずに3277レコード目は２面目に
書き込む形ならば境界は気にしなくて良いです。
        */

//---PCコマンド値＆応答値------------
#define	PC_CMD_PROGRAM_TRANSFER				0xD0	//プログラム転送(データ)
	#define	OK_PRG_H1D_EEP_RECODE_STORED		0		//OK(成功)
	#define	NG_PRG_H1D_EEP_RECODE_CNT_OVER		1		//NG(失敗)
//#define	VUART_CMD_PRG_RESULT		0xD1	//プログラム転送結果
	#define	OK_PRG_H1D_EEP_RECODE_COMPLETED	0		//OK(成功)
	#define	NG_PRG_H1D_EEP_RECODE_SUM_UNMATCH	1		//異常(サム値異常)
	#define	NG_PRG_H1D_EEP_RECODE_FLASH		2		//フラッシュ異常
//#define	VUART_CMD_PRG_CHECK		0xD3	//プログラム転送結果確認

	
//---CPU_COMコマンド値------------
#define	CPU_COM_CMD_PROGRAM_AREA_ERASE			0xD4	//プログラム領域消去
	#define	OK_FLASH_ERASED						0		//OK(成功)
	#define	NG_FLASH_ERASE_FAILED				1		//NG(失敗)
#define	CPU_COM_CMD_PROGRAM_TRANSFER			PC_CMD_PROGRAM_TRANSFER	//プログラム転送(データ)
	#define	CPU_COM_PROGRAM_ADRESS_SIZE			4		//[Byte]
	#define	CPU_COM_PROGRAM_RECODE_SIZE_MAX		256		//[Byte]
	#define	OK_FLASH_WRITED						0		//OK(成功)
	#define	NG_FLASH_WRITE_FAILED				1		//NG(失敗)
#define	CPU_COM_PROGRAM_TRANSFER_RESULT			VUART_CMD_PRG_RESULT	//プログラム転送結果
	#define	CPU_COM_PROGRAM_SUM_SIZE			4		//[Byte]
	#define	OK_FLASH_CODE_RECODE_COMPLETED		OK_PRG_H1D_EEP_RECODE_COMPLETED		//OK(成功)
	#define	NG_FLASH_CODE_RECODE_SUM_UNMATCH	NG_PRG_H1D_EEP_RECODE_SUM_UNMATCH		//異常(サム値異常)
	#define	NG_FLASH_CODE_RECODE_FLASH			NG_PRG_H1D_EEP_RECODE_FLASH			//フラッシュ異常
#define	CPU_COM_PROGRAM_UPDATE_START			0xD2		//プログラム更新開始実行
	#define	OK_UPDATE_STARTED					0		//OK(成功)
	#define	NG_UPDATE_START_FAILED				1		//NG(失敗)
#define	CPU_COM_PROGRAM_UPDATE_FIX_CHECK		VUART_CMD_PRG_CHECK		//プログラム更新完了確認

//static UB s_unit.prg_hd_version[VERSION_NUM] = {0};			//h1DアプリのVer
//static UB s_unit.prg_hd_update_state = OK_NOW_UPDATING;			//H1D更新状態

//PCから受信した1レコード分書き込みする
//eep_write内でデータコピーしており、受信データ長も完全固定で０充填等もないため一時領域は不要
//メモ：s_ds.vuart.input.rcv_dataは引数で貰った方が良いかも
STATIC UB main_prg_hd_eep_code_record( void )
{
	UW adr;
	
	if( SYSTEM_MODE_PRG_HD != s_unit.system_mode){
		return NG;
	}
	if( s_unit.prg_hd_eep_record_cnt_wr >= PRG_H1D_EEP_RECODE_CNT_MAX ){
		return NG;
	}
	
	adr = ( s_unit.prg_hd_eep_record_cnt_wr * ( PRG_H1D_EEP_RECODE_UNIT + PRG_H1D_EEP_RECODE_OFFSET) );
	eep_write( adr, &s_ds.vuart.input.rcv_data[0], PRG_H1D_EEP_RECODE_UNIT, ON );
#if 0	//ベリファイ
	eep_read( adr, &s_ds.cpu_com.order.snd_data[0], PRG_H1D_EEP_RECODE_UNIT );
	if (memcmp( &s_ds.vuart.input.rcv_data[0], &s_ds.cpu_com.order.snd_data[0], PRG_H1D_EEP_RECODE_UNIT) != 0){
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		__no_operation();		// RD8001暫定：ブレイク貼り用
		
		
		
		
	}
#endif
	// サム値計算 ※データ
	calc_sum_uw_cont( &s_unit.prg_hd_eep_code_record_sum, &s_ds.vuart.input.rcv_data[4], PRG_H1D_EEP_RECODE_UNIT -4 );
	s_unit.prg_hd_eep_record_cnt_wr++;

	return OK;
}


STATIC void main_prg_hd_result(void)
{
	//サム値組立
//	UB i;
	UB ret;
	UW recv_sum = 0;
	
	
	recv_sum   = (UW)(s_ds.vuart.input.rcv_data[4]);
	recv_sum <<= (UW)8;
	recv_sum += (UW)(s_ds.vuart.input.rcv_data[3]);
	recv_sum <<= (UW)8;
	recv_sum += (UW)(s_ds.vuart.input.rcv_data[2]);
	recv_sum <<= (UW)8;
	recv_sum += (UW)(s_ds.vuart.input.rcv_data[1]);
	
	
//	for(i=1; i<5; i++){
//		recv_sum <<= 8;
//		recv_sum += s_ds.vuart.input.rcv_data[i];
//	}
	
	//サム値比較 RD8001暫定：EEPチェックに変えたい？
	if( s_unit.prg_hd_eep_code_record_sum != recv_sum ){
		ret = NG_PRG_H1D_EEP_RECODE_SUM_UNMATCH;
	}else{
		ret = OK_PRG_H1D_EEP_RECODE_COMPLETED;
	}
	
	//---応答---
	{
		UB tx[10] = {0};
		tx[0] = VUART_CMD_PRG_RESULT;
		tx[1] = ret;
	
		main_send_vuart( &tx[0], 2 );
	}
	
	if( OK_PRG_H1D_EEP_RECODE_COMPLETED == ret ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_READY;
		s_ds.cpu_com.order.data_size = 0;
	}

}

//【PCからの受信コマンド対応動作】プログラム更新完了確認
STATIC void main_prg_hd_update(void)
{
//	UB i;
//	UB loop_cnt;
	
	
#if FUNC_DEBUG_PRG_H1D_U == ON
	{
		static UB dbg_prg_hd_update_flg = 0;
		
		if( 0 == dbg_prg_hd_update_flg){
			
			dbg_prg_hd_update_flg = 1;
			
		}else{
			//2回に1回OK返す
			s_unit.prg_hd_update_state = OK_UPDATE_FIX;
			dbg_prg_hd_update_flg = 0;
		}
	}
#endif
	
	//---応答---
	{
		UB tx[10] = {0};
		tx[0] = VUART_CMD_PRG_CHECK;
		tx[1] = s_unit.prg_hd_update_state;
		tx[2] = s_unit.prg_hd_version[0];
		tx[3] = s_unit.prg_hd_version[1];
		tx[4] = s_unit.prg_hd_version[2];
		tx[5] = s_unit.prg_hd_version[3];
	
		main_send_vuart( &tx[0], 6 );
	}
}



/* 【CPU間通信コマンド】プログラム転送準備		*/
STATIC void main_cpu_com_rcv_prg_hd_ready(void)
{
	s_unit.timer_sec = TIMER_SEC_PRG_READY_WAIT;
	s_unit.prg_hd_seq = PRG_SEQ_READY_WAIT;
	
}

/* 【CPU間通信コマンド】プログラム転送開始		*/
STATIC void main_cpu_com_rcv_prg_hd_start(void)
{
	s_unit.timer_sec = TIMER_SEC_PRG_START_WAIT;
	s_unit.prg_hd_seq = PRG_SEQ_START_WAIT;
}




/* 【CPU間通信コマンド】プログラム転送消去		*/
STATIC void main_cpu_com_rcv_prg_hd_erase(void)
{
	s_unit.timer_sec = TIMER_SEC_PRG_ERASE_WAIT;
	s_unit.prg_hd_seq = PRG_SEQ_ERASE_WAIT;
}
	
/* 【CPU間通信コマンド】プログラム転送データ	*/
STATIC void main_cpu_com_rcv_prg_hd_data(void)
{
	if( s_unit.prg_hd_eep_record_cnt_rd < s_unit.prg_hd_eep_record_cnt_wr ){
		// 継続
		main_prg_hd_read_eep_record();
	}else{
		// 終了
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_RESLUT;
		// RD8001暫定：サム値格納　※値未確認
		s_ds.cpu_com.order.snd_data[0] = (UB)(  s_unit.prg_hd_eep_code_record_sum & (UW)0x000000FF );
		s_ds.cpu_com.order.snd_data[1] = (UB)(( s_unit.prg_hd_eep_code_record_sum & (UW)0x0000FF00 ) >> (UW)8 );
		s_ds.cpu_com.order.snd_data[2] = (UB)(( s_unit.prg_hd_eep_code_record_sum & (UW)0x00FF0000 ) >> (UW)16 );
		s_ds.cpu_com.order.snd_data[3] = (UB)(( s_unit.prg_hd_eep_code_record_sum & (UW)0xFF000000 ) >> (UW)24 );
		s_ds.cpu_com.order.data_size = 4;
	}
}


/* 【CPU間通信コマンド】プログラム転送結果		*/
STATIC void main_cpu_com_rcv_prg_hd_reslut(void)
{
	if( 0 == s_ds.cpu_com.input.rcv_data[0] ){
		// 正常
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_START;
		s_ds.cpu_com.order.data_size = 0;
	}else{
		// 異常
		s_unit.prg_hd_update_state = PRG_HD_UPDATE_STATE_NG;
	}
}

/* 【CPU間通信コマンド】プログラム転送確認		*/
STATIC void main_cpu_com_rcv_prg_hd_check(void)
{
	// バージョン格納
	s_unit.prg_hd_version[0] = s_ds.vuart.input.rcv_data[0];
	s_unit.prg_hd_version[1] = s_ds.vuart.input.rcv_data[1];
	s_unit.prg_hd_version[2] = s_ds.vuart.input.rcv_data[2];
	s_unit.prg_hd_version[3] = s_ds.vuart.input.rcv_data[3];
	
	// 正常終了
	s_unit.prg_hd_update_state = PRG_HD_UPDATE_STATE_OK;
}

//CPUへ送信する1レコード分を準備する
STATIC void main_prg_hd_read_eep_record( void )
{
	UW adr;
	
	adr = ( s_unit.prg_hd_eep_record_cnt_rd * ( PRG_H1D_EEP_RECODE_UNIT + PRG_H1D_EEP_RECODE_OFFSET ) );
	eep_read( adr, &s_ds.cpu_com.order.snd_data[0], PRG_H1D_EEP_RECODE_UNIT );
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_DATA;
	s_ds.cpu_com.order.data_size = PRG_H1D_EEP_RECODE_UNIT;
	s_unit.prg_hd_eep_record_cnt_rd++;
}


