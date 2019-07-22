/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : led.c													*/
/* �@�\         : 							*/
/* �ύX����		: 	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

#include	"header.h"				//���[�U�[��`
#include	"led.h"

// �O���[�o���ϐ�
LED_PATT pattern = LED_PATT_MAX;
UH		 led_orbit_timer = 0;

// �v���g�^�C�v�錾
STATIC void led_green_lighting(UH led_timer);
STATIC void led_green_blink(UH led_timer);
STATIC void led_yellow_lighting(UH led_timer);
STATIC void led_yellow_blink(UH led_timer);

/************************************************************************/
/* �֐�     : led1_lighting													*/
/* �֐���   : 										*/
/* ����     : 									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 			*/
/************************************************************************/
/* �@�\ : 											*/
/************************************************************************/
/* ���ӎ��� : �Ȃ�														*/
/************************************************************************/
void led_start(UH led_timer)
{
	switch(pattern)
	{
		case LED_PATT_GREEN_LIGHTING:
			led_green_lighting(led_timer);
			break;
		case LED_PATT_GREEN_BLINK:
			led_green_blink(led_timer);
			break;
		case LED_PATT_YELLOW_LIGHTING:
			led_yellow_lighting(led_timer);
			break;
		case LED_PATT_YELLOW_BLINK:
			led_yellow_blink(led_timer);
			break;
		default:
			break;
	}
}


void set_led(LED_PATT patt)
{
	// �^�C�}�[���Z�b�g
	
	pattern = patt;
}

void led_green_on(void)
{
	LED_GREEN = LED_ON;
}

void led_green_off(void)
{
	LED_GREEN = LED_OFF;
}

STATIC void led_green_lighting(UH led_timer)
{
	if(led_timer < PERIOD_10SEC)
	{
		if(read1_sfr( LED_PORT, LED_GREEN_BIT) != LED_ON)
		{
			LED_GREEN = LED_ON;
		}
	}else {
		if(read1_sfr( LED_PORT, LED_GREEN_BIT) != LED_OFF)
		{
			LED_GREEN = LED_OFF;
		}
	}
}
 
STATIC void led_green_blink(UH led_timer)
{
	if(led_timer < PERIOD_10SEC)
	{
		if( PERIOD_1SEC <= (led_timer - led_orbit_timer) )
		{
			if(read1_sfr( LED_PORT, LED_GREEN_BIT) != LED_OFF)
			{
				LED_GREEN = LED_OFF;
			} else
			{
				LED_GREEN = LED_ON;
			}
			led_orbit_timer = led_timer;
		}
	}else{
		if(read1_sfr( LED_PORT, LED_GREEN_BIT) != LED_OFF)
		{
			LED_GREEN = LED_OFF;
		}		
	}
}

void led_yellow_on(void)
{
	LED_YELLOW = LED_ON;
}

void led_yellow_off(void)
{
	LED_YELLOW = LED_OFF;
}

STATIC void led_yellow_lighting(UH led_timer)
{
	if(led_timer < PERIOD_10SEC)
	{
		if(read1_sfr( LED_PORT, LED_YELLOW_BIT) != LED_ON)
		{
			LED_YELLOW = LED_ON;
		}
	}else {
		if(read1_sfr( LED_PORT, LED_YELLOW_BIT) != LED_OFF)
		{
			LED_YELLOW = LED_OFF;
		}
	}
}
 
STATIC void led_yellow_blink(UH led_timer)
{
	 
}
 