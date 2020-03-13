/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : led.c															*/
/* 機能         : LED制御														*/
/* 変更履歴     : 2019.07.24 oneA 葛原 弘安	初版作成							*/
/* 注意事項     : なし															*/
/********************************************************************************/
#ifndef LED_H_
#define LED_H_

#include	"sys.h"

// LEDポート
#define LED_GREEN	P1_bit.no5

#define LED_PORT		P1
#define LED_GREEN_BIT	5

#define LED_ORBIT	3	// LED周回

// LED状態
#define LED_ON           1
#define LED_OFF          0

// timer
#define LED_TIMER_10SEC		1000	// 10msec * 1000 = 10000msec = 10sec
#define LED_TIMER_5SEC		 500	// 10msec * 500  = 5000msec  = 5sec
#define LED_TIMER_2SEC		 200	// 10msec * 200  = 2000msec  = 2sec
#define LED_TIMER_1SEC		 100	// 10msec * 100  = 1000msec  = 1sec
#define LED_TIMER_200MSEC	  20	// 10msec * 20   = 200msec   = 200msec
#define LED_TIMER_100MSEC	  10	// 10msec * 10   = 100msec   = 100msec
#define LED_TIMER_20MSEC	   2	// 10msec * 2    = 20msec    = 20msec


typedef enum{
	LED_PATT_GREEN_ON,				// LED緑点灯
	LED_PATT_GREEN_OFF,				// LED緑消灯
	LED_PATT_GREEN_LIGHTING,		// LED緑点灯(タイマー)
	LED_PATT_GREEN_BLINK,			// LED緑点滅(タイマー)
	LED_PATT_GREEN_BLINK_LOW_BATT,	// LED緑点滅(電池残量なし)
	LED_PATT_GREEN_BLINK_SENSING,	// LED緑点滅(センシング中)
	LED_PATT_LIGHT_UP,				// LED点灯中
	LED_PATT_INITIAL
}LED_PATT;


extern void led_start(UW led_timer);
extern void set_led(LED_PATT patt);
extern void led_green_on(void);
extern void led_green_off(void);
#endif // LED_H_
