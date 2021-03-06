/************************************************************************/
/* システム名   : RD8001快眠チェッカー									*/
/* ファイル名   : calc_snore.h											*/
/* 機能         : いびき判定演算処理									*/
/* 変更履歴     : 2018.07.25 Axia Soft Design mmura	初版作成			*/
/* 注意事項     : なし													*/
/************************************************************************/
#ifndef		_CALC_SNORE_H_		/* 二重定義防止 */
#define		_CALC_SNORE_H_

#include "sys.h"

/************************************************************/
/*					マクロ定数定義							*/
/************************************************************/

/************************************************************/
/* 						型定義								*/
/************************************************************/
typedef enum{
	SNORE_SENS_WEAK = 0,		// 弱
	SNORE_SENS_DURING,			// 中
	SNORE_SENS_STRENGTH,		// 強
	SNORE_SENS_MAX
}SNORE_SENS;

typedef enum{
	CALC_TYPE_SNORE = 0,
	CALC_TYPE_BREATH,
	CALC_TYPE_MAX
}CALC_TYPE;

/************************************************************/
/*					外部参照宣言							*/
/************************************************************/
extern void	calc_snore_init(void);
extern void	calc_proc(const UH *pData, UB calc_type);
extern UB	calc_snore_get(void);
extern void set_snore_sens( UB sens );
extern void Reset(UB calc_type);

extern UB	calc_breath_get(void);

#endif	/*_CALC_SNORE_H_*/
/************************************************************/
/* END OF TEXT												*/
/************************************************************/
