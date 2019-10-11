/************************************************************************/
/* システム名   : RD8001快眠チェッカー									*/
/* ファイル名   : calc_data.h											*/
/* 機能         : 演算共通データ										*/
/* 変更履歴     : 2018.01.15 Axia Soft Design 和田 初版作成				*/
/* 注意事項     : なし													*/
/************************************************************************/
#ifndef		_CALC_DATA_H_		/* 二重定義防止 */
#define		_CALC_DATA_H_

#include "sys.h"

/************************************************************/
/*					マクロ定数定義							*/
/************************************************************/
//#define DATA_SIZE_SPO2	(128)
#define DATA_SIZE		(200)
#define DATA_SIZE_PRMS	(10)

// いびき判定結果
#define SNORE_OFF		0	// いびきなし
#define SNORE_ON		1	// いびきあり

/************************************************************/
/* 						型定義								*/
/************************************************************/


/************************************************************/
/*					外部参照宣言							*/
/************************************************************/
extern void calc_data_init(void);

extern	double	data_apnea_temp_[DATA_SIZE];
extern	double	prms_temp[DATA_SIZE_PRMS];
extern	double	data_movave_temp_[DATA_SIZE];
extern	double	ppoint_temp[DATA_SIZE];

extern	H		thresholds_over_num[DATA_SIZE];
//extern	H		temp_int_buf1[DATA_SIZE];
//extern	H		temp_int_buf2[DATA_SIZE];

extern UB cont_apnea_point;

#endif	/*_CALC_DATA_H_*/
/************************************************************/
/* END OF TEXT												*/
/************************************************************/
