/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : user_main_tbl.h													*/
/* �@�\         : �S�̊Ǘ��i�e�[�u����`�j										*/
/* �ύX����     : 2018.01.25 Axia Soft Design ���� ��	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
/************************************************************/
/* �v���b�g�t�H�[��											*/
/************************************************************/



static const struct ke_msg_handler cpu_com_handler[] =
{
	{ USER_MAIN_CYC_ACT			, (ke_msg_func_t)cpu_com_evt },
	{ USER_MAIN_CALC_SEKIGAI	, (ke_msg_func_t)main_calc_sekigai },
	{ USER_MAIN_CALC_SEKISHOKU	, (ke_msg_func_t)main_calc_sekishoku },
	{ USER_MAIN_CALC_KOKYU		, (ke_msg_func_t)main_calc_kokyu },
	{ USER_MAIN_CALC_IBIKI		, (ke_msg_func_t)main_calc_ibiki },
	{ USER_MAIN_CALC_ACL		, (ke_msg_func_t)main_calc_acl },
	{ USER_MAIN_ACT2			, (ke_msg_func_t)cpu_com_timer_handler },
};

/* Status Handler */
const struct ke_state_handler cpu_com_state_handler[ CPU_COM_STATE_MAX ] =
{   /* State message handlers */
    KE_STATE_HANDLER(cpu_com_handler)
};

/* Default Handler */
const struct ke_state_handler cpu_com_default_handler = KE_STATE_HANDLER_NONE;


















/************************************************************/
/* ���[�U�[��`												*/
/************************************************************/





/* �o�[�W�����\�L�̒��ӎ��� */



const B		version_product_tbl[]= {1, 9, 2, 0};				/* �\�t�g�E�F�A�o�[�W���� */
																/* �o�[�W�����\�L���[�� */
/* �@�A�A�ύX����format_num_init_setting�����킹�鎖 */			/* �@���W���[�o�[�W�����F[0 �` 99] */
																/* �A�}�C�i�[�o�[�W�����F[0 �` 9] �������ݒ�t�H�[�}�b�g�̊֌W��1���̂� */
																/* �B���r�W�����F[0 �` 99] */
																/* �C�r���h�o�[�W�����F[0 �` 99] */


/* ��M�f�[�^���� �֐��e�[�u�� */
STATIC const CPU_COM_RCV_CMD_TBL s_cpu_com_rcv_func_tbl[CPU_COM_CMD_MAX] = {
	/* �R�}���h */		/* �֐�  */					/* �����L�� */
	{	0x00,			NULL,								OFF	},	/* �yCPU�ԒʐM�R�}���h�z�R�}���h����				*/
	{	0xE0,			main_cpu_com_rcv_sts_res,			OFF	},	/* �yCPU�ԒʐM�R�}���h�z�X�e�[�^�X�v��				*/
	{	0xA0,			main_cpu_com_rcv_sensor_res,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�Z���T�[�f�[�^�X�V			*/
	{	0xB0,			main_cpu_com_rcv_mode_chg,			OFF	},	/* �yCPU�ԒʐM�R�}���h�z��ԕύX(G1D)				*/
	{	0xF0,			NULL,								OFF	},	/* �yCPU�ԒʐM�R�}���h�zPC���O���M(�����R�}���h)	*/
	{	0xB1,			main_cpu_com_rcv_date_set,			OFF	},	/* �yCPU�ԒʐM�R�}���h�z�����ݒ�					*/
	{	0xD5,			main_cpu_com_rcv_prg_hd_ready,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	{	0xD2,			main_cpu_com_rcv_prg_hd_start,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���J�n		*/
	{	0xD4,			main_cpu_com_rcv_prg_hd_erase,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	{	0xD0,			main_cpu_com_rcv_prg_hd_data,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���f�[�^		*/
	{	0xD1,			main_cpu_com_rcv_prg_hd_reslut,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]������		*/
	{	0xD3,			main_cpu_com_rcv_prg_hd_check,		OFF	},	/* �yCPU�ԒʐM�R�}���h�z�v���O�����]���m�F		*/
};


/* ���[�h�ʏ��� */
#if 0
STATIC void	(* const p_user_main_mode_func[SYSTEM_MODE_MAX])() 		= {	user_main_mode_idle			,	// SYSTEM_MODE_IDLE			�A�C�h��
																		user_main_mode_sensing		,	// SYSTEM_MODE_SENSING		�Z���V���O
																		user_main_mode_move			,	// SYSTEM_MODE_MOVE			�ڍs
																		user_main_mode_get			,	// SYSTEM_MODE_GET			�f�[�^�擾
																		user_main_mode_prg_hd			// SYSTEM_MODE_PRG_HD		�v���O�����X�V
};
#endif

STATIC void	(* const p_user_main_mode_func[])()						= {	user_main_mode_idle			,	// SYSTEM_MODE_IDLE			�A�C�h��
																		user_main_mode_sensing		,	// SYSTEM_MODE_SENSING		�Z���V���O
																		user_main_mode_move			,	// SYSTEM_MODE_MOVE			�ڍs
																		user_main_mode_get			,	// SYSTEM_MODE_GET			�f�[�^�擾
																		user_main_mode_prg_hd			// SYSTEM_MODE_PRG_HD		�v���O�����X�V
};

