/************************************************************************/
/* �V�X�e����   : RD8001�����`�F�b�J�[									*/
/* �t�@�C����   : calc_apnea.h											*/
/* �@�\         : ���ċz���艉�Z����									*/
/* �ύX����     : 2017.07.12 Axia Soft Design mmura	���ō쐬			*/
/* ���ӎ���     : �Ȃ�													*/
/************************************************************************/
#ifndef		_CALC_APNEA_H_		/* ��d��`�h�~ */
#define		_CALC_APNEA_H_

#include "sys.h"

/************************************************************/
/*					�}�N���萔��`							*/
/************************************************************/
// ���ċz���茋��
#define APNEA_NORMAL	0	// �ُ�Ȃ�
#define APNEA_WARN		1	// ���ċz��Ԃ���
#define APNEA_ERROR		2	// ���S���ċz
#define APNEA_NONE		3	// ����G���[

// ���т����茋��
#define SNORE_OFF		0	// ���т��Ȃ�
#define SNORE_ON		1	// ���т�����

/************************************************************/
/* 						�^��`								*/
/************************************************************/


/************************************************************/
/*					�O���Q�Ɛ錾							*/
/************************************************************/
extern void	calculator_apnea(const UH *data);
extern UB	get_state(void);

#endif	/*_CALC_APNEA_H_*/
/************************************************************/
/* END OF TEXT												*/
/************************************************************/