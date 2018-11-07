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

// �V�X�e�����[�h
typedef enum{
	SYSTEM_MODE_IDLE = 0,					// �A�C�h��
	SYSTEM_MODE_SENSING,					// �Z���V���O
	SYSTEM_MODE_MOVE,						// �ڍs
	SYSTEM_MODE_GET,						// �f�[�^�擾
	SYSTEM_MODE_PRG_HD,						// �v���O�����X�V
	SYSTEM_MODE_MAX
}SYSTEM_MODE;


//#define			MAIN_STATUS_REQ_TIME			50		// �X�e�[�^�X�v������[500ms]
#define			MAIN_STATUS_REQ_TIME			100		// �X�e�[�^�X�v������[1s]

#define			SENSING_END_JUDGE_TIME				100		// �X�e�[�^�X�v������[1s]



#define			SYSTEM_MODE_HD_CHG_NON				0		// �Ȃ�
#define			SYSTEM_MODE_HD_CHG_SENSING			1		// ����(�Z���V���O)
#define			SYSTEM_MODE_HD_CHG_IDLE				2		// ����(�A�C�h��)




#define		MEAS_SEKIGAI_CNT_MAX		140
#define		MEAS_SEKISHOKU_CNT_MAX		140
#define		MEAS_KOKYU_CNT_MAX			200
#define		MEAS_IBIKI_CNT_MAX			200
#define		MEAS_ACL_CNT_MAX			20


// ������
typedef struct{
	union{
		UB	byte[CPU_COM_SND_DATA_SIZE_SENSOR_DATA];
		struct{
			H	sekigaival;		// �������ׂ͂̈ɕ�������
			H	sekishoku_val;	// �������ׂ͂̈ɕ�������
			UH	kokyu_val;		
			UH	ibiki_val;		
			B acl_x;
			B acl_y;
			B acl_z;
			B dummy;			// ��[�p�f�B���O����]
		}dat;
	}info;
}MEAS;


#define CACL_DATA_SIZE		8		//�_�~�[�܂�
typedef struct{
	union{
		UB	byte[CACL_DATA_SIZE];
		struct{
			UB	state;			/* ��� */
			UB	ibiki_val_u;	/* ���т���ʃo�C�g */
			UB	ibiki_val_d;	/* ���т����ʃo�C�g */
			UB	myaku_val;
			UB	spo2_val;
			UB	kubi;
			UB	aki;			/* �� */
			UB	dummy;			/* ���E�l�����p */
		}dat;
	}info;
}CALC;





// EEP�������ݗp���R�[�h(�A�h���X�t��)
typedef struct{
	union{
		UB	byte[EEP_RECORD_SIZE + EEP_ADRS_SIZE];
		struct{
			UH		wr_adrs;				/* EEP�A�h���X */
			UH	kokyu_val;		
			UH	ibiki_val;		
			W	sekishoku_val;	// �������ׂ͂̈ɕ�������
			W	sekigaival;		// �������ׂ͂̈ɕ�������
			UB	valid;			/* ���R�[�h�L��/����				*/
			B acl_x;
			B acl_y;
			B acl_z;
		}data;
	}record;
}WR_EEP_RECORD;



// EEP���R�[�h[���Z��f�[�^]
typedef struct{
	union{
		UB	byte[EEP_RECORD_SIZE];
		struct{
			UH	kokyu_val;		
			UH	ibiki_val;		
			W	sekishoku_val;	// �������ׂ͂̈ɕ�������
			W	sekigaival;		// �������ׂ͂̈ɕ�������
			UB	valid;			/* ���R�[�h�L��/����				*/
			B acl_x;
			B acl_y;
			B acl_z;
#if 0
			double	ibiki_val;				/* ���т� */
			double	myaku_val_sekishoku;	/* ����_�ԊO */
			double	myaku_val_sekigai;		/* ����_�ԐF */
			double	spo2_val_normal;		/* SPO2_�ʏ� */
			double	spo2_val_konica;		/* SPO2 */
			UB state_flg;					/* �t���O(�ċz���,�����X�e�[�W,���т��L�� */
			UB	valid;						/* ���R�[�h�L��/���� */
#endif
		}data;
	}record;
}EEP_RECORD;



typedef struct{
	
	SYSTEM_MODE system_mode;			/* �V�X�e�����[�h */
	SYSTEM_MODE next_system_mode;	/* ���̃V�X�e�����[�h */
	UB main_cyc_req;		/* ���C�������v��(20ms) */
	
	UH eep_wr_record_cnt;	/* �������݃��R�[�h */
	UH eep_rd_record_cnt;	/* �ǂݏo�����R�[�h */	
	// �v���l(20ms)
	UH dench_val;
#if 0
	UH kokyu_val;		
	UH ibiki_val;		
	W sekishoku_val;	// �������ׂ͂̈ɕ�������
	W sekigaival;		// �������ׂ͂̈ɕ�������
	B acl_x;			// �����x�Z���T(8bit)
	B acl_y;
	B acl_z;
#endif
	EEP_RECORD	eep;		// �f�[�^���R�[�h(EEP�ǂݏ����AUART���M)
	
	UB hour;
	UB min;
	UB sec;
	
	
	
	UB sensing_start_trig;		// �Z���V���O�J�n�g���K
	
	UB pow_sw_last;				// �d���{�^�����(�O��)
	
	ke_time_t last_time;			//�O�񎞊�
	ke_time_t last_sensing_data_rcv;		//�O��Z���V���O�f�[�^��M
	
	// ���Z�֘A
	UB calc_cnt;
	CALC calc;		//���Z��f�[�^
	
	// �@��f�[�^(���Z�O)
	H	sekigai_val[MEAS_SEKIGAI_CNT_MAX];		// �������ׂ͂̈ɕ�������
	H	sekishoku_val[MEAS_SEKISHOKU_CNT_MAX];	// �������ׂ͂̈ɕ�������
	UH	kokyu_val[MEAS_KOKYU_CNT_MAX];		
	UH	ibiki_val[MEAS_IBIKI_CNT_MAX];		
	B	acl_x[MEAS_ACL_CNT_MAX];
	B	acl_y[MEAS_ACL_CNT_MAX];
	B	acl_z[MEAS_ACL_CNT_MAX];
	
	UB	sekigai_cnt;
	UB	sekishoku_cnt;	// �������ׂ͂̈ɕ�������
	UB	kokyu_cnt;		
	UB	ibiki_cnt;		
	UB	acl_cnt;
	
	
	UW sec30_cnt;			//30�b�J�E���g
	UW sec10_cnt;			//10�b�J�E���g
	UW sec7_cnt;			//7�b�J�E���g
	
	
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
	void (*func)(UB *p_data);		/* ��M���� */
	UB res;							/* �����L(ON)��(OFF) */
}CPU_COM_RCV_CMD_TBL;


/* DS�\���� */
typedef struct{
	DS_CPU_COM 		cpu_com;		/* CPU�ԒʐM�� */
}DS;










/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void codeptr app_evt_usr_2(void);
extern void codeptr app_evt_usr_3(void);
extern void ds_get_cpu_com_order( DS_CPU_COM_ORDER **p_data );
extern void ds_set_cpu_com_input( DS_CPU_COM_INPUT *p_data );

#endif // __MAIN_USR_INC__
