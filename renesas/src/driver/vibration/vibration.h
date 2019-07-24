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


#define VIB_CTL		P0_bit.no1
#define VIB_ENA		P0_bit.no0
#define VIB_ORBIT	3	// バイブ周回


typedef enum{
	VIB_MODE_ON,		// ON(単純動作)
	VIB_MODE_OFF,		// OFF(単純動作)
	VIB_MODE_WEAK,		// 弱
	VIB_MODE_DURING,	// 中
	VIB_MODE_STRENGTH,	// 強
	VIB_MODE_STANDBY,	// 待機モード移行時
	VIB_MODE_SENSING,	// センシングモード移行時
	VIB_MODE_MAX
}VIB_MODE;

/******************/
/*  外部参照宣言  */
/******************/
extern void vib_start(UH vib_timer);
extern void set_vib(VIB_MODE mode);

#endif // __VIBRATION_INC__
