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
#include "movave_param.h"
#include "../calc_data.h"

/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
void calc_apnea(const double* pData, int DSize, int Param1, double Param2, double Param3, double Param4);
void edgeWeighted_MovAve(const UH* data_apnea, double* movave_, UW datasize);
void heartBeat_Remov(double* dc_, const UH *data_apnea, double* movave_, const UH* data_snore, UW datasize);
extern int	peak_modify_a(const double in_data[] , H in_res[] , double ot_data[] , double ot_hz[] , int size , double delta, double th);
extern void peak_vallay_a(const double in[] , H ot[] , int size, int width , int peak );

/************************************************************/
/* 定数定義													*/
/************************************************************/

/************************************************************/
/* 変数定義													*/
/************************************************************/
static int		apnea_ = APNEA_NONE;	// 呼吸状態

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
	double* data_apnea_temp;
	double* movave_;
	int ii;
	
	//データサイズ制限
	datasize = DATA_SIZE_APNEA;
	
	// 演算用データ移行
	data_apnea_temp = &temp_dbl_buf0[0];	//calloc
	movave_ = &temp_dbl_buf2[0];	
	
	for(ii = 0; ii < datasize; ++ii){
		data_apnea_temp[ii] = data_apnea[ii];
	}
	
	// エッジ強調移動平均
	edgeWeighted_MovAve(data_apnea, movave_, datasize);
	
	// 心拍除去
	heartBeat_Remov(data_apnea_temp, data_apnea, movave_, data_snore, datasize);
	
	// 除去後の呼吸音の移動平均
	for (ii = 0; ii < datasize; ++ii) {
		movave_[ii] = 0;
		if (ii >= APNEA_PARAM_AVE_NUM_HALF && ii < (datasize - APNEA_PARAM_AVE_NUM_HALF))
		{
			movave_[ii] += data_apnea_temp[ii - 2];
			movave_[ii] += data_apnea_temp[ii - 1];
			movave_[ii] += data_apnea_temp[ii];
			movave_[ii] += data_apnea_temp[ii + 1];
			movave_[ii] += data_apnea_temp[ii + 2];
			movave_[ii] /= (double)APNEA_PARAM_AVE_NUM;
			movave_[ii] /= APNEA_PARAM_RAW;	// 生データ補正
		}
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
void edgeWeighted_MovAve(const UH* data_apnea, double* movave_, UW datasize)
{
	int ii;
	
	// エッジ強調移動平均
	for (ii = 0; ii < datasize; ++ii) {
		movave_[ii] = 0;

		if (ii >= APNEA_PARAM_AVE_NUM_HALF && ii < (datasize - APNEA_PARAM_AVE_NUM_HALF))
		{
			movave_[ii] += data_apnea[ii - 2] * DIAMETER_END;
			movave_[ii] += data_apnea[ii - 1] * DIAMETER_NEXT;
			movave_[ii] += data_apnea[ii] * DIAMETER_CENTER;
			movave_[ii] += data_apnea[ii + 1] * DIAMETER_NEXT;
			movave_[ii] += data_apnea[ii + 2] * DIAMETER_END;
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
void heartBeat_Remov(double* dc_, const UH *data_apnea, double* movave_, const UH* data_snore, UW datasize)
{
	boolean before_under = FALSE;
	boolean after_under = FALSE;
	int i, j;
	
	// 心拍除去
	for (i = 0; i < datasize; i++)
	{
		if (i >= PREVIOUS_DATA_NUM && i < (datasize - PREVIOUS_DATA_NUM))
		{
			if (movave_[i] >= MAX_EDGE_THRESHOLD && data_snore[i] <= MIN_SNORE_THRESHOLD)
			{
				for (j = 1; j <= PREVIOUS_DATA_NUM; j++)
				{
					if (data_apnea[i - j] < MIN_BREATH_THRESHOLD)
					{
						before_under = TRUE;
					}
					if (data_apnea[i + j] < MIN_BREATH_THRESHOLD)
					{
						after_under = TRUE;
					}
				}
				
				if (before_under && after_under)
				{
					dc_[i] = 0;
					for (j = 1; j <= PREVIOUS_DATA_NUM; j++)
					{
						dc_[i - j] = 0;
						dc_[i + j] = 0;
					}
				}
				before_under = FALSE;
				after_under = FALSE;
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

/*==============================================================================*/
/* EOF */
