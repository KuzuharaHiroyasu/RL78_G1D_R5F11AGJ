/************************************************************************/
/* �V�X�e����   : RD8001�����`�F�b�J�[									*/
/* �t�@�C����   : calc_apnea.c											*/
/* �@�\         : ���ċz���艉�Z����									*/
/* �ύX����     : 2018.01.11 Axia Soft Design �a�c ���ō쐬				*/
/* ���ӎ���     : �Ȃ�													*/
/************************************************************************/
#include <math.h>
#include "calc_apnea.h"
#include "apnea_param.h"
#include "../calc_data.h"

/************************************************************/
/* �v���g�^�C�v�錾											*/
/************************************************************/
void calc_apnea(const double* pData, int DSize, int Param1, double Param2, double Param3, double Param4);
void edgeWeighted_MovAve(const double* dc_, double* movave_, UW datasize);
void heartBeat_Remov(double* dc_, double* movave_, const UH* data_snore, UW datasize);
void init_before_raw(void);
extern int	peak_modify_a(const double in_data[] , H in_res[] , double ot_data[] , double ot_hz[] , int size , double delta, double th);
extern void peak_vallay_a(const double in[] , H ot[] , int size, int width , int peak );

/************************************************************/
/* �萔��`													*/
/************************************************************/

/************************************************************/
/* �ϐ���`													*/
/************************************************************/
static int		apnea_ = APNEA_NONE;	// �ċz���
double  before_raw[PREVIOUS_DATA_NUM];

/************************************************************************/
/* �֐�     : calculator_apnea											*/
/* �֐���   : ���ċz���艉�Z											*/
/* ����     : int *data : �g�`�f�[�^(�ċz��)									*/
/*          : int len   : �g�`�f�[�^�T�C�Y								*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2017.07.12 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void calculator_apnea(const UH *data_apnea, const UH *data_snore)
{
	UW datasize;						// �g�`�f�[�^�̃o�C�g��
	double* ptest1;
	double* movave_;
	int ii, jj;
	
	//�f�[�^�T�C�Y����
	datasize = DATA_SIZE_APNEA;
	
	// ���Z�p�f�[�^�ڍs
	ptest1 = &temp_dbl_buf0[0];	//calloc
	movave_ = &temp_dbl_buf2[0];	
	
	for(ii=0; ii < datasize; ++ii){
		ptest1[ii] = data_apnea[ii];
	}
	
	// �G�b�W�����ړ�����
	edgeWeighted_MovAve(ptest1, movave_, datasize);
	
	// �S������
	heartBeat_Remov(ptest1, movave_, data_snore, datasize);
	
	// ���݂̃f�[�^�̖�����ۊǁi�v�f197, 198, 199�j
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
/* �֐�     : calc_apnea												*/
/* �֐���   : ���ċz���Z����											*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 2017.07.12 Axia Soft Design mmura	���ō쐬				*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
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
	
	// (41) ... �g�p���Ă��Ȃ����ߏȗ�
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
					// �ʏ�ċz�Ɣ��f�������_�Ŕ�����
					apnea_ = APNEA_NORMAL;
					break;
				}
			}
		}
	}else{
		apnea_ = APNEA_NORMAL;
	}
	
/* //��ċz�����ċz�Ɣ��肷�邽��
	// ���S���ċz�̔���
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
/* �֐�     : edgeWeighted_MovAve										*/
/* �֐���   : �G�b�W�����ړ�����										*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 															*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void edgeWeighted_MovAve(const double* dc_, double* movave_, UW datasize)
{
	int ii;
	
	// �G�b�W�����ړ�����
	for (ii = 0; ii < datasize; ++ii) {
		movave_[ii] = 0;

		if ((ii - 2) >= 0)
		{//�v�f2�ȏ�
			if ((ii + 2) <= (datasize - 1))
			{// �v�f197�܂�
				movave_[ii] += dc_[ii - 2] * 0.1;
				movave_[ii] += dc_[ii - 1];
				movave_[ii] += dc_[ii] * 20;
				movave_[ii] += dc_[ii + 1];
				movave_[ii] += dc_[ii + 2] * 0.1;
				movave_[ii] /= (double)APNEA_PARAM_AVE_NUM;
			}
		}
		else if (before_raw[ii] != -1)
		{// �ߋ��f�[�^�����邩
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
/* �֐�     : heartBeat_Remov											*/
/* �֐���   : �S������													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 															*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void heartBeat_Remov(double* dc_, double* movave_, const UH* data_snore, UW datasize)
{
	boolean before_under = FALSE;
	boolean after_under = FALSE;
	int i, j;
	int pastDataNum = 0;
	
	// �S������
	for (i = 0; i < datasize; i++)
	{
		if (i <= datasize - 4)
		{
			if (movave_[i] >= MAX_THRESHOLD && data_snore[i] <= MIN_THRESHOLD)
			{// �G�b�W����1000�ȏ�A���т���100�ȉ�
				if (i < 3)
				{
				// 0 �` 2�܂�
					// �O150ms�m�F
					if (i == 0)
					{// �f�[�^�z��̐擪
						if (before_raw[0] < MIN_THRESHOLD || before_raw[1] < MIN_THRESHOLD || before_raw[2] < MIN_THRESHOLD)
						{
							before_under = TRUE;
							pastDataNum = 3;
						}
					}
					else if (i == 1)
					{// �f�[�^�z��2�Ԗ�
						if (dc_[0] < MIN_THRESHOLD || before_raw[1] < MIN_THRESHOLD || before_raw[2] < MIN_THRESHOLD)
						{
							before_under = TRUE;
							pastDataNum = 2;
						}
					}
					else if (i == 2)
					{// �f�[�^�z��3�Ԗ�
						if (dc_[0] < MIN_THRESHOLD || dc_[1] < MIN_THRESHOLD || before_raw[2] < MIN_THRESHOLD)
						{
							before_under = TRUE;
							pastDataNum = 1;
						}
					}
					// �O�ɊY���f�[�^������ꍇ�͌�150ms�m�F
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
				// 3 �` 196�܂�
					// �O��150ms�m�F
					for (j = 1; j < 4; j++)
					{
						// �O150ms�m�F
						if (dc_[i - j] < MIN_THRESHOLD)
						{
							before_under = TRUE;
						}
						// ��150ms�m�F
						if (dc_[i + j] < MIN_THRESHOLD)
						{
							after_under = TRUE;
						}
					}
				}

				if (before_under && after_under)
				{// �O��ɊY���ӏ�������ꍇ
					for (j = -3 + pastDataNum; j < 4; j++)
					{// �O��150ms��0�ɂ���
						dc_[i + j] = 0;
					}
				}
				// �t���O���Z�b�g
				before_under = FALSE;
				after_under = FALSE;
				pastDataNum = 0;
			}
		}
	}
}

/************************************************************************/
/* �֐�     : get_state													*/
/* �֐���   : ��Ԏ擾													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 															*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
UB get_state(void)
{
	UB ret = 0;
	ret |= ((apnea_ << 6) & 0xC0);
	
	return ret;
}

/************************************************************************/
/* �֐�     : init_before_raw													*/
/* �֐���   : �O�̃f�[�^�̏�����													*/
/* ����     : �Ȃ�														*/
/* �߂�l   : �Ȃ�														*/
/* �ύX���� : 															*/
/************************************************************************/
/* �@�\ :																*/
/************************************************************************/
/* ���ӎ��� :															*/
/* �Ȃ�																	*/
/************************************************************************/
void init_before_raw(void)
{ // ����J�n���ɌĂяo���I(�ꏊ����)
	int i;
	for(i = 0; i < PREVIOUS_DATA_NUM; i++)
	{
		before_raw[i] = -1;
	}
}

/*==============================================================================*/
/* EOF */
