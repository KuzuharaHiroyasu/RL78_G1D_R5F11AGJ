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

#include	"header.h"				//���[�U�[��`

#include	"r_vuart_app.h"

// RD8001�b��FOS�֘A�̖��O�͎b��Ȃ̂Ō�ŏC���\��





// �v���g�^�C�v�錾
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

// �ȍ~���Z���̏���
static int_t main_calc_sekigai(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_sekishoku(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);
static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id);





#include	"user_main_tbl.h"		// ���[�U�[�e�[�u�����Ԓ�`


extern void test_cpu_com_send( void );



//�ϐ���`
/* Status */
ke_state_t cpu_com_state[ CPU_COM_IDX_MAX ] = {0};


//STATIC T_UNIT s_unit;
T_UNIT s_unit;					//RD8001�b��Fstatic�֕ύX�\��
STATIC DS s_ds;



// OS�֘A
// �C�x���g��͏���(OS)�@���ʐM�o�b�t�@���g�p���Ă���

// 1�b����������
void codeptr app_evt_usr_2(void) 
{ 
	uint8_t *ke_msg;
	
	if( SYSTEM_MODE_SENSING != s_unit.system_mode ){
		ke_evt_clear(KE_EVT_USR_2_BIT);
		return;
	}


	s_unit.sec30_cnt++;
	if( s_unit.sec30_cnt >= 30 ){		// 30�b
//	if( s_unit.sec30_cnt >= 3 ){		// �f�o�b�O3�b��
		
		s_unit.sec30_cnt = 0;
		
		// ���Z����
		user_main_calc_result();
	}
	
	s_unit.sec10_cnt++;
	if( s_unit.sec10_cnt >= 10 ){
		s_unit.sec10_cnt = 0;
		// �ԐF���ԊO�̏���
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_SEKISHOKU, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
		ke_msg = ke_msg_alloc( USER_MAIN_CALC_SEKIGAI, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);


	}
	s_unit.sec7_cnt++;
	if( s_unit.sec7_cnt >= 7 ){
		s_unit.sec7_cnt = 0;

		ke_msg = ke_msg_alloc( USER_MAIN_CALC_KOKYU, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);

		ke_msg = ke_msg_alloc( USER_MAIN_CALC_IBIKI, USER_MAIN_ID, USER_MAIN_ID, 0 );
		ke_msg_send(ke_msg);
	}

	ke_msg = ke_msg_alloc( USER_MAIN_CALC_ACL, USER_MAIN_ID, USER_MAIN_ID, 0 );
	ke_msg_send(ke_msg);

	ke_evt_clear(KE_EVT_USR_2_BIT); 
}

// 50ms��������
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
	// RD8001�Ή��FOS�C�x���g�Ή�(����I�ɌĂ΂Ȃ��l)
	cpu_com_proc();			// �ʐM�T�[�r�X�~�h��
	
	main_vuart_proc();
	
	main_cpu_com_proc();	//�ʐM�T�[�r�X�A�v��
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

void user_main_init( void )
{
	R_PORT_Create();
	R_INTC_Create();
//	R_IT_Create();
//	R_RTC_Create();
	
	memset( &s_unit, 0, sizeof(s_unit) );
	
	//EEP ���[�h
	user_main_eep_read();
	
	s_unit.last_time = ke_time();
	
	// ��Ԑݒ�
	ke_state_set(USER_MAIN_ID, 0);
	
	
	//H1D�����Z�b�g�����^�C�~���O�v����
	drv_o_port_h1d_reset( OFF );
}

STATIC void user_main_calc_result( void )
{
	UW wr_adrs = 0;
#if 0
	//���Z���ʂ�PC���t
	main_cpu_com_snd_pc_log( &s_unit.calc.info.byte[0], CPU_COM_SND_DATA_SIZE_PC_LOG );
#endif
	
	// �t���[���ʒu�ƃf�[�^�ʒu����EEP�A�h���X���Z�o
	wr_adrs = ( s_unit.frame_num_write * EEP_FRAME_SIZE ) + ( s_unit.calc_cnt * EEP_CACL_DATA_SIZE );

	eep_write( wr_adrs, (UB*)&s_unit.calc, EEP_CACL_DATA_SIZE, OFF );
	
	s_unit.calc_cnt++;
	//�͈̓`�F�b�N
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
	// �e�탂�[�h����
	p_user_main_mode_func[s_unit.system_mode]();
}

STATIC void user_main_mode_sensing_before( void )
{
	s_unit.calc_cnt = 0;
	
	

}

STATIC void user_main_mode_sensing_after( void )
{
	UB oikosi_flg = OFF;
	UW wr_adrs = 0;
	UB wr_data[2] = {0};
	
	wait_ms(5);		//RD8001�b��G�҂�
	
	// ���Z�񐔏�������
	wr_adrs = ( s_unit.frame_num_write * EEP_FRAME_SIZE ) + EEP_ADRS_TOP_FRAME_CALC_CNT;
	eep_write( wr_adrs, (UB*)&s_unit.calc_cnt, 2, ON );
	
	
	// �������ݘg�ԍ���i�߂�
	INC_MAX_INI(s_unit.frame_num_write, (EEP_FRAME_MAX - 1), 0);
	
	// �ǂ��z������
	if( s_unit.frame_num_write == s_unit.frame_num_read ){
		// �ǂ��z����Ă��܂��̂œǂݏo���|�C���^��i�߂�
		INC_MAX_INI(s_unit.frame_num_read, (EEP_FRAME_MAX  -1), 0);
		oikosi_flg = ON;
	}
	
	wr_adrs = EEP_ADRS_TOP_SETTING;
	if( OFF == oikosi_flg ){
		// �������݃|�C���^
		eep_write( wr_adrs + 1, &s_unit.frame_num_write, 1, ON );
	}else{
		// �������݁A�ǂݏo���|�C���^
		wr_data[0] = s_unit.frame_num_read;
		wr_data[1] = s_unit.frame_num_write;
		eep_write( wr_adrs, &wr_data[0], 2, ON );
	}
}

STATIC UB user_main_mode_get_before( void )
{
	UB ret = ON;
	UW rd_adrs = 0;
	UH calc_cnt;
	
	s_unit.get_mode_status = 0;
	s_unit.get_mode_calc_cnt = 0;
	
	if( s_unit.frame_num_write == s_unit.frame_num_read ){
		// �ǂݏo���s��
		ret = OFF;
		// RD8001�b��F�f�[�^�Ȃ��ʒm�d�l�������ׂɎb��
		{
			UB tx[10];
			tx[0] = 'S';
			tx[1] = 'T';
			tx[2] = 'A';
			tx[3] = 'R';
			tx[4] = 'T';
			tx[5] = 0xEE;		//RD8001�b��F�f�[�^�Ȃ�
			s_ds.vuart.input.send_status = ON;
			R_APP_VUART_Send_Char( &tx[0], 6 );
		}
		return ret;
	}
	
	// �t���[���ʒu�ƃf�[�^�ʒu����EEP�A�h���X���Z�o
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
	//�X�e�[�^�X�v���ʒm
	user_main_req_cyc();
	
	
	
	
}


STATIC void user_main_mode_sensing(void)
{
	ke_time_t now_time;
	
	now_time = ke_time();
	
	if(( now_time - s_unit.last_sensing_data_rcv ) >= SENSING_END_JUDGE_TIME ){
		// �K��l���ԃZ���V���O�Ȃ�
		user_main_req_cyc();
	}else{
		// �Z���V���O����
	}
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
}


STATIC void user_main_mode_move(void)
{
	// �v�����M
	main_cpu_com_snd_mode_chg();
	
}

STATIC void user_main_mode_get(void)
{
	char tx[10];
	UW wr_adrs;
	UW rd_adrs;
	CALC calc_eep;				// ���Z��f�[�^
	
	
	// ���M���̏ꍇ�̓E�F�C�g
	if( ON == s_ds.vuart.input.send_status ){
		return;
	}
	
	// RD8001�b��
#if 0
	if( s_unit.get_wait_cnt++ < 10 ){
		return;
	}else{
		s_unit.get_wait_cnt = 0;
	}
#endif
	
	if( 0 == s_unit.get_mode_status ){
		tx[0] = 'S';
		tx[1] = 'T';
		tx[2] = 'A';
		tx[3] = 'R';
		tx[4] = 'T';
		tx[5] = s_unit.denchi_sts;
		s_ds.vuart.input.send_status = ON;
		R_APP_VUART_Send_Char( &tx[0], 6 );
		s_unit.get_mode_status = 1;
	}else if( 1 == s_unit.get_mode_status ){
		tx[0] = (UB)( s_unit.date.year & 0x00FF );
		tx[1] = (UB)(( s_unit.date.year & 0xFF00 ) >> 8);		// ���g���G���f�B�A��
		tx[2] = s_unit.date.month;	
		tx[3] = s_unit.date.day;	
		tx[4] = s_unit.date.hour;	
		tx[5] = s_unit.date.min;	
		tx[6] = s_unit.date.sec;	
		s_ds.vuart.input.send_status = ON;
		R_APP_VUART_Send_Char( &tx[0], 7 );
		s_unit.get_mode_status = 2;

	}else if( 2 == s_unit.get_mode_status ){
		if( s_unit.calc_cnt <= s_unit.get_mode_calc_cnt ){
			
			s_unit.get_mode_status = 3;
		}else{
			// EEP�ǂݏo��
			// �t���[���ʒu�ƃf�[�^�ʒu����EEP�A�h���X���Z�o
			rd_adrs = ( s_unit.frame_num_read * EEP_FRAME_SIZE ) + ( s_unit.get_mode_calc_cnt * EEP_CACL_DATA_SIZE );
			
//			i2c_read_sub( EEP_DEVICE_ADR, rd_adrs, &rx[0], EEP_CACL_DATA_SIZE );
			eep_read( rd_adrs, (UB*)&calc_eep, EEP_CACL_DATA_SIZE );
			
			// VUART(BLE)���M
			// �X�}�z��IF�ɍ��킹��
			tx[0] = calc_eep.info.dat.state;
			tx[1] = (( calc_eep.info.dat.ibiki_val & 0xff00 ) >> 8 );
			tx[2] =  ( calc_eep.info.dat.ibiki_val & 0x00ff );
			tx[3] = calc_eep.info.dat.myaku_val;
			tx[4] = calc_eep.info.dat.spo2_val;
			tx[5] = calc_eep.info.dat.kubi;
			tx[6] = 0x00;
			
			s_ds.vuart.input.send_status = ON;
			R_APP_VUART_Send_Char( &tx[0], 7 );
			main_cpu_com_snd_pc_log( (UB*)&tx[0], CPU_COM_SND_DATA_SIZE_PC_LOG );		// �f�o�b�O
			
			s_unit.get_mode_calc_cnt++;
		}
	}else if( 3 == s_unit.get_mode_status ){
		//�ǂݏo���g�ԍ��i�߂�
		INC_MAX_INI(s_unit.frame_num_read, ( EEP_FRAME_MAX - 1), 0);
		if( s_unit.frame_num_write == s_unit.frame_num_read ){
			// �I��
			s_unit.get_mode_status = 4;
		}else{
			//�p��
			tx[0] = 'N';
			tx[1] = 'E';
			tx[2] = 'X';
			tx[3] = 'T';
			s_ds.vuart.input.send_status = ON;
			R_APP_VUART_Send_Char( &tx[0], 4 );
			user_main_mode_get_before();
			s_unit.get_mode_status = 1;
		}
	}else if( 4 == s_unit.get_mode_status ){
		wr_adrs = EEP_ADRS_TOP_SETTING;
		eep_write( wr_adrs, &s_unit.frame_num_read, 1, ON );
		tx[0] = 'E';
		tx[1] = 'N';
		tx[2] = 'D';
		s_ds.vuart.input.send_status = ON;
		R_APP_VUART_Send_Char( &tx[0], 3 );
		
		s_unit.get_mode_status = 5;
	}else{
		user_main_mode_get_after();
	}
}

STATIC void user_main_mode_prg_hd(void)
{
	
	
	
}

STATIC void user_main_req_cyc( void )
{
	ke_time_t now_time;
	
	now_time = ke_time();
	
	// �X�e�[�^�X�v�����M����
	if(( now_time - s_unit.last_time ) >= MAIN_STATUS_REQ_TIME ){
		s_unit.last_time = now_time;
		main_cpu_com_snd_sts_req();	/* �X�e�[�^�X�v�� */
	}
}


// =============================================================
// CPU�ʐM�֘A
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
/* �֐�     : main_cpu_com_proc											*/
/* �֐���   : CPU�ԒʐM��������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.06.05  Axia Soft Design �{�{		���ō쐬			*/
/*          : 2014.06.27  Axia Soft Design �g��							*/
/*          :						��Ԃɉ����đ��M�g���K��؂芷����	*/
/* 			: 2016.05.19  Axia Soft Design �����@���g���C�A�E�g���̏����ǉ�(CPU�ԒʐM�ُ�Ή�) */
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void main_cpu_com_proc(void)
{
	int i = 0;

	/* ��M�f�[�^�`�F�b�N */
	if( 0x00 != s_ds.cpu_com.input.rcv_cmd ){
		//�����f�[�^��M  ���A���C�u
//		s_unit.cpu_com_success_once = ON;
//		time_get_elapsed_time( &s_unit.cpu_com_rcv_int_elapsed_time );
		/* ��M�R�}���h�L�� */
		for( i=CPU_COM_CMD_NONE; i<CPU_COM_CMD_MAX; i++){
			if( s_cpu_com_rcv_func_tbl[i].cmd == s_ds.cpu_com.input.rcv_cmd ){
				/* ��M�R�}���h�ƃR�}���h�e�[�u������v */
				if( NULL != s_cpu_com_rcv_func_tbl[i].func ){
					/* ��M�����L�� */
//					s_cpu_com_rcv_func_tbl[i].func(&s_ds.cpu_com.input.rcv_data[0]);
					s_cpu_com_rcv_func_tbl[i].func();
				}
			}
		}
		// ��M�R�}���h�N���A ���b��
		s_ds.cpu_com.input.rcv_cmd = 0x00;
	}
	
}

STATIC void main_cpu_com_rcv_sts_res( void )
{
	/* �ȍ~�v�� */
	if( SYSTEM_MODE_HD_CHG_SENSING == s_ds.cpu_com.input.rcv_data[0] ){
		// �ȍ~��Ԃ֕ύX
		main_chg_system_mode( SYSTEM_MODE_SENSING );
	}else if( SYSTEM_MODE_HD_CHG_IDLE == s_ds.cpu_com.input.rcv_data[0] ){
		main_chg_system_mode( SYSTEM_MODE_IDLE );
	}else{
		// �������Ȃ�
	}
	
	//�d�r��ԍX�V
	s_unit.denchi_sts = s_ds.cpu_com.input.rcv_data[1];
}

STATIC void main_cpu_com_rcv_sensor_res( void )
{
//	ke_time_t now_time;
	MEAS meas;
	
	// ��M�����i�[
	s_unit.last_sensing_data_rcv = ke_time();
	
	// �Z���T�[�f�[�^�i�[
	memcpy( &meas.info.byte[0], &s_ds.cpu_com.input.rcv_data[0], CPU_COM_SND_DATA_SIZE_SENSOR_DATA );
	
	s_unit.sekigai_val[s_unit.sekigai_cnt] = meas.info.dat.sekigaival;			// �������ׂ͂̈ɕ�������
	s_unit.sekishoku_val[s_unit.sekishoku_cnt] = meas.info.dat.sekishoku_val;	// �������ׂ͂̈ɕ�������
	s_unit.kokyu_val[s_unit.kokyu_cnt] = meas.info.dat.kokyu_val;		
	s_unit.ibiki_val[s_unit.ibiki_cnt] = meas.info.dat.ibiki_val;		
	s_unit.acl_x[s_unit.acl_cnt] = meas.info.dat.acl_x;
	s_unit.acl_y[s_unit.acl_cnt] = meas.info.dat.acl_y;
	s_unit.acl_z[s_unit.acl_cnt] = meas.info.dat.acl_z;

	INC_MAX_INI( s_unit.sekigai_cnt, ( MEAS_SEKIGAI_CNT_MAX - 1 ), 0 );
	INC_MAX_INI( s_unit.sekishoku_cnt, ( MEAS_SEKISHOKU_CNT_MAX - 1 ), 0 );	// �������ׂ͂̈ɕ�������
	INC_MAX_INI( s_unit.kokyu_cnt, ( MEAS_KOKYU_CNT_MAX - 1 ), 0 );		
	INC_MAX_INI( s_unit.ibiki_cnt, ( MEAS_IBIKI_CNT_MAX - 1 ), 0 );		
	INC_MAX_INI( s_unit.acl_cnt, ( MEAS_ACL_CNT_MAX - 1 ), 0 );

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
	
	if( 0 == s_ds.cpu_com.input.rcv_data[0] ){		// ����I��
		s_unit.system_mode = s_unit.next_system_mode;		// ���[�h�ύX
		
	}else{
		s_unit.system_mode = SYSTEM_MODE_IDLE;				// ���[�h�ύX
		s_unit.next_system_mode = SYSTEM_MODE_IDLE;			// ���[�h�ύX
		err_info(5);
	}
	
	if( SYSTEM_MODE_SENSING == s_unit.system_mode ){
		user_main_mode_sensing_before();
	}
	if( SYSTEM_MODE_IDLE == s_unit.system_mode ){
		user_main_mode_sensing_after();
	}
}

// ���[�h�ύX
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode )
{
	s_unit.next_system_mode = next_mode;
	s_unit.system_mode = SYSTEM_MODE_MOVE;
}

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
	if( 0 != s_ds.vuart.input.rcv_len  ){
		if(( 'G' == s_ds.vuart.input.rcv_data[0] ) && 
		   ( 'E' == s_ds.vuart.input.rcv_data[1] ) && 
		   ( 'T' == s_ds.vuart.input.rcv_data[2] )){
			main_cpu_com_rcv_get_mode();
		}
		s_ds.vuart.input.rcv_len = 0;		// ��M���N���A
	}
}

STATIC void main_cpu_com_rcv_get_mode( void )
{
	if( SYSTEM_MODE_IDLE != s_unit.system_mode){
#if 0
		// RD8001�b��F�f�o�b�O
		{
			UB tx[10];
			tx[0] = 'S';
			tx[1] = 'T';
			tx[2] = 'A';
			tx[3] = 'R';
			tx[4] = 'T';
			tx[5] = 0xDD;		//RD8001�b��F�f�[�^�Ȃ�
			s_ds.vuart.input.send_status = ON;
			R_APP_VUART_Send_Char( &tx[0], 6 );
		}
#endif
		return;
	}
	
	s_unit.date.year	= (( s_ds.vuart.input.rcv_data[4] << 8 ) + s_ds.vuart.input.rcv_data[3] );
	s_unit.date.month	= s_ds.vuart.input.rcv_data[5];
	s_unit.date.day		= s_ds.vuart.input.rcv_data[6];
	s_unit.date.hour	= s_ds.vuart.input.rcv_data[7];
	s_unit.date.min		= s_ds.vuart.input.rcv_data[8];
	s_unit.date.sec		= s_ds.vuart.input.rcv_data[9];

	if( OFF == user_main_mode_get_before() ){
		return;		// �f�o�b�O����
	}

	s_unit.system_mode = SYSTEM_MODE_GET;
	s_ds.vuart.input.send_status = OFF;


}


/************************************************************************/
/* �֐�     : ds_get_cpu_com_order										*/
/* �֐���   : CPU�ԒʐM�p�f�[�^�擾										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.15 Axia Soft Design �g�� �v�a	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�ԒʐM�p�f�[�^�擾												*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data )
{
	*p_data = &s_ds.cpu_com.order;
}
/************************************************************************/
/* �֐�     : ds_set_cpu_com_input										*/
/* �֐���   : CPU�ԒʐM�f�[�^�Z�b�g										*/
/* ����     : CPU�ԒʐM�f�[�^�i�[�|�C���^								*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.06.05 Axia Soft Design �{�{ �a��	���ō쐬			*/
/*          : 2014.06.12 Axia Soft Design �g��							*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�ԒʐM�~�h���f�[�^�Z�b�g�擾										*/
/************************************************************************/
/* ���ӎ��� :															*/
/************************************************************************/
void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data )
{
	s_ds.cpu_com.input = *p_data;
}

/************************************************************************/
/* �֐�     : ds_set_vuart_data											*/
/* �֐���   : CPU�ԒʐM�f�[�^�Z�b�g										*/
/* ����     : CPU�ԒʐM�f�[�^�i�[�|�C���^								*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.06.05 Axia Soft Design �{�{ �a��	���ō쐬			*/
/*          : 2014.06.12 Axia Soft Design �g��							*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�ԒʐM�~�h���f�[�^�Z�b�g�擾										*/
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
/* �֐���   : CPU�ԒʐM�f�[�^�Z�b�g										*/
/* ����     : CPU�ԒʐM�f�[�^�i�[�|�C���^								*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.06.05 Axia Soft Design �{�{ �a��	���ō쐬			*/
/*          : 2014.06.12 Axia Soft Design �g��							*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�ԒʐM�~�h���f�[�^�Z�b�g�擾										*/
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
static int_t main_calc_sekigai(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
#if FUNC_DEBUG_CALC_NON == OFF
	calculator_pulse_oximeter_inf(&s_unit.sekigai_val[0]);
	s_unit.sekigai_cnt = 0;
	s_unit.calc.info.dat.spo2_val = get_spo2();
#else
	// �_�~�[�f�[�^
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
	// �_�~�[�f�[�^
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
#if 0
	//���Z���K����



#else
	//�f�o�b�O�p�_�~�[����
	__no_operation();		// RD8001�b��F�u���C�N�\��p
	__no_operation();		// RD8001�b��F�u���C�N�\��p
	__no_operation();		// RD8001�b��F�u���C�N�\��p
	__no_operation();		// RD8001�b��F�u���C�N�\��p
	__no_operation();		// RD8001�b��F�u���C�N�\��p





	// �_�~�[�f�[�^
	s_unit.ibiki_cnt = 0;
	s_unit.calc.info.dat.spo2_val = (UB)s_unit.ibiki_val[0];
#endif

	return (KE_MSG_CONSUMED);
}

static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	B	acc_x;
//	B	acc_y;		//���󖢎g�p
	B	acc_z;
	
	DEC_MIN_INI(s_unit.acl_cnt, 0, ( MEAS_ACL_CNT_MAX - 1));
	
	acc_x = s_unit.acl_x[s_unit.acl_cnt];
//	acc_y = s_unit.acl_y[s_unit.acl_cnt];		//���󖢎g�p
	acc_z = s_unit.acl_z[s_unit.acl_cnt];
	
	// �f�[�^�i�[
#if 0
	// 45������
	if( acc_z > 48 ){
		s_unit.calc.info.dat.kubi = 0;
	}else if( acc_z > 16 ){
		if( acc_x < 0 ){
			s_unit.calc.info.dat.kubi = 1;	// 45��
		}else{
			s_unit.calc.info.dat.kubi = 7;	// 315��
		}
	}else if( acc_z > -16 ){
		if( acc_x < 0 ){
			s_unit.calc.info.dat.kubi = 2;	// 90��
		}else{
			s_unit.calc.info.dat.kubi = 6;	// 270��
		}
	}else if( acc_z > -48 ){
		if( acc_x < 0 ){
			s_unit.calc.info.dat.kubi = 3;	// 180��
		}else{
			s_unit.calc.info.dat.kubi = 5;	// 225��
		}
	}else{
		s_unit.calc.info.dat.kubi = 4;	// 180��
	}
#else
	// 90������
	if( acc_z > 32 ){
		s_unit.calc.info.dat.kubi = 0;		// 0���`45��,316���`360��
	}else if( acc_z > -32 ){
		if( acc_x < 0 ){
			s_unit.calc.info.dat.kubi = 1;	// 46���`135��
		}else{
			s_unit.calc.info.dat.kubi = 2;	// 136���`225��
		}
	}else{
		s_unit.calc.info.dat.kubi = 3;	// 226���`315��
	}
#endif

	
	return (KE_MSG_CONSUMED);
}


// BLE�ȊO�̃��[�U�[�A�v���̃X���[�v�`�F�b�N
bool user_main_sleep(void)
{
#if FUNC_DEBUG_SLEEP_NON == ON
	return false;		//SLEEP����
#else
	bool ret = true;
	
	if( ON == drv_intp_read_h1d_int() ){
		ret = false;
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
	}else{
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
	}
	
	if( ON == cpu_com_get_busy() ){
		ret = false;
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
	}else{
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
	}
	
	if( ret == true ){
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
	}
	
	return ret;
#endif

}


STATIC void user_main_eep_read(void)
{
	eep_read( EEP_ADRS_TOP_SETTING, &s_unit.frame_num_read, 1 );
	eep_read( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num_write, 1 );
	
	// �͈̓`�F�b�N
	if(( s_unit.frame_num_read > ( EEP_FRAME_MAX - 1)) ||
	   ( s_unit.frame_num_write > ( EEP_FRAME_MAX - 1))){
		err_info(11);
		// �͈͊O�Ȃ珉����
		s_unit.frame_num_read = 0;
		s_unit.frame_num_write = 0;
		eep_write( EEP_ADRS_TOP_SETTING, &s_unit.frame_num_read, 1, ON );
		eep_write( EEP_ADRS_TOP_SETTING + 1, &s_unit.frame_num_write, 1, ON );
	}
	
}





