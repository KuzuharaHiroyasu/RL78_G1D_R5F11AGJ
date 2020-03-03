/********************************************************************************/
/* システム名   : RD8001														*/
/* ファイル名   : vibration.h														*/
/* 機能         : 		*/
/* 変更履歴     : 		初版作成				*/
/* 注意事項     : なし															*/
/********************************************************************************/

#ifndef	__VIBRATION_INC__
#define	__VIBRATION_INC__

#include	"sys.h"

// バイブポート設定
#define VIB_CTL						P0_bit.no1
#define VIB_ENA						P0_bit.no0

// バイブ1セットの回数
#define	VIB_ONE_SET					3					// 1セット3回

// バイブ1セットの繰り返し数
#define VIB_REPEAT_ONE		(VIB_ONE_SET * 1)	// バイブ周回 3 × 1
#define VIB_REPEAT_TWO		(VIB_ONE_SET * 2)	// バイブ周回 3 × 2
#define VIB_REPEAT_THREE	(VIB_ONE_SET * 3)	// バイブ周回 3 × 3
#define VIB_STOP_REPEAT		(VIB_REPEAT_THREE + 1)

// バイブ設定値
#define VIB_SET_MODE_WEAK					0
#define VIB_SET_MODE_DURING					1
#define VIB_SET_MODE_STRENGTH				2
#define VIB_SET_MODE_GRADUALLY_STRONGER		3

// バイブモード
typedef enum{
	VIB_MODE_ON,						// ON(単純動作)
	VIB_MODE_OFF,						// OFF(単純動作)
	VIB_MODE_WEAK,						// 弱
	VIB_MODE_DURING,					// 中
	VIB_MODE_DURING_REPEAT,				// 中(繰り返し)
	VIB_MODE_STRENGTH,					// 強
	VIB_MODE_STRENGTH_REPEAT,			// 強(繰り返し)
	VIB_MODE_GRADUALLY_STRONGER,		// 徐々に強く
	VIB_MODE_STANDBY,					// 待機モード移行時
	VIB_MODE_SENSING,					// センシングモード移行時
	VIB_MODE_INTERVAL,					// バイブのセット間インターバル
	VIB_MODE_INTERVAL_LEVEL,			// バイブのレベル間のインターバル
	VIB_MODE_INITIAL
}VIB_MODE;

// バイブレベル
typedef enum{
	VIB_LEVEL_1 = 1,
	VIB_LEVEL_2,
	VIB_LEVEL_3,
	VIB_LEVEL_4,
	VIB_LEVEL_5,
	VIB_LEVEL_6,
	VIB_LEVEL_7,
	VIB_LEVEL_8,
	VIB_LEVEL_9,
	VIB_LEVEL_10,
	VIB_LEVEL_11,
	VIB_LEVEL_12,
	VIB_LEVEL_MAX
}VIB_LEVEL;

/******************/
/*  外部参照宣言  */
/******************/
extern void vib_start(UH vib_timer);
extern void vib_stop(void);
extern void set_vib(VIB_MODE mode);
extern void set_vib_confirm(VIB_MODE mode);
extern VIB_MODE set_vib_mode(UB yokusei_str);
extern void set_vib_level(B level);
extern void vib_on(void);
extern void vib_off(void);
#endif // __VIBRATION_INC__
