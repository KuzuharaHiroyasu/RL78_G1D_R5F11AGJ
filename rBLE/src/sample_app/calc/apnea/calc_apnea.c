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
double testdata[200] = {
2.13E-09	,
0.00175	,
0.001177	,
0.001252	,
0.001058	,
0.000919	,
0.000846	,
0.000692	,
0.00073	,
0.000747	,
0.00085	,
0.000778	,
0.000895	,
0.000532	,
0.000653	,
0.000663	,
0.000523	,
0.000371	,
0.000395	,
0.000442	,
0.000485	,
0.000409	,
0.000313	,
0.000385	,
0.000303	,
0.000424	,
0.000229	,
0.000331	,
0.000358	,
0.000647	,
0.003076	,
0.000976	,
0.003477	,
0.007533	,
0.004274	,
0.001841	,
0.001855	,
0.00491	,
0.005161	,
0.001699	,
0.006384	,
0.010133	,
0.007526	,
0.008098	,
0.008439	,
0.010036	,
0.020092	,
0.015954	,
0.022169	,
0.009964	,
0.016372	,
0.019976	,
0.024601	,
0.018608	,
0.011553	,
0.01562	,
0.012082	,
0.012445	,
0.015959	,
0.0131	,
0.013951	,
0.016809	,
0.011544	,
0.005925	,
0.010148	,
0.006467	,
0.000885	,
0.001294	,
0.00177	,
0.000786	,
0.000357	,
0.000285	,
0.00071	,
0.000555	,
0.000948	,
0.001025	,
0.001031	,
0.001049	,
0.001734	,
0.001295	,
0.001216	,
0.001169	,
0.000986	,
0.000954	,
0.001214	,
0.001439	,
0.001188	,
0.000676	,
0.000989	,
0.000804	,
0.000597	,
0.000459	,
0.000555	,
0.000554	,
0.000562	,
0.000414	,
0.000427	,
0.00052	,
0.000302	,
0.000405	,
0.000337	,
0.000435	,
0.000378	,
0.000407	,
0.000291	,
0.000372	,
0.000307	,
0.000267	,
0.000422	,
0.000382	,
0.000333	,
0.000508	,
0.000904	,
0.00225	,
0.001633	,
0.003458	,
0.006366	,
0.006567	,
0.004072	,
0.003156	,
0.002344	,
0.003789	,
0.002523	,
0.011624	,
0.011585	,
0.006825	,
0.012725	,
0.011001	,
0.011634	,
0.015454	,
0.011612	,
0.020081	,
0.023051	,
0.034028	,
0.016602	,
0.014532	,
0.016364	,
0.014282	,
0.020808	,
0.017404	,
0.011535	,
0.00961	,
0.011874	,
0.011798	,
0.010825	,
0.010439	,
0.007177	,
0.005823	,
0.005814	,
0.003869	,
0.002658	,
0.002349	,
0.000775	,
0.001449	,
0.000352	,
0.000411	,
0.000629	,
0.001271	,
0.001094	,
0.001115	,
0.001671	,
0.00093	,
0.001544	,
0.001069	,
0.001628	,
0.002096	,
0.001503	,
0.001267	,
0.000836	,
0.00161	,
0.001324	,
0.001352	,
0.001152	,
0.001076	,
0.000602	,
0.000541	,
0.000611	,
0.000587	,
0.000809	,
0.000512	,
0.000449	,
0.000522	,
0.000389	,
0.00049	,
0.000386	,
0.000418	,
0.00052	,
0.000369	,
0.000451	,
0.000506	,
0.000356	,
0.000421	,
0.000336	,
0.000354	,
0.000338	,
0.000341	,
0.000439	,
0.000349	,
0.000429	,
0.000311
};

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
//	calc_apnea(testdata, 200, 450, 0.0015f, 0.002f);
	
	// (48) - (56)
	calc_snore(ptest1, datasize, 0.0125f);
//	calc_snore(testdata, 200, 0.0125f);
	
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
	int datasize;
	double* prms;
	double* ppoint;
	double ave = 0.0f;
	int ii;
	int jj;
	int min=0;
	int loop=0;
	int apnea=0;
	
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
		for(ii=0;ii<loop;++ii){
			apnea = 0;
			for(jj=0;jj<9;++jj){
				apnea += ppoint[ii + jj];
			}
			if(apnea != 0){
				apnea_ = APNEA_NORMAL;
			}
		}
	}else{
		apnea_ = APNEA_NORMAL;
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
	double* pbase_x_y2;
	
	int cnt;
	int active;
	double* p_x_y2;
	int pos;
	
	int intervalsize;
	double* pinterval;
	
	int intervalsize2;
	// double snorecnt;
	
	int ii;
	// (48) = Param
	
	// 結果[いびき]を入れる箱
	pbase_x_y2 = &temp_dbl_buf1[0];							//calloc
	
	// (49)
	for(ii=0;ii<DSize;++ii){
		if(pData[ii] < Param){
			pbase_x_y2[ii] = 1;
		}else{
			pbase_x_y2[ii] = 0;
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
			p_x_y2[pos] = ii * 0.05;
			pos += 1;
		}else if(pbase_x_y2[ii] < 1){
			active = FALSE;
		}else{
			// なにもしない
		}
	}
	
	// (54) pinterval[1] - pinterval[intervalsize] までが 呼吸間隔
	intervalsize = cnt;
	pinterval = &temp_dbl_buf1[0];
	for(ii=1;ii<intervalsize;++ii){
		pinterval[ii] = p_x_y2[ii+1] - p_x_y2[ii];
	}
	
	// (55)
	snore_ = SNORE_OFF;
	intervalsize2 = 0;
	for(ii=0;ii<intervalsize;++ii){
		// 3 <= x <= 5 以外の値を0にする
		if((3.0f <= pinterval[ii]) && (pinterval[ii] <= 5.0f)){
			// そのまま
			snore_ = SNORE_ON;
			intervalsize2 += 1;
		}else{
			pinterval[ii] = 0.0f;
		}
	}
	
	// (56)
	// snorecnt = intervalsize2 * 2;
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
