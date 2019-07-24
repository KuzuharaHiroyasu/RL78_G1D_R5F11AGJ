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
#define LED_GREEN	P1_bit.no6
#define	LED_YELLOW	P1_bit.no5

#define LED_PORT		P1
#define LED_GREEN_BIT	6
#define	LED_YELLOW_BIT	5

#define LED_ORBIT	10	// LED周回

// LED状態
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
	LED_PATT_ON,				// LED緑、黄点灯
	LED_PATT_OFF,				// LED緑、黄消灯
	LED_PATT_GREEN_ON,			// LED緑点灯
	LED_PATT_GREEN_OFF,			// LED緑消灯
	LED_PATT_GREEN_LIGHTING,	// LED緑点灯(タイマー)
	LED_PATT_GREEN_BLINK,		// LED緑点滅(タイマー)
	LED_PATT_YELLOW_ON,			// LED黄点灯
	LED_PATT_YELLOW_OFF,		// LED黄消灯
	LED_PATT_YELLOW_LIGHTING,	// LED黄点灯(タイマー)
	LED_PATT_YELLOW_BLINK,		// LED黄点滅(タイマー)
	LED_PATT_MAX
}LED_PATT;


extern void led_start(UH led_timer);
extern void set_led(LED_PATT patt);

#endif // LED_H_
