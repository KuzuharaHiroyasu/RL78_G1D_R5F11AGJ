/************************************************************************/
/* システム名   : 無呼吸判定											*/
/* ファイル名   : apnea_param.h											*/
/* 機能         : 無呼吸演算パラメータ									*/
/* 変更履歴     : 2018.05.16 Axia Soft Design mmura	初版作成			*/
/* 注意事項     : なし                                                  */
/************************************************************************/

#ifndef		_APNEA_PARAM_H_			/* 二重定義防止 */
#define		_APNEA_PARAM_H_

/************************************************************/
/* マクロ													*/
/************************************************************/
// 生データ補正
#define APNEA_PARAM_RAW			20000.0f

// 移動平均データ数
#define APNEA_PARAM_AVE_NUM				5
#define APNEA_PARAM_AVE_NUM_HALF		(APNEA_PARAM_AVE_NUM / 2)

// ピーク間隔50%閾値
#define APNEA_PARAM_PEAK_THRE			87.85f

// 移動平均に用いるデータの数 0.05 * 90 = 4.5s分のデータ
#define APNEA_PARAM_AVE_CNT				90
// 移動平均50%閾値
#define APNEA_PARAM_AVE_THRE			0.015f
// 2値化50%閾値
#define APNEA_PARAM_BIN_THRE			20

// いびき閾値
#define APNEA_PARAM_SNORE				0.0125f

// 完全無呼吸判定閾値
#define APNEA_PARAM_APNEA_THRE			(35 / APNEA_PARAM_RAW)


// 新いびき演算向け ---------------------------------------
// 判定向け観測回数
#define SNORE_PARAM_SIZE				(10)
// ON判定回数
#define SNORE_PARAM_ON_CNT				(8)
// OFF判定回数
#define SNORE_PARAM_OFF_CNT				(0)
// いびき閾値[感度：弱]
#define SNORE_PARAM_THRE_WERK			(450)
// いびき閾値[感度：中]
#define SNORE_PARAM_THRE_DURING			(350)
// いびき閾値[感度：強]
#define SNORE_PARAM_THRE_STRENGTH		(250)
// いびきあり -> なしへの判定回数
#define SNORE_PARAM_NORMAL_CNT			(80)
// 許容誤差 0.4s
#define SNORE_PARAM_GOSA				(8)

#endif

/************************************************************/
/* END OF TEXT												*/
/************************************************************/

