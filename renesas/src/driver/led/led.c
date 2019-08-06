/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : led.c															*/
/* 機能         : LED制御														*/
/* 変更履歴     : 2019.07.24 oneA 葛原 弘安	初版作成							*/
/* 注意事項     : なし															*/
/********************************************************************************/

#include	"header.h"				//ユーザー定義
#include	"led.h"

// グローバル変数
LED_PATT pattern = LED_PATT_INITIAL;
UH		 led_orbit_timer = 0;

// プロトタイプ宣言
STATIC void led_on(void);
STATIC void led_off(void);
STATIC void led_green_on(void);
STATIC void led_green_off(void);
STATIC void led_green_lighting(UW led_timer);
STATIC void led_green_blink(UW led_timer);
STATIC void led_yellow_on(void);
STATIC void led_yellow_off(void);
STATIC void led_yellow_lighting(UW led_timer);
STATIC void led_yellow_blink(UW led_timer);

/************************************************************************/
/* 関数     : led_start													*/
/* 関数名   : LED制御開始												*/
/* 引数     : led_timer:LEDタイマー										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 											*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void led_start(UW led_timer)
{
	switch(pattern)
	{
		case LED_PATT_ON:
			led_on();
			break;
		case LED_PATT_OFF:
			led_off();
			break;
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
		case LED_PATT_YELLOW_ON:
			led_yellow_on();
			break;
		case LED_PATT_YELLOW_OFF:
			led_yellow_off();
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
	
	if(LED_TIMER_10SEC < led_timer && pattern != LED_PATT_INITIAL && pattern != LED_PATT_YELLOW_ON)
	{// 10秒超えたらパターンを初期値に戻してLEDをOFFにしておく
		pattern = LED_PATT_INITIAL;
		led_off();
	}
}

/************************************************************************/
/* 関数     : set_led													*/
/* 関数名   : LED制御設定												*/
/* 引数     : patt:LEDパターン											*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void set_led(LED_PATT patt)
{
	// タイマーリセット
	reset_led_timer();
	
	// パターンセット
	pattern = patt;
}

/************************************************************************/
/* 関数     : led_on													*/
/* 関数名   : LED1, 2点灯												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_on(void)
{
	LED_GREEN = LED_ON;
	LED_YELLOW = LED_ON;
}

/************************************************************************/
/* 関数     : led_off													*/
/* 関数名   : LED1, 2消灯												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_off(void)
{
	LED_GREEN = LED_OFF;
	LED_YELLOW = LED_OFF;
}

/************************************************************************/
/* 関数     : led_green_on												*/
/* 関数名   : LED1点灯													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_green_on(void)
{
	LED_GREEN = LED_ON;
}

/************************************************************************/
/* 関数     : led_green_off												*/
/* 関数名   : LED1消灯													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_green_off(void)
{
	LED_GREEN = LED_OFF;
}

/************************************************************************/
/* 関数     : led_green_lighting										*/
/* 関数名   : LED1点灯（タイマー）										*/
/* 引数     : led_timer:LEDタイマー										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_green_lighting(UW led_timer)
{
	if(led_timer < LED_TIMER_10SEC)
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

/************************************************************************/
/* 関数     : led_green_blink											*/
/* 関数名   : LED1点滅（タイマー）										*/
/* 引数     : led_timer:LEDタイマー										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_green_blink(UW led_timer)
{
	if(led_timer < LED_TIMER_10SEC)
	{
		if( LED_TIMER_1SEC <= (led_timer - led_orbit_timer) )
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

/************************************************************************/
/* 関数     : led_yellow_on												*/
/* 関数名   : LED2点灯													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_yellow_on(void)
{
	LED_YELLOW = LED_ON;
}

/************************************************************************/
/* 関数     : led_yellow_off											*/
/* 関数名   : LED2消灯													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_yellow_off(void)
{
	LED_YELLOW = LED_OFF;
}

/************************************************************************/
/* 関数     : led_yellow_lighting										*/
/* 関数名   : LED2点灯（タイマー）										*/
/* 引数     : led_timer:LEDタイマー										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_yellow_lighting(UW led_timer)
{
	if(led_timer < LED_TIMER_10SEC)
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

/************************************************************************/
/* 関数     : led_yellow_blink											*/
/* 関数名   : LED2点滅（タイマー）										*/
/* 引数     : led_timer:LEDタイマー										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void led_yellow_blink(UW led_timer)
{
	if(led_timer < LED_TIMER_10SEC)
	{
		if( LED_TIMER_1SEC <= (led_timer - led_orbit_timer) )
		{
			if(read1_sfr( LED_PORT, LED_YELLOW_BIT) != LED_OFF)
			{
				LED_YELLOW = LED_OFF;
			} else
			{
				LED_YELLOW = LED_ON;
			}
			led_orbit_timer = led_timer;
		}
	}else{
		if(read1_sfr( LED_PORT, LED_YELLOW_BIT) != LED_OFF)
		{
			LED_YELLOW = LED_OFF;
		}		
	} 
}
