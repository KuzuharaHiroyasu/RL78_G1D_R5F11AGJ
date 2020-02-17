/********************************************************************************/
/* VXeΌ   : RD8001 υ°`FbJ[											*/
/* t@CΌ   : led.c															*/
/* @\         : LED§δ														*/
/* ΟXπ     : 2019.07.24 oneA ΄ Oΐ	Εμ¬							*/
/* Σ     : Θ΅															*/
/********************************************************************************/
#ifndef LED_H_
#define LED_H_

#include	"sys.h"

// LED|[g
#define LED_GREEN	P1_bit.no5

#define LED_PORT		P1
#define LED_GREEN_BIT	5

#define LED_ORBIT	3	// LEDόρ

// LEDσΤ
#define LED_ON           1
#define LED_OFF          0

// timer
#define LED_TIMER_10SEC		100	// 100msec * 100 = 10000msec = 10sec
#define LED_TIMER_2SEC		 20	// 100msec * 20  = 2000msec  = 2sec
#define LED_TIMER_1SEC		 10	// 100msec * 10  = 1000msec  = 1sec
#define LED_TIMER_200MSEC	  2	// 100msec * 2   = 200msec   = 200msec
#define LED_TIMER_100MSEC	  1	// 100msec * 1   = 100msec   = 100msec
//#define LED_TIMER_20MSEC	  1	// 100msec * 1   = 20msec    = 20msec


typedef enum{
	LED_PATT_GREEN_ON,				// LEDΞ_
	LED_PATT_GREEN_OFF,				// LEDΞΑ
	LED_PATT_GREEN_LIGHTING,		// LEDΞ_(^C}[)
	LED_PATT_GREEN_BLINK,			// LEDΞ_Ε(^C}[)
	LED_PATT_GREEN_BLINK_LOW_BATT,	// LEDΞ_Ε(drcΚΘ΅)
	LED_PATT_GREEN_BLINK_SENSING,	// LEDΞ_Ε(ZVO)
	LED_PATT_LIGHT_UP,				// LED_
	LED_PATT_INITIAL
}LED_PATT;


extern void led_start(UW led_timer);
extern void set_led(LED_PATT patt);
extern void led_green_on(void);
extern void led_green_off(void);
#endif // LED_H_
