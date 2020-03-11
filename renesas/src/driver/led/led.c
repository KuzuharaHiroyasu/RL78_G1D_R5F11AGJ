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
UH		 blink_timer = 0;

// プロトタイプ宣言
STATIC void led_green_lighting(UW led_timer);
STATIC void led_green_blink(UW led_timer);
STATIC void led_green_blink_low_batt(UW led_timer);
STATIC void led_green_blink_sensing(UW led_timer);

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
	{// 10秒超えている かつ 緑色点灯以外が設定されていたら初期値に戻してLEDをOFFにしておく
		pattern = LED_PATT_INITIAL;
		led_green_off();
		reset_bat_checkflg();
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
	
	blink_timer = 0;
	led_orbit_timer = 0;
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
void led_green_on(void)
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
void led_green_off(void)
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
	if(read1_sfr( LED_PORT, LED_GREEN_BIT) == LED_OFF)
	{
		LED_GREEN = LED_ON;
		pattern = LED_PATT_LIGHT_UP;
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
/* 関数     : led_green_blink_low_batt									*/
/* 関数名   : LED1点滅（電池残量なし）									*/
/* 引数     : led_timer:LEDタイマー										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
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
/* 関数     : led_green_blink_sensing									*/
/* 関数名   : LED1点滅（センシング中）									*/
/* 引数     : led_timer:LEDタイマー										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
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
