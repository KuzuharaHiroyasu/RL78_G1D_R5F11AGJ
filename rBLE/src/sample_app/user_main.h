/********************************************************************************/
/* �V�X�e����   : RD8001														*/
/* �t�@�C����   : main.h														*/
/* �@�\         : ���[�U�[���C��(�}�N����`�A�^��`�A�֐��̊O���Q�Ɛ錾)		*/
/* �ύX����     : 2017.12.20 Axia Soft Design ����		���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

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
#define USER_MAIN_STATE_MAX		2	// Max State Num
#define USER_MAIN_IDX_MAX			1	// Max ID Num

#define USER_MAIN_ID (TASK_USR_0)

#define USER_MAIN_CYC_ACT				KE_FIRST_MSG(USER_MAIN_ID) + 1
#define USER_MAIN_CALC_SEKIGAI			KE_FIRST_MSG(USER_MAIN_ID) + 2
#define USER_MAIN_CALC_SEKISHOKU		KE_FIRST_MSG(USER_MAIN_ID) + 3
#define USER_MAIN_CALC_KOKYU			KE_FIRST_MSG(USER_MAIN_ID) + 4
#define USER_MAIN_CALC_IBIKI			KE_FIRST_MSG(USER_MAIN_ID) + 5
#define USER_MAIN_CALC_ACL				KE_FIRST_MSG(USER_MAIN_ID) + 6
#define USER_MAIN_CYC_CALC_RESULT		KE_FIRST_MSG(USER_MAIN_ID) + 7
#define USER_MAIN_CYC_BATTERY			KE_FIRST_MSG(USER_MAIN_ID) + 8
#define USER_MAIN_CYC_LED				KE_FIRST_MSG(USER_MAIN_ID) + 9
#define USER_MAIN_CYC_PHOTOREF			KE_FIRST_MSG(USER_MAIN_ID) + 10




//#define USER_MAIN_CYC_ACT			1								/* Task API ID */
//#define CPU_COM_TIMER_API		2								/* Timer API ID */


#if 0
#define RBLE_APP_CON_SW_EVT			KE_FIRST_MSG(TASK_CON_APPL) + 3	/* SW API ID */
#define RBLE_APP_CON_SEND_EVT		KE_FIRST_MSG(TASK_CON_APPL) + 4	/* SEND API ID */
#endif


#define  KE_EVT_USR_1_BIT			CO_BIT(31 - KE_EVT_USR_1) 
#define  KE_EVT_USR_2_BIT			CO_BIT(31 - KE_EVT_USR_2) 
#define  KE_EVT_USR_3_BIT			CO_BIT(31 - KE_EVT_USR_3) 

#if 0
extern volatile RBLE_RUNNING_MODE	rBLE_Run_Mode_Flg;
#endif

/* Status Handler */
extern const struct ke_state_handler user_main_state_handler[ USER_MAIN_STATE_MAX ];
/* Default Handler */
extern const struct ke_state_handler user_main_default_handler;
/* Status */
extern ke_state_t user_main_state[ USER_MAIN_IDX_MAX ];



// ============================================================
// ==================���[�U�[�A�v���P�[�V����==================
// ============================================================
// �V�X�e�����[�h ��H1D/G1D����
typedef enum{
	// �d�l��̏�ԉ���
	SYSTEM_MODE_INITIAL = 0,				// �C�j�V����
	SYSTEM_MODE_IDLE_COM,					// �A�C�h��_�ʐM�ҋ@
	SYSTEM_MODE_SENSING,					// �Z���V���O
	SYSTEM_MODE_GET,						// �f�[�^�擾
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
	EVENT_G1D_PRG,				// �v���O������������(G1D)
	EVENT_SELF_CHECK_COM,		// ���Ȑf�f(�ʐM)
	EVENT_COMPLETE,				// ����
	EVENT_STOP,					// ���f
	EVENT_TIME_OUT,				// �^�C���A�E�g
	EVENT_KENSA_ON,				// �����|�[�gON
	EVENT_MAX,					// �ő�
}EVENT_NUM;

/* �ُ�ID��`(�f�o�b�O�@�\) */
typedef enum{
	ERR_ID_EEP = 1					,			/* EEP(�ėp) */
	ERR_ID_I2C						,			/* I2C(�ėp) */
	ERR_ID_MAIN						,			/* MAIN(�ėp) */
	ERR_ID_CPU_COM					,			/* CPU�ԒʐM(�ėp) */
	ERR_ID_ACL						,			/* �����x�Z���T(�ėp) */

	ERR_ID_CPU_COM_ERR = 40			,			/* CPU�ԒʐM�ُ� */
	ERR_ID_CPU_COM_RCV_RING			,			/* CPU�ԒʐM��M�����O�o�b�t�@ �����Z���̓����O�o�b�t�@���������Ȃ��̂Ŕ������Ă��܂� */
	ERR_ID_DRV_UART_OERR = 50		,			/* UART�h���C�o(�I�[�o�[�����G���[) */
	
	// �����x���ُ�
	ERR_ID_BLE_SEND_ERR = 97	,				/* BLE���M�ُ� */
	ERR_ID_CPU_COM_RETRYOUT = 98	,			/* CPU�ԒʐM���g���C�A�E�g(�v���I) */
	ERR_ID_LOGIC = 99				,			/* ���W�b�N�s�(�ėp) */

	ERR_ID_MAX									/* �ُ�ID�ő�	*/
}ERR_ID;

// �d�r�c��臒l
#define BAT_LEVEL_1_VAL						(UH)( 1023.0 * (1.95 / 3.0 ))		// 1.95V�ȏ�
#define BAT_LEVEL_2_VAL						(UH)( 1023.0 * (1.72 / 3.0 ))		// 1.72V�ȏ�
#define BAT_LEVEL_3_VAL						(UH)( 1023.0 * (1.5 / 3.0 ))		// 1.5V�ȏ�
#define BAT_LEVEL_CHG_FIN_VAL				(UH)( 1023.0 * (2.0 / 3.0 ))		// 2.0V�ȏ�

// �d�r�c�ʏ��
#define BAT_LEVEL_STS_MAX					0	// �[�d���^��
#define BAT_LEVEL_STS_HIGH					1	// ��������
#define BAT_LEVEL_STS_LOW					2	// �c��P�������Ȃ�
#define BAT_LEVEL_STS_MIN					3	// �d�r�c�ʂȂ�

// �̂̌���
#define BODY_DIRECTION_LEFT					0	// ������
#define BODY_DIRECTION_UP					1	// �����
#define BODY_DIRECTION_RIGHT				2	// �E����
#define BODY_DIRECTION_DOWN					3	// ������

#define BODY_DIRECTION_MASK					3	// bit�}�X�N�p
#define BODY_DIRECTION_BIT					2	// �g�pbit��

// ���샂�[�h
#define ACT_MODE_NORMAL						0	// �ʏ퓮�샂�[�h
#define ACT_MODE_MONITOR					1	// ���j�^�����O���[�h

// �}������ő�p������
#define SET_MAX_YOKUSEI_CONT_5_MIN			0	// 5��
#define SET_MAX_YOKUSEI_CONT_10_MIN			1	// 10��
#define SET_MAX_YOKUSEI_CONT_NON			2	// �ݒ肵�Ȃ�

// �v���O�����V�[�P���X
typedef enum{
	PRG_SEQ_IDLE = 0,					// �A�C�h��
	PRG_SEQ_READY_WAIT,					// �J�n�҂��@���u�[�g�ւ̐ؑ�
	PRG_SEQ_START_WAIT,					// �J�n�҂��@���A�v���ւ̐ؑ�
	PRG_SEQ_ERASE_WAIT,					// �����҂��@���t���b�V������
	PRG_SEQ_COMPLETE_WAIT,				// �����҂��@�����ʒʒm
	PRG_SEQ_MAX
}PRG_SEQ;


// �^�C�}�[�֘A
#define			MAIN_STATUS_REQ_TIME			50		// �X�e�[�^�X�v������[500ms]
//#define			MAIN_STATUS_REQ_TIME			100		// �X�e�[�^�X�v������[1s]

#define			SENSING_END_JUDGE_TIME				100		// �Z���V���O�Ȃ�[1s]

#define		TIME_CNT_DISP_SELF_CHECK_ERR			(300)		/* ���Ȑf�f�ُ�\��(3�b) */
#define		TIME_CNT_DISP_SELF_CHECK_FIN			(300)		/* ���Ȑf�f�����\��(3�b) */


#define		TIME_CNT_BAT_LEVEL_MIN_INTERVAL		(100)		/* �d�r�c�ʒቺ�ʒm�C���^�[�o��(1�b) */

#define		BAT_LEVEL_GET_CYC						600		// 600�b(10��)

#define		HOUR12_CNT_50MS		(UW)( 12L * 60L * 60L * (1000L / 50L))	//12���Ԃ̃J�E���g�l[50ms]

#define		SENSING_CNT_MIN							(40)		/* �Z���V���O�񐔂̉���(20��) */
//#define		SENSING_CNT_MIN							(1)		/* �Z���V���O�񐔂̉���(30�b)[�f�o�b�O�p�Z�k��] */

#define		MAX_YOKUSEI_CONT_TIME_5_MIN_CNT		30		// 10�b�� 1��(6) * 5 = 30�� = 5��
#define		MAX_YOKUSEI_CONT_TIME_10_MIN_CNT	60		// 10�b�� 1��(6) * 10 = 60�� = 10��
#define		MAX_YOKUSEI_CONT_TIME_NON_CNT		4320	// 10�b�� 1��(6) * 60 * 12 = 4320�� = 720�� = 12����

// �����
#define		MEAS_SEKIGAI_CNT_MAX		140
#define		MEAS_SEKISHOKU_CNT_MAX		140
#define		MEAS_KOKYU_CNT_MAX			200
#define		MEAS_IBIKI_CNT_MAX			200
#if FUNC_DEBUG_LOG == ON
#define		MEAS_ACL_CNT_MAX			20
#else
#define		MEAS_ACL_CNT_MAX			3
#endif

// ���Z���ʏ������݃^�C�~���O
#define		CALC_RESULT_WR_CYC			30			// 30�b
//#define		CALC_RESULT_WR_CYC			3			// 3�b[�f�o�b�O�p�Z�k��]



// �b�^�C�}�[ ��+1�����l��ݒ� // RD8001�b��F�{���͌o�ߎ���[10ms]���g������
#define		TIMER_SEC_PRG_READY_WAIT	( 1 + 1 )
#define		TIMER_SEC_PRG_START_WAIT	( 1 + 1 )
#define		TIMER_SEC_PRG_ERASE_WAIT	( 27 + 1 )

// �V�X�e�����[�h�^�C�}�A�E�g[10ms]
#define		TIME_OUT_SYSTEM_MODE_IDLE_COM		( 18000 )

// �f�[�^�b�ԃt�F�C�Y(���т�,�ċz,�̂̌���,�t�H�g�Z���T����)
typedef enum{
	SEC_PHASE_0_10 = 0,					// 0�`10�b�܂�
	SEC_PHASE_10_20,					// 10�`20�b�܂�
	SEC_PHASE_20_30,					// 20�`30�b�܂�
	SEC_PHASE_NUM,
}SEC_PHASE;




#define	OK_NOW_UPDATING					0		//�X�V����
#define	OK_UPDATE_FIX					1		//���튮��(����)
#define	NG_UPDATE_FAILURE				2		//�ُ튮��(���s)

#define	PRG_HD_UPDATE_STATE_NON			0	//�X�V����
#define	PRG_HD_UPDATE_STATE_OK			1	//���튮��(����)
#define	PRG_HD_UPDATE_STATE_NG			2	//�ُ튮��(���s)




#define	BD_ADRS_NUM						6			// BLE�A�h���X��


///10ms timer
#define PERIOD_5SEC     500U
#define PERIOD_1SEC     100U
#define PERIOD_10MSEC   1U		//RD8001�Ή��F��`�ǉ�
#define PERIOD_20MSEC   2U		//RD8001�Ή��F��`�ǉ�
#define PERIOD_50MSEC   5U		//RD8001�Ή��F��`�ǉ�
#define PERIOD_100MSEC   10U

///20ms timer
#define	TIME_20MS_CNT_POW_SW_LONG			100				/* �d��SW_��(2�b) */
#define	TIME_20MS_CNT_POW_SW_SHORT			5				/* �d��SW_�Z(0.1�b) */


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
			UH photoref_val;			// �t�H�g���t���N�^
		}dat;
	}info;
}MEAS;

// ���Z���
typedef struct{
	union{
		UB	byte[EEP_CACL_DATA_SIZE];
		struct{
			UB	ibiki_val[SEC_PHASE_NUM];	// ���т��̑傫��
			UB	state;						// ���
			UB	body_direct;				// �̂̌���
			UB	photoref[SEC_PHASE_NUM];	// �t�H�g�Z���T�[
			UB	dummy[4];					// ���E�l�����p
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
			UB	act_mode;		// ���샂�[�h
			UB	ibiki_sens;		// ���т����x
			UB	yokusei_str;	// �}�����x
			UB	yokusei_max_time;	// �}������ő�p������
		}dat;
	}info;
}ALARM;


// ���Ȑf�f
typedef struct{
	UW	last_time;
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
	}info;
}H1D_INFO;


// �t���[��(�g)�ԍ�
typedef struct{
	UB read;		// �t���[��(�g)�̓ǂݏo���ԍ�
	UB write;		// �t���[��(�g)�̏������ݔԍ�
	UB cnt;		// �t���[��(�g)�̏������ݐ��@���������ݐ�0�ƍő�̋�ʂ����Ȃ���
}FRAME_NUM_INFO;

typedef struct{
	
	SYSTEM_MODE system_mode;			/* �V�X�e�����[�h */
	SYSTEM_MODE next_system_mode;		/* ���̃V�X�e�����[�h */
	SYSTEM_MODE last_system_mode;		/* �O��̃V�X�e�����[�h */
	
	UW system_mode_time_out_cnt;		/* �V�X�e�����[�h�^�C���A�E�g�J�E���g[10ms] */

	DATE date;
	
	UW last_time_sts_req;			//�O�񎞊�(�X�e�[�^�X�v��)
	UW last_sensing_data_rcv;		//�O��Z���V���O�f�[�^��M
	UB sensing_flg;					// �Z���V���O���t���O
	
	UB battery_sts;			// �d�r���
	H1D_INFO h1d;			// H1D���
	H1D_INFO h1d_last;		// H1D���(�O��)
	
	
	// ���Z�֘A
	CALC calc;				// ���Z��f�[�^
	UH calc_cnt;			// ���Z�J�E���g(�ۑ��f�[�^��)
	UH ibiki_detect_cnt;	// ���т����m��
	UH mukokyu_detect_cnt;	// ���ċz���m��
	UH ibiki_time;			// ���т�����
	UH mukokyu_time;		// ���ċz����
	UH max_mukokyu_sec;		// �ő喳�ċz[�b]
	
	UB phase_ibiki;			// �b�ԃt�F�C�Y(���т�)
	UB phase_kokyu;			// �b�ԃt�F�C�Y(�ċz)
	UB phase_body_direct;	// �b�ԃt�F�C�Y(�̂̌���)
	UB phase_photoref;		// �b�ԃt�F�C�Y(�t�H�g�Z���T)
	
	// �t���[��(�g)�ԍ�
	FRAME_NUM_INFO frame_num;			// �t���[��(�g)�ԍ�[EEP�R�s�[�G���A] ��EEP�Ƃ͈�v�����Ă���
	FRAME_NUM_INFO frame_num_work;		// �t���[��(�g)�ԍ����[�N

	ALARM	alarm;						// �x���@�\[EEP�R�s�[�G���A] ��EEP�Ƃ͈�v�����Ă���
	
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
	
	
	UW	timer_sec;			// �^�C�}�[[�b]�@���J�E���g�_�E��
	
	// timer
	UH tick_10ms;
	UH tick_10ms_sec;
	UH tick_10ms_new;
	UW elapsed_time;									/* �o�ߎ���(10ms) ����1�N132���p�����ĉ��Z�\ */
	UH tick_vib_10ms_sec;
	UH tick_led_10ms_sec;
	UW sensing_cnt_50ms;		// �Z���V���O�I��[50ms]
	UW cnt_time_50ms;		//�}���p�����ԗp�J�E���g
	
	UW last_time_battery_level_min;			// �d�r�c�ʒቺ����[10ms]
	UW sw_time_cnt;							// �d��SW�������ԃJ�E���^
	UB pow_sw_last;							// �d���{�^�����(�O��)
	
	// �ȍ~���[�N�̈�
	UW sec30_cnt;			//30�b�J�E���g
	UW sec10_cnt;			//10�b�J�E���g
	UW sec7_cnt;			//7�b�J�E���g
	UW sec600_cnt;			//10���J�E���g
	UW sec10_led_cnt;		//LED�p10�b�J�E���g
	
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
	
	// �ُ�֘A�f�o�b�O�p�r
	UW err_cnt;				//�ُ��(�f�o�b�O�p�r)
	ERR_ID last_err_id;		//�O��ُ�ID(�f�o�b�O�p�r)
	
	// 
	MEAS meas;				/* �v���l(50ms) */
	UH acl_timing;
	
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
//	VUART_CMD_TYPE_PRG_H1D_DATA,					// �v���O�����]��(�f�[�^)
//	VUART_CMD_TYPE_PRG_H1D_RESULT,					// �v���O�����]������
//	VUART_CMD_TYPE_PRG_H1D_CHECK,					// �v���O�����X�V�����m�F
	VUART_CMD_TYPE_DEVICE_SET,						// �f�o�C�X�ݒ�ύX
//	VUART_CMD_TYPE_ALARM_SET,						// �ݒ�ύX
//	VUART_CMD_TYPE_ALARM_INFO,						// �A���[���ʒm
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

//#define	VUART_CMD_ALARM_SET		0xC0

#define	VUART_CMD_INFO			0xC2
#define	VUART_CMD_VERSION		0xC3
#define	VUART_CMD_DEVICE_INFO	0xC5
//#define	VUART_CMD_ALARM_INFO	0xC4
#define	VUART_CMD_DEVICE_SET	0xC6
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

//#define	VUART_CMD_LEN_ALARM_SET		8
#define	VUART_CMD_LEN_DEVICE_SET	5

#define	VUART_CMD_ONLY_SIZE			1	// �R�}���h�݂̂̃T�C�Y

// ���M�f�[�^�� ���R�}���h���܂�
#define	VUART_SND_LEN_INFO			3
#define	VUART_SND_LEN_VERSION		6
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

// �����x�Z���T
#define ACL_DEVICE_ADR			0x1C				// �����x�Z���T�f�o�C�X�A�h���X
#if FUNC_DEBUG_LOG == ON
#define ACL_TIMING_VAL			10					// �����x�Z���T�E�t�H�g�Z���T�����^�C�~���O
#else
#define ACL_TIMING_VAL			100					// �����x�Z���T�E�t�H�g�Z���T�����^�C�~���O
#endif
#define I2C_LOCK_ERR_VAL		1000				// ���b�N�ُ픻��臒l

#define I2C_WAIT		255					// �X�^�[�g�R���f�B�V�����҂� ��200us���x�Ȃ̂ōő�l��ݒ肵�Ă���

// ���W�X�^�A�h���X
#define ACL_REG_ADR_WHO_AM_I			0x0F				// WHO AM I
#define ACL_REG_ADR_DATA_XYZ			0x06				// XOUT,YOUT,ZOUT
#define ACL_REG_ADR_INT_SRC1			0x16				// INT_SOURCE1
#define ACL_REG_ADR_INT_REL				0x1A				// INT_REL
#define ACL_REG_ADR_CTRL_REG1			0x1B				// CTRL_REG1
#define ACL_REG_ADR_CTRL_REG2			0x1F				// CTRL_REG2


// ���W�X�^�f�[�^
#define ACL_REG_RECOGNITION_CODE		0x35				// �F���R�[�h(0x35)

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
extern void err_info( ERR_ID id );
extern void main_cpu_com_snd_pc_log( UB* data, UB size );
extern void main_set_bd_adrs( UB* bda);
extern void user_main_timer_10ms_set( void );
extern void user_main_timer_cyc( void );
extern UW time_get_elapsed_time( void );
extern void reset_vib_timer( void );
extern void reset_led_timer( void );
extern void vib_cyc( void );
extern void set_vib_flg( bool flg );
extern void main_set_battery( void );

#endif // __MAIN_USR_INC__
