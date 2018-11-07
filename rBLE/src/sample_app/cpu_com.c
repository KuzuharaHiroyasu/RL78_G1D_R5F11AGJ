/************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[									*/
/* �t�@�C����   : cpu_com.c												*/
/* �@�\         : CPU�ԒʐM�~�h��										*/
/* �ύX����     : 2018.04.10 Axia Soft Design ����		���ō쐬		*/
/* ���ӎ���     : �Ȃ�													*/
/************************************************************************/
/********************/
/*     include      */
/********************/
#include	"header.h"		//���[�U�[��`

#include	"uart.h"


/************************************************************/
/* �v���g�^�C�v�錾											*/
/************************************************************/


STATIC void cpu_com_init_sub(void);
STATIC void cpu_com_crcset(UB *msg, UH size, UH *crc);
STATIC void cpu_com_rcv_proc(void);
STATIC UB cpu_com_analyze_msg(void);
STATIC UB cpu_com_analyze_msg_check_etx(void);
STATIC UB cpu_com_analyze_msg_check_data(void);
STATIC void cpu_com_send_proc(void);
STATIC UB cpu_com_make_send_data(void);
STATIC void cpu_com_clear_send_order(void);
STATIC void cpu_com_send_data( UB* data, UH len );
// ���M��DMA�]���ňꊇ���M����ׂɃ����O�o�b�t�@�͕s�v
//STATIC RING_BUF* drv_uart0_get_snd_ring( void );
STATIC RING_BUF* drv_uart0_get_rcv_ring( void );



/********************/
/*     �����ϐ�     */
/********************/
STATIC DS_CPU_COM_INPUT s_cpu_com_ds_input;					/* �f�[�^�Ǘ����̃R�s�[�G���A �A�v���ւ̃f�[�^�󂯓n���p */
STATIC DS_CPU_COM_ORDER *s_p_cpu_com_ds_order;				/* �f�[�^�Ǘ����̃|�C���^ �A�v������̎w���p */

STATIC UB s_cpu_com_snd_cmd;								/* ���M�R�}���h */
STATIC UB s_cpu_com_snd_data[CPU_COM_BUF_SIZE_MAX];			/* ���M���b�Z�[�W */
STATIC UH s_cpu_com_snd_size;								/* ���M���b�Z�[�W�� */
STATIC UB s_cpu_com_snd_type;								/* ���M�R�}���h�^�C�v */
STATIC UB s_cpu_com_snd_retry_cnt;							/* ���M���g���C�� */
STATIC UW s_cpu_com_snd_timeout;							/* ���g���C�^�C���A�E�g���� *10ms */
STATIC UB s_cpu_com_snd_rensou_cnt;							/* ���M�A���� */

STATIC UW s_cpu_com_snd_last_time;					/* ���M�O�񎞊� */



STATIC UB s_cpu_com_snd_seq_no;								/* ���M�V�[�P���XNo */
//STATIC UB s_cpu_com_res_seq_no;								/* ��M�V�[�P���XNo *///�}�X�^�[�Ŏ�M�V�[�P���X���g�p

STATIC UB s_drv_cpu_com_snd_status;							/* CPU�ԒʐM�h���C�o(�������x��)�̑��M�X�e�[�^�X */
																/* DRV_CPU_COM_STATUS_CAN_SEND		���M�\��� */
																/* DRV_CPU_COM_STATUS_SENDING		���M�� */


STATIC UB s_cpu_com_snd_status;								/* CPU�ԒʐM ���M�X�e�[�^�X */
																/*	CPU_COM_SND_STATUS_IDLE			���M�\���		*/
																/* 	CPU_COM_SND_STATUS_RCV_WAIT		������M�҂����	*/
																/* 	CPU_COM_SND_STATUS_RENSOU		�A�������			*/
																/* 	CPU_COM_SND_STATUS_COMPLETE		���M�������		*/
																/* 	CPU_COM_SND_STATUS_SEND_NG		���g���CNG			*/

STATIC CPU_COM_ANA_RCV_MSG	s_cpu_com_rcv_msg;				/* ��M���b�Z�[�W */
STATIC UH s_cpu_com_rcv_msg_size;							/* ��M���b�Z�[�W�T�C�Y */

// =============================
// �h���C�o�p��`
// =============================
#define	DRV_UART0_RCV_RING_LENGTH				CPU_COM_BUF_SIZE_MAX			/* �����O�o�b�t�@��(1���b�Z�[�W���{�P) ���{�P�̗��R�̓����O�o�b�t�@�̒��ӎ����Q�� */
//#define	DRV_UART0_SND_RING_LENGTH				CPU_COM_BUF_SIZE_MAX		/* �����O�o�b�t�@��(1���b�Z�[�W���{�P) ���{�P�̗��R�̓����O�o�b�t�@�̒��ӎ����Q�� */


#define			SERIAL_DRV_RCV_SIZE				1				//��MDMA�ʐM�T�C�Y:�f�[�^�����Œ�Ŗ���������1�o�C�g�Ŏ�M���� �����׊m�F�ς�(2018.09.06)
uint8_t			serial_drv_rcv[SERIAL_DRV_RCV_SIZE];

//UB drv_uart0_send_buf[DRV_UART0_SND_RING_LENGTH];				/* ���M�o�b�t�@(�����O�p) */// ���M��DMA�]���ňꊇ���M����ׂɃ����O�o�b�t�@�͕s�v
UB drv_uart0_rcv_buf[DRV_UART0_RCV_RING_LENGTH];				/* ��M�o�b�t�@(�����O�p) */
RING_BUF drv_uart0_send_ring;									/* ���M�����O�o�b�t�@�p�R���g���[�� */
RING_BUF drv_uart0_rcv_ring;									/* ��M�����O�o�b�t�@�p�R���g���[�� */


/********************/
/*   �e�[�u����`   */
/********************/
/* �R�}���h�e�[�u�� ���}�X�^�[��p */
/* �^�C�v��ύX���鎖�Ŏ�M��p�ɂ��Ή� */
STATIC const T_CPU_COM_CMD_INFO s_tbl_cmd_info[CPU_COM_CMD_MAX] = {
	/*�R�}���h*/ /*�^�C�v*/					/*���g���C(�����܂�)*//*���g���C�Ԋu[10ms] */ /*�A����*/
	{	0x00,	CPU_COM_CMD_TYPE_ONESHOT_SEND,		0,				0,						0	},	/* �R�}���h����				*/
	{	0xE0,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �X�e�[�^�X�v��			*/
	{	0xA0,	CPU_COM_CMD_TYPE_ONESHOT_RCV,		0,				0,						0	},	/* �Z���T�[�f�[�^�X�V		*/
	{	0xA1,	CPU_COM_CMD_TYPE_ONESHOT_SEND,		0,				0,						0	},	/* �Z���V���O�w��			*/
	{	0xB0,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* ��ԕύX(G1D)			*/
	{	0xF0,	CPU_COM_CMD_TYPE_ONESHOT_SEND,		0,				0,						0	},	/* PC���O���M(�����R�}���h)	*/
	{	0xB1,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �����ݒ�					*/
	{	0xD5,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �v���O�����]������		*/
	{	0xD2,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �v���O�����]���J�n		*/
	{	0xD4,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �v���O�����]������		*/
	{	0xD0,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �v���O�����]���f�[�^		*/
	{	0xD1,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �v���O�����]������		*/
	{	0xD3,	CPU_COM_CMD_TYPE_RETRY,				10,				10,						0	},	/* �v���O�����]���m�F		*/
	{	0xB2,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �\���w��					*/
	{	0xB3,	CPU_COM_CMD_TYPE_RETRY,				3,				10,						0	},	/* �o�[�W����				*/
};

/************************************************************************/
/* �֐�     : cpu_com_init												*/
/* �֐���   : ����������(�v���b�g�t�H�[��/���[�U�[)						*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : ����������(�v���b�g�t�H�[��/���[�U�[)							*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void cpu_com_init( void )
{
	// �v���b�g�t�H�[���֘A�̐ݒ�
	SERIAL_EVENT_PARAM call_back = {0};
	
	call_back.rx_callback = &cpu_com_read_comp;
	call_back.tx_callback = &cpu_com_write_comp;
	call_back.err_callback = &cpu_com_error_comp;
	
	serial_init( &call_back );
	
	// ��M�f�[�^�T�C�Y�̎w��
	serial_read( ( uint8_t * )&serial_drv_rcv[ 0 ], SERIAL_DRV_RCV_SIZE );

	cpu_com_init_sub();
	
}

/************************************************************************/
/* �֐�     : cpu_com_read_comp											*/
/* �֐���   : �ǂݏo������(�v���b�g�t�H�[��)							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : �ǂݏo������(�v���b�g�t�H�[��)								*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void cpu_com_read_comp( void )
{
#if 0
	// ���̏���
	uint8_t *ke_msg;

	DTM2Wire_Data_Wr_Point++;
	serial_read( ( uint8_t * )&DTM2Wire_Data[ ( DTM2Wire_Data_Wr_Point & ( DTM2WIRE_MAX_BUF_CNT - 1 ) ) ], sizeof( uint16_t ) );
#else
	// ��M�f�[�^�T�C�Y�̎w��
	serial_read( ( uint8_t * )&serial_drv_rcv[ 0 ], SERIAL_DRV_RCV_SIZE );
	if( E_QOVR == write_ring_buf( &drv_uart0_rcv_ring, serial_drv_rcv[0] )){
		err_info(ERR_ID_CPU_COM_RCV_RING);
	}
#endif
#if 0
	// ���̏���(OS�ʒm)
	if ( _DTM2WIRE_INIT_STATE_COMPLETE == cpu_com_init_Flg ) {
	    ke_msg = ke_msg_alloc( DTM2RBLE_READ_COMPLETE, USER_MAIN_ID, USER_MAIN_ID, 0 );
    	ke_msg_send(ke_msg);
	}
#endif

}

/************************************************************************/
/* �֐�     : cpu_com_write_comp										*/
/* �֐���   : �������݊���(�v���b�g�t�H�[��)							*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : �������݊���(�v���b�g�t�H�[��)								*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void cpu_com_write_comp( void )
{
//	drv_o_port_h1d_int( OFF );
	drv_cpu_com_set_send_status( DRV_CPU_COM_STATUS_CAN_SEND );

}

/************************************************************************/
/* �֐�     : cpu_com_error_comp										*/
/* �֐���   : �ُ픭��(�v���b�g�t�H�[��)								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : �ُ픭��(�v���b�g�t�H�[��)									*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void cpu_com_error_comp( void )
{
#if 0
	// ���̏���
	if ( _RBLE_RUN_MODE_INIT == rBLE_Run_Mode_Flg ) {
		rBLE_Run_Mode_Flg = _RBLE_RUN_MODE_NORMAL;
	} else {
	}
	serial_read( ( uint8_t * )&DTM2Wire_Data[ ( DTM2Wire_Data_Wr_Point & ( DTM2WIRE_MAX_BUF_CNT - 1 ) ) ], sizeof( uint16_t ) );
#else
	// ��M�f�[�^�T�C�Y�̎w��
	serial_read( ( uint8_t * )&serial_drv_rcv[ 0 ], SERIAL_DRV_RCV_SIZE );
	err_info(ERR_ID_DRV_UART_OERR);
#endif
}


/************************************************************************/
/* ���[�U�[�A�v��														*/
/************************************************************************/

/************************************************************************/
/* �֐�     : cpu_com_init_sub											*/
/* �֐���   : ����������(���[�U�[�A�v��)								*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : ����������													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void cpu_com_init_sub(void)
{
	memset(&s_cpu_com_ds_input, 0x00, sizeof(s_cpu_com_ds_input));
	s_p_cpu_com_ds_order = NULL;

	s_cpu_com_snd_cmd = 0x00;
	memset(s_cpu_com_snd_data, 0x00, sizeof(s_cpu_com_snd_data));
	s_cpu_com_snd_size = 0;
	s_cpu_com_snd_type = 0;
	s_cpu_com_snd_retry_cnt = 0;
	s_cpu_com_snd_timeout = 0;
	s_cpu_com_snd_rensou_cnt = 0;
	
	s_cpu_com_snd_last_time = time_get_elapsed_time();
	
	s_cpu_com_snd_seq_no = 0;
//	s_cpu_com_res_seq_no = 0xFF;		//�}�X�^�[�Ŏ�M�V�[�P���X���g�p
	
	s_drv_cpu_com_snd_status = DRV_CPU_COM_STATUS_CAN_SEND;
	s_cpu_com_snd_status = CPU_COM_SND_STATUS_IDLE;
	
	memset( &s_cpu_com_rcv_msg, 0x00, sizeof(s_cpu_com_rcv_msg));
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
/* �ύX���� : 2018.05.13  Axia Soft Design ����		���ō쐬			*/
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
/* �ύX���� : 2018.05.13  Axia Soft Design ����		���ō쐬			*/
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
/* �ύX���� : 2018.05.13  Axia Soft Design ����		���ō쐬			*/
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
/* �ύX���� : 2018.05.13  Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : ��������														*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void cpu_com_rcv_proc(void)
{
	if( ON == cpu_com_analyze_msg() ){
		/* ��M�f�[�^���� */
		s_cpu_com_ds_input.rcv_cmd = s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD];
		/* �A�v���ʒm�p�̃f�[�^�Ɏ�M�f�[�^���Z�b�g */
		memcpy( s_cpu_com_ds_input.rcv_data, &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_DATA], ( s_cpu_com_rcv_msg_size- CPU_COM_MSG_SIZE_MIN ));
	}
}


/************************************************************************/
/* �֐�     : cpu_com_analyze_msg										*/
/* �֐���   : ��M���b�Z�[�W���										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : ON :1���b�Z�[�W��M										*/
/*          : OFF:����M												*/
/* �ύX���� : 2018.05.13  Axia Soft Design ����		���ō쐬			*/
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
		if( s_cpu_com_rcv_msg.pos >= sizeof(s_cpu_com_rcv_msg.buf) ){
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
				s_cpu_com_rcv_msg.pos = 0;
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_DLE;
				s_cpu_com_rcv_msg.buf[s_cpu_com_rcv_msg.pos++] = CPU_COM_CTRL_CODE_STX;
				s_cpu_com_rcv_msg.state = CPU_COM_RCV_MSG_STATE_ETX_WAIT;
			}else if(( ON == s_cpu_com_rcv_msg.last_dle_flg ) && ( CPU_COM_CTRL_CODE_ETX == rcv_data ) && ( CPU_COM_RCV_MSG_STATE_ETX_WAIT == s_cpu_com_rcv_msg.state )){
				// ETX�܂Ŏ擾
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
//	UB seq_num = 0;	// �V�[�P���X�ԍ��`�F�b�N���� ���}�X�^�[���g�p
	
	if(( CPU_COM_MSG_SIZE_MIN > s_cpu_com_rcv_msg_size ) ||
		( CPU_COM_MSG_SIZE_MAX < s_cpu_com_rcv_msg_size )){
		/* ���b�Z�[�W�T�C�Y�ُ� */
		err_info(ERR_ID_CPU_COM);
		return OFF;
	}
	/* STX,ETX,SUM�ACRC���������f�[�^�� */
	data_size = s_cpu_com_rcv_msg_size - ( CPU_COM_STX_SIZE + CPU_COM_CRC_SIZE  + CPU_COM_ETX_SIZE );
	
	/*  CRC�`�F�b�N */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size ];			/* CRC���ʃr�b�g */
	crc_rcv |= s_cpu_com_rcv_msg.buf[ CPU_COM_STX_SIZE + data_size + 1 ] << 8;	/* CRC��ʃr�b�g */
	cpu_com_crcset( &s_cpu_com_rcv_msg.buf[CPU_COM_MSG_TOP_POS_CMD], data_size, &tmp );

	if( crc_rcv != tmp ){		// CRC�ُ�
		err_info(ERR_ID_CPU_COM);
		return OFF;
	}
	
	/* �V�[�P���X�ԍ��`�F�b�N */
#if 0	// �V�[�P���X�ԍ��`�F�b�N���� ���}�X�^�[���g�p
	seq_num = s_cpu_com_rcv_msg.buf[ CPU_COM_MSG_TOP_POS_SEQ ];				/* �V�[�P���X�ԍ����ʃr�b�g */

	if( s_cpu_com_res_seq_no == seq_num ){
		return OFF;
	}
	s_cpu_com_res_seq_no = seq_num;		//�V�[�P���X�ԍ��X�V
#endif
	
	/* �`�F�b�NOK */
	return ON;
}

/************************************************************************/
/* �֐�     : cpu_com_send_proc											*/
/* �֐���   : �������M����												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.03.10  Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : �������M����													*/
/* ���M��������ё��M�X�e�[�^�X(�~�h��)�̕ύX���s��						*/
/* ��)																	*/
/* �������K�v�ȃR�}���h�́A������M�܂��̓G���[�m��܂ő��M�\��ԂƂ͂��Ȃ� */
/* �A���≞�����s�v�ȃR�}���h�́A���M�㑗�M�\��ԂƂ���B				*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
STATIC void cpu_com_send_proc(void)
{
#if 1
	UW now_time;
//	ke_time_t time;
	UB send_status;
//	UH res_seq_expect;	/* �O�񑗐M���V�[�P���XNo ���}�X�^�[���g�p
	
	/* �h���C�o�̑��M��Ԃ��擾 */
	drv_cpu_com_get_send_status( &send_status );
//	s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE;
	
	now_time = time_get_elapsed_time();
	
	switch( s_cpu_com_snd_status ){
		case CPU_COM_SND_STATUS_RCV_WAIT:
			/* ��M�҂���� */
//			res_seq_expect = s_cpu_com_snd_seq_no-1;//RD8001�폜�\��F�V�[�P���X

#if 1
			if( s_cpu_com_snd_cmd == s_cpu_com_ds_input.rcv_cmd ){
#else
			// �V�[�P���X�ԍ��`�F�b�N���� ���}�X�^�[���g�p
			if(( s_cpu_com_snd_cmd == s_cpu_com_ds_input.rcv_cmd ) &&
				( res_seq_expect == s_cpu_com_res_seq_no )){	/* ����R�}���h ���� ����V�[�P���XNo */
#endif
				/* ������M */
				s_cpu_com_snd_status = CPU_COM_SND_STATUS_COMPLETE; /* ���M���� */
				
			}else{
				/* ��M�Ȃ� */
//				time_soft_get_10ms(TIME_TYPE_10MS_CPU_COM_RETRY, &time);
				if(( now_time - s_cpu_com_snd_last_time ) >= s_cpu_com_snd_timeout ){
					err_info( ERR_ID_CPU_COM_ERR );
					if( 0 == s_cpu_com_snd_retry_cnt ){
						/* ���g���C�A�E�g(�v���I) */
						s_cpu_com_snd_status = CPU_COM_SND_STATUS_SEND_NG; /* ���g���CNG */
						err_info( ERR_ID_CPU_COM_RETRYOUT );
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
/* �ύX���� : 2018.05.13  Axia Soft Design ����		���ō쐬			*/
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
		err_info(ERR_ID_CPU_COM);
		return FALSE;
	}
	if( CPU_COM_DATA_SIZE_MAX < s_p_cpu_com_ds_order->data_size ){
		/* �f�[�^���ُ� */
		err_info(ERR_ID_CPU_COM);
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
	
	// ���M�o�b�t�@��������[DMA�]��]
	cpu_com_send_data( &s_cpu_com_snd_data[ 0 ], s_cpu_com_snd_size );

	return TRUE;
}


/************************************************************************/
/* �֐�     : cpu_com_clear_send_order									*/
/* �֐���   : ���M�v���N���A											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.05.13  Axia Soft Design ����		���ō쐬			*/
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
/* �֐�     : cpu_com_get_status										*/
/* �֐���   : CPU�X�e�[�^�X�擾											*/
/* ����     : �Ȃ�														*/
/* �߂�l   :	CPU_COM_SND_STATUS_IDLE			���M�\���			*/
/*				CPU_COM_SND_STATUS_RCV_WAIT		������M�҂����		*/
/* 				CPU_COM_SND_STATUS_RENSOU		�A�������				*/
/* 				CPU_COM_SND_STATUS_COMPLETE		���M�������			*/
/* 				CPU_COM_SND_STATUS_SEND_NG		���g���CNG				*/
/* �ύX���� : 2018.07.09 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : CPU�X�e�[�^�X�擾												*/
/************************************************************************/
/* ���ӎ��� :�Ȃ�														*/
/************************************************************************/
// DS�f�[�^�Ŋm�F�o����̂ō폜
#if 0
UB cpu_com_get_status(void)
{
	/* �h���C�o�֐����R�[�� */
	return s_cpu_com_snd_status;
}
#endif


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
	UB cpu_com_buf_org[CPU_COM_MSG_SIZE_MAX];
	int i = 0;
	UH extension_size = 0;
	
	// �ُ�`�F�b�N
	if(size > CPU_COM_MSG_SIZE_MAX ){
		err_info(ERR_ID_LOGIC);
		return ( size + extension_size );
	}
	
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
	
	// �ُ�`�F�b�N
	if(( size + extension_size ) > CPU_COM_BUF_SIZE_MAX ){
		err_info(ERR_ID_LOGIC);
	}
	
	return ( size + extension_size );
}

/************************************************************************/
/* �֐�     : cpu_com_send_data											*/
/* �֐���   : ���M�T�u�֐�												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : ���M�T�u�֐�													*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void cpu_com_send_data( UB* data, UH len )
{
	drv_o_port_h1d_int( ON );

	DRV_CPU_COM_H1D_WAKEUP_WAIT
	
	drv_cpu_com_set_send_status( DRV_CPU_COM_STATUS_SENDING );
	serial_write( data, len );
}

/************************************************************************/
/* �֐�     : cpu_com_get_can_sleep										*/
/* �֐���   : �X���[�v�\��Ԏ擾										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.09.07 Axia Soft Design ����		���ō쐬			*/
/************************************************************************/
/* �@�\ : �X���[�v�\��Ԏ擾											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
UB cpu_com_get_can_sleep( void )
{
	UB ret = ON;
	UB send_status;

	/* �h���C�o�̑��M��Ԃ��擾 */
	drv_cpu_com_get_send_status( &send_status );
	
	// ���M���̓X���[�v�o���Ȃ�
	if( DRV_CPU_COM_STATUS_SENDING == send_status ){
		ret = OFF;
	}else{
		// �v���b�g�t�H�[����DMA�������݊����Ȃ̂ő��M��ԃ��W�X�^���`�F�b�N
		if( BIT06 & SSR00 ){
			// ���M��
			ret = OFF;
		}
	}
	
	return ret;
}


/************************************************************************/
/* �֐�     : drv_cpu_com_set_send_status								*/
/* �֐���   : CPU�ԒʐM���M��ԃZ�b�g									*/
/* ����     : UB status : �X�V������									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.04.25 Axia Soft Design ���� ��	���ō쐬			*/
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
/* �ύX���� : 2018.04.25 Axia Soft Design ���� ��	���ō쐬			*/
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
/* �Ȃ�																	*/
/************************************************************************/
void drv_uart0_data_init( void )
{
	memset( &serial_drv_rcv[0], 0x00, sizeof(serial_drv_rcv) );
	
	/* ���M�����O�o�b�t�@������ */
	// DMA�]���ňꊇ���M����ׂɃ����O�o�b�t�@�͕s�v
//	memset( &drv_uart0_send_buf[0], 0x00, sizeof(drv_uart0_send_buf) );
//	ring_buf_init( &drv_uart0_send_ring, &drv_uart0_send_buf[0], DRV_UART0_SND_RING_LENGTH );
	
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
// ���M��DMA�]���ňꊇ���M����ׂɃ����O�o�b�t�@�͕s�v
#if 0	// ���g�p
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

