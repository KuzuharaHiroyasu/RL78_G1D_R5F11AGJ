/************************************************************************/
/* �V�X�e����   : RD8001�����`�F�b�J�[									*/
/* �t�@�C����   : calc_snore.h											*/
/* �@�\         : ���т����艉�Z����									*/
/* �ύX����     : 2018.07.25 Axia Soft Design mmura	���ō쐬			*/
/* ���ӎ���     : �Ȃ�													*/
/************************************************************************/
#ifndef		_CALC_SNORE_H_		/* ��d��`�h�~ */
#define		_CALC_SNORE_H_

#include "sys.h"

/************************************************************/
/*					�}�N���萔��`							*/
/************************************************************/

/************************************************************/
/* 						�^��`								*/
/************************************************************/
typedef enum{
	SNORE_SENS_WEAK = 0,		// ��
	SNORE_SENS_DURING,			// ��
	SNORE_SENS_STRENGTH,		// ��
	SNORE_SENS_MAX
}SNORE_SENS;

typedef enum{
	CALC_TYPE_SNORE = 0,
	CALC_TYPE_BREATH,
	CALC_TYPE_MAX
}CALC_TYPE;

/************************************************************/
/*					�O���Q�Ɛ錾							*/
/************************************************************/
extern void	calc_snore_init(void);
extern void	calc_proc(const UH *pData, UB calc_type);
extern UB	calc_snore_get(void);
extern void set_snore_sens( UB sens );
extern void Reset(UB calc_type);

extern UB	calc_breath_get(void);

#endif	/*_CALC_SNORE_H_*/
/************************************************************/
/* END OF TEXT												*/
/************************************************************/
