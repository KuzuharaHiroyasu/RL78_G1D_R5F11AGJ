/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : vibration.c													*/
/* �@�\         : �o�C�u���[�V��������											*/
/* �ύX����		: 2019.07.24 oneA ���� �O��	���ō쐬							*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

#include	"header.h"				//���[�U�[��`
#include	"vibration.h"

// �O���[�o���ϐ�
B			vib_repeat_value = 0;							// �o�C�u����l
B			set_vib_repeat_value = VIB_REPEAT_ONE;			// �o�C�u����ݒ�l
B			vib_gradually_stronger_level = VIB_LEVEL_1;		// ���X�ɋ����ݒ莞�̃o�C�u���x��

VIB_MODE	vib_mode = VIB_MODE_INITIAL;
VIB_MODE	vib_last_mode = VIB_MODE_INITIAL;

bool		confirm_flg = false;

// �v���g�^�C�v�錾
STATIC void set_vib_repeat(VIB_MODE mode);
STATIC void vib_mode_weak(UH vib_timer);
STATIC void vib_mode_during(UH vib_timer);
STATIC void vib_mode_strength(UH vib_timer);
STATIC void vib_mode_during_repeat(UH vib_timer);
STATIC void vib_mode_strength_repeat(UH vib_timer);
STATIC void vib_mode_gradually_stronger(UH vib_timer);

STATIC void vib_mode_standby(UH vib_timer);
STATIC void vib_mode_sensing(UH vib_timer);
STATIC void vib_interval(UH vib_timer);
STATIC void vib_interval_level(UH vib_timer);
STATIC void vib_level_up_confirm(void);

STATIC void vib_level_1(UH vib_timer);
STATIC void vib_level_2(UH vib_timer);
STATIC void vib_level_3(UH vib_timer);
STATIC void vib_level_4(UH vib_timer);

/************************************************************************/
/* �֐�     : vib_start													*/
/* �֐���   : �o�C�u���[�V�����J�n										*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void vib_start(UH vib_timer)
{ // ���C�����[�v�������ŃR�[��
	if(vib_repeat_value < set_vib_repeat_value)
	{
		switch (vib_mode)
		{
			case VIB_MODE_ON:	// ON(�P������)
				vib_on();
				break;
			case VIB_MODE_OFF:	// OFF(�P������)
				vib_off();
				break;
			case VIB_MODE_WEAK:	// ��i�����F���łP�Z�b�g�j
				vib_mode_during_repeat(vib_timer);
				break;
			case VIB_MODE_DURING: // ���i�����F���łQ�Z�b�g�j
				vib_mode_during_repeat(vib_timer);
				break;
			case VIB_MODE_DURING_REPEAT: // ���g�p
				vib_mode_during_repeat(vib_timer);
				break;
			case VIB_MODE_STRENGTH: // ���i�����F���łR�Z�b�g�j
				vib_mode_during_repeat(vib_timer);
				break;
			case VIB_MODE_STRENGTH_REPEAT: // ���g�p
				vib_mode_strength_repeat(vib_timer);
				break;
			case VIB_MODE_GRADUALLY_STRONGER: // ���X�ɋ���
				vib_mode_gradually_stronger(vib_timer);
				break;
			case VIB_MODE_STANDBY: // �ҋ@���[�h�ڍs��
				vib_mode_standby(vib_timer);
				break;
			case VIB_MODE_SENSING: // �Z���V���O���[�h�ڍs��
				vib_mode_sensing(vib_timer);
				break;
			case VIB_MODE_INTERVAL: // �o�C�u�̃Z�b�g�ԃC���^�[�o��
				vib_interval(vib_timer);
				break;
			case VIB_MODE_INTERVAL_LEVEL: // �o�C�u�̃��x���Ԃ̃C���^�[�o��
				vib_interval_level(vib_timer);
				break;
			default:
				break;
		}
	}else if(vib_mode != VIB_MODE_INITIAL)
	{
		set_kokyu_val_off(OFF);
		vib_mode = VIB_MODE_INITIAL;
		vib_last_mode = vib_mode;
		confirm_flg = false;
		vib_gradually_stronger_level = VIB_LEVEL_1;
		set_vib_flg( false );
	}
}

/************************************************************************/
/* �֐�     : vib_stop													*/
/* �֐���   : �o�C�u���[�V������~										*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void vib_stop(void)
{
	vib_repeat_value = VIB_STOP_REPEAT;
	vib_off();
}

/************************************************************************/
/* �֐�     : set_vib													*/
/* �֐���   : �o�C�u���[�V�����ݒ�										*/
/* ����     : mode:�o�C�u�p�^�[��										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void set_vib(VIB_MODE mode)
{ //�����������^�C�~���O�ŃR�[��
	// ON
	// 10msec�^�C�}�[���Z�b�g
	reset_vib_timer();

	// ����l�ݒ�
	set_vib_repeat(mode);
	
	// ���񃊃Z�b�g
	vib_repeat_value = 0;
	
	// �p�^�[��
	vib_mode = mode;
	
	set_vib_flg( true );
}

/************************************************************************/
/* �֐�     : set_vib_confirm											*/
/* �֐���   : �o�C�u���[�V�����ݒ�(�m�F�p)								*/
/* ����     : mode:�o�C�u�p�^�[��										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void set_vib_confirm(VIB_MODE mode)
{ //�o�C�u���[�V�����m�F�p
	// ON
	// 10msec�^�C�}�[���Z�b�g
	reset_vib_timer();

	// ����l�ݒ�
	set_vib_repeat(mode);
	
	// ���񃊃Z�b�g
	vib_repeat_value = 0;
	
	// �p�^�[��
	vib_mode = mode;
	
	set_vib_flg( true );
	
	confirm_flg = true;
}

/************************************************************************/
/* �֐�     : set_vib_mode												*/
/* �֐���   : �o�C�u���[�V�����ݒ�ϊ�									*/
/* ����     : yokusei_str:�}�����x										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.08.01 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
VIB_MODE set_vib_mode(UB yokusei_str)
{
	VIB_MODE mode;
	
	switch(yokusei_str)
	{
		case VIB_SET_MODE_WEAK: // ��
			mode = VIB_MODE_WEAK;
			break;
		case VIB_SET_MODE_DURING: //��
			mode = VIB_MODE_DURING;
			break;
		case VIB_SET_MODE_STRENGTH: //��
			mode = VIB_MODE_STRENGTH;
			break;
		case VIB_SET_MODE_GRADUALLY_STRONGER: //���X�ɋ���
			mode = VIB_MODE_GRADUALLY_STRONGER;
			break;
		default:
			mode = VIB_MODE_DURING;
			break;
	}
	return mode;
}

/************************************************************************/
/* �֐�     : set_vib_repeat												*/
/* �֐���   : �o�C�u���[�V�����̎���ݒ�								*/
/* ����     : mode:�o�C�u�p�^�[��										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.11.12 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void set_vib_repeat(VIB_MODE mode)
{
	switch(mode)
	{
		case VIB_MODE_WEAK:
			set_vib_repeat_value = VIB_REPEAT_ONE;
			break;
		case VIB_MODE_DURING:
			set_vib_repeat_value = VIB_REPEAT_TWO;
			break;
		case VIB_MODE_STRENGTH:
			set_vib_repeat_value = VIB_REPEAT_THREE;
			break;
		case VIB_MODE_DURING_REPEAT:
		case VIB_MODE_STRENGTH_REPEAT:
			set_vib_repeat_value = VIB_REPEAT_THREE;
			break;
		case VIB_MODE_GRADUALLY_STRONGER:
			if(vib_gradually_stronger_level < VIB_LEVEL_5)
			{
				// ���x��1�`4��1�Z�b�g
				set_vib_repeat_value = VIB_REPEAT_ONE;
			}else if(vib_gradually_stronger_level < VIB_LEVEL_9)
			{
				// ���x��5�`8��2�Z�b�g
				set_vib_repeat_value = VIB_REPEAT_TWO;
			}else if(VIB_LEVEL_9 <= vib_gradually_stronger_level)
			{
				// ���x��9�`��3�Z�b�g
				set_vib_repeat_value = VIB_REPEAT_THREE;
			}
			break;
		default:
			set_vib_repeat_value = VIB_REPEAT_ONE;
			break;
	}
}

/************************************************************************/
/* �֐�     : vib_on													*/
/* �֐���   : �o�C�u���[�V����ON										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void vib_on(void)
{ 
	// ON
	VIB_CTL = 1;
	VIB_ENA = 1;
}

/************************************************************************/
/* �֐�     : vib_off													*/
/* �֐���   : �o�C�u���[�V����OFF										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void vib_off(void)
{ 
	// OFF
	VIB_CTL = 0;
	VIB_ENA = 0;
}

/************************************************************************/
/* �֐�     : vib_mode_weak												*/
/* �֐���   : �o�C�u���[�V�����㐧��									*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_weak(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		reset_vib_timer();
	} if( 5 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 3 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}
}

/************************************************************************/
/* �֐�     : vib_mode_during											*/
/* �֐���   : �o�C�u���[�V����������									*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_during(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		reset_vib_timer();
	} if( 13 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 11 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

/************************************************************************/
/* �֐�     : vib_mode_strength											*/
/* �֐���   : �o�C�u���[�V����������									*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_strength(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		reset_vib_timer();
	} if( 19 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 17 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

/************************************************************************/
/* �֐�     : vib_mode_during_repeat									*/
/* �֐���   : �o�C�u���[�V�������J��Ԃ�����							*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_during_repeat(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		if(!(vib_repeat_value % VIB_ONE_SET))
		{
			vib_last_mode = vib_mode;
			vib_mode = VIB_MODE_INTERVAL;
		}
		reset_vib_timer();
	} if( 13 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 11 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

/************************************************************************/
/* �֐�     : vib_mode_strength_repeat									*/
/* �֐���   : �o�C�u���[�V�������J��Ԃ�����							*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_strength_repeat(UH vib_timer)
{
	if(	30 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		if(!(vib_repeat_value % VIB_ONE_SET))
		{
			vib_last_mode = vib_mode;
			vib_mode = VIB_MODE_INTERVAL;
		}
		reset_vib_timer();
	} if( 29 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 27 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

/************************************************************************/
/* �֐�     : vib_mode_gradually_stronger								*/
/* �֐���   : �o�C�u���[�V�������X�ɋ�������							*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.11.29 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_gradually_stronger(UH vib_timer)
{
	switch(vib_gradually_stronger_level)
	{
		case VIB_LEVEL_1:
		case VIB_LEVEL_5:
		case VIB_LEVEL_9:
			vib_level_1(vib_timer);
			break;
		case VIB_LEVEL_2:
		case VIB_LEVEL_6:
		case VIB_LEVEL_10:
			vib_level_2(vib_timer);
			break;
		case VIB_LEVEL_3:
		case VIB_LEVEL_7:
		case VIB_LEVEL_11:
			vib_level_3(vib_timer);
			break;
		case VIB_LEVEL_4:
		case VIB_LEVEL_8:
		case VIB_LEVEL_12:
			vib_level_4(vib_timer);
			break;
		default:
			vib_level_4(vib_timer);
			break;
	}
}

/************************************************************************/
/* �֐�     : set_vib_level												*/
/* �֐���   : �o�C�u���[�V�������X�ɋ����̃��x���ݒ�					*/
/* ����     : level:�o�C�u���x��										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.11.30 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void set_vib_level(B vib_level)
{
	vib_gradually_stronger_level = vib_level;
}

/************************************************************************/
/* �֐�     : vib_mode_standby											*/
/* �֐���   : �o�C�u���[�V�����ҋ@���[�h�ڍs������						*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_standby(UH vib_timer)
{
	if(	80 <= vib_timer )
	{
		VIB_CTL = 0;
		VIB_ENA = 0;
		vib_repeat_value = VIB_STOP_REPEAT;
		reset_vib_timer();
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

/************************************************************************/
/* �֐�     : vib_mode_sensing											*/
/* �֐���   : �o�C�u���[�V�����Z���V���O���[�h�ڍs������				*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_sensing(UH vib_timer)
{
	if(	10 <= vib_timer )
	{
		VIB_CTL = 0;
		VIB_ENA = 0;
		vib_repeat_value = VIB_STOP_REPEAT;
		reset_vib_timer();
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}
}

/************************************************************************/
/* �֐�     : vib_interval												*/
/* �֐���   : �o�C�u�̃Z�b�g�ԃC���^�[�o��								*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.11.12 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_interval(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
		vib_mode = vib_last_mode;	// �o�C�u���[�h��߂�
		reset_vib_timer();
	}
}

/************************************************************************/
/* �֐�     : vib_interval_level										*/
/* �֐���   : �o�C�u�̃��x���Ԃ̃C���^�[�o��							*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_interval_level(UH vib_timer)
{
	if(	40 <= vib_timer )
	{
		vib_mode = vib_last_mode;	// �o�C�u���[�h��߂�
		reset_vib_timer();
	}
}

/************************************************************************/
/* �֐�     : vib_level_up_confirm										*/
/* �֐���   : �o�C�u�m�F�̃��x���A�b�v									*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_level_up_confirm(void)
{
	vib_last_mode = vib_mode;
	if(confirm_flg == true && vib_repeat_value >= set_vib_repeat_value && (vib_gradually_stronger_level+1) < VIB_LEVEL_MAX)
	{
		// ���̃��x����
		vib_gradually_stronger_level++;
		vib_repeat_value = 0;
		vib_mode = VIB_MODE_INTERVAL_LEVEL;
	}else{
		// �I��
		vib_mode = VIB_MODE_INTERVAL;
	}
}

/************************************************************************/
/* �֐�     : vib_level_1												*/
/* �֐���   : �o�C�u���[�V�������X�ɋ���_���x���P						*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_level_1(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		if(!(vib_repeat_value % VIB_ONE_SET))
		{
			vib_level_up_confirm();
		}
		reset_vib_timer();
	} if( 13 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 11 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

/************************************************************************/
/* �֐�     : vib_level_2												*/
/* �֐���   : �o�C�u���[�V�������X�ɋ���_���x���Q						*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_level_2(UH vib_timer)
{
	if(	28 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		if(!(vib_repeat_value % VIB_ONE_SET))
		{
			vib_level_up_confirm();
		}
		reset_vib_timer();
	} if( 21 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 19 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

/************************************************************************/
/* �֐�     : vib_level_3												*/
/* �֐���   : �o�C�u���[�V�������X�ɋ���_���x���R						*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_level_3(UH vib_timer)
{
	if(	30 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		if(!(vib_repeat_value % VIB_ONE_SET))
		{
			vib_level_up_confirm();
		}
		reset_vib_timer();
	} if( 29 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 27 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

/************************************************************************/
/* �֐�     : vib_level_4												*/
/* �֐���   : �o�C�u���[�V�������X�ɋ���_���x���S						*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.12.02 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_level_4(UH vib_timer)
{
	if(	38 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_repeat_value += 1;
		if(!(vib_repeat_value % VIB_ONE_SET))
		{
			vib_level_up_confirm();
		}
		reset_vib_timer();
	} if( 37 <= vib_timer )
	{
		VIB_ENA = 0;
	} if( 35 <= vib_timer )
	{
		VIB_CTL = 0;
	} else
	{
		VIB_CTL = 1;
		VIB_ENA = 1;
	}	
}

bool get_confirm_flg(void)
{
	return confirm_flg;
}

bool set_confirm_flg(bool flg)
{
	confirm_flg = flg;
}