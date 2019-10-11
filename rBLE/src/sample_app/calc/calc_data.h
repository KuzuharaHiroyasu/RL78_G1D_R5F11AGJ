/************************************************************************/
/* �V�X�e����   : RD8001�����`�F�b�J�[									*/
/* �t�@�C����   : calc_data.h											*/
/* �@�\         : ���Z���ʃf�[�^										*/
/* �ύX����     : 2018.01.15 Axia Soft Design �a�c ���ō쐬				*/
/* ���ӎ���     : �Ȃ�													*/
/************************************************************************/
#ifndef		_CALC_DATA_H_		/* ��d��`�h�~ */
#define		_CALC_DATA_H_

#include "sys.h"

/************************************************************/
/*					�}�N���萔��`							*/
/************************************************************/
//#define DATA_SIZE_SPO2	(128)
#define DATA_SIZE		(200)
#define DATA_SIZE_PRMS	(10)

// ���т����茋��
#define SNORE_OFF		0	// ���т��Ȃ�
#define SNORE_ON		1	// ���т�����

/************************************************************/
/* 						�^��`								*/
/************************************************************/


/************************************************************/
/*					�O���Q�Ɛ錾							*/
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
