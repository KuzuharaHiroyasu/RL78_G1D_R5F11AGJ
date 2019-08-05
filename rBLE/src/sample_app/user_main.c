/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : user_main.c													*/
/* 機能         : ユーザーメイン（関数,RAM,ROM定義）							*/
/* 変更履歴		: 2018.01.25 Axia Soft Design 西島 稔	初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/

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

// プロトタイプ宣言
static int_t user_main_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
#if FUNC_DEBUG_LOG == ON
STATIC void make_send_data(char* pBuff);
#endif
static int_t user_main_calc_result_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
STATIC void user_main_calc_data_set_kyokyu_ibiki( void );
STATIC void user_main_calc_data_set_acl( void );
STATIC void user_main_mode_inital(void);
STATIC void user_main_mode_idle_rest(void);
STATIC void user_main_mode_idle_com(void);
STATIC void user_main_mode_sensing(void);
STATIC void user_main_mode_move(void);
STATIC void user_main_mode_get(void);
STATIC void user_main_mode_prg_h1d(void);
STATIC void user_main_mode_prg_g1d(void);
STATIC void user_main_req_cyc( void );
STATIC UB main_cpu_com_snd_sts_req( void );
STATIC void main_cpu_com_snd_mode_chg( void );
STATIC void main_cpu_com_proc(void);
STATIC void main_cpu_com_rcv_sts_res( void );
STATIC void main_cpu_com_rcv_sensor_res( void );
STATIC void main_cpu_com_rcv_mode_chg( void );
STATIC void main_mode_chg( void );				// ■暫定
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode );
STATIC void user_main_mode( void );
STATIC void user_main_mode_common( void );
//STATIC void main_cpu_com_snd_pc_log( UB* data, UB size );
//STATIC void main_cpu_com_snd_sensing_order( UB sekigai );
STATIC void main_vuart_proc(void);
STATIC void main_vuart_rcv_mode_chg( void );
STATIC void main_vuart_rcv_date( void );
STATIC void main_vuart_rcv_info( void );
STATIC void main_vuart_rcv_version( void );
STATIC void main_vuart_rcv_device_info( void );
STATIC void sw_proc(void);
STATIC void user_main_calc_result( void );
STATIC void user_main_mode_sensing_before( void );
STATIC void user_main_mode_sensing_after( void );
STATIC UB user_main_mode_get_frame_before( void );
STATIC void user_main_mode_self_check( void );
STATIC UB evt_act( EVENT_NUM evt );
STATIC SYSTEM_MODE evt_non( int evt);
STATIC SYSTEM_MODE evt_idle_rest( int evt);
STATIC SYSTEM_MODE evt_idle_com( int evt);
STATIC SYSTEM_MODE evt_idle_com_denchi( int evt);
STATIC SYSTEM_MODE evt_sensing( int evt);
STATIC SYSTEM_MODE evt_sensing_chg( int evt);
STATIC SYSTEM_MODE evt_initial( int evt);
STATIC SYSTEM_MODE evt_initial_chg( int evt);
STATIC SYSTEM_MODE evt_get( int evt);
STATIC SYSTEM_MODE evt_h1d_prg_denchi( int evt);
STATIC SYSTEM_MODE evt_g1d_prg_denchi( int evt);
STATIC SYSTEM_MODE evt_self_check( int evt);
STATIC SYSTEM_MODE evt_prg_h1d_fin( int evt);
STATIC SYSTEM_MODE evt_prg_h1d_time_out( int evt);
STATIC void user_main_mode_get_after( void );
STATIC void user_main_eep_read_pow_on(void);
//STATIC void eep_all_erase( void );		//未使用関数
STATIC void eep_part_erase( void );
STATIC void main_vuart_send( UB *p_data, UB len );
STATIC void main_vuart_rcv_prg_hd_record( void );
STATIC void main_vuart_rcv_prg_hd_result(void);
STATIC void main_vuart_rcv_prg_hd_update(void);
STATIC void main_cpu_com_rcv_prg_hd_ready(void);
STATIC void main_cpu_com_rcv_prg_hd_start(void);
STATIC void main_cpu_com_rcv_prg_hd_erase(void);
STATIC void main_cpu_com_rcv_prg_hd_data(void);
STATIC void main_cpu_com_rcv_prg_hd_reslut(void);
STATIC void main_cpu_com_rcv_prg_hd_check(void);
STATIC void main_prg_hd_read_eep_record( void );
STATIC void main_cpu_com_rcv_date_set( void );
STATIC void main_cpu_com_rcv_disp_order( void );
STATIC void main_cpu_com_rcv_version( void );
void main_vuart_set_mode( void );
void main_vuart_rcv_data_frame( void );
void main_vuart_rcv_data_calc( void );
void main_vuart_rcv_data_end( void );
STATIC void main_vuart_rcv_data_fin( void );
void main_vuart_rcv_date( void );
void main_vuart_rcv_device_set( void );
//void main_vuart_rcv_alarm_set( void );
//void main_vuart_snd_alarm_info( UB type, UB data );

//STATIC void AlarmSnore(UB oldstate, UB newstate);
//STATIC void AlarmApnea(UB oldstate, UB newstate);

static int_t led_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t battery_level_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_photoref(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

// ACL関連
STATIC void main_acl_init(void);
STATIC void main_acl_stop(void);
STATIC void main_acl_start(void);
STATIC void main_acl_read(void);

// 以降演算部の処理
//static int_t main_calc_sekigai(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
//static int_t main_calc_sekishoku(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

// フォトセンサー
STATIC UH main_photo_read(void);

/********************/
/*     外部参照     */
/********************/
//fw update ※G1Dアップデート開始処理
extern RBLE_STATUS FW_Update_Receiver_Start( void );

/********************/
/*     変数定義     */
/********************/
/* Status */// プラットフォーム
ke_state_t user_main_state[ USER_MAIN_IDX_MAX ] = {0};


STATIC T_UNIT s_unit;
STATIC DS s_ds;

static UB act_mode = ACT_MODE_NORMAL;

/********************/
/*     定数定義     */
/********************/
#include	"user_main_tbl.h"		// ユーザーテーブル実体定義

/************************************************************************/
/* 関数     : app_evt_usr_1												*/
/* 関数名   : ユーザーイベント(10ms周期)								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.05.13 Axia Soft Design mmura		初版作成			*/
/************************************************************************/
/* 機能 : ユーザーイベント(10ms周期)									*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void codeptr app_evt_usr_1(void) 
{ 
	ke_evt_clear(KE_EVT_USR_1_BIT); 

#if FUNC_DEBUG_LOG == ON
	{
		uint8_t *ke_msg;

		ke_msg = ke_msg_alloc( USER_MAIN_CYC_ACT, USER_MAIN_ID, USER_MAIN_ID, 0 );

		ke_msg_send(ke_msg);
	}
#endif
}

/************************************************************************/
/* 関数     : app_evt_usr_2												*/
/* 関数名   : ユーザーイベント(1秒周期)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : ユーザーイベント(1秒周期)										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void codeptr app_evt_usr_2(void) 
{ 
	uint8_t *ke_msg;
	
	ke_evt_clear(KE_EVT_USR_2_BIT);
	
#if FUNC_DEBUG_LOG != ON
	// 秒タイマーカウントダウン
	DEC_MIN( s_unit.timer_sec ,0 );
	
#if 1
	// RD8001暫定：G1Dダウンロード_デバッグ用バージョン送信
	if( ON == s_unit.prg_g1d_send_ver_flg ){
		DEC_MIN( s_unit.prg_g1d_send_ver_sec ,0 );
		if( 0 == s_unit.prg_g1d_send_ver_sec ){
			s_unit.prg_g1d_send_ver_flg = OFF;
			{
				UB tx[VUART_DATA_SIZE_MAX];
				tx[0] = version_product_tbl[0];
				tx[1] = version_product_tbl[1];
				tx[2] = version_product_tbl[2];
				tx[3] = version_product_tbl[3];
				main_vuart_send( &tx[0], 4 );
			}
		}
	}
#endif
	
	s_unit.sec10_led_cnt++;
	led_start(s_unit.sec10_led_cnt);
	
	// RD8001:加速度演算暫定追加 ■暫定
//	ke_msg = ke_msg_alloc( USER_MAIN_CALC_ACL, USER_MAIN_ID, USER_MAIN_ID, 0 );
//	ke_msg_send(ke_msg);
	
	// 電池残量取得(10分周期)
	s_unit.sec600_cnt++;
	if(s_unit.sec600_cnt >= BAT_LEVEL_GET_CYC)
	{
		s_unit.sec600_cnt = 0;
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_BATTERY, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	
	if( SYSTEM_MODE_SENSING != s_unit.system_mode ){
		return;
	}


	s_unit.sec30_cnt++;
	if( s_unit.sec30_cnt >= CALC_RESULT_WR_CYC ){		// 30秒
		s_unit.sec30_cnt = 0;
		
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_CALC_RESULT, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	// 加速度演算
	ke_msg = ke_msg_alloc( USER_MAIN_CALC_ACL, USER_MAIN_ID, USER_MAIN_ID, 0 );
	ke_msg_send(ke_msg);
	
	// フォトセンサ値
	ke_msg = ke_msg_alloc( USER_MAIN_CYC_PHOTOREF, USER_MAIN_ID, USER_MAIN_ID, 0 );
	ke_msg_send(ke_msg);
	
#endif
}

/************************************************************************/
/* 関数     : app_evt_usr_2												*/
/* 関数名   : ユーザーイベント(20ms周期)								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : ユーザーイベント(20ms周期)									*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void codeptr app_evt_usr_3(void) 
{ 
	ke_evt_clear(KE_EVT_USR_3_BIT); 

#if FUNC_DEBUG_LOG != ON
	{
		uint8_t *ke_msg;

		ke_msg = ke_msg_alloc( USER_MAIN_CYC_ACT, USER_MAIN_ID, USER_MAIN_ID, 0 );

		ke_msg_send(ke_msg);
	}
#endif
}

/************************************************************************/
/* 関数     : user_main_cyc												*/
/* 関数名   : ユーザーアプリ周期処理									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : ユーザーアプリ周期処理										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
static int_t user_main_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_LOG == ON
	char dbg_tx_data[50] = {0};
	char dummydata[] = "abcdefghijk\r\n";
	int dbg_len = sizeof(dummydata);
	com_srv_send(dummydata, dbg_len);
#else
	sw_proc();				// SW検知処理
	
	cpu_com_proc();			// CPU間通信サービス
	
	main_vuart_proc();		// VUART通信サービス
	
	main_cpu_com_proc();	// 通信サービスアプリ
	
	user_main_mode();		// メインモード処理
#endif

	return (KE_MSG_CONSUMED);
}


/************************************************************************/
/* 関数     : user_main_calc_result_cyc									*/
/* 関数名   : ユーザーアプリ演算結果周期処理							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : ユーザーアプリ演算結果周期処理								*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
static int_t user_main_calc_result_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	user_main_calc_result();	// 演算結果
	
	return (KE_MSG_CONSUMED);
}



/************************************************************************/
/* 関数     : user_main_timer_10ms_set									*/
/* 関数名   : 10msタイマーカウントセット								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 10msタイマーカウントセット									*/
/************************************************************************/
/* 注意事項 :															*/
/* �@スリープ中でも有効なタイマー										*/
/************************************************************************/
void user_main_timer_10ms_set( void )
{
	s_unit.tick_10ms++;
	s_unit.tick_10ms_sec++;
	s_unit.tick_10ms_new++;
	s_unit.elapsed_time++;
	s_unit.tick_vib_10ms_sec++;
}


/************************************************************************/
/* 関数     : user_main_timer_cyc										*/
/* 関数名   : タイマー周期処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : タイマー周期処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void user_main_timer_cyc( void )
{
	if(s_unit.system_mode == SYSTEM_MODE_SENSING)
	{
		// 50ms周期
		if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_50MSEC){
#if FUNC_DEBUG_LOG == ON
			char dbg_tx_data[50] = {0};
			int dbg_len;
			
			// 呼吸音、いびき音取得
			adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
					
			s_unit.acl_timing+=1;
			if(s_unit.acl_timing >= ACL_TIMING_VAL){
				s_unit.acl_timing = 0;
				// 加速度取得
				main_acl_read();
				// フォトセンサー値取得
				s_unit.meas.info.dat.photoref_val = main_photo_read();
			}else{
				s_unit.meas.info.dat.acl_x = 99;
				s_unit.meas.info.dat.acl_y = 99;
				s_unit.meas.info.dat.acl_z = 99;
				s_unit.meas.info.dat.photoref_val = 0;
			}
			
			make_send_data(dbg_tx_data);
			dbg_len = strlen(dbg_tx_data);
			com_srv_send(dbg_tx_data, dbg_len);
#else
	//		ke_evt_set(KE_EVT_USR_1_BIT);
			
			// 呼吸音、いびき音取得
			adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
			user_main_calc_data_set_kyokyu_ibiki();
			
			// 加速度取得
			s_unit.acl_timing+=1;
			if(s_unit.acl_timing >= ACL_TIMING_VAL){
				s_unit.acl_timing = 0;
				main_acl_read();
				user_main_calc_data_set_acl();
				
				// フォトセンサー値取得
				s_unit.meas.info.dat.photoref_val = main_photo_read();
			}
#endif
			s_unit.tick_10ms_new = 0;
		}
	}
	// 20ms周期
	if(s_unit.tick_10ms >= (uint16_t)PERIOD_20MSEC){
		ke_evt_set(KE_EVT_USR_3_BIT);

		s_unit.tick_10ms = 0;
	}
	// 1秒周期 ※遅れの蓄積は厳禁
	if( s_unit.tick_10ms_sec >= (uint16_t)PERIOD_1SEC){
		s_unit.tick_10ms_sec -= PERIOD_1SEC;	// 遅れが蓄積しない様に処理
		ke_evt_set(KE_EVT_USR_2_BIT);
	}
}

/************************************************************************/
/* 関数     : led_cyc													*/
/* 関数名   : 						*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.08.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
static int_t led_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	// LED(10ms周期)
	if(s_unit.tick_led_10ms_sec >= (uint16_t)PERIOD_10MSEC)
	{
   		led_start(s_unit.tick_led_10ms_sec);
	}
	
	return (KE_MSG_CONSUMED);
}

/************************************************************************/
/* 関数     : vib_cyc													*/
/* 関数名   : 						*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.08.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void vib_cyc( void )
{
	// バイブレーション(10ms周期)
	if(s_unit.tick_vib_10ms_sec >= (uint16_t)PERIOD_10MSEC)
	{
   		vib_start(s_unit.tick_vib_10ms_sec);
	}
}

/************************************************************************/
/* 関数     : battery_level_cyc											*/
/* 関数名   : 						*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.08.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
static int_t battery_level_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	// 電池残量取得(10分周期)
	main_set_battery();
	
	return (KE_MSG_CONSUMED);
}

/************************************************************************/
/* 関数     : main_calc_photoref										*/
/* 関数名   : 						*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.08.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
static int_t main_calc_photoref(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	s_unit.calc.info.dat.photoref[s_unit.phase_photoref] = s_unit.meas.info.dat.photoref_val;
	s_unit.phase_photoref++;
	if(s_unit.phase_photoref >= SEC_PHASE_NUM){
		s_unit.phase_photoref = SEC_PHASE_0_10;
	}
	
	return (KE_MSG_CONSUMED);
}

/************************************************************************/
/* 関数     : user_main_calc_data_set_kokyu_ibiki						*/
/* 関数名   : 演算データセット処理(呼吸・いびき)						*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.07.19 Axia Soft Design 和田 耕太	初版作成			*/
/************************************************************************/
/* 機能 : 演算データセット処理(呼吸・いびき)							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_calc_data_set_kyokyu_ibiki( void )
{
	uint8_t *ke_msg;
	
	if( s_unit.kokyu_cnt < MEAS_KOKYU_CNT_MAX ){
		s_unit.kokyu_val[s_unit.kokyu_cnt] = s_unit.meas.info.dat.kokyu_val;
	}
	if( s_unit.ibiki_cnt < MEAS_IBIKI_CNT_MAX ){
		s_unit.ibiki_val[s_unit.ibiki_cnt] = s_unit.meas.info.dat.ibiki_val;
	}
	
	// データフルで演算呼出
	if( s_unit.kokyu_cnt >= ( DATA_SIZE_APNEA - 1 )){
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_KOKYU, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}

	if( s_unit.ibiki_cnt >= ( DATA_SIZE_APNEA - 1 )){
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_IBIKI, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	INC_MAX( s_unit.kokyu_cnt, MEAS_KOKYU_CNT_MAX );		
	INC_MAX( s_unit.ibiki_cnt, MEAS_IBIKI_CNT_MAX );		

	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	
}

/************************************************************************/
/* 関数     : user_main_calc_data_set_acl								*/
/* 関数名   : 演算データセット処理(加速)								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.07.19 Axia Soft Design 和田 耕太	初版作成			*/
/************************************************************************/
/* 機能 : 演算データセット処理(加速)									*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_calc_data_set_acl( void )
{
//	uint8_t *ke_msg;
	
	if( s_unit.acl_cnt < MEAS_ACL_CNT_MAX ){
		s_unit.acl_x[s_unit.acl_cnt] = s_unit.meas.info.dat.acl_x;
		s_unit.acl_y[s_unit.acl_cnt] = s_unit.meas.info.dat.acl_y;
		s_unit.acl_z[s_unit.acl_cnt] = s_unit.meas.info.dat.acl_z;
	}
	
	// 加速の演算はユーザーイベント(1秒周期)で実施する
	
	INC_MAX( s_unit.acl_cnt, MEAS_ACL_CNT_MAX );

	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	
}

#if FUNC_DEBUG_LOG == ON
/************************************************************************/
/* 関数     : make_send_data											*/
/* 関数名   : 送信データ作成処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.05.14 Axia Soft Design mmura		初版作成			*/
/************************************************************************/
/* 機能 : タイマー周期処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void make_send_data(char* pBuff)
{
	// 呼吸音, いびき音, 加速度(X), 加速度(Y), 加速度(Z), フォトセンサー値
	UB tmp;
	UH next;
	UB index = 0;
	
	// 呼吸音
	tmp = s_unit.meas.info.dat.kokyu_val / 1000;
	next = s_unit.meas.info.dat.kokyu_val % 1000;
	pBuff[index++] = '0' + tmp;
	tmp = next / 100;
	next = next % 100;
	pBuff[index++] = '0' + tmp;
	tmp = next / 10;
	next = next % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;
	pBuff[index++] = ',';
	
	// いびき音
	tmp = s_unit.meas.info.dat.ibiki_val / 1000;
	next = s_unit.meas.info.dat.ibiki_val % 1000;
	pBuff[index++] = '0' + tmp;
	tmp = next / 100;
	next = next % 100;
	pBuff[index++] = '0' + tmp;
	tmp = next / 10;
	next = next % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;
	pBuff[index++] = ',';
	
	// 加速度(X)
	if(s_unit.meas.info.dat.acl_x >= 0){
		tmp = s_unit.meas.info.dat.acl_x / 100;
		next = s_unit.meas.info.dat.acl_x % 100;
		pBuff[index++] = '0' + tmp;
		tmp = next / 10;
		next = next % 10;
		pBuff[index++] = '0' + tmp;
		tmp = next % 10;
		pBuff[index++] = '0' + tmp;
		pBuff[index++] = ',';
	}else{
		UB acl_x = -1 * s_unit.meas.info.dat.acl_x;
		pBuff[index++] = '-';
		tmp = acl_x / 100;
		next = acl_x % 100;
		pBuff[index++] = '0' + tmp;
		tmp = next / 10;
		next = next % 10;
		pBuff[index++] = '0' + tmp;
		tmp = next % 10;
		pBuff[index++] = '0' + tmp;
		pBuff[index++] = ',';
	}
	
	// 加速度(Y)
	if(s_unit.meas.info.dat.acl_y >= 0){
		tmp = s_unit.meas.info.dat.acl_y / 100;
		next = s_unit.meas.info.dat.acl_y % 100;
		pBuff[index++] = '0' + tmp;
		tmp = next / 10;
		next = next % 10;
		pBuff[index++] = '0' + tmp;
		tmp = next % 10;
		pBuff[index++] = '0' + tmp;
		pBuff[index++] = ',';
	}else{
		UB acl_y = -1 * s_unit.meas.info.dat.acl_y;
		pBuff[index++] = '-';
		tmp = acl_y / 100;
		next = acl_y % 100;
		pBuff[index++] = '0' + tmp;
		tmp = next / 10;
		next = next % 10;
		pBuff[index++] = '0' + tmp;
		tmp = next % 10;
		pBuff[index++] = '0' + tmp;
		pBuff[index++] = ',';
	}
	
	// 加速度(Z)
	if(s_unit.meas.info.dat.acl_z >= 0){
		tmp = s_unit.meas.info.dat.acl_z / 100;
		next = s_unit.meas.info.dat.acl_z % 100;
		pBuff[index++] = '0' + tmp;
		tmp = next / 10;
		next = next % 10;
		pBuff[index++] = '0' + tmp;
		tmp = next % 10;
		pBuff[index++] = '0' + tmp;
		pBuff[index++] = ',';
	}else{
		UB acl_z = -1 * s_unit.meas.info.dat.acl_z;
		pBuff[index++] = '-';
		tmp = acl_z / 100;
		next = acl_z % 100;
		pBuff[index++] = '0' + tmp;
		tmp = next / 10;
		next = next % 10;
		pBuff[index++] = '0' + tmp;
		tmp = next % 10;
		pBuff[index++] = '0' + tmp;
		pBuff[index++] = ',';
	}
	
	// フォトセンサー
	tmp = s_unit.meas.info.dat.photoref_val / 100;
	next = s_unit.meas.info.dat.photoref_val % 100;
	pBuff[index++] = '0' + tmp;
	tmp = next / 10;
	next = next % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;

	pBuff[index++] = '\r';
	pBuff[index++] = '\n';
}
#endif

/************************************************************************/
/* 関数     : time_get_elapsed_time										*/
/* 関数名   : ソフトウェア経過時間取得処理								*/
/* 引数     : なし														*/
/* 戻り値   : 現在連続稼動時間											*/
/* 変更履歴 : 2012.01.30 Axia Soft Design H.Wada	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* 現在連続稼動時間の取得を行う											*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
UW time_get_elapsed_time( void )
{
	return s_unit.elapsed_time;
}

/************************************************************************/
/* 関数     : user_system_init											*/
/* 関数名   : システム関連初期化										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : システム関連初期化											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void user_system_init( void )
{
	R_INTC_Create();
	R_IT_Create();
	R_RTC_Create();
}

/************************************************************************/
/* 関数     : user_main_init											*/
/* 関数名   : メイン初期化												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : メイン初期化													*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void user_main_init( void )
{
	// ミドル初期化
	cpu_com_init();
	com_srv_init();
	i2c_init();
	eep_init();
	main_acl_init();
	main_acl_start();
	R_ADC_Create();
	
	// メインのデータ初期化
	memset( &s_unit, 0, sizeof(s_unit) );
	
	
	//EEP読み出し
	user_main_eep_read_pow_on();
	
	s_unit.last_time_sts_req = time_get_elapsed_time();
	
	// 状態設定
	ke_state_set(USER_MAIN_ID, 0);
	
	// 演算初期化
	calc_snore_init();
	
	// H1D起動待ち
//	wait_ms(5);
//	s_unit.last_time_sts_req = time_get_elapsed_time();

	// ■暫定 待機モードへ遷移させるために仮に充電ポート
	if( FALSE == evt_act( EVENT_CHG_PORT_ON )){
		NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	}

#if FUNC_DEBUG_PORT == ON
	//空きポートによる計測用設定(スリープなど)
    write1_sfr(PM1,  5, PORT_OUTPUT);
//    write1_sfr(PM1,  6, PORT_OUTPUT);		//使うとBLEが動かなくなるので削除

    write1_sfr(P1, 5, 0);
	
	NO_OPERATION_BREAK_POINT();				// ブレイクポイント設置用

#if 0
	// デバッグ用計測処理
	DI();
	
	write1_sfr(P1, 5, 1);
	{
		UB wait = 200;
	    /* Wait */
	    while (wait--)
	    {
	        ;
	    }
	}
		wait_ms(1);
    write1_sfr(P1, 5, 0);

	wait_ms(5);
	write1_sfr(P1, 5, 1);
#endif

#endif

}


/************************************************************************/
/* 関数     : sw_proc													*/
/* 関数名   : SW周期処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.26 Axia Soft Design 和田 耕太	初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* SW周期処理(20ms周期)													*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
STATIC void sw_proc(void)
{
	UB pow_sw;
	
	pow_sw = drv_i_port_read_pow_sw();
	
	if( ON == pow_sw ){		// ON処理
		// 電源SW押下タイマー継続
		s_unit.sw_time_cnt++;
		
#if FUNC_SW_LONGPUSH_RELEASE == OFF
		if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_LONG){
			// 規定時間以上連続押下と判断
			evt_act( EVENT_POW_SW_LONG );
		}
#endif

	}else{					// OFF処理
		if( ON == s_unit.pow_sw_last ){
			// ON→OFFエッジ
#if FUNC_SW_LONGPUSH_RELEASE == ON
			if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_LONG){
				// 規定時間以上連続押下と判断
				evt_act( EVENT_POW_SW_LONG );
#else
			if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_LONG){
				// ON確定時にイベント発生済みなのでここでは何もしない
#endif
			}else if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_SHORT){
				evt_act( EVENT_POW_SW_SHORT );
			}else{
				// 何もしない
			}
		}
		// 電源SW押下タイマー再スタート 
		s_unit.sw_time_cnt = 0;
	}
	s_unit.pow_sw_last = pow_sw;
}


/************************************************************************/
/* 関数     : user_main_calc_result										*/
/* 関数名   : 演算結果処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 演算結果処理													*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_calc_result( void )
{
	UW wr_adrs = 0;
#if 0
	//演算結果をPC送付するデバッグコード
	main_cpu_com_snd_pc_log( &s_unit.calc.info.byte[0], CPU_COM_SND_DATA_SIZE_PC_LOG );
#endif
	//範囲チェック
	if( s_unit.calc_cnt > EEP_CACL_DATA_NUM ){
		err_info(ERR_ID_MAIN);
		return;
	}
	
	// フレーム位置とデータ位置からEEPアドレスを算出
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + ( s_unit.calc_cnt * EEP_CACL_DATA_SIZE );

	eep_write( wr_adrs, (UB*)&s_unit.calc, EEP_CACL_DATA_SIZE, OFF );	// 30秒周期なので5ms待ちはしない
	
	s_unit.calc_cnt++;
	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	
}


/************************************************************************/
/* 関数     : user_main_mode											*/
/* 関数名   : メインモード処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : メインモード処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode( void )
{
	// モード共通処理
	user_main_mode_common();
	
	// 各種モード処理
	p_user_main_mode_func[s_unit.system_mode]();
}

/************************************************************************/
/* 関数     : user_main_mode_common										*/
/* 関数名   : モード共通処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : モード共通処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_common( void )
{
	UW now_time;
	
	now_time = time_get_elapsed_time();
	
	// タイムアウトチェック
	if( s_unit.system_mode != s_unit.last_system_mode ){
		s_unit.system_mode_time_out_cnt = now_time;
	}
	
	if( SYSTEM_MODE_IDLE_REST == s_unit.system_mode ){
		if(( now_time - s_unit.system_mode_time_out_cnt ) >= TIME_OUT_SYSTEM_MODE_IDLE_REST ){
			evt_act( EVENT_TIME_OUT );
		}
	}
	if( SYSTEM_MODE_IDLE_COM == s_unit.system_mode ){
		if(( now_time - s_unit.system_mode_time_out_cnt ) >= TIME_OUT_SYSTEM_MODE_IDLE_COM ){
			evt_act( EVENT_TIME_OUT );
		}
	}
	if( SYSTEM_MODE_PRG_H1D == s_unit.system_mode ){
		if(( now_time - s_unit.system_mode_time_out_cnt ) >= TIME_OUT_SYSTEM_MODE_H1D_PRG ){
			evt_act( EVENT_TIME_OUT );
		}
	}
	
	s_unit.last_system_mode = s_unit.system_mode;
}

/************************************************************************/
/* 関数     : user_main_mode_sensing_before								*/
/* 関数名   : センシング前処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : センシング前処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_sensing_before( void )
{
	UW wr_adrs = 0;
	rtc_counter_value_t rtc_val;
	
	// 日時情報取得
	if( MD_OK != R_RTC_Get_CounterValue( &rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
	// BCD→バイナリ変換
	bcd2bin(&s_unit.date.year, &rtc_val.year);
	bcd2bin(&s_unit.date.month, &rtc_val.month);
	bcd2bin(&s_unit.date.week, &rtc_val.week);
	bcd2bin(&s_unit.date.day, &rtc_val.day);
	bcd2bin(&s_unit.date.hour, &rtc_val.hour);
	bcd2bin(&s_unit.date.min, &rtc_val.min);
	bcd2bin(&s_unit.date.sec, &rtc_val.sec);
	
	// 日時情報書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
	eep_write( wr_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE, ON );

	s_unit.calc_cnt = 0;
	
	s_unit.sensing_flg = ON;
}

/************************************************************************/
/* 関数     : user_main_mode_sensing_after								*/
/* 関数名   : センシング後処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : センシング後処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_sensing_after( void )
{
	UB oikosi_flg = OFF;
	UW wr_adrs = 0;
	UB wr_data[2] = {0};
	
	if( 0 == s_unit.calc_cnt ){
		err_info(ERR_ID_MAIN);
		return;
	}
	
	// 下限未満は保存しない
	if( SENSING_CNT_MIN > s_unit.calc_cnt ){
		return;
	}
	
	// EEP書き込み待ち ※周期処理では5ms待ちしていないので最後のタイミングでは固定待ちする
	wait_ms(5);
	
	// 演算回数書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_CALC_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.calc_cnt, 2, ON );
	
	// 追い越し判定
	if(( s_unit.frame_num.write == s_unit.frame_num.read ) &&
	   ( s_unit.frame_num.cnt > 0 )){
		// 追い越されてしまうので読み出しポインタを進める
		oikosi_flg = ON;
	}
	
	// 書き込み枠番号を進める
	INC_MAX_INI(s_unit.frame_num.write, (EEP_FRAME_MAX - 1), 0);
	INC_MAX(s_unit.frame_num.cnt, EEP_FRAME_MAX );
	
	
	wr_adrs = EEP_ADRS_TOP_SETTING;
	if( OFF == oikosi_flg ){
		// 書き込みポインタ
		eep_write( wr_adrs + 1, &s_unit.frame_num.write, 1, ON );
	}else{
		INC_MAX_INI(s_unit.frame_num.read, (EEP_FRAME_MAX  -1), 0);
		// 書き込み、読み出しポインタ
		wr_data[0] = s_unit.frame_num.read;
		wr_data[1] = s_unit.frame_num.write;
		eep_write( wr_adrs, &wr_data[0], 2, ON );
	}
}

/************************************************************************/
/* 関数     : user_main_mode_prg_hd_before								*/
/* 関数名   : プログラム更新(H1D)前処理									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : プログラム更新(H1D)前処理										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_prg_hd_before( void )
{
	UB eep_data = EEP_DATA_TYPE_PRG_H1D;
	
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
		UB tx[VUART_DATA_SIZE_MAX] = {0};
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = 0x00;
	
		main_vuart_send( &tx[0], 2 );
	}
}


/************************************************************************/
/* 関数     : user_main_mode_get_frame_before							*/
/* 関数名   : GETモードフレーム前処理									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : GETモードフレーム前処理										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC UB user_main_mode_get_frame_before( void )
{
	UB ret = ON;
	UW rd_adrs = 0;
	UH calc_cnt;
	
	// データ初期化
	s_unit.get_mode_calc_cnt = 0;
	
	// フレーム位置とデータ位置からEEPアドレスを算出
	rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_CALC_CNT;
	
	eep_read( rd_adrs, (UB*)&calc_cnt, 2 );
	
	if( calc_cnt > EEP_CACL_DATA_NUM ){
		err_info(ERR_ID_MAIN);
		calc_cnt = 0;
	}
	
	s_unit.calc_cnt = calc_cnt;
	s_unit.get_mode_seq = 1;
	
	return ret;
}

/************************************************************************/
/* 関数     : user_main_mode_get_before									*/
/* 関数名   : GETモード前処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : GETモード前処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_get_before( void )
{
	UB tx[VUART_DATA_SIZE_MAX];
	
	s_unit.frame_num_work = s_unit.frame_num;	//中断を考慮しワークにコピーしワークで実行する
	
	if(( s_unit.frame_num_work.write == s_unit.frame_num_work.read ) && 
	   ( s_unit.frame_num_work.cnt == 0 )){
		//データなし時は完了
		tx[0] = 0xE1;		// END
		main_vuart_send( &tx[0], 1 );
#if FUNC_DEBUG_FIN_NON == OFF
		s_unit.get_mode_seq = 6;
#else
		// 完了通知不要(ユニアース版)
		s_unit.get_mode_seq = 7;
#endif
		return;
	}
	
	user_main_mode_get_frame_before();
}


/************************************************************************/
/* 関数     : user_main_mode_get_after									*/
/* 関数名   : GETモード後処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : GETモード後処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_get_after( void )
{
	UW wr_adrs = 0;

	// ワークから正規領域へコピー
	s_unit.frame_num = s_unit.frame_num_work;
	
	wr_adrs = EEP_ADRS_TOP_SETTING;
	eep_write( wr_adrs, &s_unit.frame_num.read, 1, ON );
	s_unit.frame_num.cnt = 0;
	eep_write(( wr_adrs + 2 ), &s_unit.frame_num.cnt, 1, ON );
	
	// システム戻す
	evt_act( EVENT_COMPLETE );
}



/************************************************************************/
/* 関数     : user_main_mode_inital										*/
/* 関数名   : イニシャル状態処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : イニシャル状態処理											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_inital(void)
{
	//ステータス要求通知
	user_main_req_cyc();
}

/************************************************************************/
/* 関数     : user_main_mode_idle_rest									*/
/* 関数名   : アイドル_残量表示状態処理									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : アイドル_残量表示状態処理										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_idle_rest(void)
{
	//ステータス要求通知
	user_main_req_cyc();
}

/************************************************************************/
/* 関数     : user_main_mode_idle_com									*/
/* 関数名   : アイドル_通信待機状態処理									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : アイドル_通信待機状態処理										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_idle_com(void)
{
	//ステータス要求通知
	user_main_req_cyc();
	
}

/************************************************************************/
/* 関数     : user_main_mode_sensing									*/
/* 関数名   : センシング状態処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : センシング状態処理											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_sensing(void)
{
	UW now_time;
	
	now_time = time_get_elapsed_time();
	
	if(( now_time - s_unit.last_sensing_data_rcv ) >= SENSING_END_JUDGE_TIME ){
		// 規定値時間センシングなし
		user_main_req_cyc();
	}else{
		// センシングあり
	}
	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
}

/************************************************************************/
/* 関数     : user_main_mode_move										*/
/* 関数名   : 移行状態処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 移行状態処理													*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_move(void)
{
	// 要求送信
	main_cpu_com_snd_mode_chg();
	
}

/************************************************************************/
/* 関数     : user_main_mode_get										*/
/* 関数名   : GET状態処理												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : GET状態処理													*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_get(void)
{
	UB tx[VUART_DATA_SIZE_MAX];
	UW rd_adrs;
	CALC calc_eep;				// 演算後データ
	
	// 送信中の場合はウェイト
	if( ON == s_ds.vuart.input.send_status ){
		return;
	}
	
	if( 0 == s_unit.get_mode_seq ){
		user_main_mode_get_before();
	}else if( 1 == s_unit.get_mode_seq ){
		// 日時読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
		eep_read( rd_adrs, (UB*)&s_unit.date, EEP_CACL_DATA_SIZE );
		// いびき検知数読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
		eep_read( rd_adrs, (UB*)&s_unit.ibiki_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE );
		// 無呼吸検知数読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
		eep_read( rd_adrs, (UB*)&s_unit.mukokyu_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE );
		// いびき時間読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE );
		// 無呼吸時間読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE );
		// 最高無呼吸時間読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE );
		
		s_unit.get_mode_seq = 2;
	}else if( 2 == s_unit.get_mode_seq ){
		tx[0] = VUART_CMD_DATA_FRAME;		//枠情報(日時他)
		tx[1] = s_unit.date.year;	
		tx[2] = s_unit.date.month;	
		tx[3] = s_unit.date.week;	
		tx[4] = s_unit.date.day;	
		tx[5] = s_unit.date.hour;	
		tx[6] = s_unit.date.min;	
		tx[7] = s_unit.date.sec;
		tx[8] =  ( s_unit.ibiki_detect_cnt & 0x00ff );
		tx[9] = (( s_unit.ibiki_detect_cnt & 0xff00 ) >> 8 );
		tx[10] =  ( s_unit.mukokyu_detect_cnt & 0x00ff );
		tx[11] = (( s_unit.mukokyu_detect_cnt & 0xff00 ) >> 8 );
		tx[12] =  ( s_unit.ibiki_time & 0x00ff );
		tx[13] = (( s_unit.ibiki_time & 0xff00 ) >> 8 );
		tx[14] =  ( s_unit.mukokyu_time & 0x00ff );
		tx[15] = (( s_unit.mukokyu_time & 0xff00 ) >> 8 );
		tx[16] =  ( s_unit.max_mukokyu_sec & 0x00ff );
		tx[17] = (( s_unit.max_mukokyu_sec & 0xff00 ) >> 8 );
		
		main_vuart_send( &tx[0], 18 );
		s_unit.get_mode_seq = 3;
	}else if( 3 == s_unit.get_mode_seq ){
		if( s_unit.calc_cnt <= s_unit.get_mode_calc_cnt ){
			
			s_unit.get_mode_seq = 4;
		}else{
			// EEP読み出し
			// フレーム位置とデータ位置からEEPアドレスを算出
			rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + ( s_unit.get_mode_calc_cnt * EEP_CACL_DATA_SIZE );
			
			eep_read( rd_adrs, (UB*)&calc_eep, EEP_CACL_DATA_SIZE );
			
			// VUART(BLE)送信
			// スマホのIFに合わせる
			tx[0] = VUART_CMD_DATA_CALC;	// 演算データ
			tx[1] = calc_eep.info.dat.ibiki_val[0];
			tx[2] = calc_eep.info.dat.ibiki_val[1];
			tx[3] = calc_eep.info.dat.ibiki_val[2];
			tx[4] = calc_eep.info.dat.state;
			tx[5] = calc_eep.info.dat.body_direct;
			tx[6] = calc_eep.info.dat.photoref[0];
			tx[7] = calc_eep.info.dat.photoref[1];
			tx[8] = calc_eep.info.dat.photoref[2];
			main_vuart_send( &tx[0], 9 );
			s_unit.get_mode_calc_cnt++;
		}
	}else if( 4 == s_unit.get_mode_seq ){
		//読み出し枠番号進める
		INC_MAX_INI(s_unit.frame_num_work.read, ( EEP_FRAME_MAX - 1), 0);
		if( s_unit.frame_num_work.write == s_unit.frame_num_work.read ){
			// 終了
			s_unit.get_mode_seq = 5;
		}else{
			//継続
			tx[0] = VUART_CMD_DATA_NEXT;		// NEXT
			main_vuart_send( &tx[0], 1 );
			user_main_mode_get_frame_before();
		}
	}else if( 5 == s_unit.get_mode_seq ){
		tx[0] = VUART_CMD_DATA_END;				// END
		main_vuart_send( &tx[0], 1 );
		
		#if FUNC_DEBUG_FIN_NON == OFF
			s_unit.get_mode_seq = 6;
		#else
			// 完了通知不要(ユニアース版)
			s_unit.get_mode_seq = 7;
		#endif
	}else if( 6 == s_unit.get_mode_seq ){
		// RD8001暫定：影舞19:完了通知待ち　※タイムアウト必要？
	}else{
		user_main_mode_get_after();
	}
}

/************************************************************************/
/* 関数     : user_main_mode_prg_h1d									*/
/* 関数名   : H1Dプログラム更新状態処理									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : H1Dプログラム更新状態処理										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_prg_h1d(void)
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
	
	if( PRG_SEQ_COMPLETE_WAIT == s_unit.prg_hd_seq ){
		if( OFF == s_ds.vuart.input.send_status ){
			evt_act( EVENT_COMPLETE );
		}
	}
}

/************************************************************************/
/* 関数     : user_main_mode_prg_g1d									*/
/* 関数名   : G1Dプログラム更新状態処理									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : G1Dプログラム更新状態処理										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
//RD8001暫定：G1Dダウンロード_処理確認中
STATIC void user_main_mode_prg_g1d(void)
{
	
}

/************************************************************************/
/* 関数     : user_main_mode_self_check									*/
/* 関数名   : 自己診断状態処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2018.09.10 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 自己診断状態処理												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_mode_self_check( void )
{
	UB read_eep[EEP_ACCESS_ONCE_SIZE];
	UW now_time = time_get_elapsed_time();

	if( 0 == s_unit.self_check.seq ){
		// 全0書き込み
		// EEPプログラムモード
		eep_write( s_unit.self_check.eep_cnt * EEP_ACCESS_ONCE_SIZE, (UB*)&s_eep_page0_tbl, EEP_ACCESS_ONCE_SIZE, ON );
		INC_MAX( s_unit.self_check.eep_cnt, EEP_PAGE_CNT_MAX );
		if( s_unit.self_check.eep_cnt >= EEP_PAGE_CNT_MAX ){
			s_unit.self_check.eep_cnt = 0;
			s_unit.self_check.seq = 1;
		}
	}else if( 1 == s_unit.self_check.seq ){
		// 全0読み出し
		// フレーム位置とデータ位置からEEPアドレスを算出
		eep_read( s_unit.self_check.eep_cnt * EEP_ACCESS_ONCE_SIZE, &read_eep[0], EEP_ACCESS_ONCE_SIZE );
		if( 0 != memcmp( &s_eep_page0_tbl[0], &read_eep[0], EEP_ACCESS_ONCE_SIZE)){
			s_unit.self_check.seq = 2;
			s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DISP_ORDER;
			s_ds.cpu_com.order.snd_data[0] = CPU_COM_DISP_ORDER_SELF_CHECK_ERR;
			s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DISP_ORDER;
			s_unit.self_check.last_time = now_time;
		}
		INC_MAX( s_unit.self_check.eep_cnt, EEP_PAGE_CNT_MAX );
		if( s_unit.self_check.eep_cnt >= EEP_PAGE_CNT_MAX ){
			s_unit.self_check.eep_cnt = 0;
			s_unit.self_check.seq = 3;
			s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DISP_ORDER;
			s_ds.cpu_com.order.snd_data[0] = CPU_COM_DISP_ORDER_SELF_CHECK_FIN;
			s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DISP_ORDER;
			s_unit.self_check.last_time = now_time;
		}
	}else if( 2 == s_unit.self_check.seq ){
		// 異常表示
		if(( now_time - s_unit.self_check.last_time ) >= TIME_CNT_DISP_SELF_CHECK_ERR ){
			s_unit.self_check.seq = 3;
			s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DISP_ORDER;
			s_ds.cpu_com.order.snd_data[0] = CPU_COM_DISP_ORDER_SELF_CHECK_FIN;
			s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DISP_ORDER;
			s_unit.self_check.last_time = now_time;
		}
	}else if( 3 == s_unit.self_check.seq ){
		// 完了
		if(( now_time - s_unit.self_check.last_time ) >= TIME_CNT_DISP_SELF_CHECK_FIN ){
			s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DISP_ORDER;
			s_ds.cpu_com.order.snd_data[0] = CPU_COM_DISP_ORDER_SELF_CHECK_NON;
			s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DISP_ORDER;
			s_unit.self_check.seq = 4;
		}
	}else{
		// 完了
		if( ON ==  s_unit.self_check.com_flg ){
			s_unit.self_check.com_flg = OFF;
			evt_act( EVENT_COMPLETE );
		}
	}
}

/************************************************************************/
/* 関数     : err_info													*/
/* 関数名   : 異常通知													*/
/* 引数     : 異常ID(10進2桁)											*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : G1D側の異常をH1D経由でログ出力(デバッグ機能)					*/
/************************************************************************/
/* 注意事項 :															*/
/* �@デバッグ機能です。エラー出力出来ない可能性があります。				*/
/* �A本ログが原因で不具合が発生する事があります。理由としては製品機能の	*/
/*   CPU間通信に割り込む為。											*/
/************************************************************************/
void err_info( ERR_ID id )
{
#if FUNC_DEBUG_LOG == ON
	// ログ出力
	UB tx[CPU_COM_SND_DATA_SIZE_PC_LOG];
	
	memset( &tx[0], 0x20, sizeof(tx) );
	
	
	tx[0] = 'G';
	tx[1] = '1';
	tx[2] = 'E';
	tx[3] = 'R';
	tx[4] = 'R';
	tx[5] = (id / 10 ) + 0x30;
	tx[6] = (id % 10 ) + 0x30;
	
	if( s_unit.last_err_id != id ){
		main_cpu_com_snd_pc_log( (UB*)&tx[0], CPU_COM_SND_DATA_SIZE_PC_LOG );		// デバッグ
		NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	}
	
#if 0
	while(1){
		// 異常による永久ループ
	}
#endif
#endif
	s_unit.err_cnt++;
	s_unit.last_err_id = id;
}

/************************************************************************/
/* 関数     : evt_act													*/
/* 関数名   : イベント実行												*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : TRUE	実施												*/
/*          : FALSE	未実施												*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント実行													*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC UB evt_act( EVENT_NUM evt )
{
	SYSTEM_MODE	system_mode;
	
	
	system_mode = p_event_table[evt][s_unit.system_mode]( evt );
	if( SYSTEM_MODE_NON == system_mode ){
		return FALSE;
	}
	
	// 影舞21:電源SW押下受付後の表示の為ここで行う
	if( EVENT_POW_SW_LONG == evt ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_DISP_ORDER;
		s_ds.cpu_com.order.snd_data[0] = CPU_COM_DISP_TRG_LONG_SW_RECEPTION;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_DISP_ORDER;
	}
	
	main_chg_system_mode( system_mode );
	
	return TRUE;
}

/************************************************************************/
/* 関数     : evt_non													*/
/* 関数名   : イベント(なし)											*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(なし)												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_non( int evt)
{
	return SYSTEM_MODE_NON;
};

/************************************************************************/
/* 関数     : evt_idle_rest												*/
/* 関数名   : イベント(アイドル_残量表示)								*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(アイドル_残量表示)									*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_idle_rest( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_REST;
	
	if( s_unit.battery_sts == BAT_LEVEL_STS_MIN ){
			system_mode = SYSTEM_MODE_NON;
	}

	return system_mode;
}

/************************************************************************/
/* 関数     : evt_idle_com												*/
/* 関数名   : イベント(アイドル_通信待機)								*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(アイドル_通信待機)									*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_idle_com( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_COM;
	
	return system_mode;
}

/************************************************************************/
/* 関数     : evt_idle_com												*/
/* 関数名   : イベント(アイドル_通信待機_電池残量チェックあり)			*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(アイドル_通信待機_電池残量チェックあり)				*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_idle_com_denchi( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_COM;
	
	if( s_unit.battery_sts == BAT_LEVEL_STS_MIN ){
			system_mode = SYSTEM_MODE_NON;
	}
	
	return system_mode;
}
/************************************************************************/
/* 関数     : evt_sensing												*/
/* 関数名   : イベント(センシング)										*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(センシング)											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_sensing( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SENSING;
	
	return system_mode;
}

/************************************************************************/
/* 関数     : evt_sensing_chg											*/
/* 関数名   : イベント(センシング_充電状態チェックあり)					*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(センシング_充電状態チェックあり)						*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_sensing_chg( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SENSING;
	
	if( ON == s_unit.h1d.info.bit_f.bat_chg ){
		system_mode = SYSTEM_MODE_NON;
	}
	
	return system_mode;
}

/************************************************************************/
/* 関数     : evt_initial												*/
/* 関数名   : イベント(イニシャル)										*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(イニシャル)											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_initial( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_INITIAL;
	
	return system_mode;
}

/************************************************************************/
/* 関数     : evt_initial_chg											*/
/* 関数名   : イベント(イニシャル_充電チェックあり)						*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(イニシャル_充電チェックあり)							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_initial_chg( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_INITIAL;
	
	if( ON == s_unit.h1d.info.bit_f.bat_chg ){
		system_mode = SYSTEM_MODE_NON;
	}
	
	return system_mode;
}

/************************************************************************/
/* 関数     : evt_get													*/
/* 関数名   : イベント(GETモード)										*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(GETモード)											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_get( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_GET;
	
	return system_mode;
}

/************************************************************************/
/* 関数     : evt_h1d_prg_denchi										*/
/* 関数名   : イベント(H1Dプログラム更新_電池チェックあり)				*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(H1Dプログラム更新_電池チェックあり)					*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_h1d_prg_denchi( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_PRG_H1D;
	
	if( s_unit.battery_sts == BAT_LEVEL_STS_MIN ){
			system_mode = SYSTEM_MODE_NON;
	}

	return system_mode;
}

/************************************************************************/
/* 関数     : evt_g1d_prg_denchi										*/
/* 関数名   : イベント(G1Dプログラム更新_電池チェックあり)				*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(G1Dプログラム更新_電池チェックあり)					*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_g1d_prg_denchi( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_PRG_G1D;
	
	if( s_unit.battery_sts == BAT_LEVEL_STS_MIN ){
			system_mode = SYSTEM_MODE_NON;
	}

	return system_mode;
}

/************************************************************************/
/* 関数     : evt_g1d_prg_denchi										*/
/* 関数名   : イベント(自己診断)										*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(自己診断)											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_self_check( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SELF_CHECK;
	
	s_unit.self_check.seq = 0;
	
	return system_mode;
}


/************************************************************************/
/* 関数     : evt_prg_h1d_fin											*/
/* 関数名   : イベント(H1Dプログラム書き換え完了)						*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.13  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(H1Dプログラム書き換え完了)							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_prg_h1d_fin( int evt)
{
	SYSTEM_MODE system_mode;;
	
	// EEPを消去し通常モードで使えるようにしておく ※リセットを経由しないケースがある為
	eep_part_erase();
	
	// 移行の処理は同じなので下記関数で実行
	system_mode = evt_idle_com( evt );
	
	
	return system_mode;
}


/************************************************************************/
/* 関数     : evt_prg_h1d_time_out										*/
/* 関数名   : イベント(H1Dプログラム書き換えタイムアウト)				*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.13  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : イベント(H1Dプログラム書き換えタイムアウト)					*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_prg_h1d_time_out( int evt)
{
	SYSTEM_MODE system_mode;;

	// 上位への結果にNGを格納
	s_unit.prg_hd_update_state = PRG_HD_UPDATE_STATE_NG;
	
	// 移行の処理は同じなので下記関数で実行
	system_mode = evt_idle_com( evt );
	
	return system_mode;
}



// =============================================================
// CPU通信関連
// =============================================================
/************************************************************************/
/* 関数     : user_main_req_cyc											*/
/* 関数名   : ステータス要求周期処理									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : ステータス要求周期処理										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void user_main_req_cyc( void )
{
	UW now_time;
	
	now_time = time_get_elapsed_time();
	
	// ステータス要求送信周期
	if(( now_time - s_unit.last_time_sts_req ) >= MAIN_STATUS_REQ_TIME ){
		if( ON == main_cpu_com_snd_sts_req() ){		/* ステータス要求送信 */
			s_unit.last_time_sts_req = now_time;
		}
	}
}


/************************************************************************/
/* 関数     : main_cpu_com_snd_sts_req									*/
/* 関数名   : ステータス要求通知										*/
/* 引数     : なし														*/
/* 戻り値   : ON	実行												*/
/*          : ON	未実行												*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : ステータス要求通知											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC UB main_cpu_com_snd_sts_req( void )
{
	G1D_INFO g1d;
	UB ret = OFF;
	
	g1d.info.byte = 0;
	g1d.info.bit_f.ble = get_ble_connect();
	
	if( CPU_COM_SND_STATUS_IDLE == s_ds.cpu_com.input.cpu_com_send_status ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_STATUS_REQ;
		s_ds.cpu_com.order.snd_data[0] = s_unit.system_mode;
		s_ds.cpu_com.order.snd_data[1] = g1d.info.byte;
		s_ds.cpu_com.order.snd_data[2] = 0;
		s_ds.cpu_com.order.snd_data[3] = 0;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_STATUS_REQ;
		ret = ON;
	}
	
	return ret;
}


/************************************************************************/
/* 関数     : main_cpu_com_snd_mode_chg									*/
/* 関数名   : モード変更通知											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : モード変更通知												*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_snd_mode_chg( void )
{
	
	if( CPU_COM_SND_STATUS_IDLE == s_ds.cpu_com.input.cpu_com_send_status ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_MODE_CHG;
		s_ds.cpu_com.order.snd_data[0] = s_unit.next_system_mode;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_MODE_CHG;
	}
}



/************************************************************************/
/* 関数     : main_cpu_com_snd_pc_log									*/
/* 関数名   : CPU間通信(ログ送信)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.07  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : CPU間通信(ログ送信)											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void main_cpu_com_snd_pc_log( UB* data, UB size )
{
#if FUNC_DEBUG_LOG == ON
#endif
}

/************************************************************************/
/* 関数     : main_cpu_com_snd_sensing_order							*/
/* 関数名   : センシング指示送信										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.08.01  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : センシング指示送信											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
/*
STATIC void main_cpu_com_snd_sensing_order( UB sekigai )
{
	if( CPU_COM_SND_STATUS_IDLE == s_ds.cpu_com.input.cpu_com_send_status ){
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_SENSING_ORDER;
		s_ds.cpu_com.order.snd_data[0] = sekigai;
		s_ds.cpu_com.order.data_size = CPU_COM_SND_DATA_SIZE_SENSING_ORDER;
	}
}
*/

/************************************************************************/
/* 関数     : main_cpu_com_proc											*/
/* 関数名   : CPU間通信周期処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
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
					s_cpu_com_rcv_func_tbl[i].func();
				}
			}
		}
		// 受信コマンドクリア
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_date_set									*/
/* 関数名   : CPU間通信受信(日時)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : CPU間通信受信(日時)											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_date_set( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	tx[0] = VUART_CMD_DATE_SET;
	tx[1] = s_ds.cpu_com.input.rcv_data[0];		// CPU間の応答をそのまま入れる
	main_vuart_send( &tx[0], 2 );
	
	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
}


/************************************************************************/
/* 関数     : main_cpu_com_rcv_sts_res									*/
/* 関数名   : CPU間通信受信(ステータス要求)								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : CPU間通信受信(ステータス要求)									*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_sts_res( void )
{
	UW now_time;
	
	// H1Dイベント処理
	if( EVENT_NON != s_ds.cpu_com.input.rcv_data[0] ){
		// 以降状態へ変更
		if( FALSE == evt_act( (EVENT_NUM)s_ds.cpu_com.input.rcv_data[0] )){
			NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
		}
	}
	
	// H1D状態　※現在未使用
	// s_ds.cpu_com.input.rcv_data[1];
	
	// H1D情報
	s_unit.h1d.info.byte = s_ds.cpu_com.input.rcv_data[2];
	
	// 検査ポートON状態
	if( ON == s_unit.h1d.info.bit_f.kensa ){
		if( FALSE == evt_act( EVENT_KENSA_ON )){
			NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
		}
	}
	// 充電ポートONエッジ
	if(( ON  == s_unit.h1d.info.bit_f.bat_chg ) && 
	   ( OFF == s_unit.h1d_last.info.bit_f.bat_chg )){
		if( FALSE == evt_act( EVENT_CHG_PORT_ON )){
			NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
		}
	}
	
	s_unit.h1d_last.info.byte = s_unit.h1d.info.byte;
	
	
	// 日時
	s_unit.date.year	 = s_ds.cpu_com.input.rcv_data[3];
	s_unit.date.month	 = s_ds.cpu_com.input.rcv_data[4];
	s_unit.date.week	 = s_ds.cpu_com.input.rcv_data[5];
	s_unit.date.day		 = s_ds.cpu_com.input.rcv_data[6];
	s_unit.date.hour	 = s_ds.cpu_com.input.rcv_data[7];
	s_unit.date.min		 = s_ds.cpu_com.input.rcv_data[8];
	s_unit.date.sec		 = s_ds.cpu_com.input.rcv_data[9];

	// 電池状態更新
	s_unit.battery_sts = s_ds.cpu_com.input.rcv_data[10];
	
	now_time = time_get_elapsed_time();
	
	if(( BAT_LEVEL_STS_MIN == s_unit.battery_sts ) &&
	   (( now_time - s_unit.last_time_battery_level_min ) > TIME_CNT_BAT_LEVEL_MIN_INTERVAL )){
		evt_act( EVENT_DENCH_LOW );			// 電池残量低下
		s_unit.last_time_battery_level_min = now_time;
	}
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_sensor_res								*/
/* 関数名   : CPU間通信受信(センシングデータ)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : CPU間通信受信(センシングデータ)								*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_sensor_res( void )
{
//	ke_time_t now_time;
	uint8_t *ke_msg;
	MEAS meas;
	
	// 受信日時格納
	s_unit.last_sensing_data_rcv = time_get_elapsed_time();
	
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

	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_mode_chg									*/
/* 関数名   : CPU間通信受信(モード変更)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : CPU間通信受信(モード変更)										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_mode_chg( void )
{
	if( SYSTEM_MODE_MOVE != s_unit.system_mode){
		return;
	}
	
	if( 0 == s_ds.cpu_com.input.rcv_data[0] ){		// 正常終了
		s_unit.system_mode = s_unit.next_system_mode;		// モード変更
		
	}else{
		s_unit.system_mode = SYSTEM_MODE_IDLE_COM;				// モード変更
		s_unit.next_system_mode = SYSTEM_MODE_IDLE_COM;			// モード変更
		err_info(ERR_ID_MAIN);
		return;
	}
	
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		user_main_mode_sensing_before();
	}
	
	if( SYSTEM_MODE_IDLE_COM == s_unit.system_mode ){
		if( ON == s_unit.sensing_flg ){
			s_unit.sensing_flg = OFF;
			user_main_mode_sensing_after();
		}
	}
	
	if( SYSTEM_MODE_PRG_H1D == s_unit.system_mode ){
		user_main_mode_prg_hd_before();
	}
	
	if( SYSTEM_MODE_PRG_G1D == s_unit.system_mode ){
		//RD8001暫定：G1Dダウンロード_処理確認中_応答を返せるように修正
		FW_Update_Receiver_Start();
	}

	
	if( SYSTEM_MODE_SELF_CHECK == s_unit.system_mode ){
		if( ON == s_unit.self_check.com_flg ){
			{
				UB tx[VUART_DATA_SIZE_MAX] = {0};
				
				// OK応答
				tx[0] = VUART_CMD_MODE_CHG;
				tx[1] = 0x00;
				
				s_ds.vuart.input.send_status = OFF;
				main_vuart_send( &tx[0], 2 );
				
			}
		}
	}

	if( SYSTEM_MODE_GET == s_unit.system_mode ){
		{
			UB tx[VUART_DATA_SIZE_MAX] = {0};
			
			// OK応答
			tx[0] = VUART_CMD_MODE_CHG;
			tx[1] = 0x00;
			
			s_ds.vuart.input.send_status = OFF;
			main_vuart_send( &tx[0], 2 );
			
			s_unit.get_mode_seq = 0;
		}
	}
	
	
}


/************************************************************************/
/* 関数     : main_cpu_com_rcv_mode_chg									*/
/* 関数名   : CPU間通信受信(モード変更)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : CPU間通信受信(モード変更)										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_mode_chg( void )
{
	if( SYSTEM_MODE_MOVE != s_unit.system_mode){
		return;
	}
	
	s_unit.system_mode = s_unit.next_system_mode;		// モード変更
	
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		user_main_mode_sensing_before();
		// センシング移行時にLEDとバイブ動作
		if( s_unit.battery_sts == BAT_LEVEL_STS_HIGH || s_unit.battery_sts == BAT_LEVEL_STS_MAX )
		{
			set_led( LED_PATT_GREEN_LIGHTING );
		} else if( s_unit.battery_sts == BAT_LEVEL_STS_LOW ) {
			set_led( LED_PATT_GREEN_BLINK );
		}
		
		set_vib(VIB_MODE_SENSING);
	}
	
	if( SYSTEM_MODE_IDLE_COM == s_unit.system_mode ){
		if( ON == s_unit.sensing_flg ){
			s_unit.sensing_flg = OFF;
			user_main_mode_sensing_after();
			set_vib(VIB_MODE_STANDBY);
		}
	}
	
	if( SYSTEM_MODE_PRG_H1D == s_unit.system_mode ){
		user_main_mode_prg_hd_before();
	}
	
	if( SYSTEM_MODE_PRG_G1D == s_unit.system_mode ){
		//RD8001暫定：G1Dダウンロード_処理確認中_応答を返せるように修正
		FW_Update_Receiver_Start();
	}

	
	if( SYSTEM_MODE_SELF_CHECK == s_unit.system_mode ){
		if( ON == s_unit.self_check.com_flg ){
			{
				UB tx[VUART_DATA_SIZE_MAX] = {0};
				
				// OK応答
				tx[0] = VUART_CMD_MODE_CHG;
				tx[1] = 0x00;
				
				s_ds.vuart.input.send_status = OFF;
				main_vuart_send( &tx[0], 2 );
				
			}
		}
	}

	if( SYSTEM_MODE_GET == s_unit.system_mode ){
		{
			UB tx[VUART_DATA_SIZE_MAX] = {0};
			
			// OK応答
			tx[0] = VUART_CMD_MODE_CHG;
			tx[1] = 0x00;
			
			s_ds.vuart.input.send_status = OFF;
			main_vuart_send( &tx[0], 2 );
			
			s_unit.get_mode_seq = 0;
		}
	}
	
	
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_disp_order								*/
/* 関数名   : CPU間通信受信(表示指示)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : CPU間通信受信(表示指示)										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_disp_order( void )
{
	// 処理なし
	
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_disp_order								*/
/* 関数名   : CPU間通信受信(バージョン)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : CPU間通信受信(バージョン)										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_cpu_com_rcv_version( void )
{
	// 処理なし
	{
		UB tx[VUART_DATA_SIZE_MAX] = {0};
		
		// OK応答
		tx[0] = VUART_CMD_VERSION;
		// H1Dバージョン
		
		tx[1] = VUART_DATA_RESULT_OK;
		tx[2] = s_ds.cpu_com.input.rcv_data[0];
		tx[3] = s_ds.cpu_com.input.rcv_data[1];
		tx[4] = s_ds.cpu_com.input.rcv_data[2];
		tx[5] = s_ds.cpu_com.input.rcv_data[3];
		tx[6] = s_ds.cpu_com.input.rcv_data[4];
		tx[7] = s_ds.cpu_com.input.rcv_data[5];
		tx[8] = s_ds.cpu_com.input.rcv_data[6];
		tx[9] = s_ds.cpu_com.input.rcv_data[7];
		// G1Dバージョン
		tx[10]  = version_product_tbl[0];
		tx[11] = version_product_tbl[1];
		tx[12] = version_product_tbl[2];
		tx[13] = version_product_tbl[3];
		
		main_vuart_send( &tx[0], VUART_SND_LEN_VERSION );
	}
}


/************************************************************************/
/* 関数     : main_chg_system_mode										*/
/* 関数名   : モード変更												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : モード変更													*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode )
{
	s_unit.next_system_mode = next_mode;
	s_unit.system_mode = SYSTEM_MODE_MOVE;
	
	// ■暫定 CPU間通信のモード変更はないので、直接遷移処理する
	main_mode_chg();
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
	int i  =0;
	
	if( 0 == s_ds.vuart.input.rcv_len ){
		return;		//受信なし
	}
	
	if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
		// RD8001暫定：イニシャル時は受信せずクリア
		s_ds.vuart.input.rcv_len = 0;
		return;		//受信なし
	}
	
	/* 以降受信データ有り */
	for( i = VUART_CMD_TYPE_NONE; i < VUART_CMD_TYPE_MAX; i++){
		if( s_vuart_rcv_func_tbl[i].len == s_ds.vuart.input.rcv_len ){
			/* データ長が一致 */
			if(( s_vuart_rcv_func_tbl[i].cmd == s_ds.vuart.input.rcv_data[0] ) ||
			   ( s_vuart_rcv_func_tbl[i].cmd == VUART_CMD_INVALID )){
				/* 受信コマンドとコマンドテーブルが一致またはコマンド無効 */
				if( NULL != s_vuart_rcv_func_tbl[i].func ){
					/* 受信処理有り */
					s_vuart_rcv_func_tbl[i].func();
				}
			}
		}
	}

#if 1
	// RD8001暫定：G1Dダウンロード_処理確認中(テストボードでのデバッグ処理)
	if(( VUART_CMD_LEN_PRG_G1D_START == s_ds.vuart.input.rcv_len  ) && 
	         ( VUART_CMD_PRG_G1D_START == s_ds.vuart.input.rcv_data[0] )){
		FW_Update_Receiver_Start();
	}else if(( VUART_CMD_LEN_PRG_G1D_VER == s_ds.vuart.input.rcv_len  ) && 
	         ( VUART_CMD_PRG_G1D_VER == s_ds.vuart.input.rcv_data[0] )){
		s_unit.prg_g1d_send_ver_flg = ON;
		s_unit.prg_g1d_send_ver_sec = 5;	// 5秒後
	}else{
		// 該当コマンドなし
		
	}
#endif
	
	// 受信長クリア
	s_ds.vuart.input.rcv_len = 0;
}

/************************************************************************/
/* 関数     : ds_set_vuart_data											*/
/* 関数名   : VUART通信データセット										*/
/* 引数     : VUART通信データ格納ポインタ								*/
/*          : データ長													*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART通信データセット												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_send( UB *p_data, UB len )
{
	// Vuart送信中は
	if( ON == s_ds.vuart.input.send_status ){
		err_info(ERR_ID_BLE_SEND_ERR);
		return;
	}
	
	s_ds.vuart.input.send_status = ON;
	R_APP_VUART_Send_Char( (char *)p_data, len );
}

/************************************************************************/
/* 関数     : main_vuart_set_mode									*/
/* 関数名   : VUART通信データセット										*/
/* 引数     : VUART通信データ格納ポインタ								*/
/*          : データ長													*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART通信データセット												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_set_mode( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	
	// OK応答
	tx[0] = VUART_CMD_MODE_CHG;
	tx[1] = 0x00;
	
	main_vuart_send( &tx[0], 2 );

	s_unit.calc_cnt = 0;
}


/************************************************************************/
/* 関数     : main_vuart_rcv_mode_chg									*/
/* 関数名   : VUART通信データセット										*/
/* 引数     : VUART通信データ格納ポインタ								*/
/*          : データ長													*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART通信データセット												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_rcv_mode_chg( void )
{
	UB ret = TRUE;
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	if( 2 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_H1D_PRG );
	}else if( 3 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_GET_DATA );
	}else if( 4 == s_ds.vuart.input.rcv_data[1] ){
		main_vuart_set_mode();		// RD8001暫定：デバッグ用データ設定(SETコマンド_最終必要？)
	}else if( 5 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_G1D_PRG );
	}else if( 6 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_SELF_CHECK_COM );
		if( TRUE == ret){
			s_unit.self_check.com_flg = ON;
		} 
	}else{
		// 何もしない
	}
	
	if( FALSE == ret ){
		// NG応答
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = VUART_DATA_RESULT_NG;
		main_vuart_send( &tx[0], 2 );
	}else{
		// OK応答
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = VUART_DATA_RESULT_OK;
		main_vuart_send( &tx[0], 2 );
	}
}

/************************************************************************/
/* 関数     : main_vuart_rcv_info										*/
/* 関数名   : VUART通信(情報取得)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART通信(情報取得)													*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_rcv_info( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	UB result = VUART_DATA_RESULT_OK;
	
	if(( s_unit.system_mode != SYSTEM_MODE_IDLE_REST ) &&
	   ( s_unit.system_mode != SYSTEM_MODE_IDLE_COM )){
		result = VUART_DATA_RESULT_NG;
	}
	
	tx[0] = VUART_CMD_INFO;
	tx[1] = result;
	tx[2] = s_unit.battery_sts;
	main_vuart_send( &tx[0], VUART_SND_LEN_INFO );
}

/************************************************************************/
/* 関数     : main_vuart_rcv_version									*/
/* 関数名   : VUART受信(バージョン)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(バージョン)												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_rcv_version( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};

//	if(( s_unit.system_mode != SYSTEM_MODE_IDLE_REST ) &&
//	   ( s_unit.system_mode != SYSTEM_MODE_IDLE_COM )){
		
		// OK応答
		tx[0] = VUART_CMD_VERSION;
		// G1Dバージョン
		tx[1] = VUART_DATA_RESULT_OK;
		tx[2] = version_product_tbl[0];
		tx[3] = version_product_tbl[1];
		tx[4] = version_product_tbl[2];
		tx[5] = version_product_tbl[3];
		main_vuart_send( &tx[0], VUART_SND_LEN_VERSION );
//	}else{
		// バージョンをブリッジで送信
//		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_VERSION;
//		s_ds.cpu_com.order.data_size = 0;
//	}
}


/************************************************************************/
/* 関数     : main_set_bd_adrs											*/
/* 関数名   : BDアドレス設定											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* BDアドレス設定														*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_set_bd_adrs( UB* bda)
{
	s_unit.bd_device_adrs[0] = bda[0];
	s_unit.bd_device_adrs[1] = bda[1];
	s_unit.bd_device_adrs[2] = bda[2];
	s_unit.bd_device_adrs[3] = bda[3];
	s_unit.bd_device_adrs[4] = bda[4];
	s_unit.bd_device_adrs[5] = bda[5];

}

/************************************************************************/
/* 関数     : main_vuart_rcv_device_info								*/
/* 関数名   : VUART受信(デバイス情報)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(デバイス情報)												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_rcv_device_info( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	UB result = VUART_DATA_RESULT_OK;
	rtc_counter_value_t rtc_val;
	rtc_counter_value_t rtc_val_bin;
	
	if(( s_unit.system_mode != SYSTEM_MODE_IDLE_REST ) &&
	   ( s_unit.system_mode != SYSTEM_MODE_IDLE_COM )){
		result = VUART_DATA_RESULT_NG;
	}
	
	if( MD_OK != R_RTC_Get_CounterValue( &rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
	// BCD→バイナリ変換
	bcd2bin(&rtc_val_bin.year, &rtc_val.year);
	bcd2bin(&rtc_val_bin.month, &rtc_val.month);
	bcd2bin(&rtc_val_bin.week, &rtc_val.week);
	bcd2bin(&rtc_val_bin.day, &rtc_val.day);
	bcd2bin(&rtc_val_bin.hour, &rtc_val.hour);
	bcd2bin(&rtc_val_bin.min, &rtc_val.min);
	bcd2bin(&rtc_val_bin.sec, &rtc_val.sec);
	
	tx[0] = VUART_CMD_DEVICE_INFO;
	tx[1] = result;							// 結果
	tx[2] = s_unit.bd_device_adrs[0];		// BDデバイスアドレス
	tx[3] = s_unit.bd_device_adrs[1];
	tx[4] = s_unit.bd_device_adrs[2];
	tx[5] = s_unit.bd_device_adrs[3];
	tx[6] = s_unit.bd_device_adrs[4];
	tx[7] = s_unit.bd_device_adrs[5];
	tx[8] = s_unit.frame_num.cnt;
	tx[9]  = rtc_val_bin.year;
	tx[10]  = rtc_val_bin.month;
	tx[11] = rtc_val_bin.week;
	tx[12] = rtc_val_bin.day;
	tx[13] = rtc_val_bin.hour;
	tx[14] = rtc_val_bin.min;
	tx[15] = rtc_val_bin.sec;
	
	main_vuart_send( &tx[0], VUART_SND_LEN_DEVICE_INFO );
}

/************************************************************************/
/* 関数     : main_vuart_rcv_data_frame									*/
/* 関数名   : VUART受信(フレーム)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(フレーム)													*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
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
	
	s_unit.ibiki_detect_cnt  = s_ds.vuart.input.rcv_data[9] << 8;
	s_unit.ibiki_detect_cnt  |= s_ds.vuart.input.rcv_data[8];
	s_unit.mukokyu_detect_cnt  = s_ds.vuart.input.rcv_data[11] << 8;
	s_unit.mukokyu_detect_cnt  |= s_ds.vuart.input.rcv_data[10];
	s_unit.ibiki_time  = s_ds.vuart.input.rcv_data[13] << 8;
	s_unit.ibiki_time  |= s_ds.vuart.input.rcv_data[12];
	s_unit.mukokyu_time  = s_ds.vuart.input.rcv_data[15] << 8;
	s_unit.mukokyu_time  |= s_ds.vuart.input.rcv_data[14];
	s_unit.max_mukokyu_sec  = s_ds.vuart.input.rcv_data[17] << 8;
	s_unit.max_mukokyu_sec  |= s_ds.vuart.input.rcv_data[16];
	
	// 日時情報書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
	eep_write( wr_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE, ON );
	// いびき検知数書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE, ON );
	// 無呼吸検知数書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE, ON );
	// いびき時間書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE, ON );
	// 無呼吸時間書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE, ON );
	// 最高無呼吸時間書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE, ON );
	
}
/************************************************************************/
/* 関数     : main_vuart_rcv_data_calc									*/
/* 関数名   : VUART受信(演算)											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(演算)														*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_rcv_data_calc( void )
{
	s_unit.calc.info.dat.ibiki_val[0] = s_ds.vuart.input.rcv_data[1];
	s_unit.calc.info.dat.ibiki_val[1] = s_ds.vuart.input.rcv_data[2];
	s_unit.calc.info.dat.ibiki_val[2] = s_ds.vuart.input.rcv_data[3];
	s_unit.calc.info.dat.state = s_ds.vuart.input.rcv_data[4];
	s_unit.calc.info.dat.body_direct = s_ds.vuart.input.rcv_data[5];
	s_unit.calc.info.dat.photoref[0] = s_ds.vuart.input.rcv_data[6];
	s_unit.calc.info.dat.photoref[1] = s_ds.vuart.input.rcv_data[7];
	s_unit.calc.info.dat.photoref[2] = s_ds.vuart.input.rcv_data[8];
	
//	memcpy(&s_unit.calc.info.byte[0], &s_ds.vuart.input.rcv_data[1], ( VUART_CMD_LEN_DATA_CALC - VUART_CMD_ONLY_SIZE ));
	user_main_calc_result();
}


/************************************************************************/
/* 関数     : main_vuart_rcv_data_end									*/
/* 関数名   : VUART受信(END)											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(END)														*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_rcv_data_end( void )
{
	user_main_mode_sensing_after();
}

/************************************************************************/
/* 関数     : main_vuart_rcv_data_fin									*/
/* 関数名   : VUART受信(完了)											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(完了)														*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_rcv_data_fin( void )
{
	s_unit.get_mode_seq = 7;
}

/************************************************************************/
/* 関数     : main_vuart_rcv_date										*/
/* 関数名   : VUART受信(日時設定)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(日時設定)													*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_rcv_date( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	rtc_counter_value_t rtc_val;
	
	rtc_val.year = s_ds.vuart.input.rcv_data[1];
	rtc_val.month = s_ds.vuart.input.rcv_data[2];
	rtc_val.week = s_ds.vuart.input.rcv_data[3];
	rtc_val.day = s_ds.vuart.input.rcv_data[4];
	rtc_val.hour = s_ds.vuart.input.rcv_data[5];
	rtc_val.min = s_ds.vuart.input.rcv_data[6];
	rtc_val.sec = s_ds.vuart.input.rcv_data[7];
	
	// バイナリ→BCD変換
	rtc_val.year  = bin2bcd(rtc_val.year);
	rtc_val.month = bin2bcd(rtc_val.month);
	rtc_val.week  = bin2bcd(rtc_val.week);
	rtc_val.day   = bin2bcd(rtc_val.day);
	rtc_val.hour  = bin2bcd(rtc_val.hour);
	rtc_val.min   = bin2bcd(rtc_val.min);
	rtc_val.sec   = bin2bcd(rtc_val.sec);
	
	
	if( MD_OK != R_RTC_Set_CounterValue( rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
//	if(( s_unit.system_mode != SYSTEM_MODE_IDLE_REST ) &&
//	   ( s_unit.system_mode != SYSTEM_MODE_IDLE_COM )){
		tx[0] = VUART_CMD_DATE_SET;
		tx[1] = VUART_DATA_RESULT_OK;
		main_vuart_send( &tx[0], 2 );
//	}
}

/************************************************************************/
/* 関数     : main_vuart_rcv_device_set									*/
/* 関数名   : VUART受信(デバイス設定)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.26  Axia Soft Design 和田 耕太	初版作成		*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(デバイス設定)												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_rcv_device_set( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	UB result = VUART_DATA_RESULT_OK;
	
	if(( s_unit.system_mode != SYSTEM_MODE_IDLE_REST ) &&
	   ( s_unit.system_mode != SYSTEM_MODE_IDLE_COM )){
		result = VUART_DATA_RESULT_NG;
	}else{
		s_unit.alarm.info.dat.act_mode = s_ds.vuart.input.rcv_data[1];
		s_unit.alarm.info.dat.ibiki_sens = s_ds.vuart.input.rcv_data[2];
		s_unit.alarm.info.dat.yokusei_str = s_ds.vuart.input.rcv_data[3];
		s_unit.alarm.info.dat.yokusei_max_time = s_ds.vuart.input.rcv_data[4];
		
		eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE, ON );
	}
	
	tx[0] = VUART_CMD_DEVICE_SET;
	tx[1] = result;
	main_vuart_send( &tx[0], 2 );
	
	if(result == VUART_DATA_RESULT_OK)
	{
		// 動作モード設定
		act_mode = s_unit.alarm.info.dat.act_mode;
		// いびき感度設定
		set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
	}
}

#if 0
/************************************************************************/
/* 関数     : main_vuart_rcv_alarm_set									*/
/* 関数名   : VUART受信(アラーム設定)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(アラーム設定)												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_rcv_alarm_set( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	UB result = VUART_DATA_RESULT_OK;
	
	if(( s_unit.system_mode != SYSTEM_MODE_IDLE_REST ) &&
	   ( s_unit.system_mode != SYSTEM_MODE_IDLE_COM )){
		result = VUART_DATA_RESULT_NG;
	}else{
		s_unit.alarm.info.dat.valid = s_ds.vuart.input.rcv_data[1];
		s_unit.alarm.info.dat.ibiki = s_ds.vuart.input.rcv_data[2];
		s_unit.alarm.info.dat.ibiki_sens = s_ds.vuart.input.rcv_data[3];
		s_unit.alarm.info.dat.low_kokyu = s_ds.vuart.input.rcv_data[4];
		s_unit.alarm.info.dat.delay = s_ds.vuart.input.rcv_data[5];
		s_unit.alarm.info.dat.stop = s_ds.vuart.input.rcv_data[6];
		s_unit.alarm.info.dat.time = s_ds.vuart.input.rcv_data[7];
		
		eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE, ON );
	}
	
	tx[0] = VUART_CMD_ALARM_SET;
	tx[1] = result;
	main_vuart_send( &tx[0], 2 );
}

/************************************************************************/
/* 関数     : main_vuart_snd_alarm_info									*/
/* 関数名   : VUART送信(アラーム通知)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART送信(アラーム通知)												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_snd_alarm_info( UB type, UB data )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	tx[0] = VUART_CMD_ALARM_INFO;
	tx[1] = type;
	tx[2] = data;
	main_vuart_send( &tx[0], 3 );
}
#endif

/************************************************************************/
/* 関数     : ds_get_cpu_com_order										*/
/* 関数名   : CPU間通信用データ取得										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
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
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
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
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
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
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
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
/*
static int_t main_calc_sekigai(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	s_unit.sekigai_seq += 1;
	// 4回に1回赤外を有効にする
	if(s_unit.sekigai_seq == 3){
		main_cpu_com_snd_sensing_order(ON);
	}else if(s_unit.sekigai_seq >= 4){
		calculator_pulse_oximeter_inf(&s_unit.sekigai_val[0]);
		s_unit.calc.info.dat.spo2_val = get_spo2();
		s_unit.sekigai_seq = 0;
		
		// 低呼吸 or 無呼吸時はOFFしない
		if((s_unit.calc.info.dat.state & 0xC0) == 0){
			main_cpu_com_snd_sensing_order(OFF);
		}
	}else{
		// 処理なし
	}
	s_unit.sekigai_cnt = 0;
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
*/

static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	UB newstate;
//	UB state;
	UB	set_ibiki_mask = 0x01;
	UB	set_kokyu_mask = 0x02;
	UB	bit_shift = 0;
	
	calculator_apnea(&s_unit.kokyu_val[0], &s_unit.ibiki_val[0]);
	s_unit.kokyu_cnt = 0;
	newstate = get_state();
//	AlarmApnea(s_unit.calc.info.dat.state, newstate);
//	state = (s_unit.calc.info.dat.state & 0x3F);
//	s_unit.calc.info.dat.state = (newstate | state);
	
	bit_shift = s_unit.phase_kokyu * 2;
	if(newstate == APNEA_ERROR){
		s_unit.calc.info.dat.state |= (set_kokyu_mask << bit_shift);		// 無呼吸状態ON
		if(act_mode != ACT_MODE_MONITOR)
		{//モニタリングモードでないならバイブレーション動作
			set_vib(set_vib_mode(s_unit.alarm.info.dat.yokusei_str));
		}
	}else{
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// 無呼吸状態OFF
	}
	// もし、いびきも無呼吸もどちらもセットされたらいびきを優先するため、いびき状態とする
	if( (s_unit.calc.info.dat.state >> bit_shift) & 0x03 == 0x03 ){
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// 無呼吸状態OFF
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// いびき状態ON
	}
	
	s_unit.phase_kokyu++;
	if(s_unit.phase_kokyu >= SEC_PHASE_NUM){
		s_unit.phase_kokyu = SEC_PHASE_0_10;
	}
	
#endif
	
	return (KE_MSG_CONSUMED);
}


static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	//演算正規処理
	int ii;
	int max = s_unit.ibiki_val[0];
	static const int size = 9;
	UB newstate;
//	UB state;
	UB	set_ibiki_mask = 0x01;
	UB	set_kokyu_mask = 0x02;
	UB	bit_shift = 0;

#if 0 // テスト用データ
	static const UH testdata[200] = {
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000, 1000,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
		   0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
	};
#endif
	
	for(ii=0;ii<s_unit.ibiki_cnt;++ii){
		if(max < s_unit.ibiki_val[ii]){
			max = s_unit.ibiki_val[ii];
		}
	}
	s_unit.calc.info.dat.ibiki_val[s_unit.phase_ibiki] = max;

	// いびき演算
	calc_snore_proc(&s_unit.ibiki_val[0]);
	//calc_snore_proc(&testdata[0]);
	newstate = calc_snore_get();
//	AlarmSnore(s_unit.calc.info.dat.state, newstate);
//	state = (s_unit.calc.info.dat.state & 0xFE);
//	s_unit.calc.info.dat.state = (newstate | state);
	
	bit_shift = s_unit.phase_ibiki * 2;
	if(newstate == SNORE_ON){
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// いびき状態ON
		if(act_mode != ACT_MODE_MONITOR)
		{//モニタリングモードでないならバイブレーション動作
			set_vib(set_vib_mode(s_unit.alarm.info.dat.yokusei_str));
		}
	}else{
		s_unit.calc.info.dat.state &= ~(set_ibiki_mask << bit_shift);		// いびき状態OFF
	}
	// もし、いびきも無呼吸もどちらもセットされたらいびきを優先するため、いびき状態とする
	if( (s_unit.calc.info.dat.state >> bit_shift) & 0x03 == 0x03 ){
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// 無呼吸状態OFF
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// いびき状態ON
	}
	
	s_unit.phase_ibiki++;
	if(s_unit.phase_ibiki >= SEC_PHASE_NUM){
		s_unit.phase_ibiki = SEC_PHASE_0_10;
	}	
	
	
	// 移動累計とるので前のデータを残す
	for(ii=0;ii<size;++ii){
		s_unit.ibiki_val[ii] = s_unit.ibiki_val[DATA_SIZE_APNEA-size+ii];
	}
	s_unit.ibiki_cnt = size;

#else
	//デバッグ用ダミー処理
	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用

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
	
	UB	body_direct = BODY_DIRECTION_LEFT;
	UB	clear_mask = BODY_DIRECTION_MASK;
	UB	bit_shift = 0;
	
	// 最新のデータを使う
	acc_x = s_unit.acl_x[s_unit.acl_cnt];
//	acc_y = s_unit.acl_y[s_unit.acl_cnt];		//現状未使用
	acc_z = s_unit.acl_z[s_unit.acl_cnt];

	s_unit.acl_cnt = 0;
	
	// 体の向き判定
	if( 0 <= acc_x )
	{// 上 or 左
		if( 0 <= acc_z )
		{// 左
			body_direct = BODY_DIRECTION_LEFT;
		} else {
		 // 上
			body_direct = BODY_DIRECTION_UP;
		}
	} else {
	// 下 or 右
		if( 0 <= acc_z )
		{// 下
			body_direct = BODY_DIRECTION_DOWN;
		} else {
		 // 右
			body_direct = BODY_DIRECTION_RIGHT;
		}
	}
	
	// 10秒ごとの判定値をbitで設定する
	bit_shift = s_unit.phase_body_direct * BODY_DIRECTION_BIT;
	s_unit.calc.info.dat.body_direct &= ~(clear_mask << bit_shift);
	s_unit.calc.info.dat.body_direct |= (body_direct << bit_shift);
	// ■暫定 本関数は10秒に1回呼び出されることを前提とし、10秒ごとに秒間フェイズを進める
	s_unit.sec10_cnt++;
	if(s_unit.sec10_cnt >= 10){
		s_unit.sec10_cnt = 0;
		
		s_unit.phase_body_direct++;
		if(s_unit.phase_body_direct >= SEC_PHASE_NUM){
			s_unit.phase_body_direct = SEC_PHASE_0_10;
		}
	}

	return (KE_MSG_CONSUMED);
}


/************************************************************************/
/* 関数     : user_main_sleep											*/
/* 関数名   : ユーザーアプリのスリープチェック							*/
/* 引数     : なし														*/
/* 戻り値   : true		sleep有効										*/
/*          : false		sleep無効										*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* ユーザーアプリのスリープチェック										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
bool user_main_sleep(void)
{
#if FUNC_DEBUG_SLEEP_NON == ON
	return false;		//SLEEP無効
#else
	bool ret = true;
	
	if( OFF == cpu_com_get_can_sleep() ){
		ret = false;
		NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	}
	
	if( ret == true ){
		NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	}
	
	return ret;
#endif

}

/************************************************************************/
/* 関数     : user_main_eep_read_pow_on									*/
/* 関数名   : EEP読み出し処理(起動時)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* EEP読み出し処理(起動時)												*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void user_main_eep_read_pow_on(void)
{
	UB eep_type;
	
	// EEP種別チェック
	eep_read( EEP_ADRS_DATA_TYPE, &eep_type, 1 );
	
	if( EEP_DATA_TYPE_NORMAL != eep_type){
		eep_part_erase();
	}
	
	// フレーム関連
	eep_read( EEP_ADRS_TOP_SETTING, &s_unit.frame_num.read, 1 );
	eep_read( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num.write, 1 );
	eep_read( EEP_ADRS_TOP_SETTING + 2, &s_unit.frame_num.cnt, 1 );
	
	// 範囲チェック
	if(( s_unit.frame_num.read > ( EEP_FRAME_MAX - 1)) ||
	   ( s_unit.frame_num.write > ( EEP_FRAME_MAX - 1)) ||
	   ( s_unit.frame_num.cnt > EEP_FRAME_MAX )){
		err_info(ERR_ID_MAIN);
		// 範囲外なら初期化
		s_unit.frame_num.read = 0;
		s_unit.frame_num.write = 0;
		s_unit.frame_num.cnt = 0;
		eep_write( EEP_ADRS_TOP_SETTING, &s_unit.frame_num.read, 1, ON );
		eep_write( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num.write, 1, ON );
		eep_write( EEP_ADRS_TOP_SETTING + 2, &s_unit.frame_num.cnt, 1, ON );
	}
	
	
	// 警告機能
	eep_read( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE );
	// RD8001暫：範囲チェック入れる

	
}

/************************************************************************/
/* 関数     : eep_all_erase												*/
/* 関数名   : EEP全消去													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.01.25 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : EEP全消去														*/
/************************************************************************/
/* 注意事項 : 															*/
/* �@時間がかかる為に使用する際は注する事。約7.5Sec						*/
/************************************************************************/
#if 0		//未使用関数
STATIC void eep_all_erase( void )
{
	UW adrs = 0;
	UW i = 0;
	UB eep_data = EEP_DATA_TYPE_NORMAL;
	
	for( i = 0; i < (EEP_DATA_SIZE_ALL / EEP_ACCESS_ONCE_SIZE); i++ ){
		adrs = i * EEP_ACCESS_ONCE_SIZE;
		eep_write( adrs, (UB*)&s_eep_page0_tbl[0], EEP_ACCESS_ONCE_SIZE, ON );
	}
	
	// 消去済みとして通常種別を書き込む
	eep_write( EEP_ADRS_DATA_TYPE, &eep_data, 1, ON );
	
}
#endif

/************************************************************************/
/* 関数     : eep_part_erase											*/
/* 関数名   : EEP部分消去(測定データ以外)								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.13 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : EEP部分消去(測定データ以外)									*/
/************************************************************************/
/* 注意事項 : 															*/
/* �@測定データ消去には時間がかかる為に消去しない						*/
/************************************************************************/
STATIC void eep_part_erase( void )
{
	UB eep_data = EEP_DATA_TYPE_NORMAL;
	
	eep_write( EEP_ADRS_TOP_SETTING, (UB*)&s_eep_page0_tbl[0], EEP_SETTING_SIZE, ON );
	memset( &s_unit.frame_num, 0, sizeof(s_unit.frame_num) );
	eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_eep_page0_tbl[0], EEP_ALARM_SIZE, ON );
	memset( &s_unit.alarm, 0, sizeof(s_unit.alarm) );
	
	// 消去済みとして通常種別を書き込む
	eep_write( EEP_ADRS_DATA_TYPE, &eep_data, 1, ON );
}


// =====================================
// H1Dプログラム転送コード
// =====================================

/************************************************************************/
/* 関数     : main_vuart_rcv_prg_hd_record								*/
/* 関数名   : H1Dプログラム書き換え(レコード書き込み)					*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(レコード書き込み)								*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_rcv_prg_hd_record( void )
{
	UW adr;
	
	if( s_unit.prg_hd_eep_record_cnt_wr >= EEP_PRG_H1D_RECODE_CNT_MAX ){
		return;
	}
	
	adr = ( s_unit.prg_hd_eep_record_cnt_wr * ( EEP_PRG_H1D_RECODE_UNIT + EEP_PRG_H1D_RECODE_OFFSET) );
	eep_write( adr, &s_ds.vuart.input.rcv_data[0], EEP_PRG_H1D_RECODE_UNIT, ON );
#if 0	//ベリファイ
	eep_read( adr, &s_ds.cpu_com.order.snd_data[0], EEP_PRG_H1D_RECODE_UNIT );
	if (memcmp( &s_ds.vuart.input.rcv_data[0], &s_ds.cpu_com.order.snd_data[0], EEP_PRG_H1D_RECODE_UNIT) != 0){
		NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	}
#endif
	// サム値計算 ※データ
	calc_sum_uw_cont( &s_unit.prg_hd_eep_code_record_sum, &s_ds.vuart.input.rcv_data[4], EEP_PRG_H1D_RECODE_UNIT -4 );
	s_unit.prg_hd_eep_record_cnt_wr++;
}

/************************************************************************/
/* 関数     : main_vuart_rcv_prg_hd_result								*/
/* 関数名   : H1Dプログラム書き換え(転送結果)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(転送結果)										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_rcv_prg_hd_result(void)
{
	//サム値組立
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
	
	//サム値比較 RD8001暫定：EEPチェックの方が良いか？ EEP異常時の動作未定義
	if( s_unit.prg_hd_eep_code_record_sum != recv_sum ){
		ret = NG;
	}else{
		ret = OK;
	}
	
	//---応答---
	{
		UB tx[VUART_DATA_SIZE_MAX] = {0};
		tx[0] = VUART_CMD_PRG_RESULT;
		tx[1] = ret;
	
		main_vuart_send( &tx[0], 2 );
	}
	
	if( OK == ret ){
		// ■暫定 7/30 ブリッジは消さないといけないと思われる
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_READY;
		s_ds.cpu_com.order.data_size = 0;
	}

}

/************************************************************************/
/* 関数     : main_vuart_rcv_prg_hd_update								*/
/* 関数名   : H1Dプログラム書き換え(完了確認)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(完了確認)										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_vuart_rcv_prg_hd_update(void)
{
#if FUNC_DEBUG_PRG_H1D_U == ON
	// デバッグ用処理
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
		UB tx[VUART_DATA_SIZE_MAX] = {0};
		tx[0] = VUART_CMD_PRG_CHECK;
		tx[1] = s_unit.prg_hd_update_state;
		tx[2] = 0x00;		// 予備領域
		tx[3] = 0x00;		// 予備領域
		tx[4] = 0x00;		// 予備領域
		tx[5] = 0x00;		// 予備領域
	
		main_vuart_send( &tx[0], 6 );
		if( OK_NOW_UPDATING != s_unit.prg_hd_update_state ){
			s_unit.prg_hd_seq = PRG_SEQ_COMPLETE_WAIT;
		}
	}
}



/************************************************************************/
/* 関数     : main_cpu_com_rcv_prg_hd_ready								*/
/* 関数名   : H1Dプログラム書き換え(転送準備)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(転送準備)										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_cpu_com_rcv_prg_hd_ready(void)
{
	s_unit.timer_sec = TIMER_SEC_PRG_READY_WAIT;
	s_unit.prg_hd_seq = PRG_SEQ_READY_WAIT;
	
}

/************************************************************************/
/* 関数     : main_cpu_com_rcv_prg_hd_start								*/
/* 関数名   : H1Dプログラム書き換え(転送開始)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(転送開始)										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_cpu_com_rcv_prg_hd_start(void)
{
	s_unit.timer_sec = TIMER_SEC_PRG_START_WAIT;
	s_unit.prg_hd_seq = PRG_SEQ_START_WAIT;
}



/************************************************************************/
/* 関数     : main_cpu_com_rcv_prg_hd_erase								*/
/* 関数名   : H1Dプログラム書き換え(消去)								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(消去)											*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_cpu_com_rcv_prg_hd_erase(void)
{
	s_unit.timer_sec = TIMER_SEC_PRG_ERASE_WAIT;
	s_unit.prg_hd_seq = PRG_SEQ_ERASE_WAIT;
}
	
/************************************************************************/
/* 関数     : main_cpu_com_rcv_prg_hd_erase								*/
/* 関数名   : H1Dプログラム書き換え(データ)								*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(データ)										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_cpu_com_rcv_prg_hd_data(void)
{
	if( s_unit.prg_hd_eep_record_cnt_rd < s_unit.prg_hd_eep_record_cnt_wr ){
		// 継続
		main_prg_hd_read_eep_record();
	}else{
		// 終了
		s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_RESLUT;
		s_ds.cpu_com.order.snd_data[0] = (UB)(  s_unit.prg_hd_eep_code_record_sum & (UW)0x000000FF );
		s_ds.cpu_com.order.snd_data[1] = (UB)(( s_unit.prg_hd_eep_code_record_sum & (UW)0x0000FF00 ) >> (UW)8 );
		s_ds.cpu_com.order.snd_data[2] = (UB)(( s_unit.prg_hd_eep_code_record_sum & (UW)0x00FF0000 ) >> (UW)16 );
		s_ds.cpu_com.order.snd_data[3] = (UB)(( s_unit.prg_hd_eep_code_record_sum & (UW)0xFF000000 ) >> (UW)24 );
		s_ds.cpu_com.order.data_size = 4;
	}
}


/************************************************************************/
/* 関数     : main_cpu_com_rcv_prg_hd_erase								*/
/* 関数名   : H1Dプログラム書き換え(転送結果)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(転送結果)										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
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

/************************************************************************/
/* 関数     : main_cpu_com_rcv_prg_hd_erase								*/
/* 関数名   : H1Dプログラム書き換え(転送確認)							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(転送確認)										*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
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

/************************************************************************/
/* 関数     : main_cpu_com_rcv_prg_hd_erase								*/
/* 関数名   : H1Dプログラム書き換え(1レコード分を準備)					*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* H1Dプログラム書き換え(1レコード分を準備)								*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
STATIC void main_prg_hd_read_eep_record( void )
{
	UW adr;
	
	adr = ( s_unit.prg_hd_eep_record_cnt_rd * ( EEP_PRG_H1D_RECODE_UNIT + EEP_PRG_H1D_RECODE_OFFSET ) );
	eep_read( adr, &s_ds.cpu_com.order.snd_data[0], EEP_PRG_H1D_RECODE_UNIT );
	s_ds.cpu_com.order.snd_cmd_id = CPU_COM_CMD_PRG_DOWNLORD_DATA;
	s_ds.cpu_com.order.data_size = EEP_PRG_H1D_RECODE_UNIT;
	s_unit.prg_hd_eep_record_cnt_rd++;
}




#if 0
STATIC void AlarmSnore(UB oldstate, UB newstate)
{
	// debug向け
//	main_vuart_snd_alarm_info(0,1);
	
	// いびきアラーム
	if((s_unit.alarm.info.dat.valid == 1) && (s_unit.alarm.info.dat.ibiki == 1)){
		if((oldstate & 0x01) != (newstate & 0x01)){
			if(newstate & 0x01){
				// いびき発生
				main_vuart_snd_alarm_info(0,0);
			}else{
				// いびき解除
				main_vuart_snd_alarm_info(0,1);
			}
		}
	}
}

STATIC void AlarmApnea(UB oldstate, UB newstate)
{
	// debug向け
//	main_vuart_snd_alarm_info(0,1);
	
	// 無呼吸アラーム
	if((s_unit.alarm.info.dat.valid == 1) && (s_unit.alarm.info.dat.low_kokyu == 1)){
		if(((oldstate & 0xC0) != 0x00) && ((newstate & 0xC0) == 0x00)){
			// 無呼吸解除
			main_vuart_snd_alarm_info(1,1);
		}else if(((oldstate & 0xC0) == 0x00) && ((newstate & 0xC0) != 0x00)){
			// 無呼吸発生
			main_vuart_snd_alarm_info(1,0);
		}
	}
}
#endif

//================================
//ACL関連
//================================
/************************************************************************/
/* 関数     : main_acl_init												*/
/* 関数名   : 加速度センサ初期化										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.13 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 加速度センサ初期化 ※初期状態は停止にしておく					*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_acl_init(void)
{
	UB rd_data[2];
	
	wait_ms( 30 );		// 加速度センサ　※電源ON待ち

	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_WHO_AM_I, &rd_data[0], 1 );
	if( rd_data[0] != ACL_REG_RECOGNITION_CODE ){
		err_info( ERR_ID_ACL );
	}
	
	main_acl_stop();
}

/************************************************************************/
/* 関数     : main_acl_stop												*/
/* 関数名   : 加速度センサ停止											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 加速度センサ停止												*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_acl_stop(void)
{
	UB rd_data[2];
	UB wr_data[2];
	
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	// 動作モード設定
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_CTRL_REG1, &rd_data[0], 1 );
	if( rd_data[0] != 0x00 ){
		err_info( ERR_ID_ACL );
	}
}

/************************************************************************/
/* 関数     : main_acl_stop												*/
/* 関数名   : 加速度センサスタート										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 加速度センサスタート											*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_acl_start(void)
{
	UB wr_data[2];
	
	// 動作モード初期化
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	// 動作モード設定
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x20;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );

	// 動作モード開始
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0xA0;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	
}

/************************************************************************/
/* 関数     : main_acl_stop												*/
/* 関数名   : 加速度センサ読出し										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.09.11 Axia Soft Design 西島 稔	初版作成			*/
/************************************************************************/
/* 機能 : 加速度センサス読出し											*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void main_acl_read(void)
{
	UB rd_data[10];
	
	// INT_SOURCE1		
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_INT_SRC1, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & BIT04 )){
		// データ未達
		err_info( ERR_ID_ACL );
		return;
	}
	
	// データ取得
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_DATA_XYZ, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
	// INT_REL読み出し　※割り込み要求クリア
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_INT_REL, &rd_data[0], 1 );
}

/************************************************************************/
/* 関数     : main_photo_read											*/
/* 関数名   : フォトセンサー読出し										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC UH main_photo_read(void)
{
	UH photoref_off_val;
	UH photoref_on_val;
	UH ret_photoref_val = 0;
	
	// フォトセンサー値取得
	// OFF
	adc_photoreflector( &photoref_off_val );
	
	// ON
	P2_bit.no0 = 0;

	wait_ms(1);
	adc_photoreflector( &photoref_on_val );
	
	// OFF
	P2_bit.no0 = 1;
	
	if(photoref_on_val > photoref_off_val)
	{
		ret_photoref_val = photoref_on_val - photoref_off_val;
	}
	
	return ret_photoref_val;
}

/************************************************************************/
/* 関数     : reset_vib_timer											*/
/* 関数名   : バイブレーションタイマー初期化							*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void reset_vib_timer(void)
{
	s_unit.tick_vib_10ms_sec = 0;
}

/************************************************************************/
/* 関数     : reset_led_timer											*/
/* 関数名   : LEDタイマー初期化											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.08.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void reset_led_timer(void)
{
	s_unit.sec10_led_cnt = 0;
}

/************************************************************************/
/* 関数     : main_set_battery											*/
/* 関数名   : 電池残量設定												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.26 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void main_set_battery(void)
{
	UH battery_val = BAT_LEVEL_1_VAL;
	
	adc_battery( &battery_val );
	
	if( battery_val >= BAT_LEVEL_1_VAL ){
		s_unit.battery_sts = BAT_LEVEL_STS_MAX;
	}else if( battery_val >= BAT_LEVEL_2_VAL ){
		s_unit.battery_sts = BAT_LEVEL_STS_HIGH;
	}else if( battery_val >= BAT_LEVEL_3_VAL ){
		s_unit.battery_sts = BAT_LEVEL_STS_LOW;
	}else{
		s_unit.battery_sts = BAT_LEVEL_STS_MIN;
	}
}
