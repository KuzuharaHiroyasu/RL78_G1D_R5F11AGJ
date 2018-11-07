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
	
	s_unit.sec30_cnt++;
	if( s_unit.sec30_cnt >= 30 ){		// 30�b
//	if( s_unit.sec30_cnt >= 3 ){		// �f�o�b�O3�b��
		
		s_unit.sec30_cnt = 0;
		
		if( SYSTEM_MODE_SENSING == s_unit.system_mode){
			//���Z���ʂ�PC���t
			main_cpu_com_snd_pc_log( &s_unit.calc.info.byte[0], CPU_COM_SND_DATA_SIZE_PC_LOG );
		}
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
	main_eep_write();			//EEP��������
	
	return (KE_MSG_CONSUMED);
}

void user_main_init( void )
{
	memset( &s_unit, 0, sizeof(s_unit) );
	
	s_unit.last_time = ke_time();
	
	// ��Ԑݒ�
	ke_state_set(USER_MAIN_ID, 0);
}


STATIC void user_main_mode( void )
{
	// �e�탂�[�h����
	p_user_main_mode_func[s_unit.system_mode]();
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
		// �Z���V���O�Ȃ�
		user_main_req_cyc();
	}else{
		// �Z���V���O����
		if( s_unit.calc_cnt++ >= 10 ){
			s_unit.calc_cnt = 0;
			__no_operation();
			/* ���Z�v�� */
			
		}
	}
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
	__no_operation();
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
					s_cpu_com_rcv_func_tbl[i].func(&s_ds.cpu_com.input.rcv_data[0]);
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
}

STATIC void main_cpu_com_rcv_sensor_res( void )
{
	ke_time_t now_time;
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
}

// ���[�h�ύX
STATIC void main_chg_system_mode( SYSTEM_MODE next_mode )
{
	s_unit.next_system_mode = next_mode;
	s_unit.system_mode = SYSTEM_MODE_MOVE;
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
/* �֐�     : ds_get_cpu_com_rcv_cmd									*/
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

// ============================
// �ȍ~���Z���̏���
// ============================
static int_t main_calc_sekigai(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	calculator_pulse_oximeter_inf(&s_unit.sekigai_val[0]);
	s_unit.sekigai_cnt = 0;
	s_unit.calc.info.dat.spo2_val = get_spo2();
	
	return (KE_MSG_CONSUMED);
}


static int_t main_calc_sekishoku(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	calculator_pulse_oximeter_red(&s_unit.sekishoku_val[0]);
	s_unit.sekishoku_cnt = 0;
	s_unit.calc.info.dat.myaku_val = get_sinpak();
	
	return (KE_MSG_CONSUMED);
}

static int_t main_calc_kokyu(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	UH max;
	int ii;
	
	calculator_apnea(&s_unit.kokyu_val[0]);
	s_unit.kokyu_cnt = 0;
	s_unit.calc.info.dat.state = get_state();
	// ���т�����̏ꍇ�͑傫�����Z�b�g
	if(s_unit.calc.info.dat.state & 0x01){
		max = s_unit.ibiki_val[0];
		for(ii=1;ii<s_unit.ibiki_cnt;++ii){
			if(max < s_unit.ibiki_val[ii]){
				max = s_unit.ibiki_val[ii];
			}
		}
		s_unit.calc.info.dat.ibiki_val_u = (UB)((max & 0xFF00) >> 8);
		s_unit.calc.info.dat.ibiki_val_d = (UB)(max & 0x00FF);
	}
	
	return (KE_MSG_CONSUMED);
}


static int_t main_calc_ibiki(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	
	
	
	return (KE_MSG_CONSUMED);
}

static int_t main_calc_acl(ke_msg_id_t const msgid, void const *param, ke_task_id_t const dest_id, ke_task_id_t const src_id)
{
	B	acc_x, acc_y, acc_z;
	UB now_acl_cnt;
	
	now_acl_cnt = s_unit.acl_cnt;
	
	DEC_MIN_INI(s_unit.acl_cnt, 0, ( MEAS_ACL_CNT_MAX - 1));
	
	acc_x = s_unit.acl_x[s_unit.acl_cnt];
	acc_y = s_unit.acl_y[s_unit.acl_cnt];
	acc_z = s_unit.acl_z[s_unit.acl_cnt];
	
	// �f�[�^�i�[
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
		
	return (KE_MSG_CONSUMED);
}



