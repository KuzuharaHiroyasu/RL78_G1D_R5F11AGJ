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
static int_t user_main_calc_result_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t led_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t battery_level_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_photoref(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

#if FUNC_DEBUG_LOG != ON
static int_t user_main_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
STATIC void user_main_calc_data_set_kyokyu_ibiki( void );
STATIC void user_main_mode( void );
STATIC void user_main_mode_common( void );
STATIC void main_vuart_proc(void);
#else
STATIC void make_send_data(char* pBuff);
STATIC void user_main_calc_data_set_kyokyu_ibiki( void );
static UB main_calc_kokyu( void);
static UB main_calc_ibiki( void);
#endif
static void set_suppress_cnt_time(UB suppress_max_time);

STATIC void user_main_mode_inital(void);
STATIC void user_main_mode_idle_com(void);
STATIC void user_main_mode_sensing(void);
STATIC void user_main_mode_move(void);
STATIC void user_main_mode_get(void);
STATIC void user_main_mode_prg_g1d(void);
STATIC void main_mode_chg( void );
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode );
STATIC void main_vuart_rcv_mode_chg( void );
STATIC void main_vuart_rcv_date( void );
STATIC void main_vuart_rcv_info( void );
STATIC void main_vuart_rcv_version( void );
STATIC void main_vuart_rcv_device_info( void );
STATIC void sw_proc(void);
STATIC void user_main_calc_result( void );
STATIC void user_main_mode_sensing_before( void );
STATIC void user_main_mode_sensing_after( void );
STATIC void user_main_mode_self_check( void );
STATIC UB user_main_mode_get_frame_before( void );
STATIC UB evt_act( EVENT_NUM evt );
STATIC SYSTEM_MODE evt_non( int evt);
STATIC SYSTEM_MODE evt_idle_com( int evt);
STATIC SYSTEM_MODE evt_idle_com_denchi( int evt);
#if 0
STATIC SYSTEM_MODE evt_sensing( int evt);
#endif
STATIC SYSTEM_MODE evt_sensing_chg( int evt);
STATIC SYSTEM_MODE evt_bat_check( int evt);
#if 0
STATIC SYSTEM_MODE evt_send_clear( int evt);
#endif
STATIC SYSTEM_MODE evt_get( int evt);
STATIC SYSTEM_MODE evt_g1d_prg_denchi( int evt);
STATIC SYSTEM_MODE evt_self_check( int evt);
STATIC SYSTEM_MODE evt_remove( int evt);
STATIC SYSTEM_MODE evt_time_out( int evt);
STATIC void user_main_mode_get_after( void );
STATIC void user_main_eep_read_pow_on(void);
STATIC void eep_part_erase( void );
STATIC void main_vuart_send( UB *p_data, UB len );
STATIC void main_vuart_rcv_data_fin( void );
//STATIC void eep_all_erase( void );		//未使用関数
void main_vuart_set_mode( void );
void main_vuart_rcv_data_frame( void );
void main_vuart_rcv_data_calc( void );
void main_vuart_rcv_data_end( void );
void main_vuart_rcv_date( void );
void main_vuart_rcv_device_set( void );
void main_vuart_rcv_vib_confirm( void );
void main_vuart_rcv_vib_stop( void );
void main_vuart_rcv_power_off( void );

// ACL、フォト関連
STATIC void main_acl_init(void);
STATIC void main_acl_stop(void);
STATIC void main_acl_start(void);
#if (FUNC_DEBUG_LOG != ON) || (FUNC_DEBUG_WAVEFORM_LOG != ON)
STATIC void main_acl_read(void);
STATIC UH main_photo_read(void);
#endif

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
STATIC T_UNIT_SAVE s_unit_save;
STATIC DS s_ds;

static bool vib_flg = false;
static UB act_mode = ACT_MODE_SUPPRESS_SNORE;
static UB vib_power = VIB_MODE_DURING;
static UH suppress_max_cnt = MAX_SUPPRESS_CONT_TIME_10_MIN_CNT;

#if FUNC_DEBUG_LOG != ON
static UB suppress_start_time = SUPPRESS_START_CNT;
static UB suppress_max_cnt_over_flg = OFF;
static UB acl_photo_sens_read_flg = OFF;
static bool bat_check_flg = false;
static bool apnea_data_max = false;
static bool snore_data_max = false;
static bool acl_data_max = false;
static bool photo_data_max = false;
#else
static UB suppress_start_time = SUPPRESS_START_CNT;
static UB sw_on_flg = OFF;
static UB snore_state;
static UB apnea_state;
void set_serial_command(char dbg_rx_data);
#endif

static B	vib_level = VIB_LEVEL_1;
static bool vib_startflg = false;
static UB vib_start_limit_cnt = 0;

// 検査モード
static bool diagStartFlg = false;

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
#if FUNC_DEBUG_LOG != ON
	uint8_t *ke_msg;
#endif	
	ke_evt_clear(KE_EVT_USR_2_BIT);
	
#if FUNC_DEBUG_LOG != ON
	// 秒タイマーカウントダウン
	DEC_MIN( s_unit.timer_sec ,0 );
	
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
				main_vuart_send( &tx[0], VUART_SND_LEN_PRG_G1D_VER );
			}
		}
	}
	
	// G1Dダウンロード_応答を返してからアップデートを開始するための待ちに入る
	if( ON == s_unit.prg_g1d_update_wait_flg ){
		DEC_MIN( s_unit.prg_g1d_update_wait_sec ,0 );
		if( 0 == s_unit.prg_g1d_update_wait_sec ){
			s_unit.prg_g1d_update_wait_flg = OFF;
			{
				FW_Update_Receiver_Start();
			}
		}
	}
	
	if( SYSTEM_MODE_SENSING != s_unit.system_mode ){
		return;
	}
	
	// フォトセンサー
	if(s_unit.photosens_remove_cnt >= PHOTO_SENSOR_REMOVE_CNT)
	{
		// 30分間外れていた（閾値を下回っている）
		evt_act(EVENT_REMOVE_TIMEOUT);
	}
	
	// センシング時間上限検知
	if( s_unit.calc_cnt >= EEP_CALC_DATA_NUM )
	{
		// 12時間を超えたなら待機モードへ
		evt_act( EVENT_COMPLETE );
	}

	// 加速度センサ、フォトセンサ値演算(10秒周期)
	s_unit.sec10_cnt++;
	if(s_unit.sec10_cnt >= CALC_ACL_PHOTO_WR_CYC){
		s_unit.sec10_cnt -= CALC_ACL_PHOTO_WR_CYC;

		// 加速度演算
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_ACL, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
		
		// フォトセンサ値
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_PHOTOREF, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	// 電池残量取得(10分周期)
	s_unit.sec600_cnt++;
	if(s_unit.sec600_cnt >= BAT_LEVEL_GET_CYC)
	{
		s_unit.sec600_cnt -= BAT_LEVEL_GET_CYC;
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_BATTERY, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	// 取得データ保存(30秒周期)
	s_unit.sec30_cnt++;
	if( s_unit.sec30_cnt >= CALC_RESULT_WR_CYC ){		// 30秒
		if(apnea_data_max == true && snore_data_max == true && snore_data_max == true && acl_data_max == true && photo_data_max == true)
		{ // 無呼吸判定、いびき判定、首の向き、フォトセンサの値が３つ(0～10、10～20、20～30)入っていれば保存
			s_unit.sec30_cnt -= CALC_RESULT_WR_CYC;
			
			ke_msg = ke_msg_alloc( USER_MAIN_CYC_CALC_RESULT, USER_MAIN_ID, USER_MAIN_ID, 0 );
			ke_msg_send(ke_msg);

			// フラグを戻す
			apnea_data_max = false;
			snore_data_max = false;
			acl_data_max = false;
			photo_data_max = false;
		}
	}
	
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
		
		// ユーザーアプリ周期処理
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_ACT, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
		
		// LED
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_LED, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
#endif
}

#if FUNC_DEBUG_LOG != ON
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
	sw_proc();				// SW検知処理
	
	main_vuart_proc();		// VUART通信サービス
	
	user_main_mode();		// メインモード処理

	return (KE_MSG_CONSUMED);
}
#endif

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
/* ①スリープ中でも有効なタイマー										*/
/************************************************************************/
void user_main_timer_10ms_set( void )
{
	s_unit.tick_10ms++;
	s_unit.tick_10ms_sec++;
	s_unit.tick_10ms_new++;
	s_unit.elapsed_time++;
	s_unit.tick_vib_10ms_sec++;
	s_unit.tick_diag_10ms++;
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
#if FUNC_DEBUG_LOG != ON
	UB bat;
#else
	char dbg_rcv_data = 0;
	
	if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_50MSEC){
		sw_proc();				// SW検知処理
	}
#endif
	if(s_unit.system_mode == SYSTEM_MODE_SENSING)
	{
		// 50ms周期
		if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_50MSEC){
#if FUNC_DEBUG_LOG == ON
			char dbg_tx_data[50] = {0};
			int dbg_len;
	
			// 呼吸音、いびき音取得
			adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
			
			if(vib_startflg == true)
			{
				if( (s_unit.meas.info.dat.ibiki_val < BREATH_VALLEY) || ( VIB_START_LIMIT <= vib_start_limit_cnt) )
				{
					vib_startflg = false;
					vib_start_limit_cnt = 0;
					set_vib(set_vib_mode(vib_power));
				}else{
					vib_start_limit_cnt++;
				}
			}
#if FUNC_DEBUG_WAVEFORM_LOG == ON
	// 波形&結果確認
			user_main_calc_data_set_kyokyu_ibiki();
#else
			user_main_calc_data_set_kyokyu_ibiki();
	//通常デバッグ版
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
#endif
			make_send_data(dbg_tx_data);
			dbg_len = strlen(dbg_tx_data);
			com_srv_send(dbg_tx_data, dbg_len);
#else
			// 呼吸音、いびき音取得
			adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
			user_main_calc_data_set_kyokyu_ibiki();
			
			if(vib_startflg == true)
			{
				if( (s_unit.meas.info.dat.ibiki_val < BREATH_VALLEY) || ( VIB_START_LIMIT <= vib_start_limit_cnt) )
				{
					vib_startflg = false;
					vib_start_limit_cnt = 0;
					set_vib(set_vib_mode(vib_power));
				}else{
					vib_start_limit_cnt++;
				}
			}
			
			s_unit.acl_timing+=1;
			
			// 5秒後に取得
			if(s_unit.acl_timing >= ACL_TIMING_VAL){
				if( acl_photo_sens_read_flg == OFF)
				{
					// 加速度取得
					main_acl_read();
					
					// フォトセンサー値取得
					s_unit.meas.info.dat.photoref_val = main_photo_read();
					
					acl_photo_sens_read_flg = ON;
					
					if( bat_check_flg != true )
					{
						set_led( LED_PATT_GREEN_BLINK_SENSING );
					}
				}
				
				// センサー値取得5秒後にリセット
				if(s_unit.acl_timing >= ACL_RESET_TIMING_VAL)
				{
					s_unit.acl_timing = 0;
					acl_photo_sens_read_flg = OFF;
					if( bat_check_flg != true )
					{
						set_led( LED_PATT_GREEN_BLINK_SENSING );
					}
				}
			}
			
			//充電検知
			bat = drv_i_port_bat_chg_detect();
			if(bat == ON)
			{
				//充電中なら待機モードへ
				evt_act( EVENT_CHG_PORT_ON );
			}
#endif
			s_unit.tick_10ms_new = 0;
		}
	}
	
	// シリアルデータ受信
	if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_10MSEC){
		com_srv_rcv(&dbg_rcv_data, 0);

		if(com_get_read_status() == DRV_CPU_COM_STATUS_RECEIVE_COMP)
		{
			set_serial_command(dbg_rcv_data);
			NO_OPERATION_BREAK_POINT();
		}
	}
			
	// 20ms周期
	if(s_unit.tick_10ms >= (uint16_t)PERIOD_20MSEC){
		ke_evt_set(KE_EVT_USR_3_BIT);

		s_unit.tick_10ms -= PERIOD_20MSEC;
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
	led_start(s_unit.tick_led_20ms_sec);
	s_unit.tick_led_20ms_sec++;
	
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
	if(vib_flg == true)
	{
		// バイブレーション(10ms周期)
		if(s_unit.tick_vib_10ms_sec >= (uint16_t)PERIOD_10MSEC)
		{
	   		vib_start(s_unit.tick_vib_10ms_sec);
		}
	}
}

/************************************************************************/
/* 関数     : set_vib_flg												*/
/* 関数名   : 						*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.08.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 							*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
void set_vib_flg( bool flg )
{
	vib_flg = flg;
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
	
	// 電池残量検知
	if(s_unit.battery_sts == BAT_LEVEL_STS_MIN)
	{
		// 電池残量なしなら待機モードへ
		evt_act( EVENT_DENCH_LOW );
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
#if FUNC_DEBUG_LOG != ON
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
	if( s_unit.kokyu_cnt >= ( DATA_SIZE - 1 )){
		s_unit.suppress_start_cnt++;
		
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_KOKYU, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}

	if( s_unit.ibiki_cnt >= ( DATA_SIZE - 1 )){
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_IBIKI, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	INC_MAX( s_unit.kokyu_cnt, MEAS_KOKYU_CNT_MAX );
	INC_MAX( s_unit.ibiki_cnt, MEAS_IBIKI_CNT_MAX );

	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
}
#else
STATIC void user_main_calc_data_set_kyokyu_ibiki( void )
{
	if( s_unit.kokyu_cnt < MEAS_KOKYU_CNT_MAX ){
		s_unit.kokyu_val[s_unit.kokyu_cnt] = s_unit.meas.info.dat.kokyu_val;
	}
	if( s_unit.ibiki_cnt < MEAS_IBIKI_CNT_MAX ){
		s_unit.ibiki_val[s_unit.ibiki_cnt] = s_unit.meas.info.dat.ibiki_val;
	}
	
	// データフルで演算呼出
	if( s_unit.kokyu_cnt >= ( DATA_SIZE - 1 )){
		apnea_state = main_calc_kokyu();
	}else
	{
		apnea_state = 99;
	}

	if( s_unit.ibiki_cnt >= ( DATA_SIZE - 1 )){
		snore_state = main_calc_ibiki();
	}else
	{
		snore_state = 99;
	}
	
	
	INC_MAX_INI( s_unit.kokyu_cnt, MEAS_KOKYU_CNT_MAX - 1, 0 );		
	INC_MAX_INI( s_unit.ibiki_cnt, MEAS_IBIKI_CNT_MAX - 1, 0 );		

	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
}
#endif

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
	
#if FUNC_DEBUG_WAVEFORM_LOG == OFF
	//通常デバッグ版
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
#else
	// いびき判定結果
	tmp = snore_state / 10;
	next = snore_state % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;
	pBuff[index++] = ',';
	
	// 無呼吸判定結果
	tmp = apnea_state / 10;
	next = apnea_state % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;
#endif
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
	R_WDT_Create();
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
#if FUNC_DEBUG_LOG == ON
	com_srv_init();
#endif
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
	
	s_unit.system_mode = SYSTEM_MODE_IDLE_COM;
	set_ble_state(BLE_STATE_INITIAL);
	set_ble_isconnect(false);
	
#if FUNC_DEBUG_PORT == ON
	//空きポートによる計測用設定(スリープなど)
    write1_sfr(PM1,  5, PORT_OUTPUT);
//    write1_sfr(PM1,  6, PORT_OUTPUT);		//使うとBLEが動かなくなるので削除

    write1_sfr(P1, 5, 0);
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
	
#if FUNC_DEBUG_LOG == ON
	if( ON == pow_sw ){		// ON処理
		// 電源SW押下タイマー継続
		s_unit.sw_time_cnt++;
		if(s_unit.system_mode == SYSTEM_MODE_IDLE_COM && s_unit.sw_time_cnt > TIME_20MS_CNT_POW_SW_SHORT_DEBUG && sw_on_flg == OFF){
			sw_on_flg = ON;
			memset(&s_unit.kokyu_val, 0, sizeof(s_unit.kokyu_val));
			memset(&s_unit.ibiki_val, 0, sizeof(s_unit.ibiki_val));
			s_unit.kokyu_cnt = 0;
			s_unit.ibiki_cnt = 0;
			Reset();
			
			
			s_unit.system_mode = SYSTEM_MODE_SENSING;
			led_green_on();
			s_unit.sw_time_cnt = 0;
			set_vib(VIB_MODE_SENSING);
		}
		
		if(s_unit.system_mode == SYSTEM_MODE_SENSING && s_unit.sw_time_cnt > TIME_20MS_CNT_POW_SW_SHORT_DEBUG && sw_on_flg == OFF){
			sw_on_flg = ON;
			s_unit.system_mode = SYSTEM_MODE_IDLE_COM;
			led_green_on();
			s_unit.sw_time_cnt = 0;
			set_vib(VIB_MODE_STANDBY);
		}
	}else{
		sw_on_flg = OFF;
		s_unit.sw_time_cnt = 0;
		led_green_off();
	}
#else
	if( ON == pow_sw ){		// ON処理
		// 電源SW押下タイマー継続
		s_unit.sw_time_cnt++;
		
		if( s_unit.sw_time_cnt == TIME_20MS_CNT_POW_SW_LONG){
			// 規定時間以上連続押下と判断
			evt_act( EVENT_POW_SW_LONG );
		}
	}else{					// OFF処理
		if( ON == s_unit.pow_sw_last ){
			// ON→OFFエッジ
			if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_LONG){
				// ON確定時にイベント発生済みなのでここでは何もしない
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
#endif
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

	//範囲チェック
	if( s_unit.calc_cnt >= EEP_CALC_DATA_NUM ){
		err_info(ERR_ID_MAIN);
		return;
	}
	
	// フレーム位置とデータ位置からEEPアドレスを算出
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + ( s_unit.calc_cnt * EEP_CALC_DATA_SIZE );

	eep_write( wr_adrs, (UB*)&s_unit.calc, EEP_CALC_DATA_SIZE, OFF );	// 30秒周期なので5ms待ちはしない
	
	s_unit.calc_cnt++;
	
	/* いびき検知回数と無呼吸検知回数の確定処理 */
	s_unit.ibiki_detect_cnt_decided = s_unit.ibiki_detect_cnt;
	s_unit.mukokyu_detect_cnt_decided = s_unit.mukokyu_detect_cnt;
	
	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
}

#if FUNC_DEBUG_LOG != ON
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
	
	if( SYSTEM_MODE_IDLE_COM == s_unit.system_mode ){
		if(( now_time - s_unit.system_mode_time_out_cnt ) >= TIME_OUT_SYSTEM_MODE_IDLE_COM ){
			evt_act( EVENT_TIME_OUT );
		}
	}
	
	s_unit.last_system_mode = s_unit.system_mode;
}
#endif

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
	
	s_unit.device_set_info = 0;
	// 動作モード保存
	s_unit.device_set_info = s_unit.alarm.info.dat.act_mode;
	// バイブレーションの強さ保存
	s_unit.device_set_info |= s_unit.alarm.info.dat.suppress_power << 2;
	// いびき検出感度保存
	s_unit.device_set_info |= s_unit.alarm.info.dat.ibiki_sens << 4;
	// 無呼吸検出感度保存
	// 現在検出感度の設定なし
	
	// デバイス設定書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DEVICE_SET_INFO;
	eep_write( wr_adrs, &s_unit.device_set_info, EEP_DEVICE_SET_INFO_SIZE, ON );
	
	s_unit.suppress_start_time = s_unit.alarm.info.dat.suppress_start_time;
	// 抑制開始設定時間書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DEVICE_SET_SUPPRESS_START_TIME;
	eep_write( wr_adrs, &s_unit.suppress_start_time, EEP_DEVICE_SET_SUPPRESS_START_TIME_SIZE, ON );
	
	s_unit.calc_cnt = 0;
	s_unit.ibiki_detect_cnt = 0;
	s_unit.mukokyu_detect_cnt = 0;
	s_unit.ibiki_time = 0;
	s_unit.mukokyu_time = 0;
	s_unit.max_mukokyu_sec = 0;
	s_unit.cont_mukokyu_detect_cnt_max = 0;
	s_unit.cont_mukokyu_detect_cnt_current = 0;
	s_unit.ibiki_chg_detect_cnt = 0;
	s_unit.mukokyu_chg_detect_cnt = 0;
	s_unit.ibiki_state_flg = 0;
	s_unit.mukokyu_state_flg = 0;
	s_unit.photosens_remove_cnt = 0;
	s_unit.ibiki_detect_cnt_decided = 0;
	s_unit.mukokyu_detect_cnt_decided = 0;
	s_unit.suppress_start_cnt = 0;
	
	// センサー取得データをクリア
	memset(s_unit.kokyu_val, 0, MEAS_KOKYU_CNT_MAX);
	memset(s_unit.ibiki_val, 0, MEAS_IBIKI_CNT_MAX);
	Reset();
	
	s_unit.kokyu_cnt = 0;
	s_unit.ibiki_cnt = 0;
	
	s_unit.suppress_cont_time_cnt = 0;
	s_unit.sec10_cnt = 0;
	s_unit.sec30_cnt = 0;
	s_unit.sec600_cnt = 0;
	
#if FUNC_DEBUG_LOG != ON
	apnea_data_max = false;
	snore_data_max = false;
	acl_data_max = false;
	photo_data_max = false;
#endif
	
	// バイブレベル初期化
	vib_level = VIB_LEVEL_1;
	set_vib_level(vib_level);
	
	s_unit.sensing_flg = ON;
	
	// センシング移行時にバイブ動作
	set_vib(VIB_MODE_SENSING);
	
	/* BLEを無効化(電力消費量低減の為) */
	set_ble_state(BLE_STATE_ON); // BLE初期化完了時がわからないため、ここで状態更新する(これでOFF処理ができるようになる)
	RBLE_VS_RF_Control( RBLE_VS_RFCNTL_CMD_POWDOWN );
	
	// BLEのLEDを消灯(暫定)→本来はセンシング移行時BLE切断で消灯する
	led_green_off();
	set_ble_isconnect(false);
	
	vib_start_limit_cnt = 0;
	
	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
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
	UB wr_data[3] = {0};
	
	set_led(LED_PATT_GREEN_OFF);
	
	/* BLEを無効→有効化(APIリファレンスマニュアルの通り、電源ON後にResetする) */
	RBLE_VS_RF_Control( RBLE_VS_RFCNTL_CMD_POWUP_DDCON );
	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用
	
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
	
	// いびき時間、無呼吸時間更新(30秒保存周期ごとに確定したカウント数を使用する)
	s_unit.ibiki_time = s_unit.ibiki_detect_cnt_decided * SAMPLING_INTERVAL_SEC;
	s_unit.mukokyu_time = s_unit.mukokyu_detect_cnt_decided * SAMPLING_INTERVAL_SEC;
	// 最大無呼吸時間は継続回数の最大値から計算する
	s_unit.max_mukokyu_sec = s_unit.cont_mukokyu_detect_cnt_max * SAMPLING_INTERVAL_SEC;
	
	// いびき検知数書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_chg_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE, ON );
	// 無呼吸検知数書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_chg_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE, ON );
	// いびき時間書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE, ON );
	// 無呼吸時間書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE, ON );
	// 最高無呼吸時間書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE, ON );
	
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
		wr_data[0] = s_unit.frame_num.write;
		wr_data[1] = s_unit.frame_num.cnt;
		eep_write( wr_adrs + 1, &wr_data[0], 2, ON );
	}else{
		INC_MAX_INI(s_unit.frame_num.read, (EEP_FRAME_MAX  -1), 0);
		// 書き込み、読み出しポインタ
		wr_data[0] = s_unit.frame_num.read;
		wr_data[1] = s_unit.frame_num.write;
		wr_data[2] = s_unit.frame_num.cnt;
		eep_write( wr_adrs, &wr_data[0], 3, ON );
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
	
	if( calc_cnt > EEP_CALC_DATA_NUM ){
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
		main_vuart_send( &tx[0], VUART_SND_LEN_DATA_END );
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
	
	if(s_unit.get_mode_timeout > GET_MODE_TIME_OUT)
	{
		// 5秒間送信中状態ならタイムアウト
		evt_act( EVENT_TIME_OUT );
		return;
	}
	
	// 送信中の場合はウェイト
	if( ON == s_ds.vuart.input.send_status ){
		// タイムアウト
		s_unit.get_mode_timeout++;
		return;
	} else {
		s_unit.get_mode_timeout = 0;
	}
	
	if( 0 == s_unit.get_mode_seq ){
		user_main_mode_get_before();
	}else if( 1 == s_unit.get_mode_seq ){
		// 日時読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
		eep_read( rd_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE );
		// いびき検知数読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
		eep_read( rd_adrs, (UB*)&s_unit.ibiki_chg_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE );
		// 無呼吸検知数読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
		eep_read( rd_adrs, (UB*)&s_unit.mukokyu_chg_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE );
		// いびき時間読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE );
		// 無呼吸時間読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE );
		// 最高無呼吸時間読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE );
		// デバイス設定情報読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DEVICE_SET_INFO;
		eep_read( rd_adrs, &s_unit.device_set_info, EEP_DEVICE_SET_INFO_SIZE );
		// 抑制開始設定時間読み出し
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DEVICE_SET_SUPPRESS_START_TIME;
		eep_read( rd_adrs, &s_unit.suppress_start_time, EEP_DEVICE_SET_SUPPRESS_START_TIME_SIZE );
		
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
		tx[8] =  ( s_unit.ibiki_chg_detect_cnt & 0x00ff );
		tx[9] = (( s_unit.ibiki_chg_detect_cnt & 0xff00 ) >> 8 );
		tx[10] =  ( s_unit.mukokyu_chg_detect_cnt & 0x00ff );
		tx[11] = (( s_unit.mukokyu_chg_detect_cnt & 0xff00 ) >> 8 );
		tx[12] =  ( s_unit.ibiki_time & 0x00ff );
		tx[13] = (( s_unit.ibiki_time & 0xff00 ) >> 8 );
		tx[14] =  ( s_unit.mukokyu_time & 0x00ff );
		tx[15] = (( s_unit.mukokyu_time & 0xff00 ) >> 8 );
		tx[16] =  ( s_unit.max_mukokyu_sec & 0x00ff );
		tx[17] = (( s_unit.max_mukokyu_sec & 0xff00 ) >> 8 );
		tx[18] = s_unit.device_set_info;
		tx[19] = s_unit.suppress_start_time;
		
		main_vuart_send( &tx[0], VUART_SND_LEN_DATA_FRAME );
		s_unit.get_mode_seq = 3;
	}else if( 3 == s_unit.get_mode_seq ){
		if( s_unit.calc_cnt <= s_unit.get_mode_calc_cnt ){
			
			s_unit.get_mode_seq = 4;
		}else{
			// EEP読み出し
			// フレーム位置とデータ位置からEEPアドレスを算出
			rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + ( s_unit.get_mode_calc_cnt * EEP_CALC_DATA_SIZE );
			
			eep_read( rd_adrs, (UB*)&calc_eep, EEP_CALC_DATA_SIZE );
			
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
			main_vuart_send( &tx[0], VUART_SND_LEN_DATA_CALC );
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
			main_vuart_send( &tx[0], VUART_SND_LEN_DATA_NEXT );
			user_main_mode_get_frame_before();
		}
	}else if( 5 == s_unit.get_mode_seq ){
		tx[0] = VUART_CMD_DATA_END;				// END
		main_vuart_send( &tx[0], VUART_SND_LEN_DATA_END );
		
		// タイムアウトタイマー初期化
		s_unit.data_end_timeout = 0;
		
		#if FUNC_DEBUG_FIN_NON == OFF
			s_unit.get_mode_seq = 6;
		#else
			// 完了通知不要(ユニアース版)
			s_unit.get_mode_seq = 7;
		#endif
	}else if( 6 == s_unit.get_mode_seq ){
		s_unit.data_end_timeout++;
		if(s_unit.data_end_timeout >= DATA_END_TIME_OUT)
		{
			// タイムアウト
			evt_act( EVENT_TIME_OUT );
		}
	}else{
		user_main_mode_get_after();
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
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	UB read_eep[EEP_ACCESS_ONCE_SIZE];
	UB diag_acl_data[10];
	UW now_time = time_get_elapsed_time();
	UH diag_kokyu_val;
	UH diag_ibiki_val;
	UH diag_photoref_val;
	UB  diag_acl_x;
	UB  diag_acl_y;
	UB  diag_acl_z;
	
	if( 0 == s_unit.self_check.seq ){
		// LED確認
		if( diagStartFlg == false )
		{
			// LED確認開始
			tx[0] = VUART_CMD_DIAG_LED;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_LED );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
			// LED点灯
			led_green_on();
		}else{
			if(s_unit.tick_diag_10ms >= DIAG_LED_TIMER)
			{
				// LED消灯
				led_green_off();
				// LED確認終了
				tx[0] = VUART_CMD_DIAG_LED;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_LED );
				diagStartFlg = false;
				s_unit.self_check.seq = 1;
			}
		}
	}else if( 1 == s_unit.self_check.seq ){
		// バイブ確認
		if( diagStartFlg == false )
		{
			// バイブ確認開始
			tx[0] = VUART_CMD_DIAG_VIB;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_VIB );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
			// バイブON
			vib_on();
		}else{
			if(s_unit.tick_diag_10ms >= DIAG_LED_TIMER)
			{
				// バイブOFF
				vib_off();
				// バイブ確認終了
				tx[0] = VUART_CMD_DIAG_VIB;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_VIB );
				diagStartFlg = false;
				s_unit.self_check.seq = 2;
			}
		}
	}else if( 2 == s_unit.self_check.seq ){
		// マイク確認
		if( diagStartFlg == false )
		{
			// マイク確認開始
			tx[0] = VUART_CMD_DIAG_MIC;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_MIC );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
		}else{
			if(s_unit.tick_diag_10ms <= DIAG_MIC_TIMER)
			{
				// 呼吸音取得・送信
				adc_ibiki_kokyu( &diag_ibiki_val, &diag_kokyu_val );
				tx[0] = VUART_CMD_DIAG_MIC_VAL;
				tx[1] = ( diag_kokyu_val & 0x00ff );
				tx[2] = (( diag_kokyu_val & 0xff00 ) >> 8 );
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_MIC_VAL );
			}else{
				// マイク確認終了
				tx[0] = VUART_CMD_DIAG_MIC;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_MIC );
				diagStartFlg = false;
				s_unit.self_check.seq = 3;
			}
		}
	}else if( 3 == s_unit.self_check.seq ){
		// 加速度センサー確認
		if( diagStartFlg == false )
		{
			// 加速度センサー確認開始
			tx[0] = VUART_CMD_DIAG_ACL;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_ACL );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
		}else{
			if(s_unit.tick_diag_10ms <= DIAG_ACL_TIMER)
			{
				// INT_SOURCE1		
				i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_INT_SRC1, &diag_acl_data[0], 1 );
				
				// 加速度データ取得
				i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_DATA_XYZ, &diag_acl_data[0], 6 );
				diag_acl_x = diag_acl_data[1];
				diag_acl_y = diag_acl_data[3];
				diag_acl_z = diag_acl_data[5];
				
				// INT_REL読み出し　※割り込み要求クリア
				i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_INT_REL, &diag_acl_data[0], 1 );
				
				// 加速度データ送信
				tx[0] = VUART_CMD_DIAG_ACL_VAL;
				// X
				tx[1] = ( diag_acl_x & 0x00ff );
				tx[2] = (( diag_acl_x & 0xff00 ) >> 8 );
				// Y
				tx[3] = ( diag_acl_y & 0x00ff );
				tx[4] = (( diag_acl_y & 0xff00 ) >> 8 );
				// Z
				tx[5] = ( diag_acl_z & 0x00ff );
				tx[6] = (( diag_acl_z & 0xff00 ) >> 8 );
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_ACL_VAL );
			}else{
				// 加速度センサー確認終了
				tx[0] = VUART_CMD_DIAG_ACL;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_ACL );
				diagStartFlg = false;
				s_unit.self_check.seq = 4;
			}
		}
	}else if( 4 == s_unit.self_check.seq ){
		// 装着センサー確認
		if( diagStartFlg == false )
		{
			// 装着センサー確認開始
			tx[0] = VUART_CMD_DIAG_PHOTO;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_PHOTO );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
		}else{
			if(s_unit.tick_diag_10ms <= DIAG_PHOTO_TIMER)
			{
				// 装着センサー値取得・送信
				diag_photoref_val = main_photo_read();
				tx[0] = VUART_CMD_DIAG_PHOTO_VAL;
				tx[1] = ( diag_photoref_val & 0x00ff );
				tx[2] = (( diag_photoref_val & 0xff00 ) >> 8 );
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_PHOTO_VAL );
			}else{
				// 装着センサー確認終了
				tx[0] = VUART_CMD_DIAG_PHOTO;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_PHOTO );
				diagStartFlg = false;
				s_unit.self_check.seq = 5;
			}
		}
	}else if( 5 == s_unit.self_check.seq ){
		// EEPROM確認(書き込み)
		if( diagStartFlg == false )
		{
			tx[0] = VUART_CMD_DIAG_EEPROM;
			tx[1] = VUART_DIAG_START;
			tx[2] = VUART_DATA_RESULT_OK;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_EEPROM );
			diagStartFlg = true;
		}
		// 全0書き込み
		// EEPプログラムモード
		eep_write( s_unit.self_check.eep_cnt * EEP_ACCESS_ONCE_SIZE, (UB*)&s_eep_page0_tbl, EEP_ACCESS_ONCE_SIZE, ON );
		INC_MAX( s_unit.self_check.eep_cnt, EEP_PAGE_CNT_MAX );
		if( s_unit.self_check.eep_cnt >= EEP_PAGE_CNT_MAX ){
			s_unit.self_check.eep_cnt = 0;
			s_unit.self_check.seq = 6;
		}
	}else if( 6 == s_unit.self_check.seq ){
		// EEPROM確認(読み出し)
		// フレーム位置とデータ位置からEEPアドレスを算出
		eep_read( s_unit.self_check.eep_cnt * EEP_ACCESS_ONCE_SIZE, &read_eep[0], EEP_ACCESS_ONCE_SIZE );
		if( 0 != memcmp( &s_eep_page0_tbl[0], &read_eep[0], EEP_ACCESS_ONCE_SIZE)){
			s_unit.self_check.seq = 7;
			s_unit.self_check.last_time = now_time;
		}
		INC_MAX( s_unit.self_check.eep_cnt, EEP_PAGE_CNT_MAX );
		if( s_unit.self_check.eep_cnt >= EEP_PAGE_CNT_MAX ){
			s_unit.self_check.eep_cnt = 0;
			s_unit.self_check.seq = 8;
			s_unit.self_check.last_time = now_time;
		}
	}else if( 7 == s_unit.self_check.seq ){
		// 異常
		tx[0] = VUART_CMD_DIAG_EEPROM;
		tx[1] = VUART_DIAG_END;
		tx[2] = VUART_DATA_RESULT_NG;
		main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_EEPROM );
		diagStartFlg = false;
		s_unit.self_check.seq = 9;
	}else if( 8 == s_unit.self_check.seq ){
		// 正常
		tx[0] = VUART_CMD_DIAG_EEPROM;
		tx[1] = VUART_DIAG_END;
		tx[2] = VUART_DATA_RESULT_OK;
		main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_EEPROM );
		diagStartFlg = false;
		s_unit.self_check.seq = 9;
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
/* ①デバッグ機能です。エラー出力出来ない可能性があります。				*/
/* ②本ログが原因で不具合が発生する事があります。理由としては製品機能の	*/
/*   CPU間通信に割り込む為。											*/
/************************************************************************/
void err_info( ERR_ID id )
{
#if 0	// CPU間通信の削除のため一旦削除する(使用する場合は別のIFを用意する必要あり)
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
#endif	// CPU間通信の削除のため一旦削除する(使用する場合は別のIFを用意する必要あり)

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
#if 0
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
#endif

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
	UB bat = drv_i_port_bat_chg_detect();
	
	//電池残量確認
	main_set_battery();
	
	if( s_unit.battery_sts == BAT_LEVEL_STS_MIN )
	{
		// 電池残量なしなら移行しない
		system_mode = SYSTEM_MODE_IDLE_COM;
		set_led( LED_PATT_GREEN_BLINK_LOW_BATT );	
	}
	else if( bat == ON ){
		// 充電中ならセンシングに移行しない
		system_mode = SYSTEM_MODE_IDLE_COM;
	} else {
		calc_data_init();
	}
	
	return system_mode;
}

/************************************************************************/
/* 関数     : evt_bat_check												*/
/* 関数名   : イベント(電池残量確認)									*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2019.08.08  oneA 葛原 弘安 初版作成						*/
/************************************************************************/
/* 機能 : イベント(電池残量確認)										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_bat_check( int evt)
{
	if(get_ble_isconnect() != true)
	{
		main_set_battery();
		// LED制御
		if( s_unit.battery_sts == BAT_LEVEL_STS_HIGH || s_unit.battery_sts == BAT_LEVEL_STS_MAX )
		{
			set_led( LED_PATT_GREEN_LIGHTING );
		} else if( s_unit.battery_sts == BAT_LEVEL_STS_LOW ) {
			set_led( LED_PATT_GREEN_BLINK );
		}
#if FUNC_DEBUG_LOG != ON
		bat_check_flg = true;
#endif
	}
	return SYSTEM_MODE_NON;
}

#if 0
STATIC SYSTEM_MODE evt_send_clear( int evt)
{
	s_ds.vuart.input.send_status = OFF;
	s_unit.system_mode = SYSTEM_MODE_IDLE_COM;
	set_led( LED_PATT_YELLOW_BLINK );
	return s_unit.system_mode;
}
#endif

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
/* 関数     : evt_self_check											*/
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
/* 関数     : evt_remove												*/
/* 関数名   : イベント(未装着検知)										*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.09.24  OneA 葛原 初版作成							*/
/************************************************************************/
/* 機能 : イベント(未装着検知)											*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_remove( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_COM;
	
	s_unit.calc_cnt						= s_unit_save.calc_cnt;
	s_unit.ibiki_detect_cnt				= s_unit_save.ibiki_detect_cnt;
	s_unit.mukokyu_detect_cnt			= s_unit_save.mukokyu_detect_cnt;
	s_unit.ibiki_chg_detect_cnt			= s_unit_save.ibiki_chg_detect_cnt;
	s_unit.mukokyu_chg_detect_cnt		= s_unit_save.mukokyu_chg_detect_cnt;
	s_unit.cont_mukokyu_detect_cnt_max	= s_unit_save.cont_mukokyu_detect_cnt_max;
	s_unit.ibiki_detect_cnt_decided		= s_unit_save.ibiki_detect_cnt_decided;
	s_unit.mukokyu_detect_cnt_decided	= s_unit_save.mukokyu_detect_cnt_decided;
	
	return system_mode;
}

/************************************************************************/
/* 関数     : evt_time_out												*/
/* 関数名   : イベント(タイムアウト)									*/
/* 引数     : evt	イベント番号										*/
/* 戻り値   : システムモード											*/
/* 変更履歴 : 2018.10.08  OneA 葛原 初版作成							*/
/************************************************************************/
/* 機能 : イベント(タイムアウト)										*/
/************************************************************************/
/* 注意事項 :なし														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_time_out( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_COM;
	s_ds.vuart.input.send_status = OFF;
	return system_mode;
}

/************************************************************************/
/* 関数     : main_mode_chg												*/
/* 関数名   : モード変更												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.05.19  Axia Soft Design 西島 初版作成				*/
/************************************************************************/
/* 機能 : モード変更													*/
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
	}
	
	if( SYSTEM_MODE_IDLE_COM == s_unit.system_mode ){
		if( ON == s_unit.sensing_flg ){
			s_unit.sensing_flg = OFF;
			user_main_mode_sensing_after();
			set_vib(VIB_MODE_STANDBY);
		}
	}
	
	if( SYSTEM_MODE_PRG_G1D == s_unit.system_mode ){
		// 応答を返してからアップデートを開始するための待ちに入る
		s_unit.prg_g1d_update_wait_flg = ON;
		s_unit.prg_g1d_update_wait_sec = 2;	// 2秒後
//		FW_Update_Receiver_Start();
	}
	
	if( SYSTEM_MODE_SELF_CHECK == s_unit.system_mode ){
		s_ds.vuart.input.send_status = OFF;
	}

	if( SYSTEM_MODE_GET == s_unit.system_mode ){
		s_ds.vuart.input.send_status = OFF;
		s_unit.get_mode_seq = 0;
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
	
	// 以前はCPU間通信のモード変更を待ってモード遷移を行っていたが、
	// CPU間通信はなくなったので、直接遷移処理を行う
	main_mode_chg();
}

#if FUNC_DEBUG_LOG != ON
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
#endif

/************************************************************************/
/* 関数     : main_vuart_send											*/
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
/* 関数     : main_vuart_set_mode										*/
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
	tx[0] = VUART_CMD_SET_CHG;
	tx[1] = 0x00;
	
	main_vuart_send( &tx[0], VUART_SND_LEN_SET_CHG );

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
	
	if( 3 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_GET_DATA );
		if( TRUE == ret){
			// データ取得タイムアウト初期化
			s_unit.get_mode_timeout = 0;
		} 
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
		main_vuart_send( &tx[0], VUART_SND_LEN_MODE_CHG );
	}else{
		// OK応答
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = VUART_DATA_RESULT_OK;
		main_vuart_send( &tx[0], VUART_SND_LEN_MODE_CHG );
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
	
	if( s_unit.system_mode != SYSTEM_MODE_IDLE_COM ){
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

	// OK応答
	tx[0] = VUART_CMD_VERSION;
	// G1Dバージョン
	tx[1] = VUART_DATA_RESULT_OK;
	tx[2] = version_product_tbl[0];
	tx[3] = version_product_tbl[1];
	tx[4] = version_product_tbl[2];
	tx[5] = version_product_tbl[3];
	main_vuart_send( &tx[0], VUART_SND_LEN_VERSION );
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
	
	if( s_unit.system_mode != SYSTEM_MODE_IDLE_COM ){
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
	
	s_unit.ibiki_chg_detect_cnt  = s_ds.vuart.input.rcv_data[9] << 8;
	s_unit.ibiki_chg_detect_cnt  |= s_ds.vuart.input.rcv_data[8];
	s_unit.mukokyu_chg_detect_cnt  = s_ds.vuart.input.rcv_data[11] << 8;
	s_unit.mukokyu_chg_detect_cnt  |= s_ds.vuart.input.rcv_data[10];
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
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_chg_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE, ON );
	// 無呼吸検知数書き込み
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_chg_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE, ON );
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
	
//	if( s_unit.system_mode != SYSTEM_MODE_IDLE_COM ){
		tx[0] = VUART_CMD_DATE_SET;
		tx[1] = VUART_DATA_RESULT_OK;
		main_vuart_send( &tx[0], VUART_SND_LEN_DATE_SET );
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
	
	if( s_unit.system_mode != SYSTEM_MODE_IDLE_COM ){
		result = VUART_DATA_RESULT_NG;
	}else{
		s_unit.alarm.info.dat.act_mode = s_ds.vuart.input.rcv_data[1];
		s_unit.alarm.info.dat.ibiki_sens = s_ds.vuart.input.rcv_data[2];
		s_unit.alarm.info.dat.suppress_power = s_ds.vuart.input.rcv_data[3];
		s_unit.alarm.info.dat.suppress_max_time = s_ds.vuart.input.rcv_data[4];
		s_unit.alarm.info.dat.suppress_start_time = s_ds.vuart.input.rcv_data[5];
		
		eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE, ON );
	}
	
	tx[0] = VUART_CMD_DEVICE_SET;
	tx[1] = result;
	main_vuart_send( &tx[0], VUART_SND_LEN_DEVICE_SET );
	
	if(result == VUART_DATA_RESULT_OK)
	{
#if FUNC_DEBUG_LOG != ON		
		// 動作モード設定
		act_mode = s_unit.alarm.info.dat.act_mode;
		// いびき感度設定
		set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
		// 抑制強度設定
		vib_power = s_unit.alarm.info.dat.suppress_power;
		// 抑制動作最大継続時間
		set_suppress_cnt_time(s_unit.alarm.info.dat.suppress_max_time);
		// 抑制開始設定時間
		suppress_start_time = s_unit.alarm.info.dat.suppress_start_time;
#endif
	}
}

/************************************************************************/
/* 関数     : main_vuart_rcv_vib_confirm								*/
/* 関数名   : VUART受信(バイブ動作確認)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.11.18  oneA 葛原 弘安				初版作成		*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(バイブ動作確認)											*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_rcv_vib_confirm( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	UB result = VUART_DATA_RESULT_OK;
#if FUNC_DEBUG_LOG != ON
	UB vib_power_conf;
#endif
	
	if( s_unit.system_mode != SYSTEM_MODE_IDLE_COM ){
		result = VUART_DATA_RESULT_NG;
	}else{
#if FUNC_DEBUG_LOG != ON
		vib_power_conf = s_ds.vuart.input.rcv_data[1];
#endif
	}
	
	tx[0] = VUART_CMD_VIB_CONFIRM;
	tx[1] = result;
	main_vuart_send( &tx[0], VUART_SND_LEN_VIB_CONFIRM );
	
	if(result == VUART_DATA_RESULT_OK)
	{
#if FUNC_DEBUG_LOG != ON
		// バイブレベル初期化
		vib_level = VIB_LEVEL_1;
		set_vib_level(vib_level);
		
		set_vib_confirm(set_vib_mode(vib_power_conf));
#endif
	}
}

/************************************************************************/
/* 関数     : main_vuart_rcv_vib_stop									*/
/* 関数名   : VUART受信(バイブ動作停止)									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.03  oneA 葛原 弘安				初版作成		*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(バイブ動作停止)											*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_rcv_vib_stop( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	UB result = VUART_DATA_RESULT_OK;
	
	if( s_unit.system_mode != SYSTEM_MODE_IDLE_COM ){
		result = VUART_DATA_RESULT_NG;
	}
	
	tx[0] = VUART_CMD_VIB_STOP;
	tx[1] = result;
	main_vuart_send( &tx[0], VUART_SND_LEN_VIB_STOP );
	
	if(result == VUART_DATA_RESULT_OK)
	{
#if FUNC_DEBUG_LOG != ON
		vib_stop();
#endif
	}
}

/************************************************************************/
/* 関数     : main_vuart_rcv_power_off									*/
/* 関数名   : VUART受信(電源OFF)										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2020.2.20  oneA 葛原 弘安				初版作成			*/
/************************************************************************/
/* 機能 :																*/
/* VUART受信(電源OFF)													*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
void main_vuart_rcv_power_off( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	UB result = VUART_DATA_RESULT_OK;
	
	if( s_unit.system_mode != SYSTEM_MODE_SELF_CHECK ){
		result = VUART_DATA_RESULT_NG;
	}
	
	tx[0] = VUART_CMD_DIAG_POWER_OFF;
	tx[1] = result;
	main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_POWER_OFF );
	
	if(result == VUART_DATA_RESULT_OK)
	{
#if FUNC_DEBUG_LOG != ON
		// 電源OFF
		
#endif
	}
}

/************************************************************************/
/* 関数     : ds_set_vuart_data											*/
/* 関数名   : VUART通信データセット										*/
/* 引数     : VUART通信データ格納ポインタ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART通信ミドルデータセット取得										*/
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
/* 関数名   : VUART通信データセット										*/
/* 引数     : VUART通信データ格納ポインタ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.04.16  Axia Soft Design 西島	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/* VUART通信ミドルデータセット取得										*/
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
/************************************************************************/
/* 関数     : main_calc_kokyu											*/
/* 関数名   : 無呼吸演算処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 															*/
/************************************************************************/
/* 機能 :																*/
/* 																		*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
#if FUNC_DEBUG_LOG != ON
static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	UB newstate;
	UB	set_ibiki_mask = 0x01;
	UB	set_kokyu_mask = 0x02;
	UB	bit_shift = 0;
	
	calculator_apnea(&s_unit.kokyu_val[0], &s_unit.ibiki_val[0]);
	s_unit.kokyu_cnt = 0;
	newstate = get_state();
	
	bit_shift = s_unit.phase_kokyu * 2;
	if(newstate == APNEA_ERROR){
		s_unit.calc.info.dat.state |= (set_kokyu_mask << bit_shift);		// 無呼吸状態ON
		if(act_mode == ACT_MODE_SUPPRESS_SNORE_APNEA || act_mode == ACT_MODE_SUPPRESS_APNEA)
		{//抑制モード（いびき + 無呼吸）か抑制モード（無呼吸）ならバイブレーション動作
			if(s_unit.suppress_start_cnt >= (suppress_start_time * 6))
			{// 抑制開始時間経過（センシング開始から20分）
				if(vib_power == VIB_SET_MODE_GRADUALLY_STRONGER)
				{
					set_vib_level(vib_level);
					vib_level++;
				}
				set_vib(set_vib_mode(vib_power));
			}
		}
	}else{
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// 無呼吸状態OFF
		if(act_mode == ACT_MODE_SUPPRESS_APNEA)
		{// 抑制モード（無呼吸）の場合
			vib_level = VIB_LEVEL_1;
		}
	}
	// もし、いびきも無呼吸もどちらもセットされたらいびきを優先するため、いびき状態とする
	if( ((s_unit.calc.info.dat.state >> bit_shift) & 0x03) == 0x03 ){
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// 無呼吸状態OFF
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// いびき状態ON
	}
	
	// 無呼吸検知数更新
	if( ((s_unit.calc.info.dat.state >> bit_shift) & set_kokyu_mask) == set_kokyu_mask ){
		s_unit.mukokyu_detect_cnt++;
		s_unit.cont_mukokyu_detect_cnt_current++;
		
		// 無呼吸発生検知回数
		if(s_unit.mukokyu_state_flg == OFF){
			s_unit.mukokyu_state_flg = ON;
			s_unit.mukokyu_chg_detect_cnt++;
		}
	}else{
		// 継続無呼吸検知の終了処理(最大値を更新)
		if(s_unit.cont_mukokyu_detect_cnt_current > s_unit.cont_mukokyu_detect_cnt_max){
			// 5分未満は保存する（５分以上はアプリ側で[不明]状態とするため）(5分 = 30回)
			if(s_unit.cont_mukokyu_detect_cnt_current < MUKOKYU_DETECT_CNT_MAX)
			{
				s_unit.cont_mukokyu_detect_cnt_max = s_unit.cont_mukokyu_detect_cnt_current;
			}
		}
		s_unit.cont_mukokyu_detect_cnt_current = 0;
		
		s_unit.mukokyu_state_flg = OFF;
	}
	
	s_unit.phase_kokyu++;
	if(s_unit.phase_kokyu >= SEC_PHASE_NUM){
		s_unit.phase_kokyu = SEC_PHASE_0_10;
		apnea_data_max = true;
	}
#endif
	
	return (KE_MSG_CONSUMED);
}
#else
//デバッグ版演算処理
static UB main_calc_kokyu( void)
{
	calculator_apnea(&s_unit.kokyu_val[0], &s_unit.ibiki_val[0]);
	return get_state();
}
#endif

/************************************************************************/
/* 関数     : main_calc_ibiki											*/
/* 関数名   : いびき演算処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 															*/
/************************************************************************/
/* 機能 :																*/
/* 																		*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
#if FUNC_DEBUG_LOG != ON
static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	//演算正規処理
	int ii;
	int average =0;
	UB newstate;
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
	
	// 10秒間の平均値
	for(ii=0;ii<s_unit.ibiki_cnt;++ii){
		average += s_unit.ibiki_val[ii];
	}
	average = average / s_unit.ibiki_cnt;
	
	// AD値10bitデータを2bitシフトして上位8bitのみを保存する(下位2bitは誤差範囲で問題なし)
	s_unit.calc.info.dat.ibiki_val[s_unit.phase_ibiki] = (UB)(( average >> 2 ) & 0xff );

	// いびき演算
	calc_snore_proc(&s_unit.ibiki_val[0]);
	newstate = calc_snore_get();
	
	if(suppress_max_cnt_over_flg == ON)
	{// 抑制動作最大時間オーバー時
		s_unit.suppress_max_time_interval_cnt++;
		if( SUPPRESS_INTERVAL_CNT <= s_unit.suppress_max_time_interval_cnt )
		{// 抑制動作最大時間オーバー時のインターバル満了
			suppress_max_cnt_over_flg = OFF;
			s_unit.suppress_max_time_interval_cnt = 0;
			s_unit.suppress_cont_time_cnt = 0;
		}
	}
	
	bit_shift = s_unit.phase_ibiki * 2;
	if(newstate == SNORE_ON){
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// いびき状態ON
		s_unit.suppress_cont_time_cnt++;
		if(act_mode == ACT_MODE_SUPPRESS_SNORE_APNEA || act_mode == ACT_MODE_SUPPRESS_SNORE)
		{//抑制モード（いびき + 無呼吸）か抑制モード（いびき）ならバイブレーション動作
			if(s_unit.suppress_cont_time_cnt <= suppress_max_cnt)
			{//抑制動作最大時間以下
				if(suppress_max_cnt_over_flg == OFF)
				{//抑制動作最大時間オーバー時以外
					if(s_unit.suppress_start_cnt >= (suppress_start_time * 6))
					{// 抑制開始時間経過（センシング開始から20分）
						if(vib_power == VIB_SET_MODE_GRADUALLY_STRONGER)
						{
							set_vib_level(vib_level);
							vib_level++;
						}
						vib_startflg = true;
//						set_vib(set_vib_mode(vib_power));
					}
				}
			} else {
				//抑制動作最大時間オーバー時にフラグON
				suppress_max_cnt_over_flg = ON;
			}
		}
	}else{
		s_unit.calc.info.dat.state &= ~(set_ibiki_mask << bit_shift);		// いびき状態OFF
		s_unit.suppress_cont_time_cnt = 0;	// 初期化
		if(vib_power == VIB_SET_MODE_GRADUALLY_STRONGER)
		{
			if(act_mode == ACT_MODE_SUPPRESS_SNORE_APNEA)
			{// 抑制モード（いびき + 無呼吸）の場合
				if( ((s_unit.calc.info.dat.state >> bit_shift) & set_kokyu_mask) == 0x00 )
				{// 無呼吸判定チェック
					// 無呼吸判定もされていない場合バイブレベルを初期化
					vib_level = VIB_LEVEL_1;
				}
			} else if(act_mode == ACT_MODE_SUPPRESS_SNORE)
			{// 抑制モード（いびき）の場合
				vib_level = VIB_LEVEL_1;
			}
		}
	}
	// もし、いびきも無呼吸もどちらもセットされたらいびきを優先するため、いびき状態とする
	if( ((s_unit.calc.info.dat.state >> bit_shift) & 0x03) == 0x03 ){
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// 無呼吸状態OFF
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// いびき状態ON
	}
	
	// いびき検知数更新
	if( ((s_unit.calc.info.dat.state >> bit_shift) & set_ibiki_mask) == set_ibiki_mask ){
		s_unit.ibiki_detect_cnt++;
		
		// いびき発生検知回数
		if(s_unit.ibiki_state_flg == OFF){
			s_unit.ibiki_state_flg = ON;
			s_unit.ibiki_chg_detect_cnt++;
		}
	}else{
		s_unit.ibiki_state_flg = OFF;
	}
	
	s_unit.phase_ibiki++;
	if(s_unit.phase_ibiki >= SEC_PHASE_NUM){
		s_unit.phase_ibiki = SEC_PHASE_0_10;
		snore_data_max = true;
	}	
	
#if 0	
	// 移動累計とるので前のデータを残す
	for(ii=0;ii<size;++ii){
		s_unit.ibiki_val[ii] = s_unit.ibiki_val[DATA_SIZE-size+ii];
	}
	s_unit.ibiki_cnt = size;
#else
	s_unit.ibiki_cnt = 0;
#endif
	
#else
	//デバッグ用ダミー処理
	NO_OPERATION_BREAK_POINT();									// ブレイクポイント設置用

	// ダミーデータ
	s_unit.ibiki_cnt = 0;
	s_unit.calc.info.dat.spo2_val = (UB)s_unit.ibiki_val[0];
#endif

	return (KE_MSG_CONSUMED);
}
#else
static UB main_calc_ibiki( void)
{
	// いびき演算
	calc_snore_proc(&s_unit.ibiki_val[0]);
	return calc_snore_get();
}
#endif

/************************************************************************/
/* 関数     : main_calc_acl												*/
/* 関数名   : 加速度演算処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 															*/
/************************************************************************/
/* 機能 :																*/
/* 																		*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	UB	body_direct = BODY_DIRECTION_LEFT;
	UB	clear_mask = BODY_DIRECTION_MASK;
	UB	bit_shift = 0;

	// 体の向き判定
	if( 0 <= s_unit.meas.info.dat.acl_x )
	{// 上 or 右
		if( 0 <= s_unit.meas.info.dat.acl_z )
		{// 上
			body_direct = BODY_DIRECTION_UP;
		} else {
		 // 右
			body_direct = BODY_DIRECTION_RIGHT;
		}
	} else {
	// 下 or 左
		if( 0 <= s_unit.meas.info.dat.acl_z )
		{// 左
			body_direct = BODY_DIRECTION_LEFT;
		} else {
		 // 下
			body_direct = BODY_DIRECTION_DOWN;
		}
	}
	
	// 10秒ごとの判定値をbitで設定する
	bit_shift = s_unit.phase_body_direct * BODY_DIRECTION_BIT;
	s_unit.calc.info.dat.body_direct &= ~(clear_mask << bit_shift);
	s_unit.calc.info.dat.body_direct |= (body_direct << bit_shift);
	s_unit.phase_body_direct++;
	if(s_unit.phase_body_direct >= SEC_PHASE_NUM){
		s_unit.phase_body_direct = SEC_PHASE_0_10;
#if FUNC_DEBUG_LOG != ON
		acl_data_max = true;
#endif
	}

	return (KE_MSG_CONSUMED);
}

/************************************************************************/
/* 関数     : main_calc_photoref										*/
/* 関数名   : フォトセンサ保存処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 															*/
/************************************************************************/
/* 機能 :																*/
/* 																		*/
/************************************************************************/
/* 注意事項 :															*/
/************************************************************************/
static int_t main_calc_photoref(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	// AD値10bitデータを2bitシフトして上位8bitのみを保存する(下位2bitは誤差範囲で問題なし)
	s_unit.calc.info.dat.photoref[s_unit.phase_photoref] = (UB)(( s_unit.meas.info.dat.photoref_val >> 2 ) & 0xff );

	s_unit.phase_photoref++;
	if(s_unit.phase_photoref >= SEC_PHASE_NUM){
		s_unit.phase_photoref = SEC_PHASE_0_10;
#if FUNC_DEBUG_LOG != ON
		photo_data_max = true;
#endif
	}
	
	// 装着判定
	if(s_unit.meas.info.dat.photoref_val <= PHOTO_SENSOR_WEARING_AD)
	{
		if(s_unit.photosens_remove_cnt == 0)
		{
			// 判定はじめのデータを保存
			s_unit_save.calc_cnt					= s_unit.calc_cnt;
			s_unit_save.ibiki_detect_cnt			= s_unit.ibiki_detect_cnt;
			s_unit_save.mukokyu_detect_cnt			= s_unit.mukokyu_detect_cnt;
			s_unit_save.ibiki_chg_detect_cnt		= s_unit.ibiki_chg_detect_cnt;
			s_unit_save.mukokyu_chg_detect_cnt		= s_unit.mukokyu_chg_detect_cnt;
			s_unit_save.cont_mukokyu_detect_cnt_max	= s_unit.cont_mukokyu_detect_cnt_max;
			s_unit_save.ibiki_detect_cnt_decided	= s_unit.ibiki_detect_cnt_decided;
			s_unit_save.mukokyu_detect_cnt_decided	= s_unit.mukokyu_detect_cnt_decided;
		}
		s_unit.photosens_remove_cnt++;
	}else{
		s_unit.photosens_remove_cnt = 0; // 途切れたらリセット
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

	// 設定反映
	// 動作モード設定
	act_mode = s_unit.alarm.info.dat.act_mode;
	// いびき感度設定
	set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
	// 抑制強度設定
	vib_power = s_unit.alarm.info.dat.suppress_power;
	// 抑制動作最大継続時間
	set_suppress_cnt_time(s_unit.alarm.info.dat.suppress_max_time);
	// 抑制開始設定時間
	suppress_start_time = s_unit.alarm.info.dat.suppress_start_time;
	
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
/* ①時間がかかる為に使用する際は注する事。約7.5Sec						*/
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
/* ①測定データ消去には時間がかかる為に消去しない						*/
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

	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_WHO_AM_I, &rd_data[0], 1 );
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
	
	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_CTRL_REG1, &rd_data[0], 1 );
	if( rd_data[0] != 0x00 ){
		err_info( ERR_ID_ACL );
	}
}

/************************************************************************/
/* 関数     : main_acl_start											*/
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

#if (FUNC_DEBUG_LOG != ON) || (FUNC_DEBUG_WAVEFORM_LOG != ON)
/************************************************************************/
/* 関数     : main_acl_read												*/
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
	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_INT_SRC1, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & BIT04 )){
		// データ未達
		err_info( ERR_ID_ACL );
		return;
	}
	
	// データ取得
	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_DATA_XYZ, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
	// INT_REL読み出し　※割り込み要求クリア
	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_INT_REL, &rd_data[0], 1 );
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
#endif

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
	s_unit.tick_led_20ms_sec = 0;
}

/************************************************************************/
/* 関数     : reset_bat_checkflg										*/
/* 関数名   : 電池チェックフラグOFF										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2020.01.28 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void reset_bat_checkflg(void)
{
#if FUNC_DEBUG_LOG != ON
	bat_check_flg = false;
#endif
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

/************************************************************************/
/* 関数     : set_suppress_cnt_time										*/
/* 関数名   : 抑制最大連続時間設定										*/
/* 引数     : suppress_max_time											*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.26 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
static void set_suppress_cnt_time(UB suppress_max_time)
{
	switch(suppress_max_time)
	{
	case SET_MAX_SUPPRESS_CONT_5_MIN:
		suppress_max_cnt = MAX_SUPPRESS_CONT_TIME_5_MIN_CNT;
		break;
	case SET_MAX_SUPPRESS_CONT_10_MIN:
		suppress_max_cnt = MAX_SUPPRESS_CONT_TIME_10_MIN_CNT;
		break;
	case SET_MAX_SUPPRESS_CONT_NON:
		suppress_max_cnt = MAX_SUPPRESS_CONT_TIME_NON_CNT;
		break;
	default:
		suppress_max_cnt = MAX_SUPPRESS_CONT_TIME_10_MIN_CNT;
		break;
	}
}

/************************************************************************/
/* 関数     : set_ble_state												*/
/* 関数名   : BLE管理状態設定											*/
/* 引数     : state														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.09.18 Axia Soft Design 和田 耕太	初版作成			*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void set_ble_state(UB state)
{
	if( (state != BLE_STATE_ON)&&(state != BLE_STATE_OFF)&&(state != BLE_STATE_INITIAL)){
		return;
	}
	s_unit.ble_state = state;
}

/************************************************************************/
/* 関数     : get_ble_state												*/
/* 関数名   : BLE管理状態取得											*/
/* 引数     : なし														*/
/* 戻り値   : BLE管理状態												*/
/* 変更履歴	: 2019.09.18 Axia Soft Design 和田 耕太	初版作成			*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
UB get_ble_state(void)
{
	return s_unit.ble_state;
}

/************************************************************************/
/* 関数     : set_ble_isconnect											*/
/* 関数名   : BLE接続状態設定											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.12.03 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void set_ble_isconnect(bool connect)
{
	s_unit.ble_isconnect = connect;
}

/************************************************************************/
/* 関数     : get_ble_isconnect											*/
/* 関数名   : BLE接続状態取得											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2019.12.03 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
bool get_ble_isconnect(void)
{
	return s_unit.ble_isconnect;
}

/************************************************************************/
/* 関数     : set_serial_command										*/
/* 関数名   : シリアルコマンド設定										*/
/* 引数     : dbg_rx_data:受信データ（コマンド）						*/
/* 戻り値   : なし														*/
/* 変更履歴	: 2020.01.06 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void set_serial_command(char dbg_rx_data)
{
	switch(dbg_rx_data)
	{
		case RCV_COM_MODE_MONITORING:			// モニタリングモード
			s_unit.alarm.info.dat.act_mode = ACT_MODE_MONITOR;
			act_mode = s_unit.alarm.info.dat.act_mode;
			break;
		case RCV_COM_MODE_SUPPRESS_SNORE:		// 抑制モード（いびき）
			s_unit.alarm.info.dat.act_mode = ACT_MODE_SUPPRESS_SNORE;
			act_mode = s_unit.alarm.info.dat.act_mode;
			break;
		case RCV_COM_MODE_SUPPRESS_APNEA:		// 抑制モード（無呼吸）
			s_unit.alarm.info.dat.act_mode = ACT_MODE_SUPPRESS_APNEA;
			act_mode = s_unit.alarm.info.dat.act_mode;
			break;
		case RCV_COM_MODE_SUPPRESS_SNORE_APNEA:	// 抑制モード（いびき＋無呼吸）
			s_unit.alarm.info.dat.act_mode = ACT_MODE_SUPPRESS_SNORE_APNEA;
			act_mode = s_unit.alarm.info.dat.act_mode;
			break;
		case RCV_COM_SNORE_SENS_WEAK: 			// いびき検出感度（弱）
			s_unit.alarm.info.dat.ibiki_sens = 0;
			set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
			break;
		case RCV_COM_SNORE_SENS_MED:			// いびき検出感度（中）
			s_unit.alarm.info.dat.ibiki_sens = 1;
			set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
			break;
		case RCV_COM_SNORE_SENS_STRONG: 		// いびき検出感度（強）
			s_unit.alarm.info.dat.ibiki_sens = 2;
			set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
			break;
		case RCV_COM_VIB_WEAK:					// バイブの強さ(弱)
			s_unit.alarm.info.dat.suppress_power = VIB_SET_MODE_WEAK;
			vib_power = s_unit.alarm.info.dat.suppress_power;
			break;
		case RCV_COM_VIB_MED:					// バイブの強さ(中)
			s_unit.alarm.info.dat.suppress_power = VIB_SET_MODE_DURING;
			vib_power = s_unit.alarm.info.dat.suppress_power;
			break;
		case RCV_COM_VIB_STRONG:				// バイブの強さ(強)
			s_unit.alarm.info.dat.suppress_power = VIB_SET_MODE_STRENGTH;
			vib_power = s_unit.alarm.info.dat.suppress_power;
			break;
		case RCV_COM_VIB_GRAD:					// バイブの強さ(徐々に強く)
			s_unit.alarm.info.dat.suppress_power = VIB_SET_MODE_GRADUALLY_STRONGER;
			vib_power = s_unit.alarm.info.dat.suppress_power;
			break;
		case RCV_COM_VIB_GRAD_LEVELUP:			// バイブの強さ(徐々に強く)のレベルアップ
			vib_level++;
			set_vib_level(vib_level);
			break;
		case RCV_COM_VIB_GRAD_LEVEL_INIT:		// バイブの強さ(徐々に強く)のレベルを初期化
			vib_level = VIB_LEVEL_1;
			set_vib_level(vib_level);
			break;
		case RCV_COM_VIB_WEAK_CONF:				// バイブの強さ(弱)[確認]
			set_vib_confirm(VIB_MODE_WEAK);
			break;
		case RCV_COM_VIB_MED_CONF:				// バイブの強さ(中)[確認]
			set_vib_confirm(VIB_MODE_DURING);
			break;
		case RCV_COM_VIB_STRONG_CONF:			// バイブの強さ(強)[確認]
			set_vib_confirm(VIB_MODE_STRENGTH);
			break;
		case RCV_COM_VIB_GRAD_CONF:				// バイブの強さ(徐々に強く)[確認]
			set_vib_confirm(VIB_MODE_GRADUALLY_STRONGER);
			break;
		case RCV_COM_SNORE_SUPPRESS_TIME_FIVE:	// いびき抑制の連続時間（5分）
			break;
		case RCV_COM_SNORE_SUPPRESS_TIME_TEN:	// いびき抑制の連続時間（10分）
			break;
		case RCV_COM_SNORE_SUPPRESS_TIME_NON:	// いびき抑制の連続時間（設定しない）
			break;
		case RCV_COM_SUPPRESS_START_TIME:		// バイブ抑制開始時間変更
			break;
		case RCV_COM_SUPPRESS_START:			// バイブ動作
			set_vib(set_vib_mode(vib_power));
			break;

		default:
			break;
	}
	// ROMに保存
	eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE, ON );
}

