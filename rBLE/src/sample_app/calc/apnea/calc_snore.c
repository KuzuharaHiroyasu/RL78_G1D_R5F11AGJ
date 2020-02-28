/************************************************************************/
/* システム名   : RD8001快眠チェッカー									*/
/* ファイル名   : calc_snore.c											*/
/* 機能         : いびき判定演算処理									*/
/* 変更履歴     : 2018.07.25 Axia Soft Design mmura 初版作成			*/
/* 注意事項     : なし													*/
/************************************************************************/
#include <stdlib.h>
#include "calc_snore.h"
#include "apnea_param.h"
#include "../calc_data.h"

/************************************************************/
/* プロトタイプ宣言											*/
/************************************************************/
static int proc_on(int Pos);
static int proc_off(int Pos);
static void Save(void);
static void Judge(void);

/************************************************************/
/* 定数定義													*/
/************************************************************/
#define RIREKI		3

/************************************************************/
/* 変数定義													*/
/************************************************************/
static UB	SnoreFlg_; // ONカウント中 or OFFカウント中
static UB	SnoreCnt_; // ON連続回数, OFF連続回数 兼用
static B	SnoreTime_[RIREKI];
static UB	SnoreState_;		// いびき
static UH	snore_sens = SNORE_PARAM_THRE_DURING;	// いびき感度

/************************************************************************/
/* 関数     : calculator_apnea											*/
/* 関数名   : 無呼吸判定演算											*/
/* 引数     : int *data : 波形データ									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2017.07.12 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void calc_snore_init(void)
{
	int ii;
	
	SnoreFlg_ = OFF;
	SnoreCnt_ = 0;
	SnoreState_ = SNORE_OFF;
	for(ii=0;ii<RIREKI;++ii){
		SnoreTime_[ii] = -1;
	}
}

/************************************************************************/
/* 関数     : calc_snore_proc											*/
/* 関数名   : いびき判定演算											*/
/* 引数     : int *pData : 波形データ									*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.25 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void calc_snore_proc(const UH *pData)
{
	int ii;
	int jj;
	int loop;
	int size;
	int pos=0;
	
	// 閾値を超えた回数の移動累計をとる
	loop = DATA_SIZE - SNORE_PARAM_SIZE;
	for(ii=0;ii<loop;++ii){
		thresholds_over_num[ii] = 0;
		size = ii+SNORE_PARAM_SIZE;
		for(jj=ii;jj<size;++jj){
			if(pData[jj] >= snore_sens){
				thresholds_over_num[ii] += 1;
			}
		}
	}
	
	while(pos < loop){
		switch(SnoreFlg_){
		case ON:
			pos = proc_on(pos);
			break;
		case OFF:
			pos = proc_off(pos);
			break;
		default:
			calc_snore_init();
			return;
		}
	}
}

/************************************************************************/
/* 関数     : proc_on													*/
/* 関数名   : いびきON時処理											*/
/* 引数     : int Data : 波形データ										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.25 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
static int proc_on(int Pos)
{
	int ii;
	int loop = DATA_SIZE - SNORE_PARAM_SIZE;
	int pos = loop;
	
	// OFF確定している場所を特定する
	for(ii=Pos;ii<loop;++ii){
		if(thresholds_over_num[ii] <= SNORE_PARAM_OFF_CNT){
			SnoreFlg_ = OFF;
			pos = ii;
			Save();
			Judge();
			break;
		}else{
			SnoreCnt_ += 1;
			if (SnoreCnt_ >= SNORE_PARAM_NORMAL_CNT) {
				Reset();
			}			
		}
	}
	
	return pos;
}

/************************************************************************/
/* 関数     : proc_off													*/
/* 関数名   : いびきOFF時処理											*/
/* 引数     : int Data : 波形データ										*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.25 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
static int proc_off(int Pos)
{
	int ii;
	int loop = DATA_SIZE - SNORE_PARAM_SIZE;
	int pos = loop;
	
	// ON確定している場所を特定する
	for(ii=Pos;ii<loop;++ii){
		if(thresholds_over_num[ii] >= SNORE_PARAM_ON_CNT){
			SnoreFlg_ = ON;
			SnoreCnt_ = 0;
			pos = ii;
			break;
		}else{
			SnoreCnt_ += 1;
			if(SnoreCnt_ >= SNORE_PARAM_NORMAL_CNT){
				Reset();
			}
		}
	}
	
	return pos;
}

/************************************************************************/
/* 関数     : Save														*/
/* 関数名   : いびき時間を保存											*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.25 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
static void Save(void)
{
	int ii;
	
	for(ii=1;ii<RIREKI;++ii){
		SnoreTime_[RIREKI-ii] = SnoreTime_[RIREKI-ii-1];
	}
	SnoreTime_[0] = SnoreCnt_;
	SnoreCnt_ = 0;
}

/************************************************************************/
/* 関数     : Judge														*/
/* 関数名   : いびき判定												*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.25 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
static void Judge(void)
{
	int ii;
	
	for(ii=0;ii<RIREKI;++ii){
		if(SnoreTime_[ii] == -1){
			SnoreState_ = SNORE_OFF;
			return;
		}
	}
	
	for(ii=0;ii<RIREKI-1;++ii){
		if(abs(SnoreTime_[0]-SnoreTime_[ii+1]) > SNORE_PARAM_GOSA){
			SnoreState_ = SNORE_OFF;
			return;
		}
	}
	SnoreState_ = SNORE_ON;
}

/************************************************************************/
/* 関数     : Reset														*/
/* 関数名   : 通常呼吸への復帰処理										*/
/* 引数     : なし														*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.07.25 Axia Soft Design mmura	初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void Reset(void)
{
	int ii;
	
	for(ii=0;ii<RIREKI;++ii){
		SnoreTime_[ii] = -1;
	}
	SnoreState_ = SNORE_OFF;
}


UB calc_snore_get(void)
{
	return SnoreState_;
}

/************************************************************************/
/* 関数     : set_snore_thre											*/
/* 関数名   : いびき閾値設定											*/
/* 引数     : UB sens													*/
/* 戻り値   : なし														*/
/* 変更履歴 : 2018.08.05 oneA 葛原 弘安			初版作成				*/
/************************************************************************/
/* 機能 :																*/
/************************************************************************/
/* 注意事項 :															*/
/* なし																	*/
/************************************************************************/
void set_snore_sens( UB sens )
{
	switch ( sens )
	{
	case SNORE_SENS_WEAK:		// 弱
		snore_sens = SNORE_PARAM_THRE_WERK;
		break;
	case SNORE_SENS_DURING:		// 中
		snore_sens = SNORE_PARAM_THRE_DURING;
		break;
	case SNORE_SENS_STRENGTH:	// 強
		snore_sens = SNORE_PARAM_THRE_STRENGTH;
		break;
	default:
		snore_sens = SNORE_PARAM_THRE_DURING;
		break;
	}
}

/*==============================================================================*/
/* EOF */
