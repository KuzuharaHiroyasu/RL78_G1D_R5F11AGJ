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
B			vib_orbit_value = 0;							// �o�C�u����l
B			set_vib_orbit_value = VIB_THREE_ORBIT_ONE_SET;	// �o�C�u����ݒ�l
B			vib_gradually_stronger_level = 0;				// ���X�ɋ����ݒ莞�̃o�C�u���x��

VIB_MODE	vib_mode = VIB_MODE_INITIAL;
VIB_MODE	vib_last_mode = VIB_MODE_INITIAL;

// �v���g�^�C�v�錾
STATIC void set_vib_orbit(VIB_MODE mode);
STATIC void vib_on(void);
STATIC void vib_off(void);
STATIC void vib_mode_weak(UH vib_timer);
STATIC void vib_mode_during(UH vib_timer);
STATIC void vib_mode_strength(UH vib_timer);
STATIC void vib_mode_during_three(UH vib_timer);
STATIC void vib_mode_strength_three(UH vib_timer);
STATIC void vib_mode_gradually_stronger(UH vib_timer);

STATIC void vib_mode_standby(UH vib_timer);
STATIC void vib_mode_sensing(UH vib_timer);
STATIC void vib_interval(UH vib_timer);

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
	if(vib_orbit_value < set_vib_orbit_value)
	{
		switch (vib_mode)
		{
			case VIB_MODE_ON:	// ON(�P������)
				vib_on();
				break;
			case VIB_MODE_OFF:	// OFF(�P������)
				vib_off();
				break;
			case VIB_MODE_WEAK:	// ��
				vib_mode_weak(vib_timer);
				break;
			case VIB_MODE_DURING: // ��
				vib_mode_during(vib_timer);
				break;
			case VIB_MODE_DURING_THREE: // �� �~ 3
				vib_mode_during_three(vib_timer);
				break;
			case VIB_MODE_STRENGTH: // ��
				vib_mode_strength(vib_timer);
				break;
			case VIB_MODE_STRENGTH_THREE: // �� �~ 3
				vib_mode_strength_three(vib_timer);
				break;
			case VIB_MODE_GRADUALLY_STRONGER_THREE: // ���X�ɋ����~ 3
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
			default:
				break;
		}
	}else if(vib_mode != VIB_MODE_INITIAL)
	{
		vib_mode = VIB_MODE_INITIAL;
		vib_last_mode = VIB_MODE_INITIAL;
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
	vib_orbit_value = VIB_STOP_ORBIT;
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
	set_vib_orbit(mode);
	
	// ���񃊃Z�b�g
	vib_orbit_value = 0;
	
	// �p�^�[��
	vib_mode = mode;
	
	set_vib_flg( true );
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
	VIB_MODE mode = VIB_MODE_DURING;
	
	switch(yokusei_str)
	{
		case VIB_SET_MODE_WEAK: //��
			mode = VIB_MODE_DURING;
			break;
		case VIB_SET_MODE_DURING: //�� �~ 3
			mode = VIB_MODE_DURING_THREE;
			break;
		case VIB_SET_MODE_STRENGTH: //�� �~ 3
			mode = VIB_MODE_STRENGTH_THREE;
			break;
		case VIB_SET_MODE_GRADUALLY_STRONGER: //���X�ɋ��� �~ 3
			mode = VIB_MODE_GRADUALLY_STRONGER_THREE;
			break;
		default:
			break;
	}
	return mode;
}

/************************************************************************/
/* �֐�     : set_vib_orbit												*/
/* �֐���   : �o�C�u���[�V�����̎���ݒ�								*/
/* ����     : mode:�o�C�u�p�^�[��										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.11.12 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void set_vib_orbit(VIB_MODE mode)
{
	switch(mode)
	{
		case VIB_MODE_DURING_THREE:
		case VIB_MODE_STRENGTH_THREE:
		case VIB_SET_MODE_GRADUALLY_STRONGER:
			set_vib_orbit_value = VIB_THREE_ORBIT_THREE_SET;
			break;
		default:
			set_vib_orbit_value = VIB_THREE_ORBIT_ONE_SET;
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
STATIC void vib_on(void)
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
STATIC void vib_off(void)
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
		vib_orbit_value += 1;
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
		vib_orbit_value += 1;
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
		vib_orbit_value += 1;
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
/* �֐�     : vib_mode_during_three										*/
/* �֐���   : �o�C�u���[�V�������~�R����								*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_during_three(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_orbit_value += 1;
		if(!(vib_orbit_value % VIB_ONE_SET))
		{
			vib_last_mode = VIB_MODE_DURING_THREE;
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
/* �֐�     : vib_mode_strength_three									*/
/* �֐���   : �o�C�u���[�V�������~�R����								*/
/* ����     : vib_timer:�o�C�u�^�C�}�[									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void vib_mode_strength_three(UH vib_timer)
{
	if(	30 <= vib_timer )
	{
//		VIB_CTL = 0; // �ی�
//		VIB_ENA = 0;
		vib_orbit_value += 1;
		if(!(vib_orbit_value % VIB_ONE_SET))
		{
			vib_last_mode = VIB_MODE_STRENGTH_THREE;
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
/* �֐���   : �o�C�u���[�V�������X�ɋ����~�R����						*/
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
			vib_level_1(vib_timer);
			break;
		case VIB_LEVEL_2:
			vib_level_2(vib_timer);
			break;
		case VIB_LEVEL_3:
			vib_level_3(vib_timer);
			break;
		case VIB_LEVEL_4:
			vib_level_4(vib_timer);
			break;
		default:
			break;
	}
}

/************************************************************************/
/* �֐�     : set_vib_level												*/
/* �֐���   : �o�C�u���[�V�������X�ɋ����~�R�̃��x���ݒ�				*/
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
		vib_orbit_value = set_vib_orbit_value;
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
		vib_orbit_value = set_vib_orbit_value;
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
		vib_orbit_value += 1;
		if(!(vib_orbit_value % VIB_ONE_SET))
		{
			vib_last_mode = VIB_MODE_DURING_THREE;
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
		vib_orbit_value += 1;
		if(!(vib_orbit_value % VIB_ONE_SET))
		{
			vib_last_mode = VIB_MODE_DURING_THREE;
			vib_mode = VIB_MODE_INTERVAL;
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
		vib_orbit_value += 1;
		if(!(vib_orbit_value % VIB_ONE_SET))
		{
			vib_last_mode = VIB_MODE_STRENGTH_THREE;
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
		vib_orbit_value += 1;
		if(!(vib_orbit_value % VIB_ONE_SET))
		{
			vib_last_mode = VIB_MODE_STRENGTH_THREE;
			vib_mode = VIB_MODE_INTERVAL;
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
