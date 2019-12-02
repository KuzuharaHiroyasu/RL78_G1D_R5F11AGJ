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


#define VIB_CTL						P0_bit.no1
#define VIB_ENA						P0_bit.no0

#define	VIB_ONE_SET					3					// 1セット3回
#define VIB_THREE_ORBIT_ONE_SET		(VIB_ONE_SET * 1)	// バイブ周回 3 × 1
#define VIB_THREE_ORBIT_THREE_SET	(VIB_ONE_SET * 3)	// バイブ周回 3 × 3

#define VIB_SET_MODE_WEAK					0
#define VIB_SET_MODE_DURING					1
#define VIB_SET_MODE_STRENGTH				2
#define VIB_SET_MODE_GRADUALLY_STRONGER		3

typedef enum{
	VIB_MODE_ON,						// ON(単純動作)
	VIB_MODE_OFF,						// OFF(単純動作)
	VIB_MODE_WEAK,						// 弱
	VIB_MODE_DURING,					// 中
	VIB_MODE_DURING_THREE,				// 中 × 3
	VIB_MODE_STRENGTH,					// 強
	VIB_MODE_STRENGTH_THREE,			// 強 × 3
	VIB_MODE_GRADUALLY_STRONGER_THREE,	// 徐々に強く× 3
	VIB_MODE_STANDBY,					// 待機モード移行時
	VIB_MODE_SENSING,					// センシングモード移行時
	VIB_MODE_INTERVAL,					// バイブのセット間インターバル
	VIB_MODE_INITIAL
}VIB_MODE;

typedef enum{
	VIB_LEVEL_1 = 0,
	VIB_LEVEL_2,
	VIB_LEVEL_3,
	VIB_LEVEL_4,
	VIB_LEVEL_MAX
}VIB_LEVEL;

/******************/
/*  外部参照宣言  */
/******************/
extern void vib_start(UH vib_timer);
extern void set_vib(VIB_MODE mode);
extern VIB_MODE set_vib_mode(UB yokusei_str);
extern void set_vib_level(B level);

#endif // __VIBRATION_INC__
