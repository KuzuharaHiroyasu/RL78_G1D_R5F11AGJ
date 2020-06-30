/********************************************************************************/
/* システム名   : RD8001 快眠チェッカー											*/
/* ファイル名   : vibration.c													*/
/* 機能         : バイブレーション制御											*/
/* 変更履歴		: 2019.07.24 oneA 葛原 弘安	初版作成							*/
/* 注意事項     : なし															*/
/********************************************************************************/

#include	"header.h"				//ユーザー定義
#include	"vibration.h"

// グローバル変数
B			vib_repeat_value = 0;							// バイブ周回値
B			set_vib_repeat_value = VIB_REPEAT_ONE;			// バイブ周回設定値
B			vib_gradually_stronger_level = VIB_LEVEL_1;		// 徐々に強く設定時のバイブレベル

VIB_MODE	vib_mode = VIB_MODE_INITIAL;
VIB_MODE	vib_last_mode = VIB_MODE_INITIAL;

bool		confirm_flg = false;

// プロトタイプ宣言
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
/* 関数     : vib_start													*/
/* 関数名   : バイブレーション開始										*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void vib_start(UH vib_timer)
{ // メインループ処理内でコール
	if(vib_repeat_value < set_vib_repeat_value)
	{
		switch (vib_mode)
		{
			case VIB_MODE_ON:	// ON(単純動作)
				vib_on();
				break;
			case VIB_MODE_OFF:	// OFF(単純動作)
				vib_off();
				break;
			case VIB_MODE_WEAK:	// 弱（強さ：中で１セット）
				vib_mode_during_repeat(vib_timer);
				break;
			case VIB_MODE_DURING: // 中（強さ：中で２セット）
				vib_mode_during_repeat(vib_timer);
				break;
			case VIB_MODE_DURING_REPEAT: // 未使用
				vib_mode_during_repeat(vib_timer);
				break;
			case VIB_MODE_STRENGTH: // 強（強さ：中で３セット）
				vib_mode_during_repeat(vib_timer);
				break;
			case VIB_MODE_STRENGTH_REPEAT: // 未使用
				vib_mode_strength_repeat(vib_timer);
				break;
			case VIB_MODE_GRADUALLY_STRONGER: // 徐々に強く
				vib_mode_gradually_stronger(vib_timer);
				break;
			case VIB_MODE_STANDBY: // 待機モード移行時
				vib_mode_standby(vib_timer);
				break;
			case VIB_MODE_SENSING: // センシングモード移行時
				vib_mode_sensing(vib_timer);
				break;
			case VIB_MODE_INTERVAL: // バイブのセット間インターバル
				vib_interval(vib_timer);
				break;
			case VIB_MODE_INTERVAL_LEVEL: // バイブのレベル間のインターバル
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
/* 関数     : vib_stop													*/
/* 関数名   : バイブレーション停止										*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void vib_stop(void)
{
	vib_repeat_value = VIB_STOP_REPEAT;
	vib_off();
}

/************************************************************************/
/* 関数     : set_vib													*/
/* 関数名   : バイブレーション設定										*/
/* 引数     : mode:バイブパターン										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void set_vib(VIB_MODE mode)
{ //動かしたいタイミングでコール
	// ON
	// 10msecタイマーリセット
	reset_vib_timer();

	// 周回値設定
	set_vib_repeat(mode);
	
	// 周回リセット
	vib_repeat_value = 0;
	
	// パターン
	vib_mode = mode;
	
	set_vib_flg( true );
}

/************************************************************************/
/* 関数     : set_vib_confirm											*/
/* 関数名   : バイブレーション設定(確認用)								*/
/* 引数     : mode:バイブパターン										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void set_vib_confirm(VIB_MODE mode)
{ //バイブレーション確認用
	// ON
	// 10msecタイマーリセット
	reset_vib_timer();

	// 周回値設定
	set_vib_repeat(mode);
	
	// 周回リセット
	vib_repeat_value = 0;
	
	// パターン
	vib_mode = mode;
	
	set_vib_flg( true );
	
	confirm_flg = true;
}

/************************************************************************/
/* 関数     : set_vib_mode												*/
/* 関数名   : バイブレーション設定変換									*/
/* 引数     : yokusei_str:抑制強度										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.08.01 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
VIB_MODE set_vib_mode(UB yokusei_str)
{
	VIB_MODE mode;
	
	switch(yokusei_str)
	{
		case VIB_SET_MODE_WEAK: // 弱
			mode = VIB_MODE_WEAK;
			break;
		case VIB_SET_MODE_DURING: //中
			mode = VIB_MODE_DURING;
			break;
		case VIB_SET_MODE_STRENGTH: //強
			mode = VIB_MODE_STRENGTH;
			break;
		case VIB_SET_MODE_GRADUALLY_STRONGER: //徐々に強く
			mode = VIB_MODE_GRADUALLY_STRONGER;
			break;
		default:
			mode = VIB_MODE_DURING;
			break;
	}
	return mode;
}

/************************************************************************/
/* 関数     : set_vib_repeat												*/
/* 関数名   : バイブレーションの周回設定								*/
/* 引数     : mode:バイブパターン										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.11.12 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
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
				// レベル1～4は1セット
				set_vib_repeat_value = VIB_REPEAT_ONE;
			}else if(vib_gradually_stronger_level < VIB_LEVEL_9)
			{
				// レベル5～8は2セット
				set_vib_repeat_value = VIB_REPEAT_TWO;
			}else if(VIB_LEVEL_9 <= vib_gradually_stronger_level)
			{
				// レベル9～は3セット
				set_vib_repeat_value = VIB_REPEAT_THREE;
			}
			break;
		default:
			set_vib_repeat_value = VIB_REPEAT_ONE;
			break;
	}
}

/************************************************************************/
/* 関数     : vib_on													*/
/* 関数名   : バイブレーションON										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void vib_on(void)
{ 
	// ON
	VIB_CTL = 1;
	VIB_ENA = 1;
}

/************************************************************************/
/* 関数     : vib_off													*/
/* 関数名   : バイブレーションOFF										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void vib_off(void)
{ 
	// OFF
	VIB_CTL = 0;
	VIB_ENA = 0;
}

/************************************************************************/
/* 関数     : vib_mode_weak												*/
/* 関数名   : バイブレーション弱制御									*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_mode_weak(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_mode_during											*/
/* 関数名   : バイブレーション中制御									*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_mode_during(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_mode_strength											*/
/* 関数名   : バイブレーション強制御									*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_mode_strength(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_mode_during_repeat									*/
/* 関数名   : バイブレーション中繰り返し制御							*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_mode_during_repeat(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_mode_strength_repeat									*/
/* 関数名   : バイブレーション強繰り返し制御							*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_mode_strength_repeat(UH vib_timer)
{
	if(	30 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_mode_gradually_stronger								*/
/* 関数名   : バイブレーション徐々に強く制御							*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.11.29 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
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
/* 関数     : set_vib_level												*/
/* 関数名   : バイブレーション徐々に強くのレベル設定					*/
/* 引数     : level:バイブレベル										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.11.30 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
void set_vib_level(B vib_level)
{
	vib_gradually_stronger_level = vib_level;
}

/************************************************************************/
/* 関数     : vib_mode_standby											*/
/* 関数名   : バイブレーション待機モード移行時制御						*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
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
/* 関数     : vib_mode_sensing											*/
/* 関数名   : バイブレーションセンシングモード移行時制御				*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
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
/* 関数     : vib_interval												*/
/* 関数名   : バイブのセット間インターバル								*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.11.12 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_interval(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
		vib_mode = vib_last_mode;	// バイブモードを戻す
		reset_vib_timer();
	}
}

/************************************************************************/
/* 関数     : vib_interval_level										*/
/* 関数名   : バイブのレベル間のインターバル							*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_interval_level(UH vib_timer)
{
	if(	40 <= vib_timer )
	{
		vib_mode = vib_last_mode;	// バイブモードを戻す
		reset_vib_timer();
	}
}

/************************************************************************/
/* 関数     : vib_level_up_confirm										*/
/* 関数名   : バイブ確認のレベルアップ									*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_level_up_confirm(void)
{
	vib_last_mode = vib_mode;
	if(confirm_flg == true && vib_repeat_value >= set_vib_repeat_value && (vib_gradually_stronger_level+1) < VIB_LEVEL_MAX)
	{
		// 次のレベルへ
		vib_gradually_stronger_level++;
		vib_repeat_value = 0;
		vib_mode = VIB_MODE_INTERVAL_LEVEL;
	}else{
		// 終了
		vib_mode = VIB_MODE_INTERVAL;
	}
}

/************************************************************************/
/* 関数     : vib_level_1												*/
/* 関数名   : バイブレーション徐々に強く_レベル１						*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_level_1(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_level_2												*/
/* 関数名   : バイブレーション徐々に強く_レベル２						*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_level_2(UH vib_timer)
{
	if(	28 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_level_3												*/
/* 関数名   : バイブレーション徐々に強く_レベル３						*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_level_3(UH vib_timer)
{
	if(	30 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_level_4												*/
/* 関数名   : バイブレーション徐々に強く_レベル４						*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.12.02 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_level_4(UH vib_timer)
{
	if(	38 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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