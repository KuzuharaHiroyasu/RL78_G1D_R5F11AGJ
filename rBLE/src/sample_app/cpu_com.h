/**
 ****************************************************************************************
 *
 * @file		cpu_com.h
 *
 * @brief Direct Test Mode 2Wire UART Driver.
 *
 * Copyright(C) 2013-2014 Renesas Electronics Corporation
 *
 * $Rev: v1.01.00
 *
 ****************************************************************************************
 */

#ifndef	__CPU_COM_INC__
#define	__CPU_COM_INC__

#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */
#include	"ke_task.h"
#endif


/**************************** Structure Declaration ***********************************/
/**************************** PROTOTYPE Declaration ***********************************/
void cpu_com_init( void );
//bool DTM2Wire_Check( void );
//bool DTM2Wire_Start( void );
void cpu_com_read_comp( void );
void cpu_com_write_comp( void );
void cpu_com_error_comp( void );
extern void codeptr app_evt_usr0(void); 

// ========================================
// H1D���ʃR�[�h
// ========================================



//RD8001�b��FDS���ǂ����邩����





/******************/
/* �萔�}�N����` */
/******************/
#define CPU_COM_SND_DATA_SIZE_STATUS_REQ			4				/* �X�e�[�^�X�v��			*/
#define CPU_COM_SND_DATA_SIZE_MODE_CHG				1				/* ��ԕύX(G1D)			*/
#define CPU_COM_SND_DATA_SIZE_PC_LOG				7				/* PC���O���M(�����R�}���h)	*/
#define CPU_COM_SND_DATA_SIZE_SENSOR_DATA			11				/* �Z���T�[�f�[�^�T�C�Y */

/* �f�[�^���͈� */
#define CPU_COM_DATA_SIZE_MAX					(262+1)		/* �f�[�^�̍ő咷 */
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


/* ���b�Z�[�W�̍ő� �R�}���h(1)+SEQ(2)+�f�[�^(262)+SUM(2)+CRC(2) */
#define CPU_COM_MSG_SIZE_MAX					(CPU_COM_DATA_SIZE_MAX + CPU_COM_MSG_SIZE_MIN)

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

/* CPU�ԒʐM�C�x���g�g���KID */
typedef enum _CPU_COM_EVENT_TRIGGER_ID {
	CPU_COM_EVENT_TRG_ID_NONE					=0	,	/* �C�x���g���� */
	CPU_COM_EVENT_TRG_ID_NORMAL						,	/* [�ʏ�\��]�ʏ�\�� */
	CPU_COM_EVENT_TRG_ID_ATARI						,	/* [�ʏ�\��]�哖��J�n */
	CPU_COM_EVENT_TRG_ID_KAKUHEN					,	/* [�ʏ�\��]�m�ϊJ�n */
	CPU_COM_EVENT_TRG_ID_BB							,	/* [�ʏ�\��]BB�J�n */
	CPU_COM_EVENT_TRG_ID_RB							,	/* [�ʏ�\��]RB�J�n */
	CPU_COM_EVENT_TRG_ID_ART						,	/* [�ʏ�\��]ART�J�n */
	CPU_COM_EVENT_TRG_ID_ATARI_END					,	/* [�ʏ�\��]�哖��I�� */
	CPU_COM_EVENT_TRG_ID_KAKUHEN_END				,	/* [�ʏ�\��]�m�ϏI�� */
	CPU_COM_EVENT_TRG_ID_BB_END						,	/* [�ʏ�\��]BB�I�� */
	CPU_COM_EVENT_TRG_ID_RB_END						,	/* [�ʏ�\��]RB�I�� */
	CPU_COM_EVENT_TRG_ID_ART_END					,	/* [�ʏ�\��]ART�I�� */
	CPU_COM_EVENT_TRG_ID_HIGH_DATA					,	/* [�ʏ�\��]�ߋ��ō��f�[�^ */
	CPU_COM_EVENT_TRG_ID_DAY_BEFORE_DATA			,	/* [�ʏ�\��]�ߋ��f�[�^ */
	CPU_COM_EVENT_TRG_ID_HIST_DATA					,	/* [�ʏ�\��]�����f�[�^ */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_HIST_DATA_NORMAL			,	/* [�ʏ�\��]�����f�[�^(�ʏ�^�C�v) */ /* �ʏ�̃X�����v�O���t�^�C�v�ؑ֗p */
	CPU_COM_EVENT_TRG_ID_HIST_DATA_SLUMP			,	/* [�ʏ�\��]�����f�[�^(�X�����v�O���t�^�C�v) */ /* �ʏ�̃X�����v�O���t�^�C�v�ؑ֗p */
	CPU_COM_EVENT_TRG_ID_GRAPH_DATA					,	/* [�ʏ�\��]�X�����v�O���t */
	CPU_COM_EVENT_TRG_ID_RANKING_DATA				,	/* [�ʏ�\��]�����L���O�f�[�^ */
	CPU_COM_EVENT_TRG_ID_OTHER_DAI_DATA				,	/* [�ʏ�\��]���ӑ�f�[�^ */
	CPU_COM_EVENT_TRG_ID_KISYU_SPEC_DATA			,	/* [�ʏ�\��]�@��X�y�b�N */
	CPU_COM_EVENT_TRG_ID_OSHIRASE_DATA				,	/* [�ʏ�\��]���m�点 */
	CPU_COM_EVENT_TRG_ID_ZONE_DATA					,	/* [�ʏ�\��]�]�[���f�[�^ */
	CPU_COM_EVENT_TRG_ID_CALENDER_DATA				,	/* [�ʏ�\��]�J�����_�[ */
	CPU_COM_EVENT_TRG_ID_PRIVATE_DATA				,	/* [�ʏ�\��]�v���C�x�[�g�f�[�^ */
	CPU_COM_EVENT_TRG_ID_ONETOUCH_MENU				,	/* [�ʏ�\��]�����^�b�`���j���[ */
	CPU_COM_EVENT_TRG_ID_ONETOUCH_ORDER				,	/* [�ʏ�\��]�����^�b�`������� */
	CPU_COM_EVENT_TRG_ID_SADAMA_SAMAI				,	/* [�ʏ�\��]���ʁE�����\�� */
	CPU_COM_EVENT_TRG_ID_TEST_PLAY					,	/* [�ʏ�\��]�e�X�g�ł����[�h */
	CPU_COM_EVENT_TRG_ID_NORMAL_RETURN				,	/* [�ʏ�\��]�ʏ�\���ɖ߂� */
	CPU_COM_EVENT_TRG_ID_TROUBLE					,	/* [�g���u��]�g���u�� */
	CPU_COM_EVENT_TRG_ID_TROUBLE_TO					,	/* [�g���u��]�g���u���^�C���I�[�o�[ */
	CPU_COM_EVENT_TRG_ID_SERVICE1					,	/* [�T�[�r�X]�T�[�r�X1 */
	CPU_COM_EVENT_TRG_ID_SERVICE2					,	/* [�T�[�r�X]�T�[�r�X2 */
	CPU_COM_EVENT_TRG_ID_LUNCH						,	/* [�����`]�����` */
	CPU_COM_EVENT_TRG_ID_UNIT_LUNCH					,	/* [�����`]���j�b�g�����` */
	CPU_COM_EVENT_TRG_ID_LUNCH_TO					,	/* [�����`]�����`�^�C���I�[�o�[ */
	CPU_COM_EVENT_TRG_ID_UNIT_LUNCH_TO				,	/* [�����`]���j�b�g�����`�^�C���I�[�o�[ */
	CPU_COM_EVENT_TRG_ID_DATA_RANKING_CHG_ATARI		,	/* [�f�[�^���]�����L���O�哖�� */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_DATA_RANKING_CHG_KEIZOKU	,	/* [�f�[�^���]�����L���O�p�� */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_DATA_RANKING_CHG_KAKUTOKU	,	/* [�f�[�^���]�����L���O�l�� */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_DATA_CLR_READY				,	/* [�f�[�^����]�f�[�^�N���A���� */
	CPU_COM_EVENT_TRG_ID_DATA_CLR_RUN				,	/* [�f�[�^����]�f�[�^�N���A���s */
	CPU_COM_EVENT_TRG_ID_DATA_ALL_CLR_READY			,	/* [�f�[�^����]�f�[�^�I�[���N���A���� */
	CPU_COM_EVENT_TRG_ID_DATA_ALL_CLR_RUN			,	/* [�f�[�^����]�f�[�^�I�[���N���A���s */
	CPU_COM_EVENT_TRG_ID_FEED_DAY_READY				,	/* [�f�[�^����]�����菀�� */
	CPU_COM_EVENT_TRG_ID_FEED_DAY_RUN				,	/* [�f�[�^����]��������s */
	CPU_COM_EVENT_TRG_ID_RETURN_DAY_READY			,	/* [�f�[�^����]���߂����� */
	CPU_COM_EVENT_TRG_ID_RETURN_DAY_RUN				,	/* [�f�[�^����]���߂����s */
	CPU_COM_EVENT_TRG_ID_NIGHT_READY				,	/* [�V���[�g�J�b�g]��ԊĎ����� */
	CPU_COM_EVENT_TRG_ID_BONUS_START_CLR			,	/* [�V���[�g�J�b�g]�{�[�i�X�ԃX�^�[�g�񐔃N���A */
	CPU_COM_EVENT_TRG_ID_BONUS_START_CLR_LINE		,	/* [�V���[�g�J�b�g]���{�[�i�X�ԃX�^�[�g�񐔃N���A */
	CPU_COM_EVENT_TRG_ID_HIGH_KEEP_DATA_CLR			,	/* [�V���[�g�J�b�g]�ߋ��ō��ȊO�f�[�^�N���A */
	CPU_COM_EVENT_TRG_ID_GRAPH_DATA_CLR				,	/* [�V���[�g�J�b�g]�O���t�f�[�^�N���A */
	CPU_COM_EVENT_TRG_ID_DEMO_MODE					,	/* [�V���[�g�J�b�g]�f�����[�h�ڍs */
	CPU_COM_EVENT_TRG_ID_ART_SET					,	/* [�V���[�g�J�b�g]ART�ݒ� */
	CPU_COM_EVENT_TRG_ID_DATA_SET_MODE				,	/* [�f�[�^�ł����݃��[�h]�f�[�^�ł����݃��[�h�ڍs */
	CPU_COM_EVENT_TRG_ID_DATA_SET_HIGH				,	/* [�f�[�^�ł����݃��[�h]�ߋ��ō��f�[�^ */
	CPU_COM_EVENT_TRG_ID_DATA_SET_DAY				,	/* [�f�[�^�ł����݃��[�h]�ߋ��f�[�^ */
	CPU_COM_EVENT_TRG_ID_ERR_TROUBLE_MODE			,	/* [�G���[/�g���u���\��]�G���[/�g���u���\���ڍs */
	CPU_COM_EVENT_TRG_ID_ALL_ON						,	/* [�S�����[�h]�S�����[�h */
	CPU_COM_EVENT_TRG_ID_VERSION					,	/* [�o�[�W�����\��]�o�[�W�����\�� */
	CPU_COM_EVENT_TRG_ID_NEWS_CHK_MODE				,	/* [�j���[�X���e�m�F���[�h]�j���[�X���e�m�F���[�h�ڍs */
	CPU_COM_EVENT_TRG_ID_ADDR_CLR					,	/* [�A�h���X�ݒ�]�A�h���X��񏉊��� */
	CPU_COM_EVENT_TRG_ID_ADDR_SET_HOST				,	/* [�A�h���X�ݒ�]�A�h���X�ݒ�(�z�X�g) */
	CPU_COM_EVENT_TRG_ID_ADDR_SET_SLV				,	/* [�A�h���X�ݒ�]�A�h���X�ݒ�(�X���[�u) */
	CPU_COM_EVENT_TRG_ID_ADDR_SET_HOST_MANU			,	/* [�A�h���X�ݒ�]�蓮�A�h���X�擾(�z�X�g) */
	CPU_COM_EVENT_TRG_ID_ADDR_SET_SLV_MANU			,	/* [�A�h���X�ݒ�]�蓮�A�h���X�擾(�X���[�u) */
	CPU_COM_EVENT_TRG_ID_LAMP_CHK					,	/* [�����v����m�F���[�h]�����v����m�F���[�h�J�� */
	CPU_COM_EVENT_TRG_ID_LAMP_CHK_CHG				,	/* [�����v����m�F���[�h]�����v����쓮�؂�ւ� */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_INPUT			,	/* [�����ݒ胂�[�h]�����ݒ���̓��[�h�ڍs */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_ALONE_SEL		,	/* [�����ݒ胂�[�h]�ʐݒ��M�I�� */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_ALONE_ON			,	/* [�����ݒ胂�[�h]�ʐݒ��M�L�� */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_ALONE_RCV		,	/* [�����ݒ胂�[�h]�ݒ��M */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_INPUT_VALUE		,	/* [�����ݒ胂�[�h]�C�Ӑݒ�l���̓��[�h */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ4_INPUT		,	/* [�����ݒ胂�[�h]�Z�L�����e�BID(4��)���� */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ4_INPUT_RE	,	/* [�����ݒ胂�[�h]�Z�L�����e�BID(4��)����(2���) */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ6_INPUT		,	/* [�����ݒ胂�[�h]�Z�L�����e�BID(6��)���� */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ6_INPUT_RE	,	/* [�����ݒ胂�[�h]�Z�L�����e�BID(6��)����(2���) */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_INPUT_ERR		,	/* [�����ݒ胂�[�h]���̓G���[ */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEND_READY		,	/* [�����ݒ胂�[�h]���M���[�h */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_CHK				,	/* [�����ݒ胂�[�h]�m�F���[�h */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_CLEAR			,	/* [�����ݒ胂�[�h]�����ݒ�I�[���N���A */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_CLEAR_RUN		,	/* [�����ݒ胂�[�h]�����ݒ�I�[���N���A���s */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_VERSION			,	/* [�����ݒ胂�[�h]�o�[�W�����\�� */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_SEQ_CHK			,	/* [�����ݒ胂�[�h]�Z�L�����e�BID�m�F */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_RUN				,	/* [�����ݒ胂�[�h]���s */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_ALONE_SEND		,	/* [�����ݒ胂�[�h]�P�̏����ݒ著�M */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_MAINTE_1			,	/* [�����ݒ胂�[�h]�����e�i���X���[�h1 */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_MAINTE_2			,	/* [�����ݒ胂�[�h]�����e�i���X���[�h2 */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_INIT_CONF_RETURN			,	/* [�����ݒ胂�[�h]���̓��[�h�ɖ߂� */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_PASS				,	/* [���Ȑf�f���[�h]�p�X���[�h���͉�ʈڍs */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_START				,	/* [���Ȑf�f���[�h]�N����ʈڍs */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_NEXT				,	/* [���Ȑf�f���[�h]���̉�� */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_ALL_ON			,	/* [���Ȑf�f���[�h]�_���`�F�b�N(�S�_��) */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_ALL_RED			,	/* [���Ȑf�f���[�h]�_���`�F�b�N(�ԓ_��) */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_ALL_BLUE			,	/* [���Ȑf�f���[�h]�_���`�F�b�N(�_��) */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_ALL_GREEN			,	/* [���Ȑf�f���[�h]�_���`�F�b�N(�Γ_��) */
	CPU_COM_EVENT_TRG_ID_SELF_CHK_SW_CHECK			,	/* [���Ȑf�f���[�h]SW���̓`�F�b�N */
	CPU_COM_EVENT_TRG_ID_PRG_DL_PASS				,	/* [�v���O�����]��]�p�X���[�h���͉�ʈڍs */
	CPU_COM_EVENT_TRG_ID_PRG_DL_BOOT_CHG			,	/* [�v���O�����]��]�����J�n��ʈڍs */
	CPU_COM_EVENT_TRG_ID_PRG_DL_ERASE				,	/* [�v���O�����]��]�C���[�X����ʈڍs */
	CPU_COM_EVENT_TRG_ID_PRG_DL_TRANS_RUN			,	/* [�v���O�����]��]�]������ʈڍs */
	CPU_COM_EVENT_TRG_ID_PRG_DL_TRANS_END			,	/* [�v���O�����]��]�]��������ʈڍs */
	CPU_COM_EVENT_TRG_ID_PRG_DL_CHECK_SUM			,	/* [�v���O�����]��]�`�F�b�N�T���ƍ���ʈڍs */
	CPU_COM_EVENT_TRG_ID_PRG_DL_NG					,	/* [�v���O�����]��]�������s��ʈڍs */
	CPU_COM_EVENT_TRG_ID_IMAGE_SEND_START			,	/* [�摜�]��]�摜�]���J�n */				/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_MODE				,	/* [SD�摜����]SD�摜������ʈڍs */		/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_RUN				,	/* [SD�摜�]��]�摜�]���J�n */				/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_ERR				,	/* [SD�摜�]��]�A�N�Z�X�G���[ */			/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_MAINTE_MODE				,	/* [SD�摜�]��]�����e�i���X���[�h */		/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_OK				,	/* [SD�摜�]��]�摜�������� */				/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_IMAGE_NG				,	/* [SD�摜�]��]�摜�������s */				/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_PRG_DL					,	/* [SD�����e�i���X]�v���O�����]�� */		/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_DEF_IMAGE				,	/* [SD�����e�i���X]�f�t�H���g�摜���� */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_SH_PRG_DL				,	/* [SD�����e�i���X]SH�v���O�����]�� */		/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_MAINTE_ERR				,	/* [SD�����e�i���X]�A�N�Z�X�G���[ */		/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE				,	/* [SD�f�[�^�ڍs]SD�f�[�^�ڍs���[�h */		/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_CHG			,	/* [SD�f�[�^�ڍs]SD�f�[�^�ڍs�\���؂�ւ� */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_DUMMY			,	/* [SD�f�[�^�ڍs]SD�f�[�^�ڍs�_�~�[ �� ���g�p�g���K */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_DATA			,	/* [SD�f�[�^�ڍs]�f�[�^ */					/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_PICT			,	/* [SD�f�[�^�ڍs]�摜 */					/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_SD_DATA_MOVE_ALL			,	/* [SD�f�[�^�ڍs]�S�� */					/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_MODE_MENU		,	/* [USB�����e�i���X]�������J�[�h�����e�i���X���[�h���j���[��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_RX_RUN	,	/* [USB�����e�i���X]�v���O�����������[�h(RX)�@������������� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_RX_ERR	,	/* [USB�����e�i���X]�v���O�����������[�h(RX)�@�A�N�Z�X���s��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_RX_OK	,	/* [USB�����e�i���X]�v���O�����������[�h(RX)�@����������� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_RX_NG	,	/* [USB�����e�i���X]�v���O�����������[�h(RX)�@�������s��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_AN_RUN	,	/* [USB�����e�i���X]�v���O�����������[�h(android)�@������������� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_AN_ERR	,	/* [USB�����e�i���X]�v���O�����������[�h(android)�@�A�N�Z�X���s��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_AN_OK	,	/* [USB�����e�i���X]�v���O�����������[�h(android)�@����������� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_PRG_DL_AN_NG	,	/* [USB�����e�i���X]�v���O�����������[�h(android)�@�������s��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_MENU	,	/* [USB�����e�i���X]�摜�]�����[�h�@�摜�I����� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_DISP	,	/* [USB�����e�i���X]�摜�]�����[�h�@�I���摜�m�F��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_RUN	,	/* [USB�����e�i���X]�摜�]�����[�h�@�摜������������� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_ERR	,	/* [USB�����e�i���X]�摜�]�����[�h�@�摜���������A�N�Z�X���s��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_OK		,	/* [USB�����e�i���X]�摜�]�����[�h�@�摜��������������� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_IMAGE_DL_NG		,	/* [USB�����e�i���X]�摜�]�����[�h�@�摜�����������s��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_ONEMEMO_MENU	,	/* [USB�����e�i���X]�����������[�h�@���j���[��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_ONEMEMO_READ	,	/* [USB�����e�i���X]�����������[�h�@�ǂݍ��݉�� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_ONEMEMO_WRITE	,	/* [USB�����e�i���X]�����������[�h�@����������� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_BACKUP_MENU		,	/* [USB�����e�i���X]�o�b�N�A�b�v���[�h�@���j���[��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_BACKUP_READ		,	/* [USB�����e�i���X]�o�b�N�A�b�v���[�h�@�o�b�N�A�b�v��� */
	CPU_COM_EVENT_TRG_ID_USB_MAINTE_BACKUP_WRITE	,	/* [USB�����e�i���X]�o�b�N�A�b�v���[�h�@���J�o����� */
	CPU_COM_EVENT_TRG_ID_HARD_CHK_MODE				,	/* [�n�[�h�������[�h]�n�[�h�����N����� */		/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_HARD_CHK_IOBOX				,	/* [�n�[�h�������[�h]���o��BOX���� */			/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_HARD_CHK_FUNC				,	/* [�n�[�h�������[�h]�t�@���N�V�������� */		/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_MODE				,	/* [�������ڍs���[�h]�������ڍs���[�h�ڍs */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_READ				,	/* [�������ڍs���[�h]�ǂݏo�� */				/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_WRITE				,	/* [�������ڍs���[�h]�������� */				/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_BACK				,	/* [�������ڍs���[�h]�o�b�N�A�b�v�������� */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_MEM_MOVE_CLEAR				,	/* [�������ڍs���[�h]�N���A */					/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_ALL_ON_ON					,	/* [�S�����[�h]�S�����[�h�_�� */
	CPU_COM_EVENT_TRG_ID_ALL_ON_OFF					,	/* [�S�����[�h]�S�����[�h���� */
	CPU_COM_EVENT_TRG_ID_MAIN_MENU_ON				,	/* [����]���C�����j���[ON */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_MAIN_MENU_OFF				,	/* [����]���C�����j���[OFF */	/* ���g�p �폜�����c�� */
	CPU_COM_EVENT_TRG_ID_MACHINE_SET_HOST			,	/* [��ԍ��ݒ�]��ԍ��ݒ�(�z�X�g) */
	CPU_COM_EVENT_TRG_ID_MACHINE_SET_SLV			,	/* [��ԍ��ݒ�]��ԍ��ݒ�(�X���[�u) */
	CPU_COM_EVENT_TRG_ID_STARTUP_FIN				,	/* [�N���ʒm]�N�������ʒm */
	CPU_COM_EVENT_TRG_ID_MAX							/* �ő�l */
}CPU_COM_EVENT_TRIGGER_ID;

/* CPU�ԒʐM�\���f�[�^�X�V�g���KID */
typedef enum _CPU_COM_DATA_TRIGGER_ID {
	CPU_COM_DATA_TRG_ID_NONE=0,					/* �f�[�^�X�V���� */
	CPU_COM_DATA_TRG_ID_COM_ADDR,				/* �ʐM�A�h���X */
	CPU_COM_DATA_TRG_ID_PARA_A,					/* �����ݒ�A���� */
	CPU_COM_DATA_TRG_ID_PARA_B,					/* �����ݒ�B���� */
	CPU_COM_DATA_TRG_ID_PARA_C,					/* �����ݒ�C���� */
	CPU_COM_DATA_TRG_ID_PARA_D,					/* �����ݒ�D���� */
	CPU_COM_DATA_TRG_ID_PARA_F,					/* �����ݒ�F���� */
	CPU_COM_DATA_TRG_ID_PARA_H,					/* �����ݒ�H���� */
	CPU_COM_DATA_TRG_ID_PARA_J,					/* �����ݒ�J���� */
	CPU_COM_DATA_TRG_ID_PARA_L,					/* �����ݒ�L���� */
	CPU_COM_DATA_TRG_ID_PARA_P,					/* �����ݒ�P���� */
	CPU_COM_DATA_TRG_ID_PARA_S,					/* �����ݒ�S���� */
	CPU_COM_DATA_TRG_ID_PARA_OPT,				/* �����ݒ�C�Ӑݒ�l */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_00,			/* �X�����v�O���t0���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_00,		/* �X�����v�O���t0���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_01,			/* �X�����v�O���t1���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_01,		/* �X�����v�O���t1���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_02,			/* �X�����v�O���t2���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_02,		/* �X�����v�O���t2���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_03,			/* �X�����v�O���t3���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_03,		/* �X�����v�O���t3���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_04,			/* �X�����v�O���t4���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_04,		/* �X�����v�O���t4���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_05,			/* �X�����v�O���t5���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_05,		/* �X�����v�O���t5���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_06,			/* �X�����v�O���t6���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_06,		/* �X�����v�O���t6���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_07,			/* �X�����v�O���t7���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_07,		/* �X�����v�O���t7���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_08,			/* �X�����v�O���t8���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_08,		/* �X�����v�O���t8���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_09,			/* �X�����v�O���t9���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_09,		/* �X�����v�O���t9���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_10,			/* �X�����v�O���t10���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_10,		/* �X�����v�O���t10���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_11,			/* �X�����v�O���t11���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_11,		/* �X�����v�O���t11���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_12,			/* �X�����v�O���t12���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_12,		/* �X�����v�O���t12���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_13,			/* �X�����v�O���t13���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_13,		/* �X�����v�O���t13���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_14,			/* �X�����v�O���t14���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_14,		/* �X�����v�O���t14���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_15,			/* �X�����v�O���t15���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_15,		/* �X�����v�O���t15���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_16,			/* �X�����v�O���t16���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_16,		/* �X�����v�O���t16���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_17,			/* �X�����v�O���t17���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_17,		/* �X�����v�O���t17���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_18,			/* �X�����v�O���t18���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_18,		/* �X�����v�O���t18���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_19,			/* �X�����v�O���t19���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_19,		/* �X�����v�O���t19���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_20,			/* �X�����v�O���t20���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_20,		/* �X�����v�O���t20���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_21,			/* �X�����v�O���t21���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_21,		/* �X�����v�O���t21���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_22,			/* �X�����v�O���t22���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_22,		/* �X�����v�O���t22���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_23,			/* �X�����v�O���t23���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_23,		/* �X�����v�O���t23���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_24,			/* �X�����v�O���t24���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_24,		/* �X�����v�O���t24���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_25,			/* �X�����v�O���t25���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_25,		/* �X�����v�O���t25���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_26,			/* �X�����v�O���t26���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_26,		/* �X�����v�O���t26���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_27,			/* �X�����v�O���t27���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_27,		/* �X�����v�O���t27���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_28,			/* �X�����v�O���t28���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_28,		/* �X�����v�O���t28���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_29,			/* �X�����v�O���t29���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_29,		/* �X�����v�O���t29���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_30,			/* �X�����v�O���t30���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_30,		/* �X�����v�O���t30���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_PROT_31,			/* �X�����v�O���t31���O�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_GRAPH_TAMA_MAI_31,		/* �X�����v�O���t31���O���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_GRAPH_COLOR_00,			/* �X�����v�O���t�F��� 0���O */
	CPU_COM_DATA_TRG_ID_GRAPH_COLOR_01,			/* �X�����v�O���t�F��� 1���O */
	CPU_COM_DATA_TRG_ID_GRAPH_COLOR_02,			/* �X�����v�O���t�F��� 2���O */
	CPU_COM_DATA_TRG_ID_GRAPH_COLOR_03,			/* �X�����v�O���t�F��� 3���O */
	CPU_COM_DATA_TRG_ID_HIGH_GRAPH_COLOR,		/* �ō��X�����v�O���t�F��� */
	CPU_COM_DATA_TRG_ID_GRAPH_HIST_00,			/* �X�����v�O���t�����f�[�^�Ή���� 0���O */
	CPU_COM_DATA_TRG_ID_GRAPH_HIST_01,			/* �X�����v�O���t�����f�[�^�Ή���� 1���O */
	CPU_COM_DATA_TRG_ID_GRAPH_HIST_02,			/* �X�����v�O���t�����f�[�^�Ή���� 2���O */
	CPU_COM_DATA_TRG_ID_GRAPH_HIST_03,			/* �X�����v�O���t�����f�[�^�Ή���� 3���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_00,			/* �ߋ��f�[�^0���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_01,			/* �ߋ��f�[�^1���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_02,			/* �ߋ��f�[�^2���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_03,			/* �ߋ��f�[�^3���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_04,			/* �ߋ��f�[�^4���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_05,			/* �ߋ��f�[�^5���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_06,			/* �ߋ��f�[�^6���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_07,			/* �ߋ��f�[�^7���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_08,			/* �ߋ��f�[�^8���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_09,			/* �ߋ��f�[�^9���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_10,			/* �ߋ��f�[�^10���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_11,			/* �ߋ��f�[�^11���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_12,			/* �ߋ��f�[�^12���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_13,			/* �ߋ��f�[�^13���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_14,			/* �ߋ��f�[�^14���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_15,			/* �ߋ��f�[�^15���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_16,			/* �ߋ��f�[�^16���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_17,			/* �ߋ��f�[�^17���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_18,			/* �ߋ��f�[�^18���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_19,			/* �ߋ��f�[�^19���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_20,			/* �ߋ��f�[�^20���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_21,			/* �ߋ��f�[�^21���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_22,			/* �ߋ��f�[�^22���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_23,			/* �ߋ��f�[�^23���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_24,			/* �ߋ��f�[�^24���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_25,			/* �ߋ��f�[�^25���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_26,			/* �ߋ��f�[�^26���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_27,			/* �ߋ��f�[�^27���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_28,			/* �ߋ��f�[�^28���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_29,			/* �ߋ��f�[�^29���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_30,			/* �ߋ��f�[�^30���O */
	CPU_COM_DATA_TRG_ID_DAY_BEFORE_31,			/* �ߋ��f�[�^31���O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_01,			/* �����f�[�^1��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_02,			/* �����f�[�^2��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_03,			/* �����f�[�^3��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_04,			/* �����f�[�^4��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_05,			/* �����f�[�^5��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_06,			/* �����f�[�^6��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_07,			/* �����f�[�^7��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_08,			/* �����f�[�^8��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_09,			/* �����f�[�^9��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_10,			/* �����f�[�^10��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_11,			/* �����f�[�^11��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_12,			/* �����f�[�^12��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_13,			/* �����f�[�^13��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_14,			/* �����f�[�^14��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_15,			/* �����f�[�^15��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_16,			/* �����f�[�^16��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_17,			/* �����f�[�^17��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_18,			/* �����f�[�^18��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_19,			/* �����f�[�^19��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_20,			/* �����f�[�^20��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_21,			/* �����f�[�^21��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_22,			/* �����f�[�^22��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_23,			/* �����f�[�^23��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_24,			/* �����f�[�^24��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_25,			/* �����f�[�^25��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_26,			/* �����f�[�^26��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_27,			/* �����f�[�^27��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_28,			/* �����f�[�^28��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_29,			/* �����f�[�^29��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_30,			/* �����f�[�^30��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_31,			/* �����f�[�^31��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_32,			/* �����f�[�^32��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_33,			/* �����f�[�^33��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_34,			/* �����f�[�^34��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_35,			/* �����f�[�^35��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_36,			/* �����f�[�^36��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_37,			/* �����f�[�^37��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_38,			/* �����f�[�^38��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_39,			/* �����f�[�^39��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_40,			/* �����f�[�^40��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_41,			/* �����f�[�^41��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_42,			/* �����f�[�^42��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_43,			/* �����f�[�^43��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_44,			/* �����f�[�^44��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_45,			/* �����f�[�^45��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_46,			/* �����f�[�^46��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_47,			/* �����f�[�^47��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_48,			/* �����f�[�^48��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_49,			/* �����f�[�^49��O */
	CPU_COM_DATA_TRG_ID_NUM_BEFORE_50,			/* �����f�[�^50��O */
	CPU_COM_DATA_TRG_ID_HIGH_DATA,				/* �ߋ��ō��f�[�^ */
	CPU_COM_DATA_TRG_ID_HIGH_DATA_GRAPH_PROT,	/* �ߋ��ō��O���t�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_HIGH_DATA_GRAPH_TAMA_MAI,	/* �ߋ��ō��O���t���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_DAY_DATA_00,			/* �����f�[�^���߂��u���b�N 0���O */
	CPU_COM_DATA_TRG_ID_DAY_DATA_01,			/* �����f�[�^���߂��u���b�N 1���O */
	CPU_COM_DATA_TRG_ID_DAY_DATA_02,			/* �����f�[�^���߂��u���b�N 2���O */
	CPU_COM_DATA_TRG_ID_TODAY_DATA,				/* �����f�[�^ */
	CPU_COM_DATA_TRG_ID_MARS_OUT,				/* �}�[�X�p���A�E�g */
	CPU_COM_DATA_TRG_ID_MARS_SAFE,				/* �}�[�X�p���Z�[�t */
	CPU_COM_DATA_TRG_ID_PRIVATE,				/* �v���C�x�[�g�f�[�^ */
	CPU_COM_DATA_TRG_ID_PRIVATE_GRAPH_PROT,		/* �v���C�x�[�g�f�[�^�O���t�v���b�g�y�уX�P�[�� */
	CPU_COM_DATA_TRG_ID_PRIVATE_GRAPH_TAMA_MAI,	/* �v���C�x�[�g�f�[�^�O���t���(�ʐ�/����) */
	CPU_COM_DATA_TRG_ID_ZONE,					/* �]�[���f�[�^ */
	CPU_COM_DATA_TRG_ID_CALENDER,				/* �J�����_�[ */
	CPU_COM_DATA_TRG_ID_POWER_FAIL,				/* �u���� */
	CPU_COM_DATA_TRG_ID_STATUS,					/* ��ԃf�[�^ */
	CPU_COM_DATA_TRG_ID_ERR_EQP,				/* �ʐM�ُ� */
	CPU_COM_DATA_TRG_ID_ERR_CNT,				/* �G���[�� */
	CPU_COM_DATA_TRG_ID_ERR_SAND,				/* �T���h�ʐM�G���[ */
	CPU_COM_DATA_TRG_ID_MAINTE_RELATION,		/* �����e�i���X�֌W */
	CPU_COM_DATA_TRG_ID_ERR_IOBOX,				/* ���o��BOX�ʐM�G���[ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_01,			/* �j���[�X1���f�[�^ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_02,			/* �j���[�X2���f�[�^ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_03,			/* �j���[�X3���f�[�^ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_04,			/* �j���[�X4���f�[�^ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_05,			/* �j���[�X5���f�[�^ */
	CPU_COM_DATA_TRG_ID_NEWS_DATA_06,			/* �j���[�X6���f�[�^ */
	CPU_COM_DATA_TRG_ID_COM_LINE,				/* ���[�ڑ��䐔 */
	CPU_COM_DATA_TRG_ID_VERSION_INFO,			/* �o�[�W������� */
	CPU_COM_DATA_TRG_ID_TROUBLE_INFO,			/* �g���u����� */
	CPU_COM_DATA_TRG_ID_LUNCH_INFO,				/* �����`��� */
	CPU_COM_DATA_TRG_ID_AWAY_STATUS,			/* ���ȏ�� */
	CPU_COM_DATA_TRG_ID_KUUSEKI_STATUS,			/* ��ȏ�� */
	CPU_COM_DATA_TRG_ID_ONETOUCH_STATUS,		/* �����^�b�`��� */
	CPU_COM_DATA_TRG_ID_EVENT_STATUS,			/* �C�x���g��� */
	CPU_COM_DATA_TRG_ID_RANKING_ATARI,			/* �����L���O(�哖��)��� */
	CPU_COM_DATA_TRG_ID_RANKING_KEIZOKU,		/* �����L���O(�p��)��� */
	CPU_COM_DATA_TRG_ID_RANKING_KAKUTOKU,		/* �����L���O(�l��)��� */
	CPU_COM_DATA_TRG_ID_OTHER_DAI,				/* ���ӑ�f�[�^ */
	CPU_COM_DATA_TRG_ID_OTHER_DAI_DETAIL,		/* ���ӏڍ׃f�[�^ */
	CPU_COM_DATA_TRG_ID_OTHER_DAI_B_BLOCK,		/* ���ӑO��f�[�^ */
	CPU_COM_DATA_TRG_ID_NEWS_INFO,				/* �j���[�X�z�M��� */
	CPU_COM_DATA_TRG_ID_CUT_IN_INFO,			/* �J�b�g�C����� */
	CPU_COM_DATA_TRG_ID_LABEL_INFO,				/* �D���\����� */
	CPU_COM_DATA_TRG_ID_SAND_INFO,				/* �T���h�ʐM��� */
	CPU_COM_DATA_TRG_ID_INFORMATION,			/* �C���t�H���[�V�����z�M��� */
	CPU_COM_DATA_TRG_ID_KADOU_STATUS,			/* ���ғ��X�e�[�^�X */
	CPU_COM_DATA_TRG_ID_INIT_CONF_DATA,			/* �����ݒ胂�[�h�ꎞ�f�[�^ */
	CPU_COM_DATA_TRG_ID_DATA_SET_DATA,			/* ���f�[�^�ł����݃��[�h�ꎞ�f�[�^ */
	CPU_COM_DATA_TRG_ID_ADDR_GET_DATA,			/* ���A�h���X�擾���[�h�ꎞ�f�[�^ */
	CPU_COM_DATA_TRG_ID_LAMP_CHK_DATA,			/* �����v����m�F���[�h�ꎞ�f�[�^ */
	CPU_COM_DATA_TRG_ID_MEM_MOVE_DATA,			/* �������ڍs���[�h�ꎞ�f�[�^ */
	CPU_COM_DATA_TRG_ID_SD_IMAGE_DATA,			/* SD�摜�]���ꎞ�f�[�^ */
	CPU_COM_DATA_TRG_ID_NEWS_CHK_DATA,			/* �j���[�X���e�m�F���[�h�ꎞ�f�[�^ */
	CPU_COM_DATA_TRG_ID_DATA_ORDER,				/* �f�[�^�\���w����� */
	CPU_COM_DATA_TRG_ID_MAINTE_STATUS,			/* �����e�i���X�p�\����� */
	CPU_COM_DATA_TRG_ID_GRAPH_STATE,			/* �O���t��ԕ\����� */
	CPU_COM_DATA_TRG_ID_MACHINE_SET_DATA,		/* ��ԍ��ݒ胂�[�h�ꎞ�f�[�^ */
	CPU_COM_DATA_TRG_ID_DATE_INFO,				/* ���t���(�����܂�) */
	CPU_COM_DATA_TRG_ID_MAX						/* �ő�l */
	/* �f�[�^��ǉ�����ۂ͈ȉ��̃e�[�u�������킹�ĕύX���鎖 */
	/* �Ecpu_com_ram_data_update[CPU_COM_RAM_UPDATE_CHK_MAX] */
	/* �Ecpu_com_data_data_table[CPU_COM_DATA_TRG_ID_MAX] */
}CPU_COM_DATA_TRIGGER_ID;

/* CPU�ԒʐM���M�v���g���KID */
/* �ėp�����֘A �����ʂȂ��̂͏��� */
/* �����v��������������̂͏��� ��)�\���f�[�^�X�V�A�C�x���g�ʒm�A�����Đ��� */
/* ���ʂ͏��������Ă�����̂͏��� ��)�v���O�����]���� */
typedef enum _CPU_COM_REQ_TRIGGER_ID {
	CPU_COM_REQ_TRG_ID_NONE=0,					/* �y���M�v���g���KID�z�v������ */
	CPU_COM_REQ_TRG_ID_STRING_WRITE,			/* �y���M�v���g���KID�z�����񏑂����� */
	CPU_COM_REQ_TRG_ID_STRING_READ,				/* �y���M�v���g���KID�z������ǂݏo�� */
	CPU_COM_REQ_TRG_ID_FILE_READ,				/* �y���M�v���g���KID�z�t�@�C���ǂݏo�� */
//	CPU_COM_REQ_TRG_ID_SOUND_REQ,				/* �y���M�v���g���KID�z�����Đ��v�� */	/* �����v���o�b�t�@�����O�Ή��̂��߃g���KID����ړ� */
	CPU_COM_REQ_TRG_ID_MAX						/* �y���M�v���g���KID�z�ő�l */
}CPU_COM_REQ_TRIGGER_ID;

/* CPU�ԒʐM��ʐ����� ���ID */
typedef enum _CPU_COM_CONTROL_STATUS_ID {
	CPU_COM_CONTROL_STATUS_ID_NONE=0,			/* �y��ʐ����� ���ID�z��Ԗ��� */
	CPU_COM_CONTROL_STATUS_ID_NEWS_END,			/* �y��ʐ����� ���ID�z�j���[�X�z�M�I�� */
	CPU_COM_CONTROL_STATUS_ID_ANIME_END,		/* �y��ʐ����� ���ID�z�A�j���Đ��I�� */
	CPU_COM_CONTROL_STATUS_ID_MAX				/* �y��ʐ����� ���ID�z�ő�l */
}CPU_COM_CONTROL_STATUS_ID;


/* CPU�ԒʐM �R�}���h��� */
/* �v���E�����̃Z�b�g */
typedef enum _CPU_COM_CMD_ID{
	CPU_COM_CMD_NONE=0,							/* �yCPU�ԒʐM�R�}���h�z�R�}���h����				*/
	CPU_COM_CMD_STATUS_REQ,						/* �yCPU�ԒʐM�R�}���h�z�X�e�[�^�X�v��				*/
	CPU_COM_CMD_SENSOR_DATA,					/* �yCPU�ԒʐM�R�}���h�z�Z���T�[�f�[�^�X�V			*/
	CPU_COM_CMD_MODE_CHG,						/* �yCPU�ԒʐM�R�}���h�z��ԕύX(G1D)				*/
	CPU_COM_CMD_PC_LOG,							/* �yCPU�ԒʐM�R�}���h�zPC���O���M(�����R�}���h)	*/
	
	CPU_COM_CMD_PRG_DOWNLORD_REQ,				/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n		*/
	CPU_COM_CMD_PRG_DOWNLORD_DATA_REQ,			/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���f�[�^�v�� */
	CPU_COM_CMD_PRG_DOWNLORD_DATA_RCV,			/* �yCPU�ԒʐM�R�}���h�z�v���O�����]��(��M)	*/
	CPU_COM_CMD_PRG_DOWNLORD_SUM_REQ,			/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���T���l�v�� */
	CPU_COM_CMD_PRG_DOWNLORD_RESLUT_REQ,		/* �yCPU�ԒʐM�R�}���h�z�v���O�����]�����ʗv��	*/
	CPU_COM_CMD_FILE_REQ,						/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]���J�n		*/
	CPU_COM_CMD_FILE,							/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]��			*/
	CPU_COM_CMD_FILE_BLOCK_RESULT_REQ,			/* �yCPU�ԒʐM�R�}���h�z�u���b�N�]�����ʗv��	*/
	CPU_COM_CMD_FILE_RESLUT_REQ,				/* �yCPU�ԒʐM�R�}���h�z�t�@�C���]�����ʗv��	*/
	CPU_COM_CMD_MAX								/* �yCPU�ԒʐM�R�}���h�z�ő�l					*/
}CPU_COM_CMD_ID;



/* �R�}���h�^�C�v */
/* ���}�X�^�[��p */
#define CPU_COM_CMD_TYPE_ONESHOT_SEND			0					/* �P�����M�R�}���h */
#define CPU_COM_CMD_TYPE_RETRY					1					/* ���g���C */
#define CPU_COM_CMD_TYPE_RENSOU					2					/* �A�� */
#define CPU_COM_CMD_TYPE_ONESHOT_RCV			3					/* �P����M�R�}���h */


/* CPU�ԒʐM�_�E�����[�h */
/* RD1402�b�� �u���b�N�ԍ���0�n�܂��SH���ŃI�t�Z�b�g�������� */
/* �u���b�N�ԍ��ő�l�͎b��l�̂��ߗv���� */
#define CPU_COM_BLOCK_NUM_MIN			(0x00000000)		/* CPU�ԒʐM�u���b�N�ԍ� �ŏ��l */
#define CPU_COM_BLOCK_NUM_MAX			(0x00FFFFFF)		/* CPU�ԒʐM�u���b�N�ԍ� �ő�l */

// RD8001�b��F�h���C�o�ֈړ�
/* ���M�X�e�[�^�X */
#define DRV_CPU_COM_STATUS_CAN_SEND		(0)			/* ���M�\��� */
#define DRV_CPU_COM_STATUS_SENDING		(1)			/* ���M�� */
//#define DRV_CPU_COM_STATUS_SEND_END		(2)			/* ���M�I�� */ /* ���M�I����Ԃ���5ms��ɑ��M�\�ƂȂ� */



/* �ǂݏo��(������A�t�@�C��)���� */
typedef enum{
	READ_RESULT_NO_ERROR = 0,		/* ����I�� */
	READ_RESULT_ERROR_READ,			/* �ǂݏo�����s */
	READ_RESULT_FILE_NON,			/* �t�@�C���Ȃ� */
	READ_RESULT_MAX
} ENUM_READ_RESULT;


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



//�T�uCPU�ʐM�ُ픻���
#define CPU_COM_SUB_CNT_OPE_START			( 20 )					/* �J�n�p��1�b[10ms] */
#define CPU_COM_SUB_CNT_NORMAL				( 1 * 60 * 100 )		/* �ʏ��1��[10ms] */
#define CPU_COM_SUB_CNT_PRG					( 6 * 60 * 100 )		/* �v���O����������6��[10ms] */

#define CPU_COM_SUB_OPE_END_TIME_PRG		7000	/* ���슮���m�F����[70�b] ���v���O���� */
#define CPU_COM_SUB_OPE_END_TIME			500		/* ���슮���m�F����[5�b] �����̑� */


// ����I�����
typedef enum{
	CPU_COM_SUB_OPE_END_RESULT_OK = 0,			/* 0�F����I��*/
	CPU_COM_SUB_OPE_END_RESULT_NG,				/* 1�F�ُ�I�� */
	CPU_COM_SUB_OPE_END_RESULT_CONT,			/* 2�F������ */
}CPU_COM_SUB_OPE_END_RESULT;

/* �T�u�}�C�R������ */
typedef enum{
	CPU_COM_SUB_OPE_IMAGE = 0,			/* �摜�]��(�������[�J�[�h) */
	CPU_COM_SUB_OPE_PRG,				/* �A���h���C�h(�T�u�}�C�R��)�v���O�����X�V */
	CPU_COM_SUB_OPE_BACKUP,				/* �o�b�N�A�b�v(�A���h���C�h���������[�J�[�h) */
	CPU_COM_SUB_OPE_RECOVERY,			/* ���J�o��(�������[�J�[�h���A���h���C�h) */
	CPU_COM_SUB_OPE_NON					/* �Ȃ� */
}CPU_COM_SUB_OPE;

/* �T�u�}�C�R�������� */
typedef enum{
	CPU_COM_SUB_STATE_NON = 0,			/* �Ȃ� */
	CPU_COM_SUB_STATE_START,			/* ����J�n(�A���h���C�h���������[�J�[�h) */
	CPU_COM_SUB_STATE_END,				/* ����I��(�A���h���C�h���������[�J�[�h) */
	CPU_COM_SUB_STATE_RX_PRG,			/* RX(���C���}�C�R��)�v���O�����X�V�J�n */
}CPU_COM_SUB_STATE;

//�T�uCPU����
typedef struct{
	CPU_COM_SUB_OPE ope;		// ����
	CPU_COM_SUB_STATE state;	// ���
	UW last_time;				// ����
	UW no_res_cnt;				// ��������
}CPU_COM_SUB;

/******************/
/*   �\���̒�`   */
/******************/
typedef struct _T_CPU_COM_CMD_INFO {
	UB cmd;					/* �R�}���h */
	UB cmd_type;			/* �R�}���h�^�C�v */
	UB retry_cnt;			/* ���g���C�� */
	UW retry_time;			/* ���g���C�Ԋu */
	UB rensou_cnt;			/* �A���� */
} T_CPU_COM_CMD_INFO;


typedef struct{
	UH file_num;					/* �t�@�C���ԍ�	2byte */
	UW block_num;					/* �u���b�N�ԍ�	4byte */
	UB* p_data;						/* ���������f�[�^�̐擪�|�C���^	256byte�܂� */
	UH len;							/* �f�[�^�� *///RD1402�b��
}T_CPU_COM_CMD_FILE_DATA;


typedef struct{
	UW block_num;					/* �u���b�N�ԍ�	4byte */
}T_CPU_COM_CMD_FILE_RESULT_BLOCK;

typedef struct{
	UW sum;							/* �T���l	4byte */
}T_CPU_COM_CMD_FILE_RESULT;

// �t�@�C���ǂݏo��
typedef struct{
	UH file_num;					/* �t�@�C���ԍ�	2byte */
	UW block_num;					/* �u���b�N�ԍ�	4byte */
}T_CPU_COM_CMD_READ_FILE;




typedef struct{
	UB buf[CPU_COM_MSG_SIZE_MAX];		/* �o�b�t�@ */
	UH pos;								/* ��M�ʒu */
	UB last_dle_flg;					/* �O���M�f�[�^��DLE */
	UB state;							/* �O���M�f�[�^��DLE */
}CPU_COM_RCV_MSG;



/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void cpu_com_init_sub(void);
extern void cpu_com_proc(void);
extern void cpu_com_sub_reset(void);
extern UB cpu_com_get_status(void);
extern UH cpu_com_dle_extension( UB* data, UH size );
extern UB cpu_com_file_write( UB* p_buf, UH size, UW block, UB file_no );
extern UB cpu_com_file_read( UB* p_buf, UH size, UW block, UB file_no );
extern void drv_uart0_data_init( void );
//extern RING_BUF* drv_uart0_get_snd_ring( void );
//extern RING_BUF* drv_uart0_get_rcv_ring( void );
extern UB cpu_com_get_busy( void );




#endif // __CPU_COM_INC__
