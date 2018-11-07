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

#ifndef	__MAIN_USR_INC__
#define	__MAIN_USR_INC__

#if !defined(_USE_RWBLE_SOURCE)
#include "arch.h"
#include "rwke_api.h"
#else /* !defined(_USE_RWBLE_SOURCE) */
#include	"ke_task.h"
#endif

// ====================================================
// ==================�v���b�g�t�H�[��==================
// ====================================================

/* Tas Infomation */
#define CPU_COM_STATE_MAX		2	// Max State Num
#define CPU_COM_IDX_MAX			1	// Max ID Num

#define USER_MAIN_ID (TASK_USR_0)

//#define USER_MAIN_CYC_ACT			1	/* Task API ID */

#define USER_MAIN_CYC_ACT				KE_FIRST_MSG(USER_MAIN_ID) + 1		/* Task API ID */
#define USER_MAIN_CALC_SEKIGAI			KE_FIRST_MSG(USER_MAIN_ID) + 2		/* Timer API ID */
#define USER_MAIN_CALC_SEKISHOKU		KE_FIRST_MSG(USER_MAIN_ID) + 3		/* Timer API ID */
#define USER_MAIN_CALC_KOKYU			KE_FIRST_MSG(USER_MAIN_ID) + 4		/* Timer API ID */
#define USER_MAIN_CALC_IBIKI			KE_FIRST_MSG(USER_MAIN_ID) + 5		/* Timer API ID */
#define USER_MAIN_CALC_ACL				KE_FIRST_MSG(USER_MAIN_ID) + 6		/* Timer API ID */
#define USER_MAIN_ACT2					KE_FIRST_MSG(USER_MAIN_ID) + 7		/* Timer API ID */







//#define USER_MAIN_CYC_ACT			1								/* Task API ID */
//#define CPU_COM_TIMER_API		2								/* Timer API ID */


#if 0
#define RBLE_APP_CON_SW_EVT			KE_FIRST_MSG(TASK_CON_APPL) + 3	/* SW API ID */
#define RBLE_APP_CON_SEND_EVT		KE_FIRST_MSG(TASK_CON_APPL) + 4	/* SEND API ID */
#endif


#define  KE_EVT_USR_2_BIT			CO_BIT(31 - KE_EVT_USR_2) 
#define  KE_EVT_USR_3_BIT			CO_BIT(31 - KE_EVT_USR_3) 

#if 0
extern volatile RBLE_RUNNING_MODE	rBLE_Run_Mode_Flg;
#endif
/* Status Handler */
extern const struct ke_state_handler cpu_com_state_handler[ CPU_COM_STATE_MAX ];
/* Default Handler */
extern const struct ke_state_handler cpu_com_default_handler;
/* Status */
extern ke_state_t cpu_com_state[ CPU_COM_IDX_MAX ];



// ============================================================
// ==================���[�U�[�A�v���P�[�V����==================
// ============================================================
// �V�X�e�����[�h ��H1D/G1D����
typedef enum{
	// �d�l��̏�ԉ���
	SYSTEM_MODE_INITIAL = 0,					// �C�j�V����
	SYSTEM_MODE_IDLE_REST,					// �A�C�h��_�c�ʕ\�� ��RD8001�b��FIDLE�𓝍����邩�v����
	SYSTEM_MODE_IDLE_COM,					// �A�C�h��_�ʐM�ҋ@ ��RD8001�b��FIDLE�𓝍����邩�v����
	SYSTEM_MODE_SENSING,					// �Z���V���O
	SYSTEM_MODE_GET,						// �f�[�^�擾
	SYSTEM_MODE_PRG_H1D,					// H1D�v���O�����X�V
	SYSTEM_MODE_PRG_G1D,					// G1D�v���O�����X�V
	SYSTEM_MODE_SELF_CHECK,					// ���Ȑf�f
	// �d�l��̏�ԏ��
	SYSTEM_MODE_MOVE,						// �ڍs
	SYSTEM_MODE_NON,						// �Ȃ�
	SYSTEM_MODE_MAX
}SYSTEM_MODE;


// �C�x���g ��H1D/G1D����
typedef enum{
	EVENT_NON = 0,				// �Ȃ�
	EVENT_POW_SW_SHORT,			// �d��SW����(�Z)
	EVENT_POW_SW_LONG,			// �d��SW����(��)
	EVENT_CHG_PORT_ON,			// �[�d���m�|�[�gON
	EVENT_CHG_PORT_OFF,			// �[�d���m�|�[�gOFF
	EVENT_DENCH_LOW,			// �d�r�c�ʒቺ
	EVENT_CHG_FIN,				// �[�d����
	EVENT_GET_DATA,				// �f�[�^�擾
	EVENT_H1D_PRG,				// �v���O������������(H1D)
	EVENT_G1D_PRG,				// �v���O������������(G1D)
	EVENT_SELF_CHECK_COM,		// ���Ȑf�f(�ʐM)
	EVENT_COMPLETE,				// ����
	EVENT_STOP,					// ���f
	EVENT_TIME_OUT,				// �^�C���A�E�g
	EVENT_KENSA_ON,				// �����|�[�gON
	EVENT_MAX,					// �ő�
}EVENT_NUM;

// �d�r�c�ʏ�� ��H1D/G1D����
#define DENCH_ZANRYO_STS_MAX					1	// �[�d���^��
#define DENCH_ZANRYO_STS_HIGH					2	// ��������
#define DENCH_ZANRYO_STS_LOW					3	// �c��P�������Ȃ�
#define DENCH_ZANRYO_STS_MIN					4	// �d�r�c�ʂȂ�


// �v���O�����V�[�P���X
typedef enum{
	PRG_SEQ_IDLE = 0,					// �A�C�h��
	PRG_SEQ_READY_WAIT,					// �J�n�҂��@���u�[�g�ւ̐ؑ�
	PRG_SEQ_START_WAIT,					// �J�n�҂��@���A�v���ւ̐ؑ�
	PRG_SEQ_ERASE_WAIT,					// �����҂��@���t���b�V������
	PRG_SEQ_COMPLETE_WAIT,				// �����҂��@�����ʒʒm
	PRG_SEQ_MAX
}PRG_SEQ;



#define			MAIN_STATUS_REQ_TIME			50		// �X�e�[�^�X�v������[500ms]
//#define			MAIN_STATUS_REQ_TIME			100		// �X�e�[�^�X�v������[1s]

#define			SENSING_END_JUDGE_TIME				100		// �Z���V���O�Ȃ�[1s]



#define		TIME_CNT_DISP_SELF_CHECK_ERR			(300)		/* ���Ȑf�f�ُ�\��(3�b) */
#define		TIME_CNT_DISP_SELF_CHECK_FIN			(300)		/* ���Ȑf�f�����\��(3�b) */

#define		SENSING_CNT_MIN							(40)		/* �Z���V���O�񐔂̉���(20��) */

// �����
#define		MEAS_SEKIGAI_CNT_MAX		140
#define		MEAS_SEKISHOKU_CNT_MAX		140
#define		MEAS_KOKYU_CNT_MAX			200
#define		MEAS_IBIKI_CNT_MAX			200
#define		MEAS_ACL_CNT_MAX			20

// �b�^�C�}�[ ��+1�����l��ݒ�
#define		TIMER_SEC_PRG_READY_WAIT	( 1 + 1 )
#define		TIMER_SEC_PRG_START_WAIT	( 1 + 1 )
#define		TIMER_SEC_PRG_ERASE_WAIT	( 27 + 1 )

// �^�C�}�A�E�g
#define		TIME_OUT_SYSTEM_MODE_IDLE_REST		( 6 - 1 )
#define		TIME_OUT_SYSTEM_MODE_IDLE_COM		( 180 - 1 )		


#define	OK_NOW_UPDATING					0		//�X�V����
#define	OK_UPDATE_FIX					1		//���튮��(����)
#define	NG_UPDATE_FAILURE				2		//�ُ튮��(���s)

#define	PRG_HD_UPDATE_STATE_NON			0	//�X�V����
#define	PRG_HD_UPDATE_STATE_OK			1	//���튮��(����)
#define	PRG_HD_UPDATE_STATE_NG			2	//�ُ튮��(���s)

// �v���O�����]��(H1D)
#define	PRG_H1D_EEP_RECODE_OFFSET		(UW)12				//[Byte]
#define	PRG_H1D_EEP_RECODE_UNIT			(UW)20				//[Byte]

//#define	PRG_H1D_EEP_RECODE_CNT_MAX		(UW)((3276*2)-1)
#define	PRG_H1D_EEP_RECODE_CNT_MAX		(UW)(( EEP_DATA_SIZE_ALL / ( PRG_H1D_EEP_RECODE_UNIT + PRG_H1D_EEP_RECODE_OFFSET )) - (UW)1 )	// �ŏI���R�[�h�̓v���O������ʗp



#define	BD_ADRS_NUM						6

typedef enum program_ver{
	VERSION_MAJOR = 0,
	VERSION_MINOR,
	VERSION_REVISION,
	VERSION_BUILD,
	//-------
	VERSION_NUM
}tag_program_ver;



// ������
typedef struct{
	union{
		UB	byte[CPU_COM_SND_DATA_SIZE_SENSOR_DATA];
		struct{
			W	sekigaival;		// �������ׂ͂̈ɕ�������
			W	sekishoku_val;	// �������ׂ͂̈ɕ�������
			UH	kokyu_val;		
			UH	ibiki_val;		
			B acl_x;
			B acl_y;
			B acl_z;
			B dummy;			// ��[�p�f�B���O����]
		}dat;
	}info;
}MEAS;

// ���Z���
typedef struct{
	union{
		UB	byte[EEP_CACL_DATA_SIZE];
		struct{
			UH	ibiki_val;
			UB	state;			/* ��� */
			UB	myaku_val;
			UB	spo2_val;
			UB	kubi;
			UB	dummy_1;			/* ���E�l�����p */
			UB	dummy_2;			/* ���E�l�����p */
		}dat;
	}info;
}CALC;

// �������
typedef struct{
	UB year;
	UB month;
	UB week;
	UB day;
	UB hour;
	UB min;
	UB sec;
}DATE;

// �x���@�\
typedef struct{
	union{
		UB	byte[EEP_ALARM_SIZE];
		struct{
			UB	valid;			// �A���[���@�\�L��/����
			UB	ibiki;			// ���т��A���[��
			UB	ibiki_sens;		// ���т��A���[�����x
			UB	low_kokyu;		// ��ċz�A���[��
			UB	delay;			// �A���[���x��
			UB	stop;			// �̓���~
			UB	time;			// ������
			UB	dummy;			// ���E�l�����p
		}dat;
	}info;
}ALARM;


// ���Ȑf�f
typedef struct{
	ke_time_t	last_time;
	UH	eep_cnt;		// EEP������
	UB	seq;			// �V�[�P���X
	UB	com_flg;		// �ʐM�ł̎��Ȑf�f�t���O
}SELF_CHECK;

// H1D���
typedef struct{
	UB	ble				:1;		/* 1  BLE�ڑ� */
	UB	dummy1			:1;		/* 2  ����`  */
	UB	dummy2			:1;		/* 3  ����` */
	UB	dummy3			:1;		/* 4  ����` */
	UB	dummy4			:1;		/* 5  ����` */
	UB	dummy5			:1;		/* 6  ����` */
	UB	dummy6			:1;		/* 7  ����` */
	UB	dummy7			:1;		/* 8  ����` */
}BIT_G1D_INFO;
typedef struct{
	union {
		UB	byte;
		BIT_G1D_INFO bit_f;
		/*�ďo�����v���*/
	}info;
}G1D_INFO;


// H1D���
typedef struct{
	UB	bat_chg			:1;		/* 1  �[�d���m�|�[�g */
	UB	kensa			:1;		/* 2  �����|�[�g */
	UB	dummy1			:1;		/* 3  ����` */
	UB	dummy2			:1;		/* 4  ����` */
	UB	dummy3			:1;		/* 5  ����` */
	UB	dummy4			:1;		/* 6  ����` */
	UB	dummy5			:1;		/* 7  ����` */
	UB	dummy6			:1;		/* 8  ����` */
}BIT_H1D_INFO;
typedef struct{
	union {
		UB	byte;
		BIT_H1D_INFO bit_f;
		/*�ďo�����v���*/
	}info;
}H1D_INFO;


// �t���[��(�g)�ԍ�
typedef struct{
	UB read;		// �t���[��(�g)�̓ǂݏo���ԍ�
	UB write;		// �t���[��(�g)�̏������ݔԍ�
	UB cnt;		// �t���[��(�g)�̏������ݐ��@���������ݐ�0�ƍő�̋�ʂ����Ȃ���
}FRAME_NUM_INFO;

typedef struct{
	
	SYSTEM_MODE system_mode;		/* �V�X�e�����[�h */
	SYSTEM_MODE next_system_mode;	/* ���̃V�X�e�����[�h */
	SYSTEM_MODE last_system_mode;		/* �V�X�e�����[�h */
	
	UH system_mode_time_out_cnt;		/* �V�X�e�����[�h�^�C���A�E�g�J�E���g[S] */

	DATE date;
	
	ke_time_t last_time;			//�O�񎞊�
	ke_time_t last_sensing_data_rcv;		//�O��Z���V���O�f�[�^��M
	UB sensing_flg;					// �Z���V���O���t���O
	
	UB denchi_sts;			// �d�r���
	H1D_INFO h1d;			// H1D���
	H1D_INFO h1d_last;		// H1D���(�O��)
	
	
	// ���Z�֘A
	CALC calc;				// ���Z��f�[�^
	UH calc_cnt;			// ���Z�J�E���g
	UH max_mukokyu_sec;		// �ő喳�ċz[�b]
	
	// �t���[��(�g)�ԍ�
	FRAME_NUM_INFO frame_num;			// �t���[��(�g)�ԍ�
	FRAME_NUM_INFO frame_num_work;		// �t���[��(�g)�ԍ����[�N
	
	// �@��f�[�^(���Z�O)
	H	sekigai_val[MEAS_SEKIGAI_CNT_MAX];		// �������ׂ͂̈ɕ�������
	H	sekishoku_val[MEAS_SEKISHOKU_CNT_MAX];	// �������ׂ͂̈ɕ�������
	UH	kokyu_val[MEAS_KOKYU_CNT_MAX];		
	UH	ibiki_val[MEAS_IBIKI_CNT_MAX];		
	B	acl_x[MEAS_ACL_CNT_MAX];
	B	acl_y[MEAS_ACL_CNT_MAX];
	B	acl_z[MEAS_ACL_CNT_MAX];
	// �@��f�[�^(���Z�O)��
	UB	sekigai_cnt;
	UB	sekishoku_cnt;
	UB	kokyu_cnt;
	UB	ibiki_cnt;
	UB	acl_cnt;
	UB	sekigai_seq;		// �ԊO�L��/�����ؑ֗p�̃V�[�P���X
	
	ALARM	alarm;			// �x���@�\
	
	UW	timer_sec;			// �^�C�}�[[�b]�@���J�E���g�_�E��
	
	// �ȍ~���[�N�̈�
	UW sec30_cnt;			//30�b�J�E���g
	UW sec10_cnt;			//10�b�J�E���g
	UW sec7_cnt;			//7�b�J�E���g
	
	UB get_mode_seq;				// GET���[�h�V�[�P���X
	UH get_mode_calc_cnt;
	UH set_mode_calc_cnt;
	
	// �v���O�������������p
	UH prg_hd_eep_record_cnt_wr;					// ���R�[�h�J�E���g
	UH prg_hd_eep_record_cnt_rd;					// ���R�[�h�J�E���g
	UW prg_hd_eep_code_record_sum;					// ���R�[�h�T���l
	UB prg_hd_update_state;							// �v���O�����X�V���
	UB prg_hd_seq;									// �v���O�����X�V���
	UB prg_hd_version[VERSION_NUM];			// �v���O�����o�[�W����
	
	// �v���O������������(G1D)
	UB prg_g1d_send_ver_flg;				// G1D�o�[�W�������M
	UB prg_g1d_send_ver_sec;				// G1D�o�[�W�������M�b
	
	// ���Ȑf�f
	SELF_CHECK	self_check;
	
	UB	bd_device_adrs[BD_ADRS_NUM];						// BD�f�o�C�X�A�h���X
	
	UW err_cnt;			//�ُ��(�f�o�b�O�p�r)
}T_UNIT;






/*##################################################################*/
/*							CPU�ԒʐM��								*/
/*##################################################################*/
typedef struct _DS_CPU_COM_INPUT{
	UB rcv_cmd;											/* ��M�R�}���h */
	UB rcv_data[CPU_COM_DATA_SIZE_MAX];					/* ��M�f�[�^ */
	UB cpu_com_send_status;								/* CPU�ԒʐM���M�X�e�[�^�X */
														/* 	CPU_COM_SND_STATUS_IDLE			�A�C�h����� �����M�\���	*/
														/* 	CPU_COM_SND_STATUS_SEND			���M��						*/
														/* 	CPU_COM_SND_STATUS_COMPLETE		���M����					*/
														/* 	CPU_COM_SND_STATUS_SEND_NG		���g���CNG					*/
}DS_CPU_COM_INPUT;

typedef struct _DS_CPU_COM_ORDER{
	CPU_COM_CMD_ID snd_cmd_id;							/* ���M�R�}���hID */
	UB snd_data[CPU_COM_DATA_SIZE_MAX];					/* ���M�f�[�^ */
	UH data_size;										/* ���M�f�[�^�� */
}DS_CPU_COM_ORDER;

typedef struct _DS_CPU_COM{
	DS_CPU_COM_INPUT input;
	DS_CPU_COM_ORDER order;
}DS_CPU_COM;

typedef struct _CPU_COM_RCV_CMD_TBL{
	UB cmd;							/* ��M�R�}���h */
//	void (*func)(UB *p_data);		/* ��M���� */
	void (*func)(void);				/* ��M���� */
	UB res;							/* �����L(ON)��(OFF) */
}CPU_COM_RCV_CMD_TBL;

/*##################################################################*/
/*							VUART(BLE)�ʐM��						*/
/*##################################################################*/
/* CPU�ԒʐM �R�}���h��� */
/* �v���E�����̃Z�b�g */
typedef enum{
	VUART_CMD_TYPE_NONE=0,							// �Ȃ�
	VUART_CMD_TYPE_MODE_CHG,						// ��ԕύX(G1D)
	VUART_CMD_TYPE_DATE_SET,						// �����ݒ�
	VUART_CMD_TYPE_INFO,							// ���擾
	VUART_CMD_TYPE_VERSION,							// �o�[�W�����擾
	VUART_CMD_TYPE_DEVICE_INFO,						// �f�o�C�X�󋵎擾
	VUART_CMD_TYPE_DATA_NEXT,						// NEXT
	VUART_CMD_TYPE_DATA_END,						// END
	VUART_CMD_TYPE_DATA_FRAME,						// �g���(������)
	VUART_CMD_TYPE_DATA_CALC,						// �@��f�[�^
	VUART_CMD_TYPE_DATA_FIN,						// �f�[�^�擾�����ʒm
	VUART_CMD_TYPE_PRG_H1D_DATA,					// �v���O�����]��(�f�[�^)
	VUART_CMD_TYPE_PRG_H1D_RESULT,					// �v���O�����]������
	VUART_CMD_TYPE_PRG_H1D_CHECK,					// �v���O�����X�V�����m�F
	VUART_CMD_TYPE_ALARM_SET,						// �ݒ�ύX
	VUART_CMD_TYPE_ALARM_INFO,						// �A���[���ʒm
	VUART_CMD_TYPE_MAX								// �ő�l					
}VUART_CMD_TYPE;

typedef struct{
	UB cmd;							/* ��M�R�}���h */
	UB len;							/* ��M�R�}���h */
	void (*func)(void);				/* ��M���� */
}VUART_RCV_CMD_TBL;


// �R�}���h
#define	VUART_CMD_MODE_CHG		0xB0
#define	VUART_CMD_SET_CHG		0xC0
#define	VUART_CMD_DATE_SET		0xC1
#define	VUART_CMD_INFO			0xC2
#define	VUART_CMD_DATA_NEXT		0xE0	// NEXT
#define	VUART_CMD_DATA_END		0xE1	// END
#define	VUART_CMD_DATA_FRAME	0xE2	// �g
#define	VUART_CMD_DATA_CALC		0xE3	// ���Z�f�[�^
#define	VUART_CMD_DATA_FIN		0xE4	// ����
#define	VUART_CMD_PRG_RESULT	0xD1
#define	VUART_CMD_PRG_CHECK		0xD3
#define	VUART_CMD_PRG_G1D_START	0xF0
#define	VUART_CMD_PRG_G1D_VER	0xF9

#define	VUART_CMD_ALARM_SET		0xC0
#define	VUART_CMD_ALARM_INFO	0xC4

#define	VUART_CMD_INFO			0xC2
#define	VUART_CMD_VERSION		0xC3
#define	VUART_CMD_DEVICE_INFO	0xC5
#define	VUART_CMD_ALARM_INFO	0xC4
#define	VUART_CMD_INVALID		0xFF	// �R�}���h�������ꏈ��


// ��M�f�[�^�� ���R�}���h���܂�
#define	VUART_CMD_LEN_MODE_CHG		2
#define	VUART_CMD_LEN_SET_CHG		3
#define	VUART_CMD_LEN_DATE_SET		8
#define	VUART_CMD_LEN_INFO			1
#define	VUART_CMD_LEN_DATA_NEXT		1	// NEXT
#define	VUART_CMD_LEN_DATA_END		1	// END
#define	VUART_CMD_LEN_DATA_FRAME	10	// �g
#define	VUART_CMD_LEN_DATA_CALC		7	// ���Z�f�[�^
#define	VUART_CMD_LEN_DATA_FIN		2	// ���Z�f�[�^
#define	VUART_CMD_LEN_PRG_RESULT	5
#define	VUART_CMD_LEN_PRG_DATA		20
#define	VUART_CMD_LEN_PRG_CHECK		1
#define	VUART_CMD_LEN_PRG_G1D_START	1
#define	VUART_CMD_LEN_PRG_G1D_VER	1

#define	VUART_CMD_LEN_VERSION		1
#define	VUART_CMD_LEN_DEVICE_INFO	1

#define	VUART_CMD_LEN_ALARM_SET		8

#define	VUART_CMD_ONLY_SIZE			1	// �R�}���h�݂̂̃T�C�Y

// ���M�f�[�^�� ���R�}���h���܂�
#define	VUART_SND_LEN_INFO			3
#define	VUART_SND_LEN_VERSION		14 
#define	VUART_SND_LEN_DEVICE_INFO	16


#define VUART_DATA_SIZE_MAX				20

#define VUART_DATA_RESULT_OK		0
#define VUART_DATA_RESULT_NG		1



typedef struct _DS_VUART_INPUT{
	UB rcv_data[VUART_DATA_SIZE_MAX];					/* ��M�f�[�^ */
	UB rcv_len;											/* ��M�� */
	UB send_status;										/* ���M���(ON:���M���AOFF:���M�Ȃ�) */
}DS_VUART_INPUT;

typedef struct _DS_VUART{
	DS_VUART_INPUT input;
}DS_VUART;


/* DS�\���� */
typedef struct{
	DS_CPU_COM 		cpu_com;		/* CPU�ԒʐM�� */
	DS_VUART		vuart;			/* ���zUART(BLE) */
}DS;


/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void codeptr app_evt_usr_2(void);
extern void codeptr app_evt_usr_3(void);
extern void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
extern void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );
extern bool user_main_sleep(void);
extern void ds_set_vuart_data( UB *p_data, UB len );
extern void ds_set_vuart_send_status( UB status );
extern void user_system_init( void );
extern void user_main_init( void );
extern void main_cpu_com_snd_pc_log( UB* data, UB size );
extern void main_set_bd_adrs( UB* bda);

#endif // __MAIN_USR_INC__
