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
#if FUNC_DEBUG_LOG != ON
	{ USER_MAIN_CYC_ACT			, (ke_msg_func_t)user_main_cyc },
	{ USER_MAIN_CALC_KOKYU		, (ke_msg_func_t)main_calc_kokyu },
	{ USER_MAIN_CALC_IBIKI		, (ke_msg_func_t)main_calc_ibiki },
	{ USER_MAIN_CYC_VUART		, (ke_msg_func_t)main_vuart_cyc },	
#endif
	{ USER_MAIN_CALC_ACL		, (ke_msg_func_t)main_calc_acl },
	{ USER_MAIN_CYC_CALC_RESULT	, (ke_msg_func_t)user_main_calc_result_cyc },
	{ USER_MAIN_CYC_BATTERY		, (ke_msg_func_t)battery_level_cyc },
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
const B		version_product_tbl[]= {0, 0, 4,99};				/* �\�t�g�E�F�A�o�[�W���� */
																/* �o�[�W�����\�L���[�� */
																/* �@���W���[�o�[�W�����F[0 �` 99] */
																/* �A�}�C�i�[�o�[�W�����F[0 �` 9] */
																/* �B���r�W�����F[0 �` 99] */
																/* �C�r���h�o�[�W�����F[0 �` 99] */

/* VUART�ʐM��M�f�[�^���� �֐��e�[�u�� */
STATIC const VUART_RCV_CMD_TBL s_vuart_rcv_func_tbl[VUART_CMD_TYPE_MAX] = {
	/* �R�}���h */					/* �����O�X */					/* �֐�  */							/* �����L�� */
	{	0x00,						0,								NULL							},	// �Ȃ�
	{	VUART_CMD_MODE_CHG,			VUART_CMD_LEN_MODE_CHG,			main_vuart_rcv_mode_chg			},	// ��ԕύX(G1D)
	{	VUART_CMD_DATE_SET,			VUART_CMD_LEN_DATE_SET,			main_vuart_rcv_date				},	// �����ݒ�
	{	VUART_CMD_INFO,				VUART_CMD_LEN_INFO,				main_vuart_rcv_info				},	// ���擾
	{	VUART_CMD_VERSION,			VUART_CMD_LEN_VERSION,			main_vuart_rcv_version			},	// �o�[�W�����擾
	{	VUART_CMD_DEVICE_INFO,		VUART_CMD_LEN_DEVICE_INFO,		main_vuart_rcv_device_info		},	// �f�o�C�X�󋵎擾
	{	VUART_CMD_DATA_NEXT,		VUART_CMD_LEN_DATA_NEXT,		NULL							},	// NEXT[���M��p]
	{	VUART_CMD_DATA_END,			VUART_CMD_LEN_DATA_END,			main_vuart_rcv_data_end			},	// END[��M��SET��]
	{	VUART_CMD_DATA_FRAME,		VUART_CMD_LEN_DATA_FRAME,		main_vuart_rcv_data_frame		},	// �g���(������)[��M��SET��]
	{	VUART_CMD_DATA_CALC,		VUART_CMD_LEN_DATA_CALC,		main_vuart_rcv_data_calc		},	// �@��f�[�^[��M��SET��]
	{	VUART_CMD_DATA_FIN,			VUART_CMD_LEN_DATA_FIN,			main_vuart_rcv_data_fin			},	// �f�[�^�擾�����ʒm
	{	VUART_CMD_DEVICE_SET,		VUART_CMD_LEN_DEVICE_SET,		main_vuart_rcv_device_set		},	// �f�o�C�X�ݒ�ύX
	{	VUART_CMD_VIB_CONFIRM,		VUART_CMD_LEN_VIB_CONFIRM,		main_vuart_rcv_vib_confirm		},	// �o�C�u����m�F
	{	VUART_CMD_VIB_STOP,			VUART_CMD_LEN_VIB_STOP,			main_vuart_rcv_vib_stop			},	// �o�C�u�����~
};

/* VUART�ʐM��M�f�[�^���� �֐��e�[�u�� */
STATIC const VUART_RCV_CMD_TBL s_vuart_diag_rcv_func_tbl[VUART_CMD_TYPE_MAX] = {
	/* �R�}���h */					/* �����O�X */					/* �֐�  */							/* �����L�� */
	{	0x00,						0,								NULL							},	// �Ȃ�
	{	VUART_CMD_MODE_CHG,			VUART_CMD_LEN_MODE_CHG,			main_vuart_rcv_mode_chg			},	// ��ԕύX(G1D)
	{	VUART_CMD_VERSION,			VUART_CMD_LEN_VERSION,			main_vuart_rcv_version			},	// �o�[�W�����擾
	{	VUART_CMD_DEVICE_INFO,		VUART_CMD_LEN_DEVICE_INFO,		main_vuart_rcv_device_info		},	// �f�o�C�X�󋵎擾
	{	VUART_CMD_DIAG_POWER_OFF,	VUART_CMD_LEN_DIAG_POWER_OFF,	main_vuart_diag_rcv_power_off	},	// �d��OFF
	{	VUART_CMD_DIAG_CHARGE,		VUART_CMD_LEN_DIAG_CHARGE,		main_vuart_diag_rcv_charge		},	// �[�d����
	{	VUART_CMD_DIAG_LED,			VUART_CMD_LEN_DIAG_LED,			main_vuart_diag_rcv_led			},	// LED����
	{	VUART_CMD_DIAG_VIB,			VUART_CMD_LEN_DIAG_VIB,			main_vuart_diag_rcv_vib			},	// �o�C�u���[�V��������
	{	VUART_CMD_DIAG_MIC,			VUART_CMD_LEN_DIAG_MIC,			main_vuart_diag_rcv_mic			},	// �}�C�N����
	{	VUART_CMD_DIAG_ACL,			VUART_CMD_LEN_DIAG_ACL,			main_vuart_diag_rcv_acl			},	// �����x�Z���T�[����
	{	VUART_CMD_DIAG_PHOTO,		VUART_CMD_LEN_DIAG_PHOTO,		main_vuart_diag_rcv_photo		},	// �����Z���T�[����
	{	VUART_CMD_DIAG_EEPROM,		VUART_CMD_LEN_DIAG_EEPROM,		main_vuart_diag_rcv_eep			},	// EEPROM����
};

/* ���[�h�ʏ��� */
typedef void (*MODE_FUNC)(void);
STATIC MODE_FUNC p_user_main_mode_func[] = {								user_main_mode_inital,			// SYSTEM_MODE_INITIAL			�C�j�V����
																			user_main_mode_idle_com,		// SYSTEM_MODE_IDLE_COM			�A�C�h��_�ʐM�ҋ@
																			user_main_mode_sensing,			// SYSTEM_MODE_SENSING			�Z���V���O
																			user_main_mode_get,				// SYSTEM_MODE_GET				�f�[�^�擾
																			user_main_mode_prg_g1d,			// SYSTEM_MODE_PRG_G1D			G1D�v���O�����X�V
																			user_main_mode_self_check,		// SYSTEM_MODE_PRG_SELF_CHECK	���Ȑf�f
																			user_main_mode_move,			// SYSTEM_MODE_MOVE				�ڍs
																			dummy,							// SYSTEM_MODE_NON				�Ȃ�
};

typedef SYSTEM_MODE ( *EVENT_TABLE )( int event );
STATIC EVENT_TABLE p_event_table[ EVENT_MAX ][ SYSTEM_MODE_MAX ] = {
// ���[�h				INITIAL				IDLE_COM			SENSING			GET					PRG_G1D			SELF_CHECK		MOVE				NON
/*�C�x���g�Ȃ�		*/	{ evt_non,			evt_non,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�d��SW(�Z)		*/	{ evt_non,			evt_non,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�d��SW����(��)	*/	{ evt_idle_com,		evt_sensing_chg,	evt_idle_com,	evt_sensing_chg,	evt_non,		evt_non,		evt_non,			evt_non },
/*�[�d���m�|�[�gON	*/	{ evt_idle_com,		evt_non,			evt_idle_com,	evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�[�d���m�|�[�gOFF	*/	{ evt_non,			evt_non,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�d�r�c�ʒቺ		*/	{ evt_non,			evt_non,			evt_idle_com,	evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�[�d����			*/	{ evt_non,			evt_non,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�f�[�^�擾		*/	{ evt_non,			evt_get,			evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�v���O����(G1D)	*/	{ evt_non,			evt_g1d_prg_denchi,	evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*���Ȑf�f(�ʐM)	*/	{ evt_non,			evt_self_check,		evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*����				*/	{ evt_non,			evt_non,			evt_idle_com,	evt_idle_com,		evt_idle_com,	evt_idle_com,	evt_non,			evt_non },
/*���f				*/	{ evt_non,			evt_non,			evt_idle_com,	evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*�^�C���A�E�g		*/	{ evt_non,			evt_non,			evt_non,		evt_time_out,		evt_non,		evt_non,		evt_non,			evt_non },
/*����				*/	{ evt_idle_com_denchi,evt_self_check,	evt_non,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
/*���O����ѱ��	*/	{ evt_non,			evt_non,			evt_remove,		evt_non,			evt_non,		evt_non,		evt_non,			evt_non },
};

// 1�y�[�W256byte�̂O���߃e�[�u��
STATIC const UB s_eep_page0_tbl[EEP_ACCESS_ONCE_SIZE] = { 0 };

