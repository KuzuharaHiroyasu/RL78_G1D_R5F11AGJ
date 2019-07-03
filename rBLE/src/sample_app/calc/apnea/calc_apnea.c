/************************************************************************/
/* システム名   : RD8001快眠チェッカー									*/
/* ファイル名   : calc_apnea.c											*/
/* 機能         : 無呼吸判定演算処理									*/
/* 変更履歴     : 2018.01.11 Axia Soft Design 和田 初版作成				*/
/* 注意事項     : なし													*/
/************************************************************************/
#include <math.h>
#include "calc_apnea.h"
#include "apnea_param.h"
#include "../calc_data.h"

/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
void calc_apnea(const double* pData, int DSize, int Param1, double Param2, double Param3, double Param4);
void edgeWeighted_MovAve(const double* dc_, double* movave_, UW datasize);
void heartBeat_Remov(double* dc_, double* movave_, const UH* data_snore, UW datasize);
void init_before_raw(void);
extern int	peak_modify_a(const double in_data[] , H in_res[] , double ot_data[] , double ot_hz[] , int size , double delta, double th);
extern void peak_vallay_a(const double in[] , H ot[] , int size, int width , int peak );

/************************************************************/
/* 定数定義													*/
/************************************************************/

/************************************************************/
/* 変数定義													*/
/************************************************************/
static int		apnea_ = APNEA_NONE;	// 呼吸状態
double  before_raw[PREVIOUS_DATA_NUM];

/************************************************************************/
/* 関数     : calculator_apnea											*/
/* 関数名   : 無呼吸判定演算											*/
/* 引数     : int *data : 波形データ(呼吸音)									*/
/*          : int len   : 波形データサイズ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void calculator_apnea(const UH *data_apnea, const UH *data_snore)
{
	UW datasize;						// 波形データのバイト数
	double* ptest1;
	double* movave_;
	int ii, jj;
	
	//データサイズ制限
	datasize = DATA_SIZE_APNEA;
	
	// 演算用データ移行
	ptest1 = &temp_dbl_buf0[0];	//calloc
	movave_ = &temp_dbl_buf2[0];	
	
	for(ii=0; ii < datasize; ++ii){
		ptest1[ii] = data_apnea[ii];
	}
	
	// エッジ強調移動平均
	edgeWeighted_MovAve(ptest1, movave_, datasize);
	
	// 心拍除去
	heartBeat_Remov(ptest1, movave_, data_snore, datasize);
	
	// 現在のデータの末尾を保管（要素197, 198, 199）
	before_raw[0] = data_apnea[197];
	before_raw[1] = data_apnea[198];
	before_raw[2] = data_apnea[199];
	
	for(ii=0;ii<datasize;++ii){
		movave_[ii]=0;
		for(jj=0;jj<APNEA_PARAM_AVE_NUM;++jj){
			if((ii-jj)>=0){
				movave_[ii]+=ptest1[ii-jj];
			}
		}
		movave_[ii] /= (double)APNEA_PARAM_AVE_NUM;
		movave_[ii] /= APNEA_PARAM_RAW;
	}
	
	// (35) - (47)
	calc_apnea(movave_, datasize, APNEA_PARAM_AVE_CNT, APNEA_PARAM_AVE_THRE, APNEA_PARAM_BIN_THRE, APNEA_PARAM_APNEA_THRE);
}

/************************************************************************/
/* 関数     : calc_apnea												*/
/* 関数名   : 無呼吸演算処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void calc_apnea(const double* pData, int DSize, int Param1, double Param2, double Param3, double Param4)
{
	double* prms;
	double* ppoint;
	int datasize;
	int ii;
	int jj;
	int loop=0;
	int apnea=0;
	
	// (41) ... 使用していないため省略
	// (42)
	// (43) = prms
	datasize = DSize / 20;
	prms = &temp_dbl_buf1[0];
	for(ii=0;ii<datasize;++ii){
		double tmp = 0.0f;
		prms[ii] = 0.0f;
		for(jj=0;jj<20;++jj){
			tmp += (pData[ii*20 + jj]*pData[ii*20 + jj]);
		}
		tmp /= 20;
		prms[ii] = sqrt(tmp);
	}
	
	// (44) = ppoint
	// (45) = prms
	ppoint = &temp_dbl_buf0[0];
	for(ii=0;ii<datasize;++ii){
		if(prms[ii] >= Param3){
			ppoint[ii] = 1;
		}else{
			ppoint[ii] = 0;
		}
	}
	
	// (46)
	if(datasize == 0){
		apnea_ = APNEA_NORMAL;
	}
	else if(datasize > 9){
		apnea_ = APNEA_ERROR;
		loop = datasize - 9;
		for(ii = 0; ii < loop; ++ii){
			apnea = 0;
			for(jj = 0; jj < datasize; ++jj){
				apnea += ppoint[ii + jj];
				if(APNEA_JUDGE_CNT < apnea){
					// 通常呼吸と判断した時点で抜ける
					apnea_ = APNEA_NORMAL;
					break;
				}
			}
		}
	}else{
		apnea_ = APNEA_NORMAL;
	}
	
/* //低呼吸も無呼吸と判定するため
	// 完全無呼吸の判定
	if(apnea_ == APNEA_WARN){
		apnea_ = APNEA_ERROR;
		for(ii=0;ii<datasize;++ii){
			if(pData[ii] > Param4){
				apnea_ = APNEA_WARN;
				break;
			}
		}
	}
*/
}

/************************************************************************/
/* 関数     : edgeWeighted_MovAve										*/
/* 関数名   : エッジ強調移動平均										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 															*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void edgeWeighted_MovAve(const double* dc_, double* movave_, UW datasize)
{
	int ii;
	
	// エッジ強調移動平均
	for (ii = 0; ii < datasize; ++ii) {
		movave_[ii] = 0;

		if ((ii - 2) >= 0)
		{//要素2以上
			if ((ii + 2) <= (datasize - 1))
			{// 要素197まで
				movave_[ii] += dc_[ii - 2] * 0.1;
				movave_[ii] += dc_[ii - 1];
				movave_[ii] += dc_[ii] * 20;
				movave_[ii] += dc_[ii + 1];
				movave_[ii] += dc_[ii + 2] * 0.1;
				movave_[ii] /= (double)APNEA_PARAM_AVE_NUM;
			}
		}
		else if (before_raw[ii] != -1)
		{// 過去データがあるか
			movave_[ii] += before_raw[ii + 1] * 0.1;
			if (ii == 0)
			{
				movave_[ii] += before_raw[ii + 2];
			}
			else {
				movave_[ii] += dc_[ii - 1];
			}
			movave_[ii] += dc_[ii] * 20;
			movave_[ii] += dc_[ii + 1];
			movave_[ii] += dc_[ii + 2] * 0.1;
			movave_[ii] /= (double)APNEA_PARAM_AVE_NUM;
		}
	}
}

/************************************************************************/
/* 関数     : heartBeat_Remov											*/
/* 関数名   : 心拍除去													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 															*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void heartBeat_Remov(double* dc_, double* movave_, const UH* data_snore, UW datasize)
{
	boolean before_under = FALSE;
	boolean after_under = FALSE;
	int i, j;
	int pastDataNum = 0;
	
	// 心拍除去
	for (i = 0; i < datasize; i++)
	{
		if (i <= datasize - 4)
		{
			if (movave_[i] >= MAX_THRESHOLD && data_snore[i] <= MIN_THRESHOLD)
			{// エッジ強調1000以上、いびき音100以下
				if (i < 3)
				{
				// 0 ～ 2まで
					// 前150ms確認
					if (i == 0)
					{// データ配列の先頭
						if (before_raw[0] < MIN_THRESHOLD || before_raw[1] < MIN_THRESHOLD || before_raw[2] < MIN_THRESHOLD)
						{
							before_under = TRUE;
							pastDataNum = 3;
						}
					}
					else if (i == 1)
					{// データ配列2番目
						if (dc_[0] < MIN_THRESHOLD || before_raw[1] < MIN_THRESHOLD || before_raw[2] < MIN_THRESHOLD)
						{
							before_under = TRUE;
							pastDataNum = 2;
						}
					}
					else if (i == 2)
					{// データ配列3番目
						if (dc_[0] < MIN_THRESHOLD || dc_[1] < MIN_THRESHOLD || before_raw[2] < MIN_THRESHOLD)
						{
							before_under = TRUE;
							pastDataNum = 1;
						}
					}
					// 前に該当データがある場合は後150ms確認
					if (before_under)
					{
						for (j = 1; j < 4; j++)
						{
							if (dc_[i + j] < MIN_THRESHOLD)
							{
								after_under = TRUE;
							}
						}
					}
				}
				else {
				// 3 ～ 196まで
					// 前後150ms確認
					for (j = 1; j < 4; j++)
					{
						// 前150ms確認
						if (dc_[i - j] < MIN_THRESHOLD)
						{
							before_under = TRUE;
						}
						// 後150ms確認
						if (dc_[i + j] < MIN_THRESHOLD)
						{
							after_under = TRUE;
						}
					}
				}

				if (before_under && after_under)
				{// 前後に該当箇所がある場合
					for (j = -3 + pastDataNum; j < 4; j++)
					{// 前後150ms分0にする
						dc_[i + j] = 0;
					}
				}
				// フラグリセット
				before_under = FALSE;
				after_under = FALSE;
				pastDataNum = 0;
			}
		}
	}
}

/************************************************************************/
/* 関数     : get_state													*/
/* 関数名   : 状態取得													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 															*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
UB get_state(void)
{
	UB ret = 0;
	ret |= ((apnea_ << 6) & 0xC0);
	
	return ret;
}

/************************************************************************/
/* 関数     : init_before_raw													*/
/* 関数名   : 前のデータの初期化													*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 															*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void init_before_raw(void)
{ // 測定開始時に呼び出す！(場所未定)
	int i;
	for(i = 0; i < PREVIOUS_DATA_NUM; i++)
	{
		before_raw[i] = -1;
	}
}

/*==============================================================================*/
/* EOF */
