/************************************************************************/
/* システム名   : RD8001快眠チェッカー									*/
/* ファイル名   : calc_apnea.c											*/
/* 機能         : 無呼吸判定演算処理									*/
/* 変更履歴     : 2018.01.11 Axia Soft Design 和田 初版作成				*/
/* 注意事項     : なし													*/
/************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "calc_apnea.h"
#include "../calc_data.h"

// 演算を軽量化するための定義
#define LIGHT_PROC


/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
#ifndef LIGHT_PROC
void calc_pp(const double* pData, int DSize, double Param1);
#endif
void calc_apnea(const double* pData, int DSize, int Param1, double Param2, double Param3);
void calc_snore(const double* pData, int DSize, double Param);
extern int	peak_modify_a(const double in_data[] , H in_res[] , double ot_data[] , double ot_hz[] , int size , double delta, double th);
extern void peak_vallay_a(const double in[] , H ot[] , int size, int width , int peak );

/************************************************************/
/* 定数定義													*/
/************************************************************/

/************************************************************/
/* 変数定義													*/
/************************************************************/
#ifndef LIGHT_PROC
double	result_peak[BUF_SIZE];			// 結果ピーク間隔
int		result_peak_size;
#endif
static int		apnea_ = APNEA_NONE;	// 呼吸状態
static int		snore_ = SNORE_OFF;		// いびき

/************************************************************************/
/* 関数     : calculator_apnea											*/
/* 関数名   : 無呼吸判定演算											*/
/* 引数     : int *data : 波形データ									*/
/*          : int len   : 波形データサイズ								*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void calculator_apnea(const UH *data)
{
	UW datasize;						// 波形データのバイト数
	double* ptest1;
	int ii;
	
	//データサイズ制限
	datasize = DATA_SIZE_APNEA;
	ptest1 = &temp_dbl_buf0[0];							//calloc
	memset(temp_dbl_buf0,0,datasize*sizeof(double));
	for(ii=0;ii<datasize;++ii){
		ptest1[ii] = (double)data[ii];
	}

	for (ii = 0; ii<datasize; ++ii) {
		//ptest1[ii] = (ptest1[ii] - 512) * 64;	// 10bitカウント値を0中心にし、16bit幅に拡張
		ptest1[ii] = ptest1[ii] * 16;	// 16bit幅に拡張
	}

	for (ii = 0; ii<datasize; ++ii) {
		ptest1[ii] = (double)ptest1[ii] * 0.0000305;
	}

	// (21) - (34)
#ifndef LIGHT_PROC
	calc_pp(ptest1, datasize, 0.003f);
#endif
	
	// (35) - (47)
	calc_apnea(ptest1, datasize, 450, 0.0015f, 0.002f);
	
	// (48) - (56)
	calc_snore(ptest1, datasize, 0.0125f);
	
	// (57)
	
}

#ifndef LIGHT_PROC
/************************************************************************/
/* 関数     : calc_pp													*/
/* 関数名   : ピーク間隔演算処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void calc_pp(const double* pData, int DSize, double Param1)
{
	int16_t* ppeak1;
	double* ppdata1;
	double* pf1;
	int peakcnt1;
	
	int16_t* ppeak2;
	double* ppdata2;
	double* pf2;
	int peakcnt2;
	
	double* ppdata3;
	int peakpos;
	double max;
	//double pos;
	
	int peakcnt;
	double* ppos;
	double* pinterval;
	int ppcnt;
	double* pp2;
	double* pp5;
	
	int ii;
	int jj;
	
	// (22) = Param1
	
	// (23)
	// (24) = peak1
	ppeak1 = &temp_int_buf0[0];	
	ppdata1 = &temp_dbl_buf1[0];
	pf1 = &temp_dbl_buf2[0];	
	peak_vallay_a(pData, ppeak1, DSize, 3, 1);
	peakcnt1 = peak_modify_a(pData, ppeak1, ppdata1, pf1, DSize, 1.0, 0.0);
	
	// (25) = peakcnt2
	// (26) = f2
	// (27) = peak2
	ppeak2 = &temp_int_buf0[0];	
	ppdata2 = &temp_dbl_buf0[0];
	pf2 = &temp_dbl_buf3[0];	
	peak_vallay_a(ppdata1, ppeak2, peakcnt1, 3, 1);
	peakcnt2 = peak_modify_a(ppdata1, ppeak2, ppdata2, pf2, peakcnt1, 1.0, Param1);
	
	ppdata3 = &temp_dbl_buf0[0];
	peakpos = 0;
	for(ii=0;ii<peakcnt2;++ii){
		peakpos = (int)(pf2[ii] + 0.5);
		ppdata3[ii] = pf1[peakpos] * 0.01f;
	}
	
	// (28) - (30)
	max = pData[0];
	//pos = 0;
	for(ii=1;ii<DSize;++ii){
		if(max < pData[ii]){
			max = pData[ii];
			//pos = ii;
		}
	}
	
	peakcnt = peakcnt2;
	ppos = ppdata3;
	pinterval = &temp_dbl_buf1[0];
	pinterval[0] = ppos[0];
	for(ii=0;ii<peakcnt;++ii){
		pinterval[ii+1] = ppos[ii+1] - ppos[ii];
	}
	
	// (31) - (33)
	ppcnt = 0;
	pp2 = &temp_dbl_buf2[0];
	pp5 = &temp_dbl_buf3[0];
	for(ii=1;ii<peakcnt;++ii){
		if(pinterval[ii] >= 10){
			pp2[ppcnt] = ppos[ii-1];
			pp5[ppcnt] = ppos[ii];
			ppcnt += 1;
		}
	}
	// (34)
	if(ppcnt >= 1){
		int size = (int)(pp5[ppcnt-1] + 0.5);
		int rpos = 0;
		double* presult = &temp_dbl_buf0[0];				//calloc
		for(ii=0;ii<ppcnt;++ii){
			int loop2;
			int loop5;
			loop2 = (int)(pp2[ii] + 0.5) -1;
			for(jj=rpos;jj<loop2;++jj){
				presult[rpos] = 0;
				rpos+=1;
			}
			loop5 = (int)(pp5[ii] + 0.5) - 1;
			for(jj=rpos;jj<loop5;++jj){
				presult[rpos] = max;
				rpos+=1;
			}
		}
		memcpy(result_peak, presult, size * sizeof(double));
		result_peak_size = size;
	}else{
		result_peak[0] = 0x00;
		result_peak_size = 0;
	}
}
#endif

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
void calc_apnea(const double* pData, int DSize, int Param1, double Param2, double Param3)
{
	double* pave;
	double* peval2;
	int datasize;
	double* prms;
	double* ppoint;
	double* papnea;
	double ave = 0.0f;
	int ii;
	int jj;
	int min=0;
	int loop=0;
	
	// (35) = Param1
	// (36) = Param2
	// (40) = Param3
	
	// (37)
	pave = &temp_dbl_buf1[0];
	for(ii=0;ii<DSize;++ii){
		min=0;
		loop=0;
		ave = 0.0f;
		if(ii <= DSize-1){
			int tmp = DSize-1 - ii;
			if(tmp > Param1){
				min = Param1;
			}else{
				min = tmp;
			}
			loop = min * 2 + 1;
			min = ii - min;
			if(min < 0){
				min = 0;
			}
		}else{
			min = 0;
			loop = ii * 2 + 1;
		}
		for(jj=0;jj<loop;++jj){
			ave += pData[min+jj];
		}
		ave /= loop;
		pave[ii] = ave;
	}
	
	// (38) - (39)
	peval2 = &temp_dbl_buf2[0];
	for(ii=0;ii<DSize;++ii){
		if(pave[ii] >= Param2){
			peval2[ii] = 1;
		}else{
			peval2[ii] = 0;
		}
	}
	
	// (41) ... 使用していないため省略
	// (42)
	// (43) = prms
	datasize = DSize / 100;
	prms = &temp_dbl_buf1[0];
	for(ii=0;ii<datasize;++ii){
		double tmp = 0.0f;
		prms[ii] = 0.0f;
		for(jj=0;jj<100;++jj){
			tmp += (pData[ii*100 + jj]*pData[ii*100 + jj]);
		}
		tmp /= 100;
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
	papnea = &temp_dbl_buf1[0];
	if(datasize == 0){
		apnea_ = APNEA_ERROR;
	}
	else if(datasize > 9){
		apnea_ = APNEA_NORMAL;
		loop = datasize - 9;
		for(ii=0;ii<9;++ii){
			papnea[ii] = 0.0f;
		}
		for(ii=0;ii<loop;++ii){
			ave = 0.0f;
			for(jj=0;jj<9;++jj){
				ave += ppoint[ii + jj];
			}
			ave /= 9;
			if(ave == 0.0f){
				papnea[ii+9] = 1.0f;
				apnea_ = APNEA_WARN;
			}else{
				papnea[ii+9] = 0.0f;
			}
		}
	}else{
		for(ii=0;ii<datasize;++ii){
			papnea[ii] = 0.0f;
			apnea_ = APNEA_NONE;
		}
	}
}

/************************************************************************/
/* 関数     : calc_snore												*/
/* 関数名   : いびき演算処理											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void calc_snore(const double* pData, int DSize, double Param)
{
	double* pSnore;
	double* pbase_x_y2;
	double* pbase_x_y3;
	
	int cnt;
	int active;
	double* p_x_y2;
	int pos;
	double* p_x_y3;
	
	double* ptime;
	
	int intervalsize;
	double* pinterval;
	
	int intervalsize2;
	double* pinterval2;
	
	double snorecnt;
	
	int ii;
	// (48) = Param
	
	// 結果[いびき]を入れる箱
	pSnore = &temp_dbl_buf1[0];								//calloc
	pbase_x_y2 = &temp_dbl_buf2[0];							//calloc
	pbase_x_y3 = &temp_dbl_buf3[0];							//calloc
	
	// (49)
	for(ii=0;ii<DSize;++ii){
		if(pData[ii] < Param){
			pSnore[ii] = pData[ii];
			pbase_x_y2[ii] = 1;
			pbase_x_y3[ii] = -1;
		}else{
			pSnore[ii] = 0;
			pbase_x_y2[ii] = 0;
			pbase_x_y3[ii] = 0;
		}
	}
	
	// (50)
	for(ii=0;ii<DSize;++ii){
		if(pData[ii] > Param){
			pSnore[ii] *= (1-0.94);
		}else{
			pSnore[ii] *= (0-0.94);
		}
	}
	
	// (51)
	cnt=0;
	active = FALSE;
	for(ii=0;ii<DSize;++ii){
		if((pbase_x_y2[ii] >= 1) && (active == FALSE)){
			active = TRUE;
			cnt += 1;
		}else if(pbase_x_y2[ii] < 1){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	p_x_y2 = &temp_dbl_buf0[0];								//calloc
	pos = 0;
	active = FALSE;
	for(ii=0;ii<DSize;++ii){
		if((pbase_x_y2[ii] >= 1) && (active == FALSE)){
			active = TRUE;
			p_x_y2[pos] = ii * 0.01;
			pos += 1;
		}else if(pbase_x_y2[ii] < 1){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	
	// (52)
	cnt=0;
	active = FALSE;
	for(ii=0;ii<DSize;++ii){
		if((pbase_x_y3[ii] >= 0) && (active == FALSE)){
			active = TRUE;
			cnt += 1;
		}else if(pbase_x_y3[ii] < 0){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	p_x_y3 = &temp_dbl_buf1[0];								//calloc
	pos = 0;
	p_x_y3[pos] = 0;
	pos += 1;
	active = FALSE;
	for(ii=0;ii<DSize;++ii){
		if((pbase_x_y3[ii] >= 0) && (active == FALSE)){
			active = TRUE;
			p_x_y3[pos] = ii * 0.01;
			pos += 1;
		}else if(pbase_x_y3[ii] < 0){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	
	// (53)
	ptime = &temp_dbl_buf2[0];								//calloc
	for(ii=0;ii<cnt;++ii){
		ptime[ii] = p_x_y2[ii+1] - p_x_y3[ii+1];
	}
	
	// (54) pinterval[1] - pinterval[intervalsize] までが 呼吸間隔
	intervalsize = cnt;
	pinterval = &temp_dbl_buf1[0];
	for(ii=0;ii<intervalsize;++ii){
		pinterval[ii] = p_x_y2[ii+1] - p_x_y2[ii];
	}
	
	// (55)
	intervalsize2 = 0;
	for(ii=0;ii<intervalsize;++ii){
		// 3 <= x <= 5 以外の値を0にする
		if((3.0f <= pinterval[ii]) && (pinterval[ii] <= 5.0f)){
			// そのまま
			intervalsize2 += 1;
		}else{
			pinterval[ii] = 0.0f;
		}
	}
	
	pinterval2 = &temp_dbl_buf0[0];							//calloc
	pos = 0;
	snore_ = SNORE_OFF;
	for(ii=0;ii<intervalsize;++ii){
		if(pinterval[ii] != 0.0f){
			pinterval2[pos] = pinterval[ii];
			snore_ = SNORE_ON;
			pos+=1;
		}
	}
	
	// (56)
	snorecnt = intervalsize2 * 2;
}

// 状態を取得
UB get_state(void)
{
	UB ret = 0;
	ret |= ((apnea_ << 6) & 0xC0);
	ret |= (snore_ & 0x01);
	
	return ret;
}

/*==============================================================================*/
/* EOF */
