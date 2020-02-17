/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : led.c															*/
/* �@�\         : LED����														*/
/* �ύX����     : 2019.07.24 oneA ���� �O��	���ō쐬							*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

#include	"header.h"				//���[�U�[��`
#include	"led.h"

// �O���[�o���ϐ�
LED_PATT pattern = LED_PATT_INITIAL;
UH		 led_orbit_timer = 0;
UH		 blink_timer = 0;

// �v���g�^�C�v�錾
STATIC void led_green_lighting(UW led_timer);
STATIC void led_green_blink(UW led_timer);
STATIC void led_green_blink_low_batt(UW led_timer);
STATIC void led_green_blink_sensing(UW led_timer);

/************************************************************************/
/* �֐�     : led_start													*/
/* �֐���   : LED����J�n												*/
/* ����     : led_timer:LED�^�C�}�[										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void led_start(UW led_timer)
{
	if( led_timer < LED_TIMER_10SEC )
	{
		switch(pattern)
		{
			case LED_PATT_GREEN_ON:
				led_green_on();
				break;
			case LED_PATT_GREEN_OFF:
				led_green_off();
				break;
			case LED_PATT_GREEN_LIGHTING:
				led_green_lighting(led_timer);
				break;
			case LED_PATT_GREEN_BLINK:
				led_green_blink(led_timer);
				break;
			case LED_PATT_GREEN_BLINK_LOW_BATT:
				led_green_blink_low_batt(led_timer);
				break;
			case LED_PATT_GREEN_BLINK_SENSING:
				led_green_blink_sensing(led_timer);
				break;
			default:
				break;
		}
	} else if(pattern != LED_PATT_INITIAL && pattern != LED_PATT_GREEN_ON)
	{// 10�b�����Ă��� ���� �ΐF�_���ȊO���ݒ肳��Ă����珉���l�ɖ߂���LED��OFF�ɂ��Ă���
		pattern = LED_PATT_INITIAL;
		led_green_off();
		reset_bat_checkflg();
	}
}

/************************************************************************/
/* �֐�     : set_led													*/
/* �֐���   : LED����ݒ�												*/
/* ����     : patt:LED�p�^�[��											*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void set_led(LED_PATT patt)
{
	// �^�C�}�[���Z�b�g
	reset_led_timer();
	
	// �p�^�[���Z�b�g
	pattern = patt;
	
	blink_timer = 0;
	led_orbit_timer = 0;
}

/************************************************************************/
/* �֐�     : led_green_on												*/
/* �֐���   : LED1�_��													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void led_green_on(void)
{
	LED_GREEN = LED_ON;
}

/************************************************************************/
/* �֐�     : led_green_off												*/
/* �֐���   : LED1����													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void led_green_off(void)
{
	LED_GREEN = LED_OFF;
}

/************************************************************************/
/* �֐�     : led_green_lighting										*/
/* �֐���   : LED1�_���i�^�C�}�[�j										*/
/* ����     : led_timer:LED�^�C�}�[										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void led_green_lighting(UW led_timer)
{
	if(read1_sfr( LED_PORT, LED_GREEN_BIT) == LED_OFF)
	{
		LED_GREEN = LED_ON;
		pattern = LED_PATT_LIGHT_UP;
	}
}

/************************************************************************/
/* �֐�     : led_green_blink											*/
/* �֐���   : LED1�_�Łi�^�C�}�[�j										*/
/* ����     : led_timer:LED�^�C�}�[										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void led_green_blink(UW led_timer)
{
	if( blink_timer <= LED_TIMER_1SEC )
	{
		if(read1_sfr( LED_PORT, LED_GREEN_BIT) == LED_OFF)
		{
			LED_GREEN = LED_ON;
		}
	} else if( blink_timer <= LED_TIMER_2SEC ){
		if(read1_sfr( LED_PORT, LED_GREEN_BIT) == LED_ON)
		{
			LED_GREEN = LED_OFF;
		}
	}
	blink_timer++;
	
	if( LED_TIMER_2SEC < blink_timer )
	{
		blink_timer = 0;
	}
}

/************************************************************************/
/* �֐�     : led_green_blink_low_batt									*/
/* �֐���   : LED1�_�Łi�d�r�c�ʂȂ��j									*/
/* ����     : led_timer:LED�^�C�}�[										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void led_green_blink_low_batt(UW led_timer)
{
	if( led_orbit_timer < LED_ORBIT )
	{
		if( blink_timer <= LED_TIMER_100MSEC )
		{
			if(read1_sfr( LED_PORT, LED_GREEN_BIT) == LED_OFF)
			{
				LED_GREEN = LED_ON;
			}
		} else if( blink_timer <= LED_TIMER_200MSEC ){
			if(read1_sfr( LED_PORT, LED_GREEN_BIT) == LED_ON)
			{
				LED_GREEN = LED_OFF;
			}
		}
		blink_timer++;
		
		if( LED_TIMER_200MSEC < blink_timer )
		{
			blink_timer = 0;
			led_orbit_timer++;
		}
	}
}

/************************************************************************/
/* �֐�     : led_green_blink_sensing									*/
/* �֐���   : LED1�_�Łi�Z���V���O���j									*/
/* ����     : led_timer:LED�^�C�}�[										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2019.07.24 oneA ���� �O��	���ō쐬						*/
/************************************************************************/
/* �@�\ : 																*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
STATIC void led_green_blink_sensing(UW led_timer)
{
	if( blink_timer <= LED_TIMER_100MSEC )
	{
		if(read1_sfr( LED_PORT, LED_GREEN_BIT) == LED_OFF)
		{
			LED_GREEN = LED_ON;
		}
	} else {
		if(read1_sfr( LED_PORT, LED_GREEN_BIT) == LED_ON)
		{
			LED_GREEN = LED_OFF;
			pattern = LED_PATT_LIGHT_UP;
		}
	}
	blink_timer++;
}
