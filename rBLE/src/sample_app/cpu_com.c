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

#include	"header.h"		//���[�U�[��`

#include	"uart.h"
#include	"port.h"


/************************************************************************/
/* �V�X�e����   : RD1402 �p�`���R/�p�`�X���p�Ăяo�������v				*/
/* �t�@�C����   : cpu_com.c												*/
/* �@�\         : CPU�ԒʐM�~�h��										*/
/* �ύX����     : 2014.05.08 Axia Soft Design �g��		���ō쐬		*/
/* ���ӎ���     : �Ȃ�													*/
/************************************************************************/
//RD1508�b��F���������K�v
/********************/
/*     include      */
/********************/
//#include	"header.h"



/********************/
/* �v���g�^�C�v�錾 */
/********************/
STATIC void cpu_com_rcv_proc(void);
STATIC void cpu_com_send_proc(void);
STATIC UB cpu_com_analyze_msg(void);
STATIC void cpu_com_clear_send_order(void);
STATIC void cpu_com_crcset(UB *msg, UH size, UH *crc);
STATIC void cpu_com_rcv_proc(void);
STATIC UB cpu_com_analyze_msg(void);
STATIC UB cpu_com_analyze_msg_check_etx(void);
STATIC UB cpu_com_analyze_msg_check_data(void);
STATIC void cpu_com_send_proc(void);
STATIC UB cpu_com_make_send_data(void);
STATIC void cpu_com_clear_send_order(void);
void drv_cpu_com_set_send_status( UB status );
void drv_cpu_com_get_send_status( UB *status );
void test_cpu_com_send( void );
#if 0		//���g�p
STATIC RING_BUF* drv_uart0_get_snd_ring( void );
#endif
STATIC RING_BUF* drv_uart0_get_rcv_ring( void );
STATIC void cpu_com_send_data( UB* data, UH len );


/********************/
/*     �����ϐ�     */
/********************/
//RD8001�b��F���C���Ƃ̃f�[�^IF����
STATIC DS_CPU_COM_ORDER s_ds_cpu_com_order;
STATIC DS_CPU_COM_INPUT s_cpu_com_ds_input;					/* �f�[�^�Ǘ����̃R�s�[�G���A �A�v���ւ̃f�[�^�󂯓n���p */
STATIC DS_CPU_COM_ORDER *s_p_cpu_com_ds_order;				/* �f�[�^�Ǘ����̃|�C���^ �A�v������̎w���p */

STATIC UB s_cpu_com_snd_cmd;								/* ���M�R�}���h */
STATIC UB s_cpu_com_snd_data[CPU_COM_MSG_SIZE_MAX];			/* ���M���b�Z�[�W */
STATIC UH s_cpu_com_snd_size;								/* ���M���b�Z�[�W�� */
STATIC UB s_cpu_com_snd_type;								/* ���M�R�}���h�^�C�v */
STATIC UB s_cpu_com_snd_retry_cnt;							/* ���M���g���C�� */
STATIC UW s_cpu_com_snd_timeout;							/* ���g���C�^�C���A�E�g���� *10ms */
STATIC UB s_cpu_com_snd_rensou_cnt;							/* ���M�A���� */

STATIC ke_time_t s_cpu_com_snd_last_time;					/* ���M�O�񎞊� */


STATIC UB s_cpu_com_snd_flg;								/* ���M�t���O */

STATIC UB s_cpu_com_snd_seq_no;								/* ���M�V�[�P���XNo */
STATIC UB s_cpu_com_res_seq_no;								/* ��M�V�[�P���XNo */

STATIC UB s_cpu_com_snd_status;								/* CPU�ԒʐM ���M�X�e�[�^�X */
																/*	CPU_COM_SND_STATUS_IDLE			���M�\���		*/
																/* 	CPU_COM_SND_STATUS_RCV_WAIT		������M�҂����	*/
																/* 	CPU_COM_SND_STATUS_RENSOU		�A�������			*/
																/* 	CPU_COM_SND_STATUS_COMPLETE		���M�������		*/
																/* 	CPU_COM_SND_STATUS_SEND_NG		���g���CNG			*/

STATIC CPU_COM_RCV_MSG	s_cpu_com_rcv_msg;					/* ��M���b�Z�[�W */
STATIC UH s_cpu_com_rcv_msg_size;							/* ��M���b�Z�[�W�T�C�Y */


// �h���C�o�R�s�y RD8001�b��F�T�C�Y����
#define	DRV_UART0_DATA_LENGH					100									/* CPU�ԒʐM�f�[�^��(����A����) �w�b�_XXX�o�C�g�{�I�v�V�����f�[�^XXX�o�C�g */
#define	DRV_UART0_RCV_RING_LENGTH				( (DRV_UART0_DATA_LENGH * 5) + 1 )		/* �����O�o�b�t�@��(1���b�Z�[�W���{�P) ���{�P�̗��R�̓����O�o�b�t�@�̒��ӎ����Q�� */
#define	DRV_UART0_SND_RING_LENGTH				( (DRV_UART0_DATA_LENGH * 1) + 1 )		/* �����O�o�b�t�@��(3���b�Z�[�W���{�P) ���{�P�̗��R�̓����O�o�b�t�@�̒��ӎ����Q�� */


#define			SERIAL_DRV_RCV_SIZE		1			//��MDMA�ʐM�T�C�YRD8001�b��F1�o�C�g�ŕ��דI�ɖ��Ȃ���
uint8_t			serial_drv_rcv[SERIAL_DRV_RCV_SIZE];


/********************/
/*     �����ϐ�     */
/********************/
UB drv_uart0_send_buf[DRV_UART0_SND_RING_LENGTH];				/* ���M�o�b�t�@(�����O�p) */
UB drv_uart0_rcv_buf[DRV_UART0_RCV_RING_LENGTH];				/* ��M�o�b�t�@(�����O�p) */
RING_BUF drv_uart0_send_ring;									/* ���M�����O�o�b�t�@�p�R���g���[�� */
RING_BUF drv_uart0_rcv_ring;									/* ��M�����O�o�b�t�@�p�R���g���[�� */


/********************/
/*   �e�[�u����`   */
/********************/
/* �R�}���h�e�[�u�� ���}�X�^�[��p */
/* �^�C�v��ύX���鎖�Ŏ�M��p�ɂ��Ή� */
STATIC const T_CPU_COM_CMD_INFO s_tbl_cmd_info[CPU_COM_CMD_MAX] = {
	/*�R�}���h*/ /*�^�C�v*/					/*���g���C(�����܂�)*//*���g���C�Ԋu *10ms*/ /*�A����*/
	{	0x00,	CPU_COM_CMD_TYPE_ONESHOT_SEND,		0,				0,					0	},	/* �R�}���h����				*/
	{	0xE0,	CPU_COM_CMD_TYPE_RETRY,				3,				5,					0	},	/* �X�e�[�^�X�v��			*/
	{	0xA0,	CPU_COM_CMD_TYPE_ONESHOT_RCV,		0,				0,					0	},	/* �Z���T�[�f�[�^�X�V		*/
	{	0xB0,	CPU_COM_CMD_TYPE_RETRY,				3,				5,					0	},	/* ��ԕύX(G1D)			*/
	{	0xF0,	CPU_COM_CMD_TYPE_ONESHOT_SEND,		0,				0,					0	},	/* PC���O���M(�����R�}���h)	*/
																								// �v���O�����X�V
	{	0xC0,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* �v���O�����]���J�n */
	{	0xC1,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* �v���O�����]���f�[�^�v�� */
	{	0xC2,	CPU_COM_CMD_TYPE_ONESHOT_RCV,		5,				3,					0	},	/* �v���O�����]��(��M)	*/
	{	0xC3,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* �v���O�����]���T���l�v�� */
	{	0xCF,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* �v���O�����]�����ʗv�� */
	{	0xD0,	CPU_COM_CMD_TYPE_RETRY,				5,				3,					0	},	/* �t�@�C���]���J�n */
	{	0xD1,	CPU_COM_CMD_TYPE_RENSOU,			0,				0,					3	},	/* �t�@�C���]��(�A��) */
	{	0xD3,	CPU_COM_CMD_TYPE_RETRY,				5,				10,					0	},	/* �u���b�N�]�����ʗv�� */
	{	0xDF,	CPU_COM_CMD_TYPE_RETRY,				5,				10,					0	},	/* �t�@�C���]�����ʗv�� *///SH���̌��ʂ��x���Ȃ鎖����������
};


void cpu_com_init( void )
{
	SERIAL_EVENT_PARAM call_back = {0};
	
	call_back.rx_callback = &cpu_com_read_comp;
	call_back.tx_callback = &cpu_com_write_comp;
	call_back.err_callback = &cpu_com_error_comp;
	
	s_cpu_com_snd_flg = OFF;

	serial_init( &call_back );
	
	// ��M�f�[�^�T�C�Y�̎w��
	serial_read( ( uint8_t * )&serial_drv_rcv[ 0 ], SERIAL_DRV_RCV_SIZE );

	cpu_com_init_sub();
	
}

//  �ǂݏo������(�R�[���o�b�N)
void cpu_com_read_comp( void )
{
#if 0
	uint8_t *ke_msg;

	DTM2Wire_Data_Wr_Point++;
	serial_read( ( uint8_t * )&DTM2Wire_Data[ ( DTM2Wire_Data_Wr_Point & ( DTM2WIRE_MAX_BUF_CNT - 1 ) ) ], sizeof( uint16_t ) );
#else
	// ��M�f�[�^�T�C�Y�̎w��
	serial_read( ( uint8_t * )&serial_drv_rcv[ 0 ], SERIAL_DRV_RCV_SIZE );
	if( E_QOVR == write_ring_buf( &drv_uart0_rcv_ring, serial_drv_rcv[0] )){
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
		__no_operation();		// RD8001�b��F�u���C�N�\��p
	}
#endif
	//��UOS�Ȃ�
#if 0
	if ( _DTM2WIRE_INIT_STATE_COMPLETE == cpu_com_init_Flg ) {
	    ke_msg = ke_msg_alloc( DTM2RBLE_READ_COMPLETE, USER_MAIN_ID, USER_MAIN_ID, 0 );
    	ke_msg_send(ke_msg);
	}
#endif

}

// �������݊���(�R�[���o�b�N)
void cpu_com_write_comp( void )
{
	s_cpu_com_snd_flg = OFF;
}

void cpu_com_error_comp( void )
{
#if 0
	if ( _RBLE_RUN_MODE_INIT == rBLE_Run_Mode_Flg ) {
		rBLE_Run_Mode_Flg = _RBLE_RUN_MODE_NORMAL;
	} else {
	}
#endif
#if 0
		serial_read( ( uint8_t * )&DTM2Wire_Data[ ( DTM2Wire_Data_Wr_Point & ( DTM2WIRE_MAX_BUF_CNT - 1 ) ) ], sizeof( uint16_t ) );
#else
	// ��M�f�[�^�T�C�Y�̎w��
	serial_read( ( uint8_t * )&serial_drv_rcv[ 0 ], SERIAL_DRV_RCV_SIZE );
#endif
}

// ========================================
// H1D���ʃR�[�h
// ========================================

// ========================================

/************************************************************************/
/* �֐�     : cpu_com_init												*/
/* �֐���   : ����������												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : ����������													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void cpu_com_init_sub(void)
{
	memset(&s_cpu_com_ds_input, 0x00, sizeof(s_cpu_com_ds_input));
	s_p_cpu_com_ds_order = &s_ds_cpu_com_order;		//RD8001�b��

	s_cpu_com_snd_cmd = 0x00;
	memset(s_cpu_com_snd_data, 0x00, sizeof(s_cpu_com_snd_data));
	s_cpu_com_snd_size = 0;
	s_cpu_com_snd_type = 0;
	s_cpu_com_snd_retry_cnt = 0;
	s_cpu_com_snd_timeout = 0;
	s_cpu_com_snd_rensou_cnt = 0;
	s_cpu_com_snd_seq_no = 0;
	s_cpu_com_res_seq_no = 0;
	
	s_cpu_com_snd_status = CPU_COM_SND_STATUS_IDLE;
	
	memset(s_cpu_com_rcv_msg.buf, 0x00, sizeof(s_cpu_com_rcv_msg.buf));
	s_cpu_com_rcv_msg_size = 0;
	
#if FUNC_DEBUG_CPU_COM == OFF
	drv_uart0_data_init();
#else
	drv_uart1_data_init();
#endif
}

/************************************************************************/
/* �֐�     : cpu_com_crcset											*/
/* �֐���   : CRC�Z�b�g													*/
/* ����     : UB *msg : �Ώۃf�[�^�A�h���X								*/
/*			: UH size : �Ώۃf�[�^��									*/
/*          : UH crc : CRC�v�Z����										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2012.11.30  Axia Soft Design �Y�v��   ���ō쐬			*/
/*          : 2014.05.13  Axia Soft Design �g��		CPU�ԒʐM�p�ɈڐA	*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� : ���Z�̑Ώۂ́ACMD�ASEQ�ADATA�܂łƂ���B					*/
/************************************************************************/
STATIC void cpu_com_crcset(UB *msg, UH size, UH *crc)
{
	/* CRC�v�Z */
	*crc = crc16(msg, (INT)size);

}

/************************************************************************/
/* �֐�     : cpu_com_proc												*/
/* �֐���   : CPU�ԒʐM��������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
void cpu_com_proc(void)
{
	/* �A�v������f�[�^�擾 */
	ds_get_cpu_com_order( &s_p_cpu_com_ds_order );
	
	/* ��M�f�[�^��� */
	cpu_com_rcv_proc();
	
	/* �������M���� */
	cpu_com_send_proc();
	
	/* ��Ԓʒm�p�ϐ��̃Z�b�g */
	s_cpu_com_ds_input.cpu_com_send_status = s_cpu_com_snd_status;
	
	/* �A�v���ւ̃f�[�^�ݒ� */
	ds_set_cpu_com_input( &s_cpu_com_ds_input );
	
	/* �f�[�^�ʒm��̓N���A */
	s_cpu_com_ds_input.rcv_cmd = 0x00;
	memset( s_cpu_com_ds_input.rcv_data, 0x00, sizeof(s_cpu_com_ds_input.rcv_data));
	
}

/************************************************************************/
/* �֐�     : get_cpu_com_send_req										*/
/* �֐���   : CPU�ԒʐM���X�|���X�҂����M����							*/
/* ����     : *order ���M�v��											*/
/* ����     : *input ��M�f�[�^�i�[�p									*/
/* �߂�l   : CPU_COM_SND_RES_OK										*/
/*          : CPU_COM_SND_RES_BUSY_NG									*/
/*          : CPU_COM_SND_RES_RETRY_OUT_NG								*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : �w��R�}���h�𑗐M�����X�|���X��M�҂����s��					*/
/************************************************************************/
/* ���ӎ��� :���M�\��Ԃ̂ݎ��s�\									*/
/*          :���M�X�e�[�^�X�͎��������ɂ���čX�V����邽�߃r�W�[������	*/
/*          :���������̂���ʊ֐��̃��[�v�ő҂��͏o���Ȃ�			*/
/*          :��ʂ̃V�[�P���X�ɑ��M�\�҂���݂��邱��					*/
/************************************************************************/
#if 0
UB get_cpu_com_send_req( DS_CPU_COM_ORDER *order, DS_CPU_COM_INPUT *input )
{
	
	if( CPU_COM_SND_STATUS_IDLE != s_cpu_com_snd_status ){
		/* ���M�\��ԈȊO�͖��� */
		return CPU_COM_SND_RES_BUSY_NG;	/* CPU�ԒʐM�r�W�[ */
	}
	/* �I�[�_�[�̃Z�b�g */
	s_p_cpu_com_ds_order = order;
	
	/* ���M�����܂ŌJ��Ԃ� */
	while( CPU_COM_SND_STATUS_COMPLETE != s_cpu_com_snd_status ){
		/* ��M�f�[�^��� */
		cpu_com_rcv_proc();
		/* ���M���� */ /* ���������Ɠ��� */
		cpu_com_send_proc();
		
		if( CPU_COM_SND_STATUS_SEND_NG == s_cpu_com_snd_status ){
			/* ���g���C�A�E�g */
			return CPU_COM_SND_RES_RETRY_OUT_NG;
		}else if( CPU_COM_SND_STATUS_IDLE == s_cpu_com_snd_status ){
			/* �A�C�h����ԂɑJ�ڂ��鎖�͖��� �J�ڂ����ꍇ�̓R�}���h�ݒ��� */
			return CPU_COM_SND_RES_RETRY_OUT_NG;
		}else{
			/* �������Ȃ� */
		}
		
		/* RD1402�b�� ���g���C�҂����Ԃɂ���Ă�WDT���t���b�V�������K�v */
		//wdt_refresh();
	}
	
	/* ��Ԓʒm�p�ϐ��̃Z�b�g */
	s_cpu_com_ds_input.cpu_com_send_status = s_cpu_com_snd_status;
	
	/* �A�v���ւ̃f�[�^�ݒ� */
	*input = s_cpu_com_ds_input;
	
	/* �f�[�^�ʒm��̓N���A */
	s_cpu_com_ds_input.rcv_cmd = 0x00;
	memset( s_cpu_com_ds_input.rcv_data, 0x00, sizeof(s_cpu_com_ds_input.rcv_data));
	
	return CPU_COM_SND_RES_OK;
}
#endif

/************************************************************************/
/* �֐�     : cpu_com_rcv_proc											*/
/* �֐���   : ������M����												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void cpu_com_rcv_proc(void)
{
	if( ON == cpu_com_analyze_msg() ){
		/* ��M�f�[�^���� */
#if 1
		s_cpu_com_ds_input.rcv_cmd = s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD];
		/* ��M�V�[�P���XNo�̃Z�b�g */
		s_cpu_com_res_seq_no = s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_SEQ];				/* SEQ���ʃr�b�g */
		/* �A�v���ʒm�p�̃f�[�^�Ɏ�M�f�[�^���Z�b�g */
		memcpy( s_cpu_com_ds_input.rcv_data, &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_DATA], ( s_cpu_com_rcv_msg_size- CPU_COM_MSG_SIZE_MIN ));
#endif
		// �f�o�b�O���M
//		test_cpu_com_send();
	}else{
#if 1
		/* ��M�Ȃ� */
		s_cpu_com_ds_input.rcv_cmd = 0x00;
		s_cpu_com_res_seq_no = 0x0000;
		memset( s_cpu_com_ds_input.rcv_data, 0x00, sizeof(s_cpu_com_ds_input.rcv_data));
#endif
	}
}


/************************************************************************/
/* �֐�     : cpu_com_analyze_msg										*/
/* �֐���   : ��M���b�Z�[�W���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ON :1���b�Z�[�W��M										*/
/*          : OFF:����M												*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg(void)
{
	//stx����etx�܂Ń`�F�b�N�����b�Z�[�W�o�b�t�@�Ɋi�[
	if( OFF == cpu_com_analyze_msg_check_etx() ){
		return OFF;
	}
	
	if( OFF ==  cpu_com_analyze_msg_check_data() ){
		return OFF;
	}
	
	return ON;
	
}

/************************************************************************/
/* �֐�     : cpu_com_analyze_msg_check_etx								*/
/* �֐���   : ��M���b�Z�[�W���(ETX)									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ON :1���b�Z�[�W��M										*/
/*          : OFF:����M												*/
/* �ύX���� : 2015.10.23 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��M���b�Z�[�W���(ETX)										*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg_check_etx(void)
{
	UB ret = OFF;
	RING_BUF *p_ring_buf;
	UB rcv_data;
	
#if FUNC_DEBUG_CPU_COM == OFF
	p_ring_buf = drv_uart0_get_rcv_ring();
#else
	p_ring_buf = drv_uart1_get_rcv_ring();
#endif
	
	
	// 1�o�C�g���Ɏ�M��ETX�܂Ń`�F�b�N
	while(1){
		if( E_OK != read_ring_buf(p_ring_buf, &rcv_data ) ){
			break;
		}
		//�o�b�t�@�I�[�o�[�����΍�
		if( s_cpu_com_rcv_msg.pos >= CPU_COM_MSG_SIZE_MAX ){
			s_cpu_com_rcv_msg.pos = 0;
			s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_STX_WAIT;
		}
		
		// ����R�[�h
		if( rcv_data == CPU_COM_CTRL_CODE_DLE ){
			if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state )){
				//�f�[�^����DLE�d���Ɣ��f�� ���i�[���Ȃ�
				s_cpu_com_rcv_msg.last_dle_flg = OFF;
			}else{
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = rcv_data;
				s_cpu_com_rcv_msg.last_dle_flg = ON;
			}
		}else{
			//����R�[�h�ȊO
			if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_CTRL_CODE_STX == rcv_data )){
				// STX��M�Ɣ��f
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
//				com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"rcv stx\r\n");
#endif
				s_cpu_com_rcv_msg.pos = 0;
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_DLE;
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_STX;
				s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_ETX_WAIT;
			}else if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_CTRL_CODE_ETX == rcv_data ) && ( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state )){
				// ETX�܂Ŏ擾
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
//				com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"rcv etx\r\n");
#endif
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_ETX;
				s_cpu_com_rcv_msg_size = s_cpu_com_rcv_msg.pos;
				s_cpu_com_rcv_msg.pos = 0;
				s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_STX_WAIT;
				ret = ON;
				break;
			}else{
				if( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state ){
					s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = rcv_data;
				}else{
					s_cpu_com_rcv_msg.pos = 0;
				}
			}
			s_cpu_com_rcv_msg.last_dle_flg = OFF;
		}
	}
	return ret;
}

/************************************************************************/
/* �֐�     : cpu_com_analyze_msg_check_data							*/
/* �֐���   : ��M���b�Z�[�W���(CRC,SUM�`�F�b�N)						*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ON :1���b�Z�[�W��M										*/
/*          : OFF:����M												*/
/* �ύX���� : 2015.10.23 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��M���b�Z�[�W���(CRC,SUM�`�F�b�N)							*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB cpu_com_analyze_msg_check_data(void)
{
	UH crc_rcv = 0; /* ��M�f�[�^CRC */
	UH tmp = 0;
	UH data_size = 0;
	
	if(( CPU_COM_MSG_SIZE_MIN > s_cpu_com_rcv_msg_size ) ||
		( CPU_COM_MSG_SIZE_MAX < s_cpu_com_rcv_msg_size )){
		/* ���b�Z�[�W�T�C�Y�ُ� */
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
		com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU_COM DATA_SIZE_NG\r\n");
#endif
		return OFF;
	}
	/* STX,ETX,SUM�ACRC���������f�[�^�� */
	data_size = s_cpu_com_rcv_msg_size - ( CPU_COM_STX_SIZE + CPU_COM_CRC_SIZE  + CPU_COM_ETX_SIZE );
	
	/*  CRC�`�F�b�N */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size ];			/* CRC���ʃr�b�g */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size + 1 ] << 8;	/* CRC��ʃr�b�g */
	cpu_com_crcset( &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD], data_size, &tmp );

	if( crc_rcv != tmp ){
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
		com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU_COM CRC_NG\r\n");
#endif
		return OFF;
	}
	
	/* �`�F�b�NOK */
	return ON;
}

/************************************************************************/
/* �֐�     : cpu_com_send_proc											*/
/* �֐���   : �������M����												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/*          : 2016.03.10  Axia Soft Design ����		�V�[�P���X�`�F�b�N�폜(RD1508�Ή�) */
/************************************************************************/
/* �@�\ : �������M����													*/
/* ���M��������ё��M�X�e�[�^�X(�~�h��)�̕ύX���s��						*/
/* ��)																	*/
/* �������K�v�ȃR�}���h�́A������M�܂��̓G���[�m��܂ő��M�\��ԂƂ͂��Ȃ� */
/* �A���≞�����s�v�ȃR�}���h�́A���M�㑗�M�\��ԂƂ���B				*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
UB dbg_rcv_int = 0;
#endif

STATIC void cpu_com_send_proc(void)
{
#if 1
	ke_time_t now_time;
//	ke_time_t time;
	UB send_status;
//	UH res_seq_expect;	/* �O�񑗐M���V�[�P���XNo *///RD1508�폜�\��F�V�[�P���X
	
	/* �h���C�o�̑��M��Ԃ��擾 */
	drv_cpu_com_get_send_status( &send_status );
//	s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE;
	send_status = DRV_CPU_COM_STATUS_CAN_SEND;		//RD8001�b��
	
	now_time = ke_time();
	
	switch( s_cpu_com_snd_status ){
		case CPU_COM_SND_STATUS_RCV_WAIT:
			/* ��M�҂���� */
//			res_seq_expect = s_cpu_com_snd_seq_no-1;//RD1508�폜�\��F�V�[�P���X

#if 1		//RD1508�b��	�V�[�P���X�`�F�b�N�͖������\��
			if( s_cpu_com_snd_cmd == s_cpu_com_ds_input.rcv_cmd ){
#else
			if(( s_cpu_com_snd_cmd == s_cpu_com_ds_input.rcv_cmd ) &&
				( res_seq_expect == s_cpu_com_res_seq_no )){	/* ����R�}���h ���� ����V�[�P���XNo */
#endif
				/* ������M */
				s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE; /* ���M���� */
				
			}else{
				/* ��M�Ȃ� */
//				time_soft_get_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, &time);
				if(( now_time - s_cpu_com_snd_last_time ) >= s_cpu_com_snd_timeout ){
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
					com_srv_printf(COM_SRV_LOG_COMMAND,(const B*)"CPU_COM_RCV TIME_OUT:%d\r\n",dbg_rcv_int );
//					if( 1 == dbg_rcv_int ){
						dbg_rcv_int = 0;
//						//drv_cpu_com_init(ON);		//���ʂȂ�
//					}
#endif
					if( 0 == s_cpu_com_snd_retry_cnt ){
						/* ���g���C�A�E�g */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_SEND_NG; /* ���g���CNG */
#if FUNC_DBG_CPU_COM_LOG_ERR == ON
						com_srv_puts(COM_SRV_LOG_DEBUG,(const B*)"CPU�ԒʐM���g���C�A�E�g(�v���I)\r\n");
#endif
					}else{
						if( DRV_CPU_COM_STATUS_CAN_SEND == send_status ){
							/* ���M�\��� */
							cpu_com_send_data( &s_cpu_com_snd_data[ 0 ], s_cpu_com_snd_size );
							/* ���g���C�񐔌��Z */
							s_cpu_com_snd_retry_cnt--;
							/* �^�C���A�E�g���ԍĐݒ� */
							s_cpu_com_snd_last_time = now_time;
//							time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
						}
					}
				}
			}
			break;
		case CPU_COM_SND_STATUS_RENSOU:
			/* �A������� */
//			time_soft_get_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, &time);
			if(( now_time - s_cpu_com_snd_last_time ) >= s_cpu_com_snd_timeout ){
				if( DRV_CPU_COM_STATUS_CAN_SEND == send_status ){
					/* ���M�\��� */
					if( 0 < s_cpu_com_snd_rensou_cnt ){
						
						cpu_com_send_data( &s_cpu_com_snd_data[ 0 ], s_cpu_com_snd_size );
						/* �A���񐔌��Z */
						s_cpu_com_snd_rensou_cnt--;
						/* �^�C���A�E�g���ԍĐݒ� */
						s_cpu_com_snd_last_time = now_time;
//						time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
					}else{
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE; /* ���M���� */
					}
				}
			}
			break;
		case CPU_COM_SND_STATUS_SEND_WAIT:
			/* ���M�҂���� */
			if( DRV_CPU_COM_STATUS_CAN_SEND == send_status ){
				/* ���M�\�܂ő҂� */
				if( TRUE == cpu_com_make_send_data() ){
					cpu_com_send_data( &s_cpu_com_snd_data[ 0 ], s_cpu_com_snd_size );
					if( CPU_COM_CMD_TYPE_RETRY == s_cpu_com_snd_type ){
						/* ���g���C�^�C�v ��M�҂��֑J�� */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_RCV_WAIT;
						s_cpu_com_snd_last_time = now_time;
//						time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
					}else if( CPU_COM_CMD_TYPE_RENSOU == s_cpu_com_snd_type ){
						/* �A���^�C�v �A���҂��֑J�� */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_RENSOU;
						s_cpu_com_snd_last_time = now_time;
//						time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
					}else{
						/* �����V���b�g ���M�����֑J�� */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE;
					}
				}
				/* �A�v������̃f�[�^������芮�� */
				cpu_com_clear_send_order();
			}
			break;
		case CPU_COM_SND_STATUS_IDLE:
		case CPU_COM_SND_STATUS_COMPLETE:
		case CPU_COM_SND_STATUS_SEND_NG:
		default:
			/* ���M�\��� */
			if( CPU_COM_CMD_NONE != s_p_cpu_com_ds_order->snd_cmd_id ){
				/* ���M�v���L�� */
				if( DRV_CPU_COM_STATUS_CAN_SEND == send_status ){
					/* ���M�\��� */
					if( TRUE == cpu_com_make_send_data() ){
						cpu_com_send_data( &s_cpu_com_snd_data[ 0 ], s_cpu_com_snd_size );
						if( CPU_COM_CMD_TYPE_RETRY == s_cpu_com_snd_type ){
							/* ���g���C�^�C�v ��M�҂��֑J�� */
							s_cpu_com_snd_status = CPU_COM_SND_STATUS_RCV_WAIT;
							s_cpu_com_snd_last_time = now_time;
//							time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
						}else if( CPU_COM_CMD_TYPE_RENSOU == s_cpu_com_snd_type ){
							/* �A���^�C�v �A���҂��֑J�� */
							s_cpu_com_snd_status = CPU_COM_SND_STATUS_RENSOU;
							s_cpu_com_snd_last_time = now_time;
//							time_soft_set_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, s_cpu_com_snd_timeout);
						}else{
							/* �����V���b�g ���M�����֑J�� */
							s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE;
						}
					}
					/* �A�v������̃f�[�^������芮�� */
					cpu_com_clear_send_order();
				}else{
					/* �h���C�o���M�s�� */
					/* ���M�҂���Ԃ֑J�� */
					s_cpu_com_snd_status = CPU_COM_SND_STATUS_SEND_WAIT;
				}
			}else{
				/* ���M�v�������ŃA�C�h����� */
				s_cpu_com_snd_status = CPU_COM_SND_STATUS_IDLE;
			}
			break;
		
	}
#endif
}


/************************************************************************/
/* �֐�     : cpu_com_make_send_data									*/
/* �֐���   : ���M�f�[�^����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : TRUE:���� FALSE:�ُ�										*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : �A�v������̎w���ɏ]�����M�f�[�^�𐶐�����					*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC UB cpu_com_make_send_data(void)
{
	CPU_COM_CMD_ID cmd_id;
	UH size;
	
	UH crc;
	UH pos = 0;
//	RING_BUF* p_ring;
//	INT i = 0;
//	UH wr_size;
	
	/* �R�}���h�A�f�[�^�A�f�[�^���̃N���A */
	s_cpu_com_snd_cmd = 0x00;
	memset( s_cpu_com_snd_data, 0x00, sizeof(s_cpu_com_snd_data) );
	s_cpu_com_snd_size = 0;
	
	/* ���g���C�񐔁A�^�C���A�E�g���ԁA�A���񐔃N���A */
	s_cpu_com_snd_type = CPU_COM_CMD_TYPE_ONESHOT_SEND;
	s_cpu_com_snd_retry_cnt = 0;
	s_cpu_com_snd_timeout = 0;
	s_cpu_com_snd_rensou_cnt = 0;
	
	if(( CPU_COM_CMD_NONE == s_p_cpu_com_ds_order->snd_cmd_id ) || ( CPU_COM_CMD_MAX <= s_p_cpu_com_ds_order->snd_cmd_id ) ||
		( CPU_COM_CMD_TYPE_ONESHOT_RCV == s_tbl_cmd_info[s_p_cpu_com_ds_order->snd_cmd_id].cmd_type )){
		/* �R�}���hID�ُ� */
		return FALSE;
	}
	if( CPU_COM_DATA_SIZE_MAX <= s_p_cpu_com_ds_order->data_size ){
		/* �f�[�^���ُ� */
		return FALSE;
	}
	
	/* �R�}���hID�A�f�[�^���擾 */
	cmd_id = s_p_cpu_com_ds_order->snd_cmd_id;
	size = s_p_cpu_com_ds_order->data_size;
	
	
	/* �R�}���h�A�f�[�^���ێ� */
	s_cpu_com_snd_cmd = s_tbl_cmd_info[cmd_id].cmd;
	s_cpu_com_snd_size = size + CPU_COM_MSG_SIZE_MIN;
	
	/* STX */
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_DLE;
	pos++;
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_STX;
	pos++;
	
	/* �R�}���h�̃Z�b�g */
	s_cpu_com_snd_data[pos] = s_cpu_com_snd_cmd;
	pos++;
	
	/* SEQ�̃Z�b�g */
	s_cpu_com_snd_data[pos] = s_cpu_com_snd_seq_no;
	pos += CPU_COM_SEQ_SIZE;

	//RD1508�b��
#if FUNC_DBG_CPU_COM_LOG_EVENT == ON
	if( s_cpu_com_snd_cmd == 0xA0 ){
		//RD1508�b��FCPU�ԒʐM�f�o�b�O�p����
#if 0
		if( s_p_cpu_com_ds_order->snd_data[0] == 214 ){
//			return FALSE;
		}
#endif
		com_srv_printf(COM_SRV_LOG_COMMAND,(const B*)"CPUCOM �\���X�V:%d\r\n",s_p_cpu_com_ds_order->snd_data[0] + ( s_p_cpu_com_ds_order->snd_data[1] << 8 ));
	}
	if( 0xA1 == s_cpu_com_snd_cmd ){
		com_srv_printf(COM_SRV_LOG_DISP,(const B*)"CPUCOM �C�x���g:%d\r\n",s_p_cpu_com_ds_order->snd_data[0] + ( s_p_cpu_com_ds_order->snd_data[1] << 8 ));
	}
	if( 0xA2 == s_cpu_com_snd_cmd ){
		//�X�e�[�^�X�v��
		com_srv_puts(COM_SRV_LOG_COMMAND,(const B*)"CPUCOM �X�e�[�^�X�v��\r\n");
	}
#endif
	/* �f�[�^�̃Z�b�g */
	memcpy( &s_cpu_com_snd_data[pos], s_p_cpu_com_ds_order->snd_data, size );
	pos += size;
	
	/* CRC�̃Z�b�g */
	cpu_com_crcset( &s_cpu_com_snd_data[CPU_COM_MSG_TOP_POS_CMD], size + CPU_COM_CMD_SIZE + CPU_COM_SEQ_SIZE , &crc );
	s_cpu_com_snd_data[pos] = (UB)( crc & 0x00FF );
	s_cpu_com_snd_data[pos+1] = (UB)( crc >> 8 );
	pos += CPU_COM_CRC_SIZE;
	
	/* ETX */
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_DLE;
	pos++;
	s_cpu_com_snd_data[pos] = CPU_COM_CTRL_CODE_ETX;
	pos++;

	/* DLE �g�� */
	s_cpu_com_snd_size = cpu_com_dle_extension( &s_cpu_com_snd_data[0], s_cpu_com_snd_size );
	
	/* �R�}���h�^�C�v�A���g���C�E�A���񐔃Z�b�g */
	s_cpu_com_snd_type = s_tbl_cmd_info[cmd_id].cmd_type;
	s_cpu_com_snd_retry_cnt = s_tbl_cmd_info[cmd_id].retry_cnt;
	s_cpu_com_snd_timeout = s_tbl_cmd_info[cmd_id].retry_time;
	if( 1 < s_tbl_cmd_info[cmd_id].rensou_cnt ){
		/* �A���񐔂͏�����܂ނ���1���Z */
		s_cpu_com_snd_rensou_cnt = s_tbl_cmd_info[cmd_id].rensou_cnt-1;
	}
	
	/* �V�[�P���XNo���Z */
	s_cpu_com_snd_seq_no++;
	
	// ���M�o�b�t�@��������
#if 1
	cpu_com_send_data( &s_cpu_com_snd_data[ 0 ], s_cpu_com_snd_size );
#else
	p_ring = drv_uart0_get_snd_ring();
	for( i = 0;i < (size + CPU_COM_MSG_SIZE_MIN) ; i++ ){
		if( E_OK != write_ring_buf( p_ring, s_cpu_com_snd_data[i] )){	/* �����O�o�b�t�@�������� */
			break;
		}
	}
#endif

	return TRUE;
}


/************************************************************************/
/* �֐�     : cpu_com_clear_send_order									*/
/* �֐���   : ���M�v���N���A											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.05.08  Axia Soft Design �g��		���ō쐬			*/
/************************************************************************/
/* �@�\ : �A�v������̎w�����N���A����									*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void cpu_com_clear_send_order(void)
{
	if( NULL != s_p_cpu_com_ds_order ){
		/* �R�}���h�N���A */
		s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_NONE;
		/* �f�[�^�N���A */
		memset( s_p_cpu_com_ds_order->snd_data, 0x00, CPU_COM_DATA_SIZE_MAX );
		/* �f�[�^���N���A */
		s_p_cpu_com_ds_order->data_size = 0;
	}
}

/************************************************************************/
/* �֐�     : cpu_com_sub_reset											*/
/* �֐���   : �T�u�}�C�R�������Z�b�g����								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.06.24 Axia Soft Design �g��	���ō쐬				*/
/*          : 2016.02.23 Axia Soft Design ����	�T�u�}�C�R���ɖ��̕ύX	*/
/************************************************************************/
/* �@�\ : �T�u�}�C�R�������Z�b�g����									*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
#if 0
void cpu_com_sub_reset(void)
{
	/* �h���C�o�֐����R�[�� */
	drv_cpu_com_sub_reset();
}
#endif

/************************************************************************/
/* �֐�     : cpu_com_get_status										*/
/* �֐���   : CPU�X�e�[�^�X�擾											*/
/* ����     : �Ȃ�														*/
/* �߂�l   :	CPU_COM_SND_STATUS_IDLE			���M�\���			*/
/*				CPU_COM_SND_STATUS_RCV_WAIT		������M�҂����		*/
/* 				CPU_COM_SND_STATUS_RENSOU		�A�������				*/
/* 				CPU_COM_SND_STATUS_COMPLETE		���M�������			*/
/* 				CPU_COM_SND_STATUS_SEND_NG		���g���CNG				*/
/* �ύX���� : 2014.07.09 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : CPU�X�e�[�^�X�擾												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
UB cpu_com_get_status(void)
{
	/* �h���C�o�֐����R�[�� */
	return s_cpu_com_snd_status;
}


/************************************************************************/
/* �֐�     : cpu_com_dle_extension										*/
/* �֐���   : DLE�L��													*/
/* ����     : *data	DLE�L������f�[�^									*/
/*          : size	DLE�L������T�C�Y									*/
/* �߂�l   : DLE�L����̃T�C�Y											*/
/* �ύX���� : 2015.10.23 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : DLE�L��														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
UH cpu_com_dle_extension( UB* data, UH size )
{
	UB cpu_com_buf_org[CPU_COM_DATA_SIZE_MAX];
	int i = 0;
	UH extension_size = 0;
	
	memcpy( &cpu_com_buf_org[0], data, size );
	
	// STX�Ή� �����g��
	*data++ = cpu_com_buf_org[0];
	*data++ = cpu_com_buf_org[1];
	
	// DLE�g��
	for( i = CPU_COM_STX_SIZE; i < ( size - CPU_COM_ETX_SIZE ); i++ ){
		*data++ = cpu_com_buf_org[i];
		if( CPU_COM_CTRL_CODE_DLE == cpu_com_buf_org[i] ){
			*data++ = CPU_COM_CTRL_CODE_DLE;
			extension_size++;
		}
	}
	// ETX�Ή� �����g��
	*data++ = cpu_com_buf_org[i++];
	*data   = cpu_com_buf_org[i];

	return ( size + extension_size );
}

STATIC void cpu_com_send_data( UB* data, UH len )
{
	s_cpu_com_snd_flg = ON;
	serial_write( data, len );
}

UB cpu_com_get_busy( void )
{
	return s_cpu_com_snd_flg;
}


#if 0
/************************************************************************/
/* �֐�     : cpu_com_file_write										*/
/* �֐���   : �t�@�C��������											*/
/* ����     : *p_buf	�������݃f�[�^									*/
/*          : size		�T�C�Y											*/
/*          : adrs		�A�h���X										*/
/*          : file_no	�t�@�C���ԍ�									*/
/* �߂�l   : TRUE		����											*/
/*          : FALSE		���s											*/
/* �ύX���� : 2015.10.23 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : �t�@�C��������												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
UB cpu_com_file_write( UB* p_buf, UH size, UW adrs, UB file_no )
{
	UB ret;
	DS_CPU_COM_ORDER	com_order;
	DS_CPU_COM_INPUT	com_input;
	UB adrs_num[4];
	UB file_num[2];
	UB size_num[2];

	/* �T�u�}�C�R������J�n�R�}���h���M */
	com_order.snd_cmd_id = CPU_COM_CMD_FILE_WRITE;

	/* �t�@�C���ԍ��̃Z�b�g */
	chg_uh_ub( file_no, &file_num[1], &file_num[0] );
	com_order.snd_data[0] = file_num[0];
	com_order.snd_data[1] = file_num[1];
	// �u���b�N�ԍ�
	
	chg_uw_ub( adrs, &adrs_num[3], &adrs_num[2], &adrs_num[1], &adrs_num[0] );
	com_order.snd_data[2] = adrs_num[0];
	com_order.snd_data[3] = adrs_num[1];
	com_order.snd_data[4] = adrs_num[2];
	com_order.snd_data[5] = adrs_num[3];

	chg_uh_ub( size, &size_num[1], &size_num[0] );
	com_order.snd_data[6] = size_num[0];
	com_order.snd_data[7] = size_num[1];
	memcpy( &com_order.snd_data[8], p_buf, SD_MOVE_ACCESS_SIZE );
	
	com_order.data_size = SD_MOVE_ACCESS_SIZE + 8;
	ret = get_cpu_com_send_req( &com_order, &com_input );
	
	//RD1508�b��F�f�o�b�O�R�[�h�@���葤���L���ɂȂ����疳��
//	return TRUE;
	
	
	if(( CPU_COM_SND_RES_OK != ret ) || ( 0 != com_input.rcv_data[0] )){
		return FALSE;
	}
	if( CPU_COM_SND_STATUS_COMPLETE != com_input.cpu_com_send_status ){
		return FALSE;
	}
	
	return TRUE;
}

/************************************************************************/
/* �֐�     : cpu_com_file_read											*/
/* �֐���   : �t�@�C���ǂݏo��											*/
/* ����     : *p_buf	�������݃f�[�^									*/
/*          : size		�T�C�Y											*/
/*          : block		�u���b�N										*/
/*          : file_no	�t�@�C���ԍ�									*/
/* �߂�l   : TRUE		����											*/
/*          : FALSE		���s											*/
/* �ύX���� : 2015.10.23 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : �t�@�C��������												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
UB cpu_com_file_read( UB* p_buf, UH read_size, UW adrs, UB file_no )
{
	UB ret;
	DS_CPU_COM_ORDER	com_order;
	DS_CPU_COM_INPUT	com_input;
	UB adrs_num[4];
	UB file_num[2];
	UB size_num[2];
	int i;
	UB *p_data;
	UH size;
	
	/* �T�u�}�C�R������J�n�R�}���h���M */
	com_order.snd_cmd_id = CPU_COM_CMD_FILE_READ;

	/* �t�@�C���ԍ��̃Z�b�g */
	chg_uh_ub( file_no, &file_num[1], &file_num[0] );
	com_order.snd_data[0] = file_num[0];
	com_order.snd_data[1] = file_num[1];

	/* �u���b�N�ԍ��̃Z�b�g */
	chg_uw_ub( adrs, &adrs_num[3], &adrs_num[2], &adrs_num[1], &adrs_num[0] );
	com_order.snd_data[2] = adrs_num[0];
	com_order.snd_data[3] = adrs_num[1];
	com_order.snd_data[4] = adrs_num[2];
	com_order.snd_data[5] = adrs_num[3];

	chg_uh_ub( read_size, &size_num[1], &size_num[0] );
	com_order.snd_data[6] = size_num[0];
	com_order.snd_data[7] = size_num[1];

	com_order.data_size = 8;
	ret = get_cpu_com_send_req( &com_order, &com_input );
	
	//RD1508�b��F�f�o�b�O�R�[�h�@���葤���L���ɂȂ����疳��
//	return TRUE;

	if(( CPU_COM_SND_RES_OK != ret ) || ( 0 != com_input.rcv_data[0] )){
		return FALSE;
	}
	if( CPU_COM_SND_STATUS_COMPLETE != com_input.cpu_com_send_status ){
		return FALSE;
	}
	p_data = &com_input.rcv_data[0];
	
	p_data++;	// ���ʓǂݔ�΂�
	
	/* �f�[�^�� */
	size = (*(UH*)p_data);
	p_data++;	// �f�[�^���ǂݔ�΂�
	p_data++;	// �f�[�^���ǂݔ�΂�
	
	// �w��T�C�Y�Ɠǂݏo���T�C�Y���Ⴄ
	if( size != read_size ){
		return FALSE;
	}
	
	/* �t�@�C���ǂݏo������ */
	for( i = 0;i < size ; i++ ){
		p_buf[i] = p_data[i];
	}
	
	return TRUE;
}
#endif

STATIC UB			s_drv_cpu_com_snd_status;							/* CPU�ԒʐM���M�X�e�[�^�X */
/************************************************************************/
/* �֐�     : drv_cpu_com_set_send_status								*/
/* �֐���   : CPU�ԒʐM���M��ԃZ�b�g									*/
/* ����     : UB status : �X�V������									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.04.25 Axia Soft Design �g�� �v�a	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�ԒʐM�̑��M��Ԃ��X�V����										*/
/************************************************************************/
/* ���ӎ��� :															*/
/* ���g�p�֐�(2016.02.24)�v���g�R���ύX�ɂ���ʂ���ݒ肳��Ȃ��Ȃ��� */
/************************************************************************/
void drv_cpu_com_set_send_status( UB status )
{
	
	s_drv_cpu_com_snd_status = status;
}

/************************************************************************/
/* �֐�     : drv_cpu_com_get_send_status								*/
/* �֐���   : CPU�ԒʐM���M��Ԏ擾										*/
/* ����     : UB *status : ���M���										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2014.04.25 Axia Soft Design �g�� �v�a	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* CPU�ԒʐM�̑��M��Ԃ��擾����										*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void drv_cpu_com_get_send_status( UB *status )
{
	
	*status = s_drv_cpu_com_snd_status;
}


//RD8001�b��F�e�X�g��
void test_cpu_com_send( void )
{
//	s_p_cpu_com_ds_order->snd_cmd_id = CPU_COM_CMD_STATUS_REQ;		/* ���M�R�}���hID */
//	s_p_cpu_com_ds_order->snd_data[0] = 0x00;								/* ���M�f�[�^ */
//	s_p_cpu_com_ds_order->data_size = 1;										/* ���M�f�[�^�� */
	
	// �f�[�^�쐬
	cpu_com_make_send_data();
}



/************************************************************************/
/* �֐�     : drv_uart0_init											*/
/* �֐���   : UART0����������											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* UART0�̏���������													*/
/************************************************************************/
/* ���ӎ��� :															*/
/* RX321�n�[�h�E�F�A�}�j���A��P.1386�Q��								*/
/* �f�o�b�O�|�[�g�L������SCI10�A��������SCI1�œ���Asys.h�Ő؂�ւ��\ */
/************************************************************************/
void drv_uart0_data_init( void )
{
	/* ���M�����O�o�b�t�@������ */
	memset( &drv_uart0_send_buf[0], 0x00, sizeof(drv_uart0_send_buf) );
	ring_buf_init( &drv_uart0_send_ring, &drv_uart0_send_buf[0], DRV_UART0_SND_RING_LENGTH );
	
	/* ��M�����O�o�b�t�@������ */
	memset( &drv_uart0_rcv_buf[0], 0x00, sizeof(drv_uart0_rcv_buf) );
	ring_buf_init( &drv_uart0_rcv_ring, &drv_uart0_rcv_buf[0], DRV_UART0_RCV_RING_LENGTH );	
}

/************************************************************************/
/* �֐�     : drv_uart0_get_snd_ring									*/
/* �֐���   : ���M�����O�o�b�t�@�|�C���^�擾							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ���M�����O�o�b�t�@�̃|�C���^								*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �����O�o�b�t�@�̃|�C���^��Ԃ�										*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
#if 0		//���g�p
RING_BUF* drv_uart0_get_snd_ring( void )
{
	return &drv_uart0_send_ring;

}
#endif

/************************************************************************/
/* �֐�     : drv_uart0_get_rcv_ring									*/
/* �֐���   : ��M�����O�o�b�t�@�|�C���^�擾							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ��M�����O�o�b�t�@�̃|�C���^								*/
/* �ύX���� : 2018.01.25 Axia Soft Design ���� ��	���ō쐬			*/
/************************************************************************/
/* �@�\ :																*/
/* �����O�o�b�t�@�̃|�C���^��Ԃ�										*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
RING_BUF* drv_uart0_get_rcv_ring( void )
{
	return &drv_uart0_rcv_ring;
}

