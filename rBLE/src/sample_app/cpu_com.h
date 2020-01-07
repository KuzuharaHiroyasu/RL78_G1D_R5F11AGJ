/************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[									*/
/* �t�@�C����   : cpu_com.h												*/
/* �@�\         : CPU�ԒʐM�~�h��										*/
/* �ύX����     : 2018.04.10 Axia Soft Design ����		���ō쐬		*/
/* ���ӎ���     : �Ȃ�                                                  */
/************************************************************************/
#ifndef _CPU_COM_H_
#define _CPU_COM_H_


/******************/
/* �萔�}�N����` */
/******************/
#define CPU_COM_SND_DATA_SIZE_STATUS_REQ			4				/* �X�e�[�^�X�v��			*/
#define CPU_COM_SND_DATA_SIZE_MODE_CHG				1				/* ��ԕύX(G1D)			*/
#define CPU_COM_SND_DATA_SIZE_PC_LOG				10				/* PC���O���M(�����R�}���h)	*/
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			15				/* �Z���T�[�f�[�^�T�C�Y		*/
#define CPU_COM_SND_DATA_SIZE_DATE_SET				7				/* �����ݒ�					*/
#define CPU_COM_SND_DATA_SIZE_DISP_ORDER			1				/* �\���w��					*/
#define CPU_COM_SND_DATA_SIZE_SENSING_ORDER			1				/* �Z���V���O�w��			*/

/* �f�[�^���͈� */
#define CPU_COM_DATA_SIZE_MAX					( 20 )		/* �f�[�^�̍ő咷 */
															/* �ő�f�[�^ �\���f�[�^�X�V �ڍ׃X�����v�O���t ���No(2)+�f�[�^��(2)+�f�[�^(308) */
															
															
// �e�f�[�^�T�C�Y
#define CPU_COM_CMD_SIZE						(1)			/* �R�}���h�f�[�^�T�C�Y 1byte */
#define CPU_COM_SEQ_SIZE						(1)			/* SEQNo�f�[�^�T�C�Y 1byte */
#define CPU_COM_CRC_SIZE						(2)			/* CRC�f�[�^�T�C�Y 2byte */
#define CPU_COM_STX_SIZE						(2)			/* STX�f�[�^�T�C�Y 2byte */
#define CPU_COM_ETX_SIZE						(2)			/* ETX�f�[�^�T�C�Y 2byte */

/* ���b�Z�[�W�̍ŏ� */
#define CPU_COM_MSG_SIZE_MIN					( CPU_COM_CMD_SIZE + \
												  CPU_COM_SEQ_SIZE + \
												  CPU_COM_CRC_SIZE + \
												  CPU_COM_STX_SIZE + \
												  CPU_COM_ETX_SIZE )


/* ���b�Z�[�W�̍ő�[���󋤒�] ���w�b�_�{�ő�f�[�^,DLE�L���Ȃ� */
#define CPU_COM_MSG_SIZE_MAX					(CPU_COM_DATA_SIZE_MAX + CPU_COM_MSG_SIZE_MIN)

/* �o�b�t�@�̍ő�[���󋤒�] ��DLE�L���l����2�{,�����O�o�b�t�@�p��+1 */
#define CPU_COM_BUF_SIZE_MAX					(( CPU_COM_MSG_SIZE_MAX * 2 ) + 1 )

/* ���M�X�e�[�^�X */
#define CPU_COM_SND_STATUS_IDLE					0			/* ���M�\��� */
#define CPU_COM_SND_STATUS_RCV_WAIT				1			/* ������M�҂���� */
#define CPU_COM_SND_STATUS_RENSOU				2			/* �A������� */
#define CPU_COM_SND_STATUS_COMPLETE				3			/* ���M������� */
#define CPU_COM_SND_STATUS_SEND_NG				4			/* ���g���CNG */
#define CPU_COM_SND_STATUS_SEND_WAIT			5			/* ���M�҂� */

/* CPU�ԒʐM��M�҂����M���N�G�X�g���� */
#define CPU_COM_SND_RES_OK						0			/* ���������M */
#define CPU_COM_SND_RES_BUSY_NG					1			/* CPU�ԒʐMBUSY���(���M��or��M�҂�)NG */
#define CPU_COM_SND_RES_RETRY_OUT_NG			2			/* ���g���C�A�E�g */
#define CPU_COM_SND_RES_COM_NG					3			/* ���M�R�}���hNG */


/* CPU�ԒʐM �R�}���h��� */
/* �v���E�����̃Z�b�g */
typedef enum _CPU_COM_CMD_ID{
	CPU_COM_CMD_NONE=0,							/* �yCPU�ԒʐM�R�}���h�z�R�}���h����				*/
	CPU_COM_CMD_STATUS_REQ,						/* �yCPU�ԒʐM�R�}���h�z�X�e�[�^�X�v��				*/
	CPU_COM_CMD_SENSOR_DATA,					/* �yCPU�ԒʐM�R�}���h�z�Z���T�[�f�[�^�X�V			*/
	CPU_COM_CMD_SENSING_ORDER,					/* �yCPU�ԒʐM�R�}���h�z�Z���V���O�w��				*/
	CPU_COM_CMD_MODE_CHG,						/* �yCPU�ԒʐM�R�}���h�z��ԕύX(G1D)				*/
	CPU_COM_CMD_PC_LOG,							/* �yCPU�ԒʐM�R�}���h�zPC���O���M(�����R�}���h)	*/
	CPU_COM_CMD_DATE_SET,						/* �yCPU�ԒʐM�R�}���h�z�����ݒ�					*/

	CPU_COM_CMD_PRG_DOWNLORD_READY,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	CPU_COM_CMD_PRG_DOWNLORD_START,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n		*/
	CPU_COM_CMD_PRG_DOWNLORD_ERASE,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	CPU_COM_CMD_PRG_DOWNLORD_DATA,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���f�[�^	*/
	CPU_COM_CMD_PRG_DOWNLORD_RESLUT,			/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	CPU_COM_CMD_PRG_DOWNLORD_CHECK,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���m�F		*/
	CPU_COM_CMD_DISP_ORDER,						/* �yCPU�ԒʐM�R�}���h�z�\���w��				*/
	CPU_COM_CMD_VERSION,						/* �yCPU�ԒʐM�R�}���h�z�o�[�W����				*/
	CPU_COM_CMD_MAX								/* �yCPU�ԒʐM�R�}���h�z�ő�l					*/
}CPU_COM_CMD_ID;



/* �R�}���h�^�C�v */
/* ���}�X�^�[��p */
#define CPU_COM_CMD_TYPE_ONESHOT_SEND			0					/* �P�����M�R�}���h */
#define CPU_COM_CMD_TYPE_RETRY					1					/* ���g���C */
#define CPU_COM_CMD_TYPE_RENSOU					2					/* �A�� */
#define CPU_COM_CMD_TYPE_ONESHOT_RCV			3					/* �P����M�R�}���h */





/* =====�ʐM�d�l�֘A===== */
// �ʐM�f�[�^�ɉ����鐧��R�[�h
#define CPU_COM_CTRL_CODE_STX			0x02					/* STX */
#define CPU_COM_CTRL_CODE_ETX			0x03					/* ETX */
#define CPU_COM_CTRL_CODE_DLE			0x10					/* DLE */

/* �ǂݏo��(������A�t�@�C��)���� */
typedef enum{
	CPU_COM_RCV_MSG_STATE_STX_WAIT = 0,			/* STX�҂� */
	CPU_COM_RCV_MSG_STATE_ETX_WAIT,				/* ETX�҂� */
}CPU_COM_RCV_MSG_STATE;

// ���b�Z�[�W�z��̓Y����
#define CPU_COM_MSG_TOP_POS_CMD					2				/* �R�}���h */
#define CPU_COM_MSG_TOP_POS_SEQ					3				/* �V�[�P���X */
#define CPU_COM_MSG_TOP_POS_DATA				4				/* �f�[�^�� */




// =========================
// �f�[�^���
// =========================
// �\���w��
#define CPU_COM_DISP_ORDER_SELF_CHECK_NON	0	// ���Ȑf�f���\��(�\���Ȃ�)
#define CPU_COM_DISP_ORDER_SELF_CHECK_ERR	1	// ���Ȑf�f���\��(�ُ�)
#define CPU_COM_DISP_ORDER_SELF_CHECK_FIN	2	// ���Ȑf�f���\��(����)

#define CPU_COM_DISP_TRG_LONG_SW_RECEPTION	3	// SW������(��t)



// �h���C�o�֘A�̒�`
/* ���M�X�e�[�^�X */
#define DRV_CPU_COM_STATUS_CAN_SEND		(0)			/* ���M�\��� */
#define DRV_CPU_COM_STATUS_SENDING		(1)			/* ���M�� */
/* ��M�X�e�[�^�X */
#define DRV_CPU_COM_STATUS_NOT_RCV		(0)			/* ��M�Ȃ� */
//#define DRV_CPU_COM_STATUS_CAN_RCV		(1)			/* ��M�\��� */
#define DRV_CPU_COM_STATUS_RECEIVING	(1)			/* ��M�� */
#define DRV_CPU_COM_STATUS_RECEIVE_COMP	(2)			/* ��M���� */

/* H1D�N���҂� �������Ă��������O�ׂ̈ɓ���Ă��� */
#define DRV_CPU_COM_H1D_WAKEUP_WAIT		WAIT_50US();WAIT_10US();WAIT_5US();WAIT_1US();	//66us(65us+83.3333ns�~7�N���b�N)


/******************/
/*   �\���̒�`   */
/******************/
typedef struct _T_CPU_COM_CMD_INFO {
	UB cmd;					/* �R�}���h[�}�X�^�[/�X���[�u] */
	UB cmd_type;			/* �R�}���h�^�C�v[�}�X�^�[] */
	UB retry_cnt;			/* ���g���C��[�}�X�^�[] */
	UW retry_time;			/* ���g���C�Ԋu[�}�X�^�[] */
	UB rensou_cnt;			/* �A����[�}�X�^�[] */
} T_CPU_COM_CMD_INFO;
// ���M�p�R�}���h���

typedef struct{
	UB buf[CPU_COM_MSG_SIZE_MAX];		/* �o�b�t�@[DLE�Ȃ�] */
	UH pos;								/* ��M�ʒu */
	UB last_dle_flg;					/* �O���M�f�[�^��DLE */
	UB state;							/* �O���M�f�[�^��DLE */
}CPU_COM_ANA_RCV_MSG;
// ��͗p��M���b�Z�[�W



/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void cpu_com_init( void );
extern void cpu_com_read_comp( void );
extern void cpu_com_write_comp( void );
extern void cpu_com_error_comp( void );
extern void cpu_com_proc(void);
extern UB cpu_com_get_status(void);
extern UH cpu_com_dle_extension( UB* data, UH size );
extern UB cpu_com_get_can_sleep( void );
extern void drv_cpu_com_set_send_status( UB status );
extern void drv_cpu_com_get_send_status( UB *status );
extern void drv_uart0_data_init( void );


#endif
/************************************************************************/
/* END OF TEXT															*/
/************************************************************************/