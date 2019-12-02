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
B			vib_orbit_value = 0;							// バイブ周回値
B			set_vib_orbit_value = VIB_THREE_ORBIT_ONE_SET;	// バイブ周回設定値
B			vib_gradually_stronger_level = 0;				// 徐々に強く設定時のバイブレベル

VIB_MODE	vib_mode = VIB_MODE_INITIAL;
VIB_MODE	vib_last_mode = VIB_MODE_INITIAL;

// プロトタイプ宣言
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
	if(vib_orbit_value < set_vib_orbit_value)
	{
		switch (vib_mode)
		{
			case VIB_MODE_ON:	// ON(単純動作)
				vib_on();
				break;
			case VIB_MODE_OFF:	// OFF(単純動作)
				vib_off();
				break;
			case VIB_MODE_WEAK:	// 弱
				vib_mode_weak(vib_timer);
				break;
			case VIB_MODE_DURING: // 中
				vib_mode_during(vib_timer);
				break;
			case VIB_MODE_DURING_THREE: // 中 × 3
				vib_mode_during_three(vib_timer);
				break;
			case VIB_MODE_STRENGTH: // 強
				vib_mode_strength(vib_timer);
				break;
			case VIB_MODE_STRENGTH_THREE: // 強 × 3
				vib_mode_strength_three(vib_timer);
				break;
			case VIB_MODE_GRADUALLY_STRONGER_THREE: // 徐々に強く× 3
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
	vib_orbit_value = VIB_STOP_ORBIT;
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
	set_vib_orbit(mode);
	
	// 周回リセット
	vib_orbit_value = 0;
	
	// パターン
	vib_mode = mode;
	
	set_vib_flg( true );
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
	VIB_MODE mode = VIB_MODE_DURING;
	
	switch(yokusei_str)
	{
		case VIB_SET_MODE_WEAK: //中
			mode = VIB_MODE_DURING;
			break;
		case VIB_SET_MODE_DURING: //中 × 3
			mode = VIB_MODE_DURING_THREE;
			break;
		case VIB_SET_MODE_STRENGTH: //強 × 3
			mode = VIB_MODE_STRENGTH_THREE;
			break;
		case VIB_SET_MODE_GRADUALLY_STRONGER: //徐々に強く × 3
			mode = VIB_MODE_GRADUALLY_STRONGER_THREE;
			break;
		default:
			break;
	}
	return mode;
}

/************************************************************************/
/* 関数     : set_vib_orbit												*/
/* 関数名   : バイブレーションの周回設定								*/
/* 引数     : mode:バイブパターン										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.11.12 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
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
STATIC void vib_on(void)
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
STATIC void vib_off(void)
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
/* 関数     : vib_mode_during_three										*/
/* 関数名   : バイブレーション中×３制御								*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_mode_during_three(UH vib_timer)
{
	if(	20 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_mode_strength_three									*/
/* 関数名   : バイブレーション強×３制御								*/
/* 引数     : vib_timer:バイブタイマー									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2019.07.24 oneA 葛原 弘安	初版作成						*/
/************************************************************************/
/* 機能 : 																*/
/************************************************************************/
/* 注意事項 : なし														*/
/************************************************************************/
STATIC void vib_mode_strength_three(UH vib_timer)
{
	if(	30 <= vib_timer )
	{
//		VIB_CTL = 0; // 保険
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
/* 関数     : vib_mode_gradually_stronger								*/
/* 関数名   : バイブレーション徐々に強く×３制御						*/
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
/* 関数     : set_vib_level												*/
/* 関数名   : バイブレーション徐々に強く×３のレベル設定				*/
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
		vib_orbit_value = set_vib_orbit_value;
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
		vib_orbit_value = set_vib_orbit_value;
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
