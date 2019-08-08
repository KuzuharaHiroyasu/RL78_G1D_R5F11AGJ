/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : user_main_tbl.h												*/
/* �@�\         : �S�̊Ǘ��i�e�[�u����`�j										*/
/* �ύX����     : 2018.01.25 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
/************************************************************/
/* �v���b�g�t�H�[��											*/
/************************************************************/



static const struct ke_msg_handler cpu_com_handler[] =
{
	{ USER_MAIN_CYC_ACT			, (ke_msg_func_t)user_main_cyc },
//	{ USER_MAIN_CALC_SEKIGAI	, (ke_msg_func_t)main_calc_sekigai },
//	{ USER_MAIN_CALC_SEKISHOKU	, (ke_msg_func_t)main_calc_sekishoku },
	{ USER_MAIN_CALC_KOKYU		, (ke_msg_func_t)main_calc_kokyu },
	{ USER_MAIN_CALC_IBIKI		, (ke_msg_func_t)main_calc_ibiki },
	{ USER_MAIN_CALC_ACL		, (ke_msg_func_t)main_calc_acl },
	{ USER_MAIN_CYC_CALC_RESULT	, (ke_msg_func_t)user_main_calc_result_cyc },
	{ USER_MAIN_CYC_BATTERY		, (ke_msg_func_t)battery_level_cyc },
	{ USER_MAIN_CYC_LED			, (ke_msg_func_t)led_cyc },
	{ USER_MAIN_CYC_PHOTOREF	, (ke_msg_func_t)main_calc_photoref },
};

/* Status Handler */
const struct ke_state_handler user_main_state_handler[ USER_MAIN_STATE_MAX ] =
{   /* State message handlers */
    KE_STATE_HANDLER(cpu_com_handler)
};

/* Default Handler */
const struct ke_state_handler user_main_default_handler = KE_STATE_HANDLER_NONE;

/************************************************************/
/* ���[�U�[��`												*/
/************************************************************/
/* �o�[�W�����\�L�̒��ӎ��� */
const B		version_product_tbl[]= {0, 0, 1, 1};				/* �\�t�g�E�F�A�o�[�W���� */
																/* �o�[�W�����\�L���[�� */
																/* �@���W���[�o�[�W�����F[0 �` 99] */
																/* �A�}�C�i�[�o�[�W�����F[0 �` 9] */
																/* �B���r�W�����F[0 �` 99] */
																/* �C�r���h�o�[�W�����F[0 �` 99] */


/* CPU�ԒʐM��M�f�[�^���� �֐��e�[�u�� */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* �R�}���h */		/* �֐�  */					/* �����L�� */
	{	0x00,			NULL,								OFF	},	/* �yCPU�ԒʐM�R�}���h�z�R�}���h����				*/
	{	0xE0,			main_cpu_com_rcv_sts_res,			OFF	},	/* �yCPU�ԒʐM�R�}���h�z�X�e�[�^�X�v��				*/
	{	0xA0,			main_cpu_com_rcv_sensor_res,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�Z���T�[�f�[�^�X�V			*/
	{	0xA1,			NULL,								OFF	},	/* �yCPU�ԒʐM�R�}���h�z�Z���V���O�w��				*/
	{	0xB0,			main_cpu_com_rcv_mode_chg,			OFF	},	/* �yCPU�ԒʐM�R�}���h�z��ԕύX(G1D)				*/
	{	0xF0,			NULL,								OFF	},	/* �yCPU�ԒʐM�R�}���h�zPC���O���M(�����R�}���h)	*/
	{	0xB1,			main_cpu_com_rcv_date_set,			OFF	},	/* �yCPU�ԒʐM�R�}���h�z�����ݒ�					*/
	{	0xD5,			main_cpu_com_rcv_prg_hd_ready,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	{	0xD2,			main_cpu_com_rcv_prg_hd_start,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n		*/
	{	0xD4,			main_cpu_com_rcv_prg_hd_erase,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	{	0xD0,			main_cpu_com_rcv_prg_hd_data,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���f�[�^		*/
	{	0xD1,			main_cpu_com_rcv_prg_hd_reslut,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	{	0xD3,			main_cpu_com_rcv_prg_hd_check,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���m�F		*/
	{	0xB2,			main_cpu_com_rcv_disp_order,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�\���w��				*/
	{	0xB3,			main_cpu_com_rcv_version,			OFF	},	/* �yCPU�ԒʐM�R�}���h�z�o�[�W����				*/
};

/* VUART�ʐM��M�f�[�^���� �֐��e�[�u�� */
STATIC const VUART_RCV_CMD_TBL s_vuart_rcv_func_tbl[VUART_CMD_TYPE_MAX] = {
	/* �R�}���h */				/* �����O�X */				/* �֐�  */					/* �����L�� */
	{	0x00,					0,							NULL							},	// �Ȃ�
	{	VUART_CMD_MODE_CHG,		VUART_CMD_LEN_MODE_CHG,		main_vuart_rcv_mode_chg			},	// ��ԕύX(G1D)
	{	VUART_CMD_DATE_SET,		VUART_CMD_LEN_DATE_SET,		main_vuart_rcv_date				},	// �����ݒ�
	{	VUART_CMD_INFO,			VUART_CMD_LEN_INFO,			main_vuart_rcv_info				},	// ���擾
	{	VUART_CMD_VERSION,		VUART_CMD_LEN_VERSION,		main_vuart_rcv_version			},	// �o�[�W�����擾
	{	VUART_CMD_DEVICE_INFO,	VUART_CMD_LEN_DEVICE_INFO,	main_vuart_rcv_device_info		},	// �f�o�C�X�󋵎擾
	{	VUART_CMD_DATA_NEXT,	VUART_CMD_LEN_DATA_NEXT,	NULL							},	// NEXT[���M��p]
	{	VUART_CMD_DATA_END,		VUART_CMD_LEN_DATA_END,		main_vuart_rcv_data_end			},	// END[��M��SET��]
	{	VUART_CMD_DATA_FRAME,	VUART_CMD_LEN_DATA_FRAME,	main_vuart_rcv_data_frame		},	// �g���(������)[��M��SET��]
	{	VUART_CMD_DATA_CALC,	VUART_CMD_LEN_DATA_CALC,	main_vuart_rcv_data_calc		},	// �@��f�[�^[��M��SET��]
	{	VUART_CMD_DATA_FIN,		VUART_CMD_LEN_DATA_FIN,		main_vuart_rcv_data_fin			},	// �f�[�^�擾�����ʒm
	{	VUART_CMD_INVALID,		VUART_CMD_LEN_PRG_DATA,		main_vuart_rcv_prg_hd_record	},	// �v���O�����]��(�f�[�^)
	{	VUART_CMD_PRG_RESULT,	VUART_CMD_LEN_PRG_RESULT,	main_vuart_rcv_prg_hd_result	},	// �v���O�����]������
	{	VUART_CMD_PRG_CHECK,	VUART_CMD_LEN_PRG_CHECK,	main_vuart_rcv_prg_hd_update	},	// �v���O�����X�V�����m�F
	{	VUART_CMD_DEVICE_SET,	VUART_CMD_LEN_DEVICE_SET,	main_vuart_rcv_device_set		},	// �f�o�C�X�ݒ�ύX
//	{	VUART_CMD_ALARM_SET,	VUART_CMD_LEN_ALARM_SET,	main_vuart_rcv_alarm_set		},	// �A���[���ݒ�ύX
//	{	VUART_CMD_ALARM_INFO,	0,							NULL							},	// �A���[���ʒm[���M��p]
};

/* ���[�h�ʏ��� */
typedef void (*MODE_FUNC)(void);
STATIC MODE_FUNC p_user_main_mode_func[] = {								user_main_mode_inital,			// SYSTEM_MODE_INITIAL			�C�j�V����
																			user_main_mode_idle_rest,		// SYSTEM_MODE_IDLE_REST		�A�C�h��_�c�ʕ\��
																			user_main_mode_idle_com,		// SYSTEM_MODE_IDLE_COM			�A�C�h��_�ʐM�ҋ@
																			user_main_mode_sensing,			// SYSTEM_MODE_SENSING			�Z���V���O
																			user_main_mode_get,				// SYSTEM_MODE_GET				�f�[�^�擾
																			user_main_mode_prg_h1d,			// SYSTEM_MODE_PRG_H1D			H1D�v���O�����X�V
																			user_main_mode_prg_g1d,			// SYSTEM_MODE_PRG_G1D			G1D�v���O�����X�V
																			user_main_mode_self_check,		// SYSTEM_MODE_PRG_SELF_CHECK	���Ȑf�f
																			user_main_mode_move,			// SYSTEM_MODE_MOVE				�ڍs
																			dummy,							// SYSTEM_MODE_NON				�Ȃ�
};

typedef SYSTEM_MODE ( *EVENT_TABLE )( int event );
STATIC EVENT_TABLE p_event_table[ EVENT_MAX ][ SYSTEM_MODE_MAX ] = {
// ���[�h				INITAL				IDLE_REST			IDLE_COM			SENSING			GET					PRG_H1D				PRG_G1D			SELF_CHECK		MOVE				NON
/*�C�x���g�Ȃ�		*/	{ evt_non,			evt_non,			evt_non,			evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
///*�d��SW(�Z)		*/	{ evt_idle_rest,	evt_sensing,		evt_sensing_chg,	evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�d��SW(�Z)		*/	{ evt_non,			evt_non,			evt_non,			evt_bat_check,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
	///*�d��SW����(��)	*/	{ evt_non,			evt_initial,		evt_initial_chg,	evt_initial,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�d��SW����(��)	*/	{ evt_non,			evt_sensing,		evt_sensing_chg,	evt_idle_com,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�[�d���m�|�[�gON	*/	{ evt_idle_com,		evt_idle_com,		evt_non,			evt_idle_com,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�[�d���m�|�[�gOFF	*/	{ evt_non,			evt_non,			evt_non,			evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�d�r�c�ʒቺ		*/	{ evt_non,			evt_non,			evt_initial,		evt_initial,	evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�[�d����			*/	{ evt_non,			evt_non,			evt_initial,		evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�f�[�^�擾		*/	{ evt_non,			evt_get,			evt_get,			evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�v���O����(H1D)	*/	{ evt_non,			evt_h1d_prg_denchi,	evt_h1d_prg_denchi,	evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�v���O����(G1D)	*/	{ evt_non,			evt_g1d_prg_denchi,	evt_g1d_prg_denchi,	evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*���Ȑf�f(�ʐM)	*/	{ evt_non,			evt_self_check,		evt_self_check,		evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*����				*/	{ evt_non,			evt_non,			evt_non,			evt_idle_com,	evt_idle_com,		evt_prg_h1d_fin,	evt_idle_com,	evt_idle_com,	evt_non,			evt_non },
/*���f				*/	{ evt_non,			evt_non,			evt_non,			evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�^�C���A�E�g		*/	{ evt_non,			evt_idle_com,		evt_initial_chg,	evt_non,		evt_non,			evt_prg_h1d_time_out,evt_non,		evt_non,		evt_non,			evt_non },
/*����				*/	{ evt_idle_com_denchi,evt_self_check,	evt_self_check,		evt_non,		evt_non,			evt_non,			evt_non,		evt_non,		evt_non,			evt_non }
};

// 1�y�[�W256byte�̂O���߃e�[�u��
STATIC const UB s_eep_page0_tbl[EEP_ACCESS_ONCE_SIZE] = { 0 };

