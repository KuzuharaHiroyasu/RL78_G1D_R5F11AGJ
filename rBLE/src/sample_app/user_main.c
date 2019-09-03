/********************************************************************************/
/* ƒVƒXƒeƒ€–¼   : RD8001 ‰õ–°ƒ`ƒFƒbƒJ[											*/
/* ƒtƒ@ƒCƒ‹–¼   : user_main.c													*/
/* ‹@”\         : ƒ†[ƒU[ƒƒCƒ“iŠÖ”,RAM,ROM’è‹`j							*/
/* •ÏX—š—ğ		: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬				*/
/* ’ˆÓ–€     : ‚È‚µ															*/
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

#include	"header.h"				//ƒ†[ƒU[’è‹`

#include	"r_vuart_app.h"

// ƒvƒƒgƒ^ƒCƒvéŒ¾
static int_t user_main_calc_result_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t led_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t battery_level_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_photoref(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);

#if FUNC_DEBUG_LOG != ON
static void set_yokusei_cnt_time(UB yokusei_max_time);
static int_t user_main_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
STATIC void user_main_calc_data_set_kyokyu_ibiki( void );
STATIC void user_main_mode( void );
STATIC void user_main_mode_common( void );
STATIC void user_main_calc_data_set_acl( void );
STATIC void user_main_calc_data_set_photoref( void );
STATIC void main_vuart_proc(void);
#else
STATIC void make_send_data(char* pBuff);
#if FUNC_DEBUG_WAVEFORM_LOG == ON
static UB main_calc_kokyu( void);
static UB main_calc_ibiki( void);
STATIC void user_main_calc_data_set_kyokyu_ibiki( void );
#endif
#endif

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
STATIC SYSTEM_MODE evt_sensing( int evt);
STATIC SYSTEM_MODE evt_sensing_chg( int evt);
STATIC SYSTEM_MODE evt_initial( int evt);
STATIC SYSTEM_MODE evt_bat_check( int evt);
STATIC SYSTEM_MODE evt_send_clear( int evt);
STATIC SYSTEM_MODE evt_get( int evt);
STATIC SYSTEM_MODE evt_g1d_prg_denchi( int evt);
STATIC SYSTEM_MODE evt_self_check( int evt);
STATIC void user_main_mode_get_after( void );
STATIC void user_main_eep_read_pow_on(void);
STATIC void eep_part_erase( void );
STATIC void main_vuart_send( UB *p_data, UB len );
STATIC void main_vuart_rcv_data_fin( void );
//STATIC void eep_all_erase( void );		//–¢g—pŠÖ”
void main_vuart_set_mode( void );
void main_vuart_rcv_data_frame( void );
void main_vuart_rcv_data_calc( void );
void main_vuart_rcv_data_end( void );
void main_vuart_rcv_date( void );
void main_vuart_rcv_device_set( void );

// ACLAƒtƒHƒgŠÖ˜A
STATIC void main_acl_init(void);
STATIC void main_acl_stop(void);
STATIC void main_acl_start(void);
#if (FUNC_DEBUG_LOG != ON) || (FUNC_DEBUG_WAVEFORM_LOG != ON)
STATIC void main_acl_read(void);
STATIC UH main_photo_read(void);
#endif

/********************/
/*     ŠO•”QÆ     */
/********************/
//fw update ¦G1DƒAƒbƒvƒf[ƒgŠJnˆ—
extern RBLE_STATUS FW_Update_Receiver_Start( void );

/********************/
/*     •Ï”’è‹`     */
/********************/
/* Status */// ƒvƒ‰ƒbƒgƒtƒH[ƒ€
ke_state_t user_main_state[ USER_MAIN_IDX_MAX ] = {0};

STATIC T_UNIT s_unit;
STATIC DS s_ds;

static bool vib_flg = false;
#if FUNC_DEBUG_LOG != ON
static UB act_mode = ACT_MODE_NORMAL;
static UB vib_str = VIB_MODE_DURING;
static UH yokusei_max_cnt = MAX_YOKUSEI_CONT_TIME_10_MIN_CNT;
static UB yokusei_max_cnt_over_flg = OFF;
static UB acl_photo_sens_read_flg = OFF;
#else
static UB sw_on_flg = OFF;
#if FUNC_DEBUG_WAVEFORM_LOG == ON
static UB snore_state;
static UB apnea_state;
#endif
#endif

/********************/
/*     ’è”’è‹`     */
/********************/
#include	"user_main_tbl.h"		// ƒ†[ƒU[ƒe[ƒuƒ‹À‘Ì’è‹`

/************************************************************************/
/* ŠÖ”     : app_evt_usr_1												*/
/* ŠÖ”–¼   : ƒ†[ƒU[ƒCƒxƒ“ƒg(10msüŠú)								*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.05.13 Axia Soft Design mmura		‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒ†[ƒU[ƒCƒxƒ“ƒg(10msüŠú)									*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
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
/* ŠÖ”     : app_evt_usr_2												*/
/* ŠÖ”–¼   : ƒ†[ƒU[ƒCƒxƒ“ƒg(1•büŠú)									*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒ†[ƒU[ƒCƒxƒ“ƒg(1•büŠú)										*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
void codeptr app_evt_usr_2(void) 
{
#if FUNC_DEBUG_LOG != ON
	uint8_t *ke_msg;
#endif	
	ke_evt_clear(KE_EVT_USR_2_BIT);
	
#if FUNC_DEBUG_LOG != ON
	// •bƒ^ƒCƒ}[ƒJƒEƒ“ƒgƒ_ƒEƒ“
	DEC_MIN( s_unit.timer_sec ,0 );
	
#if 1
	// RD8001b’èFG1Dƒ_ƒEƒ“ƒ[ƒh_ƒfƒoƒbƒO—pƒo[ƒWƒ‡ƒ“‘—M
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
	
	// “d’rc—Êæ“¾(10•ªüŠú)
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
	if( s_unit.sec30_cnt >= CALC_RESULT_WR_CYC ){		// 30•b
		s_unit.sec30_cnt = 0;
		
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_CALC_RESULT, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	// ‰Á‘¬“x‰‰Z
	ke_msg = ke_msg_alloc( USER_MAIN_CALC_ACL, USER_MAIN_ID, USER_MAIN_ID, 0 );
	ke_msg_send(ke_msg);
	
	// ƒtƒHƒgƒZƒ“ƒT’l
	ke_msg = ke_msg_alloc( USER_MAIN_CYC_PHOTOREF, USER_MAIN_ID, USER_MAIN_ID, 0 );
	ke_msg_send(ke_msg);
	
#endif
}

/************************************************************************/
/* ŠÖ”     : app_evt_usr_2												*/
/* ŠÖ”–¼   : ƒ†[ƒU[ƒCƒxƒ“ƒg(20msüŠú)								*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒ†[ƒU[ƒCƒxƒ“ƒg(20msüŠú)									*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
void codeptr app_evt_usr_3(void) 
{ 
	ke_evt_clear(KE_EVT_USR_3_BIT); 

#if FUNC_DEBUG_LOG != ON
	{
		uint8_t *ke_msg;
		
		// ƒ†[ƒU[ƒAƒvƒŠüŠúˆ—
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
/* ŠÖ”     : user_main_cyc												*/
/* ŠÖ”–¼   : ƒ†[ƒU[ƒAƒvƒŠüŠúˆ—									*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒ†[ƒU[ƒAƒvƒŠüŠúˆ—										*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
static int_t user_main_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	sw_proc();				// SWŒŸ’mˆ—
	
	main_vuart_proc();		// VUART’ÊMƒT[ƒrƒX
	
	user_main_mode();		// ƒƒCƒ“ƒ‚[ƒhˆ—

	return (KE_MSG_CONSUMED);
}
#endif

/************************************************************************/
/* ŠÖ”     : user_main_calc_result_cyc									*/
/* ŠÖ”–¼   : ƒ†[ƒU[ƒAƒvƒŠ‰‰ZŒ‹‰ÊüŠúˆ—							*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒ†[ƒU[ƒAƒvƒŠ‰‰ZŒ‹‰ÊüŠúˆ—								*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
static int_t user_main_calc_result_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	user_main_calc_result();	// ‰‰ZŒ‹‰Ê
	
	return (KE_MSG_CONSUMED);
}



/************************************************************************/
/* ŠÖ”     : user_main_timer_10ms_set									*/
/* ŠÖ”–¼   : 10msƒ^ƒCƒ}[ƒJƒEƒ“ƒgƒZƒbƒg								*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : 10msƒ^ƒCƒ}[ƒJƒEƒ“ƒgƒZƒbƒg									*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/* ‡@ƒXƒŠ[ƒv’†‚Å‚à—LŒø‚Èƒ^ƒCƒ}[										*/
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
/* ŠÖ”     : user_main_timer_cyc										*/
/* ŠÖ”–¼   : ƒ^ƒCƒ}[üŠúˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒ^ƒCƒ}[üŠúˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
void user_main_timer_cyc( void )
{
#if FUNC_DEBUG_LOG != ON
	UB bat;
#else
	if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_50MSEC){
		sw_proc();				// SWŒŸ’mˆ—
	}
#endif
	if(s_unit.system_mode == SYSTEM_MODE_SENSING)
	{
		// 50msüŠú
		if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_50MSEC){
#if FUNC_DEBUG_LOG == ON
			char dbg_tx_data[50] = {0};
			int dbg_len;
			
			// ŒÄ‹z‰¹A‚¢‚Ñ‚«‰¹æ“¾
			adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
#if FUNC_DEBUG_WAVEFORM_LOG == ON
	// ”gŒ`&Œ‹‰ÊŠm”F
			user_main_calc_data_set_kyokyu_ibiki();
#else
	//’ÊíƒfƒoƒbƒO”Å
			s_unit.acl_timing+=1;
			if(s_unit.acl_timing >= ACL_TIMING_VAL){
				s_unit.acl_timing = 0;
				// ‰Á‘¬“xæ“¾
				main_acl_read();
				// ƒtƒHƒgƒZƒ“ƒT[’læ“¾
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
			// ŒÄ‹z‰¹A‚¢‚Ñ‚«‰¹æ“¾
			adc_ibiki_kokyu( &s_unit.meas.info.dat.ibiki_val, &s_unit.meas.info.dat.kokyu_val );
			user_main_calc_data_set_kyokyu_ibiki();
			
			s_unit.acl_timing+=1;
			
			// 5•bŒã‚Éæ“¾
			if(s_unit.acl_timing >= ACL_TIMING_VAL){
				if( acl_photo_sens_read_flg == OFF)
				{
					// ‰Á‘¬“xæ“¾
					main_acl_read();
					user_main_calc_data_set_acl();
					
					// ƒtƒHƒgƒZƒ“ƒT[’læ“¾
					s_unit.meas.info.dat.photoref_val = main_photo_read();
					user_main_calc_data_set_photoref();
					
					acl_photo_sens_read_flg = ON;
				}
				
				// ƒZƒ“ƒT[’læ“¾5•bŒã‚ÉƒŠƒZƒbƒg
				if(s_unit.acl_timing >= ACL_RESET_TIMING_VAL)
				{
					s_unit.acl_timing = 0;
					acl_photo_sens_read_flg = OFF;
				}
			}
			
			s_unit.sensing_cnt_50ms++;
			
			//[“dŒŸ’m
			bat = drv_i_port_bat_chg_detect();
			if(bat == ON)
			{
				//[“d’†‚È‚ç‘Ò‹@ƒ‚[ƒh‚Ö
				evt_act( EVENT_CHG_PORT_ON );
			}
			
			// “d’rc—ÊŒŸ’m
			if(s_unit.battery_sts == BAT_LEVEL_STS_MIN)
			{
				// “d’rc—Ê‚È‚µ‚È‚ç‘Ò‹@ƒ‚[ƒh‚Ö
				evt_act( EVENT_DENCH_LOW );
			}
			
			// ƒZƒ“ƒVƒ“ƒOŠÔãŒÀŒŸ’m
			if( s_unit.sensing_cnt_50ms >= HOUR12_CNT_50MS )
			{
				// 12ŠÔ‚ğ’´‚¦‚½‚È‚ç‘Ò‹@ƒ‚[ƒh‚Ö
				evt_act( EVENT_COMPLETE );
			}
#endif
			s_unit.tick_10ms_new = 0;
		}
	}
	// 20msüŠú
	if(s_unit.tick_10ms >= (uint16_t)PERIOD_20MSEC){
		ke_evt_set(KE_EVT_USR_3_BIT);

		s_unit.tick_10ms = 0;
	}
	// 1•büŠú ¦’x‚ê‚Ì’~Ï‚ÍŒµ‹Ö
	if( s_unit.tick_10ms_sec >= (uint16_t)PERIOD_1SEC){
		s_unit.tick_10ms_sec -= PERIOD_1SEC;	// ’x‚ê‚ª’~Ï‚µ‚È‚¢—l‚Éˆ—
		ke_evt_set(KE_EVT_USR_2_BIT);
	}
}

/************************************************************************/
/* ŠÖ”     : led_cyc													*/
/* ŠÖ”–¼   : 						*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.08.02 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 							*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
static int_t led_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	led_start(s_unit.tick_led_20ms_sec);
	s_unit.tick_led_20ms_sec++;
	
	return (KE_MSG_CONSUMED);
}

/************************************************************************/
/* ŠÖ”     : vib_cyc													*/
/* ŠÖ”–¼   : 						*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.08.02 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 							*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
void vib_cyc( void )
{
	if(vib_flg == true)
	{
		// ƒoƒCƒuƒŒ[ƒVƒ‡ƒ“(10msüŠú)
		if(s_unit.tick_vib_10ms_sec >= (uint16_t)PERIOD_10MSEC)
		{
	   		vib_start(s_unit.tick_vib_10ms_sec);
		}
	}
}

/************************************************************************/
/* ŠÖ”     : set_vib_flg												*/
/* ŠÖ”–¼   : 						*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.08.02 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 							*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
void set_vib_flg( bool flg )
{
	vib_flg = flg;
}

/************************************************************************/
/* ŠÖ”     : battery_level_cyc											*/
/* ŠÖ”–¼   : 						*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.08.02 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 							*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
static int_t battery_level_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	// “d’rc—Êæ“¾(10•ªüŠú)
	main_set_battery();
	
	return (KE_MSG_CONSUMED);
}


/************************************************************************/
/* ŠÖ”     : user_main_calc_data_set_kokyu_ibiki						*/
/* ŠÖ”–¼   : ‰‰Zƒf[ƒ^ƒZƒbƒgˆ—(ŒÄ‹zE‚¢‚Ñ‚«)						*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.07.19 Axia Soft Design ˜a“c k‘¾	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ‰‰Zƒf[ƒ^ƒZƒbƒgˆ—(ŒÄ‹zE‚¢‚Ñ‚«)							*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
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
	
	// ƒf[ƒ^ƒtƒ‹‚Å‰‰ZŒÄo
	if( s_unit.kokyu_cnt >= ( DATA_SIZE_APNEA - 1 )){
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_KOKYU, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
		set_led( LED_PATT_GREEN_BLINK_SENSING );
	}

	if( s_unit.ibiki_cnt >= ( DATA_SIZE_APNEA - 1 )){
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_IBIKI, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	INC_MAX( s_unit.kokyu_cnt, MEAS_KOKYU_CNT_MAX );		
	INC_MAX( s_unit.ibiki_cnt, MEAS_IBIKI_CNT_MAX );		

	NO_OPERATION_BREAK_POINT();									// ƒuƒŒƒCƒNƒ|ƒCƒ“ƒgİ’u—p
}
#else
#if FUNC_DEBUG_WAVEFORM_LOG == ON
STATIC void user_main_calc_data_set_kyokyu_ibiki( void )
{
	if( s_unit.kokyu_cnt < MEAS_KOKYU_CNT_MAX ){
		s_unit.kokyu_val[s_unit.kokyu_cnt] = s_unit.meas.info.dat.kokyu_val;
	}
	if( s_unit.ibiki_cnt < MEAS_IBIKI_CNT_MAX ){
		s_unit.ibiki_val[s_unit.ibiki_cnt] = s_unit.meas.info.dat.ibiki_val;
	}
	
	// ƒf[ƒ^ƒtƒ‹‚Å‰‰ZŒÄo
	if( s_unit.kokyu_cnt >= ( DATA_SIZE_APNEA - 1 )){
		apnea_state = main_calc_kokyu();
	}else
	{
		apnea_state = 99;
	}

	if( s_unit.ibiki_cnt >= ( DATA_SIZE_APNEA - 1 )){
		snore_state = main_calc_ibiki();
	}else
	{
		snore_state = 99;
	}
	
	INC_MAX( s_unit.kokyu_cnt, MEAS_KOKYU_CNT_MAX );		
	INC_MAX( s_unit.ibiki_cnt, MEAS_IBIKI_CNT_MAX );		

	NO_OPERATION_BREAK_POINT();									// ƒuƒŒƒCƒNƒ|ƒCƒ“ƒgİ’u—p
}
#endif
#endif

#if FUNC_DEBUG_LOG != ON
/************************************************************************/
/* ŠÖ”     : user_main_calc_data_set_acl								*/
/* ŠÖ”–¼   : ‰‰Zƒf[ƒ^ƒZƒbƒgˆ—(‰Á‘¬)								*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.07.19 Axia Soft Design ˜a“c k‘¾	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ‰‰Zƒf[ƒ^ƒZƒbƒgˆ—(‰Á‘¬)									*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_calc_data_set_acl( void )
{
	if( s_unit.acl_cnt < MEAS_ACL_CNT_MAX ){
		s_unit.acl_x[s_unit.acl_cnt] = s_unit.meas.info.dat.acl_x;
		s_unit.acl_y[s_unit.acl_cnt] = s_unit.meas.info.dat.acl_y;
		s_unit.acl_z[s_unit.acl_cnt] = s_unit.meas.info.dat.acl_z;
	}
	
	// ‰Á‘¬‚Ì‰‰Z‚Íƒ†[ƒU[ƒCƒxƒ“ƒg(1•büŠú)‚ÅÀ{‚·‚é
	
	INC_MAX( s_unit.acl_cnt, MEAS_ACL_CNT_MAX );

	NO_OPERATION_BREAK_POINT();									// ƒuƒŒƒCƒNƒ|ƒCƒ“ƒgİ’u—p
}

/************************************************************************/
/* ŠÖ”     : user_main_calc_data_set_photoref							*/
/* ŠÖ”–¼   : 						*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.08.02 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 							*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_calc_data_set_photoref( void )
{
	s_unit.calc.info.dat.photoref[s_unit.phase_photoref] = s_unit.meas.info.dat.photoref_val;
	s_unit.phase_photoref++;
	if(s_unit.phase_photoref >= SEC_PHASE_NUM){
		s_unit.phase_photoref = SEC_PHASE_0_10;
	}
}
#endif

#if FUNC_DEBUG_LOG == ON
/************************************************************************/
/* ŠÖ”     : make_send_data											*/
/* ŠÖ”–¼   : ‘—Mƒf[ƒ^ì¬ˆ—										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2019.05.14 Axia Soft Design mmura		‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒ^ƒCƒ}[üŠúˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void make_send_data(char* pBuff)
{
	// ŒÄ‹z‰¹, ‚¢‚Ñ‚«‰¹, ‰Á‘¬“x(X), ‰Á‘¬“x(Y), ‰Á‘¬“x(Z), ƒtƒHƒgƒZƒ“ƒT[’l
	UB tmp;
	UH next;
	UB index = 0;
	
	// ŒÄ‹z‰¹
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
	
	// ‚¢‚Ñ‚«‰¹
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
	//’ÊíƒfƒoƒbƒO”Å
	// ‰Á‘¬“x(X)
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
	
	// ‰Á‘¬“x(Y)
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
	
	// ‰Á‘¬“x(Z)
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
	
	// ƒtƒHƒgƒZƒ“ƒT[
	tmp = s_unit.meas.info.dat.photoref_val / 100;
	next = s_unit.meas.info.dat.photoref_val % 100;
	pBuff[index++] = '0' + tmp;
	tmp = next / 10;
	next = next % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;
#else
	// ‚¢‚Ñ‚«”»’èŒ‹‰Ê
	tmp = snore_state / 10;
	next = snore_state % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;
	pBuff[index++] = ',';
	
	// –³ŒÄ‹z”»’èŒ‹‰Ê
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
/* ŠÖ”     : time_get_elapsed_time										*/
/* ŠÖ”–¼   : ƒ\ƒtƒgƒEƒFƒAŒo‰ßŠÔæ“¾ˆ—								*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : Œ»İ˜A‘±‰Ò“®ŠÔ											*/
/* •ÏX—š—ğ : 2012.01.30 Axia Soft Design H.Wada	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ :																*/
/* Œ»İ˜A‘±‰Ò“®ŠÔ‚Ìæ“¾‚ğs‚¤											*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/* ‚È‚µ																	*/
/************************************************************************/
UW time_get_elapsed_time( void )
{
	return s_unit.elapsed_time;
}

/************************************************************************/
/* ŠÖ”     : user_system_init											*/
/* ŠÖ”–¼   : ƒVƒXƒeƒ€ŠÖ˜A‰Šú‰»										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒVƒXƒeƒ€ŠÖ˜A‰Šú‰»											*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
void user_system_init( void )
{
	R_INTC_Create();
	R_IT_Create();
	R_RTC_Create();
}

/************************************************************************/
/* ŠÖ”     : user_main_init											*/
/* ŠÖ”–¼   : ƒƒCƒ“‰Šú‰»												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒƒCƒ“‰Šú‰»													*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
void user_main_init( void )
{
	// ƒ~ƒhƒ‹‰Šú‰»
#if FUNC_DEBUG_LOG == ON
	com_srv_init();
#endif
	i2c_init();
	eep_init();
	main_acl_init();
	main_acl_start();
	R_ADC_Create();
	
	// ƒƒCƒ“‚Ìƒf[ƒ^‰Šú‰»
	memset( &s_unit, 0, sizeof(s_unit) );
	
	
	//EEP“Ç‚İo‚µ
	user_main_eep_read_pow_on();
	
	s_unit.last_time_sts_req = time_get_elapsed_time();
	
	// ó‘Ôİ’è
	ke_state_set(USER_MAIN_ID, 0);
	
	// ‰‰Z‰Šú‰»
	calc_snore_init();
	
	// ¡b’è ‘Ò‹@ƒ‚[ƒh‚Ö‘JˆÚ‚³‚¹‚é‚½‚ß‚É‰¼‚É[“dƒ|[ƒg
	if( FALSE == evt_act( EVENT_CHG_PORT_ON )){
		NO_OPERATION_BREAK_POINT();									// ƒuƒŒƒCƒNƒ|ƒCƒ“ƒgİ’u—p
	}

#if FUNC_DEBUG_PORT == ON
	//‹ó‚«ƒ|[ƒg‚É‚æ‚éŒv‘ª—pİ’è(ƒXƒŠ[ƒv‚È‚Ç)
    write1_sfr(PM1,  5, PORT_OUTPUT);
//    write1_sfr(PM1,  6, PORT_OUTPUT);		//g‚¤‚ÆBLE‚ª“®‚©‚È‚­‚È‚é‚Ì‚Åíœ

    write1_sfr(P1, 5, 0);
#endif

}

/************************************************************************/
/* ŠÖ”     : sw_proc													*/
/* ŠÖ”–¼   : SWüŠúˆ—												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2019.07.26 Axia Soft Design ˜a“c k‘¾	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ :																*/
/* SWüŠúˆ—(20msüŠú)													*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/* ‚È‚µ																	*/
/************************************************************************/
STATIC void sw_proc(void)
{
	UB pow_sw;
	
	pow_sw = drv_i_port_read_pow_sw();
	
#if FUNC_DEBUG_LOG == ON
	if( ON == pow_sw ){		// ONˆ—
		// “dŒ¹SW‰Ÿ‰ºƒ^ƒCƒ}[Œp‘±
		s_unit.sw_time_cnt++;
		if(s_unit.system_mode == SYSTEM_MODE_IDLE_COM && s_unit.sw_time_cnt > TIME_20MS_CNT_POW_SW_SHORT_DEBUG && sw_on_flg == OFF){
			sw_on_flg = ON;
			// ï¿½fï¿½[ï¿½^ï¿½ï¿½ï¿½ï¿½ï¿½ï¿½
			memset(&s_unit.kokyu_val, 0, sizeof(s_unit.kokyu_val));
			memset(&s_unit.ibiki_val, 0, sizeof(s_unit.ibiki_val));
			s_unit.kokyu_cnt = 0;
			s_unit.ibiki_cnt = 0;
			Reset();
			
			
			s_unit.system_mode = SYSTEM_MODE_SENSING;
			led_green_on();
			s_unit.sw_time_cnt = 0;
		}
		
		if(s_unit.system_mode == SYSTEM_MODE_SENSING && s_unit.sw_time_cnt > TIME_20MS_CNT_POW_SW_SHORT_DEBUG && sw_on_flg == OFF){
			sw_on_flg = ON;
			s_unit.system_mode = SYSTEM_MODE_IDLE_COM;
			led_on();
			s_unit.sw_time_cnt = 0;
		}
	}else{
		sw_on_flg = OFF;
		s_unit.sw_time_cnt = 0;
		led_off();
	}
#else
	if( ON == pow_sw ){		// ONˆ—
		// “dŒ¹SW‰Ÿ‰ºƒ^ƒCƒ}[Œp‘±
		s_unit.sw_time_cnt++;
		
		if( s_unit.sw_time_cnt == TIME_20MS_CNT_POW_SW_LONG){
			// ‹K’èŠÔˆÈã˜A‘±‰Ÿ‰º‚Æ”»’f
			evt_act( EVENT_POW_SW_LONG );
		}
	}else{					// OFFˆ—
		if( ON == s_unit.pow_sw_last ){
			// ON¨OFFƒGƒbƒW
			if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_LONG){
				// ONŠm’è‚ÉƒCƒxƒ“ƒg”­¶Ï‚İ‚È‚Ì‚Å‚±‚±‚Å‚Í‰½‚à‚µ‚È‚¢
			}else if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_SHORT){
				evt_act( EVENT_POW_SW_SHORT );
			}else{
				// ‰½‚à‚µ‚È‚¢
			}
		}
		// “dŒ¹SW‰Ÿ‰ºƒ^ƒCƒ}[ÄƒXƒ^[ƒg 
		s_unit.sw_time_cnt = 0;
	}
	s_unit.pow_sw_last = pow_sw;
#endif
}

/************************************************************************/
/* ŠÖ”     : user_main_calc_result										*/
/* ŠÖ”–¼   : ‰‰ZŒ‹‰Êˆ—												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ‰‰ZŒ‹‰Êˆ—													*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_calc_result( void )
{
	UW wr_adrs = 0;

	//”ÍˆÍƒ`ƒFƒbƒN
	if( s_unit.calc_cnt > EEP_CACL_DATA_NUM ){
		err_info(ERR_ID_MAIN);
		return;
	}
	
	// ƒtƒŒ[ƒ€ˆÊ’u‚Æƒf[ƒ^ˆÊ’u‚©‚çEEPƒAƒhƒŒƒX‚ğZo
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + ( s_unit.calc_cnt * EEP_CACL_DATA_SIZE );

	eep_write( wr_adrs, (UB*)&s_unit.calc, EEP_CACL_DATA_SIZE, OFF );	// 30•büŠú‚È‚Ì‚Å5ms‘Ò‚¿‚Í‚µ‚È‚¢
	
	s_unit.calc_cnt++;
	NO_OPERATION_BREAK_POINT();									// ƒuƒŒƒCƒNƒ|ƒCƒ“ƒgİ’u—p
}

#if FUNC_DEBUG_LOG != ON
/************************************************************************/
/* ŠÖ”     : user_main_mode											*/
/* ŠÖ”–¼   : ƒƒCƒ“ƒ‚[ƒhˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒƒCƒ“ƒ‚[ƒhˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode( void )
{
	// ƒ‚[ƒh‹¤’Êˆ—
	user_main_mode_common();
	
	// Šeíƒ‚[ƒhˆ—
	p_user_main_mode_func[s_unit.system_mode]();
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_common										*/
/* ŠÖ”–¼   : ƒ‚[ƒh‹¤’Êˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒ‚[ƒh‹¤’Êˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_common( void )
{
	UW now_time;
	
	now_time = time_get_elapsed_time();
	
	// ƒ^ƒCƒ€ƒAƒEƒgƒ`ƒFƒbƒN
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
/* ŠÖ”     : user_main_mode_sensing_before								*/
/* ŠÖ”–¼   : ƒZƒ“ƒVƒ“ƒO‘Oˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒZƒ“ƒVƒ“ƒO‘Oˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_sensing_before( void )
{
	UW wr_adrs = 0;
	rtc_counter_value_t rtc_val;
	
	// BLE‚ÌLED‚ğÁ“”(b’è)¨–{—ˆ‚ÍƒZƒ“ƒVƒ“ƒOˆÚsBLEØ’f‚ÅÁ“”‚·‚é
	led_yellow_off();
	
	// “úî•ñæ“¾
	if( MD_OK != R_RTC_Get_CounterValue( &rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
	// BCD¨ƒoƒCƒiƒŠ•ÏŠ·
	bcd2bin(&s_unit.date.year, &rtc_val.year);
	bcd2bin(&s_unit.date.month, &rtc_val.month);
	bcd2bin(&s_unit.date.week, &rtc_val.week);
	bcd2bin(&s_unit.date.day, &rtc_val.day);
	bcd2bin(&s_unit.date.hour, &rtc_val.hour);
	bcd2bin(&s_unit.date.min, &rtc_val.min);
	bcd2bin(&s_unit.date.sec, &rtc_val.sec);
	
	// “úî•ñ‘‚«‚İ
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
	eep_write( wr_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE, ON );

	s_unit.calc_cnt = 0;
	
	// ƒZƒ“ƒT[æ“¾ƒf[ƒ^‚ğƒNƒŠƒA
	memset(s_unit.kokyu_val, 0, MEAS_KOKYU_CNT_MAX);
	memset(s_unit.ibiki_val, 0, MEAS_KOKYU_CNT_MAX);
	memset(s_unit.acl_x, 0, MEAS_ACL_CNT_MAX);
	memset(s_unit.acl_y, 0, MEAS_ACL_CNT_MAX);
	memset(s_unit.acl_z, 0, MEAS_ACL_CNT_MAX);
	Reset();
	
	s_unit.kokyu_cnt = 0;
	s_unit.ibiki_cnt = 0;
	s_unit.acl_cnt = 0;
	
	s_unit.sensing_cnt_50ms = 0;
	s_unit.yokusei_cnt_time_10sec = 0;
	s_unit.sensing_flg = ON;
	
	// ƒZƒ“ƒVƒ“ƒOˆÚs‚ÉLED‚ÆƒoƒCƒu“®ì
	if( s_unit.battery_sts == BAT_LEVEL_STS_HIGH || s_unit.battery_sts == BAT_LEVEL_STS_MAX )
	{
		set_led( LED_PATT_GREEN_LIGHTING );
	} else if( s_unit.battery_sts == BAT_LEVEL_STS_LOW ) {
		set_led( LED_PATT_GREEN_BLINK );
	}
	set_vib(VIB_MODE_SENSING);
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_sensing_after								*/
/* ŠÖ”–¼   : ƒZƒ“ƒVƒ“ƒOŒãˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒZƒ“ƒVƒ“ƒOŒãˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_sensing_after( void )
{
	UB oikosi_flg = OFF;
	UW wr_adrs = 0;
	UB wr_data[3] = {0};
	
	set_led(LED_PATT_OFF);
	
	if( 0 == s_unit.calc_cnt ){
		err_info(ERR_ID_MAIN);
		return;
	}
	
	// ‰ºŒÀ–¢–‚Í•Û‘¶‚µ‚È‚¢
	if( SENSING_CNT_MIN > s_unit.calc_cnt ){
		return;
	}
	
	// EEP‘‚«‚İ‘Ò‚¿ ¦üŠúˆ—‚Å‚Í5ms‘Ò‚¿‚µ‚Ä‚¢‚È‚¢‚Ì‚ÅÅŒã‚Ìƒ^ƒCƒ~ƒ“ƒO‚Å‚ÍŒÅ’è‘Ò‚¿‚·‚é
	wait_ms(5);
	
	// ‰‰Z‰ñ”‘‚«‚İ
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_CALC_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.calc_cnt, 2, ON );
	
	// ’Ç‚¢‰z‚µ”»’è
	if(( s_unit.frame_num.write == s_unit.frame_num.read ) &&
	   ( s_unit.frame_num.cnt > 0 )){
		// ’Ç‚¢‰z‚³‚ê‚Ä‚µ‚Ü‚¤‚Ì‚Å“Ç‚İo‚µƒ|ƒCƒ“ƒ^‚ği‚ß‚é
		oikosi_flg = ON;
	}
	
	// ‘‚«‚İ˜g”Ô†‚ği‚ß‚é
	INC_MAX_INI(s_unit.frame_num.write, (EEP_FRAME_MAX - 1), 0);
	INC_MAX(s_unit.frame_num.cnt, EEP_FRAME_MAX );
	
	
	wr_adrs = EEP_ADRS_TOP_SETTING;
	if( OFF == oikosi_flg ){
		// ‘‚«‚İƒ|ƒCƒ“ƒ^
		wr_data[0] = s_unit.frame_num.write;
		wr_data[1] = s_unit.frame_num.cnt;
		eep_write( wr_adrs + 1, &wr_data[0], 2, ON );
	}else{
		INC_MAX_INI(s_unit.frame_num.read, (EEP_FRAME_MAX  -1), 0);
		// ‘‚«‚İA“Ç‚İo‚µƒ|ƒCƒ“ƒ^
		wr_data[0] = s_unit.frame_num.read;
		wr_data[1] = s_unit.frame_num.write;
		wr_data[2] = s_unit.frame_num.cnt;
		eep_write( wr_adrs, &wr_data[0], 3, ON );
	}
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_get_frame_before							*/
/* ŠÖ”–¼   : GETƒ‚[ƒhƒtƒŒ[ƒ€‘Oˆ—									*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : GETƒ‚[ƒhƒtƒŒ[ƒ€‘Oˆ—										*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC UB user_main_mode_get_frame_before( void )
{
	UB ret = ON;
	UW rd_adrs = 0;
	UH calc_cnt;
	
	// ƒf[ƒ^‰Šú‰»
	s_unit.get_mode_calc_cnt = 0;
	
	// ƒtƒŒ[ƒ€ˆÊ’u‚Æƒf[ƒ^ˆÊ’u‚©‚çEEPƒAƒhƒŒƒX‚ğZo
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
/* ŠÖ”     : user_main_mode_get_before									*/
/* ŠÖ”–¼   : GETƒ‚[ƒh‘Oˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : GETƒ‚[ƒh‘Oˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_get_before( void )
{
	UB tx[VUART_DATA_SIZE_MAX];
	
	s_unit.frame_num_work = s_unit.frame_num;	//’†’f‚ğl—¶‚µƒ[ƒN‚ÉƒRƒs[‚µƒ[ƒN‚ÅÀs‚·‚é
	
	if(( s_unit.frame_num_work.write == s_unit.frame_num_work.read ) && 
	   ( s_unit.frame_num_work.cnt == 0 )){
		//ƒf[ƒ^‚È‚µ‚ÍŠ®—¹
		tx[0] = 0xE1;		// END
		main_vuart_send( &tx[0], 1 );
#if FUNC_DEBUG_FIN_NON == OFF
		s_unit.get_mode_seq = 6;
#else
		// Š®—¹’Ê’m•s—v(ƒ†ƒjƒA[ƒX”Å)
		s_unit.get_mode_seq = 7;
#endif
		return;
	}
	
	user_main_mode_get_frame_before();
}


/************************************************************************/
/* ŠÖ”     : user_main_mode_get_after									*/
/* ŠÖ”–¼   : GETƒ‚[ƒhŒãˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : GETƒ‚[ƒhŒãˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_get_after( void )
{
	UW wr_adrs = 0;

	// ƒ[ƒN‚©‚ç³‹K—Ìˆæ‚ÖƒRƒs[
	s_unit.frame_num = s_unit.frame_num_work;
	
	wr_adrs = EEP_ADRS_TOP_SETTING;
	eep_write( wr_adrs, &s_unit.frame_num.read, 1, ON );
	s_unit.frame_num.cnt = 0;
	eep_write(( wr_adrs + 2 ), &s_unit.frame_num.cnt, 1, ON );
	
	// ƒVƒXƒeƒ€–ß‚·
	evt_act( EVENT_COMPLETE );
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_inital										*/
/* ŠÖ”–¼   : ƒCƒjƒVƒƒƒ‹ó‘Ôˆ—										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒCƒjƒVƒƒƒ‹ó‘Ôˆ—											*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_inital(void)
{
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_idle_com									*/
/* ŠÖ”–¼   : ƒAƒCƒhƒ‹_’ÊM‘Ò‹@ó‘Ôˆ—									*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒAƒCƒhƒ‹_’ÊM‘Ò‹@ó‘Ôˆ—										*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_idle_com(void)
{
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_sensing									*/
/* ŠÖ”–¼   : ƒZƒ“ƒVƒ“ƒOó‘Ôˆ—										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ƒZƒ“ƒVƒ“ƒOó‘Ôˆ—											*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_sensing(void)
{
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_move										*/
/* ŠÖ”–¼   : ˆÚsó‘Ôˆ—												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ˆÚsó‘Ôˆ—													*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_move(void)
{
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_get										*/
/* ŠÖ”–¼   : GETó‘Ôˆ—												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : GETó‘Ôˆ—													*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_get(void)
{
	UB tx[VUART_DATA_SIZE_MAX];
	UW rd_adrs;
	CALC calc_eep;				// ‰‰ZŒãƒf[ƒ^
	
	// ‘—M’†‚Ìê‡‚ÍƒEƒFƒCƒg
	if( ON == s_ds.vuart.input.send_status ){
		return;
	}
	
	if( 0 == s_unit.get_mode_seq ){
		user_main_mode_get_before();
	}else if( 1 == s_unit.get_mode_seq ){
		// “ú“Ç‚İo‚µ
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
		eep_read( rd_adrs, (UB*)&s_unit.date, EEP_CACL_DATA_SIZE );
		// ‚¢‚Ñ‚«ŒŸ’m”“Ç‚İo‚µ
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
		eep_read( rd_adrs, (UB*)&s_unit.ibiki_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE );
		// –³ŒÄ‹zŒŸ’m”“Ç‚İo‚µ
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
		eep_read( rd_adrs, (UB*)&s_unit.mukokyu_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE );
		// ‚¢‚Ñ‚«ŠÔ“Ç‚İo‚µ
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE );
		// –³ŒÄ‹zŠÔ“Ç‚İo‚µ
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE );
		// Å‚–³ŒÄ‹zŠÔ“Ç‚İo‚µ
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE );
		
		s_unit.get_mode_seq = 2;
	}else if( 2 == s_unit.get_mode_seq ){
		tx[0] = VUART_CMD_DATA_FRAME;		//˜gî•ñ(“ú‘¼)
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
			// EEP“Ç‚İo‚µ
			// ƒtƒŒ[ƒ€ˆÊ’u‚Æƒf[ƒ^ˆÊ’u‚©‚çEEPƒAƒhƒŒƒX‚ğZo
			rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + ( s_unit.get_mode_calc_cnt * EEP_CACL_DATA_SIZE );
			
			eep_read( rd_adrs, (UB*)&calc_eep, EEP_CACL_DATA_SIZE );
			
			// VUART(BLE)‘—M
			// ƒXƒ}ƒz‚ÌIF‚É‡‚í‚¹‚é
			tx[0] = VUART_CMD_DATA_CALC;	// ‰‰Zƒf[ƒ^
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
		//“Ç‚İo‚µ˜g”Ô†i‚ß‚é
		INC_MAX_INI(s_unit.frame_num_work.read, ( EEP_FRAME_MAX - 1), 0);
		if( s_unit.frame_num_work.write == s_unit.frame_num_work.read ){
			// I—¹
			s_unit.get_mode_seq = 5;
		}else{
			//Œp‘±
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
			// Š®—¹’Ê’m•s—v(ƒ†ƒjƒA[ƒX”Å)
			s_unit.get_mode_seq = 7;
		#endif
	}else if( 6 == s_unit.get_mode_seq ){
		// RD8001b’èF‰e•‘19:Š®—¹’Ê’m‘Ò‚¿@¦ƒ^ƒCƒ€ƒAƒEƒg•K—vH
	}else{
		user_main_mode_get_after();
	}
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_prg_g1d									*/
/* ŠÖ”–¼   : G1DƒvƒƒOƒ‰ƒ€XVó‘Ôˆ—									*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : G1DƒvƒƒOƒ‰ƒ€XVó‘Ôˆ—										*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
//RD8001b’èFG1Dƒ_ƒEƒ“ƒ[ƒh_ˆ—Šm”F’†
STATIC void user_main_mode_prg_g1d(void)
{
}

/************************************************************************/
/* ŠÖ”     : user_main_mode_self_check									*/
/* ŠÖ”–¼   : ©ŒÈf’fó‘Ôˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ	: 2018.09.10 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ©ŒÈf’fó‘Ôˆ—												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void user_main_mode_self_check( void )
{
	UB read_eep[EEP_ACCESS_ONCE_SIZE];
	UW now_time = time_get_elapsed_time();

	if( 0 == s_unit.self_check.seq ){
		// ‘S0‘‚«‚İ
		// EEPƒvƒƒOƒ‰ƒ€ƒ‚[ƒh
		eep_write( s_unit.self_check.eep_cnt * EEP_ACCESS_ONCE_SIZE, (UB*)&s_eep_page0_tbl, EEP_ACCESS_ONCE_SIZE, ON );
		INC_MAX( s_unit.self_check.eep_cnt, EEP_PAGE_CNT_MAX );
		if( s_unit.self_check.eep_cnt >= EEP_PAGE_CNT_MAX ){
			s_unit.self_check.eep_cnt = 0;
			s_unit.self_check.seq = 1;
		}
	}else if( 1 == s_unit.self_check.seq ){
		// ‘S0“Ç‚İo‚µ
		// ƒtƒŒ[ƒ€ˆÊ’u‚Æƒf[ƒ^ˆÊ’u‚©‚çEEPƒAƒhƒŒƒX‚ğZo
		eep_read( s_unit.self_check.eep_cnt * EEP_ACCESS_ONCE_SIZE, &read_eep[0], EEP_ACCESS_ONCE_SIZE );
		if( 0 != memcmp( &s_eep_page0_tbl[0], &read_eep[0], EEP_ACCESS_ONCE_SIZE)){
			s_unit.self_check.seq = 2;
			s_unit.self_check.last_time = now_time;
		}
		INC_MAX( s_unit.self_check.eep_cnt, EEP_PAGE_CNT_MAX );
		if( s_unit.self_check.eep_cnt >= EEP_PAGE_CNT_MAX ){
			s_unit.self_check.eep_cnt = 0;
			s_unit.self_check.seq = 3;
			s_unit.self_check.last_time = now_time;
		}
	}else if( 2 == s_unit.self_check.seq ){
		// ˆÙí•\¦
		if(( now_time - s_unit.self_check.last_time ) >= TIME_CNT_DISP_SELF_CHECK_ERR ){
			s_unit.self_check.seq = 3;
			s_unit.self_check.last_time = now_time;
		}
	}else if( 3 == s_unit.self_check.seq ){
		// Š®—¹
		if(( now_time - s_unit.self_check.last_time ) >= TIME_CNT_DISP_SELF_CHECK_FIN ){
			s_unit.self_check.seq = 4;
		}
	}else{
		// Š®—¹
		if( ON ==  s_unit.self_check.com_flg ){
			s_unit.self_check.com_flg = OFF;
			evt_act( EVENT_COMPLETE );
		}
	}
}

/************************************************************************/
/* ŠÖ”     : err_info													*/
/* ŠÖ”–¼   : ˆÙí’Ê’m													*/
/* ˆø”     : ˆÙíID(10i2Œ…)											*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : G1D‘¤‚ÌˆÙí‚ğH1DŒo—R‚ÅƒƒOo—Í(ƒfƒoƒbƒO‹@”\)					*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/* ‡@ƒfƒoƒbƒO‹@”\‚Å‚·BƒGƒ‰[o—Ío—ˆ‚È‚¢‰Â”\«‚ª‚ ‚è‚Ü‚·B				*/
/* ‡A–{ƒƒO‚ªŒ´ˆö‚Å•s‹ï‡‚ª”­¶‚·‚é–‚ª‚ ‚è‚Ü‚·B——R‚Æ‚µ‚Ä‚Í»•i‹@”\‚Ì	*/
/*   CPUŠÔ’ÊM‚ÉŠ„‚è‚Şˆ×B											*/
/************************************************************************/
void err_info( ERR_ID id )
{
#if 0	// CPUŠÔ’ÊM‚Ìíœ‚Ì‚½‚ßˆê’Uíœ‚·‚é(g—p‚·‚éê‡‚Í•Ê‚ÌIF‚ğ—pˆÓ‚·‚é•K—v‚ ‚è)
#if FUNC_DEBUG_LOG == ON
	// ƒƒOo—Í
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
		main_cpu_com_snd_pc_log( (UB*)&tx[0], CPU_COM_SND_DATA_SIZE_PC_LOG );		// ƒfƒoƒbƒO
		NO_OPERATION_BREAK_POINT();									// ƒuƒŒƒCƒNƒ|ƒCƒ“ƒgİ’u—p
	}
	
#if 0
	while(1){
		// ˆÙí‚É‚æ‚é‰i‹vƒ‹[ƒv
	}
#endif
#endif
#endif	// CPUŠÔ’ÊM‚Ìíœ‚Ì‚½‚ßˆê’Uíœ‚·‚é(g—p‚·‚éê‡‚Í•Ê‚ÌIF‚ğ—pˆÓ‚·‚é•K—v‚ ‚è)

	s_unit.err_cnt++;
	s_unit.last_err_id = id;
}

/************************************************************************/
/* ŠÖ”     : evt_act													*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒgÀs												*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : TRUE	À{												*/
/*          : FALSE	–¢À{												*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒgÀs													*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC UB evt_act( EVENT_NUM evt )
{
	SYSTEM_MODE	system_mode;
	
	
	system_mode = p_event_table[evt][s_unit.system_mode]( evt );
	if( SYSTEM_MODE_NON == system_mode ){
		return FALSE;
	}
	
	if(EVENT_POW_SW_SHORT != evt)
	{
		main_chg_system_mode( system_mode );
	}
	
	return TRUE;
}

/************************************************************************/
/* ŠÖ”     : evt_non													*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(‚È‚µ)											*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(‚È‚µ)												*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_non( int evt)
{
	return SYSTEM_MODE_NON;
};

/************************************************************************/
/* ŠÖ”     : evt_idle_com												*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(ƒAƒCƒhƒ‹_’ÊM‘Ò‹@)								*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(ƒAƒCƒhƒ‹_’ÊM‘Ò‹@)									*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_idle_com( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_COM;
	
	return system_mode;
}

/************************************************************************/
/* ŠÖ”     : evt_idle_com												*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(ƒAƒCƒhƒ‹_’ÊM‘Ò‹@_“d’rc—Êƒ`ƒFƒbƒN‚ ‚è)			*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(ƒAƒCƒhƒ‹_’ÊM‘Ò‹@_“d’rc—Êƒ`ƒFƒbƒN‚ ‚è)				*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
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
/* ŠÖ”     : evt_sensing												*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(ƒZƒ“ƒVƒ“ƒO)										*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(ƒZƒ“ƒVƒ“ƒO)											*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_sensing( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SENSING;
	
	return system_mode;
}

/************************************************************************/
/* ŠÖ”     : evt_sensing_chg											*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(ƒZƒ“ƒVƒ“ƒO_[“dó‘Ôƒ`ƒFƒbƒN‚ ‚è)					*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(ƒZƒ“ƒVƒ“ƒO_[“dó‘Ôƒ`ƒFƒbƒN‚ ‚è)						*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_sensing_chg( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SENSING;
	UB bat = drv_i_port_bat_chg_detect();
	
	//“d’rc—ÊŠm”F
	main_set_battery();
	
	if( s_unit.battery_sts == BAT_LEVEL_STS_MIN )
	{
		// “d’rc—Ê‚È‚µ‚È‚çˆÚs‚µ‚È‚¢
		system_mode = SYSTEM_MODE_IDLE_COM;
		set_led( LED_PATT_GREEN_BLINK_LOW_BATT );	
	}
	else if( bat == ON ){
		// [“d’†‚È‚çƒZƒ“ƒVƒ“ƒO‚ÉˆÚs‚µ‚È‚¢
		system_mode = SYSTEM_MODE_IDLE_COM;
	}
	
	return system_mode;
}

/************************************************************************/
/* ŠÖ”     : evt_initial												*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(ƒCƒjƒVƒƒƒ‹)										*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(ƒCƒjƒVƒƒƒ‹)											*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_initial( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_COM;
	
	return system_mode;
}

/************************************************************************/
/* ŠÖ”     : evt_bat_check												*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(“d’rc—ÊŠm”F)									*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2019.08.08  oneA Š‹Œ´ OˆÀ ‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(“d’rc—ÊŠm”F)										*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_bat_check( int evt)
{
	main_set_battery();
	// LED§Œä
	if( s_unit.battery_sts == BAT_LEVEL_STS_HIGH || s_unit.battery_sts == BAT_LEVEL_STS_MAX )
	{
		set_led( LED_PATT_GREEN_LIGHTING );
	} else if( s_unit.battery_sts == BAT_LEVEL_STS_LOW ) {
		set_led( LED_PATT_GREEN_BLINK );
	}
	return s_unit.system_mode;
}

STATIC SYSTEM_MODE evt_send_clear( int evt)
{
	s_ds.vuart.input.send_status = OFF;
	s_unit.system_mode = SYSTEM_MODE_IDLE_COM;
	set_led( LED_PATT_YELLOW_BLINK );
	return s_unit.system_mode;
}

/************************************************************************/
/* ŠÖ”     : evt_get													*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(GETƒ‚[ƒh)										*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(GETƒ‚[ƒh)											*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_get( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_GET;
	
	return system_mode;
}

/************************************************************************/
/* ŠÖ”     : evt_g1d_prg_denchi										*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(G1DƒvƒƒOƒ‰ƒ€XV_“d’rƒ`ƒFƒbƒN‚ ‚è)				*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(G1DƒvƒƒOƒ‰ƒ€XV_“d’rƒ`ƒFƒbƒN‚ ‚è)					*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
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
/* ŠÖ”     : evt_g1d_prg_denchi										*/
/* ŠÖ”–¼   : ƒCƒxƒ“ƒg(©ŒÈf’f)										*/
/* ˆø”     : evt	ƒCƒxƒ“ƒg”Ô†										*/
/* –ß‚è’l   : ƒVƒXƒeƒ€ƒ‚[ƒh											*/
/* •ÏX—š—ğ : 2018.09.07  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒCƒxƒ“ƒg(©ŒÈf’f)											*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_self_check( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SELF_CHECK;
	
	s_unit.self_check.seq = 0;
	
	return system_mode;
}

/************************************************************************/
/* ŠÖ”     : main_mode_chg												*/
/* ŠÖ”–¼   : ƒ‚[ƒh•ÏX												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.05.19  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒ‚[ƒh•ÏX													*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void main_mode_chg( void )
{
	if( SYSTEM_MODE_MOVE != s_unit.system_mode){
		return;
	}
	
	s_unit.system_mode = s_unit.next_system_mode;		// ƒ‚[ƒh•ÏX
	
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
		//RD8001b’èFG1Dƒ_ƒEƒ“ƒ[ƒh_ˆ—Šm”F’†_‰“š‚ğ•Ô‚¹‚é‚æ‚¤‚ÉC³
		FW_Update_Receiver_Start();
	}
	
	if( SYSTEM_MODE_SELF_CHECK == s_unit.system_mode ){
		if( ON == s_unit.self_check.com_flg ){
			{
				UB tx[VUART_DATA_SIZE_MAX] = {0};
				
				// OK‰“š
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
			
			// OK‰“š
			tx[0] = VUART_CMD_MODE_CHG;
			tx[1] = 0x00;
			
			s_ds.vuart.input.send_status = OFF;
			main_vuart_send( &tx[0], 2 );
			
			s_unit.get_mode_seq = 0;
		}
	}
}

/************************************************************************/
/* ŠÖ”     : main_chg_system_mode										*/
/* ŠÖ”–¼   : ƒ‚[ƒh•ÏX												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.05.19  Axia Soft Design ¼“‡ ‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : ƒ‚[ƒh•ÏX													*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode )
{
	s_unit.next_system_mode = next_mode;
	s_unit.system_mode = SYSTEM_MODE_MOVE;
	
	// ˆÈ‘O‚ÍCPUŠÔ’ÊM‚Ìƒ‚[ƒh•ÏX‚ğ‘Ò‚Á‚Äƒ‚[ƒh‘JˆÚ‚ğs‚Á‚Ä‚¢‚½‚ªA
	// CPUŠÔ’ÊM‚Í‚È‚­‚È‚Á‚½‚Ì‚ÅA’¼Ú‘JˆÚˆ—‚ğs‚¤
	main_mode_chg();
}

#if FUNC_DEBUG_LOG != ON
/************************************************************************/
/* ŠÖ”     : main_vuart_proc											*/
/* ŠÖ”–¼   : VUARTüŠúˆ—												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ : üŠúˆ—														*/
/************************************************************************/
/* ’ˆÓ–€ :‚È‚µ														*/
/************************************************************************/
STATIC void main_vuart_proc(void)
{
	int i  =0;
	
	if( 0 == s_ds.vuart.input.rcv_len ){
		return;		//óM‚È‚µ
	}
	
	if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
		// RD8001b’èFƒCƒjƒVƒƒƒ‹‚ÍóM‚¹‚¸ƒNƒŠƒA
		s_ds.vuart.input.rcv_len = 0;
		return;		//óM‚È‚µ
	}
	
	/* ˆÈ~óMƒf[ƒ^—L‚è */
	for( i = VUART_CMD_TYPE_NONE; i < VUART_CMD_TYPE_MAX; i++){
		if( s_vuart_rcv_func_tbl[i].len == s_ds.vuart.input.rcv_len ){
			/* ƒf[ƒ^’·‚ªˆê’v */
			if(( s_vuart_rcv_func_tbl[i].cmd == s_ds.vuart.input.rcv_data[0] ) ||
			   ( s_vuart_rcv_func_tbl[i].cmd == VUART_CMD_INVALID )){
				/* óMƒRƒ}ƒ“ƒh‚ÆƒRƒ}ƒ“ƒhƒe[ƒuƒ‹‚ªˆê’v‚Ü‚½‚ÍƒRƒ}ƒ“ƒh–³Œø */
				if( NULL != s_vuart_rcv_func_tbl[i].func ){
					/* óMˆ——L‚è */
					s_vuart_rcv_func_tbl[i].func();
				}
			}
		}
	}

#if 1
	// RD8001b’èFG1Dƒ_ƒEƒ“ƒ[ƒh_ˆ—Šm”F’†(ƒeƒXƒgƒ{[ƒh‚Å‚ÌƒfƒoƒbƒOˆ—)
	if(( VUART_CMD_LEN_PRG_G1D_START == s_ds.vuart.input.rcv_len  ) && 
	         ( VUART_CMD_PRG_G1D_START == s_ds.vuart.input.rcv_data[0] )){
		FW_Update_Receiver_Start();
	}else if(( VUART_CMD_LEN_PRG_G1D_VER == s_ds.vuart.input.rcv_len  ) && 
	         ( VUART_CMD_PRG_G1D_VER == s_ds.vuart.input.rcv_data[0] )){
		s_unit.prg_g1d_send_ver_flg = ON;
		s_unit.prg_g1d_send_ver_sec = 5;	// 5•bŒã
	}else{
		// ŠY“–ƒRƒ}ƒ“ƒh‚È‚µ
		
	}
#endif
	
	// óM’·ƒNƒŠƒA
	s_ds.vuart.input.rcv_len = 0;
}
#endif

/************************************************************************/
/* ŠÖ”     : ds_set_vuart_data											*/
/* ŠÖ”–¼   : VUART’ÊMƒf[ƒ^ƒZƒbƒg										*/
/* ˆø”     : VUART’ÊMƒf[ƒ^Ši”[ƒ|ƒCƒ“ƒ^								*/
/*          : ƒf[ƒ^’·													*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUART’ÊMƒf[ƒ^ƒZƒbƒg												*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
STATIC void main_vuart_send( UB *p_data, UB len )
{
	// Vuart‘—M’†‚Í
	if( ON == s_ds.vuart.input.send_status ){
		err_info(ERR_ID_BLE_SEND_ERR);
		return;
	}
	
	s_ds.vuart.input.send_status = ON;
	R_APP_VUART_Send_Char( (char *)p_data, len );
}

/************************************************************************/
/* ŠÖ”     : main_vuart_set_mode									*/
/* ŠÖ”–¼   : VUART’ÊMƒf[ƒ^ƒZƒbƒg										*/
/* ˆø”     : VUART’ÊMƒf[ƒ^Ši”[ƒ|ƒCƒ“ƒ^								*/
/*          : ƒf[ƒ^’·													*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUART’ÊMƒf[ƒ^ƒZƒbƒg												*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
void main_vuart_set_mode( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	
	// OK‰“š
	tx[0] = VUART_CMD_MODE_CHG;
	tx[1] = 0x00;
	
	main_vuart_send( &tx[0], 2 );

	s_unit.calc_cnt = 0;
}


/************************************************************************/
/* ŠÖ”     : main_vuart_rcv_mode_chg									*/
/* ŠÖ”–¼   : VUART’ÊMƒf[ƒ^ƒZƒbƒg										*/
/* ˆø”     : VUART’ÊMƒf[ƒ^Ši”[ƒ|ƒCƒ“ƒ^								*/
/*          : ƒf[ƒ^’·													*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUART’ÊMƒf[ƒ^ƒZƒbƒg												*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
STATIC void main_vuart_rcv_mode_chg( void )
{
	UB ret = TRUE;
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	if( 3 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_GET_DATA );
	}else if( 4 == s_ds.vuart.input.rcv_data[1] ){
		main_vuart_set_mode();		// RD8001b’èFƒfƒoƒbƒO—pƒf[ƒ^İ’è(SETƒRƒ}ƒ“ƒh_ÅI•K—vH)
	}else if( 5 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_G1D_PRG );
	}else if( 6 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_SELF_CHECK_COM );
		if( TRUE == ret){
			s_unit.self_check.com_flg = ON;
		} 
	}else{
		// ‰½‚à‚µ‚È‚¢
	}
	
	if( FALSE == ret ){
		// NG‰“š
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = VUART_DATA_RESULT_NG;
		main_vuart_send( &tx[0], 2 );
	}else{
		// OK‰“š
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = VUART_DATA_RESULT_OK;
		main_vuart_send( &tx[0], 2 );
	}
}

/************************************************************************/
/* ŠÖ”     : main_vuart_rcv_info										*/
/* ŠÖ”–¼   : VUART’ÊM(î•ñæ“¾)										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUART’ÊM(î•ñæ“¾)													*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
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
/* ŠÖ”     : main_vuart_rcv_version									*/
/* ŠÖ”–¼   : VUARTóM(ƒo[ƒWƒ‡ƒ“)										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUARTóM(ƒo[ƒWƒ‡ƒ“)												*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
STATIC void main_vuart_rcv_version( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};

	// OK‰“š
	tx[0] = VUART_CMD_VERSION;
	// G1Dƒo[ƒWƒ‡ƒ“
	tx[1] = VUART_DATA_RESULT_OK;
	tx[2] = version_product_tbl[0];
	tx[3] = version_product_tbl[1];
	tx[4] = version_product_tbl[2];
	tx[5] = version_product_tbl[3];
	main_vuart_send( &tx[0], VUART_SND_LEN_VERSION );
}


/************************************************************************/
/* ŠÖ”     : main_set_bd_adrs											*/
/* ŠÖ”–¼   : BDƒAƒhƒŒƒXİ’è											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* BDƒAƒhƒŒƒXİ’è														*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
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
/* ŠÖ”     : main_vuart_rcv_device_info								*/
/* ŠÖ”–¼   : VUARTóM(ƒfƒoƒCƒXî•ñ)									*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUARTóM(ƒfƒoƒCƒXî•ñ)												*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
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
	
	// BCD¨ƒoƒCƒiƒŠ•ÏŠ·
	bcd2bin(&rtc_val_bin.year, &rtc_val.year);
	bcd2bin(&rtc_val_bin.month, &rtc_val.month);
	bcd2bin(&rtc_val_bin.week, &rtc_val.week);
	bcd2bin(&rtc_val_bin.day, &rtc_val.day);
	bcd2bin(&rtc_val_bin.hour, &rtc_val.hour);
	bcd2bin(&rtc_val_bin.min, &rtc_val.min);
	bcd2bin(&rtc_val_bin.sec, &rtc_val.sec);
	
	tx[0] = VUART_CMD_DEVICE_INFO;
	tx[1] = result;							// Œ‹‰Ê
	tx[2] = s_unit.bd_device_adrs[0];		// BDƒfƒoƒCƒXƒAƒhƒŒƒX
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
/* ŠÖ”     : main_vuart_rcv_data_frame									*/
/* ŠÖ”–¼   : VUARTóM(ƒtƒŒ[ƒ€)										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUARTóM(ƒtƒŒ[ƒ€)													*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
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
	
	// “úî•ñ‘‚«‚İ
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
	eep_write( wr_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE, ON );
	// ‚¢‚Ñ‚«ŒŸ’m”‘‚«‚İ
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE, ON );
	// –³ŒÄ‹zŒŸ’m”‘‚«‚İ
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE, ON );
	// ‚¢‚Ñ‚«ŠÔ‘‚«‚İ
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE, ON );
	// –³ŒÄ‹zŠÔ‘‚«‚İ
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE, ON );
	// Å‚–³ŒÄ‹zŠÔ‘‚«‚İ
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE, ON );
	
}
/************************************************************************/
/* ŠÖ”     : main_vuart_rcv_data_calc									*/
/* ŠÖ”–¼   : VUARTóM(‰‰Z)											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUARTóM(‰‰Z)														*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
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
/* ŠÖ”     : main_vuart_rcv_data_end									*/
/* ŠÖ”–¼   : VUARTóM(END)											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUARTóM(END)														*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
void main_vuart_rcv_data_end( void )
{
	user_main_mode_sensing_after();
}

/************************************************************************/
/* ŠÖ”     : main_vuart_rcv_data_fin									*/
/* ŠÖ”–¼   : VUARTóM(Š®—¹)											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUARTóM(Š®—¹)														*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
STATIC void main_vuart_rcv_data_fin( void )
{
	s_unit.get_mode_seq = 7;
}

/************************************************************************/
/* ŠÖ”     : main_vuart_rcv_date										*/
/* ŠÖ”–¼   : VUARTóM(“úİ’è)										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUARTóM(“úİ’è)													*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
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
	
	// ƒoƒCƒiƒŠ¨BCD•ÏŠ·
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
		main_vuart_send( &tx[0], 2 );
//	}
}

/************************************************************************/
/* ŠÖ”     : main_vuart_rcv_device_set									*/
/* ŠÖ”–¼   : VUARTóM(ƒfƒoƒCƒXİ’è)									*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2019.07.26  Axia Soft Design ˜a“c k‘¾	‰”Åì¬		*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUARTóM(ƒfƒoƒCƒXİ’è)												*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
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
		s_unit.alarm.info.dat.yokusei_str = s_ds.vuart.input.rcv_data[3];
		s_unit.alarm.info.dat.yokusei_max_time = s_ds.vuart.input.rcv_data[4];
		
		eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE, ON );
	}
	
	tx[0] = VUART_CMD_DEVICE_SET;
	tx[1] = result;
	main_vuart_send( &tx[0], 2 );
	
	if(result == VUART_DATA_RESULT_OK)
	{
#if FUNC_DEBUG_LOG != ON		
		// “®ìƒ‚[ƒhİ’è
		act_mode = s_unit.alarm.info.dat.act_mode;
		// ‚¢‚Ñ‚«Š´“xİ’è
		set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
		//—}§‹­“xİ’è
		vib_str = s_unit.alarm.info.dat.yokusei_str;
		//—}§“®ìÅ‘åŒp‘±ŠÔ
		set_yokusei_cnt_time(s_unit.alarm.info.dat.yokusei_max_time);
#endif
	}
}

/************************************************************************/
/* ŠÖ”     : ds_set_vuart_data											*/
/* ŠÖ”–¼   : VUART’ÊMƒf[ƒ^ƒZƒbƒg										*/
/* ˆø”     : VUART’ÊMƒf[ƒ^Ši”[ƒ|ƒCƒ“ƒ^								*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUART’ÊMƒ~ƒhƒ‹ƒf[ƒ^ƒZƒbƒgæ“¾										*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
void ds_set_vuart_data( UB *p_data, UB len )
{
	s_ds.vuart.input.rcv_len = len;
	memcpy( &s_ds.vuart.input.rcv_data[0], &p_data[0], s_ds.vuart.input.rcv_len );
}

/************************************************************************/
/* ŠÖ”     : ds_set_vuart_send_status									*/
/* ŠÖ”–¼   : VUART’ÊMƒf[ƒ^ƒZƒbƒg										*/
/* ˆø”     : VUART’ÊMƒf[ƒ^Ši”[ƒ|ƒCƒ“ƒ^								*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* VUART’ÊMƒ~ƒhƒ‹ƒf[ƒ^ƒZƒbƒgæ“¾										*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
void ds_set_vuart_send_status( UB status )
{
	s_ds.vuart.input.send_status = status;
}

// ============================
// ˆÈ~‰‰Z•”‚Ìˆ—
// ============================
/************************************************************************/
/* ŠÖ”     : main_calc_kokyu											*/
/* ŠÖ”–¼   : –³ŒÄ‹z‰‰Zˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 															*/
/************************************************************************/
/* ‹@”\ :																*/
/* 																		*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
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
		s_unit.calc.info.dat.state |= (set_kokyu_mask << bit_shift);		// –³ŒÄ‹zó‘ÔON
		if(act_mode != ACT_MODE_MONITOR)
		{//ƒ‚ƒjƒ^ƒŠƒ“ƒOƒ‚[ƒh‚Å‚È‚¢‚È‚çƒoƒCƒuƒŒ[ƒVƒ‡ƒ““®ì
//			set_vib(set_vib_mode(vib_str));
		}
	}else{
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// –³ŒÄ‹zó‘ÔOFF
	}
	// ‚à‚µA‚¢‚Ñ‚«‚à–³ŒÄ‹z‚à‚Ç‚¿‚ç‚àƒZƒbƒg‚³‚ê‚½‚ç‚¢‚Ñ‚«‚ğ—Dæ‚·‚é‚½‚ßA‚¢‚Ñ‚«ó‘Ô‚Æ‚·‚é
	if( (s_unit.calc.info.dat.state >> bit_shift) & 0x03 == 0x03 ){
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// –³ŒÄ‹zó‘ÔOFF
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// ‚¢‚Ñ‚«ó‘ÔON
	}
	
	s_unit.phase_kokyu++;
	if(s_unit.phase_kokyu >= SEC_PHASE_NUM){
		s_unit.phase_kokyu = SEC_PHASE_0_10;
	}
#endif
	
	return (KE_MSG_CONSUMED);
}
#else
#if FUNC_DEBUG_WAVEFORM_LOG == ON
//ƒfƒoƒbƒO”Å‰‰Zˆ—
static UB main_calc_kokyu( void)
{
	calculator_apnea(&s_unit.kokyu_val[0], &s_unit.ibiki_val[0]);
	s_unit.kokyu_cnt = 0;
	return get_state();
}
#endif
#endif

/************************************************************************/
/* ŠÖ”     : main_calc_ibiki											*/
/* ŠÖ”–¼   : ‚¢‚Ñ‚«‰‰Zˆ—											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 															*/
/************************************************************************/
/* ‹@”\ :																*/
/* 																		*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
#if FUNC_DEBUG_LOG != ON
static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	//‰‰Z³‹Kˆ—
	int ii;
	int max = s_unit.ibiki_val[0];
	UB newstate;
	UB	set_ibiki_mask = 0x01;
	UB	set_kokyu_mask = 0x02;
	UB	bit_shift = 0;

#if 0 // ƒeƒXƒg—pƒf[ƒ^
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

	// ‚¢‚Ñ‚«‰‰Z
	calc_snore_proc(&s_unit.ibiki_val[0]);
	newstate = calc_snore_get();
	
	
	if(yokusei_max_cnt_over_flg == ON)
	{// —}§“®ìÅ‘åŠÔƒI[ƒo[
		s_unit.cnt_overtime_10sec++;
		if( YOKUSEI_INTERVAL_CNT <= s_unit.cnt_overtime_10sec )
		{// —}§“®ìÅ‘åŠÔƒI[ƒo[‚ÌƒCƒ“ƒ^[ƒoƒ‹–—¹
			yokusei_max_cnt_over_flg = OFF;
			s_unit.cnt_overtime_10sec = 0;
			s_unit.yokusei_cnt_time_10sec = 0;
		}
	}
	
	bit_shift = s_unit.phase_ibiki * 2;
	if(newstate == SNORE_ON){
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// ‚¢‚Ñ‚«ó‘ÔON
		s_unit.yokusei_cnt_time_10sec++;
		if(act_mode != ACT_MODE_MONITOR)
		{//ƒ‚ƒjƒ^ƒŠƒ“ƒOƒ‚[ƒh‚Å‚È‚¢
			if(s_unit.yokusei_cnt_time_10sec <= yokusei_max_cnt)
			{//—}§“®ìÅ‘åŠÔˆÈ‰º
				if(yokusei_max_cnt_over_flg == OFF)
				{//—}§“®ìÅ‘åŠÔƒI[ƒo[ˆÈŠO
					set_vib(set_vib_mode(vib_str));
				}
			} else {
				//—}§“®ìÅ‘åŠÔƒI[ƒo[‚Éƒtƒ‰ƒOON
				yokusei_max_cnt_over_flg = ON;
			}
		}
	}else{
		s_unit.calc.info.dat.state &= ~(set_ibiki_mask << bit_shift);		// ‚¢‚Ñ‚«ó‘ÔOFF
		s_unit.yokusei_cnt_time_10sec = 0;	// ‰Šú‰»
	}
	// ‚à‚µA‚¢‚Ñ‚«‚à–³ŒÄ‹z‚à‚Ç‚¿‚ç‚àƒZƒbƒg‚³‚ê‚½‚ç‚¢‚Ñ‚«‚ğ—Dæ‚·‚é‚½‚ßA‚¢‚Ñ‚«ó‘Ô‚Æ‚·‚é
	if( (s_unit.calc.info.dat.state >> bit_shift) & 0x03 == 0x03 ){
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// –³ŒÄ‹zó‘ÔOFF
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// ‚¢‚Ñ‚«ó‘ÔON
	}
	
	s_unit.phase_ibiki++;
	if(s_unit.phase_ibiki >= SEC_PHASE_NUM){
		s_unit.phase_ibiki = SEC_PHASE_0_10;
	}	
	
#if 0	
	// ˆÚ“®—İŒv‚Æ‚é‚Ì‚Å‘O‚Ìƒf[ƒ^‚ğc‚·
	for(ii=0;ii<size;++ii){
		s_unit.ibiki_val[ii] = s_unit.ibiki_val[DATA_SIZE_APNEA-size+ii];
	}
	s_unit.ibiki_cnt = size;
#else
	s_unit.ibiki_cnt = 0;
#endif
	
#else
	//ƒfƒoƒbƒO—pƒ_ƒ~[ˆ—
	NO_OPERATION_BREAK_POINT();									// ƒuƒŒƒCƒNƒ|ƒCƒ“ƒgİ’u—p

	// ƒ_ƒ~[ƒf[ƒ^
	s_unit.ibiki_cnt = 0;
	s_unit.calc.info.dat.spo2_val = (UB)s_unit.ibiki_val[0];
#endif

	return (KE_MSG_CONSUMED);
}
#else
#if FUNC_DEBUG_WAVEFORM_LOG == ON
static UB main_calc_ibiki( void)
{
	// ‚¢‚Ñ‚«‰‰Z
	calc_snore_proc(&s_unit.ibiki_val[0]);
	s_unit.ibiki_cnt = 0;
	return calc_snore_get();
}
#endif
#endif

static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	B	acc_x;
//	B	acc_y;		//Œ»ó–¢g—p
	B	acc_z;
	
	UB	body_direct = BODY_DIRECTION_LEFT;
	UB	clear_mask = BODY_DIRECTION_MASK;
	UB	bit_shift = 0;
	
	// ÅV‚Ìƒf[ƒ^‚ğg‚¤
	acc_x = s_unit.acl_x[s_unit.acl_cnt];
//	acc_y = s_unit.acl_y[s_unit.acl_cnt];		//Œ»ó–¢g—p
	acc_z = s_unit.acl_z[s_unit.acl_cnt];

	s_unit.acl_cnt = 0;
	
	// ‘Ì‚ÌŒü‚«”»’è
	if( 0 <= acc_x )
	{// ã or ‰E
		if( 0 <= acc_z )
		{// ã
			body_direct = BODY_DIRECTION_UP;
		} else {
		 // ‰E
			body_direct = BODY_DIRECTION_RIGHT;
		}
	} else {
	// ‰º or ¶
		if( 0 <= acc_z )
		{// ¶
			body_direct = BODY_DIRECTION_LEFT;
		} else {
		 // ‰º
			body_direct = BODY_DIRECTION_DOWN;
		}
	}
	
	// 10•b‚²‚Æ‚Ì”»’è’l‚ğbit‚Åİ’è‚·‚é
	bit_shift = s_unit.phase_body_direct * BODY_DIRECTION_BIT;
	s_unit.calc.info.dat.body_direct &= ~(clear_mask << bit_shift);
	s_unit.calc.info.dat.body_direct |= (body_direct << bit_shift);
	// ¡b’è –{ŠÖ”‚Í10•b‚É1‰ñŒÄ‚Ño‚³‚ê‚é‚±‚Æ‚ğ‘O’ñ‚Æ‚µA10•b‚²‚Æ‚É•bŠÔƒtƒFƒCƒY‚ği‚ß‚é
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

static int_t main_calc_photoref(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	
	return (KE_MSG_CONSUMED);
}

/************************************************************************/
/* ŠÖ”     : user_main_sleep											*/
/* ŠÖ”–¼   : ƒ†[ƒU[ƒAƒvƒŠ‚ÌƒXƒŠ[ƒvƒ`ƒFƒbƒN							*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : true		sleep—LŒø										*/
/*          : false		sleep–³Œø										*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* ƒ†[ƒU[ƒAƒvƒŠ‚ÌƒXƒŠ[ƒvƒ`ƒFƒbƒN										*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
bool user_main_sleep(void)
{
#if FUNC_DEBUG_SLEEP_NON == ON
	return false;		//SLEEP–³Œø
#else
	bool ret = true;
	
	if( ret == true ){
		NO_OPERATION_BREAK_POINT();									// ƒuƒŒƒCƒNƒ|ƒCƒ“ƒgİ’u—p
	}
	
	return ret;
#endif

}

/************************************************************************/
/* ŠÖ”     : user_main_eep_read_pow_on									*/
/* ŠÖ”–¼   : EEP“Ç‚İo‚µˆ—(‹N“®)									*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.04.16  Axia Soft Design ¼“‡	‰”Åì¬				*/
/************************************************************************/
/* ‹@”\ :																*/
/* EEP“Ç‚İo‚µˆ—(‹N“®)												*/
/************************************************************************/
/* ’ˆÓ–€ :															*/
/************************************************************************/
STATIC void user_main_eep_read_pow_on(void)
{
	UB eep_type;
	
	// EEPí•Êƒ`ƒFƒbƒN
	eep_read( EEP_ADRS_DATA_TYPE, &eep_type, 1 );
	
	if( EEP_DATA_TYPE_NORMAL != eep_type){
		eep_part_erase();
	}
	
	// ƒtƒŒ[ƒ€ŠÖ˜A
	eep_read( EEP_ADRS_TOP_SETTING, &s_unit.frame_num.read, 1 );
	eep_read( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num.write, 1 );
	eep_read( EEP_ADRS_TOP_SETTING + 2, &s_unit.frame_num.cnt, 1 );
	
	// ”ÍˆÍƒ`ƒFƒbƒN
	if(( s_unit.frame_num.read > ( EEP_FRAME_MAX - 1)) ||
	   ( s_unit.frame_num.write > ( EEP_FRAME_MAX - 1)) ||
	   ( s_unit.frame_num.cnt > EEP_FRAME_MAX )){
		err_info(ERR_ID_MAIN);
		// ”ÍˆÍŠO‚È‚ç‰Šú‰»
		s_unit.frame_num.read = 0;
		s_unit.frame_num.write = 0;
		s_unit.frame_num.cnt = 0;
		eep_write( EEP_ADRS_TOP_SETTING, &s_unit.frame_num.read, 1, ON );
		eep_write( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num.write, 1, ON );
		eep_write( EEP_ADRS_TOP_SETTING + 2, &s_unit.frame_num.cnt, 1, ON );
	}
	
	
	// Œx‹@”\
	eep_read( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE );
	// RD8001bF”ÍˆÍƒ`ƒFƒbƒN“ü‚ê‚é
}

/************************************************************************/
/* ŠÖ”     : eep_all_erase												*/
/* ŠÖ”–¼   : EEP‘SÁ‹													*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.01.25 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : EEP‘SÁ‹														*/
/************************************************************************/
/* ’ˆÓ–€ : 															*/
/* ‡@ŠÔ‚ª‚©‚©‚éˆ×‚Ég—p‚·‚éÛ‚Í’‚·‚é–B–ñ7.5Sec						*/
/************************************************************************/
#if 0		//–¢g—pŠÖ”
STATIC void eep_all_erase( void )
{
	UW adrs = 0;
	UW i = 0;
	UB eep_data = EEP_DATA_TYPE_NORMAL;
	
	for( i = 0; i < (EEP_DATA_SIZE_ALL / EEP_ACCESS_ONCE_SIZE); i++ ){
		adrs = i * EEP_ACCESS_ONCE_SIZE;
		eep_write( adrs, (UB*)&s_eep_page0_tbl[0], EEP_ACCESS_ONCE_SIZE, ON );
	}
	
	// Á‹Ï‚İ‚Æ‚µ‚Ä’Êíí•Ê‚ğ‘‚«‚Ş
	eep_write( EEP_ADRS_DATA_TYPE, &eep_data, 1, ON );
	
}
#endif

/************************************************************************/
/* ŠÖ”     : eep_part_erase											*/
/* ŠÖ”–¼   : EEP•”•ªÁ‹(‘ª’èƒf[ƒ^ˆÈŠO)								*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.09.13 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : EEP•”•ªÁ‹(‘ª’èƒf[ƒ^ˆÈŠO)									*/
/************************************************************************/
/* ’ˆÓ–€ : 															*/
/* ‡@‘ª’èƒf[ƒ^Á‹‚É‚ÍŠÔ‚ª‚©‚©‚éˆ×‚ÉÁ‹‚µ‚È‚¢						*/
/************************************************************************/
STATIC void eep_part_erase( void )
{
	UB eep_data = EEP_DATA_TYPE_NORMAL;
	
	eep_write( EEP_ADRS_TOP_SETTING, (UB*)&s_eep_page0_tbl[0], EEP_SETTING_SIZE, ON );
	memset( &s_unit.frame_num, 0, sizeof(s_unit.frame_num) );
	eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_eep_page0_tbl[0], EEP_ALARM_SIZE, ON );
	memset( &s_unit.alarm, 0, sizeof(s_unit.alarm) );
	
	// Á‹Ï‚İ‚Æ‚µ‚Ä’Êíí•Ê‚ğ‘‚«‚Ş
	eep_write( EEP_ADRS_DATA_TYPE, &eep_data, 1, ON );
}


//================================
//ACLŠÖ˜A
//================================
/************************************************************************/
/* ŠÖ”     : main_acl_init												*/
/* ŠÖ”–¼   : ‰Á‘¬“xƒZƒ“ƒT‰Šú‰»										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.09.13 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ‰Á‘¬“xƒZƒ“ƒT‰Šú‰» ¦‰Šúó‘Ô‚Í’â~‚É‚µ‚Ä‚¨‚­					*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
/************************************************************************/
STATIC void main_acl_init(void)
{
	UB rd_data[2];
	
	wait_ms( 30 );		// ‰Á‘¬“xƒZƒ“ƒT@¦“dŒ¹ON‘Ò‚¿

	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_WHO_AM_I, &rd_data[0], 1 );
	if( rd_data[0] != ACL_REG_RECOGNITION_CODE ){
		err_info( ERR_ID_ACL );
	}
	
	main_acl_stop();
}

/************************************************************************/
/* ŠÖ”     : main_acl_stop												*/
/* ŠÖ”–¼   : ‰Á‘¬“xƒZƒ“ƒT’â~											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.09.11 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ‰Á‘¬“xƒZƒ“ƒT’â~												*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
/************************************************************************/
STATIC void main_acl_stop(void)
{
	UB rd_data[2];
	UB wr_data[2];
	
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	// “®ìƒ‚[ƒhİ’è
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_CTRL_REG1, &rd_data[0], 1 );
	if( rd_data[0] != 0x00 ){
		err_info( ERR_ID_ACL );
	}
}

/************************************************************************/
/* ŠÖ”     : main_acl_stop												*/
/* ŠÖ”–¼   : ‰Á‘¬“xƒZƒ“ƒTƒXƒ^[ƒg										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.09.11 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ‰Á‘¬“xƒZƒ“ƒTƒXƒ^[ƒg											*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
/************************************************************************/
STATIC void main_acl_start(void)
{
	UB wr_data[2];
	
	// “®ìƒ‚[ƒh‰Šú‰»
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	// “®ìƒ‚[ƒhİ’è
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x20;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );

	// “®ìƒ‚[ƒhŠJn
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0xA0;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	
}

#if (FUNC_DEBUG_LOG != ON) || (FUNC_DEBUG_WAVEFORM_LOG != ON)
/************************************************************************/
/* ŠÖ”     : main_acl_stop												*/
/* ŠÖ”–¼   : ‰Á‘¬“xƒZƒ“ƒT“Ço‚µ										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2018.09.11 Axia Soft Design ¼“‡ –«	‰”Åì¬			*/
/************************************************************************/
/* ‹@”\ : ‰Á‘¬“xƒZƒ“ƒTƒX“Ço‚µ											*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
/************************************************************************/
STATIC void main_acl_read(void)
{
	UB rd_data[10];
	
	// INT_SOURCE1		
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_INT_SRC1, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & BIT04 )){
		// ƒf[ƒ^–¢’B
		err_info( ERR_ID_ACL );
		return;
	}
	
	// ƒf[ƒ^æ“¾
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_DATA_XYZ, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
	// INT_REL“Ç‚İo‚µ@¦Š„‚è‚İ—v‹ƒNƒŠƒA
	i2c_read_sub( ACL_DEVICE_ADR, ACL_REG_ADR_INT_REL, &rd_data[0], 1 );
}

/************************************************************************/
/* ŠÖ”     : main_photo_read											*/
/* ŠÖ”–¼   : ƒtƒHƒgƒZƒ“ƒT[“Ço‚µ										*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2019.07.24 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 																*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
/************************************************************************/
STATIC UH main_photo_read(void)
{
	UH photoref_off_val;
	UH photoref_on_val;
	UH ret_photoref_val = 0;
	
	// ƒtƒHƒgƒZƒ“ƒT[’læ“¾
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
/* ŠÖ”     : reset_vib_timer											*/
/* ŠÖ”–¼   : ƒoƒCƒuƒŒ[ƒVƒ‡ƒ“ƒ^ƒCƒ}[‰Šú‰»							*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2019.07.24 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 																*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
/************************************************************************/
void reset_vib_timer(void)
{
	s_unit.tick_vib_10ms_sec = 0;
}

/************************************************************************/
/* ŠÖ”     : reset_led_timer											*/
/* ŠÖ”–¼   : LEDƒ^ƒCƒ}[‰Šú‰»											*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2019.08.02 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 																*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
/************************************************************************/
void reset_led_timer(void)
{
	s_unit.tick_led_20ms_sec = 0;
}

/************************************************************************/
/* ŠÖ”     : main_set_battery											*/
/* ŠÖ”–¼   : “d’rc—Êİ’è												*/
/* ˆø”     : ‚È‚µ														*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2019.07.26 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 																*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
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

#if FUNC_DEBUG_LOG != ON
/************************************************************************/
/* ŠÖ”     : set_yokusei_cnt_time										*/
/* ŠÖ”–¼   : —}§Å‘å˜A‘±ŠÔİ’è										*/
/* ˆø”     : yokusei_max_time											*/
/* –ß‚è’l   : ‚È‚µ														*/
/* •ÏX—š—ğ : 2019.07.26 oneA Š‹Œ´ OˆÀ	‰”Åì¬						*/
/************************************************************************/
/* ‹@”\ : 																*/
/************************************************************************/
/* ’ˆÓ–€ : ‚È‚µ														*/
/************************************************************************/
static void set_yokusei_cnt_time(UB yokusei_max_time)
{
	switch(yokusei_max_time)
	{
	case SET_MAX_YOKUSEI_CONT_5_MIN:
		yokusei_max_cnt = MAX_YOKUSEI_CONT_TIME_5_MIN_CNT;
		break;
	case SET_MAX_YOKUSEI_CONT_10_MIN:
		yokusei_max_cnt = MAX_YOKUSEI_CONT_TIME_10_MIN_CNT;
		break;
	case SET_MAX_YOKUSEI_CONT_NON:
		yokusei_max_cnt = MAX_YOKUSEI_CONT_TIME_NON_CNT;
		break;
	default:
		yokusei_max_cnt = MAX_YOKUSEI_CONT_TIME_10_MIN_CNT;
		break;
	}
}
#endif
