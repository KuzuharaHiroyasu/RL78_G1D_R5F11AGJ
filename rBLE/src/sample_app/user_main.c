/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : user_main.c													*/
/* �@�\         : ���[�U�[���C���i�֐�,RAM,ROM��`�j							*/
/* �ύX����		: 2018.01.25 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
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

#include	"header.h"				//���[�U�[��`

#include	"r_vuart_app.h"

// �v���g�^�C�v�錾
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
//STATIC void eep_all_erase( void );		//���g�p�֐�
void main_vuart_set_mode( void );
void main_vuart_rcv_data_frame( void );
void main_vuart_rcv_data_calc( void );
void main_vuart_rcv_data_end( void );
void main_vuart_rcv_date( void );
void main_vuart_rcv_device_set( void );
void main_vuart_rcv_vib_confirm( void );
void main_vuart_rcv_vib_stop( void );
void main_vuart_rcv_power_off( void );

// ACL�A�t�H�g�֘A
STATIC void main_acl_init(void);
STATIC void main_acl_stop(void);
STATIC void main_acl_start(void);
#if (FUNC_DEBUG_LOG != ON) || (FUNC_DEBUG_WAVEFORM_LOG != ON)
STATIC void main_acl_read(void);
STATIC UH main_photo_read(void);
#endif

/********************/
/*     �O���Q��     */
/********************/
//fw update ��G1D�A�b�v�f�[�g�J�n����
extern RBLE_STATUS FW_Update_Receiver_Start( void );

/********************/
/*     �ϐ���`     */
/********************/
/* Status */// �v���b�g�t�H�[��
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

// �������[�h
static bool diagStartFlg = false;

/********************/
/*     �萔��`     */
/********************/
#include	"user_main_tbl.h"		// ���[�U�[�e�[�u�����̒�`

/************************************************************************/
/* �֐�     : app_evt_usr_1												*/
/* �֐���   : ���[�U�[�C�x���g(10ms����)								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.05.13 Axia Soft Design mmura		���ō쐬			*/
/************************************************************************/
/* �@�\ : ���[�U�[�C�x���g(10ms����)									*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
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
/* �֐�     : app_evt_usr_2												*/
/* �֐���   : ���[�U�[�C�x���g(1�b����)									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���[�U�[�C�x���g(1�b����)										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void codeptr app_evt_usr_2(void) 
{
#if FUNC_DEBUG_LOG != ON
	uint8_t *ke_msg;
#endif	
	ke_evt_clear(KE_EVT_USR_2_BIT);
	
#if FUNC_DEBUG_LOG != ON
	// �b�^�C�}�[�J�E���g�_�E��
	DEC_MIN( s_unit.timer_sec ,0 );
	
	// RD8001�b��FG1D�_�E�����[�h_�f�o�b�O�p�o�[�W�������M
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
	
	// G1D�_�E�����[�h_������Ԃ��Ă���A�b�v�f�[�g���J�n���邽�߂̑҂��ɓ���
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
	
	// �t�H�g�Z���T�[
	if(s_unit.photosens_remove_cnt >= PHOTO_SENSOR_REMOVE_CNT)
	{
		// 30���ԊO��Ă����i臒l��������Ă���j
		evt_act(EVENT_REMOVE_TIMEOUT);
	}
	
	// �Z���V���O���ԏ�����m
	if( s_unit.calc_cnt >= EEP_CALC_DATA_NUM )
	{
		// 12���Ԃ𒴂����Ȃ�ҋ@���[�h��
		evt_act( EVENT_COMPLETE );
	}

	// �����x�Z���T�A�t�H�g�Z���T�l���Z(10�b����)
	s_unit.sec10_cnt++;
	if(s_unit.sec10_cnt >= CALC_ACL_PHOTO_WR_CYC){
		s_unit.sec10_cnt -= CALC_ACL_PHOTO_WR_CYC;

		// �����x���Z
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_ACL, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
		
		// �t�H�g�Z���T�l
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_PHOTOREF, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	// �d�r�c�ʎ擾(10������)
	s_unit.sec600_cnt++;
	if(s_unit.sec600_cnt >= BAT_LEVEL_GET_CYC)
	{
		s_unit.sec600_cnt -= BAT_LEVEL_GET_CYC;
		ke_msg = ke_msg_alloc( USER_MAIN_CYC_BATTERY, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}
	
	// �擾�f�[�^�ۑ�(30�b����)
	s_unit.sec30_cnt++;
	if( s_unit.sec30_cnt >= CALC_RESULT_WR_CYC ){		// 30�b
		if(apnea_data_max == true && snore_data_max == true && snore_data_max == true && acl_data_max == true && photo_data_max == true)
		{ // ���ċz����A���т�����A��̌����A�t�H�g�Z���T�̒l���R��(0�`10�A10�`20�A20�`30)�����Ă���Εۑ�
			s_unit.sec30_cnt -= CALC_RESULT_WR_CYC;
			
			ke_msg = ke_msg_alloc( USER_MAIN_CYC_CALC_RESULT, USER_MAIN_ID, USER_MAIN_ID, 0 );
			ke_msg_send(ke_msg);

			// �t���O��߂�
			apnea_data_max = false;
			snore_data_max = false;
			acl_data_max = false;
			photo_data_max = false;
		}
	}
	
#endif
}

/************************************************************************/
/* �֐�     : app_evt_usr_2												*/
/* �֐���   : ���[�U�[�C�x���g(20ms����)								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���[�U�[�C�x���g(20ms����)									*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void codeptr app_evt_usr_3(void) 
{ 
	ke_evt_clear(KE_EVT_USR_3_BIT); 

#if FUNC_DEBUG_LOG != ON
	{
		uint8_t *ke_msg;
		
		// ���[�U�[�A�v����������
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
/* �֐�     : user_main_cyc												*/
/* �֐���   : ���[�U�[�A�v����������									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���[�U�[�A�v����������										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
static int_t user_main_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	sw_proc();				// SW���m����
	
	main_vuart_proc();		// VUART�ʐM�T�[�r�X
	
	user_main_mode();		// ���C�����[�h����

	return (KE_MSG_CONSUMED);
}
#endif

/************************************************************************/
/* �֐�     : user_main_calc_result_cyc									*/
/* �֐���   : ���[�U�[�A�v�����Z���ʎ�������							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���[�U�[�A�v�����Z���ʎ�������								*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
static int_t user_main_calc_result_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	user_main_calc_result();	// ���Z����
	
	return (KE_MSG_CONSUMED);
}



/************************************************************************/
/* �֐�     : user_main_timer_10ms_set									*/
/* �֐���   : 10ms�^�C�}�[�J�E���g�Z�b�g								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : 10ms�^�C�}�[�J�E���g�Z�b�g									*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �@�X���[�v���ł��L���ȃ^�C�}�[										*/
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
/* �֐�     : user_main_timer_cyc										*/
/* �֐���   : �^�C�}�[��������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �^�C�}�[��������												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void user_main_timer_cyc( void )
{	
#if FUNC_DEBUG_LOG != ON
	UB bat;
#else
	char dbg_rcv_data = 0;
	
	if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_50MSEC){
		sw_proc();				// SW���m����
	}
#endif
	if(s_unit.system_mode == SYSTEM_MODE_SENSING)
	{
		// 50ms����
		if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_50MSEC){
#if FUNC_DEBUG_LOG == ON
			char dbg_tx_data[50] = {0};
			int dbg_len;
	
			// �ċz���A���т����擾
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
	// �g�`&���ʊm�F
			user_main_calc_data_set_kyokyu_ibiki();
#else
			user_main_calc_data_set_kyokyu_ibiki();
	//�ʏ�f�o�b�O��
			s_unit.acl_timing+=1;
			if(s_unit.acl_timing >= ACL_TIMING_VAL){
				s_unit.acl_timing = 0;
				// �����x�擾
				main_acl_read();
				// �t�H�g�Z���T�[�l�擾
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
			// �ċz���A���т����擾
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
			
			// 5�b��Ɏ擾
			if(s_unit.acl_timing >= ACL_TIMING_VAL){
				if( acl_photo_sens_read_flg == OFF)
				{
					// �����x�擾
					main_acl_read();
					
					// �t�H�g�Z���T�[�l�擾
					s_unit.meas.info.dat.photoref_val = main_photo_read();
					
					acl_photo_sens_read_flg = ON;
					
					if( bat_check_flg != true )
					{
						set_led( LED_PATT_GREEN_BLINK_SENSING );
					}
				}
				
				// �Z���T�[�l�擾5�b��Ƀ��Z�b�g
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
			
			//�[�d���m
			bat = drv_i_port_bat_chg_detect();
			if(bat == ON)
			{
				//�[�d���Ȃ�ҋ@���[�h��
				evt_act( EVENT_CHG_PORT_ON );
			}
#endif
			s_unit.tick_10ms_new = 0;
		}
	}
	
	// �V���A���f�[�^��M
	if(s_unit.tick_10ms_new >= (uint16_t)PERIOD_10MSEC){
		com_srv_rcv(&dbg_rcv_data, 0);

		if(com_get_read_status() == DRV_CPU_COM_STATUS_RECEIVE_COMP)
		{
			set_serial_command(dbg_rcv_data);
			NO_OPERATION_BREAK_POINT();
		}
	}
			
	// 20ms����
	if(s_unit.tick_10ms >= (uint16_t)PERIOD_20MSEC){
		ke_evt_set(KE_EVT_USR_3_BIT);

		s_unit.tick_10ms -= PERIOD_20MSEC;
	}
	// 1�b���� ���x��̒~�ς͌���
	if( s_unit.tick_10ms_sec >= (uint16_t)PERIOD_1SEC){
		s_unit.tick_10ms_sec -= PERIOD_1SEC;	// �x�ꂪ�~�ς��Ȃ��l�ɏ���
		ke_evt_set(KE_EVT_USR_2_BIT);
	}
}

/************************************************************************/
/* �֐�     : led_cyc													*/
/* �֐���   : 						*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.08.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 							*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
static int_t led_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	led_start(s_unit.tick_led_20ms_sec);
	s_unit.tick_led_20ms_sec++;
	
	return (KE_MSG_CONSUMED);
}

/************************************************************************/
/* �֐�     : vib_cyc													*/
/* �֐���   : 						*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.08.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 							*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void vib_cyc( void )
{
	if(vib_flg == true)
	{
		// �o�C�u���[�V����(10ms����)
		if(s_unit.tick_vib_10ms_sec >= (uint16_t)PERIOD_10MSEC)
		{
	   		vib_start(s_unit.tick_vib_10ms_sec);
		}
	}
}

/************************************************************************/
/* �֐�     : set_vib_flg												*/
/* �֐���   : 						*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.08.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 							*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void set_vib_flg( bool flg )
{
	vib_flg = flg;
}

/************************************************************************/
/* �֐�     : battery_level_cyc											*/
/* �֐���   : 						*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.08.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 							*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
static int_t battery_level_cyc(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	// �d�r�c�ʎ擾(10������)
	main_set_battery();
	
	// �d�r�c�ʌ��m
	if(s_unit.battery_sts == BAT_LEVEL_STS_MIN)
	{
		// �d�r�c�ʂȂ��Ȃ�ҋ@���[�h��
		evt_act( EVENT_DENCH_LOW );
	}
	
	return (KE_MSG_CONSUMED);
}


/************************************************************************/
/* �֐�     : user_main_calc_data_set_kokyu_ibiki						*/
/* �֐���   : ���Z�f�[�^�Z�b�g����(�ċz�E���т�)						*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.07.19 Axia Soft Design �a�c �k��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���Z�f�[�^�Z�b�g����(�ċz�E���т�)							*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
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
	
	// �f�[�^�t���ŉ��Z�ďo
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

	NO_OPERATION_BREAK_POINT();									// �u���C�N�|�C���g�ݒu�p
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
	
	// �f�[�^�t���ŉ��Z�ďo
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

	NO_OPERATION_BREAK_POINT();									// �u���C�N�|�C���g�ݒu�p
}
#endif

#if FUNC_DEBUG_LOG == ON
/************************************************************************/
/* �֐�     : make_send_data											*/
/* �֐���   : ���M�f�[�^�쐬����										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.05.14 Axia Soft Design mmura		���ō쐬			*/
/************************************************************************/
/* �@�\ : �^�C�}�[��������												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void make_send_data(char* pBuff)
{
	// �ċz��, ���т���, �����x(X), �����x(Y), �����x(Z), �t�H�g�Z���T�[�l
	UB tmp;
	UH next;
	UB index = 0;
	
	// �ċz��
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
	
	// ���т���
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
	//�ʏ�f�o�b�O��
	// �����x(X)
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
	
	// �����x(Y)
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
	
	// �����x(Z)
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
	
	// �t�H�g�Z���T�[
	tmp = s_unit.meas.info.dat.photoref_val / 100;
	next = s_unit.meas.info.dat.photoref_val % 100;
	pBuff[index++] = '0' + tmp;
	tmp = next / 10;
	next = next % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;
#else
	// ���т����茋��
	tmp = snore_state / 10;
	next = snore_state % 10;
	pBuff[index++] = '0' + tmp;
	tmp = next % 10;
	pBuff[index++] = '0' + tmp;
	pBuff[index++] = ',';
	
	// ���ċz���茋��
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
/* �֐�     : time_get_elapsed_time										*/
/* �֐���   : �\�t�g�E�F�A�o�ߎ��Ԏ擾����								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ���ݘA���ғ�����											*/
/* �ύX���� : 2012.01.30 Axia Soft Design H.Wada	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* ���ݘA���ғ����Ԃ̎擾���s��											*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
UW time_get_elapsed_time( void )
{
	return s_unit.elapsed_time;
}

/************************************************************************/
/* �֐�     : user_system_init											*/
/* �֐���   : �V�X�e���֘A������										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �V�X�e���֘A������											*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void user_system_init( void )
{
	R_INTC_Create();
	R_IT_Create();
	R_RTC_Create();
	R_WDT_Create();
}

/************************************************************************/
/* �֐�     : user_main_init											*/
/* �֐���   : ���C��������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���C��������													*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void user_main_init( void )
{
	// �~�h��������
#if FUNC_DEBUG_LOG == ON
	com_srv_init();
#endif
	i2c_init();
	eep_init();
	main_acl_init();
	main_acl_start();
	R_ADC_Create();
	
	// ���C���̃f�[�^������
	memset( &s_unit, 0, sizeof(s_unit) );
	
	
	//EEP�ǂݏo��
	user_main_eep_read_pow_on();
	
	s_unit.last_time_sts_req = time_get_elapsed_time();
	
	// ��Ԑݒ�
	ke_state_set(USER_MAIN_ID, 0);
	
	// ���Z������
	calc_snore_init();
	
	s_unit.system_mode = SYSTEM_MODE_IDLE_COM;
	set_ble_state(BLE_STATE_INITIAL);
	set_ble_isconnect(false);
	
#if FUNC_DEBUG_PORT == ON
	//�󂫃|�[�g�ɂ��v���p�ݒ�(�X���[�v�Ȃ�)
    write1_sfr(PM1,  5, PORT_OUTPUT);
//    write1_sfr(PM1,  6, PORT_OUTPUT);		//�g����BLE�������Ȃ��Ȃ�̂ō폜

    write1_sfr(P1, 5, 0);
#endif

}

/************************************************************************/
/* �֐�     : sw_proc													*/
/* �֐���   : SW��������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.26 Axia Soft Design �a�c �k��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* SW��������(20ms����)													*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
STATIC void sw_proc(void)
{
	UB pow_sw;
	
	pow_sw = drv_i_port_read_pow_sw();
	
#if FUNC_DEBUG_LOG == ON
	if( ON == pow_sw ){		// ON����
		// �d��SW�����^�C�}�[�p��
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
	if( ON == pow_sw ){		// ON����
		// �d��SW�����^�C�}�[�p��
		s_unit.sw_time_cnt++;
		
		if( s_unit.sw_time_cnt == TIME_20MS_CNT_POW_SW_LONG){
			// �K�莞�Ԉȏ�A�������Ɣ��f
			evt_act( EVENT_POW_SW_LONG );
		}
	}else{					// OFF����
		if( ON == s_unit.pow_sw_last ){
			// ON��OFF�G�b�W
			if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_LONG){
				// ON�m�莞�ɃC�x���g�����ς݂Ȃ̂ł����ł͉������Ȃ�
			}else if( s_unit.sw_time_cnt >= TIME_20MS_CNT_POW_SW_SHORT){
				evt_act( EVENT_POW_SW_SHORT );
			}else{
				// �������Ȃ�
			}
		}
		// �d��SW�����^�C�}�[�ăX�^�[�g 
		s_unit.sw_time_cnt = 0;
	}
	s_unit.pow_sw_last = pow_sw;
#endif
}

/************************************************************************/
/* �֐�     : user_main_calc_result										*/
/* �֐���   : ���Z���ʏ���												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���Z���ʏ���													*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_calc_result( void )
{
	UW wr_adrs = 0;

	//�͈̓`�F�b�N
	if( s_unit.calc_cnt >= EEP_CALC_DATA_NUM ){
		err_info(ERR_ID_MAIN);
		return;
	}
	
	// �t���[���ʒu�ƃf�[�^�ʒu����EEP�A�h���X���Z�o
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + ( s_unit.calc_cnt * EEP_CALC_DATA_SIZE );

	eep_write( wr_adrs, (UB*)&s_unit.calc, EEP_CALC_DATA_SIZE, OFF );	// 30�b�����Ȃ̂�5ms�҂��͂��Ȃ�
	
	s_unit.calc_cnt++;
	
	/* ���т����m�񐔂Ɩ��ċz���m�񐔂̊m�菈�� */
	s_unit.ibiki_detect_cnt_decided = s_unit.ibiki_detect_cnt;
	s_unit.mukokyu_detect_cnt_decided = s_unit.mukokyu_detect_cnt;
	
	NO_OPERATION_BREAK_POINT();									// �u���C�N�|�C���g�ݒu�p
}

#if FUNC_DEBUG_LOG != ON
/************************************************************************/
/* �֐�     : user_main_mode											*/
/* �֐���   : ���C�����[�h����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���C�����[�h����												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode( void )
{
	// ���[�h���ʏ���
	user_main_mode_common();
	
	// �e�탂�[�h����
	p_user_main_mode_func[s_unit.system_mode]();
}

/************************************************************************/
/* �֐�     : user_main_mode_common										*/
/* �֐���   : ���[�h���ʏ���											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���[�h���ʏ���												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_common( void )
{
	UW now_time;
	
	now_time = time_get_elapsed_time();
	
	// �^�C���A�E�g�`�F�b�N
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
/* �֐�     : user_main_mode_sensing_before								*/
/* �֐���   : �Z���V���O�O����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �Z���V���O�O����												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_sensing_before( void )
{
	UW wr_adrs = 0;
	rtc_counter_value_t rtc_val;
	
	// �������擾
	if( MD_OK != R_RTC_Get_CounterValue( &rtc_val ) ){
		err_info( ERR_ID_MAIN );
	}
	
	// BCD���o�C�i���ϊ�
	bcd2bin(&s_unit.date.year, &rtc_val.year);
	bcd2bin(&s_unit.date.month, &rtc_val.month);
	bcd2bin(&s_unit.date.week, &rtc_val.week);
	bcd2bin(&s_unit.date.day, &rtc_val.day);
	bcd2bin(&s_unit.date.hour, &rtc_val.hour);
	bcd2bin(&s_unit.date.min, &rtc_val.min);
	bcd2bin(&s_unit.date.sec, &rtc_val.sec);
	
	// ������񏑂�����
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
	eep_write( wr_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE, ON );
	
	s_unit.device_set_info = 0;
	// ���샂�[�h�ۑ�
	s_unit.device_set_info = s_unit.alarm.info.dat.act_mode;
	// �o�C�u���[�V�����̋����ۑ�
	s_unit.device_set_info |= s_unit.alarm.info.dat.suppress_power << 2;
	// ���т����o���x�ۑ�
	s_unit.device_set_info |= s_unit.alarm.info.dat.ibiki_sens << 4;
	// ���ċz���o���x�ۑ�
	// ���݌��o���x�̐ݒ�Ȃ�
	
	// �f�o�C�X�ݒ菑������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DEVICE_SET_INFO;
	eep_write( wr_adrs, &s_unit.device_set_info, EEP_DEVICE_SET_INFO_SIZE, ON );
	
	s_unit.suppress_start_time = s_unit.alarm.info.dat.suppress_start_time;
	// �}���J�n�ݒ莞�ԏ�������
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
	
	// �Z���T�[�擾�f�[�^���N���A
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
	
	// �o�C�u���x��������
	vib_level = VIB_LEVEL_1;
	set_vib_level(vib_level);
	
	s_unit.sensing_flg = ON;
	
	// �Z���V���O�ڍs���Ƀo�C�u����
	set_vib(VIB_MODE_SENSING);
	
	/* BLE�𖳌���(�d�͏���ʒጸ�̈�) */
	set_ble_state(BLE_STATE_ON); // BLE���������������킩��Ȃ����߁A�����ŏ�ԍX�V����(�����OFF�������ł���悤�ɂȂ�)
	RBLE_VS_RF_Control( RBLE_VS_RFCNTL_CMD_POWDOWN );
	
	// BLE��LED������(�b��)���{���̓Z���V���O�ڍs��BLE�ؒf�ŏ�������
	led_green_off();
	set_ble_isconnect(false);
	
	vib_start_limit_cnt = 0;
	
	NO_OPERATION_BREAK_POINT();									// �u���C�N�|�C���g�ݒu�p
}

/************************************************************************/
/* �֐�     : user_main_mode_sensing_after								*/
/* �֐���   : �Z���V���O�㏈��											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �Z���V���O�㏈��												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_sensing_after( void )
{
	UB oikosi_flg = OFF;
	UW wr_adrs = 0;
	UB wr_data[3] = {0};
	
	set_led(LED_PATT_GREEN_OFF);
	
	/* BLE�𖳌����L����(API���t�@�����X�}�j���A���̒ʂ�A�d��ON���Reset����) */
	RBLE_VS_RF_Control( RBLE_VS_RFCNTL_CMD_POWUP_DDCON );
	NO_OPERATION_BREAK_POINT();									// �u���C�N�|�C���g�ݒu�p
	
	if( 0 == s_unit.calc_cnt ){
		err_info(ERR_ID_MAIN);
		return;
	}
	
	// ���������͕ۑ����Ȃ�
	if( SENSING_CNT_MIN > s_unit.calc_cnt ){
		return;
	}
	
	// EEP�������ݑ҂� �����������ł�5ms�҂����Ă��Ȃ��̂ōŌ�̃^�C�~���O�ł͌Œ�҂�����
	wait_ms(5);
	
	// ���Z�񐔏�������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_CALC_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.calc_cnt, 2, ON );
	
	// ���т����ԁA���ċz���ԍX�V(30�b�ۑ��������ƂɊm�肵���J�E���g�����g�p����)
	s_unit.ibiki_time = s_unit.ibiki_detect_cnt_decided * SAMPLING_INTERVAL_SEC;
	s_unit.mukokyu_time = s_unit.mukokyu_detect_cnt_decided * SAMPLING_INTERVAL_SEC;
	// �ő喳�ċz���Ԃ͌p���񐔂̍ő�l����v�Z����
	s_unit.max_mukokyu_sec = s_unit.cont_mukokyu_detect_cnt_max * SAMPLING_INTERVAL_SEC;
	
	// ���т����m����������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_chg_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE, ON );
	// ���ċz���m����������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_chg_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE, ON );
	// ���т����ԏ�������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE, ON );
	// ���ċz���ԏ�������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE, ON );
	// �ō����ċz���ԏ�������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE, ON );
	
	// �ǂ��z������
	if(( s_unit.frame_num.write == s_unit.frame_num.read ) &&
	   ( s_unit.frame_num.cnt > 0 )){
		// �ǂ��z����Ă��܂��̂œǂݏo���|�C���^��i�߂�
		oikosi_flg = ON;
	}
	
	// �������ݘg�ԍ���i�߂�
	INC_MAX_INI(s_unit.frame_num.write, (EEP_FRAME_MAX - 1), 0);
	INC_MAX(s_unit.frame_num.cnt, EEP_FRAME_MAX );
	
	
	wr_adrs = EEP_ADRS_TOP_SETTING;
	if( OFF == oikosi_flg ){
		// �������݃|�C���^
		wr_data[0] = s_unit.frame_num.write;
		wr_data[1] = s_unit.frame_num.cnt;
		eep_write( wr_adrs + 1, &wr_data[0], 2, ON );
	}else{
		INC_MAX_INI(s_unit.frame_num.read, (EEP_FRAME_MAX  -1), 0);
		// �������݁A�ǂݏo���|�C���^
		wr_data[0] = s_unit.frame_num.read;
		wr_data[1] = s_unit.frame_num.write;
		wr_data[2] = s_unit.frame_num.cnt;
		eep_write( wr_adrs, &wr_data[0], 3, ON );
	}
}

/************************************************************************/
/* �֐�     : user_main_mode_get_frame_before							*/
/* �֐���   : GET���[�h�t���[���O����									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : GET���[�h�t���[���O����										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB user_main_mode_get_frame_before( void )
{
	UB ret = ON;
	UW rd_adrs = 0;
	UH calc_cnt;
	
	// �f�[�^������
	s_unit.get_mode_calc_cnt = 0;
	
	// �t���[���ʒu�ƃf�[�^�ʒu����EEP�A�h���X���Z�o
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
/* �֐�     : user_main_mode_get_before									*/
/* �֐���   : GET���[�h�O����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : GET���[�h�O����												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_get_before( void )
{
	UB tx[VUART_DATA_SIZE_MAX];
	
	s_unit.frame_num_work = s_unit.frame_num;	//���f���l�������[�N�ɃR�s�[�����[�N�Ŏ��s����
	
	if(( s_unit.frame_num_work.write == s_unit.frame_num_work.read ) && 
	   ( s_unit.frame_num_work.cnt == 0 )){
		//�f�[�^�Ȃ����͊���
		tx[0] = 0xE1;		// END
		main_vuart_send( &tx[0], VUART_SND_LEN_DATA_END );
#if FUNC_DEBUG_FIN_NON == OFF
		s_unit.get_mode_seq = 6;
#else
		// �����ʒm�s�v(���j�A�[�X��)
		s_unit.get_mode_seq = 7;
#endif
		return;
	}
	
	user_main_mode_get_frame_before();
}


/************************************************************************/
/* �֐�     : user_main_mode_get_after									*/
/* �֐���   : GET���[�h�㏈��											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : GET���[�h�㏈��												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_get_after( void )
{
	UW wr_adrs = 0;

	// ���[�N���琳�K�̈�փR�s�[
	s_unit.frame_num = s_unit.frame_num_work;
	
	wr_adrs = EEP_ADRS_TOP_SETTING;
	eep_write( wr_adrs, &s_unit.frame_num.read, 1, ON );
	s_unit.frame_num.cnt = 0;
	eep_write(( wr_adrs + 2 ), &s_unit.frame_num.cnt, 1, ON );
	
	// �V�X�e���߂�
	evt_act( EVENT_COMPLETE );
}

/************************************************************************/
/* �֐�     : user_main_mode_inital										*/
/* �֐���   : �C�j�V������ԏ���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �C�j�V������ԏ���											*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_inital(void)
{
}

/************************************************************************/
/* �֐�     : user_main_mode_idle_com									*/
/* �֐���   : �A�C�h��_�ʐM�ҋ@��ԏ���									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �A�C�h��_�ʐM�ҋ@��ԏ���										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_idle_com(void)
{
}

/************************************************************************/
/* �֐�     : user_main_mode_sensing									*/
/* �֐���   : �Z���V���O��ԏ���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �Z���V���O��ԏ���											*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_sensing(void)
{
}

/************************************************************************/
/* �֐�     : user_main_mode_move										*/
/* �֐���   : �ڍs��ԏ���												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �ڍs��ԏ���													*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_move(void)
{
}

/************************************************************************/
/* �֐�     : user_main_mode_get										*/
/* �֐���   : GET��ԏ���												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : GET��ԏ���													*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void user_main_mode_get(void)
{
	UB tx[VUART_DATA_SIZE_MAX];
	UW rd_adrs;
	CALC calc_eep;				// ���Z��f�[�^
	
	if(s_unit.get_mode_timeout > GET_MODE_TIME_OUT)
	{
		// 5�b�ԑ��M����ԂȂ�^�C���A�E�g
		evt_act( EVENT_TIME_OUT );
		return;
	}
	
	// ���M���̏ꍇ�̓E�F�C�g
	if( ON == s_ds.vuart.input.send_status ){
		// �^�C���A�E�g
		s_unit.get_mode_timeout++;
		return;
	} else {
		s_unit.get_mode_timeout = 0;
	}
	
	if( 0 == s_unit.get_mode_seq ){
		user_main_mode_get_before();
	}else if( 1 == s_unit.get_mode_seq ){
		// �����ǂݏo��
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
		eep_read( rd_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE );
		// ���т����m���ǂݏo��
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
		eep_read( rd_adrs, (UB*)&s_unit.ibiki_chg_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE );
		// ���ċz���m���ǂݏo��
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
		eep_read( rd_adrs, (UB*)&s_unit.mukokyu_chg_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE );
		// ���т����ԓǂݏo��
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE );
		// ���ċz���ԓǂݏo��
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE );
		// �ō����ċz���ԓǂݏo��
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
		eep_read( rd_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE );
		// �f�o�C�X�ݒ���ǂݏo��
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DEVICE_SET_INFO;
		eep_read( rd_adrs, &s_unit.device_set_info, EEP_DEVICE_SET_INFO_SIZE );
		// �}���J�n�ݒ莞�ԓǂݏo��
		rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DEVICE_SET_SUPPRESS_START_TIME;
		eep_read( rd_adrs, &s_unit.suppress_start_time, EEP_DEVICE_SET_SUPPRESS_START_TIME_SIZE );
		
		s_unit.get_mode_seq = 2;
	}else if( 2 == s_unit.get_mode_seq ){
		tx[0] = VUART_CMD_DATA_FRAME;		//�g���(������)
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
			// EEP�ǂݏo��
			// �t���[���ʒu�ƃf�[�^�ʒu����EEP�A�h���X���Z�o
			rd_adrs = ( s_unit.frame_num_work.read * EEP_FRAME_SIZE ) + ( s_unit.get_mode_calc_cnt * EEP_CALC_DATA_SIZE );
			
			eep_read( rd_adrs, (UB*)&calc_eep, EEP_CALC_DATA_SIZE );
			
			// VUART(BLE)���M
			// �X�}�z��IF�ɍ��킹��
			tx[0] = VUART_CMD_DATA_CALC;	// ���Z�f�[�^
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
		//�ǂݏo���g�ԍ��i�߂�
		INC_MAX_INI(s_unit.frame_num_work.read, ( EEP_FRAME_MAX - 1), 0);
		if( s_unit.frame_num_work.write == s_unit.frame_num_work.read ){
			// �I��
			s_unit.get_mode_seq = 5;
		}else{
			//�p��
			tx[0] = VUART_CMD_DATA_NEXT;		// NEXT
			main_vuart_send( &tx[0], VUART_SND_LEN_DATA_NEXT );
			user_main_mode_get_frame_before();
		}
	}else if( 5 == s_unit.get_mode_seq ){
		tx[0] = VUART_CMD_DATA_END;				// END
		main_vuart_send( &tx[0], VUART_SND_LEN_DATA_END );
		
		// �^�C���A�E�g�^�C�}�[������
		s_unit.data_end_timeout = 0;
		
		#if FUNC_DEBUG_FIN_NON == OFF
			s_unit.get_mode_seq = 6;
		#else
			// �����ʒm�s�v(���j�A�[�X��)
			s_unit.get_mode_seq = 7;
		#endif
	}else if( 6 == s_unit.get_mode_seq ){
		s_unit.data_end_timeout++;
		if(s_unit.data_end_timeout >= DATA_END_TIME_OUT)
		{
			// �^�C���A�E�g
			evt_act( EVENT_TIME_OUT );
		}
	}else{
		user_main_mode_get_after();
	}
}

/************************************************************************/
/* �֐�     : user_main_mode_prg_g1d									*/
/* �֐���   : G1D�v���O�����X�V��ԏ���									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : G1D�v���O�����X�V��ԏ���										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
//RD8001�b��FG1D�_�E�����[�h_�����m�F��
STATIC void user_main_mode_prg_g1d(void)
{
}

/************************************************************************/
/* �֐�     : user_main_mode_self_check									*/
/* �֐���   : ���Ȑf�f��ԏ���											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2018.09.10 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : ���Ȑf�f��ԏ���												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
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
		// LED�m�F
		if( diagStartFlg == false )
		{
			// LED�m�F�J�n
			tx[0] = VUART_CMD_DIAG_LED;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_LED );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
			// LED�_��
			led_green_on();
		}else{
			if(s_unit.tick_diag_10ms >= DIAG_LED_TIMER)
			{
				// LED����
				led_green_off();
				// LED�m�F�I��
				tx[0] = VUART_CMD_DIAG_LED;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_LED );
				diagStartFlg = false;
				s_unit.self_check.seq = 1;
			}
		}
	}else if( 1 == s_unit.self_check.seq ){
		// �o�C�u�m�F
		if( diagStartFlg == false )
		{
			// �o�C�u�m�F�J�n
			tx[0] = VUART_CMD_DIAG_VIB;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_VIB );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
			// �o�C�uON
			vib_on();
		}else{
			if(s_unit.tick_diag_10ms >= DIAG_LED_TIMER)
			{
				// �o�C�uOFF
				vib_off();
				// �o�C�u�m�F�I��
				tx[0] = VUART_CMD_DIAG_VIB;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_VIB );
				diagStartFlg = false;
				s_unit.self_check.seq = 2;
			}
		}
	}else if( 2 == s_unit.self_check.seq ){
		// �}�C�N�m�F
		if( diagStartFlg == false )
		{
			// �}�C�N�m�F�J�n
			tx[0] = VUART_CMD_DIAG_MIC;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_MIC );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
		}else{
			if(s_unit.tick_diag_10ms <= DIAG_MIC_TIMER)
			{
				// �ċz���擾�E���M
				adc_ibiki_kokyu( &diag_ibiki_val, &diag_kokyu_val );
				tx[0] = VUART_CMD_DIAG_MIC_VAL;
				tx[1] = ( diag_kokyu_val & 0x00ff );
				tx[2] = (( diag_kokyu_val & 0xff00 ) >> 8 );
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_MIC_VAL );
			}else{
				// �}�C�N�m�F�I��
				tx[0] = VUART_CMD_DIAG_MIC;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_MIC );
				diagStartFlg = false;
				s_unit.self_check.seq = 3;
			}
		}
	}else if( 3 == s_unit.self_check.seq ){
		// �����x�Z���T�[�m�F
		if( diagStartFlg == false )
		{
			// �����x�Z���T�[�m�F�J�n
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
				
				// �����x�f�[�^�擾
				i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_DATA_XYZ, &diag_acl_data[0], 6 );
				diag_acl_x = diag_acl_data[1];
				diag_acl_y = diag_acl_data[3];
				diag_acl_z = diag_acl_data[5];
				
				// INT_REL�ǂݏo���@�����荞�ݗv���N���A
				i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_INT_REL, &diag_acl_data[0], 1 );
				
				// �����x�f�[�^���M
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
				// �����x�Z���T�[�m�F�I��
				tx[0] = VUART_CMD_DIAG_ACL;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_ACL );
				diagStartFlg = false;
				s_unit.self_check.seq = 4;
			}
		}
	}else if( 4 == s_unit.self_check.seq ){
		// �����Z���T�[�m�F
		if( diagStartFlg == false )
		{
			// �����Z���T�[�m�F�J�n
			tx[0] = VUART_CMD_DIAG_PHOTO;
			tx[1] = VUART_DIAG_START;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_PHOTO );
			diagStartFlg = true;
			s_unit.tick_diag_10ms = 0;
		}else{
			if(s_unit.tick_diag_10ms <= DIAG_PHOTO_TIMER)
			{
				// �����Z���T�[�l�擾�E���M
				diag_photoref_val = main_photo_read();
				tx[0] = VUART_CMD_DIAG_PHOTO_VAL;
				tx[1] = ( diag_photoref_val & 0x00ff );
				tx[2] = (( diag_photoref_val & 0xff00 ) >> 8 );
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_PHOTO_VAL );
			}else{
				// �����Z���T�[�m�F�I��
				tx[0] = VUART_CMD_DIAG_PHOTO;
				tx[1] = VUART_DIAG_END;
				main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_PHOTO );
				diagStartFlg = false;
				s_unit.self_check.seq = 5;
			}
		}
	}else if( 5 == s_unit.self_check.seq ){
		// EEPROM�m�F(��������)
		if( diagStartFlg == false )
		{
			tx[0] = VUART_CMD_DIAG_EEPROM;
			tx[1] = VUART_DIAG_START;
			tx[2] = VUART_DATA_RESULT_OK;
			main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_EEPROM );
			diagStartFlg = true;
		}
		// �S0��������
		// EEP�v���O�������[�h
		eep_write( s_unit.self_check.eep_cnt * EEP_ACCESS_ONCE_SIZE, (UB*)&s_eep_page0_tbl, EEP_ACCESS_ONCE_SIZE, ON );
		INC_MAX( s_unit.self_check.eep_cnt, EEP_PAGE_CNT_MAX );
		if( s_unit.self_check.eep_cnt >= EEP_PAGE_CNT_MAX ){
			s_unit.self_check.eep_cnt = 0;
			s_unit.self_check.seq = 6;
		}
	}else if( 6 == s_unit.self_check.seq ){
		// EEPROM�m�F(�ǂݏo��)
		// �t���[���ʒu�ƃf�[�^�ʒu����EEP�A�h���X���Z�o
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
		// �ُ�
		tx[0] = VUART_CMD_DIAG_EEPROM;
		tx[1] = VUART_DIAG_END;
		tx[2] = VUART_DATA_RESULT_NG;
		main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_EEPROM );
		diagStartFlg = false;
		s_unit.self_check.seq = 9;
	}else if( 8 == s_unit.self_check.seq ){
		// ����
		tx[0] = VUART_CMD_DIAG_EEPROM;
		tx[1] = VUART_DIAG_END;
		tx[2] = VUART_DATA_RESULT_OK;
		main_vuart_send( &tx[0], VUART_SND_LEN_DIAG_EEPROM );
		diagStartFlg = false;
		s_unit.self_check.seq = 9;
	}else{
		// ����
		if( ON ==  s_unit.self_check.com_flg ){
			s_unit.self_check.com_flg = OFF;
			evt_act( EVENT_COMPLETE );
		}
	}
}

/************************************************************************/
/* �֐�     : err_info													*/
/* �֐���   : �ُ�ʒm													*/
/* ����     : �ُ�ID(10�i2��)											*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : G1D���ُ̈��H1D�o�R�Ń��O�o��(�f�o�b�O�@�\)					*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �@�f�o�b�O�@�\�ł��B�G���[�o�͏o���Ȃ��\��������܂��B				*/
/* �A�{���O�������ŕs����������鎖������܂��B���R�Ƃ��Ă͐��i�@�\��	*/
/*   CPU�ԒʐM�Ɋ��荞�ވׁB											*/
/************************************************************************/
void err_info( ERR_ID id )
{
#if 0	// CPU�ԒʐM�̍폜�̂��߈�U�폜����(�g�p����ꍇ�͕ʂ�IF��p�ӂ���K�v����)
#if FUNC_DEBUG_LOG == ON
	// ���O�o��
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
		main_cpu_com_snd_pc_log( (UB*)&tx[0], CPU_COM_SND_DATA_SIZE_PC_LOG );		// �f�o�b�O
		NO_OPERATION_BREAK_POINT();									// �u���C�N�|�C���g�ݒu�p
	}
	
#if 0
	while(1){
		// �ُ�ɂ��i�v���[�v
	}
#endif
#endif
#endif	// CPU�ԒʐM�̍폜�̂��߈�U�폜����(�g�p����ꍇ�͕ʂ�IF��p�ӂ���K�v����)

	s_unit.err_cnt++;
	s_unit.last_err_id = id;
}

/************************************************************************/
/* �֐�     : evt_act													*/
/* �֐���   : �C�x���g���s												*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : TRUE	���{												*/
/*          : FALSE	�����{												*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g���s													*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
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
/* �֐�     : evt_non													*/
/* �֐���   : �C�x���g(�Ȃ�)											*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g(�Ȃ�)												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_non( int evt)
{
	return SYSTEM_MODE_NON;
};

/************************************************************************/
/* �֐�     : evt_idle_com												*/
/* �֐���   : �C�x���g(�A�C�h��_�ʐM�ҋ@)								*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g(�A�C�h��_�ʐM�ҋ@)									*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_idle_com( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_COM;
	
	return system_mode;
}

/************************************************************************/
/* �֐�     : evt_idle_com												*/
/* �֐���   : �C�x���g(�A�C�h��_�ʐM�ҋ@_�d�r�c�ʃ`�F�b�N����)			*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g(�A�C�h��_�ʐM�ҋ@_�d�r�c�ʃ`�F�b�N����)				*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
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
/* �֐�     : evt_sensing												*/
/* �֐���   : �C�x���g(�Z���V���O)										*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g(�Z���V���O)											*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_sensing( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SENSING;
	
	return system_mode;
}
#endif

/************************************************************************/
/* �֐�     : evt_sensing_chg											*/
/* �֐���   : �C�x���g(�Z���V���O_�[�d��ԃ`�F�b�N����)					*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g(�Z���V���O_�[�d��ԃ`�F�b�N����)						*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_sensing_chg( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SENSING;
	UB bat = drv_i_port_bat_chg_detect();
	
	//�d�r�c�ʊm�F
	main_set_battery();
	
	if( s_unit.battery_sts == BAT_LEVEL_STS_MIN )
	{
		// �d�r�c�ʂȂ��Ȃ�ڍs���Ȃ�
		system_mode = SYSTEM_MODE_IDLE_COM;
		set_led( LED_PATT_GREEN_BLINK_LOW_BATT );	
	}
	else if( bat == ON ){
		// �[�d���Ȃ�Z���V���O�Ɉڍs���Ȃ�
		system_mode = SYSTEM_MODE_IDLE_COM;
	} else {
		calc_data_init();
	}
	
	return system_mode;
}

/************************************************************************/
/* �֐�     : evt_bat_check												*/
/* �֐���   : �C�x���g(�d�r�c�ʊm�F)									*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2019.08.08  oneA ���� �O�� ���ō쐬						*/
/************************************************************************/
/* �@�\ : �C�x���g(�d�r�c�ʊm�F)										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_bat_check( int evt)
{
	if(get_ble_isconnect() != true)
	{
		main_set_battery();
		// LED����
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
/* �֐�     : evt_get													*/
/* �֐���   : �C�x���g(GET���[�h)										*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g(GET���[�h)											*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_get( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_GET;
	
	return system_mode;
}

/************************************************************************/
/* �֐�     : evt_g1d_prg_denchi										*/
/* �֐���   : �C�x���g(G1D�v���O�����X�V_�d�r�`�F�b�N����)				*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g(G1D�v���O�����X�V_�d�r�`�F�b�N����)					*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
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
/* �֐�     : evt_self_check											*/
/* �֐���   : �C�x���g(���Ȑf�f)										*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.07  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : �C�x���g(���Ȑf�f)											*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_self_check( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_SELF_CHECK;
	
	s_unit.self_check.seq = 0;
	
	return system_mode;
}

/************************************************************************/
/* �֐�     : evt_remove												*/
/* �֐���   : �C�x���g(���������m)										*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.09.24  OneA ���� ���ō쐬							*/
/************************************************************************/
/* �@�\ : �C�x���g(���������m)											*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
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
/* �֐�     : evt_time_out												*/
/* �֐���   : �C�x���g(�^�C���A�E�g)									*/
/* ����     : evt	�C�x���g�ԍ�										*/
/* �߂�l   : �V�X�e�����[�h											*/
/* �ύX���� : 2018.10.08  OneA ���� ���ō쐬							*/
/************************************************************************/
/* �@�\ : �C�x���g(�^�C���A�E�g)										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC SYSTEM_MODE evt_time_out( int evt)
{
	SYSTEM_MODE system_mode = SYSTEM_MODE_IDLE_COM;
	s_ds.vuart.input.send_status = OFF;
	return system_mode;
}

/************************************************************************/
/* �֐�     : main_mode_chg												*/
/* �֐���   : ���[�h�ύX												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.05.19  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : ���[�h�ύX													*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void main_mode_chg( void )
{
	if( SYSTEM_MODE_MOVE != s_unit.system_mode){
		return;
	}
	
	s_unit.system_mode = s_unit.next_system_mode;		// ���[�h�ύX
	
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
		// ������Ԃ��Ă���A�b�v�f�[�g���J�n���邽�߂̑҂��ɓ���
		s_unit.prg_g1d_update_wait_flg = ON;
		s_unit.prg_g1d_update_wait_sec = 2;	// 2�b��
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
/* �֐�     : main_chg_system_mode										*/
/* �֐���   : ���[�h�ύX												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.05.19  Axia Soft Design ���� ���ō쐬				*/
/************************************************************************/
/* �@�\ : ���[�h�ύX													*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode )
{
	s_unit.next_system_mode = next_mode;
	s_unit.system_mode = SYSTEM_MODE_MOVE;
	
	// �ȑO��CPU�ԒʐM�̃��[�h�ύX��҂��ă��[�h�J�ڂ��s���Ă������A
	// CPU�ԒʐM�͂Ȃ��Ȃ����̂ŁA���ڑJ�ڏ������s��
	main_mode_chg();
}

#if FUNC_DEBUG_LOG != ON
/************************************************************************/
/* �֐�     : main_vuart_proc											*/
/* �֐���   : VUART��������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void main_vuart_proc(void)
{
	int i  =0;
	
	if( 0 == s_ds.vuart.input.rcv_len ){
		return;		//��M�Ȃ�
	}
	
	if( SYSTEM_MODE_INITIAL == s_unit.system_mode ){
		// RD8001�b��F�C�j�V�������͎�M�����N���A
		s_ds.vuart.input.rcv_len = 0;
		return;		//��M�Ȃ�
	}
	
	/* �ȍ~��M�f�[�^�L�� */
	for( i = VUART_CMD_TYPE_NONE; i < VUART_CMD_TYPE_MAX; i++){
		if( s_vuart_rcv_func_tbl[i].len == s_ds.vuart.input.rcv_len ){
			/* �f�[�^������v */
			if(( s_vuart_rcv_func_tbl[i].cmd == s_ds.vuart.input.rcv_data[0] ) ||
			   ( s_vuart_rcv_func_tbl[i].cmd == VUART_CMD_INVALID )){
				/* ��M�R�}���h�ƃR�}���h�e�[�u������v�܂��̓R�}���h���� */
				if( NULL != s_vuart_rcv_func_tbl[i].func ){
					/* ��M�����L�� */
					s_vuart_rcv_func_tbl[i].func();
				}
			}
		}
	}

#if 1
	// RD8001�b��FG1D�_�E�����[�h_�����m�F��(�e�X�g�{�[�h�ł̃f�o�b�O����)
	if(( VUART_CMD_LEN_PRG_G1D_START == s_ds.vuart.input.rcv_len  ) && 
	         ( VUART_CMD_PRG_G1D_START == s_ds.vuart.input.rcv_data[0] )){
		FW_Update_Receiver_Start();
	}else if(( VUART_CMD_LEN_PRG_G1D_VER == s_ds.vuart.input.rcv_len  ) && 
	         ( VUART_CMD_PRG_G1D_VER == s_ds.vuart.input.rcv_data[0] )){
		s_unit.prg_g1d_send_ver_flg = ON;
		s_unit.prg_g1d_send_ver_sec = 5;	// 5�b��
	}else{
		// �Y���R�}���h�Ȃ�
		
	}
#endif
	
	// ��M���N���A
	s_ds.vuart.input.rcv_len = 0;
}
#endif

/************************************************************************/
/* �֐�     : main_vuart_send											*/
/* �֐���   : VUART�ʐM�f�[�^�Z�b�g										*/
/* ����     : VUART�ʐM�f�[�^�i�[�|�C���^								*/
/*          : �f�[�^��													*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART�ʐM�f�[�^�Z�b�g												*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
STATIC void main_vuart_send( UB *p_data, UB len )
{
	// Vuart���M����
	if( ON == s_ds.vuart.input.send_status ){
		err_info(ERR_ID_BLE_SEND_ERR);
		return;
	}
	
	s_ds.vuart.input.send_status = ON;
	R_APP_VUART_Send_Char( (char *)p_data, len );
}

/************************************************************************/
/* �֐�     : main_vuart_set_mode										*/
/* �֐���   : VUART�ʐM�f�[�^�Z�b�g										*/
/* ����     : VUART�ʐM�f�[�^�i�[�|�C���^								*/
/*          : �f�[�^��													*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART�ʐM�f�[�^�Z�b�g												*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
void main_vuart_set_mode( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	
	// OK����
	tx[0] = VUART_CMD_SET_CHG;
	tx[1] = 0x00;
	
	main_vuart_send( &tx[0], VUART_SND_LEN_SET_CHG );

	s_unit.calc_cnt = 0;
}


/************************************************************************/
/* �֐�     : main_vuart_rcv_mode_chg									*/
/* �֐���   : VUART�ʐM�f�[�^�Z�b�g										*/
/* ����     : VUART�ʐM�f�[�^�i�[�|�C���^								*/
/*          : �f�[�^��													*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART�ʐM�f�[�^�Z�b�g												*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
STATIC void main_vuart_rcv_mode_chg( void )
{
	UB ret = TRUE;
	UB tx[VUART_DATA_SIZE_MAX] = {0};
	
	if( 3 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_GET_DATA );
		if( TRUE == ret){
			// �f�[�^�擾�^�C���A�E�g������
			s_unit.get_mode_timeout = 0;
		} 
	}else if( 4 == s_ds.vuart.input.rcv_data[1] ){
		main_vuart_set_mode();		// RD8001�b��F�f�o�b�O�p�f�[�^�ݒ�(SET�R�}���h_�ŏI�K�v�H)
	}else if( 5 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_G1D_PRG );
	}else if( 6 == s_ds.vuart.input.rcv_data[1] ){
		ret = evt_act( EVENT_SELF_CHECK_COM );
		if( TRUE == ret){
			s_unit.self_check.com_flg = ON;
		} 
	}else{
		// �������Ȃ�
	}
	
	if( FALSE == ret ){
		// NG����
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = VUART_DATA_RESULT_NG;
		main_vuart_send( &tx[0], VUART_SND_LEN_MODE_CHG );
	}else{
		// OK����
		tx[0] = VUART_CMD_MODE_CHG;
		tx[1] = VUART_DATA_RESULT_OK;
		main_vuart_send( &tx[0], VUART_SND_LEN_MODE_CHG );
	}
}

/************************************************************************/
/* �֐�     : main_vuart_rcv_info										*/
/* �֐���   : VUART�ʐM(���擾)										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART�ʐM(���擾)													*/
/************************************************************************/
/* ���ӎ��� :															*/
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
/* �֐�     : main_vuart_rcv_version									*/
/* �֐���   : VUART��M(�o�[�W����)										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(�o�[�W����)												*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
STATIC void main_vuart_rcv_version( void )
{
	UB tx[VUART_DATA_SIZE_MAX] = {0};

	// OK����
	tx[0] = VUART_CMD_VERSION;
	// G1D�o�[�W����
	tx[1] = VUART_DATA_RESULT_OK;
	tx[2] = version_product_tbl[0];
	tx[3] = version_product_tbl[1];
	tx[4] = version_product_tbl[2];
	tx[5] = version_product_tbl[3];
	main_vuart_send( &tx[0], VUART_SND_LEN_VERSION );
}


/************************************************************************/
/* �֐�     : main_set_bd_adrs											*/
/* �֐���   : BD�A�h���X�ݒ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* BD�A�h���X�ݒ�														*/
/************************************************************************/
/* ���ӎ��� :															*/
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
/* �֐�     : main_vuart_rcv_device_info								*/
/* �֐���   : VUART��M(�f�o�C�X���)									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(�f�o�C�X���)												*/
/************************************************************************/
/* ���ӎ��� :															*/
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
	
	// BCD���o�C�i���ϊ�
	bcd2bin(&rtc_val_bin.year, &rtc_val.year);
	bcd2bin(&rtc_val_bin.month, &rtc_val.month);
	bcd2bin(&rtc_val_bin.week, &rtc_val.week);
	bcd2bin(&rtc_val_bin.day, &rtc_val.day);
	bcd2bin(&rtc_val_bin.hour, &rtc_val.hour);
	bcd2bin(&rtc_val_bin.min, &rtc_val.min);
	bcd2bin(&rtc_val_bin.sec, &rtc_val.sec);
	
	tx[0] = VUART_CMD_DEVICE_INFO;
	tx[1] = result;							// ����
	tx[2] = s_unit.bd_device_adrs[0];		// BD�f�o�C�X�A�h���X
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
/* �֐�     : main_vuart_rcv_data_frame									*/
/* �֐���   : VUART��M(�t���[��)										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(�t���[��)													*/
/************************************************************************/
/* ���ӎ��� :															*/
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
	
	// ������񏑂�����
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_DATE;
	eep_write( wr_adrs, (UB*)&s_unit.date, EEP_DATE_SIZE, ON );
	// ���т����m����������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_chg_detect_cnt, EEP_IBIKI_DETECT_CNT_SIZE, ON );
	// ���ċz���m����������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_DETECT_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_chg_detect_cnt, EEP_MUKOKYU_DETECT_CNT_SIZE, ON );
	// ���т����ԏ�������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_IBIKI_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.ibiki_time, EEP_IBIKI_TIME_SIZE, ON );
	// ���ċz���ԏ�������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.mukokyu_time, EEP_MUKOKYU_TIME_SIZE, ON );
	// �ō����ċz���ԏ�������
	wr_adrs = ( s_unit.frame_num.write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_MAX_MUKOKYU_TIME;
	eep_write( wr_adrs, (UB*)&s_unit.max_mukokyu_sec, EEP_MAX_MUKOKYU_TIME_SIZE, ON );
	
}
/************************************************************************/
/* �֐�     : main_vuart_rcv_data_calc									*/
/* �֐���   : VUART��M(���Z)											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(���Z)														*/
/************************************************************************/
/* ���ӎ��� :															*/
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
/* �֐�     : main_vuart_rcv_data_end									*/
/* �֐���   : VUART��M(END)											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(END)														*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
void main_vuart_rcv_data_end( void )
{
	user_main_mode_sensing_after();
}

/************************************************************************/
/* �֐�     : main_vuart_rcv_data_fin									*/
/* �֐���   : VUART��M(����)											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(����)														*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
STATIC void main_vuart_rcv_data_fin( void )
{
	s_unit.get_mode_seq = 7;
}

/************************************************************************/
/* �֐�     : main_vuart_rcv_date										*/
/* �֐���   : VUART��M(�����ݒ�)										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(�����ݒ�)													*/
/************************************************************************/
/* ���ӎ��� :															*/
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
	
	// �o�C�i����BCD�ϊ�
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
/* �֐�     : main_vuart_rcv_device_set									*/
/* �֐���   : VUART��M(�f�o�C�X�ݒ�)									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.26  Axia Soft Design �a�c �k��	���ō쐬		*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(�f�o�C�X�ݒ�)												*/
/************************************************************************/
/* ���ӎ��� :															*/
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
		// ���샂�[�h�ݒ�
		act_mode = s_unit.alarm.info.dat.act_mode;
		// ���т����x�ݒ�
		set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
		// �}�����x�ݒ�
		vib_power = s_unit.alarm.info.dat.suppress_power;
		// �}������ő�p������
		set_suppress_cnt_time(s_unit.alarm.info.dat.suppress_max_time);
		// �}���J�n�ݒ莞��
		suppress_start_time = s_unit.alarm.info.dat.suppress_start_time;
#endif
	}
}

/************************************************************************/
/* �֐�     : main_vuart_rcv_vib_confirm								*/
/* �֐���   : VUART��M(�o�C�u����m�F)									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.11.18  oneA ���� �O��				���ō쐬		*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(�o�C�u����m�F)											*/
/************************************************************************/
/* ���ӎ��� :															*/
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
		// �o�C�u���x��������
		vib_level = VIB_LEVEL_1;
		set_vib_level(vib_level);
		
		set_vib_confirm(set_vib_mode(vib_power_conf));
#endif
	}
}

/************************************************************************/
/* �֐�     : main_vuart_rcv_vib_stop									*/
/* �֐���   : VUART��M(�o�C�u�����~)									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.03  oneA ���� �O��				���ō쐬		*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(�o�C�u�����~)											*/
/************************************************************************/
/* ���ӎ��� :															*/
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
/* �֐�     : main_vuart_rcv_power_off									*/
/* �֐���   : VUART��M(�d��OFF)										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2020.2.20  oneA ���� �O��				���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* VUART��M(�d��OFF)													*/
/************************************************************************/
/* ���ӎ��� :															*/
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
		// �d��OFF
		
#endif
	}
}

/************************************************************************/
/* �֐�     : ds_set_vuart_data											*/
/* �֐���   : VUART�ʐM�f�[�^�Z�b�g										*/
/* ����     : VUART�ʐM�f�[�^�i�[�|�C���^								*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART�ʐM�~�h���f�[�^�Z�b�g�擾										*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
void ds_set_vuart_data( UB *p_data, UB len )
{
	s_ds.vuart.input.rcv_len = len;
	memcpy( &s_ds.vuart.input.rcv_data[0], &p_data[0], s_ds.vuart.input.rcv_len );
}

/************************************************************************/
/* �֐�     : ds_set_vuart_send_status									*/
/* �֐���   : VUART�ʐM�f�[�^�Z�b�g										*/
/* ����     : VUART�ʐM�f�[�^�i�[�|�C���^								*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* VUART�ʐM�~�h���f�[�^�Z�b�g�擾										*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
void ds_set_vuart_send_status( UB status )
{
	s_ds.vuart.input.send_status = status;
}

// ============================
// �ȍ~���Z���̏���
// ============================
/************************************************************************/
/* �֐�     : main_calc_kokyu											*/
/* �֐���   : ���ċz���Z����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 															*/
/************************************************************************/
/* �@�\ :																*/
/* 																		*/
/************************************************************************/
/* ���ӎ��� :															*/
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
		s_unit.calc.info.dat.state |= (set_kokyu_mask << bit_shift);		// ���ċz���ON
		if(act_mode == ACT_MODE_SUPPRESS_SNORE_APNEA || act_mode == ACT_MODE_SUPPRESS_APNEA)
		{//�}�����[�h�i���т� + ���ċz�j���}�����[�h�i���ċz�j�Ȃ�o�C�u���[�V��������
			if(s_unit.suppress_start_cnt >= (suppress_start_time * 6))
			{// �}���J�n���Ԍo�߁i�Z���V���O�J�n����20���j
				if(vib_power == VIB_SET_MODE_GRADUALLY_STRONGER)
				{
					set_vib_level(vib_level);
					vib_level++;
				}
				set_vib(set_vib_mode(vib_power));
			}
		}
	}else{
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// ���ċz���OFF
		if(act_mode == ACT_MODE_SUPPRESS_APNEA)
		{// �}�����[�h�i���ċz�j�̏ꍇ
			vib_level = VIB_LEVEL_1;
		}
	}
	// �����A���т������ċz���ǂ�����Z�b�g���ꂽ�炢�т���D�悷�邽�߁A���т���ԂƂ���
	if( ((s_unit.calc.info.dat.state >> bit_shift) & 0x03) == 0x03 ){
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// ���ċz���OFF
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// ���т����ON
	}
	
	// ���ċz���m���X�V
	if( ((s_unit.calc.info.dat.state >> bit_shift) & set_kokyu_mask) == set_kokyu_mask ){
		s_unit.mukokyu_detect_cnt++;
		s_unit.cont_mukokyu_detect_cnt_current++;
		
		// ���ċz�������m��
		if(s_unit.mukokyu_state_flg == OFF){
			s_unit.mukokyu_state_flg = ON;
			s_unit.mukokyu_chg_detect_cnt++;
		}
	}else{
		// �p�����ċz���m�̏I������(�ő�l���X�V)
		if(s_unit.cont_mukokyu_detect_cnt_current > s_unit.cont_mukokyu_detect_cnt_max){
			// 5�������͕ۑ�����i�T���ȏ�̓A�v������[�s��]��ԂƂ��邽�߁j(5�� = 30��)
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
//�f�o�b�O�ŉ��Z����
static UB main_calc_kokyu( void)
{
	calculator_apnea(&s_unit.kokyu_val[0], &s_unit.ibiki_val[0]);
	return get_state();
}
#endif

/************************************************************************/
/* �֐�     : main_calc_ibiki											*/
/* �֐���   : ���т����Z����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 															*/
/************************************************************************/
/* �@�\ :																*/
/* 																		*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
#if FUNC_DEBUG_LOG != ON
static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	//���Z���K����
	int ii;
	int average =0;
	UB newstate;
	UB	set_ibiki_mask = 0x01;
	UB	set_kokyu_mask = 0x02;
	UB	bit_shift = 0;

#if 0 // �e�X�g�p�f�[�^
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
	
	// 10�b�Ԃ̕��ϒl
	for(ii=0;ii<s_unit.ibiki_cnt;++ii){
		average += s_unit.ibiki_val[ii];
	}
	average = average / s_unit.ibiki_cnt;
	
	// AD�l10bit�f�[�^��2bit�V�t�g���ď��8bit�݂̂�ۑ�����(����2bit�͌덷�͈͂Ŗ��Ȃ�)
	s_unit.calc.info.dat.ibiki_val[s_unit.phase_ibiki] = (UB)(( average >> 2 ) & 0xff );

	// ���т����Z
	calc_snore_proc(&s_unit.ibiki_val[0]);
	newstate = calc_snore_get();
	
	if(suppress_max_cnt_over_flg == ON)
	{// �}������ő厞�ԃI�[�o�[��
		s_unit.suppress_max_time_interval_cnt++;
		if( SUPPRESS_INTERVAL_CNT <= s_unit.suppress_max_time_interval_cnt )
		{// �}������ő厞�ԃI�[�o�[���̃C���^�[�o������
			suppress_max_cnt_over_flg = OFF;
			s_unit.suppress_max_time_interval_cnt = 0;
			s_unit.suppress_cont_time_cnt = 0;
		}
	}
	
	bit_shift = s_unit.phase_ibiki * 2;
	if(newstate == SNORE_ON){
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// ���т����ON
		s_unit.suppress_cont_time_cnt++;
		if(act_mode == ACT_MODE_SUPPRESS_SNORE_APNEA || act_mode == ACT_MODE_SUPPRESS_SNORE)
		{//�}�����[�h�i���т� + ���ċz�j���}�����[�h�i���т��j�Ȃ�o�C�u���[�V��������
			if(s_unit.suppress_cont_time_cnt <= suppress_max_cnt)
			{//�}������ő厞�Ԉȉ�
				if(suppress_max_cnt_over_flg == OFF)
				{//�}������ő厞�ԃI�[�o�[���ȊO
					if(s_unit.suppress_start_cnt >= (suppress_start_time * 6))
					{// �}���J�n���Ԍo�߁i�Z���V���O�J�n����20���j
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
				//�}������ő厞�ԃI�[�o�[���Ƀt���OON
				suppress_max_cnt_over_flg = ON;
			}
		}
	}else{
		s_unit.calc.info.dat.state &= ~(set_ibiki_mask << bit_shift);		// ���т����OFF
		s_unit.suppress_cont_time_cnt = 0;	// ������
		if(vib_power == VIB_SET_MODE_GRADUALLY_STRONGER)
		{
			if(act_mode == ACT_MODE_SUPPRESS_SNORE_APNEA)
			{// �}�����[�h�i���т� + ���ċz�j�̏ꍇ
				if( ((s_unit.calc.info.dat.state >> bit_shift) & set_kokyu_mask) == 0x00 )
				{// ���ċz����`�F�b�N
					// ���ċz���������Ă��Ȃ��ꍇ�o�C�u���x����������
					vib_level = VIB_LEVEL_1;
				}
			} else if(act_mode == ACT_MODE_SUPPRESS_SNORE)
			{// �}�����[�h�i���т��j�̏ꍇ
				vib_level = VIB_LEVEL_1;
			}
		}
	}
	// �����A���т������ċz���ǂ�����Z�b�g���ꂽ�炢�т���D�悷�邽�߁A���т���ԂƂ���
	if( ((s_unit.calc.info.dat.state >> bit_shift) & 0x03) == 0x03 ){
		s_unit.calc.info.dat.state &= ~(set_kokyu_mask << bit_shift);		// ���ċz���OFF
		s_unit.calc.info.dat.state |= (set_ibiki_mask << bit_shift);		// ���т����ON
	}
	
	// ���т����m���X�V
	if( ((s_unit.calc.info.dat.state >> bit_shift) & set_ibiki_mask) == set_ibiki_mask ){
		s_unit.ibiki_detect_cnt++;
		
		// ���т��������m��
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
	// �ړ��݌v�Ƃ�̂őO�̃f�[�^���c��
	for(ii=0;ii<size;++ii){
		s_unit.ibiki_val[ii] = s_unit.ibiki_val[DATA_SIZE-size+ii];
	}
	s_unit.ibiki_cnt = size;
#else
	s_unit.ibiki_cnt = 0;
#endif
	
#else
	//�f�o�b�O�p�_�~�[����
	NO_OPERATION_BREAK_POINT();									// �u���C�N�|�C���g�ݒu�p

	// �_�~�[�f�[�^
	s_unit.ibiki_cnt = 0;
	s_unit.calc.info.dat.spo2_val = (UB)s_unit.ibiki_val[0];
#endif

	return (KE_MSG_CONSUMED);
}
#else
static UB main_calc_ibiki( void)
{
	// ���т����Z
	calc_snore_proc(&s_unit.ibiki_val[0]);
	return calc_snore_get();
}
#endif

/************************************************************************/
/* �֐�     : main_calc_acl												*/
/* �֐���   : �����x���Z����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 															*/
/************************************************************************/
/* �@�\ :																*/
/* 																		*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	UB	body_direct = BODY_DIRECTION_LEFT;
	UB	clear_mask = BODY_DIRECTION_MASK;
	UB	bit_shift = 0;

	// �̂̌�������
	if( 0 <= s_unit.meas.info.dat.acl_x )
	{// �� or �E
		if( 0 <= s_unit.meas.info.dat.acl_z )
		{// ��
			body_direct = BODY_DIRECTION_UP;
		} else {
		 // �E
			body_direct = BODY_DIRECTION_RIGHT;
		}
	} else {
	// �� or ��
		if( 0 <= s_unit.meas.info.dat.acl_z )
		{// ��
			body_direct = BODY_DIRECTION_LEFT;
		} else {
		 // ��
			body_direct = BODY_DIRECTION_DOWN;
		}
	}
	
	// 10�b���Ƃ̔���l��bit�Őݒ肷��
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
/* �֐�     : main_calc_photoref										*/
/* �֐���   : �t�H�g�Z���T�ۑ�����										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 															*/
/************************************************************************/
/* �@�\ :																*/
/* 																		*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
static int_t main_calc_photoref(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	// AD�l10bit�f�[�^��2bit�V�t�g���ď��8bit�݂̂�ۑ�����(����2bit�͌덷�͈͂Ŗ��Ȃ�)
	s_unit.calc.info.dat.photoref[s_unit.phase_photoref] = (UB)(( s_unit.meas.info.dat.photoref_val >> 2 ) & 0xff );

	s_unit.phase_photoref++;
	if(s_unit.phase_photoref >= SEC_PHASE_NUM){
		s_unit.phase_photoref = SEC_PHASE_0_10;
#if FUNC_DEBUG_LOG != ON
		photo_data_max = true;
#endif
	}
	
	// ��������
	if(s_unit.meas.info.dat.photoref_val <= PHOTO_SENSOR_WEARING_AD)
	{
		if(s_unit.photosens_remove_cnt == 0)
		{
			// ����͂��߂̃f�[�^��ۑ�
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
		s_unit.photosens_remove_cnt = 0; // �r�؂ꂽ�烊�Z�b�g
	}
	
	return (KE_MSG_CONSUMED);
}

/************************************************************************/
/* �֐�     : user_main_sleep											*/
/* �֐���   : ���[�U�[�A�v���̃X���[�v�`�F�b�N							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : true		sleep�L��										*/
/*          : false		sleep����										*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* ���[�U�[�A�v���̃X���[�v�`�F�b�N										*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
bool user_main_sleep(void)
{
#if FUNC_DEBUG_SLEEP_NON == ON
	return false;		//SLEEP����
#else
	bool ret = true;
	
	if( ret == true ){
		NO_OPERATION_BREAK_POINT();									// �u���C�N�|�C���g�ݒu�p
	}
	
	return ret;
#endif

}

/************************************************************************/
/* �֐�     : user_main_eep_read_pow_on									*/
/* �֐���   : EEP�ǂݏo������(�N����)									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.16  Axia Soft Design ����	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/* EEP�ǂݏo������(�N����)												*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
STATIC void user_main_eep_read_pow_on(void)
{
	UB eep_type;
	
	// EEP��ʃ`�F�b�N
	eep_read( EEP_ADRS_DATA_TYPE, &eep_type, 1 );
	
	if( EEP_DATA_TYPE_NORMAL != eep_type){
		eep_part_erase();
	}
	
	// �t���[���֘A
	eep_read( EEP_ADRS_TOP_SETTING, &s_unit.frame_num.read, 1 );
	eep_read( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num.write, 1 );
	eep_read( EEP_ADRS_TOP_SETTING + 2, &s_unit.frame_num.cnt, 1 );
	
	// �͈̓`�F�b�N
	if(( s_unit.frame_num.read > ( EEP_FRAME_MAX - 1)) ||
	   ( s_unit.frame_num.write > ( EEP_FRAME_MAX - 1)) ||
	   ( s_unit.frame_num.cnt > EEP_FRAME_MAX )){
		err_info(ERR_ID_MAIN);
		// �͈͊O�Ȃ珉����
		s_unit.frame_num.read = 0;
		s_unit.frame_num.write = 0;
		s_unit.frame_num.cnt = 0;
		eep_write( EEP_ADRS_TOP_SETTING, &s_unit.frame_num.read, 1, ON );
		eep_write( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num.write, 1, ON );
		eep_write( EEP_ADRS_TOP_SETTING + 2, &s_unit.frame_num.cnt, 1, ON );
	}
	
	// �x���@�\
	eep_read( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE );

	// �ݒ蔽�f
	// ���샂�[�h�ݒ�
	act_mode = s_unit.alarm.info.dat.act_mode;
	// ���т����x�ݒ�
	set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
	// �}�����x�ݒ�
	vib_power = s_unit.alarm.info.dat.suppress_power;
	// �}������ő�p������
	set_suppress_cnt_time(s_unit.alarm.info.dat.suppress_max_time);
	// �}���J�n�ݒ莞��
	suppress_start_time = s_unit.alarm.info.dat.suppress_start_time;
	
	// RD8001�b�F�͈̓`�F�b�N�����
}

/************************************************************************/
/* �֐�     : eep_all_erase												*/
/* �֐���   : EEP�S����													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : EEP�S����														*/
/************************************************************************/
/* ���ӎ��� : 															*/
/* �@���Ԃ�������ׂɎg�p����ۂ͒����鎖�B��7.5Sec						*/
/************************************************************************/
#if 0		//���g�p�֐�
STATIC void eep_all_erase( void )
{
	UW adrs = 0;
	UW i = 0;
	UB eep_data = EEP_DATA_TYPE_NORMAL;
	
	for( i = 0; i < (EEP_DATA_SIZE_ALL / EEP_ACCESS_ONCE_SIZE); i++ ){
		adrs = i * EEP_ACCESS_ONCE_SIZE;
		eep_write( adrs, (UB*)&s_eep_page0_tbl[0], EEP_ACCESS_ONCE_SIZE, ON );
	}
	
	// �����ς݂Ƃ��Ēʏ��ʂ���������
	eep_write( EEP_ADRS_DATA_TYPE, &eep_data, 1, ON );
	
}
#endif

/************************************************************************/
/* �֐�     : eep_part_erase											*/
/* �֐���   : EEP��������(����f�[�^�ȊO)								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.13 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : EEP��������(����f�[�^�ȊO)									*/
/************************************************************************/
/* ���ӎ��� : 															*/
/* �@����f�[�^�����ɂ͎��Ԃ�������ׂɏ������Ȃ�						*/
/************************************************************************/
STATIC void eep_part_erase( void )
{
	UB eep_data = EEP_DATA_TYPE_NORMAL;
	
	eep_write( EEP_ADRS_TOP_SETTING, (UB*)&s_eep_page0_tbl[0], EEP_SETTING_SIZE, ON );
	memset( &s_unit.frame_num, 0, sizeof(s_unit.frame_num) );
	eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_eep_page0_tbl[0], EEP_ALARM_SIZE, ON );
	memset( &s_unit.alarm, 0, sizeof(s_unit.alarm) );
	
	// �����ς݂Ƃ��Ēʏ��ʂ���������
	eep_write( EEP_ADRS_DATA_TYPE, &eep_data, 1, ON );
}


//================================
//ACL�֘A
//================================
/************************************************************************/
/* �֐�     : main_acl_init												*/
/* �֐���   : �����x�Z���T������										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.13 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����x�Z���T������ ��������Ԃ͒�~�ɂ��Ă���					*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_acl_init(void)
{
	UB rd_data[2];
	
	wait_ms( 30 );		// �����x�Z���T�@���d��ON�҂�

	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_WHO_AM_I, &rd_data[0], 1 );
	if( rd_data[0] != ACL_REG_RECOGNITION_CODE ){
		err_info( ERR_ID_ACL );
	}
	
	main_acl_stop();
}

/************************************************************************/
/* �֐�     : main_acl_stop												*/
/* �֐���   : �����x�Z���T��~											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����x�Z���T��~												*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_acl_stop(void)
{
	UB rd_data[2];
	UB wr_data[2];
	
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	// ���샂�[�h�ݒ�
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_CTRL_REG1, &rd_data[0], 1 );
	if( rd_data[0] != 0x00 ){
		err_info( ERR_ID_ACL );
	}
}

/************************************************************************/
/* �֐�     : main_acl_start											*/
/* �֐���   : �����x�Z���T�X�^�[�g										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����x�Z���T�X�^�[�g											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_acl_start(void)
{
	UB wr_data[2];
	
	// ���샂�[�h������
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x00;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	// ���샂�[�h�ݒ�
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0x20;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );

	// ���샂�[�h�J�n
	wr_data[0] = ACL_REG_ADR_CTRL_REG1;
	wr_data[1] = 0xA0;
	i2c_write_sub( ACL_DEVICE_ADR, &wr_data[0], 2, OFF );
	
	
}

#if (FUNC_DEBUG_LOG != ON) || (FUNC_DEBUG_WAVEFORM_LOG != ON)
/************************************************************************/
/* �֐�     : main_acl_read												*/
/* �֐���   : �����x�Z���T�Ǐo��										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.11 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ : �����x�Z���T�X�Ǐo��											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void main_acl_read(void)
{
	UB rd_data[10];
	
	// INT_SOURCE1		
	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_INT_SRC1, &rd_data[0], 1 );
	if( 0 == ( rd_data[0] & BIT04 )){
		// �f�[�^���B
		err_info( ERR_ID_ACL );
		return;
	}
	
	// �f�[�^�擾
	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_DATA_XYZ, &rd_data[0], 6 );
	s_unit.meas.info.dat.acl_x = rd_data[1];
	s_unit.meas.info.dat.acl_y = rd_data[3];
	s_unit.meas.info.dat.acl_z = rd_data[5];
	
	// INT_REL�ǂݏo���@�����荞�ݗv���N���A
	i2c_read_sub_for_acl( ACL_DEVICE_ADR, ACL_REG_ADR_INT_REL, &rd_data[0], 1 );
}

/************************************************************************/
/* �֐�     : main_photo_read											*/
/* �֐���   : �t�H�g�Z���T�[�Ǐo��										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC UH main_photo_read(void)
{
	UH photoref_off_val;
	UH photoref_on_val;
	UH ret_photoref_val = 0;
	
	// �t�H�g�Z���T�[�l�擾
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
/* �֐�     : reset_vib_timer											*/
/* �֐���   : �o�C�u���[�V�����^�C�}�[������							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void reset_vib_timer(void)
{
	s_unit.tick_vib_10ms_sec = 0;
}

/************************************************************************/
/* �֐�     : reset_led_timer											*/
/* �֐���   : LED�^�C�}�[������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.08.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void reset_led_timer(void)
{
	s_unit.tick_led_20ms_sec = 0;
}

/************************************************************************/
/* �֐�     : reset_bat_checkflg										*/
/* �֐���   : �d�r�`�F�b�N�t���OOFF										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2020.01.28 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void reset_bat_checkflg(void)
{
#if FUNC_DEBUG_LOG != ON
	bat_check_flg = false;
#endif
}

/************************************************************************/
/* �֐�     : main_set_battery											*/
/* �֐���   : �d�r�c�ʐݒ�												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.26 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
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
/* �֐�     : set_suppress_cnt_time										*/
/* �֐���   : �}���ő�A�����Ԑݒ�										*/
/* ����     : suppress_max_time											*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.26 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
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
/* �֐�     : set_ble_state												*/
/* �֐���   : BLE�Ǘ���Ԑݒ�											*/
/* ����     : state														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.09.18 Axia Soft Design �a�c �k��	���ō쐬			*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void set_ble_state(UB state)
{
	if( (state != BLE_STATE_ON)&&(state != BLE_STATE_OFF)&&(state != BLE_STATE_INITIAL)){
		return;
	}
	s_unit.ble_state = state;
}

/************************************************************************/
/* �֐�     : get_ble_state												*/
/* �֐���   : BLE�Ǘ���Ԏ擾											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : BLE�Ǘ����												*/
/* �ύX����	: 2019.09.18 Axia Soft Design �a�c �k��	���ō쐬			*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
UB get_ble_state(void)
{
	return s_unit.ble_state;
}

/************************************************************************/
/* �֐�     : set_ble_isconnect											*/
/* �֐���   : BLE�ڑ���Ԑݒ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.12.03 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void set_ble_isconnect(bool connect)
{
	s_unit.ble_isconnect = connect;
}

/************************************************************************/
/* �֐�     : get_ble_isconnect											*/
/* �֐���   : BLE�ڑ���Ԏ擾											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2019.12.03 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
bool get_ble_isconnect(void)
{
	return s_unit.ble_isconnect;
}

/************************************************************************/
/* �֐�     : set_serial_command										*/
/* �֐���   : �V���A���R�}���h�ݒ�										*/
/* ����     : dbg_rx_data:��M�f�[�^�i�R�}���h�j						*/
/* �߂�l   : �Ȃ�														*/
/* �ύX����	: 2020.01.06 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void set_serial_command(char dbg_rx_data)
{
	switch(dbg_rx_data)
	{
		case RCV_COM_MODE_MONITORING:			// ���j�^�����O���[�h
			s_unit.alarm.info.dat.act_mode = ACT_MODE_MONITOR;
			act_mode = s_unit.alarm.info.dat.act_mode;
			break;
		case RCV_COM_MODE_SUPPRESS_SNORE:		// �}�����[�h�i���т��j
			s_unit.alarm.info.dat.act_mode = ACT_MODE_SUPPRESS_SNORE;
			act_mode = s_unit.alarm.info.dat.act_mode;
			break;
		case RCV_COM_MODE_SUPPRESS_APNEA:		// �}�����[�h�i���ċz�j
			s_unit.alarm.info.dat.act_mode = ACT_MODE_SUPPRESS_APNEA;
			act_mode = s_unit.alarm.info.dat.act_mode;
			break;
		case RCV_COM_MODE_SUPPRESS_SNORE_APNEA:	// �}�����[�h�i���т��{���ċz�j
			s_unit.alarm.info.dat.act_mode = ACT_MODE_SUPPRESS_SNORE_APNEA;
			act_mode = s_unit.alarm.info.dat.act_mode;
			break;
		case RCV_COM_SNORE_SENS_WEAK: 			// ���т����o���x�i��j
			s_unit.alarm.info.dat.ibiki_sens = 0;
			set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
			break;
		case RCV_COM_SNORE_SENS_MED:			// ���т����o���x�i���j
			s_unit.alarm.info.dat.ibiki_sens = 1;
			set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
			break;
		case RCV_COM_SNORE_SENS_STRONG: 		// ���т����o���x�i���j
			s_unit.alarm.info.dat.ibiki_sens = 2;
			set_snore_sens(s_unit.alarm.info.dat.ibiki_sens);
			break;
		case RCV_COM_VIB_WEAK:					// �o�C�u�̋���(��)
			s_unit.alarm.info.dat.suppress_power = VIB_SET_MODE_WEAK;
			vib_power = s_unit.alarm.info.dat.suppress_power;
			break;
		case RCV_COM_VIB_MED:					// �o�C�u�̋���(��)
			s_unit.alarm.info.dat.suppress_power = VIB_SET_MODE_DURING;
			vib_power = s_unit.alarm.info.dat.suppress_power;
			break;
		case RCV_COM_VIB_STRONG:				// �o�C�u�̋���(��)
			s_unit.alarm.info.dat.suppress_power = VIB_SET_MODE_STRENGTH;
			vib_power = s_unit.alarm.info.dat.suppress_power;
			break;
		case RCV_COM_VIB_GRAD:					// �o�C�u�̋���(���X�ɋ���)
			s_unit.alarm.info.dat.suppress_power = VIB_SET_MODE_GRADUALLY_STRONGER;
			vib_power = s_unit.alarm.info.dat.suppress_power;
			break;
		case RCV_COM_VIB_GRAD_LEVELUP:			// �o�C�u�̋���(���X�ɋ���)�̃��x���A�b�v
			vib_level++;
			set_vib_level(vib_level);
			break;
		case RCV_COM_VIB_GRAD_LEVEL_INIT:		// �o�C�u�̋���(���X�ɋ���)�̃��x����������
			vib_level = VIB_LEVEL_1;
			set_vib_level(vib_level);
			break;
		case RCV_COM_VIB_WEAK_CONF:				// �o�C�u�̋���(��)[�m�F]
			set_vib_confirm(VIB_MODE_WEAK);
			break;
		case RCV_COM_VIB_MED_CONF:				// �o�C�u�̋���(��)[�m�F]
			set_vib_confirm(VIB_MODE_DURING);
			break;
		case RCV_COM_VIB_STRONG_CONF:			// �o�C�u�̋���(��)[�m�F]
			set_vib_confirm(VIB_MODE_STRENGTH);
			break;
		case RCV_COM_VIB_GRAD_CONF:				// �o�C�u�̋���(���X�ɋ���)[�m�F]
			set_vib_confirm(VIB_MODE_GRADUALLY_STRONGER);
			break;
		case RCV_COM_SNORE_SUPPRESS_TIME_FIVE:	// ���т��}���̘A�����ԁi5���j
			break;
		case RCV_COM_SNORE_SUPPRESS_TIME_TEN:	// ���т��}���̘A�����ԁi10���j
			break;
		case RCV_COM_SNORE_SUPPRESS_TIME_NON:	// ���т��}���̘A�����ԁi�ݒ肵�Ȃ��j
			break;
		case RCV_COM_SUPPRESS_START_TIME:		// �o�C�u�}���J�n���ԕύX
			break;
		case RCV_COM_SUPPRESS_START:			// �o�C�u����
			set_vib(set_vib_mode(vib_power));
			break;

		default:
			break;
	}
	// ROM�ɕۑ�
	eep_write( EEP_ADRS_TOP_ALARM, (UB*)&s_unit.alarm, EEP_ALARM_SIZE, ON );
}

