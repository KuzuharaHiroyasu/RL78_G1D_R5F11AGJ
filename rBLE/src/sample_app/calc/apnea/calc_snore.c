/************************************************************************/
/* �V�X�e����   : RD8001�����`�F�b�J�[									*/
/* �t�@�C����   : calc_snore.c											*/
/* �@�\         : ���т����艉�Z����									*/
/* �ύX����     : 2018.07.25 Axia Soft Design mmura ���ō쐬			*/
/* ���ӎ���     : �Ȃ�													*/
/************************************************************************/
#include <stdlib.h>
#include "calc_snore.h"
#include "apnea_param.h"
#include "../calc_data.h"

/************************************************************/
/* �v���g�^�C�v�錾											*/
/************************************************************/
static int proc_on(int Pos, UB calc_type);
static int proc_off(int Pos, UB calc_type);
static void Save(UB calc_type);
static void Judge(UB calc_type);

/************************************************************/
/* �萔��`													*/
/************************************************************/
#define RIREKI				3
#define RIREKI_BREATH		3

#define BREATH_THRE			50

/************************************************************/
/* �ϐ���`													*/
/************************************************************/
static UB	SnoreFlg_; // ON�J�E���g�� or OFF�J�E���g��
static UB	SnoreCnt_; // ON�A����, OFF�A���� ���p
static B	SnoreTime_[RIREKI];
static UB	SnoreState_;		// ���т�
static UH	snore_sens = SNORE_PARAM_THRE_DURING;	// ���т����x
static UB	snoreJudgeOnFlg_;	// ���т�����ON�t���O
static UB	judgeSkipFlg_;	// ���т�����X�L�b�v�t���O

static B	BreathTime_[RIREKI_BREATH];
static UB	BreathState_;		// �ċz
static UH	breath_thre = BREATH_THRE;

/************************************************************************/
/* �֐�     : calculator_apnea											*/
/* �֐���   : ���ċz���艉�Z											*/
/* ����     : int *data : �g�`�f�[�^									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2017.07.12 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void calc_snore_init(void)
{
	int ii;
	
	SnoreFlg_ = OFF;
	SnoreCnt_ = 0;
	SnoreState_ = SNORE_OFF;
	BreathState_ = BREATH_OFF;
	snoreJudgeOnFlg_ = OFF;
	judgeSkipFlg_ = OFF;
	for(ii=0;ii<RIREKI;++ii){
		SnoreTime_[ii] = -1;
	}
	
	for(ii=0;ii<RIREKI_BREATH;++ii){
		BreathTime_[ii] = -1;
	}
}

/**************************** ���т����� ********************************/
/************************************************************************/
/* �֐�     : calc_proc													*/
/* �֐���   : ���т����艉�Z											*/
/* ����     : int *pData : �g�`�f�[�^									*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.25 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void calc_proc(const UH *pData, UB calc_type)
{
	int ii;
	int jj;
	int loop;
	int size;
	int pos=0;
	
	// 臒l�𒴂����񐔂̈ړ��݌v���Ƃ�
	loop = DATA_SIZE - SNORE_PARAM_SIZE;
	for(ii=0;ii<loop;++ii){
		thresholds_over_num[ii] = 0;
		size = ii+SNORE_PARAM_SIZE;
		for(jj=ii;jj<size;++jj){
			if(calc_type == CALC_TYPE_SNORE)
			{
				if(pData[jj] >= snore_sens){
					thresholds_over_num[ii] += 1;
				}
			}else if(calc_type == CALC_TYPE_BREATH){
				if(pData[jj] >= breath_thre){
					thresholds_over_num[ii] += 1;
				}
			}
		}
	}
	
	while(pos < loop){
		switch(SnoreFlg_){
		case ON:
			pos = proc_on(pos, calc_type);
			break;
		case OFF:
			pos = proc_off(pos, calc_type);
			break;
		default:
			calc_snore_init();
			return;
		}
	}
	

	if(snoreJudgeOnFlg_ == OFF)
	{
		if(calc_type == CALC_TYPE_SNORE)
		{
			SnoreState_ = SNORE_OFF;
		}else if(calc_type == CALC_TYPE_BREATH){
			BreathState_ = BREATH_OFF;
		}
	}

	judgeSkipFlg_ = OFF;
	snoreJudgeOnFlg_ = OFF;
}

/************************************************************************/
/* �֐�     : proc_on													*/
/* �֐���   : ���т�ON������											*/
/* ����     : int Data : �g�`�f�[�^										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.25 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
static int proc_on(int Pos, UB calc_type)
{
	int ii;
	int loop = DATA_SIZE - SNORE_PARAM_SIZE;
	int pos = loop;
	
	// OFF�m�肵�Ă���ꏊ����肷��
	for(ii=Pos;ii<loop;++ii){
		if(thresholds_over_num[ii] <= SNORE_PARAM_OFF_CNT){
			SnoreFlg_ = OFF;
			pos = ii;
			Save(calc_type);
			if(judgeSkipFlg_ == OFF)
			{
				Judge(calc_type);
			}
			break;
		}else{
			SnoreCnt_ += 1;
			if (SnoreCnt_ >= SNORE_PARAM_NORMAL_CNT) {
				Reset(calc_type);
			}			
		}
	}
	
	return pos;
}

/************************************************************************/
/* �֐�     : proc_off													*/
/* �֐���   : ���т�OFF������											*/
/* ����     : int Data : �g�`�f�[�^										*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.25 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
static int proc_off(int Pos, UB calc_type)
{
	int ii;
	int loop = DATA_SIZE - SNORE_PARAM_SIZE;
	int pos = loop;
	
	// ON�m�肵�Ă���ꏊ����肷��
	for(ii=Pos;ii<loop;++ii){
		if(thresholds_over_num[ii] >= SNORE_PARAM_ON_CNT){
			SnoreFlg_ = ON;
			SnoreCnt_ = 0;
			pos = ii;
			break;
		}else{
			SnoreCnt_ += 1;
			if(SnoreCnt_ >= SNORE_PARAM_NORMAL_CNT){
				Reset(calc_type);
			}
		}
	}
	
	return pos;
}

/************************************************************************/
/* �֐�     : Save														*/
/* �֐���   : ���т����Ԃ�ۑ�											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.25 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
static void Save(UB calc_type)
{
	int ii;
	
	if(calc_type == CALC_TYPE_SNORE)
	{
		if(SnoreState_ == SNORE_ON)
		{
			// ���т����̃m�C�Y��̓����̎C�ꉹ�Ȃǂ͔��肵�Ȃ�
			for(ii=0;ii<RIREKI-1;++ii){
				if(abs(SnoreCnt_-SnoreTime_[ii]) > SNORE_PARAM_GOSA){
					judgeSkipFlg_ = ON;
					SnoreCnt_ = 0;
					return;
				}
			}
			judgeSkipFlg_ = OFF;
		}
		
		for(ii=1;ii<RIREKI;++ii){
			SnoreTime_[RIREKI-ii] = SnoreTime_[RIREKI-ii-1];
		}
		SnoreTime_[0] = SnoreCnt_;
	}else if(calc_type == CALC_TYPE_BREATH){
		for(ii=1;ii<RIREKI_BREATH;++ii){
			BreathTime_[RIREKI_BREATH-ii] = BreathTime_[RIREKI_BREATH-ii-1];
		}
		BreathTime_[0] = SnoreCnt_;
	}
	SnoreCnt_ = 0;
}

/************************************************************************/
/* �֐�     : Judge														*/
/* �֐���   : ���т�����												*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.25 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
static void Judge(UB calc_type)
{
	int ii;
	
	if(calc_type == CALC_TYPE_SNORE)
	{
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
	}else if(calc_type == CALC_TYPE_BREATH){
		for(ii=0;ii<RIREKI_BREATH;++ii){
			if(BreathTime_[ii] == -1){
				BreathState_ = BREATH_OFF;
				return;
			}
		}
	
		for(ii=0;ii<RIREKI_BREATH-1;++ii){
			if(abs(BreathTime_[0]-BreathTime_[ii+1]) > SNORE_PARAM_GOSA){
				BreathState_ = BREATH_OFF;
				return;
			}
		}
		BreathState_ = BREATH_ON;
	}
	snoreJudgeOnFlg_ = ON;
}

/************************************************************************/
/* �֐�     : Reset														*/
/* �֐���   : �ʏ�ċz�ւ̕��A����										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.07.25 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void Reset(UB calc_type)
{
	int ii;
	
	if(calc_type == CALC_TYPE_SNORE)
	{
		for(ii=0;ii<RIREKI;++ii){
			SnoreTime_[ii] = -1;
		}
		SnoreState_ = SNORE_OFF;
	}else if(calc_type == CALC_TYPE_BREATH){
		for(ii=0;ii<RIREKI_BREATH;++ii){
			BreathTime_[ii] = -1;
		}
		BreathState_ = BREATH_OFF;
	}
}

UB calc_snore_get(void)
{
	return SnoreState_;
}


UB calc_breath_get(void)
{
	return BreathState_;
}

/************************************************************************/
/* �֐�     : set_snore_thre											*/
/* �֐���   : ���т�臒l�ݒ�											*/
/* ����     : UB sens													*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2018.08.05 oneA ���� �O��			���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void set_snore_sens( UB sens )
{
	switch ( sens )
	{
	case SNORE_SENS_WEAK:		// ��
		snore_sens = SNORE_PARAM_THRE_WERK;
		break;
	case SNORE_SENS_DURING:		// ��
		snore_sens = SNORE_PARAM_THRE_DURING;
		break;
	case SNORE_SENS_STRENGTH:	// ��
		snore_sens = SNORE_PARAM_THRE_STRENGTH;
		break;
	default:
		snore_sens = SNORE_PARAM_THRE_DURING;
		break;
	}
}

/*==============================================================================*/
/* EOF */
