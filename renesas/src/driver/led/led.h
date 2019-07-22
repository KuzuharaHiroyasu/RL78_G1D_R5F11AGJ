/********************************************************************************/
/* �V�X�e����   : RD8001 �����`�F�b�J�[											*/
/* �t�@�C����   : led.h													*/
/* �@�\         : 							*/
/* �ύX����		: 	���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/
#ifndef LED_H_
#define LED_H_

#include	"sys.h"

// LED�|�[�g
#define LED_GREEN	P1_bit.no6
#define	LED_YELLOW	P1_bit.no5

#define LED_PORT		P1
#define LED_GREEN_BIT	6
#define	LED_YELLOW_BIT	5


// LED���
#define LED_ON           1
#define LED_OFF          0

///10ms timer
#define PERIOD_10SEC     1000U
#define PERIOD_1SEC      100U
#define PERIOD_100MSEC   10U

struct led_env_tag
{
    ///Timer elapsed flag
    uint8_t  timer_flag;
    ///LEd blink interval
    uint16_t tick_10ms;
};

typedef enum{
	LED_PATT_GREEN_LIGHTING,	// LED�Γ_��
	LED_PATT_GREEN_BLINK,		// LED�Γ_��
	LED_PATT_YELLOW_LIGHTING,	// LED���_��
	LED_PATT_YELLOW_BLINK,		// LED���_��
	LED_PATT_MAX
}LED_PATT;


extern void led_start(UH led_timer);
extern void set_led(LED_PATT patt);

#endif // LED_H_
