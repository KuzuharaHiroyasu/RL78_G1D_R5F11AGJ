/********************************************************************************/
/* �V�X�e����   : RD8001														*/
/* �t�@�C����   : vibration.h														*/
/* �@�\         : 		*/
/* �ύX����     : 		���ō쐬				*/
/* ���ӎ���     : �Ȃ�															*/
/********************************************************************************/

#ifndef	__VIBRATION_INC__
#define	__VIBRATION_INC__

#include	"sys.h"


#define VIB_CTL		P0_bit.no1
#define VIB_ENA		P0_bit.no0
#define VIB_ORBIT	3	// �o�C�u����


typedef enum{
	VIB_MODE_ON,		// ON(�P������)
	VIB_MODE_OFF,		// OFF(�P������)
	VIB_MODE_WEAK,		// ��
	VIB_MODE_DURING,	// ��
	VIB_MODE_STRENGTH,	// ��
	VIB_MODE_STANDBY,	// �ҋ@���[�h�ڍs��
	VIB_MODE_SENSING,	// �Z���V���O���[�h�ڍs��
	VIB_MODE_MAX
}VIB_MODE;

/******************/
/*  �O���Q�Ɛ錾  */
/******************/
extern void vib_start(UH vib_timer);
extern void set_vib(VIB_MODE mode);

#endif // __VIBRATION_INC__
