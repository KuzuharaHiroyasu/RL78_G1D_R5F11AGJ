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
	UH year;
	UB month;
	UB day;
	UB hour;
	UB min;
	UB sec;
}DATE;


typedef struct{
	
	SYSTEM_MODE system_mode;		/* �V�X�e�����[�h */
	SYSTEM_MODE next_system_mode;	/* ���̃V�X�e�����[�h */
	
	DATE date;
	
	ke_time_t last_time;			//�O�񎞊�
	ke_time_t last_sensing_data_rcv;		//�O��Z���V���O�f�[�^��M
	
	UB denchi_sts;			// �d�r���
	
	// ���Z�֘A
	CALC calc;				// ���Z��f�[�^
	UH calc_cnt;			// ���Z�J�E���g
	UH max_mukokyu_sec;		// �ő喳�ċz[�b]
	
	UB frame_num_read;		// �t���[��(�g)�̓ǂݏo���ԍ�
	UB frame_num_write;		// �t���[��(�g)�̏������ݔԍ�
	
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
	UB	sekishoku_cnt;	// �������ׂ͂̈ɕ�������
	UB	kokyu_cnt;		
	UB	ibiki_cnt;		
	UB	acl_cnt;
	
	
	
	
	// �ȍ~���[�N�̈�
	UW sec30_cnt;			//30�b�J�E���g
	UW sec10_cnt;			//10�b�J�E���g
	UW sec7_cnt;			//7�b�J�E���g
	
	UB get_mode_status;
	UB get_mode_calc_cnt;
	UH get_wait_cnt;		// RD8001�b��F�X�}�z������������ƃp�P�b�g��������	
	
	
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
#define VUART_DATA_SIZE_MAX				20

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
extern void user_main_init( void );

#endif // __MAIN_USR_INC__
